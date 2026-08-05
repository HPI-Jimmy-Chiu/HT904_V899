// =============================================================================
//  WebBridge/WebBridgeServer.h -- the HTTP + WebSocket server that lets a
//  browser display real handler state.
//
//  Implements option A of D:\HT9045\web\docs\ARCHITECTURE.md section 3:
//  an embedded server inside the handler process, serving D:\HT9045\web as
//  static files and one WebSocket endpoint (default "/ht9045") speaking the
//  section-4 wire protocol.
//
//  THREADING CONTRACT -- ARCHITECTURE.md section 5. Read this before editing.
//  --------------------------------------------------------------------------
//  The handler's UI thread has hard obligations elsewhere (the SECS/GEM layer
//  has a 30-second reply budget; blocking it is a known failure mode in this
//  product). Therefore:
//
//    * This object owns ONE thread. It runs select() over the listener plus
//      every client socket. Not a thread per connection.
//    * That thread NEVER calls machine logic. Its entire view of the machine
//      is the TagSnapshot it reads and the CommandQueue it pushes into.
//    * State flows UI thread -> browser: the UI thread calls
//      TagSnapshot::Publish() on its existing timer tick; this server notices
//      the generation change and emits per-connection "patch" frames.
//    * Commands flow browser -> UI thread: the socket thread validates and
//      enqueues, then returns immediately. It does not wait.
//    * The ack for an accepted command is sent when the UI thread has actually
//      processed it and called CompleteCommand() -- section 5 again. Only
//      REJECTED commands (bad JSON, unknown shape, read-only, queue full) are
//      acked straight from the socket thread, because those never reach the UI
//      thread at all.
//
//  Which methods may be called from where:
//
//    UI thread (or any thread):  Start, Stop, SetReadOnly, CompleteCommand,
//                                PostAlarm, Wake, Stats, BoundPort, IsRunning
//    Socket thread (internal):   everything else
//
//  SAFETY POSTURE
//  --------------
//  bindAddress defaults to 127.0.0.1 (loopback ONLY) and readOnly defaults to
//  TRUE, because this endpoint can eventually command machine motion --
//  ARCHITECTURE.md section 6 questions 2 and 3. Widening either is an explicit,
//  deliberate act by the caller, never a default.
//
//  DEPENDENCIES ON SIBLING WebBridge COMPONENTS (written in parallel)
//  -----------------------------------------------------------------
//  Every call into a sibling component is confined to one clearly marked
//  "SIBLING ADAPTER" block at the top of WebBridgeServer.cpp. If a sibling's
//  real signature differs from the shape assumed there, that block is the only
//  place to fix -- nothing else in this file or the .cpp touches those APIs.
//  The assumed shapes are listed in that block.
//
//  This layer is independent of VCL: no <vcl.h>, no vclcompat, no AnsiString,
//  no machine headers.
// =============================================================================
#ifndef WEBBRIDGE_WEBBRIDGESERVER_H
#define WEBBRIDGE_WEBBRIDGESERVER_H

#include <memory>
#include <string>

#include "WebBridge/CommandQueue.h"
#include "WebBridge/HttpStatic.h"
#include "WebBridge/TagSnapshot.h"
#include "WebBridge/TagValue.h"

namespace webbridge {

// -----------------------------------------------------------------------------
struct WebBridgeConfig {
    // Loopback by default. "0.0.0.0" exposes this on the fab LAN, which
    // ARCHITECTURE.md section 6 question 2 says must never be the default.
    std::string    bindAddress;

    // 0 asks the OS for an ephemeral port; read it back with BoundPort().
    unsigned short port;

    // Directory served as "/" -- normally D:\HT9045\web. Empty = 404 for all
    // static requests (the WebSocket endpoint still works).
    std::string    documentRoot;

    // Simultaneous client sockets. Clamped internally to what select() can
    // carry (FD_SETSIZE) with room for the listener and the wakeup socket.
    int            maxConnections;

    // TRUE = every inbound "cmd" frame is refused with ack ok:false. This is
    // the read-only dashboard milestone of ARCHITECTURE.md section 6 q3, and
    // it is the DEFAULT.
    bool           readOnly;

    // WebSocket endpoint path. js/transport/ws.js connects to "/ht9045".
    std::string    wsPath;

    int            pingIntervalMs;   // WS ping cadence           (default 15000)
    int            idleTimeoutMs;    // silence before drop        (default 45000)
    int            pollIntervalMs;   // select() timeout           (default 50)

    // A connection whose unsent backlog exceeds this is dropped rather than
    // allowed to stall the single socket thread for every other client.
    size_t         maxSendBacklog;   // default 256 KiB

    WebBridgeConfig();
};

// -----------------------------------------------------------------------------
//  Observable counters. Copy-returned under a lock; safe from any thread.
// -----------------------------------------------------------------------------
struct WebBridgeStats {
    unsigned long long httpRequests;
    unsigned long long wsAccepted;
    unsigned long long wsRejected;       // upgrade attempts we refused
    unsigned long long snapshotsSent;
    unsigned long long patchesSent;
    unsigned long long alarmsSent;
    unsigned long long acksSent;
    unsigned long long cmdAccepted;      // validated and enqueued
    unsigned long long cmdRejected;      // refused, ack ok:false sent
    unsigned long long pingsSent;
    unsigned long long pongsReceived;
    unsigned long long slowClientDrops;
    unsigned long long connectionsAccepted;
    unsigned long long connectionsClosed;
    int                liveConnections;

    WebBridgeStats();
};

// -----------------------------------------------------------------------------
class WebBridgeServer {
public:
    explicit WebBridgeServer(const WebBridgeConfig& cfg);
    ~WebBridgeServer();   // calls Stop()

    // Wire up the two -- and only two -- things that cross the machine
    // boundary. Both may be null (then the bridge serves an empty snapshot and
    // refuses all commands). Call before Start().
    void SetSnapshot(TagSnapshot* snapshot);
    void SetCommandQueue(CommandQueue* queue);

    // Live-switchable; takes effect on the next inbound frame.
    void SetReadOnly(bool readOnly);
    bool IsReadOnly() const;

    // Creates and binds the listener on the CALLING thread (so a bind failure
    // is reported synchronously and BoundPort() is valid on return), then
    // spawns the socket thread and returns immediately.
    // Returns false and fills errOut on failure. Calling Start() twice is a
    // no-op returning true.
    bool Start(std::string* errOut = 0);

    // Wakes the socket thread, joins it, and releases every socket and handle.
    // Safe to call twice, safe when Start() was never called, safe from the
    // destructor. Prompt: bounded by one wakeup round trip, not by
    // pollIntervalMs and not by any client's behaviour.
    void Stop();

    bool           IsRunning() const;
    unsigned short BoundPort() const;   // 0 until Start() succeeds

    // --- called by the UI thread -------------------------------------------
    // Report the outcome of a command previously drained from the CommandQueue.
    // `ticket` is the id this server put on the queued command. The browser
    // receives {"type":"ack","id":<its own id>,"ok":...,"error":"..."} on the
    // connection that sent it; if that connection has since gone, the result is
    // dropped silently. Non-blocking.
    void CompleteCommand(unsigned long long ticket, bool ok, const std::string& error);

    // Broadcast {"type":"alarm","code":...,"text":...,"at":...} to every
    // connected browser. Non-blocking. `at` should be ISO-8601; when empty the
    // server fills in the current local time.
    void PostAlarm(const std::string& code, const std::string& text,
                   const std::string& at = std::string());

    // Nudge the socket thread to re-check the snapshot now instead of at the
    // next poll tick. Cheap; safe to call from the UI timer after Publish().
    void Wake();

    WebBridgeStats Stats() const;

private:
    WebBridgeServer(const WebBridgeServer&);
    WebBridgeServer& operator=(const WebBridgeServer&);

    class Impl;                      // all winsock lives in the .cpp
    std::unique_ptr<Impl> impl_;
};

}  // namespace webbridge

#endif  // WEBBRIDGE_WEBBRIDGESERVER_H
