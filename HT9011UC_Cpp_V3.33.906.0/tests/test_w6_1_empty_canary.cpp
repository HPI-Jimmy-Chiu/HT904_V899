// =============================================================================
//  test_w6_1_empty_canary.cpp  --  W6.1 CANARY test (Empty-tray SM)
//
//  Translation wave: W6.1 (CANARY)
//  Author: AI(W6.0-SCAFFOLD) 20260626
//  AI(W906-W6.1-EMPTYfix) 20260801: rewritten.  See "WHAT CHANGED" below.
//
//  PROOF OBJECTIVE
//  ---------------
//  asendic_Empty.cpp is the first ROOT state machine driven over the W6 substrate
//  (Sim HAL: Cylinder[]/MOT[]/Sen[]/SW[]) + the frozen csystem.h predicate
//  interface + the FormsFacade fAGV satellite + the canary-support scaffold.
//  It is also the REFERENCE FILE the rest of the asendic_* family is modelled on,
//  so this test is held to the family's standard: every assertion here must be
//  able to fail, and the ones that cannot are named in the NOT-COVERED register
//  instead of being left to look like coverage.
//
//  WHAT CHANGED -- AI(W906-W6.1-EMPTYfix) 20260801
//  -----------------------------------------------
//  (1) TWO TAUTOLOGIES REMOVED.  The committed baseline contained
//        CHECK(done || iLoadNewEmptyTrayToCarTask != 1 || steps == 200, ...)
//        CHECK(reachedDispatch || iAutoEmptyTask == 1, ...)
//      Both wrote a bounded loop's own termination condition into the assertion's
//      disjunction, so both were unconditionally true.  The first was actively
//      HIDING A FAILURE: the loop was a tight spin with no time source, so
//      DoLoadNewEmptyTrayToCar() never got past case 400 (`EmptyDelay.Off()`,
//      a 1000 ms wait) -- the baseline run printed
//        (returned true=0 after 200 steps, final cursor=400)
//      i.e. the SM did NOT converge, while the file header claimed acceptance
//      criterion [3] "DoLoadNewEmptyTrayToCar() converges (returns true)".  Fixed
//      by giving the test a VIRTUAL-TIME SEAM (below) and asserting bare `done`.
//  (2) A THIRD, VACUOUS assertion removed: CHECK(iLoadNewEmptyTrayToCarTask >= 1,
//      "cursor is a valid state").  Every Task assignment in asendic_Empty.cpp is
//      a positive literal and the cursor initialises to 1, so no mutation of any
//      transition could make `>= 1` false.  Replaced with set membership in the
//      SM's documented state set, which a re-targeted transition does break.
//  (3) COVERAGE HOLES CLOSED (the W7-L1/Auto2 lesson applied to the canary):
//      conveyor SW[] polarity, cylinder Push()/Pop() direction, and MOT[] tray
//      bookkeeping were all unasserted -- the suite stayed green with any of them
//      inverted.  Now [3]/[4]/[5] assert each one, mutation-proven.
//      Worse than "unasserted": DoAutoEmpty1() -- the Empty1 stack the old header
//      claimed this test drove "over SW[SwACEmpty1CCW]/MOT[MMEmpty1]" -- NEVER
//      LEFT case 1.  Its case-1 entry needs a tray on MMEmpty1 or an Empty1 stack
//      sensor reading on, and the old fixture provided neither.  MEASURED: the
//      old drive loop visited state set {1} and left SW[SwACEmpty1CCW].OutValue
//      untouched at false.  [3] now seeds the tray and drives the real cycle.
//  (4) BOTH `#if 0` GATES IN asendic_Empty.cpp RETIRED (verified, not assumed --
//      see [7]/[8] and the file-head note in asendic_Empty.cpp).
//  (5) The fixture is now HERMETIC.  The baseline leaked sim-sensor state between
//      sub-tests and sub-test [4] had to hand-disable two sensors to undo what
//      [2]/[3] had left enabled.  emptyFixture() now resets every Sen[] this SM
//      reads plus every MOT[] tray it touches, so sub-tests can run in any order.
//
//  VIRTUAL-TIME SEAM
//  -----------------
//  TQPF_Timer is a QueryPerformanceCounter wall clock.  A tight test loop takes
//  microseconds per iteration, so ANY SetMSAndOn(1000) wait blocks forever in
//  test time -- which is exactly how the baseline's case-400 stall arose.  Same
//  seam as the sibling test_w7_l1_auto2.cpp: extern the file-scope timers of
//  asendic_Empty.cpp and re-arm the PROGRESS timers with SetMSAndOn(0) once per
//  tick, so one loop iteration == one tick of virtual time.  The JAM WATCHDOGS
//  (hEmptyTrayToFront 20 s, hEmptyTrayToRear 20 s, WaitTrayDelay 15 s,
//  EmptyMidCylinTime 5 s, EmptyReadTrayID 7 s) are deliberately NEVER advanced,
//  so no alarm arm fires spuriously.  hEmptyTrayToRear is dual-role (it is also
//  the case-450 settle timer), so instead of advancing it the operator-tunable
//  Ld_UldDelayTime settle delays are pinned to 0 s -- they are machine settings,
//  not SM logic.
//
//  SENSOR SEAM
//  -----------
//  Sen[i] with Enable==false reads BOTH IsOn()==false and IsOff()==false
//  ("unknown").  With Enable==true and ISABase==eISABase the raw-port stub
//  (mysensor.cpp's static IOInputBit) always returns false, so Type picks the
//  polarity: TYPE_A(1) => IsOff()==true / IsOn()==false ("off");
//  TYPE_B(0) => IsOn()==true / IsOff()==false ("on").  Verified in this run:
//  TYPE_B -> IsOn=1 IsOff=0; TYPE_A -> IsOn=0 IsOff=1; disabled -> 0/0.
//
//  NOT COVERED (stated so nobody reads coverage into silence)
//  ----------------------------------------------------------
//  Fixture invariants, all held by emptyFixture() for the whole file:
//    (i)   LastSet.iRealDummy == DUMMY (0) in every sub-test.
//    (ii)  Every Sen[] this SM reads starts "unknown".  The ONLY sensor ever
//          raised is SnEmptyIsFull, and only to "on", and only in [7] and [8].
//    (iii) bHandlerPause == false, bEmptyPause == false, bOCRRunTime == false.
//    (iv)  TRAY_ARM_MODE != eUnderCoveyor, DOUBLE_BELT_MODE == 0,
//          SUPPORT_2_EMPTY_EMPTY == false, USE_COLOR_TRAY_SENSOR == 0,
//          IniConfig.bG11ASEReport == false, CosFunction.bSpecialP24 == false.
//    (v)   ShowErrorMessage() (canary_support.cpp) always answers K_RETRY, so
//          every K_SKIP arm is unreachable even where its alarm is reached.
//
//  Consequently NOT covered:
//    * DoLoadNewEmptyTrayToCar case 20 (the EmptyMidCylinTime / JAM1006
//      separation-cylinder arm) and the MES1021 alarm.  Both live in case 1's
//      `else`, which is dead by (i): the guard's `|| LastSet.iRealDummy==DUMMY`
//      disjunct always wins.  MEASURED: 20 is absent from the [4] trajectory.
//    * DoLoadNewEmptyTrayToCar cases 52/53 -- the bG11ASEReport tray-ID read
//      (RespondASECom / EmptyReadTrayID / WAR16335).  Dead by (iv).  MEASURED:
//      52 and 53 absent from the [4] trajectory.
//    * DoLoadNewEmptyTrayToCar case 420's `else if(EmptyDelay.Off())` JAM1013
//      arm: the preceding `if` carries a `|| LastSet.iRealDummy==DUMMY` disjunct
//      that always wins by (i).
//    * Cylinder[C_EmptyLoaderZ_Select] IS NEVER ACTUATED.  Its Push() (case 100)
//      and Pop() (case 300) both sit behind a DUMMY short-circuit, so by (i) the
//      SM takes the DUMMY arm and the select cylinder is never commanded.  The
//      Push()/Pop() direction assertions in [5] are about C_Empty_Fix only.
//    * DoLoadNewEmptyTrayToCar case 200's bEmptyPause split-pause arm, dead by
//      (iii).
//    * DoAutoEmpty cases 70 and 150.  MEASURED: absent from a 400-tick
//      trajectory under this fixture (the SM settles into the
//      1-20-30-40-50-60-65-100-200-300 cycle).
//    * DoAutoEmpty's CUSTOMER_CODE==CC_SCC early return, the CosFunction
//      .bSpecialP24 P24 branch and both P25 no-supply returns -- all gated off by
//      (iv) and by IniConfig.bP25EmptyColorNoSuppleAutoNoLoadEmpty==false.
//    * THE RECEIVE CHAIN.  DoEmptyTrayToFront and DoUnLoadNewEmptyToStack are
//      NOT DRIVEN AT ALL, and DoAutoEmptyReceive is entered only at case 300
//      ([8] pins the cursor there).  Cases 1/100/200 of DoAutoEmptyReceive are
//      never reached because iReceiveEmptyTray stays 0 everywhere else.
//      MEASURED with a probe over sub-test [2]'s 400-tick drive: the
//      DoEmptyTrayToFront, DoUnLoadNewEmptyToStack and DoAutoEmptyReceive
//      cursors each visited exactly {1} -- and since DoEmptyTrayToFront case 1
//      FALLS THROUGH to case 100, a cursor still reading 1 proves the function
//      was never called at all, not merely that it did not advance.  This is the
//      single largest gap in the file and it is deliberate: driving the receive
//      chain needs a TrayArm-side fixture that belongs to the W7 work, not to
//      the canary.  Reported again at the end of main() from the live cursors.
//    * DoEmptyTrayToRear IS driven, but only partly.  It is reached transitively
//      from DoAutoEmpty case 300, and [2] additionally asserts its trajectory.
//      MEASURED: cases 1/200/450/460/500/510 are walked; cases 100, 300, 400,
//      420, 430 and 470 are NOT -- 100/430 need TRAY_ARM_MODE==eUnderCoveyor or
//      DOUBLE_BELT_MODE==1 (excluded by (iv)); 420/470 are their follow-ups;
//      300/400 are the SenEmptyFixCyPush re-fix arm, unreachable because case
//      200 leaves via the DUMMY arm by (i).  The JAM1001/JAM1003/WAR1051 alarms
//      on those arms are dead with them.
//    * DoAutoEmpty1's real-machine arms (the JAM1001 retry/skip, the
//      WaitTrayDelay 15 s timeout, the bRetry re-trigger) -- all behind
//      `LastSet.iRealDummy==DUMMY` short-circuits, dead by (i).
//    * MyMessageBox->Close() itself.  [8] sets MyMessageBox->Visible=true so the
//      `if` DOES fire and the call IS made, but TMyMessageBoxShim::Close() is an
//      empty body (acatchtray_shims.cpp) that does not even clear Visible, so
//      there is nothing to observe.  MEASURED: MyMessageBox->Visible is still
//      true after the block runs.  [8] therefore asserts the ENCLOSING pre-alarm
//      block instead (iUnLoaderCount / RunInfo.iPreAlarmPosition), which proves
//      the retired gate's region executes; it does not prove Close() ran.
//    * All #ifdef SOFT_SIMULTE arms (SOFT_SIMULTE is not defined in this build).
//    * REAL LIFTER BEHAVIOUR.  CylinderUp/CylinderMiddle/CylinderLower are SIM
//      bodies in this tree (asendic.cpp says so in its own file head): they
//      command Cylinder[] and return true, they are not golden's lifter state
//      machines.  [4]'s lifter assertions prove the COMMAND DIRECTION at each
//      call site (up at case 60, down at case 410) and nothing about physics.
//
//  Offline: no vendor SDK, no hardware, no window.
// =============================================================================
#include "asendic_Empty.h"
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
#include "canary_support.h"     // LastSet, ShowErrorMessage, WhichAutoNeedTray
#include "FormsFacade.h"        // fAGV
#include "SECSGEM/SecsEventType.h"      // SECS_EVENT      (retired gate [7])
#include "SECSGEM/SecsEventReport.h"    // EventReport sim observables
#include "acatchtray_shims.h"           // MyMessageBox    (retired gate [8])
// AI(W906-W7-L1-Wave3) 20260802: CylinderUp/Middle/Lower are no longer Sim bodies
// that return true -- asendic.cpp now carries golden's real closed-loop lifter
// state machines, which wait on Cylinder[].OnStatus()/OffStatus().  This header
// supplies the physical stack that answers them.  See its banner for the sensor
// law and for why no static fixture can replace it.
#include "w3_cylinder_plant.h"
#include <cstdio>
#include <set>

// [W6.3] bAutoNeedTray[] is owned by the TrayArm engine (acatchtray.cpp); it has
// no shared-header extern, so forward-declare it here to drive WhichAutoNeedTray()
// into its 0-return path for the "nobody needs a tray" guard check below.
extern bool bAutoNeedTray[MAX_AUTO_TRAY];

// InitLoadNewEmptyTrayToCarTask is a file-scope (external-linkage) function in
// asendic_Empty.cpp that the golden header does NOT export (the golden calls it
// cross-module via an implicit decl).  asendic_Empty.h is mirrored VERBATIM, so
// we forward-declare it here exactly as the golden does at the call site.
extern void InitLoadNewEmptyTrayToCarTask();

// ---------------------------------------------------------------------------
//  VIRTUAL-TIME SEAM -- the file-scope TQPF_Timer objects of asendic_Empty.cpp.
//  PROGRESS timers (re-armed to 0 ms every tick):
// ---------------------------------------------------------------------------
extern TQPF_Timer EmptyDelay;                   // golden asendic_Empty.cpp:34
extern TQPF_Timer DoAutoEmptyDelay;             // golden asendic_Empty.cpp:624
extern TQPF_Timer CheckEmptyTrayDelay;          // golden asendic_Empty.cpp:625
extern TQPF_Timer hEmptyTrayToFrontdelay;       // golden asendic_Empty.cpp:280
extern TQPF_Timer hEmptyTrayToFrontForDummy;    // golden asendic_Empty.cpp:281
extern TQPF_Timer EmptyUnloadDelay;             // golden asendic_Empty.cpp:282
extern TQPF_Timer hEmptyTrayToRearForDummy;     // golden asendic_Empty.cpp:403
extern TQPF_Timer hEmptyUnderCoveyorDelay;      // golden asendic_Empty.cpp:404
extern TQPF_Timer DoUnLoadNewEmptyToStackDelay; // golden asendic_Empty.cpp:895
extern TQPF_Timer DoAutoEmpty1Delay;            // golden asendic_Empty.cpp:1166
//  JAM WATCHDOGS -- declared here for documentation; deliberately NEVER advanced:
//    extern TQPF_Timer EmptyMidCylinTime;      // golden asendic_Empty.cpp:35   (5 s)
//    extern TQPF_Timer EmptyReadTrayID;        // golden asendic_Empty.cpp:36   (7 s)
//    extern TQPF_Timer hEmptyTrayToFront;      // golden asendic_Empty.cpp:279  (20 s)
//    extern TQPF_Timer hEmptyTrayToRear;       // golden asendic_Empty.cpp:402  (20 s, dual-role)
//    extern TQPF_Timer WaitTrayDelay;          // golden asendic_Empty.cpp:1167 (15 s)

static void advanceEmptyTime()
{
    EmptyDelay.SetMSAndOn(0);
    DoAutoEmptyDelay.SetMSAndOn(0);
    CheckEmptyTrayDelay.SetMSAndOn(0);
    hEmptyTrayToFrontdelay.SetMSAndOn(0);
    hEmptyTrayToFrontForDummy.SetMSAndOn(0);
    EmptyUnloadDelay.SetMSAndOn(0);
    hEmptyTrayToRearForDummy.SetMSAndOn(0);
    hEmptyUnderCoveyorDelay.SetMSAndOn(0);
    DoUnLoadNewEmptyToStackDelay.SetMSAndOn(0);
    DoAutoEmpty1Delay.SetMSAndOn(0);
}

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as test_lane_io_sim.cpp)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                   \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---------------------------------------------------------------------------
//  SIM SENSOR SEAM -- see banner note.
// ---------------------------------------------------------------------------
static void simSensorOn(int idx)        // IsOn()==true, IsOff()==false
{
    Sen[idx].Enable  = true;
    Sen[idx].Type    = TYPE_B;          // cmydef.cpp: TYPE_B==0
    Sen[idx].ISABase = eISABase;        // -> mysensor.cpp raw-port stub (false)
}
static void simSensorUnknown(int idx)   // IsOn()==false AND IsOff()==false
{
    Sen[idx].Enable = false;
}

// Every Sen[] index asendic_Empty.cpp reads (enumerated from the source, not
// from memory).  Reset to "unknown" by emptyFixture() so sub-tests are hermetic.
static const int kEmptySensors[] = {
    SenEmptySelectHasTray, SenEmptyCCWDete, SenEmptyCarHasTray, SnEmptyIsFull,
    SenEmptyHasTray,       SenEmptyCWDete,  SnEmptySeparateHasTray,
    SenEmptyFixCyPush,     SnEmptyColorTrayDetect, SnEmptyIsPreAlarm,
    SenEmpty1HasTray,      SenEmpty1CarHasTray
};
static const int kEmptySensorCount =
    (int)(sizeof(kEmptySensors) / sizeof(kEmptySensors[0]));

// ---------------------------------------------------------------------------
//  Documented state sets.  A cursor outside its set means corruption -- and,
//  unlike the ">= 1" check this replaces, a RE-TARGETED transition breaks it.
// ---------------------------------------------------------------------------
static bool autoEmptyCursorSane(int t)          // DoAutoEmpty (golden :685-885)
{
    switch (t)
    {
        case 1: case 20: case 30: case 40: case 50: case 60: case 65:
        case 70: case 100: case 150: case 200: case 300:
            return true;
        default:
            return false;
    }
}
static bool loadNewEmptyCursorSane(int t)       // DoLoadNewEmptyTrayToCar (golden :55-239)
{
    switch (t)
    {
        case 1: case 20: case 50: case 52: case 53: case 60:
        case 100: case 200: case 300: case 400: case 410: case 420:
            return true;
        default:
            return false;
    }
}
static bool autoEmpty1CursorSane(int t)         // DoAutoEmpty1 (golden :1181-1237)
{
    return (t == 1 || t == 100 || t == 200);
}

// ---------------------------------------------------------------------------
//  Trajectory recorder helpers
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
//  HERMETIC FIXTURE -- the DUMMY tray-feed configuration the Empty SM expects.
//  Resets every Sen[] the SM reads and every MOT[] tray it touches, so sub-test
//  order cannot change any result.  The three Ld_UldDelayTime settle delays are
//  pinned to 0 s: they are operator-tunable machine settings (Ld_ULd form), not
//  SM logic, and pinning them lets case 450/460 settle without advancing the
//  dual-role 20 s hEmptyTrayToRear watchdog.
// ---------------------------------------------------------------------------
static void emptyFixture()
{
    for (int k = 0; k < kEmptySensorCount; ++k)
        simSensorUnknown(kEmptySensors[k]);

    LastSet.iRealDummy             = DUMMY;       // dummy feed (no real IC/tray sensors)
    fEmptyCanSupplyNewTray         = true;        // allow supply
    iReceiveEmptyTray              = 0;           // not in receive mode
    bIsCatchingFromBuffer          = false;
    bIsPlacingToBuffer             = false;
    bEmptyPause                    = false;
    bHandlerPause                  = false;
    bOCRRunTime                    = false;

    CosFunction.bSpecialP24                  = false;
    CosFunction.bUseEmptyColorTrayPreAlarm   = false;
    IniConfig.bP25EmptyColorNoSuppleAutoNoLoadEmpty = false;
    IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray = false;
    IniConfig.bA65_BundleIDList              = false;
    IniConfig.bP32EmptyColorTrayPreAlarm     = false;
    IniConfig.bG11ASEReport                  = false;
    IniConfig.bEnable_SECS_GEM               = false;
    IniConfig.bP04ColorIsEmptyUnloader       = false;
    TrayForm.bEnableAMR                      = false;
    TrayForm.bColorTray                      = false;

    Ld_UldDelayTime.LD_TrayArrivalDely = 0;
    Ld_UldDelayTime.ULD_LiftDownDelay  = 0;
    Ld_UldDelayTime.ULD_TrayBackDelay  = 0;
    // AI(W906-W7-L1-Wave3) 20260802: the three delays golden's real
    // CylinderUp/Middle/Lower read (asendic.cpp :424, :344/:375, :475/:504).
    Ld_UldDelayTime.LD_BeforeDownDelay    = 0;
    Ld_UldDelayTime.LD_StackMiddLockDelay = 0;
    Ld_UldDelayTime.LD_LiftDownDelay      = 0;

    // AI(W906-W7-L1-Wave3) 20260802: wire the Empty tray-group lifter as a real
    // machine wires it (Enable + distinct Sim IO addresses for the output and the
    // two position sensors + non-zero alarm windows) and reset the lifter cursors
    // the way golden main.cpp:9159-9160 does at FormShow.  Without this the real
    // CylinderUp can never see C_Empty_Up's top sensor make and the SM parks.
    w3::Forget();
    w3::WireLifter(C_Empty_Up, C_Empty_Middle, 12);
    w3::ResetCursors();

    AUTO_EMPTY_COLOR      = 1;      // 6-lane auto track
    SUPPORT_2_EMPTY_EMPTY = false;
    TRAY_ARM_MODE         = 0;      // not eUnderCoveyor
    DOUBLE_BELT_MODE      = 0;
    USE_COLOR_TRAY_SENSOR = 0;

    MOT[MMEmpty].ClearTray("test_w6_1");
    MOT[MMEmpty_Car].ClearTray("test_w6_1");
    MOT[MMEmptyZ].ClearTray("test_w6_1");
    MOT[MMTrayY].ClearTray("test_w6_1");
    MOT[MTrayX].ClearTray("test_w6_1");
    MOT[MMEmpty1].ClearTray("test_w6_1");
}

// ---------------------------------------------------------------------------
//  [1] Init* reset their cursor ints to 1
// ---------------------------------------------------------------------------
static void test_inits()
{
    printf("\n[1] Init* reset task cursors to 1\n");

    iAutoEmptyTask         = 999;
    iAutoEmptyReceiveTask  = 999;
    iLoadNewEmptyTrayToCarTask = 999;
    InitAutoEmptyTask();
    InitAutoEmptyReceiveTask();
    InitLoadNewEmptyTrayToCarTask();
    CHECK(iAutoEmptyTask == 1,        "InitAutoEmptyTask()        -> iAutoEmptyTask==1");
    CHECK(iAutoEmptyReceiveTask == 1, "InitAutoEmptyReceiveTask() -> iAutoEmptyReceiveTask==1");
    CHECK(iLoadNewEmptyTrayToCarTask == 1,
          "InitLoadNewEmptyTrayToCarTask() -> iLoadNewEmptyTrayToCarTask==1");
}

// ---------------------------------------------------------------------------
//  [2] DoAutoEmpty() dispatch: the cursor must WALK a specific trajectory.
//
//  Replaces the baseline's CHECK(reachedDispatch || iAutoEmptyTask == 1, ...),
//  which was unconditionally true: reachedDispatch was set on the last iteration
//  whenever the cursor was off 1, so the two disjuncts partitioned the outcome
//  space.  A trajectory-coverage assertion cannot be satisfied that way -- kill
//  any one transition on the walk and the corresponding state goes missing.
// ---------------------------------------------------------------------------
static void test_drive_auto_empty()
{
    printf("\n[2] DoAutoEmpty() dispatch trajectory in DUMMY\n");

    emptyFixture();
    InitAutoEmptyTask();
    InitAutoEmptyReceiveTask();

    // DoAutoEmpty case 300 hands off to DoEmptyTrayToRear, so its cursor is
    // recorded too -- that sub-SM has no other driver in this file.
    iEmptyTrayToRearTask = 1;

    StateSet seen, rearSeen;
    bool cursorAlwaysSane = true;

    for (int i = 0; i < 400; ++i)
    {
        seen.insert(iAutoEmptyTask);
        rearSeen.insert(iEmptyTrayToRearTask);
        advanceEmptyTime();
        w3::Tick();                 // AI(W906-W7-L1-Wave3) 20260802: DoAutoEmpty
                                    // reaches the real CylinderUp/Middle/Lower
                                    // through DoEmptyTrayToRear; without the
                                    // plant the lifter never reports arrival and
                                    // the rear sub-SM parks short of 500/510.
        DoAutoEmpty();
        if (!autoEmptyCursorSane(iAutoEmptyTask))
            cursorAlwaysSane = false;
    }

    // MEASURED this run under emptyFixture(): the SM settles into this cycle.
    // 70 and 150 are NOT on it -- see the NOT-COVERED register.
    static const int want[] = { 1, 20, 30, 40, 50, 60, 65, 100, 200, 300 };
    static const int nWant  = (int)(sizeof(want) / sizeof(want[0]));
    // MEASURED likewise for the rear sub-SM.  100/300/400/420/430/470 are NOT on
    // this walk -- see the NOT-COVERED register for why each one is excluded.
    static const int wantRear[] = { 1, 200, 450, 460, 500, 510 };
    static const int nWantRear  = (int)(sizeof(wantRear) / sizeof(wantRear[0]));

    CHECK(cursorAlwaysSane,
          "DoAutoEmpty cursor stayed within the documented state set across 400 ticks");
    CHECK(covers(seen, want, nWant),
          "DoAutoEmpty walked 1-20-30-40-50-60-65-100-200-300 (every transition on the feed cycle fired)");
    printSeen("DoAutoEmpty", seen);
    if (!covers(seen, want, nWant))
        printMissing("DoAutoEmpty", seen, want, nWant);

    CHECK(covers(rearSeen, wantRear, nWantRear),
          "DoEmptyTrayToRear (via case 300) walked 1-200-450-460-500-510");
    printSeen("DoEmptyTrayToRear", rearSeen);
    if (!covers(rearSeen, wantRear, nWantRear))
        printMissing("DoEmptyTrayToRear", rearSeen, wantRear, nWantRear);
}

// ---------------------------------------------------------------------------
//  [3] DoAutoEmpty1(): the independent Empty1 stack over SW[SwACEmpty1CCW] and
//      MOT[MMEmpty1].  CONVEYOR SWITCH POLARITY + tray bookkeeping.
//
//  The baseline called DoAutoEmpty1() 500 times and asserted nothing about it.
//  MEASURED: with no tray on MMEmpty1 and the Empty1 sensors unknown, its case-1
//  entry guard never opens -- state set {1}, SW[SwACEmpty1CCW] never touched.
//  Both halves are asserted here: the guard holds shut when it should, and the
//  real cycle runs (and drives the switch in the right direction) when it should.
// ---------------------------------------------------------------------------
static void test_drive_auto_empty1()
{
    printf("\n[3] DoAutoEmpty1() Empty1 stack -- SW[SwACEmpty1CCW] polarity + MOT[MMEmpty1]\n");

    // (a) guard shut: nothing on the Empty1 stack -> cursor pinned at 1, switch untouched.
    emptyFixture();
    iAutoEmpty1ReceiveTask = 1;
    SW[SwACEmpty1CCW].Off();
    {
        bool stayedAt1 = true, switchTouched = false;
        for (int i = 0; i < 64; ++i)
        {
            advanceEmptyTime();
            DoAutoEmpty1();
            if (iAutoEmpty1ReceiveTask != 1)      stayedAt1 = false;
            if (SW[SwACEmpty1CCW].OutValue)       switchTouched = true;
        }
        CHECK(stayedAt1 && !switchTouched,
              "DoAutoEmpty1 case-1 guard holds (no tray, no Empty1 sensor) -- cursor 1, conveyor off");
    }

    // (b) guard open: seed a tray on MMEmpty1 and drive the whole cycle.
    emptyFixture();
    iAutoEmpty1ReceiveTask = 1;
    SW[SwACEmpty1CCW].Off();
    MOT[MMEmpty1].SetTray(NULL_IC, "test_w6_1");
    {
        StateSet seen;
        bool cursorSane = true, convOnSeen = false;
        for (int i = 0; i < 64; ++i)
        {
            seen.insert(iAutoEmpty1ReceiveTask);
            advanceEmptyTime();
            DoAutoEmpty1();
            if (!autoEmpty1CursorSane(iAutoEmpty1ReceiveTask)) cursorSane = false;
            if (SW[SwACEmpty1CCW].OutValue)                    convOnSeen = true;
        }

        static const int want[] = { 1, 100, 200 };
        static const int nWant  = (int)(sizeof(want) / sizeof(want[0]));

        CHECK(cursorSane,        "DoAutoEmpty1 cursor stayed in {1,100,200}");
        CHECK(covers(seen, want, nWant),
              "DoAutoEmpty1 walked 1-100-200 once the Empty1 stack has a tray");
        printSeen("DoAutoEmpty1", seen);

        // POLARITY -- golden :1186 turns the CCW conveyor ON entering the wait,
        // golden :1248 (DUMMY arm of case 200) turns it OFF on completion.
        // Swapping either call inverts one of these two reads.
        CHECK(convOnSeen,
              "SW[SwACEmpty1CCW] was driven ON while the Empty1 stack unloads (golden :1186)");
        CHECK(SW[SwACEmpty1CCW].OutValue == false,
              "SW[SwACEmpty1CCW] is driven OFF again when the cycle completes (golden :1248)");
        // BOOKKEEPING -- golden :1249 clears the Empty1 tray record.
        CHECK(MOT[MMEmpty1].fHasTray == false,
              "MOT[MMEmpty1].ClearTray() ran on completion (tray record released)");
    }
}

// ---------------------------------------------------------------------------
//  [4] DoLoadNewEmptyTrayToCar() CONVERGES under virtual time, walks the whole
//      lifter path, drives the lifter cylinders in the right direction, and does
//      its tray bookkeeping.
//
//  Replaces the baseline's tautology
//      CHECK(done || iLoadNewEmptyTrayToCarTask != 1 || steps == 200, ...)
//  and the vacuous CHECK(iLoadNewEmptyTrayToCarTask >= 1, ...).
//
//  C_Empty_Up / C_Empty_Middle must be Enable==true for the lifter surface to be
//  observable at all: CylinderUp/Middle/Lower (asendic.cpp) guard every On()/Off()
//  with `if(Cylinder[x].Enable)`, so with the default Enable==false the commands
//  are swallowed and bCylinderOn never moves.  (That is why the baseline could
//  not have caught an inverted lifter even if it had looked.)
// ---------------------------------------------------------------------------
static void test_load_new_empty_converges()
{
    printf("\n[4] DoLoadNewEmptyTrayToCar() converges in DUMMY (lifter + Cylinder[]/MOT[] HAL)\n");

    emptyFixture();
    InitLoadNewEmptyTrayToCarTask();

    // AI(W906-W7-L1-Wave3) 20260802: emptyFixture() now does the full realistic
    // wiring (Enable + Sim IO addresses + sensor types + alarm windows) through
    // w3::WireLifter, which also leaves both cylinders retracted -- so the two
    // bare `Enable = true` / `Off()` lines that used to stand here are redundant.
    // They were sufficient while CylinderUp/Middle/Lower were Sim bodies that
    // returned true; they are not sufficient for the real closed-loop bodies.

    TrayID[1][0] = "W6.1-TID";      // golden :256-257 hand it over to slot [1][1]
    TrayID[1][1] = "";
    const int trayTotalBefore = iTrayTotal[1];

    StateSet seen;
    bool done = false, cursorSane = true;
    bool liftUpSeen = false, liftMidSeen = false;
    int  steps = 0;

    // AI(W906-W7-L1-Wave3) 20260802: the bound went 64 -> 128 because the real
    // lifter is a multi-tick closed loop (CylinderUp alone walks 1-50-60-100-200
    // with a one-tick sensor lag at each arrival) where the Sim body returned
    // true on its first call.  This is a BOUND change, not an assertion change:
    // `done` is still asserted bare, and the walk measured below settles well
    // inside the new bound.
    for (steps = 0; steps < 128; ++steps)
    {
        seen.insert(iLoadNewEmptyTrayToCarTask);
        advanceEmptyTime();
        w3::Tick();                 // move the lifter metal + expire LifterTime[][]
        if (DoLoadNewEmptyTrayToCar())
        {
            done = true;
            break;
        }
        if (!loadNewEmptyCursorSane(iLoadNewEmptyTrayToCarTask)) cursorSane = false;
        if (Cylinder[C_Empty_Up].bCylinderOn)     liftUpSeen  = true;
        if (Cylinder[C_Empty_Middle].bCylinderOn) liftMidSeen = true;
    }

    static const int want[] = { 1, 50, 60, 100, 200, 300, 400, 410, 420 };
    static const int nWant  = (int)(sizeof(want) / sizeof(want[0]));

    // The loop is bounded, so `done` alone is a real claim only because the
    // virtual-time seam removes every wall-clock wait: the SM either converges
    // or it does not, and 64 ticks is 8x the measured requirement.
    CHECK(done, "DoLoadNewEmptyTrayToCar() returned true (converged) under virtual time");
    CHECK(cursorSane,
          "DoLoadNewEmptyTrayToCar cursor stayed within its documented state set");
    CHECK(covers(seen, want, nWant),
          "DoLoadNewEmptyTrayToCar walked 1-50-60-100-200-300-400-410-420");
    printSeen("DoLoadNewEmptyTrayToCar", seen);
    if (!covers(seen, want, nWant))
        printMissing("DoLoadNewEmptyTrayToCar", seen, want, nWant);
    CHECK(iLoadNewEmptyTrayToCarTask == 1,
          "DoLoadNewEmptyTrayToCar reset its own cursor to 1 on success (golden :253)");

    // LIFTER DIRECTION -- CylinderUp at case 60 raises the stack, CylinderLower at
    // case 410 drops it.  Both cylinders must have been ON somewhere on the walk
    // and OFF again at the end.
    CHECK(liftUpSeen && liftMidSeen,
          "lifter raised: C_Empty_Up + C_Empty_Middle both commanded ON (CylinderUp, case 60)");
    CHECK(Cylinder[C_Empty_Up].bCylinderOn == false &&
          Cylinder[C_Empty_Middle].bCylinderOn == false,
          "lifter lowered: both commanded OFF again on completion (CylinderLower, case 410)");

    // TRAY BOOKKEEPING -- golden :251-257.
    CHECK(MOT[MMEmpty_Car].fHasTray == true,
          "MOT[MMEmpty_Car].SetTray() ran -- the car now carries the new empty tray (golden :251)");
    CHECK(MOT[MMEmptyZ].fHasTray == false,
          "MOT[MMEmptyZ].ClearTray() ran -- the Z stage released it (golden :252)");
    CHECK(iTrayTotal[1] == trayTotalBefore + 1,
          "iTrayTotal[1] incremented by exactly 1 -- one Empty tray entered the track (golden :255)");
    CHECK(TrayID[1][1] == AnsiString("W6.1-TID") && TrayID[1][0] == AnsiString(""),
          "Empty tray ID handed from slot [1][0] to [1][1] and the read slot cleared (golden :256-257)");

    printf("    (converged in %d ticks, final cursor=%d)\n", steps, iLoadNewEmptyTrayToCarTask);
}

// ---------------------------------------------------------------------------
//  [5] Cylinder Push()/Pop() DIRECTION on C_Empty_Fix.
//
//  Push() extends (bCylinderOn -> true), Pop() retracts (bCylinderOn -> false);
//  both return true under this fixture (no on/off sensor installed, zero delay),
//  so the RETURN VALUE cannot distinguish them and the cursor advances either
//  way.  Only the commanded direction can.  Each arm pins the cursor straight to
//  the case under test and pre-loads the cylinder into the OPPOSITE position, so
//  the observed transition is caused by that one call and nothing else.
// ---------------------------------------------------------------------------
static void test_cylinder_push_pop_direction()
{
    printf("\n[5] Cylinder[C_Empty_Fix] Push()/Pop() direction\n");

    // (a) DoAutoEmpty case 30 pops the fix cylinder (golden :726-728).
    emptyFixture();
    iAutoEmptyTask = 30;
    Cylinder[C_Empty_Fix].Enable = true;
    Cylinder[C_Empty_Fix].Push();                       // pre-load EXTENDED
    CHECK(Cylinder[C_Empty_Fix].bCylinderOn == true,
          "pre-load: C_Empty_Fix extended before the Pop() under test");
    advanceEmptyTime();
    DoAutoEmpty();
    CHECK(Cylinder[C_Empty_Fix].bCylinderOn == false,
          "DoAutoEmpty case 30 RETRACTS C_Empty_Fix (Pop(), golden :727)");
    CHECK(iAutoEmptyTask == 40,
          "DoAutoEmpty case 30 consumed Pop()==true and stepped 30 -> 40 (golden :728)");

    // (b) DoEmptyTrayToRear case 510 pushes it (golden :610-617), reached through
    //     DoAutoEmpty case 300 (golden :885-888).
    emptyFixture();
    iAutoEmptyTask       = 300;
    iEmptyTrayToRearTask = 510;
    Cylinder[C_Empty_Fix].Enable = true;
    Cylinder[C_Empty_Fix].Pop();                        // pre-load RETRACTED
    MOT[MMEmpty_Car].SetTray(NULL_IC, "test_w6_1");
    CHECK(Cylinder[C_Empty_Fix].bCylinderOn == false,
          "pre-load: C_Empty_Fix retracted before the Push() under test");
    advanceEmptyTime();
    DoAutoEmpty();
    CHECK(Cylinder[C_Empty_Fix].bCylinderOn == true,
          "DoEmptyTrayToRear case 510 EXTENDS C_Empty_Fix (Push(), golden :611)");
    CHECK(MOT[MMEmpty].fHasTray == true && MOT[MMEmpty_Car].fHasTray == false,
          "case 510 moved the tray record car -> Empty (SetTray + ClearTray, golden :613-614)");
    CHECK(iAutoEmptyTask == 1 && iEmptyTrayToRearTask == 1,
          "both cursors reset to 1 after DoEmptyTrayToRear returned true (golden :615, :888)");
}

// ---------------------------------------------------------------------------
//  [6] Supply gate + the case-1 "nobody needs a tray" guard.
//
//  The baseline asserted only "Task stays 1 after the guard".  That is satisfied
//  just as well by the SM doing NOTHING, so it could not distinguish a working
//  guard from a dead dispatcher.  Both arms are now driven from the same fixture:
//  remove the guard's precondition and the cursor MUST advance to 100.
// ---------------------------------------------------------------------------
static void test_supply_gate()
{
    printf("\n[6] fEmptyCanSupplyNewTray gate + case-1 'nobody needs tray' guard\n");

    emptyFixture();
    InitAutoEmptyTask();
    fEmptyCanSupplyNewTray = false;   // gate closed

    int before = iAutoEmptyTask;
    for (int i = 0; i < 20; ++i)
    {
        advanceEmptyTime();
        DoAutoEmpty();
    }
    CHECK(iAutoEmptyTask == before,
          "DoAutoEmpty no-ops while fEmptyCanSupplyNewTray==false (early return)");

    // [W6.3] WhichAutoNeedTray() is no longer a return-0 stub -- the real TrayArm
    // engine (acatchtray.cpp) now owns it.  To exercise the guard faithfully we
    // must first drive the REAL WhichAutoNeedTray() into its 0-return path: it
    // short-circuits to iWhichAutoNeedTray=0 when a Clean-Out is in progress with
    // no IC left under the machine (golden acatchtray.cpp:463-466, re-derived from
    // the cp950 golden in this run).  Restore iCleanOut afterwards.
    const int savedCleanOut = iCleanOut;

    // (a) guard precondition ABSENT (no tray parked on MMTrayY) -> case 1 must
    //     dispatch to 100.  Without this half, the "stays at 1" check below is
    //     indistinguishable from a dead dispatcher.
    emptyFixture();
    InitAutoEmptyTask();
    iCleanOut = 1;
    for (int i = 0; i < MAX_AUTO_TRAY; ++i) bAutoNeedTray[i] = false;
    CHECK(WhichAutoNeedTray() == 0,
          "WhichAutoNeedTray()==0 on the Clean-Out/no-IC path (real engine, golden acatchtray.cpp:463)");
    advanceEmptyTime();
    DoAutoEmpty();
    CHECK(iAutoEmptyTask == 100,
          "DoAutoEmpty case 1 dispatches to 100 when no tray is parked on MMTrayY (golden :700)");

    // (b) RE-BASELINED by PT-W5c (20260809).  This half used to assert
    //     `iAutoEmptyTask == 1`, i.e. that parking a tray on MMTrayY established the
    //     "nobody needs tray" precondition and case 1 held at 1.  That expectation was
    //     calibrated against a SCAFFOLD predicate, not against golden:
    //
    //       * golden csystem.cpp:12548 HasICUnderMachine() tests
    //         `MOT[MMTrayZ].fHasTray || MOT[MMTrayY].fHasTray` -- TRAY presence, not IC
    //         presence -- plus MMPlate1/2.HasIC(), InArmSuck/OutArmSuck.HasIC() and
    //         MInRotateKit/MOutRotateKit.HasIC().
    //       * the retired scaffold (csystem_predicates.cpp) had only
    //         `ShuttleHasIC() || IndexHasIC() || HasICUnderHotPlate()` -- every term above
    //         was missing, so a parked tray read as "nothing under the machine".
    //       * golden acatchtray.cpp:463 gates on `iCleanOut==1 && HasICUnderMachine()==false`.
    //         With the real predicate a parked tray makes that FALSE, so control reaches the
    //         `else` branch, whose first act (acatchtray.cpp:598) is
    //         `bAutoNeedTray[iAuto]=false;` -- it OVERWRITES the flags this fixture pre-set
    //         and recomputes them from real state.
    //
    //     Measured under golden semantics: HasICUnderMachine()==1, WhichAutoNeedTray()==1,
    //     iAutoEmptyTask==100.  So parking a tray does not reach the guard at all -- it
    //     routes to the same dispatch as half (a).  Asserting golden's actual behaviour.
    emptyFixture();
    InitAutoEmptyTask();
    iCleanOut = 1;
    for (int i = 0; i < MAX_AUTO_TRAY; ++i) bAutoNeedTray[i] = false;
    MOT[MMTrayY].SetTray(NULL_IC, "test_w6_1");   // tray present, no IC
    CHECK(MOT[MMTrayY].fHasTray && MOT[MMTrayY].HasIC() == false,
          "fixture: MMTrayY holds an empty tray");
    CHECK(HasICUnderMachine() == true,
          "a parked tray alone makes HasICUnderMachine() true (golden csystem.cpp:12548 "
          "tests fHasTray, NOT HasIC -- the scaffold predicate missed this)");
    advanceEmptyTime();
    DoAutoEmpty();
    CHECK(WhichAutoNeedTray() == 1,
          "with a tray parked the clean-out shortcut is skipped, so the else branch "
          "recomputes bAutoNeedTray[] and Auto 1 asks for a tray (golden acatchtray.cpp:463/598)");
    CHECK(iAutoEmptyTask == 100,
          "DoAutoEmpty case 1 therefore dispatches to 100 here too (golden :700)");
    // NOT COVERED, stated rather than silently lost: golden acatchtray.cpp:700-709's
    // per-Auto `bAutoNeedTray[iAuto]==false -> continue` guard. Reaching it needs a fixture
    // where WhichAutoNeedTray() returns 0 while a tray IS parked, which means making every
    // Auto genuinely not want a tray under real evaluation -- this fixture cannot, because
    // pre-setting the flags is undone by :598. Needs its own fixture; see DEVLOG PT-W5c.

    iCleanOut = savedCleanOut;                     // restore
    MOT[MMTrayY].ClearTray("test_w6_1");
}

// ---------------------------------------------------------------------------
//  [7] RETIRED GATE 1 -- SECS EventReport on the Empty-stack-full path.
//
//  asendic_Empty.cpp used to carry
//      #if 0  // TODO(W7-SECS)
//          EventReport(SECS_EVENT.NoCoverTray_Normal);
//      #endif
//  The gate is gone: SECSGEM/SecsEventReport.h supplies a real, Sim-observable
//  EventReport(unsigned) and SECSGEM/SecsEventType.h supplies SECS_EVENT.  Every
//  target that links ht9045_sm also links ht9045_secsgem (audited: 57 of 57), so
//  the un-gated call resolves everywhere.
//
//  This is the ONE retirement with a behaviour delta -- the gated build silently
//  dropped the event -- so it is asserted BOTH ways: it fires when SECS/GEM is
//  enabled, and it does NOT fire when it is not.
// ---------------------------------------------------------------------------
static void test_secs_gate_retired()
{
    printf("\n[7] retired #if 0 TODO(W7-SECS) gate -- EventReport(NoCoverTray_Normal)\n");

    // (a) SECS/GEM on + Empty stack full -> the event must be reported.
    emptyFixture();
    InitLoadNewEmptyTrayToCarTask();
    ResetSimEventReport();
    IniConfig.bEnable_SECS_GEM = true;
    simSensorOn(SnEmptyIsFull);
    {
        const unsigned long before = g_SimEventReportCount;
        const bool ret = DoLoadNewEmptyTrayToCar();
        CHECK(ret == false && iLoadNewEmptyTrayToCarTask == 1,
              "stack-full path taken: case 1 alarms and returns false with the cursor held at 1");
        CHECK(g_SimEventReportCount == before + 1,
              "exactly one SECS event reported on the Empty-stack-full path");
        CHECK(g_SimLastEventReportCeid == (unsigned)SECS_EVENT.NoCoverTray_Normal,
              "the reported CEID is SECS_EVENT.NoCoverTray_Normal (golden :84)");
    }

    // (b) SECS/GEM off -> the golden guard must suppress it.
    emptyFixture();
    InitLoadNewEmptyTrayToCarTask();
    ResetSimEventReport();
    IniConfig.bEnable_SECS_GEM = false;
    simSensorOn(SnEmptyIsFull);
    {
        const unsigned long before = g_SimEventReportCount;
        DoLoadNewEmptyTrayToCar();
        CHECK(g_SimEventReportCount == before,
              "no SECS event when IniConfig.bEnable_SECS_GEM==false (golden :83 guard)");
    }
}

// ---------------------------------------------------------------------------
//  [8] RETIRED GATE 2 -- MyMessageBox close-before-alarm in the Empty/Color
//      pre-alarm path (DoAutoEmptyReceive case 300).
//
//  acatchtray_shims.h supplies TMyMessageBoxShim *MyMessageBox with the same
//  Visible/Close() surface golden's mymessbox.h exposes here, and it is already
//  linked into ht9045_sm.  Close() is an empty body offline, so this sub-test
//  asserts the ENCLOSING pre-alarm block -- see the NOT-COVERED register.
//
//  bHasEmptyTrayPreAlarm is a function-static latch, so this runs ONCE.
// ---------------------------------------------------------------------------
static void test_msgbox_gate_retired()
{
    printf("\n[8] retired #if 0 TODO(W6.x) gate -- MyMessageBox pre-alarm block\n");

    emptyFixture();
    iUnLoaderCount            = 0;
    RunInfo.iPreAlarmPosition = 0;
    MyMessageBox->Visible     = true;      // make the un-gated `if` actually fire
    CosFunction.bUseEmptyColorTrayPreAlarm = true;
    IniConfig.bP32EmptyColorTrayPreAlarm   = true;
    simSensorOn(SnEmptyIsFull);
    iReceiveEmptyTray     = 1;
    iAutoEmptyReceiveTask = 300;           // pin straight to the pre-alarm case

    DoAutoEmptyReceive();

    CHECK(iUnLoaderCount == 8,
          "pre-alarm block ran: iUnLoaderCount=8 keeps the Handler running (golden :1107)");
    CHECK(RunInfo.iPreAlarmPosition == 8,
          "pre-alarm position tagged as 8 (Empty) (golden :1110)");
    CHECK(iReceiveEmptyTray == 0 && iAutoEmptyReceiveTask == 1,
          "case 300 released the receive state and reset its cursor (golden :1118-1119)");
    // Honest record: Close() is a no-op shim, so Visible is UNCHANGED.  This is
    // the evidence behind the NOT-COVERED entry, not a coverage claim.
    printf("    (MyMessageBox->Visible after the block = %d -- Close() is a no-op shim)\n",
           (int)MyMessageBox->Visible);
    MyMessageBox->Visible = false;         // do not leak the flag to other tests
}

// ---------------------------------------------------------------------------
//  [9] fAGV satellite stub: IsATK_AMR()==false (DUMMY non-AMR branch)
// ---------------------------------------------------------------------------
static void test_fagv_stub()
{
    printf("\n[9] FormsFacade fAGV satellite stub\n");
    CHECK(fAGV != 0,                 "fAGV is constructed (non-null)");
    CHECK(fAGV->IsATK_AMR() == false, "fAGV->IsATK_AMR()==false (offline DUMMY non-AMR path)");
}

// ---------------------------------------------------------------------------
//  [10] Frozen predicate interface links + is conservative in the empty machine
// ---------------------------------------------------------------------------
static void test_predicates_link()
{
    printf("\n[10] csystem.h predicate interface links (canary uses none, but they must link)\n");
    // Not exercised by the canary; assert they link and return the conservative
    // 'no IC' answer (TMyKitSuck grid not yet wired -> false).
    CHECK(ShuttleHasIC()        == false, "ShuttleHasIC()==false (no KitSuck grid yet)");
    CHECK(IndexHasIC()          == false, "IndexHasIC()==false");
    CHECK(TestSocketHasIC()     == false, "TestSocketHasIC()==false");
    CHECK(HasICUnderHotPlate()  == false, "HasICUnderHotPlate()==false");
    CHECK(HasAnyICInMachine()   == false, "HasAnyICInMachine()==false");
}

int main()
{
    printf("==============================================================\n");
    printf(" W6.1 CANARY -- asendic_Empty (Empty-tray stack feed) test\n");
    printf("==============================================================\n");

    test_inits();
    test_drive_auto_empty();
    test_drive_auto_empty1();
    test_load_new_empty_converges();
    test_cylinder_push_pop_direction();
    test_supply_gate();
    test_secs_gate_retired();
    test_msgbox_gate_retired();
    test_fagv_stub();
    test_predicates_link();

    // Evidence for the NOT-COVERED entry on the receive chain.  These two
    // cursors are REPORTED, not asserted, precisely because this test never
    // calls their state machines; silence here would read as coverage.  Both
    // still read 1, and DoEmptyTrayToFront case 1 falls through to case 100, so
    // a cursor of 1 means "never called", not merely "did not advance".
    // (iEmptyTrayToRearTask is NOT in this list -- DoEmptyTrayToRear IS driven,
    //  transitively from DoAutoEmpty case 300, and asserted in [2].)
    printf("\n[NOT COVERED, reported] receive-chain SMs this test never calls:\n");
    printf("    iEmptyTrayToFrontTask=%d  iUnLoadNewEmptyTrayTask=%d\n",
           iEmptyTrayToFrontTask, iUnLoadNewEmptyTrayTask);

    printf("\n--------------------------------------------------------------\n");
    printf(" RESULT: %d passed, %d failed\n", g_pass, g_fail);
    printf("--------------------------------------------------------------\n");
    return (g_fail == 0) ? 0 : 1;
}
