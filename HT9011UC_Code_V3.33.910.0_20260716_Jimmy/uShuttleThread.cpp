//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uShuttleThread.h"

#include "acarry.h"
#include "cprod.h"
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
//      void __fastcall Unit1::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

extern  bool InitialOK;
TShuttleThread *SHThread;

__fastcall TShuttleThread::TShuttleThread(bool CreateSuspended)
    : TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall TShuttleThread::ShuttleThreadProcess(void)
{
    if(InitialOK )
    {
        ShuttleProc();
    }
}
//---------------------------------------------------------------------------
void __fastcall TShuttleThread::Execute()
{
    int i, j,k;

    for(i=0; i<2; i++)
    {
        for(j=0; j<4; j++)
        {
            for(k=0;k<4;k++)
            {
                bShuttleHasIC[i][j][k]=false;
                bEnter[i][j][k]=false;
            }
        }
    }

    do{
        Synchronize( ShuttleThreadProcess);

        MySleepEx(1, true);
    } while (!Terminated);                                                      //do
}
//---------------------------------------------------------------------------
