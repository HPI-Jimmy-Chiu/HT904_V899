//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260514: OutArm Place planner (Phase 4 scaffold)
//
// cOutArmPlacePlanner
//   Second adapter pattern instance, targeting OutArm place-to-Auto-tray.
//   Phase 4 ships SCAFFOLD ONLY:
//     - Class compiles, registered in HT9045.bpr
//     - NOT wired into any caller (no #include in aoutarm9045.cpp)
//     - bUseOutArmPlacePlanner default false; even if dev flips it,
//       no production code path can reach this class
//   Rationale: aoutarm9045.cpp has no clean Loader-style single-entry search
//   function; correct integration point requires Phase 4.1 design. Keeping
//   the scaffold compilable but unreachable matches the "default off /
//   zero production impact" rule from ADR-0005.
//
// Future Phase 4.1 work (NOT in this commit):
//   - Decide integration point inside SearchTrayToPlace_9045 or
//     DoOutArmPlaceToAuto_9045 case 10
//   - Implement IArmPickPlanContext virtuals against MOT[MMAutoX].Tray.Data
//     and OutArmSuck.Item / OutArmSuckUse
//   - Apply() writes iXPosition[]/iYPosition/iAutoPitchStepY
//---------------------------------------------------------------------------
#ifndef cOutArmPlacePlannerH
#define cOutArmPlacePlannerH

#include "cArmPickPlan.h"

//AI(ht9045-v899) 20260514: runtime gate, default OFF. Currently unreachable
// because no caller invokes g_OutArmPlacePlan. Reserved for Phase 4.1.
extern bool bUseOutArmPlacePlanner;

class cOutArmPlacePlanner : public IArmPickPlanContext
{
private:
    cArmPickPlan m_Engine;
    bool         m_bInitialized;
    int          m_iLastActiveCount;

public:
    cOutArmPlacePlanner();

    // Phase 4 lifecycle (no callers yet)
    void Init();
    bool Search();
    void Apply();        // no-op until Phase 4.1
    bool Found() const                  { return m_iLastActiveCount > 0; }
    int  GetActiveCount() const         { return m_iLastActiveCount; }

    //AI(ht9045-v899) 20260514: explicit gate. Returns false until Phase 4.1
    // marks Apply() as production-ready.
    bool IsAuthoritative() const        { return false; }

    //--- IArmPickPlanContext (Phase 4 stubs; Phase 4.1 will fill in) -------
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

extern cOutArmPlacePlanner g_OutArmPlacePlan;

#endif
