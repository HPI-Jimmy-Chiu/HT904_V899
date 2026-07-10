// =============================================================================
//  test_atester_front_suckic.cpp -- W64b Wave 2 VERIFY: DoFrontTestSuckIC /
//                                    InitFrontTestSuckICTask PUMP over the Sim HAL.
//
//  Translation wave: W64b-Translate-Wave2 (aTester_Front.cpp additions)
//  Author: AI(W64b-Translate) 20260710
//  Suite name (add_test): W64b_FrontSuckIC
//
//  PURPOSE
//  -------
//  Proves the Wave-2 addition to aTester_Front.cpp (DoFrontTestSuckIC /
//  InitFrontTestSuckICTask, now real translations, no longer atester_shims.cpp
//  offline stubs) LINKS and its state machine PUMPS over the Sim HAL with no
//  hardware, asserting:
//    (a) InitFrontTestSuckICTask() resets its cursor to 1 and the
//        bZ1Isdownflag "index-is-down" latch to false (golden
//        aTester_Front.cpp:870-875), mirroring the sibling Wave-1 oracle in
//        test_w6_4b_front_rear_destroy.cpp Part A.
//    (b) DoFrontTestSuckIC() from a fresh Init, over an EMPTY Sim HAL (every
//        TMyKitSuck grid set to NULL_IC via SetAllToNullIC()), PARKS at
//        Task==1 forever: case 1's only state-advancing branch is guarded by
//        `if(FLCarryKit.UseSiteHasIC())` (golden aTester_Front.cpp:919), and
//        TMyKitSuck::UseSiteHasIC() (aHotPlateSubstrate.cpp:181-188) is a pure
//        Item[][]!=NULL_IC scan -- with every site NULL_IC there is genuinely
//        "nothing to pick from the shuttle", so the golden SM correctly does
//        nothing and stays parked ("no work" convergence, NOT a translation
//        defect -- same shape as the sibling suite's documented Task=100 park
//        for DoFrontTestDestroyIC).
//    (c) A pump-sanity loop additionally proves the cursor never escapes the
//        documented switch(Task) label set (no crash, no wild jump) even when
//        driven many ticks past the park point.
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here == clean
//  g++ compile/link + the SM pumps over the Sim HAL without crash + outputs
//  match values hand-derived from the golden formula/cursor logic (mirrors
//  test_w6_4b_front_rear_destroy.cpp's own equivalence note).
// =============================================================================
#include "aTester_Front.h"          // iFrontTestSuckICTask / InitFrontTestSuckICTask / DoFrontTestSuckIC
#include "aTester_Rear.h"           // CheckZ2IsDown() (DoFrontTestSuckIC case 301 dep -- not exercised on the park path, but must link)
#include "aHotPlateSubstrate.h"     // FTestSuck/BTestSuck/FLCarryKit/BLCarryKit/FRCarryKit/BRCarryKit/TestSocket/InArmSuck
#include "Motor/mymotor.h"          // MOT[]
#include "cprod.h"                  // Prod / TestIF / TestIF_File / IniConfig (via Config.h) / CosFunction / DeviceForm(_File)
#include "cpublic.h"
#include "cmydef.h"                 // bZ1Isdownflag / iOneCycle / iCleanOut / bUseTwoArm32Site / ...
#include "csystem.h"
#include "canary_support.h"         // LastSet
#include "FormsFacade.h"
#include "atester_shims.h"          // IsNNMode / fiosetview / CheckHeaterOK / ...
#include <cstdio>

static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) label set (golden aTester_Front.cpp:877-2224) ----
static bool suckIcCursorSane(int t)
{
    switch (t) {
        case 1: case 299: case 300: case 301: case 307: case 308: case 309:
        case 3091: case 3099: case 310: case 3051: case 3052: case 305:
        case 306: case 320: case 321: case 500: case 501: case 502:
            return true;
        default: return false;
    }
}

int main()
{
    printf("==== W64b Wave 2 \"DoFrontTestSuckIC\" SM verify ====\n");

    // -----------------------------------------------------------------------
    //  KNOWN Sim HAL state (same convention as test_w6_4b_front_rear_destroy.cpp).
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
    bNowDoInterFaceErrorStep = false;
    bContactTimeOverStep     = false;
    CosFunction.bSortingBy2DList = false;
    iOneCycle  = 0;
    iCleanOut  = 0;

    // =======================================================================
    //  PART A -- InitFrontTestSuckICTask() resets the cursor + down-flag (oracle a)
    // =======================================================================
    printf("[A] InitFrontTestSuckICTask cursor/flag reset\n");
    iFrontTestSuckICTask = 999;   bZ1Isdownflag = true;
    InitFrontTestSuckICTask();    // golden aTester_Front.cpp:870-875 -> Task=1, bZ1Isdownflag=false
    CHECK(iFrontTestSuckICTask == 1, "InitFrontTestSuckICTask: cursor -> 1 (golden :872)");
    CHECK(bZ1Isdownflag == false,    "InitFrontTestSuckICTask: bZ1Isdownflag -> false (golden :873)");

    // =======================================================================
    //  PART B -- DoFrontTestSuckIC() pump: PARKS at Task==1 over an empty
    //            Sim HAL (no real IC anywhere to pick) (oracle b)
    // =======================================================================
    printf("[B] DoFrontTestSuckIC pump (parks at Task=1, no IC in FLCarryKit)\n");

    InitFrontTestSuckICTask();
    CHECK(iFrontTestSuckICTask == 1, "DoFrontTestSuckIC: entry cursor == 1");
    CHECK(FLCarryKit.UseSiteHasIC() == false,
          "precondition: FLCarryKit.UseSiteHasIC()==false after SetAllToNullIC (aHotPlateSubstrate.cpp:181-188)");

    bool ret0 = DoFrontTestSuckIC();
    CHECK(ret0 == false, "DoFrontTestSuckIC: first pump returns false (no finish condition met)");
    CHECK(iFrontTestSuckICTask == 1,
          "DoFrontTestSuckIC: case 1 UseSiteHasIC()==false -> no branch taken -> Task stays 1 (golden :919)");

    for (int tick = 0; tick < 16; ++tick)
        DoFrontTestSuckIC();
    CHECK(iFrontTestSuckICTask == 1,
          "DoFrontTestSuckIC: PARKS at Task=1 (no real IC to pick from shuttle -- not a Wave-2 defect)");

    // ---- pump-sanity: cursor never escapes the documented switch(Task) set.
    {
        InitFrontTestSuckICTask();
        bool sane = true;
        for (int tick = 0; tick < 128; ++tick) {
            DoFrontTestSuckIC();
            if (!suckIcCursorSane(iFrontTestSuckICTask)) { sane = false; break; }
        }
        CHECK(sane, "DoFrontTestSuckIC: cursor stays in the documented switch(Task) set (no escape)");
    }

    // =======================================================================
    printf("\n==== W64b Wave 2 FrontSuckIC summary: %d PASS, %d FAIL ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
