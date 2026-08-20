// ===========================================================================
//  vclcompat/StringGrid.h
//  Headless BCB6 TStringGrid compatibility shim (VCL Grids.hpp).
//
//  AI(W906-uHGemEquipment) 20260716: new file.
//
//  FIRST CONSUMER: SECSGEM/uHGemEquipment.h/.cpp's CEID/Report/Alarm
//  "database" family -- THGem's strGrdCEID / stdGridReportID / strGrdAlarm /
//  strGrdAlarmOld members (golden uHGemEquipment.h:139-144), which the
//  golden .dfm declares:
//      strGrdAlarmOld : ColCount=<default 5>, RowCount=1,   FixedRows=0
//      strGrdCEID     : ColCount=258,         RowCount=1025
//      stdGridReportID: ColCount=1026,        RowCount=257
//      strGrdAlarm    : ColCount=12,          RowCount=<default 5>
//  (uHGemEquipment.dfm:375-436; unspecified RowCount/ColCount falls back to
//  real VCL TStringGrid's design-time default of 5, replicated below).
//
//  SCOPE (deliberately narrow, per this translation wave's plan): headless
//  Cells[ACol][ARow] + growable RowCount/ColCount. STILL NO rendering, NO
//  Options/OnDrawCell/OnSelectCell/RowHeights/MouseToCell -- none of those
//  are read by any of the pure-logic THGem methods this shim was built for
//  (CopyStringGridAsTabFormat/PasteStringGridAsTabFormat/SetCEIDContent/
//  GetCEIDContent/... -- see uHGemEquipment.cpp's own file-head note for the
//  full list).
//
//  AI(W906-VclGrid-1) 20260820: Visible/Font/DefaultColWidth/FixedRows/
//  FixedCols/Row/ColWidths[]/ClearRow(i) ADDED below -- collapsed HOME from
//  forms/fObserver.h's TfObserverGrid and forms/fConfiguration.h's
//  TfConfigurationGrid, which had each independently forked a byte-for-byte
//  (or near-identical) copy of the exact same auto-growing ColWidths proxy
//  plus plain-data property set (forms/fStartCondition.h's
//  TfStartConditionGrid and forms/fContactCT.h's TfContactCTGrid had ALSO
//  grown their own extra widget surface, but a DIFFERENTLY-SHAPED one --
//  fixed-size `int ColWidths[16]`/`int RowHeights[64]` arrays plus Top/
//  Width/Height, not this proxy -- so those two stay local; see those two
//  headers' own updated notes. forms/fShowBinSelect.h's TfShowBinSelectGrid
//  never grew any of this set at all -- its only addition, `Selection.Top`,
//  is unrelated and also stays local). Consuming code is UNCHANGED: every
//  one of these was already reached through `grid->Member` on a `TfXxxGrid*`,
//  which now simply resolves one level up the inheritance chain instead of
//  on the subclass itself; no call site needed editing for this move. Still
//  no rendering -- these are the same plain-data/no-op members the two
//  subclasses already carried, moved, not upgraded.
//
//  RESIDUAL GATE NOTE: this addition also means the two SECSGEM call sites
//  that were previously gated specifically because "vclcompat::TStringGrid
//  has no ColWidths[]" (cMyDB.cpp:1661 GetAlarmCodeList's `#if 0` block, and
//  SECSGEM/uHGemHT9045.cpp:604-608 AddAlarmList's GATE [R2]) COULD now
//  compile if un-gated. NOT done as part of this wave (its stated scope is
//  this header plus the five form facades, not SECSGEM consumers) -- flagged
//  here so the next reader of either GATE comment does not have to re-derive
//  that the premise changed.
//
//  A future UI-facing wave that needs actual grid RENDERING (drawing rows/
//  columns/fonts sized per these properties) should extend this class
//  additively (matching the project's established additive-only vclcompat
//  convention), not replace it.
//
//  INDEXING: Cells[ACol][ARow] is 0-based on BOTH axes in real Delphi/BCB6
//  (unlike vclcompat::AnsiString, which is 1-based) -- confirmed by golden's
//  own loops, which start `for(int y=1; y<strGrd->RowCount; y++)` specifically
//  to SKIP row 0 (the header/caption row -- e.g. real on-disk row 0 of
//  D:\HT9045\SECS\SECS\SYSTEM\EventReport_CEID.def is the literal text
//  "CEID\tEnable\tAlias\tReportID\t", not CEID data; row 0 of AlarmData.def is
//  "No.\tID_AlarmList\tAlarmCode\t...").
//
//  PROPERTY EMULATION: RowCount/ColCount are BCB6 properties whose assignment
//  has a SIDE EFFECT (resize, preserving existing cell content -- real VCL
//  behavior; shrinking truncates, growing pads with ""). Modeled with the
//  same "Proxy-with-operator=" idiom already established by
//  vclcompat::TStringList's StringsProxy family and
//  Scktcomp::TClientSocket::ActiveProxy.
//
//  OUT-OF-RANGE BEHAVIOR: Cells[][] access outside [0,ColCount) x [0,RowCount)
//  throws std::out_of_range (std::vector::at), mirroring real VCL's
//  ERangeError on an out-of-bounds grid cell rather than silently growing the
//  grid or returning a dummy value. This is DELIBERATE and load-bearing: see
//  SECSGEM/uHGemEquipment.cpp's THGem::GetCEIDContent / GetReportIDContent
//  call-site comments for a genuine golden bug (loop bound uses ->RowCount
//  where ->ColCount is clearly the intent) that this out-of-range behavior
//  faithfully preserves rather than papering over.
// ===========================================================================
#ifndef VCLCOMPAT_STRINGGRID_H
#define VCLCOMPAT_STRINGGRID_H

#include "vclcompat/AnsiString.h"
#include "vclcompat/TStringList.h"   // vclcompat::TObject -- see R8 base-class note below
#include <vector>

namespace vclcompat {

// AI(W906-VclGrid-1) 20260820: forward-declared only. `Font` below is an
// OWNED pointer whose allocation/deallocation lives in StringGrid.cpp (which
// #includes vclcompat/Controls.h for TFont's complete type) -- this header
// itself never needs more than the pointer, so it does not have to pull in
// Controls.h's whole stock-widget set (TPanel/TEdit/TComboBox/TLabel/...)
// just to carry a 4-int-plus-one-AnsiString struct. Keeps this shared,
// ~15-TU-wide header's include footprint exactly as narrow as before this
// wave (still just AnsiString.h + TStringList.h).
class TFont;

//  AI(W906-F0fix) 20260728: added `: public TObject`.  MEDIUM-2 finding: in
//  golden every VCL class (TStringGrid included) derives from TObject, so
//  `SetSVDataPointer(..., fMain->AutoCleanStringGrid, ...)` binds the
//  TObject* overload (SecsSvEcRegistration.h:149).  With NO base here, the
//  identical ported call could only reach the `void *P` overload (:133) --
//  SILENTLY (the exact R8/SS9-R8 hazard plan SS4-V2 documents; see
//  vclcompat/Controls.h's file-head note for the full mechanism).
//
//  WHY ADDING THE BASE IS SAFE -- the real argument is a language-level one,
//  not a survey of call sites: this class was ALREADY a non-aggregate (it has
//  private data members and a user-declared constructor), so it could never
//  be aggregate-initialized `{...}` in the first place, and adding a base
//  changes nothing about how it may legally be initialized. That holds
//  regardless of how many construction sites exist or what shape they take.
//
//  AI(W906-F0fix2) 20260729: an earlier version of this comment instead
//  justified the change by asserting "every construction site is
//  `new TStringGrid(...)` through a pointer -- no by-value use exists
//  anywhere". That claim is FALSE and is removed: by-value stack
//  constructions do exist (tests/test_common.cpp and
//  tests/test_uHGemEquipment.cpp both do `vclcompat::TStringGrid g(3, 2);`).
//  They are unaffected -- a by-value object simply gains a vptr -- so the
//  conclusion was right for the wrong reason. Kept the sound argument above
//  and dropped the survey, which was both untrue and unnecessary.
//
//  tests/test_w7_f0_controls_guard.cpp static_asserts that this class binds
//  the TObject* overload, same as the other stock-widget stand-ins.
class TStringGrid : public TObject {
public:
    // Real VCL TStringGrid design-time default (before any .dfm ColCount=/
    // RowCount= override) is 5x5 -- e.g. golden strGrdAlarm's .dfm sets only
    // ColCount=12 and leaves RowCount at this default (uHGemEquipment.dfm:
    // 428-436); strGrdAlarmOld leaves ColCount at this default and sets only
    // RowCount=1 (uHGemEquipment.dfm:375-383).
    explicit TStringGrid(int initialColCount = 5, int initialRowCount = 5);

    // AI(W906-VclGrid-1) 20260820: out-of-line so it can `delete Font` against
    // TFont's complete type (only StringGrid.cpp includes Controls.h for
    // that) -- see the TFont forward-decl note above.
    virtual ~TStringGrid();

    // Non-copyable: the property proxies below hold a back-pointer to
    // `this`, same rationale as vclcompat::TStringList forbidding copy/move.
    TStringGrid(const TStringGrid&) = delete;
    TStringGrid& operator=(const TStringGrid&) = delete;

    // ---- RowCount / ColCount : property, assignment RESIZES ---------------
    class RowCountProxy {
    public:
        explicit RowCountProxy(TStringGrid* o) : owner_(o) {}
        operator int() const;
        RowCountProxy& operator=(int v);
    private:
        TStringGrid* owner_;
    };
    class ColCountProxy {
    public:
        explicit ColCountProxy(TStringGrid* o) : owner_(o) {}
        operator int() const;
        ColCountProxy& operator=(int v);
    private:
        TStringGrid* owner_;
    };
    RowCountProxy RowCount;
    ColCountProxy ColCount;

    // ---- Cells[ACol][ARow] : 0-based on both axes; l-value capable --------
    class ColAccessor {
    public:
        ColAccessor(TStringGrid* o, int col) : owner_(o), col_(col) {}
        AnsiString& operator[](int row);
        const AnsiString& operator[](int row) const;
    private:
        TStringGrid* owner_;
        int col_;
    };
    struct CellsAccessor {
        TStringGrid* owner;
        ColAccessor operator[](int col) const { return ColAccessor(owner, col); }
    };
    CellsAccessor Cells;

    // =========================================================================
    //  AI(W906-VclGrid-1) 20260820 ADD -- collapsed from forms/fObserver.h's
    //  TfObserverGrid and forms/fConfiguration.h's TfConfigurationGrid (see
    //  this header's own SCOPE banner for the full provenance/rationale).
    //  Defaults below were verified against BOTH ex-owners' ctor init lists,
    //  20260820: Visible/DefaultColWidth/FixedRows existed only on
    //  TfObserverGrid; Font/FixedCols/Row existed only on
    //  TfConfigurationGrid; DefaultColWidth/FixedRows/the ColWidths proxy
    //  existed on BOTH, with IDENTICAL default values (64/0/empty-growable)
    //  and (for the proxy) an identical body -- so one shared definition
    //  serves every prior caller with no behaviour change.
    // =========================================================================
    bool Visible;           // golden TStringGrid->Visible (ex-TfObserverGrid, strngrdMDBQuery only) -- default false
    TFont *Font;            // golden TStringGrid->Font (ex-TfConfigurationGrid, strngrdTray/HP ->Font->Size) -- OWNED, see ctor/dtor
    int  DefaultColWidth;   // golden TStringGrid->DefaultColWidth -- default 64 (both ex-owners agreed)
    int  FixedRows;         // golden TStringGrid->FixedRows -- default 0 (both ex-owners agreed)
    int  FixedCols;         // golden TStringGrid->FixedCols (ex-TfConfigurationGrid only) -- default 0
    int  Row;               // golden TStringGrid->Row (ex-TfConfigurationGrid only, "select the new row") -- default 0

    // golden ->ColWidths[i]= -- proxy so `grid->ColWidths[i]=w;` keeps
    // golden's property-assignment spelling. Auto-grows on write past
    // current size, padding new slots with the CURRENT DefaultColWidth at
    // the time of the write (matches real VCL, and both ex-owners' proxy
    // bodies verbatim). AI(W906-VclGrid-1) 20260820: both ex-owners'
    // constructors additionally PRE-SIZED this vector at construction time
    // (32 for TfObserverGrid, 16 for TfConfigurationGrid) -- REMOVED here as
    // a verified no-op: grepped every `ColWidths[` access tree-wide
    // (`grep -rn "ColWidths\[" --include=*.cpp`, 20260820) and every single
    // hit across every consumer (cConfiguration.cpp/cObserver.cpp/
    // cStartCondition.cpp/the gated cMyDB.cpp+uHGemHT9045.cpp sites) is a
    // WRITE -- none reads an index nobody wrote first -- so eager pre-sizing
    // vs. lazy first-touch growth are observationally identical; this
    // constructor leaves the backing vector empty and lets first access grow
    // it.
    struct ColWidthsProxy {
        TStringGrid *owner;
        int &operator[](int idx)
        {
            if (idx >= static_cast<int>(owner->colWidths_.size()))
                owner->colWidths_.resize(idx + 1, owner->DefaultColWidth);
            return owner->colWidths_[idx];
        }
    };
    ColWidthsProxy ColWidths{this};

    // golden ->Rows[i]->Clear() substitute (this shim has no TStrings-shaped
    // Rows[] accessor to return) -- wipes every cell in row `row` to "".
    // Moved verbatim from TfObserverGrid (its only prior owner).
    void ClearRow(int row)
    {
        for (int c = 0; c < ColCount; ++c)
            Cells[c][row] = AnsiString("");
    }
    // ==== end W906-VclGrid-1 ADD =============================================

private:
    int rowCount_;
    int colCount_;
    std::vector<std::vector<AnsiString> > data_;   // data_[row][col]
    std::vector<int> colWidths_;   // AI(W906-VclGrid-1) 20260820: backing store for ColWidths[] above

    friend class RowCountProxy;
    friend class ColCountProxy;
    friend class ColAccessor;

    void doSetRowCount(int v);
    void doSetColCount(int v);
    AnsiString& cellRef(int col, int row);
    const AnsiString& cellRef(int col, int row) const;
};

} // namespace vclcompat

// NOT brought into the global namespace here (this shim is new/not yet wired
// into vclcompat/vcl_compat.h -- integrate stage's job per project
// convention; see vcl_compat.h's own TList.h precedent/caution). Consumers
// add their own `using vclcompat::TStringGrid;` in a TU that does not clash
// (this type name is confirmed collision-free across the whole Cpp tree as
// of this writing -- grepped).

#endif // VCLCOMPAT_STRINGGRID_H
