// =============================================================================
//  ainarm9045_2x4_16_shims.cpp  --  offline bodies for the 2x4_16 in-arm shims
//
//  Translation wave: W6.2b (in-arm per-site-config variant: 2x4_16, 16-site)
//  Translator: AI(W6.2b-2x4_16) 20260626
//
//  Conservative offline bodies for the helpers declared in
//  ainarm9045_2x4_16_shims.h.  Faithful to a handler with no loader tray / no
//  HotPlate populated: motion gates report "not finished" (false) so the SMs
//  hold their cursor; setters are no-ops; pick/team queries report empty.
//  See the header for the per-symbol golden provenance.
//
//  Big5: no Chinese; ZERO U+FFFD.
// =============================================================================
#include "ainarm9045_2x4_16_shims.h"

#ifndef HT9045_2x4_16_SHIMS_DEFINED
#define HT9045_2x4_16_SHIMS_DEFINED

// ---- Loader-pick helpers ----------------------------------------------------
int  iXPosition[8] = {0,0,0,0,0,0,0,0};   // loader-pick X positions per pick column
int  iYPosition    = 0;                   // loader-pick base Y position

#if 0   // PT-W7d RETIRED: real translated body now in ainarm9045.cpp
void AddLoadingCount(int /*iSuckRow*/, int /*iSuckCol*/, int /*iTrayRow*/, int /*iTrayCol*/) {}
#endif
int  CheckLoaderHasTray()                                  { return 0; }   // offline: no second tray under loader
#if 0   // PT-W7d RETIRED: real translated body now in ainarm9045.cpp
int  CheckLoaderHasTray(bool /*bAlarm*/, int /*iErrorCount*/, bool /*bTrayDuplicateErr*/) { return 0; }
#endif
#if 0   // PT-W7d RETIRED: real translated body now in ainarm9045.cpp
bool DoAutoSkipCheck()                                     { return false; } // offline: not in auto-skip flow
#endif
bool MoveArmXYToLoaderStage_9045()                         { return false; } // offline: never reports "arrived"
#if 0   // PT-W7d RETIRED: real translated body now in ainarm9045.cpp
int  GetLoaderYPitchStep()                                 { return 0; }   // offline: single-row pull
#endif
#if 0   // PT-W7e-part2 RETIRED (CheckTrayMapData): real translated body now in ainarm2.cpp
void CheckTrayMapData(int /*iTrayRow*/, int /*iTrayCol*/)  {}
#endif
#if 0   // PT-W7e-part2 RETIRED (ProcessTrayMapDataError): real translated body now in ainarm2.cpp
bool ProcessTrayMapDataError(bool /*bReset*/)              { return false; }
#endif
#if 0   // PT-W7d RETIRED: real translated body now in ainarm9045.cpp
int  ProcessMES0101InArmPickLoaderError(bool /*bHasDuplicateErr*/, AnsiString /*ErrPart*/) { return 0; } // 0 = not RETRY/SKIP/HOME/TRAY_END
#endif
//AI(W6.2b-2x4_16) 20260626: PorcessJAM0109HotPlatePickUpErrorSkip is now DEFINED
// in the registered ainarm_SearchPickPlate.cpp (its golden home) -- removed the
// local stub to avoid an ODR multiple-definition at link once this TU registers.
// The extern decl stays in the shims .h; the symbol resolves to SearchPickPlate.

// ---- HotPlate-pick helpers --------------------------------------------------
//AI(W6.2b-2x4_16) 20260626: SearchPlateToPick() is now DEFINED in the registered
// ainarm_SearchPickPlate.cpp (golden ainarm2.h home) -- removed the local stub to
// avoid an ODR multiple-definition at link.  Decl stays in the shims .h.
void GetInShuttleStatus_9045(bool bZFlag[MAX_ARM_Row][MAX_ARM_Col], bool /*bPlace*/, bool /*IncludeZ*/)
{
    // offline: no Z drop requested on any nozzle (the caller ZeroMemory'd it; keep it false)
    for(int i=0;i<MAX_ARM_Row;i++)
        for(int j=0;j<MAX_ARM_Col;j++)
            bZFlag[i][j]=false;
}
bool MoveInArmZToPickHotPlate_9045(int /*iRetryCT*/)       { return false; } // offline: never reports "arrived"
bool DoInArmPlaceToHotPlate_9045()                         { return false; } // offline: place-to-HP never finishes
bool DoInArmTryPickFromHotPlate_9045(bool & /*bCheckAll*/, bool & /*bShowError*/) { return false; }
void DoInArm_CheckSuckerMap()                              {}
bool CheckCloseSiteHasIC(int /*iMode*/, int /*iSht*/, int /*iKit*/) { return false; } // offline: no closed-site IC
bool IsHaveSameHotCount(int /*iHotCount*/)                 { return false; }
void AddArmSiteRecord(int /*iWhichSht*/)                   {}
void WhichShuttleReady(bool /*bFlag*/)                     {}
#if 0   // PT-W7d RETIRED: real translated body now in ainarm9045.cpp
bool bNeedOneCycle()                                       { return false; } // offline: not in one-cycle exit
#endif
#if 0   // PT-W7e-part2 RETIRED (AdjustShuttlePlaceOrder_AutoSiteMapping): real translated body now in ainarm2.cpp
void AdjustShuttlePlaceOrder_AutoSiteMapping()            {}
#endif
void DoJudgeInputShuttleNeedChangeToNullIC()              {}
#if 0   // PT-W7e-part2 RETIRED (InitInArmTryPickFromHotPlateTask100): real translated body now in ainarm2.cpp
void InitInArmTryPickFromHotPlateTask100()               {}
#endif
#if 0   // PT-W7e-part2 RETIRED (bNeedPickupErrorICToRecycleBin): real translated body now in ainarm2.cpp
bool bNeedPickupErrorICToRecycleBin()                     { return false; } // offline: no pickup-error bin flow
#endif
bool AutoTeachLoadTrayZ(bool /*bInit*/, int /*iArm*/, int & /*iTask*/) { return false; } // offline: never reports "taught"
TQPF_Timer DoArmPickFromLoadStage_Delay;                 // offline timer (Off() reports done via myTimer sim)
int  iInArmZTeachTask = 0;
int  iAutoSiteMapHotplateiWhichShuttle = 0;
//AI(W6.2b-2x4_16) 20260626: ProcessSCKARTLoadingCount(bool) is now DEFINED in the
// registered ainarm9045.cpp (the in-arm engine) -- removed the local stub to avoid
// an ODR multiple-definition at link.  The extern decl stays in the shims .h.
// AI(W906-CommonCompletion) 20260721: MyTickCount stand-in DEFINITION REMOVED
// -- common.cpp now provides the real body (golden common.h:259 un-gated this
// wave), and this file's own header (ainarm9045_2x4_16_shims.h:89) already
// declares it with the exact same signature, citing "golden common.h:259" --
// so removing just the definition here needs zero caller changes.
int  iArmXShuttle1OffsetPos = 0; // AutoClean InArm X offset to Sht1 (0)
int  iArmYShuttle1OffsetPos = 0; // AutoClean InArm Y offset to Sht1 (0)

// ---- scalars / flags --------------------------------------------------------
int  iPickP             = 0;     // HP plate index picked from (0 = HP1)
int  USE_Y_AUTO_PITCH   = 0;     // 0 = manual Y pitch (no auto pitch home)
int  iAutoSiteMapHPRow  = 0;     // JCET ASM HP row (0)
int  iAutoSiteMapHPCol  = 0;     // JCET ASM HP col (0)

// ---- Yield-monitoring facade ------------------------------------------------
// AI(W906-FW-YMSwap) 20260818: static shim instance + global RETIRED -- the
// live fYieldMonitoring is the REAL TfYieldMonitoring, defined at its golden
// home (uYieldMonitoring.cpp). DoAutoCloseSite stays a documented no-op there.

#endif // HT9045_2x4_16_SHIMS_DEFINED
