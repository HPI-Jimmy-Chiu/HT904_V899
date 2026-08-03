// =============================================================================
//  ckernel_shims.h  --  offline home for the ckernel.cpp free functions that
//                       have no ported translation unit (W7-L2)
//
//  Translation wave: W7-L2 (golden ckernel.cpp -- 2589 lines, 17 functions)
//  Author: AI(W906-W7-L2) 20260803
//
//  ROLE
//  ----
//  Golden ckernel.cpp's ScanSystemSensor (:359-691) calls two free functions
//  whose golden definitions live in cinitial.cpp -- a translation unit that
//  does NOT exist in this ported tree and is not scheduled for this wave.
//  This file is their ACTIVE, transparently-named home, in the same role the
//  engine *_shims.cpp files already play (acarry_shims / acatchtray_shims /
//  atester_shims / csystem_shims).
//
//    SetWorkParameter()   -- called golden ckernel.cpp:371
//    SetSuckRetryCount()  -- called golden ckernel.cpp:496
//
//  BOTH GOLDEN DEFINITIONS WERE FOUND, not assumed absent.  An exhaustive scan
//  of all 708 .c/.cpp/.h/.hpp files in the golden tree (368 .h, 338 .cpp, 1 .c,
//  1 .hpp) for these two identifiers returned exactly one definition each:
//      cinitial.cpp:13493  `bool SetWorkParameter()`   (decl cinitial.h:17)
//      cinitial.cpp:13676  `void SetSuckRetryCount()`  (decl cinitial.h:20)
//  Neither has any home in the ported tree: a grep for both identifiers across
//  every .cpp and .h here finds no definition and no call, the only other
//  occurrence being a prose cross-reference in a comment at forms/fMain.h:622.
//  So they belong here, and they are TRANSLATED, not stubbed -- see
//  ckernel_shims.cpp for the
//  bodies and for the precise, itemised list of the sub-calls inside
//  SetWorkParameter that are still seams.
//
//  ---------------------------------------------------------------------------
//  RETURN-TYPE CORRECTION vs THE W7-L2 BRIEF
//  ---------------------------------------------------------------------------
//  The assignment specified `void SetWorkParameter();`.  Golden declares it
//  `bool SetWorkParameter();` at cinitial.h:17 and defines it
//  `bool SetWorkParameter()` at cinitial.cpp:13493, so **bool** is the faithful
//  signature -- full stop, no further argument needed.  (cinitial.h:29 carries a
//  commented-out duplicate of the same declaration; it is dead text and is the
//  authority for nothing.)  What the bool means: false when any of 15 tray
//  motors still has an uninitialised tray geometry, cinitial.cpp:13577-13579.
//
//  CALL-SITE CENSUS, RE-MEASURED THIS PASS -- and it does NOT say what an
//  earlier draft of this comment said.  That draft defended the bool by claiming
//  "the other 25 golden call sites ... would lose that signal".  Corrected in
//  place rather than argued with.  Exhaustive cp950 scan of all 708 golden
//  source files with // and /* */ text masked: 33 occurrences of the identifier,
//  minus the definition (cinitial.cpp:13493), the declaration (cinitial.h:17)
//  and one appearance inside a ShowMyMessage string literal (cinitial.cpp:13545)
//  = **30 call sites**:
//      main.cpp                       17   (not 16)
//      Command.cpp                     4
//      cBinSel.cpp, cinitial.cpp, ckernel.cpp, cOffSet.cpp, cSetUp.cpp,
//      cTesterIF.cpp, AutoClean/uCleaning.cpp, ProductionInfo/
//      ProductionInfo.cpp, RotateKit/fRotate.cpp   1 each  (9)
//  ALL 30 ARE BARE STATEMENTS `SetWorkParameter();`.  **ZERO consume the return
//  value** -- ckernel.cpp:371 included.  So narrowing to void would have deleted
//  no signal from any existing caller; bool is kept purely because golden
//  declares bool.  The consequence a test author needs to know: the return value
//  is not observable through ANY real golden caller, so it can only be tested by
//  calling SetWorkParameter() directly.
//
//  ---------------------------------------------------------------------------
//  SCOPE RULE OBSERVED
//  ---------------------------------------------------------------------------
//  Only symbols with NO ported home go here.  Nothing in this file duplicates
//  an existing definition:  InitProcessSingleMotorTask lives in
//  acatchtray_shims.cpp, ShowMyMessage in canary_support.cpp, the KitSuck
//  objects in aHotPlateSubstrate.cpp, `Tech` in acarry_shims.cpp, MOT[] in
//  Motor/mymotor.cpp -- all reused, none redefined.
//
//  Big5: Chinese comments carried over from golden were decoded via cp950 and
//  are stored here as real UTF-8.  ZERO U+FFFD.
// =============================================================================
#ifndef ckernel_shimsH
#define ckernel_shimsH

// ---------------------------------------------------------------------------
//  golden cinitial.cpp:13493 (decl cinitial.h:17).
//  Reloads the work parameters after any setup/recipe change and re-derives the
//  shuttle-thread sensor/position table from them.  Returns false if any of the
//  15 loader/unloader/hotplate tray motors still has XItem<=0 or YItem<=0, i.e.
//  "tray geometry not initialised yet".
//  Called at golden ckernel.cpp:371, inside ScanSystemSensor's SoftStart arm,
//  where the return value is discarded.
// ---------------------------------------------------------------------------
bool SetWorkParameter();

// ---------------------------------------------------------------------------
//  golden cinitial.cpp:13676 (decl cinitial.h:20).
//  Pushes the per-arm vacuum retry counts from ArmSpeed[] down onto every
//  nozzle of the in-arm / out-arm / index / tray-arm suck grids.
//  Called at golden ckernel.cpp:496, inside ScanSystemSensor's SoftStart arm.
// ---------------------------------------------------------------------------
void SetSuckRetryCount();

// ---------------------------------------------------------------------------
//  OBSERVABILITY SEAM for SetSuckRetryCount -- AI(W906-W7-L2) 20260803.
//
//  Golden's SetSuckRetryCount works entirely through TMySucker::SetRetryCount
//  (declared `void SetRetryCount(int);` at golden MyKitSuck.h:113; body
//  MyKitSuck.cpp:2673-2676, whose single statement `RetryCT=ct;` is at :2675)
//  writing the member `int RetryCT;` at MyKitSuck.h:20.  The PORTED TMySucker
//  (aHotPlateSubstrate.h) has
//  neither the method nor the RetryCT field, and that header is owned by another
//  agent this wave, so the per-nozzle write cannot be landed here.
//
//  Rather than drop the writes silently, ckernel_shims.cpp records what golden
//  would have written -- the value per suck grid, and how many nozzles golden
//  would have visited -- and publishes it here so the behaviour is TESTABLE
//  instead of merely absent.  Same device as the operator-dialog observability
//  seam at canary_support.h:270-311 (section-4 banner at :270, the seam's own
//  extern declarations at :303-311).
//
//  THIS IS A RECORD OF AN UNPERFORMED WRITE, NOT THE RETRY STATE.  Nothing may
//  read these counters as if they were nozzle state.  The section banner in
//  ckernel_shims.cpp gives the full argument for why nothing observable is lost
//  today (the ported TMySucker::Suck/Destroy contain no retry logic to consume
//  RetryCT) and the exact THREE-line change to aHotPlateSubstrate.{h,cpp} that
//  retires the seam -- three, not two: the constructor initialiser
//  `RetryCT=1;` (golden MyKitSuck.cpp:1775) is part of the contract and
//  omitting it would install 0 where golden has 1.
// ---------------------------------------------------------------------------
struct W7L2_RetryCountSeam
{
    int iInArmRetryCT;      int iInArmNozzles;      // golden cinitial.cpp:13683
    int iOutArmRetryCT;     int iOutArmNozzles;     // golden cinitial.cpp:13684
    int iFTestRetryCT;      int iFTestNozzles;      // golden cinitial.cpp:13692
    int iBTestRetryCT;      int iBTestNozzles;      // golden cinitial.cpp:13693
    int iCatchTrayRetryCT;  int iCatchTrayNozzles;  // golden cinitial.cpp:13699
    W7L2_RetryCountSeam();
};

// Counters are CUMULATIVE across calls (SetSuckRetryCount is idempotent per
// START in golden, so a test normally reads this after a single call).
const W7L2_RetryCountSeam &W7L2_GetRetryCountSeam();

#endif
