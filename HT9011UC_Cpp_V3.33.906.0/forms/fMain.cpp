// =============================================================================
//  forms/fMain.cpp  --  definitions for the fMain facade
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.cpp by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Every body below is moved
//  VERBATIM -- these no-op / safe-default bodies are the PERMANENT OFFLINE
//  IMPLEMENTATION of the now-virtual method surface (plan SS6-F0-e); see the
//  contract block at the top of forms/fMain.h.
//
//  NOTE ON THIS FILE'S CROSS-LIBRARY DEPENDENCIES: cprod.h / cmydef.h pull in
//  globals that live in ht9045_globals (TestIF, asTCPIPTemperature) while this
//  file lives in the bottom-layer ht9045_forms.  ht9045_forms therefore declares
//  ht9045_globals as a dependency -- which is acyclic (ht9045_globals links no
//  project library except vclcompat; its only other link entries are the
//  psapi/version Win32 import libs).  See the ht9045_forms block in
//  CMakeLists.txt.
// =============================================================================
#include "forms/fMain.h"
// AI(W906-TesterTCPTimer) 20260720: cprod.h (TestIF.iTestType) / cmydef.h
// (asTCPIPTemperature, TCP_IP_MODE, MAX_SOCKET_TOTAL via cprod.h) -- needed by
// TfMain::WritePERSITETemperature below (Interface/TesterTCP_Socket.cpp's
// TimerProcessTCPDataTimer wave). cprod.h itself #includes MachineType.h, which
// is where MAX_SOCKET_TOTAL is #defined -- so this one include also covers the
// ctor's tBarCodeList->Strings[MAX_SOCKET_TOTAL-1] use below.
#include "cprod.h"
#include "cmydef.h"

// --- W6.3 ADD: TfMainHanaART ------------------------------------------------
bool TfMainHanaART::IsHanaArtAvailable() { return false; }     // offline: no HANA link
void TfMainHanaART::AddNewTrayHead(int /*iAuto*/) {}           // offline: no-op

// --- W6.5 ADD: TfMainInplace ------------------------------------------------
TfMainInplace::TfMainInplace()
{
    iNo9ShtErrICCt[0]=0; iNo9ShtErrICCt[1]=0;
    for(int i=0;i<2;i++) for(int j=0;j<8;j++) bNo9ShtErrNo[i][j]=false;
    bNo9Action=false;   // W7-A1: golden-faithful default (floating case 9000 sets false; no offline true-set)
}
bool TfMainInplace::InArmPlacementEnable() { return false; }   // offline: No9 placement disabled

// --- AI(W906-W7-L1-Wave0) 20260801 ADD: TfMainRENESASServer -----------------
// Golden Automation/uRENESAS_Server.h:199 (bLoadingCountFullFlag) / :201
// (DoNeedSupplyOrNot).  Offline there is no RENESAS FT-CT server socket, so
// DoNeedSupplyOrNot reports "no supply demanded" (false) and golden
// asendic_Loader.cpp:2722 / :2756 stay on their non-supply arm; the flag is
// plain storage that :2644 clears.
TfMainRENESASServer::TfMainRENESASServer() : bLoadingCountFullFlag(false) {}
bool TfMainRENESASServer::DoNeedSupplyOrNot(bool /*bNotEnough*/) { return false; }

// --- W6.2: TfMain ----------------------------------------------------------
TfMain::TfMain()
{
    slAutoSiteMapLog = new TfMainSiteMapLog();      // golden main.h:1486 (TMyStringList*)
    // -- W6.3 ADD --
    mtAuto1 = new TfMainTrayPanel();
    mtAuto2 = new TfMainTrayPanel();
    mtAuto3 = new TfMainTrayPanel();
    hanaART = new TfMainHanaART();
    // -- W6.4 ADD --
    lbCCDStatus = new TfMainTrayPanel();           // golden main.h:672 (TLabel* lbCCDStatus)
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
    // AI(W906-W7-F1fix2) 20260729: Pause() observation seams (see forms/fMain.h)
    W906_PauseCallCount    = 0;
    W906_PauseLastFunc     = "";
    // AI(W906-W7-F1) 20260729: W7-F1 ADD -- "Wall 2" facade members
    //    SECSGEM/uHGemHT9045.cpp derefs (plan SS6-F1) -- see forms/fMain.h for
    //    the full per-member citations. Call-count seams start at 0; Sim
    //    seams default to the golden "no hardware blocks it" success/true
    //    value (see each member's own comment for why that default is
    //    golden-faithful).
    W906_cbSetupFileNameChangeCallCount = 0;
    W906_Clarn_DataCallCount            = 0;
    W906_BtnPauseClickCallCount         = 0;  // AI(W906-W7-F1fix) 20260729: seam for the bare-no-op-forward gap
    W906_LoadRunModePictureCallCount    = 0;
    W906_CanChangeSite_Sim              = true;
    W906_BtnTrayEndClickCallCount       = 0;
    W906_UpdateMainOperateModeCallCount = 0;
    W906_LoadStartModePictureCallCount  = 0;
    W906_LookForFileCallCount           = 0;
    W906_ChangeTesterConnect_Sim        = 0;
    W906_SetTemp_Sim                    = 0;
    W906_ChangePasswordCallCount        = 0;
    W906_FTClick_Sim                    = 0;
    W906_RTClick_Sim                    = 0;
    tSiteOnOff[0] = new TStringList();            // golden main.cpp:2229
    tSiteOnOff[1] = new TStringList();            // golden main.cpp:2230
    for (int iW7F1 = 0; iW7F1 < MAX_SOCKET_ROW * MAX_SOCKET_COL; iW7F1++)   // golden main.cpp:2242-2248
    {
        tSiteOnOff[0]->Add("0");
        tSiteOnOff[1]->Add("0");
    }
    edSoakTime = new TfLotInfoEdit();             // golden main.h:733 (TEdit*)
    // AI(W906-W7-L1-Wave0) 20260801: allocations for the 16 W7-L1 members added
    // to forms/fMain.h this pass (the asendic_* tray-SM family's fMain surface).
    // See that header for the per-member golden citations; the StringGrid2 size
    // below is golden's own main.dfm value, not a guess.
    lblLoadTrayCnt   = new TfMainTrayPanel();     // golden main.h:392 (TLabel*)
    lblAuto1TrayCnt  = new TfMainTrayPanel();     // golden main.h:393
    lblAuto2TrayCnt  = new TfMainTrayPanel();     // golden main.h:394
    lblAuto3TrayCnt  = new TfMainTrayPanel();     // golden main.h:395
    lblAuto4TrayCnt  = new TfMainTrayPanel();     // golden main.h:861
    lblAuto5TrayCnt  = new TfMainTrayPanel();     // golden main.h:862
    lblAuto6TrayCnt  = new TfMainTrayPanel();     // golden main.h:863
    edtAuto1         = new TfLotInfoEdit();       // golden main.h:867 (TEdit*)
    edtAuto2         = new TfLotInfoEdit();       // golden main.h:373
    edtAuto3         = new TfLotInfoEdit();       // golden main.h:374
    edtAuto4         = new TfLotInfoEdit();       // golden main.h:376
    edtAuto5         = new TfLotInfoEdit();       // golden main.h:377
    edtAuto6         = new TfLotInfoEdit();       // golden main.h:378
    chkE84IDTray     = new TfMainCheckBox();      // golden main.h:887 (TCheckBox*) -- offline Checked=false
    StringGrid2      = new TStringGrid(8, 60);    // golden main.h:490 (TStringGrid*); ColCount=8 / RowCount=60
                                                  //   verbatim from golden main.dfm:15447 / :15451 -- the
                                                  //   vclcompat default 5x5 would make asendic_Color.cpp:831's
                                                  //   Cells[3][38] throw std::out_of_range
    RENESAS_Server   = new TfMainRENESASServer(); // golden main.h:1710 (TRENESAS_Server*)
    // AI(W906-W7-L2) 20260803: allocations for the two W7-L2 widget members
    // (golden main.h:102/:103 `TBtnPanel *BtnSTEP; TBtnPanel *BtnT_Start;`) that
    // golden ckernel.cpp's WaitManualStepKey/WaitManualStartKey write ->Color on.
    // See forms/fMain.h for the per-member citations and the TfMainPanel
    // type choice.
    //
    // THE INITIAL Color IS GOLDEN'S OWN, NOT A GUESS -- and it is set here
    // explicitly because vclcompat::TPanel defaults Color to 0 (clBlack,
    // vclcompat/Controls.h:245), which is a value neither button ever holds in
    // golden.  Golden's design-time value comes from the form resource, read
    // this pass: main.dfm:10812 `object BtnSTEP: TBtnPanel` carries
    // `Color = 8404992` at :10819, and main.dfm:10834 `object BtnT_Start:
    // TBtnPanel` carries the same `Color = 8404992` at :10841.
    // 8404992 == 0x00804000 -- byte-identical to the literal ckernel.cpp:71 /
    // :113 write, i.e. golden ships both lamps already at their OFF colour.
    // (Both .dfm blocks also set `TrueColor = clYellow` / `FalseColor = 8404992`
    // -- :10829-10830 and :10851-10852 -- so the TBtnPanel's own Down-latch
    // state machine agrees with what ckernel writes by hand.  Independently
    // corroborated by this tree's extracted layout table,
    // tools/dfm2rc/layout_out/main_layout.gen.cpp:603-604, which records
    // Color 8404992 / TrueColor 65535 (clYellow) / FalseColor 8404992 for both.)
    // Golden's TBtnPanel CONSTRUCTOR leaves Color at clBtnFace
    // (elec\myvcl\butPa1.cpp:33, via the shadow-local bug documented in
    // vclcompat/BtnPanelCore.h:101-122), but .dfm streaming overwrites that
    // before the form is ever shown, so 0x00804000 -- not clBtnFace -- is the
    // value a running Handler observes before ckernel first writes it.
    // Setting a non-default in the ctor follows the precedent already set for
    // fLotInfo->palRemoveTray (forms/fLotInfo.cpp restores its true/true).
    //
    // BRANCH SELECTION: this default selects NOTHING.  Both members are
    // write-only in golden (the four ckernel sites are all assignments; nothing
    // in golden or in this tree reads BtnSTEP->Color or BtnT_Start->Color), so
    // no arm of ckernel's logic turns on it -- unlike, say, cInplace's
    // InArmPlacementEnable()==false.  It matters only to the test plan, which
    // asserts on the colour.  CAVEAT for whoever writes those assertions: because
    // golden's initial value and golden's lamp-OFF write are the SAME number
    // (0x00804000), asserting `Color==0x00804000` cannot distinguish "never
    // written" from "written OFF"; only the clYellow (lamp-ON) transition is a
    // sharp assertion.
    BtnSTEP    = new TfMainPanel();               // golden main.h:102 (TBtnPanel*)
    BtnT_Start = new TfMainPanel();               // golden main.h:103 (TBtnPanel*)
    BtnSTEP->Color    = 0x00804000;               // golden main.dfm:10819 (8404992)
    BtnT_Start->Color = 0x00804000;               // golden main.dfm:10841 (8404992)
}
void TfMain::LightOn() {}                                       // W6.4: CCD light sink (offline no-op)
void TfMain::DebugOneCycleHotPlate(AnsiString /*sfunc*/) {}     // debug log sink (offline no-op)
// AI(W906-W7-F1fix2) 20260729: Pause() keeps its golden-faithful offline return
// (false -- offline never pauses) but now records that it RAN and with WHAT Func.
// Without this, every forward INTO Pause() (BtnPauseClick's
// Pause("BtnPauseClick"), golden main.cpp:6967, plus ~40 SM call sites) is
// completely unobservable, so no test can distinguish "forwarded" from
// "silently dropped the call". Zero behavioural change: the return value and the
// absence of any real pause are untouched.
bool TfMain::Pause(AnsiString Func) { W906_PauseCallCount++; W906_PauseLastFunc = Func; return false; }
void TfMain::ShowTestHeadComp(bool /*bRefresh*/) {}
void TfMain::ReStartAutoSiteMapping(bool /*bStart*/) {}
// ---------------------------------------------------------------------------
// AI(W906-AutoSiteMapCleanOut) 20260727: TfMain::SetMainRunStartMode -- GAP-
// DOCUMENTED no-op stub, added solely so csystem.cpp's InitCleanOutFunction
// AutoSiteMap branch (golden csystem.cpp:15751-15785) could be un-gated.
//
// Golden's REAL SetMainRunStartMode (main.cpp:28236-28308, ~72 lines) is NOT
// translated here -- it is out of scope for that small wave:
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
//    Automation/automation.cpp ProcessBuffer (golden :1522).
bool TfMain::Home(AnsiString /*Func*/) { return false; }
// -- W906-AutoCleanFoundation ADD: golden AutoClean.cpp AddAutoCleanMessage
//    sink -- offline no-op log sink, same idiom as AddShuttleMessage/CleanOut.
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
//    forms/fMain.h member comment. Independent future wave (temp/GPIB surface).
AnsiString TfMain::PERSITETemperatureStrings()
{
    return W906_PERSITETemperatureStrings_Sim;
}
// -- W7-F1 ADD: "Wall 2" facade method bodies -- see forms/fMain.h for the
//    full per-member golden citations and offline-default rationale.
void TfMain::cbSetupFileNameChange(void * /*Sender*/) { W906_cbSetupFileNameChangeCallCount++; }
void TfMain::Clarn_Data(int /*Tag*/, AnsiString /*Msg*/) { W906_Clarn_DataCallCount++; }
void TfMain::BtnPauseClick(void * /*Sender*/) { W906_BtnPauseClickCallCount++; Pause("BtnPauseClick"); }   // TRANSLATED:
                                    // golden's own first body line (main.cpp:6967, inside the function at :6965).
                                    // Two independent seams cover this one line: W906_BtnPauseClickCallCount
                                    // (AI(W906-W7-F1fix) 20260729) proves BtnPauseClick RAN;
                                    // W906_PauseCallCount/W906_PauseLastFunc (AI(W906-W7-F1fix2) 20260729) prove the
                                    // FORWARD happened and carried golden's own "BtnPauseClick" argument -- the
                                    // call-count seam alone cannot see the forward at all.
void TfMain::LoadRunModePicture() { W906_LoadRunModePictureCallCount++; }
bool TfMain::CanChangeSite(bool /*bNoIncludeHotplate*/) { return W906_CanChangeSite_Sim; }
void TfMain::BtnTrayEndClick(void * /*Sender*/) { W906_BtnTrayEndClickCallCount++; }
void TfMain::UpdateMainOperateMode() { W906_UpdateMainOperateModeCallCount++; }
void TfMain::LoadStartModePicture() { W906_LoadStartModePictureCallCount++; }
void TfMain::LookForFile() { W906_LookForFileCallCount++; }
int  TfMain::ChangeTesterConnect(int /*Mode*/, bool /*Msg*/, bool /*bRemote*/) { return W906_ChangeTesterConnect_Sim; }
int  TfMain::SetTemp(bool /*bAsk*/, double /*fWorkTemp*/, double /*fSoakTime*/) { return W906_SetTemp_Sim; }
void TfMain::ChangePassword() { W906_ChangePasswordCallCount++; }
int  TfMain::FTClick(bool /*bMan*/) { return W906_FTClick_Sim; }
int  TfMain::RTClick(bool /*bMan*/) { return W906_RTClick_Sim; }
// ---------------------------------------------------------------------------
// AI(W906-W7-L2) 20260803: TfMain::MainFormChange -- golden main.h:1261
// (`void __fastcall MainFormChange();`), body golden main.cpp:3883-4096.
// Offline no-op, same shape as ProcessSensorScan (this file :272) /
// ChangeLevelAttr (:275) above.
//
// WHAT THE REAL BODY DOES, AND THEREFORE WHAT IS ELIDED HERE (all 214 golden
// lines read this pass, not summarised from a recon): it repaints the form's
// site-map LED matrix for the current TestIF_File.iTestMode and nothing else.
// It hides all 16x8 TALed pointers in a local Ptr[][] array (built golden
// main.cpp:3890-3914, cleared :3922-3924) plus the 2x8 9046AU sort-shuttle
// PtrSort[][] array (:3916-3920, cleared :3926-3932); derives SingleRow/iCol
// from iTestMode across a 16-arm else-if ladder (:3934-4002); re-shows the
// matching subset (:4004-4034); applies the IsNNMode()==NN_1Row (:4036-4055)
// and ==NN_2Row (:4057-4066) corrections; and finally sets
// labFailAlarmCnt->Visible / ->Caption from IniConfig.bG04ShowFailAlarmCount
// with Prod.bContsFailBySocket / Prod.bContsFailByHead and their counters
// (:4068-4094).
//
// Every one of those writes targets a TfMain-owned VCL widget that has no
// facade home: the 144 distinct TALed members the two arrays name (16x8 + 2x8;
// e.g. golden main.h:313 `TALed *led_BLCarryKit_0;`, :889
// `TALed *led_SortShtKit_0;`) plus main.h:673 `TLabel *labFailAlarmCnt;`.
// The body writes no global and no Prod/TestIF field, and touches
// no motor/IO -- so offline the elided effect is purely cosmetic: the LED
// matrix and the fail-alarm-count label are not redrawn.  Golden's only caller
// on this front, ckernel.cpp:367, sits in ScanSystemSensor's one-shot
// `if(SoftStart==true)` startup block (:365-533 -- brace-matched this pass with
// comments and string literals masked: `if` at :365, body `{` at :366, closing
// `}` at :533, `else if(SoftStop==true)` at :534; an earlier comment in this
// same wave gave the end as :379, wrong by 154 lines -- :379 is only the
// mid-block `SoftStop=false;`) and ignores any result (the function returns
// void), so nothing downstream of that call site changes.
// ---------------------------------------------------------------------------
void TfMain::MainFormChange() {}                               // W7-L2: offline LED/label repaint no-op
TfMain *fMain = new TfMain();
