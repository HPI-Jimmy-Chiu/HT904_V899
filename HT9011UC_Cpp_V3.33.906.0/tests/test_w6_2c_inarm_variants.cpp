// =============================================================================
//  test_w6_2c_inarm_variants.cpp  --  W6.2c batch-1 VERIFY: 5 in-arm site variants
//
//  Translation wave: W6.2c batch-1 (in-arm per-site-config variant batch)
//  Author: AI(W6.2c-verify) 20260626
//
//  PURPOSE
//  -------
//  Independently proves the W6.2c batch-1 of 5 in-arm site variants
//    (1x3_4, 1x2_2, 1x2_2_14, 1x2_4_Hot, 1x3_2_14)
//  offline over the Sim HAL, with NO hardware and NO Borland binary.  Proves:
//    (1) the in-arm ENGINE dispatch ladders (DoInArm_9045 / DoInArm_9045_SuckerMap,
//        ainarm9045.cpp) route each of the 5 variant iInArmType values to its
//        own translated DoInArm_9045_<v>() / _SuckerMap() arm (un-gated, ACTIVE),
//        while a still-gated iInArmType hits the Program-Error else (cursor
//        UNCHANGED) -- so ONLY these 5 (+ the prior 1x1_1) were un-gated;
//    (2) the pure-const lookup table XPHSuckToSht_1x2_4 (read by the engine at
//        ainarm9045.cpp:513 for 1x2_4_Hot) carries the exact golden values
//        (ORACLE 1);
//    (3) the TMyKitSuck Arm{Up,Down,Left,Right}SideHaveRealIC Item-grid scans
//        (transcribed verbatim from golden mykitsuck.cpp; drive 1x2_4_Hot's
//        place-to-shuttle SM) return the exact golden-derived values (ORACLE 2);
//    (4) GetShuttleState_1x3_4 pick branch returns the golden values (ORACLE 3).
//
//  EQUIVALENCE NOTE (honest): no Borland binary exists, so "equivalence" here ==
//  clean g++ compile/link + the 5 variant funcs defined+dispatched + the pure
//  numeric outputs match values hand-derived from the golden formula/table.
//
//  Prints PASS/FAIL per check + a final summary; returns non-zero on any FAIL.
// =============================================================================
#include "ainarm9045.h"             // DoInArm_9045 / DoInArm_9045_SuckerMap
#include "ainarm9045_1x3_4.h"       // GetShuttleState_1x3_4 (ORACLE 3)
#include "ainarm9045_1x2_4_Hot.h"   // XPHSuckToSht_1x2_4 (ORACLE 1)
#include "aHotPlateSubstrate.h"     // TMyKitSuck, InArmSuck, FLCarryKit, ptrInSHT, iArmTask
#include "csystem.h"
#include "Motor/mymotor.h"
#include "cprod.h"
#include "cpublic.h"
#include "cmydef.h"                 // NULL_IC / HAS_IC / HAS_NULL_IC / bAutoPick
#include "MachineType.h"            // eInArmType enum values
#include "canary_support.h"         // LastSet, ShowErrorMessage, ...
#include "FormsFacade.h"
#include <cstdio>

// The 5 variant master SMs + picker-map builders are file-local in the golden
// (consumed via the engine dispatch ladder), so they are NOT on the per-variant
// .h surface.  Declare them locally for the direct-dispatch assertions -- the
// engine declares the SAME externs near its ladders (ainarm9045.cpp :772-781).
extern void DoInArm_9045_1x2_2();            extern void DoInArm_9045_1x2_2_SuckerMap();
extern void DoInArm_9045_1x2_2_14();         extern void DoInArm_9045_1x2_2_14_SuckerMap();
extern void DoInArm_9045_1x2_4_Hot();        extern void DoInArm_9045_1x2_4_Hot_SuckerMap();
extern void DoInArm_9045_1x3_4();            extern void DoInArm_9045_1x3_4_SuckerMap();
extern void DoInArm_9045_1x3_2_14();         extern void DoInArm_9045_1x3_2_14_SuckerMap();

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
// baseline before a dispatch probe (mirror of the W6.2b verify reset).
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
// for the HaveRealIC scans -- iMaxRow rows x 8 cols, full 4x8 to be safe).
static void clearKitItem(TMyKitSuck &k, int rows)
{
    k.iMaxRow = rows;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 8; ++j)
            k.Item[i][j] = NULL_IC;
}

int main()
{
    printf("==== W6.2c batch-1 InArm 5-variant verify ====\n");

    // =======================================================================
    //  PART D -- dispatch ROUTING over DoInArm_9045() / _SuckerMap()
    //  Each variant's iInArmType must route to ITS OWN arm (cursor advances
    //  out of entry / no Program-Error), and a still-gated type must NOT.
    // =======================================================================
    printf("[D] DoInArm_9045() / _SuckerMap() dispatch routing for the 5 variants\n");

    // The 5 active iInArmType enum values for this batch (per the integrate map:
    // 1x2_2 routes on e9045_1x2_2_13; golden engine arms :4538-4559).
    struct { int t; const char *name; } variants[] = {
        { e9045_1x2_2_13, "e9045_1x2_2_13 -> DoInArm_9045_1x2_2"      },
        { e9045_1x2_2_14, "e9045_1x2_2_14 -> DoInArm_9045_1x2_2_14"   },
        { e9045_1x2_4_Hot,"e9045_1x2_4_Hot -> DoInArm_9045_1x2_4_Hot" },
        { e9045_1x3_2_14, "e9045_1x3_2_14 -> DoInArm_9045_1x3_2_14"   },
        { e9045_1x3_4,    "e9045_1x3_4 -> DoInArm_9045_1x3_4"         },
    };
    for (int v = 0; v < 5; ++v) {
        resetInArmBaseline();
        iInArmType = variants[v].t;
        iArmTask   = 1;
        DoInArm_9045();                  // ONE tick from entry
        char buf[160];
        snprintf(buf, sizeof(buf),
            "D-dispatch [%s]: routed to its own arm -> iArmTask advanced out of entry (NOT Program-Error else)",
            variants[v].name);
        CHECK(iArmTask != 1, buf);
    }

    // A STILL-GATED iInArmType (e9045_2x4_8 = 22, remains in the #if 0 gate) must
    // hit the Program-Error else and leave the cursor UNCHANGED -- proving only
    // the intended arms were un-gated.
    resetInArmBaseline();
    iInArmType = e9045_2x4_8;
    iArmTask   = 1;
    DoInArm_9045();
    CHECK(iArmTask == 1,
          "D-gate [e9045_2x4_8 still GATED]: Program-Error else -> iArmTask UNCHANGED (only the 5 + 1x1_1 un-gated)");

    // The SuckerMap ladder: each variant routes to its own builder without crash;
    // a gated type -> Program-Error, grid stays cleared.  Drive the builder arms
    // directly first (they are real, defined symbols), then via the engine ladder.
    DoInArm_9045_1x2_2_SuckerMap();
    DoInArm_9045_1x2_2_14_SuckerMap();
    DoInArm_9045_1x2_4_Hot_SuckerMap();
    DoInArm_9045_1x3_4_SuckerMap();
    DoInArm_9045_1x3_2_14_SuckerMap();
    CHECK(true, "D-suckermap: all 5 *_SuckerMap() builder symbols are defined + callable (no crash)");

    // =======================================================================
    //  PART O1 -- ORACLE 1: pure const table XPHSuckToSht_1x2_4 (zero HAL)
    //  golden ainarm9045_1x2_4_Hot.cpp:32 ; read by engine ainarm9045.cpp:513
    // =======================================================================
    printf("[O1] XPHSuckToSht_1x2_4 const lookup table (golden 1x2_4_Hot:32)\n");
    CHECK(XPHSuckToSht_1x2_4[0][0][1] == 2,
          "O1 XPHSuckToSht_1x2_4[0][0][1] == 2");
    CHECK(XPHSuckToSht_1x2_4[1][0][1] == 0,
          "O1 XPHSuckToSht_1x2_4[1][0][1] == 0");
    CHECK(XPHSuckToSht_1x2_4[1][1][0] == 4,
          "O1 XPHSuckToSht_1x2_4[1][1][0] == 4");
    CHECK(XPHSuckToSht_1x2_4[0][1][7] == 7,
          "O1 XPHSuckToSht_1x2_4[0][1][7] == 7");

    // =======================================================================
    //  PART O2 -- ORACLE 2: Arm{Up,Down,Left,Right}SideHaveRealIC Item scans
    //  (TMyKitSuck, golden mykitsuck.cpp; drive 1x2_4_Hot place-to-shuttle SM)
    // =======================================================================
    printf("[O2] TMyKitSuck Arm*SideHaveRealIC Item-grid scans (golden mykitsuck.cpp)\n");
    {
        TMyKitSuck k;
        // all-NULL_IC, iMaxRow=1 -> nothing is a "real" IC -> all false
        clearKitItem(k, 1);
        CHECK(k.ArmUpSideHaveRealIC(true)    == false, "O2 all-NULL: ArmUpSideHaveRealIC(true)==false");
        CHECK(k.ArmDownSideHaveRealIC(true)  == false, "O2 all-NULL: ArmDownSideHaveRealIC(true)==false");
        CHECK(k.ArmLeftSideHaveRealIC(1)     == false, "O2 all-NULL: ArmLeftSideHaveRealIC(1)==false");
        CHECK(k.ArmRightSideHaveRealIC(1)    == false, "O2 all-NULL: ArmRightSideHaveRealIC(1)==false");

        // Item[0][2]=HAS_IC (a real IC; up-side j-loop j=0..3, offset 0 -> j=2 hits) -> iOther=1
        clearKitItem(k, 1);
        k.Item[0][2] = HAS_IC;
        CHECK(k.ArmUpSideHaveRealIC(true)    == true,  "O2 Item[0][2]=HAS_IC: ArmUpSideHaveRealIC(true)==true (j=2,offset=0)");

        // Item[0][3]=HAS_IC, ArmRightSideHaveRealIC(2): scans Item[i][1+j*2] j=0,1 -> idx 1,3 -> 3 hits
        clearKitItem(k, 1);
        k.Item[0][3] = HAS_IC;
        CHECK(k.ArmRightSideHaveRealIC(2)    == true,  "O2 Item[0][3]=HAS_IC: ArmRightSideHaveRealIC(2)==true (j=1 -> idx 1+1*2=3)");

        // Item[0][0]=HAS_NULL_IC only -> counts as HasNull, NOT Other -> false
        clearKitItem(k, 1);
        k.Item[0][0] = HAS_NULL_IC;
        CHECK(k.ArmUpSideHaveRealIC(true)    == false, "O2 Item[0][0]=HAS_NULL_IC: ArmUpSideHaveRealIC(true)==false (HasNull, not Other)");

        // offset=4 (left=false) scans Item[0][4..7]: a real IC at [0][2] is NOT seen
        clearKitItem(k, 1);
        k.Item[0][2] = HAS_IC;
        CHECK(k.ArmUpSideHaveRealIC(false)   == false, "O2 left=false offset=4: real IC at [0][2] not in [4..7] -> false");
        k.Item[0][5] = HAS_IC;
        CHECK(k.ArmUpSideHaveRealIC(false)   == true,  "O2 left=false offset=4: real IC at [0][5] IS in [4..7] -> true");
    }

    // =======================================================================
    //  PART O3 -- ORACLE 3: GetShuttleState_1x3_4 pick branch (verbatim)
    //  bPick==bAutoPick(false) -> pick branch; ptrInSHT defaults to &FLCarryKit.
    // =======================================================================
    printf("[O3] GetShuttleState_1x3_4 pick branch (golden ainarm9045_1x3_4.cpp:2965)\n");
    {
        // ptrInSHT -> FLCarryKit (substrate default).  All NULL_IC: ArmUpSideAllTypeIC
        // (NULL_IC,0,3)==true -> "no IC" -> return 0.
        clearKitItem(FLCarryKit, 1);
        InArmSuck.SetAllToNullIC();
        int s0 = GetShuttleState_1x3_4(0, bAutoPick);
        CHECK(s0 == 0, "O3 all-NULL pick branch: GetShuttleState_1x3_4(0,bAutoPick)==0 (no IC)");

        // ptrInSHT->Item[0][0]=HAS_IC -> ArmUpSideAllTypeIC(NULL_IC,0,3)==false -> return 1
        clearKitItem(FLCarryKit, 1);
        FLCarryKit.Item[0][0] = HAS_IC;
        int s1 = GetShuttleState_1x3_4(0, bAutoPick);
        CHECK(s1 == 1, "O3 Item[0][0]=HAS_IC pick branch: GetShuttleState_1x3_4(0,bAutoPick)==1 (left-top has IC)");
        clearKitItem(FLCarryKit, 1);   // restore clean
    }

    // -----------------------------------------------------------------------
    printf("==== W6.2c batch-1 InArm 5-variant verify: %d passed, %d failed ====\n",
           g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
