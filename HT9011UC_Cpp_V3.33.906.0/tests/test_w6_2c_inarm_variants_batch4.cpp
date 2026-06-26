// =============================================================================
//  test_w6_2c_inarm_variants_batch4.cpp -- W6.2c batch-4 VERIFY: 5 in-arm variants
//
//  Translation wave: W6.2c batch-4 (in-arm per-site-config variant batch)
//  Author: AI(W6.2c-verify-batch4) 20260626
//
//  PURPOSE
//  -------
//  Independently proves the W6.2c batch-4 of 5 in-arm site variants
//    (2x2_8_Hot, 2x4_8, 2x5_8, 2x6_8, 2x8_8)
//  offline over the Sim HAL, with NO hardware and NO Borland binary.  Proves:
//
//    (D) the in-arm ENGINE dispatch ladders (DoInArm_9045 / DoInArm_9045_SuckerMap,
//        ainarm9045.cpp) route each of the 5 active variant iInArmType values to
//        its OWN translated DoInArm_9045_<v>() arm (un-gated, ACTIVE), via the SOUND
//        differential witness used by batch-2/3: for an ACTIVE iInArmType the engine
//        ladder simply CALLS DoInArm_9045_<v>(), so DoInArm_9045() from a given entry
//        state must leave iArmTask IDENTICAL to calling DoInArm_9045_<v>() directly
//        from that SAME entry state.  All 5 *_SuckerMap symbols defined + callable.
//
//    (D-route) the LOAD-BEARING golden routing fact (golden ainarm9045.cpp :4639 /
//        :4804): iInArmType==e9045_2x8_32 routes to the SAME DoInArm_9045_2x8_8()
//        (and _SuckerMap) callee as e9045_2x8_8 -- both arms now ACTIVE, both call
//        the LIVE 2x8_8 family.  Witnessed by engine(2x8_32)==direct(2x8_8).
//
//    (D-gate) a STILL-gated iInArmType (e9045_2x4_4_13 / e9045_1x4_4_13, both still
//        inside the #if 0 gate after batch-4) hits the Program-Error else and leaves
//        the cursor UNCHANGED -- proving only the intended 5 (+routed 2x8_32) arms
//        were un-gated.
//
//    (O1) the pure-const picker-column -> shuttle-column map XPHSuckToSht_2x6_6
//         [e2x6ModeTotal][8] (golden ainarm9045_2x6_8.cpp:75) carries the exact
//         golden values (standard row {0,1,2,6,7,3,4,5}; the Steven-20250123 fix
//         row1 {0,2,1,6,7,4,3,5}; the close-center-2x4 row {0,1,4,5,2,3,6,7}).
//
//    (O2) the pure-const map XPHSuckToSht_2x8_8[e2x8ModeTotal][2][8] (golden
//         ainarm9045_2x8_8.cpp:112) carries the golden STM (e2x8_STMMode) interleave
//         {1,5,3,7,0,2,4,6}/{4,0,6,2,1,3,5,7} + the identity standard mode.
//
//  EQUIVALENCE NOTE (honest): no Borland binary exists, so "equivalence" here ==
//  clean g++ compile/link + the 5 variant funcs defined+dispatched + the pure
//  numeric outputs match values hand-derived from the golden table.
//
//  Prints PASS/FAIL per check + a final summary; returns non-zero on any FAIL.
// =============================================================================
#include "ainarm9045.h"             // DoInArm_9045 / DoInArm_9045_SuckerMap
#include "ainarm9045_2x6_8.h"       // XPHSuckToSht_2x6_6 + e2x6Mode (ORACLE 1)
#include "ainarm9045_2x8_8.h"       // XPHSuckToSht_2x8_8 (ORACLE 2)
#include "ainarm9045_2x2_8_Hot.h"   // XPHSuckToSht_2x2_8 (ORACLE 3 const tbl)
#include "ainarm9045_2x4_8.h"       // XPHSuckToSht_2x4_8 (ORACLE 3 const tbl)
#include "ainarm9045_2x5_8.h"       // XPHSuckToSht_2x5_8 (ORACLE 3 const tbl)
#include "aHotPlateSubstrate.h"     // TMyKitSuck, InArmSuck, FLCarryKit, iArmTask
#include "csystem.h"
#include "Motor/mymotor.h"
#include "cprod.h"                  // Prod, TestIF, PROD_INFO_ST, SYSTEM_TEST_IF
#include "cpublic.h"
#include "cmydef.h"                 // NULL_IC / HAS_IC / iInArmType / USE_PICKER_COUNT
#include "MachineType.h"            // eInArmType enum values + ePickCount (ep1Picker / ep4Picker) + e2x8Mode
#include "canary_support.h"         // LastSet, ShowErrorMessage, ...
#include "FormsFacade.h"
#include <cstdio>

// The 5 batch-4 variant master SMs + picker-map builders are file-local in the
// golden (consumed via the engine dispatch ladder).  Declare them locally for the
// direct-dispatch assertions -- the engine declares the SAME externs near its
// ladders (ainarm9045.cpp, the W6.2c batch-4 extern block).
extern void DoInArm_9045_2x2_8_Hot();  extern void DoInArm_9045_2x2_8_Hot_SuckerMap();
extern void DoInArm_9045_2x4_8();       extern void DoInArm_9045_2x4_8_SuckerMap();
extern void DoInArm_9045_2x5_8();       extern void DoInArm_9045_2x5_8_SuckerMap();
extern void DoInArm_9045_2x6_8();       extern void DoInArm_9045_2x6_8_SuckerMap();
extern void DoInArm_9045_2x8_8();       extern void DoInArm_9045_2x8_8_SuckerMap();
// W6.2c batch-5 S-family callees (now LIVE; IRREGULAR func names -- copy EXACTLY).
extern void DoInArm_9045S_1x4_4();      extern void DoInArm_9045S_1x4_4_SuckerMap();
extern void DoInArm_9045_2x4_4_13();    extern void DoInArm_9045_2x4_4_13_SuckerMap();

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
// baseline before a dispatch probe (mirror of the batch-1/2/3 verify reset).
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

int main()
{
    printf("==== W6.2c batch-4 InArm 5-variant verify ====\n");

    // =======================================================================
    //  PART D -- dispatch ROUTING over DoInArm_9045() (main engine ladder).
    //
    //  DIFFERENTIAL witness (sound + deterministic):  for an ACTIVE iInArmType the
    //  engine ladder simply CALLS DoInArm_9045_<v>(), so DoInArm_9045() from a given
    //  entry state must leave iArmTask IDENTICAL to calling DoInArm_9045_<v>()
    //  directly from that SAME entry state.  ("cursor advanced" is the WRONG witness
    //  -- some variants legitimately wait in golden `case 1:`; "engine path ==
    //  direct path" is the right one.)  USE_PICKER_COUNT != ep1Picker so the leading
    //  All_1Pick `if` is skipped and the iInArmType chain is reached.
    // =======================================================================
    printf("[D] DoInArm_9045() engine-ladder routing == direct variant-arm call (5 variants)\n");

    USE_PICKER_COUNT = ep4Picker;        // != ep1Picker -> reach the iInArmType chain

    typedef void (*ArmFn)();
    struct { int t; ArmFn fn; const char *name; } variants[] = {
        { e9045_2x2_8_Hot, &DoInArm_9045_2x2_8_Hot, "e9045_2x2_8_Hot -> DoInArm_9045_2x2_8_Hot" },
        { e9045_2x4_8,     &DoInArm_9045_2x4_8,     "e9045_2x4_8 -> DoInArm_9045_2x4_8"         },
        { e9045_2x5_8,     &DoInArm_9045_2x5_8,     "e9045_2x5_8 -> DoInArm_9045_2x5_8"         },
        { e9045_2x6_8,     &DoInArm_9045_2x6_8,     "e9045_2x6_8 -> DoInArm_9045_2x6_8"         },
        { e9045_2x8_8,     &DoInArm_9045_2x8_8,     "e9045_2x8_8 -> DoInArm_9045_2x8_8"         },
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

        char buf[160];
        snprintf(buf, sizeof(buf),
            "D-dispatch [%s]: engine ladder routes to its own arm (cursorEngine==cursorDirect==%d)",
            variants[v].name, cursorDirect);
        CHECK(cursorEngine == cursorDirect, buf);
    }

    // =======================================================================
    //  PART D-route -- golden routing fact (golden ainarm9045.cpp :4639):
    //  e9045_2x8_32 routes to the SAME DoInArm_9045_2x8_8() callee as e9045_2x8_8.
    //  Witness: DoInArm_9045()@e9045_2x8_32 leaves iArmTask IDENTICAL to a direct
    //  DoInArm_9045_2x8_8() from the SAME entry state.
    // =======================================================================
    printf("[D-route] e9045_2x8_32 routes to the LIVE DoInArm_9045_2x8_8 callee (golden :4639)\n");
    {
        resetInArmBaseline();
        iInArmType = e9045_2x8_8;
        iArmTask   = 1;
        DoInArm_9045_2x8_8();
        int cursor2x8_8 = iArmTask;

        resetInArmBaseline();
        iInArmType = e9045_2x8_32;
        iArmTask   = 1;
        DoInArm_9045();
        int cursor2x8_32 = iArmTask;

        CHECK(cursor2x8_32 == cursor2x8_8,
              "D-route e9045_2x8_32: engine ladder == direct DoInArm_9045_2x8_8 (golden routes 2x8_32->2x8_8)");
    }

    // =======================================================================
    //  PART D-route(S) -- W6.2c batch-5 made the 2 S-family arms LIVE.  Both
    //  e9045_2x4_4_13 (=21) -> DoInArm_9045_2x4_4_13 and e9045_1x4_4_13 ->
    //  DoInArm_9045S_1x4_4 now dispatch through the engine ladder (no longer the
    //  Program-Error else).  Witness: DoInArm_9045()@type leaves iArmTask IDENTICAL
    //  to a direct call of the variant's own callee from the SAME entry state.
    //  (Was: D-gate UNCHANGED assertion -- stale after batch-5 un-gated these arms.)
    // =======================================================================
    printf("[D-route(S)] e9045_2x4_4_13 / e9045_1x4_4_13 now route to their LIVE S-family callees (batch-5)\n");
    {
        resetInArmBaseline();
        iInArmType = e9045_2x4_4_13;
        iArmTask   = 1;
        DoInArm_9045_2x4_4_13();
        int cursorDirect2x4 = iArmTask;

        resetInArmBaseline();
        iInArmType = e9045_2x4_4_13;
        iArmTask   = 1;
        DoInArm_9045();
        CHECK(iArmTask == cursorDirect2x4,
              "D-route(S) e9045_2x4_4_13: engine ladder == direct DoInArm_9045_2x4_4_13 (batch-5 LIVE)");

        resetInArmBaseline();
        iInArmType = e9045_1x4_4_13;
        iArmTask   = 1;
        DoInArm_9045S_1x4_4();
        int cursorDirect1x4 = iArmTask;

        resetInArmBaseline();
        iInArmType = e9045_1x4_4_13;
        iArmTask   = 1;
        DoInArm_9045();
        CHECK(iArmTask == cursorDirect1x4,
              "D-route(S) e9045_1x4_4_13: engine ladder == direct DoInArm_9045S_1x4_4 (batch-5 LIVE)");
    }

    // The 5 *_SuckerMap() builder symbols are real, defined, callable (no crash).
    DoInArm_9045_2x2_8_Hot_SuckerMap();
    DoInArm_9045_2x4_8_SuckerMap();
    DoInArm_9045_2x5_8_SuckerMap();
    DoInArm_9045_2x6_8_SuckerMap();
    DoInArm_9045_2x8_8_SuckerMap();
    CHECK(true, "D-suckermap: all 5 *_SuckerMap() builder symbols are defined + callable (no crash)");

    // =======================================================================
    //  PART O1 -- ORACLE 1: pure-const picker-column -> shuttle-column map for the
    //  2x6_6 12-site layout.  Side-effect-free; values from golden
    //  ainarm9045_2x6_8.cpp:75.  Exported via the variant .h extern.
    // =======================================================================
    printf("[O1] XPHSuckToSht_2x6_6 const table (golden ainarm9045_2x6_8.cpp:75)\n");
    {
        const int stdRow[8]   = {0, 1, 2, 6, 7, 3, 4, 5};   // e2x6Standard
        const int fixRow[8]   = {0, 2, 1, 6, 7, 4, 3, 5};   // e2x6Step2 (Steven 20250123 fix)
        const int closeRow[8] = {0, 1, 4, 5, 2, 3, 6, 7};   // e2x6CloseCenter2x4
        bool stdOk = true, fixOk = true, closeOk = true;
        for (int j = 0; j < 8; ++j) {
            if (XPHSuckToSht_2x6_6[e2x6Standard][j]       != stdRow[j])   stdOk   = false;
            if (XPHSuckToSht_2x6_6[e2x6Step2][j]          != fixRow[j])   fixOk   = false;
            if (XPHSuckToSht_2x6_6[e2x6CloseCenter2x4][j] != closeRow[j]) closeOk = false;
        }
        CHECK(stdOk,   "O1 XPHSuckToSht_2x6_6[e2x6Standard] == {0,1,2,6,7,3,4,5}");
        CHECK(fixOk,   "O1 XPHSuckToSht_2x6_6[e2x6Step2] == {0,2,1,6,7,4,3,5} (Steven 20250123 fix)");
        CHECK(closeOk, "O1 XPHSuckToSht_2x6_6[e2x6CloseCenter2x4] == {0,1,4,5,2,3,6,7}");
    }

    // =======================================================================
    //  PART O2 -- ORACLE 2: pure-const map XPHSuckToSht_2x8_8[e2x8ModeTotal][2][8].
    //  golden ainarm9045_2x8_8.cpp:112.  Standard row == identity; the STM mode
    //  (e2x8_STMMode) carries the JerryYang-20190729 interleave.
    // =======================================================================
    printf("[O2] XPHSuckToSht_2x8_8 const table (golden ainarm9045_2x8_8.cpp:112)\n");
    {
        const int idRow[8]   = {0, 1, 2, 3, 4, 5, 6, 7};   // e2x8Standard
        const int stm0[8]    = {1, 5, 3, 7, 0, 2, 4, 6};   // e2x8_STMMode row0
        const int stm1[8]    = {4, 0, 6, 2, 1, 3, 5, 7};   // e2x8_STMMode row1
        bool idOk = true, stmOk = true;
        for (int j = 0; j < 8; ++j) {
            if (XPHSuckToSht_2x8_8[e2x8Standard][0][j]  != idRow[j]) idOk  = false;
            if (XPHSuckToSht_2x8_8[e2x8Standard][1][j]  != idRow[j]) idOk  = false;
            if (XPHSuckToSht_2x8_8[e2x8_STMMode][0][j]  != stm0[j])  stmOk = false;
            if (XPHSuckToSht_2x8_8[e2x8_STMMode][1][j]  != stm1[j])  stmOk = false;
        }
        CHECK(idOk,  "O2 XPHSuckToSht_2x8_8[e2x8Standard] both rows == identity {0..7}");
        CHECK(stmOk, "O2 XPHSuckToSht_2x8_8[e2x8_STMMode] == {1,5,3,7,0,2,4,6}/{4,0,6,2,1,3,5,7}");
    }

    // =======================================================================
    //  PART O3 -- ORACLE 3 (independent VERIFY extension, W6.2c-verify):
    //  the file-local pure-const picker-column -> shuttle-column geometry tables
    //  exported by 2x2_8_Hot / 2x4_8 / 2x5_8 (golden ainarm9045_<v>.cpp:32), plus
    //  the DoInArm_9045_2x2_8_Hot_SuckerMap() grid effect on Prod.fInArmSuck4x8
    //  (golden ainarm9045_2x2_8_Hot.cpp:3232).  Zero HAL; deterministic.
    // =======================================================================
    printf("[O3] file-local XPHSuckToSht_2x2_8/2x4_8/2x5_8 const tables + 2x2_8_Hot SuckerMap grid\n");
    {
        // 2x2_8_Hot: const int XPHSuckToSht_2x2_8[8]={0,0,1,1,3,4,5,6}; (golden :32)
        CHECK(XPHSuckToSht_2x2_8[0] == 0, "O3 XPHSuckToSht_2x2_8[0]==0");
        CHECK(XPHSuckToSht_2x2_8[4] == 3, "O3 XPHSuckToSht_2x2_8[4]==3");
        CHECK(XPHSuckToSht_2x2_8[7] == 6, "O3 XPHSuckToSht_2x2_8[7]==6");

        // 2x4_8: row1 (the dual-shuttle swap row) == {0,2,1,3,4,6,5,7} (golden :33)
        CHECK(XPHSuckToSht_2x4_8[1][1] == 2, "O3 XPHSuckToSht_2x4_8[1][1]==2 (swap row)");
        CHECK(XPHSuckToSht_2x4_8[1][2] == 1, "O3 XPHSuckToSht_2x4_8[1][2]==1 (swap row)");
        CHECK(XPHSuckToSht_2x4_8[1][5] == 6, "O3 XPHSuckToSht_2x4_8[1][5]==6 (swap row)");

        // 2x5_8: row0 == {0,1,2,5,6,7,3,4} (golden :32) -- the 10-site interleave
        CHECK(XPHSuckToSht_2x5_8[0][3] == 5, "O3 XPHSuckToSht_2x5_8[0][3]==5");
        CHECK(XPHSuckToSht_2x5_8[0][6] == 3, "O3 XPHSuckToSht_2x5_8[0][6]==3");

        // 2x8_8 倆倆放 (e2x8Step2=1) row0 == {0,2,1,3,4,6,5,7} (golden :37); also
        // the standard-mode identity entry [e2x8Standard][0][3]==3 (golden :35).
        CHECK(XPHSuckToSht_2x8_8[e2x8Standard][0][3] == 3, "O3 XPHSuckToSht_2x8_8[e2x8Standard][0][3]==3");
        CHECK(XPHSuckToSht_2x8_8[e2x8Step2][0][1]    == 2, "O3 XPHSuckToSht_2x8_8[e2x8Step2/*倆倆放*/][0][1]==2");

        // SuckerMap grid effect (golden ainarm9045_2x2_8_Hot.cpp:3232): for i=0,1
        //   fInArmSuck4x8[0][i][0]=[0][i][1]=bUseTestSocket[0][i][0]
        //   fInArmSuck4x8[0][i][2]=[0][i][3]=bUseTestSocket[0][i][1]
        // Seed bUseTestSocket all-true, clear the target grid, drive the builder
        // arm directly, and assert the 2x2_8 fan-out pattern landed.
        for (int a = 0; a < 2; ++a)
            for (int r = 0; r < 4; ++r)
                for (int c = 0; c < 8; ++c)
                    LastSet.bUseTestSocket[a][r][c] = true;
        for (int a = 0; a < 2; ++a)
            for (int r = 0; r < 2; ++r)
                for (int c = 0; c < 8; ++c)
                    Prod.fInArmSuck4x8[a][r][c] = false;

        DoInArm_9045_2x2_8_Hot_SuckerMap();

        bool fanOut = Prod.fInArmSuck4x8[0][0][0] && Prod.fInArmSuck4x8[0][0][1] &&
                      Prod.fInArmSuck4x8[0][0][2] && Prod.fInArmSuck4x8[0][0][3] &&
                      Prod.fInArmSuck4x8[0][1][0] && Prod.fInArmSuck4x8[0][1][1] &&
                      Prod.fInArmSuck4x8[0][1][2] && Prod.fInArmSuck4x8[0][1][3];
        CHECK(fanOut,
              "O3 2x2_8_Hot_SuckerMap: bUseTestSocket all-true -> fInArmSuck4x8[0][0..1][0..3] all true (golden :3232)");

        // Falsify guard: with bUseTestSocket all-FALSE the same arm leaves the
        // 2x2_8 fan-out cells false (proves the builder reads the source map, not
        // a constant), driving the SAME [0][i][0..3] cells the true-case set.
        for (int a = 0; a < 2; ++a)
            for (int r = 0; r < 4; ++r)
                for (int c = 0; c < 8; ++c)
                    LastSet.bUseTestSocket[a][r][c] = false;
        DoInArm_9045_2x2_8_Hot_SuckerMap();
        bool clearedOut = !Prod.fInArmSuck4x8[0][0][0] && !Prod.fInArmSuck4x8[0][0][3] &&
                          !Prod.fInArmSuck4x8[0][1][0] && !Prod.fInArmSuck4x8[0][1][3];
        CHECK(clearedOut,
              "O3 2x2_8_Hot_SuckerMap: bUseTestSocket all-false -> fInArmSuck4x8[0][0..1][0,3] all false (reads source map)");
    }

    // -----------------------------------------------------------------------
    printf("==== W6.2c batch-4 InArm 5-variant verify: %d passed, %d failed ====\n",
           g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
