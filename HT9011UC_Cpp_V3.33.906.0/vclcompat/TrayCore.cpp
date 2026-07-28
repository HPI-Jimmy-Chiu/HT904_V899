// ===========================================================================
//  vclcompat/TrayCore.cpp
//  See TrayCore.h for scope/provenance. Every method banner cites the exact
//  golden D:\HT9045\elec\myvcl\HTray{,256}.cpp line range it translates.
//
//  AI(W906-W7-C1) 20260728: added this file's convention tag (LOW-3 audit
//  -- this .cpp itself had no functional change; see TrayCore.h for the
//  file that already carried one).
// ===========================================================================
#include "vclcompat/TrayCore.h"
#include <cstdio>

namespace vclcompat {

// ===========================================================================
//  TrayCore
// ===========================================================================

TrayCore::TrayCore()                                            // HTray.cpp:19-60
{
    // HTray.cpp:23-27 -- only 11 of MAX_COLOR_INDEX(32) initialisers are
    // given in golden's array literal; GOLDEN QUIRK: entries 11..31 are
    // value-initialised to 0 == clBlack (C++ aggregate-init rule, same in
    // both golden BCB6 and here).
    const TColor TC[MAX_COLOR_INDEX] = {
        clWhite, clGreen, clYellow, clLime, clRed,
        TColor(0x00FF8080), TColor(0x00DC34BE), clBlue,
        TColor(0x00FF80FF), TColor(0x00FF0080), TColor(0x0000FF80)
        // remaining 21 entries: value-initialised 0 (clBlack), matching golden
    };

    Width = 100; Height = 200;                                   // HTray.cpp:29-30
    FXItem = 2; FYItem = 2;
    FXVisibleItem = 2; FYVisibleItem = 2;
    FXBlock = 1; FYBlock = 1; FXBlockWidth = 1; FYBlockWidth = 1;
    FLineWidth = 1; FEdgeWidth = 1;
    FShowFont = true;
    // GOLDEN QUIRK: HTray.cpp's ctor never assigns FShape at all (HTray.h
    // declares its published default as `default = false`, but a C++
    // __published default is only the value the STREAMING SYSTEM writes if
    // nothing else does -- for a .dfm instance that never overrides it, the
    // in-memory field is genuinely whatever the compiler happened to
    // zero/leave it as before the ctor body runs). We pin it to false here
    // (matching the declared default) since a framework-free core has no
    // streaming system to reproduce the "maybe uninitialised" nuance
    // usefully, and false is what every real .dfm instance ends up seeing
    // in practice.
    FShape = false;

    iStartX = 0; iStartY = 0; iPitchX = 0; iPitchY = 0; iXWidth = 0; iYWidth = 0;
    for (int i = 0; i < MAX_COLOR_INDEX; ++i) ColorMap[i] = TC[i];
    for (int i = 0; i < MAX_ITEM; ++i) {
        for (int j = 0; j < MAX_ITEM; ++j) {
            CellColorIndex[i][j] = 0;
            CellText[i][j] = "5";
        }
        bXVisible[i] = true;
        bYVisible[i] = true;
    }
    FTrayColor = clWhite;
    FFrameColor = clBlack;
    FTrayDirect = csLeftTop;
    FDirectWidth = 10;
}

void TrayCore::SetExtents(int w, int h)                          // NEW, not golden -- see header
{
    Width = w;
    Height = h;
    CaculateTrayParameter();
}

void TrayCore::SetXItem(int value)                                // HTray.cpp:105-122
{
    if (value <= 0) value = 1;
    if (value >= MAX_ITEM) value = MAX_ITEM - 1;

    FXItem = value;
    FXVisibleItem = FXItem;

    for (int i = 0; i < MAX_ITEM; ++i) bXVisible[i] = true;

    CaculateTrayParameter();
    ClearCell();
}

void TrayCore::SetYItem(int value)                                // HTray.cpp:124-140
{
    if (value <= 0) value = 1;
    if (value >= MAX_ITEM) value = MAX_ITEM - 1;
    FYItem = value;
    FYVisibleItem = FYItem;

    // GOLDEN BUG (HTray.cpp:133-136): resets bXVisible, NOT bYVisible, even
    // though this setter just changed the Y axis. Preserved verbatim -- see
    // SetBlockYItem below for the matching copy-paste sibling. Net effect
    // (also verbatim): NO setter in this class ever resets bYVisible, so
    // FYVisibleItem can be changed (via SetYItem/SetBlockYItem) while
    // bYVisible[] itself is left exactly as SetYVisible last left it.
    for (int i = 0; i < MAX_ITEM; ++i) bXVisible[i] = true;

    CaculateTrayParameter();
    ClearCell();
}

void TrayCore::SetBlockXItem(int value)                           // HTray.cpp:142-162
{
    if (value <= 0) value = 1;
    if (value >= 2) value = 2;
    if (value >= FXItem) FXItem = value;

    FXVisibleItem = FXItem;

    for (int i = 0; i < MAX_ITEM; ++i) bXVisible[i] = true;  // golden: bXVisible (correct here --
                                                              // this setter IS on the X axis, unlike
                                                              // SetBlockYItem's copy of this same line)

    FXBlock = value;
    CaculateTrayParameter();
    ClearCell();
}

void TrayCore::SetBlockYItem(int value)                           // HTray.cpp:164-184
{
    if (value <= 0) value = 1;
    if (value >= 5) value = 5;               // NOTE: golden clamps Y-block to 5, X-block to 2 --
                                              // an asymmetric limit, verified against HTray.cpp:169
                                              // (not a transcription slip: X truly clamps at 2).
    if (value >= FYItem) FYItem = value;

    FYVisibleItem = FYItem;

    // GOLDEN BUG (HTray.cpp:176-179): same copy-paste as SetYItem above --
    // this setter changed the Y axis but resets bXVisible, never bYVisible.
    for (int i = 0; i < MAX_ITEM; ++i) bXVisible[i] = true;

    FYBlock = value;
    CaculateTrayParameter();
    ClearCell();
}

void TrayCore::SetXBlockWidth(int value)                          // HTray.cpp:186-194
{
    if (value <= 0) value = 1;
    FXBlockWidth = value;
    CaculateTrayParameter();
    ClearCell();
}

void TrayCore::SetYBlockWidth(int value)                          // HTray.cpp:196-204
{
    if (value <= 0) value = 1;
    FYBlockWidth = value;
    CaculateTrayParameter();
    ClearCell();
}

void TrayCore::SetLineWidth(int value)                            // HTray.cpp:346-351
{
    FLineWidth = value;
    CaculateTrayParameter();
    // DrawTray() dropped, see SetFrameColor below.
}

void TrayCore::SetEdgeWidth(int value)                            // HTray.cpp:353-358
{
    FEdgeWidth = value;
    CaculateTrayParameter();
    // DrawTray() dropped, see SetFrameColor below.
}

void TrayCore::SetDirectWidth(int value)                          // HTray.cpp:360-366
{
    if (value < 0 || value > Width || value > Height) return;
    FDirectWidth = value;
    // DrawTray() dropped, see SetFrameColor below.
}

void TrayCore::SetTrayDirect(TTrayDirectStyle value)              // HTray.cpp:368-372
{
    FTrayDirect = value;
    // DrawTray() dropped, see SetFrameColor below.
}

void TrayCore::SetFrameColor(TColor Color)                        // HTray.cpp:206-210
{
    FFrameColor = Color;
    // golden also calls DrawTray() here -- rendering side effect, dropped
    // (W7-C2 renderer reads FFrameColor directly on its next paint).
}

void TrayCore::SetTrayColor(TColor Color)                         // HTray.cpp:212-216
{
    FTrayColor = Color;
    // DrawTray() dropped, see SetFrameColor above.
}

void TrayCore::CaculateTrayParameter()                            // HTray.cpp:262-270
{
    // GOLDEN BUG: no guard against FXVisibleItem/FYVisibleItem == 0 (reachable
    // by hiding every row/column via SetXVisible/SetYVisible) -> integer
    // divide-by-zero. Preserved verbatim -- see test_traycore.cpp for the
    // regression assertion that would fail if someone "fixed" this with a
    // guard.
    iXWidth = (Width  - 2 - (FXVisibleItem - 1) * FLineWidth - 2 * FEdgeWidth
               - (FXBlock - 1) * FXBlockWidth) / FXVisibleItem;
    iYWidth = (Height - 2 - (FYVisibleItem - 1) * FLineWidth - 2 * FEdgeWidth
               - (FYBlock - 1) * FYBlockWidth) / FYVisibleItem;
    iStartX = (Width  - iXWidth * FXVisibleItem - (FXVisibleItem - 1) * FLineWidth
               - (FXBlock - 1) * FXBlockWidth) / 2;
    iStartY = (Height - iYWidth * FYVisibleItem - (FYVisibleItem - 1) * FLineWidth
               - (FYBlock - 1) * FYBlockWidth) / 2;
    iPitchX = iXWidth + FLineWidth;
    iPitchY = iYWidth + FLineWidth;
}

bool TrayCore::CellRect(int X, int Y, TrayCellRect& r) const     // geometry half of
                                                                  // HTray.cpp:230-260 (DrawSingleIC)
{
    if (X >= FXItem || Y >= FYItem) return false;
    if (!bXVisible[X] || !bYVisible[Y]) return false;

    // GOLDEN BUG (HTray.cpp:241-242): no guard against FXBlock > FXItem (or
    // FYBlock > FYItem) -> integer divide-by-zero. Preserved verbatim.
    int iBX = X / (FXItem / FXBlock) * FXBlockWidth;
    int iBY = Y / (FYItem / FYBlock) * FYBlockWidth;

    r.Left = iStartX + X * iPitchX + iBX;
    r.Top  = iStartY + Y * iPitchY + iBY;
    r.Right  = r.Left + iXWidth;
    r.Bottom = r.Top  + iYWidth;
    return true;
}

int TrayCore::ConvertIndexCells(int& X, int& Y) const            // HTray.cpp:437-454
{
    // GOLDEN BUG: ignores the iBX/iBY block offsets that CellRect/DrawSingleIC
    // apply, so hit-testing is wrong whenever XBlockItem/YBlockItem > 1
    // (cells past the first block column/row hit-test into the wrong index
    // or miss entirely). Preserved verbatim -- 16 golden call sites depend on
    // this exact (buggy) mapping.
    X -= iStartX;
    Y -= iStartY;
    // GOLDEN BUG (also preserved): X/Y are mutated in place BEFORE the -1
    // early-return checks below, so a caller that ignores the -1 result and
    // reuses X/Y afterwards sees the partially-transformed (garbage)
    // coordinates, not the original ones. All golden call sites do check the
    // return value first; this core keeps the same contract.
    if (X < 0 || Y < 0) return -1;
    int ix = X / iPitchX, iy = Y / iPitchY, mx = X % iPitchX, my = Y % iPitchY;
    if (mx >= iXWidth || my >= iYWidth || ix >= FXItem || iy >= FYItem) return -1;
    X = ix; Y = iy;
    return 1;
}

void TrayCore::SetColorMap(int Index, TColor Color)               // HTray.cpp:374-380
{
    if (Index >= MAX_COLOR_INDEX || Index < 0) return;
    ColorMap[Index] = Color;
    // DrawTray() dropped, see SetFrameColor above.
}

void TrayCore::SetCellColorIndex(int X, int Y, int Index)         // HTray.cpp:382-388
{
    if (X >= FXItem || Y >= FYItem || Index >= MAX_COLOR_INDEX || Index < 0 || X < 0 || Y < 0)
        return;
    CellColorIndex[X][Y] = Index;
    // DrawSingleIC(X,Y) dropped -- pure paint, no further state change.
}

void TrayCore::SetShowFont(bool value)                            // HTray.cpp:390-394
{
    FShowFont = value;
}

void TrayCore::SetShape(bool value)                               // HTray.cpp:396-400
{
    FShape = value;
}

void TrayCore::SetCellNumber(int x, int y, const AnsiString& s)   // HTray.cpp:402-416 (char*/
                                                                   // AnsiString overloads folded --
                                                                   // AnsiString(const char*) already
                                                                   // exists, identical body either way)
{
    if (x >= FXItem || y >= FYItem || x < 0 || y < 0) return;
    CellText[x][y] = s;
}

void TrayCore::SetCellNumber(int x, int y, int s)                 // HTray.cpp:418-423
{
    char b[64];
    std::sprintf(b, "%d", s);
    SetCellNumber(x, y, AnsiString(b));
}

void TrayCore::SetCellNumber(int x, int y, double c)              // HTray.cpp:425-430
{
    char b[64];
    std::sprintf(b, "%0.2f", c);
    SetCellNumber(x, y, AnsiString(b));
}

int TrayCore::GetCellData(int i, int j) const                     // HTray.cpp:432-435
{
    return CellColorIndex[i][j];
}

AnsiString TrayCore::GetCellText(int X, int Y) const              // HTray.cpp:456-459
{
    return CellText[X][Y];
}

void TrayCore::ClearCell()                                        // HTray.cpp:92-103
{
    for (int i = 0; i < MAX_ITEM; ++i)
        for (int j = 0; j < MAX_ITEM; ++j) {
            CellColorIndex[i][j] = 0;
            CellText[i][j] = "";   // note: "" here vs the ctor's "5" -- a real (deliberate-looking)
                                    // asymmetry in golden, not a transcription slip.
        }
    // DrawTray() dropped, see SetFrameColor above.
}

void TrayCore::SetXVisible(int x, bool bVisible)                  // HTray.cpp:480-492
{
    if (x < FXItem) {
        bXVisible[x] = bVisible;
        FXVisibleItem = 0;
        for (int i = 0; i < FXItem; ++i)
            if (bXVisible[i]) ++FXVisibleItem;
    }
}

void TrayCore::SetYVisible(int y, bool bVisible)                  // HTray.cpp:494-506
{
    if (y < FYItem) {
        bYVisible[y] = bVisible;
        FYVisibleItem = 0;
        for (int i = 0; i < FYItem; ++i)
            if (bYVisible[i]) ++FYVisibleItem;
    }
}

AnsiString TrayCore::SaveCellTextToString() const                 // HTray.cpp:461-478, I/O split out
{
    AnsiString out;
    for (int i = 0; i < FXItem; ++i) {
        AnsiString row;
        for (int j = 0; j < FYItem; ++j)
            row += CellText[i][j] + AnsiString(", ");
        out += row;
        out += AnsiString("\r\n");
    }
    return out;
}

// ===========================================================================
//  Tray256Core
// ===========================================================================

Tray256Core::Tray256Core()                                        // HTray256.cpp:18-49
{
    const TColor TC[MAX_COLOR_INDEX] = {                          // HTray256.cpp:22-26, same
        clWhite, clGreen, clYellow, clLime, clRed,                // GOLDEN QUIRK as TrayCore (only
        TColor(0x00FF8080), TColor(0x00DC34BE), clBlue,            // 11/32 given, rest 0==clBlack)
        TColor(0x00FF80FF), TColor(0x00FF0080), TColor(0x0000FF80)
    };

    Width = 100; Height = 200;
    FXItem = 2; FYItem = 2;
    FLineWidth = 1; FEdgeWidth = 1;
    FShowFont = true;
    // GOLDEN QUIRK (same as TrayCore::TrayCore): FShape is never assigned in
    // golden's ctor either; pinned to its declared __published default.
    FShape = false;
    for (int i = 0; i < MAX_COLOR_INDEX; ++i) ColorMap[i] = TC[i];
    for (int i = 0; i < MAX_ITEM; ++i)
        for (int j = 0; j < MAX_ITEM; ++j) {
            CellColorIndex[i][j] = 0;
            CellText[i][j] = "5";
        }
    FTrayColor = clWhite;
    FFrameColor = clBlack;
    FTrayDirect = csLeftTop;
    FDirectWidth = 10;
    iStartX = 0; iStartY = 0; iPitchX = 0; iPitchY = 0; iXWidth = 0; iYWidth = 0;
}

void Tray256Core::SetExtents(int w, int h)                        // NEW, not golden
{
    Width = w;
    Height = h;
    CaculateTrayParameter();
}

void Tray256Core::SetXItem(int value)                             // HTray256.cpp:92-102
{
    if (value <= 0) value = 1;
    if (value >= MAX_ITEM) value = MAX_ITEM - 1;
    FXItem = value;
    CaculateTrayParameter();
    ClearCell();
}

void Tray256Core::SetYItem(int value)                             // HTray256.cpp:104-113
{
    if (value <= 0) value = 1;
    if (value >= MAX_ITEM) value = MAX_ITEM - 1;
    FYItem = value;
    CaculateTrayParameter();
    ClearCell();
}

void Tray256Core::SetFrameColor(TColor Color)                     // HTray256.cpp:115-119
{
    FFrameColor = Color;
}

void Tray256Core::SetTrayColor(TColor Color)                      // HTray256.cpp:121-125
{
    FTrayColor = Color;
}

void Tray256Core::CaculateTrayParameter()                         // HTray256.cpp:169-177
{
    // GOLDEN BUG: same missing FXItem/FYItem==0 guard as TrayCore -- reachable
    // here via SetXItem(0) clamping to 1 minimum, so in practice FXItem/FYItem
    // can never actually be 0 through the public setters (unlike TrayCore's
    // FXVisibleItem/FYVisibleItem, which SetXVisible/SetYVisible CAN legally
    // drive to 0). Documented for symmetry with TrayCore, not independently
    // reachable through this class's own public surface.
    iXWidth = (Width  - 2 - (FXItem - 1) * FLineWidth - 2 * FEdgeWidth) / FXItem;
    iYWidth = (Height - 2 - (FYItem - 1) * FLineWidth - 2 * FEdgeWidth) / FYItem;
    iStartX = (Width  - iXWidth * FXItem - (FXItem - 1) * FLineWidth) / 2;
    iStartY = (Height - iYWidth * FYItem - (FYItem - 1) * FLineWidth) / 2;
    iPitchX = iXWidth + FLineWidth;
    iPitchY = iYWidth + FLineWidth;
}

bool Tray256Core::CellRect(int X, int Y, TrayCellRect& r) const  // geometry half of
                                                                  // HTray256.cpp:140-167 (DrawSingleIC)
{
    if (X >= FXItem || Y >= FYItem) return false;
    r.Left = iStartX + X * iPitchX;
    r.Top  = iStartY + Y * iPitchY;
    r.Right  = r.Left + iXWidth;
    r.Bottom = r.Top  + iYWidth;
    return true;
}

int Tray256Core::ConvertIndexCells(int& X, int& Y) const         // HTray256.cpp:336-353
{
    X -= iStartX;
    Y -= iStartY;
    if (X < 0 || Y < 0) return -1;
    int ix = X / iPitchX, iy = Y / iPitchY, mx = X % iPitchX, my = Y % iPitchY;
    if (mx >= iXWidth || my >= iYWidth || ix >= FXItem || iy >= FYItem) return -1;
    X = ix; Y = iy;
    return 1;
}

void Tray256Core::SetColorMap(int Index, TColor Color)            // HTray256.cpp:281-287
{
    if (Index >= MAX_COLOR_INDEX || Index < 0) return;
    ColorMap[Index] = Color;
}

void Tray256Core::SetCellColorIndex(int X, int Y, int Index)      // HTray256.cpp:289-295
{
    if (X >= FXItem || Y >= FYItem || Index >= MAX_COLOR_INDEX || Index < 0 || X < 0 || Y < 0)
        return;
    CellColorIndex[X][Y] = Index;
}

void Tray256Core::SetShowFont(bool value)                         // HTray256.cpp:297-301
{
    FShowFont = value;
}

void Tray256Core::SetShape(bool value)                            // HTray256.cpp:303-307
{
    FShape = value;
}

void Tray256Core::SetCellNumber(int x, int y, const AnsiString& s) // HTray256.cpp:309-315
{
    if (x >= FXItem || y >= FYItem || x < 0 || y < 0) return;
    CellText[x][y] = s;
}

void Tray256Core::SetCellNumber(int x, int y, int s)              // HTray256.cpp:317-322
{
    char b[64];
    std::sprintf(b, "%d", s);
    SetCellNumber(x, y, AnsiString(b));
}

void Tray256Core::SetCellNumber(int x, int y, double c)           // HTray256.cpp:324-329
{
    char b[64];
    std::sprintf(b, "%0.2f", c);
    SetCellNumber(x, y, AnsiString(b));
}

int Tray256Core::GetCellData(int i, int j) const                  // HTray256.cpp:331-334
{
    return CellColorIndex[i][j];
}

AnsiString Tray256Core::GetCellText(int X, int Y) const           // HTray256.cpp:355-358
{
    return CellText[X][Y];
}

void Tray256Core::ClearCell()                                     // HTray256.cpp:81-90
{
    for (int i = 0; i < MAX_ITEM; ++i)
        for (int j = 0; j < MAX_ITEM; ++j) {
            CellColorIndex[i][j] = 0;
            CellText[i][j] = "";
        }
}

void Tray256Core::SetLineWidth(int value)                         // HTray256.cpp:253-258
{
    FLineWidth = value;
    CaculateTrayParameter();
}

void Tray256Core::SetEdgeWidth(int value)                         // HTray256.cpp:260-265
{
    FEdgeWidth = value;
    CaculateTrayParameter();
}

void Tray256Core::SetDirectWidth(int value)                       // HTray256.cpp:267-273
{
    if (value < 0 || value > Width || value > Height) return;
    FDirectWidth = value;
}

void Tray256Core::SetTrayDirect(TTrayDirectStyle value)           // HTray256.cpp:275-279
{
    FTrayDirect = value;
}

AnsiString Tray256Core::SaveCellTextToString() const              // HTray256.cpp:360-377
{
    AnsiString out;
    for (int i = 0; i < FXItem; ++i) {
        AnsiString row;
        for (int j = 0; j < FYItem; ++j)
            row += CellText[i][j] + AnsiString(", ");
        out += row;
        out += AnsiString("\r\n");
    }
    return out;
}

} // namespace vclcompat
