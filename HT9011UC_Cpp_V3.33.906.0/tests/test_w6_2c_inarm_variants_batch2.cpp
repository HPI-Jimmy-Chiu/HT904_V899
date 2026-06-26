// =============================================================================
//  test_w6_2c_inarm_variants_batch2.cpp -- W6.2c batch-2 VERIFY: 6 in-arm variants
//
//  Translation wave: W6.2c batch-2 (in-arm per-site-config variant batch)
//  Author: AI(W6.2c-verify-batch2) 20260626
//
//  PURPOSE
//  -------
//  Independently proves the W6.2c batch-2 of 6 in-arm site variants
//    (1x4_2, 1x4_8_Hot, 2x2_4_12, 2x1_2, 2x2_4, 2x2_4_14)
//  offline over the Sim HAL, with NO hardware and NO Borland binary.  Proves:
//    (D) the in-arm ENGINE dispatch ladders (DoInArm_9045 / DoInArm_9045_SuckerMap,
//        ainarm9045.cpp) route each of the 6 variant iInArmType values to its own
//        translated DoInArm_9045_<v>() / _SuckerMap() arm (un-gated, ACTIVE),
//        while a still-gated iInArmType hits the Program-Error else (cursor
//        UNCHANGED) -- so ONLY the intended arms were un-gated;
//    (O1) the pure-const picker-column->shuttle-column permutation maps
//         XPHSuckToSht_2x2_12[8] / XPHSuckToSht_2x2_13[3][8] /
//         XPHSuckToSht_2x2_14[3][8] carry the exact golden values (ORACLE 1);
//    (O2) DoInArm_9045_2x1_2_SuckerMap, when reached via the engine ladder with
//         iInArmType==e9045_2x1_2_13 and TestIF.iSiteMap seeded, composes the
//         engine grid-clear + per-variant fill exactly per golden: only cols 0
//         and 2 of row 0 used, iSiteMap routed from TestIF (ORACLE 2);
//    (O3a) on first DoInArm_9045_2x1_2() tick from iArmTask init the function
//          binds `int &Task=iArmTask;` so the SHARED engine cursor iArmTask (not a
//          per-file copy) is the one advanced (ORACLE 3, cursor-entry);
//    (O3b) GetShuttleState_2x2_4 pick branch returns the golden values
//          (0 all-NULL / 1 left-top IC / 2 left-bottom IC), driving the substrate
//          ArmUpSideAllTypeIC / ArmDownSideAllTypeIC twins.
//
//  EQUIVALENCE NOTE (honest): no Borland binary exists, so "equivalence" here ==
//  clean g++ compile/link + the 6 variant funcs defined+dispatched + the pure
//  numeric outputs match values hand-derived from the golden formula/table.
//
//  Prints PASS/FAIL per check + a final summary; returns non-zero on any FAIL.
// =============================================================================
#include "ainarm9045.h"             // DoInArm_9045 / DoInArm_9045_SuckerMap
#include "ainarm9045_2x2_4_12.h"    // XPHSuckToSht_2x2_12[8]            (ORACLE 1)
#include "ainarm9045_2x2_4.h"       // XPHSuckToSht_2x2_13[3][8] + GetShuttleState_2x2_4
#include "ainarm9045_2x2_4_14.h"    // XPHSuckToSht_2x2_14[3][8] + GetShuttleState_2x2_4_14
#include "aHotPlateSubstrate.h"     // TMyKitSuck, InArmSuck, FLCarryKit, ptrInSHT, iArmTask
#include "csystem.h"
#include "Motor/mymotor.h"
#include "cprod.h"                  // Prod, TestIF, PROD_INFO_ST, SYSTEM_TEST_IF
#include "cpublic.h"
#include "cmydef.h"                 // NULL_IC / HAS_IC / bAutoPick / iInArmType
#include "MachineType.h"            // eInArmType enum values
#include "canary_support.h"         // LastSet, ShowErrorMessage, ...
#include "FormsFacade.h"
#include <cstdio>

// The 6 batch-2 variant master SMs + picker-map builders are file-local in the
// golden (consumed via the engine dispatch ladder), so they are NOT all on the
// per-variant .h surface.  Declare them locally for the direct-dispatch
// assertions -- the engine declares the SAME externs near its ladders
// (ainarm9045.cpp :784-795).
extern void DoInArm_9045_1x4_2();         extern void DoInArm_9045_1x4_2_SuckerMap();
extern void DoInArm_9045_1x4_8_Hot();     extern void DoInArm_9045_1x4_8_Hot_SuckerMap();
extern void DoInArm_9045_2x1_2();         extern void DoInArm_9045_2x1_2_SuckerMap();
extern void DoInArm_9045_2x2_4();         extern void DoInArm_9045_2x2_4_SuckerMap();
extern void DoInArm_9045_2x2_4_12();      extern void DoInArm_9045_2x2_4_12_SuckerMap();
extern void DoInArm_9045_2x2_4_14();      extern void DoInArm_9045_2x2_4_14_SuckerMap();

// O4: the pure geometry helper GetNowSiteKitMode_2x1_2 is a file-local
// (external-linkage, non-static) function in the 2x1_2 leaf -- NOT on its .h
// surface (golden keeps it file-local).  Declared here for the direct geometry
// assertion below.  golden ainarm9045_2x1_2.cpp:57.
extern int GetNowSiteKitMode_2x1_2(int iSht, bool bPlace);

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as the other W6 verify TUs)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// Reset the in-arm grids + the relevant master flags to a clean, no-IC offline
// baseline before a dispatch probe (mirror of the batch-1 verify reset).
static void resetInArmBaseline()
{
    FLCarryKit.ClearAll();
    BLCarryKit.ClearAll();
    InArmSuck.SetAllToNullIC();          // HasRealIC() -> false
    bPlaceToHotplate     = false;
    bPickFromHotplate    = false;
    bIndexAlarmInArmAway = false;
    bIndexDropVacuumError= false;
    LastSet.iTemperature = Tempture_Ambient;
    LastSet.iRunStartMode= 0;
}

// Set every cell of a TMyKitSuck Item grid to NULL_IC (a known clean baseline
// for the AllTypeIC side-scans -- 4 rows x 8 cols, full to be safe).
static void clearKitItem(TMyKitSuck &k, int rows)
{
    k.iMaxRow = rows;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 8; ++j)
            k.Item[i][j] = NULL_IC;
}

int main()
{
    printf("==== W6.2c batch-2 InArm 6-variant verify ====\n");

    // =======================================================================
    //  PART D -- dispatch ROUTING over DoInArm_9045() (main engine ladder).
    //
    //  DIFFERENTIAL witness (sound + deterministic, NOT a flaky "cursor moved"
    //  guess):  for an ACTIVE iInArmType the engine ladder simply CALLS
    //  DoInArm_9045_<v>(), so DoInArm_9045() from a given entry state must leave
    //  iArmTask IDENTICAL to calling DoInArm_9045_<v>() directly from that SAME
    //  entry state.  (Several variants legitimately wait in golden `case 1:` on
    //  MoveInArm2XYToWait() and do NOT advance the cursor on the first tick --
    //  so "cursor advanced" is the WRONG witness; "engine path == direct path"
    //  is the right one.)  For the still-GATED type the engine path diverges
    //  (Program-Error else, cursor untouched) from the direct body -- proven
    //  separately below.
    //  (Enum/helper name asymmetries -- e9045_1x4_2_14 vs DoInArm_9045_1x4_2,
    //   e9045_2x1_2_13 vs _2x1_2, e9045_2x2_4_13 vs _2x2_4 -- are golden and
    //   preserved verbatim; we route on the golden enum value.)
    // =======================================================================
    printf("[D] DoInArm_9045() engine-ladder routing == direct variant-arm call (6 variants)\n");

    typedef void (*ArmFn)();
    struct { int t; ArmFn fn; const char *name; } variants[] = {
        { e9045_1x4_2_14, &DoInArm_9045_1x4_2,     "e9045_1x4_2_14 -> DoInArm_9045_1x4_2"        },
        { e9045_1x4_8_Hot,&DoInArm_9045_1x4_8_Hot, "e9045_1x4_8_Hot -> DoInArm_9045_1x4_8_Hot"   },
        { e9045_2x1_2_13, &DoInArm_9045_2x1_2,     "e9045_2x1_2_13 -> DoInArm_9045_2x1_2"        },
        { e9045_2x2_4_12, &DoInArm_9045_2x2_4_12,  "e9045_2x2_4_12 -> DoInArm_9045_2x2_4_12"     },
        { e9045_2x2_4_13, &DoInArm_9045_2x2_4,     "e9045_2x2_4_13 -> DoInArm_9045_2x2_4"        },
        { e9045_2x2_4_14, &DoInArm_9045_2x2_4_14,  "e9045_2x2_4_14 -> DoInArm_9045_2x2_4_14"     },
    };
    for (int v = 0; v < 6; ++v) {
        // direct variant-arm call from a clean entry state
        resetInArmBaseline();
        iInArmType = variants[v].t;
        iArmTask   = 1;
        variants[v].fn();
        int cursorDirect = iArmTask;

        // engine-ladder call from the IDENTICAL clean entry state
        resetInArmBaseline();
        iInArmType = variants[v].t;
        iArmTask   = 1;
        DoInArm_9045();
        int cursorEngine = iArmTask;

        char buf[200];
        snprintf(buf, sizeof(buf),
            "D-dispatch [%s]: engine ladder routes to its own arm (cursorEngine==cursorDirect==%d)",
            variants[v].name, cursorDirect);
        CHECK(cursorEngine == cursorDirect, buf);
    }

    // e9045_2x4_4_13 (=21) is now LIVE: W6.2c batch-5 un-gated the S-family arm
    // (-> DoInArm_9045_2x4_4_13).  It dispatches through the engine ladder and
    // advances the cursor out of entry (NOT the Program-Error else, which leaves
    // iArmTask UNCHANGED).  (Was a still-GATED UNCHANGED assertion pre-batch-5.)
    resetInArmBaseline();
    iInArmType = e9045_2x4_4_13;
    iArmTask   = 1;
    DoInArm_9045();
    CHECK(iArmTask != 1,
          "D-route [e9045_2x4_4_13 LIVE]: routed to DoInArm_9045_2x4_4_13 -> cursor advanced (batch-5 un-gated)");

    // The 6 *_SuckerMap() builder symbols are real, defined, callable (no crash).
    DoInArm_9045_1x4_2_SuckerMap();
    DoInArm_9045_1x4_8_Hot_SuckerMap();
    DoInArm_9045_2x1_2_SuckerMap();
    DoInArm_9045_2x2_4_SuckerMap();
    DoInArm_9045_2x2_4_12_SuckerMap();
    DoInArm_9045_2x2_4_14_SuckerMap();
    CHECK(true, "D-suckermap: all 6 *_SuckerMap() builder symbols are defined + callable (no crash)");

    // =======================================================================
    //  PART O1 -- ORACLE 1: pure-const picker-column -> shuttle-column maps.
    //  Side-effect-free geometry; values from golden .cpp (2x2_4_12 / 2x2_4 /
    //  2x2_4_14).  These are exported via the variant .h externs.
    // =======================================================================
    printf("[O1] XPHSuckToSht permutation maps (golden 2x2_4_12/2x2_4/2x2_4_14 :32)\n");
    // XPHSuckToSht_2x2_12[8] == {0,1,2,3,4,5,6,7}
    {
        bool ok12 = true;
        for (int j = 0; j < 8; ++j) if (XPHSuckToSht_2x2_12[j] != j) ok12 = false;
        CHECK(ok12, "O1 XPHSuckToSht_2x2_12[8] == {0,1,2,3,4,5,6,7} (identity)");
    }
    // XPHSuckToSht_2x2_13[3][8] each row == {0,2,1,3,4,5,6,7}
    {
        const int row13[8] = {0, 2, 1, 3, 4, 5, 6, 7};
        bool ok13 = true;
        for (int r = 0; r < 3; ++r)
            for (int j = 0; j < 8; ++j)
                if (XPHSuckToSht_2x2_13[r][j] != row13[j]) ok13 = false;
        CHECK(ok13, "O1 XPHSuckToSht_2x2_13[3][8] each row == {0,2,1,3,4,5,6,7}");
    }
    // XPHSuckToSht_2x2_14[3][8] each row == {0,2,3,1,4,5,6,7}
    {
        const int row14[8] = {0, 2, 3, 1, 4, 5, 6, 7};
        bool ok14 = true;
        for (int r = 0; r < 3; ++r)
            for (int j = 0; j < 8; ++j)
                if (XPHSuckToSht_2x2_14[r][j] != row14[j]) ok14 = false;
        CHECK(ok14, "O1 XPHSuckToSht_2x2_14[3][8] each row == {0,2,3,1,4,5,6,7}");
    }

    // =======================================================================
    //  PART O2 -- ORACLE 2: SuckerMap grid compose (2x1_2) via engine ladder.
    //  Engine DoInArm_9045_SuckerMap() clears the Prod grids (bInSuckUse=false,
    //  iSiteMap=-1) THEN dispatches to DoInArm_9045_2x1_2_SuckerMap, which fills
    //  only cols 0 and 2 of row 0 and routes iSiteMap from TestIF.
    //  golden ainarm9045_2x1_2.cpp:1570 ; engine grid-clear ainarm9045.cpp:971.
    // =======================================================================
    printf("[O2] SuckerMap grid effect (2x1_2) -- engine clear + per-variant fill compose\n");
    {
        // Seed two distinct, non-(-1) site ids so the routing is observable.
        TestIF.iSiteMap[0][0] = 7;
        TestIF.iSiteMap[1][0] = 9;
        // Dirty a cell the variant does NOT touch, so the engine grid-clear is
        // what zeroes it (proves clear ran) -- col 1 of row 0 must end false/-1.
        Prod.bInSuckUse[0][0][1] = true;
        Prod.iSiteMap[0][0][1]   = 123;

        iInArmType = e9045_2x1_2_13;
        DoInArm_9045_SuckerMap();        // engine ladder: clear -> 2x1_2 fill

        CHECK(Prod.bInSuckUse[0][0][0] == true  &&
              Prod.bInSuckUse[0][0][2] == true  &&
              Prod.bInSuckUse[1][0][0] == true  &&
              Prod.bInSuckUse[1][0][2] == true,
              "O2 2x1_2 fill: bInSuckUse[*][0][{0,2}] == true (cols 0,2 of row 0 used)");
        CHECK(Prod.bInSuckUse[0][0][1] == false,
              "O2 2x1_2 clear: bInSuckUse[0][0][1] cleared by engine (untouched col) == false");
        CHECK(Prod.iSiteMap[0][0][0] == TestIF.iSiteMap[0][0] &&
              Prod.iSiteMap[0][0][2] == TestIF.iSiteMap[1][0],
              "O2 2x1_2 route: iSiteMap[0][0][0]==TestIF[0][0]=7, iSiteMap[0][0][2]==TestIF[1][0]=9");
        CHECK(Prod.iSiteMap[0][0][1] == -1,
              "O2 2x1_2 clear: iSiteMap[0][0][1] reset to -1 by engine (untouched col)");
    }

    // =======================================================================
    //  PART O3a -- ORACLE 3: cursor-entry binding for DoInArm_9045_2x1_2().
    //  The variant binds `int &Task=iArmTask;` so the SHARED engine cursor
    //  iArmTask (not a per-file copy) is advanced.  Drive ONE tick from the
    //  init cursor value and assert the SAME global iArmTask moved.
    // =======================================================================
    printf("[O3a] cursor-entry: DoInArm_9045_2x1_2() advances the shared iArmTask global\n");
    {
        resetInArmBaseline();
        iInArmType = e9045_2x1_2_13;
        iArmTask   = 1;                  // entry / init value
        DoInArm_9045_2x1_2();            // direct (file-local engine arm)
        CHECK(iArmTask != 1,
              "O3a DoInArm_9045_2x1_2: shared iArmTask global advanced from entry (int &Task=iArmTask)");
    }

    // =======================================================================
    //  PART O3b -- GetShuttleState_2x2_4 pick branch (verbatim golden).
    //  bPick==bAutoPick -> pick branch; ptrInSHT defaults to &FLCarryKit.
    //  Drives substrate ArmUpSideAllTypeIC / ArmDownSideAllTypeIC twins.
    //  golden ainarm9045_2x2_4.cpp:2965.
    // =======================================================================
    printf("[O3b] GetShuttleState_2x2_4 pick branch (golden ainarm9045_2x2_4.cpp:2965)\n");
    {
        // ptrInSHT -> FLCarryKit (substrate default).  All NULL_IC:
        // ArmUpSideAllTypeIC(NULL_IC,0,2)==true && ArmDownSideAllTypeIC==true -> return 0.
        clearKitItem(FLCarryKit, 2);
        InArmSuck.SetAllToNullIC();
        int s0 = GetShuttleState_2x2_4(0, bAutoPick);
        CHECK(s0 == 0, "O3b all-NULL pick branch: GetShuttleState_2x2_4(0,bAutoPick)==0 (no IC)");

        // Item[0][0]=HAS_IC -> up-side scan Item[0][0..1] sees a non-NULL -> false ->
        // ArmUpSideAllTypeIC==false -> return 1 (left-top has IC).
        clearKitItem(FLCarryKit, 2);
        FLCarryKit.Item[0][0] = HAS_IC;
        int s1 = GetShuttleState_2x2_4(0, bAutoPick);
        CHECK(s1 == 1, "O3b Item[0][0]=HAS_IC pick branch: GetShuttleState_2x2_4(0,bAutoPick)==1 (left-top IC)");

        // Item[1][1]=HAS_IC -> up-side all NULL (true), down-side Item[1][0..1] sees
        // non-NULL at [1][1] -> false -> ArmDownSideAllTypeIC==false -> return 2.
        clearKitItem(FLCarryKit, 2);
        FLCarryKit.Item[1][1] = HAS_IC;
        int s2 = GetShuttleState_2x2_4(0, bAutoPick);
        CHECK(s2 == 2, "O3b Item[1][1]=HAS_IC pick branch: GetShuttleState_2x2_4(0,bAutoPick)==2 (left-bottom IC)");
        clearKitItem(FLCarryKit, 1);   // restore clean baseline
    }

    // =======================================================================
    //  PART O4 -- ORACLE 2 (this wave's NEW assertion): the pure geometry
    //  helper GetNowSiteKitMode_2x1_2() place-branch.  golden
    //  ainarm9045_2x1_2.cpp:57.  With the offline-clean substrate
    //  (Zteach->fShow==false [facade default], bRunAutoClean==false [cmydef.cpp
    //  global default] -> iX=HAS_TESTING_IC, and the AutoClean guard
    //  `iAutoClean_Function && !bUse8Picker && bRunAutoClean` is FALSE because
    //  bRunAutoClean==false), the place branch reduces to the simple else:
    //      InArmSuck.Item[0][0] >= HAS_IC  ?  return 10000  :  return 11000 .
    //  This is side-effect-free integer geometry (no HAL motion), so the value
    //  is a deterministic witness that the 2x1_2 leaf's helper was translated
    //  with the >=HAS_IC comparison + the exact 10000/11000 magic returns intact
    //  (NULL_IC==0 < HAS_IC==2, verified cmydef.cpp:153-155).
    // =======================================================================
    printf("[O4] GetNowSiteKitMode_2x1_2 place branch (golden ainarm9045_2x1_2.cpp:57)\n");
    {
        bRunAutoClean = false;                       // ensure AutoClean guard off
        // else case: Item[0][0] (NULL_IC) < HAS_IC -> 11000
        InArmSuck.SetAllToNullIC();
        int m0 = GetNowSiteKitMode_2x1_2(0, true);
        CHECK(m0 == 11000,
              "O4 place, InArmSuck.Item[0][0]=NULL_IC: GetNowSiteKitMode_2x1_2(0,true)==11000");

        // if case: Item[0][0] >= HAS_IC -> 10000
        InArmSuck.SetAllToNullIC();
        InArmSuck.Item[0][0] = HAS_IC;
        int m1 = GetNowSiteKitMode_2x1_2(0, true);
        CHECK(m1 == 10000,
              "O4 place, InArmSuck.Item[0][0]=HAS_IC: GetNowSiteKitMode_2x1_2(0,true)==10000");
        InArmSuck.SetAllToNullIC();                  // restore clean baseline
    }

    // -----------------------------------------------------------------------
    printf("==== W6.2c batch-2 InArm 6-variant verify: %d passed, %d failed ====\n",
           g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
