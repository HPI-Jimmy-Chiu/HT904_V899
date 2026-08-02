// =============================================================================
//  cSiteUseManager.h  --  HotPlate / nozzle "site use" manager (FULL translation)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/cSiteUseManager.h
//                   (golden tree ROOT -- NOT under Public/; the W7 plan's path is
//                    wrong.  72 golden lines -> this file.)
//  Translation wave: W906-W7-L3
//  Translator: AI(W906-W7-L3) 20260802
//
//  AI(W906-W7-L3) 20260802: REPLACES the W6.2c "OFFLINE SHIM" that declared only 5
//  of golden's 24 public members with hardcoded bodies (UseCompactSearch->false,
//  CompactSearchPlateToPlace->true, CanAnyNozzleReachHP->true, ShadowLogBeforeSearch
//  /ValidateSearchResult->no-op).  All 24 members are now declared and really
//  implemented in cSiteUseManager.cpp.  Class name, member names, method names,
//  parameter names and declaration ORDER are preserved VERBATIM so the team can
//  cross-reference the BCB6 original.
//
//  BCB6 -> portable notes:
//    * golden :6 `#include "cmydef.h"` kept verbatim -- it is what drags in
//      AnsiString (via the vclcompat umbrella here, via the VCL one under BCB6).
//    * `__fastcall` on the constructor is KEPT (golden :12).  This tree does NOT
//      neutralise __fastcall (see vclcompat/vcl_compat.h:24-50); a declaration and
//      its definition must agree and both live in these two files.
//
//  GOLDEN COMMENT CONTRADICTION, preserved (do NOT "fix"): golden :24 documents
//  "iPlate: 0=HP1, 1=HP2" while golden cSiteUseManager.cpp:406 documents
//  "iPlate: 0=HP2(upper), 1=HP1(lower)" and golden cSiteUseManager.cpp:344 calls
//  plate 0 "HP2".  Both spellings are reproduced at their own sites.  Every LOG
//  line in the .cpp prints `iPlate + 1`, i.e. plate index 0 prints as "HP1".
//
//  ODR NOTE FOR THE INTEGRATOR (reported, not fixed here): ainarm9045_1x4_4.cpp
//  carries its own `#ifndef cSiteUseManagerH` forward facade for this class
//  (ainarm9045_1x4_4.cpp:96-108) and does NOT include this header, so that TU sees
//  a data-member-less cSiteUseManager while this TU defines the real 2-bool one.
//  Link-safe today (that TU only makes non-virtual calls on the extern object and
//  never takes its size/copies/constructs it), but it IS an ODR violation.  The
//  clean fix is one `#include "cSiteUseManager.h"` in ainarm9045_1x4_4.cpp, which
//  collapses that facade on the guard macro.  That file is outside this wave's
//  write set.
//
//  Encoding: pure ASCII, UTF-8, no BOM, trailing newline.
// =============================================================================
//AI(ht9045-v899) 20260405: Phase0 SiteUseManager read-only facade for nozzle/HP site queries
//---------------------------------------------------------------------------
#ifndef cSiteUseManagerH
#define cSiteUseManagerH

#include "cmydef.h"
//---------------------------------------------------------------------------

class cSiteUseManager
{
public:
    __fastcall cSiteUseManager();                                               //golden cSiteUseManager.h:12

    void Init();                                                                //golden cSiteUseManager.h:14

    //--- InArm nozzle queries (iSht: 0=SHT1, 1=SHT2) ---
    bool CanInNozzlePick(int iSht, int iRow, int iCol);                         //golden cSiteUseManager.h:17
    int  GetActiveInNozzleCount(int iSht);                                      //golden cSiteUseManager.h:18

    //--- OutArm nozzle queries ---
    bool CanOutNozzlePick(int iRow, int iCol);                                  //golden cSiteUseManager.h:21
    int  GetActiveOutNozzleCount();                                             //golden cSiteUseManager.h:22

    //--- HotPlate queries (iPlate: 0=HP1, 1=HP2) ---
    int  GetHPOccupiedCount(int iPlate);                                        //golden cSiteUseManager.h:25
    int  GetHPAvailableCount(int iPlate);                                       //golden cSiteUseManager.h:26
    int  GetHPCapacity();                                                       //golden cSiteUseManager.h:27
    bool HPHasSpace(int iPlate, int iNeedCells);                                //golden cSiteUseManager.h:28

    //--- Combined nozzle + HP ---
    int  GetPlaceableCountPerCycle(int iSht);                                   //golden cSiteUseManager.h:31
    bool WillExceedHPBoundary(int iPlate, int iCount);                          //golden cSiteUseManager.h:32

    //--- Debug dump ---
    AnsiString DumpInNozzleMap(int iSht);                                       //golden cSiteUseManager.h:35
    AnsiString DumpOutNozzleMap();                                              //golden cSiteUseManager.h:36
    AnsiString DumpHPStatus(int iPlate);                                        //golden cSiteUseManager.h:37

    //AI(ht9045-v899) 20260405: Phase1 shadow verification logging
    //--- Shadow verify (log-only, no control flow change) ---
    void ShadowLogBeforeSearch(int iPlate);                                     //golden cSiteUseManager.h:41
    void ShadowLogAfterPlace(int iPlate, int iRow, int iCol, int iCellState);   //golden cSiteUseManager.h:42

    //AI(ht9045-v899) 20260405: Phase2 SearchPlateToPlace result validation
    void ValidateSearchResult(int iIdx);                                        //golden cSiteUseManager.h:45

    //AI(ht9045-v899) 20260405: Phase3 switchable compact search algorithm
    //--- Algorithm switch ---
    bool UseCompactSearch();                                                    //golden cSiteUseManager.h:49
    void SetUseCompactSearch(bool bEnable);                                     //golden cSiteUseManager.h:50

    //--- Compact search (anti-fragmentation, writes to iPlacePlate/X/Y globals) ---
    //AI(ht9045-v899) 20260405: Phase5A return bool for failure detection
    bool CompactSearchPlateToPlace();                                           //golden cSiteUseManager.h:54

    //AI(ht9045-v899) 20260405: Phase6 motor soft limit pre-check
    bool WillExceedMotorLimit(int iPlate, int iHPCol, int iHPRow);              //golden cSiteUseManager.h:57
    bool CanAnyNozzleReachHP();                                                 //golden cSiteUseManager.h:58

    //AI(ht9045-v899) 20260407: diagnostic dump on CompactSearch failure
    void DumpCompactSearchFailure(int iNeedCol, int iNeedRow, int iYStep,       //golden cSiteUseManager.h:61-63
                                  int iAutoCleanYSkip, bool bDualPlate,
                                  int iStartPlate);

private:
    bool m_bInited;                                                             //golden cSiteUseManager.h:66
    bool m_bUseCompactSearch;                                                   //golden cSiteUseManager.h:67
};

extern cSiteUseManager SiteUseMgr;                                              //golden cSiteUseManager.h:70

#endif
