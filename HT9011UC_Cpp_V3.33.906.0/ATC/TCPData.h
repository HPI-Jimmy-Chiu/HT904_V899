// =============================================================================
//  ATC/TCPData.h  --  generic byte/string TCP client+server buffering pair
//                     (TCPIP / TCPClient / TCPServer).
//
//  Faithful translation of golden ATC/TCPData.h (91 lines, BCB6, Big5/cp950 --
//  this header itself is pure ASCII, no Chinese comments).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2, group "atc".  First file of golden ATC/ to
//  land in the port tree.
//
//  ROLE: a small abstract base (TCPIP) plus two concrete VCL-socket adapters
//  (TCPClient wraps a TClientSocket, TCPServer wraps a TServerSocket) that
//  buffer inbound bytes / connect-disconnect state transitions / outbound echo
//  copies into two mutex-guarded queues (_byte_datas / _string_datas) so a
//  polling caller (elsewhere in the ATC subsystem) can drain them without
//  touching the VCL socket objects directly.  Protocol framing is NOT this
//  unit's job -- it is a raw byte/line pump.  NO state machine, NO switch(Task)
//  cursor.
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim): the WHOLE header -- both classes, all method
//     declarations, both typedef families, all 4 macros.  Golden already wrote
//     this file in std::string/std::vector STL style (its own `#include
//     <vector>/<string>/<map>` + `using namespace std;`, NOT AnsiString) --
//     kept exactly that way; substituting AnsiString here would be an
//     un-asked-for redesign, not a faithful translation.
//   SATISFIED-BY-SUBSTRATE (already real in this tree, nothing shimmed here):
//     CRITICAL_SECTION/BYTE (windows.h, via vcl_compat.h), Scktcomp::
//     TClientSocket/TCustomWinSocket/TErrorEvent (vclcompat/ClientSocket.h)
//     and Scktcomp::TServerSocket (vclcompat/ServerSocket.h) -- both already
//     wired into the vcl_compat.h umbrella and both already anticipate THIS
//     exact golden call shape: ClientSocket.h's own EXTENSION note cites
//     "ATC\TCPData.cpp:187 `int iSend = _client_socket->Socket->SendText(...)`"
//     verbatim as the reason TCustomWinSocket::SendText returns int, not void.
//   GATE (#if 0): NONE.
//
//  VCL/Borland conversions: golden's `#include <ScktComp.hpp>` is replaced by
//  `#include "vclcompat/vcl_compat.h"` (brings in Scktcomp::TClientSocket/
//  TCustomWinSocket/TErrorEvent via ClientSocket.h and Scktcomp::TServerSocket
//  via ServerSocket.h, both already part of the umbrella -- see
//  CMakeLists.txt:240's `#include "vclcompat/ServerSocket.h"` in vcl_compat.h).
//  `__fastcall` is DROPPED from all 8 socket-event method declarations (golden
//  :67-70, :86-89) -- a real MinGW keyword, matching the house convention
//  already stated in acarry.h:8 / MyStringList.h's own banner. No __property /
//  __published / #pragma in this header (golden has none either).
//
//  Big5: this header has ZERO non-ASCII bytes (verified byte-scan of the
//  golden file) -- no Chinese comments to carry over, no U+FFFD risk here.
// =============================================================================
#ifndef _TCPIP_DATA_H
#define _TCPIP_DATA_H

#include "vclcompat/vcl_compat.h"   // AnsiString; windows.h -> CRITICAL_SECTION/BYTE;
                                    // Scktcomp::TClientSocket/TCustomWinSocket/TErrorEvent/
                                    // TServerSocket (global via the umbrella's `using namespace`)

#include <vector>
#include <string>
#include <map>
using namespace std;

#define BYTE_DATA       0x00
#define STRING_DATA     0x01

#define TD_ERROR       0
#define TD_READ        1
#define TD_SEND        2
#define TD_STATE       3

typedef vector< BYTE > DATA;
typedef vector< DATA > DATAs;
typedef vector< string > STRINGs;

class TCPIP
{
    private:
    protected:
        string      _address;
        int         _port;
        bool        _connect;
        int         _data_type;
        DATAs       _byte_datas;
        STRINGs     _string_datas;
        CRITICAL_SECTION csBufferData;
    public:
        virtual void Initial(string sAddress, int iPort)    = 0;
        virtual int Connect()                               = 0;
        virtual int DisConnect()                            = 0;
        virtual int Send(DATA Data)                         = 0;
        virtual int Send(string Data)                       = 0;
        bool IsConnect()                {return _connect;};
        void SetDataType(int type)      {_data_type=type;};
        void Clear();
        int GetStringSize()             {return _string_datas.size();};
        string PopString();
        int GetBufferSize();
        int PopBuffer(int& iState, DATA& Data);
        int PushBuffer(DATA Data);
};
//---------------------------------------------------------------------------
class TCPClient : public TCPIP
{
    private:
        TClientSocket* _client_socket;
    protected:
    public:
        TCPClient(TClientSocket* pSocket);
        ~TCPClient();
        virtual void Initial(string sAddress, int iPort);
        virtual int Connect();
        virtual int DisConnect();
        virtual int Send(DATA Data);
        virtual int Send(string Data);
        void SocketConnect(TObject *Sender,TCustomWinSocket *Socket);
        void SocketDisConnect(TObject *Sender,TCustomWinSocket *Socket);
        void SocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);
        void SocketRead(TObject *Sender, TCustomWinSocket *Socket);
};
//---------------------------------------------------------------------------
class TCPServer : public TCPIP
{
    private:
        TServerSocket* _server_socket;
    protected:
    public:
        TCPServer(TServerSocket* pSocket);
        ~TCPServer();
        virtual void Initial(int iPort);
        virtual int Connect();
        virtual int DisConnect();
        virtual int Send(DATA Data);
        virtual int Send(string Data);
        void SocketConnect(TObject *Sender,TCustomWinSocket *Socket);
        void SocketDisConnect(TObject *Sender,TCustomWinSocket *Socket);
        void SocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);
        void SocketRead(TObject *Sender, TCustomWinSocket *Socket);
};
#endif
