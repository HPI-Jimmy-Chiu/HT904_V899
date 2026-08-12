// ===========================================================================
//  WebBridge/TcpTagLink.cpp -- see TcpTagLink.h for the contract, the
//  threading rules, and why SECS was rejected as the transport.
//
//  AI(W906-WebBridge-Tcp) 20260812: new file.
//
//  INCLUDE ORDER IS LOAD-BEARING: <winsock2.h> must precede anything that
//  drags in <windows.h> (Sync.h does), or windows.h pulls the ancient
//  winsock.h first and every socket symbol redefines. TagSnapshot.h keeps
//  windows.h out of itself for the same reason -- see its header note.
// ===========================================================================
#include <winsock2.h>
#include <ws2tcpip.h>

#include "WebBridge/TcpTagLink.h"

#include "WebBridge/Sync.h"
#include "WebBridge/TagJson.h"
#include "WebBridge/JsonWriter.h"
#include "Public/cJSON.h"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <string>
#include <vector>

namespace webbridge {
namespace {

// ---------------------------------------------------------------------------
//  Winsock lifetime. WSAStartup/WSACleanup are refcounted by the OS, so a
//  second component calling them (WebBridgeServer already does) is safe and
//  expected -- each just has to balance its own pair.
// ---------------------------------------------------------------------------
class WinsockScope {
public:
    WinsockScope() : ok_(false)
    {
        WSADATA d;
        ok_ = (::WSAStartup(MAKEWORD(2, 2), &d) == 0);
    }
    ~WinsockScope() { if (ok_) ::WSACleanup(); }
    bool ok() const { return ok_; }
private:
    bool ok_;
    WinsockScope(const WinsockScope&);
    WinsockScope& operator=(const WinsockScope&);
};

// std::chrono, not GetTickCount64(): the latter needs _WIN32_WINNT >= 0x0600 to
// be DECLARED by MinGW's headers even though the project pins 0x0601, and this
// file has no reason to depend on that. WebBridgeServer.cpp:257 already reads
// the clock exactly this way.
unsigned long long NowMs()
{
    using namespace std::chrono;
    return static_cast<unsigned long long>(
        duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

// getaddrinfo rather than inet_addr/inet_pton -- "present and non-deprecated on
// both MinGW and MSVC", the same reasoning (and the same code shape) as
// WebBridgeServer.cpp:561. inet_pton in particular is NOT declared by MinGW's
// ws2tcpip.h in this configuration, which is how this was found.
bool ResolveIPv4(const std::string& host, in_addr& out)
{
    addrinfo hints;
    ::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_NUMERICHOST;

    addrinfo* ai = NULL;
    int rc = ::getaddrinfo(host.c_str(), NULL, &hints, &ai);
    if (rc != 0) {                       // allow "localhost" and friends
        hints.ai_flags = 0;
        rc = ::getaddrinfo(host.c_str(), NULL, &hints, &ai);
    }
    if (rc != 0 || ai == NULL) {
        if (ai) ::freeaddrinfo(ai);
        return false;
    }
    out = reinterpret_cast<const sockaddr_in*>(ai->ai_addr)->sin_addr;
    ::freeaddrinfo(ai);
    return true;
}

void SetNonBlocking(SOCKET s)
{
    u_long nb = 1;
    ::ioctlsocket(s, FIONBIO, &nb);
}

void CloseSock(SOCKET& s)
{
    if (s != INVALID_SOCKET) { ::closesocket(s); s = INVALID_SOCKET; }
}

std::string LastErrText(const char* what)
{
    const int e = ::WSAGetLastError();
    char buf[64];
    ::sprintf(buf, " (WSA error %d)", e);
    return std::string(what) + buf;
}

// ---- frame builders -------------------------------------------------------
// Every builder ends with '\n'; see the header's wire-format note.

std::string FrameHello(std::uint64_t gen)
{
    JsonWriter w;
    w.BeginObject();
    w.Key("type").String("hello");
    w.Key("proto").Number(static_cast<wb_int64>(1));
    w.Key("gen").Number(static_cast<wb_int64>(gen));
    w.EndObject();
    return w.Str() + "\n";
}

std::string FrameSnapshot(const TagSnapshotView& v)
{
    JsonWriter w;
    w.BeginObject();
    w.Key("type").String("snapshot");
    w.Key("gen").Number(static_cast<wb_int64>(v.generation));
    w.Key("data").RawValue(EncodeTagObject(v.tags));
    w.EndObject();
    return w.Str() + "\n";
}

std::string FramePatch(const TagPatch& p)
{
    JsonWriter w;
    w.BeginObject();
    w.Key("type").String("patch");
    w.Key("gen").Number(static_cast<wb_int64>(p.generation));
    w.Key("data").RawValue(EncodeTagObject(p.changed));
    w.Key("removed").BeginArray();
    for (std::size_t i = 0; i < p.removed.size(); ++i) w.String(p.removed[i]);
    w.EndArray();
    w.EndObject();
    return w.Str() + "\n";
}

std::string FramePing(unsigned long long t)
{
    JsonWriter w;
    w.BeginObject();
    w.Key("type").String("ping");
    w.Key("t").Number(static_cast<wb_int64>(t));
    w.EndObject();
    return w.Str() + "\n";
}

} // namespace

// ===========================================================================
//  TcpTagPublisher
// ===========================================================================
struct TcpTagPublisher::Impl {
    // One connected consumer.
    struct Peer {
        SOCKET          sock;
        std::string     out;        // pending bytes, drained by the loop
        TagSnapshotView lastSent;   // what this peer's state is after `out` lands
        unsigned long long lastWriteMs;

        Peer() : sock(INVALID_SOCKET), lastWriteMs(0) {}
    };

    const TagSnapshot&  source;
    TcpPublisherConfig  cfg;

    WinsockScope        winsock;
    SOCKET              listener;
    unsigned short      boundPort;

    WbThread            thread;
    volatile LONG       stopFlag;
    volatile LONG       running;

    mutable WbMutex     statsLock;
    Stats               stats;

    std::vector<Peer>   peers;      // publisher thread only, no lock needed
    unsigned long long  startMs;

    Impl(const TagSnapshot& src, const TcpPublisherConfig& c)
        : source(src), cfg(c), listener(INVALID_SOCKET), boundPort(0)
        , stopFlag(0), running(0), startMs(0) {}

    void BumpSent(std::size_t bytes)
    {
        WbGuard g(statsLock);
        stats.framesSent++;
        stats.bytesSent += bytes;
    }

    void SetActive(int n)
    {
        WbGuard g(statsLock);
        stats.activeClients = n;
    }

    // Queue a frame on one peer. Dropping is the caller's job (see Loop).
    static void Queue(Peer& p, const std::string& frame) { p.out += frame; }

    void DropPeer(std::size_t i, const char* /*why*/)
    {
        CloseSock(peers[i].sock);
        peers.erase(peers.begin() + static_cast<long>(i));
        WbGuard g(statsLock);
        stats.clientsDropped++;
    }

    static void ThreadEntry(void* arg) { static_cast<Impl*>(arg)->Loop(); }

    void Loop();
};

void TcpTagPublisher::Impl::Loop()
{
    ::InterlockedExchange(&running, 1);
    startMs = NowMs();

    while (::InterlockedCompareExchange(&stopFlag, 0, 0) == 0) {

        fd_set rd, wr;
        FD_ZERO(&rd);
        FD_ZERO(&wr);
        FD_SET(listener, &rd);
        for (std::size_t i = 0; i < peers.size(); ++i) {
            FD_SET(peers[i].sock, &rd);                       // inbound = discard
            if (!peers[i].out.empty()) FD_SET(peers[i].sock, &wr);
        }

        timeval tv;
        tv.tv_sec  = cfg.pollIntervalMs / 1000;
        tv.tv_usec = (cfg.pollIntervalMs % 1000) * 1000;
        // First arg is ignored on Winsock; passing 0 is correct and portable
        // enough here because this file is Windows-only by construction.
        const int sel = ::select(0, &rd, &wr, NULL, &tv);
        if (sel == SOCKET_ERROR) break;

        // ---- accept ------------------------------------------------------
        if (FD_ISSET(listener, &rd)) {
            SOCKET s = ::accept(listener, NULL, NULL);
            if (s != INVALID_SOCKET) {
                if (static_cast<int>(peers.size()) >= cfg.maxConnections) {
                    // Refuse rather than evict: an established consumer that is
                    // working is worth more than a new one that might not be.
                    ::closesocket(s);
                } else {
                    SetNonBlocking(s);
                    Peer p;
                    p.sock = s;
                    p.lastWriteMs = NowMs();
                    // Hello + a FULL snapshot, so a consumer is correct from
                    // its first frame and never has to ask for anything.
                    const TagSnapshotView v = source.read();
                    Queue(p, FrameHello(v.generation));
                    Queue(p, FrameSnapshot(v));
                    p.lastSent = v;
                    peers.push_back(p);
                    WbGuard g(statsLock);
                    stats.clientsAccepted++;
                }
            }
        }

        // ---- per-peer: inbound drain, diff, send -------------------------
        for (std::size_t i = 0; i < peers.size(); ) {
            Peer& p = peers[i];
            bool drop = false;

            // Inbound is DISCARDED (read-only link). Bounded on purpose --
            // see the header's note about Command.cpp:12697's 100-byte buffer.
            if (FD_ISSET(p.sock, &rd)) {
                char scratch[2048];
                std::size_t burst = 0;
                for (;;) {
                    const int n = ::recv(p.sock, scratch, sizeof(scratch), 0);
                    if (n > 0) {
                        burst += static_cast<std::size_t>(n);
                        WbGuard g(statsLock);
                        stats.inboundBytesDiscarded += static_cast<std::uint64_t>(n);
                        if (burst > cfg.maxInboundBurst) { drop = true; break; }
                        continue;
                    }
                    if (n == 0) { drop = true; break; }            // peer closed
                    if (::WSAGetLastError() != WSAEWOULDBLOCK) drop = true;
                    break;
                }
            }

            // New generation -> queue a patch for THIS peer's position.
            if (!drop) {
                const TagPatch patch = source.diffFrom(p.lastSent);
                if (!patch.empty()) {
                    Queue(p, FramePatch(patch));
                    applyPatch(p.lastSent.tags, patch);
                    p.lastSent.generation = patch.generation;
                } else if (NowMs() - p.lastWriteMs >= static_cast<unsigned long long>(cfg.heartbeatMs)) {
                    // Silence is indistinguishable from a hung publisher on the
                    // consumer side, so say something on a timer.
                    Queue(p, FramePing(NowMs() - startMs));
                }
            }

            // A peer that stopped reading must never stall this loop: it is the
            // one that gets dropped, not the machine that gets delayed.
            if (!drop && p.out.size() > cfg.maxSendBacklog) drop = true;

            if (!drop && !p.out.empty()) {
                const int n = ::send(p.sock, p.out.data(),
                                     static_cast<int>(p.out.size()), 0);
                if (n > 0) {
                    p.out.erase(0, static_cast<std::size_t>(n));
                    p.lastWriteMs = NowMs();
                    BumpSent(static_cast<std::size_t>(n));
                } else if (n == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
                    drop = true;
                }
            }

            if (drop) DropPeer(i, "peer error/backlog");
            else      ++i;
        }

        SetActive(static_cast<int>(peers.size()));
    }

    for (std::size_t i = 0; i < peers.size(); ++i) CloseSock(peers[i].sock);
    peers.clear();
    SetActive(0);
    ::InterlockedExchange(&running, 0);
}

TcpTagPublisher::TcpTagPublisher(const TagSnapshot& source, const TcpPublisherConfig& cfg)
    : impl_(new Impl(source, cfg))
{
}

TcpTagPublisher::~TcpTagPublisher()
{
    Stop();
    delete impl_;
}

bool TcpTagPublisher::Start(std::string& error)
{
    error.clear();
    if (!impl_->winsock.ok()) { error = "WSAStartup failed"; return false; }
    if (impl_->listener != INVALID_SOCKET) { error = "already started"; return false; }

    SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) { error = LastErrText("socket()"); return false; }

    // No SO_REUSEADDR on purpose: on Windows it permits two processes to bind
    // the SAME port and silently split connections between them. A second
    // publisher must fail loudly instead of half-working.
    sockaddr_in addr;
    ::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port   = ::htons(impl_->cfg.port);
    if (!ResolveIPv4(impl_->cfg.bindAddress, addr.sin_addr)) {
        ::closesocket(s);
        error = "bad bindAddress '" + impl_->cfg.bindAddress + "'";
        return false;
    }

    if (::bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        ::closesocket(s);
        error = LastErrText("bind()");
        return false;
    }
    if (::listen(s, SOMAXCONN) == SOCKET_ERROR) {
        ::closesocket(s);
        error = LastErrText("listen()");
        return false;
    }

    sockaddr_in bound;
    int blen = sizeof(bound);
    if (::getsockname(s, reinterpret_cast<sockaddr*>(&bound), &blen) == 0) {
        impl_->boundPort = ::ntohs(bound.sin_port);
    }

    SetNonBlocking(s);
    impl_->listener = s;
    ::InterlockedExchange(&impl_->stopFlag, 0);

    if (!impl_->thread.start(&Impl::ThreadEntry, impl_)) {
        CloseSock(impl_->listener);
        error = "CreateThread failed";
        return false;
    }
    return true;
}

void TcpTagPublisher::Stop()
{
    if (impl_ == NULL) return;
    ::InterlockedExchange(&impl_->stopFlag, 1);
    if (impl_->thread.joinable()) impl_->thread.join();
    CloseSock(impl_->listener);
}

bool TcpTagPublisher::Running() const
{
    return ::InterlockedCompareExchange(&impl_->running, 0, 0) != 0;
}

unsigned short TcpTagPublisher::BoundPort() const { return impl_->boundPort; }

TcpTagPublisher::Stats TcpTagPublisher::GetStats() const
{
    WbGuard g(impl_->statsLock);
    return impl_->stats;
}

// ===========================================================================
//  TcpTagClient
// ===========================================================================
struct TcpTagClient::Impl {
    TagSnapshot&     sink;
    TcpClientConfig  cfg;

    WinsockScope     winsock;
    WbThread         thread;
    volatile LONG    stopFlag;
    volatile LONG    running;
    volatile LONG    connected;

    mutable WbMutex  statsLock;
    Stats            stats;

    Impl(TagSnapshot& s, const TcpClientConfig& c)
        : sink(s), cfg(c), stopFlag(0), running(0), connected(0) {}

    static void ThreadEntry(void* arg) { static_cast<Impl*>(arg)->Loop(); }

    void Loop();
    bool ConnectOnce(SOCKET& out);
    void ServeConnection(SOCKET s);
    void HandleLine(const std::string& line);
};

// Non-blocking connect with a bounded wait: a blocking connect() to a dead
// port can sit for the OS default (seconds), and Stop() would then wait on it.
bool TcpTagClient::Impl::ConnectOnce(SOCKET& out)
{
    out = INVALID_SOCKET;

    SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) return false;

    sockaddr_in addr;
    ::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port   = ::htons(cfg.port);
    if (!ResolveIPv4(cfg.host, addr.sin_addr)) {
        ::closesocket(s);
        return false;
    }

    SetNonBlocking(s);
    if (::connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR &&
        ::WSAGetLastError() != WSAEWOULDBLOCK) {
        ::closesocket(s);
        return false;
    }

    fd_set wr, ex;
    FD_ZERO(&wr); FD_ZERO(&ex);
    FD_SET(s, &wr); FD_SET(s, &ex);
    timeval tv;
    tv.tv_sec  = 2;
    tv.tv_usec = 0;
    if (::select(0, NULL, &wr, &ex, &tv) <= 0 || FD_ISSET(s, &ex)) {
        ::closesocket(s);
        return false;
    }

    out = s;
    return true;
}

void TcpTagClient::Impl::HandleLine(const std::string& line)
{
    if (line.empty()) return;

    cJSON* root = cJSON_Parse(line.c_str());
    if (root == NULL) {
        WbGuard g(statsLock);
        stats.parseErrors++;
        return;
    }

    const cJSON* jType = cJSON_GetObjectItemCaseSensitive(root, "type");
    const std::string type =
        (jType && cJSON_IsString(jType) && jType->valuestring) ? jType->valuestring : "";

    const cJSON* jGen = cJSON_GetObjectItemCaseSensitive(root, "gen");
    const std::uint64_t gen =
        (jGen && cJSON_IsNumber(jGen)) ? static_cast<std::uint64_t>(jGen->valuedouble) : 0;

    if (type == "snapshot" || type == "patch") {
        const cJSON* jData = cJSON_GetObjectItemCaseSensitive(root, "data");
        TagMap data;
        std::string err;
        if (!DecodeTagObjectNode(jData, data, err)) {
            cJSON_Delete(root);
            WbGuard g(statsLock);
            stats.parseErrors++;
            return;
        }

        if (type == "snapshot") {
            // Full replace: a snapshot IS the state, so anything not in it is
            // gone. publish() = beginPublish + stageFrom + commit.
            sink.publish(data);
        } else {
            // Merge onto what we already hold. TagSnapshot has no partial
            // update -- beginPublish() clears staging by design (see its
            // header) -- so the merge happens here and the result is published
            // whole. This thread is the sink's only publisher, so read() cannot
            // race a concurrent commit.
            TagSnapshotView cur = sink.read();
            for (TagMap::const_iterator it = data.begin(); it != data.end(); ++it) {
                cur.tags[it->first] = it->second;
            }
            const cJSON* jRem = cJSON_GetObjectItemCaseSensitive(root, "removed");
            if (jRem && cJSON_IsArray(jRem)) {
                for (const cJSON* r = jRem->child; r != NULL; r = r->next) {
                    if (cJSON_IsString(r) && r->valuestring) cur.tags.erase(r->valuestring);
                }
            }
            sink.publish(cur.tags);
        }

        WbGuard g(statsLock);
        stats.framesReceived++;
        stats.lastGeneration = gen;
    } else {
        // hello / ping / anything a newer publisher invents: counted, ignored.
        // Forward compatibility is deliberate -- an unknown frame type must
        // never take the link down.
        WbGuard g(statsLock);
        stats.framesReceived++;
    }

    cJSON_Delete(root);
}

void TcpTagClient::Impl::ServeConnection(SOCKET s)
{
    std::string buf;
    unsigned long long lastRx = NowMs();

    while (::InterlockedCompareExchange(&stopFlag, 0, 0) == 0) {
        fd_set rd;
        FD_ZERO(&rd);
        FD_SET(s, &rd);
        timeval tv;
        tv.tv_sec  = 0;
        tv.tv_usec = 200 * 1000;

        const int sel = ::select(0, &rd, NULL, NULL, &tv);
        if (sel == SOCKET_ERROR) break;

        if (sel > 0 && FD_ISSET(s, &rd)) {
            char scratch[8192];
            const int n = ::recv(s, scratch, sizeof(scratch), 0);
            if (n == 0) break;                                   // publisher closed
            if (n < 0) {
                if (::WSAGetLastError() == WSAEWOULDBLOCK) continue;
                break;
            }
            buf.append(scratch, static_cast<std::size_t>(n));
            lastRx = NowMs();
            {
                WbGuard g(statsLock);
                stats.bytesReceived += static_cast<std::uint64_t>(n);
            }

            for (;;) {
                const std::size_t nl = buf.find('\n');
                if (nl == std::string::npos) {
                    // A line that never terminates is either a broken publisher
                    // or an attempt to exhaust this process's memory. Neither
                    // is worth tolerating.
                    if (buf.size() > cfg.maxLineBytes) return;
                    break;
                }
                const std::string line = buf.substr(0, nl);
                buf.erase(0, nl + 1);
                HandleLine(line);
            }
        }

        // Silence beyond the heartbeat window means the link is dead even
        // though TCP has not noticed: reconnect rather than sit on a socket
        // that will never speak again.
        if (NowMs() - lastRx > static_cast<unsigned long long>(cfg.staleTimeoutMs)) return;
    }
}

void TcpTagClient::Impl::Loop()
{
    ::InterlockedExchange(&running, 1);

    while (::InterlockedCompareExchange(&stopFlag, 0, 0) == 0) {
        SOCKET s = INVALID_SOCKET;
        if (ConnectOnce(s)) {
            ::InterlockedExchange(&connected, 1);
            {
                WbGuard g(statsLock);
                stats.connected = true;
                stats.reconnects++;
            }
            ServeConnection(s);
            CloseSock(s);
            ::InterlockedExchange(&connected, 0);
            WbGuard g(statsLock);
            stats.connected = false;
        }

        // Sleep in slices so Stop() is prompt even with a long reconnect delay.
        for (int waited = 0;
             waited < cfg.reconnectMs &&
             ::InterlockedCompareExchange(&stopFlag, 0, 0) == 0;
             waited += 50) {
            ::Sleep(50);
        }
    }

    ::InterlockedExchange(&running, 0);
}

TcpTagClient::TcpTagClient(TagSnapshot& sink, const TcpClientConfig& cfg)
    : impl_(new Impl(sink, cfg))
{
}

TcpTagClient::~TcpTagClient()
{
    Stop();
    delete impl_;
}

bool TcpTagClient::Start(std::string& error)
{
    error.clear();
    if (!impl_->winsock.ok()) { error = "WSAStartup failed"; return false; }
    ::InterlockedExchange(&impl_->stopFlag, 0);
    if (!impl_->thread.start(&Impl::ThreadEntry, impl_)) {
        error = "CreateThread failed";
        return false;
    }
    return true;
}

void TcpTagClient::Stop()
{
    if (impl_ == NULL) return;
    ::InterlockedExchange(&impl_->stopFlag, 1);
    if (impl_->thread.joinable()) impl_->thread.join();
}

bool TcpTagClient::Running() const
{
    return ::InterlockedCompareExchange(&impl_->running, 0, 0) != 0;
}

bool TcpTagClient::Connected() const
{
    return ::InterlockedCompareExchange(&impl_->connected, 0, 0) != 0;
}

TcpTagClient::Stats TcpTagClient::GetStats() const
{
    WbGuard g(impl_->statsLock);
    return impl_->stats;
}

} // namespace webbridge
