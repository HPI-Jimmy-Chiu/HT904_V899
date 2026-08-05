// =============================================================================
//  WebBridge/HttpStatic.h -- static file serving for the web HMI bridge.
//
//  Serves D:\HT9045\web (or any document root) over HTTP/1.1 for the browser
//  HMI described in D:\HT9045\web\docs\ARCHITECTURE.md section 7 step 4.
//
//  THE ONE LINE THAT DECIDES WHETHER THE UI LOADS AT ALL
//  -----------------------------------------------------
//  web/index.html loads js/main.js as an ES module. A browser REFUSES to
//  execute a module whose Content-Type is not a JavaScript MIME type, so
//  MimeTypeForPath() must return "text/javascript" for ".js". Everything else
//  in this file is secondary to that.
//
//  SECURITY POSTURE
//  ----------------
//  This serves files off a production machine PC, so the path handling is
//  deliberately paranoid and deliberately SEPARATE from the socket plumbing
//  so that it can be unit-tested with attack strings and no server running:
//
//      SanitizeUrlPath()  raw request target -> safe relative path, or reject
//      HttpStatic::Serve() safe relative path -> bytes on the wire
//
//  Rejected outright: "..", absolute paths, drive letters ("c:/"), UNC
//  ("//host/share"), backslashes, NUL bytes, other control characters, the
//  Windows-illegal set < > : " | * , malformed percent-escapes, and anything
//  that still lexically resolves outside the root after normalisation.
//  Percent-decoding happens BEFORE those checks, exactly once, so "%2e%2e%2f"
//  is caught as ".." and "%252e%252e%252f" stays a literal (unfound) filename
//  rather than being decoded twice into a traversal.
//
//  Layer independence: nothing here includes <vcl.h>, vclcompat, winsock, or
//  any machine header. Pure std C++17 plus <windows.h> inside the .cpp only.
// =============================================================================
#ifndef WEBBRIDGE_HTTPSTATIC_H
#define WEBBRIDGE_HTTPSTATIC_H

#include <map>
#include <string>

namespace webbridge {

// -----------------------------------------------------------------------------
//  Why a request path was refused. kPathOk == 0, so `if (r)` means "rejected".
// -----------------------------------------------------------------------------
enum PathReject {
    kPathOk = 0,
    kPathRejectEmpty,          // ""                       -> 400
    kPathRejectNotAbsolute,    // "js/main.js", "c:\x"      -> 400
    kPathRejectBadPercent,     // "%2", "%zz"               -> 400
    kPathRejectNulByte,        // "%00"                     -> 403
    kPathRejectControlChar,    // raw \r \n \t etc.         -> 403
    kPathRejectBackslash,      // "..\\.."                  -> 403
    kPathRejectDotDot,         // "../", "%2e%2e%2f"        -> 403
    kPathRejectDriveLetter,    // "/c:/windows/..."         -> 403
    kPathRejectUnc,            // "//attacker/share"        -> 403
    kPathRejectIllegalChar,    // < > : " | *               -> 403
    kPathRejectEscapesRoot     // survived the above, still outside root -> 403
};

// Stable short name for logs and test failure messages.
const char* PathRejectName(PathReject r);

// HTTP status this library answers a given rejection with.
// 400 = the client sent nonsense; 403 = the client sent an attack.
int HttpStatusForReject(PathReject r);

// -----------------------------------------------------------------------------
//  MIME
// -----------------------------------------------------------------------------
//  Extension match is case-insensitive. Unknown extensions get
//  "application/octet-stream" -- never text/html, which would let an uploaded
//  file be served as markup.
//
//  ".js" and ".mjs" MUST map to "text/javascript" (see header comment).
const char* MimeTypeForPath(const std::string& path);

// -----------------------------------------------------------------------------
//  Percent-decoding. Decodes exactly once. Returns false on a malformed escape
//  ("%", "%2", "%zz"); '+' is NOT treated as space (that is form encoding, not
//  path encoding). A decoded NUL is preserved in `out` so the caller's own NUL
//  check can reject it -- it is not silently dropped.
// -----------------------------------------------------------------------------
bool PercentDecode(const std::string& in, std::string& out);

// -----------------------------------------------------------------------------
//  Raw request target ("/js/main.js?v=3#frag") -> safe relative path
//  ("js/main.js"). Strips query and fragment, percent-decodes, then applies
//  every rejection rule above. "/" and any path ending in "/" resolve to
//  index.html. On rejection `outRel` is cleared and the reason is returned.
// -----------------------------------------------------------------------------
PathReject SanitizeUrlPath(const std::string& target, std::string& outRel);

// Lexical (no disk access) Windows path normalisation: forward slashes become
// backslashes, duplicate separators collapse, "." segments vanish and ".."
// segments pop. A leading "\\\\" (UNC) is preserved. Exposed for the test.
std::string NormalizeWindowsPath(const std::string& path);

// -----------------------------------------------------------------------------
//  A response ready to be turned into bytes.
// -----------------------------------------------------------------------------
struct HttpResponse {
    int         status;         // 200 400 403 404 405 500
    std::string reason;         // "OK", "Not Found", ...
    std::string contentType;    // "" -> header omitted
    std::string body;           // payload; empty when headOnly
    bool        headOnly;       // HEAD: emit Content-Length, omit the body
    long long   contentLength;  // what to advertise (body size, even for HEAD)
    std::map<std::string, std::string> extraHeaders;  // e.g. Allow:

    HttpResponse();

    // Full HTTP/1.1 response, headers + CRLFCRLF + body. Always
    // "Connection: close" -- this server answers one request per TCP
    // connection and then closes, which keeps the select() loop trivial.
    std::string ToWire() const;
};

// -----------------------------------------------------------------------------
//  The server-side file mapper. Const and thread-safe once the root is set;
//  the socket thread calls Serve() concurrently with nothing else.
// -----------------------------------------------------------------------------
class HttpStatic {
public:
    HttpStatic();
    explicit HttpStatic(const std::string& documentRoot);

    void SetDocumentRoot(const std::string& documentRoot);
    const std::string& DocumentRoot() const { return root_; }

    // method: "GET" or "HEAD"; anything else -> 405 + Allow: GET, HEAD.
    // target: raw request target straight off the request line.
    HttpResponse Serve(const std::string& method, const std::string& target) const;

    // Target -> absolute filesystem path, without touching the disk except for
    // the final "did a junction move us out of the root" check. Returns
    // kPathOk and fills outFsPath, or a rejection reason.
    PathReject ResolveToFile(const std::string& target, std::string& outFsPath) const;

private:
    std::string root_;  // normalised, backslashes, no trailing separator
};

}  // namespace webbridge

#endif  // WEBBRIDGE_HTTPSTATIC_H
