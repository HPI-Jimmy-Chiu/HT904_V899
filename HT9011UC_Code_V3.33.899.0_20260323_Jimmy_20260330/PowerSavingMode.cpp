#include "MachineDefine.h"
#pragma hdrstop

#include "PowerSavingMode.h"

#include "csystem.h"
#include "mymessbox.h"
#include "main.h"
#include "cmydef.h"
#include "cprod.h"
#include "MyMotor.h"
#include "Note.h"
#include "cBinSel.h"
#include "cContactCT.h"
#include "uLotInfo.h"
#include "cSortCT.h"
#include "cTestCategory.h"
#include "cTemperFrom.h"
#include "ATCInterface.h"
#include "ATC_Handler_Side.h"                                                   //Ifor 20151230 :add New ATC Interface
#include "cpublic.h"
//#include "common.h"
#include "MyKitSuck.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

class TPowerSaving *tPowerSaving;
const int MaxMinute=200;                                                        //Alarm 設定最大值
TDateTime OverDayPM=StrToTime("pm 11:59:59");
TDateTime OverDayAM=StrToTime("am 12:00:00");
//---------------------------------------------------------------------------
__fastcall TPowerSaving::TPowerSaving()
{
    ScanTmr=new TTimer(fMain);

    ScanTmr->OnTimer=OnScanTmr;
    ScanTmr->Interval=1000;
    ScanTmr->Enabled=false;

    flagStartTmr=false;
    bRestart=true;

    MtrModule   =new TMtrModule;
    VacuumModule=new TVacuumModule;                                             //Steven 20221215 : Power saving for vacuum pump
    HotModule   =new THotModule;
    tModule     =new TModule;

    SetFunction(true);
    flagUseFunc=true;
    HotModule->bCheckTempClose=false;
    tModule->iCountDown=StrToTime("00:00:00");
}
//---------------------------------------------------------------------------
__fastcall TPowerSaving::~TPowerSaving()
{
    try
    {
        delete VacuumModule;
        delete MtrModule;
        delete HotModule;
        delete tModule;
        delete ScanTmr;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TPowerSaving");
    }
}
//---------------------------------------------------------------------------
//主流程
void __fastcall TPowerSaving::OnScanTmr(TObject *Sender)
{
    static int Task=0;

    if(InitialOK==false)
        return;

    if(SystemStart || IniConfig.bPowerSaveFunction==false)
    {
        bRestart=true;
        return;
    }

    if(IniConfig.bC05_PowerSaveTemp==false &&
       IniConfig.bC05_PowerSaveMotor==false &&
       IniConfig.bC05_PowerSaveVacuum==false)                                   //Steven 20221215 : Power saving for vacuum pump
    {
        fMain->pnlPowerSaving->Visible=false;
        return;
    }
    else
    {
        fMain->pnlPowerSaving->Visible=true;
    }

    if(CUSTOMER_CODE==CC_TSMC_TAINAN && bPowersavingStart==false)
        return;

//    if(CUSTOMER_CODE==CC_GIGAS &&                                               //Isaac 20200720 : 客戶要求開啟contact頁面下，不啟動省電模式
    if(fContact->fShow)                                                         //Steven 20250324 : contact頁面下，不啟動省電模式
    {
        bRestart=true;
        return;
    }

    if(bRestart)
    {
        bRestart=false;
        Task=0;
        return ;
    }
    ScanTmr->Enabled=false;
    switch(Task)
    {
        case 0:
            if(CheckSystemRun())
            {
                flagStartTmr=false;
                Task++;
            }
            break;
        case 1:
            flagStartTmr=true;
            StartTmr=Time();
            HotModule->Doing(true);
            Task++;
            break;
        case 2:
            if(flagUseFunc)
            {
                if(tModule->TimeOut(StartTmr))
                {
                    Task++;
                }
                else if(CheckChangeState()==false)
                {
                    Restart();
                }
                if(tModule->iCountDown>OverDayAM)
                    fMain->pnlPowerSaving->Caption="PowerSaving:"+tModule->iCountDown.FormatString("hh:nn:ss");
//                    fMain->StatusBar1->Panels->Items[7]->Text="PowerSaving:"+tModule->iCountDown.FormatString("hh:nn:ss"); //Ifor 20160601 Power Saving Mode 顯示位置與時間衝突 Items[6] ==> Items[7]
            }
            break;
        case 3:
            int iTmp;
            iTmp=0;

            if(IniConfig.iPowersaveMode==0)
            {
                Task++;
            }
            else if(IniConfig.iPowersaveMode==1)
            {
//                iTmp=ShowErrorMessage("MES15193", K_RETRY|K_SKIP, 0, MMSystem);   //Enter power saving mode
//                if(iTmp==K_RETRY)
                //jou 2012-03-06 不能用ShowErrorMessage,因為retry->Start,機台就跑起來了
                iTmp=ShowMyMessageBox_YES_NO("Enter power saving mode", "進入省電模式");   //Enter power saving mode
                if(iTmp==1)
                {
                    Task++;
                }
                else
                {
                    Restart();
                }
            }
            break;
        case 4:
            if(MtrModule->Enabled)
            {   //Use Motor Module
                if(MtrModule->TimeOut(StartTmr))
                {
                    MtrModule->Doing(false);
                    MtrModule->Enabled=false;
                }
            }

            if(HotModule->Enabled)
            {  //Use Hot Module
                if(HotModule->TimeOut(StartTmr))
                {
                    HotModule->bCheckTempClose=true;
                    if(HotModule->Doing(false))
                    {
                        HotModule->Enabled=false;
                    }
                }
            }

            if(VacuumModule->Enabled)               //Steven 20221215 : Power saving for vacuum pump
            {  //Use Hot Module
                if(VacuumModule->TimeOut(StartTmr))
                {
//                    VacuumModule->bCheckTempClose=true;
                    if(VacuumModule->Doing(false))
                    {
                        VacuumModule->Enabled=false;
                    }
                }
            }

            if(SW[SwLightOff].Enable==true)
                SW[SwLightOff].Off();    //wei 20180627
//            if(SW[SwAirOff].Enable==true)
//                SW[SwAirOff].Off();      //wei 20180627

            if(MtrModule->Enabled==false &&
               HotModule->Enabled==false &&
               VacuumModule->Enabled==false)        //Steven 20221215 : Power saving for vacuum pump
            {
                Task++;
            }
            break;
        case 5:
            bPowersavingStart=false;
            break;
        default:
            Task =0;
            break;
    }
    ScanTmr->Enabled=true;
}
//---------------------------------------------------------------------------
bool TPowerSaving::CheckSystemRun()
{
    if((SystemStart==false) && (HasICUnderMachine()==false) && CheckChangeState())
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
//單位:分鐘
void TPowerSaving::SetAlarmTime(TDateTime &Module ,int Minute)
{
    Word whh ,wmm ,wss ,wms;

    if(Minute==0)
        Minute=1;
    else if(Minute>MaxMinute)
        Minute=MaxMinute;

    whh=Minute/60;
    wmm=Minute%60;
    wss=wms=0;

    Module=EncodeTime(whh, wmm, wss, wms);
}
//---------------------------------------------------------------------------
void TPowerSaving::SetFunction(bool OnOff)
{
    int tmp=0;
    flagUseFunc=(IniConfig.bC05_PowerSaveTemp ||
                 IniConfig.bC05_PowerSaveMotor ||
                 IniConfig.bC05_PowerSaveVacuum);                               //Steven 20221215 : Power saving for vacuum pump

    TStringList *list;
    list=new TStringList();
    if(IniConfig.bC05_PowerSaveTemp)
        list->Add(IniConfig.iHaltTime_Temp);
    if(IniConfig.bC05_PowerSaveMotor)
        list->Add(IniConfig.iHaltTime_Motor);
    if(IniConfig.bC05_PowerSaveVacuum)
        list->Add(IniConfig.iC05HaltTime_Vacuum);

    list->Sort();                                                               //排序
    if(list->Count!=0)
        tmp=atoi(list->Strings[0].c_str());                                     //取最小值

    list->Clear();
    delete list;

    if(flagUseFunc)
    {
        tModule->Enabled=true;
        SetAlarmTime(tModule->AlarmTmr, tmp);

        ScanTmr->Enabled=true;
    }
    else
    {
        ScanTmr->Enabled=false;
    }

    if(IniConfig.bC05_PowerSaveMotor)
    {
        SetAlarmTime(MtrModule->AlarmTmr, IniConfig.iHaltTime_Motor);
    }

    if(IniConfig.bC05_PowerSaveTemp)
    {
        SetAlarmTime(HotModule->AlarmTmr, IniConfig.iHaltTime_Temp);
    }

    if(IniConfig.bC05_PowerSaveVacuum)                                          //Steven 20221215 : Power saving for vacuum pump
    {
        SetAlarmTime(HotModule->AlarmTmr, IniConfig.iC05HaltTime_Vacuum);
    }
    MtrModule->Enabled=IniConfig.bC05_PowerSaveMotor;
    HotModule->Enabled=IniConfig.bC05_PowerSaveTemp;
    HotModule->bCheckTempClose=false;
    VacuumModule->Enabled=IniConfig.bC05_PowerSaveVacuum;
}
//---------------------------------------------------------------------------
void TPowerSaving::Restart()
{
   bRestart=true;
   SetFunction(true);
   tModule->iCountDown=tModule->AlarmTmr;
}
//---------------------------------------------------------------------------
bool TPowerSaving::CheckChangeState()                                           //2013-03-05    Dell modify for ATK 在主畫面不是在最前就不能進入
{
    bool bRe=true;
    if(InitialOK==false)                                                        //ChungHung 20140617 close program some time happen error
        return true;
    if(CUSTOMER_CODE==CC_AMKOR_Korea)                                           //2013-03-05    Dell modify for ATK 在主畫面不是在最前就不能進入
    {
        bRe=fMain->Active;
        if(bRe==false)
        {
            bRe=(fBinSel->Active || fContactCT->Active ||
                 fLotInfo->Active || fSortCT->Active ||
                 fTestCategory->Active || fTemperFrom->Active);
        }
    }

    return bRe;
}
//---------------------------------------------------------------------------
__fastcall TModule::TModule()
{
    Enabled =true;
}
//---------------------------------------------------------------------------
__fastcall TModule::~TModule()
{

}
//---------------------------------------------------------------------------
bool TModule::TimeOut(TDateTime StartTmr)
{
    TDateTime nowT=Time();

    bool bResult=false;
    AnsiString ss;

    if(nowT>StartTmr)
    {
        bResult=((nowT-StartTmr)>AlarmTmr);
        iCountDown=((StartTmr+AlarmTmr)-nowT);
    }
    else
    {
        bResult=((nowT-OverDayAM)+(OverDayPM-StartTmr))>AlarmTmr;
        iCountDown=((StartTmr+AlarmTmr)-nowT);
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool TModule::Doing(bool Reset)
{
   return true;
}
//---------------------------------------------------------------------------
bool TMtrModule::Doing(bool Reset)
{
    fHome->GaliMotorServoOff("Motor enter power saving mode.");                 //Steven 20230712 : 修正SwServoOn.Off時, 要抓住Z煞車
    RecordProcess("Motor enter power saving mode.");                            //Steven 20230303 : 省電模式增加Log
    bPowersaving=true;                                                          //wei 20180301 顯示Powersaving
    return true;
}
//---------------------------------------------------------------------------
bool TVacuumModule::Doing(bool Reset)                                           //Steven 20221215 : Power saving for vacuum pump
{
    if(FTestSuck.HasRealIC()==false && BTestSuck.HasRealIC()==false)
    {
        if(SW[SwAirOff].Enable==true)
        {
            SW[SwAirOff].Off();
        }
        bPowersaving=true;                                                      //wei 20180301 顯示Powersaving
        fAllMotorHome=false;                                                    //Steven 20110211
        RecordProcess("Vacuum pump enter power saving mode.");                  //Steven 20230303 : 省電模式增加Log
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool THotModule::Doing(bool Reset)
{
    static int Task=0;
    int Waiting=10000;
    bool bResult=false;

    if(Reset)
    {
        Task=0;
        return bResult;
    }
    switch(Task)
    {
        case 0:
            RecordProcess("Heater enter power saving mode.");                   //Steven 20230303 : 省電模式增加Log
            SW[SwHeaterRelay].Off();
            HeaterLog("Power Saving_THotModule::Doing", false);                 //Steven 20151123 : Log for Heater Relay

            if(ATC_SYSTEM!=eNonChamber && ATC_SYSTEM!=eATCUninstall)            //Steven 20170922 (wei) : 修正省電模式沒裝ATC會向賽
            {
                if(Temperature.bATC70Active==true)
                {
                    ATCInterfaceForm->SendCommToATC7(ATC_STOP, "", "");
                }
                else
                {
                    ATCInterfaceForm->SetRunATC(false);                         //Steven 20151110 : ATC在省電模式也要停下來
                    bRunATC=false;                                              //ChungHung 20160118 add for Hisi V102
                    ATCInterfaceForm->iStopATCChillerType=1;
                }
            }
            tHotModuleTimer.SetMSAndOn(Waiting);
            Task++;
            break;
        case 1:
            if(tHotModuleTimer.Off())
            {
                SW[SwHeaterFan].Off();
                bResult=true;
                if(CUSTOMER_CODE==CC_AMKOR_Korea)
                    ShowMyMessagePWD("", "Enter power saving mode", NULL, true, false, true);
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
