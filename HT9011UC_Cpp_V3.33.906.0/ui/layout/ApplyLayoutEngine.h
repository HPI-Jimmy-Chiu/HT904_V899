// ===========================================================================
//  ui/layout/ApplyLayoutEngine.h -- D7 pixel-authority layout pass
//  (docs/DESIGN_GA4_UI_ENGINES.md D-GA4-2/-4/-5/-6).
//
//  AI(W906-GA4) 20260804: NEW FILE, hand-authored contract (main loop).
//
//  THE ONE RULE: template coordinates are untrustworthy (pixel numbers were
//  written into DLU fields, so the dialog manager scaled everything by the
//  dialog-font factor at creation). This pass rewrites geometry for every
//  window that has a layout row, in .dfm pixels, relative to its host
//  dialog's client area:
//    * LEAF / TABHOST rows with has_left..has_height -> SetWindowPos;
//    * SUBDLG rows (the child dialogs themselves)     -> ditto, EXCEPT
//      TTabSheet dialogs (positioned by DialogTreeEngine via TCM_ADJUSTRECT);
//    * ROOT -> client sized to UiMapForm::root_client_w/h through
//      AdjustWindowRectEx(actual GetWindowLong styles), positioned at the
//      ROOT row's left/top when present;
//    * children WITHOUT a layout row (synthesized frames / radio items,
//      D-GA4-4) -> proportional rescale from the host's pre-resize client to
//      its post-resize client, applied per host dialog;
//    * per-control fonts (has_font) -> FormRuntime font cache
//      ("face|height|style" key), CreateFontIndirectW (UTF-8 face -> wide,
//      lfHeight = font_height_px VERBATIM per the schema's sign rule),
//      WM_SETFONT; rows without a font keep the template's DS_SETFONT font;
//    * visible == 0 -> SW_HIDE (visible -1 means VCL default True);
//    * z-order: template order is kept as-is; when rt.ZOrderFlip() the
//      sibling chain of every dialog is reversed once with SetWindowPos
//      (GA-5's --zorder polarity experiment, W7 plan section 7 item 10).
//
//  Align/Anchors are deliberately NOT interpreted in Gate A (static .dfm
//  geometry only; fMain is a fixed-size tool window). Recorded in
//  docs/W7-UI-SKIPPED.md, not silently dropped.
// ===========================================================================
#ifndef HT9045_UI_LAYOUT_APPLYLAYOUTENGINE_H
#define HT9045_UI_LAYOUT_APPLYLAYOUTENGINE_H

#include "FormRuntime.h"

namespace ht9045_ui {

// Run the whole pass over rt's built tree (BuildDialogTree must have run).
// Returns the number of windows whose geometry was rewritten -- the probe
// asserts this against the table's has_left population, so DO NOT silently
// skip rows: a row that should move but can't (no HWND found) must be
// BootLog'd and still counted in the returned struct's `missing` field.
struct ApplyLayoutStats {
    int moved;        // windows repositioned from their layout row
    int fonts_set;    // WM_SETFONT applied
    int hidden;       // visible==0 rows hidden
    int rescaled;     // D-GA4-4 proportional fixups (synthesized children)
    int missing;      // rows with geometry but no live HWND (should be 0)
};
ApplyLayoutStats ApplyLayoutPass(FormRuntime& rt);

} // namespace ht9045_ui

#endif // HT9045_UI_LAYOUT_APPLYLAYOUTENGINE_H
