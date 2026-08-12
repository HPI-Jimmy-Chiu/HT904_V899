// =============================================================================
//  WebBridge/WebBridgeServer.cpp -- see WebBridgeServer.h for the threading
//  contract. If you are about to make this thread touch machine state, stop and
//  read ARCHITECTURE.md section 5 first.
// =============================================================================

// select() carries the listener + the wakeup socket + every client, so raise
// the fd_set capacity before <winsock2.h> is pulled in. maxConnections is
// clamped against this below.
#ifndef FD_SETSIZE
#define FD_SETSIZE 128
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

#include "WebBridge/WebBridgeServer.h"

#include "WebBridge/JsonWriter.h"
#include "WebBridge/TagJson.h"
#include "WebBridge/WsFrame.h"
#include "WebBridge/WsHandshake.h"
#include "WebBridge/Sync.h"

#include "Public/cJSON.h"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <deque>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#if defined(_MSC_VER)
#pragma comment(lib, "ws2_32.lib")
#endif

namespace webbridge {

// =============================================================================
//  SIBLING ADAPTER -- the ONLY code in this file that calls into the sibling
//  WebBridge components (WsHandshake / WsFrame / TagValue / TagSnapshot /
//  CommandQueue / JsonWriter), which are authored in parallel with this one.
//
//  This file was written against ASSUMED sibling signatures while the siblings
//  were being written in parallel. Every one of those guesses was wrong, and
//  they were reconciled here on 20260805. What follows is the REAL API this
//  adapter binds to -- kept accurate on purpose, because a comment describing
//  an API that does not exist costs more than no comment at all.
//
//    WsHandshake.h  std::string ComputeAcceptKey(const std::string& key);
//                       // base64(SHA1(key + RFC6455 GUID))
//    WsFrame.h      enum WsOpcode { kWsContinuation=0x0, kWsText=0x1,
//                       kWsBinary=0x2, kWsClose=0x8, kWsPing=0x9, kWsPong=0xA };
//                   std::string EncodeFrame(int opcode, const std::string& payload,
//                       bool fin=true, bool mask=false, uint32_t maskKey=0);
//                   class WsDecoder {                     // STATEFUL, per conn
//                       explicit WsDecoder(bool requireMaskedInput=true,
//                                          std::size_t maxMessageBytes=...);
//                       bool Feed(const std::string&, std::vector<WsMessage>*);
//                       bool Failed() const; uint16_t CloseCode() const;
//                       std::size_t PendingBytes() const, FragmentBytes() const; };
//                   // WsDecoder reassembles fragments and enforces masking,
//                   // UTF-8 and the size cap itself, so a delivered WsMessage
//                   // is always whole -- which is why sib::Frame::consumed is
//                   // dead and Conn::fragment/fragmenting are now unreachable.
//    TagValue.h     static TagValue makeNull()/makeBool(bool)/makeInt(int64)/
//                                   makeDouble(double)/makeString(const string&);
//                   TagType type() const;  asBool/asInt/asDouble/asString;
//                   bool operator==(const TagValue&) const;   // type-strict
//    TagSnapshot.h  TagSnapshotView read() const;   // { TagMap tags; uint64 generation; }
//                   TagPatch diffFrom(const TagSnapshotView&) const;
//                   // NOTE: diffFrom() would replace this file's hand-rolled
//                   // per-connection delta in PublishIfChanged(). Left alone for
//                   // now because that path has no test yet; see the DEVLOG.
//    CommandQueue.h struct WebCommand { uint64 id; string cmd; string tag;
//                                       bool hasTag; TagValue value;
//                                       bool hasValue; uint64 connId; };
//                   bool tryPush(const WebCommand&);   // false when full
//    JsonWriter.h   class JsonWriter { JsonWriter& BeginObject()/EndObject()/
//                       Key(const string&)/Null()/Bool(bool)/Number(wb_int64)/
//                       Number(double)/String(const string&)/RawValue(const string&);
//                       bool Ok() const; const std::string& Str() const; };
//                   std::string JsonQuote(const std::string& raw);
//                   // JsonWriter has its own JsonValue variant, unrelated to
//                   // TagValue; WebBridge/TagJson.h owns that mapping (both
//                   // directions) and sib::ObjectFrom forwards to it.
// =============================================================================
namespace sib {

// --- WsHandshake ------------------------------------------------------------
static std::string AcceptKey(const std::string& clientKey)
{
    return ComputeAcceptKey(clientKey);
}

// --- WsFrame ----------------------------------------------------------------
enum {
    kOpCont   = kWsContinuation,
    kOpText   = kWsText,
    kOpBinary = kWsBinary,
    kOpClose  = kWsClose,
    kOpPing   = kWsPing,
    kOpPong   = kWsPong
};

struct Frame {
    int         opcode;
    bool        fin;
    bool        masked;
    std::string payload;
    size_t      consumed;   // always 0: the decoder owns consumption now
    Frame() : opcode(0), fin(false), masked(false), consumed(0) {}
};

// Server-to-client frames are never masked (RFC 6455 section 5.1).
static std::string EncodeServerFrame(int opcode, const std::string& payload)
{
    return EncodeFrame(opcode, payload, /*fin=*/true, /*mask=*/false, /*maskKey=*/0);
}

// One per connection. WsDecoder is stateful -- it buffers partial frames and
// reassembles fragmented messages across TCP chunk boundaries -- so it cannot
// be a free function over the socket buffer the way this adapter first assumed.
struct Decoder {
    WsDecoder                dec;
    std::deque<WsMessage>    ready;

    // Server role: inbound frames MUST be masked, and the decoder enforces it.
    // The cap matches this file's own kMaxWsMessage so oversize is rejected by
    // the decoder (close 1009) rather than after reassembly.
    Decoder() : dec(/*requireMaskedInput=*/true, 64u * 1024u) {}
};

static size_t PendingBytes(const Decoder& d)
{
    return d.dec.PendingBytes() + d.dec.FragmentBytes();
}

// 1 = one message ready, 0 = need more bytes, -1 = protocol error.
//
// `in` is drained completely into the decoder; anything not yet a whole message
// stays inside the decoder, which is why Frame::consumed is always 0 and the
// caller must NOT erase from `in` itself.
static int TryDecode(Decoder& d, std::string& in, Frame& out)
{
    if (d.ready.empty()) {
        if (d.dec.Failed()) return -1;
        if (!in.empty()) {
            std::vector<WsMessage> got;
            const bool ok = d.dec.Feed(in, &got);
            in.clear();
            for (size_t i = 0; i < got.size(); ++i) d.ready.push_back(got[i]);
            if (!ok) {
                // Deliver whatever completed before the failure, then fail.
                if (d.ready.empty()) return -1;
            }
        }
        if (d.ready.empty()) return 0;
    }

    const WsMessage& m = d.ready.front();
    out.opcode  = m.opcode;
    // Data messages arrive fully reassembled, so a delivered message is always
    // a complete one; masking was enforced by the decoder before delivery.
    out.fin     = true;
    out.masked  = true;
    out.payload = m.payload;
    out.consumed = 0;
    d.ready.pop_front();
    return 1;
}

// --- TagValue ---------------------------------------------------------------
static TagValue MakeNull()                        { return TagValue::makeNull(); }
static TagValue MakeBool(bool v)                  { return TagValue::makeBool(v); }
static TagValue MakeNumber(double v)              { return TagValue::makeDouble(v); }
static TagValue MakeString(const std::string& v)  { return TagValue::makeString(v); }
static bool ValuesEqual(const TagValue& a, const TagValue& b) { return a == b; }

// --- TagSnapshot ------------------------------------------------------------
static unsigned long long SnapGeneration(const TagSnapshot* s)
{
    // generation() and NOT read().generation: read() copies the entire tag map
    // to build its view, and this is called on every poll iteration purely to
    // decide whether anything changed. Copying ~270 tags to learn "no" is the
    // kind of waste that only shows up under load.
    return s ? static_cast<unsigned long long>(s->generation()) : 0;
}

static void SnapRead(const TagSnapshot* s, std::map<std::string, TagValue>& out)
{
    out.clear();
    if (s) out = s->read().tags;
}

// --- CommandQueue -----------------------------------------------------------
static bool QueuePush(CommandQueue* q, unsigned long long ticket,
                      const std::string& cmd, const std::string& tag,
                      const TagValue& value)
{
    if (!q) return false;
    WebCommand c;
    c.id       = ticket;
    c.cmd      = cmd;
    c.tag      = tag;
    c.hasTag   = !tag.empty();
    c.value    = value;
    c.hasValue = !value.isNull();
    return q->tryPush(c);
}

// --- JsonWriter -------------------------------------------------------------
// AI(W906-WebBridge-Tcp) 20260812: the TagValue -> JsonWriter mapping that used
// to live here as sib::WriteValue now lives in WebBridge/TagJson.cpp, which is
// also where the reverse direction lives. The Null vs "" distinction it
// preserves is unchanged and still load-bearing: the browser renders null as
// "---" and "" as blank, and collapsing them misreports an uninstalled device
// as a real zero.

// {"tag":value,"tag2":value2}
//
// AI(W906-WebBridge-Tcp) 20260812: delegated to WebBridge/TagJson.h. This used
// to be the only tag encoder in the tree; the TCP sidecar link now needs the
// identical bytes (and the decode direction, which has no counterpart here), so
// the definition moved to TagJson and this became a forwarder. Keeping a second
// hand-maintained copy is how the two wires would silently drift apart.
static std::string ObjectFrom(const std::map<std::string, TagValue>& m)
{
    return EncodeTagObject(m);
}

// A JSON string literal, quotes included.
static std::string QuoteString(const std::string& s)
{
    return JsonQuote(s);
}

}  // namespace sib
// =============================== END SIBLING ADAPTER =========================

namespace {

const size_t kMaxHttpHead    = 32u * 1024u;   // request head before we give up
const size_t kMaxWsMessage   = 64u * 1024u;   // reassembled text message cap
const size_t kMaxPendingAcks = 4096u;         // unanswered CompleteCommand slots
const size_t kRecvChunk      = 8192u;

unsigned long long NowMs()
{
    using namespace std::chrono;
    return static_cast<unsigned long long>(
        duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

char LowerAscii(char c)
{
    return (c >= 'A' && c <= 'Z') ? static_cast<char>(c - 'A' + 'a') : c;
}

std::string Lower(const std::string& s)
{
    std::string o(s);
    for (size_t i = 0; i < o.size(); ++i) o[i] = LowerAscii(o[i]);
    return o;
}

std::string Trim(const std::string& s)
{
    size_t b = 0, e = s.size();
    while (b < e && (s[b] == ' ' || s[b] == '\t')) ++b;
    while (e > b && (s[e - 1] == ' ' || s[e - 1] == '\t' ||
                     s[e - 1] == '\r' || s[e - 1] == '\n')) --e;
    return s.substr(b, e - b);
}

bool ContainsCI(const std::string& hay, const char* needle)
{
    return Lower(hay).find(needle) != std::string::npos;
}

// A command / tag name we are willing to hand to the UI thread. Deliberately
// narrow: these strings come off a socket and end up selecting machine actions.
bool IsSaneName(const std::string& s, size_t maxLen)
{
    if (s.size() > maxLen) return false;
    for (size_t i = 0; i < s.size(); ++i) {
        const char c = s[i];
        const bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                        (c >= '0' && c <= '9') ||
                        c == '.' || c == '_' || c == '-';
        if (!ok) return false;
    }
    return true;
}

std::string IsoLocalNow()
{
    std::time_t t = std::time(0);
    std::tm tmv;
#if defined(_MSC_VER)
    localtime_s(&tmv, &t);
#else
    std::tm* p = std::localtime(&t);
    if (p) tmv = *p; else std::memset(&tmv, 0, sizeof(tmv));
#endif
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d",
                  tmv.tm_year + 1900, tmv.tm_mon + 1, tmv.tm_mday,
                  tmv.tm_hour, tmv.tm_min, tmv.tm_sec);
    return std::string(buf);
}

// --- Winsock reference counting ---------------------------------------------
// Two servers in one process, or a host app that already called WSAStartup,
// must both keep working. Winsock itself refcounts, but we still balance our
// own calls exactly so our WSACleanup never pulls the rug out from under the
// host application.
WbMutex g_wsaMx;
int        g_wsaRefs = 0;

bool WsaAcquire(std::string* err)
{
    WbGuard lk(g_wsaMx);
    if (g_wsaRefs > 0) { ++g_wsaRefs; return true; }
    WSADATA wsad;
    const int rc = WSAStartup(MAKEWORD(2, 2), &wsad);
    if (rc != 0) {
        if (err) {
            std::ostringstream os;
            os << "WSAStartup failed, rc=" << rc;
            *err = os.str();
        }
        return false;
    }
    g_wsaRefs = 1;
    return true;
}

void WsaRelease()
{
    WbGuard lk(g_wsaMx);
    if (g_wsaRefs <= 0) return;
    if (--g_wsaRefs == 0) WSACleanup();
}

void SetNonBlocking(SOCKET s)
{
    u_long nb = 1;
    ioctlsocket(s, FIONBIO, &nb);
}

std::string WsaErrText(const char* what, int code)
{
    std::ostringstream os;
    os << what << " failed, WSAGetLastError=" << code;
    return os.str();
}

}  // namespace

// =============================================================================
//  WebBridgeConfig / WebBridgeStats
// =============================================================================
WebBridgeConfig::WebBridgeConfig()
    : bindAddress("127.0.0.1"),   // loopback only -- see header
      port(8045),
      documentRoot(),
      maxConnections(16),
      readOnly(true),             // read-only by default -- see header
      wsPath("/ht9045"),
      pingIntervalMs(15000),
      idleTimeoutMs(45000),
      pollIntervalMs(50),
      maxSendBacklog(256u * 1024u)
{
}

WebBridgeStats::WebBridgeStats()
    : httpRequests(0), wsAccepted(0), wsRejected(0), snapshotsSent(0),
      patchesSent(0), alarmsSent(0), acksSent(0), cmdAccepted(0),
      cmdRejected(0), pingsSent(0), pongsReceived(0), slowClientDrops(0),
      connectionsAccepted(0), connectionsClosed(0), liveConnections(0)
{
}

// =============================================================================
//  Impl
// =============================================================================
class WebBridgeServer::Impl {
public:
    explicit Impl(const WebBridgeConfig& cfg);
    ~Impl();

    bool Start(std::string* errOut);
    void Stop();
    void Wake();

    void CompleteCommand(unsigned long long ticket, bool ok, const std::string& error);
    void PostAlarm(const std::string& code, const std::string& text, const std::string& at);

    WebBridgeConfig      cfg;
    HttpStatic           files;
    TagSnapshot*         snapshot;
    CommandQueue*        queue;
    std::atomic<bool>    readOnly;
    std::atomic<bool>    running;
    std::atomic<bool>    stopFlag;
    std::atomic<unsigned short> boundPort;

    mutable WbMutex   statsMx;
    WebBridgeStats       stats;

private:
    // --- one connection, owned solely by the socket thread ------------------
    struct Conn {
        SOCKET             s;
        unsigned long long id;
        bool               isWs;
        bool               closeAfterFlush;
        std::string        in;
        std::string        out;
        // The frame decoder is per-connection and stateful: it buffers partial
        // frames and reassembles fragmented messages across TCP chunk
        // boundaries. Because it reassembles, `fragment`/`fragmenting` below
        // are never exercised any more -- a delivered message is always whole.
        sib::Decoder       dec;
        std::string        fragment;      // text message being reassembled
        bool               fragmenting;
        bool               sentSnapshot;
        std::map<std::string, TagValue> lastSent;   // this connection's view
        unsigned long long lastRecvMs;
        unsigned long long lastPingMs;
        bool               awaitingPong;

        Conn()
            : s(INVALID_SOCKET), id(0), isWs(false), closeAfterFlush(false),
              fragmenting(false), sentSnapshot(false), lastRecvMs(0),
              lastPingMs(0), awaitingPong(false) {}
    };

    struct Outgoing {
        unsigned long long connId;   // 0 = broadcast to every WS connection
        std::string        frame;
    };

    struct PendingAck {
        unsigned long long connId;
        double             browserId;
    };

    // WbThread takes a plain function pointer, so ThreadMain needs a static
    // trampoline. WebBridge/Sync.h explains why the standard thread type is
    // unusable on this tree's MinGW oracle.
    static void ThreadEntry(void* self);
    void ThreadMain();
    void CloseAllSockets();

    void AcceptNew();
    bool ReceiveInto(Conn& c);                 // false -> close this connection
    void Flush(Conn& c);
    void Enqueue(Conn& c, const std::string& bytes);
    void CloseConn(size_t index);

    bool ProcessHttpHead(Conn& c);             // false -> close
    bool DoWebSocketUpgrade(Conn& c, const std::string& target,
                            const std::map<std::string, std::string>& headers);
    bool ProcessWsBytes(Conn& c);              // false -> close
    void HandleTextMessage(Conn& c, const std::string& text);

    void PumpSnapshot(bool force);
    void PumpOutgoing();
    void PumpLiveness();

    void SendJson(Conn& c, const std::string& json);
    void SendAck(Conn& c, double id, bool ok, const std::string& error);
    static std::string AckJson(double id, bool ok, const std::string& error);

    std::vector<Conn>       conns_;
    WbThread                th_;
    WbMutex              lifeMx_;

    SOCKET                  listener_;
    SOCKET                  wake_;
    sockaddr_in             wakeAddr_;

    unsigned long long      lastGen_;
    unsigned long long      nextConnId_;
    std::atomic<unsigned long long> nextTicket_;

    WbMutex              outMx_;
    std::deque<Outgoing>    outQ_;

    WbMutex              pendMx_;
    std::map<unsigned long long, PendingAck> pending_;
    std::deque<unsigned long long>           pendingOrder_;
};

// -----------------------------------------------------------------------------
WebBridgeServer::Impl::Impl(const WebBridgeConfig& c)
    : cfg(c),
      files(c.documentRoot),
      snapshot(0),
      queue(0),
      readOnly(c.readOnly),
      running(false),
      stopFlag(false),
      boundPort(0),
      listener_(INVALID_SOCKET),
      wake_(INVALID_SOCKET),
      lastGen_(0),
      nextConnId_(1),
      nextTicket_(1)
{
    std::memset(&wakeAddr_, 0, sizeof(wakeAddr_));

    // select() must be able to hold listener + wakeup + every client.
    const int cap = static_cast<int>(FD_SETSIZE) - 8;
    if (cfg.maxConnections < 1)   cfg.maxConnections = 1;
    if (cfg.maxConnections > cap) cfg.maxConnections = cap;
    if (cfg.pollIntervalMs < 1)   cfg.pollIntervalMs = 1;
    if (cfg.wsPath.empty())       cfg.wsPath = "/ht9045";
    if (cfg.maxSendBacklog < 64u * 1024u) cfg.maxSendBacklog = 64u * 1024u;
}

WebBridgeServer::Impl::~Impl()
{
    Stop();
}

// -----------------------------------------------------------------------------
bool WebBridgeServer::Impl::Start(std::string* errOut)
{
    WbGuard lk(lifeMx_);
    if (running.load()) return true;   // idempotent

    if (!WsaAcquire(errOut)) return false;

    stopFlag.store(false);

    // --- listener, bound on the CALLING thread so failures are synchronous --
    listener_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener_ == INVALID_SOCKET) {
        if (errOut) *errOut = WsaErrText("socket(listener)", WSAGetLastError());
        WsaRelease();
        return false;
    }

    // NOTE: SO_REUSEADDR is deliberately NOT set. On Windows it permits another
    // process to steal a bound port, and it would also mask a leaked listener
    // from our own lifecycle test.
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(cfg.port);
    {
        // getaddrinfo rather than inet_addr/inet_pton: present and
        // non-deprecated on both MinGW and MSVC.
        addrinfo hints;
        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family   = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags    = AI_NUMERICHOST;
        addrinfo* ai = 0;
        const std::string host = cfg.bindAddress.empty() ? std::string("127.0.0.1")
                                                         : cfg.bindAddress;
        int rc = getaddrinfo(host.c_str(), 0, &hints, &ai);
        if (rc != 0) {                       // allow "localhost" and friends
            hints.ai_flags = 0;
            rc = getaddrinfo(host.c_str(), 0, &hints, &ai);
        }
        if (rc != 0 || !ai) {
            if (ai) freeaddrinfo(ai);
            closesocket(listener_);
            listener_ = INVALID_SOCKET;
            if (errOut) *errOut = "cannot resolve bind address '" + host + "'";
            WsaRelease();
            return false;
        }
        const sockaddr_in* r = reinterpret_cast<const sockaddr_in*>(ai->ai_addr);
        addr.sin_addr = r->sin_addr;
        freeaddrinfo(ai);
    }

    if (bind(listener_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        const int e = WSAGetLastError();
        closesocket(listener_);
        listener_ = INVALID_SOCKET;
        if (errOut) *errOut = WsaErrText("bind", e);
        WsaRelease();
        return false;
    }
    if (listen(listener_, SOMAXCONN) == SOCKET_ERROR) {
        const int e = WSAGetLastError();
        closesocket(listener_);
        listener_ = INVALID_SOCKET;
        if (errOut) *errOut = WsaErrText("listen", e);
        WsaRelease();
        return false;
    }

    // Read back the real port so cfg.port == 0 (ephemeral) is usable.
    sockaddr_in got;
    std::memset(&got, 0, sizeof(got));
    int gotLen = sizeof(got);
    if (getsockname(listener_, reinterpret_cast<sockaddr*>(&got), &gotLen) == 0) {
        boundPort.store(ntohs(got.sin_port));
    } else {
        boundPort.store(cfg.port);
    }
    SetNonBlocking(listener_);

    // --- self-pipe: a loopback UDP socket we sendto() to break select() -----
    wake_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (wake_ == INVALID_SOCKET) {
        const int e = WSAGetLastError();
        closesocket(listener_);
        listener_ = INVALID_SOCKET;
        if (errOut) *errOut = WsaErrText("socket(wakeup)", e);
        WsaRelease();
        return false;
    }
    sockaddr_in wa;
    std::memset(&wa, 0, sizeof(wa));
    wa.sin_family = AF_INET;
    wa.sin_port   = 0;
    wa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(wake_, reinterpret_cast<sockaddr*>(&wa), sizeof(wa)) == SOCKET_ERROR) {
        const int e = WSAGetLastError();
        closesocket(wake_);   wake_ = INVALID_SOCKET;
        closesocket(listener_); listener_ = INVALID_SOCKET;
        if (errOut) *errOut = WsaErrText("bind(wakeup)", e);
        WsaRelease();
        return false;
    }
    int waLen = sizeof(wakeAddr_);
    if (getsockname(wake_, reinterpret_cast<sockaddr*>(&wakeAddr_), &waLen) != 0) {
        const int e = WSAGetLastError();
        closesocket(wake_);   wake_ = INVALID_SOCKET;
        closesocket(listener_); listener_ = INVALID_SOCKET;
        if (errOut) *errOut = WsaErrText("getsockname(wakeup)", e);
        WsaRelease();
        return false;
    }
    SetNonBlocking(wake_);

    lastGen_ = 0;
    running.store(true);
    th_.start(&WebBridgeServer::Impl::ThreadEntry, this);
    return true;
}

// -----------------------------------------------------------------------------
void WebBridgeServer::Impl::Stop()
{
    WbGuard lk(lifeMx_);
    stopFlag.store(true);
    if (th_.joinable()) {
        Wake();                 // prompt: do not wait out pollIntervalMs
        th_.join();
    }
    const bool wasRunning = running.exchange(false);

    // The socket thread closes the listener and the client sockets on its way
    // out; the wakeup socket is ours to close once nobody can select() on it.
    if (wake_ != INVALID_SOCKET) { closesocket(wake_); wake_ = INVALID_SOCKET; }
    if (listener_ != INVALID_SOCKET) { closesocket(listener_); listener_ = INVALID_SOCKET; }

    {
        WbGuard ol(outMx_);
        outQ_.clear();
    }
    {
        WbGuard pl(pendMx_);
        pending_.clear();
        pendingOrder_.clear();
    }
    boundPort.store(0);
    if (wasRunning) WsaRelease();   // balances the Start() that succeeded
}

void WebBridgeServer::Impl::Wake()
{
    if (wake_ == INVALID_SOCKET) return;
    const char b = 'w';
    sendto(wake_, &b, 1, 0, reinterpret_cast<sockaddr*>(&wakeAddr_), sizeof(wakeAddr_));
}

// -----------------------------------------------------------------------------
void WebBridgeServer::Impl::CloseAllSockets()
{
    for (size_t i = 0; i < conns_.size(); ++i) {
        if (conns_[i].s != INVALID_SOCKET) closesocket(conns_[i].s);
    }
    conns_.clear();
    if (listener_ != INVALID_SOCKET) { closesocket(listener_); listener_ = INVALID_SOCKET; }
    WbGuard sl(statsMx);
    stats.liveConnections = 0;
}

// -----------------------------------------------------------------------------
//  The socket thread. Everything below runs here and nowhere else.
// -----------------------------------------------------------------------------
void WebBridgeServer::Impl::ThreadEntry(void* self)
{
    static_cast<WebBridgeServer::Impl*>(self)->ThreadMain();
}

void WebBridgeServer::Impl::ThreadMain()
{
    while (!stopFlag.load()) {
        fd_set rd, wr;
        FD_ZERO(&rd);
        FD_ZERO(&wr);

        if (listener_ != INVALID_SOCKET &&
            static_cast<int>(conns_.size()) < cfg.maxConnections) {
            FD_SET(listener_, &rd);
        }
        if (wake_ != INVALID_SOCKET) FD_SET(wake_, &rd);

        for (size_t i = 0; i < conns_.size(); ++i) {
            FD_SET(conns_[i].s, &rd);
            if (!conns_[i].out.empty()) FD_SET(conns_[i].s, &wr);
        }

        timeval tv;
        tv.tv_sec  = cfg.pollIntervalMs / 1000;
        tv.tv_usec = (cfg.pollIntervalMs % 1000) * 1000;

        const int rc = select(0, &rd, &wr, 0, &tv);
        if (stopFlag.load()) break;

        if (rc == SOCKET_ERROR) {
            // A closed client between FD_SET and select() shows up here; drop
            // any dead socket and carry on rather than killing the thread.
            const int e = WSAGetLastError();
            if (e == WSAENOTSOCK || e == WSAEINVAL) {
                for (size_t i = conns_.size(); i-- > 0;) {
                    if (conns_[i].s == INVALID_SOCKET) CloseConn(i);
                }
                continue;
            }
            WbSleepMs(10);
            continue;
        }

        if (rc > 0) {
            if (wake_ != INVALID_SOCKET && FD_ISSET(wake_, &rd)) {
                char drain[64];
                sockaddr_in from;
                int fromLen = sizeof(from);
                while (recvfrom(wake_, drain, sizeof(drain), 0,
                                reinterpret_cast<sockaddr*>(&from), &fromLen) > 0) {
                    fromLen = sizeof(from);
                }
            }
            if (listener_ != INVALID_SOCKET && FD_ISSET(listener_, &rd)) AcceptNew();

            for (size_t i = conns_.size(); i-- > 0;) {
                Conn& c = conns_[i];
                bool keep = true;
                if (FD_ISSET(c.s, &rd)) keep = ReceiveInto(c);
                if (keep && FD_ISSET(c.s, &wr)) Flush(c);
                if (!keep) { CloseConn(i); continue; }
            }
        }

        PumpSnapshot(false);
        PumpOutgoing();
        PumpLiveness();

        // Flush whatever the pumps queued, and retire finished connections.
        for (size_t i = conns_.size(); i-- > 0;) {
            Conn& c = conns_[i];
            if (!c.out.empty()) Flush(c);
            if (c.out.size() > cfg.maxSendBacklog) {
                // One slow client must not stall the thread for everyone else.
                {
                    WbGuard sl(statsMx);
                    ++stats.slowClientDrops;
                }
                CloseConn(i);
                continue;
            }
            if (c.closeAfterFlush && c.out.empty()) { CloseConn(i); continue; }
            if (c.s == INVALID_SOCKET) { CloseConn(i); continue; }
        }
    }

    CloseAllSockets();
}

// -----------------------------------------------------------------------------
void WebBridgeServer::Impl::AcceptNew()
{
    for (;;) {
        sockaddr_in from;
        int fromLen = sizeof(from);
        const SOCKET s = accept(listener_, reinterpret_cast<sockaddr*>(&from), &fromLen);
        if (s == INVALID_SOCKET) return;

        if (static_cast<int>(conns_.size()) >= cfg.maxConnections) {
            closesocket(s);   // at capacity: refuse now, do not queue work
            continue;
        }

        SetNonBlocking(s);
        // Small frames, latency matters more than packing.
        int one = 1;
        setsockopt(s, IPPROTO_TCP, TCP_NODELAY,
                   reinterpret_cast<const char*>(&one), sizeof(one));

        Conn c;
        c.s          = s;
        c.id         = nextConnId_++;
        c.lastRecvMs = NowMs();
        c.lastPingMs = c.lastRecvMs;
        conns_.push_back(c);

        WbGuard sl(statsMx);
        ++stats.connectionsAccepted;
        stats.liveConnections = static_cast<int>(conns_.size());
    }
}

void WebBridgeServer::Impl::CloseConn(size_t index)
{
    if (index >= conns_.size()) return;
    if (conns_[index].s != INVALID_SOCKET) closesocket(conns_[index].s);
    conns_.erase(conns_.begin() + static_cast<long>(index));
    WbGuard sl(statsMx);
    ++stats.connectionsClosed;
    stats.liveConnections = static_cast<int>(conns_.size());
}

bool WebBridgeServer::Impl::ReceiveInto(Conn& c)
{
    for (;;) {
        char buf[kRecvChunk];
        const int n = recv(c.s, buf, static_cast<int>(sizeof(buf)), 0);
        if (n > 0) {
            c.in.append(buf, static_cast<size_t>(n));
            c.lastRecvMs = NowMs();
            if (n < static_cast<int>(sizeof(buf))) break;
            continue;
        }
        if (n == 0) return false;                        // peer closed
        const int e = WSAGetLastError();
        if (e == WSAEWOULDBLOCK) break;
        return false;
    }

    if (!c.isWs) {
        if (c.in.size() > kMaxHttpHead) return false;    // head never terminated
        return ProcessHttpHead(c);
    }
    return ProcessWsBytes(c);
}

void WebBridgeServer::Impl::Enqueue(Conn& c, const std::string& bytes)
{
    c.out += bytes;
}

void WebBridgeServer::Impl::Flush(Conn& c)
{
    while (!c.out.empty()) {
        const int n = send(c.s, c.out.data(), static_cast<int>(c.out.size()), 0);
        if (n > 0) {
            c.out.erase(0, static_cast<size_t>(n));
            continue;
        }
        const int e = WSAGetLastError();
        if (e == WSAEWOULDBLOCK) return;    // stays buffered; write-set retries
        closesocket(c.s);
        c.s = INVALID_SOCKET;
        return;
    }
}

// -----------------------------------------------------------------------------
//  HTTP: either a WebSocket upgrade on cfg.wsPath, or a static file.
// -----------------------------------------------------------------------------
bool WebBridgeServer::Impl::ProcessHttpHead(Conn& c)
{
    const size_t end = c.in.find("\r\n\r\n");
    if (end == std::string::npos) return true;      // wait for the rest

    const std::string head = c.in.substr(0, end);
    c.in.erase(0, end + 4);

    // Request line.
    const size_t eol = head.find("\r\n");
    const std::string reqLine = (eol == std::string::npos) ? head : head.substr(0, eol);
    std::string method, target, version;
    {
        std::istringstream is(reqLine);
        is >> method >> target >> version;
    }

    // Headers.
    std::map<std::string, std::string> headers;
    if (eol != std::string::npos) {
        size_t p = eol + 2;
        while (p < head.size()) {
            size_t e2 = head.find("\r\n", p);
            if (e2 == std::string::npos) e2 = head.size();
            const std::string line = head.substr(p, e2 - p);
            const size_t colon = line.find(':');
            if (colon != std::string::npos) {
                const std::string k = Lower(Trim(line.substr(0, colon)));
                const std::string v = Trim(line.substr(colon + 1));
                if (headers.count(k)) headers[k] += ", " + v;
                else                  headers[k] = v;
            }
            p = e2 + 2;
        }
    }

    if (method.empty() || target.empty()) {
        HttpResponse bad;
        bad.status = 400; bad.reason = "Bad Request";
        bad.contentType = "text/plain; charset=utf-8";
        bad.body = "400 malformed request line\n";
        bad.contentLength = static_cast<long long>(bad.body.size());
        Enqueue(c, bad.ToWire());
        c.closeAfterFlush = true;
        return true;
    }

    std::string path = target;
    const size_t q = path.find_first_of("?#");
    if (q != std::string::npos) path.erase(q);

    const bool wantsUpgrade =
        ContainsCI(headers.count("upgrade") ? headers["upgrade"] : std::string(), "websocket") &&
        ContainsCI(headers.count("connection") ? headers["connection"] : std::string(), "upgrade");

    if (wantsUpgrade) {
        if (method != "GET" || path != cfg.wsPath) {
            {
                WbGuard sl(statsMx);
                ++stats.wsRejected;
            }
            HttpResponse bad;
            bad.status = 404; bad.reason = "Not Found";
            bad.contentType = "text/plain; charset=utf-8";
            bad.body = "404 no websocket endpoint here\n";
            bad.contentLength = static_cast<long long>(bad.body.size());
            Enqueue(c, bad.ToWire());
            c.closeAfterFlush = true;
            return true;
        }
        return DoWebSocketUpgrade(c, path, headers);
    }

    {
        WbGuard sl(statsMx);
        ++stats.httpRequests;
    }
    const HttpResponse res = files.Serve(method, target);
    Enqueue(c, res.ToWire());
    c.closeAfterFlush = true;      // one request per connection, then close
    return true;
}

bool WebBridgeServer::Impl::DoWebSocketUpgrade(
    Conn& c, const std::string& /*target*/,
    const std::map<std::string, std::string>& headers)
{
    std::map<std::string, std::string>::const_iterator itKey = headers.find("sec-websocket-key");
    std::map<std::string, std::string>::const_iterator itVer = headers.find("sec-websocket-version");

    const std::string key = (itKey == headers.end()) ? std::string() : itKey->second;
    const std::string ver = (itVer == headers.end()) ? std::string() : itVer->second;

    if (key.empty() || ver != "13") {
        {
            WbGuard sl(statsMx);
            ++stats.wsRejected;
        }
        HttpResponse bad;
        bad.status = 400; bad.reason = "Bad Request";
        bad.contentType = "text/plain; charset=utf-8";
        bad.body = key.empty() ? "400 missing Sec-WebSocket-Key\n"
                               : "400 unsupported websocket version\n";
        bad.contentLength = static_cast<long long>(bad.body.size());
        bad.extraHeaders["Sec-WebSocket-Version"] = "13";
        Enqueue(c, bad.ToWire());
        c.closeAfterFlush = true;
        return true;
    }

    std::ostringstream os;
    os << "HTTP/1.1 101 Switching Protocols\r\n"
       << "Upgrade: websocket\r\n"
       << "Connection: Upgrade\r\n"
       << "Sec-WebSocket-Accept: " << sib::AcceptKey(key) << "\r\n"
       << "\r\n";
    Enqueue(c, os.str());

    c.isWs = true;
    c.lastRecvMs = NowMs();
    c.lastPingMs = c.lastRecvMs;
    {
        WbGuard sl(statsMx);
        ++stats.wsAccepted;
    }

    // ARCHITECTURE.md section 4: full state on connect, deltas thereafter.
    std::map<std::string, TagValue> cur;
    sib::SnapRead(snapshot, cur);
    SendJson(c, "{\"type\":\"snapshot\",\"data\":" + sib::ObjectFrom(cur) + "}");
    c.lastSent     = cur;
    c.sentSnapshot = true;
    {
        WbGuard sl(statsMx);
        ++stats.snapshotsSent;
    }

    // Any bytes the client pipelined behind the handshake are WS frames now.
    return c.in.empty() ? true : ProcessWsBytes(c);
}

// -----------------------------------------------------------------------------
//  WebSocket frames
// -----------------------------------------------------------------------------
bool WebBridgeServer::Impl::ProcessWsBytes(Conn& c)
{
    for (;;) {
        sib::Frame f;
        const int rc = sib::TryDecode(c.dec, c.in, f);
        if (rc == 0) {
            // Guard against a client that dribbles a giant frame header. The
            // decoder holds the partial bytes now, so ask it -- c.in has already
            // been drained into it and is always empty here.
            return sib::PendingBytes(c.dec) <= kMaxWsMessage + 1024u;
        }
        if (rc < 0) return false;                      // protocol error -> drop

        // RFC 6455 5.1: a client-to-server frame MUST be masked.
        if (!f.masked) return false;

        switch (f.opcode) {
            case sib::kOpPing:
                Enqueue(c, sib::EncodeServerFrame(sib::kOpPong, f.payload));
                break;

            case sib::kOpPong:
                c.awaitingPong = false;
                {
                    WbGuard sl(statsMx);
                    ++stats.pongsReceived;
                }
                break;

            case sib::kOpClose:
                Enqueue(c, sib::EncodeServerFrame(sib::kOpClose, std::string()));
                c.closeAfterFlush = true;
                return true;

            case sib::kOpBinary:
                return false;      // this protocol is JSON text only

            case sib::kOpText:
                if (f.fin) {
                    if (f.payload.size() > kMaxWsMessage) return false;
                    HandleTextMessage(c, f.payload);
                } else {
                    c.fragmenting = true;
                    c.fragment = f.payload;
                    if (c.fragment.size() > kMaxWsMessage) return false;
                }
                break;

            case sib::kOpCont:
                if (!c.fragmenting) return false;
                c.fragment += f.payload;
                if (c.fragment.size() > kMaxWsMessage) return false;
                if (f.fin) {
                    const std::string msg = c.fragment;
                    c.fragment.clear();
                    c.fragmenting = false;
                    HandleTextMessage(c, msg);
                }
                break;

            default:
                return false;      // reserved opcode
        }

        if (c.s == INVALID_SOCKET) return false;
    }
}

void WebBridgeServer::Impl::SendJson(Conn& c, const std::string& json)
{
    Enqueue(c, sib::EncodeServerFrame(sib::kOpText, json));
}

std::string WebBridgeServer::Impl::AckJson(double id, bool ok, const std::string& error)
{
    std::ostringstream os;
    os << "{\"type\":\"ack\",\"id\":";
    // Ids are integers on the wire (js/transport/ws.js counts 1,2,3...).
    const long long i = static_cast<long long>(id);
    if (static_cast<double>(i) == id) os << i; else os << id;
    os << ",\"ok\":" << (ok ? "true" : "false");
    if (!ok) os << ",\"error\":" << sib::QuoteString(error);
    os << "}";
    return os.str();
}

void WebBridgeServer::Impl::SendAck(Conn& c, double id, bool ok, const std::string& error)
{
    SendJson(c, AckJson(id, ok, error));
    WbGuard sl(statsMx);
    ++stats.acksSent;
}

// -----------------------------------------------------------------------------
//  One inbound JSON text message. Parsed with the vendored cJSON.
//
//  NOTHING here calls machine logic. A validated command is pushed onto the
//  CommandQueue and this function returns; the UI thread drains it later and
//  calls CompleteCommand(), which is what finally produces the ack.
// -----------------------------------------------------------------------------
void WebBridgeServer::Impl::HandleTextMessage(Conn& c, const std::string& text)
{
    cJSON* root = cJSON_Parse(text.c_str());
    if (!root) {
        {
            WbGuard sl(statsMx);
            ++stats.cmdRejected;
        }
        SendAck(c, 0, false, "malformed json");
        return;
    }

    const cJSON* jType = cJSON_GetObjectItemCaseSensitive(root, "type");
    const std::string type = (jType && cJSON_IsString(jType) && jType->valuestring)
                             ? std::string(jType->valuestring) : std::string();

    const cJSON* jId = cJSON_GetObjectItemCaseSensitive(root, "id");
    const bool haveId = (jId && cJSON_IsNumber(jId));
    const double id = haveId ? jId->valuedouble : 0.0;

    if (type == "ping") {
        // Liveness. Answered from this thread: it touches nothing but the socket.
        SendAck(c, id, true, std::string());
        cJSON_Delete(root);
        return;
    }

    if (type != "cmd") {
        // Forward compatibility: unknown frame types are ignored, not fatal.
        cJSON_Delete(root);
        return;
    }

    const cJSON* jCmd = cJSON_GetObjectItemCaseSensitive(root, "cmd");
    const std::string cmdName = (jCmd && cJSON_IsString(jCmd) && jCmd->valuestring)
                                ? std::string(jCmd->valuestring) : std::string();

    const cJSON* jTag = cJSON_GetObjectItemCaseSensitive(root, "tag");
    const std::string tagName = (jTag && cJSON_IsString(jTag) && jTag->valuestring)
                                ? std::string(jTag->valuestring) : std::string();

    std::string reject;
    if (!haveId)                            reject = "missing numeric id";
    else if (cmdName.empty())               reject = "missing cmd";
    else if (!IsSaneName(cmdName, 64))      reject = "illegal cmd name";
    else if (!tagName.empty() && !IsSaneName(tagName, 128)) reject = "illegal tag name";

    TagValue value = sib::MakeNull();
    if (reject.empty()) {
        const cJSON* jVal = cJSON_GetObjectItemCaseSensitive(root, "value");
        if (!jVal || cJSON_IsNull(jVal))    value = sib::MakeNull();
        else if (cJSON_IsBool(jVal))        value = sib::MakeBool(cJSON_IsTrue(jVal) != 0);
        else if (cJSON_IsNumber(jVal))      value = sib::MakeNumber(jVal->valuedouble);
        else if (cJSON_IsString(jVal) && jVal->valuestring)
                                            value = sib::MakeString(std::string(jVal->valuestring));
        else                                reject = "unsupported value type";
    }

    // The read-only gate. Default configuration lands here for every command.
    if (reject.empty() && readOnly.load()) reject = "bridge is read-only";
    if (reject.empty() && !queue)          reject = "no command queue attached";

    if (!reject.empty()) {
        {
            WbGuard sl(statsMx);
            ++stats.cmdRejected;
        }
        SendAck(c, id, false, reject);
        cJSON_Delete(root);
        return;
    }

    const unsigned long long ticket = nextTicket_++;
    if (!sib::QueuePush(queue, ticket, cmdName, tagName, value)) {
        {
            WbGuard sl(statsMx);
            ++stats.cmdRejected;
        }
        SendAck(c, id, false, "command queue full");
        cJSON_Delete(root);
        return;
    }

    {
        WbGuard pl(pendMx_);
        PendingAck pa;
        pa.connId    = c.id;
        pa.browserId = id;
        pending_[ticket] = pa;
        pendingOrder_.push_back(ticket);
        while (pendingOrder_.size() > kMaxPendingAcks) {
            pending_.erase(pendingOrder_.front());
            pendingOrder_.pop_front();
        }
    }
    {
        WbGuard sl(statsMx);
        ++stats.cmdAccepted;
    }
    // No ack yet -- ARCHITECTURE.md section 5: the ack is sent when the UI
    // thread has actually processed the command (CompleteCommand).
    cJSON_Delete(root);
}

// -----------------------------------------------------------------------------
//  Snapshot -> per-connection patch frames.
// -----------------------------------------------------------------------------
void WebBridgeServer::Impl::PumpSnapshot(bool force)
{
    const unsigned long long gen = sib::SnapGeneration(snapshot);
    if (!force && gen == lastGen_) return;
    lastGen_ = gen;

    if (conns_.empty()) return;

    std::map<std::string, TagValue> cur;
    sib::SnapRead(snapshot, cur);

    for (size_t i = 0; i < conns_.size(); ++i) {
        Conn& c = conns_[i];
        if (!c.isWs || !c.sentSnapshot || c.closeAfterFlush) continue;

        // Deltas for THIS connection only: a client that joined mid-run has a
        // different baseline from one that has been watching for an hour.
        std::map<std::string, TagValue> delta;
        for (std::map<std::string, TagValue>::const_iterator it = cur.begin();
             it != cur.end(); ++it) {
            std::map<std::string, TagValue>::const_iterator prev = c.lastSent.find(it->first);
            if (prev == c.lastSent.end() || !sib::ValuesEqual(prev->second, it->second)) {
                delta[it->first] = it->second;
            }
        }
        // A tag that disappeared becomes null == "unknown / not installed"
        // (ARCHITECTURE.md section 4 rule 3).
        for (std::map<std::string, TagValue>::const_iterator it = c.lastSent.begin();
             it != c.lastSent.end(); ++it) {
            if (cur.find(it->first) == cur.end()) delta[it->first] = sib::MakeNull();
        }

        if (delta.empty()) continue;

        SendJson(c, "{\"type\":\"patch\",\"data\":" + sib::ObjectFrom(delta) + "}");
        c.lastSent = cur;
        WbGuard sl(statsMx);
        ++stats.patchesSent;
    }
}

// -----------------------------------------------------------------------------
//  Frames handed over by the UI thread (acks and alarms).
// -----------------------------------------------------------------------------
void WebBridgeServer::Impl::PumpOutgoing()
{
    std::deque<Outgoing> batch;
    {
        WbGuard ol(outMx_);
        if (outQ_.empty()) return;
        batch.swap(outQ_);
    }

    for (std::deque<Outgoing>::const_iterator it = batch.begin(); it != batch.end(); ++it) {
        for (size_t i = 0; i < conns_.size(); ++i) {
            Conn& c = conns_[i];
            if (!c.isWs || c.closeAfterFlush) continue;
            if (it->connId != 0 && c.id != it->connId) continue;
            SendJson(c, it->frame);
        }
    }
}

// -----------------------------------------------------------------------------
void WebBridgeServer::Impl::PumpLiveness()
{
    const unsigned long long now = NowMs();
    for (size_t i = conns_.size(); i-- > 0;) {
        Conn& c = conns_[i];
        if (!c.isWs) continue;

        if (cfg.idleTimeoutMs > 0 &&
            now - c.lastRecvMs > static_cast<unsigned long long>(cfg.idleTimeoutMs)) {
            CloseConn(i);          // dead peer, or one that stopped ponging
            continue;
        }
        if (cfg.pingIntervalMs > 0 &&
            now - c.lastPingMs >= static_cast<unsigned long long>(cfg.pingIntervalMs)) {
            Enqueue(c, sib::EncodeServerFrame(sib::kOpPing, std::string()));
            c.lastPingMs   = now;
            c.awaitingPong = true;
            WbGuard sl(statsMx);
            ++stats.pingsSent;
        }
    }
}

// -----------------------------------------------------------------------------
//  UI-thread entry points. Both only touch a mutex-guarded queue, then wake the
//  socket thread. Neither blocks.
// -----------------------------------------------------------------------------
void WebBridgeServer::Impl::CompleteCommand(unsigned long long ticket, bool ok,
                                           const std::string& error)
{
    unsigned long long connId = 0;
    double browserId = 0.0;
    {
        WbGuard pl(pendMx_);
        std::map<unsigned long long, PendingAck>::iterator it = pending_.find(ticket);
        if (it == pending_.end()) return;      // connection already gone
        connId    = it->second.connId;
        browserId = it->second.browserId;
        pending_.erase(it);
    }
    {
        WbGuard ol(outMx_);
        Outgoing o;
        o.connId = connId;
        o.frame  = AckJson(browserId, ok, error);
        outQ_.push_back(o);
    }
    Wake();
}

void WebBridgeServer::Impl::PostAlarm(const std::string& code, const std::string& text,
                                     const std::string& at)
{
    std::ostringstream os;
    os << "{\"type\":\"alarm\",\"code\":" << sib::QuoteString(code)
       << ",\"text\":" << sib::QuoteString(text)
       << ",\"at\":" << sib::QuoteString(at.empty() ? IsoLocalNow() : at)
       << "}";
    {
        WbGuard ol(outMx_);
        Outgoing o;
        o.connId = 0;              // broadcast
        o.frame  = os.str();
        outQ_.push_back(o);
    }
    {
        WbGuard sl(statsMx);
        ++stats.alarmsSent;
    }
    Wake();
}

// =============================================================================
//  WebBridgeServer -- thin forwarding shell
// =============================================================================
WebBridgeServer::WebBridgeServer(const WebBridgeConfig& cfg)
    : impl_(new Impl(cfg))
{
}

WebBridgeServer::~WebBridgeServer()
{
    impl_->Stop();
}

void WebBridgeServer::SetSnapshot(TagSnapshot* snapshot)   { impl_->snapshot = snapshot; }
void WebBridgeServer::SetCommandQueue(CommandQueue* queue) { impl_->queue = queue; }
void WebBridgeServer::SetReadOnly(bool ro)                 { impl_->readOnly.store(ro); }
bool WebBridgeServer::IsReadOnly() const                   { return impl_->readOnly.load(); }
bool WebBridgeServer::Start(std::string* errOut)           { return impl_->Start(errOut); }
void WebBridgeServer::Stop()                               { impl_->Stop(); }
bool WebBridgeServer::IsRunning() const                    { return impl_->running.load(); }
unsigned short WebBridgeServer::BoundPort() const          { return impl_->boundPort.load(); }
void WebBridgeServer::Wake()                               { impl_->Wake(); }

void WebBridgeServer::CompleteCommand(unsigned long long ticket, bool ok,
                                      const std::string& error)
{
    impl_->CompleteCommand(ticket, ok, error);
}

void WebBridgeServer::PostAlarm(const std::string& code, const std::string& text,
                                const std::string& at)
{
    impl_->PostAlarm(code, text, at);
}

WebBridgeStats WebBridgeServer::Stats() const
{
    WbGuard lk(impl_->statsMx);
    return impl_->stats;
}

}  // namespace webbridge
