//------------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "ModbusTCPClient.h"

#include "MyPLC_IO_Modbus.h"
#pragma package(smart_init)
//------------------------------------------------------------------------------
__fastcall ModbusTCPClient::ModbusTCPClient()
{
    pClinetSocket=new TClientSocket(NULL);

    pClinetSocket->OnConnect       = SocketConnect;
    pClinetSocket->OnDisconnect    = SocketDisConnect;
    pClinetSocket->OnError         = SocketError;
    pClinetSocket->OnRead          = SocketRead;

    InitializeCriticalSection(&cs);
}
//------------------------------------------------------------------------------
ModbusTCPClient::~ModbusTCPClient()
{
    DisConnect();
    if(pClinetSocket!=NULL)
    {
        delete pClinetSocket;
    }
}
//------------------------------------------------------------------------------
void ModbusTCPClient::SetScan(bool bSet)
{
    if(bSet)
    {
        Connect();
    }
    else
    {
        DisConnect();
    }
}
//------------------------------------------------------------------------------
void ModbusTCPClient::SetTCPInfo(AnsiString asIP, int Port)
{
    pClinetSocket->Address=asIP;
    pClinetSocket->Port=Port;
}
//------------------------------------------------------------------------------
bool ModbusTCPClient::Connect()
{
    try
    {
        pClinetSocket->Active=true;
//        bConnected=true;                                                      //JerryYang 20250206 : fix
    }
    catch(...)
    {
        bConnected=false;
        return false;
    }
    return bConnected;
}
//------------------------------------------------------------------------------
void ModbusTCPClient::Cycle()
{
    vector<int>::iterator it;
    if(vtEarseData.size()>0)
    {
        for(it=vtEarseData.begin(); it!=vtEarseData.end(); it++)
        {
            if(mapReplyData.find((*it))!=mapReplyData.end())
            {
                EnterCriticalSection(&cs);
                mapReplyData.erase(mapReplyData.find((*it)));
                vtEarseData.erase(it);
                LeaveCriticalSection(&cs);
                return;
            }
        }
    }
}
//------------------------------------------------------------------------------
bool ModbusTCPClient::DisConnect()
{
    try
    {
        pClinetSocket->Active=false;
        bConnected=false;
        if(pClinetSocket!=NULL)
            pClinetSocket->Close();
        return true;
    }
    catch(...)
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool ModbusTCPClient::IsConnected()
{
//    return pClinetSocket->Socket->Connected;                                  //JerryYang 20250206 : fix
    return bConnected;
}
//------------------------------------------------------------------------------
bool ModbusTCPClient::SendData(int iID, int iSlave, int iFunctionCode, int iAddr, VTBYTEDATA data)
{
    BYTE* ByteData;
    int iSizeCount;
    int iDataSize=data.size();
    if(IsConnected()==false || iDataSize<=0)
    {
        return false;
    }
    else
    {
        iSizeCount=iDataSize+10;                                                //從index=10的兩個Byte開始才是資料內容
        ByteData=new BYTE[iSizeCount];

        //*************Header*************
        ByteData[0]=HIBYTE(iID);                                                //識別碼
        ByteData[1]=LOBYTE(iID);
        ByteData[2]=0;                                                          //通訊方式 0:Modbus TCP
        ByteData[3]=0;
        ByteData[4]=0;                                                          //Length
        ByteData[5]=iDataSize + 4                                               /*Targer Parameter:: Address:1 Byte  Function Code:1 Byte Start register addr:2 Byte*/;
        //*************Header*************
        ByteData[6]=iSlave;                                                     //Slave Address
        ByteData[7]=iFunctionCode;                                              //Function Code
        ByteData[8]=HIBYTE(iAddr);                                              //Start register addr
        ByteData[9]=LOBYTE(iAddr);

        for(int i=0; i<iDataSize; i++)
        {
            ByteData[i+10]=data[i];                                             //從header之後(index=10)開始填入Data
        }
        try
        {
            pClinetSocket->Socket->SendBuf(ByteData, iDataSize+10);
        }
        catch(...)
        {}
        delete []ByteData;
        return true;
    }
}
//------------------------------------------------------------------------------
bool ModbusTCPClient::GetRecevie(int iID, VTBYTEDATA &data, bool bRemove)
{
    if(mapReplyData.size()==0)
        return false;

    if(mapReplyData.find(iID)==mapReplyData.end())
    {                                                                           //還沒收到或沒回應
        return false;
    }
    else
    {
        data=mapReplyData[iID];
        if(bRemove)                                                             //刪除
            vtEarseData.push_back(iID);                                         //刪除動作隔開避免執行緒互搶
        return true;
    }
}
//------------------------------------------------------------------------------
void __fastcall ModbusTCPClient::SocketConnect(TObject *Sender, TCustomWinSocket *Socket)
{                                                                               //預留
    bConnected=true;
}
//------------------------------------------------------------------------------
void __fastcall ModbusTCPClient::SocketDisConnect(TObject *Sender, TCustomWinSocket *Socket)
{                                                                               //預留
    bConnected=false;
}
//------------------------------------------------------------------------------
void __fastcall ModbusTCPClient::SocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    Socket->Disconnect(Socket->RemotePort);
    Sleep(1000);
    bConnected=false;
    ErrorCode=0;
//    Connect();
}
//------------------------------------------------------------------------------
void __fastcall ModbusTCPClient::SocketRead(TObject *Sender, TCustomWinSocket *Socket)
{
    BYTE* ByteData;
    vector<BYTE> data;
    int iID;
    int iDataLength=Socket->ReceiveLength();
    if(iDataLength<=6)
    {                                                                           //長度不足
        return;
    }
    int iDataSize;
    int iTextDataSize;
    ByteData = new BYTE[iDataLength];
    Socket->ReceiveBuf( ByteData , iDataLength );

    iDataSize=ByteData[4]*256+ByteData[5];                                      //檢查資料長度
    if(iDataLength-6/*Header*/==iDataSize)                                      //長度正確
    {
        iTextDataSize=ByteData[8];                                              //內文Size

        if(iDataLength-9 ==iTextDataSize)
        {
            iID=ByteData[0]*256+ByteData[1];

            for(int i=9; i<iDataLength; i++)
                data.push_back(ByteData[i]);

            if(mapReplyData.find(iID)==mapReplyData.end())                      //沒重複
            {
                EnterCriticalSection(&cs);
                mapReplyData.insert(make_pair(iID,data));                       //加入
                LeaveCriticalSection(&cs);
            }
        }
    }
    delete []ByteData;
}
//------------------------------------------------------------------------------
