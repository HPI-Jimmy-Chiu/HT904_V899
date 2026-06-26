// =============================================================================
//  ainarm9045_2x4_16_shims.h  --  offline shims for the 2x4_16 in-arm variant
//
//  Translation wave: W6.2b (in-arm per-site-config variant: 2x4_16, 16-site)
//  Translator: AI(W6.2b-2x4_16) 20260626
//
//  WHY THIS FILE EXISTS
//  --------------------
//  ainarm9045_2x4_16.cpp is the per-site-config (common 16-site, 105 cases)
//  in-arm pick/place state-machine variant.  Its SM bodies are translated
//  FAITHFULLY and kept ACTIVE wherever they touch the Sim HAL substrate
//  (MOT[]/Suck/Sensor/Cylinder/InArmSuck/FormsFacade).  They additionally call
//  a set of in-arm-engine helpers that have NO translated home yet (their real
//  bodies live in the still-untranslated ainarm core / W7 modules):
//
//    AddLoadingCount / CheckLoaderHasTray / DoAutoSkipCheck /
//    MoveArmXYToLoaderStage_9045 / GetLoaderYPitchStep / CheckTrayMapData /
//    ProcessTrayMapDataError / ProcessMES0101InArmPickLoaderError /
//    PorcessJAM0109HotPlatePickUpErrorSkip / SearchPlateToPick /
//    GetInShuttleStatus_9045 / MoveInArmZToPickHotPlate_9045 /
//    DoInArmPlaceToHotPlate_9045 / DoInArmTryPickFromHotPlate_9045 /
//    DoInArm_CheckSuckerMap / CheckCloseSiteHasIC / IsHaveSameHotCount /
//    AddArmSiteRecord / WhichShuttleReady / bNeedOneCycle /
//    AdjustShuttlePlaceOrder_AutoSiteMapping / DoJudgeInputShuttleNeedChangeToNullIC /
//    iXPosition[8]/iYPosition/iPickP/iInposLed/USE_Y_AUTO_PITCH/fYieldMonitoring.
//
//  Several of these have a golden declaration (cited per line); a handful are
//  CALL-ONLY in golden (referenced solely by the 2x4_16 / sibling variant files
//  and NEVER declared/defined anywhere in the 906 golden tree -- i.e. this
//  variant is a dead-by-design file in golden).  For those we keep the call-site
//  text VERBATIM and supply a transparently-named ACTIVE offline stub here so
//  the variant remains self-consistent + compilable once the integrate stage
//  registers it.  Every stub is CONSERVATIVE: motion-gates return "not finished"
//  (false) so the SMs hold; pure setters are no-ops; the team-list pick reports
//  empty so the "nothing to do" branch is taken deterministically.
//
//  Big5: no Chinese in this file; nothing to decode.  ZERO U+FFFD.
// =============================================================================
#ifndef ainarm9045_2x4_16_shimsH
#define ainarm9045_2x4_16_shimsH

#include "vclcompat/vcl_compat.h"   // AnsiString
#include "cmydef.h"                 // MAX_ARM_Row / MAX_ARM_Col
#include "myTimer.h"                // TQPF_Timer (DoArmPickFromLoadStage_Delay)

#ifndef HT9045_DWORD_TYPEDEF
#define HT9045_DWORD_TYPEDEF
typedef unsigned long DWORD;        // golden windows.h (matches acarry_shims.h typedef)
#endif

#ifndef HT9045_2x4_16_SHIMS_DECLARED
#define HT9045_2x4_16_SHIMS_DECLARED

// ---- Loader-pick helpers (golden ainarm2.h / ainarm9045.cpp signatures) -----
extern int  iXPosition[8];                                                      // golden ainarm2.h:`iXPosition[8], iYPosition`
extern int  iYPosition;                                                         // golden ainarm2.h
extern void AddLoadingCount(int iSuckRow, int iSuckCol, int iTrayRow, int iTrayCol); // golden ainarm9045.cpp
extern int  CheckLoaderHasTray();                                               // golden ainarm9045.cpp (no-arg form)
extern int  CheckLoaderHasTray(bool bAlarm, int iErrorCount, bool bTrayDuplicateErr); // golden ainarm9045.cpp
extern bool DoAutoSkipCheck();                                                  // golden ainarm9045.cpp
extern bool MoveArmXYToLoaderStage_9045();                                      // golden (call-only) -- move XY over loader pick pos
extern int  GetLoaderYPitchStep();                                              // golden ainarm9045.cpp
extern void CheckTrayMapData(int iTrayRow, int iTrayCol);                       // golden ainarm2.h
extern bool ProcessTrayMapDataError(bool bReset);                              // golden ainarm2.h (default elsewhere)
extern int  ProcessMES0101InArmPickLoaderError(bool bHasDuplicateErr, AnsiString ErrPart); // golden ainarm9045.cpp
extern void PorcessJAM0109HotPlatePickUpErrorSkip(int iSuckR, int iSuckC, int iP, int iPlateR, int iPlateC); // golden ainarm_SearchPickPlate.cpp

// ---- HotPlate-pick helpers --------------------------------------------------
extern bool SearchPlateToPick();                                                // golden ainarm2.h
extern void GetInShuttleStatus_9045(bool bZFlag[MAX_ARM_Row][MAX_ARM_Col], bool bPlace, bool IncludeZ); // golden (call-only)
extern bool MoveInArmZToPickHotPlate_9045(int iRetryCT);                        // golden (call-only)
extern bool DoInArmPlaceToHotPlate_9045();                                      // golden (call-only)
extern bool DoInArmTryPickFromHotPlate_9045(bool &bCheckAll, bool &bShowError); // golden (call-only)
extern void DoInArm_CheckSuckerMap();                                           // golden (call-only)
extern bool CheckCloseSiteHasIC(int iMode, int iSht, int iKit);                 // golden (call-only)
extern bool IsHaveSameHotCount(int iHotCount);                                  // golden (call-only)
extern void AddArmSiteRecord(int iWhichSht);                                    // golden (call-only)
extern void WhichShuttleReady(bool bFlag);                                      // golden (call-only)
extern bool bNeedOneCycle();                                                    // golden ainarm9045.cpp
extern void AdjustShuttlePlaceOrder_AutoSiteMapping();                          // golden ainarm2.h
extern void DoJudgeInputShuttleNeedChangeToNullIC();                            // golden (call-only)
extern void InitInArmTryPickFromHotPlateTask100();                              // golden ainarm2.h
extern bool bNeedPickupErrorICToRecycleBin();                                   // golden ainarm2.h
extern bool AutoTeachLoadTrayZ(bool bInit, int iArm, int &iTask);               // golden (call-only) -- loader Z auto-teach
extern TQPF_Timer DoArmPickFromLoadStage_Delay;                                 // golden ainarm9045.cpp (TQPF_Timer)
extern int  iInArmZTeachTask;                                                   // golden (call-only) -- loader Z teach task cursor
extern int  iAutoSiteMapHotplateiWhichShuttle;                                  // golden (call-only) -- JCET ASM hotplate which-shuttle
extern bool ProcessSCKARTLoadingCount(bool bReset=false);                       // golden (call-only) -- SCK ART loading-count alarm flow
extern DWORD MyTickCount();                                                     // golden common.h:259 (gated #if 0 in common.h) -- ms tick
extern int  iArmXShuttle1OffsetPos;                                             // golden cmydef.h:3727 (commented-out) -- AutoClean InArm X offset to Sht1
extern int  iArmYShuttle1OffsetPos;                                             // golden cmydef.h:3728 (commented-out) -- AutoClean InArm Y offset to Sht1

// ---- scalars / flags the 2x4_16 SMs read -----------------------------------
extern int  iPickP;                                                             // golden (call-only) -- which HP plate (0/1) picked from
// NOTE: iInposLed is the golden Motor/HTMotor.h LED-index enum value (=8); it is
//       NOT a separate variable -- do not redeclare it here.
extern int  USE_Y_AUTO_PITCH;                                                   // golden (call-only) -- Y auto-pitch mode flag (compared ==true)
extern int  iAutoSiteMapHPRow;                                                  // golden (call-only) -- JCET ASM: HP row of the site being mapped
extern int  iAutoSiteMapHPCol;                                                  // golden (call-only) -- JCET ASM: HP col of the site being mapped

// ---- Yield-monitoring UI facade (golden uYieldMonitoring.h TfYieldMonitoring*)
//  Referenced only by the 2x4_16 / 2x8_32 dead variants (case 2000 calls
//  fYieldMonitoring->DoAutoCloseSite(false)).  Minimal offline stand-in.
class TfYieldMonitoring_2x4_16
{
public:
    void DoAutoCloseSite(bool /*bFlag*/) {}   // offline: no low-yield auto site-off
};
extern TfYieldMonitoring_2x4_16 *fYieldMonitoring;                              // golden uYieldMonitoring.h:fYieldMonitoring

#endif // HT9045_2x4_16_SHIMS_DECLARED

#endif
