//*******************************************************
//       API-SMC(WDM)
//       FILE NAME       CSMC.H
//********************************************************
/* --------- THESE STRUCTS ARE USED BY USER --------- */
//-----------------------------
// API-SMC(WDM) Function List
//-----------------------------
#ifdef __cplusplus
extern"C"{
#endif
long WINAPI SmcWGetDeviceInfo(char *Device, short InfoType, void *Param1, void *Param2, void *Param3);
long WINAPI SmcWQueryDeviceName(short Index, char *DeviceName, char *Device);

//---------------------------
// Initialization Functions
//---------------------------
long WINAPI SmcWInit(char * DeviceName, short *DevId);
long WINAPI SmcWExit(short DevId);
long WINAPI SmcWGetErrorString(long ErrorCode, char *ErrorString);

//---------------------------------------
// Initial Parameters Setting Functions
//---------------------------------------
long WINAPI SmcWSetPulseType(short DevId, short AxisNo, short PulseMode, short DirTimer);
long WINAPI SmcWGetPulseType(short DevId, short AxisNo, short *PulseMode, short *DirTimer);
long WINAPI SmcWSetPulseDuty(short DevId, short AxisNo, short Duty);
long WINAPI SmcWGetPulseDuty(short DevId, short AxisNo, short *Duty);
long WINAPI SmcWSetEncType(short DevId, short AxisNo, short EncType);
long WINAPI SmcWGetEncType(short DevId, short AxisNo, short *EncType);
long WINAPI SmcWSetCtrlTypeOut(short DevId, short AxisNo, short CtrlOut1, short CtrlOut2, short CtrlOut3);
long WINAPI SmcWGetCtrlTypeOut(short DevId, short AxisNo, short *CtrlOut1, short *CtrlOut2, short *CtrlOut3);
long WINAPI SmcWSetCtrlTypeIn(short DevId, short AxisNo, short CtrlIn);
long WINAPI SmcWGetCtrlTypeIn(short DevId, short AxisNo, short *CtrlIn);
long WINAPI SmcWSetOrgLog(short DevId, short AxisNo, short OrgLog);
long WINAPI SmcWGetOrgLog(short DevId, short AxisNo, short *OrgLog);
long WINAPI SmcWSetCtrlInOutLog(short DevId, short AxisNo, short CtrlInOutLog);
long WINAPI SmcWGetCtrlInOutLog(short DevId, short AxisNo, short* CtrlInOutLog);
long WINAPI SmcWSetErcMode(short DevId, short AxisNo, short ErcMode);
long WINAPI SmcWGetErcMode(short DevId, short AxisNo, short *ErcMode);
long WINAPI SmcWSetErcAlmClearTime(short DevId, short AxisNo, short ErcTime, short ErcOffTimer, short AlmTime);
long WINAPI SmcWGetErcAlmClearTime(short DevId, short AxisNo, short *ErcTime, short *ErcOffTimer, short *AlmTime);
long WINAPI SmcWSetOrgMode(short DevId, short AxisNo, short LimitTurn, short OrgType, short EndDir, short ZCount);
long WINAPI SmcWGetOrgMode(short DevId, short AxisNo, short *LimitTurn, short *OrgType, short *EndDir, short *ZCount);
long WINAPI SmcWSetSAccelType(short DevId, short AxisNo, short SAccelType);
long WINAPI SmcWGetSAccelType(short DevId, short AxisNo, short *SAccelType);
long WINAPI SmcWSetInFilterType(short DevId, short AxisNo, short FilterType);
long WINAPI SmcWGetInFilterType(short DevId, short AxisNo, short *FilterType);
long WINAPI SmcWSetSDMode(short DevId, short AxisNo, short SDMode);
long WINAPI SmcWGetSDMode(short DevId, short AxisNo, short *SDMode);
long WINAPI SmcWSetCounterMode(short DevId, short AxisNo, short ClrCounterLtc, short LtcMode, short ClrCounterClr, short ClrMode);
long WINAPI SmcWGetCounterMode(short DevId, short AxisNo, short *ClrCounterLtc, short *LtcMode, short *ClrCounterClr, short *ClrMode);
long WINAPI SmcWSetSoftLimit(short DevId, short AxisNo, short PLimMode, short MLimMode, long PLimCount, long MLimCount);
long WINAPI SmcWGetSoftLimit(short DevId, short AxisNo, short *PLimMode, short *MLimMode, long *PLimCount, long *MLimCount);
long WINAPI SmcWSetSoftLimitEx(short DevId, short AxisNo, short PLimMode, short MLimMode, long PLimCount, long MLimCount);				//+
long WINAPI SmcWGetSoftLimitEx(short DevId, short AxisNo, short *PLimMode, short *MLimMode, long *PLimCount, long *MLimCount);			//+
long WINAPI SmcWSetInitParam(short DevId, short AxisNo);
long WINAPI SmcWGetInitParam(short DevId, short AxisNo, short *InitParam);

//------------------------------------
// Basic Operation Setting Functions
//------------------------------------
long WINAPI SmcWSetReady(short DevId, short AxisNo, short MotionType, short StartDir);
long WINAPI SmcWSetReadyEx(short DevId, short AxisNo, short MotionType, short StartDir);
long WINAPI SmcWGetReady(short DevId, short AxisNo, short *MotionType, short *StartDir);
long WINAPI SmcWSetResolveSpeed(short DevId, short AxisNo, double ResolveSpeed);
long WINAPI SmcWGetResolveSpeed(short DevId, short AxisNo, double *ResolveSpeed);
long WINAPI SmcWSetStartSpeed(short DevId, short AxisNo, double StartSpeed);
long WINAPI SmcWGetStartSpeed(short DevId, short AxisNo, double *StartSpeed);
long WINAPI SmcWSetTargetSpeed(short DevId, short AxisNo, double TargetSpeed);
long WINAPI SmcWGetTargetSpeed(short DevId, short AxisNo, double *TargetSpeed);
long WINAPI SmcWSetAccelTime(short DevId, short AxisNo, double AccelTime);
long WINAPI SmcWGetAccelTime(short DevId, short AxisNo, double *AccelTime);
long WINAPI SmcWSetDecelTime(short DevId, short AxisNo, double DecelTime);
long WINAPI SmcWGetDecelTime(short DevId, short AxisNo, double *DecelTime);
long WINAPI SmcWSetSSpeed(short DevId, short AxisNo, double SSpeed);
long WINAPI SmcWGetSSpeed(short DevId, short AxisNo, double *SSpeed);
long WINAPI SmcWSetStopPosition(short DevId, short AxisNo, short Coodinate, long StopPosition);
long WINAPI SmcWGetStopPosition(short DevId, short AxisNo, short Coodinate, long *StopPosition);
long WINAPI SmcWSetSync(short DevId, short SyncAxis, short SyncChip, short SyncBoard);
long WINAPI SmcWGetSync(short DevId, short *SyncAxis, short *SyncChip, short *SyncBoard);
long WINAPI SmcWSetZCountMotion(short DevId, short AxisNo, short ZMoveCount, short ZLog);
long WINAPI SmcWGetZCountMotion(short DevId, short AxisNo, short *ZMoveCount, short *ZLog);

//----------------------------------------
//  Extended Operation Setting Functions 
//----------------------------------------
long WINAPI SmcWSetBankNumber(short DevId, short AxisNo, short BankNum);
long WINAPI SmcWGetBankNumber(short DevId, short AxisNo, short *BankNum);
long WINAPI SmcWSetBankReady(short DevId, short AxisNo, short MotionType);
long WINAPI SmcWGetBankReady(short DevId, short AxisNo, short *MotionType);
long WINAPI SmcWSetBankDistance(short DevId, short AxisNo, short BankNo, long StopPosition);
long WINAPI SmcWGetBankDistance(short DevId, short AxisNo, short BankNo, long *StopPosition);
long WINAPI SmcWSetBankResolveSpeed(short DevId, short AxisNo, short BankNo, double ResolveSpeed);
long WINAPI SmcWGetBankResolveSpeed(short DevId, short AxisNo, short BankNo, double *ResolveSpeed);
long WINAPI SmcWSetBankStartSpeed(short DevId, short AxisNo, short BankNo, double StartSpeed);
long WINAPI SmcWGetBankStartSpeed(short DevId, short AxisNo, short BankNo, double *StartSpeed);
long WINAPI SmcWSetBankTargetSpeed(short DevId, short AxisNo, short BankNo, double TargetSpeed);
long WINAPI SmcWGetBankTargetSpeed(short DevId, short AxisNo, short BankNo, double *TargetSpeed);
long WINAPI SmcWSetBankAccelTime(short DevId, short AxisNo, short BankNo, double AccelTime);
long WINAPI SmcWGetBankAccelTime(short DevId, short AxisNo, short BankNo, double *AccelTime);
long WINAPI SmcWSetBankDecelTime(short DevId, short AxisNo, short BankNo, double DecelTime);
long WINAPI SmcWGetBankDecelTime(short DevId, short AxisNo, short BankNo, double *DecelTime);
long WINAPI SmcWSetBankSSpeed(short DevId, short AxisNo, short BankNo, double SSpeed);
long WINAPI SmcWGetBankSSpeed(short DevId, short AxisNo, short BankNo, double *SSpeed);
long WINAPI SmcWSetBankInterpolation(short DevId, short AxisNo, short BankNo, short InterType, short InterAxis, short Reserved);
long WINAPI SmcWGetBankInterpolation(short DevId, short AxisNo, short BankNo, short *InterType, short *InterAxis, short *Reserved);
long WINAPI SmcWSetBankArcPoint(short DevId, short AxisNo, short BankNo, double ArcSpeed, long Center_X, long Center_Y, long End_X, long End_Y);
long WINAPI SmcWGetBankArcPoint(short DevId, short AxisNo, short BankNo, double *ArcSpeed, long *Center_X, long *Center_Y, long *End_X, long *End_Y);
long WINAPI SmcWSetBankArcParam(short DevId, short AxisNo, short BankNo, short ArcStrtDir, short AutoEndPointPull);
long WINAPI SmcWGetBankArcParam(short DevId, short AxisNo, short BankNo, short *ArcStrtDir, short *AutoEndPointPull);
long WINAPI SmcWSetBankContinuation(short DevId, short AxisNo, short BankNo, long ContType);
long WINAPI SmcWGetBankContinuation(short DevId, short AxisNo, short BankNo, long *ContType);

//-----------------------------------
// Control Signal Setting Functions
//-----------------------------------
long WINAPI SmcWSetAlarmClear(short DevId, short AxisNo);
long WINAPI SmcWSetLimitMask(short DevId, short AxisNo, short LimitMask, short LimitMaskEnable);
long WINAPI SmcWGetLimitMask(short DevId, short AxisNo, short *LimitMask);
long WINAPI SmcWSetDigitalOut(short DevId, short AxisNo, short OutData, short OutDataEnable);
long WINAPI SmcWGetDigitalOut(short DevId, short AxisNo, short *OutData);
long WINAPI SmcWGetDigitalIn(short DevId, short AxisNo, short *InData);
long WINAPI SmcWSetHoldOff(short DevId, short AxisNo, short HoldOff);
long WINAPI SmcWGetHoldOff(short DevId, short AxisNo, short *HoldOff);
long WINAPI SmcWSetErcOut(short DevId, short AxisNo, short ErcOn);
long WINAPI SmcWGetAlarmCode(short DevId, short AxisNo, short *AlarmCode);

//----------------------------------
// Operation Status Read Functions
//----------------------------------
long WINAPI SmcWSetOutPulse(short DevId, short AxisNo, long OutPulse);
long WINAPI SmcWGetOutPulse(short DevId, short AxisNo, long *OutPulse);
long WINAPI SmcWSetCountPulse(short DevId, short AxisNo, long CountPulse);
long WINAPI SmcWGetCountPulse(short DevId, short AxisNo, long *CountPulse);
long WINAPI SmcWGetPulseStatus(short DevId, short AxisNo, short *PulseSts);
long WINAPI SmcWGetMoveStatus(short DevId, short AxisNo, short *MoveSts);
long WINAPI SmcWGetStopStatus(short DevId, short AxisNo, short *StopSts);
long WINAPI SmcWGetLimitStatus(short DevId, short AxisNo, short *LimitSts);
long WINAPI SmcWGetLatchOutPulse(short DevId, short AxisNo, long *OutPulse);
long WINAPI SmcWGetLatchCountPulse(short DevId, short AxisNo, long *CountPulse);
long WINAPI SmcWGetBankNo(short DevId, short AxisNo, short *BankNo);

long WINAPI SmcWGetMoveSpeed(short DevId, short AxisNo, double *MoveSpeed);
long WINAPI SmcWGetZCount(short DevId, short AxisNo, short *MoveZCount);
long WINAPI SmcWGetCtrlInOutStatus(short DevId, short AxisNo, short *CtrlSts);

//-----------------------------
// Motor Operation Functions
//-----------------------------
long WINAPI SmcWMotionStart(short DevId, short AxisNo);
long WINAPI SmcWMotionStop(short DevId, short AxisNo);
long WINAPI SmcWMotionDecStop(short DevId, short AxisNo);
long WINAPI SmcWMotionChange(short DevId, short AxisNo);
long WINAPI SmcWSetMotionChangeReady(short DevId, short AxisNo, short ChangeType);
long WINAPI SmcWGetMotionChangeReady(short DevId, short AxisNo, short *ChangeType);
long WINAPI SmcWSyncMotionStart(short DevId, short AxisNo);

//------------------
// Event Functions
//------------------
long WINAPI SmcWStopEvent(short DevId, short AxisNo, HWND hMsgWnd, short EventMode);
long WINAPI SmcWBankEvent(short DevId, short AxisNo, HWND hMsgWnd, short EventMode, short BankNo);
long WINAPI SmcWCountEvent(short DevId, short AxisNo, HWND hMsgWnd, short EventMode, short CountType, long Count);
long WINAPI SmcWIrqEvent(short DevId, short AxisNo, HWND hMsgWnd, short EventMode, short EventType);

//------------------
// Direct access Functions
//------------------
long WINAPI SmcWSetOperationMode(short DevId, short AxisNo, short Direct, short ParamSel);		//+
long WINAPI SmcWGetOperationMode(short DevId, short AxisNo, short *Direct, short *ParamSel);	//+
long WINAPI SmcWSetSpeedDirect( short Id, short AxisNo, short PRFL, short PRFH );				//+
long WINAPI SmcWGetSpeedDirect( short Id, short AxisNo, short *PRFL, short *PRFH);				//+
long WINAPI SmcWSetAccDecDirect( short Id, short AxisNo, short PRUR, short PRDR);				//+
long WINAPI SmcWGetAccDecDirect( short Id, short AxisNo, short *PRUR, short *PRDR);				//+
long WINAPI SmcWSetSDPointDirect( short Id, short AxisNo, long PRDP);							//+
long WINAPI SmcWGetSDPointDirect( short Id, short AxisNo, long *PRDP);							//+
long WINAPI SmcWSetPositionDirect( short Id, short AxisNo, long PRMV);							//+
long WINAPI SmcWGetPositionDirect( short Id, short AxisNo, long *PRMV);							//+
long WINAPI SmcWSetMagnificationDirect( short Id, short AxisNo, short PRMG);					//+
long WINAPI SmcWGetMagnificationDirect( short Id, short AxisNo, short *PRMG);					//+

//------------------
// FIFOLatch Functions
//------------------
long WINAPI SmcWResetLatchFIFO(short DevId);														//+
long WINAPI SmcWSetFIFOLatchSrc(short DevId, short AxisNo, short LatchAxisNo, short Enable);		//+
long WINAPI SmcWGetFIFOLatchSrc(short DevId, short AxisNo, short *LatchAxisNo, short *Enable);		//+
long WINAPI SmcWGetLatchDataFromBuffer(short DevId, short BufferNo, short *AxisCounterNo, short *LatchDataCnt, long *LatchDataTable);		//+
long WINAPI SmcWGetLatchFIFOLength(short DevId, short AxisNo, short *length);						//+
long WINAPI SmcWGetLatchDataFromBufferEx(short DevId, short BufferNo, short *AxisCounterNo, short *LatchDataCnt, long *LatchDataTable, short *UpCnt, short *DownCnt);//+

#ifdef __cplusplus
}
#endif
