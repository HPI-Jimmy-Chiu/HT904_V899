// tests/test_SCK_ART_Remainder.cpp
// Verification harness for the Automation/SCK_ART_Remainder.h/.cpp REMAINDER slice
// (W5-Final-SckArtRemainder), continuing the sibling test_SCK_ART.cpp (8-function extract).
//
// Exercises the translated public API against input->expected-output values hand-derived from the
// ORIGINAL golden reference
//   HT9011UC_Code_V3.33.906.0_20260618/Automation/SCK_ART.cpp:183-1645 (see SCK_ART_Remainder.h for
//   the exact per-function golden line ranges).
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
    // PART 8 -- SaveTestSummary -- golden :1619-1645 (dispatch logic only; all 4 callees gated, see
    //   gate #7 -- deferred to next wave). Exercises all 4 branches; nothing externally observable
    //   beyond "does not crash", since the callees are no-op stand-ins.
    // =========================================================================================
    printf("\n-- SaveTestSummary (dispatch logic only -- callees deferred, gate #7) --\n");
    {
        CosFunction.bSortingBy2DList = true;
        LastSet.iTester = _2D_SORT;
        TestIF_File.bSortingBy2DIDList = true;
        SckArtRem_SaveTestSummary(1);
        CHECK(true, "2D-sort branch dispatches to (gated) Save2DSortingSummary without crashing (golden :1621-1626)");

        CosFunction.bSortingBy2DList = false;
        CosFunction.bART_SECSGEM_93K = true;
        SckArtRem_SaveTestSummary(1);
        CHECK(true, "SECS/93K branch dispatches to (gated) SaveTestSummarySECS without crashing (golden :1627-1630)");

        CosFunction.bART_SECSGEM_93K = false;
        TestIF_File.iTestType = TCP_IP_MODE;
        SckArtRem_SaveTestSummary(1);
        CHECK(true, "TCP/IP branch dispatches to (gated) ProcessOSPrint+SaveTestSummaryTSV without crashing (golden :1631-1637)");

        TestIF_File.iTestType = 0;   // not TCP_IP_MODE
        SckArtRem_SaveTestSummary(1);
        CHECK(true, "fallback branch dispatches to (gated) SaveSummaryTrayFeed+SaveTestSummaryTSV without crashing (golden :1638-1644)");
    }

    printf("\n=== %d PASS, %d FAIL (of %d) ===\n", g_pass, g_fail, g_pass + g_fail);
    return (g_fail == 0) ? 0 : 1;
}
