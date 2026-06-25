// ===========================================================================
//  tests/test_PickPlanner.cpp
//  Golden oracle test for the W2 PickPlanner units:
//
//    Common/PickPlanner/cArmPickPlan.cpp         (generic enumerate+score engine)
//    Common/PickPlanner/cInArmLoaderPickPlanner.cpp (Phase 3 scaffold adapter)
//    Common/PickPlanner/cOutArmPlacePlanner.cpp     (Phase 4 scaffold adapter)
//
//  Strategy:
//    1. Drive cArmPickPlan with a hand-built stub IArmPickPlanContext whose
//       topology is fully known, and assert the chosen "best plan" matches the
//       documented enumerate-(lead,row,col) / score-by-active-count / strict-'>'
//       tie-break semantics (cArmPickPlan.cpp:93,97).
//    2. Assert the boundary guards: null ctx, 0 pickers, 0 row/col max,
//       pickers > ARM_PICK_PLAN_MAX_SLOTS all return false (Search() lines
//       109-116).
//    3. Assert the IsLeadValid() pre-filter is honoured (Search() line 122-126).
//    4. Assert the shipped scaffold adapters behave as documented:
//       Init->Search()==false, Found()==false, GetActiveCount()==0,
//       IsAuthoritative()==false, runtime gates default false.
//
//  Toolchain: MinGW g++ 6.3+, C++17.  Non-zero exit on any failure.
//  No-Borland limitation: behavior is validated against the faithfully
//  translated logic, not a Borland binary diff.
// ===========================================================================
#include "cArmPickPlan.h"
#include "cInArmLoaderPickPlanner.h"
#include "cOutArmPlacePlanner.h"

#include <cstdio>

static int g_fail  = 0;
static int g_total = 0;

static void checkInt(int got, int expect, const char* expr, const char* file, int line)
{
    ++g_total;
    if (got != expect) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got %d  expected %d\n",
                    file, line, expr, got, expect);
    }
}
static void checkBool(bool got, bool expect, const char* expr, const char* file, int line)
{
    ++g_total;
    if (got != expect) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got %s  expected %s\n",
                    file, line, expr, got ? "true" : "false",
                    expect ? "true" : "false");
    }
}
#define CHECK_INT(expr, expect)  checkInt((expr),  (expect), #expr, __FILE__, __LINE__)
#define CHECK_BOOL(expr, expect) checkBool((expr), (expect), #expr, __FILE__, __LINE__)

// ===========================================================================
//  StubContext: a fully deterministic IArmPickPlanContext.
//
//  Topology:
//    - 2 logical pickers (0,1).
//    - physical sucker mapping: logical i -> (row=0, col=i).
//    - target field: 2 rows x 2 cols (rowMax=2, colMax=2).
//
//  Slot computation: each non-lead picker shares the lead's row, and its
//  target col = leadCol + (logical - leadLogical), clamped semantics are NOT
//  applied -- we simply return a col that may be out of the candidate set so
//  IsTargetCandidate() does the filtering.  X/Y are leadCol*10 + logical.
//
//  Candidate map (which (row,col) cells "have an IC" / are valid targets):
//      (row=0,col=0) = true
//      (row=0,col=1) = true
//      (row=1,col=0) = false
//      (row=1,col=1) = true
//
//  Demand: both logical pickers demanded.
//  Reachable: all reachable.
//
//  => The plan led by (lead=0,row=0,col=0):
//        logical0 -> target(0,0) candidate=true  -> active
//        logical1 -> target(0,1) candidate=true  -> active
//        iActiveCount = 2   (the maximum possible)
//  This should be the winning plan; GetActiveCount()==2.
// ===========================================================================
class StubContext : public IArmPickPlanContext
{
public:
    int  pickerCount;
    int  rowMax;
    int  colMax;
    bool allReachable;
    bool allDemanded;

    StubContext()
        : pickerCount(2), rowMax(2), colMax(2),
          allReachable(true), allDemanded(true) {}

    virtual int GetLogicalPickerCount() { return pickerCount; }

    virtual bool GetPhysicalSucker(int iLogical, int& rOutRow, int& rOutCol)
    {
        if (iLogical < 0 || iLogical >= pickerCount) return false;
        rOutRow = 0;
        rOutCol = iLogical;
        return true;
    }

    virtual int GetTargetRowMax() { return rowMax; }
    virtual int GetTargetColMax() { return colMax; }

    virtual bool ComputeSlot(int iLogical,
                             int iLeadLogical,
                             int iLeadTargetRow,
                             int iLeadTargetCol,
                             int& rOutTargetRow,
                             int& rOutTargetCol,
                             int& rOutX,
                             int& rOutY)
    {
        rOutTargetRow = iLeadTargetRow;
        rOutTargetCol = iLeadTargetCol + (iLogical - iLeadLogical);
        rOutX = iLeadTargetCol * 10 + iLogical;
        rOutY = iLeadTargetRow * 10 + iLogical;
        return true;
    }

    virtual bool IsReachable(int /*iX*/, int /*iY*/) { return allReachable; }

    virtual bool IsTargetCandidate(int iTargetRow, int iTargetCol)
    {
        // valid cells: (0,0),(0,1),(1,1)  ; everything else false
        if (iTargetRow == 0 && (iTargetCol == 0 || iTargetCol == 1)) return true;
        if (iTargetRow == 1 && iTargetCol == 1) return true;
        return false;
    }

    virtual bool IsSuckerDemanded(int /*iLogical*/) { return allDemanded; }
};

// A context that demands only ONE sucker, to verify scoring picks the
// max-active plan and that a single-active plan is still "found".
class SingleDemandContext : public StubContext
{
public:
    virtual bool IsSuckerDemanded(int iLogical) { return iLogical == 0; }
};

// A context whose IsLeadValid only accepts lead at (0,0,0), to prove the
// pre-filter short-circuits all other leads (cArmPickPlan.cpp:122).
class LeadFilterContext : public StubContext
{
public:
    virtual bool IsLeadValid(int iLeadLogical, int iLeadTargetRow, int iLeadTargetCol)
    {
        return (iLeadLogical == 0 && iLeadTargetRow == 0 && iLeadTargetCol == 0);
    }
};

int main()
{
    // =====================================================================
    //  1. Generic engine: full 2-active best plan
    // =====================================================================
    {
        StubContext ctx;
        cArmPickPlan engine;
        engine.Init(&ctx);
        bool found = engine.Search();

        CHECK_BOOL(found, true);
        CHECK_BOOL(engine.Found(), true);
        CHECK_INT(engine.GetActiveCount(), 2);    // both pickers active

        const TGenericPickPlan& plan = engine.GetPlan();
        CHECK_INT(plan.iActiveCount, 2);
        CHECK_INT(plan.iSlotCount, 2);

        // The winning lead must be one that yields 2 active slots.
        // lead=(0,row=0,col=0): logical0->(0,0)ok, logical1->(0,1)ok => 2.
        // Because scoring uses strict '>', the FIRST lead (enumeration order
        // lead,row,col) that reaches the max count wins and is never replaced.
        // Enumeration order hits (lead=0,row=0,col=0) first among the 2-active
        // candidates, so that is the recorded best.
        CHECK_INT(plan.iLeadLogical,   0);
        CHECK_INT(plan.iLeadTargetRow, 0);
        CHECK_INT(plan.iLeadTargetCol, 0);

        // Slot 0: logical0 -> physical (row0,col0), target (0,0), active
        CHECK_INT(plan.Slots[0].iLogical,   0);
        CHECK_INT(plan.Slots[0].iPhysRow,   0);
        CHECK_INT(plan.Slots[0].iPhysCol,   0);
        CHECK_INT(plan.Slots[0].iTargetRow, 0);
        CHECK_INT(plan.Slots[0].iTargetCol, 0);
        CHECK_BOOL(plan.Slots[0].bActive,   true);

        // Slot 1: logical1 -> physical (row0,col1), target (0,1), active
        CHECK_INT(plan.Slots[1].iLogical,   1);
        CHECK_INT(plan.Slots[1].iPhysRow,   0);
        CHECK_INT(plan.Slots[1].iPhysCol,   1);
        CHECK_INT(plan.Slots[1].iTargetRow, 0);
        CHECK_INT(plan.Slots[1].iTargetCol, 1);
        CHECK_BOOL(plan.Slots[1].bActive,   true);

        // Slot X/Y faithfully computed: lead col=0 -> X = 0*10 + logical
        CHECK_INT(plan.Slots[0].iX, 0);
        CHECK_INT(plan.Slots[1].iX, 1);
    }

    // =====================================================================
    //  2. Single-demand context: best plan has exactly 1 active slot
    // =====================================================================
    {
        SingleDemandContext ctx;
        cArmPickPlan engine;
        engine.Init(&ctx);
        bool found = engine.Search();

        CHECK_BOOL(found, true);              // 1 active >= 1 -> found
        CHECK_INT(engine.GetActiveCount(), 1);

        const TGenericPickPlan& plan = engine.GetPlan();
        // Only logical0 is demanded; in the winning plan slot0 active, slot1 not.
        CHECK_BOOL(plan.Slots[0].bActive, true);
        CHECK_BOOL(plan.Slots[1].bActive, false);
    }

    // =====================================================================
    //  3. Lead pre-filter honoured (IsLeadValid)
    // =====================================================================
    {
        LeadFilterContext ctx;
        cArmPickPlan engine;
        engine.Init(&ctx);
        bool found = engine.Search();

        // Only (lead=0,row=0,col=0) is evaluated -> 2 active.
        CHECK_BOOL(found, true);
        CHECK_INT(engine.GetActiveCount(), 2);
        const TGenericPickPlan& plan = engine.GetPlan();
        CHECK_INT(plan.iLeadLogical,   0);
        CHECK_INT(plan.iLeadTargetRow, 0);
        CHECK_INT(plan.iLeadTargetCol, 0);
    }

    // =====================================================================
    //  4. Boundary guards (cArmPickPlan.cpp:105-116)
    // =====================================================================
    {
        // null ctx
        cArmPickPlan engine;
        CHECK_BOOL(engine.Search(), false);   // m_pCtx == 0 -> false
        CHECK_BOOL(engine.Found(), false);
        CHECK_INT(engine.GetActiveCount(), 0);
    }
    {
        // zero pickers
        StubContext ctx; ctx.pickerCount = 0;
        cArmPickPlan engine; engine.Init(&ctx);
        CHECK_BOOL(engine.Search(), false);
    }
    {
        // zero rowMax
        StubContext ctx; ctx.rowMax = 0;
        cArmPickPlan engine; engine.Init(&ctx);
        CHECK_BOOL(engine.Search(), false);
    }
    {
        // zero colMax
        StubContext ctx; ctx.colMax = 0;
        cArmPickPlan engine; engine.Init(&ctx);
        CHECK_BOOL(engine.Search(), false);
    }
    {
        // pickers > ARM_PICK_PLAN_MAX_SLOTS (8) -> reject
        StubContext ctx; ctx.pickerCount = ARM_PICK_PLAN_MAX_SLOTS + 1;
        cArmPickPlan engine; engine.Init(&ctx);
        CHECK_BOOL(engine.Search(), false);
    }
    {
        // no reachable -> no active -> not found
        StubContext ctx; ctx.allReachable = false;
        cArmPickPlan engine; engine.Init(&ctx);
        CHECK_BOOL(engine.Search(), false);
        CHECK_INT(engine.GetActiveCount(), 0);
    }
    {
        // no demand -> no active -> not found
        StubContext ctx; ctx.allDemanded = false;
        cArmPickPlan engine; engine.Init(&ctx);
        CHECK_BOOL(engine.Search(), false);
        CHECK_INT(engine.GetActiveCount(), 0);
    }

    // =====================================================================
    //  5. cInArmLoaderPickPlanner scaffold (Phase 3): zero side effects
    // =====================================================================
    {
        CHECK_BOOL(bUseLoaderPickPlanner, false);   // runtime gate default OFF

        cInArmLoaderPickPlanner planner;
        planner.Init();
        CHECK_BOOL(planner.Search(), false);
        CHECK_BOOL(planner.Found(), false);
        CHECK_INT(planner.GetActiveCount(), 0);
        CHECK_BOOL(planner.IsAuthoritative(), false);
        planner.Apply();   // no-op; must not crash

        // shared global instance behaves identically
        g_LoaderPlan.Init();
        CHECK_BOOL(g_LoaderPlan.Search(), false);
        CHECK_INT(g_LoaderPlan.GetActiveCount(), 0);

        // stub virtuals return documented defaults
        CHECK_INT(planner.GetLogicalPickerCount(), 0);
        CHECK_INT(planner.GetTargetRowMax(), 0);
        CHECK_INT(planner.GetTargetColMax(), 0);
        CHECK_BOOL(planner.IsReachable(0, 0), true);            // Loader: no soft-limit
        CHECK_BOOL(planner.IsTargetCandidate(0, 0), false);
        CHECK_BOOL(planner.IsSuckerDemanded(0), false);
        int r = 99, c = 99;
        CHECK_BOOL(planner.GetPhysicalSucker(0, r, c), false);
        CHECK_INT(r, -1);
        CHECK_INT(c, -1);
    }

    // =====================================================================
    //  6. cOutArmPlacePlanner scaffold (Phase 4): zero side effects
    // =====================================================================
    {
        CHECK_BOOL(bUseOutArmPlacePlanner, false);  // runtime gate default OFF

        cOutArmPlacePlanner planner;
        planner.Init();
        CHECK_BOOL(planner.Search(), false);
        CHECK_BOOL(planner.Found(), false);
        CHECK_INT(planner.GetActiveCount(), 0);
        CHECK_BOOL(planner.IsAuthoritative(), false);
        planner.Apply();   // no-op; must not crash

        g_OutArmPlacePlan.Init();
        CHECK_BOOL(g_OutArmPlacePlan.Search(), false);
        CHECK_INT(g_OutArmPlacePlan.GetActiveCount(), 0);

        CHECK_INT(planner.GetLogicalPickerCount(), 0);
        CHECK_INT(planner.GetTargetRowMax(), 0);
        CHECK_INT(planner.GetTargetColMax(), 0);
        CHECK_BOOL(planner.IsReachable(0, 0), true);
        CHECK_BOOL(planner.IsTargetCandidate(0, 0), false);
        CHECK_BOOL(planner.IsSuckerDemanded(0), false);
    }

    // =====================================================================
    //  7. Search() not initialized -> false (scaffold m_bInitialized guard)
    // =====================================================================
    {
        cInArmLoaderPickPlanner planner;   // not Init()'d
        CHECK_BOOL(planner.Search(), false);
    }

    // =====================================================================
    //  Summary
    // =====================================================================
    if (g_fail == 0) {
        std::printf("PASS  %d/%d tests passed\n", g_total, g_total);
        return 0;
    } else {
        std::printf("FAIL  %d/%d tests failed\n", g_fail, g_total);
        return 1;
    }
}
