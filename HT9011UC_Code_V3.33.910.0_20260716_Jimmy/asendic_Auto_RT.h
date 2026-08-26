//---------------------------------------------------------------------------
#ifndef asendic_Auto_RTH
#define asendic_Auto_RTH
//---------------------------------------------------------------------------

extern int iLoadNewAutoTrayToCarTask[MAX_AUTO_TRAY];
extern int iAutoTrayToFrontTask[MAX_AUTO_TRAY];
extern int iAutoTrayReceiveTask[MAX_AUTO_TRAY];
//extern int iAutoTrayTask[MAX_AUTO_TRAY];
extern int iUnLoadNewAutoTrayTask[MAX_AUTO_TRAY];
extern int iAutoTrayToRearTask[MAX_AUTO_TRAY];
extern int iTrayZAutoTrayToWaitTask[MAX_AUTO_TRAY];

//extern void InitAutoTrayTask(int Pos);
extern void InitAutoTrayReceiveTask(int Pos);

extern void DoAutoTray(int Pos);
extern void DoAutoTrayReceive(int Pos);
extern bool DoLoadNewAutoTrayToCar(int Pos);

extern void InitAutoTrayToRearTask(int Pos);
extern bool DoAutoTrayToRear(int Pos);
extern void InitLoadNewAutoTrayToCarTask(int Pos);
extern int DoLoadNewEmptyTrayToCar_RT(int Pos);                                 //kevin 20150613 bool ->Int

extern void InitTrayZAutoTrayToWaitTask();                                      //kevin 20150610
extern int DoTrayZAutoTrayToWait(int Pos);                                      //kevin 20150610
#endif

