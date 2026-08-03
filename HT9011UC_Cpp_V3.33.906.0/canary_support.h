// =============================================================================
//  canary_support.h  --  W6.1 canary-support scaffold (Empty-tray SM)
//
//  Translation wave: W6.0 (canary-support scaffold for asendic_Empty)
//  Translator: AI(W6.0-SCAFFOLD) 20260626
//
//  Collects the small substrate/scaffold gaps the W6.1 Empty-tray canary needs
//  that are NOT yet in the translated global headers:
//
//    1. __FUNC__ macro
//       BCB6 compiler builtin (current function name as a string).  Standard C++
//       spells it __func__.  asendic_Empty.cpp passes __FUNC__ to functions that
//       take AnsiString (which has a const char* ctor), so __func__ works.
//
//    2. LAST_GENERAL_SET / LastSet (MINIMAL shim)
//       The golden LastSet.h (1162 lines, LAST_GENERAL_SET) is gated out of the
//       target (MachineDefine.h #if 0 block).  The canary touches ONLY
//       LastSet.iRealDummy (23 refs, all comparisons vs DUMMY/REALLY).  This is
//       a MINIMAL shim with just iRealDummy.  TODO(W6.x): replace with the full
//       translated LastSet.h when the LastSet subsystem lands.
//
//    3. Free-function decls the canary calls that are not in a shared target
//       header:
//         ShowErrorMessage  (golden note.h)        -- drives Retry/Skip in the SM
//         RecordProcess     (golden cMyDB.h)
//         ShowUnloaderTrayMessage (golden mymessbox.h)
//         ReadWriteTrayID   (golden csystem.h -- already declared via csystem.h)
//         WhichAutoNeedTray (golden acatchtray.h)
//         ShowMyMessage     (golden mymessbox.h -- matches database.cpp local decl)
//       Sim bodies live in canary_support.cpp.
//
//  EventReport (SECS) is NOT declared here -- the single EventReport call in the
//  canary is gated #if 0 (W7-SECS).  MyMessageBox (VCL) is NOT declared here --
//  the pre-alarm block that uses it is gated #if 0 (W6.x).
// =============================================================================
#ifndef canary_supportH
#define canary_supportH

#include "vclcompat/vcl_compat.h"   // AnsiString

// ---------------------------------------------------------------------------
//  1. __FUNC__  (BCB6 builtin -> standard __func__)
// ---------------------------------------------------------------------------
#ifndef __FUNC__
#define __FUNC__ __func__
#endif

// ---------------------------------------------------------------------------
//  2. LAST_GENERAL_SET / LastSet  (MINIMAL shim -- iRealDummy only)
//     Golden field type: int iRealDummy;  (LastSet.h:54)
// ---------------------------------------------------------------------------
struct LAST_GENERAL_SET
{
    int iRealDummy;     // golden LastSet.h:54.  ==DUMMY(0) in the sim canary path.
    // AI(W6.2-INARM) 20260626: the W6.2 HP pick/place geometry leaves
    // (ainarm_SearchPickPlate/PlacePlate) deref three more LastSet fields.
    // Added to this shared minimal shim (golden field types from LastSet.h):
    int iRunStartMode;  // golden LastSet.h -- run-start mode (rsmAutoSiteMap/rsmQAMode/...)
    int iTemperature;   // golden LastSet.h -- temperature mode (Tempture_Hot/...)
    int iTester;        // golden LastSet.h -- tester link (OFF_LINE/...)
    // AI(W7-CATCHTRAY) 20260626: the TrayArm engine (acatchtray.cpp) derefs the
    // ART tray-count + ATK-tray-feed + Loader-count LastSet fields.  Added to this
    // shared minimal shim (golden field TYPES verbatim from LastSet.h):
    int  iUnloaderTrayCount_ART[256];   // golden LastSet.h:205 -- per-Auto ART unload tray count
    int  iEmptyTrayCount_ART;           // golden LastSet.h:373
    int  iColorTrayCount_ART;           // golden LastSet.h:374
    bool bLoaderTrayCount_ART;          // golden LastSet.h:370
    bool bCleanOut_ART;                 // golden LastSet.h:377
    int  iUnloadFixTray;                // golden LastSet.h:120 -- ATK AMR tray-feed mode (eAtkTf*)
    int  iOutTrayNum[256];              // golden LastSet.h:458 -- production-log output tray serial
    int  iSCKARTInputCT;                // golden LastSet.h:406 -- SCK ART input count
    int  iP57_InputCT;                  // golden LastSet.h:496 -- P57 Loader-count AutoCleanOut
    long SendCT[4];                     // golden LastSet.h:14  -- per-port send count
    // AI(W6.4-TESTER) 20260626: the TESTER/INDEX engine (atester.cpp DoTestY case
    // 60/260 + CheckTwoArmSiteMap/CheckIndexArmInitState) derefs the socket-purge
    // count + the per-arm use-socket map.  Added to this shared minimal shim
    // (golden field TYPES verbatim from LastSet.h; [2][MAX_SOCKET_ROW=4][MAX_SOCKET_COL=8]):
    int  iD47SocketTestedCount;         // golden LastSet.h -- socket-purge tested count (D47)
    bool bUseTestSocket[2][4][8];       // golden LastSet.h -- per-arm[0/1] site enable map
    // AI(W6.5-CARRY) 20260626: the SHUTTLE/CARRY engine (acarry.cpp Do_Auto_SHT1/2
    // case 250 purge-fan branch) compares the local purge counter vs
    // LastSet.iShuttlePurgeCount (golden acarry.cpp:3693).  Added to this shared
    // minimal shim (golden field TYPE verbatim from LastSet.h:199):
    int  iShuttlePurgeCount;            // golden LastSet.h:199 -- In-Shuttle purge-fan threshold
    // AI(W6.2b-2x4_16) 20260626: the in-arm place-to-shuttle SM increments the
    // place-to-shuttle counter (golden DoPlaceToShuttle_9045_* :LastSet.lShuttleCount++).
    long lShuttleCount;                 // golden LastSet.h:393 -- count place to shuttle
    // AI(W7T1-Integrate) 20260701: the un-gated DoTestHeadMotor down-press / socket-IC-check
    // tree derefs the D41 test-socket IC-check-skip flag + the initial-IC-check-position mode
    // (golden atester.cpp case 12000/14000/600 socket-check branches).  Added to this shared
    // minimal shim (golden field TYPES verbatim from LastSet.h:132 bool / :274 int).
    bool bD41TestSocketICCheckSkip;     // golden LastSet.h:132 -- [D41] skip test-socket IC vacuum check
    int  iD41SocketInitialICCheckPosition; // golden LastSet.h:274 -- [D41] initial IC-check position (1=above socket)
    // AI(W5-Automation-Integrate) 20260710: fields required by the W5 Automation
    // wave's TeraPower AMR tray-count bridge (Automation/AMR.cpp) + HANA-ART SRQ
    // helper (Automation/HANA_ART.cpp) -- both are REAL compile dependencies (not
    // TU-local gates), added per those units' translate reports (golden field
    // TYPES verbatim from LastSet.h).
    int  iAMRTrayConut[9];              // golden LastSet.h:502 -- per-Auto/Loader AMR tray-in-flight count
    int  iAMRTrayLoaderTotal;           // golden LastSet.h:503 -- total Loader trays carried in by AMR
    bool bAMRTrayFeedWait;              // golden LastSet.h:504 -- waiting for AMR tray-feed completion
    bool bAMRLoaderLast;                // golden LastSet.h:505 -- Loader is down to its last cover tray
    bool bAMRRequestSupplyTray;         // golden LastSet.h:506 -- AMR supply-tray request pending
    int  iBinData32[4][260];            // golden LastSet.h:366 -- per-[arm/category] bin-count grid (also read by HANA_ART.cpp)
    int  iASEContact;                   // golden LastSet.h:379 -- ASE contact-count log index (HANA_ART.cpp)
    // AI(W5-Automation-Integrate) 20260710: fields required by
    // atester_ProcessCount.cpp (Low-Yield-AutoClean / ART load count / per-site
    // socket-contact count) -- REAL compile dependencies, golden field TYPES
    // verbatim from LastSet.h.
    int  iIndexCount;                   // golden LastSet.h:381 -- Low Yield Auto Clean index count
    long SendCT_ART[4];                 // golden LastSet.h:384 -- per-port ART LOAD count
    int  iSocketContactCount[4][8];     // golden LastSet.h:396 -- per-[row][col] socket contact count
    // AI(W5-Final-Integrate) 20260711: fields required by Automation/auto9045.cpp
    // (SetLotInfo/GetJamCount) -- ADDITIVE ONLY this integrate pass (auto9045.cpp
    // itself still uses its own TU-local W5FA_LS_iJamCount/W5FA_LS_BinCT stand-ins,
    // not retargeted here -- see this integrate's report for the rationale).
    int          iJamCount[3];          // golden LastSet.h:182
    unsigned int BinCT[4][256];         // golden LastSet.h:106
    // AI(W5-Final-Integrate) 20260711: 12 fields required by
    // Automation/SCK_ART_Remainder.cpp (UpdateCount/AddAlarmCode/ClearLotInfo) --
    // ADDITIVE ONLY (same not-yet-retargeted rationale; that file's own gate #4
    // TU-local stand-ins remain in place). CROSS-UNIT NOTE: the sibling
    // Automation/SCK_ART.cpp ALREADY carries its OWN separate TU-local copies of
    // iSCKART_RTUnitCount/lSCKARTBinCT[256] (same golden fields) -- when a future
    // wave retargets either translated unit onto these real shim fields, retire
    // BOTH TU-local copies so exactly one definition survives.
    bool bBreakSCKART;                  // golden LastSet.h:420
    int  iSCKART_RTUnitCount;           // golden LastSet.h:410
    long lSCKARTBinCT[256];             // golden LastSet.h:507
    int  iHdPass;                       // golden LastSet.h:20
    int  iHdFail;                       // golden LastSet.h:21
    int  iHdRejectATPass;               // golden LastSet.h:22
    int  iHdRejectATFail;               // golden LastSet.h:23
    int  iHdRejectBT;                   // golden LastSet.h:24
    int  iTesterMatch;                  // golden LastSet.h:25
    int  iTesterUnmatch;                // golden LastSet.h:26
    int  iTestIgnore;                   // golden LastSet.h:27
    int  iHdPickUp;                     // golden LastSet.h:28
    // AI(W5-Final-Integrate) 20260711: 4 more fields SCK_ART_Remainder.cpp's own
    // report flagged as "still owed" (needed by DoARTLotStart/DoAutoSocketOff,
    // both out of that unit's translated-function set, not consumed by anything
    // yet) -- added now since they are zero-risk, pure-additive POD fields and a
    // future SCK_ART.cpp-remainder wave will need them regardless.
    bool bWaitStartLotAutoRetestGPIB;   // golden LastSet.h:400
    bool bEndLotAutoRetestGPIB;         // golden LastSet.h:402
    bool bFirstTestAutoRetestGPIB;      // golden LastSet.h:405
    bool bUseTestSocketEE[2][4][8];     // golden LastSet.h:392
    // AI(W906-Automation) 20260716: field required by the (still-GATED this
    // wave) Automation/automation.cpp ProcessBuffer OLP command-dispatch
    // ladder (golden automation.cpp:1645/1656/1667 -- CATEGORY_REQUEST/
    // BINDEFINE_REQUEST/FIXTRAYDEFINE_REQUEST setting-error flags). Added now
    // as a small additive cross-file gap ahead of ProcessBuffer's own future
    // translation (per that front's task brief); not yet referenced by any
    // ACTIVE translated code this wave.
    int OLPSetBinErr[10];               // golden LastSet.h:472 -- Sam 20230921 : Bin 設定錯誤不能啟動 OLPSetBinErr[0]是CATEGORY_REQUEST設定錯誤。OLPSetBinErr[1]是BINDEFINE_REQUEST設定錯誤。OLPSetBinErr[2]是FIXTRAYDEFINE_REQUEST設定錯誤
    // AI(W906-W7-L1-Wave0) 20260801: 7 fields the W7-L1 asendic_* tray SM family
    // needs (deduped -- two of the seven were requested by two different files
    // each).  Golden field TYPES and NAMES verbatim from the cp950-decoded golden
    // LastSet.h, re-read this pass.  All 7 verified absent from this struct before
    // adding; note in particular that `bool bLoaderTrayCount_ART` (golden
    // LastSet.h:370) already above is a DIFFERENT field from
    // `int iLoaderTrayCount_ART` (:371) added here -- different name, different
    // type -- and that iUnloaderTrayCount_ART[256] (:205) was already present and
    // is correctly not duplicated.
    int  iLoaderTrayCount_ART;          // golden LastSet.h:371 -- Loader ART tray count (asendic_Loader.cpp:1229/:2082/:2105/:2849/:2861)
    int  iAutoRetestCount_ART;          // golden LastSet.h:372
    int  iLoaderTotalTray;              // golden LastSet.h:378
    int  iLoaderTraySimulateTime;       // golden LastSet.h:243
    // KEEP THE [3] BOUND VERBATIM -- it is golden's, and golden itself can
    // overrun it: golden PushUnLoaderTrayInAverageTime writes this array at index
    // `pos` while its own companion buffer is [MAX_AUTO_TRAY][5], so pos>=3 is an
    // out-of-bounds write IN GOLDEN.  Mirror the bound rather than widening it,
    // and constrain tests to pos 0..2.
    int  iUnLoaderTraySimulateTime[3];  // golden LastSet.h:244
    int  iDevice_Info_By_Tray[256];     // golden LastSet.h:464 (jou 20190930 -- Barcode Tray record file)
    int  RecodeTrayCount[256];          // golden LastSet.h:465 (frank 20200814 -- per-10-tray summary log)
    // RETIREMENT DEBT created by the three fields above that already have TU-local
    // stand-ins: csystem.cpp carries `static int W7C1_LastSet_iLoaderTotalTray`
    // (:1342), `W7C1_LastSet_iLoaderTrayCount_ART` (:1343) and
    // `W7C2_LastSet_iAutoRetestCount_ART` (:2561), each behind its own W7C1_LS_* /
    // W7C2_LS_* macro.  Wave 0 does NOT retarget them: csystem.cpp is an
    // integrator-only file that another front is also perturbing, and retargeting
    // is a behaviour change (the seams are separate storage today).  Whoever next
    // owns csystem.cpp should point those three macros at these real fields and
    // delete the statics, so exactly one copy of each survives.
    // AI(W906-W7-L2) 20260803: the ten vacuum-"dummy" timing fields that golden
    // ckernel.cpp's ScanSystemSensor writes on its PAUSE branch -- golden
    // ckernel.cpp:547-550 (in-/out-arm), :565-568 (front/back test arm) and
    // :613-614 (catch-tray arm).  Golden field TYPES and ARRAY DIMENSIONS taken
    // verbatim from the cp950-decoded golden LastSet.h, re-read this pass.
    // DIMENSION WARNING: the eight arm fields are [4][8], but the two CatchArm
    // fields are PLAIN SCALARS (golden LastSet.h:240-241) -- golden writes them
    // from a single Suck[0][0] (golden ckernel.cpp:613-614), NOT from a loop.
    // Do not "regularise" them into arrays.
    // BOUNDS NOTE: golden's own write loops are bounded by RUNTIME extents, not
    // by the declared [4][8] -- InArmSuck.iMaxRow/iMaxCol (golden
    // ckernel.cpp:543-545) and MAX_Index_Row/NEW_MAX_Index_Col (:561-563).  The
    // declared bound is mirrored verbatim rather than widened, exactly as the
    // iUnLoaderTraySimulateTime[3] note above does; constrain tests to i<4, j<8.
    // OFFLINE DEFAULT (all ten = 0 via `LastSet = {0}`, canary_support.cpp:26).
    // AI(W906-W7-L2) 20260803: an earlier draft of this paragraph claimed the
    // zeros "select no branch" and are handled by a "load-time sanitiser" whose
    // `<=20` test "substitutes the per-block max".  Re-reading golden cinitial.cpp
    // this pass disproves all three parts; corrected in place.
    //  * NOT LOAD TIME.  The enclosing golden function is
    //    `void UpdateMyKitSuckDelayTimeToProd()` (cinitial.cpp:6497) and its own
    //    comment two lines above -- cinitial.cpp:6495 `// 當換TRAY或更改點位時須呼叫`
    //    -- marks it a TRAY-/TEACH-POINT-CHANGE hook.  Its two golden call sites are
    //    cinitial.cpp:6663 (inside DoSetupSystemToProd) and AutoClean/AutoClean.cpp:739
    //    (inside CleanSetSpeed).
    //  * THE SUBSTITUTION IS A NO-OP ON AN ALL-ZERO LastSet.  For the EIGHT [4][8]
    //    arm arrays golden does run a `>=500 || <=20 -> max[]` pass
    //    (cinitial.cpp:6531-6545 in/out-arm, :6547-6559 F/B-test-arm), but the
    //    per-block max it substitutes is computed from those same fields at
    //    :6504-6527 starting from `int max[8]={0,...}` (:6499), so with all-zero
    //    input the max is itself 0 and :6536
    //    `LastSet.iInArmVacuumDummyOnTime[i][j]=max[0];` assigns 0 to 0.  Nothing is
    //    repaired; the zeros flow straight out to every suction cup --
    //    cinitial.cpp:6596-6599 copy the four arm arrays into
    //    In/OutArmSuck.Suck[i][j].VacuumOnTime/VacuumOffTime, and :6637-6640 copy the
    //    four test-arm arrays into F/BTestSuck.Suck[i][j].
    //  * THE TWO CatchArm SCALARS GET NO SANITISER AT ALL -- both loops above are
    //    array-only.  Their 0 propagates verbatim through cinitial.cpp:6646
    //    `CatchTraySuck.Suck[0][0].VacuumOnTime           =LastSet.iCatchArmVacuumDummyOnTime;`
    //    and :6647 (the VacuumOffTime twin), which are UNCONDITIONAL straight-line
    //    statements at the tail of that function.  So 0 IS a behaviour selection
    //    here: ZERO catch-tray-nozzle vacuum dummy on/off timing.  Those two lines
    //    are also the ONLY golden READERS of either scalar (whole-golden-tree grep
    //    this pass; every other hit is a write -- ckernel.cpp:613-614,
    //    main.cpp:24586-24587 and :26032-26033).  A test that cares about catch-tray
    //    suction timing must set them explicitly.
    int  iInArmVacuumDummyOnTime[4][8];     // golden LastSet.h:230
    int  iOutArmVacuumDummyOnTime[4][8];    // golden LastSet.h:231
    int  iFTestArmVacuumDummyOnTime[4][8];  // golden LastSet.h:232
    int  iBTestArmVacuumDummyOnTime[4][8];  // golden LastSet.h:233
    int  iInArmVacuumDummyOffTime[4][8];    // golden LastSet.h:235
    int  iOutArmVacuumDummyOffTime[4][8];   // golden LastSet.h:236
    int  iFTestArmVacuumDummyOffTime[4][8]; // golden LastSet.h:237
    int  iBTestArmVacuumDummyOffTime[4][8]; // golden LastSet.h:238
    int  iCatchArmVacuumDummyOnTime;        // golden LastSet.h:240 -- SCALAR (see warning above)
    int  iCatchArmVacuumDummyOffTime;       // golden LastSet.h:241 -- SCALAR (see warning above)
    // TODO(W6.x): the other ~430 LAST_GENERAL_SET fields land with the full
    //             translated LastSet.h.
};
extern LAST_GENERAL_SET LastSet;    // golden: extern LAST_GENERAL_SET LastSet; (LastSet.h:514)

// ---------------------------------------------------------------------------
//  3. Free-function decls (golden homes noted) + sim bodies in canary_support.cpp
// ---------------------------------------------------------------------------
//  ShowErrorMessage -- golden note.h:466.  In the sim it returns the Kxxx code
//  that drives the SM's Retry/Skip branch (default K_RETRY).  Signature VERBATIM.
int  ShowErrorMessage(AnsiString Code, int KCode, int Pos,
                      bool bDuplicateErr=false, AnsiString errPart=" ");         //Steven 20091231

//  RecordProcess -- golden cMyDB.h:63.  Sim: logs to stdout.
void RecordProcess(AnsiString S, AnsiString S2="");

//  ShowUnloaderTrayMessage -- golden mymessbox.h:60.  Sim: logs (no UI).
void ShowUnloaderTrayMessage(AnsiString S1, AnsiString S2);                      //Jou 20150721

//  WhichAutoNeedTray -- golden acatchtray.h:8.  Sim: 0 (no auto needs a tray).
int  WhichAutoNeedTray();

//  ShowMyMessage -- golden mymessbox.h:58.  Signature matches the database.cpp
//  local forward-decl (S3 defaulted) so the two TUs agree.  Sim: logs (no UI).
void ShowMyMessage(AnsiString S1, AnsiString S2="", AnsiString S3="",
                   bool Ok=false, bool bServoOff=false);

// ---------------------------------------------------------------------------
//  4. AI(W906-W7-L1-Wave0) 20260801 -- OBSERVABILITY SEAM for the two operator
//     dialogs above.  ONE seam set, landed once, in the serialized pass.
//
//  WHY IT CANNOT BE DEFERRED.  canary_support.cpp's ShowErrorMessage returned
//  K_RETRY UNCONDITIONALLY.  Every K_SKIP / K_CLEAN_OUT recovery arm reached
//  through it is therefore structurally unreachable, so any "the skip path is
//  covered" claim about those arms is unfalsifiable -- precisely the tautology
//  class this project has already paid for on this file family.  With
//  W906_ShowErrorMessage_SimReturn a test can feed K_SKIP and actually walk the
//  arm; with the count/last-code fields it can also prove the alarm was raised at
//  all, and with WHICH code.  (The W7-L1 recons put the affected arm count in the
//  low tens across the asendic_* family -- that figure is THEIRS and was not
//  re-counted in this pass, so treat it as an estimate, not a measurement.  The
//  structural argument above does not depend on the number.)
//
//  WHY IT IS ONE SEAM AND NOT SIX.  Five separate recon agents each proposed
//  their own version under three different names.  123 files in this tree carry
//  an `#include "canary_support.h"` line (counted this pass; a looser grep for
//  the mere string reports 141, which is why the exact figure is stated here), so
//  six competing seams would be six collisions on one very widely included
//  header.  Naming follows the established `W906_..._Sim` idiom named in the
//  forms/fMain.h facade contract (rule 4).
//
//  HEADER COST: deliberately zero new includes.  These are `extern` declarations
//  only; K_RETRY (cmydef.h:276, `extern const int`, value 0x0001 at cmydef.cpp:337)
//  is applied in canary_support.cpp, which already includes cmydef.h.
//
//  NOTE ON THE ShowMyMessage HALF: golden's ShowMyMessage returns void, so there
//  is no "next return" to make settable -- it gets the other three (last-argument
//  capture, call count, reset) and nothing more.  That asymmetry is golden's, not
//  an omission.
// ---------------------------------------------------------------------------
//  ShowErrorMessage seam
extern int        W906_ShowErrorMessage_SimReturn;  // next return value; RESET VALUE IS K_RETRY (the pre-seam behaviour)
extern AnsiString W906_ShowErrorMessage_LastCode;   // last `Code` argument seen; "" when never called
extern int        W906_ShowErrorMessage_LastKCode;  // last `KCode` mask seen (which buttons golden offered); 0 when never called
extern int        W906_ShowErrorMessage_Count;      // call count -- distinguishes "not called" from "called and returned K_RETRY"
void W906_ShowErrorMessage_Reset();                 // restore SimReturn to K_RETRY, clear capture + count
//  ShowMyMessage seam
extern AnsiString W906_ShowMyMessage_LastS1;        // last `S1` argument seen; "" when never called
extern int        W906_ShowMyMessage_Count;         // call count
void W906_ShowMyMessage_Reset();                    // clear capture + count

// ---------------------------------------------------------------------------
//  5. AI(W906-W7-L2) 20260803 -- the three golden note.h FREE FUNCTIONS that
//     ckernel.cpp's motor/cylinder alarm path calls.  note.cpp is NOT ported
//     (307 KB, TfNote form unit), so these have no home anywhere in the tree --
//     verified this pass: `grep -rn` for all three over the whole ported tree
//     returned ZERO hits before this edit.
//
//  Header line numbers verified by re-reading the cp950-decoded golden note.h:
//     :467  void       ShowMotorErrorMessage(AnsiString, int, AnsiString="")
//     :468  AnsiString MotorIndexToJamCode(int)
//     :470  AnsiString CylinderIndexToJamCode(int, int*)
//
//  ROUTE TAKEN PER FUNCTION (each decided by reading the golden BODY, not the
//  declaration):
//
//   * MotorIndexToJamCode  -> REAL TRANSLATION.  Golden note.cpp:4291-4296 is
//     four lines, VCL-free and side-effect-free: `S.sprintf("WAR24%03d", MotNo)`.
//     A real body beats a sim body and is directly unit-testable.
//
//   * CylinderIndexToJamCode -> REAL TRANSLATION (of the mapper), with ONE
//     documented omission.  Golden note.cpp:4156-4284 is a pure 20-branch
//     cylinder-index -> owning-unit table plus `S.sprintf("JAM%d", Code)`; all
//     71 constants it needs (C_* cylinder ids, MM*/M* unit ids) already exist in
//     this tree as `extern const int` in cmydef.h -- each of the 71 was checked
//     individually this pass, zero missing.  The omission is golden's opening
//     `for(int i=0;i<6;i++) fNote->Select[i]=false;` (note.cpp:4161-4162): the
//     ported TfNote (forms/fNote.h) has no `Select[]` member, that file is NOT
//     owned by this front, and the field is a pure note-DIALOG button pre-clear
//     (golden note.h:396 `bool Select[10];` //ChungHung HT9045 2011/12/13
//     //Input pickup device error時,按"retry"鍵,機台都會自動home add BtnHome).
//     Nothing in ProcessAlarm reads it -- the function's entire contract with
//     golden ckernel.cpp:2522-2523 is the returned JAM string and *Pos, and BOTH
//     are translated faithfully.  See the report's integrator notes.
//
//   * ShowMotorErrorMessage -> RECORDING SIM (mandated, and independently
//     correct: golden note.cpp:1052+ drives StopAllMotor, Galil "ST", the note
//     form, MyDBIEvent, fFTPClient and fAutomation).  It is NOT a bare no-op --
//     see canary_support.cpp for the three global side effects and the two
//     control-flow arms that ARE reproduced faithfully.
// ---------------------------------------------------------------------------
//  golden note.h:468 -- REAL body (golden note.cpp:4291).  "WAR24%03d" % MotNo.
AnsiString MotorIndexToJamCode(int MotNo);

//  golden note.h:470 -- REAL mapper (golden note.cpp:4156).  //Steven 20231127 :
//  氣缸Alarm改成自動生成, 分類31.  Writes the owning unit id through *Pos and
//  returns "JAM<Code>".  *Pos is ALWAYS written (golden's final else, :4277-4280,
//  defaults it to MMSystem), so callers may leave it uninitialised, as golden
//  ckernel.cpp:2505 does.
AnsiString CylinderIndexToJamCode(int Code, int *Pos);

//  golden note.h:467 -- RECORDING SIM.  Signature VERBATIM (errPart defaulted).
void ShowMotorErrorMessage(AnsiString Code, int MotorAlarmNo, AnsiString errPart="");

//  ShowMotorErrorMessage recorder.  The W7-L2 ckernel test plan asserts this
//  fired EXACTLY ONCE with a specific JamCode, so the count and the last Code
//  are both required; MotorAlarmNo is captured too because golden ckernel passes
//  a *derived* value (`iRef+1`, golden ckernel.cpp:2471/:2491) that is worth
//  pinning independently of the code string.
extern AnsiString W906_ShowMotorErrorMessage_LastCode;         // last `Code`; "" when never called
extern int        W906_ShowMotorErrorMessage_LastMotorAlarmNo; // last `MotorAlarmNo`; 0 when never called
extern AnsiString W906_ShowMotorErrorMessage_LastErrPart;      // last `errPart`; "" when never called
extern int        W906_ShowMotorErrorMessage_Count;            // call count
void W906_ShowMotorErrorMessage_Reset();                       // clear capture + count

// ---------------------------------------------------------------------------
//  6. AI(W906-W7-L2) 20260803 -- MyDBIProcessNew (golden cMyDB.h:21, //Steven
//     20161220 : Process加上Alarm Code).  golden ckernel.cpp:2452 calls it from
//     GetMotorAlarmCode, so the W7-L2 translation wave needs a real declaration.
//
//  COLLISION ANALYSIS (done before landing this, because the tree already had a
//  TU-local stand-in).  atester_32Site.cpp:410 defines
//  `static void W5_32S_MyDBIProcessNew(...)` and :414 does
//  `#define MyDBIProcessNew W5_32S_MyDBIProcessNew`.  That is SAFE against this
//  declaration, and the reason is ORDERING, not luck:
//    - atester_32Site.cpp includes canary_support.h at its line 175;
//    - the #define lands at line 414, i.e. AFTER every #include in that TU
//      (its last #include is at :175 -- checked, there are no later ones).
//  So the preprocessor sees this prototype with its real spelling, and only the
//  CALL SITE at atester_32Site.cpp:1857 is rewritten to the static stand-in.
//  There is also no link-time clash: the stand-in has internal linkage, and that
//  TU never references the external symbol.  NOTHING in atester_32Site.cpp needs
//  to change -- which is just as well, since this front does not own it.
//  KEEP THIS DECLARATION ABOVE any future #include added to that file.
void MyDBIProcessNew(AnsiString asTable, AnsiString AlarmCode, AnsiString S1, AnsiString S2=" ");

//  MyDBIProcessNew recorder -- ckernel.cpp:2452 writes the JCET-gated
//  "WAR240004" motion row, and a test needs to tell "row written" from "branch
//  not taken" (see the memory note on CCW/sensor alarms having two emit paths).
extern AnsiString W906_MyDBIProcessNew_LastAlarmCode;   // last `AlarmCode`; "" when never called
extern int        W906_MyDBIProcessNew_Count;           // call count
void W906_MyDBIProcessNew_Reset();

// ---------------------------------------------------------------------------
//  7. AI(W906-W7-L2) 20260803 -- THE ALARM-QUEUE SEAM (PopUpAlarm/ClearAllAlarm)
//
//  ####################################################################
//  #  READ THIS BEFORE CHANGING THE SIGNATURES.  The brief for this    #
//  #  item stated that an exhaustive scan of the golden tree found     #
//  #  ZERO declarations and ZERO definitions of PopUpAlarm and         #
//  #  ClearAllAlarm, and that they therefore had to be DESIGNED from   #
//  #  scratch.  That premise is WRONG, and this pass disproved it.     #
//  ####################################################################
//
//  The scan was right about the SOURCE TREE and wrong about the PROJECT.  Both
//  functions live in a sibling BCB6 *component package* that is not part of the
//  translated source tree:
//
//      D:\HT9045\elec\Component\halarm.h    :36  void PACKAGE ClearAllAlarm();
//                                                     // 清除所有物件的錯誤碼
//                                           :37  bool PACKAGE PopUpAlarm(
//                                                     TComponent **Component,
//                                                     int &iErrCode);
//                                                     // 取出要顯示的錯誤訊息
//      D:\HT9045\elec\Component\HAlarm.cpp  :234 ClearAllAlarm() body
//                                           :264 PopUpAlarm() body
//
//  The trail was left in this very tree: Motor/HTMotor.h:9-11 records that the
//  W4 wave dropped `#include "halarm.h"` and identified it as "VCL TComponent
//  class".  Golden ckernel.cpp compiles against halarm.h through its own first
//  line -- golden ckernel.cpp:1 is `#include "MachineDefine.h"` and golden
//  MachineDefine.h:63 is `#include "halarm.h"`; golden Motor/HTMotor.h:5 carries
//  the same live include (both re-read this pass).
//  AI(W906-W7-L2) 20260803: an earlier draft of this paragraph ALSO listed
//  mycylin.cpp as an includer.  IT IS NOT ONE -- golden mycylin.cpp:6 is
//  `//#include "halarm.h"`, commented out; that TU only carries
//  `extern HAlarm  *Alarm;` (mycylin.cpp:83).  Corrected in place; the two live
//  includes above carry the conclusion on their own.
//  So these are TRANSLATED below from real bodies, not invented.
//
//  GOLDEN SEMANTICS (HAlarm.cpp:264-283, read in full):
//    PopUpAlarm pops the FRONT of a FIFO of `ERR_MSG { TComponent *ObjPtr;
//    int iErrCode; }` (HAlarm.cpp:16-19) held in `ShowAlarmList` (:22,
//    // 顯示錯誤串列).  On a non-empty list it writes *Component = P->ObjPtr
//    (:273, // 指向錯誤物件), iErrCode = P->iErrCode (:274, // 傳回錯誤碼),
//    Delete(0)s the entry, frees it, and returns true.  On an empty list it
//    falls through to `return false` (:282) WITHOUT touching either out-param.
//    ClearAllAlarm (:234-242) walks every registered HAlarm clearing its code
//    list, then sets SystemNG=false (:241).
//
//  POINTER TYPE -- READ THIS, IT DIVERGES FROM GOLDEN ON PURPOSE.
//  Golden spells the out-param `TComponent **` (halarm.h:37), and golden
//  ckernel.cpp:2503 declares `TComponent *Comp;` to match.  This tree types it
//  `HTMotor **` instead.  Why:
//    - The ONLY thing golden ever does with the popped pointer is an IDENTITY
//      comparison against a motor: `if(MOT[i].Motor==Comp && ...)` at golden
//      ckernel.cpp:2440, inside GetMotorAlarmCode(TComponent *Comp) (:2432).
//    - `MOT[i].Motor` is `HTMotor *` -- golden Motor/mymotor.h:56, and likewise
//      ported Motor/mymotor.h:139 (MOT[] itself is `TTrayMotor MOT[]`, ported
//      Motor/mymotor.h:385, whose base TMyMotor owns that member).
//    - Golden's `class HTMotor` (golden Motor/HTMotor.h:31) has NO base class
//      and no conversion operator -- checked explicitly this pass, and the same
//      is true of the ported one (Motor/HTMotor.h:79).  So golden's own
//      `HTMotor* == TComponent*` comparison at :2440 is between unrelated
//      pointer types and does not type-check under standard C++.  That is an
//      anomaly IN GOLDEN (flagged in this front's report); it is not something
//      the port can faithfully reproduce, because it does not compile.
//    - Typing the seam on `HTMotor **` keeps the ONE semantic that matters --
//      pointer identity of the alarming motor -- and lets the ckernel
//      translation write golden's comparison verbatim with no cast.  Typing it
//      on vclcompat::TComponent would force a cast at :2440 and buy nothing.
//  The POINTER LEVEL is unchanged from golden: pointer-to-pointer out-param.
//
//  HEADER COST: still zero new includes.  `class HTMotor;` is a forward
//  declaration -- pointer-to-incomplete is all the prototype needs, and it is
//  compatible with the real definition in Motor/HTMotor.h:79.  This matters:
//  123 TUs include this header.
//
//  ALM_MOTOR_MOVE is deliberately NOT defined here.  Motor/HTMotor.h:29 already
//  has `#define ALM_MOTOR_MOVE 55555`, identical to golden ckernel.cpp:2499, and
//  the ckernel translation will include that header anyway for MOT[].
//
//  OFFLINE DEFAULT AND WHICH BRANCH IT SELECTS.  With an empty queue PopUpAlarm
//  returns false, so golden ProcessAlarm's `while(PopUpAlarm(&Comp, iCode))`
//  (golden ckernel.cpp:2507) runs ZERO iterations and the function falls
//  straight through to ClearAllAlarm() at :2526.  This is not "false is safe" --
//  it is golden's own empty-list arm (HAlarm.cpp:266 guards on
//  `ShowAlarmList->Count>0`, :282 returns false otherwise), and it is the
//  CORRECT arm offline because the list is only ever populated by HAlarm::Set
//  (HAlarm.cpp:110-130) on an HAlarm object, and HAlarm objects only exist once
//  something constructs one (HAlarm.cpp:49 `HAlarmList->Add(this)`).  An offline
//  facade constructs none, so "nothing has alarmed" is the faithful state.  Use
//  W906_PopUpAlarm_Push() to leave that arm.
// ---------------------------------------------------------------------------
//  AI(W906-W7-L2) 20260803: forward declaration only -- see HEADER COST above.
class HTMotor;

//  golden halarm.h:37 / HAlarm.cpp:264.  Element type retargeted TComponent* ->
//  HTMotor* per the POINTER TYPE note above.  Out-params are left UNTOUCHED when
//  the queue is empty, exactly as golden does.
bool PopUpAlarm(HTMotor **ppComp, int &iCode);

//  golden halarm.h:36 / HAlarm.cpp:234.
void ClearAllAlarm();

//  --- test-settable push + call recorder ---------------------------------
//  Golden's push is HAlarm::Set (HAlarm.cpp:110-130), a member of a VCL
//  component this tree does not have; W906_PopUpAlarm_Push is its offline
//  stand-in and appends to the SAME FIFO PopUpAlarm drains.  A test can push one
//  ALM_MOTOR_MOVE(55555) entry and one non-motor (cylinder) entry and then watch
//  ProcessAlarm drain both -- taking the iCode==ALM_MOTOR_MOVE arm (golden
//  ckernel.cpp:2512) once and the CylinderIndexToJamCode/ShowErrorMessage arm
//  (:2520-2524) once.
//  NOTE: golden's HAlarm::Set DEDUPLICATES -- `if(GetStat(iCode)) return;`
//  (HAlarm.cpp:112) drops a code already outstanding ON THAT COMPONENT.  This
//  push does NOT deduplicate, because dedup state lives in the per-HAlarm
//  ErrNoList that this facade has no equivalent of.  Pushing the same code twice
//  therefore yields two pops here but would yield one in golden; tests that care
//  should push distinct codes.
void W906_PopUpAlarm_Push(HTMotor *pComp, int iCode);
int  W906_Alarm_QueueDepth();                       // entries still queued
extern int W906_PopUpAlarm_Count;                   // SUCCESSFUL pops (true returns) only
extern int W906_PopUpAlarm_LastCode;                // last iCode handed out; 0 when never popped
extern int W906_ClearAllAlarm_Count;                // ClearAllAlarm call count
void W906_Alarm_Reset();                            // empty the queue + clear all three counters

#endif // canary_supportH
