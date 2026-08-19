// =============================================================================
//  tools/wb_serve.cpp -- run the web HMI against the REAL handler data layer.
//
//  AI(W906-WebBridge) 20260806.
//
//  This is the WB-2 milestone in runnable form: load the machine config, stand
//  up the bridge, serve D:\HT9045\web, and publish real tag values to whatever
//  browser connects.
//
//      wb_serve.exe                 -> http://127.0.0.1:8045/?src=ws
//      wb_serve.exe 9000            -> different port
//      wb_serve.exe --root <dir>    -> serve a different web root
//      wb_serve.exe --seconds 20    -> exit after N seconds (for scripted runs)
//
//  WHAT IT IS NOT
//  Not the product. The handler proper will own this server on its own UI
//  thread once GA-3 lands the god-stack in HT9045.exe (see LoadMachineConfig in
//  database.h). This exe exists so the whole path can be exercised, and looked
//  at, before that.
//
//  SAFETY POSTURE -- inherited, not re-decided
//    * loopback only, and read-only: WebBridgeConfig defaults both that way
//      because this endpoint can eventually command machine motion
//      (web/docs/ARCHITECTURE.md section 6, questions 2 and 3). This file does
//      not widen either.
//    * LoadMachineConfig() SEEDS missing keys, i.e. it WRITES to asGeneralPath.
//      That is the real system\Gerneral.ini. Running this on a machine is
//      therefore the same class of act as starting the handler -- which is the
//      point, but it is stated here rather than discovered. --dry copies the
//      config to a scratch file first and leaves the real one alone.
//
//  WHAT THE BROWSER WILL SHOW
//  Mostly "---". That is correct, not broken: only ~8 of the tags have a source
//  that the port actually loads today. WebBridgeTags.h carries the measured
//  live/dead inventory and the reason for each.
// =============================================================================
#include "WebBridgeTags.h"

#include "WebBridge/WebBridgeServer.h"
#include "WebBridge/TagSnapshot.h"
#include "WebBridge/CommandQueue.h"   // AI(W906-FW-W1) 20260819: cmd channel e2e (--allow-cmd)
#include "WebAuth.h"                  // AI(W906-FW-W2) 20260819: auth.login verification core
#include "cmydef.h"                   // AccessLevel, pwPath (golden globals the auth commands drive)
// AI(W906-FW-W4) 20260819: first real command family -- counter.clear drives
// the translated TfCounterClear::ClearCount core (test_counterclear_core
// covers it) behind golden's own Security_new.def per-item authorization.
#include "forms/fCounterClear.h"      // fCounterClear global + ClearCount (MachineType.h: eClearType)
#include "forms/fMain.h"              // fMain->Clarn_Data (golden spbExeClick bracket)
#include "cAuthority.h"               // GetCountClrAuth(), authCounterClr[]
#include "cMyDB.h"                    // MyDBIProcess (recording sim in this tree)
#include "LastSet.h"                  // LastSet (post-clear observable printed to the serve log)
// AI(W906-FW-W5a) 20260819: ShowMyMessage + its forward hook live in
// canary_support.{h,cpp} -- but that header re-defaults RecordProcess/
// MyDBIProcessNew parameters that common.h (already included above) also
// defaults, which is ill-formed in one TU. Local forward-decls instead,
// the same idiom database.cpp already uses for ShowMyMessage; no defaults
// here, every argument is passed explicitly at the call site.
void ShowMyMessage(AnsiString S1, AnsiString S2, AnsiString S3, bool Ok, bool bServoOff);
extern void (*W906_ShowMyMessage_Hook)(const char* S1, const char* S2);

#include <vector>

#include "database.h"
#include "common.h"

#include <windows.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

// AI(W906-FW-W5a) 20260819: ShowMyMessage -> browser info modal. The hook is a
// raw function pointer (canary_support.h keeps zero includes), so the server
// handle rides a file-scope static. Display-only by design: golden's
// ShowMyMessage returns void, nothing flows back (design doc section 4).
static webbridge::WebBridgeServer* g_modalServer = 0;
static void ForwardShowMyMessage(const char* s1, const char* s2)
{
    if (!g_modalServer) return;
    std::string text(s1 ? s1 : "");
    if (s2 && s2[0]) { text += " | "; text += s2; }
    g_modalServer->PostModal("Message", text);
}

int main(int argc, char** argv)
{
    std::setvbuf(stdout, 0, _IONBF, 0);

    unsigned short port = 8045;
    std::string    root = "D:\\HT9045\\web";
    int            seconds = 0;      // 0 = run until Ctrl-C
    bool           dry = true;
    // AI(W906-FW-W1) 20260819: write-path channel opt-in. Default stays
    // READ-ONLY (the server refuses every "cmd" frame); --allow-cmd attaches
    // a CommandQueue and flips SetReadOnly(false) -- same explicit-flag
    // safety convention as --real. First dispatch table carries only
    // sys.ping (pure echo); see docs/WEBBRIDGE_WRITEPATH_DESIGN.md section 6.
    bool           allowCmd = false;

    //AI(W906-FW1) 20260817: same two safety reversals as wb_publish, paid for
    // the same evening (see tools/wb_publish.cpp): unknown arguments refuse
    // instead of falling through to atoi-as-port, and touching the REAL
    // system\Gerneral.ini needs an explicit --real -- a config load can WRITE
    // the file it reads, and the TIniFile flush destroys comments and layout.
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--root") == 0 && i + 1 < argc) {
            root = argv[++i];
        } else if (std::strcmp(argv[i], "--seconds") == 0 && i + 1 < argc) {
            seconds = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--dry") == 0) {
            dry = true;
        } else if (std::strcmp(argv[i], "--real") == 0) {
            dry = false;
        } else if (std::strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            port = static_cast<unsigned short>(std::atoi(argv[++i]));
        } else if (std::strcmp(argv[i], "--allow-cmd") == 0) {
            allowCmd = true;
        } else {
            std::printf("wb_serve: unknown argument '%s'\n"
                        "usage: wb_serve [--port N] [--root DIR] [--seconds N] [--dry] [--real] [--allow-cmd]\n"
                        "  the config load uses a scratch copy by default; --real opts\n"
                        "  into touching the live system\\Gerneral.ini.\n",
                        argv[i]);
            return 2;
        }
    }

    // --- 1. bring the machine data layer up ---------------------------------
    const AnsiString savedGeneralPath = asGeneralPath;
    AnsiString scratch;

    if (dry) {
        char tmp[MAX_PATH];
        ::GetTempPathA(MAX_PATH, tmp);
        scratch = AnsiString(tmp) + "wb_serve_general.ini";
        if (!::CopyFileA(savedGeneralPath.c_str(), scratch.c_str(), FALSE)) {
            std::printf("--dry: cannot copy %s (err %lu)\n",
                        savedGeneralPath.c_str(), (unsigned long)::GetLastError());
            return 2;
        }
        asGeneralPath = scratch;
        std::printf("--dry: using a scratch copy, real config untouched\n");
    }

    std::printf("loading machine config from %s ...\n", asGeneralPath.c_str());
    if (!LoadMachineConfig()) {
        std::printf("LoadMachineConfig FAILED -- cannot open the ini\n");
        return 1;
    }

    webbridge::TagSnapshot snap;
    const std::size_t staged = ht9045::PublishHandlerTags(snap);
    const ht9045::TagCoverage cov = ht9045::HandlerTagCoverage();
    std::printf("published %u tags, %u of %u carry a loaded value\n",
                (unsigned)staged, (unsigned)cov.live, (unsigned)cov.total);

    // --- 2. stand the bridge up ---------------------------------------------
    webbridge::WebBridgeConfig cfg;
    cfg.port         = port;
    cfg.documentRoot = root;
    // bindAddress and readOnly keep their safe defaults on purpose.

    webbridge::WebBridgeServer server(cfg);
    server.SetSnapshot(&snap);

    // AI(W906-FW-W1) 20260819: the command channel. Queue is attached
    // unconditionally (harmless while read-only); the read-only gate is what
    // --allow-cmd actually opens. Dispatch happens on THIS thread's tick
    // below -- the single-process stand-in for "the UI thread drains on its
    // existing timer tick" (design doc section 1).
    webbridge::CommandQueue cmdQueue;
    server.SetCommandQueue(&cmdQueue);
    server.SetReadOnly(!allowCmd);

    std::string err;
    if (!server.Start(&err)) {
        std::printf("server failed to start: %s\n", err.c_str());
        return 1;
    }

    // AI(W906-FW-W5a) 20260819: from here on, every ShowMyMessage anywhere in
    // the linked machine code also reaches the browser as an info modal.
    g_modalServer = &server;
    W906_ShowMyMessage_Hook = &ForwardShowMyMessage;

    std::printf("\n  http://127.0.0.1:%u/?src=ws     (live handler data)\n",
                (unsigned)server.BoundPort());
    std::printf("  serving %s\n", root.c_str());
    std::printf(allowCmd ? "  COMMANDS ENABLED (--allow-cmd; dispatch: sys.ping), loopback only\n"
                         : "  read-only, loopback only\n");
    std::printf("  most values will read \"---\": that is the truth, see WebBridgeTags.h\n");
    std::printf(seconds > 0 ? "  exiting after %d s\n\n" : "  Ctrl-C to stop\n\n", seconds);

    // --- 3. republish on a slow tick -----------------------------------------
    // The real handler will do this from its existing UI timer. Nothing here
    // reads machine state off the socket thread; the snapshot is the only seam.
    const DWORD started = ::GetTickCount();
    std::vector<webbridge::WebCommand> drained;
    for (;;) {
        ::Sleep(500);

        // AI(W906-FW-W1) 20260819: drain + dispatch on the tick, ack via
        // CompleteCommand (ticket == WebCommand.id, see QueuePush). FW-W1's
        // dispatch table is deliberately just sys.ping -- proving the
        // browser->ws->queue->tick->ack round trip end to end; real commands
        // land per design doc section 6 (FW-W2+).
        drained.clear();
        cmdQueue.drain(drained);
        for (size_t i = 0; i < drained.size(); ++i) {
            const webbridge::WebCommand& wc = drained[i];
            if (wc.cmd == "sys.ping") {
                server.CompleteCommand((unsigned long long)wc.id, true, std::string());
            } else if (wc.cmd == "sys.echoModal") {
                // AI(W906-FW-W5a) 20260819: probe surface for the modal path,
                // sys.ping's sibling -- drives the REAL ShowMyMessage (capture
                // seam included), whose hook then broadcasts the modal frame.
                // Text rides `value` (free-form JSON string), not `tag` (the
                // server's tag-name charset rejects spaces).
                AnsiString mtext = (wc.hasValue && wc.value.isString())
                                   ? AnsiString(wc.value.asString().c_str()) : AnsiString("");
                ShowMyMessage(mtext, AnsiString(""), AnsiString(""), false, false);
                server.CompleteCommand((unsigned long long)wc.id, true, std::string());
            } else if (wc.cmd == "auth.login") {
                // AI(W906-FW-W2) 20260819: tag = user name, value = password
                // (both strings). Verification = golden's password-book arm
                // (WebAuth.cpp); success drives the SAME global golden's
                // btLogin drives: AccessLevel. Book path: golden's pwPath
                // global, test-overridable via W906_PWBOOK_PATH (call-time
                // getenv, the tree's established env-seam shape).
                AnsiString book = getenv("W906_PWBOOK_PATH")
                                  ? AnsiString(getenv("W906_PWBOOK_PATH")) : pwPath;
                AnsiString u = wc.hasTag   ? AnsiString(wc.tag.c_str())   : AnsiString("");
                AnsiString p = (wc.hasValue && wc.value.isString())
                                  ? AnsiString(wc.value.asString().c_str()) : AnsiString("");
                const int level = WebAuthVerify(book, u, p);
                if (level >= 0) {
                    AccessLevel = level;                 // golden: AccessLevel=l;
                    server.CompleteCommand((unsigned long long)wc.id, true, std::string());
                } else {
                    server.CompleteCommand((unsigned long long)wc.id, false, "bad credentials");
                }
            } else if (wc.cmd == "auth.logout") {
                AccessLevel = 0;                          // golden: back to Operator
                server.CompleteCommand((unsigned long long)wc.id, true, std::string());
            } else if (wc.cmd == "counter.clear") {
                // AI(W906-FW-W4) 20260819: tag = counter family, mirroring one
                // checkbox of golden spbExeClick (cCounterClear.cpp golden
                // :394-450). Authorization is golden's OWN gate replayed: the
                // FormShow rule "an unauthorized checkbox can never be checked"
                // becomes a per-family refusal here, read from the same
                // Security_new.def via GetCountClrAuth() (cAuthority.cpp
                // golden :379-384). WRITE CAVEAT, stated not discovered:
                // golden's own readers seed defaults back into that file --
                // CheckFile when the FILE is missing, and CheckAndReadIniData
                // (common.cpp:409) when a KEY is missing. Faithful, but it
                // means this dispatch can write config\Security_new.def on a
                // machine with a stale def; every e2e run MD5-checks the file
                // stayed untouched.
                // Deliberately NOT called (documented deviations):
                //   * MyDBIProductionData -- golden runs it with the sqlite
                //     production DB open; wb_serve never brings the DB layer
                //     up, so its precondition does not exist here (and
                //     MyDBExecSQL on a null dbReadWrite is a crash, not a row).
                //   * the CC_KYEC_LEE re-auth branch -- already GATE CC3
                //     (#if 0) in the translated core itself.
                struct FamilyRow { const char* key; int authIdx; int ct1; int ct2; };
                static const FamilyRow kFamilies[] = {
                    // authIdx = funCounterClr[] order (cAuthority.cpp:203);
                    // ct pairing = golden spbExeClick, incl. loadingCount's
                    // ctIndexCount rider (golden :433 kevin 20130125).
                    { "alarmData",      0, ctAlarmData,       -1           },
                    { "testerCategory", 1, ctTesterCategory,  -1           },
                    { "loadingCount",   3, ctLoadingCounts,   ctIndexCount },
                    { "contactCurr",    4, ctContactCounts,   -1           },
                    { "contactHis",     5, ctContactCountsHis,-1           },
                    { "sortingCount",   6, ctTraySortCount,   -1           },
                    { "timeData",       7, ctTimeData,        -1           },
                };
                const FamilyRow* row = 0;
                for (size_t f = 0; f < sizeof(kFamilies)/sizeof(kFamilies[0]); ++f)
                    if (wc.hasTag && wc.tag == kFamilies[f].key) { row = &kFamilies[f]; break; }
                if (!row) {
                    server.CompleteCommand((unsigned long long)wc.id, false,
                                           "unknown counter family");
                } else {
                    GetCountClrAuth();                       // golden FormShow gate
                    if (!authCounterClr[row->authIdx]) {
                        server.CompleteCommand((unsigned long long)wc.id, false,
                                               "not-authorized");
                    } else {
                        fMain->Clarn_Data(10, "Manual clear count");      // golden :421
                        fCounterClear->ClearCount(row->ct1);
                        if (row->ct2 >= 0) fCounterClear->ClearCount(row->ct2);
                        fMain->Clarn_Data(10, "Manual clear count done"); // golden :448
                        MyDBIProcess("Process", "Counter Clear has been executed!!"); // golden :449
                        std::printf("counter.clear[%s] done (SendCT[0]=%d iIndexCount=%d)\n",
                                    row->key, LastSet.SendCT[0], LastSet.iIndexCount);
                        server.CompleteCommand((unsigned long long)wc.id, true, std::string());
                    }
                }
            } else {
                server.CompleteCommand((unsigned long long)wc.id, false,
                                       "unknown cmd (dispatch: sys.ping, sys.echoModal, auth.login, auth.logout, counter.clear)");
            }
        }

        ht9045::SetWebControlOwner(server.ControlOwner());   // AI(W906-FW-W3) 20260819: control.owner tag feed
        ht9045::PublishHandlerTags(snap);
        server.Wake();

        if (seconds > 0 &&
            (::GetTickCount() - started) >= static_cast<DWORD>(seconds) * 1000u) {
            break;
        }
    }

    // AI(W906-FW-W5a) 20260819: unhook before the server object dies.
    W906_ShowMyMessage_Hook = 0;
    g_modalServer = 0;

    server.Stop();

    if (dry) {
        CloseGeneralIniFile();
        asGeneralPath = savedGeneralPath;
        ::DeleteFileA(scratch.c_str());
    }

    std::printf("stopped\n");
    return 0;
}
