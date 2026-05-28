#include "MachineDefine.h"
#pragma hdrstop

#include "cSpeed.h"

#include "common.h"
#include "cprod.h"
#include "cpublic.h"
#include "uShowMessage.h"
#include "cmydef.h"
#include "myQwertyKeyBoard.h"
#include "cSecurity.h"
#include "cAuthority.h"
#include "csystem.h"
#include "MyKitSuck.h"
#include "uHGemHT9045.h"
#include "RPDefault.h"
#include "main.h"
#include "cObserver.h"
#include "mymessbox.h"
//#include "rs232.h"
#include "TrayStepMotor.h"
#include "uCleaning.h"
#include "uYieldMonitoring.h"
#include "cAuthority.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfSpeed *fSpeed;
//---------------------------------------------------------------------------
__fastcall TfSpeed::TfSpeed(TComponent* Owner)
    : TForm(Owner)
{
    LastFileName="";
    tbAllSpeed->Enabled=false;
    spbSpeedAdd->Enabled=false;
    spbSpeedDec->Enabled=false;
    edtHPVacuumDelay->Hint="edtHPVacuumDelay";                                  //Sam 20201209 : Default Recipe ChangeLog
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::FormShow(TObject *Sender)
{
    AnsiString S;
    LastFileName=GetLastOpenFN();
    ReadFile();
    DoIniDataToForm();
    S.sprintf("Speed Condition  ''%s''  ",LastFileName);
    Caption=S;
    PageControl1->ActivePage=tsAllSpeed;
    fShow=true;
    Left=125;                                                                   //Steven 20091103
    Top=10;                                                                     //Steven 20091103

    gbIndexRetry->Visible   =false;
    gbAutoSkip->Visible     =IniConfig.bRecordSkipPosition;                     //Steven 20140816 : rgInArmAutoSpeed --> gbAutoSkip
    edIndexVacumCheckTime->Enabled=true;                                        //JerryYang 20220311 : ATP鎖定Critical parameter
    edIndexAirOnTime->Enabled=true;
    edIndexSpeed->Enabled   =true;
    gbInArmCylinderDelay->Visible =(InOutArmPickerUseMotor!=eptUseMot);
    gbOutArmCylinderDelay->Visible=(InOutArmPickerUseMotor!=eptUseMot);

    edIndexRetryCount->Text =0;
    edIndexArmRetryMM->Text =0;
    edIndexAccDec->Text     =100;                                               //Steven 20091214 Add for Index ACC & DEC must be 100%

    palInRotate->Visible    =(USE_ROTATE_KIT && iRotate_Type!=eCynRotate);      //Steven 20170425 (wei) : Add rotate motor
    palOutRotate->Visible   =(USE_ROTATE_KIT && iRotate_Type!=eCynRotate);
    cbInRotate->Visible     =(USE_ROTATE_KIT && iRotate_Type!=eCynRotate);
    cbOutRotate->Visible    =(USE_ROTATE_KIT && iRotate_Type!=eCynRotate);
    tsMagazine->Visible     =(AUTO3_IS_MAGAZINE==1);                            //JerryYang 20220909 : add magazine
    //jou 2010-05-19 start : 負壓
    if(INDEX_SUCKER_TYPE==0)
    {
        grpIndexPreSuck->Visible=true;
    }
    else
    {
        grpIndexPreSuck->Visible=false;
        chkIndexPreSuck->Checked=true;
    }
    //jou 2010-05-19 end

    rgAutoSpeed->Visible    =CosFunction.bEnableAutoSpeed;                      //Steven 20110526
    rgStepShuttle->Visible  =IniConfig.bEnableStepShuttle;                      //jou 2013-07-16 Step Shuttle check Index -> Input
    rgShakeShuttle->Visible =CosFunction.bShakeShuttleEveryTime;                //Steven 20200616 : ATK要求每次放料都要搖搖蝦頭
    rgInShtOrder->Visible   =(TestIF_File.iTestMode==QualSite1X4 ||             //Steven 20250630 : 力旺要照順序擺
                              TestIF_File.iTestMode>=_8Site2X4);

    if(CosFunction.bLimitMaxSpeed==true)                                        //Steven 20131101 : 限制最高速度
    {
        tbAllSpeed->Max     =CosFunction.iLimitMaxSpeed;
        udInXSpd->Max       =CosFunction.iLimitMaxSpeed;
        udInZSpd->Max       =CosFunction.iLimitMaxSpeed;
        udInPitchSpd->Max   =CosFunction.iLimitMaxSpeed;
        udOutXSpd->Max      =CosFunction.iLimitMaxSpeed;
        udOutZSpd->Max      =CosFunction.iLimitMaxSpeed;
        udOutPitchSpd->Max  =CosFunction.iLimitMaxSpeed;
        udTrayXSpd->Max     =CosFunction.iLimitMaxSpeed;
        udIndexSpd->Max     =CosFunction.iLimitMaxSpeed;
        udSht1Spd->Max      =CosFunction.iLimitMaxSpeed;
        udSht2Spd->Max      =CosFunction.iLimitMaxSpeed;
        udInRotSpd->Max     =CosFunction.iLimitMaxSpeed;
        udOutRotSpd->Max    =CosFunction.iLimitMaxSpeed;

        tbAccSpeed->Max     =CosFunction.iLimitMaxSpeed;
        udInXAcc->Max       =CosFunction.iLimitMaxSpeed;
        udInZSpd->Max       =CosFunction.iLimitMaxSpeed;
        udInPitchAcc->Max   =CosFunction.iLimitMaxSpeed;
        udOutXSpd->Max      =CosFunction.iLimitMaxSpeed;
        udOutZAcc->Max      =CosFunction.iLimitMaxSpeed;
        udOutPitchAcc->Max  =CosFunction.iLimitMaxSpeed;
        udIndexAcc->Max     =CosFunction.iLimitMaxSpeed;
        udSht1Acc->Max      =CosFunction.iLimitMaxSpeed;
        udSht2Acc->Max      =CosFunction.iLimitMaxSpeed;
        udTrayXAcc->Max     =CosFunction.iLimitMaxSpeed;
        udInRotAcc->Max     =CosFunction.iLimitMaxSpeed;
        udOutRotAcc->Max    =CosFunction.iLimitMaxSpeed;
        tbEPControl->Max    =CosFunction.iLimitMaxSpeed;                        //Ifor 20150710 :限制最高速度
        udLoaderSpeed1->Max =CosFunction.iLimitMaxSpeed;
        udEmptySpeed1->Max  =CosFunction.iLimitMaxSpeed;
        udColorSpeed1->Max  =CosFunction.iLimitMaxSpeed;
        udAuto1Speed1->Max  =CosFunction.iLimitMaxSpeed;
        udAuto2Speed1->Max  =CosFunction.iLimitMaxSpeed;
        udAuto3Speed1->Max  =CosFunction.iLimitMaxSpeed;
        udAuto4Speed1->Max  =CosFunction.iLimitMaxSpeed;                        //Steven 20230907 : For HT-9011UC
        udAuto5Speed1->Max  =CosFunction.iLimitMaxSpeed;
        udAuto6Speed1->Max  =CosFunction.iLimitMaxSpeed;
    }

    grpHPVacuumDelay->Visible=IniConfig.bE57HPCanUseAnotherVacuumDelay;         //Steven 20180125 (Jou) : 加熱盤的真空等待時間
    rgInArmPitch->Enabled   =true;
    rgOutArmPitch->Enabled  =true;
    if(TestIF_File.iTestMode==QualSite1X4 ||
       TestIF_File.iTestMode>=_8Site2X4)                                        //Steven 20140212 : For Xilinx 31x31生產8Site
    {
        if(DeviceForm.XDimension>=ArmMaxPitch_13Suck &&
           DeviceForm.XDimension<ArmMaxPitch)                                   //Steven 20140212 : IC大於25mm時,一次吸四顆只能使用Fix Mode
        {
            rgInArmPitch->Enabled =false;
            rgOutArmPitch->Enabled=false;
        }
    }

    if(IniConfig.bRecordSkipPosition)                                           //wei 20150320  Onecycle 後才能開關Auto Skip 避免吸嘴錯誤hang up
    {
        if(InArmSuck.HasIC() || OutArmSuck.HasIC() ||
           ShuttleHasIC()    || IndexHasIC())
        {
            rgInArmAutoSpeed->Enabled=false;
        }
        else
        {
            rgInArmAutoSpeed->Enabled=true;
        }
    }
    gbOutPickErrAct->Visible=(CUSTOMER_CODE==CC_AMKOR_Japan);                   //Steven 20170202 : For ATJ Out Arm pickup error put to define tray

    if(CUSTOMER_CODE==CC_ASE_CL)                                                //JerryYang 20170303 (wei) ASE_CL 權限控制
    {
        rgAutoSpeed->Enabled          =fSecurity->Insufficient(133, false);
        gbIndexWaitTime->Enabled      =fSecurity->Insufficient(134, false);
        gbIndexDestoryTime->Enabled   =fSecurity->Insufficient(135, false);
        //Input Arm
        gbInVacuumOnFunc->Enabled     =fSecurity->Insufficient(136, false);
        gbInArmReleaseDelay->Enabled  =fSecurity->Insufficient(137, false);
        gbInArmRetry->Enabled         =fSecurity->Insufficient(138, false);
        gbInArmWaitTime->Enabled      =fSecurity->Insufficient(138, false);
        gbInArmDestoryTime->Enabled   =fSecurity->Insufficient(139, false);
        gbInDestoryCheck->Enabled     =fSecurity->Insufficient(139, false);
        gbTwoSpeed_In->Enabled        =fSecurity->Insufficient(140, false);
        //Output Arm
        gbOutVacuumOnFunc->Enabled    =fSecurity->Insufficient(141, false);
        gbOutArmRetry->Enabled        =fSecurity->Insufficient(142, false);
        gbOutArmWaitTime ->Enabled    =fSecurity->Insufficient(142, false);
        gbOutArmDestoryTime->Enabled  =fSecurity->Insufficient(143, false);
        gbOutDestoryCheck->Enabled    =fSecurity->Insufficient(144, false);
        gbTwoSpeed_Out->Enabled       =fSecurity->Insufficient(145, false);
        gbTrayArmWaitTime->Enabled    =fSecurity->Insufficient(146, false);
    }
    else //jou 981207 權限控制
    {
        tsAllSpeed->Enabled =(fSecurity->Insufficient(3, false) && authMainForm[3]);
        tsInArm->Enabled    =(fSecurity->Insufficient(3, false) && authMainForm[3]);
        tsShuttle->Enabled  =(fSecurity->Insufficient(3, false) && authMainForm[3]);
        tsIndex->Enabled    =(fSecurity->Insufficient(3, false) && authMainForm[3]);
        tsOutArm->Enabled   =(fSecurity->Insufficient(3, false) && authMainForm[3]);
        tsTrayArm->Enabled  =(fSecurity->Insufficient(3, false) && authMainForm[3]);
        tsLoader->Enabled   =(fSecurity->Insufficient(3, false) && authMainForm[3]);
    }
    ckb_HeightCheck_In->Visible         =(CUSTOMER_CODE==CC_TSMC_TAINAN);       //wei 20160825 回吸偵測
    Edt_HeightCheck_In->Visible         =(CUSTOMER_CODE==CC_TSMC_TAINAN);       //wei 20160825 回吸偵測
    gbInArmReleaseDelay->Visible        =(CUSTOMER_CODE==CC_TSMC_TAINAN);       //JerryYang 20170406 (Steven) 只有TSMC才使用input arm release IC前delay
    gbInarmDeviveConfirm->Visible       =(CUSTOMER_CODE==CC_ASE_KaohSiung);
    gbOutarmDeviveConfirm->Visible      =(CUSTOMER_CODE==CC_ASE_KaohSiung);
    gbIndexeviveConfirm->Visible        =(CUSTOMER_CODE==CC_ASE_KaohSiung);     //kevin 20190629 add index 4 site conform
    grpIndexCycleTimeMonitoring->Visible=(CUSTOMER_CODE==CC_SCK ||              //Isaac 20180301 (Steven) Index Cycle Time Monitoring function，SCK功能開放
                                          IniConfig.bSPILFunction==true);       //JerryYang 20220824 : SPIL index cycle time monitor

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20220923 : Index cycle time監控
    {
        chkEnableIndexCycleTimeMonitoring->Enabled=(AccessLevel>=iDefHonPrecLevel);
        edtIndexCycleTimetolerance->Enabled=(AccessLevel>=iDefHonPrecLevel);
    }
//    grpIndexCycleTimeMonitoring->Visible=(!IniConfig.bSPILFunction);          //JerryYang 20220824 : SPIL index cycle time monitor

    rgSearchLastModeForYPitch->Visible=(CosFunction.bYPitchNotUseSearchLastMode==true && USE_OUT_Y_IS_AUTO_PITCH==true);       //JerryYang 20181112 (Steven) : ATP要求修改unloader擺放方式，盡量不要有空格  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    rgSearchInarmLastModeForYPitch->Visible=(CosFunction.bYPitchNotUseSearchLastMode==true && USE_IN_Y_IS_AUTO_PITCH==true);  //kevin 20211130 for in arm Ypitch  //JerryYang 20181112 (Steven) : ATP要求修改unloader擺放方式，盡量不要有空格
    rbTemp->SetFocus();

    //Steven 20200529 : Loader入Tray改步進
    //==>
    tsLoader->TabVisible=(LoaderUnload_StepMotor ||
                          LOAD_Z_USE_MOTOR[0] || LOAD_Z_USE_MOTOR[1] || LOAD_Z_USE_MOTOR[2] ||
                          LOAD_Z_USE_MOTOR[3] || LOAD_Z_USE_MOTOR[4] || LOAD_Z_USE_MOTOR[5] ||
                          LOAD_Z_USE_MOTOR[6] || LOAD_Z_USE_MOTOR[7] || LOAD_Z_USE_MOTOR[8]);

    if(tsLoader->TabVisible)                                                    //Steven 20200724 : 修改Tray Z馬達速度設定
    {
        pnlLoaderY->Visible=LoaderUnload_StepMotor;
        pnlpnlLoaderZ->Visible=LOAD_Z_USE_MOTOR[0];
        pnEmptyY->Visible =LoaderUnload_StepMotor;
        pnEmptyZ->Visible =(LOAD_Z_USE_MOTOR[1] && AUTO_EMPTY_COLOR!=0);
        pnlColorY->Visible=LoaderUnload_StepMotor;
        pnlColorZ->Visible=(LOAD_Z_USE_MOTOR[2] && AUTO_EMPTY_COLOR!=0);
        pnlAuto1Y->Visible=LoaderUnload_StepMotor;
        pnlAuto1Z->Visible=LOAD_Z_USE_MOTOR[3];
        pnlAuto2Y->Visible=LoaderUnload_StepMotor;
        pnlAuto2Z->Visible=LOAD_Z_USE_MOTOR[4];
        pnlAuto3Y->Visible=LoaderUnload_StepMotor;
        pnlAuto3Z->Visible=LOAD_Z_USE_MOTOR[5];

        grpAuto4->Visible =((LoaderUnload_StepMotor && AUTO_EMPTY_COLOR>=3) || LOAD_Z_USE_MOTOR[6]);
        pnlAuto4Y->Visible= (LoaderUnload_StepMotor && AUTO_EMPTY_COLOR>=3);    //Steven 20230907 : For HT-9011UC
        pnlAuto4Z->Visible=LOAD_Z_USE_MOTOR[6];
        grpAuto5->Visible =((LoaderUnload_StepMotor && AUTO_EMPTY_COLOR>=3) || LOAD_Z_USE_MOTOR[7]);
        pnlAuto5Y->Visible=(LoaderUnload_StepMotor && AUTO_EMPTY_COLOR>=3);
        pnlAuto5Z->Visible=LOAD_Z_USE_MOTOR[7];
        grpAuto6->Visible =((LoaderUnload_StepMotor && AUTO_EMPTY_COLOR>=4) || LOAD_Z_USE_MOTOR[8]);
        pnlAuto6Y->Visible=(LoaderUnload_StepMotor && AUTO_EMPTY_COLOR>=4);
        pnlAuto6Z->Visible=LOAD_Z_USE_MOTOR[8];

        if(IniConfig.bSPILFunction)                                             //JerryYang 20230214 : SPIL佶融 要求鎖定75%不能改
        {
            pnlLoaderY->Enabled=false;
            pnEmptyY->Enabled=false;
            pnlColorY->Enabled=false;
            pnlAuto1Y->Enabled=false;
            pnlAuto2Y->Enabled=false;
            pnlAuto3Y->Enabled=false;
            pnlAuto4Y->Enabled=false;
            pnlAuto5Y->Enabled=false;
            pnlAuto6Y->Enabled=false;
        }
    }

    grpEmpty->Visible=(AUTO_EMPTY_COLOR!=0);
    grpColor->Visible=(AUTO_EMPTY_COLOR!=0);
    //<==
    //Steven 20200529 : Loader入Tray改步進

    lblTrayStepSpeedByMachine->Visible=(LoaderUnload_StepMotor &&
                                        IniConfig.bP40TrayYSpeedByMachine);     //Sam 20201221 : Tray y step motor by machine
    #ifdef DEBUG_INDEX_UPH                                                      //Steven 20170818 : 測試UPH用
        grpIndexUPHTryRun->Visible=true;
    #else
        grpIndexUPHTryRun->Visible=false;
    #endif

    edtLoaderSpeed2->Visible=(CUSTOMER_CODE==CC_ASE_KaohSiung);                 //kevin 20201225 2段速設定
    gbInArmDieClean->Visible=(USE_DIE_CLEAN==1);                                //wei 20170418
    gbOutArmDieClean->Visible=false;                                            //wei 20170418
    if(CosFunction.bOutArmPickUpErrBySetup)                                     //JerryYang 20220923 : E50改成選項
    {
        rgOutArmPickupErrMode->Visible=true;
        if(AccessLevel==iDefHonPrecLevel)
        {
            rgOutArmPickupErrMode->Enabled=true;
        }
        else
        {
            rgOutArmPickupErrMode->Enabled=false;
        }
    }
    else
    {
        rgOutArmPickupErrMode->Visible=false;
    }
    if(CosFunction.bLotStartLockCriticalPara && RunInfo.bLotStart)              //JerryYang 20220311 : ATP鎖定Critical parameter
    {
        if(bAuthCriticalPara[6])
        {
            edIndexVacumCheckTime->Enabled=false;
            edIndexAirOnTime->Enabled=false;
        }

        if(bAuthCriticalPara[11])
        {
            edIndexSpeed->Enabled=false;
        }
    }

    if(CUSTOMER_CODE==CC_QUALCOMM)
    {
        gbEnableDelayTimeZero->Visible=false;
    }

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::ReadFile()
{
    AnsiString szDir="";
    LastFileName=GetLastOpenFN();
    if(CosFunction.bSaveArmSpeedByMachine && IniConfig.bA57_1SaveArmSpeedByMachine)//JimmyChiu 20220618 : save by machine
    {
        szDir=sSaveByMachine;
    }
    else
    {
        szDir.sprintf("%s%s", DataPath, LastFileName);
    }
    MyForceDirectories(szDir);
    szDir+="\\ArmCondition.Data";
    tbAllSpeed->Position                    =ReadIniData(szDir, "All", "Speed", 1);
    tbAccSpeed->Position                    =ReadIniData(szDir, "All", "Accel", 1);
    edAllSpeed->Text                        =tbAllSpeed->Position;
    edAllAccSpeed->Text                     =tbAccSpeed->Position;

    if(USE_CKD_FCM_CleanAir)                                                    //Ifor 20150710 : EP Speed Control
    {
        gbEPControl->Visible                =true;                              //Ifor 20150710 :安裝CKD 才顯示
        tbEPControl->Position               =ReadIniData(szDir, "All", "EPControl", 1);
        edEPControl->Text                   =tbEPControl->Position;
        iEPControlValue                     =atoi(edEPControl->Text.c_str());
    }
    else
    {
        gbEPControl->Visible                =false;                             //Ifor 20150710 :安裝CKD 才顯示
    }
    //--------------------------------------------------------------------------
    //Index Arm
    //--------------------------------------------------------------------------
    ArmSpeed_File[IndexArm].iBodySP         =ReadIniData(szDir, "Index Arm", "X Speed",                  1);
    ArmSpeed_File[IndexArm].iACDCBodySP     =100;//ReadIniData(szDir, "Index Arm", "X Accel",                100);
    if(CosFunction.bLimitMaxSpeed==true)                                        //Steven 20131101 : 限制最高速度
    {
        ArmSpeed_File[IndexArm].iBodySP     =CheckRange(ArmSpeed_File[IndexArm].iBodySP, CosFunction.iLimitMaxSpeed, 1);
    }
    if(CUSTOMER_CODE==CC_ASE_SG)                                                //Ifor 20240913 ASE 新加坡客戶要求 X => Z
    {
        ArmSpeed_File[IndexArm].iBodySP     =CheckAndReadIniData(szDir, "Index Arm", "Y Speed",   ArmSpeed_File[IndexArm].iBodySP);
        ArmSpeed_File[IndexArm].iACDCBodySP =CheckAndReadIniData(szDir, "Index Arm", "Y Accel",   ArmSpeed_File[IndexArm].iACDCBodySP);
    }
    ArmSpeed_File[IndexArm].iRetryCT        =ReadIniData(szDir, "Index Arm", "Retry Count",              0);
    ArmSpeed_File[IndexArm].iRetryCT        =0;
    ArmSpeed_File[IndexArm].dRetryDown      =ReadIniData(szDir, "Index Arm", "Retry Down",             0.0);
    ArmSpeed_File[IndexArm].dVacuumTI       =ReadIniData(szDir, "Index Arm", "Vacuum Check Time",      1.0);
    ArmSpeed_File[IndexArm].dCTAirOn        =ReadIniData(szDir, "Index Arm", "Counter Air ON Time",    1.0);
    //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    ArmSpeed_File[IndexArm].dDestroyAgainTime=ReadIniData(szDir, "Index Arm", "Destroy Again Time", 0.0);
    ArmSpeed_File[IndexArm].iDestroyAgainCount=ReadIniData(szDir, "Index Arm", "Destroy Again Count", 1);

    if(IniConfig.bD32_35TrayPitchIndexDOnMoreThen500MS)                         //Steven 20120727 : Tray Pitch 大於35mm的話, Index吹氣至少要0.5秒
    {
        if(UserDefForm_File[TrayForm.Loader.iTrayType].XDivision==1 ||          //Steven 20241205 : 修正只有一排的會失效
           UserDefForm_File[TrayForm.Loader.iTrayType].XPitch*100>=3500)        //JerryYang 20181228 fix E41功能失效   //Steven 20140221
        {
            ArmSpeed_File[IndexArm].dCTAirOn=CheckRange(ArmSpeed_File[IndexArm].dCTAirOn, 0.5, 10.0);
        }
    }

    ArmSpeed_File[IndexArm].bIndexFloatCHK  =ReadIniData(szDir, "Index Arm", "Socket Check",             0);
    ArmSpeed_File[IndexArm].bSuckOnDown     =ReadIniData(szDir, "Index Arm", "Vacuum Timing",            0);

    if(CUSTOMER_CODE==CC_SINOICTECH || CUSTOMER_CODE==CC_FMSH)                  //Steven 20221207 : 華嶺預設是打開
        TestIF_File.bIndexPickICWhenOutShtNoIC  =ReadIniData(szDir, "Index Arm", "Pick IC when out shuttle no IC", 1);          //Steven 20221207 : Index必須在out shuttle沒料才可以吸
    else
        TestIF_File.bIndexPickICWhenOutShtNoIC  =ReadIniData(szDir, "Index Arm", "Pick IC when out shuttle no IC", 0);

    if(CUSTOMER_CODE==CC_QUALCOMM)                                              //JerryYang 20250826 : QCT不使用
    {
        TestIF_File.bEnableDelayTimeZero=false;
    }
    else
    {
        TestIF_File.bEnableDelayTimeZero=(ReadIniData(szDir, "Index Arm", "Enable Delay Time Zero", 0)==1)?true:false;          //Jimmychiu 20230922 : R230824-ATK-H9-01 Request add “Air purge during place device on out-shuttle” function.
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        ArmSpeed_File[IndexArm].bDevicConfirm=ReadIniData(szDir, "Index Arm", "DevicConfirm", true);                        //kevin 20190629 add index 4 site conform
    else
        ArmSpeed_File[IndexArm].bDevicConfirm=false;

    if(CUSTOMER_CODE==CC_ASE_KaohSiung && IniConfig.bD44CheckIndexICDestroy && ArmSpeed_File[IndexArm].bDevicConfirm)       //kevin 20190629 add 高雄強制4個 SITE INDEX check
    {
        IniConfig.bD44CheckIndexICDestroy=false;
    }
    //Isaac 20180301 (Steven) Index Cycle Time Monitoring function，SCK功能開放
    //==>
    if(CUSTOMER_CODE==CC_SCK || IniConfig.bSPILFunction)                        //JerryYang 20220824 : SPIL index cycle time monitor
    {
        if(IniConfig.bSPILFunction)
        {
            TestIF_File.bIndexCycleTimeMonitor  =ReadIniData(szDir, "Index Arm", "EnableNewIndexCycleTimeMonitoring", false);   //JerryYang 20230131 : 預設先不開啟
        }
        else
        {
            TestIF_File.bIndexCycleTimeMonitor  =ReadIniData(szDir, "Index Arm", "EnableIndexCycleTimeMonitoring", true);
        }

        TestIF_File.dIndexCycletimeMonitor      =ReadIniData(szDir, "Index Arm", "Monitoring_IndexCycletime", 0.0);
        TestIF_File.dMonitorOutlier             =ReadIniData(szDir, "Index Arm", "Monitoring_Outlier", 0.0);
        TestIF_File.iMonitorWindow              =ReadIniData(szDir, "Index Arm", "Monitoring_Window", 0);
        TestIF_File.dICTTolerance               =ReadIniData(szDir, "Index Arm", "IndexCycleTimetolerance", 15);
        TestIF_File.iICTAction                  =ReadIniData(szDir, "Index Arm", "IndexCycleTimeAction", 0);        //Steven 20250521 : 可以選擇要不要停機
    }
    else
    {
        TestIF_File.bIndexCycleTimeMonitor      =false;
    }
    //<==
    //Isaac 20180301 (Steven) Index Cycle Time Monitoring function，SCK功能開放
    if(CosFunction.bOutArmPickUpErrBySetup==true)                               //JerryYang 20220923 : E50改成選項
    {
        IniConfig.iE50_OutArmPickUpErrorOption=ReadIniData(szDir, "Output Arm", "iE50_OutArmPickUpErrorOption", 0);
    }

    //--------------------------------------------------------------------------
    //InPut Arm
    //--------------------------------------------------------------------------
    ArmSpeed_File[InArm].iBodySP            =ReadIniData(szDir, "Input Arm", "XY Speed",                 1);
    if(IniConfig.bE41_35TrayPitchInOutSpeedSmall80Percent)                      //Steven 20120727 : Tray Pitch 大於35mm的話, In Out Arm需要小於80%
    {
        if(UserDefForm_File[TrayForm.Loader.iTrayType].XDivision==1 ||          //Steven 20241205 : 修正只有一排的會失效
           UserDefForm_File[TrayForm.Loader.iTrayType].XPitch*100>=3500)        //JerryYang 20181228 fix E41功能失效    //Steven 20140221
        {
            ArmSpeed_File[InArm].iBodySP    =CheckRange(ArmSpeed_File[InArm].iBodySP, 80, 1);
        }
    }
    if(CosFunction.bLimitMaxSpeed==true)                                        //Steven 20131101 : 限制最高速度
    {
        ArmSpeed_File[InArm].iBodySP        =CheckRange(ArmSpeed_File[InArm].iBodySP, CosFunction.iLimitMaxSpeed, 1);
    }

    AutoArmSpeed[InArm].iBodySP             =ArmSpeed_File[InArm].iBodySP;      //jou 2012-05-16 Auto Speed
    ArmSpeed_File[InArm].iACDCBodySP        =ReadIniData(szDir, "Input Arm", "XY Accel",                 1);
    AutoArmSpeed[InArm].iACDCBodySP         =ArmSpeed_File[InArm].iACDCBodySP;   //jou 2012-05-16 Auto Speed
    ArmSpeed_File[InArm].iZSP               =ReadIniData(szDir, "Input Arm", "Z Up Speed",               1);
    ArmSpeed_File[InArm].iACDCZSP           =ReadIniData(szDir, "Input Arm", "Z Up Accel",               1);
    ArmSpeed_File[InArm].bAutoSpeed         =ReadIniData(szDir, "Input Arm", "Auto Speed Down",          0);
    ArmSpeed_File[InArm].iAutoSpeedLow      =ReadIniData(szDir, "Input Arm", "AutoSpeedLow",            50); //kevin 20210913 AutoSpeed 最低速
    if(CosFunction.bEnableAutoSpeed==false)                                     //Steven 20110526
        ArmSpeed_File[InArm].bAutoSpeed     =false;
    ArmSpeed_File[InArm].iRetryCT           =ReadIniData(szDir, "Input Arm", "Retry Count",              1);
    ArmSpeed_File[InArm].dRetryDown         =ReadIniData(szDir, "Input Arm", "Retry Down",             1.0);
    ArmSpeed_File[InArm].dVacuumTI          =ReadIniData(szDir, "Input Arm", "Vacuum Check Time",      1.0);
    ArmSpeed_File[InArm].dHPVacuumTI        =ReadIniData(szDir, "Input Arm", "HP Vacuum Check Time",   1.0);        //Steven 20180125 (Jou) : 加熱盤的真空等待時間
    ArmSpeed_File[InArm].bUseHPVacuum       =ReadIniData(szDir, "Input Arm", "Use HP Vacuum Check Time",  false);   //Steven 20180125 (Jou) : 加熱盤的真空等待時間
    if(IniConfig.bE57HPCanUseAnotherVacuumDelay==false)
        ArmSpeed_File[InArm].bUseHPVacuum=false;
    ArmSpeed_File[InArm].dCTAirOn           =ReadIniData(szDir, "Input Arm", "Counter Air ON Time",    1.0);
    ArmSpeed_File[InArm].dWaitOnSH          =ReadIniData(szDir, "Input Arm", "Shuttle Wait Time",      1.0);
    ArmSpeed_File[InArm].bAutoSKIP          =ReadIniData(szDir, "Input Arm", "Auto Skip",                0);
    ArmSpeed_File[InArm].iAutoSkipCT        =ReadIniData(szDir, "Input Arm", "Auto Skip CT",            20);
    ArmSpeed_File[InArm].bVariModeFIX       =ReadIniData(szDir, "Input Arm", "One by one",               0);

    if(IniConfig.bSPILFunction==true && ArmSpeed_File[InArm].bAutoSKIP)         //wei 20150317 新竹矽品 auto skip 強制手動拿Tray  //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
    {
        TrayForm.iManualRemoveLoader=1;
    }

    if(TestIF_File.iTestMode==QualSite1X4 ||
       TestIF_File.iTestMode>=_8Site2X4)                                        //Steven 20140212 : For Xilinx 31x31生產8Site
    {
        if(DeviceForm.XDimension>=ArmMaxPitch_13Suck &&
           DeviceForm.XDimension<ArmMaxPitch)                                   //Steven 20140212 : IC大於25mm時,一次吸四顆只能使用Fix Mode
        {
            ArmSpeed_File[InArm].bVariModeFIX=true;
        }

        TestIF_File.iInArmToShtReleaseMode  =ReadIniData(szDir, "Input Arm", "iInArmToShtReleaseMode", 0);  //Steven 20250630 : 力旺要照順序擺
    }
    else
    {
        TestIF_File.iInArmToShtReleaseMode  =0;                                 //Steven 20250630 : 力旺要照順序擺
    }

    if(IniConfig.bI37_EnableFIFOMode && LastSet.iRunStartMode==rsmFIFOMode)     //Steven 20160303 (wei) : FIFO Mode
    {
        ArmSpeed_File[InArm].bVariModeFIX=true;
    }

    ArmSpeed_File[InArm].iTwoSpeedMove     =ReadIniData(szDir, "Input Arm", "Two Speed On Off",    0);     //Steven 20140217 : 兩段速移動
    ArmSpeed_File[InArm].dTwoSpeedDistance =ReadIniData(szDir, "Input Arm", "Two Speed Distance",  3.0);   //Steven 20140217 : 兩段速移動
    ArmSpeed_File[InArm].iTwoSpeed         =ReadIniData(szDir, "Input Arm", "Two Speed Precent",   10);    //Steven 20140217 : 兩段速移動
    ArmSpeed_File[InArm].iTwoADC           =ReadIniData(szDir, "Input Arm", "Two Speed ADC",       10);    //Steven 20140217 : 兩段速移動
    ArmSpeed_File[InArm].bTwoSpeedOnlyLoader=ReadIniData(szDir, "Input Arm", "Two Speed Only Loader", false);  //JerryYang 20190729 二段速功能可選擇only at loader

    ArmSpeed_File[InArm].bSuckOnDown       =ReadIniData(szDir, "Input Arm", "Vacuum Timing",            0);
    ArmSpeed_File[InArm].iVariSP           =ReadIniData(szDir, "Input Arm", "Open/Close Speed",         1);
    ArmSpeed_File[InArm].iACDCVariSP       =ReadIniData(szDir, "Input Arm", "Open/Close Accel",         1);

    //ChungHung 20131231 alter AutoYPitch start
    ArmSpeed_File[InArm].iVariSPY          =ArmSpeed_File[InArm].iVariSP;       //Steven 20140930 : For XY-Pitch
    ArmSpeed_File[InArm].iACDCVariSPY      =ArmSpeed_File[InArm].iACDCVariSP;
    //ChungHung 20131231 alter AutoYPitch end

    //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    ArmSpeed_File[InArm].dDestroyAgainTime  =ReadIniData(szDir, "Input Arm", "Destroy Again Time", 0.0);
    ArmSpeed_File[InArm].iDestroyAgainCount =ReadIniData(szDir, "Input Arm", "Destroy Again Count", 1);

    //2013-08-01    Dell    modify  公司統一回吸功能
    ArmSpeed_File[InArm].dDestroyCheckTime  =ReadIniData(szDir, "Input Arm", "Destroy Check Time", 0.1);
    ArmSpeed_File[InArm].bDestroyPauseCheck =ReadIniData(szDir, "Input Arm", "Destroy Pause Check", 0);

    ArmSpeed_File[InRotate].iBodySP         =ReadIniData(szDir, "Input Arm", "Rotate Input Speed", 0.1);     //kevin 20130517 rotate add
    ArmSpeed_File[InRotate].iACDCBodySP     =ReadIniData(szDir, "Input Arm", "Rotate Input Accel", 0);

    ArmSpeed_File[InArm].dCylinderDelay     =ReadIniData(szDir, "Input Arm", "Cylinder Delay", 0.1);
    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           //JerryYang 20170406 (Steven) 只有TSMC才使用input arm release IC前delay
    {
        ArmSpeed_File[InArm].dReleaseDelayTime  =ReadIniData(szDir, "Input Arm", "Release Delay Time", 0.0);     //JerryYang 20160127 for TSMC inarm release device前delay
        ArmSpeed_File[InArm].iEnableReleaseDelay=ReadIniData(szDir, "Input Arm", "Enable Relase Delay", 1);      //JerryYang 20160127 for TSMC inarm release device前delay
    }
    else
    {
        ArmSpeed_File[InArm].dReleaseDelayTime=0.0;                             //JerryYang 20160127 for TSMC inarm release device前delay
        ArmSpeed_File[InArm].iEnableReleaseDelay=1;                             //JerryYang 20160127 for TSMC inarm release device前delay
    }

    if(USE_DIE_CLEAN)
    {
        ArmSpeed_File[InArm].iEnableDieClean    =ReadIniData(szDir, "Input Arm", "Enable Die Clean",  0);        //wei 20170418
        ArmSpeed_File[InArm].dDieCleanDelay     =ReadIniData(szDir, "Input Arm", "Die Clean Delay",   0.0);      //wei 20170418
        ArmSpeed_File[InArm].dDieCleanHeight    =ReadIniData(szDir, "Input Arm", "Die Clean Height",  0.1);      //wei 20170418
    }
    else
    {
        ArmSpeed_File[InArm].iEnableDieClean    =0;
    }

    ArmSpeed_File[InArm].bHeightCheck  =ReadIniData(szDir, "Input Arm", "Enable Height Check", false);          //wei 20160825 回吸偵測
    ArmSpeed_File[InArm].iHeightCheck  =ReadIniData(szDir, "Input Arm", "Height Check",         1000);          //wei 20160825 回吸偵測
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        ArmSpeed_File[InArm].bDevicConfirm=ReadIniData(szDir, "Input Arm", "DevicConfirm", true);                 //kevin 20170510 (wei) 回吸偵測開關
    else
        ArmSpeed_File[InArm].bDevicConfirm=false;

    if(CosFunction.bYPitchNotUseSearchLastMode && USE_OUT_Y_IS_AUTO_PITCH==true)       //JerryYang 20181112 (Steven) : ATP要求修改unloader擺放方式，盡量不要有空格  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20200324 中科志強說預設要false
        {
            ArmSpeed_File[InArm].bYPitchUseSearchLastMode=ReadIniData(szDir, "Input Arm", "bYPitchNotUseSearchLastMode", false);
        }
        else
        {
            ArmSpeed_File[InArm].bYPitchUseSearchLastMode=ReadIniData(szDir, "Input Arm", "bYPitchNotUseSearchLastMode", true);
        }
    }
    else
    {
        ArmSpeed_File[InArm].bYPitchUseSearchLastMode=true;
    }

    ArmSpeed_File[InArm].bTT_SetSpeed         =ReadIniData(szDir, "Input Arm", "Test Time Set Speed",          0);//Ztex 2024.08.11 Add Test Time Set Speed
    if(ArmSpeed_File[InArm].bTT_SetSpeed==1)                                                                      //Ztex 2024.08.11 Add Test Time Set Speed
        ArmSpeed_File[InArm].bAutoSpeed=0;
    //--------------------------------------------------------------------------
    //Output Arm
    //--------------------------------------------------------------------------
    ArmSpeed_File[OutArm].iBodySP           =ReadIniData(szDir, "Output Arm", "XY Speed",                1);
    if(IniConfig.bE41_35TrayPitchInOutSpeedSmall80Percent)                      //Steven 20120727 : Tray Pitch 大於35mm的話, In Out Arm需要小於80%
    {
        if(UserDefForm_File[TrayForm.Loader.iTrayType].XDivision==1 ||          //Steven 20241205 : 修正只有一排的會失效
           UserDefForm_File[TrayForm.Loader.iTrayType].XPitch*100>=3500)        //JerryYang 20181228 fix E41功能失效
        {
            ArmSpeed_File[OutArm].iBodySP   =CheckRange(ArmSpeed_File[OutArm].iBodySP, 80, 1);
        }
    }

    if(CosFunction.bLimitMaxSpeed==true)                                        //Steven 20131101 : 限制最高速度
    {
        ArmSpeed_File[OutArm].iBodySP       =CheckRange(ArmSpeed_File[OutArm].iBodySP, CosFunction.iLimitMaxSpeed, 1);
    }

    AutoArmSpeed[OutArm].iBodySP            =ArmSpeed_File[OutArm].iBodySP;     //jou 2012-05-16 Auto Speed
    ArmSpeed_File[OutArm].iACDCBodySP       =ReadIniData(szDir, "Output Arm", "XY Accel",                1);
    AutoArmSpeed[OutArm].iACDCBodySP        =ArmSpeed_File[OutArm].iACDCBodySP;   //jou 2012-05-16 Auto Speed
    ArmSpeed_File[OutArm].iZSP              =ReadIniData(szDir, "Output Arm", "Z Up Speed",              1);
    ArmSpeed_File[OutArm].iACDCZSP          =ReadIniData(szDir, "Output Arm", "Z Up Accel",              1);
    ArmSpeed_File[OutArm].iRetryCT          =ReadIniData(szDir, "Output Arm", "Retry Count",             1);
    ArmSpeed_File[OutArm].dRetryDown        =ReadIniData(szDir, "Output Arm", "Retry Down",              1.0);
    ArmSpeed_File[OutArm].dVacuumTI         =ReadIniData(szDir, "Output Arm", "Vacuum Check Time",       1.0);
    ArmSpeed_File[OutArm].dCTAirOn          =ReadIniData(szDir, "Output Arm", "Counter Air ON Time",     1.0);
    ArmSpeed_File[OutArm].dWaitOnSH         =ReadIniData(szDir, "Output Arm", "Shuttle Wait Time",       0.0);    //kevin 20210525 out arm 在 shuttlr 上等待時間到再往下吸取
    ArmSpeed_File[OutArm].bVariModeFIX      =ReadIniData(szDir, "Output Arm", "One by one",              0);

    ArmSpeed_File[OutArm].iAutoSpeedLow     =ReadIniData(szDir, "Input Arm", "AutoSpeedLow",            50); //kevin 20210913 AutoSpeed 最低速

    if(TestIF_File.iTestMode==QualSite1X4 ||
       TestIF_File.iTestMode>=_8Site2X4)                                        //Steven 20140212 : For Xilinx 31x31生產8Site
    {
        if(DeviceForm.XDimension>=ArmMaxPitch_13Suck &&
           DeviceForm.XDimension<ArmMaxPitch)                                   //Steven 20140212 : IC大於25mm時,一次吸四顆只能使用Fix Mode
        {
            ArmSpeed_File[OutArm].bVariModeFIX=true;
        }
    }

    if(IniConfig.bI37_EnableFIFOMode && LastSet.iRunStartMode==rsmFIFOMode)     //Steven 20160303 (wei) : FIFO Mode
    {
        ArmSpeed_File[OutArm].bVariModeFIX=true;
    }

    ArmSpeed_File[OutArm].bSuckOnDown       =ReadIniData(szDir, "Output Arm", "Vacuum Timing",           0);
    ArmSpeed_File[OutArm].iVariSP           =ReadIniData(szDir, "Output Arm", "Open/Close Speed",        1);
    ArmSpeed_File[OutArm].iACDCVariSP       =ReadIniData(szDir, "Output Arm", "Open/Close Accel",        1);
    ArmSpeed_File[OutArm].dDestroyPauseTime =ReadIniData(szDir, "Output Arm", "Wait before Air On",      0.01);
    ArmSpeed_File[OutArm].iTwoSpeedMove     =ReadIniData(szDir, "Output Arm", "Two Speed On Off",        0);     //Steven 20140217 : 兩段速移動
    ArmSpeed_File[OutArm].dTwoSpeedDistance =ReadIniData(szDir, "Output Arm", "Two Speed Distance",      3.0);   //Steven 20140217 : 兩段速移動
    ArmSpeed_File[OutArm].iTwoSpeed         =ReadIniData(szDir, "Output Arm", "Two Speed Precent",       10);    //Steven 20140217 : 兩段速移動
    ArmSpeed_File[OutArm].iTwoADC           =ReadIniData(szDir, "Output Arm", "Two Speed ADC",           10);    //Steven 20140217 : 兩段速移動

    //ChungHung 20131231 alter AutoYPitch start
    ArmSpeed_File[OutArm].iVariSPY          =ArmSpeed_File[OutArm].iVariSP;      //Steven 20140930 : For XY-Pitch
    ArmSpeed_File[OutArm].iACDCVariSPY      =ArmSpeed_File[OutArm].iACDCVariSP;
    //ChungHung 20131231 alter AutoYPitch end

    //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    ArmSpeed_File[OutArm].dDestroyAgainTime =ReadIniData(szDir, "Output Arm", "Destroy Again Time", 0.0);
    ArmSpeed_File[OutArm].iDestroyAgainCount=ReadIniData(szDir, "Output Arm", "Destroy Again Count", 1);

    //2013-08-01    Dell    modify  公司統一回吸功能
    ArmSpeed_File[OutArm].dDestroyCheckTime =ReadIniData(szDir, "Output Arm", "Destroy Check Time", 0.1);
    ArmSpeed_File[OutArm].bDestroyPauseCheck=ReadIniData(szDir, "Output Arm", "Destroy Pause Check", 0);

    ArmSpeed_File[OutArm].dCylinderDelay    =ReadIniData(szDir, "Output Arm", "Cylinder Delay", 0.1);
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        ArmSpeed_File[OutArm].bDevicConfirm =ReadIniData(szDir, "Output Arm", "DevicConfirm", true);         //kevin 20170510 (wei) 回吸偵測開關
    else
        ArmSpeed_File[OutArm].bDevicConfirm =false;

    if(CosFunction.bYPitchNotUseSearchLastMode && USE_OUT_Y_IS_AUTO_PITCH==true)       //JerryYang 20181112 (Steven) : ATP要求修改unloader擺放方式，盡量不要有空格  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20200324 中科志強說預設要false
        {
            ArmSpeed_File[OutArm].bYPitchUseSearchLastMode=ReadIniData(szDir, "Output Arm", "bYPitchNotUseSearchLastMode", false);
        }
        else
        {
            ArmSpeed_File[OutArm].bYPitchUseSearchLastMode=ReadIniData(szDir, "Output Arm", "bYPitchNotUseSearchLastMode", true);
        }
    }
    else
    {
        ArmSpeed_File[OutArm].bYPitchUseSearchLastMode=true;
    }

    if(USE_DIE_CLEAN)
    {
        ArmSpeed_File[OutArm].iEnableDieClean   =ReadIniData(szDir, "Output Arm", "Enable Die Clean",  0);       //wei 20170418
        ArmSpeed_File[OutArm].dDieCleanDelay    =ReadIniData(szDir, "Output Arm", "Die Clean Delay",   0.0);     //wei 20170418
    }
    else
    {
        ArmSpeed_File[OutArm].iEnableDieClean   =0;
    }

    ArmSpeed_File[OutRotate].iBodySP        =ReadIniData(szDir, "Output Arm", "Rotate Output Speed", 0.1);   //Steven 20140520 : Input --> Output
    ArmSpeed_File[OutRotate].iACDCBodySP    =ReadIniData(szDir, "Output Arm", "Rotate Output Accel", 0);     //Steven 20140520 : Input --> Output
    //--------------------------------------------------------------------------
    //Empty Tray Arm
    //--------------------------------------------------------------------------
    ArmSpeed_File[TrayArm].iBodySP          =ReadIniData(szDir, "Empty Tray Arm", "Speed",               1);

    if(CosFunction.bLimitMaxSpeed==true)                                        //Steven 20131101 : 限制最高速度
    {
        ArmSpeed_File[TrayArm].iBodySP      =CheckRange(ArmSpeed_File[TrayArm].iBodySP, CosFunction.iLimitMaxSpeed, 1);
    }

    ArmSpeed_File[TrayArm].iACDCBodySP      =ReadIniData(szDir, "Empty Tray Arm", "Accel",               1);
    ArmSpeed_File[TrayArm].iRetryCT         =ReadIniData(szDir, "Empty Tray Arm", "Retry Count",         1);
    ArmSpeed_File[TrayArm].dVacuumTI        =ReadIniData(szDir, "Empty Tray Arm", "Vacuum Check Time",   1.0);
    ArmSpeed_File[TrayArm].dCTAirOn         =ReadIniData(szDir, "Empty Tray Arm", "Counter Air ON Time", 1.0);
    ArmSpeed_File[TrayArm].dTAHeadDownIT    =ReadIniData(szDir, "Empty Tray Arm", "Hand Down Time",      1.0);
    if(IniConfig.bC03UseCatchTray)                                              //use catch Tray Fix
    {
        ArmSpeed_File[TrayArm].dTAHeadDownIT=CheckRange(ArmSpeed_File[TrayArm].dTAHeadDownIT, 0.3, 10.0);
    }
    //--------------------------------------------------------------------------
    //Shuttle
    //--------------------------------------------------------------------------
    SHSpeed_File.iSH1Sp                    =ReadIniData(szDir, "Shuttle", "Shuttle 1 Speed",    1);
    SHSpeed_File.iShakeCycles              =ReadIniData(szDir, "Shuttle", "Shake Cycles",           1);      //Sam 20250326 : 新增 Shake 條件設定
    SHSpeed_File.iShakeDistance            =ReadIniData(szDir, "Shuttle", "Shake distance",         5);
    SHSpeed_File.dShakeDelay               =ReadIniData(szDir, "Shuttle", "Delay between shakes",   0.0);
    SHSpeed_File.iShakeAccDec              =ReadIniData(szDir, "Shuttle", "Shake Accel",            100);

    if(CosFunction.bLimitMaxSpeed==true)                                        //Steven 20131101 : 限制最高速度
    {
        SHSpeed_File.iSH1Sp                 =CheckRange(SHSpeed_File.iSH1Sp, CosFunction.iLimitMaxSpeed, 1);
    }

    SHSpeed_File.iSH1ACDCSp                =ReadIniData(szDir, "Shuttle", "Shuttle 1 Accel",    1);
    SHSpeed_File.iSH2Sp                    =ReadIniData(szDir, "Shuttle", "Shuttle 2 Speed",    1);

    if(CosFunction.bLimitMaxSpeed==true)                                        //Steven 20131101 : 限制最高速度
    {
        SHSpeed_File.iSH2Sp                 =CheckRange(SHSpeed_File.iSH2Sp, CosFunction.iLimitMaxSpeed, 1);
    }

    SHSpeed_File.iSH2ACDCSp                 =ReadIniData(szDir, "Shuttle", "Shuttle 2 Accel",    1);
    SHSpeed_File.iStepSH                    =ReadIniData(szDir, "Shuttle", "Step Shuttle",       0);
    TestIF_File.iShakeShuttleWhenPlaceIC    =ReadIniData(szDir, "Shuttle", "Shake Shuttle",      0);      //Steven 20200616 : ATK要求每次放料都要搖搖蝦頭

    //Steven 20170317 (wei) mark
    //==>
//    if(CUSTOMER_CODE==CC_AMKOR_Japan)                                         //Steven 20170202 : For ATJ Out Arm pickup error put to define tray
//    {
//        TestIF_File.bOutPickErrAct          =ReadIniData(szDir, "Output Arm", "Out Pick Err Action", false);
//        TestIF_File.iOutPickErrAuto         =ReadIniData(szDir, "Output Arm", "Out Pick Err Tray", 6);
//    }
//    else
    //<==
    //Steven 20170317 (wei) mark
    {
        TestIF_File.bOutPickErrAct          =false;
        TestIF_File.iOutPickErrAuto         =6;
    }

    iIndexSpeed=atoi(edtTrySpeed->Text.c_str());                                //Steven 20170818 : 測試UPH用
    iIndexAcc=atoi(edtTryAcc->Text.c_str());

    //--------------------------------------------------------------------------
    //Magazine
    //--------------------------------------------------------------------------
    MGSpeed_File.iCYSp                  =ReadIniData(szDir, "Magazine", "Magazine CatchY Speed",    1);
    MGSpeed_File.iCYACDCSp              =ReadIniData(szDir, "Magazine", "Magazine CatchY Accel",    1);

    MGSpeed_File.iMZSp                  =ReadIniData(szDir, "Magazine", "MagazineZ Speed",    1);
    MGSpeed_File.iMZACDCSp              =ReadIniData(szDir, "Magazine", "MagazineZ Accel",    1);

    MGSpeed_File.dCYDecRange            =ReadIniData(szDir, "Magazine", "Two Speed Distance",  3.0);
    MGSpeed_File.iCY2Sp                 =ReadIniData(szDir, "Magazine", "Two Speed Precent",   10);
    MGSpeed_File.iCY2ACDCSp             =ReadIniData(szDir, "Magazine", "Two Speed ADC",       10);

    ReadWriteFile(true, szDir);
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::ReadWriteFile(bool bRead, AnsiString szDir)
{
    AnsiString szTrayDir="";
    szTrayDir=szDir;
    int idefaultSpeed=0;
    if(IniConfig.bP40TrayYSpeedByMachine)                                       //Sam 20201221 : Tray y step motor by machine
       szTrayDir=asTrayStepSpeedByMachinePatch;

    if(CUSTOMER_CODE==CC_TERAPOWER)                                             //Sam 20210325 : 晶兆成要求預設值為70
        idefaultSpeed=70;
    else
        idefaultSpeed=100;

    if(LoaderUnload_StepMotor)                                                  //Steven 20200529 : Loader入Tray改步進
    {
        if(IniConfig.bSPILFunction)                                             //JerryYang 20230214 : SPIL佶融 要求鎖定75%不能改
        {
            dmTrayMotor->iStepMotorSpeed[0]=75;
            dmTrayMotor->iStepMotorSpeed[1]=75;
            dmTrayMotor->iStepMotorSpeed[2]=75;
            dmTrayMotor->iStepMotorSpeed[3]=75;
            dmTrayMotor->iStepMotorSpeed[0]=75;
            dmTrayMotor->iStepMotorSpeed[6]=75;
            dmTrayMotor->iStepMotorSpeed[7]=75;
            dmTrayMotor->iStepMotorSpeed[8]=75;
        }
        else
        {
            dmTrayMotor->iStepMotorSpeed[0]=ReadWriteIni(szTrayDir, "Tray", "Loader Tray Speed", atoi(edtLoaderSpeed1->Text.c_str()), idefaultSpeed, bRead, true, 1, 100);    //Sam 20201221 : Tray y step motor by machine
            dmTrayMotor->iStepMotorSpeed[1]=ReadWriteIni(szTrayDir, "Tray", "Auto 1 Tray Speed", atoi(edtAuto1Speed1->Text.c_str()),  idefaultSpeed, bRead, true, 1, 100);
            dmTrayMotor->iStepMotorSpeed[2]=ReadWriteIni(szTrayDir, "Tray", "Auto 2 Tray Speed", atoi(edtAuto2Speed1->Text.c_str()),  idefaultSpeed, bRead, true, 1, 100);
            dmTrayMotor->iStepMotorSpeed[3]=ReadWriteIni(szTrayDir, "Tray", "Auto 3 Tray Speed", atoi(edtAuto3Speed1->Text.c_str()),  idefaultSpeed, bRead, true, 1, 100);
            if(AUTO_EMPTY_COLOR>=3)                                             //Steven 20230907 : For HT-9011UC
            {
                dmTrayMotor->iStepMotorSpeed[6]=ReadWriteIni(szTrayDir, "Tray", "Auto 4 Tray Speed", atoi(edtAuto4Speed1->Text.c_str()),  idefaultSpeed, bRead, true, 1, 100);
                dmTrayMotor->iStepMotorSpeed[7]=ReadWriteIni(szTrayDir, "Tray", "Auto 5 Tray Speed", atoi(edtAuto5Speed1->Text.c_str()),  idefaultSpeed, bRead, true, 1, 100);
            }
            if(AUTO_EMPTY_COLOR>=4)                                             //Steven 20230907 : For HT-9011UC
                dmTrayMotor->iStepMotorSpeed[8]=ReadWriteIni(szTrayDir, "Tray", "Auto 6 Tray Speed", atoi(edtAuto6Speed1->Text.c_str()),  idefaultSpeed, bRead, true, 1, 100);
        }

        if(AUTO_EMPTY_COLOR!=0)
        {
            if(IniConfig.bSPILFunction)                                         //JerryYang 20230214 : SPIL佶融 要求鎖定75%不能改
            {
                dmTrayMotor->iStepMotorSpeed[4]=75;
                dmTrayMotor->iStepMotorSpeed[5]=75;
            }
            else
            {
                dmTrayMotor->iStepMotorSpeed[4]=ReadWriteIni(szTrayDir, "Tray", "Empty Tray Speed",      atoi(edtEmptySpeed1->Text.c_str()), idefaultSpeed, bRead, true, 1, 100);
                dmTrayMotor->iStepMotorSpeed[5]=ReadWriteIni(szTrayDir, "Tray", "Color Tray Speed",      atoi(edtColorSpeed1->Text.c_str()), idefaultSpeed, bRead, true, 1, 100);
            }
            dmTrayMotor->iStepMotorSpeed[9]    =ReadWriteIni(szTrayDir, "Tray", "Loader Tray 2nd Speed", atoi(edtLoaderSpeed2->Text.c_str()), 20, bRead, true, 1, 100);
        }

        if(bRead)
        {
            udLoaderSpeed1->Position=dmTrayMotor->iStepMotorSpeed[0];           //Steven 20210625 : 修正軌道速度初始化設定問題
            edtLoaderSpeed1->Text   =dmTrayMotor->iStepMotorSpeed[0];
            udAuto1Speed1->Position =dmTrayMotor->iStepMotorSpeed[1];
            edtAuto1Speed1->Text    =dmTrayMotor->iStepMotorSpeed[1];
            udAuto2Speed1->Position =dmTrayMotor->iStepMotorSpeed[2];
            edtAuto2Speed1->Text    =dmTrayMotor->iStepMotorSpeed[2];
            udAuto3Speed1->Position =dmTrayMotor->iStepMotorSpeed[3];
            edtAuto3Speed1->Text    =dmTrayMotor->iStepMotorSpeed[3];
            udEmptySpeed1->Position =dmTrayMotor->iStepMotorSpeed[4];
            edtEmptySpeed1->Text    =dmTrayMotor->iStepMotorSpeed[4];
            udColorSpeed1->Position =dmTrayMotor->iStepMotorSpeed[5];
            edtColorSpeed1->Text    =dmTrayMotor->iStepMotorSpeed[5];
            udAuto4Speed1->Position =dmTrayMotor->iStepMotorSpeed[6];
            edtAuto4Speed1->Text    =dmTrayMotor->iStepMotorSpeed[6];
            udAuto5Speed1->Position =dmTrayMotor->iStepMotorSpeed[7];
            edtAuto5Speed1->Text    =dmTrayMotor->iStepMotorSpeed[7];
            udAuto6Speed1->Position =dmTrayMotor->iStepMotorSpeed[8];
            edtAuto6Speed1->Text    =dmTrayMotor->iStepMotorSpeed[8];

            edtLoaderSpeed2->Text   =dmTrayMotor->iStepMotorSpeed[9];           //SECOND SPEED   kevin 20201211
        }
        if(CUSTOMER_CODE==CC_ASE_KaohSiung)
            dmTrayMotor->bNeedSetStepMotorSpeed=true;                           //kevin 20210602
    }

    if(LOAD_Z_USE_MOTOR[0])                                                     //Steven 20200724 : 修改Tray Z馬達速度設定
    {
        TestIF_File.iTrayZStepSpeed[0]=ReadWriteIni(szDir, "Tray", "Loader Tray Z Speed", atoi(edtLoaderSpeedZ->Text.c_str()), 50, bRead, true, 1, 100);
        if(bRead)
        {
            udLoaderSpeedZ->Position=TestIF_File.iTrayZStepSpeed[0];
            edtLoaderSpeedZ->Text=TestIF_File.iTrayZStepSpeed[0];
        }
    }
    else
    {
        TestIF_File.iTrayZStepSpeed[0]=50;
    }

    if(AUTO_EMPTY_COLOR!=0 && LOAD_Z_USE_MOTOR[1])
    {
        TestIF_File.iTrayZStepSpeed[1]=ReadWriteIni(szDir, "Tray", "Empty Z Speed", atoi(edtEmptySpeedZ->Text.c_str()), 50, bRead, true, 1, 100);
        if(bRead)
        {
            udEmptySpeedZ->Position=TestIF_File.iTrayZStepSpeed[1];
            edtEmptySpeedZ->Text=TestIF_File.iTrayZStepSpeed[1];
        }
    }
    else
    {
        TestIF_File.iTrayZStepSpeed[1]=50;
    }

    if(AUTO_EMPTY_COLOR!=0 && LOAD_Z_USE_MOTOR[2])
    {
        TestIF_File.iTrayZStepSpeed[2]=ReadWriteIni(szDir, "Tray", "Color Z Speed", atoi(edtColorSpeedZ->Text.c_str()), 50, bRead, true, 1, 100);
        if(bRead)
        {
            udColorSpeedZ->Position=TestIF_File.iTrayZStepSpeed[2];
            edtColorSpeedZ->Text=TestIF_File.iTrayZStepSpeed[2];
        }
    }
    else
    {
        TestIF_File.iTrayZStepSpeed[2]=50;
    }

    if(LOAD_Z_USE_MOTOR[3])
    {
        TestIF_File.iTrayZStepSpeed[3]=ReadWriteIni(szDir, "Tray", "Auto 1 Z Speed", atoi(edtAuto1SpeedZ->Text.c_str()), 50, bRead, true, 1, 100);
        if(bRead)
        {
            udAuto1SpeedZ->Position=TestIF_File.iTrayZStepSpeed[3];
            edtAuto1SpeedZ->Text=TestIF_File.iTrayZStepSpeed[3];
        }
    }
    else
    {
        TestIF_File.iTrayZStepSpeed[3]=50;
    }

    if(LOAD_Z_USE_MOTOR[4])
    {
        TestIF_File.iTrayZStepSpeed[4]=ReadWriteIni(szDir, "Tray", "Auto 2 Z Speed", atoi(edtAuto2SpeedZ->Text.c_str()), 50, bRead, true, 1, 100);
        if(bRead)
        {
            udAuto2SpeedZ->Position=TestIF_File.iTrayZStepSpeed[4];
            edtAuto2SpeedZ->Text=TestIF_File.iTrayZStepSpeed[4];
        }
    }
    else
    {
        TestIF_File.iTrayZStepSpeed[4]=50;
    }

    if(LOAD_Z_USE_MOTOR[5])
    {
        TestIF_File.iTrayZStepSpeed[5]=ReadWriteIni(szDir, "Tray", "Auto 3 Z Speed", atoi(edtAuto3SpeedZ->Text.c_str()), 50, bRead, true, 1, 100);
        if(bRead)
        {
            udAuto3SpeedZ->Position=TestIF_File.iTrayZStepSpeed[5];
            edtAuto3SpeedZ->Text=TestIF_File.iTrayZStepSpeed[5];
        }
    }
    else
    {
        TestIF_File.iTrayZStepSpeed[5]=50;
    }

    if(LOAD_Z_USE_MOTOR[6])                                                     //Steven 20230907 : For HT-9011UC
    {
        TestIF_File.iTrayZStepSpeed[6]=ReadWriteIni(szDir, "Tray", "Auto 4 Z Speed", atoi(edtAuto1SpeedZ->Text.c_str()), 50, bRead, true, 1, 100);
        if(bRead)
        {
            udAuto4SpeedZ->Position=TestIF_File.iTrayZStepSpeed[6];
            edtAuto4SpeedZ->Text=TestIF_File.iTrayZStepSpeed[6];
        }
    }
    else
    {
        TestIF_File.iTrayZStepSpeed[6]=50;
    }

    if(LOAD_Z_USE_MOTOR[7])
    {
        TestIF_File.iTrayZStepSpeed[7]=ReadWriteIni(szDir, "Tray", "Auto 5 Z Speed", atoi(edtAuto2SpeedZ->Text.c_str()), 50, bRead, true, 1, 100);
        if(bRead)
        {
            udAuto5SpeedZ->Position=TestIF_File.iTrayZStepSpeed[7];
            edtAuto5SpeedZ->Text=TestIF_File.iTrayZStepSpeed[7];
        }
    }
    else
    {
        TestIF_File.iTrayZStepSpeed[7]=50;
    }

    if(LOAD_Z_USE_MOTOR[8])
    {
        TestIF_File.iTrayZStepSpeed[8]=ReadWriteIni(szDir, "Tray", "Auto 4 Z Speed", atoi(edtAuto3SpeedZ->Text.c_str()), 50, bRead, true, 1, 100);
        if(bRead)
        {
            udAuto6SpeedZ->Position=TestIF_File.iTrayZStepSpeed[8];
            edtAuto6SpeedZ->Text=TestIF_File.iTrayZStepSpeed[8];
        }
    }
    else
    {
        TestIF_File.iTrayZStepSpeed[8]=50;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::DoIniDataToForm()
{
    //--------------------------------------------------------------------------
    //Index Arm
    //--------------------------------------------------------------------------
    udIndexSpd->Position            =ArmSpeed_File[IndexArm].iBodySP;
    edIndexSpeed->Text              =udIndexSpd->Position;
    udIndexAcc->Position            =ArmSpeed_File[IndexArm].iACDCBodySP;
    edIndexAccDec->Text             =udIndexAcc->Position;
    edIndexRetryCount->Text         =ArmSpeed_File[IndexArm].iRetryCT;
    edIndexArmRetryMM->Text         =FormatFloat("0.00", ArmSpeed_File[IndexArm].dRetryDown);
    edIndexVacumCheckTime->Text     =FormatFloat("0.00", ArmSpeed_File[IndexArm].dVacuumTI);
    edIndexAirOnTime->Text          =FormatFloat("0.00", ArmSpeed_File[IndexArm].dCTAirOn);
    rgSocketCheck->ItemIndex        =ArmSpeed_File[IndexArm].bIndexFloatCHK;
    chkIndexPreSuck->Checked        =!ArmSpeed_File[IndexArm].bSuckOnDown;
    chkPickAfterOutArm->Checked     =TestIF_File.bIndexPickICWhenOutShtNoIC;    //Steven 20221207 : Index必須在out shuttle沒料才可以吸
    cbEnableDelayTimeZero->Checked  =TestIF_File.bEnableDelayTimeZero;          //Jimmychiu 20230922 : R230824-ATK-H9-01 Request add “Air purge during place device on out-shuttle” function.
    //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    edIndexDestroyAgainTime->Text   =FormatFloat("0.00", ArmSpeed_File[IndexArm].dDestroyAgainTime);
    edIndexDestroyAgainCount->Text  =ArmSpeed_File[IndexArm].iDestroyAgainCount;

    //Isaac 20180301 (Steven) Index Cycle Time Monitoring function
    //==>
    chkEnableIndexCycleTimeMonitoring->Checked      =TestIF_File.bIndexCycleTimeMonitor;
    edtMonitoringIndexCycletime->Text               =TestIF_File.dIndexCycletimeMonitor;
    edtMonitoringOutlier->Text                      =TestIF_File.dMonitorOutlier;
    edtMonitoringWindow->Text                       =TestIF_File.iMonitorWindow;

    edtIndexCycleTimetolerance->Text                =TestIF_File.dICTTolerance; //JerryYang 20220824 : SPIL index cycle time monitor
    rgIndexCycleTime->ItemIndex                     =TestIF_File.iICTAction;    //Steven 20250521 : 可以選擇要不要停機

    gbIndexeviveConfirm->ItemIndex =(ArmSpeed_File[IndexArm].bDevicConfirm==1)?0:1;              //kevin 20190629 add index 4 site conform
    //<==
    //Isaac 20180301 (Steven) Index Cycle Time Monitoring function
    if(CosFunction.bOutArmPickUpErrBySetup)                                     //JerryYang 20220923 : E50改成選項
    {
        rgOutArmPickupErrMode->ItemIndex=IniConfig.iE50_OutArmPickUpErrorOption;
    }

    //--------------------------------------------------------------------------
    //Input Arm
    //--------------------------------------------------------------------------
    udInXSpd->Position              =ArmSpeed_File[InArm].iBodySP;
    edInXYSpd->Text                 =udInXSpd->Position;
    udInXAcc->Position              =ArmSpeed_File[InArm].iACDCBodySP;
    edInXYAcc->Text                 =udInXAcc->Position;
    udInZSpd->Position              =ArmSpeed_File[InArm].iZSP;
    edInZSpd->Text                  =udInZSpd->Position;
    udInZAcc->Position              =ArmSpeed_File[InArm].iACDCZSP;
    edInZAcc->Text                  =udInZAcc->Position;

    //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    edInArmDestroyAgainTime->Text   =FormatFloat("0.00", ArmSpeed_File[InArm].dDestroyAgainTime);
    edInArmDestroyAgainCount->Text  =ArmSpeed_File[InArm].iDestroyAgainCount;
    rgAutoSpeed->ItemIndex          =(ArmSpeed_File[InArm].bAutoSpeed==false)?1:0;
    edAutoSpeedLow->Text            = ArmSpeed_File[InArm].iAutoSpeedLow;       //kevin 20210913 AutoSpeed 最低速

    edInArmRetryCount->Text         =ArmSpeed_File[InArm].iRetryCT;
    edInArmRetryMM->Text            =FormatFloat("0.00", ArmSpeed_File[InArm].dRetryDown);
    edInVacumCheckTime->Text        =FormatFloat("0.00", ArmSpeed_File[InArm].dVacuumTI);
    edtHPVacuumDelay->Text          =FormatFloat("0.00", ArmSpeed_File[InArm].dHPVacuumTI);         //Steven 20180125 (Jou) : 加熱盤的真空等待時間
    chkUseHPVacuumDelay->Checked    =ArmSpeed_File[InArm].bUseHPVacuum;                             //Steven 20180125 (Jou) : 加熱盤的真空等待時間
    edInArmAirOnTime->Text          =FormatFloat("0.00", ArmSpeed_File[InArm].dCTAirOn);
    edInArmShtWaitTime->Text        =FormatFloat("0.00", ArmSpeed_File[InArm].dWaitOnSH);
    rgTwoSpeedOnOff_In->ItemIndex   =ArmSpeed_File[InArm].iTwoSpeedMove;                            //Steven 20140217 : 兩段速移動
    edTwoSpeedDistance_In->Text     =FormatFloat("0.00", ArmSpeed_File[InArm].dTwoSpeedDistance);   //Steven 20140217 : 兩段速移動
    edSecondSpeedIn->Text           =AnsiString(ArmSpeed_File[InArm].iTwoSpeed);                    //Steven 20140217 : 兩段速移動
    edSecondADCIn->Text             =AnsiString(ArmSpeed_File[InArm].iTwoADC);                      //Steven 20140217 : 兩段速移動
    edtAutoSkipCT->Text             =ArmSpeed_File[InArm].iAutoSkipCT;
    rgInArmAutoSpeed->ItemIndex     =(ArmSpeed_File[InArm].bAutoSKIP==1)?0:1;
    rgInArmPitch->ItemIndex         =(ArmSpeed_File[InArm].bVariModeFIX==true)?1:0;                 //Jimmychiu 20251924 : bVariModeFIX==1 to bVariModeFIX==true
    rgInShtOrder->ItemIndex         =TestIF_File.iInArmToShtReleaseMode;        //Steven 20250630 : 力旺要照順序擺
    chkTwoSpeedOnlyLoader->Checked  =ArmSpeed_File[InArm].bTwoSpeedOnlyLoader;  //JerryYang 20190729 二段速功能可選擇only at loader

    chkInArmPreSuck->Checked        =!ArmSpeed_File[InArm].bSuckOnDown;
    udInPitchSpd->Position          =ArmSpeed_File[InArm].iVariSP;
    edInPitchSpd->Text              =udInPitchSpd->Position;
    udInPitchAcc->Position          =ArmSpeed_File[InArm].iACDCVariSP;
    edInPitchAcc->Text              =udInPitchAcc->Position;

    //Steven 20140428 : For Rotate
    udInRotSpd->Position            =ArmSpeed_File[InRotate].iBodySP;
    edInRotSpd->Text                =udInRotSpd->Position;
    udInRotAcc->Position            =ArmSpeed_File[InRotate].iACDCBodySP;
    edInRotAcc->Text                =udInRotAcc->Position;

    //2013-08-01    Dell    modify  公司統一回吸功能
    Edt_CheckTime_In->Text          =FormatFloat("0.0", ArmSpeed_File[InArm].dDestroyCheckTime);
    ckb_PauseCheck_In->Checked      =ArmSpeed_File[InArm].bDestroyPauseCheck;
    edRelaseDelay->Text             =FormatFloat("0.00", ArmSpeed_File[InArm].dReleaseDelayTime);   //JerryYang 20160127 for TSMC inarm release device前delay
    rgReleaseDelayTime->ItemIndex   =ArmSpeed_File[InArm].iEnableReleaseDelay;                      //JerryYang 20160127 for TSMC inarm release device前delay

    edInArmDieCleanDelay->Text      =FormatFloat("0.00", ArmSpeed_File[InArm].dDieCleanDelay);      //wei 20170418
    rgInArmDieClean->ItemIndex      =(ArmSpeed_File[InArm].iEnableDieClean==1)?0:1;                 //wei 20170418
    edInArmDieCleanHeight->Text     =FormatFloat("0.00", ArmSpeed_File[InArm].dDieCleanHeight);     //wei 20170418
    ckb_HeightCheck_In->Checked     =ArmSpeed_File[InArm].bHeightCheck;                             //wei 20160825 回吸偵測
    Edt_HeightCheck_In->Text        =ArmSpeed_File[InArm].iHeightCheck;                             //wei 20160825 回吸偵測
    gbInarmDeviveConfirm->ItemIndex =(ArmSpeed_File[InArm].bDevicConfirm==1)?0:1;                   //kevin 20170510 (wei) 回吸功能開關內定開啟
    rgSearchInarmLastModeForYPitch->ItemIndex=(ArmSpeed_File[InArm].bYPitchUseSearchLastMode==0)?1:0;

    edInArmCylinderDelay->Text      =FormatFloat("0.00", ArmSpeed_File[InArm].dCylinderDelay);
    rgTT_SetSpeed->ItemIndex        =(ArmSpeed_File[InArm].bTT_SetSpeed==false)?1:0;                //Ztex 2024.08.11 Add Test Time Set Speed
    //--------------------------------------------------------------------------
    //Output Arm
    //--------------------------------------------------------------------------
    udOutXSpd->Position             =ArmSpeed_File[OutArm].iBodySP;
    edOutXSpd->Text                 =udOutXSpd->Position;
    udOutXAcc->Position             =ArmSpeed_File[OutArm].iACDCBodySP;
    edOutXAcc->Text                 =udOutXAcc->Position;
    udOutZSpd->Position             =ArmSpeed_File[OutArm].iZSP;
    edOutZSpd->Text                 =udOutZSpd->Position;
    udOutZAcc->Position             =ArmSpeed_File[OutArm].iACDCZSP;
    edOutZAcc->Text                 =udOutZAcc->Position;

    //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    edOutArmDestroyAgainTime->Text  =FormatFloat("0.00", ArmSpeed_File[OutArm].dDestroyAgainTime);
    edOutArmDestroyAgainCount->Text =FormatFloat("0", ArmSpeed_File[OutArm].iDestroyAgainCount);
    edOutArmRetryCount->Text        =FormatFloat("0", ArmSpeed_File[OutArm].iRetryCT) ;
    edOutArmRetryMM->Text           =FormatFloat("0.00", ArmSpeed_File[OutArm].dRetryDown);
    edOutVacumCheckTime->Text       =FormatFloat("0.00", ArmSpeed_File[OutArm].dVacuumTI);
    edOutArmShtWaitTime->Text       =FormatFloat("0.00", ArmSpeed_File[OutArm].dWaitOnSH);          //kevin 20210525 add out arm 等待時間到再往下 Shuttle 吸取 IC
    edOutArmAirOnTime->Text         =FormatFloat("0.00", ArmSpeed_File[OutArm].dCTAirOn);
    rgOutArmPitch->ItemIndex        =(ArmSpeed_File[OutArm].bVariModeFIX==0)?0:1;
    rgTwoSpeedOnOff_Out->ItemIndex  =ArmSpeed_File[OutArm].iTwoSpeedMove;                           //Steven 20140217 : 兩段速移動
    edTwoSpeedDistance_Out->Text    =FormatFloat("0.00", ArmSpeed_File[OutArm].dTwoSpeedDistance);  //Steven 20140217 : 兩段速移動
    edSecondSpeedOut->Text          =AnsiString(ArmSpeed_File[OutArm].iTwoSpeed);                   //Steven 20140217 : 兩段速移動
    edSecondADCOut->Text            =AnsiString(ArmSpeed_File[OutArm].iTwoADC);                     //Steven 20140217 : 兩段速移動

    chkOutArmPreSuck->Checked       =!ArmSpeed_File[OutArm].bSuckOnDown;
    udOutPitchSpd->Position         =ArmSpeed_File[OutArm].iVariSP;
    edOutPitchSpd->Text             =udOutPitchSpd->Position;
    udOutPitchAcc->Position         =ArmSpeed_File[OutArm].iACDCVariSP;
    edOutPitchAcc->Text             =udOutPitchAcc->Position;
    edtOutArmPreSuck->Text          =FormatFloat("0.00", ArmSpeed_File[OutArm].dDestroyPauseTime);
    gbOutarmDeviveConfirm->ItemIndex=(ArmSpeed_File[OutArm].bDevicConfirm==1)?0:1;                  //kevin 20170510 (wei) 回吸功能開關
    rgSearchLastModeForYPitch->ItemIndex=(ArmSpeed_File[OutArm].bYPitchUseSearchLastMode==0)?1:0;   //JerryYang 20181112 (Steven) : ATP要求修改unloader擺放方式，盡量不要有空格

    //Steven 20140428 : For Rotate
    udOutRotSpd->Position           =ArmSpeed_File[OutRotate].iBodySP;
    edOutRotSpd->Text               =udOutRotSpd->Position;
    udOutRotAcc->Position           =ArmSpeed_File[OutRotate].iACDCBodySP;
    edOutRotAcc->Text               =udOutRotAcc->Position;

    //2013-08-01    Dell    modify  公司統一回吸功能
    Edt_CheckTime_Out->Text         =FormatFloat("0.0", ArmSpeed_File[OutArm].dDestroyCheckTime);
    ckb_PauseCheck_Out->Checked     =ArmSpeed_File[OutArm].bDestroyPauseCheck;

    edOutArmDieCleanDelay->Text      =FormatFloat("0.00", ArmSpeed_File[OutArm].dDieCleanDelay);        //wei 20170418
    rgOutArmDieClean->ItemIndex      =(ArmSpeed_File[OutArm].iEnableDieClean==0)?1:0;                   //wei 20170418

    edOutArmCylinderDelay->Text      =FormatFloat("0.00", ArmSpeed_File[OutArm].dCylinderDelay);
    //--------------------------------------------------------------------------
    //Empty Tray Arm
    //--------------------------------------------------------------------------
    udTrayXSpd->Position            =ArmSpeed_File[TrayArm].iBodySP;
    edTrayXSpd->Text                =udTrayXSpd->Position;
    udTrayXAcc->Position            =ArmSpeed_File[TrayArm].iACDCBodySP;
    edTrayXAcc->Text                =udTrayXAcc->Position;
    edTrayArmRetryCount->Text       =ArmSpeed_File[TrayArm].iRetryCT;
    edTrayVacumCheckTime->Text      =FormatFloat("0.00", ArmSpeed_File[TrayArm].dVacuumTI);
    edTrayArmAirOnTime->Text        =FormatFloat("0.00", ArmSpeed_File[TrayArm].dCTAirOn);
    edTrayArmHandDown->Text         =FormatFloat("0.00", ArmSpeed_File[TrayArm].dTAHeadDownIT);
    //--------------------------------------------------------------------------
    //Shuttle
    //--------------------------------------------------------------------------
    udSht1Spd->Position             =SHSpeed_File.iSH1Sp;
    edSht1Spd->Text                 =udSht1Spd->Position;
    udSht1Acc->Position             =SHSpeed_File.iSH1ACDCSp;
    edSht1Acc->Text                 =udSht1Acc->Position;
    udSht2Spd->Position             =SHSpeed_File.iSH2Sp;
    edSht2Spd->Text                 =udSht2Spd->Position;
    udSht2Acc->Position             =SHSpeed_File.iSH2ACDCSp;
    edSht2Acc->Text                 =udSht2Acc->Position;
    rgStepShuttle->ItemIndex        =(SHSpeed_File.iStepSH==1)?0:1;
    rgShakeShuttle->ItemIndex       =TestIF_File.iShakeShuttleWhenPlaceIC;      //Steven 20200616 : ATK要求每次放料都要搖搖蝦頭  //Steven 20220427 : 每次放料都要抖抖馬達
    edShakeCycles->Text             =SHSpeed_File.iShakeCycles;                 //Sam 20250326 : 新增 Shake 條件設定
    edShakeDistance->Text           =SHSpeed_File.iShakeDistance;
    edShakeDelay->Text              =SHSpeed_File.dShakeDelay;
    edShakeAccDec->Text             =SHSpeed_File.iShakeAccDec;
    //--------------------------------------------------------------------------
    //Magazine
    //--------------------------------------------------------------------------
    udMagCatchYSpd->Position        =MGSpeed_File.iCYSp;
    edMagCatchYSpd->Text            =udMagCatchYSpd->Position;
    udMagCatchYAcc->Position        =MGSpeed_File.iCYACDCSp;
    edMagCatchYAcc->Text            =udMagCatchYAcc->Position;

    udMagZSpd->Position              =MGSpeed_File.iMZSp;
    edMagZSpd->Text                  =udMagZSpd->Position;
    udMagZAcc->Position              =MGSpeed_File.iMZACDCSp;
    edMagZAcc->Text                  =udMagZAcc->Position;

    edTwoSpeedDistanceCatchY->Text =FormatFloat("0.00", MGSpeed_File.dCYDecRange);
    edSecondSpeedCatchY->Text        =AnsiString(MGSpeed_File.iCY2Sp);
    edSecondADCCatchY->Text          =AnsiString(MGSpeed_File.iCY2ACDCSp);

    //Steven 20170317 (wei) mark
    //==>
//    cbOutPickErrAct->Checked        =TestIF_File.bOutPickErrAct;              //Steven 20170202 : For ATJ Out Arm pickup error put to define tray
//    cobOutPickErrAct->ItemIndex     =TestIF_File.iOutPickErrAuto;
    //<==
    //Steven 20170317 (wei) mark
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edInArmRetryCountKeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edInVacumCheckTimeKeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edIndexArmRetryMMKeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edInArmRetryCountMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CosFunction.bUseOneByOneIndexCheck==true)
    {
       fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 2, true, 3, 0);
    }
    else
    {
       fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 2, true, 10, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edInVacumCheckTimeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 10.0, 0.01);  //Steven 20150420
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edIndexArmRetryMMMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, -0.01, -0.3); //Steven 20150420
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edIndexSpeedMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, (CosFunction.bLimitMaxSpeed)?CosFunction.iLimitMaxSpeed:100, 1);  //Steven 20150420
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edIndexAccDecMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 100, 1);     //Steven 20150420
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
    ReadFile();
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::tbAllSpeedChange(TObject *Sender)
{
    edAllSpeed->Text            =tbAllSpeed->Position;
    tbAllSpeed->SelEnd          =tbAllSpeed->Position;
    if(cbIndexArm->Checked)
    {
        edIndexSpeed->Text      =tbAllSpeed->Position;
        udIndexSpd->Position    =tbAllSpeed->Position;
    }

    if(cbInArm->Checked)
    {
        edInXYSpd->Text         =tbAllSpeed->Position;
        edInPitchSpd->Text      =tbAllSpeed->Position;                          //Steven 20210608 : 100 --> tbAllSpeed->Position
        udInXSpd->Position      =tbAllSpeed->Position;
    }

    if(cbOutArm->Checked)
    {
        edOutXSpd->Text         =tbAllSpeed->Position;
        edOutPitchSpd->Text     =tbAllSpeed->Position;                          //Steven 20210608 : 100 --> tbAllSpeed->Position
        udOutXSpd->Position     =tbAllSpeed->Position;
    }

    if(cbShuttle->Checked)
    {
        edSht1Spd->Text         =tbAllSpeed->Position;
        edSht2Spd->Text         =tbAllSpeed->Position;                          //jou 2016-09-01 edSht1Spd -> edSht2Spd fix all speed shuttle2 error
        udSht1Spd->Position     =tbAllSpeed->Position;
        udSht2Spd->Position     =tbAllSpeed->Position;
    }

    if(cbTrayArm->Checked)
    {
        edTrayXSpd->Text        =tbAllSpeed->Position;
        udTrayXSpd->Position    =tbAllSpeed->Position;
    }

    if(cbInArmZ->Checked)
    {
        edInZSpd->Text          =tbAllSpeed->Position;
        udInZSpd->Position      =tbAllSpeed->Position;
    }

    if(cbOutArmZ->Checked)
    {
        edOutZSpd->Text         =tbAllSpeed->Position;
        udOutZSpd->Position     =tbAllSpeed->Position;
    }

    if(cbInRotate->Checked)                                                     //kevin 20130517 rotate
    {
        edInRotSpd->Text        =tbAllSpeed->Position;
        udInRotSpd->Position    =tbAllSpeed->Position;
    }

    if(cbOutRotate->Checked)                                                    //kevin 20130517 rotate
    {
        edOutRotSpd->Text       =tbAllSpeed->Position;
        udOutRotSpd->Position   =tbAllSpeed->Position;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::tbAccSpeedChange(TObject *Sender)
{
    edAllAccSpeed->Text         =tbAccSpeed->Position;
    tbAccSpeed->SelEnd          =tbAccSpeed->Position;
    if(cbIndexArm->Checked)
    {
        //edIndexAccDec->Text   =tbAccSpeed->Position;                          //Steven 20091214 Mark for Index ACC & DEC must be 100%
        udIndexAcc->Position    =tbAccSpeed->Position;
    }

    if(cbInArm->Checked)
    {
        edInXYAcc->Text         =tbAccSpeed->Position;
        edInPitchAcc->Text      =tbAccSpeed->Position;                          //Steven 20210608 : 100 --> tbAllSpeed->Position
        udInXAcc->Position      =tbAccSpeed->Position;
    }

    if(cbOutArm->Checked)
    {
        edOutXAcc->Text         =tbAccSpeed->Position;
        edOutPitchAcc->Text     =tbAccSpeed->Position;                          //Steven 20210608 : 100 --> tbAllSpeed->Position
        udOutXAcc->Position     =tbAccSpeed->Position;
    }

    if(cbShuttle->Checked)
    {
        edSht1Acc->Text         =tbAccSpeed->Position;
        edSht2Acc->Text         =tbAccSpeed->Position;
        udSht1Acc->Position     =tbAccSpeed->Position;
        udSht2Acc->Position     =tbAccSpeed->Position;
    }

    if(cbTrayArm->Checked)
    {
        edTrayXAcc->Text        =tbAccSpeed->Position;
        udTrayXAcc->Position    =tbAccSpeed->Position;
    }

    if(cbInArmZ->Checked)
    {
        edInZAcc->Text          =tbAccSpeed->Position;
        udInZAcc->Position      =tbAccSpeed->Position;
    }

    if(cbOutArmZ->Checked)
    {
        edOutZAcc->Text         =tbAccSpeed->Position;
        udOutZAcc->Position     =tbAccSpeed->Position;
    }

    if(cbInRotate->Checked)                                                     //kevin 20130517 rotate
    {
        edInRotAcc->Text        =tbAccSpeed->Position;
        udInRotAcc->Position    =tbAccSpeed->Position;
    }

    if(cbOutRotate->Checked)                                                    //kevin 20130517 rotate
    {
        edOutRotAcc->Text       =tbAccSpeed->Position;
        udOutRotAcc->Position   =tbAccSpeed->Position;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::tbEPControlChange(TObject *Sender)
{
    edEPControl->Text  =tbEPControl->Position;
    tbEPControl->SelEnd=tbEPControl->Position;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::spbSpeedAddClick(TObject *Sender)
{
    tbAllSpeed->Position+=10;
    tbAllSpeed->Position/=10;
    tbAllSpeed->Position*=10;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::spbSpeedDecClick(TObject *Sender)
{
    tbAllSpeed->Position-=10;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::cbIndexArmClick(TObject *Sender)
{
    tbAllSpeed->Enabled =true;
    spbSpeedAdd->Enabled=true;
    spbSpeedDec->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::spbSaveClick(TObject *Sender)
{
    AnsiString szDir="";
    if(CosFunction.bSaveArmSpeedByMachine && IniConfig.bA57_1SaveArmSpeedByMachine)//JimmyChiu 20220618 : save by machine
    {
        szDir=sSaveByMachine;
    }
    else
    {
        szDir.sprintf("%s%s", DataPath, LastFileName);
    }
    MyForceDirectories(szDir);
    szDir+="\\ArmCondition.Data";
    edIndexAccDec->Text=100;                                                    //Steven 20091214 Add for Index ACC & DEC must be 100%

    //jou 2010-09-10 start : 增加保護speed In/Out Arm 吸嘴Retry高度不應該是正的值
    edInArmRetryMM->Text =AnsiString(CheckRange(atof(edInArmRetryMM->Text.c_str()),  -0.01, -0.3));
    edOutArmRetryMM->Text=AnsiString(CheckRange(atof(edOutArmRetryMM->Text.c_str()), -0.01, -0.3));
    //jou 2010-09-10 end

    WriteIniData(szDir, "All", "Speed",     edAllSpeed->Text);
    WriteIniData(szDir, "All", "Accel",     edAllAccSpeed->Text);
    WriteIniData(szDir, "All", "EPControl", edEPControl->Text);                 //Ifor 20150710 :EP Control

    //--------------------------------------------------------------------------
    //Index Arm
    //--------------------------------------------------------------------------
    if(CUSTOMER_CODE==CC_ASE_SG)                                                //Ifor 20240913 ASE 新加坡客戶要求 X => Z
    {
        WriteIniData(szDir, "Index Arm", "Y Speed",             edIndexSpeed->Text);
        WriteIniData(szDir, "Index Arm", "Y Accel",             edIndexAccDec->Text);
    }
    else
    {
        WriteIniData(szDir, "Index Arm", "X Speed",             edIndexSpeed->Text);
        WriteIniData(szDir, "Index Arm", "X Accel",             edIndexAccDec->Text);
    }
    WriteIniData(szDir, "Index Arm", "Z Up Speed",              edIndexSpeed->Text);
    WriteIniData(szDir, "Index Arm", "Z Up Accel",              edIndexAccDec->Text);
    WriteIniData(szDir, "Index Arm", "Z Down Speed",            edIndexSpeed->Text);
    WriteIniData(szDir, "Index Arm", "Z Down Accel",            edIndexAccDec->Text);
    WriteIniData(szDir, "Index Arm", "Retry Count",             edIndexRetryCount->Text);
    WriteIniData(szDir, "Index Arm", "Retry Down",              edIndexArmRetryMM->Text);
    WriteIniData(szDir, "Index Arm", "Vacuum Check Time",       edIndexVacumCheckTime->Text);
    WriteIniData(szDir, "Index Arm", "Counter Air ON Time",     edIndexAirOnTime->Text);
    WriteIniData(szDir, "Output Arm", "Shuttle Wait Time",      edOutArmShtWaitTime->Text);        //kevin 20210525 out arm 在SHUTTLE 上等待 時間再往下吸IC

    //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    WriteIniData(szDir, "Index Arm", "Destroy Again Time",      edIndexDestroyAgainTime->Text);
    WriteIniData(szDir, "Index Arm", "Destroy Again Count",     edIndexDestroyAgainCount->Text);

    WriteIniData(szDir, "Index Arm", "Vacuum Check Optimize",   (cbIndexVacuumCheckOptimize->Checked)?1:0);
    WriteIniData(szDir, "Index Arm", "Socket Check",            rgSocketCheck->ItemIndex);
    WriteIniData(szDir, "Index Arm", "Vacuum Timing",           (chkIndexPreSuck->Checked)?0:1);
    WriteIniData(szDir, "Index Arm", "Pick IC when out shuttle no IC", (chkPickAfterOutArm->Checked)?1:0);  //Steven 20221207 : Index必須在out shuttle沒料才可以吸
    WriteIniData(szDir, "Index Arm", "Enable Delay Time Zero",  (cbEnableDelayTimeZero->Checked)?1:0);      //Jimmychiu 20230922 : R230824-ATK-H9-01 Request add “Air purge during place device on out-shuttle” function.

    //Isaac 20180301 (Steven) Index Cycle Time Monitoring function
    //==>
    if(IniConfig.bSPILFunction)     //JerryYang 20220824 : SPIL index cycle time monitor
    {
        WriteIniData(szDir, "Index Arm", "EnableNewIndexCycleTimeMonitoring",  (chkEnableIndexCycleTimeMonitoring->Checked)?1:0);   //Isaac 20180301
    }
    else
    {
        WriteIniData(szDir, "Index Arm", "EnableIndexCycleTimeMonitoring",  (chkEnableIndexCycleTimeMonitoring->Checked)?1:0);   //Isaac 20180301
    }

    WriteIniData(szDir, "Index Arm", "Monitoring_IndexCycletime",       edtMonitoringIndexCycletime->Text);
    WriteIniData(szDir, "Index Arm", "Monitoring_Outlier",              edtMonitoringOutlier->Text);
    WriteIniData(szDir, "Index Arm", "Monitoring_Window",               edtMonitoringWindow->Text);
    WriteIniData(szDir, "Index Arm", "IndexCycleTimetolerance",         edtIndexCycleTimetolerance->Text);  //JerryYang 20220824 : SPIL index cycle time monitor
    WriteIniData(szDir, "Index Arm", "DevicConfirm",           (gbIndexeviveConfirm->ItemIndex==0)?1:0);    //kevin 20190629 add index 4 site conform
    WriteIniData(szDir, "Index Arm", "IndexCycleTimeAction",   rgIndexCycleTime->ItemIndex);                //Steven 20250521 : 可以選擇要不要停機

    if(CosFunction.bOutArmPickUpErrBySetup==true)                               //JerryYang 20220923 : E50改成選項
    {
        WriteIniData(szDir, "Output Arm", "iE50_OutArmPickUpErrorOption",           rgOutArmPickupErrMode->ItemIndex);  //kevin 20190629 add index 4 site conform
    }

    if(TestIF_File.bIndexCycleTimeMonitor)
    {
        if(IniConfig.bSPILFunction)                                             //JerryYang 20220824 : SPIL index cycle time monitor
        {
        }
        else
        {
            IniRecordMonitoringIndexCycleTime();
        }
        //        IniRecordMonitoringIndexCycleTime();
        if(MyMessageBox->Visible==true)                                         //Alarm 前若form有開啟先關閉
        {
            MyMessageBox->Close();
        }
        bResetflag=true;
    }
    //<==
    //Isaac 20180301 (Steven) Index Cycle Time Monitoring function

    //--------------------------------------------------------------------------
    //Input Arm
    //--------------------------------------------------------------------------
    WriteIniData(szDir, "Input Arm", "XY Speed",                edInXYSpd->Text);
    WriteIniData(szDir, "Input Arm", "XY Accel",                edInXYAcc->Text);

    //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    WriteIniData(szDir, "Input Arm", "Destroy Again Time",      edInArmDestroyAgainTime->Text);
    WriteIniData(szDir, "Input Arm", "Destroy Again Count",     edInArmDestroyAgainCount->Text);

    //ChungHung 20110408 Z Speed can change start
//    if(atoi(edInZSpd->Text.c_str())>atoi(edInXYSpd->Text.c_str()) || atoi(edInZSpd->Text.c_str())>atoi(edInPitchSpd->Text.c_str()))
//    {
//        edInZSpd->Text=min(atoi(edInXYSpd->Text.c_str()), atoi(edInPitchSpd->Text.c_str()));
//    }
//    if(atoi(edInZAcc->Text.c_str())>atoi(edInXYAcc->Text.c_str()) || atoi(edInZAcc->Text.c_str())>atoi(edInPitchAcc->Text.c_str()))
//    {
//        edInZAcc->Text=min(atoi(edInXYAcc->Text.c_str()), atoi(edInPitchAcc->Text.c_str()));
//    }
    //ChungHung 20110408 Z Speed can change end
    WriteIniData(szDir, "Input Arm", "Z Up Speed",              edInZSpd->Text);
    WriteIniData(szDir, "Input Arm", "Z Up Accel",              edInZAcc->Text);
    WriteIniData(szDir, "Input Arm", "Z Down Speed",            edInZSpd->Text);
    WriteIniData(szDir, "Input Arm", "Z Down Accel",            edInZAcc->Text);
    WriteIniData(szDir, "Input Arm", "Auto Speed Down",         (rgAutoSpeed->ItemIndex==0)?1:0);
    WriteIniData(szDir, "Input Arm", "AutoSpeedLow",            edAutoSpeedLow->Text);               //kevin 20210913 add low speed
    WriteIniData(szDir, "Input Arm", "Retry Count",             edInArmRetryCount->Text);
    WriteIniData(szDir, "Input Arm", "Retry Down",              edInArmRetryMM->Text);
    WriteIniData(szDir, "Input Arm", "Vacuum Check Time",       edInVacumCheckTime->Text);
    WriteIniData(szDir, "Input Arm", "HP Vacuum Check Time",    edtHPVacuumDelay->Text);            //Steven 20180125 (Jou) : 加熱盤的真空等待時間
    WriteIniData(szDir, "Input Arm", "Use HP Vacuum Check Time",  chkUseHPVacuumDelay->Checked);    //Steven 20180125 (Jou) : 加熱盤的真空等待時間
    WriteIniData(szDir, "Input Arm", "Counter Air ON Time",     edInArmAirOnTime->Text);
    WriteIniData(szDir, "Input Arm", "Shuttle Wait Time",       edInArmShtWaitTime->Text);
    WriteIniData(szDir, "Input Arm", "Two Speed On Off",        rgTwoSpeedOnOff_In->ItemIndex);     //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Input Arm", "Two Speed Distance",      edTwoSpeedDistance_In->Text);       //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Input Arm", "Two Speed Precent",       edSecondSpeedIn->Text);             //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Input Arm", "Two Speed ADC",           edSecondADCIn->Text);               //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Input Arm", "Two Speed Only Loader",   chkTwoSpeedOnlyLoader->Checked);    //JerryYang 20190729 二段速功能可選擇only at loader

    //auto Skip enable
    WriteIniData(szDir, "Input Arm", "Auto Skip",               (rgInArmAutoSpeed->ItemIndex==0)?1:0);
    WriteIniData(szDir, "Input Arm", "Auto Skip CT",            edtAutoSkipCT->Text);

    //One by One Enable
    if(IniConfig.bI37_EnableFIFOMode && LastSet.iRunStartMode==rsmFIFOMode)     //Steven 20160303 (wei) : FIFO Mode
    {
        ;
    }
    else
    {
        WriteIniData(szDir, "Input Arm", "One by one",          (rgInArmPitch->ItemIndex==0)?0:1);
    }

    WriteIniData(szDir, "Input Arm", "iInArmToShtReleaseMode",  rgInShtOrder->ItemIndex);           //Steven 20250630 : 力旺要照順序擺
    WriteIniData(szDir, "Input Arm", "Vacuum Timing",           (chkInArmPreSuck->Checked)?0:1);
    WriteIniData(szDir, "Input Arm", "Vacuum Check Optimize",   (cbInVacuumCheckOptimize->Checked)?1:0);
    WriteIniData(szDir, "Input Arm", "Open/Close Speed",        edInPitchSpd->Text);
    WriteIniData(szDir, "Input Arm", "Open/Close Accel",        edInPitchAcc->Text);

    WriteIniData(szDir, "Input Arm", "Rotate Input Speed",      edInRotSpd->Text);                  //kevin 20130517 rotate add
    WriteIniData(szDir, "Input Arm", "Rotate Input Accel",      edInRotAcc->Text);                  //kevin 20130517 rotate add

    //2013-08-01    Dell    modify  公司統一回吸功能
    WriteIniData(szDir, "Input Arm", "Destroy Check Time",      Edt_CheckTime_In->Text);
    WriteIniData(szDir, "Input Arm", "Destroy Pause Check",     (ckb_PauseCheck_In->Checked)?1:0);
    WriteIniData(szDir, "Input Arm", "Release Delay Time",      edRelaseDelay->Text);               //JerryYang 20160127 for TSMC inarm release device前delay
    WriteIniData(szDir, "Input Arm", "Enable Relase Delay",     rgReleaseDelayTime->ItemIndex);     //JerryYang 20160127 for TSMC inarm release device前delay
    WriteIniData(szDir, "Input Arm", "Enable Height Check",    (ckb_HeightCheck_In->Checked)?1:0);  //wei 20160825 回吸偵測
    WriteIniData(szDir, "Input Arm", "Height Check",            Edt_HeightCheck_In->Text);          //wei 20160825 回吸偵測
    WriteIniData(szDir, "Input Arm", "DevicConfirm",           (gbInarmDeviveConfirm->ItemIndex==0)?1:0); //kevin 20170510 (wei) 回吸偵測開關
    WriteIniData(szDir, "Input Arm", "bYPitchNotUseSearchLastMode", (rgSearchInarmLastModeForYPitch->ItemIndex==0)?1:0);//kevin 20211130 add Ypitch 要求修改unloader擺放方式，盡量不要有空格

    WriteIniData(szDir, "Input Arm", "Cylinder Delay",          edInArmCylinderDelay->Text);
    if(USE_DIE_CLEAN)
    {
        WriteIniData(szDir, "Input Arm", "Enable Die Clean",    (rgInArmDieClean->ItemIndex==0)?1:0);        //wei 20170418
        WriteIniData(szDir, "Input Arm", "Die Clean Delay",     edInArmDieCleanDelay->Text);        //wei 20170418
        WriteIniData(szDir, "Input Arm", "Die Clean Height",    edInArmDieCleanHeight->Text);       //wei 20170418
    }
    WriteIniData(szDir, "Input Arm", "Test Time Set Speed",     (rgTT_SetSpeed->ItemIndex==0)?1:0); //Ztex 2024.08.11 Add Test Time Set Speed
    //--------------------------------------------------------------------------
    //Output Arm
    //--------------------------------------------------------------------------
    WriteIniData(szDir, "Output Arm", "XY Speed",               edOutXSpd->Text);
    WriteIniData(szDir, "Output Arm", "XY Accel",               edOutXAcc->Text);

    //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    WriteIniData(szDir, "Output Arm", "Destroy Again Time",     edOutArmDestroyAgainTime->Text);
    WriteIniData(szDir, "Output Arm", "Destroy Again Count",    edOutArmDestroyAgainCount->Text);

    //ChungHung 20110408 Z Speed can change start
    /*
    if(atoi(edOutZSpd->Text.c_str())>atoi(edOutXSpd->Text.c_str()) || atoi(edOutZSpd->Text.c_str())>atoi(edOutPitchSpd->Text.c_str()))
    {
        edOutZSpd->Text = min( atoi(edOutXSpd->Text.c_str()), atoi(edOutPitchSpd->Text.c_str()));
    }
    if(atoi(edOutZAcc->Text.c_str())>atoi(edOutXAcc->Text.c_str()) || atoi(edOutZAcc->Text.c_str())>atoi(edOutPitchAcc->Text.c_str()))
    {
        edOutZAcc->Text = min(atoi(edOutXAcc->Text.c_str()), atoi(edOutPitchAcc->Text.c_str()));
    } *///kevin 20170822 (Steven) 不卡 最大值
    //ChungHung 20110408 Z Speed can change start
    WriteIniData(szDir, "Output Arm", "Z Up Speed",             edOutZSpd->Text);
    WriteIniData(szDir, "Output Arm", "Z Up Accel",             edOutZAcc->Text);
    WriteIniData(szDir, "Output Arm", "Z Down Speed",           edOutZSpd->Text);
    WriteIniData(szDir, "Output Arm", "Z Down Accel",           edOutZAcc->Text);
    WriteIniData(szDir, "Output Arm", "Auto Speed Down",        (rgAutoSpeed->ItemIndex==0)?1:0);
    WriteIniData(szDir, "Output Arm", "AutoSpeedLow",            edAutoSpeedLow->Text);               //kevin 20210913 add low speed
    WriteIniData(szDir, "Output Arm", "Retry Count",            edOutArmRetryCount->Text);
    WriteIniData(szDir, "Output Arm", "Retry Down",             edOutArmRetryMM->Text);
    WriteIniData(szDir, "Output Arm", "Vacuum Check Time",      edOutVacumCheckTime->Text);
    WriteIniData(szDir, "Output Arm", "Counter Air ON Time",    edOutArmAirOnTime->Text);
    WriteIniData(szDir, "Output Arm", "Shuttle Wait Time",      edOutArmShtWaitTime->Text);        //kevin 20210525 out arm 在SHUTTLE 上等待 時間再往下吸IC
    WriteIniData(szDir, "Output Arm", "Two Speed On Off",       rgTwoSpeedOnOff_Out->ItemIndex);   //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Output Arm", "Two Speed Distance",     edTwoSpeedDistance_Out->Text);     //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Output Arm", "Two Speed Precent",      edSecondSpeedOut->Text);           //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Output Arm", "Two Speed ADC",          edSecondADCOut->Text);             //Steven 20140217 : 兩段速移動

    //One by One Enable
    if(IniConfig.bI37_EnableFIFOMode && LastSet.iRunStartMode==rsmFIFOMode)       //Steven 20160303 (wei) : FIFO Mode
    {
        ;
    }
    else
    {
        WriteIniData(szDir, "Output Arm", "One by one",         (rgOutArmPitch->ItemIndex==0)?0:1);
    }
    WriteIniData(szDir, "Output Arm", "Vacuum Timing",          (chkOutArmPreSuck->Checked)?0:1);
    WriteIniData(szDir, "Output Arm", "Vacuum Check Optimize",  (cbOutVacuumCheckOptimize->Checked)?1:0);
    WriteIniData(szDir, "Output Arm", "Open/Close Speed",       edOutPitchSpd->Text);
    WriteIniData(szDir, "Output Arm", "Open/Close Accel",       edOutPitchAcc->Text);
    WriteIniData(szDir, "Output Arm", "Wait before Air On",     edtOutArmPreSuck->Text);

    WriteIniData(szDir, "Output Arm", "Rotate Output Speed",    edOutRotSpd->Text);  //kevin 20130517 rotate add
    WriteIniData(szDir, "Output Arm", "Rotate Output Accel",    edOutRotAcc->Text);  //kevin 20130517 rotate add
    //2013-08-01    Dell    modify  公司統一回吸功能
    WriteIniData(szDir, "Output Arm", "Destroy Check Time",     Edt_CheckTime_Out->Text);
    WriteIniData(szDir, "Output Arm", "Destroy Pause Check",    (ckb_PauseCheck_Out->Checked)?1:0);
    WriteIniData(szDir, "Output Arm", "DevicConfirm",           (gbOutarmDeviveConfirm->ItemIndex==0)?1:0);  //kevin 20170510 (wei) 回吸偵測開關
    WriteIniData(szDir, "Output Arm", "bYPitchNotUseSearchLastMode", (rgSearchLastModeForYPitch->ItemIndex==0)?1:0);    //JerryYang 20181112 (Steven) : ATP要求修改unloader擺放方式，盡量不要有空格

    WriteIniData(szDir, "Output Arm", "Cylinder Delay",          edOutArmCylinderDelay->Text);
    //--------------------------------------------------------------------------
    //Empty Tray Arm
    //--------------------------------------------------------------------------
    WriteIniData(szDir, "Empty Tray Arm", "Speed",              edTrayXSpd->Text);
    WriteIniData(szDir, "Empty Tray Arm", "Accel",              edTrayXAcc->Text);
    WriteIniData(szDir, "Empty Tray Arm", "Retry Count",        edTrayArmRetryCount->Text);
    WriteIniData(szDir, "Empty Tray Arm", "Vacuum Check Time",  edTrayVacumCheckTime->Text);
    WriteIniData(szDir, "Empty Tray Arm", "Counter Air ON Time",edTrayArmAirOnTime->Text);
    WriteIniData(szDir, "Empty Tray Arm", "Hand Down Time",     edTrayArmHandDown->Text);
    //--------------------------------------------------------------------------
    //Shuttle
    //--------------------------------------------------------------------------
    WriteIniData(szDir, "Shuttle", "Shuttle 1 Speed",           edSht1Spd->Text);
    WriteIniData(szDir, "Shuttle", "Shuttle 1 Accel",           edSht1Acc->Text);
    WriteIniData(szDir, "Shuttle", "Shuttle 2 Speed",           edSht2Spd->Text);
    WriteIniData(szDir, "Shuttle", "Shuttle 2 Accel",           edSht2Acc->Text);
    WriteIniData(szDir, "Shuttle", "Device Check Time",         edShtDeviceCheckTime->Text);
    WriteIniData(szDir, "Shuttle", "Shake Cycles",              edShakeCycles->Text);       //Sam 20250326 : 新增 Shake 條件設定
    WriteIniData(szDir, "Shuttle", "Shake distance",            edShakeDistance->Text);
    WriteIniData(szDir, "Shuttle", "Delay between shakes",      edShakeDelay->Text);
    WriteIniData(szDir, "Shuttle", "Shake Accel",               edShakeAccDec->Text);
    //step shuttle
    WriteIniData(szDir, "Shuttle", "Step Shuttle",              (rgStepShuttle->ItemIndex==0)?1:0);
    WriteIniData(szDir, "Shuttle", "Shake Shuttle",             (rgShakeShuttle->ItemIndex));    //Steven 20200616 : ATK要求每次放料都要搖搖蝦頭   //Steven 20220427 : 每次放料都要抖抖馬達

    //--------------------------------------------------------------------------
    //Magazine
    //--------------------------------------------------------------------------
    WriteIniData(szDir, "Magazine", "Magazine CatchY Speed",     edMagCatchYSpd->Text);
    WriteIniData(szDir, "Magazine", "Magazine CatchY Accel",     edMagCatchYAcc->Text);
    WriteIniData(szDir, "Magazine", "MagazineZ Speed",           edMagZSpd->Text);
    WriteIniData(szDir, "Magazine", "MagazineZ Accel",           edMagZAcc->Text);

    WriteIniData(szDir, "Magazine", "Two Speed Distance",      edTwoSpeedDistanceCatchY->Text);
    WriteIniData(szDir, "Magazine", "Two Speed Precent",       edSecondSpeedCatchY->Text);
    WriteIniData(szDir, "Magazine", "Two Speed ADC",           edSecondADCCatchY->Text);
    ReadWriteFile(false, szDir);

    //Steven 20170317 (wei) mark
    //==>
//    if(CUSTOMER_CODE==CC_AMKOR_Japan)       //Steven 20170202 : For ATJ Out Arm pickup error put to define tray
//    {
//        WriteIniData(szDir, "Output Arm", "Out Pick Err Action", cbOutPickErrAct->Checked);
//        WriteIniData(szDir, "Output Arm", "Out Pick Err Tray",   cobOutPickErrAct->ItemIndex);
//    }
    //<==
    //Steven 20170317 (wei) mark

    AddSpace(szDir);
    ReadFile();
    DoIniDataToForm();
    fShowMessage->sgdSpeedView->Refresh();
    spbSave->Down=false;

    if(IniConfig.bEnable_SECS_GEM==true)        //Steven 20140528 : Secs Gem
    {
        bHasSaveSet=true;                       //Ifor 20151204 新增判斷機台有無修改設定檔
        EventReport(SECS_EVENT.SwitchSpeed);    //47     Change Handler Speed
    }

    if(CosFunction.bRecipeParameterDefault)     //Isaac 20170527 (Steven) defalut值比較功能
    {
        fRPDefault->RP_Name->Clear()    ;
        fRPDefault->RP_Default->Clear() ;
        fRPDefault->RP_Value->Clear()   ;
        SearchRecipeParameter(fSpeed->tsAllSpeed,-1);   //Isaac 20171227 (Steven) : AutoSpeed RPD一鍵復原
        SearchRecipeParameter(fSpeed->tsIndex,0);
        SearchRecipeParameter(fSpeed->tsInArm,1);
        SearchRecipeParameter(fSpeed->tsOutArm,2);
        SearchRecipeParameter(fSpeed->tsTrayArm,3);
        SearchRecipeParameter(fSpeed->tsShuttle,4);
         //Sam 20201209 : Default Recipe ChangeLog
        //==>
        if(CosFunction.bRecipeParameterDefaultLog)
        {
           fCleaning->SearchRecipeParameter(fCleaning, 0);
           FTestIF->SearchRecipeParameter(FTestIF, 0);
           fYieldMonitoring->SearchRecipeParameter(fYieldMonitoring, 0);
        }
        //<==
        //Sam 20201209 : Default Recipe ChangeLog
        fRPDefault->CompareRPDefaultAndValue(fRPDefault->RP_Name,fRPDefault->RP_Default,fRPDefault->RP_Value);
    }
    if(IniConfig.bSPILFunction || bEnable_KLT_Function)
    {
        //Ifor 20180802 (Steven) : add SPIL && KLT 判斷Auto Skip功能是否開啟並切換模式
        //==>
        AnsiString S="";
        S=GetLastOpenFN();
        szDir.sprintf("%s%s\\Tray.Data", DataPath, S);
        if(ArmSpeed_File[InArm].bAutoSKIP)
            TrayForm.iManualRemoveLoader=1;
        else
            TrayForm.iManualRemoveLoader       =ReadIniData(szDir, "Flag",     "Skip Manual Remove Tray",  0);
        //<==
        //Ifor 20180802 (Steven) : add SPIL && KLT 判斷Auto Skip功能是否開啟並切換模式
    }

    if(TestIF_File.bEnableReadAndCheckTorque)  //kevin 20210804 change
    {
        bResetArm1Value=true;
        bResetArm2Value=true;
    }
    fMain->BackupSetupFile();                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
    dmTrayMotor->StartSetSpeed();    //Steven 20210318 : Loader入Tray改步進, 換位置寫入速度, 避免動作異常
    bTT_SetSpeed_Check=false;                                                   //Ztex 2024.08.11 Add Test Time Set Speed
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::sbtExitClick(TObject *Sender)
{
    Close();
    fShowMessage->sgdSpeedView->Repaint();
    sbtExit->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::spbSelectAllClick(TObject *Sender)
{
    cbIndexArm->Checked =true;
    cbInArm->Checked    =true;
    cbOutArm->Checked   =true;
    cbShuttle->Checked  =true;
    cbTrayArm->Checked  =true;
    cbInArmZ->Checked   =true;
    cbOutArmZ->Checked  =true;
    tbAllSpeed->Enabled =true;
    spbSpeedAdd->Enabled=true;
    spbSpeedDec->Enabled=true;
    spbSelectAll->Down  =false;
    cbInRotate->Checked =true;
    cbOutRotate->Checked=true;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::spbSetToDefClick(TObject *Sender)
{
    tbAllSpeed->Position=100;
    tbAccSpeed->Position=100;
    edAllSpeed->Text    =100;
    edAllAccSpeed->Text =100;
    spbSelectAllClick(this);
    spbSpeedAddClick(this);
    tbAccSpeedChange(this);

    //Index unit Condition
    //Speed
    edIndexSpeed->Text=100;// %
    //Accel./Decel.
    edIndexAccDec->Text=100;// %
    //Retry Count
    edIndexRetryCount->Text=0;// Times
    edIndexArmRetryMM->Text=0;// Down mm
    //Function for small package
    chkIndexPreSuck->Checked=true;
    //Wait Time
    //Vacuum Check Wait
    edIndexVacumCheckTime->Text=0.1; // Sec
    //Counter Air On Time
    edIndexAirOnTime->Text=0.1;//Sec
    //Socket Device Floating Check
    rgSocketCheck->ItemIndex=1; //0:ON,1:OFF

    //InputArm Condition
    //Speed & Accel./Dece.
    //X-Y Axis
    edInXYSpd->Text=100; //Speed(%)
    edInXYAcc->Text=100; //Accel/Decel.(%)
    //Z-Axis
    edInZSpd->Text=100; //Speed(%)
    edInZAcc->Text=100; //Accel/Decel.(%)
    //Open/Xlose
    edInPitchSpd->Text=100; //Speed(%)
    edInPitchAcc->Text=100; //Accel/Decel.(%)
    //Retry Count
    edInArmRetryCount->Text=2;// Times
    edInArmRetryMM->Text=-0.01;// Down mm
    //Function for small package
    chkInArmPreSuck->Checked=true;
    //Wait Time
    //Vacuum Check Wait
    edInVacumCheckTime->Text=0.1; // Sec
    edtHPVacuumDelay->Text=0.1; // Sec
    //Counter Air On Time
    edInArmAirOnTime->Text=0.1;//Sec
    //Wait Time On Shuttle
    edInArmShtWaitTime->Text=0.01;//Sec
    //Auto Skip
    rgInArmAutoSpeed->ItemIndex=1; //0:ON,1:OFF
    //Variable Pitch Function
    rgInArmPitch->ItemIndex=0; //0:Open/Close,1:Fixed

    //OutputArm Condition
    //Speed & Accel./Dece.
    //X-Y Axis
    edOutXSpd->Text=100; //Speed(%)
    edOutXAcc->Text=100; //Accel/Decel.(%)
    //Z-Axis
    edOutZSpd->Text=100; //Speed(%)
    edOutZAcc->Text=100; //Accel/Decel.(%)
    //Open/Xlose
    edOutPitchSpd->Text=100; //Speed(%)
    edOutPitchAcc->Text=100; //Accel/Decel.(%)
    //Retry Count
    edOutArmRetryCount->Text=2;// Times
    edOutArmRetryMM->Text=-0.01;// Down mm
    //Function for small package
    chkOutArmPreSuck->Checked=true;
    edtOutArmPreSuck->Text=0.01; //Wait before Air-On Sec.
    //Wait Time
    //Vacuum Check Wait
    edOutVacumCheckTime->Text=0.1; // Sec
    //Counter Air On Time
    edOutArmAirOnTime->Text=0.1;//Sec
    //Variable Pitch Function
    rgOutArmPitch->ItemIndex=0; //0:Open/Close,1:Fixed

    //Shuttle Condition
    //Speed & Accel/Dece.
    //Shuttle1
    edSht1Spd->Text=100;//Speed(%)
    edSht1Acc->Text=100;//Accel/Decel.(%)
    //Shuttle2
    edSht2Spd->Text=100;//Speed(%)
    edSht2Acc->Text=100;//Accel/Decel.(%)
    //Wait Time
    //Device Check(Input)
    edShtDeviceCheckTime->Text=0.01;//Sec.
    //Step Shuttle
    rgStepShuttle->ItemIndex=1; //0:ON,1:Off

    //Empty Tray Arm Condition
    //Speed & Accel/Dece.
    edTrayXSpd->Text=100;//Speed(%)
    edTrayXAcc->Text=100;//Accel./Decel.(%)
    //Wait Time
    //Vacuum Check Wait
    edTrayVacumCheckTime->Text=0.1;//Sec.
    //Counter Air On Time
    edTrayArmAirOnTime->Text=0.1;//Sec.
    //Hand Down
    edTrayArmHandDown->Text=0.5;//Sec.
    //Retry Count
    edTrayArmRetryCount->Text=2; //Times
    edShakeCycles->Text=1;                                                      //Sam 20250326 : 新增 Shake 條件設定
    edShakeDistance->Text=5;
    edShakeDelay->Text=0.0;
    edShakeAccDec->Text=100;
    //--------------------------------------------------------------------------
    //Magazine
    //--------------------------------------------------------------------------
    edMagCatchYSpd->Text=50;//Speed(%)
    edMagCatchYAcc->Text=50;//Accel/Decel.(%)

    edMagZSpd->Text=50;//Speed(%)
    edMagZAcc->Text=50;//Accel/Decel.(%)
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::Edt_CheckTime_InKeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::Edt_CheckTime_InMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 1.0, 0.1);  //Steven 20140420
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edtAutoSkipCTMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1000, 5);  //Steven 20150205 : KK說要改成最小5次
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edSecondSpeedInMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, atoi(edInZSpd->Text.c_str()));  //Steven 20140217 : 兩段速移動
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edSecondADCInMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, atoi(edInZAcc->Text.c_str()));  //Steven 20140217 : 兩段速移動
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edSecondSpeedOutMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, atoi(edOutZSpd->Text.c_str()));  //Steven 20140217 : 兩段速移動
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edSecondADCOutMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, atoi(edOutZAcc->Text.c_str()));  //Steven 20140217 : 兩段速移動
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edRelaseDelayMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 3.0, 0.1);  //JerryYang 20160127 for TSMC inarm release device前delay
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edRelaseDelayKeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;                         //JerryYang 20160127 for TSMC inarm release device前delay
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::Edt_HeightCheck_InMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 1500, 500);  //Steven 20140420
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::DoSetRPDefault(TWinControl *PCtrl,int iSection)        //Isaac 20170527 (Steven) defalut值比較功能
{
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini", aSection="";
    AnsiString aHintName;
    int iValue=0;
    if(iSection==-1)        aSection="All";    //Isaac 20170718 (wei) defalut值比較功能:add trackbar default
    else if(iSection==0)    aSection="Index Arm";
    else if(iSection==1)    aSection="Input Arm";
    else if(iSection==2)    aSection="Output Arm";
    else if(iSection==3)    aSection="Empty Tray Arm";
    else if(iSection==4)    aSection="Shuttle";

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            DoSetRPDefault((TWinControl *) P, iSection);       // 找該物件附屬的物件(遞迴)
        }
        TEdit        *PEdit                     = dynamic_cast <TEdit           *>(P);
        TLabeledEdit *PLEdit                    = dynamic_cast <TLabeledEdit    *>(P);  //Steven 20200715 : 補上TLabeledEdit
        TCheckBox    *PCheckBox                 = dynamic_cast <TCheckBox       *>(P);
        TRadioGroup  *PRadioGroup               = dynamic_cast <TRadioGroup     *>(P);
        TGroupBox    *PGroupBox                 = dynamic_cast <TGroupBox       *>(P);
        TComboBox    *PComboBox                 = dynamic_cast <TComboBox       *>(P);
        TRadioButton *PRadioButton              = dynamic_cast <TRadioButton    *>(P);

        if(PEdit!=NULL && PEdit->Hint!="")
        {
            aHintName=PEdit->Hint;
            WriteIniData(szDir, aSection, aHintName, PEdit->Text);
        }
        else if(PLEdit!=NULL && PLEdit->Hint!="")
        {
            aHintName=PLEdit->Hint;
            WriteIniData(szDir, aSection, aHintName, PLEdit->Text);
        }
        else if(PCheckBox!=NULL && PCheckBox->Hint!="")
        {
            aHintName = PCheckBox->Hint;
            if(PCheckBox->Checked)      iValue=1;
            else                        iValue=0;
            WriteIniData(szDir, aSection, aHintName, iValue);
        }
        else if(PRadioGroup!=NULL && PRadioGroup->Hint!="")
        {
            aHintName = PRadioGroup->Hint;
            WriteIniData(szDir, aSection, aHintName, PRadioGroup->ItemIndex);
        }
        else if(PGroupBox!=NULL && PGroupBox->Hint!="")
        {
            aHintName = PGroupBox->Hint;
            WriteIniData(szDir, aSection, aHintName, PGroupBox->Caption);
        }
        else if(PComboBox!=NULL && PComboBox->Hint!="")
        {
            aHintName = PComboBox->Hint;
            WriteIniData(szDir, aSection, aHintName, PComboBox->ItemIndex);
        }
        else if(PRadioButton!=NULL && PRadioButton->Hint!="")
        {
            aHintName = PRadioButton->Hint;
            if(PRadioButton->Checked)       iValue=1;
            else                            iValue=0;
            WriteIniData(szDir, aSection, aHintName, iValue);
        }
    }
}
//--------------------------------------------------------------------------
void __fastcall TfSpeed::DoReplyDefaultToForm(TWinControl *PCtrl,int iSection)
{
    AnsiString aHintName;
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini";
    AnsiString aSection="";
    if(iSection==-1)        aSection="All";       //Isaac 20170718 (wei) defalut值比較功能:add trackbar default
    else if(iSection==0)    aSection="Index Arm";
    else if(iSection==1)    aSection="Input Arm";
    else if(iSection==2)    aSection="Output Arm";
    else if(iSection==3)    aSection="Empty Tray Arm";
    else if(iSection==4)    aSection="Shuttle";
    AnsiString Stringtemp = "";
    if(CosFunction.bRecipeParameterDefault==false)
        return;

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {

        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            DoReplyDefaultToForm((TWinControl *)P, iSection);       // 找該物件附屬的物件(遞迴)
        }
        TEdit       *PEdit                      = dynamic_cast <TEdit           *>(P);
        TLabeledEdit *PLEdit                    = dynamic_cast <TLabeledEdit    *>(P);  //Steven 20200715 : 補上TLabeledEdit
        TRadioGroup *PRadioGroup                = dynamic_cast <TRadioGroup     *>(P);

        if(PEdit!=NULL)
        {
            aHintName=PEdit->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                PEdit->Text=ReadIniData(szDir, aSection, aHintName, Stringtemp);
            }
        }
        else if(PLEdit!=NULL)
        {
            aHintName=PLEdit->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                PLEdit->Text=ReadIniData(szDir, aSection, aHintName, Stringtemp);
            }
        }
        else if(PRadioGroup!=NULL)
        {
            aHintName = PRadioGroup->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                PRadioGroup->ItemIndex=ReadIniData(szDir, aSection, aHintName, -1);
            }
        }
    }
}
//--------------------------------------------------------------------------
void __fastcall TfSpeed::SearchRecipeParameter(TWinControl *PCtrl,int iSection )
{
    AnsiString aHintName;
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini";
    AnsiString aSection="";
    if(iSection==-1)         aSection="All";    //Isaac 20171227 (Steven) : AutoSpeed RPD一鍵復原
    else if(iSection==0)         aSection="Index Arm";
    else if(iSection==1)    aSection="Input Arm";
    else if(iSection==2)    aSection="Output Arm";
    else if(iSection==3)    aSection="Empty Tray Arm";
    else if(iSection==4)    aSection="Shuttle";
    AnsiString Stringtemp = "";
    AnsiString asDefaultValue = "";

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            SearchRecipeParameter((TWinControl *) P, iSection);       // 找該物件附屬的物件(遞迴)
        }
        TEdit       *PEdit                      = dynamic_cast <TEdit           *>(P);
        TLabeledEdit *PLEdit                    = dynamic_cast <TLabeledEdit    *>(P);  //Steven 20200715 : 補上TLabeledEdit
        TRadioGroup *PRadioGroup                = dynamic_cast <TRadioGroup     *>(P);

        if(PEdit!=NULL)
        {
            aHintName=PEdit->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                 asDefaultValue=CheckAndReadIniData(szDir, aSection, aHintName, Stringtemp);
                if(CosFunction.bRecipeParameterDefaultLog && asDefaultValue=="")  //Sam 20201209 : Default Recipe ChangeLog
                {
                    //Default 數值被手動刪除掉的話，就不要監控了
                }
                else
                {
                    fRPDefault->RP_Name->Add(aHintName);
                    fRPDefault->RP_Value->Add(PEdit->Text);
                    fRPDefault->RP_Default->Add(asDefaultValue);
                }
            }
        }
        else if(PLEdit!=NULL)
        {
            aHintName=PLEdit->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                PLEdit->Text=ReadIniData(szDir, aSection, aHintName, Stringtemp);
            }
        }
        else if(PRadioGroup!=NULL)
        {
            aHintName=PRadioGroup->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                asDefaultValue=CheckAndReadIniData(szDir, aSection, aHintName, Stringtemp);
                if(CosFunction.bRecipeParameterDefaultLog && asDefaultValue=="")  //Sam 20201209 : Default Recipe ChangeLog
                {
                    //Default 數值被手動刪除掉的話，就不要監控了
                }
                else
                {
                    fRPDefault->RP_Name->Add(aHintName);
                    fRPDefault->RP_Value->Add(PRadioGroup->ItemIndex);
                    fRPDefault->RP_Default->Add(asDefaultValue);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
//Isaac 20180301 (Steven) Index Cycle Time Monitoring function
//==>
void __fastcall TfSpeed::edtMonitoringIndexCycletimeKeyPress(
      TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edtMonitoringOutlierKeyPress(TObject *Sender,
      char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edtMonitoringWindowKeyPress(TObject *Sender,
      char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edtMonitoringIndexCycletimeMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.01, 10.0);
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edtMonitoringOutlierMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.01, 30.0);
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edtMonitoringWindowMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 500, 1);
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edtIndexCycleTimetoleranceMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 15, true, 5, 200);  //JerryYang 20220824 : SPIL index cycle time monitor
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edtIndexCycleTimetoleranceKeyPress(
      TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
//<==
//Isaac 20180301 (Steven) Index Cycle Time Monitoring function
//------------------------------------------------------------
//kevin 20180824  儲存 檔案
//------------------------------------------------------------
void __fastcall TfSpeed::SaveSetupFile(AnsiString szDir)
{
    AnsiString str;
    AnsiString S=GetLastOpenFN();  //Steven 20151015 : change the path
    MyForceDirectories(szDir);
    szDir+="\\ArmCondition.Data";

    edIndexAccDec->Text=100;   //Steven 20091214 Add for Index ACC & DEC must be 100%

    //jou 2010-09-10 start : 增加保護speed In/Out Arm 吸嘴Retry高度不應該是正的值
    edInArmRetryMM->Text =AnsiString(CheckRange(atof(edInArmRetryMM->Text.c_str()),  -0.01, -0.3));
    edOutArmRetryMM->Text=AnsiString(CheckRange(atof(edOutArmRetryMM->Text.c_str()), -0.01, -0.3));
    //jou 2010-09-10 end

    WriteIniData(szDir, "All", "Speed",     edAllSpeed->Text);
    WriteIniData(szDir, "All", "Accel",     edAllAccSpeed->Text);
    WriteIniData(szDir, "All", "EPControl", edEPControl->Text);       //Ifor 20150710 :EP Control

    //--------------------------------------------------------------------------
    //Index Arm
    //--------------------------------------------------------------------------
    if(CUSTOMER_CODE==CC_ASE_SG)                                                //Ifor 20240913 ASE 新加坡客戶要求 X => Y
    {
        WriteIniData(szDir, "Index Arm", "Y Speed",             edIndexSpeed->Text);
        WriteIniData(szDir, "Index Arm", "Y Accel",             edIndexAccDec->Text);
    }
    else
    {
        WriteIniData(szDir, "Index Arm", "X Speed",             edIndexSpeed->Text);
        WriteIniData(szDir, "Index Arm", "X Accel",             edIndexAccDec->Text);
    }
    WriteIniData(szDir, "Index Arm", "Z Up Speed",              edIndexSpeed->Text);
    WriteIniData(szDir, "Index Arm", "Z Up Accel",              edIndexAccDec->Text);
    WriteIniData(szDir, "Index Arm", "Z Down Speed",            edIndexSpeed->Text);
    WriteIniData(szDir, "Index Arm", "Z Down Accel",            edIndexAccDec->Text);
    WriteIniData(szDir, "Index Arm", "Retry Count",             edIndexRetryCount->Text);
    WriteIniData(szDir, "Index Arm", "Retry Down",              edIndexArmRetryMM->Text);
    WriteIniData(szDir, "Index Arm", "Vacuum Check Time",       edIndexVacumCheckTime->Text);
    WriteIniData(szDir, "Index Arm", "Counter Air ON Time",     edIndexAirOnTime->Text);

    //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    WriteIniData(szDir, "Index Arm", "Destroy Again Time",      edIndexDestroyAgainTime->Text);
    WriteIniData(szDir, "Index Arm", "Destroy Again Count",     edIndexDestroyAgainCount->Text);

    WriteIniData(szDir, "Index Arm", "Vacuum Check Optimize",   (cbIndexVacuumCheckOptimize->Checked)?1:0);
    WriteIniData(szDir, "Index Arm", "Socket Check",            rgSocketCheck->ItemIndex);
    WriteIniData(szDir, "Index Arm", "Vacuum Timing",           (chkIndexPreSuck->Checked)?0:1);
    WriteIniData(szDir, "Index Arm", "Pick IC when out shuttle no IC", (chkPickAfterOutArm->Checked)?1:0);  //Steven 20221207 : Index必須在out shuttle沒料才可以吸
    //--------------------------------------------------------------------------
    //Input Arm
    //--------------------------------------------------------------------------
    WriteIniData(szDir, "Input Arm", "XY Speed",                edInXYSpd->Text);
    WriteIniData(szDir, "Input Arm", "XY Accel",                edInXYAcc->Text);

    //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    WriteIniData(szDir, "Input Arm", "Destroy Again Time",      edInArmDestroyAgainTime->Text);
    WriteIniData(szDir, "Input Arm", "Destroy Again Count",     edInArmDestroyAgainCount->Text);

    //ChungHung 20110408 Z Speed can change start
//    if(atoi(edInZSpd->Text.c_str())>atoi(edInXYSpd->Text.c_str()) || atoi(edInZSpd->Text.c_str())>atoi(edInPitchSpd->Text.c_str()))
//    {
//        edInZSpd->Text=min(atoi(edInXYSpd->Text.c_str()), atoi(edInPitchSpd->Text.c_str()));
//    }
//    if(atoi(edInZAcc->Text.c_str())>atoi(edInXYAcc->Text.c_str()) || atoi(edInZAcc->Text.c_str())>atoi(edInPitchAcc->Text.c_str()))
//    {
//        edInZAcc->Text=min(atoi(edInXYAcc->Text.c_str()), atoi(edInPitchAcc->Text.c_str()));
//    }
    //ChungHung 20110408 Z Speed can change end
    WriteIniData(szDir, "Input Arm", "Z Up Speed",              edInZSpd->Text);
    WriteIniData(szDir, "Input Arm", "Z Up Accel",              edInZAcc->Text);
    WriteIniData(szDir, "Input Arm", "Z Down Speed",            edInZSpd->Text);
    WriteIniData(szDir, "Input Arm", "Z Down Accel",            edInZAcc->Text);
    WriteIniData(szDir, "Input Arm", "Auto Speed Down",         (rgAutoSpeed->ItemIndex==0)?1:0);
    WriteIniData(szDir, "Input Arm", "AutoSpeedLow",            edAutoSpeedLow->Text);               //kevin 20210913 add low speed
    WriteIniData(szDir, "Input Arm", "Retry Count",             edInArmRetryCount->Text);
    WriteIniData(szDir, "Input Arm", "Retry Down",              edInArmRetryMM->Text);
    WriteIniData(szDir, "Input Arm", "Vacuum Check Time",       edInVacumCheckTime->Text);
    WriteIniData(szDir, "Input Arm", "Counter Air ON Time",     edInArmAirOnTime->Text);
    WriteIniData(szDir, "Input Arm", "Shuttle Wait Time",       edInArmShtWaitTime->Text);
    WriteIniData(szDir, "Input Arm", "Two Speed On Off",        rgTwoSpeedOnOff_In->ItemIndex);     //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Input Arm", "Two Speed Distance",      edTwoSpeedDistance_In->Text);       //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Input Arm", "Two Speed Precent",       edSecondSpeedIn->Text);             //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Input Arm", "Two Speed ADC",           edSecondADCIn->Text);               //Steven 20140217 : 兩段速移動

    //auto Skip enable
    WriteIniData(szDir, "Input Arm", "Auto Skip",               (rgInArmAutoSpeed->ItemIndex==0)?1:0);
    WriteIniData(szDir, "Input Arm", "Auto Skip CT",            edtAutoSkipCT->Text);

    //One by One Enable
    if(IniConfig.bI37_EnableFIFOMode && LastSet.iRunStartMode==rsmFIFOMode)       //Steven 20160303 (wei) : FIFO Mode
    {
        ;
    }
    else
    {
        WriteIniData(szDir, "Input Arm", "One by one",          (rgInArmPitch->ItemIndex==0)?0:1);
    }
    WriteIniData(szDir, "Input Arm", "Vacuum Timing",           (chkInArmPreSuck->Checked)?0:1);
    WriteIniData(szDir, "Input Arm", "Vacuum Check Optimize",   (cbInVacuumCheckOptimize->Checked)?1:0);
    WriteIniData(szDir, "Input Arm", "Open/Close Speed",        edInPitchSpd->Text);
    WriteIniData(szDir, "Input Arm", "Open/Close Accel",        edInPitchAcc->Text);

    WriteIniData(szDir, "Input Arm", "Rotate Input Speed",      edInRotSpd->Text);  //kevin 20130517 rotate add
    WriteIniData(szDir, "Input Arm", "Rotate Input Accel",      edInRotAcc->Text);  //kevin 20130517 rotate add

    //2013-08-01    Dell    modify  公司統一回吸功能
    WriteIniData(szDir, "Input Arm", "Destroy Check Time",      Edt_CheckTime_In->Text);
    WriteIniData(szDir, "Input Arm", "Destroy Pause Check",     (ckb_PauseCheck_In->Checked)?1:0);
    WriteIniData(szDir, "Input Arm", "Release Delay Time",      edRelaseDelay->Text);               //JerryYang 20160127 for TSMC inarm release device前delay
    WriteIniData(szDir, "Input Arm", "Enable Relase Delay",     rgReleaseDelayTime->ItemIndex);     //JerryYang 20160127 for TSMC inarm release device前delay
    WriteIniData(szDir, "Input Arm", "Enable Height Check",    (ckb_HeightCheck_In->Checked)?1:0);  //wei 20160825 回吸偵測
    WriteIniData(szDir, "Input Arm", "Height Check",            Edt_HeightCheck_In->Text);          //wei 20160825 回吸偵測
    WriteIniData(szDir, "Input Arm", "DevicConfirm",           (gbInarmDeviveConfirm->ItemIndex==0)?1:0); //kevin 20170510 (wei) 回吸偵測開關
    //--------------------------------------------------------------------------
    //Output Arm
    //--------------------------------------------------------------------------
    WriteIniData(szDir, "Output Arm", "XY Speed",               edOutXSpd->Text);
    WriteIniData(szDir, "Output Arm", "XY Accel",               edOutXAcc->Text);

    //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    WriteIniData(szDir, "Output Arm", "Destroy Again Time",     edOutArmDestroyAgainTime->Text);
    WriteIniData(szDir, "Output Arm", "Destroy Again Count",    edOutArmDestroyAgainCount->Text);

    //ChungHung 20110408 Z Speed can change start
    WriteIniData(szDir, "Output Arm", "Z Up Speed",             edOutZSpd->Text);
    WriteIniData(szDir, "Output Arm", "Z Up Accel",             edOutZAcc->Text);
    WriteIniData(szDir, "Output Arm", "Z Down Speed",           edOutZSpd->Text);
    WriteIniData(szDir, "Output Arm", "Z Down Accel",           edOutZAcc->Text);
    WriteIniData(szDir, "Output Arm", "Auto Speed Down",        (rgAutoSpeed->ItemIndex==0)?1:0);
    WriteIniData(szDir, "Output Arm", "AutoSpeedLow",           edAutoSpeedLow->Text);               //kevin 20210913 add low speed
    WriteIniData(szDir, "Output Arm", "Retry Count",            edOutArmRetryCount->Text);
    WriteIniData(szDir, "Output Arm", "Retry Down",             edOutArmRetryMM->Text);
    WriteIniData(szDir, "Output Arm", "Vacuum Check Time",      edOutVacumCheckTime->Text);
    WriteIniData(szDir, "Output Arm", "Counter Air ON Time",    edOutArmAirOnTime->Text);
    WriteIniData(szDir, "Output Arm", "Two Speed On Off",       rgTwoSpeedOnOff_Out->ItemIndex);   //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Output Arm", "Two Speed Distance",     edTwoSpeedDistance_Out->Text);     //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Output Arm", "Two Speed Precent",      edSecondSpeedOut->Text);           //Steven 20140217 : 兩段速移動
    WriteIniData(szDir, "Output Arm", "Two Speed ADC",          edSecondADCOut->Text);             //Steven 20140217 : 兩段速移動

    //One by One Enable
    if(IniConfig.bI37_EnableFIFOMode && LastSet.iRunStartMode==rsmFIFOMode)       //Steven 20160303 (wei) : FIFO Mode
    {
        ;
    }
    else
    {
        WriteIniData(szDir, "Output Arm", "One by one",         (rgOutArmPitch->ItemIndex==0)?0:1);
    }
    WriteIniData(szDir, "Output Arm", "Vacuum Timing",          (chkOutArmPreSuck->Checked)?0:1);
    WriteIniData(szDir, "Output Arm", "Vacuum Check Optimize",  (cbOutVacuumCheckOptimize->Checked)?1:0);
    WriteIniData(szDir, "Output Arm", "Open/Close Speed",       edOutPitchSpd->Text);
    WriteIniData(szDir, "Output Arm", "Open/Close Accel",       edOutPitchAcc->Text);
    WriteIniData(szDir, "Output Arm", "Wait before Air On",     edtOutArmPreSuck->Text);

    WriteIniData(szDir, "Output Arm", "Rotate Output Speed",    edOutRotSpd->Text);  //kevin 20130517 rotate add
    WriteIniData(szDir, "Output Arm", "Rotate Output Accel",    edOutRotAcc->Text);  //kevin 20130517 rotate add
    //2013-08-01    Dell    modify  公司統一回吸功能
    WriteIniData(szDir, "Output Arm", "Destroy Check Time",     Edt_CheckTime_Out->Text);
    WriteIniData(szDir, "Output Arm", "Destroy Pause Check",    (ckb_PauseCheck_Out->Checked)?1:0);
    WriteIniData(szDir, "Output Arm", "DevicConfirm",           (gbOutarmDeviveConfirm->ItemIndex==0)?1:0);  //kevin 20170510 (wei) 回吸偵測開關
    //--------------------------------------------------------------------------
    //Empty Tray Arm
    //--------------------------------------------------------------------------
    WriteIniData(szDir, "Empty Tray Arm", "Speed",              edTrayXSpd->Text);
    WriteIniData(szDir, "Empty Tray Arm", "Accel",              edTrayXAcc->Text);
    WriteIniData(szDir, "Empty Tray Arm", "Retry Count",        edTrayArmRetryCount->Text);
    WriteIniData(szDir, "Empty Tray Arm", "Vacuum Check Time",  edTrayVacumCheckTime->Text);
    WriteIniData(szDir, "Empty Tray Arm", "Counter Air ON Time",edTrayArmAirOnTime->Text);
    WriteIniData(szDir, "Empty Tray Arm", "Hand Down Time",     edTrayArmHandDown->Text);
    //--------------------------------------------------------------------------
    //Shuttle
    //--------------------------------------------------------------------------
    WriteIniData(szDir, "Shuttle", "Shuttle 1 Speed",           edSht1Spd->Text);
    WriteIniData(szDir, "Shuttle", "Shuttle 1 Accel",           edSht1Acc->Text);
    WriteIniData(szDir, "Shuttle", "Shuttle 2 Speed",           edSht2Spd->Text);
    WriteIniData(szDir, "Shuttle", "Shuttle 2 Accel",           edSht2Acc->Text);
    WriteIniData(szDir, "Shuttle", "Device Check Time",         edShtDeviceCheckTime->Text);
    WriteIniData(szDir, "Shuttle", "Shake Cycles",              edShakeCycles->Text);       //Sam 20250326 : 新增 Shake 條件設定
    WriteIniData(szDir, "Shuttle", "Shake distance",            edShakeDistance->Text);
    WriteIniData(szDir, "Shuttle", "Delay between shakes",      edShakeDelay->Text);
    WriteIniData(szDir, "Shuttle", "Shake Accel",               edShakeAccDec->Text);
    //step shuttle
    WriteIniData(szDir, "Shuttle", "Step Shuttle",              (rgStepShuttle->ItemIndex==0)?1:0);

    //--------------------------------------------------------------------------
    //Magazine
    //--------------------------------------------------------------------------
    WriteIniData(szDir, "Magazine", "Magazine CatchY Speed",    edMagCatchYSpd->Text);
    WriteIniData(szDir, "Magazine", "Magazine CatchY Accel",    edMagCatchYAcc->Text);
    WriteIniData(szDir, "Magazine", "MagazineZ Speed",          edMagZSpd->Text);
    WriteIniData(szDir, "Magazine", "MagazineZ Accel",          edMagZAcc->Text);
    WriteIniData(szDir, "Magazine", "Two Speed Distance",       edTwoSpeedDistanceCatchY->Text);
    WriteIniData(szDir, "Magazine", "Two Speed Precent",        edSecondSpeedCatchY->Text);
    WriteIniData(szDir, "Magazine", "Two Speed ADC",            edSecondADCCatchY->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edtLoaderSpeed1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TLabeledEdit *)Sender, N_INTEGER, 1, true, (CosFunction.bLimitMaxSpeed)?CosFunction.iLimitMaxSpeed:100, 1);  //Steven 20150420
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edInArmDieCleanDelayMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 10.0, 0.1);  //JerryYang 20160127 for TSMC inarm release device前delay
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edInArmDieCleanHeightMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CUSTOMER_CODE==CC_TSMC_TAINAN)
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 20.0, 0.1);
    else
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, -20.0, 0.1);
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edOutArmShtWaitTimeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 50.0,0);  //kevin 20210525 add out arm shuttle wait
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edAutoSpeedLowMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100, 1);
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edTwoSpeedDistanceCatchYMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 20.0, 0.01);  //Steven 20150420
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edShakeCyclesMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 1, true, 100, 1);
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edShakeDistanceMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 5, true, 30, 5);
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edShakeDelayMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 0.0, true, 2.0, 0.0);
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edShakeDelayKeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::edShakeAccDecMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 100, true, 100, 1);
}
//---------------------------------------------------------------------------
