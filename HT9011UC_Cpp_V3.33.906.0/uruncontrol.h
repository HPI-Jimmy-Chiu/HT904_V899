// =============================================================================
//  uruncontrol.h  --  the main-loop control thread (TRunControl) that pumps
//                     MainProc() every tick.
//
//  Faithful translation of golden uruncontrol.h (21 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("tiny" group -- five very small units).
//
//  ROLE: declares TRunControl, the top-level handler main-loop thread.  Its
//  Execute() do-while calls ThreadProcess() (which pumps MainProc() while
//  InitialOK) on a tight cadence, throttled by MySleepEx.  StartMainThread /
//  EndMainThread (uruncontrol.cpp) are the process-lifetime wrappers.
//
//  THE ONE STRUCTURAL CHANGE
//  --------------------------
//  golden :7  `class TRunControl : public TThread`  ->  STANDALONE class, no
//  base.  Golden's TThread is VCL Classes::TThread; there is no Classes.hpp
//  and no vclcompat/TThread.h in this tree, and inventing one is out of this
//  unit's authorized scope (two files).  This is the SAME precedent this tree
//  already applied twice to sibling TThread subclasses:
//    * uHeaterThread.h:18-40      (THeaterThread) -- the closest sibling: same
//      shape (Execute do-while + Synchronize-turned-inline call + bEnd flag).
//    * MyPLC/MyPLC_IO_Modbus.h:58-69 (TPLCIOThread), which explicitly named
//      uHeaterThread as one of the units that would face the same choice.
//  Consequences, stated exactly (identical to uHeaterThread.h's own list):
//    - `Resume()` is a no-op; no OS thread is ever spawned, so `Execute()`
//      (kept VERBATIM apart from its Synchronize() line, see the .cpp) is
//      dead code offline.
//    - `Terminate()` latches a private `Terminated` flag that nothing reads --
//      golden's Execute() loops on `!bEnd` (golden :68), not on Terminated
//      (golden :69 shows the Terminated form commented out), so this is
//      behaviour-neutral.
//  Unlike THeaterThread, golden's TRunControl never reads/writes
//  TThread::Priority (StartMainThread, golden :72-76, sets no Priority), so
//  no Priority member is carried here -- there is nothing golden did with it
//  to preserve.
//
//  NO __fastcall anywhere.  golden :12/:13/:14 carry `void __fastcall` /
//  `__fastcall TRunControl(...)` / `void __fastcall TRunControl::ThreadProcess`;
//  __fastcall is a REAL MinGW keyword and a declaration/definition calling-
//  convention mismatch would produce two differently-mangled symbols, so it is
//  dropped from BOTH sides (vclcompat/vcl_compat.h:24-54), same as every other
//  ported unit.
//
//  golden :5 `#include <Classes.hpp>` dropped -- it existed only for TThread.
//
//  Big5: golden's only non-ASCII text is the Steven 20110726 comment on `bEnd`
//  (":Chinese phrase") -- decoded via cp950 and preserved as UTF-8 below.
//  Final gate: ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef uruncontrolH
#define uruncontrolH
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"   //AI(W906-PT-W2) 20260807: was <Classes.hpp> (TThread only); the umbrella is what every ported header uses
//---------------------------------------------------------------------------
class TRunControl                                                               //AI(W906-PT-W2) 20260807: golden ":7 : public TThread" dropped -- see the file head
{
private:
    bool Terminated;                                                           //AI(W906-PT-W2) 20260807: golden TThread member; latched by Terminate(), never read (Execute() loops on !bEnd, golden :68)
protected:
    void Execute();
public:
    TRunControl(bool CreateSuspended);
    void ThreadProcess(void);
    void Resume();                                                             //AI(W906-PT-W2) 20260807: golden TThread::Resume() (called golden :75); offline no-op
    void Terminate();                                                          //AI(W906-PT-W2) 20260807: golden TThread::Terminate() (called golden :84); offline latches Terminated
    bool bEnd;   //Steven 20110726 : 為了要可以關閉執行緒
};
extern TRunControl *MyThread;
//---------------------------------------------------------------------------
void StartMainThread();
void EndMainThread();
#endif
