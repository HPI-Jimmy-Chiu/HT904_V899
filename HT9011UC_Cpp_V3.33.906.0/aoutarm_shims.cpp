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
bool CheckDuplicateBarCode()                       { return true; }            // golden aoutarm.h:130 -- offline: barcode OK / not duplicate (so the case-3000 guard `==false` never forces Task=3000); only called by 1x4_4 under CosFunction.bBarcodeDuplicateFileByOutArm (false offline)
// (W6.2c-OUT batch-3) The temp MoveOutArmToShuttleIncludeZ_9045_2x4_4 stub that lived
// here is REMOVED -- aoutarm9045_2x4_4.cpp is now translated/added live and DEFINES the
// real symbol (golden aoutarm9045_2x4_4.cpp:486); keeping the stub = duplicate symbol.

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
int  SearchTrayToPlace_Magazine()                  { return 0; }               // golden aoutarm9045.h:63 (int) -- AI(W906-W7-A2) 20260728: fix return-type ODR violation -- this def and 14 of the 27 site-variant TUs (aoutarm9045_{1x2_2,1x2_4,1x4_2,1x4_4,1x4_4S,1x4_4_Back,1x4_8,2x2_4,2x2_4_14,2x2_4_23,2x2_8,2x4_16,2x5_8,S_1x4_4}.cpp) forward-declared this `void`, disagreeing with the other 13 variants' `int` and with golden's `int` (aoutarm9045.h:63, def aoutarm9045.cpp:1420). Harmless today only because the one golden call site that consumes the return value (aoutarm9045.cpp:1369 `return SearchTrayToPlace_Magazine();`) is not yet translated into this tree's aoutarm9045.cpp -- unified to `int`/`return 0` now so that future translation of that call site does not inherit a live ODR trap. AI(W906-A2-followup) 20260728: corrected gloss -- golden's real not-found path returns Prod.iIfErrorT6 (golden aoutarm9045.cpp:1558), NOT 0; golden never returns a literal 0 from this function's real body (paths return 2, iSortTrayIndex[k], OutArmSuck.iWhichAuto[i][j], or Prod.iIfErrorT6 -- golden :1453,1469,1530,1535,1540,1546,1552,1558). `return 0` here is an arbitrary offline placeholder, not golden's not-found value; if ever consumed downstream (currently all 27 ported call sites discard the return value) it would read as Auto1, matching this file's own SearchTrayToPlace_9045 stand-in at aoutarm9045.cpp:1501 (`return 0; // golden :2451 -> iWhichAuto=0 (Auto1)`) -- there is no other Search* stand-in in this file, and the only Verify* stand-in (VerifyTrayStatus, line 94 above) returns 3300, not 0.
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

//==============================================================================
//  (W6.2c-OUT batch-3) 9046AU SORT-ARM/SORT-SHUTTLE engine surface (golden
//  aoutarm9045.cpp / aoutarm.h, RogerYang 20250513-20250710).  Called by
//  aoutarm9045_2x4_8 / 2x8_8 only under USE_OUT_SORT_ARM != eartUninstall, which
//  is never true offline -> these branches are inert; defined here ONCE so
//  ht9045_sm links.  Faithful offline end-state (no AU sort dispatch).
//==============================================================================
int  iPlaceToSortShtTask = 0;                                                  // golden aoutarm.h (9046AU) -- offline SM cursor
void InitPlaceToSortShtTask()                                  {}              // golden aoutarm9045.h (9046AU) -- offline: no-op
bool NeedPlaceToSort()                                         { return false; } // golden aoutarm9045.h (9046AU) -- offline: no AU dispatch
int  GetSortArmToSortShuttleOffset()                           { return 0; }   // golden aoutarm9045.cpp (9046AU) -- offline: 0
int  GetSortArmPitchX(int /*iMovePitchX*/, int /*iOffsetPos*/) { return 0; }   // golden aoutarm9045.cpp (9046AU) -- offline: 0
void GetSortShuttleStatus_Pick(int /*iZPos*/[][MAX_ARM_Col], bool /*bZDown*/, bool /*bZFlag*/[][MAX_ARM_Col]) {} // golden aoutarm9045.cpp (9046AU) -- offline: leave caller arrays untouched (zeroed by caller)
bool IsCheckSortArmDestroyActiveFinish(int /*iXPos*/, int /*iYPos*/) { return true; } // golden aoutarm9045.cpp (9046AU) -- offline: finished
void DoSortArmSuckPreOn(int /*iKit*/, int /*iXPos*/, int /*iYPos*/, bool /*bZDown*/) {} // golden aoutarm9045.cpp (9046AU) -- offline: no-op
bool CheckSortArmXYPitch_2x4_8(int * /*iX*/, int * /*iY*/, int /*iMovePitchX*/) { return true; } // golden aoutarm9045_2x4_8.cpp (9046AU; engine def) -- offline: pitch OK
void SetSortShuttleStatus_Place(int /*iShtRow*/, int /*iShtCol*/, int /*iSuckRow*/, int /*iSuckCol*/) {} // golden aoutarm9045.h:33 (9046AU) -- offline: no-op

//==============================================================================
//  (W6.2c-OUT batch-3) AutoTeach close-pitch geometry (golden aoutarm.h AutoTeach
//  surface).  Used by aoutarm9045_2x8_8 close-site placement; offline returns 0 /
//  inert so the normal (non-auto-teach) placement path is taken.
//==============================================================================
int    AutoCalculateOutArmXClosePitch(int /*iWhichAuto*/, bool /*bUseDeviceDinemsion*/) { return 0; } // golden aoutarm.h -- offline: 0
int    AutoCalculateOutArmYClosePitch(int /*iWhichAuto*/) { return 0; }        // golden aoutarm.h -- offline: 0
double dOutArmXPitch_1Step = 0.0;                                              // golden aoutarm.h -- offline: 0
int    iOutArmYPosition    = 0;                                                // golden aoutarm.h -- offline: 0

//==============================================================================
//  (W6.2c-OUT batch-3) Out-rotate kit (golden aRotateKIT_Out.h, Ifor 20251215).
//  Used by aoutarm9045_All_1Picker rotate cases 110/120; offline: rotate "done".
//==============================================================================
bool MoveOutRotateToDegreeAtSameTime(int /*iDegree*/, bool /*bInit*/) { return true; } // golden aRotateKIT_Out.h -- offline: rotate done
