#include "MachineDefine.h"
#pragma hdrstop
#include "myQwertyKeyBoard.h"
#include "automation.h"
#include "MachineType.h"
#include "cMydef.h"
#include "cProd.h"
#include "common.h"
#include "main.h"

#ifdef HT7040
    #include "auto7040.h"
#else
    #include "auto9045.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfAutomation *fAutomation;
//Steven 20100830 : For OLP
static bool bStandard=true; //Steven 20110208
bool bRestConnect=false;    //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server
//int iFTPCanUse=10;
AnsiString TCPstr="";
AnsiString sDLFileName="";
AnsiString ReceiveString="";
AnsiString TCPstr2="";
AnsiString ReceiveString2="";
AnsiString STX;
AnsiString SOH;
AnsiString ETX;
AnsiString stx = "[STX]";
AnsiString soh = "[SOH]";
AnsiString etx = "[ETX]";
AnsiString HandlerType;
AnsiString aDataPath;
bool WRITE=false;
bool READ=true;
//Steven 20100830 End

//ChungHung 20101119 Start
const int iMaxEvent = 10;
struct sResult
{
    bool def;
    AnsiString CommandString;
    AnsiString NumString;
    sResult()
    {
        def=false;
        CommandString="";
        NumString="";
    };
}MyEventResult[iMaxEvent];
//---------------------------------------------------------------------------
__fastcall TfAutomation::TfAutomation(TComponent* Owner)
    : TForm(Owner)
{
    try
    {
        STX.sprintf("%c",0x02);
        SOH.sprintf("%c",0x01);
        ETX.sprintf("%c",0x03);
        OLPClientConnected=false;
        if(CUSTOMER_CODE==CC_ASE_CL)
        {
            bStandard=true;     //Steven 20110208
            OLPServer->Open();
            gbAutomation->Visible=true;
        }
        else if(CUSTOMER_CODE==CC_HTML_Monitor ||
                CUSTOMER_CODE==CC_Greatek ||                                    //Jimmychiu 20260318 : 超豐加入透過Socket遠端控制
                CUSTOMER_CODE==CC_MTI ||
                CUSTOMER_CODE==CC_PTI)                                          //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            OLPServer->Port=6671;                                               //Sam 20210812 : PTI 強制寫死6671
            #ifdef SOFT_SIMULTE
            OLPServer->Port=5001;
            #endif
            bStandard=false;
            OLPServer->Open();
            gbAutomation->Visible=true;
        }
        else
        {
            bStandard=true;
            OLPServer->Close();
            gbAutomation->Visible=false;
        }

        btUDPSend->Visible=(CosFunction.bAmkorFunction || CUSTOMER_CODE==CC_QUALCOMM); //Steven 20120330 : ATK Site Map Monitorning  //JerryYang 20170412 (Steven) add QUALCOMM
        btUDPMap ->Visible=(CosFunction.bAmkorFunction || CUSTOMER_CODE==CC_QUALCOMM); //Steven 20120330 : ATK Site Map Monitorning  //JerryYang 20170412 (Steven) add QUALCOMM
        btUDPTemp->Visible=(CosFunction.bAmkorFunction || CUSTOMER_CODE==CC_QUALCOMM); //Steven 20120330 : ATK Site Map Monitorning  //JerryYang 20170412 (Steven) add QUALCOMM

        if(CUSTOMER_CODE==CC_HTML_Monitor ||
           CUSTOMER_CODE==CC_MTI ||
           CUSTOMER_CODE==CC_PTI)    //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            OLPClient->Address=edinputIP->Text;
            OLPClient->Port=atoi(edinputport->Text.c_str());
            OLPClient->Open();
        }
        else
        {
            OLPClient->Close();
        }

        if(CosFunction.bOLPFunction)      //Steven 20141229 : OLP功能
            tmrOLP->Enabled=true;
        else
            tmrOLP->Enabled=false;

        //Steven 20100830 Start: For OLP
        CommandBuffer=new TList(); // 2009.11.13 , Joye
        CommandBuffer->Clear();
        CommandClientReplyBuffer=new TStringList(); //Sam 20200813 : 增加 Log debug
        CommandClientReplyBuffer->Clear();

        GetTimeInfo();
        HandlerType=GetHandlerType();
        aDataPath=GetDataPath();

        bReceive=false;
        bReceive2=false;
        iSocketHandle=0;    //Sam 20200727 : 修正連線問題

        bReceiveHasData = false;    // 2011.09.05 , Joye , OLP
        bOneCycle=false;
        bCleanOut=false;

        TestMode=0;
        for(int i=0; i<256; i++)
        {
            sATKSendMessage[i]='\0';
            sATKSendTemp[i]='\0';
            sATKSendMap[i]='\0';
            sATKSendTempAndMap[i]='\0';
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfAutomation::TfAutomation");
    }
    iClientReConnect=0;         //Sam 20240226 : Client 重連次數
    brecordmsgLock=false;
}
//---------------------------------------------------------------------------
bool TfAutomation::GetEventNum(int &num,AnsiString cmdstr,AnsiString numstr)
{
    for(int i=0; i<iMaxEvent; i++)
    {
        if(MyEventResult[i].CommandString=="")
        {
            num=i;
            SetEventResult(i,false,cmdstr,numstr);
            return true;
        }
    }
    num=-1;
    return false;
}
//---------------------------------------------------------------------------
bool TfAutomation::SetEventResult(int num,bool def,AnsiString cmdstr,AnsiString numstr)
{
    if(num>=0 && num<iMaxEvent)
    {
        MyEventResult[num].def=def;
        MyEventResult[num].CommandString=cmdstr;
        MyEventResult[num].NumString=numstr;

        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfAutomation::GetEventResult(int num)
{
    bool result=false;
    if(num>=0 && num<iMaxEvent)
    {
        result=MyEventResult[num].def;
        //str=MyEventResult[num].GetString;
        return result;
    }
    return result;
}
//---------------------------------------------------------------------------
void TfAutomation::CompareEvent(AnsiString str)
{
    for(int i=0; i<iMaxEvent; i++)
    {
        if(MyEventResult[i].CommandString!="")
        {
            if(str.Pos(MyEventResult[i].CommandString)!=0 &&
               str.Pos(MyEventResult[i].NumString)!=0 || MyEventResult[i].NumString=="")
            {
                SetEventResult(i, true, "", "");
                return;
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfAutomation::ClearEvent(int num)
{
    SetEventResult(num, false, "", "");
    return;
}
//ChungHung 20101119 End
//---------------------------------------------------------------------------
AnsiString SplitDataBySoh(AnsiString &Buffer)
{
    int pos;
    AnsiString Data;
    pos=Buffer.Pos(SOH);
    if(pos==0)
        pos=Buffer.Pos(ETX);

    Data=Buffer.SubString(1, pos-1);
    Buffer.Delete(1, pos);
    return Data;
}
//---------------------------------------------------------------------------
AnsiString GetCRC(AnsiString &Buffer)
{
    AnsiString Data;
    Data=Buffer.SubString(1, 5);
    Buffer.Delete(1, 1);
    return Data;
}
//---------------------------------------------------------------------------
AnsiString __fastcall AddBlock(AnsiString S)
{
    AnsiString S1;
    S1="["+S+"]";
    return S1;
}
//---------------------------------------------------------------------------
bool IsStartWord(AnsiString &Buffer, AnsiString w)
{
    if(Buffer.Pos(w)==1)
    {
        Buffer.Delete(1, 1);
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
AnsiString crc16(char *ip_str, int n)
{
    unsigned  crctmp=65535, ch;
    char str[256];
    AnsiString S;
    for(int i=0; i<n; i++)
    {
        ch=ip_str[i];
        crctmp=crctmp^ch;
        for(int j=0; j<8; j++)
        {
            if((crctmp & 1)==1)
            {
                crctmp=crctmp/2;
                crctmp=crctmp^40961;
            }
            else
            {
                crctmp=crctmp/2;
            }
        }
    }
    sprintf(str, "%5d", crctmp);
    S=str;
    return S;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfAutomation::GetTimeInfo()
{
    static TDateTime dtPresent;
    AnsiString TimeString;
    dtPresent= Now();
    DecodeDate(dtPresent, aSystemYear, aSystemMonth, aSystemDate);
    DecodeTime(dtPresent, aSystemHour, aSystemMin, aSystemSec, aSystemMSec);
    TimeString.sprintf("%04d%02d%02d%02d%02d%02d", aSystemYear, aSystemMonth, aSystemDate, aSystemHour, aSystemMin, aSystemSec);
    return TimeString;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPServerClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server
    //==>
    int iConnectCount=(int)(OLPServer->Socket->ActiveConnections);

    if(CUSTOMER_CODE==CC_PTI)//Sam 20200826 : PTI 允許多重連線
    {
        OnLine->Enabled=true;
        ShowOLPState(1);   // 2009.12.02 , Joye
        if(iConnectCount>1)
        {
            NewRecordProcess("","OLP server is about to reset due to too many connections","iConnectCount="+IntToStr(iConnectCount));
        }
    }
    else
    {
        if(iConnectCount==1)
        {
            OnLine->Enabled=true;
            ShowOLPState(1);   // 2009.12.02 , Joye
        }
        else
        {
            NewRecordProcess("","OLP server is about to reset due to too many connections","iConnectCount="+IntToStr(iConnectCount));
            bRestConnect=true;
            ShowOLPState(0);
        }
    }

    //<==
    //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPServerClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    OnLine->Enabled=false;
    ShowOLPState(0);   // 2009.12.02 , Joye
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPServerClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    ShowMSG(GetErrorMsg(Sender, ErrorCode));
    if(ErrorCode==10053)   // 網路線拔除
    {
        ShowOLPState(2);   // 2009.12.02 , Joye
    }
    else if(ErrorCode==10061) //連線失敗
    {
        ShowOLPState(0);   // 2009.12.02 , Joye
    }
    else
    {
        ShowOLPState(0);
    }
    ErrorCode=0;
    OnLine->Enabled=false;
    try
    {
        OLPServer->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "OLPServer Error");
    }
    OLPServer->Open();
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPServerClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    int StartPos=0,EndPos=0;
    if (ReceiveString.Length() > 1024)                                         //jimmychiu 20251009 : avoid msg too long to dead
    {
        ShowMSG(AnsiString().sprintf("OLPServerClientRead too long to delete, %s",ReceiveString));
        ReceiveString="";
    }
    //接收字串
    ReceiveString+=Socket->ReceiveText();   //Sam 20200727 : 修正連線問題
    iSocketHandle=Socket->SocketHandle ;
    //尋找STX
    StartPos=ReceiveString.Pos(STX);
    //尋找ETX
    EndPos=ReceiveString.Pos(ETX);

    if(bReceive==false && StartPos>0 && EndPos>0 && StartPos<EndPos)
    {
        TCPstr=ReceiveString.SubString(StartPos,EndPos);
        ReceiveString.Delete(1,EndPos);
        bReceive=true;
    }
    else if(ReceiveString.Length()>5000)
    {
        ReceiveString="";
    }

    if(bReceive)
    {
        ProcessBuffer(TCPstr,iSocketHandle); //Sam 20200813 : 增加 Log debug //被動回傳
        bReceive=false;

        // 2011.09.05 , Joye , OLP ------------------------
        if( ReceiveString.Length() > 0)
        {
            bReceiveHasData = true;
        }
        else
            bReceiveHasData = false;
        // ------------------------------------------------
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::CommandProcess(AnsiString CMD, AnsiString V_TOTAL, AnsiString *Data, bool Standard,int iHandle) //Sam 20200813 : 增加 Log debug
{
    AnsiString HEAD, head, SendString, S2, cmd, v_total;

    cmd=AddBlock(CMD);
    v_total=AddBlock(V_TOTAL);

    HEAD=STX+CMD+SOH+V_TOTAL;
    head=stx+cmd+soh+v_total;

    for(int i=0; i<atoi(V_TOTAL.c_str()); i++)
    {
        HEAD=HEAD+SOH+Data[i];
        head=head+soh+AddBlock(Data[i]);
    }
    HEAD+=ETX;
    head+=etx;

    SendString=HEAD;
    S2=head;
    if(Standard==true)
    {
        bReceive=false;
        SendClient(SendString,S2,iHandle);  //Sam 20200813 : 增加 Log debug
    }
    else
    {
        bReceive=false;
        SendServer(SendString,S2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::tmrOLPTimer(TObject *Sender) //主動回傳
{
    static bool bRun=false;
    if(bRun)
        return;
    bRun=true;
    //
    static int TimeCount=0;
    static int bfirst=true;
    AnsiString S="";
    int iConnectCount=0;
    static bool bRecordState=false;
    if(CosFunction.bOLPFunction==false || InitialOK==false)
    {
        bRun=false;
        return;
    }

    if(bfirst)
    {
        fAutomation->edinputIP->Text    =IniConfig.sN08OlpIP;
        if(IniConfig.sN08OlpPort=="")
            IniConfig.sN08OlpPort="6670";
        fAutomation->edinputport->Text  =IniConfig.sN08OlpPort;
        bfirst=false;
    }

    if(SendInvalid==false)
    {
        SendInvalid=true;
        TimeCount=0;
    }

    if(SendInvalid==true && TimeCount>=10000 && OnLine2->Enabled==true)
    {
        SendOLPClient(" "+ETX);
        SendInvalid=false;
        TimeCount=0;
    }
    else if(OnLine2->Enabled==false)
    {
        if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)  //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            if(iClientReConnect<10) //Sam 20240226 : Client 重連次數
            {
                iClientReConnect++;
                OLPClient->Address=edinputIP->Text;
                OLPClient->Port=atoi(edinputport->Text.c_str());
                OLPClient->Open();
            }
        }
    }
    else
    {
        TimeCount++;
    }
    //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server
    //==>
    try
    {
        if(bRestConnect==true && bReceive==false)  //Sam 20200727 : 修正連線問題
        {
            OLPServer->Close();
            OLPServer->Open();
            OnLine->Enabled=false;
            bRestConnect=false;
            bRecordState=false;
            NewRecordProcess("","OLP server reset !!!",""); //Sam 20200826 : Add Log
            bRun=false;
            return;
        }
        else if(bRestConnect==true && bReceive==true && bRecordState==false)//Sam 20200813 : 增加 Log debug
        {
            bRecordState=true;
            NewRecordProcess("","OLP server will reset connect,But client is receiving data!!!","");
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfAutomation::tmrOLPTimer");
    }
    //<==
    //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server

    if(CheckInitialOK()==false)
    {
        bRun=false;
        return;
    }
    AnsiString Data[32], TimeString="";     //Sam 20191126 : 20>32
    AnsiString MainStatus=GetMainStatus();
    MyProcessBuffer *ProcessPtr;
    static int Task=1;
    static MyProcessBuffer Process;
    static AnsiString SysStatus="";
    int DutCount;

    if(SysStatus=="")
        SysStatus=MainStatus;

    if(MainStatus!=SysStatus)
    {
        SendReportRequest("0001", bStandard);
        SysStatus=MainStatus;
    }

    switch(Task)
    {
        case 1:
            if(CommandBuffer->Count!=0)
            {
                if((bStandard==true && OnLine->Enabled==true) || (bStandard==false && OnLine2->Enabled==true))
                {
                    ProcessPtr=(MyProcessBuffer *)CommandBuffer->Items[0];
                    Process.Command     =ProcessPtr->Command;
                    Process.SubCommand  =ProcessPtr->SubCommand;
                    Process.AlarmID     =ProcessPtr->AlarmID;
                    Process.AlarmMessage=ProcessPtr->AlarmMessage;
                    Process.TimeString  =ProcessPtr->TimeString;
                    Process.ErrType     =ProcessPtr->ErrType;
                    CommandBuffer->Delete(0);
                    Task=100;
                }
                else
                {
                    WriteTCPDataToTextFile();                    //2008/08/05 lee
                }
            }
            break;
        case 100:
            if(Process.Command=="ALARM_REQUEST")
            {
                Data[0]=GetTimeInfo();
                Data[1]=Process.AlarmID;
                Data[2]="1";        //Steven 20110802 : Count固定為1
                Data[3]=Process.AlarmMessage;
                CommandProcess(Process.Command, 4, Data, bStandard);
            }
            else if(Process.Command=="MESSAGE_REQUEST")
            {
                Data[0]=GetTimeInfo();
                Data[1]=Process.AlarmMessage;
                CommandProcess(Process.Command, 2, Data, bStandard);
            }
            else if(Process.Command=="PRODUCTION_REQUEST")
            {
                Data[0]=GetTimeInfo();
                Data[1]=Process.AlarmMessage;
                CommandProcess(Process.Command, 2, Data, bStandard);
            }
            else if(Process.Command=="PRODUCTION_REPLY")
            {
                Data[0]=Process.AlarmMessage;     //Brian,20181130
                Data[1]="0";                      //Brian,20181130
                CommandProcess(Process.Command, 2, Data, bStandard);
            }
            else if(Process.Command=="TEST_RESULT_REQUEST")
            {
                DutCount=GetTestResult(Data);
                CommandProcess("TEST_RESULT_REQUEST", DutCount, Data, bStandard);
            }
            Task=1;
            break;
    }

    // 2011.09.05 , Joye , OLP ------------------------>>
    if(bReceiveHasData==true)
    {
        //尋找STX
        int StartPos=ReceiveString.Pos(STX);
        //尋找ETX
        int EndPos=ReceiveString.Pos(ETX);

        if(StartPos!=0 && EndPos!=0 && StartPos<EndPos)
        {
            AnsiString sSubTCPstr = ReceiveString.SubString(StartPos, EndPos);
            ReceiveString.Delete(1,EndPos);

            ProcessBuffer(sSubTCPstr,iSocketHandle); //Sam 20200813 : 增加 Log debug //被動回傳

            if(ReceiveString.Length()>0)
            {
                bReceiveHasData=true;
            }
            else
            {
                bReceiveHasData=false;
            }
        }
    }
    // 2011.09.05 , Joye , OLP ------------------------<<

    if(CommandClientReplyBuffer->Count>=1)                                      //Sam 20200813 : send client data error add retry send
    {
        iConnectCount=(int)(OLPServer->Socket->ActiveConnections);
        if(OnLine->Enabled==true && iConnectCount>=1)
        {
            if(iConnectCount>1)
            {
                NewRecordProcess("","OLP connected warnning!!!","iConnectCount="+IntToStr(iConnectCount));
            }
            NewRecordProcess("","OLP retry reply !!!","Send socket handle="+IntToStr(OLPServer->Socket->Connections[0]->Handle));   //Sam 20200826 : Add Log
            S=CommandClientReplyBuffer->Strings[0];
            OLPServer->Socket->Connections[0]->SendBuf(S.c_str(), S.Length());
            ShowCharHex(S);
            CommandClientReplyBuffer->Delete(0);
        }
    }
    bRun=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btnConnectClick(TObject *Sender)
{
    if(edinputIP->Text!=0)
        OLPClient->Address=edinputIP->Text;
    if(edinputport->Text!=0)
        OLPClient->Port=edinputport->Text.ToInt();

    try
    {
        if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)  //Sam 20240226 : Client 重連次數
        {
            iClientReConnect=0;
        }
        btnConnect->Enabled=false;
        OLPServer->Open();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfAutomation::btnConnectClick");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btnDisconnectClick(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)
    {
        iClientReConnect=0;
    }
    OLPServer->Close();
    OnLine->Enabled=false;

    ShowOLPState(0);   // 2009.12.02 , Joye
    btnConnect->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btEventReportClick(TObject *Sender)
{
    AnsiString S;
    S=GetMainStatus().UpperCase();
    if(S=="PAUSE" || S=="HALT")
    {
    }
    else
    {
        S="RUN";
    }

    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)//Sam 20190429 : Add CC_PTI_NEWWORK
    {
        SendReportRequest(cbbOLPCommand->Text,0);
    }
    else
    {
        SendReportRequest(cbbOLPCommand->Text);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btAlarmReportClick(TObject *Sender)
{
    DoCommandBuffer("ALARM_REQUEST", "", "InArm:Device pick-up error on Hot Plate", 1, "JAM0109");
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btTestResultClick(TObject *Sender)
{
    DoCommandBuffer("TEST_RESULT_REQUEST");
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::MemoDblClick(TObject *Sender)
{
    SaveRecord();
    Memo->Clear();
    MemoChar->Clear();
    MemoCode->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::ShowCharHex(AnsiString S)
{
    if(chkViewComm->Checked==false)
        return;

    AnsiString S1="", S2="";
    char s[12];
    char Target[10240];
    int len, i;
    strncpy(Target, S.c_str(), sizeof(Target));
    len=strlen(Target);
    for(i=0; i<len; i++)
    {
        if(Target[i]==0x01 || Target[i]==0x02 || Target[i]==0x03)
        {
            if(Target[i]==0x02)
                s[0]='<';
            if(Target[i]==0x01)
                s[0]='=';
            if(Target[i]==0x03)
                s[0]='>';

            s[1]=' ';
            s[2]=0;
        }
        else
        {
            s[0]=Target[i];
            s[1]=' ';
            s[2]=0;
        }
        S1+=s;

        sprintf(s, "%02X ", Target[i]);
        S2+=s;
    }
    MemoChar->Lines->Add(S1);
    MemoChar->Lines->Add("");
    MemoCode->Lines->Add(S2);
    MemoCode->Lines->Add("");

    if(MemoChar->Lines->Count>1000)
    {
        MemoChar->Clear();
    }

    if(MemoCode->Lines->Count>1000)
    {
        MemoCode->Clear();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::ShowRecord(bool bRead , AnsiString S,int iHandle) //Sam 20200813 : 增加 Log debug
{
    if(IniConfig.bN08_1SaveOLPLog==false && chkViewComm->Checked==false)
        return;

    AnsiString S2;
    char str[256];
    if(bRead)
        sprintf(str, "%s [Receive] [%06d]:", GetTimeInfo(),iHandle);    //Sam 20200813 : 增加 Log debug
    else
        sprintf(str, "%s [Send   ] [%06d]:", GetTimeInfo(),iHandle);    //Sam 20200813 : 增加 Log debug
    S2=AnsiString(str)+S;
    ShowMSG(S2);
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::SaveRecord()  //Steven 20141229 : OLP的Log要存檔
{
    AnsiString FileName;

    if(CUSTOMER_CODE==CC_PTI)   //Sam 20200812 : PTI 暫時強制打開
    {
        IniConfig.bN08_1SaveOLPLog=true;
    }

    if(IniConfig.bN08_1SaveOLPLog)
    {
        FileName.sprintf("%s\\%04d_%02d", asOLPLogPath, SystemYear, SystemMonth);
        MyForceDirectories(FileName);
        FileName.sprintf("%s\\%04d_%02d\\%04d-%02d-%02d %02d-%02d-%02d.log", asOLPLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        Memo->Lines->SaveToFile(FileName);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::SendClient(AnsiString S, AnsiString S2,int iHandle)
{
    int i;
    int iConnectCount=(int)(OLPServer->Socket->ActiveConnections);
    int iConnect=-1;

    if(OnLine->Enabled==true && iConnectCount>=1) //Sam 20200727 : 如果連接數大於1以上，以  SocketHandle 來確保回傳
    {
        iConnect=-1;
        for(i=0; i<iConnectCount; i++)    //找尋對應 iConnect Client Handle
        {
            if(iHandle!=iSocketHandle)
            {
                //iHandle != iSocketHandle 時做紀錄 Debug 用
                NewRecordProcess("","OLP iHandle not match iSocketHandle","iHandle="+IntToStr(iHandle)+",iSocketHandle="+IntToStr(iSocketHandle));
            }

            //基本上 iHandle &  iSocketHandle 是一樣的東西 Debug 中先這樣弄
            if(OLPServer->Socket->Connections[i]->SocketHandle==iHandle)            //從資料流丟過來的 Handle
            {
                iConnect=i;
                break;
            }
            else if(OLPServer->Socket->Connections[i]->SocketHandle==iSocketHandle) //接收到 Client 時紀錄 Handle
            {
                iConnect=i;
                break;
            }
        }

        if(iConnect==-1)
        {
            //如果真的找不到對應 Handle 就丟 iConnect=0
            NewRecordProcess("","OLP Not Find SocketHandle","iConnectCount="+IntToStr(iConnectCount));
            iConnect=0;
        }

        if(iConnect!=-1)
        {
            OLPServer->Socket->Connections[iConnect]->SendBuf(S.c_str(), S.Length());
            ShowCharHex(S);
            ShowRecord(WRITE, S2,OLPServer->Socket->Connections[iConnect]->SocketHandle);
        }
        else
        {
            if(bStandard==false)
                AlarmMessage("Not Find SocketHandle.", "找不到 SocketHandle ...");
        }
    }
    else
    {
        ShowRecord(WRITE, S2,iHandle);
        CommandClientReplyBuffer->Add(S);
        NewRecordProcess("", "OLP Wait client connect", "iHandle="+IntToStr(iHandle)+", iSocketHandle="+IntToStr(iSocketHandle)+", iConnectCount="+IntToStr(iConnectCount));  //Sam 20200826 : Add Log
        if(bStandard==false)
            AlarmMessage("Wait client connect.", "等待連線...");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::SendServer(AnsiString S, AnsiString S2)
{
    int iHandle=0;
    if(OnLine2->Enabled==true)//是否連線中
    {
        iHandle=(int)OLPClient->Socket->Handle;
        OLPClient->Socket->SendBuf(S.c_str(), S.Length());
        ShowCharHex(S);
        ShowRecord(WRITE, S2, iHandle);      //Sam 20200813 : 增加 Log debug
    }
    else
    {
        if(IniConfig.bN08_1SaveOLPLog==false && chkViewComm->Checked==false)    //Sam 20240226 : Client 重連次數
            return;

        char str[256];
        sprintf(str, "%s [Send Error] [%06d]:", GetTimeInfo(),0);    //Sam 20200813 : 增加 Log debug
        S2=AnsiString(str)+S2;
        ShowMSG(S2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::WriteTCPDataToTextFile()    //把離線的相關資料寫到Log檔
{
    AnsiString str="", datestr="";
    FILE *file;
    MyProcessBuffer *ProcessPtr;
    MyProcessBuffer *Process=new MyProcessBuffer();
    GetTimeInfo();

    ProcessPtr=(MyProcessBuffer *)CommandBuffer->Items[0];
    if(CommandBuffer->Count>0)
    {
        ProcessPtr=(MyProcessBuffer *)CommandBuffer->Items[0];
        Process->Command=ProcessPtr->Command;
        if(Process->Command=="")
            Process->Command="NULL";

        Process->SubCommand=ProcessPtr->SubCommand;
        if(Process->SubCommand=="")
            Process->SubCommand="NULL";

        Process->AlarmID=ProcessPtr->AlarmID;
        if(Process->AlarmID=="")
            Process->AlarmID="NULL";

        Process->AlarmMessage=ProcessPtr->AlarmMessage;
        if(Process->AlarmMessage=="")
            Process->AlarmMessage="NULL";

        Process->TimeString=ProcessPtr->TimeString;
        if(Process->TimeString=="")
            Process->TimeString="NULL";

        Process->ErrType=ProcessPtr->ErrType;
        str.sprintf("%s#%s#%s#%s#%s#%d\n", Process->Command.c_str(), Process->SubCommand.c_str(), Process->AlarmID.c_str(), Process->AlarmMessage.c_str(), Process->TimeString.c_str(), Process->ErrType);

        datestr.sprintf("d:\\%s_Log\\TCP_Data\\", HandlerType);
        MyForceDirectories(datestr);
        GetTimeInfo();                                                          //Steven 20101105
        datestr.sprintf("d:\\%s_Log\\TCP_Data\\%04d%02d%02d.tcp", HandlerType, aSystemYear, aSystemMonth, aSystemDate);
        file=fopen(datestr.c_str(), "a+");
        if(file!=NULL)
        {
            fputs(str.c_str(), file);
            fclose(file);
        }
        CommandBuffer->Clear();
    }
    delete Process;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::DoCommandBuffer(AnsiString Command, AnsiString SubCommand, AnsiString AlarmMessage, int ErrType, AnsiString AlarmID)
{
    P1=new MyProcessBuffer();      //Steven 20120102 : 改為全域變數
    P1->Command=Command;
    P1->SubCommand=SubCommand;
    P1->AlarmMessage=AlarmMessage;
    P1->ErrType=ErrType;
    P1->AlarmID=AlarmID;
    P1->TimeString=GetTimeInfo();
    CommandBuffer->Add(P1);
}
//---------------------------------------------------------------------------
extern bool bLockByServer;
extern bool SoftStart;
void __fastcall TfAutomation::ProcessBuffer(AnsiString Buffer, int iHandle)  //Sam 20200813 : 增加 Log debug
{
    int iType;
    bool bClient=true;

    if(Buffer==NULL)    //如果Buffer是Null的話，就要跳開，不然會出現異常。
        return;

    AnsiString Command, V_Total, ShowString;
    AnsiString Data[40];        //Sam 20200415 : fix 32 Site
    int v_total;

    ShowCharHex(Buffer);

    if(IsStartWord(Buffer, STX))
    {
        ShowString=stx;
        Command=SplitDataBySoh(Buffer);
        ShowString=ShowString+AddBlock(Command);
    }
    else
    {
        Command="";
        return;
    }

    V_Total=SplitDataBySoh(Buffer);
    ShowString=ShowString+soh+AddBlock(V_Total);
    v_total=atoi(V_Total.c_str());

    for(int i=0; i<v_total; i++)
    {
        Data[i]=SplitDataBySoh(Buffer);
        ShowString=ShowString+soh+AddBlock(Data[i]);
    }
    ShowString=ShowString+etx;
    ShowRecord(READ, ShowString, iHandle);  //Sam 20200813 : 增加 Log debug

    if(Command.Pos("INQUIRE")!=0)
    {
        if(Command=="REPORT_INQUIRE")
        {
            Data[0]=0;
            CommandProcess("REPORT_GRANT", 1, Data, bClient, iHandle);  //Sam 20200813 : 增加 Log debug
            MySleep(100);           //Steven 20110902 : 動作太快可能只會做其中一個,所以要Delay一下
            SendReportRequest(Data[2]);
        }
        else
        {
            TCPstr = "";
            int iInquire=0;
            AnsiString CMD=Command;
            CMD.Delete(CMD.Length()-6, 7);
            CMD+="GRANT";

            if(     Command=="PP_LOAD_INQUIRE")
            {
                if(CheckCanChangeRealDummy())
                {
                    Data[0]="0";
                }
                else
                {
                    Data[0]="1";
                }
                iInquire=1;
            }
            else if(Command=="HTMLJSON_INQUIRE")
            {
                Data[0]=GetHTMLJSONDatas();
                iInquire=2;
            }
            else if(Command=="VERSION_INQUIRE")
            {
                Data[0]=GetSoftwareVersion();
                iInquire=2;
            }
            else if(Command=="MAIN_STATUS_INQUIRE")                             //AI(ht9045-v899) 20260422: support MAIN_STATUS_INQUIRE and normalize status reply
            {
                AnsiString sStatus=GetMainStatus().UpperCase();
                sStatus=StringReplace(sStatus, " ", "_", TReplaceFlags()<<rfReplaceAll);
                if(sStatus=="")
                    sStatus="UNKNOWN";
                Data[0]=sStatus;
                iInquire=2;
            }
            else if(Command=="ACT_TEMP_INQUIRE")
            {
                for(int i=0; i<10; i++)
                    Data[i]=GetActTemp(i);
                Data[10]="0";
                iInquire=11;
            }
            else if(Command=="TRAY_INFO_INQUIRE")
            {
                iInquire=GetTrayForm(Data) + 1;
            }
            else if(Command=="HANDMODE_INQUIRE")
            {
                iInquire=GetHandMode(Data) + 1;
            }
            else if(Command=="PLATE_INFO_INQUIRE")
            {
                iInquire=GetPlateForm(Data) + 1;
            }
            else if(Command=="SETUP_FILE_NAME_INQUIRE")
            {
                Data[0]=GetSetUpFileName();
                iInquire=2;
            }
            else if(Command=="JAM_COUNT_INQUIRE")
            {
                Data[0]=GetJamCount();
                iInquire=2;
            }
            else if(Command=="CATEGORY_INQUIRE")
            {
                iInquire=GetCategory(Data) + 1;
            }
            else if(Command=="BINDEFINE_INQUIRE")       //Sam 20230803 : 新增OLP指令
            {
                iInquire=GetBindefine(Data)+1;
            }
            else if(Command=="FIXTRAYDEFINE_INQUIRE")   //Sam 20230921 : 新增 FixTray 指令
            {
                iInquire=GetFixTrayDefine(Data)+1;
            }
            else if(Command=="MAPPING_INQUIRE")
            {
                iInquire=GetMapping(Data) + 1;
            }
            else if(Command=="DUT_INQUIRE")
            {
                iInquire = GetDutOnOff(Data) + 1;
            }
            else if(Command=="SOAK_TIME_INQUIRE")
            {
                Data[0]=GetSoakTime();
                iInquire=2;
            }
            else if(Command=="TEMPERATURE_INQUIRE")
            {
                Data[0]=GetTemperature();
                iInquire=2;
            }
            else if(Command=="TEMPMODE_INQUIRE")
            {
                Data[0]=GetTempMode();
                iInquire=2;
            }
            else if(Command=="CONNECTION_INQUIRE")
            {
                Data[0]=GetTesterConnect();
                iInquire=2;
            }
            else if(Command=="TESTMODE_INQUIRE")
            {
                Data[0]=GetTesterMode();
                iInquire=2;
            }
            else if(Command=="AlarmMode_INQUIRE")
            {
            }
            else if(Command=="AllSiteFail_INQUIRE")
            {
            }
            else if(Command=="ByHeadFail_INQUIRE")
            {
            }
            else if(Command=="ByBinAll_INQUIRE")
            {
            }
            else if(Command=="SetSiteYield_INQUIRE")
            {
            }
            else if(Command=="BinOverLimitSelec_INQUIRE")
            {
            }
            else if(Command=="BinOverLimitSet_INQUIRE")
            {
            }
            else if(Command=="BinOverCountSet_INQUIRE")
            {
            }
            else if(Command=="SetAutoHeight_INQUIRE")
            {
                Data[0]=GetAutoHeight(0);
                Data[1]=GetAutoHeight(1);
                iInquire=3;
            }
            else if(Command=="SetContactOffset_INQUIRE")
            {
                Data[0]=GetContactOffset(0);
                Data[1]=GetContactOffset(1);
                iInquire=3;
            }
            else if(Command=="SetContactTestMode_INQUIRE")
            {
                Data[0]=GetContactTestMode();
                iInquire=2;
            }
            else if(Command=="SetSecondSpeed_INQUIRE")
            {
                Data[0]=GetSecondSpeed();
                iInquire=2;
            }
            else if(Command=="SetContactWaitHeight_INQUIRE")
            {
                Data[0]=GetContactWaitHeight();
                iInquire=2;
            }
            else if(Command=="SetDropHeight_INQUIRE")
            {
                Data[0]=GetDropHeight(0);
                Data[0]=GetDropHeight(1);
                iInquire=3;
            }
            else if(Command=="SetReleaseWait_INQUIRE")
            {
                Data[0]=GetReleaseWait();
                iInquire=2;
            }
            else if(Command=="SetShuttlePickOffset_INQUIRE")
            {
                Data[0]=GetShuttlePickOffset(0);
                Data[0]=GetShuttlePickOffset(1);
                iInquire=3;
            }
            else if(Command=="ShuttlePickHeight_INQUIRE")
            {
                Data[0]=GetShuttlePickHeight(0);
                Data[0]=GetShuttlePickHeight(1);
                iInquire=3;
            }
            else if(Command=="ShuttleReleaseHeight_INQUIRE")
            {
                Data[0]=GetShuttleReleaseHeight(0);
                Data[0]=GetShuttleReleaseHeight(1);
                iInquire=3;
            }
            else if(Command=="ArmTotalForce_INQUIRE ")
            {
                Data[0]=GetArmTotalForce(0);
                Data[0]=GetArmTotalForce(1);
                iInquire=3;
            }
            else if(Command=="ClinderForce_INQUIRE")
            {
                Data[0]=GetClinderForce(0);
                Data[0]=GetClinderForce(1);
                iInquire=3;
            }
            else if(Command=="ForcePerDevice_INQUIRE")
            {
                Data[0]=GetForcePerDevice(0);
                Data[0]=GetForcePerDevice(1);
                iInquire=3;
            }
            else if(Command=="SetNoPerPin_INQUIRE")
            {
                Data[0]=GetSetNoPerPin();
                iInquire=2;
            }
            else if(Command=="SetForcePerPin_INQUIRE")
            {
                Data[0]=GetForcePerPin();
                iInquire=2;
            }
            else if(Command=="SetContactForcen_INQUIRE")
            {
                Data[0]=GetContactForcen();
                iInquire=2;
            }
            else if(Command=="GetStartPos_INQUIRE")
            {
                Data[0]=GetStartPos(0);
                Data[0]=GetStartPos(1);
                iInquire=3;
            }
            else if(Command=="GetDivision_INQUIRE")
            {
                Data[0]=GetDivision(0);
                Data[0]=GetDivision(1);
                iInquire=3;
            }
            else if(Command=="GetDimemsion_INQUIRE")
            {
                iInquire=GetDimemsion(Data)+1;
            }
            else if(Command=="GetZThickness_INQUIRE")
            {
                Data[0]=GetZThickness();
                iInquire=2;
            }
            else if(Command=="SetTrayType_INQUIRE")
            {
                Data[0]=GetTrayType();
                iInquire=2;
            }
            else if(Command=="GetPitch_INQUIRE")
            {
                Data[0]=GetPitch(0);
                Data[1]=GetPitch(1);
                iInquire=2;
            }
//            else if(Command=="GetIP_INQUIRE")
//            {
//                Data[0]=GetIP(Data);
//                iInquire=2;
//            }
//            else if(Command=="GetPort_INQUIRE")
//            {
//                Data[0]=GetPort(Data);
//                iInquire=2;
//            }
//            else if(Command=="GetCusCode_INQUIRE")
//            {
//                Data[0]=GetCusCode(Data);
//                iInquire=2;
//            }
            //Sam 20190802 : Add New OLP For HT9045
            //==>
            else if(Command=="LowYield_INQUIRE")
            {
                iInquire=GetLowYield(Data)+1;
            }
            else if(Command=="ByArmPerSiteDiffYield_INQUIRE")
            {
                iInquire=GetByArmPerSiteDiffYield(Data)+1;
            }
            else if(Command=="ConsecutiveFailureAlarmByHead_INQUIRE")
            {
                iInquire=GetConsecutiveFailureAlarmByHead(Data)+1;
            }
            else if(Command=="ConsecutiveFailureAlarmBySocket_INQUIRE")
            {
                iInquire=GetConsecutiveFailureAlarmBySocket(Data)+1;
            }
            else if(Command=="AllSiteFailFor9045_INQUIRE")
            {
                iInquire=GetAllSiteFailFor9045(Data)+1;
            }
            else if(Command=="TrayFormTypeInfo_INQUIRE")
            {
                iType=atoi(Data[0].c_str());
                iInquire=GetTrayFormTypeInfo(Data,iType)+1;
            }
            else if(Command=="TrayFormTypeThickness_INQUIRE")
            {
                iType=atoi(Data[0].c_str());
                iInquire=GetTrayFormTypeThickness(Data,iType)+1;
            }
            else if(Command=="TrayFormTypePickUp_INQUIRE")
            {
                iType=atoi(Data[0].c_str());
                iInquire=GetTrayFormTypePickUp(Data,iType)+1;
            }
            else if(Command=="TrayFormTypeName_INQUIRE")
            {
                iType=atoi(Data[0].c_str());
                iInquire=GetTrayFormTypeName(Data,iType)+1;
            }
            else if(Command=="ContactModeFor9045_INQUIRE")
            {
                iInquire=GetContactModeFor9045(Data)+1;
            }
            else if(Command=="ContactVacuumMode_INQUIRE")
            {
                iInquire=GetContactVacuumMode(Data)+1;
            }
            else if(Command=="ContactDropWait_INQUIRE")
            {
                iInquire=GetContactDropWait(Data)+1;
            }
            else if(Command=="SlowContactSpeed_INQUIRE")
            {
                iInquire=GetSlowContactSpeed(Data)+1;
            }
            else if(Command=="ShuttleWaitOutSideCamber_INQUIRE")
            {
                iInquire=GetShuttleWaitOutSideCamber(Data)+1;
            }
            else if(Command=="PickShuttleDeviceAfterTested_INQUIRE")
            {
                iInquire=GetPickShuttleDeviceAfterTested(Data)+1;
            }
            else if(Command=="PickShuttleDeviceThenWaitOnShuttle_INQUIRE")
            {
                iInquire=GetPickShuttleDeviceThenWaitOnShuttle(Data)+1;
            }
            else if(Command=="PickShuttleDeviceTogetherFor32SiteN_INQUIRE")
            {
                iInquire=GetPickShuttleDeviceTogetherFor32SiteN(Data)+1;
            }
            else if(Command=="IndexArm1Height_INQUIRE")
            {
                iInquire=GetIndexArm1Height(Data)+1;
            }
            else if(Command=="IndexArm2Height_INQUIRE")
            {
                iInquire=GetIndexArm2Height(Data)+1;
            }
            else if(Command=="TestICCheckMode_INQUIRE")
            {
                iInquire=GetTestICCheckMode(Data)+1;
            }
            else if(Command=="AboveSocket_INQUIRE")
            {
                iInquire=GetAboveSocket(Data)+1;
            }
            else if(Command=="ContactForceInfo_INQUIRE")
            {
                iInquire=GetContactForceInfo(Data)+1;
            }
            else if(Command=="HotPlateFormName_INQUIRE")
            {
                iInquire=GetHotPlateFormName(Data)+1;
            }
            else if(Command=="HotPlate1_INQUIRE")
            {
                iInquire=GetHotPlate1(Data)+1;
            }
            else if(Command=="HotPlate2_INQUIRE")
            {
                iInquire=GetHotPlate2(Data)+1;
            }
            else if(Command=="InterfaceType_INQUIRE")
            {
                iInquire=GetInterfaceType(Data)+1;
            }
            else if(Command=="TesterInitialMaximumTest_INQUIRE")
            {
                iInquire=GetTesterInitialMaximumTest(Data)+1;
            }
            else if(Command=="TesterMaximumTest_INQUIRE")
            {
                iInquire=GetTesterMaximumTest(Data)+1;
            }
            else if(Command=="TesterDummyTest_INQUIRE")
            {
                iInquire=GetTesterDummyTest(Data)+1;
            }
            else if(Command=="TesterStartDelay_INQUIRE")
            {
                iInquire=GetTesterStartDelay(Data)+1;
            }
            else if(Command=="HotSoakTime_INQUIRE")
            {
                iInquire=GetHotSoakTime(Data)+1;
            }
            else if(Command=="HotJamSoakTime_INQUIRE")
            {
                iInquire=GetHotJamSoakTime(Data)+1;
            }
            else if(Command=="HotInitialWaitTime_INQUIRE")
            {
                iInquire=GetHotInitialWaitTime(Data)+1;
            }
            else if(Command=="HotInitialStart1Time_INQUIRE")
            {
                iInquire=GetHotInitialStart1Time(Data)+1;
            }
            else if(Command=="HotShuttleSoakTime_INQUIRE")
            {
                iInquire=GetHotShuttleSoakTime(Data)+1;
            }
            else if(Command=="ChamberCoolingTemp_INQUIRE")
            {
                iInquire=GetChamberCoolingTemp(Data)+1;
            }
            else if(Command=="HotIndexSoakTime_INQUIRE")
            {
                iInquire=GetHotIndexSoakTime(Data)+1;
            }
            else if(Command=="HotOpenShortTime_INQUIRE")
            {
                iInquire=GetHotOpenShortTime(Data)+1;
            }
            else if(Command=="HotZ1Down_INQUIRE")
            {
                iInquire=GetHotZ1Down(Data)+1;
            }
            else if(Command=="HotShuttleSoakMode_INQUIRE")
            {
                iInquire=GetHotShuttleSoakMode(Data)+1;
            }
            else if(Command=="MachineTempMode_INQUIRE")
            {
                iInquire=GetMachineTempMode(Data)+1;
            }
            else if(Command=="AmbientCheck_INQUIRE")
            {
                iInquire=GetAmbientCheck(Data)+1;
            }
            else if(Command=="AmbientCheckTemp_INQUIRE")
            {
                iInquire=GetAmbientCheckTemp(Data)+1;
            }
            else if(Command=="TempeartureOffset_INQUIRE")
            {
                iInquire=GetTemperatureOffset(Data)+1;
            }
            else if(Command=="ContactCountForOffsetPeriod_INQUIRE")
            {
                iInquire=GetContactCountForOffsetPeriod(Data)+1;
            }
            else if(Command=="ContactCountForCoolDown_INQUIRE")
            {
                iInquire=GetContactCountForCoolDown(Data)+1;
            }
            else if(Command=="InterfaceDIOInfo_INQUIRE")
            {
                iInquire=GetInterfaceDIOInfo(Data)+1;
            }
            else if(Command=="InterfaceGPIBInfo_INQUIRE")
            {
                iInquire=GetInterfaceGPIBInfo(Data)+1;
            }
            else if(Command=="InterfaceRS232Info_INQUIRE")
            {
                iInquire=GetInterfaceRS232Info(Data)+1;
            }
            else if(Command=="LotInfo_INQUIRE")
            {
                iInquire=GetLotInfo(Data)+1;
            }
            else if(Command=="GetIP_INQUIRE")
            {
                iInquire=GetIP(Data)+1;
            }
            else if(Command=="GetPort_INQUIRE")
            {
                iInquire=GetPort(Data)+1;
            }
            else if(Command=="GetCusCode_INQUIRE")
            {
                iInquire=GetCusCode(Data)+1;
            }
            else if(Command=="StartMode_INQUIRE")   //Sam 20221212 : 新增 StartMode 指令
            {
                iInquire=GetStartMode(Data)+1;
            }
            else if(Command=="LOTTOTAL_INQUIRE")    //Sam 20230803 : 新增OLP指令
            {
                iInquire=GetLotTotal(Data)+1;
            }

            if(iInquire!=1)
                Data[iInquire-1]="0";
            CommandProcess(CMD, iInquire, Data, bClient, iHandle);
        }
    }
    else if(Command=="ON_LINE_REQUEST")  // 2008/05/23
    {
        DoOnLineReply(Data);
        if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)  //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            CommandProcess("ON_LINE_REPLY", 5, Data, bClient, iHandle);//for sback
        }
        else
        {
            CommandProcess("ON_LINE_REPLY", 4, Data, bClient, iHandle);
        }
    }
    else if(Command=="INITIATE_REQUEST")  //2008/05/23
    {
        SYSTEMTIME SysTime;
        SysTime.wYear   = atoi(Data[0].SubString(1,  4).c_str());
        SysTime.wMonth  = atoi(Data[0].SubString(5,  2).c_str());
        SysTime.wDay    = atoi(Data[0].SubString(7,  2).c_str());
        SysTime.wHour   = atoi(Data[0].SubString(9,  2).c_str());
        SysTime.wMinute = atoi(Data[0].SubString(11, 2).c_str());
        SysTime.wSecond = atoi(Data[0].SubString(13, 2).c_str());
        SysTime.wMilliseconds=0;
        SetLocalTime(&SysTime);

        Data[0]=0;

        if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)                      //2016.03.24 , Brian   Add CC_PTI_NEWWORK
        {
           fMain->Home("TfAutomation::ProcessBuffer");
        }
        CommandProcess("INITIATE_REPLY", 1, Data, bClient, iHandle);
        //Sam 20190429 : Add CC_PTI_NEWWORK
        if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)
        {
            OLPServer->Close();
            OnLine->Enabled=false;

            ShowOLPState(0);
            btnConnect->Enabled=true;

            if(edinputIP->Text!=0)
                OLPClient->Address=edinputIP->Text;
            if(edinputport->Text != 0)
                OLPClient->Port=edinputport->Text.ToInt();
            try
            {
                btnConnect->Enabled=false;
                OLPServer->Open();
                OLPServer->Active=true;
                ShowOLPState(1);//Hsiong 2015.04.16 Add CC_MTI_NEWWORK
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfAutomation::ProcessBuffer");
            }
        }
    }
    else if(Command=="PAUSE_REQUEST")
    {
        bLockByServer=true;
        SoftStop = true;

        Data[0]=0;
        CommandProcess("PAUSE_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="RESUME_REQUEST")
    {
        bLockByServer=false;

        Data[0]=0;
        CommandProcess("RESUME_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="START_REQUEST")
    {
        if(fMain->palMainStatus->Caption=="HALT" && SystemStart==false)         //Sam 20240125 : 只能在閒置時啟動
        {
            MyDBIProcess("Message","Automation Command Start succeed!!");
            fMain->Start("TfAutomation::ProcessBuffer");
            //SoftStart=true;
        }
        else
        {
            MyDBIProcess("Message", "Automation Command Start fail!!");
        }
        Data[0]=0;
        CommandProcess("START_REPLY",1,Data, bClient, iHandle);
    }
    else if(Command=="PAUSE_REQUEST")
    {
        if(SystemStart==true)                                                   //Sam 20240125 : 只能在閒置時啟動
        {
            MyDBIProcess("Message","Automation Command PAUSE succeed!!");
            fMain->Pause("TfAutomation::ProcessBuffer");
        }
        else
        {
            MyDBIProcess("Message","Automation Command PAUSE fail!!");
        }
        Data[0]=0;
        CommandProcess("START_REPLY",1,Data, bClient, iHandle);
    }
  #ifdef DEBUG_DUTONOFF
    else if(Command=="CLEANOUT_REQUEST")
    {
        iCleanOut=1;
        bCleanOut=true;
        Data[0]=0;
//        CommandProcess("CLEANOUT_REPLY",1,Data);
    }
    else if(Command=="HOMEANDSTART_REQUEST")
    {
        DoHomeAndStart();

        Data[0]=0;
        CommandProcess("CLEANOUT_REPLY",1,Data, bClient, iHandle);
    }
    else if(Command=="ONECYCLE_REQUEST")
    {
        DoOneCycle();
        Data[0]=0;
        bOneCycle=true;

//        CommandProcess("ONECYCLE_REPLY",1,Data);
    }
    #endif
    else if(Command=="CLEAR_REPORT_REQUEST")
    {
        int iSwitchCase=0;
        for(int i=0; i<v_total; i++)
        {
            iSwitchCase=atoi(Data[0].c_str());
            DoClearReportRequest(iSwitchCase);
        }

        Data[0]=0;
        CommandProcess("CLEAR_REPORT_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="PP_UL_REQUEST")                                           //把工作檔送給HOST
    {
        DoULRequest(Data);
        CommandProcess("PP_UL_REPLY", 4, Data, bClient, iHandle);
    }
    else if(Command=="PP_DL_REQUEST")                                           //收到檔案並解壓縮
    {
        ProcessBuffer1();
    }
    else if(Command=="CATEGORY_REQUEST")
    {
        NewRecordProcess("","CATEGORY_REQUEST",ShowString);                     //Sam 20230803 : 客戶設定 Bin 要特別紀錄
        Data[0]=SetCategory(Data);
        if(atoi(Data[0].c_str())>=3)                                            //Sam 20230921 : Bin 設定錯誤不能啟動
            LastSet.OLPSetBinErr[0]=atoi(Data[0].c_str());
        else
            LastSet.OLPSetBinErr[0]=0;

        CommandProcess("CATEGORY_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="BINDEFINE_REQUEST")                                       //Sam 20230803 : 新增OLP指令
    {
        NewRecordProcess("","BINDEFINE_REQUEST",ShowString);                    //Sam 20230803 : 客戶設定 Bin 要特別紀錄
        Data[0]=SetBinDefine(Data);
        if(atoi(Data[0].c_str())>=3)                                            //Sam 20230921 : Bin 設定錯誤不能啟動
            LastSet.OLPSetBinErr[1]=atoi(Data[0].c_str());
        else
            LastSet.OLPSetBinErr[1]=0;

        CommandProcess("BINDEFINE_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="FIXTRAYDEFINE_REQUEST")                                   //Sam 20230921 : 新增 FixTray 指令
    {
        NewRecordProcess("","FIXTRAYDEFINE_REQUEST",ShowString);
        Data[0]=SetFixTrayDefine(Data);
        if(atoi(Data[0].c_str())>=3)    //Sam 20230921 : Bin 設定錯誤不能啟動
            LastSet.OLPSetBinErr[2]=atoi(Data[0].c_str());
        else
            LastSet.OLPSetBinErr[2]=0;

        CommandProcess("FIXTRAYDEFINE_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="MAPPING_REQUEST")
    {
        if(CheckNeedCleanOut()==false)
        {
            Data[0]=AnsiString(SetMapping(Data));
        }
        else
        {
            Data[0]="1";
        }
        CommandProcess("MAPPING_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="DUT_REQUEST")
    {
        if(CheckNeedCleanOut()==false)
        {
            Data[0]=AnsiString(SetDutOnOff(Data));
        }
        else
        {
            Data[0]="1";
        }
        CommandProcess("DUT_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="SOAK_TIME_REQUEST")
    {
        if(CheckNeedCleanOut()==false)
        {
            SetSoakTime(Data);
            Data[0]="0";
        }
        else
        {
            Data[0]="1";
        }
        CommandProcess("SOAK_TIME_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="TEMPERATURE_REQUEST")
    {
        if(CheckNeedCleanOut()==false)
        {
            SetTemperature(Data);
            Data[0]="0";
        }
        else
        {
            Data[0]="1";
        }
        CommandProcess("TEMPERATURE_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="TEMPMODE_REQUEST")
    {
        if(CheckNeedCleanOut()==false)
        {
            SetTempMode(Data);
            Data[0]="0";
        }
        else
        {
            Data[0]="1";
        }
        CommandProcess("TEMPMODE_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="CONNECTION_REQUEST")
    {
        if(CheckNeedCleanOut()==false)
        {
            Data[0]=SetTesterConnect(Data);
        }
        else
        {
            Data[0]="1";
        }
        CommandProcess("CONNECTION_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="TESTMODE_REQUEST")
    {
        if(CheckNeedCleanOut()==false)
        {
            Data[0]=SetTesterMode(Data);
        }
        else
        {
            Data[0]="1";
        }
        CommandProcess("TESTMODE_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="SETUP_FILE_NAME_REQUEST")
    {
        if(CheckNeedCleanOut()==false)
        {
            Data[0]=SetSetUpFileName(Data);
        }
        else
        {
            Data[0]="1";
        }
        CommandProcess("SETUP_FILE_NAME_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="AlarmMode_REQUEST")
    {
    }
    else if(Command=="AllSiteFail_REQUEST")
    {
    }
    else if(Command=="ByHeadFail_REQUEST")
    {
    }
    else if(Command=="ByBinAll_REQUEST")
    {
    }
    else if(Command=="SetSiteYield_REQUEST")
    {
    }
    else if(Command=="BinOverLimitSelec_REQUEST")
    {
    }
    else if(Command=="BinOverLimitSet_REQUEST")
    {
    }
    else if(Command=="BinOverCountSet_REQUEST")
    {
    }
    else if(Command=="SetContactTestMode_REQUEST")
    {
    }
    else if(Command=="SetDropHeight_REQUEST")
    {
    }
    else if(Command=="SetReleaseWait_REQUEST")
    {
    }
    else if(Command=="LowYield_REQUEST")
    {
        Data[0]=SetLowYield(Data);
        CommandProcess("LowYield_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="ByArmPerSiteDiffYield_REQUEST")
    {
        Data[0]=SetByArmPerSiteDiffYield(Data);
        CommandProcess("ByArmPerSiteDiffYield_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="ConsecutiveFailureAlarmByHead_REQUEST")
    {
        Data[0]=SetConsecutiveFailureAlarmByHead(Data);
        CommandProcess("ConsecutiveFailureAlarmByHead_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="ConsecutiveFailureAlarmBySocket_REQUEST")
    {
        Data[0]=SetConsecutiveFailureAlarmBySocket(Data);
        CommandProcess("ConsecutiveFailureAlarmBySocket_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="AllSiteFailFor9045_REQUEST")
    {
        Data[0]=SetAllSiteFailFor9045(Data);
        CommandProcess("AllSiteFailFor9045_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="ContactModeFor9045_REQUEST")
    {
        Data[0]=SetContactModeFor9045(Data);
        CommandProcess("ContactModeFor9045_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="ContactVacuumMode_REQUEST")
    {
        Data[0]=SetContactVacuumMode(Data);
        CommandProcess("ContactVacuumMode_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="ContactDropWait_REQUEST")
    {
        Data[0]=SetContactDropWait(Data);
        CommandProcess("ContactDropWait_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="SlowContactSpeed_REQUEST")
    {
        Data[0]=SetSlowContactSpeed(Data);
        CommandProcess("SlowContactSpeed_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="ShuttleWaitOutSideCamber_REQUEST")
    {
        Data[0]=SetShuttleWaitOutSideCamber(Data);
        CommandProcess("ShuttleWaitOutSideCamber_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="PickShuttleDeviceAfterTested_REQUEST")
    {
        Data[0]=SetPickShuttleDeviceAfterTested(Data);
        CommandProcess("PickShuttleDeviceAfterTested_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="PickShuttleDeviceThenWaitOnShuttle_REQUEST")
    {
        Data[0]=SetPickShuttleDeviceThenWaitOnShuttle(Data);
        CommandProcess("PickShuttleDeviceThenWaitOnShuttle_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="PickShuttleDeviceTogetherFor32SiteN_REQUEST")
    {
        Data[0]=SetPickShuttleDeviceTogetherFor32SiteN(Data);
        CommandProcess("PickShuttleDeviceTogetherFor32SiteN_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="IndexArm1Height_REQUEST")
    {
        Data[0]=SetIndexArm1Height(Data);
        CommandProcess("IndexArm1Height_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="IndexArm2Height_REQUEST")
    {
        Data[0]=SetIndexArm2Height(Data);
        CommandProcess("IndexArm2Height_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="TestICCheckMode_REQUEST")
    {
        Data[0]=SetTestICCheckMode(Data);
        CommandProcess("TestICCheckMode_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="AboveSocket_REQUEST")
    {
        Data[0]=SetAboveSocket(Data);
        CommandProcess("AboveSocket_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="HotPlate1_REQUEST")
    {
        Data[0]=SetHotPlate1(Data);
        CommandProcess("HotPlate1_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="HotPlate2_REQUEST")
    {
        Data[0]=SetHotPlate2(Data);
        CommandProcess("HotPlate2_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="TesterInitialMaximumTest_REQUEST")
    {
        Data[0]=SetTesterInitialMaximumTest(Data);
        CommandProcess("TesterInitialMaximumTest_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="TesterMaximumTest_REQUEST")
    {
        Data[0]=SetTesterMaximumTest(Data);
        CommandProcess("TesterMaximumTest_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="TesterDummyTest_REQUEST")
    {
        Data[0]=SetTesterDummyTest(Data);
        CommandProcess("TesterDummyTest_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="TesterStartDelay_REQUEST")
    {
        Data[0]=SetTesterStartDelay(Data);
        CommandProcess("TesterStartDelay_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="HotZ1Down_REQUEST")
    {
        Data[0]=SetHotZ1Down(Data);
        CommandProcess("HotZ1Down_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="HotShuttleSoakMode_REQUEST")
    {
        Data[0]=SetHotShuttleSoakMode(Data);
        CommandProcess("HotShuttleSoakMode_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="AmbientCheck_REQUEST")
    {
        Data[0]=SetAmbientCheck(Data);
        CommandProcess("AmbientCheck_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="AmbientCheckTemp_REQUEST")
    {
        Data[0]=SetAmbientCheckTemp(Data);
        CommandProcess("AmbientCheckTemp_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="TempeartureOffset_REQUEST")
    {
        Data[0]=SetTemperatureOffset(Data);
        CommandProcess("TempeartureOffset_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="ContactCountForOffsetPeriod_REQUEST")
    {
        Data[0]=SetContactCountForOffsetPeriod(Data);
        CommandProcess("ContactCountForOffsetPeriod_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="ContactCountForCoolDown_REQUEST")
    {
        Data[0]=SetContactCountForCoolDown(Data);
        CommandProcess("ContactCountForCoolDown_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="LotInfo_REQUEST")
    {
        if(CUSTOMER_CODE==CC_Greatek && CosFunction.bOEEFunction)               //AI(JimmyChiu) 20260515: F899-008 §6.2 超豐在 OEE 開啟才走內嵌 OEE Start Lot
        {
            AnsiString asErrorMsg="";
            Data[0]=SetLotInfoGreatekOEE(Data, asErrorMsg);
            Data[1]=asErrorMsg;
            if(Data[0]=="0")
                CommandProcess("LotInfo_REPLY", 1, Data, bClient, iHandle);
            else
                CommandProcess("LotInfo_REPLY", 2, Data, bClient, iHandle);
        }
        else
        {
            Data[0]=SetLotInfo(Data);
            CommandProcess("LotInfo_REPLY", 1, Data, bClient, iHandle);
        }
    }
    else if(Command=="StartMode_REQUEST")   //Sam 20221212 : 新增 StartMode 指令
    {
        Data[0]=SetStartMode(Data);
        CommandProcess("StartMode_REPLY", 1, Data, bClient, iHandle);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::ProcessBuffer1()
{
    AnsiString Command, V_Total, Temp, ShowString, filename, filelength;
    AnsiString Data[4];
    int pos, ret=0;
    FILE *fs;

    if(IsStartWord(TCPstr, STX))
    {
        ShowCharHex(TCPstr);
        ShowString=stx;
        pos=TCPstr.Pos('\x01');//取得分隔符號位置
        Command=TCPstr.SubString(1, pos-1);//取得Command
        TCPstr.Delete(1, pos);//刪除以擷取字串
        ShowString=ShowString+AddBlock(Command)+soh;
    }
    else
    {
        Command="";
        return;
    }

    if(Command=="PP_DL_REQUEST")    //接收到Host傳送過來的壓縮檔
    {
        pos=TCPstr.Pos('\x01');
        V_Total=TCPstr.SubString(1, pos-1);
        TCPstr.Delete(1, pos); //取得資料筆數 並刪除已讀取字串
        pos=TCPstr.AnsiPos('\x01');
        sDLFileName=TCPstr.SubString(1, pos-1); //取得檔案名
        sDLFileName.Delete(sDLFileName.Pos(".zip"), 4); //刪除附檔名
        TCPstr.Delete(1, pos);//刪除已讀取資料
        pos=TCPstr.Pos('\x01');
        filelength=TCPstr.SubString(1, pos-1); //取得檔案大小
        TCPstr.Delete(1, pos);
        ShowString=ShowString+AddBlock(V_Total)+soh+AddBlock(sDLFileName+".zip")+soh+AddBlock(filelength);

        int s;
        AnsiString ddd;
        filename=aDataPath+sDLFileName+".zip"; //filename : 存放路徑
        fs=fopen(filename.c_str(), "wb"); //開啟檔案
        if(fs!=NULL)    // 2009.07.30 , Joye
        {
            for(int i=0; i<(TCPstr.Length()-2); i+=2) //寫入檔案
            {
                ddd="0x"+AnsiString(TCPstr.c_str()[i])+AnsiString(TCPstr.c_str()[i+1]);
                s=atoi(ddd.c_str());
                fprintf(fs, "%1c", s);
            }
            fclose(fs);//關檔
        }

        ShowString=ShowString+soh+AddBlock(TCPstr)+etx;
        ShowRecord(READ, ShowString, 0);

        ret=DoDLRequest(sDLFileName);
        if(ret==0)
            Data[0]="0";
        else
            Data[0]="1";
        CommandProcess("PP_DL_REPLY", 1, Data, 0);
        TCPstr="";
        sDLFileName="";
    }
    TCPstr="";
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::SendReportRequest(AnsiString ReportID,bool Standard)
{
    int iTotal, i;
    AnsiString CMD="REPORT_REQUEST",SV_TOTAL;
    AnsiString V_TOTAL=2;
    AnsiString HEAD,head,SendString,S2,cmd,v_total,sv_total;//,ReportID;
    AnsiString Data[100];
    AnsiString R_TIME,r_time,S;

    R_TIME=GetTimeInfo();
    r_time=AddBlock(R_TIME);

    cmd=AddBlock(CMD);
    v_total=AddBlock(V_TOTAL);

    HEAD=STX+CMD+SOH+V_TOTAL+SOH+R_TIME+SOH+ReportID+SOH;
    head=stx+cmd+soh+v_total+soh+r_time+soh+AddBlock(ReportID)+soh;

    if(ReportID=="0001")
    {
        iTotal=1;
        S=GetMainStatus();
        Data[0]=S;
    }
    else if(ReportID=="0002")   //MTBF與MUBF
    {
        if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)
        {
            iTotal=7;
        }
        else
        {
            iTotal=4;
        }
        GetProductivity(Data);
    }
    else if(ReportID=="0003")   //載入的IC數
    {
        iTotal=1;
        Data[0]=GetLoadCount();
    }
    else if(ReportID=="0004")   //每個Tray分幾顆IC
    {
        iTotal=9;
        GetSortingCount(Data);
    }
    else if(ReportID=="0005")
    {
        iTotal=GetSocketCount(Data);
    }
    else if(ReportID=="0006")
    {
        iTotal=GetHeadCount(Data);
    }
    else if(ReportID=="0007")
    {
        iTotal=2;
        Data[0]="100";
        Data[1]="101";
    }

    SV_TOTAL=AnsiString(iTotal);
    sv_total=AddBlock(SV_TOTAL);
    HEAD=HEAD+SV_TOTAL;
    head=head+sv_total;

    for(i=0; i<iTotal; i++)
    {
        HEAD=HEAD+SOH+Data[i];
        head=head+soh+AddBlock(Data[i]);
    }

    SendString=HEAD+ETX;
    S2=head+etx;

    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)   //Sam 20190429 : Add CC_PTI_NEWWORK
    {
        ShowOLPState(1);
    }

    if(Standard)
        SendClient(SendString, S2, iSocketHandle);                                        //2007_0522
    else
        SendServer(SendString, S2);
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    if (ReceiveString2.Length() > 1000)
    {
        ShowMSG(AnsiString().sprintf("OLPServerClientRead too long to delete, %s",ReceiveString2));
        ReceiveString2="";
    }
    //讀取訊息
    int StartPos=0,EndPos=0;
    //接收字串
    ReceiveString2+=OLPClient->Socket->ReceiveText();

    while(true)
    {
        //尋找STX
        StartPos=ReceiveString2.Pos(STX);
        //尋找ETX
        EndPos=ReceiveString2.Pos(ETX);

        if(bReceive2==false && StartPos>0 && EndPos>0  && StartPos<EndPos)
        {
            TCPstr2=ReceiveString2.SubString(StartPos,EndPos);
            ReceiveString2.Delete(1,EndPos);
            bReceive2=true;
        }

        if(bReceive2)
        {
            CompareEvent(TCPstr2);
            bReceive2=false;
        }
        else
        {
            break;
        }
    }

    SendInvalid=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    OnLine2->Enabled=true;
    iClientReConnect=0; //Sam 20240226 : Client 重連次數
    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)   //Sam 20190429 : Add CC_PTI_NEWWORK
    {
        ShowOLPState(1);
    }
    OLPClientConnected=true;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    OnLine2->Enabled=false;
    iClientReConnect=0; //Sam 20240226 : Client 重連次數
    OLPClientConnected=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    ShowMSG("OLPClient Connect fail : "+GetErrorMsg(Sender, ErrorCode));
    if(ErrorCode==10053)   // 網路線拔除
    {
        if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)   //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            ShowOLPState(2);
        }
    }
    else if(ErrorCode==10061) //連線失敗
    {
    }
    else
    {
    }

    ErrorCode=0;
    OnLine2->Enabled=false;
    try
    {
        Socket->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "OLPClient Error");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPClientConnecting(TObject *Sender,
      TCustomWinSocket *Socket)
{
    OnLine2->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::FormDestroy(TObject *Sender)
{
    try
    {
        SaveRecord();  //Steven 20141229 : OLP的Log要存檔
        OLPClient->Close();
        OLPServer->Close();
        tmrOLP->Enabled=false;
        if(CommandBuffer->Count ==0)
            delete CommandBuffer;

        CommandClientReplyBuffer->Clear();
        delete CommandClientReplyBuffer;

    //    delete Pbuffer;   //Steven 20120102 : 改為全域變數
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfAutomation::FormDestroy");
    }
    LogSoftwareOffTime("TfAutomation, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::ShowCharHexATK(bool bRead, char *Message, int Length)     //Steven 20120330 : ATK Site Map Monitorning
{
    AnsiString S1="", S2="";

    for(int i=0; i<Length; i++)
    {
        S2=AnsiString(IntToHex(Message[i], 1));
        if(S2.Length()==1)
        {
            S2="0"+S2;
        }
        S1+="<0x"+S2+">";
    }

    if(bRead)
    {
        S2.sprintf("%s [Receive] : %s", GetTimeInfo(), S1);
    }
    else
    {
        S2.sprintf("%s [Send   ] : %s", GetTimeInfo(), S1);
    }

    ShowMSG(S2);
}
//---------------------------------------------------------------------------
//ChungHung 20120413 add
bool charcmp(char* buf1,char* buf2,int size)
{
    for(int i=0; i<size; i++)
    {
        if(buf1[i] != buf2[i])
            return false;
    }

    return true;
}
//---------------------------------------------------------------------------
void TfAutomation::AmkorSendMessage(int Type)   //Steven 20120330 : ATK Site Map Monitorning
{
//1Byte         1Byte        1Byte          4Byte      N     1Byte
//Packet Count, Packet Type, Handler Model, Data Size, Data, Check Sum

//Packet Type Define : SITE MAP=0x01, TEMPERATURE=0x02
//Handler Model :
//Check Sum : Packet Count ~ Data N 使用 XOR

//Site MAP Data: Aa Site Num, Aa Enable/Disable, Ab Site Num... Ba Site Num, Ba Enable/Disable...
//Temperature Info Data: Temperature Mode(1Byte), Temperature(1Byte), Soak Time(2Byte)
//Temperature Mode: High=0x00, Ambient=0x01

    bool bSend=false;
    int i;
    if(CosFunction.bAmkorFunction || CUSTOMER_CODE==CC_QUALCOMM)     //JerryYang 20170412 (Steven) add QUALCOMM
    {
    }
    else
    {
        return;
    }

    if(Type==0)
    {
        AmkorUDPGetSite();
        //if(strcmp(sATKSendMap, sATKSendMessage)!=0)
        if(charcmp(sATKSendMap,sATKSendMessage,256)==false) //ChungHung 20120413 add
        {
            bSend=true;
            for(i=0; i<256; i++)
                sATKSendMap[i]=sATKSendMessage[i];
        }
    }
    else if(Type==1)
    {
        AmkorUDPGetTemp();
        //if(strcmp(sATKSendTemp, sATKSendMessage)!=0)
        if(charcmp(sATKSendTemp,sATKSendMessage,256)==false) //ChungHung 20120413 add
        {
            bSend=true;
            for(i=0; i<256; i++)
                sATKSendTemp[i]=sATKSendMessage[i];
        }
    }
    else
    {
        AmkorUDPGetSiteAndTemp();
        //if(strcmp(sATKSendTempAndMap, sATKSendMessage)!=0)
        if(charcmp(sATKSendTempAndMap,sATKSendMessage,256)==false) //ChungHung 20120413 add
        {
            bSend=true;
            for(i=0; i<256; i++)
                sATKSendTempAndMap[i]=sATKSendMessage[i];
        }
    }

    if(bSend)
    {
        NMUDP1->SendBuffer(sATKSendMessage, sATKSendMessageCount, sATKSendMessageCount);
        ShowCharHexATK(WRITE, sATKSendMessage, sATKSendMessageCount);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::NMUDP1DataReceived(TComponent *Sender,
      int NumberBytes, AnsiString FromIP, int Port)
{
    if(NumberBytes<=0) //ChungHung 20120410 add NumberBytes有可能出現負值
        return;

    char *buff=new char[NumberBytes+1];
    int i;
    AnsiString ReadMessage;
    NMUDP1->ReadBuffer(buff, 4, i);
    buff[NumberBytes]='\0';
    ShowCharHexATK(READ, buff, NumberBytes);
    ReadMessage="FromIP:" + FromIP + ":" + "Port:" + Port + ":" +"Bytes:" + NumberBytes;
    ShowMSG(ReadMessage);
    //delete buff;  //與法錯誤會照成亂刪記憶體
    delete[] buff;  //ChungHung 20131231 alter
    buff=NULL;  //kevin 20180517
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btUDPSendClick(TObject *Sender)
{
    AmkorSendMessage(2);
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btUDPTempClick(TObject *Sender)
{
    AmkorSendMessage(1);
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btUDPMapClick(TObject *Sender)
{
    AmkorSendMessage(0);
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::SckSendAlarmCode(AnsiString Text)  //ChungHung 20150518 add for SCK send Alarm Code
{
    SendOLPClient(Text);
    AnsiString S2;
    S2.sprintf("%s : %s", GetTimeInfo(), Text);
    ShowMSG(S2);
}
//---------------------------------------------------------------------------
int TfAutomation::GetEventStrResult(int num)    //Brian,20181025
{
    AnsiString str1,str2;
    if(num>=0 && num<iMaxEvent)
    {
        str1=MyEventResult[num].CommandString;
        str2=MyEventResult[num].NumString;
        if(str1=="PRODUCTION_REPLY" && str2=="0003")
        {
            return 1;
        }

        if(str1=="PRODUCTION_REQUEST" && str2=="0007")
        {
            return 2;
        }

        if(str1=="TEST_RESULT_REPLY" && str2=="")
        {
            return 3;
        }
    }
    return 4;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::edinputportMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//---------------------------------------------------------------------------
void TfAutomation::SendOLPClient(AnsiString Text)
{
    if(OLPClientConnected==true)
    {
        OLPClient->Socket->SendText(Text);
    }
}
//---------------------------------------------------------------------------
void TfAutomation::ShowMSG(AnsiString asStr)
{
    if(brecordmsgLock==false)
    {
        brecordmsgLock=true;
        if(Memo->Lines->Count>500)
        {
            SaveRecord();
            Memo->Lines->Clear();
        }
        Memo->Lines->Add(asStr);
        brecordmsgLock=false;
    }
    Memo->Lines->Add("");
}
//---------------------------------------------------------------------------
AnsiString TfAutomation::GetHTMLJSONDatas()
{
    AnsiString machineStatus = GetMachineStatus();
    AnsiString workOrder = GetWorkOrder();
    AnsiString sMainTemp = GetMainTemp();
    double temperatures[10];
    GetTemperatures(temperatures);

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "machineStatus", machineStatus.c_str());
    cJSON_AddStringToObject(root, "workOrder", workOrder.c_str());
    cJSON *temps = cJSON_CreateObject();
    for (int i = 0; i < 10; i++)
    {
        AnsiString sensor = AnsiString::Format("A%d", ARRAYOFCONST((i + 1)));
        AnsiString tempStr = AnsiString::Format("%.1f", ARRAYOFCONST((temperatures[i])));
        cJSON_AddStringToObject(temps, sensor.c_str(), tempStr.c_str());
    }
    cJSON_AddItemToObject(root, "temperatures", temps);
    cJSON_AddStringToObject(root, "MainTemp", sMainTemp.c_str());

    char *jsonStr = cJSON_PrintUnformatted(root);
    AnsiString result = jsonStr;
    cJSON_free(jsonStr);
    cJSON_Delete(root);
    return result;
}
//---------------------------------------------------------------------------
AnsiString TfAutomation::GetMachineStatus()
{
    return fMain->palMainStatus->Caption;
}
//---------------------------------------------------------------------------
AnsiString TfAutomation::GetWorkOrder()
{
    return fMain->cbSetupFileName->Text;
}
//---------------------------------------------------------------------------
void TfAutomation::GetTemperatures(double temps[10])
{
    for(int i=0;i<10;i++)
    {
        temps[i]=i;
    }
}
//---------------------------------------------------------------------------
AnsiString TfAutomation::GetMainTemp()
{
    return fMain->edWorkTemperBase->Text;
}
//---------------------------------------------------------------------------
