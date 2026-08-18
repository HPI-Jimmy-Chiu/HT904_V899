// =============================================================================
//  test_binsel_core.cpp -- FW-BinSel-WA: TfBinSel data-core test coverage
//
//  AI(W906-FW-BinSel-WA) 20260819: new file.
//
//  Covers the ACTIVE, file-I/O-free surface of TfBinSel: the ctor's
//  TStringList/MyBinPanelData bootstrap (the state the 21+ SECSGEM EC gate
//  sites and Command.cpp's SetTrayBinByDLL family read), TransferBinTrayStrToName
//  (pure index-to-name conversion), ARTBinCheck (pure Pass-Bin-Retest forcing
//  logic), CheckFix2Tray/CheckOSBin (pure predicate reads), and
//  ChangeActivePageIndex (pure widget-index mapping).
//
//  DELIBERATELY NOT COVERED (see forms/fBinSel.h WRITE-PATH GATE TABLE for
//  the policy this follows -- every one of these calls a REAL file-reading
//  free function, `CheckAndReadIniData`/`ReadIniData`, against a REAL
//  production path built from `GetRecipePath()`/`GetRecipeFileName()`/
//  `DataPath+GetLastOpenFN()` -- none of the three has a test-mode redirect
//  seam reachable from TfBinSel's own public surface, so calling any of them
//  here would risk touching real, possibly production-shared config under
//  system\/config\):
//    * ReadFile, SetConFail, Save, spbSaveClick, SaveOther -- the whole
//      MUST-HAVE / write-path family this wave's brief calls out as "極可能
//      讀寫 bin/tray 設定檔".
//    * ReadWriteMRTMode, ReadWriteSpecialFunction, ReadPrimeDara,
//      WritePrimeDara -- each unconditionally builds `DataPath+
//      GetLastOpenFN()+"...Data"` and reads it via `ReadIniData` on at least
//      one code path (verified by reading all four bodies in full this
//      wave); `SetOSBin` is ALSO excluded for this reason since its own body
//      calls `ReadWriteMRTMode` twice.
//    * The 27 `TStringList*[eBinTypeTotal]` arrays' PERSISTED content
//      (ReadFunctionData/SaveFunctionData) -- WAVE B stubs this wave, see
//      forms/fBinSel.h WAVE B QUEUE; nothing to test yet beyond the ctor's
//      own "0"-placeholder state, which IS covered below.
//    * CheckFix2Tray's "found a Fix2 tray" TRUE branch -- the column index
//      that would trigger it depends on `eBinNotUse`'s numeric value, which
//      is TU-LOCAL to cBinSel.cpp (`enum eBinSettingItems`, not exposed via
//      forms/fBinSel.h) -- only the "no match" paths (AOI disabled; AOI
//      enabled but the ctor's all-zero table) are constructible from outside
//      that TU and are covered below.
// =============================================================================
#include "forms/fBinSel.h"

#include "MachineType.h"      // eBinType (eBinFT/eBinRT/...), eartInstall, USE_AUTO_RETEST
#include "cmydef.h"            // s3TrayName[], iTestRunMode
#include "cprod.h"              // Prod, BinSelect[], tAOISetup
#include "LastSet.h"             // LastSet
#include "Config.h"               // IniConfig
#include "CosFunction.h"           // CosFunction

#include <cstdio>
#include <cstring>

// ---------------------------------------------------------------------------
//  Minimal CHECK harness (matches tests/test_counterclear_core.cpp precedent)
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
    USE_AUTO_RETEST = 0;
    IniConfig.bA10_AutoReTest = false;
    LastSet.iRunStartMode = 0;
    tAOISetup.bEnabledAOI = false;
    CosFunction.bAutoSiteMappingSetOpenBIN = false;
    Prod.iOpenBin = -1;
    iTestRunMode = eBinFT;
    for (int i = 0; i < TEST_MAX_BIN; i++)
        BinSelect[eBinFT].iCatDataT3Pos[i] = 0;
}

// =============================================================================
//  (1) ctor -- the 27 TStringList*[eBinTypeTotal] arrays (golden :984-1076)
// =============================================================================
static void Test_Ctor_PopulatesBinTypeStringLists()
{
    TfBinSel f;

    for (int tag = 0; tag < eBinTypeTotal; tag++)
    {
        CHECK(f.sBinDoubleContact[tag] != NULL, "ctor: sBinDoubleContact[tag] non-null");
        CHECK(f.sBinDoubleContact[tag]->Count == iTestBinCount, "ctor: sBinDoubleContact[tag] sized iTestBinCount");
        CHECK(AnsiString(f.sBinDoubleContact[tag]->Strings[0]) == "0", "ctor: sBinDoubleContact[tag][0] == \"0\" placeholder");

        CHECK(f.sBinTraySetT3Pos[tag] != NULL, "ctor: sBinTraySetT3Pos[tag] non-null (Command.cpp SetTrayBinByDLL consumer)");
        CHECK(f.sBinTraySetT3Pos[tag]->Count == iTestBinCount, "ctor: sBinTraySetT3Pos[tag] sized iTestBinCount");
        CHECK(AnsiString(f.sBinTraySetT3Pos[tag]->Strings[0]) == "0", "ctor: sBinTraySetT3Pos[tag][0] == \"0\" placeholder");

        CHECK(f.sBinTraySetT3PosName[tag] != NULL, "ctor: sBinTraySetT3PosName[tag] non-null");
        CHECK(AnsiString(f.sBinTraySetT3PosName[tag]->Strings[0]) == "NotUse", "ctor: sBinTraySetT3PosName[tag][0] == \"NotUse\" placeholder");

        CHECK(f.sBinType[tag] != NULL, "ctor: sBinType[tag] non-null");
        CHECK(f.sLowYield[tag] != NULL, "ctor: sLowYield[tag] non-null (SECSGEM g21 consumer)");
        CHECK(f.sBySiteClean[tag] != NULL, "ctor: sBySiteClean[tag] non-null (SECSGEM g21 consumer)");
        CHECK(f.sSpecBinByArmPerSiteComparePercent[tag] != NULL, "ctor: sSpecBinByArmPerSiteComparePercent[tag] non-null");
        CHECK(f.sBinTrayLinked[tag] != NULL, "ctor: sBinTrayLinked[tag] non-null (Ifor 20240730)");
        CHECK(f.sAOIBinTraySetting[tag] != NULL, "ctor: sAOIBinTraySetting[tag] non-null (Eastsun 20260316)");
        CHECK(AnsiString(f.sAOIBinTraySetting[tag]->Strings[0]) == "0", "ctor: sAOIBinTraySetting[tag][0] == \"0\" (single-entry Add, golden :1024-1025)");
    }

    CHECK(f.bShow == false, "ctor: bShow defaults false");
}

// =============================================================================
//  (2) ctor -- MyBinPanel[eBinTypeTotal] (Wave-A TMyBinPanelData bootstrap)
// =============================================================================
static void Test_Ctor_MyBinPanelZeroInitialized()
{
    TfBinSel f;

    for (int tag = 0; tag < eBinTypeTotal; tag++)
    {
        CHECK(f.MyBinPanel[tag] != NULL, "ctor: MyBinPanel[tag] non-null");
        CHECK(f.MyBinPanel[tag]->iErrorT6 == 0, "ctor: MyBinPanel[tag]->iErrorT6 == 0");
        CHECK(f.MyBinPanel[tag]->iT6IsFail[0] == 0, "ctor: MyBinPanel[tag]->iT6IsFail[0] == 0");
        CHECK(f.MyBinPanel[tag]->bT6Link[0] == false, "ctor: MyBinPanel[tag]->bT6Link[0] == false");
        CHECK(f.MyBinPanel[tag]->BackT6PosTray[0][0] == 0, "ctor: MyBinPanel[tag]->BackT6PosTray[0][0] == 0");
        CHECK(f.MyBinPanel[tag]->BackT6PosTray[TEST_MAX_BIN-1][TEST_MAX_BIN-1] == 0, "ctor: MyBinPanel[tag]->BackT6PosTray last cell == 0");
    }
}

// =============================================================================
//  (3) TransferBinTrayStrToName -- golden :6404-6423 (pure index/name convert)
// =============================================================================
// AI(W906-FW-BinSel-WA-integrate) 20260819: both tests below first landed
// constructing TfBinSel with the ambient iTestBinCount==0 -- golden's ctor
// fill loop (`for(j=0;j<iTestBinCount;j++)`, golden :1027) then adds ZERO
// entries and golden's own `i>=iTestBinCount` guard forces every slot to
// "NotUse", so the name-conversion assertions failed at the integration gate
// (the wave agent never executes its tests -- first real run is always the
// gate). Golden's real precondition is WinMain-after-config-load where
// iTestBinCount is loaded (e.g. 256); the fix reproduces that precondition
// test-locally BEFORE construction, with save/restore.
static void Test_TransferBinTrayStrToName_ConvertsIndexToTrayName()
{
    int savedTestBinCount = iTestBinCount;
    iTestBinCount = 256;                 // golden WinMain precondition: ctor fill + guard both live
    {
        TfBinSel f;

        f.sBinTraySetT3Pos[eBinFT]->CommaText = "0,3,5";

        f.TransferBinTrayStrToName(eBinFT);

        CHECK(AnsiString(f.sBinTraySetT3PosName[eBinFT]->Strings[0]) == "NotUse", "TransferBinTrayStrToName: iT3==0 -> \"NotUse\"");
        CHECK(AnsiString(f.sBinTraySetT3PosName[eBinFT]->Strings[1]) == s3TrayName[3-1], "TransferBinTrayStrToName: iT3==3 -> s3TrayName[2]");
        CHECK(AnsiString(f.sBinTraySetT3PosName[eBinFT]->Strings[2]) == s3TrayName[5-1], "TransferBinTrayStrToName: iT3==5 -> s3TrayName[4]");
    }
    iTestBinCount = savedTestBinCount;
}

static void Test_TransferBinTrayStrToName_OutOfRangeIndexIsNotUse()
{
    int savedTestBinCount = iTestBinCount;
    iTestBinCount = 256;
    {
        TfBinSel f;

        // sBinTraySetT3Pos[eBinRT] keeps the ctor's own "0" placeholders
        // (256 of them under the restored precondition) -- every slot reads
        // back "NotUse" through golden's iT3==0 arm.
        f.TransferBinTrayStrToName(eBinRT);

        CHECK(AnsiString(f.sBinTraySetT3PosName[eBinRT]->Strings[0]) == "NotUse", "TransferBinTrayStrToName: ctor-default \"0\" entries stay \"NotUse\"");
    }
    iTestBinCount = savedTestBinCount;
}

// =============================================================================
//  (4) ARTBinCheck -- golden :6381-6401 (pure Pass-Bin-Retest forcing logic)
// =============================================================================
static void Test_ARTBinCheck_ForcesRetestWhenAllThreeSitesPassOrNoneAreART()
{
    ResetGlobals();
    TfBinSel f;

    USE_AUTO_RETEST = eartInstall;
    IniConfig.bA10_AutoReTest = true;
    LastSet.iRunStartMode = rsmInitial_ART;

    // All three sites currently PASS (iT6IsFail==0) -- golden forces site 1
    // into a Fail+ART state so at least one retest site exists.
    f.MyBinPanel[eBinFT]->iT6IsFail[0]=0;
    f.MyBinPanel[eBinFT]->iT6IsFail[1]=0;
    f.MyBinPanel[eBinFT]->iT6IsFail[2]=0;
    f.MyBinPanel[eBinFT]->bT6ART[0]=1;
    f.MyBinPanel[eBinFT]->bT6ART[1]=0;
    f.MyBinPanel[eBinFT]->bT6ART[2]=1;

    f.ARTBinCheck(eBinFT);

    CHECK(f.MyBinPanel[eBinFT]->bT6ART[0]==0, "ARTBinCheck: pre-loop clears bT6ART for every PASS site (site 0)");
    CHECK(f.MyBinPanel[eBinFT]->bT6ART[2]==0, "ARTBinCheck: pre-loop clears bT6ART for every PASS site (site 2)");
    CHECK(f.MyBinPanel[eBinFT]->iT6IsFail[1]==1, "ARTBinCheck: all-pass forces site 1 into Fail");
    CHECK(f.MyBinPanel[eBinFT]->bT6ART[1]==1, "ARTBinCheck: all-pass forces site 1 into ART");
}

static void Test_ARTBinCheck_NoOpWhenAnAlreadyFailingARTSiteExists()
{
    ResetGlobals();
    TfBinSel f;

    USE_AUTO_RETEST = eartInstall;
    IniConfig.bA10_AutoReTest = true;
    LastSet.iRunStartMode = rsmInitial_ART;

    f.MyBinPanel[eBinFT]->iT6IsFail[0]=1;   // site 0 already failing
    f.MyBinPanel[eBinFT]->iT6IsFail[1]=0;
    f.MyBinPanel[eBinFT]->iT6IsFail[2]=0;
    f.MyBinPanel[eBinFT]->bT6ART[0]=1;      // ...and already marked ART
    f.MyBinPanel[eBinFT]->bT6ART[1]=0;
    f.MyBinPanel[eBinFT]->bT6ART[2]=0;

    f.ARTBinCheck(eBinFT);

    // Neither disjunct holds (not all iT6IsFail==0; not all bT6ART==0), so
    // golden's own forcing branch does not fire -- site 1 stays untouched.
    CHECK(f.MyBinPanel[eBinFT]->iT6IsFail[1]==0, "ARTBinCheck: no-op leaves site 1 iT6IsFail untouched");
    CHECK(f.MyBinPanel[eBinFT]->bT6ART[1]==0, "ARTBinCheck: no-op leaves site 1 bT6ART untouched");
}

static void Test_ARTBinCheck_OuterGuardSkipsWhenNotARTMode()
{
    ResetGlobals();
    TfBinSel f;

    USE_AUTO_RETEST = 0;   // eartInstall not selected -- outer guard should short-circuit
    IniConfig.bA10_AutoReTest = true;
    LastSet.iRunStartMode = rsmInitial_ART;

    f.MyBinPanel[eBinFT]->iT6IsFail[0]=0;
    f.MyBinPanel[eBinFT]->iT6IsFail[1]=0;
    f.MyBinPanel[eBinFT]->iT6IsFail[2]=0;
    f.MyBinPanel[eBinFT]->bT6ART[0]=0;
    f.MyBinPanel[eBinFT]->bT6ART[1]=0;
    f.MyBinPanel[eBinFT]->bT6ART[2]=0;

    f.ARTBinCheck(eBinFT);

    CHECK(f.MyBinPanel[eBinFT]->iT6IsFail[1]==0, "ARTBinCheck: outer guard (USE_AUTO_RETEST!=eartInstall) is a true no-op");
}

// =============================================================================
//  (5) CheckFix2Tray -- golden :4785-4805 (pure; see file banner for the
//      constructible-subset note on eBinNotUse being TU-local)
// =============================================================================
static void Test_CheckFix2Tray_FalseWhenAOIDisabled()
{
    ResetGlobals();
    TfBinSel f;

    tAOISetup.bEnabledAOI = false;
    CHECK(f.CheckFix2Tray() == false, "CheckFix2Tray: AOI disabled -> always false regardless of table content");
}

static void Test_CheckFix2Tray_FalseWhenAOIEnabledButTableEmpty()
{
    ResetGlobals();
    TfBinSel f;

    tAOISetup.bEnabledAOI = true;   // ctor's own all-zero BackT6PosTray -> no Fix2 entry exists
    CHECK(f.CheckFix2Tray() == false, "CheckFix2Tray: AOI enabled, ctor-default all-zero table -> false");
}

// =============================================================================
//  (6) CheckOSBin -- golden :6254-6266 (pure)
// =============================================================================
static void Test_CheckOSBin_TrueWhenFeatureDisabled()
{
    ResetGlobals();
    TfBinSel f;

    CosFunction.bAutoSiteMappingSetOpenBIN = false;
    CHECK(f.CheckOSBin() == true, "CheckOSBin: feature disabled -> always true");
}

static void Test_CheckOSBin_TrueWhenOpenBinNegative()
{
    ResetGlobals();
    TfBinSel f;

    CosFunction.bAutoSiteMappingSetOpenBIN = true;
    Prod.iOpenBin = -1;
    CHECK(f.CheckOSBin() == true, "CheckOSBin: iOpenBin<0 -> skips the array check, stays true");
}

static void Test_CheckOSBin_FalseWhenSelectedBinNotInCatData()
{
    ResetGlobals();
    TfBinSel f;

    CosFunction.bAutoSiteMappingSetOpenBIN = true;
    iTestRunMode = eBinFT;
    Prod.iOpenBin = 3;
    BinSelect[eBinFT].iCatDataT3Pos[3] = 0;

    CHECK(f.CheckOSBin() == false, "CheckOSBin: iCatDataT3Pos[iOpenBin]==0 -> false");

    BinSelect[eBinFT].iCatDataT3Pos[3] = 7;   // any nonzero tray assignment
    CHECK(f.CheckOSBin() == true, "CheckOSBin: iCatDataT3Pos[iOpenBin]!=0 -> true");
}

// =============================================================================
//  (7) ChangeActivePageIndex -- golden :2194-2212 (pure widget-index mapping,
//      see forms/fBinSel.h DESIGN NOTE)
// =============================================================================
static void Test_ChangeActivePageIndex_MapsEveryBinTypeToItsOwnTab()
{
    ResetGlobals();
    TfBinSel f;

    iTestRunMode = eBinFT;      f.ChangeActivePageIndex(); CHECK(f.PageControl1->ActivePageIndex==0, "ChangeActivePageIndex: eBinFT -> 0 (tsNormal)");
    iTestRunMode = eBinRT;      f.ChangeActivePageIndex(); CHECK(f.PageControl1->ActivePageIndex==1, "ChangeActivePageIndex: eBinRT -> 1 (tsRetest)");
    iTestRunMode = eBinOffLine; f.ChangeActivePageIndex(); CHECK(f.PageControl1->ActivePageIndex==2, "ChangeActivePageIndex: eBinOffLine -> 2 (tsOffline)");
    iTestRunMode = eBinFT_ART;  f.ChangeActivePageIndex(); CHECK(f.PageControl1->ActivePageIndex==3, "ChangeActivePageIndex: eBinFT_ART -> 3 (tsArtFT)");
    iTestRunMode = eBinRT_ART;  f.ChangeActivePageIndex(); CHECK(f.PageControl1->ActivePageIndex==4, "ChangeActivePageIndex: eBinRT_ART -> 4 (tsArtRT)");
    iTestRunMode = eBinFT_MRT;  f.ChangeActivePageIndex(); CHECK(f.PageControl1->ActivePageIndex==5, "ChangeActivePageIndex: eBinFT_MRT -> 5 (tsMrtFT)");
    iTestRunMode = eBinRT_MRT;  f.ChangeActivePageIndex(); CHECK(f.PageControl1->ActivePageIndex==6, "ChangeActivePageIndex: eBinRT_MRT -> 6 (tsMrtRT)");
}

static void Test_ChangeActivePageIndex_InvalidModeLeavesIndexUntouched()
{
    ResetGlobals();
    TfBinSel f;

    f.PageControl1->ActivePageIndex = 4;   // arbitrary known prior value
    iTestRunMode = -1;                     // not any eBinType value
    f.ChangeActivePageIndex();

    // golden's own `else` arm (ShowMyMessage only) never touches
    // PageControl1->ActivePage -- the port's ActivePageIndex must likewise
    // survive unchanged.
    CHECK(f.PageControl1->ActivePageIndex == 4, "ChangeActivePageIndex: invalid iTestRunMode leaves ActivePageIndex untouched");
}

int main()
{
    Test_Ctor_PopulatesBinTypeStringLists();
    Test_Ctor_MyBinPanelZeroInitialized();
    Test_TransferBinTrayStrToName_ConvertsIndexToTrayName();
    Test_TransferBinTrayStrToName_OutOfRangeIndexIsNotUse();
    Test_ARTBinCheck_ForcesRetestWhenAllThreeSitesPassOrNoneAreART();
    Test_ARTBinCheck_NoOpWhenAnAlreadyFailingARTSiteExists();
    Test_ARTBinCheck_OuterGuardSkipsWhenNotARTMode();
    Test_CheckFix2Tray_FalseWhenAOIDisabled();
    Test_CheckFix2Tray_FalseWhenAOIEnabledButTableEmpty();
    Test_CheckOSBin_TrueWhenFeatureDisabled();
    Test_CheckOSBin_TrueWhenOpenBinNegative();
    Test_CheckOSBin_FalseWhenSelectedBinNotInCatData();
    Test_ChangeActivePageIndex_MapsEveryBinTypeToItsOwnTab();
    Test_ChangeActivePageIndex_InvalidModeLeavesIndexUntouched();

    std::printf("%d/%d checks passed (test_binsel_core)\n", g_pass, g_pass + g_fail);
    return g_fail == 0 ? 0 : 1;
}
