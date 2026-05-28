#include "MachineDefine.h"
#pragma hdrstop

#include "asendic_Scanner.h"

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

#pragma package(smart_init)

//------------------------------------------------------------------------------
HTimer DoSupplyNewICTray_ScannerDelay;
bool DoSupplyNewICTray_Scanner()    //Supply IC from Loader car
{
    int &Task=SupplyNewIC_From_LoaderCar;
    int ret;

    switch(Task)
    {
        case 1:
            if(MOT[MMAuto1_Car].fHasTray==true && MOT[MMAuto1].fHasTray==false)
                Task=50;
            break;
        case 50:
            if(MOT[MMAuto1_Car].fHasTray==true && Sen[SnAuto1_Tray_Car].IsOff()==false)
            {
                Task=100;
            }
            else
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    Task=110;
                    break;
                }
                ret=ShowErrorMessage("JAM1113", K_RETRY|K_SKIP, MMAuto1_Car);   //Steven 20120926 : JAM1169 -> JAM1113
                if(ret==K_SKIP)
                {
                    MOT[MMAuto1_Car].ClearTray();
                    Task=1;
                    return true;
                }
            }
            break;
        case 100:
            if(MOT[MMAuto1_Car].fCanMove==false)
                break;
            SW[SwACAutoCW].On();
            DetectLoaderTime.SetMS(20000);
            DetectLoaderTime.On();
            Task=150;
            break;
        case 110:
            if(MOT[MMAuto1_Car].fCanMove==false)
                break;
            SW[SwACAutoCW].On();
            DetectLoaderTime.SetMS(2000);
            DetectLoaderTime.On();
            Task=160;
            break;
        case 150:
            if(Sen[SnAuto1TrayDetect].IsOff()==false)
            {
                SW[SwACAutoCW].Off();
                if(Sen[SnAuto1TrayDetect].IsOff()==false)//滑到定位
                {
                    Task=300;
                    break;
                }
                else
                {
                    SW[SwACAutoCW].On();
                }
            }
            if(DetectLoaderTime.Off())
            {
                ret=ShowErrorMessage("JAM1101", K_RETRY, MMAuto1);
                if(ret==K_RETRY)
                    Task=100;
            }
            break;
        case 160:
            if(DetectLoaderTime.Off())
            {
                SW[SwACAutoCW].Off();
                Task=300;
            }
            break;
        case 300:
            if(Cylinder[C_Auto1EdgePush].Enable)
            {
                Cylinder[C_Auto1EdgePush].On();
                DoSupplyNewICTray_ScannerDelay.SetMS(500);
                DoSupplyNewICTray_ScannerDelay.On();
                Task=400;
            }
            else
                Task=500;
            break;
        case 400:
            if(DoSupplyNewICTray_ScannerDelay.Off())
                Task=500;
            break;
        case 500:
            if(Cylinder[C_Auto1Side_Fixer].Push())
            {
                Cylinder[C_Auto1EdgePush].On();
                MOT[MMAuto1].SetTray(HAS_IC);   //Steven 20130909
                MOT[MMAuto1_Car].ClearTray();
                Task=1;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
// loader 分離出一個 Tray 不下降
//------------------------------------------------------------------------------
bool DoTrayZLoadTrayToWait_Scanner()
{
    int &Task=iTrayZLoadTrayToWaitTask;
    switch(Task)
    {
        case 1:
            if(MOT[MMAuto1Z].MotorMove(Prod.Auto1Z_Up))
            {
                MOT[MMAuto1Z].fHasTray=true;
                Task=100;
            }
            break;
        case 100:
            if(Cylinder[C_Auto1LoaderZ_Select].Push())
                Task=200;
            break;
        case 200:
            if(MOT[MMAuto1Z].MotorMove(Prod.Auto1Z_Down))
            {
                if(bUnLoaderPause)
                {
                    bUnLoaderPause=false;
                    ShowMyMessage("Loader split pause.", "Loader 分離暫停", "DoTrayZLoadTrayToWait_Scanner");
                    Task=200;
                    break;
                }
                Task=300;
            }
            break;
        case 300:
            if(Cylinder[C_Auto1LoaderZ_Select].Pop())
            {
                MOT[MMAuto1Z].fHasTray=true;
                Task=1;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool DoLoadNewICTray_Scanner()
{
    int ret;
    int &Task=LoadNewICTrayTask;
    switch(Task)
    {
        case 1:
            InitTrayZLoadTrayToWaitTask();
            if(MOT[MMAuto1].fHasTray==false)
            {
                Cylinder[C_Auto1Side_Fixer].Off();   //Open Fix Supply Try Fix Cylinder
                Cylinder[C_Auto1EdgePush].Off();
            }
            Task=100;
            break;
        case 100:
            if(DoTrayZLoadTrayToWait_Scanner())
                Task=300;
            break;
        case 300:
            if(MOT[MMAuto1Z].MotorMove(iAuto1ZeroPos))
                Task=400;
             break;
        case 400:
            if(Sen[SnAuto1_Tray_Car].IsOff()==false || LastSet.iRealDummy==DUMMY)
            {
                MOT[MMAuto1Z].fHasTray=false;
                MOT[MMAuto1_Car].InitNewTray(HAS_IC);
                MOT[MMAuto1_Car].fHasTray=true;
                Task=1;
                return true;
            }
            else
            {
                ret=ShowErrorMessage("JAM1113", K_RETRY|K_SKIP, MMAuto1_Car);   //Steven 20120926 : JAM1169 -> JAM1113
                if(ret==K_SKIP)
                {
                    MOT[MMAuto1Z].fHasTray=false;
                    Task=1;
                    return true;
                }
                else return 0;
            }
    }
    return false;
}
//------------------------------------------------------------------------------
bool DoLoad_Scanner()
{
    int ret,ct,per,tct,tpass,i;
    bool flag;
    int &Task=LoadTask;
    switch(Task)
    {
        case 1:
            #ifdef SOFT_SIMULTE
                Task=300;
                break;
            #else
                if(MOT[MMAuto1Z].fHasTray)
                {
                    MOT[MMAuto1_Car].SetTray(HAS_IC);   //Steven 20130909
                    MOT[MMAuto1Z].ClearTray();
                }
                flag=Sen[SnAuto1TrayDetect].IsOn();

                if(LastSet.iRealDummy==DUMMY && flag==false)
                {
                    Task=300;
                    break;
                }
                if(flag!=MOT[MMAuto1].fHasTray)
                {
                    if(MOT[MMAuto1].fHasTray && MOT[MTrayX].ReadPos()!=Prod.iXTrayAuto[0]) // tray miss
                        Task=200;
                    else
                        Task=100;              // more tray or sensor error
                    break;
                }
                else
                    Task=300;                  //ok
                break;
            #endif
        case 100:
            ret=ShowErrorMessage("JAM1168", K_RETRY|K_SKIP, MMAuto1);
            if(ret==K_SKIP)
            {
                MOT[MMAuto1].SetTray(HAS_IC);   //Steven 20130909
            }
            Task=1;
            break;
        case 200:
            ret=ShowErrorMessage("JAM1130", K_RETRY|K_SKIP, MMAuto1);   //Steven 20120926 : JAM1169 -> JAM1130
            if(ret==K_RETRY)
            {
                Task=1;
            }
            else
            {
                MOT[MMAuto1].ClearTray();   //Steven 20130909
                Task=300;
            }
            break;
        case 300:         //    from hear begin check
            flag=false;
            if(Sen[SnAuto1_Tray_Car].Enable)
            {
                flag=Sen[SnAuto1_Tray_Car].IsOn();
                if(flag==MOT[MMAuto1_Car].fHasTray)
                    Task=600;
                else if(flag)
                    Task=400; // has tray under load stage
                else
                {
                    if(LastSet.iRealDummy==DUMMY)
                        Task=600;
                    else
                        Task=500; // tray is lost
                }
            }
            else
            {
                Task=600;
            }
            break;
        case 400:
            ret=ShowErrorMessage("JAM1168", K_RETRY|K_SKIP, MMAuto1_Car);
            if(ret==K_SKIP)
            {
                MOT[MMAuto1_Car].SetTray(HAS_IC);   //Steven 20130909
            }
            Task=300;
            break;
        case 500:
            ret=ShowErrorMessage("JAM1130", K_RETRY|K_SKIP, MMAuto1_Car);   //Steven 20120926 : JAM1169 -> JAM1113
            if(ret==K_RETRY)
            {
                Task=300;
            }
            else
            {
                MOT[MMAuto1_Car].ClearTray();
                Task=600;
            }
            break;
        case 600:                                                               // START
            if(MOT[MMAuto1_Car].fHasTray && Sen[SnAuto1_Tray_Car].IsOff()==false) //因為客戶會把粹拿起來
            {
                InitSupplyNewIC_From_LoaderCarTask();
                Task=1000;
            }
            else if(MOT[MMAuto1_Car].fHasTray && LastSet.iRealDummy==DUMMY)
            {
                InitSupplyNewIC_From_LoaderCarTask();
                Task=1000;
            }
            else
                Task=800;
            break;
        case 800:
            if(iCleanOut || iOneCycle)   // no load new tray
            {
                Task=600;
                break;
            }
            if(fMain->ALed1->Value==false && MOT[MMAuto1_Car].fHasTray==false &&
                 MOT[MMAuto1].fHasTray==false)     // no any tray
            {
                ret=ShowErrorMessage("MES1121", K_RETRY|K_CLEAN_OUT, MMAuto1_Car);
                if(ret==K_CLEAN_OUT)
                    fMain->BtnCleanOutClick(fMain);
                Task=600;
                break;
            }
            if(fMain->ALed1->Value==false)
            {
                Task=600;
                break;
            }

            InitLoadNewICTrayTask();
            Task=900;
            break;
        case 900:
            if(DoLoadNewICTray_Scanner())
                Task=600;
            break;
        case 1000:
            if(DoSupplyNewICTray_Scanner())     //Supply IC from Loader car
                Task=1;
            break;
    }
    return false;
}
