// =============================================================================
//  test_barcode_shuttle2_scanremainder2.cpp  --  VERIFY: BarCode_Shuttle2_ScanRemainder2.cpp
//
//  Translation wave: W5-BarCode hand-off completion
//  Author: AI(W5-Final-BarCodeShuttle2ScanRemainder2) 20260711
//
//  PURPOSE
//  -------
//  Exercises the one translated free function (BarCode_Sh2_DoShuttleFloatCheck_2,
//  see BarCode_Shuttle2_ScanRemainder2.h for full scope) over the Sim HAL with
//  no hardware, proving:
//    - the file compiles/links against the existing translated substrate
//      (aHotPlateSubstrate / cprod / Motor / canary_support / acarry_shims)
//      AND against BOTH sibling BarCode units it reuses shared state from
//      (BarCode_Shuttle1_Scan.o for the SFC arrays, BarCode_Shuttle2_Scan.o for
//      iBarCode2_1/2/iShuttleFloatCheck2Task/bCCDcheckShuttle), and
//    - the core switch(Task) SM cursor progresses the way hand-derived from the
//      golden BarCode_Sh2.cpp:4406-4878 source (NOT a live BCB6 binary diff --
//      no Borland compiler available in this environment, same documented
//      limitation as every other W5/W6 verify TU in this tree).
//
//  ORACLES
//  -------
//    [O1] Fast-exit: BLCarryKit.HasRealIC()==false makes case 1 return true
//         immediately (golden :4429-4433) without touching any CCD/timer state
//         or advancing the Task cursor away from 1 -- mirrors the sibling
//         DoShuttleFloatCheck_1's own O7 in test_barcode_shuttle1_scan.cpp.
//    [O2] With a real IC seeded (BLCarryKit.Item[0][0]=HAS_IC), one call:
//           - falls through case 1 -> case 500 in the SAME call (golden has no
//             `break;` between them, verbatim preserved here), landing the
//             cursor at 500 (golden :4436-4470);
//           - iSFCTotalMoveStep / iSFCCurrentStep[iBarCode2_1/2_2] get set from
//             InArmSuck.iShtCol (golden BarSh2_SetSFCCheckStepCount, :4435/
//             cprod TestIF_File.bSFCUse2Photo==false path);
//           - bCCDcheckShuttle[1] is consumed (reset to false) since the
//             fixture seeds it true (golden :4448-4452, wei 20161102).
//         Then a fast N-tick pump (no real time elapses -- SFCStartDelay's 5s
//         wall-clock window never opens in a tight loop) proves the cursor
//         stays PARKED at 500 (documented member of the switch(Task) universe,
//         golden case 500's `else if(SFCStartDelay[iSht].Off())` correctly
//         evaluating false) rather than escaping to an undocumented value --
//         this is the deterministic, hardware-independent oracle available for
//         this function (a real WAR0462 timeout requires actually waiting out
//         the wall-clock 5000ms window, which this fast unit test does not do
//         -- see BarCode_Shuttle2_ScanRemainder2.h's own "hardware-gated SM
//         never truly completes" note for the documented full-timeline shape).
//    [O3] The golden Shuttle-1/Shuttle-2 IsNNMode() asymmetry preserved
//         verbatim (see .h "GOLDEN QUIRK PRESERVED VERBATIM"): forcing the
//         cursor to 1150 with bSFCStepError[iBarCode2_1]=true and pumping one
//         tick lands ErrPart via the *2-way* `InArmSuck.iShtRow` branch (no
//         IsNNMode() 3-way split) -- exercised indirectly by confirming the
//         function does not crash/misbehave when IsNNMode()'s dependencies
//         (atester_shims fContact) are left at their offline defaults, since
//         this function's translated body never calls IsNNMode() at all
//         (grep-verified against the .cpp -- this oracle just proves case 1150
//         is reachable and lands the cursor at the documented next state).
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here means
//  clean g++ compile/link + the SM cursor landing where hand-derived from the
//  golden switch(Task) ladder, exactly the same standard applied by every
//  other W5/W6 verify TU in this tree (test_barcode_shuttle1_scan.cpp,
//  test_barcode_shuttle2_scan.cpp, etc).
// =============================================================================
#include "vclcompat/vcl_compat.h"
#include "aHotPlateSubstrate.h"     // InArmSuck / BLCarryKit
#include "cprod.h"                  // Prod / TestIF_File / Offset
#include "cmydef.h"                  // NULL_IC / HAS_IC
#include "Motor/mymotor.h"           // MOT[] (Sim HAL)
#include "BarCode/BarCode_Shuttle2_ScanRemainder2.h"
#include <cstdio>

static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) state set for DoShuttleFloatCheck_2 -----------
// golden BarCode_Sh2.cpp:4406-4878 (translated BarCode_Shuttle2_ScanRemainder2.cpp).
static bool floatCheckCursorSane(int t)
{
    switch (t) {
        case 1: case 500: case 1000: case 1120: case 1150: case 1160:
        case 1170: case 1180: case 1200: case 1300: case 1350: case 2000:
        case 2500: case 2600: case 5000:
            return true;
        default:
            return false;
    }
}

static void ResetSim1Row2ColAllNullIC()
{
    // Same "empty shuttle" Sim-HAL baseline as the sibling BarCode units'
    // verify TUs (test_barcode_shuttle1_scan.cpp / test_barcode_shuttle2_scan.cpp).
    InArmSuck.iShtRow = 1;
    InArmSuck.iShtCol = 2;
    BLCarryKit.iMaxRow = 2;
    BLCarryKit.iMaxCol = 2;
    for (int i = 0; i < BLCarryKit.iMaxRow; i++)
        for (int j = 0; j < BLCarryKit.iMaxCol; j++)
        {
            BLCarryKit.Item[i][j] = NULL_IC;
            BLCarryKit.cDeviceInf[i][j] = "";
        }
}

int main()
{
    printf("=== BarCode_Shuttle2_ScanRemainder2 (DoShuttleFloatCheck_2) translation verification ===\n");
    printf("(cursor progression hand-derived from golden BarCode_Sh2.cpp:4406-4878;\n");
    printf(" original BCB6 binary NOT run -- no Borland compiler available)\n\n");

    // -------------------------------------------------------------------
    // [O1] Fast-exit: no real IC on the shuttle
    // -------------------------------------------------------------------
    printf("-- O1: DoShuttleFloatCheck_2 fast-exit (no real IC) --\n");
    {
        ResetSim1Row2ColAllNullIC();
        BarCode_Sh2_InitialShuttleFloatCheck2();
        CHECK(iShuttleFloatCheck2Task == 1, "iShuttleFloatCheck2Task reset to 1 by InitialShuttleFloatCheck2");
        CHECK(BLCarryKit.HasRealIC() == false, "fixture: BLCarryKit has no real IC");

        bool result = BarCode_Sh2_DoShuttleFloatCheck_2();
        CHECK(result == true, "DoShuttleFloatCheck_2 returns true immediately when HasRealIC()==false (golden :4429-4433)");
        CHECK(iShuttleFloatCheck2Task == 1, "cursor untouched at 1 (case 1's fast-exit break happens before any Task= write)");
    }

    // -------------------------------------------------------------------
    // [O2] Real IC present: case 1 falls through to 500, cursor parks there
    // -------------------------------------------------------------------
    printf("-- O2: DoShuttleFloatCheck_2 with real IC -- case 1 -> 500 fallthrough, parks at 500 --\n");
    {
        ResetSim1Row2ColAllNullIC();
        BLCarryKit.Item[0][0] = HAS_IC;
        TestIF_File.bSFCUse2Photo = false;
        bCCDcheckShuttle[1] = true;   // seed so we can prove case 1 consumes it (golden :4448-4452)

        BarCode_Sh2_InitialShuttleFloatCheck2();
        CHECK(BLCarryKit.HasRealIC() == true, "fixture: BLCarryKit now has a real IC at [0][0]");

        bool finished = BarCode_Sh2_DoShuttleFloatCheck_2();
        CHECK(finished == false, "first call does not finish (falls through case 1 into case 500, no SFCStartDelay elapsed yet)");
        CHECK(iShuttleFloatCheck2Task == 500, "cursor landed at 500 after the case1->500 fallthrough (golden, no break between them)");
        CHECK(iSFCTotalMoveStep == InArmSuck.iShtCol, "BarSh2_SetSFCCheckStepCount: bSFCUse2Photo==false -> iSFCTotalMoveStep=iShtCol (golden BarCode.cpp:3128)");
        CHECK(iSFCCurrentStep[iBarCode2_1] == InArmSuck.iShtCol - 1, "iSFCCurrentStep[iBarCode2_1] seeded to iSFCTotalMoveStep-1");
        CHECK(iSFCCurrentStep[iBarCode2_2] == InArmSuck.iShtCol - 1, "iSFCCurrentStep[iBarCode2_2] seeded to iSFCTotalMoveStep-1");
        CHECK(bCCDcheckShuttle[1] == false, "bCCDcheckShuttle[1] consumed (reset false) by case 1 (golden :4448-4452, wei 20161102)");

        // Fast re-pump: SendCCDCommand is gated (no-op), so bSFCGetSE9[] never
        // becomes true; SFCStartDelay[1]'s 5000ms wall-clock window has not
        // elapsed in a tight loop, so Off() stays false -- cursor must stay
        // parked at 500 (never silently escape to an undocumented value).
        bool cursorStayedSane = true;
        for (int tick = 0; tick < 32; tick++)
        {
            BarCode_Sh2_DoShuttleFloatCheck_2();
            if (!floatCheckCursorSane(iShuttleFloatCheck2Task))
            {
                cursorStayedSane = false;
                printf("    (escaped cursor set at tick %d: Task=%d)\n", tick, iShuttleFloatCheck2Task);
                break;
            }
        }
        CHECK(cursorStayedSane, "32-tick fast re-pump stays in the documented switch(Task) universe");
        CHECK(iShuttleFloatCheck2Task == 500, "cursor still parked at 500 (SFCStartDelay's 5s wall-clock window never elapsed in a tight loop)");
    }

    // -------------------------------------------------------------------
    // [O3] case 1150 reachable with the 2-way (not 3-way IsNNMode()) branch
    // -------------------------------------------------------------------
    printf("-- O3: case 1150, 2-way InArmSuck.iShtRow branch (no IsNNMode() 3-way split) --\n");
    {
        ResetSim1Row2ColAllNullIC();
        InArmSuck.iShtRow = 2;   // 2-row shuttle -> both iSFCRowA/B branches taken
        iShuttleFloatCheck2Task = 1150;
        bSFCStepError[iBarCode2_1] = true;
        bSFCStepError[iBarCode2_2] = false;

        BarCode_Sh2_DoShuttleFloatCheck_2();
        CHECK(iShuttleFloatCheck2Task == 1180,
              "case 1150 with a step-error routes to 1180 (golden :4547, ErrPart built via plain iShtRow branch, no IsNNMode())");
    }

    printf("\n=== SUMMARY: %d passed, %d failed ===\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
