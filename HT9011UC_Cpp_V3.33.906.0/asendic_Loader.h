// =============================================================================
//  asendic_Loader.h  --  Loader (incoming IC tray supply) SM prototypes + cursors
//
//  Mirrored from: HT9011UC_Code_V3.33.906.0_20260618/asendic_Loader.h  (27 ln)
//  Translation wave: W906-W7-L1 Wave 1 (Loader + Loader_RT bundle)
//  Translator: AI(W906-W7-L1-Loader) 20260802
//
//  Same W6 strategy as asendic_Empty.h / asendic_Auto2.h: each SM owns its own
//  extern cursor int (the iXxxTask family); asendic_Loader.cpp defines them at
//  file scope and this header externs them, faithful to the golden split.
//
//  ONE GOLDEN DECLARATION DELIBERATELY OMITTED
//  -------------------------------------------
//  golden asendic_Loader.h:9 `extern int iTrayZLoadTrayToWaitTask;`.  Wave 0
//  already landed that exact declaration ONCE in the ported asendic.h (see the
//  AI(W906-W7-L1-Wave0) note there), specifically so the several W7-L1 agents
//  that only READ the cursor do not each add their own.  Repeating it here would
//  be harmless C++ but would defeat that serialization, so it is not repeated.
//  The DEFINITION `int iTrayZLoadTrayToWaitTask=1;` (note the NON-ZERO initial
//  value -- it is the SM's idle cursor) belongs to and lives in
//  asendic_Loader.cpp, golden asendic_Loader.cpp:51.
//
//  ORPHAN DECLARATIONS (mirrored verbatim, NOT defined anywhere)
//  -------------------------------------------------------------
//  `Init_TrayY_TrayZ_Task()` (golden asendic_Loader.h:13) and `Do_TrayY_TrayZ()`
//  (golden asendic_Loader.h:19) have NO definition anywhere in the golden tree:
//  a cp950-decoded grep of every .h/.cpp in
//  HT9011UC_Code_V3.33.906.0_20260618 finds exactly ONE hit each, the
//  declaration itself.  Same class of finding as the asendic_Auto2.h orphans.
//  Mirrored for header fidelity; nothing in this tree calls them, so the missing
//  definition is inert.
//
//  DECLARATIONS ADDED THAT THE GOLDEN HEADER DOES NOT CARRY
//  --------------------------------------------------------
//  golden declares `DoLoadNewICTray()` and `DoSupplyNewICTray()` in ainarm2.h
//  (golden ainarm2.h:208 / :210), not here.  ainarm2.h is not translated, so the
//  two are exported here instead -- same idiom asendic_Auto2.h uses for
//  InitLoadNewAuto2TrayToCarTask et al.  `DoCassetteLoadNewICTray` /
//  `InitCassetteLoadNewICTrayTask` / `DoInspectTrayColorOnLoader` and the
//  remaining file-scope cursors/flags are exported for the same reason (golden
//  reaches them through the god-header chain this tree does not reproduce).
// =============================================================================
//---------------------------------------------------------------------------

#ifndef asendic_LoaderH
#define asendic_LoaderH
//---------------------------------------------------------------------------

#include "vclcompat/vcl_compat.h"   // AnsiString (not needed by the decls below,
                                    // but keeps this header self-contained the
                                    // same way asendic.h is)

extern int LoadTask;                                                            // golden asendic_Loader.h:7  (defined asendic_Loader.cpp:2441)
extern int iSupplyNewIC_From_LoaderCar;                                         // golden asendic_Loader.h:8  (defined asendic_Loader.cpp:148)
// golden asendic_Loader.h:9 iTrayZLoadTrayToWaitTask -- see banner (lives in asendic.h)
extern int iLoadNewICTrayTask;                                                  // golden asendic_Loader.h:10 (defined asendic_Loader.cpp:52)

extern void InitLoadTask();                                                     // golden asendic_Loader.h:12
extern void Init_TrayY_TrayZ_Task();                                            // golden asendic_Loader.h:13 -- ORPHAN, see banner
extern void InitLoadNewICTrayTask();                                            // golden asendic_Loader.h:14
extern void InitTrayZLoadTrayToWaitTask();                                      // golden asendic_Loader.h:15
extern void InitSupplyNewIC_From_LoaderCarTask();                               // golden asendic_Loader.h:16

extern void DoLoad();                                                           // golden asendic_Loader.h:18
extern void Do_TrayY_TrayZ();                                                   // golden asendic_Loader.h:19 -- ORPHAN, see banner
extern bool DoTrayZLoadTrayToWait();                                            // golden asendic_Loader.h:20
extern int iLoaderTrackDetectICFloatingTask;                                    // golden asendic_Loader.h:21
extern void InitLoaderTrackDetectICFloatingTask();                              // golden asendic_Loader.h:22  //Sam 20200316 : Loader Detect Tray
extern bool DoLoaderTrackDetectICFloating();                                    // golden asendic_Loader.h:23
extern bool CheckLoaderICFloating(int iDir);                                    // golden asendic_Loader.h:24  //Sam 20240108 : integrated Loader IC-floating check
void CheckContinManualRemoveTray();                                             // golden asendic_Loader.h:25  //Sam 20221027 : integrated P24 function
extern bool DoKnockBeforeRemainICCheck(bool bIsFirst);                          // golden asendic_Loader.h:26

// --- not in the golden header; see banner ------------------------------------
extern bool DoLoadNewICTray();                                                  // golden ainarm2.h:208 (body asendic_Loader.cpp:2022)
extern bool DoSupplyNewICTray();                                                // golden ainarm2.h:210 (body asendic_Loader.cpp:155)
extern void InitCassetteLoadNewICTrayTask();                                    // golden asendic_Loader.cpp:62  //Frank 20251217 add
extern bool DoCassetteLoadNewICTray();                                          // golden asendic_Loader.cpp:70  //Frank 20251217 add
extern bool DoInspectTrayColorOnLoader(bool bIsFirst);                          // golden asendic_Loader.cpp:1876

extern int  iCassetteLoadNewICTrayTask;                                         // golden asendic_Loader.cpp:53
extern int  iInspectTrayColorOnLoaderTask;                                      // golden asendic_Loader.cpp:54
extern bool bSupplyNewICTrayPause;                                              // golden asendic_Loader.cpp:55 (golden csystem/note/mymessbox write it)
extern bool bLoadTrayCCDMapOK;                                                  // golden asendic_Loader.cpp:56  //kevin 20220401 Get CCD Data
extern int  iKnockTrayTask;                                                     // golden asendic_Loader.cpp:57

#endif
