//---------------------------------------------------------------------------
//  BarCode_Bottom2DID8CCD.h  --  Bottom-2DID 8-CCD scan
//                                (TfBarCode_Shim::DoBottom2DID_8CCD_Scan real body)
//
//  Translation wave: W5-BarCode (BarCode/BarCode.cpp front)
//  Translator: AI(W5-BarCode-Bottom2DID8CCD-Translate) 20260711
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode.cpp
//                 TfBarCode::DoBottom2DID_8CCD_Scan()   golden :9940-11278 (1339 lines)
//                 TfBarCode::CheckWhichKitBottom2DID()  golden :7092-7134  (helper, faithful)
//                 TfBarCode::DoBarcodeCount()           golden :5841-5902  (helper, faithful math)
//
//  SCOPE (narrow, per this wave's hand-off -- do NOT expand): only the ONE
//  TfBarCode_Shim method `bool DoBottom2DID_8CCD_Scan()` (aHotPlateSubstrate.h:628)
//  is this unit's assignment.  CheckWhichKitBottom2DID()/DoBarcodeCount() are
//  private TfBarCode helpers (not part of the 17-method shim list) that
//  DoBottom2DID_8CCD_Scan calls; both are pure Item-grid/arithmetic logic with
//  no HW/VCL coupling of their own (only cosmetic StringGrid writes, gated --
//  see .cpp), so they are translated FOR REAL here rather than stubbed.
//
//  ============================================================================
//  RECONCILIATION WITH ALREADY-EXISTING SIBLING UNITS (discovered mid-translate
//  -- IMPORTANT, read before touching any BarCode_* file)
//  ============================================================================
//  This unit was written expecting to be first to define the Bottom2DID-family
//  shared class state (bCCDBarcodeExposureOK[]/bStepError[]/bGetSE9[]/
//  iCurrentStep[]/bNeedScanBottom2D[][]/iBottomKit/iBottom2DID_8CCDTask/
//  s2DIDYield/map2DShuttleList[]/list2DByLot/iBarCode1_1) -- golden BarCode.h
//  class members genuinely shared with the 1-CCD sibling
//  `TfBarCode::DoBottom2DIDScan`/`InitBottom2DIDScan`.  Mid-translate, a
//  pre-existing parallel unit was found ALREADY on disk:
//      BarCode/BarCode_Bottom2DID.h/.cpp   (InitBottom2DIDScan/DoBottom2DIDScan,
//                                           1-CCD sibling -- its own banner
//                                           EXPLICITLY names this unit's
//                                           DoBottom2DID_8CCD_Scan as a co-owner
//                                           of this exact shared state)
//      BarCode/BarCode_Shuttle1_Scan.h      (iBarCode1_1/iBarCode1_2 enum)
//      BarCode/BarCode_Shuttle2_Scan.h/.cpp (iBarCode2_1/iBarCode2_2, map2DList,
//                                           iShtDupilcateFailCnt[2], BAR_CODE_COUNT)
//  Rather than re-defining the SAME golden fields a second time (guaranteed
//  duplicate-definition link error if both TUs ship), this unit now
//  `#include`s `BarCode/BarCode_Bottom2DID.h` and consumes its externs
//  directly.  See this unit's translate report item 6 for the full
//  before/after symbol-ownership table.
//
//  What THIS unit still uniquely owns (verified 0 references in every sibling
//  file that exists on disk at the time this unit ran):
//    * bCCDBarcodeExposureError[BAR_CODE_COUNT]  -- golden BarCode.h:743, used
//      ONLY by the 8-CCD variant (KaiHuang 20200824 exposure-timeout guard).
//
//  Bottom2DPosDelay / Bottom2DExposureDelay / Bottom2DDelay (TQPF_Timer, golden
//  BarCode.h:872-874) are ALSO already genuinely defined in
//  BarCode_Bottom2DID.cpp (confirmed via `nm` object-file symbol-table
//  cross-check -- an earlier grep pass during this translate mistakenly
//  concluded they were undefined there; they are not).  This unit only
//  extern-declares them below and consumes that existing definition, same as
//  every other Bottom2DID-family field -- it does NOT define them (that would
//  be a real "multiple definition" link error).
//  ============================================================================
//
//  TRANSLATION RULES (mirrors atester.cpp/ainarm9045.cpp/aTester_Front.cpp
//  conventions established earlier in this migration)
//  --------------------------------------------------------------------------
//    * Off Borland: __fastcall removed; AnsiString/TStringList/TQPF_Timer via
//      vclcompat; numeric semantics EXACT (no int/float rewrites).
//    * SOFT_SIMULTE is NOT defined in this target build -- the golden
//      `#ifdef SOFT_SIMULTE / #else` blocks are reproduced VERBATIM (comment
//      only where the #else branch is trivial/absent) exactly as golden; the
//      preprocessor selects the non-SOFT_SIMULTE path, same as every other
//      translated engine file.
//    * Big5 Chinese comments decoded via cp950 (`iconv -f CP950 -t UTF-8`),
//      preserved as UTF-8 in this file.  Zero U+FFFD verified (see report).
//    * Faithful BCB6 quirks preserved, NOT fixed (documented at each site):
//        - `bNeesScanCode[2][4]` (golden BarCode.cpp:9953/:10032) is a golden
//          TYPO (`Neesd`->`Nees`) for a write-only/dead array: zeroed at case 1,
//          never read anywhere in the function.  Preserved verbatim.
//        - case 2500 golden has NO `break;` (the golden source itself shows
//          `//            break;` commented OUT) -- explicit intentional
//          fall-through into case 2600.  Preserved verbatim.
//        - case 3600's `if(ret==K_RETRY)` reads whatever `ret` held from a
//          PRIOR pass through this cursor when `bLotCheckError!=true` on THIS
//          pass (golden only assigns `ret` inside `if(bLotCheckError==true)`).
//          Preserved verbatim (not "fixed" to a fresh default).
//        - case 5000/5100 fallthrough: when `DoBarcodeCount()` is false, Task
//          is set to 5100 with NO break, immediately re-entering `case 5100:`
//          which re-assigns Task=1 (redundant but harmless) then returns true.
//          Preserved verbatim.
//
//  GATED DEPENDENCIES (per the preamble's "gating hardware/cross-module calls"
//  rule -- surrounding control-flow kept ACTIVE/faithful, only the leaf HW/VCL
//  touch is gated with a documented conservative default):
//    1. 8x `ClientSocket_Bottom_1..8->Active` disconnect-guard (golden :9961-
//       10024, before the switch) -- TClientSocket, no vclcompat shim yet.
//       Gated (whole 4-line early-return block per index); conservative
//       default = always connected (never take the early-return branch).
//    2. `SendCCDCommand()` (golden BarCode.cpp:5305-5526) raw
//       `ClientSocket_Bottom_N->Socket->SendText(...)` -- same reason.  This
//       unit supplies a MINIMAL local stand-in (`Bottom2DID8CCD_SendCCDCommand`,
//       covering only the iBottom_1..8 branch, the only indices this function
//       ever passes) that keeps the log side-effect and gates only the socket
//       write (no-op success).
//    3. `mtBarcodeInSh->SetCellNumber(...)` (golden BarCode.h:145, TTMyTray
//       StringGrid-like widget, ~33 call sites) -- cosmetic UI-only writes,
//       zero control-flow influence.  ONE shared no-op stand-in
//       (`Bottom2DID8CCD_ShowKitCell`) used at every site instead of 33
//       near-identical individual `#if 0` blocks (documented compression of
//       the established atester_32Site.cpp per-site-`#if 0` cosmetic-gate
//       pattern -- same intent, far less repetition for this call volume).
//    4. `fNote->t2DCode` board (golden note.h, TfNote VCL form, NOT translated
//       anywhere yet -- unlike fContact, no shim exists) -- cosmetic
//       duplicate/lot-check display board (Visible/XItem/YItem/SetCellNumber/
//       SetCellColorIndex).  Two shared no-op stand-ins
//       (`Bottom2DID8CCD_ShowNoteBoard`/`Bottom2DID8CCD_SetNoteCell`).
//    5. `MoveInArmXY_ToBottom2DID()` (golden BarCode.cpp:7006-7091) -- moves
//       the in-arm to the Bottom-2DID camera position via
//       `InArmContinuousMove_9045` + `Tech.iBottom2DIDInX/Y` + `InArmOffSet[]`
//       (none of which are part of this unit's assigned surface; a genuine
//       in-arm-engine cross-module call, same category as the ClientSocket
//       gate).  Gated with a local stand-in returning `true` (motion "already
//       succeeded"), matching the Motor==NULL-fast-path convention used
//       elsewhere (e.g. mymotor.cpp Gali_Two_ZAxis_Move).  SHARED: the 1-CCD
//       sibling `DoBottom2DIDScan` also calls this exact golden function
//       (independently gated the same way in BarCode_Bottom2DID.cpp).
//    6. `fLotInfo->sgBarcode->Cells[...]` (golden BarCode.cpp:5854-5892, inside
//       `DoBarcodeCount`) -- TfLotInfo StringGrid, FormsFacade.h's `TfLotInfo`
//       shim has no `sgBarcode` member.  8 individual `#if 0` gates (cosmetic
//       display only); the surrounding rate/yield MATH is translated for real
//       (needed for `s2DIDYield` / the yield-alarm decision, both consumed by
//       our own case 5000/5200).
//    7. `AddCCDCommunicationLog()` (golden BarCode.cpp:5903-5975) -- golden
//       writes to a VCL Memo (`memoCCDCommLog`) AND an hourly per-day log file
//       (`MyForceDirectories`/`asBarCodeCommLogPath`/`GetTimeInfo()`).  This
//       unit supplies a SIMPLIFIED faithful stand-in
//       (`Bottom2DID8CCD_AddCCDCommunicationLog`) that keeps the same
//       index->Name resolution but routes the message through `RecordProcess`
//       (canary_support.h, an already-established Sim log sink) instead of the
//       VCL Memo + file-path machinery.  Pure side-effect logging; zero
//       influence on control flow either way.  fContact->IsRun2DCheck() is
//       NOT in this list -- a real (offline-false) shim already exists in
//       atester_shims.h and is used directly, unguarded.
//---------------------------------------------------------------------------
#ifndef BarCode_Bottom2DID8CCDH
#define BarCode_Bottom2DID8CCDH

#include "vclcompat/vcl_compat.h"       // AnsiString / TStringList
#include "myTimer.h"                    // TQPF_Timer
#include "BarCode/BarCode_Bottom2DID.h" // Bottom2DID-family shared class state (canonical
                                        // home -- see reconciliation banner above):
                                        // iBarCode1_1 (enum), BAR_CODE_COUNT,
                                        // iBottom2DID_8CCDTask, bNeedScanBottom2D[2][4],
                                        // iBottomKit, bBottom2DNeedMoveInArm,
                                        // bCCDBarcodeExposureOK[]/bCCDBarcodeGetResultOK[]/
                                        // bGetSE9[]/bStepError[]/iCurrentStep[],
                                        // s2DIDYield, map2DShuttleList[], list2DByLot;
                                        // transitively also BarCode_Shuttle2_Scan.h's
                                        // map2DList / iShtDupilcateFailCnt[2] /
                                        // iBarCode2_1/iBarCode2_2.
#include <map>

// ---------------------------------------------------------------------------
//  Per-CCD-position constants (golden ctor BarCode.cpp:208-215: iBottom_1=0 ..
//  iBottom_8=7) -- unique to the 8-CCD variant (NOT declared by
//  BarCode_Bottom2DID.h / BarCode_Shuttle1_Scan.h / BarCode_Shuttle2_Scan.h,
//  which only define the iBarCode1_1/iBarCode1_2/iBarCode2_1/iBarCode2_2
//  quartet for the 1-CCD/shuttle variants).  Plain compile-time enum -- no
//  ODR risk even if a future unit redeclares it identically in its own TU.
// ---------------------------------------------------------------------------
#ifndef HT9045_BARCODE_IBOTTOM_CONSTS
#define HT9045_BARCODE_IBOTTOM_CONSTS
enum
{
    iBottom_1 = 0, iBottom_2 = 1, iBottom_3 = 2, iBottom_4 = 3,
    iBottom_5 = 4, iBottom_6 = 5, iBottom_7 = 6, iBottom_8 = 7
};
#endif

// ---------------------------------------------------------------------------
//  bCCDBarcodeExposureError -- golden BarCode.h:743 (KaiHuang 20200824 exposure-
//  timeout guard).  Verified: 0 references in BarCode_Bottom2DID.cpp /
//  BarCode_Shuttle1_Scan.* / BarCode_Shuttle2_Scan.* -- owned here (this unit
//  is the only Bottom2DID-family method that uses it).
// ---------------------------------------------------------------------------
extern bool bCCDBarcodeExposureError[BAR_CODE_COUNT]; // golden BarCode.h:743

// ---------------------------------------------------------------------------
//  Bottom2DPosDelay / Bottom2DExposureDelay / Bottom2DDelay -- golden
//  BarCode.h:872-874.  Canonical definitions live in BarCode_Bottom2DID.cpp
//  (the 1-CCD sibling, confirmed via `nm` object-file symbol-table check --
//  see reconciliation banner above); this unit only extern-declares and
//  reuses them, exactly like the other Bottom2DID-family fields.
// ---------------------------------------------------------------------------
extern TQPF_Timer Bottom2DPosDelay;
extern TQPF_Timer Bottom2DExposureDelay;
extern TQPF_Timer Bottom2DDelay;

// ---------------------------------------------------------------------------
//  Real, faithful helper functions (pure Item-grid/arithmetic logic; only
//  their cosmetic StringGrid writes are gated -- see .cpp).
// ---------------------------------------------------------------------------
void BarCode_CheckWhichKitBottom2DID();     // golden BarCode.cpp:7092-7134
bool BarCode_DoBarcodeCount();              // golden BarCode.cpp:5841-5902

// ---------------------------------------------------------------------------
//  This unit's assignment: TfBarCode_Shim::DoBottom2DID_8CCD_Scan() real body.
//  Integrate-agent delegation (aHotPlateSubstrate.h):
//      bool TfBarCode_Shim::DoBottom2DID_8CCD_Scan()
//      {
//          return BarCode_DoBottom2DID_8CCD_Scan();
//      }
// ---------------------------------------------------------------------------
bool BarCode_DoBottom2DID_8CCD_Scan();      // golden BarCode.cpp:9940-11278

#endif // BarCode_Bottom2DID8CCDH
