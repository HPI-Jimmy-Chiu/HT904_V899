// tests/test_MainCalcCore.cpp
// Verification harness for the MainCalcCore translation (ComputeShtModeFlag /
// ComputeCanChangeRealDummy / ComputeSiteMapIsStander / ComputeSiteMapPriority /
// ComputeCanChangeSite / ComputeCanChangeToSocket / ComputeATCAmbientTemperCheck /
// ComputeCheckAllMOTHome / ComputeCheckSiteMapState / ComputeCheckOLPErrorHasErr /
// ComputeCheckARTSetupFile / ComputeSMCDLLVersionMismatchCode / ComputeATPDLLVersionMismatch /
// ComputeJamRateRecordStrings / ComputeSetESDTriTempCommand).
//
// Exercises the translated public API with input->expected-output values hand-derived from
// the BCB6 formula in the ORIGINAL golden reference
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:34423-34442 (GetShtModeFlag)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:12374-12380 (CheckCanChangeRealDummy)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:23526-23543 (CheckSiteMapIsStander)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:31187-31208 (CheckSiteMapPriority)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:14350-14393 (CanChangeSite)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:30156-30163 (CanChangeToSocket)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:31942-31962 (ATCAmbientTemperCheck)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:15112-15125 (Check_AllMOT_Home)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:31148-31172 (CheckSiteMapState, no-arg)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:33986-34012 (CheckOLPError, partial)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:32211-32238 (CheckARTSetupFile)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:303-318   (DoCheckSMCDLLVersion, decision tail)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:348-355   (DoCheckATPDLLVersion, decision tail)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:32047-32067 (RecordJamRateByTime, partial)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:34528-34550 (SET_ESD_Tri_Temp)
//
// The ComputeJamRateRecordStrings expected strings below were cross-checked against a
// standalone (non-translation) C snprintf("%.15g", ...) probe of the exact
// iH=(int)(fH*1000); fH=iH/1000.0 pipeline, using only "nice" (power-of-2-fraction) fH values
// so the double->text formatting is unambiguous (1, 1.5, 0.125, 999 -- no repeating-decimal
// rounding uncertainty in the expected literals).
//
// LIMITATION (stated explicitly, same as test_cContact.cpp): we CANNOT run the original BCB6
// binary (no Borland compiler in this environment).  Verification here is therefore:
//   (1) the translation compiles clean under MinGW g++, AND
//   (2) its outputs equal values hand-computed / transcribed from the BCB6 source (golden
//       file:line cited per case).
//
// No external test framework: a tiny check harness prints PASS/FAIL per case and a final
// summary, and returns non-zero on ANY failure.

#include "MainCalcCore.h"
#include <cstdio>
#include <cstring>

static int g_pass = 0;
static int g_fail = 0;

static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected)
    {
        printf("PASS  %-56s got=%-5s exp=%-5s\n", name, got ? "true" : "false", expected ? "true" : "false");
        ++g_pass;
    }
    else
    {
        printf("FAIL  %-56s got=%-5s exp=%-5s\n", name, got ? "true" : "false", expected ? "true" : "false");
        ++g_fail;
    }
}

static void check_i(const char* name, int got, int expected)
{
    if (got == expected)
    {
        printf("PASS  %-56s got=%-6d exp=%-6d\n", name, got, expected);
        ++g_pass;
    }
    else
    {
        printf("FAIL  %-56s got=%-6d exp=%-6d\n", name, got, expected);
        ++g_fail;
    }
}

static void check_s(const char* name, const AnsiString& got, const char* expected)
{
    if (std::strcmp(got.c_str(), expected) == 0)
    {
        printf("PASS  %-56s got=%-10s exp=%-10s\n", name, got.c_str(), expected);
        ++g_pass;
    }
    else
    {
        printf("FAIL  %-56s got=%-10s exp=%-10s\n", name, got.c_str(), expected);
        ++g_fail;
    }
}

int main()
{
    printf("=== MainCalcCore translation verification ===\n");
    printf("(values hand-derived/transcribed from BCB6 main.cpp; original binary NOT run)\n\n");

    // =========================================================================================
    // ComputeShtModeFlag -- BCB6 main.cpp:34423-34442
    // =========================================================================================
    printf("-- ComputeShtModeFlag --\n");
    {
        check_i("iShuttleMode==1,iShuttle_Sel==1 -> 1", ComputeShtModeFlag(1, 1), 1);
        check_i("iShuttleMode==1,iShuttle_Sel==0 -> 0", ComputeShtModeFlag(1, 0), 0);
        check_i("iShuttleMode==1,iShuttle_Sel==2 -> 0 (else)", ComputeShtModeFlag(1, 2), 0);
        check_i("iShuttleMode==0 -> 2 (else)", ComputeShtModeFlag(0, 1), 2);
        check_i("iShuttleMode==2 -> 2 (else)", ComputeShtModeFlag(2, 1), 2);
    }

    // =========================================================================================
    // ComputeCanChangeRealDummy -- BCB6 main.cpp:12374-12380
    // =========================================================================================
    printf("\n-- ComputeCanChangeRealDummy --\n");
    {
        check_b("all HasIC false -> true", ComputeCanChangeRealDummy(false, false, false, false, false, false), true);
        check_b("Plate1HasIC -> false", ComputeCanChangeRealDummy(true, false, false, false, false, false), false);
        check_b("Plate2HasIC -> false", ComputeCanChangeRealDummy(false, true, false, false, false, false), false);
        check_b("ShuttleHasIC -> false", ComputeCanChangeRealDummy(false, false, true, false, false, false), false);
        check_b("IndexHasIC -> false", ComputeCanChangeRealDummy(false, false, false, true, false, false), false);
        check_b("InArmSuckHasIC -> false", ComputeCanChangeRealDummy(false, false, false, false, true, false), false);
        check_b("OutArmSuckHasIC -> false", ComputeCanChangeRealDummy(false, false, false, false, false, true), false);
    }

    // =========================================================================================
    // ComputeSiteMapIsStander -- BCB6 main.cpp:23526-23543
    // =========================================================================================
    printf("\n-- ComputeSiteMapIsStander --\n");
    {
        // 2x2 standard layout: count runs iR+1 per row start, +iRowCount per column step.
        // row0: col0=1, col1=1+2=3;  row1: col0=2, col1=2+2=4.
        int map2x2_ok[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        map2x2_ok[0][0] = 1; map2x2_ok[0][1] = 3;
        map2x2_ok[1][0] = 2; map2x2_ok[1][1] = 4;
        check_b("2x2 standard ascending layout -> true", ComputeSiteMapIsStander(2, 2, map2x2_ok), true);

        // Same layout but [0][1] mismatched (5 instead of 3) -> false.
        int map2x2_bad[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        map2x2_bad[0][0] = 1; map2x2_bad[0][1] = 5;
        map2x2_bad[1][0] = 2; map2x2_bad[1][1] = 4;
        check_b("2x2 with one mismatched non-zero site -> false", ComputeSiteMapIsStander(2, 2, map2x2_bad), false);

        // 0 (unmapped) is a permitted exception per golden's Ifor 20251002 guard.
        int map2x2_zero[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        map2x2_zero[0][0] = 1; map2x2_zero[0][1] = 3;
        map2x2_zero[1][0] = 2; map2x2_zero[1][1] = 0;   // disabled site -> still "stander"
        check_b("2x2 with a 0 (unmapped) site -> true (0 is exempt)", ComputeSiteMapIsStander(2, 2, map2x2_zero), true);

        // 1x3 row (iRowCount==1): count runs 1,2,3 straight across the single row.
        int map1x3_ok[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        map1x3_ok[0][0] = 1; map1x3_ok[0][1] = 2; map1x3_ok[0][2] = 3;
        check_b("1x3 standard ascending row -> true", ComputeSiteMapIsStander(1, 3, map1x3_ok), true);

        int map1x3_bad[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        map1x3_bad[0][0] = 1; map1x3_bad[0][1] = 99; map1x3_bad[0][2] = 3;
        check_b("1x3 with one mismatched site -> false", ComputeSiteMapIsStander(1, 3, map1x3_bad), false);
    }

    // =========================================================================================
    // ComputeSiteMapPriority -- BCB6 main.cpp:31187-31208
    // =========================================================================================
    printf("\n-- ComputeSiteMapPriority --\n");
    {
        // Full 4x8 grid, every cell a distinct value 1..32, iSiteTotal==32 -> no duplicate -> true.
        int mapUnique[MAX_SOCKET_ROW][MAX_SOCKET_COL];
        for (int i = 0; i < MAX_SOCKET_ROW; ++i)
            for (int j = 0; j < MAX_SOCKET_COL; ++j)
                mapUnique[i][j] = i * MAX_SOCKET_COL + j + 1;   // 1..32, all distinct
        check_b("32 distinct site indices, iSiteTotal=32 -> true", ComputeSiteMapPriority(mapUnique, 32), true);

        // Force a duplicate within the checked range: last cell (was 32) becomes 1 (duplicate of [0][0]).
        int mapDup[MAX_SOCKET_ROW][MAX_SOCKET_COL];
        for (int i = 0; i < MAX_SOCKET_ROW; ++i)
            for (int j = 0; j < MAX_SOCKET_COL; ++j)
                mapDup[i][j] = i * MAX_SOCKET_COL + j + 1;
        mapDup[MAX_SOCKET_ROW - 1][MAX_SOCKET_COL - 1] = 1;   // duplicate value 1
        check_b("duplicate site index 1 within iSiteTotal=32 -> false", ComputeSiteMapPriority(mapDup, 32), false);

        // Same duplicate-1 grid, but iSiteTotal==0 -> the i=1..iSiteTotal loop never runs -> true.
        check_b("iSiteTotal=0 short-circuits the check -> true", ComputeSiteMapPriority(mapDup, 0), true);

        // Duplicate a value OUTSIDE the checked [1..iSiteTotal] range: force cell [3][0] (was 25)
        // to 5, duplicating [0][4]==5, but only check iSiteTotal=4 -> duplicate at 5 is not
        // inspected -> true (matches golden: the histogram is built over the WHOLE grid, but the
        // pass/fail loop only walks 1..iSiteTotal).
        int mapDupOutOfRange[MAX_SOCKET_ROW][MAX_SOCKET_COL];
        for (int i = 0; i < MAX_SOCKET_ROW; ++i)
            for (int j = 0; j < MAX_SOCKET_COL; ++j)
                mapDupOutOfRange[i][j] = i * MAX_SOCKET_COL + j + 1;
        mapDupOutOfRange[3][0] = 5;   // duplicate of [0][4]==5, but 5 > iSiteTotal below
        check_b("duplicate index 5 present but iSiteTotal=4 (out of checked range) -> true",
                ComputeSiteMapPriority(mapDupOutOfRange, 4), true);
    }

    // =========================================================================================
    // ComputeCanChangeSite -- BCB6 main.cpp:14350-14393
    // =========================================================================================
    printf("\n-- ComputeCanChangeSite --\n");
    {
        // Branch A: bCanAutoCloseSite==true, bI28_OnOffSiteOnTheFly==false -> 3-way HasIC guard.
        check_b("A1: AutoCloseSite=true,I28=false,no HasIC -> true",
                ComputeCanChangeSite(false, true, false, false, false, false, false, false, false, 1), true);
        check_b("A1: AutoCloseSite=true,I28=false,InArmSuckHasIC -> false",
                ComputeCanChangeSite(false, true, false, true, false, false, false, false, false, 1), false);
        check_b("A1: AutoCloseSite=true,I28=false,InputShuttleHasIC -> false",
                ComputeCanChangeSite(false, true, false, false, true, false, false, false, false, 1), false);
        check_b("A1: AutoCloseSite=true,I28=false,IndexHasIC -> false",
                ComputeCanChangeSite(false, true, false, false, false, true, false, false, false, 1), false);

        // Branch A2: bCanAutoCloseSite==true, bI28_OnOffSiteOnTheFly==true -> no gate at all in this
        // branch (golden has no else for I28==true here) -> falls straight through to final true,
        // even with every HasIC flag set.
        check_b("A2: AutoCloseSite=true,I28=true,all HasIC=true -> true (no gate, golden quirk)",
                ComputeCanChangeSite(false, true, true, true, true, true, true, true, true, 99), true);

        // Branch B1a: bCanAutoCloseSite==false, I28==false, bNoIncludeHotplate==true -> 3-way guard
        // (Plate1/Plate2 NOT checked).
        check_b("B1a: AutoCloseSite=false,I28=false,NoIncludeHotplate=true,no HasIC -> true",
                ComputeCanChangeSite(true, false, false, false, false, false, false, false, false, 1), true);
        check_b("B1a: ...,IndexHasIC=true -> false",
                ComputeCanChangeSite(true, false, false, false, false, true, false, false, false, 1), false);
        check_b("B1a: Plate1HasIC=true is NOT checked in this branch -> true",
                ComputeCanChangeSite(true, false, false, false, false, false, true, false, false, 1), true);

        // Branch B1b: bCanAutoCloseSite==false, I28==false, bNoIncludeHotplate==false -> 5-way guard
        // (Plate1/Plate2 DO matter here).
        check_b("B1b: AutoCloseSite=false,I28=false,NoIncludeHotplate=false,no HasIC -> true",
                ComputeCanChangeSite(false, false, false, false, false, false, false, false, false, 1), true);
        check_b("B1b: Plate1HasIC=true -> false",
                ComputeCanChangeSite(false, false, false, false, false, false, true, false, false, 1), false);
        check_b("B1b: Plate2HasIC=true -> false",
                ComputeCanChangeSite(false, false, false, false, false, false, false, true, false, 1), false);
        check_b("B1b: InArmSuckHasIC=true -> false",
                ComputeCanChangeSite(false, false, false, true, false, false, false, false, false, 1), false);

        // Branch B2: bCanAutoCloseSite==false, I28==true -> bPickFromLoader/iPickFromLoadStageTask gate.
        check_b("B2: AutoCloseSite=false,I28=true,PickFromLoader=false,Task=1 -> true",
                ComputeCanChangeSite(false, false, true, false, false, false, false, false, false, 1), true);
        check_b("B2: PickFromLoader=true,Task=1 -> false",
                ComputeCanChangeSite(false, false, true, false, false, false, false, false, true, 1), false);
        check_b("B2: PickFromLoader=false,Task=2(!=1) -> false",
                ComputeCanChangeSite(false, false, true, false, false, false, false, false, false, 2), false);
    }

    // =========================================================================================
    // ComputeCanChangeToSocket -- BCB6 main.cpp:30156-30163
    // =========================================================================================
    printf("\n-- ComputeCanChangeToSocket --\n");
    {
        check_b("SystemStart=true -> false regardless of anything else",
                ComputeCanChangeToSocket(true, true, true, true, true, true, true, true, true, 1), false);

        // SystemStart=false, and ComputeCanChangeSite(false,...) with no HasIC/no auto-close/no I28
        // -> B1b all-false path -> true -> overall true.
        check_b("SystemStart=false, inner CanChangeSite -> true -> true",
                ComputeCanChangeToSocket(false, false, false, false, false, false, false, false, false, 1), true);

        // SystemStart=false, but Plate1HasIC=true drives the inner B1b 5-way guard to false.
        check_b("SystemStart=false, Plate1HasIC drives inner CanChangeSite -> false -> false",
                ComputeCanChangeToSocket(false, false, false, false, false, false, true, false, false, 1), false);
    }

    // =========================================================================================
    // ComputeATCAmbientTemperCheck -- BCB6 main.cpp:31942-31962
    //   eNewATCSystem==6 (MachineType.h eATCType); ATC_TYPE_33/35/61 (ATC_Handler_Side.h)
    // =========================================================================================
    printf("\n-- ComputeATCAmbientTemperCheck --\n");
    {
        // Ambient temp inside [25,30] and !=0 -> outer guard never fires -> true, regardless
        // of ATC system/mode.
        check_b("temp=27 (in [25,30]) -> true regardless of ATC system",
                ComputeATCAmbientTemperCheck(27.0, 0, 0), true);

        // temp==0 (unconfigured sentinel) -> outer guard fires; new system + matching mode,
        // but 0 is NOT < -5 -> inner check doesn't fail -> true.
        check_b("temp=0 (sentinel), new ATC system, mode=33, 0 is not < -5 -> true",
                ComputeATCAmbientTemperCheck(0.0, 6, 33), true);

        // temp<-5, new system (6), mode==33 -> inner check fires -> false.
        check_b("temp=-10, new ATC system, mode=33, -10<-5 -> false",
                ComputeATCAmbientTemperCheck(-10.0, 6, 33), false);
        check_b("temp=-10, new ATC system, mode=35 -> false",
                ComputeATCAmbientTemperCheck(-10.0, 6, 35), false);
        check_b("temp=-10, new ATC system, mode=61 -> false",
                ComputeATCAmbientTemperCheck(-10.0, 6, 61), false);

        // temp=-3 (>= -5) with new system + matching mode -> inner check does NOT fire -> true.
        check_b("temp=-3 (>=-5), new ATC system, mode=61 -> true",
                ComputeATCAmbientTemperCheck(-3.0, 6, 61), true);

        // temp=20 (<25) but mode=99 doesn't match 33/35/61 -> falls to the else branch -> false.
        check_b("temp=20 (<25), new ATC system, mode=99 (no match) -> false",
                ComputeATCAmbientTemperCheck(20.0, 6, 99), false);

        // temp=35 (>30), ATC system NOT new (0) -> else branch -> false.
        check_b("temp=35 (>30), old ATC system (0) -> false",
                ComputeATCAmbientTemperCheck(35.0, 0, 33), false);
    }

    // =========================================================================================
    // ComputeCheckAllMOTHome -- BCB6 main.cpp:15112-15125
    //   TOTAL_MOTOR=164 (cmydef.h:33); MTrayX=30, MInArmY=1, MOutArmY=20 (cmydef.cpp)
    // =========================================================================================
    printf("\n-- ComputeCheckAllMOTHome --\n");
    {
        const int kTotalMotor = 164;
        bool allHome[kTotalMotor];

        // All motors at home -> true.
        for (int i = 0; i < kTotalMotor; ++i) allHome[i] = true;
        check_b("all 164 motors at home -> true", ComputeCheckAllMOTHome(allHome), true);

        // A single non-skip-list motor (index 5) off-home -> false.
        bool oneOff[kTotalMotor];
        for (int i = 0; i < kTotalMotor; ++i) oneOff[i] = true;
        oneOff[5] = false;
        check_b("motor 5 (not in skip-list) off-home -> false", ComputeCheckAllMOTHome(oneOff), false);

        // Only the 3 skip-list motors (MTrayX=30, MInArmY=1, MOutArmY=20) off-home -> still true.
        bool skipOnly[kTotalMotor];
        for (int i = 0; i < kTotalMotor; ++i) skipOnly[i] = true;
        skipOnly[30] = false; skipOnly[1] = false; skipOnly[20] = false;
        check_b("only MTrayX/MInArmY/MOutArmY off-home -> true (skip-list)",
                ComputeCheckAllMOTHome(skipOnly), true);

        // A skip-list motor (MInArmY=1) AND a non-skip motor (index 50) both off-home -> false
        // (confirms the skip only exempts the 3 named indices, not everything after them).
        bool mixedOff[kTotalMotor];
        for (int i = 0; i < kTotalMotor; ++i) mixedOff[i] = true;
        mixedOff[1] = false; mixedOff[50] = false;
        check_b("MInArmY off-home (skipped) + motor 50 off-home (not skipped) -> false",
                ComputeCheckAllMOTHome(mixedOff), false);
    }

    // =========================================================================================
    // ComputeCheckSiteMapState -- BCB6 main.cpp:31148-31172 (no-arg overload only)
    //   Returns raw bHasErr (see header polarity note); ErrPart is an append-only out-param.
    // =========================================================================================
    printf("\n-- ComputeCheckSiteMapState --\n");
    {
        // No sites mapped -> no error, ErrPart stays empty.
        int zeroMap[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        int zeroDut[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        AnsiString names1[MAX_SOCKET_ROW][MAX_SOCKET_COL];
        AnsiString errPart1 = "";
        bool r1 = ComputeCheckSiteMapState(zeroMap, zeroDut, names1, errPart1);
        check_b("all iSiteMap==0 -> bHasErr=false", r1, false);
        check_s("all iSiteMap==0 -> ErrPart empty", errPart1, "");

        // Site [0][0] mapped (>=1) but Index shows it OFF (iDutOnOff0==0) -> one error.
        int map2[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        int dut2[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        map2[0][0] = 1;
        dut2[0][0] = 0;
        AnsiString names2[MAX_SOCKET_ROW][MAX_SOCKET_COL];
        names2[0][0] = "S00";
        AnsiString errPart2 = "";
        bool r2 = ComputeCheckSiteMapState(map2, dut2, names2, errPart2);
        check_b("[0][0] mapped + Index off -> bHasErr=true", r2, true);
        check_s("[0][0] mapped + Index off -> ErrPart==S00", errPart2, "S00");

        // Site mapped AND Index shows it ON (iDutOnOff0!=0) -> NOT an error.
        int map3[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        int dut3[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        map3[1][2] = 1;
        dut3[1][2] = 1;   // Index still ON -> no mismatch
        AnsiString names3[MAX_SOCKET_ROW][MAX_SOCKET_COL];
        names3[1][2] = "S12";
        AnsiString errPart3 = "";
        bool r3 = ComputeCheckSiteMapState(map3, dut3, names3, errPart3);
        check_b("[1][2] mapped + Index ON -> bHasErr=false (no mismatch)", r3, false);
        check_s("[1][2] mapped + Index ON -> ErrPart stays empty", errPart3, "");

        // Two error cells in row-major order -> ErrPart concatenates in scan order (i then j).
        int map4[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        int dut4[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{0}};
        map4[0][0] = 1; dut4[0][0] = 0;
        map4[2][3] = 1; dut4[2][3] = 0;
        AnsiString names4[MAX_SOCKET_ROW][MAX_SOCKET_COL];
        names4[0][0] = "A";
        names4[2][3] = "B";
        AnsiString errPart4 = "";
        bool r4 = ComputeCheckSiteMapState(map4, dut4, names4, errPart4);
        check_b("two mismatched cells -> bHasErr=true", r4, true);
        check_s("two mismatched cells -> ErrPart==AB (row-major scan order)", errPart4, "AB");
    }

    // =========================================================================================
    // ComputeCheckOLPErrorHasErr -- BCB6 main.cpp:33986-34012 (partial: pure reduction only)
    // =========================================================================================
    printf("\n-- ComputeCheckOLPErrorHasErr --\n");
    {
        int allZero[10] = {0,0,0,0,0,0,0,0,0,0};
        check_b("all OLPSetBinErr==0 -> false", ComputeCheckOLPErrorHasErr(allZero), false);

        int oneAtThreshold[10] = {0,0,3,0,0,0,0,0,0,0};
        check_b("OLPSetBinErr[2]==3 (threshold) -> true", ComputeCheckOLPErrorHasErr(oneAtThreshold), true);

        int belowThreshold[10] = {0,2,0,0,0,0,0,0,0,0};
        check_b("OLPSetBinErr[1]==2 (below threshold) -> false", ComputeCheckOLPErrorHasErr(belowThreshold), false);

        int lastIndex[10] = {0,0,0,0,0,0,0,0,0,5};
        check_b("OLPSetBinErr[9]==5 (last index) -> true", ComputeCheckOLPErrorHasErr(lastIndex), true);
    }

    // =========================================================================================
    // ComputeCheckARTSetupFile -- BCB6 main.cpp:32211-32238
    //   eAuto3==2, eartInstall==1 (MachineType.h); bAutoRetest[1]=true, iCatDataT3Pos matches
    //   i==1 at j==1 and j==3, so the "BinSelect match" gate is satisfied for i=1 in all cases
    //   below unless bAutoRetest is explicitly all-false.
    // =========================================================================================
    printf("\n-- ComputeCheckARTSetupFile --\n");
    {
        bool bAutoRetest[3]     = {false, true, false};   // only index 1 qualifies
        int  iCatDataT3Pos[4]   = {0, 1, 2, 1};            // j=1 and j=3 both map to i=1
        // 1 == MachineType.h eAutoRetest::eartInstall; 0 == eartUninstall (raw int literals,
        // same idiom as this file's other tests, e.g. ComputeShtModeFlag's iShuttleMode==1).

        check_b("gate off (USE_AUTO_RETEST != eartInstall) -> false regardless of filename",
                ComputeCheckARTSetupFile(0, true, bAutoRetest, iCatDataT3Pos, 4,
                                          AnsiString("XYZ_ART001")), false);

        check_b("gate on but bA10_AutoReTest=false -> false",
                ComputeCheckARTSetupFile(1, false, bAutoRetest, iCatDataT3Pos, 4,
                                          AnsiString("XYZ_ART001")), false);

        bool noMatch[3] = {false, false, false};
        check_b("gate on, bA10=true, but no BinSelect cell qualifies -> false",
                ComputeCheckARTSetupFile(1, true, noMatch, iCatDataT3Pos, 4,
                                          AnsiString("XYZ_ART001")), false);

        check_b("qualifying cell + filename contains \"_ART\" (Pos!=0, any position) -> true",
                ComputeCheckARTSetupFile(1, true, bAutoRetest, iCatDataT3Pos, 4,
                                          AnsiString("XYZ_ART001")), true);

        check_b("qualifying cell + filename STARTS WITH \"9203\" (Pos==1) -> true",
                ComputeCheckARTSetupFile(1, true, bAutoRetest, iCatDataT3Pos, 4,
                                          AnsiString("9203_setup.cfg")), true);

        check_b("qualifying cell + \"9203\" present but NOT at position 1 -> false",
                ComputeCheckARTSetupFile(1, true, bAutoRetest, iCatDataT3Pos, 4,
                                          AnsiString("ABC9203.cfg")), false);

        check_b("qualifying cell + filename matches none of the 7 keywords -> false",
                ComputeCheckARTSetupFile(1, true, bAutoRetest, iCatDataT3Pos, 4,
                                          AnsiString("NormalSetup.cfg")), false);

        // Sweep the remaining 5 position-anchored keywords individually.
        check_b("keyword \"5611\" at position 1 -> true",
                ComputeCheckARTSetupFile(1, true, bAutoRetest, iCatDataT3Pos, 4,
                                          AnsiString("5611_setup.cfg")), true);
        check_b("keyword \"9287\" at position 1 -> true",
                ComputeCheckARTSetupFile(1, true, bAutoRetest, iCatDataT3Pos, 4,
                                          AnsiString("9287xxx.cfg")), true);
        check_b("keyword \"3971\" at position 1 -> true",
                ComputeCheckARTSetupFile(1, true, bAutoRetest, iCatDataT3Pos, 4,
                                          AnsiString("3971abc.cfg")), true);
        check_b("keyword \"9606\" at position 1 -> true",
                ComputeCheckARTSetupFile(1, true, bAutoRetest, iCatDataT3Pos, 4,
                                          AnsiString("9606abc.cfg")), true);
        check_b("keyword \"KL\" at position 1 -> true",
                ComputeCheckARTSetupFile(1, true, bAutoRetest, iCatDataT3Pos, 4,
                                          AnsiString("KLxxx.cfg")), true);
        check_b("keyword \"9378\" at position 1 -> true",
                ComputeCheckARTSetupFile(1, true, bAutoRetest, iCatDataT3Pos, 4,
                                          AnsiString("9378xxx.cfg")), true);
    }

    // =========================================================================================
    // ComputeSMCDLLVersionMismatchCode -- BCB6 main.cpp:303-318 (decision tail of
    // DoCheckSMCDLLVersion, a free function). CSMCDLLRevision == "3.15.0.0" (MachineType.h:111).
    // =========================================================================================
    printf("\n-- ComputeSMCDLLVersionMismatchCode --\n");
    {
        check_i("both match CSMCDLLRevision -> 0 (OK)",
                ComputeSMCDLLVersionMismatchCode(AnsiString("3.15.0.0"), AnsiString("3.15.0.0")), 0);
        check_i("only [0] mismatches -> 1",
                ComputeSMCDLLVersionMismatchCode(AnsiString("1.0.0.0"), AnsiString("3.15.0.0")), 1);
        check_i("only [1] mismatches -> 2",
                ComputeSMCDLLVersionMismatchCode(AnsiString("3.15.0.0"), AnsiString("1.0.0.0")), 2);
        check_i("both mismatch -> 3",
                ComputeSMCDLLVersionMismatchCode(AnsiString("1.0.0.0"), AnsiString("2.0.0.0")), 3);
        check_i("both empty (never populated) -> 3",
                ComputeSMCDLLVersionMismatchCode(AnsiString(""), AnsiString("")), 3);
    }

    // =========================================================================================
    // ComputeATPDLLVersionMismatch -- BCB6 main.cpp:348-355 (decision tail of
    // DoCheckATPDLLVersion, a free function). ATPDLLVersion == "1.0.0.1" (MachineType.h:113).
    // =========================================================================================
    printf("\n-- ComputeATPDLLVersionMismatch --\n");
    {
        check_i("matches ATPDLLVersion -> 0 (OK)",
                ComputeATPDLLVersionMismatch(AnsiString("1.0.0.1")), 0);
        check_i("mismatches ATPDLLVersion -> 1",
                ComputeATPDLLVersionMismatch(AnsiString("1.0.0.0")), 1);
        check_i("empty (never populated) -> 1",
                ComputeATPDLLVersionMismatch(AnsiString("")), 1);
    }

    // =========================================================================================
    // ComputeJamRateRecordStrings -- BCB6 main.cpp:32047-32067 (partial extract of
    // RecordJamRateByTime). Both out-params are unconditionally overwritten each call.
    // =========================================================================================
    printf("\n-- ComputeJamRateRecordStrings --\n");
    {
        // Case A: JamCount==0 branch. IntervalTime=60min(1hr) -> fH=60/60=1.0 -> iH=1000 ->
        // fH=1.0 -> "1".
        AnsiString mtbfA = "", jamA = "";
        ComputeJamRateRecordStrings(0, 10, 60, mtbfA, jamA);
        check_s("A: JamCount=0,Loader=10,Interval=60 -> sMTBFRecord", mtbfA, "  MTBF 0/1hr");
        check_s("A: JamCount=0,Loader=10,Interval=60 -> sJamRateRecord", jamA,
                "[Jam Rate Record] 0/10  MTBF 0/1hr");

        // Case B: JamCount==0 branch, non-integer hours. IntervalTime=90min(1.5hr) ->
        // fH=90/60=1.5 (exact in binary) -> iH=1500 -> fH=1.5 -> "1.5".
        AnsiString mtbfB = "", jamB = "";
        ComputeJamRateRecordStrings(0, 0, 90, mtbfB, jamB);
        check_s("B: JamCount=0,Loader=0,Interval=90 -> sMTBFRecord", mtbfB, "  MTBF 0/1.5hr");
        check_s("B: JamCount=0,Loader=0,Interval=90 -> sJamRateRecord", jamB,
                "[Jam Rate Record] 0/0  MTBF 0/1.5hr");

        // Case C: JamCount!=0 branch. IntervalTime=120min(2hr), JamCount=3 ->
        // fH=3/(120/60)=3/2=1.5 -> iH=1500 -> fH=1.5 -> "1.5". Note the "MTBF 1/" fixed-digit
        // quirk: JamCount is 3, but the record still literally says "1", not "3".
        AnsiString mtbfC = "", jamC = "";
        ComputeJamRateRecordStrings(3, 50, 120, mtbfC, jamC);
        check_s("C: JamCount=3,Loader=50,Interval=120 -> sMTBFRecord (fixed '1', GOLDEN QUIRK)",
                mtbfC, "  MTBF 1/1.5hr");
        check_s("C: JamCount=3,Loader=50,Interval=120 -> sJamRateRecord", jamC,
                "[Jam Rate Record] 3/50  MTBF 1/1.5hr");

        // Case D: JamCount!=0 branch, eighths fraction. IntervalTime=480min(8hr), JamCount=1 ->
        // fH=1/(480/60)=1/8=0.125 (exact in binary) -> iH=125 -> fH=0.125 -> "0.125".
        AnsiString mtbfD = "", jamD = "";
        ComputeJamRateRecordStrings(1, 7, 480, mtbfD, jamD);
        check_s("D: JamCount=1,Loader=7,Interval=480 -> sMTBFRecord", mtbfD, "  MTBF 1/0.125hr");
        check_s("D: JamCount=1,Loader=7,Interval=480 -> sJamRateRecord", jamD,
                "[Jam Rate Record] 1/7  MTBF 1/0.125hr");

        // Case E: GOLDEN QUIRK spotlight -- large JamCount=999, Interval=60(1hr) ->
        // fH=999/(60/60)=999/1=999.0 -> iH=999000 -> fH=999.0 -> "999". sMTBFRecord still
        // literally reads "MTBF 1/999hr" (the fixed '1', NOT the real JamCount=999) --
        // main.cpp:32063's literal "MTBF 1/" preserved verbatim, not "fixed".
        AnsiString mtbfE = "", jamE = "";
        ComputeJamRateRecordStrings(999, 20, 60, mtbfE, jamE);
        check_s("E: JamCount=999 (large) -> sMTBFRecord still says fixed '1' (GOLDEN QUIRK)",
                mtbfE, "  MTBF 1/999hr");
        check_s("E: JamCount=999,Loader=20,Interval=60 -> sJamRateRecord", jamE,
                "[Jam Rate Record] 999/20  MTBF 1/999hr");

        // Case F: out-params are unconditionally OVERWRITTEN, not appended -- pre-seed both
        // with garbage and confirm it is fully replaced (not left as a stale prefix).
        AnsiString mtbfF = "STALE", jamF = "STALE";
        ComputeJamRateRecordStrings(0, 10, 60, mtbfF, jamF);
        check_s("F: pre-seeded out-params are fully overwritten, not appended-to", mtbfF,
                "  MTBF 0/1hr");
    }

    // =========================================================================================
    // ComputeSetESDTriTempCommand -- BCB6 main.cpp:34528-34550
    //   Tempture_Hot=1, Tempture_AmbientHot=3 (cmydef.cpp); ESD_TemperatureAmbient=82,
    //   ESD_TemperatureHot=83, ESD_TemperatureCold=84, ESD_TemperatureSuperHot=88
    //   (Interface/InterfaceSYS.h ESD_COMMAND enum); kNoESDTriTempCommand==-1 (MainCalcCore.h).
    // =========================================================================================
    printf("\n-- ComputeSetESDTriTempCommand --\n");
    {
        // Outer guard fails (USE_NOVX3360!=true) -> no command regardless of anything else.
        check_i("USE_NOVX3360=0 -> kNoESDTriTempCommand",
                ComputeSetESDTriTempCommand(0, 1, 25.0, 1), kNoESDTriTempCommand);

        // Outer guard fails (Tri_Temp_Machine!=1) -> no command.
        check_i("USE_NOVX3360=1,TriTempMachine=2 -> kNoESDTriTempCommand",
                ComputeSetESDTriTempCommand(1, 2, 100.0, 1), kNoESDTriTempCommand);

        // Guard passes, iTemperature==Tempture_Hot(1), fWorkTemperBase=25 falls in the golden
        // GAP [10,40) -- none of the 3 inner branches match -> no command (GOLDEN QUIRK).
        check_i("Hot branch, temp=25 (in [10,40) gap) -> kNoESDTriTempCommand (GOLDEN QUIRK)",
                ComputeSetESDTriTempCommand(1, 1, 25.0, 1), kNoESDTriTempCommand);

        // Same gap, exactly at the lower boundary 10 (not <10, since the test is strict <10).
        check_i("Hot branch, temp=10 (boundary, not <10) -> kNoESDTriTempCommand (GOLDEN QUIRK)",
                ComputeSetESDTriTempCommand(1, 1, 10.0, 1), kNoESDTriTempCommand);

        // fWorkTemperBase>=40 && <=130 -> ESD_TemperatureHot(83). Boundary 40 included.
        check_i("Hot branch, temp=40 (lower boundary of Hot range) -> ESD_TemperatureHot(83)",
                ComputeSetESDTriTempCommand(1, 1, 40.0, 1), 83);
        check_i("Hot branch, temp=130 (upper boundary of Hot range) -> ESD_TemperatureHot(83)",
                ComputeSetESDTriTempCommand(1, 1, 130.0, 1), 83);

        // fWorkTemperBase>130 -> ESD_TemperatureSuperHot(88).
        check_i("Hot branch, temp=131 (>130) -> ESD_TemperatureSuperHot(88)",
                ComputeSetESDTriTempCommand(1, 1, 131.0, 1), 88);

        // fWorkTemperBase<10 -> ESD_TemperatureCold(84).
        check_i("Hot branch, temp=9.9 (<10) -> ESD_TemperatureCold(84)",
                ComputeSetESDTriTempCommand(1, 1, 9.9, 1), 84);

        // iTemperature==Tempture_AmbientHot(3) -> ESD_TemperatureAmbient(82), fWorkTemperBase
        // irrelevant to this branch.
        check_i("AmbientHot branch (temp param irrelevant) -> ESD_TemperatureAmbient(82)",
                ComputeSetESDTriTempCommand(1, 1, 999.0, 3), 82);

        // iTemperature is neither Tempture_Hot nor Tempture_AmbientHot -> the catch-all else
        // sends the SAME ESD_TemperatureAmbient(82) as the AmbientHot branch (golden keeps
        // these as two separate branches with an identical result -- preserved verbatim).
        check_i("neither Hot nor AmbientHot (else branch) -> ESD_TemperatureAmbient(82) too",
                ComputeSetESDTriTempCommand(1, 1, 999.0, 0), 82);
    }

    printf("\n=== Summary: %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
