// =============================================================================
//  BarCode/BarCode_Shuttle2_ScanRemainder1.h  --  TfBarCode::DoBarcodeScanInShuttle_2
//                                                  (Shuttle-2 "Scan" IN-shuttle 2DID SM)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode_Sh2.cpp
//                   :3040-4260 (1221 lines)              //various authors, latest
//                                                          //Eastsun 20260526 (Pin1)
//  Golden decl home: BarCode/BarCode.h:804 (TfBarCode class, public method).
//
//  Translation wave: W5-BarCode hand-off completion (this is the 1st of the
//  2-method "REMAINING / HANDED OFF" leftover flagged by BarCode_Shuttle2_Scan.h's
//  own translate report -- see that header's banner, section "REMAINING / HANDED
//  OFF").  The sibling method from that same hand-off, `DoShuttleFloatCheck_2`,
//  is NOT part of this unit -- it is delivered separately in the already-landed
//  BarCode_Shuttle2_ScanRemainder2.{h,cpp} (a peer unit from the same wave, still
//  pending integrate as of this writing).  Only `DoBarcodeScanInShuttle_2` is
//  delivered here, in its own "Remainder1" file (distinct name from both the
//  original BarCode_Shuttle2_Scan.{h,cpp} -- which this task's boundary does not
//  permit editing -- and from the sibling Remainder2 file).
//  Translator: AI(W5-Final-BarCodeShuttle2ScanRemainder1) 20260711.
//
//  WHY A SEPARATE TU (not folded into BarCode_Shuttle2_Scan.cpp)
//  ---------------------------------------------------------------------------
//  BarCode_Shuttle2_Scan.{h,cpp} is a prior-wave deliverable this task's
//  boundary does not include editing (only the two NEW files named by the task:
//  BarCode_Shuttle2_ScanRemainder1.{h,cpp}).  This file therefore #includes
//  BarCode_Bottom2DID.h (which itself pulls in BOTH BarCode_Shuttle1_Scan.h AND
//  BarCode_Shuttle2_Scan.h transitively -- verified by reading all three headers)
//  to reuse -- NOT redefine -- every piece of shared TfBarCode instance state
//  this function touches.  This file defines nothing that any other
//  already-delivered TU also defines.
//
//  SCOPE
//  -----
//  golden BarCode_Sh2.cpp:3040-4260 (`bool TfBarCode::DoBarcodeScanInShuttle_2
//  (bool bErrorSkip)`) -- exact bounds re-verified by this unit directly against
//  the golden file (grep for the enclosing function signatures immediately
//  before/after): starts at :3040 (`bool TfBarCode::DoBarcodeScanInShuttle_2
//  (bool bErrorSkip)`), ends at :4260 (closing `}` immediately before the
//  `//---...` separator and the next function `DoBarcodeScanOutShuttle_2` at
//  :4262).  1221 lines total, matching the task brief's "~1222 lines" estimate.
//  Read and translated IN FULL this unit (not a partial hand-off -- the whole
//  function turned out to be tractable once cross-checked against how much
//  supporting infrastructure the parallel W5-BarCode wave had already landed on
//  disk by the time this unit ran; see "INFRASTRUCTURE ALREADY LANDED" below).
//
//  INFRASTRUCTURE ALREADY LANDED (discrepancy vs. the task brief's gate list --
//  golden/actual-code wins, per this project's own instructions; see translate
//  report item 8)
//  ---------------------------------------------------------------------------
//  The task brief (written by an earlier feasibility-assessment agent that had
//  only read ~54% of this function) called out `bBarcodeFirstAutoRetry[]`/
//  `iNeedBarcodeCount[]`/`bBarcodeAutoSkip[]`/`bImageSaveflag[]`,
//  `iBarcodeAutoRetry[]`, and `CosFunction.bUseHandShakeCommunication`/
//  `b2DUsePinInspection` as NEW gated stubs this unit would need to add.  By the
//  time this unit actually ran, EVERY one of those was already real:
//    * bBarcodeFirstAutoRetry[4] / iNeedBarcodeCount[4] / bBarcodeAutoSkip[4] /
//      iBarcodeAutoRetry[4] / bCheckCodeError[4] / bCheckLotError[4] /
//      bBarcodeAutoSkipError[4] / iBarcodeErrorCount[4] / iBarcodeReject /
//      bBarcodePassword / asBarcodeLotNumber -- all real `extern` globals
//      already defined in cmydef.h/cmydef.cpp (verified directly; also already
//      documented as real by the sibling BarCode_Shuttle1_Scan.h's own "ODR
//      RESOLVED" banner).  NOT gated here -- used as-is.
//    * bImageSaveflag[BAR_CODE_COUNT] / bHasPin1Error[BAR_CODE_COUNT] /
//      BarcodeWaitReplyTimeOut[BAR_CODE_COUNT] / bCCDBarcodeWaitReply
//      [BAR_CODE_COUNT] -- all real, canonically defined in
//      BarCode_Shuttle1_Scan.cpp (reused via #include, NOT redefined -- see
//      "GENUINE DISCREPANCY FOUND" below, these are actually NOT Shuttle-1-only).
//    * `CosFunction.bUseHandShakeCommunication` (CosFunction.h:156) and
//      `CosFunction.b2DUsePinInspection` (CosFunction.h:345) -- BOTH fields
//      ALREADY EXIST in the real (non-ban-listed) CosFunction.h, added by an
//      earlier BarCode-wave unit (the Sh1/Sh2 CCDScan units already reference
//      them).  This unit does NOT add anything to CosFunction.h -- verified by
//      direct grep immediately before writing this file.
//    * `TMyProductionRecord::Add2DIDRecord(AnsiString)` -- declared
//      Public/MyProductionRecord.h:42, REAL no-op body already defined
//      aHotPlateSubstrate.cpp:479 (added by the original W5-BarCode integrate
//      agent).  Called directly via `BLCarryKit.PordRec[i][j].Add2DIDRecord(...)`
//      -- NOT gated.
//    * `list2DByLot` (TStringList*) -- real, canonically defined
//      BarCode_Bottom2DID.cpp (`extern TStringList *list2DByLot;` in
//      BarCode_Bottom2DID.h).  Reused via #include -- its ->Clear()/->Add()/
//      ->Text are all real vclcompat TStringList members.  NOT gated (only the
//      one downstream WriteDataToFile(asBarCodeLot, list2DByLot->Text) call is
//      gated -- see GATE MAP below -- so list2DByLot's own mutations are kept
//      real/faithful even though their file-write consumer is stubbed).
//
//  GENUINE DISCREPANCY FOUND (flag for review -- NOT fixed by this unit, since
//  fixing it means editing a sibling file this task's boundary forbids)
//  ---------------------------------------------------------------------------
//  BarCode_Shuttle1_Scan.h:152-155 comments `bCCDBarcodeWaitReply`/
//  `bHasPin1Error`/`bImageSaveflag`/`BarcodeWaitReplyTimeOut` as
//  "Shuttle-1-only (Sh2 unit does not touch)".  That claim is FALSE as of this
//  unit's reading of the FULL golden DoBarcodeScanInShuttle_2 body (golden
//  :3278-3280, :3319-3324, :3355-3360, :3480-3485, :3537-3538, :3544-3554,
//  :3560, :3689-3694, :3703-3711, :3741, :3743, :3839): this function reads AND
//  writes all four arrays at the SAME BAR_CODE_COUNT-sized indices (iBarCode2_1
//  ==2 / iBarCode2_2==3) the Shuttle-1 side uses at indices 0/1 -- i.e. these
//  are genuinely GLOBAL per-logical-CCD-channel arrays (BAR_CODE_COUNT==8,
//  channels shared across both shuttle sides), not Shuttle-1-private state.  The
//  comment was written by the Sh1 translator based on grepping only their own
//  7 assigned Sh1 functions -- they could not have known this (not-yet-
//  translated-at-the-time) Sh2 function also touches the same arrays.  This
//  unit does NOT edit BarCode_Shuttle1_Scan.h (sibling file, out of this task's
//  boundary) to correct the stale comment -- flagged in the translate report
//  for the integrate agent to fix (comment-only change, zero behavioural risk:
//  the arrays are already correctly SHARED/real either way, only the
//  explanatory comment is wrong).
//
//  GOLDEN QUIRKS PRESERVED VERBATIM (NOT "fixed" here)
//  ---------------------------------------------------------------------------
//  1. Row-selection asymmetry vs. the sibling InitialBarcodeScanInShuttle2:
//     this function additionally has an `else if(BAR_CODE_INSTALL==ebcUseOCR)`
//     branch (golden :3074-3078) that InitialBarcodeScanInShuttle2 (already
//     translated, BarCode_Shuttle2_Scan.cpp) does not have.  Reproduced as-is.
//  2. Unlike the Shuttle-1 sibling `DoBarcodeScanInShuttle_1`
//     (BarCode_Shuttle1_Scan.cpp case 1180/case bBarcodeAutoSkipError branch),
//     which selects the IndexSuckName[] row via `IsNNMode()==NN_2Row/NN_1Row/
//     else` (a later "Steven 20220315" NN-mode enhancement), THIS function's
//     equivalent case-1180 branches (golden :3762-3810, :3889-3938,
//     :3950-4006, :4020-4061) select IndexSuckName[] purely on
//     `InArmSuck.iShtRow==2` (2-way, rows 0/1) with NO IsNNMode() call anywhere
//     in this function (verified by grep over the full golden body: zero
//     IsNNMode/NN_1Row/NN_2Row hits).  Looks like the same class of "later
//     enhancement applied to Shuttle-1 only, never back-ported to Shuttle-2"
//     already documented by the sibling Remainder2 unit for
//     DoShuttleFloatCheck_2 -- NOT fixed here, reproduced exactly.
//  3. Case 1300's golden `else` branch (iNowCheckStep out of range) sets
//     Task=2000 and then has a COMMENTED-OUT `break;` (golden :4163
//     `//            break;`) immediately before `case 2000:` -- i.e. golden
//     deliberately falls through from case 1300 into case 2000 in that branch
//     (the in-range branch above it has its own real, uncommented `break;`).
//     Reproduced verbatim (no `break;` added at that spot).
//  4. Case 1 and case 1000 both fall through into the next case with NO
//     `break;` in golden (case 1 -> case 1000 always executes case 1000's body
//     the same tick regardless of which value the case-1 if/else assigned to
//     Task; case 1000 -> case 1100 likewise; case 3000 -> case 3100 likewise).
//     Reproduced verbatim, same as every other DoXxx state-machine in this
//     tree.
//
//  Already-available real dependencies reused (NOT gated) -- pulled in via
//  BarCode_Bottom2DID.h transitively (which itself pulls in
//  BarCode_Shuttle1_Scan.h + BarCode_Shuttle2_Scan.h) plus a few direct includes
//  below:
//    InArmSuck / BLCarryKit (aHotPlateSubstrate.h, TMyKitSuck: iShtRow/iShtCol/
//      iMaxRow/iMaxCol/Item[][]/cDeviceInf[][]/SetItemData/PordRec[][]
//      .Add2DIDRecord -- the last one a REAL no-op body, see above)
//    MOT[] / TTrayMotor::MotorMove / MInShuttle2 (Motor/mymotor.h, cmydef.h)
//    Prod / Offset / TestIF_File (cprod.h)  IniConfig (Config.h)
//    CosFunction.bUseHandShakeCommunication / .b2DUsePinInspection (CosFunction.h,
//      real fields, see above)
//    NULL_IC / HAS_NULL_IC / DUMMY / asBarCodeErrorSend / asBarCodeErrorCheckSum /
//      BAR_CODE_INSTALL / ebcUseOCR / ebctUseCCDMode / ebctEtherNetCCD / eAMD /
//      IndexSuckName[4][8] / iSH2_1BarcodePosition / iSH2_2BarcodePosition /
//      SystemYear/Month/Date/Hour/Min/Sec (cmydef.h / MachineType.h, real)
//    iBarCode2_1 / iBarCode2_2 / iInitialBarcodeInShuttle2Task /
//      bBarcodeStartDelay[] / bBarcodeDataSaveReady[] / BarcodePosDelay[] /
//      BarcodeDelay[] (BarCode_Shuttle2_Scan.h, canonical defs in
//      BarCode_Shuttle2_Scan.cpp -- SAME shared `iInitialBarcodeInShuttle2Task`
//      cursor the already-translated DoBarcodeTriggerInShuttle_2 uses; golden
//      genuinely aliases the same class-member cursor across both mutually-
//      exclusive Trigger/Scan protocol implementations, selected by
//      config -- same pattern already documented for DoBarcodeCCDInShuttle_2)
//    bHasPin1Error[] / BarcodeWaitReplyTimeOut[] / bCCDBarcodeWaitReply[] /
//      bImageSaveflag[] (BarCode_Shuttle1_Scan.h, canonical defs in
//      BarCode_Shuttle1_Scan.cpp -- see "GENUINE DISCREPANCY FOUND" above)
//    list2DByLot (BarCode_Bottom2DID.h, canonical def in BarCode_Bottom2DID.cpp)
//    ShowMyMessage / ShowErrorMessage / K_RETRY / K_SKIP / LastSet.iRealDummy
//      (canary_support.h)
//    fContact->IsRun2DCheck() (atester_shims.h)
//    DoInOutARM_SHT_MoveSafe(int) (acarry.h, real W6.5 body)
//    asBarCodeLogPath / asBarCodeLot (common.h, real data)
//
//  GATE MAP (this unit's own local, internal-linkage stand-ins -- anonymous
//  namespace in the .cpp, named `Gated_*` per the established convention already
//  used by the original BarCode_Shuttle2_Scan.cpp and the sibling
//  BarCode_Shuttle2_ScanRemainder2.cpp; distinct linkage per TU so none of these
//  collide with any other file's own `Gated_*` helpers of the same name):
//    Gated_GridSetCell / Gated_GridGetCell     <- mtBarcodeInSh->SetCellNumber/
//                                                  GetCellText (BarCode.h:145,
//                                                  TTMyTray*, W7)
//    Gated_LotInfoGridSetCell                   <- fLotInfo->mtBarcodeInShLotInfo
//                                                  ->SetCellNumber (uLotInfo.h
//                                                  TfLotInfo, not translated)
//    Gated_Note2DSetCell / SetCellColor /
//      SetXItem / SetYItem                      <- fNote->t2DCode->... (note.h
//                                                  TfNote, not translated)
//    Gated_ClickSaveImage                       <- btSaveImage->Click() (BarCode.h
//                                                  TButton widget)
//    Gated_ShowNowStatus                        <- fMain->ShowNowStatus(...)
//                                                  (FormsFacade has no such
//                                                  method yet)
//    Gated_TurnOffInspection                    <- TfBarCode::TurnOffInspection
//                                                  (BarCode.cpp:2364, not one of
//                                                  our 20 shim methods)
//    Gated_DoBarcodeCount                       <- TfBarCode::DoBarcodeCount
//                                                  (BarCode.cpp:5841)
//    Gated_InitSht2DCodeComPort                 <- TfBarCode::InitSht2DCodeComPort
//                                                  (BarCode.cpp:2512)
//    Gated_CleanBarcodeError                     <- TfBarCode::CleanBarcodeError
//                                                  (BarCode.cpp:6622)
//    Gated_Barcode_StartScan_In                  <- TfBarCode::Barcode_StartScan_In
//                                                  (BarCode.cpp:2097, ~470 lines)
//    Gated_DoCheckSHT2DIDStatus                  <- fBarCode->DoCheckSHT2DIDStatus
//                                                  (another TfBarCode method, not
//                                                  one of our 20/shim surface)
//    Gated_RunCheckBarcodeByServerData            <- fBarCode->
//                                                  RunCheckBarcodeByServerData
//                                                  (ditto)
//    Gated_IsMatchServerData                      <- fMesSystem->IsMatchServerData
//                                                  (Mes/json cross-module,
//                                                  explicit gate per task brief)
//    Gated_WriteDataToFile / Gated_MyForceDirectories <- common.h
//                                                  WriteDataToFile/
//                                                  MyForceDirectories (bodies
//                                                  themselves `#if 0` in
//                                                  common.cpp -- TODO(wave-file))
//    Gated_ShowMyMessageBox_YES_SKIP               <- golden mymessbox.h:54
//                                                  (blocking VCL Yes/Skip
//                                                  dialog); mirrors the ALREADY-
//                                                  translated sibling
//                                                  BarCode_Shuttle2_Scan.cpp's
//                                                  own `Gated_ShowMyMessageBox_
//                                                  YES_SKIP` conservative
//                                                  default (return 0 == Skip),
//                                                  NOT Sh1's differently-chosen
//                                                  real global of the same
//                                                  golden name (Sh1 elected to
//                                                  give it a real Sim/log body
//                                                  under the bare golden name;
//                                                  this unit instead follows its
//                                                  OWN direct-sibling Sh2
//                                                  file's precedent, for
//                                                  internal consistency within
//                                                  the same original golden
//                                                  source file family).
//
//  A local, pure-dedup helper (NOT a gate -- real logic, just factored to avoid
//  repeating the identical golden if/else-if ladder twice in this file, same
//  spirit as BarCode_Shuttle1_Scan.cpp's own `BarSh1_GetInSHBarCodeDetectPos`):
//    Gated_GetInSHBarCodeDetectPos(iShtCol, iStep, funcName) -- golden repeats
//      `if(iShtCol==1) pos=Prod.iInSHBarCodeDetectPos1x1[1]; else if(...)...`
//      verbatim at case 1100 (golden :3213-3244) and case 1160 (golden
//      :3640-3671) of THIS function only (Shuttle-2 row index [1], NOT Sh1's
//      [0]).
//
//  FLOATING-POINT TRUNCATION ORDER (documented once here, applies at both call
//  sites in the .cpp): golden repeatedly does `pos+=Offset.iSHLeft2D[1];` where
//  `pos` is `int` and `iSHLeft2D` is `double` (cprod.h).  Per the C++ standard's
//  compound-assignment semantics ([expr.ass]), `pos+=x` for mismatched types is
//  defined as "compute `pos+x` using the usual arithmetic conversions (i.e. in
//  double), THEN convert the RESULT back to int for the store" -- i.e. it
//  already truncates the SUM, not the addend, exactly matching golden.  This
//  unit writes it explicitly as `pos=(int)(pos+Offset.iSHLeft2D[1]);` purely to
//  keep this tree's `-Wconversion`-clean convention (an implicit narrowing
//  inside `+=` would warn; the explicit cast does not) -- NOT because the
//  semantics differ, they don't.  Same explicit-cast treatment applied to the
//  `TestIF_File.dRetryShiftOffsetMove*100` / `dRetryOffsetMove*100` adjustments
//  a few lines below each occurrence.  Mirrors the already-established
//  precedent + rationale left by the original BarCode_Shuttle2_Scan.cpp unit at
//  its own (different) `pos+=Offset.iSHLeft2D[1]` call site.
//
//  Big5: golden read via cp950-aware iconv; zero U+FFFD verified in this file.
// =============================================================================
#ifndef BarCode_Shuttle2_ScanRemainder1H
#define BarCode_Shuttle2_ScanRemainder1H

#include "BarCode_Bottom2DID.h"      // pulls in BarCode_Shuttle1_Scan.h + BarCode_Shuttle2_Scan.h
                                     // transitively (shared TfBarCode state this
                                     // function reuses) + list2DByLot (real)
#include "acarry.h"                  // DoInOutARM_SHT_MoveSafe (real, W6.5 body)
#include "common.h"                  // asBarCodeLogPath / asBarCodeLot (real data;
                                     // WriteDataToFile/MyForceDirectories declared
                                     // but their bodies are themselves `#if 0` in
                                     // common.cpp -- this unit gates its own calls)

// ============================================================================
//  This unit's one assigned free function (exact signature per
//  aHotPlateSubstrate.h:641 -- the TfBarCode_Shim delegation target).
// ============================================================================
bool BarCode_Sh2_DoBarcodeScanInShuttle_2(bool bErrorSkip = false);   // golden BarCode.h:804 / BarCode_Sh2.cpp:3040

#endif // BarCode_Shuttle2_ScanRemainder1H
