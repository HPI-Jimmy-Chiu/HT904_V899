#include "MachineDefine.h"
#pragma hdrstop
#include "myQwertyKeyBoard.h"
#include "fDTME08.h"
#include "cmydef.h"
#include "myswitch.h"
#include "ATC_Handler_Side.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma link "MyLed"
#pragma link "ALed"
#pragma resource "*.dfm"
TfrmDTME08 *frmDTME08;
//---------------------------------------------------------------------------
__fastcall TfrmDTME08::TfrmDTME08(TComponent* Owner)
    : TForm(Owner)
{
    if(USE_16_HEATER==eht16HeaterDTME08  ||                                     //Steven 20221201 : DTM08沒用到不要進來
       USE_16_HEATER==eht32HeaterDTME08)
    {
        NewuDTME08Control();
        Reload();
        InitialData();
        AddTempGUI();
        InitialComboList();
    }
}
//---------------------------------------------------------------------------
void TfrmDTME08::NewuDTME08Control()
{
    if(dtme08==NULL)
        dtme08=new uDTME08Control();
    dtme08->RecordErrorMsg=RecordErrorMsg;
    dtme08->RecordMsg=RecordMsg;
}
//---------------------------------------------------------------------------
void TfrmDTME08::AddTempGUI()
{
    for(int i=0; i<dtme08->GetMaxStationNumber()*dtme08->GetChannelNumberPerStation(); i++)
    {
        AddSingleView(ScrollBox1);
    }
}
//---------------------------------------------------------------------------
void TfrmDTME08::DrawTempGUI()
{
    for(int i=0; i<GetSize(); i++)
    {
        GetPalGroup(i)->SetVisible((i<GetChannelNumber()));
    }
}
//---------------------------------------------------------------------------
void TfrmDTME08::InitialData()
{
    iDoInitialCommandStatus=DoInitialNone;
    brecordmsgLock=bTimerUpdateLock=false;
    TargetCh=TargetStation=TargetNo=iSingleStationTask=iCycleRunNumber=iNowActiveStation=0;
    TimerUpdate->Enabled=false;

    if(USE_16_HEATER==eht16HeaterDTME08)                                        //Steven 20241113 : add for DTM 32CH
        SetChannelNumber(16);
    else if(USE_16_HEATER==eht32HeaterDTME08)
        SetChannelNumber(32);

    bShow=false;
    ClearAllCommandStatus();
    bCycleFirstRun=true;
    iTaskDTME08Cycle=1;
    iDoManualCommandStatus=DoNone;
    bCtrlSwitch=true;
}
//---------------------------------------------------------------------------
void TfrmDTME08::SaveData()
{
    dtme08->SetSocketAddress(edAddress->Text);
    dtme08->SetSocketPort(edPort->Text);
    dtme08->WriteSettingFile();
}
//---------------------------------------------------------------------------
void TfrmDTME08::Reload()
{
    dtme08->ReadSettingFile();
    edAddress->Text=dtme08->GetSocketAddress();
    edPort->Text=dtme08->GetSocketPort();
}
//---------------------------------------------------------------------------
void TfrmDTME08::ShowMSG(AnsiString asStr)
{
    if(brecordmsgLock==false)
    {
        brecordmsgLock=true;
        if(meLogs->Lines->Count>50)
            meLogs->Lines->Clear();
        meLogs->Lines->Add(asStr);
        brecordmsgLock=false;
    }
}
//---------------------------------------------------------------------------
void TfrmDTME08::RecordMsg(AnsiString asMsg)
{
    asMsg=FormatDateTime("yyyy-mm-dd HH:MM:ss ", Now())+asMsg;
    ShowMSG(asMsg);
}
//---------------------------------------------------------------------------
void TfrmDTME08::RecordErrorMsg(AnsiString asMsg)
{
    asErrorDTME08=asMsg;
    RecordMsg("[Error]"+asMsg);
}
//---------------------------------------------------------------------------
void TfrmDTME08::AddSingleView(TScrollBox *ScrollBox1)
{
    TMyOmronPanel* panel      = new TMyOmronPanel(this);
    int numOfCol=ScrollBox1->Width/210;
    panel->GroupBox->Parent   = ScrollBox1;
    panel->GroupBox->Caption  = "CH"+IntToStr(GetSize()+1);
    panel->GroupBox->Visible  = true;
    panel->iMyTag             = GetSize();
    panel->edSV->Text         = AnsiString(30);
    panel->GroupBox->Left     = 210*(GetSize()%numOfCol);
    panel->GroupBox->Top      = 124*(ChangeToFloatNonPcnt((double)(GetSize()), (double)(numOfCol)));
    panel->SetSP("          ", TColor(0x004080FF));
    panel->SetEvent("Event", clBlack);                                          //警報事件
    panel->SetPV("          ", clYellow);
    panel->SetRunStop("STOP", clBlack);
    panel->SetDegree("℃", clYellow);                                           //顯示量測單位
    AddTempPanel(panel);
}
//---------------------------------------------------------------------------
void TfrmDTME08::AddTempPanel(TMyOmronPanel* tmop)
{
    myPalGroup[GetSize()]=tmop;
}
//---------------------------------------------------------------------------
TMyOmronPanel* TfrmDTME08::GetPalGroup(int index)
{
    index=(index<0 || index>=GetSize())?0:index;
    return myPalGroup[index];
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btnSetSaveClick(TObject *Sender)
{
    SaveData();
}
//---------------------------------------------------------------------------
void TfrmDTME08::InitialComboList()
{
    cbCh->Clear();
    for(int i=0; i<GetChannelNumber(); i++)
    {
        cbCh->Items->Add("CH"+IntToStr(i+1));
    }
    cbCh->ItemIndex=0;
}
//---------------------------------------------------------------------------
bool bFirstTime;
void __fastcall TfrmDTME08::TimerUpdateTimer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    if(Temperature.bATCActiveCooling ||                                                 //Jimmychiu 20230628 : ATC6.0不需要跑Omron DTME08
       ATC_SYSTEM==eATC60 ||
       (ATC_SYSTEM==eNewATCSystem && ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60))   //Steven 20201120 : 修正ATC6.0不需要跑Omron EJ1N
        return;

    if(bTimerUpdateLock)
        return;
    else
        bTimerUpdateLock=true;
    //
    DoDTME08Cycle();
    //
    bTimerUpdateLock=false;
}
//---------------------------------------------------------------------------
bool TfrmDTME08::GetErrorMsg()
{
    if(asErrorDTME08!="")
    {
        RecordErrorMsg(asErrorDTME08);
        return true;
    }
    else if(DTMTimeout.Off())
    {
        RecordErrorMsg("time out 10 Sec");
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btnGetPVClick(TObject *Sender)
{
    SetSingleCommand(dtme08->DoGetPV, GetChannelIndex());
    iDoCommandStatus=DoGetPV;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btnSetATClick(TObject *Sender)
{
    SetSingleCommand(dtme08->DoSetAT, GetChannelIndex());
    dtme08->InitialmapDTMInfoList();
    dtme08->mapDTMInfo[TargetNo].RunAutoTunning=GetPalGroup(TargetCh)->cbEnableAT->Checked;
    iDoCommandStatus=DoSetAT;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btnGetSVClick(TObject *Sender)
{
    SetSingleCommand(dtme08->DoGetSV, GetChannelIndex());
    iDoCommandStatus=DoGetSV;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btnSetSVClick(TObject *Sender)
{
    SetSingleCommand(dtme08->DoSetSV, GetChannelIndex());
    dtme08->InitialmapDTMInfoList();
    AnsiString Str=GetPalGroup(TargetCh)->edSV->Text;
    double dTargetSV=atof(Str.c_str());
    dtme08->mapDTMInfo[TargetNo].SetSettingSV(dTargetSV);
    iDoCommandStatus=DoSetSV;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btnGetStatusClick(TObject *Sender)
{
    SetSingleCommand(dtme08->DoGetStatus, GetChannelIndex());
    iDoCommandStatus=DoGetStatus;
}
//---------------------------------------------------------------------------
bool TfrmDTME08::DoSendCommandByManual(bool bIsFirst, int iStation)
{
    if(dtme08->DoCommuncation())
    {
        if(bIsFirst)
        {
            asErrorDTME08="";
            tpvDoCommand(true, iStation);
            return true;
        }
        else if(tpvDoCommand(false, iStation))
        {
            return true;
        }
    }

    if(GetErrorMsg())
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfrmDTME08::GetTargetCh(int ich)
{
    TargetCh=ich;
    GetStationAndNumber(TargetCh, TargetStation, TargetNo);
}
//---------------------------------------------------------------------------
void TfrmDTME08::SetSingleCommand(TPointVoidDoCommand tcdc, int ich)
{
    tpvDoCommand=tcdc;
    iTaskCommand=1;
    DTMTimeout.SetSecAndOn(10);
    GetTargetCh(ich);
}
//---------------------------------------------------------------------------
void TfrmDTME08::GetStationAndNumber(int iCh, int &iStation, int &iNumber)
{
    iStation=ChangeToFloatNonPcnt((double)(iCh), (double)(dtme08->GetChannelNumberPerStation()));
    iNumber =iCh%dtme08->GetChannelNumberPerStation();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btSetThermoTypeClick(TObject *Sender)
{
    iDoManualCommandStatus=DoSetSeonsorType;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btnReloadClick(TObject *Sender)
{
    Reload();
}
//---------------------------------------------------------------------------
bool TfrmDTME08::DoManualProcess()
{
    if(iDoCommandStatus>0)
    {
        switch(iTaskCommand)
        {
            case 1:
                if(DoSendCommandByManual(true, TargetStation))
                {
                    iTaskCommand=10;
                }
                break;
            case 10:
                if(DoSendCommandByManual(false, TargetStation))
                {
                    if(iDoCommandStatus==DoGetSV)
                    {
                        SetSP(TargetCh, dtme08->mapDTMInfo[TargetNo].SV, dtme08->mapDTMInfo[TargetNo].Event);
                    }
                    else if(iDoCommandStatus==DoGetPV)
                    {
                        SetPV(TargetCh, dtme08->mapDTMInfo[TargetNo].PV, dtme08->mapDTMInfo[TargetNo].Event);
                    }
                    else if(iDoCommandStatus==DoGetStatus)
                    {
                        //
                    }
                    iTaskCommand=20;
                }
                break;
            case 20://finish
                iDoCommandStatus=DoNone;
                iTaskCommand=1;
                return false;
        }
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::FormShow(TObject *Sender)
{
    bShow=true;
    DrawTempGUI();
    InitialComboList();
    SetPanelPos();                                                              //Jimmychiu 20230628 : DTME08物件排列
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow=false;
}
//---------------------------------------------------------------------------
void TfrmDTME08::SetSP(int iCh,double SV,AnsiString Event)
{
    GetPalGroup(iCh)->SetSV(SV);
}
//---------------------------------------------------------------------------
void TfrmDTME08::SetPV(int iCh,double PV,AnsiString Event)
{
    GetPalGroup(iCh)->SetPV(AnsiString().sprintf("%5.1f", PV), clYellow);
    GetPalGroup(iCh)->SetEvent(Event,(Event=="Event")?clBlack:clRed);           //警報事件
}
//---------------------------------------------------------------------------
void TfrmDTME08::SetATStatus(int iCh,bool bOn)
{
    GetPalGroup(iCh)->SetAT("AT", (bOn)?clYellow:clBlack);                      //顯示AT是否啟動
}
//---------------------------------------------------------------------------
void TfrmDTME08::InitialDoSetSensorType(int iStation)
{
    for(int i=0; i<dtme08->GetChannelNumberPerStation(); i++)
    {
        TargetCh=dtme08->GetChannelNumberPerStation()*iStation+i;
        dtme08->mapDTMInfo[i].SensorType=dtme08->esstPT100;
        if(rgSensorType->ItemIndex==0)                                          // PT Type
        {
            dtme08->mapDTMInfo[i].SensorType=dtme08->esstPT100;
        }
        else
        {
            dtme08->mapDTMInfo[i].SensorType=dtme08->esstKType;
        }
    }
    SetSingleCommand(dtme08->DoSetSensorType);
    DTMTimeout.SetSecAndOn(GetDTMTimeout());
}
//---------------------------------------------------------------------------
void TfrmDTME08::InitialDoSetAllAT(int iStation)
{
    for(int i=0; i<dtme08->GetChannelNumberPerStation(); i++)
    {
        TargetCh=dtme08->GetChannelNumberPerStation()*iStation+i;
        dtme08->mapDTMInfo[i].RunAutoTunning=GetPalGroup(TargetCh)->cbEnableAT->Checked;
    }
    SetSingleCommand(dtme08->DoSetAT);
    DTMTimeout.SetSecAndOn(GetDTMTimeout());
}
//---------------------------------------------------------------------------
void TfrmDTME08::InitialDoSetAllSV(int iStation)
{
    AnsiString Str="";
    double dTargetSV=0;
    for(int i=0; i<dtme08->GetChannelNumberPerStation(); i++)
    {
        TargetCh=dtme08->GetChannelNumberPerStation()*iStation+i;
        Str=GetPalGroup(TargetCh)->edSV->Text;
        dTargetSV=atof(Str.c_str());
        dtme08->mapDTMInfo[i].SetSettingSV(dTargetSV);
    }
    SetSingleCommand(dtme08->DoSetSV);
    DTMTimeout.SetSecAndOn(GetDTMTimeout());
}
//---------------------------------------------------------------------------
void TfrmDTME08::InitialDoSetCycleTime(int iStation)
{
    for(int i=0; i<dtme08->GetChannelNumberPerStation(); i++)
    {
        TargetCh=dtme08->GetChannelNumberPerStation()*iStation+i;
        dtme08->mapDTMInfo[i].SetCycleTime(GetCycleTime());
    }
    SetSingleCommand(dtme08->DoSetCycleTime);
    DTMTimeout.SetSecAndOn(GetDTMTimeout());
}
//---------------------------------------------------------------------------
void TfrmDTME08::InitialDoSetControlActionOut2(int iStation)
{
    for(int i=0; i<dtme08->GetChannelNumberPerStation(); i++)
    {
        TargetCh=dtme08->GetChannelNumberPerStation()*iStation+i;
        dtme08->mapDTMInfo[i].ControlActionOut2=dtme08->ecaDisable;
    }
    SetSingleCommand(dtme08->DoSetControlActionOut2);
    DTMTimeout.SetSecAndOn(GetDTMTimeout());
}
//---------------------------------------------------------------------------
void TfrmDTME08::InitialSetting(int iStation)
{
    if(iDoAutoCommandStatus==DoSetAT)
    {
        InitialDoSetAllAT(iStation);
    }
    else if(iDoAutoCommandStatus==DoSetSV)
    {
        InitialDoSetAllSV(iStation);
    }
    else if(iDoAutoCommandStatus==DoSetSeonsorType)
    {
        InitialDoSetSensorType(iStation);
    }
    else if(iDoAutoCommandStatus==DoSetCycleTime)
    {
        InitialDoSetCycleTime(iStation);
    }
    else if(iDoAutoCommandStatus==DoSetControlActionOut2)
    {
        InitialDoSetControlActionOut2(iStation);
    }
}
//---------------------------------------------------------------------------
void TfrmDTME08::ClearAllCommandStatus()
{
    if(bCycleFirstRun==false)
    iDoInitialCommandStatus=DoInitialNone;
    iDoCommandStatus=DoNone;
    iDoAutoCommandStatus=DoNone;
    iNowActiveStation=0;
}
//---------------------------------------------------------------------------
void TfrmDTME08::SetSettingSV(int iCh, double dTemp)
{
    bool bdiff=!(GetPalGroup(iCh)->GetSV()==dTemp);
    if(bdiff)
    {
        HeaterSVLog(iTempCode[iCh], dTemp);                                     //Steven 20250116 : add SV change log
        GetPalGroup(iCh)->SetSettingSV(dTemp);
    }
}
//---------------------------------------------------------------------------
bool TfrmDTME08::IsNeedSetSV()
{
    for(int i=0; i<GetSize(); i++)
    {
        if(i>=GetChannelNumber())
            return false;
        if(GetPalGroup(i)->GetSettingSV()!=GetPalGroup(i)->GetSV())
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
const int iAutoCycleCount=10;
void TfrmDTME08::DoDTME08Cycle()
{
    int &Task=iTaskDTME08Cycle;
    //ctrl
    if(bCtrlSwitch)
    {
        btnCtrl->Caption="Running";
    }
    else
    {
        btnCtrl->Caption="Stop";
        return;
    }
    //
    if(DTMReconnectTime.Off())
    {
        if(dtme08->DoCommuncation())
        {
            if(ledEJ1N1->Value==false)
                ledEJ1N1->Value=true;
        }
        else
        {
            if(ledEJ1N1->Value==true)
                ledEJ1N1->Value=false;
            DTMReconnectTime.SetSecAndOn(GetReconnectTime());
            return;
        }
    }
    else
    {
        return;
    }
    //
    switch(Task)
    {
        case 1:
            if(bCycleFirstRun)
            {
                if(iDoInitialCommandStatus>=DoInitialTotal)
                {
                    DTMTimeout.SetSecAndOn(GetDTMTimeout());
                    bCycleFirstRun=false;
                }
                else if(iDoInitialCommandStatus==DoInitialSetTypeSeonsor)
                {
                    iDoAutoCommandStatus=DoSetSeonsorType;
                    Task=100;
                }
                else if(iDoInitialCommandStatus==DoInitialSetCycleTime)
                {
                    iDoAutoCommandStatus=DoSetCycleTime;
                    Task=100;
                }
                else if(iDoInitialCommandStatus==DoInitialSetControlActionOut2)
                {
                    iDoAutoCommandStatus=DoSetControlActionOut2;
                    Task=100;
                }
                else if(iDoInitialCommandStatus==DoInitialSetSV)
                {
                    iDoAutoCommandStatus=DoSetSV;
                    Task=100;
                }
                else
                {
                    DTMTimeout.SetSecAndOn(GetDTMTimeout());
                }
                iDoInitialCommandStatus++;
            }
            else if(DoManualProcess()) //single ctrl
            {
                Task=9000;//finish
            }
            else if(iDoManualCommandStatus!=DoNone)//Manual
            {
                if(iDoManualCommandStatus==DoSetSV)
                {
                    iDoAutoCommandStatus=DoSetSV;
                    Task=100;
                }
                else if(iDoManualCommandStatus==DoSetAT)
                {
                    iDoAutoCommandStatus=DoSetAT;
                    Task=100;
                }
                else if(iDoManualCommandStatus==DoSetSeonsorType)
                {
                    iDoAutoCommandStatus=DoSetSeonsorType;
                    Task=100;
                }
                iDoManualCommandStatus=DoNone;
            }
            else //Cycle Run
            {
                if(iCycleRunNumber%iAutoCycleCount==1)  //getstatus
                {
                    SetSingleCommand(dtme08->DoGetStatus);
                    iDoAutoCommandStatus=DoGetStatus;
                }
                else if(iCycleRunNumber%iAutoCycleCount==2)  //get sv
                {
                    SetSingleCommand(dtme08->DoGetSV,0);
                    iDoAutoCommandStatus=DoGetSV;
                }
                else if(iCycleRunNumber%iAutoCycleCount==3)  //set sv
                {
                    if(IsNeedSetSV())
                    {
                        iDoAutoCommandStatus=DoSetSV;
                    }
                    else
                    {
                        SetSingleCommand(dtme08->DoGetPV,0);
                        iDoAutoCommandStatus=DoGetPV;
                    }
                }
                else //bDoGetPV
                {
                    SetSingleCommand(dtme08->DoGetPV,0);
                    iDoAutoCommandStatus=DoGetPV;
                }
                Task=100;
                DTMTimeout.SetSecAndOn(GetDTMTimeout());
                if(iCycleRunNumber>65530)
                    iCycleRunNumber=0;
                else
                    iCycleRunNumber++;
            }
            break;
        case 100://station
            InitialSetting(iNowActiveStation);
            if(tpvDoCommand!=NULL)
            {
                if(DoSendCommandByManual(true, iNowActiveStation))
                {
                    Task=200;
                }
            }
            else
            {
                Task=9000;
            }
            break;
        case 200:
            if(tpvDoCommand!=NULL)
            {
                if(DoSendCommandByManual(false, iNowActiveStation))
                {
                    if(iDoAutoCommandStatus==DoGetSV)
                    {
                        for(int i=0; i<dtme08->GetChannelNumberPerStation(); i++)
                        {
                            TargetCh=dtme08->GetChannelNumberPerStation()*iNowActiveStation+i;
                            SetSP(TargetCh, dtme08->mapDTMInfo[i].SV, dtme08->mapDTMInfo[i].Event);
                        }
                    }
                    else if(iDoAutoCommandStatus==DoGetPV)
                    {
                        for(int i=0; i<dtme08->GetChannelNumberPerStation(); i++)
                        {
                            TargetCh=dtme08->GetChannelNumberPerStation()*iNowActiveStation+i;
                            SetPV(TargetCh, dtme08->mapDTMInfo[i].PV, dtme08->mapDTMInfo[i].Event);
                        }
                    }
                    else if(iDoAutoCommandStatus==DoGetStatus)
                    {
                        for(int i=0;i<dtme08->GetChannelNumberPerStation();i++)
                        {
                            TargetCh=dtme08->GetChannelNumberPerStation()*iNowActiveStation+i;
                            SetATStatus(TargetCh,dtme08->mapDTMInfo[i].IsRunAutoTuning);
                        }
                    }
                    Task=9000;
                }
            }
            else
            {
                Task=9000;
            }
            break;
        case 9000://finish
            iNowActiveStation++;
            DTMTimeout.SetSecAndOn(GetDTMTimeout());
            if(GetChannelNumber()>=(iNowActiveStation+1)*dtme08->GetChannelNumberPerStation())//continue
            {
                Task=100;
            }
            else  //finish
            {
                ClearAllCommandStatus();
                Task=1;
            }
            break;

    }
    if(DTMTimeout.Off())
    {
        RecordErrorMsg(AnsiString().sprintf("DoDTME08Cycle Time Out %d Sec", GetDTMTimeout()));
        Task=9000;                                                              //finish
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btATClick(TObject *Sender)
{
    for(int i=0; i<GetSize(); i++)
    {
        GetPalGroup(i)->SetAT(true);
    }
    iDoManualCommandStatus=DoSetAT;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btSetSVClick(TObject *Sender)
{
    iDoManualCommandStatus=DoSetSV;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btAT1by1Click(TObject *Sender)
{
    iDoManualCommandStatus=DoSetAT;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btATOffClick(TObject *Sender)
{
    for(int i=0; i<GetSize(); i++)
    {
        GetPalGroup(i)->SetAT(false);
    }
    iDoManualCommandStatus=DoSetAT;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btnSetSensorClick(TObject *Sender)
{
    iDoAutoCommandStatus=DoSetSeonsorType;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btnCtrlClick(TObject *Sender)
{
    bCtrlSwitch=!bCtrlSwitch;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::btnSetSVFollowCh1Click(TObject *Sender)
{
    for(int i=0; i<GetSize(); i++)
    {
        GetPalGroup(i)->edSV->Text=GetPalGroup(0)->edSV->Text;
    }
    iDoManualCommandStatus=DoSetSV;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::chkHeaterClick(TObject *Sender)
{
    SW[SwHeaterRelay].OnOff(chkHeater->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::edPortClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDTME08::edSVClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 1, true, dTempMax, 0.0);
}
//---------------------------------------------------------------------------
void TfrmDTME08::SetPanelPos()                                                  //Jimmychiu 20230628 : DTME08物件排列
{
    int numOfCol=ScrollBox1->Width/210;
    //排列方式:
    // 1-1, 1-3, 2-1, 2-3
    // 1-2, 1-4, 2-2, 2-4
    int iPanelSort[32]=
    {
        0,  2,  4,  6,
        1,  3,  5,  7,
        8, 10, 12, 14,
        9, 11, 13, 15,
       16, 18, 20, 22,
       17, 19, 21, 23,
       24, 26, 28, 30,
       25, 27, 29, 31,
    };
    int iMem=0;
    for(int i=0; i<GetSize(); i++)
    {
        iMem=iPanelSort[i];
        GetPalGroup(iMem)->GroupBox->Left     = 210*(i%numOfCol);
        GetPalGroup(iMem)->GroupBox->Top      = 124*(ChangeToFloatNonPcnt((double)(i), (double)(numOfCol)));
    }
}
//---------------------------------------------------------------------------
