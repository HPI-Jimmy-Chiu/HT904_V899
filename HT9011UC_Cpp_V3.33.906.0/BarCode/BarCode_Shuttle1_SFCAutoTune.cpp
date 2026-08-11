// =============================================================================
//  BarCode/BarCode_Shuttle1_SFCAutoTune.cpp
//
//  See BarCode_Shuttle1_SFCAutoTune.h for the ROLE / WAVE SCOPE / who-pumps-it
//  banner and the GATE REGISTER summary table.  This file carries the FULL
//  gate-register entries.
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode_Sh1.cpp
//      TfBarCode::InitialSFCAutoTune1(bool)  golden :4698-4705   (8 lines)
//      TfBarCode::DoSFCAutoTune_1(bool)      golden :4706-5285   (581 lines)
//      TfBarCode::Do2DIDCheckSh1()           golden :5287-5533   (248 lines)
//  Translator: AI(W906-PT-BarCodeSh1-SFCAutoTune) 20260811
//
// -----------------------------------------------------------------------------
//  GATE REGISTER -- one entry per gated call site introduced by this unit.
//
//  ####################################################################
//  ## AI(pt-wave) 20260811 PT-W8 integrate -- READ THIS BEFORE TRUSTING
//  ## ANY `golden :NNNN` NUMBER IN THE GATE REGISTER OR BUG NOTES BELOW.
//  ##
//  ## The golden line numbers in THIS BANNER are systematically WRONG.
//  ## An independent audit checked them against the cp950-decoded golden and
//  ## the main loop then re-verified a sample by hand.  Six confirmed examples,
//  ## claimed -> actual:
//  ##    Barcode_StartScan_In call sites  :5439 :5452 :5483 -> :5424 :5437 :5468
//  ##    BUG NOTE 2  `Task=13000`         :5267             -> :5269
//  ##    BUG NOTE 5  row-A cell/row-B data:5468             -> :5471
//  ##    BUG NOTE 7  `int pos=0, ret;`    :4718             -> :4716
//  ## The error is NOT a constant offset (it runs about +28 in GATE 1, +3 to +6
//  ## in GATE 2, +/-2 elsewhere), so it cannot be corrected by shifting.
//  ##
//  ## WHAT IS RELIABLE: the per-case `// golden :NNNN` annotations INSIDE the
//  ## function bodies below were checked 29/30 exact and can be used.
//  ## The TRANSLATED CODE ITSELF was verified independently by the main loop:
//  ## Do2DIDCheckSh1 is 230 golden code lines vs 230 port code lines with every
//  ## difference a systematic SFCAT1_ seam rename.  Only the banner's citations
//  ## are bad -- the translation is sound.
//  ##
//  ## They are left uncorrected rather than hand-patched because ~40 numbers
//  ## re-typed by hand is a good way to introduce a fresh set of wrong ones.
//  ## RECOMPUTE ANY BANNER CITATION AGAINST GOLDEN BEFORE ACTING ON IT.
//  ####################################################################
//  For each: (a) the golden line, (b) WHY the offline default is FAITHFUL,
//  (c) how REAL-MACHINE behaviour differs.  No whole function is `#if 0`.
//  Every gate helper lives in this file's ANONYMOUS namespace (internal
//  linkage), so none of them can ever satisfy, shadow or be mistaken for the
//  real symbol in another TU.
// -----------------------------------------------------------------------------
//  GATE 1 -- mtSFC_Shuttle->SetCellNumber(int,int,AnsiString)
//    (a) golden BarCode.h:200 declares `TTMyTray *mtSFC_Shuttle;` (the SFC
//        result grid on fBarCode's tsFloatResult tab -- confirmed present in
//        the dfm2rc IR as "fBarCode.Panel55.pgc2DID.tsFloating.pcShtFloat.
//        tsFloatResult.mtSFC_Shuttle", class TTMyTray).  Call sites here:
//        golden :4848 :4853 :4868 :4871 :4881 :4887 :4894 :4900 (case 1120/1150)
//        and :5118 :5123 :5138 :5141 :5151 :5157 :5164 :5170 (case 11120/11150).
//    (b) FAITHFUL because the grid is WRITE-ONLY on every one of those 16 sites
//        -- the SM never reads a cell back and never branches on grid content;
//        the only reader in golden is the human operator.  Dropping the writes
//        therefore cannot change which Task the cursor advances to.  (Contrast
//        the READ-BACK case in the sibling BarCode_Shuttle1_Scan.cpp, which had
//        to supply BarSh1_GridGetCell returning ""; no such read exists here.)
//    (c) REAL MACHINE: the operator sees per-column "SFC Exposure..." /
//        "SFC AT Exposure..." / "Waiting result.." / "Exposure time out" text
//        appear live in the SFC tab while the tune sweeps.  Offline: nothing is
//        displayed; the sweep is otherwise identical.
//        NOTE: this is deliberately a SEPARATE stand-in from GATE 2.  The
//        sibling BarCode_Shuttle1_Scan.cpp routed mtSFC_Shuttle through its
//        mtBarcodeInSh stand-in (BarSh1_GridSetCell, see its :2043); those are
//        two DIFFERENT golden widgets (BarCode.h:200 vs :145) and conflating
//        them would misreport which grid a future real UI wiring must target.
//
//  GATE 2 -- mtBarcodeInSh->SetCellNumber(int,int,AnsiString)
//    (a) golden BarCode.h:145 (the in-shuttle 2DID result grid).  Call sites
//        here: golden :5327 :5328 (case 1 clear loop), :5424 :5434 :5455 :5468
//        (case 1150 success writes) and :5487 :5493 (case 1150 timeout writes).
//    (b) FAITHFUL for the same write-only reason as GATE 1: no read-back, no
//        branch.  (Golden :5468 writes RowA's cell with RowB's data -- a golden
//        copy-paste defect preserved verbatim below, see BUG NOTE 5.)
//    (c) REAL MACHINE: the operator watches each column's decoded 2D string (or
//        asBarCodeErrorSend) land in the grid.  Offline: no display.
//
//  GATE 3/4 -- fNote->t2DCode->SetCellNumber / ->SetCellColorIndex
//    (a) golden note.h (TfNote's 2D-code alarm grid); call sites golden :5335
//        and :5336 (Do2DIDCheckSh1 case 1 clear loop, 2 sites).
//    (b) FAITHFUL because forms/fNote.h's TfNote facade DELIBERATELY does not
//        carry t2DCode -- its own banner section (4) "NOT LANDED, ON PURPOSE"
//        says so and names the other already-ported call sites (aoutarm9045.cpp
//        :388/:1460, BarCode_Bottom2DID.cpp:1180) that are all still self-gated
//        for exactly this reason.  Both calls are pure display CLEARS in the
//        init case; nothing reads the grid or its colour index back.  Choosing
//        the no-op keeps this unit consistent with the established tree-wide
//        treatment instead of unilaterally growing a facade this unit does not
//        own.  ABSENCE CLAIM, with command and time, re-run at delivery:
//          `rg -n "t2DCode" --glob "*.{h,cpp}"` over the port tree, run
//          2026-08-11 11:23 and again 2026-08-11 (final sweep) -- only comments
//          and self-gated sites, no member and no widget object anywhere.
//    (c) REAL MACHINE: the fNote alarm dialog's 2D grid is blanked (text and
//        colour) at the start of each manual check.  Offline: it is never shown
//        at all, so there is nothing to blank.
//
//  GATE 5 -- TfBarCode::AddCCDCommunicationLog(int,AnsiString)
//    (a) golden BarCode.cpp:5903-5975; call sites here golden :4736 :4795
//        :4877 :4890 :4986 :5024 :5063 :5147 :5160 :5254 :5279.
//    (b) FAITHFUL: return type void, pure trace.  Its real body writes to
//        memoCCDCommLog (a VCL TMemo with no shim) plus a per-hour Big5 log file
//        through common.h's WriteDataToFile/MyForceDirectories.
//        AI(pt-wave) 20260811 PT-W8 integrate -- CORRECTION.  This entry
//        originally continued "...whose BODIES are themselves still
//        `#if 0 // TODO(wave-file)` in the already-ported common.cpp -- i.e. the
//        real function is not callable yet even in principle."  THAT IS FALSE and
//        is struck.  All three bodies are LIVE and un-gated: common.cpp:1759 and
//        :1822 (both WriteDataToFile overloads) and :1888 (MyForceDirectories),
//        all at `#if 0` depth 0, and the string "TODO(wave-file)" does not occur
//        in common.cpp at all.  They already have live callers (acatchtray.cpp,
//        ainarm2.cpp, atester.cpp).  The stale sentence was copied from
//        BarCode_Shuttle1_Scan.cpp:44, written 2026-07-11, and was already out of
//        date when it was copied -- exactly the expiring-absence-claim trap.
//        The GATE ITSELF STILL STANDS, on the OTHER half of the reason: the Memo
//        (memoCCDCommLog) has no shim, and this is a void trace, so gating it
//        changes no control flow.  Verified by main loop, 2026-08-11.
//        ABSENCE CLAIM, with command and time: no external-linkage
//        `AddCCDCommunicationLog` exists in the tree -- every hit is either an
//        anonymous-namespace TU-local (BarCode_Bottom2DID.cpp:109), a `static`
//        (BarCode_Shuttle1_CCDScan.cpp CCD1_*), or a distinctly-named stand-in
//        (Glue8CCD_/BarSh1_/Gated_/Bottom2DID8CCD_).  Verified by
//          `grep -rn "AddCCDCommunicationLog" BarCode/ .` (excluding build*/),
//        run 2026-08-11 11:20:59 and re-run in the final sweep.
//        Routed to stdout (matching BarSh1_AddCCDCommunicationLog's precedent in
//        the sibling) so the tune trace stays visible without the VCL/file dep.
//    (c) REAL MACHINE: the operator gets the full CCD-comm trace in the Memo and
//        an on-disk Big5 log.  Offline: stdout only, no file.
//
//  GATE 6 -- TfBarCode::InitSht2DCodeComPort(int)
//    (a) golden BarCode.cpp:2512; single call site here golden :5321
//        (Do2DIDCheckSh1 case 1, comment "//Ifor 20160322 Retry 時重置").
//    (b) FAITHFUL: void, and it only re-primes the CCD serial channel before a
//        retry.  Every piece of channel state THIS function actually consumes
//        (bBarcodeStartDelay[], bBarcodeNum[], the BarcodePosDelay/BarcodeDelay
//        timers) is re-initialised by the surrounding case-1 code on the very
//        next lines, so the SM cursor is unaffected.  ABSENCE CLAIM, with
//        command and time: `grep -rn "InitSht2DCodeComPort" BarCode/ .` run
//        2026-08-11 11:20:59 (and re-run at delivery) finds only gate-map
//        comments and per-unit stand-ins -- no port body anywhere.
//    (c) REAL MACHINE: the COM/socket channel is flushed and re-opened, so a
//        retry after a comms fault actually recovers.  Offline: no channel.
//
//  GATE 7 -- TfBarCode::Barcode_StartScan_In(int,int,AnsiString&,AnsiString)
//    (a) golden BarCode.cpp:2097 (~470 lines); call sites here golden :5439
//        :5452 :5470 :5483 (Do2DIDCheckSh1 case 1150, the 2 OCR + 2 non-OCR
//        row arms).
//    (b) FAITHFUL AT `false`: golden's own contract is "true == this column's
//        code is now in cDeviceInf, false == not yet".  `false` is exactly what
//        golden returns on every tick while the CCD has not answered, which is
//        the permanent state of a machine with no CCD attached.  The call site
//        already carries the full handling for that: BarcodeDelay[iSht].Off()
//        fires, both rows get asBarCodeErrorSend written into cDeviceInf, and
//        the operator's Step key advances to case 1200.  So the SM still walks
//        the whole column ladder and still terminates -- it just records every
//        column as a read failure.  A `true` default would have been the unsafe
//        choice (it would fabricate a successful decode of an empty string).
//        ABSENCE CLAIM, with command and time: `grep -rn "Barcode_StartScan_In"`
//        over BarCode/ + tree root, run 2026-08-11 11:18:39 and re-run at
//        delivery -- 8 hits, ALL gate-map comments or per-unit stand-ins
//        (BarSh1_/Gated_ prefixed); no real body.
//    (c) REAL MACHINE: real 2D codes land in FLCarryKit.cDeviceInf[row][col] and
//        the grid shows them; a genuine no-read still ends at asBarCodeErrorSend.
//
//  NOT A GATE -- SFCAT1_SetSFCCheckStepCount()
//    Faithful 2-line mirror of golden BarCode.cpp:3128
//    TfBarCode::SetSFCCheckStepCount() (called golden :4739 and :5027).  It is
//    a pure calculation that sets iSFCTotalMoveStep, the LOOP BOUND both cycles
//    depend on; gating it would silently break the sweep.  Same decision, same
//    reasoning and the same "distinct local name" shape the tree already uses
//    three times (BarSh1_SetSFCCheckStepCount, BarSh2_SetSFCCheckStepCount,
//    Glue8CCD_SetSFCCheckStepCount) -- a fourth local mirror is consistent with
//    precedent; consolidating all four onto one published symbol is a cleanup
//    for the main loop to schedule, not something this unit does unilaterally.
//
//  NOT GATED -- REAL DEPENDENCIES USED AS-IS
//    MOT[]/TMyMotor::MotorMove, TQPF_Timer (SetMSAndOn/Off), Prod, Offset,
//    TestIF_File, InArmSuck/FLCarryKit (TMyKitSuck via aHotPlateSubstrate.h --
//    see TRAP-5 note below), IndexSuckName[][], BAR_CODE_INSTALL/ebcUseOCR,
//    IsNNMode()/NN_1Row/NN_2Row, ShowMyMessage/ShowErrorMessage/K_RETRY/K_SKIP,
//    asBarCodeErrorSend, bBarcodeFirstAutoRetry[], iSH1_1/2BarcodePosition,
//    MInShuttle1, WaitManualRetryKey()/WaitManualStepKey() (ckernel.cpp:352/:226),
//    and -- see next block -- SendCCDCommand().
//
// -----------------------------------------------------------------------------
//  TRAP-2 EXPIRED-ABSENCE FINDING (a real one, not a formality)
// -----------------------------------------------------------------------------
//  The sibling BarCode_Shuttle1_Scan.cpp gates SendCCDCommand (its GATE MAP
//  line 38: "BarSh1_SendCCDCommand <- TfBarCode::SendCCDCommand
//  (BarCode.cpp:5305)") with a no-op.  That premise has EXPIRED: a REAL,
//  faithful port of that exact golden function now exists at
//      BarCode/BarCode_8CCD_Glue.cpp:178   `void SendCCDCommand(int index,
//                                            AnsiString Msg, AnsiString Msg2)`
//  declared BarCode_8CCD_Glue.h:216, external linkage, already listed in
//  CMakeLists.txt (ht9045_sm, right below the other BarCode units) and already
//  exercised by tests/test_barcode_8ccd_glue.cpp -- so that object is already
//  extracted from the archive today, and calling it adds no new link edge that
//  the test suite does not already carry.
//  Verified with `grep -n "^void \|^bool " BarCode/BarCode_8CCD_Glue.h` and by
//  READING the body, on 2026-08-11 11:20:48; re-verified in the final sweep.
//  THIS UNIT THEREFORE CALLS THE REAL ONE.  Consequences, stated openly:
//    * More faithful: golden's SendCCDCommand also flips bChangeSetupFile /
//      OCRChangeFileTimeOut on an "LF" payload and writes the trace line;
//      a no-op silently drops all of that.
//    * The port body's `if(InitialOK==false) return;` front guard means that
//      offline (InitialOK false) it is still effectively inert, so nothing in
//      this unit's SM behaviour is destabilised by the choice.
//    * It DIVERGES from the 2026-07-11 integrate decision recorded in
//      CMakeLists.txt ("the 6 pre-existing per-unit gated stand-ins are LEFT
//      SEPARATE, NOT retargeted").  That decision's three stated reasons were
//      (1) those files are outside the front's scope, (2) each already has a
//      passing oracle test asserting its own no-op behaviour, (3) consolidation
//      is a cleanup.  NONE of the three applies to a brand-new file with no
//      pre-existing test.  Flagged to the main loop regardless: if it prefers
//      uniformity over fidelity here, the one-line change is to add an
//      SFCAT1_SendCCDCommand no-op to the anonymous namespace below and
//      s/SendCCDCommand(/SFCAT1_SendCCDCommand(/ on the 12 call sites.
//
// -----------------------------------------------------------------------------
//  TRAP-2 FINAL SWEEP -- a SIBLING UNIT LANDED WHILE THIS ONE WAS BEING WRITTEN
// -----------------------------------------------------------------------------
//  Every absence claim above was re-run at 2026-08-11 11:35 (a full walk of all
//  1142 .h/.cpp/.c/.hpp files under the port root, excluding build*/ .git/
//  third_party/).  It caught a real change made AFTER this unit started:
//      BarCode/BarCode_Shuttle2_SFCAutoTune.h    written 2026-08-11 11:27
//      BarCode/BarCode_Shuttle2_SFCAutoTune.cpp  written 2026-08-11 11:32
//  i.e. the Shuttle-2 twin of this very unit (BarCode_Sh2_InitialSFCAutoTune2 /
//  _DoSFCAutoTune_2 / _Do2DIDCheckSh2) is a concurrent sibling.  Checked for
//  collisions against it EXPLICITLY rather than assuming:
//    * It owns `iSFCAutoTune2Task` (golden BarCode.h:885) and its header states
//      in as many words that `iSFCAutoTune1Task` (golden :884) "is NOT touched
//      here -- it belongs to the concurrently-running Sh1 counterpart unit".
//      So the two ownership claims are complementary, not overlapping.  NO ODR
//      CONFLICT on either cursor.
//    * `bSFCError` -- 0 hits anywhere in the tree outside this file.  Confirmed
//      from GOLDEN why: BarCode_Sh2.cpp contains ZERO occurrences of bSFCError
//      (golden's Sh2 twin simply has no exposure-error arms), so despite
//      BarCode.h:907 declaring it BAR_CODE_COUNT-wide across all four CCD rows,
//      only the Shuttle-1 path ever touches it.  THIS unit is therefore the sole
//      and canonical owner; any future TU (including a revised Sh2 unit or the
//      port of BarCode.cpp:3602's socket Read handler) must reuse this extern.
//    * The Sh2 unit independently reached the SAME conclusion about
//      SendCCDCommand (its GATE REGISTER entry G-0 also declines to gate it and
//      calls the real BarCode_8CCD_Glue.cpp:178 body).  Two units converging on
//      that from opposite ends is corroboration, not coordination.
//    * Both units' free-function names are disjoint (BarCode_Sh2_* there vs. the
//      plain golden names here), so no link-time clash exists in either
//      direction.
//
// -----------------------------------------------------------------------------
//  TRAP-5 NOTE -- which header, which definition
// -----------------------------------------------------------------------------
//  This unit reaches InArmSuck/FLCarryKit (TMyKitSuck) ONLY through
//  BarCode_Shuttle1_Scan.h -> aHotPlateSubstrate.h, i.e. the aHotPlateSubstrate.h
//  :365 TMyKitSuck layout, NOT the incompatible mykitsuck.h:274 one; the objects
//  are defined in aHotPlateSubstrate.cpp, the same TU family the sibling
//  BarCode units already link against.  It does NOT include mykitsuck.h, so the
//  documented two-headers-one-class-name hazard cannot arise in this TU.
//  Likewise SendCCDCommand's declaration comes from the very header whose .cpp
//  defines it (BarCode_8CCD_Glue.h/.cpp), by construction rather than by a
//  hand-written forward declaration that could drift.
//
// -----------------------------------------------------------------------------
//  TRAP-4 NOTE -- static initialisation
// -----------------------------------------------------------------------------
//  This TU's only namespace-scope objects are `int iSFCAutoTune1Task` and
//  `bool bSFCError[BAR_CODE_COUNT]` -- both PODs with constant initialisers, so
//  they are constant-initialised and run NO code before main().  Nothing in this
//  file has a file-scope constructor, and no function here is called from any
//  constructor.  The `static AnsiString` locals inside the two state machines
//  are function-local, so they are constructed on first call (never before
//  main()).  None of the 18 NULL globals of docs/PT_CAMPAIGN_PLAN.md section 8
//  is touched at static-init time by this unit.
//
// -----------------------------------------------------------------------------
//  GOLDEN BUGS PRESERVED VERBATIM (found, NOT fixed -- flagged here and in the
//  deliverable report)
// -----------------------------------------------------------------------------
//  BUG NOTE 1 (golden :5011-5012, case 1400): the no-device cycle's terminal
//    case sets `Task=1300;` and `bResult=true;`.  1300 moves the shuttle to
//    Prod.OutSHT[0].iLeft and goes straight back to 1400, so the cursor
//    ping-pongs 1300<->1400 forever, re-reporting "Cycle no device end" and
//    re-returning true on every second tick.  The has-device twin (case 11400,
//    golden :5279-5280) does NOT reassign Task.  Preserved exactly.
//  BUG NOTE 2 (golden :5267, case 11200): when the has-device sweep finishes it
//    sets `Task=13000;` -- and there is NO `case 13000` anywhere in the switch.
//    The cursor parks on an unhandled value and DoSFCAutoTune_1 returns false
//    forever, so the has-device cycle NEVER reports completion.
//  BUG NOTE 3 (consequence of BUG NOTE 2, golden :5272-5280): `case 11300` and
//    `case 11400` are therefore DEAD -- nothing in the function can reach them,
//    so the has-device cycle also never moves the shuttle back to
//    Prod.OutSHT[0].iLeft.  (11200's completion arm was evidently meant to say
//    11300, mirroring 1200->1300.)  Preserved.
//  BUG NOTE 4 (golden :4706 and :5018): the `bHasIC` PARAMETER of
//    DoSFCAutoTune_1 is never read -- the no-device/has-device choice is made
//    solely by InitialSFCAutoTune1's cursor seed.  Preserved (signature is the
//    golden one; the golden call sites pass it).
//  BUG NOTE 5 (golden :5468, Do2DIDCheckSh1 case 1150 non-OCR row-B arm):
//    `mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1,
//     FLCarryKit.cDeviceInf[1][iNowCheckStep]);` writes ROW A's cell with ROW
//    B's decoded string -- every one of the other three sibling arms writes its
//    own row.  Display-only, so behaviourally inert, but it is a real defect.
//    Preserved verbatim.
//  BUG NOTE 6 (golden :4711 `static AnsiString sErrorPart="";`, :4710
//    `static bool bHasError=false;`, :4708 `static int iRetryCount=0;`, and in
//    Do2DIDCheckSh1 :5292 `static int iRetryCount`, :5295 `AnsiString DataStr,
//    Path, Data;`, :5298 `static AnsiString sErrorPart="", sCheckErrorPart="";`,
//    :5299 `AnsiString asString;`, :4722 `AnsiString cLastString` in
//    DoSFCAutoTune_1): all written-but-never-read or never-touched leftovers.
//    Kept, because deleting them would change nothing but would make the next
//    diff against golden lie.
//  BUG NOTE 7 (golden :4718 `int pos=0, ret;`): `ret` is left uninitialised at
//    declaration; it is assigned before its only reads (cases 1160 / 11160).
//    Kept exactly as golden spells it.
//  DELIBERATE, NOT A BUG (golden :5038-5040): `case 10000` has NO `break` and
//    falls through into `case 11000` -- that is how the has-device cycle starts
//    moving on the same tick it initialises.  Golden's `case 1` twin instead
//    ends at Task=500 with a break.  Fall-through preserved.
//  DELIBERATE, NOT A BUG (golden :5519-5522): Do2DIDCheckSh1's `case 1200`
//    breaks ONLY on the "more columns left" arm and otherwise falls through into
//    `case 1300`.  Preserved.
//  ALIASED CURSOR, NOT A BUG (golden :5289): Do2DIDCheckSh1 drives
//    `iInitialBarcodeInShuttle1Task` -- the SAME cursor as
//    DoBarcodeTriggerInShuttle_1 / DoBarcodeScanInShuttle_1 /
//    DoBarcodeCCDInShuttle_1.  Golden's own design (these are mutually
//    exclusive entry points); BarCode_Shuttle1_Scan.h already documents it.
//
//  Big5: golden read via cp950; Chinese comments transcribed character for
//  character.  Zero U+FFFD verified in this file.
// =============================================================================
#include "BarCode_Shuttle1_SFCAutoTune.h"
#include <cstdio>

// ============================================================================
//  Definitions of the golden TfBarCode instance data this unit OWNS.
//  (Nothing else in the tree declares or defines either -- see header banner.)
// ============================================================================
// AI(pt-wave) 20260811 PT-W8 integrate: seed corrected 1 -> 0.  Golden
// BarCode.h:884 declares `int iSFCAutoTune1Task;` as a bare member of
// `class TfBarCode : public TForm`, with NO initialiser and NO ctor assignment
// anywhere in the golden tree -- its only writes are inside InitialSFCAutoTune1
// (golden BarCode_Sh1.cpp:4701 -> 1, :4703 -> 10000).  VCL zero-fills the
// instance, so golden's power-on value is 0, and 0 is not a handled case in
// DoSFCAutoTune_1's switch: golden idles, doing nothing, until something calls
// InitialSFCAutoTune1 to arm it.
// Seeding 1 armed the cycle at static-init time instead: the first tick would
// run golden's `case 1`, fire the E9 clear-buffer at both CCD rows, reset the
// step/error/exposure/result arrays and arm SFCStartDelay[0] for 5000 ms, whose
// expiry raises WAR0462 (golden :4766).  Golden does none of that at power-on.
// The Sh2 twin written in this same wave got it right and states the same
// reasoning (BarCode_Shuttle2_SFCAutoTune.cpp:148-152).
int  iSFCAutoTune1Task = 0;                     // golden BarCode.h:884 -- see note above
bool bSFCError[BAR_CODE_COUNT] = {false};       // golden BarCode.h:907

// ============================================================================
//  Local gate helpers -- see GATE REGISTER in the file banner.  ANONYMOUS
//  namespace: internal linkage, so they can neither collide with nor stand in
//  for any other TU's symbol.
// ============================================================================
namespace {

// -- GATE 1: mtSFC_Shuttle (TTMyTray*, golden BarCode.h:200) ------------------
void SFCAT1_SFCGridSetCell(int /*index*/, int /*col*/, const AnsiString& /*text*/) {}

// -- GATE 2: mtBarcodeInSh (TTMyTray*, golden BarCode.h:145) ------------------
void SFCAT1_BarcodeInShGridSetCell(int /*index*/, int /*col*/, const AnsiString& /*text*/) {}

// -- GATE 3/4: fNote->t2DCode (TfNote's 2D grid; forms/fNote.h deliberately
//    does not carry t2DCode -- see its banner section (4)).
void SFCAT1_Note2DSetCell(int /*row*/, int /*col*/, const AnsiString& /*text*/) {}
void SFCAT1_Note2DSetCellColorIndex(int /*row*/, int /*col*/, int /*colorIndex*/) {}

// -- GATE 5: TfBarCode::AddCCDCommunicationLog(int,AnsiString) --------------
//    golden BarCode.cpp:5903.  Sim: stdout, matching the sibling's precedent.
void SFCAT1_AddCCDCommunicationLog(int index, const AnsiString& s)
{
    std::printf("[BarCodeSh1 SFC-AT CCD#%d] %s\n", index, s.c_str());
}

// -- GATE 6: TfBarCode::InitSht2DCodeComPort(int) -- golden BarCode.cpp:2512.
void SFCAT1_InitSht2DCodeComPort(int /*iSht*/) {}

// -- GATE 7: TfBarCode::Barcode_StartScan_In(int,int,AnsiString&,AnsiString) --
//    golden BarCode.cpp:2097.  Offline: "no reply yet" == false.
bool SFCAT1_Barcode_StartScan_In(int /*BarCodeIndex*/, int /*iXpos*/,
                                 AnsiString& /*cDeviceInf*/,
                                 const AnsiString& /*cLastDeviceInf*/)
{
    return false;
}

// -- NOT A GATE: faithful mirror of golden BarCode.cpp:3128
//    TfBarCode::SetSFCCheckStepCount() (pure calc; sets the sweep's loop bound).
void SFCAT1_SetSFCCheckStepCount()
{
    if(TestIF_File.bSFCUse2Photo)
        iSFCTotalMoveStep = InArmSuck.iShtCol * 2;
    else
        iSFCTotalMoveStep = InArmSuck.iShtCol;
}

} // anonymous namespace

// ============================================================================
//  InitialSFCAutoTune1  --  golden BarCode_Sh1.cpp:4698-4705
// ============================================================================
void InitialSFCAutoTune1(bool bHasIC)
{
    if(bHasIC==false)
        iSFCAutoTune1Task=1;
    else
        iSFCAutoTune1Task=10000;
}

// ============================================================================
//  DoSFCAutoTune_1  --  golden BarCode_Sh1.cpp:4706-5285
//  (`bHasIC` is dead in golden -- BUG NOTE 4 in the file banner.)
// ============================================================================
bool DoSFCAutoTune_1(bool bHasIC)
{
    static int iNowMoveStep=0;
    static int iRetryCount=0;
    static int iRetryCountExpTimo=0;
    static bool bHasError=false;
    static AnsiString ErrPart;
    static AnsiString sErrorPart="";

    int &Task=iSFCAutoTune1Task;
    int pos=0, ret;                                                             //golden :4718 -- `ret` uninitialised here, see BUG NOTE 7
    int iSht=0;
    int MSht=MInShuttle1;
    int iSFCRowA=iBarCode1_1;
    int iSFCRowB=iBarCode1_2;
    bool bResult=false;
    AnsiString cLastString, sCCDCommand;
    AnsiString Log;

    // NOTE (fidelity): golden never reads `bHasIC` (BUG NOTE 4), nor
    // iRetryCount / bHasError / sErrorPart / cLastString.  They are kept
    // declared exactly as golden declares them and NOT silenced with `(void)`
    // casts -- no sibling BarCode unit does that, and adding statements golden
    // does not have would make the next golden diff lie.  -Wunused* only.

    switch(Task)
    {
        //---------------------
        //AT無料的狀況
        //---------------------
        case 1:                                                                 // golden :4730
            SFCAT1_AddCCDCommunicationLog(iSFCRowA, "<<SFC AT Cycle, Cycle no device start.------------------");

            SFCAT1_SetSFCCheckStepCount();
            iNowMoveStep=iSFCTotalMoveStep-1;
            iSFCCurrentStep[iSFCRowA]=iNowMoveStep;
            iSFCCurrentStep[iSFCRowB]=iNowMoveStep;
            bSFCStepError[iSFCRowA]=false;
            bSFCStepError[iSFCRowB]=false;
            bSFCExposureOK[iSFCRowA]=true;
            bSFCExposureOK[iSFCRowB]=true;
            iRetryCountExpTimo=0;

            for(int i=0; i<16; i++)
            {
                iSFCCheckResult[iSFCRowA][i]=-1;
                iSFCCheckResult[iSFCRowB][i]=-1;
            }

            bHasError=false;
            iRetryCount=0;
            sCCDCommand.sprintf("E9,1,%d", iSFCTotalMoveStep);                  //清空暫存器
            bSFCGetSE9[iSFCRowA]=false;
            bSFCGetSE9[iSFCRowB]=false;
            SendCCDCommand(iSFCRowA, "SFC AT Clear buffer 1", sCCDCommand);     //Reset CCD Shuttle暫存器
            SendCCDCommand(iSFCRowB, "SFC AT Clear buffer 1", sCCDCommand);
            SFCStartDelay[iSht].SetMSAndOn(5000);
            Task=500;
            break;
        case 500:                                                               //確認Buffer清空        // golden :4759
            if(bSFCGetSE9[iSFCRowA] && bSFCGetSE9[iSFCRowB])
            {
                Task=1000;
            }
            else if(SFCStartDelay[iSht].Off())
            {
                ShowErrorMessage("WAR0462", K_RETRY, MSht, false, "SFC AT Clear buffer");
                Task=1;
            }
            break;
        case 1000:                                                              // golden :4770
            if(InArmSuck.iShtCol==1)
            {
                pos=Prod.iInSFCPos1x1[iSht];
            }
            else if(InArmSuck.iShtCol==2)
            {
                pos=Prod.iInSFCPos1x2[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==3)                                       //ChungHung 20140115 add for 2x3_6
            {
                pos=Prod.iInSFCPos1x3[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==4)
            {
                pos=Prod.iInSFCPos1x4[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==5)                                       //Steven 20221107 : Add for 2x5
            {
                pos=Prod.iInSFCPos1x5[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==6)
            {
                pos=Prod.iInSFCPos1x6[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==8)
            {
                pos=Prod.iInSFCPos1x8[iSht][iNowMoveStep];
            }
            else
            {
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoSFCAutoTune_1");
            }

            if(TestIF_File.bSFCUse2Photo)
            {
                if(iNowMoveStep%2==0)
                    pos+=TestIF_File.iSFCUse2PhotoOffset*100;
                else
                    pos-=TestIF_File.iSFCUse2PhotoOffset*100;
            }

            pos+=Offset.iSHLeft2D[iSht];

            if(MOT[MSht].MotorMove(pos))
            {
                Log.sprintf("SFC AT Action, Shuttle %d move to step %d", iSht+1, iNowMoveStep);
                SFCAT1_AddCCDCommunicationLog(iSFCRowA, Log);
                SFCStartDelay[iSht].SetMSAndOn(TestIF_File.iSFCStartDelay);
                Task=1120;
            }
            break;
        case 1120:                                                              // golden :4822
            if(SFCStartDelay[iSht].Off())                                       //到位Delay
            {
                bSFCExposureOK[iSFCRowA]=false;
                if(InArmSuck.iShtRow==2)
                    bSFCExposureOK[iSFCRowB]=false;
                else
                    bSFCExposureOK[iSFCRowB]=true;

                bSFCError[iSFCRowA]=false;
                bSFCError[iSFCRowB]=false;

                //--------------------------------------
                //SE   1,        X,    1
                //拍照 偵測置偏, 位置, 左中右
                //--------------------------------------
                if(TestIF_File.bSFCUse2Photo)
                {
                    if(iNowMoveStep%2==0)
                        sCCDCommand.sprintf("E2,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E2,1,%d,2", iNowMoveStep);
                }
                else
                    sCCDCommand.sprintf("E2,1,%d,1", iNowMoveStep);

                SFCAT1_SFCGridSetCell(iSFCRowA, iNowMoveStep+1, "SFC Exposure...");   // golden mtSFC_Shuttle->SetCellNumber
                SendCCDCommand(iSFCRowA, "SFC AT exposure 1120: ", sCCDCommand);

                if(InArmSuck.iShtRow==2)
                {
                    SFCAT1_SFCGridSetCell(iSFCRowB, iNowMoveStep+1, "SFC Exposure...");
                    SendCCDCommand(iSFCRowB, "SFC AT exposure 1120: ", sCCDCommand);
                }

                Task=1150;
                iRetryCountExpTimo=0;                                           //Steven 20160425 : 修改拍照Time Out
                SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            }
            break;
        case 1150:                                                              // golden :4862
            if(bSFCStepError[iSFCRowA]==true || bSFCStepError[iSFCRowB]==true)  //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                ErrPart="";
                if(IsNNMode()==NN_2Row)                                         //Steven 20220315 : for 2did nn mode alarm
                {
                    if(bSFCStepError[iSFCRowA])
                        ErrPart+=IndexSuckName[2][iNowMoveStep];

                    if(bSFCStepError[iSFCRowB])
                        ErrPart+=IndexSuckName[3][iNowMoveStep];
                }
                else if(IsNNMode()==NN_1Row)
                {
                    if(bSFCStepError[iSFCRowA])
                        ErrPart+=IndexSuckName[1][iNowMoveStep];

                    if(bSFCStepError[iSFCRowB])
                        ErrPart+=IndexSuckName[1][iNowMoveStep];
                }
                else
                {
                    if(bSFCStepError[iSFCRowA])
                        ErrPart+=IndexSuckName[0][iNowMoveStep];

                    if(bSFCStepError[iSFCRowB])
                        ErrPart+=IndexSuckName[1][iNowMoveStep];
                }

                Task=1180;
            }
            else if(bSFCExposureOK[iSFCRowA]==true &&
                    bSFCExposureOK[iSFCRowB]==true)                             //拍照完成
            {
                SFCAT1_SFCGridSetCell(iSFCRowA, iNowMoveStep+1, "Waiting result..");

                if(InArmSuck.iShtRow==2)
                {
                    SFCAT1_SFCGridSetCell(iSFCRowB, iNowMoveStep+1, "Waiting result..");
                }
                Task=1200;
            }
            else if(SFCExposureDelay[iSht].Off() ||                             //拍照time out
                    bSFCError[iSFCRowA] || bSFCError[iSFCRowB])
            {
                if(bSFCExposureOK[iSFCRowA]==false || bSFCError[iSFCRowA])
                {
                    SFCAT1_SFCGridSetCell(iSFCRowA, iNowMoveStep+1, "Exposure time out");
                    Log.sprintf("SFC AT Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    SFCAT1_AddCCDCommunicationLog(iSFCRowA, Log);
                }
                else
                {
                    SFCAT1_SFCGridSetCell(iSFCRowA, iNowMoveStep+1, "Waiting result..");
                }

                if(InArmSuck.iShtRow==2)
                {
                    if(bSFCExposureOK[iSFCRowB]==false || bSFCError[iSFCRowB])
                    {
                        SFCAT1_SFCGridSetCell(iSFCRowB, iNowMoveStep+1, "Exposure time out");
                        Log.sprintf("SFC AT Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        SFCAT1_AddCCDCommunicationLog(iSFCRowB, Log);
                    }
                    else
                    {
                        SFCAT1_SFCGridSetCell(iSFCRowB, iNowMoveStep+1, "Waiting result..");
                    }
                }
                iRetryCountExpTimo++;                                           //Steven 20160425 : 修改拍照Time Out
                Task=1160;
            }
            break;
        case 1160:                                                              // golden :4935
            if(iRetryCountExpTimo<3)
            {
                Task=1170;
            }
            else
            {
                ret=ShowErrorMessage("WAR0464", K_RETRY|K_SKIP, MSht);          //2D function CCD 1 or 2 Exposure time out!
                if(ret==K_SKIP)
                {
                    Task=1200;
                }
                else
                {
                    Task=1170;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 1170:                                                              // golden :4954
            if(TestIF_File.bSFCUse2Photo)
            {
                if(iNowMoveStep%2==0)
                    sCCDCommand.sprintf("E2,1,%d,0", iNowMoveStep);
                else
                    sCCDCommand.sprintf("E2,1,%d,2", iNowMoveStep);
            }
            else
            {
                sCCDCommand.sprintf("E2,1,%d,1", iNowMoveStep);
            }

            if(bSFCExposureOK[iSFCRowA]==false || bSFCError[iSFCRowA]==true)
            {
                bSFCError[iSFCRowA]=false;
                SendCCDCommand(iSFCRowA, "SFC AT exposure 1170: ", sCCDCommand);
            }

            if(InArmSuck.iShtRow==2)
            {
                if(bSFCExposureOK[iSFCRowB]==false || bSFCError[iSFCRowB]==true)
                {
                    bSFCError[iSFCRowB]=false;
                    SendCCDCommand(iSFCRowB, "SFC AT exposure 1170: ", sCCDCommand);
                }
            }
            Task=1150;
            SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            break;
        case 1180:                                                              // golden :4984
            Log.sprintf("SFC AT Alarm, In shuttle %d exposure position error!", iSht+1);
            SFCAT1_AddCCDCommunicationLog(iSFCRowA, Log);
            ShowErrorMessage("WAR0469", K_RETRY, MSht, false, ErrPart);         //In shuttle 1 exposure position error!
            Task=1;
            break;
        case 1200:                                                              // golden :4990
            iNowMoveStep--;
            iSFCCurrentStep[iSFCRowA]=iNowMoveStep;                             //Steven 20160503 : 確認移動位置與拍照位置相同
            iSFCCurrentStep[iSFCRowB]=iNowMoveStep;

            if(iNowMoveStep>=0 && iNowMoveStep<iSFCTotalMoveStep)
            {
                Task=1000;
            }
            else
            {
                Task=1300;
            }
            break;
        case 1300:                                                              // golden :5004
            if(MOT[MSht].MotorMove(Prod.OutSHT[0].iLeft))
            {
                Task=1400;
            }
            break;
        case 1400:                                                              // golden :5010 -- see BUG NOTE 1 (Task=1300 makes 1300<->1400 loop forever)
            SFCAT1_AddCCDCommunicationLog(iSFCRowA, "<<SFC AT Cycle, Cycle no device end.------------------");
            Task=1300;
            bResult=true;
            break;
        //---------------------
        //AT有料的狀況
        //---------------------
        case 10000:                                                             // golden :5018 -- NO break: deliberate fall-through into case 11000
            SFCAT1_AddCCDCommunicationLog(iSFCRowA, "<<SFC AT Cycle, Cycle has device start.------------------");

            SFCAT1_SetSFCCheckStepCount();
            iNowMoveStep=iSFCTotalMoveStep-1;
            iSFCCurrentStep[iSFCRowA]=iNowMoveStep;
            iSFCCurrentStep[iSFCRowB]=iNowMoveStep;
            bSFCStepError[iSFCRowA]=false;
            bSFCStepError[iSFCRowB]=false;
            bSFCExposureOK[iSFCRowA]=true;
            bSFCExposureOK[iSFCRowB]=true;
            iRetryCountExpTimo=0;

            for(int i=0; i<16; i++)
            {
                iSFCCheckResult[iSFCRowA][i]=-1;
                iSFCCheckResult[iSFCRowB][i]=-1;
            }

            bHasError=false;
            iRetryCount=0;
            Task=11000;
            // fall through -- golden has no `break` here
        case 11000:                                                             // golden :5040
            if(InArmSuck.iShtCol==1)
            {
                pos=Prod.iInSFCPos1x1[iSht];
            }
            else if(InArmSuck.iShtCol==2)
            {
                pos=Prod.iInSFCPos1x2[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==3)                                       //ChungHung 20140115 add for 2x3_6
            {
                pos=Prod.iInSFCPos1x3[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==4)
            {
                pos=Prod.iInSFCPos1x4[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==5)                                       //Steven 20221107 : Add for 2x5
            {
                pos=Prod.iInSFCPos1x5[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==6)
            {
                pos=Prod.iInSFCPos1x6[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==8)
            {
                pos=Prod.iInSFCPos1x8[iSht][iNowMoveStep];
            }
            else
            {
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoSFCAutoTune_1");
            }

            if(TestIF_File.bSFCUse2Photo)
            {
                if(iNowMoveStep%2==0)
                    pos+=TestIF_File.iSFCUse2PhotoOffset*100;
                else
                    pos-=TestIF_File.iSFCUse2PhotoOffset*100;
            }

            pos+=Offset.iSHLeft2D[iSht];

            if(MOT[MSht].MotorMove(pos))
            {
                Log.sprintf("SFC AT Action, Shuttle %d move to step %d", iSht+1, iNowMoveStep);
                SFCAT1_AddCCDCommunicationLog(iSFCRowA, Log);
                SFCStartDelay[iSht].SetMSAndOn(TestIF_File.iSFCStartDelay);
                Task=11120;
            }
            break;
        case 11120:                                                             // golden :5092
            if(SFCStartDelay[iSht].Off())                                       //到位Delay
            {
                bSFCExposureOK[iSFCRowA]=false;
                if(InArmSuck.iShtRow==2)
                    bSFCExposureOK[iSFCRowB]=false;
                else
                    bSFCExposureOK[iSFCRowB]=true;

                bSFCError[iSFCRowA]=false;
                bSFCError[iSFCRowB]=false;

                //--------------------------------------
                //SE   1,        X,    1
                //拍照 偵測置偏, 位置, 左中右
                //--------------------------------------
                if(TestIF_File.bSFCUse2Photo)
                {
                    if(iNowMoveStep%2==0)
                        sCCDCommand.sprintf("E3,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E3,1,%d,2", iNowMoveStep);
                }
                else
                    sCCDCommand.sprintf("E3,1,%d,1", iNowMoveStep);

                SFCAT1_SFCGridSetCell(iSFCRowA, iNowMoveStep+1, "SFC AT Exposure...");
                SendCCDCommand(iSFCRowA, "SFC AT exposure 1120: ", sCCDCommand);

                if(InArmSuck.iShtRow==2)
                {
                    SFCAT1_SFCGridSetCell(iSFCRowB, iNowMoveStep+1, "SFC AT Exposure...");
                    SendCCDCommand(iSFCRowB, "SFC AT exposure 1120: ", sCCDCommand);
                }

                Task=11150;
                iRetryCountExpTimo=0;                                           //Steven 20160425 : 修改拍照Time Out
                SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            }
            break;
        case 11150:                                                             // golden :5132
            if(bSFCStepError[iSFCRowA]==true || bSFCStepError[iSFCRowB]==true)  //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                ErrPart="";
                if(IsNNMode()==NN_2Row)                                         //Steven 20220315 : for 2did nn mode alarm
                {
                    if(bSFCStepError[iSFCRowA])
                        ErrPart+=IndexSuckName[2][iNowMoveStep];

                    if(bSFCStepError[iSFCRowB])
                        ErrPart+=IndexSuckName[3][iNowMoveStep];
                }
                else if(IsNNMode()==NN_1Row)
                {
                    if(bSFCStepError[iSFCRowA])
                        ErrPart+=IndexSuckName[1][iNowMoveStep];

                    if(bSFCStepError[iSFCRowB])
                        ErrPart+=IndexSuckName[1][iNowMoveStep];
                }
                else
                {
                    if(bSFCStepError[iSFCRowA])
                        ErrPart+=IndexSuckName[0][iNowMoveStep];

                    if(bSFCStepError[iSFCRowB])
                        ErrPart+=IndexSuckName[1][iNowMoveStep];
                }

                Task=11180;
            }
            else if(bSFCExposureOK[iSFCRowA]==true &&
                    bSFCExposureOK[iSFCRowB]==true)                             //拍照完成
            {
                SFCAT1_SFCGridSetCell(iSFCRowA, iNowMoveStep+1, "Waiting result..");

                if(InArmSuck.iShtRow==2)
                {
                    SFCAT1_SFCGridSetCell(iSFCRowB, iNowMoveStep+1, "Waiting result..");
                }
                Task=11200;
            }
            else if(SFCExposureDelay[iSht].Off() ||
                    bSFCError[iSFCRowA] || bSFCError[iSFCRowB])                 //拍照time out
            {
                if(bSFCExposureOK[iSFCRowA]==false || bSFCError[iSFCRowA])
                {
                    SFCAT1_SFCGridSetCell(iSFCRowA, iNowMoveStep+1, "Exposure time out");
                    Log.sprintf("SFC AT Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    SFCAT1_AddCCDCommunicationLog(iSFCRowA, Log);
                }
                else
                {
                    SFCAT1_SFCGridSetCell(iSFCRowA, iNowMoveStep+1, "Waiting result..");
                }

                if(InArmSuck.iShtRow==2)
                {
                    if(bSFCExposureOK[iSFCRowB]==false || bSFCError[iSFCRowB])
                    {
                        SFCAT1_SFCGridSetCell(iSFCRowB, iNowMoveStep+1, "Exposure time out");
                        Log.sprintf("SFC AT Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        SFCAT1_AddCCDCommunicationLog(iSFCRowB, Log);
                    }
                    else
                    {
                        SFCAT1_SFCGridSetCell(iSFCRowB, iNowMoveStep+1, "Waiting result..");
                    }
                }
                iRetryCountExpTimo++;                                           //Steven 20160425 : 修改拍照Time Out
                Task=11160;
            }
            break;
        case 11160:                                                             // golden :5205
            if(iRetryCountExpTimo<3)
            {
                Task=11170;
            }
            else
            {
                ret=ShowErrorMessage("WAR0464", K_RETRY|K_SKIP, MSht);          //2D function CCD 1 or 2 Exposure time out!
                if(ret==K_SKIP)
                {
                    Task=11200;
                }
                else
                {
                    Task=11170;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 11170:                                                             // golden :5224
            if(TestIF_File.bSFCUse2Photo)
            {
                if(iNowMoveStep%2==0)
                    sCCDCommand.sprintf("E3,1,%d,0", iNowMoveStep);
                else
                    sCCDCommand.sprintf("E3,1,%d,2", iNowMoveStep);
            }
            else
                sCCDCommand.sprintf("E3,1,%d,1", iNowMoveStep);

            if(bSFCExposureOK[iSFCRowA]==false || bSFCError[iSFCRowA]==true)
            {
                bSFCError[iSFCRowA]=false;
                SendCCDCommand(iSFCRowA, "SFC AT exposure 1170: ", sCCDCommand);
            }

            if(InArmSuck.iShtRow==2)
            {
                if(bSFCExposureOK[iSFCRowB]==false || bSFCError[iSFCRowB]==true)
                {
                    bSFCError[iSFCRowB]=false;
                    SendCCDCommand(iSFCRowB, "SFC AT exposure 1170: ", sCCDCommand);
                }
            }
            Task=11150;
            SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            break;
        case 11180:                                                             // golden :5252
            Log.sprintf("SFC AT Alarm, In shuttle %d exposure position error!", iSht+1);
            SFCAT1_AddCCDCommunicationLog(iSFCRowA, Log);
            ShowErrorMessage("WAR0469", K_RETRY, MSht, false, ErrPart);         //In shuttle 1 exposure position error!
            Task=10000;
            break;
        case 11200:                                                             // golden :5258 -- Task=13000 has NO case: see BUG NOTE 2/3
            iNowMoveStep--;
            iSFCCurrentStep[iSFCRowA]=iNowMoveStep;                             //Steven 20160503 : 確認移動位置與拍照位置相同
            iSFCCurrentStep[iSFCRowB]=iNowMoveStep;

            if(iNowMoveStep>=0 && iNowMoveStep<iSFCTotalMoveStep)
            {
                Task=11000;
            }
            else
            {
                Task=13000;
            }
            break;
        case 11300:                                                             // golden :5272 -- unreachable in golden (BUG NOTE 3)
            if(MOT[MSht].MotorMove(Prod.OutSHT[0].iLeft))
            {
                Task=11400;
            }
            break;
        case 11400:                                                             // golden :5278 -- unreachable in golden (BUG NOTE 3)
            SFCAT1_AddCCDCommunicationLog(iSFCRowA, "<<SFC AT Cycle, Cycle has device end.------------------");
            bResult=true;
            break;
    }

    return bResult;
}

// ============================================================================
//  Do2DIDCheckSh1  --  golden BarCode_Sh1.cpp:5287-5533
// ============================================================================
bool Do2DIDCheckSh1()
{
    int &Task=iInitialBarcodeInShuttle1Task;
    int iSht=0;
    int pos=0;
    static int iNowCheckStep=0;
    static int iRetryCount=0;
    static bool bBarcodeNum[BAR_CODE_COUNT]={false};
    AnsiString DataStr, Path, Data;
    AnsiString cLastString;
    static AnsiString sErrorPart="", sCheckErrorPart="";
    AnsiString asString;
    int iBarCodeRowA=iBarCode1_1;
    int iBarCodeRowB=iBarCode1_2;

    // NOTE (fidelity): DataStr/Path/Data/asString are declared and never used
    // by golden, and sCheckErrorPart is written once and never read (BUG NOTE
    // 6).  Kept verbatim, NOT silenced -- see the same note in DoSFCAutoTune_1.

    if(InArmSuck.iShtRow==1 &&                                                  //Steven 20240227 : Shuttle使用一排的
       BAR_CODE_INSTALL!=ebcUseOCR)                                             //Ifor 20220322 add 自制OCR不使用
    {
        if(TestIF_File.iSelectUseCCDSh1==0)
        {
            iBarCodeRowA=iBarCode1_1;
            iBarCodeRowB=iBarCode1_2;
        }
        else
        {
            iBarCodeRowA=iBarCode1_2;
            iBarCodeRowB=iBarCode1_1;
        }
    }

    switch(Task)
    {
        case 1:                                                                 // golden :5319
            iNowCheckStep=InArmSuck.iShtCol-1;                                   //遞減方式檢查
            SFCAT1_InitSht2DCodeComPort(iSht);        //Ifor 20160322 Retry 時重置
            bBarcodeStartDelay[iBarCodeRowA]=true;
            if(InArmSuck.iShtRow==2)
            {
                bBarcodeStartDelay[iBarCodeRowB]=true;
            }

            for(int i=0; i<8; i++)
            {
                SFCAT1_BarcodeInShGridSetCell(iBarCodeRowA, i+1, "");
                SFCAT1_BarcodeInShGridSetCell(iBarCodeRowB, i+1, "");
            }

            for(int i=0; i<InArmSuck.iShtRow; i++)
            {
                for(int j=0; j<InArmSuck.iShtCol; j++)
                {
                    SFCAT1_Note2DSetCell(i, j, "");
                    SFCAT1_Note2DSetCellColorIndex(i, j, 0);
                }
            }
            bBarcodeNum[iBarCodeRowA]=false;
            bBarcodeNum[iBarCodeRowB]=false;
            Task=1000;
            break;
        case 1000:                                                              // golden :5346
            if(iNowCheckStep>=0 && iNowCheckStep<InArmSuck.iShtCol)
            {
                if(InArmSuck.iShtCol==1)
                {
                    pos=Prod.iInSHBarCodeDetectPos1x1[0];
                }
                else if(InArmSuck.iShtCol==2)
                {
                    pos=Prod.iInSHBarCodeDetectPos1x2[0][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==3)  //ChungHung 20140115 add for 2x3_6
                {
                    pos=Prod.iInSHBarCodeDetectPos2x3[0][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==4)
                {
                    pos=Prod.iInSHBarCodeDetectPos1x4[0][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==5)  //Steven 20221027 : Add for 2x5
                {
                    pos=Prod.iInSHBarCodeDetectPos2x5[0][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==6)  //Steven 20170111 : fixed for 2x6
                {
                    pos=Prod.iInSHBarCodeDetectPos2x6[0][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==8)
                {
                    pos=Prod.iInSHBarCodeDetectPos2x8[0][iNowCheckStep];
                }
                else
                {
                    ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "Do2DIDCheckSh1");
                }

                pos+=Offset.iSHLeft2D[0];       //Steven 20151218 : Offset for 2d reader

                if(MOT[MInShuttle1].MotorMove(pos))
                {
                    bBarcodeStartDelay[iBarCodeRowA]=true;
                    if(InArmSuck.iShtRow==2)
                    {
                        bBarcodeStartDelay[iBarCodeRowB]=true;
                    }
                    BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePosDelay);

                    if(WaitManualRetryKey())
                    {
                        Task=1000;
                    }
                    else if(WaitManualStepKey())
                    {
                        Task=1120;
                    }
                }
            }
            else
            {
                Task=2000;
            }
            break;
        case 1120:                                                              // golden :5408
            if(BarcodePosDelay[iSht].Off())
            {
                BarcodeDelay[iSht].SetMSAndOn(TestIF_File.iBarCodeDelay);
                sErrorPart="";
                sCheckErrorPart="";
                Task=1150;
            }
            break;
        case 1150:                                                              // golden :5417
            if(BAR_CODE_INSTALL==ebcUseOCR)//Ifor 20211129 OCR Shuttle1 Row A 使用第2隻CCD Row B使用第1隻CCD
            {
                if(bBarcodeNum[iBarCodeRowB]==false)
                {
                    iSH1_1BarcodePosition=iNowCheckStep;

                    if(SFCAT1_Barcode_StartScan_In(iBarCodeRowB, iNowCheckStep, FLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString)==true)
                    {
                        bBarcodeNum[iBarCodeRowB]=true;
                        SFCAT1_BarcodeInShGridSetCell(iBarCodeRowB, iNowCheckStep+1, FLCarryKit.cDeviceInf[0][iNowCheckStep]);
                    }
                }

                if(InArmSuck.iShtRow==2)
                {
                    if(bBarcodeNum[iBarCodeRowA]==false)
                    {
                        iSH1_2BarcodePosition=iNowCheckStep;

                        if(SFCAT1_Barcode_StartScan_In(iBarCodeRowA, iNowCheckStep, FLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString)==true)
                        {
                            bBarcodeNum[iBarCodeRowA]=true;
                            SFCAT1_BarcodeInShGridSetCell(iBarCodeRowA, iNowCheckStep+1, FLCarryKit.cDeviceInf[1][iNowCheckStep]);
                        }
                    }
                }
                else
                {
                    bBarcodeNum[iBarCodeRowA]=true;
                }
            }
            else    //Shuttle 1, Row A使用第1隻CCD, Row B使用第2隻CCD
            {
                if(bBarcodeNum[iBarCodeRowA]==false)
                {
                    iSH1_1BarcodePosition=iNowCheckStep;

                    if(SFCAT1_Barcode_StartScan_In(iBarCodeRowA, iNowCheckStep, FLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString)==true)
                    {
                        bBarcodeNum[iBarCodeRowA]=true;
                        SFCAT1_BarcodeInShGridSetCell(iBarCodeRowA, iNowCheckStep+1, FLCarryKit.cDeviceInf[0][iNowCheckStep]);
                    }
                }

                if(InArmSuck.iShtRow==2)
                {
                    if(bBarcodeNum[iBarCodeRowB]==false)
                    {
                        iSH1_2BarcodePosition=iNowCheckStep;

                        if(SFCAT1_Barcode_StartScan_In(iBarCodeRowB, iNowCheckStep, FLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString)==true)
                        {
                            bBarcodeNum[iBarCodeRowB]=true;
                            // golden :5468 writes RowA's CELL with RowB's DATA -- see BUG NOTE 5, preserved
                            SFCAT1_BarcodeInShGridSetCell(iBarCodeRowA, iNowCheckStep+1, FLCarryKit.cDeviceInf[1][iNowCheckStep]);
                        }
                    }
                }
                else
                {
                    bBarcodeNum[iBarCodeRowB]=true;
                }
            }

            if(bBarcodeNum[iBarCodeRowA]==true && bBarcodeNum[iBarCodeRowB]==true)
            {
                Task=1200;
            }
            else if(BarcodeDelay[iSht].Off())
            {
                if(bBarcodeNum[iBarCodeRowA]==false)
                {
                    FLCarryKit.cDeviceInf[0][iNowCheckStep]=asBarCodeErrorSend;
                    SFCAT1_BarcodeInShGridSetCell(iBarCodeRowA, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                }

                if(bBarcodeNum[iBarCodeRowB]==false)
                {
                    FLCarryKit.cDeviceInf[1][iNowCheckStep]=asBarCodeErrorSend;
                    SFCAT1_BarcodeInShGridSetCell(iBarCodeRowB, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                }

                if(WaitManualStepKey())
                {
                    Task=1200;
                }
            }
            break;
        case 1200:                                                              // golden :5505 -- falls through into 1300 on the "no columns left" arm
            bBarcodeNum[iBarCodeRowA]=false;
            bBarcodeNum[iBarCodeRowB]=false;
            iRetryCount=0;
            iNowCheckStep--;
            bBarcodeFirstAutoRetry[iBarCodeRowA]=false;
            bBarcodeFirstAutoRetry[iBarCodeRowB]=false;

            if(iNowCheckStep>=0 && iNowCheckStep<InArmSuck.iShtCol)
            {
                Task=1000;
                break;
            }
            else
            {
                Task=1300;
            }
            // fall through -- golden has no `break` on this arm
        case 1300:                                                              // golden :5522
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))
            {
                Task=2000;
            }
            break;
        case 2000:                                                              // golden :5528
            Task=0;
            return true;
    }
    return false;
}
