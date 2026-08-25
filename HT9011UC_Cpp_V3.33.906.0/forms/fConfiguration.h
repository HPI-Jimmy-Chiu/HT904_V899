// =============================================================================
//  forms/fConfiguration.h  --  non-VCL stand-in for golden's fConfiguration
//                               form pointer (TfConfiguration)
//
//  AI(W906-FW3-Config-WA) 20260820: NEW FILE. cConfiguration Wave A, batch-5
//  of the FW campaign (alongside cSpeed.cpp/forms/fSpeed.h and
//  cStartCondition.cpp+HandlerSys.cpp/forms/fStartCondition.h, same batch
//  numbering). UNLIKE fLotInfo/fObserver/fSpeed, this form had ZERO prior
//  facade in this tree (`Grep "TfConfiguration|fConfiguration" forms/` was 0
//  hits, 20260820, per docs/RECON_cConfiguration_displayside.md section 0)
//  -- this wave builds the facade skeleton from scratch, not additively on
//  top of a half-built one.
//
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/cConfiguration.cpp (7,808
//  lines, cp950 -- decoded with `python3 -c "open(path,'rb').read()
//  .decode('cp950')"`, 0 U+FFFD, 20260820) + cConfiguration.h (2,407 lines)
//  + cConfiguration.dfm (22,521 lines). Source recon:
//  docs/RECON_cConfiguration_displayside.md, Wave A proposal (section 6,
//  subsections 6.1+6.2, 28 methods).
//
//  LINE-COUNT NOTE: the recon's own section 6.4 rollup table states "354
//  golden lines" for the Wave A batch. Re-summing this wave from the SAME
//  recon's per-method figures in its section 1 table gives a different
//  number: 6.1 sums to 77 lines (53 for the 13 one-line-body handlers +
//  14 for edA22_2Click/edA22_3Click + 10 for EnableRMSFunc), 6.2 sums to
//  212 lines (107 Tray-cluster + 105 HP-cluster) -- 289 lines total, not
//  354. The recon text itself already flags the 6.1 bullet's own header
//  count (12/60L) as inconsistent with its enumerated list (13/53L) and
//  says to trust the enumeration; the 6.4 table apparently propagated the
//  stale headline number instead of the corrected one. FUNCTION COUNT (28)
//  matches exactly across every reading; only the LINE total is off.
//  Recorded here so a future reader citing "Wave A = 354 lines" knows why
//  this file's own line-by-line accounting reads 289.
//
//  WAVE SCOPE -- all 28 golden methods, this file's status
//  --------------------------------------------------------------------------
//    [[6.1 pure-ShowQwertyKey / trivial-widget bucket, RECON section 6.1]]
//    edSetTempClick          golden :5962-5965 (4L)  -- live (WA-1 OPENED 20260824)
//    edSetTempKeyDown        golden :5967-5971 (5L)  -- live (WA-1 OPENED 20260824); Key/Shift params dropped, unread
//    edE31_Fi1XClick         golden :5983-5986 (4L)  -- live (WA-1 OPENED 20260824)
//    edD25_60mmClick         golden :6440-6443 (4L)  -- live (WA-1 OPENED 20260824)
//    edD60_56mmClick         golden :6580-6583 (4L)  -- live (WA-1 OPENED 20260824)
//    edA32_1Click            golden :7611-7614 (4L)  -- live (WA-1 OPENED 20260824)
//    edtC15Click             golden :7675-7678 (4L)  -- live (WA-1 OPENED 20260824)
//    edL09_Sh1LClick         golden :7680-7683 (4L)  -- live (WA-1 OPENED 20260824)
//    edN05_AmbTempClick      golden :7685-7688 (4L)  -- live (WA-1 OPENED 20260824)
//    edN06_PortClick         golden :7690-7693 (4L)  -- live (WA-1 OPENED 20260824)
//    edN06_PasswordClick     golden :7695-7698 (4L)  -- live (WA-1 OPENED 20260824)
//    edtSetIPSCQtyClick      golden :7700-7703 (4L)  -- live (WA-1 OPENED 20260824)
//    edN04_ModelClick        golden :7705-7708 (4L)  -- live (WA-1 OPENED 20260824)
//    edA22_2Click            golden :6593-6601 (9L)  -- REAL edA22_3-nudge if-block; 1 line live (WA-1 OPENED 20260824)
//    edA22_3Click            golden :6603-6607 (5L)  -- dCount consumed again; 1 line live (WA-1 OPENED 20260824)
//    EnableRMSFunc           golden :7726-7735 (10L) -- REAL, no gate, zero widget deps
//    [[6.2 Tray/HP StringGrid cluster, RECON section 6.2]]
//    strngrdTraySelectCell   golden :6839-6844 (6L)  -- REAL; Sender/CanSelect dropped, unread
//    strngrdTrayDblClick     golden :6846-6849 (4L)  -- REAL, calls ->Click() (see DEVIATION)
//    btnModifyTrayClick      golden :6851-6876 (26L) -- REAL structure; 3 branch bodies gated (WA-1)
//    btnAddTrayClick         golden :6878-6888 (11L) -- REAL (needs TfConfigurationGrid::Row)
//    btnDeleteTrayClick      golden :6890-6904 (15L) -- REAL (needs TfConfiguration::Tag)
//    sbtReloadTrayClick      golden :6931-6975 (45L) -- REAL (needs TfConfigurationGrid Font/DefaultColWidth/ColWidths/FixedRows/FixedCols)
//    strngrdHPSelectCell     golden :6977-6982 (6L)  -- REAL, mirrors strngrdTraySelectCell
//    strngrdHPDblClick       golden :6984-6987 (4L)  -- REAL, calls ->Click() (see DEVIATION)
//    btnModifyHPClick        golden :6989-7012 (24L) -- REAL structure; 3 branch bodies gated (WA-1)
//    btnAddHPClick           golden :7014-7024 (11L) -- REAL, mirrors btnAddTrayClick
//    btnDeleteHPClick        golden :7026-7040 (15L) -- REAL, mirrors btnDeleteTrayClick
//    sbtReloadHPClick        golden :7042-7086 (45L) -- REAL, mirrors sbtReloadTrayClick
//  28 methods total. sbUpdateTrayClick (golden :6906-6929, RECON #90) and
//  sbUpdateHPClick (golden :7088-7111, RECON #98) -- the paired (b)
//  write-path "Update" buttons that persist each grid to
//  TrayTablePath/PlateTablePath -- are DELIBERATELY NOT declared here, per
//  RECON section 6.2's own instruction ("不要把 Update 函式一起排進顯示側
//  波次") and RECON 4.3's documented golden copy-paste bug in sbUpdateHPClick
//  (it reloads the TRAY grid and clears the TRAY button's Down state after
//  saving the HP grid) -- both are out of this wave's scope, untouched.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (WA-1) fQwertyKey->ShowQwertyKey(...) -- ALREADY AN ESTABLISHED tree-wide
//      gate (forms/fLotInfo.h WA-8/WB-8/WB-9/WB-10/WB-11/WB-15,
//      ATC/ATCInterface.cpp GATE (3)/(4), VacuumUnit.cpp GATE (3),
//      VacuumUnit/MyVacuumPanel.cpp GATE (3), EJ1N/MyOmronPanel.cpp GATE (3),
//      Public/HTEdit.cpp GATE (6) -- all cite "fQwertyKey has no port
//      anywhere in this tree"). Re-verified this wave (20260820):
//      `Grep "class TfQwertyKey|TfQwertyKey\s*\*|fQwertyKey\s*="` tree-wide
//      -- 0 compiled (non-#if 0) hits AT THAT TIME. [Superseded 20260824:
//      FW-QWKEY1 landed the entity and FW-QWKEY3 opened HTEdit GATE (6), so
//      `fQwertyKey==NULL` there is now LIVE lazy-construction code.]
//      This wave's own recon (RECON_cConfiguration_displayside.md
//      section 6.1) already named this as the shared dependency for the
//      entire 6.1 bucket; per task instruction 4 ("依賴缺就 GATE，本體照
//      翻") the call is gated, the surrounding body stays real.
//      Touches 17 of the 28 methods:
//        13 fully-gated one-liners (edSetTempClick/edSetTempKeyDown/
//        edE31_Fi1XClick/edD25_60mmClick/edD60_56mmClick/edA32_1Click/
//        edtC15Click/edL09_Sh1LClick/edN05_AmbTempClick/edN06_PortClick/
//        edN06_PasswordClick/edtSetIPSCQtyClick/edN04_ModelClick) reduce to
//        empty bodies;
//        edA22_2Click keeps its real edA22_3-nudge if-block; only the
//        ShowQwertyKey line is gated;
//        edA22_3Click keeps `dCount`'s computation real, but the gate makes
//        it write-only -- `(void)dCount;` added right after the gated
//        block, matching the established "gate made a real local go quiet"
//        idiom (forms/fYieldMonitoring.h's `(void)dYield;`/`(void)str;`,
//        forms/fTemperFrom.h's `(void)S1;`);
//        btnModifyTrayClick/btnModifyHPClick keep their real if/else-if/else
//        condition structure (the AnsiPos() column-name checks have no
//        fQwertyKey dependency) and the shared post-if `Cells[...]=
//        edtTemp->Text;` write; only each branch's single ShowQwertyKey
//        call is gated.
//      BEHAVIOUR DELTA while gated: the on-screen numeric keypad never
//      appears; every gated TEdit keeps whatever value it already had --
//      this matches the pre-existing tree-wide fQwertyKey posture, it is
//      not a new delta this wave introduces.
//      OPENED 20260824 (FW-QWKEY2): FW-QWKEY1 (fc08e09) landed forms/
//      fQwertyKey.{h,cpp}; all 21 WA-1 ShowQwertyKey sites in
//      cConfiguration.cpp are un-gated and live (latent: runtime instance
//      NULL until HTEdit GATE (6) wiring). The BEHAVIOUR DELTA above is
//      superseded: the calls execute; the keypad still cannot appear
//      until the keyboard instance is constructed.
//
//  DEVIATION
//  --------------------------------------------------------------------------
//  * strngrdTrayDblClick/strngrdHPDblClick translate `btnModifyTray/HP->
//    Click();` as REAL, compiling code (NOT gated -- TSpeedButton::Click()
//    is a real, existing virtual method, not a missing symbol). Per
//    vclcompat/Controls.h's own TControl::Click() note ("Offline no-op:
//    with no window there is no handler to dispatch to"), this reduces to
//    a no-op at runtime rather than invoking btnModifyTrayClick/
//    btnModifyHPClick's logic -- a tree-wide, pre-existing behaviour (95
//    other `->Click()` call sites already carry the identical property per
//    that header's own count), not something introduced by this wave.
//  * Every golden `__fastcall` handler in this batch drops `TObject
//    *Sender` (and, for edSetTempKeyDown, the unread `WORD &Key,
//    TShiftState Shift`; for strngrdTraySelectCell/strngrdHPSelectCell, the
//    unread `bool &CanSelect`) -- none of these are read anywhere in any of
//    the 28 bodies (verified per-function this wave, including inside the
//    #if 0 blocks where the ONLY remaining `Sender` reference lives, which
//    is exactly why it is safe to drop the parameter -- dead code inside
//    #if 0 is never semantically checked). This matches the tree-wide "drop
//    unused VCL glue parameters" convention (forms/fLotInfo.h's own
//    DEVIATION section states the identical rule, WA-8 precedent). `ACol`/
//    `ARow` ARE read in strngrdTraySelectCell/strngrdHPSelectCell and are
//    kept as real parameters.
//  * `EnableRMSFunc()` keeps its exact golden signature/return type (bool,
//    no parameters) -- golden itself takes none either.
//
//  DESIGN NOTES -- TfConfigurationGrid
//  --------------------------------------------------------------------------
//  golden `strngrdTray`/`strngrdHP` are `TStringGrid*` (cConfiguration.h:
//  1040/1054). vclcompat::TStringGrid (vclcompat/StringGrid.h) deliberately
//  carries ONLY Cells[][]/RowCount/ColCount ("NO rendering, NO FixedRows/
//  FixedCols/.../ColWidths" -- its own file-head SCOPE note) -- but
//  sbtReloadTrayClick/sbtReloadHPClick (golden :6937-6940/:7048-7051) ALSO
//  write ->Font->Size, ->DefaultColWidth, ->ColWidths[0], ->FixedRows,
//  ->FixedCols, and btnAddTrayClick/btnAddHPClick (golden :6887/:7023)
//  write ->Row -- none of which vclcompat::TStringGrid exposes (verified by
//  reading vclcompat/StringGrid.h in full this wave, 20260820). RECON
//  section 8.5 already flagged this exact gap as unverified ("vclcompat
//  StringGrid.h 存在不代表它有...需要的所有成員") -- this wave is the
//  verification, and the answer is "no, it doesn't, for 6 of the members
//  6.2's own functions touch".
//
//  AI(W906-VclGrid-1) 20260820: Font/DefaultColWidth/FixedRows/FixedCols/Row/
//  the ColWidths[] auto-growing proxy MOVED to the shared vclcompat header
//  (vclcompat/StringGrid.h) as part of the 5-subclass TStringGrid-extension
//  consolidation wave -- this facade's own copy of the ColWidths proxy was
//  byte-for-byte identical to forms/fObserver.h's TfObserverGrid::
//  ColWidthsProxy, and the two were about to become a THIRD independent
//  fork the next time a form needed the same set, so they collapsed home
//  instead. TfConfigurationGrid PUBLICLY INHERITS vclcompat::TStringGrid (so
//  it still converts for free to any `vclcompat::TStringGrid*` parameter,
//  should a future wave need that) and now keeps ONLY a pass-through
//  hydration ctor -- see vclcompat/StringGrid.h's own updated SCOPE banner
//  for the base's full current member set, defaults, and the verified-inert
//  reasoning behind dropping the ctor's old eager ColWidths pre-sizing. NOT
//  a GATE: every one of these properties still compiles and holds the exact
//  value golden assigns; only the RENDERING implied by a real Font/
//  DefaultColWidth/FixedRows/Row (i.e. actually drawing rows/columns/fonts
//  sized accordingly) remains out of scope -- unchanged from before this
//  wave.
//
//  DFM HYDRATION -- neither strngrdTray (cConfiguration.dfm:21204-21214) nor
//  strngrdHP (cConfiguration.dfm:22475-22485) sets an explicit ColCount=/
//  RowCount= property in the golden .dfm (both blocks only carry Left/Top/
//  Width/Height/Align/Color/TabOrder/OnDblClick/OnSelectCell) -- so both
//  fall back to real VCL's design-time default of 5x5, matching
//  vclcompat::TStringGrid's own documented default (and this file's
//  TfConfigurationGrid constructor default arguments). This is consistent
//  with golden's runtime behaviour: sbtReloadTrayClick/sbtReloadHPClick
//  unconditionally overwrite ColCount=16 and RowCount=List->Count the first
//  time either grid is populated, so the .dfm default is only ever visible
//  for the instant between construction and the first reload.
//
//  INTEGRATION STATUS -- `fConfiguration` global NOT declared this wave.
//  Matches the posture already established by forms/fTemperFrom.h/
//  cTemperFrom.cpp and forms/fSpeed.h/cSpeed.cpp: the ctor (RECON #1, golden
//  :109-225, 117 lines -- ReadLockByFile()+InitConfigEdtList()+
//  ReadConfigStandard()+WriteContactData()+dynamic widget alloc+WinSock
//  hostname lookup) is (b) write-path and is NOT one of this wave's 28
//  methods, so there is no translated constructor to build a real instance
//  from. Wiring up a live global would mean either fabricating a
//  constructor this wave was not asked to write, or shipping a
//  default-constructed instance whose field values do not match golden's
//  real startup state -- deferred to the wave that translates the ctor.
// =============================================================================
#ifndef FORMS_FCONFIGURATION_H
#define FORMS_FCONFIGURATION_H

#include "vclcompat/vcl_compat.h"   // AnsiString, TStringList, StringReplace/TReplaceFlags/rfReplaceAll -- brought to global scope
#include "vclcompat/Controls.h"     // TEdit, TSpeedButton, TFont, TControl (stock widgets, reused as-is)
#include "vclcompat/StringGrid.h"   // vclcompat::TStringGrid -- TfConfigurationGrid's base
// AI(W906-VclGrid-1) 20260820: <vector> dropped -- it was only for
// TfConfigurationGrid::colWidths_, which moved to the base (StringGrid.h
// owns that vector now).

using vclcompat::TEdit;
using vclcompat::TSpeedButton;
using vclcompat::TFont;
using vclcompat::TStringGrid;

// ===========================================================================
//  TfConfigurationGrid -- see DESIGN NOTES above.
// ===========================================================================
class TfConfigurationGrid : public vclcompat::TStringGrid
{
public:
    // golden .dfm design-time ColCount/RowCount -- see DFM HYDRATION above:
    // neither strngrdTray nor strngrdHP sets either property explicitly, so
    // both use real VCL's design-time default (5x5), matching the base
    // class's own default arguments.
    //
    // AI(W906-VclGrid-1) 20260820: Font/DefaultColWidth/FixedRows/FixedCols/
    // Row/ColWidths already collapsed to the base (this class's own SCOPE-
    // comment above has the provenance) -- this class now only remains as a
    // pass-through hydration ctor. Base ctor's default DefaultColWidth (64)
    // matches this form's own historical value exactly, so nothing needs
    // re-setting here.
    explicit TfConfigurationGrid(int initialColCount = 5, int initialRowCount = 5)
        : vclcompat::TStringGrid(initialColCount, initialRowCount)
    {}

};

// ===========================================================================
//  TfConfiguration -- see WAVE SCOPE / GATE REGISTER / DEVIATION above.
//  No base class, matching the plain-facade convention already used by
//  forms/fLotInfo.h's TfLotInfo (no `: public TObject`/`: public TForm`).
// ===========================================================================
class TfConfiguration
{
public:
    // -- 6.1 bucket: TEdit widgets, each with exactly 1 dependent handler --
    TEdit *edSetTemp      = new TEdit();   // golden cConfiguration.h:81
    TEdit *edE31_Fi1X     = new TEdit();   // golden cConfiguration.h:1833
    TEdit *edD25_60mm     = new TEdit();   // golden cConfiguration.h:142
    TEdit *edD60_56mm     = new TEdit();   // golden cConfiguration.h:952
    TEdit *edA32_1        = new TEdit();   // golden cConfiguration.h:1089
    TEdit *edtC15         = new TEdit();   // golden cConfiguration.h:1228
    TEdit *edL09_Sh1L     = new TEdit();   // golden cConfiguration.h:164
    TEdit *edN05_AmbTemp  = new TEdit();   // golden cConfiguration.h:193
    TEdit *edN06_Port     = new TEdit();   // golden cConfiguration.h:876
    TEdit *edN06_Password = new TEdit();   // golden cConfiguration.h:211
    TEdit *edtSetIPSCQty  = new TEdit();   // golden cConfiguration.h:1078
    TEdit *edN04_Model    = new TEdit();   // golden cConfiguration.h:764
    TEdit *edA22_2        = new TEdit();   // golden cConfiguration.h:807
    TEdit *edA22_3        = new TEdit();   // golden cConfiguration.h:808

    // -- 6.2 bucket: Tray/HP StringGrid cluster --
    TfConfigurationGrid *strngrdTray = new TfConfigurationGrid();  // golden cConfiguration.h:1040; dfm:21204-21214 (no explicit dims -> default 5x5)
    TfConfigurationGrid *strngrdHP   = new TfConfigurationGrid();  // golden cConfiguration.h:1054; dfm:22475-22485 (no explicit dims -> default 5x5)
    TEdit *edtTemp = new TEdit();   // golden cConfiguration.h:1047 -- SHARED scratch edit for BOTH Tray and HP "Modify" handlers

    TSpeedButton *btnModifyTray = new TSpeedButton();   // golden cConfiguration.h:1044
    TSpeedButton *btnAddTray    = new TSpeedButton();   // golden cConfiguration.h:1042
    TSpeedButton *btnDeleteTray = new TSpeedButton();   // golden cConfiguration.h:1043
    TSpeedButton *sbtReloadTray = new TSpeedButton();   // golden cConfiguration.h:1046
    TSpeedButton *btnModifyHP   = new TSpeedButton();   // golden cConfiguration.h:1051
    TSpeedButton *btnAddHP      = new TSpeedButton();   // golden cConfiguration.h:1049
    TSpeedButton *btnDeleteHP   = new TSpeedButton();   // golden cConfiguration.h:1050
    TSpeedButton *sbtReloadHP   = new TSpeedButton();   // golden cConfiguration.h:1053

    // -- plain data --
    int iSelTrayRow = 0;   // golden cConfiguration.h:2397
    int iSelTrayCol = 0;   // golden cConfiguration.h:2398
    int iSelHPRow   = 0;   // golden cConfiguration.h:2399
    int iSelHPCol   = 0;   // golden cConfiguration.h:2400
    int Tag         = 0;   // golden TComponent->Tag (bare `Tag=` writes in btnDeleteTrayClick/btnDeleteHPClick)

    // -- 6.1 methods --
    // AI(W906-FW-QWKEY2) 20260824: TObject *Sender restored on all 15 WA-1
    // methods -- the un-gated ShowQwertyKey((TEdit*)Sender,...) calls read
    // it (the drop was only valid while the bodies were gated).
    void edSetTempClick(TObject *Sender);
    void edSetTempKeyDown(TObject *Sender);
    void edE31_Fi1XClick(TObject *Sender);
    void edD25_60mmClick(TObject *Sender);
    void edD60_56mmClick(TObject *Sender);
    void edA32_1Click(TObject *Sender);
    void edtC15Click(TObject *Sender);
    void edL09_Sh1LClick(TObject *Sender);
    void edN05_AmbTempClick(TObject *Sender);
    void edN06_PortClick(TObject *Sender);
    void edN06_PasswordClick(TObject *Sender);
    void edtSetIPSCQtyClick(TObject *Sender);
    void edN04_ModelClick(TObject *Sender);
    void edA22_2Click(TObject *Sender);
    void edA22_3Click(TObject *Sender);
    bool EnableRMSFunc();

    // -- 6.2 methods --
    void strngrdTraySelectCell(int ACol, int ARow);
    void strngrdTrayDblClick();
    void btnModifyTrayClick();
    void btnAddTrayClick();
    void btnDeleteTrayClick();
    void sbtReloadTrayClick();
    void strngrdHPSelectCell(int ACol, int ARow);
    void strngrdHPDblClick();
    void btnModifyHPClick();
    void btnAddHPClick();
    void btnDeleteHPClick();
    void sbtReloadHPClick();

    // -- Wave FW-CFG-W2 methods (bodies: cConfiguration.cpp) --------------
    virtual void InitConfigEdtList_ItemD();   // golden :1244-1759
    virtual void InitConfigEdtList_ItemE();   // golden :1761-2111
    virtual void InitConfigEdtList_ItemF();   // golden :2113-2328
    virtual void InitConfigEdtList_ItemG();   // golden :2330-2419
    virtual void InitConfigEdtList_ItemL();   // golden :2873-3118
    virtual void InitConfigEdtList_ItemM();   // golden :3120-3161
    virtual void InitConfigEdtList_ItemO();   // golden :3921-4095

    // -- Wave FW-CFG-W1 methods (bodies: cConfiguration.cpp) --------------
    virtual void InitConfigEdtList_ItemA();   // golden :378-977
    virtual void InitConfigEdtList_ItemB();   // golden :979-1035
    virtual void InitConfigEdtList_ItemC();   // golden :1037-1242

    // ========================================================================
    // AI(W906-FW-CFG-W1) 20260825: the widgets InitConfigEdtList_ItemA/B/C
    // register. 212 members, every type taken from golden cConfiguration.h and
    // cited per line -- nothing guessed.
    //
    // These are NOT inert surface: each one is the FIRST argument of an
    // `elConfig->Add(widget, &IniConfig.field, ECBool/ECInteger/ECText,
    // section, key, ...)` call, i.e. the registration table that binds a screen
    // control to a config field and its ini section/key. That table IS the
    // Configuration screen's schema, which is exactly what the web HMI needs to
    // render it, so landing it is the point of the wave rather than a side
    // effect of it.
    //
    // The port's HTEditList (Public/HTEditList.h:178-228) and the elConfig /
    // elConfig_byRecipe / cbLastSet globals (:247/:252/:253) already exist and
    // its Add signature is argument-for-argument golden's, so the 863
    // translated lines needed no adaptation.
    // ========================================================================
    // golden TCheckBox (121)
    TCheckBox     *cbA01         = new TCheckBox();           // golden cConfiguration.h:498
    TCheckBox     *cbA01_1       = new TCheckBox();           // golden cConfiguration.h:500
    TCheckBox     *cbA01_2       = new TCheckBox();           // golden cConfiguration.h:2189
    TCheckBox     *cbA02         = new TCheckBox();           // golden cConfiguration.h:404
    TCheckBox     *cbA03         = new TCheckBox();           // golden cConfiguration.h:400
    TCheckBox     *cbA04         = new TCheckBox();           // golden cConfiguration.h:401
    TCheckBox     *cbA05         = new TCheckBox();           // golden cConfiguration.h:402
    TCheckBox     *cbA08         = new TCheckBox();           // golden cConfiguration.h:405
    TCheckBox     *cbA09         = new TCheckBox();           // golden cConfiguration.h:406
    TCheckBox     *cbA10         = new TCheckBox();           // golden cConfiguration.h:585
    TCheckBox     *cbA10_3       = new TCheckBox();           // golden cConfiguration.h:474
    TCheckBox     *cbA10_5       = new TCheckBox();           // golden cConfiguration.h:549
    TCheckBox     *cbA10_6       = new TCheckBox();           // golden cConfiguration.h:1657
    TCheckBox     *cbA10_7       = new TCheckBox();           // golden cConfiguration.h:2137
    TCheckBox     *cbA11         = new TCheckBox();           // golden cConfiguration.h:411
    TCheckBox     *cbA12         = new TCheckBox();           // golden cConfiguration.h:415
    TCheckBox     *cbA14         = new TCheckBox();           // golden cConfiguration.h:418
    TCheckBox     *cbA15         = new TCheckBox();           // golden cConfiguration.h:419
    TCheckBox     *cbA15_1       = new TCheckBox();           // golden cConfiguration.h:2245
    TCheckBox     *cbA17_1       = new TCheckBox();           // golden cConfiguration.h:1057
    TCheckBox     *cbA17_2       = new TCheckBox();           // golden cConfiguration.h:1058
    TCheckBox     *cbA19         = new TCheckBox();           // golden cConfiguration.h:421
    TCheckBox     *cbA20_1       = new TCheckBox();           // golden cConfiguration.h:626
    TCheckBox     *cbA20_2       = new TCheckBox();           // golden cConfiguration.h:627
    TCheckBox     *cbA20_3       = new TCheckBox();           // golden cConfiguration.h:628
    TCheckBox     *cbA20_4       = new TCheckBox();           // golden cConfiguration.h:629
    TCheckBox     *cbA20_5       = new TCheckBox();           // golden cConfiguration.h:630
    TCheckBox     *cbA21         = new TCheckBox();           // golden cConfiguration.h:800
    TCheckBox     *cbA22_1       = new TCheckBox();           // golden cConfiguration.h:806
    TCheckBox     *cbA23         = new TCheckBox();           // golden cConfiguration.h:809
    TCheckBox     *cbA24         = new TCheckBox();           // golden cConfiguration.h:810
    TCheckBox     *cbA26         = new TCheckBox();           // golden cConfiguration.h:819
    TCheckBox     *cbA27         = new TCheckBox();           // golden cConfiguration.h:821
    TCheckBox     *cbA27_1       = new TCheckBox();           // golden cConfiguration.h:820
    TCheckBox     *cbA29         = new TCheckBox();           // golden cConfiguration.h:822
    TCheckBox     *cbA30         = new TCheckBox();           // golden cConfiguration.h:2056
    TCheckBox     *cbA31         = new TCheckBox();           // golden cConfiguration.h:933
    TCheckBox     *cbA31_1       = new TCheckBox();           // golden cConfiguration.h:938
    TCheckBox     *cbA32         = new TCheckBox();           // golden cConfiguration.h:1088
    TCheckBox     *cbA32_01      = new TCheckBox();           // golden cConfiguration.h:1092
    TCheckBox     *cbA32_02      = new TCheckBox();           // golden cConfiguration.h:1093
    TCheckBox     *cbA32_03      = new TCheckBox();           // golden cConfiguration.h:1094
    TCheckBox     *cbA32_04      = new TCheckBox();           // golden cConfiguration.h:1095
    TCheckBox     *cbA32_05      = new TCheckBox();           // golden cConfiguration.h:1096
    TCheckBox     *cbA32_06      = new TCheckBox();           // golden cConfiguration.h:1097
    TCheckBox     *cbA32_07      = new TCheckBox();           // golden cConfiguration.h:1098
    TCheckBox     *cbA32_08      = new TCheckBox();           // golden cConfiguration.h:1099
    TCheckBox     *cbA32_09      = new TCheckBox();           // golden cConfiguration.h:1100
    TCheckBox     *cbA32_10      = new TCheckBox();           // golden cConfiguration.h:1101
    TCheckBox     *cbA32_2       = new TCheckBox();           // golden cConfiguration.h:1090
    TCheckBox     *cbA32_3       = new TCheckBox();           // golden cConfiguration.h:1173
    TCheckBox     *cbA33         = new TCheckBox();           // golden cConfiguration.h:1102
    TCheckBox     *cbA35         = new TCheckBox();           // golden cConfiguration.h:1104
    TCheckBox     *cbA36         = new TCheckBox();           // golden cConfiguration.h:1241
    TCheckBox     *cbA37         = new TCheckBox();           // golden cConfiguration.h:1282
    TCheckBox     *cbA38         = new TCheckBox();           // golden cConfiguration.h:1294
    TCheckBox     *cbA39         = new TCheckBox();           // golden cConfiguration.h:1327
    TCheckBox     *cbA40         = new TCheckBox();           // golden cConfiguration.h:1594
    TCheckBox     *cbA51         = new TCheckBox();           // golden cConfiguration.h:1318
    TCheckBox     *cbA55         = new TCheckBox();           // golden cConfiguration.h:1323
    TCheckBox     *cbA56_1       = new TCheckBox();           // golden cConfiguration.h:1073
    TCheckBox     *cbA57_1       = new TCheckBox();           // golden cConfiguration.h:1268
    TCheckBox     *cbA57_2       = new TCheckBox();           // golden cConfiguration.h:1269
    TCheckBox     *cbA57_3       = new TCheckBox();           // golden cConfiguration.h:1270
    TCheckBox     *cbA58         = new TCheckBox();           // golden cConfiguration.h:1581
    TCheckBox     *cbA60_1       = new TCheckBox();           // golden cConfiguration.h:2124
    TCheckBox     *cbA61         = new TCheckBox();           // golden cConfiguration.h:1716
    TCheckBox     *cbA62         = new TCheckBox();           // golden cConfiguration.h:1717
    TCheckBox     *cbA65         = new TCheckBox();           // golden cConfiguration.h:1627
    TCheckBox     *cbA66         = new TCheckBox();           // golden cConfiguration.h:1628
    TCheckBox     *cbA67         = new TCheckBox();           // golden cConfiguration.h:1718
    TCheckBox     *cbA68         = new TCheckBox();           // golden cConfiguration.h:1719
    TCheckBox     *cbA69         = new TCheckBox();           // golden cConfiguration.h:2099
    TCheckBox     *cbA71         = new TCheckBox();           // golden cConfiguration.h:2187
    TCheckBox     *cbA72         = new TCheckBox();           // golden cConfiguration.h:2226
    TCheckBox     *cbA73         = new TCheckBox();           // golden cConfiguration.h:2227
    TCheckBox     *cbA74         = new TCheckBox();           // golden cConfiguration.h:2229
    TCheckBox     *cbA75         = new TCheckBox();           // golden cConfiguration.h:2246
    TCheckBox     *cbA81WaitSECS = new TCheckBox();           // golden cConfiguration.h:2225
    TCheckBox     *cbB01         = new TCheckBox();           // golden cConfiguration.h:559
    TCheckBox     *cbB02         = new TCheckBox();           // golden cConfiguration.h:565
    TCheckBox     *cbB03         = new TCheckBox();           // golden cConfiguration.h:1464
    TCheckBox     *cbB05         = new TCheckBox();           // golden cConfiguration.h:1672
    TCheckBox     *cbB11Enable   = new TCheckBox();           // golden cConfiguration.h:2096
    TCheckBox     *cbB12Enable   = new TCheckBox();           // golden cConfiguration.h:2082
    TCheckBox     *cbC01         = new TCheckBox();           // golden cConfiguration.h:1219
    TCheckBox     *cbC02         = new TCheckBox();           // golden cConfiguration.h:1196
    TCheckBox     *cbC03         = new TCheckBox();           // golden cConfiguration.h:1197
    TCheckBox     *cbC04         = new TCheckBox();           // golden cConfiguration.h:1198
    TCheckBox     *cbC05_ATC     = new TCheckBox();           // golden cConfiguration.h:1610
    TCheckBox     *cbC05_Motor   = new TCheckBox();           // golden cConfiguration.h:1605
    TCheckBox     *cbC05_Temp    = new TCheckBox();           // golden cConfiguration.h:1604
    TCheckBox     *cbC05_Vacuum  = new TCheckBox();           // golden cConfiguration.h:1608
    TCheckBox     *cbC06_01      = new TCheckBox();           // golden cConfiguration.h:1202
    TCheckBox     *cbC06_02      = new TCheckBox();           // golden cConfiguration.h:1203
    TCheckBox     *cbC06_03      = new TCheckBox();           // golden cConfiguration.h:1204
    TCheckBox     *cbC06_04      = new TCheckBox();           // golden cConfiguration.h:1205
    TCheckBox     *cbC06_05      = new TCheckBox();           // golden cConfiguration.h:1206
    TCheckBox     *cbC06_06      = new TCheckBox();           // golden cConfiguration.h:1207
    TCheckBox     *cbC06_07      = new TCheckBox();           // golden cConfiguration.h:1208
    TCheckBox     *cbC06_08      = new TCheckBox();           // golden cConfiguration.h:1209
    TCheckBox     *cbC06_09      = new TCheckBox();           // golden cConfiguration.h:1210
    TCheckBox     *cbC06_10      = new TCheckBox();           // golden cConfiguration.h:1211
    TCheckBox     *cbC06_11      = new TCheckBox();           // golden cConfiguration.h:1212
    TCheckBox     *cbC06_12      = new TCheckBox();           // golden cConfiguration.h:1213
    TCheckBox     *cbC07         = new TCheckBox();           // golden cConfiguration.h:1214
    TCheckBox     *cbC08         = new TCheckBox();           // golden cConfiguration.h:1215
    TCheckBox     *cbC08_1       = new TCheckBox();           // golden cConfiguration.h:1242
    TCheckBox     *cbC09         = new TCheckBox();           // golden cConfiguration.h:1217
    TCheckBox     *cbC10         = new TCheckBox();           // golden cConfiguration.h:1218
    TCheckBox     *cbC11         = new TCheckBox();           // golden cConfiguration.h:1224
    TCheckBox     *cbC12         = new TCheckBox();           // golden cConfiguration.h:1226
    TCheckBox     *cbC13         = new TCheckBox();           // golden cConfiguration.h:1225
    TCheckBox     *cbC16         = new TCheckBox();           // golden cConfiguration.h:1324
    TCheckBox     *cbC17         = new TCheckBox();           // golden cConfiguration.h:2115
    TCheckBox     *cbC17_1       = new TCheckBox();           // golden cConfiguration.h:2117
    TCheckBox     *cbC24         = new TCheckBox();           // golden cConfiguration.h:2224
    TCheckBox     *chA16         = new TCheckBox();           // golden cConfiguration.h:420
    TCheckBox     *chkA09_1      = new TCheckBox();           // golden cConfiguration.h:1235
    TCheckBox     *chkA50        = new TCheckBox();           // golden cConfiguration.h:1319
    TCheckBox     *chkC14        = new TCheckBox();           // golden cConfiguration.h:1220

    // golden TComboBox (2)
    TComboBox     *cbA10_3_ARTTestMode  = new TComboBox();           // golden cConfiguration.h:473
    TComboBox     *cbA10_6_HANA_ARTMode = new TComboBox();           // golden cConfiguration.h:1658

    // golden TEdit (59)
    TEdit         *edA01               = new TEdit();               // golden cConfiguration.h:499
    TEdit         *edA10_2             = new TEdit();               // golden cConfiguration.h:471
    TEdit         *edA10_3             = new TEdit();               // golden cConfiguration.h:472
    TEdit         *edA10_4             = new TEdit();               // golden cConfiguration.h:476
    TEdit         *edA11               = new TEdit();               // golden cConfiguration.h:412
    TEdit         *edA12               = new TEdit();               // golden cConfiguration.h:417
    TEdit         *edA15_ESDReportTime = new TEdit();               // golden cConfiguration.h:424
    TEdit         *edA25_1             = new TEdit();               // golden cConfiguration.h:814
    TEdit         *edA25_2             = new TEdit();               // golden cConfiguration.h:815
    TEdit         *edA39               = new TEdit();               // golden cConfiguration.h:1326
    TEdit         *edA56_2             = new TEdit();               // golden cConfiguration.h:1075
    TEdit         *edA56_3             = new TEdit();               // golden cConfiguration.h:1074
    TEdit         *edA60_1             = new TEdit();               // golden cConfiguration.h:2125
    TEdit         *edA60_2             = new TEdit();               // golden cConfiguration.h:2126
    TEdit         *edA60_3             = new TEdit();               // golden cConfiguration.h:2127
    TEdit         *edA60_4             = new TEdit();               // golden cConfiguration.h:2128
    TEdit         *edA60_5             = new TEdit();               // golden cConfiguration.h:2129
    TEdit         *edA71               = new TEdit();               // golden cConfiguration.h:2186
    TEdit         *edB01               = new TEdit();               // golden cConfiguration.h:560
    TEdit         *edB01_1             = new TEdit();               // golden cConfiguration.h:561
    TEdit         *edB02               = new TEdit();               // golden cConfiguration.h:564
    TEdit         *edB03_1             = new TEdit();               // golden cConfiguration.h:1465
    TEdit         *edB03_2             = new TEdit();               // golden cConfiguration.h:1466
    TEdit         *edB11PATServerPath  = new TEdit();               // golden cConfiguration.h:2097
    TEdit         *edB12Path           = new TEdit();               // golden cConfiguration.h:2083
    TEdit         *edB13DownloadPath   = new TEdit();               // golden cConfiguration.h:2093
    TEdit         *edB13UploadPath     = new TEdit();               // golden cConfiguration.h:2092
    TEdit         *edB14IntervalTime   = new TEdit();               // golden cConfiguration.h:2087
    TEdit         *edB14ReportRealTime = new TEdit();               // golden cConfiguration.h:2088
    TEdit         *edC05_ATC           = new TEdit();               // golden cConfiguration.h:1611
    TEdit         *edC05_Motor         = new TEdit();               // golden cConfiguration.h:1606
    TEdit         *edC05_Temp          = new TEdit();               // golden cConfiguration.h:1607
    TEdit         *edC05_Vacuum        = new TEdit();               // golden cConfiguration.h:1609
    TEdit         *edC09               = new TEdit();               // golden cConfiguration.h:1216
    TEdit         *edtA12_1            = new TEdit();               // golden cConfiguration.h:416
    TEdit         *edtA28_1            = new TEdit();               // golden cConfiguration.h:818
    TEdit         *edtB05              = new TEdit();               // golden cConfiguration.h:1673
    TEdit         *edtC17              = new TEdit();               // golden cConfiguration.h:2116
    TEdit         *edtC21_Auto1        = new TEdit();               // golden cConfiguration.h:1396
    TEdit         *edtC21_Auto1_b      = new TEdit();               // golden cConfiguration.h:1426
    TEdit         *edtC21_Auto1_m      = new TEdit();               // golden cConfiguration.h:1411
    TEdit         *edtC21_Auto2        = new TEdit();               // golden cConfiguration.h:1397
    TEdit         *edtC21_Auto2_b      = new TEdit();               // golden cConfiguration.h:1427
    TEdit         *edtC21_Auto2_m      = new TEdit();               // golden cConfiguration.h:1412
    TEdit         *edtC21_Auto3        = new TEdit();               // golden cConfiguration.h:1398
    TEdit         *edtC21_Auto3_b      = new TEdit();               // golden cConfiguration.h:1428
    TEdit         *edtC21_Auto3_m      = new TEdit();               // golden cConfiguration.h:1413
    TEdit         *edtC21_HP1          = new TEdit();               // golden cConfiguration.h:1392
    TEdit         *edtC21_HP1_b        = new TEdit();               // golden cConfiguration.h:1422
    TEdit         *edtC21_HP1_m        = new TEdit();               // golden cConfiguration.h:1407
    TEdit         *edtC21_HP2          = new TEdit();               // golden cConfiguration.h:1393
    TEdit         *edtC21_HP2_b        = new TEdit();               // golden cConfiguration.h:1423
    TEdit         *edtC21_HP2_m        = new TEdit();               // golden cConfiguration.h:1408
    TEdit         *edtC21_SHT1         = new TEdit();               // golden cConfiguration.h:1394
    TEdit         *edtC21_SHT1_b       = new TEdit();               // golden cConfiguration.h:1424
    TEdit         *edtC21_SHT1_m       = new TEdit();               // golden cConfiguration.h:1409
    TEdit         *edtC21_SHT2         = new TEdit();               // golden cConfiguration.h:1395
    TEdit         *edtC21_SHT2_b       = new TEdit();               // golden cConfiguration.h:1425
    TEdit         *edtC21_SHT2_m       = new TEdit();               // golden cConfiguration.h:1410

    // golden TGroupBox (18)
    TGroupBox     *gbA10    = new TGroupBox();           // golden cConfiguration.h:466
    TGroupBox     *gbA22    = new TGroupBox();           // golden cConfiguration.h:801
    TGroupBox     *gbA31    = new TGroupBox();           // golden cConfiguration.h:932
    TGroupBox     *gbA56    = new TGroupBox();           // golden cConfiguration.h:1070
    TGroupBox     *gbA57    = new TGroupBox();           // golden cConfiguration.h:1267
    TGroupBox     *gbC05    = new TGroupBox();           // golden cConfiguration.h:1199
    TGroupBox     *gbC06    = new TGroupBox();           // golden cConfiguration.h:1201
    TGroupBox     *grpA25   = new TGroupBox();           // golden cConfiguration.h:811
    TGroupBox     *grpA28   = new TGroupBox();           // golden cConfiguration.h:816
    TGroupBox     *grpA32   = new TGroupBox();           // golden cConfiguration.h:1085
    TGroupBox     *grpA32_1 = new TGroupBox();           // golden cConfiguration.h:1091
    TGroupBox     *grpA55   = new TGroupBox();           // golden cConfiguration.h:1321
    TGroupBox     *grpA60   = new TGroupBox();           // golden cConfiguration.h:2118
    TGroupBox     *grpB01   = new TGroupBox();           // golden cConfiguration.h:556
    TGroupBox     *grpB02   = new TGroupBox();           // golden cConfiguration.h:562
    TGroupBox     *grpB03   = new TGroupBox();           // golden cConfiguration.h:1460
    TGroupBox     *grpC17   = new TGroupBox();           // golden cConfiguration.h:2113
    TGroupBox     *grpC21   = new TGroupBox();           // golden cConfiguration.h:1382

    // golden TLabel (6)
    TLabel        *lblA16   = new TLabel();              // golden cConfiguration.h:409
    TLabel        *lblA60_1 = new TLabel();              // golden cConfiguration.h:2119
    TLabel        *lblA60_2 = new TLabel();              // golden cConfiguration.h:2120
    TLabel        *lblA60_3 = new TLabel();              // golden cConfiguration.h:2121
    TLabel        *lblA60_4 = new TLabel();              // golden cConfiguration.h:2122
    TLabel        *lblA60_5 = new TLabel();              // golden cConfiguration.h:2123

    // golden TPanel (3)
    TPanel        *palA10_ART = new TPanel();              // golden cConfiguration.h:467
    TPanel        *pal_B11    = new TPanel();              // golden cConfiguration.h:2079
    TPanel        *pnlA12     = new TPanel();              // golden cConfiguration.h:413

    // golden TRadioGroup (1)
    TRadioGroup   *rgC05 = new TRadioGroup();         // golden cConfiguration.h:1200

    // golden TSpeedButton (1)
    TSpeedButton  *spbA27 = new TSpeedButton();        // golden cConfiguration.h:799

    // golden TTabSheet (1)
    TTabSheet     *tsb00 = new TTabSheet();           // golden cConfiguration.h:552

    // ========================================================================
    // AI(W906-FW-CFG-W2) 20260825: the widgets ItemD/ItemE/ItemF/ItemG/ItemL/ItemM/ItemO register.
    // 495 members, same derivation as FW-CFG-W1: every type read out of golden
    // cConfiguration.h and cited per line, nothing guessed.
    // ========================================================================
    // golden TCheckBox (276)
    TCheckBox     *cbD01                 = new TCheckBox();           // golden cConfiguration.h:115
    TCheckBox     *cbD01_1               = new TCheckBox();           // golden cConfiguration.h:924
    TCheckBox     *cbD02                 = new TCheckBox();           // golden cConfiguration.h:116
    TCheckBox     *cbD04                 = new TCheckBox();           // golden cConfiguration.h:438
    TCheckBox     *cbD05                 = new TCheckBox();           // golden cConfiguration.h:610
    TCheckBox     *cbD05_1               = new TCheckBox();           // golden cConfiguration.h:2098
    TCheckBox     *cbD06                 = new TCheckBox();           // golden cConfiguration.h:1140
    TCheckBox     *cbD10                 = new TCheckBox();           // golden cConfiguration.h:119
    TCheckBox     *cbD11                 = new TCheckBox();           // golden cConfiguration.h:120
    TCheckBox     *cbD12                 = new TCheckBox();           // golden cConfiguration.h:121
    TCheckBox     *cbD14                 = new TCheckBox();           // golden cConfiguration.h:237
    TCheckBox     *cbD15                 = new TCheckBox();           // golden cConfiguration.h:257
    TCheckBox     *cbD16                 = new TCheckBox();           // golden cConfiguration.h:322
    TCheckBox     *cbD21                 = new TCheckBox();           // golden cConfiguration.h:124
    TCheckBox     *cbD22_1               = new TCheckBox();           // golden cConfiguration.h:509
    TCheckBox     *cbD22_2               = new TCheckBox();           // golden cConfiguration.h:511
    TCheckBox     *cbD22_3               = new TCheckBox();           // golden cConfiguration.h:1317
    TCheckBox     *cbD22_3_              = new TCheckBox();           // golden cConfiguration.h:1353
    TCheckBox     *cbD22_4               = new TCheckBox();           // golden cConfiguration.h:1429
    TCheckBox     *cbD23                 = new TCheckBox();           // golden cConfiguration.h:228
    TCheckBox     *cbD24                 = new TCheckBox();           // golden cConfiguration.h:127
    TCheckBox     *cbD26                 = new TCheckBox();           // golden cConfiguration.h:128
    TCheckBox     *cbD26_1               = new TCheckBox();           // golden cConfiguration.h:320
    TCheckBox     *cbD26_2               = new TCheckBox();           // golden cConfiguration.h:423
    TCheckBox     *cbD26_3               = new TCheckBox();           // golden cConfiguration.h:1612
    TCheckBox     *cbD27                 = new TCheckBox();           // golden cConfiguration.h:130
    TCheckBox     *cbD28                 = new TCheckBox();           // golden cConfiguration.h:882
    TCheckBox     *cbD29                 = new TCheckBox();           // golden cConfiguration.h:131
    TCheckBox     *cbD30                 = new TCheckBox();           // golden cConfiguration.h:104
    TCheckBox     *cbD31                 = new TCheckBox();           // golden cConfiguration.h:105
    TCheckBox     *cbD32                 = new TCheckBox();           // golden cConfiguration.h:106
    TCheckBox     *cbD33                 = new TCheckBox();           // golden cConfiguration.h:107
    TCheckBox     *cbD34                 = new TCheckBox();           // golden cConfiguration.h:108
    TCheckBox     *cbD35                 = new TCheckBox();           // golden cConfiguration.h:109
    TCheckBox     *cbD36                 = new TCheckBox();           // golden cConfiguration.h:230
    TCheckBox     *cbD36_1               = new TCheckBox();           // golden cConfiguration.h:590
    TCheckBox     *cbD36_2               = new TCheckBox();           // golden cConfiguration.h:1142
    TCheckBox     *cbD37                 = new TCheckBox();           // golden cConfiguration.h:268
    TCheckBox     *cbD38                 = new TCheckBox();           // golden cConfiguration.h:589
    TCheckBox     *cbD40                 = new TCheckBox();           // golden cConfiguration.h:94
    TCheckBox     *cbD41                 = new TCheckBox();           // golden cConfiguration.h:1369
    TCheckBox     *cbD42                 = new TCheckBox();           // golden cConfiguration.h:95
    TCheckBox     *cbD43                 = new TCheckBox();           // golden cConfiguration.h:102
    TCheckBox     *cbD43_1               = new TCheckBox();           // golden cConfiguration.h:465
    TCheckBox     *cbD43_2               = new TCheckBox();           // golden cConfiguration.h:594
    TCheckBox     *cbD44                 = new TCheckBox();           // golden cConfiguration.h:97
    TCheckBox     *cbD45                 = new TCheckBox();           // golden cConfiguration.h:98
    TCheckBox     *cbD47                 = new TCheckBox();           // golden cConfiguration.h:746
    TCheckBox     *cbD48                 = new TCheckBox();           // golden cConfiguration.h:101
    TCheckBox     *cbD49                 = new TCheckBox();           // golden cConfiguration.h:449
    TCheckBox     *cbD50                 = new TCheckBox();           // golden cConfiguration.h:546
    TCheckBox     *cbD51                 = new TCheckBox();           // golden cConfiguration.h:243
    TCheckBox     *cbD52                 = new TCheckBox();           // golden cConfiguration.h:244
    TCheckBox     *cbD53                 = new TCheckBox();           // golden cConfiguration.h:245
    TCheckBox     *cbD54                 = new TCheckBox();           // golden cConfiguration.h:247
    TCheckBox     *cbD55                 = new TCheckBox();           // golden cConfiguration.h:249
    TCheckBox     *cbD56                 = new TCheckBox();           // golden cConfiguration.h:250
    TCheckBox     *cbD57                 = new TCheckBox();           // golden cConfiguration.h:252
    TCheckBox     *cbD58                 = new TCheckBox();           // golden cConfiguration.h:256
    TCheckBox     *cbD59                 = new TCheckBox();           // golden cConfiguration.h:330
    TCheckBox     *cbD61                 = new TCheckBox();           // golden cConfiguration.h:954
    TCheckBox     *cbD62                 = new TCheckBox();           // golden cConfiguration.h:955
    TCheckBox     *cbD63                 = new TCheckBox();           // golden cConfiguration.h:1055
    TCheckBox     *cbD63_1               = new TCheckBox();           // golden cConfiguration.h:966
    TCheckBox     *cbD64                 = new TCheckBox();           // golden cConfiguration.h:959
    TCheckBox     *cbD65                 = new TCheckBox();           // golden cConfiguration.h:960
    TCheckBox     *cbD66                 = new TCheckBox();           // golden cConfiguration.h:962
    TCheckBox     *cbD67                 = new TCheckBox();           // golden cConfiguration.h:963
    TCheckBox     *cbD70                 = new TCheckBox();           // golden cConfiguration.h:968
    TCheckBox     *cbD72                 = new TCheckBox();           // golden cConfiguration.h:1148
    TCheckBox     *cbD73                 = new TCheckBox();           // golden cConfiguration.h:1240
    TCheckBox     *cbD74                 = new TCheckBox();           // golden cConfiguration.h:1633
    TCheckBox     *cbD75                 = new TCheckBox();           // golden cConfiguration.h:1591
    TCheckBox     *cbD78                 = new TCheckBox();           // golden cConfiguration.h:1614
    TCheckBox     *cbD79                 = new TCheckBox();           // golden cConfiguration.h:1615
    TCheckBox     *cbD80                 = new TCheckBox();           // golden cConfiguration.h:1617
    TCheckBox     *cbD81                 = new TCheckBox();           // golden cConfiguration.h:2136
    TCheckBox     *cbD82                 = new TCheckBox();           // golden cConfiguration.h:2183
    TCheckBox     *cbE30                 = new TCheckBox();           // golden cConfiguration.h:1734
    TCheckBox     *cbE30_1               = new TCheckBox();           // golden cConfiguration.h:2014
    TCheckBox     *cbE30_2               = new TCheckBox();           // golden cConfiguration.h:2031
    TCheckBox     *cbE31                 = new TCheckBox();           // golden cConfiguration.h:1803
    TCheckBox     *cbE31_1               = new TCheckBox();           // golden cConfiguration.h:1867
    TCheckBox     *cbE31_2               = new TCheckBox();           // golden cConfiguration.h:1929
    TCheckBox     *cbE32                 = new TCheckBox();           // golden cConfiguration.h:1773
    TCheckBox     *cbE32_1               = new TCheckBox();           // golden cConfiguration.h:1776
    TCheckBox     *cbE32_2               = new TCheckBox();           // golden cConfiguration.h:1991
    TCheckBox     *cbE33                 = new TCheckBox();           // golden cConfiguration.h:302
    TCheckBox     *cbE34                 = new TCheckBox();           // golden cConfiguration.h:303
    TCheckBox     *cbE35                 = new TCheckBox();           // golden cConfiguration.h:304
    TCheckBox     *cbE38                 = new TCheckBox();           // golden cConfiguration.h:307
    TCheckBox     *cbE39                 = new TCheckBox();           // golden cConfiguration.h:308
    TCheckBox     *cbE39_1               = new TCheckBox();           // golden cConfiguration.h:309
    TCheckBox     *cbE40                 = new TCheckBox();           // golden cConfiguration.h:310
    TCheckBox     *cbE41                 = new TCheckBox();           // golden cConfiguration.h:311
    TCheckBox     *cbE42                 = new TCheckBox();           // golden cConfiguration.h:312
    TCheckBox     *cbE43                 = new TCheckBox();           // golden cConfiguration.h:313
    TCheckBox     *cbE44                 = new TCheckBox();           // golden cConfiguration.h:314
    TCheckBox     *cbE45                 = new TCheckBox();           // golden cConfiguration.h:315
    TCheckBox     *cbE46                 = new TCheckBox();           // golden cConfiguration.h:316
    TCheckBox     *cbE47                 = new TCheckBox();           // golden cConfiguration.h:317
    TCheckBox     *cbE48                 = new TCheckBox();           // golden cConfiguration.h:318
    TCheckBox     *cbE49                 = new TCheckBox();           // golden cConfiguration.h:319
    TCheckBox     *cbE51                 = new TCheckBox();           // golden cConfiguration.h:577
    TCheckBox     *cbE53                 = new TCheckBox();           // golden cConfiguration.h:580
    TCheckBox     *cbE54                 = new TCheckBox();           // golden cConfiguration.h:581
    TCheckBox     *cbE55                 = new TCheckBox();           // golden cConfiguration.h:582
    TCheckBox     *cbE56                 = new TCheckBox();           // golden cConfiguration.h:583
    TCheckBox     *cbE57                 = new TCheckBox();           // golden cConfiguration.h:584
    TCheckBox     *cbE58                 = new TCheckBox();           // golden cConfiguration.h:587
    TCheckBox     *cbE59                 = new TCheckBox();           // golden cConfiguration.h:609
    TCheckBox     *cbE60                 = new TCheckBox();           // golden cConfiguration.h:824
    TCheckBox     *cbE61                 = new TCheckBox();           // golden cConfiguration.h:825
    TCheckBox     *cbE62                 = new TCheckBox();           // golden cConfiguration.h:869
    TCheckBox     *cbE63                 = new TCheckBox();           // golden cConfiguration.h:870
    TCheckBox     *cbE64                 = new TCheckBox();           // golden cConfiguration.h:920
    TCheckBox     *cbE68                 = new TCheckBox();           // golden cConfiguration.h:1314
    TCheckBox     *cbE85_Auto1           = new TCheckBox();           // golden cConfiguration.h:1649
    TCheckBox     *cbE85_Auto3           = new TCheckBox();           // golden cConfiguration.h:1648
    TCheckBox     *cbE85_Enable          = new TCheckBox();           // golden cConfiguration.h:1647
    TCheckBox     *cbE86                 = new TCheckBox();           // golden cConfiguration.h:2192
    TCheckBox     *cbE87                 = new TCheckBox();           // golden cConfiguration.h:2193
    TCheckBox     *cbE88                 = new TCheckBox();           // golden cConfiguration.h:2194
    TCheckBox     *cbE89                 = new TCheckBox();           // golden cConfiguration.h:2195
    TCheckBox     *cbE90                 = new TCheckBox();           // golden cConfiguration.h:2196
    TCheckBox     *cbF01                 = new TCheckBox();           // golden cConfiguration.h:276
    TCheckBox     *cbF03                 = new TCheckBox();           // golden cConfiguration.h:277
    TCheckBox     *cbF05                 = new TCheckBox();           // golden cConfiguration.h:275
    TCheckBox     *cbF06                 = new TCheckBox();           // golden cConfiguration.h:279
    TCheckBox     *cbF09                 = new TCheckBox();           // golden cConfiguration.h:281
    TCheckBox     *cbF11                 = new TCheckBox();           // golden cConfiguration.h:285
    TCheckBox     *cbF12                 = new TCheckBox();           // golden cConfiguration.h:286
    TCheckBox     *cbF14                 = new TCheckBox();           // golden cConfiguration.h:827
    TCheckBox     *cbF14_1               = new TCheckBox();           // golden cConfiguration.h:828
    TCheckBox     *cbF15                 = new TCheckBox();           // golden cConfiguration.h:295
    TCheckBox     *cbF16                 = new TCheckBox();           // golden cConfiguration.h:296
    TCheckBox     *cbF17                 = new TCheckBox();           // golden cConfiguration.h:457
    TCheckBox     *cbF18                 = new TCheckBox();           // golden cConfiguration.h:297
    TCheckBox     *cbF19                 = new TCheckBox();           // golden cConfiguration.h:298
    TCheckBox     *cbF20                 = new TCheckBox();           // golden cConfiguration.h:455
    TCheckBox     *cbF21                 = new TCheckBox();           // golden cConfiguration.h:544
    TCheckBox     *cbF22                 = new TCheckBox();           // golden cConfiguration.h:545
    TCheckBox     *cbF23                 = new TCheckBox();           // golden cConfiguration.h:550
    TCheckBox     *cbF24                 = new TCheckBox();           // golden cConfiguration.h:586
    TCheckBox     *cbF25                 = new TCheckBox();           // golden cConfiguration.h:894
    TCheckBox     *cbF28                 = new TCheckBox();           // golden cConfiguration.h:1147
    TCheckBox     *cbF29                 = new TCheckBox();           // golden cConfiguration.h:1236
    TCheckBox     *cbF30                 = new TCheckBox();           // golden cConfiguration.h:1293
    TCheckBox     *cbF31                 = new TCheckBox();           // golden cConfiguration.h:1499
    TCheckBox     *cbF32                 = new TCheckBox();           // golden cConfiguration.h:1669
    TCheckBox     *cbF33_Check2DHardware = new TCheckBox();           // golden cConfiguration.h:1661
    TCheckBox     *cbF34                 = new TCheckBox();           // golden cConfiguration.h:2134
    TCheckBox     *cbF35                 = new TCheckBox();           // golden cConfiguration.h:2135
    TCheckBox     *cbF36                 = new TCheckBox();           // golden cConfiguration.h:2223
    TCheckBox     *cbG01                 = new TCheckBox();           // golden cConfiguration.h:145
    TCheckBox     *cbG04                 = new TCheckBox();           // golden cConfiguration.h:146
    TCheckBox     *cbG05                 = new TCheckBox();           // golden cConfiguration.h:147
    TCheckBox     *cbG06                 = new TCheckBox();           // golden cConfiguration.h:148
    TCheckBox     *cbG07                 = new TCheckBox();           // golden cConfiguration.h:425
    TCheckBox     *cbG08                 = new TCheckBox();           // golden cConfiguration.h:427
    TCheckBox     *cbG09                 = new TCheckBox();           // golden cConfiguration.h:428
    TCheckBox     *cbG10                 = new TCheckBox();           // golden cConfiguration.h:444
    TCheckBox     *cbG11                 = new TCheckBox();           // golden cConfiguration.h:481
    TCheckBox     *cbG12                 = new TCheckBox();           // golden cConfiguration.h:571
    TCheckBox     *cbG13                 = new TCheckBox();           // golden cConfiguration.h:896
    TCheckBox     *cbG14                 = new TCheckBox();           // golden cConfiguration.h:897
    TCheckBox     *cbG15                 = new TCheckBox();           // golden cConfiguration.h:1060
    TCheckBox     *cbG17                 = new TCheckBox();           // golden cConfiguration.h:1178
    TCheckBox     *cbG17_1               = new TCheckBox();           // golden cConfiguration.h:1433
    TCheckBox     *cbG18_1               = new TCheckBox();           // golden cConfiguration.h:1438
    TCheckBox     *cbG18_2               = new TCheckBox();           // golden cConfiguration.h:1439
    TCheckBox     *cbG19                 = new TCheckBox();           // golden cConfiguration.h:1443
    TCheckBox     *cbG20                 = new TCheckBox();           // golden cConfiguration.h:1444
    TCheckBox     *cbG21                 = new TCheckBox();           // golden cConfiguration.h:1445
    TCheckBox     *cbG22                 = new TCheckBox();           // golden cConfiguration.h:1587
    TCheckBox     *cbG23                 = new TCheckBox();           // golden cConfiguration.h:2073
    TCheckBox     *cbG24                 = new TCheckBox();           // golden cConfiguration.h:2172
    TCheckBox     *cbL03                 = new TCheckBox();           // golden cConfiguration.h:439
    TCheckBox     *cbL07                 = new TCheckBox();           // golden cConfiguration.h:168
    TCheckBox     *cbL09                 = new TCheckBox();           // golden cConfiguration.h:1316
    TCheckBox     *cbL10                 = new TCheckBox();           // golden cConfiguration.h:1500
    TCheckBox     *cbL11_1               = new TCheckBox();           // golden cConfiguration.h:181
    TCheckBox     *cbL11_2               = new TCheckBox();           // golden cConfiguration.h:179
    TCheckBox     *cbL11_6               = new TCheckBox();           // golden cConfiguration.h:324
    TCheckBox     *cbL11_7               = new TCheckBox();           // golden cConfiguration.h:328
    TCheckBox     *cbL11_8               = new TCheckBox();           // golden cConfiguration.h:329
    TCheckBox     *cbL12                 = new TCheckBox();           // golden cConfiguration.h:174
    TCheckBox     *cbL13                 = new TCheckBox();           // golden cConfiguration.h:173
    TCheckBox     *cbL15                 = new TCheckBox();           // golden cConfiguration.h:184
    TCheckBox     *cbL17                 = new TCheckBox();           // golden cConfiguration.h:321
    TCheckBox     *cbL18                 = new TCheckBox();           // golden cConfiguration.h:394
    TCheckBox     *cbL19                 = new TCheckBox();           // golden cConfiguration.h:485
    TCheckBox     *cbL20                 = new TCheckBox();           // golden cConfiguration.h:570
    TCheckBox     *cbL21                 = new TCheckBox();           // golden cConfiguration.h:607
    TCheckBox     *cbL22                 = new TCheckBox();           // golden cConfiguration.h:871
    TCheckBox     *cbL24                 = new TCheckBox();           // golden cConfiguration.h:907
    TCheckBox     *cbL25                 = new TCheckBox();           // golden cConfiguration.h:1371
    TCheckBox     *cbL28                 = new TCheckBox();           // golden cConfiguration.h:1590
    TCheckBox     *cbL29                 = new TCheckBox();           // golden cConfiguration.h:1325
    TCheckBox     *cbL30                 = new TCheckBox();           // golden cConfiguration.h:1106
    TCheckBox     *cbL30_1               = new TCheckBox();           // golden cConfiguration.h:1107
    TCheckBox     *cbL32_1               = new TCheckBox();           // golden cConfiguration.h:1514
    TCheckBox     *cbL32_2               = new TCheckBox();           // golden cConfiguration.h:1519
    TCheckBox     *cbL33_1               = new TCheckBox();           // golden cConfiguration.h:1540
    TCheckBox     *cbL33_2               = new TCheckBox();           // golden cConfiguration.h:1541
    TCheckBox     *cbL34_1               = new TCheckBox();           // golden cConfiguration.h:1550
    TCheckBox     *cbL34_5               = new TCheckBox();           // golden cConfiguration.h:1553
    TCheckBox     *cbL35_1               = new TCheckBox();           // golden cConfiguration.h:1557
    TCheckBox     *cbL37                 = new TCheckBox();           // golden cConfiguration.h:1567
    TCheckBox     *cbL39_1               = new TCheckBox();           // golden cConfiguration.h:1570
    TCheckBox     *cbL39_2               = new TCheckBox();           // golden cConfiguration.h:1571
    TCheckBox     *cbL42                 = new TCheckBox();           // golden cConfiguration.h:1574
    TCheckBox     *cbL43                 = new TCheckBox();           // golden cConfiguration.h:2058
    TCheckBox     *cbL44                 = new TCheckBox();           // golden cConfiguration.h:1725
    TCheckBox     *cbL45                 = new TCheckBox();           // golden cConfiguration.h:1726
    TCheckBox     *cbL46                 = new TCheckBox();           // golden cConfiguration.h:1724
    TCheckBox     *cbM01                 = new TCheckBox();           // golden cConfiguration.h:227
    TCheckBox     *cbM01_01              = new TCheckBox();           // golden cConfiguration.h:220
    TCheckBox     *cbM01_02              = new TCheckBox();           // golden cConfiguration.h:221
    TCheckBox     *cbM01_03              = new TCheckBox();           // golden cConfiguration.h:222
    TCheckBox     *cbM01_04              = new TCheckBox();           // golden cConfiguration.h:223
    TCheckBox     *cbM01_05              = new TCheckBox();           // golden cConfiguration.h:224
    TCheckBox     *cbM01_06              = new TCheckBox();           // golden cConfiguration.h:225
    TCheckBox     *cbM01_07              = new TCheckBox();           // golden cConfiguration.h:226
    TCheckBox     *cbM01_08              = new TCheckBox();           // golden cConfiguration.h:251
    TCheckBox     *cbM01_09              = new TCheckBox();           // golden cConfiguration.h:269
    TCheckBox     *cbM01_10              = new TCheckBox();           // golden cConfiguration.h:445
    TCheckBox     *cbM01_12              = new TCheckBox();           // golden cConfiguration.h:447
    TCheckBox     *cbM01_13              = new TCheckBox();           // golden cConfiguration.h:448
    TCheckBox     *cbM01_14              = new TCheckBox();           // golden cConfiguration.h:823
    TCheckBox     *cbO01                 = new TCheckBox();           // golden cConfiguration.h:362
    TCheckBox     *cbO02                 = new TCheckBox();           // golden cConfiguration.h:360
    TCheckBox     *cbO05                 = new TCheckBox();           // golden cConfiguration.h:361
    TCheckBox     *cbO06                 = new TCheckBox();           // golden cConfiguration.h:372
    TCheckBox     *cbO07                 = new TCheckBox();           // golden cConfiguration.h:363
    TCheckBox     *cbO09                 = new TCheckBox();           // golden cConfiguration.h:388
    TCheckBox     *cbO10_EventLogSaver   = new TCheckBox();           // golden cConfiguration.h:389
    TCheckBox     *cbO11                 = new TCheckBox();           // golden cConfiguration.h:393
    TCheckBox     *cbO12_LifeTimeCount   = new TCheckBox();           // golden cConfiguration.h:430
    TCheckBox     *cbO13_LifeTimeCount   = new TCheckBox();           // golden cConfiguration.h:431
    TCheckBox     *cbO14_LifeTimeCount   = new TCheckBox();           // golden cConfiguration.h:435
    TCheckBox     *cbO18                 = new TCheckBox();           // golden cConfiguration.h:909
    TCheckBox     *cbO19_1               = new TCheckBox();           // golden cConfiguration.h:913
    TCheckBox     *cbO19_2               = new TCheckBox();           // golden cConfiguration.h:914
    TCheckBox     *cbO19_4               = new TCheckBox();           // golden cConfiguration.h:916
    TCheckBox     *cbO20                 = new TCheckBox();           // golden cConfiguration.h:1309
    TCheckBox     *cbO20_1               = new TCheckBox();           // golden cConfiguration.h:1626
    TCheckBox     *cbO21                 = new TCheckBox();           // golden cConfiguration.h:1642
    TCheckBox     *cbkE78                = new TCheckBox();           // golden cConfiguration.h:2170
    TCheckBox     *cdD13                 = new TCheckBox();           // golden cConfiguration.h:229
    TCheckBox     *cdE52                 = new TCheckBox();           // golden cConfiguration.h:576
    TCheckBox     *chkD18                = new TCheckBox();           // golden cConfiguration.h:1430
    TCheckBox     *chkE43_1              = new TCheckBox();           // golden cConfiguration.h:2054
    TCheckBox     *chkE65                = new TCheckBox();           // golden cConfiguration.h:921
    TCheckBox     *chkE66                = new TCheckBox();           // golden cConfiguration.h:1176
    TCheckBox     *chkE67                = new TCheckBox();           // golden cConfiguration.h:1177
    TCheckBox     *chkE69                = new TCheckBox();           // golden cConfiguration.h:1311
    TCheckBox     *chkE70                = new TCheckBox();           // golden cConfiguration.h:2047
    TCheckBox     *chkE71                = new TCheckBox();           // golden cConfiguration.h:2048
    TCheckBox     *chkE72                = new TCheckBox();           // golden cConfiguration.h:2049
    TCheckBox     *chkE73                = new TCheckBox();           // golden cConfiguration.h:2050
    TCheckBox     *chkE74                = new TCheckBox();           // golden cConfiguration.h:2052
    TCheckBox     *chkF27                = new TCheckBox();           // golden cConfiguration.h:1139
    TCheckBox     *chkG16                = new TCheckBox();           // golden cConfiguration.h:1061
    TCheckBox     *chkO06AlarmHistroy    = new TCheckBox();           // golden cConfiguration.h:377
    TCheckBox     *chkO06AlarmStatist    = new TCheckBox();           // golden cConfiguration.h:379
    TCheckBox     *chkO06Production      = new TCheckBox();           // golden cConfiguration.h:384
    TCheckBox     *chkO06TimePeriod      = new TCheckBox();           // golden cConfiguration.h:386
    TCheckBox     *chkO06UseNetDrive     = new TCheckBox();           // golden cConfiguration.h:381
    TCheckBox     *chkO15_2              = new TCheckBox();           // golden cConfiguration.h:507
    TCheckBox     *chkO15_3              = new TCheckBox();           // golden cConfiguration.h:1134
    TCheckBox     *chkO16                = new TCheckBox();           // golden cConfiguration.h:843
    TCheckBox     *chkO17                = new TCheckBox();           // golden cConfiguration.h:903
    TCheckBox     *chkO22                = new TCheckBox();           // golden cConfiguration.h:1643
    TCheckBox     *chkO23                = new TCheckBox();           // golden cConfiguration.h:1652
    TCheckBox     *chkO24                = new TCheckBox();           // golden cConfiguration.h:1674

    // golden TComboBox (9)
    TComboBox     *cbO06TimePeriod = new TComboBox();           // golden cConfiguration.h:387
    TComboBox     *cbbD47          = new TComboBox();           // golden cConfiguration.h:751
    TComboBox     *cbbO15_1        = new TComboBox();           // golden cConfiguration.h:505
    TComboBox     *coD22           = new TComboBox();           // golden cConfiguration.h:510
    TComboBox     *coD22_3         = new TComboBox();           // golden cConfiguration.h:1354
    TComboBox     *coD26           = new TComboBox();           // golden cConfiguration.h:1329
    TComboBox     *coD41           = new TComboBox();           // golden cConfiguration.h:1366
    TComboBox     *coO19_3         = new TComboBox();           // golden cConfiguration.h:915
    TComboBox     *coO19_5         = new TComboBox();           // golden cConfiguration.h:917

    // golden TEdit (187)
    TEdit         *EdF12                 = new TEdit();               // golden cConfiguration.h:287
    TEdit         *edD01                 = new TEdit();               // golden cConfiguration.h:117
    TEdit         *edD01DelayTime_Xilinx = new TEdit();               // golden cConfiguration.h:541
    TEdit         *edD01_Xilinx          = new TEdit();               // golden cConfiguration.h:540
    TEdit         *edD05                 = new TEdit();               // golden cConfiguration.h:611
    TEdit         *edD06                 = new TEdit();               // golden cConfiguration.h:1141
    TEdit         *edD14                 = new TEdit();               // golden cConfiguration.h:238
    TEdit         *edD21_Sec             = new TEdit();               // golden cConfiguration.h:126
    TEdit         *edD21_mm              = new TEdit();               // golden cConfiguration.h:125
    TEdit         *edD23                 = new TEdit();               // golden cConfiguration.h:353
    TEdit         *edD26                 = new TEdit();               // golden cConfiguration.h:129
    TEdit         *edD26_3               = new TEdit();               // golden cConfiguration.h:1613
    TEdit         *edD41                 = new TEdit();               // golden cConfiguration.h:1367
    TEdit         *edD41_2               = new TEdit();               // golden cConfiguration.h:1368
    TEdit         *edD44                 = new TEdit();               // golden cConfiguration.h:96
    TEdit         *edD44_Height          = new TEdit();               // golden cConfiguration.h:266
    TEdit         *edD46                 = new TEdit();               // golden cConfiguration.h:100
    TEdit         *edD47_3               = new TEdit();               // golden cConfiguration.h:749
    TEdit         *edD47_Count           = new TEdit();               // golden cConfiguration.h:747
    TEdit         *edD47_Time            = new TEdit();               // golden cConfiguration.h:748
    TEdit         *edD53                 = new TEdit();               // golden cConfiguration.h:246
    TEdit         *edD54                 = new TEdit();               // golden cConfiguration.h:248
    TEdit         *edD62                 = new TEdit();               // golden cConfiguration.h:961
    TEdit         *edD63                 = new TEdit();               // golden cConfiguration.h:958
    TEdit         *edD68                 = new TEdit();               // golden cConfiguration.h:964
    TEdit         *edE31_1_Au1X          = new TEdit();               // golden cConfiguration.h:1872
    TEdit         *edE31_1_Au1Y          = new TEdit();               // golden cConfiguration.h:1873
    TEdit         *edE31_1_Au2X          = new TEdit();               // golden cConfiguration.h:1877
    TEdit         *edE31_1_Au2Y          = new TEdit();               // golden cConfiguration.h:1878
    TEdit         *edE31_1_Au3X          = new TEdit();               // golden cConfiguration.h:1882
    TEdit         *edE31_1_Au3Y          = new TEdit();               // golden cConfiguration.h:1883
    TEdit         *edE31_1_Au4X          = new TEdit();               // golden cConfiguration.h:1903
    TEdit         *edE31_1_Au4Y          = new TEdit();               // golden cConfiguration.h:1902
    TEdit         *edE31_1_Au5X          = new TEdit();               // golden cConfiguration.h:1907
    TEdit         *edE31_1_Au5Y          = new TEdit();               // golden cConfiguration.h:1908
    TEdit         *edE31_1_Au6X          = new TEdit();               // golden cConfiguration.h:1912
    TEdit         *edE31_1_Au6Y          = new TEdit();               // golden cConfiguration.h:1913
    TEdit         *edE31_1_Fi1X          = new TEdit();               // golden cConfiguration.h:1897
    TEdit         *edE31_1_Fi1Y          = new TEdit();               // golden cConfiguration.h:1898
    TEdit         *edE31_1_Fi2X          = new TEdit();               // golden cConfiguration.h:1892
    TEdit         *edE31_1_Fi2Y          = new TEdit();               // golden cConfiguration.h:1893
    TEdit         *edE31_1_Fi3X          = new TEdit();               // golden cConfiguration.h:1887
    TEdit         *edE31_1_Fi3Y          = new TEdit();               // golden cConfiguration.h:1888
    TEdit         *edE31_1_Fi4X          = new TEdit();               // golden cConfiguration.h:1917
    TEdit         *edE31_1_Fi4Y          = new TEdit();               // golden cConfiguration.h:1918
    TEdit         *edE31_1_Fi5X          = new TEdit();               // golden cConfiguration.h:1922
    TEdit         *edE31_1_Fi5Y          = new TEdit();               // golden cConfiguration.h:1923
    TEdit         *edE31_1_Fi6X          = new TEdit();               // golden cConfiguration.h:1927
    TEdit         *edE31_1_Fi6Y          = new TEdit();               // golden cConfiguration.h:1928
    TEdit         *edE31_2_Au1X          = new TEdit();               // golden cConfiguration.h:1934
    TEdit         *edE31_2_Au1Y          = new TEdit();               // golden cConfiguration.h:1935
    TEdit         *edE31_2_Au2X          = new TEdit();               // golden cConfiguration.h:1939
    TEdit         *edE31_2_Au2Y          = new TEdit();               // golden cConfiguration.h:1940
    TEdit         *edE31_2_Au3X          = new TEdit();               // golden cConfiguration.h:1944
    TEdit         *edE31_2_Au3Y          = new TEdit();               // golden cConfiguration.h:1945
    TEdit         *edE31_2_Au4X          = new TEdit();               // golden cConfiguration.h:1965
    TEdit         *edE31_2_Au4Y          = new TEdit();               // golden cConfiguration.h:1964
    TEdit         *edE31_2_Au5X          = new TEdit();               // golden cConfiguration.h:1969
    TEdit         *edE31_2_Au5Y          = new TEdit();               // golden cConfiguration.h:1970
    TEdit         *edE31_2_Au6X          = new TEdit();               // golden cConfiguration.h:1974
    TEdit         *edE31_2_Au6Y          = new TEdit();               // golden cConfiguration.h:1975
    TEdit         *edE31_2_Fi1X          = new TEdit();               // golden cConfiguration.h:1959
    TEdit         *edE31_2_Fi1Y          = new TEdit();               // golden cConfiguration.h:1960
    TEdit         *edE31_2_Fi2X          = new TEdit();               // golden cConfiguration.h:1954
    TEdit         *edE31_2_Fi2Y          = new TEdit();               // golden cConfiguration.h:1955
    TEdit         *edE31_2_Fi3X          = new TEdit();               // golden cConfiguration.h:1949
    TEdit         *edE31_2_Fi3Y          = new TEdit();               // golden cConfiguration.h:1950
    TEdit         *edE31_2_Fi4X          = new TEdit();               // golden cConfiguration.h:1979
    TEdit         *edE31_2_Fi4Y          = new TEdit();               // golden cConfiguration.h:1980
    TEdit         *edE31_2_Fi5X          = new TEdit();               // golden cConfiguration.h:1984
    TEdit         *edE31_2_Fi5Y          = new TEdit();               // golden cConfiguration.h:1985
    TEdit         *edE31_2_Fi6X          = new TEdit();               // golden cConfiguration.h:1989
    TEdit         *edE31_2_Fi6Y          = new TEdit();               // golden cConfiguration.h:1990
    TEdit         *edE31_Au1X            = new TEdit();               // golden cConfiguration.h:1808
    TEdit         *edE31_Au1Y            = new TEdit();               // golden cConfiguration.h:1809
    TEdit         *edE31_Au2X            = new TEdit();               // golden cConfiguration.h:1813
    TEdit         *edE31_Au2Y            = new TEdit();               // golden cConfiguration.h:1814
    TEdit         *edE31_Au3X            = new TEdit();               // golden cConfiguration.h:1818
    TEdit         *edE31_Au3Y            = new TEdit();               // golden cConfiguration.h:1819
    TEdit         *edE31_Au4X            = new TEdit();               // golden cConfiguration.h:1839
    TEdit         *edE31_Au4Y            = new TEdit();               // golden cConfiguration.h:1838
    TEdit         *edE31_Au5X            = new TEdit();               // golden cConfiguration.h:1843
    TEdit         *edE31_Au5Y            = new TEdit();               // golden cConfiguration.h:1844
    TEdit         *edE31_Au6X            = new TEdit();               // golden cConfiguration.h:1848
    TEdit         *edE31_Au6Y            = new TEdit();               // golden cConfiguration.h:1849
    TEdit         *edE31_Fi1Y            = new TEdit();               // golden cConfiguration.h:1834
    TEdit         *edE31_Fi2X            = new TEdit();               // golden cConfiguration.h:1828
    TEdit         *edE31_Fi2Y            = new TEdit();               // golden cConfiguration.h:1829
    TEdit         *edE31_Fi3X            = new TEdit();               // golden cConfiguration.h:1823
    TEdit         *edE31_Fi3Y            = new TEdit();               // golden cConfiguration.h:1824
    TEdit         *edE31_Fi4X            = new TEdit();               // golden cConfiguration.h:1853
    TEdit         *edE31_Fi4Y            = new TEdit();               // golden cConfiguration.h:1854
    TEdit         *edE31_Fi5X            = new TEdit();               // golden cConfiguration.h:1858
    TEdit         *edE31_Fi5Y            = new TEdit();               // golden cConfiguration.h:1859
    TEdit         *edE31_Fi6X            = new TEdit();               // golden cConfiguration.h:1863
    TEdit         *edE31_Fi6Y            = new TEdit();               // golden cConfiguration.h:1864
    TEdit         *edE36                 = new TEdit();               // golden cConfiguration.h:305
    TEdit         *edE37                 = new TEdit();               // golden cConfiguration.h:306
    TEdit         *edE51                 = new TEdit();               // golden cConfiguration.h:578
    TEdit         *edE52                 = new TEdit();               // golden cConfiguration.h:579
    TEdit         *edE68                 = new TEdit();               // golden cConfiguration.h:1315
    TEdit         *edE73                 = new TEdit();               // golden cConfiguration.h:2051
    TEdit         *edF01                 = new TEdit();               // golden cConfiguration.h:282
    TEdit         *edF05                 = new TEdit();               // golden cConfiguration.h:278
    TEdit         *edF13_ADC             = new TEdit();               // golden cConfiguration.h:292
    TEdit         *edF13_Ini             = new TEdit();               // golden cConfiguration.h:293
    TEdit         *edF13_Jog             = new TEdit();               // golden cConfiguration.h:294
    TEdit         *edF14                 = new TEdit();               // golden cConfiguration.h:845
    TEdit         *edF14_1               = new TEdit();               // golden cConfiguration.h:848
    TEdit         *edF14_1_No            = new TEdit();               // golden cConfiguration.h:850
    TEdit         *edF14_No              = new TEdit();               // golden cConfiguration.h:847
    TEdit         *edF23                 = new TEdit();               // golden cConfiguration.h:551
    TEdit         *edF25                 = new TEdit();               // golden cConfiguration.h:895
    TEdit         *edF31_1               = new TEdit();               // golden cConfiguration.h:1493
    TEdit         *edF31_2               = new TEdit();               // golden cConfiguration.h:1494
    TEdit         *edF31_3               = new TEdit();               // golden cConfiguration.h:1495
    TEdit         *edF31_4               = new TEdit();               // golden cConfiguration.h:1496
    TEdit         *edF31_5               = new TEdit();               // golden cConfiguration.h:1497
    TEdit         *edF31_6               = new TEdit();               // golden cConfiguration.h:1498
    TEdit         *edF32                 = new TEdit();               // golden cConfiguration.h:1670
    TEdit         *edG14                 = new TEdit();               // golden cConfiguration.h:898
    TEdit         *edG18Auto1Count       = new TEdit();               // golden cConfiguration.h:1440
    TEdit         *edG18Auto2Count       = new TEdit();               // golden cConfiguration.h:1441
    TEdit         *edG18Auto3Count       = new TEdit();               // golden cConfiguration.h:1442
    TEdit         *edHeadCondition1      = new TEdit();               // golden cConfiguration.h:432
    TEdit         *edHeadCondition2      = new TEdit();               // golden cConfiguration.h:433
    TEdit         *edHeadCondition3      = new TEdit();               // golden cConfiguration.h:434
    TEdit         *edL03                 = new TEdit();               // golden cConfiguration.h:437
    TEdit         *edL04                 = new TEdit();               // golden cConfiguration.h:156
    TEdit         *edL05                 = new TEdit();               // golden cConfiguration.h:157
    TEdit         *edL08_Over            = new TEdit();               // golden cConfiguration.h:170
    TEdit         *edL08_Under           = new TEdit();               // golden cConfiguration.h:172
    TEdit         *edL11_1               = new TEdit();               // golden cConfiguration.h:180
    TEdit         *edL11_1_2             = new TEdit();               // golden cConfiguration.h:182
    TEdit         *edL11_2               = new TEdit();               // golden cConfiguration.h:178
    TEdit         *edL11_4               = new TEdit();               // golden cConfiguration.h:231
    TEdit         *edL11_6_Continuous    = new TEdit();               // golden cConfiguration.h:326
    TEdit         *edL11_6_Outside       = new TEdit();               // golden cConfiguration.h:325
    TEdit         *edL11_7_MaxSurge      = new TEdit();               // golden cConfiguration.h:327
    TEdit         *edL24                 = new TEdit();               // golden cConfiguration.h:908
    TEdit         *edL32_4               = new TEdit();               // golden cConfiguration.h:1528
    TEdit         *edL32_5               = new TEdit();               // golden cConfiguration.h:1529
    TEdit         *edL32_6               = new TEdit();               // golden cConfiguration.h:1530
    TEdit         *edL32_7_1             = new TEdit();               // golden cConfiguration.h:1531
    TEdit         *edL32_7_2             = new TEdit();               // golden cConfiguration.h:1532
    TEdit         *edL32_8               = new TEdit();               // golden cConfiguration.h:1533
    TEdit         *edL33_3               = new TEdit();               // golden cConfiguration.h:1539
    TEdit         *edL33_4               = new TEdit();               // golden cConfiguration.h:1542
    TEdit         *edL33_5               = new TEdit();               // golden cConfiguration.h:1543
    TEdit         *edL33_6               = new TEdit();               // golden cConfiguration.h:1544
    TEdit         *edL34_2               = new TEdit();               // golden cConfiguration.h:1549
    TEdit         *edL34_3               = new TEdit();               // golden cConfiguration.h:1551
    TEdit         *edL34_4               = new TEdit();               // golden cConfiguration.h:1552
    TEdit         *edL35_2               = new TEdit();               // golden cConfiguration.h:1556
    TEdit         *edL40                 = new TEdit();               // golden cConfiguration.h:1576
    TEdit         *edL41                 = new TEdit();               // golden cConfiguration.h:1573
    TEdit         *edO06_FilePath        = new TEdit();               // golden cConfiguration.h:373
    TEdit         *edO07                 = new TEdit();               // golden cConfiguration.h:364
    TEdit         *edtD04                = new TEdit();               // golden cConfiguration.h:1184
    TEdit         *edtD04_20mm           = new TEdit();               // golden cConfiguration.h:1188
    TEdit         *edtD04_30mm           = new TEdit();               // golden cConfiguration.h:1187
    TEdit         *edtD04_40mm           = new TEdit();               // golden cConfiguration.h:1185
    TEdit         *edtD04_60mm           = new TEdit();               // golden cConfiguration.h:1186
    TEdit         *edtD04_80mm           = new TEdit();               // golden cConfiguration.h:1632
    TEdit         *edtD17_3              = new TEdit();               // golden cConfiguration.h:1432
    TEdit         *edtD47_5              = new TEdit();               // golden cConfiguration.h:755
    TEdit         *edtD47_6              = new TEdit();               // golden cConfiguration.h:756
    TEdit         *edtL06                = new TEdit();               // golden cConfiguration.h:169
    TEdit         *edtL09_1              = new TEdit();               // golden cConfiguration.h:1721
    TEdit         *edtL09_2              = new TEdit();               // golden cConfiguration.h:1723
    TEdit         *edtL36_1              = new TEdit();               // golden cConfiguration.h:1565
    TEdit         *edtL36_2              = new TEdit();               // golden cConfiguration.h:1566
    TEdit         *edtL39_2              = new TEdit();               // golden cConfiguration.h:1572
    TEdit         *edtL42                = new TEdit();               // golden cConfiguration.h:1575
    TEdit         *edtL44                = new TEdit();               // golden cConfiguration.h:1727
    TEdit         *edtL45                = new TEdit();               // golden cConfiguration.h:1728
    TEdit         *edtO06AlarmHistroy    = new TEdit();               // golden cConfiguration.h:378
    TEdit         *edtO06AlarmStatist    = new TEdit();               // golden cConfiguration.h:380
    TEdit         *edtO06Production      = new TEdit();               // golden cConfiguration.h:385
    TEdit         *edtO06_Local          = new TEdit();               // golden cConfiguration.h:382
    TEdit         *edtO06_Remote         = new TEdit();               // golden cConfiguration.h:383
    TEdit         *edtO11                = new TEdit();               // golden cConfiguration.h:392
    TEdit         *edtO16                = new TEdit();               // golden cConfiguration.h:844
    TEdit         *edtO17_Count          = new TEdit();               // golden cConfiguration.h:904
    TEdit         *edtO17_Time           = new TEdit();               // golden cConfiguration.h:905
    TEdit         *edtO18                = new TEdit();               // golden cConfiguration.h:910
    TEdit         *edtO19_6              = new TEdit();               // golden cConfiguration.h:918

    // golden TGroupBox (3)
    TGroupBox     *gbE85                = new TGroupBox();           // golden cConfiguration.h:1646
    TGroupBox     *gbF13_RotateShtSpeed = new TGroupBox();           // golden cConfiguration.h:288
    TGroupBox     *gbL30                = new TGroupBox();           // golden cConfiguration.h:1105

    // golden TLabel (9)
    TLabel        *cbF20Label2 = new TLabel();              // golden cConfiguration.h:456
    TLabel        *labD41      = new TLabel();              // golden cConfiguration.h:1362
    TLabel        *labD41_1    = new TLabel();              // golden cConfiguration.h:1365
    TLabel        *labD41_2    = new TLabel();              // golden cConfiguration.h:1364
    TLabel        *labD41_3    = new TLabel();              // golden cConfiguration.h:1363
    TLabel        *labD62      = new TLabel();              // golden cConfiguration.h:945
    TLabel        *labD68_1    = new TLabel();              // golden cConfiguration.h:946
    TLabel        *labF12      = new TLabel();              // golden cConfiguration.h:284
    TLabel        *lblO17      = new TLabel();              // golden cConfiguration.h:906

    // golden TLabeledEdit (1)
    TLabeledEdit  *edtF23_1 = new TLabeledEdit();        // golden cConfiguration.h:1370

    // golden TPanel (2)
    TPanel        *pnlD41 = new TPanel();              // golden cConfiguration.h:1361
    TPanel        *pnlD63 = new TPanel();              // golden cConfiguration.h:956

    // golden TRadioGroup (7)
    TRadioGroup   *rgD17 = new TRadioGroup();         // golden cConfiguration.h:1431
    TRadioGroup   *rgD69 = new TRadioGroup();         // golden cConfiguration.h:965
    TRadioGroup   *rgD71 = new TRadioGroup();         // golden cConfiguration.h:1059
    TRadioGroup   *rgE50 = new TRadioGroup();         // golden cConfiguration.h:1295
    TRadioGroup   *rgF07 = new TRadioGroup();         // golden cConfiguration.h:280
    TRadioGroup   *rgF26 = new TRadioGroup();         // golden cConfiguration.h:1137
    TRadioGroup   *rgL10 = new TRadioGroup();         // golden cConfiguration.h:171

    // golden TTabSheet (1)
    TTabSheet     *tsM00 = new TTabSheet();           // golden cConfiguration.h:83
};

#endif // FORMS_FCONFIGURATION_H
