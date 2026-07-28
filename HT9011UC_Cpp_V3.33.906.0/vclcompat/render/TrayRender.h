// ===========================================================================
//  vclcompat/render/TrayRender.h
//  HDC renderer for vclcompat::TrayCore / vclcompat::Tray256Core (W7-C2).
//
//  Golden sources (cp950/big5-decoded, D:\HT9045\elec\myvcl\HTray{,256}.cpp):
//  DrawTray (HTray.cpp:272-344 / HTray256.cpp:179-251) and the Canvas half of
//  DrawSingleIC (HTray.cpp:230-260 / HTray256.cpp:140-167) -- the geometry
//  half of DrawSingleIC (CellRect) and all state (CaculateTrayParameter, cell
//  colour/text storage, visibility) already live in the W7-C1 core and are
//  NOT recomputed here; this file consumes them read-only.
//
//  These are the ONLY two custom controls in the whole family with real
//  drawing code (22/23 grep hits for Canvas|Paint|DrawText|Rectangle|
//  Ellipse|StretchDraw across the whole elec\myvcl custom-control set belong
//  to HTray{,256}.cpp -- LED and button families have zero, see
//  LedRender.h / BtnPanelRender.h for how each of those is handled instead).
//
//  AI(W906-W7-C2) 20260728: added this file's convention tag (LOW-3 audit
//  -- no functional change).
// ===========================================================================
#ifndef VCLCOMPAT_RENDER_TRAYRENDER_H
#define VCLCOMPAT_RENDER_TRAYRENDER_H

#include "vclcompat/render/RenderCommon.h"
#include "vclcompat/TrayCore.h"

namespace vclcompat {
namespace render {

// Renders into hdc's (0,0)-(core.Width,core.Height) region -- control-LOCAL
// coordinates, matching every Canvas call in golden's DrawTray/DrawSingleIC.
// GOLDEN QUIRK: DrawTray computes `TRect NewRect = Rect(Left, Top,
// Left+Width, Top+Height)` (HTray.cpp:278 / HTray256.cpp:185) but NEVER uses
// it -- dead code, harmless, documented here so nobody wonders why this
// renderer ignores a `Left`/`Top` control-screen-offset that golden itself
// never actually paints with either (a VCL Canvas is already control-
// relative, so every real Canvas call below correctly starts at (0,0)).
//
// If FShowFont is set, text is drawn with whatever font/text colour is
// CURRENTLY SELECTED into hdc -- golden's `Canvas->Font = Font` (HTray.cpp:
// 279) reads the control's ambient inherited VCL Font property, which this
// framework-free core has no field for at all (out of scope, see
// TrayCore.h); callers that care about a specific typeface/colour must
// SelectObject/SetTextColor it into hdc before calling. The per-glyph
// BACKGROUND colour, however, IS reproduced faithfully (see RenderCommon.h's
// GdiRectangle/GdiEllipse banner on the Brush->SetBkColor coupling) -- it is
// governed by the cell's own fill colour, exactly as in golden, regardless
// of what the caller pre-selected.
void RenderTray(HDC hdc, const TrayCore& core);

// Same shape for TTMyTray256 (HTray256.cpp) -- no block support, so its
// DrawSingleIC has no iBX/iBY offset term at all (already reflected in
// Tray256Core::CellRect, consumed here read-only same as RenderTray above).
void RenderTray256(HDC hdc, const Tray256Core& core);

} // namespace render
} // namespace vclcompat
#endif // VCLCOMPAT_RENDER_TRAYRENDER_H
