//---------------------------------------------------------------------------
//  cSiteUseManager.h  --  HP anti-fragmentation site-use manager (OFFLINE shim)
//
//  Translation wave: W6.2c batch-3 (Integrate).  Translator: AI(W6.2c-INARM-batch3)
//  Golden source: cSiteUseManager.h (AI(ht9045-v899) 20260405/20260407 :
//                 anti-fragmentation HotPlate site manager) -- mirror name = ROOT.
//
//  FAITHFUL offline substrate for the batch-3 1x4_4 in-arm variant
//  (ainarm9045_1x4_4.cpp), which derefs SiteUseMgr.UseCompactSearch() /
//  .CompactSearchPlateToPlace() / .ShadowLogBeforeSearch(int) /
//  .ValidateSearchResult(int) / .CanAnyNozzleReachHP() (12 refs).  The golden
//  class drives compact-search HP placement to fight site fragmentation; that
//  logic touches live HP geometry + logging, so the offline behaviour here is the
//  conservative "compact search OFF" defaults the translator reported:
//    UseCompactSearch()        -> false  (so the normal SearchPlateToPlace path
//                                          runs, NOT the compact branch)
//    CompactSearchPlateToPlace()-> true   (if ever called, report "placed ok")
//    CanAnyNozzleReachHP()     -> true   (do not skip-to-shuttle offline)
//    ShadowLogBeforeSearch()   -> no-op  (offline log)
//    ValidateSearchResult()    -> no-op  (offline log)
//  Method names/signatures preserved VERBATIM for BCB6 reverse-lookup.  The guard
//  macro cSiteUseManagerH matches the guarded forward facade inside
//  ainarm9045_1x4_4.cpp so that facade collapses when this real header is present.
//---------------------------------------------------------------------------
#ifndef cSiteUseManagerH
#define cSiteUseManagerH
//---------------------------------------------------------------------------

class cSiteUseManager
{
public:
    bool UseCompactSearch();            //golden cSiteUseManager.h:49
    bool CompactSearchPlateToPlace();   //golden cSiteUseManager.h:54
    void ShadowLogBeforeSearch(int iPlate); //golden cSiteUseManager.h:41
    void ValidateSearchResult(int iIdx);    //golden cSiteUseManager.h:45
    bool CanAnyNozzleReachHP();         //golden cSiteUseManager.h:58
};

extern cSiteUseManager SiteUseMgr;      //golden cSiteUseManager.h:70

#endif
