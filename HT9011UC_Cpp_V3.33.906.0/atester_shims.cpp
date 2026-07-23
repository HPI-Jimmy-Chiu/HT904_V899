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
// AI(W64b-Integrate) 20260706: TestIF_File / site-mode enums for the GetSiteCount
// shim below (golden home cprod.cpp, whole function-body region gated -- see there).
#include "cprod.h"
// AI(W5-Automation-Integrate) 20260710: TMyKitSuck (CheckTestSuckICOn stand-in below).
#include "aHotPlateSubstrate.h"
// AI(W906-cContactLeaf) 20260721: ComputeIsRun2DCheck (real TfContact::IsRun2DCheck body,
// translated as a free function) for the TfContactShim::IsRun2DCheck() wire-up below.
#include "cContact.h"

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

// AI(W5-Automation-Integrate) 20260710: 3 more csystem.h-declared bodies atester_32Site.cpp
// needs (golden home csystem.cpp, whole regions not translated this wave -- HeadContactCount/
// temperature-log/index-time-log subsystems).  Same "these are the csystem.h-declared bodies;
// no other translated TU defines them" contract as the 4 above.
//   CheckContactOver -> false: golden guards its whole body on CosFunction.bUseHeadContactCount
//     (default false offline), so false matches the feature-disabled golden outcome exactly.
//   ShowIndexTime -> no-op: pure debug index-timing log, no control-flow effect on callers.
//   TemperatureStorageLog -> false: golden's own first statement is `if(InitialOK==false) return
//     false;` (offline InitialOK is false), matching the not-yet-initialized golden outcome.
bool CheckContactOver()                         { return false; }  // golden csystem.h:156
void ShowIndexTime(int /*Item*/)                {}                 // golden csystem.h:200
bool TemperatureStorageLog(int /*iRecord*/)     { return false; }  // golden csystem.h:275

// AI(W5-Automation-Integrate) 20260710: atester_32Site.cpp's own local `extern` globals/
// predicate (golden main.cpp-owned bEcho/bExist/bUnderTest/bEchoStop -- same untranslated-main.cpp
// gap already documented in Interface/InterfaceSYS.cpp's file header; CheckTestSuckICOn is a
// golden atester.cpp-family leaf with no translated home yet).  Offline-safe defaults: the bools
// start false (no echo/exist/under-test/stop-request pending, matching a freshly-started handler);
// CheckTestSuckICOn->false (no per-site suck-IC-on-during-drop alarm) -- and its only call site
// (atester_32Site.cpp :1063/:1069) is itself guarded by `if(LastSet.iRealDummy==REALLY)` (false
// in the Sim canary), so this is inert offline either way.
bool bEcho=false, bExist=false, bUnderTest=false;                  // golden main.cpp (untranslated)
bool bEchoStop=false;                                              // golden main.cpp (untranslated) //ChungHung 20130326 add
bool CheckTestSuckICOn(TMyKitSuck &/*Ptr*/, int /*iR*/, int /*iC*/) { return false; }

// AI(W906-TesterTCPTimer) 20260720: iBin[4][8] production definition -- golden
// main.cpp:15156 (untranslated, file-scope, zero-initialized). Same
// untranslated-main.cpp-owned-global family as bEcho/bExist/bUnderTest just
// above (atester.cpp:652 and Interface/InterfaceSYS.cpp:77 both already
// `extern unsigned int iBin[4][8];` this symbol). Until now the ONLY definition
// lived test-locally in tests/test_interfacesys.cpp -- moved here (the real
// ht9045_sm production home) because Interface/TesterTCP_Socket.cpp's new
// TimerProcessTCPDataTimer/SimulateBin (this wave) write iBin directly, so
// test_testertcp_socket now also needs a link-time definition, not just
// test_interfacesys. tests/test_interfacesys.cpp's own local definition was
// changed to `extern` in the same step to avoid a duplicate-symbol link error
// (see that file's own comment).
unsigned int iBin[4][8];

// AI(W64b-Integrate) 20260706: GetSiteCount (golden cprod.cpp:305, declared
// cprod.h:3299) -- discovered as an undefined-reference link error while
// integrating aTester_Front.cpp/aTester_Rear.cpp (case 650/750:
// `CosFunction.bD44Use4ea && GetSiteCount()>4`).  The real golden body is a
// pure TestIF_File.iTestMode/iSiteMap[][] computation with NO untranslated
// substrate dependency, BUT it textually lives inside cprod.cpp's whole-file
// `#if 0 // TODO(W6): function bodies depend on untranslated state machines +
// globals` region (cprod.cpp:184-4036), so it never actually compiles into
// libht9045_globals.a today.  Faithful duplicate here (same pattern already
// used a few lines up for IsInArmCleanOutFinish/IsInArmOneCycleFinish, which
// are ALSO real csystem.cpp bodies shimmed here for the identical reason).
// TODO(W6): delete this duplicate once cprod.cpp's #if 0 gate lifts (would
// otherwise ODR-conflict with the real definition).
int GetSiteCount(bool IncludeCloseSite)                             // golden cprod.cpp:305
{
    int iSiteCount=0;

    if(IncludeCloseSite==true)
    {
        if(TestIF_File.iTestMode==SingleSite)
            iSiteCount=1;
        else if(TestIF_File.iTestMode==DualSite)
            iSiteCount=2;
        else if(TestIF_File.iTestMode==TriSite1X3)
            iSiteCount=3;
        else if(TestIF_File.iTestMode==QualSite1X4 ||
                TestIF_File.iTestMode==_8Site1X4)
            iSiteCount=4;
        else if(TestIF_File.iTestMode==QualSite2X2 ||
                TestIF_File.iTestMode==QualSite2X2N)
            iSiteCount=4;
        else if(TestIF_File.iTestMode==DualSite2x1)
            iSiteCount=2;
        else if(TestIF_File.iTestMode==_6Site2X3 ||
                TestIF_File.iTestMode==_6Site2X3N)
            iSiteCount=6;
        else if(TestIF_File.iTestMode==_8Site2X4 ||
                TestIF_File.iTestMode==_8Site2X4N)
            iSiteCount=8;
        else if(TestIF_File.iTestMode==_10Site2X5)
            iSiteCount=10;
        else if(TestIF_File.iTestMode==_12Site2X6)
            iSiteCount=12;
        else if(TestIF_File.iTestMode==_16Site2X8 ||
                TestIF_File.iTestMode==_16Site4X4)
            iSiteCount=16;
        else if(TestIF_File.iTestMode==_32Site4X8M ||
                TestIF_File.iTestMode==_32Site4X8N)
            iSiteCount=32;
    }
    else
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                if(TestIF_File.iSiteMap[i][j]>0)
                    iSiteCount++;
            }
        }
    }

    return iSiteCount;
}

// ---- atester_32Site: AI(W5-Automation-Integrate) 20260710: ALL bodies that
// used to live here are now REAL (atester_32Site.cpp) -- REMOVED (would
// otherwise duplicate-define).

// ---- atester_ProcessCount: AI(W5-Automation-Integrate) 20260710: 11 bodies
// that used to live here are now REAL (atester_ProcessCount.cpp) -- REMOVED
// (would otherwise duplicate-define).  The 5 still-out-of-scope exports keep
// their offline no-op bodies here exactly as before.
void ProcessShowTestStatus(int /*Index*/)   {}
void ProcessCount(int /*Index*/, bool /*bHasIC*/) {}    //Eastsun 20260515 F022: D7
void ProcessStartTestData(int /*Direct*/)   {}
void RecordHistroy(int /*attr*/)            {}
void ProcessQASampling(int /*Index*/)       {}

// ---- aTester_Front cursors + bodies ----------------------------------------
int iFTestSuckTestICTask                  = 1;
// AI(W64b-Integrate) 20260706: iFrontTestDestroyICTask now defined for real in
// aTester_Front.cpp (golden aTester_Front.cpp:300) -- stub definition removed.
// AI(W5-Automation-Integrate) 20260710: iFrontTestSuckICTask now defined for
// real in aTester_Front.cpp too -- stub definition removed.
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
// AI(W64b-Integrate) 20260706: InitFrontTestDestroyICTask/TestZ1SetPos/
// DoFrontTestDestroyIC/CheckZ1IsDown/TestZ1OutRandge now defined for real in
// aTester_Front.cpp (golden aTester_Front.cpp:170-298,309-865) -- stub bodies removed.
// AI(W5-Automation-Integrate) 20260710: InitFrontTestSuckICTask/DoFrontTestSuckIC
// now defined for real in aTester_Front.cpp too -- stub bodies removed.
void InitFrontTestPurgBeforePickShuttle()   {}
void DoArm1Suck()                           {}
void DoArm1D44VacCheck()                    {}
bool FTestNeedDestroy()                     { return false; }
bool DoFTestSuckTestIC()                    { return true;  }
bool DoFrontTestPurgBeforePickShuttle(int /*isp*/) { return true; }
bool DoTestYFront()                         { return true;  }
bool TestZ1OutRandge2()                     { return false; }
bool FTestSeparateSLK(bool /*bReset*/)      { return true;  }
bool FTestCombineSLK(bool /*bReset*/)       { return true;  }
bool DoFRTCAutoModelVerify(bool /*bInitial*/){ return true; }

// ---- aTester_Rear cursors + bodies -----------------------------------------
int iBTestSuckTestICTask                  = 1;
// AI(W64b-Integrate) 20260706: iRearTestDestroyICTask now defined for real in
// aTester_Rear.cpp (golden aTester_Rear.cpp:300) -- stub definition removed.
// AI(W5-Automation-Integrate) 20260710: iRearTestSuckICTask now defined for
// real in aTester_Rear.cpp too -- stub definition removed.
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
// AI(W64b-Integrate) 20260706: InitRearTestDestroyICTask/TestZ2SetPos/
// DoRearTestDestroyIC/CheckZ2IsDown/TestZ2OutRandge now defined for real in
// aTester_Rear.cpp (golden aTester_Rear.cpp:170-298,309-886) -- stub bodies removed.
// AI(W5-Automation-Integrate) 20260710: InitRearTestSuckICTask/DoRearTestSuckIC
// now defined for real in aTester_Rear.cpp too -- stub bodies removed.
void InitRearTestPurgBeforePickShuttle()    {}
void DoArm2Suck()                           {}
void DoArm2D44VacCheck()                    {}
bool BTestNeedDestroy()                     { return false; }
bool DoBTestSuckTestIC()                    { return true;  }
bool DoRearTestPurgBeforePickShuttle(int /*isp*/) { return true; }
bool DoTestYRear()                          { return true;  }
bool TestZ2OutRandge2()                     { return false; }
bool BTestCombineSLK(bool /*bReset*/)       { return true;  }
bool BTestSeparateSLK(bool /*bReset*/)      { return true;  }
bool DoBRTCAutoModelVerify(bool /*bInitial*/){ return true; }

// ---- fContact (offline contact-mode form) ----------------------------------
TfContactShim::TfContactShim() : fShow(false) {}
bool TfContactShim::Do_ROILearning() { return true; }   // offline: ROI learning "done"
// AI(W906-cContactLeaf) 20260721: was a hardcoded `return false;` stub (W6.2b1x1 note, now
// superseded).  Swapped in the real golden predicate (TfContact::IsRun2DCheck, cContact.cpp:
// 21203-21219, translated as ComputeIsRun2DCheck in cContact.h/.cpp) over the real globals it
// reads.  Checked this wave: bRun2DCheck (cmydef.h:5830) has no ASSIGNMENT anywhere in the
// translated tree yet -- only its own zero-init definition (cmydef.cpp:5811, un-gated this same
// wave; see the AI(W906-cContactLeaf) comment there) -- so this still evaluates to `false` under
// every reachable state today, identical to the old stub; see cContact.h's ComputeIsRun2DCheck
// banner for the full call-site risk check.
bool TfContactShim::IsRun2DCheck()
{
    return ComputeIsRun2DCheck(IniConfig.bF33_Check2DHardware,
                                BAR_CODE_INSTALL,
                                TestIF_File.bEnableBarCode,
                                InArmSuck.iShtRow,
                                iContactMode,
                                bRun2DCheck);
}
void TfContactShim::InitDoFullViewCheck() {}                   // offline no-op
bool TfContactShim::DoFullViewCheck() { return true; }         // offline: reached (true) -- see .h banner
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
// -- W5-Final-Auto9045 INTEGRATE ADD: TestMode/sATKSendMessage/sATKSendMessageCount --
TfAutomationShim::TfAutomationShim() : TestMode(0), sATKSendMessageCount(0)
{
    for(int i=0;i<256;i++) sATKSendMessage[i]=0;
}
void TfAutomationShim::DoCommandBuffer(AnsiString /*Command*/, AnsiString /*SubCommand*/,
                                       AnsiString /*AlarmMessage*/, int /*ErrType*/, AnsiString /*AlarmID*/) {}
bool TfAutomationShim::GetEventNum(int & /*num*/, AnsiString /*cmdstr*/, AnsiString /*numstr*/) { return false; }
bool TfAutomationShim::GetEventResult(int /*num*/)     { return false; }
int  TfAutomationShim::GetEventStrResult(int /*num*/)  { return 0; }
void TfAutomationShim::ClearEvent(int /*num*/)         {}
TfAutomationShim *fAutomation = new TfAutomationShim();

// ---- fObserver (offline OEE observer) --------------------------------------
TfObserverShim::TfObserverShim() : bTestIndexZ(false)
{
    // -- W5-Final-Auto9045 INTEGRATE ADD: Memo1Lines + 7 label stand-ins --
    Memo1Lines      = new TfObserverMemoLines0();
    labModel        = new TfObserverLabel();
    labPowerOnTime  = new TfObserverLabel();
    labRunningTime  = new TfObserverLabel();
    labProductTime  = new TfObserverLabel();
    labLoadingCount = new TfObserverLabel();
    labMUBA         = new TfObserverLabel();
    labMTBA         = new TfObserverLabel();
    // -- AI(W906-SaveTestSummarySECS) 20260721: new memoLotSummary member, see atester_shims.h.
    memoLotSummary  = new TfObserverMemoLotSummary();
}
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
