//---------------------------------------------------------------------------

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
void Initial_Auto_SendIC_Task();
void Do_Auto_SendIC();
void InitAuto2ReceiveTask();                                                    //kevin 20120726 Auto2
void DoAuto2Receive();                                                          //kevin 20120726 Auto2
void InitUnLoadNewAuto2TrayTask();                                              //kevin 20120726 Auto2 ¦¬tray
bool DoUnLoadNewAuto2ToStack();                                                 //kevin 20120726 Auto2 ¦¬tray
extern void DoAuto2();                                                          //kevin 20120718
void InitUnLoadNewAutoTrayTask(int iWhichAuto);                                 //ChungHung 20140317 add Auto Retest
bool DoUnLoadNewAutoToStack(int Pos);                                           //ChungHung 20140317 add Auto Retest

#endif
