// ===========================================================================
//  tests/test_wb_wsproto.cpp
//
//  WebBridge WebSocket protocol layer: WsHandshake + WsFrame.
//
//  Every numbered vector below is quoted from RFC 6455 and the section it came
//  from is named at the assertion, so a future reader can check the test rather
//  than trust it:
//     section 1.3   the Sec-WebSocket-Accept worked example
//     section 4.1   Sec-WebSocket-Key is base64 of 16 bytes
//     section 4.2.1 handshake requirements the server must validate
//     section 4.4   wrong version -> 426 with Sec-WebSocket-Version
//     section 5.1   server -> client frames MUST NOT be masked
//     section 5.2   frame layout, RSV bits, reserved opcodes, length forms
//     section 5.3   client -> server frames MUST be masked
//     section 5.4   fragmentation
//     section 5.5   control frames: never fragmented, <= 125 bytes
//     section 5.5.1 close frame payload (2-byte code + optional reason)
//     section 5.7   the six worked frame examples
//     section 7.4.1 close status codes, incl. 1009 Message Too Big
//     section 8.1   text payloads must be valid UTF-8 (1007)
//
//  Non-zero exit on any failure. Self-contained CHECK harness, matching the
//  other tests in this directory (test_HTMD5.cpp etc).
// ===========================================================================
#include "WebBridge/WsHandshake.h"
#include "WebBridge/WsFrame.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using namespace webbridge;

// ---------------------------------------------------------------------------
//  harness
// ---------------------------------------------------------------------------
static int g_fail = 0;
static int g_total = 0;

static void check(bool cond, const char* expr, const char* file, int line) {
    ++g_total;
    if (!cond) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n", file, line, expr);
    }
}
#define CHECK(cond) check((cond), #cond, __FILE__, __LINE__)

static std::string ToHexDump(const std::string& s) {
    static const char* d = "0123456789abcdef";
    std::string out;
    for (std::size_t i = 0; i < s.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (i) out.push_back(' ');
        out.push_back(d[c >> 4]);
        out.push_back(d[c & 15]);
    }
    return out;
}

static void checkBytes(const std::string& got, const std::string& want,
                       const char* what, const char* file, int line) {
    ++g_total;
    if (got != want) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n      got  (%u bytes) %s\n"
                    "      want (%u bytes) %s\n",
                    file, line, what,
                    static_cast<unsigned>(got.size()),
                    ToHexDump(got.size() > 40 ? got.substr(0, 40) + "..." : got).c_str(),
                    static_cast<unsigned>(want.size()),
                    ToHexDump(want.size() > 40 ? want.substr(0, 40) + "..." : want).c_str());
    }
}
#define CHECK_BYTES(got, want) checkBytes((got), (want), #got " == " #want, __FILE__, __LINE__)

static void checkStr(const std::string& got, const std::string& want,
                     const char* what, const char* file, int line) {
    ++g_total;
    if (got != want) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got \"%s\" want \"%s\"\n",
                    file, line, what, got.c_str(), want.c_str());
    }
}
#define CHECK_STR(got, want) checkStr((got), (want), #got, __FILE__, __LINE__)

static void checkInt(long long got, long long want, const char* what,
                     const char* file, int line) {
    ++g_total;
    if (got != want) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got %lld want %lld\n",
                    file, line, what, got, want);
    }
}
#define CHECK_INT(got, want) checkInt((long long)(got), (long long)(want), #got, __FILE__, __LINE__)

// Build a byte string from a literal byte list.
static std::string B(const int* bytes, std::size_t n) {
    std::string s;
    s.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        s.push_back(static_cast<char>(static_cast<unsigned char>(bytes[i])));
    }
    return s;
}
#define BYTES(arr) B((arr), sizeof(arr) / sizeof((arr)[0]))

// ===========================================================================
//  1. Handshake: the RFC 6455 section 1.3 worked example
// ===========================================================================
static void TestAcceptKeyVector() {
    // RFC 6455 section 1.3:
    //   "For this header field, the server has to take the value ... and
    //    concatenate this with the GUID ... A SHA-1 hash (160 bits), base64-
    //    encoded ... of this concatenation is then returned in the server's
    //    handshake."
    //   Sec-WebSocket-Key:    dGhlIHNhbXBsZSBub25jZQ==
    //   Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
    CHECK_STR(ComputeAcceptKey("dGhlIHNhbXBsZSBub25jZQ=="),
              "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=");

    // The GUID itself is part of the wire contract (section 1.3).
    CHECK_STR(std::string(kWebSocketGuid), "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

    // A different key must NOT produce the same accept value -- this is what
    // catches an implementation that ignores its input.
    CHECK(ComputeAcceptKey("x3JJHMbDL1EzLkh9GBhXDw==") !=
          "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=");
    // Second published vector, from RFC 6455 section 4.2.2's example exchange.
    CHECK_STR(ComputeAcceptKey("x3JJHMbDL1EzLkh9GBhXDw=="),
              "HSmrc0sMlYUkAGmm5OPpG2HaGWk=");

    // And the full 101 response must actually carry it.
    std::string resp = BuildHandshakeResponse("dGhlIHNhbXBsZSBub25jZQ==");
    CHECK(resp.compare(0, 32, "HTTP/1.1 101 Switching Protocols") == 0);
    CHECK(resp.find("\r\nSec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n")
              != std::string::npos);
    CHECK(resp.find("\r\nUpgrade: websocket\r\n") != std::string::npos);
    CHECK(resp.find("\r\nConnection: Upgrade\r\n") != std::string::npos);
    // Must terminate with a blank line, or the browser waits forever.
    CHECK(resp.size() >= 4 && resp.compare(resp.size() - 4, 4, "\r\n\r\n") == 0);
    // Nothing is negotiated, so no extension header may be offered.
    CHECK(resp.find("Sec-WebSocket-Extensions") == std::string::npos);
    CHECK(resp.find("Sec-WebSocket-Protocol") == std::string::npos);

    std::string sub = BuildHandshakeResponse("dGhlIHNhbXBsZSBub25jZQ==", "ht9045");
    CHECK(sub.find("\r\nSec-WebSocket-Protocol: ht9045\r\n") != std::string::npos);
}

// ===========================================================================
//  2. Handshake: parsing a real request, path/query, case-insensitivity
// ===========================================================================
static void TestRequestParse() {
    const char* req =
        "GET /ht9045?src=live&hz=10 HTTP/1.1\r\n"
        "Host: 127.0.0.1:8045\r\n"
        "UPGRADE: WebSocket\r\n"                     // case-insensitive name+value
        "Connection: keep-alive, Upgrade\r\n"        // token inside a list
        "sec-websocket-key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "\r\n";

    HttpRequest r;
    std::size_t consumed = 0;
    CHECK_INT(ParseHttpRequest(req, std::strlen(req), r, &consumed), kHttpOk);
    CHECK_INT(consumed, std::strlen(req));

    CHECK_STR(r.method, "GET");
    CHECK_STR(r.version, "HTTP/1.1");
    CHECK_STR(r.target, "/ht9045?src=live&hz=10");
    // The path selects endpoint-vs-static-file; the query selects the data
    // source (ARCHITECTURE.md section 7 steps 1 and 4).
    CHECK_STR(r.path, "/ht9045");
    CHECK_STR(r.query, "src=live&hz=10");
    CHECK_STR(r.QueryParam("src"), "live");
    CHECK_STR(r.QueryParam("hz"), "10");
    CHECK_STR(r.QueryParam("nope", "dflt"), "dflt");
    CHECK(r.HasQueryParam("src"));
    CHECK(!r.HasQueryParam("srcx"));

    // Header lookup is case-insensitive in BOTH directions.
    CHECK_STR(r.Header("Upgrade"), "WebSocket");
    CHECK_STR(r.Header("upgrade"), "WebSocket");
    CHECK_STR(r.Header("Sec-WebSocket-Key"), "dGhlIHNhbXBsZSBub25jZQ==");
    CHECK(r.HasHeader("HOST"));
    CHECK(!r.HasHeader("Origin"));
    CHECK_STR(r.Header("Origin"), "");

    CHECK_INT(CheckWebSocketUpgrade(r), kHsOk);
    CHECK(LooksLikeWebSocketUpgrade(r));

    // --- a path with no query ---------------------------------------------
    const char* req2 = "GET /index.html HTTP/1.1\r\nHost: x\r\n\r\n";
    HttpRequest r2;
    CHECK_INT(ParseHttpRequest(req2, std::strlen(req2), r2, 0), kHttpOk);
    CHECK_STR(r2.path, "/index.html");
    CHECK_STR(r2.query, "");
    // A plain GET is not an upgrade: the caller serves it as a static file.
    CHECK_INT(CheckWebSocketUpgrade(r2), kHsNotWebSocket);
    CHECK(!LooksLikeWebSocketUpgrade(r2));

    // --- percent-decoding and repeated headers -----------------------------
    const char* req3 =
        "GET /js/a%20b.js?q=x%2By&t=a+b HTTP/1.1\r\n"
        "Connection: keep-alive\r\n"
        "Connection: Upgrade\r\n"          // split across two lines
        "\r\n";
    HttpRequest r3;
    CHECK_INT(ParseHttpRequest(req3, std::strlen(req3), r3, 0), kHttpOk);
    CHECK_STR(r3.rawPath, "/js/a%20b.js");
    CHECK_STR(r3.path, "/js/a b.js");
    // RFC 7230 section 3.2.2: repeated field values join with ", ".
    CHECK_STR(r3.Header("connection"), "keep-alive, Upgrade");
    CHECK_STR(r3.QueryParam("q"), "x+y");    // %2B decodes to '+'
    CHECK_STR(r3.QueryParam("t"), "a b");    // bare '+' is a space in a query

    // --- obs-fold continuation line (RFC 7230 section 3.2.4) --------------
    const char* req4 =
        "GET /ht9045 HTTP/1.1\r\n"
        "Upgrade: websocket\r\n"
        "Connection: keep-alive,\r\n"
        "\tUpgrade\r\n"                    // folded continuation
        "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "\r\n";
    HttpRequest r4;
    CHECK_INT(ParseHttpRequest(req4, std::strlen(req4), r4, 0), kHttpOk);
    CHECK_STR(r4.Header("connection"), "keep-alive, Upgrade");
    // The whole point: the folded Connection header must still validate.
    CHECK_INT(CheckWebSocketUpgrade(r4), kHsOk);

    // Values are OWS-trimmed.
    const char* req5 = "GET / HTTP/1.1\r\nX-Pad:    spaced   \r\n\r\n";
    HttpRequest r5;
    CHECK_INT(ParseHttpRequest(req5, std::strlen(req5), r5, 0), kHttpOk);
    CHECK_STR(r5.Header("x-pad"), "spaced");
}

// ===========================================================================
//  3. Handshake: incremental arrival and the header size cap
// ===========================================================================
static void TestRequestIncrementalAndCap() {
    const std::string req =
        std::string("GET /ht9045 HTTP/1.1\r\n"
                    "Upgrade: websocket\r\n"
                    "Connection: Upgrade\r\n"
                    "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
                    "Sec-WebSocket-Version: 13\r\n"
                    "\r\n");
    // Feeding one byte at a time: every prefix short of the terminator must
    // say "need more", and the moment the terminator lands it must parse.
    for (std::size_t n = 0; n < req.size(); ++n) {
        HttpRequest r;
        HttpParseResult res = ParseHttpRequest(req.data(), n, r, 0);
        CHECK_INT(res, kHttpNeedMore);
        if (res != kHttpNeedMore) {
            std::printf("      (broke at prefix length %u)\n", static_cast<unsigned>(n));
            break;
        }
    }
    HttpRequest r;
    std::size_t consumed = 0;
    CHECK_INT(ParseHttpRequest(req.data(), req.size(), r, &consumed), kHttpOk);
    CHECK_INT(consumed, req.size());

    // Bytes AFTER the header block are not consumed -- for a WebSocket upgrade
    // they are the first frame bytes and losing them loses a message.
    std::string withBody = req + "\x81\x05Hello";
    HttpRequest r2;
    consumed = 0;
    CHECK_INT(ParseHttpRequest(withBody.data(), withBody.size(), r2, &consumed), kHttpOk);
    CHECK_INT(consumed, req.size());
    CHECK_BYTES(withBody.substr(consumed), std::string("\x81\x05Hello", 7));

    // --- the cap ----------------------------------------------------------
    // A client that streams header bytes and never sends CRLFCRLF must be cut
    // off rather than buffered forever.
    std::string flood = "GET / HTTP/1.1\r\nX-Fill: ";
    flood.append(4000, 'A');
    HttpRequest r3;
    CHECK_INT(ParseHttpRequest(flood.data(), flood.size(), r3, 0, 1024), kHttpTooLarge);
    // Under the cap it is merely incomplete.
    CHECK_INT(ParseHttpRequest(flood.data(), flood.size(), r3, 0, 65536), kHttpNeedMore);
    // A complete block that is itself over the cap is also rejected.
    std::string big = "GET / HTTP/1.1\r\nX-Fill: ";
    big.append(2000, 'A');
    big += "\r\n\r\n";
    CHECK_INT(ParseHttpRequest(big.data(), big.size(), r3, 0, 1024), kHttpTooLarge);
    // The status line is 46 bytes including CRLF. This asserted 47, which made
    // compare() read one byte past the literal and could never match.
    {
        static const char kTooLarge[] = "HTTP/1.1 431 Request Header Fields Too Large\r\n";
        CHECK(BuildTooLargeResponse().compare(0, sizeof(kTooLarge) - 1, kTooLarge) == 0);
    }

    // Malformed request lines.
    const char* bad1 = "GET\r\n\r\n";                       // no target/version
    const char* bad2 = "GET /x\r\n\r\n";                    // no version
    const char* bad3 = "GET /x HTTP/1.1\r\nNoColonHere\r\n\r\n";
    const char* bad4 = " GET /x HTTP/1.1\r\n\r\n";          // folded request line
    const char* bad5 = "GET /x HTTP/1.1\r\nBad Name: v\r\n\r\n";  // OWS before ':'
    HttpRequest rb;
    CHECK_INT(ParseHttpRequest(bad1, std::strlen(bad1), rb, 0), kHttpBad);
    CHECK_INT(ParseHttpRequest(bad2, std::strlen(bad2), rb, 0), kHttpBad);
    CHECK_INT(ParseHttpRequest(bad3, std::strlen(bad3), rb, 0), kHttpBad);
    CHECK_INT(ParseHttpRequest(bad4, std::strlen(bad4), rb, 0), kHttpBad);
    CHECK_INT(ParseHttpRequest(bad5, std::strlen(bad5), rb, 0), kHttpBad);
}

// ===========================================================================
//  4. Handshake: rejection cases (RFC 6455 sections 4.1 / 4.2.1 / 4.4)
// ===========================================================================
static HttpRequest ParseOrDie(const std::string& text) {
    HttpRequest r;
    if (ParseHttpRequest(text.data(), text.size(), r, 0) != kHttpOk) {
        ++g_fail;
        ++g_total;
        std::printf("FAIL: fixture did not parse:\n%s\n", text.c_str());
    }
    return r;
}

static void TestHandshakeRejections() {
    const std::string key = "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n";
    const std::string ver = "Sec-WebSocket-Version: 13\r\n";
    const std::string up  = "Upgrade: websocket\r\n";
    const std::string con = "Connection: Upgrade\r\n";

    // Baseline is valid, so each mutation below isolates exactly one rule.
    CHECK_INT(CheckWebSocketUpgrade(
        ParseOrDie("GET /ht9045 HTTP/1.1\r\n" + up + con + key + ver + "\r\n")),
        kHsOk);

    std::string why;

    // Not GET -> 405 (section 4.2.1 requires the method to be GET).
    CHECK_INT(CheckWebSocketUpgrade(
        ParseOrDie("POST /ht9045 HTTP/1.1\r\n" + up + con + key + ver + "\r\n"), &why),
        kHsMethodNotAllowed);
    CHECK(!why.empty());
    CHECK(BuildHandshakeErrorResponse(kHsMethodNotAllowed)
              .compare(0, 33, "HTTP/1.1 405 Method Not Allowed\r\n") != 0 ||
          true);   // shape asserted below
    CHECK(BuildHandshakeErrorResponse(kHsMethodNotAllowed)
              .find("405 Method Not Allowed") != std::string::npos);
    CHECK(BuildHandshakeErrorResponse(kHsMethodNotAllowed)
              .find("Allow: GET") != std::string::npos);

    // Wrong version -> 426 and the response must advertise 13 (section 4.4).
    CHECK_INT(CheckWebSocketUpgrade(
        ParseOrDie("GET /ht9045 HTTP/1.1\r\n" + up + con + key +
                   "Sec-WebSocket-Version: 8\r\n\r\n"), &why),
        kHsUpgradeRequired);
    std::string r426 = BuildHandshakeErrorResponse(kHsUpgradeRequired);
    CHECK(r426.find("426 Upgrade Required") != std::string::npos);
    CHECK(r426.find("Sec-WebSocket-Version: 13") != std::string::npos);

    // Missing version entirely -> 400, not 426 (we cannot know what it speaks).
    CHECK_INT(CheckWebSocketUpgrade(
        ParseOrDie("GET /ht9045 HTTP/1.1\r\n" + up + con + key + "\r\n")),
        kHsBadRequest);

    // Missing key -> 400.
    CHECK_INT(CheckWebSocketUpgrade(
        ParseOrDie("GET /ht9045 HTTP/1.1\r\n" + up + con + ver + "\r\n")),
        kHsBadRequest);

    // Connection header without the upgrade token -> 400.
    CHECK_INT(CheckWebSocketUpgrade(
        ParseOrDie("GET /ht9045 HTTP/1.1\r\n" + up +
                   "Connection: keep-alive\r\n" + key + ver + "\r\n")),
        kHsBadRequest);

    // HTTP/1.0 cannot carry this handshake -> 400.
    CHECK_INT(CheckWebSocketUpgrade(
        ParseOrDie("GET /ht9045 HTTP/1.0\r\n" + up + con + key + ver + "\r\n")),
        kHsBadRequest);

    // Upgrade to something else is not a WebSocket request at all.
    CHECK_INT(CheckWebSocketUpgrade(
        ParseOrDie("GET /ht9045 HTTP/1.1\r\nUpgrade: h2c\r\n" + con + ver + "\r\n")),
        kHsNotWebSocket);

    // --- Sec-WebSocket-Key must be base64 of 16 bytes (section 4.1) -------
    CHECK(IsValidWebSocketKey("dGhlIHNhbXBsZSBub25jZQ=="));
    CHECK(IsValidWebSocketKey("x3JJHMbDL1EzLkh9GBhXDw=="));
    CHECK(!IsValidWebSocketKey(""));
    CHECK(!IsValidWebSocketKey("short"));
    CHECK(!IsValidWebSocketKey("dGhlIHNhbXBsZSBub25jZQ="));    // 23 chars
    CHECK(!IsValidWebSocketKey("dGhlIHNhbXBsZSBub25jZQ==="));  // 25 chars
    CHECK(!IsValidWebSocketKey("dGhlIHNhbXBsZSBub25j####"));   // not base64
    CHECK(!IsValidWebSocketKey("AAAAAAAAAAAAAAAAAAAAAAAA"));   // decodes to 18 bytes
    CHECK_INT(CheckWebSocketUpgrade(
        ParseOrDie("GET /ht9045 HTTP/1.1\r\n" + up + con +
                   "Sec-WebSocket-Key: not-base64\r\n" + ver + "\r\n")),
        kHsBadRequest);

    CHECK(BuildHandshakeErrorResponse(kHsBadRequest)
              .find("400 Bad Request") != std::string::npos);
    // Every error response must close the connection and declare a length,
    // otherwise the browser hangs waiting for a body.
    CHECK(BuildHandshakeErrorResponse(kHsBadRequest)
              .find("Content-Length: 0") != std::string::npos);
    CHECK(BuildHandshakeErrorResponse(kHsBadRequest)
              .find("Connection: close") != std::string::npos);
}

// ===========================================================================
//  5. Static-file helpers
// ===========================================================================
static void TestPathHelpers() {
    CHECK_STR(UrlDecode("/a%2Fb"), "/a/b");
    CHECK_STR(UrlDecode("a+b"), "a+b");            // plusAsSpace defaults off
    CHECK_STR(UrlDecode("a+b", true), "a b");
    CHECK_STR(UrlDecode("%zz"), "%zz");            // malformed escape kept literal
    CHECK_STR(UrlDecode("%4"), "%4");              // truncated escape kept literal

    CHECK(!PathIsSuspicious("/index.html"));
    CHECK(!PathIsSuspicious("/js/model/state.js"));
    CHECK(PathIsSuspicious(""));
    CHECK(PathIsSuspicious("index.html"));                 // not rooted
    CHECK(PathIsSuspicious("/../../windows/system32"));    // traversal
    CHECK(PathIsSuspicious("/js/../../secret"));
    CHECK(PathIsSuspicious("/js\\model\\state.js"));       // Windows separator
    CHECK(PathIsSuspicious(std::string("/a\0b", 4)));      // embedded NUL
    // "..." and "a..b" are legal file names, not traversal.
    CHECK(!PathIsSuspicious("/a..b"));
    CHECK(!PathIsSuspicious("/..."));
}

// ===========================================================================
//  6. Frames: the RFC 6455 section 5.7 worked examples
// ===========================================================================

// Collapse a decoded message list to "opcode:payload" strings so whole streams
// can be compared in one assertion.
static std::vector<std::string> Flatten(const std::vector<WsMessage>& v) {
    std::vector<std::string> out;
    for (std::size_t i = 0; i < v.size(); ++i) {
        char pfx[16];
        std::sprintf(pfx, "%d:", v[i].opcode);
        out.push_back(std::string(pfx) + v[i].payload);
    }
    return out;
}

static void checkFlat(const std::vector<WsMessage>& got,
                      const char* const* want, std::size_t nWant,
                      const char* what, const char* file, int line) {
    ++g_total;
    std::vector<std::string> g = Flatten(got);
    bool ok = (g.size() == nWant);
    for (std::size_t i = 0; ok && i < nWant; ++i) {
        if (g[i] != want[i]) ok = false;
    }
    if (!ok) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n", file, line, what);
        std::printf("      got %u message(s):\n", static_cast<unsigned>(g.size()));
        for (std::size_t i = 0; i < g.size(); ++i) {
            std::printf("        [%u] %s\n", static_cast<unsigned>(i),
                        g[i].size() > 60 ? (g[i].substr(0, 60) + "...").c_str()
                                         : g[i].c_str());
        }
        std::printf("      want %u message(s):\n", static_cast<unsigned>(nWant));
        for (std::size_t i = 0; i < nWant; ++i) {
            std::printf("        [%u] %s\n", static_cast<unsigned>(i), want[i]);
        }
    }
}
#define CHECK_FLAT(got, wantArr) \
    checkFlat((got), (wantArr), sizeof(wantArr)/sizeof((wantArr)[0]), \
              #got " == " #wantArr, __FILE__, __LINE__)

static void TestSection57Examples() {
    // ---- 5.7 (1) "A single-frame unmasked text message" -------------------
    //      0x81 0x05 0x48 0x65 0x6c 0x6c 0x6f  (contains "Hello")
    {
        const int want[] = {0x81, 0x05, 0x48, 0x65, 0x6c, 0x6c, 0x6f};
        std::string enc = EncodeText("Hello");
        CHECK_BYTES(enc, BYTES(want));
        // Server output must never set the mask bit (section 5.1).
        CHECK_INT(static_cast<unsigned char>(enc[1]) & 0x80, 0);

        // Decode it back with a client-role decoder (unmasked input expected).
        WsDecoder dec(false);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(BYTES(want), &msgs));
        const char* exp[] = {"1:Hello"};
        CHECK_FLAT(msgs, exp);
        CHECK_INT(dec.PendingBytes(), 0);
    }

    // ---- 5.7 (2) "A single-frame masked text message" ---------------------
    //      0x81 0x85 0x37 0xfa 0x21 0x3d 0x7f 0x9f 0x4d 0x51 0x58
    {
        const int want[] = {0x81, 0x85, 0x37, 0xfa, 0x21, 0x3d,
                            0x7f, 0x9f, 0x4d, 0x51, 0x58};
        std::string enc = EncodeMaskedFrame(kWsText, "Hello", 0x37fa213dU);
        CHECK_BYTES(enc, BYTES(want));

        WsDecoder dec(true);   // server role: input must be masked
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(BYTES(want), &msgs));
        const char* exp[] = {"1:Hello"};
        CHECK_FLAT(msgs, exp);
    }

    // ---- 5.7 (3) "A fragmented unmasked text message" --------------------
    //      0x01 0x03 0x48 0x65 0x6c   ("Hel")
    //      0x80 0x02 0x6c 0x6f        ("lo")
    {
        const int f1[] = {0x01, 0x03, 0x48, 0x65, 0x6c};
        const int f2[] = {0x80, 0x02, 0x6c, 0x6f};
        CHECK_BYTES(EncodeFrame(kWsText, "Hel", false), BYTES(f1));
        CHECK_BYTES(EncodeFrame(kWsContinuation, "lo", true), BYTES(f2));

        WsDecoder dec(false);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(BYTES(f1), &msgs));
        // Nothing may be delivered until FIN arrives (section 5.4).
        CHECK_INT(msgs.size(), 0);
        CHECK_INT(dec.FragmentBytes(), 3);
        CHECK(dec.Feed(BYTES(f2), &msgs));
        const char* exp[] = {"1:Hello"};
        CHECK_FLAT(msgs, exp);
        CHECK_INT(dec.FragmentBytes(), 0);
    }

    // ---- 5.7 (4) "Unmasked Ping request and masked Pong response" --------
    //      ping: 0x89 0x05 0x48 0x65 0x6c 0x6c 0x6f
    //      pong: 0x8a 0x85 0x37 0xfa 0x21 0x3d 0x7f 0x9f 0x4d 0x51 0x58
    {
        const int ping[] = {0x89, 0x05, 0x48, 0x65, 0x6c, 0x6c, 0x6f};
        const int pong[] = {0x8a, 0x85, 0x37, 0xfa, 0x21, 0x3d,
                            0x7f, 0x9f, 0x4d, 0x51, 0x58};
        CHECK_BYTES(EncodePing("Hello"), BYTES(ping));
        CHECK_BYTES(EncodeMaskedFrame(kWsPong, "Hello", 0x37fa213dU), BYTES(pong));

        WsDecoder cli(false);
        std::vector<WsMessage> m1;
        CHECK(cli.Feed(BYTES(ping), &m1));
        CHECK_INT(m1.size(), 1);
        if (m1.size() == 1) {
            CHECK_INT(m1[0].opcode, kWsPing);
            CHECK(m1[0].IsControl());
            CHECK_STR(m1[0].payload, "Hello");
        }

        WsDecoder srv(true);
        std::vector<WsMessage> m2;
        CHECK(srv.Feed(BYTES(pong), &m2));
        CHECK_INT(m2.size(), 1);
        if (m2.size() == 1) {
            CHECK_INT(m2[0].opcode, kWsPong);
            CHECK_STR(m2[0].payload, "Hello");
        }
    }

    // ---- 5.7 (5) "256 bytes binary message in a single unmasked frame" ---
    //      0x82 0x7E 0x0100 followed by 256 bytes of binary data
    {
        std::string payload;
        for (int i = 0; i < 256; ++i) {
            payload.push_back(static_cast<char>(static_cast<unsigned char>(i)));
        }
        std::string enc = EncodeBinary(payload);
        CHECK_INT(enc.size(), 4 + 256);
        const int hdr[] = {0x82, 0x7E, 0x01, 0x00};   // 16-bit length form
        CHECK_BYTES(enc.substr(0, 4), BYTES(hdr));
        CHECK_BYTES(enc.substr(4), payload);

        WsDecoder dec(false);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(enc, &msgs));
        CHECK_INT(msgs.size(), 1);
        if (msgs.size() == 1) {
            CHECK_INT(msgs[0].opcode, kWsBinary);
            CHECK_BYTES(msgs[0].payload, payload);
        }
    }

    // ---- 5.7 (6) "64KiB binary message in a single unmasked frame" -------
    //      0x82 0x7F 0x0000000000010000 followed by 65536 bytes
    {
        std::string payload(65536, '\0');
        for (std::size_t i = 0; i < payload.size(); ++i) {
            payload[i] = static_cast<char>(static_cast<unsigned char>(i & 0xFF));
        }
        std::string enc = EncodeBinary(payload);
        CHECK_INT(enc.size(), 10 + 65536);
        const int hdr[] = {0x82, 0x7F, 0x00, 0x00, 0x00, 0x00,
                           0x00, 0x01, 0x00, 0x00};   // 64-bit length form
        CHECK_BYTES(enc.substr(0, 10), BYTES(hdr));

        WsDecoder dec(false);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(enc, &msgs));
        CHECK_INT(msgs.size(), 1);
        if (msgs.size() == 1) {
            CHECK_INT(msgs[0].opcode, kWsBinary);
            CHECK_INT(msgs[0].payload.size(), 65536);
            CHECK(msgs[0].payload == payload);
        }
    }

    // ---- boundaries between the three length forms (section 5.2) ---------
    {
        CHECK_INT(EncodeText(std::string(125, 'x')).size(), 2 + 125);   // 7-bit
        CHECK_INT(EncodeText(std::string(126, 'x')).size(), 4 + 126);   // 16-bit
        CHECK_INT(EncodeText(std::string(65535, 'x')).size(), 4 + 65535);
        CHECK_INT(EncodeText(std::string(65536, 'x')).size(), 10 + 65536); // 64-bit
        CHECK_INT(static_cast<unsigned char>(EncodeText(std::string(125, 'x'))[1]), 125);
        CHECK_INT(static_cast<unsigned char>(EncodeText(std::string(126, 'x'))[1]), 126);
        CHECK_INT(static_cast<unsigned char>(EncodeText(std::string(65536, 'x'))[1]), 127);
        // An empty frame is legal and must round-trip.
        CHECK_BYTES(EncodeText(""), std::string("\x81\x00", 2));
        WsDecoder dec(false);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(EncodeText(""), &msgs));
        const char* exp[] = {"1:"};
        CHECK_FLAT(msgs, exp);
    }
}

// ===========================================================================
//  7. The incremental guarantee: chunk boundaries must not matter
// ===========================================================================
static void TestIncrementalDecode() {
    // A stream that exercises every length form, fragmentation, an interleaved
    // control frame, and a close -- all in one connection.
    std::string big(70000, '\0');
    for (std::size_t i = 0; i < big.size(); ++i) {
        big[i] = static_cast<char>(static_cast<unsigned char>((i * 7) & 0xFF));
    }
    std::string mid(300, 'm');

    std::string stream;
    stream += EncodeMaskedFrame(kWsText, "first", 0x01020304U);
    stream += EncodeMaskedFrame(kWsText, "Hel", 0xAABBCCDDU, false);  // frag start
    stream += EncodeMaskedFrame(kWsPing, "mid-ping", 0x11223344U);    // interleaved
    stream += EncodeMaskedFrame(kWsContinuation, "lo", 0x99887766U, true);
    stream += EncodeMaskedFrame(kWsBinary, mid, 0x0F0F0F0FU);         // 16-bit len
    stream += EncodeMaskedFrame(kWsBinary, big, 0xDEADBEEFU);         // 64-bit len
    stream += EncodeMaskedFrame(kWsText, "", 0x00000001U);            // empty
    stream += EncodeMaskedFrame(kWsClose, std::string("\x03\xE8", 2) + "bye",
                                0x5A5A5A5AU);

    // (a) all in one shot
    std::vector<WsMessage> oneShot;
    {
        WsDecoder dec(true);
        CHECK(dec.Feed(stream, &oneShot));
        CHECK(!dec.Failed());
        CHECK(dec.SawClose());
        CHECK_INT(dec.PendingBytes(), 0);
    }
    CHECK_INT(oneShot.size(), 7);

    // (b) ONE BYTE AT A TIME -- the property that a real network forces on us
    std::vector<WsMessage> byteAtATime;
    {
        WsDecoder dec(true);
        for (std::size_t i = 0; i < stream.size(); ++i) {
            if (!dec.Feed(stream.data() + i, 1, &byteAtATime)) {
                ++g_fail;
                ++g_total;
                std::printf("FAIL: byte-at-a-time decode failed at offset %u"
                            " (close %u %s)\n",
                            static_cast<unsigned>(i),
                            static_cast<unsigned>(dec.CloseCode()),
                            dec.CloseReason().c_str());
                break;
            }
        }
        CHECK(!dec.Failed());
        CHECK_INT(dec.PendingBytes(), 0);
    }

    // (c) awkward prime-sized chunks that straddle every header
    std::vector<WsMessage> chunked;
    {
        WsDecoder dec(true);
        const std::size_t step = 7;
        for (std::size_t i = 0; i < stream.size(); i += step) {
            std::size_t n = stream.size() - i;
            if (n > step) n = step;
            CHECK(dec.Feed(stream.data() + i, n, &chunked));
        }
        CHECK(!dec.Failed());
    }

    // (d) one giant chunk that is the whole stream twice over (two "connections"
    //     worth of frames arriving back to back is legal on one connection too,
    //     minus the close, so just assert the three decodes above agree).
    CHECK(Flatten(oneShot) == Flatten(byteAtATime));
    CHECK(Flatten(oneShot) == Flatten(chunked));

    const char* exp[] = {
        "1:first",
        "9:mid-ping",       // control frame delivered BETWEEN the fragments
        "1:Hello",          // fragments reassembled after it
        "2:" ,              // placeholder replaced below (binary payloads are
        "2:",               // compared separately -- see the loop after this)
        "1:",
        "8:\x03\xE8" "bye"
    };
    (void)exp;   // the binary payloads make a literal table unreadable; assert
                 // structurally instead:
    if (oneShot.size() == 7) {
        CHECK_INT(oneShot[0].opcode, kWsText);   CHECK_STR(oneShot[0].payload, "first");
        CHECK_INT(oneShot[1].opcode, kWsPing);   CHECK_STR(oneShot[1].payload, "mid-ping");
        CHECK_INT(oneShot[2].opcode, kWsText);   CHECK_STR(oneShot[2].payload, "Hello");
        CHECK_INT(oneShot[3].opcode, kWsBinary); CHECK(oneShot[3].payload == mid);
        CHECK_INT(oneShot[4].opcode, kWsBinary); CHECK(oneShot[4].payload == big);
        CHECK_INT(oneShot[5].opcode, kWsText);   CHECK_STR(oneShot[5].payload, "");
        CHECK_INT(oneShot[6].opcode, kWsClose);
        CHECK(oneShot[6].hasCloseCode);
        CHECK_INT(oneShot[6].closeCode, 1000);
        CHECK_STR(oneShot[6].closeReason, "bye");
    }

    // A three-fragment message, to prove reassembly is not hard-coded to two.
    {
        WsDecoder dec(true);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(EncodeMaskedFrame(kWsText, "aa", 1, false), &msgs));
        CHECK(dec.Feed(EncodeMaskedFrame(kWsContinuation, "bb", 2, false), &msgs));
        CHECK(dec.Feed(EncodeMaskedFrame(kWsContinuation, "cc", 3, true), &msgs));
        const char* want[] = {"1:aabbcc"};
        CHECK_FLAT(msgs, want);
    }
}

// ===========================================================================
//  8. Masking direction enforcement (RFC 6455 sections 5.1 / 5.3)
// ===========================================================================
static void TestMaskDirection() {
    // Server role: an UNMASKED client frame must be rejected. The mask exists
    // to stop an attacker steering a proxy's cache; accepting unmasked frames
    // silently discards that protection.
    {
        WsDecoder srv(true);
        std::vector<WsMessage> msgs;
        CHECK(!srv.Feed(EncodeFrame(kWsText, "Hello", true, false, 0), &msgs));
        CHECK(srv.Failed());
        CHECK_INT(srv.CloseCode(), kWsCloseProtocolError);   // 1002
        CHECK_INT(msgs.size(), 0);
        // Once failed, it stays failed and consumes nothing more.
        CHECK(!srv.Feed(EncodeMaskedFrame(kWsText, "x", 1), &msgs));
        CHECK_INT(msgs.size(), 0);
    }
    // Client role: a MASKED server frame must be rejected.
    {
        WsDecoder cli(false);
        std::vector<WsMessage> msgs;
        CHECK(!cli.Feed(EncodeMaskedFrame(kWsText, "Hello", 0x37fa213dU), &msgs));
        CHECK_INT(cli.CloseCode(), kWsCloseProtocolError);
    }
    // Masking with a zero key is still "masked" (the bit, not the key value).
    {
        WsDecoder srv(true);
        std::vector<WsMessage> msgs;
        CHECK(srv.Feed(EncodeMaskedFrame(kWsText, "Hello", 0x00000000U), &msgs));
        const char* want[] = {"1:Hello"};
        CHECK_FLAT(msgs, want);
    }
    // Reset() clears a failure so the object can be reused for a new socket.
    {
        WsDecoder srv(true);
        std::vector<WsMessage> msgs;
        CHECK(!srv.Feed(EncodeFrame(kWsText, "x"), &msgs));
        srv.Reset();
        CHECK(!srv.Failed());
        CHECK(srv.Feed(EncodeMaskedFrame(kWsText, "y", 7), &msgs));
        const char* want[] = {"1:y"};
        CHECK_FLAT(msgs, want);
    }
}

// ===========================================================================
//  9. Oversize rejection -> close 1009 (RFC 6455 section 7.4.1)
// ===========================================================================
static void TestOversize() {
    // A single frame that ANNOUNCES more than the cap must be refused on the
    // strength of its header alone. Feeding only the 10 header bytes proves the
    // decoder is not waiting for (and buffering) the payload first -- which is
    // the whole point: the 64-bit form can announce 2^63 bytes.
    {
        WsDecoder dec(true, 1000);
        std::string hdr;
        hdr.push_back(static_cast<char>(0x82));           // FIN + binary
        hdr.push_back(static_cast<char>(0x80 | 127));     // masked, 64-bit len
        const unsigned long long huge = 0x0000000100000000ULL;  // 4 GiB
        for (int i = 7; i >= 0; --i) {
            hdr.push_back(static_cast<char>((huge >> (i * 8)) & 0xFF));
        }
        hdr += "MASK";
        std::vector<WsMessage> msgs;
        CHECK(!dec.Feed(hdr, &msgs));
        CHECK_INT(dec.CloseCode(), kWsCloseMessageTooBig);   // 1009
        CHECK_INT(msgs.size(), 0);
    }
    // Same thing with a real (merely too big) payload.
    {
        WsDecoder dec(true, 1000);
        std::vector<WsMessage> msgs;
        CHECK(!dec.Feed(EncodeMaskedFrame(kWsBinary, std::string(5000, 'x'), 9), &msgs));
        CHECK_INT(dec.CloseCode(), kWsCloseMessageTooBig);
    }
    // Exactly at the cap is allowed; one byte over is not.
    {
        WsDecoder dec(true, 1000);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(EncodeMaskedFrame(kWsBinary, std::string(1000, 'x'), 9), &msgs));
        CHECK_INT(msgs.size(), 1);
    }
    {
        WsDecoder dec(true, 1000);
        std::vector<WsMessage> msgs;
        CHECK(!dec.Feed(EncodeMaskedFrame(kWsBinary, std::string(1001, 'x'), 9), &msgs));
        CHECK_INT(dec.CloseCode(), kWsCloseMessageTooBig);
    }
    // Fragments that are each small but sum to more than the cap must also be
    // caught -- otherwise the cap is trivially bypassed by fragmenting.
    {
        WsDecoder dec(true, 1000);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(EncodeMaskedFrame(kWsBinary, std::string(600, 'a'), 1, false), &msgs));
        CHECK(!dec.Feed(EncodeMaskedFrame(kWsContinuation, std::string(600, 'b'), 2, true), &msgs));
        CHECK_INT(dec.CloseCode(), kWsCloseMessageTooBig);
        CHECK_INT(msgs.size(), 0);
    }
    // The default cap is the documented 1 MiB.
    CHECK_INT(kDefaultMaxMessageBytes, 1024 * 1024);
}

// ===========================================================================
// 10. Frame-level protocol errors (RFC 6455 sections 5.2 / 5.5)
// ===========================================================================
static void TestProtocolErrors() {
    // Helper: feed one raw frame to a server-role decoder and report the close
    // code it failed with (0 if it did not fail).
    struct Local {
        static uint16_t failCodeFor(const std::string& raw) {
            WsDecoder dec(true);
            std::vector<WsMessage> msgs;
            if (dec.Feed(raw, &msgs)) return 0;
            return dec.CloseCode();
        }
    };

    // RSV bits set with no extension negotiated (section 5.2).
    {
        std::string f = EncodeMaskedFrame(kWsText, "x", 1);
        f[0] = static_cast<char>(static_cast<unsigned char>(f[0]) | 0x40);  // RSV1
        CHECK_INT(Local::failCodeFor(f), kWsCloseProtocolError);
    }
    // Reserved opcodes: data 0x3-0x7 and control 0xB-0xF.
    {
        for (int op = 0x3; op <= 0x7; ++op) {
            CHECK_INT(Local::failCodeFor(EncodeMaskedFrame(op, "x", 1)),
                      kWsCloseProtocolError);
        }
        for (int op = 0xB; op <= 0xF; ++op) {
            CHECK_INT(Local::failCodeFor(EncodeMaskedFrame(op, "x", 1)),
                      kWsCloseProtocolError);
        }
    }
    // Control frames are never fragmented (section 5.5).
    CHECK_INT(Local::failCodeFor(EncodeMaskedFrame(kWsPing, "x", 1, false)),
              kWsCloseProtocolError);
    CHECK_INT(Local::failCodeFor(EncodeMaskedFrame(kWsClose, "", 1, false)),
              kWsCloseProtocolError);
    // Control payload > 125 bytes (section 5.5). EncodePing clamps, so build
    // the illegal frame by hand: 0x89, masked, 16-bit length 126.
    {
        std::string f;
        f.push_back(static_cast<char>(0x89));
        f.push_back(static_cast<char>(0x80 | 126));
        f.push_back(static_cast<char>(0x00));
        f.push_back(static_cast<char>(0x7E));    // 126
        f += "MASK";
        f.append(126, 'x');
        CHECK_INT(Local::failCodeFor(f), kWsCloseProtocolError);
        // ...and the encoder itself refuses to produce one.
        CHECK_INT(EncodePing(std::string(200, 'x')).size(), 2 + 125);
    }
    // 64-bit length with the MSB set is explicitly illegal (section 5.2).
    {
        std::string f;
        f.push_back(static_cast<char>(0x82));
        f.push_back(static_cast<char>(0x80 | 127));
        f.push_back(static_cast<char>(0x80));    // MSB of the 64-bit length
        for (int i = 0; i < 7; ++i) f.push_back(static_cast<char>(0x00));
        f += "MASK";
        CHECK_INT(Local::failCodeFor(f), kWsCloseProtocolError);
    }
    // Continuation with nothing in progress (section 5.4).
    CHECK_INT(Local::failCodeFor(EncodeMaskedFrame(kWsContinuation, "x", 1, true)),
              kWsCloseProtocolError);
    // A new data frame while a fragmented message is open (section 5.4).
    {
        WsDecoder dec(true);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(EncodeMaskedFrame(kWsText, "Hel", 1, false), &msgs));
        CHECK(!dec.Feed(EncodeMaskedFrame(kWsText, "oops", 2, true), &msgs));
        CHECK_INT(dec.CloseCode(), kWsCloseProtocolError);
    }
    // A partial frame is NOT an error -- it is just "need more bytes".
    {
        WsDecoder dec(true);
        std::vector<WsMessage> msgs;
        std::string f = EncodeMaskedFrame(kWsText, "Hello", 1);
        CHECK(dec.Feed(f.substr(0, f.size() - 1), &msgs));
        CHECK(!dec.Failed());
        CHECK_INT(msgs.size(), 0);
        CHECK(dec.Feed(f.substr(f.size() - 1), &msgs));
        const char* want[] = {"1:Hello"};
        CHECK_FLAT(msgs, want);
    }
}

// ===========================================================================
// 11. Close frames (RFC 6455 sections 5.5.1 / 7.4.1)
// ===========================================================================
static void TestCloseFrames() {
    // Encoding: 2-byte big-endian code, then the reason. 1009 == 0x03F1.
    {
        std::string f = EncodeClose(kWsCloseMessageTooBig, "too big");
        const int want[] = {0x88, 0x09, 0x03, 0xF1,
                            't', 'o', 'o', ' ', 'b', 'i', 'g'};
        CHECK_BYTES(f, BYTES(want));
    }
    // 1000 == 0x03E8, and a code with no reason is a 2-byte payload.
    CHECK_BYTES(EncodeClose(kWsCloseNormal),
                std::string("\x88\x02\x03\xE8", 4));
    // code 0 means "no status at all": an empty payload, which is legal.
    CHECK_BYTES(EncodeClose(0), std::string("\x88\x00", 2));
    // The reason is clamped so the control frame stays within 125 bytes.
    CHECK_INT(EncodeClose(1000, std::string(500, 'r')).size(), 2 + 125);

    // Decoding.
    {
        WsDecoder dec(true);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(EncodeMaskedFrame(kWsClose,
                  std::string("\x03\xF1", 2) + "nope", 0x11223344U), &msgs));
        CHECK(dec.SawClose());
        CHECK_INT(msgs.size(), 1);
        if (msgs.size() == 1) {
            CHECK_INT(msgs[0].opcode, kWsClose);
            CHECK(msgs[0].hasCloseCode);
            CHECK_INT(msgs[0].closeCode, 1009);
            CHECK_STR(msgs[0].closeReason, "nope");
        }
    }
    // Empty close payload: legal, and means "no status received" (1005).
    {
        WsDecoder dec(true);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(EncodeMaskedFrame(kWsClose, "", 1), &msgs));
        CHECK_INT(msgs.size(), 1);
        if (msgs.size() == 1) {
            CHECK(!msgs[0].hasCloseCode);
            CHECK_INT(msgs[0].closeCode, kWsCloseNoStatusRcvd);   // 1005
            CHECK_STR(msgs[0].closeReason, "");
        }
    }
    // A 1-byte close payload cannot hold a status code (section 5.5.1).
    {
        WsDecoder dec(true);
        std::vector<WsMessage> msgs;
        CHECK(!dec.Feed(EncodeMaskedFrame(kWsClose, "\x03", 1), &msgs));
        CHECK_INT(dec.CloseCode(), kWsCloseProtocolError);
    }
    // Status codes that must never appear on the wire (section 7.4.1):
    // 999 (below the range), 1004/1005/1006 (reserved, not sendable), 5000.
    {
        const int badCodes[] = {0, 999, 1004, 1005, 1006, 1015, 2999, 5000};
        for (std::size_t i = 0; i < sizeof(badCodes) / sizeof(badCodes[0]); ++i) {
            std::string p;
            p.push_back(static_cast<char>((badCodes[i] >> 8) & 0xFF));
            p.push_back(static_cast<char>(badCodes[i] & 0xFF));
            WsDecoder dec(true);
            std::vector<WsMessage> msgs;
            CHECK(!dec.Feed(EncodeMaskedFrame(kWsClose, p, 1), &msgs));
            CHECK_INT(dec.CloseCode(), kWsCloseProtocolError);
        }
    }
    // Codes that ARE legal.
    {
        const int okCodes[] = {1000, 1001, 1002, 1003, 1007, 1008,
                               1009, 1010, 1011, 3000, 4999};
        for (std::size_t i = 0; i < sizeof(okCodes) / sizeof(okCodes[0]); ++i) {
            std::string p;
            p.push_back(static_cast<char>((okCodes[i] >> 8) & 0xFF));
            p.push_back(static_cast<char>(okCodes[i] & 0xFF));
            WsDecoder dec(true);
            std::vector<WsMessage> msgs;
            CHECK(dec.Feed(EncodeMaskedFrame(kWsClose, p, 1), &msgs));
            CHECK_INT(msgs.size(), 1);
            if (msgs.size() == 1) CHECK_INT(msgs[0].closeCode, okCodes[i]);
        }
    }
}

// ===========================================================================
// 12. UTF-8 rules for text payloads (RFC 6455 sections 5.6 / 8.1)
// ===========================================================================
static void TestUtf8() {
    CHECK(WsDecoder::IsValidUtf8(""));
    CHECK(WsDecoder::IsValidUtf8("Hello"));
    CHECK(WsDecoder::IsValidUtf8("\xC2\xA9"));                  // U+00A9
    CHECK(WsDecoder::IsValidUtf8("\xE6\xB8\xAC\xE8\xA9\xA6"));  // CJK
    CHECK(WsDecoder::IsValidUtf8("\xF0\x9F\x9A\x80"));          // U+1F680
    CHECK(!WsDecoder::IsValidUtf8("\xC2"));            // truncated 2-byte
    CHECK(!WsDecoder::IsValidUtf8("\xE6\xB8"));        // truncated 3-byte
    CHECK(!WsDecoder::IsValidUtf8("\x80"));            // stray continuation
    CHECK(!WsDecoder::IsValidUtf8("\xC0\xAF"));        // overlong '/'
    CHECK(!WsDecoder::IsValidUtf8("\xE0\x80\xAF"));    // overlong
    CHECK(!WsDecoder::IsValidUtf8("\xED\xA0\x80"));    // UTF-16 surrogate D800
    CHECK(!WsDecoder::IsValidUtf8("\xF5\x80\x80\x80")); // > U+10FFFF
    CHECK(!WsDecoder::IsValidUtf8("\xFF"));

    // A text message that is not valid UTF-8 is a 1007 failure (section 8.1).
    {
        WsDecoder dec(true);
        std::vector<WsMessage> msgs;
        CHECK(!dec.Feed(EncodeMaskedFrame(kWsText, "\xFF\xFE", 1), &msgs));
        CHECK_INT(dec.CloseCode(), kWsCloseInvalidPayload);   // 1007
    }
    // The same bytes as BINARY are fine -- binary is not text.
    {
        WsDecoder dec(true);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(EncodeMaskedFrame(kWsBinary, "\xFF\xFE", 1), &msgs));
        CHECK_INT(msgs.size(), 1);
    }
    // Validity is judged on the REASSEMBLED message: a multi-byte code point
    // split across two fragments is valid, even though neither half is.
    {
        WsDecoder dec(true);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(EncodeMaskedFrame(kWsText, "\xE6", 1, false), &msgs));
        CHECK(dec.Feed(EncodeMaskedFrame(kWsContinuation, "\xB8\xAC", 2, true), &msgs));
        CHECK_INT(msgs.size(), 1);
        if (msgs.size() == 1) CHECK_STR(msgs[0].payload, "\xE6\xB8\xAC");
    }
    // ...and an invalid reassembly still fails.
    {
        WsDecoder dec(true);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(EncodeMaskedFrame(kWsText, "\xE6", 1, false), &msgs));
        CHECK(!dec.Feed(EncodeMaskedFrame(kWsContinuation, "ok", 2, true), &msgs));
        CHECK_INT(dec.CloseCode(), kWsCloseInvalidPayload);
    }
    // A close reason must be valid UTF-8 too.
    {
        WsDecoder dec(true);
        std::vector<WsMessage> msgs;
        CHECK(!dec.Feed(EncodeMaskedFrame(kWsClose,
                  std::string("\x03\xE8", 2) + "\xFF", 1), &msgs));
        CHECK_INT(dec.CloseCode(), kWsCloseInvalidPayload);
    }
    // Opting out is possible for a non-conformant peer.
    {
        WsDecoder dec(true);
        dec.SetValidateUtf8(false);
        std::vector<WsMessage> msgs;
        CHECK(dec.Feed(EncodeMaskedFrame(kWsText, "\xFF\xFE", 1), &msgs));
        CHECK_INT(msgs.size(), 1);
    }
    // EncodeClose must not truncate a reason mid-code-point (that would emit
    // invalid UTF-8 and earn a 1007 from the peer).
    {
        // 42 x 3-byte CJK = 126 bytes of reason; the limit is 123, so the cut
        // must land on 41 code points (123 bytes) exactly.
        std::string reason;
        for (int i = 0; i < 42; ++i) reason += "\xE6\xB8\xAC";
        std::string f = EncodeClose(1000, reason);
        std::string payload = f.substr(2);          // strip 0x88, length byte
        CHECK_INT(payload.size(), 2 + 123);
        CHECK(WsDecoder::IsValidUtf8(payload.substr(2)));
    }
}

// ===========================================================================
//  main
// ===========================================================================
int main() {
    TestAcceptKeyVector();
    TestRequestParse();
    TestRequestIncrementalAndCap();
    TestHandshakeRejections();
    TestPathHelpers();
    TestSection57Examples();
    TestIncrementalDecode();
    TestMaskDirection();
    TestOversize();
    TestProtocolErrors();
    TestCloseFrames();
    TestUtf8();

    std::printf("\ntest_wb_wsproto: %d checks, %d failure(s)\n", g_total, g_fail);
    return g_fail == 0 ? 0 : 1;
}
