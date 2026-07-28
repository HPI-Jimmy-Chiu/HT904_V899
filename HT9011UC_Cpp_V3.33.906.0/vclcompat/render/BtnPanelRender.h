// ===========================================================================
//  vclcompat/render/BtnPanelRender.h
//  HDC renderer for vclcompat::BtnPanelCoreBase (TBtnPanel/TBtnPanelLane,
//  W7-C2).
//
//  TBtnPanel/TBtnPanelLane have ZERO custom Paint code in golden --
//  butPa1.cpp/BtnPanelLane.cpp grep for Canvas|Paint|DrawText|Rectangle|
//  Ellipse|StretchDraw scores 0 (see BtnPanelCore.h's own W7-C1 banner).
//  Their entire visible appearance is the INHERITED stock TPanel: a flat
//  Color fill, plus (in a real running VCL/Win32 app) a themed 3-D bevel
//  driven by BevelInner/BevelOuter.
//
//  This renderer paints ONLY the fill -- the part that is genuinely
//  golden-authored, observable state (BtnPanelCoreBase::Color, driven by
//  SetTrueColor/SetFalseColor/SetDown). It deliberately does NOT paint a
//  bevel: TPanel's bevel is generic VCL/Win32 chrome with no golden-specific
//  code behind it at all, its exact pixels depend on the live OS theme
//  (classic vs. themed edge shading differs by Windows version, and even
//  Windows' OWN look changed this multiple times across releases), and
//  there is no golden pixel oracle to translate it against -- painting one
//  here would be INVENTING a visual golden does not itself draw, which this
//  wave's brief explicitly forbids ("be careful not to invent visuals that
//  golden does not have").
//
//  The natural home for a faithful bevel is the eventual W7-C5 MFC shell
//  (CBtnPanelCtrl : CStatic + SS_NOTIFY, plan §7-6), which can call the real
//  Win32 DrawEdge() against BevelInner/BevelOuter as thin adapter code that
//  copies stock Win32/CStatic behaviour -- not something this framework-free
//  wave should predict or invent ahead of time.
//
//  AI(W906-W7-C2) 20260728: added this file's convention tag (LOW-3 audit
//  -- no functional change).
// ===========================================================================
#ifndef VCLCOMPAT_RENDER_BTNPANELRENDER_H
#define VCLCOMPAT_RENDER_BTNPANELRENDER_H

#include "vclcompat/render/RenderCommon.h"
#include "vclcompat/BtnPanelCore.h"

namespace vclcompat {
namespace render {

// Fills `rc` (device coordinates, caller-supplied -- this core carries no
// Width/Height/extent fields of its own, unlike Tray/Led, since golden's
// TPanel-inherited extent was never part of butPa1.h/BtnPanelLane.h's own
// declared surface) with core.Color. No border, no bevel -- see banner
// above for why.
void RenderBtnPanel(HDC hdc, const RECT& rc, const BtnPanelCoreBase& core);

} // namespace render
} // namespace vclcompat
#endif // VCLCOMPAT_RENDER_BTNPANELRENDER_H
