// =============================================================================
//  test_barcode_shuttle2_scan.cpp  --  VERIFY: BarCode_Shuttle2_Scan.cpp
//
//  Translation wave: W5-BarCode-Shuttle2Scan
//  Author: AI(W5-BarCode-Shuttle2Scan) 20260711
//
//  PURPOSE
//  -------
//  Exercises the 5 translated free functions (see BarCode_Shuttle2_Scan.h for
//  full scope) over the Sim HAL with no hardware, proving:
//    - the file compiles/links against the existing translated substrate
//      (aHotPlateSubstrate / cprod / Motor / canary_support / acarry), and
//    - the core switch(Task) SM cursors progress the way hand-derived from the
//      golden BarCode_Sh2.cpp/BarCode.cpp source (NOT a live BCB6 binary diff
//      -- no Borland compiler available in this environment, same documented
//      limitation as every other W6/W5 verify TU in this tree).
//
//  ORACLES
//  -------
//    [O1] InitialBarcodeScanInShuttle2 resets the shared cursor + per-shuttle
//         duplicate-fail counter (golden :36-37).
//    [O2] InitialBarcodeScanOutShuttle2 / DoBarcodeScanOutShuttle_2 cursor
//         entry: with BRCarryKit an all-NULL_IC 1-row shuttle (offline Sim
//         default), case 1000's per-column NULL_IC short-circuit never fires
//         a motor-move-gated branch decision that depends on hardware, so the
//         very first tick (Task 1->1000, iShtCol==0 boundary) still requires a
//         motor move; assert cursor progression stays within the documented
//         switch(Task) universe over an N-tick pump with Sim HAL MotorMove
//         always succeeding (offline idle motor).
//    [O3] DoBarcodeTriggerInShuttle_2 case-1 entry seeds iNowMoveStep/iNowCheckStep
//         from InArmSuck.iShtCol-1 and clears bBarcodeNum for both rows; with
//         BLCarryKit an all-NULL_IC shuttle (offline Sim default) case 1000's
//         all-NULL_IC short-circuit (golden :2515-2520) fires immediately and
//         the cursor advances straight to 1200 without ever calling MotorMove
//         -- this is the one branch this TU can assert deterministically
//         without a real Sim-HAL motor-position contract.
//    [O4] InitialShuttleFloatCheck2 sets the (currently-unconsumed, since
//         DoShuttleFloatCheck_2 is NOT part of this unit's delivered scope)
//         cursor + bCCDcheckShuttle[1] flag -- proves the golden write targets
//         compile/link even though nothing in THIS unit reads them back yet.
//    [O5] The golden Shuttle-1/Shuttle-2 asymmetry preserved verbatim at case
//         3400 (iSH1_1BarcodePosition/iSH1_2BarcodePosition instead of the
//         SH2 globals) is exercised by seeding the Task cursor directly to
//         3400 and checking iSH1_1/iSH1_2BarcodePosition changed while
//         iSH2_1/iSH2_2BarcodePosition did not -- a regression guard against
//         a future "helpful" fix that would silently diverge from golden.
//         RUNS FIRST in main() (see its own comment): it needs
//         DoBarcodeTriggerInShuttle_2's function-local statics (iNowCheckStep/
//         bBarcodeNum[]) at their pristine static-initializer values, which
//         only holds true for the very first call ever made to the function
//         in this binary.
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here means
//  clean g++ compile/link + the SM cursors landing where hand-derived from the
//  golden switch(Task) ladder, exactly the same standard applied by every
//  other W5/W6 verify TU in this tree (test_w6_5_shuttle.cpp, etc).
// =============================================================================
#include "vclcompat/vcl_compat.h"
#include "aHotPlateSubstrate.h"     // InArmSuck / BLCarryKit / BRCarryKit
#include "cprod.h"                  // Prod / TestIF_File / Offset
#include "cmydef.h"                  // NULL_IC / iSH1_1BarcodePosition / iSH2_1BarcodePosition
#include "Motor/mymotor.h"           // MOT[] (Sim HAL)
#include "BarCode/BarCode_Shuttle2_Scan.h"
#include <cstdio>

static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) state set for DoBarcodeTriggerInShuttle_2 ----
// golden BarCode_Sh2.cpp:2474-3038 (translated BarCode_Shuttle2_Scan.cpp).
static bool triggerCursorSane(int t)
{
    switch (t) {
        case 1: case 1000: case 1120: case 1150: case 1200: case 1300:
        case 1350: case 2000: case 3000: case 3100: case 3200: case 3300:
        case 3350: case 3400: case 5000:
            return true;
        default:
            return false;
    }
}

// ---- documented switch(Task) state set for DoBarcodeScanOutShuttle_2 -------
// golden BarCode_Sh2.cpp:4262-4399.
static bool scanOutCursorSane(int t)
{
    switch (t) {
        case 1: case 1000: case 1100: case 1200: case 2000:
            return true;
        default:
            return false;
    }
}

static void ResetSim1Row2ColAllNullIC()
{
    // A 1-row, 2-column shuttle with every site NULL_IC -- the "empty
    // shuttle" Sim-HAL default this whole tree's other verify TUs use for
    // in/outbound-shuttle SM entry (mirrors test_w6_5_shuttle.cpp's baseline).
    // iMaxRow/iMaxCol are NOT set by TMyKitSuck's ctor (golden separates the
    // "grid capacity" fields from iShtRow/iShtCol); explicitly set them here,
    // matching the established convention in test_w6_2c_inarm_variants*.cpp /
    // test_w7_grid_occupancy.cpp.
    InArmSuck.iShtRow = 1;
    InArmSuck.iShtCol = 2;
    BLCarryKit.iMaxRow = 2;
    BLCarryKit.iMaxCol = 2;
    BRCarryKit.iMaxRow = 2;
    BRCarryKit.iMaxCol = 2;
    for (int i = 0; i < BLCarryKit.iMaxRow; i++)
        for (int j = 0; j < BLCarryKit.iMaxCol; j++)
        {
            BLCarryKit.Item[i][j] = NULL_IC;
            BLCarryKit.cDeviceInf[i][j] = "";
        }
    for (int i = 0; i < BRCarryKit.iMaxRow; i++)
        for (int j = 0; j < BRCarryKit.iMaxCol; j++)
        {
            BRCarryKit.Item[i][j] = NULL_IC;
            BRCarryKit.cDeviceInf[i][j] = "";
        }
}

int main()
{
    printf("=== BarCode_Shuttle2_Scan translation verification ===\n");
    printf("(cursor progression hand-derived from golden BarCode_Sh2.cpp/BarCode.cpp;\n");
    printf(" original BCB6 binary NOT run -- no Borland compiler available)\n\n");

    ResetSim1Row2ColAllNullIC();

    // -------------------------------------------------------------------
    // [O5] golden Shuttle-1/Shuttle-2 asymmetry preserved at case 3400.
    // MUST run before any other call to BarCode_Sh2_DoBarcodeTriggerInShuttle_2
    // in this binary: it forces the Task cursor directly to 3400 and relies
    // on the function's local statics (iNowCheckStep / bBarcodeNum[]) still
    // holding their pristine static-initializer values (0 / false) from the
    // very first call ever made to the function -- realistically pumping the
    // full retry ladder (case 1000->1200->1300->1350->3100->3200->3300->3350
    // ->3400) to reach case 3400 turned out to depend on a fragile multi-path
    // pipeline choreography (case 1350's inline-timeout branch jumps straight
    // to 3100, bypassing 3000's re-seed, so whether 3200-3400 are ever
    // reached depends on exactly which column iNowCheckStep happens to still
    // be parked at); a direct, white-box cursor seed is simpler and no less
    // faithful to what this oracle actually needs to prove (this specific
    // case body writes the SH1 globals, not SH2).
    // -------------------------------------------------------------------
    printf("-- O5: golden SH1/SH2 position-global asymmetry (case 3400) --\n");
    {
        iSH1_1BarcodePosition = -100;   // poison
        iSH1_2BarcodePosition = -100;   // poison
        iSH2_1BarcodePosition = -200;   // poison (must stay untouched by case 3400)
        iSH2_2BarcodePosition = -200;   // poison (must stay untouched by case 3400)

        BLCarryKit.cDeviceInf[0][0] = "";
        BLCarryKit.cDeviceInf[1][0] = "";

        iInitialBarcodeInShuttle2Task = 3400;   // seed cursor directly (see note above)
        BarCode_Sh2_DoBarcodeTriggerInShuttle_2();   // FIRST EVER call to this function

        CHECK(iSH1_1BarcodePosition != -100, "case 3400 wrote iSH1_1BarcodePosition (golden quirk reproduced)");
        CHECK(iSH1_2BarcodePosition != -100, "case 3400 wrote iSH1_2BarcodePosition (golden quirk reproduced)");
        CHECK(iSH2_1BarcodePosition == -200, "case 3400 does NOT touch iSH2_1BarcodePosition (only case 1120 does)");
        CHECK(iSH2_2BarcodePosition == -200, "case 3400 does NOT touch iSH2_2BarcodePosition (only case 1120 does)");
    }

    // -------------------------------------------------------------------
    // [O1] InitialBarcodeScanInShuttle2 resets cursor + dup-fail counter
    // -------------------------------------------------------------------
    printf("-- O1: InitialBarcodeScanInShuttle2 reset --\n");
    {
        iInitialBarcodeInShuttle2Task = 999;   // poison
        iShtDupilcateFailCnt[1] = 7;            // poison
        BarCode_Sh2_InitialBarcodeScanInShuttle2(true);
        CHECK(iInitialBarcodeInShuttle2Task == 1, "iInitialBarcodeInShuttle2Task reset to 1");
        CHECK(iShtDupilcateFailCnt[1] == 0, "iShtDupilcateFailCnt[1] reset to 0");
    }

    // -------------------------------------------------------------------
    // [O2] InitialBarcodeScanOutShuttle2 / DoBarcodeScanOutShuttle_2 pump
    // -------------------------------------------------------------------
    printf("-- O2: OutShuttle2 scan cursor progression --\n");
    {
        BarCode_Sh2_InitialBarcodeScanOutShuttle2();
        CHECK(iInitialBarcodeOutShuttle2Task == 1, "iInitialBarcodeOutShuttle2Task reset to 1");

        bool cursorStayedSane = true;
        bool finishedOK = false;
        for (int tick = 0; tick < 64; tick++)
        {
            bool finished = BarCode_Sh2_DoBarcodeScanOutShuttle_2();
            if (!scanOutCursorSane(iInitialBarcodeOutShuttle2Task))
            {
                cursorStayedSane = false;
                printf("    (escaped cursor set at tick %d: Task=%d)\n", tick, iInitialBarcodeOutShuttle2Task);
                break;
            }
            if (finished)
            {
                finishedOK = true;
                break;
            }
        }
        CHECK(cursorStayedSane, "DoBarcodeScanOutShuttle_2 64-tick pump stays in documented switch(Task) set");
        CHECK(finishedOK, "DoBarcodeScanOutShuttle_2 completes a full cycle (all-NULL_IC shuttle, Sim MotorMove always succeeds)");
    }

    // -------------------------------------------------------------------
    // [O3] DoBarcodeTriggerInShuttle_2 all-NULL_IC short-circuit
    // -------------------------------------------------------------------
    printf("-- O3: InShuttle2 trigger, all-NULL_IC short-circuit --\n");
    {
        ResetSim1Row2ColAllNullIC();
        BarCode_Sh2_InitialBarcodeScanInShuttle2(true);
        CHECK(iInitialBarcodeInShuttle2Task == 1, "trigger cursor starts at 1");

        // one tick: case 1 falls through into case 1000, whose all-NULL_IC
        // guard (golden :2515-2520) fires immediately for iNowMoveStep ==
        // iShtCol-1 == 1 (both rows NULL_IC) -> Task=1200 without any
        // MotorMove call.
        BarCode_Sh2_DoBarcodeTriggerInShuttle_2();
        CHECK(iInitialBarcodeInShuttle2Task == 1200,
              "case 1000 all-NULL_IC short-circuit advances cursor to 1200 with no motor move");
    }

    // -------------------------------------------------------------------
    // [O4] InitialShuttleFloatCheck2 write-target smoke check
    // -------------------------------------------------------------------
    printf("-- O4: InitialShuttleFloatCheck2 write targets --\n");
    {
        iShuttleFloatCheck2Task = -1;   // poison
        bCCDcheckShuttle[1] = false;
        BarCode_Sh2_InitialShuttleFloatCheck2();
        CHECK(iShuttleFloatCheck2Task == 1, "iShuttleFloatCheck2Task reset to 1");
        CHECK(bCCDcheckShuttle[1] == true, "bCCDcheckShuttle[1] set true");
    }

    printf("\n=== SUMMARY: %d passed, %d failed ===\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
