// =============================================================================
//  TempCtrl/TriTemp.h  --  HT-1032 TRI-TEMPERATURE (三溫機) ENGINE public interface
//
//  Faithful translation of golden TempCtrl/TriTemp.h (78 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-W7-TriTemp) 20260807
//  Translation wave: W906-W7-TriTemp (pure-translation completion campaign;
//  first file under TempCtrl/ -- the directory is created by this wave).
//
//  ROLE: declares the tri-temperature engine's entry points -- the three scan
//  functions (DoTriTempState / DoTriTempState_1032 / DoTriTempState_AlwaysCheck),
//  the defrost subsystem, the tri-temp safety-door interlocks, the dew-point /
//  dry-air / ion-bar pollers and the ATC temperature-reached gates.
//
//  SCOPE NOTE -- READ BEFORE ASSUMING A DECLARATION HERE HAS A BODY.
//  The counts, measured this wave: golden TriTemp.h declares 62 functions;
//  golden TriTemp.cpp defines 31.  30 of those 31 are declared here -- the
//  31st, DoorOpenAlarmForTriTemp (golden TriTemp.cpp:1870, the 13-message
//  door-alarm catalogue, carries a default argument), is deliberately
//  file-local in golden and is NOT exported here either.
//  That leaves 32 declarations in this header with NO body in this TU: they
//  live in other, still-untranslated golden TUs (main.cpp, cTemperFrom.cpp,
//  uTemp_Set.cpp, TriMachineDeforst.cpp) -- e.g. CheckTriTemperature,
//  CheckTriTemperatureReal, CheckTriTempShuttlePos, fWriteAutoTuningTemperature,
//  fBackToRoomTemperature, fUseAirCoolerToCoolDown, CopyFolder,
//  fInitTaskBackUpSystemData.  They are reproduced VERBATIM (a declaration
//  costs nothing and keeps this header a faithful mirror of golden), but
//  nothing in the ported tree may CALL them yet -- there is no definition to
//  link against, and doing so would be an undefined reference, not a silent
//  no-op.
//    ONE EXCEPTION worth naming: `Del_Tree(AnsiString)` below is ALSO declared
//    by csystem.h:284 with an identical signature (different parameter name
//    only), and csystem.cpp DOES define it -- so that one declaration is
//    already satisfied.  Re-declaring it is legal and harmless.
//
//  VCL/Borland conversions: golden's `__fastcall` is dropped from every
//  DECLARATION here, exactly as TriTemp.cpp drops it from the matching
//  definition, so the two still mangle to the same symbol (the house rule is
//  "if a declaration carries it, the definition must too" -- here neither
//  does).  Golden's COMMENTED-OUT declarations keep their text verbatim,
//  __fastcall and all.  Default arguments stay on the declaration only
//  (Check_DefrostALLTempReady(int iIndex=0),
//  fcheckUltraTempatureKit_Function(..., double dSetcheckTempRange = 0.0)),
//  matching golden.  `PACKAGE` / `__property` / `__published` do not occur.
//
//  AnsiString comes from vclcompat (1-based) -- golden relied on <vcl.h> being
//  pulled in first by IncludeAllHeader.H; this header is self-sufficient.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef TriTempH
#define TriTempH

// AI(W906-W7-TriTemp) 20260807: golden relied on <vcl.h> having already been
//   pulled in by IncludeAllHeader.H before this header was reached.  Made
//   self-sufficient here so TriTemp.cpp can include it first, per house rule.
#include "vclcompat/vcl_compat.h"   // AnsiString (1-based)

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
bool fcheckDefrostTempature(int iMachineDevice);
void fAllDevicePosDefrostON_OFF(AnsiString strOnOff);
//void fUseDeviceReHeat(AnsiString strDevice,int iHeatTemperature);
void fUseDeviceReHeat(int iDevice, int iHeatTemperature);
void fCheckDewPointStatus();
int fWriteAutoTuningTemperature(int iTempature1,int iTemperature2);
void fcheckHeadAndHeadTempDiffOverRange();
bool fHighTemperatureExhaustAir();
void fUseDeviceSetOffset_ForEnhancedInitTempOffsetFunction(int iIndex,int iEnhancedInitTempOffset);
bool fcheckTempature_ForEnhancedInitTempOffsetFunction(int iIndex,int iEnhancedInitTempOffset);
bool UseIndexVacuumWhenDefrosting(int iIndex, int iOnOff);
bool DoDelayTimeAfterFixDoorOpen();
bool CheckDoorOpenForTriTemp();
bool fcheckDockingAreaOpen_EnableFunction();
void DoTriTempState_AlwaysCheck();
void fCheckIndexArmFrostSensor();
void fCheckIonBarAirStatus();
void fCheckDryAirStatus();
bool  fCheckTriTempHumidityAnomaly();
//AnsiString  fCheckUltraHighTemperatureKit();
bool fcheckUltraTempatureKit_Function(int iTemperature,double dSetcheckTempRange = 0.0);
bool fcheckLoadBoardTemperaturetStatus();
bool fCheckATCTempOK();
bool fCheckLowTempExecTime();
bool fExecuteFsatCoolDownFunction();
bool fcheckFsatCoolDownFunction();
//AnsiString __fastcall fcheckTemperaturetIsWithInRange(int iSetTemperature,double dRange);

void  fReadGerneralMachineDefineData_ForTriTempMachine();
void  fWriteGerneralMachineDefineData_ForTriTempMachine();

void CopyFolder(AnsiString SourceName);
void Del_Tree(AnsiString SourceName);
//bool __fastcall fBackUpSystemData();
bool fBackToRoomTemperature();

bool fUseAirCoolerToCoolDown(bool bOnOff);
bool fcheckUseAirCoolerToCoolDownFunction();
bool fUseChangeTriTempKit(bool bOnOff);
bool fcheckUseChangTriTempKitFunction();

bool fUseColdTemperatureAutoDefrost();
void CheckTriTemperatureReal_1016();
//void SwitchRefrigerator(AnsiString strDevice, int iOnOff);
void SwitchRefrigerator(int iDevice, int iOnOff);

bool CheckPositionIsRange(int iIndex,int iPos);
bool fIntiTaskCheckUltraHighTemperatureKit();

bool fInitTaskBackUpSystemData();               //Hmy 20200407 Add Backup SystemData
bool Check_AirVolume_Lmt();
bool Check_ALLTempFirstReady();
bool Check_TriTemp_OverRang();
bool Check_DefrostALLTempReady(int iIndex=0);
bool Retrrn_InitialOK();
int  Retrrn_AccessLevel();
//Ztex 2023.04.19 Add HT-1032 TriTemp Function
#endif

