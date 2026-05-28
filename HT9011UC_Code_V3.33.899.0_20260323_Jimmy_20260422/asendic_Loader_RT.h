//---------------------------------------------------------------------------

#ifndef asendic_Loader_RTH
#define asendic_Loader_RTH
//---------------------------------------------------------------------------

extern int iLoadNewLoaderTrayToCarTask;
extern int iLoaderTrayToFrontTask;
extern int iAutoLoaderReceiveTask;
extern int iAutoLoaderTask;
extern int iUnLoadNewLoaderTrayTask;
extern int iLoaderTrayToRearTask;

extern void InitAutoLoaderTask();
extern void InitAutoLoaderReceiveTask();

extern void DoAutoLoaderReceive();
extern bool DoLoadNewLoaderTrayToCar();

extern void DoAutoLoader();

extern void InitUnLoadNewLoaderTrayTask();
extern bool DoUnLoadNewLoaderToStack(bool bAlarm);
extern void InitLoaderTrayToFrontTask();
extern bool DoLoaderTrayToFront();
#endif
