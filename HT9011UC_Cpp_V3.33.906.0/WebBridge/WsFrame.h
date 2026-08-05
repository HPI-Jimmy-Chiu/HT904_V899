// ===========================================================================
//  WebBridge/WsFrame.h
//
//  RFC 6455 section 5 framing: encode (server -> client) and an INCREMENTAL
//  decode (client -> server).
//
//  SCOPE / LAYERING
//  ----------------
//  Pure bytes in -> bytes out, exactly like WsHandshake: no sockets, no
//  machine state, no <winsock2.h>, no <vcl.h>. Multi-byte lengths are composed
//  and decomposed with explicit shifts rather than htons/htonl so that this
//  file needs no platform networking header at all and its byte order is
//  visible in the source.
//
//  WHY THE DECODER IS INCREMENTAL (and why that is not optional)
//  ------------------------------------------------------------
//  TCP has no message boundaries. recv() will hand you half a frame, two and a
//  half frames, or one byte -- whatever the network happened to deliver. A
//  decoder that assumes "one recv == one frame" works on localhost during
//  development and then corrupts state the first time a real network splits a
//  64 KiB payload. WsDecoder::Feed() therefore accepts arbitrary chunk
//  boundaries and is required (and tested) to produce byte-identical messages
//  when fed one byte at a time.
//
//  WHAT IS COVERED (each item is called out again at its implementation site)
//  -------------------------------------------------------------------------
//   * opcodes: continuation 0x0, text 0x1, binary 0x2, close 0x8, ping 0x9,
//     pong 0xA. Every other opcode is reserved and is a protocol error.
//   * all three payload length forms: 7-bit, 16-bit (126) and 64-bit (127),
//     both in network byte order (big-endian).
//   * masking DIRECTION is enforced, not merely supported: client -> server
//     frames MUST be masked (RFC 6455 section 5.3) and an unmasked one is
//     rejected; server -> client frames MUST NOT be masked, and the encoder
//     defaults to unmasked. The masked encoder exists for tests and for any
//     future client role -- it is not what the server sends.
//   * fragmentation: continuation frames are reassembled, and control frames
//     may be interleaved between the fragments of a data message (they are
//     delivered immediately and do not disturb the reassembly buffer).
//     Control frames are themselves never fragmented -- FIN=0 on a control
//     frame is a protocol error.
//   * a hard maximum message size, enforced BEFORE the payload is buffered, so
//     a hostile or buggy client cannot make the machine PC allocate 4 GiB by
//     announcing a 64-bit length. Exceeding it closes with status 1009.
//   * close frames: 2-byte big-endian status code plus an optional UTF-8
//     reason; a 1-byte payload and illegal status codes are protocol errors.
//
//  C++ DIALECT: C++14-compatible (see the note in WsHandshake.h -- the
//  configured MinGW g++ 6.3.0 accepts -std=c++17 but does not implement it).
// ===========================================================================
#ifndef WEBBRIDGE_WSFRAME_H
#define WEBBRIDGE_WSFRAME_H

#include <cstddef>
#include <string>
#include <vector>

#if defined(_MSC_VER) && (_MSC_VER < 1600)
   typedef unsigned __int8  uint8_t;
   typedef unsigned __int16 uint16_t;
   typedef unsigned __int32 uint32_t;
   typedef unsigned __int64 uint64_t;
#else
#  include <cstdint>
#endif

namespace webbridge {

// --- RFC 6455 section 5.2 opcodes ------------------------------------------
enum WsOpcode {
    kWsContinuation = 0x0,
    kWsText         = 0x1,
    kWsBinary       = 0x2,
    kWsClose        = 0x8,
    kWsPing         = 0x9,
    kWsPong         = 0xA
};

// --- RFC 6455 section 7.4.1 close status codes -----------------------------
enum WsCloseCode {
    kWsCloseNormal          = 1000,
    kWsCloseGoingAway       = 1001,
    kWsCloseProtocolError   = 1002,
    kWsCloseUnsupportedData = 1003,
    kWsCloseNoStatusRcvd    = 1005,  // never sent on the wire
    kWsCloseAbnormal        = 1006,  // never sent on the wire
    kWsCloseInvalidPayload  = 1007,
    kWsClosePolicyViolation = 1008,
    kWsCloseMessageTooBig   = 1009,
    kWsCloseExtensionNeeded = 1010,
    kWsCloseInternalError   = 1011
};

// Default cap on one reassembled message. The tag snapshot for the home screen
// is ~234 tags (ARCHITECTURE.md section 4 rule 2), so an inbound command frame
// is tiny; 1 MiB is generous for anything an operator UI legitimately sends.
const std::size_t kDefaultMaxMessageBytes = 1u * 1024u * 1024u;

// Control frame payloads are capped by the RFC itself, not by policy.
const std::size_t kWsMaxControlPayload = 125;

// ---------------------------------------------------------------------------
//  Encoding (server -> client unless stated otherwise)
// ---------------------------------------------------------------------------

// The general form. `mask` MUST be false for anything the server sends
// (RFC 6455 section 5.1); it exists so tests and any future client role can
// generate conformant client frames.
std::string EncodeFrame(int opcode, const std::string& payload,
                        bool fin = true, bool mask = false, uint32_t maskKey = 0);

std::string EncodeText(const std::string& utf8Payload);
std::string EncodeBinary(const std::string& bytes);
std::string EncodePing(const std::string& payload = std::string());
std::string EncodePong(const std::string& payload = std::string());

// A close frame carrying `code` big-endian followed by `reason` (which must be
// UTF-8 and is truncated so the total payload stays within 125 bytes).
std::string EncodeClose(uint16_t code, const std::string& reason = std::string());

// Client-direction helper: same as EncodeFrame with mask=true.
std::string EncodeMaskedFrame(int opcode, const std::string& payload,
                              uint32_t maskKey, bool fin = true);

// ---------------------------------------------------------------------------
//  A decoded message
// ---------------------------------------------------------------------------
struct WsMessage {
    int opcode;            // kWsText / kWsBinary for data; kWsClose/Ping/Pong for control
    std::string payload;   // data messages: fully reassembled across fragments
                           // close frames: the raw payload (code + reason bytes)

    // Only meaningful when opcode == kWsClose.
    bool hasCloseCode;
    uint16_t closeCode;    // kWsCloseNoStatusRcvd (1005) when hasCloseCode is false
    std::string closeReason;

    WsMessage()
        : opcode(0), hasCloseCode(false), closeCode(kWsCloseNoStatusRcvd) {}

    bool IsControl() const { return (opcode & 0x08) != 0; }
};

// ---------------------------------------------------------------------------
//  WsDecoder -- incremental, one instance per connection
// ---------------------------------------------------------------------------
class WsDecoder {
public:
    // `requireMaskedInput` true is the server role: inbound frames must be
    // masked. Construct with false only to decode a server's frames (tests).
    explicit WsDecoder(bool requireMaskedInput = true,
                       std::size_t maxMessageBytes = kDefaultMaxMessageBytes);

    // Feed received bytes. Completed messages are APPENDED to *out (existing
    // contents are left alone), in arrival order.
    //
    // Returns true while the stream is still well-formed. On false the stream
    // has failed: CloseCode()/CloseReason() give the status the caller must
    // send via EncodeClose() before closing the socket, and every later Feed()
    // returns false without doing anything.
    bool Feed(const char* data, std::size_t len, std::vector<WsMessage>* out);
    bool Feed(const std::string& chunk, std::vector<WsMessage>* out);

    bool Failed() const { return failed_; }
    uint16_t CloseCode() const { return closeCode_; }
    const std::string& CloseReason() const { return closeReason_; }

    // True once a close frame has been received from the peer.
    bool SawClose() const { return sawClose_; }

    // Bytes held for a partially received frame, plus bytes held for a
    // partially reassembled fragmented message. For diagnostics/tests.
    std::size_t PendingBytes() const { return buf_.size(); }
    std::size_t FragmentBytes() const { return frag_.size(); }

    // Text payloads and close reasons are UTF-8 validated by default and a
    // violation closes with 1007 (RFC 6455 sections 5.6 and 8.1). Turn off
    // only if a non-conformant peer must be tolerated.
    void SetValidateUtf8(bool on) { validateUtf8_ = on; }

    void Reset();

    // Exposed because it is genuinely useful to callers and is tested here.
    static bool IsValidUtf8(const std::string& s);

private:
    // Try to consume one whole frame from the front of buf_.
    // Returns: 1 progress made, 0 need more bytes, -1 protocol failure.
    int TryOneFrame(std::vector<WsMessage>* out);
    bool Fail(uint16_t code, const char* reason);

    std::string buf_;        // received-but-unparsed bytes
    std::string frag_;       // reassembly buffer for the current data message
    int  fragOpcode_;        // kWsText/kWsBinary while fragmenting, else 0
    bool requireMask_;
    bool validateUtf8_;
    bool failed_;
    bool sawClose_;
    uint16_t closeCode_;
    std::string closeReason_;
    std::size_t maxMessage_;
};

}  // namespace webbridge

#endif  // WEBBRIDGE_WSFRAME_H
