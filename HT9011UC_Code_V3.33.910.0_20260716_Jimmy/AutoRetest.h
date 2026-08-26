//---------------------------------------------------------------------------

#ifndef AutoRetestH
#define AutoRetestH
//---------------------------------------------------------------------------

extern int iAutoRetestTask;
extern int iTrayArm_AutoRT_Task;
extern int iTrayArm_PickFromAuto_AutoRT_Task;
extern int iTrayArm_PlaceToLoad_AutoRT_Task;
extern int iAuto_AutoRT_Task[MAX_AUTO_TRAY];
extern int iLoader_AutoRT_Task;

extern bool bTrayArmOK_AutoRT;
extern bool bAutoOK_AutoRT[MAX_AUTO_TRAY];
extern bool bLoadOK_AutoRT;

extern bool DoAutoRetest(bool bReset=false);

extern bool DoTrayArm_AutoRT(bool bReset=false);
extern bool DoPickFromAuto_AutoRT(int Pos, bool bReset=false);
extern bool DoPlaceToLoader_AutoRT(bool bReset=false);

extern bool DoAuto_AutoRT(int Pos, bool bReset=false);
extern bool DoLoader_AutoRT(bool bReset=false);

extern int GPIB_RemoteCommand(AnsiString asCommand);    //jou 2015-09-21 Auto Retest function
extern AnsiString GPIB_QueryData(AnsiString asCommand); //jou 2015-09-21 Auto Retest function
extern int GPIB_SetData(AnsiString asCommand);          //jou 2015-09-21 Auto Retest function
#endif
