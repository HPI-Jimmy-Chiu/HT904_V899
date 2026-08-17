// =============================================================================
//  ui/forms/FMainFirstLightDlg.h -- GA-4 first-light root dialog for fMain
//
//  AI(W906-GA4) 20260804: NEW FILE. The MFC CDialog over the GENERATED
//  IDD_MAIN template (tools/dfm2rc/rc_out/main.rc, value 8192 in main_ids.h)
//  that replaces the disposable CGateAPlaceholderDlg as HT9045.exe's main
//  window. "First light" scope, deliberately narrow: prove that the whole
//  91-dialog fMain tree BUILDS (D-GA4-3), LAYS OUT in .dfm pixels (D-GA4-2),
//  gets its per-control fonts/colours (D-GA4-6/-7) and live custom controls
//  (D-GA4-8) -- with ZERO event bindings. Every WM_COMMAND is logged and
//  default-routed until GA-3's binder subscribes (D-GA4-3 last bullet).
//
//  WHY A CLASS THIS THIN: all real work lives in the four shared engines
//  (ui/layout/*.h) driven off the generated tables, because that machinery
//  must be reusable for all 133 forms (FormRuntime.h:10-13, the "write once"
//  lever). This dialog only ORCHESTRATES the documented lifecycle
//  (FormRuntime.h:15-24): Init -> BuildDialogTree -> ApplyLayoutPass ->
//  AttachCustomControls, then routes the three message families the root
//  window alone must route (OnCtlColor / root-level TCN_SELCHANGE /
//  WM_COMMAND bubble-ups).
//
//  OWNERSHIP: FormRuntime is a by-value member -- one live form instance =
//  one FormRuntime (FormRuntime.h:9-13). rt.Destroy() runs in OnDestroy()
//  BEFORE CDialog::OnDestroy(): the shells it owns must UnsubclassWindow
//  while their HWNDs still exist (WM_DESTROY reaches the parent before its
//  children are destroyed, so the whole child tree is still alive inside our
//  OnDestroy -- unsubclassing any later would leave the shells pointing at
//  dead windows; FormRuntime.h:60-63 contract).
//
//  CLOSE BEHAVIOUR: OnCancel/OnClose are NOT overridden -- CDialog's default
//  (ESC / [X] -> IDCANCEL -> EndDialog) is exactly the "allowed to close"
//  first-light behaviour. Golden fMain guards close with confirmation logic
//  (main.cpp FormCloseQuery); that arrives with GA-3's bindings, not here.
//
//  CTOR FLAGS (all plumbed from HT9045App command-line switches; the
//  integration is proposed in docs/_ga4_integration_proposal.md -- this
//  agent edits no existing file, D-GA4-10):
//    smokeCloseMs   > 0 -> EndDialog(IDOK) after that many ms (--smoke).
//    zorderFlip     --zorder: FormRuntime::SetZOrderFlip, D-GA4-5's one-shot
//                   sibling-chain reversal experiment for GA-5.
//    centerOnScreen --center: CenterWindow() AFTER the layout pass, a
//                   dev-only nicety in the spirit of --devpath (D-GA4-2:
//                   golden fMain sits at .dfm Left/Top 1057/489, off-screen
//                   on small dev monitors).
//
//  MSVC+MFC only (compiled solely inside CMake's if(HT9045_UI) block, same
//  rule as ui/HT9045App.h:18-20).
// =============================================================================
#ifndef FMainFirstLightDlg_H
#define FMainFirstLightDlg_H

#include <afxwin.h>
#include "main_ids.h"      // generated numeric IDs: IDD_MAIN (rc_out on include path)
#include "FormRuntime.h"   // ui/layout shared runtime (owns fonts/brushes/shells)

class CFMainFirstLightDlg : public CDialog
{
public:
    CFMainFirstLightDlg(int smokeCloseMs, bool zorderFlip, bool centerOnScreen);

    enum { IDD = IDD_MAIN };

protected:
    virtual BOOL OnInitDialog();
    // Virtual overrides (not message-map entries): OnCommand/OnNotify are
    // CWnd virtuals in MFC's routing, the natural interception point BEFORE
    // any map lookup -- there is nothing bound in a map to look up yet.
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    afx_msg void   OnTimer(UINT_PTR nIDEvent);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void   OnDestroy();
    DECLARE_MESSAGE_MAP()

private:
    ht9045_ui::FormRuntime m_rt;   // one live form = one FormRuntime (FormRuntime.h:9)

    int  m_iSmokeCloseMs;    // 0 = interactive
    bool m_bZOrderFlip;
    bool m_bCenterOnScreen;

    // Timer id 1 = the Gate A smoke convention (same id the placeholder used,
    // GateAPlaceholderDlg.h:30). 0x9045 is reserved by CLedCtrl's blink timer
    // (CLedCtrl.h:78) -- timers are per-HWND so a clash could not actually
    // occur (LEDs blink on their own HWNDs), but keeping the ids disjoint
    // keeps a debugger trace unambiguous, which is the reason CLedCtrl's own
    // banner gives for avoiding 1.
    static const UINT_PTR kSmokeTimerId = 1;
};

#endif // FMainFirstLightDlg_H
