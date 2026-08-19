// =============================================================================
//  forms/fLotInfo.h  --  non-VCL stand-in for golden's fLotInfo form pointer
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.h by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Content moved VERBATIM apart
//  from the F0-e virtualisation; the widget stand-ins (TfLotInfoRunMode /
//  TfLotInfoLabel / TfLotInfoEdit / TfLotInfoStatusLabel / TfLotInfoPanel /
//  TfMainMemo / TfLedValue) now come from forms/FormWidgets.h.
//  Facade-wide contract: see forms/fMain.h.
// =============================================================================
#ifndef FORMS_FLOTINFO_H
#define FORMS_FLOTINFO_H

#include "forms/FormWidgets.h"
#include "MachineType.h"   // AI(W906-FW3-LotInfo-WA) 20260819: ATC_HEAD_COUNT (32) sizes ATCChPal[]/ATCPtr[]/ATCReferPtr[] below

// =============================================================================
//  AI(W906-FW3-LotInfo-WA) 20260819: uLotInfo displayside Wave A.
//
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/uLotInfo.cpp (16,613 lines,
//  cp950 -- decoded with `python3 -c "open(...,'rb').read().decode('cp950')"`,
//  0 U+FFFD, this wave) + uLotInfo.h (1,426 lines). Source recon:
//  docs/RECON_uLotInfo_displayside.md Tier 1 (39 methods, 1,199 golden lines,
//  section 5.1) -- every one of those 39 already had its full body read
//  line-by-line by that recon; this wave re-read each one again before
//  translating (per task brief step 4) and found 2 further RECON MISSES
//  documented in the GATE REGISTER below (WA-2, WA-6).
//
//  WAVE SCOPE -- the 39 Tier 1 methods (golden span, this file's status)
//  --------------------------------------------------------------------------
//    RefreshYieldMonitor_SIGURD      golden :13327-13545 (219L) -- REAL, 1 line gated (WA-1)
//    TransformTemperature_AirStream  golden :15080-15193 (114L) -- REAL, 1 block gated (WA-2)
//    AdjtsYieldMonitiorSize          golden :13636-13717 (82L)  -- REAL, 2 calls gated (WA-3)
//    SetSelectionVisible             golden :1262-1342   (81L)  -- REAL, 1 loop gated (WA-4)
//    ShowSocketID                    golden :11443-11516 (74L)  -- REAL, no gate
//    CheckAirMachineStatus           golden :14805-14868 (64L)  -- early-return only REAL, rest gated (WA-6)
//    InitialRefrigerantSystem        golden :14877-14939 (63L)  -- REAL, 1 call gated (WA-5)
//    SetATCFormVisible               golden :9938-9985   (48L)  -- REAL, no gate
//    CheckEventLogParameter          golden :10673-10720 (48L)  -- REAL, no gate
//    ShowATCTempPanel                golden :14561-14606 (46L)  -- REAL, no gate
//    JCETWhite2DIDShow               golden :15953-15992 (40L)  -- REAL, no gate
//    ShowInformation                 golden :12326-12362 (37L)  -- REAL, no gate
//    N23UseLotInfoFile               golden :7346-7380   (35L)  -- REAL, no gate
//    edTempKeyUp                     golden :4966-4999   (34L)  -- REAL, no gate (params dropped, unused in golden)
//    edtSysLotIDKeyPress             golden :11892-11917 (26L)  -- REAL, no gate (Sender/Shift dropped, unused)
//    SettsChipAdvVisible             golden :1240-1260   (21L)  -- REAL, no gate
//    bCheckOnlyOneFile               golden :14193-14213 (21L)  -- REAL, no gate
//    edPageKeyPress                  golden :11919-11933 (15L)  -- REAL, no gate (Sender/Shift dropped)
//    CheckActionFlag                 golden :16234-16246 (13L)  -- REAL, no gate
//    SetTesterStartTimeByB03         golden :14298-14309 (12L)  -- REAL, no gate
//    btnLoadFileClick                golden :14175-14184 (10L)  -- REAL, no gate (Sender dropped, unused)
//    cbRunModeKeyDown                golden :11990-11997 (8L)   -- body gated (WA-7)
//    cbRunModeKeyUp                  golden :11999-12006 (8L)   -- body gated (WA-7)
//    edDeviceNameKeyUp               golden :12044-12051 (8L)   -- REAL, no gate (params dropped, unused)
//    ReflashInfo                     golden :16479-16485 (7L)   -- REAL, no gate
//    labLotIDMouseDown               golden :8423-8428   (6L)   -- body gated (WA-8)
//    edtLotVerifyMouseDown           golden :14515-14520 (6L)   -- REAL, no gate (params dropped, unused)
//    btnCancelTestPauseClick         golden :12297-12301 (5L)   -- REAL, no gate (Sender dropped, unused)
//    edtLotVerifyKeyPress            golden :14522-14526 (5L)   -- REAL, no gate (Sender/Key dropped, unused)
//    BtnPauseMouseDown               golden :14533-14537 (5L)   -- REAL, no gate (params dropped, unused)
//    BtnPauseMouseUp                 golden :14539-14543 (5L)   -- REAL, no gate (params dropped, unused)
//    VisibleUploadBtnPAT             golden :15710-15714 (5L)   -- REAL, no gate
//    cbbDeviceNameChange             golden :7216-7219   (4L)   -- REAL, no gate (Sender dropped, unused)
//    btStartCountClick               golden :8418-8421   (4L)   -- REAL, no gate (Sender dropped, unused)
//    edtASECL_LotIDClick             golden :10491-10494 (4L)   -- body gated (WA-8)
//    btTesterTCPShowClick            golden :13841-13844 (4L)   -- body gated (WA-9)
//    InitialLoaderTask               golden :16259-16262 (4L)   -- REAL, no gate
//    InitialLDLevelTask              golden :16264-16267 (4L)   -- REAL, no gate
//  (InitialUnLoaderTask, golden :16250-16253, is the 39th Tier 1 method --
//  already translated by the AI(W906-AutoCleanFoundation) 20260721 wave, see
//  its own member/method comments below; not re-touched here.)
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (WA-1) RefreshYieldMonitor_SIGURD, golden :13346 --
//      `cbContactMode->ItemIndex=fContact->cbContactMode->ItemIndex;`.
//      RECON logged this dependency as "fContact->cbContactMode（fContactCT.h
//      已存在但成員需查）" -- CORRECTION found this wave: `fContactCT.h`
//      declares class `TfContactCT` / global `fContactCT`, NOT `TfContact` /
//      `fContact` -- two different golden classes with similar names. `fContact`
//      itself does not exist ANYWHERE in this tree (grepped `class TfContact\b`
//      and `\bfContact\b` tree-wide, 20260819 -- 0 hits outside this comment).
//      Gated at the single assignment; `cbContactMode` (fLotInfo's own combo)
//      is still declared and constructed below so future integration has
//      something to assign into.
//  (WA-2) TransformTemperature_AirStream, golden :15134-15169 -- the
//      `if(!tIni->ValueExists("Temp_Index","-60~-51")) { ...16x
//      tIni->WriteString(...)... }` lazy-init block. RECON's one-line summary
//      ("純查表計算回傳 double...無其他依賴") MISSED this: it is a real
//      config-file write to `D:\HT9045\config\AirStream.ini` on first run.
//      Per AGENTS.md / CLAUDE.md, `config\` is a production-machine shared
//      runtime-parameter directory, default read-only, changed only on
//      explicit user request -- so the write is gated rather than silently
//      ported. BEHAVIOUR DELTA while gated: `ReadString` below still runs
//      against whatever the ini already has (real read, unaffected); on a
//      brand-new `AirStream.ini` missing the "Temp_Index"/"Temp_Socket"
//      sections the lookup falls through to the `sLoadIni=""` default
//      (`atof("")==0`), instead of golden's built-in fallback table. Same
//      "gate made a real local go quiet" pattern is NOT needed here (no local
//      var loses its only use -- the surrounding classification/clamp logic
//      is unconditionally real).
//  (WA-3) AdjtsYieldMonitiorSize, golden :13671 and :13678 -- both
//      `RefreshYieldMonitor();` calls. `RefreshYieldMonitor` itself (golden
//      :13310-13325) is RECON item #115, classified (b) write-path: its
//      non-SIGURD (else) branch calls `RefreshYieldMonitor_TERAPOWER`, which
//      (RECON's own finding, section 4 item 3) calls
//      `fCleaning->ChangeACSmartInterval(2,...)` -- a real AutoClean-interval
//      config write, and moreover THE DEFAULT branch for most customer codes.
//      `RefreshYieldMonitor` is not one of this wave's 39 Tier-1 methods, so
//      per the task's dependency rule (missing sibling method -> gate, do not
//      invent a shim) both call sites are gated rather than the dispatcher
//      being half-translated. `RefreshAMR();` (golden :13683) is UNGATED: it
//      already exists as a real (no-op) translated method in this file from
//      an earlier wave (AI(W906-...) "W5-Automation ADD" below).
//  (WA-4) SetSelectionVisible, golden :13331-13336 (`for(int
//      i=0;i<groupbDownloadItem->ControlCount;i++) { if(groupbDownloadItem->
//      Controls[i]!=lblDownloadAccessWarning)
//      groupbDownloadItem->Controls[i]->Enabled=!bDenyByOP; }`). This walks
//      every DESIGN-TIME CHILD of a container widget -- `vclcompat::TGroupBox`
//      (vclcompat/Controls.h) has no `Controls[]`/`ControlCount` surface (it
//      only carries Caption/Visible/Enabled, per that header's own class-map
//      note), and adding one is a shared-header change outside this wave's
//      2-file write boundary (`forms/fLotInfo.h`/`.cpp` only). Gated; the
//      `bDenyByOP` calc and both `lblDownloadAccessWarning->Visible=...;`
//      writes (the only OTHER effect of this if/else) stay REAL.
//  (WA-5) InitialRefrigerantSystem, golden :14922 -- `ATC_OFFLINE_FormComInit();`.
//      That method is RECON Tier 2 (section 5.2: "grepped clean; NOT fully
//      body-read"), not one of this wave's 39 verified Tier-1 methods. Gated
//      rather than reaching into unverified territory to satisfy one call
//      site; `bInitFormcomponent=false;`/`OldRefrigerantCommand=false;`
//      (golden :14920-14921, the two lines immediately around it) stay REAL
//      since they are plain member writes with no dependency on it.
//  (WA-6) CheckAirMachineStatus, golden :14810-14868 -- effectively the WHOLE
//      body after the `AirStream_Select==0` early return. TWO separate,
//      independent reasons, both of which RECON's "(a) 顯示/唯讀 ... verified
//      fully" call (table row #156 / section 5.1 item 6) MISSED:
//        (a) every remaining line dereferences `ATC_InterfaceForm->
//            AirMachineInfo`/`.AirMachineInfo_Index`/`.IsConnect()`.
//            `ATC_InterfaceForm` is `TATC_InterfaceFormShim*`
//            (acarry_shims.h:109-115) and that shim carries EXACTLY ONE
//            member, `int iATC_MODE_TYPE` -- verified by reading the class
//            this wave (20260819). `AirMachineInfo`/`AirMachineInfo_Index`/
//            `IsConnect()` do not exist anywhere in this tree (grepped
//            tree-wide, 20260819 -- 0 hits outside golden/docs). Adding them
//            means editing `acarry_shims.h`, outside this wave's write
//            boundary.
//        (b) even where (a) did not apply, golden :14843-14858 is
//            `if(SystemStart){ if(Temperature.EnableAirMachineSocket){
//            ...ShowMyMessage(...)... } else if(...)
//            ShowErrorMessage("WAR1611", K_RETRY, MMSystem); }` -- a REAL
//            alarm-raising call (`ShowErrorMessage`, canary_support.h:66,
//            drives real Retry/Skip SM plumbing per its own header banner),
//            gated for the identical reason forms/fTemperFrom.h's GATE (T1)
//            gates its own `ShowErrorMessage("WAR15xx",...)` calls: raising a
//            real alarm channel is an operational behaviour change reserved
//            for explicit user sign-off, not a translation-fidelity question.
//      Net effect: only `if(AirStream_Select==0) return;` is real; nothing
//      else compiles into this function this wave, and consequently NONE of
//      the ~15 `palAirMachine*`/`pal_Air*`/`pnl_AirMachine*` Caption widgets
//      this function would otherwise need are declared below (adding them
//      would be unused surface -- same "don't add scope-creep surface" call
//      forms/fTemperFrom.h's own banner makes).
//  (WA-7) cbRunModeKeyDown / cbRunModeKeyUp, golden :11993 / :12002 --
//      `fBarCode->JCETUseMakeWhite2DIDList()`. Already an ESTABLISHED
//      tree-wide gate: Public/MyProductionRecord.cpp:1198 GATE G-1 gates the
//      identical golden call for the identical reason (`fBarCode`, i.e.
//      `TfBarCode_Shim*` per aHotPlateSubstrate.h:1018-1026, has no such
//      member -- verified again this wave). Both handlers reduce to a no-op
//      when gated, matching the G-1 precedent's own "ACTIVE arm: no-op" idiom.
//  (WA-8) labLotIDMouseDown (golden :8427) / edtASECL_LotIDClick (golden
//      :10493) -- `fQwertyKey->ShowQwertyKey(...)`. Already an ESTABLISHED
//      tree-wide gate (ATC/ATCInterface.cpp GATE (3)/(4), VacuumUnit.cpp GATE
//      (3), VacuumUnit/MyVacuumPanel.cpp GATE (3), EJ1N/MyOmronPanel.cpp GATE
//      (3), Public/HTEdit.cpp GATE (6) -- all cite "fQwertyKey has no port
//      anywhere in this tree"). Re-verified this wave (20260819): still 0
//      compiled (non-`#if 0`) definitions of `class TfQwertyKey` or a real
//      `fQwertyKey` global tree-wide. Both handlers reduce to a no-op.
//  (WA-9) btTesterTCPShowClick, golden :13843 -- `fTesterTCP->Show();`.
//      Interface/TesterTCP.h's own file banner (line 59) states plainly this
//      TU is "NOT a TfTesterTCP class or facade at all, only free functions".
//      `fTesterTCP` does not exist. Gated; reduces to a no-op.
//
//  DEVIATION -- event-handler signatures drop unused VCL glue parameters
//  --------------------------------------------------------------------------
//  Every golden `__fastcall` event handler in this batch (KeyUp/KeyPress/
//  KeyDown/MouseDown/MouseUp/Click) carries `TObject *Sender` plus, per event
//  kind, `WORD &Key`/`char &Key`/`TShiftState Shift`/mouse-button+coordinate
//  params. `__fastcall` itself is dropped tree-wide (vclcompat/vcl_compat.h's
//  own note: it is a compiler keyword this toolchain does not know, so a
//  neutralizing macro can never fire -- every other translated handler in
//  this tree already omits it). Of the params: NONE of `Sender`/`Shift`/mouse
//  X,Y/mouse Button are ever read by any of these 39 bodies (verified
//  per-function this wave); `Key` IS read (by value, never re-assigned) in
//  exactly 2 of them (`edtSysLotIDKeyPress`, `edPageKeyPress`) so those two
//  keep a plain `char Key` parameter. This is the same signature-simplification
//  idiom forms/fTemperFrom.h's own DEVIATION note documents for `FormClose`
//  dropping `(TObject*,TCloseAction&)` -- unread parameters, not a behaviour
//  change.
//
//  DEVIATION -- `fLotInfo->Height/Width/Top/Left` translated as bare
//  (implicit `this->`) member access
//  --------------------------------------------------------------------------
//  AdjtsYieldMonitiorSize/ShowInformation (both TfLotInfo methods) read golden
//  source that spells its OWN form's geometry through the global pointer,
//  e.g. `fLotInfo->Height=610;`, even from inside TfLotInfo's own methods --
//  ordinary Delphi/BCB6 style, not a cross-form reference. Translated as bare
//  `Height=610;` (i.e. `this->Height`). Under this tree's established
//  single-instance assumption (`this == fLotInfo` for every production call
//  path, same assumption forms/fTemperFrom.h's own banner names explicitly)
//  the two spellings are semantically identical; the bare form is additionally
//  the SAFER one for tests that construct their own local `TfLotInfo`
//  instance (`fLotInfo->Height` would silently write into the unrelated
//  global singleton instead of the instance under test, whereas bare
//  `Height` always writes into whichever object the method was actually
//  invoked on). `this->Top=205;`/`this->Left=340;` in golden ShowInformation
//  already used the bare/this-> spelling explicitly, translated verbatim.
// =============================================================================

// ===========================================================================
//  AI(W906-FW3-LotInfo-WA) 20260819 -- Wave A composed widget stand-ins.
//  vclcompat's stock TPanel/TSpeedButton/TPageControl carry no raw pixel
//  geometry (Left/Top/Width/Height) or child-enumeration surface (that is a
//  deliberate "no rendering" scope call made elsewhere -- see
//  vclcompat/Controls.h). A handful of Tier 1 methods only ever WRITE a
//  couple of geometry ints, or read back one extra pointer VCL's real
//  TPageControl carries (->ActivePage) that this facade's TPageControl does
//  not. Composing (public-inherit + add the one missing field), not editing
//  the shared vclcompat header, is the same "compose, don't fork" idiom
//  forms/fTemperFrom.h's own TfTemperFromPanel/TfTemperFromTimer already use.
// ===========================================================================

// golden TPanel* Panel28 (->Width only) / Pan_ATC_Use_4Head,_8Head,_32Head
// (->Left/->Top only). One reusable type for all 4 -- nothing in Tier 1 reads
// any of these back, so all 4 ints default 0 (write-only within this wave).
class TfLotInfoLayoutPanel : public vclcompat::TPanel
{
public:
    int Left, Top, Width, Height;
    TfLotInfoLayoutPanel() : Left(0), Top(0), Width(0), Height(0) {}
};

// golden TSpeedButton* sbTest (->Top only, ShowInformation).
class TfLotInfoLayoutButton : public vclcompat::TSpeedButton
{
public:
    int Top;
    TfLotInfoLayoutButton() : Top(0) {}
};

// golden TPageControl* pgLotinfo -- AdjtsYieldMonitiorSize compares
// ->ActivePage against 15 different TTabSheet* members (real VCL TPageControl
// carries this pointer; vclcompat::TPageControl only models ->ActivePageIndex,
// which ShowInformation separately needs). Defaults NULL: no tab is
// "definitively active" offline, the same conservative-default posture as
// every other Visible/Enabled default in this facade -- AdjtsYieldMonitiorSize
// falls through every ->ActivePage== branch to its final `else` until a future
// wave wires a real active-tab notion.
class TfLotInfoPageControl : public vclcompat::TPageControl
{
public:
    vclcompat::TTabSheet *ActivePage;
    TfLotInfoPageControl() : ActivePage(0) {}
};

// golden TEdit* edTemp -- edTempKeyUp writes ->SelStart (cursor position),
// which vclcompat::TEdit does not model (a pure rendering/cursor concern,
// same "no rendering" scope call as every other geometry field above). Never
// read back anywhere in Tier 1's scope -- write-only int, default 0.
class TfLotInfoTextEdit : public vclcompat::TEdit
{
public:
    int SelStart;
    TfLotInfoTextEdit() : SelStart(0) {}
};

// golden TOpenDialog* OpenDialog1 (btnLoadFileClick: ->Title, ->Execute(),
// ->FileName). No vclcompat model exists (no prior consumer). Offline
// Execute() always returns false ("user cancelled") -- there is no real
// modal file picker in a headless build, matching this tree's established
// "no UI -> the interactive path never completes" convention (e.g.
// cinitial.cpp's own `W8N2_ShowMyMessageBox_YES_NO` returning a fixed "NO").
class TfLotInfoOpenDialog : public vclcompat::TObject
{
public:
    AnsiString Title;
    AnsiString FileName;
    TfLotInfoOpenDialog() {}
    virtual ~TfLotInfoOpenDialog() {}
    bool Execute() { return false; }
};

// ===========================================================================
//  TfLotInfo -- non-VCL stub (golden uLotInfo.h).  cbRunMode is TComboBox* in
//  the golden; the leaves deref ->Visible (bool) and ->Text.Pos("RT")
//  (AnsiString).  Used inside the iResetSiteMappingStep==2 ASM path
//  (offline: Visible=false -- see forms/FormWidgets.h, that default is
//  LOAD-BEARING: ainarm_SearchPlacePlate.cpp:4811 reads it).
// ===========================================================================
class TfLotInfo
{
public:
    TfLotInfoRunMode *cbRunMode;                  // [DATA] golden uLotInfo.h:307 (TComboBox*)
    // -- W6.3 ADD: members the TRAY-ARM ENGINE (acatchtray.cpp) derefs -----------
    TfLotInfoLabel   *labNowLoaderTrayID;         // [DATA] golden uLotInfo.h:1078 (TLabel* loader tray-ID label)
    TfLotInfoEdit    *edtSysLotID;                // [DATA] golden uLotInfo.h:302  (TEdit* system lot-ID edit)
    // AI(W906-FW-Y3) 20260819: the 5 AutoClean-display members whose absence
    // kept uYieldMonitoring's (Y3) CalculateSiteYield block gated.
    TfLotInfoLabel   *Label17;                    // [DATA] golden uLotInfo.h:48 (TLabel* AutoClean low-yield user-set line)
    TfLotInfoLabel   *Label18;                    // [DATA] golden uLotInfo.h:49 (TLabel* AutoClean site-yield-diff user-set line)
    TfLotInfoLabel   *Label21;                    // [DATA] golden uLotInfo.h:55 (TLabel* AutoClean interval/contact user-set line)
    TfLotInfoEdit    *edtAutoCleanLowYield;       // [DATA] golden uLotInfo.h:50 (TEdit* live low-yield threshold display)
    TfLotInfoEdit    *edtAutoCleanSiteYieldDiff;  // [DATA] golden uLotInfo.h:51 (TEdit* live site-yield-diff display)
    // AI(W906-AutoCleanFoundation) 20260721: golden uLotInfo.h:1264 `int
    // iUnloaderTask[3];` (Eastsun 20260515 F011) -- the backing store
    // InitialUnLoaderTask (below) writes. Was missing entirely (the previous
    // stand-in's InitialUnLoaderTask didn't even declare it) -- see
    // InitialUnLoaderTask's own comment for the behaviour-change note.
    int iUnloaderTask[3];                         // [DATA] golden uLotInfo.h:1264
    // golden uLotInfo.cpp:16250-16253 -- REAL one-line body (was a total no-op
    // before that wave). 5 existing call sites in acatchtray.cpp (all inside
    // `#ifdef SOFT_SIMULTE`, which is #undef'd/commented-out in MachineType.h
    // -- verified via grep -- so this is currently dormant on every compiled
    // path; a genuine no-op -> real-write behaviour change ONLY if/when
    // SOFT_SIMULTE is ever defined). No existing test references
    // InitialUnLoaderTask or iUnloaderTask (grepped tests/ -- zero hits).
    virtual void InitialUnLoaderTask(int iPos);    // [METHOD] golden uLotInfo.h -- REAL body: iUnloaderTask[iPos]=1;
    // -- W5-Automation INTEGRATE ADD: members Automation/AMR.cpp + HANA_ART.cpp derefs --
    TfLotInfoRunMode *cbProcess;                  // [DATA] golden uLotInfo.h:470 (TComboBox*; only ->Text used)
    virtual void RefreshAMR();                    // [METHOD] golden uLotInfo.h:1416 -- offline: no UI to refresh (no-op)
    virtual void SetLotID(AnsiString ID, bool bReadFromFile=false);       // [METHOD] golden uLotInfo.h:1310 -- offline no-op
    virtual void SetLotStart(AnsiString sFunc, bool bReadFromFile=false); // [METHOD] golden uLotInfo.h:1311 -- offline no-op
    // -- W5-Final-TesterTCPSocket INTEGRATE ADD: members Interface/TesterTCP_Socket.cpp
    //    derefs (golden TesterTCP.cpp:170/180/621 labTCPIPStatus, :290/294 mmTesterLog) --
    TfLotInfoStatusLabel *labTCPIPStatus;         // [DATA] golden uLotInfo.h:465 (TLabel* TCP/IP link-status)
    TfMainMemo           *mmTesterLog;            // [DATA] golden uLotInfo.h:468 (TMemo* tester comm-log) -- reuse TfMainMemo shape (fAGV->mmE84Log precedent)
    // -- W5-Automation ADD (AGV_PortScan unit, 20260713): members
    //    Automation/AGV_PortScan.cpp derefs (AMR SPIL port-scan LEDs +
    //    E84 unloader tray-count scan's "remove tray" prompt panel) ----------
    TfLedValue     *ALedLoader;                   // [DATA] golden uLotInfo.h:971 (TALed*)
    TfLedValue     *aLedAuto[3];                  // [DATA] golden uLotInfo.h:965,969,970 (TALed* aLedAuto1/2/3, consolidated into an array -- same convention as fSortCT->pnlTrayCnt[6])
    TfLotInfoPanel *palRemoveTray;                 // [DATA] golden uLotInfo.h:828 (TPanel*, Enabled+Visible) -- see the ctor: its true/true initial state is set EXPLICITLY
    // AI(W906-Save2DSortingSummary) 20260723: 6 new members Automation/SCK_ART_Remainder.cpp's
    // SckArtRem_Save2DSortingSummary derefs (golden SCK_ART.cpp:3402-4061, only ->Text read/each --
    // reuse the existing TfLotInfoEdit shape, same idiom as edtSysLotID above).
    // mmo2DLotInfo is a TMemo* in golden (uLotInfo.h:846, `sList->Text=fLotInfo->mmo2DLotInfo->Text;`,
    // golden :3628) -- WIDENED by AI(W906-FW3-LotInfo-WA) 20260819 from
    // TfLotInfoEdit (TEdit) to vclcompat::TMemo (TCustomEdit still gives
    // ->Text, so the SCK_ART_Remainder ->Text-only consumer above is
    // unaffected): N23UseLotInfoFile (this wave, golden :7354-7371) needs
    // ->Lines->LoadFromFile()/->Lines->Count/->Lines->Strings[i], which TEdit
    // does not carry. This is exactly the "one-word edit... whenever their
    // form's own wave next touches them" repointing forms/FormWidgets.h's own
    // TfLotInfoEdit banner comment anticipated.
    TfLotInfoEdit *edtCusLotID;                    // [DATA] golden uLotInfo.h:623 (TEdit* customer lot-ID edit)
    TfLotInfoEdit *edtCusDevGrp;                   // [DATA] golden uLotInfo.h:624 (TEdit* customer device-group edit)
    TfLotInfoEdit *edtCusStep;                     // [DATA] golden uLotInfo.h:1038 (TEdit* customer step-no edit)
    TfLotInfoEdit *edtDevice;                      // [DATA] golden uLotInfo.h:625 (TEdit* device-name edit)
    TfLotInfoEdit *edtSysOperatorID;                // [DATA] golden uLotInfo.h:306 (TEdit* system operator-ID edit)
    vclcompat::TMemo *mmo2DLotInfo;                 // [DATA] golden uLotInfo.h:846 (TMemo*) -- ->Text (pre-existing) + ->Lines (this wave)
    // AI(W906-SaveTestSummaryTSV) 20260728: 4 new members Automation/SCK_ART_Remainder.cpp's
    // SckArtRem_SaveTestSummaryTSV derefs (golden SCK_ART.cpp:2884/3087, only ->Text read each --
    // reuse the existing TfLotInfoEdit shape, same idiom as edtCusLotID above).
    // golden edtASECL_LotID/edInsertion/edFlowID are TEdit* (uLotInfo.h:576/1044/1046); lbledtCustomer
    // is a TLabeledEdit* (uLotInfo.h:609, golden SckArtRem_SaveSummaryTrayFeed's own dependency) --
    // only ->Text is ever read from it here too, so the same minimal shape applies.
    TfLotInfoEdit *edtASECL_LotID;                  // [DATA] golden uLotInfo.h:576 (TEdit* ASE-CL lot-ID edit)
    TfLotInfoEdit *edInsertion;                     // [DATA] golden uLotInfo.h:1044 (TEdit* insertion-code edit)
    TfLotInfoEdit *edFlowID;                        // [DATA] golden uLotInfo.h:1046 (TEdit* flow-ID edit)
    // AI(W906-SaveSummaryTrayFeed) 20260728: golden SCK_ART.cpp:3157 (`fLotInfo->lbledtCustomer->Text`).
    TfLotInfoEdit *lbledtCustomer;                  // [DATA] golden uLotInfo.h:609 (TLabeledEdit* customer name edit)
    // AI(W906-W7-L1-Wave0) 20260801: W7-L1 Wave-0 ADD -- the 7 fLotInfo members
    // the asendic_* tray SM family dereferences, landed in one serialized pass so
    // the four parallel translation agents cannot collide on this header.  Every
    // golden line was re-read from the cp950-decoded golden uLotInfo.h this pass.
    // The first three are golden TLabel* whose Caption golden reads back through
    // atoi() (asendic_Loader.cpp:2069-2070), so they are ordinary Caption storage.
    //
    // OFFLINE-DEFAULT CONSEQUENCE, RECORDED BECAUSE IT IS NOT NEUTRAL: all three
    // Captions default to "" and atoi("")==0, which makes golden's KYEC-AMR
    // overflow guard at asendic_Loader.cpp:2068-2070
    //   (... atoi(LabDiffTrayCount->Caption)==0 &&
    //        (atoi(labLoaderTrayCount->Caption)-atoi(labNowTrayCount->Caption))==0)
    // evaluate TRUE whenever the surrounding TrayForm.bEnableAMR arm is entered.
    // A test that turns bEnableAMR on must therefore set all three captions
    // deliberately rather than relying on the constructed state.
    TfLotInfoLabel *LabDiffTrayCount;               // [DATA] golden uLotInfo.h:1086 (TLabel*) -- Caption read via atoi()
    TfLotInfoLabel *labLoaderTrayCount;             // [DATA] golden uLotInfo.h:1080 (TLabel*) -- Caption read via atoi()
    TfLotInfoLabel *labNowTrayCount;                // [DATA] golden uLotInfo.h:1084 (TLabel*) -- Caption read via atoi()
    TfLotInfoLabel *labNowAuto1TrayID;              // [DATA] golden uLotInfo.h:799 (TLabel*) -- Caption written (asendic_Auto.cpp:561)
    TfLotInfoLabel *labNowAuto2TrayID;              // [DATA] golden uLotInfo.h:802 (TLabel*) -- (asendic_Auto.cpp:562)
    TfLotInfoLabel *labNowAuto3TrayID;              // [DATA] golden uLotInfo.h:804 (TLabel*) -- (asendic_Auto.cpp:563)
    // Golden uLotInfo.h:1026 is a TCheckBox*, read-only in this family
    // (asendic_Auto.cpp:1994 `->Checked==true`).  Reuses the TfMainCheckBox alias
    // (forms/FormWidgets.h:114 -> vclcompat::TCheckBox, default Checked=false) --
    // the same alias fMain->chkE84IDTray uses, so no new widget stand-in is needed.
    TfMainCheckBox *cbFirstTrayCheckOnUnloader;     // [DATA] golden uLotInfo.h:1026 (TCheckBox*) -- offline Checked=false
    // AI(W906-PT-W3-integrate) 20260808: the 1 member the PT-W3 unit
    // Automation/uRENESAS_Server.cpp derefs (its "FACADE ADDITIONS NEEDED" banner,
    // uRENESAS_Server.cpp:105, is the measurement).  Only ->Click() is ever touched
    // (uRENESAS_Server.cpp:917/:1028, golden uLotInfo.cpp:1864 / :8231).
    // SHAPE: spelled `vclcompat::TButton*` rather than adding a TfLotInfoButton
    // alias, so forms/FormWidgets.h is NOT edited for a single member -- the same
    // decision forms/fShuttleMove.h:104 already took for its btRetry.  Confirmed a
    // real TButton in golden (uLotInfo.h:266 `TButton *btClearBarcodeList;`).
    // BEHAVIOUR: vclcompat::TControl::Click() (vclcompat/Controls.h:222) is an
    // offline no-op, so golden's OnClick handler -- btClearBarcodeListClick,
    // golden uLotInfo.cpp:10005-10014: zeroes iBarcodeReject, clears
    // fBarCode->map2DList / list2DByLot, re-sends the CCD "2DID by lot list"
    // command, RecordProcess, SaveToFile(asBarCodeLot), then chains
    // btClearBarcodeCount->Click() -- does NOT run.  That whole chain is
    // fBarCode-owned and unported, so a real body here would have nothing to call.
    // NOTE: csystem.cpp:2817 gates this identical golden call behind its TU-local
    // W7C2_FLOTINFO_CLEARBARCODE() macro (4 sites), whose stated premise ("member
    // absent") stops being true with this ADD.  Behaviour is unchanged either way
    // today (both are no-ops), but that macro is now retirable.
    vclcompat::TButton *btClearBarcodeList;         // [WIDGET] golden uLotInfo.h:266 (TButton*) -- ->Click() only

    // =======================================================================
    //  AI(W906-FW3-LotInfo-WA) 20260819: Wave A ADD -- see file banner above
    //  for WAVE SCOPE / GATE REGISTER / DEVIATION.  Grouped per Tier-1 method,
    //  each member cites the golden uLotInfo.h line it mirrors.
    // =======================================================================

    // -- RefreshYieldMonitor_SIGURD (golden :13327-13545) -- 37 widgets ------
    TGroupBox    *gbManualCheckList;             // golden uLotInfo.h:525
    TButton      *btnManualStandard;             // golden uLotInfo.h:527
    TCheckBox    *cbMonitor_FTPRMS;              // golden uLotInfo.h:481
    TComboBox    *cbContactMode;                 // golden uLotInfo.h:482 -- WA-1 gates the fContact-> RHS assigned into this
    TCheckBox    *cbSiteYieldCmp_FT;             // golden uLotInfo.h:486
    TCheckBox    *cbLowYieldByTotal_FT;          // golden uLotInfo.h:495
    TEdit        *edSiteYieldCmpOnOff_Cur;       // golden uLotInfo.h:491
    TEdit        *edLowYieldByTotalOnOff_Cur;    // golden uLotInfo.h:500
    TEdit        *edSiteYieldCmpIg_FT;           // golden uLotInfo.h:488
    TEdit        *edLowYieldByTotalIg_FT;        // golden uLotInfo.h:496
    TEdit        *edSiteYieldCmpIg_Cur;          // golden uLotInfo.h:489
    TEdit        *edLowYieldByTotalIg_Cur;       // golden uLotInfo.h:498
    TLabel       *lblSiteYieldCmpIg_Cur;         // golden uLotInfo.h:485
    TLabel       *lblLowYieldByTotalIg_Cur;      // golden uLotInfo.h:494
    TEdit        *edSiteYieldCmp_FT;             // golden uLotInfo.h:487
    TEdit        *edSiteYieldCmp_Cur;            // golden uLotInfo.h:490
    TLabel       *lblSiteYieldCmp_Cur;           // golden uLotInfo.h:484
    TEdit        *edLowYieldByTotal_FT;          // golden uLotInfo.h:497
    TEdit        *edLowYieldByTotal_Cur;         // golden uLotInfo.h:499
    TLabel       *lblLowYieldByTotal_Cur;        // golden uLotInfo.h:493
    TRadioButton *rbContsFailBySocket_FTOn;      // golden uLotInfo.h:504
    TRadioButton *rbContsFailBySocket_FTOff;     // golden uLotInfo.h:505
    TEdit        *edtContsFailBySocket_Cur;      // golden uLotInfo.h:508
    TEdit        *edContsFailSocketAlarmCT_FT;   // golden uLotInfo.h:506
    TEdit        *edContsFailSocketAlarmCT_Cur;  // golden uLotInfo.h:507
    TLabel       *lblContsFailSocketAlarmCT_Cur; // golden uLotInfo.h:503
    TRadioButton *rbContsFailByHead_FTOn;        // golden uLotInfo.h:512
    TRadioButton *rbContsFailByHead_FTOff;       // golden uLotInfo.h:513
    TEdit        *edtContsFailByHead_Cur;        // golden uLotInfo.h:516
    TEdit        *edContsFailHeadAlarmCT_FT;     // golden uLotInfo.h:514
    TEdit        *edContsFailHeadAlarmCT_Cur;    // golden uLotInfo.h:515
    TLabel       *lblContsFailHeadAlarmCT_Cur;   // golden uLotInfo.h:511
    TEdit        *edOSBin;                       // golden uLotInfo.h:520
    TEdit        *edOSBinCnt;                    // golden uLotInfo.h:522
    TEdit        *edOSBinCnt_Cur;                // golden uLotInfo.h:523
    TEdit        *edOSBinPreset;                 // golden uLotInfo.h:521
    TEdit        *edOSBinPreset_Cur;             // golden uLotInfo.h:524
    TLabel       *lblOSBin_Cur;                  // golden uLotInfo.h:519
    TGroupBox    *grpOSBin;                      // golden uLotInfo.h:517
    virtual void RefreshYieldMonitor_SIGURD();   // golden uLotInfo.cpp:13327-13545

    // -- TransformTemperature_AirStream (golden :15080-15193) -- zero new ---
    // widgets (fMain->edWorkTemperBase already existed before this wave).
    virtual double TransformTemperature_AirStream(double Offset, int iIndex);  // golden uLotInfo.cpp:15080-15193

    // -- AdjtsYieldMonitiorSize (golden :13636-13717) ------------------------
    TfLotInfoPageControl *pgLotinfo;              // golden uLotInfo.h:25 (TPageControl*)
    vclcompat::TPageControl *pgcLotInfo;           // golden uLotInfo.h:581 (TPageControl*) -- ->Visible only
    TTabSheet *ts_ATC6_1;                          // golden uLotInfo.h:637
    TTabSheet *tsATC;                              // golden uLotInfo.h:31  (shared w/ SetATCFormVisible)
    TTabSheet *tsASECLEventLog;                    // golden uLotInfo.h:237
    TTabSheet *tsYieldMonitior;                    // golden uLotInfo.h:462
    TTabSheet *tsFTP;                              // golden uLotInfo.h:28
    TTabSheet *tsLotID;                            // golden uLotInfo.h:27
    TTabSheet *tsMurata;                           // golden uLotInfo.h:582
    TTabSheet *tsSigurd_CX;                        // golden uLotInfo.h:583
    TTabSheet *tsSPIL_SZ;                          // golden uLotInfo.h:594
    TTabSheet *tsOEE;                              // golden uLotInfo.h:597
    TTabSheet *ts2DSort;                           // golden uLotInfo.h:604
    TTabSheet *tsChipAdv;                          // golden uLotInfo.h:605 (shared w/ SettsChipAdvVisible)
    TTabSheet *tsVTest;                            // golden uLotInfo.h:792
    TTabSheet *tsOCRBarCode;                       // golden uLotInfo.h:77
    TTabSheet *tsAMR;                              // golden uLotInfo.h:996
    int Height, Width, Top, Left;                  // golden TForm properties (shared w/ ShowInformation) -- NSDMI 0
    virtual void AdjtsYieldMonitiorSize();          // golden uLotInfo.cpp:13636-13717

    // -- SetSelectionVisible (golden :1262-1342) -----------------------------
    TGroupBox *groupbDownloadItem;                 // golden uLotInfo.h:247
    TGroupBox *grpMesCheck;                        // golden uLotInfo.h:788
    TTabSheet *tsSelection;                        // golden uLotInfo.h:40
    TfLotInfoLayoutPanel *Panel28;                  // golden uLotInfo.h:303 (TPanel*) -- ->Width only
    TComboBox *cbTestTimes;                        // golden uLotInfo.h:990
    TLabel    *lblTestTimes;                       // golden uLotInfo.h:991
    TLabel    *lblDownloadAccessWarning;           // golden uLotInfo.h:1120
    virtual void SetSelectionVisible();             // golden uLotInfo.cpp:1262-1342

    // -- ShowSocketID (golden :11443-11516) ----------------------------------
    TPanel *SocketSiteCH_Display[MAX_SOCKET_ROW][MAX_SOCKET_COL];  // golden uLotInfo.h:1327 (TPanel*[4][8])
    TEdit  *edSocket[MAX_SOCKET_ROW][MAX_SOCKET_COL];              // golden uLotInfo.h:1326 (TEdit*[4][8])
    // NOTE: golden's SocketLabRow_Display[MAX_SOCKET_ROW]/SocketLabCol_Display
    // [MAX_SOCKET_COL] are FILE-SCOPE globals (uLotInfo.cpp:89-90), not
    // TfLotInfo members -- declared/defined that way in forms/fLotInfo.cpp too
    // (same treatment as iLoaderTask[]/iloaderLevelTask[]/bLotFirstKeyIn/
    // bLotID_OK below).
    virtual void ShowSocketID();                    // golden uLotInfo.cpp:11443-11516

    // -- CheckAirMachineStatus (golden :14805-14868) -- WA-6: body gated, ---
    // no new widgets declared (see GATE REGISTER; adding Caption-only widgets
    // nothing writes into this wave would be unused surface).
    virtual void CheckAirMachineStatus();

    // -- InitialRefrigerantSystem (golden :14877-14939) -- 72 named panels --
    bool bInitFormcomponent;                       // golden uLotInfo.h:1396
    bool OldRefrigerantCommand;                    // golden uLotInfo.h:1397
    TTabSheet *ts_RefrigerantStatus_Page_2;        // golden uLotInfo.h:738
    // group 1..8 (golden uLotInfo.h, individually __published, see :690-786):
    TPanel *pnlRefrigerantMachine1, *pnlRefrigerantMachine2, *pnlRefrigerantMachine3, *pnlRefrigerantMachine4;
    TPanel *pnlRefrigerantMachine5, *pnlRefrigerantMachine6, *pnlRefrigerantMachine7, *pnlRefrigerantMachine8;
    TLabel *LabRefrigerantValue1, *LabRefrigerantValue2, *LabRefrigerantValue3, *LabRefrigerantValue4;
    TLabel *LabRefrigerantValue5, *LabRefrigerantValue6, *LabRefrigerantValue7, *LabRefrigerantValue8;
    TPanel *pnlRefCopm1Status_1, *pnlRefCopm1Status_2, *pnlRefCopm1Status_3, *pnlRefCopm1Status_4;
    TPanel *pnlRefCopm1Status_5, *pnlRefCopm1Status_6, *pnlRefCopm1Status_7, *pnlRefCopm1Status_8;
    TPanel *pnlRefCopm2Status_1, *pnlRefCopm2Status_2, *pnlRefCopm2Status_3, *pnlRefCopm2Status_4;
    TPanel *pnlRefCopm2Status_5, *pnlRefCopm2Status_6, *pnlRefCopm2Status_7, *pnlRefCopm2Status_8;
    TLabel *labRefCopm1HpValue_1, *labRefCopm1HpValue_2, *labRefCopm1HpValue_3, *labRefCopm1HpValue_4;
    TLabel *labRefCopm1HpValue_5, *labRefCopm1HpValue_6, *labRefCopm1HpValue_7, *labRefCopm1HpValue_8;
    TLabel *labRefCopm2HpValue_1, *labRefCopm2HpValue_2, *labRefCopm2HpValue_3, *labRefCopm2HpValue_4;
    TLabel *labRefCopm2HpValue_5, *labRefCopm2HpValue_6, *labRefCopm2HpValue_7, *labRefCopm2HpValue_8;
    TLabel *labRefCopm1LpValue_1, *labRefCopm1LpValue_2, *labRefCopm1LpValue_3, *labRefCopm1LpValue_4;
    TLabel *labRefCopm1LpValue_5, *labRefCopm1LpValue_6, *labRefCopm1LpValue_7, *labRefCopm1LpValue_8;
    TLabel *labRefCopm2LpValue_1, *labRefCopm2LpValue_2, *labRefCopm2LpValue_3, *labRefCopm2LpValue_4;
    TLabel *labRefCopm2LpValue_5, *labRefCopm2LpValue_6, *labRefCopm2LpValue_7, *labRefCopm2LpValue_8;
    TLabel *LabRefrigerantAdjustValue1, *LabRefrigerantAdjustValue2, *LabRefrigerantAdjustValue3, *LabRefrigerantAdjustValue4;
    TLabel *LabRefrigerantAdjustValue5, *LabRefrigerantAdjustValue6, *LabRefrigerantAdjustValue7, *LabRefrigerantAdjustValue8;
    // destination arrays golden .h:1387-1395 (already __published as arrays there)
    TPanel *TripnlRefrigerantMachine[8];
    TLabel *TriLabRefrigerantValue[8];
    TPanel *TripnlRefCopm1Status[8];
    TPanel *TripnlRefCopm2Status[8];
    TLabel *TriLabRefCopm1HpValue[8];
    TLabel *TriLabRefCopm2HpValue[8];
    TLabel *TriLabRefCopm1LpValue[8];
    TLabel *TriLabRefCopm2LpValue[8];
    TLabel *TriLabRefrigerantAdjustValue[8];
    virtual void InitialRefrigerantSystem();        // golden uLotInfo.cpp:14877-14939 -- WA-5 gates the ATC_OFFLINE_FormComInit() call

    // -- SetATCFormVisible (golden :9938-9985) -------------------------------
    TPanel *palATC;                                 // golden uLotInfo.h:33
    TPanel *aldATC7Status;                          // golden uLotInfo.h:958 (TALed* -- conflated to TPanel, ->Visible only)
    TLabel *lblATC70;                               // golden uLotInfo.h:959
    TPanel *aldATCChillerStatus;                    // golden uLotInfo.h:849 (TALed* -- conflated to TPanel, ->Visible only)
    TLabel *lblChiller;                             // golden uLotInfo.h:851
    TLabel *lblATC_Now_RecipeFile;                  // golden uLotInfo.h:854
    TPanel *pan_ATCChillerSV;                       // golden uLotInfo.h:855
    TPanel *pl_ATCChillerSV;                        // golden uLotInfo.h:856
    struct TfLotInfoTimer { bool Enabled; TfLotInfoTimer():Enabled(false){} } *NetATCTime;  // golden uLotInfo.h:83 (TTimer*) -- ->Enabled only
    virtual void SetATCFormVisible();                // golden uLotInfo.cpp:9938-9985

    // -- CheckEventLogParameter (golden :10673-10720) ------------------------
    TEdit *edCustomerDevice;                        // golden uLotInfo.h:1045
    TSpeedButton *btnASECL_LotStart;                // golden uLotInfo.h:241 -- ->Down only
    virtual bool CheckEventLogParameter();           // golden uLotInfo.cpp:10673-10720

    // -- ShowATCTempPanel (golden :14561-14606) ------------------------------
    TfLotInfoLayoutPanel *Pan_ATC_Use_4Head;        // golden uLotInfo.h:872 (TPanel*) -- ->Left/->Top
    TfLotInfoLayoutPanel *Pan_ATC_Use_8Head;        // golden uLotInfo.h:859 (TPanel*) -- ->Left/->Top
    TfLotInfoLayoutPanel *Pan_ATC_Use_32Head;       // golden uLotInfo.h:885 (TPanel*) -- ->Visible only, layout type reused for symmetry
    TPanel *ATCChPal[ATC_HEAD_COUNT];               // golden uLotInfo.h:1328
    TPanel *ATCPtr[ATC_HEAD_COUNT];                 // golden uLotInfo.h:1330
    TPanel *ATCReferPtr[ATC_HEAD_COUNT];            // golden uLotInfo.h:1329
    virtual void ShowATCTempPanel();                 // golden uLotInfo.cpp:14561-14606

    // -- JCETWhite2DIDShow (golden :15953-15992) -----------------------------
    TLabel *labCusLotID;                            // golden uLotInfo.h:620
    virtual void JCETWhite2DIDShow(bool bUse);       // golden uLotInfo.cpp:15953-15992

    // -- ShowInformation (golden :12326-12362) -------------------------------
    TGroupBox *gbFTPAutomation_Download;            // golden uLotInfo.h:438
    TGroupBox *gbFTPAutomation_Upload;              // golden uLotInfo.h:447
    TSpeedButton *sbRecipeUpload;                   // golden uLotInfo.h:459
    TSpeedButton *sbRecipeDownload;                 // golden uLotInfo.h:460
    TSpeedButton *sbFTPAutomationSave;               // golden uLotInfo.h:461
    TfLotInfoLayoutButton *sbTest;                   // golden uLotInfo.h:458 (TSpeedButton*) -- ->Top only
    virtual void ShowInformation(bool bShow);        // golden uLotInfo.cpp:12326-12362

    // -- N23UseLotInfoFile (golden :7346-7380) -- zero new widgets (all 4 -----
    // dependencies -- mmo2DLotInfo/edtCusLotID/edtCusDevGrp/edtDevice -- were
    // already in this facade before this wave).
    virtual bool N23UseLotInfoFile();                // golden uLotInfo.cpp:7346-7380

    // -- edTempKeyUp (golden :4966-4999) --------------------------------------
    TfLotInfoTextEdit *edTemp;                       // golden uLotInfo.h:316 (TEdit*) -- ->SelStart needs the composed type above
    virtual void edTempKeyUp();                       // golden uLotInfo.cpp:4966-4999 (Sender/Key/Shift dropped, unused)

    // -- edtSysLotIDKeyPress / edPageKeyPress (golden :11892-11917 / :11919-11933) --
    // DEVIATION: both golden bodies call ->SetFocus() on the cross-field target
    // (edPage / edtSysOperatorID) -- a pure UI focus-transfer with no
    // observable state anywhere in this facade. vclcompat::TControl carries no
    // SetFocus() (a rendering concern, same scope call as ->Refresh() being a
    // no-op elsewhere in this tree e.g. forms/fTemperFrom.h's
    // TfTemperFromPanel). Dropped at both call sites below rather than
    // widening `edtSysOperatorID`'s pre-existing shared type for one method.
    TEdit *edPage;                                   // golden uLotInfo.h:420 (shared by both)
    virtual void edtSysLotIDKeyPress(char Key);       // golden uLotInfo.cpp:11892-11917 (Sender/Shift dropped, unused; ->SetFocus() dropped, see DEVIATION above)
    virtual void edPageKeyPress(char Key);            // golden uLotInfo.cpp:11919-11933 (Sender/Shift dropped, unused; ->SetFocus() dropped, see DEVIATION above)

    // -- SettsChipAdvVisible (golden :1240-1260) -- tsChipAdv declared above --
    virtual void SettsChipAdvVisible();               // golden uLotInfo.cpp:1240-1260

    // -- bCheckOnlyOneFile (golden :14193-14213) -- zero new members ---------
    virtual bool bCheckOnlyOneFile(AnsiString asPath, AnsiString &asFileName);  // golden uLotInfo.cpp:14193-14213

    // -- CheckActionFlag (golden :16234-16246) -------------------------------
    TfLedValue *ledLoader, *ledEmpty, *ledColor;                    // golden uLotInfo.h (TALed*)
    TfLedValue *ledAuto1, *ledAuto2, *ledAuto3;                     // golden uLotInfo.h (TALed*)
    TfLedValue *ledStartAGV, *ledSTART, *ledLoaderTotalTray, *ledLOT_START;  // golden uLotInfo.h (TALed*)
    virtual void CheckActionFlag();                   // golden uLotInfo.cpp:16234-16246

    // -- SetTesterStartTimeByB03 (golden :14298-14309) -- zero new members ---
    virtual void SetTesterStartTimeByB03();           // golden uLotInfo.cpp:14298-14309

    // -- btnLoadFileClick (golden :14175-14184) ------------------------------
    TfLotInfoOpenDialog *OpenDialog1;                // golden uLotInfo.h:568 (TOpenDialog*)
    TEdit *edSort2DIDBinFile;                        // golden uLotInfo.h:843
    virtual void btnLoadFileClick();                  // golden uLotInfo.cpp:14175-14184 (Sender dropped, unused)

    // -- cbRunModeKeyDown / cbRunModeKeyUp (golden :11990-11997 / :11999-12006) -- WA-7
    virtual void cbRunModeKeyDown();                  // golden uLotInfo.cpp:11990-11997
    virtual void cbRunModeKeyUp();                    // golden uLotInfo.cpp:11999-12006

    // -- edDeviceNameKeyUp (golden :12044-12051) -- zero new members --------
    virtual void edDeviceNameKeyUp();                 // golden uLotInfo.cpp:12044-12051 (Sender/Key/Shift dropped, unused)

    // -- ReflashInfo (golden :16479-16485) -----------------------------------
    TLabel *labAuto1TrayCount_KYEC, *labAuto2TrayCount_KYEC, *labAuto3TrayCount_KYEC;  // golden uLotInfo.h:1072/1074/1076
    virtual void ReflashInfo();                       // golden uLotInfo.cpp:16479-16485

    // -- labLotIDMouseDown (golden :8423-8428) -- WA-8, edtSysLotID exists ---
    virtual void labLotIDMouseDown();                 // golden uLotInfo.cpp:8423-8428 (params dropped, unused)

    // -- edtLotVerifyMouseDown / edtLotVerifyKeyPress (golden :14515-14520 / :14522-14526) --
    TLabeledEdit *edtLotVerify;                      // golden uLotInfo.h:836
    virtual void edtLotVerifyMouseDown();              // golden uLotInfo.cpp:14515-14520 (params dropped, unused)
    virtual void edtLotVerifyKeyPress();               // golden uLotInfo.cpp:14522-14526 (params dropped, unused)

    // -- btnCancelTestPauseClick (golden :12297-12301) -- zero new members --
    virtual void btnCancelTestPauseClick();           // golden uLotInfo.cpp:12297-12301 (Sender dropped, unused)

    // -- BtnPauseMouseDown / BtnPauseMouseUp (golden :14533-14537 / :14539-14543) --
    TfMainSpeedButton *BtnPause;                     // golden uLotInfo.h:837 (TBtnPanel*) -- reuses the existing TBtnPanel stand-in (FormWidgets.h)
    virtual void BtnPauseMouseDown();                  // golden uLotInfo.cpp:14533-14537 (params dropped, unused)
    virtual void BtnPauseMouseUp();                    // golden uLotInfo.cpp:14539-14543 (params dropped, unused)

    // -- VisibleUploadBtnPAT (golden :15710-15714) ---------------------------
    TSpeedButton *sbUploadPAT;                       // golden uLotInfo.h:985
    virtual void VisibleUploadBtnPAT(bool bVisible);  // golden uLotInfo.cpp:15710-15714

    // -- cbbDeviceNameChange (golden :7216-7219) -----------------------------
    TEdit *edDeviceName;                             // golden uLotInfo.h:313
    TComboBox *cbbDeviceName;                        // golden uLotInfo.h:317
    virtual void cbbDeviceNameChange();                // golden uLotInfo.cpp:7216-7219 (Sender dropped, unused)

    // -- btStartCountClick (golden :8418-8421) -------------------------------
    bool bStartCount_SCK;                            // golden uLotInfo.h:1294 (real TfLotInfo member, not a bare global)
    virtual void btStartCountClick();                  // golden uLotInfo.cpp:8418-8421 (Sender dropped, unused)

    // -- edtASECL_LotIDClick (golden :10491-10494) -- WA-8, edtASECL_LotID exists --
    virtual void edtASECL_LotIDClick();               // golden uLotInfo.cpp:10491-10494 (Sender dropped -- moot once gated)

    // -- btTesterTCPShowClick (golden :13841-13844) -- WA-9, whole body gated --
    virtual void btTesterTCPShowClick();               // golden uLotInfo.cpp:13841-13844 (Sender dropped, unused)

    // -- InitialLoaderTask / InitialLDLevelTask (golden :16259-16262 / :16264-16267) --
    // NOTE: golden's iLoaderTask[3]/iloaderLevelTask[3] are FILE-SCOPE globals
    // (uLotInfo.cpp:16256-16257, `={1,1,1}`), NOT TfLotInfo members -- declared
    // as such in forms/fLotInfo.cpp, mirroring InitialUnLoaderTask's sibling
    // iUnloaderTask[3] being a REAL member only because golden itself declares
    // it in uLotInfo.h:1264 (different from these two).
    virtual void InitialLoaderTask(int iPos);          // golden uLotInfo.cpp:16259-16262
    virtual void InitialLDLevelTask(int iPos);         // golden uLotInfo.cpp:16264-16267

    TfLotInfo();
    virtual ~TfLotInfo() {}
};
extern TfLotInfo *fLotInfo;

#endif // FORMS_FLOTINFO_H
