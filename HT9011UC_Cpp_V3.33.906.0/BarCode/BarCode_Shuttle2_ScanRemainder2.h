// =============================================================================
//  BarCode/BarCode_Shuttle2_ScanRemainder2.h  --  TfBarCode::DoShuttleFloatCheck_2
//                                                  (Shuttle-2 "IC float check" SM)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode_Sh2.cpp
//                   :4406-4878 (473 lines)                       //Steven 20160920
//  Golden decl home: BarCode/BarCode.h:891 (TfBarCode class, public method).
//
//  Translation wave: W5-BarCode hand-off completion (this is the exact 2-method
//  "REMAINING / HANDED OFF" leftover flagged by BarCode_Shuttle2_Scan.h's own
//  translate report -- see that header's banner. The sibling method in that
//  same hand-off, `DoBarcodeScanInShuttle_2`, is NOT part of this unit; it
//  keeps its own pre-existing offline stub in aHotPlateSubstrate.cpp
//  unchanged.  Only `DoShuttleFloatCheck_2` is delivered here).
//  Translator: AI(W5-Final-BarCodeShuttle2ScanRemainder2) 20260711.
//
//  WHY A SEPARATE TU (not folded into BarCode_Shuttle2_Scan.cpp)
//  ---------------------------------------------------------------------------
//  BarCode_Shuttle2_Scan.{h,cpp} is listed as a shared/prior-wave deliverable
//  this task's boundary does not include editing (only the two NEW files named
//  by the task: BarCode_Shuttle2_ScanRemainder2.{h,cpp}).  This file therefore
//  #includes BarCode_Shuttle1_Scan.h (see below) to reuse -- NOT redefine --
//  every piece of shared TfBarCode instance state it needs; it defines nothing
//  that any other already-delivered TU also defines.
//
//  GOLDEN STRUCTURE (near-mirror of the already-translated sibling
//  DoShuttleFloatCheck_1, BarCode_Shuttle1_Scan.cpp:1921-2349)
//  ---------------------------------------------------------------------------
//  Both functions are the SAME golden TfBarCode class's "Shuttle Float Check"
//  (SFC) state machine, one instantiated per shuttle side.  DoShuttleFloatCheck_2
//  is structurally almost identical to DoShuttleFloatCheck_1 (same switch(Task)
//  case numbers: 1/500/1000/1120/1150/1160/1170/1180/1200/1300/1350/2000/2500/
//  2600/5000), differing only in: BLCarryKit (not FLCarryKit), iSht=1 /
//  MSht=MInShuttle2 (not 0/MInShuttle1), iSFCRowA/B=iBarCode2_1/2_2 (not
//  iBarCode1_1/1_2), bCCDcheckShuttle[1] (not [0]), and the alarm codes
//  (WAR0470/JAM0479 vs Sh1's WAR0469/JAM0478 -- WAR0462/WAR0464 are the SAME
//  code on both sides, verbatim from golden).
//
//  GOLDEN QUIRK PRESERVED VERBATIM (Shuttle-1/Shuttle-2 asymmetry -- same class
//  of preserved-not-fixed divergence as BarCode_Shuttle2_Scan.h's documented
//  case-3400 quirk):
//    DoShuttleFloatCheck_1 (golden BarCode_Sh1.cpp:4224-4696) branches case
//    1150/1350's ErrPart/sErrorPart IndexSuckName row selection on
//    `IsNNMode()==NN_2Row / NN_1Row / else` (3-way).  DoShuttleFloatCheck_2
//    (golden BarCode_Sh2.cpp:4406-4878, THIS function) has NO IsNNMode() call
//    at all -- it branches purely on `InArmSuck.iShtRow==2` (2-way), using
//    IndexSuckName[0]/[1] directly.  Verified by reading the full golden body
//    (not just skimming): no IsNNMode/NN_1Row/NN_2Row token appears anywhere in
//    BarCode_Sh2.cpp:4406-4878.  This looks like the NN-mode alarm-part
//    enhancement (a later addition, "Steven 20220315") was applied to the
//    Shuttle-1 side only and never back-ported to Shuttle-2.  NOT "fixed" here
//    -- reproduced exactly (2-way branch, no IsNNMode()).
//
//  SHARED golden TfBarCode STATE THIS FILE REUSES (does NOT redefine -- see
//  each header's own canonical-ownership banner):
//    From BarCode_Shuttle1_Scan.h (canonical definitions in
//    BarCode_Shuttle1_Scan.cpp -- that header's own comment already flags
//    "DoShuttleFloatCheck_2 ... still flagged for whoever completes Sh2's
//    float check later", i.e. exactly this unit):
//      iSFCTotalMoveStep, SFCStartDelay[2], SFCExposureDelay[2],
//      SFCResultDelay[2], iSFCCurrentStep[BAR_CODE_COUNT],
//      bSFCStepError[BAR_CODE_COUNT], bSFCExposureOK[BAR_CODE_COUNT],
//      bSFCGetResultOK[BAR_CODE_COUNT], bSFCGetSE9[BAR_CODE_COUNT],
//      iSFCCheckResult[BAR_CODE_COUNT][16].
//      NOTE (judgment call, flag for review): in golden these are single
//      TfBarCode-instance members, so golden Sh1 and Sh2 SFC cycles genuinely
//      share `iSFCTotalMoveStep` (no per-side copy) -- reproduced faithfully by
//      reusing the SAME extern rather than giving Sh2 its own copy. This is
//      harmless in practice: both sides recompute it identically from the same
//      shared `InArmSuck.iShtCol/iShtRow` config at their own Task==1 entry
//      before using it, and the per-row-indexed arrays (iSFCCheckResult[iSFCRowA/B]
//      etc.) use DISJOINT indices for Sh1 (rows 0/1) vs Sh2 (rows 2/3), so no
//      actual cross-talk occurs even if both SMs happen to run in the same tick.
//    From BarCode_Shuttle2_Scan.h (canonical definitions in
//    BarCode_Shuttle2_Scan.cpp, already delivered):
//      iBarCode2_1, iBarCode2_2, iShuttleFloatCheck2Task, bCCDcheckShuttle[2].
//
//  Already-available real dependencies reused (NOT gated) -- all pulled in
//  transitively via BarCode_Shuttle1_Scan.h's own include list:
//    InArmSuck / BLCarryKit (aHotPlateSubstrate.h, TMyKitSuck::HasRealIC())
//    MOT[] / TTrayMotor::MotorMove (Motor/mymotor.h)
//    Prod / Offset / TestIF_File (cprod.h)
//    fLtcSensor->ClearLtcSensor(int) (acarry_shims.h)
//    ShowMyMessage / ShowErrorMessage / K_RETRY / K_SKIP (canary_support.h)
//    IndexSuckName[4][8] (cmydef.h -- real global, already defined cmydef.cpp:14)
//
//  Gated (golden lives in the still-untranslated shared BarCode.cpp core, or a
//  VCL widget/form not yet translated) -- see the .cpp's Gated_* wrapper
//  functions for the exact per-symbol golden citation + conservative-default
//  rationale:
//    SendCCDCommand(int,AnsiString,AnsiString)        golden BarCode.cpp:5305
//    AddCCDCommunicationLog(int,AnsiString)            golden BarCode.cpp:5903
//    mtSFC_Shuttle->SetCellNumber(int,int,AnsiString)  golden BarCode.h (TTMyTray* grid, W7)
//  One EXCEPTION (mirrors BarCode_Shuttle1_Scan.cpp's own precedent): golden
//  `TfBarCode::SetSFCCheckStepCount()` (BarCode.cpp:3128) is a trivial 2-line
//  pure calc that sets the very loop bound (`iSFCTotalMoveStep`) this SM
//  depends on for correctness, so it is NOT gated -- this file supplies its
//  own faithful mirror (`BarSh2_SetSFCCheckStepCount`, file-local/internal
//  linkage; deliberately NOT calling the sibling's internal-linkage
//  `BarSh1_SetSFCCheckStepCount`, which is unreachable across TUs anyway --
//  same reasoning, harmless duplication of a 2-line pure function).
//
//  Because SendCCDCommand/AddCCDCommunicationLog are gated to no-ops,
//  `bSFCGetSE9[]` never becomes true offline -- case 500 always times out
//  after 5s and alarms WAR0462 (K_RETRY, canary_support Sim default), looping
//  back to Task=1 forever.  This is the SAME documented "hardware-gated SM
//  never truly completes, but never escapes its documented switch(Task)
//  universe either" shape already established by every sibling BarCode unit
//  in this wave (Do*Trigger*/Do*Scan* with no real CCD channel).
//
//  Big5: golden read via cp950-aware iconv; zero U+FFFD verified in this file.
// =============================================================================
#ifndef BarCode_Shuttle2_ScanRemainder2H
#define BarCode_Shuttle2_ScanRemainder2H

#include "BarCode_Shuttle1_Scan.h"   // SFC shared state (canonical owner) + everything
                                     // it itself pulls in transitively (aHotPlateSubstrate,
                                     // Motor/mymotor, cprod/cmydef, acarry_shims, canary_support,
                                     // BarCode_Shuttle2_Scan.h for iBarCode2_1/2/iShuttleFloatCheck2Task)

// ============================================================================
//  This unit's one assigned free function (exact signature per
//  aHotPlateSubstrate.h:651 -- the TfBarCode_Shim delegation target).
// ============================================================================
bool BarCode_Sh2_DoShuttleFloatCheck_2();   // golden BarCode.h:891 / BarCode_Sh2.cpp:4406

#endif // BarCode_Shuttle2_ScanRemainder2H
