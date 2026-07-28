// ===========================================================================
//  vclcompat/render/LedRender.h
//  HDC renderer for vclcompat::LedCore / MyLedCore / MyLedLaneCore (the
//  "LED" custom-control family, W7-C2).
//
//  Golden source: D:\HT9045\elec\Component\aled.pas (TALed, the real common
//  base every TMyLed/TMyLedLane instance runs -- MyLed.cpp/MyLedLane.cpp add
//  IO-address metadata only, ZERO drawing code of their own, confirmed by
//  grep; see LedCore.h's own banner). The drawing is:
//    - CreateLedBitmap (aled.pas:106-160): per LEDStyle, LoadImage() one of 6
//      RT_BITMAP resources from aled.res, then flood-fill it once with the
//      brush colour (fTrueColor into fLedBitmap[0], fFalseColor into
//      fLedBitmap[1]) starting at (Width DIV 2, Height DIV 2) -- the
//      CONTROL's own Width/Height (aled.pas:157/159), NOT the bitmap's
//      native size; those two only coincide for 5 of the 6 styles (see the
//      LEDSqSmall GOLDEN QUIRK note in LedRender.cpp).
//    - Paint / OnLedTimer / SetToTrueColor / SetToFalseColor (aled.pas:170-
//      192): `Canvas->StretchDraw(Rect(0,0,Width,Height), fLedBitmap[Integer
//      (ColorTemp)])`.
//
//  This renderer reproduces BOTH halves for real (unlike Tray, there is no
//  W7-C1 "geometry half" to consume -- LedCore intentionally carries no
//  bitmap/pixel state at all, see LedCore.h's SCOPE note): it embeds the 6
//  aled.res RT_BITMAP blobs verbatim (extracted via a one-off Python RES-
//  format parser, see LedRender.cpp), floods a freshly-blitted copy with the
//  resolved TrueColor/FalseColor exactly as CreateLedBitmap does, then
//  stretch-blits it -- an ExtFloodFill(..., FLOODFILLSURFACE) + StretchBlt
//  pair standing in 1:1 for TCanvas.FloodFill + TCanvas.StretchDraw (plan
//  §6-W7-C2's own mapping table).
//
//  Only 4 of the 6 styles have any live .dfm instance: LEDHorizontal 1,357 /
//  LEDSqLarge 459 / LEDVertical 23 / LEDLarge 1 = 1,840 total. LEDSmall and
//  LEDSqSmall are the dead pair (0 instances between them). AI(W906-W7-C-
//  followup) 20260728: counts re-derived directly from all 133 golden .dfm
//  files, replacing an unedited draft sentence that contradicted itself
//  mid-clause about which two styles were dead.
//  All 6 are implemented anyway (the resource data
//  + algorithm are generically table-driven, not per-style special-cased --
//  see the GOLDEN QUIRK note in LedRender.cpp about LEDSqSmall's bitmap
//  actually being a different native size than its declared Width/Height,
//  discovered while embedding this table) so there is no extra invented
//  code path for the live 4 vs. the dead 2, just data most callers happen
//  never to reach.
//
//  AI(W906-W7-C2) 20260728: corrected the CreateLedBitmap summary above --
//  the flood-fill seed is the CONTROL's Width/Height, not the bitmap's
//  native size (see LedRender.cpp for the LOW-1 fix and full explanation).
// ===========================================================================
#ifndef VCLCOMPAT_RENDER_LEDRENDER_H
#define VCLCOMPAT_RENDER_LEDRENDER_H

#include "vclcompat/render/RenderCommon.h"
#include "vclcompat/LedCore.h"

namespace vclcompat {
namespace render {

// Stretch-blits the LED art for `core`'s current LEDStyle/ColorTemp/
// TrueColor/FalseColor into `destRect` (device coordinates). In every real
// .dfm instance destRect is core.Width x core.Height (verified §4-V12/
// KNOWLEDGE gotcha #11: 0 of 1,840 instances actually override the .dfm
// geometry away from what ApplyStyleExtents would produce) so this never
// actually stretches in practice -- but it genuinely WOULD if asked to
// (matching golden's own StretchDraw, which always stretches to whatever
// target rect it is given, regardless of source size).
void RenderLed(HDC hdc, const RECT& destRect, const LedCore& core);

// Convenience overload: renders at (x,y) using the core's own natural
// Width/Height, matching golden Paint's `Rect(0,0,Width,Height)` (control-
// local target rect -- caller supplies the screen/parent-relative origin).
void RenderLed(HDC hdc, int x, int y, const LedCore& core);

} // namespace render
} // namespace vclcompat
#endif // VCLCOMPAT_RENDER_LEDRENDER_H
