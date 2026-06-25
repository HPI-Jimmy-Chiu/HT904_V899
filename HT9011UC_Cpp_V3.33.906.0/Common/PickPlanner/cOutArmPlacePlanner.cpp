// BCB6 provenance: HT9011UC_Code_V3.33.906.0_20260618/Common/PickPlanner/cOutArmPlacePlanner.cpp
//---------------------------------------------------------------------------
// Migration W2: translated to standard C++17 (MinGW g++ 6.3).
//   - Replaced <vcl.h> with "vclcompat/vcl_compat.h"
//   - Removed #pragma hdrstop, #pragma package(smart_init)
//   - All names, logic, and numeric conventions preserved verbatim
//   - No hardware globals / VCL forms reached; leaf coupling
//
//AI(ht9045-v899) 20260514: cOutArmPlacePlanner Phase 4 scaffold
//
// All IArmPickPlanContext methods return safe defaults that cause
// cArmPickPlan::Search() to bail early (GetLogicalPickerCount() == 0).
// This translation unit compiles but no caller invokes g_OutArmPlacePlan,
// so the class is dead code in this revision.
// Phase 4.1 will choose the integration point and fill in OutArm-tray /
// OutArmSuck logic.
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"

#include "cOutArmPlacePlanner.h"

//AI(ht9045-v899) 20260514: runtime gate; reserved for Phase 4.1
bool bUseOutArmPlacePlanner = false;

//AI(ht9045-v899) 20260514: shared global instance (currently unreachable)
cOutArmPlacePlanner g_OutArmPlacePlan;

//---------------------------------------------------------------------------
cOutArmPlacePlanner::cOutArmPlacePlanner()
{
    m_bInitialized     = false;
    m_iLastActiveCount = 0;
}
//---------------------------------------------------------------------------
void cOutArmPlacePlanner::Init()
{
    m_bInitialized     = true;
    m_iLastActiveCount = 0;
    m_Engine.Init(this);
}
//---------------------------------------------------------------------------
bool cOutArmPlacePlanner::Search()
{
    if(!m_bInitialized) return false;
    bool bFound = m_Engine.Search();
    m_iLastActiveCount = bFound ? m_Engine.GetActiveCount() : 0;
    return bFound;
}
//---------------------------------------------------------------------------
void cOutArmPlacePlanner::Apply()
{
    //AI(ht9045-v899) 20260514: Phase 4 scaffold - no globals are written.
    // Phase 4.1 will set iXPosition[]/iYPosition/iAutoPitchStepY/etc.
}
//---------------------------------------------------------------------------
//=== IArmPickPlanContext stubs ============================================
// Returning 0 pickers makes cArmPickPlan::Search() return false immediately,
// guaranteeing zero side effects from this Phase 4 scaffold.
//---------------------------------------------------------------------------
int cOutArmPlacePlanner::GetLogicalPickerCount()
{
    return 0;                                                                   //AI(ht9045-v899) 20260514: Phase 4.1 -> active sucker count from OutArmSuckUse
}
//---------------------------------------------------------------------------
bool cOutArmPlacePlanner::GetPhysicalSucker(int /*iLogical*/, int& rOutRow, int& rOutCol)
{
    rOutRow = -1;
    rOutCol = -1;
    return false;
}
//---------------------------------------------------------------------------
int cOutArmPlacePlanner::GetTargetRowMax()
{
    return 0;                                                                   //AI(ht9045-v899) 20260514: Phase 4.1 -> MOT[MMAutoX].Tray.YItem
}
//---------------------------------------------------------------------------
int cOutArmPlacePlanner::GetTargetColMax()
{
    return 0;                                                                   //AI(ht9045-v899) 20260514: Phase 4.1 -> MOT[MMAutoX].Tray.XItem
}
//---------------------------------------------------------------------------
bool cOutArmPlacePlanner::ComputeSlot(int /*iLogical*/,
                                      int /*iLeadLogical*/,
                                      int /*iLeadTargetRow*/,
                                      int /*iLeadTargetCol*/,
                                      int& rOutTargetRow,
                                      int& rOutTargetCol,
                                      int& rOutX,
                                      int& rOutY)
{
    rOutTargetRow = -1;
    rOutTargetCol = -1;
    rOutX         = 0;
    rOutY         = 0;
    return false;
}
//---------------------------------------------------------------------------
bool cOutArmPlacePlanner::IsReachable(int /*iX*/, int /*iY*/)
{
    return true;
}
//---------------------------------------------------------------------------
bool cOutArmPlacePlanner::IsTargetCandidate(int /*iTargetRow*/, int /*iTargetCol*/)
{
    return false;
}
//---------------------------------------------------------------------------
bool cOutArmPlacePlanner::IsSuckerDemanded(int /*iLogical*/)
{
    return false;
}
//---------------------------------------------------------------------------
