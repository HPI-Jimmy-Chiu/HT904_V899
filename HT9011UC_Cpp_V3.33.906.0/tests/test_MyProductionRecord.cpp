// test_MyProductionRecord.cpp
// AI(W906-MyProductionRecord) 20260721: acceptance test for the newly-real
// Public/MyProductionRecord.cpp pure-logic slice (ctor/InitialRecord/
// DeleteProductionRecord + ~28 Add*Record/Get* leaves).
//
// ISOLATION NOTE (avoid the god-stack, established precedent -- see
// test_FTPClient_EventHandlers/test_FTPClient_Transfer/test_config_loaders/
// test_MyCCLink in this same directory's CMakeLists.txt for the same shape):
// this test links ht9045_globals + vclcompat + compiles Public/
// MyProductionRecord.cpp directly, but does NOT link aHotPlateSubstrate.cpp
// (part of ht9045_sm). Three reasons:
//   1. aHotPlateSubstrate.cpp (post-reconciliation, AI(W906-AutoCleanFoundation)
//      + AI(W906-MyProductionRecord) landing together) still carries its OWN
//      no-op stand-in bodies for exactly 3 TMyProductionRecord methods --
//      AddErrorRecord, AddTestRecord (golden dead method), SaveRecordCleanPad
//      -- deliberately left there (not translated in this wave) rather than
//      dual-defined; every other method this wave translates has its single
//      real definition in Public/MyProductionRecord.cpp only. Linking both
//      TUs together is therefore safe today, but this isolated test predates
//      that reconciliation and there is no value yet in undoing the isolation.
//   2. The whole ht9045_sm library now DOES compile cleanly (the FormsFacade.h
//      TfMainFont/TfMainAutoCleanGrid gap referenced by an earlier draft of
//      this comment was resolved by the same reconciliation).
//   3. This is a focused unit test for ONE file's pure logic; pulling the
//      entire state-machine substrate for it would be disproportionate even
//      with (1) resolved and (2) no longer blocking.
// AddLoadRecord/AddHPRecord need TestSocket/HotPlateForm/LoadForm; the four
// InArmSuck/OutArmSuck-touching leaves (AddInArmLoaderPickRecord/
// AddOutArmShuttlePickRecord/AddInArmHotplatePickRecord/AddPickCleanPad-family)
// need InArmSuck/OutArmSuck. TestSocket/InArmSuck/OutArmSuck are `extern
// TMyKitSuck` in aHotPlateSubstrate.h; this file defines its OWN LOCAL trivial
// stand-ins for TMyKitSuck's ctor + those 3 globals below (never linked
// alongside the real aHotPlateSubstrate.cpp -- no collision, same "supply
// your own local stand-in" pattern as MyDBIProcess/ShowMyMessage in the
// sibling tests cited above). TMyKitSuck::PordRec[][] (a by-value
// TMyProductionRecord[4][8] grid) means constructing these 3 stand-in globals
// ALSO exercises the real TMyProductionRecord ctor 96 times at program
// startup -- free extra coverage.
//
// NOT tested (deliberately, matching Public/MyProductionRecord.cpp's own
// scope boundary):
//   - AddErrorRecord (declared, no body this wave -- calling it would be a
//     link error, which is the point: it's not supposed to be callable yet)
//   - AddTestRecord (golden dead/commented-out method; the pre-existing
//     aHotPlateSubstrate.cpp no-op IS the correct, final translation -- not
//     redefined here, so this TU cannot call it without relinking that file)
//   - SaveRecordCleanPad / SaveRecord / GetProdLogFileName / GetProdLogFilePath
//     / any other Save* (heavy half, out of scope, undeclared in the header)
//   - AddIndexPickVacuum (undeclared -- see header note)
#include <cstdio>
#include <cstring>

#include "vclcompat/vcl_compat.h"
#include "Public/MyProductionRecord.h"
#include "cprod.h"                  // LoadForm, HotPlateForm, TestIF_File, TRAY_TYPE_PARA
#include "cmydef.h"                 // CUSTOMER_CODE, IndexSuckName[][], iOneTrayPickCount[], iTestBinCount, SystemHour...
#include "MachineType.h"            // CC_KYEC_LEE / CC_Greatek / CC_HANA_MICRON
#include "aHotPlateSubstrate.h"     // TMyKitSuck type + extern InArmSuck/OutArmSuck/TestSocket

// ============================================================================
//  Local stand-ins (see file banner) -- NOT the real aHotPlateSubstrate.cpp.
// ============================================================================
// AI(W906-GA1-B2-integrate) 20260804: the local TMyKitSuck ctor + 3 global
// stand-ins are RETIRED -- the full RESCAN group now supplies the real
// aHotPlateSubstrate.cpp bodies/objects (was a duplicate-definition set).

// ============================================================================
//  Helpers
// ============================================================================
static int g_pass = 0;
static int g_fail = 0;

#define CHECK(expr) do { \
    if (!(expr)) { \
        fprintf(stderr, "FAIL [%s:%d] %s\n", __FILE__, __LINE__, #expr); \
        g_fail++; \
    } else { \
        g_pass++; \
    } \
} while(0)

#define CHECK_STR(actual, expected) do { \
    AnsiString a_ = (actual); \
    AnsiString e_ = (expected); \
    if (!(a_ == e_)) { \
        fprintf(stderr, "FAIL [%s:%d] expected \"%s\" got \"%s\"\n", __FILE__, __LINE__, e_.c_str(), a_.c_str()); \
        g_fail++; \
    } else { \
        g_pass++; \
    } \
} while(0)

// ============================================================================
//  [A] ctor / InitialRecord / DeleteProductionRecord
// ============================================================================
static void test_ctor_and_lifecycle()
{
    TMyProductionRecord rec;
    CHECK(rec.asBuffer != NULL);
    CHECK(rec.asBuffer->Count == eDataTotal);
    CHECK(rec.bUse == false);                       // InitialRecord() ran from the ctor
    // every slot from eLoadNum.. is blanked by InitialRecord's loop
    CHECK_STR(rec.asBuffer->Strings[eLoadNum], "");
    CHECK_STR(rec.asBuffer->Strings[eErrorCode], "");
    // slot 0 (eScheduleName) is only blanked by the ctor's own Add("") loop,
    // NOT by InitialRecord's loop (which starts at eLoadNum=2) -- golden verbatim.
    CHECK_STR(rec.asBuffer->Strings[eScheduleName], "");
    // title strings got built
    CHECK(rec.asDataTitle.Length() > 0);
    CHECK(rec.asDataTitleGreatek.Length() > 0);
    CHECK(rec.asDataTitle_SG.Length() > 0);
    CHECK(rec.asDataTitleCleanPad.Length() > 0);
    CHECK(rec.asJCET2DIDList.Length() > 0);

    // Dirty it, then re-run InitialRecord: buffer count is already eDataTotal,
    // so golden's `if(asBuffer->Count!=eDataTotal)` guard stays false and the
    // dead `TMyProductionRecord();` temporary-construct line does not fire.
    rec.AddOrderTestRecord(7);
    rec.bUse = true;
    rec.InitialRecord();
    CHECK(rec.bUse == false);
    CHECK_STR(rec.asBuffer->Strings[eOrderTest], "");

    // DeleteProductionRecord frees the buffer and nulls the pointer.
    rec.DeleteProductionRecord();
    CHECK(rec.asBuffer == NULL);
}

// ============================================================================
//  [B] Pure asBuffer leaves needing no external substrate
// ============================================================================
static void test_pure_leaves()
{
    TMyProductionRecord rec;

    rec.AddArmSiteRecord(1, 5);
    CHECK_STR(rec.asBuffer->Strings[eArmNO], "1");
    CHECK_STR(rec.asBuffer->Strings[eSiteNO], "5");
    CHECK(rec.asBuffer->GetString(eArmTime).Length() > 0);   // StringsProxy has no .Length(); GetString() returns a plain AnsiString

    rec.AddIndexCycleTimeRecord(1.5);
    CHECK_STR(rec.asBuffer->Strings[eIndexCycleTime], " 1.500");

    rec.AddOrderTestRecord(3);
    CHECK(rec.GetOrderOfContact() == 3);

    rec.AddDataTimeRecord("20260721_120000");
    CHECK(rec.GetSOTTime() == "20260721_120000");

    rec.AddDataTimeEOTRecord("20260721_130000");
    CHECK(rec.GetEOTTime() == "20260721_130000");

    rec.Add2DIDRecord("ABC123");
    CHECK_STR(rec.asBuffer->Strings[e2DCode], "ABC123");

    rec.AddTestTime("00:00:05.123");
    CHECK(rec.GetTestTime() == "00:00:05.123");

    rec.AddTSDTime("00:00:02.000");
    CHECK_STR(rec.asBuffer->Strings[eTSDTime], "00:00:02.000");

    rec.AddTestModeRecord(4);
    CHECK_STR(rec.asBuffer->Strings[eTestMode], "4");

    rec.AddInRotationAngRecord(90);
    CHECK(rec.GetInRotationAngRecord() == "90");

    // AddIndexPickShuttleRecord/AddIndexPlaceShuttleRecord/AddOutShuttleDetectRecord
    // need IndexSuckName[][] (cmydef.cpp, real; no substrate needed).
    IndexSuckName[0][0] = "F1";
    rec.AddIndexPickShuttleRecord(3, 0, 0);
    CHECK_STR(rec.asBuffer->Strings[eIndexPickSHTSiteNo], "F1-3");
    rec.AddIndexPlaceShuttleRecord(4, 0, 0);
    CHECK_STR(rec.asBuffer->Strings[eIndexPlaceSHTSiteNo], "F1-4");
    rec.AddOutShuttleDetectRecord(5, 0, 0);
    CHECK_STR(rec.asBuffer->Strings[eOutSHTDetectSiteNo], "F1-5");
}

// ============================================================================
//  [C] AddErrorLog dedup logic + AddErrorRecordNoSave + AddTestResultRecord
// ============================================================================
static void test_error_log_and_test_result()
{
    TMyProductionRecord rec;

    // AddErrorLog: first write goes straight in.
    rec.AddErrorLog("JAM0109");
    CHECK(rec.GetErrorLog() == "JAM0109");
    // duplicate is NOT re-appended (golden AnsiPos guard).
    rec.AddErrorLog("JAM0109");
    CHECK(rec.GetErrorLog() == "JAM0109");
    // a NEW code is appended with "_".
    rec.AddErrorLog("JAM0110");
    CHECK(rec.GetErrorLog() == "JAM0109_JAM0110");
    // "" / "na" are no-ops.
    rec.AddErrorLog("");
    rec.AddErrorLog("na");
    CHECK(rec.GetErrorLog() == "JAM0109_JAM0110");

    // AddErrorRecordNoSave: position fields + AddErrorLog, no SaveRecord dependency.
    TMyProductionRecord rec2;
    rec2.AddErrorRecordNoSave("WAR0152", true, 10, 20, 30, 40, 50);
    CHECK_STR(rec2.asBuffer->Strings[eUnloadXPos], "10");
    CHECK_STR(rec2.asBuffer->Strings[eUnloadYPos], "20");
    CHECK_STR(rec2.asBuffer->Strings[eUnloadXpPos], "30");
    CHECK_STR(rec2.asBuffer->Strings[eUnloadXp2Pos], "40");
    CHECK_STR(rec2.asBuffer->Strings[eUnloadYpPos], "50");
    CHECK(rec2.GetErrorLog() == "WAR0152");
    // bAddPosition=false (default): no position writes.
    TMyProductionRecord rec3;
    rec3.AddErrorRecordNoSave("WAR0152");
    CHECK_STR(rec3.asBuffer->Strings[eUnloadXPos], "");

    // AddTestResultRecord: iGpibMode!=1 branch, under/over iTestBinCount.
    TestIF_File.iGpibMode = 0;
    iTestBinCount = 8;
    TMyProductionRecord rec4;
    rec4.AddTestResultRecord(3, "3", "");
    CHECK_STR(rec4.asBuffer->Strings[eTestCate], "3");
    rec4.AddTestResultRecord(9, "9", "OverflowBin");   // >= iTestBinCount -> "E"
    CHECK_STR(rec4.asBuffer->Strings[eTestCate], "E");
    CHECK(rec4.GetErrorLog() == "OverflowBin");

    // iGpibMode==1 (256 bin) branch: >=255 -> "E".
    TestIF_File.iGpibMode = 1;
    TMyProductionRecord rec5;
    rec5.AddTestResultRecord(254, "254");
    CHECK_STR(rec5.asBuffer->Strings[eTestCate], "254");
    rec5.AddTestResultRecord(255, "255");
    CHECK_STR(rec5.asBuffer->Strings[eTestCate], "E");
    TestIF_File.iGpibMode = 0;   // restore
}

// ============================================================================
//  [D] AddUnloadRecord (customer-code row/col shift + iOneTrayPickCount)
// ============================================================================
static void test_unload_record()
{
    int save_cc = CUSTOMER_CODE;
    int before = iOneTrayPickCount[1];

    CUSTOMER_CODE = 0;   // not KYEC_LEE/Greatek/HANA_MICRON -> no +1 shift
    TMyProductionRecord rec;
    rec.AddUnloadRecord(/*iWhichAuto*/0, /*iNum*/2, /*iTrayRow*/1, /*iTrayCol*/3,
                        /*iXPos*/100, /*iYPos*/200, /*ascode*/"OK",
                        /*iXPitch*/1, /*iXPitch2*/2, /*iYPitch*/3, /*TrayID*/"TRAY01");
    CHECK_STR(rec.asBuffer->Strings[eUnloadX], "3");     // iTrayCol, unshifted
    CHECK_STR(rec.asBuffer->Strings[eUnloadY], "1");     // iTrayRow, unshifted
    CHECK_STR(rec.asBuffer->Strings[eWhichAuto], "1");
    CHECK_STR(rec.asBuffer->Strings[eOutTrayNum], "O1-2");
    CHECK_STR(rec.asBuffer->Strings[eTrayID], "TRAY01");
    CHECK(iOneTrayPickCount[1] == before + 1);

    CUSTOMER_CODE = CC_KYEC_LEE;   // shifted +1 branch
    TMyProductionRecord rec2;
    rec2.AddUnloadRecord(0, 2, 1, 3, 100, 200, "OK", 1, 2, 3, "TRAY01");
    CHECK_STR(rec2.asBuffer->Strings[eUnloadX], "4");    // iTrayCol+1
    CHECK_STR(rec2.asBuffer->Strings[eUnloadY], "2");    // iTrayRow+1

    CUSTOMER_CODE = save_cc;
}

// ============================================================================
//  [E] AddGroundRecord / AddESDRecord (CosFunction.bRecordGroundESDByTestIC)
// ============================================================================
static void test_ground_esd()
{
    bool save = CosFunction.bRecordGroundESDByTestIC;

    CosFunction.bRecordGroundESDByTestIC = true;
    TMyProductionRecord rec;
    rec.AddGroundRecord("1.23");
    rec.AddESDRecord("4.56");
    CHECK_STR(rec.asBuffer->Strings[eGround], "1.23");
    CHECK_STR(rec.asBuffer->Strings[eESD], "4.56");

    CosFunction.bRecordGroundESDByTestIC = false;
    TMyProductionRecord rec2;
    rec2.AddGroundRecord("1.23");
    rec2.AddESDRecord("4.56");
    CHECK_STR(rec2.asBuffer->Strings[eGround], "NA");
    CHECK_STR(rec2.asBuffer->Strings[eESD], "NA");

    CosFunction.bRecordGroundESDByTestIC = save;
}

// ============================================================================
//  [F] Get* round-trip family
// ============================================================================
static void test_get_family()
{
    int save_cc = CUSTOMER_CODE;
    CUSTOMER_CODE = 0;
    TRAY_TYPE_PARA loadForm; std::memset(&loadForm, 0, sizeof(loadForm));
    loadForm.XDivision = 2; loadForm.YDivision = 4;
    TRAY_TYPE_PARA* save_loadform = LoadForm;
    LoadForm = &loadForm;
    TestSocket.iShtRow = 2; TestSocket.iShtCol = 8;

    TMyProductionRecord rec;
    rec.AddLoadRecord(5, /*iR*/1, /*iC*/2, /*iXPos*/11, /*iYPos*/22, "SCHED1", "20260721_090000");
    CHECK(rec.GetLoaderX() == 2);
    CHECK(rec.GetLoaderY() == 1);
    CHECK(rec.GetLoaderNum() == 5);        // "I1-5" -> SubString(4,..) -> "5"
    CHECK_STR(rec.asBuffer->Strings[eLoadXPos], "11");
    CHECK_STR(rec.asBuffer->Strings[eLoadYPos], "22");
    CHECK(rec.bUse == true);

    rec.AddUnloadRecord(2, 1, 0, 0, 0, 0, "", 0, 0, 0, "");
    CHECK(rec.GetWhcihAuto() == 2);         // eWhichAuto stored as (iWhichAuto+1); -1 undoes it
    CHECK(rec.GetAutoX() == 0);
    CHECK(rec.GetAutoY() == 0);

    rec.AddArmSiteRecord(0, 9);
    CHECK(rec.GetSiteNo() == 9);

    LoadForm = save_loadform;
    CUSTOMER_CODE = save_cc;
}

// ============================================================================
//  [G] AddHPRecord (HotPlateForm + iPlate -> sHPNo mapping)
// ============================================================================
static void test_hp_record()
{
    int save_cc = CUSTOMER_CODE;
    CUSTOMER_CODE = 0;
    TRAY_TYPE_PARA save_hp = HotPlateForm;
    HotPlateForm.XDivision = 1; HotPlateForm.YDivision = 2;

    TMyProductionRecord rec;
    rec.AddHPRecord(0, 3, 4);
    CHECK_STR(rec.asBuffer->Strings[eHotplateNo], "2");   // iPlate==0 -> "2" (golden quirk, preserved)
    CHECK_STR(rec.asBuffer->Strings[eHotX], "4");
    CHECK_STR(rec.asBuffer->Strings[eHotY], "3");

    TMyProductionRecord rec2;
    rec2.AddHPRecord(1, 0, 0);
    CHECK_STR(rec2.asBuffer->Strings[eHotplateNo], "1");  // iPlate==1 -> "1"

    TMyProductionRecord rec3;
    rec3.AddHPRecord(9, 0, 0);
    CHECK_STR(rec3.asBuffer->Strings[eHotplateNo], "na"); // else -> "na"

    HotPlateForm = save_hp;
    CUSTOMER_CODE = save_cc;
}

// ============================================================================
//  [H] InArmSuck/OutArmSuck-touching leaves (sName real; vacuum tail gated --
//      see Public/MyProductionRecord.cpp's #if 0 blocks. Confirms the gated
//      fields stay at InitialRecord's "" default, i.e. the gate really is a
//      no-op today, not silently wrong.)
// ============================================================================
static void test_suck_pick_records()
{
    InArmSuck.Suck[0][0].sName = "InArm_R0C0";
    OutArmSuck.Suck[1][2].sName = "OutArm_R1C2";

    TMyProductionRecord rec;
    rec.AddInArmLoaderPickRecord(0, 0);
    CHECK_STR(rec.asBuffer->Strings[eInArmLoaderPick], "InArm_R0C0");
    CHECK_STR(rec.asBuffer->Strings[eLoaderPickVacuum], "");   // gated tail -> still default

    TMyProductionRecord rec2;
    rec2.AddOutArmShuttlePickRecord(1, 2);
    CHECK_STR(rec2.asBuffer->Strings[eOutArmShuttlePick], "OutArm_R1C2");
    CHECK_STR(rec2.asBuffer->Strings[eOSHTPickVacuum], "");

    TMyProductionRecord rec3;
    rec3.AddInArmHotplatePickRecord(0, 0);
    CHECK_STR(rec3.asBuffer->Strings[eInArmHotplatePick], "InArm_R0C0");
    CHECK_STR(rec3.asBuffer->Strings[eHPPickVacuum], "");
}

// ============================================================================
//  [I] Auto Clean pick/place record family (needs GetTimeInfo()/System* + InArmSuck)
// ============================================================================
static void test_clean_pad_records()
{
    InArmSuck.Suck[0][0].sName = "InArm_R0C0";

    TMyProductionRecord rec;
    rec.AddPickCleanPad(0, 0, 1, 2, 5);
    CHECK_STR(rec.asBuffer->Strings[ePickCleanPadData], "5");
    CHECK_STR(rec.asBuffer->Strings[ePickCleanPadSuck], "InArm_R0C0");
    CHECK_STR(rec.asBuffer->Strings[ePickCleanPadRow], "1");
    CHECK_STR(rec.asBuffer->Strings[ePickCleanPadCol], "2");
    CHECK(rec.asBuffer->GetString(ePickCleanPadTime).Length() > 0);   // StringsProxy has no .Length(); GetString() returns a plain AnsiString

    TMyProductionRecord rec2;
    rec2.AddPlaceCleanPadToShuttle(0, 0, 3, 4);
    CHECK_STR(rec2.asBuffer->Strings[ePlaceCleanPadToShtSuck], "InArm_R0C0");
    CHECK_STR(rec2.asBuffer->Strings[ePlaceCleanPadToShtRow], "3");
    CHECK_STR(rec2.asBuffer->Strings[ePlaceCleanPadToShtCol], "4");

    TMyProductionRecord rec3;
    rec3.AddPickCleanPadFormShuttle(0, 0, 5, 6);
    CHECK_STR(rec3.asBuffer->Strings[ePickCleanPadFormShtRow], "5");
    CHECK_STR(rec3.asBuffer->Strings[ePickCleanPadFormShtCol], "6");

    TMyProductionRecord rec4;
    rec4.AddPlaceCleanPad(0, 0, 7, 8);
    CHECK_STR(rec4.asBuffer->Strings[ePlaceCleanPadRow], "7");
    CHECK_STR(rec4.asBuffer->Strings[ePlaceCleanPadCol], "8");
}

// ============================================================================
int main()
{
    test_ctor_and_lifecycle();
    test_pure_leaves();
    test_error_log_and_test_result();
    test_unload_record();
    test_ground_esd();
    test_get_family();
    test_hp_record();
    test_suck_pick_records();
    test_clean_pad_records();

    fprintf(stderr, "\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
