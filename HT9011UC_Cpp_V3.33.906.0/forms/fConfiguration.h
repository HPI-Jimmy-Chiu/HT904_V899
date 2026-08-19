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
//    edSetTempClick          golden :5962-5965 (4L)  -- body gated (WA-1)
//    edSetTempKeyDown        golden :5967-5971 (5L)  -- body gated (WA-1); Key/Shift params dropped, unread
//    edE31_Fi1XClick         golden :5983-5986 (4L)  -- body gated (WA-1)
//    edD25_60mmClick         golden :6440-6443 (4L)  -- body gated (WA-1)
//    edD60_56mmClick         golden :6580-6583 (4L)  -- body gated (WA-1)
//    edA32_1Click            golden :7611-7614 (4L)  -- body gated (WA-1)
//    edtC15Click             golden :7675-7678 (4L)  -- body gated (WA-1)
//    edL09_Sh1LClick         golden :7680-7683 (4L)  -- body gated (WA-1)
//    edN05_AmbTempClick      golden :7685-7688 (4L)  -- body gated (WA-1)
//    edN06_PortClick         golden :7690-7693 (4L)  -- body gated (WA-1)
//    edN06_PasswordClick     golden :7695-7698 (4L)  -- body gated (WA-1)
//    edtSetIPSCQtyClick      golden :7700-7703 (4L)  -- body gated (WA-1)
//    edN04_ModelClick        golden :7705-7708 (4L)  -- body gated (WA-1)
//    edA22_2Click            golden :6593-6601 (9L)  -- REAL edA22_3-nudge if-block; 1 line gated (WA-1)
//    edA22_3Click            golden :6603-6607 (5L)  -- REAL dCount calc goes quiet; 1 line gated (WA-1)
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
//      -- 0 compiled (non-#if 0) hits; the one surviving textual reference
//      (Public/HTEdit.cpp:302, `fQwertyKey==NULL`) is itself inside that
//      file's own #if 0 GATE (6) block, confirmed by reading the surrounding
//      lines. This wave's own recon (RECON_cConfiguration_displayside.md
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
//  Rather than edit the shared vclcompat header (outside this wave's write
//  boundary; ~15 other TUs already depend on vclcompat::TStringGrid's exact
//  current shape per that header's own note), this file follows the SAME
//  additive facade-only subclass idiom ALREADY established by forms/
//  fObserver.h's TfObserverGrid, forms/fShowBinSelect.h's
//  TfShowBinSelectGrid, forms/fStartCondition.h's TfStartConditionGrid,
//  forms/fContactCT.h's TfContactCTGrid: TfConfigurationGrid PUBLICLY
//  INHERITS vclcompat::TStringGrid (so it still converts for free to any
//  `vclcompat::TStringGrid*` parameter, should a future wave need that) and
//  ADDS Font (TFont*)/DefaultColWidth/FixedRows/FixedCols/Row as plain
//  facade-only data, plus a ColWidths[] auto-growing proxy (byte-for-byte
//  the same proxy shape as TfObserverGrid::ColWidthsProxy). NOT a GATE:
//  every one of these properties compiles and holds the exact value golden
//  assigns; only the RENDERING implied by a real Font/DefaultColWidth/
//  FixedRows/Row (i.e. actually drawing rows/columns/fonts sized
//  accordingly) is out of scope -- the same "no renderer this wave" posture
//  every other headless facade grid in this tree already carries.
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
#include <vector>                   // TfConfigurationGrid::colWidths_

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
    explicit TfConfigurationGrid(int initialColCount = 5, int initialRowCount = 5)
        : vclcompat::TStringGrid(initialColCount, initialRowCount),
          Font(new TFont()), DefaultColWidth(64), FixedRows(0), FixedCols(0), Row(0)
    {
        colWidths_.assign(initialColCount > 16 ? initialColCount : 16, DefaultColWidth);
    }
    ~TfConfigurationGrid() { delete Font; }

    TFont *Font;            // golden ->Font->Size (sbtReloadTrayClick/sbtReloadHPClick)
    int    DefaultColWidth; // golden ->DefaultColWidth
    int    FixedRows;       // golden ->FixedRows -- plain data, no renderer this wave
    int    FixedCols;       // golden ->FixedCols -- plain data, no renderer this wave
    int    Row;             // golden ->Row (btnAddTrayClick/btnAddHPClick "select the new row") -- plain data, no renderer this wave

    // golden ->ColWidths[i]= -- proxy so `grid->ColWidths[i]=w;` keeps
    // golden's property-assignment spelling. Auto-grows on write past
    // current size (same idiom as TfObserverGrid::ColWidthsProxy).
    struct ColWidthsProxy
    {
        TfConfigurationGrid *owner;
        int &operator[](int idx)
        {
            if (idx >= static_cast<int>(owner->colWidths_.size()))
                owner->colWidths_.resize(idx + 1, owner->DefaultColWidth);
            return owner->colWidths_[idx];
        }
    };
    ColWidthsProxy ColWidths{this};

private:
    std::vector<int> colWidths_;
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
    void edSetTempClick();
    void edSetTempKeyDown();
    void edE31_Fi1XClick();
    void edD25_60mmClick();
    void edD60_56mmClick();
    void edA32_1Click();
    void edtC15Click();
    void edL09_Sh1LClick();
    void edN05_AmbTempClick();
    void edN06_PortClick();
    void edN06_PasswordClick();
    void edtSetIPSCQtyClick();
    void edN04_ModelClick();
    void edA22_2Click();
    void edA22_3Click();
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
};

#endif // FORMS_FCONFIGURATION_H
