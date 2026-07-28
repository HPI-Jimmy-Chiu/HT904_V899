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
// AI(W906-TesterTCPTimer) 20260720: cprod.h (TestIF.iTestType) / cmydef.h
// (asTCPIPTemperature, TCP_IP_MODE, MAX_SOCKET_TOTAL via cprod.h) -- needed by
// TfMain::WritePERSITETemperature below (Interface/TesterTCP_Socket.cpp's new
// TimerProcessTCPDataTimer wave). cprod.h itself #includes MachineType.h, which
// is where MAX_SOCKET_TOTAL is #defined -- so this one include also covers the
// ctor's tBarCodeList->Strings[MAX_SOCKET_TOTAL-1] use below.
#include "cprod.h"
#include "cmydef.h"

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

// --- W906-AutoCleanFoundation ADD: TfNote (first home, see FormsFacade.h) --
TfNote::TfNote() : bMyServoOffInArm(false), iMyServoOffInArmPosX(0), iMyServoOffInArmPosY(0), aJamCodeFilePath("") {}
TfNote *fNote = new TfNote();

// --- W906-AutoCleanFoundation ADD: TfShowMessage (first home, see FormsFacade.h) --
void TfShowMessage::ShowSpeed(bool /*bShow*/) {}
TfShowMessage *fShowMessage = new TfShowMessage();

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
    // -- W5-Automation ADD (AGV_PortScan unit, 20260713) -----------------------
    ALed1         = new TfLedValue();
    labAutomation = new TfMainPanel();
    // -- W906-Automation ADD (20260716) ----------------------------------------
    palMainStatus   = new TfMainPanel();
    cbSetupFileName = new TfLotInfoRunMode();
    edWorkTemperBase = new TfLotInfoEdit();
    // -- W906-TesterTCPTimer ADD (20260720) ------------------------------------
    tTestResult   = new TStringList();
    tBarCodeList  = new TStringList();
    for (int iW906T = 0; iW906T < 32; iW906T++)               // golden main.cpp:2236-2239
    {
        tTestResult->Add("-1");
        tBarCodeList->Add("0");
    }
    tBarCodeList->Strings[MAX_SOCKET_TOTAL-1] = AnsiString("0;");   // golden main.cpp:2241 quirk
                                                                     // (`+=";"` on a freshly-Add()ed
                                                                     // "0" -- equivalent to a direct
                                                                     // assignment here), PRESERVED:
                                                                     // makes the default BARCODE?
                                                                     // reply's last token "0;" (see
                                                                     // TesterTCP_Socket.cpp quirk #13).
    SVID1190_OSSetup = "";
    W906_PERSITETemperatureStrings_Sim = "";
    // -- W906-AutoCleanFoundation ADD (20260721) ------------------------------
    bAutoCleanTest         = false;
    cbIndexDrop            = new TfMainCheckBox();
    chkCleanPadPickErr     = new TfMainCheckBox();
    pnlCleanCount          = new TfMainPanel();
    pnlCleanCountFont      = new TfMainFont();
    AutoCleanContactCountLabel = new TfMainPanel();
    edHPX                  = new TfLotInfoEdit();
    edHPY                  = new TfLotInfoEdit();
    tmyAutoClean           = new TfMainAutoCleanGrid();
    AutoCleanStringGrid    = new TStringGrid();
    // -- W906-AutoCleanCluster ADD (20260722) ----------------------------------
    mtPlate2               = new TfMainAutoCleanGrid();
}
void TfMain::LightOn() {}                                       // W6.4: CCD light sink (offline no-op)
void TfMain::DebugOneCycleHotPlate(AnsiString /*sfunc*/) {}     // debug log sink (offline no-op)
bool TfMain::Pause(AnsiString /*Func*/) { return false; }      // offline never pauses
void TfMain::ShowTestHeadComp(bool /*bRefresh*/) {}
void TfMain::ReStartAutoSiteMapping(bool /*bStart*/) {}
// ---------------------------------------------------------------------------
// AI(W906-AutoSiteMapCleanOut) 20260727: TfMain::SetMainRunStartMode -- GAP-
// DOCUMENTED no-op stub, added solely so csystem.cpp's InitCleanOutFunction
// AutoSiteMap branch (golden csystem.cpp:15751-15785) could be un-gated.
//
// Golden's REAL SetMainRunStartMode (main.cpp:28236-28308, ~72 lines) is NOT
// translated here -- it is out of scope for this small wave:
//   * it dereferences fLotInfo->cbRunMode (Visible/Text.Pos), fBinSel (an
//     entirely new VCL form, ->cbUseMRTMode -- no facade member exists for it),
//     and this TfMain's own edSetOpenBin/lbSetOpenBin/cbRunStartMode/
//     cbbRunModeSel (none of which have a facade home yet);
//   * every branch ends by calling SetRunStartMode() (golden's *different*,
//     already-real function -- NOT called from this stub) and then
//     unconditionally calls UpdateMainOperateMode() (main.cpp:12803-13127,
//     ~325 lines), which walks a real hardware relay/IO ladder (ATC site-use
//     relays, edWorkTemperBase/edSoakTime enable-locks, WriteLastDataFile /
//     ReadLastDataFile, ChangeATCSiteUse) -- none of that surface exists in
//     this ported tree.
// Per this project's established "extend only what's read, stub what's out
// of scope" convention (see ShowTestHeadComp/ReStartAutoSiteMapping just
// above, same class), this is intentionally a documented no-op: the
// InitCleanOutFunction call site only needs the CALL to resolve. Nothing
// currently functioning is lost by this stub for THIS call site specifically,
// because golden's real SetMainRunStartMode always ends by calling
// SetRunStartMode() -- which is ALREADY a separate no-op stub elsewhere in
// this tree (aHotPlateSubstrate.cpp:764) -- so the mode-transition cascade it
// would drive is already inert here regardless. (bSiteMappingCHKOK/
// SiteMapData-zero/bAutoSiteMapHotplateSave are set directly by
// InitCleanOutFunction's own body, independent of this call -- but
// iAutoSiteMapCount is NOT: it is only touched by ReStartAutoSiteMapping,
// golden main.cpp:28166-28180, called from SetMainRunStartMode's
// iSetMode==rsmAutoSiteMap branch only, main.cpp:28283-28306 -- a branch this
// call site never reaches, since iSetMode here is always rsmContinuStart or
// rsmContinuRetest, csystem.cpp:274/276.)
// ---------------------------------------------------------------------------
void TfMain::SetMainRunStartMode(int /*iSetMode*/) {}
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
// -- W906-Automation ADD: golden main.h:1276 `bool __fastcall Home(AnsiString Func);`
//    (kevin 20141108) -- runs a full motor Home cycle and reports success/
//    failure. Offline: no real motors to home, so there is nothing to
//    actually succeed -> false (same "offline never succeeds a hardware
//    cycle" posture as Pause() above). Only current caller in the
//    translated tree is auto9045.cpp's `#ifdef DEBUG_DUTONOFF` DoHomeAndStart
//    (compiled out, DEBUG_DUTONOFF undefined) plus the still-GATED
//    Automation/automation.cpp ProcessBuffer (golden :1522) -- added now per
//    that front's task brief as a small additive cross-file gap ahead of
//    ProcessBuffer's own future translation.
bool TfMain::Home(AnsiString /*Func*/) { return false; }
// -- W906-AutoCleanFoundation ADD: golden AutoClean.cpp AddAutoCleanMessage
//    sink. Next-wave real consumer (the 4 core engines); offline no-op log
//    sink, same idiom as AddShuttleMessage/CleanOut above.
void TfMain::AddAutoCleanMessage(AnsiString /*S*/) {}
// -- W906-TesterTCPTimer ADD: golden TfMain::WritePERSITETemperature,
//    Command.cpp:935-943 (void __fastcall) -- WRAPPER, translated faithfully.
void TfMain::WritePERSITETemperature()
{
    AnsiString sRet="";
    sRet=PERSITETemperatureStrings();
    if(TestIF.iTestType==TCP_IP_MODE)                // golden Command.cpp:939 (wei 20211027 open short TCP/IP)
        asTCPIPTemperature.sprintf("%s\r", sRet);    // golden :940 (Sam 20231205) -- NOTE: unconditional "%s\r"
                                                     //   => even empty sRet yields "\r" != "" => the timer's
                                                     //   TempArm? branch ALWAYS replies (min "TempArm:")
    // AI(W906-TesterTCPTimer) 20260720: golden else-branch (Command.cpp:942)
    //   SendMSG_CMD(MSG_CMD_TempArm, sRet+"\r") -- MSG_CMD_TempArm is an extern
    //   const in untranslated MessageDef.h/.cpp and facade SendMSG_CMD is an
    //   offline no-op anyway; branch gated with this note (no #if 0 needed:
    //   nothing to compile). Un-gate together with MessageDef wave.
}
// -- W906-TesterTCPTimer ADD: golden TfMain::PERSITETemperatureStrings body,
//    Command.cpp:945-1482 (+RefreshTempData main.h:1388) -- GATED LEAF, see
//    FormsFacade.h member comment. Independent future wave (temp/GPIB surface).
AnsiString TfMain::PERSITETemperatureStrings()
{
    return W906_PERSITETemperatureStrings_Sim;
}
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
    // -- W5-Automation ADD (AGV_PortScan unit, 20260713) -----------------------
    ALedLoader    = new TfLedValue();
    for(int i=0;i<3;i++) aLedAuto[i] = new TfLedValue();
    palRemoveTray = new TfLotInfoPanel();
    // -- W906-AutoCleanFoundation ADD (20260721) ------------------------------
    for(int iW906AC=0; iW906AC<3; iW906AC++) iUnloaderTask[iW906AC] = 0;
    // -- AI(W906-Save2DSortingSummary) 20260723 ADD: 6 new TfLotInfoEdit members --
    edtCusLotID       = new TfLotInfoEdit();
    edtCusDevGrp      = new TfLotInfoEdit();
    edtCusStep        = new TfLotInfoEdit();
    edtDevice         = new TfLotInfoEdit();
    edtSysOperatorID  = new TfLotInfoEdit();
    mmo2DLotInfo      = new TfLotInfoEdit();
    // -- AI(W906-SaveTestSummaryTSV) 20260728 ADD: 3 new TfLotInfoEdit members + 1 more
    //    (lbledtCustomer, AI(W906-SaveSummaryTrayFeed) 20260728) --
    edtASECL_LotID    = new TfLotInfoEdit();
    edInsertion       = new TfLotInfoEdit();
    edFlowID          = new TfLotInfoEdit();
    lbledtCustomer    = new TfLotInfoEdit();
}
// AI(W906-AutoCleanFoundation) 20260721: golden uLotInfo.cpp:16250-16253 --
// REAL one-line body (was a total no-op stub before this wave). See
// FormsFacade.h's iUnloaderTask/InitialUnLoaderTask member comments for the
// behaviour-change + dormant-call-site (SOFT_SIMULTE undefined) analysis.
void TfLotInfo::InitialUnLoaderTask(int iPos) { iUnloaderTask[iPos]=1; }
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

// --- W906-AutoCleanFoundation ADD: TfCleaning -------------------------------
TfCleaning::TfCleaning() : iDeviceCount(0), bResetCleanCount(false), b1x2SiteAbClosePutDummy(false)
{
    edCleaningCount = new TfLotInfoEdit();
}
TfCleaning *fCleaning = new TfCleaning();
