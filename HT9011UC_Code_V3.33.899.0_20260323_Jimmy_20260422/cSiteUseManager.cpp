//AI(ht9045-v899) 20260405: Phase0 SiteUseManager implementation - read-only queries over existing globals
//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "cSiteUseManager.h"
#include "ainarm2.h"
#include "ainarm_SearchPlacePlate.h"
#include "Motor/mymotor.h"
#include "MyKitSuck.h"
#include "cprod.h"
#include "common.h"
#include "csystem.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

cSiteUseManager SiteUseMgr;

//---------------------------------------------------------------------------
__fastcall cSiteUseManager::cSiteUseManager()
{
    m_bInited = false;
    m_bUseCompactSearch = false;
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260405: Phase4 load switch from Gerneral.ini [SiteUseMgr] section
void cSiteUseManager::Init()
{
    m_bUseCompactSearch = CheckAndReadIniDataGeneral("SiteUseMgr", "bUseCompactSearch", false);
    m_bInited = true;

    AnsiString sMsg;
    sMsg.sprintf("Init CompactSearch=%s", m_bUseCompactSearch ? "ON" : "OFF");
    RecordErrorLog(0, "SiteUseMgr", sMsg);
}
//---------------------------------------------------------------------------
//  InArm nozzle queries
//---------------------------------------------------------------------------
bool cSiteUseManager::CanInNozzlePick(int iSht, int iRow, int iCol)
{
    if(iSht < 0 || iSht > 1) return false;
    if(iRow < 0 || iRow >= MAX_ARM_Row) return false;
    if(iCol < 0 || iCol >= MAX_ARM_Col) return false;
    return Prod.fInArmSuckUse[iSht][iRow][iCol];
}
//---------------------------------------------------------------------------
int cSiteUseManager::GetActiveInNozzleCount(int iSht)
{
    if(iSht < 0 || iSht > 1) return 0;
    int iCount = 0;
    for(int r = 0; r < MAX_ARM_Row; r++)
        for(int c = 0; c < MAX_ARM_Col; c++)
            if(Prod.fInArmSuckUse[iSht][r][c])
                iCount++;
    return iCount;
}
//---------------------------------------------------------------------------
//  OutArm nozzle queries
//---------------------------------------------------------------------------
bool cSiteUseManager::CanOutNozzlePick(int iRow, int iCol)
{
    if(iRow < 0 || iRow >= MAX_ARM_Row) return false;
    if(iCol < 0 || iCol >= MAX_ARM_Col) return false;
    return Prod.fOutArmSuckUse[iRow][iCol];
}
//---------------------------------------------------------------------------
int cSiteUseManager::GetActiveOutNozzleCount()
{
    int iCount = 0;
    for(int r = 0; r < MAX_ARM_Row; r++)
        for(int c = 0; c < MAX_ARM_Col; c++)
            if(Prod.fOutArmSuckUse[r][c])
                iCount++;
    return iCount;
}
//---------------------------------------------------------------------------
//  HotPlate queries
//---------------------------------------------------------------------------
int cSiteUseManager::GetHPOccupiedCount(int iPlate)
{
    if(iPlate < 0 || iPlate > 1) return 0;
    int iCount = 0;
    int iXDiv = HotPlateForm.XDivision;
    int iYDiv = HotPlateForm.YDivision;
    for(int x = 0; x < iXDiv; x++)
        for(int y = 0; y < iYDiv; y++)
            if(MOT[MMPlate1 + iPlate].Tray.Data[x][y] != NULL_IC)
                iCount++;
    return iCount;
}
//---------------------------------------------------------------------------
int cSiteUseManager::GetHPAvailableCount(int iPlate)
{
    if(iPlate < 0 || iPlate > 1) return 0;
    return GetHPCapacity() - GetHPOccupiedCount(iPlate);
}
//---------------------------------------------------------------------------
int cSiteUseManager::GetHPCapacity()
{
    return HotPlateForm.XDivision * HotPlateForm.YDivision;
}
//---------------------------------------------------------------------------
bool cSiteUseManager::HPHasSpace(int iPlate, int iNeedCells)
{
    if(iNeedCells <= 0) return true;
    return GetHPAvailableCount(iPlate) >= iNeedCells;
}
//---------------------------------------------------------------------------
//  Combined nozzle + HP
//---------------------------------------------------------------------------
int cSiteUseManager::GetPlaceableCountPerCycle(int iSht)
{
    if(iSht < 0 || iSht > 1) return 0;
    int iCount = 0;
    for(int r = 0; r < MAX_ARM_Row; r++)
        for(int c = 0; c < MAX_ARM_Col; c++)
            if(Prod.fInArmSuckUse[iSht][r][c] && bZFlgToHP[r][c])
                iCount++;
    return iCount;
}
//---------------------------------------------------------------------------
bool cSiteUseManager::WillExceedHPBoundary(int iPlate, int iCount)
{
    if(iPlate < 0 || iPlate > 1) return true;
    return GetHPAvailableCount(iPlate) < iCount;
}
//---------------------------------------------------------------------------
//  Debug dump
//---------------------------------------------------------------------------
AnsiString cSiteUseManager::DumpInNozzleMap(int iSht)
{
    if(iSht < 0 || iSht > 1) return "Invalid sht";
    AnsiString s = "InArm SuckUse[" + IntToStr(iSht) + "]: ";
    for(int r = 0; r < MAX_ARM_Row; r++)
    {
        s += "R" + IntToStr(r) + "[";
        for(int c = 0; c < MAX_ARM_Col; c++)
        {
            s += Prod.fInArmSuckUse[iSht][r][c] ? "1" : "0";
            if(c < MAX_ARM_Col - 1) s += ",";
        }
        s += "] ";
    }
    return s;
}
//---------------------------------------------------------------------------
AnsiString cSiteUseManager::DumpOutNozzleMap()
{
    AnsiString s = "OutArm SuckUse: ";
    for(int r = 0; r < MAX_ARM_Row; r++)
    {
        s += "R" + IntToStr(r) + "[";
        for(int c = 0; c < MAX_ARM_Col; c++)
        {
            s += Prod.fOutArmSuckUse[r][c] ? "1" : "0";
            if(c < MAX_ARM_Col - 1) s += ",";
        }
        s += "] ";
    }
    return s;
}
//---------------------------------------------------------------------------
AnsiString cSiteUseManager::DumpHPStatus(int iPlate)
{
    if(iPlate < 0 || iPlate > 1) return "Invalid plate";
    AnsiString s = "HP" + IntToStr(iPlate + 1) + ": ";
    s += IntToStr(GetHPOccupiedCount(iPlate)) + "/" + IntToStr(GetHPCapacity());
    s += " (avail=" + IntToStr(GetHPAvailableCount(iPlate)) + ")";
    return s;
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260405: Phase1 shadow verification - log HP state before SearchPlateToPlace
void cSiteUseManager::ShadowLogBeforeSearch(int iPlate)
{
    AnsiString sMsg;
    int iOccupied = GetHPOccupiedCount(iPlate);
    int iAvail = GetHPAvailableCount(iPlate);
    int iNozzle0 = GetActiveInNozzleCount(0);
    int iNozzle1 = GetActiveInNozzleCount(1);
    int iPlaceable = GetPlaceableCountPerCycle(0);

    sMsg.sprintf("BeforeSearch HP%d occ=%d avail=%d cap=%d nz0=%d nz1=%d placeable=%d",
        iPlate + 1, iOccupied, iAvail, GetHPCapacity(), iNozzle0, iNozzle1, iPlaceable);

    RecordErrorLog(0, "SiteUseMgr", sMsg);
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260405: Phase1 shadow verification - log HP cell state after place attempt
void cSiteUseManager::ShadowLogAfterPlace(int iPlate, int iRow, int iCol, int iCellState)
{
    AnsiString sMsg;
    int iAvailAfter = GetHPAvailableCount(iPlate);

    sMsg.sprintf("AfterPlace HP%d [%d][%d] cellState=%d avail=%d",
        iPlate + 1, iRow, iCol, iCellState, iAvailAfter);

    RecordErrorLog(0, "SiteUseMgr", sMsg);
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260405: Phase2 validate SearchPlateToPlace result
void cSiteUseManager::ValidateSearchResult(int iIdx)
{
    if(iIdx < 0 || iIdx > 1) return;

    int iPlate = iPlacePlate[iIdx];
    int ix     = iPlacePlateX[iIdx];
    int iy     = iPlacePlateY[iIdx];
    AnsiString sMsg;
    bool bAnyWarn = false;

    // Check 1: plate index in range
    if(iPlate < 0 || iPlate > 1)
    {
        sMsg.sprintf("WARN plate=%d out of range [0,1] idx=%d", iPlate, iIdx);
        RecordErrorLog(0, "SiteUseMgr", sMsg);
        return;
    }

    int iXDiv = HotPlateForm.XDivision;
    int iYDiv = HotPlateForm.YDivision;

    // Check 2: X boundary
    if(ix < 0 || ix >= iXDiv)
    {
        sMsg.sprintf("WARN HP%d X=%d out of [0,%d) idx=%d", iPlate+1, ix, iXDiv, iIdx);
        RecordErrorLog(0, "SiteUseMgr", sMsg);
        bAnyWarn = true;
    }

    // Check 3: Y boundary
    if(iy < 0 || iy >= iYDiv)
    {
        sMsg.sprintf("WARN HP%d Y=%d out of [0,%d) idx=%d", iPlate+1, iy, iYDiv, iIdx);
        RecordErrorLog(0, "SiteUseMgr", sMsg);
        bAnyWarn = true;
    }

    // Check 4: target cell should be empty (NULL_IC)
    if(ix >= 0 && ix < iXDiv && iy >= 0 && iy < iYDiv)
    {
        int iCellData = MOT[MMPlate1 + iPlate].Tray.Data[ix][iy];
        if(iCellData != NULL_IC)
        {
            sMsg.sprintf("WARN HP%d [%d][%d] not empty cellState=%d idx=%d",
                iPlate+1, ix, iy, iCellData, iIdx);
            RecordErrorLog(0, "SiteUseMgr", sMsg);
            bAnyWarn = true;
        }
    }

    // Check 5: overall HP available vs nozzle count
    int iAvail = GetHPAvailableCount(iPlate);
    int iNozzle = GetActiveInNozzleCount(0);

    // OK summary
    sMsg.sprintf("Validate idx=%d HP%d pos=[%d][%d] avail=%d nozzle=%d %s",
        iIdx, iPlate+1, ix, iy, iAvail, iNozzle,
        bAnyWarn ? "HAS_WARN" : "OK");
    RecordErrorLog(0, "SiteUseMgr", sMsg);
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260405: Phase3 algorithm switch accessors
bool cSiteUseManager::UseCompactSearch()
{
    //AI(JimmyChiu) 20260428: revert force-enable for e9045_1x4_4. CompactSearch performs
    //  a 1x1 cell scan but DoPlaceToHotPlate uses stride placement (e.g. 0,2,4,6).
    //  When the chosen start cell is not stride-aligned, the second placement attempt
    //  cannot find a matching slot and triggers DoHotPlateHangUp. Realtek site reproduced
    //  this with 2 real ICs at col 0+3 followed by hangup on the 2nd batch.
    //  Honor INI bUseCompactSearch only until stride-aware search is implemented.
    return m_bUseCompactSearch;
}
//---------------------------------------------------------------------------
void cSiteUseManager::SetUseCompactSearch(bool bEnable)
{
    m_bUseCompactSearch = bEnable;
    AnsiString sMsg;
    sMsg.sprintf("CompactSearch %s", bEnable ? "ENABLED" : "DISABLED");
    RecordErrorLog(0, "SiteUseMgr", sMsg);
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260405: Phase5 compact search with edge-case protection
//  Returns true=found space, false=HP full (caller must handle HangUp)
bool cSiteUseManager::CompactSearchPlateToPlace()
{
    int &iPlate = iPlacePlate[0];
    int &ix     = iPlacePlateX[0];
    int &iy     = iPlacePlateY[0];

    int iXDiv = HotPlateForm.XDivision;
    int iYDiv = HotPlateForm.YDivision;

    // Determine placement block size
    //AI(ht9045-v899) 20260407: force 1x1 single-cell search for starting position.
    //  Original SearchPlateToPlace returns a starting (ix,iy) position, then
    //  DoPlaceToHotPlate maps each nozzle to HP columns via GetPlaceToHotPlateSuckCol()
    //  using stride patterns (e.g. stride=2 for 1x4 on 8-col HP → cols 0,2,4,6).
    //  CompactSearch cannot replicate this stride logic. Using iNeedCol=iPickCol or
    //  GetHotPlateColStep() assumes consecutive columns which is wrong.
    //  Fix: search for single empty cells, let the placement code handle stride mapping.
    int iNeedRow = 1;
    int iNeedCol = 1;

    //AI(ht9045-v899) 20260405: Phase5B bPitchOver12000 downgrade to 1x1 scan
    if(bPitchOver12000)
    {
        iNeedRow = 1;
        iNeedCol = 1;
    }

    //AI(ht9045-v899) 20260405: Phase5B Row2CanPutHP/HotPlateYPitchCanPutAll guard
    if(iNeedRow >= 2)
    {
        if(HotPlateYPitchCanPutAll() == false || Row2CanPutHP(PlaceMode) == false)
            iNeedRow = 1;
    }

    // Adjust need based on iYHalf (half-height spacing for 2-row nozzles)
    int iYStep = 1;
    if(iNeedRow >= 2 && iYHalf > 0)
        iYStep = iYHalf;

    //AI(ht9045-v899) 20260405: Phase5B AutoClean HP2 Y-offset skip
    int iAutoCleanYSkip = 0;
    if(TestIF_File.iAutoClean_Tray == eCKPos_HP2 &&
       TestIF_File.iAutoClean_Function == 1)
    {
        iAutoCleanYSkip = 4;
    }

    // Try each plate (respect iPlateSelect)
    int iStartPlate = iPlate;
    bool bDualPlate = (HotPlateForm.iPlateSelect == 0x03);

    for(int iPlateTry = 0; iPlateTry < (bDualPlate ? 2 : 1); iPlateTry++)
    {
        int iP = (iStartPlate + iPlateTry) % 2;

        //AI(ht9045-v899) 20260407: scan Y outer, X inner (left-to-right then top-to-bottom)
        for(int cy = 0; cy < iYDiv; cy++)
        {
            for(int cx = 0; cx < iXDiv; cx++)
            {
                //AI(ht9045-v899) 20260405: Phase5B skip AutoClean reserved zone on HP2(plate=0)
                if(iP == 0 && iAutoCleanYSkip > 0 && cy < iAutoCleanYSkip)
                    continue;

                //AI(ht9045-v899) 20260405: Phase6 skip positions that would exceed motor soft limit
                if(WillExceedMotorLimit(iP, cx, cy))
                    continue;

                // Check if block [cx..cx+iNeedCol-1][cy, cy+iYStep*(iNeedRow-1)] all empty
                bool bFit = true;
                for(int dr = 0; dr < iNeedRow && bFit; dr++)
                {
                    int ty = cy + iYStep * dr;
                    if(ty >= iYDiv) { bFit = false; break; }

                    for(int dc = 0; dc < iNeedCol && bFit; dc++)
                    {
                        int tx = cx + dc;
                        if(tx >= iXDiv) { bFit = false; break; }

                        if(MOT[MMPlate1 + iP].Tray.Data[tx][ty] != NULL_IC)
                            bFit = false;
                    }
                }

                if(bFit)
                {
                    iPlate = iP;
                    ix     = cx;
                    iy     = cy;

                    AnsiString sMsg;
                    sMsg.sprintf("CompactSearch OK HP%d [%d][%d] need=%dx%d",
                        iP+1, cx, cy, iNeedCol, iNeedRow);
                    RecordErrorLog(0, "SiteUseMgr", sMsg);

                    #ifndef SOFT_SIMULTE
                    CheckSafeDoorIsClosed();
                    #endif
                    return true;
                }
            }
        }
    }

    // No space found — log warning and return false for caller to handle HangUp
    AnsiString sMsg;
    sMsg.sprintf("CompactSearch FULL no space need=%dx%d", iNeedCol, iNeedRow);
    RecordErrorLog(0, "SiteUseMgr", sMsg);

    //AI(ht9045-v899) 20260407: dump all diagnostic info on search failure for root cause analysis
    DumpCompactSearchFailure(iNeedCol, iNeedRow, iYStep, iAutoCleanYSkip, bDualPlate, iStartPlate);

    #ifndef SOFT_SIMULTE
    CheckSafeDoorIsClosed();
    #endif
    return false;
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260405: Phase6 motor soft limit pre-check
//  Conservative estimate: compute base X position for given HP cell,
//  then check if worst-case nozzle offset exceeds PSoftLimitP/N.
//  iPlate: 0=HP2(upper), 1=HP1(lower); iHPCol: 0..XDiv-1; iHPRow: 0..YDiv-1
//AI(ht9045-v899) 20260407: DISABLED — iPitchSpan=iXpitchMaxX3 is too conservative
//  (±12000 when actual motor offset is ~+4000). Original SearchPlateToPlace never
//  checks motor limits. Re-enable after implementing accurate center-offset calculation.
bool cSiteUseManager::WillExceedMotorLimit(int iPlate, int iHPCol, int iHPRow)
{
    return false;  // disabled: original algorithm has no motor limit pre-check
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260405: Phase6 check if ANY HP cell is reachable by current nozzles
bool cSiteUseManager::CanAnyNozzleReachHP()
{
    int iXDiv = HotPlateForm.XDivision;
    int iYDiv = HotPlateForm.YDivision;
    bool bDualPlate = (HotPlateForm.iPlateSelect == 0x03);

    for(int iP = 0; iP < (bDualPlate ? 2 : 1); iP++)
    {
        for(int cx = 0; cx < iXDiv; cx++)
        {
            for(int cy = 0; cy < iYDiv; cy++)
            {
                if(MOT[MMPlate1 + iP].Tray.Data[cx][cy] == NULL_IC)
                {
                    if(WillExceedMotorLimit(iP, cx, cy) == false)
                        return true;  // found at least one reachable empty cell
                }
            }
        }
    }
    return false;  // no reachable empty cell
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260407: diagnostic dump on CompactSearch failure — log all decision params + HP grid
void cSiteUseManager::DumpCompactSearchFailure(int iNeedCol, int iNeedRow, int iYStep,
                                                int iAutoCleanYSkip, bool bDualPlate,
                                                int iStartPlate)
{
    int iXDiv = HotPlateForm.XDivision;
    int iYDiv = HotPlateForm.YDivision;
    AnsiString s;

    // 1. Algorithm parameters
    s.sprintf("FAIL PARAMS iInArmType=%d iPickRow=%d iPickCol=%d HPStep=%d need=%dx%d yStep=%d yHalf=%d",
        iInArmType, InArmSuck.iPickRow, InArmSuck.iPickCol,
        GetHotPlateColStep(iPlaceHP), iNeedCol, iNeedRow, iYStep, iYHalf);
    RecordErrorLog(0, "SiteUseMgr", s);

    // 2. HP config — use Prod.HotPlateForm[0] (int TRAY_DATA), NOT global HotPlateForm (double TRAY_TYPE_PARA)
    //AI(ht9045-v899) 20260407: fix dump to use correct struct for XPitch/YPitch
    s.sprintf("FAIL HP XDiv=%d YDiv=%d ProdXPitch=%d ProdYPitch=%d plateSelect=0x%02X dual=%d startPlate=%d cleanSkip=%d pitchOver=%d",
        iXDiv, iYDiv, Prod.HotPlateForm[0].iXPitch, Prod.HotPlateForm[0].iYPitch,
        HotPlateForm.iPlateSelect, bDualPlate ? 1 : 0, iStartPlate,
        iAutoCleanYSkip, bPitchOver12000 ? 1 : 0);
    RecordErrorLog(0, "SiteUseMgr", s);

    // 2b. Motor limit reference data for WillExceedMotorLimit diagnosis
    {
        int iBaseX0 = Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase];
        int iBaseX1 = Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase];
        int iBaseY0 = Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase];
        int iBaseY1 = Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase];
        s.sprintf("FAIL MOTOR baseX0=%d baseX1=%d baseY0=%d baseY1=%d pitchSpan=%d",
            iBaseX0, iBaseX1, iBaseY0, iBaseY1, iXpitchMaxX3);
        RecordErrorLog(0, "SiteUseMgr", s);
        s.sprintf("FAIL MOTOR limXP=%d limXN=%d limYP=%d limYN=%d",
            MOT[MInArmX].Motor->PSoftLimitP, MOT[MInArmX].Motor->PSoftLimitN,
            MOT[MInArmY].Motor->PSoftLimitP, MOT[MInArmY].Motor->PSoftLimitN);
        RecordErrorLog(0, "SiteUseMgr", s);
        // Sample: show target for col 0 and col 7 on plate 0
        for(int cx = 0; cx < iXDiv; cx++)
        {
            int iTgtX = iBaseX0 + Prod.HotPlateForm[0].iXPitch * cx;
            int iTgtY = iBaseY0 - Prod.HotPlateForm[0].iYPitch * 0;
            bool bExceed = WillExceedMotorLimit(0, cx, 0);
            s.sprintf("FAIL MOTOR P0 col=%d tgtX=%d tgtY=%d exceed=%d", cx, iTgtX, iTgtY, bExceed ? 1 : 0);
            RecordErrorLog(0, "SiteUseMgr", s);
        }
    }

    // 3. Current search position
    s.sprintf("FAIL POS plate=%d ix=%d iy=%d iPlaceHP=%d",
        iPlacePlate[0], iPlacePlateX[0], iPlacePlateY[0], iPlaceHP);
    RecordErrorLog(0, "SiteUseMgr", s);

    // 4. HP occupancy summary
    for(int iP = 0; iP < (bDualPlate ? 2 : 1); iP++)
    {
        int iOcc = GetHPOccupiedCount(iP);
        int iAvail = GetHPAvailableCount(iP);
        int iMotorSkip = 0;
        int iEmptyCount = 0;

        for(int cx = 0; cx < iXDiv; cx++)
        {
            for(int cy = 0; cy < iYDiv; cy++)
            {
                if(MOT[MMPlate1 + iP].Tray.Data[cx][cy] == NULL_IC)
                {
                    iEmptyCount++;
                    if(WillExceedMotorLimit(iP, cx, cy))
                        iMotorSkip++;
                }
            }
        }
        s.sprintf("FAIL HP%d occ=%d avail=%d empty=%d motorSkip=%d cap=%d",
            iP+1, iOcc, iAvail, iEmptyCount, iMotorSkip, GetHPCapacity());
        RecordErrorLog(0, "SiteUseMgr", s);
    }

    // 5. HP grid map — dump each row as a string of cell states
    //    '.' = NULL_IC(empty), 'X' = occupied, 'M' = empty but motor-limited
    for(int iP = 0; iP < (bDualPlate ? 2 : 1); iP++)
    {
        for(int cy = 0; cy < iYDiv; cy++)
        {
            AnsiString sRow = "";
            for(int cx = 0; cx < iXDiv; cx++)
            {
                int iData = MOT[MMPlate1 + iP].Tray.Data[cx][cy];
                if(iData == NULL_IC)
                {
                    if(WillExceedMotorLimit(iP, cx, cy))
                        sRow += "M";
                    else if(iP == 0 && iAutoCleanYSkip > 0 && cy < iAutoCleanYSkip)
                        sRow += "C";
                    else
                        sRow += ".";
                }
                else
                    sRow += "X";
            }
            s.sprintf("FAIL GRID HP%d Y%02d [%s]", iP+1, cy, sRow.c_str());
            RecordErrorLog(0, "SiteUseMgr", s);
        }
    }

    // 6. InArm sucker state
    s = "FAIL SUCK Item[";
    for(int r = 0; r < MAX_ARM_Row; r++)
    {
        if(r > 0) s += "|";
        for(int c = 0; c < MAX_ARM_Col; c++)
        {
            if(c > 0) s += ",";
            s += IntToStr(InArmSuck.Item[r][c]);
        }
    }
    s += "]";
    RecordErrorLog(0, "SiteUseMgr", s);
}
