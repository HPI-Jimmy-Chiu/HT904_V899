// =============================================================================
//  test_counterclear_core.cpp -- FW-SecCC: TfCounterClear test coverage
//
//  AI(W906-FW-SecCC) 20260819: new file.
//
//  Covers the ACTIVE, file-I/O-free surface of TfCounterClear: ClearCount's
//  in-memory-only cases (ctContactCounts/ctContactCountsHis/ctTimeData/
//  ctIndexCount/ctBinCount), LowYieldSpecialInitail, and the two checkbox
//  mouse-up handlers (cbSelectAllMouseUp/cbAlarmDataMouseUp).
//
//  DELIBERATELY NOT COVERED (see forms/fCounterClear.h banner for why each
//  is safe to skip rather than a coverage gap):
//    * WriteCTInfo/ReadCTInfo -- delegate to TArm::WriteFile()/ReadFile()
//      (cSocket.cpp:537/:737), which write to a HARD-CODED path under
//      D:\HT9045\system\ with NO env-var or path-injection seam of any kind
//      (verified by reading both bodies in full this wave). Calling either
//      from this test would touch real, possibly production-shared files --
//      exactly what the write-boundary policy exists to prevent. The 3-line
//      per-arm delegation itself is trivial and not worth that risk.
//    * FormShow/AutoClear/spbExeClick -- all three call GetCountClrAuth()
//      (cAuthority.cpp:479) as their first/only gating statement, which reads
//      AuthPath+"Security_new.def" through CheckAndReadIniData -- a REAL file
//      under D:\HT9045\config\ (on the write-gate list) with no redirect
//      hook reachable from TfCounterClear's own public surface (AuthPath
//      itself is a mutable global other tests already redirect -- see
//      tests/test_ga1_cmydb.cpp:173 -- but doing that here would make this
//      test's behaviour depend on whichever OTHER already-linked TU defines
//      AuthPath, which is a main-loop link-composition decision, not this
//      file's to make). ClearCount itself -- what these three ultimately
//      call -- IS fully covered below by calling it directly.
//    * TMySecurity -- entirely unreachable this wave (GATE SEC1/SEC10 in
//      forms/fSecurity.h are the CounterClear file's sibling concern, not
//      this file's; noted here only for completeness -- TfCounterClear does
//      not use TMySecurity at all).
// =============================================================================
#include "forms/fCounterClear.h"

#include "MachineType.h"      // eClearType (ct*), eTrayCount, TEST_MAX_BIN, eAuto1, iAutoRight, MAX_SOCKET_ROW/COL
#include "cmydef.h"            // iTo3Unload, iSVByBinCount, bLowYeildAlarmSpecial(1stPass), bRefreshCount
#include "cprod.h"              // Prod
#include "cSocket.h"             // ArmData[3]/ArmHistory[3], TArm, TMySocket::GetTotal()
#include "LastSet.h"              // LastSet
#include "cAuthority.h"            // authCounterClr[9]

#include <cstdio>
#include <cmath>

// ---------------------------------------------------------------------------
//  Minimal CHECK harness (matches tests/test_contactct_core.cpp precedent)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg) do { \
        if (cond) { g_pass++; } \
        else { g_fail++; std::printf("FAIL: %s (line %d)\n", msg, __LINE__); } \
    } while (0)

// ---------------------------------------------------------------------------
//  Reset the handful of globals every test in this file touches.
// ---------------------------------------------------------------------------
static void ResetGlobals()
{
    for (int i = 0; i < MAX_SOCKET_ROW; i++)
        for (int j = 0; j < MAX_SOCKET_COL; j++)
        {
            ArmData[0]->SetFailCT(i, j, 0);
            ArmData[0]->SetPassCT(i, j, 0);
            ArmData[1]->SetFailCT(i, j, 0);
            ArmData[1]->SetPassCT(i, j, 0);
            ArmData[2]->SetFailCT(i, j, 0);
            ArmData[2]->SetPassCT(i, j, 0);
            ArmHistory[0]->SetFailCT(i, j, 0);
            ArmHistory[0]->SetPassCT(i, j, 0);
            ArmHistory[1]->SetFailCT(i, j, 0);
            ArmHistory[1]->SetPassCT(i, j, 0);
            ArmHistory[2]->SetFailCT(i, j, 0);
            ArmHistory[2]->SetPassCT(i, j, 0);
        }

    for (int k = 0; k < 4; k++)
        for (int i = 0; i < 8; i++)
            LastSet.SystemAccSecond[k][i] = 0;

    for (int k = 0; k < 4; k++)
        LastSet.iIndexInputOutPut[k] = 0;

    for (int k = 0; k < 4; k++)
        for (int i = 0; i < TEST_MAX_BIN; i++)
            LastSet.iBinData32[k][i] = 0;

    for (int i = 0; i < TEST_MAX_BIN; i++)
        iSVByBinCount[i] = 0;

    for (int k = 0; k < 4; k++)
        for (int i = 0; i < eTrayCount; i++)
            LastSet.BinCT_PTI[k][iTo3Unload[i]] = 0;

    bLowYeildAlarmSpecial = false;
    bLowYeildAlarmSpecial1stPass = false;
    bRefreshCount = false;

    for (int i = 0; i < 9; i++)
        authCounterClr[i] = false;
}

// =============================================================================
//  (1) ClearCount(ctContactCounts) -- golden :177-191
// =============================================================================
static void Test_ClearCount_ContactCounts_ClearsAllThreeArms()
{
    ResetGlobals();
    TfCounterClear f;

    ArmData[0]->SetPassCT(1, 2, 5);
    ArmData[0]->SetFailCT(1, 2, 3);
    ArmData[1]->SetPassCT(0, 0, 7);
    ArmData[2]->SetFailCT(3, 7, 2);
    CHECK(ArmData[0]->ArmSKET[1][2]->GetTotal() == 8, "setup: Arm0 site(1,2) Total=8");

    f.ClearCount(ctContactCounts);

    CHECK(ArmData[0]->ArmSKET[1][2]->GetTotal() == 0, "ClearCount(ctContactCounts): Arm0 site(1,2) cleared");
    CHECK(ArmData[1]->ArmSKET[0][0]->GetTotal() == 0, "ClearCount(ctContactCounts): Arm1 site(0,0) cleared");
    CHECK(ArmData[2]->ArmSKET[3][7]->GetTotal() == 0, "ClearCount(ctContactCounts): Arm2 site(3,7) cleared");
    CHECK(bRefreshCount == true, "ClearCount always sets bRefreshCount=true (golden :391)");
}

// =============================================================================
//  (2) ClearCount(ctContactCountsHis) -- golden :192-206
// =============================================================================
static void Test_ClearCount_ContactCountsHis_ClearsHistoryOnly()
{
    ResetGlobals();
    TfCounterClear f;

    ArmHistory[0]->SetPassCT(2, 2, 4);
    ArmData[0]->SetPassCT(2, 2, 9);   // current-arm data must survive this call
    CHECK(ArmHistory[0]->ArmSKET[2][2]->GetTotal() == 4, "setup: ArmHistory0 site(2,2) Total=4");

    f.ClearCount(ctContactCountsHis);

    CHECK(ArmHistory[0]->ArmSKET[2][2]->GetTotal() == 0, "ClearCount(ctContactCountsHis): history cleared");
    CHECK(ArmData[0]->ArmSKET[2][2]->GetTotal() == 9, "ClearCount(ctContactCountsHis): current-arm data untouched");
}

// =============================================================================
//  (3) ClearCount(ctTimeData) -- golden :245-252 (k<2 only, NOT k<4)
// =============================================================================
static void Test_ClearCount_TimeData_ClearsOnlyFirstTwoSlots()
{
    ResetGlobals();
    TfCounterClear f;

    for (int k = 0; k < 4; k++)
        for (int i = 0; i < 8; i++)
            LastSet.SystemAccSecond[k][i] = 100 + k;

    f.ClearCount(ctTimeData);

    CHECK(LastSet.SystemAccSecond[0][3] == 0, "ClearCount(ctTimeData): slot k=0 cleared");
    CHECK(LastSet.SystemAccSecond[1][3] == 0, "ClearCount(ctTimeData): slot k=1 cleared");
    CHECK(LastSet.SystemAccSecond[2][3] == 102, "ClearCount(ctTimeData): slot k=2 NOT cleared (golden loop is k<2)");
    CHECK(LastSet.SystemAccSecond[3][3] == 103, "ClearCount(ctTimeData): slot k=3 NOT cleared (golden loop is k<2)");
}

// =============================================================================
//  (4) ClearCount(ctIndexCount) -- golden :263-268
// =============================================================================
static void Test_ClearCount_IndexCount_ClearsAllFourSlots()
{
    ResetGlobals();
    TfCounterClear f;

    for (int k = 0; k < 4; k++)
        LastSet.iIndexInputOutPut[k] = 77;

    f.ClearCount(ctIndexCount);

    for (int k = 0; k < 4; k++)
        CHECK(LastSet.iIndexInputOutPut[k] == 0, "ClearCount(ctIndexCount): all 4 slots cleared");
}

// =============================================================================
//  (5) ClearCount(ctBinCount) -- golden :253-262
// =============================================================================
static void Test_ClearCount_BinCount_ClearsBinData32AndSVCount()
{
    ResetGlobals();
    TfCounterClear f;

    LastSet.iBinData32[0][5] = 42;
    LastSet.iBinData32[3][9] = 13;
    iSVByBinCount[5] = 9;
    iSVByBinCount[9] = 4;

    f.ClearCount(ctBinCount);

    CHECK(LastSet.iBinData32[0][5] == 0, "ClearCount(ctBinCount): iBinData32[0][5] cleared");
    CHECK(LastSet.iBinData32[3][9] == 0, "ClearCount(ctBinCount): iBinData32[3][9] cleared");
    CHECK(iSVByBinCount[5] == 0, "ClearCount(ctBinCount): iSVByBinCount[5] cleared");
    CHECK(iSVByBinCount[9] == 0, "ClearCount(ctBinCount): iSVByBinCount[9] cleared");
}

// =============================================================================
//  (6) LowYieldSpecialInitail -- golden :493-503
// =============================================================================
static void Test_LowYieldSpecialInitail_ResetsFlagsAndPTIBins()
{
    ResetGlobals();
    TfCounterClear f;

    bLowYeildAlarmSpecial = true;
    bLowYeildAlarmSpecial1stPass = true;
    for (int i = 0; i < eTrayCount; i++)
    {
        LastSet.BinCT_PTI[0][iTo3Unload[i]] = 55;
        LastSet.BinCT_PTI[2][iTo3Unload[i]] = 55;
        LastSet.BinCT_PTI[3][iTo3Unload[i]] = 55;
    }

    f.LowYieldSpecialInitail();

    CHECK(bLowYeildAlarmSpecial == false, "LowYieldSpecialInitail: bLowYeildAlarmSpecial reset");
    CHECK(bLowYeildAlarmSpecial1stPass == false, "LowYieldSpecialInitail: bLowYeildAlarmSpecial1stPass reset");
    for (int i = 0; i < eTrayCount; i++)
    {
        CHECK(LastSet.BinCT_PTI[0][iTo3Unload[i]] == 0, "LowYieldSpecialInitail: BinCT_PTI[0] cleared");
        CHECK(LastSet.BinCT_PTI[2][iTo3Unload[i]] == 0, "LowYieldSpecialInitail: BinCT_PTI[2] cleared");
        CHECK(LastSet.BinCT_PTI[3][iTo3Unload[i]] == 0, "LowYieldSpecialInitail: BinCT_PTI[3] cleared");
    }
}

// =============================================================================
//  (7) cbSelectAllMouseUp -- golden :50-77 (authCounterClr-gated toggle)
// =============================================================================
static void Test_CbSelectAllMouseUp_OnlyTogglesAuthorizedBoxes()
{
    ResetGlobals();
    TfCounterClear f;

    // Only indices 0,1,3 authorized -- 4 (Contact Count Curr) is NOT.
    authCounterClr[0] = true;
    authCounterClr[1] = true;
    authCounterClr[3] = true;

    f.cbSelectAll->Checked = true;   // simulate the operator having just checked "Select All"
    f.cbSelectAllMouseUp(NULL);

    CHECK(f.cbAlarmData->Checked == true, "SelectAll(check): authorized cbAlarmData checked");
    CHECK(f.cbTestCategory->Checked == true, "SelectAll(check): authorized cbTestCategory checked");
    CHECK(f.cbLoadingCount->Checked == true, "SelectAll(check): authorized cbLoadingCount checked");
    CHECK(f.cbContactCountCurr->Checked == false, "SelectAll(check): UNauthorized cbContactCountCurr left unchecked");
    CHECK(f.cbSelectAll->Caption == "UnSelect All", "SelectAll(check): caption flips to 'UnSelect All'");

    f.cbSelectAll->Checked = false;  // simulate clicking it again
    f.cbSelectAllMouseUp(NULL);

    CHECK(f.cbAlarmData->Checked == false, "SelectAll(uncheck): authorized cbAlarmData unchecked");
    CHECK(f.cbSelectAll->Caption == "Select All", "SelectAll(uncheck): caption flips back to 'Select All'");
}

// =============================================================================
//  (8) cbAlarmDataMouseUp -- golden :79-88
// =============================================================================
static void Test_CbAlarmDataMouseUp_UnchecksSelectAllWhenAnyBoxCleared()
{
    ResetGlobals();
    TfCounterClear f;

    f.cbSelectAll->Checked = true;
    f.cbSelectAll->Caption = "UnSelect All";
    f.cbAlarmData->Checked = false;   // operator just unchecked this one box

    f.cbAlarmDataMouseUp(f.cbAlarmData);

    CHECK(f.cbSelectAll->Checked == false, "cbAlarmDataMouseUp: cbSelectAll unchecked when a member box is unchecked");
    CHECK(f.cbSelectAll->Caption == "Select All", "cbAlarmDataMouseUp: caption resets to 'Select All'");
}

int main()
{
    Test_ClearCount_ContactCounts_ClearsAllThreeArms();
    Test_ClearCount_ContactCountsHis_ClearsHistoryOnly();
    Test_ClearCount_TimeData_ClearsOnlyFirstTwoSlots();
    Test_ClearCount_IndexCount_ClearsAllFourSlots();
    Test_ClearCount_BinCount_ClearsBinData32AndSVCount();
    Test_LowYieldSpecialInitail_ResetsFlagsAndPTIBins();
    Test_CbSelectAllMouseUp_OnlyTogglesAuthorizedBoxes();
    Test_CbAlarmDataMouseUp_UnchecksSelectAllWhenAnyBoxCleared();

    std::printf("%d/%d checks passed (test_counterclear_core)\n", g_pass, g_pass + g_fail);
    return g_fail == 0 ? 0 : 1;
}
