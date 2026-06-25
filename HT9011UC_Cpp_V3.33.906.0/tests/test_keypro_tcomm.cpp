// =============================================================================
//  tests/test_keypro_tcomm.cpp  -- W?-HAL ENABLER acceptance test
//
//  Proves the two HAL "interface-cut" enablers in this batch work OFFLINE with
//  NO DLL and NO serial hardware:
//
//    (1) KeyPro license shim  (Public/HTKeyProShim.{h,cpp})
//        --------------------------------------------------------------------
//        The ONLY KeyPro entry the V906 source calls is KEYPRO_GET_LEVEL(3)
//        at HS_Function.cpp:4933 (TFormHS::CheckKeyPro); level==1 means "valid
//        dongle".  The shim resolves the export from KeyProDLL.dll at run time
//        (LoadLibrary/GetProcAddress) and, when the DLL is ABSENT (offline /
//        no dongle), returns 1 so the SECS/GEM gate passes.  KeyProDLL.dll is a
//        32-bit OMF Borland lib that is NOT static-linked; there is also NO
//        KeyProDLL.dll anywhere in the C++ target tree, so this test exercises
//        the genuine offline fallback (not a forced compile-time stub).
//
//    (2) TComm serial shim  (vclcompat/Comm.{h,cpp}, namespace Spcomm)
//        --------------------------------------------------------------------
//        Faithful stand-in for the SPComm VCL component (spcomm.hpp:154-277)
//        used by the temp/barcode/EJ1N/RS232-tester code.  In SIM mode it needs
//        no COM port: StartComm() succeeds, WriteCommData() buffers into a tx
//        sink the test reads back, and SimInjectReceive() drives OnReceiveData
//        exactly as the real reader thread would.
//
//  No hardware / no DLL / no Borland-diff are available here (documented
//  limitation); these are functional offline proofs, not bit-for-bit oracles.
// =============================================================================
#include "Public/HTKeyProShim.h"
#include "vclcompat/Comm.h"        // Spcomm::TComm (also reachable via the umbrella)

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#if defined(_WIN32)
#  include <windows.h>             // LoadLibraryA -- to assert the DLL is absent
#endif

// --------------------------------------------------------------------------
//  Tiny PASS / FAIL harness (same style as the other W-series tests)
// --------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                   \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ===========================================================================
//  [1] KeyPro license shim -- offline stub returns 1 (valid dongle)
// ===========================================================================
static void test_keypro_offline_stub()
{
    printf("\n[1] KeyPro shim: KeyPro_GetLevel offline stub\n");

#if defined(_WIN32) && !defined(KEYPRO_STUB)
    // Sanity: confirm the runtime path is genuinely the OFFLINE fallback, i.e.
    // KeyProDLL.dll is NOT reachable from this test's load path.  If a future
    // change drops the DLL next to the test exe this assertion documents it
    // (KeyPro_GetLevel would then return the DLL's real level instead of the
    // stub's 1, and the test author should revisit the expectation).
    HMODULE hDll = ::LoadLibraryA("KeyProDLL.dll");
    CHECK(hDll == 0, "KeyProDLL.dll is absent (genuine offline fallback path)");
    if (hDll != 0) ::FreeLibrary(hDll);
#endif

    // The live call site passes 3 (the SECS/GEM feature index); 1 == valid.
    int lvl3 = KeyPro_GetLevel(3);
    CHECK(lvl3 == 1, "KeyPro_GetLevel(3) == 1  (offline => valid dongle)");

    // Any feature index returns the valid-dongle stub when offline -- the gate
    // in CheckKeyPro only ever compares against 1.
    CHECK(KeyPro_GetLevel(0) == 1, "KeyPro_GetLevel(0) == 1  (offline stub)");
    CHECK(KeyPro_GetLevel(7) == 1, "KeyPro_GetLevel(7) == 1  (offline stub)");

    // Resolution is cached: repeated calls stay stable (no per-call LoadLibrary
    // flapping that could change the answer mid-run).
    CHECK(KeyPro_GetLevel(3) == 1, "KeyPro_GetLevel(3) stable on repeat call");
}

// ===========================================================================
//  [2] TComm shim -- SIM mode tx capture + rx callback
// ===========================================================================

// OnReceiveData closure capture target (filled by the callback below).
struct RxCapture {
    Spcomm::TObject* sender;
    std::vector<char> bytes;
    int               fireCount;
    RxCapture() : sender(0), fireCount(0) {}
};

static RxCapture g_rx;

// Handler matching Spcomm::TReceiveDataEvent
//   void(TObject* Sender, void* Buffer, Word BufferLength)
static void OnRx(Spcomm::TObject* Sender, void* Buffer, Spcomm::Word Len)
{
    g_rx.sender = Sender;
    ++g_rx.fireCount;
    g_rx.bytes.assign(static_cast<char*>(Buffer),
                      static_cast<char*>(Buffer) + Len);
}

static void test_tcomm_sim()
{
    printf("\n[2] TComm shim: SIM mode tx capture + rx callback\n");

    // .dfm-style construction with an owner (here: none).
    Spcomm::TComm comm(0);

    // Set properties exactly as the consumers do BEFORE StartComm (rs232.cpp /
    // MyTempture.cpp etc. assign these fields, then call StartComm()).
    comm.CommName = "\\\\.\\COM1";   // a real port name; sim mode ignores it
    comm.BaudRate = 115200;
    comm.ByteSize = Spcomm::_8;
    comm.Parity   = Spcomm::None;
    comm.StopBits = Spcomm::_1;

    // Force SIM so the test never touches a real \\.\COM1 even if one exists.
    comm.SetSimMode(true);
    CHECK(comm.IsSimMode() == true, "SetSimMode(true) -> IsSimMode()==true");
    CHECK(comm.IsOpen()    == false, "not open before StartComm()");

    // --- StartComm succeeds with no hardware ------------------------------
    comm.StartComm();
    CHECK(comm.IsOpen()    == true, "StartComm() succeeds offline (sim open)");
    CHECK(comm.IsSimMode() == true, "still in sim mode after StartComm()");

    // --- WriteCommData buffers into the tx sink ---------------------------
    // Faithful sig is WriteCommData(char*, Word); use a writable buffer.
    char  tx1[] = "HELLO";            // 5 bytes (excl. NUL)
    bool  w1 = comm.WriteCommData(tx1, (Spcomm::Word)5);
    CHECK(w1 == true, "WriteCommData(\"HELLO\",5) returns true in sim");

    char  tx2[] = "\x02PING\x03";     // 6 bytes incl. STX/ETX framing
    bool  w2 = comm.WriteCommData(tx2, (Spcomm::Word)6);
    CHECK(w2 == true, "WriteCommData(framed,6) returns true in sim");

    // Read the captured bytes back from the sink and verify exact content.
    const std::vector<char>& sink = comm.SimTxBuffer();
    CHECK(sink.size() == 11, "tx sink accumulated 5+6 == 11 bytes");
    bool sinkMatch =
        sink.size() == 11 &&
        std::memcmp(&sink[0], "HELLO", 5) == 0 &&
        std::memcmp(&sink[5], "\x02PING\x03", 6) == 0;
    CHECK(sinkMatch, "tx sink content == \"HELLO\" + framed \"PING\" (exact)");

    // SimClearTx empties the sink (a test can reset between exchanges).
    comm.SimClearTx();
    CHECK(comm.SimTxBuffer().empty(), "SimClearTx() empties the tx sink");

    // A zero-length write is a no-op success; a NULL ptr write fails.
    CHECK(comm.WriteCommData(tx1, 0) == true,  "WriteCommData(_,0) is a no-op success");
    CHECK(comm.WriteCommData(0, 4)   == false, "WriteCommData(NULL,4) fails");
    CHECK(comm.SimTxBuffer().empty(), "no bytes captured by the no-op/NULL writes");

    // --- OnReceiveData callback fires with injected RX --------------------
    g_rx = RxCapture();
    comm.OnReceiveData = &OnRx;        // std::function assignable from a fn ptr

    const char rx[] = { 'A', 'C', 'K', '\r', '\n' };  // 5 bytes
    comm.SimInjectReceive(rx, (Spcomm::Word)5);

    CHECK(g_rx.fireCount == 1,        "SimInjectReceive fires OnReceiveData once");
    CHECK(g_rx.sender == &comm,       "OnReceiveData Sender == the TComm instance");
    CHECK(g_rx.bytes.size() == 5,     "OnReceiveData delivered 5 bytes");
    bool rxMatch = g_rx.bytes.size() == 5 &&
                   std::memcmp(&g_rx.bytes[0], rx, 5) == 0;
    CHECK(rxMatch, "OnReceiveData delivered EXACTLY the injected bytes (ACK\\r\\n)");

    // A second injection accumulates a second fire with fresh content.
    const char rx2[] = { 'N', 'A', 'K' };
    comm.SimInjectReceive(rx2, (Spcomm::Word)3);
    CHECK(g_rx.fireCount == 2,        "second SimInjectReceive fires again");
    CHECK(g_rx.bytes.size() == 3 &&
          std::memcmp(&g_rx.bytes[0], rx2, 3) == 0,
          "second injection delivers \"NAK\"");

    // With NO handler bound, injection is a safe no-op (mirrors the real reader
    // skipping the post when OnReceiveData is unset).
    Spcomm::TComm comm2(0);
    comm2.SetSimMode(true);
    comm2.StartComm();
    comm2.SimInjectReceive(rx, 5);     // OnReceiveData unset -> no crash, no fire
    CHECK(true, "SimInjectReceive with no handler bound is a safe no-op");
    comm2.StopComm();

    // --- StopComm closes; re-StartComm still works ------------------------
    comm.StopComm();
    CHECK(comm.IsOpen() == false, "StopComm() closes the sim port");
    comm.StartComm();
    CHECK(comm.IsOpen() == true,  "StartComm() re-opens after StopComm()");
    comm.StopComm();
}

// ===========================================================================
int main()
{
    printf("=============================================================\n");
    printf(" test_keypro_tcomm : KeyPro license shim + TComm serial shim\n");
    printf("=============================================================\n");

    test_keypro_offline_stub();
    test_tcomm_sim();

    printf("\n-------------------------------------------------------------\n");
    printf(" RESULT: %d passed, %d failed\n", g_pass, g_fail);
    printf("-------------------------------------------------------------\n");
    return (g_fail == 0) ? 0 : 1;
}
