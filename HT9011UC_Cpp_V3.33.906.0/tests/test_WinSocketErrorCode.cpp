// ===========================================================================
//  tests/test_WinSocketErrorCode.cpp
//  Correctness tests for translated Public/WinSocketErrorCode (W1 leaf).
//
//  Oracle: BCB6 golden source
//  (HT9011UC_Code_V3.33.906.0_20260618/Public/WinSocketErrorCode.cpp:12-314).
//  GetErrorMsg(TObject* Sender, int code) maps a Winsock error code to its
//  descriptive string.  Sender is unused (the BCB6 cast was commented out).
//
//  We assert:
//    * the numeric-code cases (6/8/87/995/996/997) begin with their exact
//      "Socket Error (N)" prefix;
//    * the named WSAE* cases map to the documented decimal Winsock numbers
//      (e.g. WSAEWOULDBLOCK -> "Socket Error (10035) ...");
//    * the QoS named cases map to their 110xx prefixes;
//    * the default branch yields exactly
//      "Socket Error (<code>) Unknown error code." (BCB6 :305).
//    * Sender==NULL is accepted (function ignores it).
//
//  Build: linked against ht9045_public (which links vclcompat) + winsock2
//  constants from <winsock2.h>.
//  Non-zero exit on any failure.
// ===========================================================================

#include "Public/WinSocketErrorCode.h"
#include <winsock2.h>

#include <cstdio>
#include <cstring>
#include <string>

static int g_fail  = 0;
static int g_total = 0;

// Assert that `whole` starts with `prefix`.
static void checkPrefix(const AnsiString& whole, const char* prefix,
                        const char* what, const char* file, int line) {
    ++g_total;
    std::string w(whole.c_str());
    std::string p(prefix);
    if (w.compare(0, p.size(), p) != 0) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : \"%s\" does not start with \"%s\"\n",
                    file, line, what, w.c_str(), p.c_str());
    }
}
#define CHECK_PREFIX(whole, prefix) checkPrefix((whole), (prefix), #whole, __FILE__, __LINE__)

static void checkExact(const AnsiString& got, const char* expect,
                       const char* what, const char* file, int line) {
    ++g_total;
    if (std::string(got.c_str()) != std::string(expect)) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got \"%s\" expected \"%s\"\n",
                    file, line, what, got.c_str(), expect);
    }
}
#define CHECK_EXACT(got, expect) checkExact((got), (expect), #got, __FILE__, __LINE__)

int main()
{
    // Sender is unused; pass NULL (BCB6 commented-out cast).
    TObject* sender = 0;

    // -----------------------------------------------------------------------
    // Numeric (non-WSA-named) cases  (BCB6 :19-46)
    // -----------------------------------------------------------------------
    CHECK_PREFIX(GetErrorMsg(sender, 6),   "Socket Error (6) Specified event object handle is invalid.");
    CHECK_PREFIX(GetErrorMsg(sender, 8),   "Socket Error (8) Insufficient memory available.");
    CHECK_PREFIX(GetErrorMsg(sender, 87),  "Socket Error (87) One or more parameters are invalid.");
    CHECK_PREFIX(GetErrorMsg(sender, 995), "Socket Error (995) Overlapped operation aborted.");
    CHECK_PREFIX(GetErrorMsg(sender, 996), "Socket Error (996) Overlapped I/O event object not in signaled state.");
    CHECK_PREFIX(GetErrorMsg(sender, 997), "Socket Error (997) Overlapped operations will complete later.");

    // -----------------------------------------------------------------------
    // Named WSAE* cases -> documented decimal Winsock error numbers.
    // -----------------------------------------------------------------------
    CHECK_PREFIX(GetErrorMsg(sender, WSAEINTR),         "Socket Error (10004) Interrupted function call.");
    CHECK_PREFIX(GetErrorMsg(sender, WSAEACCES),        "Socket Error (10013) Permission denied.");
    CHECK_PREFIX(GetErrorMsg(sender, WSAEFAULT),        "Socket Error (10014) Bad address.");
    CHECK_PREFIX(GetErrorMsg(sender, WSAEWOULDBLOCK),   "Socket Error (10035) Resource temporarily unavailable.");
    CHECK_PREFIX(GetErrorMsg(sender, WSAENOTSOCK),      "Socket Error (10038) Socket operation on nonsocket.");
    CHECK_PREFIX(GetErrorMsg(sender, WSAEADDRINUSE),    "Socket Error (10048) Address already in use.");
    CHECK_PREFIX(GetErrorMsg(sender, WSAECONNREFUSED),  "Socket Error (10061) Connection refused.");
    CHECK_PREFIX(GetErrorMsg(sender, WSAETIMEDOUT),     "Socket Error (10060) Connection timed out.");
    CHECK_PREFIX(GetErrorMsg(sender, WSAHOST_NOT_FOUND),"Socket Error (11001) Host not found.");

    // -----------------------------------------------------------------------
    // QoS named cases  (BCB6 :233-313)
    // -----------------------------------------------------------------------
    CHECK_PREFIX(GetErrorMsg(sender, WSA_QOS_RECEIVERS),   "Socket Error (11005) QoS receivers.");
    CHECK_PREFIX(GetErrorMsg(sender, WSA_QOS_GENERIC_ERROR), "Socket Error (11015) QoS generic error.");
    CHECK_PREFIX(GetErrorMsg(sender, WSA_QOS_RESERVED_PETYPE), "Socket Error (11031) Reserved policy QoS element type.");

    // -----------------------------------------------------------------------
    // default branch  (BCB6 :305): exact "Socket Error (N) Unknown error code."
    // Use codes guaranteed to be outside every named case.
    // -----------------------------------------------------------------------
    CHECK_EXACT(GetErrorMsg(sender, 1),       "Socket Error (1) Unknown error code.");
    CHECK_EXACT(GetErrorMsg(sender, -999),    "Socket Error (-999) Unknown error code.");
    CHECK_EXACT(GetErrorMsg(sender, 123456),  "Socket Error (123456) Unknown error code.");
    CHECK_EXACT(GetErrorMsg(sender, 0),       "Socket Error (0) Unknown error code.");

    // -----------------------------------------------------------------------
    std::printf("test_WinSocketErrorCode: %d / %d passed\n", g_total - g_fail, g_total);
    return (g_fail == 0) ? 0 : 1;
}
