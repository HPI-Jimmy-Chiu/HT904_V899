// =============================================================================
//  test_w7_l1_auto2.cpp  --  W7-L1 asendic_Auto2 (Auto2-tray stack-feed) SM test
//
//  Translation wave: W906-W7-L1
//  Author: AI(W906-W7-L1) 20260729
//  AI(W906-W7-L1fix) 20260729: REWRITTEN.  The original version of this file
//  asserted `cleared || steps == 1000` / `done || cursor != 1 || steps == 500`
//  -- both TAUTOLOGIES (the loop can only exit by setting the first disjunct or
//  by exhausting the bound, so the assertion was true by construction).  It
//  therefore reported PASS while DoUnLoadNewAuto2ToStack (golden
//  asendic_Auto2.cpp:600-706) had ZERO coverage and DoLoadNewAuto2TrayToCar
//  converged on step 0 through a short-circuit that never touched the
//  AutoCylinder* helpers the file header claimed it exercised end-to-end.
//  Every assertion below now constrains a specific observable of the state
//  machine and has been PROVEN capable of failing (see FAILABILITY PROOF).
//
//  PROOF OBJECTIVE
//  ---------------
//  Drive the Auto2-tray SM family over the Sim HAL (Cylinder[]/MOT[]/Sen[]/SW[])
//  with no hardware, and assert each SM walks the SPECIFIC cursor trajectory the
//  golden prescribes -- not merely "it did not crash".  A trajectory assertion is
//  load-bearing: deleting or re-targeting any single `Task=N` transition drops N
//  (and everything downstream of it) out of the observed set and the test goes red.
//
//  TWO TEST SEAMS THIS FILE INTRODUCES (both fixture-only; zero production change)
//  ------------------------------------------------------------------------------
//  1. VIRTUAL TIME.  TQPF_Timer (myTimer.h) calls QueryPerformanceCounter
//     directly and has NO injection point -- unlike MyTickCount (common.cpp:1907,
//     which atester.cpp:1515 redirects with a TU-local macro) there is no seam to
//     override, and unlike uHGemEquipment's TimerSetMSAndOn there is no <=0
//     shortcut inside the class.  So instead of overriding the clock we RE-ARM the
//     pending timer with a 0 ms deadline before each SM tick: SetMS(0) leaves
//     rSetTime==0 and On() then sets rEnd = rStart - 2*overhead, so the very next
//     Off() is true (myTimer.cpp:40-44/94-98).  The Auto2 timers are non-static
//     file-scope objects in asendic_Auto2.cpp, so the extern declarations below
//     reach them without touching asendic_Auto2.h.
//     Only the five PROGRESS timers are advanced.  The two 20 s JAM watchdogs
//     (hAuto2TrayToFront / hAuto2TrayToRear, golden :176 / :313) are deliberately
//     left RUNNING so that a stalled transition surfaces as a JAM alarm in the
//     trace instead of being masked by a forced expiry.
//  2. SIM SENSOR LEVELS.  Sen[] defaults to Enable==false, which makes IsOn() AND
//     IsOff() both return false (mysensor.cpp:127-131 / :177-181) -- an "unknown"
//     reading that is NOT the same as "off", and which is exactly what let the old
//     [3] short-circuit out of DoLoadNewAuto2TrayToCar on step 0.  To read a
//     deterministic level with no IO plumbing: Enable=true + ISABase=eISABase
//     routes to mysensor.cpp:29's raw-port stub (always false), and Type then
//     picks the polarity -- TYPE_A(1) => IsOff()==true / IsOn()==false ("off"),
//     TYPE_B(0) => IsOn()==true / IsOff()==false ("on").
//
//  WHAT IT DRIVES
//    * All 6 Init* -- each resets its own cursor int to 1.
//    * DoAuto2() (top dispatcher, golden :414-592) over FOUR scenarios, because a
//      single scenario cannot reach the whole switch: A the feed walk
//      (1,2,3,20,30,40,50,60,70,100,150), B the case-100 -> 200 feed handoff,
//      C the case-100 -> 300 recycle handoff, D the case-60 -> 65 tray re-check
//      and its DoAuto2Receive hand-off (iReceiveAuto2Tray=2 / iAuto2ReceiveTask=200).
//    * DoLoadNewAuto2TrayToCar() (golden :38-173) BOTH ways: the full lifter walk
//      50,100,200,201,300,400,410,420 with the stack sensors reading "off", AND
//      the case-1 short-circuit (sensors "unknown") that returns true on step 0.
//    * DoAuto2TrayToFront() (golden :183-310) long path 101,102,103,104,105.
//    * DoAuto2TrayToRear() (golden :321-404) path 100,200,450,460,500.
//    * DoUnLoadNewAuto2ToStack() (golden :600-706) path 50,100,150,200,300,310,
//      400,401,402 -- driven directly AND through DoAuto2Receive().
//    * DoAuto2Receive() (golden :715-749) full chain to the unload hand-off.
//    * The fAuto2CanSupplyNewTray gate: closed -> DoAuto2() is a no-op.
//
//  AutoCylinderUp/Middle/Lower ARE NOT REAL BODIES IN THIS TREE.  The original
//  header claimed this test exercised them "end-to-end"; it did not, and there is
//  nothing to exercise: golden defines them in asendic.cpp:562/767/937 (declared
//  golden asendic.h:19-21) and asendic.cpp's Auto* lifter SMs are NOT translated,
//  so acatchtray_shims.cpp holds no-op `return true` stubs.  What the trajectory
//  assertions below prove is exactly this much and no more: every AutoCylinder*
//  CALL SITE is reached and its true-return is consumed by the caller (call sites
//  live in DoLoadNewAuto2TrayToCar cases 1/50/200/201/400/410 and
//  DoUnLoadNewAuto2ToStack cases 1/50/150/200/400/401 -- all on the asserted
//  trajectories).  It proves nothing about real lifter behaviour.
//
//  NOT COVERED (stated so nobody reads coverage into silence)
//    * Every JAM/MES alarm arm (JAM1213/JAM1212/JAM1201/JAM1203/JAM1113/MES1024):
//      ShowErrorMessage is a canary_support stub that always answers K_RETRY
//      (canary_support.cpp:44-50), so the K_SKIP arms are unreachable offline.
//    * DoAuto2 case 3's `WhichAutoNeedTray()==0` guard arm -- unreachable while
//      LastSet.iRealDummy==DUMMY (case 3's first guard always wins).
//    * The #ifdef SOFT_SIMULTE arms (not defined in this build).
//    * Real lifter/cylinder motion -- see the AutoCylinder* note above.
//
//  FAILABILITY PROOF (each assertion group was seen RED before being trusted)
//    Documented in the W7-L1fix report: `Task=201` in DoLoadNewAuto2TrayToCar
//    case 200 was temporarily changed to `Task=400`, and `Task=310` in
//    DoUnLoadNewAuto2ToStack case 300 to `Task=400`; both were observed to fail
//    the corresponding trajectory assertions, then reverted and re-run green.
//
//  Offline: no vendor SDK, no hardware, no window.
// =============================================================================
#include "asendic_Auto2.h"
#include "csystem.h"
#include "asendic.h"
#include "acatchtray.h"
#include "Motor/mymotor.h"
#include "mycylin.h"
#include "mysensor.h"
#include "myswitch.h"
#include "myTimer.h"            // TQPF_Timer (the virtual-time seam below)
#include "cprod.h"
#include "cpublic.h"
#include "cmydef.h"
#include "canary_support.h"     // LastSet, ShowErrorMessage, WhichAutoNeedTray
#include "FormsFacade.h"        // fAGV
#include <cstdio>
#include <set>

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as test_w6_1_empty_canary.cpp)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                   \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---------------------------------------------------------------------------
//  VIRTUAL-TIME SEAM -- the file-scope TQPF_Timer objects of asendic_Auto2.cpp.
//  Progress timers (advanced every tick):
// ---------------------------------------------------------------------------
extern TQPF_Timer Auto2Delay;                   // golden asendic_Auto2.cpp:31
extern TQPF_Timer DoAuto2Delay;                 // golden asendic_Auto2.cpp:407
extern TQPF_Timer CheckAuto2TrayDelay;          // golden asendic_Auto2.cpp:408
extern TQPF_Timer hAuto2TrayToFrontForDummy;    // golden asendic_Auto2.cpp:177
extern TQPF_Timer hAuto2TrayToRearForDummy;     // golden asendic_Auto2.cpp:314
//  JAM watchdogs -- declared for documentation; deliberately NEVER advanced:
//    extern TQPF_Timer hAuto2TrayToFront;      // golden asendic_Auto2.cpp:176 (20 s)
//    extern TQPF_Timer hAuto2TrayToRear;       // golden asendic_Auto2.cpp:313 (20 s)

// Expire whatever progress timer the SM is currently waiting on, so one loop
// iteration == one "tick" of virtual time instead of one nanosecond of real time.
static void advanceAuto2Time()
{
    Auto2Delay.SetMSAndOn(0);
    DoAuto2Delay.SetMSAndOn(0);
    CheckAuto2TrayDelay.SetMSAndOn(0);
    hAuto2TrayToFrontForDummy.SetMSAndOn(0);
    hAuto2TrayToRearForDummy.SetMSAndOn(0);
}

// ---------------------------------------------------------------------------
//  SIM SENSOR SEAM -- see banner note 2.
// ---------------------------------------------------------------------------
static void simSensorOff(int idx)       // IsOff()==true, IsOn()==false
{
    Sen[idx].Enable  = true;
    Sen[idx].Type    = TYPE_A;          // cmydef.cpp:269 TYPE_A==1
    Sen[idx].ISABase = eISABase;        // -> mysensor.cpp:29 raw-port stub (false)
}
static void simSensorUnknown(int idx)   // IsOn()==false AND IsOff()==false (default)
{
    Sen[idx].Enable = false;
}

// DoAuto2()'s documented state set (golden :430-591 -> 1,2,3,20,30,40,50,60,65,
// 70,100,150,200,300).  A wild cursor means corruption.
static bool auto2CursorSane(int t)
{
    switch (t)
    {
        case 1: case 2: case 3: case 20: case 30: case 40: case 50: case 60:
        case 65: case 70: case 100: case 150: case 200: case 300:
            return true;
        default:
            return false;
    }
}

// ---------------------------------------------------------------------------
//  Trajectory recorder + reporter
// ---------------------------------------------------------------------------
typedef std::set<int> StateSet;

static bool covers(const StateSet &seen, const int *want, int nWant)
{
    for (int i = 0; i < nWant; ++i)
        if (seen.find(want[i]) == seen.end())
            return false;
    return true;
}

static void printSeen(const char *what, const StateSet &seen)
{
    printf("    (%s visited:", what);
    for (StateSet::const_iterator it = seen.begin(); it != seen.end(); ++it)
        printf(" %d", *it);
    printf(")\n");
}

static void printMissing(const char *what, const StateSet &seen,
                         const int *want, int nWant)
{
    printf("    (%s MISSING:", what);
    for (int i = 0; i < nWant; ++i)
        if (seen.find(want[i]) == seen.end())
            printf(" %d", want[i]);
    printf(")\n");
}

// ---------------------------------------------------------------------------
//  Run-mode globals: the DUMMY tray-feed configuration the SM expects.
//  The two settle delays are pinned to 0 s because they are operator-tunable
//  machine settings (Ld_ULd form), not SM logic: SetSecAndOn(0) makes the
//  arrival/back settle waits immediately elapsed, which is what makes the SM
//  steppable in a headless test without faking any transition.
// ---------------------------------------------------------------------------
static void setupDummyFeed()
{
    LastSet.iRealDummy      = DUMMY;    // dummy feed (no real IC/tray sensors)
    fAuto2CanSupplyNewTray  = true;     // allow supply
    iReceiveAuto2Tray       = 0;        // not in receive mode
    bEmptyPause             = false;
    bHandlerPause           = false;
    SUPPORT_2_EMPTY_EMPTY   = false;    // cmydef.cpp:3050 default
    Ld_UldDelayTime.LD_TrayArrivalDely = 0;   // golden DoAuto2TrayToRear case 450
    Ld_UldDelayTime.ULD_TrayBackDelay  = 0;   // golden DoUnLoadNewAuto2ToStack 100/310

    // Auto2 stack/track sensors: "unknown" by default (matches an unconfigured
    // offline IO table); individual sub-tests raise the ones they need.
    simSensorUnknown(SnAuto2PreDete);
    simSensorUnknown(SnAuto2_Tray_Car);
    simSensorUnknown(SnAuto2TrayDetect);
    simSensorUnknown(SnAuto2CWPreDetect);
    simSensorUnknown(SnAuto2FixCyPush);
    simSensorUnknown(SnAuto2TrayHasTray);
    simSensorUnknown(SnAutoUpSafedetect1);
}

// ---------------------------------------------------------------------------
//  [1] Init* reset task cursors to 1
// ---------------------------------------------------------------------------
static void test_inits()
{
    printf("\n[1] Init* reset task cursors to 1\n");

    iLoadNewAuto2TrayToCarTask = 999;
    InitLoadNewAuto2TrayToCarTask();
    CHECK(iLoadNewAuto2TrayToCarTask == 1, "InitLoadNewAuto2TrayToCarTask() -> iLoadNewAuto2TrayToCarTask==1");

    iAuto2TrayToFrontTask = 999;
    InitAuto2TrayToFrontTask();
    CHECK(iAuto2TrayToFrontTask == 1, "InitAuto2TrayToFrontTask() -> iAuto2TrayToFrontTask==1");

    iAuto2TrayToRearTask = 999;
    InitAuto2TrayToRearTask();
    CHECK(iAuto2TrayToRearTask == 1, "InitAuto2TrayToRearTask() -> iAuto2TrayToRearTask==1");

    iAutoAuto2Task = 999;
    InitAuto2Task();
    CHECK(iAutoAuto2Task == 1, "InitAuto2Task() -> iAutoAuto2Task==1");

    iUnLoadNewAuto2TrayTask = 999;
    InitUnLoadNewAuto2TrayTask();
    CHECK(iUnLoadNewAuto2TrayTask == 1, "InitUnLoadNewAuto2TrayTask() -> iUnLoadNewAuto2TrayTask==1");

    iAuto2ReceiveTask = 999;
    InitAuto2ReceiveTask();
    CHECK(iAuto2ReceiveTask == 1, "InitAuto2ReceiveTask() -> iAuto2ReceiveTask==1");
}

// ---------------------------------------------------------------------------
//  [2] The virtual-time seam itself must work, or every trajectory below is
//      vacuous.  Assert BOTH directions: a re-armed 1 s timer is NOT elapsed,
//      and after advanceAuto2Time() it IS.
// ---------------------------------------------------------------------------
static void test_virtual_time_seam()
{
    printf("\n[2] Virtual-time seam (TQPF_Timer re-arm to 0 ms)\n");

    DoAuto2Delay.SetMSAndOn(1000);
    CHECK(DoAuto2Delay.Off() == false, "a freshly armed 1000 ms timer is NOT elapsed");
    advanceAuto2Time();
    CHECK(DoAuto2Delay.Off() == true,  "after advanceAuto2Time() the same timer IS elapsed");

    // And the sensor seam: prove both polarities, since 'unknown' vs 'off' is
    // exactly the distinction the old test got wrong.
    simSensorUnknown(SnAuto2PreDete);
    CHECK(Sen[SnAuto2PreDete].IsOn() == false && Sen[SnAuto2PreDete].IsOff() == false,
          "Enable==false sensor reads neither On nor Off ('unknown', mysensor.cpp:127/177)");
    simSensorOff(SnAuto2PreDete);
    CHECK(Sen[SnAuto2PreDete].IsOff() == true && Sen[SnAuto2PreDete].IsOn() == false,
          "Enable+TYPE_A sensor reads Off (IsOff()==true, IsOn()==false)");
    simSensorUnknown(SnAuto2PreDete);
}

// ---------------------------------------------------------------------------
//  [3] DoLoadNewAuto2TrayToCar() -- BOTH branches of case 1.
//
//  [3a] Stack sensors "off"  -> the FULL lifter walk (golden :121-122 takes
//       Task=50), trajectory 50,100,200,201,300,400,410,420, converges with a
//       tray latched onto MMAuto2_Car (case 420, golden :210).
//  [3b] Stack sensors "unknown" -> the case-1 short-circuit (golden :113-120)
//       returns true on step 0 WITHOUT ever reaching 50.  This is the branch the
//       old test hit while claiming to exercise the lifter end-to-end; pinned
//       here so the difference can never again be mistaken for coverage.
// ---------------------------------------------------------------------------
static void test_load_new_auto2_full_lifter_walk()
{
    printf("\n[3a] DoLoadNewAuto2TrayToCar() full lifter walk (stack sensors OFF)\n");

    setupDummyFeed();
    InitLoadNewAuto2TrayToCarTask();
    MOT[MMAuto2].ClearTray("test_w7_l1_auto2");
    MOT[MMAuto2Z].ClearTray("test_w7_l1_auto2");
    MOT[MMAuto2_Car].ClearTray("test_w7_l1_auto2");

    // Both stack sensors must read a definite OFF, else case 1 short-circuits.
    simSensorOff(SnAuto2PreDete);
    simSensorOff(SnAuto2_Tray_Car);

    StateSet seen;
    bool done  = false;
    int  steps = 0;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceAuto2Time();
        done = DoLoadNewAuto2TrayToCar();
        seen.insert(iLoadNewAuto2TrayToCarTask);
    }

    static const int want[] = {50, 100, 200, 201, 300, 400, 410, 420};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(done, "DoLoadNewAuto2TrayToCar() returns true (converged, not bound-exhausted)");
    CHECK(covers(seen, want, nWant),
          "trajectory covers 50,100,200,201,300,400,410,420 (all AutoCylinder* call sites reached)");
    CHECK(MOT[MMAuto2_Car].fHasTray == true,
          "case 420 latched the tray onto MMAuto2_Car (golden :210 SetTray)");
    CHECK(iLoadNewAuto2TrayToCarTask == 1, "cursor reset to 1 on convergence");
    printSeen("DoLoadNewAuto2TrayToCar", seen);
    if (!covers(seen, want, nWant)) printMissing("DoLoadNewAuto2TrayToCar", seen, want, nWant);
    printf("    (converged=%d after %d steps)\n", (int)done, steps);
}

static void test_load_new_auto2_case1_shortcircuit()
{
    printf("\n[3b] DoLoadNewAuto2TrayToCar() case-1 short-circuit (sensors UNKNOWN)\n");

    setupDummyFeed();                       // leaves all Auto2 sensors 'unknown'
    InitLoadNewAuto2TrayToCarTask();
    MOT[MMAuto2].ClearTray("test_w7_l1_auto2");
    MOT[MMAuto2Z].ClearTray("test_w7_l1_auto2");
    MOT[MMAuto2_Car].ClearTray("test_w7_l1_auto2");

    advanceAuto2Time();
    bool firstStep = DoLoadNewAuto2TrayToCar();

    CHECK(firstStep == true,
          "returns true on the FIRST step (case-1 short-circuit, golden :113-120)");
    CHECK(iLoadNewAuto2TrayToCarTask == 1, "cursor never left 1 -- case 50 NOT reached");
    CHECK(MOT[MMAuto2_Car].fHasTray == true, "short-circuit still latches MMAuto2_Car (golden :116)");
}

// ---------------------------------------------------------------------------
//  [4] DoAuto2TrayToFront() long path (golden :183-310).
//      case 1 falls through into case 100 (GOLDEN QUIRK, golden :194), which
//      hands off to 101 -> 102 -> 103 -> 104 -> 105 -> return true.
//
//      FALL-THROUGH PIN: because case 1 has no `break`, case 1 and case 100 both
//      run in the SAME tick, so the cursor value 100 is never observable between
//      ticks -- after ONE call it is already 101.  Asserting exactly that is a
//      direct test of the golden quirk: insert a `break;` after golden :193's
//      `Task=100;` and the first tick would leave the cursor at 100 instead.
// ---------------------------------------------------------------------------
static void test_tray_to_front_long_path()
{
    printf("\n[4] DoAuto2TrayToFront() long path 101,102,103,104,105\n");

    setupDummyFeed();
    InitAuto2TrayToFrontTask();
    MOT[MMAuto2].SetTray(NULL_IC, "test_w7_l1_auto2");
    MOT[MMAuto2_Car].SetTray(NULL_IC, "test_w7_l1_auto2");
    simSensorOff(SnAuto2_Tray_Car);         // IsOn()==false -> case 100 hands to 101

    StateSet seen;
    bool done  = false;
    int  steps = 0;
    int  afterFirstTick = -1;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceAuto2Time();
        done = DoAuto2TrayToFront();
        seen.insert(iAuto2TrayToFrontTask);
        if (steps == 0) afterFirstTick = iAuto2TrayToFrontTask;
    }

    static const int want[] = {101, 102, 103, 104, 105};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(afterFirstTick == 101,
          "GOLDEN QUIRK pin: case 1 fell through into case 100 in one tick (cursor 1 -> 101)");
    CHECK(done, "DoAuto2TrayToFront() returns true (converged)");
    CHECK(covers(seen, want, nWant), "trajectory covers 101,102,103,104,105");
    CHECK(MOT[MMAuto2].fHasTray == false && MOT[MMAuto2_Car].fHasTray == false,
          "case 105 cleared BOTH MMAuto2 and MMAuto2_Car (golden :372-373)");
    printSeen("DoAuto2TrayToFront", seen);
    if (!covers(seen, want, nWant)) printMissing("DoAuto2TrayToFront", seen, want, nWant);
}

// ---------------------------------------------------------------------------
//  [5] DoAuto2TrayToRear() (golden :321-404).  case 1 falls through into case
//      100 (GOLDEN QUIRK, golden :333) -> 200 -> 450 -> 460 -> 500 -> true.
//
//      Same FALL-THROUGH PIN as [4], and here it is even tighter: case 100's own
//      body (`if(Cylinder[C_Auto2Side_Fixer].Pop()) Task=200;`) also completes in
//      that first tick offline, so the cursor goes 1 -> 200 in ONE call and the
//      value 100 is never observable between ticks.  That is why 100 is NOT in
//      the wanted set below -- its execution is proven by the 1 -> 200 jump.
//      (This is exactly what the first run of this rewritten test caught: an
//      earlier draft wanted 100 in the set and went red.)
// ---------------------------------------------------------------------------
static void test_tray_to_rear_path()
{
    printf("\n[5] DoAuto2TrayToRear() path 200,450,460,500 (100 runs in tick 1)\n");

    setupDummyFeed();
    InitAuto2TrayToRearTask();
    MOT[MMAuto2].ClearTray("test_w7_l1_auto2");
    MOT[MMAuto2_Car].SetTray(NULL_IC, "test_w7_l1_auto2");

    StateSet seen;
    bool done  = false;
    int  steps = 0;
    int  afterFirstTick = -1;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceAuto2Time();
        done = DoAuto2TrayToRear();
        seen.insert(iAuto2TrayToRearTask);
        if (steps == 0) afterFirstTick = iAuto2TrayToRearTask;
    }

    static const int want[] = {200, 450, 460, 500};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(afterFirstTick == 200,
          "GOLDEN QUIRK pin: case 1 fell through into case 100 AND case 100 ran (cursor 1 -> 200)");
    CHECK(done, "DoAuto2TrayToRear() returns true (converged)");
    CHECK(covers(seen, want, nWant), "trajectory covers 200,450,460,500");
    CHECK(MOT[MMAuto2].fHasTray == true && MOT[MMAuto2_Car].fHasTray == false,
          "case 500 moved the tray MMAuto2_Car -> MMAuto2 (golden :477-478)");
    printSeen("DoAuto2TrayToRear", seen);
    if (!covers(seen, want, nWant)) printMissing("DoAuto2TrayToRear", seen, want, nWant);
}

// ---------------------------------------------------------------------------
//  [6] DoUnLoadNewAuto2ToStack() (golden :600-706) driven DIRECTLY.
//      This is the function the old test claimed to drive and never entered.
// ---------------------------------------------------------------------------
static void test_unload_to_stack_direct()
{
    printf("\n[6] DoUnLoadNewAuto2ToStack() direct: 50,100,150,200,300,310,400,401,402\n");

    setupDummyFeed();
    InitUnLoadNewAuto2TrayTask();
    MOT[MMAuto2].SetTray(NULL_IC, "test_w7_l1_auto2");
    MOT[MMAuto2_Car].SetTray(NULL_IC, "test_w7_l1_auto2");
    // case 402 must NOT see both stack sensors ON, or it raises JAM1113 forever.
    simSensorOff(SnAuto2PreDete);
    simSensorOff(SnAuto2_Tray_Car);
    fAuto2CanSupplyNewTray = false;          // case 402 sets it true on success

    StateSet seen;
    bool done  = false;
    int  steps = 0;
    for (steps = 0; steps < 200 && !done; ++steps)
    {
        advanceAuto2Time();
        done = DoUnLoadNewAuto2ToStack();
        seen.insert(iUnLoadNewAuto2TrayTask);
    }

    static const int want[] = {50, 100, 150, 200, 300, 310, 400, 401, 402};
    const int nWant = (int)(sizeof(want)/sizeof(want[0]));

    CHECK(done, "DoUnLoadNewAuto2ToStack() returns true (converged)");
    CHECK(covers(seen, want, nWant),
          "trajectory covers 50,100,150,200,300,310,400,401,402");
    CHECK(MOT[MMAuto2].fHasTray == false,
          "case 310 cleared MMAuto2 (golden :761)");
    CHECK(fAuto2CanSupplyNewTray == true,
          "case 402 re-opened the supply gate on success (golden :802)");
    CHECK(iUnLoadNewAuto2TrayTask == 1, "cursor reset to 1 on convergence");
    printSeen("DoUnLoadNewAuto2ToStack", seen);
    if (!covers(seen, want, nWant)) printMissing("DoUnLoadNewAuto2ToStack", seen, want, nWant);
}

// ---------------------------------------------------------------------------
//  [7] DoAuto2Receive() full chain -> DoAuto2TrayToFront -> hand-off ->
//      DoUnLoadNewAuto2ToStack -> car cleared, receive mode exited.
//
//      SUPPORT_2_EMPTY_EMPTY=true is REQUIRED to reach the hand-off, and this is
//      a real property of the golden, not a test convenience: it is the ONLY
//      DoAuto2TrayToFront exit that returns true with a tray still ON the car
//      (golden :277 SetTray).  Every other success arm (case 105 :373, the DUMMY
//      timeout arm :306, the JAM1212 skip arm :329) CLEARS MMAuto2_Car first, and
//      DoAuto2Receive case 100 only hands off `if(MOT[MMAuto2_Car].fHasTray)`
//      (golden :837) -- so with SUPPORT_2_EMPTY_EMPTY==false the unload branch of
//      DoAuto2Receive is unreachable, which is why the old test sat at
//      iAuto2ReceiveTask==100 for all 1000 iterations.
// ---------------------------------------------------------------------------
static void test_receive_chain_converges()
{
    printf("\n[7] DoAuto2Receive() chain -> DoUnLoadNewAuto2ToStack hand-off\n");

    setupDummyFeed();
    InitAuto2ReceiveTask();
    InitAuto2TrayToFrontTask();
    InitUnLoadNewAuto2TrayTask();
    SUPPORT_2_EMPTY_EMPTY = true;            // see banner above
    MOT[MMAuto2_Car].SetTray(NULL_IC, "test_w7_l1_auto2");
    MOT[MMAuto2].ClearTray("test_w7_l1_auto2");
    simSensorOff(SnAuto2PreDete);
    simSensorOff(SnAuto2_Tray_Car);
    iReceiveAuto2Tray = 1;                   // receive mode active

    StateSet seenRecv, seenUnload;
    bool handedOff = false, cleared = false;
    int  steps = 0;
    for (steps = 0; steps < 200 && !cleared; ++steps)
    {
        advanceAuto2Time();
        DoAuto2Receive();
        seenRecv.insert(iAuto2ReceiveTask);
        seenUnload.insert(iUnLoadNewAuto2TrayTask);
        if (iAuto2ReceiveTask == 200)
            handedOff = true;
        if (handedOff && iAuto2ReceiveTask == 1 && !MOT[MMAuto2_Car].fHasTray)
            cleared = true;
    }

    static const int wantUnload[] = {50, 100, 150, 200, 300, 310, 400, 401, 402};
    const int nWantUnload = (int)(sizeof(wantUnload)/sizeof(wantUnload[0]));

    CHECK(handedOff,
          "DoAuto2Receive reached case 200 (front-feed handed the car tray to unload)");
    CHECK(covers(seenUnload, wantUnload, nWantUnload),
          "DoUnLoadNewAuto2ToStack walked its full trajectory INSIDE the chain");
    CHECK(cleared, "chain converged: MMAuto2_Car cleared and cursor back to 1");
    CHECK(iReceiveAuto2Tray == 0, "receive mode exited (iReceiveAuto2Tray==0, golden :853)");
    printSeen("DoAuto2Receive", seenRecv);
    printSeen("DoUnLoadNewAuto2ToStack (in chain)", seenUnload);
    if (!covers(seenUnload, wantUnload, nWantUnload))
        printMissing("DoUnLoadNewAuto2ToStack (in chain)", seenUnload, wantUnload, nWantUnload);
    printf("    (converged after %d steps)\n", steps);
    SUPPORT_2_EMPTY_EMPTY = false;           // restore for later sub-tests
}

// ---------------------------------------------------------------------------
//  [8] DoAuto2() dispatcher -- four scenarios.
//
//  Scenario A: the feed walk.  The gate is re-armed every tick because case 70
//      deliberately closes it (golden :640) once the stage tray is latched; a
//      real machine re-opens it from DoUnLoadNewAuto2ToStack case 402.
//  Scenario B: at case 100, an EMPTY stage + empty car takes the feed arm
//      (Task=200 -> DoLoadNewAuto2TrayToCar -> iReceiveAuto2Tray=1, golden :673).
//  Scenario C: at case 100, a car that HAS a tray takes the recycle arm
//      (Task=300 -> DoAuto2TrayToRear -> Task=50, golden :680).
//  Scenario D: at case 60, a stage that HAS a tray takes the re-check arm
//      (Task=65) and, with MMTrayY holding an empty tray, fires the
//      DoAuto2Receive hand-off (golden :624-625).
//
//  THE THIRD GOLDEN FALL-THROUGH IS NOT PINNED HERE, ON PURPOSE.  DoAuto2 case 1
//  also falls through (into case 2, golden :433) but -- unlike the two pinned in
//  [4]/[5] -- that fall-through is behaviourally INERT and therefore not
//  observable: case 1's only action is `DoAuto2Delay.SetMSAndOn(1000); Task=2;`
//  and case 2's only action is gated on `DoAuto2Delay.Off()`, which case 1 just
//  re-armed to 1000 ms, so case 2's body can never do anything in that same tick.
//  Adding a `break;` after golden :432 would be unobservable from outside.  Stated
//  here rather than asserted, because a test that cannot fail is worse than none.
// ---------------------------------------------------------------------------
static void test_drive_auto2()
{
    printf("\n[8] DoAuto2() dispatcher -- 4 scenarios over the documented switch\n");

    // ---- Scenario A ------------------------------------------------------
    setupDummyFeed();
    InitAuto2Task();
    MOT[MMAuto2].ClearTray("test_w7_l1_auto2");
    MOT[MMAuto2_Car].ClearTray("test_w7_l1_auto2");
    MOT[MMTrayY].ClearTray("test_w7_l1_auto2");
    MOT[MTrayX].ClearTray("test_w7_l1_auto2");

    StateSet seenA;
    bool cursorAlwaysSane = true;
    for (int i = 0; i < 200; ++i)
    {
        advanceAuto2Time();
        fAuto2CanSupplyNewTray = true;      // re-arm the gate (see banner)
        DoAuto2();
        seenA.insert(iAutoAuto2Task);
        if (!auto2CursorSane(iAutoAuto2Task)) cursorAlwaysSane = false;
    }
    static const int wantA[] = {1, 2, 3, 20, 30, 40, 50, 60, 70, 100, 150};
    const int nWantA = (int)(sizeof(wantA)/sizeof(wantA[0]));
    CHECK(cursorAlwaysSane, "DoAuto2 cursor stayed within the documented state set");
    CHECK(covers(seenA, wantA, nWantA),
          "scenario A trajectory covers 1,2,3,20,30,40,50,60,70,100,150");
    printSeen("DoAuto2 scenario A", seenA);
    if (!covers(seenA, wantA, nWantA)) printMissing("DoAuto2 scenario A", seenA, wantA, nWantA);

    // ---- Scenario B: case 100 -> 200 feed arm -----------------------------
    setupDummyFeed();
    InitAuto2Task();
    InitLoadNewAuto2TrayToCarTask();
    MOT[MMAuto2].ClearTray("test_w7_l1_auto2");
    MOT[MMAuto2_Car].ClearTray("test_w7_l1_auto2");
    MOT[MMTrayY].ClearTray("test_w7_l1_auto2");
    MOT[MTrayX].ClearTray("test_w7_l1_auto2");
    simSensorOff(SnAuto2PreDete);           // let the fed tray take the lifter walk
    simSensorOff(SnAuto2_Tray_Car);

    StateSet seenB;
    bool reached200 = false;
    for (int i = 0; i < 400 && !reached200; ++i)
    {
        advanceAuto2Time();
        fAuto2CanSupplyNewTray = true;
        // Model the tray being consumed off the stage: case 100 only takes its
        // feed arm while the stage is empty (golden :656-662 guard).
        if (iAutoAuto2Task == 100)
            MOT[MMAuto2].ClearTray("test_w7_l1_auto2");
        DoAuto2();
        seenB.insert(iAutoAuto2Task);
        if (iAutoAuto2Task == 200) reached200 = true;
    }
    CHECK(reached200, "scenario B reached case 200 (case-100 feed arm, golden :654-655)");
    // Let case 200 run DoLoadNewAuto2TrayToCar to completion.
    for (int i = 0; i < 200 && iReceiveAuto2Tray == 0; ++i)
    {
        advanceAuto2Time();
        fAuto2CanSupplyNewTray = true;
        DoAuto2();
        seenB.insert(iAutoAuto2Task);
    }
    CHECK(iReceiveAuto2Tray == 1,
          "case 200 consumed DoLoadNewAuto2TrayToCar()==true -> iReceiveAuto2Tray=1 (golden :673)");
    printSeen("DoAuto2 scenario B", seenB);

    // ---- Scenario C: case 100 -> 300 recycle arm --------------------------
    setupDummyFeed();
    InitAuto2Task();
    InitAuto2TrayToRearTask();
    MOT[MMAuto2].ClearTray("test_w7_l1_auto2");
    MOT[MMAuto2_Car].ClearTray("test_w7_l1_auto2");
    MOT[MMTrayY].ClearTray("test_w7_l1_auto2");
    MOT[MTrayX].ClearTray("test_w7_l1_auto2");

    StateSet seenC;
    bool reached300 = false;
    for (int i = 0; i < 400; ++i)
    {
        advanceAuto2Time();
        fAuto2CanSupplyNewTray = true;
        // A tray sitting on the car at case 100 selects the recycle arm.
        if (iAutoAuto2Task == 100)
            MOT[MMAuto2_Car].SetTray(NULL_IC, "test_w7_l1_auto2");
        DoAuto2();
        seenC.insert(iAutoAuto2Task);
        if (iAutoAuto2Task == 300) reached300 = true;
    }
    CHECK(reached300, "scenario C reached case 300 (case-100 recycle arm, golden :649-650)");
    CHECK(seenC.find(50) != seenC.end(),
          "case 300 consumed DoAuto2TrayToRear()==true -> Task=50 (golden :680)");
    printSeen("DoAuto2 scenario C", seenC);

    // ---- Scenario D: case 60 -> 65 re-check + receive hand-off ------------
    setupDummyFeed();
    InitAuto2Task();
    MOT[MMAuto2].ClearTray("test_w7_l1_auto2");
    MOT[MMAuto2_Car].ClearTray("test_w7_l1_auto2");
    MOT[MMTrayY].SetTray(NULL_IC, "test_w7_l1_auto2");      // tray, no IC
    MOT[MTrayX].ClearTray("test_w7_l1_auto2");
    CHECK(MOT[MMTrayY].fHasTray && MOT[MMTrayY].HasIC() == false,
          "scenario D precondition: MMTrayY holds an empty tray");

    StateSet seenD;
    bool reached65 = false;
    for (int i = 0; i < 400 && !reached65; ++i)
    {
        advanceAuto2Time();
        fAuto2CanSupplyNewTray = true;
        // case 60 takes the re-check arm only when the stage HAS a tray, and the
        // hand-off at 65 additionally wants a tray on the car (golden :610-626).
        if (iAutoAuto2Task == 60)
        {
            MOT[MMAuto2].SetTray(NULL_IC, "test_w7_l1_auto2");
            MOT[MMAuto2_Car].SetTray(NULL_IC, "test_w7_l1_auto2");
        }
        DoAuto2();
        seenD.insert(iAutoAuto2Task);
        if (iAutoAuto2Task == 65) reached65 = true;
    }
    CHECK(reached65, "scenario D reached case 65 (case-60 re-check arm, golden :603-604)");
    // one more tick to run the case-65 body
    advanceAuto2Time();
    fAuto2CanSupplyNewTray = true;
    DoAuto2();
    seenD.insert(iAutoAuto2Task);
    CHECK(iReceiveAuto2Tray == 2 && iAuto2ReceiveTask == 200,
          "case 65 fired the receive hand-off (iReceiveAuto2Tray=2, iAuto2ReceiveTask=200, golden :624-625)");
    printSeen("DoAuto2 scenario D", seenD);

    // leave the SM in a clean state for [9]
    setupDummyFeed();
    InitAuto2Task();
    InitAuto2ReceiveTask();
    MOT[MMAuto2].ClearTray("test_w7_l1_auto2");
    MOT[MMAuto2_Car].ClearTray("test_w7_l1_auto2");
    MOT[MMTrayY].ClearTray("test_w7_l1_auto2");
}

// ---------------------------------------------------------------------------
//  [9] fAuto2CanSupplyNewTray gate: DoAuto2() no-ops while closed
// ---------------------------------------------------------------------------
static void test_supply_gate()
{
    printf("\n[9] fAuto2CanSupplyNewTray gate\n");

    setupDummyFeed();
    InitAuto2Task();
    iReceiveAuto2Tray      = 0;
    fAuto2CanSupplyNewTray = false;   // gate closed

    int before = iAutoAuto2Task;
    for (int i = 0; i < 20; ++i)
    {
        advanceAuto2Time();
        DoAuto2();
    }
    CHECK(iAutoAuto2Task == before,
          "DoAuto2 no-ops while fAuto2CanSupplyNewTray==false (early return, golden :425-426)");
}

// ---------------------------------------------------------------------------
//  [10] fAGV satellite stub still links (DoAuto2 case 70 derefs fAGV->IsATK_AMR())
// ---------------------------------------------------------------------------
static void test_fagv_stub()
{
    printf("\n[10] FormsFacade fAGV satellite stub (DoAuto2 case 70 dependency)\n");
    CHECK(fAGV != 0,                 "fAGV is constructed (non-null)");
    CHECK(fAGV->IsATK_AMR() == false, "fAGV->IsATK_AMR()==false (offline DUMMY non-AMR path)");
}

int main()
{
    printf("==============================================================\n");
    printf(" W7-L1 -- asendic_Auto2 (Auto2-tray stack feed) SM test\n");
    printf("==============================================================\n");

    test_inits();
    test_virtual_time_seam();
    test_load_new_auto2_full_lifter_walk();
    test_load_new_auto2_case1_shortcircuit();
    test_tray_to_front_long_path();
    test_tray_to_rear_path();
    test_unload_to_stack_direct();
    test_receive_chain_converges();
    test_drive_auto2();
    test_supply_gate();
    test_fagv_stub();

    printf("\n--------------------------------------------------------------\n");
    printf(" RESULT: %d passed, %d failed\n", g_pass, g_fail);
    printf("--------------------------------------------------------------\n");
    return (g_fail == 0) ? 0 : 1;
}
