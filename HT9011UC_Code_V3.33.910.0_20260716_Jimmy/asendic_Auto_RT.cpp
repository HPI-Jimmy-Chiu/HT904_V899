#include "MachineDefine.h"
#pragma hdrstop

#include "asendic_Auto_RT.h"

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

#pragma package(smart_init)

int iAutoReceiveTask            [MAX_AUTO_TRAY]={1};
int iLoadNewAutoTrayToCarTask   [MAX_AUTO_TRAY]={1};
int iTrayZAutoTrayToWaitTask    [MAX_AUTO_TRAY]={1, 1, 1, 1, 1, 1};             //kevin 20170809 (Steven) add 20150610 判斷是否最後一盤
int iAutoTrayReceiveTask        [MAX_AUTO_TRAY]={1, 1, 1, 1, 1, 1};
TQPF_Timer AutoDelay            [MAX_AUTO_TRAY];
TQPF_Timer AutoRTDelay;                                                         //kevin 20150610
//---------------------------------------------------------------------------
void InitLoadNewAutoTrayToCarTask(int Pos)
{
    iLoadNewAutoTrayToCarTask[Pos]=1;
}
//---------------------------------------------------------------------------
int DoLoadNewEmptyTrayToCar_RT(int Pos)
{
    AnsiString Str;
    int ret;
    int &Task=iLoadNewAutoTrayToCarTask[Pos];
    switch(Task)
    {
        case 1:
            if(MOT[iMMAuto_Car[Pos]].fHasTray)
            {
                AutoDelay[Pos].SetMSAndOn(1000);                                                                        //Steven 20130904 : 加入Time Out 避免Hang Up
                Task=400;
                break;
            }

            if(Sen[SnAutoTrayHasTray[Pos]].IsOff()==false ||
               LastSet.iRealDummy==DUMMY)
            {
                if(Sen[SnAutoPreDete[Pos]].IsOn() ||
                   Sen[SnAutoTrayCar[Pos]].IsOn())
                {
                    MOT[iMMAuto_Car[Pos]].SetTray(NULL_IC, __FUNC__);
                    MOT[iAutoZMot[Pos]].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }

                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                                                     //kevin 20150610  偵測是否最後一盤
                {
                    InitTrayZAutoTrayToWaitTask();                                                                      //kevin 20150610
                    Task=50;
                }
                else
                {
                    Task=51;
                }
                AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                        //Steven 20140409 : AutoCylinderXX Add Reset
            }
            else
            {
                if(MOT[iMMAuto_Car[Pos]].fHasTray==false)                                                               //jou 2011-12-21 有發生Tray feed完,汽缸還在上面,所以做完下降
                {
                    if(Cylinder[C_Auto_Selector[Pos]].OffStatus()==false)                                               //汽缸在上面
                    {
                        if(Sen[SnAutoTrayCar[Pos]].IsOff() && Sen[SnAutoPreDete[Pos]].IsOff())
                        {
                            Cylinder[C_Auto_Up[Pos]].Off();
                            Cylinder[C_Auto_Selector[Pos]].Off();
                            break;
                        }
                    }
                }

                ShowErrorMessage(sMES1121[Pos], K_RETRY, iMMAuto_Car[Pos]);                                             //Steven 20150108 : MES1021 --> MES1121, MES1221, MES1321
            }
            break;
        case 50:
            ret=DoTrayZAutoTrayToWait(Pos);
            if(ret==1)
            {
                Task=300;
            }
            else if(ret==2)                                                     //kevin 20150613 尾盤不入tray
            {
                MOT[iAutoZMot[Pos]].ClearTray(__FUNC__);
                Task=1;
                return 2;
            }
            break;
        case 51:
            if(AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))
            {
                MOT[iAutoZMot[Pos]].ClearTray(__FUNC__);
                Task=100;
            }
            break;
        case 100:
            if(LastSet.iRealDummy==DUMMY)                                                                               //Steven 20100618
                Task=200;
            else if(Cylinder[C_AutoZ_Select[Pos]].Push())
                Task=200;

            AutoCylinderMiddle(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                        //Steven 20140409 : AutoCylinderXX Add Reset
            break;
        case 200:
            if(AutoCylinderMiddle(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))
            {
                if(bEmptyPause)
                {
                    bEmptyPause=false;
                    ShowMyMessage("Auto split wait", "Auto 分離暫停", "DoLoadNewEmptyTrayToCar 200");
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
            else if(Cylinder[C_AutoZ_Select[Pos]].Pop())
            {
                Task=400;
            }
            AutoDelay[Pos].SetMSAndOn(1000);                                    //Steven 20130904 Start: 加入Time Out 避免Hang Up
            break;
        case 400:
            if(AutoDelay[Pos].Off())
            {
                AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                     //Steven 20140409 : AutoCylinderXX Add Reset
                Task=410;
            }
            break;
        case 410:
            if(AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))
            {
                AutoDelay[Pos].SetMSAndOn(1000);
                Task=420;
            }
            break;
        case 420:
            if(bHandlerPause)                                                                                           //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                AutoDelay[Pos].SetMSAndOn(1000);
            }

#ifndef SOFT_SIMULTE
            if((Sen[SnAutoPreDete[Pos]].IsOn()  ||
                Sen[SnAutoTrayCar[Pos]].IsOn()) ||
               LastSet.iRealDummy==DUMMY)
#endif
            {
                MOT[iMMAuto_Car[Pos]].SetTray(NULL_IC, __FUNC__);
                MOT[iAutoZMot[Pos]].ClearTray(__FUNC__);
                Task=1;
                return 1;
            }
#ifndef SOFT_SIMULTE
            else if(AutoDelay[Pos].Off())
            {
                ret=ShowErrorMessage(sJAM1113[Pos], K_RETRY|K_SKIP, iMMAuto_Car[Pos]);                                  //Steven 2010108 : JAM1001 --> JAM1113, JAM1213, JAM1313
                if(ret==K_SKIP)
                {
                    MOT[iAutoZMot[Pos]].ClearTray(__FUNC__);
                    Task=1;
                }
            }
            break;
#endif
    }
    return 0;
}
//------------------------------------------------------------------------------
int iAutoTrayToFrontTask            [MAX_AUTO_TRAY]={1};
TQPF_Timer hAutoTrayToFront         [MAX_AUTO_TRAY];
TQPF_Timer hAutoTrayToFrontForDummy [MAX_AUTO_TRAY];
void InitAutoTrayToFrontTask(int Pos)
{
    iAutoTrayToFrontTask[Pos]=1;
}
//------------------------------------------------------------------------------
bool DoAutoTrayToFront(int Pos)
{
    AnsiString Str;
    int iMot=iMMAuto[Pos];
    int &Task=iAutoTrayToFrontTask[Pos];
    int ret;
    static bool bAutoHasDuplicateError[MAX_AUTO_TRAY]={false, false, false, false, false, false};                       //Steven 20120220 : Empty Tray重複Alarm Flag
    switch(Task)
    {
        case 1:
            hAutoTrayToFront[Pos].SetMSAndOn(20000);
            hAutoTrayToFrontForDummy[Pos].SetMSAndOn(5000);
            Task=100;
        case 100:
            if(bHandlerPause)                                                                                                                                   //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                Task=1;
                break;
            }

            Cylinder[C_AutoSide_Fixer[Pos]].Off();
            Cylinder[C_AutoEdgePush[Pos]].Off();
            Cylinder[C_AutoUpPress[Pos]].Off();                                                                                                                 //JerryYang 20190423 新增unloader壓tray

            TrayMoveOut(true, Pos+3);

            if(Sen[SnAutoTrayCar[Pos]].IsOff()==false)
            {
                TrayMoveOut(false, Pos+3);
                bAutoHasDuplicateError[Pos]=false;                                                                                                              //Steven 20120220 : Empty Tray重複Alarm Flag
                Task=101;
                hAutoTrayToFrontForDummy[Pos].SetMSAndOn(10000);
                break;
            }

            if(hAutoTrayToFrontForDummy[Pos].Off())
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    TrayMoveOut(false, Pos+3);
                    MOT[iMMAuto_Car[Pos]].SetTray(NULL_IC, __FUNC__);
                    MOT[iMot].ClearTray(__FUNC__);
                    bAutoHasDuplicateError[Pos]=false;                                                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                    return true;
                }
            }

            if(hAutoTrayToFront[Pos].Off())
            {
                ret=ShowErrorMessage(sJAM1112[Pos], K_SKIP|K_RETRY, iMot, bAutoHasDuplicateError[Pos], "DoAutoTrayToFront_100");                                //Steven 20150108 : JAM1012 --> JAM1112, JAM1212, JAM1312
                if(ret==K_RETRY)
                {
                    bAutoHasDuplicateError[Pos]=true;                                                                                                           //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                }
                else
                {
                    TrayMoveOut(false, Pos+3);
                    bAutoHasDuplicateError[Pos]=false;                                                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                    MOT[iMot].ClearTray(__FUNC__);
                    MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }
            }
            break;
        case 101:
            if(Sen[SnAutoCWPreDetect[Pos]].IsOff()==false &&
               Sen[SnAutoTrayDetect[Pos]].IsOff()==false)
            {
                Cylinder[C_AutoEdgePush[Pos]].On();
                Task=102;
                break;
            }
            else if(hAutoTrayToFrontForDummy[Pos].Off())                                                                //kevin 20120904
            {
                ShowErrorMessage(sJAM1112[Pos], K_RETRY, iMot, bAutoHasDuplicateError[Pos], "DoAutoTrayToFront_101");   //Steven 20150108 : JAM1212 --> JAM1112, JAM1212, JAM1312
                Task=100;
            }
            break;
        case 102:
            if(Cylinder[C_AutoSide_Fixer[Pos]].Push())
            {
                hAutoTrayToFrontForDummy[Pos].SetMSAndOn(100);
                Task=103;
            }
            break;
        case 103:
            if(hAutoTrayToFrontForDummy[Pos].Off())
            {
                hAutoTrayToFrontForDummy[Pos].SetMSAndOn(100);
                Cylinder[C_AutoSide_Fixer[Pos]].Off();
                Task=104;
            }
            break;
        case 104:
            if(hAutoTrayToFrontForDummy[Pos].Off())
            {
               Cylinder[C_AutoEdgePush[Pos]].Off();
               Task=105;
            }
            break;
        case 105:
            MOT[iMot].ClearTray(__FUNC__);
            MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);
            bAutoHasDuplicateError[Pos]=false;                                  //Steven 20120220 : Empty Tray重複Alarm Flag
            Task=1;
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int iAutoTrayToRearTask             [MAX_AUTO_TRAY]={1};
TQPF_Timer hAutoTrayToRear          [MAX_AUTO_TRAY];
TQPF_Timer hAutoTrayToRearForDummy  [MAX_AUTO_TRAY];
void InitAutoTrayToRearTask(int Pos)
{
    iAutoTrayToRearTask[Pos]=1;
}
//------------------------------------------------------------------------------
//---- 收空TRAY----
bool DoAutoTrayToRear(int Pos)
{
    static bool bAutoHasDuplicateError[MAX_AUTO_TRAY]={false, false, false, false, false, false};                       //Steven 20120220 : Empty Tray重複Alarm Flag

    int &Task       =iAutoTrayToRearTask[Pos];
    int ret         =0;
    bool bFlag      =false;
    AnsiString Str  ="";

    switch(Task)
    {
        case 1:
            hAutoTrayToRearForDummy[Pos].SetMSAndOn(5000);
            hAutoTrayToRear[Pos].SetMSAndOn(20000);
            Cylinder[C_AutoEdgePush[Pos]].Off();
            Cylinder[C_AutoUpPress[Pos]].Off();                                 //JerryYang 20190423 新增unloader壓tray
            Task=100;
        case 100:
            if(Cylinder[C_AutoSide_Fixer[Pos]].Pop())                           //ChungHung 20140625 add
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

            if(MOT[iMMAuto[Pos]].fHasTray &&
               MOT[iMMAuto_Car[Pos]].fHasTray)                                                                                                                  //Steven 20130917 : Double Tray問題,測試 Q4
            {
                Task=1;
                return true;
            }

            TrayMoveIn(true, Pos+3);
            if(Sen[SnAutoTrayDetect[Pos]].IsOff()==false)
            {
                Task=450;
                break;
            }

            if(hAutoTrayToRearForDummy[Pos].Off())
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    Task=450;
                    break;
                }
            }

            if(hAutoTrayToRear[Pos].Off())
            {
                ret=ShowErrorMessage(sJAM1101[Pos], K_SKIP|K_RETRY, iMMAuto_Car[Pos], bAutoHasDuplicateError[Pos], "DoAutoTrayToRear_200");                     //Steven 20150108 : JAM1001 -> JAM1101, JAM1201, JAM1301
                if(ret==K_RETRY)
                {
                    bAutoHasDuplicateError[Pos]=true;                                                                                                           //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                }
                else
                {
                    bAutoHasDuplicateError[Pos]=false;                                                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                    MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);
                    Task=1;
                    return true;
                }
            }
            break;
        case 300:
            if(Cylinder[C_AutoSide_Fixer[Pos]].Push())
                Task=400;
            break;
        case 400:
            #ifndef SOFT_SIMULTE
                bFlag=(Sen[SnAutoFixCyPush[Pos]].IsOn() ||
                       Cylinder[C_AutoSide_Fixer[Pos]].OffSensor());                                                                                            //wei 20241011 新增後勾氣缸off sensor,避免氣缸沒作動
            #else
                bFlag=false;
            #endif

            if(bFlag)
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    bAutoHasDuplicateError[Pos]=false;                                                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=450;
                }
                else
                {
                    Cylinder[C_AutoSide_Fixer[Pos]].Off();
                    ret=ShowErrorMessage(sJAM1103[Pos], K_RETRY, iMMAuto[Pos], bAutoHasDuplicateError[Pos], "DoAutoTrayToRear_400");                            //Steven 20150108 : JAM1003 -> JAM1103, JAM1203, JAM1303
                    bAutoHasDuplicateError[Pos]=false;                                                                                                          //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=300;
                    break;
                }
            }
            else
            {
                bAutoHasDuplicateError[Pos]=false;                                                                                                              //Steven 20120220 : Empty Tray重複Alarm Flag
                Task=500;
            }
            break;
        case 450:
            Cylinder[C_AutoEdgePush[Pos]].Off();
            hAutoTrayToRear[Pos].SetSecAndOn(Ld_UldDelayTime.LD_TrayArrivalDely);
            Task=460;
            break;
        case 460:
            if(hAutoTrayToRear[Pos].Off())
            {
                Task=500;
            }
            break;
        case 500:                                                               //ChungHung 20140625 add start
            if(Cylinder[C_AutoSide_Fixer[Pos]].Push())
            {
                TrayMoveIn(false, Pos+3);                                       //JerryYang 20201230 先後勾再停止,避免off的時候tray盤被往回帶
                hAutoTrayToRear[Pos].SetSecAndOn(0.5);
                Task=510;
            }
            break;
        case 510:
            if(hAutoTrayToRear[Pos].Off())
            {
                Task=600;
            }
            break;
        case 600:
            if(Cylinder[C_AutoSide_Fixer[Pos]].Pop())
            {
                MOT[iMMAuto[Pos]].SetTray(HAS_IC, __FUNC__);
                MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);                      //Steven 20111109 : 已入Tray卻未清掉資料
                Task=1;
                return true;
            }
            break;
            //ChungHung 20140625 add End
    }
    return false;
}
//------------------------------------------------------------------------------
//int iAutoTrayTask[MAX_AUTO_TRAY]={1};
//TQPF_Timer DoAutoTrayDelay[MAX_AUTO_TRAY];
//TQPF_Timer CheckAutoTrayDelay[MAX_AUTO_TRAY];
//void InitAutoTrayTask(int Pos)
//{
//    iAutoTrayTask[Pos]=1;
//}
//------------------------------------------------------------------------------
/*void DoAutoTray(int Pos)
{
    AnsiString Str;
    int iMot=iMMAuto[Pos];
    int &Task=iAutoTrayTask[Pos];
    static bool bAutoHasDuplicateError[MAX_AUTO_TRAY]={false, false, false, false, false, false};                       //Steven 20120220 : Auto Tray重複Alarm Flag
    if(iReceiveAutoTray[Pos]!=0)
    {
        DoAutoReceiveBinTray(Pos);
        return;
    }

    if(fAutoCanSupplyNewTray[Pos]==false)
        return;

    switch(Task)
    {
        case 1:
            if(MOT[iMot].fHasTray || Sen[SnAutoTrayDetect[Pos]].IsOn())
            {
                Task=20;
                break;
            }
            else
            {
                MOT[iMot].ClearTray(__FUNC__);
            }

            if(MOT[iMot].fHasTray==false)
            {
                Task=100;
                if(MOT[MMTrayY].fHasTray && MOT[MMTrayY].HasIC()==false)
                {
                    Task=1;
                    break;
                }
            }
            break;
        case 20:
            Cylinder[C_AutoSide_Fixer[Pos]].Off();
            Cylinder[C_AutoEdgePush[Pos]].Off();
            Cylinder[C_AutoUpPress[Pos]].Off();                                 //JerryYang 20190423 新增unloader壓tray
            Task=30;
            break;
        case 30:
            if(Cylinder[C_AutoSide_Fixer[Pos]].Pop())
                Task=40;
            break;
        case 40:
            if(Sen[SnAutoFixCyPush[Pos]].IsOn() ||
               Cylinder[C_AutoSide_Fixer[Pos]].OffSensor())                                                             //wei 20241011 新增後勾氣缸off sensor,避免氣缸沒作動
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    bAutoHasDuplicateError[Pos]=false;                                                                  //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=50;
                }
                else
                {
                    Cylinder[C_AutoSide_Fixer[Pos]].Off();
                    ShowErrorMessage(sJAM1103[Pos], K_RETRY, MMEmpty, bAutoHasDuplicateError[Pos], __FUNC__);           //Steven 20150108 : JAM1003 -> JAM1103, JAM1203, JAM1303
                    bAutoHasDuplicateError[Pos]=true;                                                                   //Steven 20120220 : Empty Tray重複Alarm Flag
                    Task=1;
                }
            }
            else
            {
                bAutoHasDuplicateError[Pos]=false;                                                                      //Steven 20120220 : Empty Tray重複Alarm Flag
                Task=50;
            }
            break;
        case 50:
            if(Cylinder[C_AutoSide_Fixer[Pos]].Pop())
            {
                #ifdef SOFT_SIMULTE
                MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);
                MOT[iMot].SetTray(NULL_IC);                                     //Steven 20130909
                #endif
                Task=60;
            }
            break;
        case 60:
            if(MOT[iMot].fHasTray==false)
            {
                //Question ASE_G
                DoAutoTrayDelay[Pos].SetMSAndOn(1000);
                Task=70;
            }
            else
            {
                CheckAutoTrayDelay[Pos].SetMSAndOn(100);
                Task=65;
            }
            break;
        case 65:
            if(CheckAutoTrayDelay[Pos].Off())
            {
                if(MOT[iMot].fHasTray==true && LastSet.iRealDummy>=HAS_TRAY &&
                   Sen[SnAutoTrayDetect[Pos]].IsOff() && Sen[SnAutoCWPreDetect[Pos]].IsOff())
                {
                    MOT[iMot].ClearTray(__FUNC__);                              //Steven 20130909
                }
                Task=60;

                if(MOT[iMot].fHasTray==false        &&
                   LastSet.iRealDummy>=HAS_TRAY &&
                   Sen[SnAutoTrayDetect[Pos]].IsOff()==false &&
                   Sen[SnAutoCWPreDetect[Pos]].IsOff()==false)                  //Steven 20130930
                {
                    MOT[iMot].SetTray(NULL_IC);
                }

                if(MOT[iMMAuto_Car[Pos]].fHasTray==false    &&
                   LastSet.iRealDummy>=HAS_TRAY &&
                   Sen[SnAutoTrayCar[Pos]].IsOff()==false &&
                   Sen[SnAutoPreDete[Pos]].IsOff()==false)                      //Steven 20130930
                {
                    MOT[iMMAuto_Car[Pos]].SetTray(NULL_IC);
                }

                if(MOT[MMTrayY].fHasTray && MOT[MMTrayY].HasIC()==false &&
                    MOT[iMot].fHasTray && MOT[iMMAuto_Car[Pos]].fHasTray)
                {
                    iReceiveAutoTray[Pos]=2;
                    iAutoTrayReceiveTask[Pos]=200;
                }

                //Steven 20130917 : Double Tray問題,測試 Q4
                if(MOT[iMot].fHasTray && MOT[iMMAuto_Car[Pos]].fHasTray)
                {
                    iReceiveAutoTray[Pos]=2;
                    iAutoTrayReceiveTask[Pos]=200;
                }
            }
            break;
        case 70:
            if(CheckAutoTrayDelay[Pos].Off())
            {
                if((Sen[SnAutoTrayDetect[Pos]].IsOn() && MOT[iMot].fHasTray==false) ||
                   (LastSet.iRealDummy==DUMMY && MOT[iMot].fHasTray==false))
                {
                    MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);
                    MOT[iMot].SetTray(NULL_IC);                                 //Steven 20130909
                }
                Task=100;
            }
            break;
        case 100:
            if(MOT[iMMAuto_Car[Pos]].fHasTray)
            {
                if((MOT[MTrayX].fHasTray && MOT[MTrayX].ReadPos()==Prod.iXTrayAuto[Pos]))
                {
                    CheckAutoTrayDelay[Pos].SetMSAndOn(2000);
                    Task=150;
                }
                else
                {
                    InitAutoTrayToRearTask(Pos);
                    Task=300;
                }
            }
            else
            {
                InitLoadNewAutoTrayToCarTask(Pos);
                Task=200;
                //jou 980421 empty tray no need relay one tray in empty_car
                if(iReceiveAutoTray[Pos]!=0 || (MOT[MTrayX].fHasTray && MOT[MTrayX].ReadPos()==Prod.iXTrayAuto[Pos]) ||
                    MOT[iMot].fHasTray)
                {
                    CheckAutoTrayDelay[Pos].SetMSAndOn(2000);
                    Task=150;
                }
            }
            break;
        case 150:
            if(CheckAutoTrayDelay[Pos].Off())
                Task=1;
            break;
        case 200:
            if(DoLoadNewAutoTrayToCar(Pos))
            {
                InitLoadNewAutoTrayToCarTask(Pos);
                Task=1;
            }
            break;
        case 300:
            if(DoAutoTrayToRear(Pos))
            {
                Task=1;
            }
            break;
    }
}   */
//------------------------------------------------------------------------------
int iUnLoadNewAutoTrayTask[MAX_AUTO_TRAY]={1};
TQPF_Timer DoUnLoadNewAutoToStackDelay[MAX_AUTO_TRAY];
TQPF_Timer hUnLoadSafetyCheckTimer[MAX_AUTO_TRAY];                             //AI(general) 20260323 (RogerYang) : UpSafe sensor 5秒保護計時
void InitUnLoadNewAutoTrayTask(int Pos)                                         //ChungHung 20140317 add Auto Retest
{
    iUnLoadNewAutoTrayTask[Pos]=1;
}
//------------------------------------------------------------------------------
bool DoUnLoadNewAutoToStack(int Pos)                                            //ChungHung 20140317 add Auto Retest
{
    int ret;
    AnsiString Str;
    int iMot=iMMAuto[Pos];
    int &Task=iUnLoadNewAutoTrayTask[Pos];
    static bool bAlreadyIn[MAX_AUTO_TRAY]={false, false, false, false, false, false};                                   //JerryYang 20180628 (wei) : add二段氣缸先on off一次避免爆衝
    static bool bUnLoadSafetyTimerStarted[MAX_AUTO_TRAY]={false, false, false, false, false, false};  //AI(general) 20260323 (RogerYang) : UpSafe sensor 5秒保護計時旗標
    bool bflag=false;
    switch(Task)
    {
        case 1:
            #ifdef SOFT_SIMULTE
                Task=50;
            #else
            {
                bool bPreDeteOn =Sen[SnAutoPreDete[Pos]].IsOn();                //AI(general) 20260323 (RogerYang) : 三顆 sensor 到位，重置保護計時
                bool bTrayCarOn =Sen[SnAutoTrayCar[Pos]].IsOn();
                bool bUpSafeOn  =Sen[SnAutoUpSafe[Pos]].IsOn();

                if((bPreDeteOn &&
                    bTrayCarOn &&
                    bUpSafeOn) ||
                   LastSet.iRealDummy==DUMMY)
                {
                    bUnLoadSafetyTimerStarted[Pos]=false;
                    Task=50;
                }
                else                                                            //sensor 未全部到位
                {
                    if(bUnLoadSafetyTimerStarted[Pos]==false)
                    {
                        hUnLoadSafetyCheckTimer[Pos].SetSecAndOn(5);            //啟動5秒保護計時
                        bUnLoadSafetyTimerStarted[Pos]=true;
                    }
                    else if(hUnLoadSafetyCheckTimer[Pos].Off())                 //超過5秒仍未到位
                    {
                        AnsiString sAlarmSensorAlias="";
                        if(bPreDeteOn==false)
                            sAlarmSensorAlias=Sen[SnAutoPreDete[Pos]].Name;

                        if(bTrayCarOn==false)
                        {
                            if(sAlarmSensorAlias!="")
                                sAlarmSensorAlias+="/";
                            sAlarmSensorAlias+=Sen[SnAutoTrayCar[Pos]].Name;
                        }

                        if(bUpSafeOn==false)
                        {
                            if(sAlarmSensorAlias!="")
                                sAlarmSensorAlias+="/";
                            sAlarmSensorAlias+=Sen[SnAutoUpSafe[Pos]].Name;
                        }

                        bUnLoadSafetyTimerStarted[Pos]=false;
                        ShowErrorMessage(sJAM1158[Pos], K_RETRY, iMMAuto[Pos], false,
                                         sAlarmSensorAlias);                    //帶出異常 sensor alias/name
                    }
                }
            }
            #endif
            AutoCylinderMiddle(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                        //Steven 20140409 : AutoCylinderXX Add Reset
            if(bARTUnloaderUseTwoCylin(Pos))                                                                            //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
            {
                Cylinder[C_Auto_Selector[Pos]].On();
                DoUnLoadNewAutoToStackDelay[Pos].SetMSAndOn(1000);
                bAlreadyIn[Pos]=false;
            }

            break;
        case 45:
            if(DoUnLoadNewAutoToStackDelay[Pos].Off()==true)                    //Sam 20211115 : 確保汽缸上前的狀態皮帶是一直再帶動轉
            {
                Task=50;
            }
            break;
        case 50:
            #ifndef SOFT_SIMULTE
            if(TrayMoveStatus(Pos+3,__FUNC__)==0  &&
               LastSet.iRealDummy!=DUMMY)                                                                               //Sam 20211115 : 確保汽缸上前的狀態皮帶是一直再帶動轉
            {
                TrayMoveOut(true, Pos+3);
                Task=45;
                DoUnLoadNewAutoToStackDelay[Pos].SetSecAndOn(2);
                break;
            }
            #endif
            if(bARTUnloaderUseTwoCylin(Pos))                                                                            //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
            {
                if(bAlreadyIn[Pos]==false)
                {
                    if(DoUnLoadNewAutoToStackDelay[Pos].Off()==true)
                    {
                        Cylinder[C_Auto_Selector[Pos]].Off();
                        bAlreadyIn[Pos]=true;
                    }
                    else
                    {
                        bAlreadyIn[Pos]=false;
                    }
                    break;
                }
            }
            else if(bARTUnloaderUseOneCylin(Pos))                                                                       //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
            {
                if(AutoCylinderMiddle(Pos, C_Auto_Selector[Pos], C_Auto_Up[Pos]))                                       //升到分離高度
                {
                    TrayMoveIn(false, Pos+3);
                    Task=100;
                }
                break;
            }

            if(AutoCylinderMiddle(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))                                           //升到分離高度
            {
                TrayMoveIn(false, Pos+3);
                Task=100;
            }
            break;
        case 100:
            if(LastSet.iRealDummy==DUMMY ||
               Cylinder[C_AutoZ_Select[Pos]].Push())
            {
                DoUnLoadNewAutoToStackDelay[Pos].SetSecAndOn(Ld_UldDelayTime.ULD_TrayBackDelay);
                Task=150;
            }
            break;
        case 150:
            if(DoUnLoadNewAutoToStackDelay[Pos].Off())
            {
                AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                        //Steven 20140409 : AutoCylinderXX Add Reset
                Task=200;
            }
            break;
        case 200:
            if(bARTUnloaderUseTwoCylin(Pos))                                                                            //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                bflag=AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]);                                        //Steven 20140409 : AutoCylinderXX Add Reset
            else if(bARTUnloaderUseOneCylin(Pos))                                                                       //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                bflag=AutoCylinderUp(Pos, C_Auto_Selector[Pos], C_Auto_Up[Pos]);                                        //Steven 20140409 : AutoCylinderXX Add Reset
            else
                bflag=AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]);                                        //Steven 20140409 : AutoCylinderXX Add Reset

            if(bflag)
            {
                Task=300;
            }
            break;
        case 300:
            if(Cylinder[C_AutoZ_Select[Pos]].Pop())
            {
                Task=350;
            }
            break;
        case 350:                                                               //kevin 20161129  Auto 收tray sensor沒調好 需alarm
            if((Sen[SnAutoPreDete[Pos]].IsOff() &&
                Sen[SnAutoTrayCar[Pos]].IsOff() &&
                Sen[SnAutoUpSafe[Pos]].IsOff()) ||
               LastSet.iRealDummy==DUMMY)
            {
                DoUnLoadNewAutoToStackDelay[Pos].SetSecAndOn(Ld_UldDelayTime.ULD_TrayBackDelay);
                MOT[iMot].ClearTray(__FUNC__);                                  //ChungHung 20140519 開啟AutoRtest時 Auto2在Tray End 後 會被清掉
                Task=400;
            }
            else
            {
                #ifdef SOFT_SIMULTE
                Task=400;
                #else
                ShowErrorMessage(sMES1121[Pos], K_RETRY, iMMAuto_Car[Pos]);
                #endif
            }
            break;
        case 400:
            if(DoUnLoadNewAutoToStackDelay[Pos].Off())
            {
                if(bARTUnloaderUseTwoCylin(Pos))                                                                        //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                    AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                 //Steven 20140409 : AutoCylinderXX Add Reset
                else if(bARTUnloaderUseOneCylin(Pos))                                                                   //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                    AutoCylinderLower(Pos, C_Auto_Selector[Pos], C_Auto_Up[Pos], true);                                 //Steven 20140409 : AutoCylinderXX Add Reset
                else
                    AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                 //Steven 20140409 : AutoCylinderXX Add Reset
                Task=401;
            }
            break;
        case 401:
            if(IniConfig.bP37bAutoCylinderUP)                                                                           //JerryYang 20200916 : tray feed時常態在上
            {
                bflag=true;
            }
            else
            {
                if(bARTUnloaderUseTwoCylin(Pos))                                                                        //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                    bflag=AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]);                                 //Steven 20140409 : AutoCylinderXX Add Reset
                else if(bARTUnloaderUseOneCylin(Pos))                                                                   //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                    bflag=AutoCylinderLower(Pos, C_Auto_Selector[Pos], C_Auto_Up[Pos]);                                 //Steven 20140409 : AutoCylinderXX Add Reset
                else
                    bflag=AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]);                                 //Steven 20140409 : AutoCylinderXX Add Reset
            }

            if(bflag)
            {
                Task=402;
                DoUnLoadNewAutoToStackDelay[Pos].SetSecAndOn(3);
            }
            break;
        case 402:
            if(bHandlerPause)                                                   //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                DoUnLoadNewAutoToStackDelay[Pos].SetSecAndOn(3);
            }

            if(Sen[SnAutoPreDete[Pos]].IsOn() && Sen[SnAutoTrayCar[Pos]].IsOn())
            {
                ret=ShowErrorMessage(sJAM1113[Pos], K_RETRY|K_SKIP, iMMAuto_Car[Pos]);
                if(ret==K_SKIP)
                    Task=1;
            }
            else if(DoUnLoadNewAutoToStackDelay[Pos].Off())
            {
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
// 送空 tray
//------------------------------------------------------------------------------
void InitAutoTrayReceiveTask(int Pos)
{
    iAutoTrayReceiveTask[Pos]=1;
}
//------------------------------------------------------------------------------
void DoAutoTrayReceive(int Pos)
{
    int &Task=iAutoTrayReceiveTask[Pos];
    switch(Task)
    {
        case 1:
            InitAutoTrayToFrontTask(Pos);
            Task=100;
            break;
        case 100:
            if(DoAutoTrayToFront(Pos))
            {
                iReceiveAutoTray[Pos]=2;
                if(MOT[iMMAuto_Car[Pos]].fHasTray)
                {
                    InitUnLoadNewAutoTrayTask(Pos);
                    Task=200;
                }
                else
                {
                    Task=1;
                    iReceiveAutoTray[Pos]=0;
                }
            }
            break;
        case 200:
            if(DoUnLoadNewAutoToStack(Pos))
            {
                MOT[iMMAuto_Car[Pos]].ClearTray(__FUNC__);
                iReceiveAutoTray[Pos]=0;
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
//kevin 20150610 ART 判斷是否最後一盤
//------------------------------------------------------------------------------
void InitTrayZAutoTrayToWaitTask()
{
    iTrayZLoadTrayToWaitTask=1;
}
//--------------------------------------------------------------------------------
int DoTrayZAutoTrayToWait(int Pos)
{
    bool bAutoLoadHasTray=false;
    int &Task=iTrayZAutoTrayToWaitTask[Pos];                                    //kevin 20170809 (Steven) add
    AnsiString Str1, Str2;

    switch(Task)
    {
        case 1:
            AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);    //Steven 20140409 : AutoCylinderXX Add Reset
            Task++;

            break;
        case 2:
            if(AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))       //升到接盤位置
            {
                Task=100;
            }
            break;
        case 100:
            if(LastSet.iRealDummy==DUMMY)                                       //Steven 20100618 : Dummy Run時，不要放下Tray
                Task=200;
            else if(Cylinder[C_AutoZ_Select[Pos]].Push())
                Task=200;
            AutoRTDelay.SetMSAndOn(1000);
            break;
        case 200:
            if(AutoRTDelay.Off())
            {
                AutoCylinderMiddle(1, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);                                      //Steven 20140409 : AutoCylinderXX Add Reset
                Task=201;
            }
            break;
        case 201:
            if(AutoCylinderMiddle(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))   //kevin 20170810 (Steven) 升到分離高度 1-->Pos
            {
                if(LastSet.iRealDummy!=DUMMY && IniConfig.bLastLoaderNoInSide)
                {
                    bAutoLoadHasTray=Sen[SnAutoTrayHasTray[Pos]].IsOn();
                    if(bAutoLoadHasTray)                                        //kevin 20150613 尾盤不下去,需到分離高度去確認一盤不能偵測到
                    {
                        Task=300;
                        break;
                    }
                    else if(CosFunction.bUseSCKART==false &&
                            iAutoTrayCount[Pos]>0)                              //kevin 20170810 (Steven) add 放空盤
                    {
                        Str1.sprintf("Auto %d Tray, Please Cover One Tray", Pos+1);
                        Str2.sprintf("Auto %d Tray, 請放空盤", Pos+1);
                        ShowMyMessage(Str1,Str2);
                        Task=1;
                        break;
                    }
                }
                Task=1000;
            }
            break;
        case 300:
            if(Cylinder[C_AutoZ_Select[Pos]].Pop())
            {
                Task=400;
            }
            break;
        case 400:
            if(SW[SwACTrayY].Status()==false)
            {
                MOT[MMTrayZ].fHasTray=true;
                Task=1;
                return 1;
            }
            break;
        case 1000:                                                              //jou 2010-01-24 start : 如果Loader是最後一盤，不再load進去。
            if(AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))       //升到接盤位置
            {
                Task=1100;
            }
            break;
        case 1100:
            if(Cylinder[C_AutoZ_Select[Pos]].Pop())
            {
                Task=1200;
            }
            break;
        case 1200:
            if(AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))
            {
                Task=1;
                return 2;
            }
            break;
    }
    return 0;
}
//==============================================================================
