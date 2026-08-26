//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260514: generic enumerate-and-score Arm pick/place planner
//
// cArmPickPlan
//   Region-agnostic planner. Enumerates all candidate plans
//   (leadLogical x leadTargetRow x leadTargetCol), evaluates every slot
//   through an IArmPickPlanContext, and remembers the plan with the
//   highest active-sucker count.
//
//   This is the shared core extracted from the original
//   cCleanKitPickPlan (AutoClean) so that future Loader / HotPlate /
//   OutArm planners can share the same enumeration + scoring logic.
//
// Lifecycle:
//   1) Init(pCtx)        -- caller keeps ownership of pCtx
//   2) Search()          -- runs enumeration; returns true if any plan found
//   3) Found() / GetPlan() -- inspect result
//
// BCB6 friendly: no STL, no C++11, no AnsiString.
//---------------------------------------------------------------------------
#ifndef cArmPickPlanH
#define cArmPickPlanH

#include "IArmPickPlanContext.h"

//--- mirrors CKPP_MAX_SLOTS in cCleanKitPickPlan.h ---
#ifndef ARM_PICK_PLAN_MAX_SLOTS
#define ARM_PICK_PLAN_MAX_SLOTS 8
#endif

struct TGenericPickSlot
{
    int  iLogical;       // logical picker index (0..n-1)
    int  iPhysRow;       // physical sucker row inside TMyKitSuck.Suck[][]
    int  iPhysCol;       // physical sucker col inside TMyKitSuck.Suck[][]
    int  iTargetRow;     // target tray/kit row
    int  iTargetCol;     // target tray/kit col
    int  iX;             // computed X (raw motor units, region defined)
    int  iY;             // computed Y (raw motor units, region defined)
    bool bReachable;     // passes soft-limit / mechanical check
    bool bTargetOK;      // target cell is a valid candidate
    bool bDemanded;      // sucker is needed by upstream (shuttle / site)
    bool bActive;        // final: reachable && bTargetOK && bDemanded
};

struct TGenericPickPlan
{
    TGenericPickSlot Slots[ARM_PICK_PLAN_MAX_SLOTS];
    int iSlotCount;          // == GetLogicalPickerCount()
    int iLeadLogical;        // which logical picker drove this plan
    int iLeadTargetRow;      // lead's target row
    int iLeadTargetCol;      // lead's target col
    int iActiveCount;        // sum of bActive slots
};

class cArmPickPlan
{
private:
    IArmPickPlanContext* m_pCtx;
    TGenericPickPlan     m_Best;
    bool                 m_bFound;

    void EvaluatePlan(int iLeadLogical, int iLeadTargetRow, int iLeadTargetCol);
    void ResetPlan(TGenericPickPlan& rPlan);

public:
    cArmPickPlan();

    // pCtx is borrowed (NOT owned). Caller keeps it alive across Search().
    void Init(IArmPickPlanContext* pCtx);

    // Returns true if at least one plan with iActiveCount >= 1 was found.
    bool Search();

    bool                    Found()         const { return m_bFound; }
    const TGenericPickPlan& GetPlan()       const { return m_Best;   }
    int                     GetActiveCount()const { return m_Best.iActiveCount; }
};

#endif
