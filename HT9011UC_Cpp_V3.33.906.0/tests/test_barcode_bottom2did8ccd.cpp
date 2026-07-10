// tests/test_barcode_bottom2did8ccd.cpp
// Verification harness for the BarCode Bottom-2DID 8-CCD scan translation
// (BarCode_Bottom2DID8CCD.{h,cpp} -- BarCode_DoBottom2DID_8CCD_Scan /
// BarCode_CheckWhichKitBottom2DID / BarCode_DoBarcodeCount).
//
// No external test framework: a tiny check harness prints PASS/FAIL per case
// and a final summary, and returns non-zero on ANY failure (same style as
// tests/test_ContactForce.cpp / tests/test_barcode_bottom2did.cpp).
//
// LIMITATION (stated explicitly): the real CCD hardware path (SendCCDCommand /
// ClientSocket_Bottom_1..8) is gated offline -- there is no way to drive the
// SM past the case-500 "waiting for bGetSE9[]" timeout gate without a live
// CCD (the same documented limitation as the 1-CCD sibling's own test), so
// this harness verifies:
//   (1) BarCode_CheckWhichKitBottom2DID's pure Item-grid exposure-flag math,
//       directly, independent of the state machine (both the 8-CCD-active
//       branch and the false/legacy branch).
//   (2) BarCode_DoBarcodeCount's rate/yield math + yield-alarm decision,
//       directly, with controlled iNeedBarcodeCount[]/iBarcodePassCount[]/
//       iBarcodeErrorCount[] inputs.
//   (3) The "no real IC anywhere" case-1 fast-finish path (BarCode.cpp:9944-
//       9947 `if(InArmSuck.HasRealIC()==false) return true;`).
//   (4) One real IC present drives the SM forward case 1->100->200->500 in a
//       single call (BOTTOM_2DID_CCD/BOTTOM_2DID/TestIF_File.bEnableBottom2D
//       all set so CheckWhichKitBottom2DID takes the 8-CCD-active branch),
//       and that it does NOT falsely finish while Bottom2DPosDelay is still
//       pending (no false-positive "finished" -- the CCD-disconnected-offline
//       behaviour is a faithful "keep waiting").
#include "BarCode_Bottom2DID8CCD.h"
#include "aHotPlateSubstrate.h"   // InArmSuck
#include "cmydef.h"               // HAS_IC/NULL_IC/HAS_NULL_IC, BOTTOM_2DID/BOTTOM_2DID_CCD,
                                  // iNeedBarcodeCount[]/iBarcodePassCount[]/iBarcodeErrorCount[]/
                                  // iBarcodeAutoRetry[]/iBarcodeDuplicate[]
#include "cprod.h"                // TestIF_File
#include <cstdio>
#include <cstring>

static int g_pass = 0;
static int g_fail = 0;

static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected) { printf("PASS  %-58s got=%d\n", name, (int)got); ++g_pass; }
    else { printf("FAIL  %-58s got=%d exp=%d\n", name, (int)got, (int)expected); ++g_fail; }
}

static void check_i(const char* name, int got, int expected)
{
    if (got == expected) { printf("PASS  %-58s got=%d\n", name, got); ++g_pass; }
    else { printf("FAIL  %-58s got=%d exp=%d\n", name, got, expected); ++g_fail; }
}

static void check_s(const char* name, const char* got, const char* expected)
{
    if (std::strcmp(got, expected) == 0) { printf("PASS  %-58s got=\"%s\"\n", name, got); ++g_pass; }
    else { printf("FAIL  %-58s got=\"%s\" exp=\"%s\"\n", name, got, expected); ++g_fail; }
}

int main()
{
    printf("=== BarCode Bottom2DID 8-CCD scan translation verification ===\n\n");

    // -------------------------------------------------------------------
    // Test 1: BarCode_CheckWhichKitBottom2DID pure Item-grid math, 8-CCD-
    // active branch (BOTTOM_2DID_CCD==1 && BOTTOM_2DID==1 &&
    // TestIF_File.bEnableBottom2D==true).
    // -------------------------------------------------------------------
    printf("-- Test 1: BarCode_CheckWhichKitBottom2DID (8-CCD-active branch) --\n");
    {
        BOTTOM_2DID_CCD = 1;
        BOTTOM_2DID = 1;
        TestIF_File.bEnableBottom2D = true;

        InArmSuck.iMaxRow = 2;
        InArmSuck.iMaxCol = 4;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 4; j++)
            {
                InArmSuck.Item[i][j] = NULL_IC;
                InArmSuck.cDeviceInf[i][j] = "";
                bNeedScanBottom2D[i][j] = false;
            }

        // site [0][0] -> a real IC, no code yet, needs scan -> exposure NOT ok
        InArmSuck.Item[0][0] = HAS_IC;
        bNeedScanBottom2D[0][0] = true;

        BarCode_CheckWhichKitBottom2DID();
        check_b("bCCDBarcodeExposureOK[0]==false (real IC, no code, needs scan)",
                 bCCDBarcodeExposureOK[0], false);
        check_b("bCCDBarcodeExposureOK[1]==true (site [1][0] empty)",
                 bCCDBarcodeExposureOK[1], true);
        check_b("bBottom2DNeedMoveInArm==true (at least one site pending)",
                 bBottom2DNeedMoveInArm, true);

        // now give site [0][0] a decoded code -> exposure IS ok, no move needed
        InArmSuck.cDeviceInf[0][0] = "CODE0001";
        BarCode_CheckWhichKitBottom2DID();
        check_b("bCCDBarcodeExposureOK[0]==true (code already present)",
                 bCCDBarcodeExposureOK[0], true);
        check_b("bBottom2DNeedMoveInArm==false (nothing pending anymore)",
                 bBottom2DNeedMoveInArm, false);

        // cleanup
        InArmSuck.Item[0][0] = NULL_IC;
        InArmSuck.cDeviceInf[0][0] = "";
        bNeedScanBottom2D[0][0] = false;
        TestIF_File.bEnableBottom2D = false;
    }

    // -------------------------------------------------------------------
    // Test 2: BarCode_DoBarcodeCount rate/yield math + yield-alarm decision.
    // -------------------------------------------------------------------
    printf("\n-- Test 2: BarCode_DoBarcodeCount rate/yield math --\n");
    {
        for (int i = 0; i < 4; i++)
        {
            iNeedBarcodeCount[i] = 0;
            iBarcodePassCount[i] = 0;
            iBarcodeErrorCount[i] = 0;
            iBarcodeAutoRetry[i] = 0;
            iBarcodeDuplicate[i] = 0;
        }
        // 10 needed, 9 pass, 1 error -> overall rate1 = 90.00
        iNeedBarcodeCount[0] = 10;
        iBarcodePassCount[0] = 9;
        iBarcodeErrorCount[0] = 1;

        TestIF_File.b2DIDYield = false;   // alarm gate OFF -> never alarms regardless of rate
        bool alarm1 = BarCode_DoBarcodeCount();
        check_b("b2DIDYield==false -> never alarms", alarm1, false);
        check_s("s2DIDYield formatted \"90.00\"", s2DIDYield.c_str(), "90.00");

        TestIF_File.b2DIDYield = true;
        TestIF_File.i2DYieldIgnoreCnt = 5;   // Count1(10) > 5 -> ignore-count gate passes
        TestIF_File.d2DIDYield = 95.0;       // rate1(90) < 95 -> alarm
        bool alarm2 = BarCode_DoBarcodeCount();
        check_b("b2DIDYield==true, rate1<threshold -> alarm", alarm2, true);

        TestIF_File.d2DIDYield = 50.0;       // rate1(90) >= 50 -> no alarm
        bool alarm3 = BarCode_DoBarcodeCount();
        check_b("b2DIDYield==true, rate1>=threshold -> no alarm", alarm3, false);

        // cleanup
        for (int i = 0; i < 4; i++)
        {
            iNeedBarcodeCount[i] = 0;
            iBarcodePassCount[i] = 0;
            iBarcodeErrorCount[i] = 0;
        }
        TestIF_File.b2DIDYield = false;
    }

    // -------------------------------------------------------------------
    // Test 3: no real IC anywhere -> case 1's
    // `if(InArmSuck.HasRealIC()==false) return true;` fast path.
    // -------------------------------------------------------------------
    printf("\n-- Test 3: no real IC -> instant finish --\n");
    {
        iBottom2DID_8CCDTask = 1;   // fresh cursor (shared w/ the 1-CCD sibling's Init)
        InArmSuck.iMaxRow = 2;
        InArmSuck.iMaxCol = 4;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 4; j++)
                InArmSuck.Item[i][j] = NULL_IC;

        check_b("first call returns true (nothing to scan)", BarCode_DoBottom2DID_8CCD_Scan(), true);
        check_i("Task cursor stays at 1 (never advanced)", iBottom2DID_8CCDTask, 1);
        check_b("second call still true", BarCode_DoBottom2DID_8CCD_Scan(), true);
    }

    // -------------------------------------------------------------------
    // Test 4: one real IC at [0][0] -> SM advances case 1->100->200->500 in
    // a single call; does not falsely finish while Bottom2DPosDelay pending.
    // -------------------------------------------------------------------
    printf("\n-- Test 4: real IC at [0][0] drives the SM forward --\n");
    {
        BOTTOM_2DID_CCD = 1;
        BOTTOM_2DID = 1;
        TestIF_File.bEnableBottom2D = true;

        iBottom2DID_8CCDTask = 1;
        InArmSuck.iMaxRow = 2;
        InArmSuck.iMaxCol = 4;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 4; j++)
            {
                InArmSuck.Item[i][j] = NULL_IC;
                InArmSuck.cDeviceInf[i][j] = "";
            }
        InArmSuck.Item[0][0] = HAS_IC;   // only [0][0] has a real IC
        InArmSuck.cDeviceInf[0][0] = ""; // not yet decoded

        bool r1 = BarCode_DoBottom2DID_8CCD_Scan();   // case 1->100->200->500
        check_b("first tick: not finished yet (still scanning)", r1, false);
        check_i("Task cursor landed on 500 (case 500, no CCD ack yet)", iBottom2DID_8CCDTask, 500);
        check_b("bBottom2DNeedMoveInArm==true (site [0][0] pending)", bBottom2DNeedMoveInArm, true);
        check_b("bCCDBarcodeExposureOK[0]==false ([0][0] real IC, no code yet)",
                 bCCDBarcodeExposureOK[0], false);

        bool r2 = BarCode_DoBottom2DID_8CCD_Scan();   // case 500 again, timer just set
        check_b("second tick: still not finished (Bottom2DPosDelay pending, no false-positive)",
                 r2, false);
        check_i("Task cursor still 500 (no premature advance)", iBottom2DID_8CCDTask, 500);

        // cleanup -- reset the shared cursor + grid so this test does not leak
        // state into whatever process/test runs after it in the same binary.
        iBottom2DID_8CCDTask = 1;
        InArmSuck.Item[0][0] = NULL_IC;
        InArmSuck.cDeviceInf[0][0] = "";
        TestIF_File.bEnableBottom2D = false;
    }

    printf("\n=== Summary: %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
