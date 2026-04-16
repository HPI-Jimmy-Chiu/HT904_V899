//---------------------------------------------------------------------------
#ifndef asendic_AutoH
#define asendic_AutoH
//---------------------------------------------------------------------------

extern void Initial_Auto_BinTray_Task(int index);
extern void InitUnLoadNewAutoTrayTask(int pos);
extern void DoAutoReceiveBinTray(int Pos);
extern int BinTrayTask[MAX_AUTO_TRAY];
extern bool bAutoIsFullNeedStop[MAX_AUTO_TRAY];                                 //JerryYang 20160810 Unloader滿tray是否需要馬上停機

extern bool DoAutoTrackDetectICFloating(int Pos);                               //Isaac 20180109 (Steven) : auto123可前進後退
extern int iAutoTrackDetectICFloatingTask[MAX_AUTO_TRAY];
extern int iAuto123TrayToRearTask[MAX_AUTO_TRAY];
extern void InitAuto123TrayToRearTask(int Pos);
extern void InitAutoTrackDetectICFloatingTask(int Pos);
extern bool DoAuto123TrayToRear(int Pos);
extern void CheckHasErrorBinOnTray(int Pos);                                    //Sam 20240108 : 新增退 Tray 時顯示裡面有多少 Error Bin

extern void InitAutoCassetteTrayFeedTask(int iPos);
extern bool DoAutoCassetteTrayFeed(int iPos);                                   //Frank 20251217 add
#endif

