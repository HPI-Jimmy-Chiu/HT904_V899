// ===========================================================================
//  WebBridge/WsHandshake.cpp
//
//  See WsHandshake.h for the contract. Implementation notes are inline; the
//  ones worth reading before changing anything:
//
//   * The header block is located FIRST (search for CRLFCRLF) and only then
//     split into lines. That ordering is what makes the size cap effective:
//     until the terminator is seen, nothing is parsed and nothing is copied.
//   * obs-fold (RFC 7230 section 3.2.4) continuation lines are unfolded into
//     the previous header's value as a single space. Modern clients never send
//     them, but a proxy in front of the machine PC still might, and silently
//     mis-parsing a folded Connection header would break the handshake in a
//     way that is very hard to see from the browser side.
//   * Header names are compared case-insensitively because RFC 7230 says they
//     are case-insensitive, and real clients disagree about the casing of
//     "Sec-WebSocket-Key" in particular.
// ===========================================================================
#include "WsHandshake.h"

#include "Sha1.h"
#include "Base64.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace webbridge {

const char* const kWebSocketGuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

// ---------------------------------------------------------------------------
//  small ASCII helpers (locale-independent on purpose -- ::tolower is
//  locale-sensitive and this is wire protocol, not user text)
// ---------------------------------------------------------------------------
static char LowerCh(char c) {
    return (c >= 'A' && c <= 'Z') ? static_cast<char>(c - 'A' + 'a') : c;
}

std::string ToLowerAscii(const std::string& s) {
    std::string r(s);
    for (std::size_t i = 0; i < r.size(); ++i) r[i] = LowerCh(r[i]);
    return r;
}

static bool IsOws(char c) { return c == ' ' || c == '\t'; }

// RFC 7230 section 3.2.6:
//   tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." /
//           "^" / "_" / "`" / "|" / "~" / DIGIT / ALPHA
// Everything else -- SP, HTAB, the separators, and every control or 8-bit byte
// -- is excluded. Note `c` is compared as unsigned so a high-bit byte cannot
// slip through on a signed-char platform.
static bool IsTChar(char c)
{
    const unsigned char u = static_cast<unsigned char>(c);
    if (u >= 'a' && u <= 'z') return true;
    if (u >= 'A' && u <= 'Z') return true;
    if (u >= '0' && u <= '9') return true;
    switch (u) {
        case '!': case '#': case '$': case '%': case '&': case '\'':
        case '*': case '+': case '-': case '.': case '^': case '_':
        case '`': case '|': case '~':
            return true;
        default:
            return false;
    }
}

// token = 1*tchar  (so the empty string is NOT a token)
static bool IsToken(const std::string& s)
{
    if (s.empty()) return false;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (!IsTChar(s[i])) return false;
    }
    return true;
}

static std::string TrimOws(const std::string& s) {
    std::size_t b = 0, e = s.size();
    while (b < e && IsOws(s[b])) ++b;
    while (e > b && IsOws(s[e - 1])) --e;
    return s.substr(b, e - b);
}

static bool EqualsIgnoreCase(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (LowerCh(a[i]) != LowerCh(b[i])) return false;
    }
    return true;
}

// Does a comma-separated header value contain `token` as one of its elements?
// Used for Connection: it legitimately arrives as "keep-alive, Upgrade".
static bool ListContainsToken(const std::string& value, const char* token) {
    std::size_t pos = 0;
    while (pos <= value.size()) {
        std::size_t comma = value.find(',', pos);
        std::string item = (comma == std::string::npos)
                               ? value.substr(pos)
                               : value.substr(pos, comma - pos);
        if (EqualsIgnoreCase(TrimOws(item), token)) return true;
        if (comma == std::string::npos) break;
        pos = comma + 1;
    }
    return false;
}

static int HexVal(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

std::string UrlDecode(const std::string& s, bool plusAsSpace) {
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '%' && i + 2 < s.size()) {
            int hi = HexVal(s[i + 1]);
            int lo = HexVal(s[i + 2]);
            if (hi >= 0 && lo >= 0) {
                out.push_back(static_cast<char>((hi << 4) | lo));
                i += 2;
                continue;
            }
            // Malformed escape: keep the '%' literally rather than dropping
            // input. The path check below still refuses anything dangerous.
        }
        if (plusAsSpace && c == '+') { out.push_back(' '); continue; }
        out.push_back(c);
    }
    return out;
}

bool PathIsSuspicious(const std::string& p) {
    if (p.empty() || p[0] != '/') return true;
    if (p.find('\0') != std::string::npos) return true;
    if (p.find('\\') != std::string::npos) return true;   // Windows separator
    // reject any ".." path segment
    std::size_t seg = 1;
    while (seg <= p.size()) {
        std::size_t slash = p.find('/', seg);
        std::string s = (slash == std::string::npos) ? p.substr(seg)
                                                     : p.substr(seg, slash - seg);
        if (s == "..") return true;
        if (slash == std::string::npos) break;
        seg = slash + 1;
    }
    return false;
}

// ---------------------------------------------------------------------------
//  HttpRequest members
// ---------------------------------------------------------------------------
void HttpRequest::Clear() {
    method.clear();
    target.clear();
    version.clear();
    rawPath.clear();
    path.clear();
    query.clear();
    headers.clear();
}

bool HttpRequest::HasHeader(const std::string& name) const {
    std::string want = ToLowerAscii(name);
    for (std::size_t i = 0; i < headers.size(); ++i) {
        if (headers[i].first == want) return true;
    }
    return false;
}

std::string HttpRequest::Header(const std::string& name) const {
    std::string want = ToLowerAscii(name);
    std::string joined;
    bool found = false;
    for (std::size_t i = 0; i < headers.size(); ++i) {
        if (headers[i].first != want) continue;
        if (found) joined += ", ";      // RFC 7230 section 3.2.2
        joined += headers[i].second;
        found = true;
    }
    return joined;
}

// Walk "a=1&b=2" looking for `name`. `*value` gets the raw (still encoded)
// value; returns false when the key is absent.
static bool FindQueryRaw(const std::string& query, const std::string& name,
                         std::string* value) {
    std::size_t pos = 0;
    while (pos <= query.size()) {
        std::size_t amp = query.find('&', pos);
        std::string item = (amp == std::string::npos) ? query.substr(pos)
                                                     : query.substr(pos, amp - pos);
        if (!item.empty()) {
            std::size_t eq = item.find('=');
            std::string k = (eq == std::string::npos) ? item : item.substr(0, eq);
            if (UrlDecode(k, true) == name) {
                if (value) {
                    *value = (eq == std::string::npos) ? std::string()
                                                       : item.substr(eq + 1);
                }
                return true;
            }
        }
        if (amp == std::string::npos) break;
        pos = amp + 1;
    }
    return false;
}

std::string HttpRequest::QueryParam(const std::string& name,
                                    const std::string& def) const {
    std::string raw;
    if (!FindQueryRaw(query, name, &raw)) return def;
    return UrlDecode(raw, true);
}

bool HttpRequest::HasQueryParam(const std::string& name) const {
    return FindQueryRaw(query, name, 0);
}

// ---------------------------------------------------------------------------
//  ParseHttpRequest
// ---------------------------------------------------------------------------
HttpParseResult ParseHttpRequest(const char* data, std::size_t len,
                                 HttpRequest& out, std::size_t* consumed,
                                 std::size_t maxHeaderBytes) {
    out.Clear();
    if (consumed) *consumed = 0;
    if (data == 0) return kHttpNeedMore;

    // Locate the end of the header block. std::string::find on a std::string
    // built from data would copy; scan the raw bytes instead so an over-long
    // hostile stream costs no allocation at all.
    std::size_t end = std::string::npos;   // index of the first byte of CRLFCRLF
    if (len >= 4) {
        for (std::size_t i = 0; i + 3 < len; ++i) {
            if (data[i] == '\r' && data[i + 1] == '\n' &&
                data[i + 2] == '\r' && data[i + 3] == '\n') {
                end = i;
                break;
            }
        }
    }
    if (end == std::string::npos) {
        // No terminator yet. THE CAP IS CHECKED HERE, before any parsing or
        // copying, which is the whole point of doing the scan first.
        if (len >= maxHeaderBytes) return kHttpTooLarge;
        return kHttpNeedMore;
    }
    std::size_t blockLen = end + 4;
    if (blockLen > maxHeaderBytes) return kHttpTooLarge;

    std::string block(data, end);          // header lines, no trailing CRLFCRLF
    if (consumed) *consumed = blockLen;

    // --- split into CRLF-delimited lines, unfolding obs-fold as we go -------
    std::vector<std::string> lines;
    std::size_t pos = 0;
    while (pos <= block.size()) {
        std::size_t crlf = block.find("\r\n", pos);
        std::string line = (crlf == std::string::npos) ? block.substr(pos)
                                                      : block.substr(pos, crlf - pos);
        // A bare CR or LF inside a line is malformed (and a smuggling vector).
        if (line.find('\r') != std::string::npos ||
            line.find('\n') != std::string::npos) {
            return kHttpBad;
        }
        if (!line.empty() && IsOws(line[0]) && !lines.empty()) {
            // obs-fold: continuation of the previous line.
            lines.back() += " ";
            lines.back() += TrimOws(line);
        } else {
            lines.push_back(line);
        }
        if (crlf == std::string::npos) break;
        pos = crlf + 2;
    }
    if (lines.empty()) return kHttpBad;
    // A folded FIRST line would mean the request line itself began with
    // whitespace, which is malformed.
    if (!lines[0].empty() && IsOws(lines[0][0])) return kHttpBad;

    // --- request line: METHOD SP TARGET SP VERSION -------------------------
    const std::string& rl = lines[0];
    std::size_t sp1 = rl.find(' ');
    if (sp1 == std::string::npos) return kHttpBad;
    std::size_t sp2 = rl.find(' ', sp1 + 1);
    if (sp2 == std::string::npos) return kHttpBad;
    out.method = rl.substr(0, sp1);
    out.target = rl.substr(sp1 + 1, sp2 - sp1 - 1);
    out.version = rl.substr(sp2 + 1);
    if (out.method.empty() || out.target.empty()) return kHttpBad;
    if (out.version.find(' ') != std::string::npos) return kHttpBad;
    if (out.version.compare(0, 5, "HTTP/") != 0) return kHttpBad;

    // --- split target into path + query ------------------------------------
    std::size_t q = out.target.find('?');
    if (q == std::string::npos) {
        out.rawPath = out.target;
        out.query.clear();
    } else {
        out.rawPath = out.target.substr(0, q);
        out.query = out.target.substr(q + 1);
    }
    // Strip a "#fragment" if some client sent one (it should not).
    std::size_t hash = out.query.find('#');
    if (hash != std::string::npos) out.query.erase(hash);
    hash = out.rawPath.find('#');
    if (hash != std::string::npos) out.rawPath.erase(hash);
    out.path = UrlDecode(out.rawPath, false);   // '+' is literal in a path

    // --- header lines ------------------------------------------------------
    for (std::size_t i = 1; i < lines.size(); ++i) {
        const std::string& line = lines[i];
        if (line.empty()) continue;              // tolerate a stray blank line
        std::size_t colon = line.find(':');
        if (colon == std::string::npos || colon == 0) return kHttpBad;
        std::string name = line.substr(0, colon);
        // RFC 7230 section 3.2: field-name = token, and SP/HTAB are not tchar.
        // Checking only the character before the colon (which is all this did
        // before) accepts "Bad Name: v", because its last character is 'e'.
        // The whole name has to be a token -- section 3.2.4 makes rejecting
        // malformed field-names mandatory, and this endpoint is one that can
        // eventually command machine motion, so it parses strictly.
        if (!IsToken(name)) return kHttpBad;
        out.headers.push_back(
            std::make_pair(ToLowerAscii(name), TrimOws(line.substr(colon + 1))));
    }
    return kHttpOk;
}

// ---------------------------------------------------------------------------
//  Handshake validation
// ---------------------------------------------------------------------------
bool IsValidWebSocketKey(const std::string& key) {
    // RFC 6455 section 4.1: the key is "a base64-encoded (see Section 4 of
    // [RFC4648]) value that, when decoded, is 16 bytes in length". That means
    // exactly 24 characters, the last two being the '=' padding for a 16 % 3 == 1
    // remainder.
    //
    // The shape check is done here rather than delegated entirely to
    // Base64Decode because that decoder deliberately skips ASCII whitespace
    // (legal for line-wrapped MIME base64), so on its own it would accept a
    // wrapped or padded key of the wrong length.
    if (key.size() != 24) return false;
    if (key[22] != '=' || key[23] != '=') return false;
    for (std::size_t i = 0; i < 22; ++i) {
        char c = key[i];
        bool ok = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
                  (c >= '0' && c <= '9') || c == '+' || c == '/';
        if (!ok) return false;
    }
    // Then confirm it really decodes, and to exactly 16 bytes.
    std::string raw;
    if (!Base64Decode(key, &raw)) return false;
    return raw.size() == 16;
}

bool LooksLikeWebSocketUpgrade(const HttpRequest& req) {
    return ListContainsToken(req.Header("upgrade"), "websocket");
}

HandshakeResult CheckWebSocketUpgrade(const HttpRequest& req, std::string* whyNot) {
    if (whyNot) whyNot->clear();

    if (!LooksLikeWebSocketUpgrade(req)) {
        if (whyNot) *whyNot = "no Upgrade: websocket header";
        return kHsNotWebSocket;
    }
    // From here on the client is definitely attempting a WebSocket handshake,
    // so every remaining failure is an error response, not a static file.
    if (req.method != "GET") {
        if (whyNot) *whyNot = "method is not GET";
        return kHsMethodNotAllowed;
    }
    if (req.version != "HTTP/1.1") {
        if (whyNot) *whyNot = "HTTP version is not 1.1";
        return kHsBadRequest;
    }
    if (!ListContainsToken(req.Header("connection"), "upgrade")) {
        if (whyNot) *whyNot = "Connection header does not contain the upgrade token";
        return kHsBadRequest;
    }
    if (!req.HasHeader("sec-websocket-version")) {
        if (whyNot) *whyNot = "missing Sec-WebSocket-Version";
        return kHsBadRequest;
    }
    if (TrimOws(req.Header("sec-websocket-version")) != "13") {
        // RFC 6455 section 4.4: the right answer is 426 plus the version we do
        // speak, so the client can retry instead of guessing.
        if (whyNot) *whyNot = "Sec-WebSocket-Version is not 13";
        return kHsUpgradeRequired;
    }
    std::string key = TrimOws(req.Header("sec-websocket-key"));
    if (key.empty()) {
        if (whyNot) *whyNot = "missing Sec-WebSocket-Key";
        return kHsBadRequest;
    }
    if (!IsValidWebSocketKey(key)) {
        if (whyNot) *whyNot = "Sec-WebSocket-Key is not base64 of 16 bytes";
        return kHsBadRequest;
    }
    return kHsOk;
}

// ---------------------------------------------------------------------------
//  Response construction
// ---------------------------------------------------------------------------
std::string ComputeAcceptKey(const std::string& secWebSocketKey) {
    // RFC 6455 section 4.2.2 step 5: base64(SHA1(key + GUID)). The key is
    // concatenated exactly as received (trimmed by the caller), NOT decoded.
    std::string toHash = secWebSocketKey;
    toHash += kWebSocketGuid;
    std::string digest = WB_SHA1_RAW(toHash);      // 20 raw bytes
    return WB_BASE64_ENCODE(digest);
}

std::string BuildHandshakeResponse(const std::string& secWebSocketKey,
                                   const std::string& subprotocol) {
    std::string r;
    r += "HTTP/1.1 101 Switching Protocols\r\n";
    r += "Upgrade: websocket\r\n";
    r += "Connection: Upgrade\r\n";
    r += "Sec-WebSocket-Accept: ";
    r += ComputeAcceptKey(secWebSocketKey);
    r += "\r\n";
    if (!subprotocol.empty()) {
        r += "Sec-WebSocket-Protocol: ";
        r += subprotocol;
        r += "\r\n";
    }
    // Deliberately NO Sec-WebSocket-Extensions: nothing is negotiated, which
    // is what entitles WsFrame's decoder to reject non-zero RSV bits.
    r += "\r\n";
    return r;
}

static std::string SimpleResponse(const char* statusLine, const char* extraHeader) {
    std::string r;
    r += "HTTP/1.1 ";
    r += statusLine;
    r += "\r\n";
    if (extraHeader && *extraHeader) {
        r += extraHeader;
        r += "\r\n";
    }
    r += "Content-Length: 0\r\n";
    r += "Connection: close\r\n";
    r += "\r\n";
    return r;
}

std::string BuildHandshakeErrorResponse(HandshakeResult why) {
    switch (why) {
        case kHsUpgradeRequired:
            return SimpleResponse("426 Upgrade Required", "Sec-WebSocket-Version: 13");
        case kHsMethodNotAllowed:
            return SimpleResponse("405 Method Not Allowed", "Allow: GET");
        case kHsOk:            // caller error -- never send a 200 for a handshake
        case kHsNotWebSocket:
        case kHsBadRequest:
        default:
            return SimpleResponse("400 Bad Request", 0);
    }
}

std::string BuildTooLargeResponse() {
    return SimpleResponse("431 Request Header Fields Too Large", 0);
}

}  // namespace webbridge
