//---------------------------------------------------------------------------

#ifndef atester_ProcessCountH
#define atester_ProcessCountH
//---------------------------------------------------------------------------

void InitialPiggyBackFunction();
void ProcessPiggyBackFunction();
void RecordPiggyBackStartEnd(bool bStart);
void ProcessSocketPurgeCount();
void ProcessShowTestStatus(int Index);
void CheckContinuoussFail(int Index);
void ProcessContinuoussFailForATC(int Index);                                   //Steven 20151123 : Continue Fail Temp Offset for ATC
void CheckContinuoussPassBySocket(int Index);
void CheckContinuoussPass(int Index);
void ProcessCount(int Index, bool bHasIC=true);                                 //Eastsun 20260515 F022: D7

void ProcessStartTestData(int Direct);
void __fastcall RecordHistroy(int attr);

int DoLowYieldAlarm(AnsiString AlarmCode, AnsiString ErrPart);                  //Steven 20180627 (wei) : 整合Low Yield Alarm
int DoLowYieldAlarmFirst(AnsiString AlarmCode, AnsiString ErrPart);
void ProcessQASampling(int Index);                                              //Steven 20190326 : QA Sampling
void ProcessAutoloadcellMeasureCount();                                         // kevin 20190907 add load cell Measure     //kevin 20190907 Arm 測區次數道量測 功能;
#endif
