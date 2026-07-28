// ===========================================================================
//  vclcompat/render/RenderCommon.h
//  Shared GDI plumbing for the W7-C2 HDC renderers (TrayRender.*/LedRender.*/
//  BtnPanelRender.*). No golden source maps 1:1 onto this file -- it factors
//  out the small amount of "how do we turn a VCL TColor into a COLORREF and
//  reproduce a VCL-Canvas-shaped filled+outlined primitive" plumbing that all
//  3 renderers independently need, so each of THEM stays a line-for-line
//  translation of its own golden Draw*/Paint function instead of re-deriving
//  this 3 times over.
//
//  Targets a raw HDC (link `gdi32`, MinGW ships it in full). Per plan
//  §6-W7-C2: "renderer 不卡 MFC -- CDC 只是 HDC 的薄包裝", so this deliberately
//  never touches MFC/CDC; a later CDC-based shell can wrap these same HDC
//  entry points almost verbatim (CDC::Rectangle etc. just forward to the
//  identical Win32 GDI calls used here).
//
//  AI(W906-W7-C2) 20260728: added this file's convention tag (LOW-3 audit
//  -- no functional change).
// ===========================================================================
#ifndef VCLCOMPAT_RENDER_RENDERCOMMON_H
#define VCLCOMPAT_RENDER_RENDERCOMMON_H

#include <windows.h>

namespace vclcompat {

// Shared with vclcompat/TrayCore.h / LedCore.h / BtnPanelCore.h -- identical
// guarded copy, same convention already established across those 3 headers
// (see TrayCore.h's own banner for why each file carries its own guarded
// block rather than a single shared umbrella colour header: this directory
// is deliberately going to have more than one of these headers #included
// into the same translation unit by a later wave).
#ifndef HT9045_W7C1_TCOLOR_SHIM
#define HT9045_W7C1_TCOLOR_SHIM
typedef int TColor;                           // golden Graphics.hpp TColor (0x00BBGGRR int)
const TColor clBlack   = 0x00000000;
const TColor clGreen   = 0x00008000;
const TColor clRed     = 0x000000FF;
const TColor clBlue    = 0x00FF0000;
const TColor clLime    = 0x0000FF00;
const TColor clYellow  = 0x0000FFFF;
const TColor clWhite   = 0x00FFFFFF;
const TColor clSilver  = 0x00C0C0C0;
const TColor clBtnFace = TColor(0x8000000F);  // system-colour reference (COLOR_BTNFACE)
#endif // HT9045_W7C1_TCOLOR_SHIM

namespace render {

// Delphi TColor -> COLORREF, i.e. VCL's own ColorToRGB(). TColor is already
// byte-identical to COLORREF (0x00BBGGRR) for a literal colour (plan
// §6-W7-C2: "TColor 是 0x00BBGGRR，與 COLORREF byte-identical，直接 cast，不要
// swizzle") EXCEPT when the top byte is 0x80, which flags a SYSTEM colour
// reference -- the low byte is then a COLOR_* index (e.g. clBtnFace =
// 0x8000000F = COLOR_BTNFACE = 15). This path is real and load-bearing here,
// not a defensive nicety: BtnPanelCoreBase's ctor leaves the public `Color`
// field set to clBtnFace (butPa1.cpp:33 / BtnPanelLane.cpp:35, via the
// locally-shadowed `tcFalseColor` -- see BtnPanelCore.h/.cpp's own GOLDEN BUG
// note) until the very first SetTrueColor/SetFalseColor call, so any render
// of a freshly-constructed BtnPanelCoreBase needs this resolved correctly to
// avoid painting a nonsense colour (a naive direct cast of 0x8000000F to
// COLORREF would read as R=0x0F,G=0x00,B=0x80 -- visibly wrong).
inline COLORREF ResolveColor(TColor c)
{
    unsigned int uc = (unsigned int)c;
    if ((uc & 0xFF000000u) == 0x80000000u)
        return ::GetSysColor((int)(uc & 0xFFu));
    return (COLORREF)(uc & 0x00FFFFFFu);
}

// GDI equivalent of golden's `Canvas->Brush->Color=brush; Canvas->Pen->Color
// =pen; Canvas->Rectangle(l,t,r,b);` -- VCL's TCanvas.Rectangle is a thin
// wrapper over the Win32 Rectangle() GDI call in the BCB6-era VCL this
// codebase targets, so `::Rectangle` reproduces it directly, including its
// right/bottom-EXCLUSIVE edge semantics (both golden and here).
//
// Also reproduces a real, easy-to-overlook VCL behaviour: assigning
// Canvas->Brush->Color syncs the DC's background colour too (TBrush
// realization pairs the brush handle with a SetBkColor call, so that a
// following hollow-brush fill or DrawText on the SAME Canvas lands on the
// same colour as whatever was just filled, with no visible seam). This is
// what makes DrawSingleIC's `Canvas->Brush->Color=ColorMap[...]` immediately
// followed by `DrawText(...)` (HTray.cpp:249-258) paint the glyph's opaque
// background as the SAME colour as the cell fill rather than default white
// -- golden's DrawSingleIC never calls SetBkColor explicitly, it relies on
// this ambient Canvas mechanism.
void GdiRectangle(HDC hdc, int left, int top, int right, int bottom,
                   TColor brushColor, TColor penColor);

// GDI equivalent of `Canvas->Ellipse(l,t,r,b)`, under the identical
// Brush/Pen + SetBkColor coupling as GdiRectangle above.
void GdiEllipse(HDC hdc, int left, int top, int right, int bottom,
                TColor brushColor, TColor penColor);

} // namespace render
} // namespace vclcompat
#endif // VCLCOMPAT_RENDER_RENDERCOMMON_H
