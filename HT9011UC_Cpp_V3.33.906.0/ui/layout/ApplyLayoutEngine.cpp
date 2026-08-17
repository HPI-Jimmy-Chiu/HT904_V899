// ===========================================================================
//  ui/layout/ApplyLayoutEngine.cpp -- D7 pixel-authority layout pass
//  (docs/DESIGN_GA4_UI_ENGINES.md D-GA4-2 / -4 / -5 / -6).
//
//  AI(W906-GA4) 20260804: NEW FILE (agent C). Implements ApplyLayoutEngine.h
//  exactly; runs after BuildDialogTree (FormRuntime.h lifecycle) on a tree
//  that is built hidden on purpose (D-GA4-9) -- nothing here may SW_SHOW.
//
//  WHY THE PASS ORDER IS LOAD-BEARING (D-GA4-2 + D-GA4-4):
//    pass 1  snapshot   -- old client rect of EVERY dialog (root + SUBDLGs),
//                          taken BEFORE any pixel correction. The generated
//                          templates have .dfm PIXEL numbers written into DLU
//                          fields, so the dialog manager scaled the whole tree
//                          by the dialog-font factor at creation (D-GA4-2:
//                          "模板幾何不可信"). This snapshot is the only record
//                          of that wrong-but-self-consistent geometry, which
//                          pass 4 needs as the rescale source space.
//    pass 2  root       -- client sized to UiMapForm::root_client_w/h through
//                          AdjustWindowRectEx with the window's ACTUAL styles:
//                          the .dfm stores only ClientWidth/ClientHeight (the
//                          uimap exists partly to carry them, UiFormMap.h
//                          banner), so the outer size must be derived, never
//                          guessed. Position = ROOT row left/top (faithful
//                          placement; the --center convenience flag lives
//                          with the app shell, not in this engine).
//    pass 3  row pass   -- every LEAF/TABHOST/SUBDLG row with has_left+has_top
//                          is SetWindowPos'd in verbatim .dfm pixels, in its
//                          HOST dialog's client space -- which is the window's
//                          parent client space for every row this pass
//                          touches (D-GA4-3 parenting: LEAF/TABHOST parent is
//                          their host dialog; a non-sheet SUBDLG's parent is
//                          host_dialog_path's HWND). Skips: the ROOT row
//                          (pass 2 owns it), TTabSheet SUBDLGs (D-GA4-3:
//                          DialogTreeEngine placed sheets via TCM_ADJUSTRECT;
//                          their layout rows carry no geometry anyway), and
//                          NONVISUAL rows -- those DO carry has_left/has_top
//                          (the .dfm stores the design-surface icon position
//                          for TTimer & friends, e.g. main_layout.gen.cpp's
//                          "fMain.Timer1" row: left=680, top=65532) but have
//                          no HWND by design (D-GA4-9 check 2), so counting
//                          them would poison `missing`.
//    pass 4  rescale    -- D-GA4-4: direct children with NO layout row (the
//                          synthesized SYNTH_CONTAINER_FRAME / _RADIOGROUP_
//                          ITEM controls) ate the DLU scaling at creation and
//                          were not touched by pass 3; per host dialog, map
//                          their rects proportionally from the pass-1 client
//                          to the post-correction client (MulDiv). Must run
//                          AFTER passes 2+3 so "new client" includes every
//                          correction the host received.
//    pass 5  fonts      -- D-GA4-6: per-row CreateFontIndirectW via the
//                          FormRuntime cache ("face|height|style" key,
//                          FormRuntime.h:87), WM_SETFONT with lParam=FALSE
//                          (tree is hidden; one repaint happens at show time).
//    pass 6  visibility -- visible==0 -> SW_HIDE. visible==-1 is
//                          property-absent = VCL default True
//                          (DfmLayoutTypes.h note 3) -> do nothing; never
//                          SW_SHOW anything (D-GA4-9's no-popup guarantee is
//                          structural, this engine must not break it).
//    pass 7  z-order    -- D-GA4-5: template order (= .dfm declaration order,
//                          generator保序) is the default polarity; only when
//                          rt.ZOrderFlip() is the sibling chain of every
//                          dialog reversed once (GA-5's --zorder polarity
//                          experiment, W7 plan section 7 item 10).
//
//  PROBE ACCOUNTING CONTRACT (ApplyLayoutEngine.h: "no silent skips"):
//    moved + missing == count of rows passing the pass-3 filter, i.e.
//      kind in {LEAF, TABHOST, SUBDLG}  AND  has_left AND has_top
//      AND NOT (kind==SUBDLG AND vcl_class=="TTabSheet").
//    The ROOT row is NOT in that population (its geometry is pass 2, driven
//    by uimap client size + row left/top) -- the probe must exclude it too.
//    A row whose SetWindowPos fails outright (live HWND, API said no) is
//    BootLog'd and still counted in `moved`: `missing` is defined as "no
//    live HWND" (ApplyLayoutEngine.h:51) and inventing a third bucket would
//    break the probe's two-term equation.
//    fonts_set / hidden / rescaled count successful applications only; their
//    failure paths (font row with no HWND, CreateFontIndirectW failure) are
//    BootLog'd -- dfm_path is globally unique per form (DfmLayoutTypes.h:128),
//    so one log line per row IS "once per path".
// ===========================================================================
#include "ApplyLayoutEngine.h"

#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "Public/cBootLog.h"   //AI(W906-GA4) 20260804: WriteBootLog for the
                               // no-silent-skip contract -- same diagnostics
                               // channel ui/HT9045App.cpp already uses.

namespace ht9045_ui {

namespace {

// ---------------------------------------------------------------------------
// Small helpers (file-local; the engine itself is a single free function per
// the FormRuntime.h "engines hold NO global state" rule).
// ---------------------------------------------------------------------------

bool IsKind(const ht9045_layout::DfmControlLayout* pRow, const char* pszKind)
{
    return ::strcmp(pRow->kind, pszKind) == 0;
}

// font_style_bits is a comma-joined Pascal SET dump ("fsBold,fsItalic"; "" =
// empty set, DfmLayoutTypes.h:172-173). None of the four member names is a
// substring of another (fsBold / fsItalic / fsUnderline / fsStrikeOut), so a
// plain strstr IS an exact membership test -- no tokenizer needed.
bool StyleHas(const char* pszBits, const char* pszToken)
{
    return pszBits != NULL && ::strstr(pszBits, pszToken) != NULL;
}

// Dialog row -> HWND. BuildDialogTree indexes every dialog it CREATES via
// SetHwnd, but the root window is created by the owning CDialog before the
// engines run (FormRuntime.h lifecycle) -- tolerate an unindexed root by
// falling back to rt.Root() instead of depending on who called SetHwnd(root).
HWND DialogHwnd(const FormRuntime& rt, const ht9045_uimap::UiMapDialog& dlg)
{
    HWND h = rt.HwndByPath(dlg.dfm_path);
    if (h == NULL && dlg.is_root)
        h = rt.Root();
    return h;
}

// Build the FormRuntime font-cache key, "face|height|style" verbatim
// (FormRuntime.h:87 names ApplyLayoutEngine as the key's author -- keep this
// the single place that composes it).
std::string FontKey(const ht9045_layout::DfmControlLayout* pRow)
{
    std::string sKey(pRow->font_face != NULL ? pRow->font_face : "");
    sKey += '|';
    sKey += std::to_string(pRow->font_height_px);
    sKey += '|';
    sKey += (pRow->font_style_bits != NULL ? pRow->font_style_bits : "");
    return sKey;
}

// D-GA4-6: one LOGFONTW from one layout row.
HFONT CreateRowFont(const ht9045_layout::DfmControlLayout* pRow)
{
    LOGFONTW lf;
    ::ZeroMemory(&lf, sizeof(lf));

    // Schema rule (DfmLayoutTypes.h:167-171): font_height_px is the raw
    // signed .dfm Font.Height, SAME sign convention as LOGFONT.lfHeight --
    // verbatim pass-through, never "fix" the sign.
    lf.lfHeight = pRow->font_height_px;

    lf.lfWeight    = StyleHas(pRow->font_style_bits, "fsBold") ? FW_BOLD : FW_NORMAL;
    lf.lfItalic    = StyleHas(pRow->font_style_bits, "fsItalic")    ? TRUE : FALSE;
    lf.lfUnderline = StyleHas(pRow->font_style_bits, "fsUnderline") ? TRUE : FALSE;
    lf.lfStrikeOut = StyleHas(pRow->font_style_bits, "fsStrikeOut") ? TRUE : FALSE;

    // D-GA4-6 recorded deviation: the layout table carries no Charset field,
    // so DEFAULT_CHARSET is the ruling. It must be set EXPLICITLY: the zeroed
    // struct above would otherwise mean ANSI_CHARSET (0), which breaks the
    // mapping of CJK faces the corpus really uses (DfmLayoutTypes.h:165-166,
    // e.g. u8"新細明體"). DEFAULT_CHARSET also matches VCL's
    // own TFont.Charset default, so it is the faithful choice, not merely
    // the convenient one.
    lf.lfCharSet = DEFAULT_CHARSET;

    // face is UTF-8 -> UTF-16 for CreateFontIndirectW. MultiByteToWideChar
    // does NOT truncate -- with an undersized buffer it fails outright -- so
    // convert into a roomy temp first, then copy at most LF_FACESIZE-1 chars
    // (lfFaceName must stay NUL-terminated; the tail is already zero).
    WCHAR wTmp[128];
    wTmp[0] = L'\0';
    if (pRow->font_face != NULL && pRow->font_face[0] != '\0')
        ::MultiByteToWideChar(CP_UTF8, 0, pRow->font_face, -1, wTmp, 127);
    wTmp[127] = L'\0';
    for (int i = 0; i < LF_FACESIZE - 1 && wTmp[i] != L'\0'; ++i)
        lf.lfFaceName[i] = wTmp[i];

    return ::CreateFontIndirectW(&lf);
}

// Direct children only: GetWindow(GW_CHILD)/GW_HWNDNEXT walks one level of
// the sibling chain, unlike EnumChildWindows which recurses into nested
// dialogs (that recursion is exactly what D-GA4-4's "DIRECT child" wording
// forbids -- a grandchild's coordinates live in ITS parent's client space,
// not this dialog's). Collected into a vector so passes may reposition /
// re-z-order while iterating without touching a live enumeration.
void CollectDirectChildren(HWND hDlg, std::vector<HWND>& kids)
{
    kids.clear();
    for (HWND h = ::GetWindow(hDlg, GW_CHILD); h != NULL; h = ::GetWindow(h, GW_HWNDNEXT))
        kids.push_back(h);
}

} // anonymous namespace

// ---------------------------------------------------------------------------
ApplyLayoutStats ApplyLayoutPass(FormRuntime& rt)
{
    ApplyLayoutStats stats;
    stats.moved = stats.fonts_set = stats.hidden = stats.rescaled = stats.missing = 0;

    const ht9045_uimap::UiMapForm* pMap = rt.Map();
    HWND hRoot = rt.Root();
    if (pMap == NULL || hRoot == NULL)
    {
        // Contract violation upstream (Init/BuildDialogTree not run); zeroed
        // stats will fail the probe's count assert loudly rather than let a
        // half-initialised pass "succeed".
        WriteBootLog("ApplyLayout ABORT: FormRuntime has no map/root");
        return stats;
    }

    const ht9045_layout::DfmControlLayout* pRows = rt.Rows();
    const int nRows = rt.RowCount();

    // ---- pass 1: snapshot every dialog's creation-time client rect --------
    // Keyed by HWND; the map doubles as the "indexed dialogs" set that
    // pass 4 must exclude from rescaling (a child dialog is positioned by
    // pass 3 or by DialogTreeEngine, never by proportional fixup) and that
    // pass 7 iterates. UiMapForm::dialogs is exactly "root + all SUBDLG
    // dialogs" (UiFormMap.h: one is_root row, every other row a SUBDLG).
    std::map<HWND, RECT> oldClient;
    for (int i = 0; i < pMap->dialog_count; ++i)
    {
        const ht9045_uimap::UiMapDialog& dlg = pMap->dialogs[i];
        HWND h = DialogHwnd(rt, dlg);
        if (h == NULL)
        {
            // BuildDialogTree returning true promises this never happens; if
            // it does, the dialog's own SUBDLG layout row also surfaces in
            // pass 3's `missing`, so the count contract still holds. Log,
            // don't crash.
            WriteBootLog("ApplyLayout snapshot: dialog has no HWND", dlg.dfm_path);
            continue;
        }
        RECT rc;
        ::GetClientRect(h, &rc);
        oldClient[h] = rc;
    }

    // ---- pass 2: root sizing + faithful placement (D-GA4-2) ---------------
    {
        if (pMap->root_client_w > 0 && pMap->root_client_h > 0)
        {
            // Client -> outer frame via the window's ACTUAL styles: the MFC
            // shell (or a future variant) may have altered the template's
            // style bits before this pass, and D-GA4-2 explicitly says "真實
            // style/exstyle", so read them live instead of trusting the .rc.
            RECT rc = { 0, 0, pMap->root_client_w, pMap->root_client_h };
            const DWORD dwStyle   = (DWORD)::GetWindowLongW(hRoot, GWL_STYLE);
            const DWORD dwExStyle = (DWORD)::GetWindowLongW(hRoot, GWL_EXSTYLE);
            ::AdjustWindowRectEx(&rc, dwStyle, FALSE /* no menu: the ported
                                 fMain carries no HMENU (menus are popup-only
                                 in this corpus) */, dwExStyle);
            ::SetWindowPos(hRoot, NULL, 0, 0,
                           rc.right - rc.left, rc.bottom - rc.top,
                           SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
        else
        {
            // uimap contract says these come from IR ClientWidth/Height with
            // a Width fallback (UiFormMap.h:66-68) -- a non-positive value
            // means a generator bug, not a valid "keep template size" ask.
            WriteBootLog("ApplyLayout root: non-positive root_client_w/h, template size kept");
        }

        // Faithful placement: ROOT row left/top when present (fMain's row
        // really carries them: main_layout.gen.cpp:396 left=1057 top=489).
        // .dfm form Left/Top are screen coordinates of the outer frame,
        // which is precisely SetWindowPos's coordinate space for a
        // top-level window -- no translation needed. A --center flag lives
        // with the app shell and may reposition AFTER this pass; this
        // engine implements faithful placement only (D-GA4-2).
        const ht9045_layout::DfmControlLayout* pRootRow = NULL;
        for (int i = 0; i < pMap->dialog_count && pRootRow == NULL; ++i)
            if (pMap->dialogs[i].is_root)
                pRootRow = rt.RowByPath(pMap->dialogs[i].dfm_path);
        for (int i = 0; i < nRows && pRootRow == NULL; ++i)   // paranoia fallback:
            if (IsKind(&pRows[i], "ROOT"))                    // scan by kind if the
                pRootRow = &pRows[i];                         // uimap join missed

        if (pRootRow != NULL && (pRootRow->has_left || pRootRow->has_top))
        {
            // Partial presence (only one of Left/Top written) substitutes
            // the current coordinate rather than inventing 0 -- same
            // "never fabricate an absent property" rule as DfmLayoutTypes.h
            // note 3.
            RECT wr;
            ::GetWindowRect(hRoot, &wr);
            const int x = pRootRow->has_left ? pRootRow->left : wr.left;
            const int y = pRootRow->has_top  ? pRootRow->top  : wr.top;
            ::SetWindowPos(hRoot, NULL, x, y, 0, 0,
                           SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        }
        // NOTE: the root is deliberately NOT counted in stats.moved -- the
        // probe's movable-row population is the pass-3 filter below, which
        // excludes the ROOT row (see file banner "PROBE ACCOUNTING").
    }

    // ---- pass 3: geometry for every movable layout row (D-GA4-2) ----------
    for (int i = 0; i < nRows; ++i)
    {
        const ht9045_layout::DfmControlLayout* pRow = &pRows[i];

        // Kind gate: LEAF / TABHOST / SUBDLG only. ROOT was pass 2;
        // NONVISUAL rows carry designer-icon coordinates but no HWND
        // (file banner) -- including them would report ~27 fake `missing`.
        const bool bGeomKind = IsKind(pRow, "LEAF")
                            || IsKind(pRow, "TABHOST")
                            || IsKind(pRow, "SUBDLG");
        if (!bGeomKind)
            continue;
        if (!pRow->has_left || !pRow->has_top)
            continue;   // not a movable row; the probe applies the same filter
        if (IsKind(pRow, "SUBDLG") && ::strcmp(pRow->vcl_class, "TTabSheet") == 0)
            continue;   // sheets are TCM_ADJUSTRECT territory (D-GA4-3); their
                        // rows carry no geometry in this corpus anyway --
                        // belt and braces, both documented.

        HWND h = rt.HwndByPath(pRow->dfm_path);
        if (h == NULL)
        {
            ++stats.missing;    // honest accounting: probe asserts moved+missing
            WriteBootLog("ApplyLayout row MISSING HWND", pRow->dfm_path);
            continue;
        }

        // left/top are verbatim .dfm pixels in the HOST dialog's client
        // space (DfmLayoutTypes.h note 2), and the host dialog IS the Win32
        // parent for every row that reaches here (D-GA4-3) -- so they feed
        // SetWindowPos directly (child x/y are parent-client-relative).
        UINT uFlags = SWP_NOZORDER | SWP_NOACTIVATE;
        int cx = 0, cy = 0;
        if (!pRow->has_width && !pRow->has_height)
        {
            uFlags |= SWP_NOSIZE;   // keep the current (template-scaled) size:
                                    // absent .dfm properties are absent data,
                                    // not zeros (DfmLayoutTypes.h note 3)
        }
        else
        {
            RECT wr;
            ::GetWindowRect(h, &wr);    // outer size == SetWindowPos cx/cy space
            cx = pRow->has_width  ? pRow->width  : (wr.right - wr.left);
            cy = pRow->has_height ? pRow->height : (wr.bottom - wr.top);
        }

        if (!::SetWindowPos(h, NULL, pRow->left, pRow->top, cx, cy, uFlags))
        {
            // Live HWND but the API failed -- not `missing` by definition
            // (ApplyLayoutEngine.h:51); log and keep it in `moved` so the
            // probe's two-term equation cannot be broken by a third,
            // uncounted bucket (file banner "PROBE ACCOUNTING").
            WriteBootLog("ApplyLayout SetWindowPos FAILED", pRow->dfm_path);
        }
        ++stats.moved;
    }

    // ---- pass 4: proportional rescale of unrowed direct children ----------
    // (D-GA4-4: synthesized SYNTH_CONTAINER_FRAME / SYNTH_RADIOGROUP_ITEM
    // controls have numeric IDs but no layout row -- UiFormMap.h banner --
    // so pass 3 never corrected the DLU scaling they got at creation.)
    for (std::map<HWND, RECT>::const_iterator it = oldClient.begin();
         it != oldClient.end(); ++it)
    {
        HWND hDlg = it->first;
        const RECT& rcOld = it->second;
        RECT rcNew;
        ::GetClientRect(hDlg, &rcNew);

        const int oldW = rcOld.right  - rcOld.left;   // client rects: origin 0,0
        const int oldH = rcOld.bottom - rcOld.top;
        const int newW = rcNew.right  - rcNew.left;
        const int newH = rcNew.bottom - rcNew.top;

        if (oldW <= 0 || oldH <= 0)
            continue;                        // empty source space: nothing to
                                             // scale from (and MulDiv by 0)
        if (oldW == newW && oldH == newH)
            continue;                        // dialog untouched by passes 2-3
                                             // (e.g. TTabSheet dialogs): its
                                             // children's spaces are unchanged

        std::vector<HWND> kids;
        CollectDirectChildren(hDlg, kids);
        for (size_t k = 0; k < kids.size(); ++k)
        {
            HWND hKid = kids[k];
            if (rt.RowByHwnd(hKid) != NULL)
                continue;                    // has a layout row: pass 3 owned it
                                             // (or deliberately left it alone)
            if (oldClient.find(hKid) != oldClient.end())
                continue;                    // is itself an indexed dialog:
                                             // pass 3 / DialogTreeEngine owns it

            // Old rect in the host's client space -> new space, edges scaled
            // independently so adjacency survives: a template-full-coverage
            // frame stays full-coverage exactly (D-GA4-4 "框架控件...精確");
            // radio items land on the approximate grid the design accepts
            // (D-GA4-4 records the exact-VCL-grid algorithm as W7-UI-SKIPPED).
            RECT rc;
            ::GetWindowRect(hKid, &rc);
            ::MapWindowPoints(NULL, hDlg, (POINT*)&rc, 2);   // screen -> host client

            const int l = ::MulDiv(rc.left,   newW, oldW);
            const int t = ::MulDiv(rc.top,    newH, oldH);
            const int r = ::MulDiv(rc.right,  newW, oldW);
            const int b = ::MulDiv(rc.bottom, newH, oldH);
            ::SetWindowPos(hKid, NULL, l, t, r - l, b - t,
                           SWP_NOZORDER | SWP_NOACTIVATE);
            ++stats.rescaled;
        }
    }

    // ---- pass 5: per-row fonts (D-GA4-6) -----------------------------------
    // Runs over ALL kinds (a SUBDLG dialog's own row and even the ROOT row
    // carry fonts -- main_layout.gen.cpp:396 has "MS Sans Serif"/-11 on
    // fMain itself; D-GA4-6 says WM_SETFONT "含 SUBDLG dialog 自身"). Rows
    // without has_font keep the template's DS_SETFONT font untouched.
    for (int i = 0; i < nRows; ++i)
    {
        const ht9045_layout::DfmControlLayout* pRow = &pRows[i];
        if (!pRow->has_font)
            continue;

        HWND h = rt.HwndByPath(pRow->dfm_path);
        if (h == NULL && IsKind(pRow, "ROOT"))
            h = hRoot;                       // same root fallback as pass 1
        if (h == NULL)
        {
            WriteBootLog("ApplyLayout font row without HWND", pRow->dfm_path);
            continue;
        }

        const std::string sKey = FontKey(pRow);
        HFONT hFont = rt.CachedFont(sKey);
        if (hFont == NULL)
        {
            hFont = CreateRowFont(pRow);
            if (hFont == NULL)
            {
                // CreateFontIndirect essentially never fails (GDI maps to the
                // closest installed font), but a NULL HFONT sent via
                // WM_SETFONT would silently reset the control to the system
                // font -- log and leave the template font instead.
                WriteBootLog("ApplyLayout CreateFontIndirectW FAILED", pRow->dfm_path);
                continue;
            }
            rt.CacheFont(sKey, hFont);       // FormRuntime owns + frees it
        }

        // lParam=FALSE: no immediate redraw -- the whole tree is hidden and
        // repaints once at show time (D-GA4-6 / recipe step 5).
        ::SendMessageW(h, WM_SETFONT, (WPARAM)hFont, FALSE);
        ++stats.fonts_set;
    }

    // ---- pass 6: visibility -------------------------------------------------
    for (int i = 0; i < nRows; ++i)
    {
        const ht9045_layout::DfmControlLayout* pRow = &pRows[i];
        if (pRow->visible != 0)
            continue;    // 1 = explicit True, -1 = absent = VCL default True
                         // (DfmLayoutTypes.h:154-156) -> both mean "leave it";
                         // never SW_SHOW: the tree is built hidden on purpose
                         // and D-GA4-9's no-popup guarantee depends on it.

        HWND h = rt.HwndByPath(pRow->dfm_path);
        if (h == NULL)
        {
            WriteBootLog("ApplyLayout visible=0 row without HWND", pRow->dfm_path);
            continue;
        }
        ::ShowWindow(h, SW_HIDE);
        ++stats.hidden;
    }

    // ---- pass 7: z-order (D-GA4-5) ------------------------------------------
    // Template order is .dfm declaration order (generator保序) and is the
    // default polarity: DO NOTHING. The flip is GA-5's one-switch visual
    // experiment for W7 §7-10's recorded unknown -- reverse the sibling
    // chain of every dialog exactly once.
    if (rt.ZOrderFlip())
    {
        std::vector<HWND> kids;
        for (std::map<HWND, RECT>::const_iterator it = oldClient.begin();
             it != oldClient.end(); ++it)
        {
            CollectDirectChildren(it->first, kids);   // current top -> bottom
            if (kids.size() < 2)
                continue;                             // 0/1 child: flip is a no-op

            // HWND_TOP ladder: promoting the current top-to-bottom sequence
            // one element at a time means each promotion pushes all previous
            // ones down by one -- after the last step the chain is exactly
            // reversed (element N-1 on top, element 0 at the bottom).
            for (size_t k = 0; k < kids.size(); ++k)
                ::SetWindowPos(kids[k], HWND_TOP, 0, 0, 0, 0,
                               SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        }
    }

    return stats;
}

} // namespace ht9045_ui
