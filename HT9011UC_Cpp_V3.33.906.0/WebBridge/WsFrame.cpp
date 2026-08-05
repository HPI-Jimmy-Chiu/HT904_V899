// ===========================================================================
//  WebBridge/WsFrame.cpp
//
//  RFC 6455 section 5 framing. See WsFrame.h for the contract; the per-rule
//  commentary lives at the point each rule is enforced, below.
//
//  FRAME LAYOUT being implemented (RFC 6455 section 5.2):
//
//     0                   1                   2                   3
//     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-------+-+-------------+-------------------------------+
//    |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
//    |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
//    |N|V|V|V|       |S|             |   (if payload len==126/127)   |
//    | |1|2|3|       |K|             |                               |
//    +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
//    |     Extended payload length continued, if payload len == 127  |
//    + - - - - - - - - - - - - - - - +-------------------------------+
//    |                               |Masking-key, if MASK set to 1  |
//    +-------------------------------+-------------------------------+
//    | Masking-key (continued)       |          Payload Data         |
//    +-------------------------------- - - - - - - - - - - - - - - - +
// ===========================================================================
#include "WsFrame.h"

#include <cstring>

namespace webbridge {

// ===========================================================================
//  Encoding
// ===========================================================================

// Append `n` as the RFC 6455 section 5.2 payload-length field(s), OR-ing in the
// mask bit. All three forms live here so the choice between them is in one
// place:
//    n <= 125          -> the 7-bit field carries the length directly
//    126 <= n <= 65535 -> field is 126, followed by 16 bits, network order
//    n >= 65536        -> field is 127, followed by 64 bits, network order
// "Network order" is big-endian, written out with explicit shifts so no
// platform header (and no host-endianness assumption) is involved.
static void AppendLength(std::string& f, std::size_t n, unsigned char maskBit) {
    if (n < 126) {
        f.push_back(static_cast<char>(maskBit | static_cast<unsigned char>(n)));
    } else if (n <= 0xFFFFu) {
        f.push_back(static_cast<char>(maskBit | 126));
        f.push_back(static_cast<char>((n >> 8) & 0xFF));
        f.push_back(static_cast<char>(n & 0xFF));
    } else {
        f.push_back(static_cast<char>(maskBit | 127));
        uint64_t v = static_cast<uint64_t>(n);
        for (int i = 7; i >= 0; --i) {
            f.push_back(static_cast<char>((v >> (i * 8)) & 0xFF));
        }
    }
}

std::string EncodeFrame(int opcode, const std::string& payload,
                        bool fin, bool mask, uint32_t maskKey) {
    std::string f;
    f.reserve(payload.size() + 14);

    // FIN + RSV1..3 (always zero: no extension is negotiated in the
    // handshake, so sending a non-zero RSV bit would be a protocol error) +
    // 4-bit opcode.
    unsigned char b0 = static_cast<unsigned char>((fin ? 0x80 : 0x00) |
                                                  (opcode & 0x0F));
    f.push_back(static_cast<char>(b0));

    AppendLength(f, payload.size(), mask ? 0x80 : 0x00);

    if (mask) {
        // Client -> server direction only. The key is written big-endian and
        // the payload is XORed with key[i % 4] (RFC 6455 section 5.3).
        unsigned char k[4];
        k[0] = static_cast<unsigned char>((maskKey >> 24) & 0xFF);
        k[1] = static_cast<unsigned char>((maskKey >> 16) & 0xFF);
        k[2] = static_cast<unsigned char>((maskKey >> 8) & 0xFF);
        k[3] = static_cast<unsigned char>(maskKey & 0xFF);
        f.append(reinterpret_cast<const char*>(k), 4);
        for (std::size_t i = 0; i < payload.size(); ++i) {
            f.push_back(static_cast<char>(
                static_cast<unsigned char>(payload[i]) ^ k[i & 3]));
        }
    } else {
        // Server -> client: NEVER masked (RFC 6455 section 5.1). A masked
        // frame from the server is a protocol error the browser will drop the
        // connection over, so the default of mask=false is the safe one.
        f.append(payload);
    }
    return f;
}

std::string EncodeMaskedFrame(int opcode, const std::string& payload,
                              uint32_t maskKey, bool fin) {
    return EncodeFrame(opcode, payload, fin, true, maskKey);
}

std::string EncodeText(const std::string& utf8Payload) {
    return EncodeFrame(kWsText, utf8Payload, true, false, 0);
}

std::string EncodeBinary(const std::string& bytes) {
    return EncodeFrame(kWsBinary, bytes, true, false, 0);
}

std::string EncodePing(const std::string& payload) {
    // Control frames carry at most 125 bytes and are never fragmented
    // (RFC 6455 section 5.5), so an over-long ping payload is clamped rather
    // than split.
    std::string p = payload;
    if (p.size() > kWsMaxControlPayload) p.resize(kWsMaxControlPayload);
    return EncodeFrame(kWsPing, p, true, false, 0);
}

std::string EncodePong(const std::string& payload) {
    std::string p = payload;
    if (p.size() > kWsMaxControlPayload) p.resize(kWsMaxControlPayload);
    return EncodeFrame(kWsPong, p, true, false, 0);
}

// Truncate to at most `limit` bytes without splitting a UTF-8 sequence -- a
// close reason that ends mid-code-point is invalid UTF-8 and the peer is
// entitled to treat it as a 1007 protocol error.
static std::string TruncateUtf8(const std::string& s, std::size_t limit) {
    if (s.size() <= limit) return s;
    std::size_t cut = limit;
    while (cut > 0 && (static_cast<unsigned char>(s[cut]) & 0xC0) == 0x80) --cut;
    return s.substr(0, cut);
}

std::string EncodeClose(uint16_t code, const std::string& reason) {
    // RFC 6455 section 5.5.1: the payload is a 2-byte big-endian status code
    // followed by an optional UTF-8 reason. An empty payload is also legal and
    // means "no status" -- code 0 is the way to ask for that here.
    std::string p;
    if (code != 0) {
        p.push_back(static_cast<char>((code >> 8) & 0xFF));
        p.push_back(static_cast<char>(code & 0xFF));
        // 125 total - 2 for the code = 123 bytes of reason.
        p += TruncateUtf8(reason, kWsMaxControlPayload - 2);
    }
    return EncodeFrame(kWsClose, p, true, false, 0);
}

// ===========================================================================
//  UTF-8 validation (RFC 3629): used for text messages and close reasons
// ===========================================================================
bool WsDecoder::IsValidUtf8(const std::string& s) {
    std::size_t i = 0, n = s.size();
    while (i < n) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        std::size_t extra;
        uint32_t cp;
        if (c < 0x80) { ++i; continue; }
        else if ((c & 0xE0) == 0xC0) { extra = 1; cp = c & 0x1F; }
        else if ((c & 0xF0) == 0xE0) { extra = 2; cp = c & 0x0F; }
        else if ((c & 0xF8) == 0xF0) { extra = 3; cp = c & 0x07; }
        else return false;                       // 0x80-0xBF stray, or 0xF8+
        // Continuation bytes live at i+1 .. i+extra, so they all exist only if
        // i + extra <= n - 1. A sequence cut short by the end of the string is
        // invalid (this is a whole-message check, not a streaming one).
        if (i + extra >= n) return false;
        for (std::size_t k = 1; k <= extra; ++k) {
            unsigned char cc = static_cast<unsigned char>(s[i + k]);
            if ((cc & 0xC0) != 0x80) return false;
            cp = (cp << 6) | (cc & 0x3F);
        }
        // Reject overlong encodings, UTF-16 surrogates, and > U+10FFFF.
        if (extra == 1 && cp < 0x80) return false;
        if (extra == 2 && cp < 0x800) return false;
        if (extra == 3 && cp < 0x10000) return false;
        if (cp >= 0xD800 && cp <= 0xDFFF) return false;
        if (cp > 0x10FFFF) return false;
        i += extra + 1;
    }
    return true;
}

// ===========================================================================
//  WsDecoder
// ===========================================================================
WsDecoder::WsDecoder(bool requireMaskedInput, std::size_t maxMessageBytes)
    : fragOpcode_(0),
      requireMask_(requireMaskedInput),
      validateUtf8_(true),
      failed_(false),
      sawClose_(false),
      closeCode_(kWsCloseNormal),
      maxMessage_(maxMessageBytes ? maxMessageBytes : kDefaultMaxMessageBytes) {}

void WsDecoder::Reset() {
    buf_.clear();
    frag_.clear();
    fragOpcode_ = 0;
    failed_ = false;
    sawClose_ = false;
    closeCode_ = kWsCloseNormal;
    closeReason_.clear();
}

bool WsDecoder::Fail(uint16_t code, const char* reason) {
    // First failure wins: once the stream is broken, later observations are
    // just noise and must not overwrite the real cause.
    if (!failed_) {
        failed_ = true;
        closeCode_ = code;
        closeReason_ = reason ? reason : "";
    }
    return false;
}

bool WsDecoder::Feed(const std::string& chunk, std::vector<WsMessage>* out) {
    return Feed(chunk.data(), chunk.size(), out);
}

bool WsDecoder::Feed(const char* data, std::size_t len, std::vector<WsMessage>* out) {
    if (failed_) return false;
    std::vector<WsMessage> sink;
    if (out == 0) out = &sink;

    if (data != 0 && len != 0) buf_.append(data, len);

    // Drain as many whole frames as the buffer now contains. This loop is what
    // makes the decoder chunk-boundary agnostic: it does not care whether a
    // Feed() delivered a fragment of one frame or three and a half frames.
    for (;;) {
        int r = TryOneFrame(out);
        if (r > 0) continue;      // consumed a frame, try for another
        if (r == 0) return true;  // incomplete frame at the front: need more
        return false;             // protocol failure; CloseCode() says why
    }
}

int WsDecoder::TryOneFrame(std::vector<WsMessage>* out) {
    if (buf_.size() < 2) return 0;
    const unsigned char* p = reinterpret_cast<const unsigned char*>(buf_.data());

    const bool fin = (p[0] & 0x80) != 0;
    const unsigned rsv = static_cast<unsigned>(p[0] & 0x70);
    const int opcode = p[0] & 0x0F;
    const bool masked = (p[1] & 0x80) != 0;

    uint64_t payLen = static_cast<uint64_t>(p[1] & 0x7F);
    std::size_t hdr = 2;

    // --- the three payload length forms ------------------------------------
    if (payLen == 126) {
        if (buf_.size() < 4) return 0;
        payLen = (static_cast<uint64_t>(p[2]) << 8) | static_cast<uint64_t>(p[3]);
        hdr = 4;
    } else if (payLen == 127) {
        if (buf_.size() < 10) return 0;
        uint64_t v = 0;
        for (int i = 0; i < 8; ++i) {
            v = (v << 8) | static_cast<uint64_t>(p[2 + i]);
        }
        // RFC 6455 section 5.2: "the most significant bit MUST be 0". A peer
        // setting it is either broken or probing; either way, refuse.
        if ((v & 0x8000000000000000ULL) != 0) {
            Fail(kWsCloseProtocolError, "64-bit payload length has the MSB set");
            return -1;
        }
        payLen = v;
        hdr = 10;
    }
    if (masked) hdr += 4;

    // --- checks that need only the header ----------------------------------

    // RSV1..3 must be zero: the handshake negotiates no extension, so there is
    // nothing that could give those bits a meaning.
    if (rsv != 0) {
        Fail(kWsCloseProtocolError, "RSV bits set with no extension negotiated");
        return -1;
    }

    const bool isControl = (opcode & 0x08) != 0;

    // Reserved opcodes: data 0x3-0x7 and control 0xB-0xF are undefined.
    if (opcode != kWsContinuation && opcode != kWsText && opcode != kWsBinary &&
        opcode != kWsClose && opcode != kWsPing && opcode != kWsPong) {
        Fail(kWsCloseProtocolError, "reserved opcode");
        return -1;
    }

    if (isControl) {
        // RFC 6455 section 5.5: control frames MUST NOT be fragmented and MUST
        // have a payload of 125 bytes or less. Note this is checked before the
        // payload is waited for, so a control frame claiming 2^40 bytes is
        // rejected instantly instead of buffering.
        if (!fin) {
            Fail(kWsCloseProtocolError, "fragmented control frame");
            return -1;
        }
        if (payLen > kWsMaxControlPayload) {
            Fail(kWsCloseProtocolError, "control frame payload exceeds 125 bytes");
            return -1;
        }
    }

    // --- masking DIRECTION enforcement (RFC 6455 sections 5.1 and 5.3) -----
    // A server MUST close the connection on receiving an unmasked frame, and a
    // client MUST close on receiving a masked one. This is not cosmetic: the
    // mask exists to defeat cache-poisoning of intermediaries, so accepting
    // unmasked client frames would quietly remove that property.
    if (masked != requireMask_) {
        Fail(kWsCloseProtocolError,
             requireMask_ ? "client frame is not masked"
                          : "server frame is masked");
        return -1;
    }

    // --- fragmentation state machine (RFC 6455 section 5.4) ----------------
    if (!isControl) {
        if (opcode == kWsContinuation) {
            if (fragOpcode_ == 0) {
                Fail(kWsCloseProtocolError,
                     "continuation frame with no message in progress");
                return -1;
            }
        } else {
            if (fragOpcode_ != 0) {
                Fail(kWsCloseProtocolError,
                     "new data frame while a fragmented message is in progress");
                return -1;
            }
        }
    }
    // Control frames deliberately fall through all of the above untouched:
    // they MAY be interleaved between the fragments of a data message and must
    // not disturb the reassembly buffer.

    // --- maximum message size, enforced BEFORE buffering the payload -------
    // This is the check that stops a hostile or buggy client from exhausting
    // the machine PC's memory: the 64-bit length form can announce 2^63 bytes,
    // and a decoder that waits for them has already lost. Exceeding the cap is
    // a 1009 close (RFC 6455 section 7.4.1, "Message Too Big").
    if (!isControl) {
        const uint64_t base = (opcode == kWsContinuation)
                                  ? static_cast<uint64_t>(frag_.size())
                                  : 0u;
        if (base + payLen > static_cast<uint64_t>(maxMessage_)) {
            Fail(kWsCloseMessageTooBig, "message exceeds the maximum size");
            return -1;
        }
    }

    // --- do we have the whole payload yet? ---------------------------------
    // Compared in 64-bit so this cannot wrap on a 32-bit build. Safe to reach
    // here only because payLen is now known to be bounded (<= maxMessage_ for
    // data frames, <= 125 for control frames).
    if (static_cast<uint64_t>(buf_.size()) < static_cast<uint64_t>(hdr) + payLen) {
        return 0;
    }
    const std::size_t n = static_cast<std::size_t>(payLen);

    // --- unmask ------------------------------------------------------------
    std::string payload(buf_.data() + hdr, n);
    if (masked) {
        const unsigned char* k = p + hdr - 4;   // key sits just before the payload
        for (std::size_t i = 0; i < n; ++i) {
            payload[i] = static_cast<char>(
                static_cast<unsigned char>(payload[i]) ^ k[i & 3]);
        }
    }
    buf_.erase(0, hdr + n);

    // --- dispatch ----------------------------------------------------------
    if (isControl) {
        WsMessage m;
        m.opcode = opcode;
        m.payload = payload;
        if (opcode == kWsClose) {
            sawClose_ = true;
            if (n == 1) {
                // RFC 6455 section 5.5.1: if there is a body at all it is at
                // least the 2-byte status code.
                Fail(kWsCloseProtocolError, "close frame with a 1-byte payload");
                return -1;
            }
            if (n >= 2) {
                m.hasCloseCode = true;
                m.closeCode = static_cast<uint16_t>(
                    (static_cast<unsigned char>(payload[0]) << 8) |
                     static_cast<unsigned char>(payload[1]));
                m.closeReason = payload.substr(2);
                // RFC 6455 section 7.4: 1000-1003 and 1007-1011 are defined,
                // 3000-4999 are registered/private. 1004/1005/1006 and
                // anything else must never appear on the wire.
                const uint16_t c = m.closeCode;
                const bool legal = (c >= 1000 && c <= 1003) ||
                                   (c >= 1007 && c <= 1011) ||
                                   (c >= 3000 && c <= 4999);
                if (!legal) {
                    Fail(kWsCloseProtocolError, "illegal close status code");
                    return -1;
                }
                if (validateUtf8_ && !IsValidUtf8(m.closeReason)) {
                    Fail(kWsCloseInvalidPayload, "close reason is not valid UTF-8");
                    return -1;
                }
            }
        }
        out->push_back(m);
        return 1;
    }

    // Data frame.
    if (opcode == kWsContinuation) {
        frag_ += payload;
        if (!fin) return 1;                 // still mid-message
        WsMessage m;
        m.opcode = fragOpcode_;
        m.payload = frag_;
        frag_.clear();
        fragOpcode_ = 0;
        if (validateUtf8_ && m.opcode == kWsText && !IsValidUtf8(m.payload)) {
            Fail(kWsCloseInvalidPayload, "text message is not valid UTF-8");
            return -1;
        }
        out->push_back(m);
        return 1;
    }

    if (!fin) {
        // First fragment of a new message.
        fragOpcode_ = opcode;
        frag_ = payload;
        return 1;
    }

    // Unfragmented single-frame message.
    WsMessage m;
    m.opcode = opcode;
    m.payload = payload;
    if (validateUtf8_ && opcode == kWsText && !IsValidUtf8(m.payload)) {
        // RFC 6455 section 8.1: a text message whose payload is not valid
        // UTF-8 is a 1007 failure, not something to hand upstairs.
        Fail(kWsCloseInvalidPayload, "text message is not valid UTF-8");
        return -1;
    }
    out->push_back(m);
    return 1;
}

}  // namespace webbridge
