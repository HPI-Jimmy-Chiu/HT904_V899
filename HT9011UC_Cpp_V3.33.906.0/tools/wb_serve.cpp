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

#include <vector>

#include "database.h"
#include "common.h"

#include <windows.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

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
            } else {
                server.CompleteCommand((unsigned long long)wc.id, false,
                                       "unknown cmd (dispatch: sys.ping, auth.login, auth.logout)");
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

    server.Stop();

    if (dry) {
        CloseGeneralIniFile();
        asGeneralPath = savedGeneralPath;
        ::DeleteFileA(scratch.c_str());
    }

    std::printf("stopped\n");
    return 0;
}
