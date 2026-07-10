// ===========================================================================
//  vclcompat/ClientSocket.h
//  Portable shim for the VCL ScktComp TClientSocket / TCustomWinSocket
//  components (BCB6 #include <ScktComp.hpp>).
//
//  AI(W5-MyPLC-Translate) 20260710: new file. The ONLY 906 consumer of
//  TClientSocket/TCustomWinSocket is MyPLC/ModbusTCPClient.cpp (recon
//  confirmed via full-tree grep). This shim is genuinely new shared
//  vclcompat/ infrastructure -- authorized for THIS unit only (per the batch
//  task spec) because it is this unit's own prerequisite and no other
//  parallel unit in this batch needs a TCP client socket. A later unit that
//  also needs one should REUSE this file rather than adding a second shim.
//
//  PROVENANCE: golden ScktComp.hpp is Borland RTL and is NOT present anywhere
//  in this repo snapshot (unlike SPComm.hpp, which vclcompat/Comm.h could
//  cite from D:/HT9045/elec/Component/spcomm.hpp). The API surface below was
//  reverse-engineered EXHAUSTIVELY from the actual call sites in golden
//  MyPLC/ModbusTCPClient.cpp (grepped, not guessed from memory of the full
//  VCL API -- see SCOPED API SURFACE below).
//
//  SCOPED API SURFACE (every TClientSocket/TCustomWinSocket touch point in
//  the golden 906 tree, MyPLC/ModbusTCPClient.cpp:1-221):
//      new TClientSocket(NULL)                     ctor, owner=NULL   (:12)
//      ->OnConnect    = SocketConnect               event assign      (:14)
//      ->OnDisconnect = SocketDisConnect             event assign      (:15)
//      ->OnError      = SocketError                  event assign      (:16)
//      ->OnRead       = SocketRead                    event assign      (:17)
//      ->Address      = AnsiString                    property          (:40)
//      ->Port         = int                            property          (:41)
//      ->Active       = bool     SIDE EFFECT: true opens/connects,
//                                 false closes                          (:48,:83)
//      ->Close()                                       method            (:86)
//      ->Socket->SendBuf(void*, int)                   method            (:141)
//      ->Socket->ReceiveLength()                        method -> int     (:180)
//      ->Socket->ReceiveBuf(void*, int)                  method -> int    (:186)
//      ->Socket->RemotePort                              property -> int  (:170)
//      ->Socket->Disconnect(int Port)                    method            (:169)
//        (golden SocketError handler calls Disconnect(Socket->RemotePort);
//        kept faithfully even though vanilla documented VCL TCustomWinSocket
//        does not advertise a Disconnect(int) overload -- this project's 906
//        source calls it this way, so the shim provides exactly that call
//        shape rather than "fixing" it to Close().)
//
//  Event signatures (standard VCL ScktComp __closure -> std::function, same
//  translation choice as vclcompat/Comm.h's TReceiveDataEvent):
//      TSocketNotifyEvent = void(TObject* Sender, TCustomWinSocket* Socket)
//      TSocketErrorEvent  = void(TObject* Sender, TCustomWinSocket* Socket,
//                                TErrorEvent ErrorEvent, int& ErrorCode)
//
//  NOT implemented (never called by the only 906 consumer; do NOT add):
//      ClientType, LookupAddress/LookupPort/LocalPort, OnLookup, Handle,
//      TCustomWinSocket::ReceiveText/SendText, Owner/Session, the whole
//      TServerSocket/TServerWinSocket family.
//
//  BACKING -- Sim/Real, SAFETY-MOTIVATED POLICY (deliberately DIFFERENT from
//  the Comm.h auto-fallback precedent -- see rationale below):
//    * SIM mode (the DEFAULT, unconditionally, until a caller opts in to
//      Real): no OS socket. Active=true synchronously marks connected and
//      fires OnConnect (deterministic for offline tests -- real VCL's async
//      connect timing is not reproduced). SendBuf captures into a tx sink;
//      the test hook Socket->SimPushReceive() enqueues bytes and fires OnRead
//      so the handler pulls them via ReceiveLength()/ReceiveBuf() exactly as
//      the real one does.
//    * REAL mode -- entered ONLY via an explicit SetSimMode(false) call (a
//      composition-root / factory decision, matching the project's Sim<->Real
//      selection architecture, KNOWLEDGE.md Sec.1) -- WinSock2 connect() to
//      Address:Port; a background reader thread (Win32 CreateThread, same
//      idiom as vclcompat/Comm.cpp's ReaderProc_) blocks in recv() and, on
//      each arrival, enqueues bytes + fires OnRead (a "data available" notice
//      -- the handler itself pulls via Socket->ReceiveBuf, exactly as golden
//      ModbusTCPClient::SocketRead does).
//    * DELIBERATE DEVIATION from Comm.h: Comm.h's TComm auto-falls-back to
//      Sim whenever the real port cannot be opened. This shim does NOT: this
//      client feeds Enable_PLCSafety_IO (an E-STOP/safe-door interlock read
//      over Modbus, cmydef.h/cinitial.cpp) -- silently reporting a fake
//      "connected" success after a genuine Real-mode connect failure would
//      mask a real safety-IO outage in production. In Real mode, a failed
//      connect() leaves bConnected==false, fires OnError with the real
//      WSAGetLastError() code, and otherwise changes NOTHING -- so golden's
//      OWN faithfully-translated retry loop (PLCStatusCheck's tCycleConnect,
//      5000ms) keeps retrying exactly as the 906 source intends. Flagged
//      explicitly for the integrate agent / reviewer.
//
//  Include directly: #include "vclcompat/ClientSocket.h" (NOT yet added to
//  the vcl_compat.h umbrella -- left for the integrate agent to decide/wire).
// ===========================================================================
#ifndef VCLCOMPAT_CLIENTSOCKET_H
#define VCLCOMPAT_CLIENTSOCKET_H

#include "vclcompat/AnsiString.h"
#include "vclcompat/TStringList.h"   // vclcompat::TObject
#include "vclcompat/Comm.h"          // vclcompat::TComponent (reuse, per Comm.h)
#include <vector>
#include <functional>

namespace Scktcomp {

using vclcompat::AnsiString;
using vclcompat::TObject;
using vclcompat::TComponent;

// Real VCL ScktComp TErrorEvent enum (documented Borland RTL ordinals). The
// golden SocketError handler never inspects this parameter (only clears
// ErrorCode), so exact ordinal fidelity is not load-bearing here -- kept for
// signature completeness.
enum TErrorEvent { eeGeneral, eeSend, eeReceive, eeConnect, eeDisconnect, eeAccept };

class TClientSocket;   // fwd (back-pointer target)

// ---------------------------------------------------------------------------
//  TCustomWinSocket -- the live "Socket" sub-object (golden: ->Socket->...).
//  Always non-null on a TClientSocket (mirrors VCL's auto-created
//  TClientWinSocket instance) -- golden always dereferences ->Socket
//  unconditionally, never null-checks it.
// ---------------------------------------------------------------------------
class TCustomWinSocket : public TObject {
public:
    TCustomWinSocket();
    virtual ~TCustomWinSocket();

    // ---- properties ---------------------------------------------------
    int RemotePort;   // golden Socket->RemotePort; 0 until connected

    // ---- methods (faithful signatures per the SCOPED API SURFACE above) ---
    int  ReceiveLength() const;               // bytes currently queued
    int  ReceiveBuf(void* Buf, int BufSize);   // dequeue up to BufSize bytes
    int  SendBuf(void* Buf, int BufSize);      // send/capture bytes; -1 on fail

    // Faithful to golden `Socket->Disconnect(Socket->RemotePort)`
    // (SocketError handler). Closes the underlying connection; Port is
    // accepted for call-shape fidelity and not otherwise used.
    void Disconnect(int Port);

    // ---- SIM / test surface (additive, NOT part of the BCB6 API) ----------
    void SimPushReceive(const void* pData, int len);  // enqueue as if-arrived,
                                                       // fires owner->OnRead
    const std::vector<char>& SimTxBuffer() const;     // bytes SendBuf captured (sim)
    void SimClearTx();

private:
    TCustomWinSocket(const TCustomWinSocket&);
    TCustomWinSocket& operator=(const TCustomWinSocket&);

    friend class TClientSocket;
    static unsigned long __stdcall ReaderProc_(void* param);  // real-mode reader thread

    struct Impl;
    Impl* pImpl_;
};

// Event signatures (VCL __closure -> std::function; matches the Comm.h
// TReceiveDataEvent translation choice).
typedef std::function<void(TObject* Sender, TCustomWinSocket* Socket)>
        TSocketNotifyEvent;
typedef std::function<void(TObject* Sender, TCustomWinSocket* Socket,
                            TErrorEvent ErrorEvent, int& ErrorCode)>
        TSocketErrorEvent;

// ---------------------------------------------------------------------------
//  TClientSocket
// ---------------------------------------------------------------------------
class TClientSocket : public TComponent {
public:
    explicit TClientSocket(TComponent* AOwner);
    virtual ~TClientSocket();

    // ---- properties --------------------------------------------------------
    AnsiString Address;
    int        Port;

    // Active: BCB6 property with a connect/disconnect SIDE EFFECT on plain
    // assignment (`pClinetSocket->Active=true;`). Proxy-with-operator=
    // idiom, same pattern already established by vclcompat/TStringList.h's
    // StringsProxy/TextProxy family.
    class ActiveProxy {
    public:
        explicit ActiveProxy(TClientSocket* o) : owner_(o) {}
        operator bool() const;
        ActiveProxy& operator=(bool v);
    private:
        TClientSocket* owner_;
    };
    ActiveProxy Active;

    void Close();   // golden also calls this explicitly right after Active=false

    // ---- events --------------------------------------------------------------
    TSocketNotifyEvent OnConnect;
    TSocketNotifyEvent OnDisconnect;
    TSocketErrorEvent  OnError;
    TSocketNotifyEvent OnRead;

    // ---- Socket sub-object -----------------------------------------------
    TCustomWinSocket* Socket;   // owned; always valid for the object's lifetime

    // ---- SIM / test surface (additive) ------------------------------------
    // Composition-root switch: default is Sim (safe, offline, deterministic).
    // Call SetSimMode(false) to opt in to a REAL WinSock2 connection.
    void SetSimMode(bool bSim);
    bool IsSimMode() const;
    bool IsActiveNow() const;   // true between a successful connect and close

private:
    TClientSocket(const TClientSocket&);
    TClientSocket& operator=(const TClientSocket&);

    void DoConnect_();
    void DoClose_();
    friend class ActiveProxy;

    struct Impl;
    Impl* pImpl_;
};

} // namespace Scktcomp

// Bring the ScktComp names into global scope, matching the original header's
// implicit unqualified TClientSocket/TCustomWinSocket usage in golden
// (Comm.h precedent: same umbrella-suppressible `using namespace`).
#ifndef VCLCOMPAT_NO_GLOBAL_USING
using namespace Scktcomp;
#endif

#endif // VCLCOMPAT_CLIENTSOCKET_H
