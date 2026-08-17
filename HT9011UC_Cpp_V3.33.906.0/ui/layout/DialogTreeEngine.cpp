// ===========================================================================
//  ui/layout/DialogTreeEngine.cpp -- D8 nested-DIALOGEX mounting engine.
//  Contract: DialogTreeEngine.h; design: docs/DESIGN_GA4_UI_ENGINES.md
//  D-GA4-3 (and the visibility/geometry hand-off rules it shares with
//  D-GA4-2's ApplyLayoutEngine).
//
//  AI(W906-GA4) 20260804: NEW FILE (agent B). The decisions that are not
//  visible from the header alone:
//
//  * PARENT RESOLUTION (D-GA4-3). A child dialog's LITERAL parent is its own
//    layout row's parent_path. When that path's row is kind TABHOST, the
//    Win32 parent is the tab control's HWND (VCL: a TTabSheet's WinAPI
//    parent IS the TPageControl) -- verified against the generated table:
//    "fMain.pgMain.tsMain" has parent_path="fMain.pgMain" (TABHOST) but
//    host_dialog_path="fMain" (main_layout.gen.cpp:398), because note 2 of
//    DfmLayoutTypes.h defines host_dialog_path as "which template hosts this
//    node AS A CONTROL", which for a tab sheet is the dialog that hosts the
//    page control -- NOT the window parent. Everything else parents on
//    host_dialog_path's HWND.
//
//  * ORDERING. dialogs[] is rcmeta order = pre-order (UiFormMap.h:69),
//    asserted here two ways: dialogs[0] must be THE unique is_root row, and
//    every parent HWND lookup treats a miss as a hard error -- a pre-order
//    violation surfaces as exactly such a miss, because a parent dialog (or
//    the tab control indexed with its host's controls) would not have been
//    seen yet. Controls are indexed IMMEDIATELY after each dialog is
//    created, before the loop advances -- that is what makes "the tab
//    control is already indexed when its sheets arrive" true (pgMain is a
//    control OF fMain, indexed while handling the root row; the same holds
//    for the four nested hosts inside their SUBDLG owners).
//
//  * DIALOG-PROC RETURN CONVENTION for WM_CTLCOLOR*. DialogProc's contract:
//    most messages return their result through DWLP_MSGRESULT + TRUE, but a
//    documented exception list (WM_CHARTOITEM, WM_COMPAREITEM, WM_CTLCOLOR*,
//    WM_INITDIALOG, WM_QUERYDRAGICON, WM_VKEYTOITEM) has the dialog manager
//    take the DLGPROC's raw return value AS the message result. So the
//    HBRUSH is returned directly, cast to INT_PTR; returning 0/FALSE falls
//    through to default colouring. (Task brief demanded this be verified,
//    not guessed: it is the "However, if the dialog box procedure processes
//    a message from this list..." paragraph of the DialogProc docs.)
//
//  * SHEET GEOMETRY is computed from the tab control's CURRENT client rect
//    through TabCtrl_AdjustRect(FALSE) -- never from a layout row, because
//    .dfm writes no geometry for TTabSheet (has_* all false, D-GA4-3). The
//    five hosts are all single-row (no TCS_MULTILINE in the generated
//    templates, main.rc CONTROL lines carry style 0), so later inserts do
//    not change the display area and sheets fitted early stay valid DURING
//    the build. But ApplyLayoutPass runs AFTER this engine and rewrites the
//    TABHOST's pixel geometry (D-GA4-2), which stales the build-time fit;
//    OnTabSelChanged therefore RE-FITS a sheet every time it shows one
//    (recorded deviation -- see this file's report), and the initial
//    active sheet's post-layout fit is flagged for the integrator.
// ===========================================================================
#include "DialogTreeEngine.h"

#include "CtlColorEngine.h"     // CtlColorRoute -- D-GA4-7 routing target

#include <commctrl.h>           // SysTabControl32: TCITEMW / TCM_* / TabCtrl_*
#include <string.h>
#include <string>

#include "Public/cBootLog.h"    // WriteBootLog -- same channel as HT9045App.cpp

namespace ht9045_ui {

// ---------------------------------------------------------------------------
// UTF-8 -> UTF-16 for tab captions (uimap caption_utf8 carries real CJK,
// UiFormMap.h:41; the tab control is created from a W template, and we send
// TCM_INSERTITEMW explicitly so the text is wide end-to-end, D-GA4-3).
// ---------------------------------------------------------------------------
static std::wstring Utf8ToWide(const char* pszUtf8)
{
    std::wstring w;
    if (pszUtf8 == NULL || pszUtf8[0] == '\0')
        return w;
    const int n = ::MultiByteToWideChar(CP_UTF8, 0, pszUtf8, -1, NULL, 0);
    if (n <= 1)
        return w;                       // n counts the terminator; <=1 = empty/bad
    w.resize((size_t)n);                // include room for the terminator
    ::MultiByteToWideChar(CP_UTF8, 0, pszUtf8, -1, &w[0], n);
    w.resize((size_t)n - 1);            // drop the terminator from the string
    return w;
}

// ---------------------------------------------------------------------------
// Set / clear WS_VISIBLE without any activation semantics. "Show" in the
// D-GA4-3 tab rules means the style bit, never focus/activation (the whole
// tree is built under a hidden root, D-GA4-9, and must stay passive).
// SWP_NOACTIVATE + a NULL insert-after (SWP_NOZORDER) make that structural;
// SW_HIDE never activates by definition.
// ---------------------------------------------------------------------------
static void ShowSheetNoActivate(HWND hSheet, bool bShow)
{
    if (bShow)
        ::SetWindowPos(hSheet, NULL, 0, 0, 0, 0,
                       SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                       SWP_NOACTIVATE | SWP_SHOWWINDOW);
    else
        ::ShowWindow(hSheet, SW_HIDE);
}

// ---------------------------------------------------------------------------
// Fit a sheet dialog to its tab control's display area: client rect ->
// TabCtrl_AdjustRect(FALSE) -> MoveWindow. The result is in the tab
// control's CLIENT coordinates, which is exactly the sheet's coordinate
// space because the sheet's window parent IS the tab control (D-GA4-3).
// ---------------------------------------------------------------------------
static void FitSheetToTabDisplayArea(HWND hTab, HWND hSheet)
{
    RECT rc;
    ::GetClientRect(hTab, &rc);
    TabCtrl_AdjustRect(hTab, FALSE, &rc);
    ::MoveWindow(hSheet, rc.left, rc.top,
                 rc.right - rc.left, rc.bottom - rc.top, FALSE);
}

// ---------------------------------------------------------------------------
// Index every UiMapControl hosted by dialog `d` under its live HWND:
// GetDlgItem(hDlg, id_value) -> SetHwnd(source_path, h). SetHwnd also feeds
// the HWND->layout-row map when a row exists (FormRuntime.h:79), which is
// what arms CtlColorRoute / ApplyLayout / Attach for this dialog's controls.
//
// A NULL GetDlgItem for a NON-synthesized control is a build-stopping error
// (the template and the uimap disagree -- generator drift, nothing later
// passes could repair), BootLog'd with the path. A synthesized control
// (SYNTH_CONTAINER_FRAME / SYNTH_RADIOGROUP_ITEM) does have a real template
// entry, so a miss is equally anomalous -- but per the task contract only
// the non-synthesized miss stops the build; the synthesized miss is logged
// and skipped (it only starves the D-GA4-4 rescale set of one member).
// ---------------------------------------------------------------------------
static bool IndexDialogControls(FormRuntime& rt,
                                const ht9045_uimap::UiMapDialog& d,
                                HWND hDlg)
{
    const ht9045_uimap::UiMapForm* pMap = rt.Map();
    for (int i = 0; i < pMap->control_count; ++i)
    {
        const ht9045_uimap::UiMapControl& c = pMap->controls[i];
        if (::strcmp(c.host_dfm_path, d.dfm_path) != 0)
            continue;

        HWND h = ::GetDlgItem(hDlg, c.id_value);
        if (h == NULL)
        {
            if (!c.synthesized)
            {
                WriteBootLog("DialogTree control MISSING in template -- build stopped",
                             AnsiString().sprintf("%s (id=%d, host=%s)",
                                                  c.source_path, c.id_value, d.dfm_path));
                return false;
            }
            WriteBootLog("DialogTree synthesized control not found (skipped)",
                         AnsiString().sprintf("%s (id=%d, host=%s)",
                                              c.source_path, c.id_value, d.dfm_path));
            continue;
        }
        rt.SetHwnd(c.source_path, h);
    }
    return true;
}

// ---------------------------------------------------------------------------
// Shared child DLGPROC (D-GA4-3, header banner). One proc serves all 90
// child dialogs; per-dialog state is only the FormRuntime* parked in
// DWLP_USER by WM_INITDIALOG. Messages that arrive BEFORE WM_INITDIALOG
// (WM_SETFONT etc. during template instantiation) see a zero DWLP_USER and
// fall through to default -- correct, since nothing table-driven can happen
// before the runtime pointer is known.
// ---------------------------------------------------------------------------
static INT_PTR CALLBACK SharedChildProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_INITDIALOG)
    {
        // lParam is the (LPARAM)&rt from CreateDialogParamW below.
        ::SetWindowLongPtrW(hDlg, DWLP_USER, (LONG_PTR)lParam);
        return FALSE;   // FALSE = do not set default focus: the tree is being
                        // built hidden and must not steal focus (D-GA4-3).
    }

    FormRuntime* pRt = (FormRuntime*)::GetWindowLongPtrW(hDlg, DWLP_USER);
    if (pRt == NULL)
        return FALSE;

    switch (uMsg)
    {
    // ---- WM_CTLCOLOR* -> CtlColorRoute (D-GA4-7) --------------------------
    // Message -> MFC CTLCOLOR_* constant mapping done here because
    // CtlColorRoute's contract (CtlColorEngine.h:33) speaks CTLCOLOR_*.
    // These messages are on DialogProc's direct-return exception list: the
    // HBRUSH is the return value itself (see file banner), NOT DWLP_MSGRESULT.
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORDLG:
    {
        int nType;
        switch (uMsg)
        {
        case WM_CTLCOLORSTATIC:  nType = CTLCOLOR_STATIC;  break;
        case WM_CTLCOLORBTN:     nType = CTLCOLOR_BTN;     break;
        case WM_CTLCOLOREDIT:    nType = CTLCOLOR_EDIT;    break;
        case WM_CTLCOLORLISTBOX: nType = CTLCOLOR_LISTBOX; break;
        default:                 nType = CTLCOLOR_DLG;     break;
        }
        // wParam = HDC; lParam = control HWND (== the dialog for _DLG).
        HBRUSH hbr = CtlColorRoute(*pRt, (HDC)wParam, (HWND)lParam, nType);
        if (hbr != NULL)
            return (INT_PTR)hbr;    // direct-return convention (file banner)
        return FALSE;               // no row demands a colour: default handling
    }

    // ---- WM_NOTIFY: nested TABHOST page switch (D-GA4-3) ------------------
    // Only TCN_SELCHANGE is interesting, and only when hwndFrom IS one of
    // this form's indexed tab controls -- hwndFrom equality is exact, so
    // iterating all tabhosts is safe even though each dialog only ever
    // receives notifications from its own children.
    case WM_NOTIFY:
    {
        const NMHDR* pnmh = (const NMHDR*)lParam;
        if (pnmh != NULL && pnmh->code == TCN_SELCHANGE)
        {
            const ht9045_uimap::UiMapForm* pMap = pRt->Map();
            if (pMap != NULL)
            {
                for (int i = 0; i < pMap->tabhost_count; ++i)
                {
                    if (pRt->HwndByPath(pMap->tabhosts[i].dfm_path) == pnmh->hwndFrom)
                    {
                        OnTabSelChanged(*pRt, pMap->tabhosts[i].dfm_path);
                        ::SetWindowLongPtrW(hDlg, DWLP_MSGRESULT, 0);
                        return TRUE;    // handled; TCN_SELCHANGE ignores result
                    }
                }
            }
        }
        return FALSE;
    }

    // ---- WM_COMMAND: bubble to the root (D-GA4-3) -------------------------
    // GA-3's binder will subscribe at the root; until then the root only
    // BootLogs. Forwarded verbatim (wParam keeps notify-code+id, lParam the
    // control HWND) with SendMessage so ordering is preserved. The root
    // never uses this proc, so no forwarding loop is possible; the guard is
    // belt-and-braces against a future mis-wire.
    case WM_COMMAND:
    {
        HWND hRoot = pRt->Root();
        if (hRoot != NULL && hRoot != hDlg)
            ::SendMessageW(hRoot, WM_COMMAND, wParam, lParam);
        ::SetWindowLongPtrW(hDlg, DWLP_MSGRESULT, 0);
        return TRUE;
    }

    default:
        break;
    }
    return FALSE;
}

// ---------------------------------------------------------------------------
// Mount one TTabSheet dialog on its tab control: insert the TCITEMW at the
// sheet's sibling_index, apply the host's fixed tab width once (on the first
// insert), fit the sheet to the display area, and set initial visibility +
// selection per UiMapTabHost::active_page_path.
// ---------------------------------------------------------------------------
static bool MountTabSheet(FormRuntime& rt,
                          const ht9045_uimap::UiMapDialog& d,
                          const ht9045_layout::DfmControlLayout& row,
                          HWND hTab,
                          HWND hSheet)
{
    // --- tab item (caption from uimap: the ONLY carrier of TTabSheet
    //     captions -- neither the child template nor the layout table has
    //     them, UiFormMap.h banner) -----------------------------------------
    std::wstring wCaption = Utf8ToWide(d.caption_utf8);
    TCITEMW tie;
    ::ZeroMemory(&tie, sizeof(tie));
    tie.mask    = TCIF_TEXT;
    tie.pszText = const_cast<LPWSTR>(wCaption.c_str());

    // Insertion at sibling_index: sheets arrive in sibling order (pre-order
    // walk), so this equals "append", but using the table's index keeps the
    // item<->sheet correspondence (TabCtrl sel N == sheet sibling_index N,
    // which OnTabSelChanged relies on) true by construction, not by luck.
    if ((int)::SendMessageW(hTab, TCM_INSERTITEMW,
                            (WPARAM)row.sibling_index, (LPARAM)&tie) < 0)
    {
        WriteBootLog("DialogTree TCM_INSERTITEMW FAILED", AnsiString(d.dfm_path));
        return false;
    }

    const ht9045_uimap::UiMapTabHost* pHost = rt.TabHostByPath(row.parent_path);

    // --- fixed tab width (VCL TabWidth), once per host, right after the
    //     FIRST insert: item 0 now exists, so its rect yields the natural
    //     item height, which TCM_SETITEMSIZE must preserve (D-GA4-3 /
    //     task contract: width=tab_width in pixels -- VCL semantics -- and
    //     keep the current height). Applying it before any sheet is fitted
    //     also keeps the display-area math consistent for every sheet.
    if (TabCtrl_GetItemCount(hTab) == 1 && pHost != NULL && pHost->tab_width > 0)
    {
        const LONG lStyle = ::GetWindowLongW(hTab, GWL_STYLE);
        ::SetWindowLongW(hTab, GWL_STYLE, lStyle | TCS_FIXEDWIDTH);
        RECT rcItem = { 0, 0, 0, 0 };
        TabCtrl_GetItemRect(hTab, 0, &rcItem);
        TabCtrl_SetItemSize(hTab, pHost->tab_width, rcItem.bottom - rcItem.top);
    }

    // --- geometry: display area of the tab control, never a layout row
    //     (.dfm writes no TTabSheet geometry; D-GA4-3). Single-row hosts
    //     (no TCS_MULTILINE) mean later inserts leave this rect unchanged.
    FitSheetToTabDisplayArea(hTab, hSheet);

    // --- initial visibility: only the active page keeps WS_VISIBLE (the
    //     child templates are WS_VISIBLE by generation, main.rc:35, so the
    //     inactive sheets must be explicitly hidden). "" -> first sheet.
    bool bShow;
    if (pHost != NULL && pHost->active_page_path[0] != '\0')
        bShow = (::strcmp(pHost->active_page_path, d.dfm_path) == 0);
    else
        bShow = (row.sibling_index == 0);

    ShowSheetNoActivate(hSheet, bShow);

    // Keep the tab control's SELECTION on the shown sheet. VCL ActivePage
    // sets both the visible page and the selected tab; without this a
    // non-zero active page would render with tab 0 highlighted (desyncs
    // until the first user click). TCM_SETCURSEL is documented NOT to send
    // TCN_SELCHANGE, so this cannot re-enter OnTabSelChanged.
    if (bShow)
        TabCtrl_SetCurSel(hTab, row.sibling_index);

    return true;
}

// ---------------------------------------------------------------------------
// BuildDialogTree -- see DialogTreeEngine.h:33-40 for the external contract.
// ---------------------------------------------------------------------------
bool BuildDialogTree(FormRuntime& rt, HWND hRoot)
{
    const ht9045_uimap::UiMapForm* pMap = rt.Map();
    if (pMap == NULL || pMap->dialogs == NULL || pMap->dialog_count <= 0)
    {
        WriteBootLog("DialogTree BuildDialogTree: no uimap bound (Init not run?)");
        return false;
    }
    if (hRoot == NULL || !::IsWindow(hRoot))
    {
        WriteBootLog("DialogTree BuildDialogTree: invalid root HWND");
        return false;
    }

    // Pre-order assertion (header banner: "guaranteed by the generator,
    // asserted here"): the unique is_root row must be dialogs[0] -- pre-order
    // puts the tree's root first by definition, and UiFormMap.h:43 promises
    // exactly one root. Any other shape means the table is not the shape
    // this walk was designed for, so stop before creating anything.
    if (!pMap->dialogs[0].is_root)
    {
        WriteBootLog("DialogTree ORDER VIOLATION: dialogs[0] is not the root",
                     AnsiString(pMap->dialogs[0].dfm_path));
        return false;
    }
    for (int i = 1; i < pMap->dialog_count; ++i)
    {
        if (pMap->dialogs[i].is_root)
        {
            WriteBootLog("DialogTree ORDER VIOLATION: duplicate is_root row",
                         AnsiString(pMap->dialogs[i].dfm_path));
            return false;
        }
    }

    for (int i = 0; i < pMap->dialog_count; ++i)
    {
        const ht9045_uimap::UiMapDialog& d = pMap->dialogs[i];

        // ---- root row: the CALLER created it (MFC CDialog on IDD_<root>).
        // Register it and index its controls exactly like any other
        // dialog's -- pgMain (the top tab control) is one of THESE controls,
        // and every later tab-sheet mount depends on it being indexed now.
        if (d.is_root)
        {
            rt.SetRoot(hRoot);
            rt.SetHwnd(d.dfm_path, hRoot);
            if (!IndexDialogControls(rt, d, hRoot))
                return false;
            continue;
        }

        // ---- parent resolution (D-GA4-3, file banner) ---------------------
        const ht9045_layout::DfmControlLayout* pRow = rt.RowByPath(d.dfm_path);
        if (pRow == NULL)
        {
            // Every real dialog node is a real .dfm node with a layout row;
            // a miss means uimap and layout table drifted apart. Without the
            // row there is no parent_path/sibling_index -- unresolvable.
            WriteBootLog("DialogTree dialog has NO layout row -- build stopped",
                         AnsiString(d.dfm_path));
            return false;
        }

        const ht9045_layout::DfmControlLayout* pParentRow = rt.RowByPath(pRow->parent_path);
        if (pParentRow == NULL)
        {
            // Anomalous (parent_path should always name a real node) but not
            // fatal by itself: D-GA4-3's "otherwise" branch -- parent on the
            // host dialog -- is still well-defined. Logged, not hidden.
            WriteBootLog("DialogTree parent_path has no layout row (using host_dialog_path)",
                         AnsiString().sprintf("%s -> %s", d.dfm_path, pRow->parent_path));
        }
        const bool bTabSheet =
            (pParentRow != NULL && ::strcmp(pParentRow->kind, "TABHOST") == 0);

        HWND hParent;
        if (bTabSheet)
        {
            // The tab control was indexed when its HOST dialog's controls
            // were indexed (immediately after that dialog's creation), so a
            // miss here can only mean the pre-order guarantee broke: hard
            // error per the task contract.
            hParent = rt.HwndByPath(pRow->parent_path);
            if (hParent == NULL)
            {
                WriteBootLog("DialogTree tab control HWND not indexed (pre-order violated?) -- build stopped",
                             AnsiString().sprintf("%s -> %s", d.dfm_path, pRow->parent_path));
                return false;
            }
        }
        else
        {
            hParent = rt.HwndByPath(pRow->host_dialog_path);
            if (hParent == NULL)
            {
                WriteBootLog("DialogTree host dialog HWND not indexed (pre-order violated?) -- build stopped",
                             AnsiString().sprintf("%s -> %s", d.dfm_path, pRow->host_dialog_path));
                return false;
            }
        }

        // ---- create ---------------------------------------------------------
        HWND hDlg = ::CreateDialogParamW(rt.Inst(),
                                         MAKEINTRESOURCEW(d.idd_value),
                                         hParent,
                                         SharedChildProc,
                                         (LPARAM)&rt);
        if (hDlg == NULL)
        {
            WriteBootLog("DialogTree CreateDialogParamW FAILED -- build stopped",
                         AnsiString().sprintf("%s (idd=%d, err=%lu)",
                                              d.dfm_path, d.idd_value,
                                              (unsigned long)::GetLastError()));
            return false;   // children already created are reaped by the
                            // root's normal destruction (header contract)
        }
        rt.SetHwnd(d.dfm_path, hDlg);

        // ---- index this dialog's controls IMMEDIATELY (before the loop can
        // reach any child dialog): nested TABHOSTs (pgMotionView etc.) live
        // among these controls and their sheets follow in pre-order.
        if (!IndexDialogControls(rt, d, hDlg))
            return false;

        // ---- tab-sheet mounting (insert item / fixed width / fit / show) ---
        if (bTabSheet)
        {
            if (!MountTabSheet(rt, d, *pRow, hParent, hDlg))
                return false;
        }
        // Non-tab-sheet SUBDLGs keep their template position for now:
        // geometry is ApplyLayout's job (D-GA4-2), not this engine's.
    }

    return true;
}

// ---------------------------------------------------------------------------
// OnTabSelChanged -- see DialogTreeEngine.h:42-45. Sel N maps to the sheet
// whose sibling_index == N: MountTabSheet inserted every item AT its
// sibling_index, so the correspondence is exact and no ordering pass is
// needed here. The shown sheet is re-fitted to the CURRENT display area
// first, which also self-heals the fit after ApplyLayoutPass has moved or
// resized the tab control (file banner, SHEET GEOMETRY note).
// ---------------------------------------------------------------------------
void OnTabSelChanged(FormRuntime& rt, const char* pageControlPath)
{
    HWND hTab = rt.HwndByPath(pageControlPath);
    if (hTab == NULL)
    {
        WriteBootLog("DialogTree OnTabSelChanged: tab control not indexed",
                     AnsiString(pageControlPath ? pageControlPath : "(null)"));
        return;
    }
    const int nSel = TabCtrl_GetCurSel(hTab);
    if (nSel < 0)
        return;             // no selection (empty host) -- nothing to show

    const ht9045_layout::DfmControlLayout* pRows = rt.Rows();
    const int nRows = rt.RowCount();
    for (int i = 0; i < nRows; ++i)
    {
        const ht9045_layout::DfmControlLayout& r = pRows[i];
        if (::strcmp(r.kind, "SUBDLG") != 0)
            continue;
        if (::strcmp(r.parent_path, pageControlPath) != 0)
            continue;       // only THIS host's TTabSheet children

        HWND hSheet = rt.HwndByPath(r.dfm_path);
        if (hSheet == NULL)
        {
            WriteBootLog("DialogTree OnTabSelChanged: sheet not indexed (skipped)",
                         AnsiString(r.dfm_path));
            continue;
        }
        if (r.sibling_index == nSel)
        {
            FitSheetToTabDisplayArea(hTab, hSheet);
            ShowSheetNoActivate(hSheet, true);
        }
        else
        {
            ShowSheetNoActivate(hSheet, false);
        }
    }
}

} // namespace ht9045_ui
