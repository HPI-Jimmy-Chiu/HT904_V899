// =============================================================================
//  atester_shims.h  --  W6.4 offline cross-module shims for the TESTER/INDEX engine
//
//  Translation wave: W6.4 (TESTER/INDEX ENGINE -- atester.cpp)
//  Translator: AI(W6.4-TESTER) 20260626
//
//  WHY THIS FILE EXISTS
//  --------------------
//  The faithfully-transcribed tester/index engine (atester.cpp) reaches a set of
//  cross-module subsystems that have NO translated home this wave:
//    * atester_32Site.cpp   (4749 lines -- W6.4b sibling)
//    * atester_ProcessCount.cpp (2161 lines -- W6.4b sibling)
//    * aTester_Front.cpp / aTester_Rear.cpp (front/rear test-head SMs -- W6.4b)
//    * fContact (cContact.h, PACKAGE) -- contact-mode index-check form
//    * the ADAM_* EP-pressure DAQ API (adam6024.h)
//
//  Mirroring acatchtray_shims, this header exposes ONLY the surface the engine
//  dereferences, on transparently-named non-VCL stand-ins, with offline-safe
//  bodies (true / false / 0 / no-op) in atester_shims.cpp.  Per the W6 pattern,
//  the 32Site/Front/Rear "Do*" report COMPLETE (true) so the gated SM advances,
//  "Init*" are no-op, and their cursor ints live here (init per golden =1).
//
//  Each symbol cites its golden home.  When the real subsystem lands (W6.4b/W7),
//  these shims are replaced and the call sites bind to the real bodies unchanged.
//
//  Big5: Chinese comments decoded via cp950, preserved UTF-8.  ZERO U+FFFD.
// =============================================================================
#ifndef atester_shimsH
#define atester_shimsH

#include "vclcompat/vcl_compat.h"   // AnsiString
#include "MachineType.h"            // MAX_SOCKET_ROW / MAX_SOCKET_COL

// ===========================================================================
//  atester_32Site.cpp  -- golden atester_32Site.h.  4 call sites in DoTestHeadMotor
//  / DoTestY.  Offline: Do* = "complete" (true) so the 32-site path falls through;
//  Init* = no-op; the cursor ints are owned here (golden Init*Task home =1).
// ===========================================================================
void InitTestSuckTestIC_TwoArm32Site_Task();   // golden atester_32Site.h
void InitTestYTwoArm32SiteTask();              // golden atester_32Site.h
bool DoInterFaceErrorStep_TwoArm32Site();      // golden -- offline: handled (true)
bool DoTestSuckTestIC_TwoArm32Site();          // golden -- offline: complete (true)
bool DoTestY_TwoArm32Site();                   // golden -- offline: complete (true)
bool Do32SiteTestDestroyIC();                  // golden -- offline: complete (true)
bool Do32SiteTestSuckIC();                     // golden -- offline: complete (true)
extern int i32RTCAutoModelVerifyTask;          // golden atester_32Site.h
extern int iTestSuckTestIC_TwoArm32Site_Task;  // golden atester_32Site.h
extern int iTestTwoArm32SiteTask;              // golden atester_32Site.h

// ===========================================================================
//  atester_ProcessCount.cpp  -- golden atester_ProcessCount.h.  7+ call sites.
//  Offline: void bodies no-op; int body (DoLowYieldAlarm) returns 0 (no alarm).
// ===========================================================================
void InitialPiggyBackFunction();               // golden atester_ProcessCount.h
void ProcessPiggyBackFunction();
void RecordPiggyBackStartEnd(bool bStart);     // (also declared in atester.h; real home is ProcessCount)
void ProcessSocketPurgeCount();
void ProcessShowTestStatus(int Index);         // (also declared in atester.h)
void CheckContinuoussFail(int Index);
void ProcessContinuoussFailForATC(int Index);
void CheckContinuoussPassBySocket(int Index);
void CheckContinuoussPass(int Index);
void ProcessCount(int Index, bool bHasIC=true);// (also declared in atester.h)  //Eastsun 20260515 F022: D7
void ProcessStartTestData(int Direct);
void RecordHistroy(int attr);                  // golden -- (__fastcall removed)
int  DoLowYieldAlarm(AnsiString AlarmCode, AnsiString ErrPart);        // golden -- offline 0
int  DoLowYieldAlarmFirst(AnsiString AlarmCode, AnsiString ErrPart);   // golden -- offline 0
void ProcessQASampling(int Index);
void ProcessAutoloadcellMeasureCount();

// ===========================================================================
//  aTester_Front.cpp / aTester_Rear.cpp  -- golden aTester_Front.h / aTester_Rear.h.
//  Sibling test-head SMs (8+ call sites in DoTestY / DoTestHeadMotor).  Offline:
//  Do* = "complete" (true) so the dispatcher advances; Init* = no-op; cursor ints
//  owned here.  Front/Rear share the same offline contract as the 3 arm engines.
// ===========================================================================
//  -- Front cursors (golden aTester_Front.h) --
extern int iFTestSuckTestICTask;
extern int iFrontTestDestroyICTask;
extern int iFrontTestSuckICTask;
extern int iTestYFrontTask;
extern int iFRTCUseSocketFloatTask;
extern int iFRTCAutoModelVerifyTask;
extern int iFrontTestPurgBeforePickShuttle;
extern int iIndexArm2PickUpErrNeedPiggybackTask;
extern int iFTestSocketClampCloseTask;
extern int iFTestSocketClampOpenTask;
extern bool bArm2NeedCombine;                  //JerryYang 20180520 (wei)
extern bool bArm1SuckFinish[4][8];
int  CheckAnyCaseNeedToDoArm1();               // golden -- offline 0
void InitTestYFrontTask();
void InitFTestSuckTestICTask();                // (also declared in atester.h)
void InitFrontTestSuckICTask();
void InitFrontTestDestroyICTask();
void InitFrontTestPurgBeforePickShuttle();
void TestZ1SetPos();
void DoArm1Suck();
void DoArm1D44VacCheck();
bool FTestNeedDestroy();
bool DoFrontTestDestroyIC(bool bCheckZ2);      // golden -- offline complete (true)
bool DoFrontTestSuckIC();
bool DoFTestSuckTestIC();                       // golden -- offline complete (true)
bool CheckZ1IsDown();
bool DoFrontTestPurgBeforePickShuttle(int isp);
bool DoTestYFront();                            // golden -- offline complete (true)
bool TestZ1OutRandge();
bool TestZ1OutRandge2();
bool FTestSeparateSLK(bool bReset=false);
bool FTestCombineSLK(bool bReset=false);
bool DoFRTCAutoModelVerify(bool bInitial);
//  -- Rear cursors (golden aTester_Rear.h) --
extern int iBTestSuckTestICTask;
extern int iRearTestDestroyICTask;
extern int iRearTestSuckICTask;
extern int iTestYRearTask;
extern int iBRTCUseSocketFloatTask;
extern int iBRTCGiveWayCheckTask;
extern int iBRTCAutoModelVerifyTask;
extern int iBTestSocketClampCloseTask;
extern int iBTestSocketClampOpenTask;
extern int iRearTestPurgBeforePickShuttle;
extern int iIndexArm1PickUpErrNeedPiggybackTask;
extern bool bArm2SuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL];
int  CheckAnyCaseNeedToDoArm2();               // golden -- offline 0
void InitTestYRearTask();
void InitBTestSuckTestICTask();
void InitRearTestSuckICTask();
void InitRearTestDestroyICTask();
void InitRearTestPurgBeforePickShuttle();
void TestZ2SetPos();
void DoArm2Suck();
void DoArm2D44VacCheck();
bool BTestNeedDestroy();
bool DoRearTestDestroyIC(bool bCheckZ1);       // golden -- offline complete (true)
bool DoRearTestSuckIC();
bool DoBTestSuckTestIC();                        // golden -- offline complete (true)
bool CheckZ2IsDown();
bool DoRearTestPurgBeforePickShuttle(int isp);
bool DoTestYRear();                             // golden -- offline complete (true)
bool TestZ2OutRandge();
bool TestZ2OutRandge2();
bool BTestCombineSLK(bool bReset=false);
bool BTestSeparateSLK(bool bReset=false);
bool DoBRTCAutoModelVerify(bool bInitial);

// ===========================================================================
//  fContact (cContact.h, PACKAGE TfContact* fContact) -- contact-mode index-check
//  form.  atester.cpp derefs it ~57x; the only value read on an ACTIVE skeleton
//  path is fContact->fShow (ProcessTestResult :2693).  Minimal offline mirror:
//  fShow=false so the bin-set guard takes its "can set bin" branch.  The contact
//  SM DoIndecxCHECkFunction itself is NOT called from atester.cpp (see roadmap).
// ===========================================================================
class TfContactShim
{
public:
    bool fShow;                                 // golden cContact.h -- form visible? (offline false)
    bool Do_ROILearning();                      // golden -- RTC ROI learning (offline: done=true)
    // -- W6.2b1x1 ADD: in-arm 1x1_1 place SM (DoInArmPlaceToShuttle_9045_1x1_1)
    //    reads fContact->IsRun2DCheck() (golden cContact.h:627).  Offline: not
    //    running a 2DID re-check -> false.
    bool IsRun2DCheck();                        // golden cContact.h:627 -- offline false
    TfContactShim();
};
extern TfContactShim *fContact;                 // golden cContact.h:667 (PACKAGE TfContact* fContact)

// ===========================================================================
//  ADAM_* EP-pressure DAQ API (golden adam6024.h).  IndexEveryTimeCheckEP /
//  CheckAndRecodrEP drive the EP balloon pressure.  Offline: no DAQ card -> write
//  ops no-op, Alarm() reports "no alarm" (false) so the EP-check SM advances.
// ===========================================================================
void ADAM_DirectWriteData(int iData, int iCh);  // golden adam6024.h -- no-op
void ADAM_WriteVoltage(double dVolt);            // golden adam6024.h -- no-op
bool ADAM_Alarm();                               // golden adam6024.h -- offline false
bool ADAM_Alarm(int iCh);                        // golden adam6024.h -- offline false
void ADAM_Rang(int iRange);                      // golden adam6024.h -- no-op

// ===========================================================================
//  EP switch + heater-OK + NN-mode + in-arm-giveway cross-module helpers.
//  These are golden free functions/enums the tester engine derefs:
//    * EPSwitchOnOff / enum EPSwOn          -- golden adam6024.h:46/47-50 (EP DAQ; offline no-op)
//    * CheckHeaterOK                        -- golden aArmHeader.h:77 / uHeaterThread.h:23 (offline true)
//    * IsNNMode                             -- golden cinitial.h:60 (offline 0 = not NN mode)
//    * CheckInArmFinishAllPickerAction      -- golden ainarm2.h:95 (offline true = finished)
//    * MoveInArm2XYToWait                   -- golden ainarm2.h:146 (offline true = at wait pos)
//    * iCASE_REAL_CCD2                      -- golden cinitial.h RTC-CCD entry case id
// ===========================================================================
enum EPSwOn{ eEPSwOff=0, eEPSwArm1=1, eEPSwArm2=2, eEPSwBoth=3 };  // golden adam6024.h:47-50
void EPSwitchOnOff(int iArm);                    // golden adam6024.h:46 -- offline no-op
bool CheckHeaterOK();                            // golden aArmHeader.h:77 -- offline true (heater ready)
int  IsNNMode();                                 // golden cinitial.h:60 -- offline 0 (not NN)
// In-arm give-way helpers the tester engine calls (IndexAlarmInArmAway, atester.cpp
// :264/:272).  DECLARED here so atester.cpp resolves them; DEFINED ONCE in
// acatchtray_shims.cpp (do NOT define in atester_shims.cpp -> multiple-definition).
bool CheckInArmFinishAllPickerAction();          // golden ainarm2.h:95  -- offline true (finished)
bool MoveInArm2XYToWait();                       // golden ainarm2.h:146 -- offline true (at wait pos)
extern const int iCASE_REAL_CCD2;                // golden cinitial.h (RTC-CCD index-check entry case id)

// ===========================================================================
//  Untranslated VCL forms the tester engine derefs on its ACTIVE entry preamble
//  / oracle-window cases.  Minimal non-VCL stand-ins; offline-safe members only.
//    * CCDInterfaceForm  -- golden CCDInterface.h (CCD identification form)
//    * fAutomation       -- golden Automation/automation.h:151 (host-command form)
//    * fObserver         -- golden cObserver.h (OEE observer)
//    * fiosetview        -- golden iosetview.h (index-suck IO view; bIndexSuck grid)
// ===========================================================================
class TCCDInterfaceFormShim
{
public:
    bool bAtestScanCCDProgram;                   // golden CCDInterface.h:99
    void CCDTimerOnOff(bool bOn);                // golden CCDInterface.h:95 -- offline no-op
    bool CCDRunExec();                           // golden CCDInterface.h:96 -- offline false
    TCCDInterfaceFormShim();
};
extern TCCDInterfaceFormShim *CCDInterfaceForm;  // golden CCDInterface.h

class TfAutomationShim
{
public:
    void DoCommandBuffer(AnsiString Command, AnsiString SubCommand="",
                         AnsiString AlarmMessage="", int ErrType=0, AnsiString AlarmID=""); // golden automation.h:113
    bool GetEventNum(int &num, AnsiString cmdstr, AnsiString numstr);   // golden automation.h:123 -- offline false
    bool GetEventResult(int num);                                       // golden automation.h:125 -- offline false
    int  GetEventStrResult(int num);                                   // golden automation.h:126 -- offline 0
    void ClearEvent(int num);                                          // golden automation.h:128 -- offline no-op
    TfAutomationShim();
};
extern TfAutomationShim *fAutomation;            // golden automation.h:151 (PACKAGE TfAutomation* fAutomation)

class TfObserverShim
{
public:
    bool bTestIndexZ;                            // golden cObserver.h:544 (OEE)
    // -- W6.2b1x1 ADD: in-arm 1x1_1 place/cycle SMs call these OEE recorders
    //    (golden cObserver.h:515/516).  Offline: time bookkeeping no-ops.
    void RecordInArmTime();                      // golden cObserver.h:515
    void AddTimeData(int iRow, double Time);     // golden cObserver.h:516
    TfObserverShim();
};
extern TfObserverShim *fObserver;                // golden cObserver.h

class TfiosetviewShim
{
public:
    bool bIndexSuck[2][4][8];                    // golden iosetview.h:3014
    TfiosetviewShim();
};
extern TfiosetviewShim *fiosetview;              // golden iosetview.h

// ===========================================================================
//  COM2 (golden rs232.h:203 PACKAGE TCOM2* COM2) -- the DTK RTC/CCD serial-comm
//  module (W5 tester-comm).  The tester engine derefs COM2->bCCDDummyRum (DoTestY
//  case 20) + COM2->DoReleaseAndInspEnd() (DoTestHeadMotor case 600000).  Offline:
//  bCCDDummyRum=true (so the RTC/CCD branches short-circuit), DoReleaseAndInspEnd
//  no-op.  HTimer (golden cpublic.h fwd) is a minimal timer stand-in used only by
//  the gated GetTesterResult body (bTestFailNeedWait).
// ===========================================================================
class TCOM2Shim
{
public:
    bool bCCDDummyRum;                           // golden rs232.h:157 -- offline true (no real RTC dummy run)
    void DoReleaseAndInspEnd();                  // golden rs232.h:160 -- offline no-op
    TCOM2Shim();
};
extern TCOM2Shim *COM2;                          // golden rs232.h:203 (PACKAGE TCOM2* COM2)

struct HTimer { bool Off(){ return true; } void SetSecAndOn(double){} };  // golden cpublic.h fwd (W5); gated-use only

#endif // atester_shimsH
