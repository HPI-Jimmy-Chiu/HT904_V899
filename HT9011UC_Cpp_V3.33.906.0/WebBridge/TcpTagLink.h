// ===========================================================================
//  WebBridge/TcpTagLink.h
//  The sidecar seam: a loopback, read-only TCP link that carries a TagSnapshot
//  from the process that HAS the machine data to a process that serves the web
//  HMI.
//
//  AI(W906-WebBridge-Tcp) 20260812: new file. NEW-BUILD infrastructure, not a
//  translation of any BCB6 golden source.
//
//  ===========================================================================
//  WHY THIS EXISTS AT ALL (the alternative was rejected on evidence)
//  ===========================================================================
//  web/docs/ARCHITECTURE.md section 3 lists three shapes. This file implements
//  the transport for option B (sidecar), chosen for one specific deployment:
//  the web server must be able to show data from a handler WITHOUT the web
//  server linking any machine code, and without the handler hosting HTTP.
//
//  SECS was evaluated first and REJECTED, at code level, not on taste:
//      HT9011UC_Code_V3.33.899.0_.../SECSGEM/uHGemEquipment.cpp:2005-2013
//  the passive send path is
//      if (ActiveConnections > 0)
//        if (Connections[0]->Connected)
//          if (ActiveConnections == 1)          <-- note this
//            Connections[0]->SendBuf(...)
//  so a SECOND HSMS client (which a web bridge would be) makes
//  ActiveConnections 2, the innermost test fails, and the handler silently
//  stops sending to the REAL host. No else branch, no log. Using SECS as the
//  UI bus therefore breaks the customer's MES link in production. Line 4037
//  compounds it by always writing to Connections[0].
//
//  ===========================================================================
//  THREADING CONTRACT -- THE PART THAT CAN STOP A MACHINE
//  ===========================================================================
//  ARCHITECTURE.md section 5: "Nothing on the socket thread may block on
//  machine I/O. Ever." The handler's SECS/GEM layer runs on the UI thread with
//  a 30-second host reply budget; a socket that stalls the UI thread stalls a
//  production machine.
//
//  So:
//    * TcpTagPublisher owns its OWN thread. The machine/UI thread never enters
//      this class -- it only calls TagSnapshot::publish(), exactly as it
//      already does for the WebSocket server.
//    * The publisher thread only ever calls TagSnapshot::diffFrom()/read(),
//      both of which are documented as callable from any thread and hold the
//      snapshot lock for a copy and nothing more.
//    * Every socket is non-blocking and every send is bounded. A consumer that
//      stops reading is DROPPED (maxSendBacklog), never allowed to back up into
//      the publisher loop.
//    * Stop() joins the thread. Destruction without Stop() calls it for you.
//
//  ===========================================================================
//  SAFETY POSTURE -- INHERITED FROM THE WEBSOCKET SERVER, NOT RE-DECIDED
//  ===========================================================================
//    * bindAddress defaults to 127.0.0.1. ARCHITECTURE.md section 6 question 2
//      says LAN exposure must never be a default, and this file does not widen
//      it. Binding elsewhere is possible but must be an explicit act by the
//      caller.
//    * READ-ONLY. There is no inbound command path in this version at all
//      (section 6 question 3 recommends shipping read-only first). Bytes a
//      consumer sends are counted and discarded, and a consumer that sends more
//      than maxInboundBurst in one go is dropped.
//
//      That last rule is not paranoia. The comparable channel in the shipping
//      handler, Command.cpp:12767-12778 (was cited :12697-12708; golden line
//      numbers drifted -- re-verified 20260817, grep "ReceiveBuf" hits :12778
//      only, inside TCPCommandServerClientRead), does:
//          int iBufferLenght = Socket->ReceiveLength();   // unbounded
//          char EthernetBuffer[100];
//          Socket->ReceiveBuf(EthernetBuffer, iBufferLenght);
//      which overflows a 100-byte stack buffer the moment a client sends 101
//      bytes. This link refuses to repeat that.
//
//  ===========================================================================
//  WIRE FORMAT -- one JSON object per line, '\n' terminated, UTF-8
//  ===========================================================================
//    {"type":"hello","proto":1,"gen":<n>}
//    {"type":"snapshot","gen":<n>,"data":{tag:value,...}}
//    {"type":"patch","gen":<n>,"data":{tag:value,...},"removed":["tag",...]}
//    {"type":"ping","t":<ms since publisher start>}
//
//  Line-delimited rather than length-prefixed on purpose: it is greppable in a
//  log, a human can `telnet 127.0.0.1 8046` and read it, and the framing bug
//  class (desync on a bad length) does not exist. Tag names and string values
//  are JSON-escaped, so a literal '\n' can never appear inside a line.
//
//  `data` is the SAME object shape the WebSocket layer already sends the
//  browser, so a gateway can forward it with no re-modelling, and a wire log
//  from either hop diffs against the other.
//
//  `removed` is carried explicitly instead of being encoded as null. On the
//  browser wire those collapse (both render "---"), but between two C++ peers
//  "went absent" and "is present and null" are different facts, and the
//  consumer's TagSnapshot can represent both.
// ===========================================================================
#ifndef WEBBRIDGE_TCPTAGLINK_H
#define WEBBRIDGE_TCPTAGLINK_H

#include "WebBridge/TagSnapshot.h"

#include <cstddef>
#include <cstdint>
#include <string>

namespace webbridge {

// ---------------------------------------------------------------------------
//  Publisher (runs in the process that owns the machine data)
// ---------------------------------------------------------------------------
struct TcpPublisherConfig {
    std::string    bindAddress;      // default "127.0.0.1" -- see SAFETY above
    unsigned short port;             // default 8046; 0 = ephemeral, read back
    int            maxConnections;   // default 4
    int            pollIntervalMs;   // default 50   (select() timeout)
    int            heartbeatMs;      // default 5000 (ping when otherwise idle)
    std::size_t    maxSendBacklog;   // default 256 KiB -> drop a stalled peer
    std::size_t    maxInboundBurst;  // default 4 KiB   -> drop a chatty peer

    TcpPublisherConfig()
        : bindAddress("127.0.0.1")
        , port(8046)
        , maxConnections(4)
        , pollIntervalMs(50)
        , heartbeatMs(5000)
        , maxSendBacklog(256u * 1024u)
        , maxInboundBurst(4u * 1024u)
    {}
};

class TcpTagPublisher {
public:
    struct Stats {
        std::uint64_t framesSent;
        std::uint64_t bytesSent;
        std::uint64_t clientsAccepted;
        std::uint64_t clientsDropped;
        std::uint64_t inboundBytesDiscarded;
        int           activeClients;

        Stats()
            : framesSent(0), bytesSent(0), clientsAccepted(0)
            , clientsDropped(0), inboundBytesDiscarded(0), activeClients(0) {}
    };

    // `source` must outlive this object. Nothing is copied out of it until
    // Start(); construction touches no sockets.
    TcpTagPublisher(const TagSnapshot& source, const TcpPublisherConfig& cfg);
    ~TcpTagPublisher();

    // Bind, listen, and spawn the publisher thread. False + `error` on failure;
    // on failure nothing is left running and the object may be destroyed.
    bool Start(std::string& error);

    // Idempotent. Signals the thread, closes the listener, joins.
    void Stop();

    bool Running() const;

    // Valid after a successful Start(); the resolved port when cfg.port was 0.
    unsigned short BoundPort() const;

    Stats GetStats() const;

    TcpTagPublisher(const TcpTagPublisher&) = delete;
    TcpTagPublisher& operator=(const TcpTagPublisher&) = delete;

private:
    struct Impl;
    Impl* impl_;
};

// ---------------------------------------------------------------------------
//  Consumer (runs in the web server process; links NO machine code)
// ---------------------------------------------------------------------------
struct TcpClientConfig {
    std::string    host;             // default "127.0.0.1"
    unsigned short port;             // default 8046
    int            reconnectMs;      // default 1000 between attempts
    int            staleTimeoutMs;   // default 20000 silence -> reconnect
    std::size_t    maxLineBytes;     // default 1 MiB -> drop the connection

    TcpClientConfig()
        : host("127.0.0.1")
        , port(8046)
        , reconnectMs(1000)
        , staleTimeoutMs(20000)
        , maxLineBytes(1024u * 1024u)
    {}
};

class TcpTagClient {
public:
    struct Stats {
        std::uint64_t framesReceived;
        std::uint64_t bytesReceived;
        std::uint64_t reconnects;
        std::uint64_t parseErrors;
        std::uint64_t lastGeneration;   // generation last applied to the sink
        bool          connected;

        Stats()
            : framesReceived(0), bytesReceived(0), reconnects(0)
            , parseErrors(0), lastGeneration(0), connected(false) {}
    };

    // `sink` must outlive this object. THIS CLASS BECOMES THE SINK'S PUBLISHER:
    // its worker thread is the one thread allowed on the sink's publish path
    // (TagSnapshot.h, "UI THREAD ONLY"). Do not also publish to `sink` from
    // anywhere else -- TagSnapshot::publisherViolations() will count it, but by
    // then the staging buffer has already been raced on.
    TcpTagClient(TagSnapshot& sink, const TcpClientConfig& cfg);
    ~TcpTagClient();

    // Spawns the worker. Returns true once the thread is running -- NOT once a
    // connection exists. The link is designed to come up before the publisher
    // does and to survive the publisher restarting, so "cannot connect yet" is
    // a normal state, not a start failure. Poll Connected() if you care.
    bool Start(std::string& error);
    void Stop();
    bool Running() const;
    bool Connected() const;

    Stats GetStats() const;

    TcpTagClient(const TcpTagClient&) = delete;
    TcpTagClient& operator=(const TcpTagClient&) = delete;

private:
    struct Impl;
    Impl* impl_;
};

} // namespace webbridge

#endif // WEBBRIDGE_TCPTAGLINK_H
