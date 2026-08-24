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
#include "forms/fSetup.h"       // AI(W906-FW3-Setup-WA) 20260820: fSetup->edOcrText (WC-27 retired)

// AI(W906-FW3-LotInfo-WB) 20260819: Wave B includes -- see forms/fLotInfo.h's
// WB banner for the per-dependency existence citations gathered this wave.
#include "Automation/AMR.h"      // real TTeraPowerAMR AMR (RefreshAMR: CheckLoaderCount/CheckUnloaderCount)
#include "SortingBinTray/SortingBinTray.h"  // SaveTrayRecord (btnSaveDataClick)

// AI(W906-FW3-LotInfo-WC) 20260819: Wave C include -- fAGV->IsSPIL_AMR()
// (Timer2Timer T4, confirmed REAL by this wave's step 0).
#include "forms/fAGV.h"
#include "forms/fQwertyKey.h"  // AI(W906-FW-QWKEY2) 20260824: fQwertyKey extern for un-gated ShowQwertyKey sites (real since FW-QWKEY1 fc08e09; latent until HTEdit GATE (6) wiring)

// AI(W906-FW3-LotInfo-WB) 20260819: TU-local forward decl of MyDBIProcess
// (FormDestroy's exception log). Its only declaration in this tree is
// aHotPlateSubstrate.h:933, but that header transitively drags in
// Motor/mymotor.h/HTMotor.h (a large unrelated surface) for one extern
// function -- forward-declaring here instead, matching the SAME "TU-local
// copy instead of a shared heavy header" idiom this file already uses for
// IncludeTrailingPathDelimiter above and the TColor constants below. Real
// definition (linked from wherever aHotPlateSubstrate.cpp/its owner TU is)
// is unaffected; signature copied verbatim.
extern void MyDBIProcess(AnsiString S1, AnsiString S2);

// AI(W906-FW3-LotInfo-WB) 20260819: TU-local TColor constants -- the SAME
// "every TU that needs this carries an identical local copy" idiom this
// file's own IncludeTrailingPathDelimiter (above) and, tree-wide,
// cObserver.cpp/cShowBinSelect.cpp/Interface/TesterTCP_Socket.cpp etc. all
// already use for these exact 4 colours (TColor itself is `typedef int` from
// cmydef.h, already included). Values copied verbatim from
// vclcompat/LedCore.h:54-59 (clGreen/clRed/clLime) and ATC/ATCInterface.h:181
// (clGray) -- not #include'd directly to avoid a duplicate-const-definition
// ODR risk if some other already-included header also brings one in.
static const TColor clGreen = 0x00008000;
static const TColor clRed   = 0x000000FF;
static const TColor clLime  = 0x0000FF00;
static const TColor clGray  = 0x00808080;

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
// AI(W906-FW3-LotInfo-WB) 20260819: 2 more file-scope globals from the SAME
// golden top-of-file cluster as bLotID_OK/bLotFirstKeyIn above (golden
// uLotInfo.cpp:92-94), needed by this wave's edtSysOperatorIDKeyDown/
// edTempKeyPress/edTempKeyDown/edDeviceNameKeyDown.
bool bOPID_OK=false;                  // golden uLotInfo.cpp:92
bool bTemp_OK=false;                  // golden uLotInfo.cpp:93
bool bDeviceName_OK=false;            // golden uLotInfo.cpp:94

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

    // =======================================================================
    //  AI(W906-FW3-LotInfo-WB) 20260819: Wave B ctor ADD -- see forms/
    //  fLotInfo.h WB-19 for the gated named-widget-array wiring block this
    //  section deliberately omits. Allocations first, then the REAL golden
    //  logic in golden's own order (golden uLotInfo.cpp:157-299).
    // =======================================================================
    lblOCR_LotID = new TLabel();
    slASECLTestInfor = new TStringList();
    TimerERMS = new TfLotInfoTimer();
    Timer3    = new TfLotInfoTimer();
    tsChamberBoost = new TTabSheet();
    btnCancelTestPause = new TButton();
    btnESCFunction     = new TButton();
    ts_FTPAutomation   = new TTabSheet();
    ATC_WinWay         = new TTabSheet();
    pnlWinwayPVCH1 = new TPanel(); pnlWinwayPVCH2 = new TPanel();
    pnlWinwayPVCH3 = new TPanel(); pnlWinwayPVCH4 = new TPanel();
    lbOCRUseFile      = new TLabel();
    spOCRChangeFile   = new TSpeedButton();
    lbCheckCodeByLot  = new TPanel();
    sbSECSLotStart = new TSpeedButton(); sbSECSLotEnd = new TSpeedButton();
    labRefrigerantMachineHighLimit = new TLabel();
    labRefrigerantMachineLowLimit  = new TLabel();
    leRunCardNumber = new TLabeledEdit();
    cbA60_1 = new TCheckBox();
    pnlWaitTXSetLoader = new TPanel(); pnlWaitRXSetAuto1 = new TPanel();
    pnlWaitRXSetAuto2  = new TPanel(); pnlWaitRXSetAuto3 = new TPanel();
    pnlWaitTXTotalLoader = new TPanel(); pnlWaitTXCntLoader = new TPanel();
    pnlWaitRXCntAuto1 = new TPanel(); pnlWaitRXCntAuto2 = new TPanel(); pnlWaitRXCntAuto3 = new TPanel();
    aldWaitTXLoader = new TfLedValue();
    aldWaitRXAuto1 = new TfLedValue(); aldWaitRXAuto2 = new TfLedValue(); aldWaitRXAuto3 = new TfLedValue();
    aldWaitTrayFeed = new TfLedValue();
    aldLoaderLast   = new TfLedValue();
    // AI(W906-FW3-LotInfo-WBfix) 20260819: hydrate to golden uLotInfo.dfm dims
    // (ColCount=3, RowCount=16) -- the bare 5x5 default let ShowAMRCategoryBin's
    // non-initial path (Cells[1][1+i], rows up to iTestBinCount) run off row 5
    // before anything called the bInitial=true resize: caught by the AMR test
    // (TTeraPowerAMR::Initial -> RefreshAMR) in the Wave B gate, std::out_of_range
    // n=5 size=5. Same defect class and same fix shape as cObserver's
    // sgStatisticsJam (Obs2fix).
    StrGrdCategory  = new TStringGrid(3, 16);
    edLoaderCountNow = new TEdit(); edLoaderCountAlarm = new TEdit();
    bP60UserClicked = false;   // golden uLotInfo.h:1262 -- NSDMI-style false default, same idiom as bStartCount_SCK above
    // -- pgLotinfoChange: 20 TCheckBox widgets --
    chkTempOffset = new TCheckBox(); chkContactHigh = new TCheckBox();
    chkContactForce = new TCheckBox(); chkContactMode = new TCheckBox();
    chkHotPlate = new TCheckBox(); chkLoadUnload = new TCheckBox();
    chkSpeedSetting = new TCheckBox(); chkShuttleMode = new TCheckBox();
    chkTestMode = new TCheckBox(); chkBinasgn = new TCheckBox();
    chkBinasgnOff = new TCheckBox(); checkbAutoClean = new TCheckBox();
    chkAutoCleanContactHeight = new TCheckBox(); cbBottom2DOffset = new TCheckBox();
    chkART = new TCheckBox(); chkART_RTCount = new TCheckBox();
    chkIndexHeatingMode = new TCheckBox(); chkStopYield = new TCheckBox();
    chkConsecutiveFailure = new TCheckBox(); chkCleanCount = new TCheckBox();

    // -- REAL golden ctor logic (golden :160-299, minus the WB-19 gated span) --
    bShow=false;
    lblOCR_LotID->Caption="";
    iLotRead  =0;
    iLotStart =1;
    iLotEnd   =2;
    bStartChamberBoost=false;
    iXMLOnLineStatus=0;
    bRTCChangeFileFinish=false;
    // golden :169 `bStartCount_SCK=false;` -- already set above by the
    // pre-existing Wave A ctor tail; not re-set here (same net value).
    sVTestInternalLot="";

    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-19 -- golden :172-269, the
    // ESD_DataPtr[18]/ESD_DECAY_DATA_Ptr[36]/ATCChPal[32]/ATCReferPtr[32]/
    // ATCPtr[32]/edSocket[4][8]/SocketSiteCH_Display[][]/
    // SocketLabCol_Display[]/SocketLabRow_Display[] named-widget wiring
    // block. See forms/fLotInfo.h GATE REGISTER WB-19.
#if 0
    ESD_DataPtr[ 0]=pl_ESDProx1_1; ESD_DataPtr[ 1]=pl_ESDProx1_2; ESD_DataPtr[ 2]=pl_ESDProx1_3;
    ESD_DataPtr[ 3]=pl_ESDProx2_1; ESD_DataPtr[ 4]=pl_ESDProx2_2; ESD_DataPtr[ 5]=pl_ESDProx2_3;
    ESD_DataPtr[ 6]=pl_ESDProx3_1; ESD_DataPtr[ 7]=pl_ESDProx3_2; ESD_DataPtr[ 8]=pl_ESDProx3_3;
    ESD_DataPtr[ 9]=pl_ESDProx4_1; ESD_DataPtr[10]=pl_ESDProx4_2; ESD_DataPtr[11]=pl_ESDProx4_3;
    ESD_DataPtr[12]=pl_ESDProx5_1; ESD_DataPtr[13]=pl_ESDProx5_2; ESD_DataPtr[14]=pl_ESDProx5_3;
    ESD_DataPtr[15]=pl_ESDProx6_1; ESD_DataPtr[16]=pl_ESDProx6_2; ESD_DataPtr[17]=pl_ESDProx6_3;

    ESD_DECAY_DATA_Ptr[ 0]=pl_PositiveDecay1_1; ESD_DECAY_DATA_Ptr[ 1]=pl_PositiveDecay1_2; ESD_DECAY_DATA_Ptr[ 2]=pl_PositiveDecay1_3;
    ESD_DECAY_DATA_Ptr[ 3]=pl_PositiveDecay2_1; ESD_DECAY_DATA_Ptr[ 4]=pl_PositiveDecay2_2; ESD_DECAY_DATA_Ptr[ 5]=pl_PositiveDecay2_3;
    ESD_DECAY_DATA_Ptr[ 6]=pl_PositiveDecay3_1; ESD_DECAY_DATA_Ptr[ 7]=pl_PositiveDecay3_2; ESD_DECAY_DATA_Ptr[ 8]=pl_PositiveDecay3_3;
    ESD_DECAY_DATA_Ptr[ 9]=pl_PositiveDecay4_1; ESD_DECAY_DATA_Ptr[10]=pl_PositiveDecay4_2; ESD_DECAY_DATA_Ptr[11]=pl_PositiveDecay4_3;
    ESD_DECAY_DATA_Ptr[12]=pl_PositiveDecay5_1; ESD_DECAY_DATA_Ptr[13]=pl_PositiveDecay5_2; ESD_DECAY_DATA_Ptr[14]=pl_PositiveDecay5_3;
    ESD_DECAY_DATA_Ptr[15]=pl_PositiveDecay6_1; ESD_DECAY_DATA_Ptr[16]=pl_PositiveDecay6_2; ESD_DECAY_DATA_Ptr[17]=pl_PositiveDecay6_3;

    ESD_DECAY_DATA_Ptr[18]=pl_NegativeDecay1_1; ESD_DECAY_DATA_Ptr[19]=pl_NegativeDecay1_2; ESD_DECAY_DATA_Ptr[20]=pl_NegativeDecay1_3;
    ESD_DECAY_DATA_Ptr[21]=pl_NegativeDecay2_1; ESD_DECAY_DATA_Ptr[22]=pl_NegativeDecay2_2; ESD_DECAY_DATA_Ptr[23]=pl_NegativeDecay2_3;
    ESD_DECAY_DATA_Ptr[24]=pl_NegativeDecay3_1; ESD_DECAY_DATA_Ptr[25]=pl_NegativeDecay3_2; ESD_DECAY_DATA_Ptr[26]=pl_NegativeDecay3_3;
    ESD_DECAY_DATA_Ptr[27]=pl_NegativeDecay4_1; ESD_DECAY_DATA_Ptr[28]=pl_NegativeDecay4_2; ESD_DECAY_DATA_Ptr[29]=pl_NegativeDecay4_3;
    ESD_DECAY_DATA_Ptr[30]=pl_NegativeDecay5_1; ESD_DECAY_DATA_Ptr[31]=pl_NegativeDecay5_2; ESD_DECAY_DATA_Ptr[32]=pl_NegativeDecay5_3;
    ESD_DECAY_DATA_Ptr[33]=pl_NegativeDecay6_1; ESD_DECAY_DATA_Ptr[34]=pl_NegativeDecay6_2; ESD_DECAY_DATA_Ptr[35]=pl_NegativeDecay6_3;

    ATCChPal[ 0]=pan_ATCTempHead01; /* ... 32 total, see golden :193-200 ... */
    ATCReferPtr[ 0]=pl_ATCRefHead01; /* ... 32 total, see golden :202-209 ... */
    ATCPtr[ 0]=pl_ATCTempHead01; /* ... 32 total, see golden :211-218 ... */

    edSocket[0][0]=edtSocketAa; /* ... 32 total, see golden :222-229 ... */
    slASECLTestInfor = new TStringList;

    TPanel *tempTestSiteCBox_Dis[MAX_SOCKET_ROW][MAX_SOCKET_COL]=
    {
        {palAa, palAb, palAc, palAd, palAe, palAf, palAg, palAh},
        {palBa, palBb, palBc, palBd, palBe, palBf, palBg, palBh},
        {palCa, palCb, palCc, palCd, palCe, palCf, palCg, palCh},
        {palDa, palDb, palDc, palDd, palDe, palDf, palDg, palDh}
    };
    TLabel *tempTestLabCol_Dis[MAX_SOCKET_COL]=
    {
        lbSocketIDColA, lbSocketIDColB, lbSocketIDColC, lbSocketIDColD, lbSocketIDColE, lbSocketIDColF, lbSocketIDColG, lbSocketIDColH
    };
    TLabel *tempTestLabRow_Dis[MAX_SOCKET_ROW]={lbSocketIDRowA, lbSocketIDRowB, lbSocketIDRowC, lbSocketIDRowD};

    int iCol, iRow;
    for(int i=0; i<MAX_SOCKET_TOTAL; i++)
    {
        iRow=i/MAX_SOCKET_COL;
        iCol=i%MAX_SOCKET_COL;

        if(i<MAX_SOCKET_ROW)
        {
            SocketLabRow_Display[i]=tempTestLabRow_Dis[i];
            SocketLabRow_Display[i]->Visible=false;
        }

        if(i<MAX_SOCKET_COL)
        {
            SocketLabCol_Display[i]=tempTestLabCol_Dis[i];
            SocketLabCol_Display[i]->Visible=false;
        }
        SocketSiteCH_Display[iRow][iCol]=tempTestSiteCBox_Dis[iRow][iCol];
        SocketSiteCH_Display[iRow][iCol]->Visible=false;
    }
#endif

    TimerERMS->Enabled=true;
    tsChamberBoost->TabVisible=false;
    btnCancelTestPause->Enabled=false;
    btnESCFunction->Enabled=(IniConfig.bI41_6_Manual);
    if(IniConfig.bSIGURDFunction)                                               //Sam 20210401 : 俊堯要求只要顯示 FTP
        ts_FTPAutomation->Caption="FTP";
    if(ATC_SYSTEM==eWinWay && Temperature.bATCActiveCooling==true) ATC_WinWay->TabVisible=true;
    else ATC_WinWay->TabVisible=false;
    ATCPtrWinWay[0]=pnlWinwayPVCH1;ATCPtrWinWay[1]=pnlWinwayPVCH2;ATCPtrWinWay[2]=pnlWinwayPVCH3;ATCPtrWinWay[3]=pnlWinwayPVCH4;

    bNeedToDeleteFile=false;
    iWaitRtcDeleteTask=0;
    bEventLogAlarm=false;
    ZeroMemory(fTempUserOffset,sizeof(fTempUserOffset));
    ZeroMemory(fContactHeight, sizeof(fContactHeight));
    ZeroMemory(fTempATCOffset, sizeof(fTempATCOffset));
    ZeroMemory(iShuttleMode, sizeof(iShuttleMode));
    ZeroMemory(bART, sizeof(bART));
    iART=0;
    iIndexHeatingMode=0;
    iProduceTimeCT=0;                                                           //jou 20221125 : 機台添加三小時送檢報警，從lot start時間開始計算

    // AI(W906-FW3-LotInfo-WC) 20260819: Wave C ADD -- FormShow/Timer2Timer
    // widget allocations. See forms/fLotInfo.h's WC banner for WAVE SCOPE /
    // GATE REGISTER; grouped in the same order as the header declarations.
    tsPATSetUp        = new TTabSheet();
    tsBundle          = new TTabSheet();
    tsSetupFileCheck  = new TTabSheet();
    tsESDMonitor      = new TTabSheet();
    tsBarCode         = new TTabSheet();
    ts_OCRInterface   = new TTabSheet();
    ts_SocketInterface= new TTabSheet();
    tsKYEC_AMR        = new TTabSheet();
    tsRTCFullViewImg  = new TTabSheet();

    grpRFID           = new TGroupBox();
    ts_AutoCleanMonitor = new TTabSheet();
    Label5            = new TLabel();
    btDownload        = new TButton();
    btnFtpTester      = new TButton();
    Label153          = new TLabel();
    FileListBox1      = new vclcompat::TFileListBox();
    cbRTCASTD         = new TCheckBox();

    ART_Panel         = new TfLotInfoLayoutPanel();
    GroupBox3         = new TfLotInfoLayoutGroupBox();

    btnDataFTPSaveToData = new TButton();
    lblOPID           = new TLabel();
    spSECSLotCheck    = new TSpeedButton();
    Panel6            = new TfLotInfoLayoutPanel();

    tsTPW             = new TTabSheet();
    tsSigurd          = new TTabSheet();

    labLevelMode      = new TLabel();
    coLevelMode       = new TComboBox();

    sgBarcode         = new TStringGrid(6, 7);        // dfm: ColCount=6 RowCount=7
    sgOCR             = new TStringGrid(3, 5);        // dfm: ColCount=3, RowCount default(5)
    sgATRCount        = new TStringGrid(3, 4);        // dfm: ColCount=3 RowCount=4

    pl_ATC_Online     = new TPanel();

    labDeviceName     = new TLabel();
    lbLotRunMode      = new TLabel();
    labLotID          = new TLabel();
    btnFtpServer      = new TButton();
    btnFtpHD          = new TButton();
    sb_RunExecutFile  = new TSpeedButton();

    edtLine           = new TLabeledEdit();
    edtProcessName    = new TfLotInfoLayoutLabeledEdit();
    edtProduct        = new TfLotInfoLayoutLabeledEdit();
    lbProcess         = new TLabel();
    labConfigL04      = new TLabel();
    grpOEEState       = new TfLotInfoLayoutGroupBox();
    sgOEEState        = new TStringGrid(2, 10);       // dfm: ColCount=2 RowCount=10
    labCusDevGrp      = new TLabel();
    labCusStep        = new TLabel();

    btnFTPTryConnect  = new TButton();
    lbFTPStatus       = new TLabel();

    tsASEMARMS        = new TTabSheet();
    pnlLotInfo_ASECL  = new TPanel();
    pnlLotStart_ASECL = new TPanel();

    lblPage           = new TLabel();
    labJobSeq         = new TLabel();
    edtJobSeq         = new TEdit();
    labQACount        = new TLabel();
    edQAMode          = new TfLotInfoLayoutEdit();
    btnQAmodeSave     = new TfLotInfoLayoutPushButton();
    palQAMode         = new TPanel();

    pan_DewPoint      = new TPanel();
    pl_DewPoint       = new TPanel();

    grpBarcodeDisplayLotInfo = new TGroupBox();

    btChangeFile      = new TButton();

    btnClearTemperature = new TButton();
    lbShowDevName     = new TLabel();

    lbLotAQLSetCount  = new TLabel();
    lbLotAQLSetBin    = new TLabel();

    tsDeviceInfo      = new TTabSheet();
    palSecsGem        = new TPanel();
    cbPATMode         = new TComboBox();

    palCurrFailRate   = new TPanel();
    Panel27           = new TPanel();
    LotKeyInTime      = new TfLotInfoTimer();
    btnSaveData       = new TSpeedButton();

    labLoaderBundleID     = new TLabel();
    lblLoaderCarBundleID  = new TLabel();
    lbOCRNowFile      = new TLabel();
    Label41           = new TLabel();
    palHandlerwithTester = new TPanel();
    lblTester_LotID   = new TLabel();
    lblAutoCount      = new TLabel();
    lblAutoCount2     = new TLabel();
    lblAutoCount3     = new TLabel();
    labBarcodeRecipe  = new TLabel();
    edtBarcodeRecipe  = new TEdit();
    spOCRCleanList    = new TSpeedButton();
    tsTesterLog       = new TTabSheet();
    ts_AutoRetestMonitor = new TTabSheet();
    tsOtherTool       = new TTabSheet();
    palAQLMode        = new TPanel();
}
// AI(W906-AutoCleanFoundation) 20260721: golden uLotInfo.cpp:16250-16253 --
// REAL one-line body (was a total no-op stub before that wave). See
// forms/fLotInfo.h's iUnloaderTask/InitialUnLoaderTask member comments for the
// behaviour-change + dormant-call-site (SOFT_SIMULTE undefined) analysis.
void TfLotInfo::InitialUnLoaderTask(int iPos) { iUnloaderTask[iPos]=1; }
// -- W5-Automation ADD: AMR.cpp + HANA_ART.cpp method sinks --
// AI(W906-FW3-LotInfo-WB) 20260819: RefreshAMR was a total no-op stub before
// this wave (golden uLotInfo.cpp:15816-15844 is RECON Tier 2 item, never
// translated). Given a REAL body below (see Wave B method bodies section);
// this comment stays here as the historical marker for where the stub used
// to live.
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
        // AI(W906-FW3-LotInfo-WA) 20260819: GATE WA-8 (OPENED 20260824) -- see forms/fLotInfo.h
        // GATE WA-8 OPENED 20260824 (FW-QWKEY2): fQwertyKey real since FW-QWKEY1 (fc08e09).
        fQwertyKey->ShowQwertyKey(edtSysLotID, N_NO_SYMBOL|N_NO_SPACE);
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
void TfLotInfo::edtASECL_LotIDClick(TObject *Sender)
{
    // AI(W906-FW3-LotInfo-WA) 20260819: GATE WA-8 (OPENED 20260824), see labLotIDMouseDown above.
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL|N_NO_SPACE);
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

// ===========================================================================
//  AI(W906-FW3-LotInfo-WB) 20260819: Wave B method bodies. See forms/
//  fLotInfo.h's file banner for WAVE SCOPE / GATE REGISTER.
// ===========================================================================

// -- FormDestroy (golden uLotInfo.cpp:301-314) -- RECON MISS, see banner ----
void TfLotInfo::FormDestroy()
{
    try
    {
        TimerERMS->Enabled=false;
        slASECLTestInfor->Clear();
        delete slASECLTestInfor;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfLotInfo::FormDestroy");
    }
    LogSoftwareOffTime("TfLotInfo, FormDestroy");                               //Steven 20210526 : 紀錄軟體執行時間
}

// -- FormClose (golden uLotInfo.cpp:1235-1238) -------------------------------
void TfLotInfo::FormClose()
{
    bShow=false;
}

// -- pgLotinfoChange (golden uLotInfo.cpp:7221-7266) -------------------------
void TfLotInfo::pgLotinfoChange()
{
    AnsiString sConfigPath=AuthPath+"Security_new.def";
    if(pgLotinfo->ActivePage==tsSelection)
    {
        chkTempOffset->Checked  =CheckAndReadIniData(sConfigPath, "Network", "Temp Offset",     true);
        chkContactHigh->Checked =CheckAndReadIniData(sConfigPath, "Network", "Contact High",    false);
        chkContactForce->Checked=CheckAndReadIniData(sConfigPath, "Network", "Contact Force",   true);
        chkContactMode->Checked =CheckAndReadIniData(sConfigPath, "Network", "Contact Mode",    true);
        chkHotPlate->Checked    =CheckAndReadIniData(sConfigPath, "Network", "HotPlate",        false);
        chkLoadUnload->Checked  =CheckAndReadIniData(sConfigPath, "Network", "Load Unload",     false);
        chkSpeedSetting->Checked=CheckAndReadIniData(sConfigPath, "Network", "Speed Setting",   true);
        chkShuttleMode->Checked =CheckAndReadIniData(sConfigPath, "Network", "Shuttle Mode",    false);

        chkTestMode->Checked    =CheckAndReadIniData(sConfigPath, "Network", "Test Mode",       true);
        chkBinasgn->Checked     =CheckAndReadIniData(sConfigPath, "Network", "Binasgn",         true);
        chkBinasgnOff->Checked  =CheckAndReadIniData(sConfigPath, "Network", "BinasgnOff",      true);

        checkbAutoClean->Checked=CheckAndReadIniData(sConfigPath, "Network", "Auto Clean",      true);                  //jou 20161122 Auto Clean 參數可以選擇是否需要上傳下載

        chkAutoCleanContactHeight->Checked=CheckAndReadIniData(sConfigPath, "Network", "Auto Clean Contact Height",      false);                                //JerryYang 20241019 : 矽品二林 耀仁要求Auto clean高度可選擇不覆蓋

        cbBottom2DOffset->Checked =CheckAndReadIniData(sConfigPath, "Network", "Bottom 2D Offset",    false);           //JerryYang 20201122 Bottom 2D offset不覆蓋

        chkART->Visible=(CosFunction.bUseSCKART);
        chkART_RTCount->Visible=(CosFunction.bUseSCKART);
        if(CosFunction.bUseSCKART)
        {
            chkART->Checked     =CheckAndReadIniData(sConfigPath, "Network", "Auto Retest",     true);                  //Steven 20190918 : ART設定下載不覆蓋
            chkART_RTCount->Checked=CheckAndReadIniData(sConfigPath, "Network", "ART_RT_Count ", true);                 //Steven 20191101 : ART RT count不覆蓋
        }
        chkIndexHeatingMode->Checked=CheckAndReadIniData(sConfigPath, "Network", "Index Heat Mode", true);              //Steven 20180420 (Jou) : JCET吳如春說不覆蓋Index加熱模式

        if(IniConfig.bVTESTFunction==true)                                      //RogerYang 20250314 偉測張冬冬要求 Stop Yield和Consecutive Fail功能修改為灰色,不可更改,默認開啟
        {
            chkStopYield->Checked           =true;
            chkConsecutiveFailure->Checked  =true;
            chkStopYield->Enabled           =false;
            chkConsecutiveFailure->Enabled  =false;
        }

        chkCleanCount->Checked=CheckAndReadIniData(sConfigPath, "Network", "Cleaning Count", checkbAutoClean->Checked);                                         //KenHsieh 20230518 : Auto Clean count不覆蓋
    }

    AdjtsYieldMonitiorSize();                                                   //Steven 20221225 : 統一Lot Info尺寸調整
}

// -- LoadRTCFullViewImg (golden uLotInfo.cpp:5366-5448) -- WB-1 -------------
void TfLotInfo::LoadRTCFullViewImg(bool /*bShowImage*/)
{
    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-1 -- see forms/fLotInfo.h
    // GATE REGISTER WB-1 (TImage/TCanvas have no port anywhere in this tree).
#if 0
    imgRTCFullView1->Visible=true;
    imgRTCFullView2->Visible=true;
    imgRTCFullView3->Visible=true;
    imgRTCFullView4->Visible=true;

    if(bShowImage)
    {
        if(MachineTypeChoice==Type_HT9046_LS)
        {
            if(FileExists(FULLVIEWIMAGEPATH1))
                imgRTCFullView1->Picture->LoadFromFile(FULLVIEWIMAGEPATH1);
            else if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView1->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView1->Visible=false;

            if(FileExists(FULLVIEWIMAGEPATH4))
                imgRTCFullView2->Picture->LoadFromFile(FULLVIEWIMAGEPATH4);
            else if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView2->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView2->Visible=false;

            if(FileExists(FULLVIEWIMAGEPATH2))
                imgRTCFullView3->Picture->LoadFromFile(FULLVIEWIMAGEPATH2);
            else if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView3->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView3->Visible=false;

            if(FileExists(FULLVIEWIMAGEPATH5))
                imgRTCFullView4->Picture->LoadFromFile(FULLVIEWIMAGEPATH5);
            else if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView4->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView4->Visible=false;
        }
        else
        {
            if(FileExists(FULLVIEWIMAGEPATH1))
                imgRTCFullView1->Picture->LoadFromFile(FULLVIEWIMAGEPATH1);
            else if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView1->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView1->Visible=false;

            if(FileExists(FULLVIEWIMAGEPATH2))
                imgRTCFullView2->Picture->LoadFromFile(FULLVIEWIMAGEPATH2);
            else if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView2->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView2->Visible=false;
        }
    }
    else
    {
        if(FileExists(FULLVIEWIMAGEPATH3))
            imgRTCFullView1->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
        else
            imgRTCFullView1->Visible=false;

        if(FileExists(FULLVIEWIMAGEPATH3))
            imgRTCFullView2->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
        else
            imgRTCFullView2->Visible=false;

        if(MachineTypeChoice==Type_HT9046_LS)
        {
            if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView3->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView3->Visible=false;

            if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView4->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView4->Visible=false;
        }
    }
#endif
}

// -- edDeviceNameMouseDown (golden uLotInfo.cpp:7193-7214) -- WB-2 ----------
void TfLotInfo::edDeviceNameMouseDown()
{
    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-2 -- see forms/fLotInfo.h
    // GATE REGISTER WB-2 (Clipboard()/InputBarcodeNumber/TMouseButton have no
    // port anywhere in this tree).
#if 0
#ifndef SOFT_SIMULTE
    if(CUSTOMER_CODE==CC_AMKOR_China ||                                     //jou 2013-01-04 防止OP使用複製貼上的方式讀取工作檔
       CUSTOMER_CODE==CC_QUALCOMM)                                          //JerryYang 20170412 (Steven) add QUALCOMM
    {
        if(Button==mbRight || Button==mbLeft)
        {
            Clipboard()->Clear();
        }
    }
#endif
    if((CUSTOMER_CODE==CC_SCC && AccessLevel<iDefHonPrecLevel))
    {
        edDeviceName->Text="";
        Clipboard()->Clear();
        AnsiString sBarcodeID=InputBarcodeNumber("Input Device Name:");
        edDeviceName->Text=sBarcodeID;
    }
#endif
}

// -- CutTempToEdit (golden uLotInfo.cpp:5225-5244) ---------------------------
void TfLotInfo::CutTempToEdit(AnsiString asString)
{
    int iPos1=0,iPos2=0,iTemp=25;
    AnsiString asBuffer1,asBuffer2;
    if(CUSTOMER_CODE==CC_SCC ||
       CUSTOMER_CODE==CC_SCK)                                                   //ChungHung 20130621 add SCK RMS
    {
        iPos1=asString.Pos("_")+1;
        asBuffer1=asString.SubString(iPos1,asString.Length());
        iPos2=asBuffer1.Pos("_")+1;
        asBuffer2=asBuffer1.SubString(iPos2,asBuffer1.Length());

        iTemp=atoi(asBuffer2.c_str());
        if(iTemp>140)
            iTemp=140;
        if(iTemp<25)
            iTemp=25;
        edTemp->Text=iTemp;
    }
}

// -- btChangeFileClick (golden uLotInfo.cpp:10213-10237) -- WB-3 -----------
void TfLotInfo::btChangeFileClick()
{
    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-3 -- see forms/fLotInfo.h
    // GATE REGISTER WB-3 (fBarCode has SOME port but none of these 4 members).
#if 0
    if(TestIF_File.bEnableBarCode)
    {
        if(BAR_CODE_INSTALL==ebctInShtIntel)
        {
            fBarCode->btBarcodeChangeFileDisConnect->Click();                   //wei 20160728 Barcode File切換
            fBarCode->btBarcodeChangeFileConnect->Click();
        }
        else if(BAR_CODE_INSTALL==ebctEtherNetCCD)
        {
            bBarcodeConnect=true;
            fBarCode->InitialBarcodeScanChangeFile();
            fBarCode->TimerBarcodeChangeFile->Enabled=true;
        }
        else if(BAR_CODE_INSTALL==ebctUseCCDMode &&
                CosFunction.b2DUseSubJobFunction==true &&
                TestIF_File.b2DUseSubJob==true)
        {
            bBarcodeConnect=true;
            fBarCode->InitialBarcodeScanChangeFile();
            fBarCode->TimerBarcodeChangeFile->Enabled=true;
        }
    }
#endif
}

// -- edtSysOperatorIDKeyUp (golden uLotInfo.cpp:10287-10312) -- WB-4 -------
void TfLotInfo::edtSysOperatorIDKeyUp()
{
    if(CUSTOMER_CODE==CC_Murata)
    {
        return;
    }

    if(bLotFirstKeyIn==false)                                                   //Ifor 20190924 : add Barcode 輸入判斷避免讀碼失敗
    {
        bLotFirstKeyIn=true;
    }

    if(IniConfig.bO23_InputLotIDByBarcode)                                      //Steven 20241224 : LotID只能用Barcode
    {
        // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-4 -- see forms/fLotInfo.h
        // GATE REGISTER WB-4 (InputBarcodeNumber has no port anywhere in
        // this tree).
#if 0
        edtSysOperatorID->Text=InputBarcodeNumber("Input OP ID:", "UserName");
#endif
    }
    else if(CUSTOMER_CODE==CC_TFME_CHINA ||                                     //Steven 20211112 : 通富微不可以用鍵盤輸入
            IniConfig.bVTESTFunction==true ||                                   //jou 20220912 : 增加VTEST不可以用鍵盤輸入
            (IniConfig.bSPILFunction &&
             TestIF_File.b2DIDAllowList &&                                      //JerryYang 20241104 : 支援2DID白名單功能
             AccessLevel==0))
    {
        edtSysOperatorID->Text="";
    }
}

// -- edtSysLotIDKeyUp (golden uLotInfo.cpp:10314-10344) -- WB-5 -----------
void TfLotInfo::edtSysLotIDKeyUp()
{
    if(CUSTOMER_CODE==CC_Murata)
    {
        return;
    }

    if(CUSTOMER_CODE==CC_PTI ||                                                 //RogerYang 20170327 (Steven) 力成使用條碼機 避免利用Tab切換游標直接輸入
       IniConfig.bO23_InputLotIDByBarcode)                                      //Steven 20241224 : LotID只能用Barcode
    {
        // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-5 -- see forms/fLotInfo.h
        // GATE REGISTER WB-5 (InputBarcodeNumber has no port anywhere in
        // this tree).
#if 0
        SetLotID("");
        AnsiString sBarcodeID=InputBarcodeNumber("Input Lot ID:", "LotID");
        SetLotID(sBarcodeID);
#endif
    }
    else if(CUSTOMER_CODE==CC_TFME_CHINA ||                                     //Steven 20211112 : 通富微不可以用鍵盤輸入
            IniConfig.bVTESTFunction==true ||                                   //jou 20220912 : 增加VTEST不可以用鍵盤輸入
            (IniConfig.bSPILFunction &&
             TestIF_File.b2DIDAllowList &&                                      //JerryYang 20241104 : 支援2DID白名單功能
             AccessLevel==0))
    {
#ifndef SOFT_SIMULTE
        edtSysLotID->Text="";
#endif
    }

    if(bLotFirstKeyIn==false)                                                   //Ifor 20190924 : add Barcode 輸入判斷避免讀碼失敗
    {
        bLotFirstKeyIn=true;
    }
}

// -- spOCRChangeFileClick (golden uLotInfo.cpp:8458-8474) --------------------
void TfLotInfo::spOCRChangeFileClick()
{
    if(lbOCRUseFile->Caption!="")
    {
        for(int i=0; i<10; i++)
        {
            bOCROK[i]=false;
        }
        bOCROK[0]=true;
        Timer3->Enabled=true;
    }
    else
    {
       ShowMyMessage("Please Enter Change OCR File Name");
    }
    spOCRChangeFile->Down=false;
}

// -- Timer3Timer (golden uLotInfo.cpp:8476-8485) -- WB-6 -------------------
void TfLotInfo::Timer3Timer()
{
    if(InitialOK==false)                                                        //Steven 20160912 : Add InitialOK in Timer
        return;

    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-6 -- see forms/fLotInfo.h
    // GATE REGISTER WB-6 (fOCR->OCRChangeFile() does not exist on TfOCR).
#if 0
    int ret;
    ret=fOCR->OCRChangeFile();
    if(ret==1)
        Timer3->Enabled=false;
#endif
}

// -- edDeviceNameKeyDown (golden uLotInfo.cpp:10457-10473) -- WB-7 --------
void TfLotInfo::edDeviceNameKeyDown()
{
    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-7 -- see forms/fLotInfo.h
    // GATE REGISTER WB-7 (Clipboard()/InputBarcodeNumber have no port
    // anywhere in this tree).
#if 0
    if((CUSTOMER_CODE==CC_SCC && AccessLevel<iDefHonPrecLevel))
    {
        edDeviceName->Text="";
        Clipboard()->Clear();
        AnsiString sBarcodeID=InputBarcodeNumber("Input Device Name:");
        edDeviceName->Text=sBarcodeID;
    }
#endif

    if(CUSTOMER_CODE==CC_AMD_M && bDeviceName_OK==true)                         //Ifor 20200827 add:避免刷兩次Barcode造成異常
    {
        bDeviceName_OK=false;
        edDeviceName->Text="";
    }
}

// -- SetCheckCodeByLot (golden uLotInfo.cpp:12311-12324) --------------------
void TfLotInfo::SetCheckCodeByLot(bool _enable)
{
    lbCheckCodeByLot->Caption="Check duplicate code by lot：";
    if(_enable)
    {
        lbCheckCodeByLot->Caption=lbCheckCodeByLot->Caption+"Enable";
        lbCheckCodeByLot->Color=clLime;
    }
    else
    {
        lbCheckCodeByLot->Caption=lbCheckCodeByLot->Caption+"Disable";
        lbCheckCodeByLot->Color=clRed;
    }
}

// -- edTempKeyPress (golden uLotInfo.cpp:12032-12042) ------------------------
void TfLotInfo::edTempKeyPress(char Key)
{
    if(CUSTOMER_CODE==CC_AMD_M && CosFunction.bHiSiliconFunction==true)
    {
        if(bTemp_OK==true)
            bTemp_OK=false;

        if(Key=='\r')
            bTemp_OK=true;
    }
}

// -- edTempKeyDown (golden uLotInfo.cpp:12053-12063) -- Key never read ------
void TfLotInfo::edTempKeyDown()
{
    if(CUSTOMER_CODE==CC_AMD_M &&
       CosFunction.bHiSiliconFunction==true &&
       bTemp_OK==true)                                                          //Ifor 20200827 add:避免刷兩次Barcode造成異常
    {
        bTemp_OK=false;
        edTemp->Text="";
    }
}

// -- btnSaveDataClick (golden uLotInfo.cpp:12285-12295) ----------------------
void TfLotInfo::btnSaveDataClick()
{
    if(RunInfo.bLotStart)
    {
        ShowMyMessage("save data error,need lot end!!");
        return;
    }

    if(SystemStart==false)                                                      //frank 20200814 : 每10盤記錄一次summary log
        SaveTrayRecord(10);
}

// -- edPageMouseDown (golden uLotInfo.cpp:11534-11541) -- WB-8 -------------
void TfLotInfo::edPageMouseDown(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_Murata)
        return;
    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-8 (OPENED 20260824) -- see forms/fLotInfo.h
    // GATE WB-8 OPENED 20260824 (FW-QWKEY2): fQwertyKey real since FW-QWKEY1 (fc08e09).
    else
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 20);
}

// -- edtSysLotIDKeyDown (golden uLotInfo.cpp:12065-12080) --------------------
void TfLotInfo::edtSysLotIDKeyDown()
{
    if(IniConfig.bO23_InputLotIDByBarcode)                                      //Steven 20241224 : LotID只能用Barcode
    {
        return;
    }

    if(CUSTOMER_CODE==CC_AMD_M &&
       CosFunction.bHiSiliconFunction==true &&
       bLotID_OK==true)                                                         //Ifor 20200827 add:避免刷兩次Barcode造成異常
    {
        bLotID_OK=false;
        edtSysLotID->Text="";
    }
}

// -- edtSysOperatorIDKeyDown (golden uLotInfo.cpp:12082-12097) ---------------
void TfLotInfo::edtSysOperatorIDKeyDown()
{
    if(IniConfig.bO23_InputLotIDByBarcode)                                      //Steven 20241224 : LotID只能用Barcode
    {
        return;
    }

    if(CUSTOMER_CODE==CC_AMD_M &&
       CosFunction.bHiSiliconFunction==true &&
       bOPID_OK==true)                                                          //Ifor 20200827 add:避免刷兩次Barcode造成異常
    {
        bOPID_OK=false;
        edtSysOperatorID->Text="";
    }
}

// -- edtSysOperatorIDMouseUp (golden uLotInfo.cpp:11755-11794) -- WB-9 -----
void TfLotInfo::edtSysOperatorIDMouseUp()
{
    if(CUSTOMER_CODE==CC_Murata)
    {
        return;
    }
    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-9 -- see forms/fLotInfo.h
    // GATE REGISTER WB-9 (every remaining branch needs InputBarcodeNumber
    // and/or fQwertyKey, neither of which has a port).
#if 0
    else if(IniConfig.bO23_InputLotIDByBarcode)                                 //Steven 20241224 : LotID只能用Barcode
    {
        AnsiString str, str2;
        TEdit *TempEdit=(TEdit *)Sender;
        if(TempEdit==edtCusLotID)                                               //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能
        {
            str="Input Cus. lot ID:";
            str2="LotID";
        }
        else
        {
            str="Input OP ID:";
            str2="UserName";
        }
        TempEdit->Text=InputBarcodeNumber(str, str2);
    }
    else if(CUSTOMER_CODE==CC_TFME_CHINA ||                                     //Steven 20211112 : 通富微不可以用鍵盤輸入
            IniConfig.bVTESTFunction==true ||                                   //jou 20220912 : 增加VTEST不可以用鍵盤輸入
            (IniConfig.bSPILFunction &&
             TestIF_File.b2DIDAllowList &&                                      //JerryYang 20241104 : 支援2DID白名單功能
             AccessLevel==0))
    {
        edtSysOperatorID->Text=InputBarcodeNumber("Input OP ID:", "UserName");
    }
    else if(CUSTOMER_CODE==CC_SCC)                                              //Steven 20200302 : SCC楊恩民說輸入字串5~30個字元
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL);
        if(edtSysOperatorID->Text.Length()<5 || edtSysOperatorID->Text.Length()>30)
        {
            edtSysOperatorID->Text="";
        }
    }
#endif
}

// -- edtSysLotIDMouseUp (golden uLotInfo.cpp:11796-11831) -- WB-10 --------
void TfLotInfo::edtSysLotIDMouseUp()
{
    if(CUSTOMER_CODE==CC_Murata)
    {
    }
    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-10 -- see forms/fLotInfo.h
    // GATE REGISTER WB-10 (every remaining branch needs InputBarcodeNumber
    // and/or fQwertyKey, neither of which has a port).
#if 0
    else if(CUSTOMER_CODE==CC_PTI ||                                            //RogerYang 20170327 (Steven) 力成使用條碼機
            IniConfig.bO23_InputLotIDByBarcode)                                 //Steven 20241224 : LotID只能用Barcode
    {
        SetLotID("");
        AnsiString sBarcodeID=InputBarcodeNumber("Input Lot ID:", "LotID");
        SetLotID(sBarcodeID);
    }
    else if(CUSTOMER_CODE==CC_TFME_CHINA)
    {
        edtSysLotID->Text=InputBarcodeNumber("Input Lot ID:", "LotID");
    }
    else if(IniConfig.bSPILFunction &&
            TestIF_File.b2DIDAllowList && AccessLevel==0)                       //JerryYang 20241104 : 支援2DID白名單功能
    {
        edtSysLotID->Text=InputBarcodeNumber("Input Lot ID:", "LotID");
    }
    else if(CUSTOMER_CODE==CC_SCC)                                              //Steven 20200302 : SCC楊恩民說輸入字串5~30個字元
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL);
        if(edtSysLotID->Text.Length()<5 || edtSysLotID->Text.Length()>30)
        {
            SetLotID("");
        }
    }
    else if(IniConfig.bVTESTFunction==true)                                     //Steven 20211112 : 通富微不可以用鍵盤輸入  //jou 20220912 : 增加VTEST不可以用鍵盤輸入
    {
        edtSysLotID->Text=InputBarcodeNumber("Input Lot ID:", "LotID");
        edtSysOperatorID->Text=InputBarcodeNumber("Input OP ID:", "UserName");
    }
#endif
}

// -- edQAModeMouseDown (golden uLotInfo.cpp:11528-11532) -- WB-11 --------
void TfLotInfo::edQAModeMouseDown(TObject *Sender)
{
    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-11 (OPENED 20260824) -- see forms/fLotInfo.h
    // GATE WB-11 OPENED 20260824 (FW-QWKEY2): fQwertyKey real since FW-QWKEY1 (fc08e09).
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 10000, 5);
}

// -- edtSysOperatorIDKeyPress (golden uLotInfo.cpp:11935-11963) -------------
void TfLotInfo::edtSysOperatorIDKeyPress(char Key)
{
    if(IniConfig.bO23_InputLotIDByBarcode)                                      //Steven 20241224 : LotID只能用Barcode
        return;

    if(CUSTOMER_CODE==CC_Murata)
    {
        if(Key=='\r')
        {
            cbRunMode->Text="";
            // DEVIATION: golden `cbRunMode->SetFocus();` dropped -- see
            // forms/fLotInfo.h's DEVIATION note (edPage's declaration, Wave A).
        }
        else if(Key=='$')
        {
            edtSysOperatorID->Text=edtSysOperatorID->Text.SubString(1, edtSysOperatorID->Text.Length()-1);
            cbRunMode->Text="";
            // DEVIATION: golden `cbRunMode->SetFocus();` dropped, see above.
        }
    }
    else if(CUSTOMER_CODE==CC_AMD_M && CosFunction.bHiSiliconFunction==true)
    {
        if(bOPID_OK==true)
            bOPID_OK=false;

        if(Key=='\r')
            bOPID_OK=true;
    }
}

// -- cbRunModeKeyPress (golden uLotInfo.cpp:11965-11988) -- WB-12 ---------
void TfLotInfo::cbRunModeKeyPress(char Key)
{
    if(CUSTOMER_CODE==CC_Murata)                                                //Steven 20200629 : Murata要求輸入barcode後, 自動跳下一個欄位
    {
        if(Key=='\r')
        {
            sbSECSLotStart->Click();
            sbSECSLotStart->Down=true;
            sbSECSLotEnd->Down=false;
        }
        else if(Key=='$')
        {
            cbRunMode->Text=cbRunMode->Text.SubString(1, cbRunMode->Text.Length()-1);
            sbSECSLotStart->Click();
            sbSECSLotStart->Down=true;
            sbSECSLotEnd->Down=false;
        }
    }

    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-12 -- see forms/fLotInfo.h
    // GATE REGISTER WB-12 (the SAME golden call Wave A's WA-7 already gates).
#if 0
    if(fBarCode->JCETUseMakeWhite2DIDList()==true)                              //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    {
        cbRunMode->Text="";                                                     //不允許手動輸入
    }
#endif
}

// -- cbRunModeDropDown (golden uLotInfo.cpp:15928-15951) -- WB-13 --------
void TfLotInfo::cbRunModeDropDown()
{
    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-13 -- see forms/fLotInfo.h
    // GATE REGISTER WB-13 (the guard itself needs the WA-7/WB-12-gated
    // fBarCode member, so there is no independent real branch to keep).
#if 0
    if(CUSTOMER_CODE==CC_JCET && fBarCode->JCETUseMakeWhite2DIDList()==true)
    {
        AnsiString sRunMode="";
        bool bFlag=false;
        sRunMode=InputBarcodeNumber("Input Run Mode", "RunMode");
        for(int i=0; i<cbRunMode->Items->Count; i++)                            //RogerYang 20251208 : JCET 2D FT1白名單/FT2比對功能
        {
            if(cbRunMode->Items->Strings[i]==sRunMode)
            {
                cbRunMode->Text=sRunMode;
                cbRunMode->ItemIndex=i;
                bFlag=true;
                break;
            }
        }

        if(bFlag==false)
        {
            cbRunMode->ItemIndex=-1;
        }
    }
#endif
}

// -- GetFTP_SettingN06 (golden uLotInfo.cpp:15449-15460) ---------------------
void TfLotInfo::GetFTP_SettingN06(AnsiString &asUserID, AnsiString &asPassword, AnsiString &asHost)
{
    #ifdef SOFT_SIMULTE
    asUserID="HONPREC";
    asPassword="27025312";
    asHost="127.0.0.1";
    #else
    asUserID=IniConfig.FtpUserName;
    asPassword=IniConfig.FtpPassword;
    asHost=IniConfig.FtpHost;
    #endif
}

// -- CheckNoRetestBinFlag (golden uLotInfo.cpp:14608-14630) -- WB-14 ------
bool TfLotInfo::CheckNoRetestBinFlag()                                          //RogerYang 20250604 偉測不可複測bin功能
{
    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-14 -- see forms/fLotInfo.h
    // GATE REGISTER WB-14 (fMesSystem has no port at all; ShowErrorMessage is
    // a real alarm-raise). Gated body returns true, golden's own initial/
    // conservative "no violation detected" value.
#if 0
    bool flag=true;
    for(int i=0; i<3; i++)
    {
        if(fMesSystem->NeedNoRTBinID(i)==true)
        {
            ShowErrorMessage(sMES1713[i], K_RETRY|K_SKIP, iMMAuto[eFix1+i], false);                                     //RogerYang 20250806 修正FixIndex
            flag=false;
        }
    }

    if(flag==false)
    {
        flag=true;
        for(int i=0; i<3; i++)                                                  //再檢查一次，如果user都輸入正確，則繼續執行
        {
            if(fMesSystem->NeedNoRTBinID(i)==true)
                flag=false;
        }
    }
    return flag;
#else
    return true;
#endif
}

// -- edStationNumMouseDown (golden uLotInfo.cpp:14060-14064) -- WB-15 ----
void TfLotInfo::edStationNumMouseDown(TObject *Sender)
{
    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-15 (OPENED 20260824) -- see forms/fLotInfo.h
    // GATE WB-15 OPENED 20260824 (FW-QWKEY2): fQwertyKey real since FW-QWKEY1 (fc08e09).
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 99);
}

// -- ATC_OFFLINE_FormComInit (golden uLotInfo.cpp:14969-14992) ---------------
void TfLotInfo::ATC_OFFLINE_FormComInit()
{
    if(AirStream_Select==0)
        return;

    bInitFormcomponent = true;
    for(int i=0;i<8;i++)
    {
        TriLabRefrigerantValue[i]->Caption = "-999.0";
        TripnlRefCopm1Status[i]->Color = clGray;
        TripnlRefCopm2Status[i]->Color = clGray;
        TriLabRefCopm1HpValue[i]->Caption = "-999.0";
        TriLabRefCopm2HpValue[i]->Caption = "-999.0";
        TriLabRefCopm1LpValue[i]->Caption = "-999.0";
        TriLabRefCopm2LpValue[i]->Caption = "-999.0";
        TriLabRefrigerantAdjustValue[i]->Caption = "-999.0";
        TriLabRefrigerantAdjustValue[i]->Visible = false;
        TripnlRefrigerantMachine[i]->Color = clGray;
    }
    labRefrigerantMachineHighLimit->Caption     ="Comp#2 Hp Over High Limit : -999.0";
    labRefrigerantMachineLowLimit->Caption      ="Comp#2 Hp Over Low Limit : -999.0";

    OldRefrigerantCommand = false;
}

// -- ScanRefrigerantSystem (golden uLotInfo.cpp:14941-14967) -- WB-16 ----
void TfLotInfo::ScanRefrigerantSystem()
{
    if(AirStream_Select==0)
        return;

    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-16 -- see forms/fLotInfo.h
    // GATE REGISTER WB-16 (ATC_InterfaceForm exposes only iATC_MODE_TYPE,
    // same finding as Wave A's WA-6).
#if 0
    if(iATCOnLine==0 || ATC_InterfaceForm->IsConnect()==false)
    {
        if(bInitFormcomponent==false)
        {
            ATC_OFFLINE_FormComInit();
        }
        ATC_InterfaceForm->bReadRefrigerantMode_Send=false;
        ATC_InterfaceForm->bReadRefrigerantMode_Recv=false;
    }
    else
    {
        bInitFormcomponent=false;
        if(ATC_InterfaceForm->bReadRefrigerantMode_Recv==true)
        {
            ATC_InterfaceForm->bReadRefrigerantMode_Recv=false;
            RefreshRefrigerantAllStatus();
        }

        if(ATC_InterfaceForm->bReadRefrigerantMode_Send==false)
            ATC_InterfaceForm->Get_ATCRefrigeratorAllStatus(true);
    }
#endif
}

// -- RefreshRefrigerantAllStatus (golden uLotInfo.cpp:14994-15078) -- WB-17 --
void TfLotInfo::RefreshRefrigerantAllStatus()
{
    if(AirStream_Select==0)
        return;

    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-17 -- see forms/fLotInfo.h
    // GATE REGISTER WB-17 (both arms need ATC_InterfaceForm members absent
    // from the 1-member shim, same finding as WB-16/WA-6).
#if 0
    char strRefrigeratorAllStatu[8][40];
    if(ATC_InterfaceForm->bReadRefrigerantMode_AllStatus ==true)
    {
        for(int i=0; i<iATC_Refrigerator_Num; i++)
        {
            if(ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][0] ==1)
            {
                TripnlRefrigerantMachine[i]->Color = clGreen;
            }
            else
            {
                TripnlRefrigerantMachine[i]->Color = clGray;
            }
            sprintf(strRefrigeratorAllStatu[0], "%5.0f", ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][1]);
            sprintf(strRefrigeratorAllStatu[1], "%5.1f", ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][2]);
            sprintf(strRefrigeratorAllStatu[2], "%5.1f", ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][3]);
            sprintf(strRefrigeratorAllStatu[3], "%5.1f", ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][4]);
            sprintf(strRefrigeratorAllStatu[4], "%5.1f", ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][5]);
            sprintf(strRefrigeratorAllStatu[5], "%5.1f", ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][6]);
            TriLabRefrigerantValue[i]->Caption  = strRefrigeratorAllStatu[0];
            TriLabRefCopm1HpValue[i]->Caption   = strRefrigeratorAllStatu[1];
            TriLabRefCopm1LpValue[i]->Caption   = strRefrigeratorAllStatu[2];
            TriLabRefCopm2HpValue[i]->Caption   = strRefrigeratorAllStatu[3];
            TriLabRefCopm2LpValue[i]->Caption   = strRefrigeratorAllStatu[4];
            TriLabRefrigerantAdjustValue[i]->Caption    = strRefrigeratorAllStatu[5];
            if(ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][7] ==1)
            {
                TripnlRefCopm1Status[i]->Color = clGreen;
            }
            else
            {
                TripnlRefCopm1Status[i]->Color = clGray;
            }

            if(ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][8] ==1)
            {
                TripnlRefCopm2Status[i]->Color = clGreen;
            }
            else
            {
                TripnlRefCopm2Status[i]->Color = clGray;
            }
        }
        sprintf(strRefrigeratorAllStatu[6],"Comp#2 Hp Over High Limit : %5.1f",ATC_InterfaceForm->dATC_RefrigerantMachineHighLimit);
        sprintf(strRefrigeratorAllStatu[7],"Comp#2 Hp Over Low Limit : %5.1f",ATC_InterfaceForm->dATC_RefrigerantMachineLowLimit);
        labRefrigerantMachineHighLimit->Caption     =strRefrigeratorAllStatu[6];
        labRefrigerantMachineLowLimit->Caption      =strRefrigeratorAllStatu[7];
    }
    else
    {
        for(int i=0; i<8; i++)
        {
            if(ATC_InterfaceForm->iATC_RefrigeratorUserMode[0][i]==1)
            {
                TripnlRefrigerantMachine[i]->Color = clGreen;
            }
            else
            {
                TripnlRefrigerantMachine[i]->Color = clGray;
            }
        }

        if(OldRefrigerantCommand ==false)
        {
            OldRefrigerantCommand = true;
            for(int i=1; i<8; i++)
            {
                TriLabRefrigerantValue[i]->Caption  = FloatToStr(-999.0);
                TriLabRefCopm1HpValue[i]->Caption   = FloatToStr(-999.0);
                TriLabRefCopm1LpValue[i]->Caption   = FloatToStr(-999.0);
                TriLabRefCopm2HpValue[i]->Caption   = FloatToStr(-999.0);
                TriLabRefCopm2LpValue[i]->Caption   = FloatToStr(-999.0);
                TriLabRefrigerantAdjustValue[i]->Caption    = FloatToStr(-999.0);
                TripnlRefCopm1Status[i]->Color = clGray;
                TripnlRefCopm2Status[i]->Color = clGray;
                TripnlRefrigerantMachine[i]->Color = clGray;
            }
        }
    }
#endif
}

// -- leRunCardNumberMouseDown (golden uLotInfo.cpp:15780-15784) -------------
void TfLotInfo::leRunCardNumberMouseDown()
{
    leRunCardNumber->Text="";
}

// -- RefreshAMR (golden uLotInfo.cpp:15816-15844) -- was a no-op stub -------
void TfLotInfo::RefreshAMR()                                                    //Sam 20240304 : 新增 AMR 功能
{
    cbA60_1->Checked=IniConfig.bA60EnableAMR;

    if(CosFunction.bEnableHandlerResultServer==false || IniConfig.bA60EnableAMR==false)
        return;

    pnlWaitTXSetLoader->Caption=IntToStr(IniConfig.iA60NotifyQty[0]);
    pnlWaitRXSetAuto1->Caption=IntToStr(IniConfig.iA60NotifyQty[3]);
    pnlWaitRXSetAuto2->Caption=IntToStr(IniConfig.iA60NotifyQty[4]);
    pnlWaitRXSetAuto3->Caption=IntToStr(IniConfig.iA60NotifyQty[5]);

    pnlWaitTXTotalLoader->Caption=IntToStr(LastSet.iAMRTrayLoaderTotal);
    pnlWaitTXCntLoader->Caption=IntToStr(LastSet.iAMRTrayConut[0]);
    pnlWaitRXCntAuto1->Caption=IntToStr(LastSet.iAMRTrayConut[3]);
    pnlWaitRXCntAuto2->Caption=IntToStr(LastSet.iAMRTrayConut[4]);
    pnlWaitRXCntAuto3->Caption=IntToStr(LastSet.iAMRTrayConut[5]);

    aldWaitTXLoader->Value=AMR.CheckLoaderCount();

    aldWaitRXAuto1->Value=AMR.CheckUnloaderCount(0);
    aldWaitRXAuto2->Value=AMR.CheckUnloaderCount(1);
    aldWaitRXAuto3->Value=AMR.CheckUnloaderCount(2);

    aldWaitTrayFeed->Value=LastSet.bAMRTrayFeedWait;
    aldLoaderLast->Value=LastSet.bAMRLoaderLast;
    ShowAMRCategoryBin();
}

// -- ShowAMRCategoryBin (golden uLotInfo.cpp:15846-15881) --------------------
void TfLotInfo::ShowAMRCategoryBin(bool bInitial)
{
    if(bInitial)
    {
        StrGrdCategory->Cells[1][0]="Count";
        StrGrdCategory->Cells[2][0]="Percent";
        StrGrdCategory->RowCount=iTestBinCount+2;
        for(int i=0; i<iTestBinCount; i++)
            StrGrdCategory->Cells[0][1+i]="Category "+AnsiString(i);

        StrGrdCategory->Cells[0][iTestBinCount+1]="Error Bin";
        return;
    }

    if(CosFunction.bEnableHandlerResultServer==false || IniConfig.bA60EnableAMR==false)
        return;

    int sum=0;
    double f=0.0;
    for(int i=0; i<iTestBinCount; i++)
        sum+=LastSet.iBinData32[2][i];

    for(int i=0; i<iTestBinCount+1; i++)
    {
        StrGrdCategory->Cells[1][1+i]=LastSet.iBinData32[2][i];
        if(sum>0)
        {
            f=(double)LastSet.iBinData32[2][i]*100.0/(double)sum;
            StrGrdCategory->Cells[2][1+i]=(AnsiString)GetFloatFormatString(f, 5, 2)+(AnsiString)("%");
        }
        else
        {
            StrGrdCategory->Cells[2][1+i]="0.00%";
        }
    }
}

// -- cbFirstTrayCheckOnUnloaderMouseDown (golden uLotInfo.cpp:16029-16034) --
void TfLotInfo::cbFirstTrayCheckOnUnloaderMouseDown()
{
    bP60UserClicked=true;
}

// -- RefreshOtherTool (golden uLotInfo.cpp:16047-16072) ----------------------
void  TfLotInfo::RefreshOtherTool()                                             //Jimmychiu 20251219 : Refresh PTI funciton
{
    static bool bTimerRunning=false;
    if((iTestRunMode==FT && TestIF_File.bContinuousLoader==false) ||
       (iTestRunMode==RT && TestIF_File.bContinuousLoader_RT==false))
    {
        return;
    }

    if(bTimerRunning)
        return;
    bTimerRunning=true;
    //
    if(iTestRunMode==FT)
    {
        edLoaderCountNow->Text=IntToStr(LastSet.SendCT[2]);
        edLoaderCountAlarm->Text=IntToStr(TestIF_File.iContinuousLoaderCount);
    }
    else
    {
        edLoaderCountNow->Text=IntToStr(LastSet.SendCT[2]);
        edLoaderCountAlarm->Text=IntToStr(TestIF_File.iContinuousLoaderCount_RT);
    }
    //
    bTimerRunning=false;
}

// -- labNowTrayCountClick (golden uLotInfo.cpp:16186-16190) ------------------
void TfLotInfo::labNowTrayCountClick()
{
    int i = 0;
    labNowTrayCount->Caption=i;
}

// -- Timer4Timer (golden uLotInfo.cpp:16487-16493) -- WB-18 -----------------
void TfLotInfo::Timer4Timer()
{
    CheckActionFlag();
    // AI(W906-FW3-LotInfo-WB) 20260819: GATE WB-18 -- see forms/fLotInfo.h
    // GATE REGISTER WB-18 (CheckAMRAction() is RECON item #206, (b)
    // write-path, not part of this wave's 39 Tier-2 methods).
#if 0
    CheckAMRAction();
#endif

    ReflashInfo();
}

// =============================================================================
//  AI(W906-FW3-LotInfo-WC) 20260819: uLotInfo MIXED-split Wave C -- FormShow
//  and Timer2Timer. See forms/fLotInfo.h's WC banner for STEP 0 / WAVE SCOPE
//  / GATE REGISTER. Segment numbers (S#/T#) follow
//  docs/RECON_uLotInfo_mixed_split.md's own segment table.
// =============================================================================

// -- FormShow (golden uLotInfo.cpp:316-1233) ---------------------------------
void TfLotInfo::FormShow()
{
    // -- S1 (golden :316-346) -- WC-10 gates the ts_ATC6_1 ATC_TYPE_61 clause --
    bShow=true;

    AnsiString sConfigPath=AuthPath+"Security_new.def";                         //JerryYang 20241019 : 矽品二林 耀仁要求Auto clean高度可選擇不覆蓋

    GetTimeInfo();
    ts_FTPAutomation->TabVisible=false;                                         //KaiChen 20190530 ：Sigurd FTP Automation
    tsSigurd_CX->TabVisible =(CUSTOMER_CODE==CC_SIGURD_ChungXing);              //Sam 20220223 : 矽格中興廠新增 Lot 資料
    tsMurata->TabVisible    =(CUSTOMER_CODE==CC_Murata);
    tsSPIL_SZ->TabVisible   =(CUSTOMER_CODE==CC_SPIL_CHINA_SUZHOU);
    tsOEE->TabVisible       =(CosFunction.bOEEFunction);                        //Steven 20221225 : 整理LotInfo畫面
    SettsChipAdvVisible();                                                      //Steven 20221225 : add for CyuEan
    // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-10 -- see forms/fLotInfo.h
    // GATE REGISTER WC-10 (ATC_TYPE_61 is not a defined identifier anywhere
    // in this tree).
#if 0
    ts_ATC6_1->TabVisible   =(Tri_Temp_Machine==1 || (ATC_SYSTEM==eNewATCSystem && ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_61));
#endif
    tsPATSetUp->TabVisible  =(CUSTOMER_CODE==CC_PANTHER);
    tsBundle->TabVisible    =(USE_COVER_TRAYID!=tCIDNotUse);
    tsSetupFileCheck->TabVisible=(IniConfig.bEnableRmsCheckSetupFile==true);    //Ifor 20230516 add: TFAMD 要求工作檔驗證

    lblPage->Visible=CosFunction.bUseSCKART;
    edPage->Visible=CosFunction.bUseSCKART;

    labCusLotID->Visible=false;                                                 //JerryYang 20230322 : 2D sort lot info UI修改
    labCusDevGrp->Visible=false;
    labDeviceName->Visible=false;
    edtCusLotID->Visible=false;
    edtCusDevGrp->Visible=false;
    edtDevice->Visible=false;
    labCusStep->Visible=false;                                                  //JerryYang 20260201 : add
    edtCusStep->Visible=false;

    pgLotinfo->ActivePage=tsLotID;

    // -- S2 (golden :348-362) --
    if(CUSTOMER_CODE==CC_CYUEAN ||                                              //Steven 20240122 : CyuEan要FT/RT
       CUSTOMER_CODE==CC_JSI_HAOXING)                                           //Steven 20230302 : Add for 紹興長電
    {
        lbLotRunMode->Visible   =true;
        cbRunMode->Visible      =true;
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("FT");
        cbRunMode->Items->Add("RT");
        cbRunMode->Text="FT";
    }
    else if(CUSTOMER_CODE==CC_NEXPERIA_Guangdong)                               //Steven 20230301 : Add for 安世
    {
        lbLotRunMode->Visible   =false;
        cbRunMode->Visible      =false;
    }

    // -- S3 (golden :364-380) -- WC-1 gates ResetLotInfo() --
    if(CUSTOMER_CODE==CC_MTI)
    {
        tsDeviceInfo->TabVisible=false;
        tsFTP->TabVisible=false;
        palSecsGem->Visible=false;                                              //Steven 20140701
    }
    else if(IniConfig.bShowLotInfo)
    {
        tsDeviceInfo->TabVisible=(IniConfig.bEnableRms==true);
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-1 -- see forms/fLotInfo.h
        // GATE REGISTER WC-1 (ResetLotInfo() is RECON item #149, (b)
        // write-path, not yet translated).
#if 0
        ResetLotInfo();
#endif
        if(tsDeviceInfo->TabVisible==true)
        {
            pgLotinfo->ActivePage=tsDeviceInfo;
        }
    }
    SetSelectionVisible();                                                      //Steven 20250519 : 統一Selection的顯示設定

    // -- S4 (golden :381-398) -- WC-2 gates cbPATModeChange(cbPATMode) --
    if(CUSTOMER_CODE==CC_TSI)                                                   //frank 20200814 : 每10盤記錄一次summary log
    {
        lblOPID->Caption        ="Tester ID";
        lbLotRunMode->Visible   =false;
        cbRunMode->Visible      =false;
        btnSaveData->Visible    =true;
    }
    else if(CUSTOMER_CODE==CC_PANTHER)
    {
        tsLotID->TabVisible=IniConfig.bB12UsePATSetup;
        cbPATMode->ItemIndex=0;
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-2 -- see forms/fLotInfo.h
        // GATE REGISTER WC-2 (cbPATModeChange() is RECON item #169, (b)
        // write-path dispatcher, not yet translated).
#if 0
        cbPATModeChange(cbPATMode);
#endif
        #ifdef SOFT_SIMULTE
        btnRealTime->Visible=true;
        btnpatHourly->Visible=true;
        btnpatEndLot->Visible=true;
        #endif
    }
    // -- S5 (golden :399) -- WC-3 gates ShowXMLOnLine() --
    // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-3 -- see forms/fLotInfo.h
    // GATE REGISTER WC-3 (ShowXMLOnLine() is RECON item #103, a
    // RecordProcess-backed function that writes files x4 despite the "Show"
    // name).
#if 0
    ShowXMLOnLine();                                                            //Steven 20200706 : 移到外面
#endif

    // -- S6 (golden :401-410) --
    if(CUSTOMER_CODE==CC_JCET)                                                  //Steven 20170605 (wei) : For長電
        tsFTP->TabVisible=IniConfig.bEnableFTP;
    else
        tsFTP->TabVisible=(CosFunction.bFTPFunction);

    if(IniConfig.bEnable_SECS_GEM ||                                            //Steven 20140701
       CosFunction.bLotStartLockCriticalPara)                                   //JerryYang 20220311 : ATP鎖定Critical parameter
    {
        palSecsGem->Visible=true;
    }

    // -- S7 (golden :412-425) -- WC-4 gates the FormBarcodeReader PopupMenu lines --
    if(CUSTOMER_CODE==CC_KYEC_LEE ||
       (CUSTOMER_CODE==CC_AMD_M && CosFunction.bHiSiliconFunction==true))       //wei 20150826 Lot 強制顯示
    {
        palCurrFailRate->Visible=false;
        Panel27->Visible=false;                                                 //Ifor 20190517 KYEC 不顯示

        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-4 -- see forms/fLotInfo.h
        // GATE REGISTER WC-4 (FormBarcodeReader has no port anywhere in this
        // tree; PopupMenu is also not a modeled TControl property).
#if 0
        edtSysLotID->PopupMenu=FormBarcodeReader->pmBarcode;
        edtSysOperatorID->PopupMenu=FormBarcodeReader->pmBarcode;
#endif
        #ifdef SOFT_SIMULTE
            LotKeyInTime->Enabled = false;
        #else
            LotKeyInTime->Enabled = true;                                       //Ifor 20190919 add Lot Info Sacn Time
        #endif
    }

    // -- S8 (golden :427) -- WC-1 (2nd call site, see S3) --
    // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-1 -- see forms/fLotInfo.h
    // GATE REGISTER WC-1 (same ResetLotInfo() gate as S3; golden calls it
    // twice with no state change between the calls -- a golden oddity, not a
    // translation choice).
#if 0
    fLotInfo->ResetLotInfo();                                                   //Steven 20240925 : 重開軟體時, 要讀回lot info
#endif

    // -- S9 (golden :429-522) -- WC-5 gates btnSaveClick(this) x2; WC-6 gates
    // FileListBox1->Visible=false --
    grpRFID->Visible=(USE_RFID_READER);                                         //Steven 20220713 : RFID Reader for SJSEMI

    if(CUSTOMER_CODE==CC_PTI ||                                                 //RogerYang 20170329 (Steven) 力成 LotID卡關
       CUSTOMER_CODE==CC_TFME_CHINA)
    {
        tsFTP->TabVisible=false;                                                //不知道幹嘛用，先藏起來
    }

    if(CUSTOMER_CODE==CC_SCK)                                                   //ChungHung 20131225 add
    {
        ts_AutoCleanMonitor->TabVisible=true;
    }
    else
    {
        ts_AutoCleanMonitor->TabVisible=false;
    }

    if(CUSTOMER_CODE==CC_AMKOR_Korea)                                           //Steven 20150923
    {
        Label5->Visible=false;
        edTemp->Visible=false;
    }
    else if(CUSTOMER_CODE==CC_SCC ||
            CUSTOMER_CODE==CC_SCK)                                              //ChungHung 20130621 add SCK RMS
    {
        Label5->Visible=false;
        edTemp->Visible=false;
        btDownload->Caption="Download from Handler";
    }
    else if(IniConfig.bSPILFunction==true)                                      //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
    {
        chkAutoCleanContactHeight->Checked=CheckAndReadIniData(sConfigPath, "Network", "Auto Clean Contact Height",      false);                                //JerryYang 20241019 : 矽品二林 耀仁要求Auto clean高度可選擇不覆蓋
        btnFtpTester->Visible       =false;
        Label153->Visible           =false;
        edDeviceName->Visible       =false;
        Label5->Visible             =false;
        edTemp->Visible             =false;
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-6 -- see forms/fLotInfo.h
        // GATE REGISTER WC-6 (TFileListBox derives from bare TObject, no
        // ->Visible member).
#if 0
        FileListBox1->Visible       =false;
#endif
        tsDeviceInfo->TabVisible    =true;
        FileListBox1->Directory     =DataPath;
        chkTempOffset->Checked      =false;                                     //JerryYang 20180212 (Steven) SPIL要求鎖死不得修改
        chkContactHigh->Checked     =false;
        chkContactForce->Checked    =true;
        chkContactMode->Checked     =true;
        checkbAutoClean->Checked    =false;                                     //JerryYang 20191003 矽品只還原auto clean offset
        chkHotPlate->Checked        =true;
        chkLoadUnload->Checked      =true;
        chkSpeedSetting->Checked    =true;
        chkShuttleMode->Checked     =true;
        chkTestMode->Checked        =true;
        cbBottom2DOffset->Checked   =false;                                     //JerryYang 20201122 Bottom 2D offset不覆蓋
        chkTestMode->Visible        =false;                                     //JerryYang 20170214 (Steven) 此三項無作用,先不顯示
        chkBinasgn->Visible         =false;
        chkBinasgnOff->Visible      =false;
        groupbDownloadItem->Enabled =false;
        cbRTCASTD->Visible          =true;
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-5 -- see forms/fLotInfo.h
        // GATE REGISTER WC-5 (btnSaveClick() is RECON item #38, (b)
        // write-path, WriteIniData x19 inside).
#if 0
        btnSaveClick(this);                                                     //JerryYang 20180212 (Steven) SPIL要求鎖死不得修改
#endif
    }
    else if(CUSTOMER_CODE==CC_TERAPOWER)
    {
        btnFtpTester->Visible=false;
    }
    else if(CUSTOMER_CODE==CC_AMD_M)
    {
        btnFtpTester->Visible=false;                                            //不顯示
    }
    else if(CUSTOMER_CODE==CC_AMKOR_China ||                                    //jou 2016-06-13 修正 Amkor china download recipe 參數會參照本機
            CUSTOMER_CODE==CC_QUALCOMM)                                         //JerryYang 20170412 (Steven) add QUALCOMM
    {
        chkTempOffset->Checked  =false;
        chkContactHigh->Checked =false;
        chkContactForce->Checked=true;
        chkContactMode->Checked =true;
        chkBinasgn->Visible     =false;                                         //jou 2014-08-26 安靠要求 Auto download Binasgn不顯示並且強制開啟
        chkBinasgnOff->Visible  =false;
        chkBinasgn->Checked     =true;
        chkBinasgnOff->Checked  =true;

        chkContactHigh->Enabled =false;
        chkTempOffset->Enabled  =false;
        chkContactForce->Enabled=false;
        chkContactMode->Enabled =false;

        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-5 (2nd call site) --
#if 0
        btnSaveClick(this);
#endif
    }
    else if(CUSTOMER_CODE==CC_ChipMos_ZHUBEI)                                   //Jimmychiu 20250430 : ChipMos 關閉Device info 溫度欄位
    {
        edTemp->Visible=false;
    }
    else
    {
        ART_Panel->Visible=false;
    }
    tsRTCFullViewImg->TabVisible=REAL_TIME_CCD;                                 //Steven 20110825 : Real time CCD - 顯示Full View Image

    // -- S10 (golden :523-541) --
    if(!TrayForm.bEnableAMR) //Eastsun 20260515 F020
        tsKYEC_AMR->TabVisible=false;

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20150706 ART
    {
        GroupBox3->Visible=false;
        ART_Panel->Visible=true;
        ART_Panel->Left=3;
        ART_Panel->Top=5;
    }
    else
    {
        ART_Panel->Visible=false;
        GroupBox3->Visible=true;
        GroupBox3->Left=3;
        GroupBox3->Top=5;
    }

    // -- S11 (golden :543) --
    SetATCFormVisible();                                                        //Steven 20160217 : For ATC7.0  //Ifor 20160516 往下移動，避免ATC頁面關閉後又被打開

    // -- S12 (golden :545-562) -- WC-7 gates the HT9046_LS imgRTCFullView block --
    tsBarCode->TabVisible=(BAR_CODE_INSTALL==ebctUseCCDMode || BAR_CODE_INSTALL==ebctInShtIntel || BAR_CODE_INSTALL==ebctEtherNetCCD);                          //Ifor 20190129 : add Cognex EtherNet 通訊
    btChangeFile->Visible=(BAR_CODE_INSTALL==ebctInShtIntel || BAR_CODE_INSTALL==ebctEtherNetCCD || (BAR_CODE_INSTALL==ebctUseCCDMode && CosFunction.b2DUseSubJobFunction==true));  //wei 20160728 Barcode File切換 //add Sub Job

    tsESDMonitor->TabVisible=false;                                             //Ifor 20160308 ESD 畫面顯示

    ts_OCRInterface->TabVisible=(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR==false);
    ts_SocketInterface->TabVisible=IniConfig.bSocketCommunication;              //ChungHung 20130112 add for ASE_KR Socket Tester

    // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-7 -- see forms/fLotInfo.h
    // GATE REGISTER WC-7 (TImage/TCanvas have zero port anywhere in this
    // tree, same established WB-1 finding, re-confirmed 20260819).
#if 0
    if(MachineTypeChoice==Type_HT9046_LS)                                       //2013-01-15    Dell
    {
        if(REAL_TIME_CCD==true && COM2->bCCDDummyRum == false)
        {
            imgRTCFullView1->Width = imgRTCFullView1->Width/2;
            imgRTCFullView2->Width = imgRTCFullView2->Width/2;
            imgRTCFullView3->Width = imgRTCFullView3->Width/2;
            imgRTCFullView4->Width = imgRTCFullView4->Width/2;
        }
    }
#endif

    // -- S13 (golden :564-590) --
    tsBarCode->TabVisible=(BAR_CODE_INSTALL==ebctUseCCDMode || BAR_CODE_INSTALL==ebctInShtIntel || BAR_CODE_INSTALL==ebctEtherNetCCD);                          //Ifor 20190129 : add Cognex EtherNet 通訊

    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           //ChungHung 20150413 add for TSMC
    {
        IniConfig.bShowLotInfo=true;
        btnDataFTPSaveToData->Visible=true;
        edtSysOperatorID->Visible=false;
        lblOPID->Visible=false;
        spSECSLotCheck->Visible=true;
        sbSECSLotStart->Enabled=false;
        Panel6->Visible=true;
        Panel6->Top=144;
        Panel6->Left=3;
    }
    else
    {
        btnDataFTPSaveToData->Visible=false;
        edtSysOperatorID->Visible=true;
        lblOPID->Visible=true;
        spSECSLotCheck->Visible=false;
        sbSECSLotStart->Enabled=true;
        sbSECSLotEnd->Down=true;
        sbSECSLotStart->Down=false;
        Panel6->Visible=false;
        Panel6->Top=320;
        Panel6->Left=3;
    }

    // -- S14 (golden :592-602) -- WC-8 gates RefreshYieldMonitor() (1st site) --
    if(IniConfig.bSIGURDFunction || CosFunction.bShowYieldMonitor)              //Sam 20210916 : 新增 Yiled Monitor 到畫面上 //Sam 20210324 : 新增 Yield Monitor
    {
        tsYieldMonitior->TabVisible=true;
        tsTPW->TabVisible   =(CUSTOMER_CODE==CC_TERAPOWER || CUSTOMER_CODE==CC_PTI);
        tsSigurd->TabVisible=(CUSTOMER_CODE==CC_SIGURD_PeiXing);
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-8 -- see forms/fLotInfo.h
        // GATE REGISTER WC-8 (reuses Wave A's WA-3 RefreshYieldMonitor()
        // gate verbatim).
#if 0
        RefreshYieldMonitor();
#endif
    }
    else
    {
        tsYieldMonitior->TabVisible=false;
    }

    // -- S15 (golden :604-608) -- WC-9 gates fBarCode->mtBarcodeSetDefaultView() --
    // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-9 -- see forms/fLotInfo.h
    // GATE REGISTER WC-9 (mtBarcodeSetDefaultView has no port anywhere in
    // this tree).
#if 0
    fBarCode->mtBarcodeSetDefaultView();
#endif
    chkTestMode->Visible=CosFunction.bLastSetInSetUpFile;                       //jou 2014-08-25 修正未開啟 CosFunction.bLastSetInSetUpFile LotInfo 顯示錯誤

    labLevelMode->Visible=CosFunction.bDownloadRecipeLevelMode;                 //jou 2016-01-06 download recipe 增加權限模式選擇
    coLevelMode->Visible=CosFunction.bDownloadRecipeLevelMode;                  //jou 2016-01-06 download recipe 增加權限模式選擇

    // -- S16 (golden :610-641) -- zero customer-code branches --------------
    sgBarcode->Cells[ 0][ 0]="Shuttle";
    sgBarcode->Cells[ 1][ 0]="1_A";
    sgBarcode->Cells[ 2][ 0]="1_B";
    sgBarcode->Cells[ 3][ 0]="2_A";
    sgBarcode->Cells[ 4][ 0]="2_B";
    sgBarcode->Cells[ 5][ 0]="Total";
    sgBarcode->Cells[ 0][ 1]="Load";
    sgBarcode->Cells[ 0][ 2]="Pass";
    sgBarcode->Cells[ 0][ 3]="Fail";
    sgBarcode->Cells[ 0][ 4]="Rate(%)";
    sgBarcode->Cells[ 0][ 5]="Retry";
    sgBarcode->Cells[ 0][ 6]="Duplicate";

    sgOCR->Cells[ 0][ 0]="OCR";
    sgOCR->Cells[ 1][ 0]="Tray";
    sgOCR->Cells[ 2][ 0]="Lot";
    sgOCR->Cells[ 0][ 1]="Pass";
    sgOCR->Cells[ 0][ 2]="Key In";
    sgOCR->Cells[ 0][ 3]="No IC";
    sgOCR->Cells[ 0][ 4]="Total";

    sgATRCount->Cells[ 0][ 0]="ATR";
    sgATRCount->Cells[ 1][ 0]="FT";
    sgATRCount->Cells[ 2][ 0]="RT";
    sgATRCount->Cells[ 0][ 1]="Pass";
    sgATRCount->Cells[ 0][ 2]="Fail";
    sgATRCount->Cells[ 0][ 3]="Total";

    // -- S17 (golden :643-649) --
    ShowATCTempPanel();                                                         //Steven 20241112 : 調整ATC溫度顯示
    if(Temperature.bATCActiveCooling==false || CUSTOMER_CODE==CC_KYEC_LEE)      //Ifor 20160902 add 未啟動ATC功能 顯示ATC off line //Ifor 20170609 (wei) add KYEC ATC 不開啟自動連線由人員啟動
    {
        bStartATCRun=false;
        fLotInfo->pl_ATC_Online->Color=clRed;                                   //Ifor 20170609 (wei) add KYEC ATC 不開啟自動連線由人員啟動   //Color 改紅色
        fLotInfo->pl_ATC_Online->Caption="ATC Off Line";                        //Ifor 20170609 (wei) add KYEC ATC 不開啟自動連線由人員啟動   //顯示 ATC Offline
    }

    // -- S18 (golden :651-713) -- WC-21 gates the whole CC_JSCC_OS branch --
    if(CUSTOMER_CODE==CC_PTI &&                                                 //RogerYang 20170417 LotInfo更新至FT Mode
       IniConfig.bB03_TesterReport==false)                                      //Sam 20240809 : PTI ART 模式
    {
        cbRunMode->Enabled=false;
    }

    if(CUSTOMER_CODE==CC_ONSEMI_M)
    {
        labDeviceName->Caption="Device ID";
        labDeviceName->Visible=true;
        edtDevice->Visible=true;
    }

    // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-21 -- see forms/fLotInfo.h
    // GATE REGISTER WC-21 (edtSysLotID/labLotID/edtDevice/labDeviceName/
    // btnFTPDownLoadbyDeviceID geometry + fTesterTCP->rgUnloader, all
    // unavailable/unverified this wave).
#if 0
    if(CUSTOMER_CODE==CC_JSCC_OS)                                               //RogerYang 20260127 : Add For JSCC_OS download by Device list
    {
        int iTopTmp=edtSysLotID->Top;
        edtSysLotID->Top=Panel28->Top+2;
        labLotID->Top=edtSysLotID->Top+4;
        edtDevice->Top=iTopTmp;
        labDeviceName->Top=edtDevice->Top+4;
        btnFTPDownLoadbyDeviceID->Top=edtDevice->Top-2;
        Panel28->Top=edtSysLotID->Top+20;
        labDeviceName->Caption="Device ID";
        labDeviceName->Visible=true;
        edtDevice->Visible=true;
        btnFTPDownLoadbyDeviceID->Visible=true;
        rgUnloader->ItemIndex=fTesterTCP->rgUnloader->ItemIndex;
    }
#endif

    if(CUSTOMER_CODE==CC_SIGURD_ChungXing)                                      //Ifor 20170505 (wei) add 矽格中興使用LotID不顯示Run Mode
    {
        cbRunMode->Visible      =false;
        lbLotRunMode->Visible   =false;
    }

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {
        labLotID->Visible=false;                                                //JerryYang 20220923 : add
        edtSysLotID->Visible=false;
        lblOPID->Visible=false;
        edtSysOperatorID->Visible=false;
        lbLotRunMode->Visible=false;
        cbRunMode->Visible=false;

        sbSECSLotStart->Caption="Start Lot";                                    //Sam 20170925 : 顯示名稱修改
        sbSECSLotEnd->Caption="End Lot";
        btnFtpTester->Visible=false;                                            //Sam 20171006 (wei) : 超豐用不到隱藏起來
        btnFtpServer->Caption ="Download To Handler";                           //Sam 20171006 (wei) : 超豐要求顯示名稱修改
        btnFtpHD->Caption="Upload To Server";                                   //Sam 20171006 (wei) : 超豐要求顯示名稱修改
        sb_RunExecutFile->Visible=true;
        if(IniConfig.asA25RunExecutButtonName!="")
        {
            sb_RunExecutFile->Caption=IniConfig.asA25RunExecutButtonName;
        }
        else
        {
            sb_RunExecutFile->Caption="Run Execut";
        }
    }

    // -- S19 (golden :715-978) -- WC-11/WC-12/WC-13/WC-14/WC-15 (see header) --
    tsMurata->TabVisible    =(CUSTOMER_CODE==CC_Murata);
    edtLine->Visible        =(CUSTOMER_CODE==CC_Murata);

    edtProcessName->Visible =(CUSTOMER_CODE==CC_Murata || IniConfig.bVTESTFunction==true);
    edtProduct->Visible     =(CUSTOMER_CODE==CC_Murata || IniConfig.bVTESTFunction==true);
    tsVTest->TabVisible     =(IniConfig.bVTESTFunction==true);

    if(CUSTOMER_CODE==CC_ASE_KaohSiung_K12)                                     //jou 20191008 : (Steven) add SCC使用Lot ID
    {
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("FT");
        cbRunMode->Items->Add("RT1");
        cbRunMode->Items->Add("RT2");
        cbRunMode->Text="FT";
    }
    else if(IniConfig.bVTESTFunction==true)
    {
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-12 -- see forms/fLotInfo.h
        // GATE REGISTER WC-12 (pgcLotInfo is the stock TPageControl, no
        // ->ActivePage pointer member).
#if 0
        pgcLotInfo->ActivePage=tsVTest;
#endif
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("FT0");                                           //RogerYang 20260420 : 何航航要求增加站點(第二次修改)
        cbRunMode->Items->Add("FT1");
        cbRunMode->Items->Add("FT2");
        cbRunMode->Items->Add("FT3");
        cbRunMode->Items->Add("FT4");
        cbRunMode->Items->Add("FT5");
        cbRunMode->Items->Add("FT6");
        cbRunMode->Items->Add("FT7");
        cbRunMode->Items->Add("FT8");
        cbRunMode->Items->Add("FT9");
        cbRunMode->Items->Add("FT10");
        cbRunMode->Items->Add("FT11");
        cbRunMode->Items->Add("FT12");
        cbRunMode->Items->Add("FT13");
        cbRunMode->Items->Add("FT14");
        cbRunMode->Items->Add("FT15");
        cbRunMode->Items->Add("RT0");                                           //RogerYang 20260420 : 何航航要求增加站點
        cbRunMode->Items->Add("RT1");
        cbRunMode->Items->Add("RT2");
        cbRunMode->Items->Add("RT3");
        cbRunMode->Items->Add("RT4");
        cbRunMode->Items->Add("RT5");
        cbRunMode->Items->Add("EQC0");                                          //RogerYang 20260420 : 何航航要求增加站點
        cbRunMode->Items->Add("EQC1");
        cbRunMode->Items->Add("EQC2");
        cbRunMode->Items->Add("EQC3");
        cbRunMode->Items->Add("EQC4");
        cbRunMode->Items->Add("EQC5");
        cbRunMode->Text="FT1";

        cbTestTimes->Items->Clear();                                            //RogerYang 20250809 偉測Summary文件修改
        cbTestTimes->Items->Add("RP0");
        cbTestTimes->Items->Add("RP1");
        cbTestTimes->Items->Add("RP2");
        cbTestTimes->Items->Add("RP3");
        cbTestTimes->Items->Add("RP4");
        cbTestTimes->Items->Add("RP5");
        cbTestTimes->Items->Add("RP6");
        cbTestTimes->Items->Add("RP7");
        cbTestTimes->Items->Add("RP8");
        cbTestTimes->Items->Add("RP9");
        cbTestTimes->Items->Add("RP10");
        cbTestTimes->Items->Add("RP11");
        cbTestTimes->Items->Add("RP12");
        cbTestTimes->Items->Add("RP13");
        cbTestTimes->Items->Add("RP14");
        cbTestTimes->Items->Add("RP15");
        cbTestTimes->Text="RP0";

        lbProcess->Visible=true;
        cbProcess->Visible=true;

        grpMesCheck->Visible=true;

        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-13 -- see forms/fLotInfo.h
        // GATE REGISTER WC-13 (control re-parenting via ->Parent= has no
        // model anywhere in vclcompat/Controls.h).
#if 0
        edtProcessName->Parent=tsVTest;
#endif
        edtProcessName->EditLabelCaption="CustPart";
        edtProcessName->Left=70;
        edtProcessName->Top=5;
        edtProcessName->Enabled=true;

#if 0
        edtProduct->Parent=tsVTest;
#endif
        edtProduct->EditLabelCaption="CustLotNum";
        edtProduct->Left=70;
        edtProduct->Top=edtProcessName->Top+27;
        edtProduct->Enabled=true;

        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-14 -- see forms/fLotInfo.h
        // GATE REGISTER WC-14 (sbSECSLotStart/sbSECSLotEnd are Wave A/B
        // TSpeedButton* members with no geometry field).
#if 0
        sbSECSLotStart->Top=lbLotRunMode->Top+lbLotRunMode->Height+55;
        sbSECSLotEnd->Top=sbSECSLotStart->Top+35;
#endif

        btnFtpTester->Visible=false;

        labConfigL04->Visible=true;

        chkTempOffset->Enabled=false;
        chkTempOffset->Checked=false;
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-15 -- see forms/fLotInfo.h
        // GATE REGISTER WC-15 (WriteIniData embedded write-path, same class
        // as WC-1/WC-2/WC-5).
#if 0
        WriteIniData(AuthPath+"Security_new.def", "Network", "Temp Offset",     false);
#endif
        checkbAutoClean->Enabled=false;
        checkbAutoClean->Checked=false;
#if 0
        WriteIniData(AuthPath+"Security_new.def", "Network", "Auto Clean",     false);
#endif

        grpOEEState->Visible=true;
        grpOEEState->Left=0;
        sgOEEState->Cells[0][0]="OEE";
        sgOEEState->Cells[0][1]="TimeOEE";
        sgOEEState->Cells[0][2]="Jam";
        sgOEEState->Cells[0][3]="Retest";
        sgOEEState->Cells[0][4]="Down";
        sgOEEState->Cells[0][5]="Setup";
        sgOEEState->Cells[0][6]="Idle";
        sgOEEState->Cells[0][7]="ENG";
        sgOEEState->Cells[0][8]="PM";
        sgOEEState->Cells[0][9]="Stop";
    }
    else if(CUSTOMER_CODE==CC_Murata)
    {
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("FT");
        cbRunMode->Items->Add("RT");
    }
    else if(IniConfig.bSPILFunction==true && LastSet.iTester==_2D_SORT)         //JerryYang 20230322 : 2D sort lot info UI修改
    {
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("VS");
        cbRunMode->Items->Add("RC1");
        cbRunMode->Items->Add("RC2");
        labCusLotID->Visible=true;
        labCusDevGrp->Visible=true;
        labDeviceName->Visible=true;
        edtCusLotID->Visible=true;
        edtCusDevGrp->Visible=true;
        edtDevice->Visible=true;
        edtCusLotID->Enabled=false;                                             //JerryYang 20230322 : 2D sort lot info UI修改
        edtCusDevGrp->Enabled=false;
        labCusStep->Visible=true;                                               //JerryYang 20260201 : add
        edtCusStep->Visible=true;
        edtCusStep->Enabled=true;
    }
    else if(CUSTOMER_CODE==CC_PANTHER)
    {
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("Normal Test");
        cbRunMode->Items->Add("Pre-Test");
        cbRunMode->Items->Add("Re-Test");
        cbRunMode->Items->Add("GD");
        cbRunMode->Items->Add("EQC");
        labLotID->Visible=false;
        edtSysLotID->Visible=false;
        lblOPID->Visible=false;
        edtSysOperatorID->Visible=false;
        lbLotRunMode->Visible=false;
        cbRunMode->Visible=false;
        pgLotinfo->ActivePage=tsLotID;
    }
    else if(TestIF_File.b2DIDAllowList &&                                       //JerryYang 20241104 : 支援2DID白名單功能
            IniConfig.iN23DownloadMethod!=2)                                    //JerryYang 20250320 : 2DID白名單功能
    {
        if(CUSTOMER_CODE==CC_JCET)                                              //RogerYang 20251208 : JCET 2D FT1白名單/FT2比對功能
        {
            // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-11 -- see
            // forms/fLotInfo.h GATE REGISTER WC-11 (guard `bFlag` depends
            // entirely on the WA-7-gated fBarCode->
            // JCETUseMakeWhite2DIDList()).
#if 0
            bool bFlag=false;
            bFlag=fBarCode->JCETUseMakeWhite2DIDList();
            if(bFlag==true && cbRunMode->Enabled==true)
            {
                cbRunMode->Items->Clear();
                cbRunMode->Items->Add("FT1");
                cbRunMode->Items->Add("FT2");
                cbRunMode->Items->Add("FT3");
                cbRunMode->Items->Add("FT4");
                cbRunMode->Items->Add("FT5");
                cbRunMode->Items->Add("FT6");
                cbRunMode->Items->Add("FT7");
                cbRunMode->Items->Add("FT8");
                cbRunMode->Items->Add("FT9");
            }
            edtSysOperatorID->Visible=true;
            lblOPID->Visible=true;
            bFlag=fBarCode->JCETUseMakeWhite2DIDList();
            labCusLotID->Visible=bFlag;
            edtCusLotID->Visible=bFlag;
#endif
        }
        else
        {
            edtSysOperatorID->Visible=false;
            lblOPID->Visible=false;
            labCusLotID->Visible=false;                                         //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能
            edtCusLotID->Visible=false;                                         //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能
        }
        labDeviceName->Visible=false;
        edtDevice->Visible=false;
        lbProcess->Visible=false;
        cbProcess->Visible=false;
    }
    else if(CUSTOMER_CODE==CC_SCC ||                                            //Steven 20250314 : add for JSCC
            CUSTOMER_CODE==CC_SJ_Semiconductor)
    {
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("P1");
        cbRunMode->Items->Add("P2");
        cbRunMode->Items->Add("P3");
        cbRunMode->Items->Add("P4");
        cbRunMode->Items->Add("P5");
        cbRunMode->Items->Add("P6");
        cbRunMode->Items->Add("P7");
        cbRunMode->Items->Add("P8");
        cbRunMode->Items->Add("P9");
        cbRunMode->Items->Add("P10");
        cbRunMode->Items->Add("P11");
        cbRunMode->Items->Add("P12");
        cbRunMode->Items->Add("P13");
        cbRunMode->Items->Add("P14");
        cbRunMode->Items->Add("P15");
        cbRunMode->Items->Add("RT1");
        cbRunMode->Items->Add("RT2");
        cbRunMode->Items->Add("RT3");
        cbRunMode->Items->Add("RT4");
        cbRunMode->Items->Add("RT5");
        cbRunMode->Items->Add("RT6");
        cbRunMode->Items->Add("RT7");
        cbRunMode->Items->Add("RT8");
        cbRunMode->Items->Add("RT9");
        cbRunMode->Items->Add("RT10");
        cbRunMode->Items->Add("RT11");
        cbRunMode->Items->Add("RT12");
        cbRunMode->Items->Add("RT13");
        cbRunMode->Items->Add("RT14");
        cbRunMode->Items->Add("RT15");
        cbRunMode->Text="P1";
    }
    else if(CUSTOMER_CODE==CC_PTI && IniConfig.bB03_TesterReport)               //Sam 20240809 : PTI ART 模式
    {
        lbProcess->Visible=true;
        cbProcess->Visible=true;
        cbProcess->Items->Clear();
        cbProcess->Items->Add("Sample");
        cbProcess->Items->Add("100%");
        cbProcess->Text="100%";

        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("1'st");
        cbRunMode->Items->Add("2'nd");
        cbRunMode->Items->Add("3'th");
        cbRunMode->Items->Add("4'th");
        cbRunMode->Items->Add("5'th");
        cbRunMode->Items->Add("6'th");
        cbRunMode->Items->Add("7'th");
        cbRunMode->Items->Add("8'th");
        cbRunMode->Items->Add("9'th");
        cbRunMode->Items->Add("10'th");
        cbRunMode->Items->Add("11'th");
        cbRunMode->Items->Add("12'th");
        cbRunMode->Items->Add("13'th");
        cbRunMode->Items->Add("14'th");
        cbRunMode->Items->Add("15'th");
        cbRunMode->Items->Add("16'th");
        cbRunMode->Items->Add("17'th");
        cbRunMode->Items->Add("18'th");
        cbRunMode->Items->Add("19'th");
        cbRunMode->Items->Add("20'th");
        cbRunMode->Text="1'st";
    }

    // -- S20 (golden :980-1013) --
    if(CUSTOMER_CODE==CC_GIGAS)
    {
        btnFtpTester->Visible=false;                                            //沒用到
        btnFTPTryConnect->Visible=true;                                         //測試FTP是否有連線(按鈕)
        lbFTPStatus->Visible=true;                                              //測試FTP是否有連線(狀態顯示)
        btnFtpServer->Caption ="Barcode Download";                              //Server->Barcode Download
        btnFtpHD->Caption="HD Upload";                                          //HD->HD Upload
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-28 -- see forms/fLotInfo.h
        // GATE REGISTER WC-28 (BtnPause is TfMainSpeedButton*, no ->Visible).
#if 0
        BtnPause->Visible=false;
#endif
        if(IniConfig.bEnableFTP==true)
        {
            TCheckBox *tempTCheckBox[17]=
            {
                chkContactHigh,chkContactForce,chkContactMode,checkbAutoClean,chkART,
                chkART_RTCount,chkCleanCount,chkHotPlate,chkLoadUnload,chkSpeedSetting,
                chkShuttleMode,chkTestMode,chkBinasgn,chkBinasgnOff,chkIndexHeatingMode,
                cbBottom2DOffset,chkAutoCleanContactHeight
            };
            for(int i=0;i<17;i++)
            {
                tempTCheckBox[i]->Visible=false;
                tempTCheckBox[i]->Checked=true;
            }
            grpMesCheck->Visible=false;
        }
        tsLotID->TabVisible=false;
    }
    else if(CUSTOMER_CODE==CC_KYEC_LEE)                                         //Ifor 20251028 add: KLT 要求不顯示畫面
    {
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-28 (2nd site) --
#if 0
        BtnPause->Visible=false;
#endif
    }
    else
    {
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-28 (3rd site) --
#if 0
        BtnPause->Visible=(CUSTOMER_CODE==CC_FOREHOPE_NINGBO);                  //Steven 20251125 : change
#endif
    }

    // -- S21 (golden :1014-1021) --
    tsASEMARMS->TabVisible=CosFunction.bUseARMSFunction;                        //Ifor 20170621 (wei) add ARMS Function
    tsASECLEventLog->TabVisible=(CUSTOMER_CODE==CC_ASE_CL ||
                                 CUSTOMER_CODE==CC_HANA_MICRON ||               //JimmyChiu 20211008 R211005-Hana-H9-01
                                 CUSTOMER_CODE==CC_SJ_Semiconductor ||          //Steven 20221128 : Add Socket Id for SJSM
                                 CosFunction.bUseSocketContactCount);

    pnlLotInfo_ASECL->Visible=(CUSTOMER_CODE==CC_ASE_CL);                       //Steven 20221214 : fixed for SJSM
    pnlLotStart_ASECL->Visible=(CUSTOMER_CODE==CC_ASE_CL);

    // -- S22 (golden :1023-1032) -- RefreshAMR/ShowAMRCategoryBin already Tier1 real (Wave B) --
    if(CosFunction.bEnableHandlerResultServer)
    {
        tsAMR->TabVisible=true;
        RefreshAMR();
        ShowAMRCategoryBin(true);
    }
    else
    {
        tsAMR->TabVisible=false;
    }

    // -- S23 (golden :1034-1043) -- edSocket/LastSet.strSocketID confirmed REAL --
    if(CUSTOMER_CODE==CC_ASE_CL)                                                //JerryYang 20250120 : add
    {
         for(int i=0; i<4; i++)
        {
            for(int j=0; j<8; j++)
            {
                edSocket[i][j]->Text=AnsiString(LastSet.strSocketID[i][j]);
            }
        }
    }

    // -- S24 (golden :1045-1067) --
    if(IniConfig.bEnableFTP)
    {
        if((CUSTOMER_CODE==CC_JSCC_OS))                                         //RogerYang 20260127 : Add For JSCC_OS 田揚志說LOTInfo擺第一頁
        {
            pgLotinfo->ActivePage=tsLotID;
        }
        else
        {
            pgLotinfo->ActivePage=tsFTP;
        }
    }
    else if(IniConfig.bEnableRms==true)
        pgLotinfo->ActivePage=tsDeviceInfo;
    else if(CosFunction.bOEEFunction)
        pgLotinfo->ActivePage=tsLotID;
    else if(REAL_TIME_CCD)
        pgLotinfo->ActivePage=tsRTCFullViewImg;
    else if(ATC_SYSTEM==eWinWay && Temperature.bATCActiveCooling==true)         //jimmychiu 20210906
        pgLotinfo->ActivePage=ATC_WinWay;
    else if(ATC_SYSTEM!=eATCUninstall && ATC_SYSTEM!=eNonChamber)
        pgLotinfo->ActivePage=tsATC;
    else if(Tri_Temp_Machine==1)                                                //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        pgLotinfo->ActivePage=ts_ATC6_1;

    // -- S25 (golden :1069-1118) -- WC-22 gates edtSysLotID->Width/->Left;
    // WC-23 gates labQACount->Font->Size=8 --
    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20190701 SPIL要求主畫面可切換EQC mode
    {
        lblPage->Visible=false;
        edPage->Visible=false;
        if(LastSet.iTester!=_2D_SORT && TestIF_File.b2DIDAllowList==false)      //JerryYang 20241104 : 支援2DID白名單功能  //JerryYang 20230322 : 2D sort lot info UI修改
        {
            palQAMode->Visible=true;
            labLotID->Caption="SPIL LOT ID-STAGE-STEP";
            // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-22 -- see
            // forms/fLotInfo.h GATE REGISTER WC-22 (edtSysLotID is a Wave
            // A/B member with no geometry field).
#if 0
            edtSysLotID->Width=150;
            edtSysLotID->Left=150;
#endif
            lbLotRunMode->Visible=false;
            cbRunMode->Visible=false;
        }
        else
        {
            if(TestIF_File.b2DIDAllowList==true)                                //JerryYang 20241104 : 支援2DID白名單功能
            {
                lbLotRunMode->Visible=true;
                cbRunMode->Visible=true;
                cbRunMode->Items->Clear();
                cbRunMode->Items->Add("Normal");
                cbRunMode->Items->Add("RT");
                cbRunMode->Items->Add("EQC");
                cbRunMode->Items->Add("CORR");
            }
            else
            {
                lbLotRunMode->Visible=true;
                cbRunMode->Visible=true;
            }

            labJobSeq->Visible=false;
            edtJobSeq->Visible=false;
        }
    }
    else if(CUSTOMER_CODE==CC_KYEC_LEE && IniConfig.bQAMode==true)
    {
        labQACount->Caption="EQC Mode Device Counts:";
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-23 -- see
        // forms/fLotInfo.h GATE REGISTER WC-23 (TLabel carries no Font
        // member).
#if 0
        labQACount->Font->Size=8;
#endif
        edQAMode->Left=136;
        btnQAmodeSave->Left=210;
        palQAMode->Visible=true;
    }
    else
    {
        palQAMode->Visible=false;
        labLotID->Caption="Lot ID : ";                                          //JerryYang 20220923 : add
    }

    // -- S26 (golden :1120) --
    ShowSocketID();                                                             //JerryYang 20190702 ASE-CL顯示SocketID

    // -- S27 (golden :1122-1123) -- DewPoint_Hardware_Install confirmed REAL --
    pan_DewPoint->Visible=(DewPoint_Hardware_Install>0);                        //Steven 20191017 : 露點計
    pl_DewPoint->Visible=(DewPoint_Hardware_Install>0);                         //Steven 20191017 : 露點計

    // -- S28 (golden :1125-1146) -- WC-18 gates the whole segment --
    // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-18 -- see forms/fLotInfo.h
    // GATE REGISTER WC-18 (myInShuttleLotInfo/mtBarcodeInShLotInfo are not
    // members of any ported TfLotInfo instance anywhere in this tree).
#if 0
    myInShuttleLotInfo->SetColorMap(0, TColor(0x00DFD9CC));
    myInShuttleLotInfo->SetColorMap(1, clGreen);
    myInShuttleLotInfo->SetColorMap(2, clRed);
    myInShuttleLotInfo->SetColorMap(3, clBtnFace);

    mtBarcodeInShLotInfo->SetColorMap(0, TColor(0x00DFD9CC));
    mtBarcodeInShLotInfo->SetColorMap(1, clGreen);
    mtBarcodeInShLotInfo->SetColorMap(2, clRed);
    mtBarcodeInShLotInfo->SetColorMap(3, clBtnFace);

    myInShuttleLotInfo->SetCellNumber(0, 0, "In Shuttle");
    myInShuttleLotInfo->SetCellColorIndex(0, 0, 3);
    myInShuttleLotInfo->SetCellNumber(0, 1, "a");
    myInShuttleLotInfo->SetCellColorIndex(0, 1, 3);
    myInShuttleLotInfo->SetCellNumber(0, 2, "b");
    myInShuttleLotInfo->SetCellColorIndex(0, 2, 3);
    myInShuttleLotInfo->SetCellNumber(0, 3, "c");

    mtBarcodeInShLotInfo->SetCellNumber(0, 0, "Shuttle1");
    mtBarcodeInShLotInfo->SetCellColorIndex(0, 0, 3);
    mtBarcodeInShLotInfo->SetCellNumber(1, 0, "Shuttle2");
    mtBarcodeInShLotInfo->SetCellColorIndex(1, 0, 3);
#endif

    // -- S29 (golden :1147-1154) --
    if(TestIF_File.i2DIDFormat==eAMD)                                           //JerryYang 20200422 2DID format選項改用下拉選單
    {
        grpBarcodeDisplayLotInfo->Visible=true;
    }
    else
    {
        grpBarcodeDisplayLotInfo->Visible=false;
    }

    // -- S30 (golden :1156-1158) -- WC-19 gates tmrChamberBoost->Enabled and
    // SetLotStart(); sPath (golden :1157) stays REAL between them -- see
    // forms/fLotInfo.h GATE REGISTER WC-19's own note on why. --
    // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-19 -- SAFETY RED LINE,
    // SetLotStart is the 399-line SECS Lot-Start main control function
    // (RECON #10); not re-evaluated.
#if 0
    tmrChamberBoost->Enabled=(CosFunction.bUseChamberBoostMode);                //Steven 20191128 : Chamber Boost Function
#endif
    AnsiString sPath=AuthPath+"config.ini";
#if 0
    SetLotStart("fLotInfo::FormShow", true);                                    //Steven 20250515 : 整合Open Short測試報表
#endif

    // -- S31 (golden :1160) --
    edtJobSeq->Text=ReadIniData(sPath, "Lot Info", "Job Sequence", AnsiString(""));                                     //JerryYang 20220923 : add

    // -- S32 (golden :1166-1192) -- WC-20 gates the whole segment (SAFETY) --
    // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-20 -- SAFETY RED LINE,
    // critical-para access-control + audit-flag cluster; not re-evaluated.
#if 0
    if(edtSysLotID->Text!="" ||
       (CosFunction.bLotStartLockCriticalPara &&                                //JerryYang 20220311 : ATP鎖定Critical parameter
        LastSet.iTester==ON_LINE &&
        (fMain->CheckCanChangeRealDummy()==false ||
         HasICUnderMachine())))
    {
        fMain->cbRunStartMode->Enabled=false;
        edtBarcodeRecipe->Enabled=false;                                        //Ifor 20241108 add:Barcode Multi Recipe   //Eastsun 20260527 整合 #027-2.MR.U1 edtBarcodeRecipe disable :KYEC

        if(CosFunction.bUseLogUploadToFTPFunction==true)                        //Steven 20240925 : Fixed for log uoload
        {
            bSysLotStart      =true;                                            //Ifor 20160302 add for KYEC_HS LotStart
            bEPLogStart_KYEC  =true;                                            //Ifor 20160302 add for KYEC_HS EPLogStart
            bTempLogStart_KYEC=true;                                            //Ifor 20160302 add for KYEC_HS TempLogStart
            if(USE_NOVX3360==true)
            {
                bESDLogStart_KYEC=true;                                         //Ifor 20160302 add for KYEC_HS ESD
            }
            asATCEvenLotID=fLotInfo->edtSysLotID->Text;                         //Ifor 20170124 (Steven) :add LotID By ATC Even Log
            bArmTestInfoEvenLogStart_KYEC=true;                                 //Ifor 20190912 :add 海思 V02.30 版 Record Torque
        }
    }

    if(CosFunction.bLotStartLockCriticalPara && RunInfo.bLotStart)              //JerryYang 20220311 : ATP鎖定Critical parameter
    {
        edQAMode->Enabled=!bAuthCriticalPara[14];
    }
#endif

    // -- S33 (golden :1194-1219) --
    if(CUSTOMER_CODE==CC_ASE_SG)
    {
        btnClearTemperature->Visible=true;
    }
    else
    {
        btnClearTemperature->Visible=false;
    }

    if(CUSTOMER_CODE==CC_LEADYO)                                                //KenHsieh 20230406 : 新增OCR Data + Bin Log功能
    {
        lblPage->Visible=false;
        edPage->Visible =false;
        Panel28->Visible=false;
    }

    if(CosFunction.bFirstTrayCheckOnUnloader)                                   //Jimmychiu 20251205 : First Tray Check On Unloader
    {
        tsOtherTool->TabVisible=true;
        cbFirstTrayCheckOnUnloader->Visible=true;
    }
    else
    {
        tsOtherTool->TabVisible=false;
        cbFirstTrayCheckOnUnloader->Visible=false;
    }
    // -- connective (golden :1220-1221) -- see forms/fLotInfo.h's note on the
    // S33/S34 boundary (2 real lines recon's own segment table doesn't cover) --
    lbShowDevName->Visible=CosFunction.bScanBarcodeAndDownloadFileInRMS;
    lbShowDevName->Caption="";

    // -- S34 (golden :1223-1229) -- WC-16 gates ReadWriteFTPAutomationData() --
    if(iAQLBin==0)                                                              //Eastsun 20260520 整合
        iAQLBin=-1;                                                             //Eastsun 20260520 整合
    fLotInfo->lbLotAQLSetCount->Caption=IntToStr(iAQLCount);                    //Eastsun 20260520 整合
    fLotInfo->lbLotAQLSetBin->Caption=IntToStr(iAQLBin);                        //Eastsun 20260520 整合

    // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-16 -- see forms/fLotInfo.h
    // GATE REGISTER WC-16 (ReadWriteFTPAutomationData has zero port anywhere
    // in this tree, overturning recon's own S34 note).
#if 0
    ReadWriteFTPAutomationData(true);                                           //KaiChen 20190530 ：Sigurd FTP Automation
#endif
    AdjtsYieldMonitiorSize();                                                   //Steven 20221225 : 統一Lot Info尺寸調整
    InitialRefrigerantSystem();                                                 //Ztex 2023.04.19 Add HT-1032 TriTemp Function

    // -- S35 (golden :1231-1233) -- WC-17 gates FrmAOI->AOIFailCountRefresh() --
    // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-17 -- see forms/fLotInfo.h
    // GATE REGISTER WC-17 (FrmAOI carries only bSimulateTopBtm, no
    // AOIFailCountRefresh() method).
#if 0
    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall && IniConfig.bA74AOIFailCountLinkLotRunMode)
        FrmAOI->AOIFailCountRefresh();                 //Eastsun 20260408 :
#endif
}

// -- Timer2Timer (golden uLotInfo.cpp:6934-7191) -----------------------------
void TfLotInfo::Timer2Timer()
{
    // -- T1 (golden :6936-6939) -- iTempSetTimer moved into the WC-24 gate --
    static bool bOldFTPAutomation=false;
    if(InitialOK==false)                                                        //Steven 20160912 : Add InitialOK in Timer
        return;

    // -- T2 (golden :6941-6945) --
    if(CUSTOMER_CODE==CC_TSMC_TAINAN && bSecsGemDownloadFTP==true)              //wei 20170119 (Steven) DownLoad 沒有馬上按掉會Time Out
    {
        if(IniConfig.bEnable_SECS_GEM==true)
            ShowMyMessage("DOWNLOAD_RECIPE_BY_EA Finish!!");
    }

    // -- T3 (golden :6947-6950) --
    if(CUSTOMER_CODE==CC_KYEC_XILINX)                                           //Frank 20171030 (Steven) add Clear Barcode List新增權限 Xilinx
    {
        btClearBarcodeList->Visible=(AccessLevel>=iDefHonPrecLevel)?true:false;
    }

    // -- T4 (golden :6952-6956) -- fAGV->IsSPIL_AMR() confirmed REAL, step 0 --
    if(fAGV->IsSPIL_AMR())
    {
        labLoaderBundleID->Caption=asBundleTrayID[ePortLoader];
        lblLoaderCarBundleID->Caption=asBundleTrayID[ePortEmpty];
    }

    // -- T5 (golden :6958-6985) --
    bSecsGemDownloadFTP=false;

    if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction==true && bOCRConnectTest==true)                                 //wei 20160613 ocr連線測試
    {
        bOCRConnectTestCount++;

        if(bOCRConnectTestCount>3)                                              //wei 20161028 ocr連線測試
        {
            if(bOCRConnectOK)
            {
                ShowMyMessage("OCR Connect Test OK");
                bSignIn=true;
                bOCRConnect=true;
            }
            else
            {
                ShowMyMessage("OCR Connect Test NG");
                bSignIn=false;
                bOCRConnect=false;
            }
            bOCRConnectTest=false;
            bOCRConnectTestCount=0;
        }
        else
        {
            return;
        }
    }

    // -- T6 (golden :6987-7016) -- fMain->cbSetupFileName confirmed REAL --
    lbOCRUseFile->Caption="";
    lbOCRNowFile->Caption="";

    lblAutoCount->Caption=iUnloaderTrayCountCal[0];
    lblAutoCount2->Caption=iUnloaderTrayCountCal[1];
    lblAutoCount3->Caption=iUnloaderTrayCountCal[2];

    if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction==true)
    {
        if(bSignIn)
        {
            Label41->Caption="Log In";
        }
        else
        {
            Label41->Caption="Log Out";
        }
        lbOCRUseFile->Caption=fMain->cbSetupFileName->Text;
        if(asCheckFileName!="")
            lbOCRNowFile->Caption=asCheckFileName;
        else
            lbOCRNowFile->Caption="";
    }
    else
    {
        Label41->Caption="Log Out";
        lbOCRUseFile->Caption=fMain->cbSetupFileName->Text;
        lbOCRNowFile->Caption="";
    }

    // -- T7 (golden :7017-7029) -- WC-26/WC-27 gate the fOCR/fSetup lines --
    if(bGetLotIDFormTester)
    {
        fLotInfo->palHandlerwithTester->Caption="Connection";
        fLotInfo->palHandlerwithTester->Color=clLime;
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-26 -- see forms/fLotInfo.h
        // GATE REGISTER WC-26 (fOCR->sTesterLotId lives only on OCRInsp.cpp's
        // TU-local shim, unreachable here -- same TfOCR shape as WB-6, a
        // different absent member).
#if 0
        lblTester_LotID->Caption= fOCR->sTesterLotId;
#endif
    }
    else
    {
        fLotInfo->palHandlerwithTester->Caption="No Connection";
        fLotInfo->palHandlerwithTester->Color=clRed;
        // AI(W906-FW3-Setup-WA) 20260820: WC-27 RETIRED -- the cSetUp display
        // wave landed the real fSetup->edOcrText (forms/fSetup.h:271), so the
        // gate's cause is gone and golden :7027 is live again. History in
        // forms/fLotInfo.h's GATE REGISTER.
        lblTester_LotID->Caption=fSetup->edOcrText->Text;
    }

    // -- T8 (golden :7030-7034) --
    ts_AutoRetestMonitor->TabVisible=(CosFunction.bUseSCKART==false &&
                                      USE_AUTO_RETEST==eartInstall &&
                                      ((IniConfig.bA10_AutoReTest && (CosFunction.bAutoRetestGPIBmode==false || CUSTOMER_CODE==CC_KYEC_XILINX)) || bAutoReTest_ART));  //kevin 20150601);       //wei 20150331 打開功能就顯示        //Steven 20161201 : For SCK 93K ART  //Frank 20161212 (Jou) For Xilinx 打開功能顯示
    tsOCRBarCode        ->TabVisible=(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR);                               //wei 20150720  打開功能就顯示

    // -- T9 (golden :7035-7085) -- WC-24 SAFETY GATE (WinWay ATC hardware) --
    if(ATC_SYSTEM==eWinWay)                                                     //jimmychiu 2021
        ATC_WinWay->TabVisible=true;
    else
        ATC_WinWay->TabVisible=false;

    // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-24 -- SAFETY RED LINE, WinWay
    // ATC hardware temperature/comm control; not re-evaluated, gated
    // verbatim per the task brief. iTempSetTimer (golden :6937) moved here
    // since this gated block is its only use.
#if 0
    static int iTempSetTimer=0;                                                 //jimmychiu 2021
    if(ATC_SYSTEM==eWinWay && Temperature.bATCActiveCooling==true)
    {
        fWinway->OpenCommPort();

        double dbGetAtcSetTemp=0.0;
        double dbSetATCTemp=0.0;
        bool bHasErr=false;

        if(LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot)
            dbSetATCTemp=Temperature.fWorkTemperBase;
        else
            dbSetATCTemp=IniConfig.dATCAmbientTemperature;
        iTempSetTimer++;
        for(int i=0;i<4;i++)
        {
            dbGetAtcSetTemp=fWinway->arrATC_Site[i]->GetST();
            if(dbSetATCTemp!=dbGetAtcSetTemp || iTempSetTimer>=3)
            {
                fWinway->SetTempratureAll(dbSetATCTemp);
                iTempSetTimer=0;
            }

            if(SystemStart==true && fWinway->arrATC_Site[i]->iWinWaySendCount >=6)
            {
                fWinway->arrATC_Site[i]->WinwayCOM->StopComm();
                fWinway->arrATC_Site[i]->bCommConnect=false;
                fWinway->arrATC_Site[i]->SetPT((double)9999);
                fWinway->arrATC_Site[i]->iWinWaySendCount=0;
                bHasErr=true;
            }

            ATCPtrWinWay[i]->Caption=FormatFloat(L"0.0" ,fWinway->arrATC_Site[i]->GetPT_NoCommand()) ;
        }

        if(bHasErr)
        {
            ShowMyMessage("WinWay ATC System Connect Error!");
        }
    }
#endif

    // -- T10 (golden :7087-7094) --
    if(CUSTOMER_CODE==CC_GIGAS ||                                               //Isaac 20210129 : FTP頁面，關閉功能要不顯示畫面，但視窗不要關掉
       IniConfig.bVTESTFunction==true)
    {
    }
    else
    {
        tsFTP               ->TabVisible=IniConfig.bEnableFTP;                  //Ifor 20180822 : Add 關閉ftp功能關閉顯示頁面
    }

    // -- T11/T12 (golden :7096-7123) -- WC-25 SAFETY GATE (WAR16123 3hr alarm);
    // T12 is this same if-statement's `else` arm, kept REAL --
    if(IniConfig.bVTESTFunction==true)
    {
        // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-25 -- SAFETY RED LINE,
        // WAR16123 3-hour send-for-inspection alarm; not re-evaluated, gated
        // verbatim per the task brief.
#if 0
        if(IniConfig.bEnableRms)
        {
            fLotInfo->Panel29->Caption="Server";
        }

        if(RunInfo.bLotStart==true &&                                           //RogerYang 20260424 : 陳永恆說不再需要，等趙坤鵬同意  //jou 20230203 : 無錫偉測 機台添加三小時送檢報警，從lot start時間開始計算
           SystemStart==true && iHome==0 &&
           fNote->fShow==false && MyMessageBox->fShow==false )
        {
            iProduceTimeCT++;
            if(iProduceTimeCT>=(3600*3))
            {
                int ret=ShowErrorMessage("WAR16123", K_SKIP , MMSystem);        //RogerYang 20251030 : 袁林要把Retry拿掉
                if(ret==K_RETRY)
                    iProduceTimeCT=9000;
                else
                    iProduceTimeCT=0;
            }
        }

        labConfigL04->Caption="[L04] Temperature range : " + AnsiString(IniConfig.iL04TemptureRange);
#endif
    }
    else
    {
        tsDeviceInfo        ->TabVisible=IniConfig.bEnableRms;                  //Ifor 20181029 : Add 關閉RMS功能關閉顯示頁面
    }

    // -- T13 (golden :7124-7125) --
    tsTesterLog         ->TabVisible=(TestIF_File.iTestType==TCP_IP_MODE);
    grpBarcodeDisplayLotInfo->Visible=(TestIF_File.i2DIDFormat==eAMD);          //JerryYang 20200422 2DID format選項改用下拉選單 //Ifor 20200422 :add 2D主畫面顯示

    // -- T14 (golden :7127-7149) -- WC-8 gates RefreshYieldMonitor() (2nd site) --
    ts_FTPAutomation    ->TabVisible=IniConfig.bA32EnableFTPAutomation;
    if(bOldFTPAutomation!=ts_FTPAutomation->TabVisible)
    {
        bOldFTPAutomation=ts_FTPAutomation->TabVisible;
        if(ts_FTPAutomation->TabVisible)
        {
            pgLotinfo->ActivePage=ts_FTPAutomation;
            fLotInfo->ShowInformation(false);
            fLotInfo->Height=150;
            fLotInfo->Width=150;
            sbTest->Top=50;
        }
        else
        {
            pgLotinfo->ActivePage=tsYieldMonitior;
            // AI(W906-FW3-LotInfo-WC) 20260819: GATE WC-8 -- see
            // forms/fLotInfo.h GATE REGISTER WC-8 (2nd call site).
#if 0
            RefreshYieldMonitor();
#endif
        }
    }
    else
    {
        AdjtsYieldMonitiorSize();                                               //Steven 20221225 : 統一Lot Info尺寸調整
    }

    // -- T15 (golden :7152-7164) --
    if(TestIF_File.bEnableBarCode==true &&
       BAR_CODE_INSTALL==ebctUseCCDMode &&
       TestIF_File.bBarCodeMultiRecipe==true)
    {
        labBarcodeRecipe->Visible=true;
        edtBarcodeRecipe->Visible=true;
    }
    else
    {
        labBarcodeRecipe->Visible=false;
        edtBarcodeRecipe->Visible=false;
    }

    // -- T16 (golden :7166-7187) --
    spOCRCleanList->Visible=(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR && IniConfig.bCompareOCRData);           //KenHsieh 20220825 : 新增OCR比對功能
    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20241104 : 支援2DID白名單功能
    {
        if(TestIF_File.b2DIDAllowList)
        {
            if(RunInfo.bLotStart==false)
            {
                if(AccessLevel>=iDefEngineerLevel ||
                   SPIL_FOR_QLE==1)                                             // KevinC 20250912 渠梁OP權限
                {
                    cbRunMode->Enabled=true;
                }
                else
                {
                    cbRunMode->Enabled=false;
                }
            }
        }
        else
        {
           cbRunMode->Enabled=true;
        }
    }

    // -- T17 (golden :7189) --
    ScanRefrigerantSystem();                                                    //Ztex 2023.04.19 Add HT-1032 TriTemp Function

    // -- T18 (golden :7190) --
    fLotInfo->palAQLMode->Visible=IniConfig.bI52_bAQLSortMode;                  //Eastsun 20260520 整合//Ifor 20210713 add: AQL Sor tMode
}

TfLotInfo *fLotInfo = new TfLotInfo();
