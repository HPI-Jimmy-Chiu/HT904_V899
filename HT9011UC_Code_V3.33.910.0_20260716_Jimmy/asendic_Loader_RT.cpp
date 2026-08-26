#include "MachineDefine.h"
#pragma hdrstop

#include "asendic_Loader_RT.h"

//---------------------------------------------------------------------------
#include "asendic.h"
#include "MyMotor.h"
#include "mycylin.h"
#include "main.h"
#include "mymessbox.h"
#include "note.h"
#include "mysensor.h"
#include "myswitch.h"
#include "acatchtray.h"
#include "cprod.h"
#include "OCR.h"

#include "OCRInsp.h"
#include "cMyDB.h"
#include "uHGemHT9045.h"
#include "cmydef.h"
#include "SCK_ART.h"
#include "cpublic.h"
#include "common.h"

#pragma package(smart_init)

int iAutoLoaderReceiveTask=1;
int iLoadNewLoaderTrayToCarTask=1;
TQPF_Timer LoaderDelay;
//---------------------------------------------------------------------------
void InitLoadNewLoaderTrayToCarTask()
{
    iLoadNewLoaderTrayToCarTask=1;
}
//---------------------------------------------------------------------------
bool DoLoadNewLoaderTrayToCar()
{
#ifndef SOFT_SIMULTE
    int ret;
#endif
    int &Task=iLoadNewLoaderTrayToCarTask;
    switch(Task)
    {
        case 1:
            if(MOT[MMTrayZ].fHasTray)
            {
                LoaderDelay.SetMSAndOn(1000);                                                                           //Steven 20130904 : 加入Time Out 避免Hang Up
                Task=400;
                break;
            }

            if(((bForKyecBu3RunART==false && Sen[SnLoaderTrayHasTray_ART].IsOff()==false) ||                            //wei 20160113 ART增加一顆sensor  //wei 20161118 bRunART-->bForKyecBu3RunART
                (bForKyecBu3RunART && Sen[SnLoaderTrayHasTray_ART].IsOff()==false))       ||
                 LastSet.iRealDummy==DUMMY)
            {
                if(Sen[SenLoaderCCWDete].IsOn()   ||
                   Sen[SnLoaderCarHasTray].IsOn())
                {
                    MOT[MMTrayY_Car].SetTray(NULL_IC, __FUNC__);
                    MOT[MMTrayZ].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }
                Task=50;
            }
            else
            {
                if(MOT[MMTrayY_Car].fHasTray==false)                                                                    //jou 2011-12-21 有發生Tray feed完,汽缸還在上面,所以做完下降
                {
                    if(Cylinder[C_Load_Middle].OffStatus()==false)                                                      //汽缸在上面
                    {
                        if(Sen[SnLoaderCarHasTray].IsOff() &&
                           Sen[SenLoaderCCWDete].IsOff())
                        {
                            Cylinder[C_Load_Up].Off();
                            Cylinder[C_Load_Middle].Off();
                            break;
                        }
                    }
                }
                ShowErrorMessage("MES0921", K_RETRY, MMTrayY_Car);
            }
            break;
        case 50:
            if(CylinderUp(C_Load_Up))
            {
                MOT[MMTrayZ].ClearTray(__FUNC__);
                Task=100;
            }
            break;
        case 100:
            if(LastSet.iRealDummy==DUMMY)                                       //Steven 20100618
                Task=200;
            else if(Cylinder[C_TrayZ_Selector].Push())
                Task=200;
            break;
        case 200:
            if(CylinderMiddle(C_Load_Up))
            {
                if(bLoaderPause)
                {
                    bLoaderPause=false;
                    ShowMyMessage("Loader split wait", "Loader 分離暫停", "DoLoadNewLoaderTrayToCar 200");
                    Task=200;
                    break;
                }
                Task=300;
            }
            break;
        case 300:
            if(LastSet.iRealDummy==DUMMY)                                       //Steven 20111212
            {
                Task=400;
            }
            else if(Cylinder[C_TrayZ_Selector].Pop())
            {
                Task=400;
            }

            LoaderDelay.SetMSAndOn(1000);                                       //Steven 20130904 Start: 加入Time Out 避免Hang Up
            break;
        case 400:
            if(LoaderDelay.Off())
            {
                Task=410;
            }
            break;
        case 410:
            if(CylinderLower(C_Load_Up))
            {
                LoaderDelay.SetMSAndOn(1000);
                Task=420;
            }
            break;
        case 420:
            if(bHandlerPause)                                                   //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
                LoaderDelay.SetMSAndOn(1000);

#ifndef SOFT_SIMULTE
            if((Sen[SenLoaderCCWDete].IsOn()     ||
                Sen[SnLoaderCarHasTray].IsOn()) ||
               LastSet.iRealDummy==DUMMY)
#endif
            {
                MOT[MMTrayY_Car].SetTray(NULL_IC, __FUNC__);
                MOT[MMTrayZ].ClearTray(__FUNC__);
                Task=1;
                return true;
            }
#ifndef SOFT_SIMULTE
            else if(LoaderDelay.Off())
            {
                ret=ShowErrorMessage("JAM0913", K_RETRY|K_SKIP, MMTrayY_Car);   //Steven 20150108 : JAM1013 --> JAM0913
                if(ret==K_SKIP)
                {
                    MOT[MMTrayZ].ClearTray(__FUNC__);
                    Task=1;
                }
            }
            break;
#endif
    }
    return false;
}
//------------------------------------------------------------------------------
int iLoaderTrayToFrontTask=1;
TQPF_Timer hLoaderTrayToFront;
TQPF_Timer hLoaderTrayToFrontForDummy;
TQPF_Timer htRTDetectTrayRetryDelay;                                            //Sam 20200803 : Alrm retry timeout
void InitLoaderTrayToFrontTask()
{
    iLoaderTrayToFrontTask=1;
}
//------------------------------------------------------------------------------
bool DoLoaderTrayToFront()
{
    int &Task=iLoaderTrayToFrontTask;
    int ret;

    if(Sen[SnLoaderTrackDetect].Enable)                                         //Sam 20200316 : Loader Detect Tray
    {
       if(TrayMoveStatus(0, __FUNC__)==0)                                       //Sam 20240108 : 整合 Loader 跳料檢查。
        {
            hLoaderTrayToFront.SetMSAndOn(20000);
            hLoaderTrayToFrontForDummy.SetMSAndOn(5000);
        }

        if(CheckLoaderICFloating(1)==false)                                     //Sam 20240108 : 整合 Loader 跳料檢查。
            return false;
    }

    static bool bLoaderHasDuplicateError=false;                                 //Steven 20120220 : Loader Tray重複Alarm Flag
    switch(Task)
    {
        case 1:
            hLoaderTrayToFront.SetMSAndOn(20000);
            hLoaderTrayToFrontForDummy.SetMSAndOn(5000);
            if(INSTALL_OCR_YMot==eocrYMotInstal)
            {
                if(Cylinder[C_TrayY_Fixer].Enable)
                    Cylinder[C_TrayY_Fixer].On();
                if(Cylinder[C_LoaderEdgePush].Enable)
                    Cylinder[C_LoaderEdgePush].On();
            }
            Task=100;
        case 100:
            if(bHandlerPause)                                                                                           //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                Task=1;
                break;
            }

            if(TRAY_ARM_MODE==eUnderCoveyor)
            {
                Cylinder[C_LoaderPushBack_Back].Off();
                Cylinder[C_LoaderPushBack_Push].On();
                Cylinder[C_LoaderEdgePush].Off();                                                                       //Sam 20200316 : Loader Detect Tray
            }
            else if(INSTALL_OCR_YMot==eocrYMotInstal)
            {
                if(Cylinder[C_TrayY_Fixer].Enable)
                    Cylinder[C_TrayY_Fixer].On();
                if(Cylinder[C_LoaderEdgePush].Enable)
                    Cylinder[C_LoaderEdgePush].On();
            }
            else
            {
                Cylinder[C_TrayY_Fixer].Off();
                Cylinder[C_LoaderEdgePush].Off();                                                                       //Sam 20200316 : Loader Detect Tray
            }

            if(TrayMoveStatus(0, __FUNC__)==0)                                                                          //Sam 20240108 : 整合 Loader 跳料檢查。
            {
                hLoaderTrayToFront.SetMSAndOn(20000);
                hLoaderTrayToFrontForDummy.SetMSAndOn(5000);
            }

            TrayMoveOut(true, 0, Prod.iMLoaderYCarPos);
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                                                         //kevin 20150610  收tray
            {
                if(Sen[SenLoaderCCWDete].IsOn() &&
                   Sen[SnLoaderCarHasTray].IsOn())                                                                      //kevin 20170809 (Steven) 收tray
                {
                    TrayMoveOut(false, 0);
                    MOT[MMTrayY_Car].SetTray(NULL_IC, __FUNC__);
                    MOT[MMTrayY].ClearTray(__FUNC__);
                    bLoaderHasDuplicateError=false;                                                                     //Steven 20120220 : Loader Tray重複Alarm Flag
                    Task=1;
                    if(INSTALL_OCR_YMot==eocrYMotInstal)
                    {
                        if(Cylinder[C_TrayY_Fixer].Enable)
                            Cylinder[C_TrayY_Fixer].Off();
                        if(Cylinder[C_LoaderEdgePush].Enable)
                            Cylinder[C_LoaderEdgePush].Off();
                    }
                    return true;
                }
            }
            else
            {
                if(Sen[SnLoaderCarHasTray].IsOff()==false)
                {                                                                                                       //jou 2015-10-02 Auto Retest GPIB mode
                    hLoaderTrayToFront.SetSecAndOn(1+Ld_UldDelayTime.ULD_TrayArrivalWaitDelay);                         //JerryYang 20220324 : 後退到位時delay time比照Unloader
                    Task=200;
                    break;
                }
            }

            if(hLoaderTrayToFrontForDummy.Off())
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    TrayMoveOut(false, 0);
                    MOT[MMTrayY_Car].SetTray(NULL_IC, __FUNC__);
                    MOT[MMTrayY].ClearTray(__FUNC__);
                    bLoaderHasDuplicateError=false;                                                                     //Steven 20120220 : Loader Tray重複Alarm Flag
                    Task=1;
                    if(INSTALL_OCR_YMot==eocrYMotInstal)
                    {
                        if(Cylinder[C_TrayY_Fixer].Enable)
                            Cylinder[C_TrayY_Fixer].Off();
                        if(Cylinder[C_LoaderEdgePush].Enable)
                            Cylinder[C_LoaderEdgePush].Off();
                    }
                    return true;
                }
            }

            if(hLoaderTrayToFront.Off())
            {
                ret=ShowErrorMessage("JAM1012", K_SKIP|K_RETRY, MMTrayY, bLoaderHasDuplicateError, __FUNC__);           //Steven 20120925 : JAM1009 -> JAM1012
                if(ret==K_RETRY)
                {
                    bLoaderHasDuplicateError=true;                                                                      //Steven 20120220 : Loader Tray重複Alarm Flag
                    Task=1;
                    break;                                                                                              //Sam 20240108 : 整合 Loader 跳料檢查。
                }
                else
                {
                    bLoaderHasDuplicateError=false;                                                                     //Steven 20120220 : Loader Tray重複Alarm Flag
                    MOT[MMTrayY].ClearTray(__FUNC__);
                    Task=1;
                    if(INSTALL_OCR_YMot==eocrYMotInstal)
                    {
                        if(Cylinder[C_TrayY_Fixer].Enable)
                            Cylinder[C_TrayY_Fixer].Off();
                        if(Cylinder[C_LoaderEdgePush].Enable)
                            Cylinder[C_LoaderEdgePush].Off();
                    }
                    return true;
                }
            }
        case 200:                                                               //jou 2015-10-02 Auto Retest GPIB mode
            if(hLoaderTrayToFront.Off())
            {
                MOT[MMTrayY_Car].SetTray(NULL_IC, __FUNC__);
                MOT[MMTrayY].ClearTray(__FUNC__);
                bLoaderHasDuplicateError=false;                                 //Steven 20120220 : Loader Tray重複Alarm Flag
                Task=1;
                if(INSTALL_OCR_YMot==eocrYMotInstal)
                {
                    if(Cylinder[C_TrayY_Fixer].Enable)
                        Cylinder[C_TrayY_Fixer].Off();
                    if(Cylinder[C_LoaderEdgePush].Enable)
                        Cylinder[C_LoaderEdgePush].Off();
                }
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iLoaderTrayToRearTask=1;
TQPF_Timer hLoaderTrayToRear;
TQPF_Timer hLoaderTrayToRearForDummy;
void InitLoaderTrayToRearTask()
{
    iLoaderTrayToRearTask=1;
}
//------------------------------------------------------------------------------
//---- 收空TRAY----
bool DoLoaderTrayToRear()
{
    static bool bLoaderHasDuplicateError=false;                                 //Steven 20120220 : Loader Tray重複Alarm Flag

    int &Task   =iLoaderTrayToRearTask;
    int ret     =0;
    bool bFlag  =false;

    switch(Task)
    {
        case 1:
            hLoaderTrayToRearForDummy.SetMSAndOn(5000);
            hLoaderTrayToRear.SetMSAndOn(20000);
            Task=100;
        case 100:
            if(Cylinder[C_TrayY_Fixer].Pop())
            {
                Task=200;
            }
            break;
        case 200:
            if(bHandlerPause)                                                                                                                                   //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                Task=1;
                break;
            }

            if(MOT[MMTrayY].fHasTray && MOT[MMTrayY_Car].fHasTray)                                                                                              //Steven 20130917 : Double Tray問題,測試 Q4
            {
                Task=1;
                return true;
            }

            TrayMoveIn(true, 0, Prod.iMLoaderYSurePos);                         //RogerYang 20260401 : 修正RT時台車版本往前移動問題
            if(Sen[SnLoaderSureTray].IsOff()==false)
            {
                Task=450;
                break;
            }

            if(hLoaderTrayToRearForDummy.Off())
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    Task=450;
                    break;
                }
            }

            if(hLoaderTrayToRear.Off())
            {
                ret=ShowErrorMessage("JAM1001", K_SKIP|K_RETRY, MMTrayY_Car, bLoaderHasDuplicateError, "DoLoaderTrayToRear_200");                               //Steven 20120925 : JAM1009 -> JAM1001
                if(ret==K_RETRY)
                {
                    bLoaderHasDuplicateError=true;                                                                                                              //Steven 20120220 : Loader Tray重複Alarm Flag
                    Task=1;
                }
                else
                {
                    bLoaderHasDuplicateError=false;                                                                                                             //Steven 20120220 : Loader Tray重複Alarm Flag
                    MOT[MMTrayY_Car].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }
            }
            break;
        case 300:
            if(Cylinder[C_TrayY_Fixer].Push())
                Task=400;
            break;
        case 400:
            #ifndef SOFT_SIMULTE
                bFlag=(Sen[SnLoaderFixCyPush].IsOn() ||
                       Cylinder[C_TrayY_Fixer].OffSensor());                                                                                                    //wei 20241011 新增後勾氣缸off sensor,避免氣缸沒作動
            #else
                bFlag=false;
            #endif

            if(bFlag)
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    bLoaderHasDuplicateError=false;                                                                                                             //Steven 20120220 : Loader Tray重複Alarm Flag
                    Task=450;
                }
                else
                {
                    Cylinder[C_TrayY_Fixer].Off();
                    ret=ShowErrorMessage("JAM1003", K_RETRY, MMTrayY, bLoaderHasDuplicateError, "DoLoaderTrayToRear_400");                                      //Steven 20120925 : JAM1009 -> JAM1003
                    bLoaderHasDuplicateError=false;                                                                                                             //Steven 20120220 : Loader Tray重複Alarm Flag
                    Task=300;
                    break;
                }
            }
            else
            {
                bLoaderHasDuplicateError=false;                                                                                                                 //Steven 20120220 : Loader Tray重複Alarm Flag
                Task=500;
            }
            break;
        case 450:
            hLoaderTrayToRear.SetSecAndOn(Ld_UldDelayTime.LD_TrayArrivalDely);
            Task=460;
            break;
        case 460:
            if(hLoaderTrayToRear.Off())
            {
                TrayMoveOut(false, 0);
                Task=500;
            }
            break;
        case 500:
            if(Cylinder[C_TrayY_Fixer].Push())
            {
                MOT[MMTrayY].SetTray(NULL_IC, __FUNC__);
                MOT[MMTrayY_Car].ClearTray(__FUNC__);                           //Steven 20111109 : 已入Tray卻未清掉資料
                Task=1;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iAutoLoaderTask=1;
TQPF_Timer DoAutoLoaderDelay;
TQPF_Timer CheckLoaderTrayDelay;
void InitAutoLoaderTask()
{
    iAutoLoaderTask=1;
}
//------------------------------------------------------------------------------
void DoAutoLoader()
{
    static bool bLoaderHasDuplicateError=false;                                 //Steven 20120220 : Loader Tray重複Alarm Flag
    if(iReceiveLoaderTray!=0)
    {
        DoAutoLoaderReceive();
        return;
    }

    int &Task   =iAutoLoaderTask;
    int iTray   =0;
    bool bFlag  =false;

    if(fLoaderCanSupplyNewTray==false)
        return;

    switch(Task)
    {
        case 1:
            if(MOT[MMTrayY].fHasTray || Sen[SnLoaderSureTray].IsOn())
            {
                Task=20;
                break;
            }
            else
            {
                MOT[MMTrayY].ClearTray(__FUNC__);
            }

            if(MOT[MMTrayY].fHasTray==false)
            {
                Task=100;

                iTray=WhichAutoNeedTray();                                      //which auto need tray
                if(MOT[MMTrayY].fHasTray &&
                   MOT[MMTrayY].HasIC()==false &&
                   iTray==0)
                {
                    Task=1;
                    break;
                }
            }
            break;
        case 20:
            Cylinder[C_TrayY_Fixer].Off();
            Task=30;
            break;
        case 30:
            if(Cylinder[C_TrayY_Fixer].Pop())
                Task=40;
            break;
        case 40:
            #ifndef SOFT_SIMULTE
                bFlag=(Sen[SnLoaderFixCyPush].IsOn() ||
                       Cylinder[C_TrayY_Fixer].OffSensor());                                                            //wei 20241011 新增後勾氣缸off sensor,避免氣缸沒作動
            #else
                bFlag=false;
            #endif

            if(bFlag)
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    bLoaderHasDuplicateError=false;                                                                     //Steven 20120220 : Loader Tray重複Alarm Flag
                    Task=50;
                }
                else
                {
                    Cylinder[C_TrayY_Fixer].Off();
                    ShowErrorMessage("JAM1003", K_RETRY, MMTrayY, bLoaderHasDuplicateError, __FUNC__);                  //Steven 20120925 : JAM1009 -> JAM1003
                    bLoaderHasDuplicateError=true;                                                                      //Steven 20120220 : Loader Tray重複Alarm Flag
                    Task=1;
                }
            }
            else
            {
                bLoaderHasDuplicateError=false;                                                                         //Steven 20120220 : Loader Tray重複Alarm Flag
                Task=50;
            }
            break;
        case 50:
            if(Cylinder[C_TrayY_Fixer].Pop())
            {
                #ifdef SOFT_SIMULTE
                MOT[MMTrayY_Car].ClearTray(__FUNC__);
                MOT[MMTrayY].SetTray(NULL_IC, __FUNC__);
                #endif
                Task=60;
            }
            break;
        case 60:
            if(MOT[MMTrayY].fHasTray==false)
            {
                DoAutoLoaderDelay.SetMSAndOn(1000);                             //Question ASE_G
                Task=70;
            }
            else
            {
                CheckLoaderTrayDelay.SetMSAndOn(100);
                Task=65;
            }
            break;
        case 65:
            if(CheckLoaderTrayDelay.Off())
            {
                if(MOT[MMTrayY].fHasTray==true && LastSet.iRealDummy>=HAS_TRAY &&
                   Sen[SnLoaderSureTray].IsOff() && Sen[SnLoaderPreDete].IsOff())
                {
                    MOT[MMTrayY].ClearTray(__FUNC__);
                }
                Task=60;

                if(MOT[MMTrayY].fHasTray==false         &&
                   LastSet.iRealDummy>=HAS_TRAY         &&
                   Sen[SnLoaderSureTray].IsOff()==false &&
                   Sen[SnLoaderPreDete].IsOff()==false)                         //Steven 20130930
                {
                    MOT[MMTrayY].SetTray(NULL_IC, __FUNC__);
                }

                if(MOT[MMTrayY_Car].fHasTray==false       &&
                   LastSet.iRealDummy>=HAS_TRAY           &&
                   Sen[SnLoaderCarHasTray].IsOff()==false &&
                   Sen[SenLoaderCCWDete].IsOff()==false)                        //Steven 20130930
                {
                    MOT[MMTrayY_Car].SetTray(NULL_IC, __FUNC__);
                }

                if(MOT[MMTrayY].fHasTray && MOT[MMTrayY].HasIC()==false &&
                    MOT[MMTrayY].fHasTray && MOT[MMTrayY_Car].fHasTray)
                {
                    iReceiveLoaderTray=2;
                    iAutoLoaderReceiveTask=200;
                }

                if(MOT[MMTrayY].fHasTray && MOT[MMTrayY_Car].fHasTray)          //Steven 20130917 : Double Tray問題,測試 Q4
                {
                    iReceiveLoaderTray=2;
                    iAutoLoaderReceiveTask=200;
                }
            }
            break;
        case 70:
            if(DoAutoLoaderDelay.Off())
            {
                if((Sen[SnLoaderSureTray].IsOn() && MOT[MMTrayY].fHasTray==false) ||
                   (LastSet.iRealDummy==DUMMY && MOT[MMTrayY].fHasTray==false))
                {
                    MOT[MMTrayY_Car].ClearTray(__FUNC__);
                    MOT[MMTrayY].SetTray(NULL_IC, __FUNC__);
                }
                Task=100;
            }
            break;
        case 100:
            if(MOT[MMTrayY_Car].fHasTray)
            {
                if((MOT[MTrayX].fHasTray && MOT[MTrayX].ReadPos()==Prod.iXTrayEmpty))
                {
                    DoAutoLoaderDelay.SetMSAndOn(2000);
                    Task=150;
                }
                else
                {
                    InitLoaderTrayToRearTask();
                    Task=300;
                }
            }
            else
            {
                InitLoadNewLoaderTrayToCarTask();
                Task=200;
                if(iReceiveLoaderTray!=0 ||                                     //jou 980421 Loader Tray no need relay one tray in empty_car
                   (MOT[MTrayX].fHasTray &&
                    MOT[MTrayX].ReadPos()==Prod.iXTrayEmpty ) ||
                   MOT[MMTrayY].fHasTray)
                {
                    DoAutoLoaderDelay.SetMSAndOn(2000);
                    Task=150;
                }
            }
            break;
        case 150:
            if(DoAutoLoaderDelay.Off())
                Task=1;
            break;
        case 200:
            if(DoLoadNewLoaderTrayToCar())
            {
                InitLoadNewLoaderTrayToCarTask();
                Task=1;
            }
            break;
        case 300:
            if(DoLoaderTrayToRear())
            {
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
int iUnLoadNewLoaderTrayTask=1;
TQPF_Timer DoUnLoadNewLoaderToStackDelay;
void InitUnLoadNewLoaderTrayTask()
{
    iUnLoadNewLoaderTrayTask=1;
}
//------------------------------------------------------------------------------
bool DoUnLoadNewLoaderToStack(bool bAlarm)
{
//    int ret;
    int &Task=iUnLoadNewLoaderTrayTask;
    AnsiString sBuffer="";

    switch(Task)
    {
        case 1:
            #ifndef SOFT_SIMULTE
            if(TrayMoveStatus(0, __FUNC__)==0 &&                                //Sam 20210325 : 整合 tray move 狀態
               LastSet.iRealDummy!=DUMMY)                                       //Steven 20130930
            {
                Task=10;
                break;
            }
            #endif
            MOT[MMTrayZ].SetTray(NULL_IC, __FUNC__);

            if(Sen[SnLoaderCarHasTray].IsOff() &&
               Sen[SenLoaderCCWDete].IsOff() &&                                 //kevin 20120518 有tray在軌道上升到分離高度
               Cylinder[C_TrayZ_Selector].OnStatus())                           //Sam 20210728 : 確保分離汽缸已經被打開
                Task=200;
            else
                Task=50;
            break;
        case 10:
            if(Sen[SnLoaderCarHasTray].IsOn() &&                                //ChungHung 20150604 add for AutoRetest
               Sen[SnCheckTrayDirection].IsOn())                                //JerryYang 20190902 SnCheckTrayDirection //Steven 20190815 : SnCheckTrayDirection -- > SnLoaderUpSafedetect
            {
                Task=50;
                break;
            }
            else                                                                //ChungHung 20150604 add for AutoRetest
            {
                ShowMyMessage("Please check LoadTray postion", "請確認LoadTray位置.");
            }
            DoUnLoadNewLoaderToStackDelay.SetMSAndOn(1000);
            Task=20;
            break;
        case 20:
            if(DoUnLoadNewLoaderToStackDelay.Off())
                Task=1;
            break;
        case 50:
            #ifndef SOFT_SIMULTE
            if(TrayMoveStatus(0,__FUNC__)==0 &&                                 //Sam 20210621 : 退出來時還要一直轉，不然會回車導致翻盤
               LastSet.iRealDummy!=DUMMY)
            {
                TrayMoveOut(true, 0, Prod.iMLoaderYCarPos);
                Task=1;
                break;
            }
            #endif

            MOT[MMTrayZ].SetTray(NULL_IC, __FUNC__);
            if(CylinderMiddle(C_Load_Up))
            {
                TrayMoveOut(false, 0, Prod.iMLoaderYCarPos);                    //Sam 20210621 : 退出來時還要一直轉，不然會回車導致翻盤
                Task=100;
            }
            break;
        case 100:
            if(LastSet.iRealDummy==DUMMY ||
               Cylinder[C_TrayZ_Selector].Push())                               //Steven 20111212
            {
                DoUnLoadNewLoaderToStackDelay.SetMSAndOn(500);
                Task=150;
            }
            break;
        case 150:
            if(DoUnLoadNewLoaderToStackDelay.Off())
                Task=200;
            break;
        case 200:
            if(CylinderUp(C_Load_Up))
                Task=300;
            break;
        case 300:
            if(LastSet.iRealDummy==DUMMY ||
               Cylinder[C_TrayZ_Selector].Pop())                                //Steven 20111212
            {
                DoUnLoadNewLoaderToStackDelay.SetSecAndOn(Ld_UldDelayTime.ULD_TrayBackDelay);
                MOT[MMTrayZ].ClearTray(__FUNC__);
                Task=400;
            }
            break;
        case 400:
            if(DoUnLoadNewLoaderToStackDelay.Off())
            {
                if(CylinderLower(C_Load_Up))
                {
                    Task=1;

                    if(bLoadBFBackTray ||                                                                                                                       //kevin 20230328 add Load Receive Tray
                       bDoLoaderCleanOut)                                                                                                                       //KenHsieh 20230510 : add 輸入顆數達成就 Clean out
                    {
                        return true;
                    }

                    iCountReceiveTray_ART++;                                                                                                                    //kevin 20150613 計算收回tray

                    if(CUSTOMER_CODE==CC_KYEC_LEE)
                    {
                        if(bARTTrayArmOK==true)                                                                                                                 //wei 20151217 ART 回盤OK
                        {
                            if(IniConfig.bEnable_SECS_GEM==true)                                                                                                //wei 20150630 使用SECS_GEM不ShowMessage //ChungHung 20150511 modify
                            {
                                EventReport(SECS_EVENT.ArtReceiveTrayOK);                                                                                       //59    自動回Tray作業完成發報Event給Host  //wei 20150630
                            }
                            else
                            {
                                if(bAlarm)
                                    ShowErrorMessage("WAR0612", 0, MMSystem, 0,"");                                                                             //wei 20150805 WAR0601==>WAR0612
                            }
                            iCountReceiveTray_ART=0;
                            bARTTrayArmOK=false;
                        }
                        return true;
                    }
                    else if(CosFunction.bUseSCKART)                                                                                                             //Steven 20161201 (wei) : For SCK 93K ART
                    {
                        if(fSCKART->iTesterType==0)
                            fSCKART->SetLotStatus(fSCKART->iLOTSTATUS_W);
                        return true;
                    }
                    else
                    {
                        if(iCountReceiveTray_ART>=(LastSet.iUnloaderTrayCount_ART[eAuto1]+
                                                   LastSet.iUnloaderTrayCount_ART[eAuto2]+
                                                   LastSet.iUnloaderTrayCount_ART[eAuto3]))
                        {
                            RespondASECom("@e02009Done");                                                                                                       //kevin 20170830 回應 ase ART Clean out finish
                            sBuffer.sprintf("@e02009Tray A1,%d,%d,A2,%d,%d,A3,%d,%d", iArt_AutoTotal[0][0], iArt_AutoTotal[0][1],
                                                                                      iArt_AutoTotal[1][0], iArt_AutoTotal[1][1],
                                                                                      iArt_AutoTotal[2][0], iArt_AutoTotal[2][1]);                              //kevin 20170830 記錄ART Auto Tray IC數量
                            RespondASECom(sBuffer);                                                                                                             //kevin 20170830 回應 ase ART Mov Tray finish

                            if(IniConfig.bEnable_SECS_GEM==true)                                                                                                //wei 20150630 使用SECS_GEM不ShowMessage //ChungHung 20150511 modify
                            {
                                EventReport(SECS_EVENT.ArtReceiveTrayOK);                                                                                       //59    自動回Tray作業完成發報Event給Host  //wei 20150630
                            }
                            else
                            {
                                if(bAlarm)
                                    ShowErrorMessage("WAR0612", 0, MMSystem, 0,"");                                                                             //wei 20150805 WAR0601==>WAR0612
                            }

                            iCountReceiveTray_ART=0;
                        }
                        return true;
                    }
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
// 送空 tray
//------------------------------------------------------------------------------
void InitAutoLoaderReceiveTask()
{
    iAutoLoaderReceiveTask=1;
}
//------------------------------------------------------------------------------
void DoAutoLoaderReceive()
{
    int &Task=iAutoLoaderReceiveTask;
    switch(Task)
    {
        case 1:
            iReceiveLoaderTray=2;
            if(MOT[MMTrayY_Car].fHasTray)
            {
                InitUnLoadNewLoaderTrayTask();
                Task=200;
            }
            else
            {
                InitLoaderTrayToFrontTask();
                Task=100;
            }
            break;
        case 100:
            if(DoLoaderTrayToFront())
            {
                iReceiveLoaderTray=2;
                if(MOT[MMTrayY_Car].fHasTray)
                {
                    InitUnLoadNewLoaderTrayTask();
                    Task=200;
                }
                else
                {
                    Task=1;
                    iReceiveLoaderTray=0;
                }
            }
            break;
        case 200:
            if(DoUnLoadNewLoaderToStack(false))
            {
                MOT[MMTrayY_Car].ClearTray(__FUNC__);
                if(MOT[MMTrayY].fHasTray)
                {
                    InitLoaderTrayToFrontTask();
                    Task=100;
                }
                else
                {
                    iReceiveLoaderTray=0;
                    Task=1;
                }
            }
            break;
    }
}
//------------------------------------------------------------------------------

