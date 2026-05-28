#include "MachineDefine.h"
#pragma hdrstop

#include "Socket_ASE_KR.h"

#include "MachineType.h"
#include "auto9045.h"
#include "cprod.h"
#include "mymessbox.h"
#include "common.h"
#include "uLotInfo.h"
#include "cmydef.h"
#include "myQwertyKeyBoard.h"
using namespace std;

#ifdef HT9045
    #include "cMyDB.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfSocketCommunication *fSocketCommunication;
//---------------------------------------------------------------------------
class sTesterData
{
    private:
        AnsiString SendData;
    public:
        //ChungHung 20130204 add
        sTesterData(int Category, int Num, AnsiString msg)
        {
            SendData.sprintf("<MESSAGE category=%c%d%c number=%c%d%c>%s</MESSAGE>",'"',Category,'"','"',Num,'"',msg);
        };
        AnsiString GetString()
        {
            return SendData;
        };
};
AnsiString unKnowMessageID="";

TDateTime StartTime;
TQPF_Timer ServerTimerOut;
//---------------------------------------------------------------------------
__fastcall TfSocketCommunication::TfSocketCommunication(TComponent* Owner)
    : TForm(Owner)
{
    bWrongHandlerID=false;
    bWrongFormat=false;
    bUnknownMessage=false;

    //bDoStopOrOneCycle=false;
    bStopShowAlarm=false;
    ReadData=""; //ChungHung 20130204 add
    bOneCycleFormServer=false;
    bStopFormServer=false;
    bSendBinMapReport=false;
    bWaitServerRely=false;

    //bDisconnected=true;     //ASE_KR 又改方式。。。。OOXX
    bDisconnected=false;

    bNeedShow=true;
    bConnecting=false;
    iInitial=1;
    iBinMap=1;
    iEnd=1;

    ReadLastSetIni();
    ClientSocket1->Host=IniConfig.SocketIP;
    ClientSocket1->Port=atoi(IniConfig.SocketPort.c_str());

    //ASE_KR 又改方式。。。。OOXX
    //bTryToConnect=false;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication::Socket_ASE_KR_Open(AnsiString Ip, AnsiString Port, bool bshow)
{
    //ASE_KR 又改方式。。。。OOXX
    //if(bTryToConnect==false)
    //    return true;

    if(fSocketCommunication->bNeedShow==false)
        return false;
    if(ClientSocket1->Socket->Connected==true)
    {
        //ASE_KR 又改方式。。。。OOXX
        //bTryToConnect=false;
        return true;
    }
    if(bConnecting==true)
        return false;

    bConnecting=true;

    //if(AnsiString(ClientSocket1->Host)!=Ip || AnsiString(ClientSocket1->Port)!=atoi(Port.c_str()));
    {
        ClientSocket1->Host=Ip;
        ClientSocket1->Port=atoi(Port.c_str());
        ClientSocket1->Open();
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::InitialASEKrSocket()
{
    iInitial=1;
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::InitSendCurrentStatus()
{
    iEnd=1;
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::InitBinMapReport()
{
    iBinMap=1;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication::InitialzedHandler()
{
    int &Task=iInitial;
    int ret;
    if(bStopShowAlarm)
        return false;
    switch(Task)
    {
        case 1:
            if(C1N1()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=100;
            }
            break;
        case 100:
            if(bWaitServerRely==true)
            {

                if(ServerTimerOut.Off())
                {
                    //秀出錯誤
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        //Task=1;
                        InitialASEKrSocket();
                    }
                    else if(ret==2)
                    {
                        try
                        {
                            ClientSocket1->Close();
                        }
                        catch(...)
                        {
                            LogClientSocketExceptionError(ClientSocket1, "InitialzedHandler100");   //Steven 20231113 : 紀錄網路連線例外
                        }
                        IniConfig.bEnableSocketCommunication=false;
                        Task=1;
                    }
                    Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=200;
                }
            }
            break;
        case 200:
            SetHandlerState(2); //0:DOWN 1:RUN 2:IDLE
            if(C1N3()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=300;
            }
            break;
        case 300:
            if(bWaitServerRely==true)
            {
                if(ServerTimerOut.Off())
                {
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        //Task=1;
                        InitialASEKrSocket();
                    }
                    else if(ret==2)
                    {
                        try
                        {
                            ClientSocket1->Close();
                        }
                        catch(...)
                        {
                            LogClientSocketExceptionError(ClientSocket1, "InitialzedHandler300");   //Steven 20231113 : 紀錄網路連線例外
                        }
                        IniConfig.bEnableSocketCommunication=false;
                        //Task=1;
                    }
                    Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=400;
                }
            }
            break;
        case 400:
            return true;
//            break;
        case 1000:
            if(C9N3()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=1100;
            }
            break;
        case 1100:
            if(bWaitServerRely==true)
            {
                if(ServerTimerOut.Off())
                {
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        //Task=1;
                        InitialASEKrSocket();
                    }
                    else if(ret==2)
                    {
                        try
                        {
                            ClientSocket1->Close();
                        }
                        catch(...)
                        {
                            LogClientSocketExceptionError(ClientSocket1, "InitialzedHandler1100");  //Steven 20231113 : 紀錄網路連線例外
                        }
                        IniConfig.bEnableSocketCommunication=false;
                        //Task=1;
                    }
                    Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=1;
                }
                //return ;
            }
            break;
        case 2000:
            if(C9N5()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=2100;
            }
            break;
        case 2100:
            if(bWaitServerRely==true)
            {
                if(ServerTimerOut.Off())
                {
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        //Task=1;
                        InitialASEKrSocket();
                    }
                    else if(ret==2)
                    {
                        try
                        {
                            ClientSocket1->Close();
                        }
                        catch(...)
                        {
                            LogClientSocketExceptionError(ClientSocket1, "InitialzedHandler2100");  //Steven 20231113 : 紀錄網路連線例外
                        }
                        IniConfig.bEnableSocketCommunication=false;
                        //Task=1;
                    }
                    Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=1;
                }
                //return ;
            }
            break;
        case 3000:
            if(C9N1()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=3100;
            }
            break;
        case 3100:
            if(bWaitServerRely==true)
            {
                if(ServerTimerOut.Off())
                {
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        //Task=1;
                        InitialASEKrSocket();
                    }
                    else if(ret==2)
                    {

                        IniConfig.bEnableSocketCommunication=false;
                        //Task=1;
                    }
                    Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=1;
                }
                //return ;
            }
            break;
    }

    return false;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication::BinMapReport()
{
    int &Task=iBinMap;
    int ret;

    if(bStopShowAlarm)
        return false;

    switch(Task)
    {
        case 1:
            if(C6N1()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=100;
            }
            break;
        case 100:
            if(bWaitServerRely==true)
            {
                if(ServerTimerOut.Off())
                {
                    //秀出錯誤
                    try
                    {
                        ClientSocket1->Close();
                    }
                    catch(...)
                    {
                        LogClientSocketExceptionError(ClientSocket1, "BinMapReport100");        //Steven 20231113 : 紀錄網路連線例外
                    }
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        //Task=1;
                        InitialASEKrSocket();
                    }
                    else if(ret==2)
                    {
                        try
                        {
                            ClientSocket1->Close();
                        }
                        catch(...)
                        {
                            LogClientSocketExceptionError(ClientSocket1, "BinMapReport100-2");  //Steven 20231113 : 紀錄網路連線例外
                        }
                        IniConfig.bEnableSocketCommunication=false;
                        Task=1;
                    }
                    Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=200;
                }
                //Task=1;
                //return true;
            }
            break;
        case 200:
            Task=1;
            return true;
        case 1000:
            if(C9N3()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=1100;
            }
            break;
        case 1100:
            if(bWaitServerRely==true)
            {
                if(ServerTimerOut.Off())
                {
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        //Task=1;
                        InitialASEKrSocket();
                    }
                    else if(ret==2)
                    {
                        try
                        {
                            ClientSocket1->Close();
                        }
                        catch(...)
                        {
                            LogClientSocketExceptionError(ClientSocket1, "BinMapReport1100");   //Steven 20231113 : 紀錄網路連線例外
                        }
                        IniConfig.bEnableSocketCommunication=false;
                        //Task=1;
                    }
                    Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=1;
                }
                //return ;
            }
            break;
        case 2000:
            if(C9N5()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=2100;
            }
            break;
        case 2100:
            if(bWaitServerRely==true)
            {
                if(ServerTimerOut.Off())
                {
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        //Task=1;
                        InitialASEKrSocket();
                    }
                    else if(ret==2)
                    {
                        try
                        {
                            ClientSocket1->Close();
                        }
                        catch(...)
                        {
                            LogClientSocketExceptionError(ClientSocket1, "BinMapReport2100");   //Steven 20231113 : 紀錄網路連線例外
                        }
                        IniConfig.bEnableSocketCommunication=false;
                        //Task=1;
                    }
                    Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=1;
                }
                //return ;
            }
            break;
        case 3000:
            if(C9N1()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=3100;
            }
            break;
        case 3100:
            if(bWaitServerRely==true)
            {
                if(ServerTimerOut.Off())
                {
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        //Task=1;
                        InitialASEKrSocket();
                    }
                    else if(ret==2)
                    {
                        try
                        {
                            ClientSocket1->Close();
                        }
                        catch(...)
                        {
                            LogClientSocketExceptionError(ClientSocket1, "BinMapReport3100");   //Steven 20231113 : 紀錄網路連線例外
                        }
                        IniConfig.bEnableSocketCommunication=false;
                        Task=1;
                    }
                    Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=1;
                }
                //Task=1;
                //return ;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication::SendCurrentStatus()
{
    int &Task=iEnd;
    int ret;

    if(bStopShowAlarm)
        return false;

    switch(Task)
    {
        case 1:
            if(C1N3()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=100;
            }
            break;
        case 100:
            if(bWaitServerRely==true)
            {
                if(ServerTimerOut.Off())
                {
                    //秀出錯誤
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        InitialASEKrSocket();

                    }
                    else if(ret==2)
                    {
                        try
                        {
                            ClientSocket1->Close();
                        }
                        catch(...)
                        {
                            LogClientSocketExceptionError(ClientSocket1, "SendCurrentStatus100");   //Steven 20231113 : 紀錄網路連線例外
                        }
                        IniConfig.bEnableSocketCommunication=false;
                        //Task=1;
                    }
                    Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=200;
                }
                //return true;
            }
            break;
        case 200:
            Task=1;
            return true;
        case 1000:
            if(C9N3()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=1100;
            }
            break;
        case 1100:
            if(bWaitServerRely==true)
            {
                if(ServerTimerOut.Off())
                {
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        Task=1;
                        InitialASEKrSocket();
                    }
                    else if(ret==2)
                    {
                        try
                        {
                            ClientSocket1->Close();
                        }
                        catch(...)
                        {
                            LogClientSocketExceptionError(ClientSocket1, "SendCurrentStatus1100");  //Steven 20231113 : 紀錄網路連線例外
                        }
                        IniConfig.bEnableSocketCommunication=false;
                        Task=1;
                    }
                    //Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=1;
                }
                //Task=1;
                //return ;
            }
            break;
        case 2000:
            if(C9N5()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=2100;
            }
            break;
        case 2100:
            if(bWaitServerRely==true)
            {
                if(ServerTimerOut.Off())
                {
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        //Task=1;
                        InitialASEKrSocket();
                    }
                    else if(ret==2)
                    {
                        try
                        {
                            ClientSocket1->Close();
                        }
                        catch(...)
                        {
                            LogClientSocketExceptionError(ClientSocket1, "SendCurrentStatus2100");  //Steven 20231113 : 紀錄網路連線例外
                        }
                        IniConfig.bEnableSocketCommunication=false;
                        //Task=1;
                    }
                    Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=1;
                }
                //return ;
            }
            break;
        case 3000:
            if(C9N1()==true)
            {
                ServerTimerOut.SetSecAndOn(IniConfig.SocketTimeOut);
                Task=3100;
            }
            break;
        case 3100:
            if(bWaitServerRely==true)
            {
                if(ServerTimerOut.Off())
                {
                    ReadData=""; //ChungHung 20130204 add

                    ret=ShowMyMessageBox_ASEK("Socket Server Time Out!!!","Message Form Server");

                    if(ret==1)
                    {
                        bStopShowAlarm=true;
                    }
                    else if(ret==0)
                    {
                        //Task=1;
                        InitialASEKrSocket();
                    }
                    else if(ret==2)
                    {
                        try
                        {
                            ClientSocket1->Close();
                        }
                        catch(...)
                        {
                            LogClientSocketExceptionError(ClientSocket1, "SendCurrentStatus3100");  //Steven 20231113 : 紀錄網路連線例外
                        }
                        IniConfig.bEnableSocketCommunication=false;
                        //Task=1;
                    }
                    Task=1;
                }
            }
            else
            {
                if(bWrongHandlerID)
                {
                    bWrongHandlerID=false;
                    Task=1000; //C9N3
                }
                else if(bWrongFormat)
                {
                    bWrongFormat=false;
                    Task=2000; //C9N5
                }
                else if(bUnknownMessage)
                {
                    bUnknownMessage=false;
                    Task=3000; //C9N1
                }
                else
                {
                    Task=1;
                }
                //return ;
            }
            break;

    }
    return false;
}
//---------------------------------------------------------------------------
//ChungHung 20130204 add
void __fastcall TfSocketCommunication:: ShowMemo(AnsiString data,bool bState)
{
    AnsiString str="";
    AnsiString strchr="";
    AnsiString str1="";
    byte buf;
    for(int i=1;i<=data.Length();i++)
    {
        str1.sprintf("%02x",data[i]);
        str+=str1+" ";
        buf=data[i];
        if(buf>=32 && buf<=126)
            strchr+=(char)buf;
        else
            strchr+=" .";
    }
    AnsiString logstr;
    if(bState==false)
    {
        Memo->Lines->Add("Send:"+DateTimeToStr(Now()));
        fLotInfo->Memo1->Lines->Add("Send:"+DateTimeToStr(Now()));
        logstr="Send:"+DateTimeToStr(Now());
    }
    else
    {
        Memo->Lines->Add("Receive:"+DateTimeToStr(Now()));
        fLotInfo->Memo1->Lines->Add("Receive:"+DateTimeToStr(Now()));
        logstr="Receive:"+DateTimeToStr(Now());
    }

    Memo->Lines->Add(str);
    Memo->Lines->Add(strchr);
    Memo->Lines->Add("");
    AnsiString filelog;
    filelog="D:\\HT9045\\SocketLog\\"+FormatDateTime("\\yyyy-mm-dd",TDateTime::CurrentDate())+".txt";

    fstream fp;
    fp.open(filelog.c_str(), ios::out|ios::ate);//開啟檔案
    if(fp)
    {
        fp<<logstr.c_str()<<endl;
        fp<<strchr.c_str()<<endl;//寫入字串
        fp.close();//關閉檔案
    }

    if(Memo->Lines->Count>1000)
    {
        Memo->Lines->Clear();
    }
    fLotInfo->Memo1->Lines->Add(str);
    fLotInfo->Memo1->Lines->Add(strchr);
    fLotInfo->Memo1->Lines->Add("");
    if(fLotInfo->Memo1->Lines->Count>1000)
        fLotInfo->Memo1->Lines->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication:: SetHandlerState(int iState)
{
    //0:DOWN 1:RUN 2:IDLE
    switch(iState)
    {
        case 0:
            aSetHandlerState="DOWN";
            break;
        case 1:
            aSetHandlerState="RUN";
            break;
        case 2:
            aSetHandlerState="IDLE";
            break;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication:: C1N1()
{
    if(bDisconnected==true)
    {
        //ASE_KR 又改方式。。。。OOXX
        //bTryToConnect=true;
        return false;
    }
    if(ClientSocket1->Socket->Connected==false)
        return false;

    bWaitServerRely=true;
    //ChungHung 20130204 add
    AnsiString str="";
    str.sprintf("<HID>%s</HID>",IniConfig.SocketHandlerID);
    sTesterData sData(1,1,str);
    ShowMemo(sData.GetString());
    ClientSocket1->Socket->SendText(sData.GetString());
    iBackCategory=1;
    iBackNum=1;

    /*  //ChungHung 20130204 delete
    AnsiString str="";
    str.sprintf("%c%c%s",0x41,TesterID.Length(),TesterID);
    sTesterData sData(1,1,(byte*)str.c_str(),str.Length());
    ShowMemo(sData.GetStream());
    ClientSocket1->Socket->SendStream(sData.GetStream());
    */
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication:: C1N3()
{
    if(bDisconnected==true)
    {
        //ASE_KR 又改方式。。。。OOXX
        //bTryToConnect=true;
        return false;
    }
    if(ClientSocket1->Socket->Connected==false)
        return false;

    //ChungHung 20130204 add
    AnsiString str="";
    AnsiString aState="";
    //if(aState!=aHandlerState || bNoChangeStateNeedSend)
    //{
        bWaitServerRely=true;
        aState=aSetHandlerState;
        str.sprintf("<HID>%s</HID><STAT>%s</STAT>",IniConfig.SocketHandlerID,aState);
        sTesterData sData(1,3,str);
        ShowMemo(sData.GetString());
        ClientSocket1->Socket->SendText(sData.GetString());
        iBackCategory=1;
        iBackNum=3;
    //}
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication:: C6N1()
{
    if(bDisconnected==true)
    {
        //ASE_KR 又改方式。。。。OOXX
        //bTryToConnect=true;
        return false;
    }
    if(ClientSocket1->Socket->Connected==false)
        return false;

    bWaitServerRely=true;

    //ChungHung 20130204 add
    int row,col;
    GetTotalSite_ASE_KR(row,col);
    AnsiString str="";
    AnsiString substr="";
    substr=GetTestResult_ASE_KR(row,col);
    str.sprintf("<HID>%s</HID>%s",IniConfig.SocketHandlerID,substr);
    sTesterData sData(6,1,str);
    ShowMemo(sData.GetString());
    ClientSocket1->Socket->SendText(sData.GetString());
    iBackCategory=6;
    iBackNum=1;

    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication:: C9N1()
{
    if(bDisconnected==true)
    {
        //ASE_KR 又改方式。。。。OOXX
        //bTryToConnect=true;
        return false;
    }
    if(ClientSocket1->Socket->Connected==false)
        return false;

    bWaitServerRely=true;
    //ChungHung 20130204 add
    AnsiString str="";
    //unKnowMessageID="C7N7";
    if(unKnowMessageID.IsEmpty())
        str.sprintf("<HID>%s</HID><MSGID></MSGID>",IniConfig.SocketHandlerID);
    else
        str.sprintf("<HID>%s</HID><MSGID>%s</MSGID>",IniConfig.SocketHandlerID,unKnowMessageID);
    sTesterData sData(9,1,str);
    ShowMemo(sData.GetString());
    ClientSocket1->Socket->SendText(sData.GetString());
    iBackCategory=9;
    iBackNum=1;
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication:: C9N3()
{
    if(bDisconnected==true)
    {
        //ASE_KR 又改方式。。。。OOXX
        //bTryToConnect=true;
        return false;
    }
    if(ClientSocket1->Socket->Connected==false)
        return false;

    bWaitServerRely=true;
    //ChungHung 20130204 add
    AnsiString str="";
    if(ReceivedHandler.IsEmpty())
        str.sprintf("<HID>%s</HID><RCVHID></RCVHID>",IniConfig.SocketHandlerID);
    else
        str.sprintf("<HID>%s</HID><RCVHID>%s</RCVHID>",IniConfig.SocketHandlerID,ReceivedHandler);
    sTesterData sData(9,3,str);
    ShowMemo(sData.GetString());
    ClientSocket1->Socket->SendText(sData.GetString());
    iBackCategory=9;
    iBackNum=3;
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication:: C9N5()
{
    if(bDisconnected==true)
    {
        //ASE_KR 又改方式。。。。OOXX
        //bTryToConnect=true;
        return false;
    }
    if(ClientSocket1->Socket->Connected==false)
        return false;

    bWaitServerRely=true;

    //ChungHung 20130204 add
    AnsiString str="";
    if(unKnowMessageID.IsEmpty())
        str.sprintf("<HID>%s</HID><MSGID></MSGID>",IniConfig.SocketHandlerID);
    else
        str.sprintf("<HID>%s</HID><MSGID>%s</MSGID>",IniConfig.SocketHandlerID,unKnowMessageID);
    sTesterData sData(9,5,str);
    ShowMemo(sData.GetString());
    ClientSocket1->Socket->SendText(sData.GetString());
    iBackCategory=9;
    iBackNum=5;

    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::btnC1N1Click(TObject *Sender)
{
    C1N1();
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::btnC1N3Click(TObject *Sender)
{
    C1N3();
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::btnC6N1Click(TObject *Sender)
{
    C6N1();
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::btnC9N1Click(TObject *Sender)
{
    C9N1();
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::btnC9N3Click(TObject *Sender)
{
    C9N3();
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::btnC9N5Click(TObject *Sender)
{
    C9N5();
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::ClientSocket1Error(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    static bool bEnter=false;

    if(bEnter==true)
        return;

    bEnter=true;
    OnLine->Enabled=false;
    fLotInfo->OnLine->Enabled=false;
    if(bNeedShow==false)
    {
        ErrorCode=0;
        bEnter=false;
        return;
    }

    int ret=ShowMyMessageBox_ASEK("Socket Server Error : "+GetErrorMsg(Sender, ErrorCode), "Message Form Server");
    if(ret==1)
    {
        bNeedShow=false;
    }
    else if(ret==0)
    {

    }
    else if(ret==2)
    {
        IniConfig.bEnableSocketCommunication=false;
    }

    Memo->Lines->Add(GetErrorMsg(Sender, ErrorCode));
    try
    {
        if(ErrorCode==10053)   // 網路線拔除
        {
            Socket->Close();
        }
        else if(ErrorCode==10061) //連線失敗
        {
            Socket->Close();
        }
        else
        {
            Socket->Close();
        }
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "ASE_K Socket");                  //Steven 20231113 : 紀錄網路連線例外
    }
    ErrorCode=0;
    //秀出錯誤
    //ClientSocket1->Close();
    InitBinMapReport();
    InitSendCurrentStatus();
    ReadData=""; //ChungHung 20130204 add
    bConnecting=false;
    bDisconnected=false;

    bEnter=false;
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::ClientSocket1Connect(
      TObject *Sender, TCustomWinSocket *Socket)
{
    OnLine->Enabled=true;
    fLotInfo->OnLine->Enabled=true;

    fLotInfo->Memo1->Lines->Add("Connect");
    bDisconnected=false;
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::ClientSocket1Disconnect(
      TObject *Sender, TCustomWinSocket *Socket)
{

    OnLine->Enabled=false;
    fLotInfo->OnLine->Enabled=false;

    bConnecting=false;
    //bWaitServerRely=false;
    //Memo->Lines->Add("Disconnect");
    fLotInfo->Memo1->Lines->Add("Disconnect");
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication::GetCategoryAndNum(int &Category,int &Num)
{
    //ChungHung 20130204 add
    AnsiString aCategory;
    AnsiString aNumber;
    AnsiString aNumberEnd;
    AnsiString aEnd;

    //aCategory.sprintf("<MESSAGE category=%c",'"');
    aCategory.sprintf("category=%c",'"');
    aNumber.sprintf("%c number=%c",'"','"');
    aNumberEnd.sprintf("%c>",'"');

    AnsiString temp1;
    AnsiString temp2;

    int iCategory=ReadData.Pos(aCategory);
    int iCategoryLeng=aCategory.Length();
    int iNumber=ReadData.Pos(aNumber);
    int iNumberLeng=aNumber.Length();
    int iNumberEnd=ReadData.Pos(aNumberEnd);

    if(iCategory!=0 && iNumber!=0)
    {
        if(iNumber>iCategory)
        {
            temp1 =ReadData.SubString((iCategory)+iCategoryLeng , iNumber-((iCategory)+iCategoryLeng));
            temp2 =ReadData.SubString((iNumber)+iNumberLeng ,iNumberEnd-((iNumber)+iNumberLeng));
            Category=atoi(temp1.c_str());
            Num=atoi(temp2.c_str());
            return false;
        }
    }

    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication::GetDataFinish(int &Category,int &Num)
{
    //ChungHung 20130204 add
    AnsiString aCategory;
    AnsiString aNumber;
    AnsiString aNumberEnd;
    AnsiString aEnd;

    //aCategory.sprintf("<MESSAGE category=%c",'"');
    aCategory.sprintf("category=%c",'"');
    aNumber.sprintf("%c number=%c",'"','"');
    aNumberEnd.sprintf("%c>",'"');
    aEnd="</MESSAGE>";

    AnsiString temp1;
    AnsiString temp2;

    int iCategory=ReadData.Pos(aCategory);
    int iCategoryLeng=aCategory.Length();
    int iNumber=ReadData.Pos(aNumber);
    int iNumberLeng=aNumber.Length();
    int iNumberEnd=ReadData.Pos(aNumberEnd);
    int iEnd=ReadData.Pos(aEnd);

    if(iCategory!=0 && iNumber!=0 && iEnd!=0)
    {
        if(iEnd>iNumber && iNumber>iCategory)
        {
            temp1 =ReadData.SubString((iCategory)+iCategoryLeng , iNumber-((iCategory)+iCategoryLeng));
            temp2 =ReadData.SubString((iNumber)+iNumberLeng ,iNumberEnd-((iNumber)+iNumberLeng));
            Category=atoi(temp1.c_str());
            Num=atoi(temp2.c_str());
            return true;
        }
    }

    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::ClientSocket1Read(TObject *Sender,
      TCustomWinSocket *Socket)
{
    int len=0;
    len=Socket->ReceiveLength();
    if(len<=0)
        return;

    ReadData+=Socket->ReceiveText();
    //if(GetDataFinish(Category,Num))
    {
        ShowMemo(ReadData,true);
        CommandProcess();
        ReadData="";
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication::IsUnknownMessage(int Category,int Num)
{
    if(iBackCategory!=Category)
    {
        unKnowMessageID="";
        unKnowMessageID.sprintf("C%dN%d",Category,Num);
        return true;
    }
    if(iBackNum!=Num)
        return false;

    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication::IsWrongFormat(int Category,int Num)
{
    AnsiString MessageStructure;
    MessageStructure.sprintf("<MESSAGE category=%c?%c number=%c?%c><HID>?</HID><ACK>?</ACK><MSG>?</MSG></MESSAGE>",'"','"','"','"');

    AnsiString tmp1,tmp2;

    int j=1;
    int i=1;
    bool bByPass=false;

    do{
        if(MessageStructure.IsDelimiter("?",j))
        {
            j++;
            bByPass=true;
        }
        if(MessageStructure[j]==ReadData[i])
        {
            bByPass=false;
            j++;
            i++;
        }
        else if(bByPass)
        {
            i++;
        }
        else
        {
            break;
        }
    }while(i<=ReadData.Length());

    if((j-1)!=MessageStructure.Length())
    {
        unKnowMessageID="";
        unKnowMessageID.sprintf("C%dN%d",Category,Num);
        return true;
    }

    return false;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication::IsWrongHandlerID()
{
    int iStart=ReadData.Pos("<HID>");
    int iEnd=ReadData.Pos("</HID>");
    ReceivedHandler=ReadData.SubString((iStart)+5,iEnd-((iStart)+5));
    if(ReceivedHandler!=IniConfig.SocketHandlerID )
    {
        return true;
    }

    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::GetErrorMSG()
{
    int iStart=ReadData.Pos("<MSG>");
    int iEnd=ReadData.Pos("</MSG>");
    ErrorMessage=ReadData.SubString((iStart)+5,iEnd-((iStart)+5));
    if(ErrorMessage.IsEmpty())
        ErrorMessage=" ";
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication::GetACK()
{
    AnsiString aACK;
    int iStart=ReadData.Pos("<ACK>");
    int iEnd=ReadData.Pos("</ACK>");
    aACK=ReadData.SubString((iStart)+5,iEnd-((iStart)+5));

    if(aACK.UpperCase()=="OK")
    {
        return false;
    }
    else if(aACK.UpperCase()=="PAUSE")
    {
        //PAUSE form Server
        SoftStop=false;
        SoftStart=false;
        SystemStart=false;
        GetErrorMSG();
        //秀出錯誤
        bStopFormServer=true;
    }
    else if(aACK.UpperCase()=="ONECYCLE")
    {
        //ONECYCLE form Server
        GetErrorMSG();
        if(aCurrentHandlerState=="RUN")
        {
            DoOneCycle();
            bOneCycleFormServer=true;
        }
        else
        {
            bOneCycleFormServerButNoRunJustShow=true;
        }
    }
    else
    {
        bWrongFormat=true;
    }

    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TfSocketCommunication::CommandProcess()
{
    int Category=0;
    int Num=0;
    GetCategoryAndNum(Category,Num);

    if(IsWrongFormat(Category,Num))
    {
        bWrongFormat=true;
    }
    else if(IsWrongHandlerID())
    {
        bWrongHandlerID=true;
    }
    else if(IsUnknownMessage(Category,Num))
    {
        bUnknownMessage=true;
    }

    if(bWrongHandlerID==false && bWrongFormat==false && bUnknownMessage==false)
    {
        bDoStopOrOneCycle=GetACK();
    }

    //bDisconnected=true;  //ASE_KR 又改方式。。。。OOXX
    bWaitServerRely=false;
    //MySleep(100);

    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::FormShow(TObject *Sender)
{
    edtSocketTimeout->Text=IniConfig.SocketTimeOut;
    AnsiString aSocketIP=IniConfig.SocketIP;

    TStringList* TempList = new TStringList();
    TempList->Text=StringReplace(aSocketIP,".","\n",TReplaceFlags()<<rfReplaceAll);
    edtServerIP_1->Text=TempList->Strings[0];
    edtServerIP_2->Text=TempList->Strings[1];
    edtServerIP_3->Text=TempList->Strings[2];
    edtServerIP_4->Text=TempList->Strings[3];
    TempList->Clear();  //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete TempList;
    edtServerPort->Text=IniConfig.SocketPort;
    cbEnableSocket->Checked=IniConfig.bEnableSocketCommunication;
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::FormDestroy(TObject *Sender)
{
    try
    {
        ClientSocket1->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "ASE_K FormDestroy");             //Steven 20231113 : 紀錄網路連線例外
    }
    LogSoftwareOffTime("TfSocketCommunication, FormDestroy");                   //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::ClientSocket1Connecting(
      TObject *Sender, TCustomWinSocket *Socket)
{
    bConnecting=true;
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::btnOKClick(TObject *Sender)
{
    IniConfig.bEnableSocketCommunication=cbEnableSocket->Checked;
    IniConfig.SocketTimeOut=atoi(edtSocketTimeout->Text.c_str());
    IniConfig.SocketIP=edtServerIP_1->Text+"."+edtServerIP_2->Text+"."+edtServerIP_3->Text+"."+edtServerIP_4->Text;
    IniConfig.SocketPort=edtServerPort->Text;
    SaveLastSetIni();
    try
    {
        ClientSocket1->Close();
        ClientSocket1->Host=IniConfig.SocketIP;
        ClientSocket1->Port=atoi(IniConfig.SocketPort.c_str());
    }
    catch(...)
    {
        LogClientSocketExceptionError(ClientSocket1, "btnOKClick");             //Steven 20231113 : 紀錄網路連線例外
    }
    InitialASEKrSocket();

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::Button1Click(TObject *Sender)
{
    try
    {
        ClientSocket1->Open();
    }
    catch(...)
    {
        LogClientSocketExceptionError(ClientSocket1, "Button1Click");           //Steven 20231113 : 紀錄網路連線例外
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::Button2Click(TObject *Sender)
{
    try
    {
        ClientSocket1->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(ClientSocket1, "Button2Click");           //Steven 20231113 : 紀錄網路連線例外
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::btnCancelClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::edtServerPortClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::edtServerIP_1Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0, 255);
}
//---------------------------------------------------------------------------
void __fastcall TfSocketCommunication::edtSocketTimeoutClick(
      TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10, 3000);
}
//---------------------------------------------------------------------------

