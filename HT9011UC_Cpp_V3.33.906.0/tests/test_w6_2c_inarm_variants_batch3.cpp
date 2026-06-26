// =============================================================================
//  test_w6_2c_inarm_variants_batch3.cpp -- W6.2c batch-3 VERIFY: 6 in-arm variants
//
//  Translation wave: W6.2c batch-3 (in-arm per-site-config variant batch)
//  Author: AI(W6.2c-verify-batch3) 20260626
//
//  PURPOSE
//  -------
//  Independently proves the W6.2c batch-3 of 6 in-arm site variants
//    (1x4_4, 1x4_4_Back, 2x3_6, 2x3_6_14, 2x4_4, All_1Pick)
//  offline over the Sim HAL, with NO hardware and NO Borland binary.  Proves:
//
//    (D) the in-arm ENGINE dispatch ladders (DoInArm_9045 / DoInArm_9045_SuckerMap,
//        ainarm9045.cpp) route each of the 5 iInArmType-routed variant values to
//        its OWN translated DoInArm_9045_<v>() / _SuckerMap() arm (un-gated,
//        ACTIVE), via the SOUND differential witness used by batch-2: for an
//        ACTIVE iInArmType the engine ladder simply CALLS DoInArm_9045_<v>(), so
//        DoInArm_9045() from a given entry state must leave iArmTask IDENTICAL to
//        calling DoInArm_9045_<v>() directly from that SAME entry state.  The
//        5 *_SuckerMap symbols + All_1Pick's pair are all defined+callable.
//
//    (D-pick) the LOAD-BEARING ORACLE 3 dispatch fact: with USE_PICKER_COUNT !=
//        ep1Picker, iInArmType==e9045_2x4_4_14 (=20) enters DoInArm_9045_2x4_4()
//        (NOT _2x4_4_13), and iInArmType==e9045_2x4_4_13 (=21) hits the still-
//        GATED Program-Error else (cursor UNCHANGED); and with USE_PICKER_COUNT==
//        ep1Picker the very FIRST ladder arm enters DoInArm_9045_All_1Pick()
//        REGARDLESS of iInArmType -- proven by setting a GATED iInArmType
//        (e9045_2x4_8) + ep1Picker: All_1Pick still runs (cursor advances ==
//        direct All_1Pick call), whereas without ep1Picker that same gated type
//        would hit Program-Error else (cursor UNCHANGED).
//
//    (O1) the pure-const picker->shuttle permutation tables XPHSuckToSht_2x3_6
//         [3][8] / XPHSuckToSht_2x3_6_OutArm[3][8] (golden ainarm9045_2x3_6.cpp:32)
//         carry the exact golden values: the swapped large-pitch right-6-nozzle
//         row1 == {3,1,0,2,...} (in-arm) and {0,2,1,3,...} (out-arm).
//
//    (O2) the pure-const table XPHSuckToSht_2x4_14[3][8] (golden
//         ainarm9045_2x4_4.cpp:32) -- all three rows identical {0,4,5,1,2,6,7,3}.
//
//    (O3) GetShuttleState_2x3_6 pick branch (golden ainarm9045_2x3_6.cpp:3505)
//         returns 0 (all NULL_IC) / 1 (left-top IC) / 2 (left-bottom IC), driving
//         the substrate ArmUpSideAllTypeIC / ArmDownSideAllTypeIC twins over the
//         (NULL_IC, 0, 4) 6-site scan range.
//
//  EQUIVALENCE NOTE (honest): no Borland binary exists, so "equivalence" here ==
//  clean g++ compile/link + the 6 variant funcs defined+dispatched + the pure
//  numeric outputs match values hand-derived from the golden formula/table.
//
//  Prints PASS/FAIL per check + a final summary; returns non-zero on any FAIL.
// =============================================================================
#include "ainarm9045.h"             // DoInArm_9045 / DoInArm_9045_SuckerMap
#include "ainarm9045_2x3_6.h"       // XPHSuckToSht_2x3_6 / _OutArm + GetShuttleState_2x3_6 (ORACLE 1 + 3)
#include "ainarm9045_2x4_4.h"       // XPHSuckToSht_2x4_14 (ORACLE 2)
#include "aHotPlateSubstrate.h"     // TMyKitSuck, InArmSuck, FLCarryKit, ptrInSHT, iArmTask
#include "csystem.h"
#include "Motor/mymotor.h"
#include "cprod.h"                  // Prod, TestIF, PROD_INFO_ST, SYSTEM_TEST_IF
#include "cpublic.h"
#include "cmydef.h"                 // NULL_IC / HAS_IC / bAutoPick / iInArmType / USE_PICKER_COUNT
#include "MachineType.h"            // eInArmType enum values + ePickCount (ep1Picker / ep4Picker)
#include "canary_support.h"         // LastSet, ShowErrorMessage, ...
#include "FormsFacade.h"
#include <cstdio>

// The 6 batch-3 variant master SMs + picker-map builders are file-local in the
// golden (consumed via the engine dispatch ladder), so they are NOT all on the
// per-variant .h surface.  Declare them locally for the direct-dispatch
// assertions -- the engine declares the SAME externs near its ladders
// (ainarm9045.cpp :798-809).
extern void DoInArm_9045_1x4_4();        extern void DoInArm_9045_1x4_4_SuckerMap();
extern void DoInArm_9045_1x4_4_Back();   extern void DoInArm_9045_1x4_4_Back_SuckerMap();
extern void DoInArm_9045_2x3_6();        extern void DoInArm_9045_2x3_6_SuckerMap();
extern void DoInArm_9045_2x3_6_14();     extern void DoInArm_9045_2x3_6_14_SuckerMap();
extern void DoInArm_9045_2x4_4();        extern void DoInArm_9045_2x4_4_SuckerMap();
extern void DoInArm_9045_All_1Pick();    extern void DoInArm_9045_All_1Pick_SuckerMap();

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
// baseline before a dispatch probe (mirror of the batch-1/batch-2 verify reset).
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
    printf("==== W6.2c batch-3 InArm 6-variant verify ====\n");

    // =======================================================================
    //  PART D -- dispatch ROUTING over DoInArm_9045() (main engine ladder) for
    //  the 5 iInArmType-routed variants.  (All_1Pick is picker-count-routed and
    //  proven separately in PART D-pick.)
    //
    //  DIFFERENTIAL witness (sound + deterministic):  for an ACTIVE iInArmType the
    //  engine ladder simply CALLS DoInArm_9045_<v>(), so DoInArm_9045() from a
    //  given entry state must leave iArmTask IDENTICAL to calling
    //  DoInArm_9045_<v>() directly from that SAME entry state.  (Several variants
    //  legitimately wait in golden `case 1:` and do NOT advance the cursor on the
    //  first tick -- so "cursor advanced" is the WRONG witness; "engine path ==
    //  direct path" is the right one.)
    //  USE_PICKER_COUNT must be != ep1Picker here so the leading All_1Pick `if`
    //  is skipped and the iInArmType chain is reached.
    //  (Enum/helper name asymmetry -- e9045_2x4_4_14 routes to DoInArm_9045_2x4_4
    //   -- is golden and preserved verbatim; we route on the golden enum value.)
    // =======================================================================
    printf("[D] DoInArm_9045() engine-ladder routing == direct variant-arm call (5 iInArmType variants)\n");

    USE_PICKER_COUNT = ep4Picker;        // != ep1Picker -> reach the iInArmType chain

    typedef void (*ArmFn)();
    struct { int t; ArmFn fn; const char *name; } variants[] = {
        { e9045_1x4_4_Back, &DoInArm_9045_1x4_4_Back, "e9045_1x4_4_Back -> DoInArm_9045_1x4_4_Back" },
        { e9045_1x4_4,      &DoInArm_9045_1x4_4,      "e9045_1x4_4 -> DoInArm_9045_1x4_4"           },
        { e9045_2x3_6,      &DoInArm_9045_2x3_6,      "e9045_2x3_6 -> DoInArm_9045_2x3_6"           },
        { e9045_2x3_6_14,   &DoInArm_9045_2x3_6_14,   "e9045_2x3_6_14 -> DoInArm_9045_2x3_6_14"     },
        { e9045_2x4_4_14,   &DoInArm_9045_2x4_4,      "e9045_2x4_4_14 -> DoInArm_9045_2x4_4"        },
    };
    for (int v = 0; v < 5; ++v) {
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

    // =======================================================================
    //  PART D-pick -- ORACLE 3, the LOAD-BEARING dispatch fact.
    //
    //  (a) e9045_2x4_4_14 (=20) routes to DoInArm_9045_2x4_4 (proven above in the
    //      D loop -- the engine==direct witness with iInArmType==e9045_2x4_4_14
    //      against the direct DoInArm_9045_2x4_4 fn).  Restated here as an
    //      explicit enum-value witness.
    //  (b) e9045_2x4_4_13 (=21) is STILL GATED -> Program-Error else, cursor
    //      UNCHANGED (the else branch does not touch iArmTask).  This is the
    //      _2x4_4_13 vs _2x4_4 disambiguation the oracle demands.
    //  (c) with USE_PICKER_COUNT==ep1Picker the FIRST ladder arm enters
    //      DoInArm_9045_All_1Pick() REGARDLESS of iInArmType: drive a GATED
    //      iInArmType (e9045_2x4_8) + ep1Picker and assert the cursor advances
    //      EXACTLY as a direct DoInArm_9045_All_1Pick() call would -- whereas the
    //      same gated type WITHOUT ep1Picker leaves the cursor UNCHANGED.
    // =======================================================================
    printf("[D-pick] ORACLE 3: 2x4_4_14 vs gated 2x4_4_13, + ep1Picker -> All_1Pick first arm\n");

    // (a) explicit: e9045_2x4_4_14 enters DoInArm_9045_2x4_4 (engine==direct).
    {
        USE_PICKER_COUNT = ep4Picker;
        resetInArmBaseline();
        iInArmType = e9045_2x4_4_14;
        iArmTask   = 1;
        DoInArm_9045_2x4_4();
        int cDirect = iArmTask;

        resetInArmBaseline();
        iInArmType = e9045_2x4_4_14;
        iArmTask   = 1;
        DoInArm_9045();
        int cEngine = iArmTask;
        CHECK(cEngine == cDirect,
              "D-pick(a) e9045_2x4_4_14(=20) enters DoInArm_9045_2x4_4 (engine==direct, NOT _2x4_4_13)");
    }

    // (b) e9045_2x4_4_13 (=21) is now LIVE (W6.2c batch-5 un-gated the S-family arm
    //     -> DoInArm_9045_2x4_4_13); under !ep1Picker it dispatches via the engine
    //     ladder and advances the cursor out of entry (NOT the Program-Error else).
    {
        USE_PICKER_COUNT = ep4Picker;
        resetInArmBaseline();
        iInArmType = e9045_2x4_4_13;
        iArmTask   = 1;
        DoInArm_9045();
        CHECK(iArmTask != 1,
              "D-pick(b) e9045_2x4_4_13(=21) LIVE -> DoInArm_9045_2x4_4_13 -> cursor advanced (batch-5 un-gated)");
    }

    // (c) ep1Picker -> All_1Pick is the FIRST arm regardless of iInArmType.  The
    //     leading `if(USE_PICKER_COUNT==ep1Picker)` arm short-circuits BEFORE the
    //     iInArmType chain, so even with e9045_2x4_4_13 (now LIVE after batch-5) set,
    //     ep1Picker routes to All_1Pick -- proving picker-count precedence.
    {
        // direct All_1Pick advance from a clean entry state
        USE_PICKER_COUNT = ep1Picker;
        resetInArmBaseline();
        iInArmType = e9045_2x4_4_13;     // LIVE type, but ep1Picker arm fires first
        iArmTask   = 1;
        DoInArm_9045_All_1Pick();
        int cAllDirect = iArmTask;

        // engine ladder with ep1Picker + the SAME gated type
        USE_PICKER_COUNT = ep1Picker;
        resetInArmBaseline();
        iInArmType = e9045_2x4_4_13;
        iArmTask   = 1;
        DoInArm_9045();
        int cEngineP1 = iArmTask;
        CHECK(cEngineP1 == cAllDirect && cAllDirect != 1,
              "D-pick(c) ep1Picker: FIRST arm == DoInArm_9045_All_1Pick (cursor advances, ignores gated iInArmType)");

        // control: same type WITHOUT ep1Picker -> now routes to its own LIVE
        // S-family arm (DoInArm_9045_2x4_4_13), cursor advances out of entry.
        // (Pre-batch-5 this hit the Program-Error else and stayed UNCHANGED.)
        USE_PICKER_COUNT = ep4Picker;
        resetInArmBaseline();
        iInArmType = e9045_2x4_4_13;
        iArmTask   = 1;
        DoInArm_9045();
        CHECK(iArmTask != 1,
              "D-pick(c-ctrl) NO ep1Picker + e9045_2x4_4_13 LIVE -> DoInArm_9045_2x4_4_13 -> cursor advanced");
    }

    // e9045_2x4_4_13 (=21) is LIVE after W6.2c batch-5 (-> DoInArm_9045_2x4_4_13);
    // under the normal !ep1Picker path it dispatches via the engine ladder and
    // advances the cursor out of entry -- restating the LIVE-route witness for
    // completeness.  (Was a still-GATED UNCHANGED assertion pre-batch-5.)
    USE_PICKER_COUNT = ep4Picker;
    resetInArmBaseline();
    iInArmType = e9045_2x4_4_13;
    iArmTask   = 1;
    DoInArm_9045();
    CHECK(iArmTask != 1,
          "D-route [e9045_2x4_4_13 LIVE]: routed to DoInArm_9045_2x4_4_13 -> cursor advanced (batch-5 un-gated)");

    // The 6 *_SuckerMap() builder symbols are real, defined, callable (no crash).
    // All_1Pick_SuckerMap is DEFINED (faithful) though the golden engine never
    // dispatches it -- call it directly to prove the symbol resolves.
    DoInArm_9045_1x4_4_SuckerMap();
    DoInArm_9045_1x4_4_Back_SuckerMap();
    DoInArm_9045_2x3_6_SuckerMap();
    DoInArm_9045_2x3_6_14_SuckerMap();
    DoInArm_9045_2x4_4_SuckerMap();
    DoInArm_9045_All_1Pick_SuckerMap();
    CHECK(true, "D-suckermap: all 6 *_SuckerMap() builder symbols are defined + callable (no crash)");

    // =======================================================================
    //  PART O1 -- ORACLE 1: pure-const picker-column -> shuttle-column maps for
    //  the 2x3_6 6-site layout.  Side-effect-free geometry; values from golden
    //  ainarm9045_2x3_6.cpp:32.  Exported via the variant .h externs.
    //  row1 (large-pitch right-6-nozzle) is the swapped row.
    // =======================================================================
    printf("[O1] XPHSuckToSht_2x3_6 / _OutArm tables (golden ainarm9045_2x3_6.cpp:32)\n");
    {
        // in-arm row1 == {3,1,0,2,4,5,6,7}: col0==3 and col2==0 (the swap)
        CHECK(XPHSuckToSht_2x3_6[1][0] == 3 && XPHSuckToSht_2x3_6[1][2] == 0,
              "O1 XPHSuckToSht_2x3_6 row1 swap: [1][0]==3, [1][2]==0 (large-pitch right-6-nozzle)");
        // rows 0 and 2 are identity
        bool idOk = (XPHSuckToSht_2x3_6[0][0] == 0 && XPHSuckToSht_2x3_6[0][7] == 7 &&
                     XPHSuckToSht_2x3_6[2][0] == 0 && XPHSuckToSht_2x3_6[2][7] == 7);
        CHECK(idOk, "O1 XPHSuckToSht_2x3_6 rows 0 & 2 == identity");
        // out-arm row1 == {0,2,1,3,4,5,6,7}: col1==2 and col2==1 (the swap)
        CHECK(XPHSuckToSht_2x3_6_OutArm[1][1] == 2 && XPHSuckToSht_2x3_6_OutArm[1][2] == 1,
              "O1 XPHSuckToSht_2x3_6_OutArm row1 swap: [1][1]==2, [1][2]==1");
    }

    // =======================================================================
    //  PART O2 -- ORACLE 2: pure-const table XPHSuckToSht_2x4_14[3][8].
    //  golden ainarm9045_2x4_4.cpp:32 ; all three rows identical {0,4,5,1,2,6,7,3}.
    // =======================================================================
    printf("[O2] XPHSuckToSht_2x4_14 const table (golden ainarm9045_2x4_4.cpp:32)\n");
    {
        const int row[8] = {0, 4, 5, 1, 2, 6, 7, 3};
        bool allOk = true;
        for (int r = 0; r < 3; ++r)
            for (int j = 0; j < 8; ++j)
                if (XPHSuckToSht_2x4_14[r][j] != row[j]) allOk = false;
        CHECK(allOk, "O2 XPHSuckToSht_2x4_14[3][8] all rows == {0,4,5,1,2,6,7,3}");
        // explicit spot anchors from the oracle brief
        CHECK(XPHSuckToSht_2x4_14[0][1] == 4 &&
              XPHSuckToSht_2x4_14[0][3] == 1 &&
              XPHSuckToSht_2x4_14[0][7] == 3,
              "O2 anchors: [0][1]==4, [0][3]==1, [0][7]==3");
    }

    // =======================================================================
    //  PART O3 -- GetShuttleState_2x3_6 pick branch (verbatim golden).
    //  bPick==bAutoPick -> pick branch; ptrInSHT defaults to &FLCarryKit.
    //  Drives substrate ArmUpSideAllTypeIC / ArmDownSideAllTypeIC twins over the
    //  (NULL_IC, 0, 4) 6-site scan range.  golden ainarm9045_2x3_6.cpp:3505.
    // =======================================================================
    printf("[O3] GetShuttleState_2x3_6 pick branch (golden ainarm9045_2x3_6.cpp:3505)\n");
    {
        // ptrInSHT -> FLCarryKit (substrate default).  All NULL_IC:
        // ArmUpSideAllTypeIC(NULL_IC,0,4)==true && ArmDownSideAllTypeIC==true -> return 0.
        clearKitItem(FLCarryKit, 2);
        InArmSuck.SetAllToNullIC();
        int s0 = GetShuttleState_2x3_6(0, bAutoPick);
        CHECK(s0 == 0, "O3 all-NULL pick branch: GetShuttleState_2x3_6(0,bAutoPick)==0 (no IC)");

        // Item[0][0]=HAS_IC -> up-side scan Item[0][0..3] sees a non-NULL -> false ->
        // ArmUpSideAllTypeIC==false -> return 1 (left-top has IC).
        clearKitItem(FLCarryKit, 2);
        FLCarryKit.Item[0][0] = HAS_IC;
        int s1 = GetShuttleState_2x3_6(0, bAutoPick);
        CHECK(s1 == 1, "O3 Item[0][0]=HAS_IC pick branch: GetShuttleState_2x3_6(0,bAutoPick)==1 (left-top IC)");

        // Item[1][2]=HAS_IC -> up-side all NULL (true), down-side Item[1][0..3] sees
        // non-NULL at [1][2] -> false -> ArmDownSideAllTypeIC==false -> return 2.
        clearKitItem(FLCarryKit, 2);
        FLCarryKit.Item[1][2] = HAS_IC;
        int s2 = GetShuttleState_2x3_6(0, bAutoPick);
        CHECK(s2 == 2, "O3 Item[1][2]=HAS_IC pick branch: GetShuttleState_2x3_6(0,bAutoPick)==2 (left-bottom IC)");
        clearKitItem(FLCarryKit, 1);   // restore clean baseline
    }

    // -----------------------------------------------------------------------
    printf("==== W6.2c batch-3 InArm 6-variant verify: %d passed, %d failed ====\n",
           g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
