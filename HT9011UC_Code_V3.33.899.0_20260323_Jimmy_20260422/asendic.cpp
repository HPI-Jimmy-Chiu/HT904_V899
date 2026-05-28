#include "MachineDefine.h"
#pragma hdrstop

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
#include "cmydef.h"
#include "common.h"

#pragma package(smart_init)

//==============================================================================
TQPF_Timer LifterTime[3][3];        //[0][0] Up      - Loader
                                    //[1][0] Middle  - Loader
                                    //[2][0] Down    - Loader
                                    //Loader, Empty, Color
int iLifterTask[3][10];

TQPF_Timer AutoTime[3][MAX_UNLOAD_TRAY];    //[0][0] Up      - Loader
                                            //[1][0] Middle  - Loader
                                            //[2][0] Down    - Loader
                                            //Auto1, Auto2, Auto3, Auto4, Auto5, Auto6, Fix1, Fix2, Fix3, Fix4, Fix5, Fix6

int iAutoTask[3][MAX_UNLOAD_TRAY];
TQPF_Timer CylinderAlarmTime[MAX_AUTO_TRAY];                                    //kevin 20211116 add Z CylinderAlarmTime
//------------------------------------------------------------------------------
void initAutoTask()
{
    for(int i=0; i<MAX_UNLOAD_TRAY; i++)
    {
        iAutoTask[0][i]=1;
        iAutoTask[1][i]=1;
        iAutoTask[2][i]=1;
    }
}
//------------------------------------------------------------------------------
void initLifterTask()
{
    for(int i=0; i<10; i++)
    {
        iLifterTask[0][i]=1;
        iLifterTask[1][i]=1;
        iLifterTask[2][i]=1;
    }
}
//------------------------------------------------------------------------------
TQPF_Timer tAutoCleanTimer;
//------------------------------------------------------------------------------
//ChungHung 20130910 add for Sck
void RecordAutoCleanOutStartEnd(bool bStart)
{
    if(IniConfig.bA08LastLoaderAutoCleanOutAndCheckAgain==false)
        return;

    AnsiString AStr;

    if(bStart==true)
    {
        if(bLoaderNoTrayAutoCleanOut==true)
        {
            tAutoCleanTimer.LatchCycleTimeSec(true);
        }
    }
    else
    {
        if(bLoaderNoTrayAutoCleanOut==false)
        {
            bLoaderNoTrayAutoCleanOut=false;
            AStr.sprintf("Auto CleanOut Funtion record time : %d Sec", tAutoCleanTimer.LatchCycleTimeSec());
            RecordProcess(AStr);
        }
    }
}
//==============================================================================
//Supply IC from Loader car
//==============================================================================
void PushLoaderTrayInAverageTime(DWORD st)
{
    static DWORD Buffer[20]={0, 0, 0, 0, 0};
    DWORD sum=0;
    int ct=0;
    if(st<500 || st>8000)
        return;

    for(int i=0; i<4; i++)
        Buffer[i]=Buffer[i+1];
    Buffer[4]=st;
    for(int i=0; i<5; i++)
    {
        ct++;
        sum+=Buffer[i];
    }
    if(ct!=0)
    {
        LastSet.iLoaderTraySimulateTime=sum/ct;
    }
}
//==============================================================================
//
//==============================================================================
//void DoTubeLoad()
//{
//    int &Task=TubeLoadTask;
//    switch(Task)
//    {
//
//    }
//}
//==============================================================================
// NEW ADD START
//==============================================================================
//==============================================================================
// auto color
//==============================================================================
//void DoAutoColorReceive();
//==============================================================================
// NEW ADD START
//==============================================================================
bool bARTUnloaderUseOneCylin(int Part)                                          //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
{
    if(USE_AUTO_RETEST==eartInstall && UNLOADER_ART[Part]==eartInstall && bNoAutoZSelect)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
bool bARTUnloaderUseTwoCylin(int Part)                                          //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
{
    if(USE_AUTO_RETEST==eartInstall &&
        UNLOADER_ART[Part]==eartInstall &&
        bNoAutoZSelect==false &&
        USE_LdUldCassetteMode!=1)                                               //RogerYang 20260207 : Add fot 9046 CR
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
bool CylinderUp(int CylinderName)                                               //Part 0=LD 1=Empty 2=color
{
    int Part=0, iMot=0;
    int CylinderMidd=CylinderName+1;
    if(CylinderName==C_Load_Up)                                                 //JerryYang 20191017 修正loader Z馬達版 控制錯誤
    {
        Part=0;
        CylinderMidd=C_Load_Middle;
    }
    else if(CylinderName==C_Empty_Up)
    {
        Part=1;
        CylinderMidd=C_Empty_Middle;
    }
    else if(CylinderName==C_Color_Up)
    {
        Part=2;
        CylinderMidd=C_Color_Middle;
    }

    iMot=iTrayZMotor[Part];                                                     //Steven 20230907 : For HT-9011UC
    int &Task=iLifterTask[0][Part];
    bool bRet=false;

    switch(Task)
    {
        case 1:
            if(LOAD_Z_USE_MOTOR[Part])                                          //Steven 20200724 : 入Tray改用步進馬達移動後要有delay
            {
                bRet=MOT[iMot].MotorMove(Prod.TrayZ_Up[Part]);
                Task=1000;
                break;
            }

            bRet=Cylinder[CylinderMidd].OnStatus();                             //jou 2014-04-23 Loader/Unloader分離上升不需要先做動第二段汽缸
            if(bRet==false)
            {
                Cylinder[CylinderMidd].On();
                LifterTime[0][Part].SetSecAndOn(0.5);
                Task=50;
            }
            else
            {
                bLifterPause[Part]=true;                                        //jou 2011-02-21 start : magazine沒動作會alarm
                Task=100;
            }
            break;
        case 50:
            if(LifterTime[0][Part].Off())
            {
                LifterTime[0][Part].SetSecAndOn(0.5);
                Cylinder[CylinderMidd].Off();
                Task=60;
            }
            break;
        case 60:                                                                //jou loader上升汽缸,第二段先做動一下 2010-02-24
            if(LifterTime[0][Part].Off())
            {
                bLifterPause[Part]=true;                                        //jou 2011-02-21 start : magazine沒動作會alarm
                Task=100;
            }
            break;
        case 100:
            #ifdef SOFT_SIMULTE
            {
                Task=1;
                return true;
            }
            #else
            {
                Cylinder[CylinderName].On();
                Cylinder[CylinderMidd].On();

                if(bHandlerPause ||                                             //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
                   bLifterPause[Part]==true)                                    //jou 2011-02-21 start : magazine沒動作會alarm
                {
                    bLifterPause[Part]=false;
                    LifterTime[0][Part].Set0_1SecAndOn(Cylinder[CylinderName].OnAlarmTime);
                }

                if(Cylinder[CylinderName].OnStatus()==true &&
                   Cylinder[CylinderMidd].OnStatus()==false &&
                   Cylinder[CylinderMidd].OffStatus()==false)
                {
                    LifterTime[0][Part].Set0_1SecAndOn(Cylinder[CylinderName].OnDelayTime);
                    Task=200;
                }
                else if(LifterTime[0][Part].Off())
                {
                    ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Up error", Cylinder[CylinderName].CylinderName+"上升到最高點錯誤", "CylinderUp 100");
                    iLifterTask[0][Part]=1;
                    Task=1;
                }
                break;
            }
            #endif
        case 200:
            if(Cylinder[CylinderName].OnStatus()==false)
            {
                Task=1;
                return false;
            }

            if(LifterTime[0][Part].Off())
            {
                Task=1;
                return true;
            }
            break;
        case 1000:                                                              //Steven 20200724 : 入Tray改用步進馬達移動後要有delay
            bRet=MOT[iMot].MotorMove(Prod.TrayZ_Up[Part]);
            if(bRet)
            {
                LifterTime[0][Part].SetSecAndOn(0.1);
                Task=1100;
            }
            break;
        case 1100:
            if(LifterTime[0][Part].Off())
            {
                if(MOT[iMot].CheckArmPosArrival(MOT[iMot].ReadPos(), Prod.TrayZ_Up[Part], 10))
                {
                    Task=1;
                    return true;
                }
                else
                {
                    Task=1000;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool CylinderMiddle(int CylinderName)
{
    int Part=0, iMot=0;
    int CylinderMidd=CylinderName+1;
    if(CylinderName==C_Load_Up)                                                 //JerryYang 20191017 修正loader Z馬達版 控制錯誤
    {
        Part=0;
        CylinderMidd=C_Load_Middle;
    }
    else if(CylinderName==C_Empty_Up)
    {
        Part=1;
        CylinderMidd=C_Empty_Middle;
    }
    else if(CylinderName==C_Color_Up)
    {
        Part=2;
        CylinderMidd=C_Color_Middle;
    }

    iMot=iTrayZMotor[Part];                                                     //Steven 20230907 : For HT-9011UC
    int &Task=iLifterTask[1][Part];
    bool bRet=false;

    switch(Task)
    {
        case 1:
            if(LOAD_Z_USE_MOTOR[Part])                                          //Steven 20200724 : 入Tray改用步進馬達移動後要有delay
            {
                bRet=MOT[iMot].MotorMove(Prod.TrayZ_Mid[Part]);
                Task=1000;
                break;
            }
            Cylinder[CylinderName].On();
            Cylinder[CylinderName+1].Off();
            bLifterPause[Part]=true;                                            //jou 2011-02-21 start : magazine沒動作會alarm
            Task=100;
            break;
        case 100:
            #ifdef SOFT_SIMULTE
            {
                Task=1;
                return true;
            }
            #else
            {
                Cylinder[CylinderName].On();
                Cylinder[CylinderMidd].Off();

                if(bHandlerPause ||                                             //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
                   bLifterPause[Part]==true)                                    //jou 2011-02-21 start : magazine沒動作會alarm
                {
                    bLifterPause[Part]=false;
                    LifterTime[1][Part].Set0_1SecAndOn(Cylinder[CylinderName].OnAlarmTime);
                }

                if(Cylinder[CylinderName].OnStatus()==false &&
                   Cylinder[CylinderMidd].OnStatus())
                {
                    LifterTime[1][Part].SetSecAndOn(Ld_UldDelayTime.LD_StackMiddLockDelay);
                    Task=200;
                }
                else if(LifterTime[1][Part].Off())
                {
                    ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Middle error", Cylinder[CylinderName].CylinderName+"上升到中間錯誤", "CylinderMiddle 100");
                    LifterTime[1][Part].Set0_1SecAndOn(Cylinder[CylinderName].OnAlarmTime); //kevin 20210803 add
                    iLifterTask[1][Part]=1;
                    Task=1;
                }
                break;
            }
            #endif
        case 200:
            if(!((Cylinder[CylinderName].OnStatus()==false) &&
                  Cylinder[CylinderMidd].OnStatus()))
            {
                Task=1;
                return false;
            }
            if(LifterTime[1][Part].Off())
            {
                iLifterTask[1][Part]=1;
                return true;
            }
            break;
        case 1000:
            bRet=MOT[iMot].MotorMove(Prod.TrayZ_Mid[Part]);                     //Steven 20200724 : 入Tray改用步進馬達移動後要有delay
            if(bRet)
            {
                LifterTime[1][Part].SetSecAndOn(Ld_UldDelayTime.LD_StackMiddLockDelay);
                Task=1100;
            }
            break;
        case 1100:
            if(LifterTime[1][Part].Off())
            {
                if(MOT[iMot].ReadPos()==Prod.TrayZ_Mid[Part])
                {
                    Task=1;
                    return true;
                }
                else
                {
                    Task=1000;
                }
            }
            break;

    }
    return false;
}
//------------------------------------------------------------------------------
bool CylinderLower(int CylinderName)
{
    int Part=0, iMot=0;
    int CylinderMidd=CylinderName+1;
    if(CylinderName==C_Load_Up)                                                 //JerryYang 20191017 修正loader Z馬達版 控制錯誤
    {
        Part=0;
        CylinderMidd=C_Load_Middle;
    }
    else if(CylinderName==C_Empty_Up)
    {
        Part=1;
        CylinderMidd=C_Empty_Middle;
    }
    else if(CylinderName==C_Color_Up)
    {
        Part=2;
        CylinderMidd=C_Color_Middle;
    }

    iMot=iTrayZMotor[Part];                                                     //Steven 20230907 : For HT-9011UC
    int &Task=iLifterTask[2][Part];
    bool bRet=false;

    switch(Task)
    {
        case 1:
            if(Ld_UldDelayTime.LD_BeforeDownDelay!=0.0)                         //Steven 20240215 : 下降前Delay
            {
                LifterTime[2][Part].SetSecAndOn(Ld_UldDelayTime.LD_BeforeDownDelay);
                Task=10;
            }
            else
            {
                Task=50;
            }
            break;
        case 10:
            if(LifterTime[2][Part].Off())
            {
                Task=50;
            }
            else
            {
                break;
            }
        case 50:
            if(LOAD_Z_USE_MOTOR[Part])                                          //Steven 20200724 : 入Tray改用步進馬達移動後要有delay
            {
                bRet=MOT[iMot].MotorMove(0);
                Task=1000;
                break;
            }
            Cylinder[CylinderName].Off();
            Cylinder[CylinderMidd].Off();
            bLifterPause[Part]=true;                                            //jou 2011-02-21 start : magazine沒動作會alarm
            Task=100;
            break;
        case 100:
            #ifdef SOFT_SIMULTE
            {
                Task=1;
                return true;
            }
            #else
            {
                Cylinder[CylinderName].Off();
                Cylinder[CylinderMidd].Off();

                if(bHandlerPause ||                                             //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
                   bLifterPause[Part]==true)                                    //jou 2011-02-21 start : magazine沒動作會alarm
                {
                    bLifterPause[Part]=false;
                    LifterTime[2][Part].Set0_1SecAndOn(Cylinder[CylinderName].OffAlarmTime);
                }

                if(Cylinder[CylinderMidd].OffStatus())
                {
                    LifterTime[2][Part].SetSecAndOn(Ld_UldDelayTime.LD_LiftDownDelay);
                    Task=200;
                }
                else if(LifterTime[2][Part].Off())
                {
                    ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Down error", Cylinder[CylinderName].CylinderName+"下降錯誤", "CylinderLower 100");
                    iLifterTask[2][Part]=1;
                    Task=1;
                }
                break;
            }
            #endif
        case 200:
            if(!Cylinder[CylinderMidd].OffStatus())
            {
                Task=1;
                return false;
            }
            if(LifterTime[2][Part].Off())
            {
                iLifterTask[2][Part]=1;
                return true;
            }
            break;
        case 1000:                                                              //Steven 20200724 : 入Tray改用步進馬達移動後要有delay
            bRet=MOT[iMot].MotorMove(0);
            if(bRet)
            {
                LifterTime[2][Part].SetSecAndOn(Ld_UldDelayTime.LD_LiftDownDelay);
                Task=1100;
            }
            break;
        case 1100:
            if(LifterTime[2][Part].Off())
            {
                if(MOT[iMot].ReadPos()==0)
                {
                    Task=1;
                    return true;
                }
                else
                {
                    Task=1000;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
//kevin 20120718 修改輸入氣缸因代號位置不同共用程式)
//Part 0=Auto1 1=Auto2 2=Auto3
//------------------------------------------------------------------------------
bool AutoCylinderMidIsOn(int Part, int CylinderNameMid)
{
    bool bRet=false;
    if(Cylinder[CylinderNameMid].Enable==false)                                 //Steven 20250312 : Add protection
    {
        bRet=true;
    }
    else if(bARTUnloaderUseTwoCylin(Part))                                      //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
    {
        if(Cylinder[CylinderNameMid].OnStatus()==false   &&
           Cylinder[CylinderNameMid].OffStatus()==false)
        {
            bRet=true;
        }
    }
    else if(bARTUnloaderUseOneCylin(Part))                                      //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
    {
        if(Cylinder[CylinderNameMid].OnStatus()==true)
        {
            bRet=true;
        }
    }
    else
    {
        if(Cylinder[CylinderNameMid].OnStatus()==true)
        {
            bRet=true;
        }
    }

    return bRet;
}
//------------------------------------------------------------------------------
bool AutoCylinderUp(int Part, int CylinderName, int CylinderNameMid, bool bReset)
{
    int &Task=iAutoTask[0][Part];
    int iAuto=Part+3;
    int iMot =iAutoZMot[Part], iAlarm=0;                                        //Steven 20230907 : For HT-9011UC
    bool bRet;

    if(bReset)                                                                  //Steven 20140409 : AutoCylinderXX Add Reset
    {
        iAutoTask[0][Part]=1;
        return false;
    }

    switch(Task)
    {
        case 1:
            if(LOAD_Z_USE_MOTOR[iAuto])                                         //Steven 20200724 : 入Tray改用步進馬達移動後要有delay
            {
                bRet=MOT[iMot].MotorMove(Prod.TrayZ_Up[iAuto]);
                Task=1000;
                break;
            }
            Cylinder[CylinderName].On();
            if(Cylinder[CylinderNameMid].Enable)                                //Steven 20250312 : Add protection
                Cylinder[CylinderNameMid].On();
            AutoTime[0][Part].SetSecAndOn(0.5);
            Task=50;
            break;
        case 50:
            if(AutoTime[0][Part].Off())
            {
                bAuto2Pause[Part]=true;                                         //jou 2011-02-21 start : magazine沒動作會alarm
                Task=100;
            }
            break;
        case 100:
            #ifdef SOFT_SIMULTE
            {
                Task=1;
                return true;
            }
            #else
            {
                if(bHandlerPause ||                                             //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
                   bAuto2Pause[Part]==true)                                     //jou 2011-02-21 start : magazine沒動作會alarm
                {
                    bAuto2Pause[Part]=false;
                    if(Cylinder[CylinderName].Enable==true)                     //JerryYang 20250314
                    {
                        iAlarm=Cylinder[CylinderName].OnAlarmTime;
                    }
                    else if(CUSTOMER_CODE==CC_ASE_KaohSiung)
                    {
                        iAlarm=1000;
                    }
                    else
                    {
                        iAlarm=Cylinder[CylinderNameMid].OnAlarmTime;
                    }

                    if(iAlarm<300)
                    {
                        iAlarm=300;
                    }
                    AutoTime[0][Part].Set0_1SecAndOn(iAlarm);                   //JerryYang 20250314 : fix
                }

                if(bARTUnloaderUseTwoCylin(Part))                               //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                {
                    if(Cylinder[CylinderName].OnStatus()==true &&
                       AutoCylinderMidIsOn(Part, CylinderNameMid)==true)
                    {
                        AutoTime[0][Part].Set0_1SecAndOn(Cylinder[CylinderName].OnDelayTime);
                        Task=200;
                    }
                    else if(AutoTime[0][Part].Off())
                    {
                        ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Up error", Cylinder[CylinderName].CylinderName+"上升到最高點錯誤", "AutoCylinderUp 100");
                        iAutoTask[0][Part]=1;
                        Task=1;
                    }
                }
//                else if(bARTUnloaderUseOneCylin(Part))                          //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
//                {
//                    if(Cylinder[CylinderName].OffStatus()==false &&             //kevin 20220113 一段氣缸 上下動作
//                       AutoCylinderMidIsOn(Part, CylinderNameMid)==true)
//                    {
//                        AutoTime[0][Part].Set0_1SecAndOn(Cylinder[CylinderName].OnDelayTime);
//                        Task=200;
//                    }
//                    else if(AutoTime[0][Part].Off())
//                    {
//                        ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Up error", Cylinder[CylinderName].CylinderName+"上升到最高點錯誤", "AutoCylinderUp 100");
//                        iAutoTask[0][Part]=1;
//                        Task=1;
//                    }
//                }
                else
                {
                    if(Cylinder[CylinderName].OnStatus()==false &&
                       AutoCylinderMidIsOn(Part, CylinderNameMid)==true)
                    {
                        AutoTime[0][Part].Set0_1SecAndOn(Cylinder[CylinderName].OnDelayTime);
                        Task=200;
                    }
                    else if(AutoTime[0][Part].Off())
                    {
                        ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Up error", Cylinder[CylinderName].CylinderName+"上升到最高點錯誤", "AutoCylinderUp 100");
                        iAutoTask[0][Part]=1;
                        Task=1;
                    }
                }
                break;
            }
            #endif
        case 200:
            if(AutoTime[0][Part].Off())
            {
                CylinderAlarmTime[0].SetSecAndOn(10);                           //kevin 20211116 add Z CylinderAlarmTime
                Task++;
            }
            break;
        case 201:
            if(CylinderAlarmTime[0].Off())                                      //kevin 20211116 add alarm
            {
                ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Up error", Cylinder[CylinderName].CylinderName+"上升到最高點錯誤", "AutoCylinderUp 200");
                iAutoTask[0][Part]=1;
                Task=1;
                return false;
            }

            if(bARTUnloaderUseTwoCylin(Part))                                   //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
            {
                if(Cylinder[CylinderName].OnStatus()==false)
                {
                    return false;
                }
                else if(AutoTime[0][Part].Off())
                {
                    iAutoTask[0][Part]=1;
                    return true;
                }
            }
//            else if(bARTUnloaderUseOneCylin(Part))                              //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
//            {
//                if(Cylinder[CylinderName].OnStatus()==false)                    //Steven 20211201 : 先改回來
//                {
//                    Task=1;
//                    return false;
//                }
//                else if(AutoTime[0][Part].Off())
//                {
//                    iAutoTask[0][Part]=1;
//                    return true;
//                }
//            }
            else
            {
                if(Cylinder[CylinderNameMid].OnStatus()==false)                 //JerryYang 20250314 : fix non ART AUTO上升誤ALARM
                {
                    Task=1;
                    return false;
                }
                else if(AutoTime[0][Part].Off())
                {
                    iAutoTask[0][Part]=1;
                    return true;
                }
            }
            break;
        case 1000:                                                              //Steven 20200724 : 入Tray改用步進馬達移動後要有delay
            bRet=MOT[iMot].MotorMove(Prod.TrayZ_Up[iAuto]);
            if(bRet)
            {
                AutoTime[0][Part].SetSecAndOn(0.1);
                Task=1100;
            }
            break;
        case 1100:
            if(AutoTime[0][Part].Off())
            {
                if(MOT[iMot].ReadPos()==Prod.TrayZ_Up[iAuto])
                {
                    #ifdef DEBUG_AutoZMotor                                     //RogerYang 20250512 rogertest for Auto3Z
                    if(USE_AUTO_RETEST==eartUninstall &&
                        IniConfig.bVTESTFunction==true)
                    {
                        AnsiString str="";
                        str.sprintf("CurrentP:%d, TargetP:%d, iMot:%d, iAuto:%d", MOT[iMot].ReadPos(), Prod.TrayZ_Up[iAuto], iMot, iAuto);
                        NewRecordProcess("Auto3Z", str);
                    }
                    #endif
                    Task=1;
                    return true;
                }
                else
                {
                    Task=1000;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool AutoCylinderMiddle(int Part, int CylinderName, int CylinderNameMid, bool bReset)
{
    int &Task=iAutoTask[1][Part];
    int iAuto=Part+3;
    int iMot =iAutoZMot[Part], iAlarm=0;                                        //Steven 20230907 : For HT-9011UC
    bool bRet;

    if(bReset)                                                                  //Steven 20140409 : AutoCylinderXX Add Reset
    {
        iAutoTask[1][Part]=1;
        return false;
    }

    #ifdef DEBUG_AutoZMotor                                                     //RogerYang 20250512 rogertest for Auto3Z 非AutoRetest機型不能跑Mid
    if(USE_AUTO_RETEST==eartUninstall && IniConfig.bVTESTFunction==true)
    {
        ShowMyMessage("AutoZ軸測試，自動記錄state record，稍後請關閉程式");
        fMain->DoStateRecord(0, false);
        return false;
    }
    #endif

    switch(Task)
    {
        case 1:
            if(LOAD_Z_USE_MOTOR[iAuto])                                         //Steven 20200724 : 入Tray改用步進馬達移動後要有delay
            {
                bRet=MOT[iMot].MotorMove(Prod.TrayZ_Mid[iAuto]);
                Task=1000;
                break;
            }
            Cylinder[CylinderName].On();
            if(Cylinder[CylinderNameMid].Enable)                                //Steven 20250312 : Add protection
                Cylinder[CylinderNameMid].Off();                                //kevin 20120718 修改輸入氣缸因代號位置不同共用程式
            bAuto2Pause[Part]=true;                                             //jou 2011-02-21 start : magazine沒動作會alarm
            Task=100;
            break;
        case 100:
            #ifdef SOFT_SIMULTE
            {
                Task=1;
                return true;
            }
            #else
            {
                if(bHandlerPause ||                                             //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
                   bAuto2Pause[Part]==true)                                     //jou 2011-02-21 start : magazine沒動作會alarm
                {
                    bAuto2Pause[Part]=false;
                    if(Cylinder[CylinderName].Enable==true)
                    {
                        iAlarm=Cylinder[CylinderName].OnAlarmTime;
                    }
                    else
                    {
                        iAlarm=Cylinder[CylinderNameMid].OnAlarmTime;
                    }

                    if(iAlarm<300)
                    {
                        iAlarm=300;
                    }
                    AutoTime[1][Part].Set0_1SecAndOn(iAlarm);
                }

                if(bARTUnloaderUseTwoCylin(Part))                               //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                {
                    if(Cylinder[CylinderName].OnStatus()==false &&
                       Cylinder[CylinderNameMid].OnStatus())
                    {
                        AutoTime[1][Part].SetSecAndOn(Ld_UldDelayTime.LD_StackMiddLockDelay);
                        Task=200;
                    }
                    else if(AutoTime[1][Part].Off())
                    {
                        ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Middle error", Cylinder[CylinderName].CylinderName+"上升到中間錯誤", "CylinderMiddle 100_1");   ///Sam 20200630 : Add Log
                        iAutoTask[1][Part]=1;
                        Task=1;
                    }
                }
                else
                {
                    if(Cylinder[CylinderName].OnStatus() &&
                       Cylinder[CylinderNameMid].OnStatus()==false)             //kevin 20120718 修改輸入氣缸因代號位置不同共用程式
                    {
                        AutoTime[1][Part].SetSecAndOn(Ld_UldDelayTime.LD_StackMiddLockDelay);
                        Task=200;
                    }
                    else if(AutoTime[1][Part].Off())
                    {
                        ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Middle error", Cylinder[CylinderName].CylinderName+"上升到中間錯誤", "CylinderMiddle 100_2");   ///Sam 20200630 : Add Log
                        iAutoTask[1][Part]=1;
                        Task=1;
                    }
                }
                break;
            }
            #endif
         case 200:
            if(AutoTime[1][Part].Off())
            {
                CylinderAlarmTime[1].SetSecAndOn(10);                           //kevin 20211116 add Z CylinderAlarmTime
                Task++;
            }
            break;
         case 201:
            if(CylinderAlarmTime[1].Off())                                      //kevin 20211116 add alarm
            {
                ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Middle error", Cylinder[CylinderName].CylinderName+"上升到中間錯誤", "CylinderMiddle 100_2");   ///Sam 20200630 : Add Log
                iAutoTask[1][Part]=1;
                Task=1;
                return false;
            }
            if(bARTUnloaderUseTwoCylin(Part))                                   //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
            {
                if(!(Cylinder[CylinderName].OnStatus()==false &&
                     Cylinder[CylinderNameMid].OnStatus()))
                {
                    return false;
                }
                else if(AutoTime[1][Part].Off())
                {
                    iAutoTask[1][Part]=1;
                    return true;
                }
            }
            else
            {
                if(Cylinder[CylinderName].OffStatus() ||
                   Cylinder[CylinderNameMid].OnStatus())                        //kevin 20120718
                {
                    Task=1;
                    return false;
                }
                else if(AutoTime[1][Part].Off())
                {
                    iAutoTask[1][Part]=1;
                    return true;
                }
            }
            break;
        case 1000:                                                              //Steven 20200724 : 入Tray改用步進馬達移動後要有delay
            bRet=MOT[iMot].MotorMove(Prod.TrayZ_Mid[iAuto]);
            if(bRet)
            {
                AutoTime[1][Part].SetSecAndOn(Ld_UldDelayTime.LD_StackMiddLockDelay);
                Task=1100;
            }
            break;
        case 1100:
            if(AutoTime[1][Part].Off())
            {
                if(MOT[iMot].ReadPos()==Prod.TrayZ_Mid[iAuto])
                {
                    Task=1;
                    return true;
                }
                else
                {
                    Task=1000;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
//kevin 20120718 修改輸入氣缸因代號位置不同共用程式
//------------------------------------------------------------------------------
bool AutoCylinderLower(int Part, int CylinderName, int CylinderNameMid, bool bReset)
{
    int &Task       =iAutoTask[2][Part];
    int iAuto       =Part+3;
    int iMot        =iAutoZMot[Part], iAlarm=0;                                 //Steven 20230907 : For HT-9011UC
    bool bRet;
    AnsiString str;

    if(bReset)                                                                  //Steven 20140409 : AutoCylinderXX Add Reset
    {
        iAutoTask[2][Part]=1;
        return false;
    }

#ifndef SOFT_SIMULTE
    bool bOffStatus=false, bMidOnStatus=false;                                  //Ifor 20200506 : 避免無使用AutoUP汽缸發生異常

    if(Cylinder[CylinderName].OffSenEnable)
    {
        bOffStatus=Cylinder[CylinderName].OffStatus();
    }
    else
    {
        bOffStatus=true;
    }

    if(Cylinder[CylinderNameMid].OnSenEnable==true)
    {
        bMidOnStatus=Cylinder[CylinderNameMid].OnStatus();
    }
    else
    {
        bMidOnStatus=false;
    }
#endif

    switch(Task)
    {
        case 1:
            if(Ld_UldDelayTime.LD_BeforeDownDelay!=0.0)                         //Steven 20240215 : 下降前Delay
            {
                AutoTime[2][Part].SetSecAndOn(Ld_UldDelayTime.LD_BeforeDownDelay);
                Task=10;
            }
            else
            {
                Task=50;
            }
            break;
        case 10:
            if(AutoTime[2][Part].Off())
            {
                Task=50;
            }
            else
            {
                break;
            }
        case 50:
            if(LOAD_Z_USE_MOTOR[iAuto])                                         //Steven 20200724 : 入Tray改用步進馬達移動後要有delay
            {
                bRet=MOT[iMot].MotorMove(0);
                Task=1000;
                break;
            }
            Cylinder[CylinderName].Off();
            Cylinder[CylinderNameMid].Off();                                    //kevin 20120718 修改輸入氣缸因代號位置不同共用程式
            bAuto2Pause[Part]=true;                                             //jou 2011-02-21 start : magazine沒動作會alarm
            Task=100;
            break;
        case 100:
            #ifdef SOFT_SIMULTE
            {
                Task=1;
                return true;
            }
            #else
            {
                if(bHandlerPause ||                                             //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
                   bAuto2Pause[Part]==true)                                     //jou 2011-02-21 start : magazine沒動作會alarm
                {
                    bAuto2Pause[Part]=false;
                    if(Cylinder[CylinderName].Enable==true)                     //JerryYang 20250314
                    {
                        iAlarm=Cylinder[CylinderName].OffAlarmTime;
                    }
                    else
                    {
                        iAlarm=Cylinder[CylinderNameMid].OffAlarmTime;
                    }

                    if(iAlarm<300)
                    {
                        iAlarm=300;
                    }
                    AutoTime[2][Part].Set0_1SecAndOn(iAlarm);
                }

                if(bARTUnloaderUseTwoCylin(Part))                               //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                {
                    if(Cylinder[CylinderNameMid].OffStatus())                   //JerryYang 20201117 : 修正汽缸下降未到位發生hang up
                    {
                        AutoTime[2][Part].SetSecAndOn(Ld_UldDelayTime.LD_LiftDownDelay);
                        Task=200;
                    }
                    else if(AutoTime[2][Part].Off())
                    {
                        ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Down error", Cylinder[CylinderName].CylinderName+"下降錯誤", "AutoCylinderLower1 100");
                        iAutoTask[2][Part]=1;
                        Task=1;
                    }
                }
                else if(bARTUnloaderUseOneCylin(Part))                          //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
                {
                    if(bOffStatus)
                    {
                        AutoTime[2][Part].SetSecAndOn(Ld_UldDelayTime.LD_LiftDownDelay);
                        Task=200;
                    }
                    else if(AutoTime[2][Part].Off())
                    {
                        ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Down error", Cylinder[CylinderName].CylinderName+"下降錯誤", "AutoCylinderLower2 100");
                        iAutoTask[2][Part]=1;
                        Task=1;
                    }
                }
                else
                {
                    if(bOffStatus==true &&
                       bMidOnStatus==false)                                     //kevin 20120718 修改輸入氣缸因代號位置不同共用程式
                    {
                        AutoTime[2][Part].SetSecAndOn(Ld_UldDelayTime.LD_LiftDownDelay);
                        Task=200;
                    }
                    else if(AutoTime[2][Part].Off())
                    {
                        str.sprintf("AutoCylinderLower3 %s.IsOff=%d, %s.IsOn=%d", Cylinder[CylinderName].CylinderName, bOffStatus, Cylinder[CylinderNameMid].CylinderName, bMidOnStatus);
                        ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Down error", Cylinder[CylinderName].CylinderName+"下降錯誤", str);
                        iAutoTask[2][Part]=1;
                        Task=1;
                    }
                }
                break;
            }
            #endif
         case 200:
            if(AutoTime[2][Part].Off())
            {
                CylinderAlarmTime[2].SetSecAndOn(10);                           //kevin 20211116 add Z CylinderAlarmTime
                Task++;
            }
            break;
         case 201:
            #ifndef SOFT_SIMULTE
            if(CylinderAlarmTime[2].Off())                                      //kevin 20211116 add alarm
            {
                str.sprintf("AutoCylinderLower3 %s.IsOff=%d, %s.IsOn=%d", Cylinder[CylinderName].CylinderName, bOffStatus, Cylinder[CylinderNameMid].CylinderName, bMidOnStatus);
                ShowMyMessage(Cylinder[CylinderName].CylinderName+" Lifter Down error", Cylinder[CylinderName].CylinderName+"下降錯誤", str);
                iAutoTask[2][Part]=1;
                Task=1;
                return false;
            }
            #endif
            if(bARTUnloaderUseTwoCylin(Part))                                   //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
            {
                if(Cylinder[C_Auto_Selector[Part]].OffStatus())                 //kevin 20220527 change sensor
                {
                    iAutoTask[2][Part]=1;
                    return true;
                }
            }
            else if(bARTUnloaderUseOneCylin(Part))                              //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
            {
                if(!Cylinder[CylinderName].OffStatus())                         //Steven 20211201 : 先改回來
                {
                    Task=1;
                    return false;
                }
                else if(AutoTime[2][Part].Off())
                {
                    iAutoTask[2][Part]=1;
                    return true;
                }
            }
            else
            {
                if(Cylinder[CylinderNameMid].OnStatus() ||
                   Cylinder[CylinderName].OnStatus())                           //kevin 20120718 修改輸入氣缸因代號位置不同共用程式
                {
                    Task=1;
                    return false;
                }
                else if(AutoTime[2][Part].Off())
                {
                    iAutoTask[2][Part]=1;
                    return true;
                }
            }
            break;
        case 1000:                                                              //Steven 20200724 : 入Tray改用步進馬達移動後要有delay
            bRet=MOT[iMot].MotorMove(0);
            if(bRet)
            {
                AutoTime[2][Part].SetSecAndOn(Ld_UldDelayTime.LD_LiftDownDelay);
                Task=1100;
            }
            break;
        case 1100:
            if(AutoTime[2][Part].Off())
            {
                if(MOT[iMot].ReadPos()==0)
                {
                    Task=1;
                    return true;
                }
                else
                {
                    Task=1000;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
TQPF_Timer LoopEdgeDelay[MAX_AUTO_TRAY];
bool DoAutoTrayEdgeCylinderLoop(int iAuto)
{
    static bool EdgePushLoop[MAX_AUTO_TRAY]={false, false, false, false, false, false};
    static bool bFirst=true;

    if(IniConfig.bP13EnableAutoTrayEdgePushCylinderLoop==false)                 //wei 20160309 LastSet-->IniConfig
        return false;

    if(bWaitOutArmCheckCylin==true)                                             //jou 20240131 : 修正out arm 與 auto tray互卡衝突hang up
        return true;

    if(bFirst==true)
    {
        for(int i=eAuto1; i<=iAutoRight; i++)
        {
            int iA=iAutoIndex[i];
            LoopEdgeDelay[iA].Set0_1SecAndOn(IniConfig.iP13EdgePushCylinderLoopDelay);
        }
        bFirst=false;
    }

    if(MOT[iMMAuto[iAuto]].fHasTray &&
       MOT[iMMAuto[iAuto]].Tray.HasIC() &&
       MOT[iMMAuto[iAuto]].FullIC()==false)
    {
        if(LoopEdgeDelay[iAuto].Off())
        {
            LoopEdgeDelay[iAuto].Set0_1SecAndOn(IniConfig.iP13EdgePushCylinderLoopDelay);
            EdgePushLoop[iAuto]=!EdgePushLoop[iAuto];

            if(EdgePushLoop[iAuto]==true)
                Cylinder[C_AutoEdgePush[iAuto]].On();
            else
                Cylinder[C_AutoEdgePush[iAuto]].Off();
        }
    }

    return true;
}
//------------------------------------------------------------------------------
// 2011.10.19 , Q_Q V207C new Tray Vibration{
TQPF_Timer NewLoopEdgeDelay;
TQPF_Timer NewLoopEdgeOnDelay;
void NewDoAutoTrayEdgeCylinderLoop()                                            //JerryYang 20170531 (wei) 敲擊方式改成可以分別設定on off時間
{
    static bool EdgePushLoop=false;
    static bool bFirst=true;
    if(IniConfig.bP13EnableAutoTrayEdgePushCylinderLoop==false)                 //wei 20160309 LastSet-->IniConfig
        return;
    if(bFirst==true)
    {
        NewLoopEdgeDelay.Set0_1SecAndOn(IniConfig.iP13EdgePushCylinderLoopDelay);
        EdgePushLoop=false;
        bFirst=false;
    }

    if(NewLoopEdgeDelay.Off() && EdgePushLoop==false)
    {
        NewLoopEdgeOnDelay.Set0_1SecAndOn(IniConfig.iP13EdgePushCylinderOnDelay);
        EdgePushLoop=true;
        Cylinder[C_TrayVibration].On();
        IniConfig.iVibratorUnloader=IniConfig.iVibratorUnloader+int(IniConfig.iP13EdgePushCylinderOnDelay/10.0);  //JerryYang 20200612 振動馬達作動時間累計
    }

    if(NewLoopEdgeOnDelay.Off() && EdgePushLoop==true)
    {
        NewLoopEdgeDelay.Set0_1SecAndOn(IniConfig.iP13EdgePushCylinderLoopDelay);
        EdgePushLoop=false;
        Cylinder[C_TrayVibration].Off();
    }
}
// 2011.10.19 , Q_Q V207C new Tray Vibration }
//------------------------------------------------------------------------------
void PushUnLoaderTrayInAverageTime(DWORD st, int pos)
{
    static DWORD Buffer[MAX_AUTO_TRAY][5]={{9999,9999,9999,9999,9999},
                                           {9999,9999,9999,9999,9999},
                                           {9999,9999,9999,9999,9999},
                                           {9999,9999,9999,9999,9999},
                                           {9999,9999,9999,9999,9999},
                                           {9999,9999,9999,9999,9999}};

    DWORD sum=0;
    int ct=0;
    for(int i=0; i<4; i++)
        Buffer[pos][i]=Buffer[pos][i+1];
    Buffer[pos][4]=st;
    if(st<500 || st>8000)
        return;

    for(int i=0; i<5; i++)
    {
        if(Buffer[pos][i]!=9999)
        {
            ct++;
            sum+=Buffer[pos][i];
        }
    }
    if(ct!=0)
    {
        LastSet.iUnLoaderTraySimulateTime[pos]=sum/ct;
    }
}
//------------------------------------------------------------------------------
void TrayMoveIn(bool bMove, int iAxis, int iPos)                                //Steven 20200603 : Loader入Tray改步進 for ART
{
    if(iAxis<0 || iAxis>=MAX_TRACK)
        return;

    if(TRAY_ARM_MODE==eUnderCoveyor)                                            //Frank 20230419
    {
        if(bMove)
        {
            Cylinder[iTrackFloodgate[iAxis]].On();
            if(Cylinder[iTrackFloodgate[iAxis]].OnSensor()==false)
                return;
        }
    }

    if(bMove)
    {                                                                           //Sam 20210325 : 修正整合 tray move
        if(iAxis==0 &&
           ((INSTALL_OCR!=eocrUninstal &&
             CosFunction.bTrayOCR) ||                                           //wei 20150925 待機位置改道 Color
             INSTALL_OCR_YMot==eocrYMotInstal))
        {
            MOT[MLoaderY].MotorMove(iPos);
        }
        else if(LOAD_Y_USE_MOTOR[iAxis]==true)                                  //20260325 Jimmychiu : Y軸步進馬達無特殊功能專用
        {
            int iStepMotor[6]={MLoaderY, MEmptyY, MColorY, MAuto1Y, MAuto2Y, MAuto3Y}; //AI(ht9045-v899) 20260423: dispatch stepper by track
            int iMot=iStepMotor[iAxis];
            MOT[iMot].SetSpeed(100);
            if(MOT[iMot].ReadPos()>=69900)
                MOT[iMot].PCIL132_SetPos(0);
            MOT[iMot].MotorMove(70000);
        }
        else if(LoaderUnload_StepMotor==1)
        {
            if(SW[iInSwitch[iAxis]].Status()==false &&
               SW[iOutSwitch[iAxis]].Status()==true)                            //步進馬達如果直接反向需要睡一下
            {
                SW[iInSwitch[iAxis]].Off();
                SW[iOutSwitch[iAxis]].Off();
                MySleepEx(100, false);
            }
            SW[iInSwitch[iAxis]].On();
            SW[iOutSwitch[iAxis]].Off();
        }
        else
        {
            if(iAxis<3)
            {
                SW[iInSwitch[iAxis]].On();
                SW[iOutSwitch[iAxis]].Off();
            }
            else
            {
                SW[iInSwitch[iAxis]].On();
                SW[iOutSwitch[iAxis]].On();
            }
        }
    }
    else
    {
        if(iAxis==0)
        {
            if((INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR) ||
               INSTALL_OCR_YMot==eocrYMotInstal ||            //wei 20150925 待機位置改道 Color
               LOAD_Y_USE_MOTOR[iAxis]==true)               
            {
                MOT[MLoaderY].PCIL132_StopMotor();
            }
        }
        else if(iAxis>=1 && iAxis<=5 && LOAD_Y_USE_MOTOR[iAxis]==true)          //AI(ht9045-v899) 20260423: stop dispatched stepper for empty/color/auto1~3
        {
            int iStepMotor[6]={MLoaderY, MEmptyY, MColorY, MAuto1Y, MAuto2Y, MAuto3Y};
            MOT[iStepMotor[iAxis]].PCIL132_StopMotor();
        }

        SW[iInSwitch[iAxis]].Off();
        SW[iOutSwitch[iAxis]].Off();
    }
}
//------------------------------------------------------------------------------
void TrayCylinMoveIn(int iAxis)                                                 //Front to Rear
{
    if(iAxis<0 || iAxis>=MAX_TRACK)
        return;

    Cylinder[iAutoPush[iAxis]].Off();
    Cylinder[iAutoBack[iAxis]].On();
}
//------------------------------------------------------------------------------
void TrayCylinMoveOut(int iAxis)                                                //Rear to Front
{
    if(iAxis<0 || iAxis>=MAX_TRACK)
        return;

    Cylinder[iAutoPush[iAxis]].On();
    Cylinder[iAutoBack[iAxis]].Off();
}
//------------------------------------------------------------------------------
void TrayMoveOut(bool bMove, int iAxis, int iPos)                               //Steven 20200603 : Loader入Tray改步進 for ART
{
    if(iAxis<0 || iAxis>=MAX_TRACK)
        return;

    if(TRAY_ARM_MODE==eUnderCoveyor)                                            //Frank 20230419
    {
        if(bMove)
        {
            Cylinder[iTrackFloodgate[iAxis]].On();
            if(Cylinder[iTrackFloodgate[iAxis]].OnSensor()==false)
                return;
        }
    }

    if(bMove)                                                                   //Sam 20210325 : 修正整合 tray move
    {
        if(iAxis==0 &&
           ((INSTALL_OCR!=eocrUninstal &&
            CosFunction.bTrayOCR) ||                                            //wei 20150925 待機位置改道 Color
            INSTALL_OCR_YMot==eocrYMotInstal))
        {
            MOT[MLoaderY].MotorMove(iPos);
        }
        else if(LOAD_Y_USE_MOTOR[iAxis]==true)                                  //20260325 Jimmychiu : Y軸步進馬達無特殊功能專用
        {
            int iStepMotor[6]={MLoaderY, MEmptyY, MColorY, MAuto1Y, MAuto2Y, MAuto3Y}; //AI(ht9045-v899) 20260423: dispatch stepper by track
            int iMot=iStepMotor[iAxis];
            MOT[iMot].SetSpeed(100);
            if(MOT[iMot].ReadPos()<-69900)
                MOT[iMot].PCIL132_SetPos(0);
            MOT[iMot].MotorMove(-70000);
        }
        else if(LoaderUnload_StepMotor==1)
        {
            if(SW[iInSwitch[iAxis]].Status()==true &&
               SW[iOutSwitch[iAxis]].Status()==false)                           //步進馬達如果直接反向需要睡一下
            {
                SW[iInSwitch[iAxis]].Off();
                SW[iOutSwitch[iAxis]].Off();
                MySleepEx(100, false);
            }

            SW[iInSwitch[iAxis]].Off();
            SW[iOutSwitch[iAxis]].On();
        }
        else
        {
            if(iAxis<3)
            {
                SW[iInSwitch[iAxis]].On();
                SW[iOutSwitch[iAxis]].On();
            }
            else
            {
                SW[iInSwitch[iAxis]].Off();
                SW[iOutSwitch[iAxis]].On();
            }
        }
    }
    else
    {
        if(iAxis==0)
        {
            if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR  ||             //wei 20150925 待機位置改道 Color
               INSTALL_OCR_YMot==eocrYMotInstal                   ||
               LOAD_Y_USE_MOTOR[iAxis]==true)
            {
                MOT[MLoaderY].PCIL132_StopMotor();
            }
        }
        else if(iAxis>=1 && iAxis<=5 && LOAD_Y_USE_MOTOR[iAxis]==true)          //AI(ht9045-v899) 20260423: stop dispatched stepper for empty/color/auto1~3
        {
            int iStepMotor[6]={MLoaderY, MEmptyY, MColorY, MAuto1Y, MAuto2Y, MAuto3Y};
            MOT[iStepMotor[iAxis]].PCIL132_StopMotor();
        }

        SW[iInSwitch[iAxis]].Off();
        SW[iOutSwitch[iAxis]].Off();
    }
}
//------------------------------------------------------------------------------
int TrayMoveStatus(int iAxis, AnsiString sFun)                                  //0: 停止狀態, 1: Tray move in, 2: Tray move out  //Sam 20210325 : 整合 tray move 狀態
{
    bool bInSwitch=false, bOutSwitch=false;
    AnsiString asEn;
    if(iAxis<0 || iAxis>=MAX_TRACK)
        return -1;

    int iStepMotor[6]={MLoaderY, MEmptyY, MColorY, MAuto1Y, MAuto2Y, MAuto3Y};
    if(LOAD_Y_USE_MOTOR[iAxis]==true)
    {
        bool bRun=MOT[iStepMotor[iAxis]].Led[iInposLed];
        if(bRun==true)
        {
            int pos=MOT[iStepMotor[iAxis]].ReadPos();
            bInSwitch= (pos>0);
            bOutSwitch=(pos<0);
        }
    }
    else
    {
        bInSwitch =SW[iInSwitch[iAxis]].Status();
        bOutSwitch=SW[iOutSwitch[iAxis]].Status();
    }

    if(LoaderUnload_StepMotor==1)
    {
        if(bInSwitch==true && bOutSwitch==false)
            return 1;
        else if(bInSwitch==false && bOutSwitch==true)
            return 2;
    }
    else
    {
        if(iAxis<3)
        {
            if(bInSwitch==true && bOutSwitch==false)
                return 1;
            else if(bInSwitch==true && bOutSwitch==true)
                return 2;
        }
        else
        {
            if(bInSwitch==true && bOutSwitch==true)
                return 1;
            else if(bInSwitch==false && bOutSwitch==true)
                return 2;
        }
    }

    if(bInSwitch==false && bOutSwitch==false)
    {
        return 0;                                                               //停止狀態
    }
    else
    {
        asEn.sprintf("Tray move status error, Axis=%d, InSw=%s, OutSw=%s, Fun=%s", iAxis, BoolToStr(bInSwitch), BoolToStr(bOutSwitch), sFun);
        ShowMyMessage(asEn, "");
        SW[iInSwitch[iAxis]].Off();                                             //狀態錯誤直接強制停止
        SW[iOutSwitch[iAxis]].Off();
        return 0;                                                               //停止狀態
    }
}
//==============================================================================
//kevin 20211108 抽測數量到達收TRAY pig 2011.05.25 LoaderTrackCCW start
//==============================================================================
int iDoLoaderTrayFeedTask=1;
void InitDoLoaderTrayFeedTask()
{
    iDoLoaderTrayFeedTask=1;
}
//------------------------------------------------------------------------------
bool DoLoaderTrayFeed()
{
    int &Task=iDoLoaderTrayFeedTask;                                            //2013.02.19 Q_Q RTC INDEX MOTION CHANGE
    int ret=0;

    switch(Task)
    {
        case 1:
            if(MOT[MMTrayY_Car].fHasTray || Sen[SnLoaderCarHasTray].IsOn())     //Load Buffer Has Tray
            {
                InitUnLoadNewLoaderTrayTask();
                Task=1000;
            }
            else if(MOT[MMTrayY].fHasTray || Sen[SnLoaderSureTray].IsOn())      //kevin 20120528  //Has Tray
            {
                InitAutoLoaderTask();
                Task=2000;
            }
            else
            {
                return true;
            }
            break;
        case 1000:                                                              //TRAY 上升到 STACK
            ret=DoUnLoadNewLoaderToStack(false);
            if(ret)
            {
                SW[SenLoaderCCWDete].Off();
                MOT[MMTrayY_Car].ClearTray(__FUNC__);                           //Seven 20260121 : add for loader tray feed
                Task=1;
            }
            break;
        case 2000:
            if(DoLoaderTrayToFront())
            {
                Task=1;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
