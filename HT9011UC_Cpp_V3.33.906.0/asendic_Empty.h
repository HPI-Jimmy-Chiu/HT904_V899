// =============================================================================
//  asendic_Empty.h  --  Empty-tray stack-feed SM prototypes + task-int cursors
//
//  Mirrored VERBATIM from: HT9011UC_Code_V3.33.906.0_20260618/asendic_Empty.h
//  Translation wave: W6.1 (CANARY)
//  Translator: AI(W6.0-SCAFFOLD) 20260626
//
//  W6 strategy: each SM owns its own extern cursor int (the iXxxTask family).
//  These are mirrored exactly; the .cpp defines iLoadNewEmptyTrayToCarTask at
//  file scope (golden :33) AND externs it here -- kept faithful to the golden.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef asendic_EmptyH
#define asendic_EmptyH
//---------------------------------------------------------------------------

extern int iLoadNewEmptyTrayToCarTask;
extern int iEmptyTrayToFrontTask;
extern int iAutoEmptyReceiveTask;
extern int iAutoEmptyTask;
extern int iUnLoadNewEmptyTrayTask;
extern int iEmptyTrayToRearTask;

extern void InitAutoEmptyTask();
extern void InitAutoEmptyReceiveTask();

extern void DoAutoEmptyReceive();
extern bool DoLoadNewEmptyTrayToCar();

extern int  iAutoEmpty1ReceiveTask;
extern void DoAutoEmpty();
extern void DoAutoEmpty1();
#endif
