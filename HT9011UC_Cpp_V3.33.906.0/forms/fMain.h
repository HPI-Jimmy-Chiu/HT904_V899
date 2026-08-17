// =============================================================================
//  forms/fMain.h  --  non-VCL stand-in for golden's fMain form pointer
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.h by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Members and comments moved
//  VERBATIM apart from the F0-e virtualisation and the widget types now coming
//  from forms/FormWidgets.h.
//
// =============================================================================
//  ############  THE FACADE CONTRACT -- BINDING ON EVERY forms/ FILE  ########
// =============================================================================
//  (Written into THIS file per plan SS6-F0-e, because fMain is the form every
//   later wave reads first.  It applies to all 9 forms in this directory.)
//
//  1. METHODS ARE `virtual`.  Their bodies here are offline no-ops / safe
//     defaults, and those bodies are the PERMANENT OFFLINE IMPLEMENTATION --
//     not scaffolding to be deleted.  When the MFC layer lands (plan SS2-D3),
//     a `TfXxxImpl : public TfXxx` binder held BY COMPOSITION inside the
//     CDialog overrides them and `fXxx` is repointed at that impl.  The
//     headless ctest suite keeps constructing the plain TfXxx and keeps
//     getting these bodies, so the offline oracle never depends on MFC.
//
//  2. DATA MEMBERS ARE CONCRETE STORAGE AND MUST **NEVER** BECOME ACCESSORS.
//     This is not a style preference, it is arithmetic: 69.3% of golden's
//     4,266 `fMain->` dereferences are data-member syntax
//     (`fMain->cbSetupFileName->Text`), C++ has no BCB6 `__property`, and
//     there are ~14,432 cross-module dereferences tree-wide.  Turning members
//     into Get/Set would mean rewriting all of them.  The MFC side keeps these
//     fields fresh with DDX (which is exactly what DDX is for) instead.
//     A "pure virtual interface" facade is therefore IMPOSSIBLE here, and that
//     question is closed (plan SS2-D2).
//
//  3. WIDGET TYPES COME FROM forms/FormWidgets.h -> vclcompat/Controls.h.
//     A wave that needs a new MEMBER adds it to its own forms/fXxx.h.  A wave
//     that thinks it needs a new widget TYPE is almost certainly wrong: use an
//     existing vclcompat/Controls.h stock type, or -- for the 3 custom control
//     families (tray / btn-panel / LED) -- vclcompat/{TrayCore,BtnPanelCore,
//     LedCore}.  Do NOT define another `struct {AnsiString Text;}`.
//
//  4. EVERY MEMBER CARRIES ITS GOLDEN HOME, tagged [DATA] or [METHOD].  Out-of-
//     scope bodies stay documented no-op stubs (never silently "implemented"),
//     and where a test needs to feed a gated leaf, use the established
//     `W906_..._Sim` settable-seam idiom (PERSITETemperatureStrings below is
//     the reference example) rather than a bare no-op -- a bare no-op makes the
//     gap invisible to the suite.
//
//  5. FILE OWNERSHIP.  One form per file means two waves touching two different
//     forms no longer collide.  Two waves touching the SAME form still have to
//     serialise.  forms/FormWidgets.h should need no edits at all.
// =============================================================================
//
//  Original translation waves for the content below: W6.0 (scaffold) / W6.2
//  (in-arm HP geometry) / W6.3 (tray-arm) / W6.4 (tester) / W6.5 (shuttle) /
//  W6.6 (hub) / W7-C1 / W5-comms / W5-Automation / W5-Final-SckArtRemainder /
//  W906-Automation / W906-TesterTCPTimer / W906-AutoCleanFoundation /
//  W906-AutoCleanCluster / W906-AutoSiteMapCleanOut.
//
//  W6 DECOUPLING STRATEGY (form-pointer cut), from the original file head:
//  The BCB6 state machines reach UI/automation state through global VCL TForm
//  pointers (fMain / fAGV / fSortCT / fLotInfo / fOffSet / fSCKART / ...).
//  Those forms cannot be pulled into the portable build (they derive from VCL
//  TForm).  Instead we expose ONLY the members a given sub-wave's SM actually
//  dereferences, on a non-VCL facade, and grow it per sub-wave.
//  The sbStateRecordClick(sbStateRecord) line in CheckHasSpaceToPlace_9045 is
//  gated #if 0, so TSpeedButton/TObject are NOT pulled in and those two TfMain
//  members are intentionally NOT added.  cb1 is referenced only inside
//  #ifdef SOFT_SIMULTE (undefined) so it is not added either.
// =============================================================================
#ifndef FORMS_FMAIN_H
#define FORMS_FMAIN_H

#include "forms/FormWidgets.h"

// ---------------------------------------------------------------------------
//  W6.3: TfMainHanaART -- fMain->hanaART (golden main.h, HANA ART helper).  The
//  catchtray count helpers (AddFixTrayCount/AddTrayCount) call exactly two
//  methods on it; offline (no HANA link) IsHanaArtAvailable() is false so the
//  count helpers early-out, and AddNewTrayHead is a no-op.
// ---------------------------------------------------------------------------
class TfMainHanaART
{
public:
    virtual bool IsHanaArtAvailable();      // [METHOD] golden -- offline: false
    virtual void AddNewTrayHead(int iAuto); // [METHOD] golden -- offline: no-op
    virtual ~TfMainHanaART() {}
};

// ---------------------------------------------------------------------------
//  W6.5: TfMainInplace -- golden cInplace (InArmPlacementEnable()/
//  iNo9ShtErrICCt/bNo9ShtErrNo).  Offline InArmPlacementEnable()==false makes
//  the No9 sub-blocks inert (the AutoLatch checks still run their main path).
// ---------------------------------------------------------------------------
class TfMainInplace
{
public:
    int  iNo9ShtErrICCt[2];          // [DATA]   golden cInplace -- per-shuttle No9 err count
    bool bNo9ShtErrNo[2][8];         // [DATA]   golden cInplace -- per-site No9 err flag
    // W7-A1 ADD: the In-Sht-Latch / No9 combine flag the floating + latch SMs touch
    // (golden cInArmPlacement.h:63, KenHsieh 20251105).  Golden-faithful default is
    // FALSE: DoInArmCheckShuttleFloating case 9000 SETS bNo9Action=false (golden
    // ainarm9045.cpp:3744-ctx) and NO offline path sets it true (the only true-set
    // sites live inside InArmPlacementEnable()==false No9 placement blocks, dead
    // offline).  The latch reads (golden :3941/:4045, via W7A1_NO9_ACTION()) are all
    // guarded by InArmPlacementEnable() && bNo9Action, so an always-false member is
    // behaviorally identical to the offline (false) macro.  No other TU references it.
    bool bNo9Action;                 // [DATA]   golden cInArmPlacement.h:63 -- default false
    virtual bool InArmPlacementEnable();     // [METHOD] golden cInplace -- offline: false (No9 inert)
    TfMainInplace();
    virtual ~TfMainInplace() {}
};

// ---------------------------------------------------------------------------
// AI(W906-W7-L1-Wave0) 20260801: TfMainRENESASServer -- stand-in for
// fMain->RENESAS_Server, added because golden asendic_Loader.cpp derefs it on
// the loader tray-supply path (:2644 writes bLoadingCountFullFlag=false, :2722
// and :2756 call DoNeedSupplyOrNot(true/false)).  Golden home:
// main.h:1710 `TRENESAS_Server *RENESAS_Server;` (Kirin 20191213), whose class
// declares `bool bLoadingCountFullFlag;` at Automation/uRENESAS_Server.h:199
// and `bool DoNeedSupplyOrNot(bool bNotEnough);` at :201 -- both re-read from
// the cp950-decoded golden this wave.  Same nested-helper-class idiom as
// TfMainHanaART / TfMainInplace above.  Offline DoNeedSupplyOrNot() returns
// false: there is no RENESAS FT-CT server link, so "no extra supply is being
// demanded" is the faithful quiet default and keeps the two golden call sites
// on their non-supply arm.
// ---------------------------------------------------------------------------
class TfMainRENESASServer
{
public:
    bool bLoadingCountFullFlag;                     // [DATA]   golden Automation/uRENESAS_Server.h:199
    virtual bool DoNeedSupplyOrNot(bool bNotEnough);// [METHOD] golden Automation/uRENESAS_Server.h:201 -- offline: false
    TfMainRENESASServer();
    virtual ~TfMainRENESASServer() {}
};

// ===========================================================================
//  TfMain -- non-VCL stub (golden main.h, TfMain:public TForm)
// ===========================================================================
class TfMain
{
public:
    virtual void DebugOneCycleHotPlate(AnsiString sfunc); // [METHOD] golden main.h:1565 -- debug log sink (empty)
    virtual bool Pause(AnsiString Func);                  // [METHOD] golden main.h:1249 -- offline never pauses -> false
    // AI(W906-W7-F1fix2) 20260729: two seams on the PRE-EXISTING Pause() above,
    //   added because Pause() is a bare `return false;` with no side effect, so
    //   NOTHING that forwards INTO it can be observed by a test. Specifically
    //   BtnPauseClick's `Pause("BtnPauseClick")` forward (golden main.cpp:6967)
    //   was still unobservable after W7-F1fix added W906_BtnPauseClickCallCount:
    //   that counter proves BtnPauseClick was CALLED, not that it FORWARDED --
    //   deleting the forward left every assertion green (personally reproduced:
    //   see tests/test_w7_f1_wall2_probe.cpp's BtnPauseClick block). These two
    //   make the forward itself, AND the Func argument golden passes, testable.
    //   Purely additive: Pause() still returns false unconditionally, so the
    //   ~40 SM call sites that already call fMain->Pause(...) are unaffected.
    int        W906_PauseCallCount;                       // [PORT-ONLY SEAM] call counter, default 0
    AnsiString W906_PauseLastFunc;                        // [PORT-ONLY SEAM] last Func argument seen, default ""
    virtual void ShowTestHeadComp(bool bRefresh);         // [METHOD] golden main.h:1296 -- empty
    virtual void ReStartAutoSiteMapping(bool bStart);     // [METHOD] golden main.h:1331 -- empty
    // AI(W906-AutoSiteMapCleanOut) 20260727: golden main.h:1334 __fastcall
    // SetMainRunStartMode(int) -- documented GAP no-op stub, see forms/fMain.cpp
    // for the full citation (golden main.cpp:28236-28308 cascades into
    // UpdateMainOperateMode() golden main.cpp:12803-13127 + a new fBinSel VCL
    // form, both out of scope that wave). Added so csystem.cpp's
    // InitCleanOutFunction AutoSiteMap branch (golden csystem.cpp:15751-15785)
    // can be un-gated without pulling in that chain.
    virtual void SetMainRunStartMode(int iSetMode);       // [METHOD] golden main.h:1334 -- documented GAP stub (no-op)
    TfMainSiteMapLog *slAutoSiteMapLog;           // [DATA]   golden main.h:1486 (TMyStringList*) -- new in ctor
    // -- W6.3 ADD: members the TRAY-ARM ENGINE (acatchtray.cpp) derefs ----------
    virtual void CleanOut(AnsiString Func);               // [METHOD] golden main.h -- offline: clean-out no-op
    virtual void DoStateRecord(int i, bool b);            // [METHOD] golden main.h -- offline: state-record sink no-op
    TfMainTrayPanel *mtAuto1;                     // [DATA]   golden main.h:168 (TTMyTray* mtAuto1 -- see forms/FormWidgets.h's TfMainTrayPanel conflation note; only ->Color assigned)
    TfMainTrayPanel *mtAuto2;                     // [DATA]   golden main.h:169 (TTMyTray* mtAuto2)
    TfMainTrayPanel *mtAuto3;                     // [DATA]   golden main.h:170 (TTMyTray* mtAuto3)
    TfMainHanaART   *hanaART;                     // [DATA]   golden main.h (HANA ART helper)
    // -- W6.4 ADD: members the TESTER/INDEX ENGINE (atester.cpp) derefs ----------
    virtual void LightOn();                               // [METHOD] golden main.h -- offline: CCD light no-op (DoTestHeadMotor CCD path)
    TfMainTrayPanel *lbCCDStatus;                 // [DATA]   golden main.h:672 (TLabel* lbCCDStatus) -- ->Visible written by atester.cpp:1894
    // -- W6.5 ADD: members the SHUTTLE ENGINE (acarry.cpp) derefs ----------------
    //    * TfMainCheckBox -- golden TCheckBox* (cbShowShuttleSensor /
    //      cbTestOutShuttleSensor / cbShowInShuttleSensor).  Engine reads
    //      .Checked (offline false -> the debug-log / sensor-confirm paths are
    //      skipped, the proven DUMMY posture).
    //    * TfMainGrid -- golden TTMyTray* (htShullte0 / htShullte1, golden
    //      main.h:345-346; the "THeatTable*" label this comment used to carry was
    //      wrong -- corrected by W7-F0, see forms/FormWidgets.h).  Engine calls
    //      SetCellColorIndex(col,row,idx) only inside the Motor-View page guard
    //      (pgMain->ActivePageIndex==emp7TabSheet21, both 0 offline so equal --
    //      but SetCellColorIndex is a harmless no-op so it is safe either way).
    //    * TfMainMemo / TfMainMemoLines -- golden TMemo* (meShuttle1/meShuttle2,
    //      golden main.h:541-542).  Engine calls ->Lines->Add / ->Lines->Count /
    //      ->Clear inside the cbShowInShuttleSensor.Checked guard (false offline)
    //      -> no-op.
    TfMainCheckBox *cbShowShuttleSensor;          // [DATA]   golden main.h:540 (TCheckBox*) -- offline Checked=false
    TfMainCheckBox *cbTestOutShuttleSensor;       // [DATA]   golden main.h:543 (TCheckBox*) -- offline Checked=false
    TfMainCheckBox *cbShowInShuttleSensor;        // [DATA]   golden main.h:910 (TCheckBox*) -- offline Checked=false
    TfMainGrid     *htShullte0;                   // [DATA]   golden main.h:345 (TTMyTray* shuttle-1 grid)
    TfMainGrid     *htShullte1;                   // [DATA]   golden main.h:346 (TTMyTray* shuttle-2 grid)
    TfMainMemo     *meShuttle1;                   // [DATA]   golden main.h:542 (TMemo* shuttle-1 log)
    TfMainMemo     *meShuttle2;                   // [DATA]   golden main.h:541 (TMemo* shuttle-2 log) -- AI(W906-F0fix) 20260728: corrected from wrong ":543" (that line is TCheckBox *cbTestOutShuttleSensor, already cited at :157), re-verified against golden main.h via cp950 read
    TfMainInplace  *cInplace;                     // [DATA]   golden main.h (cInplace placement helper)
    TfMainPageControl *pgMain;                    // [DATA]   golden main.h:64 (TPageControl*) -- ActivePageIndex==0 offline
    int emp7TabSheet21;                           // [DATA]   golden main.h (Motor-View tab index) -- 0 offline (==pgMain->ActivePageIndex)
    virtual void AddShuttleMessage(int iSht, AnsiString S);     // [METHOD] golden main.h -- offline log sink no-op
    virtual void Reset(AnsiString Func);                       // [METHOD] golden main.h -- offline: no-op
    virtual void BtnOneCycleClick(void *Sender);               // [METHOD] golden main.h -- offline: no-op
    virtual void BtnResetClick(void *Sender);                  // [METHOD] golden main.h -- offline: no-op
    virtual void BtnCleanOutClick(void *Sender);               // [METHOD] golden main.h -- W6.2b(2x4_16): clean-out button; offline no-op
    virtual void JSCC_ResetForShuttleLoseIC();                 // [METHOD] golden main.h -- offline: no-op (0-arg)
    virtual void ResetRecordforPiggyBack(AnsiString S);        // [METHOD] golden main.h -- offline: no-op
    // -- W6.6 ADD: the per-tick sensor scan the HUB main loop calls -------------
    virtual void ProcessSensorScan();                          // [METHOD] golden main.h -- DoAllProcess():9130 every tick; offline no-op
    // -- W7-C1 ADD: members the END-OF-LOT CLEAN-OUT FINISH-CHECK (DoCleanOutFinishCheck)
    //    derefs.  golden main.h.  All offline no-op / sane default (a handler that
    //    is draining clean-out with no UI never re-starts/re-levels/re-tests).
    //    AI(W7C1-Integrate) 20260629.
    virtual void Start(AnsiString Func);                       // [METHOD] golden main.h -- offline: do NOT auto re-start (no-op); ~6 sites
    virtual void ChangeLevelAttr();                            // [METHOD] golden main.h -- offline: level-attr UI no-op
    virtual void ModifyTester(int iWhich);                     // [METHOD] golden main.h:Steven 20191218 -- offline: QA tester-modify no-op
    virtual void CleanYieldCount();                            // [METHOD] golden main.h -- offline: yield-count clear no-op
    TfMainSpeedButton *BtnOneCycle;                           // [DATA]   golden main.h:72 (TBtnPanel* BtnOneCycle -- NOT TSpeedButton, corrected by W7-F0; see forms/FormWidgets.h); offline Down=false
    // -- W5-comms INTEGRATE ADD: members Interface/InterfaceSYS.cpp derefs ------
    //    (the WM_COPYDATA IPC bridge to the ESD / Auto-Update / Event-Log-
    //    Analyzer helper programs).  golden main.h:1216/1217/1218/1220.  Offline
    //    default NULL/0 preserves golden's own "no window found yet" safe path
    //    (FindWindow is called lazily at each send site, matching golden).
    //    AI(W5-comms-Integrate) 20260710.
    HWND HESDWnd;                                 // [DATA] golden main.h:1216 (HWND) -- offline NULL
    HWND HEventLogWnd;                            // [DATA] golden main.h:1217 (HWND) -- offline NULL
    HWND HAutoUpdateWnd;                          // [DATA] golden main.h:1218 (HWND) -- offline NULL
    int  oldGpibAddress;                          // [DATA] golden main.h:1220 (int)  -- offline 0
    // -- W5-Automation INTEGRATE ADD: members Automation/HANA_ART.cpp derefs -----
    //    (the HANA-ART tester-side SRQ helper).  golden main.h:1400/1401/1531/
    //    1532/1370.  Offline: no real GPIB-bridge process / no SamSung-specific
    //    map or soak-time source / no arm-status telemetry sink.
    virtual void SendMSG_CMD(int CMD);                         // [METHOD] golden main.h:1400 -- offline no-op
    virtual void SendMSG_CMD(int CMD, AnsiString Message);     // [METHOD] golden main.h:1401 -- offline no-op
    virtual AnsiString GetSamSungMap(bool bSend=true);         // [METHOD] golden main.h:1531 (body Command.cpp:10137) -- offline: ""
    virtual AnsiString GetSamSungSoakTime(bool bSend=true);    // [METHOD] golden main.h:1532 (body Command.cpp:10305) -- offline: "0"
    virtual AnsiString ArmStatusStrings();                     // [METHOD] golden main.h:1370 (body Command.cpp:1497) -- offline: ""
    // -- W5-Final-SckArtRemainder INTEGRATE ADD: members Automation/SCK_ART_Remainder.cpp
    //    (SckArtRem_AccessFile) derefs -- golden main.h, bodies in main.cpp (untranslated).
    //    All 3 are UI-refresh-only in golden (recipe combo/test-mode picture/backup-on-write);
    //    offline no-op, matching every other fMain UI-refresh sink above.
    virtual void SetStartModeData();                           // [METHOD] golden main.h -- offline: recipe start-mode UI refresh no-op
    virtual void LoadTestModePicture();                        // [METHOD] golden main.h -- offline: test-mode picture UI refresh no-op
    virtual void BackupSetupFile();                            // [METHOD] golden main.h -- offline: setup-file backup no-op (Ifor 20170620)
    // -- W5-Automation ADD (AGV_PortScan unit, 20260713): members
    //    Automation/AGV_PortScan.cpp derefs (AMR SPIL port-scan LED +
    //    E84 loader/unloader tray-count scan's SECS-link panel) -----------------
    TfLedValue *ALed1;                                        // [DATA] golden main.h:355 (TALed*) -- bScanLoadPortState_SPIL reads ->Value
    TfMainPanel *labAutomation;                               // [DATA] golden main.h:802 (TPanel*) -- DoE84LoaderScan/DoE84UnloaderScan compare ->Caption
    // -- W906-Automation ADD (20260716): members Automation/automation.cpp
    //    derefs (GetMachineStatus/GetWorkOrder/GetMainTemp + the deferred
    //    ProcessBuffer's own fMain->Home("TfAutomation::ProcessBuffer") call,
    //    golden automation.cpp:1522 -- ProcessBuffer itself is GATED in that
    //    wave, see Automation/automation.h, but Home() is added per that
    //    front's task brief as a small additive cross-file gap). Same
    //    Caption/Text-stub shape already used elsewhere --
    //    palMainStatus reuses TfMainPanel (golden main.h:669 TPanel*),
    //    cbSetupFileName reuses TfLotInfoRunMode (golden main.h:875 TComboBox*,
    //    only ->Text read here), edWorkTemperBase reuses TfLotInfoEdit
    //    (golden main.h:732 TEdit*).
    TfMainPanel       *palMainStatus;                         // [DATA] golden main.h:669 (TPanel*)
    TfLotInfoRunMode  *cbSetupFileName;                       // [DATA] golden main.h:875 (TComboBox*) -- only ->Text used
    TfLotInfoEdit     *edWorkTemperBase;                      // [DATA] golden main.h:732 (TEdit*)
    virtual bool Home(AnsiString Func);                        // [METHOD] golden main.h:1276 -- offline: no real Home cycle to run -> false
    // -- W906-TesterTCPTimer ADD (20260720): members Interface/TesterTCP_Socket.cpp's
    //    TimerProcessTCPDataTimer/SimulateBin deref -------------------------------
    TStringList *tTestResult;       // [DATA] golden main.h:1392 (TStringList*) -- ctor pre-fills 32x"-1" (golden main.cpp:2236-2239)
    TStringList *tBarCodeList;      // [DATA] golden main.h:1396 (TStringList*) -- ctor pre-fills 32x"0" then
                                    //   Strings[31]+=";" => "0;" (golden main.cpp:2241 quirk, PRESERVED --
                                    //   makes the default BARCODE? reply end "...,0;;")
    AnsiString SVID1190_OSSetup;    // [DATA] golden main.h:1499 -- SECS SVID1190 backing store (SV consumer
                                    //   uHGemHT9045_SV.cpp:231 untranslated; plain storage here)
    virtual void WritePERSITETemperature(); // [METHOD] golden main.h:1365 (void __fastcall; body Command.cpp:935-943)
                                    //   -- WRAPPER translated faithfully; leaf gated (below)
    virtual AnsiString PERSITETemperatureStrings();  // [METHOD] golden main.h (body Command.cpp:945-1482, ~538 lines +
                                    //   RefreshTempData) -- GATED LEAF: offline returns
                                    //   W906_PERSITETemperatureStrings_Sim (default ""), real body is its own
                                    //   future wave (temp/GPIB surface: fContact->fShow/IndexStatus/
                                    //   iContactMode/asGPIBTempShow/bTestSiteUse)
    AnsiString W906_PERSITETemperatureStrings_Sim;  // [PORT-ONLY SEAM] test-settable stand-in feed for the
                                    //   gated leaf above (same data-driven-facade idiom as GetSamSungMap
                                    //   ""-default / GetSamSungSoakTime "0"-default); default ""
    // -- W906-AutoCleanFoundation ADD (20260721): members the AutoClean foundation
    //    wave's translated functions (AutoClean/AutoClean.cpp) deref. Per-member
    //    touch-scope note (verified by grepping golden -- see that wave's own
    //    report for detail): ONLY chkCleanPadPickErr is actually dereferenced by
    //    a function THAT wave translated (CheckInSuckICFallDown, inside
    //    #ifdef SOFT_SIMULTE). AddAutoCleanMessage / bAutoCleanTest / cbIndexDrop /
    //    pnlCleanCount / AutoCleanContactCountLabel / edHPX / edHPY are touched
    //    only by the 4 named core engines (DoAutoCleanKit /
    //    DoAutoCleanPickfromCleanKit / DoIndexAutoClean(+variant) / EnableAutoclean /
    //    SearchCleanNum) -- pre-staged at zero behavioural risk (plain data /
    //    no-op sinks, same idiom as every other fMain member).
    virtual void AddAutoCleanMessage(AnsiString S); // [METHOD] golden AutoClean.cpp -- offline log sink no-op
    bool bAutoCleanTest;                          // [DATA]   golden main.h -- offline default false
    TfMainCheckBox *cbIndexDrop;                  // [DATA]   golden main.h:117 (TCheckBox*) -- offline Checked=false
    TfMainCheckBox *chkCleanPadPickErr;           // [DATA]   golden main.h:132 (TCheckBox*) -- offline Checked=false; ACTIVE (CheckInSuckICFallDown, SOFT_SIMULTE-gated)
    TfMainPanel    *pnlCleanCount;                 // [DATA]   golden main.h:864 (TPanel*) -- Caption only touched here; ->Font->Color is SearchCleanNum's, see pnlCleanCountFont below
    TfMainFont     *pnlCleanCountFont;              // [DATA]   golden main.h (TPanel->Font, TFont*) -- SearchCleanNum sets clRed/clNavy
    TfMainPanel    *AutoCleanContactCountLabel;    // [DATA]   golden main.h:148 (TLabel* -- see forms/FormWidgets.h's TfMainPanel conflation note); Caption only (EnableAutoclean)
    TfLotInfoEdit  *edHPX;                        // [DATA]   golden main.h:113 (TEdit*) -- DoIndexAutoClean
    TfLotInfoEdit  *edHPY;                        // [DATA]   golden main.h:114 (TEdit*) -- DoIndexAutoClean
    TfMainAutoCleanGrid *tmyAutoClean;             // [DATA]   golden main.h:347 (TTMyTray* clean-kit grid -- the "THeatTable*" this comment used to
                                                    //   carry was wrong; corrected by W7-F0 per plan SS10-5, verified directly against golden).
                                                    //   Write-only (confirmed by grep: SetAutoCleanICCount/SetAutoCleanTrayPosition only WRITE
                                                    //   XItem/YItem/Top/Width/Height, never read back inside AutoClean.cpp; a pure no-op/plain-data
                                                    //   sink is faithful). ->SetCellColorIndex IS called (Part D's SetCleanCellValue helper,
                                                    //   iMode==eUcleanUsed branch) -- reuses the SAME no-op idiom as htShullte0/1.
    // AI(W906-AutoCleanCluster) 20260722: golden main.h:164 `TTMyTray *mtPlate2;`
    // -- the HotPlate-2 clean-kit grid widget SetAutoCleanTrayPosition's
    // bE43AutoCleanUseHotplate branch reads (->Top/->Width/->Height only,
    // confirmed by grep). Sibling widget of tmyAutoClean above (both are golden
    // TTMyTray*), so reuses the SAME TfMainAutoCleanGrid stand-in.
    TfMainAutoCleanGrid *mtPlate2;                  // [DATA]   golden main.h:164 (TTMyTray*) -- read-only here (Top/Width/Height)
    TStringGrid *AutoCleanStringGrid;               // [DATA]   golden main.h:457 (TStringGrid*) -- REAL backing store (see forms/FormWidgets.h banner);
                                                    //   RestoreCleanKitData/CheckCleaningCount read Cells[][] back via atoi(). Default-constructed
                                                    //   5x5 (vclcompat default); a future wave's SetAutoCleanICCount translation resizes it via
                                                    //   ->ColCount=/->RowCount=.
    // AI(W906-W7-F1) 20260729: W7-F1 ADD -- members SECSGEM/uHGemHT9045.cpp's
    //    22-override layer dereferences that the facade lacked -- plan SS6-F1
    //    / SS4-V1 ("Wall 2"). Re-derived by grepping golden directly rather
    //    than trusting the plan's own inventory: golden touches 28 distinct
    //    `fMain->` spellings total, but 2 of those (PPID / bNeedClearFile,
    //    golden SECSGEM/uHGemHT9045.cpp:5311-5312) are inside `//`-commented-
    //    out code with no live call site, so they are NOT added here -- doing
    //    so would be inventing a member nothing dereferences. Of the
    //    remaining 26 live members, this wave found 3 more missing than the
    //    plan's own named list (ChangePassword/FTClick/RTClick).
    //    F1's OWN scope is the facade SURFACE only -- the 22 virtual
    //    overrides that will actually call these members are translated in a
    //    LATER wave (explicitly out of scope here per the task brief). Every
    //    body below is therefore a documented no-op or a feedable
    //    `W906_..._Sim`/call-count seam (per the fMain.h contract's rule 4
    //    above), never a silent implementation of golden's real logic.
    //
    //    AI(W906-W7-F1fix) 20260729 -- CORRECTION: the "Wall 2" unlock this
    //    facade surface provides is REAL but NARROWER than the prose above
    //    implies.
    //
    //    AI(W906-W7-F1fix3) 20260731 -- THAT CORRECTION'S FIGURES WERE STILL
    //    WRONG, and the failure mode is named here so it stops recurring: it
    //    said "9 of the 22 overrides touch fMain, and 5 of those are FULLY
    //    unblocked because they touch ONLY fMain". Its scan recognised only
    //    `//` comments and NEVER `/* */` BLOCK comments. Two of that 9 --
    //    ProcessS7F23FromatReceipe and ProcessS7F25FromatReceipe -- have NO
    //    live fMain dereference at all: their only fMain sites sit inside block
    //    comments that OPEN at golden uHGemHT9045.cpp:5844 (S7F23 --
    //    fMain->cbSetupFileName at :5850/:5852) and :5961 (S7F25 -- the same
    //    member at :5966/:5968). Re-derived this wave with a character-level
    //    comment-AND-string-aware scan of the cp950-decoded golden over the
    //    brace-matched bodies of all 22 virtuals (golden uHGemHT9045.h:346-365
    //    + :367-368), the real figures are:
    //      * SEVEN overrides live-dereference fMain: ReloadParameter,
    //        LookForFile, S2F15_CheckNewEquipmentConstant,
    //        S2F15_UpdateNewEquipmentConstant, S2F42_Host_Command_Acknowledge
    //        (body golden uHGemHT9045.cpp:1146-4189), AddSV, AddEC.
    //      * THREE of those seven touch ONLY fMain and every member they need
    //        is present here today: ReloadParameter (LoadTestModePicture,
    //        UpdateMainOperateMode, LoadRunModePicture, LoadStartModePicture),
    //        LookForFile (cbSetupFileName, LookForFile) and
    //        S2F15_CheckNewEquipmentConstant (CanChangeSite). "Unblocked" means
    //        on the FORM-FACADE axis only -- non-form dependencies are a
    //        separate question.
    //      * The other FOUR are blocked on far more than the "+fLotInfo /
    //        +fSetup / +fSCKART / +fNote" the old note listed:
    //        S2F15_UpdateNewEquipmentConstant on 8 other live form pointers,
    //        S2F42_Host_Command_Acknowledge on 14, AddSV on 8 (plus the 3
    //        missing fMain widgets in the fix2 block below), AddEC on 7.
    //      * TWO more overrides are form-blocked WITHOUT touching fMain and were
    //        missing from every earlier list: S7F4_ProcessProgramAcknowledge
    //        (fLotInfo/fOffSet/fSetup) and S125F4_LevelSettingChangeAcknowledge
    //        (fSecurity). The remaining 13 have zero live form dereference.
    //      * Across the 22 bodies, 29 distinct form pointers are live-
    //        dereferenced; PORTED/forms holds 9 form headers, so 21 of those 29
    //        have NO facade header at all. fMain is the only one that is nearly
    //        complete (3 of the 32 members these bodies need are missing).
    //    The full per-override / per-form table, with golden line citations and
    //    the LOWER-BOUND caveat on the per-header "missing" counts, lives in
    //    tests/test_w7_f1_wall2_probe.cpp's header; see also
    //    docs/W7_UI_ARCHITECTURE_PLAN.md SS10.
    //
    //    AI(W906-W7-F1fix2) 20260729 -- the block above is scoped to golden
    //    uHGemHT9045.cpp only, and that hides one gap: AddSV lives in
    //    uHGemHT9045_SV.cpp, which derefs THREE fMain members this facade
    //    still does not have -- edTorue0 (golden main.h:466, TEdit*, used at
    //    uHGemHT9045_SV.cpp:74), edTorue1 (main.h:467, TEdit*, :75),
    //    lbEPenconder (main.h:796, TPanel*, :100). All three are passed as bare
    //    WIDGET POINTERS into HGemPtr->SetSVDataPointer(SVID 1012/1013/1041),
    //    i.e. straight into the plan SS9-R8 void*-overload hazard the F0-a
    //    TObject base / F0-b static_assert exist to contain -- whoever adds
    //    them must go through the same widget stand-in types, not raw pointers.
    //    (Its other three, SVID1190_OSSetup/palMainStatus/tTestResult,
    //    are already here from earlier waves; uHGemHT9045_EC.cpp needs only
    //    cbSetupFileName + tSiteOnOff, both present.) So AddSV is blocked on
    //    the fMain side as well as by fLotInfo, and the "26 live members" count
    //    above is a per-file figure, not the whole SECSGEM layer: across all
    //    three golden SECSGEM TUs it is 34 distinct / 32 live / 29 present.
    //    These 3 are deliberately NOT added here -- adding members no
    //    translated code dereferences yet is exactly the "inventing surface"
    //    this wave refused to do for PPID/bNeedClearFile; they belong to
    //    whichever wave translates AddSV.
    virtual void cbSetupFileNameChange(void *Sender); // [METHOD] golden main.h:945 (body main.cpp:24643-24939, a 297-line
                                    //   recipe-reload cascade -- opens with ChangeSetUpFile() but the bulk of the 297
                                    //   lines is cbSetupFileNameChange's OWN body, both untranslated) -- offline: no-op
                                    //   that increments the call-count seam below so a test can observe the call happened
    int  W906_cbSetupFileNameChangeCallCount;     // [PORT-ONLY SEAM] call counter, default 0
    virtual void Clarn_Data(int Tag, AnsiString Msg=""); // [METHOD] golden main.h:1246 (body main.cpp:14925, per-day
                                    //   production-count file writer -- untranslated) -- offline: no-op call-count seam
    int  W906_Clarn_DataCallCount;                // [PORT-ONLY SEAM] call counter, default 0
    virtual void BtnPauseClick(void *Sender); // [METHOD] golden main.h:917 (body main.cpp:6965) -- TRANSLATED (partial,
                                    //   faithful): forwards to Pause("BtnPauseClick"), same call golden itself makes
                                    //   first. Golden's `#ifndef SOFT_SIMULTE` fProductionInfo->ClickPause() second
                                    //   line is NOT translated (fProductionInfo has no facade home yet) -- documented
                                    //   gap, not a silent drop.
                                    //   AI(W906-W7-F1fix) 20260729: the pre-existing TfMain::Pause it forwards to is
                                    //   itself a bare no-op (offline always returns false, no observable side
                                    //   effect) -- so without a seam of its OWN, a test could delete this entire
                                    //   forwarding call and no assertion anywhere would notice. Added the call-count
                                    //   seam below, same idiom as every other member in this wave.
                                    //   AI(W906-W7-F1fix2) 20260729 -- CORRECTION to the sentence this replaced,
                                    //   which claimed the counter below made "the forward itself" observable: it
                                    //   does NOT. W906_BtnPauseClickCallCount observes only that BtnPauseClick was
                                    //   CALLED; deleting the `Pause("BtnPauseClick")` forward leaves it at 1.
                                    //   Personally reproduced: compiled a fMain.cpp with ONLY that forward removed,
                                    //   ar-replaced fMain.cpp.obj in libht9045_forms.a, relinked the probe -- the
                                    //   check "BtnPauseClick() increments its own call-count seam" still PASSED,
                                    //   i.e. every pre-fix2 assertion stayed green with the forward gone. The
                                    //   forward is made
                                    //   observable by the W906_PauseCallCount / W906_PauseLastFunc seams on Pause()
                                    //   itself (declared next to Pause near the top of this class), which the probe
                                    //   now asserts; those DO go red when the forward is deleted.
    int  W906_BtnPauseClickCallCount;         // [PORT-ONLY SEAM] call counter, default 0 -- observes that
                                    //   BtnPauseClick ran at all; the forward INTO Pause() is observed separately by
                                    //   W906_PauseCallCount / W906_PauseLastFunc
    virtual void LoadRunModePicture(); // [METHOD] golden main.h:1302 (body main.cpp:12382, run-mode BMP picture
                                    //   selection UI -- untranslated) -- offline: no-op call-count seam
    int  W906_LoadRunModePictureCallCount;        // [PORT-ONLY SEAM] call counter, default 0
    virtual bool CanChangeSite(bool bNoIncludeHotplate=false); // [METHOD] golden main.h:1325 (body main.cpp:14350-14393) --
                                    //   GATED LEAF: real body has FOUR false-return paths, not just HasIC() tests.
                                    //   Three are live-IC guards (InArmSuck.HasIC() / InputShuttleHasIC() /
                                    //   IndexHasIC() / MOT[MMPlate1/2].HasIC(), none of which have a facade path into
                                    //   this TU). The FOURTH, in the bCanAutoCloseSite==false /
                                    //   IniConfig.bI28_OnOffSiteOnTheFly==true sub-branch (main.cpp:14383-14390), is
                                    //   NOT a HasIC() test at all: it returns false on
                                    //   `bPickFromLoader==true || iPickFromLoadStageTask!=1`. Offline returns
                                    //   W906_CanChangeSite_Sim (default true), matching golden's own fall-through
                                    //   when every one of those four guards is false (the offline-everywhere posture
                                    //   used tree-wide, e.g. cInplace/InArmPlacementEnable above).
                                    //   AI(W906-W7-F1fix) 20260729: a complete, already-tested translation of this
                                    //   SAME golden function exists as ComputeCanChangeSite (MainCalcCore.h/.cpp,
                                    //   covering all four paths above, exercised by tests/test_MainCalcCore.cpp).
                                    //   This facade member does NOT delegate to it: ComputeCanChangeSite is a pure
                                    //   function that takes the live global state (bCanAutoCloseSite,
                                    //   IniConfig.bI28_OnOffSiteOnTheFly, InArmSuck.HasIC(), InputShuttleHasIC(),
                                    //   IndexHasIC(), MOT[MMPlate1/2].HasIC(), bPickFromLoader,
                                    //   iPickFromLoadStageTask) as PARAMETERS, and none of those symbols are visible
                                    //   from ht9045_forms (the bottom layer this TU compiles into) -- wiring them in
                                    //   would require ht9045_forms to link upward into the modules that define them,
                                    //   recreating exactly the kind of cycle W7-F0 removed. A fresh Sim-seam stand-in
                                    //   is therefore the correct shape here, not a missed reuse opportunity; the
                                    //   eventual TfMain::CanChangeSite override translation (once fMain gains a real
                                    //   binder with access to those globals) should call ComputeCanChangeSite
                                    //   instead of re-deriving the branch tree.
                                    //   AI(W906-W7-F1fix3) 20260731 -- the duplication is THREE-WAY, not two-way, so
                                    //   do not read the note above as "resolved at two". The third live implementation
                                    //   is Automation/auto9045.cpp:197-202, W5FA_TfMainExt::CanChangeSite() -- a
                                    //   ZERO-argument member whose body is a literal `return true;` under a "golden
                                    //   main.cpp body unavailable this wave" JUDGMENT CALL comment, with a LIVE call
                                    //   site 487 lines below it at auto9045.cpp:684
                                    //   (`if(W5FA_FMain.CanChangeSite()==false) { return 3; }`). Verified by grepping
                                    //   the whole ported tree: those are the only three CanChangeSite bodies
                                    //   (this member + ComputeCanChangeSite + the auto9045 stub), and the auto9045 one
                                    //   is the only one that is NOT test-drivable -- being a literal constant it has
                                    //   no seam, so its `return 3` path is unreachable in any test. Whoever gives
                                    //   fMain a real binder should collapse all three onto ComputeCanChangeSite.
    bool W906_CanChangeSite_Sim;                  // [PORT-ONLY SEAM] test-settable return, default true
    virtual void BtnTrayEndClick(void *Sender); // [METHOD] golden main.h:925 (body main.cpp:13944 -- one line,
                                    //   InitialTrayFeedTask("BtnTrayEndClick"), itself untranslated: golden main.h:1245
                                    //   declares InitialTrayFeedTask as a TfMain MEMBER function, not a free function;
                                    //   csystem.cpp's W7C2_FMAIN_INITIALTRAYFEED TU-local macro is the existing
                                    //   stand-in for that member) -- offline: no-op call-count seam
    int  W906_BtnTrayEndClickCallCount;           // [PORT-ONLY SEAM] call counter, default 0
    virtual void UpdateMainOperateMode(); // [METHOD] golden main.h:1236 (body main.cpp:12803-13127, ~325-line hardware
                                    //   relay/IO ladder -- ATC site-use relays, edWorkTemperBase/edSoakTime
                                    //   enable-locks, WriteLastDataFile/ReadLastDataFile, ChangeATCSiteUse -- already
                                    //   documented as out of scope by SetMainRunStartMode's own comment above) --
                                    //   offline: no-op call-count seam
    int  W906_UpdateMainOperateModeCallCount;     // [PORT-ONLY SEAM] call counter, default 0
    virtual void LoadStartModePicture(); // [METHOD] golden main.h:1304 (body main.cpp:23827, start-mode BMP picture
                                    //   selection UI -- untranslated) -- offline: no-op call-count seam
    int  W906_LoadStartModePictureCallCount;      // [PORT-ONLY SEAM] call counter, default 0
    virtual void LookForFile(); // [METHOD] golden main.h:1300 (body main.cpp:9016, Offset-directory filesystem
                                    //   scan/migration -- untranslated) -- offline: no-op call-count seam
    int  W906_LookForFileCallCount;               // [PORT-ONLY SEAM] call counter, default 0
    virtual int ChangeTesterConnect(int Mode, bool Msg=true, bool bRemote=false); // [METHOD] golden main.h:1322
                                    //   (body main.cpp:12064) -- GATED LEAF: real body returns 1 on SystemStart==true
                                    //   or (offline #ifndef SOFT_SIMULTE branch) HasICUnderMachine()/HasAnyICInMachine(),
                                    //   neither reachable from this TU, and 0 on the fall-through success path.
                                    //   Offline returns W906_ChangeTesterConnect_Sim (default 0, the golden success
                                    //   code -- matching the "no hardware blocks it" posture used by CanChangeSite
                                    //   above).
                                    //   AI(W906-W7-F1fix) 20260729 -- CORRECTION (re-verified against the ported tree,
                                    //   not just golden): this is NOT the sole ported-tree stand-in for golden's
                                    //   fMain->ChangeTesterConnect, and the caller does NOT discard the return value.
                                    //   There are two independent TU-local stand-ins today, neither of which calls
                                    //   THIS facade member: (1) Automation/auto9045.cpp's SetTesterConnect symbol
                                    //   `return`s (does not discard) the result of its OWN TU-local
                                    //   W5FA_TfMainExt::ChangeTesterConnect stub straight to ITS OWN caller; (2)
                                    //   csystem.cpp's W7C2_FMAIN_CHANGETESTERCONNECT TU-local macro (a second,
                                    //   separate gate, near the "onLine switch" call site) discards its int ARGUMENT,
                                    //   not a return value, and does not call this member either. Nothing in the
                                    //   ported tree calls this new facade member directly yet -- it exists so a
                                    //   future SECSGEM override translation (golden uHGemHT9045.cpp:859) has
                                    //   somewhere to land, per this wave's own gate probe.
    int  W906_ChangeTesterConnect_Sim;            // [PORT-ONLY SEAM] test-settable return, default 0
    virtual int SetTemp(bool bAsk, double fWorkTemp, double fSoakTime); // [METHOD] golden main.h:1321 (body
                                    //   main.cpp:23890) -- GATED LEAF: real body returns 1 on SystemStart==true,
                                    //   then walks a ShowMyMessageBox_YES_NO confirm + ChangeTempMode cascade with no
                                    //   facade path from this TU. Offline returns W906_SetTemp_Sim (default 0, the
                                    //   golden success code).
    int  W906_SetTemp_Sim;                        // [PORT-ONLY SEAM] test-settable return, default 0
    virtual void ChangePassword(); // [METHOD] golden main.h:1417 (body main.cpp:31772, SECS/GEM password-file
                                    //   loader -- untranslated) -- offline: no-op call-count seam
    int  W906_ChangePasswordCallCount;            // [PORT-ONLY SEAM] call counter, default 0
    virtual int FTClick(bool bMan=false); // [METHOD] golden main.h:1567 (body main.cpp:29666) -- GATED LEAF: real
                                    //   body returns 1 on SystemStart==true, 2/3/4/8 on assorted live-IC/level/tray
                                    //   blocks (InArmSuck.HasIC() etc., none reachable from this TU), 0 on the
                                    //   fall-through success path (which also calls Clarn_Data() above and
                                    //   EventReport() -- both already-real/no-op sinks here). Offline returns
                                    //   W906_FTClick_Sim (default 0, the golden success code).
    int  W906_FTClick_Sim;                        // [PORT-ONLY SEAM] test-settable return, default 0
    virtual int RTClick(bool bMan=false); // [METHOD] golden main.h:1568 (body main.cpp:29790-29954) -- sibling of
                                    //   FTClick above (same untranslated live-IC-branch shape), but NOT the same
                                    //   return-code shape: FTClick returns one of {0,1,2,3,4,8}; RTClick returns one
                                    //   of {0,1,2,3,4,5,6,7,8} -- three codes (5, 6, 7) that FTClick never returns.
                                    //   AI(W906-W7-F1fix2) 20260729, per-code attribution re-derived from golden
                                    //   (the previous one lumped 5 and 6 together as "the MOT[MMTrayY] tray/IC
                                    //   guard", which is only true of 6): 7 = the IniConfig.bSPILFunction &&
                                    //   bCanRunSCKART gate at the top (main.cpp:29795-29797, no FTClick counterpart);
                                    //   5 = the iSecsGemSwitchFTRT==0 && cbRunStartMode->Enabled==false guard
                                    //   (:29866-29868), which FTClick numbers 3 (:29721-29723); 6 = the
                                    //   MOT[MMTrayY]/MOT[MMTrayY_Car] tray/IC guard (:29870-29874), which FTClick
                                    //   numbers 4 (:29725-29729). Offline returns W906_RTClick_Sim (default 0).
    int  W906_RTClick_Sim;                        // [PORT-ONLY SEAM] test-settable return, default 0
    TStringList *tSiteOnOff[2];                   // [DATA] golden main.h:1393 (TStringList*[2]) -- REAL concrete
                                    //   storage, same idiom as tTestResult/tBarCodeList above: ctor prefills BOTH
                                    //   with MAX_SOCKET_ROW*MAX_SOCKET_COL (golden main.cpp:2242-2248) "0" strings so
                                    //   uHGemHT9045.cpp's `if(z<fMain->tSiteOnOff[0]->Count)` guard and
                                    //   `->Strings[z]` read are satisfiable exactly as golden expects.
    TfLotInfoEdit *edSoakTime;                    // [DATA] golden main.h:733 (TEdit*) -- reuses TfLotInfoEdit, same
                                    //   stand-in already used for edWorkTemperBase/edHPX/edHPY (only ->Text read/written).
    // AI(W906-W7-L1-Wave0) 20260801: W7-L1 Wave-0 ADD -- the 16 fMain members the
    //    six asendic_* tray SM files (Loader / Loader_RT / Color / Auto / Auto_RT,
    //    plus the shared asendic.cpp substrate) dereference and this facade did
    //    not have.  Landed in ONE serialized pass ahead of the four parallel
    //    translation agents so they cannot collide on this header.  Every golden
    //    line below was re-read from the cp950-decoded golden main.h in THIS pass,
    //    not taken from the recon report.
    //
    //    WIDGET-TYPE NOTE (this is why all 7 tray-count labels use
    //    TfMainTrayPanel, not TfMainPanel): golden main.h:392 and :393-395 /
    //    :861-863 are ALL `TLabel*`, one adjacent golden family.  TfMainTrayPanel
    //    is the TLabel-backed alias (forms/FormWidgets.h:143 -> vclcompat::TLabel);
    //    TfMainPanel is TPanel-backed (:132).  Using the TPanel alias would have
    //    propagated into 7 new members exactly the conflation FormWidgets.h:127-130
    //    already records as W7-U debt for AutoCleanContactCountLabel.
    //    All 7 are Caption-write-only in golden (asendic_Loader.cpp:1229/:2082/
    //    :2105/:2849/:2861; asendic_Auto.cpp:537-542/:612-617/:2237-2242).
    TfMainTrayPanel *lblLoadTrayCnt;              // [DATA] golden main.h:392 (TLabel*) -- Caption written only
    TfMainTrayPanel *lblAuto1TrayCnt;             // [DATA] golden main.h:393 (TLabel*)
    TfMainTrayPanel *lblAuto2TrayCnt;             // [DATA] golden main.h:394 (TLabel*)
    TfMainTrayPanel *lblAuto3TrayCnt;             // [DATA] golden main.h:395 (TLabel*)
    TfMainTrayPanel *lblAuto4TrayCnt;             // [DATA] golden main.h:861 (TLabel*)
    TfMainTrayPanel *lblAuto5TrayCnt;             // [DATA] golden main.h:862 (TLabel*)
    TfMainTrayPanel *lblAuto6TrayCnt;             // [DATA] golden main.h:863 (TLabel*)
    //    The 6 Auto edits are golden TEdit* and only ->Text is written
    //    (asendic_Auto.cpp:545-550), so they reuse TfLotInfoEdit -- the same
    //    cross-form alias fMain->edWorkTemperBase / edHPX / edHPY already use,
    //    i.e. established precedent rather than a new conflation.
    TfLotInfoEdit *edtAuto1;                      // [DATA] golden main.h:867 (TEdit*) -- Text written only
    TfLotInfoEdit *edtAuto2;                      // [DATA] golden main.h:373 (TEdit*)
    TfLotInfoEdit *edtAuto3;                      // [DATA] golden main.h:374 (TEdit*)
    TfLotInfoEdit *edtAuto4;                      // [DATA] golden main.h:376 (TEdit*)
    TfLotInfoEdit *edtAuto5;                      // [DATA] golden main.h:377 (TEdit*)
    TfLotInfoEdit *edtAuto6;                      // [DATA] golden main.h:378 (TEdit*)
    TfMainCheckBox *chkE84IDTray;                 // [DATA] golden main.h:887 (TCheckBox*) -- Checked read
                                    //   (asendic_Color.cpp:436) AND written (:649); offline default false
    //    StringGrid2 -- REAL backing store, and its SIZE is load-bearing.
    //    vclcompat::TStringGrid stores Cells in vectors and indexes them with
    //    vector::at, so an out-of-range Cells[][] THROWS std::out_of_range
    //    (vclcompat/StringGrid.h:43-44) instead of silently growing.  golden
    //    asendic_Color.cpp:831 writes Cells[3][38].  The ctor size is taken from
    //    golden's own form resource, not guessed: main.dfm's `object StringGrid2:
    //    TStringGrid` block sets ColCount = 8 and RowCount = 60 (main.dfm:15440,
    //    :15447, :15451 -- read this pass), so forms/fMain.cpp constructs it
    //    `new TStringGrid(8, 60)`, which covers [3][38] with golden's real margin.
    TStringGrid *StringGrid2;                     // [DATA] golden main.h:490 (TStringGrid*) -- 8x60 per main.dfm:15447/:15451
    TfMainRENESASServer *RENESAS_Server;          // [DATA] golden main.h:1710 (TRENESAS_Server*) -- see the nested class above
    //    DELIBERATELY NOT ADDED -- `TfMainCheckBox *CheckBox1` (golden main.h:361,
    //    TCheckBox*, verified this pass).  Its ONLY reference in the whole W7-L1
    //    family is asendic_Loader.cpp:2942, which sits inside the
    //    `#ifdef SOFT_SIMULTE` block at :2941-2947; SOFT_SIMULTE is undefined in
    //    this tree, so that block is not compiled and the member would be surface
    //    nothing dereferences.  Same rule that kept PPID / bNeedClearFile out in
    //    W7-F1, and the same precedent this header's own `cb1` note records.
    //    Listed so a later wave does not "discover" it as an omission.
    // AI(W906-W7-L2) 20260803: W7-L2 ADD -- the three fMain members golden
    //    ckernel.cpp dereferences and this facade did not have.  Every golden
    //    line cited below was re-read from the cp950-decoded golden in THIS
    //    pass (main.h / main.cpp / main.dfm / ckernel.cpp / elec\myvcl\butPa1.h).
    //
    // (a) MainFormChange -- golden main.h:1261 `void __fastcall MainFormChange();`
    //     Sole golden caller in this front: ckernel.cpp:367, inside
    //     ScanSystemSensor's one-shot `if(SoftStart==true)` startup block
    //     (ckernel.cpp:365-533 -- an earlier comment in this same wave gave the
    //     end as :379, which is wrong by 154 lines; brace-matched this pass with
    //     comments and string literals masked: `if` at :365, body `{` at :366,
    //     closing `}` at :533, and the `else if(SoftStop==true)` arm opens at
    //     :534.  :379 is only the mid-block `SoftStop=false;`), immediately
    //     before AccelateTask=1 (:369) / ChangeUseSuckMode() (:370) /
    //     SetWorkParameter() (:371).
    //     Golden's REAL body is main.cpp:3883-4096 (214 lines) and it is PURE
    //     FORM REPAINT -- all 214 lines were read this pass, and its ONLY
    //     assignments are:
    //       1. `Ptr[i][j]->Visible=false` over a 16x8 LOCAL array of the form's
    //          own TALed members (array built main.cpp:3890-3914, hide pass
    //          :3922-3924);
    //       2. `PtrSort[i][j]->Visible=false` over the 2x8 9046AU sort-shuttle
    //          TALed array (built :3916-3920, hide pass :3926-3932);
    //       3. the re-show pass that turns back on only the rows/columns
    //          matching TestIF_File.iTestMode (:3934-4034, via the locals
    //          SingleRow/iCol), plus the IsNNMode()==NN_1Row (:4036-4055) and
    //          ==NN_2Row (:4057-4066) corrections;
    //       4. `labFailAlarmCnt->Visible` / `->Caption`, composed from
    //          IniConfig.bG04ShowFailAlarmCount + Prod.bContsFailBySocket /
    //          Prod.bContsFailByHead and their two counters (:4068-4094).
    //     It writes NO global, NO Prod/TestIF field, and drives no motor/IO:
    //     every write target is a TfMain-owned VCL widget with no facade home
    //     (e.g. golden main.h:313 `TALed *led_BLCarryKit_0;`, :889
    //     `TALed *led_SortShtKit_0;`, :673 `TLabel *labFailAlarmCnt;`).
    //     So the behaviour ELIDED offline is exactly "the site-map LED matrix
    //     and the fail-alarm-count label are not repainted to match the current
    //     iTestMode" -- unobservable to any headless caller.  That is why a
    //     no-op is faithful here rather than a silent drop, and it is the same
    //     shape as ProcessSensorScan (:210) / ChangeLevelAttr (:216) above.
    //     OBSERVABILITY GAP, stated rather than hidden (the lesson recorded in
    //     the W906-W7-F1fix2 block above): like those two neighbours this body
    //     has NO seam, so a test cannot tell "ckernel called MainFormChange"
    //     from "ckernel dropped the call".  Deliberate -- the brief for this
    //     wave was to copy the neighbours' shape exactly.  If the ckernel test
    //     plan needs to assert the call, the fix is the established one-liner:
    //     add `int W906_MainFormChangeCallCount;` here, init 0 in the ctor, and
    //     increment it in the body (same idiom as W906_LookForFileCallCount).
    virtual void MainFormChange();                // [METHOD] golden main.h:1261 (body main.cpp:3883-4096) -- offline: LED/label repaint no-op
    // (b) BtnSTEP / BtnT_Start -- golden main.h:102 `TBtnPanel *BtnSTEP;` and
    //     main.h:103 `TBtnPanel *BtnT_Start;`.  golden ckernel.cpp touches ONE
    //     property on each, ->Color, at exactly four sites (grep of ckernel.cpp
    //     for both names returns these four and nothing else), all inside the
    //     `if(CosFunction.bEnableSoftWareControlButton)` TSMC-only block
    //     (ChungHung 20150609, ckernel.cpp:66 / :108):
    //       WaitManualStepKey  (ckernel.cpp:54-94)
    //         :69  fMain->BtnSTEP->Color=clYellow;               // lamp ON
    //         :71  fMain->BtnSTEP->Color=(TColor)0x00804000;     // lamp OFF
    //       WaitManualStartKey (ckernel.cpp:96-131)
    //         :111 fMain->BtnT_Start->Color=clYellow;            // lamp ON
    //         :113 fMain->BtnT_Start->Color=(TColor)0x00804000;  // lamp OFF
    //     driven by bLampManualSetp / bLampManualStart = FlushFlag (:59 / :101).
    //     Write-only in golden: neither member is ever READ, there or anywhere
    //     in this ported tree (both names are new here).
    //
    //     WIDGET TYPE -- why TfMainPanel and NOT TfMainSpeedButton.  The tree's
    //     existing TBtnPanel stand-in IS TfMainSpeedButton (forms/FormWidgets.h
    //     :293-299, carrying fMain->BtnOneCycle, golden main.h:72), but it holds
    //     ONLY `bool Down` -- it has no Color, so it cannot store what these four
    //     sites write, and forms/FormWidgets.h is out of this wave's write scope.
    //     Of the stand-ins that DO expose a settable Color, TfMainPanel is the
    //     exact right one and not a fudge: golden's TBtnPanel is literally
    //     `class PACKAGE TBtnPanel : public TPanel` (golden elec\myvcl\butPa1.h:12,
    //     read this pass), so the `->Color` these sites write is the INHERITED
    //     stock TPanel::Color property -- and TfMainPanel is exactly
    //     vclcompat::TPanel (forms/FormWidgets.h:132 -> vclcompat/Controls.h:240-246,
    //     `int Color`).  That makes this conflation strictly tighter than the one
    //     forms/FormWidgets.h:136-141 already records and accepts for
    //     mtAuto1/mtAuto2/mtAuto3 (golden TTMyTray*, ->Color-only, aliased onto
    //     vclcompat::TLabel).  Per contract rule 3 no new widget TYPE is invented.
    //     REPORTED, NOT DONE (needs files this wave does not own): the right end
    //     state is ONE TBtnPanel stand-in carrying both Down and Color -- either
    //     give TfMainSpeedButton a `Color` member in forms/FormWidgets.h, or do
    //     the W7-C4 repoint of the whole family onto vclcompat::BtnPanelCore,
    //     which already models golden TBtnPanel faithfully (`TColor Color`,
    //     vclcompat/BtnPanelCore.h:79) but whose .cpp is NOT in the root
    //     CMakeLists `vclcompat` library today (only in two tests/ targets), so
    //     using it from here would break the link for every ht9045_forms consumer
    //     until that line is added.
    TfMainPanel *BtnSTEP;                         // [DATA] golden main.h:102 (TBtnPanel*) -- ->Color only; ctor seeds 0x00804000, see forms/fMain.cpp
    TfMainPanel *BtnT_Start;                      // [DATA] golden main.h:103 (TBtnPanel*) -- ->Color only; ctor seeds 0x00804000, see forms/fMain.cpp
    // -- AI(W906-PT-W3-integrate) 20260808 ADD: 2 methods + 1 field the PT-W3
    //    unit Automation/uRENESAS_Server.cpp derefs (its "FACADE ADDITIONS
    //    NEEDED" banner is the measurement; golden lines re-read this pass). --
    virtual void EnabledSetupFile(bool bEnabled); // [METHOD] golden main.h:1340 (body main.cpp:28310-28360: gates
                                    //   cbSetupFileName->Enabled by IC-in-machine / CUSTOMER_CODE / AccessLevel /
                                    //   RMS level rules) -- offline no-op: pure UI-enable policy, every input it
                                    //   reads (HasICUnderMachine/AccessLevel/LevelSet) is level-gating for a
                                    //   combobox this facade renders nowhere
    int  iHasChangeFile;                          // [DATA] golden main.h:1701 -- no golden ctor assignment (VCL
                                    //   zero-init; golden's own `==0` read at main.cpp:24887 means "never set");
                                    //   uRENESAS_Server writes 9 then polls ==1 (its case 30000)
    virtual void SetLotState(int iState);         // [METHOD] golden main.h:1327 (body main.cpp:15160-15250: pushes
                                    //   LOTNUMBER/LOTEND over TesterTCP when iTestType==TCP_IP_MODE, else GPIB
                                    //   MSG_CMD path; early-returns otherwise) -- offline no-op is behaviourally
                                    //   identical whenever iTestType is neither TCP_IP_MODE nor GPIB_MODE
    // -- FW3-WA ADD: Command.cpp wave-A declarations --------------------------
    // AI(W906-FW3-WA) 20260817: FW-3 Wave A ADD -- 60 golden TfMain:: PURE-method
    // declarations whose bodies are now translated for real in the NEW file
    // Command.cpp (golden Command.cpp:86-2242 / :3825-4005 / :5170-5638 minus
    // SetSiteMapData/SetAlarmSetup / :12063-12508).  3 sibling methods in this
    // same golden byte range (WritePERSITETemperature, PERSITETemperatureStrings,
    // ArmStatusStrings) are declared ABOVE already (pre-existing GATED-leaf /
    // wrapper stubs, bodies in forms/fMain.cpp) and are NOT redeclared here --
    // Command.cpp still carries a full-body translation of all three per the
    // wave brief, which is a KNOWN STUB COLLISION for the next integration pass
    // to retire (see this wave's report's STUB COLLISIONS section). Per contract
    // rule 1 every declaration below is `virtual`; per rule 4 each cites its
    // golden home. Signatures re-read from the cp950-decoded golden main.h this
    // pass (golden line numbers cited per member).
    virtual int RefreshTempData(bool bTransfer=false, int iArm=0, int iSite=0); // golden main.h:1388 (body Command.cpp:86-933)
    virtual void WriteHandlerID();                    // golden main.h:1367 (body Command.cpp:1484-1487)
    virtual void WriteArmStatus();                    // golden main.h:1369 (body Command.cpp:1489-1495)
    virtual void WriteArmForce();                     // golden main.h:1371 (body Command.cpp:1510-1541)
    virtual void WriteTempData();                     // golden main.h:1176 (body Command.cpp:1543-1549)
    virtual AnsiString TempDataStrings();             // golden main.h:1177 (body Command.cpp:1551-1586)
    virtual void WriteSetTempStatus();                // golden main.h:1178 (body Command.cpp:1588-1649)
    virtual void WriteSetTestTempStatus();            // golden main.h:1179 (body Command.cpp:1651-1764)
    virtual void WriteSoakTimeData();                 // golden main.h:1180 (body Command.cpp:1766-1775)
    virtual void WriteSetSoakTimeStatus();             // golden main.h:1181 (body Command.cpp:1777-1794)
    virtual AnsiString WriteSiteMapData(bool bGPIB=true); // golden main.h:1223 (body Command.cpp:1797-2028)
    virtual void WriteStartMode_NS();                 // golden main.h:1383 (body Command.cpp:2030-2060)
    virtual void WriteAssign_NS();                    // golden main.h:1384 (body Command.cpp:2062-2242)
    virtual void WriteForce_NS();                     // golden main.h:1386 (body Command.cpp:3825-3855)
    virtual AnsiString WriteBinMap(bool bGPIB=true);  // golden main.h:1374 (body Command.cpp:3858-3912)
    virtual void WriteSetBinMap(AnsiString BinData);  // golden main.h:1375 (body Command.cpp:3914-3987)
    virtual void WriteTestMode();                     // golden main.h:1376 (body Command.cpp:3989-4005)
    virtual void WriteChkSetup();                     // golden main.h:1420 (body Command.cpp:5170-5275)
    virtual void WriteHandlerTestArmEncoder();        // golden main.h:1421 (body Command.cpp:5277-5291)
    virtual void WriteHandlerTestArmEP();             // golden main.h:1422 (body Command.cpp:5293-5299)
    virtual void GetCZtesterBin();                    // golden main.h:1462 (body Command.cpp:5390-5417)
    virtual void GetCZSoakTime();                     // golden main.h:1463 (body Command.cpp:5419-5427)
    virtual void GetCZDoubleContactCount();           // golden main.h:1468 (body Command.cpp:5429-5438)
    virtual void GetCDHandlerID();                    // golden main.h:1464 (body Command.cpp:5440-5445)
    virtual void GetCZJamCode();                      // golden main.h:1465 (body Command.cpp:5447-5461)
    virtual void GetCZSiteMap(bool bSendGPIB=true);   // golden main.h:1466 (body Command.cpp:5463-5638)
    virtual void SetTesterID();                       // golden main.h:1594 (body Command.cpp:12063-12102)
    virtual void GetTesterID();                       // golden main.h:1595 (body Command.cpp:12104-12109)
    virtual void GetAutoClean();                      // golden main.h:1597 (body Command.cpp:12111-12116)
    virtual AnsiString AutoCleanStrings();             // golden main.h:1602 (body Command.cpp:12118-12128)
    virtual void GetForcePerPinN();                   // golden main.h:1603 (body Command.cpp:12130-12135)
    virtual AnsiString ForcePerPinNStrings();          // golden main.h:1604 (body Command.cpp:12137-12142)
    virtual void GetContactHeight();                  // golden main.h:1605 (body Command.cpp:12144-12149)
    virtual AnsiString ContactHeightStrings();         // golden main.h:1606 (body Command.cpp:12151-12156)
    virtual void GetYieldContinusFail();              // golden main.h:1607 (body Command.cpp:12158-12163)
    virtual AnsiString YieldContinusFailStrings();     // golden main.h:1608 (body Command.cpp:12165-12173)
    virtual void GetYieldSiteCompare();               // golden main.h:1609 (body Command.cpp:12175-12180)
    virtual AnsiString YieldSiteCompareStrings();      // golden main.h:1610 (body Command.cpp:12182-12190)
    virtual void GetDUTStaus();                       // golden main.h:1611 (body Command.cpp:12192-12197)
    virtual AnsiString DUTStausStrings();              // golden main.h:1612 (body Command.cpp:12199-12264)
    virtual void GetUPH();                            // golden main.h:1613 (body Command.cpp:12266-12271)
    virtual AnsiString UPHStrings();                   // golden main.h:1614 (body Command.cpp:12273-12285)
    virtual void GetIndexCycleTime();                 // golden main.h:1615 (body Command.cpp:12287-12292)
    virtual AnsiString IndexCycleTimeStrings();        // golden main.h:1616 (body Command.cpp:12294-12306)
    virtual void GetTempOfs();                        // golden main.h:1617 (body Command.cpp:12308-12313)
    virtual AnsiString TempOfsStrings();               // golden main.h:1618 (body Command.cpp:12315-12332)
    virtual void GetTempRange();                      // golden main.h:1619 (body Command.cpp:12334-12339)
    virtual AnsiString TempRangeStrings();             // golden main.h:1620 (body Command.cpp:12341-12347)
    virtual void GetVacuumAir();                      // golden main.h:1621 (body Command.cpp:12349-12354)
    virtual AnsiString VacuumAirStrings();             // golden main.h:1622 (body Command.cpp:12356-12363)
    virtual void GetAll();                            // golden main.h:1623 (body Command.cpp:12365-12385)
    virtual void GetHandlerVersion();                 // golden main.h:1624 (body Command.cpp:12387-12392)
    virtual AnsiString HandlerVersionStrings();        // golden main.h:1625 (body Command.cpp:12394-12399)
    virtual void UploadProdLog();                     // golden main.h:1534 (body Command.cpp:12401-12447)
    virtual void GetShuttleMode();                    // golden main.h:1635 (body Command.cpp:12449-12464)
    virtual void SetMaxTest();                        // golden main.h:1636 (body Command.cpp:12466-12478)
    virtual void GetMaxTest();                        // golden main.h:1637 (body Command.cpp:12480-12485)
    virtual void SetMaxInitialTest();                 // golden main.h:1638 (body Command.cpp:12487-12499)
    virtual void GetMaxInitialTest();                 // golden main.h:1639 (body Command.cpp:12501-12506)
    virtual AnsiString GetSiteState();                 // golden main.h:1644 (body Command.cpp:12508-12536)
    // -- end FW3-WA ADD --------------------------------------------------------
    TfMain();
    virtual ~TfMain() {}
};
extern TfMain *fMain;

#endif // FORMS_FMAIN_H
