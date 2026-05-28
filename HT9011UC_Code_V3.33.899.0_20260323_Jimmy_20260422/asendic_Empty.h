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
