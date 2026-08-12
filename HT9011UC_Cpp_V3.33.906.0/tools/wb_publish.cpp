// =============================================================================
//  tools/wb_publish.cpp -- the PUBLISHER half of the sidecar split.
//
//  AI(W906-WebBridge-Tcp) 20260812.
//
//  Loads the machine data layer and exposes it on a loopback, read-only TCP
//  snapshot port. It serves no HTTP and knows nothing about a browser.
//
//      wb_publish.exe                 -> tcp://127.0.0.1:8046
//      wb_publish.exe 9046            -> different port
//      wb_publish.exe --seconds 30    -> exit after N seconds (scripted runs)
//      wb_publish.exe --dry           -> scratch config copy; real ini untouched
//
//  WHAT IT IS
//  The stand-in for "HT9045.exe with a snapshot port". Everything in main()
//  below that is not argument parsing is what the handler itself would do from
//  its existing UI timer: publish a TagSnapshot, and let a socket thread it
//  never touches carry that to whoever is listening.
//
//  WHY A SEPARATE EXE FROM wb_serve
//  wb_serve serves the browser DIRECTLY from the same process as the data
//  (ARCHITECTURE.md option A, in miniature). This pair splits that in two so
//  the web server can live in a different process from the machine data
//  (option B). The split is what lets the web server be built, shipped and
//  restarted without touching the binary that runs the machine.
//
//  SAFETY POSTURE -- same as wb_serve, inherited not re-decided
//    * loopback only and read-only: TcpPublisherConfig defaults both that way.
//    * LoadMachineConfig() SEEDS missing keys, i.e. it WRITES asGeneralPath --
//      the real system\Gerneral.ini. Running this on a machine is the same
//      class of act as starting the handler. --dry avoids it.
// =============================================================================
#include "WebBridgeTags.h"

#include "WebBridge/TagSnapshot.h"
#include "WebBridge/TcpTagLink.h"

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

    unsigned short port    = 8046;
    int            seconds = 0;      // 0 = run until Ctrl-C
    bool           dry     = false;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--seconds") == 0 && i + 1 < argc) {
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
        scratch = AnsiString(tmp) + "wb_publish_general.ini";
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

    // --- 2. stand the TCP publisher up ---------------------------------------
    webbridge::TcpPublisherConfig cfg;
    cfg.port = port;
    // bindAddress stays 127.0.0.1 on purpose (ARCHITECTURE.md section 6 q2).

    webbridge::TcpTagPublisher pub(snap, cfg);

    std::string err;
    if (!pub.Start(err)) {
        std::printf("publisher failed to start: %s\n", err.c_str());
        return 1;
    }

    std::printf("\n  tcp://127.0.0.1:%u        (tag snapshot feed)\n",
                (unsigned)pub.BoundPort());
    std::printf("  loopback only, read-only, line-delimited JSON\n");
    std::printf("  consume it with: wb_gateway.exe --publisher 127.0.0.1:%u\n",
                (unsigned)pub.BoundPort());
    if (seconds > 0) std::printf("  exiting after %d s\n\n", seconds);
    else             std::printf("  Ctrl-C to stop\n\n");

    // --- 3. republish on a slow tick -----------------------------------------
    // This loop is the stand-in for the handler's UI timer. Note what it does
    // NOT do: it never touches a socket. The publisher thread does that, and it
    // only ever reads the snapshot. That separation is the whole point --
    // ARCHITECTURE.md section 5, "nothing on the socket thread may block on
    // machine I/O, ever".
    const DWORD started = ::GetTickCount();
    unsigned long ticks  = 0;
    for (;;) {
        ::Sleep(500);
        ht9045::PublishHandlerTags(snap);

        if (++ticks % 10 == 0) {
            const webbridge::TcpTagPublisher::Stats s = pub.GetStats();
            std::printf("  [%lus] clients=%d accepted=%llu dropped=%llu frames=%llu bytes=%llu\n",
                        (unsigned long)((::GetTickCount() - started) / 1000),
                        s.activeClients,
                        (unsigned long long)s.clientsAccepted,
                        (unsigned long long)s.clientsDropped,
                        (unsigned long long)s.framesSent,
                        (unsigned long long)s.bytesSent);
        }

        if (seconds > 0 &&
            (::GetTickCount() - started) >= static_cast<DWORD>(seconds) * 1000u) {
            break;
        }
    }

    pub.Stop();

    if (dry) {
        CloseGeneralIniFile();
        asGeneralPath = savedGeneralPath;
        ::DeleteFileA(scratch.c_str());
    }
    return 0;
}
