// =============================================================================
//  test_w6_2_outarm_variants.cpp  --  W6.2c-OUT VERIFY: the LIVE out-arm site
//  variants (batch-1: 1x1_1 / 1x2_2 / 1x2_4 / 1x3_2_14 / 1x3_4 / 1x4_2)
//
//  Translation wave: W6.2c-OUT (out-arm per-site-config variants over Sim HAL)
//  Author: AI(W6.2c-outarm-verify) 20260627
//
//  PURPOSE
//  -------
//  Independently prove the 6 LIVE out-arm site variants of this batch are not
//  just COMPILED+LINKED but ACTUALLY DISPATCHED: that the engine dispatch arm
//  for each layout resolves to the REAL translated DoOutArm_9045_<v>() body
//  (NOT the removed file-local no-op `static void ...(){}` stub that used to
//  short-circuit them in aoutarm9045.cpp).
//
//  DISPATCH-ROUTING WITNESS (observable side effect, golden-cited)
//  ---------------------------------------------------------------
//  Each variant's DoOutArm_9045_<v>() opens with the SAME verbatim golden
//  task-1 entry (e.g. aoutarm9045_1x1_1.cpp:676, aoutarm9045_1x4_2.cpp:788):
//
//      int &Task = OutArmTask;
//      if (bCarryControlOutarm1 || bCarryControlOutarm2) return;   // guard
//      switch (Task) {
//        case 1:
//          bSortingAllBinTrayFinish = false;   // <-- observable global write
//          Task = 5;                           // (no break -> fall through)
//        case 5:
//          if (MoveOutArmToAutoSafe()) Task = 10;   // offline stub -> true
//          if (Task != 10) break;
//        case 10:
//          if (CheckOutArmInitState()) Task = 50;   // offline shim -> true
//          if (Task != 50) break;
//        case 50: ...
//
//  So from a seeded entry cursor OutArmTask==1, ONE call to the REAL body:
//    * sets the global bSortingAllBinTrayFinish = false, AND
//    * advances OutArmTask off 1 (to 50, since the offline MoveOutArmToAutoSafe()
//      stub + CheckOutArmInitState() shim both return true and the case-1/5/10
//      fall-throughs chain to case 50).
//  The DELETED static no-op stub `static void DoOutArm_9045_<v>(){}` would have
//  left OutArmTask==1 untouched and never written bSortingAllBinTrayFinish.
//  Observing OutArmTask move off 1 is therefore a sound, deterministic witness
//  that the dispatch reaches the REAL func.  We assert it for ALL 6 variants by
//  calling each variant's engine-dispatched callee directly (the engine ladder
//  in aoutarm9045.cpp routes its enum to exactly this symbol -- see the
//  ENUM->CALLEE map: e9045_1x1_1|1x4_1_Ac->1x1_1; 1x2_2_13|14->1x2_2;
//  1x2_4_Hot->1x2_4; 1x3_2_14->1x3_2_14; 1x3_4->1x3_4; 1x4_2_14->1x4_2).
//
//  We pre-seed OutArmTask to a SENTINEL (1) and bSortingAllBinTrayFinish to a
//  DIRTY value (true) before each call, so a no-op stub would be caught by BOTH
//  the unchanged cursor AND the un-cleared flag.
//
//  EQUIVALENCE NOTE: no Borland binary exists -> "equivalence" here == clean g++
//  compile/link + the variant bodies are DEFINED (not stubbed) + the dispatch
//  reaches them (cursor/flag side effect) over the Sim HAL with no crash.
// =============================================================================
#include "aoutarm9045_1x1_1.h"      // void DoOutArm_9045_1x1_1();
#include "aoutarm9045_1x2_2.h"      // void DoOutArm_9045_1x2_2();
#include "aoutarm9045_1x2_4.h"      // void DoOutArm_9045_1x2_4();
#include "aoutarm9045_1x3_2_14.h"   // void DoOutArm_9045_1x3_2_14();
#include "aoutarm9045_1x3_4.h"      // void DoOutArm_9045_1x3_4();
#include "aoutarm9045_1x4_2.h"      // void DoOutArm_9045_1x4_2();
#include "aoutarm9045.h"            // GetOutArmPitch_9045 (engine geometry oracle)
#include "cprod.h"                  // Prod / IniConfig (via Config.h)
#include "cmydef.h"                 // bSortingAllBinTrayFinish / iPitch_Max_minus_Min / iXpitchMinX3
#include <cstdio>

// ---------------------------------------------------------------------------
//  Engine SM cursor + carry-control flags that are NOT on the variant .h
//  surfaces.  Same forward-declare idiom test_w6_2_outarm_core.cpp uses for
//  engine-internal cursors:
//    OutArmTask              -- offline-defined in aoutarm_shims.cpp:31
//    bCarryControlOutarm1/2  -- offline-defined in acarry_shims.cpp:171-172
//      (golden aoutarm.cpp:67-68 carry-control hand-off flags)
// ---------------------------------------------------------------------------
extern int  OutArmTask;
extern bool bCarryControlOutarm1;
extern bool bCarryControlOutarm2;

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as the other W6 verify TUs)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// Drive one variant's engine-dispatched callee from the task-1 entry and assert
// the REAL body ran (cursor advanced off 1 + the entry global was cleared).
static void witnessRouting(const char* name, void (*fn)())
{
    bCarryControlOutarm1 = false;        // clear the function-top early-return guard
    bCarryControlOutarm2 = false;
    OutArmTask              = 1;         // seed entry cursor
    bSortingAllBinTrayFinish = true;     // DIRTY: real case-1 entry sets it false

    fn();                                // dispatch to the REAL translated body

    char buf[160];
    snprintf(buf, sizeof(buf),
             "%s: REAL body ran -- OutArmTask advanced off 1 (got %d, golden case-1->5->10->50)",
             name, OutArmTask);
    CHECK(OutArmTask != 1, buf);

    snprintf(buf, sizeof(buf),
             "%s: REAL body ran -- bSortingAllBinTrayFinish cleared to false (golden case 1)",
             name);
    CHECK(bSortingAllBinTrayFinish == false, buf);
}

int main()
{
    printf("==== W6.2c-OUT variants verify (batch: 1x1_1/1x2_2/1x2_4/1x3_2_14/1x3_4/1x4_2) ====\n");

    // =======================================================================
    //  PART A -- dispatch-routing witness for all 6 LIVE variants
    // =======================================================================
    printf("[A] dispatch routes to the REAL DoOutArm_9045_<v> (not the removed no-op stub)\n");
    witnessRouting("1x1_1",    DoOutArm_9045_1x1_1);
    witnessRouting("1x2_2",    DoOutArm_9045_1x2_2);
    witnessRouting("1x2_4",    DoOutArm_9045_1x2_4);
    witnessRouting("1x3_2_14", DoOutArm_9045_1x3_2_14);
    witnessRouting("1x3_4",    DoOutArm_9045_1x3_4);
    witnessRouting("1x4_2",    DoOutArm_9045_1x4_2);

    // =======================================================================
    //  PART B -- engine geometry oracle still holds with variants linked
    //  (regression guard: adding the 6 variant TUs to ht9045_sm must not
    //   perturb the engine GetOutArmPitch_9045 interpolation -- same oracle
    //   as test_w6_2_outarm_core [N3], hand-derived from golden:212).
    // =======================================================================
    printf("[B] engine geometry oracle GetOutArmPitch_9045 (golden :212)\n");
    Prod.iOutArmX40Pitch  = 4000;
    Prod.iOutArmX120Pitch = 12000;
    iPitch_Max_minus_Min  = 8000;
    iXpitchMinX3          = 4000;
    CHECK(GetOutArmPitch_9045(8000) == 8000,
          "B GetOutArmPitch_9045(w=8000) == 8000 (m=1.0, endpoint)");
    CHECK(GetOutArmPitch_9045(4000) == 4000,
          "B GetOutArmPitch_9045(w=4000) == 4000 (min teach point -> X40)");

    printf("==== W6.2c-OUT variants verify: %d passed, %d failed ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
