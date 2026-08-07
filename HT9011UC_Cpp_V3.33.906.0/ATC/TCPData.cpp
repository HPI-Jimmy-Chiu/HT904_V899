// =============================================================================
//  ATC/TCPData.cpp  --  generic byte/string TCP client+server buffering pair.
//
//  Faithful translation of golden ATC/TCPData.cpp (503 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2, group "atc".  First file of golden ATC/ to
//  land in the port tree; sibling ATC_WinWay.{h,cpp} lands in the SAME wave.
//
//  ROLE: see TCPData.h's own banner for the class-level description. This TU
//  defines all 5 TCPIP methods (Clear/PopString/GetBufferSize/PopBuffer/
//  PushBuffer) plus the 11+11 TCPClient/TCPServer methods (ctor, dtor,
//  Initial, Connect, DisConnect, Send(DATA), Send(string), and the 4
//  SocketConnect/SocketDisConnect/SocketError/SocketRead event handlers).
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim bodies): all 27 golden function definitions,
//     whole file --
//       TCPIP::Clear                golden :7-13      TCPIP::PopString        golden :15-26
//       TCPIP::GetBufferSize        golden :28-34     TCPIP::PopBuffer        golden :36-62
//       TCPIP::PushBuffer           golden :64-70
//       TCPClient::TCPClient        golden :75-87     TCPClient::~TCPClient   golden :89-101
//       TCPClient::Initial          golden :103-111   TCPClient::Connect      golden :113-124
//       TCPClient::DisConnect       golden :126-132   TCPClient::Send(DATA)   golden :134-161
//       TCPClient::Send(string)     golden :163-194   TCPClient::SocketConnect golden :196-211
//       TCPClient::SocketDisConnect golden :213-229   TCPClient::SocketError  golden :231-253
//       TCPClient::SocketRead       golden :255-285
//       TCPServer::TCPServer        golden :289-304   TCPServer::~TCPServer   golden :306-318
//       TCPServer::Initial          golden :320-325   TCPServer::Connect      golden :327-340
//       TCPServer::DisConnect       golden :342-349   TCPServer::Send(DATA)   golden :351-380
//       TCPServer::Send(string)     golden :382-413   TCPServer::SocketConnect golden :415-430
//       TCPServer::SocketDisConnect golden :432-448   TCPServer::SocketError  golden :450-468
//       TCPServer::SocketRead       golden :470-502
//     Plus the 3 inline TCPIP bodies already carried verbatim in TCPData.h
//     (IsConnect / SetDataType / GetStringSize).
//   SATISFIED-BY-SUBSTRATE (already real in this tree, nothing shimmed here):
//     MyDBIProcess(AnsiString,AnsiString) -- aHotPlateSubstrate.cpp:1030,
//     declared aHotPlateSubstrate.h:924 -- this tree's standing 2-arg
//     substitution for golden cMyDB.h's 3-arg-with-default MyDBIProcess (same
//     substitution recorded at Public/MyStringList.cpp:310 / TextProcess.cpp's
//     own banner / ainarm_SearchPickPlate.cpp:52 / atester_ProcessCount.cpp
//     :111). Both golden call sites in THIS file (golden :99, :316) pass
//     exactly 2 string-literal arguments, so the call shape is unchanged.
//     MySleep(DWORD) -- REAL definition common.cpp:1919, declared common.h
//     :366 (golden common.h:261). Forward-declared locally with a bare
//     `extern` (NOT `#include "common.h"`) -- the established house pattern
//     for this exact symbol (CCLink/MyCCLink.cpp:29, Automation/auto9045.cpp
//     :130, Automation/automation.cpp:80, Interface/TesterTCP.cpp:158,
//     KYECFTP/FTPClient_Transfer.cpp:67 all do the same instead of pulling in
//     the whole of common.h for one function). Golden's own `#include
//     "common.h"` (golden :5) is replaced by this narrower declaration; no
//     other common.h symbol is used anywhere in this file (checked).
//     Scktcomp::TClientSocket/TCustomWinSocket/TErrorEvent/TServerSocket --
//     vclcompat/ClientSocket.h + vclcompat/ServerSocket.h, both already part
//     of the vcl_compat.h umbrella (TCPData.h's own banner has the citation
//     that ClientSocket.h's SendText signature was grounded on THIS file's
//     golden :187).
//   GATE (#if 0): NONE -- every outbound call in this file resolves to a real
//     body already in the port tree.
//
//  VCL/Borland conversions: `#pragma hdrstop` dropped (golden :2). Golden's
//  `#include "MachineDefine.h"` kept (brings vcl_compat.h + windows.h +
//  CRITICAL_SECTION/EnterCriticalSection family + `using namespace std;`).
//  Golden's `#include "TCPData.h"` kept. Golden's `#include "common.h"`
//  narrowed to a local `extern void MySleep(DWORD);` forward declaration --
//  see the SATISFIED-BY-SUBSTRATE note above. No __fastcall in this .cpp
//  (already dropped in TCPData.h, matching per-TU). No AnsiString in this
//  file -- golden's own std::string/std::vector STL style is kept verbatim
//  (see TCPData.h's banner on why this is faithful, not a redesign).
//  Numeric semantics kept EXACT: `ErrorCode/0xFF` / `ErrorCode%0xFF` (golden
//  :237-238, :456-457) are int arithmetic pushed into a BYTE vector via an
//  implicit narrowing conversion, exactly as golden wrote it -- not
//  "improved" to a shift/mask form. `Data[iSize] = '\0';` (golden :184, :403)
//  writes the string's own null terminator at index == size(); this stores
//  charT() at s[s.size()], which C++11 [string.access] defines as valid
//  (only storing something OTHER than charT() there is undefined) -- so this
//  is well-defined under the port's C++17 target, not merely "happened to
//  work under BCB6".
//
//  MINIMAL NECESSARY ADAPTATION: golden's 8 event-property assignments
//  (`_client_socket->OnConnect = SocketConnect;` etc., golden :82-85,
//  :298-301) rely on BCB6's __closure implicit `this`-binding, which standard
//  C++ has no syntax for. Each is translated to a `this`-capturing lambda
//  forwarding to the unchanged named method -- the SAME idiom this tree
//  already established at MyPLC/ModbusTCPClient.cpp:34-37 for the identical
//  golden pattern. No behaviour delta: the lambda calls the exact same method
//  with the exact same arguments the __closure would have.
//
//  GOLDEN QUIRK preserved verbatim: `data.push_back( ERROR );` (golden :236,
//  :455) uses the bare identifier `ERROR`, not the file's own `TD_ERROR`
//  macro (TCPData.h). This is NOT a typo this port corrects: `ERROR` is a
//  Win32 macro (`#define ERROR 0`, wingdi.h, pulled in transitively by
//  windows.h) that numerically equals `TD_ERROR` (also 0) purely by
//  coincidence -- both golden's BCB6 build and this port's MinGW build see
//  the same macro, so the byte-level behaviour is identical either way. Kept
//  exactly as golden spelled it.
//
//  Big5: one Chinese comment in the whole file (golden :246, byte-scanned:
//  the ONLY non-ASCII bytes in TCPData.cpp are at offsets 6782-6793, all
//  inside this one line). Decoded via cp950 and reproduced below as UTF-8
//  ("Steven 20200320 : \xE9\x81\x87\xE5\x88\xB010053\xE5\xBE\x97\xE6\x96\xB7"
//  "\xE9\x96\x8B\xE9\x80\xA3\xE7\xB7\x9A" -- i.e. "encountering 10053, must
//  disconnect the line"). Final gate: ZERO U+FFFD in this file (verified).
// =============================================================================
#include "MachineDefine.h"

#include "TCPData.h"
#include "aHotPlateSubstrate.h"     // MyDBIProcess(AnsiString,AnsiString) -- see banner
extern void MySleep(DWORD dwMilliseconds);   // golden common.h:261, REAL definition common.cpp:1919
// TCPIP {
void TCPIP::Clear()
{
    EnterCriticalSection( &csBufferData );
    _byte_datas.clear();
    _string_datas.clear();
    LeaveCriticalSection( &csBufferData );
}
//---------------------------------------------------------------------------
string TCPIP::PopString()
{
    string data;

    if( GetStringSize() > 0)
    {
        data = _string_datas[0];
        //_string_datas.pop_back();
        _string_datas.erase( _string_datas.begin() , _string_datas.begin()+1);
    }
    return data;
}
//---------------------------------------------------------------------------
int TCPIP::GetBufferSize()
{
    EnterCriticalSection( &csBufferData );
    int iSize = _byte_datas.size();
    LeaveCriticalSection( &csBufferData );
    return iSize;
}
//---------------------------------------------------------------------------
int TCPIP::PopBuffer( int& iState , DATA& Data )
{
    EnterCriticalSection( &csBufferData );

    Data.clear();
    iState = -1;

    //if( GetBufferSize() > 0)
    if( _byte_datas.empty() == false)
    {
        iState = _byte_datas[0][0];

        int size = _byte_datas[0].size();
        for(int i = 1 ; i < size ; i++)
        {
            int x = _byte_datas[0][i];
            Data.push_back( x );
        }

        //_byte_datas.pop_back();
        _byte_datas.erase( _byte_datas.begin() , _byte_datas.begin()+1);
    }

    LeaveCriticalSection( &csBufferData );

    return Data.size();
}
//---------------------------------------------------------------------------
int TCPIP::PushBuffer( DATA Data )
{
    EnterCriticalSection( &csBufferData );
    _byte_datas.push_back( Data );
    LeaveCriticalSection( &csBufferData );
    return 0;
}
// }
//---------------------------------------------------------------------------
// Client Socket ------------------------------------------------------------
//---------------------------------------------------------------------------
TCPClient::TCPClient(TClientSocket* pSocket )
{
    InitializeCriticalSection( &csBufferData );
    _client_socket = pSocket;
    _connect    = false;
    _address    = _client_socket->Address.c_str();
    _port       = _client_socket->Port;
    //AI(W906-PT-W2) 20260807: BCB6 __closure event assignment implicitly binds
    //  `this`; standard C++ has no such syntax. Translated to a `this`-capturing
    //  lambda forwarding to the unchanged named method -- same idiom as this
    //  tree's established precedent, MyPLC/ModbusTCPClient.cpp:34-37.
    _client_socket->OnConnect       = [this](TObject *Sender, TCustomWinSocket *Socket){ SocketConnect(Sender, Socket); };
    _client_socket->OnDisconnect    = [this](TObject *Sender, TCustomWinSocket *Socket){ SocketDisConnect(Sender, Socket); };
    _client_socket->OnError         = [this](TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode){ SocketError(Sender, Socket, ErrorEvent, ErrorCode); };
    _client_socket->OnRead          = [this](TObject *Sender, TCustomWinSocket *Socket){ SocketRead(Sender, Socket); };
    Clear();
}
//---------------------------------------------------------------------------
TCPClient::~TCPClient()
{
    try
    {
        DisConnect();
        _client_socket = NULL;
        DeleteCriticalSection( &csBufferData );
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TCPClient");
    }
}
//---------------------------------------------------------------------------
void TCPClient::Initial( string sAddress , int iPort )
{
    DisConnect();
    _address = sAddress;
    _port    = iPort;
    _client_socket->Address = _address.c_str();
    _client_socket->Port    = _port;
    Clear();
}
//---------------------------------------------------------------------------
int TCPClient::Connect()
{
    _client_socket->Active = false;
    MySleep(100);
    _client_socket->Address = _address.c_str();
    _client_socket->Port    = _port;
    _client_socket->Active  = true;

    //_connect = true;

    return 0;
}
//---------------------------------------------------------------------------
int TCPClient::DisConnect()
{
    _client_socket->Active = false;
    MySleep(100);
    //_connect = false;
    return 0;
}
//---------------------------------------------------------------------------
int TCPClient::Send( DATA Data )
{
    if(IsConnect() == false)
        return -1;

    EnterCriticalSection( &csBufferData );

    DATA SendData;
    SendData.push_back(TD_SEND );

    int iSize = (int) Data.size();
    BYTE* Buff;
    Buff = new BYTE[iSize+1];
    for(int i = 0 ; i < iSize ; i++)
    {
        Buff[i] = Data[i];
        SendData.push_back( Data[i]);
    }

    Buff[iSize] = '\0';
    int iSend = _client_socket->Socket->SendBuf(Buff , iSize/*sizeof(Buff )*/ );

    _byte_datas.push_back( SendData );
    //PushBuffer( SendData );

    LeaveCriticalSection( &csBufferData );
    return iSend;
}
//---------------------------------------------------------------------------
int TCPClient::Send( string Data )
{
    if(IsConnect() == false)
        return -1;

    EnterCriticalSection( &csBufferData );

    DATA SendData;
    SendData.push_back(TD_SEND );

    int iSize = (int) Data.length();

    //BYTE* Buff;
    //Buff = new BYTE[iSize+1];
    for(int i = 0 ; i < iSize ; i++)
    {
        //Buff[i] = Data[i];
        SendData.push_back( Data[i]);
    }

    //Buff[iSize] = '\0';
    Data[iSize] = '\0';
    SendData.push_back( '\0' );

    int iSend = _client_socket->Socket->SendText( Data.c_str());

    _byte_datas.push_back( SendData );

    LeaveCriticalSection( &csBufferData );

    return iSend;
}
//---------------------------------------------------------------------------
void TCPClient::SocketConnect(TObject *Sender,TCustomWinSocket *Socket)
{
    EnterCriticalSection( &csBufferData );

    _connect = true;

        DATA data;
        data.push_back(TD_STATE );
        data.push_back( 0x00);
        _byte_datas.push_back( data );
        //PushBuffer( data );

        _string_datas.push_back( "3CONNECT" );

    LeaveCriticalSection( &csBufferData );
}
//---------------------------------------------------------------------------
void TCPClient::SocketDisConnect(TObject *Sender,TCustomWinSocket *Socket)
{
    EnterCriticalSection( &csBufferData );

        DATA data;
        data.push_back(TD_STATE );
        data.push_back( 0xFF );

        _byte_datas.push_back( data );
        //PushBuffer( data );

        _string_datas.push_back( "3DISCONNECT" );

    _connect = false;

    LeaveCriticalSection( &csBufferData );
}
//---------------------------------------------------------------------------
void TCPClient::SocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    EnterCriticalSection( &csBufferData );

    DATA data;
    data.push_back( ERROR );
    data.push_back( ErrorCode/0xFF );
    data.push_back( ErrorCode%0xFF );

    //_byte_datas.push_back( data );
    //PushBuffer( data );
    _byte_datas.push_back( data );

    _string_datas.push_back( "0ERROR" );

    if(ErrorCode==10053)    //Steven 20200320 : 遇到10053得斷開連線
    {
        _client_socket->Close();
    }

    ErrorCode = 0;  // don't Show Error Message.
    LeaveCriticalSection( &csBufferData );
}
//---------------------------------------------------------------------------
void TCPClient::SocketRead(TObject *Sender, TCustomWinSocket *Socket)
{
    EnterCriticalSection( &csBufferData );

    int length = _client_socket->Socket->ReceiveLength();

    if( length > 0)
    {
        /*string text( "1" );
        text += Socket->ReceiveText().c_str();
        _string_datas.push_back( text );
        */

        BYTE* Buff;
        Buff = new BYTE[length];
        _client_socket->Socket->ReceiveBuf(Buff , length );

        DATA data;
        data.push_back(TD_READ );
        for(int i = 0 ; i < length ; i++)
        {
            //if(Buff[i] != '\0' )
            {
                data.push_back(Buff[i]);
            }
        }
        _byte_datas.push_back( data );
        //PushBuffer( data );
    }
    LeaveCriticalSection( &csBufferData );
}
//---------------------------------------------------------------------------
// Server Socket ---------------------------------------------------------------
//---------------------------------------------------------------------------
TCPServer::TCPServer(TServerSocket* pSocket )
{
    InitializeCriticalSection( &csBufferData );

    _server_socket = pSocket;
    _connect    = false;
    //_address    = _client_socket->Address.c_str();
    _port       = _server_socket->Port;

    //AI(W906-PT-W2) 20260807: same `this`-capturing-lambda translation as
    //  TCPClient's ctor above (BCB6 __closure has no standard-C++ equivalent).
    _server_socket->OnClientConnect     = [this](TObject *Sender, TCustomWinSocket *Socket){ SocketConnect(Sender, Socket); };
    _server_socket->OnClientDisconnect  = [this](TObject *Sender, TCustomWinSocket *Socket){ SocketDisConnect(Sender, Socket); };
    _server_socket->OnClientError       = [this](TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode){ SocketError(Sender, Socket, ErrorEvent, ErrorCode); };
    _server_socket->OnClientRead        = [this](TObject *Sender, TCustomWinSocket *Socket){ SocketRead(Sender, Socket); };

    Clear();
}
//---------------------------------------------------------------------------
TCPServer::~TCPServer()
{
    try
    {
        DisConnect();
        _server_socket  = NULL;
        DeleteCriticalSection( &csBufferData );
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TCPServer()");
    }
}
//---------------------------------------------------------------------------
void TCPServer::Initial( int iPort )
{
    //_address    = sAddress;
    _port       = iPort;
    Clear();
}
//---------------------------------------------------------------------------
int TCPServer::Connect()
{
    _server_socket->Active = false;
    MySleep(100);

    //_server_socket->Address = _address.c_str();
    _server_socket->Port    = _port;

    _server_socket->Active  = true;

    //_connect = true;

    return 0;
}
//---------------------------------------------------------------------------
int TCPServer::DisConnect()
{
    _server_socket->Active = false;
    MySleep(100);
    //_connect = false;

    return 0;
}
//---------------------------------------------------------------------------
int TCPServer::Send( DATA Data )
{
    if(IsConnect() == false)
        return -1;

    EnterCriticalSection( &csBufferData );

    DATA SendData;
    SendData.push_back(TD_SEND );

    int iSize = (int) Data.size();
    BYTE* Buff;
    Buff = new BYTE[iSize+1];
    for(int i = 0 ; i < iSize ; i++)
    {
        Buff[i] = Data[i];
        SendData.push_back( Data[i]);
    }

    Buff[iSize] = '\0';

    int iSend = _server_socket->Socket->Connections[0]->SendBuf(Buff , iSize/*sizeof(Buff )*/ );

    _byte_datas.push_back( SendData );
    //PushBuffer( SendData );

    LeaveCriticalSection( &csBufferData );

    return iSend;
}
//---------------------------------------------------------------------------
int TCPServer::Send( string Data )
{
    if(IsConnect() == false)
        return -1;

    EnterCriticalSection( &csBufferData );

    DATA SendData;
    SendData.push_back(TD_SEND );

    int iSize = (int) Data.length();

    //BYTE* Buff;
    //Buff = new BYTE[iSize+1];
    for(int i = 0 ; i < iSize ; i++)
    {
        //Buff[i] = Data[i];
        SendData.push_back( Data[i]);
    }

    //Buff[iSize] = '\0';
    Data[iSize] = '\0';
    SendData.push_back( '\0' );

    int iSend = _server_socket->Socket->Connections[0]->SendText( Data.c_str());

    _byte_datas.push_back( SendData );

    LeaveCriticalSection( &csBufferData );

    return iSend;
}
//---------------------------------------------------------------------------
void TCPServer::SocketConnect(TObject *Sender,TCustomWinSocket *Socket)
{
    EnterCriticalSection( &csBufferData );

    _connect = true;

    DATA data;
    data.push_back(TD_STATE );
    data.push_back( 0x00);
    _byte_datas.push_back( data );
    //PushBuffer( data );

    _string_datas.push_back( "3CONNECT" );

    LeaveCriticalSection( &csBufferData );
}
//---------------------------------------------------------------------------
void TCPServer::SocketDisConnect(TObject *Sender,TCustomWinSocket *Socket)
{
    EnterCriticalSection( &csBufferData );

    DATA data;
    data.push_back(TD_STATE );
    data.push_back( 0xFF );

    _byte_datas.push_back( data );
    //PushBuffer( data );

    _string_datas.push_back( "3DISCONNECT" );

    _connect = false;

    LeaveCriticalSection( &csBufferData );
}
//---------------------------------------------------------------------------
void TCPServer::SocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    EnterCriticalSection( &csBufferData );

    DATA data;
    data.push_back( ERROR );
    data.push_back( ErrorCode/0xFF );
    data.push_back( ErrorCode%0xFF );

    //_byte_datas.push_back( data );
    //PushBuffer( data );
    _byte_datas.push_back( data );

    _string_datas.push_back( "0ERROR" );

    ErrorCode = 0;  // don't Show Error Message.

    LeaveCriticalSection( &csBufferData );
}
//---------------------------------------------------------------------------
void TCPServer::SocketRead(TObject *Sender, TCustomWinSocket *Socket)
{
    EnterCriticalSection( &csBufferData );

    int length = _server_socket->Socket->Connections[0]->ReceiveLength();

    if( length > 0)
    {
        /*string text( "1" );
        text += Socket->ReceiveText().c_str();
        _string_datas.push_back( text );
        */

        BYTE* Buff;
        Buff = new BYTE[length];
        _server_socket->Socket->Connections[0]->ReceiveBuf(Buff , length );

        DATA data;
        data.push_back(TD_READ );
        for(int i = 0 ; i < length ; i++)
        {
            //if(Buff[i] != '\0' )
            {
                data.push_back(Buff[i]);
            }
        }

        _byte_datas.push_back( data );
        //PushBuffer( data );
    }

    LeaveCriticalSection( &csBufferData );
}
// -----------------------------------------------------------------------------
