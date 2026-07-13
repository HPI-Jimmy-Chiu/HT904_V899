// =============================================================================
//  tests/test_serversocket.cpp -- vclcompat/ServerSocket Sim-mode acceptance
//  test (TServerSocket / TServerWinSocket).
//
//  AI(W906-ServerSocket) 20260713: new file.
//
//  Exercises the new TServerSocket-family shim entirely in Sim mode (no real
//  network I/O -- same posture as test_myplc_modbus.cpp / test_clientsocket_ext.cpp):
//    [1] Defaults + Open()/Close() (Active proxy) side effects.
//    [2] Single-connection accept: ActiveConnections/Connections[] bookkeeping,
//        OnClientConnect firing with the right Sender/Socket.
//    [3] Multi-connection accept: independent slots, per-connection identity
//        (Handle/SocketHandle/LocalAddress/LocalPort/RemotePort).
//    [4] SendBuf via Connections[i] -- the exact golden call shape cited in
//        both SECSGEM/uHGemEquipment.cpp:2010 and Automation/automation.cpp:638/845.
//    [5] The full golden SendLocalData() guard-ladder shape (uHGemEquipment.cpp
//        ~1985-2091): ActiveConnections>0 && Connections[0]->Connected &&
//        ActiveConnections==1 -> SendBuf.
//    [6] OnClientRead firing per-connection (data pushed to one connection
//        does not fire/leak into another's queue) -- the shared-handler
//        scenario (clientGemRead wired to both OnRead and OnClientRead).
//    [7] Disconnect semantics: SimDropConnection() bookkeeping + event, and
//        Close() dropping every live connection at once.
//    [8] OnClientError is callable with the golden signature (Sender, Socket,
//        TErrorEvent, int& ErrorCode) -- srvGemClientError-shaped body.
//
//  LIMITATION (documented, same posture as ClientSocket.h's own tests):
//  Real-mode (WinSock2 listen/accept) is implemented + syntax/link-checked
//  but NOT exercised here -- a live-network test would be environment-
//  dependent and is out of scope for an offline CI-safe suite.
// =============================================================================
#include "vclcompat/ServerSocket.h"

#include <cstdio>
#include <cstring>

// --------------------------------------------------------------------------
//  Tiny PASS / FAIL harness (same style as the other vclcompat tests).
// --------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                   \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ===========================================================================
//  [1] Defaults + Open()/Close() (Active proxy)
// ===========================================================================
static void test_defaults_and_open_close()
{
    printf("\n[1] TServerSocket defaults + Open()/Close()\n");

    TServerSocket srv(0);
    CHECK(srv.IsSimMode() == true, "defaults to Sim mode");
    CHECK(srv.IsActiveNow() == false, "not active before Active=true");
    CHECK(srv.Socket != 0, "Socket sub-object always non-null");
    CHECK(srv.Socket->ActiveConnections == 0, "ActiveConnections starts at 0");
    CHECK(srv.Socket->Connections.empty(), "Connections starts empty");

    srv.Port = 6000;   // golden: srvGem .dfm Port = 6000
    CHECK(srv.Port == 6000, "Port round-trips (golden srvGem default)");

    srv.Active = true;
    CHECK(srv.IsActiveNow() == true, "Active=true -> listening (Sim)");
    CHECK(static_cast<bool>(srv.Active) == true, "Active reads back true");

    srv.Active = false;
    CHECK(srv.IsActiveNow() == false, "Active=false -> stopped listening");

    // Open()/Close() aliases (golden: srvGem->Open()/srvGem->Close(),
    // OLPServer->Open()).
    srv.Open();
    CHECK(srv.IsActiveNow() == true, "Open() reaches the same active state as Active=true");
    srv.Close();
    CHECK(srv.IsActiveNow() == false, "Close() reaches the same inactive state as Active=false");
}

// ===========================================================================
//  [2] Single-connection accept
// ===========================================================================
struct ConnEvents {
    int connectCount, disconnectCount, readCount, errorCount;
    TObject* lastSender;
    TCustomWinSocket* lastSocket;
    ConnEvents() : connectCount(0), disconnectCount(0), readCount(0), errorCount(0),
                   lastSender(0), lastSocket(0) {}
};
static ConnEvents g_ev;

static void resetEvents() { g_ev = ConnEvents(); }

static void wireEvents(TServerSocket& srv)
{
    srv.OnClientConnect = [](TObject* Sender, TCustomWinSocket* Socket) {
        g_ev.connectCount++; g_ev.lastSender = Sender; g_ev.lastSocket = Socket;
    };
    srv.OnClientDisconnect = [](TObject* Sender, TCustomWinSocket* Socket) {
        g_ev.disconnectCount++; g_ev.lastSender = Sender; g_ev.lastSocket = Socket;
    };
    srv.OnClientRead = [](TObject* Sender, TCustomWinSocket* Socket) {
        g_ev.readCount++; g_ev.lastSender = Sender; g_ev.lastSocket = Socket;
    };
    srv.OnClientError = [](TObject* /*Sender*/, TCustomWinSocket* /*Socket*/,
                           Scktcomp::TErrorEvent /*ErrorEvent*/, int& ErrorCode) {
        g_ev.errorCount++;
        ErrorCode = 0;   // golden srvGemClientError shape: clears ErrorCode
    };
}

static void test_single_connection_accept()
{
    printf("\n[2] Single-connection accept: bookkeeping + OnClientConnect\n");

    TServerSocket srv(0);
    srv.Port = 6000;
    wireEvents(srv);
    resetEvents();
    srv.Active = true;

    TCustomWinSocket* conn = srv.SimAcceptConnection("192.168.1.50", 54321);
    CHECK(conn != 0, "SimAcceptConnection returns a non-null connection");
    CHECK(srv.Socket->ActiveConnections == 1, "ActiveConnections==1 after one accept (golden guard)");
    CHECK(srv.Socket->Connections.size() == 1, "Connections has exactly one entry");
    CHECK(srv.Socket->Connections[0] == conn, "Connections[0] is the accepted connection (golden subscript shape)");
    CHECK(conn->Connected == true, "accepted connection reports Connected==true (golden Connections[0]->Connected)");
    CHECK(g_ev.connectCount == 1, "OnClientConnect fired exactly once");
    CHECK(g_ev.lastSender == static_cast<TObject*>(&srv), "OnClientConnect Sender == the TServerSocket itself");
    CHECK(g_ev.lastSocket == conn, "OnClientConnect Socket param == the newly accepted connection");
}

// ===========================================================================
//  [3] Multi-connection accept: independent identity per slot
// ===========================================================================
static void test_multi_connection_accept()
{
    printf("\n[3] Multi-connection accept: independent per-connection identity\n");

    TServerSocket srv(0);
    srv.Port = 6000;
    wireEvents(srv);
    resetEvents();
    srv.Active = true;

    TCustomWinSocket* a = srv.SimAcceptConnection("10.0.0.1", 1111);
    TCustomWinSocket* b = srv.SimAcceptConnection("10.0.0.2", 2222);

    CHECK(srv.Socket->ActiveConnections == 2, "ActiveConnections==2 after two accepts");
    CHECK(srv.Socket->Connections.size() == 2, "Connections has two entries");
    CHECK(srv.Socket->Connections[0] == a, "Connections[0] is the first-accepted connection");
    CHECK(srv.Socket->Connections[1] == b, "Connections[1] is the second-accepted connection");
    CHECK(a != b, "the two connection objects are distinct");
    CHECK(g_ev.connectCount == 2, "OnClientConnect fired once per accept (total 2)");

    // Per-connection identity fields (golden citations: Handle/SocketHandle
    // automation.cpp:636/824; LocalAddress/LocalPort uHGemEquipment.cpp:6818/6822).
    CHECK(a->Handle == a->SocketHandle, "Handle == SocketHandle for connection A (golden treats them as one identity)");
    CHECK(b->Handle == b->SocketHandle, "Handle == SocketHandle for connection B");
    CHECK(a->Handle != b->Handle, "distinct connections get distinct Handle/SocketHandle identities");
    CHECK(a->LocalPort == 6000, "A's LocalPort == the server's own Port (golden Socket->LocalPort read)");
    CHECK(b->LocalPort == 6000, "B's LocalPort == the server's own Port");
    CHECK(a->RemotePort == 1111, "A's RemotePort == the seeded peer port");
    CHECK(b->RemotePort == 2222, "B's RemotePort == the seeded peer port");
    CHECK(a->LocalAddress == AnsiString("10.0.0.1"), "A's LocalAddress round-trips the seeded value");
    CHECK(b->LocalAddress == AnsiString("10.0.0.2"), "B's LocalAddress round-trips the seeded value");

    // automation.cpp:824/829 shape: scan Connections[] by SocketHandle to find
    // "the" connection a reply belongs to.
    int iHandle = b->SocketHandle;
    int foundIndex = -1;
    for (int i = 0; i < srv.Socket->ActiveConnections; ++i)
    {
        if (srv.Socket->Connections[i]->SocketHandle == iHandle) { foundIndex = i; break; }
    }
    CHECK(foundIndex == 1, "automation.cpp-shaped SocketHandle scan finds connection B at index 1");
}

// ===========================================================================
//  [4] SendBuf via Connections[i] (golden call shape)
// ===========================================================================
static void test_sendbuf_via_connections()
{
    printf("\n[4] SendBuf via Connections[i] (golden call shape)\n");

    TServerSocket srv(0);
    srv.Port = 5001;   // golden: OLPServer .dfm Port = 5001
    srv.Active = true;

    TCustomWinSocket* conn = srv.SimAcceptConnection();

    // automation.cpp:638/845 shape: OLPServer->Socket->Connections[i]->SendBuf(...)
    const char msg[] = "HELLO,SERVER";
    int sent = srv.Socket->Connections[0]->SendBuf(const_cast<char*>(msg),
                                                     static_cast<int>(sizeof(msg) - 1));
    CHECK(sent == static_cast<int>(sizeof(msg) - 1), "SendBuf reports the exact byte count sent");

    const std::vector<char>& tx = conn->SimTxBuffer();
    CHECK(tx.size() == sizeof(msg) - 1, "sim tx sink captured the exact byte count");
    CHECK(std::memcmp(&tx[0], msg, sizeof(msg) - 1) == 0, "sim tx sink content matches exactly");
}

// ===========================================================================
//  [5] Full golden SendLocalData() guard-ladder shape
//      (uHGemEquipment.cpp ~1985-2091, bUseClientSocket==false branch)
// ===========================================================================
static void test_golden_sendlocaldata_shape()
{
    printf("\n[5] Golden THGem::SendLocalData() guard-ladder shape\n");

    TServerSocket srvGem(0);
    srvGem.Port = 6000;
    bool bServoSocketConnect = false;

    unsigned char LocalBuffer[4] = { 0x01, 0x02, 0x03, 0x04 };
    int LocalLength_4 = 4;

    // --- replicate golden's exact nested guard shape, verbatim structure ---
    if (srvGem.Active == true && bServoSocketConnect == true)
    {
        CHECK(false, "unreachable before Open()/connect -- guard must short-circuit");
    }
    CHECK(true, "guard short-circuits safely before Open() (Active==false)");

    srvGem.Active = true;
    bServoSocketConnect = true;
    TCustomWinSocket* conn = srvGem.SimAcceptConnection();
    (void)conn;

    bool sentThisPass = false;
    if (srvGem.Active == true && bServoSocketConnect == true)
    {
        if (srvGem.Socket->ActiveConnections > 0)
        {
            if (srvGem.Socket->Connections[0]->Connected)
            {
                if (srvGem.Socket->ActiveConnections == 1)
                {
                    srvGem.Socket->Connections[0]->SendBuf(LocalBuffer, LocalLength_4);
                    sentThisPass = true;
                }
            }
        }
        else
        {
            bServoSocketConnect = false;
            srvGem.Close();
            srvGem.Open();
        }
    }
    CHECK(sentThisPass, "golden guard ladder reaches SendBuf when exactly one Connected peer exists");
    CHECK(conn->SimTxBuffer().size() == 4, "the 4-byte LocalBuffer was captured by the sim tx sink");

    // Second peer connects -> golden's ActiveConnections==1 guard now fails
    // (matches golden: it deliberately does NOT send when >1 are connected).
    srvGem.SimAcceptConnection();
    bool sentSecondPass = false;
    if (srvGem.Active == true && bServoSocketConnect == true)
    {
        if (srvGem.Socket->ActiveConnections > 0)
        {
            if (srvGem.Socket->Connections[0]->Connected)
            {
                if (srvGem.Socket->ActiveConnections == 1)
                {
                    srvGem.Socket->Connections[0]->SendBuf(LocalBuffer, LocalLength_4);
                    sentSecondPass = true;
                }
            }
        }
    }
    CHECK(sentSecondPass == false, "golden guard ladder withholds SendBuf once a 2nd peer connects (ActiveConnections==1 fails)");
}

// ===========================================================================
//  [6] OnClientRead per-connection isolation (shared-handler scenario)
// ===========================================================================
static void test_onclientread_isolation()
{
    printf("\n[6] OnClientRead firing + per-connection isolation\n");

    TServerSocket srv(0);
    srv.Port = 6000;
    wireEvents(srv);
    resetEvents();
    srv.Active = true;

    TCustomWinSocket* a = srv.SimAcceptConnection("10.0.0.1", 1111);
    TCustomWinSocket* b = srv.SimAcceptConnection("10.0.0.2", 2222);

    CHECK(a->ReceiveLength() == 0, "A has nothing queued before any push");
    CHECK(b->ReceiveLength() == 0, "B has nothing queued before any push");

    const char payload[] = "S6F11";
    b->SimPushReceive(payload, static_cast<int>(sizeof(payload) - 1));

    CHECK(g_ev.readCount == 1, "OnClientRead fired exactly once");
    CHECK(g_ev.lastSender == static_cast<TObject*>(&srv), "OnClientRead Sender == the TServerSocket itself");
    CHECK(g_ev.lastSocket == b, "OnClientRead Socket param == the connection that actually received data (B)");
    CHECK(a->ReceiveLength() == 0, "A's queue is untouched by data pushed to B (per-connection isolation)");
    CHECK(b->ReceiveLength() == static_cast<int>(sizeof(payload) - 1), "B's queue holds exactly the pushed bytes");

    AnsiString drained = b->ReceiveText();
    CHECK(drained.Length() == static_cast<int>(sizeof(payload) - 1), "ReceiveText drains B's exact byte count");
    CHECK(std::memcmp(drained.c_str(), payload, sizeof(payload) - 1) == 0, "drained content matches exactly");
}

// ===========================================================================
//  [7] Disconnect semantics: SimDropConnection + Close() drops all
// ===========================================================================
static void test_disconnect_semantics()
{
    printf("\n[7] Disconnect semantics: SimDropConnection + Close()\n");

    TServerSocket srv(0);
    srv.Port = 6000;
    wireEvents(srv);
    resetEvents();
    srv.Active = true;

    TCustomWinSocket* a = srv.SimAcceptConnection();
    TCustomWinSocket* b = srv.SimAcceptConnection();
    CHECK(srv.Socket->ActiveConnections == 2, "two live connections before drop");

    srv.SimDropConnection(a);
    CHECK(a->Connected == false, "dropped connection reports Connected==false");
    CHECK(srv.Socket->ActiveConnections == 1, "ActiveConnections decrements after drop");
    CHECK(srv.Socket->Connections.size() == 1, "Connections shrinks after drop");
    CHECK(srv.Socket->Connections[0] == b, "remaining connection is the one NOT dropped");
    CHECK(g_ev.disconnectCount == 1, "OnClientDisconnect fired exactly once");
    CHECK(g_ev.lastSocket == a, "OnClientDisconnect Socket param == the dropped connection");

    // Dropping an already-dropped connection is a harmless no-op (idempotent,
    // matches DoClose_/Disconnect's existing idempotency idiom elsewhere in
    // this vclcompat layer).
    int disconnectCountBefore = g_ev.disconnectCount;
    srv.SimDropConnection(a);
    CHECK(g_ev.disconnectCount == disconnectCountBefore, "re-dropping an already-dropped connection does not re-fire OnClientDisconnect");

    // Close() drops every remaining live connection at once.
    resetEvents();
    srv.Close();
    CHECK(srv.IsActiveNow() == false, "server reports inactive after Close()");
    CHECK(srv.Socket->ActiveConnections == 0, "ActiveConnections==0 after Close() drops all");
    CHECK(srv.Socket->Connections.empty(), "Connections empty after Close()");
    CHECK(g_ev.disconnectCount == 1, "Close() fired OnClientDisconnect for the one remaining connection");
    CHECK(b->Connected == false, "the connection Close() dropped reports Connected==false");
}

// ===========================================================================
//  [8] OnClientError callable with the golden signature
// ===========================================================================
static void test_onclienterror_signature()
{
    printf("\n[8] OnClientError callable with the golden (Sender, Socket, TErrorEvent, int&) signature\n");

    TServerSocket srv(0);
    wireEvents(srv);
    resetEvents();

    TCustomWinSocket dummy;
    int errCode = 1234;
    // golden srvGemClientError body shape: reads ErrorCode, clears it, closes.
    if (srv.OnClientError)
        srv.OnClientError(&srv, &dummy, Scktcomp::eeConnect, errCode);

    CHECK(g_ev.errorCount == 1, "OnClientError handler invoked");
    CHECK(errCode == 0, "handler clears ErrorCode by reference (golden srvGemClientError shape)");
}

// ===========================================================================
int main()
{
    printf("=============================================================\n");
    printf(" test_serversocket : TServerSocket / TServerWinSocket (Sim mode)\n");
    printf("=============================================================\n");

    test_defaults_and_open_close();
    test_single_connection_accept();
    test_multi_connection_accept();
    test_sendbuf_via_connections();
    test_golden_sendlocaldata_shape();
    test_onclientread_isolation();
    test_disconnect_semantics();
    test_onclienterror_signature();

    printf("\n-------------------------------------------------------------\n");
    printf(" RESULT: %d passed, %d failed\n", g_pass, g_fail);
    printf("-------------------------------------------------------------\n");
    return (g_fail == 0) ? 0 : 1;
}
