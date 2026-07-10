// =============================================================================
//  test_atester_rear_suckic.cpp -- W64b Wave 2 VERIFY: DoRearTestSuckIC /
//                                   InitRearTestSuckICTask PUMP over the Sim HAL.
//
//  Translation wave: W5-aTester_Rear-Translate (aTester_Rear.cpp additions)
//  Author: AI(W5-aTester_Rear-Translate) 20260710
//  Suite name (add_test): W64b_RearSuckIC
//
//  Mirrors the sibling test_atester_front_suckic.cpp (same wave, parallel unit)
//  structure/conventions exactly, swapped to the Rear (Z2/Arm2/BLCarryKit/
//  BTestSuck/BRCarryKit) side.
//
//  PURPOSE
//  -------
//  Proves the Wave-2 addition to aTester_Rear.cpp (DoRearTestSuckIC /
//  InitRearTestSuckICTask, now real translations, no longer atester_shims.cpp
//  offline stubs) LINKS and its state machine PUMPS over the Sim HAL with no
//  hardware, asserting:
//    (a) InitRearTestSuckICTask() resets its cursor to 1 (golden
//        aTester_Rear.cpp:891-895).  NOTE (golden asymmetry, verified against
//        golden text, NOT a translation defect): unlike the Front twin
//        (InitFrontTestSuckICTask, which ALSO resets bZ1Isdownflag), golden
//        InitRearTestSuckICTask resets ONLY the cursor -- it does not touch
//        bZ2Isdownflag (that flag belongs to DoRearTestDestroyIC's
//        InitRearTestDestroyICTask instead, golden :300-305).  Preserved
//        faithfully, not "fixed" into a false symmetry.
//    (b) DoRearTestSuckIC() from a fresh Init, over an EMPTY Sim HAL (every
//        TMyKitSuck grid set to NULL_IC via SetAllToNullIC()), PARKS at
//        Task==1 forever: case 1's only state-advancing branch is guarded by
//        `if(BLCarryKit.UseSiteHasIC())` (golden aTester_Rear.cpp:986), and
//        TMyKitSuck::UseSiteHasIC() (aHotPlateSubstrate.cpp) is a pure
//        Item[][]!=NULL_IC scan -- with every site NULL_IC there is genuinely
//        "nothing to pick from the shuttle", so the golden SM correctly does
//        nothing and stays parked ("no work" convergence, same shape as the
//        sibling FrontSuckIC suite and the Wave-1 DestroyIC suite's documented
//        parks -- not a Wave-2 defect).
//    (c) A pump-sanity loop additionally proves the cursor never escapes the
//        documented switch(Task) label set (no crash, no wild jump) even when
//        driven many ticks past the park point.
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here == clean
//  g++ compile/link + the SM pumps over the Sim HAL without crash + outputs
//  match values hand-derived from the golden formula/cursor logic (mirrors
//  test_w6_4b_front_rear_destroy.cpp / test_atester_front_suckic.cpp's own
//  equivalence note).
// =============================================================================
#include "aTester_Rear.h"           // iRearTestSuckICTask / InitRearTestSuckICTask / DoRearTestSuckIC
#include "aTester_Front.h"          // CheckZ1IsDown() (DoRearTestSuckIC case 200/301 dep -- not exercised on the park path, but must link)
#include "aHotPlateSubstrate.h"     // FTestSuck/BTestSuck/FLCarryKit/BLCarryKit/FRCarryKit/BRCarryKit/TestSocket/InArmSuck
#include "Motor/mymotor.h"          // MOT[]
#include "cprod.h"                  // Prod / TestIF / TestIF_File / IniConfig (via Config.h) / CosFunction / DeviceForm(_File)
#include "cpublic.h"
#include "cmydef.h"                 // bZ2Isdownflag / iOneCycle / iCleanOut / bUseTwoArm32Site / ...
#include "csystem.h"
#include "canary_support.h"         // LastSet
#include "FormsFacade.h"
#include "acarry.h"                 // CheckCFixTrayFullPlace (real body, Wave-2 dep)
#include "atester_shims.h"          // IsNNMode / fiosetview / CheckHeaterOK / ...
#include <cstdio>

static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) label set (golden aTester_Rear.cpp:898-2209) ----
static bool suckIcCursorSane(int t)
{
    switch (t) {
        case 1: case 300: case 301: case 307: case 308: case 309:
        case 3091: case 3099: case 310: case 3051: case 3052: case 305:
        case 306: case 320: case 321: case 500: case 501: case 502:
            return true;
        default: return false;
    }
}

int main()
{
    printf("==== W64b Wave 2 \"DoRearTestSuckIC\" SM verify ====\n");

    // -----------------------------------------------------------------------
    //  KNOWN Sim HAL state (same convention as test_atester_front_suckic.cpp /
    //  test_w6_4b_front_rear_destroy.cpp).
    // -----------------------------------------------------------------------
    TestSocket.SetAllToNullIC();
    FTestSuck.SetAllToNullIC();
    BTestSuck.SetAllToNullIC();
    FLCarryKit.SetAllToNullIC();
    BLCarryKit.SetAllToNullIC();
    FRCarryKit.SetAllToNullIC();
    BRCarryKit.SetAllToNullIC();
    InArmSuck.SetAllToNullIC();
    bUseTwoArm32Site = false;
    iOneCycle  = 0;
    iCleanOut  = 0;

    // =======================================================================
    //  PART A -- InitRearTestSuckICTask() resets the cursor (oracle a)
    // =======================================================================
    printf("[A] InitRearTestSuckICTask cursor reset\n");
    iRearTestSuckICTask = 999;
    InitRearTestSuckICTask();    // golden aTester_Rear.cpp:891-895 -> Task=1 (cursor only; see file-top NOTE re: no flag reset)
    CHECK(iRearTestSuckICTask == 1, "InitRearTestSuckICTask: cursor -> 1 (golden :893)");

    // =======================================================================
    //  PART B -- DoRearTestSuckIC() pump: PARKS at Task==1 over an empty
    //            Sim HAL (no real IC anywhere to pick) (oracle b)
    // =======================================================================
    printf("[B] DoRearTestSuckIC pump (parks at Task=1, no IC in BLCarryKit)\n");

    InitRearTestSuckICTask();
    CHECK(iRearTestSuckICTask == 1, "DoRearTestSuckIC: entry cursor == 1");
    CHECK(BLCarryKit.UseSiteHasIC() == false,
          "precondition: BLCarryKit.UseSiteHasIC()==false after SetAllToNullIC (aHotPlateSubstrate.cpp)");

    bool ret0 = DoRearTestSuckIC();
    CHECK(ret0 == false, "DoRearTestSuckIC: first pump returns false (no finish condition met)");
    CHECK(iRearTestSuckICTask == 1,
          "DoRearTestSuckIC: case 1 UseSiteHasIC()==false -> no branch taken -> Task stays 1 (golden :986)");

    for (int tick = 0; tick < 16; ++tick)
        DoRearTestSuckIC();
    CHECK(iRearTestSuckICTask == 1,
          "DoRearTestSuckIC: PARKS at Task=1 (no real IC to pick from shuttle -- not a Wave-2 defect)");

    // ---- pump-sanity: cursor never escapes the documented switch(Task) set.
    {
        InitRearTestSuckICTask();
        bool sane = true;
        for (int tick = 0; tick < 128; ++tick) {
            DoRearTestSuckIC();
            if (!suckIcCursorSane(iRearTestSuckICTask)) { sane = false; break; }
        }
        CHECK(sane, "DoRearTestSuckIC: cursor stays in the documented switch(Task) set (no escape)");
    }

    // =======================================================================
    printf("\n==== W64b Wave 2 RearSuckIC summary: %d PASS, %d FAIL ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
