// =============================================================================
//  test_testertcp_socket.cpp  --  W5 VERIFY: Interface/TesterTCP_Socket
//                                  (bounded socket-management subset of the
//                                  golden TfTesterTCP OS/Open-Short Tester
//                                  TCP/IP channel)
//
//  Translation wave: W5 (comms), Phase 2 (Automation/TesterTCP), bounded unit.
//  Author: AI(W5-Final-TesterTCPSocket) 20260711
//
//  PURPOSE
//  -------
//  Proves Interface/TesterTCP_Socket.cpp LINKS and its ACTIVE logic behaves
//  per golden Interface/TesterTCP.cpp (READ-ONLY reference), driving the
//  Scktcomp::TClientSocket Sim mode (no real Tester attached -- exactly the
//  offline posture every other vclcompat/ClientSocket.h consumer test uses).
//
//  NO EXTERNAL FRAMEWORK: a tiny check harness prints PASS/FAIL per case and
//  a final summary, returning non-zero on ANY failure (same style as
//  tests/test_interfacesys.cpp / tests/test_clientsocket_ext.cpp).
//
//  FILESYSTEM NOTE: AddTCPIPCommunicationLog's golden body durably appends to
//  a per-hour log file under `asTestTCPIPLogPath` (production default
//  "D:\HT9045_Log\Test_TCPIP", common.cpp:180 -- a mutable global, not
//  const). This test REASSIGNS it to a small scratch folder next to the test
//  binary BEFORE exercising anything, so running this test never touches the
//  real production log tree (verified: without this override, an earlier
//  throwaway run of this same logic did write real files under
//  D:\HT9045_Log\Test_TCPIP -- cleaned up manually once, hence this override
//  in the committed test).
// =============================================================================
#include "Interface/TesterTCP_Socket.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static int g_pass = 0;
static int g_fail = 0;

static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected) { std::printf("PASS  %-58s got=%d\n", name, (int)got); ++g_pass; }
    else { std::printf("FAIL  %-58s got=%d exp=%d\n", name, (int)got, (int)expected); ++g_fail; }
}

static void check_s(const char* name, const std::string& got, const std::string& expected)
{
    if (got == expected) { std::printf("PASS  %-58s got=\"%s\"\n", name, got.c_str()); ++g_pass; }
    else { std::printf("FAIL  %-58s got=\"%s\" exp=\"%s\"\n", name, got.c_str(), expected.c_str()); ++g_fail; }
}

// -----------------------------------------------------------------------
// AI(W906-TesterTCPTimer) 20260720: bEcho / iBin[4][8] are golden main.cpp
// (untranslated) globals, `extern`-declared only inside individual .cpp TUs
// (atester_shims.cpp / Interface/TesterTCP_Socket.cpp / atester.cpp /
// Interface/InterfaceSYS.cpp) -- no header exposes them. Same pattern
// tests/test_interfacesys.cpp already follows for these same globals. This
// test needs read/write access for the T-G/T-H/T-L/T-M assertions below, so
// it declares its own local `extern` bindings to the real ht9045_sm-owned
// definitions (atester_shims.cpp) instead of redefining them.
// -----------------------------------------------------------------------
extern bool bEcho;
extern unsigned int iBin[4][8];

// -----------------------------------------------------------------------
// W906-TesterTCPTimer wave helpers (T-A..T-N below): push one full "wire
// arrival" (matching golden's own unaddressed split-command reassembly gap --
// each SimPushReceive call becomes exactly one SocketTCPIPReceiveList entry,
// same as one real TCP read event), and read back whatever
// SendTCPIPCommand wrote to the Sim wire since the last SimClearTx().
// -----------------------------------------------------------------------
static void w906_push(const std::string& wireMsg)
{
    TesterTCPSocket_ClientSocket->Socket->SimPushReceive(wireMsg.data(), static_cast<int>(wireMsg.size()));
}

static std::string w906_wire()
{
    const std::vector<char>& tx = TesterTCPSocket_ClientSocket->Socket->SimTxBuffer();
    return std::string(tx.begin(), tx.end());
}

static std::string w906_s(const AnsiString& a)
{
    return std::string(a.str());
}

int main()
{
    std::printf("=== Interface/TesterTCP_Socket translation verification ===\n");
    std::printf("(Sim ClientSocket, no real Tester attached; asTestTCPIPLogPath\n");
    std::printf(" redirected to a local scratch folder for the duration of this test)\n\n");

    // Redirect the durable log path away from the real production tree.
    asTestTCPIPLogPath = ".\\_test_scratch_testertcpsocket";

    InitialOK        = true;
    LastSet.iTester  = ON_LINE;
    TestIF_File.asTester_Address = "127.0.0.1";
    TestIF_File.iTester_Port     = 9999;

    TesterTCPSocket_Init();

    // -------------------------------------------------------------------
    // 1. Initial state (golden ctor defaults, TesterTCP.cpp:63-65).
    // -------------------------------------------------------------------
    check_b("ctor: bEnableTCPIPChannelConnect=true", TesterTCPSocket.bEnableTCPIPChannelConnect, true);
    check_b("ctor: bConnectOK=false",                TesterTCPSocket.bConnectOK, false);
    check_b("ctor: bTCPError=false",                 TesterTCPSocket.bTCPError, false);
    check_b("ctor: not connected yet",               TesterTCPSocket_ClientSocket->IsActiveNow(), false);

    // -------------------------------------------------------------------
    // 2. TimerTCPIPConnectTimer: golden's Count1 starts at 20 and only opens
    //    once Count1>30 -- drive it exactly across that threshold.
    // -------------------------------------------------------------------
    for (int i = 0; i < 15; ++i) TesterTCPSocket_TimerTCPIPConnectTimer();
    check_b("timer: Sim connect succeeded after threshold", TesterTCPSocket_ClientSocket->IsActiveNow(), true);
    check_b("OnConnect: bConnectOK set true",                TesterTCPSocket.bConnectOK, true);
    check_b("OnConnect: bConnect set false",                 TesterTCPSocket.bConnect, false);

    // -------------------------------------------------------------------
    // 3. Timer is a no-op reconnect while already Active at the SAME
    //    Address/Port (golden's own "do nothing" branch, TesterTCP.cpp:202-206).
    // -------------------------------------------------------------------
    TesterTCPSocket_TimerTCPIPConnectTimer();
    check_b("timer: stays connected (same addr/port -> no-op)", TesterTCPSocket_ClientSocket->IsActiveNow(), true);

    // -------------------------------------------------------------------
    // 4. SendTCPIPCommand while online pushes bytes to the wire (Sim tx sink).
    // -------------------------------------------------------------------
    TesterTCPSocket_ClientSocket->Socket->SimClearTx();
    TesterTCPSocket_SendTCPIPCommand(0, "hello", "PING");
    check_b("SendTCPIPCommand: SendText wrote to sim tx",
            TesterTCPSocket_ClientSocket->Socket->SimTxBuffer().size() > 0, true);
    {
        const std::vector<char>& tx = TesterTCPSocket_ClientSocket->Socket->SimTxBuffer();
        check_s("SendTCPIPCommand: wire bytes == \"PING\\r\\n\"", std::string(tx.begin(), tx.end()), "PING\r\n");
    }

    // -------------------------------------------------------------------
    // 5. ClientSocket_TCPIPRead: a Sim-pushed reply is drained, queued, and
    //    the \r\n-stripped copy becomes sTCPIPRecevieData (golden TesterTCP.cpp:339-340).
    // -------------------------------------------------------------------
    const char* reply = "PONG\r\n";
    TesterTCPSocket_ClientSocket->Socket->SimPushReceive(reply, static_cast<int>(std::strlen(reply)));
    {
        AnsiString firstQueued = TesterTCPSocket.SocketTCPIPReceiveList->Strings[0];
        check_s("OnRead: queued raw reply", std::string(firstQueued.str()), "PONG\r\n");
    }

    // -------------------------------------------------------------------
    // 6. SendTCPIPCommand's golden OFF_LINE-mode quirk (Msg2 self-duplicated,
    //    NOT merely appended -- preserved verbatim, see the .cpp comment).
    // -------------------------------------------------------------------
    {
        LastSet.iTester = OFF_LINE;
        TesterTCPSocket.mmTCPIPCommLog.Lines.Count = 0;   // isolate this case's Add() count
        TesterTCPSocket_SendTCPIPCommand(0, "ignored", "abc");
        // Msg2 becomes "abc"+"abc"+"OffLine" = "abcabcOffLine" -> Log = "Send, abcabcOffLine"
        // (AddTCPIPCommunicationLog itself does not expose the built string
        // directly; the OFF_LINE early-return before any SendText is the
        // externally-observable half of this case -- verified next.)
        check_b("OFF_LINE: AddTCPIPCommunicationLog still ran once", TesterTCPSocket.mmTCPIPCommLog.Lines.Count == 1, true);
        LastSet.iTester = ON_LINE;
    }

    // -------------------------------------------------------------------
    // 7. Disconnect click path.
    // -------------------------------------------------------------------
    TesterTCPSocket_btTCPIP_DisconnectClick();
    check_b("DisconnectClick: socket closed",       TesterTCPSocket_ClientSocket->IsActiveNow(), false);
    check_b("OnDisconnect: bConnectOK cleared",     TesterTCPSocket.bConnectOK, false);
    check_b("DisconnectClick: channel disabled",    TesterTCPSocket.bEnableTCPIPChannelConnect, false);
    check_b("DisconnectClick: timer-enabled stand-in cleared", TesterTCPSocket.bTimerTCPIPConnectEnabled, false);

    // While disabled, the timer must NOT reconnect no matter how many ticks.
    for (int i = 0; i < 40; ++i) TesterTCPSocket_TimerTCPIPConnectTimer();
    check_b("timer: stays disconnected while channel disabled", TesterTCPSocket_ClientSocket->IsActiveNow(), false);

    // -------------------------------------------------------------------
    // 8. Re-enable + reconnect via ConnectClick + timer threshold again.
    // -------------------------------------------------------------------
    TesterTCPSocket_btTCPIP_ConnectClick();
    check_b("ConnectClick: channel re-enabled",         TesterTCPSocket.bEnableTCPIPChannelConnect, true);
    check_b("ConnectClick: timer-enabled stand-in set",  TesterTCPSocket.bTimerTCPIPConnectEnabled, true);
    for (int i = 0; i < 32; ++i) TesterTCPSocket_TimerTCPIPConnectTimer();
    check_b("timer: reconnects after ConnectClick", TesterTCPSocket_ClientSocket->IsActiveNow(), true);

    // -------------------------------------------------------------------
    // 9. btTCPIP_TriggerClick: extract-calc-core -- caller supplies the
    //    edTCPIP_CMD->Text read, forwards to SendTCPIPCommand(0," ",cmd).
    // -------------------------------------------------------------------
    TesterTCPSocket_ClientSocket->Socket->SimClearTx();
    TesterTCPSocket_btTCPIP_TriggerClick("MANUALCMD");
    {
        const std::vector<char>& tx = TesterTCPSocket_ClientSocket->Socket->SimTxBuffer();
        check_s("TriggerClick: wire bytes == \"MANUALCMD\\r\\n\"", std::string(tx.begin(), tx.end()), "MANUALCMD\r\n");
    }

    // -------------------------------------------------------------------
    // 10. ClientSocket_TCPIPError: closes the socket, records ErrorMessage,
    //     sets bTCPError -- exercised directly (this IS the assigned
    //     OnError handler, but calling it directly is equally faithful: the
    //     handler reads no VCL widget, only Sender/ErrorCode).
    // -------------------------------------------------------------------
    {
        TObject* senderAsSocket = TesterTCPSocket_ClientSocket;
        int errCode = 10061; // WSAECONNREFUSED, arbitrary non-zero for the check below
        TesterTCPSocket_OnError(senderAsSocket, TesterTCPSocket_ClientSocket->Socket, eeConnect, errCode);
        check_b("OnError: bTCPError set true",        TesterTCPSocket.bTCPError, true);
        check_b("OnError: ErrorCode zeroed by handler", errCode == 0, true);
        check_b("OnError: socket closed",              TesterTCPSocket_ClientSocket->IsActiveNow(), false);
    }

    // -------------------------------------------------------------------
    // 11. TimerTCPIPConnectTimer's own bTCPError-triggered ShowMyMessage
    //     branch (golden TesterTCP.cpp:196-200) -- just verify it clears the
    //     flag and does not crash/hang (ShowMyMessage is a log-only Sim stub).
    // -------------------------------------------------------------------
    TesterTCPSocket_TimerTCPIPConnectTimer();
    check_b("timer: bTCPError cleared after firing", TesterTCPSocket.bTCPError, false);

    // -------------------------------------------------------------------
    // 12. AddTCPIPCommunicationLog's local memo stand-in: Count increments,
    //     and rolls over (Clear()) past the golden 2000-line threshold.
    // -------------------------------------------------------------------
    {
        int before = TesterTCPSocket.mmTCPIPCommLog.Lines.Count;
        TesterTCPSocket_AddTCPIPCommunicationLog(0, "manual log line");
        check_b("AddTCPIPCommunicationLog: local memo Count++", TesterTCPSocket.mmTCPIPCommLog.Lines.Count == before + 1, true);

        TesterTCPSocket.mmTCPIPCommLog.Lines.Count = 2001;   // force the rollover branch
        TesterTCPSocket_AddTCPIPCommunicationLog(0, "line after rollover");
        check_b("AddTCPIPCommunicationLog: rollover Clear() then Add()", TesterTCPSocket.mmTCPIPCommLog.Lines.Count == 1, true);
    }

    // =====================================================================
    // W906-TesterTCPTimer wave: TimerProcessTCPDataTimer + SimulateBin
    // (T-A..T-N). See DESIGN_TesterTCP_TimerProcessTCPDataTimer.md section 6.
    // =====================================================================
    std::printf("\n--- W906-TesterTCPTimer: TimerProcessTCPDataTimer + SimulateBin ---\n");

    // Common preamble: an independent, connected baseline -- the sections
    // above may have left the Sim socket disconnected/errored (step 10's
    // OnError test closes it).
    InitialOK       = true;
    LastSet.iTester = ON_LINE;
    TesterTCPSocket.bTCPError = false;
    TesterTCPSocket.SocketTCPIPReceiveList->Clear();
    if (!TesterTCPSocket_ClientSocket->IsActiveNow())
    {
        TesterTCPSocket_ClientSocket->Address = TestIF_File.asTester_Address;
        TesterTCPSocket_ClientSocket->Port    = TestIF_File.iTester_Port;
        TesterTCPSocket_ClientSocket->Open();
    }
    check_b("[T-setup] Sim socket connected", TesterTCPSocket_ClientSocket->IsActiveNow(), true);

    // -------------------------------------------------------------------
    // [T-A] pump guard: InitialOK / bTCPError (golden TesterTCP.cpp:358-363).
    // bTimerRun==true cannot be exercised externally -- GOLDEN QUIRK #1: no
    // statement in the whole golden function ever sets it true (vestigial
    // reentrancy guard) -- noted here, not tested (there is no way to arm it).
    // -------------------------------------------------------------------
    {
        bool savedInitialOK = InitialOK;

        // Direct enqueue (bypassing SimPushReceive/OnRead, which has its OWN
        // separate InitialOK gate) isolates the TIMER's own guard.
        TesterTCPSocket.SocketTCPIPReceiveList->Add("WORKFILE_OK\r\n");

        InitialOK = false;
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-A-a] InitialOK=false: message not consumed", TesterTCPSocket.SocketTCPIPReceiveList->Count == 1, true);

        InitialOK = true;
        TesterTCPSocket.bTCPError = true;
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-A-b] bTCPError=true: message not consumed", TesterTCPSocket.SocketTCPIPReceiveList->Count == 1, true);

        TesterTCPSocket.bTCPError = false;
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-A-c] both guards clear: message consumed", TesterTCPSocket.SocketTCPIPReceiveList->Count == 0, true);

        InitialOK = savedInitialOK;
    }

    // -------------------------------------------------------------------
    // [T-B] one tick == one queued command -- golden only ever processes
    // Strings[0] per call (TesterTCP.cpp:368-532); golden's own :347 comment
    // self-admits split/multi-command handling is NOT implemented.
    // -------------------------------------------------------------------
    {
        w906_push("WORKFILE_OK\r\n");
        w906_push("WORKFILE_OK\r\n");
        check_b("[T-B] two pushes queued as two entries", TesterTCPSocket.SocketTCPIPReceiveList->Count == 2, true);
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-B] one tick drains exactly one", TesterTCPSocket.SocketTCPIPReceiveList->Count == 1, true);
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-B] second tick drains the other", TesterTCPSocket.SocketTCPIPReceiveList->Count == 0, true);
    }

    // -------------------------------------------------------------------
    // [T-C] BARCODE? -- facade ctor defaults. GOLDEN QUIRK #13: tBarCodeList's
    // last slot is "0;" (ctor +=";" on a fresh "0"), not plain "0".
    // -------------------------------------------------------------------
    {
        std::string expected = "BARCODE:";
        for (int i = 0; i < 32; i++)
        {
            expected += (i == 31) ? "0;" : "0";
            expected += (i == 31) ? ";" : ",";
        }
        expected += "\r\n";

        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("BARCODE?\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_s("[T-C] BARCODE? reply == ctor-default barcode string (quirk #13 tail \"0;\";)", w906_wire(), expected);
        check_s("[T-C] asTCPIPBarCode[0] == \"0\"",   w906_s(asTCPIPBarCode[0]),  "0");
        check_s("[T-C] asTCPIPBarCode[31] == \"0;\"", w906_s(asTCPIPBarCode[31]), "0;");
        check_s("[T-C] labOcr[5].Caption == \"0\"",   w906_s(TesterTCPSocket.labOcr[5].Caption), "0");
    }

    // -------------------------------------------------------------------
    // [T-D] ECHOCODE: three states. (a) is an ECHOCODE ORACLE established by
    // REAL EXECUTION, not assumption (per this wave's task brief): golden's
    // ECHOCODE: parser (TesterTCP.cpp:407) finds Str2.Pos(";") -- the FIRST
    // semicolon in the whole comma-joined body -- and asTCPIPBarCode[31]'s own
    // stored value is "0;" (quirk #13), which embeds a semicolon. Echoing the
    // exact current barcode data back therefore has its own 32nd token's
    // terminator land INSIDE that "0;" token instead of at the message's real
    // end, so the parsed 32nd token comes back truncated to bare "0" -- a
    // MISMATCH against the stored "0;". Confirmed by running this exact case:
    // golden's own ctor-default barcode data can never successfully
    // ECHOCODE itself back on the very first try.
    // -------------------------------------------------------------------
    {
        // (a) exact echo of the current asTCPIPBarCode[] (established by T-C).
        std::string cmdA = "ECHOCODE:";
        for (int i = 0; i < 32; i++)
        {
            cmdA += w906_s(asTCPIPBarCode[i]);
            cmdA += (i == 31) ? ";" : ",";
        }
        cmdA += "\r\n";
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push(cmdA);
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_s("[T-D-a] ECHOCODE: exact-echo of ctor-default barcodes -> NG (ECHOCODE ORACLE: embedded ';' in \"0;\" truncates token 32)",
                w906_wire(), std::string("ECHOCODENG\r\n"));

        // (b) one token deliberately different.
        std::string cmdB = "ECHOCODE:";
        for (int i = 0; i < 32; i++)
        {
            cmdB += (i == 3) ? "9" : w906_s(asTCPIPBarCode[i]);
            cmdB += (i == 31) ? ";" : ",";
        }
        cmdB += "\r\n";
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push(cmdB);
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_s("[T-D-b] ECHOCODE: one differing token -> NG", w906_wire(), std::string("ECHOCODENG\r\n"));

        // (c) malformed -- only 5 tokens (Count!=32 branch, Jimmychiu 20231002).
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("ECHOCODE:1,2,3,4,5;\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_s("[T-D-c] ECHOCODE: only 5 tokens (Count!=32) -> NG", w906_wire(), std::string("ECHOCODENG\r\n"));
    }

    // -------------------------------------------------------------------
    // [T-E] Test Arm?
    // -------------------------------------------------------------------
    {
        int savedArm = iWhichArmDown;

        iWhichArmDown = 1;
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("Test Arm?\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_s("[T-E] iWhichArmDown=1 -> TestArm:1", w906_wire(), std::string("TestArm:1\r\n"));

        iWhichArmDown = 2;
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("Test Arm?\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_s("[T-E] iWhichArmDown=2 -> TestArm:2", w906_wire(), std::string("TestArm:2\r\n"));

        iWhichArmDown = 0;
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("Test Arm?\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_s("[T-E] iWhichArmDown=0 -> TestArm:0", w906_wire(), std::string("TestArm:0\r\n"));

        iWhichArmDown = savedArm;
    }

    // -------------------------------------------------------------------
    // [T-F] TempArm? -- wrapper GOLDEN QUIRK #14: unconditional "%s\r" means
    // TempArm? ALWAYS replies (minimum "TempArm:") once TestIF.iTestType==
    // TCP_IP_MODE, even with an empty seam.
    // -------------------------------------------------------------------
    {
        int savedTestType = TestIF.iTestType;

        TestIF.iTestType = TCP_IP_MODE;
        // AI(W906-FW3-WA) 20260817: RECALIBRATED. The sim-seam stub
        // PERSITETemperatureStrings was retired -- the REAL body (golden
        // Command.cpp:945-1482) landed in Command.cpp. Offline it is
        // deterministic: iArm stays -1 (no IndexStatus, no fContact), the
        // single-site arm takes golden's asSite="SINGLESITE_" branch
        // (golden :1006, port Command.cpp:1228) and the temp cell reads
        // "NULL" -> "SINGLESITE_NULL_" (measured before this edit, then
        // matched to the golden text). Quirk #14 (unconditional "%s\r"
        // wrapper) is still what [T-F-a] proves -- now with a REAL payload.
        fMain->W906_PERSITETemperatureStrings_Sim = "";
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("TempArm?\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_s("[T-F-a] TempArm? real body offline -> TempArm:SINGLESITE_NULL_ (golden :1006, quirk #14 wrapper)",
                w906_wire(), std::string("TempArm:SINGLESITE_NULL_\r\n"));

        // [T-F-b] repurposed: the retired seam must be DEAD. Seeding it must
        // not change the reply -- this line fails if anyone resurrects the
        // stub. NOT COVERED since this recalibration: driving a specific
        // per-site temperature string through the real 537-line body (needs
        // seeded UN150Read[]/IndexStatus state; deferred to the temp/GPIB
        // surface wave).
        fMain->W906_PERSITETemperatureStrings_Sim = "25.0,25.1";
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("TempArm?\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_s("[T-F-b] retired seam is dead: seeding it changes nothing",
                w906_wire(), std::string("TempArm:SINGLESITE_NULL_\r\n"));

        TestIF.iTestType = (savedTestType == TCP_IP_MODE) ? 0 : savedTestType;   // force a NON-TCP_IP_MODE value for (c)
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("TempArm?\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-F-c] TempArm? non-TCP_IP_MODE -> wrapper does not write asTCPIPTemperature -> no reply",
                w906_wire().empty(), true);

        TestIF.iTestType = savedTestType;
        fMain->W906_PERSITETemperatureStrings_Sim = "";
    }

    // -------------------------------------------------------------------
    // [T-G] BINON: geometry oracle -- row-reversal Strings[3-i], hardcoded *8
    // column width, single-digit SubString (GOLDEN QUIRK #8); embedded-CRLF
    // echo reply (GOLDEN QUIRK #7).
    // -------------------------------------------------------------------
    {
        int savedSiteMap[MAX_SOCKET_ROW][MAX_SOCKET_COL];
        for (int i = 0; i < MAX_SOCKET_ROW; i++)
            for (int j = 0; j < MAX_SOCKET_COL; j++)
            {
                savedSiteMap[i][j] = TestIF.iSiteMap[i][j];
                TestIF.iSiteMap[i][j] = i * MAX_SOCKET_COL + j + 1;   // identity, 1-based
            }

        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("BINON:11111111,22222222,33333333,44444444;\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();

        bool row0ok = true, row1ok = true, row2ok = true, row3ok = true;
        for (int j = 0; j < MAX_SOCKET_COL; j++)
        {
            if (iBin[0][j] != 4) row0ok = false;
            if (iBin[1][j] != 3) row1ok = false;
            if (iBin[2][j] != 2) row2ok = false;
            if (iBin[3][j] != 1) row3ok = false;
        }
        check_b("[T-G] iBin row0 (from Strings[3-0]=\"44444444\") == 4 (quirk #8 row-reversal)", row0ok, true);
        check_b("[T-G] iBin row1 (from Strings[3-1]=\"33333333\") == 3", row1ok, true);
        check_b("[T-G] iBin row2 (from Strings[3-2]=\"22222222\") == 2", row2ok, true);
        check_b("[T-G] iBin row3 (from Strings[3-3]=\"11111111\") == 1", row3ok, true);
        check_s("[T-G] tTestResult->Strings[0] == \"4\" (site1 == i=0,j=0)",  w906_s(fMain->tTestResult->Strings[0]),  "4");
        check_s("[T-G] tTestResult->Strings[31] == \"1\" (site32 == i=3,j=7)", w906_s(fMain->tTestResult->Strings[31]), "1");
        check_s("[T-G] plSite[0].Caption == \"4\"", w906_s(TesterTCPSocket.plSite[0].Caption), "4");
        check_s("[T-G] ECHO: reply carries embedded CRLF (quirk #7) + SendTCPIPCommand's own wire trailer",
                w906_wire(), std::string("ECHO:BINON:11111111,22222222,33333333,44444444;\r\n\r\n"));

        // Second sub-check: iSiteMap[0][0]=0 -> the else-branch zeroes iBin[0][0].
        TestIF.iSiteMap[0][0] = 0;
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("BINON:11111111,22222222,33333333,44444444;\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-G] iSiteMap[0][0]=0 -> iBin[0][0]==0 (else branch)", iBin[0][0] == 0, true);

        for (int i = 0; i < MAX_SOCKET_ROW; i++)
            for (int j = 0; j < MAX_SOCKET_COL; j++)
                TestIF.iSiteMap[i][j] = savedSiteMap[i][j];
    }

    // -------------------------------------------------------------------
    // [T-H] ECHOOK -- GOLDEN QUIRK #16: logs a blank " " line (the same
    // idiom AddTCPIPCommunicationLog special-cases), no wire reply.
    // -------------------------------------------------------------------
    {
        bool savedEcho    = bEcho;
        bool savedTimeOut = bTimeOutForNoFullSite;
        bEcho = false;
        bTimeOutForNoFullSite = true;

        int before = TesterTCPSocket.mmTCPIPCommLog.Lines.Count;
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("ECHOOK\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();

        check_b("[T-H] ECHOOK sets bEcho=true",                     bEcho, true);
        check_b("[T-H] ECHOOK clears bTimeOutForNoFullSite",        bTimeOutForNoFullSite, false);
        check_b("[T-H] ECHOOK sends no wire reply",                 w906_wire().empty(), true);
        check_b("[T-H] log +2 (OnRead's own Recv line + ECHOOK's blank-line quirk #16)",
                TesterTCPSocket.mmTCPIPCommLog.Lines.Count == before + 2, true);

        bEcho = savedEcho;
        bTimeOutForNoFullSite = savedTimeOut;
    }

    // -------------------------------------------------------------------
    // [T-I] GETOSSETUP -- only the LITERAL "\r\n" substring is stripped
    // (StringReplace on the exact 2-char pattern), so embedded lone "\n"
    // survive into sList->SetText, giving 4 tokens; sList->Delete(0) drops
    // the "GETOSSETUP" token itself before CommaText joins the rest.
    // -------------------------------------------------------------------
    {
        fMain->SVID1190_OSSetup = "";
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("GETOSSETUP\nWF1\n2.5\n3.3\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_s("[T-I] SVID1190_OSSetup == \"WF1,2.5,3.3\" (CommaText join)", w906_s(fMain->SVID1190_OSSetup), "WF1,2.5,3.3");
        check_b("[T-I] GETOSSETUP sends no wire reply", w906_wire().empty(), true);
    }

    // -------------------------------------------------------------------
    // [T-J] WORKFILE_OK / WORKFILE_FAIL / unknown prefix -- all consumed,
    // none reply, none crash (no terminal `else` in the golden dispatch chain).
    // -------------------------------------------------------------------
    {
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("WORKFILE_OK\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-J-a] WORKFILE_OK: consumed, no reply", w906_wire().empty(), true);
        check_b("[T-J-a] WORKFILE_OK: queue drained", TesterTCPSocket.SocketTCPIPReceiveList->Count == 0, true);

        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("WORKFILE_FAIL\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-J-b] WORKFILE_FAIL: consumed, no reply (ShowMyMessage is log-only)", w906_wire().empty(), true);
        check_b("[T-J-b] WORKFILE_FAIL: queue drained", TesterTCPSocket.SocketTCPIPReceiveList->Count == 0, true);

        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("GARBAGE123\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-J-c] unknown prefix: consumed, no reply", w906_wire().empty(), true);
        check_b("[T-J-c] unknown prefix: queue drained", TesterTCPSocket.SocketTCPIPReceiveList->Count == 0, true);
    }

    // -------------------------------------------------------------------
    // [T-K] malformed input -- PORT DEVIATION #1 lock-in: vclcompat
    // TStringList out-of-range reads return "" (no exception), so the port
    // quietly treats malformed messages as no-match and drains them; golden
    // (real VCL, EStringListError) would instead livelock on message #1
    // forever (SocketTCPIPReceiveList->Delete(0) never reached) plus leak 3
    // TStringLists per tick. Do NOT "fix" this port behavior to imitate
    // golden's livelock -- vclcompat's out-of-range semantics are a tree-wide
    // contract, not a bug local to this file.
    // -------------------------------------------------------------------
    {
        // (a) bare CRLF -- becomes "" after the \r\n strip; sList->Strings[0]
        // on an empty (Count==0) list returns "" (vclcompat), matching no
        // dispatch branch -- consumed without crash.
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-K-a] bare CRLF: consumed without crash", TesterTCPSocket.SocketTCPIPReceiveList->Count == 0, true);
        check_b("[T-K-a] bare CRLF: no wire reply", w906_wire().empty(), true);

        // (b) BINON with only 2 rows -- missing rows read back "" -> atoi("")==0.
        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("BINON:11111111,22222222;\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-K-b] BINON with only 2 rows: consumed without crash", TesterTCPSocket.SocketTCPIPReceiveList->Count == 0, true);
    }

    // -------------------------------------------------------------------
    // [T-L] SimulateBin via the timer's OFF_LINE Simulate tail. GOLDEN QUIRK
    // #12: SimulateStart has NO true-setter anywhere in the whole golden tree
    // -- driven directly here (public bool, same shape golden itself has).
    // -------------------------------------------------------------------
    {
        int savedSiteMap[MAX_SOCKET_ROW][MAX_SOCKET_COL];
        for (int i = 0; i < MAX_SOCKET_ROW; i++)
            for (int j = 0; j < MAX_SOCKET_COL; j++)
            {
                savedSiteMap[i][j] = TestIF.iSiteMap[i][j];
                TestIF.iSiteMap[i][j] = i * MAX_SOCKET_COL + j + 1;
            }
        for (int s = 0; s < 32; s++) TesterTCPSocket.cbSimulateBin[s].ItemIndex = 0;

        int  savedTester   = LastSet.iTester;
        bool savedEcho2    = bEcho;
        bool savedTimeOut2 = bTimeOutForNoFullSite;
        LastSet.iTester = OFF_LINE;
        bEcho = false;
        bTimeOutForNoFullSite = true;
        TesterTCPSocket.SimulateStart = true;
        TesterTCPSocket.SocketTCPIPReceiveList->Clear();   // empty queue -> straight to the Simulate tail

        TesterTCPSocket_TimerProcessTCPDataTimer();

        bool allOne = true;
        for (int i = 0; i < MAX_SOCKET_ROW; i++)
            for (int j = 0; j < MAX_SOCKET_COL; j++)
                if (iBin[i][j] != 1) allOne = false;
        check_b("[T-L] SimulateBin (all ItemIndex=0): iBin all == 1", allOne, true);

        bool allOneStr = true;
        for (int s = 0; s < 32; s++)
            if (w906_s(fMain->tTestResult->Strings[s]) != "1") allOneStr = false;
        check_b("[T-L] SimulateBin: tTestResult all == \"1\"", allOneStr, true);

        check_b("[T-L] SimulateBin tail: bEcho -> true",                 bEcho, true);
        check_b("[T-L] SimulateBin tail: bTimeOutForNoFullSite -> false", bTimeOutForNoFullSite, false);
        check_b("[T-L] SimulateBin tail: SimulateStart self-clears (quirk #12 dead trigger, driven directly here)",
                TesterTCPSocket.SimulateStart, false);

        LastSet.iTester = savedTester;
        bEcho = savedEcho2;
        bTimeOutForNoFullSite = savedTimeOut2;
        for (int i = 0; i < MAX_SOCKET_ROW; i++)
            for (int j = 0; j < MAX_SOCKET_COL; j++)
                TestIF.iSiteMap[i][j] = savedSiteMap[i][j];
    }

    // -------------------------------------------------------------------
    // [T-M] SimulateBin random bounds (2012-10-11 Dell Fix): ItemIndex==6 ->
    // random(5)+1 in [1,5]; ItemIndex>=8 (the trailing `else`) -> random(15)+1
    // in [1,15]. vclcompat::random is std::rand()-backed/unseeded (design doc
    // deviation #2, definition D-3) -- range membership is the golden-fidelity
    // requirement here, not a bit-identical sequence.
    // -------------------------------------------------------------------
    {
        int savedSiteMap00 = TestIF.iSiteMap[0][0];
        TestIF.iSiteMap[0][0] = 1;   // site1 == (i=0,j=0), identity

        TesterTCPSocket.cbSimulateBin[0].ItemIndex = 6;
        TesterTCPSocket_SimulateBin();
        check_b("[T-M] ItemIndex=6 -> iBin[0][0] in [1,5] (random(5)+1)", iBin[0][0] >= 1 && iBin[0][0] <= 5, true);

        TesterTCPSocket.cbSimulateBin[0].ItemIndex = 8;
        TesterTCPSocket_SimulateBin();
        check_b("[T-M] ItemIndex=8 -> iBin[0][0] in [1,15] (random(15)+1, trailing else)", iBin[0][0] >= 1 && iBin[0][0] <= 15, true);

        TesterTCPSocket.cbSimulateBin[0].ItemIndex = 0;
        TestIF.iSiteMap[0][0] = savedSiteMap00;
    }

    // -------------------------------------------------------------------
    // [T-N] bTimerProcessTCPDataEnabled mirror (pure state, golden .dfm
    // Enabled=False/Interval=1) + OFF_LINE reply suppression (this file's own
    // established SendTCPIPCommand OFF_LINE early-return, exercised here
    // through the timer's own reply path).
    // -------------------------------------------------------------------
    {
        check_b("[T-N] bTimerProcessTCPDataEnabled defaults false (pure state mirror, untouched anywhere in this test)",
                TesterTCPSocket.bTimerProcessTCPDataEnabled, false);

        int savedTester2 = LastSet.iTester;
        int savedArm2    = iWhichArmDown;
        LastSet.iTester = OFF_LINE;
        iWhichArmDown = 1;

        TesterTCPSocket_ClientSocket->Socket->SimClearTx();
        w906_push("Test Arm?\r\n");
        TesterTCPSocket_TimerProcessTCPDataTimer();
        check_b("[T-N] OFF_LINE: dispatch still runs (queue drained) but wire stays empty (SendTCPIPCommand's own OFF_LINE early-return)",
                TesterTCPSocket.SocketTCPIPReceiveList->Count == 0 && w906_wire().empty(), true);

        LastSet.iTester = savedTester2;
        iWhichArmDown = savedArm2;
    }

    TesterTCPSocket.SocketTCPIPReceiveList->Clear();   // leave the queue empty, matching every other section's own hygiene

    std::printf("\n=== %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
