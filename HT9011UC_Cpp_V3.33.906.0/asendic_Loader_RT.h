// =============================================================================
//  asendic_Loader_RT.h  --  Loader ROUND-TRIP (auto-retest tray recycle) SM
//                           prototypes + task-int cursors
//
//  Mirrored from: HT9011UC_Code_V3.33.906.0_20260618/asendic_Loader_RT.h (25 ln)
//  Translation wave: W906-W7-L1 Wave 1 (Loader + Loader_RT bundle)
//  Translator: AI(W906-W7-L1-Loader) 20260802
//
//  WHY THIS SHIPS WITH asendic_Loader.{h,cpp} AND NOT SEPARATELY
//  -------------------------------------------------------------
//  The two translation units are a genuine BIDIRECTIONAL cycle, so neither can
//  land without the other (all three edges re-derived from the cp950-decoded
//  golden in this pass):
//      asendic_Loader.cpp:1973  InitUnLoadNewLoaderTrayTask()
//                                        -> defined asendic_Loader_RT.cpp:681
//      asendic_Loader.cpp:1987  DoUnLoadNewLoaderToStack(false)
//                                        -> defined asendic_Loader_RT.cpp:686
//      asendic_Loader_RT.cpp:190 CheckLoaderICFloating(1)
//                                        -> defined asendic_Loader.cpp:3167
//
//  DECLARATIONS ADDED THAT THE GOLDEN HEADER DOES NOT CARRY
//  --------------------------------------------------------
//  golden reaches DoLoaderTrayToRear / InitLoaderTrayToRearTask /
//  DoAutoLoaderReceive's siblings through the god-header chain; this tree does
//  not reproduce that chain, so the file-scope entry points asendic_Loader.cpp
//  and future callers need are exported here.  Same idiom as asendic_Auto2.h.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef asendic_Loader_RTH
#define asendic_Loader_RTH
//---------------------------------------------------------------------------

extern int iLoadNewLoaderTrayToCarTask;                                         // golden asendic_Loader_RT.h:7  (defined .cpp:30)
extern int iLoaderTrayToFrontTask;                                              // golden asendic_Loader_RT.h:8  (defined .cpp:168)
extern int iAutoLoaderReceiveTask;                                              // golden asendic_Loader_RT.h:9  (defined .cpp:29)
extern int iAutoLoaderTask;                                                     // golden asendic_Loader_RT.h:10 (defined .cpp:469)
extern int iUnLoadNewLoaderTrayTask;                                            // golden asendic_Loader_RT.h:11 (defined .cpp:679)
extern int iLoaderTrayToRearTask;                                               // golden asendic_Loader_RT.h:12 (defined .cpp:336)

extern void InitAutoLoaderTask();                                               // golden asendic_Loader_RT.h:14
extern void InitAutoLoaderReceiveTask();                                        // golden asendic_Loader_RT.h:15

extern void DoAutoLoaderReceive();                                              // golden asendic_Loader_RT.h:17
extern bool DoLoadNewLoaderTrayToCar();                                         // golden asendic_Loader_RT.h:18

extern void DoAutoLoader();                                                     // golden asendic_Loader_RT.h:20

extern void InitUnLoadNewLoaderTrayTask();                                      // golden asendic_Loader_RT.h:22
extern bool DoUnLoadNewLoaderToStack(bool bAlarm);                              // golden asendic_Loader_RT.h:23
extern void InitLoaderTrayToFrontTask();                                        // golden asendic_Loader_RT.h:24
extern bool DoLoaderTrayToFront();                                              // golden asendic_Loader_RT.h:25

// --- not in the golden header; see banner ------------------------------------
extern void InitLoadNewLoaderTrayToCarTask();                                   // golden asendic_Loader_RT.cpp:33
extern void InitLoaderTrayToRearTask();                                         // golden asendic_Loader_RT.cpp:339
extern bool DoLoaderTrayToRear();                                               // golden asendic_Loader_RT.cpp:345

#endif
