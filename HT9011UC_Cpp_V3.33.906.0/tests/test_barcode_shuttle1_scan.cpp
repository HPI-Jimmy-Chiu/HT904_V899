// =============================================================================
//  test_barcode_shuttle1_scan.cpp  --  VERIFY: BarCode_Shuttle1_Scan.cpp
//
//  Translation wave: W5-BarCode-Shuttle1-Translate
//  Author: AI(W5-BarCode-Shuttle1-Translate) 20260711
//
//  PURPOSE
//  -------
//  Exercises the 7 translated free functions (see BarCode_Shuttle1_Scan.h for
//  full scope) over the Sim HAL with no hardware, proving:
//    - the file compiles/links against the existing translated substrate
//      (aHotPlateSubstrate / cprod / Motor / canary_support / acarry /
//      atester_shims) AND against the sibling BarCode_Shuttle2_Scan unit
//      (shared TfBarCode instance data -- see O0 below), and
//    - the core switch(Task) SM cursors progress the way hand-derived from
//      the golden BarCode_Sh1.cpp/BarCode.cpp source (NOT a live BCB6 binary
//      diff -- no Borland compiler available in this environment, same
//      documented limitation as every other W5/W6 verify TU in this tree).
//
//  ORACLES
//  -------
//    [O0] Sanity: this TU links cleanly together with BarCode_Shuttle2_Scan.o
//         (both reference the same shared extern globals -- iShtDupilcateFailCnt,
//         map2DList, bBarcodeStartDelay, bBarcodeDataSaveReady,
//         BarcodeExposureDelay/BarcodePosDelay/BarcodeDelay, bCCDcheckShuttle --
//         with the canonical DEFINITION living in BarCode_Shuttle2_Scan.cpp).
//         Exercised implicitly by every oracle below (all touch this shared
//         state through InitialBarcodeScanInShuttle1 / DoBarcodeTriggerInShuttle_1).
//    [O1] InitialBarcodeScanInShuttle1 resets the shared cursor + per-shuttle
//         duplicate-fail counter (golden :36-37) and, with bClear2DID=true,
//         erases a matching map2DList entry for a real (non-NULL_IC) site.
//    [O2] InitialBarcodeScanOutShuttle1 resets its own cursor + the shared
//         duplicate-fail counter (golden BarCode.cpp:606-609 -- NOT
//         BarCode_Sh1.cpp, see header file-head discrepancy note).
//    [O3] InitialShuttleFloatCheck1 resets its own cursor + bCCDcheckShuttle[0]
//         (golden :4218-4222).
//    [O4] DoBarcodeTriggerInShuttle_1 all-NULL_IC short-circuit: case 1000's
//         all-NULL_IC guard (golden :2493-2498) fires immediately for
//         iNowMoveStep==iShtCol-1 and the cursor advances straight to 1200
//         without ever calling MotorMove -- mirrors the sibling Sh2 unit's O3.
//    [O5] DoBarcodeScanInShuttle_1 64-tick pump with an all-NULL_IC shuttle:
//         cursor stays within the documented switch(Task) universe and the
//         function completes (returns true) within the tick budget.
//    [O6] DoBarcodeScanOutShuttle_1 64-tick pump with an all-NULL_IC FRCarryKit:
//         same shape as O5 (case 1000->1100->1200->2000).
//    [O7] DoShuttleFloatCheck_1 fast-exit: FLCarryKit.HasRealIC()==false makes
//         case 1 return true immediately (golden :4228-4232) without touching
//         any CCD/timer state.
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here means
//  clean g++ compile/link + the SM cursors landing where hand-derived from the
//  golden switch(Task) ladder, exactly the same standard applied by every
//  other W5/W6 verify TU in this tree (test_w6_5_shuttle.cpp,
//  test_barcode_shuttle2_scan.cpp, etc).
// =============================================================================
#include "vclcompat/vcl_compat.h"
#include "aHotPlateSubstrate.h"     // InArmSuck / FLCarryKit / FRCarryKit
#include "cprod.h"                  // Prod / TestIF_File / Offset
#include "cmydef.h"                  // NULL_IC / HAS_NULL_IC / HAS_IC
#include "Motor/mymotor.h"           // MOT[] (Sim HAL)
#include "BarCode/BarCode_Shuttle1_Scan.h"
#include <cstdio>

static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) state set for DoBarcodeTriggerInShuttle_1 -----
// golden BarCode_Sh1.cpp:2455-3024 (translated BarCode_Shuttle1_Scan.cpp).
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

// ---- documented switch(Task) state set for DoBarcodeScanInShuttle_1 -------
// golden BarCode_Sh1.cpp:3026-4098.
static bool scanInCursorSane(int t)
{
    switch (t) {
        case 1: case 1000: case 1100: case 1120: case 1150: case 1160:
        case 1170: case 1175: case 1180: case 1200: case 1300: case 2000:
        case 3000: case 3100: case 5000:
            return true;
        default:
            return false;
    }
}

// ---- documented switch(Task) state set for DoBarcodeScanOutShuttle_1 ------
// golden BarCode_Sh1.cpp:4100-4216.
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
    // in/outbound-shuttle SM entry (mirrors test_w6_5_shuttle.cpp's /
    // test_barcode_shuttle2_scan.cpp's baseline).  iMaxRow/iMaxCol are NOT
    // set by TMyKitSuck's ctor (golden separates the "grid capacity" fields
    // from iShtRow/iShtCol); explicitly set them here.
    InArmSuck.iShtRow = 1;
    InArmSuck.iShtCol = 2;
    FLCarryKit.iMaxRow = 2;
    FLCarryKit.iMaxCol = 2;
    FRCarryKit.iMaxRow = 2;
    FRCarryKit.iMaxCol = 2;
    for (int i = 0; i < FLCarryKit.iMaxRow; i++)
        for (int j = 0; j < FLCarryKit.iMaxCol; j++)
        {
            FLCarryKit.Item[i][j] = NULL_IC;
            FLCarryKit.cDeviceInf[i][j] = "";
        }
    for (int i = 0; i < FRCarryKit.iMaxRow; i++)
        for (int j = 0; j < FRCarryKit.iMaxCol; j++)
        {
            FRCarryKit.Item[i][j] = NULL_IC;
            FRCarryKit.cDeviceInf[i][j] = "";
        }
}

int main()
{
    printf("=== BarCode_Shuttle1_Scan translation verification ===\n");
    printf("(cursor progression hand-derived from golden BarCode_Sh1.cpp/BarCode.cpp;\n");
    printf(" original BCB6 binary NOT run -- no Borland compiler available)\n\n");

    ResetSim1Row2ColAllNullIC();

    // -------------------------------------------------------------------
    // [O1] InitialBarcodeScanInShuttle1 reset + map2DList erase
    // -------------------------------------------------------------------
    printf("-- O1: InitialBarcodeScanInShuttle1 reset + 2DID clear --\n");
    {
        iInitialBarcodeInShuttle1Task = 999;   // poison
        iShtDupilcateFailCnt[0] = 7;            // poison (shared w/ Sh2 unit -- canonical def there)

        // Seed a real (non-NULL_IC) site with a device code that IS present
        // in map2DList, so the bClear2DID==true path (golden :55-76) has
        // something real to erase.
        FLCarryKit.Item[0][0] = HAS_IC;
        FLCarryKit.cDeviceInf[0][0] = "ABC123";
        map2DList[AnsiString("ABC123")] = AnsiString("some-record");

        InitialBarcodeScanInShuttle1(true);

        CHECK(iInitialBarcodeInShuttle1Task == 1, "iInitialBarcodeInShuttle1Task reset to 1");
        CHECK(iShtDupilcateFailCnt[0] == 0, "iShtDupilcateFailCnt[0] reset to 0");
        CHECK(map2DList.find(AnsiString("ABC123")) == map2DList.end(),
              "matching map2DList entry erased for the real-IC site (golden bClear2DID path)");

        // restore fixture for later oracles
        FLCarryKit.Item[0][0] = NULL_IC;
        FLCarryKit.cDeviceInf[0][0] = "";
    }

    // -------------------------------------------------------------------
    // [O2] InitialBarcodeScanOutShuttle1 reset (golden BarCode.cpp:606-609)
    // -------------------------------------------------------------------
    printf("-- O2: InitialBarcodeScanOutShuttle1 reset --\n");
    {
        iInitialBarcodeOutShuttle1Task = 999;   // poison
        iShtDupilcateFailCnt[0] = 5;              // poison
        InitialBarcodeScanOutShuttle1();
        CHECK(iInitialBarcodeOutShuttle1Task == 1, "iInitialBarcodeOutShuttle1Task reset to 1");
        CHECK(iShtDupilcateFailCnt[0] == 0, "iShtDupilcateFailCnt[0] reset to 0");
    }

    // -------------------------------------------------------------------
    // [O3] InitialShuttleFloatCheck1 reset
    // -------------------------------------------------------------------
    printf("-- O3: InitialShuttleFloatCheck1 reset --\n");
    {
        iShuttleFloatCheck1Task = -1;   // poison
        bCCDcheckShuttle[0] = false;
        InitialShuttleFloatCheck1();
        CHECK(iShuttleFloatCheck1Task == 1, "iShuttleFloatCheck1Task reset to 1");
        CHECK(bCCDcheckShuttle[0] == true, "bCCDcheckShuttle[0] set true");
    }

    // -------------------------------------------------------------------
    // [O4] DoBarcodeTriggerInShuttle_1 all-NULL_IC short-circuit
    // -------------------------------------------------------------------
    printf("-- O4: InShuttle1 trigger, all-NULL_IC short-circuit --\n");
    {
        ResetSim1Row2ColAllNullIC();
        InitialBarcodeScanInShuttle1(true);
        CHECK(iInitialBarcodeInShuttle1Task == 1, "trigger cursor starts at 1");

        // one tick: case 1 falls through into case 1000, whose all-NULL_IC
        // guard fires immediately for iNowMoveStep == iShtCol-1 == 1 (both
        // rows NULL_IC) -> Task=1200 without any MotorMove call.
        DoBarcodeTriggerInShuttle_1();
        CHECK(iInitialBarcodeInShuttle1Task == 1200,
              "case 1000 all-NULL_IC short-circuit advances cursor to 1200 with no motor move");
    }

    // -------------------------------------------------------------------
    // [O5] DoBarcodeScanInShuttle_1 64-tick pump (all-NULL_IC shuttle)
    // -------------------------------------------------------------------
    printf("-- O5: InShuttle1 scan (legacy path) cursor progression --\n");
    {
        ResetSim1Row2ColAllNullIC();
        InitialBarcodeScanInShuttle1(true);
        CHECK(iInitialBarcodeInShuttle1Task == 1, "scan cursor starts at 1");

        bool cursorStayedSane = true;
        bool finishedOK = false;
        for (int tick = 0; tick < 64; tick++)
        {
            bool finished = DoBarcodeScanInShuttle_1(false);
            if (!scanInCursorSane(iInitialBarcodeInShuttle1Task))
            {
                cursorStayedSane = false;
                printf("    (escaped cursor set at tick %d: Task=%d)\n", tick, iInitialBarcodeInShuttle1Task);
                break;
            }
            if (finished)
            {
                finishedOK = true;
                break;
            }
        }
        CHECK(cursorStayedSane, "DoBarcodeScanInShuttle_1 64-tick pump stays in documented switch(Task) set");
        CHECK(finishedOK, "DoBarcodeScanInShuttle_1 completes a full cycle (all-NULL_IC shuttle, Sim MotorMove always succeeds)");
    }

    // -------------------------------------------------------------------
    // [O6] DoBarcodeScanOutShuttle_1 64-tick pump (all-NULL_IC FRCarryKit)
    // -------------------------------------------------------------------
    printf("-- O6: OutShuttle1 scan cursor progression --\n");
    {
        ResetSim1Row2ColAllNullIC();
        InitialBarcodeScanOutShuttle1();
        CHECK(iInitialBarcodeOutShuttle1Task == 1, "out-scan cursor starts at 1");

        bool cursorStayedSane = true;
        bool finishedOK = false;
        for (int tick = 0; tick < 64; tick++)
        {
            bool finished = DoBarcodeScanOutShuttle_1();
            if (!scanOutCursorSane(iInitialBarcodeOutShuttle1Task))
            {
                cursorStayedSane = false;
                printf("    (escaped cursor set at tick %d: Task=%d)\n", tick, iInitialBarcodeOutShuttle1Task);
                break;
            }
            if (finished)
            {
                finishedOK = true;
                break;
            }
        }
        CHECK(cursorStayedSane, "DoBarcodeScanOutShuttle_1 64-tick pump stays in documented switch(Task) set");
        CHECK(finishedOK, "DoBarcodeScanOutShuttle_1 completes a full cycle (all-NULL_IC shuttle, Sim MotorMove always succeeds)");
    }

    // -------------------------------------------------------------------
    // [O7] DoShuttleFloatCheck_1 fast-exit (no real IC on the shuttle)
    // -------------------------------------------------------------------
    printf("-- O7: ShuttleFloatCheck1 fast-exit (no real IC) --\n");
    {
        ResetSim1Row2ColAllNullIC();
        InitialShuttleFloatCheck1();
        CHECK(FLCarryKit.HasRealIC() == false, "fixture: FLCarryKit has no real IC");

        bool result = DoShuttleFloatCheck_1();
        CHECK(result == true, "DoShuttleFloatCheck_1 returns true immediately when HasRealIC()==false (golden :4228-4232)");
    }

    printf("\n=== SUMMARY: %d passed, %d failed ===\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
