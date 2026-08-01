// =============================================================================
//  asendic_Auto_RT.h  --  Auto-tray ROUND-TRIP (auto-retest tray recycle) SM
//                         prototypes + task-int cursors
//
//  Mirrored from: HT9011UC_Code_V3.33.906.0_20260618/asendic_Auto_RT.h
//                 (29 decoded lines: 28 through `#endif` plus one trailing
//                  blank; re-derived from the cp950 decode in this pass)
//  Translation wave: W906-W7-L1 Wave 2 (AUTO_RT track)
//  Translator: AI(W906-W7-L1-AutoRT) 20260802
//
//  WHAT THIS FILE IS
//  -----------------
//  The Auto1..Auto6 unloader-tray feed/recycle state machines used when
//  USE_AUTO_RETEST is installed: pull a fresh empty tray off the stack into the
//  car (DoLoadNewEmptyTrayToCar_RT), run a finished tray forward off the belt
//  (DoAutoTrayToFront), run a tray back in and hook it (DoAutoTrayToRear), push
//  a full tray back up onto the stack (DoUnLoadNewAutoToStack), and the
//  last-tray probe used by CC_ASE_KaohSiung (DoTrayZAutoTrayToWait).
//
//  THE GOLDEN HEADER CARRIES TWO ORPHAN DECLARATIONS -- MIRRORED VERBATIM
//  ----------------------------------------------------------------------
//  Both were re-derived by grepping the whole cp950-decoded golden tree in this
//  pass (every .cpp/.h, .svn excluded), not taken from a report:
//    * `DoAutoTray(int)`             golden asendic_Auto_RT.h:17.  Its ONLY other
//      occurrence in the entire golden tree is asendic_Auto_RT.cpp:475, which is
//      the FIRST LINE OF A COMMENT BLOCK (`/*void DoAutoTray(int Pos)` ... `*/`
//      spanning golden :475-667).  There is no definition anywhere.
//    * `DoLoadNewAutoTrayToCar(int)` golden asendic_Auto_RT.h:19.  Its ONLY other
//      occurrence is asendic_Auto_RT.cpp:654 -- inside that same comment block.
//      There is no definition anywhere either.
//  Same class of finding as asendic_Auto2.h's Initial_Auto_SendIC_Task /
//  Do_Auto_SendIC pair.  Declared here for header fidelity; nothing in this tree
//  calls them, so the missing definitions are inert (a caller would get an
//  undefined-symbol link error -- in golden too).
//
//  DECLARATIONS ADDED THAT THE GOLDEN HEADER DOES NOT CARRY
//  --------------------------------------------------------
//  golden reaches these through the asendic.h god-header chain, which this
//  tree's asendic.h subset does not reproduce.  Same idiom as
//  asendic_Loader_RT.h / asendic_Auto2.h:
//    * InitAutoTrayToFrontTask / DoAutoTrayToFront -- file-scope in golden,
//      called only from this TU today, exported so a future caller can reach
//      them without an implicit-declaration error under standard C++.
//    * InitUnLoadNewAutoTrayTask / DoUnLoadNewAutoToStack -- these two ARE
//      golden-declared, but in the SIBLINGS' headers (golden asendic_Auto.h:7
//      and golden asendic_Auto2.h:22-23), not here.  This tree's
//      asendic_Auto2.h already carries both with a "OWNED by asendic_Auto_RT.cpp"
//      note; re-declaring them here is signature-identical and harmless, and it
//      lets a caller include only this header.  golden csystem.cpp:7169/:7189
//      calls both.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef asendic_Auto_RTH
#define asendic_Auto_RTH
//---------------------------------------------------------------------------

#include "MachineType.h"        // MAX_AUTO_TRAY (golden pulls it via MachineDefine.h)

extern int iLoadNewAutoTrayToCarTask[MAX_AUTO_TRAY];                            // golden asendic_Auto_RT.h:6  (defined .cpp:28)
extern int iAutoTrayToFrontTask[MAX_AUTO_TRAY];                                 // golden asendic_Auto_RT.h:7  (defined .cpp:194)
extern int iAutoTrayReceiveTask[MAX_AUTO_TRAY];                                 // golden asendic_Auto_RT.h:8  (defined .cpp:30)
//extern int iAutoTrayTask[MAX_AUTO_TRAY];                                      // golden asendic_Auto_RT.h:9  (commented out in golden)
extern int iUnLoadNewAutoTrayTask[MAX_AUTO_TRAY];                               // golden asendic_Auto_RT.h:10 (defined .cpp:669)
extern int iAutoTrayToRearTask[MAX_AUTO_TRAY];                                  // golden asendic_Auto_RT.h:11 (defined .cpp:315)
extern int iTrayZAutoTrayToWaitTask[MAX_AUTO_TRAY];                             // golden asendic_Auto_RT.h:12 (defined .cpp:29)

//extern void InitAutoTrayTask(int Pos);                                        // golden asendic_Auto_RT.h:14 (commented out in golden)
extern void InitAutoTrayReceiveTask(int Pos);                                   // golden asendic_Auto_RT.h:15

extern void DoAutoTray(int Pos);                                                // golden asendic_Auto_RT.h:17 -- ORPHAN, see banner
extern void DoAutoTrayReceive(int Pos);                                         // golden asendic_Auto_RT.h:18
extern bool DoLoadNewAutoTrayToCar(int Pos);                                    // golden asendic_Auto_RT.h:19 -- ORPHAN, see banner

extern void InitAutoTrayToRearTask(int Pos);                                    // golden asendic_Auto_RT.h:21
extern bool DoAutoTrayToRear(int Pos);                                          // golden asendic_Auto_RT.h:22
extern void InitLoadNewAutoTrayToCarTask(int Pos);                              // golden asendic_Auto_RT.h:23
extern int DoLoadNewEmptyTrayToCar_RT(int Pos);                                 //kevin 20150613 bool ->Int    // golden asendic_Auto_RT.h:24

extern void InitTrayZAutoTrayToWaitTask();                                      //kevin 20150610              // golden asendic_Auto_RT.h:26
extern int DoTrayZAutoTrayToWait(int Pos);                                      //kevin 20150610              // golden asendic_Auto_RT.h:27

// --- not in the golden header; see banner ------------------------------------
extern void InitAutoTrayToFrontTask(int Pos);                                   // golden asendic_Auto_RT.cpp:197
extern bool DoAutoTrayToFront(int Pos);                                         // golden asendic_Auto_RT.cpp:202
extern void InitUnLoadNewAutoTrayTask(int Pos);                                 // golden asendic_Auto.h:7 / asendic_Auto2.h:22 (defined .cpp:672)
extern bool DoUnLoadNewAutoToStack(int Pos);                                    // golden asendic_Auto2.h:23              (defined .cpp:677)

#endif
