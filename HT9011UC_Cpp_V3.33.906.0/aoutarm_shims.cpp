// =============================================================================
//  aoutarm_shims.cpp  --  W6.2c-OUT cross-module / engine OFFLINE shims
//
//  Integrate-phase home (added ONCE) for the out-arm ENGINE free-functions and
//  globals that the FIRST batch of 6 translated out-arm site variants
//  (aoutarm9045_{1x1_1,1x2_2,1x2_4,1x3_2_14,1x3_4,1x4_2}.cpp) call but whose real
//  bodies live in golden modules not yet translated in this tree
//  (aoutarm.cpp / aoutarm9045.cpp engine helpers, Magazine.cpp, aRotateKIT_Out.cpp,
//  AutoTeach.cpp, FixAICCD.cpp, cSortCT.cpp, ...).
//
//  Each variant TU forward-declares these behind its own #ifndef guard (verbatim
//  golden signatures); here we DEFINE them ONCE, non-static, offline-safe
//  (return false/0/no-op/sane default), so an executable linking ht9045_sm
//  resolves them.  Faithful to the golden OFFLINE end-state -- no HW touched.
//  TODO(W7): replace with the real engine bodies once aoutarm.cpp / aoutarm9045.cpp
//  engine helpers + Magazine / Rotate / AutoTeach / FixAICCD land.
//
//  Translator: AI(ht9045-v899) 20260627
//  Big5 source comments preserved as UTF-8; ZERO U+FFFD.
// =============================================================================
#include "MachineType.h"            // MAX_ARM_Row / MAX_ARM_Col
#include "vclcompat/vcl_compat.h"   // AnsiString (for OutArmPickShuttleAlarm signature)

//==============================================================================
//  Engine SM cursors / flags (golden aoutarm9045.cpp / aoutarm.h globals).
//  Offline: zero-initialized; the SMs read/advance them.  bOutSuckShtDupErr is
//  the per-nozzle duplicate-error grid (Steven 20110216).
//==============================================================================
int  iPickFromShuttle1Task = 0;                                                // golden aoutarm9045.cpp -- offline SM cursor
int  iPickFromShuttle2Task = 0;                                                // golden aoutarm9045.cpp -- offline SM cursor
int  OutArmTask            = 0;                                                // golden aoutarm9045.cpp -- offline SM cursor
int  iOutShtRetryCount     = 0;                                                // golden aoutarm9045.cpp -- offline retry counter
int  iOutArmZTeachTask     = 0;                                                // golden AutoTeach.h:235 -- offline cursor
bool bOutArmManualStepPress = false;                                           // golden aoutarm9045.cpp -- offline: no manual step
bool bOutSuckShtDupErr[MAX_ARM_Row][MAX_ARM_Col] = {{false,false,false,false},{false,false,false,false}}; // golden aoutarm9045.cpp (Steven 20110216)

//==============================================================================
//  Shuttle offset / ratio geometry helpers (golden aoutarm9045.cpp engine).
//  Offline: no offset/ratio configured -> identity / zero.
//==============================================================================
int  GetOutArmToShuttleOffset_9045(int, int, int, bool) { return 0; }          // golden aoutarm9045.cpp:1211 -- offline: 0
int  GetOutArmYToShuttleOffset_9045(int, int)           { return 0; }          // golden aoutarm9045.cpp -- offline: 0
int  GetOutArmXToShuttleOffset_9045(int, int)           { return 0; }          // golden aoutarm9045.cpp -- offline: 0
int  GetVariableYOutShuttleData()                       { return 0; }          // golden aoutarm9045.cpp (ChungHung 20131231) -- offline: 0
void TransferOutShuttleRatio(int, int*, int*, int, int) {}                     // golden aoutarm9045.cpp -- offline: identity (no transform applied)

//==============================================================================
//  Shuttle pick / data-swap / alarm surface (golden aoutarm.cpp / aoutarm9045.cpp)
//==============================================================================
void PickFromShuttle(int)                          {}                          // golden aoutarm.h:68 -- offline: no-op
bool bOutShtwaitPick()                             { return false; }           // golden aoutarm.h:131 -- offline: not waiting
void ShowOutputShuttleDataMiss()                   {}                          // golden aoutarm9045.cpp (Steven 20110527) -- offline: no message
bool SwapShuttleDataToOutArm(int,int,int,int,int,int*) { return true; }         // golden aoutarm9045.cpp (Steven 20170428) -- offline: swap OK
int  OutArmPickShuttleAlarm(int, bool, AnsiString) { return 0; }               // golden aoutarm9045.cpp -- offline: no alarm action
bool CheckTesterZ(int)                             { return true; }            // golden aoutarm9045.cpp -- offline: tester Z safe
bool CheckShuttleICPos(int)                        { return true; }            // golden aoutarm9045.cpp -- offline: shuttle IC pos OK

// SetOutArmNeedDestory: 4-arg (golden aoutarm9045.cpp) + 5-arg (RogerYang 20250516
// 9046AU, default bPlace).  Both overloads referenced across the variant set.
void SetOutArmNeedDestory(int,int,int,int)         {}                          // golden aoutarm9045.cpp -- offline: destroy map no-op
void SetOutArmNeedDestory(int,int,int,int,bool)    {}                          // golden aoutarm.h:171 (9046AU) -- offline: destroy map no-op

//==============================================================================
//  Out-arm task init / speed / state predicates (golden aoutarm9045.cpp / aoutarm.h)
//==============================================================================
void InitPickFromShuttle1Task()                    {}                          // golden aoutarm9045.cpp -- offline: no-op
void InitPickFromShuttle2Task()                    {}                          // golden aoutarm9045.cpp -- offline: no-op
void InitPlaceToAutoTask()                         {}                          // golden aoutarm9045.cpp -- offline: no-op
void OutArmAddSpeed()                              {}                          // golden aoutarm9045.cpp (KaiChen 20171225) -- offline: no-op
void OutArmSubSpeed()                              {}                          // golden aoutarm9045.cpp -- offline: no-op
void OutArmAddSpeedDisplay()                       {}                          // golden aoutarm9045.cpp -- offline: no-op
void OutArmSubSpeedDisplay()                       {}                          // golden aoutarm9045.cpp -- offline: no-op
bool CheckOutArmInitState()                        { return true; }            // golden aoutarm9045.cpp -- offline: init state ready
bool CheckOutArmToTask50(int)                      { return true; }            // golden aoutarm9045.h:49 -- offline: reached task50
bool IsOutArmCleanOutFinish()                      { return true; }            // golden aoutarm9045.cpp -- offline: clean-out finished

// CheckOutArmCleanOut: variants split between the 0-arg (golden aoutarm.h:58) and
// the (int Task=50) overload -> define BOTH non-static.  Offline: stay/continue
// the normal pick flow (return the no-clean sentinel 1140 / pass-through Task).
int  CheckOutArmCleanOut()                         { return 1140; }            // golden aoutarm.h:58 -- offline: continue normal pick flow
int  CheckOutArmCleanOut(int Task)                 { return Task;  }            // golden aoutarm9045.h:58 -- offline: stay in requested task

//==============================================================================
//  Tray / Magazine / Rotate / catch-tray cross-module surface
//==============================================================================
bool IsCatchTrayReadySupplyNewTray()               { return false; }           // golden aoutarm.h:64 / acatchtray.h -- offline: not ready
bool CheckRotateOutNotFinish()                     { return false; }           // golden aRotateKIT_Out.h (kevin 20130524) -- offline: rotate finished
void SetFixTrayFullIC()                            {}                          // golden aoutarm9045.cpp -- offline: no-op (returns void; bool-callers ignore)
bool DoFixTrayFullAlarm()                          { return true; }            // golden aoutarm9045.h:47 -- offline: alarm handled
int  VerifyTrayStatus()                            { return 3300; }            // golden aoutarm9045.h:39 -- offline: 3300 (proceed to place)
void SearchTrayToPlace_Magazine()                  {}                          // golden aoutarm9045.h:63 -- offline: no-op (returns void)
bool CheckPlaceToMagazineTray(int)                 { return true; }            // golden Magazine.h -- offline: place OK
bool DoPickFromMagazineBuffer()                    { return true; }            // golden Magazine.h -- offline: pick OK
bool AutoTrayReCheck(int)                          { return true; }            // golden aoutarm9045.cpp (Ifor 20171031) -- offline: re-check OK
void SetOutArmHome()                               {}                          // golden aoutarm.h:97 -- offline: alarm-time Z home no-op

//==============================================================================
//  Fix-AI-CCD form stand-in (golden FixAICCD.h:152 extern PACKAGE TfFixAICCD
//  *fFixAICCD; method OutArmCycleCounterUpdate()).  The variants that hold the
//  pointer call ->OutArmCycleCounterUpdate() only under USE_Fix_AI_CCD &&
//  bEnableFix2BGAAICCD (never reached offline).  Define the class method ONCE +
//  a null global pointer.  (Some variants declare an identical `class TfFixAICCD`
//  -> same mangled method symbol resolves here; others use a same-named extern
//  pointer of a local stub type -> the global address resolves here.)
//==============================================================================
class TfFixAICCD { public: void OutArmCycleCounterUpdate(); };                 // golden FixAICCD.h form (offline single-method mirror)
void TfFixAICCD::OutArmCycleCounterUpdate() {}                                 // golden FixAICCD.cpp -- offline: cycle-counter UI no-op
TfFixAICCD *fFixAICCD = 0;                                                      // golden FixAICCD.h:152 -- offline: null (never derefed offline)
