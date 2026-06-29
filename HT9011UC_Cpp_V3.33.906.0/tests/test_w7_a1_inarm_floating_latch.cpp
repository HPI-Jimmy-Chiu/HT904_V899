// =============================================================================
//  test_w7_a1_inarm_floating_latch.cpp -- W7-A1 VERIFY: in-arm floating + latch
//
//  Translation wave: W7-A1 (un-gate the in-arm shuttle floating/latch PAIR in
//                    ainarm9045.cpp).
//  Author: AI(W7-A1-INTEGRATE) 20260629
//  Suite name (add_test): W7_A1_InArmFloatingLatch
//
//  PURPOSE
//  -------
//  Proves the two newly un-gated in-arm SMs in ainarm9045.cpp (in ht9045_sm) LINK
//  and converge over the Sim HAL with NO hardware:
//    * CheckInShuttleSensor_Latch  (golden ainarm9045.cpp:3810-4174) -- the latch
//      interlock.
//    * DoInArmCheckShuttleFloating (golden ainarm9045.cpp:3349-3757) -- the
//      place-to-shuttle floating quality SM.
//
//  GOLDEN-CITED ORACLES
//  --------------------
//  (a) LATCH SELF-GUARD (golden ainarm9045.cpp:3810-3812 entry guard):
//        if(MOTION_CARD_TYPE!=MotionCard_Contec || LastSet.iRealDummy!=REALLY)
//            return 1;
//      Offline MOTION_CARD_TYPE==0 (cmydef.cpp:3643, != MotionCard_Contec(1)) AND
//      LastSet.iRealDummy==DUMMY(0) (canary_support.cpp:22, != REALLY(2)).  So the
//      guard is TRUE and CheckInShuttleSensor_Latch returns 1 (Finish) on the FIRST
//      call WITHOUT advancing iAutoChkInSHLatchTask past its entry cursor and
//      without touching MOT[]/Ltc state (the body after the guard never runs).
//
//  (b) FLOATING CONVERGENCE (golden ainarm9045.cpp:3349-3757):
//      InitDoInArmCheckShtFloatTask() -> cursor=1, then pump
//      DoInArmCheckShuttleFloating(iSht=0, bPlaceOtherShuttle=false,
//      bNeedCheck=false) with EMPTY Sim grids.  case 1: MoveInArmZToPlateSafe()
//      ==true (aHotPlateSubstrate.cpp:648) and FLCarryKit.HasRealIC()==false
//      (empty grid) routes directly Task=9000 (golden ainarm9045.cpp:1454-1465).
//      goto IN_ARM_CHECK_SHUTTLE -> case 9000 sets the terminals (RecordShtSuperflous,
//      IndexZCanMove[0/1]=true, bShuttleShake/bShuttleKnock=false, bNo9Action=false)
//      and bResult=true, Task=1 (golden :3744-3751).  So the SM converges
//      1 -> 9000 -> (Task reset to 1, bResult=true) inside <=64 ticks, no crash.
//      Terminal SIDE-EFFECT oracle: IndexZCanMove[0]==IndexZCanMove[1]==true and
//      bShuttleKnock==false after convergence (case 9000 writes, golden :3745-3748).
//
//  (c) NO-FLOATING-NO-IC NO-ALARM BRANCH:
//      With InSHT1InLF()/InSHT2InLF()==false offline (csystem_predicates.cpp:219-220)
//      and the carry kits reporting NO superfluous IC (empty grids ->
//      FLCarryKit/BLCarryKit.UseSiteHasIC()==false), the floating SM takes the
//      no-alarm convergence (case 1 -> 9000) -- it NEVER enters the shake/vibrate/
//      knock loop (cases 1410/1411/1401/1430/1440) NOR the latch (case 3000), so
//      the only alarm sinks in the PAIR (latch JAM0401/JAM0403/JAM0404/JAM0406)
//      are never reached.  Witnessed by: bResult==true on convergence AND the
//      cursor never leaving the documented floating state set on the way to 9000.
//      (We additionally re-confirm (a): the latch -- the only ShowErrorMessage
//      carrier in the PAIR -- self-guards to 1 before its switch.)
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" == clean g++
//  compile/link + the PAIR pumps over the Sim HAL without crash + outputs match
//  values hand-derived from the golden cursor/guard logic.
// =============================================================================
#include "ainarm9045.h"            // DoInArmCheckShuttleFloating / CheckInShuttleSensor_Latch / Init*
#include "ainarm9045_w7_shims.h"   // IndexZCanMove[2] / bShuttleKnock (terminal side-effects)
#include "aHotPlateSubstrate.h"    // InArmSuck / FLCarryKit / BLCarryKit
#include "csystem.h"               // InSHT1InLF / InSHT2InLF
#include "Motor/mymotor.h"         // MOT[]
#include "cprod.h"
#include "cpublic.h"
#include "cmydef.h"                // MOTION_CARD_TYPE / In_Shuttle_Auto_Latch / DUMMY / REALLY
#include "MachineType.h"           // MotionCard_Contec / eInSHSenType
#include "canary_support.h"        // LastSet (iRealDummy)
#include "FormsFacade.h"           // fMain / cInplace->bNo9Action
#include <cstdio>

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as the W6 verify TUs)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// Documented floating switch(Task) state universe (golden ainarm9045.cpp:3349-3757):
//   1, 1260, 1300, 1301, 1410, 1411, 1401, 1430, 1440, 2000, 2100, 3000, 9000.
static bool floatCursorSane(int t)
{
    switch (t) {
        case 1: case 1260: case 1300: case 1301: case 1410: case 1411:
        case 1401: case 1430: case 1440: case 2000: case 2100:
        case 3000: case 9000:
            return true;
        default:
            return false;
    }
}

int main()
{
    printf("==== W7-A1 InArm floating + latch verify ====\n");

    // -----------------------------------------------------------------------
    //  KNOWN Sim HAL baseline: empty carry kits + empty in-arm grid.
    // -----------------------------------------------------------------------
    FLCarryKit.ClearAll();
    BLCarryKit.ClearAll();
    InArmSuck.SetAllToNullIC();

    // =======================================================================
    //  ORACLE (a) -- LATCH self-guard (golden ainarm9045.cpp:3810-3812)
    // =======================================================================
    printf("[a] CheckInShuttleSensor_Latch self-guard\n");

    // Confirm the sim canary conditions that make the entry guard TRUE.
    CHECK(MOTION_CARD_TYPE != MotionCard_Contec,
          "a0 sim canary: MOTION_CARD_TYPE != MotionCard_Contec (cmydef.cpp:3643)");
    CHECK(LastSet.iRealDummy != REALLY,
          "a0 sim canary: LastSet.iRealDummy(DUMMY=0) != REALLY(2) (canary_support.cpp:22)");

    InitAutoChkInSHLatchTask();                 // golden :3763 -> iAutoChkInSHLatchTask=1
    CHECK(iAutoChkInSHLatchTask == 1,
          "a1 latch entry cursor == 1 (golden 3763)");

    int iLtcRet = CheckInShuttleSensor_Latch(0, 0, true);   // first call, shuttle 0
    CHECK(iLtcRet == 1,
          "a2 latch returns 1 (Finish) on first call via entry guard (golden 3812)");
    CHECK(iAutoChkInSHLatchTask == 1,
          "a3 latch did NOT advance its cursor (body after guard never ran, no MOT/Ltc touch)");

    int iLtcRet2 = CheckInShuttleSensor_Latch(1, 1, false); // also shuttle 1
    CHECK(iLtcRet2 == 1,
          "a4 latch returns 1 for shuttle 1 too (guard is shuttle-independent, golden 3812)");

    // =======================================================================
    //  ORACLE (b) -- FLOATING convergence 1 -> 9000 (golden :3349-3757)
    // =======================================================================
    printf("[b] DoInArmCheckShuttleFloating convergence (empty grids)\n");

    // Pre-seed the terminal side-effect globals to the OPPOSITE of what case 9000
    // writes, so a PASS proves case 9000 actually executed.
    IndexZCanMove[0] = false;
    IndexZCanMove[1] = false;
    bShuttleKnock    = true;
    fMain->cInplace->bNo9Action = true;         // case 9000 must reset this to false

    InitDoInArmCheckShtFloatTask();             // golden :3344 -> cursor=1
    CHECK(iInArmInArmCheckShtFloatTask == 1,
          "b1 floating entry cursor == 1 (golden 3344)");

    bool sane = true, reached9000 = false, converged = false;
    for (int tick = 0; tick < 64; ++tick) {
        bool r = DoInArmCheckShuttleFloating(0, false, false);  // bNeedCheck=false -> direct 1->9000
        if (!floatCursorSane(iInArmInArmCheckShtFloatTask)) { sane = false; break; }
        if (iInArmInArmCheckShtFloatTask == 9000) reached9000 = true;
        if (r) { converged = true; break; }     // case 9000 sets bResult=true then Task=1
    }
    CHECK(sane,
          "b2 floating cursor stays in {1,1260,1300,1301,1410,1411,1401,1430,1440,2000,2100,3000,9000}");
    CHECK(converged,
          "b3 floating converges: DoInArmCheckShuttleFloating returns true (golden 3750 bResult)");
    CHECK(iInArmInArmCheckShtFloatTask == 1,
          "b4 floating resets cursor to 1 after 9000 (golden 3751 Task=1)");
    // case 9000 side-effects (golden :3745-3748)
    CHECK(IndexZCanMove[0] == true && IndexZCanMove[1] == true,
          "b5 case 9000 set IndexZCanMove[0/1]=true (golden 3745-3746)");
    CHECK(bShuttleKnock == false,
          "b6 case 9000 reset bShuttleKnock=false (golden 3748)");
    CHECK(fMain->cInplace->bNo9Action == false,
          "b7 case 9000 reset cInplace->bNo9Action=false (golden cInArmPlacement.h:63)");
    (void)reached9000;  // 9000 may be transited within the same tick that returns true

    // =======================================================================
    //  ORACLE (c) -- NO-FLOATING-NO-IC: no-alarm branch
    // =======================================================================
    printf("[c] no-floating-no-IC -> no-alarm branch\n");

    // Offline shuttle-in-left-front predicates are false (no shuttle docked) and
    // the carry kits report no IC -> the floating SM takes the case-1 ->9000
    // no-alarm convergence; the shake/vibrate/knock loop and the latch (the only
    // alarm carriers in the PAIR) are never entered.
    CHECK(InSHT1InLF() == false && InSHT2InLF() == false,
          "c1 InSHT1/2InLF() false offline (csystem_predicates.cpp:219-220)");
    CHECK(FLCarryKit.UseSiteHasIC() == false && BLCarryKit.UseSiteHasIC() == false,
          "c2 carry kits report no superfluous IC (empty grids)");

    InitDoInArmCheckShtFloatTask();
    bool noAlarmConverged = false, noAlarmSane = true, enteredShakeOrLatch = false;
    for (int tick = 0; tick < 64; ++tick) {
        bool r = DoInArmCheckShuttleFloating(0, false, false);
        int c = iInArmInArmCheckShtFloatTask;
        if (!floatCursorSane(c)) { noAlarmSane = false; break; }
        // shake/vibrate/knock loop OR latch -> would be the alarm-bearing path
        if (c==1401 || c==1430 || c==1440 || c==1410 || c==1411 || c==3000)
            enteredShakeOrLatch = true;
        if (r) { noAlarmConverged = true; break; }
    }
    CHECK(noAlarmSane,        "c3 cursor stays in documented floating set");
    CHECK(noAlarmConverged,   "c4 converges with no alarm (bResult=true, golden 3750)");
    CHECK(enteredShakeOrLatch == false,
          "c5 never entered the shake/vibrate/knock loop nor the latch (no-alarm branch)");
    // The latch is the only ShowErrorMessage carrier in the PAIR; re-confirm it
    // self-guards to 1 (so even if reached it cannot raise JAM04xx offline).
    InitAutoChkInSHLatchTask();
    CHECK(CheckInShuttleSensor_Latch(0, 0, true) == 1,
          "c6 latch (sole alarm carrier) self-guards to 1 -> JAM04xx unreachable offline");

    // -----------------------------------------------------------------------
    printf("==== W7-A1 InArm floating+latch verify: %d passed, %d failed ====\n",
           g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
