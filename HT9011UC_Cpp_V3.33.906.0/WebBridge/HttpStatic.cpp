// =============================================================================
//  WebBridge/HttpStatic.cpp -- see HttpStatic.h for the contract and the
//  reasoning behind the path rules.
// =============================================================================
#include "WebBridge/HttpStatic.h"

#include <windows.h>

#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

namespace webbridge {

namespace {

const size_t kMaxTargetLen = 2048;          // longest request target we accept
const long long kMaxFileBytes = 64LL << 20; // refuse to buffer more than 64 MiB

inline char LowerAscii(char c)
{
    return (c >= 'A' && c <= 'Z') ? static_cast<char>(c - 'A' + 'a') : c;
}

std::string LowerAsciiStr(const std::string& s)
{
    std::string out(s);
    for (size_t i = 0; i < out.size(); ++i) out[i] = LowerAscii(out[i]);
    return out;
}

inline bool IsAlpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int HexVal(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

// The Windows-illegal filename characters that are not otherwise handled.
// ':' is handled separately so a drive-letter attack gets its own reason.
bool IsIllegalPathChar(char c)
{
    return c == '<' || c == '>' || c == '"' || c == '|' || c == '*';
}

// Lowercase extension including the dot, e.g. ".js". Empty if none.
std::string ExtensionOf(const std::string& path)
{
    size_t lastSep = path.find_last_of("/\\");
    size_t start = (lastSep == std::string::npos) ? 0 : lastSep + 1;
    size_t dot = path.rfind('.');
    if (dot == std::string::npos || dot < start || dot + 1 >= path.size()) return std::string();
    return LowerAsciiStr(path.substr(dot));
}

// --- junction / symlink net -------------------------------------------------
// The lexical checks in SanitizeUrlPath cannot see a reparse point placed
// INSIDE the document root that points outside it. GetFinalPathNameByHandle
// resolves that. Bound at run time via GetProcAddress so the code builds on
// old MinGW headers as well as MSVC.
typedef DWORD (WINAPI *PfnGetFinalPathNameByHandleA)(HANDLE, LPSTR, DWORD, DWORD);

PfnGetFinalPathNameByHandleA LoadFinalPathFn()
{
    static PfnGetFinalPathNameByHandleA fn = 0;
    static bool tried = false;
    if (!tried) {
        tried = true;
        HMODULE h = GetModuleHandleA("kernel32.dll");
        if (h) {
            fn = reinterpret_cast<PfnGetFinalPathNameByHandleA>(
                     reinterpret_cast<void*>(GetProcAddress(h, "GetFinalPathNameByHandleA")));
        }
    }
    return fn;
}

// Returns "" when the true path cannot be determined.
std::string TrueFilesystemPath(const std::string& path)
{
    PfnGetFinalPathNameByHandleA fn = LoadFinalPathFn();
    if (!fn) return std::string();

    HANDLE h = CreateFileA(path.c_str(), 0,
                           FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                           0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);
    if (h == INVALID_HANDLE_VALUE) return std::string();

    char buf[1024];
    DWORD n = fn(h, buf, static_cast<DWORD>(sizeof(buf) - 1), 0 /*NORMALIZED|DOS*/);
    CloseHandle(h);
    if (n == 0 || n >= sizeof(buf)) return std::string();
    buf[n] = '\0';

    std::string out(buf);
    // Strip the "\\?\" long-path prefix so it compares against a plain root.
    if (out.compare(0, 4, "\\\\?\\") == 0) out.erase(0, 4);
    return NormalizeWindowsPath(out);
}

bool PathHasPrefixDir(const std::string& path, const std::string& dir)
{
    if (dir.empty() || path.size() <= dir.size()) return false;
    if (LowerAsciiStr(path.substr(0, dir.size())) != LowerAsciiStr(dir)) return false;
    return path[dir.size()] == '\\';
}

}  // namespace

// -----------------------------------------------------------------------------
const char* PathRejectName(PathReject r)
{
    switch (r) {
        case kPathOk:                 return "ok";
        case kPathRejectEmpty:        return "empty-target";
        case kPathRejectNotAbsolute:  return "not-absolute";
        case kPathRejectBadPercent:   return "malformed-percent-escape";
        case kPathRejectNulByte:      return "nul-byte";
        case kPathRejectControlChar:  return "control-char";
        case kPathRejectBackslash:    return "backslash";
        case kPathRejectDotDot:       return "dot-dot-segment";
        case kPathRejectDriveLetter:  return "drive-letter";
        case kPathRejectUnc:          return "unc-path";
        case kPathRejectIllegalChar:  return "illegal-char";
        case kPathRejectEscapesRoot:  return "escapes-document-root";
    }
    return "unknown";
}

int HttpStatusForReject(PathReject r)
{
    switch (r) {
        case kPathOk:                return 200;
        case kPathRejectEmpty:       return 400;
        case kPathRejectNotAbsolute: return 400;
        case kPathRejectBadPercent:  return 400;
        default:                     return 403;  // everything else is an attack
    }
}

// -----------------------------------------------------------------------------
//  MIME map. The ".js" line is load-bearing: a wrong type here and the browser
//  refuses to execute the ES module, so nothing renders at all.
// -----------------------------------------------------------------------------
const char* MimeTypeForPath(const std::string& path)
{
    const std::string ext = ExtensionOf(path);
    if (ext == ".js"  ) return "text/javascript";           // <-- must be this
    if (ext == ".mjs" ) return "text/javascript";
    if (ext == ".html") return "text/html; charset=utf-8";
    if (ext == ".htm" ) return "text/html; charset=utf-8";
    if (ext == ".css" ) return "text/css; charset=utf-8";
    if (ext == ".json") return "application/json";
    if (ext == ".map" ) return "application/json";
    if (ext == ".svg" ) return "image/svg+xml";
    if (ext == ".jpg" ) return "image/jpeg";
    if (ext == ".jpeg") return "image/jpeg";
    if (ext == ".png" ) return "image/png";
    if (ext == ".ico" ) return "image/x-icon";
    if (ext == ".txt" ) return "text/plain; charset=utf-8";
    if (ext == ".woff2") return "font/woff2";
    return "application/octet-stream";
}

// -----------------------------------------------------------------------------
bool PercentDecode(const std::string& in, std::string& out)
{
    out.clear();
    out.reserve(in.size());
    for (size_t i = 0; i < in.size(); ++i) {
        const char c = in[i];
        if (c != '%') { out.push_back(c); continue; }
        if (i + 2 >= in.size()) return false;         // "%", "%2"
        const int hi = HexVal(in[i + 1]);
        const int lo = HexVal(in[i + 2]);
        if (hi < 0 || lo < 0) return false;           // "%zz"
        out.push_back(static_cast<char>((hi << 4) | lo));
        i += 2;
    }
    return true;
}

// -----------------------------------------------------------------------------
std::string NormalizeWindowsPath(const std::string& path)
{
    std::string p(path);
    for (size_t i = 0; i < p.size(); ++i) if (p[i] == '/') p[i] = '\\';

    const bool unc = (p.size() >= 2 && p[0] == '\\' && p[1] == '\\');
    const bool rooted = (!p.empty() && p[0] == '\\');

    std::vector<std::string> parts;
    size_t i = 0;
    while (i < p.size()) {
        while (i < p.size() && p[i] == '\\') ++i;
        size_t start = i;
        while (i < p.size() && p[i] != '\\') ++i;
        if (i > start) {
            std::string seg = p.substr(start, i - start);
            if (seg == ".") continue;
            if (seg == "..") {
                // Never pop past a drive/UNC prefix.
                if (!parts.empty() && parts.back() != ".." &&
                    !(parts.size() == 1 && parts[0].size() == 2 && parts[0][1] == ':')) {
                    parts.pop_back();
                }
                continue;
            }
            parts.push_back(seg);
        }
    }

    std::string out;
    if (unc) out = "\\\\";
    else if (rooted) out = "\\";
    for (size_t k = 0; k < parts.size(); ++k) {
        if (k) out += "\\";
        out += parts[k];
    }
    const bool endsSep = !out.empty() && out[out.size() - 1] == '\\';
    if (out.size() > 1 && endsSep && !(out.size() == 2 && unc)) out.erase(out.size() - 1);
    return out;
}

// -----------------------------------------------------------------------------
PathReject SanitizeUrlPath(const std::string& target, std::string& outRel)
{
    outRel.clear();

    if (target.empty()) return kPathRejectEmpty;
    if (target.size() > kMaxTargetLen) return kPathRejectIllegalChar;

    // A NUL anywhere in the raw target is an attack, decoded or not.
    if (target.find('\0') != std::string::npos) return kPathRejectNulByte;

    // Strip query and fragment before anything else.
    std::string path = target;
    const size_t cut = path.find_first_of("?#");
    if (cut != std::string::npos) path.erase(cut);
    if (path.empty()) return kPathRejectEmpty;

    // Only origin-form targets. This alone kills "c:\windows\..." and
    // "../../etc" style targets that never had a leading slash.
    if (path[0] != '/') return kPathRejectNotAbsolute;

    // Exactly one round of percent-decoding, BEFORE the content checks, so
    // "%2e%2e%2f" is seen as "../".
    std::string dec;
    if (!PercentDecode(path, dec)) return kPathRejectBadPercent;
    if (dec.empty() || dec[0] != '/') return kPathRejectNotAbsolute;

    // "//host/share" -- a UNC path smuggled in as a network-path reference.
    if (dec.size() >= 2 && dec[1] == '/') return kPathRejectUnc;

    for (size_t i = 0; i < dec.size(); ++i) {
        const unsigned char c = static_cast<unsigned char>(dec[i]);
        if (c == 0) return kPathRejectNulByte;
        if (c < 0x20 || c == 0x7f) return kPathRejectControlChar;
        if (dec[i] == '\\') return kPathRejectBackslash;
        if (dec[i] == ':') {
            // "/c:/windows/win.ini" and NTFS ADS "index.html::$DATA".
            if (i >= 1 && IsAlpha(dec[i - 1])) return kPathRejectDriveLetter;
            return kPathRejectIllegalChar;
        }
        if (IsIllegalPathChar(dec[i])) return kPathRejectIllegalChar;
    }

    // Split into segments; "." disappears, ".." is refused outright (we do not
    // "helpfully" resolve it -- a well-formed request from this UI never has one).
    std::string rel;
    size_t i = 0;
    while (i < dec.size()) {
        while (i < dec.size() && dec[i] == '/') ++i;
        const size_t start = i;
        while (i < dec.size() && dec[i] != '/') ++i;
        if (i == start) continue;
        const std::string seg = dec.substr(start, i - start);
        if (seg == ".") continue;
        if (seg == "..") return kPathRejectDotDot;
        // Windows silently strips trailing dots and spaces, which turns
        // "secret.txt." into "secret.txt" behind any name-based filter.
        const char last = seg[seg.size() - 1];
        if (last == '.' || last == ' ') return kPathRejectIllegalChar;
        if (!rel.empty()) rel += "/";
        rel += seg;
    }

    // "/" and "/subdir/" mean index.html.
    if (rel.empty())                              rel = "index.html";
    else if (dec[dec.size() - 1] == '/')          rel += "/index.html";

    outRel = rel;
    return kPathOk;
}

// -----------------------------------------------------------------------------
//  HttpResponse
// -----------------------------------------------------------------------------
HttpResponse::HttpResponse()
    : status(200), reason("OK"), headOnly(false), contentLength(0)
{
}

std::string HttpResponse::ToWire() const
{
    std::ostringstream os;
    os << "HTTP/1.1 " << status << " " << reason << "\r\n";
    if (!contentType.empty()) os << "Content-Type: " << contentType << "\r\n";
    os << "Content-Length: " << contentLength << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = extraHeaders.begin();
         it != extraHeaders.end(); ++it) {
        os << it->first << ": " << it->second << "\r\n";
    }
    // The HMI reads live machine values over the WebSocket; nothing static
    // should be cached while this is under development.
    os << "Cache-Control: no-store\r\n";
    os << "X-Content-Type-Options: nosniff\r\n";
    os << "Connection: close\r\n";
    os << "\r\n";
    std::string head = os.str();
    if (headOnly) return head;
    return head + body;
}

// -----------------------------------------------------------------------------
//  HttpStatic
// -----------------------------------------------------------------------------
HttpStatic::HttpStatic()
{
}

HttpStatic::HttpStatic(const std::string& documentRoot)
{
    SetDocumentRoot(documentRoot);
}

void HttpStatic::SetDocumentRoot(const std::string& documentRoot)
{
    root_ = NormalizeWindowsPath(documentRoot);
}

PathReject HttpStatic::ResolveToFile(const std::string& target, std::string& outFsPath) const
{
    outFsPath.clear();

    std::string rel;
    const PathReject r = SanitizeUrlPath(target, rel);
    if (r != kPathOk) return r;
    if (root_.empty()) return kPathRejectEscapesRoot;

    const std::string joined = NormalizeWindowsPath(root_ + "\\" + rel);

    // Belt and braces: even though ".." was refused above, prove the result is
    // still under the root before we open anything.
    if (!PathHasPrefixDir(joined, root_)) return kPathRejectEscapesRoot;

    // Third net: a reparse point inside the root pointing out of it. Only
    // enforced when Windows can tell us the true path for BOTH the candidate
    // and the root; when it cannot, we keep the two lexical verdicts above
    // rather than refusing to serve a normal file.
    const std::string trueFile = TrueFilesystemPath(joined);
    if (!trueFile.empty()) {
        const std::string trueRoot = TrueFilesystemPath(root_);
        if (!trueRoot.empty() && !PathHasPrefixDir(trueFile, trueRoot)) {
            return kPathRejectEscapesRoot;
        }
    }

    outFsPath = joined;
    return kPathOk;
}

HttpResponse HttpStatic::Serve(const std::string& method, const std::string& target) const
{
    HttpResponse res;

    // HTTP methods are case-sensitive tokens.
    const bool isGet  = (method == "GET");
    const bool isHead = (method == "HEAD");
    if (!isGet && !isHead) {
        res.status = 405;
        res.reason = "Method Not Allowed";
        res.contentType = "text/plain; charset=utf-8";
        res.body = "405 method not allowed\n";
        res.contentLength = static_cast<long long>(res.body.size());
        res.extraHeaders["Allow"] = "GET, HEAD";
        return res;
    }
    res.headOnly = isHead;

    std::string fsPath;
    const PathReject r = ResolveToFile(target, fsPath);
    if (r != kPathOk) {
        res.status = HttpStatusForReject(r);
        res.reason = (res.status == 400) ? "Bad Request" : "Forbidden";
        res.contentType = "text/plain; charset=utf-8";
        // Report the class of refusal, never the path we computed.
        res.body = std::string(res.status == 400 ? "400 bad request (" : "403 forbidden (")
                 + PathRejectName(r) + ")\n";
        res.contentLength = static_cast<long long>(res.body.size());
        if (res.headOnly) res.body.clear();
        return res;
    }

    const DWORD attr = GetFileAttributesA(fsPath.c_str());
    const bool missing = (attr == INVALID_FILE_ATTRIBUTES);
    const bool isDir   = (!missing && (attr & FILE_ATTRIBUTE_DIRECTORY) != 0);
    if (missing || isDir) {  // no directory listings, ever
        res.status = 404;
        res.reason = "Not Found";
        res.contentType = "text/plain; charset=utf-8";
        res.body = "404 not found\n";
        res.contentLength = static_cast<long long>(res.body.size());
        if (res.headOnly) res.body.clear();
        return res;
    }

    std::ifstream in(fsPath.c_str(), std::ios::binary);
    if (!in) {
        res.status = 500;
        res.reason = "Internal Server Error";
        res.contentType = "text/plain; charset=utf-8";
        res.body = "500 cannot open file\n";
        res.contentLength = static_cast<long long>(res.body.size());
        if (res.headOnly) res.body.clear();
        return res;
    }

    in.seekg(0, std::ios::end);
    const std::streamoff sz = in.tellg();
    in.seekg(0, std::ios::beg);
    if (sz < 0 || static_cast<long long>(sz) > kMaxFileBytes) {
        res.status = 500;
        res.reason = "Internal Server Error";
        res.contentType = "text/plain; charset=utf-8";
        res.body = "500 file too large\n";
        res.contentLength = static_cast<long long>(res.body.size());
        if (res.headOnly) res.body.clear();
        return res;
    }

    std::string data;
    data.resize(static_cast<size_t>(sz));
    if (sz > 0) in.read(&data[0], sz);
    if (in.bad()) {
        res.status = 500;
        res.reason = "Internal Server Error";
        res.contentType = "text/plain; charset=utf-8";
        res.body = "500 read error\n";
        res.contentLength = static_cast<long long>(res.body.size());
        if (res.headOnly) res.body.clear();
        return res;
    }

    res.status = 200;
    res.reason = "OK";
    res.contentType = MimeTypeForPath(fsPath);
    res.contentLength = static_cast<long long>(data.size());
    if (!res.headOnly) res.body.swap(data);
    return res;
}

}  // namespace webbridge
