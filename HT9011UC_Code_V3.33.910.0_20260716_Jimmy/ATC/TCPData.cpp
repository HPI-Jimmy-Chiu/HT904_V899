#include "MachineDefine.h"
#pragma hdrstop

#include "TCPData.h"
#include "common.h"
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
    _client_socket->OnConnect       = SocketConnect;
    _client_socket->OnDisconnect    = SocketDisConnect;
    _client_socket->OnError         = SocketError;
    _client_socket->OnRead          = SocketRead;
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
void __fastcall TCPClient::SocketConnect(TObject *Sender,TCustomWinSocket *Socket)
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
void __fastcall TCPClient::SocketDisConnect(TObject *Sender,TCustomWinSocket *Socket)
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
void __fastcall TCPClient::SocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
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
void __fastcall TCPClient::SocketRead(TObject *Sender, TCustomWinSocket *Socket)
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

    _server_socket->OnClientConnect     = SocketConnect;
    _server_socket->OnClientDisconnect  = SocketDisConnect;
    _server_socket->OnClientError       = SocketError;
    _server_socket->OnClientRead        = SocketRead;

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
void __fastcall TCPServer::SocketConnect(TObject *Sender,TCustomWinSocket *Socket)
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
void __fastcall TCPServer::SocketDisConnect(TObject *Sender,TCustomWinSocket *Socket)
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
void __fastcall TCPServer::SocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
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
void __fastcall TCPServer::SocketRead(TObject *Sender, TCustomWinSocket *Socket)
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
