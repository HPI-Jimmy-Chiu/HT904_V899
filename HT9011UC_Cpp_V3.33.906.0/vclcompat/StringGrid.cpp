// ===========================================================================
//  vclcompat/StringGrid.cpp
//  Implementation of the headless vclcompat::TStringGrid shim
//  (see StringGrid.h for the full scope / property-emulation / out-of-range
//  policy notes).
//
//  AI(W906-uHGemEquipment) 20260716: new file.
//
//  AI(W906-VclGrid-1) 20260820: Controls.h include added -- needed for
//  TFont's COMPLETE type (StringGrid.h only forward-declares `class TFont;`,
//  see that header's own note) so the ctor can `new TFont()` and the dtor
//  can `delete Font` below. This is the ONE TU that pays that include cost;
//  StringGrid.h itself still does not.
// ===========================================================================
#include "vclcompat/StringGrid.h"
#include "vclcompat/Controls.h"   // TFont complete type (Font member, W906-VclGrid-1)

namespace vclcompat {

TStringGrid::TStringGrid(int initialColCount, int initialRowCount)
    : RowCount(this), ColCount(this),
      Visible(false), Font(new TFont()), DefaultColWidth(64), FixedRows(0),
      FixedCols(0), Row(0),
      rowCount_(initialRowCount < 1 ? 1 : initialRowCount),
      colCount_(initialColCount < 1 ? 1 : initialColCount)
{
    Cells.owner = this;
    data_.resize(static_cast<size_t>(rowCount_));
    for (size_t r = 0; r < data_.size(); ++r)
        data_[r].resize(static_cast<size_t>(colCount_));
}

// AI(W906-VclGrid-1) 20260820: deletes the owned Font (see StringGrid.h's
// TFont forward-decl note for why this must live here, not inline in the
// header).
TStringGrid::~TStringGrid()
{
    delete Font;
}

// ---------------------------------------------------------------------------
//  RowCount / ColCount resize (property side effect). Real VCL TStringGrid
//  preserves existing cell content across a resize (shrink truncates, grow
//  pads with "") -- replicated here via std::vector::resize, which does
//  exactly that for both the outer (row) and inner (col) dimensions.
// ---------------------------------------------------------------------------
void TStringGrid::doSetRowCount(int v)
{
    if (v < 1) v = 1;   // real VCL: RowCount minimum is 1 (there is always at least the header row)
    rowCount_ = v;
    data_.resize(static_cast<size_t>(rowCount_));
    for (size_t r = 0; r < data_.size(); ++r)
        data_[r].resize(static_cast<size_t>(colCount_));
}

void TStringGrid::doSetColCount(int v)
{
    if (v < 1) v = 1;   // real VCL: ColCount minimum is 1
    colCount_ = v;
    for (size_t r = 0; r < data_.size(); ++r)
        data_[r].resize(static_cast<size_t>(colCount_));
}

AnsiString& TStringGrid::cellRef(int col, int row)
{
    // .at() bounds-checks and throws std::out_of_range on a miss -- the
    // faithful analogue of real VCL's ERangeError on an out-of-bounds grid
    // cell access (see StringGrid.h's OUT-OF-RANGE BEHAVIOR note: this is
    // deliberately NOT auto-growing).
    return data_.at(static_cast<size_t>(row)).at(static_cast<size_t>(col));
}

const AnsiString& TStringGrid::cellRef(int col, int row) const
{
    return data_.at(static_cast<size_t>(row)).at(static_cast<size_t>(col));
}

// ---------------------------------------------------------------------------
//  RowCountProxy / ColCountProxy
// ---------------------------------------------------------------------------
TStringGrid::RowCountProxy::operator int() const { return owner_->rowCount_; }
TStringGrid::RowCountProxy& TStringGrid::RowCountProxy::operator=(int v)
{
    owner_->doSetRowCount(v);
    return *this;
}

TStringGrid::ColCountProxy::operator int() const { return owner_->colCount_; }
TStringGrid::ColCountProxy& TStringGrid::ColCountProxy::operator=(int v)
{
    owner_->doSetColCount(v);
    return *this;
}

// ---------------------------------------------------------------------------
//  ColAccessor (second half of Cells[ACol][ARow])
// ---------------------------------------------------------------------------
AnsiString& TStringGrid::ColAccessor::operator[](int row)
{
    return owner_->cellRef(col_, row);
}

const AnsiString& TStringGrid::ColAccessor::operator[](int row) const
{
    return owner_->cellRef(col_, row);
}

} // namespace vclcompat
