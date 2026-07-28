// ===========================================================================
//  vclcompat/render/RenderCommon.cpp
//  See RenderCommon.h for scope/provenance.
//
//  AI(W906-W7-C2) 20260728: added this file's convention tag (LOW-3 audit
//  -- no functional change).
// ===========================================================================
#include "vclcompat/render/RenderCommon.h"

namespace vclcompat {
namespace render {

void GdiRectangle(HDC hdc, int left, int top, int right, int bottom,
                   TColor brushColor, TColor penColor)
{
    COLORREF cBrush = ResolveColor(brushColor);
    COLORREF cPen   = ResolveColor(penColor);
    HBRUSH  br = ::CreateSolidBrush(cBrush);
    HPEN    pn = ::CreatePen(PS_SOLID, 1, cPen);
    HGDIOBJ oldBr = ::SelectObject(hdc, br);
    HGDIOBJ oldPn = ::SelectObject(hdc, pn);
    ::SetBkColor(hdc, cBrush);   // see header banner: mirrors VCL's Brush->Color/SetBkColor coupling
    ::Rectangle(hdc, left, top, right, bottom);
    ::SelectObject(hdc, oldBr);
    ::SelectObject(hdc, oldPn);
    ::DeleteObject(br);
    ::DeleteObject(pn);
}

void GdiEllipse(HDC hdc, int left, int top, int right, int bottom,
                TColor brushColor, TColor penColor)
{
    COLORREF cBrush = ResolveColor(brushColor);
    COLORREF cPen   = ResolveColor(penColor);
    HBRUSH  br = ::CreateSolidBrush(cBrush);
    HPEN    pn = ::CreatePen(PS_SOLID, 1, cPen);
    HGDIOBJ oldBr = ::SelectObject(hdc, br);
    HGDIOBJ oldPn = ::SelectObject(hdc, pn);
    ::SetBkColor(hdc, cBrush);
    ::Ellipse(hdc, left, top, right, bottom);
    ::SelectObject(hdc, oldBr);
    ::SelectObject(hdc, oldPn);
    ::DeleteObject(br);
    ::DeleteObject(pn);
}

} // namespace render
} // namespace vclcompat
