// =============================================================================
//  test_w6_2_outarm_core.cpp  --  W6.2c VERIFY: the OUT-ARM ENGINE core SMs
//
//  Translation wave: W6.2c (OUT-ARM ENGINE -- aoutarm9045.cpp)
//  Author: AI(W6.2c-OUTARM) 20260626
//
//  PURPOSE
//  -------
//  The task-mandated *core engine* verify TU -- the DIRECT mirror of
//  test_w6_2_inarm_core.cpp for the OUT-ARM side.  It proves the out-arm
//  ENGINE's central task state machines + numeric helpers link into ht9045_sm
//  and PUMP over the Sim HAL with NO hardware, asserting:
//
//    (a) a KNOWN Sim HAL state is set (OutArmSuck/FRCarryKit/BRCarryKit items,
//        IniConfig ESD give-way flag, the per-SM entry cursors);
//    (b) the ACTIVE core out-arm task SMs are pumped for N ticks FROM THEIR
//        ENTRY CURSOR;
//    (c) each cursor stays inside the documented switch(Task) state set and the
//        entry / early-out behaviour matches the GOLDEN (golden line cited);
//    (d) the geometry / numeric ORACLE values (hand-derived from the golden
//        formula, float-precision verified) match the translation -- incl. the
//        float->int TRUNCATION contract and the Y-pitch zero-guard.
//
//  Prints PASS/FAIL per case + a final summary; returns non-zero on any FAIL.
//
//  CORE SMs UNDER TEST (ACTIVE in aoutarm9045.cpp -- cursor flow verbatim):
//    [SM1] DoOutArmAdditionalFunction()  cursor iOutArmAdditionalFunctionTask
//          golden aoutarm9045.cpp:2246.  Documented cursor universe:
//          {1,100,10000,20000,30000}.  case 1 runs CheekNeedToDoOutArm-
//          AdditionalFunction() then FALLS THROUGH to case 100 (no break @
//          golden:2255).  Offline: every Rotate/AOI/FixAI selector branch in
//          case 100 is W7-gated out, so case 100 takes the live
//          `else { bResult=true; }` (golden:2284) -- but only AFTER the
//          ACTIVE static stub MoveOutArmToAutoSafe() returns true (it does,
//          aoutarm9045.cpp:157).  ORACLE: from entry Task=1 the FIRST tick
//          advances to 100 and returns true; cursor never leaves the set.
//    [SM2] DoOutArmIonFanGiveWay()  cursor iOutArmIonFanGiveWayTask
//          golden aoutarm9045.cpp:4613.  Documented cursor universe: {1,2,3,5}.
//          Two contracts:
//          (2a) PRE-SWITCH GUARD: IniConfig.bA15_1ESDGiveWayFunction==false ->
//               sets cursor=1 and returns true WITHOUT entering the switch
//               (golden:4615-4619).
//          (2b) ENTRY case 1 (golden:4621): bA15_1ESDGiveWayFunction==true,
//               OutArmSuck.HasRealIC()==false, FRCarryKit.HasIC()==false,
//               BRCarryKit.HasIC()==false -> the inner OR (FR&&OutSHT1InRT ||
//               BR&&OutSHT2InRT) is false -> `else { Task=2; }` (golden:4632).
//               So Task 1->2 (NOT 5).
//
//  NUMERIC / GEOMETRY ORACLES (hand-derived from the golden formula; the
//  golden ChangeToFloatNonPcnt is `template<T> float` -- divides as double then
//  NARROWS to float on return -- verified bit-exact with g++ before coding):
//    [N3] GetOutArmPitch_9045 (golden:157): m=ChangeToFloatNonPcnt(X120-X40,
//         iPitch_Max_minus_Min); r=X40+m*(w-iXpitchMinX3); return (int)r.
//         X40=4000,X120=12000,Max_minus_Min=8000,iXpitchMinX3=4000:
//           w=8000 -> m=1.0 -> r=4000+1.0*4000=8000 -> 8000.
//           w=4000 -> r=4000+1.0*0   =4000          -> 4000 (min teach point).
//    [N4] GetOutArmPitch_9045 TRUNCATION (golden:202 `return (int)r`):
//         X40=4000,X120=12001 (span 8001),Max_minus_Min=8000,iXpitchMinX3=4000,
//         w=5000: m=8001/8000=1.0001250505 -> r=4000+m*1000=5000.1250505 ->
//           (int)5000.125 == 5000 (truncation toward zero, NOT round-to-5000.125
//           and NOT round-to-5001).
//    [N5] GetOutArmPitchY_9045 ZERO-GUARD (golden:213; the denom is
//         (6000-IN_OUT_ARM_Y_PITCH_MIN)).  Force IN_OUT_ARM_Y_PITCH_MIN==6000
//         so the denom is 0 -> ChangeToFloatNonPcnt returns 0.0 -> m=0 ->
//         r=(double)Prod.iOutArmY15Pitch+0 -> return (int)r == iOutArmY15Pitch
//         (no div-by-zero crash).  iOffsetPos defaults -1 (no OutArmOffSet read).
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here ==
//  clean g++ compile/link + the core SMs pump over the Sim HAL without crash +
//  outputs match values hand-derived from the golden formula/cursor logic.
// =============================================================================
#include "aoutarm9045.h"           // engine surface (GetOutArmPitch_9045, the SMs, ...)
#include "aHotPlateSubstrate.h"    // OutArmSuck / FRCarryKit / BRCarryKit
#include "csystem.h"               // OutSHT1InRT / OutSHT2InRT / ShuttleHasIC / IndexHasIC
#include "Motor/mymotor.h"
#include "cprod.h"                 // Prod / IniConfig (via Config.h)
#include "cpublic.h"
#include "cmydef.h"                // iPitch_Max_minus_Min / iXpitchMinX3 / IN_OUT_ARM_Y_PITCH_MIN / bOutArmIonFanGiveWay
#include "canary_support.h"        // LastSet, ShowErrorMessage, WhichAutoNeedTray, ...
#include "FormsFacade.h"
#include <cstdio>

// ---------------------------------------------------------------------------
//  Engine-owned cursor that is NOT on the aoutarm9045.h surface.
//  iOutArmAdditionalFunctionTask IS externed in aoutarm9045.h (line 47);
//  iOutArmIonFanGiveWayTask is engine-internal (defined aoutarm9045.cpp:86) --
//  forward-declare it here (same idiom test_w6_2_inarm_core.cpp uses for the
//  internal shuttle-geometry leaves).
// ---------------------------------------------------------------------------
extern int iOutArmIonFanGiveWayTask;   // golden aoutarm9045.cpp:4607

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as the other W6 verify TUs)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) state sets (golden) --------------------------
static bool addlCursorSane(int t)
{   // golden aoutarm9045.cpp:728-794 (case 1/100 active + gated action cases
    //  10000/20000/30000 -- all in the documented universe)
    switch (t) {
        case 1: case 100: case 10000: case 20000: case 30000: return true;
        default: return false;
    }
}
static bool ionFanCursorSane(int t)
{   // golden aoutarm9045.cpp:1318-1405 (switch(Task) { 1,2,3,5 })
    switch (t) {
        case 1: case 2: case 3: case 5: return true;
        default: return false;
    }
}

int main()
{
    printf("==== W6.2c OutArm CORE engine verify ====\n");

    // -----------------------------------------------------------------------
    //  KNOWN Sim HAL state (shared baseline for the SM pumps)
    // -----------------------------------------------------------------------
    //  Carry kits empty + no real IC in the out-arm grid; per-SM entry
    //  preconditions are (re)set just before each pump below.
    FRCarryKit.ClearAll();
    BRCarryKit.ClearAll();
    OutArmSuck.SetAllToNullIC();         // HasRealIC() -> false, HasIC() -> false
    bOutArmIonFanGiveWay = false;        // golden cmydef.h:5754

    // =======================================================================
    //  PART A -- core out-arm task SMs pumped over the Sim HAL
    // =======================================================================
    printf("[A] core task state machines\n");

    // ---- SM1: DoOutArmAdditionalFunction ----------------------------------
    //  ENTRY (golden:730): case 1 runs the (offline-noop) predicate then FALLS
    //  THROUGH to case 100 (golden:733 no break).  case 100: the ACTIVE static
    //  stub MoveOutArmToAutoSafe() returns true; with all Rotate/AOI/FixAI
    //  branches W7-gated, the live `else { bResult=true; }` runs -> the SM
    //  returns true on the FIRST tick and the cursor settles at 100.
    InitDoOutArmAdditionalFunction();    // golden:702 -> iOutArmAdditionalFunctionTask=1
    CHECK(iOutArmAdditionalFunctionTask == 1,
          "SM1 AdditionalFunction: entry cursor == 1 (golden 704)");
    bool addlRet = false;
    {
        bool sane = true, settled100 = false;
        for (int tick = 0; tick < 64; ++tick) {
            bool r = DoOutArmAdditionalFunction();
            if (tick == 0) addlRet = r;          // first-tick return (entry->100)
            if (!addlCursorSane(iOutArmAdditionalFunctionTask)) { sane = false; break; }
            if (iOutArmAdditionalFunctionTask == 100) settled100 = true;
        }
        CHECK(sane,        "SM1 AdditionalFunction: cursor stays in {1,100,10000,20000,30000}");
        CHECK(addlRet,     "SM1 AdditionalFunction: first tick returns true (golden 764 else-path)");
        CHECK(settled100,  "SM1 AdditionalFunction: fall-through 1->100 (golden 733 no-break)");
        CHECK(iOutArmAdditionalFunctionTask == 100,
              "SM1 AdditionalFunction: settles at Task==100 (no further state)");
    }

    // ---- SM2a: DoOutArmIonFanGiveWay PRE-SWITCH GUARD ---------------------
    //  bA15_1ESDGiveWayFunction==false -> sets cursor=1 and returns true WITHOUT
    //  entering switch (golden:1312-1316).  Pre-seed cursor to a NON-{1} value
    //  to prove the guard FORCES it back to 1.
    IniConfig.bA15_1ESDGiveWayFunction = false;
    iOutArmIonFanGiveWayTask = 3;                // dirty value
    {
        bool r = DoOutArmIonFanGiveWay();
        CHECK(r == true,
              "SM2a IonFanGiveWay: ESD-off pre-switch guard returns true (golden 1315)");
        CHECK(iOutArmIonFanGiveWayTask == 1,
              "SM2a IonFanGiveWay: ESD-off guard forces cursor==1 (golden 1314)");
    }

    // ---- SM2b: DoOutArmIonFanGiveWay ENTRY case 1 -> Task=2 ----------------
    //  bA15_1ESDGiveWayFunction==true, OutArmSuck.HasRealIC()==false,
    //  FRCarryKit/BRCarryKit empty -> case 1 inner OR false -> else Task=2.
    IniConfig.bA15_1ESDGiveWayFunction = true;
    OutArmSuck.SetAllToNullIC();                 // HasRealIC()==false
    FRCarryKit.ClearAll();                       // HasIC()==false
    BRCarryKit.ClearAll();                       // HasIC()==false
    InitDoOutArmIonFanGiveWayTask();             // golden:1302 -> iOutArmIonFanGiveWayTask=1
    CHECK(iOutArmIonFanGiveWayTask == 1,
          "SM2b IonFanGiveWay: entry cursor == 1 (golden 1304)");
    {
        // ONE tick: case 1 evaluates and must move to 2 (NOT 5).
        DoOutArmIonFanGiveWay();
        CHECK(iOutArmIonFanGiveWayTask == 2,
              "SM2b IonFanGiveWay: entry case1 inner-else Task 1->2 (golden 1332)");
    }
    //  Continue pumping: the cursor must remain inside {1,2,3,5} regardless of
    //  how the gated motion stubs resolve (no escape / no crash).
    {
        // restore entry so the pump exercises the full ladder from the top.
        InitDoOutArmIonFanGiveWayTask();
        bool sane = true;
        for (int tick = 0; tick < 64; ++tick) {
            DoOutArmIonFanGiveWay();
            if (!ionFanCursorSane(iOutArmIonFanGiveWayTask)) { sane = false; break; }
        }
        CHECK(sane, "SM2b IonFanGiveWay: cursor stays in {1,2,3,5} (no escape, golden 1318)");
    }

    // =======================================================================
    //  PART B -- numeric / geometry oracles (golden-formula hand-derived)
    // =======================================================================
    printf("[B] numeric / geometry oracles\n");

    // ---- N3: GetOutArmPitch_9045 interpolation endpoints ------------------
    //  Oracle vector: X40=4000, X120=12000, iPitch_Max_minus_Min=8000,
    //  iXpitchMinX3=4000.
    Prod.iOutArmX40Pitch  = 4000;
    Prod.iOutArmX120Pitch = 12000;
    iPitch_Max_minus_Min  = 8000;
    iXpitchMinX3          = 4000;
    CHECK(GetOutArmPitch_9045(8000) == 8000,
          "N3 GetOutArmPitch_9045(w=8000) == 8000 (golden 200: m=1.0)");
    CHECK(GetOutArmPitch_9045(4000) == 4000,
          "N3 GetOutArmPitch_9045(w=4000) == 4000 (min teach point -> X40)");

    // ---- N4: GetOutArmPitch_9045 float->int TRUNCATION --------------------
    //  X120=12001 (span 8001) -> m=1.0001250505 -> r=5000.1250505 ->
    //  (int)==5000 (NOT 5001, NOT 5000.125).
    Prod.iOutArmX120Pitch = 12001;
    CHECK(GetOutArmPitch_9045(5000) == 5000,
          "N4 GetOutArmPitch_9045 truncation: 5000.125 -> 5000 (golden 202 (int)r)");
    // restore the interpolation vector
    Prod.iOutArmX120Pitch = 12000;

    // ---- N5: GetOutArmPitchY_9045 zero-guard ------------------------------
    //  Force the denom (6000-IN_OUT_ARM_Y_PITCH_MIN)==0 so ChangeToFloatNonPcnt
    //  returns 0.0 -> m=0 -> r=Prod.iOutArmY15Pitch (no div-by-zero crash).
    {
        int savedMin = IN_OUT_ARM_Y_PITCH_MIN;
        Prod.iOutArmY15Pitch = 1234;
        Prod.iOutArmY60Pitch = 9999;
        IN_OUT_ARM_Y_PITCH_MIN = 6000;          // denom (6000-6000)==0
        // iOffsetPos defaults to -1 -> no OutArmOffSet[] read (golden 215).
        CHECK(GetOutArmPitchY_9045(5000) == Prod.iOutArmY15Pitch,
              "N5 GetOutArmPitchY_9045 zero-guard -> iOutArmY15Pitch (golden 218)");
        IN_OUT_ARM_Y_PITCH_MIN = savedMin;      // restore
    }

    // -----------------------------------------------------------------------
    printf("==== W6.2c OutArm core verify: %d passed, %d failed ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
