// =============================================================================
//  tests/test_MiniFtpEngine.cpp -- KYECFTP MiniFtpEngine acceptance test
//
//  Proves the from-scratch TNMFTP FTP engine (KYECFTP/MiniFtpEngine.{h,cpp})
//  works OFFLINE with NO real FTP server and NO real socket, by driving it
//  against an in-process SIM-mode "fake FTP server" script (see
//  MiniFtpEngine.h's "SIM / TEST SURFACE" section for the exact hook
//  contract this test relies on: SetSimServerHook's "CTRL-CONNECT" / "CTRL"
//  / "DATA-OPEN" / "DATA-CLOSE" channel tags).
//
//  Two layers, per the task brief's guidance:
//    [A] ISOLATED logic: ClassifyFtpReply / BuildFtpCommandLine /
//        ParsePasvReply -- pure functions, no engine/socket involved.
//    [B] END-TO-END over the Sim-server-hook double: Connect (success +
//        3 distinct failure shapes) / CWD / NLST / RETR / STOR / DELE /
//        CurrentDir(PWD).
//
//  No external test framework (matches tests/test_ContactForce.cpp style):
//  a tiny PASS/FAIL harness, non-zero exit on any failure.
// =============================================================================
#include "KYECFTP/MiniFtpEngine.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                   \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// =============================================================================
//  [A] Isolated logic: reply classification, command framing, PASV parsing.
// =============================================================================
static void test_isolated_logic()
{
    printf("\n[A] Isolated logic: ClassifyFtpReply / BuildFtpCommandLine / ParsePasvReply\n");

    CHECK(Nmftp::ClassifyFtpReply(150) == Nmftp::rcPreliminary,      "150 -> rcPreliminary");
    CHECK(Nmftp::ClassifyFtpReply(220) == Nmftp::rcSuccess,          "220 -> rcSuccess");
    CHECK(Nmftp::ClassifyFtpReply(230) == Nmftp::rcSuccess,          "230 -> rcSuccess");
    CHECK(Nmftp::ClassifyFtpReply(226) == Nmftp::rcSuccess,          "226 -> rcSuccess");
    CHECK(Nmftp::ClassifyFtpReply(331) == Nmftp::rcIntermediate,     "331 -> rcIntermediate");
    CHECK(Nmftp::ClassifyFtpReply(425) == Nmftp::rcTransientFailure, "425 -> rcTransientFailure");
    CHECK(Nmftp::ClassifyFtpReply(530) == Nmftp::rcPermanentFailure, "530 -> rcPermanentFailure");
    CHECK(Nmftp::ClassifyFtpReply(550) == Nmftp::rcPermanentFailure, "550 -> rcPermanentFailure");
    CHECK(Nmftp::ClassifyFtpReply(502) == Nmftp::rcPermanentFailure, "502 -> rcPermanentFailure");
    CHECK(Nmftp::ClassifyFtpReply(0)   == Nmftp::rcMalformed,        "0   -> rcMalformed");
    CHECK(Nmftp::ClassifyFtpReply(999) == Nmftp::rcMalformed,        "999 -> rcMalformed");
    CHECK(Nmftp::ClassifyFtpReply(-5)  == Nmftp::rcMalformed,        "-5  -> rcMalformed");

    AnsiString l1 = Nmftp::BuildFtpCommandLine("NLST");
    CHECK(std::strcmp(l1.c_str(), "NLST\r\n") == 0, "BuildFtpCommandLine(no arg) == \"NLST\\r\\n\"");

    AnsiString l2 = Nmftp::BuildFtpCommandLine("CWD", "/pub/incoming");
    CHECK(std::strcmp(l2.c_str(), "CWD /pub/incoming\r\n") == 0,
          "BuildFtpCommandLine(verb,arg) == \"CWD /pub/incoming\\r\\n\"");

    AnsiString ip; int port = 0;
    bool ok1 = Nmftp::ParsePasvReply(
        "227 Entering Passive Mode (127,0,0,1,195,80).", ip, port);
    CHECK(ok1, "ParsePasvReply parses a standard 227 reply");
    CHECK(std::strcmp(ip.c_str(), "127.0.0.1") == 0, "  -> ip == 127.0.0.1");
    CHECK(port == 195 * 256 + 80,                     "  -> port == 195*256+80 == 50000");

    // RFC 959 does not mandate the parentheses -- tolerate their absence.
    AnsiString ip2; int port2 = 0;
    bool ok2 = Nmftp::ParsePasvReply(
        "227 Entering Passive Mode 10,0,0,5,4,1", ip2, port2);
    CHECK(ok2, "ParsePasvReply tolerates a reply with no parentheses");
    CHECK(std::strcmp(ip2.c_str(), "10.0.0.5") == 0, "  -> ip == 10.0.0.5");
    CHECK(port2 == 4 * 256 + 1,                        "  -> port == 4*256+1 == 1025");

    AnsiString ip3; int port3 = 0;
    bool ok3 = Nmftp::ParsePasvReply("500 Syntax error", ip3, port3);
    CHECK(!ok3, "ParsePasvReply fails cleanly on a non-PASV reply");
}

// =============================================================================
//  [B] Sim-mode fake FTP server double.
//
//  Uses TNMFTP::SetSimServerHook (see MiniFtpEngine.h) to script server
//  replies synchronously as MiniFtpEngine sends each control-channel command
//  (or opens/closes the data channel). One shared FIFO queue drives every
//  control-channel reply the server ever emits across a whole test scenario
//  (banner, login, PASV, 150-preliminary, 226-completion, ...) in the exact
//  order MiniFtpEngine will read them; a separate single-shot field supplies
//  whatever bytes the fake server "sends" over the data channel for a
//  RETR/NLST (STOR needs none -- the ENGINE is the one writing to the data
//  channel for an upload).
// =============================================================================
struct FakeFtpServer {
    Nmftp::TNMFTP*        engine;
    std::deque<std::string> ctrlReplies;   // FIFO, one entry per control-channel line the server will "send"
    std::string              dataPayload;    // pushed once at the next DATA-OPEN, then cleared
    std::vector<std::string> sentLines;      // every raw command line MiniFtpEngine sent (for framing assertions)

    explicit FakeFtpServer(Nmftp::TNMFTP* e) : engine(e) {}

    void operator()(const char* channel, const AnsiString& rawLine)
    {
        std::string ch(channel);

        if (ch == "CTRL")
            sentLines.push_back(rawLine.c_str());

        if (ch == "CTRL-CONNECT" || ch == "CTRL" || ch == "DATA-CLOSE")
        {
            if (!ctrlReplies.empty())
            {
                const std::string& reply = ctrlReplies.front();
                engine->DebugControlSocket()->Socket->SimPushReceive(
                    reply.data(), static_cast<int>(reply.size()));
                ctrlReplies.pop_front();
            }
        }
        else if (ch == "DATA-OPEN")
        {
            if (!dataPayload.empty())
            {
                engine->DebugDataSocket()->Socket->SimPushReceive(
                    dataPayload.data(), static_cast<int>(dataPayload.size()));
                dataPayload.clear();
            }
        }
    }
};

// --- event-capture scaffolding shared by the scenario tests -----------------
struct EventLog {
    int  connectCount, connectionFailedCount, authFailedCount, disconnectCount;
    int  successCount, failureCount, unsupportedCount;
    Nmftp::TCmdType lastSuccessCmd, lastFailureCmd, lastUnsupportedCmd;
    std::vector<std::string> listedItems;
    int  transStartCount, transStopCount;
    int  errorCount;
    int  lastErrorCode;
    std::string lastErrorMsg;

    EventLog()
        : connectCount(0), connectionFailedCount(0), authFailedCount(0), disconnectCount(0)
        , successCount(0), failureCount(0), unsupportedCount(0)
        , lastSuccessCmd(Nmftp::cmdChangeDir), lastFailureCmd(Nmftp::cmdChangeDir)
        , lastUnsupportedCmd(Nmftp::cmdChangeDir)
        , transStartCount(0), transStopCount(0)
        , errorCount(0), lastErrorCode(0)
    {}
};

static void WireEvents(Nmftp::TNMFTP& eng, EventLog& log)
{
    eng.OnConnect          = [&log](TObject*) { ++log.connectCount; };
    eng.OnConnectionFailed = [&log](TObject*) { ++log.connectionFailedCount; };
    eng.OnAuthenticationFailed = [&log](bool& handled) { ++log.authFailedCount; handled = true; };
    eng.OnDisconnect       = [&log](TObject*) { ++log.disconnectCount; };
    eng.OnSuccess          = [&log](Nmftp::TCmdType c) { ++log.successCount; log.lastSuccessCmd = c; };
    eng.OnFailure          = [&log](bool& handled, Nmftp::TCmdType c) { ++log.failureCount; log.lastFailureCmd = c; handled = true; };
    eng.OnUnSupportedFunction = [&log](Nmftp::TCmdType c) { ++log.unsupportedCount; log.lastUnsupportedCmd = c; };
    eng.OnListItem         = [&log](AnsiString item) { log.listedItems.push_back(item.c_str()); };
    eng.OnTransactionStart = [&log](TObject*) { ++log.transStartCount; };
    eng.OnTransactionStop  = [&log](TObject*) { ++log.transStopCount; };
    eng.OnError            = [&log](TComponent*, vclcompat::Word code, AnsiString msg)
    {
        ++log.errorCount;
        log.lastErrorCode = code;
        log.lastErrorMsg  = msg.c_str();
    };
}

// -----------------------------------------------------------------------------
//  [B1] Full happy-path scenario: Connect -> CWD -> NLST -> RETR -> STOR ->
//       DELE -> CurrentDir(PWD).
// -----------------------------------------------------------------------------
static void test_scenario_happy_path()
{
    printf("\n[B1] Sim-server happy path: Connect/CWD/NLST/RETR/STOR/DELE/PWD\n");

    Nmftp::TNMFTP eng(0);
    EventLog log;
    WireEvents(eng, log);

    FakeFtpServer srv(&eng);
    eng.SetSimServerHook(std::ref(srv));

    eng.Host = "ftp.example.test";
    eng.UserID = "testuser";
    eng.Password = "testpass";
    eng.Port = 21;
    eng.Vendor = Nmftp::NMOS_AUTO;
    eng.TimeOut = 5000;
    eng.Passive = true;

    // --- Connect: banner -> USER -> 331 -> PASS -> 230 ----------------------
    srv.ctrlReplies.push_back("220 Welcome to FakeFTP\r\n");
    srv.ctrlReplies.push_back("331 Password required\r\n");
    srv.ctrlReplies.push_back("230 Logged in\r\n");

    eng.Connect();

    CHECK(eng.Connected == true,      "Connect(): Connected becomes true");
    CHECK(log.connectCount == 1,      "Connect(): OnConnect fired exactly once");
    CHECK(log.connectionFailedCount == 0, "Connect(): OnConnectionFailed did NOT fire");
    CHECK(log.authFailedCount == 0,   "Connect(): OnAuthenticationFailed did NOT fire");
    CHECK(eng.LastErrorNo == 0,       "Connect(): LastErrorNo == 0 on success");
    CHECK(srv.sentLines.size() == 2, "Connect(): exactly 2 commands sent (USER, PASS)");
    if (srv.sentLines.size() == 2)
    {
        CHECK(srv.sentLines[0] == "USER testuser", "  -> sent[0] == \"USER testuser\" (exact framing)");
        CHECK(srv.sentLines[1] == "PASS testpass", "  -> sent[1] == \"PASS testpass\" (exact framing)");
    }

    // --- ChangeDir -----------------------------------------------------------
    srv.ctrlReplies.push_back("250 Directory changed\r\n");
    eng.ChangeDir("/incoming");
    CHECK(log.successCount == 1 && log.lastSuccessCmd == Nmftp::cmdChangeDir,
          "ChangeDir(): OnSuccess(cmdChangeDir) fired");
    CHECK(srv.sentLines.back() == "CWD /incoming", "ChangeDir(): sent \"CWD /incoming\"");

    // --- Nlist: PASV -> 227 -> NLST -> 150 -> [data: two names] -> 226 ------
    srv.ctrlReplies.push_back("227 Entering Passive Mode (127,0,0,1,4,1).\r\n"); // port 4*256+1=1025
    srv.ctrlReplies.push_back("150 Opening data connection\r\n");
    srv.ctrlReplies.push_back("226 Transfer complete\r\n");
    srv.dataPayload = "alpha.zip\r\nbeta.zip\r\n";

    eng.Nlist();

    CHECK(log.listedItems.size() == 2, "Nlist(): exactly 2 OnListItem firings");
    if (log.listedItems.size() == 2)
    {
        CHECK(log.listedItems[0] == "alpha.zip", "  -> item[0] == \"alpha.zip\"");
        CHECK(log.listedItems[1] == "beta.zip",  "  -> item[1] == \"beta.zip\"");
    }
    CHECK(log.lastSuccessCmd == Nmftp::cmdNList, "Nlist(): OnSuccess(cmdNList) fired last");
    CHECK(log.transStartCount == 1 && log.transStopCount == 1,
          "Nlist(): OnTransactionStart/Stop each fired once");
    CHECK(srv.sentLines.back() == "NLST", "Nlist(): sent bare \"NLST\" (no argument)");

    // --- Download: PASV -> 227 -> RETR -> 150 -> [data: file bytes] -> 226 --
    const char* kDownloadContent = "JAM0000 payload bytes \x01\x02\x03 end";
    srv.ctrlReplies.push_back("227 Entering Passive Mode (127,0,0,1,4,2).\r\n"); // port 1026
    srv.ctrlReplies.push_back("150 Opening data connection\r\n");
    srv.ctrlReplies.push_back("226 Transfer complete\r\n");
    srv.dataPayload = kDownloadContent;

    const char* kLocalDownloadPath = "miniftp_test_download.tmp";
    eng.Download("/incoming/JAM0000.dat", kLocalDownloadPath);

    CHECK(log.lastSuccessCmd == Nmftp::cmdDownload, "Download(): OnSuccess(cmdDownload) fired");
    CHECK(srv.sentLines.back() == "RETR /incoming/JAM0000.dat",
          "Download(): sent \"RETR /incoming/JAM0000.dat\"");
    {
        std::FILE* f = std::fopen(kLocalDownloadPath, "rb");
        CHECK(f != 0, "Download(): local file was created");
        if (f)
        {
            char buf[256]; size_t n = std::fread(buf, 1, sizeof(buf), f);
            std::fclose(f);
            bool contentMatch = n == std::strlen(kDownloadContent) &&
                                 std::memcmp(buf, kDownloadContent, n) == 0;
            CHECK(contentMatch, "Download(): local file content == exact server payload bytes");
        }
        std::remove(kLocalDownloadPath);
    }

    // --- Upload: PASV -> 227 -> STOR -> 150 -> [engine sends bytes] -> 226 --
    const char* kUploadLocalPath = "miniftp_test_upload.tmp";
    const char* kUploadContent = "UPLOAD-CONTENT-1234567890";
    {
        std::FILE* f = std::fopen(kUploadLocalPath, "wb");
        std::fwrite(kUploadContent, 1, std::strlen(kUploadContent), f);
        std::fclose(f);
    }

    srv.ctrlReplies.push_back("227 Entering Passive Mode (127,0,0,1,4,3).\r\n"); // port 1027
    srv.ctrlReplies.push_back("150 Ready for STOR\r\n");
    srv.ctrlReplies.push_back("226 Transfer complete\r\n");

    // The DATA-CLOSE hook fires while pImpl_'s data socket is still the LIVE
    // one and has NOT been deleted yet (see MiniFtpEngine.h note) -- capture
    // exactly what MiniFtpEngine sent via DebugDataSocket() right there.
    std::vector<char> capturedUploadBytes;
    bool capturedOnce = false;
    Nmftp::TNMFTP::TFtpSimServerHook baseHook = std::ref(srv);
    eng.SetSimServerHook(
        [&](const char* channel, const AnsiString& line)
        {
            baseHook(channel, line);
            if (std::strcmp(channel, "DATA-CLOSE") == 0 && !capturedOnce)
            {
                Scktcomp::TClientSocket* d = eng.DebugDataSocket();
                if (d) { capturedUploadBytes = d->Socket->SimTxBuffer(); capturedOnce = true; }
            }
        });

    eng.Upload(kUploadLocalPath, "/incoming/upload.dat");

    CHECK(log.lastSuccessCmd == Nmftp::cmdUpload, "Upload(): OnSuccess(cmdUpload) fired");
    CHECK(srv.sentLines.back() == "STOR /incoming/upload.dat",
          "Upload(): sent \"STOR /incoming/upload.dat\"");
    bool uploadBytesMatch =
        capturedUploadBytes.size() == std::strlen(kUploadContent) &&
        std::memcmp(&capturedUploadBytes[0], kUploadContent, capturedUploadBytes.size()) == 0;
    CHECK(uploadBytesMatch, "Upload(): data channel received the EXACT local file bytes");
    std::remove(kUploadLocalPath);

    eng.SetSimServerHook(std::ref(srv));   // restore plain hook for the rest of the scenario

    // --- Delete ----------------------------------------------------------------
    srv.ctrlReplies.push_back("250 File deleted\r\n");
    eng.Delete("/incoming/upload.dat");
    CHECK(log.lastSuccessCmd == Nmftp::cmdDelete, "Delete(): OnSuccess(cmdDelete) fired");
    CHECK(srv.sentLines.back() == "DELE /incoming/upload.dat",
          "Delete(): sent \"DELE /incoming/upload.dat\"");

    // --- CurrentDir (property read -> live PWD round-trip) ------------------
    srv.ctrlReplies.push_back("257 \"/incoming\" is the current directory\r\n");
    AnsiString cur = eng.CurrentDir;
    CHECK(std::strcmp(cur.c_str(), "/incoming") == 0,
          "CurrentDir getter parses the 257-quoted path from a live PWD");
    CHECK(log.lastSuccessCmd == Nmftp::cmdCurrentDir,
          "CurrentDir getter fires OnSuccess(cmdCurrentDir) (evidence: golden's own TCmdType enumerates it)");
    CHECK(srv.sentLines.back() == "PWD", "CurrentDir getter sent bare \"PWD\"");
}

// -----------------------------------------------------------------------------
//  [B2] Connect() failure shapes: transport-level, banner/timeout, auth (530).
// -----------------------------------------------------------------------------
static void test_scenario_connect_failures()
{
    printf("\n[B2] Connect() failure shapes\n");

    // --- (a) No banner ever arrives (SIM: nothing queued -> immediate give-up)
    {
        Nmftp::TNMFTP eng(0);
        EventLog log;
        WireEvents(eng, log);
        FakeFtpServer srv(&eng);
        eng.SetSimServerHook(std::ref(srv));   // deliberately script NOTHING

        eng.Host = "ftp.example.test";
        eng.Connect();

        CHECK(eng.Connected == false,          "no-banner: Connected stays false");
        CHECK(log.connectionFailedCount == 1,  "no-banner: OnConnectionFailed fires");
        CHECK(log.connectCount == 0,           "no-banner: OnConnect does NOT fire");
    }

    // --- (b) Immediate 530 on USER (no 331 intermediate) -> auth failure ----
    {
        Nmftp::TNMFTP eng(0);
        EventLog log;
        WireEvents(eng, log);
        FakeFtpServer srv(&eng);
        eng.SetSimServerHook(std::ref(srv));

        eng.Host = "ftp.example.test";
        eng.UserID = "baduser";
        eng.Password = "badpass";

        srv.ctrlReplies.push_back("220 Welcome\r\n");
        srv.ctrlReplies.push_back("530 Login incorrect\r\n");

        eng.Connect();

        CHECK(eng.Connected == false,        "530-on-USER: Connected stays false");
        CHECK(log.authFailedCount == 1,      "530-on-USER: OnAuthenticationFailed fires");
        CHECK(log.connectionFailedCount == 0,"530-on-USER: OnConnectionFailed does NOT fire (auth-specific path)");
        CHECK(eng.LastErrorNo == 530,        "530-on-USER: LastErrorNo == 530");
        CHECK(log.errorCount == 1 && log.lastErrorCode == 530,
              "530-on-USER: OnError also fires with code 530");
    }

    // --- (c) 331 then 530 on PASS -> auth failure ---------------------------
    {
        Nmftp::TNMFTP eng(0);
        EventLog log;
        WireEvents(eng, log);
        FakeFtpServer srv(&eng);
        eng.SetSimServerHook(std::ref(srv));

        eng.Host = "ftp.example.test";
        eng.UserID = "user";
        eng.Password = "wrongpass";

        srv.ctrlReplies.push_back("220 Welcome\r\n");
        srv.ctrlReplies.push_back("331 Need password\r\n");
        srv.ctrlReplies.push_back("530 Login incorrect\r\n");

        eng.Connect();

        CHECK(eng.Connected == false,   "331-then-530: Connected stays false");
        CHECK(log.authFailedCount == 1, "331-then-530: OnAuthenticationFailed fires");
    }
}

// -----------------------------------------------------------------------------
//  [B3] Command-level failure classification: OnFailure vs OnUnSupportedFunction.
// -----------------------------------------------------------------------------
static void test_scenario_command_failures()
{
    printf("\n[B3] Command-level failure classification (OnFailure vs OnUnSupportedFunction)\n");

    Nmftp::TNMFTP eng(0);
    EventLog log;
    WireEvents(eng, log);
    FakeFtpServer srv(&eng);
    eng.SetSimServerHook(std::ref(srv));

    eng.Host = "ftp.example.test";
    srv.ctrlReplies.push_back("220 Welcome\r\n");
    srv.ctrlReplies.push_back("230 Logged in (no password needed)\r\n");
    eng.Connect();
    CHECK(eng.Connected, "setup: Connect() succeeds with a 230-direct (no 331) login");

    // A generic 550 permanent failure -> OnFailure, NOT OnUnSupportedFunction.
    srv.ctrlReplies.push_back("550 No such directory\r\n");
    eng.ChangeDir("/does/not/exist");
    CHECK(log.failureCount == 1 && log.lastFailureCmd == Nmftp::cmdChangeDir,
          "550 on CWD -> OnFailure(cmdChangeDir)");
    CHECK(log.unsupportedCount == 0, "550 on CWD -> OnUnSupportedFunction did NOT fire");

    // A literal 502 -> OnUnSupportedFunction, NOT OnFailure.
    srv.ctrlReplies.push_back("502 Command not implemented\r\n");
    eng.MakeDirectory("/tmp/newdir");
    CHECK(log.unsupportedCount == 1 && log.lastUnsupportedCmd == Nmftp::cmdMakeDir,
          "502 on MKD -> OnUnSupportedFunction(cmdMakeDir)");
    CHECK(log.failureCount == 1, "502 on MKD -> OnFailure count unchanged (still 1 from the CWD case)");
}

// =============================================================================
int main()
{
    std::setvbuf(stdout, 0, _IONBF, 0);   // unbuffered: preserve output order across abnormal exits
    printf("=================================================================\n");
    printf(" test_MiniFtpEngine : KYECFTP from-scratch TNMFTP FTP engine shim\n");
    printf("=================================================================\n");

    test_isolated_logic();
    test_scenario_happy_path();
    test_scenario_connect_failures();
    test_scenario_command_failures();

    printf("\n-----------------------------------------------------------------\n");
    printf(" RESULT: %d passed, %d failed\n", g_pass, g_fail);
    printf("-----------------------------------------------------------------\n");
    return (g_fail == 0) ? 0 : 1;
}
