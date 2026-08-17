// ===========================================================================
//  ui/layout/CtlColorEngine.h -- table-driven WM_CTLCOLOR* + TColor resolve
//  (docs/DESIGN_GA4_UI_ENGINES.md D-GA4-7).
//
//  AI(W906-GA4) 20260804: NEW FILE, hand-authored contract (main loop).
//
//  Delphi TColor semantics (DfmLayoutTypes.h note 4): the resolved values in
//  the layout table are genuine signed 32-bit TColors. A negative value is a
//  system-colour reference and MUST be resolved at paint time --
//  GetSysColor(v & 0xFF) -- never pre-baked (theme changes). A non-negative
//  value's low 24 bits are already a COLORREF (both are 0x00BBGGRR).
//
//  Routing contract: every dialog proc (the shared child DLGPROC and the
//  root CDialog's OnCtlColor) forwards to CtlColorRoute. Return value:
//    non-NULL -> the brush to paint the control's background with (the dc
//                has been SetTextColor/SetBkColor'd as the row demands);
//    NULL     -> caller falls through to the default handler.
//  Custom HT9045.* controls paint their whole client themselves and are
//  never routed here (their class brush is NULL by design).
// ===========================================================================
#ifndef HT9045_UI_LAYOUT_CTLCOLORENGINE_H
#define HT9045_UI_LAYOUT_CTLCOLORENGINE_H

#include "FormRuntime.h"

namespace ht9045_ui {

// TColor -> COLORREF (paint-time system-colour resolve). Exposed for the
// attach pass and the probe; keep it the single definition of the rule.
COLORREF ResolveTColor(int tcolor);

// nCtlColorType is the MFC CTLCOLOR_* constant (CTLCOLOR_STATIC, _BTN, _DLG,
// _EDIT, _LISTBOX). hCtl is the control (for _DLG it equals the dialog).
HBRUSH CtlColorRoute(FormRuntime& rt, HDC hdc, HWND hCtl, int nCtlColorType);

} // namespace ht9045_ui

#endif // HT9045_UI_LAYOUT_CTLCOLORENGINE_H
