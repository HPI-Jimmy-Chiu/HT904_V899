// tests/test_SCK_ART_Remainder.cpp
// Verification harness for the Automation/SCK_ART_Remainder.h/.cpp REMAINDER slice
// (W5-Final-SckArtRemainder), continuing the sibling test_SCK_ART.cpp (8-function extract).
//
// Exercises the translated public API against input->expected-output values hand-derived from the
// ORIGINAL golden reference
//   HT9011UC_Code_V3.33.906.0_20260618/Automation/SCK_ART.cpp:183-1645, :4191-4256 (see
//   SCK_ART_Remainder.h for the exact per-function golden line ranges).
//
// AI(W906-DoARTLotStart) 20260721: added PART 9 (DoARTLotStart, golden :4191-4256).
//
// LIMITATION 1 (same as test_SCK_ART.cpp/test_ContactForce.cpp): we CANNOT run the original BCB6
// binary (no Borland compiler in this environment). Verification here is therefore: (1) the
// translation compiles+links clean against the real shared globals, AND (2) its outputs equal values
// hand-computed/transcribed from the BCB6 source (golden file:line cited per case).
//
// LIMITATION 2 (DO-NOT-TOUCH-REAL-FILES discipline, same spirit as test_ini_helpers.cpp's "DO-NOT-
// MODIFY-REAL-INI DISCIPLINE"): golden TfSCKART::AccessFile(bool bRead,...) calls WriteLastDataFile()
// when bRead==false (golden SCK_ART.cpp:436) and CustomerFunctionSelect() unconditionally (golden
// :437). An EARLY DRAFT of this test called the real AccessFile(bRead=false,...) directly and only
// discovered via an actual link (not just -fsyntax-only) that BOTH of those, plus RUN_INFO::AddAlarm,
// are TODO(W6)-gated dead text inside cprod.cpp (see SCK_ART_Remainder.cpp gate #8) -- so calling them
// is, for now, a safe no-op (gate #8), NOT the real WriteLastDataFile (which would otherwise write the
// ACTUAL production Handler's live D:\HT9045\system\lastdata*.dat files at hardcoded, non-redirectable
// paths -- this repo also contains the live D:\HT9045 production tree alongside this migration
// sandbox). PART 3b below DOES exercise AccessFile's bRead==false (save) direction, relying on gate #8
// making WriteLastDataFile/CustomerFunctionSelect harmless; when gate #8 is eventually retired (W6),
// re-verify this harness still cannot reach a real hardcoded system path before removing this note.
// SckArtRem_SetGPIBVersion is NEVER called here: golden hardcodes a DIFFERENT, still very-much-real,
// non-redirectable external path -- D:\GPIB9045\system\general.ini (a separate live project directory
// on this dev machine, read via the REAL, active ReadWriteIni/WriteIniData in common.cpp) -- with no
// equivalent gate protecting it.
//
// LIMITATION 3 (hidden TU-local gate statics, same as test_SCK_ART.cpp's CheckOutArmNeedVariModeFIX
// PART): several LastSet.* fields this file's gate #4 stands in for (lSCKARTBinCT[]/iHdPass/iHdFail/
// iHdRejectATPass/iHdRejectATFail/iHdRejectBT/iTesterMatch/iTesterUnmatch/iTestIgnore/iHdPickUp/
// iSCKART_RTUnitCount/bBreakSCKART) are `static` (internal linkage) inside SCK_ART_Remainder.cpp --
// this test binary cannot seed or observe them directly. Assertions below only cover the OBSERVABLE
// (SckArtRemainderState / real canary_support.h LastSet / real cprod.h globals) side effects, and
// choose input values that make the always-0/false stand-in defaults harmless to the assertion (same
// technique the sibling test already established).
//
// No external test framework: a tiny check harness prints PASS/FAIL per case and a final summary,
// and returns non-zero on ANY failure.

#include "Automation/SCK_ART_Remainder.h"

#include "MachineDefine.h"
#include "MachineType.h"
#include "cprod.h"
#include "cmydef.h"
#include "canary_support.h"
#include "aHotPlateSubstrate.h"
#include "FormsFacade.h"
#include "common.h"     // ReadWriteIni/WriteIniData (direct, safe scratch-path seeding) + DataPath
#include "atester_shims.h"   // AI(W906-SaveTestSummarySECS) 20260721: fObserver->memoLotSummary (PART 10)

#include <cstdio>
#include <cstdlib>
#include <cmath>

static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { std::printf("PASS  %s\n", msg); ++g_pass; }                 \
        else      { std::printf("FAIL  %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

static bool feq(double a, double b, double eps = 1e-6)
{
    return std::fabs(a - b) <= eps * (1.0 + std::fabs(b));
}

// Scratch-dir helper -- mirrors tests/test_IniFiles.cpp's tmpIniPath() convention (prefer %TEMP%/%TMP%,
// fall back to the current directory, which always exists when ctest runs).
static AnsiString ScratchDir()
{
    const char* t = std::getenv("TEMP");
    if (!t || !*t) t = std::getenv("TMP");
    if (!t || !*t) t = ".";
    AnsiString d(t);
    if (d.Length()==0 || d[d.Length()]!='\\')
        d = d + "\\";
    return d;
}

int main()
{
    printf("=== Automation/SCK_ART_Remainder.cpp translation verification ===\n");
    printf("(values hand-derived/transcribed from golden Automation/SCK_ART.cpp:183-1645; original binary NOT run)\n\n");

    // =========================================================================================
    // PART 1 -- SetSetupFilePath -- golden :183-188
    // =========================================================================================
    printf("-- SetSetupFilePath --\n");
    {
        AnsiString savedDataPath = DataPath;
        DataPath = "C:\\W5FinalSckArtRemainderScratch\\";   // gate #1 stand-in reads DataPath directly

        SckArtRemainderState st;
        st.sSetupFilePath = "stale";
        SckArtRem_SetSetupFilePath(st);
        CHECK(st.sSetupFilePath == DataPath + AnsiString("Tester.Data"),
              "SetSetupFilePath -> DataPath+\"Tester.Data\" (golden :185-187, gate #1 stand-in skips GetLastOpenFN() subfolder)");

        DataPath = savedDataPath;
    }

    // =========================================================================================
    // PART 2 -- AccessFile early-return -- golden :195-196 (CosFunction.bUseSCKART==false)
    // =========================================================================================
    printf("\n-- AccessFile (early return) --\n");
    {
        CosFunction.bUseSCKART = false;

        SckArtRemainderState st;
        st.sSetupFilePath = "SENTINEL-UNCHANGED";
        st.iInputCount = 777;

        SckArtRem_AccessFile(st, true, -1);
        CHECK(st.sSetupFilePath == "SENTINEL-UNCHANGED",
              "bUseSCKART==false -> early return BEFORE SetSetupFilePath runs (golden :195-198)");
        CHECK(st.iInputCount == 777,
              "bUseSCKART==false -> early return -> no ReadWriteIni calls at all (golden :195-196)");

        CosFunction.bUseSCKART = true;   // restore for later PARTs
    }

    // =========================================================================================
    // PART 3 -- AccessFile bRead==true (load) -- golden :190-444, minus the widget-refresh block
    //   (golden :335-403) -- see file-head LIMITATION 2 for why bRead==false is never exercised here.
    // =========================================================================================
    printf("\n-- AccessFile (bRead=true, load direction only -- see LIMITATION 2) --\n");
    {
        AnsiString scratchDir = ScratchDir();
        AnsiString scratchPath = scratchDir + "Tester.Data";   // must match gate #1's DataPath+"Tester.Data"

        // Seed the persisted values directly via the REAL ReadWriteIni(...,bRead=false,...) plumbing,
        // pointed at OUR OWN throwaway scratch path (never SckArtRem_AccessFile's bRead=false branch --
        // this bypasses WriteLastDataFile entirely, see LIMITATION 2).
        ReadWriteIni(scratchPath, "AutoRetest", "LOTSTATUS",       AnsiString("LOTSTATUS_T"), AnsiString("NONE"), false);
        ReadWriteIni(scratchPath, "AutoRetest", "iCurrentStatus",  2,                          1,                  false);
        ReadWriteIni(scratchPath, "AutoRetest", "iTesterType",     1,                          0,                  false);
        ReadWriteIni(scratchPath, "AutoRetest", "sLotID",          AnsiString("LOT-999"),      AnsiString(""),     false);
        ReadWriteIni(scratchPath, "AutoRetest", "sProceddCode",    AnsiString("PROC-7"),       AnsiString(""),     false);
        ReadWriteIni(scratchPath, "AutoRetest", "iLotCount",       5000,                       0,                  false);
        ReadWriteIni(scratchPath, "AutoRetest", "iInputCount",     4200,                       0,                  false);
        ReadWriteIni(scratchPath, "AutoRetest", "iFTRTCount",      3,                          0,                  false);
        ReadWriteIni(scratchPath, "AutoRetest", "iInputJamCnt",    7,                          0,                  false);
        ReadWriteIni(scratchPath, "AutoRetest", "iOutputJamCnt",   9,                          0,                  false);
        ReadWriteIni(scratchPath, "AutoRetest", "iManualRejectCnt",11,                         0,                  false);
        ReadWriteIni(scratchPath, "AutoRetest", "sInfoCustomer",   AnsiString("ACME"),         AnsiString(""),     false);
        ReadWriteIni(scratchPath, "AutoRetest", "sBundleList",     AnsiString("B1,B2"),        AnsiString(""),     false);
        ReadWriteIni(scratchPath, "AutoRetest", "iBundleInCnt",    24,                         12,                 false);
        ReadWriteIni(scratchPath, "AutoRetest", "sLotStartTime",   AnsiString("2026-07-11"),   AnsiString(""),     false);
        ReadWriteIni(scratchPath, "AutoRetest", "iManualStart",    0,                          1,                  false);
        ReadWriteIni(scratchPath, "AutoRetest", "bFirstFullSkip",  true,                       false,              false);
        // Deliberately opposite of the DEFAULT we set below (IniConfig.bA10_AutoReTest=true), proving
        // the persisted value wins over the ReadWriteIni default (golden :293).
        ReadWriteIni(scratchPath, "AutoRetest", "Enable ART",      false,                      true,               false);
        ReadWriteIni(scratchPath, "AutoRetest", "Pass Rate",       62.5,                       100.0,              false, true, 0.01, 100.0);

        // Globals AccessFile's non-VCL logic reads (golden :192-437).
        CosFunction.bUseSCKART = true;
        USE_AUTO_RETEST = eartInstall;
        IniConfig.bA10_AutoReTest = true;
        IniConfig.bA10TestModeForART = false;    // skip the iGpibMode side-branch (golden :404-413), not under test here
        CUSTOMER_CODE = 0;                        // not CC_AMKOR_Japan/RF360/QUALCOMM -> the plain else branches (golden :291-296, :317-323)
        bCanRunSCKART = false;                    // pre-dirty; AccessFile recomputes it (golden :415-425)

        AnsiString savedDataPath = DataPath;
        DataPath = scratchDir;

        SckArtRemainderState st;
        SckArtRem_AccessFile(st, /*bRead=*/true, -1);

        DataPath = savedDataPath;

        CHECK(st.sLOTSTATUS == "LOTSTATUS_T",           "AccessFile(true) reads back sLOTSTATUS (golden :202)");
        CHECK(st.iCurrentStatus == 2,                    "AccessFile(true) reads back iCurrentStatus (golden :203)");
        CHECK(st.iTesterType == 1,                       "AccessFile(true) reads back iTesterType (golden :204)");
        CHECK(CosFunction.bAutoRetestGPIBmode == true,   "iTesterType==1 -> CosFunction.bAutoRetestGPIBmode=true (golden :205-206)");
        CHECK(st.sLotID == "LOT-999",                    "AccessFile(true) reads back sLotID (golden :213)");
        CHECK(st.sProcessCode == "PROC-7",                "AccessFile(true) reads back sProcessCode (golden :214, key literally \"sProceddCode\" -- golden's own typo, verbatim)");
        CHECK(st.iLotCount == 5000,                       "AccessFile(true) reads back iLotCount (golden :215)");
        CHECK(st.iInputCount == 4200,                      "AccessFile(true) reads back iInputCount (golden :216)");
        CHECK(st.iFTRTCount == 3,                          "AccessFile(true) reads back iFTRTCount (golden :217)");
        CHECK(st.iInputJamCnt == 7,                        "AccessFile(true) reads back iInputJamCnt (golden :218)");
        CHECK(st.iOutputJamCnt == 9,                       "AccessFile(true) reads back iOutputJamCnt (golden :219)");
        CHECK(st.iManualRejectCnt == 11,                   "AccessFile(true) reads back iManualRejectCnt (golden :220)");
        CHECK(st.sInfo_Customer == "ACME",                 "AccessFile(true) reads back sInfo_Customer (golden :222)");
        CHECK(st.sBundleList == "B1,B2",                   "AccessFile(true) reads back sBundleList (golden :240)");
        CHECK(st.iBundleInCnt == 24,                        "AccessFile(true) reads back iBundleInCnt (golden :241)");
        CHECK(st.sLotStartTime == "2026-07-11",             "AccessFile(true) reads back sLotStartTime (golden :265)");
        CHECK(st.iManualStart == 0,                          "AccessFile(true) reads back iManualStart (golden :266)");
        CHECK(st.bFirstFullSkip == true,                     "AccessFile(true) reads back bFirstFullSkip (golden :267)");
        CHECK(TestIF_File.bSCKART_EnableART == false,        "persisted \"Enable ART\"=false wins over default=true (golden :293)");
        CHECK(bCanRunSCKART == false,                        "bSCKART_EnableART==false -> bCanRunSCKART recomputed false (golden :415-425)");
        CHECK(feq(TestIF_File.dSCKART_Yield, 62.5),          "AccessFile(true) reads back dSCKART_Yield (golden :301)");
        CHECK(st.sSetupFilePath == scratchPath,              "SetSetupFilePath ran first inside AccessFile (golden :198) -> sSetupFilePath==scratch Tester.Data path");
    }

    // =========================================================================================
    // PART 3b -- AccessFile bRead==false (save) round-trip -- golden :190-333. Safe ONLY because
    //   gate #8 makes WriteLastDataFile()/CustomerFunctionSelect() no-ops right now (see LIMITATION 2).
    // =========================================================================================
    printf("\n-- AccessFile (bRead=false save, then bRead=true reload) --\n");
    {
        AnsiString scratchDir = ScratchDir();
        AnsiString scratchPath = scratchDir + "Tester.Data";

        CosFunction.bUseSCKART = true;
        USE_AUTO_RETEST = eartInstall;
        IniConfig.bA10_AutoReTest = true;
        IniConfig.bA10TestModeForART = false;
        CUSTOMER_CODE = 0;
        TestIF_File.bSCKART_EnableART = true;

        AnsiString savedDataPath = DataPath;
        DataPath = scratchDir;

        SckArtRemainderState stSave;
        stSave.sLOTSTATUS="LOTSTATUS_F"; stSave.iCurrentStatus=5; stSave.iTesterType=0;
        stSave.sLotID="SAVE-LOT"; stSave.sProcessCode="SAVE-PROC"; stSave.iLotCount=321;
        stSave.iInputCount=222; stSave.iFTRTCount=4; stSave.iInputJamCnt=1; stSave.iOutputJamCnt=2;
        stSave.iManualRejectCnt=3; stSave.sBundleList="X1"; stSave.iBundleInCnt=6; stSave.iBundleOutCnt=6;
        stSave.sLotStartTime="2026-01-01"; stSave.iManualStart=1; stSave.bFirstFullSkip=false;

        SckArtRem_AccessFile(stSave, /*bRead=*/false, -1);

        SckArtRemainderState stLoad;
        SckArtRem_AccessFile(stLoad, /*bRead=*/true, -1);

        DataPath = savedDataPath;

        CHECK(stLoad.sLOTSTATUS == "LOTSTATUS_F",  "save(false)->load(true) round-trip: sLOTSTATUS (golden :202)");
        CHECK(stLoad.iCurrentStatus == 5,           "save->load round-trip: iCurrentStatus (golden :203)");
        CHECK(stLoad.sLotID == "SAVE-LOT",          "save->load round-trip: sLotID (golden :213)");
        CHECK(stLoad.sProcessCode == "SAVE-PROC",    "save->load round-trip: sProcessCode (golden :214)");
        CHECK(stLoad.iLotCount == 321,                "save->load round-trip: iLotCount (golden :215)");
        CHECK(stLoad.iInputCount == 222,              "save->load round-trip: iInputCount (golden :216)");
        CHECK(stLoad.iFTRTCount == 4,                  "save->load round-trip: iFTRTCount (golden :217)");
        CHECK(stLoad.sBundleList == "X1",              "save->load round-trip: sBundleList (golden :240)");
        CHECK(stLoad.sLotStartTime == "2026-01-01",    "save->load round-trip: sLotStartTime (golden :265)");
    }

    // =========================================================================================
    // PART 4 -- UpdateCount -- golden :477-591 (yield formula + LastSet.iHd* branch)
    //   NOTE: LastSet.lSCKARTBinCT[]/iHdPass/iHdFail/... are TU-local zero-init stand-ins (gate #4) --
    //   iTotalUnloadCount/iUnloadCnt are ALWAYS 0 offline (documented, matches the sibling test's
    //   CheckOutArmNeedVariModeFIX precedent). Only st.dCurrYield is observable from outside.
    // =========================================================================================
    printf("\n-- UpdateCount --\n");
    {
        LastSet.iTester = OFF_LINE;
        BinSelect[OffT].bAutoRetest[0] = true;
        BinSelect[OffT].bAutoRetest[1] = true;
        BinSelect[OffT].bAutoRetest[2] = true;
        TestIF_File.bRENESAS_EnableFTCT = false;
        TestIF_File.bSCKART_LotDeviceCheck = false;

        SckArtRemainderState st;
        st.iInputCount = 100;
        LastSet.iSCKARTInputCT = 100;
        // iUnloadCnt is always 0 (gate #4 stand-in) -> "100%!=0, iUnloadCnt==0" branch (golden :536-541).
        SckArtRem_UpdateCount(st);
        CHECK(feq(st.dCurrYield, 100.0), "iInputCount!=0 && iSCKARTInputCT!=0 && iUnloadCnt==0(gated) -> 100% (golden :536-541)");

        LastSet.iSCKARTInputCT = 0;
        SckArtRem_UpdateCount(st);
        CHECK(feq(st.dCurrYield, 0.0), "iSCKARTInputCT==0 -> 0% (golden :550-553)");

        LastSet.iSCKARTInputCT = 40;
        SckArtRem_UpdateCount(st);
        CHECK(feq(st.dCurrYield, 100.0), "iSCKARTInputCT(40)-iUnloadCnt(0,gated)==40, /40*100 -> 100% (golden :556, degenerates because iUnloadCnt is gated to 0)");

        // bSCKART_LotDeviceCheck==true branch -> the THIRD (else) sub-branch uses st.iLotCount, but
        // golden's FIRST sub-condition in THIS branch (:562-567) still reads LastSet.iSCKARTInputCT!=0
        // (a verbatim golden asymmetry -- both the false-branch and true-branch first conditions test
        // the SAME LastSet.iSCKARTInputCT, only the fallback-else differs between LastSet.iSCKARTInputCT
        // and st.iLotCount). Must clear it here too, or the still-40-from-above value short-circuits
        // the first condition to 100% before ever reaching the iLotCount==0 check below.
        LastSet.iSCKARTInputCT = 0;
        TestIF_File.bSCKART_LotDeviceCheck = true;
        st.iLotCount = 0;
        SckArtRem_UpdateCount(st);
        CHECK(feq(st.dCurrYield, 0.0), "bSCKART_LotDeviceCheck==true && iLotCount==0 -> 0% (golden :576-579)");

        st.iLotCount = 80;
        SckArtRem_UpdateCount(st);
        CHECK(feq(st.dCurrYield, 100.0), "bSCKART_LotDeviceCheck==true, iLotCount(80)-iUnloadCnt(0,gated) -> 100% (golden :582, degenerate for the same gated reason)");

        // bRENESAS_EnableFTCT==true branch just must not crash (LastSet.iHd* targets are hidden statics).
        TestIF_File.bRENESAS_EnableFTCT = true;
        SckArtRem_UpdateCount(st);
        CHECK(true, "bRENESAS_EnableFTCT==true branch (golden :525-532) runs without crashing (LastSet.iHd* targets are hidden gate #4 statics, unobservable here)");
        TestIF_File.bRENESAS_EnableFTCT = false;
    }

    // =========================================================================================
    // PART 5 -- AddAlarmCode -- golden :593-624
    //   NOTE: RunInfo.AddAlarm's method body is TODO(W6)-gated (gate #8, discovered via this wave's
    //   own real-link smoke test -- see file-head LIMITATION 2) -- it is a no-op right now, so its
    //   effect is NOT observable from this test. These calls only verify: (1) the iDuplicate/"JAM"
    //   substring dispatch condition compiles+runs without crashing across all 3 branches, and
    //   (2) will start actually asserting real RunInfo.vByLotJam state the moment gate #8 is retired
    //   (W6) -- at which point this NOTE (and the CHECK(true) placeholders) should be revisited.
    // =========================================================================================
    printf("\n-- AddAlarmCode --\n");
    {
        SckArtRem_AddAlarmCode("JAM0001", "Test jam message", /*iDuplicate=*/0);
        CHECK(true, "iDuplicate==0 && Code contains \"JAM\" branch runs without crashing (golden :619-623; RunInfo.AddAlarm itself is gate #8, unobservable)");

        SckArtRem_AddAlarmCode("NOTJAM_CODE", "Should not be added", 0);
        CHECK(true, "Code without \"JAM\" substring -> the RunInfo.AddAlarm call is skipped (golden :620, AnsiPos(\"JAM\")==0)");

        SckArtRem_AddAlarmCode("JAM0002", "Duplicate-flagged", /*iDuplicate=*/1);
        CHECK(true, "iDuplicate!=0 -> the RunInfo.AddAlarm call is skipped even for a JAM code (golden :619)");
    }

    // =========================================================================================
    // PART 6 -- ClearLotInfo -- golden :837-923
    // =========================================================================================
    printf("\n-- ClearLotInfo --\n");
    {
        TestIF_File.bRENESAS_EnableFTCT = false;

        SckArtRemainderState st;
        st.iLotCount = 999; st.iInputCount = 888; st.sLotID = "STALE-LOT";
        st.sProcessCode = "STALE-PROC"; st.sLotStartTime = "STALE-TIME";
        st.sLOTSTATUS = "LOTSTATUS_R"; st.iCurrentStatus = 4;
        st.iFTRTCount = 5; st.iManualRejectCnt = 6; st.iInputJamCnt = 7; st.iOutputJamCnt = 8;
        st.sInfo_Customer = "STALE-CUST"; st.sBundleList = "STALE-BL"; st.iInfo_MultiLotCnt = 3;
        st.sInfoArr_Customer[2] = "STALE-ARR";
        LastSet.iSCKARTInputCT = 321; LastSet.lShuttleCount = 654;
        bWaitTSV = true;

        bool needAccessFileWrite = false;
        SckArtRem_ClearLotInfo(st, &needAccessFileWrite);

        CHECK(st.iNeedRT == 1,              "ClearLotInfo -> iNeedRT=1 (golden :839)");
        CHECK(st.iFTRTCount == 0,            "ClearLotInfo -> iFTRTCount=0 (golden :840)");
        CHECK(st.iManualRejectCnt == 0,       "ClearLotInfo -> iManualRejectCnt=0 (golden :841)");
        CHECK(st.iLotCount == 0,              "bRENESAS_EnableFTCT==false -> iLotCount cleared (golden :848)");
        CHECK(st.iInputCount == 0,            "bRENESAS_EnableFTCT==false -> iInputCount cleared (golden :849)");
        CHECK(st.sLotID == "",                "bRENESAS_EnableFTCT==false -> sLotID cleared (golden :850)");
        CHECK(st.sProcessCode == "",          "bRENESAS_EnableFTCT==false -> sProcessCode cleared (golden :851)");
        CHECK(st.sLotStartTime == "",         "bRENESAS_EnableFTCT==false -> sLotStartTime cleared (golden :852)");
        CHECK(st.sLOTSTATUS == "NONE",        "SetLotStatus(iLOTSTATUS_NONE) inlined -> sLOTSTATUS=\"NONE\" (golden :853)");
        CHECK(st.iCurrentStatus == 0,          "SetLotStatus(iLOTSTATUS_NONE) inlined -> iCurrentStatus=0 (golden :853)");
        CHECK(st.iInputJamCnt == 0,            "ClearLotInfo -> iInputJamCnt=0 (golden :856)");
        CHECK(st.iOutputJamCnt == 0,           "ClearLotInfo -> iOutputJamCnt=0 (golden :857)");
        CHECK(LastSet.iSCKARTInputCT == 0,     "ClearLotInfo -> LastSet.iSCKARTInputCT=0 (golden :863)");
        CHECK(LastSet.lShuttleCount == 0,      "ClearLotInfo -> LastSet.lShuttleCount=0 (golden :864)");
        CHECK(st.sInfo_Customer == "",         "ClearLotInfo -> sInfo_Customer=\"\" (golden :879)");
        CHECK(st.sBundleList == "",            "ClearLotInfo -> sBundleList=\"\" (golden :898)");
        CHECK(st.iInfo_MultiLotCnt == 0,        "ClearLotInfo -> iInfo_MultiLotCnt=0 (golden :895)");
        CHECK(st.sInfoArr_Customer[2] == "",    "ClearLotInfo -> sInfoArr_Customer[i]=\"\" for i<5 (golden :902)");
        CHECK(needAccessFileWrite == true,      "out-param signals the caller should now call AccessFile(false) (golden :921)");
        CHECK(bWaitTSV == false,                "ClearLotInfo -> bWaitTSV=false (golden :922, real global)");

        // bRENESAS_EnableFTCT==true branch -- the lot-identity fields are intentionally SKIPPED (golden :843-845, empty if-body).
        TestIF_File.bRENESAS_EnableFTCT = true;
        st.iLotCount = 555; st.sLotID = "KEEP-ME";
        SckArtRem_ClearLotInfo(st, 0);
        CHECK(st.iLotCount == 555,  "bRENESAS_EnableFTCT==true -> iLotCount NOT reset (golden :843-845, empty if-body, faithful quirk)");
        CHECK(st.sLotID == "KEEP-ME", "bRENESAS_EnableFTCT==true -> sLotID NOT reset (golden :843-845)");
        TestIF_File.bRENESAS_EnableFTCT = false;
    }

    // =========================================================================================
    // PART 7 -- AddOutputJamCnt -- golden :1361-1389
    //   NOTE: LastSet.iHdRejectATPass/ATFail targets are hidden gate #4 statics (unobservable); only
    //   st.iOutputJamCnt is directly checkable. Exercises all 3 sub-branches without crashing.
    // =========================================================================================
    printf("\n-- AddOutputJamCnt --\n");
    {
        SckArtRemainderState st;
        st.iOutputJamCnt = 0;

        TestIF_File.bRENESAS_EnableFTCT = false;
        SckArtRem_AddOutputJamCnt(st, 0, 0, K_RETRY, 0);
        CHECK(st.iOutputJamCnt == 1, "iOutputJamCnt always increments regardless of the FTCT/K_SKIP gate (golden :1363)");

        TestIF_File.bRENESAS_EnableFTCT = true;
        SckArtRem_AddOutputJamCnt(st, 0, 0, K_SKIP, /*iBinOnCarryKit=*/1);   // "on carry kit, pass" sub-branch (golden :1366-1371)
        CHECK(st.iOutputJamCnt == 2, "increments again (iBinOnCarryKit!=0, pass sub-branch, golden :1368-1371)");

        SckArtRem_AddOutputJamCnt(st, 0, 0, K_SKIP, /*iBinOnCarryKit=*/2);   // "on carry kit, fail" sub-branch (golden :1372-1375)
        CHECK(st.iOutputJamCnt == 3, "increments again (iBinOnCarryKit!=0, fail sub-branch, golden :1372-1375)");

        SckArtRem_AddOutputJamCnt(st, 0, 0, K_SKIP, /*iBinOnCarryKit=*/0);   // OutArmSuck.iBinData[0][0]/Prod.bIsPassBin[] lookup (golden :1377-1387)
        CHECK(st.iOutputJamCnt == 4, "increments again (iBinOnCarryKit==0, OutArmSuck.iBinData/Prod.bIsPassBin lookup sub-branch, golden :1379-1386)");
        TestIF_File.bRENESAS_EnableFTCT = false;
    }

    // =========================================================================================
    // PART 8 -- SaveTestSummary -- golden :1619-1645 (dispatch logic; 3 of 4 callees gated, see
    //   gate #7 -- deferred to next wave). AI(W906-SaveTestSummarySECS) 20260721: the SECS branch now
    //   calls the REAL SckArtRem_SaveTestSummarySECS -- asSummaryPath is redirected to a scratch dir
    //   for the DURATION of this PART (never the real D:\HT9045_Log\Summary production path) so that
    //   branch's real file-write does not touch anything outside the scratch sandbox. Deep coverage of
    //   SaveTestSummarySECS's own behavior is PART 10 below; here we only confirm dispatch doesn't crash.
    // =========================================================================================
    printf("\n-- SaveTestSummary (dispatch logic; SECS branch now real, see PART 10) --\n");
    {
        AnsiString savedSummaryPath8 = asSummaryPath;
        asSummaryPath = ScratchDir() + "W906SaveTestSummaryDispatchScratch";

        SckArtRemainderState st;
        IniConfig.bSPILFunction = false;
        IniConfig.bN17UploadLotSummary = false;
        IniConfig.bA38_SLT_Summary = false;   // keep the ShellExecute-quirk branch OFF here -- PART 10 owns that
        CosFunction.bUseTSVFunction = false;

        CosFunction.bSortingBy2DList = true;
        LastSet.iTester = _2D_SORT;
        TestIF_File.bSortingBy2DIDList = true;
        SckArtRem_SaveTestSummary(st, 1);
        CHECK(true, "2D-sort branch dispatches to (gated) Save2DSortingSummary without crashing (golden :1621-1626)");

        CosFunction.bSortingBy2DList = false;
        CosFunction.bART_SECSGEM_93K = true;
        SckArtRem_SaveTestSummary(st, 1);
        CHECK(true, "SECS/93K branch dispatches to the now-REAL SckArtRem_SaveTestSummarySECS without crashing (golden :1627-1630; see PART 10 for deep coverage)");

        CosFunction.bART_SECSGEM_93K = false;
        TestIF_File.iTestType = TCP_IP_MODE;
        SckArtRem_SaveTestSummary(st, 1);
        CHECK(true, "TCP/IP branch dispatches to (gated) ProcessOSPrint+SaveTestSummaryTSV without crashing (golden :1631-1637)");

        TestIF_File.iTestType = 0;   // not TCP_IP_MODE
        SckArtRem_SaveTestSummary(st, 1);
        CHECK(true, "fallback branch dispatches to (gated) SaveSummaryTrayFeed+SaveTestSummaryTSV without crashing (golden :1638-1644)");

        asSummaryPath = savedSummaryPath8;
    }

    // =========================================================================================
    // PART 9 -- DoARTLotStart -- golden :4191-4256. AI(W906-DoARTLotStart) 20260721.
    //   HasICUnderMachine()==false/true is toggled via FTestSuck.SetItemData(0,0,HAS_IC/NULL_IC,0):
    //   FTestSuck.UseSiteHasIC() -> FrontTestHeadHasIC() -> TestHeadHasIC() -> IndexHasIC() ->
    //   HasICUnderMachine() (csystem_predicates.cpp). Cell [0][0] is always in-bounds/live because
    //   iShtRow/iShtCol default to a 2x1 grid per aHotPlateSubstrate.cpp's TMyKitSuck ctor -- same
    //   default this file's own sibling test_SCK_ART.cpp's DoAutoSocketOff PART already documents for
    //   TestSocket. FTestSuck's cell is always restored to NULL_IC before returning control so later
    //   PARTs (and re-runs within this same process) start clean.
    // =========================================================================================
    printf("\n-- DoARTLotStart --\n");
    {
        bQAModeFlag = false;
        bReadLotInfoFromART = false;
        TestIF_File.bRENESAS_EnableFTCT = false;
        TestIF_File.bAlarmAfterSendSRQKIND2 = false;
        FTestSuck.SetItemData(0, 0, NULL_IC, 0);   // baseline: HasICUnderMachine()==false

        // ---- 9A: HasICUnderMachine()==false -> bChangeLotID FORCED true regardless of sLotID
        //   (golden :4195-4197). iTesterType=0 isolates this sub-case from the independent
        //   iTesterType==1 tail (golden :4243-4254, covered separately by 9F below).
        {
            SckArtRemainderState st;
            st.sLotID = "SAME-LOT";
            st.iTesterType = 0;
            SckArtRem_DoARTLotStart(st, "SAME-LOT", "PROC-A", 100);
            CHECK(bReadLotInfoFromART == true, "DoARTLotStart -> bReadLotInfoFromART=true unconditionally (golden :4193, real cmydef.cpp global)");
            CHECK(st.iCurrent93KARTStep == 1,  "HasICUnderMachine()==false -> bChangeLotID=true -> iCurrent93KARTStep=1 (golden :4195-4213)");
            CHECK(st.iCurrentFlexARTStep == 4, "same branch -> iCurrentFlexARTStep=4 (golden :4214)");
            CHECK(st.iNeedRT == 1,             "ClearLotInfo() always sets iNeedRT=1 (golden :839); iTesterType!=1 tail does not reset it back to 0 here");
            CHECK(st.sLOTSTATUS == "LOTSTATUS_W", "SetLotStatus(iLOTSTATUS_W) inlined -> sLOTSTATUS=\"LOTSTATUS_W\" (golden :4216)");
            CHECK(st.iCurrentStatus == 1,          "SetLotStatus(iLOTSTATUS_W) inlined -> iCurrentStatus=1 (golden :4216, iLOTSTATUS_W==1 per golden ctor SCK_ART.cpp:44)");
            CHECK(st.sLotID == "SAME-LOT",         "sLotID=_sLotID (golden :4217, re-applied after ClearLotInfo cleared it)");
            CHECK(st.sProcessCode == "PROC-A",     "sProcessCode=_sProcess (golden :4218)");
            CHECK(st.sLotStartTime != "",           "bRENESAS_EnableFTCT==false -> sLotStartTime stamped via FormatDateTime(\"yyyymmdd_hhnnss\",Now()) (golden :4219-4220)");
            CHECK(st.iLotCount == 100,               "bQAModeFlag==false -> iLotCount=_iLotCount (golden :4235)");
            CHECK(st.iInputCount == 100,             "bQAModeFlag==false -> iInputCount=_iLotCount (golden :4236)");
        }

        // ---- 9B: HasICUnderMachine()==true + sLotID EXACT match (non-empty, non-space) ->
        //   bChangeLotID=false -> the WHOLE if(bChangeLotID==true) block is SKIPPED; st fields set
        //   to sentinels beforehand must come back UNCHANGED (golden :4201-4211).
        {
            FTestSuck.SetItemData(0, 0, HAS_IC, 0);   // force HasICUnderMachine()==true
            SckArtRemainderState st;
            st.sLotID = "LOT-KEEP";
            st.iCurrent93KARTStep = 999;             // sentinel
            st.sLOTSTATUS = "SENTINEL-UNCHANGED";    // sentinel
            st.iTesterType = 0;
            SckArtRem_DoARTLotStart(st, "LOT-KEEP", "PROC-B", 555);
            CHECK(st.iCurrent93KARTStep == 999,          "sLotID match (non-space) -> bChangeLotID=false -> block SKIPPED, iCurrent93KARTStep untouched");
            CHECK(st.sLOTSTATUS == "SENTINEL-UNCHANGED", "same -> sLOTSTATUS untouched (SetLotStatus never runs)");
            FTestSuck.SetItemData(0, 0, NULL_IC, 0);     // restore baseline
        }

        // ---- 9C: the " " vs "" ASYMMETRY (golden :4201) -- an EXACT match on " " (one literal
        //   space) still forces bChangeLotID=true, because the right AND-term compares _sLotID
        //   against " " (not ""), unlike the left AND-term's sLotID!="" check. Demonstrates the
        //   asymmetry is REAL and preserved, not "fixed".
        {
            FTestSuck.SetItemData(0, 0, HAS_IC, 0);
            SckArtRemainderState st;
            st.sLotID = " ";              // exact match target: one literal space, same as _sLotID below
            st.iCurrent93KARTStep = 999;  // sentinel -- expect this to be OVERWRITTEN despite the exact match
            st.iTesterType = 0;
            SckArtRem_DoARTLotStart(st, " ", "PROC-C", 7);
            CHECK(st.iCurrent93KARTStep == 1, "sLotID==\" \"==_sLotID (EXACT match) but _sLotID!=\" \" is FALSE -> right AND-term false -> bChangeLotID forced TRUE despite the match (golden :4201 asymmetry, preserved verbatim, NOT normalized to \"\")");
            FTestSuck.SetItemData(0, 0, NULL_IC, 0);
        }

        // ---- 9D: HasICUnderMachine()==true + sLotID MISMATCH -> bChangeLotID=true (straightforward
        //   else-branch, golden :4205-4207) ----
        {
            FTestSuck.SetItemData(0, 0, HAS_IC, 0);
            SckArtRemainderState st;
            st.sLotID = "OLD-LOT";
            st.iTesterType = 0;
            SckArtRem_DoARTLotStart(st, "NEW-LOT", "PROC-D", 42);
            CHECK(st.iCurrent93KARTStep == 1, "HasICUnderMachine()==true + sLotID mismatch -> bChangeLotID=true (golden :4205-4207)");
            CHECK(st.sLotID == "NEW-LOT",      "block ran -> sLotID overwritten to _sLotID (golden :4217)");
            FTestSuck.SetItemData(0, 0, NULL_IC, 0);
        }

        // ---- 9E: bQAModeFlag==true -> iLotCount/iInputCount pulled from TestIF_File.iQAModeCount,
        //   NOT _iLotCount; bQAModeFlag reset to false afterward (golden :4225-4231) ----
        {
            TestIF_File.iQAModeCount = 321;
            bQAModeFlag = true;
            SckArtRemainderState st;
            st.iTesterType = 0;
            SckArtRem_DoARTLotStart(st, "QA-LOT", "PROC-E", 9999);
            CHECK(st.iLotCount == 321,    "bQAModeFlag==true -> iLotCount=TestIF_File.iQAModeCount, NOT _iLotCount (golden :4227)");
            CHECK(st.iInputCount == 321,  "bQAModeFlag==true -> iInputCount=TestIF_File.iQAModeCount (golden :4228)");
            CHECK(bQAModeFlag == false,   "bQAModeFlag reset to false after the QA-mode branch runs (golden :4231)");
        }

        // ---- 9F: iTesterType==1 tail -- iNeedRT reset + the 3 REAL LastSet GPIB fields + gates
        //   #9/#10 (fMain->SetLotState/tESDError->Add) run without crashing (golden :4243-4254) ----
        {
            SckArtRemainderState st;
            st.iTesterType = 1;
            st.iNeedRT = 5;   // sentinel, expect reset to 0
            LastSet.bEndLotAutoRetestGPIB = true;
            LastSet.bWaitStartLotAutoRetestGPIB = true;
            LastSet.bFirstTestAutoRetestGPIB = false;
            TestIF_File.bAlarmAfterSendSRQKIND2 = true;   // also exercises gate #10
            SckArtRem_DoARTLotStart(st, "", "", 0);
            CHECK(st.iNeedRT == 0,                              "iTesterType==1 -> iNeedRT=0 (golden :4245)");
            CHECK(LastSet.bEndLotAutoRetestGPIB == false,       "iTesterType==1 -> LastSet.bEndLotAutoRetestGPIB=false (golden :4246, REAL LastSet field, canary_support.h:145)");
            CHECK(LastSet.bWaitStartLotAutoRetestGPIB == false, "iTesterType==1 -> LastSet.bWaitStartLotAutoRetestGPIB=false (golden :4247, REAL LastSet field, canary_support.h:144)");
            CHECK(LastSet.bFirstTestAutoRetestGPIB == true,     "iTesterType==1 -> LastSet.bFirstTestAutoRetestGPIB=true (golden :4248, REAL LastSet field, canary_support.h:146)");
            CHECK(true, "fMain->SetLotState(2) / fMain->tESDError->Add(\"MES07399\") -- gates #9/#10 -- run without crashing (golden :4249-4252)");
            TestIF_File.bAlarmAfterSendSRQKIND2 = false;
        }

        FTestSuck.SetItemData(0, 0, NULL_IC, 0);   // final cleanup: leave the shared global grid clean
    }

    // =========================================================================================
    // PART 10 -- SaveTestSummarySECS -- golden :1647-2044. AI(W906-SaveTestSummarySECS) 20260721.
    //   asSummaryPath is redirected to a scratch dir for the WHOLE part (never the real
    //   D:\HT9045_Log\Summary production path -- restored at the end, same discipline as PART 1/3/3b's
    //   DataPath save/restore). GetTimeInfo() is called once here BEFORE each real call so the test can
    //   reconstruct the exact SystemYear/Month/Date/Hour/Min-stamped filename the function will use
    //   (golden embeds these directly in FileName when bSPILFunction==false); any stale file from a
    //   previous run inside the SAME wall-clock minute is proactively deleted first, so this test is
    //   deterministic across repeated ctest invocations (not just "usually passes").
    // =========================================================================================
    printf("\n-- SaveTestSummarySECS --\n");
    {
        AnsiString savedSummaryPath = asSummaryPath;
        asSummaryPath = ScratchDir() + "W906SaveTestSummarySECSScratch";

        IniConfig.bSPILFunction = false;        // simplest FileName/header-block shape, unconditional SaveToFile (golden :1954-1964)
        IniConfig.bN17UploadLotSummary = false; // THE quirk precondition: strFileName is NEVER assigned (golden :1970-2023 skipped)
        IniConfig.bA38_SLT_Summary = true;      // needed for the ShellExecute-quirk branch (golden :2025)
        CosFunction.bUseTSVFunction = false;    // isolate 10A-10C from the TSV tail (covered separately in 10D)

        // Seed LotSummary so iUnloadCount>0 (TestSocket.iShtRow=2/iShtCol=1 by default -- see PART 9's
        // own comment above -- so valid site indices are 0 and 1; bin 3 is an arbitrary in-range choice,
        // 0<=3<iTestBinCount==16 default).
        W5SckArtRem_LotSummary.iCountCategory[0][3] = 5;
        W5SckArtRem_LotSummary.iTotalCategory[3] = 5;

        // ---- 10A/10B/10C shared setup: one real call, several independent assertions on its effects ----
        SckArtRemainderState st;
        st.sLotID = "LOT-SECS-1";
        st.sProcessCode = "";              // exercises the ""->"FT1" default (golden :1677-1678)
        st.sInfo_Customer = "ACME";
        st.sInfo_Stage = "FT1";
        st.sInfo_Step = "1";
        st.sInfo_ReportCnt = "1";
        st.sInfo_CustLotID = "CUST-LOT-1";
        st.sInfo_TesterID = "TESTER1";
        st.sInfo_HandlerID = "HANDLER1";
        st.sInfo_TestBinNo = "1:2";         // exercises StringReplace(":", "-", rfReplaceAll) -> "1-2"
        st.sLotStartTime = "";              // exercises the RunInfo.LotStartTime fallback branch (golden :1788-1789)
        RunInfo.LotStartTime = "2026-07-21 10:00:00";

        GetTimeInfo();   // snapshot NOW into SystemYear/Month/Date/Hour/Min, to reconstruct the expected filename
        AnsiString expectedFileName, expectedPathName2;
        expectedFileName.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%04d%02d%02d%02d%02d.txt",
                                  AnsiString("FT1"), AnsiString("1"), AnsiString("1"), AnsiString("CUST-LOT-1"),
                                  AnsiString("LOT-SECS-1"), AnsiString("TESTER1"), AnsiString("HANDLER1"), AnsiString("1-2"),
                                  SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
        expectedPathName2.sprintf("%s\\%04d\\%02d\\", asSummaryPath, SystemYear, SystemMonth);
        AnsiString expectedFullPath = expectedPathName2 + expectedFileName;
        if (FileExists(expectedFullPath))
            DeleteFile(expectedFullPath);   // idempotent cleanup -- see this PART's own header note

        W5SckArtRem_LastShellExecuteOpenPath = "<unset>";   // reset gate #11's capture before the call

        SckArtRem_SaveTestSummarySECS(st, /*iSaveData=*/1);

        // ---- 10A: real file-write path, scratch dir only ----
        CHECK(FileExists(expectedFullPath), "SaveTestSummarySECS(iSaveData=1) writes the summary .txt under the SCRATCH asSummaryPath (golden :1954-1964)");
        CHECK(st.sProcessCode == "FT1", "sProcessCode==\"\" defaulted to \"FT1\" (golden :1677-1678)");

        // ---- 10B: fObserver->memoLotSummary->Lines whole-list-assign (golden :1965) ----
        bool foundCustomerLine = false;
        for (size_t i = 0; i < fObserver->memoLotSummary->Lines.Strings.size(); ++i)
            if (fObserver->memoLotSummary->Lines.Strings[i] == "CUSTOMER:ACME")
                foundCustomerLine = true;
        CHECK(fObserver->memoLotSummary->Lines.Strings.size() > 0, "fObserver->memoLotSummary->Lines captured a COPY of sList's content (golden :1965)");
        CHECK(foundCustomerLine, "captured content includes the \"CUSTOMER:ACME\" header line built from st.sInfo_Customer");

        // ---- 10C: the bN17UploadLotSummary-off "strFileName stays empty but ShellExecute still fires"
        //   golden quirk (golden :1970/:2025-2027, this function's own header doc comment "Golden bug
        //   preserved VERBATIM"). bA38_SLT_Summary==true + FileName!="" + iUnloadCount>0 (seeded above)
        //   all hold, so gate #11's stand-in MUST have been invoked -- with an EMPTY path, because
        //   strFileName was never assigned (bN17UploadLotSummary==false skipped that whole block).
        CHECK(W5SckArtRem_LastShellExecuteOpenPath == "", "ShellExecute-open stand-in (gate #11) fired UNCONDITIONALLY with an EMPTY path -- the golden strFileName-can-be-empty quirk, preserved verbatim, not \"fixed\" to skip");

        // ---- 10D: iSaveData==1 -> LotSummary.ClearAllData() actually zeroes the 2 extended fields
        //   (golden :2041-2042; gate #5's [UPDATE] -- this macro is now real, not a no-op) ----
        CHECK(W5SckArtRem_LotSummary.iCountCategory[0][3] == 0, "iSaveData==1 -> LotSummary.ClearAllData() zeroed iCountCategory[0][3] (golden :2042, gate #5 now real)");
        CHECK(W5SckArtRem_LotSummary.iTotalCategory[3] == 0, "iSaveData==1 -> LotSummary.ClearAllData() zeroed iTotalCategory[3] (golden :2042, gate #5 now real)");

        // ---- 10E: TSV-wait tail (golden :2030-2039) -- separate call, re-seed LotSummary (10D just
        //   cleared it) so the file-write/ShellExecute path still runs the same way; only the NEW
        //   CosFunction.bUseTSVFunction/IniConfig.bN09_LotCountAutoFunc gate differs from 10A-10C ----
        {
            W5SckArtRem_LotSummary.iCountCategory[0][3] = 5;
            W5SckArtRem_LotSummary.iTotalCategory[3] = 5;
            CosFunction.bUseTSVFunction = true;
            IniConfig.bN09_LotCountAutoFunc = true;
            IniConfig.dN09_SearchTime = 5.0;
            bWaitTSV = false;

            SckArtRemainderState st2 = st;
            st2.sInfo_ReportCnt = "2";   // distinct filename from 10A-10D's, avoids the golden :1954 dedup-skip
            SckArtRem_SaveTestSummarySECS(st2, /*iSaveData=*/1);

            CHECK(bWaitTSV == true, "CosFunction.bUseTSVFunction && IniConfig.bN09_LotCountAutoFunc -> bWaitTSV=true (golden :2033, real global)");
            CHECK(st2.bShowTSVMsg == false, "same branch -> st.bShowTSVMsg=false (golden :2034, new struct field)");
            CHECK(st2.sTSVMsg == "", "same branch -> st.sTSVMsg=\"\" (golden :2035, new struct field)");
            CosFunction.bUseTSVFunction = false;
            IniConfig.bN09_LotCountAutoFunc = false;
        }

        // ---- 10F: iSaveData==0 -> early return BEFORE any file/list/ShellExecute work (golden
        //   :1699-1700), even though GetTimeInfo()/PathName2/MyForceDirectories/FileName-building
        //   (golden :1670-1697) already ran -- a DIFFERENT sInfo_ReportCnt keeps its (non-)existence
        //   independently checkable from 10A-10E's file ----
        {
            W5SckArtRem_LastShellExecuteOpenPath = "<unset>";
            SckArtRemainderState st3 = st;
            st3.sInfo_ReportCnt = "3";
            GetTimeInfo();
            AnsiString fn3, path3;
            fn3.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%04d%02d%02d%02d%02d.txt",
                        AnsiString("FT1"), AnsiString("1"), AnsiString("3"), AnsiString("CUST-LOT-1"),
                        AnsiString("LOT-SECS-1"), AnsiString("TESTER1"), AnsiString("HANDLER1"), AnsiString("1-2"),
                        SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
            path3.sprintf("%s\\%04d\\%02d\\", asSummaryPath, SystemYear, SystemMonth);
            AnsiString fullPath3 = path3 + fn3;
            if (FileExists(fullPath3))
                DeleteFile(fullPath3);

            SckArtRem_SaveTestSummarySECS(st3, /*iSaveData=*/0);

            CHECK(FileExists(fullPath3) == false, "iSaveData==0 -> early return -> no file written (golden :1699-1700)");
            CHECK(W5SckArtRem_LastShellExecuteOpenPath == "<unset>", "iSaveData==0 -> early return -> ShellExecute stand-in never reached either");
        }

        asSummaryPath = savedSummaryPath;
    }

    printf("\n=== %d PASS, %d FAIL (of %d) ===\n", g_pass, g_fail, g_pass + g_fail);
    return (g_fail == 0) ? 0 : 1;
}
