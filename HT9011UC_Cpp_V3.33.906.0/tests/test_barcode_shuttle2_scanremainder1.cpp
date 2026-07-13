// =============================================================================
//  test_barcode_shuttle2_scanremainder1.cpp  --  VERIFY: BarCode_Shuttle2_ScanRemainder1.cpp
//
//  Translation wave: W5-BarCode hand-off completion
//  Author: AI(W5-Final-BarCodeShuttle2ScanRemainder1) 20260711
//
//  PURPOSE
//  -------
//  Exercises the one translated free function (BarCode_Sh2_DoBarcodeScanInShuttle_2,
//  see BarCode_Shuttle2_ScanRemainder1.h for full scope) over the Sim HAL with
//  no hardware, proving:
//    - the file compiles/links against the existing translated substrate
//      (aHotPlateSubstrate / cprod / Motor / canary_support / atester_shims /
//      acarry) AND against the sibling BarCode units it reuses shared state
//      from (BarCode_Shuttle1_Scan.o for bHasPin1Error[]/BarcodeWaitReplyTimeOut[]/
//      bCCDBarcodeWaitReply[]/bImageSaveflag[], BarCode_Shuttle2_Scan.o for
//      iBarCode2_1/2/iInitialBarcodeInShuttle2Task/bBarcodeStartDelay[]/
//      BarcodePosDelay[]/BarcodeDelay[], BarCode_Bottom2DID.o for list2DByLot),
//      and
//    - the core switch(Task) SM cursor progresses the way hand-derived from the
//      golden BarCode_Sh2.cpp:3040-4260 source (NOT a live BCB6 binary diff --
//      no Borland compiler available in this environment, same documented
//      limitation as every other W5/W6 verify TU in this tree).
//
//  ORACLES
//  -------
//    [O1] All-NULL_IC shuttle short-circuit: with both shuttle rows entirely
//         NULL_IC, ONE call falls through case 1 -> case 1000 -> case 1100
//         (golden has no `break;` between 1/1000 and 1000/1100, verbatim
//         preserved here) and immediately hits case 1100's "both rows
//         HAS_NULL_IC/NULL_IC" short-circuit (golden :3193-3198), landing the
//         cursor at 1200 with a real `break;` (function returns false, not yet
//         finished) -- fully deterministic, no Sim-HAL motor-position
//         dependency, mirrors the sibling BarCode_Shuttle2_Scan.cpp's own O3
//         for DoBarcodeTriggerInShuttle_2.
//    [O2] Real-IC robustness pump: with ONE real (non-NULL) IC seeded on a
//         1x1 shuttle, the CCD-read gate (Gated_Barcode_StartScan_In) always
//         returns false (no hardware), so this site's bBarcodeNum[] can never
//         become true via a successful scan -- the documented
//         "hardware-gated SM never truly completes, but never escapes its
//         documented switch(Task) universe either" shape already established
//         by every sibling BarCode unit in this wave (see this unit's own .h
//         banner).  Pumps 64 ticks and asserts the cursor never leaves the 15
//         documented switch(Task) case values.
//    [O3] Pin1-mode duplicate-check skip at case 1200 (golden :4075-4080,
//         Eastsun 20260526 #026-4.PinN.Sh2-5): with
//         CosFunction.b2DUsePinInspection && TestIF_File.b2DUsePinInspection
//         both true, forcing the cursor directly to 1200 and pumping ONE tick
//         routes straight to 1300 (`break;` before the duplicate-check loop
//         even runs) -- a deterministic single-tick oracle for a real (not
//         gated) CosFunction-driven branch.  CosFunction/TestIF_File flags are
//         restored to false afterward so this oracle cannot leak state into
//         any test added later in this same binary.
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here means
//  clean g++ compile/link + the SM cursor landing where hand-derived from the
//  golden switch(Task) ladder, exactly the same standard applied by every
//  other W5/W6 verify TU in this tree (test_barcode_shuttle2_scan.cpp,
//  test_barcode_shuttle2_scanremainder2.cpp, etc).
// =============================================================================
#include "vclcompat/vcl_compat.h"
#include "aHotPlateSubstrate.h"     // InArmSuck / BLCarryKit
#include "cprod.h"                  // Prod / TestIF_File / Offset
#include "cmydef.h"                  // NULL_IC / HAS_IC
#include "CosFunction.h"             // CosFunction.b2DUsePinInspection
#include "Motor/mymotor.h"           // MOT[] (Sim HAL)
#include "BarCode/BarCode_Shuttle2_ScanRemainder1.h"
#include <cstdio>

static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) state set for DoBarcodeScanInShuttle_2 --------
// golden BarCode_Sh2.cpp:3040-4260 (translated BarCode_Shuttle2_ScanRemainder1.cpp).
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

static void ResetSim2Row2ColAllNullIC()
{
    // Same "empty shuttle" Sim-HAL baseline as the sibling BarCode units'
    // verify TUs (test_barcode_shuttle2_scan.cpp / test_barcode_shuttle2_scanremainder2.cpp).
    InArmSuck.iShtRow = 2;
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
    printf("=== BarCode_Shuttle2_ScanRemainder1 (DoBarcodeScanInShuttle_2) translation verification ===\n");
    printf("(cursor progression hand-derived from golden BarCode_Sh2.cpp:3040-4260;\n");
    printf(" original BCB6 binary NOT run -- no Borland compiler available)\n\n");

    // -------------------------------------------------------------------
    // [O1] All-NULL_IC shuttle: case 1 -> 1000 -> 1100 short-circuit to 1200
    // -------------------------------------------------------------------
    printf("-- O1: DoBarcodeScanInShuttle_2 all-NULL_IC short-circuit --\n");
    {
        ResetSim2Row2ColAllNullIC();
        BarCode_Sh2_InitialBarcodeScanInShuttle2(true);   // sibling init: sets Task=1 (real, already translated)
        CHECK(iInitialBarcodeInShuttle2Task == 1, "shared cursor starts at 1 (BarCode_Sh2_InitialBarcodeScanInShuttle2)");

        bool finished = BarCode_Sh2_DoBarcodeScanInShuttle_2(false);
        CHECK(finished == false, "one call does not finish (lands at 1200, not a return-true case)");
        CHECK(iInitialBarcodeInShuttle2Task == 1200,
              "case 1->1000->1100 all-NULL_IC short-circuit (golden :3193-3198) advances cursor to 1200 with no motor move");
    }

    // -------------------------------------------------------------------
    // [O2] Real IC present: gated CCD never answers -> cursor stays sane
    // -------------------------------------------------------------------
    printf("-- O2: real IC present, 64-tick robustness pump (CCD gated, SM never completes) --\n");
    {
        // AI(W5-Final-BarCodeRemainder-Integrate) 20260711: this oracle's own
        // documented intent is "the SM keeps trying and never falsely reports
        // success while the CCD is gated" -- but golden case 1150's retry gate
        // (BarCode_Shuttle2_ScanRemainder1.cpp:536, faithfully mirroring golden
        // BarCode_Sh2.cpp:3568 `if(iRetryCount<TestIF_File.iBarcodeRetryCount)`)
        // is itself config-driven, and TestIF_File.iBarcodeRetryCount defaults
        // to 0 (zero-initialised global, never seeded by this test). With 0
        // retries allowed, golden's OWN designed "retries exhausted -> mark
        // this position asBarCodeErrorSend -> iNowCheckStep-- -> case
        // 1300->2000->3000->3100 -> return true" completion path (this is a
        // REAL, intentional golden behaviour -- 6 sibling BarCode Do* functions
        // all gate the identical `iRetryCount<TestIF_File.iBarcodeRetryCount`
        // way, see BarCode_Bottom2DID.cpp/BarCode_Shuttle1_CCDScan.cpp/
        // BarCode_Shuttle1_Scan.cpp/BarCode_Shuttle2_CCDScan.cpp/
        // BarCode_Shuttle2_Scan.cpp) fires on the VERY FIRST attempt, so the
        // SM legitimately finishes (with the position marked as an unreadable-
        // code error, not a false "successfully scanned" report) in far fewer
        // than 64 ticks. That is NOT a translation defect -- it is this test's
        // own setup gap: a production config would always seed a positive
        // retry budget, so seed one here too, restoring it afterward so it
        // cannot leak into O3 (same discipline O3 already applies to its own
        // CosFunction/TestIF_File flags below).
        TestIF_File.iBarcodeRetryCount = 1000;   // far more retries than the 64-tick pump can exhaust

        InArmSuck.iShtRow = 1;
        InArmSuck.iShtCol = 1;
        BLCarryKit.iMaxRow = 2;
        BLCarryKit.iMaxCol = 2;
        for (int i = 0; i < BLCarryKit.iMaxRow; i++)
            for (int j = 0; j < BLCarryKit.iMaxCol; j++)
            {
                BLCarryKit.Item[i][j] = NULL_IC;
                BLCarryKit.cDeviceInf[i][j] = "";
            }
        BLCarryKit.Item[0][0] = HAS_IC;   // one real (non-NULL) IC -> needs an actual 2DID read

        BarCode_Sh2_InitialBarcodeScanInShuttle2(true);
        CHECK(iInitialBarcodeInShuttle2Task == 1, "cursor reset to 1 before the pump");

        bool cursorStayedSane = true;
        bool finishedEarly = false;
        for (int tick = 0; tick < 64; tick++)
        {
            bool finished = BarCode_Sh2_DoBarcodeScanInShuttle_2(false);
            if (!scanInCursorSane(iInitialBarcodeInShuttle2Task))
            {
                cursorStayedSane = false;
                printf("    (escaped cursor set at tick %d: Task=%d)\n", tick, iInitialBarcodeInShuttle2Task);
                break;
            }
            if (finished)
            {
                finishedEarly = true;
                break;
            }
        }
        CHECK(cursorStayedSane, "64-tick pump with a real IC stays in the documented switch(Task) universe");
        CHECK(!finishedEarly, "SM does not falsely report completion (Gated_Barcode_StartScan_In never succeeds offline)");

        TestIF_File.iBarcodeRetryCount = 0;   // restore -- do not leak into O3
    }

    // -------------------------------------------------------------------
    // [O3] Pin1-mode duplicate-check skip at case 1200 (real CosFunction branch)
    // -------------------------------------------------------------------
    printf("-- O3: case 1200 Pin1-mode dup-check skip (golden :4075-4080) --\n");
    {
        ResetSim2Row2ColAllNullIC();
        CosFunction.b2DUsePinInspection = true;
        TestIF_File.b2DUsePinInspection = true;
        iInitialBarcodeInShuttle2Task = 1200;   // seed cursor directly (white-box, same style as sibling test's O5)

        BarCode_Sh2_DoBarcodeScanInShuttle_2(false);
        CHECK(iInitialBarcodeInShuttle2Task == 1300,
              "Pin1 mode routes case 1200 straight to 1300, skipping the duplicate-check loop entirely");

        // restore so this test's global CosFunction/TestIF_File flags cannot
        // leak into any test added later in this same binary
        CosFunction.b2DUsePinInspection = false;
        TestIF_File.b2DUsePinInspection = false;
    }

    printf("\n=== SUMMARY: %d passed, %d failed ===\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
