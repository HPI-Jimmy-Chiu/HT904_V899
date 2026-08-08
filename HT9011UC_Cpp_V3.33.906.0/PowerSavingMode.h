// =============================================================================
//  PowerSavingMode.h  --  TPowerSaving / TModule / TMtrModule / TVacuumModule /
//                         THotModule: the machine-idle power-saving scheduler.
//
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/PowerSavingMode.h
//                 (67 lines, cp950 -- 6 Chinese comments, all transcribed below)
//  Translation wave: PT-W4 "powersaving"
//  Translator: AI(W906-PT-W4-powersaving) 20260808
//
//  The ROLE / WAVE SCOPE / GATE REGISTER banner for this file PAIR lives in
//  PowerSavingMode.cpp.  This header carries only its own two adaptations:
//
//  [H1] `TTimer` (golden TPowerSaving::ScanTmr, golden PowerSavingMode.h:50)
//       has NO tree-wide port.  This tree's established answer is a guarded
//       TU-local stand-in (OmronLaser/LaserSensor.h:174, ATC/ATCInterface.h:190,
//       Automation/uRENESAS_Server.h:168 -- three separate ones already), and
//       every one of those three carries a DIFFERENT member set:
//         LaserSensor.h / ATCInterface.h : `class TTimer : public TObject
//                                          { bool Enabled; }`  (Enabled ONLY)
//         uRENESAS_Server.h              : `{ int Interval; bool Enabled;
//                                             std::function<void(TObject*)>
//                                             OnTimer; TTimer(TObject*); }`
//       This unit needs the uRENESAS member set (golden writes ->Interval,
//       ->Enabled AND ->OnTimer, and constructs with an owner argument) but it
//       MUST also include ATC/ATCInterface.h for the real ATCInterfaceForm that
//       THotModule::Doing drives -- so the two `class TTimer` definitions would
//       land in one translation unit.  THAT IS EXACTLY TRAP 5 (two headers, one
//       class name, different layouts): ATC/ATCInterface.cpp `new`s its own
//       TTimer, and under C++14+ sized deallocation `delete p` passes a
//       compile-time sizeof -- so quietly widening the global `TTimer` for this
//       TU (or suppressing ATCInterface.h's guard) would give two TUs different
//       sizeof(TTimer) for the same linkage name.  "It compiles" and "it links"
//       would both be worthless evidence there.  So this file's stand-in lives
//       in its OWN NAMESPACE (`ht9045_powersaving`) instead of at global scope,
//       and the two member declarations below name it as `ht9045_powersaving::
//       TTimer *`.  Zero ODR surface shared with any other TTimer in the tree,
//       by construction rather than by convention.
//       WHAT THE STAND-IN DOES *NOT* DO: nothing in this build pumps a Windows
//       message queue, so assigning ->OnTimer stores the callback and NOTHING
//       EVER CALLS IT.  TPowerSaving::OnScanTmr -- i.e. the whole power-saving
//       state machine -- is reachable today only by a direct call (a future
//       ported main.cpp / a unit test), never by a 1000 ms tick.  Same
//       situation, and same honest accounting, as Automation/uRENESAS_Server.h
//       [G3] and its ProcTimer.
//
//  [H2] `__fastcall` dropped from TModule/TPowerSaving ctors+dtor and from
//       OnScanTmr (golden marks all five).  Every field and method is otherwise
//       preserved verbatim, in golden's order, including golden's NON-virtual
//       `~TModule()` beside a virtual `Doing()` -- see PowerSavingMode.cpp's
//       "NOT gates" note (3) for why that is safe here and deliberately not
//       "fixed".
//
//  Golden `#include "myswitch.h"` is kept (TModule::mSW is a TMySwitch*).  The
//  types golden gets ambiently from <vcl.h> via MachineDefine.h (TDateTime,
//  TObject, Word) and from the same hub (TQPF_Timer, myTimer.h) are included
//  explicitly here instead.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef PowerSavingModeH
#define PowerSavingModeH

#include "vclcompat/vcl_compat.h"   // AnsiString / TObject / TDateTime / Word (golden: <vcl.h> via MachineDefine.h)
#include "myswitch.h"               // golden PowerSavingMode.h:6 -- TMySwitch (TModule::mSW)
#include "myTimer.h"                // TQPF_Timer (THotModule::tHotModuleTimer); golden gets it via MachineDefine.h
#include <functional>               // [H1] std::function -- the OnTimer slot

//---------------------------------------------------------------------------
// [H1] TTimer stand-in.  NAMESPACED ON PURPOSE -- see banner [H1].  Member set
// is exactly what golden PowerSavingMode.cpp touches: ->Interval (:38),
// ->Enabled (:39,:113,:223,:280,:284) and ->OnTimer (:37), constructed with an
// owner (:35 `new TTimer(fMain)`).  Not a general-purpose TTimer.
//---------------------------------------------------------------------------
namespace ht9045_powersaving {
class TTimer
{
public:
    int  Interval;
    bool Enabled;
    std::function<void(TObject* Sender)> OnTimer;
    // Owner is TYPE-ERASED to `void*` on purpose.  Golden's real VCL ctor is
    // `TTimer(TComponent* Owner)` and golden passes `fMain`, but this tree's
    // form stand-ins (forms/fMain.h TfMain) are PLAIN classes -- they derive
    // from neither vclcompat::TObject nor any TComponent -- so a
    // `TObject*`/`TComponent*` parameter would simply not accept `fMain`
    // (measured: g++ rejected `TObject*` with "no known conversion from
    // 'TfMain*'").  The owner is stored nowhere and read never (there is no
    // VCL ownership tree to insert into offline), so widening the parameter to
    // `void*` erases a type that has no use rather than losing information.
    explicit TTimer(void* /*Owner*/) : Interval(0), Enabled(false) {}
};
} // namespace ht9045_powersaving

//---------------------------------------------------------------------------
class TModule
{
  public:
  TModule();
  ~TModule();

  bool Enabled;                                          //Module啟用
  TMySwitch *mSW;
  TDateTime AlarmTmr;                                    //設定警報時間
  TDateTime iCountDown;                                 //倒數的時間

  bool TimeOut(TDateTime StartTmr);                      //倒數
  virtual bool Doing(bool Reset=false);                 //時間到,Module進入處理工作
};
class TMtrModule : public TModule
{
   public:
   bool Doing(bool Reset=false);
};
class TVacuumModule : public TModule                    //Steven 20221215 : Power saving for vacuum pump
{
   public:
   bool Doing(bool Reset=false);
};
class THotModule : public TModule
{
   public:
   bool bCheckTempClose;
   bool Doing(bool Reset=false);
   TQPF_Timer tHotModuleTimer;
};
class TPowerSaving
{
   private:
   bool flagStartTmr;                                    //機台閒置進入休眠倒數旗標
   bool flagUseFunc;                                     //休眠功能啟用旗標
   TDateTime StartTmr;

   public:
   TPowerSaving();
   ~TPowerSaving();

   ht9045_powersaving::TTimer *ScanTmr;                  // [H1] golden: TTimer *ScanTmr;
   TMtrModule *MtrModule;                                //控制馬達休眠Moduel
   THotModule *HotModule;                                //控制加熱裝置休眠Moduel
   TVacuumModule *VacuumModule;                          //Steven 20221215 : Power saving for vacuum pump
   TModule *tModule;                                     //處理進入休眠ShowMessage

   void OnScanTmr(TObject *Sender);                      //Scan Timer
   void SetFunction(bool OnOff);                         //設定啟用的Module
   void SetAlarmTime(TDateTime &Module ,int Minute);     //設定警報時間
   bool CheckSystemRun();                                //檢查是否進入休眠
   void Restart();                                       //通知謷報重新計數
   bool CheckChangeState();                              //檢查是否在進入倒數時,有停止進入省電模式的操作

   bool bRestart;                                        //重新計數
};
//---------------------------------------------------------------------------
extern class TPowerSaving *tPowerSaving;
#endif
