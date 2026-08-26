#include "MachineDefine.h"
#pragma hdrstop

#include "iosetview.h"

#include "cprod.h"
#include "MyLaneIo.h"
#include "tools.h"
#include "myswitch.h"
#include "mymotor.h"
#include "cinitial.h"
#include "csystem.h"
#include "mysensor.h"
#include "cShowBinSelect.h"
#include "adam6024.h"
#include "atester.h"
#include "cpublic.h"
#include "LtcSensor.h"
#include "mycylin.h"
#include "common.h"
#include "MyKitSuck.h"
#include "note.h"
#include "mymessbox.h"
#include "cmydef.h"
#include "ShuttleMove.h"
#include "database.h"
#include "myQwertyKeyBoard.h"
#include "ProductionInfo.h"
#include "uESDControl.h"
#include "uPadInterface.h"                                                      //Ken 20210702 AddPadInterface

#include "MyPLC_IO_Modbus.h"                                                    //Austin 20190531.01 增加PLC_IO模組及安全相關IO.
#include "ModbusTCPClient.h"
#include "VacuumUnit.h"                                                         //Sam 20230210 : 新增 VacuumUnit 通訊模組
//---------------------------------------------------------------------------
#pragma package(smart_init)

extern bool g_bEnSubPage[ePLCGrp_Count];                                        //one group is means one slave for safety PLCIO
extern void InitPLCIO_All();

#pragma resource "*.dfm"
Tfiosetview *fiosetview;
bool bCheckEMG          =false;
bool bCheckMotorPower   =false;
bool bStartTTLOut       =false;
int  iTTLLoopCount      =0;
int  iTTLPulseWidth     =0;                                                     //2013-05-06    Dell
int  iNumPanelDown      =0;                                                     //Stven 20090919 : For Num Panel Test
bool bNumPanelDown      =0;                                                     //Stven 20090919 : For Num Panel Test
TMySucker *mySuckerTemp=NULL;                                                   //Steven 20110520 : 被打開的吸嘴 //AI(ht9045-v899) 20260511: init NULL to prevent dangling deref in Timer1Timer when click handler did not assign
//---------------------------------------------------------------------------
__fastcall Tfiosetview::Tfiosetview(TComponent* Owner)
     : TForm(Owner)
{
    fShow=false;

    PC_IOSET->ActivePage=tsLoader;
    palArm2_A_16->Top   =palArm2_A->Top;
    palArm2_A_16->Left  =palArm2_A->Left;
    palArm2_B_16->Top   =palArm2_B->Top;
    palArm2_B_16->Left  =palArm2_B->Left;
    palArm1_A_16->Top   =palArm1_A->Top;
    palArm1_A_16->Left  =palArm1_A->Left;
    palArm1_B_16->Top   =palArm1_B->Top;
    palArm1_B_16->Left  =palArm1_B->Left;

    btnSwHeatGun->Visible=(INSTALL_HEAT_GUN>0);
    btnSwCDAGun->Visible =(INSTALL_HEAT_GUN>1);

    Index_8[0][0][0]=palArm1_Aa_Name; Index_8[0][0][1]=palArm1_Ab_Name; Index_8[0][0][2]=palArm1_Ac_Name; Index_8[0][0][3]=palArm1_Ad_Name;
    Index_8[0][1][0]=palArm1_Ba_Name; Index_8[0][1][1]=palArm1_Bb_Name; Index_8[0][1][2]=palArm1_Bc_Name; Index_8[0][1][3]=palArm1_Bd_Name;
    Index_8[1][0][0]=palArm2_Aa_Name; Index_8[1][0][1]=palArm2_Ab_Name; Index_8[1][0][2]=palArm2_Ac_Name; Index_8[1][0][3]=palArm2_Ad_Name;
    Index_8[1][1][0]=palArm2_Ba_Name; Index_8[1][1][1]=palArm2_Bb_Name; Index_8[1][1][2]=palArm2_Bc_Name; Index_8[1][1][3]=palArm2_Bd_Name;

    Index_16[0][0][0]=palArm1_Aa_16_name; Index_16[0][0][1]=palArm1_Ab_16_name; Index_16[0][0][2]=palArm1_Ac_16_name; Index_16[0][0][3]=palArm1_Ad_16_name; Index_16[0][0][4]=palArm1_Ae_16_name; Index_16[0][0][5]=palArm1_Af_16_name; Index_16[0][0][6]=palArm1_Ag_16_name; Index_16[0][0][7]=palArm1_Ah_16_name;
    Index_16[0][1][0]=palArm1_Ba_16_name; Index_16[0][1][1]=palArm1_Bb_16_name; Index_16[0][1][2]=palArm1_Bc_16_name; Index_16[0][1][3]=palArm1_Bd_16_name; Index_16[0][1][4]=palArm1_Be_16_name; Index_16[0][1][5]=palArm1_Bf_16_name; Index_16[0][1][6]=palArm1_Bg_16_name; Index_16[0][1][7]=palArm1_Bh_16_name;
    Index_16[1][0][0]=palArm2_Aa_16_name; Index_16[1][0][1]=palArm2_Ab_16_name; Index_16[1][0][2]=palArm2_Ac_16_name; Index_16[1][0][3]=palArm2_Ad_16_name; Index_16[1][0][4]=palArm2_Ae_16_name; Index_16[1][0][5]=palArm2_Af_16_name; Index_16[1][0][6]=palArm2_Ag_16_name; Index_16[1][0][7]=palArm2_Ah_16_name;
    Index_16[1][1][0]=palArm2_Ba_16_name; Index_16[1][1][1]=palArm2_Bb_16_name; Index_16[1][1][2]=palArm2_Bc_16_name; Index_16[1][1][3]=palArm2_Bd_16_name; Index_16[1][1][4]=palArm2_Be_16_name; Index_16[1][1][5]=palArm2_Bf_16_name; Index_16[1][1][6]=palArm2_Bg_16_name; Index_16[1][1][7]=palArm2_Bh_16_name;

    InArm_8[0][0]=palInArm_A; InArm_8[0][1]=palInArm_C; InArm_8[0][2]=palInArm_E; InArm_8[0][3]=palInArm_G;
    InArm_8[1][0]=palInArm_B; InArm_8[1][1]=palInArm_D; InArm_8[1][2]=palInArm_F; InArm_8[1][3]=palInArm_H;
    OutArm_8[0][0]=palOutArm_A; OutArm_8[0][1]=palOutArm_C; OutArm_8[0][2]=palOutArm_E; OutArm_8[0][3]=palOutArm_G;
    OutArm_8[1][0]=palOutArm_B; OutArm_8[1][1]=palOutArm_D; OutArm_8[1][2]=palOutArm_F; OutArm_8[1][3]=palOutArm_H;

    InArm_16[0][0]=labInArmAa; InArm_16[0][1]=labInArmAb; InArm_16[0][2]=labInArmAc; InArm_16[0][3]=labInArmAd; InArm_16[0][4]=labInArmAe; InArm_16[0][5]=labInArmAf; InArm_16[0][6]=labInArmAg; InArm_16[0][7]=labInArmAh;
    InArm_16[1][0]=labInArmBa; InArm_16[1][1]=labInArmBb; InArm_16[1][2]=labInArmBc; InArm_16[1][3]=labInArmBd; InArm_16[1][4]=labInArmBe; InArm_16[1][5]=labInArmBf; InArm_16[1][6]=labInArmBg; InArm_16[1][7]=labInArmBh;

    OutArm_16[0][0]=labOutArmAa; OutArm_16[0][1]=labOutArmAb; OutArm_16[0][2]=labOutArmAc; OutArm_16[0][3]=labOutArmAd; OutArm_16[0][4]=labOutArmAe; OutArm_16[0][5]=labOutArmAf; OutArm_16[0][6]=labOutArmAg; OutArm_16[0][7]=labOutArmAh;
    OutArm_16[1][0]=labOutArmBa; OutArm_16[1][1]=labOutArmBb; OutArm_16[1][2]=labOutArmBc; OutArm_16[1][3]=labOutArmBd; OutArm_16[1][4]=labOutArmBe; OutArm_16[1][5]=labOutArmBf; OutArm_16[1][6]=labOutArmBg; OutArm_16[1][7]=labOutArmBh;

    Timer1->Enabled=false;
    Timer2->Enabled=false;
    LoadIoTable(0, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::Timer1Timer(TObject *Sender)
{
    static bool bTimerRun=false;
    double dReadV=0.0;
    AnsiString strEPV;
    int iHotGun=0;                                                              //KaiChen 20190729 ：Hot Gun Flow
    AnsiString asHotGun;                                                        //KaiChen 20190729 ：Hot Gun Flow
    if(fShow==false || bTimerRun==true ||
       (CUSTOMER_CODE==CC_Greatek &&
        (ESDForm->bESDFTPAction==true ||
         fProductionInfo->bOEEFTPAction==true)))                                //Sam 20190522 : 增加 timer 保護，使用ftp的時候要防止同時使用 ep 讀取會死掉
    {
        return;
    }
    bTimerRun=true;
    struct TTimerRunGuard                                                       //AI(ht9045-v899) 20260511: RAII guard - destructor always clears bTimerRun on any function exit (normal return, exception unwind, or early break), so a hardware IO exception below cannot leave reentry flag stuck true (which would freeze ScanLed and make pressed buttons stay dark)
    {
        bool *pFlag;
        TTimerRunGuard(bool *p):pFlag(p){}
        ~TTimerRunGuard(){ *pFlag=false; }
    } _timerGuard(&bTimerRun);

    if(IsIndexMotorOutOfPower())
    {
        if(bCheckEMG==false)
        {
            LockIndexMotorAndDoHomeProcess();
            bCheckEMG=true;
        }
    }
    else
    {
        bCheckEMG=false;
    }

    #ifndef SOFT_SIMULTE
        ScanLed(fiosetview);
    #else
        #ifdef SOFT_SIMULTE_EtherCAT
             ScanLed(fiosetview);                                               //Sam 20230724 : 測試 SensorEtherCAT 用
        #endif
    #endif

    if((EP_Install==3 || EP_Install==5))                                        //2012-01-03    Dell modify
       //&& (CUSTOMER_CODE!=CC_Greatek || IniConfig.bN14HandlerOEEUseFunction==false)) //Sam 20190521 : 正賢要求不要讀取，會與 OEE 的功能相衝
    {
        if(MachineTypeChoice==Type_HT1032)
        {
            labPA->Caption=ADAM_ReadPA(&dReadV, 0);                             //wei 20220309 Add EP Return Voltage
            strEPV.sprintf("%.3f", dReadV);
            labEPValue->Caption=strEPV;                                         //20111217 ChungHung

            labPA_1032->Caption=ADAM_ReadPA(&dReadV, 1);                        //wei 20220309 Add EP Return Voltage
            strEPV.sprintf("%.3f", dReadV);
            labEPValue_1032->Caption=strEPV;                                    //20111217 ChungHung
        }
        else
        {
            labPA->Caption=ADAM_ReadPA(&dReadV);                                //wei 20220309 Add EP Return Voltage
            strEPV.sprintf("%.3f", dReadV);
            labEPValue->Caption=strEPV;                                         //20111217 ChungHung
        }

        if(USE_CKD_FCM_CleanAir)                                                //Ifor 20150709
            labLoaderFlow->Caption=ADAM_ReadVoltage(1);
        if(INSTALL_DOUBLE_EP==DOUBLE_EP_NORMAL || INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)                                                //Ifor 20190104 : Add Double EP Control   //AI(ht9045-v899) 20260430: extend EP voltage display to Multi EP half.
        {
            labDOUBLEPA->Caption=ADAM_ReadPA(&dReadV, 2);                       //wei 20220309 Add EP Return Voltage
            strEPV.sprintf("%.3f", dReadV);
            labDOUBLEEPValue->Caption=strEPV;
        }
    }

    if(mySuckerTemp==NULL)                                                      //AI(ht9045-v899) 20260511: null-guard before deref; if pointer not assigned yet, force state Off to avoid AV that would leave bTimerRun stuck true and freeze UI
    {
        bChangeSuckStatus=SStatusOff;
    }
    else if(bChangeSuckStatus==SStatusSuck)                                     //手動開啟Index吸嘴
    {
        if(mySuckerTemp->Suck()==true || mySuckerTemp->Error==true)
            bChangeSuckStatus=SStatusOff;
    }
    else if(bChangeSuckStatus==SStatusDestory)                                  //手動吹氣
    {
        if(mySuckerTemp->Sensor()==true)
        {
            bChangeSuckStatus=SStatusOff;
        }
        else if(mySuckerTemp->Destroy()==true || mySuckerTemp->Error==true)
        {
            bChangeSuckStatus=SStatusOff;
        }
    }

    if(INDEX_SUCKER_TYPE==1)                                                    //jou 2010-05-24 start : 負壓
    {
        if(IndexHasIC() && bChangeSuckStatus==false)                            //Steven 20110629 : 沒有手動開啟Index吸嘴時,才要檢查
            CheckIndexSuck();
        ProcessIndexSuckDestroy1();
        ProcessIndexSuckDestroy2();
    }
    CheckIndexAllSuckICFallDown(true, true);                                    //Steven 20110725 : 修改負壓檢查方式

    if(IniConfig.bIOFormCanControlHeaterFan)                                    //jou 2010-12-03 start : ASE-KR當chamber門開啟時,需要能開關風扇
    {
        if(bHeaterDoorIsOpen[0] || bHeaterDoorIsOpen[1] ||
           bHeaterDoorIsOpen[2] || bHeaterDoorIsOpen[3])                        //Steven 20191016 : For ATC3.3, 要第三個加熱門
        {
            btnSwHeaterFan->Visible=true;
        }
        else
        {
            btnSwHeaterFan->Visible=false;
        }
    }

    if(HotGunFlowEnable)                                                        //KaiChen 20190729 ：Hot Gun Flow
    {
        iHotGun=GetAD8Value_HotGenFlow(HotGunFlow_LineNo, HotGunFlow_DevNo, HotGunFlow_Gun1_ChannelNo, "HotGun");
        asHotGun.sprintf("%d", iHotGun);
        lblGunFlow1_Value->Caption=asHotGun;
        iHotGun=GetAD8Value_HotGenFlow(HotGunFlow_LineNo, HotGunFlow_DevNo, HotGunFlow_Gun2_ChannelNo, "HotGun");
        asHotGun.sprintf("%d", iHotGun);
        lblGunFlow2_Value->Caption=asHotGun;
    }

    if(IsSafePLCIOInstall()==true)                                              //liangyou 20260607 for SafePLC//CEHCK_OK
    {
        RefreshSafePLCLed();
    }
                                                                                //AI(ht9045-v899) 20260511: bTimerRun reset is handled by _timerGuard destructor above
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Close_ADAM_6024();
    Open_ADAM_6024();                                                           //Jimmychiu 20230804 : 整合全部連線檢查
    ResetIndexDestroy();                                                        //Steven 20111101
    fShow=false;
    lbSuckEnabled->Visible=false;
    Timer1->Enabled=false;                                                      //Steven 20120131
    Timer2->Enabled=false;                                                      //Steven 20120131

    if(Cylinder[C_Shuttle_Knocker_1].Enable)
        Cylinder[C_Shuttle_Knocker_1].Off();
    if(Cylinder[C_Shuttle_Knocker_2].Enable)
        Cylinder[C_Shuttle_Knocker_2].Off();

    if(iControlPanelMode==1)                                                    //KenHsieh 20211224 : 新增按鈕狀態掃描
        fPadInterface->bScanSwitch=true;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::FormShow(TObject *Sender)
{
    bChangeSuckStatus=SStatusOff;                                               //AI(ht9045-v899) 20260511: clear residual sucker action so timer will not deref stale mySuckerTemp
    InitPairInfo_SafePLCIOLed();
    tsLoader    ->Enabled=true;                                                 //Steven 20120804 Start: 先在最上面Enable全部畫面
    tsUnLoader  ->Enabled=true;
    tsUnLoader2 ->Enabled=true;
    tsSucker    ->Enabled=true;
    tsShuttle   ->Enabled=true;
    tsKeyPad    ->Enabled=true;
    tsSystem    ->Enabled=true;
    tsIndex     ->Enabled=true;
    tsTTL       ->Enabled=true;
    tsTool      ->Enabled=true;
    tsIOTable->TabVisible=(IO_CARD_TYPE==NewIO_MN200 ||
                           IO_CARD_TYPE==PCI_P64C64);                           //Steven 20231218 HT7080B

    tsSafePLC->TabVisible=IsSafePLCIOInstall();                                 //liangyou 20260607 for SafePLC
    
    #ifdef SOFT_SIMULTE
    //soft simulate will not execute PLCIO initialization, so we have to execute one time for watch led status when soft simulate
    if(tsSafePLC->TabVisible)                                                   //Austin 20190531.01 增加PLC_IO模組及安全相關IO. //Jason 20230619 增加安全PLC部分
    {
        if(IDYES == MessageBoxW(GetForegroundWindow(),L"Safe PLCIO初始化尚未被執行,現在需要執行嗎?", L"PLCIO初始化詢問",MB_YESNO | MB_TOPMOST))
        {
            InitPLCIO_All();
        }
    }
    #endif
    
    tsSafe->TabVisible = g_bEnSubPage[ePLCGrp_Safe];

    TBtnPanelLane *tempBtn[]={btnSwBMotorBreaker, btnSwFMotorBreaker, btnSwMotorRelay, btnSwServerON,
                              btnIndexArm1SuckMode1, btnIndexArm1SuckMode2,
                              btnIndexArm2SuckMode1, btnIndexArm2SuckMode2};

//    btnTool->Visible=(CUSTOMER_CODE==CC_HONPREC_QC);                            //Steven 20230707 : Tools只能在QC模式下使用
    if(MachineTypeChoice==Type_HT9045)                                          //Steven 20240207 : 修改真空顯示方式
    {
        palArm2_A_16->Visible=false;
        palArm2_B_16->Visible=false;
        palArm1_A_16->Visible=false;
        palArm1_B_16->Visible=false;
    }
    else
    {
        if(MachineTypeChoice==Type_HT9045_12Site)
        {
            palArm2_Ag_16->Visible=false;
            palArm2_Ah_16->Visible=false;
            palArm2_Bg_16->Visible=false;
            palArm2_Bh_16->Visible=false;
            palArm1_Ag_16->Visible=false;
            palArm1_Ah_16->Visible=false;
            palArm1_Bg_16->Visible=false;
            palArm1_Bh_16->Visible=false;
        }
        palArm2_A->Visible=false;
        palArm2_B->Visible=false;
        palArm1_A->Visible=false;
        palArm1_B->Visible=false;
    }
    palTTL8Site->Visible=CosFunction.bTTLCanUse8Site;                           //Steven 20161011 (wei) : TTL支援8Site

    if(AUTO_EMPTY_COLOR!=0)
    {
        grpManual->Visible=false;
    }
    else
    {
        grpEmpty->Visible=false;
        grpColor->Visible=false;
    }

    grpLoader2_Under->Visible=(USE_2nd_LOADER!=eartUninstall);                  //Steven 20240822 : For HT-9046AU
    grpLoader2      ->Visible=(USE_2nd_LOADER!=eartUninstall);
    pnlOutArm2Suck  ->Visible=(USE_OUT_SORT_ARM!=eartUninstall);

    tsUnLoader2->TabVisible =(AUTO_EMPTY_COLOR>=3);                             //Steven 20230907 : For HT-9011UC
    grpAuto6->Visible       =(AUTO_EMPTY_COLOR==4);
    grpAuto6_Under->Visible =(AUTO_EMPTY_COLOR==4);

    grpAOI->Visible=(USE_AOI_Inspection || USE_Fix_AI_CCD);                     //Steven 20140909 : AOI沒用到不要出現
    gbTopBottomAOI->Visible=(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall);//Jimmychiu 20240322 : Top & Bottom Inspect

    SW[SwFMotorBreaker].Off();                                                  //Kenhsieh 20210923 ： 防止Index下墜並記錄
    SW[SwBMotorBreaker].Off();

    AnsiString strPos="", strEncoderPos="";
    int iTestZ1Pos, iTestZ2Pos, iTestY1Pos, iTestY2Pos;
    int iTestZ1EncoderPos, iTestZ2EncoderPos, iTestY1EncoderPos, iTestY2EncoderPos;

    iTestZ1Pos=MOT[MTestZ1].Gali_ReadPos();
    iTestZ2Pos=MOT[MTestZ2].Gali_ReadPos();
    iTestY1Pos=MOT[MTestY1].Gali_ReadPos();
    iTestY2Pos=MOT[MTestY2].Gali_ReadPos();
    iTestZ1EncoderPos=MOT[MTestZ1].Gali_ReadEncoderPos();
    iTestZ2EncoderPos=MOT[MTestZ2].Gali_ReadEncoderPos();
    iTestY1EncoderPos=MOT[MTestY1].Gali_ReadEncoderPos();
    iTestY2EncoderPos=MOT[MTestY2].Gali_ReadEncoderPos();

    btnSwInArmZBreaker->Visible=(USE_PICKER_COUNT==ep1Picker);
    btnSwOutArmZBreaker->Visible=(USE_PICKER_COUNT==ep1Picker);

    strPos.sprintf("TestZ1: %d, iTestZ2: %d, iTestY1: %d, iTestY2: %d", iTestZ1Pos, iTestZ2Pos, iTestY1Pos, iTestY2Pos);
    strEncoderPos.sprintf("TestZ1(Encoder): %d, iTestZ2(Encoder): %d, iTestY1(Encoder): %d, iTestY2(Encoder): %d", iTestZ1EncoderPos, iTestZ2EncoderPos, iTestY1EncoderPos, iTestY2EncoderPos);

    NewRecordProcess("", strPos, "");
    NewRecordProcess("", strEncoderPos, "");

    if(MachineTypeChoice==Type_HT9045 || MachineTypeChoice==Type_HT9045_12Site)                                         //ChungHung 20130507 add HT9045 updata for 12site 517
    {
        //In shuttle
        ledSnInPutSHT2S8->Visible=false;
        ledSnInPutSHT1S8->Visible=false;
        ledSnInPutSHT2S7->Visible=(IN_SHT_LAST_SENSOR!=1);                      //Steven 20181203 : In Shuttle最後一個Sensor定義
        ledSnInPutSHT1S7->Visible=(IN_SHT_LAST_SENSOR!=1);                      //Steven 20181203 : In Shuttle最後一個Sensor定義
        ledSnInPutSHT2S9->Visible=(IN_SHT_LAST_SENSOR==1);                      //Steven 20181203 : In Shuttle最後一個Sensor定義
        ledSnInPutSHT1S9->Visible=(IN_SHT_LAST_SENSOR==1);                      //Steven 20181203 : In Shuttle最後一個Sensor定義
        //Out shuttle
        if(ENABLE_OUT_SHUTTLE_SENEOR==false)                                    //Steven 20120531 : by pass out shuttle sensor
        {
            palOutShuttleSensor2->Visible=false;
            palOutShuttleSensor1->Visible=false;
        }
        else
        {
            palOutShuttleSensor2->Visible=true;
            palOutShuttleSensor1->Visible=true;
        }
    }
    else
    {
        //In shuttle
        ledSnInPutSHT2S8->Visible=true;
        ledSnInPutSHT1S8->Visible=true;
        ledSnInPutSHT2S9->Visible=true;
        ledSnInPutSHT1S9->Visible=true;
        //Out shuttle
        palOutShuttleSensor2->Visible=false;
        palOutShuttleSensor1->Visible=false;
    }

    bStartTTLOut=false;
    tsTool->TabVisible=(AccessLevel>=iDefSupervisorLevel)?true:false;           //jou 2014-06-19 Security Have 5 Level 2->iDefSupervisorLevel
    SetCompomentIO(fiosetview);
    SetCompomentHint(fiosetview);
    SetPanelElable();

    bOutDataChange=false;

    bCheckEMG=IsIndexMotorOutOfPower();

    if(TestIF_File.iTestMode<_8Site2X4)                                         //Steven 20110520
    {
        chkShowIndexAll->Visible=true;
        chkShowIndexAll->Checked=false;
    }
    else
    {
        chkShowIndexAll->Visible=false;
        chkShowIndexAll->Checked=false;
    }

    ShowSuckMode(TestIF_File.iTestMode);
    ShowShuttleSensor();
    fShow=true;

    if(CUSTOMER_CODE==CC_Greatek)                                               //Sam 20181113 : 增加獨立按鍵可以關閉視窗
    {
        btnClose->Visible=true;
    }

    if(btnSwCCDLight->Down)
        SW[SwCCDLight].On();
    else
        SW[SwCCDLight].Off();

    tbarIndexEP->Position=0;                                                    //Steven 20091027 Start : Check EP

    if(EP_Install==5)
        tbarIndexEP2->Position=0;

    if(IndexHasIC())
    {
        bplEpSwitch1->Enabled=false;                                            //Steven 20110708
        bplEpSwitch2->Enabled=false;                                            //Steven 20110708
        tbarIndexEP->Enabled=false;
        for(int k=0; k<8; k++)
            tempBtn[k]->Enabled=false;
    }
    else
    {
        if(EP_Install>0)
        {
            tbarIndexEP->Max =4095;
            tbarIndexEP->Enabled=true;
            bplEpSwitch1->Enabled=true;                                         //Steven 20110708
            bplEpSwitch2->Enabled=true;                                         //Steven 20110708
            ADAM_WriteVoltage(0);

            if(EP_Install==2)
            {
                tbarIndexEP->Max=1022;
            }

            if(USE_CKD_FCM_CleanAir)
            {
                ADAM_DirectWriteData(0, 1);                                     //Ifor 20150710 :寫入資料到ADAM (資料,裝置位置)
            }
        }
        else
        {
            tbarIndexEP->Enabled=false;
            bplEpSwitch1->Enabled=false;                                        //Steven 20110708
            bplEpSwitch2->Enabled=false;                                        //Steven 20110708
        }
        for(int k=0; k<8; k++)                                                  //當Index有IC時,要限制部分按鈕不可以被按下
            tempBtn[k]->Enabled=true;
    }

    btnSwTjSignal01->Enabled=true;
    btnSwTjSignal02->Enabled=true;
    btnSwTjSignal03->Enabled=true;
    btnSwTjSignal04->Enabled=true;
    btnSwTjSignal05->Enabled=true;
    btnSwTjSignal06->Enabled=true;
    btnSwTjSignal07->Enabled=true;
    btnSwTjSignal08->Enabled=true;
    btnSwAirOff->Visible=SW[SwAirOff].Enable;                                   //Steven 20230130 : Power saving for vacuum pump

    if(USE_CKD_FCM_CleanAir)
    {
        tbarLoaderEP->Max =4095;
        grpLoaderEP->Visible=true;                                              //Ifor 20150710 :安裝CKD 才顯示
        tbarLoaderEP->Position=0;
        lblLoadEpAlarm->Caption=tbarLoaderEP->Position;
    }
    else
    {
        grpLoaderEP->Visible=false;                                             //Ifor 20150710 :安裝CKD 才顯示
    }
    btnSwLoaderAirClean_U->Visible  =SW[SwLoaderAirClean].Enable;
    btnSwLoaderAirClean->Visible    =SW[SwLoaderAirClean].Enable;

    btnSwLoad2AirClean_U->Visible  =SW[SwLoad2AirClean].Enable;                 //RogerYang 20250409 for 9046AU
    btnSwLoad2AirClean->Visible    =SW[SwLoad2AirClean].Enable;

    #ifdef SOFT_SIMULTE
    //為了手冊撰寫須全部顯示出來
        btnSwTesterAirCooling       ->Visible = true;                           //jou 2016-01-07 Tester Air Cooling Switch
        bplEpSwitch1                ->Visible = true;
        bplEpSwitch2                ->Visible = true;
        btnSwDutHeaterCoolFan       ->Visible = true;
        btnSwShuttleCooling         ->Visible = true;
        gbRealCCD                   ->Visible = true;
        grpATCAlarm                 ->Visible = true;                           //jou 2012-03-08 Enable ATC mode;
        btnpnlnIndexIONFan          ->Visible = true;                           //jou 2012-03-08 Enable ATC mode;
        myldlnSenInArmYPitch        ->Visible = true;
        lblSenInArmYPitch60         ->Visible = true;
        myldlnSenOutArmYPitch60     ->Visible = true;
        lblSenOutArmYPitch60        ->Visible = true;
        bplFanDirection             ->Visible = true;                           //wei 20160215大風扇方向
        gbSLKClamp                  ->Visible = true;                           //JerryYang 20160803 分離測試相關IO
        gbSocketClamp               ->Visible = true;                           //JerryYang 20160803 分離測試相關IO
        btnSwIonFanClean            ->Visible = true;                           //Isaac 20210609 : IO觸發IonFan清針
        labIonFanClean              ->Visible = true;                           //Isaac 20210609 : IO觸發IonFan清針
        gbATCTJSwitch               ->Visible = true;
    #else
        btnSwTesterAirCooling       ->Visible = SW[SwTesterAirCooling].Enable;  //jou 2016-01-07 Tester Air Cooling Switch
        bplEpSwitch1                ->Visible = SW[SwEpArm1].Enable;            //Steven 20110708
        bplEpSwitch2                ->Visible = SW[SwEpArm2].Enable;            //Steven 20110708
        btnSwDutHeaterCoolFan       ->Visible = SW[SwDutHeaterCoolFan].Enable;
        btnSwShuttleCooling         ->Visible = SW[SwShuttleCooling].Enable;
        btnSwAirConditioner         ->Visible = SW[SwHotplateCooling].Enable;   //jou 2013-11-07
        gbRealCCD                   ->Visible = REAL_TIME_CCD;                  //Steven 20110705
        grpATCAlarm                 ->Visible = (ATC_SYSTEM==eATCSiliconType || ATC_SYSTEM==eWinWay) && Temperature.bATCActiveCooling==true;                    //jou 2012-03-08 Enable ATC mode;  //Jimmychiu 20210906
        grpATC70                    ->Visible = (ATC_SYSTEM!=eNonChamber && ATC_SYSTEM!=eATCSiliconType && ATC_SYSTEM!=eATCUninstall);
//        btnpnlnIndexIONFan          ->Visible = (ATC_SYSTEM!=eATCUninstall);  //jou 2012-03-08 Enable ATC mode;
        btnpnlnIndexIONFan          ->Visible = (ATC_SYSTEM>eATC60);            //2014-05-30    Dell    for ATC6.0
        myldlnSenInArmYPitch        ->Visible = (USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635);
        lblSenInArmYPitch60         ->Visible = (USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635);
        myldlnSenOutArmYPitch60     ->Visible = (USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635);
        lblSenOutArmYPitch60        ->Visible = (USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635);
        lblFix3Lock                 ->Visible = (FIX3_FULL_PLACE==Fix3K_UseCylinder46LA);                               //Steven 20140512 : For HT-9046LA
        ledSnFix3Lock               ->Visible = (FIX3_FULL_PLACE==Fix3K_UseCylinder46LA);                               //Steven 20140512 : For HT-9046LA
        btnC_FixTray_FullPlace      ->Visible = (FIX3_FULL_PLACE==Fix3K_UseCylinder || FIX3_FULL_PLACE==Fix3K_UseCylinder46LA);                                 //Steven 20140512 : For HT-9046LA
        ledC_FixTray_FullPlace_On   ->Visible = (FIX3_FULL_PLACE==Fix3K_UseCylinder || FIX3_FULL_PLACE==Fix3K_UseCylinder46LA);                                 //Steven 20140512 : For HT-9046LA
        ledC_FixTray_FullPlace_Off  ->Visible = (FIX3_FULL_PLACE==Fix3K_UseCylinder || FIX3_FULL_PLACE==Fix3K_UseCylinder46LA);                                 //Steven 20140512 : For HT-9046LA
        labFix3FullPlace            ->Visible = (FIX3_FULL_PLACE==Fix3K_ShortShuttle || FIX3_FULL_PLACE==Fix3K_UseStepperMotor);                                //Steven 20221122 : Add Fix3 Motor
        ledFix3FullPlace            ->Visible = (FIX3_FULL_PLACE==Fix3K_ShortShuttle || FIX3_FULL_PLACE==Fix3K_UseStepperMotor);                                //Steven 20221122 : Add Fix3 Motor
        gbSLKClamp                  ->Visible = (INSTALL_SOCKET_CLAMP);         //JerryYang 20160803 分離測試相關IO
        gbSocketClamp               ->Visible = (INSTALL_SOCKET_CLAMP);         //JerryYang 20160803 分離測試相關IO
        ledSnLoaderIsPreAlarm       ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);          //wei 20170802 Pre alarm sensor
        ledSnEmptyIsPreAlarm        ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);          //wei 20170802 Pre alarm sensor
        ledSnColorIsPreAlarm        ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);          //wei 20170802 Pre alarm sensor
        ledSnAuto1IsPreAlarm        ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);          //wei 20170802 Pre alarm sensor
        ledSnAuto2IsPreAlarm        ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);          //wei 20170802 Pre alarm sensor
        ledSnAuto3IsPreAlarm        ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);          //wei 20170802 Pre alarm sensor
        ledSnAuto4IsPreAlarm        ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);
        ledSnAuto5IsPreAlarm        ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);
        ledSnAuto6IsPreAlarm        ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);
        ledSnLoaderIsPreAlarm_U     ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);
        ledSnEmptyIsPreAlarm_U      ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);
        ledSnColorIsPreAlarm_U      ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);
        ledSnAuto1IsPreAlarm_U      ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);
        ledSnAuto2IsPreAlarm_U      ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);
        ledSnAuto3IsPreAlarm_U      ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);
        ledSnAuto4IsPreAlarm_U      ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);
        ledSnAuto5IsPreAlarm_U      ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);
        ledSnAuto6IsPreAlarm_U      ->Visible = (CUSTOMER_CODE==CC_KYEC_CHEN || CUSTOMER_CODE==CC_HONPREC_QC);
        gbOCR                       ->Visible = (INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR);
        gbDoubleEP                  ->Visible = (INSTALL_DOUBLE_EP==1 || INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI);            //Ifor 20190104 : Add Double EP Control //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合
        grpArm2EP                   ->Visible = (EP_Install==5);
        btnSwIonFanClean            ->Visible = ((USE_AutoCleanIonFan && SW[SwIonFanClean].Enable==true) || CUSTOMER_CODE==CC_HONPREC_QC);                      //Isaac 20210609 : IO觸發IonFan清針
        labIonFanClean              ->Visible = ((USE_AutoCleanIonFan && SW[SwIonFanClean].Enable==true) || CUSTOMER_CODE==CC_HONPREC_QC);                      //Isaac 20210609 : IO觸發IonFan清針
        gbATCTJSwitch               ->Visible = (SW[SwTjSignal01].Enable || SW[SwTjSignal02].Enable || SW[SwTjSignal05].Enable || SW[SwTjSignal06].Enable);
        btnSwTjSignal01             ->Visible = (SW[SwTjSignal01].Enable);
        btnSwTjSignal02             ->Visible = (SW[SwTjSignal02].Enable);
        btnSwTjSignal03             ->Visible = (SW[SwTjSignal03].Enable);
        btnSwTjSignal04             ->Visible = (SW[SwTjSignal04].Enable);
        btnSwTjSignal05             ->Visible = (SW[SwTjSignal05].Enable);
        btnSwTjSignal06             ->Visible = (SW[SwTjSignal06].Enable);
        btnSwTjSignal07             ->Visible = (SW[SwTjSignal07].Enable);
        btnSwTjSignal08             ->Visible = (SW[SwTjSignal08].Enable);
        pnlFix45                    ->Visible = (AUTO_EMPTY_COLOR>=3);          //Steven 20230907 : For HT-9011UC
    #endif

    gbIonFanPower->Visible=(ION_FAN_TYPE==e2IoforOne);
    if(ION_FAN_TYPE==e1IOforOne || ION_FAN_TYPE==e2IoforOne)
    {
        lblIonFan01->Caption="Ion Fan 1";
        lblIonFan02->Caption="Ion Fan 2";
        lblIonFan03->Caption="Ion Fan 3";
        lblIonFan04->Caption="Ion Fan 4";
        lblIonFan05->Caption="Ion Fan 5";
        lblIonFan06->Visible=true;
        lblIonFan07->Visible=true;
        lblIonFan08->Visible=true;
        lblIonFan09->Visible=true;
        lblIonFan10->Visible=true;
        lblIonFan11->Visible=true;
        ledIonFan06->Visible=true;
        ledIonFan07->Visible=true;
        ledIonFan08->Visible=true;
        ledIonFan09->Visible=true;
        ledIonFan10->Visible=true;
        ledIonFan11->Visible=true;
    }
    else if(ION_FAN_TYPE==eUnInstallIonFan)
    {
        grpIonFanStatus->Visible=false;
    }
    else if(ION_FAN_TYPE==e5IOforAll)
    {
        lblIonFan01->Caption="IonFanAlarm";
        lblIonFan02->Caption="IonFanLevelAlarm";
        lblIonFan03->Caption="IonBarrierAlarm";
        lblIonFan04->Caption="IonBarrierLevelAlarm";
        lblIonFan05->Caption="IonBarrierConditionAlarm";
        lblIonFan06->Visible=false;
        lblIonFan07->Visible=false;
        lblIonFan08->Visible=false;
        lblIonFan09->Visible=false;
        lblIonFan10->Visible=false;
        lblIonFan11->Visible=false;
        ledIonFan06->Visible=false;
        ledIonFan07->Visible=false;
        ledIonFan08->Visible=false;
        ledIonFan09->Visible=false;
        ledIonFan10->Visible=false;
        ledIonFan11->Visible=false;
    }
    ledIonFan13->Visible=false;
    ledIonFan14->Visible=false;
    ledSnSystemPower->Visible=false;

    bool bUseArtCatchTray=(Cylinder[C_TrayCover].Enable ||
                           USE_CATCH_TRAY_MODEL==2 ||
                           USE_CATCH_TRAY_MODEL==3 ||
                           USE_CATCH_TRAY_MODEL==4);                            //JerryYang 20190919 : 支援ART軌道+tray arm左右夾

    if(TRAY_ARM_MODE==eUnderCoveyor)                                            //Frank 20230419
    {
        tsUnderArm->TabVisible          =true;
        tsRTArm->TabVisible             =false;
        tsTrayArm->TabVisible           =false;
        bplC_TrayX_UpDown->Visible      =false;
        ledC_TrayX_UpDown_Off->Visible  =false;
        lbTrayArmSafePos->Visible       =false;
        ledSnTrayArmSafePos->Visible    =false;
    }
    else if(IniConfig.bC03UseCatchTray==true)                                   //ChungHung 20150126 ATK Catch Tray do not modify          //wei 20150213    Cylinder[C_TrayCover].Enable 一直是true 所以不會顯示夾Tray氣缸
    {
        tsUnderArm->TabVisible          =false;
        tsRTArm->TabVisible             =bUseArtCatchTray;                      //Steven 20140929 : 修正Tray Arm IO顯示
        tsTrayArm->TabVisible           =!bUseArtCatchTray;                     //Steven 20140929 : 修正Tray Arm IO顯示
        bplC_CatchTray_Fix->Visible     =!bUseArtCatchTray;                     //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
        btnC_CatchTray_FixOn->Visible   =bUseArtCatchTray;                      //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
        btnC_CatchTray_FixOff->Visible  =bUseArtCatchTray;                      //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
        ledC_CatchTray_Fix_On->Visible  =!bUseArtCatchTray;                     //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
        ledC_CatchTray_Fix_Off->Visible =!bUseArtCatchTray;                     //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
        bplC_TurnTrayArm->Visible       =bUseArtCatchTray;                      //ChungHung 20140701 add AutoRetest CatchTray use 2 Output
        ledC_CatchTray_FixOn_On->Visible=bUseArtCatchTray;                      //ChungHung 20140701 add AutoRetest CatchTray use 2 Output
        ledC_CatchTray_FixOff_On->Visible=bUseArtCatchTray;                     //ChungHung 20140701 add AutoRetest CatchTray use 2 Output
        btnC_TurnTrayArmLock->Visible   =bUseArtCatchTray;                      //ChungHung 20140814 add AutoRetest
        ledC_TurnTrayArm_On->Visible    =bUseArtCatchTray;                      //ChungHung 20140814 add AutoRetest

        myldlnSnCatchTrayFix1On->Visible=(USE_CATCH_TRAY_MODEL==3);             //Steven 20170623 (wei) : Add for catch tray with cover
        myldlnSnCatchTrayFix2On->Visible=(USE_CATCH_TRAY_MODEL==3);             //Steven 20170623 (wei) : Add for catch tray with cover
        ledSnTrayCover         ->Visible=(USE_CATCH_TRAY_MODEL==3);             //Steven 20170623 (wei) : Add for catch tray with cover
        bplC_TrayCover->Visible         =(USE_CATCH_TRAY_MODEL==2);             //Steven 20140409 : Auto Retest
        ledC_TrayCover_On->Visible      =(USE_CATCH_TRAY_MODEL==2);             //Steven 20140409 : Auto Retest

        ledSnCatchTrayFix1On->Visible   =true;
        ledSnCatchTrayFix2On->Visible   =true;
        palTrayArm->Visible             =false;
        bplCatchSuck_On->Visible        =false;
        bplCatchSuck_Off->Visible       =false;
        ledCatchSuck->Visible           =false;
    }
    else
    {
        tsUnderArm->TabVisible          =false;
        tsTrayArm->TabVisible           =true;                                  //Steven 20140929 : 修正Tray Arm IO顯示
        tsRTArm->TabVisible             =false;                                 //Steven 20140929 : 修正Tray Arm IO顯示
        bplC_CatchTray_Fix->Visible     =false;
        ledC_CatchTray_Fix_On->Visible  =false;
        ledC_CatchTray_Fix_Off->Visible =false;
        ledSnCatchTrayFix1On->Visible   =false;
        ledSnCatchTrayFix2On->Visible   =false;
        bplC_TurnTrayArm->Visible       =false;                                 //ChungHung 20140701 add AutoRetest CatchTray use 2 Output
        btnC_TurnTrayArmLock->Visible   =false;                                 //ChungHung 20140814 add AutoRetest
        ledC_TurnTrayArm_On->Visible    =false;                                 //ChungHung 20140814 add AutoRetest
        palTrayArm->Visible             =true;
        bplCatchSuck_On->Visible        =true;
        bplCatchSuck_Off->Visible       =true;
        ledCatchSuck->Visible           =true;
        bplC_TrayCover->Visible         =(USE_AUTO_RETEST==eartInstall);        //Steven 20140409 : Auto Retest
        ledC_TrayCover_On->Visible      =(USE_AUTO_RETEST==eartInstall);        //Steven 20140409 : Auto Retest
        ledSnTrayCover->Visible         =false;                                 //Steven 20170623 (wei) : Add for catch tray with cover
    }

    grpShtRotate->Visible=(IniConfig.bRotateShNeedCheck && IniConfig.bHaveRotateShuttle);

    if(NUMBER_PANEL_TYPE==2)                                                    //Steven 20100512 : 雙色七段顯示器,用通訊低!
    {
        btnSwLoaderBin->Visible =true;
        btnSwEmpty1Bin->Visible =true;
        btnSwEmpty2Bin->Visible =true;
        btnSwAuto1Bin->Visible  =true;
        btnSwAuto2Bin->Visible  =true;
        btnSwAuto3Bin->Visible  =true;
        btnSwFix1Bin->Visible   =true;
        btnSwFix2Bin->Visible   =true;
        btnSwFix3Bin->Visible   =true;
        btnSwFix4Bin->Visible   =true;
        btnSwFix5Bin->Visible   =true;
        btnSwFix6Bin->Visible   =true;
    }

    if(CUSTOMER_CODE==CC_HONPREC_QC)
    {
        btnAllVacuum->Enabled   =true;
        btnAllDestory->Enabled  =true;
        btnAllVacuum->Visible   =true;
        btnAllDestory->Visible  =true;
        grpShtRotate->Visible   =true;
    }

    if(INDEX_SUCKER_TYPE==1)
    {
        ledNegAir1->Visible=true;
        lblNegAir1->Visible=true;
        if(Sen[SnNegativePressureAir2].Enable==false)                           //Sam 20171110 (Steven) : 新增氣壓 Sensor
        {
            ledNegAir2->Visible=false;
            lblNegAir2->Visible=false;
        }
        else
        {
            ledNegAir2->Visible=true;
            lblNegAir2->Visible=true;
        }
    }
    else
    {
        ledNegAir1->Visible=false;
        lblNegAir1->Visible=false;
        ledNegAir2->Visible=false;
        lblNegAir2->Visible=false;
    }

    grpTesterDryAir->Visible=Sen[SnTesterDryAir].Enable;                        //Ifor 20200115 : add Tester Dry Air Control

    ResetIndexSuck();
    ResetIndexDestroy();
    Left=75;                                                                    //Steven 20091103
    Top=10;                                                                     //Steven 20091103
    if(SYN_TEK_MOTION_MODULE==G9004_M204)
        btnLatchCheck->Visible=true;
    else
        btnLatchCheck->Visible=false;

    bChangeSuckStatus=SStatusOff;                                               //Steven 20110520

    LabSiteMap();
    bplC_HotplateVibration->Visible=(Cylinder[C_HotplateVibration].Enable==true);                                       //jou 2011-08-09 start : Hotplate也要敲敲敲

    grpIndexChangeToque->Visible=USE_IO_CHANGE_TOQUE;                           //jou 2012-06-21 Enable index I/O Change Toque

    btnSwCCDCooling->Visible=(REAL_TIME_CCD || ATC_SYSTEM>eATC60);              //2014-05-30    Dell    for ATC6.0

    if(REAL_TIME_CCD)
    {
        btnSwCCDCooling->Caption="CCD Cooling";
    }
    else
    {
        btnSwCCDCooling->Caption="Ion Air";
    }

    gbSocketSensor->Visible=IniConfig.bC08_SocketSensor;                        //wei 20150429  SocketSensor io 顯示

    gbOneTouchDocking->Visible  =(USE_OTD==1?true:false);                       //Steven 20140312 : OTD
    gbOneTouchDocking_2->Visible=(USE_OTD==2?true:false);                       //ChungHung 20140709 add by hardware control
    gbOneTouchDocking_3->Visible=((USE_OTD==0 && IniConfig.bA05UseAutoDocking)?true:false);                             //ChungHung 20140709 add by hardware control

    Timer1->Enabled=true;                                                       //Steven 20120131
    Timer2->Enabled=true;                                                       //Steven 20120131

    btnSwTesterPower->Visible=SW[SwTesterPower].Enable;                         //jou 2012-07-24 Tester Power

    grpGroundMan->Visible=Sen[SnGroundMan].Enable;                              //wei 20150424 add SnGroundMan偵測

    //Steven 20120804 Start: 在最下面決定權限能不能Enable
    tsLoader    ->Enabled=(AccessLevel<LevelSet.AccessLevel[58])?false:tsLoader  ->Enabled;
    tsUnLoader  ->Enabled=(AccessLevel<LevelSet.AccessLevel[59])?false:tsUnLoader->Enabled;
    tsUnLoader2 ->Enabled=(AccessLevel<LevelSet.AccessLevel[59])?false:tsUnLoader2->Enabled;
    tsSucker    ->Enabled=(AccessLevel<LevelSet.AccessLevel[60])?false:tsSucker  ->Enabled;
    tsShuttle   ->Enabled=(AccessLevel<LevelSet.AccessLevel[61])?false:tsShuttle ->Enabled;
    tsKeyPad    ->Enabled=(AccessLevel<LevelSet.AccessLevel[62])?false:tsKeyPad  ->Enabled;
    tsSystem    ->Enabled=(AccessLevel<LevelSet.AccessLevel[63])?false:tsSystem  ->Enabled;
    tsIndex     ->Enabled=(AccessLevel<LevelSet.AccessLevel[64])?false:tsIndex   ->Enabled;
    tsTTL       ->Enabled=(AccessLevel<LevelSet.AccessLevel[65])?false:tsTTL     ->Enabled;
    tsTool      ->Enabled=(AccessLevel<LevelSet.AccessLevel[66])?false:tsTool    ->Enabled;
    //Steven 20120804 End: 在最下面決定權限能不能Enable

    Hide_1032_IO();                                                             //Ztex 2023.04.13 Add HT-1032 IO
    grpTesterDocking->Visible=IniConfig.bEnable_SECS_GEM;
    gbRotateKIT->Visible=(USE_ROTATE_KIT==1);                                   //wei 20150901 顯示ROTATE sensor

    gbPreciser->Visible=USE_PRECISER;                                           //Frank 20180410 (Steven) : InArm Preciser Station

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin20150707
    {
        lblSenAutoDocking360KG->Caption     ="480KG" ;
        lblC_DockXAxisOff_On_360->Caption   ="480KG OFF" ;
        lblC_DockXAxisOn_On_360->Caption    ="480KG ON" ;
        ASEBuffer->Visible                  =true;                              //kevin 20220709
    }

    //AI(ht9045-v899) 20260506: 500KG OTD label rename for all customers. Caption only; alias / Start interlock / Event log unchanged. (CASE-20260506-001)
    if(INDEX_PRESS_TYPE==e500KG && USE_OTD==2)
    {
        lblSenAutoDocking240KG->Caption     ="500KG Y" ;
        lblSenAutoDocking360KG->Caption     ="500KG X" ;
        lblC_DockYAxisOn_On_240->Caption    ="500KG Y ON" ;
        lblC_DockYAxisOff_On_240->Caption   ="500KG Y OFF" ;
        lblC_DockXAxisOn_On_360->Caption    ="500KG X ON" ;
        lblC_DockXAxisOff_On_360->Caption   ="500KG X OFF" ;
    }

    if(IniConfig.bP35TrayArm)                                                   //kevin 20171006 (wei) tray arm home 需遮住sensor
    {
        ledSnTrayArmSafePos->Visible=true;                                      //kevin 20171006 tray arm safe pos
        lbTrayArmSafePos->Visible   =true;
    }

    grpWaterLeakageSensor->Visible  =(ATC_SYSTEM!=eATCUninstall);               //wei 20190617 漏水檢測
    gbAOA->Visible                  =MACHINE_HAS_AUTO_ALIGNMENT_CCD;            //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
    btIPSetting->Click();
    grpHotGunFlow->Visible=HotGunFlowEnable;                                    //KaiChen 20190729 ：Hot Gun Flow
    //==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin
    if(INSTALL_DOUBLE_EP==1 || INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)    //kevin 20200325: Add Double EP Control
    {
        gbDoubleEP->Visible=true;
    }
    //<== //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 end

    lblLoaderZ->Visible=false;                                                  //Steven 20200724 : 修改Tray Z馬達速度設定
    lblEmptyZ->Visible=false;
    lblColorZ->Visible=false;
    lblAuto1Z->Visible=false;
    lblAuto2Z->Visible=false;
    lblAuto3Z->Visible=false;
    lblAuto4Z->Visible=false;
    lblAuto5Z->Visible=false;
    lblAuto6Z->Visible=false;
    lblLoaderZ_U->Visible=false;
    lblEmptyZ_U->Visible=false;
    lblColorZ_U->Visible=false;
    lblAuto1Z_U->Visible=false;
    lblAuto2Z_U->Visible=false;
    lblAuto3Z_U->Visible=false;
    lblAuto4Z_U->Visible=false;
    lblAuto5Z_U->Visible=false;
    lblAuto6Z_U->Visible=false;

    if(LOAD_Z_USE_MOTOR[0]==false)
    {
        btnC_Load_Middle->OnClick       =BtnPanelClick;
        btnC_Load_Up->OnClick           =BtnPanelClick;
        btnC_Load_Middle_U->OnClick     =BtnPanelClick;
        btnC_Load_Up_U->OnClick         =BtnPanelClick;
    }
    else
    {
        btnC_Load_Middle->Enabled       =true;
        btnC_Load_Up->Enabled           =true;
        lblLoaderZ->Visible             =(MOT[MLoaderZ].HomeFlag==false);
        btnC_Load_Middle_U->Enabled     =true;
        btnC_Load_Up_U->Enabled         =true;
        lblLoaderZ_U->Visible           =(MOT[MLoaderZ].HomeFlag==false);
    }

    if(AUTO_EMPTY_COLOR==0 || LOAD_Z_USE_MOTOR[1]==false)
    {
        btnC_Empty_Middle->OnClick      =BtnPanelClick;
        btnC_Empty_Up->OnClick          =BtnPanelClick;
        btnC_Empty_Middle_U->OnClick    =BtnPanelClick;
        btnC_Empty_Up_U->OnClick        =BtnPanelClick;
    }
    else
    {
        btnC_Empty_Middle->Enabled      =true;
        btnC_Empty_Up->Enabled          =true;
        lblEmptyZ->Visible              =(MOT[MEmptyZ].HomeFlag==false);
        btnC_Empty_Middle_U->Enabled    =true;
        btnC_Empty_Up_U->Enabled        =true;
        lblEmptyZ_U->Visible            =(MOT[MEmptyZ].HomeFlag==false);
    }

    if(AUTO_EMPTY_COLOR==0 || LOAD_Z_USE_MOTOR[2]==false)
    {
        btnC_Color_Middle->OnClick      =BtnPanelClick;
        btnC_Color_Up->OnClick          =BtnPanelClick;
        btnC_Color_Middle_U->OnClick    =BtnPanelClick;
        btnC_Color_Up_U->OnClick        =BtnPanelClick;
    }
    else
    {
        btnC_Color_Middle->Enabled      =true;
        btnC_Color_Up->Enabled          =true;
        lblColorZ->Visible              =(MOT[MColorZ].HomeFlag==false);
        btnC_Color_Middle_U->Enabled    =true;
        btnC_Color_Up_U->Enabled        =true;
        lblColorZ_U->Visible            =(MOT[MColorZ].HomeFlag==false);
    }

    if(LOAD_Z_USE_MOTOR[3]==false)
    {
        btnC_Auto1_Selector->OnClick        =BtnPanelClick;
        btnC_Auto1_Up->OnClick              =BtnPanelClick;
        btnC_Auto1_Selector_U->OnClick      =BtnPanelClick;
        btnC_Auto1_Up_U->OnClick            =BtnPanelClick;
    }
    else
    {
        btnC_Auto1_Selector->Enabled        =true;
        btnC_Auto1_Up->Enabled              =true;
        lblAuto1Z->Visible                  =(MOT[MAuto1Z].HomeFlag==false);
        btnC_Auto1_Selector_U->Enabled      =true;
        btnC_Auto1_Up_U->Enabled            =true;
        lblAuto1Z_U->Visible                =(MOT[MAuto1Z].HomeFlag==false);
    }

    if(LOAD_Z_USE_MOTOR[4]==false)
    {
        btnC_Auto2_Selector->OnClick        =BtnPanelClick;
        btnC_Auto2_Up->OnClick              =BtnPanelClick;
        btnC_Auto2_Selector_U->OnClick      =BtnPanelClick;
        btnC_Auto2_Up_U->OnClick            =BtnPanelClick;
    }
    else
    {
        btnC_Auto2_Selector->Enabled        =true;
        btnC_Auto2_Up->Enabled              =true;
        lblAuto2Z->Visible                  =(MOT[MAuto2Z].HomeFlag==false);
        btnC_Auto2_Selector_U->Enabled      =true;
        btnC_Auto2_Up_U->Enabled            =true;
        lblAuto2Z_U->Visible                =(MOT[MAuto2Z].HomeFlag==false);
    }

    if(LOAD_Z_USE_MOTOR[5]==false)
    {
        btnC_Auto3_Selector->OnClick        =BtnPanelClick;
        btnC_Auto3_Up->OnClick              =BtnPanelClick;
        btnC_Auto3_Selector_U->OnClick      =BtnPanelClick;
        btnC_Auto3_Up_U->OnClick            =BtnPanelClick;
    }
    else
    {
        btnC_Auto3_Selector->Enabled        =true;
        btnC_Auto3_Up->Enabled              =true;
        lblAuto3Z->Visible                  =(MOT[MAuto3Z].HomeFlag==false);
        btnC_Auto3_Selector_U->Enabled      =true;
        btnC_Auto3_Up_U->Enabled            =true;
        lblAuto3Z_U->Visible                =(MOT[MAuto3Z].HomeFlag==false);
        if(IniConfig.bVTESTFunction==true && USE_AUTO_RETEST==eartUninstall)    //RogerYang 20250513 上海偉測Auto3Z異常，先把按鈕簡化，只有select作動，客回覆正常
        {
            btnC_Auto3_Up->Visible          =false;
        }
    }

    if(LOAD_Z_USE_MOTOR[6]==false)                                              //Steven 20230907 : For HT-9011UC
    {
        btnC_Auto4_Selector->OnClick        =BtnPanelClick;
        btnC_Auto4_Up->OnClick              =BtnPanelClick;
        btnC_Auto4_Selector_U->OnClick      =BtnPanelClick;
        btnC_Auto4_Up_U->OnClick            =BtnPanelClick;
    }
    else
    {
        btnC_Auto4_Selector->Enabled        =true;
        btnC_Auto4_Up->Enabled              =true;
        lblAuto4Z->Visible                  =(MOT[MAuto4Z].HomeFlag==false);
        btnC_Auto4_Selector_U->Enabled      =true;
        btnC_Auto4_Up_U->Enabled            =true;
        lblAuto4Z_U->Visible                =(MOT[MAuto4Z].HomeFlag==false);
    }

    if(LOAD_Z_USE_MOTOR[7]==false)                                              //Steven 20230907 : For HT-9011UC
    {
        btnC_Auto5_Selector->OnClick        =BtnPanelClick;
        btnC_Auto5_Up->OnClick              =BtnPanelClick;
        btnC_Auto5_Selector_U->OnClick      =BtnPanelClick;
        btnC_Auto5_Up_U->OnClick            =BtnPanelClick;
    }
    else
    {
        btnC_Auto5_Selector->Enabled        =true;
        btnC_Auto5_Up->Enabled              =true;
        lblAuto5Z->Visible                  =(MOT[MAuto5Z].HomeFlag==false);
        btnC_Auto5_Selector_U->Enabled      =true;
        btnC_Auto5_Up_U->Enabled            =true;
        lblAuto5Z_U->Visible                =(MOT[MAuto5Z].HomeFlag==false);
    }

    if(LOAD_Z_USE_MOTOR[8]==false)                                              //Steven 20230907 : For HT-9011UC
    {
        btnC_Auto6_Selector->OnClick        =BtnPanelClick;
        btnC_Auto6_Up->OnClick              =BtnPanelClick;
        btnC_Auto6_Selector_U->OnClick      =BtnPanelClick;
        btnC_Auto6_Up_U->OnClick            =BtnPanelClick;
    }
    else
    {
        btnC_Auto6_Selector->Enabled        =true;
        btnC_Auto6_Up->Enabled              =true;
        lblAuto6Z->Visible                  =(MOT[MAuto6Z].HomeFlag==false);
        btnC_Auto6_Selector_U->Enabled      =true;
        btnC_Auto6_Up_U->Enabled            =true;
        lblAuto6Z_U->Visible                =(MOT[MAuto6Z].HomeFlag==false);
    }

    ledSnTesterAlarm->Visible=IniConfig.bVTESTFunction;
    lblSnTesterAlarm->Visible=IniConfig.bVTESTFunction;

    BtnChamboCoolDown->Visible=SW[SwAutoCoolDown].Enable;                       //kevin 20201223 AutoCool down  Chambo 高溫轉常溫快速降溫
    tsOption1->TabVisible=(USE_E84_Sensor ||                                    //kevin 20220518 AGV Modal
                           AUTO3_IS_MAGAZINE==1 ||
                           USE_AOI_Inspection ||
                           USE_Fix_AI_CCD);

    ledSnLoaderTrayHasTray_AGV->Visible =(USE_E84_Sensor || USE_COVER_TRAYID!=tCIDNotUse);                              //JerryYang 20250521 : For AMR
    ledSnEmptyTrayHasTray_AGV->Visible  =USE_E84_Sensor;
    ledSnColorTrayHasTray_AGV->Visible  =USE_E84_Sensor;

    SetTechDataToProd();

    if(ATC_SYSTEM==eWinWay && Temperature.bATCActiveCooling==true)              //jimmychiu 20210909 add winway io
    {
        grpWinWayGroup->Visible =true;
        grpWinWayGroup->Top     =152;
        grpWinWayGroup->Left    =112;
    }
    else
    {
        grpWinWayGroup->Visible =false;
    }
    sb_IO_CommunicationPad->Visible =iControlPanelMode;                         //Ken 20210702 AddPadInterface
    grpPickUpErrorPlacement->Visible=(USE_InPlacement==eartInstall);            //JimmyChiu 20220908 add Pickup Error Placement
    grpMagazine->Visible            =(AUTO3_IS_MAGAZINE==1);                    //JerryYang 20220909 : add magazine
    grpColorSensor->Visible         =(USE_COLORSENSOR_MUN!=eCSMUN_Uninstall);   //Jimmychiu 20230630 : add color sensor MU-N in Loader
    pnlTriTemp->Visible             =(MachineTypeChoice==Type_HT1032);          //Ztex 2023.04.13 Add HT-1032 IO
    pnl8PickerInArm->Visible        =(USE_PICKER_COUNT!=ep16Picker);            //Steven for HT1032
    pnl16PickerInArm->Visible       =(USE_PICKER_COUNT==ep16Picker);
    pnl8PickerOutArm->Visible       =(USE_PICKER_COUNT!=ep16Picker);
    pnl16PickerOutArm->Visible      =(USE_PICKER_COUNT==ep16Picker);
    gpSafePLC->Visible              =(IsSafePLCIOInstall());                    //KenHsieh 20250212 : 新增PLC 斷線可瞬間判斷EMG及安全門
    //modify the label string in safe page for suitting every module
    if(IsSafePLCIOType_Schneider())
    {
        lbl0x300->Caption = "0x300";
        lbl0x301->Caption = "0x301";
        lbl0x302->Caption = "0x302";
        lbl0x303->Caption = "0x303";
        lbl0x304->Caption = "0x304";
        lbl0x305->Caption = "0x305";
        lbl0x306->Caption = "0x306";
        lbl0x307->Caption = "0x307";
    }
    else if(IsSafePLCIOType_ReeR())
    {
        lbl0x300->Caption = "0x400";
        lbl0x301->Caption = "0x401";
        lbl0x302->Caption = "0x402";
        lbl0x303->Caption = "0x403";
        lbl0x304->Caption = "0x404";
        lbl0x305->Caption = "0x405";
        lbl0x306->Caption = "0x406";
        lbl0x307->Caption = "0x407";
    }
    grpLoaderFunc->Visible          =(USE_LD_Rot_Arm==1);                       //RogerYang 20250828 add for Loader Rotate Arm
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnToolClick(TObject *Sender)
{
    FTool->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::BtnPanelClick(TObject *Sender)
{
    bool bOk;

    TBtnPanelLane *Ptr  = dynamic_cast <TBtnPanelLane   *>(Sender);
    TBtnPanel     *Ptr1 = dynamic_cast <TBtnPanel       *>(Sender);
    bOutDataChange=true;

    if(Ptr!=NULL)
    {
        if(IndexHasIC() && Ptr->Alias.AnsiPos("TestSuck")!=0)                   //Steven 20110520 : Index有IC時,要用Suck()來開啟真空
//        if(Ptr->Alias.AnsiPos("TestSuck")!=0 && Ptr->Alias.AnsiPos("On")!=0)                  // 這一行是測試用的
        {
            lbSuckEnabled->Visible=true;
            bOk=false;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if((FTestSuck.Suck[i][j].SuckerName+"_On")==Ptr->Alias)
                    {
                        mySuckerTemp=&FTestSuck.Suck[i][j];
                        bOk=true;
                        bChangeSuckStatus=SStatusSuck;
                    }
                    else if((BTestSuck.Suck[i][j].SuckerName+"_On")==Ptr->Alias)
                    {
                        mySuckerTemp=&BTestSuck.Suck[i][j];
                        bOk=true;
                        bChangeSuckStatus=SStatusSuck;
                    }
                    else if((FTestSuck.Suck[i][j].SuckerName+"_Off")==Ptr->Alias)
                    {
                        mySuckerTemp=&FTestSuck.Suck[i][j];
                        bOk=true;
                        bChangeSuckStatus=SStatusDestory;
                    }
                    else if((BTestSuck.Suck[i][j].SuckerName+"_Off")==Ptr->Alias)
                    {
                        mySuckerTemp=&BTestSuck.Suck[i][j];
                        bOk=true;
                        bChangeSuckStatus=SStatusDestory;
                    }

                    if(bOk)                                                     //有找到就跳開
                    {
                        break;
                    }
                }

                if(bOk)
                    break;
            }
        }
        else
        {
            bChangeSuckStatus=false;                                            //Steven 20110629
            Ptr->Down=!Ptr->Down;
            if(Ptr->ISABase==eMotionNet ||
               Ptr->ISABase==ePCI1203)                                          //Sam 20230724 : 新增 PCI1203 IO 模組
            {
                if((Ptr->Down==true  && Ptr->OutType==1) ||
                   (Ptr->Down==false && Ptr->OutType==0))
                {
                    MyLaneIO.IOBitOn(Ptr->OutRing, Ptr->OutIP, Ptr->OutPort, Ptr->OutBit, Ptr->ISABase, Ptr->Alias);    //Sam 20230724 : 新增 PCI1203 IO 模組
                    fPadInterface->SendSwitchStatus(Ptr);                       //Ken 20210702 AddPadInterface
                }
                else
                {
                    MyLaneIO.IOBitOff(Ptr->OutRing, Ptr->OutIP, Ptr->OutPort, Ptr->OutBit, Ptr->ISABase, Ptr->Alias);   //Sam 20230724 : 新增 PCI1203 IO 模組
                    fPadInterface->SendSwitchStatus(Ptr);                       //Ken 20210702 AddPadInterface
                }
            }
            else if(Ptr->ISABase==eISABase ||                                   //Nickliu 20230309 add Cylinder ISABase Type
                    Ptr->ISABase==ePCI1735U ||                                  //Alick 20160809 modify for PCI-1735U
                    Ptr->ISABase==ePLCbase)
            {
                if((Ptr->Down==true  && Ptr->OutType==1) ||
                   (Ptr->Down==false && Ptr->OutType==0))
                {
                    IOBitOn(Ptr->OutPort, Ptr->OutBit);
                    fPadInterface->SendSwitchStatus(Ptr);                       //Ken 20210702 AddPadInterface
                }
                else
                {
                    IOBitOff(Ptr->OutPort, Ptr->OutBit);
                    fPadInterface->SendSwitchStatus(Ptr);                       //Ken 20210702 AddPadInterface
                }
            }

            if(Ptr->Down)
            {
                Ptr->Color          =Ptr->TrueColor;
                Ptr->Font->Color    =Ptr->TrueFontColor;
                Ptr->BevelInner     =bvLowered;
                Ptr->BevelOuter     =bvLowered;
            }
            else
            {
                Ptr->Color          =Ptr->FalseColor;
                Ptr->Font->Color    =Ptr->FalseFontColor;
                if(Ptr->Style==tsFlatButtons)                                   //Steven 20230826 : for 新版GUI, 增加Flat Btn選項
                {
                    Ptr->BevelInner =bvNone;
                    Ptr->BevelOuter =bvNone;
                }
                else
                {
                    Ptr->BevelInner =bvRaised;
                    Ptr->BevelOuter =bvRaised;
                }
            }

//            for(i=0; i<6; i++)                                                //jou 2013-12-06 經電控課實驗 , F/T handler 的 auto tray 要馬達停止後才能切換 relay 正反轉 , 這樣EMI才能受到控制 , F/T handler的軟體要修正(手動畫面內).
//            {
//                if(SW[iSwACCCW[i]].Enable && Ptr->Alias==asSwACCCW[i])
//                {
//                    PtrSwACCCW[i]->Down=false;
//                    PtrSwACCCW[i]->Color          =PtrSwACCCW[i]->FalseColor;
//                    PtrSwACCCW[i]->Font->Color    =PtrSwACCCW[i]->FalseFontColor;
//                    PtrSwACCCW[i]->BevelInner     =bvRaised;
//                    PtrSwACCCW[i]->BevelOuter     =bvRaised;
//                }
//            }

            if(SW[SwFMotorBreaker].Enable             &&
               SW[SwFMotorBreaker].Ring==Ptr->OutRing &&
               SW[SwFMotorBreaker].IP  ==Ptr->OutIP   &&
               SW[SwFMotorBreaker].Port==Ptr->OutPort &&
               SW[SwFMotorBreaker].Bit ==Ptr->OutBit)
            {
                fAllMotorHome=false;
                MOT[MTestZ1].HomeFlag=0;
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 20180319 add
                    ShowMyMessage("SwFMotorBreaker");                           //Ifor 20151208 :Add Motor Home Massage
            }

            if(SW[SwBMotorBreaker].Enable             &&
               SW[SwBMotorBreaker].Ring==Ptr->OutRing &&
               SW[SwBMotorBreaker].IP  ==Ptr->OutIP   &&
               SW[SwBMotorBreaker].Port==Ptr->OutPort &&
               SW[SwBMotorBreaker].Bit ==Ptr->OutBit)
            {
                fAllMotorHome=false;
                MOT[MTestZ2].HomeFlag=0;
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 20180319 add
                    ShowMyMessage("SwFMotorBreaker");                           //Ifor 20151208 :Add Motor Home Massage
            }

            if(SW[SwMotorRelay].Enable             &&
               SW[SwMotorRelay].Ring==Ptr->OutRing &&
               SW[SwMotorRelay].IP  ==Ptr->OutIP   &&
               SW[SwMotorRelay].Port==Ptr->OutPort &&
               SW[SwMotorRelay].Bit ==Ptr->OutBit)
            {
                fAllMotorHome=false;
                for(int i=0; i<TOTAL_MOTOR; i++)
                    MOT[i].HomeFlag=0;

                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 20180319 add
                    ShowMyMessage("SwFMotorBreaker");                           //Ifor 20151208 :Add Motor Home Massage
            }

            if(SW[SwServerON].Enable             &&
               SW[SwServerON].Ring==Ptr->OutRing &&
               SW[SwServerON].IP  ==Ptr->OutIP   &&
               SW[SwServerON].Port==Ptr->OutPort &&
               SW[SwServerON].Bit ==Ptr->OutBit)
            {
                fAllMotorHome=false;
                for(int i=0; i<TOTAL_MOTOR; i++)
                    MOT[i].HomeFlag=0;

                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 20180319 add
                    ShowMyMessage("SwFMotorBreaker");                           //Ifor 20151208 :Add Motor Home Massage
            }
        }
    }
    else
    {
        Ptr1->Down=!Ptr1->Down;

        if(TTL_CARD_TYPE>0)                                                     //Steven 20200901 : Win10 & Win7也可以用TTL
            return;

        if((Ptr1->Down && Ptr1->OutType==1) ||
           (Ptr1->Down==false  && Ptr1->OutType==0))
        {
            IOBitOn(Ptr1->OutPort, Ptr1->OutBit);
            fPadInterface->SendSwitchStatus(Ptr);                               //Ken 20210702 AddPadInterface
        }
        else
        {
            IOBitOff(Ptr1->OutPort, Ptr1->OutBit);
            fPadInterface->SendSwitchStatus(Ptr);                               //Ken 20210702 AddPadInterface
        }

        if(Ptr1->Down)
        {
            Ptr1->Color=Ptr1->TrueColor;
            Ptr1->Font->Color=Ptr1->TrueFontColor;
            Ptr1->BevelInner=bvLowered;
            Ptr1->BevelOuter=bvLowered;
        }
        else
        {
            Ptr1->Color=Ptr1->FalseColor;
            Ptr1->Font->Color=Ptr1->FalseFontColor;
            if(Ptr1->Style==tsFlatButtons)                                      //Steven 20230826 : for 新版GUI, 增加Flat Btn選項
            {
                Ptr1->BevelInner =bvNone;
                Ptr1->BevelOuter =bvNone;
            }
            else
            {
                Ptr1->BevelInner =bvRaised;
                Ptr1->BevelOuter =bvRaised;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnReloadClick(TObject *Sender)
{
    if(IO_CARD_TYPE==NewIO_MN200 ||
       IO_CARD_TYPE==PCI_P64C64)                                                //Steven 20231218 HT7080B
    {
        HSys.LoadIoData();
        LoadIoTable(0, 0, 0);
        sbtReload->Down=false;
    }
    InitialSwitch();
    InitialSensor();
    InitSucker();
    InitCylinder();
    ChangeSite();
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnTTLTestClick(TObject *Sender)
{
    bStartTTLOut=true;
    iTTLLoopCount =atoi(edtSendCount->Text.c_str());
    iTTLPulseWidth=atoi(edtPulseDelay->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::Timer2Timer(TObject *Sender)
{
    DoTTL_Spin();
    DoColorSensor_Spin();
}
//---------------------------------------------------------------------------
void Tfiosetview::DoColorSensor_Spin()                                          //Jimmychiu 20230711 : Add ColorSensor Spin function
{
    if(fShow==false)
        return;

    if(fTrayForm->IsEnableColorSensor()==true)                                  //Jimmychiu 20230630 : add color sensor MU-N in Loader
    {
        int iColorNum=GetColorSensorOnLoaderByMUN();
        AnsiString sColorName=fTrayForm->GetColorSensor("ColorSensor_FT")->GetColorName(iColorNum);
        lbColorSensorNumber->Caption=AnsiString().sprintf("%d %s", iColorNum, sColorName);
    }
}
//---------------------------------------------------------------------------
void Tfiosetview::DoTTL_Spin()                                                  //Jimmychiu 20230711 : Add TTL Spin function
{
    if(fShow==false || bStartTTLOut==false || iTTLLoopCount<=0)
        return;

    if(chkTTLStart0->Checked)
        SW[SwStart0].On();
    if(chkTTLStart1->Checked)
        SW[SwStart1].On();
    for(int i=0; i<iTTLPulseWidth; i++)
        for(int j=0; j<100; j++)
            ;
    SW[SwStart0].Off();
    SW[SwStart1].Off();
    iTTLLoopCount--;
    if(iTTLLoopCount==0)
        bStartTTLOut=false;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::ShowSuckMode(int iSel)                             //Steven 20240207 : 修改真空顯示方式
{
    palArm2_Ab->Visible=true;
    palArm1_Ab->Visible=true;

    if(iSel==-1)                                                                //全部顯示為使用中
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                Index_8[0][i][j]->Color =TColor(0x00393A3E);
                Index_8[1][i][j]->Color =TColor(0x00393A3E);
                Index_8[0][i][j]->Caption=IndexSuckName[i][j];
                Index_8[1][i][j]->Caption=IndexSuckName[i][j];
                InArm_8[i][j]->Color    =TColor(0x00393A3E);
                OutArm_8[i][j]->Color   =TColor(0x00393A3E);
            }
        }

        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                Index_16[0][i][j]->Color=TColor(0x00393A3E);
                Index_16[1][i][j]->Color=TColor(0x00393A3E);
                Index_16[0][i][j]->Caption=IndexSuckName[i][j];
                Index_16[1][i][j]->Caption=IndexSuckName[i][j];
                InArm_16[i][j]->Color   =TColor(0x00393A3E);
                OutArm_16[i][j]->Color  =TColor(0x00393A3E);
            }
        }
    }
    else                                                                        //全部顯示為未使用
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                Index_8[0][i][j]->Color =TColor(0x00DDDDDC);
                Index_8[1][i][j]->Color =TColor(0x00DDDDDC);
                Index_8[0][i][j]->Caption="";
                Index_8[1][i][j]->Caption="";
                InArm_8[i][j]->Color    =TColor(0x00DDDDDC);
                OutArm_8[i][j]->Color   =TColor(0x00DDDDDC);
            }
        }

        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                Index_16[0][i][j]->Color=TColor(0x00DDDDDC);
                Index_16[1][i][j]->Color=TColor(0x00DDDDDC);
                Index_16[0][i][j]->Caption="";
                Index_16[1][i][j]->Caption="";
                InArm_16[i][j]->Color   =TColor(0x00DDDDDC);
                OutArm_16[i][j]->Color  =TColor(0x00DDDDDC);
            }
        }
    }

    if(iSel==-1)
        return;

    int iR, iC;
    for(int i=0; i<FTestSuck.iShtRow; i++)
    {
        for(int j=0; j<FTestSuck.iShtCol; j++)
        {
            iR=FTestSuck.Suck[i][j].iMyRow;
            iC=FTestSuck.Suck[i][j].iMyCol;
            if(iC<4)
            {
                Index_8[0][iR][iC]->Color =TColor(0x00393A3E);
                Index_8[0][iR][iC]->Caption=IndexSuckName[i][j];
            }
            Index_16[0][iR][iC]->Color    =TColor(0x00393A3E);
            Index_16[0][iR][iC]->Caption  =IndexSuckName[i][j];

            iR=BTestSuck.Suck[i][j].iMyRow;
            iC=BTestSuck.Suck[i][j].iMyCol;
            if(iC<4)
            {
                Index_8[1][iR][iC]->Color =TColor(0x00393A3E);
                Index_8[1][iR][iC]->Caption=IndexSuckName[i][j];
            }
            Index_16[1][iR][iC]->Color    =TColor(0x00393A3E);
            Index_16[1][iR][iC]->Caption  =IndexSuckName[i][j];
        }
    }

    for(int i=0; i<OutArmSuck.iPickRow; i++)
    {
        for(int j=0; j<OutArmSuck.iPickCol; j++)
        {
            iR=OutArmSuck.Suck[i][j].iMyRow;
            iC=OutArmSuck.Suck[i][j].iMyCol;
            if(j<4)
            {
                OutArm_8[iR][iC]->Color   =TColor(0x00393A3E);
            }
            OutArm_16[iR][iC]->Color      =TColor(0x00393A3E);
        }
    }

    if(iInArmType==e9045_1x1_1)                                                 //Steven 20240223 : 調整inarm 真空在IO畫面顯示
    {
        if(Prod.bSingleUseOtherSuck || Prod.bSingleInArmUseOtherSuck)           //wei 20220823 Single Site使用C吸嘴判斷
        {
            InArm_8[0][1]->Color    =TColor(0x00393A3E);
            InArm_16[0][2]->Color   =TColor(0x00393A3E);
        }
        else
        {
            InArm_8[0][0]->Color    =TColor(0x00393A3E);
            InArm_16[0][0]->Color   =TColor(0x00393A3E);
        }
    }
    else if(iInArmType==e9045_1x2_2_13)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][2]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][4]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_1x2_2_14)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][3]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][6]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_1x2_4_Hot)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][2]->Color    =TColor(0x00393A3E);
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][2]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][4]->Color   =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][4]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_1x3_2_14)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][3]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][6]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_1x3_4)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][1]->Color    =TColor(0x00393A3E);
        InArm_8[0][2]->Color    =TColor(0x00393A3E);
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][1]->Color    =TColor(0x00393A3E);
        InArm_8[1][2]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][2]->Color   =TColor(0x00393A3E);
        InArm_16[0][4]->Color   =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][2]->Color   =TColor(0x00393A3E);
        InArm_16[1][4]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_1x4_2_14)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][3]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][6]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_1x4_1_Ac)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_1x4_4_13)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][2]->Color    =TColor(0x00393A3E);
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][2]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][4]->Color   =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][4]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_1x4_4_Back)
    {
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][1]->Color    =TColor(0x00393A3E);
        InArm_8[1][2]->Color    =TColor(0x00393A3E);
        InArm_8[1][3]->Color    =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][2]->Color   =TColor(0x00393A3E);
        InArm_16[1][4]->Color   =TColor(0x00393A3E);
        InArm_16[1][6]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_1x4_4)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][1]->Color    =TColor(0x00393A3E);
        InArm_8[0][2]->Color    =TColor(0x00393A3E);
        InArm_8[0][3]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][2]->Color   =TColor(0x00393A3E);
        InArm_16[0][4]->Color   =TColor(0x00393A3E);
        InArm_16[0][6]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_1x4_8_Hot)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][1]->Color    =TColor(0x00393A3E);
        InArm_8[0][2]->Color    =TColor(0x00393A3E);
        InArm_8[0][3]->Color    =TColor(0x00393A3E);
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][1]->Color    =TColor(0x00393A3E);
        InArm_8[1][2]->Color    =TColor(0x00393A3E);
        InArm_8[1][3]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][2]->Color   =TColor(0x00393A3E);
        InArm_16[0][4]->Color   =TColor(0x00393A3E);
        InArm_16[0][6]->Color   =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][2]->Color   =TColor(0x00393A3E);
        InArm_16[1][4]->Color   =TColor(0x00393A3E);
        InArm_16[1][6]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_2x1_2_13)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][2]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][4]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_2x2_4_12)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][1]->Color    =TColor(0x00393A3E);
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][1]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][2]->Color   =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][2]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_2x2_4_13)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][2]->Color    =TColor(0x00393A3E);
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][2]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][4]->Color   =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][4]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_2x2_4_14)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][3]->Color    =TColor(0x00393A3E);
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][3]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][6]->Color   =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][6]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_2x2_8_Hot)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][1]->Color    =TColor(0x00393A3E);
        InArm_8[0][2]->Color    =TColor(0x00393A3E);
        InArm_8[0][3]->Color    =TColor(0x00393A3E);
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][1]->Color    =TColor(0x00393A3E);
        InArm_8[1][2]->Color    =TColor(0x00393A3E);
        InArm_8[1][3]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][2]->Color   =TColor(0x00393A3E);
        InArm_16[0][4]->Color   =TColor(0x00393A3E);
        InArm_16[0][6]->Color   =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][2]->Color   =TColor(0x00393A3E);
        InArm_16[1][4]->Color   =TColor(0x00393A3E);
        InArm_16[1][6]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_2x3_6_14)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][3]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][6]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_2x3_6)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][1]->Color    =TColor(0x00393A3E);
        InArm_8[0][2]->Color    =TColor(0x00393A3E);
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][1]->Color    =TColor(0x00393A3E);
        InArm_8[1][2]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][2]->Color   =TColor(0x00393A3E);
        InArm_16[0][4]->Color   =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][2]->Color   =TColor(0x00393A3E);
        InArm_16[1][4]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_2x4_4_13)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][2]->Color    =TColor(0x00393A3E);
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][2]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][4]->Color   =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][4]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_2x4_4_14)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][3]->Color    =TColor(0x00393A3E);
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][3]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][6]->Color   =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][6]->Color   =TColor(0x00393A3E);
    }
    else if(iInArmType==e9045_2x4_8 ||
            iInArmType==e9045_2x5_8 ||
            iInArmType==e9045_2x6_8 ||
            iInArmType==e9045_2x8_8 ||
            iInArmType==e9045_2x8_32)
    {
        InArm_8[0][0]->Color    =TColor(0x00393A3E);
        InArm_8[0][1]->Color    =TColor(0x00393A3E);
        InArm_8[0][2]->Color    =TColor(0x00393A3E);
        InArm_8[0][3]->Color    =TColor(0x00393A3E);
        InArm_8[1][0]->Color    =TColor(0x00393A3E);
        InArm_8[1][1]->Color    =TColor(0x00393A3E);
        InArm_8[1][2]->Color    =TColor(0x00393A3E);
        InArm_8[1][3]->Color    =TColor(0x00393A3E);
        InArm_16[0][0]->Color   =TColor(0x00393A3E);
        InArm_16[0][2]->Color   =TColor(0x00393A3E);
        InArm_16[0][4]->Color   =TColor(0x00393A3E);
        InArm_16[0][6]->Color   =TColor(0x00393A3E);
        InArm_16[1][0]->Color   =TColor(0x00393A3E);
        InArm_16[1][2]->Color   =TColor(0x00393A3E);
        InArm_16[1][4]->Color   =TColor(0x00393A3E);
        InArm_16[1][6]->Color   =TColor(0x00393A3E);
    }
}
//-------------------------------------------------------------------------
void __fastcall Tfiosetview::ShowShuttleSensor()                                //jou 980828 start : add Out Shuttle Sensor detect mode
{                                                                               //0 = Detect Have IC
    if(LastSet.iOutShuttleSensorMode==0)                                        //1 = Detect Double Device
    {
        mmoOutSht->Visible=true;
        mmoOutSht2->Visible=false;
    }
    else
    {
        mmoOutSht->Visible=false;
        mmoOutSht2->Visible=true;
        mmoOutSht2->Top=mmoOutSht->Top;
        mmoOutSht2->Left=mmoOutSht->Left;
    }

    AnsiString Str1="", Str2="", Str3="", Str4="";
    TPanel *pInShuttleLink[9]={plInShuttleLink1, plInShuttleLink2, plInShuttleLink3, plInShuttleLink4, plInShuttleLink5,
                               plInShuttleLink6, plInShuttleLink7, plInShuttleLink8, plInShuttleLink9};
    TPanel *pOutShuttleLink[7]={plOutShuttleLink1, plOutShuttleLink2, plOutShuttleLink3, plOutShuttleLink4, plOutShuttleLink5,
                                plOutShuttleLink6, plOutShuttleLink7};

    InitShuttleThreadParameter();

    for(int i=0; i<8; i++)                                                      //Steven 20250429 : 統一整理shuttle sensor顯示
    {
        if(SThreadPara.bUseInShtSen[i]==true)                                   //Steven 20250429 : 有用到的in sht sensor位置
            pInShuttleLink[i]->Color=clLime;
        else
            pInShuttleLink[i]->Color=clBtnFace;
    }

    if(MachineTypeChoice==Type_HT9045 ||
       MachineTypeChoice==Type_HT9045_12Site)                                   //ChungHung 20130507 add HT9045 updata for 12site 517
    {
        if(ENABLE_OUT_SHUTTLE_SENEOR)
        {
            for(int i=0; i<6; i++)
            {
                if(SThreadPara.bUseInShtSen[i]==true)
                    pOutShuttleLink[i+1]->Color=clLime;
                else
                    pOutShuttleLink[i+1]->Color=clBtnFace;
            }
        }

        grpInShuttle->Visible=true;
        grpOutShuttle->Visible=ENABLE_OUT_SHUTTLE_SENEOR;
        pInShuttleLink[8-1]->Visible=false;
        pInShuttleLink[9-1]->Visible=(IN_SHT_LAST_SENSOR==1);                   //Steven 20181203 : In Shuttle最後一個Sensor定義
        MyLedInShuttleNumR8->Visible=false;
        MyLedInShuttleNumR9->Visible=(IN_SHT_LAST_SENSOR==1);                   //Steven 20181203 : In Shuttle最後一個Sensor定義
        MyLedInShuttleNumF8->Visible=false;
        MyLedInShuttleNumF9->Visible=(IN_SHT_LAST_SENSOR==1);                   //Steven 20181203 : In Shuttle最後一個Sensor定義
    }
    else
    {
        grpInShuttle->Visible=true;
        grpOutShuttle->Visible=false;
        MyLedInShuttleNumR8->Visible=true;
        MyLedInShuttleNumR9->Visible=true;
        MyLedInShuttleNumF8->Visible=true;
        MyLedInShuttleNumF9->Visible=true;
    }

    lbOutShuttle1->Visible=(ENABLE_OUT_SHUTTLE_SENEOR==true);
    lbOutShuttle2->Visible=(ENABLE_OUT_SHUTTLE_SENEOR==true);

    for(int i=0; i<FLCarryKit.iShtCol; i++)                                     //Steven 20250429 : 統一整理shuttle sensor顯示
    {
        Str1+=Sen[SThreadPara.iInShSenIndex[0][i]].Name+AnsiString(", ");
        Str2+=Sen[SThreadPara.iInShSenIndex[1][i]].Name+AnsiString(", ");
    }

    if(ENABLE_OUT_SHUTTLE_SENEOR==true &&
       FLCarryKit.iShtCol<6)
    {
        for(int i=0; i<FLCarryKit.iShtCol; i++)
        {
            Str3+=Sen[SThreadPara.iOutShSenIndex[0][i]].Name+AnsiString(", ");
            Str4+=Sen[SThreadPara.iOutShSenIndex[1][i]].Name+AnsiString(", ");
        }
    }

    lbInShuttle1->Caption=Str1;
    lbInShuttle2->Caption=Str2;
    lbOutShuttle1->Caption=Str3;
    lbOutShuttle2->Caption=Str4;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::chkShowIndexAllClick(TObject *Sender)
{
    if(fShow==false)
        return;
    ShowSuckMode(-1);
    chkShowIndexAll->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::BtnNumPanelTestClick(TObject *Sender)
{
    TBtnPanelLane *Ptr=dynamic_cast <TBtnPanelLane *>(Sender);                  //Steven 20090919 : For Test Number Panel
    Ptr->Down=!Ptr->Down;
    if(Ptr->Down)
    {
        Ptr->Color          =Ptr->TrueColor;
        Ptr->Font->Color    =Ptr->TrueFontColor;
        Ptr->BevelInner     =bvLowered;
        Ptr->BevelOuter     =bvLowered;
        fShowBinSelect->InitShowBinDigital();
        iNumPanelDown       =Ptr->Tag;
        bNumPanelDown       =true;
    }
    else
    {
        Ptr->Color          =Ptr->FalseColor;
        Ptr->Font->Color    =Ptr->FalseFontColor;
        if(Ptr->Style==tsFlatButtons)                                           //Steven 20230826 : for 新版GUI, 增加Flat Btn選項
        {
            Ptr->BevelInner =bvNone;
            Ptr->BevelOuter =bvNone;
        }
        else
        {
            Ptr->BevelInner =bvRaised;
            Ptr->BevelOuter =bvRaised;
        }
        bNumPanelDown       =false;
    }
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::tbarIndexEPChange(TObject *Sender)
{
    if(EP_Install==5)
        ADAM_DirectWriteData(tbarIndexEP->Position, 0, 10);
    else
        ADAM_DirectWriteData(tbarIndexEP->Position, 0);
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::tbarLoaderEPChange(TObject *Sender)
{
    ADAM_DirectWriteData(tbarLoaderEP->Position,1);
    lblLoadEpAlarm->Caption=tbarLoaderEP->Position;
    double dEP=0.0;                                                             //JerryYang 20190708 EP顯示百分比
    dEP=(tbarLoaderEP->Position/4095.0)*100.0;
    AnsiString strEp="";
    strEp.sprintf("%.0f", dEP);
    labEP->Caption=strEp+" %";

    double dShowData  =atof(lblLoadEpAlarm->Caption.c_str());
    double dCalculate =(16000000/4095);
    dShowData=dShowData*dCalculate;
    dShowData+=(4*1000000);

    lblLoadEpAlarm->Caption=dShowData;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::tbarDieForceChange(TObject *Sender)
{
    ADAM_DirectWriteData(tbarDieForce->Position, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnAllVacuumClick(TObject *Sender)
{
    TBtnPanelLane *Ptr;
    Ptr=(TBtnPanelLane *)Sender;

    switch(Ptr->Tag)
    {
        case 0:                                                                 //rear suck
            for(int i=0; i<BTestSuck.iShtRow; i++)
                for(int j=0; j<BTestSuck.iShtCol; j++)
                    bIndexSuck[1][i][j]=true;
            break;
        case 1:                                                                 //front suck
            for(int i=0; i<FTestSuck.iShtRow; i++)
                for(int j=0; j<FTestSuck.iShtCol; j++)
                    bIndexSuck[0][i][j]=true;
            break;
        case 2:                                                                 //rear destroy
            for(int i=0; i<BTestSuck.iShtRow; i++)
                for(int j=0; j<BTestSuck.iShtCol; j++)
                    bIndexDestroy[1][i][j]=true;
            break;
        case 3:                                                                 //front destroy
            for(int i=0; i<FTestSuck.iShtRow; i++)
                for(int j=0; j<FTestSuck.iShtCol; j++)
                    bIndexDestroy[0][i][j]=true;
            break;
    }
}
//---------------------------------------------------------------------------
void Tfiosetview::ResetIndexSuck()
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
            for(int k=0; k<8; k++)
                bIndexSuck[i][j][k]=false;
    }
}
//---------------------------------------------------------------------------
void Tfiosetview::ResetIndexDestroy()
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
            for(int k=0; k<8; k++)
                bIndexDestroy[i][j][k]=false;
    }
}
//---------------------------------------------------------------------------
bool Tfiosetview::ProcessIndexSuckDestroy2(int iType)                           //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆
{
    int iCt=0;

    for(int i=0; i<BTestSuck.iShtRow; i++)
    {
        for(int j=0; j<BTestSuck.iShtCol; j++)
        {
            if(CosFunction.bD44Once4Suck && iType==1 && iCt>=4)                 //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆
                continue;
            if(bIndexSuck[1][i][j]==true)
            {
                iCt++;
                if(BTestSuck.Suck[i][j].Suck())
                {
                    bIndexSuck[1][i][j]=false;
                }

                if(BTestSuck.Suck[i][j].Error==true)
                {
                    BTestSuck.Suck[i][j].Error=false;
                    bIndexSuck[1][i][j]=false;                                  //Sam 20221007 : 修正index check error
                }
            }
            else if(bIndexDestroy[1][i][j]==true)                               //jou 2011-11-01 加個else避免真空破壞同時開啟
            {
                iCt++;
                if(BTestSuck.Suck[i][j].Destroy())
                {
                    bIndexDestroy[1][i][j]=false;
                }
            }
        }
    }

    if(iCt==0)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
bool Tfiosetview::ProcessIndexSuckDestroy1(int iType)                           //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆 //Sam 20220902 : 增加 Retry 機制
{
    int iCt=0;

    for(int i=0; i<FTestSuck.iShtRow; i++)
    {
        for(int j=0; j<FTestSuck.iShtCol; j++)
        {
            if(CosFunction.bD44Once4Suck && iType==1 && iCt>=4)                 //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆
                continue;

            if(bIndexSuck[0][i][j]==true)
            {
                iCt++;
                if(FTestSuck.Suck[i][j].Suck())
                {
                    bIndexSuck[0][i][j]=false;
                }

                if(FTestSuck.Suck[i][j].Error==true)
                {
                    FTestSuck.Suck[i][j].Error=false;
                    bIndexSuck[0][i][j]=false;                                  //Sam 20221007 : 修正index check error
                }
            }
            else if(bIndexDestroy[0][i][j]==true)                               //jou 2011-11-01 加個else避免真空破壞同時開啟
            {
                iCt++;
                if(FTestSuck.Suck[i][j].Destroy())
                {
                    bIndexDestroy[0][i][j]=false;
                }
            }
        }
    }

    if(iCt==0)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
bool Tfiosetview::ContactIndexSuckDestroy1(int iArm, int iRow, int iCol)
{
    int iCt=0;
    int i=iRow, j=iCol;
    if(iArm==0)
    {
        if(bIndexSuck[0][i][j]==true)
        {
            iCt++;
            if(FTestSuck.Suck[i][j].Suck())
            {
                bIndexSuck[0][i][j]=false;
            }

            if(FTestSuck.Suck[i][j].Error==true)
            {
                FTestSuck.Suck[i][j].Error=false;
                bIndexSuck[0][i][j]=false;
            }
        }
        else if(bIndexDestroy[0][i][j]==true)                                   //jou 2011-11-01 加個else避免真空破壞同時開啟
        {
            iCt++;
            if(FTestSuck.Suck[i][j].Destroy())
            {
                bIndexDestroy[0][i][j]=false;
            }
        }
    }
    else
    {
        if(bIndexSuck[1][i][j]==true)
        {
            iCt++;
            if(BTestSuck.Suck[i][j].Suck())
            {
                bIndexSuck[1][i][j]=false;
            }

            if(BTestSuck.Suck[i][j].Error==true)
            {
                BTestSuck.Suck[i][j].Error=false;
                bIndexSuck[1][i][j]=false;
            }
        }
        else if(bIndexDestroy[1][i][j]==true)                                   //jou 2011-11-01 加個else避免真空破壞同時開啟
        {
            iCt++;
            if(BTestSuck.Suck[i][j].Destroy())
            {
                bIndexDestroy[1][i][j]=false;
            }
        }
    }

    if(iCt==0)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnLatchCheckClick(TObject *Sender)
{
    fLtcSensor->ShowModal();
}
//------------------------------------------------------------------------------
void Tfiosetview::SetPanelToEnable(TWinControl *PCtrl)                          //Steven 20250317 : 將Panel下的全部元件都enable
{
    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TPanel        *>(P) != NULL ||
           dynamic_cast <TPageControl  *>(P) != NULL ||
           dynamic_cast <TTabSheet     *>(P) != NULL ||
           dynamic_cast <TTabControl   *>(P) != NULL ||
           dynamic_cast <TForm         *>(P) != NULL ||
           dynamic_cast <THeader       *>(P) != NULL ||
           dynamic_cast <TPage         *>(P) != NULL ||
           dynamic_cast <TGroupBox     *>(P) != NULL ||
           dynamic_cast <TScrollBox    *>(P) != NULL ||
           dynamic_cast <TRadioGroup   *>(P) != NULL )
        {
            SetPanelToEnable((TWinControl *) P);                                // 找該物件附屬的物件(遞迴)
        }

        TBtnPanelLane   *PBtnPanelLan = dynamic_cast <TBtnPanelLane *>(P);
        TBtnPanel       *PBtnPanel    = dynamic_cast <TBtnPanel     *>(P);
        TMyLedLane      *PMyLedLan    = dynamic_cast <TMyLedLane    *>(P);
        TMyLed          *PMyLed       = dynamic_cast <TMyLed        *>(P);

        if(PBtnPanelLan!=NULL)
        {
            PBtnPanelLan->Enabled=true;
        }
        else if(PBtnPanel!=NULL)
        {
            PBtnPanel->Enabled=true;
        }
        else if(PMyLedLan!=NULL)
        {
            PMyLedLan->Enabled=true;
        }
        else if(PMyLed!=NULL)
        {
            PMyLed->Enabled=true;
        }
    }
}
//------------------------------------------------------------------------------
void Tfiosetview::SetPanelElable()
{
    #ifdef SOFT_SIMULTE
    SetPanelToEnable(fiosetview);
    #endif
    if(iControlPanelMode==1)                                                    //Steven 20250317 : 使用通訊式面板的要disable
    {
        SetPanelToEnable(grpFrontKeyPad);
        SetPanelToEnable(grpRearKeyPad);
    }
}
//------------------------------------------------------------------------------
void Tfiosetview::SetCompomentIO(TWinControl *PCtrl)
{
    AnsiString S="";
    bool flag=false, ret;
    int iRing=0, iPort=0, iIP=0;
    SetIOTableByECAT_VC8_Sucker();                                              //Sam 20230210 : 新增 VacuumUnit 通訊模組
    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TPanel        *>(P) != NULL ||
           dynamic_cast <TPageControl  *>(P) != NULL ||
           dynamic_cast <TTabSheet     *>(P) != NULL ||
           dynamic_cast <TTabControl   *>(P) != NULL ||
           dynamic_cast <TForm         *>(P) != NULL ||
           dynamic_cast <THeader       *>(P) != NULL ||
           dynamic_cast <TPage         *>(P) != NULL ||
           dynamic_cast <TGroupBox     *>(P) != NULL ||
           dynamic_cast <TScrollBox    *>(P) != NULL ||
           dynamic_cast <TRadioGroup   *>(P) != NULL )
        {
            if(P->Name=="grpAGV")
                break;
            else
                SetCompomentIO((TWinControl *) P);                              // 找該物件附屬的物件(遞迴)
        }

        TBtnPanelLane   *PBtnPanelLan = dynamic_cast <TBtnPanelLane *>(P);
        TBtnPanel       *PBtnPanel    = dynamic_cast <TBtnPanel     *>(P);
        TMyLedLane      *PMyLedLan    = dynamic_cast <TMyLedLane    *>(P);
        TMyLed          *PMyLed       = dynamic_cast <TMyLed        *>(P);

        if(PBtnPanelLan!=NULL)
        {
            S=PBtnPanelLan->Alias;
            flag=false;
            for(int i=0; i<MAX_SWITCH_ITEM; i++)
            {
                if(S==SW[i].Name)
                {
                    PBtnPanelLan->OutRing  =SW[i].Ring;
                    PBtnPanelLan->OutIP    =SW[i].IP;
                    PBtnPanelLan->OutPort  =SW[i].Port;
                    PBtnPanelLan->OutBit   =SW[i].Bit;
                    PBtnPanelLan->OutType  =SW[i].Type;
                    PBtnPanelLan->ISABase  =SW[i].ISABase;                      //Nickliu 20230306 add IO Use ISABase
                    PBtnPanelLan->Enabled  =SW[i].Enable;
                    if(PBtnPanelLan->ISABase==eMotionNet ||
                       PBtnPanelLan->ISABase==ePCI1203)
                    {
                        if(SW[i].Using!="" && SW[i].Enable==true)
                            flag=true;
                    }
                    else
                    {
                        if(SW[i].Port!=0 && SW[i].Enable==true)                 //Nickliu 20230306 add IO Use ISABase
                            flag=true;
                    }
                    break;
                }
            }

            if(flag==false)
            {
                for(int i=0; i<pSuck->Count; i++)
                {
                    pTempSuck=(TMySucker*)pSuck->Items[i];
                    if(pTempSuck!=NULL && S==pTempSuck->OnPortName)
                    {
                        PBtnPanelLan->OutRing  =pTempSuck->OnRing;
                        PBtnPanelLan->OutIP    =pTempSuck->OnIP;
                        PBtnPanelLan->OutPort  =pTempSuck->OnPort;
                        PBtnPanelLan->OutBit   =pTempSuck->OnBit;
                        PBtnPanelLan->OutType  =pTempSuck->OnType;
                        PBtnPanelLan->ISABase  =pTempSuck->ISABase;             //Nickliu 20230306 add IO Use ISABase
                        PBtnPanelLan->Enabled  =pTempSuck->Enable;
                        if(PBtnPanelLan->ISABase==eMotionNet ||
                           PBtnPanelLan->ISABase==ePCI1203)
                        {
                            if(pTempSuck->OnEnable)
                                flag=true;
                        }
                        else
                        {
                            if(pTempSuck->OnPort!=0 && pTempSuck->Enable)       //Nickliu 20230306 add IO Use ISABase
                                flag=true;
                        }
                        break;
                    }
                    else if(pTempSuck!=NULL && S==pTempSuck->OffPortName)
                    {
                        PBtnPanelLan->OutRing  =pTempSuck->OffRing;
                        PBtnPanelLan->OutIP    =pTempSuck->OffIP;
                        PBtnPanelLan->OutPort  =pTempSuck->OffPort;
                        PBtnPanelLan->OutBit   =pTempSuck->OffBit;
                        PBtnPanelLan->OutType  =pTempSuck->OffType;
                        PBtnPanelLan->ISABase  =pTempSuck->ISABase;             //Nickliu 20230306 add IO Use ISABase
                        PBtnPanelLan->Enabled  =pTempSuck->Enable;
                        if(PBtnPanelLan->ISABase==eMotionNet ||
                           PBtnPanelLan->ISABase==ePCI1203)
                        {
                            if(pTempSuck->OffEnable)
                                flag=true;
                        }
                        else
                        {
                            if(pTempSuck->OffPort!=0 && pTempSuck->Enable)      //Nickliu 20230306 add IO Use ISABase
                                flag=true;
                        }
                        break;
                    }
                }
            }

            if(flag==false)
            {
                for(int i=0; i<MaxCylinderItem; i++)
                {
                    if(S==Cylinder[i].CylinderName)
                    {
                        PBtnPanelLan->OutRing  =Cylinder[i].OutRing;
                        PBtnPanelLan->OutIP    =Cylinder[i].OutIP;
                        PBtnPanelLan->OutPort  =Cylinder[i].OutPort;
                        PBtnPanelLan->OutBit   =Cylinder[i].OutBit;
                        PBtnPanelLan->OutType  =Cylinder[i].OutType;
                        PBtnPanelLan->ISABase  =Cylinder[i].ISABase;            //Nickliu 20230306 add IO Use ISABase
                        PBtnPanelLan->Enabled  =Cylinder[i].Enable;
                        if(PBtnPanelLan->ISABase==eMotionNet ||
                           PBtnPanelLan->ISABase==ePCI1203)
                        {
                            if(Cylinder[i].Enable==true)
                                flag=true;
                        }
                        else
                        {
                            if(Cylinder[i].OutPort!=0 &&
                               Cylinder[i].Enable==true)                        //Nickliu 20230306 add IO Use ISABase
                                flag=true;
                        }
                        break;
                    }
                }
            }

            if(flag==true)                                                      //JerryYang 20250107 : add to avoid alarm
            {
                if(PBtnPanelLan->ISABase==eMotionNet ||                         //Nickliu 20230306 add IO Use ISABase
                   PBtnPanelLan->ISABase==ePCI1203)                             //Sam 20230724 : 新增 PCI1203 IO 模組
                {
                    ret=MyLaneIO.IOOutBitStatus(PBtnPanelLan->OutRing, PBtnPanelLan->OutIP, PBtnPanelLan->OutPort, PBtnPanelLan->OutBit, PBtnPanelLan->ISABase, PBtnPanelLan->Alias);  //Sam 20230724 : 新增 PCI1203 IO 模組
                    iRing=PBtnPanelLan->OutRing;                                //Steven 20240103 : 加入保護
                    iIP  =PBtnPanelLan->OutIP;
                    iPort=PBtnPanelLan->OutPort;
                }
                else                                                            //Nickliu 20230306 add IO Use ISABase
                {
                    iRing=0;                                                    //Steven 20240103 : 加入保護
                    iIP  =PBtnPanelLan->OutPort/32;
                    iPort=PBtnPanelLan->OutPort%32;
                    ret=IOOutBitStatus(PBtnPanelLan->OutPort, PBtnPanelLan->OutBit);
                }
            }                                                                   //Nickliu 20230306 add IO Use ISABase

            if(PBtnPanelLan->OutType)
                PBtnPanelLan->Down=ret;
            else
                PBtnPanelLan->Down=!ret;

            try
            {
                #ifndef SOFT_SIMULTE
                if(flag==false)
                {
                    PBtnPanelLan->FalseColor=clSilver;
                    PBtnPanelLan->TrueColor =clSilver;
                }
                else if(PBtnPanelLan->OutRing>=0 && PBtnPanelLan->OutRing<4  &&                                         //Steven 20240103 : 加入保護
                        PBtnPanelLan->OutIP>=0   && PBtnPanelLan->OutIP<64   &&
                        PBtnPanelLan->OutPort>=0 && PBtnPanelLan->OutPort<32 &&
                        PBtnPanelLan->OutBit>=0  && PBtnPanelLan->OutBit<8   &&
                        bIdleNeedCheckSafeDoor[iRing][iIP][iPort][PBtnPanelLan->OutBit])                                //JimmyChiu 20240426 : Fixed for IO display
                {
                    PBtnPanelLan->FalseColor    =(TColor)0x001865EA;
                    PBtnPanelLan->TrueColor     =(TColor)0x0003DFFD;
                    PBtnPanelLan->TrueFontColor =clBlack;
                    PBtnPanelLan->FalseFontColor=clBlack;
                }
                else
                {
                    PBtnPanelLan->FalseColor    =(TColor)0x009D4701;
                    PBtnPanelLan->TrueColor     =(TColor)0x00DCB505;
                    PBtnPanelLan->TrueFontColor =clWhite;
                    PBtnPanelLan->FalseFontColor=clWhite;
                }
                #endif
            }
            catch(...)
            {
            };
        }
        else if(PBtnPanel!=NULL)
        {
            S=PBtnPanel->Alias;
            flag=false;
            for(int i=0; i<MAX_SWITCH_ITEM; i++)
            {
                if(S==SW[i].Name)
                {
                    PBtnPanel->OutPort=SW[i].Port;
                    PBtnPanel->OutBit =SW[i].Bit;
                    PBtnPanel->OutType=SW[i].Type;
                    PBtnPanel->Enabled=SW[i].Enable;
                    if(SW[i].Port!=0 && SW[i].Enable==true)
                        flag=true;
                    break;
                }
            }

            if(flag==false)
            {
                for(int i=0; i<pSuck->Count; i++)
                {
                    pTempSuck=(TMySucker*)pSuck->Items[i];
                    if(pTempSuck!=NULL && S==pTempSuck->OnPortName)
                    {
                        PBtnPanel->OutPort=pTempSuck->OnPort;
                        PBtnPanel->OutBit =pTempSuck->OnBit;
                        PBtnPanel->OutType=pTempSuck->OnType;
                        PBtnPanel->Enabled=pTempSuck->Enable;
                        if(pTempSuck->OnPort!=0 && pTempSuck->Enable)
                            flag=true;
                        break;
                    }
                    else if(pTempSuck!=NULL && S==pTempSuck->OffPortName)
                    {
                        PBtnPanel->OutPort=pTempSuck->OffPort;
                        PBtnPanel->OutBit =pTempSuck->OffBit;
                        PBtnPanel->OutType=pTempSuck->OffType;
                        PBtnPanel->Enabled=pTempSuck->Enable;
                        if(pTempSuck->OffPort!=0 && pTempSuck->Enable)
                            flag=true;
                        break;
                    }
                }
            }

            if(flag==false)
            {
                for(int i=0; i<MaxCylinderItem; i++)
                {
                    if(S==Cylinder[i].CylinderName)
                    {
                        PBtnPanel->OutPort=Cylinder[i].OutPort;
                        PBtnPanel->OutBit =Cylinder[i].OutBit;
                        PBtnPanel->OutType=Cylinder[i].OutType;
                        PBtnPanel->Enabled=Cylinder[i].Enable;
                        if(Cylinder[i].OutPort!=0 && Cylinder[i].Enable==true)
                            flag=true;
                        break;
                    }
                }
            }
            ret=IOOutBitStatus(PBtnPanel->OutPort, PBtnPanel->OutBit);
            if(PBtnPanel->OutType)
                PBtnPanel->Down=ret;
            else
                PBtnPanel->Down=!ret;

            iRing=0;                                                            //Steven 20240103 : 加入保護
            iIP  =PBtnPanel->OutPort/32;
            iPort=PBtnPanel->OutPort%32;

            try
            {
                if(iIP>=0 && iIP<=64 &&
                   PBtnPanel->OutBit>=0 && PBtnPanel->OutBit<8   &&             //Steven 20240103 : 加入保護
                   bIdleNeedCheckSafeDoor[iRing][iIP][iPort][PBtnPanel->OutBit])
                {
                    PBtnPanel->FalseColor    =(TColor)0x001865EA;
                    PBtnPanel->TrueColor     =(TColor)0x0003DFFD;
                    PBtnPanel->TrueFontColor =clBlack;
                    PBtnPanel->FalseFontColor=clBlack;
                }
                else
                {
                    PBtnPanel->FalseColor    =(TColor)0x009D4701;
                    PBtnPanel->TrueColor     =(TColor)0x00DCB505;
                    PBtnPanel->TrueFontColor =clWhite;
                    PBtnPanel->FalseFontColor=clWhite;
                }

                #ifndef SOFT_SIMULTE
                if(flag==false)
                {
                    PBtnPanel->FalseColor=clSilver;
                    PBtnPanel->TrueColor =clSilver;
                }
                #endif
            }
            catch(...)
            {
            };
        }
        else if(PMyLedLan!=NULL)
        {
            PMyLedLan->Blink=false;
            S.sprintf("%s", PMyLedLan->Alias);
            flag=false;
            for(int i=0; i<MAX_SENSOR_ITEM; i++)
            {
                if(S==Sen[i].Name)
                {
                    PMyLedLan->InRing      =Sen[i].Ring;
                    PMyLedLan->InIP        =Sen[i].IP;
                    PMyLedLan->InPort      =Sen[i].Port;
                    PMyLedLan->InBit       =Sen[i].Bit;
                    PMyLedLan->InType      =Sen[i].Type;
                    PMyLedLan->ISABase     =Sen[i].ISABase;                     //Nickliu 20230306 add IO Use ISABase

                    if(PMyLedLan!=NULL &&                                       //Steven 20240103 : 加入保護
                       (PMyLedLan->ISABase==eMotionNet ||
                        PMyLedLan->ISABase==ePCI1203))
                    {
                        if(Sen[i].Using!="" && Sen[i].Enable==true)
                            flag=true;
                    }
                    else
                    {
                        if(Sen[i].Enable==true)                                 //Nickliu 20230306 add IO Use ISABase
                            flag=true;
                    }

                    if(Sen[i].Enable==true && flag==true)                       //Steven 20240710 : 修正IO顯示
                    {
                        PMyLedLan->Enabled   =true;
                        PMyLedLan->Visible   =true;
                        PMyLedLan->FalseColor=clSilver;
                        PMyLedLan->TrueColor =clLime;
                    }
                    else
                    {
                        PMyLedLan->Enabled   =true;
                        PMyLedLan->Visible   =true;
                        PMyLedLan->FalseColor=clRed;
                        PMyLedLan->TrueColor =clRed;
                    #ifdef SOFT_SIMULTE
                        PMyLedLan->Enabled   =false;
                    #else
                        PMyLedLan->Visible   =false;
                    #endif
                    }
                    break;
                }
            }

            if(flag==false)
            {
                for(int i=0; i<pSuck->Count; i++)
                {
                    pTempSuck=(TMySucker*)pSuck->Items[i];
                    if(pTempSuck!=NULL && S==pTempSuck->SensorName)
                    {
                        PMyLedLan->InRing      =pTempSuck->SenRing;
                        PMyLedLan->InIP        =pTempSuck->SenIP;
                        PMyLedLan->InPort      =pTempSuck->SenPort;
                        PMyLedLan->InBit       =pTempSuck->SenBit;
                        PMyLedLan->InType      =pTempSuck->SenType;
                        PMyLedLan->ISABase     =pTempSuck->ISABase;             //Nickliu 20230313 add IO Use ISABase

                        if(pTempSuck->SenUsing!="")
                            flag=true;

                        if(pTempSuck->Enable==true && flag==true)               //Steven 20240710 : 修正IO顯示
                        {
                            PMyLedLan->Enabled   =true;
                            PMyLedLan->Visible   =true;
                            PMyLedLan->FalseColor=clSilver;
                            PMyLedLan->TrueColor =clLime;
                        }
                        else
                        {
                            PMyLedLan->Enabled   =true;
                            PMyLedLan->Visible   =true;
                            PMyLedLan->FalseColor=clRed;
                            PMyLedLan->TrueColor =clRed;
                            #ifdef SOFT_SIMULTE
                                PMyLedLan->Enabled   =false;
                            #else
                                PMyLedLan->Visible   =false;
                            #endif
                        }
                        break;
                    }
                }
            }

            if(flag==false)
            {
                for(int i=0; i<MaxCylinderItem; i++)
                {
                    if(S==Cylinder[i].OnSensorName)
                    {
                        PMyLedLan->InRing  =Cylinder[i].OnSenRing;
                        PMyLedLan->InIP    =Cylinder[i].OnSenIP;
                        PMyLedLan->InPort  =Cylinder[i].OnSenPort;
                        PMyLedLan->InBit   =Cylinder[i].OnSenBit;
                        PMyLedLan->InType  =Cylinder[i].OnSenType;
                        PMyLedLan->ISABase =Cylinder[i].ISABase;

                        /*if(IO_CARD_TYPE==NewIO_MN200 ||
                           IO_CARD_TYPE==PCI_P64C64)                            //Steven 20231218 HT7080B
                        {
                            flag=Cylinder[i].OnSenEnable;
                        }
                        else
                        {
                            if(Cylinder[i].OnSenEnable)
                                flag=true;
                        }  */

                        if(Cylinder[i].OnSenEnable)                             //Steven 20250109 : 修正IO顯示
                        {
                            PMyLedLan->Enabled   =true;
                            PMyLedLan->Visible   =true;
                            PMyLedLan->FalseColor=clSilver;
                            PMyLedLan->TrueColor =clLime;
                        }
                        else
                        {
                            PMyLedLan->Enabled   =true;
                            PMyLedLan->Visible   =true;
                            PMyLedLan->FalseColor=clRed;
                            PMyLedLan->TrueColor =clRed;
                            #ifdef SOFT_SIMULTE
                                PMyLedLan->Enabled   =false;
                            #else
                                PMyLedLan->Visible   =false;
                            #endif
                        }
                        break;
                    }
                    else if(S==Cylinder[i].OffSensorName)
                    {
                        PMyLedLan->InRing  =Cylinder[i].OffSenRing;
                        PMyLedLan->InIP    =Cylinder[i].OffSenIP;
                        PMyLedLan->InPort  =Cylinder[i].OffSenPort;
                        PMyLedLan->InBit   =Cylinder[i].OffSenBit;
                        PMyLedLan->InType  =Cylinder[i].OffSenType;
                        PMyLedLan->ISABase =Cylinder[i].ISABase;                //Nickliu 20230313 add IO Use ISABase for cylinder

                        /*if(IO_CARD_TYPE==NewIO_MN200 ||
                           IO_CARD_TYPE==PCI_P64C64)                            //Steven 20231218 HT7080B
                        {
                            flag=Cylinder[i].OffSenEnable;
                        }
                        else
                        {
                            if(Cylinder[i].OffSenEnable)
                                flag=true;
                        }*/

                        if(Cylinder[i].OffSenEnable)                            //Steven 20250109 : 修正IO顯示
                        {
                            PMyLedLan->Enabled   =true;
                            PMyLedLan->Visible   =true;
                            PMyLedLan->FalseColor=clSilver;
                            PMyLedLan->TrueColor =clLime;
                        }
                        else
                        {
                            PMyLedLan->Enabled   =true;
                            PMyLedLan->Visible   =true;
                            PMyLedLan->FalseColor=clRed;
                            PMyLedLan->TrueColor =clRed;
                            #ifdef SOFT_SIMULTE
                                PMyLedLan->Enabled   =false;
                            #else
                                PMyLedLan->Visible   =false;
                            #endif
                        }
                        break;
                    }
                }
            }
        }
        else if(PMyLed!=NULL)
        {
            PMyLed->Blink=false;
            S=PMyLed->Alias;
            if(S=="SenBit33")
            {
                S=PMyLed->Alias;
            }
            flag=false;
            for(int i=0; i<MAX_SENSOR_ITEM; i++)
            {
                if(S==Sen[i].Name)
                {
                    PMyLed->InPort =Sen[i].Port;
                    PMyLed->InBit  =Sen[i].Bit;
                    PMyLed->InType =Sen[i].Type;
                    PMyLed->Enabled=Sen[i].Enable;
                    if(PMyLed->InPort!=0 && Sen[i].Enable==true)
                        flag=true;
                    break;
                }
            }

            if(flag==false)
            {
                for(int i=0; i<pSuck->Count; i++)
                {
                    pTempSuck=(TMySucker*)pSuck->Items[i];
                    if(pTempSuck!=NULL && S==pTempSuck->SensorName)
                    {
                        PMyLed->InPort =pTempSuck->OnPort;
                        PMyLed->InBit  =pTempSuck->OnBit ;
                        PMyLed->InType =pTempSuck->OnType;
                        PMyLed->Enabled=pTempSuck->Enable;
                        if(PMyLed->InPort!=0)
                            flag=true;
                        break;
                    }
                }
            }

            if(flag==false)
            {
                for(int i=0; i<MaxCylinderItem; i++)
                {
                    if(S==Cylinder[i].OnSensorName)
                    {
                        PMyLed->InPort =Cylinder[i].OnSenPort;
                        PMyLed->InBit  =Cylinder[i].OnSenBit;
                        PMyLed->InType =Cylinder[i].OnSenType;
                        PMyLed->Enabled=Cylinder[i].Enable;
                        if(PMyLed->InPort!=0)
                            flag=true;

                        flag=true;
                        break;
                    }
                    else if(S==Cylinder[i].OffSensorName)
                    {
                        PMyLed->InPort =Cylinder[i].OffSenPort;
                        PMyLed->InBit  =Cylinder[i].OffSenBit;
                        PMyLed->InType =Cylinder[i].OffSenType;
                        PMyLed->Enabled=Cylinder[i].Enable;
                        if(PMyLed->InPort!=0)
                            flag=true;
                        break;
                    }
                }
            }
            try
            {
                if(PMyLed->Enabled)                                             //JummyChiu 20240427 : Fixed for IO view
                {
                    PMyLed->FalseColor=clSilver;
                    PMyLed->TrueColor=clLime;

                    #ifndef SOFT_SIMULTE
                    if(flag==false)
                    {
                        PMyLed->FalseColor=clRed;
                        PMyLed->TrueColor=clRed;
                    }
                    #endif
                }
                else
                {
                    #ifndef SOFT_SIMULTE
                    PMyLed->FalseColor=clRed;
                    PMyLed->TrueColor =clRed;
                    if(PMyLed->Visible==true)
                        PMyLed->Visible=false;
                    #endif
                }
            }
            catch(...)
            {
                S=PMyLed->Alias;
            }
        }
    }
}
//---------------------------------------------------------------------------
void Tfiosetview::SetCompomentHint(TWinControl *PCtrl)
{
    AnsiString str="";

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TPanel        *>(P) != NULL ||
           dynamic_cast <TPageControl  *>(P) != NULL ||
           dynamic_cast <TTabSheet     *>(P) != NULL ||
           dynamic_cast <TTabControl   *>(P) != NULL ||
           dynamic_cast <TForm         *>(P) != NULL ||
           dynamic_cast <THeader       *>(P) != NULL ||
           dynamic_cast <TPage         *>(P) != NULL ||
           dynamic_cast <TGroupBox     *>(P) != NULL ||
           dynamic_cast <TScrollBox    *>(P) != NULL ||
           dynamic_cast <TRadioGroup   *>(P) != NULL)
        {
            SetCompomentHint((TWinControl *) P);                                // 找該物件附屬的物件(遞迴)
        }

        TBtnPanelLane   *PBtnPanelLan = dynamic_cast <TBtnPanelLane *>(P);
        TBtnPanel       *PBtnPanel    = dynamic_cast <TBtnPanel     *>(P);
        TMyLedLane      *PMyLedLan    = dynamic_cast <TMyLedLane    *>(P);
        TMyLed          *PMyLed       = dynamic_cast <TMyLed        *>(P);

        if(PBtnPanelLan!=NULL)
        {
            if(PBtnPanelLan->ISABase==eMotionNet ||
               PBtnPanelLan->ISABase==ePCI1203)
            {
                str.sprintf("(Lane:%d,IP:%d,Port:%d,Bit:%d) %s", PBtnPanelLan->OutRing, PBtnPanelLan->OutIP, PBtnPanelLan->OutPort, PBtnPanelLan->OutBit, PBtnPanelLan->Alias.c_str());
            }
            else
            {
                str.sprintf("(%3X0%d) %s", PBtnPanelLan->OutPort, PBtnPanelLan->OutBit, PBtnPanelLan->Alias.c_str());   //Nickliu 20230310 add IO View ISABase Type
            }
            PBtnPanelLan->ShowHint=true;
            PBtnPanelLan->Hint=str;
        }
        else if(PBtnPanel!=NULL)
        {
            str.sprintf("(%3X0%d)  %s", PBtnPanel->OutPort, PBtnPanel->OutBit, PBtnPanel->Alias.c_str());
            PBtnPanel->ShowHint=true;
            PBtnPanel->Hint=str;
        }
        else if(PMyLed!=NULL)
        {
            str.sprintf("(%3X0%d)  %s", PMyLed->InPort, PMyLed->InBit, PMyLed->Alias.c_str());
            PMyLed->ShowHint=true;
            PMyLed->Hint=str;
        }
        else if(PMyLedLan!=NULL)
        {
            if(PMyLedLan!=NULL &&                                               //JerryYang 20230915
               (PMyLedLan->ISABase==eMotionNet ||
                PMyLedLan->ISABase==ePCI1203))
            {
                str.sprintf("(Lane:%d,IP:%d,Port:%d,Bit:%d) %s", PMyLedLan->InRing, PMyLedLan->InIP, PMyLedLan->InPort, PMyLedLan->InBit, PMyLedLan->Alias.c_str());
            }
            else
            {
                str.sprintf("(%3X0%d) %s", PMyLedLan->InPort, PMyLedLan->InBit ,PMyLedLan->Alias.c_str());              //Nickliu 20230310 add IO View ISABase Type
            }
            PMyLedLan->ShowHint=true;
            PMyLedLan->Hint=str;
        }
    }
}
//---------------------------------------------------------------------------
void Tfiosetview::ScanLed(TWinControl *PCtrl)
{
    bool flag;
    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P = PCtrl->Controls[iP];
        if(dynamic_cast <TPanel        *>(P) != NULL ||
           dynamic_cast <TPageControl  *>(P) != NULL ||
           dynamic_cast <TTabSheet     *>(P) != NULL ||
           dynamic_cast <TTabControl   *>(P) != NULL ||
           dynamic_cast <TForm         *>(P) != NULL ||
           dynamic_cast <THeader       *>(P) != NULL ||
           dynamic_cast <TPage         *>(P) != NULL ||
           dynamic_cast <TGroupBox     *>(P) != NULL ||
           dynamic_cast <TScrollBox    *>(P) != NULL ||
           dynamic_cast <TRadioGroup   *>(P) != NULL )
        {
            ScanLed((TWinControl *) P);                                         // 找該物件附屬的物件(遞迴)
        }

        TMyLedLane *PMyLedLan = dynamic_cast <TMyLedLane *>(P);
        TMyLed     *PMyLed    = dynamic_cast <TMyLed     *>(P);
        if(PMyLed!=NULL)
        {
            if(TTL_CARD_TYPE>0)                                                 //Steven 20200901 : Win10 & Win7也可以用TTL
                continue;

            flag=IOInputBit(PMyLed->InPort, PMyLed->InBit);
            if(PMyLed->InType==0)
                flag=!flag;
            if(flag)
                PMyLed->Value=true;
            else
                PMyLed->Value=false;
        }
        else if(PMyLedLan!=NULL)
        {
            if(iControlPanelMode==1 &&
               fPadInterface->IsPadKey(PMyLedLan->Alias))                       //Ken 20210702 AddPadInterface
            {
                flag=fPadInterface->ProcessScanKey(PMyLedLan->Alias);
            }
            else if(PMyLedLan->ISABase==eMotionNet ||                           //Nickliu 20230309 add Cylinder Led ISABase Type
                    PMyLedLan->ISABase==ePCI1203 ||                             //Sam 20230724 : 新增 PCI1203 IO 模組
                    PMyLedLan->ISABase==ePLCbase)
            {
                flag=MyLaneIO.IOInputBit(PMyLedLan->InRing, PMyLedLan->InIP, PMyLedLan->InPort, PMyLedLan->InBit, PMyLedLan->ISABase, PMyLedLan->Alias);        //Sam 20230724 : 新增 PCI1203 IO 模組
            }
            else if(PMyLedLan->ISABase==eISABase ||
                    PMyLedLan->ISABase==ePCI1735U)                              //Nickliu 20230309 add Cylinder Led ISABase Type
            {
                flag=IOInputBit(PMyLedLan->InPort, PMyLedLan->InBit);
            }

            if(PMyLedLan->InType==0)
                flag=!flag;
            if(flag)
                PMyLedLan->Value=true;
            else
                PMyLedLan->Value=false;
        }
    }
}
//------------------------------------------------------------------------------
void Tfiosetview::CheckIndexSuck()                                              //Steven 20110520
{
    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)
        {
            if(FTestSuck.Item[i][j]==NULL_IC && FTestSuck.Suck[i][j].GetOnBit())
            {
                FTestSuck.Suck[i][j].Normal();
            }

            if(BTestSuck.Item[i][j]==NULL_IC && BTestSuck.Suck[i][j].GetOnBit())
            {
                BTestSuck.Suck[i][j].Normal();
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btIPSettingClick(TObject *Sender)
{
    AnsiString ImgPath=BmpPath+"IP Setting.bmp";
    if(FileExists(ImgPath))
        imgOther->Picture->LoadFromFile(ImgPath);
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btComPoetClick(TObject *Sender)
{
    AnsiString ImgPath=BmpPath+"COMSetting.bmp";
    if(FileExists(ImgPath))
        imgOther->Picture->LoadFromFile(ImgPath);
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btKVMClick(TObject *Sender)
{
    AnsiString ImgPath;
    int iPCCount=1;

    if(ATC_SYSTEM!=eNonChamber && ATC_SYSTEM!=eATCSiliconType && ATC_SYSTEM!=eATCUninstall)
        iPCCount++;

    if(REAL_TIME_CCD)
        iPCCount++;

    if(BAR_CODE_INSTALL==ebctUseCCDMode)
        iPCCount++;

    if(iPCCount<3)
        ImgPath=BmpPath+"KVM_2PC.bmp";
    else
        ImgPath=BmpPath+"KVM_3PC.bmp";

    if(FileExists(ImgPath))
        imgOther->Picture->LoadFromFile(ImgPath);
}
//------------------------------------------------------------------------------
void Tfiosetview::LabSiteMap()
{
    int iCH[2][4];
    AnsiString asString[9]= {"Aa", "Ab", "Ac", "Ad", "Ba", "Bb", "Bc", "Bd", "X"};
    AnsiString asString1[9]={"Aa", "Ab", "Ac", "Ba", "Bb", "Bc", "X", "X", "X"};

    //待Frank新增
    if((TTLCfg.iCateBitLength==_8Bit || TTLCfg.iCateBitLength==_10Bit ||
        TTLCfg.iCateBitLength==_10BitPE || TTLCfg.iCateBitLength==_10BitPO))
    {
        lblTTL_Aa->Visible=false;
        lblTTL_Ab->Visible=false;
        lblTTL_Ac->Visible=false;
        lblTTL_Ad->Visible=false;
        lblTTL_Ba->Visible=false;
        lblTTL_Bb->Visible=false;
        lblTTL_Bc->Visible=false;
        lblTTL_Bd->Visible=false;
        //===============================================
        lblTTLInAa->Visible=false;
        lblTTLInAb->Visible=false;
        lblTTLInAc->Visible=false;
        lblTTLInAd->Visible=false;
        lblTTLInBa->Visible=false;
        lblTTLInBb->Visible=false;
        lblTTLInBc->Visible=false;
        lblTTLInBd->Visible=false;
    }
    else
    {
        switch(TestIF_File.iTestMode)
        {
            case SingleSite:
            case DualSite:
//            case DualSiteBS:
            case TriSite1X3:
            case QualSite1X4:
            case DualSite2x1:
            case QualSite2X2:
//            case QualSite2X2BS:
            case _8Site1X4:

                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        if(TestIF_File.iSiteMap[i][j]>0)
                            iCH[i][j]=TestIF_File.iSiteMap[i][j]-1;
                        else
                            iCH[i][j]=8;
                    }
                }
                lblTTL_Aa->Caption=asString[iCH[0][0]];
                lblTTL_Ab->Caption=asString[iCH[0][1]];
                lblTTL_Ac->Caption=asString[iCH[0][2]];
                lblTTL_Ad->Caption=asString[iCH[0][3]];
                lblTTL_Ba->Caption=asString[iCH[1][0]];
                lblTTL_Bb->Caption=asString[iCH[1][1]];
                lblTTL_Bc->Caption=asString[iCH[1][2]];
                lblTTL_Bd->Caption=asString[iCH[1][3]];
                //===============================================
                lblTTLInAa->Caption=asString[iCH[0][0]];
                lblTTLInAb->Caption=asString[iCH[0][1]];
                lblTTLInAc->Caption=asString[iCH[0][2]];
                lblTTLInAd->Caption=asString[iCH[0][3]];
                lblTTLInBa->Caption=asString[iCH[1][0]];
                lblTTLInBb->Caption=asString[iCH[1][1]];
                lblTTLInBc->Caption=asString[iCH[1][2]];
                lblTTLInBd->Caption=asString[iCH[1][3]];
                break;
            case _6Site2X3:
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        if(TestIF_File.iSiteMap[i][j]>0)
                            iCH[i][j]=TestIF_File.iSiteMap[i][j]-1;
                        else
                            iCH[i][j]=8;
                    }
                }

                lblTTL_Aa->Caption=asString1[iCH[0][0]];
                lblTTL_Ab->Caption=asString1[iCH[1][0]];
                lblTTL_Ac->Caption=asString1[iCH[0][1]];
                lblTTL_Ad->Caption=asString1[iCH[0][3]];
                lblTTL_Ba->Caption=asString1[iCH[0][2]];
                lblTTL_Bb->Caption=asString1[iCH[1][2]];
                lblTTL_Bc->Caption=asString1[iCH[1][1]];
                lblTTL_Bd->Caption=asString1[iCH[1][3]];
                //===============================================
                lblTTLInAa->Caption=asString1[iCH[0][0]];
                lblTTLInAb->Caption=asString1[iCH[1][0]];
                lblTTLInAc->Caption=asString1[iCH[0][1]];
                lblTTLInAd->Caption=asString1[iCH[0][3]];
                lblTTLInBa->Caption=asString1[iCH[0][2]];
                lblTTLInBb->Caption=asString1[iCH[1][2]];
                lblTTLInBc->Caption=asString1[iCH[1][1]];
                lblTTLInBd->Caption=asString1[iCH[1][3]];
                break;
            case _32Site4X8N:
                lblTTL_Aa->Caption="Aa";
                lblTTL_Ab->Caption="Aa";
                lblTTL_Ac->Caption="Aa";
                lblTTL_Ad->Caption="Aa";
                lblTTL_Ba->Caption="Aa";
                lblTTL_Bb->Caption="Aa";
                lblTTL_Bc->Caption="Aa";
                lblTTL_Bd->Caption="Aa";
                break;
            case _32Site4X8M:
                lblTTL_Aa->Caption="Aa";
                lblTTL_Ab->Caption="Aa";
                lblTTL_Ac->Caption="Aa";
                lblTTL_Ad->Caption="Aa";
                lblTTL_Ba->Caption="Aa";
                lblTTL_Bb->Caption="Aa";
                lblTTL_Bc->Caption="Aa";
                lblTTL_Bd->Caption="Aa";
                break;
        }
    }
}
//------------------------------------------------------------------------------
void Tfiosetview::LoadIoTable(int iType, int iLane, int iIP)
{
    AnsiString sType[4]={AnsiString("Sensor"), AnsiString("Sucker"), AnsiString("Switch"), AnsiString("Cylinder")};
    int iSize=HSys.IOTable.size();
    int iRowCount=0;
    bool bAdd=false;
    iLane--;
    iType--;

    TStringList *Strs=new TStringList();

    strngrdIoTable->ColCount=16;
    strngrdIoTable->ColWidths[ 0]=50;
    strngrdIoTable->ColWidths[ 1]=100;
    strngrdIoTable->ColWidths[ 2]=200;
    strngrdIoTable->ColWidths[ 3]=50;
    strngrdIoTable->ColWidths[ 4]=50;
    strngrdIoTable->ColWidths[ 5]=50;
    strngrdIoTable->ColWidths[ 6]=50;
    strngrdIoTable->ColWidths[ 7]=50;
    strngrdIoTable->ColWidths[ 8]=50;
    strngrdIoTable->ColWidths[ 9]=50;
    strngrdIoTable->ColWidths[10]=50;
    strngrdIoTable->ColWidths[11]=50;
    strngrdIoTable->ColWidths[12]=50;
    strngrdIoTable->ColWidths[13]=50;
    strngrdIoTable->ColWidths[14]=50;
    strngrdIoTable->ColWidths[15]=300;

    strngrdIoTable->Cells[ 0][0]="No";
    strngrdIoTable->Cells[ 1][0]="Type";
    strngrdIoTable->Cells[ 2][0]="Alias";
    strngrdIoTable->Cells[ 3][0]="Lane";
    strngrdIoTable->Cells[ 4][0]="ModuleType";
    strngrdIoTable->Cells[ 5][0]="IP";
    strngrdIoTable->Cells[ 6][0]="Port";
    strngrdIoTable->Cells[ 7][0]="Bit";
    strngrdIoTable->Cells[ 8][0]="InType";
    strngrdIoTable->Cells[ 9][0]="ISA Base";
    strngrdIoTable->Cells[10][0]="Enable";
    strngrdIoTable->Cells[11][0]="OnAlarmTime";
    strngrdIoTable->Cells[12][0]="OffAlarmTime";
    strngrdIoTable->Cells[13][0]="OnDelayTime";
    strngrdIoTable->Cells[14][0]="OffDelayTime";
    strngrdIoTable->Cells[15][0]="Note";

    for(int i=1; i<strngrdIoTable->RowCount; i++)
    {
        for(int j=0; j<16; j++)
            strngrdIoTable->Cells[j][i]="";
    }

    if(iType<0 && iLane<0)
    {
        strngrdIoTable->RowCount=iSize+1;
    }
    else
    {
        strngrdIoTable->RowCount=1;
        iRowCount=0;
    }

    for(int i=0; i<iSize; i++)
    {
        if(iType<0 && iLane<0)
        {
            Strs->CommaText=HSys.IOTable[i]->_CommaText;
            for(int j=0; j<Strs->Count; j++)
            {
                strngrdIoTable->Cells[j+1][i+1]=Strs->Strings[j];
            }
            strngrdIoTable->Cells[0][i+1]=HSys.IOTable[i]->Tag;
        }
        else
        {
            if(i==0)
            {
                iRowCount++;
            }
            else if(iType>=0)
            {
                if(HSys.IOTable[i]->Type.AnsiPos(sType[iType])!=0)
                {
                    if(iLane<0 || HSys.IOTable[i]->iLane==iLane)
                    {
                        bAdd=true;
                    }
                }
            }
            else
            {
                if(iLane<0 || HSys.IOTable[i]->iLane==iLane)
                {
                    bAdd=true;
                }
            }

            if(bAdd)
            {
                strngrdIoTable->RowCount=iRowCount+1;
                strngrdIoTable->Cells[ 0][iRowCount]=HSys.IOTable[i]->Tag;
                strngrdIoTable->Cells[ 1][iRowCount]=HSys.IOTable[i]->Type;
                strngrdIoTable->Cells[ 2][iRowCount]=HSys.IOTable[i]->Alias;
                strngrdIoTable->Cells[ 3][iRowCount]=(HSys.IOTable[i]->iLane        ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iLane);
                strngrdIoTable->Cells[ 4][iRowCount]=(HSys.IOTable[i]->iModuleType  ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iModuleType);
                strngrdIoTable->Cells[ 5][iRowCount]=(HSys.IOTable[i]->iIP          ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iIP);
                strngrdIoTable->Cells[ 7][iRowCount]=(HSys.IOTable[i]->iBit         ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iBit);
                strngrdIoTable->Cells[ 8][iRowCount]=(HSys.IOTable[i]->iInType      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iInType);
                strngrdIoTable->Cells[ 9][iRowCount]=(HSys.IOTable[i]->iISABase     ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iISABase);

                if(HSys.IOTable[i]->iISABase==ePLCbase)                         //jou 20231016 : 修正CE PLC IO load table error
                    strngrdIoTable->Cells[ 6][iRowCount]=(HSys.IOTable[i]->iPort        ==-1)?AnsiString(""):"0x"+IntToHex(HSys.IOTable[i]->iPort,3);
                else
                    strngrdIoTable->Cells[ 6][iRowCount]=(HSys.IOTable[i]->iPort        ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iPort);

                strngrdIoTable->Cells[10][iRowCount]=(HSys.IOTable[i]->iEnable      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iEnable);
                strngrdIoTable->Cells[11][iRowCount]=(HSys.IOTable[i]->iOnAlarmTime ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOnAlarmTime);
                strngrdIoTable->Cells[12][iRowCount]=(HSys.IOTable[i]->iOffAlarmTime==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOffAlarmTime);
                strngrdIoTable->Cells[13][iRowCount]=(HSys.IOTable[i]->iOnDelayTime ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOnDelayTime);
                strngrdIoTable->Cells[14][iRowCount]=(HSys.IOTable[i]->iOffDelayTime==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOffDelayTime);
                iRowCount++;
                bAdd=false;
            }
        }
    }

    if(strngrdIoTable->RowCount<=1)
        strngrdIoTable->RowCount=2;
    strngrdIoTable->FixedRows=1;
    Strs->Clear();
    delete Strs;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::cbbTypeChange(TObject *Sender)
{
    LoadIoTable(cbbType->ItemIndex, cbbLane->ItemIndex, 0);
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnModifyClick(TObject *Sender)
{
    if(iSelectRow>0 && iSelectCol>0)
    {
        edtTemp->Text=strngrdIoTable->Cells[iSelectCol][iSelectRow];
        if(strngrdIoTable->Cells[iSelectCol][0].AnsiPos(AnsiString("Type"))!=0  ||
           strngrdIoTable->Cells[iSelectCol][0].AnsiPos(AnsiString("Alias"))!=0 ||
           strngrdIoTable->Cells[iSelectCol][0].AnsiPos(AnsiString("Note"))!=0)
        {
            fQwertyKey->ShowQwertyKey(edtTemp, N_NO_SYMBOL|N_NO_SPACE);
        }
        else
        {
            fQwertyKey->ShowQwertyKey(edtTemp, N_INTEGER);
        }
        strngrdIoTable->Cells[iSelectCol][iSelectRow]=edtTemp->Text;
    }
    btnModify->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::strngrdIoTableSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    iSelectRow=ARow;
    iSelectCol=ACol;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::strngrdIoTableDblClick(TObject *Sender)
{
    btnModify->Click();
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnAddIOClick(TObject *Sender)
{
    int iRow=strngrdIoTable->RowCount;
    strngrdIoTable->RowCount=iRow+1;
    for(int j=0; j<strngrdIoTable->ColCount; j++)
    {
        strngrdIoTable->Cells[j][iRow]="";
    }
    btnAddIO->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnDeleteIOClick(TObject *Sender)
{
    if(iSelectRow==0)
        return;
    Tag=atoi(strngrdIoTable->Cells[0][iSelectRow].c_str());
    for(int i=iSelectRow; i<strngrdIoTable->RowCount-1; i++)
    {
        for(int j=0; j<strngrdIoTable->ColCount; j++)
        {
            strngrdIoTable->Cells[j][i]=strngrdIoTable->Cells[j][i+1];
        }
    }
    strngrdIoTable->RowCount=strngrdIoTable->RowCount-1;
    HSys.IOTable[Tag]->bDelete=true;
    btnDeleteIO->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::sbUpdateClick(TObject *Sender)
{
    int Tag;
    TStringList *sList=new TStringList();
    TStringList *SL=new TStringList();

    SL->Clear();
    SL->Add("IOType");
    SL->Add("Alias");
    SL->Add("Lane");
    SL->Add("ModuleType");
    SL->Add("IP");
    SL->Add("Port");
    SL->Add("Bit");
    SL->Add("InType");
    SL->Add("ISABase");
    SL->Add("Enable");
    SL->Add("OnAlarmTime");
    SL->Add("OffAlarmTime");
    SL->Add("OnDelayTime");
    SL->Add("OffDelayTime");
    SL->Add("Note");
    sList->Add(SL->CommaText);

    for(int i=1; i<strngrdIoTable->RowCount; i++)
    {
        if(strngrdIoTable->Cells[0][i]=="")
        {
            HSys.IOTable.push_back(new TIODATA(""));
            Tag=HSys.IOTable[HSys.IOTable.size()-1]->Tag;
        }
        else
        {
            Tag=atoi(strngrdIoTable->Cells[0][i].c_str());
        }

        HSys.IOTable[Tag]->Type=strngrdIoTable->Cells[1][i];
        HSys.IOTable[Tag]->Alias=strngrdIoTable->Cells[2][i];
        if(strngrdIoTable->Cells[ 3][i]!="")
            HSys.IOTable[Tag]->iLane=atoi(strngrdIoTable->Cells[3][i].c_str());
        else
            HSys.IOTable[Tag]->iLane=-1;
        if(strngrdIoTable->Cells[ 4][i]!="")
            HSys.IOTable[Tag]->iModuleType=atoi(strngrdIoTable->Cells[4][i].c_str());
        else
            HSys.IOTable[Tag]->iModuleType=-1;
        if(strngrdIoTable->Cells[ 5][i]!="")
            HSys.IOTable[Tag]->iIP=atoi(strngrdIoTable->Cells[5][i].c_str());
        else
            HSys.IOTable[Tag]->iIP=-1;

        if(strngrdIoTable->Cells[ 7][i]!="")
            HSys.IOTable[Tag]->iBit=atoi(strngrdIoTable->Cells[7][i].c_str());
        else
            HSys.IOTable[Tag]->iBit=-1;
        if(strngrdIoTable->Cells[ 8][i]!="")
            HSys.IOTable[Tag]->iInType=atoi(strngrdIoTable->Cells[8][i].c_str());
        else
            HSys.IOTable[Tag]->iInType=-1;

        if(strngrdIoTable->Cells[ 9][i]!="")
            HSys.IOTable[Tag]->iISABase=atoi(strngrdIoTable->Cells[9][i].c_str());
        else
            HSys.IOTable[Tag]->iISABase=-1;

        if(strngrdIoTable->Cells[ 6][i]!="")                                    //往下移到取得iISABase
        {
            if(HSys.IOTable[Tag]->iISABase==eISABase ||                         //Nickliu 20230310 add Suck ISABase Type
               HSys.IOTable[Tag]->iISABase==ePCI1735U ||
               HSys.IOTable[Tag]->iISABase==ePLCbase)
                HSys.IOTable[Tag]->iPort=HexStrToInt(strngrdIoTable->Cells[6][i].c_str());
            else
                HSys.IOTable[Tag]->iPort=atoi(strngrdIoTable->Cells[6][i].c_str());
        }
        else
        {
            HSys.IOTable[Tag]->iPort=-1;
        }

        if(strngrdIoTable->Cells[10][i]!="")
            HSys.IOTable[Tag]->iEnable=atoi(strngrdIoTable->Cells[10][i].c_str());
        else
            HSys.IOTable[Tag]->iEnable=-1;
        if(strngrdIoTable->Cells[11][i]!="")
            HSys.IOTable[Tag]->iOnAlarmTime=atoi(strngrdIoTable->Cells[11][i].c_str());
        else
            HSys.IOTable[Tag]->iOnAlarmTime=-1;
        if(strngrdIoTable->Cells[12][i]!="")
            HSys.IOTable[Tag]->iOffAlarmTime=atoi(strngrdIoTable->Cells[12][i].c_str());
        else
            HSys.IOTable[Tag]->iOffAlarmTime=-1;
        if(strngrdIoTable->Cells[13][i]!="")
            HSys.IOTable[Tag]->iOnDelayTime=atoi(strngrdIoTable->Cells[13][i].c_str());
        else
            HSys.IOTable[Tag]->iOnDelayTime=-1;
        if(strngrdIoTable->Cells[14][i]!="")
            HSys.IOTable[Tag]->iOffDelayTime=atoi(strngrdIoTable->Cells[14][i].c_str());
        else
            HSys.IOTable[Tag]->iOffDelayTime=-1;
    }

    for(unsigned int i=0; i<HSys.IOTable.size(); i++)
    {
        if(HSys.IOTable[i]->bDelete==false)
        {
            SL->Clear();
            SL->Add(HSys.IOTable[i]->Type);
            SL->Add(HSys.IOTable[i]->Alias);
            SL->Add((HSys.IOTable[i]->iLane        ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iLane));
            SL->Add((HSys.IOTable[i]->iModuleType  ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iModuleType));
            SL->Add((HSys.IOTable[i]->iIP          ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iIP));

            if(HSys.IOTable[i]->iISABase==ePLCbase)                             //jou 20231016 : 修正CE PLC IO load table error
                SL->Add((HSys.IOTable[i]->iPort    ==-1)?AnsiString(""):"0x"+IntToHex(HSys.IOTable[i]->iPort,3));
            else
                SL->Add((HSys.IOTable[i]->iPort    ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iPort));

            SL->Add((HSys.IOTable[i]->iBit         ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iBit));
            SL->Add((HSys.IOTable[i]->iInType      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iInType));
            SL->Add((HSys.IOTable[i]->iISABase     ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iISABase));
            SL->Add((HSys.IOTable[i]->iEnable      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iEnable));
            SL->Add((HSys.IOTable[i]->iOnAlarmTime ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOnAlarmTime));
            SL->Add((HSys.IOTable[i]->iOffAlarmTime==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOffAlarmTime));
            SL->Add((HSys.IOTable[i]->iOnDelayTime ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOnDelayTime));
            SL->Add((HSys.IOTable[i]->iOffDelayTime==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOffDelayTime));
            SL->Add("");
            sList->Add(SL->CommaText);
        }
    }

    sList->SaveToFile(IoTablePath);
    sList->Clear();
    SL->Clear();
    delete sList;
    delete SL;

    sbtReload->Click();
    sbUpdate->Down=false;
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::Label120Click(TObject *Sender)
{
    fContactForce->ReadFile();                                                  //kevin 20190114 add
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btnCloseClick(TObject *Sender)
{
    HWND hKeyboard=FindWindow(0, "IO check and verify");                        //Sam 20190218 : 變更關 Form 方式
    if(hKeyboard!=NULL)
        PostMessage(hKeyboard, WM_CLOSE, 0, 0);
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btnC_Load_UpClick(TObject *Sender)                 //Steven 20200724 : 修改Tray Z馬達速度設定
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(MOT[MLoaderZ].HomeFlag==false || IsIndexMotorOutOfPower())
    {
        lblLoaderZ->Visible=(MOT[MLoaderZ].HomeFlag==false);
        return;
    }
    lblLoaderZ->Visible=false;

    if(Ptr->Tag==0)
    {
        btnC_Load_Up->Down          =!btnC_Load_Up->Down;
        btnC_Load_Up_U->Down        =!btnC_Load_Up_U->Down;
    }
    else
    {
        btnC_Load_Middle->Down      =!btnC_Load_Middle->Down;
        btnC_Load_Middle_U->Down    =!btnC_Load_Middle_U->Down;
    }

    if((btnC_Load_Up->Down          &&
        btnC_Load_Middle->Down)     ||
       (btnC_Load_Up_U->Down        &&
        btnC_Load_Middle_U->Down))
    {
        MOT[MLoaderZ].MotorMove(Prod.TrayZ_Up[0]);
    }
    else if((btnC_Load_Up->Down==false      &&
             btnC_Load_Middle->Down==false) ||
            (btnC_Load_Up_U->Down==false    &&
             btnC_Load_Middle_U->Down==false))
    {
        MOT[MLoaderZ].MotorMove(0);
    }
    else if((btnC_Load_Up->Down==true       &&
             btnC_Load_Middle->Down==false) ||
            (btnC_Load_Up_U->Down==true &&
             btnC_Load_Middle_U->Down==false))
    {
        MOT[MLoaderZ].MotorMove(Prod.TrayZ_Mid[0]);
    }
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnC_Empty_UpClick(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(MOT[MEmptyZ].HomeFlag==false || IsIndexMotorOutOfPower())
    {
        lblEmptyZ->Visible=(MOT[MEmptyZ].HomeFlag==false);
        return;
    }
    lblEmptyZ->Visible=false;
    if(Ptr->Tag==0)
    {
        btnC_Empty_Up->Down         =!btnC_Empty_Up->Down;
        btnC_Empty_Up_U->Down       =!btnC_Empty_Up_U->Down;
    }
    else
    {
        btnC_Empty_Middle->Down     =!btnC_Empty_Middle->Down;
        btnC_Empty_Middle_U->Down   =!btnC_Empty_Middle_U->Down;
    }

    if((btnC_Empty_Up->Down         &&
        btnC_Empty_Middle->Down)    ||
       (btnC_Empty_Up_U->Down       &&
        btnC_Empty_Middle_U->Down))
    {
        MOT[MEmptyZ].MotorMove(Prod.TrayZ_Up[1]);
    }
    else if((btnC_Empty_Up->Down==false         &&
             btnC_Empty_Middle->Down==false)    ||
            (btnC_Empty_Up_U->Down==false       &&
             btnC_Empty_Middle_U->Down==false))
    {
        MOT[MEmptyZ].MotorMove(0);
    }
    else if((btnC_Empty_Up->Down==true          &&
             btnC_Empty_Middle->Down==false)    ||
            (btnC_Empty_Up_U->Down==true        &&
             btnC_Empty_Middle_U->Down==false))
    {
        MOT[MEmptyZ].MotorMove(Prod.TrayZ_Mid[1]);
    }
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnC_Color_UpClick(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(MOT[MColorZ].HomeFlag==false || IsIndexMotorOutOfPower())
    {
        lblColorZ->Visible=(MOT[MColorZ].HomeFlag==false);
        Ptr->Down=false;
        return;
    }
    lblColorZ->Visible=false;
    if(Ptr->Tag==0)
    {
        btnC_Color_Up->Down         =!btnC_Color_Up->Down;
        btnC_Color_Up_U->Down       =!btnC_Color_Up_U->Down;
    }
    else
    {
        btnC_Color_Middle->Down     =!btnC_Color_Middle->Down;
        btnC_Color_Middle_U->Down   =!btnC_Color_Middle_U->Down;
    }

    if((btnC_Color_Up->Down &&
        btnC_Color_Middle->Down) ||
       (btnC_Color_Up_U->Down &&
        btnC_Color_Middle_U->Down))
    {
        MOT[MColorZ].MotorMove(Prod.TrayZ_Up[2]);
    }
    else if((btnC_Color_Up->Down==false         &&
             btnC_Color_Middle->Down==false)    ||
            (btnC_Color_Up_U->Down==false   &&
             btnC_Color_Middle_U->Down==false))
    {
        MOT[MColorZ].MotorMove(0);
    }
    else if((btnC_Color_Up->Down==true &&
             btnC_Color_Middle->Down==false) ||
            (btnC_Color_Up_U->Down==true &&
             btnC_Color_Middle_U->Down==false))
    {
        MOT[MColorZ].MotorMove(Prod.TrayZ_Mid[2]);
    }
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btnC_Auto1_UpClick(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(MOT[MAuto1Z].HomeFlag==false || IsIndexMotorOutOfPower())
    {
        lblAuto1Z->Visible=(MOT[MAuto1Z].HomeFlag==false);
        Ptr->Down=false;
        return;
    }
    lblAuto1Z->Visible=false;
    if(Ptr->Tag==0)
    {
        btnC_Auto1_Up->Down         =!btnC_Auto1_Up->Down;
        btnC_Auto1_Up_U->Down       =!btnC_Auto1_Up_U->Down;
    }
    else
    {
        btnC_Auto1_Selector->Down   =!btnC_Auto1_Selector->Down;
        btnC_Auto1_Selector_U->Down =!btnC_Auto1_Selector_U->Down;
    }

    if((btnC_Auto1_Up->Down         &&
        btnC_Auto1_Selector->Down)  ||
       (btnC_Auto1_Up_U->Down       &&
        btnC_Auto1_Selector_U->Down))
    {
        MOT[MAuto1Z].MotorMove(Prod.TrayZ_Up[3]);
    }
    else if((btnC_Auto1_Up->Down==false         &&
             btnC_Auto1_Selector->Down==false)  ||
            (btnC_Auto1_Up_U->Down==false       &&
             btnC_Auto1_Selector_U->Down==false))
    {
        MOT[MAuto1Z].MotorMove(0);
    }
    else if((btnC_Auto1_Up->Down==true          &&
             btnC_Auto1_Selector->Down==false)  ||
            (btnC_Auto1_Up_U->Down==true        &&
             btnC_Auto1_Selector_U->Down==false))
    {
        MOT[MAuto1Z].MotorMove(Prod.TrayZ_Mid[3]);
    }
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btnC_Auto2_UpClick(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(MOT[MAuto2Z].HomeFlag==false || IsIndexMotorOutOfPower())
    {
        lblAuto2Z->Visible=(MOT[MAuto2Z].HomeFlag==false);
        Ptr->Down=false;
        return;
    }
    lblAuto2Z->Visible=false;
    if(Ptr->Tag==0)
    {
        btnC_Auto2_Up->Down         =!btnC_Auto2_Up->Down;
        btnC_Auto2_Up_U->Down       =!btnC_Auto2_Up_U->Down;
    }
    else
    {
        btnC_Auto2_Selector->Down   =!btnC_Auto2_Selector->Down;
        btnC_Auto2_Selector_U->Down =!btnC_Auto2_Selector_U->Down;
    }

    if((btnC_Auto2_Up->Down         &&
        btnC_Auto2_Selector->Down)  ||
       (btnC_Auto2_Up_U->Down       &&
        btnC_Auto2_Selector_U->Down))
    {
        MOT[MAuto2Z].MotorMove(Prod.TrayZ_Up[4]);
    }
    else if((btnC_Auto2_Up->Down==false         &&
             btnC_Auto2_Selector->Down==false)  ||
            (btnC_Auto2_Up_U->Down==false       &&
             btnC_Auto2_Selector_U->Down==false))
    {
        MOT[MAuto2Z].MotorMove(0);
    }
    else if((btnC_Auto2_Up->Down==true          &&
             btnC_Auto2_Selector->Down==false)  ||
            (btnC_Auto2_Up_U->Down==true        &&
             btnC_Auto2_Selector_U->Down==false))
    {
        MOT[MAuto2Z].MotorMove(Prod.TrayZ_Mid[4]);
    }
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btnC_Auto3_UpClick(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(MOT[MAuto3Z].HomeFlag==false || IsIndexMotorOutOfPower())
    {
        lblAuto3Z->Visible=(MOT[MAuto3Z].HomeFlag==false);
        Ptr->Down=false;
        return;
    }
    lblAuto3Z->Visible=false;

    if(IniConfig.bVTESTFunction==true && USE_AUTO_RETEST==eartUninstall)        //RogerYang 20250513 上海偉測Auto3Z異常，先把按鈕簡化，只有select作動，客回覆正常
    {
        if(Ptr->Tag==0)
        {
            btnC_Auto3_Up->Down         =!btnC_Auto3_Up->Down;
            return;
        }
        else
        {
            btnC_Auto3_Selector->Down   =!btnC_Auto3_Selector->Down;
        }

        if(btnC_Auto3_Selector->Down==true)
        {
            MOT[MAuto3Z].MotorMove(Prod.TrayZ_Up[5]);
        }
        else if(btnC_Auto3_Selector->Down==false)
        {
            MOT[MAuto3Z].MotorMove(0);
        }
    }
    else
    {
        if(Ptr->Tag==0)
        {
            btnC_Auto3_Up->Down         =!btnC_Auto3_Up->Down;
            btnC_Auto3_Up_U->Down       =!btnC_Auto3_Up_U->Down;
        }
        else
        {
            btnC_Auto3_Selector->Down   =!btnC_Auto3_Selector->Down;
            btnC_Auto3_Selector_U->Down =!btnC_Auto3_Selector_U->Down;
        }

        if((btnC_Auto3_Up->Down         &&
            btnC_Auto3_Selector->Down)  ||
           (btnC_Auto3_Up_U->Down       &&
            btnC_Auto3_Selector_U->Down))
        {
            MOT[MAuto3Z].MotorMove(Prod.TrayZ_Up[5]);
        }
        else if((btnC_Auto3_Up->Down==false         &&
                 btnC_Auto3_Selector->Down==false)  ||
                (btnC_Auto3_Up_U->Down==false       &&
                 btnC_Auto3_Selector_U->Down==false))
        {
            MOT[MAuto3Z].MotorMove(0);
        }
        else if((btnC_Auto3_Up->Down==true          &&
                 btnC_Auto3_Selector->Down==false)  ||
                (btnC_Auto3_Up_U->Down==true        &&
                 btnC_Auto3_Selector_U->Down==false))
        {
            MOT[MAuto3Z].MotorMove(Prod.TrayZ_Mid[5]);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btnC_Auto4_SelectorClick(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(MOT[MAuto4Z].HomeFlag==false || IsIndexMotorOutOfPower())
    {
        lblAuto4Z->Visible=(MOT[MAuto4Z].HomeFlag==false);
        Ptr->Down=false;
        return;
    }
    lblAuto4Z->Visible=false;
    if(Ptr->Tag==0)
    {
        btnC_Auto4_Up->Down         =!btnC_Auto4_Up->Down;
        btnC_Auto4_Up_U->Down       =!btnC_Auto4_Up_U->Down;
    }
    else
    {
        btnC_Auto4_Selector->Down   =!btnC_Auto4_Selector->Down;
        btnC_Auto4_Selector_U->Down =!btnC_Auto4_Selector_U->Down;
    }

    if((btnC_Auto4_Up->Down         &&
        btnC_Auto4_Selector->Down)  ||
       (btnC_Auto4_Up_U->Down       &&
        btnC_Auto4_Selector_U->Down))
    {
        MOT[MAuto4Z].MotorMove(Prod.TrayZ_Up[6]);
    }
    else if((btnC_Auto4_Up->Down==false         &&
             btnC_Auto4_Selector->Down==false)  ||
            (btnC_Auto4_Up_U->Down==false       &&
             btnC_Auto4_Selector_U->Down==false))
    {
        MOT[MAuto4Z].MotorMove(0);
    }
    else if((btnC_Auto4_Up->Down==true          &&
             btnC_Auto4_Selector->Down==false)  ||
            (btnC_Auto4_Up_U->Down==true        &&
             btnC_Auto4_Selector_U->Down==false))
    {
        MOT[MAuto4Z].MotorMove(Prod.TrayZ_Mid[6]);
    }
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btnC_Auto5_SelectorClick(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(MOT[MAuto5Z].HomeFlag==false || IsIndexMotorOutOfPower())
    {
        lblAuto5Z->Visible=(MOT[MAuto5Z].HomeFlag==false);
        Ptr->Down=false;
        return;
    }
    lblAuto5Z->Visible=false;
    if(Ptr->Tag==0)
    {
        btnC_Auto5_Up->Down         =!btnC_Auto5_Up->Down;
        btnC_Auto5_Up_U->Down       =!btnC_Auto5_Up_U->Down;
    }
    else
    {
        btnC_Auto5_Selector->Down   =!btnC_Auto5_Selector->Down;
        btnC_Auto5_Selector_U->Down =!btnC_Auto5_Selector_U->Down;
    }

    if((btnC_Auto5_Up->Down         &&
        btnC_Auto5_Selector->Down)  ||
       (btnC_Auto5_Up_U->Down       &&
        btnC_Auto5_Selector_U->Down))
    {
        MOT[MAuto5Z].MotorMove(Prod.TrayZ_Up[7]);
    }
    else if((btnC_Auto5_Up->Down==false         &&
             btnC_Auto5_Selector->Down==false)  ||
            (btnC_Auto5_Up_U->Down==false       &&
             btnC_Auto5_Selector_U->Down==false))
    {
        MOT[MAuto5Z].MotorMove(0);
    }
    else if((btnC_Auto5_Up->Down==true          &&
             btnC_Auto5_Selector->Down==false)  ||
            (btnC_Auto5_Up_U->Down==true        &&
             btnC_Auto5_Selector_U->Down==false))
    {
        MOT[MAuto5Z].MotorMove(Prod.TrayZ_Mid[7]);
    }
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btnC_Auto6_SelectorClick(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(MOT[MAuto6Z].HomeFlag==false || IsIndexMotorOutOfPower())
    {
        lblAuto6Z->Visible=(MOT[MAuto6Z].HomeFlag==false);
        Ptr->Down=false;
        return;
    }
    lblAuto6Z->Visible=false;
    if(Ptr->Tag==0)
    {
        btnC_Auto6_Up->Down         =!btnC_Auto6_Up->Down;
        btnC_Auto6_Up_U->Down       =!btnC_Auto6_Up_U->Down;
    }
    else
    {
        btnC_Auto6_Selector->Down   =!btnC_Auto6_Selector->Down;
        btnC_Auto6_Selector_U->Down =!btnC_Auto6_Selector_U->Down;
    }

    if((btnC_Auto6_Up->Down         &&
        btnC_Auto6_Selector->Down)  ||
       (btnC_Auto6_Up_U->Down       &&
        btnC_Auto6_Selector_U->Down))
    {
        MOT[MAuto6Z].MotorMove(Prod.TrayZ_Up[8]);
    }
    else if((btnC_Auto6_Up->Down==false         &&
             btnC_Auto6_Selector->Down==false)  ||
            (btnC_Auto6_Up_U->Down==false       &&
             btnC_Auto6_Selector_U->Down==false))
    {
        MOT[MAuto6Z].MotorMove(0);
    }
    else if((btnC_Auto6_Up->Down==true          &&
             btnC_Auto6_Selector->Down==false)  ||
            (btnC_Auto6_Up_U->Down==true        &&
             btnC_Auto6_Selector_U->Down==false))
    {
        MOT[MAuto6Z].MotorMove(Prod.TrayZ_Mid[8]);
    }
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::edtSearchIOChange(TObject *Sender)                 //Steven 20210629 : 搜尋IO
{
    if(edtSearchIO->Text!="" && edtSearchIO->Text.Length()>=2)
    {
        int iSize=HSys.IOTable.size();
        int iRowCount=0;
        bool bAdd=false;
        AnsiString S;
        TStringList *Strs=new TStringList();

        strngrdIoTable->ColCount=16;
        strngrdIoTable->ColWidths[ 0]=50;
        strngrdIoTable->ColWidths[ 1]=100;
        strngrdIoTable->ColWidths[ 2]=200;
        strngrdIoTable->ColWidths[ 3]=50;
        strngrdIoTable->ColWidths[ 4]=50;
        strngrdIoTable->ColWidths[ 5]=50;
        strngrdIoTable->ColWidths[ 6]=50;
        strngrdIoTable->ColWidths[ 7]=50;
        strngrdIoTable->ColWidths[ 8]=50;
        strngrdIoTable->ColWidths[ 9]=50;
        strngrdIoTable->ColWidths[10]=50;
        strngrdIoTable->ColWidths[11]=50;
        strngrdIoTable->ColWidths[12]=50;
        strngrdIoTable->ColWidths[13]=50;
        strngrdIoTable->ColWidths[14]=50;
        strngrdIoTable->ColWidths[15]=300;

        strngrdIoTable->Cells[ 0][0]="No";
        strngrdIoTable->Cells[ 1][0]="Type";
        strngrdIoTable->Cells[ 2][0]="Alias";
        strngrdIoTable->Cells[ 3][0]="Lane";
        strngrdIoTable->Cells[ 4][0]="ModuleType";
        strngrdIoTable->Cells[ 5][0]="IP";
        strngrdIoTable->Cells[ 6][0]="Port";
        strngrdIoTable->Cells[ 7][0]="Bit";
        strngrdIoTable->Cells[ 8][0]="InType";
        strngrdIoTable->Cells[ 9][0]="ISA Base";
        strngrdIoTable->Cells[10][0]="Enable";
        strngrdIoTable->Cells[11][0]="OnAlarmTime";
        strngrdIoTable->Cells[12][0]="OffAlarmTime";
        strngrdIoTable->Cells[13][0]="OnDelayTime";
        strngrdIoTable->Cells[14][0]="OffDelayTime";
        strngrdIoTable->Cells[15][0]="Note";

        for(int i=1; i<strngrdIoTable->RowCount; i++)
        {
            for(int j=0; j<16; j++)
                strngrdIoTable->Cells[j][i]="";
        }

        strngrdIoTable->RowCount=1;
        iRowCount=0;

        for(int i=0; i<iSize; i++)
        {
            if(i==0)
            {
                iRowCount++;
            }
            else
            {
                Strs->CommaText=HSys.IOTable[i]->_CommaText;
                if(Strs->Count>2)
                    S=Strs->Strings[1].UpperCase();
                else
                    S="";

                if(S!="" && S.AnsiPos(edtSearchIO->Text.UpperCase())>=1)
                {
                    bAdd=true;
                }
            }

            if(bAdd)
            {
                strngrdIoTable->RowCount=iRowCount+1;
                strngrdIoTable->Cells[ 0][iRowCount]=HSys.IOTable[i]->Tag;
                strngrdIoTable->Cells[ 1][iRowCount]=HSys.IOTable[i]->Type;
                strngrdIoTable->Cells[ 2][iRowCount]=HSys.IOTable[i]->Alias;
                strngrdIoTable->Cells[ 3][iRowCount]=(HSys.IOTable[i]->iLane        ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iLane);
                strngrdIoTable->Cells[ 4][iRowCount]=(HSys.IOTable[i]->iModuleType  ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iModuleType);
                strngrdIoTable->Cells[ 5][iRowCount]=(HSys.IOTable[i]->iIP          ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iIP);
                strngrdIoTable->Cells[ 6][iRowCount]=(HSys.IOTable[i]->iPort        ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iPort);
                strngrdIoTable->Cells[ 7][iRowCount]=(HSys.IOTable[i]->iBit         ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iBit);
                strngrdIoTable->Cells[ 8][iRowCount]=(HSys.IOTable[i]->iInType      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iInType);
                strngrdIoTable->Cells[ 9][iRowCount]=(HSys.IOTable[i]->iISABase     ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iISABase);
                strngrdIoTable->Cells[10][iRowCount]=(HSys.IOTable[i]->iEnable      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iEnable);
                strngrdIoTable->Cells[11][iRowCount]=(HSys.IOTable[i]->iOnAlarmTime ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOnAlarmTime);
                strngrdIoTable->Cells[12][iRowCount]=(HSys.IOTable[i]->iOffAlarmTime==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOffAlarmTime);
                strngrdIoTable->Cells[13][iRowCount]=(HSys.IOTable[i]->iOnDelayTime ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOnDelayTime);
                strngrdIoTable->Cells[14][iRowCount]=(HSys.IOTable[i]->iOffDelayTime==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOffDelayTime);
                iRowCount++;
                bAdd=false;
            }
        }

        if(strngrdIoTable->RowCount<=1)
            strngrdIoTable->RowCount=2;
        strngrdIoTable->FixedRows=1;
        Strs->Clear();
        delete Strs;
    }
    else if(edtSearchIO->Text=="")
    {
        LoadIoTable(cbbType->ItemIndex, cbbLane->ItemIndex, 0);
    }
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::sb_IO_CommunicationPadClick(TObject *Sender)
{
    fPadInterface->ShowModal();                                                 //Ken 20210702 AddPadInterface
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::edtPulseDelayMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TEdit *Ptr;
    Ptr=(TEdit *)Sender;

    if(Ptr->Name=="edtPulseDelay")
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100, 1);
    }
    else if(Ptr->Name=="edtSendCount")
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100, 1);
    }
    else if(Ptr->Name=="edtTemp")
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_NO_SYMBOL);
    }
}
//------------------------------------------------------------------------------
void Tfiosetview::Hide_1032_IO()                                                //Ztex 2023.04.13 Add HT-1032 IO
{
    tsStack1_Above->TabVisible  =false;
    tsStack2_Above->TabVisible  =false;
    tsStack3_Above->TabVisible  =false;
    tsStack1_Under->TabVisible  =false;
    tsStack2_Under->TabVisible  =false;
    tsStack3_Under->TabVisible  =false;
    tsStack1_Cassette->TabVisible=false;                                        //RogerYang 20260317 : Add
    tsStack2_Cassette->TabVisible=false;
    if(USE_LdUldCassetteMode==1)                                                //Ifor 20251216 add:Boat Carrier
    {
        pgcStack1->ActivePage       =tsStack1_Cassette;
        pgcStack1->ActivePageIndex  =2;
        pgcStack2->ActivePage       =tsStack2_Cassette;
        pgcStack2->ActivePageIndex  =2;
    }
    else
    {
        pgcStack1->ActivePage       =(TRAY_ARM_MODE==eAboveCoveyor)?tsStack1_Above:tsStack1_Under;
        pgcStack1->ActivePageIndex  =(TRAY_ARM_MODE==eAboveCoveyor)?0:1;
        pgcStack2->ActivePage       =(TRAY_ARM_MODE==eAboveCoveyor)?tsStack2_Above:tsStack2_Under;
        pgcStack2->ActivePageIndex  =(TRAY_ARM_MODE==eAboveCoveyor)?0:1;
    }

    pgcStack3->ActivePage       =(TRAY_ARM_MODE==eAboveCoveyor)?tsStack3_Above:tsStack3_Under;
    pgcStack3->ActivePageIndex  =(TRAY_ARM_MODE==eAboveCoveyor)?0:1;
    grpTempOver->Visible=(Tri_Temp_Machine==1);

    pnlSnSafeDoor1Hatchway      ->Visible=(Tri_Temp_Machine==1);
    pnlSnSafeDoor2Hatchway      ->Visible=(Tri_Temp_Machine==1);
    pnlSnSafeDoor3Hatchway      ->Visible=(Tri_Temp_Machine==1);
    pnlSnSafeDoor4Hatchway      ->Visible=(Tri_Temp_Machine==1);
    pnlSnSafeDoor6Hatchway      ->Visible=(Tri_Temp_Machine==1);
    pnlSnSafeDoor7Hatchway      ->Visible=(Tri_Temp_Machine==1);
    pnlSnSafeDoor8Hatchway      ->Visible=(Tri_Temp_Machine==1);
    pnlSnSafeDoor10Hatchway     ->Visible=(Tri_Temp_Machine==1);
    pnlSnSafeDoor11Hatchway     ->Visible=(Tri_Temp_Machine==1);
    pnlSnSafeDoorForFixTray     ->Visible=(Tri_Temp_Machine==1);

    btnC_SafeDoor8Lock_1032     ->Visible=(MachineTypeChoice==Type_HT1032);
    grpTriTempAir               ->Visible=(MachineTypeChoice==Type_HT1032);
    gbExhaustAir                ->Visible=(MachineTypeChoice==Type_HT1032);
    btnC_Shuttle1Floodgate      ->Visible=(SHUTTLE_FLOODGATE==1);
    ledC_Shuttle1Floodgate_On   ->Visible=(SHUTTLE_FLOODGATE==1);
    ledC_Shuttle1Floodgate_Off  ->Visible=(SHUTTLE_FLOODGATE==1);
    btnC_Shuttle2Floodgate      ->Visible=(SHUTTLE_FLOODGATE==1);
    ledC_Shuttle2Floodgate_On   ->Visible=(SHUTTLE_FLOODGATE==1);
    ledC_Shuttle2Floodgate_Off  ->Visible=(SHUTTLE_FLOODGATE==1);

    btnC_OutShuttle1Floodgate       ->Visible=(SHUTTLE_FLOODGATE==1);           //Ifor 20240620 add:Out Shuttle Floodgate
    ledC_OutShuttle1Floodgate_On    ->Visible=(SHUTTLE_FLOODGATE==1);           //Ifor 20240620 add:Out Shuttle Floodgate
    ledC_OutShuttle1Floodgate_Off   ->Visible=(SHUTTLE_FLOODGATE==1);           //Ifor 20240620 add:Out Shuttle Floodgate
    btnC_OutShuttle2Floodgate       ->Visible=(SHUTTLE_FLOODGATE==1);           //Ifor 20240620 add:Out Shuttle Floodgate
    ledC_OutShuttle2Floodgate_On    ->Visible=(SHUTTLE_FLOODGATE==1);           //Ifor 20240620 add:Out Shuttle Floodgate
    ledC_OutShuttle2Floodgate_Off   ->Visible=(SHUTTLE_FLOODGATE==1);           //Ifor 20240620 add:Out Shuttle Floodgate
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::tbarIndexEP2Change(TObject *Sender)
{
    ADAM_DirectWriteData(tbarIndexEP2->Position, 0, 11);
}
//------------------------------------------------------------------------------
void Tfiosetview::RefreshSafePLCLed()                                           //ben 20230823 for SafePLC
{
    //========== Safe page ==========
    if(g_bEnSubPage[ePLCGrp_Safe])
    {
        aLedSPLC000->Value = bPLCInData[ePLCGrp_Safe][1][0];
        aLedSPLC001->Value = bPLCInData[ePLCGrp_Safe][1][1];
        aLedSPLC002->Value = bPLCInData[ePLCGrp_Safe][1][2];
        aLedSPLC003->Value = bPLCInData[ePLCGrp_Safe][1][3];
        aLedSPLC004->Value = bPLCInData[ePLCGrp_Safe][1][4];
        aLedSPLC005->Value = bPLCInData[ePLCGrp_Safe][1][5];
        aLedSPLC006->Value = bPLCInData[ePLCGrp_Safe][1][6];
        aLedSPLC007->Value = bPLCInData[ePLCGrp_Safe][1][7];

        aLedSPLC010->Value = bPLCInData[ePLCGrp_Safe][2][0];
        aLedSPLC011->Value = bPLCInData[ePLCGrp_Safe][2][1];
        aLedSPLC012->Value = bPLCInData[ePLCGrp_Safe][2][2];
        aLedSPLC013->Value = bPLCInData[ePLCGrp_Safe][2][3];
        aLedSPLC014->Value = bPLCInData[ePLCGrp_Safe][2][4];
        aLedSPLC015->Value = bPLCInData[ePLCGrp_Safe][2][5];
        aLedSPLC016->Value = bPLCInData[ePLCGrp_Safe][2][6];
        aLedSPLC017->Value = bPLCInData[ePLCGrp_Safe][2][7];

        aLedSPLC020->Value = bPLCInData[ePLCGrp_Safe][3][0];
        aLedSPLC021->Value = bPLCInData[ePLCGrp_Safe][3][1];
        aLedSPLC022->Value = bPLCInData[ePLCGrp_Safe][3][2];
        aLedSPLC023->Value = bPLCInData[ePLCGrp_Safe][3][3];
        aLedSPLC024->Value = bPLCInData[ePLCGrp_Safe][3][4];
        aLedSPLC025->Value = bPLCInData[ePLCGrp_Safe][3][5];
        aLedSPLC026->Value = bPLCInData[ePLCGrp_Safe][3][6];
        aLedSPLC027->Value = bPLCInData[ePLCGrp_Safe][3][7];

        aLedSPLC030->Value = bPLCInData[ePLCGrp_Safe][4][0];
        aLedSPLC031->Value = bPLCInData[ePLCGrp_Safe][4][1];
        aLedSPLC032->Value = bPLCInData[ePLCGrp_Safe][4][2];
        aLedSPLC033->Value = bPLCInData[ePLCGrp_Safe][4][3];
        aLedSPLC034->Value = bPLCInData[ePLCGrp_Safe][4][4];
        aLedSPLC035->Value = bPLCInData[ePLCGrp_Safe][4][5];
        aLedSPLC036->Value = bPLCInData[ePLCGrp_Safe][4][6];
        aLedSPLC037->Value = bPLCInData[ePLCGrp_Safe][4][7];

        aLedSPLC040->Value = bPLCInData[ePLCGrp_Safe][5][0];
        aLedSPLC041->Value = bPLCInData[ePLCGrp_Safe][5][1];
        aLedSPLC042->Value = bPLCInData[ePLCGrp_Safe][5][2];
        aLedSPLC043->Value = bPLCInData[ePLCGrp_Safe][5][3];
        aLedSPLC044->Value = bPLCInData[ePLCGrp_Safe][5][4];
        aLedSPLC045->Value = bPLCInData[ePLCGrp_Safe][5][5];
        aLedSPLC046->Value = bPLCInData[ePLCGrp_Safe][5][6];
        aLedSPLC047->Value = bPLCInData[ePLCGrp_Safe][5][7];

        aLedSPLC050->Value = bPLCInData[ePLCGrp_Safe][6][0];
        aLedSPLC051->Value = bPLCInData[ePLCGrp_Safe][6][1];
        aLedSPLC052->Value = bPLCInData[ePLCGrp_Safe][6][2];
        aLedSPLC053->Value = bPLCInData[ePLCGrp_Safe][6][3];
        aLedSPLC054->Value = bPLCInData[ePLCGrp_Safe][6][4];
        aLedSPLC055->Value = bPLCInData[ePLCGrp_Safe][6][5];
        aLedSPLC056->Value = bPLCInData[ePLCGrp_Safe][6][6];
        aLedSPLC057->Value = bPLCInData[ePLCGrp_Safe][6][7];

        aLedSPLC060->Value = bPLCInData[ePLCGrp_Safe][7][0];
        aLedSPLC061->Value = bPLCInData[ePLCGrp_Safe][7][1];
        aLedSPLC062->Value = bPLCInData[ePLCGrp_Safe][7][2];
        aLedSPLC063->Value = bPLCInData[ePLCGrp_Safe][7][3];
        aLedSPLC064->Value = bPLCInData[ePLCGrp_Safe][7][4];
        aLedSPLC065->Value = bPLCInData[ePLCGrp_Safe][7][5];
        aLedSPLC066->Value = bPLCInData[ePLCGrp_Safe][7][6];
        aLedSPLC067->Value = bPLCInData[ePLCGrp_Safe][7][7];

        aLedSPLC070->Value = bPLCInData[ePLCGrp_Safe][8][0];
        aLedSPLC071->Value = bPLCInData[ePLCGrp_Safe][8][1];
        aLedSPLC072->Value = bPLCInData[ePLCGrp_Safe][8][2];
        aLedSPLC073->Value = bPLCInData[ePLCGrp_Safe][8][3];
        aLedSPLC074->Value = bPLCInData[ePLCGrp_Safe][8][4];
        aLedSPLC075->Value = bPLCInData[ePLCGrp_Safe][8][5];
        aLedSPLC076->Value = bPLCInData[ePLCGrp_Safe][8][6];
        aLedSPLC077->Value = bPLCInData[ePLCGrp_Safe][8][7];
    }
}
//------------------------------------------------------------------------------
void Tfiosetview::InitPairInfo_SafePLCIOLed()
{
    m_vecSafePLCIOLedShowPair.clear();
    
    //SnFrontLeftEMG
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(ledSnFrontLeftEMG, &bPLCInData[ePLCGrp_Safe][2][0]));   //aLedSPLC010
    //SnRearLeftEMG
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(ledSnRearLeftEMG, &bPLCInData[ePLCGrp_Safe][2][2]));   //aLedSPLC012
    //SnRearRightEMG
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(ledSnRearRightEMG, &bPLCInData[ePLCGrp_Safe][2][4]));   //aLedSPLC014
    //SnFrontRightEMG
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(ledSnFrontRightEMG, &bPLCInData[ePLCGrp_Safe][2][6]));   //aLedSPLC016
    //SnSafeDoor1
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(ledSnSafeDoor1, &bPLCInData[ePLCGrp_Safe][3][0]));   //aLedSPLC020
    //SnSafeDoor2
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(ledSnSafeDoor2, &bPLCInData[ePLCGrp_Safe][3][2]));   //aLedSPLC022
    //SnSafeDoor3
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(ledSnSafeDoor3, &bPLCInData[ePLCGrp_Safe][3][4]));   //aLedSPLC024
    //SnSafeDoor6
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(ledSnSafeDoor6, &bPLCInData[ePLCGrp_Safe][3][6]));   //aLedSPLC026
    //SnSafeDoor7
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(ledSnSafeDoor7, &bPLCInData[ePLCGrp_Safe][4][0]));   //aLedSPLC030
    //SnSafeDoor8
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(ledSnSafeDoor8, &bPLCInData[ePLCGrp_Safe][4][2]));   //aLedSPLC032
    /*there is no control button in iosetview=>SYSTEM tab for following sensors
    //SnMRDoorL
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][4][4]));   //aLedSPLC034
    //SnMRDoorR
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][4][6]));   //aLedSPLC036
    //SnHotPlate1
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][5][0]));   //aLedSPLC040
    //SnHotPlate2
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][5][1]));   //aLedSPLC041
    //SnShuttle1
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][5][2]));   //aLedSPLC042
    //SnShuttle2
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][5][3]));   //aLedSPLC043
    //SnDut1
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][5][4]));   //aLedSPLC044
    //SnDut2
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][5][5]));   //aLedSPLC045
    //SnDut3
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][5][6]));   //aLedSPLC046
    //SnDut4
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][5][7]));   //aLedSPLC047
    //SnHeaterGun1
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][6][0]));   //aLedSPLC050
    //SnHeaterGun2
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][6][1]));   //aLedSPLC051
    //SnHotAir1
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][6][2]));   //aLedSPLC052
    //SnHotAir2
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][6][3]));   //aLedSPLC053
    //Safe Mode
    m_vecSafePLCIOLedShowPair.push_back(std::make_pair(MyLedLane### &bPLCInData[ePLCGrp_Safe][6][7]));   //aLedSPLC057
    */
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btn500KPAClick(TObject *Sender)
{
    ADAM_DirectWriteData((EP_MAXKPA<=500)?4095:2275, 0);
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btnAllLockClick(TObject *Sender)
{
    bool bOn=true;
    if(SW[SwFixedSeatTLOn].Status()!=bOn)
        btnSwFixedSeatTLOn->OnClick(btnSwFixedSeatTLOn);
    if(SW[SwFixedSeatTLOff].Status()!=!bOn)
        btnSwFixedSeatTLOff->OnClick(btnSwFixedSeatTLOff);
    if(SW[SwFixedSeatTROn].Status()!=bOn)
        btnSwFixedSeatTROn->OnClick(btnSwFixedSeatTROn);
    if(SW[SwFixedSeatTROff].Status()!=!bOn)
        btnSwFixedSeatTROff->OnClick(btnSwFixedSeatTROff);
    if(SW[SwFixedSeatBLOn].Status()!=bOn)
        btnSwFixedSeatBLOn->OnClick(btnSwFixedSeatBLOn);
    if(SW[SwFixedSeatBLOff].Status()!=!bOn)
        btnSwFixedSeatBLOff->OnClick(btnSwFixedSeatBLOff);
    if(SW[SwFixedSeatBROn].Status()!=bOn)
        btnSwFixedSeatBROn->OnClick(btnSwFixedSeatBROn);
    if(SW[SwFixedSeatBROff].Status()!=!bOn)
        btnSwFixedSeatBROff->OnClick(btnSwFixedSeatBROff);
}
//---------------------------------------------------------------------------

void __fastcall Tfiosetview::btnAllUnLockClick(TObject *Sender)
{
    bool bOn=false;
    if(SW[SwFixedSeatTLOn].Status()!=bOn)
        btnSwFixedSeatTLOn->OnClick(btnSwFixedSeatTLOn);
    if(SW[SwFixedSeatTLOff].Status()!=!bOn)
        btnSwFixedSeatTLOff->OnClick(btnSwFixedSeatTLOff);
    if(SW[SwFixedSeatTROn].Status()!=bOn)
        btnSwFixedSeatTROn->OnClick(btnSwFixedSeatTROn);
    if(SW[SwFixedSeatTROff].Status()!=!bOn)
        btnSwFixedSeatTROff->OnClick(btnSwFixedSeatTROff);
    if(SW[SwFixedSeatBLOn].Status()!=bOn)
        btnSwFixedSeatBLOn->OnClick(btnSwFixedSeatBLOn);
    if(SW[SwFixedSeatBLOff].Status()!=!bOn)
        btnSwFixedSeatBLOff->OnClick(btnSwFixedSeatBLOff);
    if(SW[SwFixedSeatBROn].Status()!=bOn)
        btnSwFixedSeatBROn->OnClick(btnSwFixedSeatBROn);
    if(SW[SwFixedSeatBROff].Status()!=!bOn)
        btnSwFixedSeatBROff->OnClick(btnSwFixedSeatBROff);
}
//---------------------------------------------------------------------------

void __fastcall Tfiosetview::TrackBarLightChange(TObject *Sender)
{
    int iPos=0;
    iPos=TrackBarLight->Position;
    SetLightZI0(iPos);
}
//---------------------------------------------------------------------------
void Tfiosetview::SetLightZI0(int iPos)
{
    bool bLightPRG[7];
    double fPos=0;
    int SwPRG[7]={SwPRGSEL0, SwPRGSEL1, SwPRGSEL2, SwPRGSEL3, SwPRGSEL4, SwPRGSEL5, SwPRGSEL6};
    if(fShow==false)
        return;

    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)
    {
        for(int i=0;i<7;i++)
        {
            bLightPRG[i]=(((iPos>>i) & 1)? true:false);
            fPos=(iPos)*0.4-0.4; //每一等份0.4mm，總行程50mm
            lbLightZHigh->Caption=fPos;
            if(bLightPRG[i]==true)
                SW[SwPRG[i]].On();
            else
                SW[SwPRG[i]].Off();
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::tmrUpdateSafePLCIOLedTimer(TObject *Sender)
{
    //we need to show led status in SYTEM page when install safe PLCIO
    //Because program will show led status automatically already when user check not install safe PLCIO
    
    if(!IsSafePLCIOType_ReeR())
        return;
    
    if(NULL == PC_IOSET || NULL == tsSystem)
        return;
    
    if(!tsSystem->TabVisible || PC_IOSET->ActivePage != tsSystem)
        return;
        
    TMyLedLane *pMyLedLane = NULL;
    bool *pPLCInData = NULL;
    
    for (size_t i = 0; i<m_vecSafePLCIOLedShowPair.size(); ++i)
    {
        pMyLedLane = m_vecSafePLCIOLedShowPair[i].first;
        pPLCInData = m_vecSafePLCIOLedShowPair[i].second;
        
        if(NULL == pMyLedLane || NULL == pPLCInData)
            continue;
        
        pMyLedLane->Value = *pPLCInData;//CHECK_NG//不會亮
    }
}
//---------------------------------------------------------------------------

