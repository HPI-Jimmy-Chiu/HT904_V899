// =============================================================================
//  forms/fObserver.h  --  non-VCL facade for golden's TfObserver (cObserver.h)
//
//  AI(W906-FW3-Observer-W1) 20260818: new file, FW-3 cObserver Wave 1.
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/cObserver.h (567 lines) +
//  cObserver.cpp (5,425 lines, cp950/Big5 -- decoded with `python3 -c
//  "open(...).read().decode('cp950')"`, 0 U+FFFD, this wave).
//
//  ROLE
//  ----
//  TfObserver is golden's "OEE Observer" dialog: Counter/TestCate/Scanner/
//  MDBQuery/Yield/TestInfo/EventLogTxt/Precautions/LotInfo tabs. This file
//  declares ONLY the facade surface Wave 1's translated methods dereference
//  (cObserver.cpp, this same wave) -- not TfObserver's full ~260-member
//  golden surface. Later cObserver waves grow this header additively, same
//  convention as forms/fMain.h.
//
//  THIS IS A SEPARATE, NEW CLASS -- NOT THE EXISTING SHIM
//  --------------------------------------------------------------------------
//  atester_shims.h/.cpp ALREADY declare a `class TfObserverShim` and a global
//  `TfObserverShim *fObserver` (golden cObserver.h's own `extern PACKAGE
//  TfObserver *fObserver;`, landed by an earlier wave for the ~7 members other
//  translated files needed). This header does NOT touch either -- no `fObserver`
//  global is defined here (not even `extern`); the shim-vs-facade swap is an
//  INTEGRATION decision the main loop makes later (same class of decision as
//  the two-TMyKitSuck headers -- see docs/KNOWLEDGE.md). Until that swap,
//  `class TfObserver` below has no live instance anywhere in the tree; it is
//  exercised only by tests/test_observer_core.cpp (this wave), which
//  constructs its own local instance.
//
//  THE FACADE CONTRACT (same rules as forms/fMain.h, summarised)
//  --------------------------------------------------------------------------
//  1. Methods that are GENUINE TRANSLATIONS this wave (real, faithful bodies
//     in cObserver.cpp) are `virtual` per the tree-wide convention (allows a
//     future MFC binder to override), EXCEPT CalculateStopTime/
//     UnCalculateStopTime -- see the DEVIATION note on those two below.
//  2. Data members are concrete storage, never accessors (same rationale as
//     fMain.h: BCB6 `__property` has no C++ equivalent, and call sites read
//     `fObserver->widget->Prop` directly).
//  3. Widget stand-ins: stock VCL controls (TComboBox/TListBox/TRadioGroup/
//     TPanel) reuse vclcompat/Controls.h directly, same as every other form.
//     TStringGrid/TTMyTray/TChart/TDateTimePicker need EXTRA surface golden
//     had (ColWidths, FixedRows, Rows[]->Clear(), Series[]/Title, Date/
//     DateTime) that vclcompat's existing shims don't carry (by design -- see
//     vclcompat/StringGrid.h and vclcompat/TrayCore.h's own SCOPE notes), so
//     this wave defines four small facade-only wrapper types below
//     (TfObserverGrid / TfObserverTray / TfObserverChart(+Series) /
//     TfObserverDateTimePicker) rather than editing those shared vclcompat
//     headers (out of this wave's write boundary; DESIGN NOTES below explain
//     each shape and why it composes the existing shim instead of forking it).
//
//  WAVE SCOPE (every golden method this header declares, golden line span,
//  ACTIVE / GATED / DEVIATION)
//  --------------------------------------------------------------------------
//    TfObserver()                        golden :137-346 (ctor)      ACTIVE
//    SetSiteYieldDiagram                 golden :771-837             ACTIVE
//    UpdateYieldChart                    golden :838-864             ACTIVE
//    SpeedButton1Click                   golden :865-872             ACTIVE
//    UpdateBin                           golden :873-904             ACTIVE
//    CalculateStopTime                   golden :1686-1707           ACTIVE, DEVIATION (public+static, was private+instance -- see below)
//    WriteContactKind                    golden :1301-1648           ACTIVE
//    UnCalculateStopTime                 golden :2237-2253           ACTIVE, DEVIATION (public+static, was private+instance -- see below)
//    BtnQueryClick                       golden :2421-2776           ACTIVE, 2 GATED sub-blocks (see GATE REGISTER 1, 2)
//    btReportClick                       golden :3186-3206           ACTIVE
//    rgContactCountKindsClick            golden :3219-3223           ACTIVE
//    rgContactCountHistoryClick          golden :3225-3229           ACTIVE
//    rgContactCountKindsFormClick        golden :3231-3235           ACTIVE
//    rgContactCountHistoryFormClick      golden :3237-3241            ACTIVE
//    lstEventLogClick                    golden :3757-3762           ACTIVE
//    cbbMonthChange                      golden :3763-3799           ACTIVE
//    GetEventLogText                     golden :3801-3971           ACTIVE
//    btnQueryEventLogTxtClick            golden :4423-4426           ACTIVE
//    DoProduction_Summary_Report(4 args) golden cObserver.h:552      GATED (body outside Wave-1 byte range; called from
//                                                                     BtnQueryClick's Production_Summary_Report branch --
//                                                                     documented no-op stub, see GATE REGISTER 3)
//    CountMTBF()                         golden cObserver.h:517      GATED (body outside Wave-1 byte range; called from
//                                                                     BtnQueryClick's AMKOR/Microchip MTBF tail --
//                                                                     documented no-op stub, see GATE REGISTER 3)
//
//  DEVIATION -- CalculateStopTime/UnCalculateStopTime made public+static
//  --------------------------------------------------------------------------
//  Golden declares both `private` and non-static (cObserver.h:486-487). Read
//  in full this wave: NEITHER touches `this` anywhere in its body (pure
//  functions of their one parameter + locals -- verified by reading golden
//  cObserver.cpp:1686-1707/:2237-2253 end to end). Keeping them
//  private+instance would force tests/test_observer_core.cpp to construct a
//  full TfObserver (which, for OTHER reasons -- see GetObserAuth/
//  CheckAndReadIniDataGeneral below -- needs real, if redirected, file I/O)
//  just to exercise two pure string/int conversions. `static` is the minimal,
//  documented, behaviour-preserving relaxation (same class of deliberate
//  access-level liberty this tree already takes with `virtual`, which golden
//  also never wrote). Call sites inside this class (none in Wave 1 -- both are
//  leaf functions with no internal callers in golden's own line ranges) are
//  unaffected either way.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (1) BtnQueryClick, golden :2668-2684 (the Alarm_Code_List per-row level/
//      silent/red-background loop). Needs `fMain->AlarmUnitMap` (grepped
//      forms/fMain.h in full this wave -- 0 hits, no such member) AND a
//      `fSecurity` facade (grepped forms/*.h -- no fSecurity.h exists in the
//      tree at all, only fAGV/fAOI/fCleaning/fFixAICCD/fHome/fLotInfo/fMain/
//      fNote/fOCR/fOffSet/fProductionInfo/fRotate/fSCKART/fSetup/
//      fShowMessage/fShuttleMove/fSortCT/fTrayForm). Two independent missing
//      dependencies, same "documented GAP, not silently dropped" idiom as
//      fMain.h's ATC_InterfaceForm/fBinSel gates. The grid columns this loop
//      would fill (Cells[4..6]) are simply left at whatever MyDBVProcess
//      populated (blank), which is the faithful "no security-level lookup
//      happened" default.
//  (2) BtnQueryClick, golden :2723/:2726/:2728/:2730/:2732/:2734 (6 calls:
//      1x MyDBVUnitEventCount + 5x MyDBVAxleEventCount). cMyDB.h:75 keeps
//      `class TChart;` a permanently OPAQUE forward decl ("TeeChart VCL
//      component -- NOT ported, W7-UI scope") and cMyDB.cpp itself ALREADY
//      gates both functions' BODIES for exactly that reason (cMyDB.h:112-113
//      "BODY GATED (no TChart port, see file head)"). This facade's Chart2 is
//      TfObserverChart* (this wave's own data-only stand-in, DESIGN NOTES
//      below) -- a deliberately DIFFERENT type from golden's opaque `TChart*`,
//      so passing it to either function cannot even bind at the type level,
//      not just "the body would no-op". Every OTHER Chart2 touch in this same
//      function (Visible, Series[0]->Clear(), Title->Text->Clear()/Add()) is
//      real and ACTIVE against TfObserverChart -- only the two golden library
//      calls that need the opaque TChart* are gated.
//  (3) DoProduction_Summary_Report / CountMTBF -- both are golden TfObserver
//      MEMBER functions (cObserver.h:552/:517) whose BODIES sit outside every
//      one of Wave 1's assigned golden byte ranges (grepped: neither name
//      appears inside cObserver.cpp:137-346/771-904/1301-1648/1686-1707/
//      2237-2253/2421-2776/3186-3242/3757-3971/4423-4426). BtnQueryClick
//      calls both (golden :2688-2691 and :2770), so this header declares them
//      with a documented no-op stub body (cObserver.cpp, this wave) -- same
//      "GAP stub, not a silent drop" idiom as forms/fMain.h's
//      SetMainRunStartMode.
//
//  DESIGN NOTES -- the four facade-only widget wrapper shapes
//  --------------------------------------------------------------------------
//  TfObserverGrid : public vclcompat::TStringGrid
//    golden TStringGrid ALSO exposes ->ColWidths[i]= (used ~30 times across
//    Wave 1's ctor/GetEventLogText/BtnQueryClick/btReportClick),
//    ->DefaultColWidth=, ->FixedRows=, ->Visible (strngrdMDBQuery only),
//    ->Rows[i]->Clear() (a whole-row wipe with no Cells-level equivalent)
//    and ->Repaint()/->Refresh() (StringGrid2/3, GDI-rendering-only, no-op
//    this wave -- see the "GDI methods not this wave" project rule).
//    AI(W906-VclGrid-1) 20260820: Visible/DefaultColWidth/FixedRows/
//    ColWidths[]/ClearRow(i) MOVED to the shared vclcompat::TStringGrid base
//    (vclcompat/StringGrid.h) as part of the 5-subclass TStringGrid-extension
//    consolidation wave -- this facade's own copies were a byte-for-byte
//    duplicate of forms/fConfiguration.h's TfConfigurationGrid, so they
//    collapsed home instead of risking a third fork. TfObserverGrid PUBLICLY
//    INHERITS vclcompat::TStringGrid (so it still converts for free to every
//    `TStringGrid*` cMyDB.h function -- MyDBVProcess/MyDBVProcessFilter/
//    MyDBVEventFreq/MyDBQTimeData all take `vclcompat::TStringGrid*`) and now
//    keeps ONLY its two GDI no-ops (Repaint()/Refresh(), StringGrid2/3 --
//    out of scope for the base, which carries no rendering surface at all)
//    plus a pass-through hydration ctor. See vclcompat/StringGrid.h's own
//    updated SCOPE banner for the base's full current member set and the
//    verified-inert reasoning behind dropping the ctor's old eager
//    ColWidths pre-sizing.
//
//  TfObserverTray : public vclcompat::TObject, composing vclcompat::TrayCore
//    golden TTMyTray (HTray.h/.cpp) already has a real, tested, framework-free
//    core: vclcompat/TrayCore.h's `TrayCore` (W7-C1), which implements
//    SetColorMap/SetCellNumber/SetCellColorIndex/SetYItem with the SAME
//    documented golden bugs golden's real HTray.cpp has (e.g. SetYItem,
//    ConvertIndexCells -- see TrayCore.h's own per-method "GOLDEN BUG, see
//    .cpp" tags). Wave 1's mtRow[]/mtCategorySum/.../mtCategoryTotal members
//    call exactly SetColorMap/SetCellNumber/SetCellColorIndex (never
//    CaculateTrayParameter/CellRect/ConvertIndexCells directly -- those three
//    are GDI/hit-test geometry, out of scope this wave per the project's
//    DrawCell*/DrawCenterLine exclusion), so composing TrayCore gives Wave 1
//    real, already-independently-verified behaviour for free -- reusing
//    tested infrastructure beats re-deriving it. TrayCore itself has no
//    Visible/Tag (those are golden TCustomControl/TComponent properties, one
//    layer up, that TrayCore's own SCOPE note explicitly keeps out of a
//    "state/geometry core"), so this wrapper adds them as plain facade
//    fields. `->Width=`/`->YItem=` (VCL property WRITES, golden ctor/
//    SetSiteYieldDiagram) route to Core.Width (plain field write -- TrayCore's
//    own note: direct field writes do NOT recompute geometry, only the
//    NEW-not-golden SetExtents() does, and no Wave-1 method ever reads
//    geometry back) and Core.SetYItem() (the real mutator, preserving its
//    documented golden bug) respectively.
//
//  TfObserverChart / TfObserverChartSeries : public vclcompat::TObject
//    Golden ChartYield/TempChart/Chart2 are all `TChart*` (TeeChart, golden
//    cObserver.h). Project-wide, `TChart` is a DELIBERATELY OPAQUE forward
//    decl (cMyDB.h:75, "NOT ported, W7-UI scope") -- so this facade cannot use
//    that name for a widget that needs REAL behaviour (ChartYield/
//    UpdateYieldChart's Series[]->Clear()/->AddY()/->Active ARE genuinely
//    ACTIVE this wave, not gated). TfObserverChart is this wave's own,
//    differently-named, minimal data-only stand-in: a growable vector of
//    TfObserverChartSeries (Title/Active/Points, where Points models golden's
//    ->AddY(value,label,color) triples), a Title.Text (reuses TStringList,
//    matching golden Chart->Title->Text being a TStrings*), and Visible. Being
//    a DIFFERENT type from the opaque `::TChart` is precisely why the two
//    library calls in GATE REGISTER (2) cannot bind -- documented there, not
//    a defect here. ChartYield needs 32 PRE-EXISTING series (golden's .dfm
//    wires 32 named TLineSeries SeriesAa..SeriesDh onto it at design time;
//    UpdateYieldChart's `for(iRow<MAX_SOCKET_TOTAL) ChartYield->Series[iRow]`
//    assumes they already exist) and Chart2 needs 1 (golden's .dfm's single
//    BarSeries2) -- both PRE-POPULATED in TfObserver's own ctor as PORT-ONLY
//    bootstrapping (a headless facade has no .dfm to instantiate named
//    child components from), clearly separated in cObserver.cpp from the
//    golden-line-by-line ctor translation. TempChart starts with ZERO
//    (golden's OWN ctor code dynamically AddSeries()-populates it,
//    genuinely translated, not bootstrapped).
//
//  TfObserverDateTimePicker : public vclcompat::TObject
//    golden TDateTimePicker exposes ->Date and ->DateTime (both TDateTime).
//    No vclcompat TDateTimePicker stand-in exists yet anywhere in the tree;
//    plain data holder, same shape as every other facade widget.
// =============================================================================
#ifndef FORMS_FOBSERVER_H
#define FORMS_FOBSERVER_H

#include "vclcompat/vcl_compat.h"     // AnsiString, TStringList, TDateTime -- brought to global scope
#include "vclcompat/Controls.h"       // TComboBox/TListBox/TRadioGroup/TPanel/TRadioButton (stock widgets, reused as-is)
#include "vclcompat/StringGrid.h"     // vclcompat::TStringGrid -- TfObserverGrid's base
#include "vclcompat/TrayCore.h"       // vclcompat::TrayCore -- TfObserverTray's composed core
// AI(W906-FW3-Observer-W2) 20260818: myTimer.h -- TQPF_Timer, golden TfObserver::
// tRecordInArmTimer (cObserver.h:549). Real, already-ported class (RecordInArmTime,
// this wave) -- QueryPerformanceCounter-backed, NOT a facade stand-in.
#include "myTimer.h"
// AI(W906-FW-ObsSwap) 20260818: <vector> for TfObserverMemoLotSummaryLines
// (moved here from atester_shims.h with the shim retirement, see below).
#include <vector>
// AI(W906-FW3-Observer-W2) 20260818: MachineType.h -- tcTotalCount (the
// dTempHistroy[tcTotalCount][60] member, UpdateTempChart). Not transitively
// pulled in by any header above; cObserver.cpp itself also includes this
// directly (Wave 1) but a HEADER member array needs it visible right here too.
#include "MachineType.h"

using vclcompat::TComboBox;
using vclcompat::TListBox;
using vclcompat::TRadioGroup;
using vclcompat::TRadioButton;
using vclcompat::TPanel;
using vclcompat::TStringGrid;

// golden Graphics.hpp TColor, at GLOBAL scope (matching cmydef.h:16 and every
// other TU-local shim in the tree -- see cObserver.cpp's own colour-shim
// banner). NOT vclcompat::TColor (TrayCore.h defines that name, but inside
// `namespace vclcompat`, so it would not resolve here unqualified).
// vclcompat/Controls.h deliberately keeps its own Color members plain `int`
// rather than pull in a TColor definition (see that header's own note); this
// facade's Chart series DOES need the golden property name for the
// ->AddY(value,label,color) triple, so it defines the one-line alias here.
#ifndef HT9045_W906_FOBSERVER_TCOLOR_SHIM
#define HT9045_W906_FOBSERVER_TCOLOR_SHIM
typedef int TColor;
#endif

// ===========================================================================
//  TfObserverGrid -- see DESIGN NOTES above.
// ===========================================================================
class TfObserverGrid : public vclcompat::TStringGrid
{
public:
    // golden .dfm design-time ColCount/RowCount, read from
    // tools/dfm2rc/ir_out/cObserver.dfm.ir.json this wave (see per-member call
    // sites in cObserver.cpp for the exact citation of which grid gets which
    // pair) -- NOT guessed: real values recorded at form-design time.
    //
    // AI(W906-VclGrid-1) 20260820: members already collapsed to the base
    // (this class's own SCOPE-comment above has the provenance) -- this
    // class only remains as a hydration ctor plus two GDI no-op overrides.
    explicit TfObserverGrid(int initialColCount = 5, int initialRowCount = 5)
        : vclcompat::TStringGrid(initialColCount, initialRowCount)
    {}

    // golden ->Repaint()/->Refresh() -- GDI repaint triggers (StringGrid2/3's
    // 4 rgContactCount*Click handlers). Out of scope this wave (DrawCell*/
    // DrawCenterLine family exclusion) -- no-op, matching that exclusion.
    void Repaint() {}
    void Refresh() {}
};

// ===========================================================================
//  TfObserverTray -- see DESIGN NOTES above.
// ===========================================================================
class TfObserverTray : public vclcompat::TObject
{
public:
    vclcompat::TrayCore Core;   // golden TTMyTray state/geometry core (vclcompat/TrayCore.h, HTray.h/.cpp)
    bool Visible;               // golden TCustomControl->Visible -- offline default false
    int  Tag;                   // golden TComponent->Tag

    TfObserverTray() : Visible(false), Tag(0) {}
    virtual ~TfObserverTray() {}
};

// ===========================================================================
//  TfObserverChart / TfObserverChartSeries -- see DESIGN NOTES above.
// ===========================================================================
class TfObserverChartSeries : public vclcompat::TObject
{
public:
    struct Point { double Value; AnsiString Text; TColor Color; };

    AnsiString         Title;    // golden TChartSeries->Title (TempChart's per-series set in ctor)
    bool               Active;   // golden TChartSeries->Active (UpdateYieldChart)
    std::vector<Point> Points;   // golden ->AddY(value,label,color) triples, in append order

    TfObserverChartSeries() : Active(false) {}
    virtual ~TfObserverChartSeries() {}

    void Clear() { Points.clear(); }
    void AddY(double value, const AnsiString &text, TColor color)
    {
        Point p; p.Value = value; p.Text = text; p.Color = color;
        Points.push_back(p);
    }
};

class TfObserverChart : public vclcompat::TObject
{
public:
    struct TitleHolder
    {
        TStringList *Text;   // golden Chart->Title->Text (a TStrings*) -- only ->Clear()/->Add() used
        TitleHolder() { Text = new TStringList(); }
        ~TitleHolder() { delete Text; }
    };

    bool        Visible;
    TitleHolder Title;

    TfObserverChart() : Visible(false) {}
    virtual ~TfObserverChart() { for (size_t i = 0; i < seriesList_.size(); ++i) delete seriesList_[i]; }

    // golden ->AddSeries(new TLineSeries(...)) -- this facade drops the VCL
    // owner-ctor-arg (no .dfm ownership tree offline); appends and returns
    // the new series so `Chart->Series[j]->Title=...;` right after (golden's
    // own idiom in the TempChart ctor loop) keeps working.
    TfObserverChartSeries *AddSeries(TfObserverChartSeries *s)
    {
        seriesList_.push_back(s);
        return s;
    }

    struct SeriesAccessor
    {
        TfObserverChart *owner;
        TfObserverChartSeries *operator[](int idx) const { return owner->seriesList_.at(static_cast<size_t>(idx)); }
    };
    SeriesAccessor Series{this};

    // golden ->Repaint()/->Refresh() -- GDI, no-op (see TfObserverGrid note).
    void Repaint() {}
    void Refresh() {}

private:
    std::vector<TfObserverChartSeries *> seriesList_;
};

// ===========================================================================
//  TfObserverDateTimePicker -- see DESIGN NOTES above.
// ===========================================================================
class TfObserverDateTimePicker : public vclcompat::TObject
{
public:
    TDateTime Date;       // golden TDateTimePicker->Date
    TDateTime DateTime;   // golden TDateTimePicker->DateTime
    // AI(W906-FW3-Observer-W2) 20260818: golden TDateTimePicker->Time (CountMTBF,
    // this wave, DateTimePicker2/4). Real VCL TDateTimePicker holds ONE TDateTime
    // value and Date/Time/DateTime are three views of it that differ only by
    // which part a given ->Kind (dtDate/dtTime) exposes; this headless facade has
    // no Kind and no single backing value, so (matching this class's own existing
    // Date/DateTime split, Wave 1) Time is a THIRD independent stored field, not
    // a computed alias. Nothing this wave's translated code WRITES any of the
    // three -- all three are populated from outside (the not-yet-built UI layer,
    // or a test) -- so three independent fields cannot desync a call path that
    // never cross-reads them.
    TDateTime Time;       // golden TDateTimePicker->Time

    TfObserverDateTimePicker() {}
    virtual ~TfObserverDateTimePicker() {}
};

// ===========================================================================
//  W906Obs2_InstanceRegistrar -- PORT-ONLY, NOT a golden type.
//
//  AI(W906-FW3-Observer-W2) 20260818: golden's file-scope free functions
//  IniRecordMonitoringIndexCycleTime/RecordIndexAirOnTime1/RecordIndexAirOnTime2
//  (golden cObserver.h:563-565) are NOT TfObserver members -- they reach the
//  single live form through golden's own module-level `fObserver` global
//  (e.g. `fObserver->iIndexCycleTimeCount=0;`). This header deliberately does
//  NOT redeclare that global here (Wave 1's own "integration-pending" note,
//  above: the live `fObserver` is already `TfObserverShim*`, atester_shims.h --
//  a second, differently-typed `fObserver` would collide exactly like the
//  two-TMyKitSuck-headers trap). Those 3 free functions still need SOME way to
//  reach the one constructed `TfObserver`, and Wave 1's own ctor BODY
//  (cObserver.cpp) is existing, untouched content this wave may not edit
//  (append-only rule) -- so it cannot simply assign a new TU-local pointer
//  itself. This tiny registrar's constructor does that assignment instead, as
//  an ordinary member (see its use below, declared LAST so `this` is fully
//  built when it runs) -- a normal RAII side effect, not a language trick.
//  Definition (the TU-local pointer + the 3 consumers): cObserver.cpp, this
//  wave.
//
//  SAFETY -- the registered pointer is NEVER cleared on destruction (the
//  destructor body is ALSO existing/untouched Wave 1 content). In real
//  production this matches golden exactly (one TfObserver, created once, torn
//  down only at process exit -- golden's own `fObserver` global has the
//  identical lifetime assumption). In a test that constructs more than one
//  TfObserver, or that destroys one before calling
//  IniRecordMonitoringIndexCycleTime/RecordIndexAirOnTime1/RecordIndexAirOnTime2,
//  the pointer would dangle -- tests/test_observer_core.cpp (this wave) is
//  written to only call those 3 functions while its one `TfObserver` instance
//  is still in scope, precisely to avoid that.
// ===========================================================================
class TfObserver;

// ============================================================================
// AI(W906-FW-ObsSwap) 20260818: the three memo stand-ins MOVED VERBATIM from
// atester_shims.h (:342/:357-374 there) as part of retiring TfObserverShim --
// the live `fObserver` global is now backed by TfObserver (this facade), and
// the shim consumers (SCK_ART_Remainder's SaveTestSummarySECS whole-list
// assign, the Memo1 single-element peek) keep compiling against these exact
// names and shapes. Full provenance comments preserved from the shim header.
// ============================================================================
struct TfObserverMemoLines0 { AnsiString Strings0; };      // golden TMemo*->Lines->Strings[0] (only index used)

// golden cObserver.h:339 `TMyMemo *memoLotSummary;` -- SckArtRem_SaveTestSummarySECS
// does `fObserver->memoLotSummary->Lines=sList;` (golden SCK_ART.cpp:1965, a
// WHOLE-LIST assignment). Golden's TStrings::operator=(TPersistent*) COPIES --
// proven by golden itself calling `sList->Clear(); delete sList;` right after
// (SCK_ART.cpp:1967-1968) -- so this is a vector-backed COPY target.
struct TfObserverMemoLotSummaryLines
{
    std::vector<AnsiString> Strings;             // last-assigned COPY of golden Lines's content
    TfObserverMemoLotSummaryLines &operator=(TStringList *src)
    {
        Strings.clear();
        if(src!=0)
        {
            for(int i=0; i<src->Count; i++)
                Strings.push_back(src->Strings[i]);
        }
        return *this;
    }
};
struct TfObserverMemoLotSummary
{
    TfObserverMemoLotSummaryLines Lines;         // golden TMyMemo->Lines (TStrings*) -- whole-list-assign shape only
};
class W906Obs2_InstanceRegistrar
{
public:
    explicit W906Obs2_InstanceRegistrar(TfObserver *self);
};

// ===========================================================================
//  TfObserver -- non-VCL facade (golden cObserver.h, TfObserver:public TForm)
// ===========================================================================
class TfObserver
{
public:
    TfObserver();
    virtual ~TfObserver();

    // -- ctor-populated widget/data members Wave 1's methods dereference ----
    //    (golden cObserver.h line cited on each; grid/tray/chart/datetime
    //    members are allocated in the ctor's PORT-ONLY bootstrap section,
    //    then configured by the golden-translated ctor body -- see
    //    cObserver.cpp)
    TfObserverTray *mtRowA;              // golden cObserver.h:97  (TTMyTray*)
    TfObserverTray *mtRowB;              // golden cObserver.h:99  (TTMyTray*)
    TfObserverTray *mtRowC;              // golden cObserver.h:98  (TTMyTray*)
    TfObserverTray *mtRowD;              // golden cObserver.h:100 (TTMyTray*)
    TfObserverTray *mtCategorySum;       // golden cObserver.h:112 (TTMyTray*)
    TfObserverTray *mtRowName;           // golden cObserver.h:111 (TTMyTray*)
    TfObserverTray *mtNo;                // golden cObserver.h:130 (TTMyTray*)
    TfObserverTray *mtTotalName;         // golden cObserver.h:132 (TTMyTray*)
    TfObserverTray *myCategoryName;      // golden cObserver.h:113 (TTMyTray*)
    TfObserverTray *mtChName;            // golden cObserver.h:131 (TTMyTray*)
    TfObserverTray *mtDutName;           // golden cObserver.h:114 (TTMyTray*)
    TfObserverTray *mtArmName;           // golden cObserver.h:115 (TTMyTray*)
    TfObserverTray *mtCategoryNo;        // golden cObserver.h:121 (TTMyTray*)
    TfObserverTray *mtHeadTotal;         // golden cObserver.h:116 (TTMyTray*)
    TfObserverTray *mtSockTotal;         // golden cObserver.h:117 (TTMyTray*)
    TfObserverTray *mtPassHead;          // golden cObserver.h:118 (TTMyTray*)
    TfObserverTray *mtPassSocket;        // golden cObserver.h:119 (TTMyTray*)
    TfObserverTray *mtIfError;           // golden cObserver.h:120 (TTMyTray*)
    TfObserverTray *mtTotal;             // golden cObserver.h:134 (TTMyTray*)
    TfObserverTray *mtCategoryTotal;     // golden cObserver.h:133 (TTMyTray*)

    TfObserverGrid *StringGrid2;                 // golden cObserver.h:104 (TStringGrid*)
    TfObserverGrid *StringGrid3;                 // golden cObserver.h:108 (TStringGrid*)
    TfObserverGrid *StringGrid5;                 // golden cObserver.h:135 (TStringGrid*)
    TfObserverGrid *TimeInfoGrid;                // golden cObserver.h:55  (TStringGrid*)
    TfObserverGrid *strngrdTestTime;              // golden cObserver.h:292 (TStringGrid*)
    TfObserverGrid *sgTimeData;                   // golden cObserver.h:142 (TStringGrid*)
    TfObserverGrid *sg_ListTimeReceiveInfoGrid;   // golden cObserver.h:188 (TStringGrid*)
    TfObserverGrid *strngrdJamLog;                // golden cObserver.h:300 (TStringGrid*)
    TfObserverGrid *strngrdIndeAirOn1;            // golden cObserver.h:305 (TStringGrid*)
    TfObserverGrid *strngrdIndeAirOn2;            // golden cObserver.h:304 (TStringGrid*)
    TfObserverGrid *strngrdEventLog;              // golden cObserver.h:176 (TStringGrid*)
    TfObserverGrid *strngrdMDBQuery;              // golden cObserver.h:173 (TStringGrid*)

    TPanel *labMachineID;    // golden cObserver.h:378 (TPanel*)
    TPanel *labSerialNo;     // golden cObserver.h:375 (TPanel*)
    TPanel *pnlTotalCount;   // golden cObserver.h:172 (TPanel*)
    TPanel *lbltTotalLoader; // golden cObserver.h:162 (TPanel*)

    TComboBox *cbbTempChart;      // golden cObserver.h:413 (TComboBox*)
    TComboBox *cbDisplayData;     // golden cObserver.h:163 (TComboBox*)
    TComboBox *cbbEventLogYear;   // golden cObserver.h:178 (TComboBox*)
    TComboBox *cbbMonth;          // golden cObserver.h:181 (TComboBox*)
    TComboBox *cbbFilter;         // golden cObserver.h:185 (TComboBox*)

    TListBox *lstEventLog;        // golden cObserver.h:182 (TListBox*)

    TRadioGroup *rgContactCountKinds;         // golden cObserver.h:136 (TRadioGroup*)
    TRadioGroup *rgContactCountKindsForm;     // golden cObserver.h:137 (TRadioGroup*)
    TRadioGroup *rgContactCountHistory;       // golden cObserver.h:138 (TRadioGroup*)
    TRadioGroup *rgContactCountHistoryForm;   // golden cObserver.h:139 (TRadioGroup*)

    TfObserverChart *ChartYield;   // golden cObserver.h:51  (TChart*) -- 32 series pre-populated, PORT-ONLY (see .cpp)
    TfObserverChart *TempChart;    // golden cObserver.h:412 (TChart*) -- 0 series; golden ctor populates tcTotalCount of them
    TfObserverChart *Chart2;       // golden cObserver.h:174 (TChart*) -- 1 series pre-populated, PORT-ONLY (see .cpp)

    TfObserverDateTimePicker *DateTimePicker1;   // golden cObserver.h:164
    TfObserverDateTimePicker *DateTimePicker2;   // golden cObserver.h:165
    TfObserverDateTimePicker *DateTimePicker3;   // golden cObserver.h:166
    TfObserverDateTimePicker *DateTimePicker4;   // golden cObserver.h:167

    int iTotoalTestTime;   // golden cObserver.h:504 (int)  -- ctor sets 0
    int iShowYieldChart;   // golden cObserver.h:509 (int)  -- ctor sets 0

    // -- Wave 1 translated methods (bodies: cObserver.cpp, this wave) -------
    virtual void SetSiteYieldDiagram();                              // golden :771-837
    virtual void UpdateYieldChart();                                  // golden :838-864
    virtual void SpeedButton1Click(void *Sender);                     // golden :865-872
    virtual void UpdateBin();                                         // golden :873-904
    virtual void WriteContactKind();                                  // golden :1301-1648
    virtual void BtnQueryClick(void *Sender);                         // golden :2421-2776
    virtual void btReportClick(void *Sender);                         // golden :3186-3206
    virtual void rgContactCountKindsClick(void *Sender);              // golden :3219-3223
    virtual void rgContactCountHistoryClick(void *Sender);            // golden :3225-3229
    virtual void rgContactCountKindsFormClick(void *Sender);          // golden :3231-3235
    virtual void rgContactCountHistoryFormClick(void *Sender);        // golden :3237-3241
    virtual void lstEventLogClick(void *Sender);                      // golden :3757-3762
    virtual void cbbMonthChange(void *Sender);                        // golden :3763-3799
    virtual void GetEventLogText();                                   // golden :3801-3971
    virtual void btnQueryEventLogTxtClick(void *Sender);              // golden :4423-4426

    // -- DEVIATION: public+static, golden private+instance (see banner) -----
    static AnsiString CalculateStopTime(int Sec);          // golden :1686-1707
    static int        UnCalculateStopTime(AnsiString Time); // golden :2237-2253

    // -- PORT-ONLY testability seam (NOT golden methods) ---------------------
    // BtnQueryClick's WhereQuery/asQuery are local variables built, then
    // immediately handed to MyDBVProcess/MyDBVEventFreq -- both of which call
    // `sqlite3_get_table(dbReadOnly, ...)` UNCONDITIONALLY (cMyDB.cpp:1584),
    // with no NULL-handle guard. Calling BtnQueryClick for real from an
    // isolated unit test would therefore need a real, opened SQLite handle
    // (MyDBOpenDB(), which binds the production asDBPath) just to avoid a
    // NULL-dereference crash -- out of proportion, and out of this wave's
    // "don't mock cMyDB's behaviour, but don't need to RUN it either" brief.
    // These two static helpers extract the STRING ASSEMBLY BtnQueryClick does
    // (golden :2551-2556 and :2568-2569) into pure, no-DB-access functions
    // with IDENTICAL output -- BtnQueryClick (cObserver.cpp) calls them
    // internally instead of inlining the same sprintf, so this is a
    // zero-behaviour-change extraction, not new logic. Lets
    // tests/test_observer_core.cpp assert on exactly what SQL text the
    // Alarm_History eQueryType branch builds.
    static AnsiString BuildWhereQuery_OccurDateTimeRange(TDateTime d1, TDateTime t1, TDateTime d2, TDateTime t2); // golden :2551-2556
    static AnsiString BuildQuery_AlarmHistory(AnsiString WhereQuery);                                             // golden :2568-2569

    // -- GATE REGISTER (3): documented no-op stubs, bodies outside Wave 1 ---
    virtual void DoProduction_Summary_Report(AnsiString asStartData, AnsiString asStartTime,
                                              AnsiString asEndData, AnsiString asEndTime); // golden cObserver.h:552
    virtual void CountMTBF();                                                              // golden cObserver.h:517

    // NOTE: no `mtRow[MAX_SOCKET_ROW]` member here -- golden declares it as a
    // FILE-SCOPE global (cObserver.cpp:54 `TTMyTray *mtRow[MAX_SOCKET_ROW];`),
    // not a TfObserver class member, and this port keeps that shape (see
    // cObserver.cpp's own file-scope data section).

    // =========================================================================
    //  -- FW3-Obs2 ADD -- FW-3 cObserver Wave 2: OEE/statistics subset
    // =========================================================================
    //  AI(W906-FW3-Observer-W2) 20260818: additive-only continuation of Wave 1's
    //  facade (same file, same class -- Wave 1's own members/methods above are
    //  UNCHANGED). Golden ref unchanged: HT9011UC_Code_V3.33.906.0_20260618/
    //  cObserver.h + cObserver.cpp (see cObserver.cpp's own Wave 2 file-tail
    //  banner for the ABSENCE-CLAIM commands and U+FFFD count, not repeated here
    //  per Wave 1's own "don't duplicate across the two files" convention).
    //
    //  WAVE SCOPE (golden line span, ACTIVE / GATED / DEVIATION)
    //  -------------------------------------------------------------------------
    //    GetMachineData                      golden :755-767             ACTIVE
    //    Timer1Timer                         golden :708-753             ACTIVE, Close() -> facade no-op (see below)
    //    UpdateTempChart                     golden :905-943             ACTIVE
    //    cbbTempChartChange                  golden :945-948             ACTIVE
    //    ProcessRunInfo                      golden :2254-2331           ACTIVE
    //    RecordIndexTime                     golden :2814-2903           ACTIVE
    //    AddTimeData                         golden :2905-2966           ACTIVE
    //    RecordInArmTime                     golden :2968-2999           ACTIVE
    //    WriteCategoryData                   golden :3274-3547           ACTIVE
    //    CountMTBF                           golden :3683-3752           ACTIVE (Wave 1's no-op stub retired --
    //                                                                     SAME signature, declared already above,
    //                                                                     no header change; body only, cObserver.cpp)
    //    GetTimeDataText                     golden :4795-4839           ACTIVE
    //    RecordIndexCycle                    golden :4846-4887           ACTIVE
    //
    //    (free functions, declared at file scope below the class, matching
    //    golden cObserver.h:560-565's own placement)
    //    RecordStartTestTime()               golden :2136-2147           ACTIVE
    //    RecordEndTestTime(int)              golden :2150-2235           ACTIVE, 3 GATED callees (GATE REGISTER W2-1/2/3)
    //    RecordReceiveTestTime()             golden :4755-4764           ACTIVE (recon's :4755-4794 span also covers
    //                                                                     the NEXT function, pgcMessageChange -- NOT
    //                                                                     one of this wave's named targets, NOT
    //                                                                     translated; see cObserver.cpp for the
    //                                                                     verbatim golden text proving the boundary)
    //    IniRecordMonitoringIndexCycleTime() golden :1836-1844           ACTIVE
    //    RecordIndexAirOnTime1()             golden :5331-5343           ACTIVE
    //    RecordIndexAirOnTime2()             golden :5346-5358           ACTIVE
    //
    //  GATE REGISTER (W2)
    //  -------------------------------------------------------------------------
    //  (W2-1) RecordEndTestTime, golden :2176 `RecordMonitoringIndexCycleTime_New();`.
    //      Not declared ANYWHERE in the golden tree outside this one call site
    //      (grepped the cp950-decoded golden .cpp in full this wave -- 0 hits for
    //      a definition) and not one of this wave's named targets either way.
    //      TU-local no-op stub in cObserver.cpp, GATE-documented there.
    //  (W2-2) RecordEndTestTime, golden :2182 `RecordMonitoringIndexCycleTime();`.
    //      This one DOES have a real golden body (cObserver.cpp:1709-1751), but
    //      it is explicitly excluded from this wave's scope (task brief: "彈訊息" --
    //      it opens a message box on the outlier-count branch). Same TU-local
    //      no-op stub treatment, NOT a translation of the real body.
    //  (W2-3) RecordEndTestTime, golden :2168 `RecordTimeInfo();`. RecordTimeInfo's
    //      real body (golden :1846-2134, ~290 lines) is explicitly excluded this
    //      wave (task brief: "290 行未 recon 完"). Same TU-local no-op stub
    //      treatment.
    //  (W2-4) RecordEndTestTime, golden :2149/:2171 `extern int
    //      SendTestResultToHttp(); ... SendTestResultToHttp();`. Golden itself
    //      only ever forward-declares this LOCALLY (its real body lives in an
    //      unported networking/MES module) -- the SAME gap 3 sibling translators
    //      already solved (atester_32Site.cpp:394/397, aTester_Front.cpp:3264,
    //      aTester_Rear.cpp:3148): TU-local stub returning 1 (== upload OK) +
    //      `#define` shadow, so the call site keeps golden's own spelling.
    //  (W2-5) RecordEndTestTime, golden :2228 `fMain->slTestLog->AddText(str);`
    //      (inside the `IniConfig.bN28_SCK_OEE` / JSCK-OEE branch). `slTestLog`
    //      is NOT a forms/fMain.h facade member -- confirmed already gated for
    //      the SAME reason at cprod.cpp:2936-2939 (`fMain->slTestLog` inside a
    //      documented `#if 0` block, "missing facade member"), so this is the
    //      SAME pre-existing gap, not a new one, and out of this wave's write
    //      boundary (forms/fMain.h is not one of the 3 files this wave may
    //      touch). Everything ELSE in that branch (List1/List2 assembly,
    //      StringReplace) is real and ACTIVE; only this one trailing call is
    //      `#if 0`-gated in cObserver.cpp, with a pointer back to cprod.cpp's
    //      existing note.
    //
    //  DEVIATION -- Timer1Timer's Close()
    //  -------------------------------------------------------------------------
    //  golden :748 `Close();` (a TForm method that closes/hides the dialog after
    //  2 consecutive stale-Yield-Chart ticks). No TForm base here (same posture
    //  as every other translated form facade); modeled as a virtual no-op method
    //  below, matching Wave 1's own "GDI/window methods -> no-op" convention
    //  (TfObserverGrid::Repaint/Refresh). The COUNTING logic that decides WHEN to
    //  call it (iShowYieldChart/iCT/iSystemSec/iSystemMin state machine) is
    //  translated in full and is genuinely observable (iShowYieldChart itself is
    //  a Wave 1 member); only the actual window-close side effect is dropped.
    //
    //  NEW FACADE MEMBERS this wave's methods dereference (golden line cited)
    //  -------------------------------------------------------------------------
    //  ALL given in-class default member initializers (NSDMI): Wave 1's own
    //  ctor BODY (cObserver.cpp) is existing, untouched content this wave may
    //  not edit or move a line of (append-only rule) -- these new members are
    //  never listed in TfObserver()'s member-init-list either, so each one is
    //  constructed from ITS OWN in-class default before the ctor body runs,
    //  with NO change to that body. Same reasoning on the destructor side:
    //  Wave 1's ~TfObserver() body is also untouched, so pointer members added
    //  here are intentionally never `delete`d there -- a process-lifetime leak
    //  identical in kind (not new in kind) to the one already accepted for
    //  TfObserverChartSeries via TfObserverChart's OWN destructor loop, just
    //  without that loop's cleanup; acceptable because every consumer (real
    //  future UI wiring, and this wave's own tests) constructs at most one
    //  TfObserver per process lifetime.
    bool bShow = false;                            // golden cObserver.h:490 (bool)

    int RowNo = 0;                                 // golden cObserver.h:482 (private int; WriteCategoryData sets/reads
                                                    // it) -- promoted to public per this facade's established
                                                    // "no BCB6 private/__published split" convention (Wave 1 banner)

    TRadioGroup  *rgRowNo = new TRadioGroup();             // golden cObserver.h:129 -- WriteCategoryData
    TRadioButton *rbHeadNumber = new TRadioButton();       // golden cObserver.h:125
    TRadioButton *rbSocketNumber = new TRadioButton();     // golden cObserver.h:126
    TRadioButton *rbHeadPercent = new TRadioButton();      // golden cObserver.h:127
    TRadioButton *rbSocketPercent = new TRadioButton();    // golden cObserver.h:128

    TPanel *labPowerOnTime = new TPanel();          // golden cObserver.h:356 -- GetMachineData
    TPanel *labRunningTime = new TPanel();          // golden cObserver.h:357
    TPanel *labProductTime = new TPanel();          // golden cObserver.h:358
    TPanel *labLoadingCount = new TPanel();         // golden cObserver.h:359
    TPanel *labMUBA = new TPanel();                 // golden cObserver.h:360 -- ProcessRunInfo
    TPanel *labMTBA = new TPanel();                 // golden cObserver.h:361
    TPanel *labMTBF = new TPanel();                 // golden cObserver.h:362
    TPanel *pnlDayJamRate = new TPanel();           // golden cObserver.h:363

    // golden .dfm design-time ColCount/RowCount (tools/dfm2rc/ir_out/
    // cObserver.dfm.ir.json, read this wave): TimeInfoGrid_InArm ColCount=2
    // RowCount=15; strngrdTimeData ColCount=13, RowCount UNSET in the .dfm ->
    // default 5 (same "no explicit RowCount -> ctor default" posture Wave 1
    // already established for strngrdEventLog/strngrdMDBQuery).
    TfObserverGrid *TimeInfoGrid_InArm = new TfObserverGrid(2, 15);  // golden cObserver.h:140 -- RecordInArmTime
    TfObserverGrid *strngrdTimeData = new TfObserverGrid(13);        // golden cObserver.h:290 -- GetTimeDataText
    TListBox       *lstTimeData = new TListBox();                   // golden cObserver.h:288

    int iIndexCycleTimeCount = 0;                  // golden cObserver.h:540 -- IniRecordMonitoringIndexCycleTime
    int iPauseTime = 0;                            // golden cObserver.h:518 -- CountMTBF
    int iProductTime = 0;                          // golden cObserver.h:519
    int iJamTime = 0;                              // golden cObserver.h:520

    double dTempHistroy[tcTotalCount][60] = {};    // golden cObserver.h:522 -- UpdateTempChart
    double fRecordIndexTime[20] = {};              // golden cObserver.h:510 -- RecordIndexTime
    // NOTE: golden also declares `fRecordInArmTime1[20]`/`fRecordInArmTime2[20]`
    // (cObserver.h:512-513) but RecordInArmTime (this wave's only fRecordInArmTime*
    // consumer) reads/writes ONLY the plain `fRecordInArmTime[20]` array -- grepped
    // golden :2968-2999 in full, 0 hits for the 1/2 variants inside this range.
    // Not added (no delivered method touches them -- Wave 1's own "don't invent
    // surface" discipline).
    double fRecordInArmTime[20] = {};              // golden cObserver.h:511 -- RecordInArmTime

    bool   bTestIndexZ = false;                    // golden cObserver.h:544 -- RecordIndexTime
    int    iTestIndexZCount = 0;                   // golden cObserver.h:545
    double dRecordIndexZTime[10] = {};             // golden cObserver.h:546
    AnsiString sTestIndexZTime;                    // golden cObserver.h:547 (AnsiString self-defaults to "")
    double dOEEIndexCycleTime = 0.0;               // golden cObserver.h:548

    TQPF_Timer tRecordInArmTimer;                  // golden cObserver.h:549 -- RecordInArmTime (real ctor, myTimer.h)

    AnsiString sRecordIndexCycleTime[200];         // golden cObserver.h:551 -- RecordIndexCycle (each self-defaults to "")

    // -- Wave 2 translated methods (bodies: cObserver.cpp, this wave) --------
    virtual void GetMachineData();                                 // golden :755-767
    virtual void Timer1Timer(void *Sender);                        // golden :708-753
    virtual void UpdateTempChart();                                // golden :905-943
    virtual void cbbTempChartChange(void *Sender);                 // golden :945-948
    virtual void ProcessRunInfo();                                 // golden :2254-2331
    virtual void RecordIndexTime(double fData);                    // golden :2814-2903
    virtual void AddTimeData(int iRow, double Time);               // golden :2905-2966
    virtual void RecordInArmTime();                                // golden :2968-2999
    virtual void WriteCategoryData();                              // golden :3274-3547
    virtual void GetTimeDataText();                                // golden :4795-4839
    virtual void RecordIndexCycle(bool bReset = false);            // golden :4846-4887

    // -- DEVIATION: TForm::Close() has no window here -- no-op (see banner) --
    virtual void Close();

    // ========================================================================
    // AI(W906-FW-ObsSwap) 20260818: swap-enablement members -- everything the
    // live fObserver consumers deref that Waves 1/2 had not yet carried
    // (measured usage sweep across all *.cpp, DEVLOG FW-ObsSwap). Same
    // unified-TPanel idiom the retired shim already justified (only ->Caption
    // is touched; the old "golden TLabel*" note was re-read and corrected to
    // TPanel* back in W7-F2).
    // ========================================================================
    TPanel *labModel    = new TPanel();   // golden cObserver.h:374
    TPanel *labFactory  = new TPanel();   // golden cObserver.h:377
    // AI(W906-FW-Q5) 20260818: golden cObserver.h:296 `TLabel *labLoaderCount;`
    // -- TPanel stand-in per this facade's labModel/labFactory precedent
    // (Caption is the only member the translated code touches).
    TPanel *labLoaderCount = new TPanel();
    TPanel *labBundleID = new TPanel();   // golden cObserver.h (SET_BUNDLE_INFO surface, uHGem G30-G33)
    TPanel *labBundlIn  = new TPanel();
    TPanel *labBundOut  = new TPanel();
    TPanel *lbSerialNumber01 = new TPanel();   // uHGem serial/firmware surface
    TPanel *lbSerialNumber02 = new TPanel();
    TPanel *lbSerialNumber03 = new TPanel();
    TPanel *lbSerialNumber04 = new TPanel();
    TPanel *lbFirmwareNumber01 = new TPanel();
    TPanel *lbFirmwareNumber02 = new TPanel();
    TPanel *lbFirmwareNumber03 = new TPanel();
    TPanel *lbFirmwareNumber04 = new TPanel();
    TfObserverMemoLines0    *Memo1Lines     = new TfObserverMemoLines0();     // golden TMemo* Memo1 (peek shape)
    TfObserverMemoLotSummary *memoLotSummary = new TfObserverMemoLotSummary(); // golden cObserver.h:339
    // AI(W906-FW-Q5) 20260818: StatisticalJamCount family REAL BODIES landed
    // (user-approved queue item 5). File writes go through the
    // W906_EVENTLOG_ROOT call-time getenv redirect (cObserver.cpp:1200
    // precedent); production (env unset) keeps golden's own literals.
    // golden decls cObserver.h:553-556 (+ ReadLoaderCount :555, btnSG_*
    // handlers :475-476).
    virtual void StatisticalJamCount(bool bIsNextDay=false);         // golden :5060-5277
    virtual void StatisticalLoaderCount();                           // golden :5278-5288
    virtual void ReadLoaderCount();                                  // golden :5289-5299
    virtual bool StatisticalJamCountEnable(AnsiString asJamCode);    // golden :5301-5329
    virtual void btnSG_QueryNowClick(TObject *Sender);               // golden :5361-5364 (body translated, NOT wired)
    virtual void btnSG_QueryYesterdayClick(TObject *Sender);         // golden :5366-5369 (body translated, NOT wired)

    // -- PORT-ONLY, NOT a golden member -- see W906Obs2_InstanceRegistrar's
    //    banner above. Declared LAST so `this` is fully constructed (every
    //    member above it already initialized) when its ctor runs.
    W906Obs2_InstanceRegistrar _w906Obs2SelfRegister{this};
};

// AI(W906-FW-ObsSwap) 20260818: the integration call was made (user-approved
// queue, 20260818 morning): the live global is now backed by THIS facade.
// TfObserverShim is retired from atester_shims.h/.cpp; the global's
// DEFINITION moved home to cObserver.cpp (golden cObserver.h declares
// `extern PACKAGE TfObserver *fObserver;` -- same homecoming as B4/cMyDB).
// Static-init construction is safe ONLY because the ctor guards its config
// reads on INIFileGeneral being open (see cObserver.cpp ctor) -- golden
// constructs this form after OpenGeneralIniFile in WinMain order, and the
// guard reproduces that precondition instead of crashing on a NULL ini or
// seeding production files at static-init (the Gerneral.ini incident class).
extern TfObserver *fObserver;                    // golden cObserver.h (extern PACKAGE)

// AI(W906-FW3-Observer-W2) 20260818: free functions (golden cObserver.h:
// 560-565, same file-scope placement -- these are NOT TfObserver members in
// golden either). Bodies: cObserver.cpp, this wave, appended at file tail.
void RecordStartTestTime();                     // golden :2136-2147
int  RecordEndTestTime(int iArm);               // golden :2150-2235 (0:arm1 1:arm2 2:雙Arm)
void RecordReceiveTestTime();                   // golden :4755-4764
void IniRecordMonitoringIndexCycleTime();       // golden :1836-1844
void RecordIndexAirOnTime1();                   // golden :5331-5343
void RecordIndexAirOnTime2();                   // golden :5346-5358

#endif // FORMS_FOBSERVER_H
