// =============================================================================
//  forms/fLotInfo.cpp  --  definitions for the fLotInfo facade
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.cpp by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Bodies moved VERBATIM, with
//  exactly ONE addition, marked below: palRemoveTray's Enabled/Visible are now
//  set explicitly in the ctor, because the bespoke TfLotInfoPanel type they
//  used to carry defaulted them to true/true while the unified
//  vclcompat::TPanel defaults every control to false (see
//  vclcompat/Controls.h's DEFAULT-VALUE RULE).  Setting them here keeps the
//  CONSTRUCTED STATE byte-for-byte identical to before the refactor, which is
//  what W7-F0's zero-behaviour-change contract requires.
// =============================================================================
#include "forms/fLotInfo.h"

// AI(W906-FW3-LotInfo-WA) 20260819: Wave A includes -- every global these 39
// Tier-1 methods dereference lives in one of these; see forms/fLotInfo.h's
// banner for the per-dependency existence citations gathered this wave.
#include "cmydef.h"            // AccessLevel/iDef*Level/CUSTOMER_CODE/CC_*/ATC_SYSTEM/Tri_Temp_Machine/
                                //   AirStream_Select/Total_Compressor/iATC_Use_Heat_Count/SiteData[]/
                                //   USE_RFID_READER/N_NO_SYMBOL/N_NO_SPACE/SystemYear../bLoaderActionFlag[]/
                                //   bUnLoaderActionFlag[]/bAMRReceive*/iLoaderTrayCountCal/iUnloaderTrayCountCal[]/
                                //   sB03RunData/sB03StartTime/bTesterSendPause/bTesterPauseMusic
#include "cprod.h"              // TestIF_File/BinSelect[]/Prod/RunInfo/SystemStart
#include "Config.h"             // IniConfig
#include "CosFunction.h"        // CosFunction
#include "LastSet.h"            // LastSet.iTester/OFF_LINE
#include "common.h"             // ReadIniData/GetLastOpenFN/AuthPath
#include "canary_support.h"    // ShowMyMessage
#include "cpublic.h"            // GetTimeInfo()
#include "vclcompat/IniFiles.h" // TIniFile (TransformTemperature_AirStream)
#include "vclcompat/FileListBox.h" // TFileListBox (bCheckOnlyOneFile)
#include "forms/fMain.h"        // fMain->edWorkTemperBase (TransformTemperature_AirStream)

using vclcompat::TFileListBox;

// ---------------------------------------------------------------------------
// IncludeTrailingPathDelimiter -- BCB6 synonym for IncludeTrailingBackslash.
// Every other TU that needs this (cprod.cpp/cpublic.cpp/handlerlog.cpp/
// Interface/TesterTCP.cpp/SECSGEM/uHGemClass.cpp/uHGemEquipment.cpp/
// Automation/SCK_ART_Remainder.cpp) carries an identical TU-local copy rather
// than a shared one -- centralizing it was not any of those tasks' call to
// make, so this file gets its own copy too (N23UseLotInfoFile, golden :7351).
// ---------------------------------------------------------------------------
static inline AnsiString IncludeTrailingPathDelimiter(const AnsiString& p)
{
    return IncludeTrailingBackslash(p);
}

// ---------------------------------------------------------------------------
// AI(W906-FW3-LotInfo-WA) 20260819: golden FILE-SCOPE globals (uLotInfo.cpp
// top-of-file, NOT TfLotInfo members -- confirmed by reading golden uLotInfo.h
// for each: none of these 4 appear there). Declared here, at file scope, in
// the one file that currently needs them, exactly mirroring golden's own
// scope. Initial values copied verbatim from golden (uLotInfo.cpp:89-91/95,
// :16256-16257).
// ---------------------------------------------------------------------------
static TLabel *SocketLabRow_Display[MAX_SOCKET_ROW] = { new TLabel(), new TLabel(), new TLabel(), new TLabel() };
static TLabel *SocketLabCol_Display[MAX_SOCKET_COL] = { new TLabel(), new TLabel(), new TLabel(), new TLabel(),
                                                         new TLabel(), new TLabel(), new TLabel(), new TLabel() };
bool bLotID_OK=false;                 // golden uLotInfo.cpp:91
bool bLotFirstKeyIn=false;            // golden uLotInfo.cpp:95
int iLoaderTask[3]={1,1,1};           // golden uLotInfo.cpp:16256
int iloaderLevelTask[3]={1,1,1};      // golden uLotInfo.cpp:16257

// --- W6.2: TfLotInfo -------------------------------------------------------
TfLotInfo::TfLotInfo()
{
    cbRunMode = new TfLotInfoRunMode();             // offline: Visible=false
    // -- W6.3 ADD --
    labNowLoaderTrayID = new TfLotInfoLabel();
    edtSysLotID        = new TfLotInfoEdit();
    // AI(W906-FW-Y3) 20260819: the 5 AutoClean-display widgets (see header)
    Label17                   = new TfLotInfoLabel();
    Label18                   = new TfLotInfoLabel();
    Label21                   = new TfLotInfoLabel();
    edtAutoCleanLowYield      = new TfLotInfoEdit();
    edtAutoCleanSiteYieldDiff = new TfLotInfoEdit();
    // -- W5-Automation ADD --
    cbProcess          = new TfLotInfoRunMode();
    // -- W5-Final-TesterTCPSocket ADD --
    labTCPIPStatus = new TfLotInfoStatusLabel();
    mmTesterLog    = new TfMainMemo();
    // -- W5-Automation ADD (AGV_PortScan unit, 20260713) -----------------------
    ALedLoader    = new TfLedValue();
    for(int i=0;i<3;i++) aLedAuto[i] = new TfLedValue();
    palRemoveTray = new TfLotInfoPanel();
    // AI(W906-W7-F0) 20260728: STATE-PRESERVING ADDITION (the only behavioural
    // line added anywhere by the F0 refactor, and it exists precisely to add
    // NOTHING behaviourally).  The retired bespoke type was
    // `struct TfLotInfoPanel { bool Enabled; bool Visible;
    //  TfLotInfoPanel():Enabled(true),Visible(true){} };` -- true/true, per its
    // own W5-Automation note ("ordinary VCL TPanel design-time defaults ...
    // no consumer depends on the initial value": both fields are only ever
    // WRITTEN, by Automation/AGV_PortScan.cpp:305-306, and tests/
    // test_agv_portscan.cpp:382-387 sets them true itself before asserting they
    // become false).  The unified vclcompat::TPanel defaults them to false, so
    // they are restored here rather than left to drift.
    palRemoveTray->Enabled = true;
    palRemoveTray->Visible = true;
    // -- W906-AutoCleanFoundation ADD (20260721) ------------------------------
    for(int iW906AC=0; iW906AC<3; iW906AC++) iUnloaderTask[iW906AC] = 0;
    // -- AI(W906-Save2DSortingSummary) 20260723 ADD: 6 new TfLotInfoEdit members --
    edtCusLotID       = new TfLotInfoEdit();
    edtCusDevGrp      = new TfLotInfoEdit();
    edtCusStep        = new TfLotInfoEdit();
    edtDevice         = new TfLotInfoEdit();
    edtSysOperatorID  = new TfLotInfoEdit();
    mmo2DLotInfo      = new vclcompat::TMemo();  // AI(W906-FW3-LotInfo-WA) 20260819: widened, see forms/fLotInfo.h member comment
    // -- AI(W906-SaveTestSummaryTSV) 20260728 ADD: 3 new TfLotInfoEdit members + 1 more
    //    (lbledtCustomer, AI(W906-SaveSummaryTrayFeed) 20260728) --
    edtASECL_LotID    = new TfLotInfoEdit();
    edInsertion       = new TfLotInfoEdit();
    edFlowID          = new TfLotInfoEdit();
    lbledtCustomer    = new TfLotInfoEdit();
    // -- AI(W906-W7-L1-Wave0) 20260801 ADD: the 7 W7-L1 fLotInfo members (see
    //    forms/fLotInfo.h for per-member golden citations and for why the ""
    //    Caption default is behaviourally load-bearing on the KYEC-AMR arm) --
    LabDiffTrayCount           = new TfLotInfoLabel();   // golden uLotInfo.h:1086
    labLoaderTrayCount         = new TfLotInfoLabel();   // golden uLotInfo.h:1080
    labNowTrayCount            = new TfLotInfoLabel();   // golden uLotInfo.h:1084
    labNowAuto1TrayID          = new TfLotInfoLabel();   // golden uLotInfo.h:799
    labNowAuto2TrayID          = new TfLotInfoLabel();   // golden uLotInfo.h:802
    labNowAuto3TrayID          = new TfLotInfoLabel();   // golden uLotInfo.h:804
    cbFirstTrayCheckOnUnloader = new TfMainCheckBox();   // golden uLotInfo.h:1026 -- offline Checked=false
    // AI(W906-PT-W3-integrate) 20260808: golden uLotInfo.h:266 -- only ->Click() is
    // touched, and that is an inherited offline no-op; see forms/fLotInfo.h for why
    // golden's OnClick chain cannot run here.
    btClearBarcodeList         = new vclcompat::TButton();  // golden uLotInfo.h:266

    // =======================================================================
    //  AI(W906-FW3-LotInfo-WA) 20260819: Wave A ADD -- see forms/fLotInfo.h
    //  for per-member golden citations.
    // =======================================================================

    // -- RefreshYieldMonitor_SIGURD ------------------------------------------
    gbManualCheckList             = new TGroupBox();
    btnManualStandard             = new TButton();
    cbMonitor_FTPRMS              = new TCheckBox();
    cbContactMode                 = new TComboBox();
    cbSiteYieldCmp_FT             = new TCheckBox();
    cbLowYieldByTotal_FT          = new TCheckBox();
    edSiteYieldCmpOnOff_Cur       = new TEdit();
    edLowYieldByTotalOnOff_Cur    = new TEdit();
    edSiteYieldCmpIg_FT           = new TEdit();
    edLowYieldByTotalIg_FT        = new TEdit();
    edSiteYieldCmpIg_Cur          = new TEdit();
    edLowYieldByTotalIg_Cur       = new TEdit();
    lblSiteYieldCmpIg_Cur         = new TLabel();
    lblLowYieldByTotalIg_Cur      = new TLabel();
    edSiteYieldCmp_FT             = new TEdit();
    edSiteYieldCmp_Cur            = new TEdit();
    lblSiteYieldCmp_Cur           = new TLabel();
    edLowYieldByTotal_FT          = new TEdit();
    edLowYieldByTotal_Cur         = new TEdit();
    lblLowYieldByTotal_Cur        = new TLabel();
    rbContsFailBySocket_FTOn      = new TRadioButton();
    rbContsFailBySocket_FTOff     = new TRadioButton();
    edtContsFailBySocket_Cur      = new TEdit();
    edContsFailSocketAlarmCT_FT   = new TEdit();
    edContsFailSocketAlarmCT_Cur  = new TEdit();
    lblContsFailSocketAlarmCT_Cur = new TLabel();
    rbContsFailByHead_FTOn        = new TRadioButton();
    rbContsFailByHead_FTOff       = new TRadioButton();
    edtContsFailByHead_Cur        = new TEdit();
    edContsFailHeadAlarmCT_FT     = new TEdit();
    edContsFailHeadAlarmCT_Cur    = new TEdit();
    lblContsFailHeadAlarmCT_Cur   = new TLabel();
    edOSBin                       = new TEdit();
    edOSBinCnt                    = new TEdit();
    edOSBinCnt_Cur                = new TEdit();
    edOSBinPreset                 = new TEdit();
    edOSBinPreset_Cur             = new TEdit();
    lblOSBin_Cur                  = new TLabel();
    grpOSBin                      = new TGroupBox();

    // -- AdjtsYieldMonitiorSize -----------------------------------------------
    pgLotinfo         = new TfLotInfoPageControl();
    pgcLotInfo        = new vclcompat::TPageControl();
    ts_ATC6_1         = new TTabSheet();
    tsATC             = new TTabSheet();
    tsASECLEventLog   = new TTabSheet();
    tsYieldMonitior   = new TTabSheet();
    tsFTP             = new TTabSheet();
    tsLotID           = new TTabSheet();
    tsMurata          = new TTabSheet();
    tsSigurd_CX       = new TTabSheet();
    tsSPIL_SZ         = new TTabSheet();
    tsOEE             = new TTabSheet();
    ts2DSort          = new TTabSheet();
    tsChipAdv         = new TTabSheet();
    tsVTest           = new TTabSheet();
    tsOCRBarCode      = new TTabSheet();
    tsAMR             = new TTabSheet();
    Height = 0; Width = 0; Top = 0; Left = 0;

    // -- SetSelectionVisible --------------------------------------------------
    groupbDownloadItem   = new TGroupBox();
    grpMesCheck          = new TGroupBox();
    tsSelection          = new TTabSheet();
    Panel28              = new TfLotInfoLayoutPanel();
    cbTestTimes          = new TComboBox();
    lblTestTimes         = new TLabel();
    lblDownloadAccessWarning = new TLabel();

    // -- ShowSocketID ----------------------------------------------------------
    for(int iWaSockR=0; iWaSockR<MAX_SOCKET_ROW; iWaSockR++)
        for(int iWaSockC=0; iWaSockC<MAX_SOCKET_COL; iWaSockC++)
        {
            SocketSiteCH_Display[iWaSockR][iWaSockC] = new TPanel();
            edSocket[iWaSockR][iWaSockC]              = new TEdit();
        }

    // -- InitialRefrigerantSystem ----------------------------------------------
    bInitFormcomponent  = false;
    OldRefrigerantCommand = false;
    ts_RefrigerantStatus_Page_2 = new TTabSheet();
    pnlRefrigerantMachine1 = new TPanel(); pnlRefrigerantMachine2 = new TPanel();
    pnlRefrigerantMachine3 = new TPanel(); pnlRefrigerantMachine4 = new TPanel();
    pnlRefrigerantMachine5 = new TPanel(); pnlRefrigerantMachine6 = new TPanel();
    pnlRefrigerantMachine7 = new TPanel(); pnlRefrigerantMachine8 = new TPanel();
    LabRefrigerantValue1 = new TLabel(); LabRefrigerantValue2 = new TLabel();
    LabRefrigerantValue3 = new TLabel(); LabRefrigerantValue4 = new TLabel();
    LabRefrigerantValue5 = new TLabel(); LabRefrigerantValue6 = new TLabel();
    LabRefrigerantValue7 = new TLabel(); LabRefrigerantValue8 = new TLabel();
    pnlRefCopm1Status_1 = new TPanel(); pnlRefCopm1Status_2 = new TPanel();
    pnlRefCopm1Status_3 = new TPanel(); pnlRefCopm1Status_4 = new TPanel();
    pnlRefCopm1Status_5 = new TPanel(); pnlRefCopm1Status_6 = new TPanel();
    pnlRefCopm1Status_7 = new TPanel(); pnlRefCopm1Status_8 = new TPanel();
    pnlRefCopm2Status_1 = new TPanel(); pnlRefCopm2Status_2 = new TPanel();
    pnlRefCopm2Status_3 = new TPanel(); pnlRefCopm2Status_4 = new TPanel();
    pnlRefCopm2Status_5 = new TPanel(); pnlRefCopm2Status_6 = new TPanel();
    pnlRefCopm2Status_7 = new TPanel(); pnlRefCopm2Status_8 = new TPanel();
    labRefCopm1HpValue_1 = new TLabel(); labRefCopm1HpValue_2 = new TLabel();
    labRefCopm1HpValue_3 = new TLabel(); labRefCopm1HpValue_4 = new TLabel();
    labRefCopm1HpValue_5 = new TLabel(); labRefCopm1HpValue_6 = new TLabel();
    labRefCopm1HpValue_7 = new TLabel(); labRefCopm1HpValue_8 = new TLabel();
    labRefCopm2HpValue_1 = new TLabel(); labRefCopm2HpValue_2 = new TLabel();
    labRefCopm2HpValue_3 = new TLabel(); labRefCopm2HpValue_4 = new TLabel();
    labRefCopm2HpValue_5 = new TLabel(); labRefCopm2HpValue_6 = new TLabel();
    labRefCopm2HpValue_7 = new TLabel(); labRefCopm2HpValue_8 = new TLabel();
    labRefCopm1LpValue_1 = new TLabel(); labRefCopm1LpValue_2 = new TLabel();
    labRefCopm1LpValue_3 = new TLabel(); labRefCopm1LpValue_4 = new TLabel();
    labRefCopm1LpValue_5 = new TLabel(); labRefCopm1LpValue_6 = new TLabel();
    labRefCopm1LpValue_7 = new TLabel(); labRefCopm1LpValue_8 = new TLabel();
    labRefCopm2LpValue_1 = new TLabel(); labRefCopm2LpValue_2 = new TLabel();
    labRefCopm2LpValue_3 = new TLabel(); labRefCopm2LpValue_4 = new TLabel();
    labRefCopm2LpValue_5 = new TLabel(); labRefCopm2LpValue_6 = new TLabel();
    labRefCopm2LpValue_7 = new TLabel(); labRefCopm2LpValue_8 = new TLabel();
    LabRefrigerantAdjustValue1 = new TLabel(); LabRefrigerantAdjustValue2 = new TLabel();
    LabRefrigerantAdjustValue3 = new TLabel(); LabRefrigerantAdjustValue4 = new TLabel();
    LabRefrigerantAdjustValue5 = new TLabel(); LabRefrigerantAdjustValue6 = new TLabel();
    LabRefrigerantAdjustValue7 = new TLabel(); LabRefrigerantAdjustValue8 = new TLabel();
    for(int iWaRef=0; iWaRef<8; iWaRef++)
    {
        TripnlRefrigerantMachine[iWaRef]    = 0;
        TriLabRefrigerantValue[iWaRef]      = 0;
        TripnlRefCopm1Status[iWaRef]        = 0;
        TripnlRefCopm2Status[iWaRef]        = 0;
        TriLabRefCopm1HpValue[iWaRef]       = 0;
        TriLabRefCopm2HpValue[iWaRef]       = 0;
        TriLabRefCopm1LpValue[iWaRef]       = 0;
        TriLabRefCopm2LpValue[iWaRef]       = 0;
        TriLabRefrigerantAdjustValue[iWaRef] = 0;
    }

    // -- SetATCFormVisible ------------------------------------------------------
    palATC              = new TPanel();
    aldATC7Status       = new TPanel();   // conflated from golden TALed*, ->Visible only
    lblATC70            = new TLabel();
    aldATCChillerStatus = new TPanel();   // conflated from golden TALed*, ->Visible only
    lblChiller          = new TLabel();
    lblATC_Now_RecipeFile = new TLabel();
    pan_ATCChillerSV    = new TPanel();
    pl_ATCChillerSV     = new TPanel();
    NetATCTime          = new TfLotInfoTimer();

    // -- CheckEventLogParameter ---------------------------------------------
    edCustomerDevice    = new TEdit();
    btnASECL_LotStart   = new TSpeedButton();

    // -- ShowATCTempPanel -----------------------------------------------------
    Pan_ATC_Use_4Head  = new TfLotInfoLayoutPanel();
    Pan_ATC_Use_8Head  = new TfLotInfoLayoutPanel();
    Pan_ATC_Use_32Head = new TfLotInfoLayoutPanel();
    for(int iWaAtcH=0; iWaAtcH<ATC_HEAD_COUNT; iWaAtcH++)
    {
        ATCChPal[iWaAtcH]    = new TPanel();
        ATCPtr[iWaAtcH]      = new TPanel();
        ATCReferPtr[iWaAtcH] = new TPanel();
    }

    // -- JCETWhite2DIDShow ------------------------------------------------------
    labCusLotID = new TLabel();

    // -- ShowInformation --------------------------------------------------------
    gbFTPAutomation_Download = new TGroupBox();
    gbFTPAutomation_Upload   = new TGroupBox();
    sbRecipeUpload           = new TSpeedButton();
    sbRecipeDownload         = new TSpeedButton();
    sbFTPAutomationSave      = new TSpeedButton();
    sbTest                   = new TfLotInfoLayoutButton();

    // -- edTempKeyUp --------------------------------------------------------------
    edTemp = new TfLotInfoTextEdit();

    // -- edtSysLotIDKeyPress / edPageKeyPress ------------------------------------
    edPage = new TEdit();

    // -- bCheckOnlyOneFile / CheckActionFlag -------------------------------------
    ledLoader = new TfLedValue(); ledEmpty = new TfLedValue(); ledColor = new TfLedValue();
    ledAuto1  = new TfLedValue(); ledAuto2 = new TfLedValue(); ledAuto3 = new TfLedValue();
    ledStartAGV = new TfLedValue(); ledSTART = new TfLedValue();
    ledLoaderTotalTray = new TfLedValue(); ledLOT_START = new TfLedValue();

    // -- btnLoadFileClick -----------------------------------------------------
    OpenDialog1        = new TfLotInfoOpenDialog();
    edSort2DIDBinFile  = new TEdit();

    // -- ReflashInfo ------------------------------------------------------------
    labAuto1TrayCount_KYEC = new TLabel();
    labAuto2TrayCount_KYEC = new TLabel();
    labAuto3TrayCount_KYEC = new TLabel();

    // -- edtLotVerifyMouseDown / edtLotVerifyKeyPress ----------------------------
    edtLotVerify = new TLabeledEdit();

    // -- BtnPauseMouseDown / BtnPauseMouseUp -------------------------------------
    BtnPause = new TfMainSpeedButton();

    // -- VisibleUploadBtnPAT ------------------------------------------------------
    sbUploadPAT = new TSpeedButton();

    // -- cbbDeviceNameChange ------------------------------------------------------
    edDeviceName  = new TEdit();
    cbbDeviceName = new TComboBox();

    // -- btStartCountClick ---------------------------------------------------------
    bStartCount_SCK = false;   // golden uLotInfo.h:1294 -- no explicit golden ctor init found; NSDMI-style false default
}
// AI(W906-AutoCleanFoundation) 20260721: golden uLotInfo.cpp:16250-16253 --
// REAL one-line body (was a total no-op stub before that wave). See
// forms/fLotInfo.h's iUnloaderTask/InitialUnLoaderTask member comments for the
// behaviour-change + dormant-call-site (SOFT_SIMULTE undefined) analysis.
void TfLotInfo::InitialUnLoaderTask(int iPos) { iUnloaderTask[iPos]=1; }
// -- W5-Automation ADD: AMR.cpp + HANA_ART.cpp method sinks (all offline no-op) --
void TfLotInfo::RefreshAMR() {}                                            // offline: no UI to refresh
void TfLotInfo::SetLotID(AnsiString /*ID*/, bool /*bReadFromFile*/) {}     // offline no-op
void TfLotInfo::SetLotStart(AnsiString /*sFunc*/, bool /*bReadFromFile*/) {} // offline no-op

// ===========================================================================
//  AI(W906-FW3-LotInfo-WA) 20260819: Wave A method bodies. See forms/
//  fLotInfo.h's file banner for WAVE SCOPE / GATE REGISTER / DEVIATION.
// ===========================================================================

// -- RefreshYieldMonitor_SIGURD (golden uLotInfo.cpp:13327-13545) -----------
void TfLotInfo::RefreshYieldMonitor_SIGURD()
{
    AnsiString sPathName,sCheckListName,asLastOpenFN,asString;
    int iCheck,iBin;
    bool bCheck;
    double dCheck;
    bool bFailAlarmLowYieldByTotal,bContsFailBySocket,bContsFailByHead;
    int iLowYieldCountByTotal, iContsFailSocketAlarmCT, iContsFailHeadAlarmCT, iFailAlarmSiteYieldCmpCount;
    double dFailAlarmSiteYieldCmp,dLowYieldLimitByTotal;

    gbManualCheckList->Visible=(AccessLevel>=iDefHonPrecLevel)?true:false;
    btnManualStandard->Visible=(CUSTOMER_CODE==CC_UTAC_TW || CUSTOMER_CODE==CC_SIGURD_PeiXing);
    AdjtsYieldMonitiorSize();

    asLastOpenFN=GetLastOpenFN();
    sPathName.sprintf("D:\\HT9045_Log\\CheckingList");
    sCheckListName.sprintf("%s\\%s.txt", sPathName, asLastOpenFN);

    cbMonitor_FTPRMS->Checked=IniConfig.bA32EnableFTPAutomation;
    // AI(W906-FW3-LotInfo-WA) 20260819: GATE WA-1 -- golden `cbContactMode->
    // ItemIndex=fContact->cbContactMode->ItemIndex;` (uLotInfo.cpp:13346).
    // `fContact` (a golden TfContact*, distinct from the real fContactCT/
    // TfContactCT this tree already ports) has no port anywhere in this tree.
    // See forms/fLotInfo.h GATE REGISTER WA-1.
#if 0
    cbContactMode->ItemIndex=fContact->cbContactMode->ItemIndex;
#endif

    if(iTestRunMode==RT)
    {
        bFailAlarmLowYieldByTotal   =TestIF_File.bFailAlarmLowYieldByTotal_RT;
        bContsFailBySocket          =TestIF_File.bContsFailBySocket_RT;
        bContsFailByHead            =TestIF_File.bContsFailByHead_RT;
        iLowYieldCountByTotal       =TestIF_File.iLowYieldCountByTotal_RT;
        dFailAlarmSiteYieldCmp      =TestIF_File.dFailAlarmSiteYieldCmp_RT;
        dLowYieldLimitByTotal       =TestIF_File.dLowYieldLimitByTotal_RT;
        iContsFailSocketAlarmCT     =TestIF_File.iContsFailSocketAlarmCT_RT;
        iContsFailHeadAlarmCT       =TestIF_File.iContsFailHeadAlarmCT_RT;
        iFailAlarmSiteYieldCmpCount =TestIF_File.iFailAlarmSiteYieldCmpCount_RT;
    }
    else
    {
        bFailAlarmLowYieldByTotal   =TestIF_File.bFailAlarmLowYieldByTotal;
        bContsFailBySocket          =TestIF_File.bContsFailBySocket;
        bContsFailByHead            =TestIF_File.bContsFailByHead;
        iLowYieldCountByTotal       =TestIF_File.iLowYieldCountByTotal;
        dFailAlarmSiteYieldCmp      =TestIF_File.dFailAlarmSiteYieldCmp;
        dLowYieldLimitByTotal       =TestIF_File.dLowYieldLimitByTotal;
        iContsFailSocketAlarmCT     =TestIF_File.iContsFailSocketAlarmCT;
        iContsFailHeadAlarmCT       =TestIF_File.iContsFailHeadAlarmCT;
        iFailAlarmSiteYieldCmpCount =TestIF_File.iFailAlarmSiteYieldCmpCount;
    }

    bCheck=ReadIniData(sCheckListName, "FT_Yield",    "Yield Func",     bFailAlarmLowYieldByTotal);
    cbSiteYieldCmp_FT->Checked=bCheck;
    cbLowYieldByTotal_FT->Checked=bCheck;
    if(bCheck!=bFailAlarmLowYieldByTotal && iTestRunMode==FT)
    {
        edSiteYieldCmpOnOff_Cur->Visible=true;
        edLowYieldByTotalOnOff_Cur->Visible=true;
        edSiteYieldCmpOnOff_Cur->Text=(bFailAlarmLowYieldByTotal)?"On":"OFF";
        edLowYieldByTotalOnOff_Cur->Text=(bFailAlarmLowYieldByTotal)?"On":"OFF";
    }
    else
    {
        edSiteYieldCmpOnOff_Cur->Visible=false;
        edLowYieldByTotalOnOff_Cur->Visible=false;
    }

    iCheck=ReadIniData(sCheckListName, "FT_Yield",    "Preset",          iLowYieldCountByTotal);
    edSiteYieldCmpIg_FT->Text=iCheck;
    edLowYieldByTotalIg_FT->Text=iCheck;
    if((iCheck!=iLowYieldCountByTotal ||
        iCheck!=iFailAlarmSiteYieldCmpCount) &&
       iTestRunMode==FT)
    {
        edSiteYieldCmpIg_Cur->Visible=true;
        edLowYieldByTotalIg_Cur->Visible=true;
        edSiteYieldCmpIg_Cur->Text=iFailAlarmSiteYieldCmpCount;
        edLowYieldByTotalIg_Cur->Text=iLowYieldCountByTotal;
        lblSiteYieldCmpIg_Cur->Visible=true;
        lblLowYieldByTotalIg_Cur->Visible=true;
    }
    else
    {
        edSiteYieldCmpIg_Cur->Visible=false;
        edLowYieldByTotalIg_Cur->Visible=false;
        lblSiteYieldCmpIg_Cur->Visible=false;
        lblLowYieldByTotalIg_Cur->Visible=false;
    }

    dCheck=ReadIniData(sCheckListName, "FT_Yield", "Variance",        dFailAlarmSiteYieldCmp);
    edSiteYieldCmp_FT->Text=dCheck;
    if(dCheck!=dFailAlarmSiteYieldCmp && iTestRunMode==FT)
    {
        edSiteYieldCmp_Cur->Visible=true;
        edSiteYieldCmp_Cur->Text=dFailAlarmSiteYieldCmp;
        lblSiteYieldCmp_Cur->Visible=true;
    }
    else
    {
        edSiteYieldCmp_Cur->Visible=false;
        lblSiteYieldCmp_Cur->Visible=false;
    }

    dCheck=ReadIniData(sCheckListName, "FT_Yield", "Low Yield",      dLowYieldLimitByTotal);
    edLowYieldByTotal_FT->Text=dCheck;
    if(dCheck!=dLowYieldLimitByTotal && iTestRunMode==FT)
    {
        edLowYieldByTotal_Cur->Visible=true;
        edLowYieldByTotal_Cur->Text=dLowYieldLimitByTotal;
        lblLowYieldByTotal_Cur->Visible=true;
    }
    else
    {
        edLowYieldByTotal_Cur->Visible=false;
        lblLowYieldByTotal_Cur->Visible=false;
    }

    bCheck=ReadIniData(sCheckListName, "FT_Yield",    "ContsFailBySocket Func",     bContsFailBySocket);
    rbContsFailBySocket_FTOn ->Checked=bCheck;
    rbContsFailBySocket_FTOff->Checked=!bCheck;

    if(bCheck!=bContsFailBySocket && iTestRunMode==FT)
    {
        edtContsFailBySocket_Cur->Visible=true;
        edtContsFailBySocket_Cur->Text=(bContsFailBySocket)?"On":"OFF";
    }
    else
    {
        edtContsFailBySocket_Cur->Visible=false;
    }

    iCheck=ReadIniData(sCheckListName, "Alarm", "Socket",   (int)iContsFailSocketAlarmCT);
    edContsFailSocketAlarmCT_FT->Text=iCheck;
    if(iCheck!=(int)iContsFailSocketAlarmCT && iTestRunMode==FT)
    {
        edContsFailSocketAlarmCT_Cur->Visible=true;
        edContsFailSocketAlarmCT_Cur->Text=iContsFailSocketAlarmCT;
    }
    else
    {
        edContsFailSocketAlarmCT_Cur->Visible=false;
    }
    lblContsFailSocketAlarmCT_Cur->Visible=(edtContsFailBySocket_Cur->Visible || edContsFailSocketAlarmCT_Cur->Visible)?true:false;

    bCheck=ReadIniData(sCheckListName, "FT_Yield",    "ContsFailByHead Func",     bContsFailByHead);
    rbContsFailByHead_FTOn ->Checked=bCheck;
    rbContsFailByHead_FTOff->Checked=!bCheck;
    if(bCheck!=bContsFailByHead && iTestRunMode==FT)
    {
        edtContsFailByHead_Cur->Visible=true;
        edtContsFailByHead_Cur->Text=(bContsFailByHead)?"On":"OFF";
    }
    else
    {
        edtContsFailByHead_Cur->Visible=false;
    }

    iCheck=ReadIniData(sCheckListName, "Alarm", "Head",     (int)TestIF_File.iContsFailHeadAlarmCT);
    edContsFailHeadAlarmCT_FT->Text=iCheck;
    if(iCheck!=(int)iContsFailHeadAlarmCT && iTestRunMode==FT)
    {
        edContsFailHeadAlarmCT_Cur->Visible=true;
        edContsFailHeadAlarmCT_Cur->Text=TestIF_File.iContsFailHeadAlarmCT;
    }
    else
    {
        edContsFailHeadAlarmCT_Cur->Visible=false;
    }

    lblContsFailHeadAlarmCT_Cur->Visible=(edtContsFailByHead_Cur->Visible || edContsFailHeadAlarmCT_Cur->Visible)?true:false;

    iBin=ReadIniData(sCheckListName, "Tester_Control", "SGOSBIN",   0);
    if(iBin>0)
    {
        edOSBin->Text=iBin;
        asString.sprintf("Category%d", iBin);
        iCheck=ReadIniData(sCheckListName, asString, "Fail Percent Ignore",   BinSelect[iTestRunMode].iPersentIgnore[iBin]);
        edOSBinCnt->Text=iCheck;
        if(iCheck!=BinSelect[iTestRunMode].iPersentIgnore[iBin])
        {
            edOSBinCnt_Cur->Text=BinSelect[iTestRunMode].iPersentIgnore[iBin];
            edOSBinCnt_Cur->Visible=true;
        }
        else
        {
            edOSBinCnt_Cur->Visible=false;
        }

        dCheck=ReadIniData(sCheckListName, asString, "Fail Percent Num",   BinSelect[iTestRunMode].dFailureLimit[iBin]);
        edOSBinPreset->Text=dCheck;
        if(dCheck!=BinSelect[iTestRunMode].dFailureLimit[iBin])
        {
            edOSBinPreset_Cur->Text=BinSelect[iTestRunMode].dFailureLimit[iBin];
            edOSBinPreset_Cur->Visible=true;
        }
        else
        {
            edOSBinPreset_Cur->Visible=false;
        }
        lblOSBin_Cur->Visible=(edOSBinCnt_Cur->Visible || edOSBinPreset_Cur->Visible)?true:false;
    }
    else
    {
        edOSBin->Text="Null";
        edOSBinCnt->Text="Null";
        edOSBinPreset->Text="Null";
    }

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)
        grpOSBin->Visible=false;
    else
        grpOSBin->Visible=true;

    AdjtsYieldMonitiorSize();
}

// -- TransformTemperature_AirStream (golden uLotInfo.cpp:15080-15193) -------
double TfLotInfo::TransformTemperature_AirStream(double Offset,int iIndex)
{
    if(AirStream_Select==0)
        return 0.0;

    TIniFile *tIni = new TIniFile("D:\\HT9045\\config\\AirStream.ini");

    AnsiString sTemp_Array[16]={"-60~-51","-50~-41","-40~-21","-20~-11","-10~0"  ,"1~15"   ,"16~25"  ,"26~50",
                                "51~80"  ,"81~90"  ,"91~105" ,"106~120","121~130","131~145","146~160","161~175"};

    AnsiString sLoadIni="";
    double dbSetTemp=atof(fMain->edWorkTemperBase->Text.c_str());
    double dRseult=dbSetTemp;

    int iTemp=-1;
    if(-60<=dbSetTemp && dbSetTemp<=-51)
        iTemp=0;
    else if(-50<=dbSetTemp && dbSetTemp<=-41)
        iTemp=1;
    else if(-40<=dbSetTemp && dbSetTemp<=-21)
        iTemp=2;
    else if(-20<=dbSetTemp && dbSetTemp<=-11)
        iTemp=3;
    else if(-10<=dbSetTemp && dbSetTemp<=0)
        iTemp=4;
    else if(1<=dbSetTemp && dbSetTemp<=15)
        iTemp=5;
    else if(16<=dbSetTemp && dbSetTemp<=25)
        iTemp=6;
    else if(26<=dbSetTemp && dbSetTemp<=50)
        iTemp=7;
    else if(51<=dbSetTemp && dbSetTemp<=80)
        iTemp=8;
    else if(81<=dbSetTemp && dbSetTemp<=90)
        iTemp=9;
    else if(91<=dbSetTemp && dbSetTemp<=105)
        iTemp=10;
    else if(106<=dbSetTemp && dbSetTemp<=120)
        iTemp=11;
    else if(121<=dbSetTemp && dbSetTemp<=130)
        iTemp=12;
    else if(131<=dbSetTemp && dbSetTemp<=145)
        iTemp=13;
    else if(146<=dbSetTemp && dbSetTemp<=160)
        iTemp=14;
    else if(161<=dbSetTemp && dbSetTemp<=175)
        iTemp=15;

    if(iTemp==-1)
    {
        delete tIni;
        return dRseult;
    }

    // AI(W906-FW3-LotInfo-WA) 20260819: GATE WA-2 -- the lazy-init default-
    // table write to D:\HT9045\config\AirStream.ini. See forms/fLotInfo.h
    // GATE REGISTER WA-2 (config\ is a shared production-machine runtime
    // parameter directory, default read-only per AGENTS.md/CLAUDE.md).
#if 0
    if(!tIni->ValueExists("Temp_Index", "-60~-51"))
    {
        tIni->WriteString("Temp_Index", "-60~-51", "-70");
        tIni->WriteString("Temp_Index", "-50~-41", "-70");
        tIni->WriteString("Temp_Index", "-40~-21", "-60");
        tIni->WriteString("Temp_Index", "-20~-11", "-60");
        tIni->WriteString("Temp_Index", "-10~0"  , "-50");
        tIni->WriteString("Temp_Index", "1~15"   , "-50");
        tIni->WriteString("Temp_Index", "16~25"  , "-50");
        tIni->WriteString("Temp_Index", "26~50"  , "25");
        tIni->WriteString("Temp_Index", "51~80"  , "30");
        tIni->WriteString("Temp_Index", "81~90"  , "30");
        tIni->WriteString("Temp_Index", "91~105" , "35");
        tIni->WriteString("Temp_Index", "106~120", "35");
        tIni->WriteString("Temp_Index", "121~130", "35");
        tIni->WriteString("Temp_Index", "131~145", "35");
        tIni->WriteString("Temp_Index", "146~160", "35");
        tIni->WriteString("Temp_Index", "161~175", "35");

        tIni->WriteString("Temp_Socket","-60~-51", "-70");
        tIni->WriteString("Temp_Socket","-50~-41", "-70");
        tIni->WriteString("Temp_Socket","-40~-21", "-50");
        tIni->WriteString("Temp_Socket","-20~-11", "-30");
        tIni->WriteString("Temp_Socket","-10~0"  , "-10");
        tIni->WriteString("Temp_Socket","1~15"   , "-10");
        tIni->WriteString("Temp_Socket","16~25"  , "10");
        tIni->WriteString("Temp_Socket","26~50"  , "25");
        tIni->WriteString("Temp_Socket","51~80"  , "90");
        tIni->WriteString("Temp_Socket","81~90"  , "100");
        tIni->WriteString("Temp_Socket","91~105" , "120");
        tIni->WriteString("Temp_Socket","106~120", "140");
        tIni->WriteString("Temp_Socket","121~130", "145");
        tIni->WriteString("Temp_Socket","131~145", "160");
        tIni->WriteString("Temp_Socket","146~160", "190");
        tIni->WriteString("Temp_Socket","161~175", "215");
    }
#endif

    dRseult=0;
    if(iIndex==0)
    {
        sLoadIni=tIni->ReadString("Temp_Index",sTemp_Array[iTemp],sLoadIni);
        dRseult= Offset+atof(sLoadIni.c_str());
        if(dRseult>35)
            dRseult=35;
    }
    else
    {
        sLoadIni=tIni->ReadString("Temp_Socket",sTemp_Array[iTemp],sLoadIni);
        dRseult = Offset+atof(sLoadIni.c_str());
    }

    if(dRseult<-70)
        dRseult=-70;

    if(dRseult>230)
        dRseult=230;

    delete tIni;
    return dRseult;
}

// -- AdjtsYieldMonitiorSize (golden uLotInfo.cpp:13636-13717) ----------------
void TfLotInfo::AdjtsYieldMonitiorSize()
{
    if(pgLotinfo->ActivePage==ts_ATC6_1)
    {
        Height=610;
        Width=440;
    }
    else if(pgLotinfo->ActivePage==tsATC)
    {
        Height=610;
        Width=580;
    }
    else if(pgLotinfo->ActivePage==tsASECLEventLog)
    {
        Width=671;
    }
    else if(CUSTOMER_CODE==CC_PANTHER)
    {
        Width=671;
    }
    else if(pgLotinfo->ActivePage==tsYieldMonitior &&
            IniConfig.bSIGURDFunction)
    {
        Width=530;
        if(grpOSBin->Visible && gbManualCheckList->Visible)
            Height=605;
        else if(grpOSBin->Visible && gbManualCheckList->Visible==false)
            Height=550;
        else
            Height=460;
        // AI(W906-FW3-LotInfo-WA) 20260819: GATE WA-3 -- see forms/fLotInfo.h
        // GATE REGISTER WA-3 (RefreshYieldMonitor is RECON item #115, (b)
        // write-path, not one of this wave's 39 Tier-1 methods).
#if 0
        RefreshYieldMonitor();
#endif
    }
    else if(pgLotinfo->ActivePage==tsYieldMonitior &&
            CosFunction.bShowYieldMonitor)
    {
        Width=530;
        Height=601;
#if 0   // GATE WA-3, see above
        RefreshYieldMonitor();
#endif
    }
    else if(CosFunction.bEnableHandlerResultServer && pgLotinfo->ActivePage==tsAMR)
    {
        Width=420;
        RefreshAMR();
    }
    else if(pgLotinfo->ActivePage==tsFTP)
    {
        Width=340;
    }
    else if(pgLotinfo->ActivePage==tsLotID && USE_RFID_READER)
    {
        Height=666;
    }
    else if(pgLotinfo->ActivePage==tsLotID && tsMurata->TabVisible==false  &&
            tsSigurd_CX->TabVisible==false && tsSPIL_SZ->TabVisible==false &&
            tsOEE->TabVisible==false       && ts2DSort->TabVisible==false  &&
            tsChipAdv->TabVisible==false   && tsVTest->TabVisible==false)
    {
        pgcLotInfo->Visible=false;
        Width=410;
    }
    else if(pgLotinfo->ActivePage==tsOCRBarCode)
    {
        Height=610;
        Width=530;
    }
    else if(IniConfig.bVTESTFunction==true)
    {
        Height=470;
        Width=620;
    }
    else
    {
        Height=490;
        Width=671;
    }
}

// -- SetSelectionVisible (golden uLotInfo.cpp:1262-1342) ---------------------
void TfLotInfo::SetSelectionVisible()
{
    if(IniConfig.bVTESTFunction==true)
    {
        if((IniConfig.bEnableRms==true || IniConfig.bEnableFTP==true) &&
            AccessLevel>=iDefEngineerLevel)
        {
            if(AccessLevel>=iDefSupervisorLevel)
            {
                groupbDownloadItem->Visible=true;
                grpMesCheck->Visible=true;
            }
            else
            {
                groupbDownloadItem->Visible=false;
                grpMesCheck->Visible=false;
            }
            tsSelection->TabVisible=true;
        }
        else
        {
            tsSelection->TabVisible=false;
        }
        Panel28->Width=285;
        cbTestTimes->Visible=true;
        lblTestTimes->Visible=true;
    }
    else
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE)
        {
            tsSelection->TabVisible=false;
        }
        else
        {
            if(IniConfig.bEnableRms==true ||
               IniConfig.bEnableErms==true ||
               IniConfig.bEnableFTP==true ||
               IniConfig.bSPILFunction)
                tsSelection->TabVisible=true;
            else
                tsSelection->TabVisible=false;
        }
    }
    if(IniConfig.bA75DownloadItemByAccessLevel==true)
    {
        bool bDenyByOP=(AccessLevel==0);
        // AI(W906-FW3-LotInfo-WA) 20260819: GATE WA-4 -- the child-control
        // enable/disable loop. See forms/fLotInfo.h GATE REGISTER WA-4
        // (vclcompat::TGroupBox has no Controls[]/ControlCount surface;
        // adding one is a shared-header change outside this wave's write
        // boundary).
#if 0
        for(int i=0; i<groupbDownloadItem->ControlCount; i++)
        {
            if(groupbDownloadItem->Controls[i]!=lblDownloadAccessWarning)
                groupbDownloadItem->Controls[i]->Enabled=!bDenyByOP;
        }
#endif
        lblDownloadAccessWarning->Visible=bDenyByOP;
    }
    else
    {
        lblDownloadAccessWarning->Visible=false;
    }
}

// -- ShowSocketID (golden uLotInfo.cpp:11443-11516) --------------------------
void TfLotInfo::ShowSocketID()
{
    int iMode=TestIF_File.iTestMode;
    int iTestCHCT=SiteData[iMode].Cnt;

    if(IniConfig.bDualSiteSupply4CH==true)
    {
        if(TestIF_File.iTestMode==DualSite)
        {
            iTestCHCT+=2;
        }
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        if((TestIF_File.iTestMode==_8Site2X4 ||
            TestIF_File.iTestMode==_16Site4X4) &&
            CosFunction.bEnableOctal_12Kit==true)
        {
            if(TestIF_File.bOctal_12Kit==true)
                iTestCHCT+=4;
            else
                iTestCHCT=SiteData[iMode].Cnt;
        }
    }

    if(CosFunction.bUse32ChanelSiteMap)
    {
        iTestCHCT=32;

        if(IniConfig.bVTESTFunction)
        {
            iTestCHCT=16;
        }
    }
    // GOLDEN ODDITY (recorded, not "fixed"): golden computes iTestCHCT through
    // 3 customer-specific override branches above but never reads it again
    // anywhere in this function body (verified against the full golden span,
    // uLotInfo.cpp:11443-11516) -- the loops below key off SiteData[iMode].
    // XItem/YItem instead. Translated literally; `(void)` silences the
    // resulting -Wunused-but-set-variable this wave's -Wall build would
    // otherwise raise on a real dead local.
    (void)iTestCHCT;

    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        SocketLabRow_Display[i]->Visible=false;
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            SocketSiteCH_Display[i][j]->Caption="";
            SocketSiteCH_Display[i][j]->Visible=false;
            SocketLabCol_Display[j]->Visible=false;
            edSocket[i][j]->Visible=false;
        }
    }

    for(int i=0; i<SiteData[iMode].XItem; i++)
    {
        SocketLabCol_Display[i]->Visible=true;
        for(int j=0; j<SiteData[iMode].YItem; j++)
        {
            SocketSiteCH_Display[j][i]->Visible=true;
            SocketLabRow_Display[j]->Visible=true;
            edSocket[j][i]->Visible=true;
        }
    }

    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            if(TestIF_File.iSiteMap[i][j]==0)
            {
                SocketSiteCH_Display[i][j]->Caption="";
            }
            else
            {
                SocketSiteCH_Display[i][j]->Caption=("CH "+AnsiString (TestIF_File.iSiteMap[i][j]));
            }
        }
    }
}

// -- CheckAirMachineStatus (golden uLotInfo.cpp:14805-14868) -- WA-6 --------
void TfLotInfo::CheckAirMachineStatus()
{
    if(AirStream_Select==0)
        return;
    // AI(W906-FW3-LotInfo-WA) 20260819: GATE WA-6 -- the entire remaining
    // body. See forms/fLotInfo.h GATE REGISTER WA-6: (a) every line below
    // dereferences ATC_InterfaceForm->AirMachineInfo/.AirMachineInfo_Index/
    // .IsConnect(), none of which exist on the acarry_shims.h
    // TATC_InterfaceFormShim (which carries only iATC_MODE_TYPE); (b) the
    // SystemStart-gated sub-block also raises a real WAR1611 alarm via
    // ShowErrorMessage, the same SAFETY-classified category as forms/
    // fTemperFrom.h's own GATE (T1). RECON's "verified fully" call on this
    // function (docs/RECON_uLotInfo_displayside.md row #156) was wrong on
    // both counts -- see this wave's final report.
#if 0
    palAirMachineStatus              ->Caption = ATC_InterfaceForm->AirMachineInfo.asATC_AirMachineStatus;
    palAirMachineSetTemperature      ->Caption = ATC_InterfaceForm->AirMachineInfo.asATC_AirMachineSetTemp;
    palAirMachineAlarmStatus         ->Caption = ATC_InterfaceForm->AirMachineInfo.asATC_AirMachineAlarm;
    palAirDefrostSec                 ->Caption = ATC_InterfaceForm->AirMachineInfo.iDefrostSec;
    pal_Air_Machine_Temp_Ch1         ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo.iATC_AirMachineSocketChTemp[0]);
    pal_Air_Machine_Temp_Ch2         ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo.iATC_AirMachineSocketChTemp[1]);
    pal_AirStream_AirVolume          ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo.iATC_AirVolume);

    if(ATC_InterfaceForm->iATC_MODE_TYPE==61)
    {
        palAirMachineStatus_Index        ->Caption = ATC_InterfaceForm->AirMachineInfo_Index.asATC_AirMachineStatus;
        palAirMachineSetTemperature_Index->Caption = ATC_InterfaceForm->AirMachineInfo_Index.asATC_AirMachineNowTemp;
        palAirMachineAlarmStatus_Index   ->Caption = ATC_InterfaceForm->AirMachineInfo_Index.asATC_AirMachineAlarm;
        palAirDefrostSec_Index           ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo_Index.iDefrostSec);
        pnl_AirMachineTemp_Index_Ch_1    ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo_Index.iATC_AirMachineSocketChTemp[0]);
        pnl_AirMachineTemp_Index_Ch_2    ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo_Index.iATC_AirMachineSocketChTemp[1]);
        pal_AirStream_AirVolume_Index    ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo_Index.iATC_AirVolume);
    }

    if(bUT150Install[tcATCHotAir1]==true || bUT150Install[tcATCHotAir2]==true)
    {
        if(ATC_InterfaceForm->IsConnect())
        {
            UN150Read[tcATCHotAir1]=ATC_InterfaceForm->AirMachineInfo_Index.iATC_AirMachineSocketChTemp[1];
            UN150Read[tcATCHotAir2]=ATC_InterfaceForm->AirMachineInfo.iATC_AirMachineSocketChTemp[1];
        }
        else
        {
            UN150Read[tcATCHotAir1]=999;
            UN150Read[tcATCHotAir2]=999;
        }
    }

    if(SystemStart)
    {
        if(Temperature.EnableAirMachineSocket)
        {
            if(palAirMachineStatus->Caption != "Run" && palAirMachineStatus->Caption != "NoUse")
            {
                if(LastSet.iLanguageCountry==1)
                    ShowMyMessage("冷風機正在執行自動除霜，請等待除霜完成再操作");
                else
                    ShowMyMessage("The air cooler is performing automatic defrosting, \n please wait until it is completed before using it");
            }
            else if(palAirMachineStatus->Caption != "Run" && palAirMachineStatus->Caption != "NoUse")
            {
                if(bManualAirCoolingOnOff ==false)
                    ShowErrorMessage("WAR1611", K_RETRY, MMSystem);
            }
        }
        else
        {
            if(palAirMachineStatus->Caption == "0")
            {
                ShowErrorMessage("WAR1611", K_RETRY, MMSystem);
            }
        }
    }
#endif
}

// -- InitialRefrigerantSystem (golden uLotInfo.cpp:14877-14939) -------------
void TfLotInfo::InitialRefrigerantSystem()
{
    if(AirStream_Select==0)
        return;

    TPanel *pnlRefrigerantMachine[8]={pnlRefrigerantMachine1,pnlRefrigerantMachine2,pnlRefrigerantMachine3,pnlRefrigerantMachine4,
                                        pnlRefrigerantMachine5,pnlRefrigerantMachine6,pnlRefrigerantMachine7,pnlRefrigerantMachine8};

    TLabel *LabRefrigerantValue[8]={LabRefrigerantValue1,LabRefrigerantValue2,LabRefrigerantValue3,LabRefrigerantValue4,
                                        LabRefrigerantValue5,LabRefrigerantValue6,LabRefrigerantValue7,LabRefrigerantValue8};

    TPanel *pnlRefCopm1Status[8]={pnlRefCopm1Status_1,pnlRefCopm1Status_2,pnlRefCopm1Status_3,pnlRefCopm1Status_4,
                                        pnlRefCopm1Status_5,pnlRefCopm1Status_6,pnlRefCopm1Status_7,pnlRefCopm1Status_8};

    TPanel *pnlRefCopm2Status[8]={pnlRefCopm2Status_1,pnlRefCopm2Status_2,pnlRefCopm2Status_3,pnlRefCopm2Status_4,
                                        pnlRefCopm2Status_5,pnlRefCopm2Status_6,pnlRefCopm2Status_7,pnlRefCopm2Status_8};

    TLabel *LabRefCopm1HpValue[8]={labRefCopm1HpValue_1,labRefCopm1HpValue_2,labRefCopm1HpValue_3,labRefCopm1HpValue_4,
                                        labRefCopm1HpValue_5,labRefCopm1HpValue_6,labRefCopm1HpValue_7,labRefCopm1HpValue_8};

    TLabel *LabRefCopm2HpValue[8]={labRefCopm2HpValue_1,labRefCopm2HpValue_2,labRefCopm2HpValue_3,labRefCopm2HpValue_4,
                                        labRefCopm2HpValue_5,labRefCopm2HpValue_6,labRefCopm2HpValue_7,labRefCopm2HpValue_8};

    TLabel *LabRefCopm1LpValue[8]={labRefCopm1LpValue_1,labRefCopm1LpValue_2,labRefCopm1LpValue_3,labRefCopm1LpValue_4,
                                        labRefCopm1LpValue_5,labRefCopm1LpValue_6,labRefCopm1LpValue_7,labRefCopm1LpValue_8};

    TLabel *LabRefCopm2LpValue[8]={labRefCopm2LpValue_1,labRefCopm2LpValue_2,labRefCopm2LpValue_3,labRefCopm2LpValue_4,
                                        labRefCopm2LpValue_5,labRefCopm2LpValue_6,labRefCopm2LpValue_7,labRefCopm2LpValue_8};

    TLabel *LabRefrigerantAdjustValue[8]={LabRefrigerantAdjustValue1,LabRefrigerantAdjustValue2,LabRefrigerantAdjustValue3,LabRefrigerantAdjustValue4,
                                        LabRefrigerantAdjustValue5,LabRefrigerantAdjustValue6,LabRefrigerantAdjustValue7,LabRefrigerantAdjustValue8};
    for(int i=0; i<8; i++)
    {
        TripnlRefrigerantMachine[i] =pnlRefrigerantMachine[i];
        TriLabRefrigerantValue[i]   =LabRefrigerantValue[i];
        TripnlRefCopm1Status[i]     =pnlRefCopm1Status[i];
        TripnlRefCopm2Status[i]     =pnlRefCopm2Status[i];
        TriLabRefCopm1HpValue[i]    =LabRefCopm1HpValue[i];
        TriLabRefCopm2HpValue[i]    =LabRefCopm2HpValue[i];
        TriLabRefCopm1LpValue[i]    =LabRefCopm1LpValue[i];
        TriLabRefCopm2LpValue[i]    =LabRefCopm2LpValue[i];
        TriLabRefrigerantAdjustValue[i]    =LabRefrigerantAdjustValue[i];
    }
    bInitFormcomponent = false;
    OldRefrigerantCommand = false;
    // AI(W906-FW3-LotInfo-WA) 20260819: GATE WA-5 -- ATC_OFFLINE_FormComInit()
    // is RECON Tier 2 (unverified), not one of this wave's 39 Tier-1 methods.
    // See forms/fLotInfo.h GATE REGISTER WA-5.
#if 0
    ATC_OFFLINE_FormComInit();
#endif
    if(Total_Compressor>5)
        ts_RefrigerantStatus_Page_2->TabVisible=true;
    else
        ts_RefrigerantStatus_Page_2->TabVisible=false;

    for(int i=0; i<8; i++)
    {
        if(i<Total_Compressor)
        {
            pnlRefrigerantMachine[i]->Visible=true;
        }
        else
        {
            pnlRefrigerantMachine[i]->Visible=false;
        }
    }
}

// -- SetATCFormVisible (golden uLotInfo.cpp:9938-9985) -----------------------
void TfLotInfo::SetATCFormVisible()
{
    if(Tri_Temp_Machine==1)
    {
        tsATC->TabVisible=false;
    }
    else
    {
        if(ATC_SYSTEM==eATCHonPrecType)
        {
            if(Temperature.bATC70Active==true)
            {
                palATC->Caption="ATC 7.0 Monitor";
                aldATC7Status->Visible=true;
                lblATC70->Visible=true;
                aldATCChillerStatus->Visible=false;
                lblChiller->Visible=false;
            }
            else
            {
                palATC->Caption="ATC 2.0 Monitor";
                aldATC7Status->Visible=false;
                lblATC70->Visible=false;
                aldATCChillerStatus->Visible=true;
                lblChiller->Visible=true;
            }
            tsATC->TabVisible=true;
        }
        else if(ATC_SYSTEM==eNewATCSystem)
        {
            palATC->Caption="ATC Monitor";
            tsATC->TabVisible=true;

            aldATC7Status->Visible=false;
            lblATC70->Visible=false;
            lblChiller->Visible=false;
            aldATCChillerStatus->Visible=false;
            pan_ATCChillerSV->Visible=false;
            pl_ATCChillerSV->Visible=false;
            lblATC_Now_RecipeFile->Visible=true;
            NetATCTime->Enabled=true;
        }
        else
        {
            tsATC->TabVisible=false;
        }
    }
}

// -- CheckEventLogParameter (golden uLotInfo.cpp:10673-10720) ----------------
bool TfLotInfo::CheckEventLogParameter()
{
    AnsiString S;

    if(CUSTOMER_CODE!=CC_ASE_CL ||
       IniConfig.bN22Enable_EventLog==false ||
       LastSet.iTester==OFF_LINE)
        return true;

    S=edtASECL_LotID->Text;
    S=S.Trim();
    if(S.Length()<=0)
    {
        ShowMyMessage("Lot Name must Key in!!");
        return false;
    }

    S=edInsertion->Text;
    S=S.Trim();
    if(S.Length()<=0)
    {
        ShowMyMessage("Insertion must Key in!!");
        return false;
    }

    S=edCustomerDevice->Text;
    S=S.Trim();
    if(S.Length()<=0)
    {
        ShowMyMessage("Customer Device must Key in!!");
        return false;
    }

    S=edFlowID->Text;
    S=S.Trim();
    if(S.Length()<=0)
    {
        ShowMyMessage("Flow ID must Key in!!");
        return false;
    }

    if(btnASECL_LotStart->Down==false)
    {
        ShowMyMessage("Must Press Lot Start First!!");
        return false;
    }
    return true;
}

// -- ShowATCTempPanel (golden uLotInfo.cpp:14561-14606) ----------------------
void TfLotInfo::ShowATCTempPanel()
{
    if(iATC_Use_Heat_Count==8)
    {
        Pan_ATC_Use_4Head->Left=4;
        Pan_ATC_Use_8Head->Top=216;
        Pan_ATC_Use_8Head->Left=4;
        Pan_ATC_Use_8Head->Visible=true;
        Pan_ATC_Use_32Head->Visible=false;
    }
    else if(iATC_Use_Heat_Count>8)
    {
        Pan_ATC_Use_4Head->Left=0;
        Pan_ATC_Use_4Head->Top=124;
        Pan_ATC_Use_8Head->Top=124;
        Pan_ATC_Use_8Head->Left=272;
        Pan_ATC_Use_8Head->Visible=true;
        Pan_ATC_Use_32Head->Visible=true;
    }
    else
    {
        Pan_ATC_Use_4Head->Left=4;
        Pan_ATC_Use_4Head->Top=124;
        Pan_ATC_Use_8Head->Visible=false;
        Pan_ATC_Use_32Head->Visible=false;
    }

    for(int i=0; i<ATC_HEAD_COUNT; i++)
    {
        ATCChPal[i]->Visible=(i<iATC_Use_Heat_Count);
        ATCPtr[i]->Visible=(i<iATC_Use_Heat_Count);
    }

    if(Temperature.bUseReferTempSensor==true)
    {
        for(int i=0; i<ATC_HEAD_COUNT; i++)
        {
            ATCReferPtr[i]->Visible=(i<iATC_Use_Heat_Count);
        }
    }
    else
    {
        for(int i=0; i<ATC_HEAD_COUNT; i++)
            ATCReferPtr[i]->Visible=false;
    }
}

// -- JCETWhite2DIDShow (golden uLotInfo.cpp:15953-15992) ---------------------
void TfLotInfo::JCETWhite2DIDShow(bool bUse)
{
    AnsiString sPath=AuthPath+"config.ini";
    AnsiString sTmp;
    cbRunMode->Items->Clear();
    if(bUse==true)
    {
        sTmp=ReadIniData(sPath, "Lot Info", "Run Mode 2DID",       AnsiString(""));
        cbRunMode->Items->Add("FT1");
        cbRunMode->Items->Add("FT2");
        cbRunMode->Items->Add("FT3");
        cbRunMode->Items->Add("FT4");
        cbRunMode->Items->Add("FT5");
        cbRunMode->Items->Add("FT6");
        cbRunMode->Items->Add("FT7");
        cbRunMode->Items->Add("FT8");
        cbRunMode->Items->Add("FT9");
        cbRunMode->Text="";
    }
    else
    {
        sTmp=ReadIniData(sPath, "Lot Info", "Run Mode",       AnsiString(""));
        cbRunMode->Items->Add("Normal");
        cbRunMode->Items->Add("RT");
        cbRunMode->Items->Add("EQC");
        cbRunMode->Text="Normal";
        cbRunMode->ItemIndex=0;
    }
    for(int i=0; i<cbRunMode->Items->Count; i++)
    {
        if(cbRunMode->Items->Strings[i]==sTmp)
        {
            cbRunMode->Text=sTmp;
            cbRunMode->ItemIndex=i;
            break;
        }
    }
    labCusLotID->Visible=bUse;
    edtCusLotID->Visible=bUse;
}

// -- ShowInformation (golden uLotInfo.cpp:12326-12362) -----------------------
void TfLotInfo::ShowInformation(bool bShow)
{
    if(bShow==true)
    {
        gbFTPAutomation_Download    ->Visible=true;
        gbFTPAutomation_Upload      ->Visible=true;
        sbRecipeUpload              ->Visible=true;
        sbRecipeDownload            ->Visible=true;
        sbFTPAutomationSave         ->Visible=true;

        if(IniConfig.bSIGURDFunction && pgLotinfo->ActivePageIndex==18)
        {
            Height=390;
            Width=475;
            sbTest->Top=304;
            this->Top=205;
            this->Left=340;
        }
    }
    else
    {
        gbFTPAutomation_Download    ->Visible=false;
        gbFTPAutomation_Upload      ->Visible=false;
        sbRecipeUpload              ->Visible=false;
        sbRecipeDownload            ->Visible=false;
        sbFTPAutomationSave         ->Visible=false;

        if(IniConfig.bSIGURDFunction && pgLotinfo->ActivePageIndex==18)
        {
            Height=150;
            Width=150;
            sbTest->Top=50;
            this->Top=592;
            this->Left=217;
        }
    }
}

// -- N23UseLotInfoFile (golden uLotInfo.cpp:7346-7380) -----------------------
bool TfLotInfo::N23UseLotInfoFile()
{
    AnsiString strPath, str4;
    if(IniConfig.bN23UseLotInfoFile)
    {
        strPath=IncludeTrailingPathDelimiter(IniConfig.sN23LotInfoPath)+edtSysLotID->Text+AnsiString(".txt");
        if(FileExists(strPath))
        {
            mmo2DLotInfo->Lines->LoadFromFile(strPath);
            for(int i=0; i<mmo2DLotInfo->Lines->Count; i++)
            {
                str4=mmo2DLotInfo->Lines->Strings[i];
                if(str4.AnsiPos("CUST_LOT_ID:")!=0)
                {
                    edtCusLotID->Text=str4.SubString(str4.AnsiPos(":")+1, str4.Length());
                }
                else if(str4.AnsiPos("FAMILY:")!=0)
                {
                    edtCusDevGrp->Text=str4.SubString(str4.AnsiPos(":")+1, str4.Length());
                }

                if(str4.AnsiPos("CURR_DEVICE:")!=0)
                {
                    edtDevice->Text=str4.SubString(str4.AnsiPos(":")+1, str4.Length());
                }
            }
        }
        else
        {
            ShowMyMessage("The Lot info for 2DID sorting is missing", "找不到2DID sorting用的Lot info");
            return false;
        }
    }
    return true;
}

// -- edTempKeyUp (golden uLotInfo.cpp:4966-4999) -----------------------------
void TfLotInfo::edTempKeyUp()
{
    int iPos;

    iPos=(edTemp->Text.UpperCase()).AnsiPos("/K");
    if(iPos!=0)
    {
        edTemp->Text=edTemp->Text.SubString(1, edTemp->Text.Length()-2);
        edTemp->Text=edTemp->Text+"+";
        edTemp->SelStart=edTemp->Text.Length();
    }

    iPos=(edTemp->Text.UpperCase()).AnsiPos("/O");
    if(iPos!=0)
    {
        edTemp->Text=edTemp->Text.SubString(1, edTemp->Text.Length()-2);
        edTemp->Text=edTemp->Text+"/";
        edTemp->SelStart=edTemp->Text.Length();
    }

    iPos=edTemp->Text.AnsiPos("/0");
    if(iPos!=0)
    {
        edTemp->Text=edTemp->Text.SubString(1, edTemp->Text.Length()-2);
        edTemp->Text=edTemp->Text+"/";
        edTemp->SelStart=edTemp->Text.Length();
    }

    if(bLotFirstKeyIn==false)
    {
        bLotFirstKeyIn=true;
    }
}

// -- edtSysLotIDKeyPress (golden uLotInfo.cpp:11892-11917) -------------------
void TfLotInfo::edtSysLotIDKeyPress(char Key)
{
    if(IniConfig.bO23_InputLotIDByBarcode)
        return;

    if(CUSTOMER_CODE==CC_Murata)
    {
        if(Key=='\r')
        {
            // DEVIATION: golden `edPage->SetFocus();` dropped -- see forms/
            // fLotInfo.h's DEVIATION note above edPage's declaration.
        }
        else if(Key=='$')
        {
            edtSysLotID->Text=edtSysLotID->Text.SubString(1, edtSysLotID->Text.Length()-1);
            // DEVIATION: golden `edPage->SetFocus();` dropped, see above.
        }
    }
    else if(CUSTOMER_CODE==CC_AMD_M && CosFunction.bHiSiliconFunction==true)
    {
        if(bLotID_OK==true)
            bLotID_OK=false;

        if(Key=='\r')
            bLotID_OK=true;
    }
}

// -- SettsChipAdvVisible (golden uLotInfo.cpp:1240-1260) ---------------------
void TfLotInfo::SettsChipAdvVisible()
{
    if(CUSTOMER_CODE==CC_CYUEAN)
    {
        tsChipAdv->TabVisible=true;
    }
    else if(CUSTOMER_CODE==CC_PANTHER ||
            CUSTOMER_CODE==CC_Greatek)
    {
        tsChipAdv->TabVisible=false;
    }
    else
    {
        tsChipAdv->TabVisible=true;
        tsChipAdv->Caption="Lot Info";
    }
}

// -- bCheckOnlyOneFile (golden uLotInfo.cpp:14193-14213) ---------------------
bool TfLotInfo::bCheckOnlyOneFile(AnsiString asPath, AnsiString &asFileName)
{
    TFileListBox *flbStr=new TFileListBox();
    int iCT=-1;
    bool bResult=true;

    flbStr->Directory=asPath;
    flbStr->Mask="*.*";
    flbStr->Refresh();
    flbStr->Update();

    iCT=flbStr->Items->Count;
    if(iCT!=1)
        bResult=false;
    else
        asFileName=flbStr->Items->Strings[0];

    delete flbStr;
    return bResult;
}

// -- edPageKeyPress (golden uLotInfo.cpp:11919-11933) ------------------------
void TfLotInfo::edPageKeyPress(char Key)
{
    if(CUSTOMER_CODE==CC_Murata)
    {
        if(Key=='\r')
        {
            // DEVIATION: golden `edtSysOperatorID->SetFocus();` dropped, see
            // forms/fLotInfo.h's DEVIATION note above edPage's declaration.
        }
        else if(Key=='$')
        {
            edPage->Text=edPage->Text.SubString(1, edPage->Text.Length()-1);
            // DEVIATION: golden `edtSysOperatorID->SetFocus();` dropped, see above.
        }
    }
}

// -- CheckActionFlag (golden uLotInfo.cpp:16234-16246) -----------------------
void TfLotInfo::CheckActionFlag()
{
    ledLoader->Value=bLoaderActionFlag[0];
    ledEmpty->Value=bLoaderActionFlag[1];
    ledColor->Value=bLoaderActionFlag[2];
    ledAuto1->Value=bUnLoaderActionFlag[0];
    ledAuto2->Value=bUnLoaderActionFlag[1];
    ledAuto3->Value=bUnLoaderActionFlag[2];
    ledStartAGV->Value=bAMRReceiveAGVStart;
    ledSTART->Value=bAMRReceiveStart;
    ledLoaderTotalTray->Value=bAMRReceiveLoaderTotalTray;
    ledLOT_START->Value=RunInfo.bLotStart;
}

// -- SetTesterStartTimeByB03 (golden uLotInfo.cpp:14298-14309) ---------------
void TfLotInfo::SetTesterStartTimeByB03()
{
    if(IniConfig.bB03_TesterReport)
    {
        GetTimeInfo();
        AnsiString sTemp="", sTemp1="";
        sTemp=IntToStr(SystemYear)+"/"+IntToStr(SystemMonth)+"/"+IntToStr(SystemDate);
        sB03RunData=sTemp;
        sTemp1=IntToStr(SystemHour)+":"+IntToStr(SystemMin)+":"+IntToStr(SystemSec);
        sB03StartTime=sTemp1;
    }
}

// -- btnLoadFileClick (golden uLotInfo.cpp:14175-14184) ----------------------
void TfLotInfo::btnLoadFileClick()
{
    AnsiString SortFileName;
    OpenDialog1->Title="Open 2DID Sort File";
    if(OpenDialog1->Execute())
    {
        SortFileName=OpenDialog1->FileName;
        edSort2DIDBinFile->Text=SortFileName;
    }
}

// -- cbRunModeKeyDown / cbRunModeKeyUp (golden uLotInfo.cpp:11990-12006) -- WA-7
void TfLotInfo::cbRunModeKeyDown()
{
    // AI(W906-FW3-LotInfo-WA) 20260819: GATE WA-7 -- see forms/fLotInfo.h
    // GATE REGISTER WA-7 (fBarCode->JCETUseMakeWhite2DIDList(), already gated
    // tree-wide at Public/MyProductionRecord.cpp:1198 GATE G-1).
#if 0
    if(fBarCode->JCETUseMakeWhite2DIDList()==true)
    {
        cbRunMode->Text="";
    }
#endif
}
void TfLotInfo::cbRunModeKeyUp()
{
#if 0   // GATE WA-7, see cbRunModeKeyDown above
    if(fBarCode->JCETUseMakeWhite2DIDList()==true)
    {
        cbRunMode->Text="";
    }
#endif
}

// -- edDeviceNameKeyUp (golden uLotInfo.cpp:12044-12051) ---------------------
void TfLotInfo::edDeviceNameKeyUp()
{
    if(bLotFirstKeyIn==false)
    {
        bLotFirstKeyIn=true;
    }
}

// -- ReflashInfo (golden uLotInfo.cpp:16479-16485) ---------------------------
void TfLotInfo::ReflashInfo()
{
    labLoaderTrayCount->Caption=iLoaderTrayCountCal;
    labAuto1TrayCount_KYEC->Caption=iUnloaderTrayCountCal[0];
    labAuto2TrayCount_KYEC->Caption=iUnloaderTrayCountCal[1];
    labAuto3TrayCount_KYEC->Caption=iUnloaderTrayCountCal[2];
}

// -- labLotIDMouseDown (golden uLotInfo.cpp:8423-8428) -- WA-8 --------------
void TfLotInfo::labLotIDMouseDown()
{
    if(CUSTOMER_CODE==CC_KYEC_LEE && AccessLevel==iDefHonPrecLevel)
    {
        // AI(W906-FW3-LotInfo-WA) 20260819: GATE WA-8 -- see forms/fLotInfo.h
        // GATE REGISTER WA-8 (fQwertyKey has no port anywhere in this tree).
#if 0
        fQwertyKey->ShowQwertyKey(edtSysLotID, N_NO_SYMBOL|N_NO_SPACE);
#endif
    }
}

// -- edtLotVerifyMouseDown / edtLotVerifyKeyPress (golden :14515-14526) ------
void TfLotInfo::edtLotVerifyMouseDown()
{
    edtLotVerify->Text=TestIF_File.sLotIDSubstr;
}
void TfLotInfo::edtLotVerifyKeyPress()
{
    edtLotVerify->Text=TestIF_File.sLotIDSubstr;
}

// -- btnCancelTestPauseClick (golden uLotInfo.cpp:12297-12301) ---------------
void TfLotInfo::btnCancelTestPauseClick()
{
    bTesterSendPause=false;
    bTesterPauseMusic=false;
}

// -- BtnPauseMouseDown / BtnPauseMouseUp (golden :14533-14543) ---------------
void TfLotInfo::BtnPauseMouseDown()
{
    BtnPause->Down=true;
}
void TfLotInfo::BtnPauseMouseUp()
{
    BtnPause->Down=false;
}

// -- VisibleUploadBtnPAT (golden uLotInfo.cpp:15710-15714) -------------------
void TfLotInfo::VisibleUploadBtnPAT(bool bVisible)
{
    sbUploadPAT->Down=!bVisible;
    sbUploadPAT->Visible=bVisible;
}

// -- cbbDeviceNameChange (golden uLotInfo.cpp:7216-7219) ---------------------
void TfLotInfo::cbbDeviceNameChange()
{
    edDeviceName->Text=cbbDeviceName->Text;
}

// -- btStartCountClick (golden uLotInfo.cpp:8418-8421) -----------------------
void TfLotInfo::btStartCountClick()
{
    bStartCount_SCK=true;
}

// -- edtASECL_LotIDClick (golden uLotInfo.cpp:10491-10494) -- WA-8 ----------
void TfLotInfo::edtASECL_LotIDClick()
{
    // AI(W906-FW3-LotInfo-WA) 20260819: GATE WA-8, see labLotIDMouseDown above.
#if 0
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL|N_NO_SPACE);
#endif
}

// -- btTesterTCPShowClick (golden uLotInfo.cpp:13841-13844) -- WA-9 --------
void TfLotInfo::btTesterTCPShowClick()
{
    // AI(W906-FW3-LotInfo-WA) 20260819: GATE WA-9 -- see forms/fLotInfo.h
    // GATE REGISTER WA-9 (fTesterTCP has no port anywhere in this tree --
    // Interface/TesterTCP.h's own banner: "NOT a TfTesterTCP class or facade
    // at all, only free functions").
#if 0
    fTesterTCP->Show();
#endif
}

// -- InitialLoaderTask / InitialLDLevelTask (golden :16259-16267) -----------
void TfLotInfo::InitialLoaderTask(int iPos)
{
    iLoaderTask[iPos]=1;
}
void TfLotInfo::InitialLDLevelTask(int iPos)
{
    iloaderLevelTask[iPos]=1;
}

TfLotInfo *fLotInfo = new TfLotInfo();
