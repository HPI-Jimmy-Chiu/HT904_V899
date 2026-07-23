// tests/test_MainCalcCore.cpp
// Verification harness for the MainCalcCore translation (ComputeShtModeFlag /
// ComputeCanChangeRealDummy / ComputeSiteMapIsStander / ComputeSiteMapPriority /
// ComputeCanChangeSite / ComputeCanChangeToSocket).
//
// Exercises the translated public API with input->expected-output values hand-derived from
// the BCB6 formula in the ORIGINAL golden reference
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:34423-34442 (GetShtModeFlag)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:12374-12380 (CheckCanChangeRealDummy)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:23526-23543 (CheckSiteMapIsStander)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:31187-31208 (CheckSiteMapPriority)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:14350-14393 (CanChangeSite)
//   HT9011UC_Code_V3.33.906.0_20260618/main.cpp:30156-30163 (CanChangeToSocket)
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

    printf("\n=== Summary: %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
