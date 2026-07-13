// =============================================================================
//  tests/test_clientsocket_ext.cpp -- W5-Final ClientSocket extension
//  acceptance test (Tag / Open() / SendText() / ReceiveText()).
//
//  AI(W5-Final-ClientSocketExt) 20260711: new file.
//
//  Exercises the 4 additions made to vclcompat/ClientSocket.h/.cpp for the
//  two Phase-2 consumers in this migration batch (BarCode's 8-CCD glue unit
//  and Automation's TesterTCP unit -- see ClientSocket.h's file-header
//  EXTENSION note for exact golden call-shape citations):
//    [1] TClientSocket::Tag            -- plain int round-trip, default 0,
//                                          discriminates which shared socket
//                                          fired an event (golden pattern:
//                                          `Ptr->Tag` off a cast Sender).
//    [2] TClientSocket::Open()         -- reaches the SAME connected state
//                                          `Active=true` would (same
//                                          OnConnect firing, same
//                                          idempotency), proven by directly
//                                          comparing against the existing
//                                          Active=true behavior already
//                                          covered by test_myplc_modbus.cpp.
//    [3] TCustomWinSocket::SendText()  -- forwards raw bytes to the sim tx
//                                          sink verbatim (no terminator
//                                          added), byte-for-byte, returning
//                                          the sent byte count.
//    [4] TCustomWinSocket::ReceiveText() -- drains whatever SimPushReceive
//                                          queued into an AnsiString,
//                                          content-exact, and leaves the
//                                          queue empty afterward (same
//                                          "drain to date" semantics as
//                                          golden's ReceiveLength()-guarded
//                                          call pattern).
//
//  LIMITATION (documented, same posture as test_myplc_modbus.cpp): SIM mode
//  only. No real network I/O is exercised here (Real-mode WinSock2 path is
//  implemented + syntax/link-checked but environment-dependent, out of scope
//  for an offline CI-safe suite).
// =============================================================================
#include "vclcompat/ClientSocket.h"

#include <cstdio>
#include <cstring>

// --------------------------------------------------------------------------
//  Tiny PASS / FAIL harness (same style as tests/test_myplc_modbus.cpp)
// --------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                   \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ===========================================================================
//  [1] TClientSocket::Tag -- plain round-trip, default 0, discriminates
//      which shared socket fired an event (golden BarCode.cpp pattern).
// ===========================================================================
static void test_tag_roundtrip()
{
    printf("\n[1] TClientSocket::Tag: default + round-trip + event-Sender discrimination\n");

    TClientSocket sockA(0);
    TClientSocket sockB(0);
    CHECK(sockA.Tag == 0, "Tag defaults to 0 (matches real VCL TComponent.Tag default)");
    CHECK(sockB.Tag == 0, "a second instance also defaults to 0 (per-instance, not shared/static)");

    // Golden BarCode.cpp:305 `Ptr->Tag=i;` (.dfm-equivalent per-instance init,
    // 0..7 range for the 8 "Bottom" sockets).
    sockA.Tag = 3;
    sockB.Tag = 7;
    CHECK(sockA.Tag == 3, "Tag assignment round-trips (sockA=3)");
    CHECK(sockB.Tag == 7, "Tag assignment round-trips (sockB=7), independent of sockA");

    // Golden event handlers cast Sender back to TClientSocket* and read
    // ->Tag to discriminate which of several shared sockets fired
    // (BarCode.cpp:3140/:3162/:3215 `Ptr=(TClientSocket*)Sender; ... Ptr->Tag`).
    int observedTag = -1;
    TObject* observedSender = 0;
    sockA.OnConnect = [&](TObject* Sender, TCustomWinSocket*) {
        TClientSocket* Ptr = static_cast<TClientSocket*>(Sender);
        observedSender = Sender;
        observedTag = Ptr->Tag;
    };
    sockA.Active = true;
    CHECK(observedSender == &sockA, "OnConnect Sender casts back to the exact TClientSocket instance");
    CHECK(observedTag == 3, "handler reads Ptr->Tag==3 off the cast Sender (BarCode discrimination idiom)");
    sockA.Active = false;
}

// ===========================================================================
//  [2] TClientSocket::Open() -- reaches the same connected state Active=true
//      would (same OnConnect firing, same idempotency).
// ===========================================================================
static void test_open_matches_active_true()
{
    printf("\n[2] TClientSocket::Open(): same connect path/state as Active=true\n");

    TClientSocket sock(0);
    int connectCount = 0;
    sock.OnConnect = [&](TObject*, TCustomWinSocket*) { connectCount++; };

    CHECK(sock.IsActiveNow() == false, "not active before Open()");
    sock.Address = "127.0.0.1";
    sock.Port    = 7016;

    // Golden: Interface/TesterTCP.cpp:228 `ClientSocket_TCPIP->Open();`
    // (wrapped in try/catch by the caller -- exercised here too, to prove
    // the shim's no-throw policy makes that wrapping harmless, not required).
    try
    {
        sock.Open();
    }
    catch (...)
    {
        CHECK(false, "Open() must not throw (shim's Real-mode-failure policy is no-throw)");
    }

    CHECK(sock.IsActiveNow() == true, "Open() reaches the connected state (Sim, synchronous)");
    CHECK((bool)sock.Active == true, "Active reads back true after Open() (same underlying state)");
    CHECK(connectCount == 1, "Open() fires OnConnect exactly once, same as Active=true would");

    // Idempotent, exactly like Active=true while already open (DoConnect_()
    // is the SAME function both paths call -- not a second implementation).
    sock.Open();
    CHECK(connectCount == 1, "a second Open() while already connected is idempotent (no re-fire)");

    sock.Active = false;
    CHECK(sock.IsActiveNow() == false, "Active=false disconnects (cross-checking the two APIs interoperate)");

    // And the reverse direction: Active=true after Open()/Active=false cycle
    // still works identically (both APIs drive the one DoConnect_() path).
    sock.Active = true;
    CHECK(connectCount == 2, "Active=true reconnects after Open()-then-close, same OnConnect firing");
    sock.Close();
}

// ===========================================================================
//  [3]+[4] TCustomWinSocket::SendText() / ReceiveText() -- Sim round trip.
// ===========================================================================
static void test_sendtext_receivetext_roundtrip()
{
    printf("\n[3+4] TCustomWinSocket::SendText()/ReceiveText(): Sim round trip\n");

    TClientSocket sock(0);
    sock.Active = true;
    CHECK(sock.IsActiveNow() == true, "connected (Sim) before exercising SendText/ReceiveText");

    // --- SendText: forwards raw bytes verbatim, no terminator added ---------
    // Golden callers append their OWN terminator before calling
    // (Interface/TesterTCP.cpp:271 `Socket->SendText(Msg2+"\r\n")`) --
    // SendText itself must not add a second one.
    AnsiString msg = "HELLO";
    msg += "\r\n";   // caller-supplied terminator, per golden idiom
    int sent = sock.Socket->SendText(msg);
    CHECK(sent == msg.Length(), "SendText returns the byte count sent (real-VCL-faithful, like SendBuf)");

    const std::vector<char>& tx = sock.Socket->SimTxBuffer();
    CHECK(tx.size() == static_cast<size_t>(msg.Length()), "sim tx sink captured the exact byte count");
    CHECK(std::memcmp(&tx[0], msg.c_str(), static_cast<size_t>(msg.Length())) == 0,
          "sim tx sink content == \"HELLO\\r\\n\" exactly (no extra terminator appended)");
    sock.Socket->SimClearTx();

    // --- ReceiveText: drains whatever SimPushReceive queued -----------------
    // Golden pattern (Interface/TesterTCP.cpp:333-336):
    //   if (Socket->ReceiveLength() > 0) EthernetBuffer = Socket->ReceiveText();
    CHECK(sock.Socket->ReceiveLength() == 0, "no bytes queued before SimPushReceive");

    const char rxData[] = "REPLY,OK\r\n";
    int rxLen = static_cast<int>(sizeof(rxData) - 1);   // exclude the trailing NUL
    sock.Socket->SimPushReceive(rxData, rxLen);
    CHECK(sock.Socket->ReceiveLength() == rxLen, "ReceiveLength() reports the exact queued byte count");

    AnsiString received = sock.Socket->ReceiveText();
    CHECK(received.Length() == rxLen, "ReceiveText() returns the exact byte count queued");
    CHECK(std::memcmp(received.c_str(), rxData, static_cast<size_t>(rxLen)) == 0,
          "ReceiveText() content == \"REPLY,OK\\r\\n\" exactly");
    CHECK(sock.Socket->ReceiveLength() == 0, "queue drained after ReceiveText() (same drain semantics as ReceiveBuf)");

    // ReceiveText() on an empty queue returns an empty AnsiString, not garbage.
    AnsiString empty = sock.Socket->ReceiveText();
    CHECK(empty.Length() == 0, "ReceiveText() on an empty queue returns an empty AnsiString");

    // Embedded-byte safety: bytes are passed through untouched (Big5-safe),
    // including an embedded NUL byte and non-ASCII (Big5 lead-byte range).
    const char rawBytes[] = { 'A', '\0', (char)0xA4, (char)0x40, 'Z' };   // 0xA440 = Big5 lead/trail pair range
    sock.Socket->SimPushReceive(rawBytes, static_cast<int>(sizeof(rawBytes)));
    AnsiString rawBack = sock.Socket->ReceiveText();
    CHECK(rawBack.Length() == static_cast<int>(sizeof(rawBytes)),
          "ReceiveText() preserves length through an embedded NUL byte (not NUL-terminated truncation)");
    CHECK(std::memcmp(rawBack.c_str(), rawBytes, sizeof(rawBytes)) == 0,
          "ReceiveText() preserves raw bytes exactly, including embedded NUL + Big5 lead-byte range");

    sock.Active = false;
}

// ===========================================================================
int main()
{
    printf("=============================================================\n");
    printf(" test_clientsocket_ext : Tag / Open() / SendText() / ReceiveText()\n");
    printf("=============================================================\n");

    test_tag_roundtrip();
    test_open_matches_active_true();
    test_sendtext_receivetext_roundtrip();

    printf("\n-------------------------------------------------------------\n");
    printf(" RESULT: %d passed, %d failed\n", g_pass, g_fail);
    printf("-------------------------------------------------------------\n");
    return (g_fail == 0) ? 0 : 1;
}
