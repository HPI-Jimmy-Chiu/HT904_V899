//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop
#include "myQwertyKeyBoard.h"
#include "aseTest.h"
#include "common.h"
#include "cprod.h"
#include "main.h"
#include "cSortCT.h"
#include "cCounterClear.h"
#include "cMyDB.h"
#include "cmydef.h"
#include "csystem.h"
#include "MyMotor.h"
#include "MyKitSuck.h"
#include "note.h"
#include "cpublic.h"
#include "InterfaceSYS.h"
#include "cContactCT.h"
#include "mymessbox.h"
#include "cShowBinSelect.h"
#include "ATC_Handler_Side.h"
#include "mysensor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TASESendMessage *ASESendMessage;
LAST_GENERAL_SET1 ASE_SetConnect;
Handle_ASECommute ASECommute;
AnsiString asSendMess="";
//extern void SetRunStartMode(eRunStartMode Mode=rsmNull, AnsiString ModeText="");   //Steven 20120615 : 改成可用序號(Mode)或名稱(ModeText)進行模式改變
bool bShow=false; //kevin 20181018 add
//---------------------------------------------------------------------------
__fastcall TASESendMessage::TASESendMessage(TComponent* Owner)
        : TForm(Owner)
{
//    SocketReceiveLength=new TStringList;
//    SocketReceiveLength->Clear();
     bASE_AutoClean=false; //kevin 20160722 遠端啟動 autoclean
     bShow=true; //kevin 20181018 add
}
//===========================================================================
// 讀取設定資料檔
//===========================================================================
bool ReadLastDataFile()
{
    AnsiString Str1, Str2;
    ASE_SetConnect.ScanInterval=CheckAndReadIniData(asGeneralPath, "Setting",     "ScanInterval",        100);

    for(int i=0; i<ASEStationCount; i++)
    {
        Str1.sprintf("Station %d", i);
        Str2.sprintf("172.16.8.%d", 140+i);
        ASE_SetConnect.IP_ADDR[i]          =CheckAndReadIniData(asGeneralPath, Str1,     "IP Address",        Str2);
        ASE_SetConnect.Port[i]             =CheckAndReadIniData(asGeneralPath, Str1,     "Port",              10001);
    }
    return true;
}
//------------------------------------------------------------------------------
void __fastcall TASESendMessage::SaveDataClick(TObject *Sender)
{
    ASE_SetConnect.IP_ADDR[0]  =IPAddress1->Text;
    ASE_SetConnect.Port[0]     =atoi(IPPort1->Text.c_str());
    ASE_SetConnect.Port[1]     =atoi(IPPort2->Text.c_str()); //kevin 20141016
    WriteData();
    InitialDataToForm();
}
//---------------------------------------------------------------------------
void __fastcall TASESendMessage::ConnectClick(TObject *Sender)
{  //Client
    ASE_SetConnect.IP_ADDR[0]=IPAddress1->Text;
    ASE_SetConnect.Port[0]=atoi(IPPort1->Text.c_str());
    NMUDP1->RemoteHost = ASE_SetConnect.IP_ADDR[0];
    NMUDP1->RemotePort = ASE_SetConnect.Port[0];
}
//---------------------------------------------------------------------------
void TASESendMessage::InitialDataToForm()
{
    ReadLastDataFile();
    IPAddress1->Text    =AnsiString(ASE_SetConnect.IP_ADDR[0]);
    IPPort1->Text       =AnsiString(ASE_SetConnect.Port[0]);
    IPPort2->Text       =AnsiString(ASE_SetConnect.Port[1]);
}
//---------------------------------------------------------------------------
void __fastcall TASESendMessage::btnTestClick(TObject *Sender)
{
    SendToASEData(edtTestMsg->Text);
}
//---------------------------------------------------------------------------
//  設定連線參數 ASE 訊息
//---------------------------------------------------------------------------
bool TASESendMessage::initial()
{
    if(CheckIniData(asGeneralPath, "ASE_KaohSiung", "ClientIP")==false)
    {
        WriteIniDataGeneral("ASE_KaohSiung", "ClientIP", "127.0.0.1");
        ASE_SetConnect.IP_ADDR[0]="127.0.0.1";
    }
    else
    {
        ASE_SetConnect.IP_ADDR[0]=CheckAndReadIniDataGeneral("ASE_KaohSiung", "ClientIP", AnsiString("127.0.0.1"));
    }

    if(CheckIniData(asGeneralPath, "ASE_KaohSiung", "ClientPort")==false)
    {
        WriteIniDataGeneral("ASE_KaohSiung", "ClientPort", 9200);
        ASE_SetConnect.Port[0]= 9200;
    }
    else
    {
        ASE_SetConnect.Port[0]=CheckAndReadIniDataGeneral("ASE_KaohSiung", "ClientPort", 9200);
    }

    if(CheckIniData(asGeneralPath, "ASE_KaohSiung", "ServerPort")==false)
    {
        WriteIniDataGeneral("ASE_KaohSiung", "ServerPort", 9150);
        ASE_SetConnect.Port[1]= 9150;
    }
    else
    {
        ASE_SetConnect.Port[1]=CheckAndReadIniDataGeneral("ASE_KaohSiung", "ServerPort", 9150);
    }

    NMUDP1->RemoteHost = ASE_SetConnect.IP_ADDR[0];
    NMUDP1->RemotePort = ASE_SetConnect.Port[0];

    NMUDP1->LocalPort = ASE_SetConnect.Port[1];
    return true;
}
//---------------------------------------------------------------------------
//  SAVEData
//---------------------------------------------------------------------------
bool TASESendMessage::WriteData()
{
    WriteIniDataGeneral("ASE_KaohSiung", "ClientIP", IPAddress1->Text);
    WriteIniDataGeneral("ASE_KaohSiung", "ClientPort",IPPort1->Text);
    WriteIniDataGeneral("ASE_KaohSiung", "ServerPort",IPPort2->Text);

    return true;
}
//---------------------------------------------------------------------------
//  HANDLE -> ASE 訊息
//---------------------------------------------------------------------------
bool TASESendMessage::SendToASEData(AnsiString Data)
{
    if(bSystemClose==true || bShow==false) //kevin 20181018 ad //kevin 20150423 //kevin 20150423
        return false;
    AnsiString SBuffer1=""; //kevin 20211112 log eKeep Port

    int lenth=Data.Length();
    if(lenth==0)     //kevin 20150706 null
        return false;
    try
    {
        SBuffer1.sprintf("%s",Data);    //kevin 20211116 log eKeep Port
        /*   //kevin 20220906 mark
        if(SBuffer1.Pos("@HTLoader") || SBuffer1.Pos("@HTColor"))  //kevin 20211116 read 2D error Retry change Port
        {
            lenth=Data.Length();
            NMUDP1->RemotePort=9500;
            NMUDP1->SendBuffer(Data.c_str(), lenth, lenth);
        }
        else
        {
            NMUDP1->RemotePort=ASE_SetConnect.Port[0];//kevin 20160531
            lenth=SBuffer1.Length();
            NMUDP1->SendBuffer(SBuffer1.c_str(), lenth, lenth); //kevin 20150703
        }*/
        NMUDP1->RemotePort=ASE_SetConnect.Port[0];//kevin 20160531
        lenth=SBuffer1.Length();
        NMUDP1->SendBuffer(SBuffer1.c_str(), lenth, lenth); //kevin 20150703
    }
    catch(...)
    {
        UDPErrorLog(ASE_SetConnect.Port[0], SBuffer1);   //kevin 20211020 UDP error log
        //NMUDP1->Destroying();
    }
    return true;
}
//--------------------------------------------------------------------------
//  ASE -> Handle 訊息
//---------------------------------------------------------------------------
bool TASESendMessage::ReceveToHandleData(AnsiString Data)
{
    AnsiString Buffer1="",Buffer2="",Buffer3="";
    int T1=0,T2=0,T3=0;//,T4=0;
    if(Data.Pos("@e01001Schedule=") &&Data.Pos("InQty="))//接收資料
    {
        if(HasICUnderMachine())                                                 //kevin 20141202
        {
            Buffer1="Prod";
            SendToASEData(Buffer1);
            ShowErrorMessage("MES1646", 0, MMSystem, false, "ReceveToHandleData");   //Must finish [Clean out]!!
            return false;
        }
        T1=Data.Pos(",");
        T2=T1-17;
        Buffer1 = Data.SubString(17,T2);

        T3= T1+7;
        Buffer1 = Data.SubString(17,T2);
        Buffer2 = Data.SubString(T3,5);
        strncpy(ASECommute.Schedule, Buffer1.c_str(), sizeof(ASECommute.Schedule));
        fMain->cbRunStartMode->ItemIndex=1;//kevin 20141020 initial start
        fMain->cbRunStartMode->Text=StartModeName[1];
        //fMain->cbRunStartModeChange(fMain);  kevin 20160727
        Memo1->Lines ->Add(Buffer1);
        ASET_INTQTY=Buffer2;
        ASECommute.RevInQty= atoi(Buffer2.c_str());
        Memo1->Lines ->Add(Buffer2);
    }
//-----------------面板 按鍵---------------------------------------
    else if(Data.Pos("@e01101"))//Power ON   Handler接收後處理指令 正常回傳OK, 其餘皆為異常
    {
//        if(fMain->Start())
//        {
//            RecordProcess("ASE_Push_Power ON OK");
//            Buffer1="ok";
//        }
//        else
//        {
//            RecordProcess("ASE_Push_Power ON NG");
//            Buffer1="ng";
//        }
//        SendToASEData(Buffer1);
    }
    else if(Data.Pos("@e01102"))//Power Off
    {
//        if(fMain->Start())
//        {
//            RecordProcess("ASE_Push_Power Off OK");
//            Buffer1="ok";
//        }
//        else
//        {
//            RecordProcess("ASE_Push_Power Off NG");
//            Buffer1="ng";
//        }
//        SendToASEData(Buffer1);
    }
    else if(Data.Pos("@e01103"))//START
    {
        if(fMain->Start("ASE_Push_START"))
        {
            //bAseStart=true;                      //kevin 20231205 mark
            RecordProcess("ASE_Push_START Done");
            Buffer1="@e02103Done";
        }
        else
        {
            RecordProcess("ASE_Push_START Fail");
            Buffer1="@e02103Fail";
        }
        SendToASEData(Buffer1);
    }
    else if(Data.Pos("@e01104"))//HOME
    {
        iAseHome=1;//kevin 20150925
        if(fMain->Home("ASE_Push_Home"))
        {
            bAseHome=true;
            RecordProcess("ASE_Push_Home Done");
           // Buffer1="@e02104Done";
        }
        else
        {
            RecordProcess("ASE_Push_Home Fail");
            Buffer1="@e02104Fail";
            SendToASEData(Buffer1);
        }
    }
    else if(Data.Pos("@e01105"))//PAUSE
    {
        if(fMain->Pause("ASE_Push_Pause"))
        {
            bAsePause=true;
            RecordProcess("ASE_Push_Pause Done");
            Buffer1="@e02105Done";
        }
        else
        {
            RecordProcess("ASE_Push_Pause Fail");
            Buffer1="@e02105Fail";
        }
        SendToASEData(Buffer1);
    }
    else if(Data.Pos("@e01106"))//RESET
    {
        if(fMain->Reset("ASE_Push_Reset"))
        {
            bAseReset=true;
            RecordProcess("ASE_Push_Reset Done");
            Buffer1="@e02106Done";
        }
        else
        {
            RecordProcess("ASE_Push_Reset Fail");
            Buffer1="@e02106Fail";
        }
        SendToASEData(Buffer1);
    }
    else if(Data.Pos("@e01107"))//CLEAN OUT
    {
        iAseCleanOut=1;
        if(fMain->CleanOut("ASE_ReceveToHandleData"))
        {
            bAseCleanOut=true;
            RecordProcess("ASE_Push_CleanOut Done");
            Buffer1="@e02107Done";
        }
        else
        {
            RecordProcess("ASE_Push_CleanOut Fail");
            Buffer1="@e02107Fail";
        }
        SendToASEData(Buffer1);
    }
    else if(Data.Pos("@e01108"))//SKIP
    {
        bAseSKIP=true;
        RecordProcess("ASE_Push_SKIP Done");
        Buffer1="@e02108Done";
        SendToASEData(Buffer1);
    }
    else if(Data.Pos("@e01109"))//RETRY
    {
        bAseRetry=true;//kevin 20150427
        RecordProcess("ASE_Push_RETRY Done"); //kevin 20150727
        Buffer1="@e02109Done";
        SendToASEData(Buffer1);   //kevin 20150727 回應 ae RETRY finish
    }
    else if(Data.Pos("@e01110"))//ONE CYCLE
    {
        iAseOneCycle=1; //kevin 20150925
        bAseOneCycle=true;
        fMain->BtnOneCycleClick(fMain);
        RecordProcess("ASE_Push_OneCycle Done");
        Buffer1="@e02110Done";
        SendToASEData(Buffer1);
    }
    else if(Data.Pos("@e01111"))//ALARM RESET
    {
        iAseAlarmReset=1;
        bAlarmBuzzer=false;
        bLampAlarmReset=false;
        SW[SwFKAlarmReset].Off();
        SW[SwRKAlarmReset].Off();
        RecordProcess("ASE_ALARM RESET pressed");

        Buffer1="@e02111Done";      //kevin 20150608 回應 ase RESET finish
        SendToASEData(Buffer1);
    }
    else if(Data.Pos("@e01112"))//TRAY END
    {
        bAseTrayEnd=true;
        iAseTrayEnd=1;//kevin 20150925
        RecordProcess("ASE_TRAY END pressed");
        Buffer1="@e02112Done";    //kevin 20150608 回應 ase TRAY END finish
        SendToASEData(Buffer1);
    }
    else if(Data.Pos("@e01113"))//TRAY FEED
    {
        bAseTrayFeed=true;       //kevin 20210809 add TRAY FEED remote
        iAseTrayFeed=1;
        RecordProcess("ASE_TRAY FEED pressed");
    }
    else if(Data.Pos("@e01115"))// T. START
    {
        bTSTART=true;
        RecordProcess("ASE_TRAY T. START pressed");
    }
    else if(Data.Pos("@e01116"))//STEP
    {
        bSTEP=true;
        RecordProcess("ASE_TRAY STEP pressed");
    }
    else if(Data.Pos("@e02001"))//傳送 handle 記錄資料
    {
    }
    else if(Data.Pos("@e02002"))//保留
    {
    }

    Memo1->Lines ->Add(Data);

    return true;
}
//---------------------------------------------------------------------------
void __fastcall TASESendMessage::Button2Click(TObject *Sender)
{
    ASE_SetConnect.Port[1]=atoi(IPPort2->Text.c_str());
    NMUDP1->LocalPort = ASE_SetConnect.Port[1];
}
//---------------------------------------------------------------------------
void __fastcall TASESendMessage::NMUDP1DataReceived(TComponent *Sender,
      int NumberBytes, AnsiString FromIP, int Port)
{
    static int iCount=0;
    AnsiString Buffer1;
    AnsiString Buffer, Buffer2;
    int iret=0, iAutoFullTray=0;                                                //kevin 20150831
    if(Port==9200)                                                              //kevin 20200220 add  ekeep server
        return;

    if(NumberBytes<=0)                                                          //ChungHung 20120410 add NumberBytes有可能出現負值
        return;

    char *buff=new char[NumberBytes+1];
    int i, lenth=0;
    bool bflag=false;                                                           //kevin 20160718
    AnsiString ReadMessage;
    AnsiString Str;
    NMUDP1->ReadBuffer(buff, 4, i);
    buff[NumberBytes]='\0';
    ReadMessage =buff;
    if(iCount>=9)
        iCount=0;

    if(buff=="")                                                                //kevin 20210927 add
        return;

    ReceviceData[iCount]= buff;

    if(ReceviceData[iCount].Pos("@e01002"))                                     //回傳 bin 數量資料
    {
        Buffer1="";
        for(int i=0; i<eTrayCount; i++)
        {
            if(Prod.iTrayType[i]!=tNotUse)
            {
                if(Buffer1=="")
                    Str.sprintf("%s=%d",  s6TrayName[i], LastSet.BinCT[0][iTo3Unload[i]]);
                else
                    Str.sprintf(",%s=%d", s6TrayName[i], LastSet.BinCT[0][iTo3Unload[i]]);
                Buffer1=Buffer1+Str;
            }
        }

        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(), lenth, lenth);                      //kevin 20150703
        delete[] buff;                                                          //kevin 20150119 add ChungHung 20131231 alter
        buff=NULL;                                                              //kevin 20180517
        RecordProcess("ASE_Return bin Data");                                   //kevin 20170814 (Steven) add
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01003"))                                //要求清除Sort Counter數量
    {
        if(HasICUnderMachine())                                                 //kevin 20141202
        {
            Buffer1="NG";
        }
        else
        {
            Buffer1="OK";
        }
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;

        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(), lenth, lenth);                      //kevin 20150703
        delete[] buff;                                                          //kevin 20150119 add ChungHung 20131231 alter
        buff=NULL;                                                              //kevin 20180517
        if(Buffer1=="OK")
        {
            MyDBIProductionData("ASE Clear Sorting Count");                     //Steven 20140816 : Production Data
            fMain->Clarn_Data(8, "ASE Clear Sorting Count");

            NMUDP1->RemoteHost=FromIP;
            NMUDP1->RemotePort=Port;
            Buffer1="OK";
        }
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01004"))                                //kevin 20141202  要求傳回目前機台狀態
    {
        ASE_ReturnState= fMain->palMainStatus->Caption;                         //kevin 20150925
        Buffer1=ASE_ReturnState;                                                //kevin 20150925
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);                       //kevin 20150703
        delete[] buff;                                                          //kevin 20150119 add ChungHung 20131231 alter
        buff=NULL;                                                              //kevin 20180517
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01005"))                                //kevin 20150831  ReFresh add
    {                                                                           // Handler 重新載入Job File (JOBFILE)   in D:\HT9045\IniData\Data\JOBFILE  and D:\HT9045\IniData\Offset\JOBFILE
        RecordProcess("ASE_Refresh_JOBFILE");
        if(HasICUnderMachine()||HasAnyICInMachine())                            //kevin 20150914
        {
            iret=1;
            Buffer1="NG";
        }
        else
        {
            Buffer="JOBFILE";
            if(fMain->ChoiceJobName(Buffer.Trim())==false)
            {
                iret=2;                                                         //kevin 20130812 找不到工作檔
                Buffer1="OK";
            }
            else
            {
                Buffer1="OK";
                bRefreshFunction=true;
            }
        }

        if(iret==1)                                                             //kevin 20141017  生產中不能任意改工作檔
            fMain->bJobFileProd=true;
        else if(iret==2)
            fMain->bRefresh=true;                                               //kevin 20130812

        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);                       //kevin 20150703
        delete[] buff;                                                          //kevin 20150119 add ChungHung 20131231 alter
        buff=NULL;                                                              //kevin 20180517
        return;
    }
    else if( ReceviceData[iCount].Pos("@e01006"))                               //kevin 20150831  ReFresh tmperature offset add
    {                                                                           // Handler 重新載入Job File (JOBFILE)   in D:\HT9045\IniData\Data\JOBFILE  and D:\HT9045\IniData\Offset\JOBFILE
        iSendChangeTempError=0;                                                 //kevin 20160531
        if(bTestOverTimeTempOffsetF)                                            //kevin 20160414  目前執行溫度加熱中 就不補OFFSET
        {
            Buffer1="NG";
            Buffer = "ASE_Tmperature_offset Initial Temperature runing no offset temp.";  //kevin 20160414
        }
        else
        {
            // RecordProcess("ASE_Tmperature_offset");                          //kevin 20160414
            Buffer = "ASE_Tmperature_offset";                                   //kevin 20160414
            bSendChangeTemp=true;                                               //kevin 20151023
            bSendChangeTempError=false;
            bSendChangeTempDelayAlarm=false;
            if(fMain->ReadTempData())
            {
                bSendChangeTempDelayAlarm=true;
                tSendChangeTempDelay.SetSecAndOn(120);                          //生產中更新溫度OFFSET 2分鐘
//                StartTemptim=MyTickCount();
            }
        }

        if(iSendChangeTempError==0)                                             //kevin 20160531
        {
            Buffer1="OK";
            bSetTempChange=true;                                                ///kevin 20220804 add eKeep send temp offset
        }
        else
        {
            Buffer1="NG";
        }
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(), lenth, lenth);                      //kevin 20150703
        delete[] buff;                                                          //kevin 20150119 add ChungHung 20131231 alter
        buff=NULL;                                                              //kevin 20180517

        NMUDP1->RemotePort=ASE_SetConnect.Port[0];
        RecordProcess("ASE_Tmperature_offset");                                 //kevin 20160414

        if(iSendChangeTempError!=0)
            RespondASECom("@e02003");                                           //kevin 20160531 回應 ase 溫度異常

        switch(iSendChangeTempError)
        {
            case 1:
                ShowErrorMessage("WAR16106", K_RETRY, MMSystem); //20160531
                break;
            case 2:
                ShowErrorMessage("WAR16107", K_RETRY, MMSystem); //20160531
                break;
            case 3:
                ShowErrorMessage("WAR16108", K_RETRY, MMSystem); //20160531
                break;
        }
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01007"))                                //kevin 20160121  AUTOSITMAP 啟用  add
    {
        if(HasICUnderMachine() || HasAnyICInMachine() || SystemStart)           //AutoSiteMap 不能再主畫面關site)
        {
            Buffer1="NG";
        }
        else
        {
            Buffer1="OK";
        }
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);                       //kevin 20150703
        delete[] buff;                                                          //kevin 20150119 add ChungHung 20131231 alter
        buff=NULL;                                                              //kevin 20180517
        if( Buffer1=="OK")
        {
            if(fMain->CheckSiteMapState(true)==false)                           //kevin 20160125 有做過 auto sitemap
            {
                fMain->cbRunStartMode->Text=StartModeName[4];
                fMain->cbRunStartModeChange(this);
            }                                                                   //kevin 20160125 end
            IniConfig.bI21EnableASM=true;
            RecordProcess("ASE_AutoSitemap_OK");
        }
        else
        {
            RecordProcess("ASE_AutoSitemap_NG");
        }
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01008"))                                //kevin 20160121  AUTOSITMAP 停用  add
    {
        if(HasICUnderMachine()||HasAnyICInMachine())                            //kevin 20150914
        {
            Buffer1="NG";
        }
        else
        {
            Buffer1="OK";
            IniConfig.bI21EnableASM=false;
            fMain->cbRunStartMode->Text=StartModeName[1];
            fMain->cbRunStartModeChange(this);
        }
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);                       //kevin 20150703
        delete[] buff;                                                          //kevin 20150119 add ChungHung 20131231 alter
        buff=NULL;                                                              //kevin 20180517
        RecordProcess("ASE_AutoSitemap_Disable");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01012"))                                //kevin 20170211 (wei) 回傳機台最後一次Contact Force 的測量值
    {
        Buffer1=Ase_SendEP;
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;                                                              //kevin 20180517
        RecordProcess("ASE_ReadEP");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01013"))                                //kevin 20170211 (wei) 回傳機台最後一次Temperature 的測量值
    {
        Buffer1=Ase_SendTemp;
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;                                                              //kevin 20180517
        RecordProcess("ASE_ReadTemperature");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01016"))                                //kevin 20160630  ESD 重新讀取設定值
    {
        if(HasICUnderMachine()||HasAnyICInMachine())
        {
            Buffer1="NG";
        }
        else
        {
            Buffer1="OK";
        }
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517

        if(Buffer1=="OK")
            SendCommand_ESD(ESD_Read_ESD_SettDATA);  //kevin 20160630

        RecordProcess("ASE_ESD_UPDATA");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01017"))//kevin 20160718  生產中關SITE
    {
        if(IniConfig.bN11_1CleanOutCloseSite)  //kevin
        {
            Buffer1="OK";
            bASECleanOutCloseSite=true;                   //kevin 20160715 ASE clean out close Site 不退tray
            fMain->CleanOut("ASE_NMUDP1DataReceived");
        }
        else
        {
            Buffer1="FunctionDisable";
        }

        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        RecordProcess("ASE_CleanOut_Close_Site");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01018"))//kevin 20160718  AutoClean
    {
       if(TestIF.iAutoClean_Function==false)
           Buffer1="function disable.";
       else
       {
           Buffer1="OK";
           bflag=true;  //kevin 20160718
       }
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517

        if(bflag)  //kevin 20160718
        {
            bASE_AutoClean=true; //kevin 20160722 遠端啟動 autoclean
            RecordProcess("ASE_AutoClean Do" );
        }
        else
        {
            bASE_AutoClean=false; //kevin 20160722 遠端啟動 autoclean
            RecordProcess("ASE_AutoClean No");
        }
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01019"))    //kevin 20170211
    {
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(), lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        bASE_AutoClean=false; //kevin 20160722 遠端啟動 autoclean
        RecordProcess("ASE_Null");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01022"))    //kevin 20170210 (wei) 回傳現在記憶體中, Index Time(Arm),Inmdex Cycle Time,Test Time,UPH數值
    {                                          //1. 取小數第一位 2. 以逗號(,)分隔     3. 回傳順序: Index Time(Arm),Inmdex Cycle Time,Test Time,UPH
        for(int i=0; i<4; i++)
        {
            if(i<=3)
            {
                if(dSend_ASEData[i]=="")
                    Buffer1+="NA,";
                else
                    Buffer1+=dSend_ASEData[i]+",";
            }
            else
            {
                Buffer1+=dSend_ASEData[i];
            }
        }
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(), lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        RecordProcess("ASE_Indextime");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01023"))//kevin 20170817 (Steven) 回傳現在Site1~Site16 Yield 以逗號(,)分隔, Site1~Site16 Yield
    {
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        Buffer1 = ASE_Yield[3];
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517

        bASE_AutoClean=false; //kevin 20160722 遠端啟動 autoclean
        RecordProcess("ASE_Site yield");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01024"))//kevin 20170817 (Steven) 回傳現在Arm1, Arm2 Yield 以逗號(,)分隔, Arm1, Arm2 Yield
    {
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;

        Buffer1.sprintf("Arm1:%sArm2:%s",ASE_Yield[1],ASE_Yield[2]);
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        bASE_AutoClean=false; //kevin 20160722 遠端啟動 autoclean
        RecordProcess("ASE_Arm1,Arm2 Site yield");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01025"))//kevin 20170817 (Steven) 回傳現在Auto1~Auto3, Fix1~Fix3 Yield以逗號(,)分隔, Auto1~Auto3, Fix1~Fix3 Yield
    {
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        Buffer1 = ASE_Yield[0];

        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        RecordProcess("ASE_Auto1~Auto3, Fix1~Fix3 Yield");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01026"))//kevin 20170829 (Steven) 要求Handler 開始執行 RT1, RT2, ...
    {                                            //If 該機台尚未設定Auto RT, 則回傳NonSetting 其餘回傳錯誤訊息 此功能等同Handler 詢問 OP是否執行ART, OP選擇Yes
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        if(HasICUnderMachine()||HasAnyICInMachine())
        {
            Buffer1="NG";
            bASEARTStart=false;                            //kevin 20170830 (Steven) ASE_KH ART SEND START
        }
        else if(iAutoRetestLimit>=2 && (TestIF_File.bEnablePassYieldART==true ||
            TestIF_File.bEnableOpenShortART==true||TestIF_File.bEnableRecoverART==true))
        {
            Buffer1= "OK";
        }
        else
        {
            Buffer1= "NonSetting";
        }

        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        if(Buffer1== "OK")
        {
            fMain->cbRunStartMode->ItemIndex=3;//kevin 20141020 initial start
            fMain->cbRunStartMode->Text=StartModeName[8];
            SetRunStartMode(rsmInitial_ART);
            bASEARTStart=true;                            //kevin 20170830 (Steven) ASE_KH ART SEND START
        }
        RecordProcess("ASE_ART_Initial");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01027"))//kevin 20170829 (Steven) 要求Handler 開始執行 RT1, RT2, ...
    {                                            //If 該機台尚未設定Auto RT, 則回傳NonSetting 其餘回傳錯誤訊息 此功能等同Handler 詢問 OP是否執行ART, OP選擇Yes
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        if(HasICUnderMachine()||HasAnyICInMachine())
        {
            Buffer1="NG";
            bASEARTStart=false;                            //kevin 20170830 ASE_KH ART SEND START
        }
        else if(iAutoRetestLimit>=2 && (TestIF_File.bEnablePassYieldART==true ||
                TestIF_File.bEnableOpenShortART==true||TestIF_File.bEnableRecoverART==true))
        {
            Buffer1= "OK";
            bASEARTStart=true;                         //kevin 20170830 ASE_KH ART SEND START
        }
        else
        {
            Buffer1= "NonSetting";
        }

        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        if(Buffer1== "OK")
        {
            fMain->cbRunStartMode->ItemIndex=4;//kevin 20141020 initial start
            fMain->cbRunStartMode->Text=StartModeName[10];
            SetRunStartMode(rsmContinuRetest_ART);
        }
        RecordProcess("ASE_ART_RT");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01028Y"))//kevin 20170901 (Steven) Y 做ART  N 就是LOAD 重新LOAD TRAY
    {                                            //If 該機台尚未設定Auto RT, 則回傳NonSetting 其餘回傳錯誤訊息 此功能等同Handler 詢問 OP是否執行ART, OP選擇Yes
        if(iAutoRetestLimit >=2 && (TestIF_File.bEnablePassYieldART==true ||
           TestIF_File.bEnableOpenShortART==true||TestIF_File.bEnableRecoverART==true))
        {
            Buffer1="OK";
        }
        else
        {
            Buffer1="NG";
        }

        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        if(Buffer1=="OK")
        {
            MyMessageBox->pnlYesClick(MyMessageBox->pnlYes);
        }
        RecordProcess("ASE_ART_CleanOut");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01028N"))//kevin 20170901 (Steven) Y 做ART  N 就是LOAD 重新LOAD TRAY
    {                                            //If 該機台尚未設定Auto RT, 則回傳NonSetting 其餘回傳錯誤訊息 此功能等同Handler 詢問 OP是否執行ART, OP選擇Yes
        if(iAutoRetestLimit >=2 && (TestIF_File.bEnablePassYieldART==true ||
            TestIF_File.bEnableOpenShortART==true||TestIF_File.bEnableRecoverART==true))
        {
            Buffer1="OK";
        }
        else
        {
            Buffer1="NG";
        }

        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        if(Buffer1== "OK")
        {
            MyMessageBox->pnlYesClick(MyMessageBox->pnlNo);
        }
        RecordProcess("ASE_ART_CleanOut");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01029"))//kevin 20180703 add  傳回目前機台Auto123 & Fix123中 各Bin數量??
    {
        AnsiString Buffer2="", Buffer3="";
        int iTray=0,iold=11;
        bool bHasIC=false;
        for(int j=0; j<6; j++)
        {
            bHasIC=false;
            if(j>=3)
            {
                iTray=j-2;
                Buffer1+= Buffer2.sprintf("Fix%d:",iTray);   //kevin 20180703 bin 數量 Bin total[0]
            }
            else
            {
                Buffer1+= Buffer2.sprintf("Auto%d:",j+1);   //kevin 20180703 bin 數量 Bin total[0]
            }

            for(int i=0; i<=iTestBinCount; i++)
            {
                //if(fShowBinSelect->MyBinSel[j]->Caption.Pos(i))
                //if(CompileWord(fShowBinSelect->MyBinSel[j]->Caption,i))
                if(bUnloadHasBin[j])                         //kevin 20180705 有BIN TRAY set true)
                {
                    Buffer3=i;
                    if(i==iTestBinCount)       //error bin
                        Buffer3="E";

                    if(CompileWord(BinAssign[j],Buffer3))
                    {
                        if(iByBinTotal[i]!=0)   //kevin 20180703 bin 數量
                        {
                            if(i== iTrayLastBin[j]||(i==iTestBinCount &&iTrayLastBin[j]==999) )                                  //kevin 20180705 每一個TRAY最後一個BIN  Error bin)
                                Buffer1+= Buffer2.sprintf("Bin%s=%d;",Buffer3,iByBinTotal[i]);   //kevin 20180703 bin 數量 Bin total[0]
                            else
                                Buffer1+= Buffer2.sprintf("Bin%s=%d,",Buffer3,iByBinTotal[i]);   //kevin 20180703 bin 數量 Bin total[0]
                            bHasIC=true;
                        }
                        else
                        {
                            /*
                            if((i==iTestBinCount-1)&&bHasIC==false)
                            {
                                Buffer1+= "NA;";
                            }
                            else if(i==(iTestBinCount-1)&&bHasIC)
                                    Buffer1+= ";";
                            */
                            if(i==iTrayLastBin[j]|| (i==iTestBinCount &&iTrayLastBin[j]==999)) //最後一個BIN
                                Buffer1+= Buffer2.sprintf("Bin%s=%d;",Buffer3,iByBinTotal[i]);   //kevin 20180703 bin 數量 Bin total[0]
                            else
                                Buffer1+= Buffer2.sprintf("Bin%s=%d,",Buffer3,iByBinTotal[i]);   //kevin 20180703 bin 數量 Bin total[0]
                        }
                        iold = j;
                    }
                }
                else
                {
                    if(j!=iold && bHasIC==false)
                    {
                        iold = j;
                        Buffer1+= "NA;";
                    }
                }
            }
        }
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        if(Buffer1=="")
           Buffer1="NULL";
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        RecordProcess("ASE_BinTotal");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01031"))//kevin 20180517 add 生產前OP OFF_LINE 強制 On line
    {
        Buffer1= "OK";
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        if(ReceviceData[iCount].Pos("@e01031Y"))// 生產前OP OFF_LINE 強制 On line
        {
            IniConfig.bI40_bStartProductOnLine=true;
            RecordProcess("ASE_Auto_OnLine");
        }
        else if(ReceviceData[iCount].Pos("@e01031N"))// 生產前OP OFF_LINE 強制 On line
        {
            IniConfig.bI40_bStartProductOnLine=false;
            RecordProcess("ASE_Auto_OFFLine");
        }
    }
    else if(ReceviceData[iCount].Pos("@e01039"))//kevin 20190306 add one cycle load cell測試
    {
        Buffer1= "OK";
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        //InitialTestHeadMotorTask();
        AsebLoadCellTest =true;     //kevin 20190305 add one cycle run arm 1 arm 2 load cell test
        fMain->BtnOneCycleClick(fMain->BtnOneCycle);
        RecordProcess("ASE_LoadCell Test");
    }
    else if(ReceviceData[iCount].Pos("@e01041OK"))//kevin 20190306 add one cycle load cell測試
    {
        Buffer1= "OK";
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        //InitialTestHeadMotorTask();
        bloadcellRece=true;
        sLoadCellReceData="";           //kevin 20190906 add load cell NG
        iloadcellRece=1;                  //kevin 20190906 add load cell command
    }
    else if(ReceviceData[iCount].Pos("@e01041NG"))//kevin 20190306 add one cycle load cell測試
    {
        Buffer1= "NG";
        lenth=Buffer1.Length();
        //NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        RecordProcess("ASE_LoadCell NG");
        sLoadCellReceData=ReceviceData[iCount];           //kevin 20190906 add load cell NG
        iloadcellRece=2;                  //kevin 20190906 add load cell command
        bloadcellRece=true;
    }
    else if(ReceviceData[iCount].Pos("@e01042"))//kevin 20210406 add  clean out finish
    {
        if(bCleanOutFinish == false)                     //kevin 20210406 clean out open close Site
            Buffer1= "NG";
        else
        {
            Buffer1= "OK";
            beKeepCloseSite = true;                      //kevin 20210406 clean out open close Site
        }
        lenth=Buffer1.Length();
        delete[] buff;
        buff=NULL;
        RecordProcess("ASE_OpenCloseSite.");
    }
    else if(ReceviceData[iCount].Pos("@e01120"))    //kevin 20220709   增加 @e01020 於單流道退出使用, command + 流道 @e01020Auto1 - 退出 Auto1 流道
    {
        i=ReceviceData[iCount].Pos("Auto");
        Buffer=ReceviceData[iCount].SubString(i+4, 1);
        Buffer1="OK";
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(), lenth, lenth);
        delete[] buff;
        buff=NULL;

        try
        {                                                                       //kevin 20220709 AGV 要搬TRAY 需退AUTO
            iAutoFullTray=StrToInt(Buffer)-1;                                   //kevin 20220709 AGV 要搬TRAY 需退AUTO 1,2,3
            MOT[iMMAuto[iAutoFullTray]].InitNewTray(HAS_IC, false, __FUNC__);   //FULL TRAY RECE TRAY
            bAutoReceTray=true;                                                 //kevin 20220908 add 退料TRAY 補空TRAY
        }
        catch(...)
        {
            iAutoFullTray=0;
        }
        Buffer1.sprintf("ASE_Auto Full AUTO %d", iAutoFullTray);
        RecordProcess(Buffer1);
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01121"))                                //kevin 20220709   增加 Input: IO number (Ex: A01)    Output: IO number=On/Off (Ex: A01=On)
    {
        if(ReceviceData[iCount].Pos("A01"))
        {
            if(Sen[SnAseTrayBufferLeft].Enable==false)
                Buffer1="IO no setup";
            else if(Sen[SnAseTrayBufferLeft].IsOn())
                Buffer1="A01=On";
            else
                Buffer1="A01=Off";
        }
        else if(ReceviceData[iCount].Pos("A02"))
        {
            if(Sen[SnAseTrayBufferRight].Enable==false)
                Buffer1="IO no setup";
            else if(Sen[SnAseTrayBufferRight].IsOn())
                Buffer1="A02=On";
            else
                Buffer1="A02=Off";
        }
        else if(ReceviceData[iCount].Pos("B01"))             //kevin 2023023 add     //kevin 20221101 add respond has tray state   load - >  auto3
        {
            if(Sen[SnLoaderTrayHasTray].Enable==false)
                Buffer1="IO no setup";
            else if(Sen[SnLoaderTrayHasTray].IsOn())
                Buffer1="B01=On";
            else
                Buffer1="B01=Off";
        }
        else if(ReceviceData[iCount].Pos("B02"))
        {
            if(Sen[SenEmptySelectHasTray].Enable==false)
                Buffer1="IO no setup";
            else if(Sen[SenEmptySelectHasTray].IsOn())
                Buffer1="B02=On";
            else
                Buffer1="B02=Off";
        }
        else if(ReceviceData[iCount].Pos("B03"))
        {
            if(Sen[SenColorSelectHasTray].Enable==false)
                Buffer1="IO no setup";
            else if(Sen[SenColorSelectHasTray].IsOn())
                Buffer1="B03=On";
            else
                Buffer1="B03=Off";
        }
        else if(ReceviceData[iCount].Pos("B04"))
        {
            if(Sen[SnAuto1TrayHasTray].Enable==false)
                Buffer1="IO no setup";
            else if(Sen[SnAuto1TrayHasTray].IsOn())
                Buffer1="B04=On";
            else
                Buffer1="B04=Off";
        }
        else if(ReceviceData[iCount].Pos("B05"))
        {
            if(Sen[SnAuto2TrayHasTray].Enable==false)
                Buffer1="IO no setup";
            else if(Sen[SnAuto2TrayHasTray].IsOn())
                Buffer1="B05=On";
            else
                Buffer1="B05=Off";
        }
        else if(ReceviceData[iCount].Pos("B06"))                     //kevin 20221101 add respond has tray state  load - >  auto3
        {
            if(Sen[SnAuto3TrayHasTray].Enable==false)
                Buffer1="IO no setup";
            else if(Sen[SnAuto3TrayHasTray].IsOn())
                Buffer1="B06=On";
            else
                Buffer1="B06=Off";
        }
        else if(ReceviceData[iCount].Pos("C01"))                     //kevin 20221101 add respond has tray state  load - >  auto3
        {
            if(Sen[SnColorIsFull].Enable==false)
                Buffer1="IO no setup";
            else if(Sen[SnColorIsFull].IsOn())
                Buffer1="C01=On";
            else
                Buffer1="C01=Off";
        }
        else
        {
            Buffer1="NG";
        }

        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(), lenth, lenth);
        delete[] buff;
        buff=NULL;

        Buffer1.sprintf("ASE_VM Tray %s",ReceviceData[iCount]);
        RecordProcess(Buffer1);
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01122"))                                //kevin 20220829   增加 change QA Mode
    {
        if(HasICUnderMachine()||HasAnyICInMachine())
        {
            iret=1;
            Buffer1="NG";
        }
        else
        {
            SetRunStartMode(rsmQAMode);
            Buffer1="OK";
        }

        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(), lenth, lenth);
        delete[] buff;
        buff=NULL;

        Buffer.sprintf("ASE_ChangeQAMode: %s",Buffer1);
        RecordProcess(Buffer);
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01124"))//kevin 20230321 add music
    {
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        bAlarmBuzzer=true;     //kevin 20230321 add sound eKeep call music
        Buffer1="OK";
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        RecordProcess("ASE_Music");
        return;
    }
    else if(ReceviceData[iCount].Pos("@e01126"))//Ken 20230328 add Vleanout Load Receive Tray
    {
        NMUDP1->RemoteHost=FromIP;
        NMUDP1->RemotePort=Port;
        if(iCleanOut)
        {
            Buffer1="OK";
            bLoadBFBackTray=true;
        }
        else
        {
            bLoadBFBackTray=false;
            Buffer1="NG";
        }

        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        Buffer2.sprintf("ASE_loadReceiveTray %s",Buffer1);
        RecordProcess(Buffer2);
        return;
    }
    else if(ReceviceData[iCount].Pos("@e02025"))//kevin 20210623 add load read tray ID
    {
        Buffer=ReceviceData[iCount];
        lenth=Buffer.Length();
        Buffer2=ReceviceData[iCount].SubString(8, lenth-7);
        TrayID[0][0]=Buffer2;                          //kevin 20210623  [6]: load empty  [2]: 0:read  1: send pick pos
        Buffer1="OK";
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        bReadTrayID =true;                          //kevin 20210623 TRAY ID Read
        Buffer.sprintf("ASE_LoadTrayID: %s", TrayID[0][0]);
        RecordProcess(Buffer);
    }
    else if(ReceviceData[iCount].Pos("@e02026"))//kevin 20210623 add load read tray ID
    {
        Buffer=ReceviceData[iCount];
        lenth=Buffer.Length();
        Buffer2=ReceviceData[iCount].SubString(8,lenth-7);
        TrayID[2][0]=Buffer2;                          //kevin 20210623  [6]: load empty  [2]: 0:read  1: send pick pos
        Buffer1="OK";
        lenth=Buffer1.Length();
        NMUDP1->SendBuffer(Buffer1.c_str(),lenth, lenth);
        delete[] buff;
        buff=NULL;  //kevin 20180517
        bReadColorTrayID =true;                          //kevin 20210623 TRAY ID Read
        Buffer.sprintf("ASE_ColorTrayID: %s",TrayID[2][0]);
        RecordProcess(Buffer);
    }
    else
    {
        ReceveToHandleData(ReceviceData[iCount]);
    }
    iCount++;
    if(Memo1->Lines->Count>30) //kevin 20210927
        Memo1->Clear();
    Memo1->Lines->Add(ReadMessage);
    if(ReadMessage.Pos("ACK"))            //kevin 20210927
        RecordProcess("ACK");

    if(NMUDP1->RemotePort == Port)
    {
        if(ReadMessage.Pos("OK")||ReadMessage.Pos("FAIL"))
        {
            delete[] buff;  //kevin 20150119 add ChungHung 20131231 alter
            buff=NULL;  //kevin 20180517
            return;
        }
    }

    NMUDP1->RemoteHost=FromIP;
    NMUDP1->RemotePort=Port;
    AnsiString Data="OK";
    lenth=Data.Length();
    NMUDP1->SendBuffer(Data.c_str(),lenth, lenth); //kevin 20150703

    ReadMessage="FromIP:" + FromIP + ":" + "Port:" + Port + ":" +"Bytes:" + NumberBytes;
    Memo1->Lines->Add(ReadMessage);
    //delete buff;  //語法錯誤會照成亂刪記憶體
    delete[] buff;  //ChungHung 20131231 alter
    buff=NULL;  //kevin 20180517
}
//---------------------------------------------------------------------------
void __fastcall TASESendMessage::btnTestRecvClick(TObject *Sender)
{
    ReceveToHandleData(edtRecvMsg->Text);
}
//---------------------------------------------------------------------------
bool TASESendMessage::CompileWord(AnsiString sData,AnsiString iObject)
{
    AnsiString sBuffer="", sBuffer1="", sBuffer2=iObject;
    int iLegh=0, iEnd=0;
    if(iObject=="E" && iTestBinCount==255)   //kevin 20210825 add 255 bin ,
        sData+=",";

    sBuffer = sData;
    iLegh = sData.Length();
    for(int i=0; i<iLegh; i++)
    {
        if(sBuffer.Pos(","))
        {
            iEnd = sBuffer.Pos(",");
            sBuffer1 = sBuffer.SubString(1,iEnd-1);
            if(sBuffer1.Trim() == sBuffer2.Trim())
                return true;

            sBuffer1 = sBuffer.SubString(iEnd+1,iLegh);
            sBuffer  = sBuffer1;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool TASESendMessage::WriteATCDataToASE() //kevin 20200414 ATC data
{
    AnsiString sBuffer="";
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    AnsiString sGroup;
    bool bDut=false;
    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
    sGroup="Configuration";
    AnsiString sP="", sI="", sD="", sbuffer="";
    int K=0;
    sBuffer = ATC_Data.SWVer;
    WriteIniData(szDir, "ATC", "SWVer", sBuffer);//kevin PID
    WriteIniData(szDir, "ATC", "FileName", Temperature.ATCFileName);               //kevin 20200805 add ATC fileName
    WriteIniData(szDir, "ATC", "ChillerSetTemp", ATC_Data.dChillerSetTemp);//  chiller
    for(int i=0; i<8; i++)
    {
        K=i+1;
        sP.sprintf("Site%dP",K);
        WriteIniData(szDir, "ATC",sP,ATC_Data.iPID[i][0]);//kevin PID            P
        sI.sprintf("Site%dI",K);
        WriteIniData(szDir, "ATC",sI,ATC_Data.iPID[i][1]);//kevin PID            I
        sD.sprintf("Site%dD",K);
        WriteIniData(szDir, "ATC",sD,ATC_Data.iPID[i][2]);//kevin PID            D
        sbuffer.sprintf("Offset%d",K);
        WriteIniData(szDir, "ATC",sbuffer,ATC_Data.iOffset[i]);//kevin offset
        sbuffer.sprintf("ATC_READ_WATER%d",K);
        WriteIniData(szDir, "ATC",sbuffer,ATC_Data.ATC_READ_WATER_Value[i]);//kevin   水閥開度
        /*   kevin 20221214 mark
        if(ATC_Data.ATC_READ_WATER_Value[i] !=9999)     //kevin 20201202
        {
            sbuffer.sprintf("ATC_READ_WATER%d",K);
            WriteIniData(szDir, "ATC",sbuffer,ATC_Data.ATC_READ_WATER_Value[i]);//kevin   水閥開度
        }*/
        sbuffer.sprintf("ATC_LimitP%d",K);
        WriteIniData(szDir, "ATC",sbuffer,ATC_Data.ATC_GET_GrandBand[i*2]);//kevin     GrandBand  Range
        sbuffer.sprintf("ATC_LimitN%d",K);
        WriteIniData(szDir, "ATC",sbuffer,ATC_Data.ATC_GET_GrandBand[i*2+1]);//kevin   GrandBand  Range
    }
    WriteIniData(szDir, "ATC","ATC_READ_FUNCTION_STATUS",ATC_Data.ATC_READ_FUNCTION_STATUS);//kevin PID
    if(Temperature.iIndexHeatMode==SocketChamber ||Temperature.iIndexHeatMode==  HeadSocket||Temperature.iIndexHeatMode==HeadChamberSocket)
         bDut=true;

    WriteIniData(szDir, "ATC","Dut",bDut);//kevin DUT
    return true;
}
//==============================================================================
// kevin 20220601
//==============================================================================
bool TASESendMessage::SendFormData(AnsiString Data)
{
    Data.sprintf("@e02027%s",Data);
    RespondASECom(Data);//kevin 20220602 回應 主畫面資訊
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TASESendMessage::IPPort2Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//---------------------------------------------------------------------------

