// =============================================================================
//  test_w7_l1_loader.cpp  --  W7-L1 Wave 1: asendic_Loader + asendic_Loader_RT
//
//  Translation wave: W906-W7-L1 Wave 1 (Loader bundle)
//  Author: AI(W906-W7-L1-Loader) 20260802
//
//  WHAT IS UNDER TEST
//  ------------------
//  The Loader (incoming IC tray supply) stack-feed state machine
//  (asendic_Loader.cpp, 16 golden functions) and its round-trip / auto-retest
//  recycle variant (asendic_Loader_RT.cpp, 12 golden functions), driven over the
//  Sim HAL (Cylinder[] / MOT[] / Sen[] / SW[]) with no hardware and no window.
//
//  PROOF OBJECTIVE
//  ---------------
//  Every assertion below constrains a SPECIFIC observable and has been proven
//  capable of failing -- see FAILABILITY PROOF.  Three deliberate rules, each
//  adopted because this project has already shipped the corresponding defect:
//    1. TRAJECTORIES ARE EXACT AND ORDERED, never set membership.  Each Do* run
//       records the cursor after every tick, collapses consecutive duplicates,
//       and compares against an ordered expected list.  Set membership is how a
//       missing case survived three reviews on the sibling Auto2 file.
//    2. NO ASSERTION PUTS THE LOOP'S OWN TERMINATION CONDITION INTO ITS
//       DISJUNCTION.  There is no `done || steps == BOUND` anywhere here.
//    3. REACHED IS NOT COMMANDED.  Offline, Push() and Pop() BOTH return true
//       (mycylin.cpp: the no-sensor/no-delay exit sets bCylinderOn and returns
//       true in both), so a cursor trajectory cannot distinguish an extend from
//       a retract.  Every actuator claim below therefore reads STATE --
//       Cylinder[].bCylinderOn or SW[].OutValue -- and every actuator is
//       PRE-DRIVEN TO THE OPPOSITE of the expected value first, so "call
//       deleted" fails the same assertion as "call inverted".
//
//  WHY SW[].OutValue AND NOT SW[].Status():  TMySwitch::On()/Off() assign
//  OutValue UNCONDITIONALLY as their first statement, before the
//  `if(Enable==false) return;` guard (myswitch.cpp:76 / :126), so OutValue is a
//  faithful record of the last call.  Status() collapses to false whenever
//  Enable==false (myswitch.cpp:187-188) and is unusable here.
//
//  WHY Cylinder[].Enable MATTERS:  TMyCylinder::On()/Off() set
//  Status/Change/bCylinderOn unconditionally; it is the internal
//  OnSwitch()/OffSwitch() (the IO write) that is .Enable-gated.  But the LIFTER
//  helpers one level up -- asendic.cpp's CylinderUp/CylinderMiddle/CylinderLower
//  -- only call On()/Off() `if(Cylinder[...].Enable)`.  So the fixture must
//  ENABLE C_Load_Up and C_Load_Middle or every lifter assertion is vacuous.
//  Same for the golden call sites guarded by `if(Cylinder[X].Enable)`
//  (C_LoaderEdgePush, C_LoaderUpPress, C_TrayY_Fixer) and by
//  `if(SW[X].Enable)` (SwLoaderAirClean).
//
//  TWO FIXTURE SEAMS
//  -----------------
//  1. VIRTUAL TIME.  TQPF_Timer is a QueryPerformanceCounter wall clock with no
//     injection point, so a 20 s deadline can never be outlasted by a tight
//     loop.  The SMs' file-scope timers are non-static, so the extern
//     declarations below reach them without touching either .h.  advanceTime()
//     re-arms ONLY THE PROGRESS TIMERS to 0 ms.  The JAM watchdogs are
//     DELIBERATELY NEVER ADVANCED and are listed by name below with the arm each
//     one guards.  Two timers are DUAL-ROLE (watchdog in one case, progress in
//     another); each of those is pinned ONLY at the cursor where its progress
//     role is live:
//        hLoaderTrayToFront -- 20 s JAM1012 watchdog at DoLoaderTrayToFront
//                              case 100 (golden asendic_Loader_RT.cpp:291), but
//                              RE-ARMED as the 1+ULD_TrayArrivalWaitDelay
//                              arrival settle timer on the 100->200 hand-off
//                              (golden :265) and read by case 200 (golden :316).
//                              Advanced ONLY while the cursor is on 200.
//        hLoaderTrayToRear  -- 20 s JAM1001 watchdog at DoLoaderTrayToRear
//                              case 200 (golden :394), RE-ARMED as the
//                              LD_TrayArrivalDely settle timer by case 450
//                              (golden :446) and read by case 460 (golden :450).
//                              Advanced ONLY while the cursor is on 460.
//        LoaderDelay        -- 1 s JAM0913 watchdog at DoLoadNewLoaderTrayToCar
//                              case 420 (golden :153), also the settle timer
//                              read by case 400 (golden :125).  Advanced ONLY
//                              while the cursor is on 400.
//  2. SIM SENSOR LEVELS.  Sen[] defaults to Enable==false, which makes IsOn()
//     AND IsOff() BOTH return false -- an "unknown" reading that is NOT "off",
//     and the distinction is load-bearing all over this file family (golden
//     tests `IsOff()==false` in a dozen places, which "unknown" satisfies).
//     Three helpers, all proven in sub-test [2]:
//        simSensorUnknown : Enable=false                 -> IsOn 0, IsOff 0
//        simSensorOff     : Enable + Type=TYPE_A(1)      -> IsOn 0, IsOff 1
//        simSensorOn      : Enable + Type=TYPE_B(0)      -> IsOn 1, IsOff 0
//
//  Offline: no vendor SDK, no hardware, no window.
//
//  ===========================================================================
//  NOT-COVERED REGISTER -- this is a deliverable, not an apology
//  ===========================================================================
//  Silence would read as coverage.  Each entry names the arm and the fixture
//  invariant (or missing substrate) that makes it dead.  Four invariants do most
//  of the pruning:
//    (i)   LastSet.iRealDummy == DUMMY (0) for every sub-test.
//    (ii)  Every machine-configuration global is at its cmydef.cpp default:
//          TRAY_ARM_MODE=0 (eAboveCoveyor), INSTALL_OCR=0 (eocrUninstal),
//          INSTALL_OCR_YMot=0 (eocrYMotUninstal), USE_LdUldCassetteMode=0,
//          DOUBLE_BELT_MODE=0, USE_TRAY_MAPPING=etmUninstall,
//          USE_AUTO_RETEST=eartUninstall, USE_LOADER_VIBRATION=false,
//          USE_CKD_FCM_CleanAir=false, MACHINE_HAS_AUTO_ALIGNMENT_CCD=false,
//          CUSTOMER_CODE=0.  (Measured, not assumed -- printed by the
//          exploratory harness this test was derived from.)
//    (iii) Every form predicate is offline false: fAGV->IsATK_AMR(),
//          fAGV->IsSPIL_AMR(), fTrayForm->IsEnableColorSensor(),
//          fTrayMapping->IsRunTrayMapLaser(), fTrayMapping->IsRunDeviceRemain-
//          Laser(), fMain->ALed1->Value.
//    (iv)  No sub-test ever raises fMain->ALed1->Value, so DoLoad's
//          load-a-new-tray-from-the-magazine limb (case 800 tail -> 900) is
//          never entered.
//
//  A. THE WHOLE ALARM SURFACE IS UNENTERED, and the reason is stronger than
//     "ShowErrorMessage answers K_RETRY".  Sub-test [18] asserts the harness's
//     own measurement: across the entire suite ShowErrorMessage is called ZERO
//     times.  Every JAM/MES/WAR arm in both files is therefore NOT COVERED:
//     JAM0901, JAM0909, JAM0912, JAM0913, JAM0929, JAM1001, JAM1003, JAM1012,
//     JAM09102, JAM09200, MES0920, MES0921, MES0922, WAR0119, WAR0952, WAR0953,
//     WAR0961, WAR0962, WAR09103, WAR09106, WAR09200, WAR16122, WAR16336,
//     WAR0612.  So are every K_SKIP and K_CLEAN_OUT recovery arm they gate --
//     the W906_ShowErrorMessage_SimReturn seam exists and would make them
//     falsifiable, but this suite does NOT use it and does NOT claim them.
//  B. TWO ARMS ARE STRUCTURALLY UNREACHABLE UNTIL WAVE 3, because
//     asendic.cpp's TrayMoveStatus() is a hardwired `return 0` with no seam (see
//     the note above its declaration in asendic.h):
//       * DoLoaderTrackDetectICFloating cases 400 and 500 both re-arm
//         DetectLoaderTime to 20 s on EVERY pass (golden :3093-3094 / :3126-3127)
//         because `TrayMoveStatus(...)==0` is always true, so their JAM1012 /
//         JAM0901 arms can never fire and the SM parks at 500 forever.
//         Sub-test [15] asserts exactly that -- the park is the observation, not
//         a failure.
//       * CheckLoaderICFloating's `bDir` is `TrayMoveStatus(...)==1` (or ==2),
//         never true, so its whole JAM09102 "IC floating detected" block
//         (golden :3181-3199) is dead; and inside the latched branch the
//         htDetectTrayRetryDelay re-arm (golden :3203-3204) fires every call, so
//         the JAM0912 / JAM0901 retry-timeout arm (golden :3220-3227) is dead
//         too.  Sub-test [16] covers only the two live exits.
//  C. GOLDEN'S OCR SUB-FLOWS ARE NOT ENTERED (INSTALL_OCR==eocrUninstal by (ii)):
//     DoSupplyNewICTray cases 2000/2100/2200/2250/2300 and DoLoadNewICTray cases
//     1900/1901/1910/2000/2100/2200/2250/2300.  This also means the two
//     `case 2250 -> case 2300` GOLDEN FALL-THROUGHS are NOT pinned by any
//     assertion -- they are preserved in the source and stated in the .cpp
//     banner, but this test cannot reach them.
//  D. NOT ENTERED because their configuration gate is off by (ii):
//     DoSupplyNewICTray cases 130/140/160/250/1060/1065/1070/1240/1245/1248/
//     1250/1290/1291/1400/3000/3100/4000/6000; DoLoadNewICTray's ART overflow
//     block (golden :2053-2112) and its colour-sensor block (golden :2114-2133);
//     DoLoad cases 100/200/400/500/900/950; the eUnderCoveyor limbs everywhere.
//  E. DoInspectTrayColorOnLoader IS NEVER ENTERED BEYOND ITS FIRST LINE.
//     fTrayForm->IsEnableColorSensor() is offline false, so golden :1878's guard
//     returns true immediately and cases 1/100/200/201/300/9999 are all dead.
//     That guard also makes both GetColorSensorIsMapping call sites dead, which
//     is why the TU-local stand-in for it (asendic_Loader.cpp stand-in #1) costs
//     nothing observable.
//  F. DoAutoLoader cases 20/30/40/50/60/65/70 ARE reached by sub-test [11] but
//     only their DUMMY limbs; the `bFlag` limb of case 40 needs
//     Sen[SnLoaderFixCyPush].IsOn(), never driven, and case 65's stale-tray
//     re-sync needs LastSet.iRealDummy>=HAS_TRAY, false by (i).
//  G. NO REAL LIFTER MOTION IS EXERCISED.  asendic.cpp's CylinderUp / Middle /
//     Lower are declared SIM-SHAPE bodies (they drive the paired Cylinder[] and
//     return true immediately); the real golden lifter state machines are Wave 3.
//     What [3]/[9]/[10] prove is that the correct lifter helper is called at the
//     correct cursor and that it moved the correct pair of cylinders -- which IS
//     falsifiable (see mutations L1/L2/U2/U3) -- and nothing about lifter timing.
//  H. The `#ifdef SOFT_SIMULTE` arms are not compiled in this build.
//  I. TWO WRITES ARE EXECUTED BUT NOT INDEPENDENTLY OBSERVABLE, because a twin
//     earlier on the same path already put the flag in the asserted state.  Both
//     were MEASURED, not assumed (mutations Q11 and G5 below stayed GREEN when
//     the later site alone was deleted, and only went red when the pair was):
//       * DoTrayZLoadTrayToWait case 400's `MOT[MMTrayZ].fHasTray=true`
//         (golden :1790) -- case 55 already set it (golden :1683/:1696/:1702).
//       * DoUnLoadNewLoaderToStack case 1's `MOT[MMTrayZ].SetTray(NULL_IC)`
//         (golden :703) -- case 50 repeats it (golden :741).
//     The assertions that read those flags are load-bearing for the PAIR, and the
//     banner says so rather than implying per-site coverage.
//  J. THE ELEVEN TU-LOCAL STAND-INS asendic_Loader.cpp installs (see its own
//     banner) are all no-ops or constants, so nothing downstream of
//     GetColorSensorIsMapping / PushLoaderTrayInAverageTime /
//     RecordAutoCleanOutStartEnd / the AutoAlignment pair / the OCR pair /
//     MyDeCodeASCII / EditTray / ShowMessage / LogIndexMaxMinPos is exercised.
//     Every one of those call sites is ALSO inside an arm this register already
//     lists as dead, except LogIndexMaxMinPos (golden :193, on [5]'s driven path
//     -- it runs, and does nothing).
//
//  ===========================================================================
//  FAILABILITY PROOF -- REPRODUCIBLE RECIPE
//  ===========================================================================
//  Everything below was run in a private scratch dir.  The shared build/ tree was
//  never written to.  Each mutation was applied to a COPY of the translated .cpp,
//  compiled off-tree with the flags CMake records for target ht9045_sm
//  (-std=c++1z -DCSYSTEM_TICK_ORACLE -I<tree> -I<tree>/SECSGEM), `ar r`'d into a
//  COPY of libht9045_sm.a, and linked into a private exe.  The tables quote the
//  ASSERTION MESSAGE and never a `(line N)` suffix -- the message is stable, the
//  line number is pushed by every edit to this banner.
//
//  BASELINE (both .cpp pristine):  RESULT: 112 passed, 0 failed  (exit 0)
//  Both translated sources were sha256-verified UNCHANGED after the whole run.
//
//  --- [1] Init* ------------------------------------------------------------
//  (I0) InitLoadNewICTrayTask sets 2 instead of 1
//       -> 105/6   FAIL: InitLoadNewICTrayTask() -> iLoadNewICTrayTask==1  (+5 knock-ons in [4])
//       The other 11 Init* checks are the identical one-line shape and were NOT
//       individually mutated; stated rather than implied.
//
//  --- [2] fixture seams -----------------------------------------------------
//  NOT MUTATION-PROVEN, and deliberately so: [2] observes TQPF_Timer / TMySensor,
//  which are NOT this agent's files.  What makes it non-vacuous is that each
//  check asserts BOTH directions of the same object in the same breath (armed ->
//  not elapsed, then elapsed; unknown -> neither, off -> IsOff only, on -> IsOn
//  only), so no single broken behaviour can satisfy all of them.
//
//  --- [3] DoTrayZLoadTrayToWait --------------------------------------------
//  (A1) case 55 CylinderUp -> CylinderLower        -> 109/2  up@100 + mid@100
//  (A2) case 300 Pop -> Push                       -> 110/1  selector retracted
//  (A3) case 400 iIsCoverTray write deleted        -> 109/2  cover==HAS_IC (+[4])
//  (A4) case 400 sTrayID clear deleted             -> 110/1  sTrayID==""
//  (P1) case 200 -> 300 hand-off retargeted        ->  99/12 converged + EXACT trajectory
//  (Q10) ALL FOUR `MOT[MMTrayZ].fHasTray=true` sites deleted
//                                                  -> 108/4  case 400 latched a tray onto MMTrayZ
//  (Q11) the case-400 site deleted ALONE           -> 112/0  GREEN, and that is
//        CORRECT, not a hole: case 55 already set the same flag on this path, so
//        the case-400 write is genuinely redundant here.  Recorded rather than
//        papered over -- the pair is load-bearing, neither member alone is.
//
//  --- [4] DoLoadNewICTray ---------------------------------------------------
//  (B1) case 500 MMTrayZ ClearTray deleted         -> 110/1
//  (B2) case 500 iIsCoverTray copy deleted         -> 110/1
//  (B3) case 300 CylinderLower -> CylinderUp       -> 110/1  both-lowered claim
//  (B4) case 100 stops pumping the nested SM       -> 107/4  incl. the 7-tick dwell
//
//  --- [5] DoSupplyNewICTray -------------------------------------------------
//  (C1) case 1 air-clean On -> Off                 -> 110/1  air@50
//  (C2) case 1100 air-clean Off -> On              -> 110/1  air end
//  (C3) case 1000 fixer Push -> Pop                -> 110/1  (cursor walk IDENTICAL --
//       offline both return true, so only bCylinderOn sees this)
//  (C4) case 1050 up-press Push -> Pop             -> 110/1  (same shape)
//  (C5) case 1100 edge-push On -> Off              -> 110/1
//  (C6) case 1300 MMTrayY SetTray deleted          -> 109/2  (+[6b])
//  (C7) case 1300 MMTrayY_Car ClearTray deleted    -> 109/2  (+[6b])
//  (C8) case 75 anti-lurch bound 2 -> 1            -> 110/1  EXACT trajectory
//  (Q1) case 1200 -> 1300 hand-off retargeted      -> 102/10 converged + trajectory
//
//  --- [6] DoLoad ------------------------------------------------------------
//  (D1) case 1 stage ClearTray deleted             -> 110/1
//  (P2) case 1 DUMMY hand-off 300 -> 999           -> 103/8
//  (P3) case 600 -> 1000 hand-off retargeted       -> 104/7
//  (D2) TrayID rotation deleted                    -> 110/1
//  (D3) iLoadTrayCount++ deleted                   -> 110/1
//  (Q8) ASE_InTrayNum++ deleted                    -> 111/1
//  (D4) bMustCoverIDTray clear deleted             -> 110/1
//
//  --- [7] DoLoaderTrayToFront ----------------------------------------------
//  (E1) explicit `break;` added after case 1's Task=100
//                                                  -> 109/2  fall-through pin + trajectory
//  (P4b) case 200 MMTrayY_Car SetTray deleted      -> 110/1
//  (E2) case 200 MMTrayY ClearTray deleted         -> 110/1
//  (P4) CONTROL, expected green: the case-100 DUMMY-timeout arm disabled
//                                                  -> 112/0  GREEN, which PROVES the
//       driven exit is the Loader-car SENSOR arm (golden :263-268) and not the
//       5 s dummy-timeout arm (golden :271-289).  A control that stays green is
//       evidence about WHICH path runs; it is listed for that reason.
//
//  --- [8] DoLoaderTrayToRear ------------------------------------------------
//  (F1) explicit `break;` added after case 1's Task=100
//                                                  -> 109/2  fall-through pin + trajectory
//  (F2) case 100 fixer Pop -> Push                 -> 110/1  fixer@450
//  (F3) case 500 fixer Push -> Pop                 -> 110/1  fixer end
//  (Q9) case 500 MMTrayY SetTray deleted           -> 111/1
//  (F4) case 500 MMTrayY_Car ClearTray deleted     -> 110/1
//
//  --- [9] DoUnLoadNewLoaderToStack -----------------------------------------
//  (Q2) case 50 CylinderMiddle -> CylinderLower    -> 111/1  mid@100
//  (G1) case 50 CylinderMiddle -> CylinderUp       -> 110/1  "Middle did NOT raise Up"
//  (G2) case 200 CylinderUp -> CylinderMiddle      -> 110/1  up@300
//  (G3) case 400 CylinderLower -> CylinderUp       -> 110/1  both-lowered
//  (G4) case 300 MMTrayZ ClearTray deleted         -> 110/1
//  (P5) case 150 -> 200 hand-off retargeted        -> 103/8  converged + trajectory (+[12])
//  (G5)  case 1's MMTrayZ SetTray deleted ALONE    -> 111/0  GREEN -- masked by the
//        case-50 twin, exactly like Q11 above.
//  (G5b) BOTH MMTrayZ SetTray sites deleted        -> 110/1  z@100
//
//  --- [10] DoLoadNewLoaderTrayToCar ----------------------------------------
//  (H1) case 50 CylinderUp -> CylinderMiddle       -> 110/1  "both rose"
//  (H2) case 410 CylinderLower -> CylinderUp       -> 110/1
//  (Q3) case 200 split-pause guard forced          -> 106/6  converged + trajectory
//  (P7) case 1 fast-path target 400 -> 410         -> 110/1  [10b] EXACT trajectory
//  (H3) case 420 MMTrayZ ClearTray deleted         -> 110/1  ([10b] only -- [10a]
//       starts with an empty stage, which is WHY [10b] exists)
//  (P6) case 420 MMTrayY_Car SetTray deleted       -> 108/3  ([10a] + [10b] + [11])
//
//  --- [11] DoAutoLoader / [12] DoAutoLoaderReceive -------------------------
//  (I1) fLoaderCanSupplyNewTray gate removed       -> 110/1
//  (Q4) receive-divert no longer RETURNs           -> 111/1  cursor-untouched claim
//  (P8) receive-divert no longer CALLS Receive     -> 110/1  divert claim
//  (I2) case 100 rear hand-off 300 -> 999          -> 110/1  EXACT trajectory
//  (I3) Receive case 200 iReceiveLoaderTray=0 del. -> 109/2  cycle-end + trajectory
//  (P9) Receive case 200 car ClearTray deleted     -> 110/1
//
//  --- [13] DoCassetteLoadNewICTray -----------------------------------------
//  (J1) layer counter increment deleted            -> 110/1
//  (J2) bBoatChangeCasset freeze guard removed     -> 110/1
//  (J3) case 200 car SetTray deleted               -> 110/1
//  (P10) case 110 -> 120 hand-off retargeted       -> 110/1  EXACT trajectory
//  (Q5) case 120 -> 200 hand-off retargeted        -> 109/3  converged + trajectory
//
//  --- [14] DoKnockBeforeRemainICCheck --------------------------------------
//  (P11) bIsFirst no longer resets the cursor      -> 108/3
//  (K1) case 1 vibration On -> Off                 -> 110/1  vib@1065
//  (K2) case 1065 vibration Off deleted            -> 110/1  vib end
//  (P12) case 1070 -> 9999 hand-off retargeted     -> 110/1  EXACT trajectory
//  (Q6) case 9999 no longer returns true           -> 111/1  convergence
//
//  --- [15] DoLoaderTrackDetectICFloating -----------------------------------
//  (L1) case 200 fixer Pop -> Push                 -> 110/1  fixer@300
//  (L2) case 400 hand-off 500 -> 600               -> 108/3  no-converge + park-at-500
//                                                            + EXACT trajectory
//  (P13) case 500 JAM0901 guard forced true        -> 110/2  the no-alarm claim AND
//        [18]'s register self-check both go red -- which is the point of [18].
//
//  --- [16] CheckLoaderICFloating -------------------------------------------
//  (P14) the clean-pass `return true` -> false     -> 108/3
//  (M1) latch clear deleted                        -> 110/1  iDir==0 clears
//  (M2) the iDir 0/1 recheck SENSORS swapped       -> 109/2  BOTH direction claims
//
//  --- [17] CheckContinManualRemoveTray -------------------------------------
//  (Q7) P24 gate forced open                       -> 111/1  gate-closed claim
//  (N1) counter decrement deleted                  -> 110/1
//  (N2) generic destination 0 -> 1                 -> 110/1
//  (N3) special destination 1 -> 0                 -> 110/1
//
//  --- [18] register self-check ---------------------------------------------
//  (P13) above.  (Q3) additionally reddened the ShowMyMessage half.
// =============================================================================
#include "asendic_Loader.h"
#include "asendic_Loader_RT.h"
#include "csystem.h"
#include "asendic.h"
#include "Motor/mymotor.h"
#include "mycylin.h"
#include "mysensor.h"
#include "myswitch.h"
#include "myTimer.h"            // TQPF_Timer (the virtual-time seam below)
#include "cprod.h"
#include "cpublic.h"
#include "cmydef.h"
#include "canary_support.h"     // LastSet, ShowErrorMessage seam, ShowMyMessage seam
#include "FormsFacade.h"        // fMain / fAGV / fTrayForm
#include "acatchtray_shims.h"   // fTrayMapping
#include <cstdio>
#include <vector>

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as tests/test_w7_l1_auto2.cpp)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---------------------------------------------------------------------------
//  VIRTUAL-TIME SEAM -- the file-scope TQPF_Timer objects of the two SM files.
//  PROGRESS timers (re-armed to 0 ms every tick by advanceTime):
// ---------------------------------------------------------------------------
extern TQPF_Timer StackDelay;                       // golden asendic_Loader.cpp:42
extern TQPF_Timer htLoaderTimeDelay;                // golden asendic_Loader.cpp:43
extern TQPF_Timer DoSupplyNewICTrayDelay;           // golden asendic_Loader.cpp:44
extern TQPF_Timer htDetectICFloatingDelay;          // golden asendic_Loader.cpp:46
extern TQPF_Timer DoAutoLoaderDelay;                // golden asendic_Loader_RT.cpp:470
extern TQPF_Timer CheckLoaderTrayDelay;             // golden asendic_Loader_RT.cpp:471
extern TQPF_Timer DoUnLoadNewLoaderToStackDelay;    // golden asendic_Loader_RT.cpp:680
extern TQPF_Timer hLoaderTrayToFrontForDummy;       // golden asendic_Loader_RT.cpp:170
extern TQPF_Timer hLoaderTrayToRearForDummy;        // golden asendic_Loader_RT.cpp:338
//  DUAL-ROLE -- pinned only at the cursor where the PROGRESS role is live:
extern TQPF_Timer hLoaderTrayToFront;               // golden asendic_Loader_RT.cpp:169
extern TQPF_Timer hLoaderTrayToRear;                // golden asendic_Loader_RT.cpp:337
extern TQPF_Timer LoaderDelay;                      // golden asendic_Loader_RT.cpp:31
//  PURE JAM WATCHDOGS -- declared for documentation, DELIBERATELY NEVER ADVANCED:
//    extern TQPF_Timer DetectLoaderTime;      // asendic_Loader.cpp:41 -- JAM0901 (20 s)
//    extern TQPF_Timer htCylinLowerDelay;     // asendic_Loader.cpp:45 -- JAM0913 (2 s)
//    extern TQPF_Timer htDetectTrayRetryDelay;// asendic_Loader.cpp:47 -- JAM0912/JAM0901 retry
//    extern TQPF_Timer LoadReadTrayID;        // asendic_Loader.cpp:48 -- WAR16122/WAR16336
//    extern TQPF_Timer CCDLoadTrayMap;        // asendic_Loader.cpp:49 -- WAR09103
//    extern TQPF_Timer htLoadHasTrayDelay;    // asendic_Loader.cpp:50 -- 3 s / 360 s AMR waits
//    extern TQPF_Timer htRTDetectTrayRetryDelay; // asendic_Loader_RT.cpp:171

static void advanceTime()
{
    StackDelay.SetMSAndOn(0);
    htLoaderTimeDelay.SetMSAndOn(0);
    DoSupplyNewICTrayDelay.SetMSAndOn(0);
    htDetectICFloatingDelay.SetMSAndOn(0);
    DoAutoLoaderDelay.SetMSAndOn(0);
    CheckLoaderTrayDelay.SetMSAndOn(0);
    DoUnLoadNewLoaderToStackDelay.SetMSAndOn(0);
    hLoaderTrayToFrontForDummy.SetMSAndOn(0);
    hLoaderTrayToRearForDummy.SetMSAndOn(0);
}
// Dual-role pins -- each fires only where the timer's PROGRESS role is live.
static void advanceFrontSettle() { if (iLoaderTrayToFrontTask      == 200) hLoaderTrayToFront.SetMSAndOn(0); }
static void advanceRearSettle()  { if (iLoaderTrayToRearTask       == 460) hLoaderTrayToRear.SetMSAndOn(0); }
static void advanceRTCarSettle() { if (iLoadNewLoaderTrayToCarTask == 400) LoaderDelay.SetMSAndOn(0); }

// ---------------------------------------------------------------------------
//  SIM SENSOR SEAM -- see banner note 2.
// ---------------------------------------------------------------------------
static void simSensorOff(int idx)       // IsOff()==true, IsOn()==false
{
    Sen[idx].Enable  = true;
    Sen[idx].Type    = TYPE_A;          // cmydef.cpp TYPE_A==1
    Sen[idx].ISABase = eISABase;        // -> mysensor.cpp raw-port stub (false)
}
static void simSensorOn(int idx)        // IsOn()==true, IsOff()==false
{
    Sen[idx].Enable  = true;
    Sen[idx].Type    = TYPE_B;          // cmydef.cpp TYPE_B==0 (the default type)
    Sen[idx].ISABase = eISABase;
}
static void simSensorUnknown(int idx)   // IsOn()==false AND IsOff()==false
{
    Sen[idx].Enable = false;
}

// ---------------------------------------------------------------------------
//  Ordered trajectory recorder.  Records the cursor after every tick and
//  collapses consecutive duplicates, so the result is the ORDERED sequence of
//  distinct states the SM passed through -- not a set.
// ---------------------------------------------------------------------------
typedef std::vector<int> Traj;

static void trajPush(Traj &t, int cursor)
{
    if (t.empty() || t.back() != cursor) t.push_back(cursor);
}
static bool trajEq(const Traj &t, const int *want, int nWant)
{
    if ((int)t.size() != nWant) return false;
    for (int i = 0; i < nWant; ++i) if (t[i] != want[i]) return false;
    return true;
}
static void trajPrint(const char *what, const Traj &t, const int *want, int nWant)
{
    printf("    (%s got:", what);
    for (size_t i = 0; i < t.size(); ++i) printf(" %d", t[i]);
    printf(" | want:");
    for (int i = 0; i < nWant; ++i) printf(" %d", want[i]);
    printf(")\n");
}

// ---------------------------------------------------------------------------
//  Fixture.  The DUMMY tray-feed configuration both SMs expect, plus the
//  actuator ENABLE flags without which the golden `if(Cylinder[X].Enable)` /
//  `if(SW[X].Enable)` guards skip the very calls this test observes.
//
//  MOT[MTrayX] IS DRIVEN TO 5000 ON PURPOSE.  DoSupplyNewICTray case 105 has an
//  eAboveCoveyor guard `MOT[MTrayX].ReadPos() < Prod.iXTrayLoad+1000 -> break`
//  (golden :382-387).  Prod.iXTrayLoad defaults to 0 and the motor's offline
//  position defaults to 0, so WITHOUT this the SM parks on 105 forever -- which
//  is exactly what the first exploratory run of this fixture did.  Recorded
//  because a future reader will otherwise "tidy" this line away.
// ---------------------------------------------------------------------------
//  ALARM ACCUMULATOR.  setupLoaderFixture() resets the ShowErrorMessage seam at
//  the start of every sub-test (so a sub-test that WANTS to reason about alarms
//  starts from a clean slate).  That would make the [18] register self-check
//  measure only the LAST sub-test, which is exactly the kind of quietly-vacuous
//  assertion this file exists to avoid -- so the count is banked here before each
//  reset and [18] asserts bank + current.
static int g_alarmsBanked = 0, g_dialogsBanked = 0;

static void setupLoaderFixture()
{
    g_alarmsBanked  += W906_ShowErrorMessage_Count;
    g_dialogsBanked += W906_ShowMyMessage_Count;

    LastSet.iRealDummy = DUMMY;

    MOT[MTrayX].MotorMove(5000);            // clear the case-105 tray-arm guard

    // Lifter pair driven by CylinderUp/Middle/Lower (asendic.cpp gates on Enable)
    Cylinder[C_Load_Up].Enable        = true;
    Cylinder[C_Load_Middle].Enable    = true;
    // Golden call sites guarded by `if(Cylinder[X].Enable)`
    Cylinder[C_TrayZ_Selector].Enable = true;
    Cylinder[C_TrayY_Fixer].Enable    = true;
    Cylinder[C_LoaderEdgePush].Enable = true;
    Cylinder[C_LoaderUpPress].Enable  = true;
    Cylinder[C_LoaderCarrier].Enable  = true;
    // Golden call sites guarded by `if(SW[X].Enable)`
    SW[SwLoaderAirClean].Enable       = true;
    SW[SwLoaderVibration].Enable      = true;

    // Loader sensor bank: "unknown" (an unconfigured offline IO table).  Golden
    // reads these as `IsOff()==false`, which "unknown" satisfies -- that is the
    // real machine's "sensor says nothing yet" reading, not "sensor says off".
    simSensorUnknown(SnLoaderSureTray);
    simSensorUnknown(SnLoaderPreDete);
    simSensorUnknown(SnLoaderCarHasTray);
    simSensorUnknown(SnLoaderTrayHasTray);
    simSensorUnknown(SnLoaderTrayHasTray_ART);
    simSensorUnknown(SenLoaderCCWDete);
    simSensorUnknown(SnLoaderTrackDetect);
    simSensorUnknown(SnDoubleLoadDetection);
    simSensorUnknown(SnLoaderIsFull);
    simSensorUnknown(SnLoaderBoatActDetect);
    simSensorUnknown(SnLoaderFixCyPush);
    simSensorUnknown(SnLoaderSeparateHasTray);
    simSensorUnknown(SnCheckTrayDirection);
    simSensorUnknown(SnCheckLoadDirection);

    bLoaderTrayICDetectErr = false;
    bHandlerPause          = false;
    bLoaderPause           = false;
    iReceiveLoaderTray     = 0;
    fLoaderCanSupplyNewTray = true;
    bOneTimeHotPlateCheckAll = false;
    bResetLoadTray         = false;
    bLoadContractModeTest  = false;
    bBackupCleanOut        = false;
    bDoIniStartAutoIonFanClean = false;
    bAskStopPort[ePortLoader]  = false;

    W906_ShowErrorMessage_Reset();
    W906_ShowMyMessage_Reset();
}

// ---------------------------------------------------------------------------
//  [1] Init* reset their own cursor to 1 -- all 12 (6 in each file).
// ---------------------------------------------------------------------------
static void test_inits()
{
    printf("\n[1] Init* reset task cursors to 1 (12 entry points, 2 files)\n");

    LoadTask = 999;                        InitLoadTask();
    CHECK(LoadTask == 1, "InitLoadTask() -> LoadTask==1");

    iLoadNewICTrayTask = 999;              InitLoadNewICTrayTask();
    CHECK(iLoadNewICTrayTask == 1, "InitLoadNewICTrayTask() -> iLoadNewICTrayTask==1");

    iTrayZLoadTrayToWaitTask = 999;        InitTrayZLoadTrayToWaitTask();
    CHECK(iTrayZLoadTrayToWaitTask == 1, "InitTrayZLoadTrayToWaitTask() -> iTrayZLoadTrayToWaitTask==1");

    iSupplyNewIC_From_LoaderCar = 999;     InitSupplyNewIC_From_LoaderCarTask();
    CHECK(iSupplyNewIC_From_LoaderCar == 1, "InitSupplyNewIC_From_LoaderCarTask() -> iSupplyNewIC_From_LoaderCar==1");

    iCassetteLoadNewICTrayTask = 999;      InitCassetteLoadNewICTrayTask();
    CHECK(iCassetteLoadNewICTrayTask == 1, "InitCassetteLoadNewICTrayTask() -> iCassetteLoadNewICTrayTask==1");

    iLoaderTrackDetectICFloatingTask = 999; InitLoaderTrackDetectICFloatingTask();
    CHECK(iLoaderTrackDetectICFloatingTask == 1, "InitLoaderTrackDetectICFloatingTask() -> cursor==1");

    iAutoLoaderTask = 999;                 InitAutoLoaderTask();
    CHECK(iAutoLoaderTask == 1, "InitAutoLoaderTask() -> iAutoLoaderTask==1");

    iAutoLoaderReceiveTask = 999;          InitAutoLoaderReceiveTask();
    CHECK(iAutoLoaderReceiveTask == 1, "InitAutoLoaderReceiveTask() -> iAutoLoaderReceiveTask==1");

    iUnLoadNewLoaderTrayTask = 999;        InitUnLoadNewLoaderTrayTask();
    CHECK(iUnLoadNewLoaderTrayTask == 1, "InitUnLoadNewLoaderTrayTask() -> iUnLoadNewLoaderTrayTask==1");

    iLoaderTrayToFrontTask = 999;          InitLoaderTrayToFrontTask();
    CHECK(iLoaderTrayToFrontTask == 1, "InitLoaderTrayToFrontTask() -> iLoaderTrayToFrontTask==1");

    iLoaderTrayToRearTask = 999;           InitLoaderTrayToRearTask();
    CHECK(iLoaderTrayToRearTask == 1, "InitLoaderTrayToRearTask() -> iLoaderTrayToRearTask==1");

    iLoadNewLoaderTrayToCarTask = 999;     InitLoadNewLoaderTrayToCarTask();
    CHECK(iLoadNewLoaderTrayToCarTask == 1, "InitLoadNewLoaderTrayToCarTask() -> iLoadNewLoaderTrayToCarTask==1");

    // The DEFINITION this bundle owns (golden asendic_Loader.cpp:51) carries a
    // NON-ZERO initial value.  Asserting it here pins the one property the
    // Wave-0 note in asendic.h singles out.
    CHECK(iTrayZLoadTrayToWaitTask == 1,
          "iTrayZLoadTrayToWaitTask is the idle cursor 1, not 0 (golden asendic_Loader.cpp:51 `=1`)");
}

// ---------------------------------------------------------------------------
//  [2] The two fixture seams themselves, in BOTH directions.  If these are
//      vacuous every trajectory below is vacuous.
// ---------------------------------------------------------------------------
static void test_seams()
{
    printf("\n[2] Fixture seams: virtual time + tri-state sensors\n");

    StackDelay.SetMSAndOn(1000);
    CHECK(StackDelay.Off() == false, "a freshly armed 1000 ms progress timer is NOT elapsed");
    advanceTime();
    CHECK(StackDelay.Off() == true,  "after advanceTime() the same timer IS elapsed");

    simSensorUnknown(SnLoaderSureTray);
    CHECK(Sen[SnLoaderSureTray].IsOn() == false && Sen[SnLoaderSureTray].IsOff() == false,
          "Enable==false sensor reads NEITHER On nor Off ('unknown' -- not the same as off)");
    simSensorOff(SnLoaderSureTray);
    CHECK(Sen[SnLoaderSureTray].IsOff() == true && Sen[SnLoaderSureTray].IsOn() == false,
          "Enable+TYPE_A sensor reads Off (IsOff()==true, IsOn()==false)");
    simSensorOn(SnLoaderSureTray);
    CHECK(Sen[SnLoaderSureTray].IsOn() == true && Sen[SnLoaderSureTray].IsOff() == false,
          "Enable+TYPE_B sensor reads On (IsOn()==true, IsOff()==false)");
    simSensorUnknown(SnLoaderSureTray);
}

// ---------------------------------------------------------------------------
//  [3] DoTrayZLoadTrayToWait -- the Loader stack lift (golden :1474-1874).
//      DUMMY path: 50 -> 55 -> 100 -> 200 -> 300 -> 400 -> converge.
//
//      Actuator claims (each pre-driven to the opposite first):
//        golden :1806/:1700  case 55  CylinderUp(C_Load_Up)  -> BOTH lifter
//                            cylinders ON (asendic.cpp drives the Up id and its
//                            paired Middle id).  Sampled at cursor 100.
//        golden :1782        case 300 Cylinder[C_TrayZ_Selector].Pop() -> the
//                            selector ends RETRACTED.  Pre-driven ON, and this
//                            is the assertion Push/Pop cannot fake: offline both
//                            return true, so only bCylinderOn sees the swap.
//        golden :1800        case 400 MOT[MMTrayZ].iIsCoverTray = iLoadIsCoverTray
//                            where iLoadIsCoverTray was set to HAS_IC at golden
//                            :1715.  Pre-set to IS_ID_TRAY so the write shows.
// ---------------------------------------------------------------------------
static void test_trayz_load_tray_to_wait()
{
    printf("\n[3] DoTrayZLoadTrayToWait() DUMMY lift walk 50,55,100,200,300,400\n");

    setupLoaderFixture();
    InitTrayZLoadTrayToWaitTask();
    MOT[MMTrayZ].ClearTray("test_w7_l1_loader");
    MOT[MMTrayZ].iIsCoverTray = IS_ID_TRAY;         // opposite of the expected HAS_IC
    MOT[MMTrayZ].sTrayID      = "STALE";            // golden :1797 writes "" on this path
    Cylinder[C_Load_Up].Off();                      // opposite of expected ON
    Cylinder[C_Load_Middle].Off();                  // opposite of expected ON
    Cylinder[C_TrayZ_Selector].On();                // opposite of expected retracted

    Traj traj;
    bool done  = false;
    int  steps = 0;
    int  upAt100 = -1, midAt100 = -1;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceTime();
        done = DoTrayZLoadTrayToWait();
        trajPush(traj, iTrayZLoadTrayToWaitTask);
        if (iTrayZLoadTrayToWaitTask == 100 && upAt100 < 0)
        {
            upAt100  = Cylinder[C_Load_Up].bCylinderOn     ? 1 : 0;
            midAt100 = Cylinder[C_Load_Middle].bCylinderOn ? 1 : 0;
        }
    }

    static const int want[] = {50, 55, 100, 200, 300, 400, 1};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(done, "DoTrayZLoadTrayToWait() returns true (converged, not bound-exhausted)");
    CHECK(trajEq(traj, want, nWant), "EXACT ordered trajectory 50,55,100,200,300,400,1");
    CHECK(upAt100 == 1,  "case 55 CylinderUp raised C_Load_Up (golden :1700/:1806)");
    CHECK(midAt100 == 1, "case 55 CylinderUp also raised the paired C_Load_Middle");
    CHECK(Cylinder[C_TrayZ_Selector].bCylinderOn == false,
          "case 300 RETRACTED the TrayZ selector (golden :1782 Pop(), not Push())");
    CHECK(MOT[MMTrayZ].fHasTray == true, "case 400 latched a tray onto MMTrayZ (golden :1790)");
    CHECK(MOT[MMTrayZ].iIsCoverTray == HAS_IC,
          "case 400 wrote iLoadIsCoverTray==HAS_IC onto MMTrayZ (golden :1715 + :1800)");
    CHECK(MOT[MMTrayZ].sTrayID == "",
          "case 400 cleared MMTrayZ.sTrayID on the non-cover path (golden :1797)");
    trajPrint("DoTrayZLoadTrayToWait", traj, want, nWant);
    printf("    (steps=%d up@100=%d mid@100=%d)\n", steps, upAt100, midAt100);
}

// ---------------------------------------------------------------------------
//  [4] DoLoadNewICTray -- the wrapper that PUMPS DoTrayZLoadTrayToWait
//      (golden :2022-2428).  Trajectory 100 -> 300 -> 400 -> 500 -> converge.
//
//      THE 7-TICK DWELL ON 100 IS THE POINT.  Golden case 100 is
//      `if(DoTrayZLoadTrayToWait()) Task=300;` -- the nested SM must be pumped
//      once per tick, and its own walk is 7 ticks long (sub-test [3]).  Asserting
//      the dwell EXACTLY proves the nesting is real rather than short-circuited.
// ---------------------------------------------------------------------------
static void test_load_new_ic_tray()
{
    printf("\n[4] DoLoadNewICTray() wrapper walk 100(x7),300,400,500\n");

    setupLoaderFixture();
    InitLoadNewICTrayTask();
    InitTrayZLoadTrayToWaitTask();
    MOT[MMTrayZ].ClearTray("test_w7_l1_loader");
    MOT[MMTrayZ].iIsCoverTray = IS_ID_TRAY;
    MOT[MMTrayY].ClearTray("test_w7_l1_loader");
    MOT[MMTrayY_Car].ClearTray("test_w7_l1_loader");
    MOT[MMTrayY_Car].iIsCoverTray = NULL_IC;        // opposite of the HAS_IC it must receive
    Cylinder[C_Load_Up].Off();
    Cylinder[C_Load_Middle].Off();

    Traj traj;
    bool done  = false;
    int  steps = 0, ticksAt100 = 0;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceTime();
        done = DoLoadNewICTray();
        trajPush(traj, iLoadNewICTrayTask);
        if (iLoadNewICTrayTask == 100) ++ticksAt100;
    }

    static const int want[] = {100, 300, 400, 500, 1};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(done, "DoLoadNewICTray() returns true (converged, not bound-exhausted)");
    CHECK(trajEq(traj, want, nWant), "EXACT ordered trajectory 100,300,400,500,1");
    CHECK(ticksAt100 == 7,
          "case 100 pumped the NESTED DoTrayZLoadTrayToWait for its full 7-tick walk (golden :2141)");
    CHECK(MOT[MMTrayY_Car].fHasTray == true,
          "case 500 latched the tray onto MMTrayY_Car (golden :2317)");
    CHECK(MOT[MMTrayY_Car].iIsCoverTray == HAS_IC,
          "case 500 copied MMTrayZ.iIsCoverTray onto MMTrayY_Car (golden :2300)");
    CHECK(MOT[MMTrayZ].fHasTray == false,
          "case 500 cleared the MMTrayZ stage flag (golden :2320)");
    CHECK(Cylinder[C_Load_Up].bCylinderOn == false && Cylinder[C_Load_Middle].bCylinderOn == false,
          "case 300 CylinderLower(C_Load_Up) LOWERED both lifter cylinders (golden :2145)");
    trajPrint("DoLoadNewICTray", traj, want, nWant);
    printf("    (steps=%d ticksAt100=%d)\n", steps, ticksAt100);
}

// ---------------------------------------------------------------------------
//  [5] DoSupplyNewICTray -- the full Loader-car -> track feed (golden :155-1465).
//      With C_LoaderUpPress and C_LoaderEdgePush ENABLED the golden walk is
//      50,70,75,70,75,100,105,150,300,350,360,370,400,1000,1050,1100,1200,1300.
//      The 70/75 PAIR REPEATS TWICE ON PURPOSE: that is golden's
//      iLoaderUpPressCount<2 anti-lurch pre-actuation loop (golden :319-338), and
//      asserting the repeat is what pins the loop bound.
//
//      Actuator claims, each pre-driven to the opposite:
//        golden :221  case 1    SW[SwLoaderAirClean].On()   -> sampled at cursor 50
//        golden :816  case 1100 SW[SwLoaderAirClean].Off()  -> sampled at the end
//        golden :729  case 1000 Cylinder[C_TrayY_Fixer].Push()   -> ends EXTENDED
//        golden :746  case 1050 Cylinder[C_LoaderUpPress].Push() -> ends EXTENDED
//        golden :801  case 1100 Cylinder[C_LoaderEdgePush].On()  -> ends EXTENDED
//        golden :1130 case 1300 MOT[MMTrayY].SetTray(HAS_IC)
//        golden :1237 case 1300 MOT[MMTrayY_Car].ClearTray()
// ---------------------------------------------------------------------------
static void test_supply_new_ic_tray()
{
    printf("\n[5] DoSupplyNewICTray() full feed walk (with the 70/75 anti-lurch pair)\n");

    setupLoaderFixture();
    InitSupplyNewIC_From_LoaderCarTask();
    MOT[MMTrayY_Car].SetTray(HAS_IC, "test_w7_l1_loader");
    MOT[MMTrayY].ClearTray("test_w7_l1_loader");
    SW[SwLoaderAirClean].Off();             // opposite of the ON case 1 must write
    Cylinder[C_TrayY_Fixer].Off();          // opposite of the Push case 1000 must do
    Cylinder[C_LoaderUpPress].Off();        // opposite of the Push case 1050 must do
    Cylinder[C_LoaderEdgePush].Off();       // opposite of the On   case 1100 must do

    Traj traj;
    bool done  = false;
    int  steps = 0;
    int  airAt50 = -1;
    for (steps = 0; steps < 400 && !done; ++steps)
    {
        advanceTime();
        done = DoSupplyNewICTray();
        trajPush(traj, iSupplyNewIC_From_LoaderCar);
        if (iSupplyNewIC_From_LoaderCar == 50 && airAt50 < 0)
            airAt50 = SW[SwLoaderAirClean].OutValue ? 1 : 0;
    }

    static const int want[] = {50, 70, 75, 70, 75, 100, 105, 150, 300, 350,
                               360, 370, 400, 1000, 1050, 1100, 1200, 1300, 1};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(done, "DoSupplyNewICTray() returns true (converged, not bound-exhausted)");
    CHECK(trajEq(traj, want, nWant),
          "EXACT ordered trajectory 50,70,75,70,75,100,105,150,300,350,360,370,400,1000,1050,1100,1200,1300,1");
    CHECK(airAt50 == 1,
          "case 1 turned the Loader air-clean output ON (golden :221 On(), not Off())");
    CHECK(SW[SwLoaderAirClean].OutValue == false,
          "case 1100 turned the Loader air-clean output back OFF (golden :816)");
    CHECK(Cylinder[C_TrayY_Fixer].bCylinderOn == true,
          "case 1000 EXTENDED the tray-Y fixer (golden :729 Push(), not Pop())");
    CHECK(Cylinder[C_LoaderUpPress].bCylinderOn == true,
          "case 1050 EXTENDED the Loader up-press (golden :746 Push(), not Pop())");
    CHECK(Cylinder[C_LoaderEdgePush].bCylinderOn == true,
          "case 1100 EXTENDED the Loader edge-push (golden :801 On(), not Off())");
    CHECK(MOT[MMTrayY].fHasTray == true,
          "case 1300 latched the tray onto the MMTrayY track (golden :1130)");
    CHECK(MOT[MMTrayY_Car].fHasTray == false,
          "case 1300 cleared the MMTrayY_Car flag (golden :1237)");
    trajPrint("DoSupplyNewICTray", traj, want, nWant);
    printf("    (steps=%d air@50=%d)\n", steps, airAt50);
}

// ---------------------------------------------------------------------------
//  [6a] DoLoad case 1 -- the Magazine-stage hand-off (golden :2485-2489).
//       If MMTrayZ carries a tray when DoLoad is first ticked, that tray is
//       moved onto MMTrayY_Car and the stage flag is cleared, IN ONE TICK, and
//       the cursor lands on 300.
// ---------------------------------------------------------------------------
static void test_do_load_case1_handoff()
{
    printf("\n[6a] DoLoad() case 1 Magazine-stage hand-off (one tick)\n");

    setupLoaderFixture();
    InitLoadTask();
    MOT[MMTrayZ].SetTray(HAS_IC, "test_w7_l1_loader");
    MOT[MMTrayY_Car].ClearTray("test_w7_l1_loader");
    MOT[MMTrayY].ClearTray("test_w7_l1_loader");

    advanceTime();
    DoLoad();

    CHECK(MOT[MMTrayY_Car].fHasTray == true,
          "case 1 moved the Magazine-stage tray onto MMTrayY_Car (golden :2487)");
    CHECK(MOT[MMTrayZ].fHasTray == false,
          "case 1 cleared the Magazine-stage flag (golden :2488)");
    CHECK(LoadTask == 300,
          "case 1 handed to 300 on the DUMMY/no-Loader-sensor path (golden :2491-2494)");
}

// ---------------------------------------------------------------------------
//  [6b] DoLoad end-to-end -- the hub entry point this bundle's landing RETIRES
//       csystem_shims' no-op for.  It drives DoSupplyNewICTray to completion and
//       then runs golden's per-tray bookkeeping block (golden :3024-3048).
//
//       Asserted bookkeeping (each pre-set to the opposite):
//         golden :3033-3034  TrayID[0][1] = TrayID[0][0]; TrayID[0][0] = "";
//         golden :3037       iLoadTrayCount++
//         golden :3036       ASE_InTrayNum++
//         golden :3047       bMustCoverIDTray = false
// ---------------------------------------------------------------------------
static void test_do_load_end_to_end()
{
    printf("\n[6b] DoLoad() end-to-end: pumps DoSupplyNewICTray + per-tray bookkeeping\n");

    setupLoaderFixture();
    InitLoadTask();
    InitSupplyNewIC_From_LoaderCarTask();
    MOT[MMTrayZ].ClearTray("test_w7_l1_loader");
    MOT[MMTrayY].ClearTray("test_w7_l1_loader");
    MOT[MMTrayY_Car].SetTray(HAS_IC, "test_w7_l1_loader");

    TrayID[0][0] = "TRAY-A";
    TrayID[0][1] = "";
    bMustCoverIDTray = true;                        // opposite of the false golden writes
    const int loadCnt0 = iLoadTrayCount;
    const int aseCnt0  = ASE_InTrayNum;

    Traj traj;
    bool reached1000 = false, wrapped = false;
    for (int i = 0; i < 200; ++i)
    {
        advanceTime();
        DoLoad();
        trajPush(traj, LoadTask);
        if (LoadTask == 1000) reached1000 = true;
        if (reached1000 && LoadTask == 1) wrapped = true;
        if (wrapped && LoadTask == 800) break;      // the idle 600<->800 oscillation
    }

    CHECK(reached1000, "DoLoad reached case 1000 (the DoSupplyNewICTray limb, golden :3024)");
    CHECK(wrapped,     "DoLoad case 1000 consumed DoSupplyNewICTray()==true and reset to 1 (golden :3048)");
    CHECK(MOT[MMTrayY].fHasTray == true && MOT[MMTrayY_Car].fHasTray == false,
          "the pumped DoSupplyNewICTray really moved the tray car -> track");
    CHECK(TrayID[0][1] == "TRAY-A" && TrayID[0][0] == "",
          "case 1000 rotated the Loader tray ID read->pick (golden :3033-3034)");
    CHECK(iLoadTrayCount == loadCnt0 + 1, "case 1000 incremented iLoadTrayCount by exactly 1 (golden :3037)");
    CHECK(ASE_InTrayNum  == aseCnt0  + 1, "case 1000 incremented ASE_InTrayNum by exactly 1 (golden :3036)");
    CHECK(bMustCoverIDTray == false, "case 1000 cleared bMustCoverIDTray (golden :3047)");
    printf("    (DoLoad visited:");
    for (size_t i = 0; i < traj.size(); ++i) printf(" %d", traj[i]);
    printf(")\n");
}

// ---------------------------------------------------------------------------
//  [7] DoLoaderTrayToFront (RT, golden :177-334).
//      GOLDEN FALL-THROUGH PIN: case 1 has no `break` (golden :207-208), so case
//      1 and case 100 both run in the SAME tick.  With the Loader-car sensor
//      "unknown" (`IsOff()==false` is TRUE) case 100 immediately hands to 200,
//      so after ONE tick the cursor is already 200 -- 100 is never observable at
//      a tick boundary.  Insert a `break;` after golden :207 and the first tick
//      would leave the cursor on 100 instead.
// ---------------------------------------------------------------------------
static void test_loader_tray_to_front()
{
    printf("\n[7] DoLoaderTrayToFront() fall-through pin + arrival settle\n");

    setupLoaderFixture();
    InitLoaderTrayToFrontTask();
    MOT[MMTrayY].SetTray(NULL_IC, "test_w7_l1_loader");     // must end CLEARED
    MOT[MMTrayY_Car].ClearTray("test_w7_l1_loader");        // must end SET

    Traj traj;
    bool done  = false;
    int  steps = 0, afterFirstTick = -1;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceTime();
        advanceFrontSettle();
        done = DoLoaderTrayToFront();
        trajPush(traj, iLoaderTrayToFrontTask);
        if (steps == 0) afterFirstTick = iLoaderTrayToFrontTask;
    }

    static const int want[] = {200, 1};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(afterFirstTick == 200,
          "GOLDEN FALL-THROUGH pin: case 1 fell through into case 100 in ONE tick (cursor 1 -> 200)");
    CHECK(done, "DoLoaderTrayToFront() returns true (converged, not bound-exhausted)");
    CHECK(trajEq(traj, want, nWant), "EXACT ordered trajectory 200,1");
    CHECK(MOT[MMTrayY_Car].fHasTray == true,
          "case 200 latched the recycled tray onto MMTrayY_Car (golden :318 SetTray)");
    CHECK(MOT[MMTrayY].fHasTray == false,
          "case 200 cleared the MMTrayY track flag (golden :319 ClearTray)");
    trajPrint("DoLoaderTrayToFront", traj, want, nWant);
}

// ---------------------------------------------------------------------------
//  [8] DoLoaderTrayToRear (RT, golden :345-467) -- the empty-tray return.
//      GOLDEN FALL-THROUGH PIN: case 1 has no `break` (golden :358-359).
//      Cylinder direction is the real payload here, and it is a Pop-then-Push
//      pair on the SAME cylinder, so a single end-state read cannot cover both:
//        golden :360  case 100 Cylinder[C_TrayY_Fixer].Pop()  -> sampled at 450
//        golden :457  case 500 Cylinder[C_TrayY_Fixer].Push() -> sampled at end
//      The fixture pre-drives the fixer EXTENDED, so the Pop is observable, and
//      the mid-trajectory re-read makes the later Push independently observable.
// ---------------------------------------------------------------------------
static void test_loader_tray_to_rear()
{
    printf("\n[8] DoLoaderTrayToRear() fall-through pin + Pop-then-Push on one cylinder\n");

    setupLoaderFixture();
    InitLoaderTrayToRearTask();
    MOT[MMTrayY].ClearTray("test_w7_l1_loader");            // must end SET (NULL_IC)
    MOT[MMTrayY_Car].SetTray(NULL_IC, "test_w7_l1_loader"); // must end CLEARED
    Cylinder[C_TrayY_Fixer].On();                           // opposite of the case-100 Pop

    Traj traj;
    bool done  = false;
    int  steps = 0, afterFirstTick = -1, fixerAt450 = -1;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceTime();
        advanceRearSettle();
        done = DoLoaderTrayToRear();
        trajPush(traj, iLoaderTrayToRearTask);
        if (steps == 0) afterFirstTick = iLoaderTrayToRearTask;
        if (iLoaderTrayToRearTask == 450 && fixerAt450 < 0)
            fixerAt450 = Cylinder[C_TrayY_Fixer].bCylinderOn ? 1 : 0;
    }

    static const int want[] = {200, 450, 460, 500, 1};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(afterFirstTick == 200,
          "GOLDEN FALL-THROUGH pin: case 1 fell through into case 100 in ONE tick (cursor 1 -> 200)");
    CHECK(done, "DoLoaderTrayToRear() returns true (converged, not bound-exhausted)");
    CHECK(trajEq(traj, want, nWant), "EXACT ordered trajectory 200,450,460,500,1");
    CHECK(fixerAt450 == 0,
          "case 100 RETRACTED the tray-Y fixer before the tray moved in (golden :360 Pop(), not Push())");
    CHECK(Cylinder[C_TrayY_Fixer].bCylinderOn == true,
          "case 500 RE-EXTENDED the tray-Y fixer to clamp the returned tray (golden :457 Push(), not Pop())");
    CHECK(MOT[MMTrayY].fHasTray == true,
          "case 500 latched the empty tray onto MMTrayY (golden :459)");
    CHECK(MOT[MMTrayY_Car].fHasTray == false,
          "case 500 cleared the MMTrayY_Car flag (golden :460)");
    trajPrint("DoLoaderTrayToRear", traj, want, nWant);
    printf("    (fixer@450=%d fixerEnd=%d)\n", fixerAt450, (int)Cylinder[C_TrayY_Fixer].bCylinderOn);
}

// ---------------------------------------------------------------------------
//  [9] DoUnLoadNewLoaderToStack (RT, golden :686-843) -- put a tray back on the
//      Loader stack.  This is the richest lifter sequence in the bundle and it
//      distinguishes all THREE lifter helpers, which the other sub-tests cannot:
//        golden :742  case 50  CylinderMiddle(C_Load_Up) -> ONLY C_Load_Middle
//                     rises.  Sampled at cursor 100: mid ON, up STILL OFF.  That
//                     asymmetry is what separates Middle from Up.
//        golden :761  case 200 CylinderUp(C_Load_Up)     -> C_Load_Up rises too.
//                     Sampled at cursor 300.
//        golden :776  case 400 CylinderLower(C_Load_Up)  -> BOTH fall.  End state.
//      Plus the stack-tray bookkeeping:
//        golden :703/:741 SetTray(NULL_IC) on MMTrayZ  -> sampled at cursor 100
//        golden :769      ClearTray()      on MMTrayZ  -> end state
// ---------------------------------------------------------------------------
static void test_unload_new_loader_to_stack()
{
    printf("\n[9] DoUnLoadNewLoaderToStack() -- Middle vs Up vs Lower, all three distinguished\n");

    setupLoaderFixture();
    InitUnLoadNewLoaderTrayTask();
    MOT[MMTrayZ].ClearTray("test_w7_l1_loader");    // must be SET at cursor 100, CLEARED at end
    Cylinder[C_Load_Up].Off();                      // opposite of the case-200 rise
    Cylinder[C_Load_Middle].Off();                  // opposite of the case-50 rise

    Traj traj;
    bool done  = false;
    int  steps = 0;
    int  upAt100 = -1, midAt100 = -1, zAt100 = -1, upAt300 = -1;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceTime();
        done = DoUnLoadNewLoaderToStack(false);
        trajPush(traj, iUnLoadNewLoaderTrayTask);
        if (iUnLoadNewLoaderTrayTask == 100 && upAt100 < 0)
        {
            upAt100  = Cylinder[C_Load_Up].bCylinderOn     ? 1 : 0;
            midAt100 = Cylinder[C_Load_Middle].bCylinderOn ? 1 : 0;
            zAt100   = MOT[MMTrayZ].fHasTray               ? 1 : 0;
        }
        if (iUnLoadNewLoaderTrayTask == 300 && upAt300 < 0)
            upAt300 = Cylinder[C_Load_Up].bCylinderOn ? 1 : 0;
    }

    static const int want[] = {50, 100, 150, 200, 300, 400, 1};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(done, "DoUnLoadNewLoaderToStack() returns true (converged, not bound-exhausted)");
    CHECK(trajEq(traj, want, nWant), "EXACT ordered trajectory 50,100,150,200,300,400,1");
    CHECK(midAt100 == 1,
          "case 50 CylinderMiddle raised C_Load_Middle (golden :742)");
    CHECK(upAt100 == 0,
          "case 50 CylinderMiddle did NOT raise C_Load_Up -- Middle != Up (golden asendic.cpp CylinderMiddle)");
    CHECK(zAt100 == 1,
          "case 1/50 marked the MMTrayZ stage as carrying the returned tray (golden :703/:741)");
    CHECK(upAt300 == 1,
          "case 200 CylinderUp raised C_Load_Up to the separation height (golden :761)");
    CHECK(Cylinder[C_Load_Up].bCylinderOn == false && Cylinder[C_Load_Middle].bCylinderOn == false,
          "case 400 CylinderLower LOWERED both lifter cylinders (golden :776)");
    CHECK(MOT[MMTrayZ].fHasTray == false,
          "case 300 cleared the MMTrayZ stage flag once the tray was released (golden :769)");
    trajPrint("DoUnLoadNewLoaderToStack", traj, want, nWant);
    printf("    (up@100=%d mid@100=%d z@100=%d up@300=%d)\n", upAt100, midAt100, zAt100, upAt300);
}

// ---------------------------------------------------------------------------
//  [10a] DoLoadNewLoaderTrayToCar (RT, golden :38-166), FULL SPLIT WALK.
//        Split one tray off the Loader stack onto the car.  Contrast with [9]:
//        here case 50 calls CylinderUp (BOTH rise) where [9]'s case 50 called
//        CylinderMiddle (only the Middle rises).  Asserting both pins the two
//        helpers apart -- neither sub-test alone can.
//          golden :87   case 50  CylinderUp(C_Load_Up)    -> sampled at cursor 100
//          golden :131  case 410 CylinderLower(C_Load_Up) -> end state
//          golden :147  case 420 MOT[MMTrayY_Car].SetTray(NULL_IC)
//        REQUIRES MMTrayZ EMPTY at entry, otherwise golden case 1's fast path
//        (golden :47-52) jumps straight to 400 -- which is what [10b] covers.
// ---------------------------------------------------------------------------
static void test_load_new_loader_tray_to_car()
{
    printf("\n[10a] DoLoadNewLoaderTrayToCar() full split walk -- case 50 is CylinderUp (both rise)\n");

    setupLoaderFixture();
    InitLoadNewLoaderTrayToCarTask();
    MOT[MMTrayZ].ClearTray("test_w7_l1_loader");            // else case 1 fast-paths to 400
    MOT[MMTrayY_Car].ClearTray("test_w7_l1_loader");        // must end SET
    Cylinder[C_Load_Up].Off();
    Cylinder[C_Load_Middle].Off();

    Traj traj;
    bool done  = false;
    int  steps = 0, upAt100 = -1, midAt100 = -1;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceTime();
        advanceRTCarSettle();
        done = DoLoadNewLoaderTrayToCar();
        trajPush(traj, iLoadNewLoaderTrayToCarTask);
        if (iLoadNewLoaderTrayToCarTask == 100 && upAt100 < 0)
        {
            upAt100  = Cylinder[C_Load_Up].bCylinderOn     ? 1 : 0;
            midAt100 = Cylinder[C_Load_Middle].bCylinderOn ? 1 : 0;
        }
    }

    static const int want[] = {50, 100, 200, 300, 400, 410, 420, 1};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(done, "DoLoadNewLoaderTrayToCar() returns true (converged, not bound-exhausted)");
    CHECK(trajEq(traj, want, nWant), "EXACT ordered trajectory 50,100,200,300,400,410,420,1");
    CHECK(upAt100 == 1 && midAt100 == 1,
          "case 50 CylinderUp raised BOTH lifter cylinders (golden :87) -- unlike [9]'s CylinderMiddle");
    CHECK(Cylinder[C_Load_Up].bCylinderOn == false && Cylinder[C_Load_Middle].bCylinderOn == false,
          "case 410 CylinderLower LOWERED both lifter cylinders (golden :131)");
    CHECK(MOT[MMTrayY_Car].fHasTray == true,
          "case 420 latched the split tray onto MMTrayY_Car (golden :147)");
    trajPrint("DoLoadNewLoaderTrayToCar", traj, want, nWant);
}

// ---------------------------------------------------------------------------
//  [10b] DoLoadNewLoaderTrayToCar case-1 FAST PATH (golden :47-52).
//        If the Z stage already carries a tray the whole lift is skipped: arm the
//        1 s settle timer and jump to 400.  This is also the ONLY fixture in
//        which golden :148's `MOT[MMTrayZ].ClearTray()` at case 420 is
//        observable -- in [10a] the stage starts empty, so deleting that line
//        would be invisible there.  Both halves exist for that reason.
// ---------------------------------------------------------------------------
static void test_load_new_loader_tray_to_car_fastpath()
{
    printf("\n[10b] DoLoadNewLoaderTrayToCar() case-1 fast path (Z stage already loaded)\n");

    setupLoaderFixture();
    InitLoadNewLoaderTrayToCarTask();
    MOT[MMTrayZ].SetTray(HAS_IC, "test_w7_l1_loader");      // triggers the fast path; must end CLEARED
    MOT[MMTrayY_Car].ClearTray("test_w7_l1_loader");        // must end SET

    Traj traj;
    bool done  = false;
    int  steps = 0;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceTime();
        advanceRTCarSettle();
        done = DoLoadNewLoaderTrayToCar();
        trajPush(traj, iLoadNewLoaderTrayToCarTask);
    }

    static const int want[] = {400, 410, 420, 1};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(done, "DoLoadNewLoaderTrayToCar() returns true on the fast path");
    CHECK(trajEq(traj, want, nWant),
          "EXACT ordered trajectory 400,410,420,1 -- cases 50/100/200/300 SKIPPED (golden :47-52)");
    CHECK(MOT[MMTrayZ].fHasTray == false,
          "case 420 cleared the MMTrayZ stage flag (golden :148)");
    CHECK(MOT[MMTrayY_Car].fHasTray == true,
          "case 420 latched the tray onto MMTrayY_Car (golden :147)");
    trajPrint("DoLoadNewLoaderTrayToCar(fast)", traj, want, nWant);
}

// ---------------------------------------------------------------------------
//  [11] DoAutoLoader (RT, golden :477-677) -- the RT dispatcher.  Three claims:
//       (a) the fLoaderCanSupplyNewTray gate CLOSED makes it a no-op
//           (golden :490-491);
//       (b) iReceiveLoaderTray != 0 diverts the whole tick into
//           DoAutoLoaderReceive and leaves the DoAutoLoader cursor untouched
//           (golden :480-484);
//       (c) with both open it walks its own state set.
// ---------------------------------------------------------------------------
static void test_auto_loader_dispatch()
{
    printf("\n[11] DoAutoLoader() gates: supply-gate closed, receive-divert, open walk\n");

    // (a) supply gate closed
    setupLoaderFixture();
    InitAutoLoaderTask();
    iReceiveLoaderTray      = 0;
    fLoaderCanSupplyNewTray = false;
    MOT[MMTrayY].ClearTray("test_w7_l1_loader");
    MOT[MMTrayY_Car].ClearTray("test_w7_l1_loader");
    for (int i = 0; i < 5; ++i) { advanceTime(); DoAutoLoader(); }
    CHECK(iAutoLoaderTask == 1,
          "fLoaderCanSupplyNewTray==false makes DoAutoLoader a no-op (golden :490-491)");

    // (b) receive divert
    setupLoaderFixture();
    InitAutoLoaderTask();
    InitAutoLoaderReceiveTask();
    iReceiveLoaderTray = 2;
    MOT[MMTrayY].ClearTray("test_w7_l1_loader");
    MOT[MMTrayY_Car].ClearTray("test_w7_l1_loader");
    advanceTime();
    DoAutoLoader();
    CHECK(iAutoLoaderTask == 1,
          "iReceiveLoaderTray!=0 leaves the DoAutoLoader cursor untouched (golden :480-484)");
    CHECK(iAutoLoaderReceiveTask != 1,
          "iReceiveLoaderTray!=0 diverted the tick into DoAutoLoaderReceive (its cursor moved)");

    // (c) open walk
    setupLoaderFixture();
    InitAutoLoaderTask();
    iReceiveLoaderTray      = 0;
    fLoaderCanSupplyNewTray = true;
    MOT[MMTrayY].ClearTray("test_w7_l1_loader");
    MOT[MMTrayY_Car].ClearTray("test_w7_l1_loader");
    MOT[MMTrayZ].ClearTray("test_w7_l1_loader");
    Traj traj;
    for (int i = 0; i < 12; ++i)
    {
        advanceTime();
        advanceRTCarSettle();
        DoAutoLoader();
        trajPush(traj, iAutoLoaderTask);
    }
    // 12 ticks is enough to walk the dispatcher twice round its first two
    // decisions, and the walk is fully determined by the fixture:
    //   t1     case 1   -- no tray anywhere            -> 100        (golden :506-508)
    //   t2     case 100 -- car empty, so load a new one-> 200        (golden :647-648)
    //   t3-t9  case 200 -- pumps DoLoadNewLoaderTrayToCar's own 8-tick
    //                      split walk ([10a])          -> stays 200  (golden :664)
    //   t10    case 200 -- that SM returned true       -> 1          (golden :666-667)
    //   t11    case 1   -- car NOW has the split tray  -> 100        (golden :506-508)
    //   t12    case 100 -- car has a tray, tray-arm not parked at Empty
    //                      -> InitLoaderTrayToRearTask -> 300        (golden :641-642)
    static const int want[] = {100, 200, 1, 100, 300};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));
    CHECK(trajEq(traj, want, nWant),
          "EXACT ordered trajectory 100,200,1,100,300 (feed a tray to the car, then route it to the rear)");
    trajPrint("DoAutoLoader", traj, want, nWant);
}

// ---------------------------------------------------------------------------
//  [12] DoAutoLoaderReceive (RT, golden :852-903) -- the recycle chain.
//       With a tray on the car it goes straight to the unload hand-off, pumps
//       DoUnLoadNewLoaderToStack, clears the car and -- because MMTrayY is empty
//       -- ends the receive cycle by zeroing iReceiveLoaderTray.
//       iReceiveLoaderTray IS THE ASSERTION: it is the only observable that
//       distinguishes "the chain finished" from "the chain looped".
// ---------------------------------------------------------------------------
static void test_auto_loader_receive()
{
    printf("\n[12] DoAutoLoaderReceive() recycle chain terminates (iReceiveLoaderTray -> 0)\n");

    setupLoaderFixture();
    InitAutoLoaderReceiveTask();
    InitUnLoadNewLoaderTrayTask();
    iReceiveLoaderTray = 2;
    MOT[MMTrayY].ClearTray("test_w7_l1_loader");
    MOT[MMTrayY_Car].SetTray(NULL_IC, "test_w7_l1_loader");
    MOT[MMTrayZ].ClearTray("test_w7_l1_loader");

    Traj traj;
    bool finished = false;
    int  ticks    = 0;
    for (ticks = 0; ticks < 60; ++ticks)
    {
        advanceTime();
        DoAutoLoaderReceive();
        trajPush(traj, iAutoLoaderReceiveTask);
        if (iReceiveLoaderTray == 0) { finished = true; break; }
    }

    static const int want[] = {200, 1};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(finished, "DoAutoLoaderReceive ended the receive cycle (iReceiveLoaderTray reached 0, golden :897)");
    CHECK(trajEq(traj, want, nWant), "EXACT ordered trajectory 200,1 (car had a tray -> unload hand-off)");
    CHECK(MOT[MMTrayY_Car].fHasTray == false,
          "case 200 cleared MMTrayY_Car after the unload completed (golden :889)");
    trajPrint("DoAutoLoaderReceive", traj, want, nWant);
    printf("    (ticks=%d)\n", ticks);
}

// ---------------------------------------------------------------------------
//  [13] DoCassetteLoadNewICTray (golden :70-146) -- the 9046CR cassette feed.
//       The layer counter is the payload: golden :112 increments
//       TestIF_File.iCassetteLDNowLayer once per fetched layer and golden :113
//       derives the Z target from it, so losing the increment silently re-fetches
//       the same cassette slot forever.
// ---------------------------------------------------------------------------
static void test_cassette_load_new_ic_tray()
{
    printf("\n[13] DoCassetteLoadNewICTray() cassette walk + layer counter\n");

    setupLoaderFixture();
    InitCassetteLoadNewICTrayTask();
    TestIF_File.iCassetteLDNowLayer = 0;
    bBoatChangeCasset[0] = false;
    MOT[MMTrayY_Car].ClearTray("test_w7_l1_loader");

    Traj traj;
    bool done  = false;
    int  steps = 0;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceTime();
        done = DoCassetteLoadNewICTray();
        trajPush(traj, iCassetteLoadNewICTrayTask);
    }

    static const int want[] = {10, 20, 100, 110, 120, 200, 1};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(done, "DoCassetteLoadNewICTray() returns true (converged, not bound-exhausted)");
    CHECK(trajEq(traj, want, nWant), "EXACT ordered trajectory 10,20,100,110,120,200,1");
    CHECK(TestIF_File.iCassetteLDNowLayer == 1,
          "case 20 advanced the cassette layer counter by exactly 1 (golden :112)");
    CHECK(MOT[MMTrayY_Car].fHasTray == true,
          "case 200 latched the cassette tray onto MMTrayY_Car (golden :140)");
    trajPrint("DoCassetteLoadNewICTray", traj, want, nWant);

    // The early-out guard is a separate, independently observable behaviour
    // (golden :83-84): a cassette-change in progress freezes the SM.
    InitCassetteLoadNewICTrayTask();
    bBoatChangeCasset[0] = true;
    advanceTime();
    bool r = DoCassetteLoadNewICTray();
    CHECK(r == false && iCassetteLoadNewICTrayTask == 1,
          "bBoatChangeCasset[0] freezes the cassette SM at its cursor (golden :83-84)");
    bBoatChangeCasset[0] = false;
}

// ---------------------------------------------------------------------------
//  [14] DoKnockBeforeRemainICCheck (golden :3256-3312) -- the Loader vibration
//       knock.  Two claims:
//         golden :3260-3264  bIsFirst==true resets the cursor to 1 and returns
//                            true WITHOUT stepping the SM;
//         golden :3274/:3283 SW[SwLoaderVibration] On() then Off().  Sampled at
//                            cursor 1065 (after case 1 ran) and at the end.
// ---------------------------------------------------------------------------
static void test_knock_before_remain_ic_check()
{
    printf("\n[14] DoKnockBeforeRemainICCheck() vibration knock On->Off\n");

    setupLoaderFixture();
    iKnockTrayTask = 777;
    bool first = DoKnockBeforeRemainICCheck(true);
    CHECK(first == true && iKnockTrayTask == 1,
          "bIsFirst==true resets the knock cursor to 1 and returns true (golden :3260-3264)");

    SW[SwLoaderVibration].Off();            // opposite of the ON case 1 must write

    Traj traj;
    bool done  = false;
    int  steps = 0, vibAt1065 = -1;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceTime();
        done = DoKnockBeforeRemainICCheck(false);
        trajPush(traj, iKnockTrayTask);
        if (iKnockTrayTask == 1065 && vibAt1065 < 0)
            vibAt1065 = SW[SwLoaderVibration].OutValue ? 1 : 0;
    }

    static const int want[] = {1065, 1070, 9999};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(done, "DoKnockBeforeRemainICCheck() returns true at case 9999 (golden :3305)");
    CHECK(trajEq(traj, want, nWant), "EXACT ordered trajectory 1065,1070,9999");
    CHECK(vibAt1065 == 1,
          "case 1 turned the Loader vibration motor ON (golden :3274 On(), not Off())");
    CHECK(SW[SwLoaderVibration].OutValue == false,
          "case 1065 turned the Loader vibration motor back OFF (golden :3283)");
    trajPrint("DoKnockBeforeRemainICCheck", traj, want, nWant);
}

// ---------------------------------------------------------------------------
//  [15] DoLoaderTrackDetectICFloating (golden :3060-3164) -- DOCUMENTED WAVE-3
//       BLOCKER, asserted rather than hidden.
//       The SM walks 100,200,300,400,500 and then PARKS ON 500 FOREVER, because
//       both case 400 and case 500 open with
//       `if(TrayMoveStatus(0,__FUNC__)==0) DetectLoaderTime.SetSecAndOn(20.0);`
//       and this tree's TrayMoveStatus is a hardwired `return 0` (asendic.cpp).
//       The 20 s JAM watchdog is therefore re-armed on every pass and its
//       JAM1012 (case 400) / JAM0901 (case 500) arms are STRUCTURALLY
//       UNREACHABLE until Wave 3 lands the real belt bodies.  Asserting the park
//       -- and asserting that nothing alarmed -- is the honest observation; the
//       alternative would be to claim coverage this fixture cannot have.
//         golden :3077 case 200 Cylinder[C_TrayY_Fixer].Pop() IS reachable and
//         is asserted.
// ---------------------------------------------------------------------------
static void test_track_detect_ic_floating_parks()
{
    printf("\n[15] DoLoaderTrackDetectICFloating() walks 100..500 then PARKS (Wave-3 blocker)\n");

    setupLoaderFixture();                   // banks + resets the alarm seam
    InitLoaderTrackDetectICFloatingTask();
    Cylinder[C_TrayY_Fixer].On();           // opposite of the case-200 Pop

    Traj traj;
    bool done  = false;
    int  fixerAt300 = -1;
    for (int i = 0; i < 200 && !done; ++i)
    {
        advanceTime();
        done = DoLoaderTrackDetectICFloating();
        trajPush(traj, iLoaderTrackDetectICFloatingTask);
        if (iLoaderTrackDetectICFloatingTask == 300 && fixerAt300 < 0)
            fixerAt300 = Cylinder[C_TrayY_Fixer].bCylinderOn ? 1 : 0;
    }

    static const int want[] = {100, 200, 300, 400, 500};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(done == false,
          "the SM does NOT converge -- TrayMoveStatus()==0 keeps re-arming the arrival watchdog");
    CHECK(iLoaderTrackDetectICFloatingTask == 500,
          "it parks on case 500 specifically (the tray-in wait, golden :3125-3154)");
    CHECK(trajEq(traj, want, nWant),
          "EXACT ordered trajectory 100,200,300,400,500 before the park");
    CHECK(fixerAt300 == 0,
          "case 200 RETRACTED the tray-Y fixer before the detect sweep (golden :3077 Pop())");
    CHECK(W906_ShowErrorMessage_Count == 0,
          "no alarm is raised while parked -- the JAM1012/JAM0901 arms are structurally unreachable");
    trajPrint("DoLoaderTrackDetectICFloating", traj, want, nWant);
}

// ---------------------------------------------------------------------------
//  [16] CheckLoaderICFloating (golden :3167-3231) -- the shared IC-floating
//       guard, and the one place in the bundle where a SENSOR LEVEL, not a
//       cursor, selects the behaviour.  Three live exits:
//         (a) no latch  -> returns true for BOTH directions (golden :3230).
//         (b) latch + iDir==0 + SnLoaderCarHasTray ON -> the tray came back, so
//             the latch is CLEARED and it still returns false this pass
//             (golden :3173 bReCheckFlag, :3211-3219).
//         (c) latch + iDir==1 + SnLoaderCarHasTray ON -> iDir 1 reads a
//             DIFFERENT sensor pair (SnLoaderPreDete && SnLoaderSureTray, golden
//             :3178), so the SAME sensor state must NOT clear the latch.
//       (c) is the direction-polarity assertion: swapping golden :3173 and :3178
//           makes (b) and (c) trade places.
// ---------------------------------------------------------------------------
static void test_check_loader_ic_floating()
{
    printf("\n[16] CheckLoaderICFloating() -- latch clear is DIRECTION-SPECIFIC\n");

    setupLoaderFixture();

    bLoaderTrayICDetectErr = false;
    bool a0 = CheckLoaderICFloating(0);
    bool a1 = CheckLoaderICFloating(1);
    CHECK(a0 == true && a1 == true,
          "no latch + no belt motion -> passes in both directions (golden :3230)");

    // (c) first, so that (b)'s clear cannot be mistaken for leftover state.
    simSensorOn(SnLoaderCarHasTray);
    simSensorUnknown(SnLoaderPreDete);
    simSensorUnknown(SnLoaderSureTray);
    bLoaderTrayICDetectErr = true;
    bool c = CheckLoaderICFloating(1);
    bool cLatch = bLoaderTrayICDetectErr;
    CHECK(c == false && cLatch == true,
          "latch + iDir==1 + only the CAR sensor on -> latch NOT cleared (golden :3178 reads PreDete+SureTray)");

    // (b)
    bLoaderTrayICDetectErr = true;
    bool b = CheckLoaderICFloating(0);
    bool bLatch = bLoaderTrayICDetectErr;
    CHECK(b == false && bLatch == false,
          "latch + iDir==0 + CAR sensor on -> latch CLEARED (golden :3173 + :3218)");

    simSensorUnknown(SnLoaderCarHasTray);
    bLoaderTrayICDetectErr = false;
}

// ---------------------------------------------------------------------------
//  [17] CheckContinManualRemoveTray (golden :3233-3254) -- the P24 "two trays in
//       a row must be removed by hand" bookkeeping.  Three claims:
//         (a) both P24 config flags off -> the counter is NOT touched;
//         (b) flag on, generic path -> counter decremented AND
//             MOT[MMTrayY].Tray.iNeedManualRemoved driven to 0 ("to empty");
//         (c) CosFunction.bSpecialP24 -> driven to 1 ("to color") instead.
//       Each target value is pre-set to the OTHER value first.
// ---------------------------------------------------------------------------
static void test_check_contin_manual_remove_tray()
{
    printf("\n[17] CheckContinManualRemoveTray() P24 counter + destination flag\n");

    setupLoaderFixture();
    const bool p24a = IniConfig.bP24TwoTrayMustManuallyRemovedForGenernal;
    const bool p24b = IniConfig.bP24SkipEventNeedRemoveColorTrayForIDT;
    const bool sp24 = CosFunction.bSpecialP24;

    // (a) gate closed
    IniConfig.bP24TwoTrayMustManuallyRemovedForGenernal = false;
    IniConfig.bP24SkipEventNeedRemoveColorTrayForIDT    = false;
    iManualRemoveTrayCnt = 3;
    CheckContinManualRemoveTray();
    CHECK(iManualRemoveTrayCnt == 3,
          "both P24 flags off -> iManualRemoveTrayCnt untouched (golden :3235-3236)");

    // (b) generic path -> "to empty" (0)
    IniConfig.bP24TwoTrayMustManuallyRemovedForGenernal = true;
    CosFunction.bSpecialP24 = false;
    iRunStartMode = 0;
    TrayForm.LoaderToEmptyColor[0] = 0;
    iManualRemoveTrayCnt = 3;
    MOT[MMTrayY].Tray.iNeedManualRemoved = 1;           // opposite of the expected 0
    CheckContinManualRemoveTray();
    CHECK(iManualRemoveTrayCnt == 2,
          "P24 on -> iManualRemoveTrayCnt decremented by exactly 1 (golden :3238)");
    CHECK(MOT[MMTrayY].Tray.iNeedManualRemoved == 0,
          "generic P24 + LoaderToEmptyColor==0 -> tray flagged TO EMPTY (0) (golden :3248)");

    // (c) special P24 -> "to color" (1)
    CosFunction.bSpecialP24 = true;
    iManualRemoveTrayCnt = 3;
    MOT[MMTrayY].Tray.iNeedManualRemoved = 0;           // opposite of the expected 1
    CheckContinManualRemoveTray();
    CHECK(MOT[MMTrayY].Tray.iNeedManualRemoved == 1,
          "CosFunction.bSpecialP24 -> tray flagged TO COLOR (1) (golden :3243)");

    IniConfig.bP24TwoTrayMustManuallyRemovedForGenernal = p24a;
    IniConfig.bP24SkipEventNeedRemoveColorTrayForIDT    = p24b;
    CosFunction.bSpecialP24 = sp24;
}

// ---------------------------------------------------------------------------
//  [18] THE NOT-COVERED REGISTER, ASSERTED.
//       Section A of the banner claims the whole alarm surface is unentered.
//       That claim is only worth something if it is measured, so measure it:
//       after every sub-test above, the ShowErrorMessage seam must still read
//       zero calls.  If a future edit makes any alarm arm reachable this goes
//       RED and forces the register to be updated rather than silently rotting.
// ---------------------------------------------------------------------------
static void test_not_covered_register_is_true()
{
    printf("\n[18] NOT-COVERED register self-check (no alarm arm was entered)\n");
    const int alarms  = g_alarmsBanked  + W906_ShowErrorMessage_Count;
    const int dialogs = g_dialogsBanked + W906_ShowMyMessage_Count;
    CHECK(alarms == 0,
          "ShowErrorMessage was never called by ANY sub-test (banner section A holds)");
    CHECK(dialogs == 0,
          "ShowMyMessage was never called by ANY sub-test either (no operator dialog is reached)");
    printf("    (banked+current: ShowErrorMessage=%d last=\"%s\"; ShowMyMessage=%d last=\"%s\")\n",
           alarms, W906_ShowErrorMessage_LastCode.c_str(),
           dialogs, W906_ShowMyMessage_LastS1.c_str());
}

// ---------------------------------------------------------------------------
int main()
{
    printf("=== test_w7_l1_loader : asendic_Loader + asendic_Loader_RT (W7-L1 Wave 1) ===\n");

    test_inits();
    test_seams();
    test_trayz_load_tray_to_wait();
    test_load_new_ic_tray();
    test_supply_new_ic_tray();
    test_do_load_case1_handoff();
    test_do_load_end_to_end();
    test_loader_tray_to_front();
    test_loader_tray_to_rear();
    test_unload_new_loader_to_stack();
    test_load_new_loader_tray_to_car();
    test_load_new_loader_tray_to_car_fastpath();
    test_auto_loader_dispatch();
    test_auto_loader_receive();
    test_cassette_load_new_ic_tray();
    test_knock_before_remain_ic_check();
    test_track_detect_ic_floating_parks();
    test_check_loader_ic_floating();
    test_check_contin_manual_remove_tray();
    test_not_covered_register_is_true();

    printf("\nRESULT: %d passed, %d failed\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
