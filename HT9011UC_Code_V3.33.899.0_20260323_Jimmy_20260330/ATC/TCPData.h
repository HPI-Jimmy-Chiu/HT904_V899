#ifndef _TCPIP_DATA_H
#define _TCPIP_DATA_H

//#include <Classes.hpp>
//#include <Controls.hpp>
//#include <StdCtrls.hpp>
//#include <Forms.hpp>
#include <ScktComp.hpp>

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
        void __fastcall SocketConnect(TObject *Sender,TCustomWinSocket *Socket);
        void __fastcall SocketDisConnect(TObject *Sender,TCustomWinSocket *Socket);
        void __fastcall SocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);
        void __fastcall SocketRead(TObject *Sender, TCustomWinSocket *Socket);
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
        void __fastcall SocketConnect(TObject *Sender,TCustomWinSocket *Socket);
        void __fastcall SocketDisConnect(TObject *Sender,TCustomWinSocket *Socket);
        void __fastcall SocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);
        void __fastcall SocketRead(TObject *Sender, TCustomWinSocket *Socket);
};
#endif
