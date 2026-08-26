#include "MachineDefine.h"
#pragma hdrstop

#include "uSocketServerClient.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
uSocketBase::uSocketBase()
{
    InitialData();
}
//---------------------------------------------------------------------------
uSocketBase::~uSocketBase()
{
//
}
//---------------------------------------------------------------------------
void uSocketBase::ReloadData(AnsiString asSettingFileNameWithPath)
{
    InitialData();
    ReadSettingFile(asSettingFileNameWithPath);
    WriteSettingFile(asSettingFileNameWithPath);
    iBufferLenght=0;
}
//---------------------------------------------------------------------------
void uSocketBase::InitialData()
{
    asAddress="127.0.0.1";
    asPort="59999";
}
//---------------------------------------------------------------------------
void uSocketBase::ReadSettingFile(AnsiString asSettingFileNameWithPath)
{
    TIniFile* IniFile=new TIniFile(asSettingFileNameWithPath);
    asAddress=IniFile->ReadString(GetSettingSection(), "asAddress", asAddress);
    asPort=IniFile->ReadString(GetSettingSection(), "asPort", asPort);
    delete IniFile;
}
//---------------------------------------------------------------------------
void uSocketBase::WriteSettingFile(AnsiString asSettingFileNameWithPath)
{
    TIniFile* IniFile=new TIniFile(asSettingFileNameWithPath);
    IniFile->WriteString(GetSettingSection(), "asAddress", asAddress);
    IniFile->WriteString(GetSettingSection(), "asPort", asPort);
    delete IniFile;
}
//---------------------------------------------------------------------------
void uSocketBase::SetReceiveFunc(TPointVoidReceive _func)
{
    tpvReceive=_func;
}
//---------------------------------------------------------------------------
bool uSocketBase::Ping(AnsiString asIP)
{
    HANDLE hIcmp = LoadLibrary("ICMP.DLL");         // ICMP.DLL load
    if(hIcmp == NULL)
        return false;
    //Get pointers to the functions
    PF_CMPCREATEFILE pfIcmpCreateFile = (PF_CMPCREATEFILE)
    GetProcAddress(hIcmp, "IcmpCreateFile");
    PF_ICMPCLOSEHANDLE pfIcmpCloseHandle = (PF_ICMPCLOSEHANDLE)
    GetProcAddress(hIcmp, "IcmpCloseHandle");
    PF_ICMPSENDECHO pfIcmpSendEcho = (PF_ICMPSENDECHO)
    GetProcAddress(hIcmp, "IcmpSendEcho");
    if(pfIcmpCreateFile==NULL || pfIcmpCloseHandle==NULL || pfIcmpSendEcho==NULL)
    {
        FreeLibrary(hIcmp);
        return false;
    }
    WSADATA wsaData; //WinSock initialize
    int ilRetVal=WSAStartup(0x0101, &wsaData);      //initial WINSOCK
    if(ilRetVal)
    {
        WSACleanup();
        FreeLibrary(hIcmp);
        return false;
    }

    if(0x0101!=wsaData.wVersion)                    //Check WinSock version
    {
        WSACleanup();
        FreeLibrary(hIcmp);
        return false;
    }
    //check host name
    struct in_addr iaDest;                          // Structure for the Internet address
    iaDest.s_addr=inet_addr(asIP.c_str());
    LPHOSTENT pHost;                                // Pointer to the Host Entry structure
    if(iaDest.s_addr==INADDR_NONE)
        pHost=gethostbyname(asIP.c_str());
    else
        pHost=gethostbyaddr((BYTE *)&iaDest, sizeof(struct in_addr), AF_INET);
    if(pHost==NULL)
    {
        WSACleanup();
        FreeLibrary(hIcmp);
        return false;
    }

    DWORD* pAddress=(DWORD*)(*pHost->h_addr_list);  // IP-Adresse copy
    HANDLE hIcmpFile=pfIcmpCreateFile();            //ICMP Echo Request Handle obtain
    ICMPECHO icmpEcho;                              // ICMP-Echo response buffer
    IPINFO ipInfo;                                  // IP options structure
    int ilCount=0;                                  // Number of Round Trip Time data
    for(int ilPingNo=0; ilPingNo<1; ilPingNo++)
    {
        ::ZeroMemory(&ipInfo, sizeof(ipInfo));
        ipInfo.bTimeToLive=255;

        // Request ICMP Echo
        pfIcmpSendEcho(hIcmpFile,                   // Handle of IcmpCreateFile
                       *pAddress,                   // destination IP address
                       NULL,                        // Pointer to the buffer with the
                       0,                           // Buffergrosse in Bytes
                       &ipInfo,                     // Request Options
                       &icmpEcho,                   // response buffer
                       sizeof(struct tagICMPECHO),  // Buffergrosse
                       500);                        // Max. wait time in milliseconds

        iaDest.s_addr=icmpEcho.dwSource;
        if(icmpEcho.dwStatus)
            break;
        ilCount++;
    }

    pfIcmpCloseHandle(hIcmpFile);                   // Close echo request file handle
    FreeLibrary(hIcmp);
    WSACleanup();                                   //release WINSOCK
    if(ilRetVal!=ilCount)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
uSocketServer::uSocketServer()
{
    Initialization();
}
//---------------------------------------------------------------------------
uSocketServer::~uSocketServer()
{
    if(ServerSocket!=NULL)
    {
        delete ServerSocket;
    }
}
//---------------------------------------------------------------------------
void uSocketServer::Initialization()
{
    InitializationSocketServer();
    DoCommuncationTask=0;
}
//---------------------------------------------------------------------------
void uSocketServer::InitializationSocketServer()
{
    ServerSocket=new TServerSocket(NULL);
    ServerSocket->Port=59999;
    ServerSocket->OnClientConnect=ServerSocketConnect;
    ServerSocket->OnClientDisconnect=ServerSocketDisconnect;
    ServerSocket->OnClientError=ServerSocketError;
    ServerSocket->OnClientRead=ServerSocketRead;
}
//---------------------------------------------------------------------------
void __fastcall uSocketServer::ServerSocketConnect(TObject *Sender,
          TCustomWinSocket *Socket)
{
    //
}
//---------------------------------------------------------------------------
void __fastcall uSocketServer::ServerSocketDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    //
}
//---------------------------------------------------------------------------
void __fastcall uSocketServer::ServerSocketError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
      int &ErrorCode)
{
    //
}
//---------------------------------------------------------------------------
void __fastcall uSocketServer::ServerSocketRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    iBufferLenght=Socket->ReceiveLength();
    if(iBufferLenght>0)
    {
        iBufferLenght=iBufferLenght>ReceiveLen?ReceiveLen:iBufferLenght;
        Socket->ReceiveBuf(strReceiveUse, iBufferLenght);
        try
        {
            char* Buff=new char[iBufferLenght];
            memcpy(Buff,strReceiveUse, iBufferLenght);
            tpvReceive(Buff, iBufferLenght);
        }
        catch(...)
        {}
    }
}
//---------------------------------------------------------------------------
int uSocketServer::GetActiveConnections()
{
    if(ServerSocket->Socket!=NULL)
        return ServerSocket->Socket->ActiveConnections;
    else
        return 0;
}
//---------------------------------------------------------------------------
bool uSocketServer::DoOpenCommuncation()
{
    switch(DoCommuncationTask)
    {
        case 0:
            DoCommuncationTask=50;
            break;
        case 50:
            if(MatchServerPort())
                DoCommuncationTask=100;
            break;
        case 100:
            if(Open())
                DoCommuncationTask=200;
            else
                DoCommuncationTask=50;
            break;
        case 200:
            if(IsConnected())
                return true;
            else
                DoCommuncationTask=50;
    }
    return false;
}
//---------------------------------------------------------------------------
bool uSocketServer::MatchServerPort()
{
    if(GetSocketPort()=="" || GetSocketPort()=="0")
        return false;
    if(ServerSocket->Port!=GetSocketPort())
        ServerSocket->Port=atoi(GetSocketPort().c_str());
    return true;
}
//---------------------------------------------------------------------------
bool uSocketServer::Open()
{
    try
    {
        ServerSocket->Open();
    }
    catch(...)
    {
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
AnsiString uSocketServer::GetConnectClientAddress()
{
    if(IsConnected() && GetActiveConnections()==1)
        return ServerSocket->Socket->Connections[0]->RemoteAddress;
    return "None";
}
//---------------------------------------------------------------------------
bool uSocketServer::SendCommand(char* cSet, int iLen)
{
    if(IsConnected()==false || GetActiveConnections()!=1)
        return false;
    ServerSocket->Socket->Connections[0]->SendBuf(cSet, iLen);
    Sleep(50);
    return true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
uSocketClient::uSocketClient()
{
    Initialization();
}
//---------------------------------------------------------------------------
uSocketClient::~uSocketClient()
{
    Close();
    if(ClientSocket!=NULL)
        delete ClientSocket;
}
//---------------------------------------------------------------------------
void uSocketClient::Initialization()
{
    InitializationSocketClient();
    bConnected=false;
}
//---------------------------------------------------------------------------
void uSocketClient::InitializationSocketClient()
{
    ClientSocket                =new TClientSocket(NULL);
    ClientSocket->Address       =GetSocketAddress();
    ClientSocket->Port          =atoi(GetSocketPort().c_str());
    ClientSocket->OnError       =ClientSocketError;
    ClientSocket->OnRead        =ClientSocketRead;
    ClientSocket->OnConnect     =ClientSocketConnect;
    ClientSocket->OnDisconnect  =ClientSocketDisconnect;
}
//---------------------------------------------------------------------------
void __fastcall uSocketClient::ClientSocketRead(TObject *Sender,
        TCustomWinSocket *Socket)
{
    iBufferLenght=Socket->ReceiveLength();
    if(iBufferLenght>0)
    {
        iBufferLenght=iBufferLenght>ReceiveLen?ReceiveLen:iBufferLenght;
        Socket->ReceiveBuf(strReceiveUse, iBufferLenght);
        try
        {
            tpvReceive(strReceiveUse, iBufferLenght);
        }
        catch(...)
        {}
    }
}
//---------------------------------------------------------------------------
void __fastcall uSocketClient::ClientSocketError(TObject *Sender,
        TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    bConnected=false;
    ErrorCode=0;
    try
    {
        Socket->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "ClientSocket Error");
    }
}
//---------------------------------------------------------------------------
void __fastcall uSocketClient::ClientSocketConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    bConnected=true;
}
//---------------------------------------------------------------------------
void __fastcall uSocketClient::ClientSocketDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    bConnected=false;
}
//---------------------------------------------------------------------------
bool uSocketClient::DoOpenCommuncation()
{
    if(MatchClientSetting() &&
       Open()               &&
       bConnected)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool uSocketClient::MatchClientSetting()
{
    if(bConnected!=ClientSocket->Active)
    {
        Close();
        return false;
    }

    if(GetSocketPort()=="" || GetSocketPort()=="0")
        return false;
    if(ClientSocket->Address!=GetSocketAddress() || ClientSocket->Port!=GetSocketPort())
    {
        if(IsConnected())
        {
            Close();
            return false;
        }
        ClientSocket->Address=GetSocketAddress();
        ClientSocket->Port=atoi(GetSocketPort().c_str());
    }
    return true;
}
//---------------------------------------------------------------------------
bool uSocketClient::Open()
{
    if(IsConnected())
        return true;
    try
    {
        Close();
        ClientSocket->Open();
    }
    catch(...)
    {
        bConnected=false;
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void uSocketClient::Close()
{
    ClientSocket->Active=false;
    if(ClientSocket!=NULL)
        ClientSocket->Close();
}
//---------------------------------------------------------------------------
bool uSocketClient::SendCommand(char* cSet, int iLen)
{
    if(IsConnected()==false)
        return false;
    ClientSocket->Socket->SendBuf(cSet,iLen);
    Sleep(50);
    return true;
}
//---------------------------------------------------------------------------
bool uSocketClient::SetCommParameter(AnsiString asAddress, AnsiString asPort)
{
    SetSocketAddress(asAddress);
    SetSocketPort(asPort);
    return MatchClientSetting();
}
//---------------------------------------------------------------------------
