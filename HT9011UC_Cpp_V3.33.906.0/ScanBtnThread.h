// =============================================================================
//  ScanBtnThread.h  --  public interface of ScanBtn (the "real-time CCD stop"
//                       button/sensor poll thread).
//
//  Faithful translation of golden ScanBtnThread.h (29 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W3) 20260807
//  Translation wave: W906-PT-W3, group "misc_a".
//
//  ROLE: declares ScanBtn -- a small poll thread that waits on two named
//  events (eStart/eFreeArm) and, once both are signalled, checks the
//  SnRealTimeCCDStop sensor; if it is on, it issues a Galil "stop-all-axes"
//  command on MTestY1 and latches the arm into a stopped state via
//  SetArmState(true).  Bodies: ScanBtnThread.cpp -- read its file head for
//  the full gate register.
//
//  THE ONE STRUCTURAL CHANGE
//  -------------------------
//  golden :8  `class ScanBtn : public TThread`  ->  STANDALONE class, no
//  base.  Golden's TThread is VCL Classes::TThread; there is no Classes.hpp
//  and no vclcompat/TThread.h in this tree, and inventing one is out of this
//  unit's authorized scope (three files, one group).  This is the SAME,
//  already-established precedent uHeaterThread.h:16-38 documents in full
//  (itself following acarry.cpp's HThreadCtrlShuttle and MyPLC_IO_Modbus.h's
//  TPLCIOThread) -- an OFFLINE-INERT wrapper whose per-tick payload
//  (Execute(), and the event/handle plumbing it drives) stays real and
//  compiles/callable, but is never actually spawned on an OS thread offline.
//  Consequences, stated exactly:
//    - Nothing in THIS file's own methods (Stop/Start/EndThread/GetRunState/
//      SetArmState/GetArmState) needs a TThread base at all -- golden itself
//      never calls Resume()/Suspend()/Terminate() from within
//      ScanBtnThread.cpp, so, UNLIKE uHeaterThread.h, this header adds NO
//      Resume()/Terminate() stand-in members: there is nothing in this TU
//      that needs them.
//    - `Execute()` is kept VERBATIM (real Win32 CreateEvent/WaitForSingleObject/
//      SetEvent/ResetEvent calls all stay real) but is dead code offline: no
//      OS thread is ever created to run it in this wave's scope (constructing
//      a ScanBtn does not start a thread -- golden's own TThread(CreateSuspended)
//      base ctor is a no-op comment now, see the .cpp file head).
//  golden :6 `#include <Classes.hpp>` dropped -- it existed only for TThread.
//  <windows.h> is included directly here (not only transitively via
//  MachineDefine.h) so this header is self-sufficient for the HANDLE type
//  regardless of include order, matching HThreadCtrl.h's own precedent of
//  including <windows.h> directly for its own HANDLE/DWORD/LPVOID members.
//
//  NO __fastcall anywhere -- golden :13-20 carry `void __fastcall` /
//  `bool __fastcall` / `__fastcall ScanBtn(...)`; __fastcall is a REAL MinGW
//  keyword and a declaration/definition mismatch would produce two
//  differently-mangled symbols, so it is dropped from BOTH sides (matching
//  uHeaterThread.h's identical treatment).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef ScanBtnThreadH
#define ScanBtnThreadH
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"   //AI(W906-PT-W3) 20260807: was <Classes.hpp> (TThread only); the umbrella is what every ported header uses
#include <windows.h>                 //AI(W906-PT-W3) 20260807: HANDLE for eStart/eFreeArm (was transitively via Classes.hpp); self-sufficient regardless of include order, matching HThreadCtrl.h's own precedent
//---------------------------------------------------------------------------
class ScanBtn                                                                   //AI(W906-PT-W3) 20260807: golden ":8 : public TThread" dropped -- see the file head
{
private:
    HANDLE eStart;
    HANDLE eFreeArm;

    bool bStart;
    bool bStopArm;
    bool bEndThread;
protected:
   void Execute();
public:
   ScanBtn(bool CreateSuspended);
   ~ScanBtn();
   void Stop();
   void Start();
   void EndThread();
   bool GetRunState();
   void SetArmState(bool bStop);
   bool GetArmState();
   bool bThreadFlag;        //Steven 20120202 : 加入Thread保護
   bool bRealTimeCCDStop;   //Steven 20120505 : 取代Aled
};
//---------------------------------------------------------------------------
#endif
