// =============================================================================
//  uHeaterThread.h  --  public interface of the heater (temperature) control
//                       thread and of the CheckHeater temperature supervisor.
//
//  Faithful translation of golden uHeaterThread.h (26 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-HEATER) 20260807
//  Translation wave: W906-PT-HEATER (pure-translation completion campaign).
//
//  ROLE: declares THeaterThread (the 20 ms heater poll thread), the
//  Start/EndHeaterThread lifecycle wrappers, CheckHeater() (the ~1500-line
//  temperature supervisor), CheckHeaterOK() (the machine-wide "may the handler
//  move at temperature?" gate that ~40 in-arm / out-arm sites consult) and the
//  two over-temperature latches bHeatOverTenErrorOK / bHeatOKOverError.
//  Bodies: uHeaterThread.cpp -- read its file head for the full gate register.
//
//  THE ONE STRUCTURAL CHANGE
//  -------------------------
//  golden :9  `class THeaterThread : public TThread`  ->  STANDALONE class, no
//  base.  Golden's TThread is VCL Classes::TThread; there is no Classes.hpp and
//  no vclcompat/TThread.h in this tree, and inventing one is out of this unit's
//  authorized scope (two files).  This is NOT a new decision: it is exactly the
//  precedent set twice already --
//    * MyPLC/MyPLC_IO_Modbus.h:5-18   (TPLCIOThread), which explicitly names
//      uHeaterThread as one of the units that would face the same choice, and
//    * acarry.cpp:7006-7066           (HThreadCtrlShuttle, the uShuttleThread
//      driver), an OFFLINE-INERT wrapper whose per-tick payload stays callable.
//  Consequences, stated exactly:
//    - `Resume()` is a no-op; no OS thread is ever spawned, so `Execute()`
//      (kept VERBATIM apart from its Synchronize() line) is dead code offline.
//    - `Terminate()` latches a private `Terminated` flag that nothing reads --
//      golden's Execute() loops on `bEnd` (golden :74), not on Terminated
//      (golden :75 shows the Terminated form commented out), so this is
//      behaviour-neutral.
//    - `Priority` is a plain `int` standing in for TThread::TPriority; golden
//      writes it once (`=tpNormal`, golden :83) and never reads it.
//    - `HeaterThreadProcess()` stays PUBLIC and directly callable, so a future
//      orchestrator can pump it once per tick exactly the way DoInArm/DoOutArm
//      are pumped without a real thread.
//  Member DECLARATION ORDER (Terminated, Priority, bEnd) is deliberate: it is
//  the ctor initialiser-list order in the .cpp, so there is no -Wreorder.
//
//  NO __fastcall anywhere.  golden :12/:14/:15 carry `void __fastcall` /
//  `__fastcall THeaterThread(...)`; __fastcall is a REAL MinGW keyword and a
//  declaration/definition mismatch would produce two differently-mangled
//  symbols, so it is dropped from BOTH sides (vclcompat/vcl_compat.h:24-54).
//
//  golden :6 `#include <Classes.hpp>` dropped -- it existed only for TThread.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef uHeaterThreadH
#define uHeaterThreadH
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"   //AI(W906-PT-HEATER) 20260807: was <Classes.hpp> (TThread only); the umbrella is what every ported header uses
//---------------------------------------------------------------------------
class THeaterThread                                                             //AI(W906-PT-HEATER) 20260807: golden ":9 : public TThread" dropped -- see the file head
{
private:
    bool Terminated;                                                            //AI(W906-PT-HEATER) 20260807: golden TThread member; latched by Terminate(), never read (Execute() loops on bEnd, golden :74)
protected:
    void Execute();
public:
    int  Priority;                                                              //AI(W906-PT-HEATER) 20260807: golden TThread::TPriority property; plain int offline, written once at golden :83
    THeaterThread(bool CreateSuspended);
    void HeaterThreadProcess(void);
    void Resume();                                                              //AI(W906-PT-HEATER) 20260807: golden TThread::Resume() (called golden :85); offline no-op
    void Terminate();                                                           //AI(W906-PT-HEATER) 20260807: golden TThread::Terminate() (called golden :96); offline latches Terminated
    bool bEnd;                                                                  //Steven 20110718 : 為了要可以關閉執行緒
};
extern THeaterThread *HeaterThread;
//---------------------------------------------------------------------------
void StartHeaterThread();
void EndHeaterThread();
void CheckHeater();
extern bool CheckHeaterOK();                                                    //Steven 20250116 : 確認HeaterOK
extern bool bHeatOverTenErrorOK;                                                //kevin 20130226 加熱ok後 溫度過高
extern bool bHeatOKOverError;                                                   //kevin 20130226 加熱ok後 溫度過高
#endif
