// =============================================================================
//  test_yieldmon_core.cpp -- FW-3 uYieldMonitoring Wave A test coverage
//
//  AI(W906-FW3-YieldMon-WA) 20260818: new file.
//
//  Covers (per this wave's task brief):
//   (a) CalculateSiteYield -- seeds ArmData[]->ArmSKET[i][j]->BySitePass/
//       BySiteTotal directly (same "direct-write-globals-then-call" idiom as
//       tests/test_ga1_cprod.cpp), asserts dSiteYield at an exact,
//       float-safe percentage (75%/25%/50% -- all exactly representable in
//       both float and double, sidestepping the iUPH-boundary float-
//       truncation trap: never assert on a x.0 cutoff line) for both the
//       "site enabled, average both arms" case and the "site disabled ->
//       0.0" case (golden's own divide-by-zero-adjacent guard path).
//   (b) SWRingReset/SWRingPush/CheckLowYieldAlarm_SW -- three states: FILL
//       (below capacity N), WRAP (push past N, oldest entry evicted,
//       iPassCT/iCount/iHead all checked), and JUDGE (three
//       CheckLowYieldAlarm_SW sub-cases: below-window-size no-op,
//       at-window-size high-yield no-alarm, at-window-size low-yield
//       alarm-fires -- observed via canary_support.h's
//       W906_ShowErrorMessage_Count seam, NOT by mocking DoLowYieldAlarm).
//   (c) CanAutoCloseSite -- one true input, one false input, both through
//       the `iAllSiteOn==0` ("Low Yield 關") branch (the most-condition-heavy
//       of the three iAllSiteOn arms).
//   (d) CheckSettingNo -- boundary: below-iMinCount, above-iMaxCount, and
//       the two fixed-16/1-bounded fields (iContinuousPassBin/_RT), which do
//       NOT use iMinCount/iMaxCount at all.
//
//  SAFETY -- no TfYieldMonitoring ctor exists this wave (see
//  forms/fYieldMonitoring.h's own "NO CONSTRUCTOR THIS WAVE" banner), so
//  there is no GetObserAuth()/CheckAndReadIniDataGeneral()-style production
//  ini-path touch to redirect here -- unlike tests/test_observer_core.cpp,
//  this test needs NO AuthPath/asGeneralPath scratch-directory dance.
// =============================================================================
#include "forms/fYieldMonitoring.h"

#include "MachineType.h"        // MAX_SOCKET_ROW/COL, NN_1Row/NN_2Row test-mode constants, FT_ART-adjacent consts unused here
#include "cmydef.h"              // SystemStart/iHome/bRunAutoClean/bZ1PickShuttle/bZ2PickShuttle/CUSTOMER_CODE/bCanAutoCloseSite
#include "cprod.h"               // Prod/TestIF/TestIF_File/RunInfo, FT, iRunStartMode
#include "LastSet.h"             // LastSet (bUseTestSocket)
#include "Config.h"              // IniConfig
#include "CosFunction.h"         // CosFunction
#include "aHotPlateSubstrate.h"  // TestSocket / FTestSuck (TMyKitSuck)
#include "cSocket.h"             // TArm/TMySocket, ArmData[3]
#include "atester_shims.h"       // fContact (TfContactShim)
#include "canary_support.h"      // W906_ShowErrorMessage_Count/_Reset -- the alarm-fired observability seam

#include <cstdio>

// ---------------------------------------------------------------------------
//  Minimal CHECK harness (matches tests/test_observer_core.cpp / tests/
//  test_ga1_cprod.cpp precedent)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg) do { \
        if (cond) { g_pass++; } \
        else { g_fail++; std::printf("FAIL: %s (line %d)\n", msg, __LINE__); } \
    } while (0)

// ---------------------------------------------------------------------------
//  Shared baseline: every Check*/CalculateSiteYield method's own first guard
//  is `SystemStart && fContact->fShow==false && iHome==0 &&
//  bRunAutoClean==false [&& LastSet.iRunStartMode!=rsmAutoSiteMap]` -- reset
//  once per test group so each test controls only what it actually varies.
// ---------------------------------------------------------------------------
static void ResetCommonGuards()
{
    SystemStart = true;
    fContact->fShow = false;
    iHome = 0;
    bRunAutoClean = false;
    bZ1PickShuttle = false;
    bZ2PickShuttle = false;
    LastSet.iRunStartMode = 0;             // != rsmAutoSiteMap (4, MachineType.h)
    CUSTOMER_CODE = 0;                     // not any CC_* special-cased in this wave's methods
    IniConfig.bKoreaFunction = false;
    CosFunction.bSmartAutoClean = false;
    CosFunction.bYieldAlmNeedOneCycle = false;
    CosFunction.bLowYieldDoOneCycle = false;
    CosFunction.bSpecailLowYeild = false;
    Prod.bFailAlarmLowYieldSpecial = false;
    bLowYeildAlarmSpecial = false;
    bLowYeildAlarmSpecial1stPass = false;
}

// =============================================================================
//  (a) CalculateSiteYield
// =============================================================================
static void Test_CalculateSiteYield_AverageBothArms_And_DisabledSite()
{
    TfYieldMonitoring y;
    ResetCommonGuards();

    // Force IsNNMode()==None_NN (golden cinitial.cpp:7253-7268: only
    // QualSite2X2N/_6Site2X3N/_8Site2X4N/_32Site4X8N/_16Site4X4 take an NN
    // branch) so CalculateSiteYield takes its plain TestSocket.iShtRow/
    // iShtCol `else` branch (the one this wave's task brief names).
    TestIF_File.iTestMode = 0;
    TestIF.iShuttleMode = 0;                // "average both arms" path (golden :3740-3760)
    Prod.bLowYieldAlarmByBin = false;        // GetBySitePCA(), not GetByBinSiteYieldPassPCA()
    Prod.bSlidingWindowYield = false;        // keep the Sliding-Window block (real, ungated) inert for this case
    TestIF_File.iAutoClean_Function = false; // real `else` arm's only ACTIVE line: iAutoClean_FailAlarmSiteYieldIntervalCount=0

    // 2 sites: (0,0) enabled on both arms, (0,1) disabled on both arms.
    TestSocket.iShtRow = 1;
    TestSocket.iShtCol = 2;
    FTestSuck.iShtRow = 1;
    FTestSuck.iShtCol = 2;

    LastSet.bUseTestSocket[0][0][0] = true;
    LastSet.bUseTestSocket[1][0][0] = true;
    LastSet.bUseTestSocket[0][0][1] = false;
    LastSet.bUseTestSocket[1][0][1] = false;

    // 75% and 25% are both exactly representable in IEEE-754 float AND
    // double -- no float/double round-trip precision risk (see file-head
    // note on the iUPH-boundary trap this deliberately avoids).
    ArmData[0]->ArmSKET[0][0]->BySitePass = 3;
    ArmData[0]->ArmSKET[0][0]->BySiteTotal = 4;   // 75%
    ArmData[1]->ArmSKET[0][0]->BySitePass = 1;
    ArmData[1]->ArmSKET[0][0]->BySiteTotal = 4;   // 25%

    y.CalculateSiteYield();

    CHECK(y.dPickerYield[0][0][0] == 75.0, "CalculateSiteYield: dPickerYield[arm0][0][0] == 75.0 (3/4*100)");
    CHECK(y.dPickerYield[1][0][0] == 25.0, "CalculateSiteYield: dPickerYield[arm1][0][0] == 25.0 (1/4*100)");
    CHECK(y.dSiteYield[0][0] == 50.0, "CalculateSiteYield: dSiteYield[0][0] == (75+25)/2 == 50.0 (iShuttleMode==0 averages both arms)");

    CHECK(y.dSiteYield[0][1] == 0.0, "CalculateSiteYield: disabled site (0,1) -> dSiteYield==0.0 (golden's `else` branch)");
    CHECK(y.dPickerYield[0][0][1] == 0.0, "CalculateSiteYield: disabled site (0,1) -> dPickerYield[arm0]==0.0");
    CHECK(y.dPickerYield[1][0][1] == 0.0, "CalculateSiteYield: disabled site (0,1) -> dPickerYield[arm1]==0.0");

    CHECK(y.bFirstCount == false, "CalculateSiteYield: bFirstCount cleared at the tail (golden :3806)");
}

// =============================================================================
//  (b) SWRingReset / SWRingPush -- FILL and WRAP states (direct ring asserts)
// =============================================================================
static void Test_SWRing_FillAndWrap()
{
    TfYieldMonitoring y;

    y.SWRingReset(0, 0, 0);
    CHECK(y.SW[0][0][0].iCount == 0 && y.SW[0][0][0].iPassCT == 0 && y.SW[0][0][0].iHead == 0,
          "SWRingReset zeroes iCount/iPassCT/iHead");

    // FILL: 3 pushes (true,true,false), N=5 -- below capacity, no eviction.
    y.SWRingPush(0, 0, 0, true,  5);
    y.SWRingPush(0, 0, 0, true,  5);
    y.SWRingPush(0, 0, 0, false, 5);
    CHECK(y.SW[0][0][0].iCount == 3, "SWRing FILL: iCount==3 after 3 pushes (below N=5, no eviction)");
    CHECK(y.SW[0][0][0].iPassCT == 2, "SWRing FILL: iPassCT==2 (2 of the 3 pushed were pass)");
    CHECK(y.SW[0][0][0].iHead == 3, "SWRing FILL: iHead==3 (advances once per push regardless of eviction)");

    // WRAP: 3 more pushes (true,false,true) onto the same buffer, N=5 each
    // time. Sequence overall: T,T,F,T,F,T (6 pushes). The 6th push evicts
    // the OLDEST surviving entry (push #1, T) before inserting -- window
    // becomes pushes #2..#6 = T,F,T,F,T = 3 pass / 2 fail.
    y.SWRingPush(0, 0, 0, true,  5);
    y.SWRingPush(0, 0, 0, false, 5);
    y.SWRingPush(0, 0, 0, true,  5);
    CHECK(y.SW[0][0][0].iCount == 5, "SWRing WRAP: iCount capped at N==5 after 6 total pushes");
    CHECK(y.SW[0][0][0].iPassCT == 3, "SWRing WRAP: iPassCT==3 (window is pushes #2..#6 -- oldest pass evicted)");
    CHECK(y.SW[0][0][0].iHead == 6, "SWRing WRAP: iHead==6 (6 total pushes, head never resets on eviction)");
}

// =============================================================================
//  (b) CheckLowYieldAlarm_SW -- JUDGE state: below-window (no-op),
//      at-window+high-yield (no alarm), at-window+low-yield (alarm fires).
//      Observed via canary_support.h's W906_ShowErrorMessage_Count seam --
//      DoLowYieldAlarm's default (all CosFunction/CUSTOMER_CODE flags
//      false/0) branch always ends in exactly one real ShowErrorMessage()
//      call when reached (atester_ProcessCount.cpp:388-391's final `else`),
//      so a +1 delta on the seam's call counter IS "the alarm fired" --
//      the SAME observability seam canary_support.h documents was added
//      specifically so tests could "actually walk the [Retry/Skip] SM"
//      without mocking DoLowYieldAlarm's body.
// =============================================================================
static void Test_CheckLowYieldAlarm_SW_JudgeStates()
{
    TfYieldMonitoring y;
    ResetCommonGuards();

    TestSocket.iShtRow = 1;
    TestSocket.iShtCol = 1;
    LastSet.bUseTestSocket[0][0][0] = true;
    LastSet.bUseTestSocket[1][0][0] = true;

    Prod.iSlidingWindowSize = 5;
    Prod.dLowYieldLimit = 70.0;

    // --- below-window: 3 pushes (< iSlidingWindowSize==5) -> not evaluated at all ---
    y.SWRingReset(0, 0, 0);
    y.SWRingReset(1, 0, 0);
    y.SWRingPush(0, 0, 0, true, 5);
    y.SWRingPush(0, 0, 0, true, 5);
    y.SWRingPush(0, 0, 0, false, 5);
    W906_ShowErrorMessage_Reset();
    y.CheckLowYieldAlarm_SW();
    CHECK(W906_ShowErrorMessage_Count == 0, "CheckLowYieldAlarm_SW: sw_cnt(3) < iSlidingWindowSize(5) -> no evaluation, no alarm");

    // --- at-window, high yield (100%) -> evaluated, but 70 > 100 is false -> no alarm ---
    y.SWRingReset(0, 0, 0);
    y.SWRingReset(1, 0, 0);
    for (int i = 0; i < 5; ++i) y.SWRingPush(0, 0, 0, true, 5);   // 5/5 pass == 100%
    W906_ShowErrorMessage_Reset();
    y.CheckLowYieldAlarm_SW();
    CHECK(W906_ShowErrorMessage_Count == 0, "CheckLowYieldAlarm_SW: sw_cnt==5, yield==100%, dLowYieldLimit(70)>100 is false -> no alarm");

    // --- at-window, low yield (20%) -> evaluated, 70 > 20 is true -> alarm fires ---
    y.SWRingReset(0, 0, 0);
    y.SWRingReset(1, 0, 0);
    y.SWRingPush(0, 0, 0, true,  5);
    y.SWRingPush(0, 0, 0, false, 5);
    y.SWRingPush(0, 0, 0, false, 5);
    y.SWRingPush(0, 0, 0, false, 5);
    y.SWRingPush(0, 0, 0, false, 5);   // 1/5 pass == 20%
    W906_ShowErrorMessage_Reset();
    y.CheckLowYieldAlarm_SW();
    CHECK(W906_ShowErrorMessage_Count == 1, "CheckLowYieldAlarm_SW: sw_cnt==5, yield==20%, dLowYieldLimit(70)>20 is true -> DoLowYieldAlarm fires (WAR0701)");
    CHECK(W906_ShowErrorMessage_LastCode == "WAR0701", "CheckLowYieldAlarm_SW: fired alarm code is golden's literal \"WAR0701\"");
}

// =============================================================================
//  (c) CanAutoCloseSite -- iAllSiteOn==0 ("Low Yield 關") branch, true/false
// =============================================================================
static void Test_CanAutoCloseSite_TrueAndFalse()
{
    TfYieldMonitoring y;

    CosFunction.bLowYieldAutoSiteOff = true;
    iRunStartMode = FT;
    TestIF_File.bLowYieldAutoSiteOff = true;
    Prod.bFailAlarmSiteYieldCmp = true;
    TestIF_File.bLowYieldAutoSiteOffByContiFail = false;
    Prod.bContsFailBySocket = false;
    TestIF_File.bLowYieldAutoSiteOffArmContiFail = false;
    Prod.bContsFailByHead = false;
    TestIF_File.bLowYieldAutoSiteOffByPicker = false;
    Prod.bLowYieldByPicker = false;
    TestIF_File.iAutoSiteOffByGPIB = 0;
    y.bGetGPIBAutoSiteOff = false;

    CHECK(y.CanAutoCloseSite(0) == true, "CanAutoCloseSite(0): bLowYieldAutoSiteOff+iRunStartMode==FT+(bLowYieldAutoSiteOff&&bFailAlarmSiteYieldCmp) -> true");
    CHECK(bCanAutoCloseSite == true, "CanAutoCloseSite(0): also writes the golden global bCanAutoCloseSite (cmydef.h)");

    // Flip the ONE OR-term that was true -> every OR-term now false -> bFlag stays false.
    Prod.bFailAlarmSiteYieldCmp = false;
    CHECK(y.CanAutoCloseSite(0) == false, "CanAutoCloseSite(0): all 5 OR-terms false -> false");
    CHECK(bCanAutoCloseSite == false, "CanAutoCloseSite(0): bCanAutoCloseSite mirrors the false result too");
}

// =============================================================================
//  (d) CheckSettingNo -- boundary clamps
// =============================================================================
static void Test_CheckSettingNo_Boundaries()
{
    TfYieldMonitoring y;
    y.iMinCount = 5;
    y.iMaxCount = 10;

    TestIF_File.iContinuousPassBinCount = 3;      // below iMinCount(5)
    TestIF_File.iContinuousPassBinCount_RT = 999;  // above iMaxCount(10)
    TestIF_File.iContinuousLoaderCount = 7;        // inside [5,10] -- unchanged
    TestIF_File.iContinuousLoaderCount_RT = 5;     // exactly at iMinCount -- unchanged
    TestIF_File.iContinuousContactCount = 10;      // exactly at iMaxCount -- unchanged
    TestIF_File.iContinuousContactCount_RT = 0;    // below iMinCount(5)

    // These two ignore iMinCount/iMaxCount entirely -- fixed [1,16] bounds.
    TestIF_File.iContinuousPassBin = 20;           // above fixed max(16)
    TestIF_File.iContinuousPassBin_RT = 0;         // below fixed min(1)

    y.CheckSettingNo();

    CHECK(TestIF_File.iContinuousPassBinCount == 5, "CheckSettingNo: 3 clamped up to iMinCount(5)");
    CHECK(TestIF_File.iContinuousPassBinCount_RT == 10, "CheckSettingNo: 999 clamped down to iMaxCount(10)");
    CHECK(TestIF_File.iContinuousLoaderCount == 7, "CheckSettingNo: 7 (inside [5,10]) left unchanged");
    CHECK(TestIF_File.iContinuousLoaderCount_RT == 5, "CheckSettingNo: 5 (== iMinCount) left unchanged");
    CHECK(TestIF_File.iContinuousContactCount == 10, "CheckSettingNo: 10 (== iMaxCount) left unchanged");
    CHECK(TestIF_File.iContinuousContactCount_RT == 5, "CheckSettingNo: 0 clamped up to iMinCount(5)");

    CHECK(TestIF_File.iContinuousPassBin == 16, "CheckSettingNo: 20 clamped down to the FIXED max(16), ignoring iMaxCount(10)");
    CHECK(TestIF_File.iContinuousPassBin_RT == 1, "CheckSettingNo: 0 clamped up to the FIXED min(1), ignoring iMinCount(5)");
}

int main()
{
    Test_CalculateSiteYield_AverageBothArms_And_DisabledSite();
    Test_SWRing_FillAndWrap();
    Test_CheckLowYieldAlarm_SW_JudgeStates();
    Test_CanAutoCloseSite_TrueAndFalse();
    Test_CheckSettingNo_Boundaries();

    std::printf("%d/%d checks passed (test_yieldmon_core)\n", g_pass, g_pass + g_fail);
    return g_fail == 0 ? 0 : 1;
}
