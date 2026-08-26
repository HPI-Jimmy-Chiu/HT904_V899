//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260514: InArm Loader pick planner (Phase 3 scaffold)
//
// cInArmLoaderPickPlanner
//   First production adapter on cArmPickPlan, targeting InArm Loader pick.
//   Phase 3 ships SCAFFOLD ONLY:
//     - Class compiles, Init/Search/Apply exist and are callable
//     - Search() returns false (no plan produced) until Phase 3.1 fills
//       in the Loader-specific tray + sucker mapping
//     - Apply() is a no-op (writes nothing back to legacy globals)
//   Wired into SearchAndMoveInArmXYToLoad_9045() under bUseLoaderPickPlanner
//   guard so that:
//     - flag=false (default) -> planner is NEVER touched, legacy path 100% intact
//     - flag=true            -> planner runs in shadow mode for log/diff,
//                               legacy path still drives real motion
//
// Future Phase 3.1 work (NOT in this commit):
//   - Implement IArmPickPlanContext virtuals against MOT[MMTrayY].Tray.Data
//     and InArmSuck.Item / InArmSuckUse
//   - Apply() writes iXPosition[]/iYPosition/iLoadPitchStepY/bCanPick2ICAtOnceTime
//   - Authoritative cut-over of SearchAndMoveInArmXYToLoad_9045
//---------------------------------------------------------------------------
#ifndef cInArmLoaderPickPlannerH
#define cInArmLoaderPickPlannerH

#include "cArmPickPlan.h"

//AI(ht9045-v899) 20260514: runtime gate, default OFF. Toggle to true ONLY in
// dev environment to capture shadow logs from g_LoaderPlan.
extern bool bUseLoaderPickPlanner;

class cInArmLoaderPickPlanner : public IArmPickPlanContext
{
private:
    cArmPickPlan m_Engine;
    bool         m_bInitialized;
    int          m_iLastActiveCount;

public:
    cInArmLoaderPickPlanner();

    // Phase 3 lifecycle (legacy callers must NOT depend on outputs yet)
    void Init();
    bool Search();
    void Apply();        // no-op until Phase 3.1
    bool Found() const                  { return m_iLastActiveCount > 0; }
    int  GetActiveCount() const         { return m_iLastActiveCount; }

    //AI(ht9045-v899) 20260514: explicit gate. Returns false until Phase 3.1
    // marks Apply() as production-ready. Callers should treat planner output
    // as advisory until this returns true.
    bool IsAuthoritative() const        { return false; }

    //--- IArmPickPlanContext (Phase 3 stubs; Phase 3.1 will fill in) -------
    virtual int  GetLogicalPickerCount();
    virtual bool GetPhysicalSucker(int iLogical, int& rOutRow, int& rOutCol);
    virtual int  GetTargetRowMax();
    virtual int  GetTargetColMax();
    virtual bool ComputeSlot(int iLogical,
                             int iLeadLogical,
                             int iLeadTargetRow,
                             int iLeadTargetCol,
                             int& rOutTargetRow,
                             int& rOutTargetCol,
                             int& rOutX,
                             int& rOutY);
    virtual bool IsReachable(int iX, int iY);
    virtual bool IsTargetCandidate(int iTargetRow, int iTargetCol);
    virtual bool IsSuckerDemanded(int iLogical);
};

extern cInArmLoaderPickPlanner g_LoaderPlan;

#endif
