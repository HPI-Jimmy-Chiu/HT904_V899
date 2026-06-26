// =============================================================================
//  test_w6_2b_inarm_variants.cpp  --  W6.2b-canary VERIFY: in-arm site variants
//
//  Translation wave: W6.2b-canary (in-arm per-site-config variant dispatch)
//  Author: AI(W6.2b-canary) 20260626
//
//  PURPOSE
//  -------
//  The task-mandated *variant dispatch* verify TU.  Proves that:
//    (1) the in-arm ENGINE's two dispatch ladders (DoInArm_9045 /
//        DoInArm_9045_SuckerMap, ainarm9045.cpp) route the e9045_1x1_1 /
//        e9045_1x4_1_Ac iInArmType to the translated 1x1_1 variant
//        (ainarm9045_1x1_1.cpp) over the Sim HAL with NO hardware;
//    (2) the 1x1_1 variant SMs (DoInArm_9045_1x1_1 master pick SM +
//        DoInArm_9045_1x1_1_SuckerMap picker-map builder) PUMP/RUN and produce
//        the golden-hand-derived oracle values;
//    (3) ONLY the 1x1_1 arm was un-gated: any still-gated iInArmType (e.g.
//        e9045_2x4_8 / e9045_2x8_32) hits the Program-Error else and leaves the
//        master cursor UNCHANGED (no spurious dispatch).
//
//  RECONCILIATION (verified against the golden, NOT assumed -- documented honestly):
//  -------------------------------------------------------------------------------
//  Of the 3 named "target dispatch arms" only ONE exists in the golden engine:
//    * e9045_1x1_1 (=0) / e9045_1x4_1_Ac (=1) -> DoInArm_9045_1x1_1()  [LIVE]
//    * There is NO e9045_2x4_16 enum (MachineType.h:1014-1040: e9045_2x4_8=22,
//      e9045_2x8_32=25, e9045_2x8_8=26; no 2x4_16) and NO DoInArm_9045_2x4_16
//      dispatch arm anywhere -- ainarm9045_2x4_16.cpp is DEAD (registered to
//      compile, never dispatched).
//    * The golden engine routes e9045_2x8_32 -> DoInArm_9045_2x8_8() (golden
//      ainarm9045.cpp:4639-4641), NOT to DoInArm_9045_2x8_32() -- so
//      ainarm9045_2x8_32.cpp is ALSO DEAD (its whole body is #if 0 // TODO(W7);
//      it needs Prod.fInArmSuckUse_TwoArm32Site, absent from golden cprod.h by
//      design).  e9045_2x8_8 is itself NOT translated this wave, so e9045_2x8_32
//      remains GATED here and is exercised as a "gated type -> Program-Error".
//  Therefore NO dispatch-routing oracle is derivable for _2x4_16 / _2x8_32; this
//  TU instead uses them as the "still-gated iInArmType -> Program-Error" witnesses
//  that prove ONLY the 1x1_1 arm was un-gated.
//
//  ORACLES (hand-derived from the golden 1x1_1 source -- cited per assert):
//  ------------------------------------------------------------------------
//   [D1] iInArmType=e9045_1x1_1, iArmTask=1, no IC -> DoInArm_9045() enters the
//        1x1_1 arm: case 1 (MoveInArm2XYToWait()==true) Task=10 + FALL-THROUGH
//        (golden 1x1_1 :1770-1771 `if(Task!=10)break;` false); case 10 no-IC
//        else (CheekNeed..==false) Task=50 + FALL-THROUGH (:1861 `if(Task!=50)
//        break;` false); case 50 CheckHeaterOK()==true -> InitArmPickFromLoad
//        StageTask() + Task=75 (:1870-1871).  Offline LastSet.iTemperature==
//        Ambient + cleared MOT[] tray state -> the Tempture_Hot tail branches are
//        skipped -> cursor SETTLES at 75 after one tick.  Cursor must stay in the
//        documented switch(Task) set {1,10,50,75,100,200,400,500,600,999,1000,
//        1100,1500,1550,2000,2100,15000}.
//   [D2] iInArmType=e9045_1x4_1_Ac -> DoInArm_9045() routes to the SAME
//        DoInArm_9045_1x1_1() (golden :4563); identical advance out of entry.
//   [D3] iInArmType=e9045_2x4_8 (gated) -> DoInArm_9045() hits the Program-Error
//        else; iArmTask UNCHANGED (proves only 1x1_1 was un-gated).
//   [D4] iInArmType=e9045_2x8_32 (gated) -> same Program-Error else; UNCHANGED.
//   [S1] DoInArm_9045_1x1_1_SuckerMap() default branch (golden 1x1_1 :1426-1433):
//        seed LastSet.bUseTestSocket[0][0][0]=true, TestIF.iSiteMap[0][0]=7,
//        iInArmType=e9045_1x1_1, TestIF_File.bSingleUseOtherSuck=false (iSuckCol=0)
//        -> Prod.fInArmSuck4x8[0][0][0]==true, Prod.bInSuckUse[0][0][0]==true,
//        Prod.iSiteMap[0][0][0]==7, Prod.iSiteMap[1][0][0]==7; GetShuttleCol(0,0)
//        ==0 (W7-gated default == golden :507-510 for e9045_1x1_1).
//   [S2] DoInArm_9045_SuckerMap() routes e9045_1x1_1 -> the same builder (the
//        Prod grid is written); a gated type -> Program-Error, grid stays cleared.
//   [S3] BONUS: TestIF_File.bSingleUseOtherSuck=true -> iSuckCol=1 -> writes land
//        at [.][0][1] (golden 1x1_1 :1385-1389).
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here ==
//  clean g++ compile/link + the dispatch routes correctly + the 1x1_1 variant SMs
//  pump over the Sim HAL without crash + outputs match values hand-derived from
//  the golden formula/cursor logic.
//
//  Prints PASS/FAIL per check + a final summary; returns non-zero on any FAIL.
// =============================================================================
#include "ainarm9045.h"            // DoInArm_9045 / DoInArm_9045_SuckerMap / GetShuttleCol
#include "ainarm9045_1x1_1.h"      // variant public surface (SetInOutArmParameter_1x1_1, ...)
#include "aHotPlateSubstrate.h"    // InArmSuck / FLCarryKit / BLCarryKit / iArmTask / bPlaceToHotplate
#include "csystem.h"
#include "Motor/mymotor.h"
#include "cprod.h"
#include "cpublic.h"
#include "cmydef.h"
#include "MachineType.h"           // eInArmType enum values
#include "canary_support.h"        // LastSet, ShowErrorMessage, ...
#include "FormsFacade.h"
#include <cstdio>

// The 1x1_1 master pick SM (DoInArm_9045_1x1_1) and its picker-map builder
// (DoInArm_9045_1x1_1_SuckerMap) are file-local in the golden (consumed via the
// engine's dispatch ladder), so they are NOT on the ainarm9045_1x1_1.h surface.
// Declare them locally for the direct-pump assertions -- the engine declares the
// SAME two externs near its dispatch ladders (ainarm9045.cpp, golden :4391/:4650).
extern void DoInArm_9045_1x1_1();
extern void DoInArm_9045_1x1_1_SuckerMap();

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as the other W6 verify TUs)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) state set for DoInArm_9045_1x1_1 (golden 1x1_1) -
//   case labels grep'd from ainarm9045_1x1_1.cpp master SM (entry @ :1702).
static bool inArm1x1CursorSane(int t)
{
    switch (t) {
        case 1:   case 10:   case 50:   case 75:   case 100:  case 200:
        case 400: case 500:  case 600:  case 999:  case 1000: case 1100:
        case 1500: case 1550: case 2000: case 2100: case 15000:
            return true;
        default:
            return false;
    }
}

// Reset the in-arm grid + carry kits + the relevant master flags to a clean,
// no-IC offline baseline before a dispatch/pump probe.
static void resetInArmBaseline()
{
    FLCarryKit.ClearAll();
    BLCarryKit.ClearAll();
    InArmSuck.SetAllToNullIC();          // HasIC() / HasRealIC() -> false
    bPlaceToHotplate     = false;
    bPickFromHotplate    = false;
    bIndexAlarmInArmAway = false;
    bIndexDropVacuumError= false;
    LastSet.iTemperature = Tempture_Ambient;   // skip Tempture_Hot tail branches
    LastSet.iRunStartMode= 0;                  // not rsmAutoSiteMap / FIFO
}

// Clear the Prod picker grids the SuckerMap builder writes (mirror the
// DoInArm_9045_SuckerMap pre-clear so a "no write" gated path is observable).
static void clearProdSuckGrid()
{
    for (int s = 0; s < 2; ++s)
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 8; ++j) {
                Prod.fInArmSuck4x8[s][i][j] = false;
                Prod.bInSuckUse[s][i][j]    = false;
                Prod.iSiteMap[s][i][j]      = -1;
            }
}

int main()
{
    printf("==== W6.2b-canary InArm site-variant dispatch verify ====\n");

    // =======================================================================
    //  PART D -- dispatch ROUTING over DoInArm_9045() (master pick SM)
    // =======================================================================
    printf("[D] DoInArm_9045() dispatch routing\n");

    // ---- D1: e9045_1x1_1 routes to the 1x1_1 arm + advances out of entry ----
    resetInArmBaseline();
    iInArmType = e9045_1x1_1;
    iArmTask   = 1;
    DoInArm_9045();                              // ONE tick from entry
    int d1Task = iArmTask;
    printf("    [D1] after 1 tick from Task=1 @e9045_1x1_1: iArmTask=%d\n", d1Task);
    CHECK(d1Task != 1,
          "D1 e9045_1x1_1: routed to 1x1_1 arm -> cursor advanced out of entry (NOT Program-Error else)");
    // ORACLE CORRECTION (AI(W6.2b-canary) 20260626): the faithful golden 1x1_1
    // master SM falls through 1->10->50->75->100 in ONE tick for this Ambient
    // no-IC baseline -- it does NOT settle at 75.  Chain (golden ainarm9045_1x1_1
    // .cpp / C++ tree): case 50 CheckHeaterOK()==true (offline) ->
    // InitArmPickFromLoadStageTask()+Task=75 (:1871) -> the Hot/OCR/RTC/tray
    // early-exit guards are all skipped (iTemperature==Ambient, trays cleared) ->
    // `if(Task!=75)break;` is false -> FALL-THROUGH to case 75 (:2096); there the
    // Hot+bNeedTrySuckHotPlate branch is false (Ambient) -> else: TryHotClearRec
    // BufMMPlate()+InitArmPickFromLoadStageTask()+Task=100 (:2110) ->
    // `if(Task!=100)break;` false -> FALL-THROUGH to case 100, which settles.
    // The original `==75` oracle overlooked the case-75->100 fall-through (the
    // `//Ifor 20260115 100=>75` note is the Hot clean-out refinement, not this
    // Ambient path).  100 is in the documented sane set, so D5 below still holds.
    CHECK(d1Task == 100,
          "D1 e9045_1x1_1: single-tick 1->10->50->75->100 fall-through (golden 1x1_1 :1871 -> :2096/:2110 else, Ambient baseline)");
    CHECK(inArm1x1CursorSane(d1Task),
          "D1 e9045_1x1_1: cursor in documented set {1,10,50,75,100,...,15000}");

    // ---- D2: e9045_1x4_1_Ac routes to the SAME DoInArm_9045_1x1_1() ---------
    resetInArmBaseline();
    iInArmType = e9045_1x4_1_Ac;
    iArmTask   = 1;
    DoInArm_9045();
    int d2Task = iArmTask;
    printf("    [D2] after 1 tick from Task=1 @e9045_1x4_1_Ac: iArmTask=%d\n", d2Task);
    CHECK(d2Task == d1Task,
          "D2 e9045_1x4_1_Ac: routes to SAME 1x1_1 SM -> identical advance (golden :4563)");
    CHECK(inArm1x1CursorSane(d2Task),
          "D2 e9045_1x4_1_Ac: cursor in documented set");

    // ---- D3: gated type e9045_2x4_8 -> Program-Error else, cursor UNCHANGED --
    resetInArmBaseline();
    iInArmType = e9045_2x4_8;                    // =22, still GATED this wave
    iArmTask   = 1;
    DoInArm_9045();                              // expect Program-Error else (no dispatch)
    printf("    [D3] after 1 tick @e9045_2x4_8 (gated): iArmTask=%d\n", iArmTask);
    CHECK(iArmTask == 1,
          "D3 e9045_2x4_8 (gated): Program-Error else -> iArmTask UNCHANGED (only 1x1_1 un-gated)");

    // ---- D4: gated type e9045_2x8_32 -> Program-Error else, cursor UNCHANGED -
    resetInArmBaseline();
    iInArmType = e9045_2x8_32;                   // =25, routes to 2x8_8 in golden -> GATED here
    iArmTask   = 1;
    DoInArm_9045();
    printf("    [D4] after 1 tick @e9045_2x8_32 (gated): iArmTask=%d\n", iArmTask);
    CHECK(iArmTask == 1,
          "D4 e9045_2x8_32 (gated): Program-Error else -> iArmTask UNCHANGED (golden routes 2x8_32->2x8_8, not translated)");

    // ---- D5: pump the 1x1_1 master SM directly for N ticks (cursor bounded) --
    //   Drive DoInArm_9045_1x1_1() from entry; assert the cursor never escapes
    //   the documented switch(Task) set across a sustained pump (no crash).
    resetInArmBaseline();
    iInArmType = e9045_1x1_1;
    iArmTask   = 1;
    {
        bool sane = true, advanced = false;
        for (int tick = 0; tick < 128; ++tick) {
            DoInArm_9045_1x1_1();
            if (iArmTask != 1) advanced = true;
            if (!inArm1x1CursorSane(iArmTask)) { sane = false; break; }
        }
        CHECK(advanced, "D5 1x1_1 SM: pump advanced out of entry at least once");
        CHECK(sane,     "D5 1x1_1 SM: cursor stays in documented set over 128-tick pump (no escape/crash)");
    }

    // =======================================================================
    //  PART S -- SuckerMap picker-map builder + dispatch
    // =======================================================================
    printf("[S] DoInArm_9045_1x1_1_SuckerMap() builder + dispatch\n");

    // ---- S1: direct 1x1_1 SuckerMap default-branch numeric oracle -----------
    clearProdSuckGrid();
    iInArmType                        = e9045_1x1_1;
    TestIF_File.bSingleUseOtherSuck   = false;   // iSuckCol = 0
    TestIF_File.bSingleInArmUseOtherSuck = false;
    Prod.bSingleUseOtherSuck          = false;
    LastSet.bUseTestSocket[0][0][0]   = true;
    LastSet.bUseTestSocket[1][0][0]   = false;
    TestIF.iSiteMap[0][0]             = 7;
    DoInArm_9045_1x1_1_SuckerMap();
    CHECK(Prod.fInArmSuck4x8[0][0][0] == true,
          "S1 SuckerMap: fInArmSuck4x8[0][0][0] = bUseTestSocket[0][0][0] (true) (golden 1x1_1 :1428)");
    CHECK(Prod.fInArmSuck4x8[1][0][0] == false,
          "S1 SuckerMap: fInArmSuck4x8[1][0][0] = bUseTestSocket[1][0][0] (false) (golden :1429)");
    CHECK(Prod.bInSuckUse[0][0][0] == true && Prod.bInSuckUse[1][0][0] == true,
          "S1 SuckerMap: bInSuckUse[0/1][0][0] = true (golden :1430-1431)");
    CHECK(Prod.iSiteMap[0][0][0] == 7 && Prod.iSiteMap[1][0][0] == 7,
          "S1 SuckerMap: iSiteMap[0/1][0][0] = TestIF.iSiteMap[0][0] (7) (golden :1432-1433)");
    CHECK(GetShuttleCol(0, 0) == 0,
          "S1 SuckerMap: GetShuttleCol(0,0)==0 for e9045_1x1_1 (W7-gated default == golden :507-510)");

    // ---- S2a: DoInArm_9045_SuckerMap() routes e9045_1x1_1 -> builder writes --
    clearProdSuckGrid();
    iInArmType                      = e9045_1x1_1;
    TestIF_File.bSingleUseOtherSuck = false;
    TestIF_File.bSingleInArmUseOtherSuck = false;
    LastSet.bUseTestSocket[0][0][0] = true;
    TestIF.iSiteMap[0][0]           = 7;
    DoInArm_9045_SuckerMap();
    CHECK(Prod.fInArmSuck4x8[0][0][0] == true &&
          Prod.bInSuckUse[0][0][0]    == true &&
          Prod.iSiteMap[0][0][0]      == 7,
          "S2a DoInArm_9045_SuckerMap @e9045_1x1_1: routes to 1x1_1 builder -> Prod grid written");

    // ---- S2b: DoInArm_9045_SuckerMap() with a gated type -> grid stays clear -
    clearProdSuckGrid();
    iInArmType                      = e9045_2x4_8;     // gated -> Program-Error else
    LastSet.bUseTestSocket[0][0][0] = true;
    TestIF.iSiteMap[0][0]           = 7;
    DoInArm_9045_SuckerMap();                          // the engine pre-clears + Program-Error
    CHECK(Prod.bInSuckUse[0][0][0] == false &&
          Prod.iSiteMap[0][0][0]   == -1,
          "S2b DoInArm_9045_SuckerMap @gated type: Program-Error else -> grid stays cleared (no 1x1_1 write)");

    // ---- S3: BONUS bSingleUseOtherSuck=true -> iSuckCol=1 -> writes at [.][0][1]
    clearProdSuckGrid();
    iInArmType                        = e9045_1x1_1;
    TestIF_File.bSingleUseOtherSuck   = true;          // iSuckCol = 1
    TestIF_File.bSingleInArmUseOtherSuck = false;
    Prod.bSingleUseOtherSuck          = false;
    LastSet.bUseTestSocket[0][0][0]   = true;
    TestIF.iSiteMap[0][0]             = 5;
    DoInArm_9045_1x1_1_SuckerMap();
    CHECK(Prod.bInSuckUse[0][0][1] == true && Prod.iSiteMap[0][0][1] == 5,
          "S3 SuckerMap: bSingleUseOtherSuck -> iSuckCol=1 writes at [0][0][1] (golden :1385-1389)");
    CHECK(Prod.bInSuckUse[0][0][0] == false,
          "S3 SuckerMap: column 0 left untouched when iSuckCol=1");
    TestIF_File.bSingleUseOtherSuck = false;           // restore

    // -----------------------------------------------------------------------
    printf("==== W6.2b-canary InArm variant verify: %d passed, %d failed ====\n",
           g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
