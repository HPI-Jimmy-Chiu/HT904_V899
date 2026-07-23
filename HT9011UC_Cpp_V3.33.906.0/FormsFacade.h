// =============================================================================
//  FormsFacade.h  --  non-VCL stand-in for the VCL form pointers the SMs deref
//
//  Translation wave: W6.0 (scaffold) ; EXTENDED W6.2 (in-arm HP geometry)
//  Translator: AI(W6.0-SCAFFOLD) 20260626 ; AI(W6.2-INARM) 20260626
//
//  W6 DECOUPLING STRATEGY (form-pointer cut)
//  -----------------------------------------
//  The BCB6 state machines reach UI/automation state through global VCL TForm
//  pointers (fMain / fAGV / fSortCT / fLotInfo / fOffSet / fSCKART / ...).  Those
//  forms cannot be pulled into the portable build (they derive from VCL TForm).
//  Instead we expose ONLY the members a given sub-wave's SM actually
//  dereferences, on a non-VCL facade, and grow it per sub-wave.
//
//  W6.1 SURFACE = fAGV ONLY (Empty-tray canary).  -- unchanged below.
//
//  W6.2 ADDITIONS (HP pick/place geometry leaves -- exactly the members
//  ainarm_SearchPickPlate.cpp / ainarm_SearchPlacePlate.cpp deref):
//    TfMain    fMain    : DebugOneCycleHotPlate / Pause / ShowTestHeadComp /
//                         ReStartAutoSiteMapping / slAutoSiteMapLog
//    TfSortCT  fSortCT  : pnlHP1.Caption / pnlHP2.Caption
//    TfLotInfo fLotInfo : cbRunMode.Visible / cbRunMode.Text
//    TfOffSet  fOffSet  : UseAutoOffsetFunction / UseInArmSetupTeach
//    TfSCKART  fSCKART  : iInputJamCnt
//  Each member is documented [DATA]/[METHOD] with its golden home.  All methods
//  are offline no-ops / false (a handler with no UI / no auto-offset / no pause).
//  The sbStateRecordClick(sbStateRecord) line in CheckHasSpaceToPlace_9045 is
//  gated #if 0 in the .cpp, so TSpeedButton/TObject are NOT pulled in and those
//  two TfMain members are intentionally NOT added.  cb1 is referenced only
//  inside #ifdef SOFT_SIMULTE (undefined) so it is not added either.
// =============================================================================
#ifndef FormsFacadeH
#define FormsFacadeH

#include "vclcompat/vcl_compat.h"   // AnsiString

// AI(W906-AutoCleanFoundation) 20260721: golden fMain->AutoCleanStringGrid
// (main.h, TStringGrid*) is NOT cosmetic -- AutoClean/AutoClean.cpp's
// RestoreCleanKitData/CheckCleaningCount (in scope this wave) read it back via
// atoi(...->Cells[X][Y].c_str()). Reuses vclcompat::TStringGrid (built
// W906-uHGemEquipment 20260716 for the analogous SECS SV/EC-grid problem) --
// a materially better fit than inventing a bespoke AutoCleanSubstrate type,
// since it already has real Cells[][]/RowCount/ColCount semantics. Second
// consumer after SECSGEM/uHGemEquipment.h, which sets the `using` idiom this
// mirrors verbatim (duplicate identical `using` declarations across headers
// are legal C++, not an ODR conflict).
#include "vclcompat/StringGrid.h"
using vclcompat::TStringGrid;

// Forward declaration: TfMainMemo (golden TMemo* shape) is fully defined further
// down this same header (W6.5 shuttle-log section) -- TfAGV only needs a
// pointer to it (W5-Final-AGV_E84 INTEGRATE, see below), so a forward
// declaration here is sufficient; FormsFacade.cpp includes this whole header
// before constructing TfAGV, so the full type is visible where it's needed.
struct TfMainMemo;

// AI(W906-Automation) 20260716: same forward-declaration idiom as
// TfMainMemo above -- TfLotInfoRunMode/TfLotInfoEdit are fully defined
// further down this header (TfLotInfo section) but TfMain (defined BEFORE
// that section) now also needs pointers to them (cbSetupFileName/
// edWorkTemperBase, see TfMain's own "W906-Automation ADD" block below).
struct TfLotInfoRunMode;
struct TfLotInfoEdit;

// ---------------------------------------------------------------------------
//  TfAGV -- non-VCL stub (W6.1).  Mirrors ONLY the one method the canary calls.
//  Golden: bool TfAGV::IsATK_AMR();  (Automation/AGV.h:205)
//  In the offline sim there is no ATK AMR present, so IsATK_AMR() returns false
//  (the DUMMY non-AMR path -- faithful to a handler with no AMR attached).
// ---------------------------------------------------------------------------
class TfAGV
{
public:
    bool IsSPIL_AMR();               // [METHOD] golden AGV.h:204 -- W5-Automation INTEGRATE: real (Automation/AGV_predicates.cpp)
    bool IsATK_AMR();
    bool Use_AMR();                 // [METHOD] golden AGV.h -- W6.3: offline no AMR -> false
    // -- W5-Final-AGV_E84 INTEGRATE ADD: member the E84 loader/unloader
    //    handshake engine (Automation/AGV_E84.cpp) derefs -----------------------
    TfMainMemo *mmE84Log;            // [DATA] golden AGV.h:85 (TMemo* E84 log) -- reuse existing TfMainMemo/TfMainMemoLines shape (meShuttle1/2's shape)
    TfAGV();
};

extern TfAGV *fAGV;     // golden: extern PACKAGE TfAGV *fAGV; (AGV.h:210)

// ===========================================================================
//  W906-AutoCleanFoundation ADD (20260721): TfNote -- FIRST home for fNote
//  (golden note.h `class TfNote : public TForm`) anywhere in this migrated
//  tree. Multiple earlier waves (Automation/AGV_PortScan.h's TfAGV::Timer2Timer
//  note, BarCode/BarCode_Bottom2DID.h's fNote->t2DCode/bMyServoOffInArm note)
//  found fNote had no home and deliberately left their own fNote-dependent
//  code gated/untranslated rather than stand up "a whole new TfNote facade
//  CLASS" for what was, in each of those cases, a single narrow call with no
//  other payoff. This wave's need is different in kind, not just degree: TWO
//  genuinely in-scope Part A functions (DoInArmPineRelease's servo-off-during-
//  alarm recovery cycle; InitialAutoCleanTask's SPIL FTP jam-code upload trace)
//  read/write exactly 4 plain-data fields -- no VCL lifecycle, no widgets, no
//  settings-editor surface like the deferred cases above. Standing up THIS
//  minimal a home costs nothing extra and unblocks a faithful (not gated)
//  translation of both functions; it does not retroactively un-gate any of
//  the earlier waves' own deferred fNote call sites (still all self-gated,
//  unaffected by this addition).
// ===========================================================================
class TfNote
{
public:
    bool       bMyServoOffInArm;     // [DATA] golden note.h:417ish (bool) -- "an alarm-triggered ServoOff on the in-arm is pending recovery"
    int        iMyServoOffInArmPosX; // [DATA] golden note.h:418 (int) -- encoder pos to jog back to (X)
    int        iMyServoOffInArmPosY; // [DATA] golden note.h:419 (int) -- encoder pos to jog back to (Y)
    AnsiString aJamCodeFilePath;     // [DATA] golden note.h:434 (AnsiString) -- SPIL FTP jam-code trace file path
    TfNote();
};
extern TfNote *fNote;   // golden: extern PACKAGE TfNote *fNote; (note.h:462)

// ===========================================================================
//  W906-AutoCleanFoundation ADD (20260721): TfShowMessage -- FIRST home for
//  fShowMessage (golden cUnitConvert.h note: "W7 VCL form pointer" -- another
//  confirmed no-home global, same class of gap as TfNote just above). Needed
//  by CleanSetSpeed's (in-scope) tail call `fShowMessage->ShowSpeed(bool)`.
//  Even smaller than TfNote: one method, zero data, offline no-op (a debug
//  "speed values changed" popup that never shows without a UI).
// ===========================================================================
class TfShowMessage
{
public:
    void ShowSpeed(bool bShow);   // [METHOD] golden cUnitConvert.h -- offline: no-op (no UI to show a debug popup on)
};
extern TfShowMessage *fShowMessage;

// ===========================================================================
//  W6.2 -- TMyStringList facade-local stub for slAutoSiteMapLog
//  Golden type is TMyStringList (cmydef), which in the target is only
//  forward-declared (cmydef.h:15).  The leaves call exactly ONE method on it:
//  AddTextWithDateTime(AnsiString).  Provide a tiny facade-local type exposing
//  only that, as an append/log no-op (offline -> no file/UI sink).
// ===========================================================================
class TfMainSiteMapLog
{
public:
    void AddTextWithDateTime(AnsiString /*S*/) {}   // golden TMyStringList::AddTextWithDateTime
};

// ===========================================================================
//  TfMain -- non-VCL stub (golden main.h, TfMain:public TForm)
//  ONLY the members the W6.2 HP geometry leaves deref.
// ===========================================================================
// ---------------------------------------------------------------------------
//  W6.3: TfMainTrayPanel -- the mtAuto1/mtAuto2/mtAuto3 status panels.  Golden
//  type is TPanel*; the catchtray engine only assigns ->Color (a TColor).  Stub
//  ->Color as an int (TColor is an int RGB code in the VCL).  TColor + the
//  clWhite/clYellow constants live in vclcompat (vcl_compat.h).
// ---------------------------------------------------------------------------
struct TfMainTrayPanel { int Color; bool Visible; TfMainTrayPanel():Color(0),Visible(false){} };  // [DATA] golden main.h (TPanel* mtAutoN / TLabel* lbCCDStatus->Visible W6.4)

// ---------------------------------------------------------------------------
//  W6.3: TfMainHanaART -- fMain->hanaART (golden main.h, HANA ART helper).  The
//  catchtray count helpers (AddFixTrayCount/AddTrayCount) call exactly two
//  methods on it; offline (no HANA link) IsHanaArtAvailable() is false so the
//  count helpers early-out, and AddNewTrayHead is a no-op.
// ---------------------------------------------------------------------------
class TfMainHanaART
{
public:
    bool IsHanaArtAvailable();      // [METHOD] golden -- offline: false
    void AddNewTrayHead(int iAuto); // [METHOD] golden -- offline: no-op
};

// ---------------------------------------------------------------------------
//  W6.5: SHUTTLE-ENGINE (acarry.cpp) fMain sub-objects.
//    * TfMainCheckBox  -- golden TCheckBox* (cbShowShuttleSensor /
//      cbTestOutShuttleSensor / cbShowInShuttleSensor).  Engine reads .Checked
//      (offline false -> the debug-log / sensor-confirm paths are skipped, the
//      proven DUMMY posture).
//    * TfMainGrid      -- golden THeatTable* (htShullte0 / htShullte1).  Engine
//      calls SetCellColorIndex(col,row,idx) only inside the Motor-View page
//      guard (pgMain->ActivePageIndex==emp7TabSheet21, both 0 offline so equal --
//      but SetCellColorIndex is a harmless no-op so it is safe either way).
//    * TfMainMemoLines / TfMainMemo -- golden TMemo* (meShuttle1 / meShuttle2).
//      Engine calls ->Lines->Add / ->Lines->Count / ->Clear inside the
//      cbShowInShuttleSensor.Checked guard (false offline) -> no-op.
//    * TfMainInplace   -- golden cInplace (InArmPlacementEnable()/iNo9ShtErrICCt/
//      bNo9ShtErrNo).  Offline InArmPlacementEnable()==false makes the No9
//      sub-blocks inert (the AutoLatch checks still run their main path).
// ---------------------------------------------------------------------------
struct TfMainCheckBox { bool Checked; TfMainCheckBox():Checked(false){} };       // [DATA] golden TCheckBox*
struct TfMainGrid     { void SetCellColorIndex(int /*col*/,int /*row*/,int /*idx*/){} }; // [METHOD] golden THeatTable*
// -- W906-AutoCleanFoundation ADD: golden TFont* (TPanel->Font) -- only ->Color
//    is ever assigned (SearchCleanNum, next-wave); same tiny data-holder idiom
//    as every other Caption/Color stand-in in this file.
struct TfMainFont { int Color; TfMainFont():Color(0){} };                       // [DATA] golden TFont*
// -- W906-AutoCleanFoundation ADD: golden THeatTable* (fMain->tmyAutoClean, the
//    AutoClean clean-kit grid widget). XItem/YItem/Top/Width/Height are
//    write-only offline (verified by grep -- SetAutoCleanICCount/
//    SetAutoCleanTrayPosition, both next-wave, only ever WRITE them);
//    SetCellColorIndex reuses the same no-op idiom as TfMainGrid above and IS
//    called this wave (AutoClean/AutoClean.cpp's SetCleanCellValue helper).
struct TfMainAutoCleanGrid
{
    int XItem, YItem, Top, Width, Height;
    TfMainAutoCleanGrid():XItem(0),YItem(0),Top(0),Width(0),Height(0){}
    void SetCellColorIndex(int /*col*/,int /*row*/,int /*idx*/){}
};
struct TfMainMemoLines{ int Count; void Add(AnsiString /*s*/){} TfMainMemoLines():Count(0){} }; // [DATA] golden TStrings*
struct TfMainMemo     { TfMainMemoLines *Lines; void Clear(){} TfMainMemo(){ Lines=new TfMainMemoLines(); } }; // [DATA] golden TMemo*
struct TfMainPageControl { int ActivePageIndex; TfMainPageControl():ActivePageIndex(0){} };      // [DATA] golden TPageControl* (pgMain)
struct TfMainSpeedButton { bool Down; TfMainSpeedButton():Down(false){} };                        // [DATA] golden TSpeedButton* (W7-C1: BtnOneCycle->Down, offline false)
// ---------------------------------------------------------------------------
//  W5-Automation (AGV_PortScan unit, 20260713): TfLedValue -- golden TALed*
//  ->Value (bool) shape.  Deliberately SHARED across TfMain (ALed1, main.h:355)
//  AND TfLotInfo (ALedLoader / aLedAuto1-3, uLotInfo.h:965-971) -- all four
//  golden fields are the exact same VCL component type (TALed), not four
//  different widgets, so one stand-in avoids near-duplicate types.
//  Consumer: Automation/AGV_PortScan.cpp (bScanLoadPortState_SPIL /
//  bScanUnLoadPortState_SPIL).
// ---------------------------------------------------------------------------
struct TfLedValue { bool Value; TfLedValue():Value(false){} };  // [DATA] golden TALed*
// ---------------------------------------------------------------------------
//  W5-Automation (AGV_PortScan unit, 20260713): TfMainPanel -- golden TPanel*
//  with only ->Caption read (fMain->labAutomation, main.h:802; compared
//  against the literal "SECS GEM 4:Enable"). Same shape as the existing
//  TfSortCTPanel/TfLotInfoLabel Caption-only stand-ins (kept as its own tiny
//  type here purely to avoid a forward-declaration/ordering dance with
//  TfLotInfoLabel, which is defined later in this same file).  Consumer:
//  Automation/AGV_PortScan.cpp (DoE84LoaderScan / DoE84UnloaderScan).
// ---------------------------------------------------------------------------
struct TfMainPanel { AnsiString Caption; };  // [DATA] golden TPanel* (main.h:802 labAutomation)
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
    bool InArmPlacementEnable();     // [METHOD] golden cInplace -- offline: false (No9 inert)
    TfMainInplace();
};

class TfMain
{
public:
    void DebugOneCycleHotPlate(AnsiString sfunc); // [METHOD] golden main.h:1565 -- debug log sink (empty)
    bool Pause(AnsiString Func);                  // [METHOD] golden main.h:1249 -- offline never pauses -> false
    void ShowTestHeadComp(bool bRefresh);         // [METHOD] golden main.h:1296 -- empty
    void ReStartAutoSiteMapping(bool bStart);     // [METHOD] golden main.h:1331 -- empty
    TfMainSiteMapLog *slAutoSiteMapLog;           // [DATA]   golden main.h:1486 (TMyStringList*) -- new in ctor
    // -- W6.3 ADD: members the TRAY-ARM ENGINE (acatchtray.cpp) derefs ----------
    void CleanOut(AnsiString Func);               // [METHOD] golden main.h -- offline: clean-out no-op
    void DoStateRecord(int i, bool b);            // [METHOD] golden main.h -- offline: state-record sink no-op
    TfMainTrayPanel *mtAuto1;                     // [DATA]   golden main.h (TPanel* mtAuto1)
    TfMainTrayPanel *mtAuto2;                     // [DATA]   golden main.h (TPanel* mtAuto2)
    TfMainTrayPanel *mtAuto3;                     // [DATA]   golden main.h (TPanel* mtAuto3)
    TfMainHanaART   *hanaART;                     // [DATA]   golden main.h (HANA ART helper)
    // -- W6.4 ADD: members the TESTER/INDEX ENGINE (atester.cpp) derefs ----------
    void LightOn();                               // [METHOD] golden main.h -- offline: CCD light no-op (DoTestHeadMotor CCD path)
    TfMainTrayPanel *lbCCDStatus;                 // [DATA]   golden main.h (TLabel* lbCCDStatus); reuse panel stub (->Visible via .Color)
    // -- W6.5 ADD: members the SHUTTLE ENGINE (acarry.cpp) derefs ----------------
    TfMainCheckBox *cbShowShuttleSensor;          // [DATA]   golden main.h (TCheckBox*) -- offline Checked=false
    TfMainCheckBox *cbTestOutShuttleSensor;       // [DATA]   golden main.h (TCheckBox*) -- offline Checked=false
    TfMainCheckBox *cbShowInShuttleSensor;        // [DATA]   golden main.h (TCheckBox*) -- offline Checked=false
    TfMainGrid     *htShullte0;                   // [DATA]   golden main.h (THeatTable* shuttle-1 grid)
    TfMainGrid     *htShullte1;                   // [DATA]   golden main.h (THeatTable* shuttle-2 grid)
    TfMainMemo     *meShuttle1;                   // [DATA]   golden main.h (TMemo* shuttle-1 log)
    TfMainMemo     *meShuttle2;                   // [DATA]   golden main.h (TMemo* shuttle-2 log)
    TfMainInplace  *cInplace;                     // [DATA]   golden main.h (cInplace placement helper)
    TfMainPageControl *pgMain;                    // [DATA]   golden main.h (TPageControl*) -- ActivePageIndex==0 offline
    int emp7TabSheet21;                           // [DATA]   golden main.h (Motor-View tab index) -- 0 offline (==pgMain->ActivePageIndex)
    void AddShuttleMessage(int iSht, AnsiString S);            // [METHOD] golden main.h -- offline log sink no-op
    void Reset(AnsiString Func);                              // [METHOD] golden main.h -- offline: no-op
    void BtnOneCycleClick(void *Sender);                      // [METHOD] golden main.h -- offline: no-op
    void BtnResetClick(void *Sender);                         // [METHOD] golden main.h -- offline: no-op
    void BtnCleanOutClick(void *Sender);                      // [METHOD] golden main.h -- W6.2b(2x4_16): clean-out button; offline no-op
    void JSCC_ResetForShuttleLoseIC();                        // [METHOD] golden main.h -- offline: no-op (0-arg)
    void ResetRecordforPiggyBack(AnsiString S);               // [METHOD] golden main.h -- offline: no-op
    // -- W6.6 ADD: the per-tick sensor scan the HUB main loop calls -------------
    void ProcessSensorScan();                                 // [METHOD] golden main.h -- DoAllProcess():9130 every tick; offline no-op
    // -- W7-C1 ADD: members the END-OF-LOT CLEAN-OUT FINISH-CHECK (DoCleanOutFinishCheck)
    //    derefs.  golden main.h.  All offline no-op / sane default (a handler that
    //    is draining clean-out with no UI never re-starts/re-levels/re-tests).
    //    AI(W7C1-Integrate) 20260629.
    void Start(AnsiString Func);                              // [METHOD] golden main.h -- offline: do NOT auto re-start (no-op); ~6 sites
    void ChangeLevelAttr();                                   // [METHOD] golden main.h -- offline: level-attr UI no-op
    void ModifyTester(int iWhich);                            // [METHOD] golden main.h:Steven 20191218 -- offline: QA tester-modify no-op
    void CleanYieldCount();                                   // [METHOD] golden main.h -- offline: yield-count clear no-op
    TfMainSpeedButton *BtnOneCycle;                           // [DATA]   golden main.h (TSpeedButton* BtnOneCycle); offline Down=false
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
    void SendMSG_CMD(int CMD);                                // [METHOD] golden main.h:1400 -- offline no-op
    void SendMSG_CMD(int CMD, AnsiString Message);            // [METHOD] golden main.h:1401 -- offline no-op
    AnsiString GetSamSungMap(bool bSend=true);                // [METHOD] golden main.h:1531 (body Command.cpp:10137) -- offline: ""
    AnsiString GetSamSungSoakTime(bool bSend=true);           // [METHOD] golden main.h:1532 (body Command.cpp:10305) -- offline: "0"
    AnsiString ArmStatusStrings();                            // [METHOD] golden main.h:1370 (body Command.cpp:1497) -- offline: ""
    // -- W5-Final-SckArtRemainder INTEGRATE ADD: members Automation/SCK_ART_Remainder.cpp
    //    (SckArtRem_AccessFile) derefs -- golden main.h, bodies in main.cpp (untranslated).
    //    All 3 are UI-refresh-only in golden (recipe combo/test-mode picture/backup-on-write);
    //    offline no-op, matching every other fMain UI-refresh sink above.
    void SetStartModeData();                                  // [METHOD] golden main.h -- offline: recipe start-mode UI refresh no-op
    void LoadTestModePicture();                                // [METHOD] golden main.h -- offline: test-mode picture UI refresh no-op
    void BackupSetupFile();                                    // [METHOD] golden main.h -- offline: setup-file backup no-op (Ifor 20170620)
    // -- W5-Automation ADD (AGV_PortScan unit, 20260713): members
    //    Automation/AGV_PortScan.cpp derefs (AMR SPIL port-scan LED +
    //    E84 loader/unloader tray-count scan's SECS-link panel) -----------------
    TfLedValue *ALed1;                                        // [DATA] golden main.h:355 (TALed*) -- bScanLoadPortState_SPIL reads ->Value
    TfMainPanel *labAutomation;                               // [DATA] golden main.h:802 (TPanel*) -- DoE84LoaderScan/DoE84UnloaderScan compare ->Caption
    // -- W906-Automation ADD (20260716): members Automation/automation.cpp
    //    derefs (GetMachineStatus/GetWorkOrder/GetMainTemp + the deferred
    //    ProcessBuffer's own fMain->Home("TfAutomation::ProcessBuffer") call,
    //    golden automation.cpp:1522 -- ProcessBuffer itself is GATED this
    //    wave, see Automation/automation.h, but Home() is added now per this
    //    front's task brief as a small additive cross-file gap). Same
    //    Caption/Text-stub shape already used elsewhere in this file --
    //    palMainStatus reuses TfMainPanel {AnsiString Caption;} (golden
    //    main.h:669 TPanel*), cbSetupFileName reuses TfLotInfoRunMode
    //    {bool Visible; AnsiString Text;} (golden main.h:875 TComboBox*,
    //    only ->Text read here), edWorkTemperBase reuses TfLotInfoEdit
    //    {AnsiString Text;} (golden main.h:732 TEdit*).
    TfMainPanel       *palMainStatus;                         // [DATA] golden main.h:669 (TPanel*)
    TfLotInfoRunMode  *cbSetupFileName;                       // [DATA] golden main.h:875 (TComboBox*) -- only ->Text used
    TfLotInfoEdit     *edWorkTemperBase;                      // [DATA] golden main.h:732 (TEdit*)
    bool Home(AnsiString Func);                                // [METHOD] golden main.h:1276 -- offline: no real Home cycle to run -> false
    // -- W906-TesterTCPTimer ADD (20260720): members Interface/TesterTCP_Socket.cpp's
    //    TimerProcessTCPDataTimer/SimulateBin deref -------------------------------
    TStringList *tTestResult;       // [DATA] golden main.h:1392 (TStringList*) -- ctor pre-fills 32x"-1" (golden main.cpp:2236-2239)
    TStringList *tBarCodeList;      // [DATA] golden main.h:1396 (TStringList*) -- ctor pre-fills 32x"0" then
                                    //   Strings[31]+=";" => "0;" (golden main.cpp:2241 quirk, PRESERVED --
                                    //   makes the default BARCODE? reply end "...,0;;")
    AnsiString SVID1190_OSSetup;    // [DATA] golden main.h:1499 -- SECS SVID1190 backing store (SV consumer
                                    //   uHGemHT9045_SV.cpp:231 untranslated; plain storage here)
    void WritePERSITETemperature(); // [METHOD] golden main.h:1365 (void __fastcall; body Command.cpp:935-943)
                                    //   -- WRAPPER translated faithfully; leaf gated (below)
    AnsiString PERSITETemperatureStrings();  // [METHOD] golden main.h (body Command.cpp:945-1482, ~538 lines +
                                    //   RefreshTempData) -- GATED LEAF: offline returns
                                    //   W906_PERSITETemperatureStrings_Sim (default ""), real body is its own
                                    //   future wave (temp/GPIB surface: fContact->fShow/IndexStatus/
                                    //   iContactMode/asGPIBTempShow/bTestSiteUse)
    AnsiString W906_PERSITETemperatureStrings_Sim;  // [PORT-ONLY SEAM] test-settable stand-in feed for the
                                    //   gated leaf above (same data-driven-facade idiom as GetSamSungMap
                                    //   ""-default / GetSamSungSoakTime "0"-default); default ""
    // -- W906-AutoCleanFoundation ADD (20260721): members the AutoClean foundation
    //    wave's translated functions (AutoClean/AutoClean.cpp) deref. Per-member
    //    touch-scope note (verified by grepping golden -- see the wave's own
    //    report for detail): ONLY chkCleanPadPickErr is actually dereferenced by
    //    a function THIS wave translates (CheckInSuckICFallDown, inside
    //    #ifdef SOFT_SIMULTE). AddAutoCleanMessage / bAutoCleanTest / cbIndexDrop /
    //    pnlCleanCount / AutoCleanContactCountLabel / edHPX / edHPY are all
    //    touched only by the 4 named core engines (DoAutoCleanKit /
    //    DoAutoCleanPickfromCleanKit / DoIndexAutoClean(+variant) / EnableAutoclean /
    //    SearchCleanNum) that are explicitly OUT OF SCOPE this wave (next wave) --
    //    pre-staged here anyway per this wave's task brief, at zero behavioural
    //    risk (plain data / no-op sinks, same idiom as every other fMain member).
    void AddAutoCleanMessage(AnsiString S);       // [METHOD] golden AutoClean.cpp -- offline log sink no-op (next-wave real consumer)
    bool bAutoCleanTest;                          // [DATA]   golden main.h -- offline default false (next-wave real consumer)
    TfMainCheckBox *cbIndexDrop;                  // [DATA]   golden main.h (TCheckBox*) -- offline Checked=false (next-wave)
    TfMainCheckBox *chkCleanPadPickErr;           // [DATA]   golden main.h (TCheckBox*) -- offline Checked=false; ACTIVE this wave (CheckInSuckICFallDown, SOFT_SIMULTE-gated)
    TfMainPanel    *pnlCleanCount;                 // [DATA]   golden main.h (TPanel*) -- Caption only touched here; ->Font->Color is next-wave (SearchCleanNum), see AutoCleanFont below
    TfMainFont     *pnlCleanCountFont;              // [DATA]   golden main.h (TPanel->Font, TFont*) -- next-wave (SearchCleanNum sets clRed/clNavy)
    TfMainPanel    *AutoCleanContactCountLabel;    // [DATA]   golden main.h (TLabel*) -- Caption only (reuse TfMainPanel {AnsiString Caption;}); next-wave (EnableAutoclean)
    TfLotInfoEdit  *edHPX;                        // [DATA]   golden main.h (TEdit*) -- reuse TfLotInfoEdit {AnsiString Text;}; next-wave (DoIndexAutoClean)
    TfLotInfoEdit  *edHPY;                        // [DATA]   golden main.h (TEdit*) -- next-wave (DoIndexAutoClean)
    TfMainAutoCleanGrid *tmyAutoClean;             // [DATA]   golden main.h (THeatTable* clean-kit grid) -- write-only (confirmed by grep: SetAutoCleanICCount/
                                                    //   SetAutoCleanTrayPosition only WRITE XItem/YItem/Top/Width/Height, never read back inside AutoClean.cpp;
                                                    //   a pure no-op/plain-data sink is faithful). ->SetCellColorIndex IS called this wave (Part D's
                                                    //   SetCleanCellValue helper, iMode==eUcleanUsed branch) -- reuses the SAME no-op idiom as htShullte0/1.
    // AI(W906-AutoCleanCluster) 20260722: golden main.h:164 `TTMyTray *mtPlate2;`
    // -- the HotPlate-2 clean-kit grid widget SetAutoCleanTrayPosition's
    // bE43AutoCleanUseHotplate branch reads (->Top/->Width/->Height only,
    // confirmed by grep). Sibling widget of tmyAutoClean above (both are golden
    // TTMyTray*), so reuses the SAME TfMainAutoCleanGrid stand-in -- this is the
    // ONLY FormsFacade.h member this wave adds.
    TfMainAutoCleanGrid *mtPlate2;                  // [DATA]   golden main.h:164 (TTMyTray*) -- read-only here (Top/Width/Height)
    TStringGrid *AutoCleanStringGrid;               // [DATA]   golden main.h (TStringGrid*) -- REAL backing store (see header banner); RestoreCleanKitData/
                                                    //   CheckCleaningCount (this wave) read Cells[][] back via atoi(). Default-constructed 5x5 (vclcompat
                                                    //   default); a future wave's SetAutoCleanICCount translation resizes it via ->ColCount=/->RowCount=.
    TfMain();
};
extern TfMain *fMain;

// ===========================================================================
//  TfSortCT -- non-VCL stub (golden cSortCT.h).  pnlHP1/pnlHP2 are TPanel* in
//  the golden; the leaves only assign ->Caption an AnsiString (HowManyIC()).
//  Stub each as a tiny {AnsiString Caption;} so `fSortCT->pnlHP1->Caption=...`
//  compiles.  Used inside `if(CosFunction.bShowHPICCount)` (default false).
// ===========================================================================
struct TfSortCTPanel { AnsiString Caption; };     // [DATA] golden TPanel* (cSortCT.h:61/63)
class TfSortCT
{
public:
    TfSortCTPanel *pnlHP1;
    TfSortCTPanel *pnlHP2;
    // -- W6.3 ADD: tray panels the TRAY-ARM ENGINE (acatchtray.cpp) derefs -------
    //    golden cSortCT.h TPanel* members; the catchtray engine only assigns
    //    ->Caption (AnsiString; HowManyIC() returns int -> AnsiString int-ctor).
    TfSortCTPanel *pnlLoad;                       // [DATA] golden cSortCT.h (loader IC-count panel)
    TfSortCTPanel *pnlLoadCID;                    // [DATA] golden cSortCT.h (loader cover-ID panel)
    TfSortCTPanel *pnlCoverTrayD;                 // [DATA] golden cSortCT.h (color cover-tray-ID panel)
    TfSortCTPanel *pnlTrayCnt[6];                 // [DATA] golden cSortCT.h (per-Auto tray-count panels)
    TfSortCT();
};
extern TfSortCT *fSortCT;

// ===========================================================================
//  TfLotInfo -- non-VCL stub (golden uLotInfo.h).  cbRunMode is TComboBox* in
//  the golden; the leaves deref ->Visible (bool) and ->Text.Pos("RT")
//  (AnsiString).  Stub cbRunMode as {bool Visible; AnsiString Text;}.
//  Used inside the iResetSiteMappingStep==2 ASM path (offline: Visible=false).
// ===========================================================================
struct TfLotInfoRunMode { bool Visible; AnsiString Text; TfLotInfoRunMode():Visible(false){} };
struct TfLotInfoLabel   { AnsiString Caption; };  // [DATA] golden uLotInfo.h (TLabel* labNowLoaderTrayID)
struct TfLotInfoEdit    { AnsiString Text; };     // [DATA] golden uLotInfo.h (TEdit*  edtSysLotID)
// -- W5-Final-TesterTCPSocket INTEGRATE ADD: golden TLabel* shape (Caption +
//    Color); same {Caption;Color} shape as TfMainTrayPanel but int Color has
//    no default-visibility meaning here, only a colour code -- kept as its own
//    tiny struct to match golden's actual TLabel (not TPanel) member type.
struct TfLotInfoStatusLabel { AnsiString Caption; int Color; TfLotInfoStatusLabel():Color(0){} };
// -- W5-Automation ADD (AGV_PortScan unit, 20260713): golden TPanel* shape
//    (fLotInfo->palRemoveTray, uLotInfo.h:828) -- both ->Enabled and
//    ->Visible are written (DoE84UnloaderScan case 1200), never read anywhere
//    in the translated tree yet; defaulted true/true (ordinary VCL TPanel
//    design-time defaults) since no consumer depends on the initial value.
struct TfLotInfoPanel { bool Enabled; bool Visible; TfLotInfoPanel():Enabled(true),Visible(true){} };
class TfLotInfo
{
public:
    TfLotInfoRunMode *cbRunMode;                  // [DATA] golden uLotInfo.h:307 (TComboBox*)
    // -- W6.3 ADD: members the TRAY-ARM ENGINE (acatchtray.cpp) derefs -----------
    TfLotInfoLabel   *labNowLoaderTrayID;         // [DATA] golden uLotInfo.h (loader tray-ID label)
    TfLotInfoEdit    *edtSysLotID;                // [DATA] golden uLotInfo.h (system lot-ID edit)
    // AI(W906-AutoCleanFoundation) 20260721: golden uLotInfo.h:1264 `int
    // iUnloaderTask[3];` (Eastsun 20260515 F011) -- the backing store
    // InitialUnLoaderTask (below) writes. Was missing entirely (the previous
    // stand-in's InitialUnLoaderTask didn't even declare it) -- see
    // InitialUnLoaderTask's own comment for the behaviour-change note.
    int iUnloaderTask[3];                         // [DATA] golden uLotInfo.h:1264
    // golden uLotInfo.cpp:16250-16253 -- REAL one-line body (was a total no-op
    // before this wave). 5 existing call sites in acatchtray.cpp (all inside
    // `#ifdef SOFT_SIMULTE`, which is #undef'd/commented-out in MachineType.h
    // -- verified via grep -- so this is currently dormant on every compiled
    // path; a genuine no-op -> real-write behaviour change ONLY if/when
    // SOFT_SIMULTE is ever defined). No existing test references
    // InitialUnLoaderTask or iUnloaderTask (grepped tests/ -- zero hits).
    void InitialUnLoaderTask(int iPos);           // [METHOD] golden uLotInfo.h -- REAL body: iUnloaderTask[iPos]=1;
    // -- W5-Automation INTEGRATE ADD: members Automation/AMR.cpp + HANA_ART.cpp derefs --
    TfLotInfoRunMode *cbProcess;                  // [DATA] golden uLotInfo.h (TComboBox*; only ->Text used) -- reuse TfLotInfoRunMode shape
    void RefreshAMR();                            // [METHOD] golden uLotInfo.h:1416 -- offline: no UI to refresh (no-op)
    void SetLotID(AnsiString ID, bool bReadFromFile=false);       // [METHOD] golden uLotInfo.h:1310 -- offline no-op
    void SetLotStart(AnsiString sFunc, bool bReadFromFile=false); // [METHOD] golden uLotInfo.h:1311 -- offline no-op
    // -- W5-Final-TesterTCPSocket INTEGRATE ADD: members Interface/TesterTCP_Socket.cpp
    //    derefs (golden TesterTCP.cpp:170/180/621 labTCPIPStatus, :290/294 mmTesterLog) --
    TfLotInfoStatusLabel *labTCPIPStatus;         // [DATA] golden uLotInfo.h (TLabel* TCP/IP link-status)
    TfMainMemo           *mmTesterLog;            // [DATA] golden uLotInfo.h (TMemo* tester comm-log) -- reuse TfMainMemo shape (fAGV->mmE84Log precedent)
    // -- W5-Automation ADD (AGV_PortScan unit, 20260713): members
    //    Automation/AGV_PortScan.cpp derefs (AMR SPIL port-scan LEDs +
    //    E84 unloader tray-count scan's "remove tray" prompt panel) ----------
    TfLedValue     *ALedLoader;                   // [DATA] golden uLotInfo.h:971 (TALed*)
    TfLedValue     *aLedAuto[3];                  // [DATA] golden uLotInfo.h:965,969,970 (TALed* aLedAuto1/2/3, consolidated into an array -- same convention as fSortCT->pnlTrayCnt[6])
    TfLotInfoPanel *palRemoveTray;                 // [DATA] golden uLotInfo.h:828 (TPanel*, Enabled+Visible)
    TfLotInfo();
};
extern TfLotInfo *fLotInfo;

// ===========================================================================
//  TfOffSet -- non-VCL stub (golden cOffSet.h).  Both methods return false
//  offline (no auto-offset / no setup-teach configured).
// ===========================================================================
class TfOffSet
{
public:
    bool UseAutoOffsetFunction(AnsiString sName);  // [METHOD] golden cOffSet.h:488 -> false
    bool UseInArmSetupTeach(int iWhich);           // [METHOD] golden cOffSet.h:489 -> false
};
extern TfOffSet *fOffSet;

// ===========================================================================
//  TfSCKART -- non-VCL stub (golden Automation/SCK_ART.h).  iInputJamCnt is
//  only incremented (inside `if(CosFunction.bUseSCKART)`, default false).
// ===========================================================================
class TfSCKART
{
public:
    int iInputJamCnt;                              // [DATA] golden SCK_ART.h:288
    // -- W6.3 ADD: members the TRAY-ARM ENGINE (acatchtray.cpp) derefs -----------
    int iFTRTCount;                                // [DATA] golden SCK_ART.h (FT/RT count; CC_TERAPOWER ART)
    int iInputCount;                               // [DATA] golden SCK_ART.h (ART input count)
    int  CheckLoadingCount();                      // [METHOD] golden SCK_ART.h -- offline: 0 (no ART loading mismatch; golden compares ==0/==1/==2)
    // -- W6.5 ADD: member the SHUTTLE ENGINE (acarry.cpp) derefs ----------------
    void AddOutputJamCnt(int row, int col, int ret, int iBinOnCarryKit=0);  // [METHOD] golden Automation/SCK_ART.h:302 -- offline: no-op (inside if(bUseSCKART), default false)
    // -- W5-Automation INTEGRATE ADD: members Automation/AMR.cpp + HANA_ART.cpp derefs --
    int  iCurrent93KARTStep;                       // [DATA] golden SCK_ART.h:263 -- needed by both AMR.cpp and HANA_ART.cpp
    AnsiString sLotID;                              // [DATA] golden SCK_ART.h:237
    AnsiString sProcessCode;                        // [DATA] golden SCK_ART.h:238
    void DoARTLotStart(AnsiString _sLotID, AnsiString _sProcessCode, int _iLotCount);  // [METHOD] golden SCK_ART.h:347 -- offline no-op
    void AccessFile(bool bRead, int iAccess=-1);    // [METHOD] golden SCK_ART.h:271 -- offline no-op
    TfSortCTPanel *palLotNumber;                    // [DATA] golden SCK_ART.h (TPanel* lot-number caption) -- reuse TfSortCTPanel {AnsiString Caption;}
    TfSortCTPanel *palTestCnt;                      // [DATA] golden SCK_ART.h (TPanel* FT/RT test-count caption)
    TfSortCTPanel *palRTTryCnt;                      // [DATA] golden SCK_ART.h (TPanel* RT-try-count caption)
    TfSortCTPanel *pnlProcessCode;                   // [DATA] golden SCK_ART.h (TPanel* process-code caption)
    TfLotInfoEdit  *edlRTTryCnt;                     // [DATA] golden SCK_ART.h (TEdit* RT-try-count edit) -- reuse TfLotInfoEdit {AnsiString Text;}
    TfSCKART();
};
extern TfSCKART *fSCKART;

// ===========================================================================
//  W906-AutoCleanFoundation ADD (20260721): TfCleaning -- non-VCL stand-in
//  (golden AutoClean/uCleaning.h `class TfCleaning : public TForm`). Golden's
//  real class backs the 2,916-line AutoClean settings-panel VCL form
//  (uCleaning.cpp) -- entirely OUT OF SCOPE this wave, per the task brief;
//  this is ONLY a minimal facade stand-in for the handful of members the
//  translated AutoClean/AutoClean.cpp (+ this wave's Part D free functions)
//  actually deref, same "form as junk drawer" idiom as TfMain/TfLotInfo/
//  TfSortCT above.
// ===========================================================================
class TfCleaning
{
public:
    TfLotInfoEdit *edCleaningCount;   // [DATA] golden uCleaning.h (TEdit*) -- reuse TfLotInfoEdit {AnsiString Text;}; SearchCleanNum (next-wave)
    int  iDeviceCount;                 // [DATA] golden uCleaning.h -- offline default 0
    bool bResetCleanCount;             // [DATA] golden uCleaning.h -- offline default false
    bool b1x2SiteAbClosePutDummy;      // [DATA] golden uCleaning.h -- ACTIVE this wave: SetShuttleIcForSpecialMode (AutoClean.cpp) + CleanPadCountCanSupport2Arm (Part D)
    TfCleaning();
};
extern TfCleaning *fCleaning;

#endif // FormsFacadeH
