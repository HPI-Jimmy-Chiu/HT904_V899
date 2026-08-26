//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260514: cInArmLoaderPickPlanner Phase 3 scaffold
//
// All IArmPickPlanContext methods return safe defaults that cause
// cArmPickPlan::Search() to bail early (GetLogicalPickerCount() == 0),
// so Found()/GetActiveCount() always return 0 in this commit.
// Phase 3.1 will replace stubs with real Loader-tray / InArmSuck logic.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "cInArmLoaderPickPlanner.h"

#pragma package(smart_init)

//AI(ht9045-v899) 20260514: runtime gate; MUST stay false in production
bool bUseLoaderPickPlanner = false;

//AI(ht9045-v899) 20260514: shared global instance used by the dual-path hook
cInArmLoaderPickPlanner g_LoaderPlan;

//---------------------------------------------------------------------------
cInArmLoaderPickPlanner::cInArmLoaderPickPlanner()
{
    m_bInitialized     = false;
    m_iLastActiveCount = 0;
}
//---------------------------------------------------------------------------
void cInArmLoaderPickPlanner::Init()
{
    m_bInitialized     = true;
    m_iLastActiveCount = 0;
    m_Engine.Init(this);
}
//---------------------------------------------------------------------------
bool cInArmLoaderPickPlanner::Search()
{
    if(!m_bInitialized) return false;
    bool bFound = m_Engine.Search();
    m_iLastActiveCount = bFound ? m_Engine.GetActiveCount() : 0;
    return bFound;
}
//---------------------------------------------------------------------------
void cInArmLoaderPickPlanner::Apply()
{
    //AI(ht9045-v899) 20260514: Phase 3 scaffold - no globals are written.
    // Phase 3.1 will set iXPosition[]/iYPosition/iLoadPitchStepY/etc.
}
//---------------------------------------------------------------------------
//=== IArmPickPlanContext stubs ============================================
// Returning 0 pickers makes cArmPickPlan::Search() return false immediately,
// guaranteeing zero side effects from this Phase 3 scaffold.
//---------------------------------------------------------------------------
int cInArmLoaderPickPlanner::GetLogicalPickerCount()
{
    return 0;                                                                   //AI(ht9045-v899) 20260514: Phase 3.1 -> active sucker count from InArmSuckUse
}
//---------------------------------------------------------------------------
bool cInArmLoaderPickPlanner::GetPhysicalSucker(int /*iLogical*/, int& rOutRow, int& rOutCol)
{
    rOutRow = -1;
    rOutCol = -1;
    return false;
}
//---------------------------------------------------------------------------
int cInArmLoaderPickPlanner::GetTargetRowMax()
{
    return 0;                                                                   //AI(ht9045-v899) 20260514: Phase 3.1 -> MOT[MMTrayY].Tray.YItem
}
//---------------------------------------------------------------------------
int cInArmLoaderPickPlanner::GetTargetColMax()
{
    return 0;                                                                   //AI(ht9045-v899) 20260514: Phase 3.1 -> MOT[MMTrayY].Tray.XItem
}
//---------------------------------------------------------------------------
bool cInArmLoaderPickPlanner::ComputeSlot(int /*iLogical*/,
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
bool cInArmLoaderPickPlanner::IsReachable(int /*iX*/, int /*iY*/)
{
    return true;                                                                //AI(ht9045-v899) 20260514: Loader has no soft-limit issue (per design)
}
//---------------------------------------------------------------------------
bool cInArmLoaderPickPlanner::IsTargetCandidate(int /*iTargetRow*/, int /*iTargetCol*/)
{
    return false;                                                               //AI(ht9045-v899) 20260514: Phase 3.1 -> MOT[MMTrayY].Tray.Data == HAS_IC
}
//---------------------------------------------------------------------------
bool cInArmLoaderPickPlanner::IsSuckerDemanded(int /*iLogical*/)
{
    return false;                                                               //AI(ht9045-v899) 20260514: Phase 3.1 -> InArmSuckUse + Item==NULL_IC
}
//---------------------------------------------------------------------------
