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
    bool           dry = false;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--root") == 0 && i + 1 < argc) {
            root = argv[++i];
        } else if (std::strcmp(argv[i], "--seconds") == 0 && i + 1 < argc) {
            seconds = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--dry") == 0) {
            dry = true;
        } else {
            port = static_cast<unsigned short>(std::atoi(argv[i]));
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

    std::string err;
    if (!server.Start(&err)) {
        std::printf("server failed to start: %s\n", err.c_str());
        return 1;
    }

    std::printf("\n  http://127.0.0.1:%u/?src=ws     (live handler data)\n",
                (unsigned)server.BoundPort());
    std::printf("  serving %s\n", root.c_str());
    std::printf("  read-only, loopback only\n");
    std::printf("  most values will read \"---\": that is the truth, see WebBridgeTags.h\n");
    std::printf(seconds > 0 ? "  exiting after %d s\n\n" : "  Ctrl-C to stop\n\n", seconds);

    // --- 3. republish on a slow tick -----------------------------------------
    // The real handler will do this from its existing UI timer. Nothing here
    // reads machine state off the socket thread; the snapshot is the only seam.
    const DWORD started = ::GetTickCount();
    for (;;) {
        ::Sleep(500);
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
