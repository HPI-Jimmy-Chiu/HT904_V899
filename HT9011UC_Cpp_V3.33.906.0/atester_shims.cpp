// =============================================================================
//  atester_shims.cpp  --  offline bodies for the W6.4 TESTER/INDEX cross-module shims
//
//  Translation wave: W6.4 (TESTER/INDEX ENGINE -- atester.cpp)
//  Translator: AI(W6.4-TESTER) 20260626
//
//  All bodies are CONSERVATIVE offline defaults over the Sim HAL.  See
//  atester_shims.h for per-symbol rationale + golden provenance.
//    * 32Site / Front / Rear "Do*" report COMPLETE (true) so the gated tester SM
//      skeletons advance; "Init*" no-op; cursor ints home to the golden value =1.
//    * ProcessCount void bodies no-op; DoLowYieldAlarm returns 0 (no alarm).
//    * fContact->fShow=false (so ProcessTestResult takes "can set bin");
//      Do_ROILearning()=true (learning "done").
//    * ADAM_* EP DAQ: writes no-op, Alarm()=false (no EP leak) so EP-check advances.
//
//  Big5: Chinese comments preserved as UTF-8 (cp950).  ZERO U+FFFD.
// =============================================================================
#include "atester_shims.h"
#include "csystem.h"                 // IsInArmCleanOutFinish / IsInArmOneCycleFinish /
                                     // CanYieldAlarmRemainInSHT / CheckIndexConnect declarations

// ===========================================================================
//  csystem.h cross-module predicates referenced ONLY by the tester/index engine
//  (their real homes are ainarm / index-connect HW, NOT translated this wave).
//  Offline-safe defaults (the W6 contract):
//    * IsInArmCleanOutFinish / IsInArmOneCycleFinish -> false (no clean-out/one-cycle
//      pending) so DoTestY case 1 takes the normal dispatch (-> Task=50, golden :4890).
//    * CanYieldAlarmRemainInSHT -> false (no yield-alarm half-cycle pending).
//    * CheckIndexConnect -> 0 (all index heaters "connected", no WAR0360).
//  These are the csystem.h-declared bodies; no other translated TU defines them.
// ===========================================================================
bool IsInArmCleanOutFinish(int /*iIsOneCycle*/) { return false; }  // golden csystem.h:89
bool IsInArmOneCycleFinish()                    { return false; }  // golden csystem.h:91
bool CanYieldAlarmRemainInSHT()                 { return false; }  // golden csystem.h:288
int  CheckIndexConnect()                        { return 0; }      // golden csystem.h:212 (Dell 20111130)
// TODO(W6.4b): real body golden csystem.cpp:23513, remove this stub when translated.
//   DoTestHeadMotor (atester.cpp:1732) fires the TTL RS232 SOT signal to the dual
//   TTL board.  Offline: no RS232/TTL board wired -> no-op (the SOT pulse is a
//   hardware side-effect; the index SM advances regardless of its return).
void SendTTLRS232CSOTsignal()                   {}                 // golden csystem.h:280 (Isaac 20210309)

// ---- atester_32Site cursors + bodies ---------------------------------------
int i32RTCAutoModelVerifyTask        = 1;   // golden Init home =1
int iTestSuckTestIC_TwoArm32Site_Task= 1;
int iTestTwoArm32SiteTask            = 1;
void InitTestSuckTestIC_TwoArm32Site_Task() {}
void InitTestYTwoArm32SiteTask()            {}
bool DoInterFaceErrorStep_TwoArm32Site()    { return true; }
bool DoTestSuckTestIC_TwoArm32Site()        { return true; }
bool DoTestY_TwoArm32Site()                 { return true; }
bool Do32SiteTestDestroyIC()                { return true; }
bool Do32SiteTestSuckIC()                   { return true; }

// ---- atester_ProcessCount bodies -------------------------------------------
void InitialPiggyBackFunction()             {}
void ProcessPiggyBackFunction()             {}
void RecordPiggyBackStartEnd(bool /*bStart*/) {}
void ProcessSocketPurgeCount()              {}
void ProcessShowTestStatus(int /*Index*/)   {}
void CheckContinuoussFail(int /*Index*/)    {}
void ProcessContinuoussFailForATC(int /*Index*/) {}
void CheckContinuoussPassBySocket(int /*Index*/) {}
void CheckContinuoussPass(int /*Index*/)    {}
void ProcessCount(int /*Index*/, bool /*bHasIC*/) {}    //Eastsun 20260515 F022: D7
void ProcessStartTestData(int /*Direct*/)   {}
void RecordHistroy(int /*attr*/)            {}
int  DoLowYieldAlarm(AnsiString /*AlarmCode*/, AnsiString /*ErrPart*/)      { return 0; }
int  DoLowYieldAlarmFirst(AnsiString /*AlarmCode*/, AnsiString /*ErrPart*/) { return 0; }
void ProcessQASampling(int /*Index*/)       {}
void ProcessAutoloadcellMeasureCount()      {}

// ---- aTester_Front cursors + bodies ----------------------------------------
int iFTestSuckTestICTask                  = 1;
int iFrontTestDestroyICTask               = 1;
int iFrontTestSuckICTask                  = 1;
int iTestYFrontTask                       = 1;
int iFRTCUseSocketFloatTask               = 1;
int iFRTCAutoModelVerifyTask              = 1;
int iFrontTestPurgBeforePickShuttle       = 1;
int iIndexArm2PickUpErrNeedPiggybackTask  = 1;
int iFTestSocketClampCloseTask            = 1;
int iFTestSocketClampOpenTask             = 1;
bool bArm2NeedCombine                     = false;
bool bArm1SuckFinish[4][8]                = {{false}};
int  CheckAnyCaseNeedToDoArm1()             { return 0; }
void InitTestYFrontTask()                   {}
void InitFTestSuckTestICTask()              {}
void InitFrontTestSuckICTask()              {}
void InitFrontTestDestroyICTask()           {}
void InitFrontTestPurgBeforePickShuttle()   {}
void TestZ1SetPos()                         {}
void DoArm1Suck()                           {}
void DoArm1D44VacCheck()                    {}
bool FTestNeedDestroy()                     { return false; }
bool DoFrontTestDestroyIC(bool /*bCheckZ2*/){ return true;  }
bool DoFrontTestSuckIC()                    { return true;  }
bool DoFTestSuckTestIC()                    { return true;  }
bool CheckZ1IsDown()                        { return false; }
bool DoFrontTestPurgBeforePickShuttle(int /*isp*/) { return true; }
bool DoTestYFront()                         { return true;  }
bool TestZ1OutRandge()                      { return false; }
bool TestZ1OutRandge2()                     { return false; }
bool FTestSeparateSLK(bool /*bReset*/)      { return true;  }
bool FTestCombineSLK(bool /*bReset*/)       { return true;  }
bool DoFRTCAutoModelVerify(bool /*bInitial*/){ return true; }

// ---- aTester_Rear cursors + bodies -----------------------------------------
int iBTestSuckTestICTask                  = 1;
int iRearTestDestroyICTask                = 1;
int iRearTestSuckICTask                   = 1;
int iTestYRearTask                        = 1;
int iBRTCUseSocketFloatTask               = 1;
int iBRTCGiveWayCheckTask                 = 1;
int iBRTCAutoModelVerifyTask              = 1;
int iBTestSocketClampCloseTask            = 1;
int iBTestSocketClampOpenTask             = 1;
int iRearTestPurgBeforePickShuttle        = 1;
int iIndexArm1PickUpErrNeedPiggybackTask  = 1;
bool bArm2SuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{false}};
int  CheckAnyCaseNeedToDoArm2()             { return 0; }
void InitTestYRearTask()                    {}
void InitBTestSuckTestICTask()              {}
void InitRearTestSuckICTask()               {}
void InitRearTestDestroyICTask()            {}
void InitRearTestPurgBeforePickShuttle()    {}
void TestZ2SetPos()                         {}
void DoArm2Suck()                           {}
void DoArm2D44VacCheck()                    {}
bool BTestNeedDestroy()                     { return false; }
bool DoRearTestDestroyIC(bool /*bCheckZ1*/) { return true;  }
bool DoRearTestSuckIC()                     { return true;  }
bool DoBTestSuckTestIC()                    { return true;  }
bool CheckZ2IsDown()                        { return false; }
bool DoRearTestPurgBeforePickShuttle(int /*isp*/) { return true; }
bool DoTestYRear()                          { return true;  }
bool TestZ2OutRandge()                      { return false; }
bool TestZ2OutRandge2()                     { return false; }
bool BTestCombineSLK(bool /*bReset*/)       { return true;  }
bool BTestSeparateSLK(bool /*bReset*/)      { return true;  }
bool DoBRTCAutoModelVerify(bool /*bInitial*/){ return true; }

// ---- fContact (offline contact-mode form) ----------------------------------
TfContactShim::TfContactShim() : fShow(false) {}
bool TfContactShim::Do_ROILearning() { return true; }   // offline: ROI learning "done"
bool TfContactShim::IsRun2DCheck()   { return false; }  // W6.2b1x1: offline -> not running 2DID re-check
TfContactShim *fContact = new TfContactShim();

// ---- ADAM_* EP DAQ (offline: no DAQ card) ----------------------------------
void ADAM_DirectWriteData(int /*iData*/, int /*iCh*/) {}
void ADAM_WriteVoltage(double /*dVolt*/)              {}
bool ADAM_Alarm()                                     { return false; }
bool ADAM_Alarm(int /*iCh*/)                          { return false; }
void ADAM_Rang(int /*iRange*/)                        {}

// ---- EP switch / heater-OK / NN-mode / in-arm-giveway helpers --------------
void EPSwitchOnOff(int /*iArm*/)                      {}             // offline: no EP DAQ
bool CheckHeaterOK()                                  { return true; }  // offline: heater ready
int  IsNNMode()                                       { return 0; }     // offline: not NN mode
// CheckInArmFinishAllPickerAction() / MoveInArm2XYToWait() are defined ONCE in
// acatchtray_shims.cpp (golden ainarm2.h:95 / :146).  Declared in atester_shims.h
// so atester.cpp sees them, but NOT re-defined here (would be a multiple-definition
// ODR clash at link).  TODO(W6.4b): real bodies golden ainarm2.cpp; both shims drop
// together when the in-arm give-way SM is translated.
// AI(W7T1-Integrate) 20260701: iCASE_REAL_CCD2 definition REMOVED -- moved to its golden
// file-scope home in atester.cpp (const int iCASE_REAL_CCD2..6 = 40200..40510, golden
// atester.cpp:5551-5555).  The W6.4 =9 remap was valid only while the RTC case tree was
// gated; the tree is now ACTIVE so =9 would collide with the active `case 9:`.

// ---- CCDInterfaceForm (offline CCD identification form) ---------------------
TCCDInterfaceFormShim::TCCDInterfaceFormShim() : bAtestScanCCDProgram(false) {}
void TCCDInterfaceFormShim::CCDTimerOnOff(bool /*bOn*/) {}
bool TCCDInterfaceFormShim::CCDRunExec()               { return false; }
TCCDInterfaceFormShim *CCDInterfaceForm = new TCCDInterfaceFormShim();

// ---- fAutomation (offline host-command form) -------------------------------
TfAutomationShim::TfAutomationShim() {}
void TfAutomationShim::DoCommandBuffer(AnsiString /*Command*/, AnsiString /*SubCommand*/,
                                       AnsiString /*AlarmMessage*/, int /*ErrType*/, AnsiString /*AlarmID*/) {}
bool TfAutomationShim::GetEventNum(int & /*num*/, AnsiString /*cmdstr*/, AnsiString /*numstr*/) { return false; }
bool TfAutomationShim::GetEventResult(int /*num*/)     { return false; }
int  TfAutomationShim::GetEventStrResult(int /*num*/)  { return 0; }
void TfAutomationShim::ClearEvent(int /*num*/)         {}
TfAutomationShim *fAutomation = new TfAutomationShim();

// ---- fObserver (offline OEE observer) --------------------------------------
TfObserverShim::TfObserverShim() : bTestIndexZ(false) {}
void TfObserverShim::RecordInArmTime()                {}  // W6.2b1x1: OEE time bookkeeping no-op
void TfObserverShim::AddTimeData(int /*iRow*/, double /*Time*/) {} // W6.2b1x1: OEE time bookkeeping no-op
TfObserverShim *fObserver = new TfObserverShim();

// ---- fiosetview (offline index-suck IO view) -------------------------------
TfiosetviewShim::TfiosetviewShim() { for(int a=0;a<2;a++) for(int b=0;b<4;b++) for(int c=0;c<8;c++) bIndexSuck[a][b][c]=false; }
TfiosetviewShim *fiosetview = new TfiosetviewShim();

// ---- COM2 (offline DTK RTC/CCD serial-comm module, W5) ---------------------
TCOM2Shim::TCOM2Shim() : bCCDDummyRum(true) {}   // offline: dummy-run true so RTC/CCD branches short-circuit
void TCOM2Shim::DoReleaseAndInspEnd() {}
TCOM2Shim *COM2 = new TCOM2Shim();
