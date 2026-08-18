// =============================================================================
//  forms/fShowBinSelect.h  --  non-VCL facade for golden's TfShowBinSelect
//                               (cShowBinSelect.h)
//
//  AI(W906-FW3-ShowBinSelect-WA) 20260818: new file, FW-3 queue item 2.
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/cShowBinSelect.h (524
//  lines) + cShowBinSelect.cpp (3,016 lines, cp950/Big5 -- decoded with
//  `python3 -c "open(path,'rb').read().decode('cp950')"`, 0 U+FFFD, this
//  wave).
//
//  ROLE
//  ----
//  TfShowBinSelect is golden's Unloader Bin-Select / Category-Info dialog:
//  per-Auto/Fix/Magazine bin assignment display, the ART Category grids, the
//  UPH history grid, and the physical bin-display-hardware refresh loop.
//  Its `ShowCategoryBin()`/`UPH_StringGrid`/`labArmDiff`/`labSiteDiff`/
//  `labLowYield`/`labTotalYield`/`labTotalYieldTotal`/`lblSpeciallYield`/
//  `lblSpeciallYieldTotal`/`IntervalByTotal`/`iLowYieldBinSelectContactCount`
//  are 14 of fYieldMonitoring's 39 GATE touches (forms/fYieldMonitoring.h
//  GATE REGISTER (Y2)) -- unblocking THOSE is this wave's primary purpose.
//
//  WAVE A / WAVE B SPLIT -- WHY THIS FILE DOES NOT COVER THE WHOLE GOLDEN CLASS
//  --------------------------------------------------------------------------
//  Golden's class carries ~300 individually-named widget members (7 parallel
//  32-slot label arrays for Auto/Fix/Magazine bin display alone, plus every
//  ART-FT/ART-RT/AI-CCD variant) and several methods are irreducibly blocked
//  by an OPAQUE hardware pointer (`database.h:300 TMyBinDispCtrl *BinDisCtrl;
//  // opaque; NULL until UI wave wires InstallColorBinDisplay` -- confirmed
//  this wave, `grep -n BinDisCtrl database.h`). Per the FW campaign's own
//  "大表單切塊" (slice big forms) policy (fw-wave-loop skill S6), this wave
//  (WAVE A) translates the subset that (a) satisfies the fYieldMonitoring
//  GATE REGISTER and (b) is reachable without the opaque hardware pointer or
//  the ~250-widget ctor-population surface; the remainder is WAVE B,
//  explicitly queued below, NOT silently dropped.
//
//  WAVE A SCOPE (golden line spans, this file's methods)
//  --------------------------------------------------------------------------
//    TfShowBinSelect()      golden :44-161   DEVIATION -- bootstrap ctor,
//                            see CTOR NOTE below (NOT a verbatim translation
//                            of golden's ctor body)
//    FormDestroy            golden :163-176  ACTIVE
//    FormClose              golden :868-873  ACTIVE
//    InitShowBinDigital     golden :874-878  ACTIVE
//    SetLabelVisible        golden :1425-1434 ACTIVE
//    SetAutoVisible         golden :1436-1477 ACTIVE
//    ShowInitialString      golden :1634-1700 ACTIVE
//    ShowCategoryBin        golden :1701-2052 ACTIVE, 3 GATEs (fCounterClear)
//    UPH_StringGridDblClick golden :2054-2099 ACTIVE, 1 GATE (Application->MessageBoxA)
//    CaculateUPH            golden :2279-2333 ACTIVE, 1 GATE (fMain->StatusBar1)
//    edSLT01Change          golden :2334-2337 ACTIVE, 1 GATE (fMain->StatusBar1)
//    btnSetInpputCntClick   golden :2339-2352 ACTIVE
//    RefreshAiCnt           golden :2354-2364 ACTIVE, 1 GATE (fFixAICCD->UnloadAICntNG[])
//    ed_AutoCleanCountClick golden :2216-2223 ACTIVE, 2 GATEs (fSecurity, fCleaning)
//    labAuto1Click          golden :2225-2242 ACTIVE, 1 GATE (fBinSel->chkShow0Xbin)
//    btReturnClick          golden :2244-2251 ACTIVE, 1 GATE (PageControl1Change -- Wave B)
//    btnCleanResetClick     golden :2253-2260 ACTIVE, 2 GATEs (fSecurity, fCleaning)
//    btnClearCountClick     golden :2262-2277 ACTIVE, 2 GATEs (fSecurity, fCounterClear)
//    DelDot (free function) golden :178-205  ACTIVE (pure)
//
//  WAVE B QUEUE (explicit, NOT translated this wave -- golden line spans)
//  --------------------------------------------------------------------------
//    ShowBinSel              golden :388-757   (widget population, ~250-widget
//                              surface; fAGV/SECS EventReport touches)
//    ShowBinSel_ARTNor        golden :2365-2580 (ART bin display, same shape)
//    ShowBinSel_ARTRT         golden :2581-2799 (ART bin display, same shape)
//    FormShow                 golden :758-866   (~15 more widgets, many
//                              customer-code branches)
//    PageControl1Change       golden :1479-1633 (page-layout geometry, many
//                              widgets) -- DECLARED here as a documented
//                              no-op GAP so btReturnClick's call site has
//                              something to bind to (see GATE REGISTER below)
//    ChangeBinDispStatus      golden :208-386   BLOCKED: >90% of its body is
//                              `HSys.BinDisCtrl->...` (opaque, always NULL)
//    ShowBinDigital           golden :880-996   physical 7-segment display
//                              driver (SW[] switch coupling + fiosetview)
//    DoShowBinDigital         golden :998-1423  BLOCKED: dominant body is
//                              `HSys.BinDisCtrl->...`
//    TimerAutoCleanCountTimer golden :2168-2214 (~6 more widgets: rg_FixBinBox,
//                              ed_FixBinBoxAlarmCount, LabErrorBinNowCount,
//                              PageControl1_ART, Tab_ARTSkipICCount,
//                              AutocleanlifeTime)
//  SAFETY-QUEUED (per project policy -- machine-mode-switching actions,
//  queued for user sign-off, NOT translated as a "widget surface" deferral):
//    btnAutoCleanClick             golden :2101-2166 (starts a REAL Auto
//                                   Clean sequence -- bRunAutoClean=true,
//                                   motor-position preconditions)
//    btnAutoDeviceEjectionRemoveClick golden :2852-2893 (physical tray
//                                   ejection: InitNewTray/bEject/bNeedEject)
//    btnAutoDeviceEjectionClick    golden :2896-3016 (switches LastSet.iTester
//                                   to OFF_LINE, rewrites Bin-config files,
//                                   purges devices to AUTO1)
//    EdLoadCountClick              golden :2800-2803 (LOW risk on its own --
//                                   pops a virtual keyboard -- but queued
//                                   alongside its sibling clicks rather than
//                                   cherry-picked, per this wave's brief
//                                   scope)
//    btnICMisPlacementClick        golden :2805-2843 (manually injects a real
//                                   JAM alarm -- ShowErrorMessage("JAM11/12/
//                                   1310", ...))
//    sbCopyRecipeClick             golden :2845-2850 (fMain->RunBatchCopyRecipe,
//                                   a setup-file mutation entry point)
//
//  CTOR NOTE -- DEVIATION: bootstrap ctor, not a verbatim golden translation
//  --------------------------------------------------------------------------
//  Golden's ctor's ENTIRE job (cShowBinSelect.cpp :47-133) is copying ~190
//  individually-named `.dfm` widget pointers (laAuto1, laFix1, ... 33 of
//  each) into 7 parallel arrays (MyBinSel/MyBinSelLab/MyBinSelARTFT/
//  MyBinSelARTFTLab/MyBinSelARTRT/MyBinSelARTRTLab/grpBinDisp/EditAi) plus a
//  handful of scalar inits (ColorRed/Green/Orange, tsUPH, sBinCode_ATK[],
//  ShowInitialString(), an SPIL_FOR_QLE branch, btReturn->Align).  VERIFIED
//  this wave (`grep -n "<name>" <utf8-decoded-cpp>` for a sample of the ~190
//  names): apart from `labAuto1/2/3`/`labFix1/2/3` (read again in the
//  DEFERRED FormShow, golden :779-784, Greatek customer code only) and each
//  name's own `<name>Click` event-handler declaration, NONE of the ~190
//  individually-named widgets is ever read anywhere outside the ctor -- only
//  the 7 ARRAYS are. This facade therefore constructs the 7 arrays directly
//  (one heap object per slot, in a loop) rather than declaring ~190 facade
//  members whose only reader would be this same ctor. This is a BEHAVIOUR-
//  PRESERVING end-state (each `MyBinSel[i]` etc. IS a live, independently
//  mutable object, exactly as golden's ctor leaves it), not a scope
//  reduction -- see forms/fObserver.h/forms/fYieldMonitoring.h's own "NO
//  CONSTRUCTOR THIS WAVE" precedent for the general shape of this class of
//  decision; here a (non-verbatim) constructor is written specifically
//  because SetLabelVisible/SetAutoVisible/ShowCategoryBin (all THIS wave's
//  scope) actively read those arrays, so NSDMI defaults alone (arrays of
//  null pointers) would crash the first `MyBinSel[i]->Caption=...`.
//
//  Two genuinely PORTED ctor statements: ShowInitialString() is called
//  (translated this wave, see below) and tsUPH is populated with 20 blank
//  entries (verbatim). The SPIL_FOR_QLE `palAutoDeviceEjection` branch and
//  `btReturn->Align=alBottom` are cosmetic-only (see GATE REGISTER) and are
//  each represented as a documented no-op rather than silently dropped.
//
//  STATIC-INIT SAFETY (for the eventual main-loop homecoming, NOT decided
//  this wave -- global NOT defined here, see banner tail)
//  --------------------------------------------------------------------------
//  This ctor calls ShowInitialString(), which calls ShowCategoryBin(), which
//  reads Prod/LastSet/CosFunction/IniConfig/ArmData[]/TestSocket (all
//  plain-old extern globals with static storage duration and no dynamic
//  initializer of their own) and fContact->fShow (atester_shims.h's
//  TfContactShim -- also a plain global, no config/file I/O in ITS ctor
//  either). Unlike cObserver's ctor (which reads INI files directly), this
//  ctor touches NO file/registry I/O and NO other class's runtime state --
//  so, UNLIKE cObserver, an `INIFileGeneral!=0`-style guard is likely NOT
//  needed. This is flagged for the main loop to VERIFY (not asserted as
//  proven-safe by this wave) before defining a live global from this class,
//  per this wave's task brief.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (B1) ShowCategoryBin's three `fCounterClear->ClearCount(ctBinCount)`
//       calls (golden :1984/:2000/:2041) -- fCounterClear has NO facade
//       anywhere in the tree (`grep -rn "fCounterClear" --include=*.h .` --
//       only ONE hit, Automation/SCK_ART_Remainder.h:71, itself a comment
//       recording the same gap: "fCounterClear->ClearCount(), the last
//       needing an untranslated cCounterClear.cpp form"). Every OTHER
//       statement in each of the three surrounding `if` blocks (the Low-
//       Yield-alarm threshold checks, DoLowYieldAlarm/InitialAutoCleanAllTask
//       calls, fYieldMonitoring->ClearYieldCount()) is REAL and ACTIVE --
//       only the counter-clear call itself is gated, same "gate the missing
//       call, not the surrounding logic" idiom as forms/fContactCT.h's
//       GATE (C1)/fObserver.h's GATE (2).
//  (B2) UPH_StringGridDblClick's `Application->MessageBoxA(...)==IDOK`
//       guard (golden :2063) -- same absent-TApplication gap as forms/
//       fContactCT.h GATE (C2)/database.cpp's GA1-B6 precedent. Fail-closed:
//       the destructive row-delete loop does not run without a real confirm
//       dialog.
//  (B3) CaculateUPH's / edSLT01Change's `fMain->StatusBar1->Panels->
//       Items[N]->Text=...` (golden :2315/:2319/:2336) -- forms/fMain.h has
//       NO `StatusBar1` member (`grep -n "StatusBar1" forms/fMain.h` -- 0
//       hits, 20260818). The UPH NUMBER ITSELF (iNetUPH/iGrossUPH/
//       iRecordEventLogUPH -- all real, already-ported globals) is computed
//       and stored regardless; only the on-screen status-bar text write is
//       gated. edSLT01Change's `strcpy(LastSet.szJamClearData[0], ...)`
//       READS the same missing StatusBar1 as its RHS, so that one statement
//       is gated as a whole (no independently computable value, same
//       posture as forms/fYieldMonitoring.h GATE (Y3)).
//  (B4) RefreshAiCnt's `fFixAICCD->UnloadAICntNG[i]` (golden :2359) --
//       forms/fFixAICCD.h exists but carries no `UnloadAICntNG` member
//       (`grep -n "UnloadAICntNG" forms/fFixAICCD.h` -- 0 hits, 20260818).
//       Safe default: blank text (same as the existing `tNotUse` else-arm),
//       i.e. "no AI-CCD count available" rather than fabricating a number.
//  (B5) ed_AutoCleanCountClick / btnCleanResetClick's `fSecurity->
//       Insufficient(43)` / `Insufficient(97)` (golden :2218/:2255) --
//       same fSecurity absence as forms/fContactCT.h GATE (C3); same
//       fail-closed substitute (forced `false`, i.e. permission NOT
//       verified) -- both methods' bodies become unreachable until a real
//       fSecurity facade lands. `fCleaning->btnResetCleanCountClick(Owner)`
//       (golden :2220/:2257) is ALSO gated (forms/fCleaning.h has no such
//       member, `grep -n "btnResetCleanCountClick" forms/fCleaning.h` -- 0
//       hits) -- moot while the Insufficient() gate is fail-closed, recorded
//       so it is not missed when (B5)'s outer gate is lifted.
//  (B6) btnClearCountClick's `fSecurity->Insufficient(108)==false` (golden
//       :2266) -- same gap/substitute as (B5); `ShowMyMessageBox_YES_NO`
//       (golden :2270) is a SEPARATE gap (Automation/AGV_E84.h:58's own
//       comment already records it: "ShowMyMessageBox_YES_NO (mymessbox.h,
//       W7-UI modal dialog, untranslated)") -- fail-closed substitute: `ret`
//       forced to 2 ("No"/cancel), so `fCounterClear->ClearCount(
//       ctIndexCount)` (golden :2276, ALSO independently gated -- see (B1))
//       never runs either way.
//  (B7) labAuto1Click's `fBinSel->chkShow0Xbin->Checked` (golden :2237) --
//       fBinSel has NO facade anywhere (`grep -rn "fBinSel" --include=*.h .`
//       -- only comment-only hits in forms/fMain.h, 20260818). Safe default:
//       `false` (an unchecked checkbox is Delphi's own real-VCL default, and
//       is also this tree's established "default false" convention for
//       boolean widget state -- see forms/FormWidgets.h's own DEFAULT-VALUE
//       RULE banner), so the `else` arm (`Width=331`) is taken.
//  (B8) btReturnClick's `PageControl1Change(this)` call (golden :2250) --
//       PageControl1Change itself is WAVE B (golden :1479-1633, ~154 lines,
//       many more widgets). Declared here with a documented no-op body
//       (cShowBinSelect.cpp) so the call site compiles and links; every
//       OTHER statement in btReturnClick (the two Left/Top assignments) is
//       real and ACTIVE.
//
//  DESIGN NOTE -- facade-only widget wrapper shapes
//  --------------------------------------------------------------------------
//  TfShowBinSelectGrid : public vclcompat::TStringGrid
//    Adds a `Selection` sub-object with one field, `.Top` (int), for
//    UPH_StringGrid->Selection.Top (UPH_StringGridDblClick) -- vclcompat::
//    TStringGrid deliberately carries no selection-state surface (its own
//    SCOPE note). Offline default 0 (no cell selected), same "cosmetic/
//    interaction-state, offline inert" posture as every other facade-only
//    extension in this tree.
//  TfShowBinSelectPanel : public vclcompat::TPanel
//    Adds `Top` (int) for palAutoDeviceEjection->Top (ctor's SPIL_FOR_QLE
//    branch) -- vclcompat::TPanel carries no geometry.
//  TfShowBinSelectTimer
//    golden `TTimer *TimerAutoCleanCount;` -- ONLY `->Enabled` is ever
//    written (FormDestroy, golden :167) among this wave's scope; no
//    vclcompat TTimer stand-in exists anywhere in the tree (`grep -rn
//    "class TTimer" vclcompat/` -- 0 hits, 20260818), so a minimal
//    facade-only stand-in is declared here (same "PART 2 -- DEFERRED
//    STAND-INS" posture as forms/FormWidgets.h's own bespoke types for
//    golden classes vclcompat/Controls.h does not carry).
//
//  HYDRATION (dfm IR: tools/dfm2rc/ir_out/cShowBinSelect.dfm.ir.json, this
//  wave)
//  --------------------------------------------------------------------------
//    StrGrdCategory/StrGrdCategoryART/StrGrdCategoryContCT/StrARTSkipICCount:
//      ColCount=3, RowCount=16 (dfm design-time default -- ShowInitialString/
//      ShowCategoryBin overwrite both at runtime, exactly as golden does).
//    UPH_StringGrid: ColCount=4, RowCount=14 (dfm design-time default;
//      FormShow -- Wave B -- overwrites ColCount to 7 when
//      IniConfig.bVTESTFunction; not reachable from Wave A's own methods).
//    palAutoDeviceEjection: Top=473 (dfm) -- overwritten to 230 by the ctor's
//      SPIL_FOR_QLE branch when that flag is set, exactly as golden does.
//    grpBinDisp[]/MyBinSel[]/... arrays: no per-slot dfm geometry hydrated
//      (golden's OWN ctor never reads geometry off these either -- only
//      ->Caption/->Visible/->Font->Color, all zero/false/black-default
//      correct via each element's own default constructor).
// =============================================================================
#ifndef FORMS_FSHOWBINSELECT_H
#define FORMS_FSHOWBINSELECT_H

#include "vclcompat/vcl_compat.h"    // AnsiString, TObject, TStringList
#include "vclcompat/Controls.h"      // TLabel, TGroupBox, TPanel, TButton, TEdit, TLabeledEdit, TPageControl
#include "vclcompat/StringGrid.h"    // TStringGrid
#include "MachineType.h"             // e3TrayCount, eTrayCount

using vclcompat::TObject;
using vclcompat::TStringGrid;
using vclcompat::TLabel;
using vclcompat::TGroupBox;
using vclcompat::TPanel;
using vclcompat::TButton;
using vclcompat::TEdit;
using vclcompat::TLabeledEdit;
using vclcompat::TPageControl;
using vclcompat::TStringList;

// -- facade-only widget extensions (see DESIGN NOTE above) -------------------
class TfShowBinSelectGrid : public vclcompat::TStringGrid
{
public:
    struct { int Top; } Selection;

    explicit TfShowBinSelectGrid(int initialColCount = 4, int initialRowCount = 14)
        : vclcompat::TStringGrid(initialColCount, initialRowCount)
    {
        Selection.Top = 0;
    }
};

class TfShowBinSelectPanel : public vclcompat::TPanel
{
public:
    int Top;
    TfShowBinSelectPanel() : Top(0) {}
};

// golden TTimer -- see DESIGN NOTE above.
class TfShowBinSelectTimer
{
public:
    bool Enabled;
    TfShowBinSelectTimer() : Enabled(false) {}
};

// =============================================================================
//  TfShowBinSelect -- non-VCL facade (golden cShowBinSelect.h), WAVE A subset
// =============================================================================
class TfShowBinSelect
{
public:
    // -- Category / UPH grids (golden __published) ---------------------------
    TStringGrid *StrGrdCategory       = new TStringGrid(3, 16);
    TStringGrid *StrGrdCategoryART    = new TStringGrid(3, 16);
    TStringGrid *StrGrdCategoryContCT = new TStringGrid(3, 16);
    TStringGrid *StrARTSkipICCount    = new TStringGrid(3, 16);
    TfShowBinSelectGrid *UPH_StringGrid = new TfShowBinSelectGrid(4, 14);

    // -- unloader bin-select label arrays (golden ctor, see CTOR NOTE) -------
    // Sized e3TrayCount (33): 6 Auto + 12 Fix + 1 BulkBox + 14 Magazine.
    TLabel    *MyBinSel[e3TrayCount];
    TLabel    *MyBinSelLab[e3TrayCount];
    TLabel    *MyBinSelARTFT[e3TrayCount];
    TLabel    *MyBinSelARTFTLab[e3TrayCount];
    TLabel    *MyBinSelARTRT[e3TrayCount];
    TLabel    *MyBinSelARTRTLab[e3TrayCount];
    TGroupBox *grpBinDisp[e3TrayCount];

    // Sam 20210609: Fix AOI software upgrade -- golden sizes this eTrayCount
    // (cShowBinSelect.h:519), one slot per unloader-tray-type destination
    // (bigger than e3TrayCount's 33 -- includes eBulkBox distinctly from the
    // Auto/Fix/Magazine set), NOT e3TrayCount; kept exactly as golden sizes
    // it.
    TLabeledEdit *EditAi[eTrayCount];

    // -- other widgets this wave's methods touch ------------------------------
    TGroupBox *gbBinBox   = new TGroupBox();
    TPanel    *pnlMag123  = new TPanel();
    TPanel    *pnlFix789  = new TPanel();
    TPanel    *pnlAuto456 = new TPanel();
    TGroupBox *gbAuto6    = new TGroupBox();
    TfShowBinSelectPanel *palAutoDeviceEjection = new TfShowBinSelectPanel();
    TButton   *btReturn   = new TButton();
    TEdit     *EdLoadCount     = new TEdit();
    TEdit     *ed_AutoCleanCount = new TEdit();
    TfShowBinSelectTimer *TimerAutoCleanCount = new TfShowBinSelectTimer();
    TPageControl *PageControl1 = new TPageControl();

    // -- data (golden cShowBinSelect.h:492-517) -------------------------------
    bool bShow;
    bool bUpdateBinDigital;
    int  iShowBinDigitalTask;
    int  ColorRed, ColorGreen, ColorOrange;
    TStringList *tsUPH;

    // -- form-level geometry (golden TForm->Left/Top/Width/Height) -----------
    // COSMETIC LAYOUT ONLY, offline inert -- nothing in this wave's scope (or
    // its tests) reads these back; declared as plain data (not a widget
    // wrapper) because nothing else in the tree needs a TfShowBinSelect* to
    // BE-A TControl. Same posture as forms/fContactCT.h's identical NOTE on
    // the golden TForm's own Height.
    int Left, Top, Width, Height;

    TfShowBinSelect();   // DEVIATION bootstrap ctor -- see CTOR NOTE above

    void FormDestroy(TObject *Sender);
    void FormClose(TObject *Sender);
    void InitShowBinDigital();
    void SetLabelVisible(int iTag, bool bVisible);
    void SetAutoVisible();
    void ShowInitialString();
    void ShowCategoryBin();
    void UPH_StringGridDblClick(TObject *Sender);
    void CaculateUPH();
    void edSLT01Change(TObject *Sender);
    void btnSetInpputCntClick(TObject *Sender);
    void RefreshAiCnt();
    void ed_AutoCleanCountClick(TObject *Sender);
    void labAuto1Click(TObject *Sender);
    void btReturnClick(TObject *Sender);
    void btnCleanResetClick(TObject *Sender);
    void btnClearCountClick(TObject *Sender);

    int iLowYieldBinSelectContactCount;   // KaiChen 20181115: BinSelect Yield control uses Contact Count

    // GATE (B8): documented no-op this wave -- real body is WAVE B (golden
    // :1479-1633). Declared so btReturnClick's call site compiles/links.
    void PageControl1Change(TObject *Sender);
};

// AI(W906-FW3-ShowBinSelect-WA) 20260818: global NOT defined here -- main-loop
// homecoming (see CTOR NOTE / STATIC-INIT SAFETY above; same posture as
// forms/fContactCT.h / forms/fYieldMonitoring.h before their own swap
// waves).
extern TfShowBinSelect *fShowBinSelect;   // golden cShowBinSelect.h:522

#endif // FORMS_FSHOWBINSELECT_H
