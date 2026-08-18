// =============================================================================
//  test_security_core.cpp -- FW-SecCC: TfSecurity test coverage
//
//  AI(W906-FW-SecCC) 20260819: new file.
//
//  Covers Insufficient/GetBit8 (the two "5+1 known consumer" oracles, see
//  forms/fSecurity.h CONSUMER REGISTER) and the customer-code-conditional
//  pure logic inside GetJamLevel/GetJemRed, plus ClearAllJamCount/
//  AddJamCount (self-contained on the form's own sgStatisticsJam widget).
//
//  WHY CONSTRUCTING TfSecurity FRESH HERE IS FILE-SAFE (no real disk I/O):
//  every test below constructs a plain `TfSecurity f;`. Its ctor's ONLY
//  disk-touching statements (GetLevelSet/the Jam-populate loop/AddAlarmList)
//  are wrapped in `if (INIFileGeneral != 0)` (see forms/fSecurity.h CTOR
//  SAFETY) -- INIFileGeneral is a nullptr-by-default global that only
//  OpenGeneralIniFile() (never called by this test binary) ever sets, so
//  that whole guarded block is skipped and `FileNameJam000` stays "" (its
//  AnsiString default). Every Get*/GetJamLevel/GetJemRed method this file
//  calls routes its file access through CheckAndReadIniData(FileNameJam000,
//  ...) -- and common.cpp:434's own `OpenIniFile`: `if (FileName == "")
//  return false;` (read this wave to confirm) makes every such call a
//  guaranteed, side-effect-free "return the caller's default" -- verified
//  behaviour, not an assumption. This is what lets this file test the
//  customer-code BRANCHING logic inside GetJamLevel/GetJemRed (which runs
//  BEFORE/AROUND the ini read) without ever touching a real path.
//
//  DELIBERATELY NOT COVERED:
//    * TMySecurity -- entirely unreachable this wave (GATE SEC1: nothing
//      constructs one; GATE SEC10: even if constructed by hand, Panel/
//      RadioGroup/SpeedButton are left null by the gated ctor body, so
//      calling the otherwise-FULL SetVisible/GetVisivle/SetLevel/GetLevel/
//      SetEnabled/GetCaption on such an instance would dereference a null
//      pointer). Testing it would require inventing allocation the real
//      ctor does not perform -- out of scope for a translation test.
//    * ChangeJamMessage/SaveJamLevel/spbImportClick/spbExportClick/
//      ChangePassword -- all either GATE-bodied (ChangePassword, SEC7) or
//      touch widgets (cbJamArea/cbJamCode/RichEditJamCode) whose Items/Lines
//      would need hand-populating to exercise meaningfully; the PURE logic
//      they delegate to (GetJamLevel/GetJemRed/GetBit8/...) is covered
//      directly below instead.
// =============================================================================
#include "forms/fSecurity.h"

#include "MachineType.h"      // CC_* customer codes
#include "cmydef.h"            // CUSTOMER_CODE, AccessLevel, iDefHonPrecLevel
#include "cprod.h"              // LevelSet
#include "CosFunction.h"         // CosFunction

#include <cstdio>

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
    CUSTOMER_CODE = 0;
    AccessLevel = 0;
    for (int i = 0; i < 256; i++)
        LevelSet.AccessLevel[i] = 0;
    CosFunction.bStatisticsJamCount = false;
}

// =============================================================================
//  (1) Insufficient -- golden :573-597
// =============================================================================
static void Test_Insufficient_TypeAboveMaxAlwaysFailsClosed()
{
    ResetGlobals();
    TfSecurity f;

    AccessLevel = 4;   // highest level -- would pass any real per-item check
    // iMaxLevelItem (file-scope, cSecurity.cpp) stays 0 while GATE (SEC1) is
    // closed (nothing in this process ever runs the mySecurityPal population
    // loop) -- so ANY iType>0 hits golden's own `if(iType>iMaxLevelItem)
    // return false;` guard verbatim. This is the SAME fail-closed answer the
    // known 5 external call sites (177/107/43/97/108) already assume -- see
    // forms/fSecurity.h CONSUMER REGISTER.
    CHECK(f.Insufficient(1, false) == false, "Insufficient(1): fails closed while mySecurityPal is empty");
    CHECK(f.Insufficient(177, false) == false, "Insufficient(177): matches atester.cpp's known call site");
    CHECK(f.Insufficient(107, false) == false, "Insufficient(107): matches cContactCT.cpp GATE (C3)'s known call site");
}

static void Test_Insufficient_NegativeTypeAlwaysFalse()
{
    ResetGlobals();
    TfSecurity f;
    AccessLevel = 4;
    CHECK(f.Insufficient(-5, false) == false, "Insufficient(iType<-1): always false (golden :584-585)");
}

static void Test_Insufficient_HontechBranchIsLiveRegardlessOfGateSEC1()
{
    ResetGlobals();
    TfSecurity f;

    AccessLevel = iDefHonPrecLevel - 1;
    CHECK(f.Insufficient(-1, false) == false, "Insufficient(-1): AccessLevel below HonPrec -> false");

    AccessLevel = iDefHonPrecLevel;
    CHECK(f.Insufficient(-1, false) == true, "Insufficient(-1): AccessLevel==HonPrec -> true (iType==-1 branch never touches iMaxLevelItem)");
}

// =============================================================================
//  (2) GetBit8 -- golden :1299-1305 (Command.cpp:14959's known call site)
// =============================================================================
static void Test_GetBit8_ReturnsDefaultWithEmptyFileName()
{
    ResetGlobals();
    TfSecurity f;
    // FileNameJam000 is "" (ctor's disk-touching guard closed, see file
    // banner) -- CheckAndReadIniData("", ..., 0) is guaranteed side-effect-
    // free and returns the literal 0 default.
    CHECK(f.GetBit8("01 Input Arm", "JAM0203") == 0, "GetBit8: deterministic 0 default with FileNameJam000==\"\"");
}

// =============================================================================
//  (3) GetJamLevel -- golden :1204-1297, customer-branch pure logic
// =============================================================================
static void Test_GetJamLevel_GigasJam0201AlwaysZero()
{
    ResetGlobals();
    TfSecurity f;
    CUSTOMER_CODE = CC_GIGAS;
    LevelSet.AccessLevel[35] = 3;   // even with a high override level set...
    // sJamArea/sJamCode non-empty is required to enter the branch at all
    // (golden :1208); the ini read itself is the side-effect-free 0 default.
    CHECK(f.GetJamLevel("01 Input Arm", "JAM0201") == 0, "GetJamLevel: CC_GIGAS JAM0201 forced to 0 (golden :1213-1216)");
}

static void Test_GetJamLevel_AmkorChinaBumpsToLevel35()
{
    ResetGlobals();
    TfSecurity f;
    CUSTOMER_CODE = CC_AMKOR_China;
    LevelSet.AccessLevel[35] = 3;
    CHECK(f.GetJamLevel("02 Output Arm", "WAR0701") == 3, "GetJamLevel: CC_AMKOR_China WAR0701 bumped to AccessLevel[35]");
    CHECK(f.GetJamLevel("02 Output Arm", "JAM9999") == 0, "GetJamLevel: CC_AMKOR_China unlisted code stays at ini default 0");
}

static void Test_GetJamLevel_DefaultCustomerBranchBumpsKnownCodes()
{
    ResetGlobals();
    TfSecurity f;
    CUSTOMER_CODE = 0;   // no branch above matches -> falls through to the `else` (golden :1279-1293)
    LevelSet.AccessLevel[35] = 2;
    CHECK(f.GetJamLevel("03 Index Unit", "JAM0203") == 2, "GetJamLevel: default branch bumps JAM0203 to AccessLevel[35]");
    CHECK(f.GetJamLevel("03 Index Unit", "WAR9999") == 0, "GetJamLevel: default branch leaves unlisted code at ini default 0");
}

// =============================================================================
//  (4) GetJemRed -- golden :1418-1464, customer-branch pure logic
// =============================================================================
static void Test_GetJemRed_AmkorChinaForcesRedAndDisablesCheckbox()
{
    ResetGlobals();
    TfSecurity f;
    CUSTOMER_CODE = CC_AMKOR_China;
    CHECK(f.GetJemRed("02 Output Arm", "WAR0701") == true, "GetJemRed: CC_AMKOR_China WAR0701 forced red");
    CHECK(f.cbJamNeedRed->Enabled == false, "GetJemRed: checkbox disabled when forced red");
}

static void Test_GetJemRed_DefaultCustomerLeavesIniDefault()
{
    ResetGlobals();
    TfSecurity f;
    CUSTOMER_CODE = 0;
    CHECK(f.GetJemRed("01 Input Arm", "JAM0203") == false, "GetJemRed: default branch, no forced-red list, stays at ini default false");
    CHECK(f.cbJamNeedRed->Enabled == true, "GetJemRed: checkbox stays enabled when not forced");
}

// =============================================================================
//  (5) GATE (SEC4) GetJamArea -- forms/fSecurity.h banner
// =============================================================================
static void Test_GetJamArea_GatedDefaultIsEmptyString()
{
    ResetGlobals();
    TfSecurity f;
    CHECK(f.GetJamArea("01 Input Arm") == "", "GetJamArea: GATE (SEC4) default is empty AnsiString (fMain->AlarmUnitMap absent)");
}

// =============================================================================
//  (6) ClearAllJamCount / AddJamCount -- golden :1678-1717
// =============================================================================
static void Test_AddJamCount_TriggersAtFiveAndClearAllResets()
{
    ResetGlobals();
    TfSecurity f;
    CosFunction.bStatisticsJamCount = true;

    f.sgStatisticsJam->RowCount = 3;
    f.sgStatisticsJam->Cells[2][1] = "WAR0700";
    f.sgStatisticsJam->Cells[7][1] = "0";
    f.sgStatisticsJam->Cells[2][2] = "JAM0100";
    f.sgStatisticsJam->Cells[7][2] = "0";

    bool bTriggered = false;
    for (int i = 0; i < 5; i++)
        bTriggered = f.AddJamCount("WAR0700");

    CHECK(bTriggered == true, "AddJamCount: 5th call for the same code returns true");
    CHECK(f.sgStatisticsJam->Cells[7][1] == "0", "AddJamCount: matching row's counter reset to 0 on trigger");
    CHECK(f.sgStatisticsJam->Cells[7][2] == "0", "AddJamCount: unrelated row (JAM0100) untouched");

    f.sgStatisticsJam->Cells[7][2] = "3";   // seed a nonzero value on the OTHER row
    f.ClearAllJamCount();
    CHECK(f.sgStatisticsJam->Cells[7][1] == "0", "ClearAllJamCount: row 1 cleared");
    CHECK(f.sgStatisticsJam->Cells[7][2] == "0", "ClearAllJamCount: row 2 cleared");
}

static void Test_AddJamCount_NoOpWhenStatisticsDisabled()
{
    ResetGlobals();
    TfSecurity f;
    CosFunction.bStatisticsJamCount = false;   // golden :1693-1696 early-return

    f.sgStatisticsJam->RowCount = 2;
    f.sgStatisticsJam->Cells[2][1] = "WAR0700";
    f.sgStatisticsJam->Cells[7][1] = "4";

    CHECK(f.AddJamCount("WAR0700") == false, "AddJamCount: disabled statistics always returns false");
    CHECK(f.sgStatisticsJam->Cells[7][1] == "4", "AddJamCount: disabled statistics never touches the grid");
}

int main()
{
    Test_Insufficient_TypeAboveMaxAlwaysFailsClosed();
    Test_Insufficient_NegativeTypeAlwaysFalse();
    Test_Insufficient_HontechBranchIsLiveRegardlessOfGateSEC1();
    Test_GetBit8_ReturnsDefaultWithEmptyFileName();
    Test_GetJamLevel_GigasJam0201AlwaysZero();
    Test_GetJamLevel_AmkorChinaBumpsToLevel35();
    Test_GetJamLevel_DefaultCustomerBranchBumpsKnownCodes();
    Test_GetJemRed_AmkorChinaForcesRedAndDisablesCheckbox();
    Test_GetJemRed_DefaultCustomerLeavesIniDefault();
    Test_GetJamArea_GatedDefaultIsEmptyString();
    Test_AddJamCount_TriggersAtFiveAndClearAllResets();
    Test_AddJamCount_NoOpWhenStatisticsDisabled();

    std::printf("%d/%d checks passed (test_security_core)\n", g_pass, g_pass + g_fail);
    return g_fail == 0 ? 0 : 1;
}
