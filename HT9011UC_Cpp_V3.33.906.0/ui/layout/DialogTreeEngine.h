// ===========================================================================
//  ui/layout/DialogTreeEngine.h -- D8 nested-DIALOGEX mounting engine
//  (docs/DESIGN_GA4_UI_ENGINES.md D-GA4-3).
//
//  AI(W906-GA4) 20260804: NEW FILE, hand-authored contract (main loop).
//
//  Builds the child-dialog tree for one form: every non-root UiMapDialog row
//  becomes a CreateDialogParamW(MAKEINTRESOURCEW(idd_value)) window, mounted
//  per D-GA4-3:
//    * literal parent is a TABHOST (TPageControl)  -> window parent = that
//      SysTabControl32 HWND, TCM_INSERTITEMW a tab (caption from uimap,
//      UTF-8 -> UTF-16), geometry = tab-control client through
//      TCM_ADJUSTRECT, initial visibility from UiMapTabHost::active_page_path;
//    * anything else -> window parent = host_dialog_path's HWND; geometry is
//      ApplyLayout's job (D-GA4-2), not this engine's.
//  Creation follows dialogs[] order, which is pre-order (parents first) --
//  guaranteed by the generator, asserted here.
//
//  The shared child DLGPROC lives in this TU. It stores the FormRuntime* in
//  DWLP_USER, returns FALSE from WM_INITDIALOG (no focus stealing while the
//  tree is built hidden), routes WM_CTLCOLOR* to CtlColorRoute, handles
//  TCN_SELCHANGE from nested TABHOSTs (show/hide the matching sheet
//  dialogs), and forwards WM_COMMAND up to the root window (GA-3's binder
//  will subscribe there; until then the root just BootLogs it).
// ===========================================================================
#ifndef HT9045_UI_LAYOUT_DIALOGTREEENGINE_H
#define HT9045_UI_LAYOUT_DIALOGTREEENGINE_H

#include "FormRuntime.h"

namespace ht9045_ui {

// Create all non-root dialogs under hRoot (the already-created root dialog,
// typically the owning MFC CDialog's m_hWnd). On success every UiMapDialog
// row has an HWND in rt (SetHwnd), every created control of every dialog is
// path-indexed too (via the uimap controls[] join), and tab items are
// inserted + initial pages shown/hidden. Returns false on the first dialog
// that fails to create (the failure is BootLog'd with its dfm_path; already
// created children are left for the root's normal destruction to reap).
bool BuildDialogTree(FormRuntime& rt, HWND hRoot);

// TCN_SELCHANGE helper, also used directly by the root dialog's WM_NOTIFY:
// shows the newly selected sheet's dialog, hides the others of that tab
// host. pageControlPath is the TABHOST node's dfm_path.
void OnTabSelChanged(FormRuntime& rt, const char* pageControlPath);

} // namespace ht9045_ui

#endif // HT9045_UI_LAYOUT_DIALOGTREEENGINE_H
