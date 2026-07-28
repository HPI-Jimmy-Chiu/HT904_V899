// ===========================================================================
//  vclcompat/render/TrayRender.cpp
//  See TrayRender.h for scope/provenance. Every drawing step cites the exact
//  golden D:\HT9045\elec\myvcl\HTray{,256}.cpp line range it translates.
//
//  AI(W906-W7-C2) 20260728: added this file's convention tag (LOW-3 audit
//  -- no functional change).
// ===========================================================================
#include "vclcompat/render/TrayRender.h"

namespace vclcompat {
namespace render {

namespace {

// Shared body for RenderTray/RenderTray256 below -- TrayCore and
// Tray256Core do not share a base class (D5: each custom-control core lives
// in its own file with no shared framework), but they DO expose an
// identical public surface for everything a renderer needs (Width/Height/
// FXItem/FYItem/FShape/FShowFont/ColorMap/FFrameColor/FTrayColor/
// FTrayDirect/FDirectWidth/CellRect()/GetCellData()/GetCellText() --
// verified directly against both headers, not assumed from naming). A
// template keeps DrawTray's single golden shape written exactly once.
template <typename CoreT>
void RenderTrayImpl(HDC hdc, const CoreT& core)
{
    // HTray.cpp:278-282 / HTray256.cpp:185-189: whole-control background
    // fill + border. (NewRect computed at :278/:185 is GOLDEN QUIRK dead
    // code -- see TrayRender.h banner -- every real Canvas call below uses
    // (0,0)-(Width,Height) control-local coordinates, never Left/Top.)
    GdiRectangle(hdc, 0, 0, core.Width, core.Height, core.FTrayColor, core.FFrameColor);

    // HTray.cpp:283-340 / HTray256.cpp:190-247: corner "direction" tick
    // marks, drawn with the SAME FFrameColor pen the border above used (the
    // golden Canvas carries its Pen->Color forward across calls -- it is one
    // shared Canvas object for the whole DrawTray, not reset per primitive).
    HPEN    tickPen = ::CreatePen(PS_SOLID, 1, ResolveColor(core.FFrameColor));
    HGDIOBJ oldPen  = ::SelectObject(hdc, tickPen);
    {
        int sx = 0, sy = 0, x = 0, y = 0;
        switch (core.FTrayDirect) {
            case csLeftTop:                                    // HTray.cpp:284-296
                sx = 0; sy = 0; x = sx; y = sy;
                for (int i = 0; i < core.FDirectWidth; ++i) {
                    ::MoveToEx(hdc, x, sy, NULL); ::LineTo(hdc, sx, y);
                    ++x; ++y;
                }
                break;
            case csLeftBottom:                                  // HTray.cpp:298-311
                sx = 0; sy = core.Height; x = sx; y = sy;
                for (int i = 0; i < core.FDirectWidth; ++i) {
                    ::MoveToEx(hdc, sx, y, NULL); ::LineTo(hdc, x, sy);
                    ++x; --y;
                }
                break;
            case csRightTop:                                    // HTray.cpp:312-326
                sx = core.Width; sy = 0; x = sx; y = sy;
                for (int i = 0; i < core.FDirectWidth; ++i) {
                    ::MoveToEx(hdc, x, sy, NULL); ::LineTo(hdc, sx, y);
                    --x; ++y;
                }
                break;
            case csRightBottom:                                 // HTray.cpp:327-340
                sx = core.Width; sy = core.Height; x = sx; y = sy;
                for (int i = 0; i < core.FDirectWidth; ++i) {
                    ::MoveToEx(hdc, sx, y, NULL); ::LineTo(hdc, x, sy);
                    --x; --y;
                }
                break;
            case csNull:
            default:
                // golden's if/else-if chain (HTray.cpp:284-340) has no branch
                // for csNull at all -- falls straight through to the cell
                // loop below with no ticks drawn. Matches exactly.
                break;
        }
    }
    ::SelectObject(hdc, oldPen);
    ::DeleteObject(tickPen);

    // HTray.cpp:341-343 / HTray256.cpp:248-250: `for(x..FXItem) for(y..FYItem)
    // DrawSingleIC(x,y);` -- loops the FULL item count, not the *visible*
    // count; DrawSingleIC's own early-return guards (bXVisible/bYVisible/
    // bounds -- HTray.cpp:232-239) are what CellRect()'s bool return already
    // encodes (TrayCore.h/.cpp banner), so a false return here is simply
    // "don't draw this cell", matching golden's early `return;`.
    for (int xi = 0; xi < core.FXItem; ++xi) {
        for (int yi = 0; yi < core.FYItem; ++yi) {
            TrayCellRect r;
            if (!core.CellRect(xi, yi, r)) continue;

            TColor cellColor = core.ColorMap[core.GetCellData(xi, yi)];   // HTray.cpp:249
            if (core.FShape)
                GdiEllipse(hdc, r.Left, r.Top, r.Right, r.Bottom, cellColor, core.FFrameColor);   // :251
            else
                GdiRectangle(hdc, r.Left, r.Top, r.Right, r.Bottom, cellColor, core.FFrameColor); // :253

            if (core.FShowFont) {                                          // HTray.cpp:255-259
                AnsiString text = core.GetCellText(xi, yi);
                RECT winRect;
                winRect.left = r.Left; winRect.top = r.Top;
                winRect.right = r.Right; winRect.bottom = r.Bottom;
                ::DrawTextA(hdc, text.c_str(), text.Length(), &winRect,
                            DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            }
        }
    }
}

} // anonymous namespace

void RenderTray(HDC hdc, const TrayCore& core)
{
    RenderTrayImpl(hdc, core);
}

void RenderTray256(HDC hdc, const Tray256Core& core)
{
    RenderTrayImpl(hdc, core);
}

} // namespace render
} // namespace vclcompat
