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
//      wb_publish.exe --pump          -> DRIVE the state machine (see below)
//      wb_publish.exe --tick-ms 250   -> pump/publish period (default 500, 250 w/ --pump)
//      wb_publish.exe --pump --with-config  -> both, and read WebBridgeTags.h first
//
//  --pump  AI(W906-SimPump) 20260813
//  Without it, this process re-reads the same config globals every tick and the
//  browser shows a static screen: nothing here ever advanced the state machine.
//  With it, each tick calls MainProc() -- the real translated spine -- so the
//  engine cursors move, the A/B tick alternates, and machine.state carries a live
//  word instead of a blank. The word is prefixed "SIM" on purpose; the reachability
//  measurement that forces that is in WebBridgeTags.h and it is not decoration.
//
//  --pump DELIBERATELY DOES NOT LOAD CONFIG, and that is a safety property, not an
//  omission. Three independent reasons, each verified 20260813:
//    1. ReadGeneralIni OVERWRITES five of the guard globals the pump fixture pins
//       (database.cpp:454/:593/:703/:1079/:1430) -- load-after-guards silently
//       changes the tick shape while everything stays green.
//    2. It can set MOTION_CARD_TYPE=Contec and LastSet.iRealDummy=REALLY, which
//       un-self-disables the hardware interlock at ainarm9045.cpp:1891-1893 and
//       would let CCLink/Ltc/MOT bodies run that have NEVER executed offline.
//       PumpInit() refuses if it sees both, but not loading config avoids the
//       question entirely.
//    3. LoadMachineConfig SEEDS missing keys = WRITES, and --dry only redirects
//       asGeneralPath. lastdata.dat / lastdata_backup*.dat paths are hard-coded
//       literals (cprod.cpp:1701, :1729). This repo has already lost teach data
//       once to exactly that shape of write.
//  Cost of skipping it: the 8 config-derived tags go NULL -- which the existing
//  liveness predicates in WebBridgeTags.cpp already do correctly, with no new
//  machinery. --with-config re-enables the load for anyone who needs those 8 and
//  has read the above.
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
    //AI(W906-SimPump) 20260813: FIRST statement, before anything can fault.
    // wb_publish is declared in the ROOT CMakeLists, so it does NOT receive
    // tests/'s directory-scoped ht9045_test_bootstrap (tests/CMakeLists.txt:25-29)
    // and has none of its modal-dialog suppression. Pumping a state machine for
    // hours unattended without this means an access violation raises a Windows
    // Error Reporting box that blocks forever with NOTHING in the log -- a failure
    // mode this project has already paid for once.
    ::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX |
                   SEM_NOOPENFILEERRORBOX);

    std::setvbuf(stdout, 0, _IONBF, 0);

    unsigned short port    = 8046;
    int            seconds = 0;      // 0 = run until Ctrl-C
    bool           dry     = false;
    bool           pump    = false;  // drive MainProc() each tick
    bool           withCfg = false;  // load config even when pumping (see header)
    int            tickMs  = 0;      // 0 = pick the default below

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--seconds") == 0 && i + 1 < argc) {
            seconds = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--tick-ms") == 0 && i + 1 < argc) {
            tickMs = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--dry") == 0) {
            dry = true;
        } else if (std::strcmp(argv[i], "--pump") == 0) {
            pump = true;
        } else if (std::strcmp(argv[i], "--with-config") == 0) {
            withCfg = true;
        } else {
            port = static_cast<unsigned short>(std::atoi(argv[i]));
        }
    }

    // Config is loaded unless we are pumping -- see the --pump block in the header
    // comment for why that default is the safe one.
    const bool loadConfig = (!pump || withCfg);

    if (tickMs <= 0) tickMs = pump ? 250 : 500;

    // --- 1. bring the machine data layer up ---------------------------------
    const AnsiString savedGeneralPath = asGeneralPath;
    AnsiString scratch;

    if (dry && loadConfig) {
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

    if (loadConfig) {
        std::printf("loading machine config from %s ...\n", asGeneralPath.c_str());
        if (!LoadMachineConfig()) {
            std::printf("LoadMachineConfig FAILED -- cannot open the ini\n");
            return 1;
        }
    } else {
        //AI(W906-SimPump) 20260813: not an error path and not a degraded one. The 8
        // config-derived tags will publish as NULL via the existing liveness
        // predicates, which is the honest rendering of "nobody read this".
        std::printf("--pump: config NOT loaded (no ini read, no ini write).\n"
                    "        the 8 config-derived tags publish as null; "
                    "pass --with-config to change that\n");
    }

    //AI(W906-SimPump) 20260813: MUST come after any config load -- ReadGeneralIni
    // overwrites five of the globals PumpInit pins (WebBridgeTags.h).
    if (pump) {
        std::string whyNot;
        if (!ht9045::PumpInit(whyNot)) {
            std::printf("--pump REFUSED: %s\n", whyNot.c_str());
            return 3;
        }
        std::printf("--pump: spine armed (guards forced, cursors reset); "
                    "tick = %d ms\n"
                    "        machine.state will read \"SIM RUN\"/\"SIM HALT\" -- "
                    "SIM because this process\n"
                    "        forced SystemStart/fAllMotorHome; no machine is "
                    "attached and none is sensed.\n", tickMs);
    }

    webbridge::TagSnapshot snap;
    const std::size_t staged = ht9045::PublishHandlerTags(snap);
    const ht9045::TagCoverage cov = ht9045::HandlerTagCoverage();
    //AI(W906-SimPump) 20260813: two different denominators, said out loud. `staged`
    // is what actually goes on the wire; cov.* counts MACHINE data sources only and
    // deliberately excludes this process's own clock/state/pump tags, so the two
    // numbers are not meant to reconcile (WebBridgeTags.h).
    std::printf("published %u tags on the wire; of the %u MACHINE-sourced ones, "
                "%u carry a loaded value\n",
                (unsigned)staged, (unsigned)cov.total, (unsigned)cov.live);

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
        ::Sleep(tickMs);

        //AI(W906-SimPump) 20260813: the spine tick comes BEFORE the publish so the
        // snapshot describes the state the tick just produced, not the previous
        // one. PumpTick() is a no-op when --pump was not given, and it never
        // throws (WebBridgeTags.h).
        ht9045::PumpTick();

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
            if (ht9045::PumpActive()) {
                // Printed alongside the socket stats because a publisher that is
                // serving frames but NOT ticking -- or ticking and throwing every
                // time -- looks identical from the browser.
                const ht9045::PumpStats ps = ht9045::PumpTelemetry();
                //AI(W906-SimPump) 20260813: %lu + unsigned long, NOT %llu, to keep
                // this line -Wformat-clean. g++ here reports "unknown conversion
                // type character 'l'" for %llu because the format checker follows
                // msvcrt semantics.
                //
                // CORRECTION, measured 20260813: that warning is a FALSE ALARM about
                // capability. I first wrote here that %llu "prints garbage"; a real
                // run disproves it -- the socket-stats line above uses %llu and
                // printed frames=79 bytes=14531, i.e. correct values. So the
                // pre-existing line is NOT broken and must not be "fixed" on the
                // strength of the warning alone. %lu is used below only to avoid the
                // diagnostic; unsigned long is ample either way (at the 250 ms
                // default, 2^32 ticks is ~34 years).
                std::printf("        pump: ticks=%lu mainProcCalls=%lu "
                            "exceptions=%lu alive=%s\n",
                            (unsigned long)ps.ticks,
                            (unsigned long)ps.mainProcCalls,
                            (unsigned long)ps.exceptions,
                            ps.alive ? "yes" : "no");
            }
        }

        if (seconds > 0 &&
            (::GetTickCount() - started) >= static_cast<DWORD>(seconds) * 1000u) {
            break;
        }
    }

    pub.Stop();

    //AI(W906-SimPump) 20260813: guarded by loadConfig as well as dry. Calling
    // CloseGeneralIniFile() when nothing was ever opened would walk into the
    // documented faithful bug at common.cpp:1464-1472 (deletes INIFileGeneral
    // without nulling it), and there is no scratch file to remove either.
    if (dry && loadConfig) {
        CloseGeneralIniFile();
        asGeneralPath = savedGeneralPath;
        ::DeleteFileA(scratch.c_str());
    }
    return 0;
}
