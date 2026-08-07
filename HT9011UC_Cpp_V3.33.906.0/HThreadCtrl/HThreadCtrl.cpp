// =============================================================================
//  HThreadCtrl.cpp  --  generic Win32 thread wrapper (CreateThread spin-loop
//                       runner with a MySleep(1) CPU-yield throttle).
//
//  Faithful translation of golden HThreadCtrl/HThreadCtrl.cpp (102 lines,
//  BCB6, Big5/cp950 -- this file has NO Chinese text at all, so there is no
//  mojibake risk to check here).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("tiny" group -- five very small units).
//
//  ROLE: HThreadCtrl owns exactly one OS thread.  A caller wires a plain
//  `void(*)(LPVOID)` worker + its LPVOID parameter through Initial(), then
//  OpenThread() spins up ThreadProcess() on a raw Win32 thread, which calls
//  the worker back-to-back in a tight loop and yields the CPU with MySleep(1)
//  every `_sleep_delay_count`+1 iterations (SetSleepDelayCount, capped at 20).
//  CloseThread() flags the loop to stop, waits for it, and force-terminates
//  on WAIT_FAILED/WAIT_TIMEOUT as a last resort.  No state machine, no cursor.
//
//  WAVE SCOPE -- ACTIVE (faithful, verbatim -- all 7 golden functions):
//    HThreadCtrl::HThreadCtrl (ctor)      golden :8-16
//    HThreadCtrl::~HThreadCtrl (dtor)     golden :18-28
//    HThreadCtrl::Initial                 golden :30-39
//    HThreadCtrl::OpenThread              golden :41-50
//    HThreadCtrl::CloseThread             golden :52-74
//    HThreadCtrl::SetSleepDelayCount      golden :76-82
//    HThreadCtrl::ThreadProcess           golden :84-101
//  SATISFIED-BY-SUBSTRATE (already real in this tree, nothing shimmed here):
//    MySleep(DWORD)          -> common.cpp:1919 (declared common.h:366), real
//                               Sleep()-based body -- not a stub.
//    LogSoftwareOffTime(AnsiString) -> declared cmydef.h:5032, real (no-op)
//                               body at acarry_shims.cpp:176.  Golden's own
//                               body (main.cpp) is a software-off-time logger;
//                               the established no-op stand-in already carries
//                               every other CloseThread()-style caller in this
//                               tree (uHeaterThread.cpp:414, acarry.cpp:7285),
//                               so this file follows the same precedent rather
//                               than inventing a second logging path.
//    MyDBIProcess(AnsiString,AnsiString) -> real body aHotPlateSubstrate.cpp
//                               :1030, declared aHotPlateSubstrate.h:924.  Same
//                               2-arg (no __fastcall) stand-in this tree already
//                               uses for golden's cMyDB.h MyDBIProcess at
//                               EJ1N/TextProcess.cpp, ainarm_SearchPickPlate.cpp
//                               :52 and atester_ProcessCount.cpp:111.  Included
//                               via aHotPlateSubstrate.h rather than cMyDB.h on
//                               purpose: cMyDB.h's own declaration is the
//                               3-arg __fastcall overload whose real body lives
//                               at SECSGEM/uHGemEquipment.cpp:3475 -- pulling
//                               BOTH declarations into one TU is exactly the
//                               ambiguous-overload trap TextProcess.cpp's GATE
//                               [1] warns about for a different symbol
//                               (HexStrToInt); this file avoids it by only ever
//                               including the one MyDBIProcess declaration it
//                               needs, same as its three precedents.
//    CreateThread/WaitForSingleObject/TerminateThread/CloseHandle/HANDLE/DWORD/
//    LPVOID/CALLBACK/INFINITE/WAIT_FAILED/WAIT_TIMEOUT -> raw Win32 API via
//    <windows.h> (MachineDefine.h's umbrella).  Golden already spoke pure
//    Win32 here -- no std::thread/std::mutex substitution needed, so
//    WebBridge/Sync.h's CRITICAL_SECTION wrapper does not apply to this file
//    (it owns no shared/mutable state guarded by a lock -- see GOLDEN QUIRK
//    below for the one place that matters).
//
//  GATE REGISTER: none.  Every symbol this file calls has a real (or
//  established no-op stand-in) compiled body already in the port tree;
//  nothing here fails to link.
//
//  GOLDEN QUIRKS preserved VERBATIM, not fixed:
//   1. `_handle` and `_execute` are read/written from both the caller's thread
//      (OpenThread/CloseThread/Initial) and the worker thread itself
//      (ThreadProcess reads `thread->_execute` every iteration) with NO lock
//      and NO volatile.  This is golden's own design (2008.10.14 header
//      comment) -- kept exactly; not "fixed" with an atomic<bool>.
//   2. CloseThread(int iTimeOut=INFINITE) unconditionally calls
//      WaitForSingleObject(_handle, iTimeOut) even when `_handle==NULL` could
//      not happen here (the early return two lines above already excludes
//      that case) -- no behaviour change, just noting the golden control flow
//      is reproduced exactly rather than "simplified".
//   3. ThreadProcess's static `int iSleep=0` is function-local static, so it
//      is shared across EVERY HThreadCtrl instance's thread that ever runs in
//      this process (golden's own quirk -- there is only ever one live
//      instance in practice, ShuttleThread's `HThreadCtrlShuttle` analogue at
//      acarry.cpp:7070, but the sharing is real if a second instance existed).
//      Kept exactly as golden wrote it -- not hoisted into a member.
//
//  VCL/Borland conversions: #pragma hdrstop dropped (golden :2); golden has no
//  #pragma package (this .cpp is not itself a VCL unit with a resource form).
//  No __fastcall / __property / __published anywhere in this file.  No
//  AnsiString parameters/locals in this unit's own code -- AnsiString only
//  passes through as string-literal arguments to MyDBIProcess/
//  LogSoftwareOffTime, exactly as golden wrote it.  SOFT_SIMULTE is not
//  referenced by this unit.  Numeric semantics kept EXACT (SetSleepDelayCount's
//  `>=20` clamp, ThreadProcess's `>` compare against `_sleep_delay_count`).
//
//  Big5: file contains no Chinese comments at all (verified against the raw
//  cp950 decode of golden); nothing to re-encode.  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "HThreadCtrl.h" / "common.h" / "cmydef.h"
// =============================================================================
#include "MachineDefine.h"          // de-VCL'd include hub (vclcompat umbrella + <windows.h> + portable STL + using namespace std)
#include "HThreadCtrl.h"            // this unit's own contract (the 7 members defined below)
#include "common.h"                 // MySleep(DWORD) -- real body common.cpp:1919
#include "aHotPlateSubstrate.h"     // MyDBIProcess(AnsiString,AnsiString) real body :1030; transitively pulls cmydef.h (LogSoftwareOffTime declaration, cmydef.h:5032)
//------------------------------------------------------------------------------
HThreadCtrl::HThreadCtrl()
{
    _handle=NULL;
    _id=0;
    _parameter=NULL;
    _process_func=NULL;
    _sleep_delay_count=0;
    _execute=false;
}
//------------------------------------------------------------------------------
HThreadCtrl::~HThreadCtrl()
{
    try
    {
        CloseThread();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~HThreadCtrl");
    }
}
//------------------------------------------------------------------------------
void HThreadCtrl::Initial(PF_PROCESS Func, LPVOID Param)
{
    if(_handle!=NULL)
    {
        CloseThread();
    }

    _process_func   = Func;
    _parameter      = Param;
}
//------------------------------------------------------------------------------
int HThreadCtrl::OpenThread()
{
    if(_handle!=NULL || _execute==true)
    {
        CloseThread();
    }
    _execute=true;
    _handle=CreateThread(0, 0, ThreadProcess, this, 0, (DWORD*)&_id);
    return 0;
}
//------------------------------------------------------------------------------
int HThreadCtrl::CloseThread(int iTimeOut)
{
    if(_handle==NULL && _execute==false)
    {
        return 0;
    }

    LogSoftwareOffTime("FC: Wait HThreadCtrl");
    _execute=false;

    DWORD dWait=WaitForSingleObject(_handle, iTimeOut);

    if(dWait==WAIT_FAILED || dWait==WAIT_TIMEOUT)
    {
        TerminateThread(_handle, 0);
    }

    CloseHandle(_handle);
    _handle=NULL;
    _id=0;

    return dWait;
}
//------------------------------------------------------------------------------
void HThreadCtrl::SetSleepDelayCount(int iDelay)
{
    if(iDelay>=20)
        iDelay=20;

    _sleep_delay_count=iDelay;
}
//------------------------------------------------------------------------------
DWORD CALLBACK HThreadCtrl::ThreadProcess(LPVOID lpParameter)
{
    HThreadCtrl* thread=(HThreadCtrl*) lpParameter;
    static int iSleep=0;
    while(thread->_execute)
    {
        thread->_process_func(thread->_parameter);

        iSleep++;
        if(iSleep>thread->_sleep_delay_count)
        {
            iSleep=0;
            MySleep(1); // Release CPU Resource
        }
    }

    return 0;
}
//------------------------------------------------------------------------------
