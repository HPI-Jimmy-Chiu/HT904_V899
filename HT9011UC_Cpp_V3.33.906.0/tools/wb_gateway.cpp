// =============================================================================
//  tools/wb_gateway.cpp -- the WEB SERVER half of the sidecar split.
//
//  AI(W906-WebBridge-Tcp) 20260812.
//
//  Connects to a publisher's loopback TCP snapshot port, and serves the browser
//  HMI (D:\HT9045\web) over HTTP + WebSocket from whatever it receives.
//
//      wb_gateway.exe                                    -> :8045, publisher 127.0.0.1:8046
//      wb_gateway.exe 9045                               -> different http port
//      wb_gateway.exe --publisher 127.0.0.1:9046         -> different feed
//      wb_gateway.exe --root D:\HT9045\web               -> different web root
//      wb_gateway.exe --seconds 30                       -> exit after N seconds
//
//  THE PROPERTY THAT MATTERS
//  This exe links ht9045_webbridge and NOTHING ELSE from the machine side. No
//  database.h, no state machines, no VCL compatibility layer, no god-stack. It
//  cannot read a motor, open an ini, or command anything, because none of that
//  code is in the binary. That is what makes it safe to restart, rebuild and
//  ship on a different schedule from the software that runs the machine -- and
//  it is why the same binary can front V906 or V899, since it only ever speaks
//  the line-JSON tag feed (WebBridge/TcpTagLink.h).
//
//  Start order does not matter. TcpTagClient reconnects on its own, so the
//  gateway may be started first, and the publisher may be restarted underneath
//  it; the browser sees values go stale and then resume.
//
//  Until the first frame arrives the served page shows "---" everywhere, which
//  is the correct rendering of "unknown" and exactly what a disconnected feed
//  means.
// =============================================================================
#include "WebBridge/TagSnapshot.h"
#include "WebBridge/TcpTagLink.h"
#include "WebBridge/WebBridgeServer.h"

#include <windows.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

namespace {

// "host:port" -> both halves. Returns false on anything malformed, so a typo
// fails at startup rather than silently connecting somewhere unintended.
bool ParseEndpoint(const std::string& s, std::string& host, unsigned short& port)
{
    const std::string::size_type colon = s.rfind(':');
    if (colon == std::string::npos || colon == 0 || colon + 1 >= s.size()) return false;
    host = s.substr(0, colon);
    const int p = std::atoi(s.c_str() + colon + 1);
    if (p <= 0 || p > 65535) return false;
    port = static_cast<unsigned short>(p);
    return true;
}

} // namespace

int main(int argc, char** argv)
{
    std::setvbuf(stdout, 0, _IONBF, 0);

    unsigned short httpPort = 8045;
    std::string    root     = "D:\\HT9045\\web";
    std::string    pubHost  = "127.0.0.1";
    unsigned short pubPort  = 8046;
    int            seconds  = 0;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--root") == 0 && i + 1 < argc) {
            root = argv[++i];
        } else if (std::strcmp(argv[i], "--publisher") == 0 && i + 1 < argc) {
            if (!ParseEndpoint(argv[++i], pubHost, pubPort)) {
                std::printf("bad --publisher endpoint, expected host:port\n");
                return 2;
            }
        } else if (std::strcmp(argv[i], "--seconds") == 0 && i + 1 < argc) {
            seconds = std::atoi(argv[++i]);
        } else {
            httpPort = static_cast<unsigned short>(std::atoi(argv[i]));
        }
    }

    // The one shared object: the TCP client publishes into it, the WebSocket
    // server reads from it. TcpTagClient's worker is the ONLY thread on the
    // publish path (TagSnapshot.h's "UI THREAD ONLY" rule), and this main
    // thread never stages anything -- it only ever calls Wake().
    webbridge::TagSnapshot snap;

    webbridge::TcpClientConfig ccfg;
    ccfg.host = pubHost;
    ccfg.port = pubPort;
    webbridge::TcpTagClient client(snap, ccfg);

    std::string err;
    if (!client.Start(err)) {
        std::printf("tag client failed to start: %s\n", err.c_str());
        return 1;
    }

    webbridge::WebBridgeConfig scfg;
    scfg.port         = httpPort;
    scfg.documentRoot = root;
    // bindAddress and readOnly keep their safe defaults on purpose.

    webbridge::WebBridgeServer server(scfg);
    server.SetSnapshot(&snap);

    if (!server.Start(&err)) {
        std::printf("web server failed to start: %s\n", err.c_str());
        client.Stop();
        return 1;
    }

    std::printf("\n  http://127.0.0.1:%u/?src=ws     (browser HMI)\n",
                (unsigned)server.BoundPort());
    std::printf("  feed  tcp://%s:%u              (tag snapshot publisher)\n",
                pubHost.c_str(), (unsigned)pubPort);
    std::printf("  serving %s\n", root.c_str());
    std::printf("  read-only, loopback only, no machine code linked in\n");
    if (seconds > 0) std::printf("  exiting after %d s\n\n", seconds);
    else             std::printf("  Ctrl-C to stop\n\n");

    const DWORD started = ::GetTickCount();
    unsigned long ticks = 0;
    bool wasConnected = false;

    for (;;) {
        ::Sleep(250);
        server.Wake();

        const bool now = client.Connected();
        if (now != wasConnected) {
            std::printf("  feed %s\n", now ? "CONNECTED" : "disconnected");
            wasConnected = now;
        }

        if (++ticks % 20 == 0) {
            const webbridge::TcpTagClient::Stats s = client.GetStats();
            std::printf("  [%lus] feed=%s frames=%llu bytes=%llu gen=%llu reconnects=%llu parseErr=%llu tags=%u\n",
                        (unsigned long)((::GetTickCount() - started) / 1000),
                        s.connected ? "up" : "down",
                        (unsigned long long)s.framesReceived,
                        (unsigned long long)s.bytesReceived,
                        (unsigned long long)s.lastGeneration,
                        (unsigned long long)s.reconnects,
                        (unsigned long long)s.parseErrors,
                        (unsigned)snap.publishedTagCount());
        }

        if (seconds > 0 &&
            (::GetTickCount() - started) >= static_cast<DWORD>(seconds) * 1000u) {
            break;
        }
    }

    server.Stop();
    client.Stop();
    return 0;
}
