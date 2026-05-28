//AI(ht9045-v899) 20260405: Phase0 SiteUseManager read-only facade for nozzle/HP site queries
//---------------------------------------------------------------------------
#ifndef cSiteUseManagerH
#define cSiteUseManagerH

#include "cmydef.h"
//---------------------------------------------------------------------------

class cSiteUseManager
{
public:
    __fastcall cSiteUseManager();

    void Init();

    //--- InArm nozzle queries (iSht: 0=SHT1, 1=SHT2) ---
    bool CanInNozzlePick(int iSht, int iRow, int iCol);
    int  GetActiveInNozzleCount(int iSht);

    //--- OutArm nozzle queries ---
    bool CanOutNozzlePick(int iRow, int iCol);
    int  GetActiveOutNozzleCount();

    //--- HotPlate queries (iPlate: 0=HP1, 1=HP2) ---
    int  GetHPOccupiedCount(int iPlate);
    int  GetHPAvailableCount(int iPlate);
    int  GetHPCapacity();
    bool HPHasSpace(int iPlate, int iNeedCells);

    //--- Combined nozzle + HP ---
    int  GetPlaceableCountPerCycle(int iSht);
    bool WillExceedHPBoundary(int iPlate, int iCount);

    //--- Debug dump ---
    AnsiString DumpInNozzleMap(int iSht);
    AnsiString DumpOutNozzleMap();
    AnsiString DumpHPStatus(int iPlate);

    //AI(ht9045-v899) 20260405: Phase1 shadow verification logging
    //--- Shadow verify (log-only, no control flow change) ---
    void ShadowLogBeforeSearch(int iPlate);
    void ShadowLogAfterPlace(int iPlate, int iRow, int iCol, int iCellState);

    //AI(ht9045-v899) 20260405: Phase2 SearchPlateToPlace result validation
    void ValidateSearchResult(int iIdx);

    //AI(ht9045-v899) 20260405: Phase3 switchable compact search algorithm
    //--- Algorithm switch ---
    bool UseCompactSearch();
    void SetUseCompactSearch(bool bEnable);

    //--- Compact search (anti-fragmentation, writes to iPlacePlate/X/Y globals) ---
    //AI(ht9045-v899) 20260405: Phase5A return bool for failure detection
    bool CompactSearchPlateToPlace();

    //AI(ht9045-v899) 20260405: Phase6 motor soft limit pre-check
    bool WillExceedMotorLimit(int iPlate, int iHPCol, int iHPRow);
    bool CanAnyNozzleReachHP();

    //AI(ht9045-v899) 20260407: diagnostic dump on CompactSearch failure
    void DumpCompactSearchFailure(int iNeedCol, int iNeedRow, int iYStep,
                                  int iAutoCleanYSkip, bool bDualPlate,
                                  int iStartPlate);

private:
    bool m_bInited;
    bool m_bUseCompactSearch;
};

extern cSiteUseManager SiteUseMgr;

#endif
