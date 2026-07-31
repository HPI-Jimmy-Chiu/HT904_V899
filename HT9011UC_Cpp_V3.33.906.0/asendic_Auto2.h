// =============================================================================
//  asendic_Auto2.h  --  Auto2-tray stack-feed SM prototypes + task-int cursors
//
//  Mirrored from: HT9011UC_Code_V3.33.906.0_20260618/asendic_Auto2.h  (26 ln)
//  Translation wave: W906-W7-L1
//  Translator: AI(W906-W7-L1) 20260729
//
//  Same W6 strategy as asendic_Empty.h: each SM owns its own extern cursor int
//  (the iXxxTask family); the .cpp defines them at file scope and this header
//  externs them, kept faithful to the golden split.
//
//  ORPHAN DECLARATIONS (mirrored verbatim, NOT defined anywhere):
//  `Initial_Auto_SendIC_Task` / `Do_Auto_SendIC` are declared in the golden
//  header but have NO definition anywhere in the golden tree (grepped the full
//  golden source tree -- zero hits).  Same class of finding as the ckernel.h
//  orphan declarations noted in the plan (SS6 W7-L2).  Declared here for
//  header fidelity; never called from asendic_Auto2.cpp or anywhere else in
//  this wave's scope, so the missing definition is inert.
//
//  CROSS-FILE NOTE (see plan SS10 correction + SS6 W7-L1):
//  `InitUnLoadNewAutoTrayTask(int)` / `DoUnLoadNewAutoToStack(int)` below are
//  the SAME parameterised functions asendic_Auto.h externs -- both OWNED by
//  asendic_Auto_RT.cpp (golden asendic_Auto_RT.cpp:672,677), NOT by this file
//  or asendic_Auto2.cpp.  asendic_Auto2.cpp itself never calls them (verified
//  by grep against the golden .cpp body) -- the extern decl exists only for
//  OTHER golden callers outside this wave's 7-file family.  Left undefined in
//  this tree until asendic_Auto_RT.cpp is translated; harmless because nothing
//  in the current build calls through this name.
// =============================================================================
#ifndef asendic_Auto2H
#define asendic_Auto2H
//---------------------------------------------------------------------------

extern int iLoadNewAuto2TrayToCarTask;                                          //kevin 20120718
extern int iAuto2TrayToFrontTask;
extern int iAuto2TrayToRearTask;
extern int iAutoAuto2Task;
extern int iUnLoadNewAuto2TrayTask;
extern int iAuto2ReceiveTask;

bool DoLoadNewAuto2TrayToCar();                                                 //kevin 20120726
void Initial_Auto_SendIC_Task();                                                // orphan decl -- see banner
void Do_Auto_SendIC();                                                          // orphan decl -- see banner
void InitAuto2ReceiveTask();                                                    //kevin 20120726 Auto2
void DoAuto2Receive();                                                          //kevin 20120726 Auto2
void InitUnLoadNewAuto2TrayTask();                                              //kevin 20120726 Auto2 收tray
bool DoUnLoadNewAuto2ToStack();                                                 //kevin 20120726 Auto2 收tray
extern void DoAuto2();                                                          //kevin 20120718
void InitUnLoadNewAutoTrayTask(int iWhichAuto);                                 //ChungHung 20140317 add Auto Retest -- OWNED by asendic_Auto_RT.cpp, see banner
bool DoUnLoadNewAutoToStack(int Pos);                                           //ChungHung 20140317 add Auto Retest -- OWNED by asendic_Auto_RT.cpp, see banner

// Not declared in the golden header (it's a file-scope/implicit-decl golden
// call), but exported here (like InitLoadNewEmptyTrayToCarTask in
// asendic_Empty.h) so callers outside this TU can reach it without an
// implicit-declaration error under standard C++:
void InitLoadNewAuto2TrayToCarTask();                                           //kevin 20120718
void InitAuto2TrayToFrontTask();
void InitAuto2TrayToRearTask();
bool DoAuto2TrayToFront();
bool DoAuto2TrayToRear();
void InitAuto2Task();

#endif
