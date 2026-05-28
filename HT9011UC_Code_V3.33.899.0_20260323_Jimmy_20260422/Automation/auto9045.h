//---------------------------------------------------------------------------

#ifndef auto9045H
#define auto9045H
//---------------------------------------------------------------------------
AnsiString GetMainStatus();
AnsiString GetHandlerType();
AnsiString GetDataPath();
AnsiString GetSoftwareVersion();
AnsiString GetSetUpFileName();
AnsiString GetActTemp(int i);
AnsiString GetJamCount();
AnsiString GetSoakTime();
AnsiString GetTemperature();
AnsiString GetTempMode();
AnsiString GetTesterConnect();
AnsiString GetTesterMode();
AnsiString GetLoadCount();

AnsiString GetAutoHeight(int index);
AnsiString GetContactOffset(int index);
AnsiString GetContactTestMode();
AnsiString GetSecondSpeed();
AnsiString GetContactWaitHeight();
AnsiString GetDropHeight(int index);
AnsiString GetReleaseWait();
AnsiString GetShuttlePickOffset(int index);
AnsiString GetShuttlePickHeight(int index);
AnsiString GetShuttleReleaseHeight(int index);
AnsiString GetArmTotalForce(int index);
AnsiString GetClinderForce(int index);
AnsiString GetForcePerDevice(int index);
AnsiString GetSetNoPerPin();
AnsiString GetForcePerPin();
AnsiString GetContactForcen();
AnsiString GetStartPos(int index);
AnsiString GetDivision(int index);
int GetDimemsion(AnsiString *Data);
AnsiString GetZThickness();
AnsiString GetTrayType();
AnsiString GetPitch(int index);

//AnsiString GetIP();
//AnsiString GetPort();
//AnsiString GetCusCode();

void AmkorUDPGetSiteAndTemp();        //Steven 20120330 : ATK Site Map Monitorning
void AmkorUDPGetSite();         //Steven 20120330 : ATK Site Map Monitorning
void AmkorUDPGetTemp();         //Steven 20120330 : ATK Site Map Monitorning

int GetCategory(AnsiString *Data);
int GetBindefine(AnsiString *Data);     //Sam 20230803 : 新增OLP指令
int GetFixTrayDefine(AnsiString *Data); //Sam 20230921 : 新增 FixTray 指令

int GetDutOnOff(AnsiString *Data);
int GetMapping(AnsiString *Data);
int GetTrayForm(AnsiString *Data);
int GetPlateForm(AnsiString *Data);
int GetProductivity(AnsiString *Data);
int GetSortingCount(AnsiString *Data);
int GetSocketCount(AnsiString *Data);
int GetHeadCount(AnsiString *Data);
int GetTestResult(AnsiString *Data);
int GetHandMode(AnsiString *Data);

int GetLowYield(AnsiString *Data);
int GetByArmPerSiteDiffYield(AnsiString *Data);
int GetConsecutiveFailureAlarmByHead(AnsiString *Data);
int GetConsecutiveFailureAlarmBySocket(AnsiString *Data);
int GetAllSiteFailFor9045(AnsiString *Data);
int GetTrayFormTypeInfo(AnsiString *Data,int iType);
int GetTrayFormTypeThickness(AnsiString *Data,int iType);
int GetTrayFormTypePickUp(AnsiString *Data,int iType);
int GetTrayFormTypeName(AnsiString *Data,int iType);
int GetContactModeFor9045(AnsiString *Data);

int GetContactVacuumMode(AnsiString *Data);
int GetContactDropWait(AnsiString *Data);
int GetSlowContactSpeed(AnsiString *Data);
int GetShuttleWaitOutSideCamber(AnsiString *Data);
int GetPickShuttleDeviceAfterTested(AnsiString *Data);
int GetPickShuttleDeviceThenWaitOnShuttle(AnsiString *Data);
int GetPickShuttleDeviceTogetherFor32SiteN(AnsiString *Data);
int GetIndexArm1Height(AnsiString *Data);
int GetIndexArm2Height(AnsiString *Data);
int GetTestICCheckMode(AnsiString *Data);
int GetAboveSocket(AnsiString *Data);
int GetContactForceInfo(AnsiString *Data);

int GetHotPlateFormName(AnsiString *Data);
int GetHotPlate1(AnsiString *Data);
int GetHotPlate2(AnsiString *Data);
int GetInterfaceType(AnsiString *Data);
int GetTesterInitialMaximumTest(AnsiString *Data);
int GetTesterMaximumTest(AnsiString *Data);
int GetTesterDummyTest(AnsiString *Data);
int GetTesterStartDelay(AnsiString *Data);
int GetHotSoakTime(AnsiString *Data);
int GetHotJamSoakTime(AnsiString *Data);
int GetHotInitialWaitTime(AnsiString *Data);
int GetHotInitialStart1Time(AnsiString *Data);
int GetHotShuttleSoakTime(AnsiString *Data);
int GetChamberCoolingTemp(AnsiString *Data);
int GetHotIndexSoakTime(AnsiString *Data);
int GetHotOpenShortTime(AnsiString *Data);
int GetHotZ1Down(AnsiString *Data);
int GetHotShuttleSoakMode(AnsiString *Data);
int GetMachineTempMode(AnsiString *Data);
int GetAmbientCheck(AnsiString *Data);
int GetAmbientCheckTemp(AnsiString *Data);
int GetTemperatureOffset(AnsiString *Data);
int GetContactCountForOffsetPeriod(AnsiString *Data);
int GetContactCountForCoolDown(AnsiString *Data);
int GetInterfaceDIOInfo(AnsiString *Data);
int GetInterfaceGPIBInfo(AnsiString *Data);
int GetInterfaceRS232Info(AnsiString *Data);
int GetLotInfo(AnsiString *Data);
int GetIP(AnsiString *Data);
int GetPort(AnsiString *Data);
int GetCusCode(AnsiString *Data);
int GetStartMode(AnsiString *Data); //Sam 20221212 : 新增 StartMode 指令
int GetLotTotal(AnsiString *Data);       //Sam 20230803 : 新增OLP指令

int SetCategory(AnsiString *Data);
int SetBinDefine(AnsiString *Data);  //Sam 20230803 : 新增OLP指令
int SetFixTrayDefine(AnsiString *Data); //Sam 20230921 : 新增 FixTray 指令
int SetDutOnOff(AnsiString *Data);
int SetMapping(AnsiString *Data);
//int SetTrayForm(AnsiString *Data);
//int SetPlateForm(AnsiString *Data);
int SetSetUpFileName(AnsiString *Data);
int SetSoakTime(AnsiString *Data);
int SetTemperature(AnsiString *Data);
int SetTempMode(AnsiString *Data);
int SetTesterConnect(AnsiString *Data);
int SetTesterMode(AnsiString *Data);

int SetLowYield(AnsiString *Data);
int SetByArmPerSiteDiffYield(AnsiString *Data);
int SetConsecutiveFailureAlarmByHead(AnsiString *Data);
int SetConsecutiveFailureAlarmBySocket(AnsiString *Data);
int SetAllSiteFailFor9045(AnsiString *Data);
int SetContactModeFor9045(AnsiString *Data);
int SetContactVacuumMode(AnsiString *Data);
int SetContactDropWait(AnsiString *Data);
int SetSlowContactSpeed(AnsiString *Data);
int SetShuttleWaitOutSideCamber(AnsiString *Data);
int SetPickShuttleDeviceAfterTested(AnsiString *Data);
int SetPickShuttleDeviceThenWaitOnShuttle(AnsiString *Data);
int SetPickShuttleDeviceTogetherFor32SiteN(AnsiString *Data);
int SetIndexArm1Height(AnsiString *Data);
int SetIndexArm2Height(AnsiString *Data);
int SetTestICCheckMode(AnsiString *Data);
int SetAboveSocket(AnsiString *Data);
int SetHotPlate1(AnsiString *Data);
int SetHotPlate2(AnsiString *Data);
int SetTesterInitialMaximumTest(AnsiString *Data);
int SetTesterMaximumTest(AnsiString *Data);
int SetTesterDummyTest(AnsiString *Data);
int SetTesterStartDelay(AnsiString *Data);
int SetHotZ1Down(AnsiString *Data);
int SetHotShuttleSoakMode(AnsiString *Data);
int SetAmbientCheck(AnsiString *Data);
int SetAmbientCheckTemp(AnsiString *Data);
int SetTemperatureOffset(AnsiString *Data);
int SetContactCountForOffsetPeriod(AnsiString *Data);
int SetContactCountForCoolDown(AnsiString *Data);
int SetLotInfo(AnsiString *Data);
//AI(ht9045-v899) 20260504: return Greatek OEE start result through LotInfo_REPLY without adding a new command
int SetLotInfoGreatekOEE(AnsiString *Data, AnsiString &asErrorMsg);
int SetStartMode(AnsiString *Data); //Sam 20221212 : 新增 StartMode 指令
int ShowOLPState(int iState);
void DoOnLineReply(AnsiString *Data);
void DoULRequest(AnsiString *Data);
int  DoDLRequest(AnsiString sDLFileName);
void DoClearReportRequest(int iSwitchCase);
void AlarmMessage(AnsiString enMess, AnsiString chMess);
bool CheckCanChangeRealDummy();
bool CheckInitialOK();
bool CheckSystemStart();
bool CheckNeedCleanOut();

int GetTotalSite_ASE_KR(int &row,int &col); //ChungHung 20120104 add
AnsiString GetTestResult_ASE_KR(int row,int col); //ChungHung 20120104 add

  #ifdef DEBUG_DUTONOFF
    void DoHomeAndStart();
  #endif
    void DoOneCycle();
#endif
