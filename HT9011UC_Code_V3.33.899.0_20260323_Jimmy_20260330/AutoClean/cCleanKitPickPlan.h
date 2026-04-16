//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260407: new class to replace SearchCleanKitUpDown search/plan/assign logic
//
// cCleanKitPickPlan
//   Enumerates all valid (startLogical, kitCol, kitRow) combinations,
//   computes every sucker's X position vs soft limits and pad availability,
//   and selects the plan that maximizes active sucker count.
//
//   Eliminates the error-prone iAutoCleanStart + soft-limit patching chain.
//
// Usage (in DoAutoCleanPickfromCleanKit Task 20):
//   cCleanKitPickPlan plan;
//   plan.Init(iSht, iShuttleRowKit);
//   if(plan.Search())
//       plan.Apply();    // sets iAutoCleanStart, iAutoCleanPickPlateX/Y, bInArmSuckActive
//---------------------------------------------------------------------------
#ifndef cCleanKitPickPlanH
#define cCleanKitPickPlanH

//AI(ht9045-v899) 20260407: runtime flag to switch between new class and old logic
extern bool bUseCKPP;   // true=new cCleanKitPickPlan, false=old SearchCleanKitUpDown

//--- max logical pickers per search ---
#define CKPP_MAX_SLOTS 4

struct TPickSlot
{
    int iLogical;      // logical picker index (0..n-1)
    int iPhysical;     // physical sucker column from GetAutoCleanPickStep()
    int iKitCol;       // target Kit Tray column
    int iXPos;         // calculated motor X position
    bool bReachable;   // X within [SoftLimitN, SoftLimitP]
    bool bHasPad;      // Kit has HAS_CLEAN_IC at (iKitCol, kitRow)
    bool bDemanded;    // shuttle demands this sucker (empty and mapped)
    bool bActive;      // bReachable && bHasPad && bDemanded
};

struct TPickPlan
{
    TPickSlot Slots[CKPP_MAX_SLOTS];
    int iSlotCount;         // number of logical pickers
    int iStartLogical;      // which logical index leads the pick
    int iKitCol;            // starting Kit column for the lead sucker
    int iKitRow;            // Kit row
    int iActiveCount;       // number of active slots
};

class cCleanKitPickPlan
{
private:
    // cached parameters
    int m_iSht;             // eWhichShuttle (0 or 1)
    int m_iShuttleRow;      // shuttle row for auto clean
    int m_iPickerCount;     // GetAutoCleanPickCount()
    int m_iXPitchStep;      // iAutoCleanUseXPitch
    double m_dMovePitchX;   // physical sucker pitch per base unit
    double m_dKitXPitch;    // TestIF_File.dAutoClean_XPitch
    int m_iBaseX;           // Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]
    int m_iSLimN;           // MOT[MInArmX].Motor->PSoftLimitN
    int m_iSLimP;           // MOT[MInArmX].Motor->PSoftLimitP
    int m_iHPXOfs;          // HotplatlXOffset
    int m_iXItem;           // Kit tray column count
    int m_iYItem;           // Kit tray row count
    int m_iRealRow;         // sucker real row for bInArmSuckActive
    int m_iSuckRow;         // picker order row
    int m_iKitStep;         // kit step offset

    // shuttle demand cache (indexed by physical sucker col)
    bool m_bDemand[8];
    bool m_bRowHasIC;       // any sucker already has IC (second-pass)?

    // best plan
    TPickPlan m_Best;
    bool m_bFound;

    // helpers
    int  CalcX(int iPhysicalSucker, int iKitCol);
    //AI(ht9045-v899) 20260410: Y position calculation for cross-verification
    int  CalcY(int iKitRow);
    bool IsPadAt(int iKitCol, int iKitRow);
    void BuildDemand();
    void EvaluatePlan(int startL, int kCol, int kRow);

public:
    cCleanKitPickPlan();

    void Init(int iSht, int iShuttleRow);
    bool Search();     // find best plan; returns true if at least 1 reachable pad
    void Apply();      // set globals: iAutoCleanStart, iAutoCleanPickPlateX/Y, bInArmSuckActive

    int  GetStartLogical()  { return m_Best.iStartLogical; }
    int  GetKitCol()        { return m_Best.iKitCol; }
    int  GetKitRow()        { return m_Best.iKitRow; }
    int  GetActiveCount()   { return m_Best.iActiveCount; }
    bool Found()            { return m_bFound; }
    //AI(ht9045-v899) 20260408: check if current shuttle group has sucker demand
    bool HasDemand();
    //AI(ht9045-v899) 20260408: accessors for downstream plan-driven functions
    int  GetSlotCount()     { return m_Best.iSlotCount; }
    int  GetRealRow()       { return m_iRealRow; }
    const TPickSlot& GetSlot(int i) { return m_Best.Slots[i]; }
    const TPickPlan& GetPlan()      { return m_Best; }
    //AI(ht9045-v899) 20260410: cross-verify motor position vs plan-calculated position per slot
    bool VerifyPosition(int iSlotIdx, int iTolerance, AnsiString &sDetail);
    bool VerifyAllSlots(int iTolerance);
};

//AI(ht9045-v899) 20260408: global plan instance shared across pick/move/check functions
extern cCleanKitPickPlan g_CKPlan;

#endif
