#include "MachineDefine.h"
#pragma hdrstop

#include "ckernel.h"

#include "MachineType.h"
#include "cprod.h"
#include "mysensor.h"
#include "main.h"

#include "MyLaneIo.h"
#include "note.h"
#include "mymotor.h"
#include "cinitial.h"

#include "MyKitSuck.h"
#include "myswitch.h"
#include "csystem.h"

#include "mymessbox.h"
#include "uhome.h"
#include "csetup.h"
#include "cTowerLight.h"
#include "cContact.h"

#include "ainarm2.h"
#include "uLotInfo.h"
#include "CCDInterface.h"
#include "FTPClient.h"
#include "cMyDB.h"
#include "rs232.h"
#include "cmydef.h"
#include "cpublic.h"
#include "mycylin.h"
#include "ATCInterface.h"
#include "ATC_Handler_Side.h"
#include "ShuttleMove.h"
#include "fRotate.h"
#include "cShowBinSet.h"
#include "common.h"
#include "InOutArmZteach.h"
#include "AutoTeach.h"
#include "ProductionInfo.h"
//Austin 20190531.01 增加PLC_IO模組及安全相關IO.
//==>
#include "MyPLC_IO_Modbus.h"
#include "ModbusTCPClient.h"
//<==
//Austin 20190531.01 增加PLC_IO模組及安全相關IO.
//---------------------------------------------------------------------------
#pragma package(smart_init)
bool bLockByServer=false;                                                       // 2009.11.13 , Joye

bool WaitManualStepKey()
{
    if(fContact->fShow && fContact->cbOneTouchAutoContactHight->Checked==true)  //Ifor 20220803 add 一鍵完成Auto Contact Hight
        return true;

    bLampManualSetp=FlushFlag;

    if(bLampManualSetp)
        fContact->btnTStep->Caption="T.Step";
    else
        fContact->btnTStep->Caption="";

    if(CosFunction.bEnableSoftWareControlButton)                                //ChungHung 20150609 add only for TSMC
    {
        if(bLampManualSetp)
            fMain->BtnSTEP->Color=clYellow;
        else
            fMain->BtnSTEP->Color=(TColor)0x00804000;
    }

    if(fContact->bSetupStep || Sen[SnRKManualStep].IsOn() || bButtonManualStep) //ChungHung 20150526 add for QualComm US
    {
        bButtonManualStep=false;                                                //ChungHung 20150526 add for QualComm US
        fContact->bSetupStep=false;

        if(bSTEP)
        {
            RespondASECom("@e02115Done");                                       //kevin 20160304 回應 ase TrayFeed finish
            bSTEP=false;                                                        //kevin 20160304
        }

        if(IsSafeLockCheck())
            return false;
        bLampManualSetp=false;
        bLampManualStart=false;
        fContact->bSetupStep=false;

        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool WaitManualStartKey()
{
    if(fContact->fShow && fContact->cbOneTouchAutoContactHight->Checked==true)  //Ifor 20220803 add 一鍵完成Auto Contact Hight
        return false;

    bLampManualStart=FlushFlag;

    if(bLampManualStart)
        fContact->btnTStart->Caption="T.Start";
    else
        fContact->btnTStart->Caption="";

    if(CosFunction.bEnableSoftWareControlButton)                                //ChungHung 20150609 add only for TSMC
    {
        if(bLampManualStart)
            fMain->BtnT_Start->Color=clYellow;
        else
            fMain->BtnT_Start->Color=(TColor)0x00804000;
    }

    if(fContact->bSetupStart ||
       Sen[SnRKManualTStart].IsOn() ||
       bButtonManualTStart)                                                     //ChungHung 20150526 add for QualComm US
    {
        bButtonManualTStart=false;                                              //ChungHung 20150526 add for QualComm US
        if(bTSTART)
        {
            RespondASECom("@e02116Done");                                       //kevin 20160304 回應 ase TrayFeed finish
            bTSTART=false;                                                      //kevin 20160304
        }
        bLampManualStart=false;
        fContact->bSetupStart=false;
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool WaitManualRetryKey()                                                       //JerryYang 20160728
{
    bLampRetry=FlushFlag;

    if(bLampRetry)
        fShuttleMove->btRetry->Caption="Retry";
    else
        fShuttleMove->btRetry->Caption="";

    if(fShuttleMove->bShuttleRetry || Sen[SnFKRetry].IsOn() || Sen[SnRKRetry].IsOn())
    {
        bLampRetry=false;
        fShuttleMove->bShuttleRetry=false;
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool CheckThermo()
{
    #ifdef SOFT_SIMULTE
        return true;
    #else
        int i;
        if(LastSet.iTemperature==Tempture_Hot)
            return true;
        if(iHeaterCooling!=0)
            return false;
        for(i=0; i<tcTotalCount; i++)
        {
            if(i==0 || i==1)  //Hotplate 1 & 2
                continue;
            //Steven 20090926 Start: Index Heating Mode
            if(Temperature.iIndexHeatMode==HeadOnly)       //Head Only (socket & chamber no use)
            {
                if(i==8 || i==9) continue;
            }
            else if(Temperature.iIndexHeatMode==ChamberOnly)  //Chamber Only (head 1 2 3 4 & socket no use)
            {
                if(i>=4 && i<=8) continue;
            }
            else if(Temperature.iIndexHeatMode==HeadChamber)  //Head + Chamber Only (socket no use)
            {
                if(i==8) continue;
            }
            else if(Temperature.iIndexHeatMode==SocketChamber)  //Socket + Chamber Only (Head 1234 no use)
            {
                if(i>=4 && i<=7) continue;
            }
            //Steven 20090926 End
        }
        return true;
    #endif
}
extern int AccelateTask;
//------------------------------------------------------------------------------
void StopAllDestroy()                                                           //Eliot 2009_12_24
{
    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxRow; j++)
        {
            InArmSuck.Suck[i][j].OffDestroy();
            OutArmSuck.Suck[i][j].OffDestroy();
        }
    }

    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)
        {
            FTestSuck.Suck[i][j].OffDestroy();
            BTestSuck.Suck[i][j].OffDestroy();
        }
    }
    CatchTraySuck.Suck[0][0].OffDestroy();
}
//------------------------------------------------------------------------------
bool CheckBinSet()
{
    int iTrayPassFail[eTrayCount];
    bool bIsPassT6[eTrayCount];

    for(int i=0; i<eTrayCount; i++)
    {
        iTrayPassFail[i]=-1;
        bIsPassT6[i]=false;
    }

    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
    {
        int j=Prod.iT6CatData[i];

        if(j>=9)
            continue;

        if(iTrayPassFail[j]<0)
        {
            iTrayPassFail[j]=Prod.bIsPassBin[i]?1:0;
            bIsPassT6[j]=Prod.bIsPassBin[i];
        }
        else
        {
            if(bIsPassT6[j]!=Prod.bIsPassBin[i])
            {
                ShowMyMessage("Category define has pass and fail at same field", "Pass Bin和Fail Bin不可放在同一個位置", "CheckBinSet");
                return false;
            }
        }
    }

    if(iTrayPassFail[Prod.iIfErrorT6]==0)
    {
        ShowMyMessage("Can not place to a pass tray of the category of [interface error]", "[interface error]不可以跟Pass Bin擺在同一個位置", "CheckBinSet");
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
//jou 2010-09-03 加熱暫時先關閉使用1x2-4,1x4-8,2x2-8
void ChangeUseSuckMode()
{
    if(LastSet.iTemperature==Tempture_Hot)
    {
        switch(TestIF.iTestMode)
        {
            case DualSite: //1x2
                //ChungHung 2011/02/10 start
                if(i8PickerHPMode==iHPWideHP && IniConfig.b1x2Use4Suck)    //JerryYang 20161007 簡化Hotplate判斷式 //Steven 20151117 : 2x2 8Picker at Hot mode
                {
                    if(HotPlateForm.XDivision==4 || HotPlateForm.XDivision==6 || HotPlateForm.XDivision==8 || HotPlateForm.XDivision==12)
                    {
                        break;
                    }
                    else
                    {
                        TestIF_File.iUseSuckMode=2;     //Steven 20161102 : 4 --> 2
                        TestIF.iUseSuckMode=2;
                    }
                }
                else
                {
                    TestIF_File.iUseSuckMode=2;
                    TestIF.iUseSuckMode=2;
                }
                //ChungHung 2011/02/10 end
                break;
            case QualSite2X2N:          //Frank 20200520 2X2NN Mode
//            case _6Site2X3N:            //Steven 20220425 : 2X3NN Mode
                TestIF_File.iUseSuckMode=2;
                TestIF.iUseSuckMode=2;
                break;
            case QualSite1X4: //1x4
            case _8Site1X4:   //ChungHung 20150528 add for 海思 _8Site1x4
            case _8Site2X4N:  //Wei 20231211 : 2X4NN Mode
                //ChungHung 2011/01/18 start
                if(i8PickerHPMode==iHPWideHP && IniConfig.b1x4Use8Suck)    //JerryYang 20161007 簡化Hotplate判斷式   //Steven 20151117 : 2x2 8Picker at Hot mode
                {
                    if(HotPlateForm.XDivision==4 || HotPlateForm.XDivision==6 || HotPlateForm.XDivision==8 || HotPlateForm.XDivision==12)
                    {
                        break;
                    }
                    else
                    {
                        TestIF_File.iUseSuckMode=4;
                        TestIF.iUseSuckMode=4;
                    }
                }
                else
                {
                    TestIF_File.iUseSuckMode=4;
                    TestIF.iUseSuckMode=4;
                }
                //ChungHung 2011/01/18 end
                break;
            case QualSite2X2: //2x2
                if(i8PickerHPMode==iHPWideHP && IniConfig.b2x2Use8Suck)    //JerryYang 20161007 簡化Hotplate判斷式  //加熱模式八進四出 //Steven 20151117 : 2x2 8Picker at Hot mode
                {
                    if(HotPlateForm.XDivision==6 ||
                       HotPlateForm.XDivision==8 ||
                       HotPlateForm.XDivision==12)  //Steven 20180409 (Jou) : Add 2x2 support X=12
                    {
                        break;
                    }
                    else
                    {
                        TestIF_File.iUseSuckMode=4;
                        TestIF.iUseSuckMode=4;
                    }
                }
                else
                {
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||                       //kevin 20161006
                       CUSTOMER_CODE==CC_SIGURD_PeiXing)                        //Sam 20250415 : add SIGURD_PeiXing
                    {
                        if(HotPlateForm.XDivision==6 || HotPlateForm.XDivision==8)
                        {
                            break;
                        }
                        else
                        {
                            TestIF_File.iUseSuckMode=4;
                            TestIF.iUseSuckMode=4;
                        }
                    }
                    else
                    {
                        TestIF_File.iUseSuckMode=4;
                        TestIF.iUseSuckMode=4;
                    }
                }
                break;
            case _8Site2X4:
            case _16Site4X4:
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                  //Steven for HT1032
                   USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                     //Ztex 2023.12.06 Add HT-1032
                {
                }
                else
                {

                }
                break;
        }
    }
}
//------------------------------------------------------------------------------
bool ScanSystemSensor()
{
    AnsiString Str2;
    bool CloseSuckok=true;                                                      //kevin 20110505
    bool bConsFail=false;

    if(SoftStart==true)                                                         //啟動檢查
    {
        fMain->MainFormChange();

        AccelateTask=1;
        ChangeUseSuckMode();
        SetWorkParameter();
        fHome->fAbort=false;
        fHome->iHomeStep=1;
        for(int i=0; i<TOTAL_MOTOR; i++)
            InitProcessSingleMotorTask(i);
        SystemNG=false;
        ClearAllAlarm();
        SoftStart=false;
        SoftStop=false;

        if(CosFunction.bConFailFoolProofing==true &&                            //jou 2014-09-28 continue fail 防呆提醒,總開關開啟為On時,BIN別未勾選任何continue fail alarm
           (Prod.bContsFailBySocket==true || Prod.bContsFailByHead==true))
        {
            for(int i=0; i<iTestBinCount; i++)
            {
                if(Prod.bConsFail[i]==true)
                    bConsFail=true;
            }

            if(bConsFail==false)
            {
#ifndef SOFT_SIMULTE
                ShowMyMessage("Bin Select Form no click any continue fail error!","Bin Selset 頁面沒有選擇任何連續錯誤BIN別!");
                return false;
#endif
            }
        }

        if(bLockByServer)                                                       //Steven 20100820 : For OLP
        {
            ShowMyMessage("Lock by Host, need unlocked from HOST or restart program !!", "被網路控管中心鎖住必須由網路控管解除或重啟程式!!");
            return false;
        }

        if(iHome==0 && CheckThermo()==false)
        {
            ShowMyMessage("Kit tempture too high!!", "治具目前溫度尚未冷卻,不能進行測試!!", "ScanSystemSensor");
            return false;
        }
        if(iHome==0 && fSetup->fShow==false)
        {
            if(fAllMotorHome)
            {
                if(bNeedArmZHome)
                {
                    InitDoArmZHome();
                }
                else if(FrmRotate->bRotateInHome)                               //kevin 20130706 rotat single home
                {
                    FrmRotate->InitialInRotateHome();                           //Steven 20170329 (wei) : Add individual rotate motor
                }
                else if(FrmRotate->bRotateOutHome)                              //kevin 20130706 rotat single home
                {
                    FrmRotate->InitialOutRotateHome();                          //Steven 20170329 (wei) : Add individual rotate motor
                }
                else if(bYpitchNeddHome)                                        //kevin 20180823
                {
                    bYpitchNeddHome=true;
                }
                else
                {
                    if(CheckMotorHome()==false)
                    {
                        ShowMyMessage("Must home again", "需要重新歸零", "ScanSystemSensor");
                        return false;
                    }
                }
            }
        }

        if(ATC_SYSTEM==eATCSiliconType && Temperature.bATCActiveCooling==true &&
           Temperature.bATCHandlerStart==true && (TestIF.iTestMode==DualSite || TestIF.iTestMode==SingleSite))    //jou 2012-03-14 增加ATC控制方式
        {
            if(bATCInitialFinish==false || COM2->ATCAlarmSenCheck()==false)
            {
                ShowMyMessage("ATC Alarm Sensor Off,please check ATC system is OK!","ATC警報偵測,請確認ATC系統是否正常!");
                return false;
            }
        }

        if(IniConfig.bA36OpenDoorSetErrBin)                                     //JerryYang 20210901 : Microchip要求開安全門要分ERROR BIN
        {
            if(bIsOpenDoorNeedSetErrBin)
            {
                bIsOpenDoorNeedSetErrBin=false;
                int iCT=0;
                AnsiString sBuffer="";
                iCT=FRCarryKit.CountRealIC();
                if(iCT>0)
                {
                    sBuffer.printf("Open safe door Out Shuttle1 Place to Error bin : Device=%d;",iCT);
                    MyDBIProcess("Message", sBuffer);
                }
                FRCarryKit.SetAllRealIC2InterfaceBin();
                iCT=BRCarryKit.CountRealIC();
                if(iCT>0)
                {
                    sBuffer.printf("Open safe door Out Shuttle2 Place to Error bin : Device=%d;",iCT);
                    MyDBIProcess("Message", sBuffer);
                }
                BRCarryKit.SetAllRealIC2InterfaceBin();
                iCT=FTestSuck.CountRealIC();
                if(iCT>0)
                {
                    sBuffer.printf("Open safe door Index Arm1 Place to Error bin : Device=%d;",iCT);
                    MyDBIProcess("Message", sBuffer);
                }
                FTestSuck.SetAllRealIC2InterfaceBin();
                iCT=BTestSuck.CountRealIC();
                if(iCT>0)
                {
                    sBuffer.printf("Open safe door Index Arm2 Place to Error bin : Device=%d;",iCT);
                    MyDBIProcess("Message", sBuffer);
                }
                BTestSuck.SetAllRealIC2InterfaceBin();
                TestSocket.SetAllRealIC2InterfaceBin();
            }
        }
        else
        {
            bIsOpenDoorNeedSetErrBin=false;
        }

        SetMotorSpeed();
        SetSuckRetryCount();
        bLampSkip=false;
        bLampRetry=false;
        bLampTrayEnd=false;
//        bLampAlarmReset=false;
        bLampAlarmReset=false;
        SystemStart=true;
        Str2="KS4,4,4,4;VT0.1,0.1,0.1,0.1;";                                    //Steven 20181231 : Galil平滑曲線改成4,4,4,4    // KS is pulse command driver use for  Smoothing  ,Large value of KS provide greater smoothing
        MOT[MTestY1].Gali_Command(Str2, __FUNC__);
        fMain->ChangeLevelAttr();

        DoInArm_SuckerMap();

        #ifndef SOFT_SIMULTE
        if(CheckSafeDoorIsClosed()==false)                                      //jou 20171030 (Steven) : 修正安全門未關按Start時IndexArm會先動作
        {
            SystemStart=false;
            StopAllMotor();
            return false;
        }
        #endif

        //AI(ht9045-v899) 20260831: Start 確定成功後才重設抖動逾時, 避免暫停與處理 alarm 的掛鐘時間被算進 10 秒而一按 Start 就誤報
        ResetShakeShuttleTimeOut();
    }
    else if(SoftStop==true)                                                     //暫停檢查
    {
        StopAllMotor();
        SoftStop=false;
        SystemStart=false;
        bLampAlarmReset=false;
        fMain->ChangeLevelAttr();

        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                LastSet.iInArmVacuumDummyOnTime[i][j]=InArmSuck.Suck[i][j].VacuumOnTime;
                LastSet.iOutArmVacuumDummyOnTime[i][j]=OutArmSuck.Suck[i][j].VacuumOnTime;
                LastSet.iInArmVacuumDummyOffTime[i][j]=InArmSuck.Suck[i][j].VacuumOffTime;
                LastSet.iOutArmVacuumDummyOffTime[i][j]=OutArmSuck.Suck[i][j].VacuumOffTime;

                InArmSuck.Suck[i][j].ReStart();
                OutArmSuck.Suck[i][j].ReStart();
                if(InArmSuck.Item[i][j]==NULL_IC)
                    InArmSuck.Suck[i][j].Normal();
                if(OutArmSuck.Item[i][j]==NULL_IC)
                    OutArmSuck.Suck[i][j].Normal();
            }
        }

        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<NEW_MAX_Index_Col; j++)
            {
                LastSet.iFTestArmVacuumDummyOnTime[i][j]=FTestSuck.Suck[i][j].VacuumOnTime;
                LastSet.iBTestArmVacuumDummyOnTime[i][j]=BTestSuck.Suck[i][j].VacuumOnTime;
                LastSet.iFTestArmVacuumDummyOffTime[i][j]=FTestSuck.Suck[i][j].VacuumOffTime;
                LastSet.iBTestArmVacuumDummyOffTime[i][j]=BTestSuck.Suck[i][j].VacuumOffTime;

                FTestSuck.Suck[i][j].ReStart();
                BTestSuck.Suck[i][j].ReStart();

                if(IniConfig.bChangeKitNoHardStop==true)                        //jou 2015-12-08 Xilinx 驗證用
                {
                    if(INDEX_SUCKER_TYPE==1 && fContact->fShow==false)
                    {
                        if(bArm1NeedSuck==false &&                              //JerryYang 20240816 : 修正當index arm真空開啟，還沒建立真空的時候剛好發生ALARM，重新START會誤發Index arm pick up error
                           FTestSuck.Item[i][j]==NULL_IC && FTestSuck.Suck[i][j].GetOnBit())
                        {
                            FTestSuck.Suck[i][j].Normal();
                            CloseSuckok=false;
                        }

                        if(bArm2NeedSuck==false &&                              //JerryYang 20240816 : 修正當index arm真空開啟，還沒建立真空的時候剛好發生ALARM，重新START會誤發Index arm pick up error
                           BTestSuck.Item[i][j]==NULL_IC && BTestSuck.Suck[i][j].GetOnBit())
                        {
                            BTestSuck.Suck[i][j].Normal();
                            CloseSuckok=false;
                        }
                    }
                }
                else
                {
                    if(INDEX_SUCKER_TYPE==1 && bIndexCheckNoStopVaccum==false)  //Sam 20220902 : 修正 All Site 掉料
                    {
                        if(bArm1NeedSuck==false &&                              //JerryYang 20240816 : 修正當index arm真空開啟，還沒建立真空的時候剛好發生ALARM，重新START會誤發Index arm pick up error
                           FTestSuck.Item[i][j]==NULL_IC && FTestSuck.Suck[i][j].GetOnBit())
                        {
                            FTestSuck.Suck[i][j].Normal();
                            CloseSuckok=false;
                        }

                        if(bArm2NeedSuck==false &&                              //JerryYang 20240816 : 修正當index arm真空開啟，還沒建立真空的時候剛好發生ALARM，重新START會誤發Index arm pick up error
                           BTestSuck.Item[i][j]==NULL_IC && BTestSuck.Suck[i][j].GetOnBit())
                        {
                            BTestSuck.Suck[i][j].Normal();
                            CloseSuckok=false;
                        }
                    }
                }
            }
        }
        LastSet.iCatchArmVacuumDummyOnTime  =CatchTraySuck.Suck[0][0].VacuumOnTime;
        LastSet.iCatchArmVacuumDummyOffTime =CatchTraySuck.Suck[0][0].VacuumOffTime;
    }

    if(ArmSpeed_File[InArm].bAutoSKIP==1 && bASkStart==true)                    //kevin 20170606 (wei) tray end 燈亮
        bLampTrayEnd=true;

    if(SystemStart==false &&
       INDEX_SUCKER_TYPE==1 &&                                                  //jou 2010-05-24 start : 負壓
       Zteach->fShow==false)                                                    //kevin 20210322 add
    {
        CheckIndexAllSuckICFallDown(true, true);                                //Steven 20110725 : 修改負壓檢查方式

        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<NEW_MAX_Index_Col; j++)
            {

                if(IniConfig.bChangeKitNoHardStop==true)                        //jou 2015-12-08 Xilinx 驗證用
                {
                    if(INDEX_SUCKER_TYPE==1 && fContact->fShow==false)
                    {
                        if(bArm1NeedSuck==false &&                              //JerryYang 20240816 : 修正當index arm真空開啟，還沒建立真空的時候剛好發生ALARM，重新START會誤發Index arm pick up error
                           FTestSuck.Item[i][j]==NULL_IC && FTestSuck.Suck[i][j].GetOnBit())
                        {
                            FTestSuck.Suck[i][j].Normal();
                            CloseSuckok=false;                                  //kevin 20110505
                        }

                        if(bArm2NeedSuck==false &&                              //JerryYang 20240816 : 修正當index arm真空開啟，還沒建立真空的時候剛好發生ALARM，重新START會誤發Index arm pick up error
                           BTestSuck.Item[i][j]==NULL_IC && BTestSuck.Suck[i][j].GetOnBit())
                        {
                            BTestSuck.Suck[i][j].Normal();
                            CloseSuckok=false;                                  //kevin 20110505
                        }
                    }
                }
                else
                {
                    if(INDEX_SUCKER_TYPE==1 && bIndexCheckNoStopVaccum==false)  //Sam 20220902 : 修正 All Site 掉料
                    {
                        if(bArm1NeedSuck==false &&                              //JerryYang 20240816 : 修正當index arm真空開啟，還沒建立真空的時候剛好發生ALARM，重新START會誤發Index arm pick up error
                           FTestSuck.Item[i][j]==NULL_IC && FTestSuck.Suck[i][j].GetOnBit())
                        {
                            FTestSuck.Suck[i][j].Normal();
                            CloseSuckok=false;                                  //kevin 20110505
                        }

                        if(bArm2NeedSuck==false &&                              //JerryYang 20240816 : 修正當index arm真空開啟，還沒建立真空的時候剛好發生ALARM，重新START會誤發Index arm pick up error
                           BTestSuck.Item[i][j]==NULL_IC && BTestSuck.Suck[i][j].GetOnBit())
                        {
                            BTestSuck.Suck[i][j].Normal();
                            CloseSuckok=false;                                  //kevin 20110505
                        }
                    }
                }
            }
        }
    }

    if(CloseSuckok==false && INDEX_SUCKER_TYPE==1)                              //kevin 20110505
    {
       bIndexPlaceIcCheck=false;                                                //kevin 20110505 可以秀alarm
    }

    if(SystemStart)
    {
        if(SystemNG)
        {
            StopAllMotor();                                                     // alarm by alarm message
            SystemStart=false;
        }
    }

    if(SystemStart)
    {
        CheckSystemPower=true;
    }
    return SystemStart;
}
extern bool SECS_GEM_PPMUSIC_CONTROL_flag;
extern int  iSECS_GEM_PPMUSIC_CONTROL_CLASS;

extern bool SECS_GEM_PPSIGNALTOWER_CONTROL_flag;
extern int  iSECS_GEM_PPSIGNALTOWER_CONTROL_RED;
extern int  iSECS_GEM_PPSIGNALTOWER_CONTROL_GREEN;
extern int  iSECS_GEM_PPSIGNALTOWER_CONTROL_YELLOW;
//==============================================================================
// 處理三色燈
//==============================================================================
//extern AnsiString sAmkorAlarmMessage;
//extern AnsiString sAmkorAlarmBuzzer;
void ShowRunLed()
{
    static bool OldFlushFlag;
    int is, i;
    if(fNote->fShow)
    {
        if(CosFunction.bTowerLightUseLD &&  //JerryYang 20230721 : LD/ULD區分不同的異常音樂
          (fNote->edErrorCode->Text=="MES0920" || fNote->edErrorCode->Text=="MES0921" || fNote->edErrorCode->Text=="MES0922" || fNote->edErrorCode->Text=="MES0923" ||
           fNote->edErrorCode->Text=="MES1021" || fNote->edErrorCode->Text=="MES1421" ||
           fNote->edErrorCode->Text=="MES1721" || fNote->edErrorCode->Text=="MES1821" || fNote->edErrorCode->Text=="MES1921" ||     //Steven 20230907 : For HT-9011UC
           fNote->edErrorCode->Text=="MES2821" || fNote->edErrorCode->Text=="MES2921" || fNote->edErrorCode->Text=="MES3021" ||
           fNote->edErrorCode->Text=="MES1120" || fNote->edErrorCode->Text=="MES1220" || fNote->edErrorCode->Text=="MES1320" ||
           fNote->edErrorCode->Text=="MES2520" || fNote->edErrorCode->Text=="MES2620" || fNote->edErrorCode->Text=="MES2720"))
        {
            RunState=LED_AutoRetest;
        }
        else if(fNote->AlarmType==3)    //Steven 20120203 : MES的Code用Message的音樂
        {
            RunState=LED_Message;
        }
        else
        {
            RunState=LED_ErrJam;
        }
    }
    else if(MyMessageBox->fShow || fShowBinSet->fShow)  //JerryYang 20190523 Microchip要求跳出site map確認視窗時蜂鳴器要叫
    {
//        if(iUnLoaderCount)      //Jou 20150721 : 重新啟用功能    //JerryYang 20160811 Mark掉,使用Message的設定
//            RunState=LED_TrayCounter;
//        else
            RunState=LED_Message;
    }
    else if(bTesterSendPause)   //Steven 20201022 : For RFMD
    {
        RunState=LED_Message;
        if(bTesterPauseMusic)   //Steven 20220616 : Can select "Alarm Reset" when show "Tester Pause" for QORVO.
            bAlarmBuzzer=true;
        else
            bAlarmBuzzer=false;
    }
    else if(fHome->fShow)
    {
        RunState=LED_Homeing;
    }
    else if(bReplyFTCTAlarm==true ||                                            //RogerYang 20251021 : 瑞薩FT-CT FTCT發送違法指令回傳Error需要報警
            bContinueMessageByReply71==true)                                    //RogerYang 20251107 : 瑞薩FT-CT Add Continue Form
    {
        RunState=LED_Message;
    }
    else if(SystemStart && (LastSet.iRunStartMode==rsmAutoRetest || bAutoRetestMusic))   //Steven 20140409 : Auto Retest
    {
        RunState=LED_AutoRetest;
    }
#ifndef DEBUG_SPIL
    else if(SystemStart && LastSet.iTester==OFF_LINE &&
            OFFLINE_ALARM)                                                      //Steven 20140805
    {
        RunState=LED_OfflineRun;
    }
#endif
    else if(SystemStart)
    {
        if(LastSet.iTemperature!=Tempture_Ambient &&
           fHeaterOK==false)
        {
            RunState=LED_Heating;
        }
        else if(bTesterDuckingFinishLightYellowAndAlarmOn || bNeedMusicAndAlarmOn) //kevin 20170817 (Steven) add music alarm
        {
            RunState=LED_Message;
            bAlarmBuzzer=true;
            bNeedMusicFinishLighAndAlarmOn=true;                                //kevin 20170817 (Steven) add music alarm
        }
        else
        {
            RunState=LED_Running;
        }
    }
    else
    {
        if(IniConfig.bG14UseStartSoundAlarm && bStartMoveSpeed)                //kevin 20201116  Start 發出聲音 不動 5sec
            RunState=LED_Message;
        else
            RunState=LED_Pause;
    }
    // 處理音樂
    if(fTowerLight->fShow==false)
    {
        if(IniConfig.bEnable_SECS_GEM==true && SECS_GEM_PPMUSIC_CONTROL_flag==true)       //Steven 20150605 : Add SECS GEM控制蜂鳴器
        {
            bAlarmBuzzer=true;
            for(i=0; i<4; i++)
                SW[SwMusic1+i].Off();
            SW[SwMusic1+iSECS_GEM_PPMUSIC_CONTROL_CLASS-1].On();
        }
        else
        {
            is=LastSet.MusicSelect[RunState];
            if(is)
            {
                is--;
                for(i=0; i<4; i++)
                {
                    if(is==i)
                    {
                        if((RunState==LED_Message || RunState==LED_ErrJam) && bAlarmBuzzer==false)  //Steven 20111030 : 少打一個等於
                            SW[SwMusic1+i].Off();
                        else
                            SW[SwMusic1+i].On();
                    }
                    else
                    {
                        SW[SwMusic1+i].Off();
                    }
                }
            }
            else
            {
                for(i=0; i<4; i++)
                    SW[SwMusic1+i].Off();
            }
            //Eliot 2010_1206 start
            if(IniConfig.bEnableCCDUSETCPIP)
            {
                if(CCDInterfaceForm->bIdentificationFinish==true &&
                   CCDInterfaceForm->iIdentificationStatus==2)//0:未測試 1:Pass 2:Fail
                {
                    SW[SwMusic1].On();
                }
            }
            //Eliot 2010_1206 end
        }
    }
    if(OldFlushFlag==FlushFlag)
    {
        return;
    }
    OldFlushFlag=FlushFlag;

    if(IniConfig.bEnable_SECS_GEM==true && SECS_GEM_PPSIGNALTOWER_CONTROL_flag==true)
    {
        bAlarmBuzzer=true;
        if(iSECS_GEM_PPSIGNALTOWER_CONTROL_RED==0)
            fMain->ledRed->Value=false;
        else if(iSECS_GEM_PPSIGNALTOWER_CONTROL_RED==1)
            fMain->ledRed->Value=true;
        else
            fMain->ledRed->Value=FlushFlag;

        if(iSECS_GEM_PPSIGNALTOWER_CONTROL_GREEN==0)
            fMain->ledGreen->Value=false;
        else if(iSECS_GEM_PPSIGNALTOWER_CONTROL_GREEN==1)
            fMain->ledGreen->Value=true;
        else
            fMain->ledGreen->Value=FlushFlag;

        if(iSECS_GEM_PPSIGNALTOWER_CONTROL_YELLOW==0)
            fMain->ledYellow->Value=false;
        else if(iSECS_GEM_PPSIGNALTOWER_CONTROL_YELLOW==1)
            fMain->ledYellow->Value=true;
        else
            fMain->ledYellow->Value=FlushFlag;
    }
    else
    {
        //ChungHung 20130528 SCK要求AutoClean後要自動檢測是否Loader有補Tray
        if(IniConfig.bA08LastLoaderAutoCleanOutAndCheckAgain && bLoaderNoTrayAutoCleanOut && RunState==LED_Running)
        {
            fMain->ledGreen->Value=FlushFlag;
        }
        else
        {
            if(LastSet.MessageLight[RunState][0]==1)        fMain->ledGreen->Value=true;
            else if(LastSet.MessageLight[RunState][0]==2)   fMain->ledGreen->Value=FlushFlag;
            else                                            fMain->ledGreen->Value=false;
        }
        if(LastSet.MessageLight[RunState][1]==1)        fMain->ledYellow->Value=true;
        else if(LastSet.MessageLight[RunState][1]==2)   fMain->ledYellow->Value=FlushFlag;
        else                                            fMain->ledYellow->Value=false;
        if(LastSet.MessageLight[RunState][2]==1)        fMain->ledRed->Value=true;
        else if(LastSet.MessageLight[RunState][2]==2)   fMain->ledRed->Value=FlushFlag;
        else                                            fMain->ledRed->Value=false;

        //if(RunState==LED_Running && fMain->ALed1->Value==false)
        //ChungHung 20141015 add for SCK When the handler start running (finished Start count function),Yellow light blinking & alarm on (press "alarm reset" then clear)
        if(RunState==LED_Running && fMain->ALed1->Value==false || bTesterDuckingFinishLightYellowAndAlarmOn)
            fMain->ledYellow->Value=FlushFlag;

        if(bNeedMusicAndAlarmOn)//kevin 20170816 (Steven) 發出音樂及閃燈
        {
            fMain->ledRed->Value=FlushFlag;
            bLampAlarmReset=true;
            if(bFrontPadActive) SW[SwFKAlarmReset].On();
            else                SW[SwRKAlarmReset].On();
        }

        //jou 2014-12-19 Temperature Less 30 deg. Show Light
        //jou 20180529 : Temperature Heater Ok Show Light
        if(CosFunction.bTempLess30degShowLight || CosFunction.bTempHeaterOkShowLight)
        {
            if(bTempLess30degShowLightFlag==true || bTempHeaterOkShowLightFlag==true)
            {
                fMain->ledGreen->Value=FlushFlag;
                fMain->ledYellow->Value=FlushFlag;
                fMain->ledRed->Value=FlushFlag;
            }
        }
    }

    SW[SwTowerRed].OnOff(fMain->ledRed->Value);
    SW[SwTowerYellow].OnOff(fMain->ledYellow->Value);
    SW[SwTowerGreen].OnOff(fMain->ledGreen->Value);

    //ben 20230913 add
    //==>
    if(Enable_PLCSafety_IO==true)
    {
        fMain->pnlSafePLC->Visible=true;
        fMain->ledSafePLC->Value=bSafePLCThread;
    }
    else
    {
        fMain->pnlSafePLC->Visible=false;
    }
    //<==
    //ben 20230913 add
}
//------------------------------------------------------------------------------
extern int iArmTask,OutArmTask,iTestHeadMotorTask;
void ShowRunLabel()
{
    AnsiString str;
    static bool OldFlushFlag=false;
    static int OldiHeaterWaitTime=-1, CoolTime=0, iOldSec=-1;
    AnsiString EmgStr="";
    static int iTempStart[2]={0,0};                                             //kevin 20200829 add
    static int iTempFinish[2]={0,0};                                            //kevin 20200829 add

    if(OldFlushFlag==FlushFlag && OldiHeaterWaitTime==iHeaterWaitTime)
        return;

    OldFlushFlag=FlushFlag;

    if(FlushFlag==false && OldiHeaterWaitTime==iHeaterWaitTime)
        return;
    OldiHeaterWaitTime=iHeaterWaitTime;

    if(bSendChangeTempDelayAlarm)                                               //kevin 20151023 Auto Temp offset
    {
        if(tSendChangeTempDelay.Off())
        {
            bSendChangeTempDelayAlarm=false;
            RecordProcess("Auto Temp offset finish");
        }
    }

    if(IsTriTempFixDoorLockCheck())                                             //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    {
        fMain->ShowNowStatus(clRed, "FixDoorOpen"); //檢查SafeDoor 6            //Ztex 2024.05.12 Change FixDoorOpen
        CoolTime=0;
        return;
    }

    if(((MyMessageBox->fShow==true &&
         iUnLoaderCount==0) ||                                                  //Steven 20140627 : Add for Secs Gem
         fNote->fShow==true) &&                                                 //JerryYang 20160830 考慮auto滿tray時show message但不停機的情況,機台不要show alarm
        bAlarmReset==false)                                                     //Steven 20140905 : 紀錄有被按下Alarm Reset
    {
        fMain->ShowNowStatus(clRed, "Alarm");
        return;
    }
    else if(IsSafeLockCheck())
    {
        fMain->ShowNowStatus(clBlack, "LOCK");
        CoolTime=0;
        return;
    }
    else if(IsEMGPressed())
    {
        //Ifor 20210810 add:Copy 7000
        //==>
//        if(bOutArmPlaceToUnloaderDestroy==true)
//        {
//            bOutArmPlaceToUnloaderDestroy_EMGAndPowerOff=true;
//        }
//        else
//        {
//            bOutArmPlaceToUnloaderDestroy_EMGAndPowerOff=false;
//        }
        //<==
        //Ifor 20210810 add:Copy 7000

        if(Sen[SnFrontLeftEMG].IsOff())
            EmgStr="EMG 1";
        else if(Sen[SnFrontRightEMG].IsOff())
            EmgStr="EMG 4";
        else if(Sen[SnRearLeftEMG].IsOff())
            EmgStr="EMG 2";
        else if(Sen[SnRearRightEMG].IsOff())                                    //kevin 20140121 add
            EmgStr="EMG 3";
        else if(Enable_PLCSafety_IO && Sen[SnAllEMG].IsOff())                   //KenHsieh 20250212 : 新增PLC 斷線可瞬間判斷EMG及安全門
            EmgStr="EMG & PLC";
        else
            EmgStr="Servo Off";                                                 //kevin 20140121 偵測sevon 訊號

        fMain->ShowNowStatus(clRed, EmgStr);
        CoolTime=0;
        return;
    }
    else if(Sen[SnMotorPower].IsOff())
    {
        //Ifor 20210810 add:Copy 7000
        //==>
//        if(bOutArmPlaceToUnloaderDestroy==true)
//        {
//            bOutArmPlaceToUnloaderDestroy_EMGAndPowerOff=true;
//        }
//        else
//        {
//            bOutArmPlaceToUnloaderDestroy_EMGAndPowerOff=false;
//        }
        //<==
        //Ifor 20210810 add:Copy 7000

        fMain->ShowNowStatus(clRed, "Power Off");
        CoolTime=0;
        return;
    }

    if(IniConfig.bQAMode==true && LastSet.iRunStartMode==rsmQAMode)             //Steven 20111128
    {
        fMain->labQAMode->Visible=true;
        if(CosFunction.bQAModeUseUnloadCnt)                                     //JerryYang 20221004 : Maxim版本QA mode
        {
            str.sprintf("[Operation count %d / Setting Count %d]", iQAModePassCT, Prod.iQAModeCount);   //jou 2012-05-03 增加QA mode使用獨立的Loader Count
        }
        else
        {
            str.sprintf("[Operation count %d / Setting Count %d]", iQAModeLoaderCT, Prod.iQAModeCount);   //jou 2012-05-03 增加QA mode使用獨立的Loader Count
        }
        fMain->labQAMode->Caption=str;
    }
    else
    {
        fMain->labQAMode->Visible=false;
    }

    if(SystemStart)
    {
        fMain->EnabledSetupFile(false);
        fLotInfo->btnFtpServer->Enabled=false;
        fLotInfo->btnFtpHD->Enabled=false;

        //ChungHung 20140716 add if testing not finish can not homing
        if(bTesterSendPause)                                                    //Steven 20201022 : For RFMD
        {
            fMain->ShowNowStatus(clRed, "Tester PAUSE");
            CoolTime=0;
        }
        else if(IniConfig.bI01TesterFinishThenHome &&
                LastSet.iTester==ON_LINE &&
                bWaitTesterFinish)                                              //kevin 20150721 收到測試資料才能home
        {
            fMain->ShowNowStatus((TColor) 0x000080FF, "Wait Tester");
            CoolTime=0;
        }
        else if(fHome->fShow)
        {
            fMain->ShowNowStatus((TColor) 0x000080FF, "Homing");
            CoolTime=0;
        }
        else if((ATC_SYSTEM==eATCHonPrecType ||
                 ATC_SYSTEM==eNewATCSystem) &&
                bNeedWaitATCRunSelfTestFinish)                                  //Ifor 20160720 add for ATC Safe Test start
        {
            fMain->ShowNowStatus((TColor) 0x000080FF, "ATC Self Test");
            CoolTime=0;
        }
        else if(CUSTOMER_CODE!=CC_KYEC_LEE &&
                bDoIniStartAutoIonFanClean &&                                   //Isaac 20210609 : IO觸發IonFan清針
                bStartAutoIonFanClean==true)                                    //Ifor 20210720 add: IO觸發IonFan清針才顯示
        {
            fMain->ShowNowStatus((TColor) 0x000080FF, "Auto Clean Ion Fan");
        }
        else if(AUTO3_IS_MAGAZINE==1 && iMagazineStatus!=0)                     //Ifor 20231130 add: 0: Normal 1:Full Tray 2:Manual put tray
        {
            fMain->labDelayStatus->Caption="MAGAZINE Remove The Tray";
            fMain->labDelayStatus->Visible=true;
        }
        else
        {
            if(LastSet.iRunStartMode==rsmAutoRetest)                            //Steven 20140409 : Auto Retest
            {
                fMain->ShowNowStatus(clNavy, "Auto Retest");
            }
//            else if(fHeaterOK==false && LastSet.iTemperature==Tempture_Ambient && Temperature.bAmbientGuardbandCheck && IniConfig.bL20AbientGuardBand)  //Steven 20200427 : JCET凌中心說常溫溫度過低不能生產
//            {
//                fMain->ShowNowStatus((TColor) 0x000080FF, "Heater Wait");
//            }
            else if(LastSet.iTemperature==Tempture_Hot ||
                    LastSet.iTemperature==Tempture_AmbientHot ||                //kevin 2014091
                    (LastSet.iTemperature==Tempture_Ambient && CUSTOMER_CODE==CC_KYEC_LEE))     //KaiChen 20180201 (Steven) ：Use initial start delay in socket 增加常溫顯示
            {
                if(bSendChangeTempDelayAlarm)                                   //kevin 20151023 Auto Temp offset
                {
                    fMain->ShowNowStatus(clNavy, "Auto Temp offset");
                }
                else if(fHeaterOK ||
                        (LastSet.iTemperature==Tempture_Ambient &&
                         CUSTOMER_CODE==CC_KYEC_LEE))                           //KaiChen 20180201 (Steven) ：Use initial start delay in socket 增加常溫顯示
                {
                    if(bOneTimeHotPlateCheckAll && IniConfig.bE39CheckHotPlateAfterCleanOutAndBeforeTrayFeed && LastSet.iTemperature==Tempture_Hot)  //Steven : 20120315 主畫面狀態 --> HP Check
                    {
                        fMain->ShowNowStatus(clNavy, "HP Check");
                    }
                    else if(fContact->IsRun2DCheck()==true)                     //JerryYang 20250220 : 2DID硬體順序檢查功能
                    {
                        fMain->ShowNowStatus(clNavy, "2DID Map Checking");
                    }
                    else if(bRunAutoClean)                                      //Steven 20120208
                    {
                        fMain->ShowNowStatus(clNavy, "Cleaning");
                    }
                    else if(bRunOcrInsp)                                        //ChungHung 20120830 add OCR Function add
                    {
                        fMain->ShowNowStatus(clNavy, "OCR Insp");
                    }
                    else if(bDoEmptySocketCheck)                                //Steven 20201022 : For RFMD Empty Socket Check Funstion.
                    {
                        fMain->ShowNowStatus(clRed, "ESC");
                    }
                    else if(bResetMode || bResetModeAndCleanOut)                //JerryYang 20151007 : ResetMode可選OneCycle或CleanOut
                    {
                        if(bDoEmptySocketOneCycle)                              //Steven 20201022 : For RFMD Empty Socket Check Funstion.
                            fMain->ShowNowStatus(clRed, "ESC Reset");
                        else
                            fMain->ShowNowStatus(clRed, "Resetting");
                    }
                    else if(bCheckIndex)                                        //Steven 20110502
                    {
                        fMain->ShowNowStatus(clNavy, "Piggy Back");
                        SocketAirCoolingStart();                                //jou 2016-04-28 Socket Air Cooling contact count trun on
                    }
                    else if(IniConfig.bQAMode==true &&
                            LastSet.iRunStartMode==rsmQAMode)                   //Steven 20111128
                    {
                        fMain->ShowNowStatus(clLime, "QA Mode");
                    }
                    else if(bIsAutoOneCycleAutoclean)                           //kevin 20120710
                    {
                        fMain->ShowNowStatus(clNavy, "Onecycle Cleaning");
                    }
                    else if(IniConfig.bA08LastLoaderAutoCleanOutAndCheckAgain && bLoaderNoTrayAutoCleanOut) //ChungHung 20130528 SCK要求AutoClean後要自動檢測是否Loader有補Tray
                    {
                        fMain->ShowNowStatus(clNavy, "No Tray");
                    }
                    else
                    {
                        if((CUSTOMER_CODE==CC_ASE_KaohSiung || CUSTOMER_CODE==CC_KYEC_LEE) &&
                           USE_AUTO_RETEST==eartInstall &&
                           (bAutoReTest_ART || (CUSTOMER_CODE==CC_KYEC_LEE && IniConfig.bA10_AutoReTest)) &&
                           (LastSet.iRunStartMode==rsmInitial_ART       ||
                            LastSet.iRunStartMode==rsmContinuStart_ART  ||
                            LastSet.iRunStartMode==rsmContinuRetest_ART))       //kevin 20150602     //Frank 20161017 KYEC_KW want add
                        {
                            if(LastSet.iRunStartMode==rsmAutoRetest)            //kevin 20150721 : Auto Retest
                            {
                                fMain->ShowNowStatus(clNavy, "Auto Retest");
                            }
                            else
                            {
                                if(CUSTOMER_CODE==CC_ASE_KaohSiung)
                                {
                                    fMain->ARTCombine->Visible=true;
                                    if(bARTSeparate==false)                     //kevin 20170908 (wei) add art 分bin 狀態成立
                                    {
                                        fMain->ARTCombine->Caption="ART Combine Bin";
                                    }
                                    else
                                    {
                                        fMain->ARTCombine->Caption="ART Separate Bin";
                                    }
                                }

                                switch(LastSet.iAutoRetestCount_ART)            //Steven 20140409 : Auto Retest
                                {
                                    case 0:
                                        fMain->ShowNowStatus(clNavy, "FT ART");
                                        break;
                                    case 1:
                                        fMain->ShowNowStatus(clNavy, "RT1 ART");
                                        break;
                                    case 2:
                                        fMain->ShowNowStatus(clNavy, "RT2 ART");
                                        break;
                                    case 3:
                                        fMain->ShowNowStatus(clNavy, "RT3 ART");
                                        break;
                                    case 4:
                                        fMain->ShowNowStatus(clNavy, "RT4 ART");
                                        break;
                                    case 5:
                                        fMain->ShowNowStatus(clNavy, "RT5 ART");
                                        break;
                                    default:
                                        fMain->ShowNowStatus(clLime, "Running");
                                        break;
                                }
                            }
                        }
                        else if(LastSet.iRunStartMode==rsmContinuRetest_ART)    //kevin 20150715
                        {
                            if(CUSTOMER_CODE==CC_ASE_KaohSiung)
                                fMain->ShowNowStatus(clNavy, "ART_Tray Move");  //kevin 20170908
                            else
                                fMain->ShowNowStatus(clNavy, "ART_ReTest");
                        }
                        else
                        {
                            fMain->ShowNowStatus(clLime, "Running");
                        }
                    }
                    CoolTime=0;
                    if(iOneCycle==0 && bRunAutoClean==false &&                  //kevin 20130220 add iOneCycle 時會秀sacktime
                       bIsAutoOneCycleAutoclean==false &&
                       iHeaterWaitTime!=0)                                      //kevin 20121023
                    {
                        fMain->ShowNowStatus(clLime, AnsiString(iHeaterWaitTime));
                    }

                    if(iHeaterWaitTime!=0)
                    {
                        fMain->ShowNowStatus(clLime, AnsiString(iHeaterWaitTime));
                        fMain->labDelayStatus->Caption="Heater Wait Time";      //wei 20171020 (jou) 延遲狀態顯示
                        fMain->labDelayStatus->Visible=true;                    //JerryYang 20171211 (Steven) 有用到再顯示避免擋到SECS GEM連線狀態
                    }
                    else if(Temperature.bLBTempFunction && iTriggerBoostFunction==Temperature.eBLBL)
                    {
                        fMain->ShowNowStatus(clFuchsia, AnsiString("Wait L/B"));
                    }
                    else if((Temperature.bBoostFuncttion || Temperature.bLBTempFunction) && iTriggerBoostFunction!=-1 && iBoostFuncStep==1)   //Steven 20180817 : Boost Function
                    {
                        fMain->ShowNowStatus(clFuchsia, AnsiString(iInitialCount));
                        fMain->labDelayStatus->Caption="Boost Duration";
                        fMain->labDelayStatus->Visible=true;
                    }
                    else if((Temperature.bBoostFuncttion || Temperature.bLBTempFunction) && iTriggerBoostFunction!=-1 && iBoostFuncStep==4)   //Steven 20180817 : Boost Function
                    {
                        fMain->ShowNowStatus(clFuchsia, AnsiString(iInitialCount));
                        fMain->labDelayStatus->Caption="Post Boost Duration";
                        fMain->labDelayStatus->Visible=true;
                    }
                    else if(((IniConfig.bA05UseAutoDocking==true && Temperature.bUseTesterDocking==true) || CUSTOMER_CODE==CC_SCK) && bTesterDucking==true && iTesterDucking>0) //ChungHung 20140714 only for SCK add CUSTOMER_CODE==CC_SCK
                    {
                        fMain->ShowNowStatus((TColor) 0x000080FF, AnsiString(iTesterDucking));
                    }
                    else if(bNeedInitialTestDelay && iInitialCount>0)           //ChungHung 20140801 add Korea Want to count down in main status
                    {
                        fMain->ShowNowStatus(clFuchsia, AnsiString(iInitialCount));
                        fMain->labDelayStatus->Caption=InitialTestDelayStatus();    //wei 20171020 (jou) 延遲狀態顯示
                        fMain->labDelayStatus->Visible=true;                        //JerryYang 20171211 (Steven) 有用到再顯示避免擋到SECS GEM連線狀態
                    }
                    else if(CosFunction.bEnableAfterTestedDelay && iAfterTestedCount>0) //ChungHung 20140730 add for ATK function after tested delay time
                    {
                        fMain->ShowNowStatus(clTeal, AnsiString(iAfterTestedCount));
                    }
                    else if(iSoakTimer>0)                                       //2013-11-27    Dell Add Index soak time
                    {
                        fMain->ShowNowStatus(clLime, AnsiString(iSoakTimer));
                        fMain->labDelayStatus->Caption="Soak Time";             //wei 20171020 (jou) 延遲狀態顯示
                        fMain->labDelayStatus->Visible=true;                    //JerryYang 20171211 (Steven) 有用到再顯示避免擋到SECS GEM連線狀態
                    }
                    else if(fHeaterOK && fHeaterStableOK==false && iStableTime>0)   //JerryYang 20210122 : ASE-CL新增待溫功能
                    {
                        fMain->ShowNowStatus(clLime, AnsiString(iStableTime));
                        fMain->labDelayStatus->Caption="Stable Time";
                        fMain->labDelayStatus->Visible=true;
                    }
                    else if(iInitialSoakTimer>0)                                //Steven 20140827
                    {
                        fMain->ShowNowStatus(clLime, AnsiString(iInitialSoakTimer));
                        fMain->labDelayStatus->Caption="Initial Soak Time";     //wei 20171020 (jou) 延遲狀態顯示
                        fMain->labDelayStatus->Visible=true;                    //JerryYang 20171211 (Steven) 有用到再顯示避免擋到SECS GEM連線狀態
                    }
                    else if(bInitStartDelayTime && iInitStartDelayCount>0)      //wei 20171020 (jou) InitStartDelayTime秒數倒數
                    {
                        fMain->ShowNowStatus(clBlue, AnsiString(iInitStartDelayCount));
                        fMain->labDelayStatus->Caption="Initial Start Delay Time";  //wei 20171020 (jou) 延遲狀態顯示
                        fMain->labDelayStatus->Visible=true;                    //JerryYang 20171211 (Steven) 有用到再顯示避免擋到SECS GEM連線狀態
                    }
                    else if(bInitialStart1Time && iInitialStart1Count>0)        //wei 20171020 (jou) InitialStart1 秒數倒數
                    {
                        fMain->ShowNowStatus(clBlue, AnsiString(iInitialStart1Count));
                        fMain->labDelayStatus->Caption="Initial Start 1 Time";  //wei 20171020 (jou) 延遲狀態顯示
                        fMain->labDelayStatus->Visible=true;                    //JerryYang 20171211 (Steven) 有用到再顯示避免擋到SECS GEM連線狀態
                    }
                    else if(bInitialStart2Time && iInitialStart2Count>0)        //kevin 20180905 InitialStart1 秒數倒數
                    {
                        fMain->ShowNowStatus(clBlue, AnsiString(iInitialStart2Count));
                        fMain->labDelayStatus->Caption="Shuttle Soak Time";
                        fMain->labDelayStatus->Visible=true;
                    }
                    else
                    {
                        fMain->labDelayStatus->Caption="";                      //wei 20171020 (jou) 延遲狀態顯示
                        fMain->labDelayStatus->Visible=false;                   //JerryYang 20171211 (Steven) 有用到再顯示避免擋到SECS GEM連線狀態
                        for(int i=0; i<15; i++)                                 //kevin 20200730 10->15
                        {
                            bInitialTestDelayStatus[i]=false;
                        }
                    }

                    bool bFlag=fMain->labDelayStatus->Visible;
                    if((Temperature.bBoostFuncttion || Temperature.bLBTempFunction) && iTriggerBoostFuncBack!=-1 && iBoostFuncStep==10)   //Steven 20180817 : Boost Function
                    {
                        str.sprintf("Boost Cooling : %d", iBoostCountDown);
                        fMain->labDelayStatus->Caption=str;
                        fMain->labDelayStatus->Visible=true;
                    }
                    else if((Temperature.bBoostFuncttion || Temperature.bLBTempFunction) && iTriggerBoostFuncBack!=-1 && iBoostFuncStep==11)   //Steven 20180817 : Boost Function
                    {
                        str.sprintf("L/B Boosting");
                        fMain->labDelayStatus->Caption=str;
                        fMain->labDelayStatus->Visible=true;
                    }
                    else
                    {
                        if(bFlag==false)
                            fMain->labDelayStatus->Visible=false;
                    }

                    if(IniConfig.bUseAutoSiteMapping)                           //jou 2011-03-24 start : Auto Site Mapping
                    {
                        if(LastSet.iTemperature==Tempture_Hot && LastSet.iRunStartMode==rsmAutoSiteMap && bSiteMappingCHKOK==false)
                        {
                            if(iShuttleHeaterWaitTime>0)
                            {
                                fMain->ShowNowStatus(clLime,AnsiString(iShuttleHeaterWaitTime));
                            }
                        }
                    }
                    //jou 2011-03-24 end
                }
                else
                {
                    iTempStart[0]=0;                                            //kevin 20200829 add
                    iTempStart[1]=0;                                            //kevin 20200829 add
                    if(Temperature.bLBTempFunction && iTriggerBoostFunction==Temperature.eBLBL)
                    {
                        fMain->ShowNowStatus(clFuchsia, AnsiString("Wait L/B"));
                    }
                    else if((Temperature.bBoostFuncttion || Temperature.bLBTempFunction) && iTriggerBoostFunction!=-1 && iBoostFuncStep==1)   //Steven 20180817 : Boost Function
                    {
                        fMain->ShowNowStatus(clFuchsia, AnsiString(iInitialCount));
                        fMain->labDelayStatus->Caption="Boost Duration";
                        fMain->labDelayStatus->Visible=true;
                    }
                    else if((Temperature.bBoostFuncttion || Temperature.bLBTempFunction) && iTriggerBoostFunction!=-1 && iBoostFuncStep==4)   //Steven 20180817 : Boost Function
                    {
                        fMain->ShowNowStatus(clFuchsia, AnsiString(iInitialCount));
                        fMain->labDelayStatus->Caption="Post Boost Duration";
                        fMain->labDelayStatus->Visible=true;
                    }
                    else
                    {
                        iTempStart[0]=1;                                        //kevin 20200829 add
                        iTempStart[1]=1;                                        //kevin 20200829 add
                        fMain->ShowNowStatus((TColor) 0x000080FF, "Heater Wait");
                    }
                    CoolTime=0;
                    if(iTemperatureOk!=2)
                    {
                        iTemperatureOk=2;                                       //kevin 20150914
                        NewRecordProcess("MES2130", "Temperature Wait");        //kevin 20150914
                        iTempStart[0]=1;                                        //kevin 20200829 add
                        iTempStart[1]=1;                                        //kevin 20200829 add
                    }

                    if(iTempStart[0]==1)                                        //kevin 20200829 add 加熱完成
                    {
                       iTempFinish[0]=2;
                    }

                    if(iTempStart[0]==0 && iTempFinish[0]==2)                   //kevin 20200829 add 加熱完成
                    {
                       iTempFinish[0]=0;                                        //kevin 20200829 add
                       RecordProcess("Working Temperature Arrived Done");       //kevin 20200829  加熱完成
                    }
                }
            }
            else
            {
                if(iHeaterCooling)
                {
                    fMain->ShowNowStatus((TColor) 0x000080FF, "Cooling Wait");
                    CoolTime=0;
                }
                else
                {
                    if(fContact->IsRun2DCheck())                                //JerryYang 20250220 : 2DID硬體順序檢查功能
                    {
                        fMain->ShowNowStatus(clNavy, "2DID Map Checking");
                    }
                    else if(bRunAutoClean)                                      //Steven 20120208
                    {
                        fMain->ShowNowStatus(clNavy, "Cleaning");
                    }
                    else if(bDoEmptySocketCheck)                                //Steven 20201022 : For RFMD Empty Socket Check Funstion.
                    {
                        fMain->ShowNowStatus(clRed, "ESC");
                    }
                    else if(bResetMode || bResetModeAndCleanOut)                //JerryYang 20151007 : ResetMode可選OneCycle或CleanOut
                    {
                        if(bDoEmptySocketOneCycle)                              //Steven 20201022 : For RFMD Empty Socket Check Funstion.
                            fMain->ShowNowStatus(clRed, "ESC Reset");
                        else
                            fMain->ShowNowStatus(clRed, "Resetting");
                    }
                    else if(bCheckIndex)                                        //Steven 20110502
                    {
                        fMain->ShowNowStatus(clNavy, "Piggy Back");
                        SocketAirCoolingStart();                                //jou 2016-04-28 Socket Air Cooling contact count trun on
                    }
                    else if(IniConfig.bQAMode==true && LastSet.iRunStartMode==rsmQAMode)    //Steven 20111128
                    {
                        fMain->ShowNowStatus(clLime, "QA Mode");
                    }
                    else if(bIsAutoOneCycleAutoclean)                           //kevin 20120710
                    {
                        fMain->ShowNowStatus(clNavy, "Onecycle Cleaning");
                    }
                    else if(IniConfig.bA08LastLoaderAutoCleanOutAndCheckAgain && bLoaderNoTrayAutoCleanOut) //ChungHung 20130528 SCK要求AutoClean後要自動檢測是否Loader有補Tray
                    {
                        fMain->ShowNowStatus(clNavy, "No Tray");
                    }
                    else if((CUSTOMER_CODE==CC_ASE_KaohSiung || CUSTOMER_CODE==CC_KYEC_LEE) &&
                            USE_AUTO_RETEST==eartInstall &&
                            (bAutoReTest_ART || (CUSTOMER_CODE==CC_KYEC_LEE && IniConfig.bA10_AutoReTest)) &&
                            (LastSet.iRunStartMode==rsmInitial_ART      ||
                             LastSet.iRunStartMode==rsmContinuStart_ART ||
                             LastSet.iRunStartMode==rsmContinuRetest_ART))      //kevin 20150610       //Frank 20161017 KYEC_KW want add
                    {
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung)
                        {
                            fMain->ARTCombine->Visible=true;                    //kevin 20170908 (wei) add art 分bin 狀態成立
                            if(bARTSeparate==false)
                            {
                                fMain->ARTCombine->Caption="ART Combine Bin";
                            }
                            else
                            {
                                fMain->ARTCombine->Caption="ART Separate Bin";
                            }
                        }

                        switch(LastSet.iAutoRetestCount_ART)                    //Steven 20140409 : Auto Retest
                        {
                            case 0:
                                fMain->ShowNowStatus(clNavy, "FT ART");
                                break;
                            case 1:
                                fMain->ShowNowStatus(clNavy, "RT1 ART");
                                break;
                            case 2:
                                fMain->ShowNowStatus(clNavy, "RT2 ART");
                                break;
                            case 3:
                                fMain->ShowNowStatus(clNavy, "RT3 ART");
                                break;
                            case 4:
                                fMain->ShowNowStatus(clNavy, "RT4 ART");
                                break;
                            case 5:
                                fMain->ShowNowStatus(clNavy, "RT5 ART");
                                break;
                            case 6:
                                fMain->ShowNowStatus(clNavy, "RT6 ART");
                                break;
                            case 7:
                                fMain->ShowNowStatus(clNavy, "RT7 ART");
                                break;
                            case 8:
                                fMain->ShowNowStatus(clNavy, "RT8 ART");
                                break;
                            case 9:
                                fMain->ShowNowStatus(clNavy, "RT9 ART");
                                break;
                            case 10:
                                fMain->ShowNowStatus(clNavy, "RT10 ART");
                                break;
                            default:
                                fMain->ShowNowStatus(clLime, "ART Over");
                                break;
                        }
                    }
                    else if(LastSet.iRunStartMode==rsmContinuRetest_ART)        //kevin 20150715
                    {
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung)
                            fMain->ShowNowStatus(clNavy, "ART_Tray Move");
                        else
                            fMain->ShowNowStatus(clNavy, "ART_ReTest");
                    }
                    else
                    {
                        if(IniConfig.bG12ContractModeManualMessage && bContractModeTest)    //kevin 20180222 (Steven) Arm 1 Arm2 吸取IC 做CONTRACT MODE
                        {                                                                   //kevin 20180222 (Steven) contract mode 秀手動送測試訊號
                            fMain->ShowNowStatus(clLime, "Manual T.Start");
                        }
                        else
                        {
                            fMain->ShowNowStatus(clLime, "Running");
                        }
                    }
                    CoolTime=0;
                }
            }
        }
        bHALTing=false;                                                         //jou 2012-01-30 常溫 & 機台沒在跑的時候chamber風扇不轉
    }
    else
    {
        if((IniConfig.bEnable_SECS_GEM==true && IniConfig.bRCMDStart==true && bPhysicalStart==true) ||          //Steven 20141006 : SECS GEM使用Remote Start功能
           (CUSTOMER_CODE==CC_ChipMos_ZHUBEI && IniConfig.bN25_1_EnableStartControl && bPhysicalStart==true))   //Steven 20210413 : 南茂的自動Start功能
        {
            fMain->ShowNowStatus(clNavy, "RUN CHECK");
            if(IniConfig.bEnable_SECS_GEM==true && IniConfig.bRCMDStart==true && bPhysicalStart==true)
                bSECSGEMAlarm=true;                                             //Ifor 20151210 鎖定面板
        }
        else if(bRunDecayTest==true)
        {
            fMain->ShowNowStatus((TColor) 0x000080FF, "Decay Test");
            CoolTime=0;
        }
        else if(CUSTOMER_CODE!=CC_KYEC_LEE &&
                bDoIniStartAutoIonFanClean &&                                   //Isaac 20210609 : IO觸發IonFan清針
                bStartAutoIonFanClean==true)                                    //Ifor 20210720 add: IO觸發IonFan清針才顯示
        {
            fMain->ShowNowStatus((TColor) 0x000080FF, "Auto Clean Ion Fan");
        }
        else if(AUTO3_IS_MAGAZINE==1 && iMagazineStatus!=0)                     //Ifor 20231130 add: 0: Normal 1:Full Tray 2:Manual put tray
        {
            fMain->labDelayStatus->Caption="MAGAZINE Remove The Tray";
            fMain->labDelayStatus->Visible=true;
        }
        else if(fMain->CheckCanChangeRealDummy()==false ||
                HasICUnderMachine() &&
                HasAnyICInMachine() &&
                CUSTOMER_CODE==CC_ASE_KaohSiung)                                //kevin 20151012
        {
            if(bCalculatePauseTime==false)
            {
                tUPH_PauseStartTime=Now();
                bCalculatePauseTime=true;
            }
            fMain->ShowNowStatus(clRed, "PAUSE");
            if(CosFunction.bOEEFunction==true && fProductionInfo->bIPSCPause==true)  //Jimmychiu 20240220 : Show IPSC Pause
            {
                fMain->ShowNowStatus(clRed, "IPSC PAUSE");
            }
            if(HasICUnderMachine() || iArmTask!=1 || OutArmTask!=1 || iTestHeadMotorTask!=1)
            {
                fMain->EnabledSetupFile(false);
                fLotInfo->btnFtpServer->Enabled=false;
            }
            else
            {
                if(CosFunction.bFTPFunction && IniConfig.bEnableFTP)
                {
                    fMain->EnabledSetupFile(true);                              //Steven 20250407 : 調整 cbSetupFileName 的 Enable判斷

                    if(IniConfig.iServerEnable<=AccessLevel)
                    {
                        if(fFTPClient->bShow==false)
                            fLotInfo->btnFtpServer->Enabled=true;
                    }
                    else
                    {
                        fLotInfo->btnFtpServer->Enabled=false;
                    }

                    if(IniConfig.iHDEnable<=AccessLevel)
                        fLotInfo->btnFtpHD->Enabled=true;
                    else
                        fLotInfo->btnFtpHD->Enabled=false;
                }
                else
                {
                    fMain->EnabledSetupFile(true);
                }
            }
            CoolTime=0;
            bHALTing=false;                                                     //jou 2012-01-30 常溫 & 機台沒在跑的時候chamber風扇不轉
            fLotInfo->btnFtpHD->Enabled=true;                                   //wei 20160314 不需要onecycle or Cleanout就可上傳
        }
        else
        {
            if(iHome==0 && CheckThermo()==false)
            {
                if(iOldSec!=SystemSec)
                {
                    iOldSec=SystemSec;
                    CoolTime++;
                }
                fMain->ShowNowStatus(clBlack,AnsiString(CoolTime));
                bHALTing=false;                                                 //jou 2012-01-30 常溫 & 機台沒在跑的時候chamber風扇不轉
            }
            else
            {
                fMain->ShowNowStatus(clBlack, "HALT");
                CoolTime=0;
                bHALTing=true;                                                  //jou 2012-01-30 常溫 & 機台沒在跑的時候chamber風扇不轉
            }

            if(CosFunction.bFTPFunction && IniConfig.bEnableFTP)                // Landam 20110324 enable setup file selection
            {
                fMain->EnabledSetupFile(true);                                  //Steven 20250407 : 調整 cbSetupFileName 的 Enable判斷

                if(IniConfig.iServerEnable<=AccessLevel)                        //jou 2012-12-21 修正 FTP clean out 後，HD & server button 沒有 enabled 的問題。
                {
                    if(fFTPClient->bShow==false)
                    {
                        if(HasICUnderMachine()==false &&
                           HasAnyICInMachine()==false)                          //JerryYang 20191212 機台內有IC不能download工作檔
                        {
                            if(CUSTOMER_CODE==CC_GIGAS &&
                               (LastSet.iRunStartMode!=rsmInitialStart &&
                                LastSet.iRunStartMode!=rsmCInitialRetest))      //Isaac 20200803 : 全智只有initial mode可以更換工作檔
                                fLotInfo->btnFtpServer->Enabled=false;
                            else
                                fLotInfo->btnFtpServer->Enabled=true;
                        }
                        else
                        {
                            fLotInfo->btnFtpServer->Enabled=false;
                        }
                    }
                }
                else
                {
                    fLotInfo->btnFtpServer->Enabled=false;
                }

                if(IniConfig.iHDEnable<=AccessLevel)
                    fLotInfo->btnFtpHD->Enabled=true;
                else
                    fLotInfo->btnFtpHD->Enabled=false;

                if(CUSTOMER_CODE==CC_Greatek)                                   //Sam 20171031 (wei) : OP 權限時隱藏 FTP
                {
                    if(AccessLevel==0)
                        fLotInfo->tsFTP->Enabled=false;                         //Sam 20190218 : 改用 Enable
                    else
                        fLotInfo->tsFTP->Enabled=true;                          //Sam 20190218 : 改用 Enable
                }
            }
            else if(CUSTOMER_CODE==CC_ASE_CL &&
                    CosFunction.bLastSetInSetUpFile)
            {
                fMain->EnabledSetupFile(OLPClientConnect);
            }
            else if(TestIF_File.bRENESAS_EnableFTCT==true)                      //RogerYang 20251020 : 瑞薩FT-CT
            {

            }
            else
            {
                fMain->EnabledSetupFile(true);
            }
            //-----------------------------------
        }

        if(AUTO3_IS_MAGAZINE==1 && iMagazineStatus!=0)
        {

        }
        else
        {
            fMain->labDelayStatus->Caption="";                                  //wei 20171020 (jou) 延遲狀態顯示
            fMain->labDelayStatus->Visible=false;                               //JerryYang 20171211 (Steven) 有用到再顯示避免擋到SECS GEM連線狀態
        }
    }

    if(IniConfig.bN09_LotCountAutoFunc && bWaitTSV)                             //Steven 20190521 : ATK lot count
    {
        fMain->ShowNowStatus(clNavy, "Waiting LCA");
    }
    if(CUSTOMER_CODE==CC_GIGAS)                                                 //Isaac 20210128 : 全智Auto Clean顯示狀態
    {
        fMain->labAutoClean->Visible=true;
        if(TestIF_File.iAutoClean_Function)
            fMain->labAutoClean->Caption="Auto clean ON";
        else
            fMain->labAutoClean->Caption="Auto clean OFF";
    }
    else
    {
        if(TestIF_File.iAutoClean_Function &&
           (IniConfig.bSIGURDFunction ||                                        //Alick 20160829 add for 矽格北興要求開啟AutoClean功能時，MAIN畫面要顯示告知Label
            CUSTOMER_CODE==CC_JCET))
            fMain->labAutoClean->Visible=true;
        else
            fMain->labAutoClean->Visible=false;
    }
    if(fAutoTeach->GetState()>0 && CosFunction.bManualSteplAutoTeach)           //JimmyChiu 20211020 : Auto alignment mode
    {
        fMain->ShowNowStatus(clLime, "Auto Alignment Mode");
        CoolTime=0;
        return;
    }
}
//------------------------------------------------------------------------------
void DoPanelLamp()
{
    if(SystemInitialOK==false)
        return;

    if(IniConfig.bA08LastLoaderAutoCleanOutAndCheckAgain)                       //ChungHung 201320610 add No Tray AlArmReset light on
    {
        if(bLoaderNoTrayAutoCleanOut && bAlarmBuzzer==true)
        {

        }
        else
        {
            if(bNeedMusicAndAlarmOn==false)                                     //kevin 20170817 (Steven) 不讓Alarm reset 燈滅
            {
                SW[SwFKAlarmReset].Off();
                SW[SwRKAlarmReset].Off();
            }
        }
    }
    else
    {
        if(fNote->fShow==false)
        {
            if(bNeedMusicAndAlarmOn==false)                                     //kevin 20170817 (Steven) 不讓Alarm reset 燈滅
            {
                SW[SwFKAlarmReset].Off();
                SW[SwRKAlarmReset].Off();
            }
        }
    }

    if((bSECSGEMAlarm && bSECSGEM_NoteAlarm==false) ||                          //wei 20150817 S10F3 按鍵不能按 //Ifor 20170616 (wei) add S10F3 後發生的Note Alarm 要恢復按鍵掃描解除Note後再鎖定按鍵
        bEnableEmployeeIDCheck==true)                                           //Ifor 20180911 (Steven) : Add 啟動 Employee ID Check
    {
        SW[SwFKPowerOff].OnOff(bLampPowerOff);
        SW[SwRKPowerOff].OnOff(bLampPowerOff);
        SW[SwFKPowerOn].OnOff(bLampPowerOn);
        SW[SwRKPowerOn].OnOff(bLampPowerOn);

        SW[SwFKReset].Off();
        SW[SwFKPause].Off();
        SW[SwRKPause].Off();
        SW[SwFKHome].Off();
        SW[SwFKStart].Off();
        SW[SwFKSkip].Off();
        SW[SwFKRetry].Off();
        SW[SwFKOneCycle].Off();
        SW[SwFKCleanOut].Off();
        SW[SwFKTrayFeed].Off();
        SW[SwFKTrayEnd].Off();
        SW[SwFKAlarmReset].Off();

        SW[SwRKReset].Off();
        SW[SwRKHome].Off();
        SW[SwRKStart].Off();
        SW[SwRKSkip].Off();
        SW[SwRKRetry].Off();
        SW[SwRKOneCycle].Off();
        SW[SwRKCleanOut].Off();
        SW[SwRKTrayFeed].Off();
        SW[SwRKTrayEnd].Off();
        SW[SwRKAlarmReset].Off();

    }
    else if(bFrontPadActive)
    {
        SW[SwFKPowerOff].OnOff(bLampPowerOff);
        SW[SwRKPowerOff].OnOff(bLampPowerOff);
        SW[SwFKPowerOn].OnOff(bLampPowerOn);
        SW[SwRKPowerOn].OnOff(bLampPowerOn);

        if(bMotorPowerState && MotorPowerOnDelay==0)
        {
            SW[SwFKReset].OnOff(bLampReset);
            SW[SwFKPause].OnOff(bLampPause);
            SW[SwRKPause].OnOff(bLampPause);
            SW[SwFKHome].OnOff(bLampHome);
            SW[SwFKStart].OnOff(bLampStart);
            SW[SwFKSkip].OnOff(bLampSkip);
            SW[SwFKRetry].OnOff(bLampRetry);
            SW[SwFKOneCycle].OnOff(bLampOneCycle);
            SW[SwFKCleanOut].OnOff(bLampCleanOut);
            SW[SwFKTrayFeed].OnOff(bLampTrayFeed);
            SW[SwFKTrayEnd].OnOff(bLampTrayEnd);
        }
        else
        {
            SW[SwFKReset].Off();
            SW[SwFKPause].Off();
            SW[SwRKPause].Off();
            SW[SwFKHome].Off();
            SW[SwFKStart].Off();
            SW[SwFKSkip].Off();
            SW[SwFKRetry].Off();
            SW[SwFKOneCycle].Off();
            SW[SwFKCleanOut].Off();
            SW[SwFKTrayFeed].Off();
            SW[SwFKTrayEnd].Off();
            SW[SwFKAlarmReset].Off();
        }
        SW[SwRKReset].Off();
        SW[SwRKHome].Off();
        SW[SwRKStart].Off();
        SW[SwRKSkip].Off();
        SW[SwRKRetry].Off();
        SW[SwRKOneCycle].Off();
        SW[SwRKCleanOut].Off();
        SW[SwRKTrayFeed].Off();
        SW[SwRKTrayEnd].Off();
        SW[SwRKAlarmReset].Off();
    }
    else
    {
        SW[SwFKPowerOff].OnOff(bLampPowerOff);
        SW[SwFKPowerOn].OnOff(bLampPowerOn);
        SW[SwRKPowerOff].OnOff(bLampPowerOff);
        SW[SwRKPowerOn].OnOff(bLampPowerOn);

        if(bMotorPowerState && MotorPowerOnDelay==0)
        {
            SW[SwRKReset].OnOff(bLampReset);
            SW[SwRKPause].OnOff(bLampPause);
            SW[SwFKPause].OnOff(bLampPause);
            SW[SwRKHome].OnOff(bLampHome);
            SW[SwRKStart].OnOff(bLampStart);
            SW[SwRKSkip].OnOff(bLampSkip);
            SW[SwRKRetry].OnOff(bLampRetry);
            SW[SwRKOneCycle].OnOff(bLampOneCycle);
            SW[SwRKCleanOut].OnOff(bLampCleanOut);
            SW[SwRKTrayFeed].OnOff(bLampTrayFeed);
            SW[SwRKTrayEnd].OnOff(bLampTrayEnd);
        }
        else
        {
            SW[SwRKReset].Off();
            SW[SwRKPause].Off();
            SW[SwRKHome].Off();
            SW[SwRKStart].Off();
            SW[SwRKSkip].Off();
            SW[SwRKRetry].Off();
            SW[SwRKOneCycle].Off();
            SW[SwRKCleanOut].Off();
            SW[SwRKTrayFeed].Off();
            SW[SwRKTrayEnd].Off();
            SW[SwRKAlarmReset].Off();
        }
        SW[SwFKReset].Off();
        SW[SwFKHome].Off();
        SW[SwFKStart].Off();
        SW[SwFKSkip].Off();
        SW[SwFKRetry].Off();
        SW[SwFKOneCycle].Off();
        SW[SwFKCleanOut].Off();
        SW[SwFKTrayFeed].Off();
        SW[SwFKTrayEnd].Off();
        SW[SwFKAlarmReset].Off();
    }
    if(iControlPanelMode==1)                                                    //Ken 20210702 AddPadInterface
    {
          SW[SwFrontActiveLed].OnOff(bFrontPadActive);
          SW[SwRearActiveLed].OnOff(!bFrontPadActive);                          //KenHsieh 20211221 : Front新增Enable燈號
    }
    else
    {
          SW[SwFrontActiveLed].OnOff(bFrontPadActive);
    }

    SW[SwRKManualStep].OnOff(bLampManualSetp);
    SW[SwRKManualTStart].OnOff(bLampManualStart);
}
//------------------------------------------------------------------------------
void DoSystemMessage()
{
    static int iMyCounter=0;                                                    //Steven 20110608 : 降低CPU負載

    if(iMyCounter==0)
    {
        ShowRunLed();                                                           //10%CPU
        ShowRunLabel();
    }
    else if(iMyCounter==3)
    {
        DoPanelLamp();                                                          //20%CPU
    }
    iMyCounter++;
    if(iMyCounter>=6)
        iMyCounter=0;
}
//------------------------------------------------------------------------------
TQPF_Timer SLK1Delay;                                                           //JerryYang 20160524
TQPF_Timer SLK2Delay;                                                           //JerryYang 20160524
int ScanPannelKey()
{
    static bool flag=true;
    static int Task=1;                                                          //JerryYang 20160524
    static int Task2=1;                                                         //JerryYang 20160524
    if(SystemInitialOK==false)
        return -1;

    bFrontPadActive=Sen[SnRearPadActive].IsOff();

    if(IsSafeLockCheck())
        return -1;

    if(bEnableEmployeeIDCheck==true)                                            //Ifor 20180927 (Steven) : Add 工號查驗時按鍵不可操作
        return -1;

    static bool bK[64];
    static bool bFlag=true;                                                     //JerryYang 20160524
    static bool bFlag2=true;                                                    //JerryYang 20160524
    static int  iRecordKey=0;                                                   //kevin 20171024 (wei) record key use double push
    int ret, p;
    if(flag)
    {
        for(int i=0; i<64; i++)
            bK[i]=false;
        flag=false;
    }
    ret=-1;
    if(bFrontPadActive)
    {
        if(IniConfig.bDisibleResetButton==false)                                //ChungHung 20111208
        {
            if(Sen[SnFKReset].IsOn() || bAseReset)
            {
                bAseReset=false;                                                //kevin 20150427
                ret=SnFKReset;
            }
        }
        if(Sen[SnFKPause].IsOn() || bAsePause)
        {
            bAsePause=false;
            ret=SnFKPause;
        }
        if(Sen[SnRKPause].IsOn() || bAsePause)
        {
            bAsePause=false;
            ret=SnFKPause;
        }
        if(Sen[SnFKHome].IsOn() || bAseHome)
        {
            if(bAseHome)
                iAseHome=2;                                                     //kevin 20150925
            else
                iAseHome=0;                                                     //kevin 20150925
            bAseHome=false;
            bPushHomeDetect=true;                                               //kevin 20171006
            ret=SnFKHome;
        }
        if(Sen[SnFKStart].IsOn() || bAseStart)                                  //Ifor 20160114 Auto Decay 搬移 至 fMain->ScanKey 判斷
        {
            bAseStart=false;                                                    //kevin 201050427
            ret=SnFKStart;
        }
        if(Sen[SnFKOneCycle].IsOn() || bAseOneCycle)
        {
            if(bAseOneCycle)
                iAseOneCycle=2;                                                 //kevin 20150925
            else
                iAseOneCycle=0;                                                 //kevin 20150925
            bAseOneCycle=false;
            ret=SnFKOneCycle;
        }
        if(Sen[SnFKPowerOff].IsOn())                                            //Alick 20160912 add for矽格北興要求紀錄按下Power Off按鈕
        {
            ret=SnFKPowerOff;
        }
        if(Sen[SnFKPowerOn].IsOn())                                             //kevin 20190328 要求紀錄按下Power On按鈕
        {
            ret=SnFKPowerOn;
        }

        if(fNote->fShow)                                                        //jou 2011-12-20 修正曾經發生過掉料的話,note訊息沒辦法操作的問題
        {
            if(fNote->IsTestSitICFallDown()==false)                             //Steven 20111030
            {
                if(Sen[SnFKRetry].IsOn() || bAseRetry)
                {
                    ret=SnFKRetry;                                              //kevin 2015042
                    bAseRetry=false;                                            //kevin 20150427
                }
            }
            else
            {
                if(Sen[SnFKRetry].IsOn() || bAseRetry)
                {
                    if(IniConfig.bD40IndexICFallDownMustPressFMotorDown==false)
                    {
                        ret=SnFKRetry;                                          //kevin 2015042
                        bAseRetry=false;                                        //kevin 20150427
                    }
                }
            }
        }
        else
        {
            if(Sen[SnFKRetry].IsOn() || bAseRetry)
            {
                ret=SnFKRetry;                                                  //kevin 2015042
                bAseRetry=false;                                                //kevin 20150427
            }
        }

        if(fNote->fShow)                                                        //jou 2011-12-20 修正曾經發生過掉料的話,note訊息沒辦法操作的問題
        {
            if(fNote->IsTestSitICFallDown()==false)
            {
                if(Sen[SnFKSkip].IsOn() || bAseSKIP)                            //kevin 20150427
                {
                    bAseSKIP=false;                                             //kevin 20150427
                    ret=SnFKSkip;
                }
            }
            else
            {
                if(Sen[SnFKSkip].IsOn() || bAseSKIP)                            //kevin 20150427
                {
                    if(IniConfig.bD40IndexICFallDownMustPressFMotorDown==false)
                    {
                        bAseSKIP=false;                                         //kevin 20150427
                        ret=SnFKSkip;
                    }
                }
            }
        }
        else
        {
            if(Sen[SnFKSkip].IsOn() || bAseSKIP)                                //kevin 20150427
            {
                bAseSKIP=false;                                                 //kevin 20150427
                ret=SnFKSkip;
            }
        }

        if(Sen[SnFKCleanOut].IsOn() || bAseCleanOut)                            //kevin 20150427
        {
            if(bAseCleanOut)                                                    //kevin 20150925
                iAseCleanOut=2;
            else
                iAseCleanOut=0;
            bAseCleanOut=false;
            ret=SnFKCleanOut;
        }
        if(Sen[SnFKTrayFeed].IsOn() || bAseTrayFeed)
        {
            if(bAseTrayFeed)                                                    //kevin 20150925
                iAseTrayFeed=2;
            else
                iAseTrayFeed=0;
            bAseTrayFeed=false;
            ret=SnFKTrayFeed;
        }
        if(Sen[SnFKTrayEnd].IsOn() || bAseTrayEnd)
        {
            if(bAseTrayEnd)
                iAseTrayEnd=2;                                                  //kevin 20150925
            else
                iAseTrayEnd=0;                                                  //kevin 20150925
            bAseTrayEnd=false;
            ret=SnFKTrayEnd;
            bManualTrayend=true;                                                //kevin 20171026 (wei) 手縱 tray end
            if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
               ArmSpeed[InArm].bAutoSKIP==true &&
               iRecordKey!=ret)                                                 //kevin 20171023 (Steven) auto tray end
            {
                bAutoTrayEndHasIC=false;                                        //kevin 20170906
                bNeedMusicAndAlarmOn=false;                                     //kevin 20170822 add alarm reset
                RecordProcess("OP Trigger_Manual_Trayend");
            }
        }

        if(Sen[SnRKAlarmReset].IsOn() || bAseAlarmReset)
        {
            bAseAlarmReset=false;
            ret=SnFKAlarmReset;
            bNeedMusicAndAlarmOn=false;                                         //kevin 20170817 (Steven) add
            bTesterPauseMusic=false;                                            //Steven 20220616 : Can select "Alarm Reset" when show "Tester Pause" for QORVO.
        }
        if(Sen[SnFKAlarmReset].IsOn() || bAseAlarmReset)
        {
            bAseAlarmReset=false;
            ret=SnFKAlarmReset;
            bNeedMusicAndAlarmOn=false;                                         //kevin 20170817 (Steven) add
            bTesterPauseMusic=false;                                            //Steven 20220616 : Can select "Alarm Reset" when show "Tester Pause" for QORVO.
        }
    }
    else
    {
        if(IniConfig.bDisibleResetButton==false)                                //ChungHung 20111208
        {
            if(Sen[SnRKReset].IsOn() || bAseReset)
            {
                bAseReset=false;                                                //kevin 20150427
                ret=SnFKReset;
            }
        }
        if(Sen[SnRKPause].IsOn() || bAsePause)
        {
            bAsePause=false;
            ret=SnFKPause;
        }
        if(Sen[SnFKPause].IsOn() || bAsePause)
        {
            bAsePause=false;
            ret=SnFKPause;
        }
        if(Sen[SnRKHome].IsOn()||bAseHome)
        {
            if(bAseHome)
                iAseHome=1;                                                     //kevin 20150925
            else
                iAseHome=0;                                                     //kevin 20150925
            bAseHome=false;
            bPushHomeDetect=true;                                               //kevin 20171006
            ret=SnFKHome;
        }
        if(Sen[SnRKStart].IsOn() || bAseStart)                                  //Ifor 20160114 Auto Decay 搬移 至 fMain->ScanKey 判斷
        {
            bAseStart=false;                                                    //kevin 201050427
            ret=SnFKStart;
        }
        if(Sen[SnRKOneCycle].IsOn() || bAseOneCycle)
        {
            if(bAseOneCycle)
                iAseOneCycle=2;                                                 //kevin 20150925
            else
                iAseOneCycle=0;                                                 //kevin 20150925
            bAseOneCycle=false;
            ret=SnFKOneCycle;
        }
        if(Sen[SnRKPowerOff].IsOn())                                            //Alick 20160912 add for矽格北興要求紀錄按下Power Off按鈕
        {
            ret=SnRKPowerOff;
        }
        if(Sen[SnRKPowerOn].IsOn())                                             //kevin 20190328 要求紀錄按下Power On按鈕
        {
            ret=SnRKPowerOn;
        }
        if(fNote->fShow)                                                        //jou 2011-12-20 修正曾經發生過掉料的話,note訊息沒辦法操作的問題
        {
            if(fNote->IsTestSitICFallDown()==false)                             //Steven 20111030   //jou 2011-12-23 解除不是Index alarm也需要按Z1的問題
            {
                if(Sen[SnRKRetry].IsOn() || bAseRetry)
                {
                    ret=SnFKRetry;                                              //kevin 2015042
                    bAseRetry=false;                                            //kevin 20150427
                }
            }
            else
            {
                if(Sen[SnRKRetry].IsOn() || bAseRetry)
                {
                    if(IniConfig.bD40IndexICFallDownMustPressFMotorDown==false)
                    {
                        ret=SnFKRetry;                                          //kevin 2015042
                        bAseRetry=false;                                        //kevin 20150427
                    }
                }
            }
        }
        else
        {
            if(Sen[SnRKRetry].IsOn() || bAseRetry)                              // ret=SnFKRetry;
            {
                ret=SnFKRetry;                                                  //kevin 2015042
                bAseRetry=false;                                                //kevin 20150427
            }
        }

        if(fNote->fShow)                                                        //jou 2011-12-20 修正曾經發生過掉料的話,note訊息沒辦法操作的問題
        {
            if(fNote->IsTestSitICFallDown()==false)                             //jou 2011-12-23 解除不是Index alarm也需要按Z1的問題
            {
                if(Sen[SnRKSkip].IsOn() || bAseSKIP)
                {
                    bAseSKIP=false;                                             //kevin 2015042
                    ret=SnFKSkip;                                               //kevin 2015042
                }
            }
            else
            {
                if(Sen[SnRKSkip].IsOn() || bAseSKIP)
                {
                    if(IniConfig.bD40IndexICFallDownMustPressFMotorDown==false)
                    {
                         bAseSKIP=false;                                        //kevin 2015042
                         ret=SnFKSkip;                                          //kevin 20150429
                    }
                }
            }
        }
        else
        {
            if(Sen[SnRKSkip].IsOn() || bAseSKIP)
            {
                bAseSKIP=false;                                                 //kevin 2015042
                ret=SnFKSkip;                                                   //kevin 20150429
            }
        }

        if(Sen[SnRKCleanOut].IsOn() || bAseCleanOut)
        {
            if(bAseCleanOut)                                                    //kevin 20150925
                iAseCleanOut=2;
            else
                iAseCleanOut=0;
            bAseCleanOut=false;
            ret=SnFKCleanOut;
        }
        if(Sen[SnRKTrayFeed].IsOn() || bAseTrayFeed)
        {
            if(bAseTrayFeed)                                                    //kevin 20150925
                iAseTrayFeed=2;
            else
                iAseTrayFeed=0;

            bAseTrayFeed=false;
            ret=SnFKTrayFeed;
        }
        if(Sen[SnRKTrayEnd].IsOn() || bAseTrayEnd)
        {
            if(bAseTrayEnd)
                iAseTrayEnd=2;                                                  //kevin 20150925
            else
                iAseTrayEnd=0;                                                  //kevin 20150925
            bAseTrayEnd=false;
            ret=SnFKTrayEnd;
            bManualTrayend=true;                                                //kevin 20171026 (wei) 手縱 tray end
            if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
               ArmSpeed[InArm].bAutoSKIP==true &&
               iRecordKey!=ret)                                                 //kevin 20171023 (Steven) auto tray end
            {
                bAutoTrayEndHasIC=false;                                        //kevin 20170906
                bNeedMusicAndAlarmOn=false;                                     //kevin 20170822 add alarm reset
                RecordProcess("OP Trigger_Manual_Trayend");
            }
        }
        if(Sen[SnRKAlarmReset].IsOn() || bAseAlarmReset)
        {
            bAseAlarmReset=false;
            ret=SnFKAlarmReset;
            bNeedMusicAndAlarmOn=false;                                         //kevin 20170817 (Steven) add
        }
        if(Sen[SnFKAlarmReset].IsOn() || bAseAlarmReset)
        {
            bAseAlarmReset=false;
            ret=SnFKAlarmReset;
            bNeedMusicAndAlarmOn=false;                                         //kevin 20170817 (Steven) add
        }
    }
    if(INSTALL_SOCKET_CLAMP)                                                    //JerryYang 20160601 如果機台硬體支援分離測試才進來
    {
        if(Sen[SnHeaterDoor].IsOff()==true &&
           Sen[SnHeaterDoor2].IsOff()==true &&
           SystemStart==false)                                                  //JerryYang 20160601
        {
            if(Sen[SnSLK1UnclampButton].IsOn() && bFlag)                        //JerryYang 20160601 偵測到Arm1 SLK夾持汽缸伸出/縮回按鈕被按下
            {
                bFlag=false;
                switch(Task)
                {
                    case 1:
                        {
                            SLK1Delay.SetSecAndOn(3);                           //JerryYang 20160601 每三秒才能控制一次ARM1 SLK夾持汽缸伸出/縮回
                            if(Cylinder[C_SLK1_Clamp].OnSensor()==true ||
                               Cylinder[C_SLK1_Unclamp].OnSensor()==false)
                            {
                                Cylinder[C_SLK1_Clamp].Off();
                                Cylinder[C_SLK1_Unclamp].On();
                            }
                            else if(Cylinder[C_SLK1_Unclamp].OnSensor()==true)
                            {
                                Cylinder[C_SLK1_Unclamp].Off();
                                Cylinder[C_SLK1_Clamp].On();
                            }
                            Task=10;
                        }
                        break;
                    case 10:
                        if(SLK1Delay.Off())
                        {
                            Task=1;
                        }
                        break;
                }
            }
            else
            {
                bFlag=true;
            }
            if(Sen[SnSLK2UnclampButton].IsOn() && bFlag2)                       //JerryYang 20160601 偵測到Arm2 SLK夾持汽缸伸出/縮回按鈕被按下
            {
                bFlag2=false;
                switch(Task2)
                {
                    case 1:
                        {
                            SLK2Delay.SetSecAndOn(3);                           //JerryYang 20160601 每三秒才能控制一次ARM2 SLK夾持汽缸伸出/縮回
                            if(Cylinder[C_SLK2_Clamp].OnSensor()==true ||
                               Cylinder[C_SLK2_Unclamp].OnSensor()==false)
                            {
                                Cylinder[C_SLK2_Clamp].Off();
                                Cylinder[C_SLK2_Unclamp].On();
                            }
                            else if(Cylinder[C_SLK2_Unclamp].OnSensor()==true)
                            {
                                Cylinder[C_SLK2_Unclamp].Off();
                                Cylinder[C_SLK2_Clamp].On();
                            }
                            Task2=10;
                        }
                        break;
                    case 10:
                        if(SLK2Delay.Off())
                        {
                            Task2=1;
                        }
                        break;
                }
            }
            else
            {
                bFlag2=true;
            }
        }
    }
    iRecordKey=ret;                                                             //kevin 20171024 (wei) record key use double push
    if(ret!=-1)
    {
        p=ret;
        if(bFrontPadActive==false)
            p+=SnRKPowerOff;

        if(bK[p]==false)
        {
            bK[p]=true;
            return ret;
        }
        else
        {
            ret=-1;
        }
    }
    for(int i=0; i<32; i++)
    {
        if(bK[i])
        {
            if(Sen[i].IsOn()==false)
            {
                bK[i]=false;
            }
        }
    }
    return ret;
}
//------------------------------------------------------------------------------
int DoScanLevelSensor()                                                         //Steven 20190503 : 指紋辨識權限
{
    int iRet=-1;
    bool bFlag=false;

    if(bFlag==true || (Sen[SnFPLevelOpe].Enable && Sen[SnFPLevelOpe].IsOn()))
    {
        iRet=0;
    }
    else if(bFlag==true || (Sen[SnFPLevelEng].Enable && Sen[SnFPLevelEng].IsOn()))
    {
        iRet=1;
    }
    else if(bFlag==true || (Sen[SnFPLevelSup].Enable && Sen[SnFPLevelSup].IsOn()))
    {
        iRet=2;
    }
    else if(bFlag==true || (Sen[SnFPLevelHon].Enable && Sen[SnFPLevelHon].IsOn()))
    {
        iRet=3;
    }
    return iRet;
}
//------------------------------------------------------------------------------
void GetMotorAlarmCode(TComponent *Comp)
{
    static int iCount=0;
    int i, iRef;
    AnsiString str1;

    for(i=0; i<TOTAL_MOTOR; i++)
    {
        if(MOT[i].Motor==Comp && MOT[i].Motor->Enable)     //pig 2016.01.30 MotorAlarm
        {
            MOT[i].ScanMotorStatus();
            iRef=MOT[i].GetErrorIndex();

            if(CUSTOMER_CODE==CC_JCET)  //Steven 20200312
            {
                if(i==0)
                {
                    if(iRef==3)
                    {
                        str1.sprintf("In Arm X CCW alarm, iAlarmLed:%d, iCcwLed%d, Pos:%d",  MOT[i].Led[iAlarmLed]?1:0, MOT[i].Led[iCcwLed]?1:0, MOT[i].ReadEncoderPos());
                        MyDBIProcessNew("Motion", "WAR240004", str1, "GetMotorAlarmCode");
                        MOT[i].Led[iAlarmLed]=false;
                        MOT[i].Led[iCcwLed]=false;
                        MOT[i].Led[iSoftccwLed]=false;
                    }
                    else
                    {
                        iCount=0;
                    }
                }
                else
                {
                    iCount=0;
                }
            }

            if(iRef==9)
                iRef=7;
            JamCode=MotorIndexToJamCode(i);
            ShowMotorErrorMessage(JamCode, iRef+1);
        }
        if(INDEX_MOTION_CARD==0 && (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2) && MOT[i].Gali_MotorAlarm && MotorPowerOnDelay==0)    //Steven 20210623 : Index使用Galil
        {
            SoftStop=true;
            MOT[i].MovFlag=false;
            MOT[MTestY1].bScanFlag=false;
            MOT[MTestZ1].bScanFlag=false;
            MOT[MTestZ2].bScanFlag=false;
            MOT[MTestY2].bScanFlag=false;
            MOT[MTestY1].GaliSofDelayCount=0;
            MOT[MTestZ1].GaliSofDelayCount=0;
            MOT[MTestZ2].GaliSofDelayCount=0;
            MOT[MTestY2].GaliSofDelayCount=0;
            MOT[i].Gali_ScanMotStatus();
            iRef = MOT[i].GetErrorIndex();
            if(iRef==9)
                iRef=7;
            JamCode = MotorIndexToJamCode(i);
            ShowMotorErrorMessage(JamCode, iRef+1);
        }
    }
    //return 0;
}
//==============================================================================
// only process Motor and Cylinder error
//==============================================================================
#define     ALM_MOTOR_MOVE  55555
//氣缸 Error is 自動加 10000
void ProcessAlarm()
{
    TComponent *Comp;
    int iCode;
    int Pos;
    AnsiString sRef;
    while(PopUpAlarm(&Comp, iCode))
    {
        SoftStop=false;
        SoftStart=false;
        ScanSystemSensor();
        if(iCode==ALM_MOTOR_MOVE)
        {
            GetMotorAlarmCode(Comp);
            fAllMotorHome=false;        //發生馬達錯誤
            //Ifor 20151208 :矽品新增馬達異常 Show Alarm Message
            if(IniConfig.bSPILFunction==true || CUSTOMER_CODE==CC_SIGURD_PeiXing || CUSTOMER_CODE==CC_ASE_KaohSiung)//kevin 20180319 add //JerryYang 20160328 for 矽格北興,觸發回home的地方補上log  //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                ShowMyMessage("Motor Move error, Need home"); //Ifor 20151208 :Add Motor Home Massage
        }
        else
        {
            sRef = CylinderIndexToJamCode(iCode, &Pos);
            ShowErrorMessage(sRef, K_RETRY, Pos);
        }
    }
    ClearAllAlarm();
}
//------------------------------------------------------------------------------
AnsiString InitialTestDelayStatus()     //wei 20171020 (jou) 延遲狀態顯示
{
    AnsiString str="";
    if(bInitialTestDelayStatus[11])     //Steven 20221214 : Add 主畫面顯示
    {
        str="Initial delay--By KL";
    }
    if(bInitialTestDelayStatus[6])
    {
        str="Initial delay--Press stop time over";
    }
    if(bInitialTestDelayStatus[3])
    {
        str="Initial delay--Auto clean";
    }
    if(bInitialTestDelayStatus[2])
    {
        str="Initial delay--Show alarm message";
    }
    if(bInitialTestDelayStatus[4])
    {
        str="Initial delay--Open heat door";
    }
    if(bInitialTestDelayStatus[5])
    {
        str="Initial delay--Happen tested time blow";
    }
    if(bInitialTestDelayStatus[1])
    {
        str="Initial delay--Fisrt device delay";
    }
    if(bInitialTestDelayStatus[7])
    {
        str="Initial delay--No fullSite";
    }
    if(bInitialTestDelayStatus[9])
    {
        str="Initial delay--OTD unlock";
    }
    if(bInitialTestDelayStatus[8])
    {
        str="EOT monitor time over delay";
    }
    if(bInitialTestDelayStatus[10])       //kevin 20181101 add SOT
    {
        str="SOT monitor time over delay";
    }
    return str;
}
