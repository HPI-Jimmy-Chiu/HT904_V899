// =============================================================================
//  test_showbinselect_core.cpp -- FW-3 queue item 2: TfShowBinSelect Wave A
//                                   test coverage
//
//  AI(W906-FW3-ShowBinSelect-WA) 20260818: new file.
//
//  Covers the main DATA methods this wave translated (per task brief
//  "涵蓋主要資料方法"): DelDot (pure), ShowInitialString/ShowCategoryBin (the
//  fYieldMonitoring GATE (Y2)-unblocking target), SetAutoVisible/
//  SetLabelVisible.
//
//  NOT COVERED this wave (documented, not silently dropped): CaculateUPH's
//  UPH>0 increment branch (needs OutArmSuck/ShuttleHasIC/IndexHasIC forced
//  true, which in turn needs live motor/sensor state this test does not
//  construct -- the "no IC anywhere" else-branch IS exercised implicitly by
//  every other test's default-false sensor state, just not asserted on
//  directly); UPH_StringGridDblClick/labAuto1Click/btReturnClick/
//  btnClearCountClick/ed_AutoCleanCountClick/btnCleanResetClick (click
//  handlers whose entire live body is GATED this wave, see forms/
//  fShowBinSelect.h GATE REGISTER -- nothing observable to assert on beyond
//  "does not crash").
// =============================================================================
#include "forms/fShowBinSelect.h"

#include "MachineType.h"        // enums, TEST_MAX_BIN, eAuto1/eTrayCount/eBulkBox/tNotUse
#include "cmydef.h"              // iTestBinCount, CUSTOMER_CODE, iSV_ErrBinCnt, iSVByBinCount[]
#include "cprod.h"                // Prod/TestIF/TestIF_File/LastSet-adjacent globals
#include "LastSet.h"               // LastSet
#include "Config.h"                 // IniConfig
#include "CosFunction.h"             // CosFunction
#include "aHotPlateSubstrate.h"       // TestSocket
#include "cSocket.h"                   // ArmData[3] (StrGrdCategoryContCT's GetSelBin source)
#include "atester_shims.h"               // fContact (TfContactShim)

#include <cstdio>
#include <cstring>

// golden free function (cContactCT.h has no declaration for it either --
// same "form as junk drawer" free-function shape as golden's own cShowBinSelect
// .cpp, which declares it file-local with external linkage and no header
// prototype). Forward-declared here since no forms/ header carries it.
AnsiString DelDot(AnsiString asBuffer);

// ---------------------------------------------------------------------------
//  Minimal CHECK harness (matches tests/test_yieldmon_core.cpp precedent)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg) do { \
        if (cond) { g_pass++; } \
        else { g_fail++; std::printf("FAIL: %s (line %d)\n", msg, __LINE__); } \
    } while (0)

// ---------------------------------------------------------------------------
//  (1) DelDot -- golden :178-205, pure string logic
// ---------------------------------------------------------------------------
static void Test_DelDot()
{
    iTestBinCount = 4;

    // "0 1 2 3 . . . ." with bin 2's marker " 2 " present -> strips
    // everything before it.
    AnsiString stripped = DelDot("0 1 2 3 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .");
    CHECK(stripped.SubString(1, 1) != "0", "DelDot: leading \"0 \" prefix removed once bin marker 2 is found");

    // No bin markers at all, but an "E" (error-bin marker) present -> falls
    // back to the golden :198-201 `iBegin=asBuffer.Pos("E")` path.
    AnsiString errOnly = DelDot("E only text");
    CHECK(errOnly == "E only text", "DelDot: falls back to the 'E' marker when no bin index is found");
}

// ---------------------------------------------------------------------------
//  Shared reset -- same idiom as tests/test_contactct_core.cpp's
//  ResetGlobals.
// ---------------------------------------------------------------------------
static void ResetGlobals()
{
    for (int k = 0; k < 3; k++)
        ArmData[k]->ClearALLCT();

    CUSTOMER_CODE = 0;                          // not CC_ASE_CL
    fContact->fShow = false;
    SystemStart = false;                         // skip ShowCategoryBin's alarm-trigger block (golden :1902+)
    iHome = 0;

    CosFunction.bSortingBy2DList = false;
    CosFunction.bLowYieldUseContactCounts = true;   // simplest `sum` derivation (direct iBinData32 sum)
    CosFunction.bCategoryInfoByContactCT = false;
    CosFunction.bUseARTSortCount = false;
    TestIF_File.bSortingBy2DIDList = false;
    LastSet.iTester = 0;                         // != _2D_SORT
    USE_AUTO_RETEST = 0;                          // != eartInstall

    for (int i = 0; i < TEST_MAX_BIN; i++)
    {
        LastSet.iBinData32[0][i] = 0;
        LastSet.iBinData32_ART[0][i] = 0;
    }

    for (int i = 0; i < e3TrayCount; i++)
        Prod.iTrayType[i] = tNotUse;
}

// ---------------------------------------------------------------------------
//  (2) ShowInitialString / ShowCategoryBin -- golden :1634-1700 / :1701-2052
//  This is the direct target of fYieldMonitoring's GATE (Y2)
//  (fShowBinSelect->ShowCategoryBin()).
// ---------------------------------------------------------------------------
static void Test_ShowInitialString_And_ShowCategoryBin_FillsGridsFromBinData()
{
    ResetGlobals();
    TfShowBinSelect f;

    iTestBinCount = 4;
    // bLowYieldUseContactCounts==true -> sum = iBinData32[0][0..3] = 10+20+30+40 = 100.
    LastSet.iBinData32[0][0] = 10;
    LastSet.iBinData32[0][1] = 20;
    LastSet.iBinData32[0][2] = 30;
    LastSet.iBinData32[0][3] = 40;
    LastSet.iBinData32[0][4] = 7;   // "Error Bin" slot (index==iTestBinCount)

    f.ShowInitialString();   // also calls f.ShowCategoryBin() internally, exactly as golden does

    CHECK(f.StrGrdCategory->RowCount == iTestBinCount + 2, "ShowInitialString: StrGrdCategory->RowCount == iTestBinCount+2");
    CHECK(f.StrGrdCategory->Cells[0][1] == "Category 0", "ShowInitialString: row label \"Category 0\"");
    CHECK(f.StrGrdCategory->Cells[0][iTestBinCount + 1] == "Error Bin", "ShowInitialString: last row labelled \"Error Bin\"");

    CHECK(f.StrGrdCategory->Cells[1][1] == "10", "ShowCategoryBin: Cells[1][1] == raw count \"10\"");
    CHECK(f.StrGrdCategory->Cells[1][4] == "40", "ShowCategoryBin: Cells[1][4] == raw count \"40\"");
    CHECK(f.StrGrdCategory->Cells[2][1] == "10.00%", "ShowCategoryBin: Cells[2][1] == \"10.00%\" (10/100)");
    CHECK(f.StrGrdCategory->Cells[2][4] == "40.00%", "ShowCategoryBin: Cells[2][4] == \"40.00%\" (40/100)");

    // CUSTOMER_CODE != CC_ASE_CL branch (golden :1719-1727): iSV_ErrBinCnt/
    // iSVByBinCount[] mirror LastSet.iBinData32[0][] directly.
    CHECK(iSV_ErrBinCnt == 7, "ShowCategoryBin: iSV_ErrBinCnt mirrors iBinData32[0][iTestBinCount]");
    CHECK(iSVByBinCount[0] == 10, "ShowCategoryBin: iSVByBinCount[0] mirrors iBinData32[0][0]");
}

// ---------------------------------------------------------------------------
//  (3) SetAutoVisible / SetLabelVisible -- golden :1425-1477
// ---------------------------------------------------------------------------
static void Test_SetAutoVisible_DrivesPerTrayAndGroupVisibility()
{
    ResetGlobals();
    TfShowBinSelect f;

    iHWFix_BinBox = 1;       // -> gbBinBox->Visible = true, and the BulkBox-caption tail fires
    AUTO3_IS_MAGAZINE = 0;   // -> pnlMag123->Visible = false
    AUTO_EMPTY_COLOR = 2;    // < 3 -> pnlFix789/pnlAuto456->Visible = false; < 4 -> gbAuto6->Visible = false

    Prod.iTrayType[eAuto1] = tNotUse;       // -> SetLabelVisible(eAuto1, false)
    Prod.iTrayType[eAuto1 + 1] = 5;          // any non-tNotUse value -> SetLabelVisible(eAuto1+1, true)

    f.SetAutoVisible();

    CHECK(f.gbBinBox->Visible == true, "SetAutoVisible: gbBinBox->Visible == true when iHWFix_BinBox==1");
    CHECK(f.pnlMag123->Visible == false, "SetAutoVisible: pnlMag123->Visible == false when AUTO3_IS_MAGAZINE==0");
    CHECK(f.pnlFix789->Visible == false, "SetAutoVisible: pnlFix789->Visible == false when AUTO_EMPTY_COLOR<3");
    CHECK(f.pnlAuto456->Visible == false, "SetAutoVisible: pnlAuto456->Visible == false when AUTO_EMPTY_COLOR<3");
    CHECK(f.gbAuto6->Visible == false, "SetAutoVisible: gbAuto6->Visible == false when AUTO_EMPTY_COLOR<4");

    CHECK(f.MyBinSel[eAuto1]->Visible == false, "SetAutoVisible: MyBinSel[eAuto1] hidden (tTrayType==tNotUse)");
    CHECK(f.grpBinDisp[eAuto1]->Visible == false, "SetAutoVisible: grpBinDisp[eAuto1] hidden (tTrayType==tNotUse)");
    CHECK(f.MyBinSel[eAuto1 + 1]->Visible == true, "SetAutoVisible: MyBinSel[eAuto1+1] shown (tTrayType!=tNotUse)");

    CHECK(f.MyBinSel[eBulkBox]->Caption == "E", "SetAutoVisible: MyBinSel[eBulkBox]->Caption==\"E\" (iHWFix_BinBox==1)");
    CHECK(f.MyBinSelARTFT[eBulkBox]->Caption == "E", "SetAutoVisible: MyBinSelARTFT[eBulkBox]->Caption==\"E\"");
}

int main()
{
    Test_DelDot();
    Test_ShowInitialString_And_ShowCategoryBin_FillsGridsFromBinData();
    Test_SetAutoVisible_DrivesPerTrayAndGroupVisibility();

    std::printf("%d/%d checks passed (test_showbinselect_core)\n", g_pass, g_pass + g_fail);
    return g_fail == 0 ? 0 : 1;
}
