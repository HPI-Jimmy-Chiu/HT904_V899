// ===========================================================================
//  vclcompat/ClientSocket.cpp
//  Implementation of the Scktcomp::TClientSocket / TCustomWinSocket shim
//  (see ClientSocket.h for the full provenance / Sim-Real policy note).
//
//  AI(W5-MyPLC-Translate) 20260710: new file.
//
//  Include order note: <winsock2.h> is included BEFORE <windows.h> in THIS
//  translation unit (both guarded #if _WIN32) so a legacy <winsock.h> pulled
//  in transitively by windows.h can never precede winsock2.h -- same fix
//  Public/WinSocketErrorCode.cpp already applies (vcl_compat.h's own
//  windows.h include has no WIN32_LEAN_AND_MEAN, by design, per that header's
//  comment about rpcndr.h's `byte` typedef).
// ===========================================================================
#include "vclcompat/ClientSocket.h"

#include <cstring>

#if defined(_WIN32)
#  include <winsock2.h>
#  include <windows.h>
#endif

namespace Scktcomp {

// ---------------------------------------------------------------------------
//  WinSock2 process-wide init (real mode only), lazy + once.
// ---------------------------------------------------------------------------
#if defined(_WIN32)
static bool EnsureWinsockInit_()
{
    static bool s_inited = false;
    static bool s_ok = false;
    if (!s_inited)
    {
        s_inited = true;
        WSADATA wsa;
        s_ok = (::WSAStartup(MAKEWORD(2, 2), &wsa) == 0);
    }
    return s_ok;
}
#endif

// ===========================================================================
//  TCustomWinSocket
// ===========================================================================
struct TCustomWinSocket::Impl {
    TClientSocket* owner;   // back-pointer, set by TClientSocket's ctor; used
                            // by the reader thread to fire owner->OnRead
    bool bSim;
    bool bConnected;

#if defined(_WIN32)
    SOCKET            sock;         // INVALID_SOCKET when not connected
    HANDLE            hReader;      // reader-thread handle (0 when none)
    volatile LONG     bStopReader;  // signal the reader to exit (0/1)
    CRITICAL_SECTION  csRx;         // guards rxQueue
#endif

    std::vector<char> rxQueue;      // bytes received, waiting for ReceiveBuf
    std::vector<char> simTx;        // sim-mode SendBuf capture

    Impl()
        : owner(0)
        , bSim(true)
        , bConnected(false)
#if defined(_WIN32)
        , sock(INVALID_SOCKET)
        , hReader(0)
        , bStopReader(0)
#endif
    {
#if defined(_WIN32)
        ::InitializeCriticalSection(&csRx);
#endif
    }

    ~Impl()
    {
#if defined(_WIN32)
        ::DeleteCriticalSection(&csRx);
#endif
    }
};

TCustomWinSocket::TCustomWinSocket()
    : RemotePort(0)
    , pImpl_(new Impl())
{
}

TCustomWinSocket::~TCustomWinSocket()
{
    delete pImpl_;
    pImpl_ = 0;
}

int TCustomWinSocket::ReceiveLength() const
{
#if defined(_WIN32)
    ::EnterCriticalSection(&pImpl_->csRx);
    int n = static_cast<int>(pImpl_->rxQueue.size());
    ::LeaveCriticalSection(&pImpl_->csRx);
    return n;
#else
    return static_cast<int>(pImpl_->rxQueue.size());
#endif
}

int TCustomWinSocket::ReceiveBuf(void* Buf, int BufSize)
{
    if (Buf == 0 || BufSize <= 0)
        return 0;

#if defined(_WIN32)
    ::EnterCriticalSection(&pImpl_->csRx);
#endif
    int n = static_cast<int>(pImpl_->rxQueue.size());
    if (n > BufSize)
        n = BufSize;
    if (n > 0)
    {
        std::memcpy(Buf, &pImpl_->rxQueue[0], static_cast<size_t>(n));
        pImpl_->rxQueue.erase(pImpl_->rxQueue.begin(), pImpl_->rxQueue.begin() + n);
    }
#if defined(_WIN32)
    ::LeaveCriticalSection(&pImpl_->csRx);
#endif
    return n;
}

int TCustomWinSocket::SendBuf(void* Buf, int BufSize)
{
    if (Buf == 0 || BufSize <= 0)
        return 0;

    if (pImpl_->bSim)
    {
        char* p = static_cast<char*>(Buf);
        pImpl_->simTx.insert(pImpl_->simTx.end(), p, p + BufSize);
        return BufSize;
    }

#if defined(_WIN32)
    if (pImpl_->sock == INVALID_SOCKET)
        return -1;
    int sent = ::send(pImpl_->sock, static_cast<const char*>(Buf), BufSize, 0);
    return sent;
#else
    return -1;
#endif
}

void TCustomWinSocket::Disconnect(int /*Port*/)
{
    // Faithful to golden `Socket->Disconnect(Socket->RemotePort)`
    // (SocketError handler). Port is accepted for call-shape fidelity only.
#if defined(_WIN32)
    if (pImpl_->sock != INVALID_SOCKET)
    {
        ::closesocket(pImpl_->sock);
        pImpl_->sock = INVALID_SOCKET;
    }
#endif
    pImpl_->bConnected = false;
}

void TCustomWinSocket::SimPushReceive(const void* pData, int len)
{
    if (pData == 0 || len <= 0)
        return;

#if defined(_WIN32)
    ::EnterCriticalSection(&pImpl_->csRx);
#endif
    const char* p = static_cast<const char*>(pData);
    pImpl_->rxQueue.insert(pImpl_->rxQueue.end(), p, p + len);
#if defined(_WIN32)
    ::LeaveCriticalSection(&pImpl_->csRx);
#endif

    if (pImpl_->owner && pImpl_->owner->OnRead)
        pImpl_->owner->OnRead(pImpl_->owner, this);
}

const std::vector<char>& TCustomWinSocket::SimTxBuffer() const { return pImpl_->simTx; }
void TCustomWinSocket::SimClearTx() { pImpl_->simTx.clear(); }

// ---------------------------------------------------------------------------
//  Real-mode reader thread (private static member -> can touch pImpl_).
//  Blocks in recv(); on each arrival, enqueues bytes and fires OnRead as a
//  "data available" notice (mirrors golden: the handler pulls the bytes
//  itself via Socket->ReceiveLength()/ReceiveBuf(), it is not handed them).
// ---------------------------------------------------------------------------
unsigned long __stdcall TCustomWinSocket::ReaderProc_(void* param)
{
#if defined(_WIN32)
    TCustomWinSocket* sk = static_cast<TCustomWinSocket*>(param);
    char buf[1024];
    for (;;)
    {
        if (::InterlockedCompareExchange(&sk->pImpl_->bStopReader, 1, 1) != 0)
            break;
        int n = ::recv(sk->pImpl_->sock, buf, static_cast<int>(sizeof(buf)), 0);
        if (::InterlockedCompareExchange(&sk->pImpl_->bStopReader, 1, 1) != 0)
            break;
        if (n > 0)
        {
            ::EnterCriticalSection(&sk->pImpl_->csRx);
            sk->pImpl_->rxQueue.insert(sk->pImpl_->rxQueue.end(), buf, buf + n);
            ::LeaveCriticalSection(&sk->pImpl_->csRx);

            if (sk->pImpl_->owner && sk->pImpl_->owner->OnRead)
                sk->pImpl_->owner->OnRead(sk->pImpl_->owner, sk);
        }
        else
        {
            break;   // n==0 (peer closed) or SOCKET_ERROR: stop the loop
        }
    }
#else
    (void)param;
#endif
    return 0;
}

// ===========================================================================
//  TClientSocket
// ===========================================================================
struct TClientSocket::Impl {
    // The requested Sim/Real policy (composition-root choice via
    // SetSimMode()). Defaults to Sim (true) -- matches
    // TCustomWinSocket::Impl::bSim's default and the "Sim unless the caller
    // explicitly opts in to Real" policy documented in ClientSocket.h.
    bool bSimRequested;
    Impl() : bSimRequested(true) {}
};

TClientSocket::TClientSocket(TComponent* AOwner)
    : TComponent(AOwner)
    , Address()
    , Port(0)
    , Active(this)
    , OnConnect()
    , OnDisconnect()
    , OnError()
    , OnRead()
    , Socket(new TCustomWinSocket())
    , pImpl_(new Impl())
{
    Socket->pImpl_->owner = this;
}

TClientSocket::~TClientSocket()
{
    DoClose_();
    delete Socket;
    Socket = 0;
    delete pImpl_;
    pImpl_ = 0;
}

void TClientSocket::SetSimMode(bool bSim)
{
    pImpl_->bSimRequested = bSim;
    if (!Socket->pImpl_->bConnected)
        Socket->pImpl_->bSim = bSim;
}

bool TClientSocket::IsSimMode() const   { return Socket->pImpl_->bSim; }
bool TClientSocket::IsActiveNow() const { return Socket->pImpl_->bConnected; }

// ---------------------------------------------------------------------------
//  DoConnect_ -- Active=true.
//
//  POLICY (see ClientSocket.h header note): the DEFAULT is Sim (safe,
//  deterministic, no real network I/O in offline tests). Real networking is
//  only attempted when the composition root has explicitly opted in via
//  SetSimMode(false) beforehand -- NOT auto-attempted, and a failed Real
//  connect does NOT fall back to a fake Sim success (safety-IO rationale,
//  see header).
// ---------------------------------------------------------------------------
void TClientSocket::DoConnect_()
{
    if (Socket->pImpl_->bConnected)
        return;   // idempotent, mirrors Comm.h's StartComm guard

#if defined(_WIN32)
    if (!Socket->pImpl_->bSim)
    {
        // Explicit Real mode requested (SetSimMode(false)).
        if (EnsureWinsockInit_())
        {
            SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (s != INVALID_SOCKET)
            {
                sockaddr_in addr;
                std::memset(&addr, 0, sizeof(addr));
                addr.sin_family = AF_INET;
                addr.sin_port   = ::htons(static_cast<u_short>(Port));
                addr.sin_addr.s_addr = ::inet_addr(Address.c_str());
                if (addr.sin_addr.s_addr == INADDR_NONE)
                {
                    // Not a dotted-quad: try a hostname lookup fallback.
                    struct hostent* he = ::gethostbyname(Address.c_str());
                    if (he != 0 && he->h_length == sizeof(addr.sin_addr))
                        std::memcpy(&addr.sin_addr, he->h_addr, sizeof(addr.sin_addr));
                }

                int rc = ::connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
                if (rc == 0)
                {
                    Socket->pImpl_->sock      = s;
                    Socket->pImpl_->bConnected = true;
                    Socket->RemotePort = Port;

                    ::InterlockedExchange(&Socket->pImpl_->bStopReader, 0);
                    DWORD tid = 0;
                    Socket->pImpl_->hReader = ::CreateThread(
                        0, 0,
                        reinterpret_cast<LPTHREAD_START_ROUTINE>(&TCustomWinSocket::ReaderProc_),
                        Socket, 0, &tid);

                    if (OnConnect) OnConnect(this, Socket);
                    return;
                }

                int errCode = ::WSAGetLastError();
                ::closesocket(s);
                if (OnError) OnError(this, Socket, eeConnect, errCode);
                return;   // Real connect failed: stay disconnected (no Sim
                          // fallback -- see safety-IO rationale in the header).
            }
        }
        // socket()/WSAStartup failed: also stay disconnected, no fallback.
        int errCode =
#if defined(_WIN32)
            ::WSAGetLastError();
#else
            0;
#endif
        if (OnError) OnError(this, Socket, eeConnect, errCode);
        return;
    }
#endif

    // SIM mode (the default): succeed synchronously and deterministically.
    Socket->pImpl_->bSim       = true;
    Socket->pImpl_->bConnected = true;
    Socket->RemotePort = Port;
    if (OnConnect) OnConnect(this, Socket);
}

void TClientSocket::DoClose_()
{
    if (!Socket->pImpl_->bConnected)
        return;   // idempotent

    bool wasSim = Socket->pImpl_->bSim;

#if defined(_WIN32)
    if (!wasSim)
    {
        ::InterlockedExchange(&Socket->pImpl_->bStopReader, 1);
        if (Socket->pImpl_->sock != INVALID_SOCKET)
            ::shutdown(Socket->pImpl_->sock, SD_BOTH);   // unblock a pending recv()
        if (Socket->pImpl_->hReader != 0)
        {
            ::WaitForSingleObject(Socket->pImpl_->hReader, 2000);
            ::CloseHandle(Socket->pImpl_->hReader);
            Socket->pImpl_->hReader = 0;
        }
        if (Socket->pImpl_->sock != INVALID_SOCKET)
        {
            ::closesocket(Socket->pImpl_->sock);
            Socket->pImpl_->sock = INVALID_SOCKET;
        }
    }
#else
    (void)wasSim;
#endif

    Socket->pImpl_->bConnected = false;
    if (OnDisconnect) OnDisconnect(this, Socket);
    // Restore the sim flag to the last explicitly-requested policy for the
    // next DoConnect_() (mirrors Comm.h's StopComm -> bSim=bSimForced).
    Socket->pImpl_->bSim = pImpl_->bSimRequested;
}

void TClientSocket::Close()
{
    DoClose_();
}

// ---------------------------------------------------------------------------
//  ActiveProxy
// ---------------------------------------------------------------------------
TClientSocket::ActiveProxy::operator bool() const { return owner_->IsActiveNow(); }

TClientSocket::ActiveProxy& TClientSocket::ActiveProxy::operator=(bool v)
{
    if (v) owner_->DoConnect_();
    else   owner_->DoClose_();
    return *this;
}

} // namespace Scktcomp
