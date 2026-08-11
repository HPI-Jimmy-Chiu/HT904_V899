// =============================================================================
//  BarCode/BarCode_Shuttle2_SFCAutoTune.h  --  Shuttle-2 "SFC Auto Tune" (AT)
//        cycle + the manual "2DID Check" cycle of the golden TfBarCode VCL form
//
//  ROLE
//  ---------------------------------------------------------------------------
//  Three golden TfBarCode methods, translated as FREE FUNCTIONS with the
//  `BarCode_Sh2_` prefix, exactly matching the convention already established by
//  the 7 sibling methods of this same golden file (BarCode_Shuttle2_Scan.cpp /
//  BarCode_Shuttle2_ScanRemainder1.cpp / BarCode_Shuttle2_ScanRemainder2.cpp).
//
//    * BarCode_Sh2_InitialSFCAutoTune2(bool) / BarCode_Sh2_DoSFCAutoTune_2(bool)
//      -- the ENGINEERING "Shuttle Float Check Auto Tune" cycle.  Not part of
//      production auto-run: golden pumps it from cContact.cpp / uhome.cpp
//      (an operator-initiated maintenance sweep that walks Shuttle-2 across
//      every SFC photo position and fires the CCD at each, once with no device
//      loaded -- Task 1..1400 -- and once with a device loaded -- Task
//      10000..11400 -- so the CCD can auto-learn its exposure/ROI).
//    * BarCode_Sh2_Do2DIDCheckSh2() -- the operator "check the 2D readers"
//      sweep: steps Shuttle-2 across every barcode-detect position and scans,
//      advancing on the manual Step/Retry keys.  Golden pumps it from
//      BarCode/BarCode.cpp.
//
//  WHO PUMPS IT (today, in this port tree): NOBODY -- see the .cpp banner's
//  trap-1 note.  All three golden callers (cContact.cpp, uhome.cpp,
//  BarCode/BarCode.cpp core) are still untranslated, and TfBarCode_Shim
//  (aHotPlateSubstrate.h:997-1013) deliberately does NOT declare these three,
//  so there is no shim wiring for them and none is added.  This unit compiles
//  and enters ht9045_sm's archive but is not extracted by the link.  That is
//  expected and is documented, not papered over.
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode_Sh2.cpp
//                   (5664-line golden file; these are the LAST 3 functions in
//                   it and the only ones that had never been translated).
//  Golden decl home: BarCode/BarCode.h:733 / :893 / :895 (TfBarCode class).
//  Translation wave: W5-BarCode tail completion (Sh2 SFC-AutoTune trio).
//  Translator: AI(W5-BarCode-Shuttle2-SFCAutoTune) 20260811.
//
//  WAVE SCOPE -- ONE LINE PER ASSIGNED GOLDEN FUNCTION
//  ---------------------------------------------------------------------------
//    ACTIVE  BarCode_Sh2_InitialSFCAutoTune2(bool bHasIC)
//            golden BarCode_Sh2.cpp:4880-4886   (7 lines)   decl BarCode.h:893
//    ACTIVE  BarCode_Sh2_DoSFCAutoTune_2(bool bHasIC)
//            golden BarCode_Sh2.cpp:4888-5417   (530 lines) decl BarCode.h:895
//    ACTIVE  BarCode_Sh2_Do2DIDCheckSh2()
//            golden BarCode_Sh2.cpp:5419-5664   (246 lines) decl BarCode.h:733
//  All three bodies are translated IN FULL.  None is `#if 0`-gated: every
//  switch(Task) case, formula and magic number is present.  What IS gated is a
//  short list of cross-module LEAF CALLS made from inside them -- see GATE
//  REGISTER in the .cpp.
//
//  NOTE: golden's `bHasIC` parameter of DoSFCAutoTune_2 is DEAD (the body never
//  reads it -- the has-device / no-device split is carried entirely by the Task
//  cursor that InitialSFCAutoTune2 seeded).  Preserved verbatim, not "cleaned
//  up"; see the .cpp.
//
//  GOLDEN QUIRKS PRESERVED VERBATIM (all reproduced, none "fixed" -- this
//  project's standing rule; each is also marked inline at the exact spot)
//  ---------------------------------------------------------------------------
//   Q1. DoSFCAutoTune_2 declares `int iSht=0;` (golden :4899) while its motor is
//       `MSht=MInShuttle2` and its CCD rows are iBarCode2_1/2_2.  Every
//       `iSht`-indexed read in the function therefore reaches SHUTTLE-1 data:
//       Prod.iInSFCPos1x{1,2,3,4,5,6,8}[0][...], Offset.iSHLeft2D[0],
//       SFCStartDelay[0], SFCExposureDelay[0], and the log line
//       "Shuttle %d", iSht+1 prints "Shuttle 1" on the Shuttle-2 machine.
//       Contrast the already-translated direct sibling DoShuttleFloatCheck_2
//       (golden :4406, port BarCode_Shuttle2_ScanRemainder2.cpp:70) which
//       correctly uses `iSht=1`.  This is a copy-paste-from-Sh1 defect in
//       golden.  On a real machine it makes the AT sweep drive Shuttle-2 to
//       Shuttle-1's taught SFC coordinates.  REPRODUCED, NOT FIXED.
//   Q2. DoSFCAutoTune_2 case 10000 (golden :5174-5197) has NO `break` -- it
//       assigns Task=11000 and FALLS THROUGH into case 11000 in the same tick.
//       Deliberate in golden (the same "seed then immediately act" shape
//       DoShuttleFloatCheck_2's case 1 -> 500 uses).  Fall-through preserved.
//   Q3. DoSFCAutoTune_2 case 11200 (golden :5401) branches to `Task=13000`, and
//       THERE IS NO case 13000 anywhere in the function.  The has-device AT
//       sweep therefore parks forever at an unhandled Task once it finishes its
//       last step, never reaching case 11300/11400 and never returning true.
//       Correspondingly case 11300 (golden :5404) is UNREACHABLE -- nothing in
//       the function ever assigns Task=11300, so the "move back to
//       Prod.OutSHT[1].iLeft then log the end marker" tail of the has-device
//       cycle is dead code.  (The no-device twin does it right: case 1200 ->
//       1300 -> 1400.)  REPRODUCED, NOT FIXED -- the operator's escape from the
//       parked state is the same one golden relies on: re-arm via
//       InitialSFCAutoTune2().
//   Q4. DoSFCAutoTune_2 case 1400 (golden :5166-5170) sets `Task=1300` BEFORE
//       `bResult=true`, so the no-device cycle ping-pongs 1300<->1400 forever,
//       re-issuing the motor move and returning true on every other pump.  Its
//       has-device twin case 11400 (golden :5410) does NOT reassign Task.
//       Asymmetry preserved.
//   Q5. Do2DIDCheckSh2 also declares `int iSht=0;` (golden :5422) -- but unlike
//       Q1 it uses hard-coded [1] for every Prod/Offset lookup, so ONLY the two
//       timers BarcodePosDelay[iSht] / BarcodeDelay[iSht] land in Shuttle-1's
//       slot.  Harmless in practice (the two sweeps are not run concurrently)
//       but it IS the same copy-paste shape.  REPRODUCED, NOT FIXED.
//   Q6. Do2DIDCheckSh2 golden :5602, non-OCR branch: after a SUCCESSFUL Row-B
//       scan it writes the result into the grid at row `iBarCodeRowA` --
//       `mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1,
//       BLCarryKit.cDeviceInf[1][iNowCheckStep])` -- overwriting Row A's own
//       cell with Row B's code, while the surrounding Row-A branch (:5589) and
//       BOTH arms of the OCR branch (:5558/:5571) correctly use their own row.
//       Display-only (the cDeviceInf data itself is stored correctly), but it
//       is a real golden defect.  REPRODUCED, NOT FIXED.
//   Q7. Do2DIDCheckSh2 case 1200 (golden :5636-5652) falls through into case
//       1300 on the `else` path (it `break`s only on the loop-again path).
//       Fall-through preserved.
//   Q8. Do2DIDCheckSh2's leading `if(InArmSuck.iShtRow==1)` row-swap block
//       (golden :5434-5446) keys on TestIF_File.iSelectUseCCDSh2, matching the
//       already-translated BarCode_Sh2_DoBarcodeScanInShuttle_2 -- but WITHOUT
//       that function's extra `else if(BAR_CODE_INSTALL==ebcUseOCR)` arm.
//       Preserved as golden has it (2-branch, not 3-branch).
//
//  SHARED golden TfBarCode STATE THIS FILE REUSES (does NOT redefine)
//  ---------------------------------------------------------------------------
//    From BarCode_Shuttle1_Scan.h (canonical defs in BarCode_Shuttle1_Scan.cpp):
//      iSFCTotalMoveStep, SFCStartDelay[2], SFCExposureDelay[2],
//      iSFCCurrentStep[], bSFCStepError[], bSFCExposureOK[], bSFCGetResultOK[],
//      bSFCGetSE9[], iSFCCheckResult[][16].
//      These are genuinely ONE TfBarCode instance's members in golden, shared
//      with DoShuttleFloatCheck_1/_2 -- reusing rather than forking is the
//      faithful choice (same reasoning BarCode_Shuttle2_ScanRemainder2.h wrote
//      down for the identical set).
//    From BarCode_Shuttle2_Scan.h (canonical defs in BarCode_Shuttle2_Scan.cpp):
//      iBarCode2_1, iBarCode2_2, iInitialBarcodeInShuttle2Task (Do2DIDCheckSh2
//      genuinely aliases the SAME golden member cursor that
//      DoBarcodeTriggerInShuttle_2 / DoBarcodeScanInShuttle_2 use -- golden does
//      too; these three sweeps are mutually exclusive by operation mode),
//      bBarcodeStartDelay[], BarcodePosDelay[2], BarcodeDelay[].
//    From cmydef.h (real globals): IndexSuckName[4][8], asBarCodeErrorSend,
//      bBarcodeFirstAutoRetry[4], iSH2_1BarcodePosition, iSH2_2BarcodePosition,
//      BAR_CODE_INSTALL, ebcUseOCR, MInShuttle2.
//    From cprod.h (real): Prod.iInSFCPos1x*, Prod.iInSHBarCodeDetectPos*,
//      Prod.InSHT[]/OutSHT[], Offset.iSHLeft2D[], TestIF_File.*.
//    From aHotPlateSubstrate.h (real): TMyKitSuck InArmSuck / BLCarryKit --
//      see the .cpp's trap-5 note on WHICH TMyKitSuck this is.
//    From Motor/mymotor.h (real, transitively): MOT[].MotorMove.
//    From ckernel.h (real, ACTIVE bodies in ckernel.cpp): WaitManualStepKey,
//      WaitManualRetryKey.
//    From BarCode_8CCD_Glue.h (real body, BarCode_8CCD_Glue.cpp:178):
//      ::SendCCDCommand(int,AnsiString,AnsiString) -- deliberately NOT gated,
//      see the .cpp's GATE REGISTER entry G-0 for why this unit differs from
//      its older siblings here.
//
//  THIS UNIT'S ONE NEW CANONICAL GLOBAL (defined in the .cpp, nothing else in
//  the tree declares or defines it -- grep-verified, see .cpp trap-2 log):
//      iSFCAutoTune2Task   -- golden BarCode.h:885
//  (golden BarCode.h:884 `iSFCAutoTune1Task` is the Shuttle-1 twin and is NOT
//  touched here -- it belongs to the concurrently-running Sh1 counterpart unit,
//  BarCode/BarCode_Shuttle1_SFCAutoTune.cpp, which this file neither reads,
//  includes nor depends on.)
//
//  Big5: golden read with encoding='cp950'; Chinese comments transcribed
//  character-for-character.  Zero U+FFFD verified in both files of this unit.
// =============================================================================
#ifndef BarCode_Shuttle2_SFCAutoTuneH
#define BarCode_Shuttle2_SFCAutoTuneH

#include "BarCode_8CCD_Glue.h"   // real ::SendCCDCommand(int,AnsiString,AnsiString);
                                 // transitively BarCode_Bottom2DID.h ->
                                 // BarCode_Shuttle1_Scan.h (SFC shared state) +
                                 // BarCode_Shuttle2_Scan.h (iBarCode2_1/2_2,
                                 // iInitialBarcodeInShuttle2Task, bBarcodeStartDelay,
                                 // BarcodePosDelay, BarcodeDelay) + cmydef.h/cprod.h/
                                 // aHotPlateSubstrate.h/Motor/canary_support.h
#include "ckernel.h"             // WaitManualStepKey() / WaitManualRetryKey() (REAL)

// ---------------------------------------------------------------------------
//  golden BarCode.h:885 -- TfBarCode::iSFCAutoTune2Task, the Shuttle-2 SFC
//  Auto-Tune cursor.  THIS unit owns the canonical definition (in the .cpp);
//  no other translated TU declares or defines it.  Any future unit needing it
//  (e.g. whoever translates the cContact.cpp / uhome.cpp call sites) MUST
//  #include this header and reuse this extern rather than making a local copy.
// ---------------------------------------------------------------------------
extern int iSFCAutoTune2Task;   // golden BarCode.h:885

// ============================================================================
//  This unit's 3 assigned free functions.  Signatures mirror the golden
//  TfBarCode member declarations verbatim (BarCode.h:733/:893/:895) -- golden
//  gives none of them a default argument, so neither does this file.
// ============================================================================
void BarCode_Sh2_InitialSFCAutoTune2(bool bHasIC);   // golden BarCode.h:893 / BarCode_Sh2.cpp:4880
bool BarCode_Sh2_DoSFCAutoTune_2(bool bHasIC);       // golden BarCode.h:895 / BarCode_Sh2.cpp:4888
bool BarCode_Sh2_Do2DIDCheckSh2();                   // golden BarCode.h:733 / BarCode_Sh2.cpp:5419

#endif // BarCode_Shuttle2_SFCAutoTuneH
