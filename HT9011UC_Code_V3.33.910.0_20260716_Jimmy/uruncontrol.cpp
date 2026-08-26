#include "MachineDefine.h"
#pragma hdrstop

#include "uruncontrol.h"

#include "csystem.h"
#include "cContact.h"
#include "cMydef.h"
#include "common.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TRunControl::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

TRunControl *MyThread;
extern  bool InitialOK;
__fastcall TRunControl::TRunControl(bool CreateSuspended)
    : TThread(CreateSuspended)
{
    bEnd=false;                                                                 //Steven 20110718 : 為了要可以關閉執行緒
}
//---------------------------------------------------------------------------
void __fastcall TRunControl::ThreadProcess(void)
{
    if(InitialOK)
    {
        MainProc();
    }
}
//---------------------------------------------------------------------------
void __fastcall TRunControl::Execute()
{
    //---- Place thread code here ----
    timeBeginPeriod(1);
    int ct=0;
    do
    {
        Synchronize(ThreadProcess);

//        MySleepEx(1,true);
        if(SystemStart==false || iHome==1 || fContact->fShow==true)
        {
            MySleepEx(1,true);
        }
        else
        {
            ct++;
            if(ct>2)
            {
                ct=0;
            }
            else
            {
                MySleepEx(1,true);
            }
        }
    } while (!bEnd);                                                            //do
//    } while (!Terminated);    //do
}
//---------------------------------------------------------------------------
void StartMainThread()
{
    MyThread->bEnd=false;                                                       //Steven 20110718 : 為了要可以關閉執行緒
    MyThread->Resume();
}
//------------------------------------------------------------------------------
void EndMainThread()
{
    LogSoftwareOffTime("FC: Wait MainThread");
    MyThread->bEnd=true;                                                        //Steven 20110718 : 為了要可以關閉執行緒
//    MyThread->WaitFor();    //Steven 20110718 : 為了要可以關閉執行緒
//    HeaterThread->Suspend();
    MyThread->Terminate();                                                      //kevin 20170421 (wei) : 為了要可以關閉執行緒
    MySleep(100);
}
