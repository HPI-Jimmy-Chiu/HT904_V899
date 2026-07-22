// =============================================================================
//  test_w7_grid_occupancy.cpp -- W7 SUBSTRATE STAGING VERIFY: KitSuck grid ON
//
//  Translation wave: W7 (turn on the cross-arm KitSuck occupancy grid by flipping
//                    csystem_predicates.cpp HT9045_KITSUCK_GRID_AVAILABLE 0 -> 1
//                    and giving TMyKitSuck::UseSiteHasIC()/UseSiteNoIC() the
//                    faithful golden Item-scan bodies).
//  Author: AI(W7-GRID-INTEGRATE) 20260629
//  Suite name (add_test): W7_GridOccupancy
//
//  PURPOSE
//  -------
//  Proves the W7 substrate-staging change wires the csystem HasIC predicate family
//  to the REAL (seedable) TMyKitSuck Item grid -- it now consults the grid instead
//  of returning a hard-coded false.
//
//    PART A -- UNIT oracles on a LOCAL TMyKitSuck (the leaf bodies themselves):
//      (a) fresh kit (Item all zero == NULL_IC, default iShtRow=2 x iShtCol=1):
//          UseSiteHasIC()==false, UseSiteNoIC()==true, HasRealIC()==false.
//      (b) seed a use-site cell within iShtRow x iShtCol to HAS_IC:
//          UseSiteHasIC()==true, UseSiteNoIC()==false, HasRealIC()==true,
//          CountRealIC()>=1 (CountRealIC scans iMaxRow x iMaxCol, so seed those too).
//      (c) clear (ClearAll) -> UseSiteHasIC()==false again, UseSiteNoIC()==true.
//
//    PART B -- END-TO-END FLAG-FLIP PROOF (the load-bearing one):
//      Take a GLOBAL carry-kit a csystem predicate reads -- FLCarryKit, which
//      InputShuttleFrontHasIC() consults (csystem_predicates.cpp:58-65, ON-path
//      `return FLCarryKit.UseSiteHasIC()`).  Save/restore its grid.  Seed a
//      use-site Item to HAS_IC and assert:
//          InputShuttleFrontHasIC()==true  (leaf via the now-live #if branch)
//          ShuttleHasIC()==true            (aggregate OR over the same leaf)
//      then clear and assert both ==false again.  This proves the flag flip wired
//      the predicate to the seedable grid (with the flag OFF the leaf was a
//      hard-coded `return false` and seeding could never make it true).
//      ALSO seed FTestSuck and assert IndexHasRealIC()==true (FTestSuck.HasRealIC()
//      is the ON-path leaf, csystem_predicates.cpp:178-185) as a second wiring
//      witness on a different kit / different predicate, then clear.
//      All globals are saved and restored so other suites are unaffected.
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" == clean g++
//  compile/link + the predicates reflect the seeded grid exactly as the golden
//  MyKitSuck.cpp:288/297 Item-scan dictates, with the empty-grid result identical
//  to the pre-flip conservative `return false`.
// =============================================================================
#include "csystem.h"               // InputShuttleFrontHasIC / ShuttleHasIC / IndexHasRealIC
#include "aHotPlateSubstrate.h"    // TMyKitSuck + FLCarryKit / FTestSuck globals
#include "cmydef.h"                // NULL_IC / HAS_IC
#include <cstdio>

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as the W6/W7 verify TUs)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// Snapshot/restore one TMyKitSuck Item grid + the loop-bound scalars the
// predicates read, so seeding a global kit cannot leak into sibling suites.
struct KitSnapshot {
    int item[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int iShtRow, iShtCol, iMaxRow, iMaxCol;
};
static void SaveKit(TMyKitSuck &k, KitSnapshot &s) {
    for (int i = 0; i < _MAX_SUCK_ROW_ITEM; ++i)
        for (int j = 0; j < _MAX_SUCK_COL_ITEM; ++j)
            s.item[i][j] = k.Item[i][j];
    s.iShtRow = k.iShtRow; s.iShtCol = k.iShtCol;
    s.iMaxRow = k.iMaxRow; s.iMaxCol = k.iMaxCol;
}
static void RestoreKit(TMyKitSuck &k, const KitSnapshot &s) {
    for (int i = 0; i < _MAX_SUCK_ROW_ITEM; ++i)
        for (int j = 0; j < _MAX_SUCK_COL_ITEM; ++j)
            k.Item[i][j] = s.item[i][j];
    k.iShtRow = s.iShtRow; k.iShtCol = s.iShtCol;
    k.iMaxRow = s.iMaxRow; k.iMaxCol = s.iMaxCol;
}

int main()
{
    printf("==== W7 GridOccupancy verify (KitSuck grid ON) ====\n");

    // =======================================================================
    //  PART A -- UNIT oracles on a LOCAL TMyKitSuck
    // =======================================================================
    printf("[A] LOCAL TMyKitSuck leaf-body oracles\n");

    TMyKitSuck kit;   // ctor: iShtRow=2, iShtCol=1 -- see note below on Item.

    // AI(W906-CommonCompletion) 20260721: FOUND + FIXED a latent test bug,
    // unmasked by this wave's unrelated common.cpp/acarry_shims.cpp/
    // ainarm9045_2x4_16_shims.cpp edits (which merely changed code size/
    // layout enough to perturb pre-main() stack contents -- root cause is
    // here, not in that wave's translated code). VERIFIED via aHotPlateSubstrate.cpp:90-95's
    // own comment ("Item/iWhichSite are int grids left to the loader"):
    // TMyKitSuck::TMyKitSuck() deliberately does NOT zero the Item[][] array
    // (unlike iBinData/cDeviceInf/cSBin/etc., which the ctor loop above DOES
    // clear). The comment removed just above this block ("Item zero-init")
    // was simply wrong about what the real ctor does -- a fresh LOCAL
    // TMyKitSuck's Item[][] is genuinely indeterminate stack memory, not
    // guaranteed zero; this test previously passed only by luck (a freshly
    // started process's first main()-local variable typically -- but not
    // reliably -- sees zero-filled stack pages). Explicitly zero it here so
    // "fresh kit" is deterministic, matching this test's own stated intent.
    for (int i = 0; i < _MAX_SUCK_ROW_ITEM; ++i)
        for (int j = 0; j < _MAX_SUCK_COL_ITEM; ++j)
            kit.Item[i][j] = 0;   // == NULL_IC

    // -- (a) fresh kit: empty grid == all NULL_IC --------------------------
    CHECK(kit.iShtRow == 2 && kit.iShtCol == 1,
          "a0 ctor defaults iShtRow=2 / iShtCol=1 (golden 163/164)");
    CHECK(kit.UseSiteHasIC() == false,
          "a1 fresh kit UseSiteHasIC()==false (empty grid, golden MyKitSuck.cpp:288)");
    CHECK(kit.UseSiteNoIC() == true,
          "a2 fresh kit UseSiteNoIC()==true (empty grid, golden MyKitSuck.cpp:297)");
    CHECK(kit.HasRealIC() == false,
          "a3 fresh kit HasRealIC()==false (no HAS_IC/HAS_HOT_IC cell)");

    // -- (b) seed a use-site within iShtRow x iShtCol (default 2x1) ----------
    // Item[0][0] is inside the 2x1 use-site window.  CountRealIC scans
    // iMaxRow x iMaxCol (ctor-default 0), so widen those to cover [0][0] too.
    kit.iMaxRow = 2; kit.iMaxCol = 1;
    kit.Item[0][0] = HAS_IC;            // seed a real IC at a use-site
    CHECK(kit.UseSiteHasIC() == true,
          "b1 seeded use-site -> UseSiteHasIC()==true (golden Item-scan finds HAS_IC)");
    CHECK(kit.UseSiteNoIC() == false,
          "b2 seeded use-site -> UseSiteNoIC()==false");
    CHECK(kit.HasRealIC() == true,
          "b3 seeded HAS_IC -> HasRealIC()==true");
    CHECK(kit.CountRealIC() >= 1,
          "b4 seeded HAS_IC -> CountRealIC()>=1 (iMaxRow x iMaxCol scan)");

    // -- (c) clear -> empty again -------------------------------------------
    kit.ClearAll();                     // every cell back to NULL_IC
    CHECK(kit.UseSiteHasIC() == false,
          "c1 cleared kit -> UseSiteHasIC()==false again");
    CHECK(kit.UseSiteNoIC() == true,
          "c2 cleared kit -> UseSiteNoIC()==true again");
    CHECK(kit.HasRealIC() == false,
          "c3 cleared kit -> HasRealIC()==false again");

    // =======================================================================
    //  PART B -- END-TO-END FLAG-FLIP PROOF over the GLOBAL kits
    // =======================================================================
    printf("[B] csystem predicate <- global seedable grid (flag-flip proof)\n");

    KitSnapshot flSave, ftSave;
    SaveKit(FLCarryKit, flSave);
    SaveKit(FTestSuck,  ftSave);

    // --- B1: empty baseline -> predicates conservative false ----------------
    FLCarryKit.ClearAll();
    FTestSuck.ClearAll();
    CHECK(InputShuttleFrontHasIC() == false,
          "B1a empty FLCarryKit -> InputShuttleFrontHasIC()==false (== old conservative stub)");
    CHECK(ShuttleHasIC() == false,
          "B1b empty grids -> ShuttleHasIC()==false (aggregate over empty leaves)");
    CHECK(IndexHasRealIC() == false,
          "B1c empty FTestSuck -> IndexHasRealIC()==false");

    // --- B2: seed FLCarryKit use-site -> InputShuttleFront / Shuttle true ----
    // FLCarryKit default iShtRow=2 x iShtCol=1; Item[0][0] is in the use-site
    // window InputShuttleFrontHasIC() reads via FLCarryKit.UseSiteHasIC().
    FLCarryKit.Item[0][0] = HAS_IC;
    CHECK(InputShuttleFrontHasIC() == true,
          "B2a seeded FLCarryKit use-site -> InputShuttleFrontHasIC()==true (flag-flip wired the leaf)");
    CHECK(ShuttleHasIC() == true,
          "B2b seeded FLCarryKit -> ShuttleHasIC()==true (aggregate sees the live leaf)");

    // --- B3: clear FLCarryKit -> back to false ------------------------------
    FLCarryKit.ClearAll();
    CHECK(InputShuttleFrontHasIC() == false,
          "B3a cleared FLCarryKit -> InputShuttleFrontHasIC()==false again");
    CHECK(ShuttleHasIC() == false,
          "B3b cleared FLCarryKit -> ShuttleHasIC()==false again");

    // --- B4: second-kit / second-predicate wiring witness (FTestSuck) -------
    // IndexHasRealIC() ON-path reads FTestSuck.HasRealIC() (full-grid HAS_IC scan).
    FTestSuck.Item[0][0] = HAS_IC;
    CHECK(IndexHasRealIC() == true,
          "B4a seeded FTestSuck -> IndexHasRealIC()==true (flag-flip wired FTestSuck.HasRealIC)");
    FTestSuck.ClearAll();
    CHECK(IndexHasRealIC() == false,
          "B4b cleared FTestSuck -> IndexHasRealIC()==false again");

    // --- restore globals so sibling suites see the pristine state -----------
    RestoreKit(FLCarryKit, flSave);
    RestoreKit(FTestSuck,  ftSave);
    CHECK(InputShuttleFrontHasIC() == false && IndexHasRealIC() == false,
          "B5 globals restored -> predicates back to baseline false");

    // -----------------------------------------------------------------------
    printf("==== W7 GridOccupancy verify: %d passed, %d failed ====\n",
           g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
