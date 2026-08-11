// =============================================================================
//  BarCode/BarCode_Shuttle1_SFCAutoTune.h  --  Shuttle-1 SFC (Shuttle Float
//      Check) AUTO-TUNE cycle + the stand-alone manual 2DID position check,
//      from the golden TfBarCode VCL form (BCB6)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode_Sh1.cpp
//  Translator: AI(W906-PT-BarCodeSh1-SFCAutoTune) 20260811
//
// -----------------------------------------------------------------------------
//  ROLE
// -----------------------------------------------------------------------------
//  This unit closes the hand-off that BarCode_Shuttle1_Scan.h:32-34 recorded
//  verbatim:
//        "* TfBarCode::InitialSFCAutoTune1/DoSFCAutoTune_1/Do2DIDCheckSh1 --
//           golden BarCode_Sh1.cpp:4698-5533, NOT one of the 7 assigned methods
//           (not touched at all by this unit; deferred)."
//  These are the LAST 3 functions of the 5533-line golden BarCode_Sh1.cpp and
//  were the only ones in that file never translated.  With this unit, golden
//  BarCode_Sh1.cpp is 10/10 functions ported (7 in BarCode_Shuttle1_Scan.cpp,
//  1 in BarCode_Shuttle1_CCDScan.cpp, 3 here).
//
//  WHAT THEY DO
//    * InitialSFCAutoTune1(bHasIC) -- one-line cursor seed: bHasIC==false picks
//      the "AT 無料" (no-device) cycle at Task 1, true picks the "AT 有料"
//      (has-device) cycle at Task 10000.
//    * DoSFCAutoTune_1(bHasIC) -- the OPERATOR-DRIVEN auto-tune sweep of the
//      Shuttle-1 float-check CCD: for each shuttle column (descending), move
//      MInShuttle1 to that column's SFC teach position, fire an E2 (no-device)
//      or E3 (has-device) exposure at BOTH CCD rows, wait for the exposure ack,
//      then step to the next column.  Its `bHasIC` PARAMETER IS DEAD in golden
//      -- the cycle is selected purely by the cursor InitialSFCAutoTune1 seeded.
//      Kept verbatim (fidelity, and the golden call sites pass it).
//    * Do2DIDCheckSh1() -- the manual/teach-mode 2DID read sweep: same
//      column-descending move ladder, but reading real 2D codes into
//      FLCarryKit.cDeviceInf via Barcode_StartScan_In, gated on the operator's
//      Retry/Step key (WaitManualRetryKey/WaitManualStepKey).
//
//  WHO PUMPS IT -- NOBODY IN THIS TREE TODAY.  Stated plainly, not papered over:
//  golden's only callers are cContact.cpp:12289 / :12355 / :12374 / :12395,
//  uhome.cpp:4311 and BarCode/BarCode.cpp:6839 -- ALL of which are still
//  untranslated.  So this TU compiles, enters ht9045_sm, and is never extracted
//  by the link (archive-extraction shape (a)).  That is the expected state for
//  this delivery; no caller was invented and no fake reference was added.  See
//  the .cpp report/banner and the deliverable report's trap-1 answer.
//
// -----------------------------------------------------------------------------
//  WAVE SCOPE -- one line per golden function (all ACTIVE, none GATED)
// -----------------------------------------------------------------------------
//    ACTIVE  InitialSFCAutoTune1(bool bHasIC)   golden BarCode_Sh1.cpp:4698-4705
//                                               decl golden BarCode.h:892
//    ACTIVE  DoSFCAutoTune_1(bool bHasIC)       golden BarCode_Sh1.cpp:4706-5285
//                                               decl golden BarCode.h:894
//    ACTIVE  Do2DIDCheckSh1()                   golden BarCode_Sh1.cpp:5287-5533
//                                               decl golden BarCode.h:732
//  ("ACTIVE" = the function body itself is translated in full and reachable.
//   Individual CALLS inside them that route to an offline stand-in are the
//   GATE REGISTER below; no whole function is `#if 0`.)
//
// -----------------------------------------------------------------------------
//  GATE REGISTER (summary -- full (a)/(b)/(c) entries in the .cpp banner)
// -----------------------------------------------------------------------------
//   #  golden line                          gated call            offline default
//   1  BarCode.h:200  mtSFC_Shuttle->SetCellNumber        SFCAT1_SFCGridSetCell            no-op
//   2  BarCode.h:145  mtBarcodeInSh->SetCellNumber        SFCAT1_BarcodeInShGridSetCell    no-op
//   3  note.h         fNote->t2DCode->SetCellNumber       SFCAT1_Note2DSetCell             no-op
//   4  note.h         fNote->t2DCode->SetCellColorIndex   SFCAT1_Note2DSetCellColorIndex   no-op
//   5  BarCode.cpp:5903 AddCCDCommunicationLog            SFCAT1_AddCCDCommunicationLog    stdout log
//   6  BarCode.cpp:2512 InitSht2DCodeComPort              SFCAT1_InitSht2DCodeComPort      no-op
//   7  BarCode.cpp:2097 Barcode_StartScan_In              SFCAT1_Barcode_StartScan_In      false ("not read yet")
//  NOT A GATE, faithful local mirror:
//      BarCode.cpp:3128 SetSFCCheckStepCount              SFCAT1_SetSFCCheckStepCount      real 2-line calc
//  NOT GATED -- REAL dependency, and a deliberate DIVERGENCE from the sibling
//  BarCode_Shuttle1_Scan.cpp (whose "SendCCDCommand has no port" premise EXPIRED):
//      BarCode.cpp:5305 SendCCDCommand   -> the REAL body at
//      BarCode/BarCode_8CCD_Glue.cpp:178, declared BarCode_8CCD_Glue.h:216.
//      See the .cpp banner "TRAP-2 EXPIRED-ABSENCE FINDING" block.
//
// -----------------------------------------------------------------------------
//  NEW SINGLE-OWNER GLOBALS this unit introduces (nothing else in the tree
//  declares or defines either -- re-verified 2026-08-11, see .cpp banner)
// -----------------------------------------------------------------------------
//    int  iSFCAutoTune1Task            golden BarCode.h:884
//    bool bSFCError[BAR_CODE_COUNT]    golden BarCode.h:907
//  Re-verified by a full 1142-file walk of the port tree at 2026-08-11 11:35,
//  AFTER the concurrent sibling BarCode/BarCode_Shuttle2_SFCAutoTune.{h,cpp}
//  landed (11:27 / 11:32) -- that unit owns iSFCAutoTune2Task and says in its
//  own header that iSFCAutoTune1Task is left to this unit; bSFCError has 0 hits
//  anywhere else in the tree, and golden's BarCode_Sh2.cpp has 0 occurrences of
//  it either, so Shuttle-1 is genuinely its only user.  See the .cpp's
//  "TRAP-2 FINAL SWEEP" block.
//  bSFCError's OTHER golden writer is BarCode.cpp:3602 (`bSFCError[Tag]=true;`
//  inside the CCD ClientSocket Read handler, which has NO port anywhere) -- so
//  offline it can only ever be false, and the exposure-error arms of
//  DoSFCAutoTune_1 (cases 1150/1170/11150/11170) are unreachable until that
//  handler is ported.  Whoever ports BarCode.cpp's socket Read handler MUST
//  reuse THIS extern rather than define a second one.
//
//  Big5: golden read via cp950; Chinese comments transcribed verbatim.
//  Zero U+FFFD verified in both files of this unit.
// =============================================================================
#ifndef BarCode_Shuttle1_SFCAutoTuneH
#define BarCode_Shuttle1_SFCAutoTuneH

// BarCode_Shuttle1_Scan.h is the canonical owner of every piece of shared
// golden TfBarCode SFC/Shuttle-1 state these three functions touch:
//   iInitialBarcodeInShuttle1Task (Do2DIDCheckSh1's Task cursor -- an ALIAS of
//     the very same cursor DoBarcodeTriggerInShuttle_1 / DoBarcodeScanInShuttle_1
//     / DoBarcodeCCDInShuttle_1 drive; golden's own design, see that header),
//   iSFCTotalMoveStep, iSFCCurrentStep[], bSFCStepError[], bSFCExposureOK[],
//   bSFCGetSE9[], iSFCCheckResult[][16], SFCStartDelay[], SFCExposureDelay[],
//   BAR_CODE_COUNT, iBarCode1_1/iBarCode1_2,
// and (transitively, via BarCode_Shuttle2_Scan.h) bBarcodeStartDelay[],
// BarcodePosDelay[], BarcodeDelay[]; and (via cmydef.h) iSH1_1BarcodePosition /
// iSH1_2BarcodePosition / bBarcodeFirstAutoRetry[] / asBarCodeErrorSend /
// IndexSuckName[][] / BAR_CODE_INSTALL; and (via aHotPlateSubstrate.h)
// InArmSuck / FLCarryKit / MOT[].  NONE of these is redefined here.
#include "BarCode_Shuttle1_Scan.h"

// BarCode_8CCD_Glue.h supplies the REAL SendCCDCommand(int,AnsiString,AnsiString)
// (golden BarCode.cpp:5305; port body BarCode_8CCD_Glue.cpp:178).  Included --
// not forward-declared -- specifically so the declaration this TU compiles
// against is BY CONSTRUCTION the one whose definition it links to.
#include "BarCode_8CCD_Glue.h"

// ckernel.h supplies the REAL WaitManualRetryKey()/WaitManualStepKey()
// (golden ckernel.cpp:133-149 / :54-94; port bodies ckernel.cpp:352 / :226).
#include "ckernel.h"

// ---------------------------------------------------------------------------
//  Golden TfBarCode instance data owned by THIS unit (see banner).
// ---------------------------------------------------------------------------
extern int  iSFCAutoTune1Task;              // golden BarCode.h:884
extern bool bSFCError[BAR_CODE_COUNT];      // golden BarCode.h:907

// ---------------------------------------------------------------------------
//  The 3 assigned functions, as FREE FUNCTIONS carrying their PLAIN golden
//  names.  Naming rationale (the task asked for it explicitly): the sibling
//  BarCode_Sh2_* prefix exists only because BarCode_Shuttle2_Scan.cpp's
//  functions WOULD have collided with the already-published Shuttle-1 free
//  functions of the same golden name.  Here there is no collision to avoid:
//  none of these 3 names is a TfBarCode_Shim method (the shim's 20 methods are
//  listed at aHotPlateSubstrate.h:987-1012 and none of them is an SFC-auto-tune
//  or a Do2DIDCheck), and a tree-wide search on 2026-08-11 found the 3 names
//  nowhere in the tree except the deferral note at BarCode_Shuttle1_Scan.h:32.
//  So per the task's "prefer plain names if no collision exists" they keep the
//  golden spelling -- exactly as InitialBarcodeScanInShuttle1 /
//  DoBarcodeTriggerInShuttle_1 / DoShuttleFloatCheck_1 already do in
//  BarCode_Shuttle1_Scan.h:188-194.  Default args: golden BarCode.h has NONE on
//  any of the three, so none are added.
// ---------------------------------------------------------------------------
void InitialSFCAutoTune1(bool bHasIC);      // golden BarCode_Sh1.cpp:4698 / BarCode.h:892
bool DoSFCAutoTune_1(bool bHasIC);          // golden BarCode_Sh1.cpp:4706 / BarCode.h:894
bool Do2DIDCheckSh1();                      // golden BarCode_Sh1.cpp:5287 / BarCode.h:732

#endif // BarCode_Shuttle1_SFCAutoTuneH
