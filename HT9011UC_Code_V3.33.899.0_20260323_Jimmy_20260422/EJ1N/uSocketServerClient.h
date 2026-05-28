//---------------------------------------------------------------------------

#ifndef uSocketServerClientH
#define uSocketServerClientH
//---------------------------------------------------------------------------
#include <ScktComp.hpp>
#include <map>
#include <vcl.h>
//---------------------------------------------------------------------------
#define ReceiveLen 1023
//---------------------------------------------------------------------------
// Definition of the IP options structure
typedef struct tagIPINFO
{
    BYTE bTimeToLive; // Time To Live
    BYTE bTypeOfService; // Type Of Service
    BYTE bIpFlags; // IP-Flags
    BYTE OptSize; // Size of the options data buffers
    BYTE FAR *Options; // Pointer to options data buffer
} IPINFO, *PIPINFO;

// Definition the ICMP-Echo response structure
typedef struct tagICMPECHO
{
    DWORD dwSource; // destination address
    DWORD dwStatus; // IP-Status
    DWORD dwRTTime; // Round trip time in milliseconds
    WORD wDataSize; // Grosse des reply-Buffers
    WORD wReserved;
    void FAR *pData; // pointer to the reply-Data
    IPINFO ipInfo; // reply-options
} ICMPECHO, *PICMPECHO;
typedef HANDLE (WINAPI *PF_CMPCREATEFILE)(VOID);
typedef BOOL (WINAPI *PF_ICMPCLOSEHANDLE)(HANDLE);
typedef DWORD (WINAPI *PF_ICMPSENDECHO)(HANDLE,DWORD,LPVOID,WORD,
PIPINFO,LPVOID,DWORD,DWORD);
//---------------------------------------------------------------------------
typedef void (__closure *TPointVoidReceive)(char*,int);
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class uSocketBase
{
private:    // User declarations
    AnsiString asAddress;
    AnsiString asPort;
public:     // User declarations
    TPointVoidReceive tpvReceive;
    char strReceiveUse[ReceiveLen];
    int iBufferLenght;
    //for setting
    AnsiString GetSettingSection(){return "SocketSetting";}
    void SetSocketAddress(AnsiString asValue){asAddress=asValue;}
    AnsiString GetSocketAddress(){return asAddress;}
    void SetSocketPort(AnsiString asValue){asPort=asValue;}
    AnsiString GetSocketPort(){return asPort;}
    //
    uSocketBase();
    ~uSocketBase();
    void InitialData();
    void ReloadData(AnsiString asSettingFileNameWithPath);
    void ReadSettingFile(AnsiString asSettingFileNameWithPath);
    void WriteSettingFile(AnsiString asSettingFileNameWithPath);
    void SetReceiveFunc(TPointVoidReceive _func);
    bool Ping(AnsiString asIP);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class uSocketServer : public uSocketBase
{
private:    // User declarations
    int DoCommuncationTask;
    TServerSocket *ServerSocket;
    //function
    void InitializationSocketServer();
    void __fastcall ServerSocketConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ServerSocketDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ServerSocketError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ServerSocketRead(TObject *Sender,
          TCustomWinSocket *Socket);
    bool MatchServerPort();
    bool IsConnected(){return ServerSocket->Socket->Connected;}
    bool Open();
public:     // User declarations
    uSocketServer();
    ~uSocketServer();
    void Initialization();
    int GetActiveConnections();
    AnsiString GetConnectClientAddress();
    bool DoOpenCommuncation();
    bool SendCommand(char* cSet,int iLen);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class uSocketClient : public uSocketBase
{
private:    // User declarations
    TClientSocket* ClientSocket;
    //function
    void __fastcall ClientSocketRead(TObject *Sender,
        TCustomWinSocket *Socket);
    void __fastcall ClientSocketError(TObject *Sender,
        TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);
    void __fastcall ClientSocketConnect(TObject *Sender,
      TCustomWinSocket *Socket);
    void __fastcall ClientSocketDisconnect(TObject *Sender,
      TCustomWinSocket *Socket);
    bool MatchClientSetting();
    bool bConnected;
public:     // User declarations
    uSocketClient();
    ~uSocketClient();
    void Initialization();
    void InitializationSocketClient();
    bool Open();
    bool IsConnected(){return bConnected  || ClientSocket->Active==true;}
    bool DoOpenCommuncation();
    void Close();
    bool SendCommand(char* cSet,int iLen);
    bool SetCommParameter(AnsiString asAddress,AnsiString asPort);
};
//---------------------------------------------------------------------------
#endif
