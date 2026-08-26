#include "MachineDefine.h"
#pragma hdrstop

#include "asendic_Auto2.h"

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
//#include "abin.h"

#include "OCRInsp.h"
#include "cMyDB.h"
#include "cmydef.h"
#include "common.h"
#include "AGV.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

int iAuto2ReceiveTask=1;                                                        //kevin 20120718
int iLoadNewAuto2TrayToCarTask=1;
TQPF_Timer Auto2Delay;
//---------------------------------------------------------------------------
void InitLoadNewAuto2TrayToCarTask()
{
    iLoadNewAuto2TrayToCarTask=1;
}
//---------------------------------------------------------------------------
bool DoLoadNewAuto2TrayToCar()
{
    int ret;
    int &Task=iLoadNewAuto2TrayToCarTask;
    static bool bAuto2HasDuplicateError=false;                                  //Steven 20120220 : Empty Tray重複Alarm Flag
    switch(Task)
    {
        case 1:
            if(MOT[MMAuto2].fHasTray)
            {
                Auto2Delay.SetMSAndOn(1000);
                Task=400;
                break;
            }

            if(Sen[SnAuto2TrayHasTray].IsOff()==false ||
               LastSet.iRealDummy==DUMMY)
            {
                if(Sen[SnAuto2PreDete].IsOff()==false   ||
                   Sen[SnAuto2_Tray_Car].IsOff()==false)
                {
                    MOT[MMAuto2_Car].SetTray(NULL_IC, __FUNC__);
                    MOT[MMAuto2Z].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }
                Task=50;
                AutoCylinderUp(1, C_Auto2_Selector, C_Auto2_Up, true);          //Steven 20140409 : AutoCylinderXX Add Reset
            }
            else
            {
                if(MOT[MMAuto2_Car].fHasTray==false)                            //jou 2011-12-21 有發生Tray feed完,汽缸還在上面,所以做完下降
                {
                    if(Cylinder[C_Auto2_Up].OffStatus()==false)
                    {
                        if(Sen[SnAuto2_Tray_Car].IsOff() && Sen[SnAuto2PreDete].IsOff())
                        {
                            Cylinder[C_Auto2_Up].Off();
                            Cylinder[C_Auto2_Selector].Off();
                            break;
                        }
                    }
                }
                ShowErrorMessage("MES1024", K_RETRY, MMAuto2_Car);
            }
            break;
        case 50:
            if(AutoCylinderUp(1, C_Auto2_Selector,C_Auto2_Up))                  //升到最高位置
            {
                MOT[MMAuto2Z].ClearTray(__FUNC__);
                Task=100;
            }
            break;
        case 100:
            if(LastSet.iRealDummy==DUMMY)                                       //Steven 20100618
                Task=200;
            else if(Cylinder[C_Auto2LoaderZ_Select].Push())
                Task=200;
            Auto2Delay.SetMSAndOn(1000);
            break;
        case 200:
            if(Auto2Delay.Off())
            {
                AutoCylinderMiddle(1, C_Auto2_Selector, C_Auto2_Up, true);      //Steven 20140409 : AutoCylinderXX Add Reset
                Task=201;
            }
            break;
        case 201:
            if(AutoCylinderMiddle(1, C_Auto2_Selector, C_Auto2_Up))             //升到分離高度
            {
                if(bEmptyPause)
                {
                    bEmptyPause=false;
                    ShowMyMessage("Auto2 split wait", "Auto2 分離暫停", "DoLoadNewAuto2TrayToCar 200");
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
            else if(Cylinder[C_Auto2LoaderZ_Select].Pop())
            {
                Task=400;
            }
            Auto2Delay.SetMSAndOn(1000);                                        //Steven 20130904 Start: 加入Time Out 避免Hang Up
            break;
        case 400:
            if(Auto2Delay.Off())
            {
                AutoCylinderLower(1, C_Auto2_Selector, C_Auto2_Up, true);       //Steven 20140409 : AutoCylinderXX Add Reset
                Task=410;
            }
            break;
        case 410:
            if(AutoCylinderLower(1, C_Auto2_Selector, C_Auto2_Up))              //降到最低位置
            {
                Auto2Delay.SetMSAndOn(1000);
                Task=420;
            }
            break;
        case 420:
            if(bHandlerPause)                                                                                                                                   //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                Auto2Delay.SetMSAndOn(1000);
            }

            if((Sen[SnAuto2PreDete].IsOn() && Sen[SnAuto2_Tray_Car].IsOn())    ||
               (Sen[SnAuto2PreDete].IsOn() && Sen[SnAutoUpSafedetect1].IsOn()) ||
                LastSet.iRealDummy==DUMMY)
            {
                MOT[MMAuto2_Car].SetTray(NULL_IC, __FUNC__);
                MOT[MMAuto2Z].ClearTray(__FUNC__);
                bAuto2HasDuplicateError=false;
                Task=1;
                return true;
            }
            else if(Auto2Delay.Off())
            {
                ret=ShowErrorMessage("JAM1213", K_RETRY|K_SKIP, MMAuto2_Car, bAuto2HasDuplicateError, "DoLoadNewAuto2TrayToCar");                               //Steven 20150108 : JAM1201 --> JAM1213
                bAuto2HasDuplicateError=true;
                if(ret==K_SKIP)
                {
                    MOT[MMAuto2Z].ClearTray(__FUNC__);
                    bAuto2HasDuplicateError=false;
                    Task=1;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iAuto2TrayToFrontTask=1;
TQPF_Timer hAuto2TrayToFront;
TQPF_Timer hAuto2TrayToFrontForDummy;
void InitAuto2TrayToFrontTask()
{
    iAuto2TrayToFrontTask=1;
}
//------------------------------------------------------------------------------
bool DoAuto2TrayToFront()
{
    int &Task=iAuto2TrayToFrontTask;
    int ret;
    static bool bAuto2HasDuplicateError=false;                                  //Steven 20120220 : Empty Tray重複Alarm Flag
    switch(Task)
    {
        case 1:
            hAuto2TrayToFront.SetMSAndOn(20000);
            hAuto2TrayToFrontForDummy.SetMSAndOn(5000);
            Task=100;
        case 100:
            if(bHandlerPause)                                                                                           //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                Task=1;
                break;
            }

            Cylinder[C_Auto2Side_Fixer].Off();
            Cylinder[C_Auto2EdgePush].Off();
            if(SUPPORT_2_EMPTY_EMPTY)
            {
                SW[SwACAuto2].Off();
                MySleepEx(100,false);
                SW[SwACAuto2CW].Off();
                MOT[MMAuto2_Car].SetTray(NULL_IC, __FUNC__);
                MOT[MMAuto2].ClearTray(__FUNC__);
                bAuto2HasDuplicateError=false;                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                Task=1;
                return true;
            }

            SW[SwACAuto2].On();
            SW[SwACAuto2CW].On();

            if(Sen[SnAuto2_Tray_Car].IsOn()==false &&
               Sen[SnAuto2TrayDetect].IsOff()==false)
            {
                SW[SwACAuto2].Off();
                MySleepEx(100, false);
                SW[SwACAuto2CW].Off();
                bAuto2HasDuplicateError=false;                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                Task=101;
                hAuto2TrayToFrontForDummy.SetMSAndOn(10000);                                                            //kevin 20120904
                break;
            }

            if(hAuto2TrayToFrontForDummy.Off())
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    SW[SwACAuto2].Off();
                    MySleepEx(100, false);
                    SW[SwACAuto2CW].Off();
                    MOT[MMAuto2_Car].ClearTray(__FUNC__);
                    MOT[MMAuto2].ClearTray(__FUNC__);
                    bAuto2HasDuplicateError=false;                                                                      //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                    return true;
                }
            }

            if(hAuto2TrayToFront.Off())
            {
                ret=ShowErrorMessage("JAM1212", K_SKIP|K_RETRY, MMAuto2, bAuto2HasDuplicateError, __FUNC__);            //Steven 20120925 : JAM1009 -> JAM1212
                if(ret==K_RETRY)
                {
                    bAuto2HasDuplicateError=true;                                                                       //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                }
                else
                {
                    SW[SwACAuto2].Off();
                    MySleepEx(100,false);
                    SW[SwACAuto2CW].Off();
                    bAuto2HasDuplicateError=false;                                                                      //Steven 20120220 : Empty Tray重複Alarm Flag
                    MOT[MMAuto2].ClearTray(__FUNC__);
                    MOT[MMAuto2_Car].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }
            }
            break;
        case 101:
            if(Sen[SnAuto2CWPreDetect].IsOff()==false &&
               Sen[SnAuto2TrayDetect].IsOff()==false)
            {
                Cylinder[C_Auto2EdgePush].On();
                Task=102;
                break;
            }
            else if(hAuto2TrayToFrontForDummy.Off())                                                                    //kevin 20120904
            {
                ShowErrorMessage("JAM1212", K_RETRY, MMAuto2, bAuto2HasDuplicateError, __FUNC__);                       //Steven 20120925 : JAM1209 -> JAM1212
                Task=100;
            }
            break;
        case 102:
            if(Cylinder[C_Auto2Side_Fixer].Push())
            {
                hAuto2TrayToFrontForDummy.SetMSAndOn(100);
                Task=103;
            }
            break;
        case 103:
            if(hAuto2TrayToFrontForDummy.Off())
            {
                hAuto2TrayToFrontForDummy.SetMSAndOn(100);
                Cylinder[C_Auto2Side_Fixer].Off();
                Task=104;
            }
            break;
        case 104:
            if(hAuto2TrayToFrontForDummy.Off())
            {
               Cylinder[C_Auto2EdgePush].Off();
               Task=105;
            }
            break;
        case 105:
            MOT[MMAuto2].ClearTray(__FUNC__);
            MOT[MMAuto2_Car].ClearTray(__FUNC__);
            bAuto2HasDuplicateError=false;                                      //Steven 20120220 : Empty Tray重複Alarm Flag
            Task=1;
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int iAuto2TrayToRearTask=1;
TQPF_Timer hAuto2TrayToRear;
TQPF_Timer hAuto2TrayToRearForDummy;
void InitAuto2TrayToRearTask()
{
    iAuto2TrayToRearTask=1;
}
//------------------------------------------------------------------------------
//---- 收空TRAY----
bool DoAuto2TrayToRear()
{
    static bool bAuto2HasDuplicateError=false;                                  //Steven 20120220 : Empty Tray重複Alarm Flag
    int &Task=iAuto2TrayToRearTask;
    int ret;
    switch(Task)
    {
        case 1:
            hAuto2TrayToRearForDummy.SetMSAndOn(5000);
            hAuto2TrayToRear.SetMSAndOn(20000);
            Cylinder[C_Auto2EdgePush].Off();
            Task=100;
        case 100:
            if(Cylinder[C_Auto2Side_Fixer].Pop())
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

            SW[SwACAuto2].On();
            SW[SwACAuto2CW].Off();
            if(Sen[SnAuto2TrayDetect].IsOn())
            {
                Task=450;
                break;
            }

            if(hAuto2TrayToRearForDummy.Off())
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    Task=450;
                    break;
                }
            }

            if(hAuto2TrayToRear.Off())
            {
                ret=ShowErrorMessage("JAM1201", K_SKIP|K_RETRY, MMAuto2_Car, bAuto2HasDuplicateError, "DoAuto2TrayToRear_200");                                 //Steven 20120925 : JAM1009 -> JAM1201
                if(ret==K_RETRY)
                {
                    bAuto2HasDuplicateError=true;                                                                                                               //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                }
                else
                {
                    bAuto2HasDuplicateError=false;                                                                                                              //Steven 20120220 : Empty Tray重複Alarm Flag
                    MOT[MMAuto2_Car].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }
            }
            break;
        case 450:
            Cylinder[C_Auto2EdgePush].Off();
            hAuto2TrayToRear.SetSecAndOn(Ld_UldDelayTime.LD_TrayArrivalDely);
            Task=460;
            break;
        case 460:
            if(hAuto2TrayToRear.Off())
            {
                SW[SwACAuto2CW].Off();
                SW[SwACAuto2].Off();
                Task=500;
            }
            break;
        case 500:
            if(Cylinder[C_Auto2Side_Fixer].Pop())
            {
                MOT[MMAuto2].SetTray(NULL_IC, __FUNC__);
                MOT[MMAuto2_Car].ClearTray(__FUNC__);                           //Steven 20111109 : 已入Tray卻未清掉資料
                Task=1;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iAutoAuto2Task=1;                                                           //kevin 20120718 auto2 送空 tray
TQPF_Timer DoAuto2Delay;
TQPF_Timer CheckAuto2TrayDelay;
void InitAuto2Task()
{
    iAutoAuto2Task=1;
}
//------------------------------------------------------------------------------
void DoAuto2()
{
    int i;
    int &Task=iAutoAuto2Task;
    static bool bAuto2HasDuplicateError=false;                                  //Steven 20120220 : Empty Tray重複Alarm Flag
    if(iReceiveAuto2Tray!=0)
    {
        DoAuto2Receive();                                                       //收auto2 空tray
        return;
    }

    if(fAuto2CanSupplyNewTray==false)
        return;

    switch(Task)
    {
        case 1:
            DoAuto2Delay.SetMSAndOn(1000);
            Task=2;
        case 2:
            if(DoAuto2Delay.Off())
               Task=3;
            break;
        case 3:
            if(MOT[MMAuto2_Car].fHasTray ||
               Sen[SnAuto2TrayDetect].IsOn() ||
               LastSet.iRealDummy==DUMMY)
            {
                Task=20;
                break;
            }
            else
            {
                MOT[MMAuto2].ClearTray(__FUNC__);
            }

            if(MOT[MMAuto2].fHasTray==false)
            {
                Task=100;

                i=WhichAutoNeedTray();                                          //which auto need tray
                if(MOT[MMTrayY].fHasTray && MOT[MMTrayY].HasIC()==false && i==0)
                {
                    Task=1;
                    break;
                }
            }
            break;
        case 20:
            Cylinder[C_Auto2Side_Fixer].Off();
            Cylinder[C_Auto2EdgePush].Off();
            Task=30;
            break;
        case 30:
            if(Cylinder[C_Auto2Side_Fixer].Pop())
                Task=40;
            break;
        case 40:
            if(Sen[SnAuto2FixCyPush].IsOff()==false)
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    bAuto2HasDuplicateError=false;                                                                      //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=50;
                }
                else
                {
                    Cylinder[C_Auto2Side_Fixer].Off();
                    ShowErrorMessage("JAM1203", K_RETRY, MMAuto2_Car, bAuto2HasDuplicateError, __FUNC__);               //Steven 20120925 : JAM1009 -> JAM1203
                    bAuto2HasDuplicateError=true;                                                                       //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                }
            }
            else
            {
                bAuto2HasDuplicateError=false;                                                                          //Steven 20120220 : Auto2 Tray重複Alarm Flag
                Task=50;
            }
            break;
        case 50:
            if(Cylinder[C_Auto2Side_Fixer].Pop())
            {
                #ifdef SOFT_SIMULTE
                MOT[MMAuto2_Car].ClearTray(__FUNC__);
                MOT[MMAuto2].SetTray(NULL_IC, __FUNC__);
                #endif
                Task=60;
            }
            break;
        case 60:
            if(MOT[MMAuto2].fHasTray==false)
            {
                DoAuto2Delay.SetMSAndOn(1000);
                Task=70;
            }
            else
            {
                CheckAuto2TrayDelay.SetMSAndOn(100);
                Task=65;
            }
            break;
        case 65:
            if(CheckAuto2TrayDelay.Off())
            {
                if(MOT[MMAuto2].fHasTray==true &&
                   LastSet.iRealDummy>=HAS_TRAY &&
                   Sen[SnAuto2TrayDetect].IsOff() &&
                   Sen[SnAuto2PreDete].IsOff())
                {
                    MOT[MMAuto2].ClearTray(__FUNC__);
                }
                Task=60;

                if(MOT[MMTrayY].fHasTray &&
                   MOT[MMTrayY].HasIC()==false &&
                   MOT[MMAuto2].fHasTray &&
                   MOT[MMAuto2_Car].fHasTray)
                {
                    iReceiveAuto2Tray=2;
                    iAuto2ReceiveTask=200;
                }
            }
            break;
        case 70:
            if(DoAuto2Delay.Off())
            {
                if((Sen[SnAuto2TrayDetect].IsOn() &&
                    MOT[MMAuto2].fHasTray==false) ||
                   (LastSet.iRealDummy==DUMMY     &&
                    fAGV->IsATK_AMR()==false &&
                    MOT[MMAuto2].fHasTray==false))
                {
                    MOT[MMAuto2_Car].ClearTray(__FUNC__);
                    MOT[MMAuto2].SetTray(NULL_IC, __FUNC__);
                    fAuto2CanSupplyNewTray=false;
                    break;
                }
                Task=100;
            }
            break;
        case 100:
            if(MOT[MMAuto2_Car].fHasTray)
            {
                InitAuto2TrayToRearTask();
                Task=300;
            }
            else
            {
                InitLoadNewAuto2TrayToCarTask();
                Task=200;
                if((MOT[MTrayX].fHasTray &&
                    MOT[MTrayX].ReadPos()==Prod.iXTrayAuto[1]) ||
                    MOT[MMAuto2].fHasTray)
                {
                    DoAuto2Delay.SetMSAndOn(2000);
                    Task=150;
                }
            }
            break;
        case 150:
            if(DoAuto2Delay.Off())
                Task=1;
            break;
        case 200:
            if(DoLoadNewAuto2TrayToCar())
            {
                InitLoadNewAuto2TrayToCarTask();
                iReceiveAuto2Tray=1;
                Task=1;
            }
            break;
        case 300:
            if(DoAuto2TrayToRear())
            {
                Task=50;
            }
            break;
    }
}
//-------------------------------------------------------------------------------
int iUnLoadNewAuto2TrayTask=1;
void InitUnLoadNewAuto2TrayTask()
{
    iUnLoadNewAuto2TrayTask=1;
}
//-------------------------------------------------------------------------------
bool DoUnLoadNewAuto2ToStack()
{
    int ret;
    int &Task=iUnLoadNewAuto2TrayTask;
    static bool bAuto2HasDuplicateError=false;                                  //Steven 20120220 : Empty Tray重複Alarm Flag
    switch(Task)
    {
        case 1:
            #ifdef SOFT_SIMULTE
                Task=50;
            #else
                if(Sen[SnAuto2PreDete].IsOn() &&
                   Sen[SnAuto2_Tray_Car].IsOn() &&
                   Sen[SnAutoUpSafedetect1].IsOn() ||
                   LastSet.iRealDummy==DUMMY)
                    Task=50;
            #endif
            AutoCylinderMiddle(1, C_Auto2_Selector, C_Auto2_Up, true);          //Steven 20140409 : AutoCylinderXX Add Reset
            break;
        case 50:
            if(AutoCylinderMiddle(1, C_Auto2_Selector, C_Auto2_Up))             //升到分離高度
            {
                SW[SwACAuto2].Off();
                SW[SwACAuto2CW].Off();
                Task=100;
            }
            break;
        case 100:
            if(LastSet.iRealDummy==DUMMY ||
               Cylinder[C_Auto2LoaderZ_Select].Push())                          //Steven 20111212
            {
                Auto2Delay.SetSecAndOn(Ld_UldDelayTime.ULD_TrayBackDelay);
                Task=150;
            }
            break;
        case 150:
            if(Auto2Delay.Off())
            {
                AutoCylinderUp(1, C_Auto2_Selector, C_Auto2_Up, true);          //Steven 20140409 : AutoCylinderXX Add Reset
                Task=200;
            }
            break;
        case 200:
            if(AutoCylinderUp(1, C_Auto2_Selector, C_Auto2_Up))                 //升到最高位置
                Task=300;
            break;
        case 300:
            if(LastSet.iRealDummy==DUMMY ||
               Cylinder[C_Auto2LoaderZ_Select].Pop())                           //Steven 20111212
            {
                Task=310;
            }
            break;
        case 310:                                                               //kevin 20161129  Auto2 收tray sensor沒調好 需alarm
            if(Sen[SnAuto2PreDete].IsOff() &&
               Sen[SnAuto2_Tray_Car].IsOff() ||
               LastSet.iRealDummy==DUMMY)                                       //kevin 20161129 沒有tray在軌道
            {
                Auto2Delay.SetSecAndOn(Ld_UldDelayTime.ULD_TrayBackDelay);
                MOT[MMAuto2].ClearTray(__FUNC__);
                Task=400;
            }
            else
            {
                #ifdef SOFT_SIMULTE
                Task=400;
                #else
                ShowErrorMessage("MES1024", K_RETRY, MMAuto2_Car);
                #endif
            }
            break;
        case 400:
            if(Auto2Delay.Off())
            {
                AutoCylinderLower(1, C_Auto2_Selector, C_Auto2_Up, true);       //Steven 20140409 : AutoCylinderXX Add Reset
                Task=401;
            }
            break;
        case 401:
            if(AutoCylinderLower(1, C_Auto2_Selector, C_Auto2_Up))              //降到最低位置
            {
                Task=402;
                Auto2Delay.SetSecAndOn(3);
            }
            break;
        case 402:
            if(Sen[SnAuto2PreDete].IsOn() && Sen[SnAuto2_Tray_Car].IsOn())
            {
                ret=ShowErrorMessage("JAM1113", K_RETRY|K_SKIP, MMAuto2_Car,bAuto2HasDuplicateError,__FUNC__);          //Steven 20120926 : JAM1169 -> JAM1113
                bAuto2HasDuplicateError=true;
                if(ret ==K_SKIP)
                {
                    bAuto2HasDuplicateError=false;
                    Task=1;
                }
            }
            else if(Auto2Delay.Off())
            {
                bAuto2HasDuplicateError=false;
                Task=1;
                fAuto2CanSupplyNewTray=true;
                return true;
            }
            break;
    }
    return false;
}
//-------------------------------------------------------------------------------
// kevin 20120718 送空 tray 去放ic
//-------------------------------------------------------------------------------
void InitAuto2ReceiveTask()
{
    iAuto2ReceiveTask=1;
}
//-------------------------------------------------------------------------------
void DoAuto2Receive()
{
    int &Task=iAuto2ReceiveTask;
    switch(Task)
    {
        case 1:
            InitAuto2TrayToFrontTask();
            Task=100;
            break;
        case 100:
            if(DoAuto2TrayToFront())
            {
                iReceiveAuto2Tray=2;
                if(MOT[MMAuto2_Car].fHasTray)
                {
                    InitUnLoadNewAuto2TrayTask();
                    Task=200;
                }
                else
                {
                    Task=1;
                    iReceiveAuto2Tray=0;
                }
            }
            break;
        case 200:
            if(DoUnLoadNewAuto2ToStack())
            {
                MOT[MMAuto2_Car].ClearTray(__FUNC__);
                iReceiveAuto2Tray=0;
                Task=1;
            }
            break;
    }
}
//-------------------------------------------------------------------------------