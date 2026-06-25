// BCB6 provenance: HT9011UC_Code_V3.33.906.0_20260618/Common/PickPlanner/cArmPickPlan.cpp
//---------------------------------------------------------------------------
// Migration W2: translated to standard C++17 (MinGW g++ 6.3).
//   - Replaced <vcl.h> with "vclcompat/vcl_compat.h"
//   - Removed #pragma hdrstop, #pragma package(smart_init)
//   - All names, logic, and numeric conventions preserved verbatim
//   - No STL / C++11 / AnsiString introduced; BCB6-compatible semantics kept
//
//AI(ht9045-v899) 20260514: generic Arm pick/place planner implementation
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"

#include "cArmPickPlan.h"

//---------------------------------------------------------------------------
cArmPickPlan::cArmPickPlan()
{
    m_pCtx = 0;
    m_bFound = false;
    ResetPlan(m_Best);
}
//---------------------------------------------------------------------------
void cArmPickPlan::ResetPlan(TGenericPickPlan& rPlan)
{
    int i;
    for(i = 0; i < ARM_PICK_PLAN_MAX_SLOTS; i++)
    {
        rPlan.Slots[i].iLogical    = -1;
        rPlan.Slots[i].iPhysRow    = -1;
        rPlan.Slots[i].iPhysCol    = -1;
        rPlan.Slots[i].iTargetRow  = -1;
        rPlan.Slots[i].iTargetCol  = -1;
        rPlan.Slots[i].iX          = 0;
        rPlan.Slots[i].iY          = 0;
        rPlan.Slots[i].bReachable  = false;
        rPlan.Slots[i].bTargetOK   = false;
        rPlan.Slots[i].bDemanded   = false;
        rPlan.Slots[i].bActive     = false;
    }
    rPlan.iSlotCount     = 0;
    rPlan.iLeadLogical   = -1;
    rPlan.iLeadTargetRow = -1;
    rPlan.iLeadTargetCol = -1;
    rPlan.iActiveCount   = 0;
}
//---------------------------------------------------------------------------
void cArmPickPlan::Init(IArmPickPlanContext* pCtx)
{
    m_pCtx = pCtx;
    m_bFound = false;
    ResetPlan(m_Best);
}
//---------------------------------------------------------------------------
void cArmPickPlan::EvaluatePlan(int iLeadLogical, int iLeadTargetRow, int iLeadTargetCol)
{
    if(m_pCtx == 0) return;

    TGenericPickPlan plan;
    ResetPlan(plan);

    int iCount = m_pCtx->GetLogicalPickerCount();
    if(iCount <= 0 || iCount > ARM_PICK_PLAN_MAX_SLOTS) return;

    plan.iSlotCount     = iCount;
    plan.iLeadLogical   = iLeadLogical;
    plan.iLeadTargetRow = iLeadTargetRow;
    plan.iLeadTargetCol = iLeadTargetCol;

    int i;
    for(i = 0; i < iCount; i++)
    {
        TGenericPickSlot& s = plan.Slots[i];
        s.iLogical = i;

        int iPR = -1, iPC = -1;
        if(!m_pCtx->GetPhysicalSucker(i, iPR, iPC))
            continue;
        s.iPhysRow = iPR;
        s.iPhysCol = iPC;

        int iTR = -1, iTC = -1, iX = 0, iY = 0;
        if(!m_pCtx->ComputeSlot(i, iLeadLogical, iLeadTargetRow, iLeadTargetCol,
                                iTR, iTC, iX, iY))
            continue;
        s.iTargetRow = iTR;
        s.iTargetCol = iTC;
        s.iX         = iX;
        s.iY         = iY;
        s.bReachable = m_pCtx->IsReachable(iX, iY);
        s.bTargetOK  = m_pCtx->IsTargetCandidate(iTR, iTC);
        s.bDemanded  = m_pCtx->IsSuckerDemanded(i);
        s.bActive    = (s.bReachable && s.bTargetOK && s.bDemanded);
        if(s.bActive)
            plan.iActiveCount++;
    }

    if(plan.iActiveCount > m_Best.iActiveCount)
    {
        m_Best  = plan;
        m_bFound = (m_Best.iActiveCount > 0);
    }
}
//---------------------------------------------------------------------------
bool cArmPickPlan::Search()
{
    m_bFound = false;
    ResetPlan(m_Best);

    if(m_pCtx == 0) return false;

    int iPickers = m_pCtx->GetLogicalPickerCount();
    int iRowMax  = m_pCtx->GetTargetRowMax();
    int iColMax  = m_pCtx->GetTargetColMax();

    if(iPickers <= 0 || iRowMax <= 0 || iColMax <= 0) return false;
    if(iPickers > ARM_PICK_PLAN_MAX_SLOTS) return false;

    int iLead, iRow, iCol;
    for(iLead = 0; iLead < iPickers; iLead++)
    {
        for(iRow = 0; iRow < iRowMax; iRow++)
        {
            for(iCol = 0; iCol < iColMax; iCol++)
            {
                //AI(ht9045-v899) 20260514: respect adapter's optional lead pre-filter
                if(!m_pCtx->IsLeadValid(iLead, iRow, iCol)) continue;
                EvaluatePlan(iLead, iRow, iCol);
            }
        }
    }
    return m_bFound;
}
//---------------------------------------------------------------------------
