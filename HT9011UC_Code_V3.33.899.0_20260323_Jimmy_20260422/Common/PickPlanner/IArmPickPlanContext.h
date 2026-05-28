//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260514: generic Arm pick/place planner context interface
//
// IArmPickPlanContext
//   Pure-virtual contract that lets cArmPickPlan run a generic
//   "enumerate every (leadLogical, targetRow, targetCol) combination,
//    score by active sucker count, return the best plan" algorithm
//   without knowing about AutoClean / Loader / HotPlate / OutArm specifics.
//
// Each region (AutoClean / InArm-Loader / InArm-HotPlate / OutArm-Place)
// supplies its own concrete adapter implementing this interface.
//
// BCB6 friendly: no STL, no C++11, no AnsiString in the contract surface.
//---------------------------------------------------------------------------
#ifndef IArmPickPlanContextH
#define IArmPickPlanContextH

class IArmPickPlanContext
{
public:
    virtual ~IArmPickPlanContext() {}

    //--- topology ----------------------------------------------------------
    // logical picker count (e.g. InArm 4-picker = 4, 8-picker = 8)
    virtual int  GetLogicalPickerCount() = 0;

    // map logical picker -> physical sucker (row, col) inside TMyKitSuck
    // returns false if the logical index is invalid
    virtual bool GetPhysicalSucker(int iLogical, int& rOutRow, int& rOutCol) = 0;

    //--- target field bounds (tray cells / kit cells) ----------------------
    virtual int  GetTargetRowMax() = 0;
    virtual int  GetTargetColMax() = 0;

    //--- per-slot evaluation -----------------------------------------------
    // For a candidate plan led by iLeadLogical sitting at (iLeadTargetRow,
    // iLeadTargetCol), compute the (X, Y) the iLogical picker would reach.
    // Returns false to mark the slot unreachable / invalid.
    virtual bool ComputeSlot(int iLogical,
                             int iLeadLogical,
                             int iLeadTargetRow,
                             int iLeadTargetCol,
                             int& rOutTargetRow,
                             int& rOutTargetCol,
                             int& rOutX,
                             int& rOutY) = 0;

    // soft-limit / mechanical reachability check on the computed (X, Y)
    virtual bool IsReachable(int iX, int iY) = 0;

    // is the target cell a valid pick/place candidate?
    // (e.g. HAS_CLEAN_IC for AutoClean, HAS_IC for Loader, NULL_IC for OutArm)
    virtual bool IsTargetCandidate(int iTargetRow, int iTargetCol) = 0;

    // is this logical picker actually demanded?
    // (e.g. shuttle has demand for it, site enabled, sucker not disabled)
    virtual bool IsSuckerDemanded(int iLogical) = 0;

    //AI(ht9045-v899) 20260514: optional pre-filter on a candidate lead.
    // Default true = no pre-filter (cArmPickPlan evaluates every (lead, row, col)).
    // Adapters that want to preserve legacy "skip when lead has no target /
    // lead unreachable" semantics override this.
    virtual bool IsLeadValid(int /*iLeadLogical*/,
                             int /*iLeadTargetRow*/,
                             int /*iLeadTargetCol*/) { return true; }
};

#endif
