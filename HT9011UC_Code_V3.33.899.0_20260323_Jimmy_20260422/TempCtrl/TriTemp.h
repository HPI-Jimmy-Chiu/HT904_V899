//---------------------------------------------------------------------------

#ifndef TriTempH
#define TriTempH

//---------------------------------------------------------------------------
void DoTriTempState();

//Ztex 2023.04.19 Add HT-1032 TriTemp Function
bool fCheckMotorMoveCount_Shuttle(int iSht);                 //Hmy 20170215 add 三溫機 Shuttle Motor移動必須計數
bool CheckTriTempAndHumidityAnomaly();
bool CheckTriTempTrayArmFloodgateIsSafe(int iPos);
bool CheckTriTempTrayArmFloodgateIsAllClose();
bool CheckTriTempShuttleFloodgateIsSafe();
bool CheckTriTempShuttleFloodgateIsAllClose();
bool TriTempShuttleFloodgateOpen(int iOnOff);
bool IsTriSafeDoor6LockCheck();
void DoTriTempState_1032();
void CheckTriTemperature();
void CheckTriTemperatureReal();
bool CheckShuttleFloodgateSafe();
bool CheckTriTempShuttlePos();
//void fTriMachineSelectDeviceDefrost_Start(AnsiString  strDevicePos);
void fTriMachineSelectDeviceDefrost_Start(int iDevicePos);
//void fTriMachineSelectDeviceDefrost_End(AnsiString  strDevicePos);
void fTriMachineSelectDeviceDefrost_End(int iDevicePos);
bool fcheckDefrostStatus();
//bool __fastcall fcheckDefrostTempature(AnsiString strMachineDevice);
bool __fastcall fcheckDefrostTempature(int iMachineDevice);
void fAllDevicePosDefrostON_OFF(AnsiString strOnOff);
//void fUseDeviceReHeat(AnsiString strDevice,int iHeatTemperature);
void fUseDeviceReHeat(int iDevice, int iHeatTemperature);
void fCheckDewPointStatus();
int fWriteAutoTuningTemperature(int iTempature1,int iTemperature2);
void fcheckHeadAndHeadTempDiffOverRange();
bool fHighTemperatureExhaustAir();
void fUseDeviceSetOffset_ForEnhancedInitTempOffsetFunction(int iIndex,int iEnhancedInitTempOffset);
bool __fastcall fcheckTempature_ForEnhancedInitTempOffsetFunction(int iIndex,int iEnhancedInitTempOffset);
bool __fastcall UseIndexVacuumWhenDefrosting(int iIndex, int iOnOff);
bool __fastcall DoDelayTimeAfterFixDoorOpen();
bool __fastcall CheckDoorOpenForTriTemp();
bool __fastcall fcheckDockingAreaOpen_EnableFunction();
void DoTriTempState_AlwaysCheck();
void fCheckIndexArmFrostSensor();
void fCheckIonBarAirStatus();
void fCheckDryAirStatus();
bool  fCheckTriTempHumidityAnomaly();
//AnsiString  fCheckUltraHighTemperatureKit();
bool __fastcall fcheckUltraTempatureKit_Function(int iTemperature,double dSetcheckTempRange = 0.0);
bool __fastcall fcheckLoadBoardTemperaturetStatus();
bool __fastcall fCheckATCTempOK();
bool __fastcall fCheckLowTempExecTime();
bool __fastcall fExecuteFsatCoolDownFunction();
bool __fastcall fcheckFsatCoolDownFunction();
//AnsiString __fastcall fcheckTemperaturetIsWithInRange(int iSetTemperature,double dRange);

void  fReadGerneralMachineDefineData_ForTriTempMachine();
void  fWriteGerneralMachineDefineData_ForTriTempMachine();

void CopyFolder(AnsiString SourceName);
void Del_Tree(AnsiString SourceName);
//bool __fastcall fBackUpSystemData();
bool __fastcall fBackToRoomTemperature();

bool __fastcall fUseAirCoolerToCoolDown(bool bOnOff);
bool __fastcall fcheckUseAirCoolerToCoolDownFunction();
bool __fastcall fUseChangeTriTempKit(bool bOnOff);
bool __fastcall fcheckUseChangTriTempKitFunction();

bool __fastcall fUseColdTemperatureAutoDefrost();
void CheckTriTemperatureReal_1016();
//void SwitchRefrigerator(AnsiString strDevice, int iOnOff);
void SwitchRefrigerator(int iDevice, int iOnOff);

bool CheckPositionIsRange(int iIndex,int iPos);
bool fIntiTaskCheckUltraHighTemperatureKit();

bool __fastcall fInitTaskBackUpSystemData();    //Hmy 20200407 Add Backup SystemData
bool Check_AirVolume_Lmt();
bool Check_ALLTempFirstReady();
bool Check_TriTemp_OverRang();
bool Check_DefrostALLTempReady(int iIndex=0);
bool Retrrn_InitialOK();
int  Retrrn_AccessLevel();
//Ztex 2023.04.19 Add HT-1032 TriTemp Function
#endif
