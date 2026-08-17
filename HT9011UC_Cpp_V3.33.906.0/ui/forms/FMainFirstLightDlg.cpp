// =============================================================================
//  ui/forms/FMainFirstLightDlg.cpp -- see FMainFirstLightDlg.h for the design
//  rationale. AI(W906-GA4) 20260804: NEW FILE.
// =============================================================================
#include "FMainFirstLightDlg.h"

#include <commctrl.h>     // TCN_SELCHANGE (afxwin.h does not pull it in)
#include <stdio.h>        // sprintf_s for the BootLog stat lines
#include <string.h>       // strcmp for the layout-row kind check

#include "DialogTreeEngine.h"
#include "ApplyLayoutEngine.h"
#include "CtlColorEngine.h"
#include "CustomCtrlAttach.h"
#include "main_layout.gen.h"    // ht9045_layout::kmainLayout / kmainLayoutCount
#include "main_uimap.gen.h"     // ht9045_uimap::kmainUiMap (D-GA4-1 join table)

#include "Public/cBootLog.h"    // WriteBootLog -- the smoke test's oracle

BEGIN_MESSAGE_MAP(CFMainFirstLightDlg, CDialog)
    ON_WM_TIMER()
    ON_WM_CTLCOLOR()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

CFMainFirstLightDlg::CFMainFirstLightDlg(int smokeCloseMs, bool zorderFlip,
                                         bool centerOnScreen)
    : CDialog(IDD, NULL)
    , m_iSmokeCloseMs(smokeCloseMs)
    , m_bZOrderFlip(zorderFlip)
    , m_bCenterOnScreen(centerOnScreen)
{
}

// -----------------------------------------------------------------------------
// OnInitDialog -- the whole first-light lifecycle, in the exact order the
// FormRuntime contract prescribes (FormRuntime.h:15-24). Ordering is
// load-bearing at every step:
//   * BuildDialogTree before ApplyLayoutPass: the layout pass needs every
//     HWND indexed (it walks rt's path->HWND map), and the tab sheets must
//     already sit in their TCM_ADJUSTRECT display areas (D-GA4-3).
//   * ApplyLayoutPass before AttachCustomControls: attach pushes LED/tray
//     props and triggers repaints; painting into pre-layout DLU-scaled rects
//     would waste work and, worse, let a probe read back stale geometry.
//   * CenterWindow AFTER layout: ApplyLayoutPass resizes the root frame via
//     AdjustWindowRectEx from the uimap client size (D-GA4-2); centering the
//     template-DLU-inflated frame first would center the wrong rectangle.
// Each pass BootLogs ONE pipe-separated summary line -- BootLog is the smoke
// test's oracle, so these numbers (not just "OK") are the observable output
// of a --smoke run (D-GA4-9's stats cross-check for runs without the probe).
// -----------------------------------------------------------------------------
BOOL CFMainFirstLightDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_rt.Init(&ht9045_uimap::kmainUiMap,
              ht9045_layout::kmainLayout,
              ht9045_layout::kmainLayoutCount,
              AfxGetInstanceHandle());
    m_rt.SetZOrderFlip(m_bZOrderFlip);   // before the layout pass reads it (D-GA4-5)

    // BuildDialogTree indexes the ROOT row itself (SetRoot + SetHwnd for
    // "fMain"), so no SetRoot call here -- handing it m_hWnd is the entire
    // handover from MFC-land to the engine-land tree build.
    if (!ht9045_ui::BuildDialogTree(m_rt, m_hWnd))
    {
        // The engine already BootLog'd the failing dfm_path (DialogTreeEngine.h
        // contract); this line marks the consequence at the orchestration level.
        // IDABORT (not IDCANCEL) so a smoke run's exit path is distinguishable
        // from a user close in the DoModal return value.
        WriteBootLog("FMain BuildDialogTree FAILED -- aborting first light");
        EndDialog(IDABORT);
        return TRUE;
    }
    {
        char szBuf[96];
        sprintf_s(szBuf, sizeof(szBuf), "dialogs=%d|controls=%d|tabhosts=%d",
                  m_rt.Map()->dialog_count, m_rt.Map()->control_count,
                  m_rt.Map()->tabhost_count);
        WriteBootLog("FMain BuildDialogTree OK", szBuf);
    }

    {
        const ht9045_ui::ApplyLayoutStats st = ht9045_ui::ApplyLayoutPass(m_rt);
        char szBuf[128];
        sprintf_s(szBuf, sizeof(szBuf),
                  "moved=%d|fonts_set=%d|hidden=%d|rescaled=%d|missing=%d",
                  st.moved, st.fonts_set, st.hidden, st.rescaled, st.missing);
        WriteBootLog("FMain ApplyLayoutPass", szBuf);
    }

    {
        const ht9045_ui::AttachStats st = ht9045_ui::AttachCustomControls(m_rt);
        char szBuf[128];
        sprintf_s(szBuf, sizeof(szBuf),
                  "leds=%d|btnpanels=%d|trays=%d|missing=%d",
                  st.leds, st.btnpanels, st.trays, st.missing);
        WriteBootLog("FMain AttachCustomControls", szBuf);
    }

    if (m_bCenterOnScreen)
        CenterWindow();     // --center dev nicety (see header banner / D-GA4-2)

    if (m_iSmokeCloseMs > 0)
        SetTimer(kSmokeTimerId, (UINT)m_iSmokeCloseMs, NULL);

    return TRUE;    // we did not move focus to a specific control
}

void CFMainFirstLightDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == kSmokeTimerId)
    {
        KillTimer(kSmokeTimerId);
        EndDialog(IDOK);    // smoke run: open -> build -> layout -> close, no human
        return;
    }
    CDialog::OnTimer(nIDEvent);
}

// -----------------------------------------------------------------------------
// WM_CTLCOLOR* for controls hosted DIRECTLY on the root dialog. Child dialogs'
// controls send their WM_CTLCOLOR to their own (engine-built) dialog, whose
// shared DLGPROC forwards to the same CtlColorRoute (D-GA4-7's "every dialog
// proc forwards" routing contract, CtlColorEngine.h:12-15) -- so this override
// and the child proc are two entry points into ONE rule table. MFC's UINT
// nCtlColor is already the CTLCOLOR_* constant CtlColorRoute expects.
// NULL from the engine = "row has no colour spec" -> fall through to the
// default MFC handling, exactly per the engine's return contract.
// -----------------------------------------------------------------------------
HBRUSH CFMainFirstLightDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH b = ht9045_ui::CtlColorRoute(m_rt, pDC->GetSafeHdc(),
                                        pWnd->GetSafeHwnd(), (int)nCtlColor);
    return b ? b : CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

// -----------------------------------------------------------------------------
// WHY THIS OVERRIDE EXISTS: a SysTabControl32 sends TCN_SELCHANGE via
// WM_NOTIFY to its PARENT window. For every NESTED tab host (e.g.
// fMain...pgMotionView, which lives inside the pnlBackground child dialog)
// that parent is an engine-built child dialog whose shared DLGPROC already
// routes it (D-GA4-3). But pgMain is a direct child of the ROOT template
// (main.rc IDD_MAIN's only control; layout row "fMain.pgMain" has
// host_dialog_path "fMain"), so ITS notifications land on this CDialog and
// nowhere else -- without this override, clicking the two top-level tabs
// (tsMain/tsMotionView) would never switch pages.
//
// Guards, in order:
//   * code check first (cheapest),
//   * direct-child check: only route notifications whose sender is parented
//     by this root -- a nested tabhost's TCN_SELCHANGE belongs to its own
//     dialog's proc, and although none should ever arrive here, silently
//     double-routing one would show/hide sheets twice;
//   * kind check: RowByHwnd + kind=="TABHOST" proves the sender is one of
//     ours before its dfm_path is handed to the engine (a foreign/future
//     tab control would miss the map and fall through harmlessly).
// -----------------------------------------------------------------------------
BOOL CFMainFirstLightDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    const NMHDR* pnmh = reinterpret_cast<const NMHDR*>(lParam);
    if (pnmh != NULL && pnmh->code == TCN_SELCHANGE &&
        ::GetParent(pnmh->hwndFrom) == m_hWnd)
    {
        const ht9045_layout::DfmControlLayout* pRow = m_rt.RowByHwnd(pnmh->hwndFrom);
        if (pRow != NULL && strcmp(pRow->kind, "TABHOST") == 0)
        {
            ht9045_ui::OnTabSelChanged(m_rt, pRow->dfm_path);
            *pResult = 0;
            return TRUE;
        }
    }
    return CDialog::OnNotify(wParam, lParam, pResult);
}

// -----------------------------------------------------------------------------
// WM_COMMAND: nothing is bound until GA-3's binder subscribes. This receives
// BOTH the root template's own controls AND the bubble-ups the shared child
// DLGPROC forwards from all 90 child dialogs (D-GA4-3 last bullet: "the root
// just BootLogs it"). Logging THEN default-routing keeps ESC/[X] alive:
// IDCANCEL still reaches CDialog::OnCommand -> OnCancel -> close (the
// documented first-light close behaviour, header banner).
// -----------------------------------------------------------------------------
BOOL CFMainFirstLightDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
    char szBuf[64];
    sprintf_s(szBuf, sizeof(szBuf), "id=%u code=%u",
              (unsigned)LOWORD(wParam), (unsigned)HIWORD(wParam));
    WriteBootLog("FMain WM_COMMAND (unbound until GA-3)", szBuf);
    return CDialog::OnCommand(wParam, lParam);
}

// -----------------------------------------------------------------------------
// rt.Destroy() BEFORE the base call: FormRuntime's shells must
// UnsubclassWindow while the HWND tree is still alive (FormRuntime.h:60-63).
// WM_DESTROY is delivered parent-first, so every child HWND still exists
// right here; after CDialog::OnDestroy the children go down and a later
// unsubclass would operate on dead windows. Destroy() is documented
// safe-to-call-twice, so the destructor's own Destroy() re-run is harmless.
// -----------------------------------------------------------------------------
void CFMainFirstLightDlg::OnDestroy()
{
    m_rt.Destroy();
    CDialog::OnDestroy();
}
