// =============================================================================
//  FormsFacade.cpp  --  definitions for the non-VCL form-pointer facade
//
//  Translation wave: W6.0 (scaffold) ; EXTENDED W6.2 (in-arm HP geometry)
//  Translator: AI(W6.0-SCAFFOLD) 20260626 ; AI(W6.2-INARM) 20260626
//
//  W6.1: fAGV->IsATK_AMR() (false offline).
//  W6.2: fMain / fSortCT / fLotInfo / fOffSet / fSCKART -- all offline no-op /
//        false, constructed once at load.  See FormsFacade.h for the member-by-
//        member documentation + golden homes.
// =============================================================================
#include "FormsFacade.h"
#include "Automation/AGV_predicates.h"      // AI(W5-Automation-Integrate) 20260710: real AMR-mode predicates

// --- W6.1 ------------------------------------------------------------------
// -- W5-Final-AGV_E84 INTEGRATE ADD: explicit ctor (was implicit) so mmE84Log
//    is constructed -- see FormsFacade.h TfAGV::mmE84Log.
TfAGV::TfAGV() { mmE84Log = new TfMainMemo(); }
TfAGV *fAGV = new TfAGV();

// AI(W5-Automation-Integrate) 20260710: wired to the real, faithfully-translated
// predicates (Automation/AGV_predicates.cpp) instead of the previous hardcoded
// false stand-ins -- see that unit's translate report.  Behaviourally identical
// offline TODAY (USE_COVER_TRAYID defaults tCIDNotUse / IniConfig.bA65_BundleIDList
// defaults false, so every predicate still evaluates false on the untouched
// baseline), but now evaluates the REAL condition once those globals are set.
bool TfAGV::IsSPIL_AMR() { return AGV_IsSPIL_AMR(); }
bool TfAGV::IsATK_AMR()  { return AGV_IsATK_AMR();  }
// --- W6.3 ADD --------------------------------------------------------------
bool TfAGV::Use_AMR()    { return AGV_Use_AMR();    }

// --- W6.2: TfMain ----------------------------------------------------------
// --- W6.3 ADD: TfMainHanaART --------------------------------------------------
bool TfMainHanaART::IsHanaArtAvailable() { return false; }     // offline: no HANA link
void TfMainHanaART::AddNewTrayHead(int /*iAuto*/) {}           // offline: no-op
// --- W6.5 ADD: TfMainInplace ----------------------------------------------
TfMainInplace::TfMainInplace()
{
    iNo9ShtErrICCt[0]=0; iNo9ShtErrICCt[1]=0;
    for(int i=0;i<2;i++) for(int j=0;j<8;j++) bNo9ShtErrNo[i][j]=false;
    bNo9Action=false;   // W7-A1: golden-faithful default (floating case 9000 sets false; no offline true-set)
}
bool TfMainInplace::InArmPlacementEnable() { return false; }   // offline: No9 placement disabled
TfMain::TfMain()
{
    slAutoSiteMapLog = new TfMainSiteMapLog();      // golden main.h:1486 (TMyStringList*)
    // -- W6.3 ADD --
    mtAuto1 = new TfMainTrayPanel();
    mtAuto2 = new TfMainTrayPanel();
    mtAuto3 = new TfMainTrayPanel();
    hanaART = new TfMainHanaART();
    // -- W6.4 ADD --
    lbCCDStatus = new TfMainTrayPanel();           // golden main.h (TLabel* lbCCDStatus)
    // -- W6.5 ADD: shuttle-engine sub-objects --
    cbShowShuttleSensor    = new TfMainCheckBox();
    cbTestOutShuttleSensor = new TfMainCheckBox();
    cbShowInShuttleSensor  = new TfMainCheckBox();
    htShullte0 = new TfMainGrid();
    htShullte1 = new TfMainGrid();
    meShuttle1 = new TfMainMemo();
    meShuttle2 = new TfMainMemo();
    cInplace   = new TfMainInplace();
    pgMain     = new TfMainPageControl();          // ActivePageIndex==0 offline
    emp7TabSheet21 = 0;                            // ==pgMain->ActivePageIndex offline
    // -- W7-C1 ADD --
    BtnOneCycle = new TfMainSpeedButton();         // offline Down=false (else-branch one-cycle trigger inert)
    // -- W5-comms INTEGRATE ADD: Interface/InterfaceSYS.cpp IPC window handles --
    HESDWnd        = NULL;
    HEventLogWnd   = NULL;
    HAutoUpdateWnd = NULL;
    oldGpibAddress = 0;
}
void TfMain::LightOn() {}                                       // W6.4: CCD light sink (offline no-op)
void TfMain::DebugOneCycleHotPlate(AnsiString /*sfunc*/) {}     // debug log sink (offline no-op)
bool TfMain::Pause(AnsiString /*Func*/) { return false; }      // offline never pauses
void TfMain::ShowTestHeadComp(bool /*bRefresh*/) {}
void TfMain::ReStartAutoSiteMapping(bool /*bStart*/) {}
void TfMain::CleanOut(AnsiString /*Func*/) {}                  // W6.3: offline clean-out no-op
void TfMain::DoStateRecord(int /*i*/, bool /*b*/) {}           // W6.3: offline state-record sink
// -- W6.5 ADD: shuttle-engine method sinks (all offline no-op) --
void TfMain::AddShuttleMessage(int /*iSht*/, AnsiString /*S*/) {}
void TfMain::Reset(AnsiString /*Func*/) {}
void TfMain::BtnOneCycleClick(void * /*Sender*/) {}
void TfMain::BtnResetClick(void * /*Sender*/) {}
void TfMain::BtnCleanOutClick(void * /*Sender*/) {}   // W6.2b(2x4_16): offline clean-out no-op
void TfMain::JSCC_ResetForShuttleLoseIC() {}
void TfMain::ResetRecordforPiggyBack(AnsiString /*S*/) {}
void TfMain::ProcessSensorScan() {}                            // W6.6: HUB main-loop per-tick sensor scan (offline no-op)
// -- W7-C1 ADD: end-of-lot clean-out finish-check fMain methods (all offline no-op) --
void TfMain::Start(AnsiString /*Func*/) {}                     // W7-C1: offline do NOT auto re-start
void TfMain::ChangeLevelAttr() {}                              // W7-C1: offline level-attr UI no-op
void TfMain::ModifyTester(int /*iWhich*/) {}                   // W7-C1: offline QA tester-modify no-op
void TfMain::CleanYieldCount() {}                              // W7-C1: offline yield-count clear no-op
// -- W5-Automation ADD: HANA_ART.cpp method sinks (all offline no-op / empty) --
void TfMain::SendMSG_CMD(int /*CMD*/) {}                                    // offline: no real GPIB-bridge process
void TfMain::SendMSG_CMD(int /*CMD*/, AnsiString /*Message*/) {}            // offline: no real GPIB-bridge process
AnsiString TfMain::GetSamSungMap(bool /*bSend*/) { return AnsiString(""); } // offline: no SamSung map source
AnsiString TfMain::GetSamSungSoakTime(bool /*bSend*/) { return AnsiString("0"); } // offline: no soak-time source
AnsiString TfMain::ArmStatusStrings() { return AnsiString(""); }            // offline: no arm-status telemetry
// -- W5-Final-SckArtRemainder ADD: method sinks (all offline no-op) --
void TfMain::SetStartModeData() {}                                          // offline: recipe start-mode UI refresh no-op
void TfMain::LoadTestModePicture() {}                                       // offline: test-mode picture UI refresh no-op
void TfMain::BackupSetupFile() {}                                           // offline: setup-file backup no-op
TfMain *fMain = new TfMain();

// --- W6.2: TfSortCT --------------------------------------------------------
TfSortCT::TfSortCT()
{
    pnlHP1 = new TfSortCTPanel();
    pnlHP2 = new TfSortCTPanel();
    // -- W6.3 ADD --
    pnlLoad       = new TfSortCTPanel();
    pnlLoadCID    = new TfSortCTPanel();
    pnlCoverTrayD = new TfSortCTPanel();
    for(int i=0;i<6;i++) pnlTrayCnt[i] = new TfSortCTPanel();
}
TfSortCT *fSortCT = new TfSortCT();

// --- W6.2: TfLotInfo -------------------------------------------------------
TfLotInfo::TfLotInfo()
{
    cbRunMode = new TfLotInfoRunMode();             // offline: Visible=false
    // -- W6.3 ADD --
    labNowLoaderTrayID = new TfLotInfoLabel();
    edtSysLotID        = new TfLotInfoEdit();
    // -- W5-Automation ADD --
    cbProcess          = new TfLotInfoRunMode();
    // -- W5-Final-TesterTCPSocket ADD --
    labTCPIPStatus = new TfLotInfoStatusLabel();
    mmTesterLog    = new TfMainMemo();
}
void TfLotInfo::InitialUnLoaderTask(int /*iUnloader*/) {}      // W6.3: offline AMR-task no-op
// -- W5-Automation ADD: AMR.cpp + HANA_ART.cpp method sinks (all offline no-op) --
void TfLotInfo::RefreshAMR() {}                                            // offline: no UI to refresh
void TfLotInfo::SetLotID(AnsiString /*ID*/, bool /*bReadFromFile*/) {}     // offline no-op
void TfLotInfo::SetLotStart(AnsiString /*sFunc*/, bool /*bReadFromFile*/) {} // offline no-op
TfLotInfo *fLotInfo = new TfLotInfo();

// --- W6.2: TfOffSet --------------------------------------------------------
bool TfOffSet::UseAutoOffsetFunction(AnsiString /*sName*/) { return false; }  // no auto-offset offline
bool TfOffSet::UseInArmSetupTeach(int /*iWhich*/)          { return false; }  // no setup-teach offline
TfOffSet *fOffSet = new TfOffSet();

// --- W6.2: TfSCKART --------------------------------------------------------
TfSCKART::TfSCKART() : iInputJamCnt(0), iFTRTCount(0), iInputCount(0), iCurrent93KARTStep(0)
{
    // -- W5-Automation ADD --
    palLotNumber   = new TfSortCTPanel();
    palTestCnt     = new TfSortCTPanel();
    palRTTryCnt    = new TfSortCTPanel();
    pnlProcessCode = new TfSortCTPanel();
    edlRTTryCnt    = new TfLotInfoEdit();
}
int  TfSCKART::CheckLoadingCount() { return 0; }              // W7: offline -> 0 (no ART loading mismatch)
void TfSCKART::AddOutputJamCnt(int /*row*/, int /*col*/, int /*ret*/, int /*iBinOnCarryKit*/) {}  // W6.5: offline no-op
// -- W5-Automation ADD: AMR.cpp + HANA_ART.cpp method sinks (all offline no-op) --
void TfSCKART::DoARTLotStart(AnsiString /*_sLotID*/, AnsiString /*_sProcessCode*/, int /*_iLotCount*/) {}
void TfSCKART::AccessFile(bool /*bRead*/, int /*iAccess*/) {}
TfSCKART *fSCKART = new TfSCKART();
