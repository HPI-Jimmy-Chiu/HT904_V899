#include "MachineDefine.h"
#pragma hdrstop

#include "ScanBtnThread.h"
#pragma package(smart_init)

#include "mymotor.h"
#include "mycylin.h"
#include "mysensor.h"
//#include "main.h"
#include "csystem.h"
#include "cmydef.h"
#include "rs232.h"
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Unit1::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall ScanBtn::ScanBtn(bool CreateSuspended)
   : TThread(CreateSuspended)
{
    eStart = CreateEvent(NULL,true,false,NULL);
    eFreeArm = CreateEvent(NULL,true,true,NULL);

    bStart=false;
    bStopArm=false;
    bEndThread=false;
    bThreadFlag=false;
    bRealTimeCCDStop;   //Steven 20120505 : 取代Aled
}
//---------------------------------------------------------------------------
__fastcall ScanBtn::~ScanBtn()
{
    try
    {
        CloseHandle(eStart);
        CloseHandle(eFreeArm);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~ScanBtn");
    }
}
//---------------------------------------------------------------------------
void __fastcall ScanBtn::Stop()
{
    ResetEvent(eStart);
    bStart=false;
}
//---------------------------------------------------------------------------
void __fastcall ScanBtn::Start()
{
    SetEvent(eStart);
    bStart=true;
}
//---------------------------------------------------------------------------
void __fastcall ScanBtn::EndThread()
{
    SetEvent(eStart);
    SetEvent(eFreeArm);
    bEndThread=true;
}
//---------------------------------------------------------------------------
bool __fastcall ScanBtn::GetRunState()
{
    return bStart;
}
//---------------------------------------------------------------------------
void __fastcall ScanBtn::SetArmState(bool bStop)
{
    if(bStop) //true
    {
        ResetEvent(eFreeArm); //false
    }
    else
    {
        SetEvent(eFreeArm); //true
    }
    bStopArm=bStop;
}
//---------------------------------------------------------------------------
bool __fastcall ScanBtn::GetArmState()
{
    return bStopArm;
}
//---------------------------------------------------------------------------
void __fastcall ScanBtn::Execute()
{
    do
    {
        if(bThreadFlag)     //Steven 20120202 : 加入Thread保護
        {
            //是否開始檢知
            WaitForSingleObject(eFreeArm,INFINITE);
            WaitForSingleObject(eStart,INFINITE);

            if(Sen[SnRealTimeCCDStop].IsOn())
            {
                MOT[MTestY1].Gali_Command("VS0;SP0,0,0,0;", __FUNC__);       //0512
                bRealTimeCCDStop=true;   //Steven 20120505 : 取代Aled
                SetArmState(true);
            }
            else
            {
                bRealTimeCCDStop=false;   //Steven 20120505 : 取代Aled
            }
        }
    }
    while(!bEndThread);
}
//---------------------------------------------------------------------------
