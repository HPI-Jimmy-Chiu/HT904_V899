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

// ===========================================================================
//  TfMain -- non-VCL stub (golden main.h, TfMain:public TForm)
// ===========================================================================
class TfMain
{
public:
    virtual void DebugOneCycleHotPlate(AnsiString sfunc); // [METHOD] golden main.h:1565 -- debug log sink (empty)
    virtual bool Pause(AnsiString Func);                  // [METHOD] golden main.h:1249 -- offline never pauses -> false
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
    TfMain();
    virtual ~TfMain() {}
};
extern TfMain *fMain;

#endif // FORMS_FMAIN_H
