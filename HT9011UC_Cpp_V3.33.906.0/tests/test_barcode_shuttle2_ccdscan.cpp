// =============================================================================
//  test_barcode_shuttle2_ccdscan.cpp  --  W5-BarCode VERIFY:
//    TfBarCode::DoBarcodeCCDInShuttle_2 (BarCode/BarCode_Shuttle2_CCDScan.cpp)
//
//  Translation wave: W5-BarCode (BarCode front, parallel translate batch)
//  Author: AI(W5-BarCode-Shuttle2CCDScan) 20260711
//
//  No external test framework -- a tiny check harness prints PASS/FAIL per case
//  and a final summary, returning non-zero on ANY failure (mirrors
//  tests/test_ContactForce.cpp's style).
//
//  LIMITATION (stated explicitly, same as every other translate-wave test in
//  this tree): no Borland/BCB6 compiler available in this environment, so this
//  is NOT a live diff against the original binary.  Oracles are hand-derived
//  from the golden switch(Task) structure (BarCode_Sh2.cpp:84-2473) + the
//  already-established Sim HAL behavior of BLCarryKit/MOT[]/TQPF_Timer.
// =============================================================================
#include "BarCode/BarCode_Shuttle2_CCDScan.h"
#include "aHotPlateSubstrate.h"   // BLCarryKit
#include "cmydef.h"               // HAS_IC / NULL_IC
#include <cstdio>

static int g_pass = 0;
static int g_fail = 0;

static void check(bool cond, const char *name)
{
    if(cond) { std::printf("PASS  %s\n", name); ++g_pass; }
    else     { std::printf("FAIL  %s\n", name); ++g_fail; }
}

// Every resting ("break"-reached or early-"return"-reached) Task value the
// translated switch(Task) can leave the cursor at (see BarCode_Shuttle2_CCDScan.h
// banner + the golden case-label list, BarCode_Sh2.cpp:84-2473).  1 and 100 are
// included because case 5100/5160's success paths explicitly set Task=1 right
// before `return true;` (golden :5106/:5121ish, translated case 5100/5160), and
// case 100's own early-return leaves Task at 100 (translated case 1 falls
// through into case 100, which can `return true;` before advancing further).
static bool TaskCursorIsSane(int Task)
{
    switch(Task)
    {
        case 1: case 100: case 500: case 1000: case 1120: case 1121:
        case 1150: case 1160: case 1170: case 1180: case 1200: case 1300:
        case 1350: case 2000: case 2500: case 2600: case 3000: case 3100:
        case 3200: case 3300: case 3400: case 3460: case 3470: case 3500:
        case 3600: case 4000: case 4050: case 4100: case 4150: case 4200:
        case 4300: case 5000: case 5050: case 5100: case 5150: case 5160:
        case 5200: case 6000:
            return true;
        default:
            return false;
    }
}

// Reset just the fields this function reads/writes on BLCarryKit so each test
// section starts from a known, deterministic state (BLCarryKit is a shared
// global -- also touched by the W6.5 acarry.cpp engine tests -- so we do not
// assume anything about its state on entry).
static void ResetBLCarryKitForTest(int iShtRow, int iShtCol, int iMaxRow, int iMaxCol)
{
    BLCarryKit.iShtRow = iShtRow;
    BLCarryKit.iShtCol = iShtCol;
    BLCarryKit.iMaxRow = iMaxRow;
    BLCarryKit.iMaxCol = iMaxCol;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<8; j++)
        {
            BLCarryKit.Item[i][j] = NULL_IC;
            BLCarryKit.cDeviceInf[i][j] = "";
        }
    }
}

int main()
{
    std::printf("==== W5-BarCode DoBarcodeCCDInShuttle_2 verify (BarCode_Sh2.cpp:84-2473 over Sim HAL) ====\n");

    // =========================================================================
    //  [O1] Init primes the shared Task cursor to 1 (golden BarCode_Sh2.cpp:31-79,
    //  translated in the sibling BarCode_Shuttle2_Scan.cpp unit -- confirms the
    //  cross-unit wiring: BOTH units agree on iInitialBarcodeInShuttle2Task).
    // =========================================================================
    std::printf("[O1] Init -> shared Task cursor\n");
    ResetBLCarryKitForTest(2, 1, 0, 0);
    BarCode_Sh2_InitialBarcodeScanInShuttle2(true);
    check(iInitialBarcodeInShuttle2Task == 1,
          "O1a BarCode_Sh2_InitialBarcodeScanInShuttle2 -> iInitialBarcodeInShuttle2Task == 1");

    // =========================================================================
    //  [O2] No-real-IC early exit (golden BarCode_Sh2.cpp:53-55 -- case 100's
    //  very first guard): `if(bVerify==false && BLCarryKit.HasRealIC()==false)
    //  return true;`.  case 1 (golden :47-51) sets Task=100 and falls through
    //  into case 100 in the SAME call, so the cursor is LEFT at 100 (never
    //  advances further) when this guard fires.
    // =========================================================================
    std::printf("[O2] No real IC -> immediate return true, Task left at 100\n");
    ResetBLCarryKitForTest(2, 1, 0, 0);   // no IC anywhere -> HasRealIC()==false
    BarCode_Sh2_InitialBarcodeScanInShuttle2(true);
    bool ret2 = BarCode_DoBarcodeCCDInShuttle_2(false);
    check(ret2 == true, "O2a DoBarcodeCCDInShuttle_2(false) with no real IC -> returns true");
    check(iInitialBarcodeInShuttle2Task == 100, "O2b Task left at 100 (golden case1->case100 fallthrough + early return)");

    // =========================================================================
    //  [O3] Real IC present -> case 100 guard is bypassed, the full per-column
    //  setup runs (golden :57-111), falls through into case 500 (golden :113),
    //  which arms a fresh 5-second BarcodePosDelay[1] (golden :138 -- literal
    //  5, not TestIF_File-derived) and `break`s (bGetSE9/.Checked/.Off() all
    //  false/false/not-yet-elapsed).  One tick from Init -> Task==500.
    // =========================================================================
    std::printf("[O3] Real IC present -> one tick lands on Task==500\n");
    ResetBLCarryKitForTest(2, 1, 2, 1);
    BLCarryKit.Item[0][0] = HAS_IC;
    BLCarryKit.Item[1][0] = HAS_IC;
    BarCode_Sh2_InitialBarcodeScanInShuttle2(true);
    check(iInitialBarcodeInShuttle2Task == 1, "O3a Init -> Task == 1 (pre-condition)");
    bool ret3 = BarCode_DoBarcodeCCDInShuttle_2(false);
    std::printf("    observed Task after 1 tick = %d (expect 500)\n", iInitialBarcodeInShuttle2Task);
    check(ret3 == false, "O3b DoBarcodeCCDInShuttle_2(false) with real IC -> does not finish on tick 1 (returns false)");
    check(iInitialBarcodeInShuttle2Task == 500, "O3c Task == 500 after 1 tick (golden case1->100->500 fallthrough)");

    // =========================================================================
    //  [O4] PUMP-SANITY: from here, keep calling the function (no real CCD, no
    //  real socket ack -- every SendCCDCommand/SendSE0 is a gated no-op) and
    //  confirm the cursor NEVER escapes the documented switch(Task) label set
    //  over many ticks (no crash, no silently-undefined Task value).  Golden's
    //  own timeout/retry ladders (WAR0462/WAR0464/BarcodeDelay.Off()/etc.) are
    //  expected to cycle the SM through the exposure/decode-timeout alarm loop
    //  forever when no CCD ever acknowledges -- exactly the real-hardware
    //  behavior for a disconnected CCD, not a translation defect.
    // =========================================================================
    std::printf("[O4] %d-tick pump sanity (no CCD ack, real-hardware-equivalent timeout/retry loop)\n", 500);
    bool escaped = false;
    int escapedAt = -1;
    for(int i=0; i<500; i++)
    {
        BarCode_DoBarcodeCCDInShuttle_2(false);
        if(!TaskCursorIsSane(iInitialBarcodeInShuttle2Task))
        {
            escaped = true;
            escapedAt = iInitialBarcodeInShuttle2Task;
            break;
        }
    }
    check(!escaped, "O4a Task cursor stays within the documented switch(Task) label set over 500 ticks");
    if(escaped)
        std::printf("    escaped to undocumented Task=%d\n", escapedAt);
    else
        std::printf("    final Task after 500-tick pump = %d\n", iInitialBarcodeInShuttle2Task);

    // =========================================================================
    //  [O5] bVerify=true bypasses the "no real IC" early exit (golden :53
    //  `bVerify==false && ...` -- short-circuits false when bVerify==true) even
    //  with an empty kit, so a verify-mode call still progresses the SM.
    // =========================================================================
    std::printf("[O5] bVerify=true bypasses the no-real-IC early exit\n");
    ResetBLCarryKitForTest(2, 1, 0, 0);   // no IC anywhere
    BarCode_Sh2_InitialBarcodeScanInShuttle2(true);
    bool ret5 = BarCode_DoBarcodeCCDInShuttle_2(true);
    std::printf("    observed Task after 1 verify-mode tick = %d (expect 500, NOT an early return)\n", iInitialBarcodeInShuttle2Task);
    check(ret5 == false, "O5a DoBarcodeCCDInShuttle_2(true) does not take the no-real-IC early return");
    check(iInitialBarcodeInShuttle2Task == 500, "O5b Task == 500 (same fallthrough as O3, IC-presence guard bypassed)");

    // =========================================================================
    //  Summary
    // =========================================================================
    std::printf("\n=== SUMMARY: %d passed, %d failed (total %d) ===\n", g_pass, g_fail, g_pass + g_fail);
    if(g_fail != 0)
    {
        std::printf("RESULT: FAIL\n");
        return 1;
    }
    std::printf("RESULT: ALL PASS\n");
    return 0;
}
