//---------------------------------------------------------------------------
#ifndef ckernelH
#define ckernelH
//---------------------------------------------------------------------------

void InitialSensor();
bool CheckReceiveTrayBuildFull(int pos,int flag);
bool CheckEmptyTrayBuildIsNull(int pos,int flag);
bool CheckHasOtherEmptyTray();
bool SendTrayBufferIsFull();
void SetWarn(int code);
void ClearWarn(int code);
bool SendTrayBufferIsFull();
void ClearAllWarn();
//void CheckManualTrayChange();
void ProcessRunStatus(int);
int ScanPannelKey();
bool ScanSystemSensor();
void ProcessAlarm();
void StopAllDestroy();
bool WaitManualStartKey();
bool WaitManualStepKey();
bool WaitManualRetryKey();
void DoSystemMessage();
AnsiString InitialTestDelayStatus();    //wei 20171020 (jou) ©µ¿ðª¬ºAÅã¥Ü
int DoScanLevelSensor(); //Steven 20190503 : «ü¯¾¿ëÃÑÅv­­
#endif
