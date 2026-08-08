// =============================================================================
//  ScanBtnThread.cpp  --  the "real-time CCD stop" button/sensor poll thread.
//
//  Faithful translation of golden ScanBtnThread.cpp (121 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W3) 20260807
//  Translation wave: W906-PT-W3, group "misc_a".  Sibling of the other two
//  OFFLINE-INERT TThread wrappers this tree already ported -- uHeaterThread.h/
//  .cpp's THeaterThread and acarry.cpp's HThreadCtrlShuttle -- reusing that
//  same idiom verbatim rather than inventing a vclcompat/TThread.h.
//
//  ROLE: ScanBtn::Execute() loops waiting for eFreeArm then eStart (both
//  Win32 auto-reset-style events created in the ctor), and once both are
//  signalled, polls Sen[SnRealTimeCCDStop]: if it is on, issues a Galil
//  "stop all axes" command on MTestY1 (VS0;SP0,0,0,0;) and calls
//  SetArmState(true) (which ResetEvent(eFreeArm)s and latches bStopArm),
//  else clears bRealTimeCCDStop.  Stop()/Start()/EndThread() are the
//  external control surface (Reset/Set the eStart event, latch bEndThread);
//  GetRunState()/GetArmState() are plain accessors.
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-substrate
//  -----------------------------------------------
//   ACTIVE (faithful, verbatim -- all 9 golden functions, whole file):
//     ScanBtn::ScanBtn     golden :29-40
//     ScanBtn::~ScanBtn    golden :42-53
//     ScanBtn::Stop        golden :55-59
//     ScanBtn::Start       golden :61-65
//     ScanBtn::EndThread   golden :67-72
//     ScanBtn::GetRunState golden :74-77
//     ScanBtn::SetArmState golden :79-90
//     ScanBtn::GetArmState golden :92-95
//     ScanBtn::Execute     golden :97-120  (dead code offline -- see header)
//   SATISFIED-BY-SUBSTRATE (already real in this tree, nothing shimmed here):
//     MyDBIProcess(AnsiString,AnsiString) -- aHotPlateSubstrate.h:924/.cpp
//       (golden ~cScanBtn's dtor catch-block; SAME 2-arg overload ATC/
//       TCPData.cpp:107 already uses, and for the SAME reason: including
//       cMyDB.h's 3-arg-with-default overload in the SAME TU would make
//       `MyDBIProcess("Exception","~ScanBtn")` AMBIGUOUS -- so, matching
//       that precedent, this file does NOT include cMyDB.h),
//     Sen[]/TMySensor::IsOn (mysensor.h), SnRealTimeCCDStop (cmydef.h,
//       extern const int), MOT[]/TTrayMotor::Gali_Command (Motor/mymotor.h),
//       MTestY1 (cmydef.h, extern const int).
//
//  GATE REGISTER -- none.  Every symbol this file's real bodies touch was
//  grep-verified present with a compiled definition somewhere in the port
//  tree before writing (MyDBIProcess/Sen[]/SnRealTimeCCDStop/MOT[]/
//  Gali_Command/MTestY1 -- all confirmed 20260807).
//
//  VCL/Borland conversions: #pragma hdrstop + #pragma package(smart_init)
//  dropped (golden :2 and :5).  golden :10 `#include "rs232.h"` DROPPED --
//  grepped this file's own body for any RS232-named symbol: zero hits: this
//  golden include is dead weight (ScanBtn talks to Sen[]/MOT[] only, never
//  to a serial port), and rs232.h has no port in this tree to include even
//  if it were needed.  golden :7-9 (mymotor.h/mycylin.h/mysensor.h) and :11
//  (cmydef.h) map to this tree's Motor/mymotor.h / mycylin.h / mysensor.h /
//  cmydef.h respectively -- mycylin.h is ALSO dropped: grepped this file's
//  body for any Cylinder[]/TMyCylinder use: zero (same "dead god-header
//  entry" treatment RotateKit/aRotateKIT.cpp gives its own unused
//  golden-include lines). golden :39 `bRealTimeCCDStop;` (a bare
//  expression-statement with no effect, presumably an incomplete
//  `bRealTimeCCDStop=false;` the original author never finished) is kept
//  VERBATIM -- HARD RULE 3 forbids "improving" golden, so the ctor really
//  does evaluate `bRealTimeCCDStop;` and discard it, leaving the member
//  default-uninitialized exactly as golden does.  No __fastcall/__property/
//  __published anywhere in golden's body (already stripped from the
//  declarations in ScanBtnThread.h; only golden's OWN definitions carried
//  `__fastcall` on the .cpp side, dropped here the same way).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
#include "MachineDefine.h"          // de-VCL'd include hub (vclcompat umbrella + portable STL + using namespace std; also <windows.h> for CreateEvent/WaitForSingleObject/SetEvent/ResetEvent/CloseHandle)

#include "ScanBtnThread.h"          // this unit's own contract

#include "Motor/mymotor.h"          // MOT[] (TTrayMotor::Gali_Command)
#include "mysensor.h"               // Sen[] (TMySensor::IsOn)
#include "cmydef.h"                 // SnRealTimeCCDStop, MTestY1 (extern const int)
#include "aHotPlateSubstrate.h"     // MyDBIProcess(AnsiString,AnsiString) -- golden cMyDB.h, 2-arg overload only (see file head: NOT cMyDB.h, to avoid the 3-arg-default ambiguity)
//AI(W906-PT-W3) 20260807: golden `__FUNC__` (BCB6 builtin, golden :109) -- same
//  shim canary_support.h:45 already established tree-wide (`#define __FUNC__
//  __func__`); reproduced locally, guarded, rather than pulling in
//  canary_support.h's much heavier LastSet surface for one macro (same
//  precedent EJ1N/uDTME08Control.cpp:92-98 already set).
#ifndef __FUNC__
#define __FUNC__ __func__
#endif
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

ScanBtn::ScanBtn(bool CreateSuspended)                                          //AI(W906-PT-W3) 20260807: golden ":30  : TThread(CreateSuspended)" dropped -- no VCL Classes::TThread offline (see ScanBtnThread.h); CreateSuspended is otherwise unused, exactly as golden handed it to the TThread base and nothing else touched it
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
ScanBtn::~ScanBtn()
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
void ScanBtn::Stop()
{
    ResetEvent(eStart);
    bStart=false;
}
//---------------------------------------------------------------------------
void ScanBtn::Start()
{
    SetEvent(eStart);
    bStart=true;
}
//---------------------------------------------------------------------------
void ScanBtn::EndThread()
{
    SetEvent(eStart);
    SetEvent(eFreeArm);
    bEndThread=true;
}
//---------------------------------------------------------------------------
bool ScanBtn::GetRunState()
{
    return bStart;
}
//---------------------------------------------------------------------------
void ScanBtn::SetArmState(bool bStop)
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
bool ScanBtn::GetArmState()
{
    return bStopArm;
}
//---------------------------------------------------------------------------
void ScanBtn::Execute()                                                         //AI(W906-PT-W3) 20260807: NEVER invoked offline -- no real OS thread spawns it (nothing in this wave's scope resumes a ScanBtn); kept compiling/callable and VERBATIM for a future real-thread wave, same treatment uHeaterThread.cpp:366-379 gave THeaterThread::Execute
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
