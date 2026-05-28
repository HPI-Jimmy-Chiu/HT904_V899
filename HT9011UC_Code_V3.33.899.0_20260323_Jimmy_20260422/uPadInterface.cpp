#include "MachineDefine.h"
#pragma hdrstop

#include "uPadInterface.h"
#include "mymessbox.h"
#include "common.h"                                                             //Ken 20210702 AddPadInterface
#include "cprod.h"                                                              //Ken 20210702 AddPadInterface
#include "cpublic.h"                                                            //Ken 20210702 AddPadInterface
#include "cmydef.h"
//#include "HTimer.h"
#include "BtnPanelLane.h"
#include "MyLedLane.h"
#include "myTimer.h"
#include "database.h"
#include "rs232.h"
#include "TrayStepMotor.h"                                                      //KenHsieh 20211222 : Pad與步進馬達為同一Comport
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SPComm"
#pragma link "ALed"
#pragma link "butPa1"
#pragma link "MyLed"
#pragma resource "*.dfm"

TfPadInterface *fPadInterface;
TPadRS232Thread  *MyPad232Thread;
TQPF_Timer tSendDataDelay;
int iReceiceTask=1;
int iSendTask=1;
//---------------------------------------------------------------------------
__fastcall TPadRS232Thread::TPadRS232Thread(bool CreateSuspended)
    : TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall TPadRS232Thread::RS232ThreadProcess(void)
{
    if(iControlPanelMode || LoaderUnload_StepMotor)                             //KenHsieh 20211222 : Pad與步進馬達為同一Comport
        fPadInterface->Main232();
}
//---------------------------------------------------------------------------
void __fastcall TPadRS232Thread::Execute()                                      //Rs-232執行緒
{
    do
    {
        Synchronize(RS232ThreadProcess);                                        //用這個會影響主程式的繪圖效能
        SleepEx(1,true);
    } while (!Terminated);
}
//---------------------------------------------------------------------------
void PAD_PTR::SetItem(TMyLed *_mlEvent, TBtnPanelLane *_btnEvent, AnsiString _PadName, int _iData, AnsiString _InputName)
{
    mlEvent =_mlEvent;
    btnEvent=_btnEvent;
    PadName =_PadName;
    iData   =_iData;
    InputName=_InputName;
    btnEvent->Enabled=true;
}
//==============================================================================
//                             Search Change Page
//==============================================================================
void TfPadInterface::SearchChangePageButton(TWinControl *PCtrl)
{
    static int iIndex=0;

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            SearchChangePageButton((TWinControl *)P);                           // 找該物件附屬的物件(遞迴)
        }

        TBtnPanelLane *BtnPanelLane=dynamic_cast <TBtnPanelLane *>(P);
        if(BtnPanelLane!=NULL)
        {
            iIndex++;
            BtnPanelLane->OnClick    = PadButtonClick;
        }
    }
}
//==============================================================================
//                             畫面控制按鈕
//==============================================================================
void __fastcall TfPadInterface::PadButtonClick(TObject *Sender)
{
    TBtnPanelLane *BtnPanelLane=dynamic_cast <TBtnPanelLane *>(Sender);
    AnsiString sData, sData1;                                                   //KenHsieh 20220308 : 新增為三個面板
    int iStatue=0x000000;

    SendData.clear();

    SendData.push_back('t');
    SendData.push_back('0');
    SendData.push_back('5');
    if(BtnPanelLane!=Null && BtnPanelLane->Tag==1)
    {
        SendData.push_back(IntToHex(PAD_RearControl, 1));
    }
    else
    {
        SendData.push_back(IntToHex(PAD_FrontControl, 1));
    }
    SendData.push_back(IntToHex(PAD_ControlDLC, 1));
    SendData.push_back('9');

    if(cb_PadInterface_PadLedBling->Checked)
    {
        SendData.push_back(IntToHex(PAD_LedBling, 1));
    }
    else
    {
        SendData.push_back(IntToHex(PAD_LedLight, 1));
    }

    for(int i=0; i<CheckPadItem; i++)
    {
        if(PadItem[i].btnEvent->Down)
        {
            if(BtnPanelLane->Tag==1 &&                                          //KenHsieh 20211221 : 新增前後面板判斷，避免前後同顆按鈕操作時顯示錯誤
              (PadItem[i].PadName.Pos("RK")>0 ||
               PadItem[i].PadName.Pos("SwRKSafeLock")>0 ||                      //KenHsieh 20211228 : 區分實體IO與通訊面板
               PadItem[i].PadName.Pos("SwRearActiveLed")>0))
            {
                iStatue|=PadItem[i].iData;
            }
            else if(BtnPanelLane->Tag==0 &&
                   (PadItem[i].PadName.Pos("FK")>0 ||
                    PadItem[i].PadName.Pos("SwFrontActiveLed")>0))
            {
                iStatue|=PadItem[i].iData;
            }
        }
    }
    SendData.push_back(IntToHex(iStatue, 6));
    for(int i=0; i<(int)SendData.size(); i++)
    {
        sData+=(AnsiString)SendData[i];
    }

    if(BtnPanelLane->Tag==1)                                                    //KenHsieh 20220308 : 新增為三個面板
    {
        sData1=sData;
        sData1=StringReplace(sData1, "t051", "t052", TReplaceFlags()<<rfReplaceAll);
        SendCommand(sData1);
        MySleep(5);
    }
    SendCommand(sData);
}
//---------------------------------------------------------------------------
__fastcall TfPadInterface::TfPadInterface(TComponent* Owner)
    : TForm(Owner)
{
    InitialVariable();
    SearchChangePageButton(sb_PadInterface_MainPage);
}
//---------------------------------------------------------------------------
void __fastcall TfPadInterface::FormShow(TObject *Sender)
{
    Left=(1280-Width)/2;
    Top =(1024-Height)/2;

    pc_PadInterface->ActivePageIndex=0;
    SendCommand("t050490000000");                                               //Front Pad 初始化
    SendCommand("t050491000000");                                               //Front Pad 初始化
    SendCommand("t051490000000");                                               //Rear Pad 初始化
    SendCommand("t051491000000");                                               //Rear Pad 初始化
    SendCommand("t052490000000");                                               //Rear Pad 初始化    //KenHsieh 20220308 : 新增為三個面板
    SendCommand("t052491000000");                                               //Rear Pad 初始化    //KenHsieh 20220308 : 新增為三個面板

    for(int i=0; i<CheckPadItem; i++)
    {
        PadItem[i].btnEvent->Down=false;
    }
    bShow=true;
}
//---------------------------------------------------------------------------
void __fastcall TfPadInterface::FormDestroy(TObject *Sender)
{
    MyPageChangeList->Clear();
    delete MyPageChangeList;
    delete CommReceiveList;
    delete CommReceiveLength;
}
//==============================================================================
//                                  初始化變數
//==============================================================================
void TfPadInterface::InitialVariable()
{
    aParaPath   = "D:\\HT9045\\System\\PadInterfacePara.ini";

    MyPageChangeList=new TList;
    MyPageChangeList->Clear();
    bShow      =false;
    bRequestVer=false;
    bScanSwitch=true;                                                           //KenHsieh 20211224 : 新增按鈕狀態掃描

    bSendSwitchStatusing=false;

    CommReceiveList         = new TStringList();
    CommReceiveLength       = new TStringList();
    CommReceiveList->Clear();
    CommReceiveLength->Clear();
    CommSendList            = new TStringList();
    CommSendLength          = new TStringList();
    CommSendList->Clear();
    CommSendLength->Clear();
    bSafeLockStatus=false;

    PadItem[ 0].SetItem(ml_PadInterface_FrontPowerOff   , sb_PadInterface_FrontPowerOff     , "SwFKPowerOff"        , PAD_PowerOff      ,"SnFKPowerOff");
    PadItem[ 1].SetItem(ml_PadInterface_FrontPowerOn    , sb_PadInterface_FrontPowerOn      , "SwFKPowerOn"         , PAD_PowerOn       ,"SnFKPowerOn");
    PadItem[ 2].SetItem(ml_PadInterface_Front           , sb_PadInterface_Front             , "SwFrontActiveLed"    , PAD_PannelEnable  ,"SnFrontPadActive");    //KenHsieh 20211221 : Front新增Enable燈號
    PadItem[ 3].SetItem(ml_PadInterface_FrontReset      , sb_PadInterface_FrontReset        , "SwFKReset"           , PAD_Reset         ,"SnFKReset");
    PadItem[ 4].SetItem(ml_PadInterface_FrontPause      , sb_PadInterface_FrontPause        , "SwFKPause"           , PAD_Pause         ,"SnFKPause");
    PadItem[ 5].SetItem(ml_PadInterface_FrontHome       , sb_PadInterface_FrontHome         , "SwFKHome"            , PAD_Home          ,"SnFKHome");
    PadItem[ 6].SetItem(ml_PadInterface_FrontStart      , sb_PadInterface_FrontStart        , "SwFKStart"           , PAD_Start         ,"SnFKStart");
    PadItem[ 7].SetItem(ml_PadInterface_FrontOneCycle   , sb_PadInterface_FrontOneCycle     , "SwFKOneCycle"        , PAD_OneCycle      ,"SnFKOneCycle");
    PadItem[ 8].SetItem(ml_PadInterface_FrontRetry      , sb_PadInterface_FrontRetry        , "SwFKRetry"           , PAD_Retry         ,"SnFKRetry");
    PadItem[ 9].SetItem(ml_PadInterface_FrontSkip       , sb_PadInterface_FrontSkip         , "SwFKSkip"            , PAD_Skip          ,"SnFKSkip");
    PadItem[10].SetItem(ml_PadInterface_FrontCleanOut   , sb_PadInterface_FrontCleanOut     , "SwFKCleanOut"        , PAD_CleanOut      ,"SnFKCleanOut");
    PadItem[11].SetItem(ml_PadInterface_FrontTrayFeed   , sb_PadInterface_FrontTrayFeed     , "SwFKTrayFeed"        , PAD_TrayFeed      ,"SnFKTrayFeed");
    PadItem[12].SetItem(ml_PadInterface_FrontTrayEnd    , sb_PadInterface_FrontTrayEnd      , "SwFKTrayEnd"         , PAD_TrayEnd       ,"SnFKTrayEnd");
    PadItem[13].SetItem(ml_PadInterface_FrontAlarmReset , sb_PadInterface_FrontAlarmReset   , "SwFKAlarmReset"      , PAD_AlarmReset    ,"SnFKAlarmReset");
    PadItem[14].SetItem(ml_PadInterface_RearPowerOff    , sb_PadInterface_RearPowerOff      , "SwRKPowerOff"        , PAD_PowerOff      ,"SnRKPowerOff");
    PadItem[15].SetItem(ml_PadInterface_RearPowerOn     , sb_PadInterface_RearPowerOn       , "SwRKPowerOn"         , PAD_PowerOn       ,"SnRKPowerOn");
    PadItem[16].SetItem(ml_PadInterface_RearReset       , sb_PadInterface_RearReset         , "SwRKReset"           , PAD_Reset         ,"SnRKReset");
    PadItem[17].SetItem(ml_PadInterface_RearPause       , sb_PadInterface_RearPause         , "SwRKPause"           , PAD_Pause         ,"SnRKPause");
    PadItem[18].SetItem(ml_PadInterface_RearHome        , sb_PadInterface_RearHome          , "SwRKHome"            , PAD_Home          ,"SnRKHome");
    PadItem[19].SetItem(ml_PadInterface_RearStart       , sb_PadInterface_RearStart         , "SwRKStart"           , PAD_Start         ,"SnRKStart");
    PadItem[20].SetItem(ml_PadInterface_RearOneCycle    , sb_PadInterface_RearOneCycle      , "SwRKOneCycle"        , PAD_OneCycle      ,"SnRKOneCycle");
    PadItem[21].SetItem(ml_PadInterface_RearRetry       , sb_PadInterface_RearRetry         , "SwRKRetry"           , PAD_Retry         ,"SnRKRetry");
    PadItem[22].SetItem(ml_PadInterface_RearSkip        , sb_PadInterface_RearSkip          , "SwRKSkip"            , PAD_Skip          ,"SnRKSkip");
    PadItem[23].SetItem(ml_PadInterface_RearCleanOut    , sb_PadInterface_RearCleanOut      , "SwRKCleanOut"        , PAD_CleanOut      ,"SnRKCleanOut");
    PadItem[24].SetItem(ml_PadInterface_RearTrayFeed    , sb_PadInterface_RearTrayFeed      , "SwRKTrayFeed"        , PAD_TrayFeed      ,"SnRKTrayFeed");
    PadItem[25].SetItem(ml_PadInterface_RearTrayEnd     , sb_PadInterface_RearTrayEnd       , "SwRKTrayEnd"         , PAD_TrayEnd       ,"SnRKTrayEnd");
    PadItem[26].SetItem(ml_PadInterface_RearAlarmReset  , sb_PadInterface_RearAlarmReset    , "SwRKAlarmReset"      , PAD_AlarmReset    ,"SnRKAlarmReset");
    PadItem[27].SetItem(ml_PadInterface_RearSafeLock    , sb_PadInterface_RearSafeLock      , "SwRKSafeLock"        , PAD_SafeLock      ,"SnRKSafeLock");         //SnRKCoverOpen  SnSafeLock   //KenHsieh 20211228 : 區分實體IO與通訊面板
    PadItem[28].SetItem(ml_PadInterface_RearStep        , sb_PadInterface_RearStep          , "SwRKManualStep"      , PAD_Step          ,"SnRKManualStep");
    PadItem[29].SetItem(ml_PadInterface_RearTStart      , sb_PadInterface_RearTStart        , "SwRKManualTStart"    , PAD_TStart        ,"SnRKManualTStart");
    PadItem[30].SetItem(ml_PadInterface_Rear            , sb_PadInterface_Rear              , "SwRearActiveLed"     , PAD_PannelEnable  ,"SnRearPadActive");    //KenHsieh 20211221 : Front新增Enable燈號
    CheckPadItem=31;                                                                                                                                            //KenHsieh 20211221 : Front新增Enable燈號，30->31
}
//==============================================================================
//
//==============================================================================
bool TfPadInterface::IsPadButton(AnsiString aName)
{
    bool bFind=false;

    for(int i=0; i<CheckPadItem; i++)
    {
        if(AnsiString(PadItem[i].PadName)==aName)
        {
            bFind=true;
            break;
        }
    }
    return bFind;
}
//==============================================================================
//
//==============================================================================
bool TfPadInterface::IsPadKey(AnsiString aName)
{
    bool bFind=false;

    for(int i=0; i<CheckPadItem; i++)
    {
        if(AnsiString(PadItem[i].InputName)==aName)
        {
            bFind=true;
            break;
        }
    }
    return bFind;
}
//==============================================================================
//
//==============================================================================
bool TfPadInterface::OpenCommPort()
{
    if(bRs232Ok)
        return true;

    try
    {
        HANDLE handle = CreateFile(dmTrayMotor->comTrayStepMotor->CommName.c_str(),     //KenHsieh 20211222 : Pad與步進馬達為同一Comport
                                   GENERIC_READ | GENERIC_WRITE,
                                   0,
                                   NULL,
                                   OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                                   0);

        if(handle==INVALID_HANDLE_VALUE)
        {
            RecordCommunication("[Connect]", " INVALID_HANDLE_VALUE");
            return false;
        }

        CloseHandle(handle);
        dmTrayMotor->comTrayStepMotor->StartComm();                             //KenHsieh 20211222 : Pad與步進馬達為同一Comport
        RecordCommunication("[Connect]", " OK");
        bRs232Ok=true;
    }
    catch(...)
    {
        RecordCommunication("[Connect]", " FAIL");
        bRs232Ok=false;
        return false;
    }

    return true;
}
//==============================================================================
//
//==============================================================================
bool TfPadInterface::CloseCommPort()
{
    try
    {
        dmTrayMotor->comTrayStepMotor->StopComm();                              //KenHsieh 20211222 : Pad與步進馬達為同一Comport
        RecordCommunication("[Disconnect]", " OK");
        bRs232Ok=false;
    }
    catch(...)
    {
        RecordCommunication("[Disconnect]", " FAIL");
        return false;
    }

    return true;
}
//==============================================================================
//                              寫資料到檔案
//==============================================================================
void TfPadInterface::PadWriteDataToFile(AnsiString cFilePath, AnsiString cData)
{
    if(CheckFileExist(cFilePath)==false)
        return;

    FILE *pFile;
    pFile=fopen(cFilePath.c_str(), "a+");

    if(pFile!=NULL)
    {
        fputs(cData.c_str(), pFile);
        fputs("\n", pFile);
        fclose(pFile);
    }
}
//==============================================================================
//
//==============================================================================
void TfPadInterface::RecordCommunication(AnsiString aTitle, AnsiString Command)
{
    AnsiString sFileName="", sMegTime="", asLog;

    GetTimeInfo();
    sFileName.sprintf("%s\\%04d\\%02d\\%02d", asPadCommLogPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(sFileName);

    sFileName.sprintf("%s\\%04d\\%02d\\%02d\\%02d%02d.txt", asPadCommLogPath, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
    sMegTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d %03d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);
    asLog.sprintf("%s,   %s ==> %s", sMegTime, aTitle, Command);

    if(Memo_PadInterface->Lines->Count>=1000)
    {
        Memo_PadInterface->Lines->SaveToFile(sFileName);
        Memo_PadInterface->Lines->Clear();
    }
    Memo_PadInterface->Lines->Add(asLog);
    Memo_PadInterface->SelLength=Memo_PadInterface->Text.Length();              //顯示最後一行資料
    PadWriteDataToFile(sFileName, asLog);
}
//==============================================================================
void __fastcall TfPadInterface::ResetComm()
{
    CloseCommPort();
    dmTrayMotor->comTrayStepMotor->CommName="\\\\.\\"+HSys.TrayStepMotor_ComPort;   //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    OpenCommPort();
}
//==============================================================================
void __fastcall TfPadInterface::sb_PadInterface_ManualSendClick(
      TObject *Sender)
{
    SendCommand(ed_PadInterface_ManualSend->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfPadInterface::ClearLog1Click(TObject *Sender)
{
    Memo_PadInterface->Lines->Clear();
}
//==============================================================================
void TfPadInterface::RequestPadVersion()
{
    AnsiString sData;
    RequestData.clear();

    RequestData.push_back('t');
    RequestData.push_back('0');
    RequestData.push_back('5');
    RequestData.push_back('1');
    RequestData.push_back('1');
    RequestData.push_back('2');
    RequestData.push_back('0');

    for(int i=0; i<(int)RequestData.size(); i++)
    {
        sData+=(AnsiString)RequestData[i];
    }
    bRequestVer=false;
    SendCommand(sData);
}
//==============================================================================
void TfPadInterface::SendSwitchStatus(TBtnPanelLane *bpPtr)
{
    AnsiString sData;
    static int iStatue=0x000000;

    for(int i=0; i<CheckPadItem; i++)
    {
        if(AnsiString(PadItem[i].PadName)==bpPtr->Alias)
        {
            if(bpPtr->Down)
            {
                iStatue|=PadItem[i].iData;
                bPadStatus[i]=true;
            }
            else
            {
                if((iStatue&PadItem[i].iData)==PadItem[i].iData)
                    iStatue^=PadItem[i].iData;
                bPadStatus[i]=false;
            }
        }
    }

    SendData.clear();
    SendData.push_back('t');
    SendData.push_back('0');
    SendData.push_back('5');
    if(bpPtr->Alias.Pos("RK")>0 || bpPtr->Alias.Pos("SwRearActiveLed")>0)       //Rear Pad  //KenHsieh 20211221 : Front新增Enable燈號
    {
        SendData.push_back(IntToHex(PAD_RearControl, 1));
    }
    else
    {
        SendData.push_back(IntToHex(PAD_FrontControl, 1));
    }
    SendData.push_back(IntToHex(PAD_ControlDLC, 1));
    SendData.push_back('9');
    if(cb_PadInterface_PadLedBling->Checked)
    {
        SendData.push_back(IntToHex(PAD_LedBling, 1));
    }
    else
    {
        SendData.push_back(IntToHex(PAD_LedLight, 1));
    }

    SendData.push_back(IntToHex(iStatue, 6));
    for(int i=0; i<(int)SendData.size(); i++)
    {
        sData+=(AnsiString)SendData[i];
    }
    SendCommand(sData);
    if(sData.Pos("t051")>0)
    {
        AnsiString sData1=StringReplace(sData, "t051", "t052", TReplaceFlags()<<rfReplaceAll);
        SendCommand(sData1);                                                    //Jimmychiu 20260304 : Fixed for 3rd Pad enable pannel error
    }
}
//==============================================================================
void TfPadInterface::SendSwitchStatus(AnsiString aName, bool Type)
{
    AnsiString sData, S;

    if(bShow)
        return;

    for(int i=0; i<CheckPadItem; i++)
    {
        if(AnsiString(PadItem[i].PadName)==aName)//  && bPadStatus[i] != Type )
        {
            bPadStatus[i]=Type;
        }
    }
}
//==============================================================================
void TfPadInterface::SendCommand(AnsiString sData)
{
    if(bRs232Ok==false)
    {
        RecordCommunication("[Send]", sData);
        RecordCommunication("[Connect Error]", " Send Fail");
        return;
    }

    int iSize=sData.Length()+1;
    if(iSize<=0)
    {
        return;
    }

    sData=sData+AnsiString('\r');
    dmTrayMotor->comTrayStepMotor->WriteCommData(sData.c_str(), iSize);         //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    RecordCommunication("[Send]", sData);
}
//==============================================================================
void TfPadInterface::SendCommand(vector<Byte>& Data)
{
    AnsiString asCommand;

    if(bRs232Ok==false)
    {
        RecordCommunication("[Connect Error]", " Send Fail");
        return;
    }

    int iSize=Data.size();
    if(iSize<=0)
        return;

    char Buff[256];
    for(int i=0; i<iSize; i++)
    {
        Buff[i]=Data[i];
    }

    Buff[iSize]='\r';

    dmTrayMotor->comTrayStepMotor->WriteCommData(Buff, strlen(Buff));           //KenHsieh 20211222 : Pad與步進馬達為同一Comport

    for(int i=0; i<iSize; i++)
    {
        asCommand+=(char)Buff[i];
    }

    RecordCommunication("[Send]", asCommand);
    Data.clear();
}
//==============================================================================
void __fastcall TfPadInterface::DoScanPanelLed(int iAddress, int iKey)
{
    int iOldAddress=iAddress;
    if(iAddress>0)                                                              //KenHsieh 20220308 : 新增為三個面板
         iAddress=1;

    if(bShow==false)                                                            //KenHsieh 20220401 : Pad頁面顯示不判斷
    {                                                                           //Sam 20220310 : 修正 SafeLock & PAD_PannelEnable  問題
        if(iOldAddress==1)                                                      //Jimmychiu 20260304 : Fixed for 3rd Pad enable pannel error
        {
            if(iKey==PAD_PannelEnable)                                          //KenHsieh 20220401 : Enable優先判斷
            {
                PadItem[epn_SnRearPadActive].mlEvent->Value=true;
                SendSwitchStatus("SwRearActiveLed", true);
            }
            else if(iKey==0)
            {
                PadItem[epn_SnRearPadActive].mlEvent->Value=false;
                SendSwitchStatus("SwRearActiveLed",false);
            }
        }

        if(PadItem[epn_SnRKSafeLock].mlEvent->Value==false)                     //後面板 SafeLock 狀態
        {
            if(PAD_SafeLock&iKey || iKey==4 || iKey==0)                         //iKey=4 is unlock
            {
                //解除 SafeLock
            }
            else
            {
                return;
            }
        }
        else if(PadItem[epn_SnRearPadActive].mlEvent->Value==false)             //後面板 PannelEnable 狀態
        {
            if(iAddress>0)                                                      //後面板所有按鍵都失效
            {
                if((PAD_PannelEnable&iKey) || (PAD_SafeLock&iKey))              //除了 PAD_PannelEnable & PAD_SafeLock
                {

                }
                else if(iKey&PAD_Pause)                                         //除了 PAD_Psuse    //KenHsieh 20220401 : 新增Pause
                {
                    PadItem[17].mlEvent->Value=true;
                    return;
                }
                else if(iKey&PAD_AlarmReset)                                    //除了 PAD_AlarmReset
                {
                    PadItem[epn_SnRKAlarmReset].mlEvent->Value=true;
                    return;
                }
                else
                {
                    PadItem[epn_SnRKPause].mlEvent->Value=false;                //kevin 20220624 add clean pause
                    PadItem[epn_SnRKAlarmReset].mlEvent->Value=false;           //kevin 20220624 add clean AlarmReset
                    return;
                }
            }
        }
        else
        {
            if(iAddress==0)                                                     //前面板所有按鍵都失效
            {
                if(iKey&PAD_Pause)                                              //除了 PAD_Psuse //KenHsieh 20220401 : 新增Pause
                {
                    PadItem[epn_SnFKPause].mlEvent->Value=true;
                    return;
                }
                else if(iKey&PAD_AlarmReset)                                    //除了 PAD_AlarmReset
                {
                    PadItem[epn_SnFKAlarmReset].mlEvent->Value=true;
                    return;
                }
                else
                {
                    PadItem[epn_SnFKPause].mlEvent->Value=false;                //kevin 20220624 add clean pause
                    PadItem[epn_SnFKAlarmReset].mlEvent->Value=false;           //kevin 20220624 add clean AlarmReset
                    return;
                }
            }
        }
    }

    for(int i=0; i<CheckPadItem; i++)
    {
        if(PadItem[i].iData&iKey)
        {
            if(iOldAddress==2 &&
               PadItem[i].iData&PAD_PannelEnable)                               //Jimmychiu 20260304 : Fixed for 3rd Pad enable pannel error
            {
                //pass
            }
            else if(PadItem[i].iData==PAD_SafeLock)
            {
                PadItem[i].mlEvent->Value=false;
                SendSwitchStatus("SwRKSafeLock", true);                         //KenHsieh 20211228 : 區分實體IO與通訊面板
            }
            else if(PadItem[i].mlEvent->Tag==iAddress)
            {
                PadItem[i].mlEvent->Value=true;
            }
            else if(iOldAddress==2 &&
                   PadItem[i].mlEvent->Tag==iOldAddress)
            {
                  PadItem[i].mlEvent->Value=true;
            }

        }
        else
        {
            if((iKey&PAD_SafeLock)==false &&
               PadItem[i].InputName=="SnRKSafeLock")                            //KenHsieh 20211221 : 因SafeLock按鈕從復歸型改為保持型，故修改判斷方式
            {
                PadItem[i].mlEvent->Value=true;
                SendSwitchStatus("SwRKSafeLock", false);                        //KenHsieh 20211228 : 區分實體IO與通訊面板
            }
            else
            {
                PadItem[i].mlEvent->Value=false;
            }
        }
    }
}
//==============================================================================
void __fastcall TfPadInterface::DoUpdataPadStatus(int iAddress, int iKey)
{
    int iTag=(iAddress>0)?1:0;
    for(int i=0; i<CheckPadItem; i++)
    {
        if(PadItem[i].mlEvent->Tag==iTag)                                       //KenHsieh 20211221 : 新增前後面板判斷，避免前後同顆按鈕操作時顯示錯誤
        {
            if(PadItem[i].iData&iKey)
            {
                if(PadItem[i].PadName=="SwFrontActiveLed" ||
                   PadItem[i].PadName=="SwRKSafeLock"     ||                    //KenHsieh 20211228 : 區分實體IO與通訊面板
                   PadItem[i].PadName=="SwRKManualStep"   ||
                   PadItem[i].PadName=="SwRKManualTStart")
                {
                    bPadStatus[i]=true;
                }
                else if(PadItem[i].mlEvent->Tag==iAddress)
                {
                    bPadStatus[i]=true;
                }
            }
            else
            {
                bPadStatus[i]=false;
            }
        }
    }
}
//==============================================================================
void __fastcall TfPadInterface::ProcessReceiceData()
{
    int &iTask=iReceiceTask;
    int iAddress, iPadKey;
    AnsiString S, sAddress, aPadKey;

    if(CommReceiveLength->Count==0)
    {
        CommReceiveList->Clear();
        return;
    }

    switch(iTask)
    {
        case 1:
            aReciveData=CommReceiveList->Strings[0];
            CommReceiveList->Delete(0);
            CommReceiveLength->Delete(0);
            iTask=10;
        case 10:
            do
            {
                S=aReciveData.SubString(6, 2);
                if(S.Pos("00") && aReciveData.Length()>=14)                     //檢查是否是INPUT訊號
                {
                    sAddress =aReciveData.SubString(4, 1);
                    iAddress =atoi(sAddress.c_str());                           //檢查判斷前後面板

                    aPadKey  =aReciveData.SubString(8, 6);                      //檢查按鈕
                    iPadKey  =StrToInt("0x"+aPadKey);

                    DoScanPanelLed(iAddress,iPadKey);
                }
                else if(S.Pos("20") && aReciveData.Length()>=8)
                {
                    bRequestVer = true;
                }
                else if(S.Pos("90") && aReciveData.Length()>=14)                //檢查是否是INPUT訊號
                {
                    bSendSwitchStatusing=false;
                    sAddress =aReciveData.SubString(4, 1);
                    iAddress =atoi(sAddress.c_str());                           //檢查判斷前後面板

                    aPadKey  =aReciveData.SubString(8, 6);                      //檢查按鈕
                    iPadKey  =StrToInt("0x"+aPadKey);
                    DoUpdataPadStatus(iAddress,iPadKey);
                }
                aReciveData = aReciveData.Delete(1, aReciveData.Pos("\r"));
                aReciveData.Trim();
            }while(aReciveData.Pos("\r")>0);
            iTask=1;
            break;
    }
}
//==============================================================================
bool __fastcall TfPadInterface::ProcessSendDataNew()                            //丟通訊資料改為固定時間前後面板的 bPadStatus 狀態，若有改變在一次丟整個面板 SW 狀態
{
    static int iTag=0;                                                          //循環切換丟前後面板
    static bool bOldPadStatus[32]={false};
    static bool bRun3rdPannel=false;
    static AnsiString sData1="";
    int iStatue=0x000000;
    bool bNeedSendData=false;                                                   //狀態有改變才需要丟資料
    AnsiString sData="";                                                        //KenHsieh 20220308 : 新增為三個面板

    if(tSendDataDelay.Off())                                                    //JerryYang 20230309 : 不要太密集的送
    {
        if(bRun3rdPannel==true)                                                 //Jimmychiu 20260304 : Fixed for 3rd Pad enable pannel error
        {
            bRun3rdPannel=false;
            SendCommand(sData1);
            sData1="";
            tSendDataDelay.SetMSAndOn(50);
            return true;
        }
        bNeedSendData=false;
        for(int i=0; i<CheckPadItem; i++)
        {
            if(PadItem[i].btnEvent->Tag==iTag)
            {
                if(bOldPadStatus[i]!=bPadStatus[i])                             //檢查狀態是否有改變
                {
                    bOldPadStatus[i]=bPadStatus[i];
                    bNeedSendData=true;
                }

                if(bPadStatus[i])
                {
                    iStatue|=PadItem[i].iData;
                }
            }
        }

        if(bNeedSendData==false)
        {
            if(iTag==0)
                iTag=1;
            else
                iTag=0;
        }
        else
        {
            SendData.clear();

            SendData.push_back('t');
            SendData.push_back('0');
            SendData.push_back('5');
            if(iTag==0)
            {
                SendData.push_back(IntToHex(PAD_FrontControl, 1));
            }
            else
            {
                SendData.push_back(IntToHex(PAD_RearControl, 1));
            }
            SendData.push_back(IntToHex(PAD_ControlDLC, 1));
            SendData.push_back('9');
            if(cb_PadInterface_PadLedBling->Checked)
            {
                SendData.push_back(IntToHex(PAD_LedBling, 1));
            }
            else
            {
                SendData.push_back(IntToHex(PAD_LedLight, 1));
            }

            SendData.push_back(IntToHex(iStatue, 6));
            for(int i=0; i<(int)SendData.size(); i++)
            {
                sData+=(AnsiString)SendData[i];
            }
            SendCommand(sData);

            if(bShow)                                                           //KenHsieh 20220402 : 頁面顯示時送出之時間不同
                tSendDataDelay.SetMSAndOn(550);                                 //先試試看 300ms
            else
                tSendDataDelay.SetMSAndOn(50);
            if(sData.Pos("t051")>0)
            {
                sData1=StringReplace(sData, "t051", "t052", TReplaceFlags()<<rfReplaceAll);
                bRun3rdPannel=true;                                                    //Jimmychiu 20260304 : Fixed for 3rd Pad enable pannel error
            }

            if(iTag==0)
                iTag=1;
            else
                iTag=0;
            return true;
        }
    }
    return false;
}
//==============================================================================
bool __fastcall TfPadInterface::ProcessScanKey(AnsiString aSenName)
{
    for(int i=0; i<CheckPadItem; i++)
    {
        if(AnsiString(PadItem[i].InputName).Pos(aSenName)>0)
            return PadItem[i].mlEvent->Value;
    }
    return false;
}
//==============================================================================
//                                  主執行緒
//==============================================================================
void __fastcall TfPadInterface::Main232()
{
    if(InitialOK==false)
        return;

    if(iControlPanelMode==0)
        return;

    static int Task=1;
    static TQPF_Timer hTimeOut, hDelay;
    static TQPF_Timer StartTime;

    bool bHasSendPad=false;
    int iInterval;

    ProcessReceiceData();

    bHasSendPad=fPadInterface->ProcessSendDataNew();

    if(bHasSendPad)                                                             //JerryYang 20230309 : 不要太密集的送
    {
        return;
    }

    if(bRs232Ok && bScanSwitch)                                                 //KenHsieh 20211224 : 新增按鈕狀態掃描
    {
        SendCommand("t051400000000");
        bScanSwitch=false;
        bHasSendPad=true;
    }

    if(bHasSendPad)                                                             //JerryYang 20230309 : 不要太密集的送
    {
        bHasSendPad=false;
        return;
    }
    dmTrayMotor->DoTrayStepMotor();                                             //KenHsieh 20211222 : Pad與步進馬達為同一Comport

    switch(Task)
    {
        case 1:
            StartTime.LatchCycleTimeSec(true);
            Task=10;
//            break;
        case 10:
            iInterval=StartTime.LatchCycleTimeSec();                            //sec

            if(iInterval>=10)
            {
                RequestPadVersion();
                hTimeOut.SetSecAndOn(1);
                Task=20;
            }
            break;
        case 20:
            if(bRs232Ok && bRequestVer)
            {
                Task=1;
            }
            else if(bRs232Ok==false)
            {
                hDelay.SetSecAndOn(0.1);
                ResetComm();
                Task=50;
            }
            else if(hTimeOut.Off())
            {
                Task=1;
            }
            break;
        case 50:
            if(hDelay.Off())
            {
                Task=1;
            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPadInterface::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfPadInterface::sb_PadInterface_ExitClick(TObject *Sender)
{
    sb_PadInterface_Exit->Down=false;
    bShow=false;
    for(int i=0; i<32; i++)
        bPadStatus[i]=false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfPadInterface::sb_PadInterface_FrontPowerOffMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    TBtnPanelLane *BtnPanelLane=dynamic_cast <TBtnPanelLane *>(Sender);
    BtnPanelLane->Down=!BtnPanelLane->Down;
}
//---------------------------------------------------------------------------
