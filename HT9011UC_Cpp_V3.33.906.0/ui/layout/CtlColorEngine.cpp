// ===========================================================================
//  ui/layout/CtlColorEngine.cpp -- table-driven WM_CTLCOLOR* handling plus
//  the ONE paint-time TColor -> COLORREF resolve
//  (docs/DESIGN_GA4_UI_ENGINES.md D-GA4-7; contract: CtlColorEngine.h).
//
//  AI(W906-GA4) 20260804: NEW FILE, implements the header exactly.
//
//  DELPHI TCOLOR SEMANTICS (DfmLayoutTypes.h note 4, CtlColorEngine.h:8-11).
//  The layout table stores genuine SIGNED 32-bit TColors, byte-identical to
//  Delphi's real `TColor = type Integer` in-memory representation. Two
//  disjoint encodings share the type:
//    * v >= 0 : the low 24 bits already ARE a COLORREF. TColor and COLORREF
//      are both 0x00BBGGRR -- no channel swizzle exists, only masking off
//      bits 24..31, which VCL's own ColorToRGB discards the same way.
//    * v < 0 (bit 31 set, e.g. clBtnFace = 0x8000000F) : a SYSTEM-COLOUR
//      REFERENCE whose LOW BYTE is the GetSysColor() index (0x0F ==
//      COLOR_BTNFACE). It MUST be resolved at paint time, never pre-baked:
//      the Windows theme can change while the app runs and golden VCL
//      re-reads the system colour on every paint. Schema note 4 makes this
//      binding ("cannot be fully resolved ahead of paint time and must not
//      be silently baked in as today's theme colour").
//  ResolveTColor below is THE single definition of that rule for the UI
//  engines, the attach pass and the headless probe (header contract).
//  vclcompat/render/RenderCommon.h:62 carries the renderer-side twin
//  (render::ResolveColor, part of the MinGW-tested W7-C2 oracle); the two
//  agree on every value the generator can emit -- colorconsts.to_i32 only
//  produces the 0x80xxxxxx form for symbolic system references, and that
//  whole range is negative as a signed int -- and they stay separate
//  because ui/layout must not grow a dependency on vclcompat/render (nor
//  the reverse: the render half also builds under MinGW, this file is
//  MSVC/MFC-only).
// ===========================================================================
#include "CtlColorEngine.h"

namespace ht9045_ui {

COLORREF ResolveTColor(int tcolor)
{
    //AI(W906-GA4) 20260804: signed test (not a 0x80-top-byte test) because
    // the schema's contract is "system-colour references wrap to negative"
    // (DfmLayoutTypes.h note 4) -- v < 0 is that contract verbatim.
    if (tcolor < 0)
        return ::GetSysColor(tcolor & 0xFF);
    return (COLORREF)(tcolor & 0x00FFFFFF);
}

// ---------------------------------------------------------------------------
//  CtlColorRoute -- the D-GA4-7 routing decision, in table order:
//
//  1. ROW SELECTION. For CTLCOLOR_DLG the control IS the dialog (header:
//     "for _DLG it equals the dialog"), so the dialog's own SUBDLG/ROOT
//     layout row is what RowByHwnd(hCtl) yields -- DialogTreeEngine
//     SetHwnd()s every dialog it creates and FormRuntime indexes the layout
//     row alongside. The one HWND that may legitimately be absent from that
//     index is the ROOT itself: it is created by the owning MFC CDialog,
//     not by BuildDialogTree, so whether its path was SetHwnd()'d is the
//     root dialog's business, not this engine's. A _DLG miss on rt.Root()
//     therefore falls back to the uimap's is_root dialog row (a <=91-entry
//     linear scan, taken only on root-background repaints, still
//     allocation-free). Every other miss means "not a table-driven window"
//     -> NULL -> caller uses the default proc.
//
//  2. BACKGROUND FALLBACK. A row without its own Color inherits the HOST
//     dialog row's colour when that row has one (task ruling). This is
//     VCL's ParentColor default made table-driven: a .dfm that writes no
//     Color shows the parent's colour at runtime, and schema note 3
//     explicitly forbids the table from fabricating VCL defaults itself --
//     so the default is applied HERE, at the single consumption point.
//     host_dialog_path is the right parent for this ("which dialog's client
//     area is this control painted into", DfmLayoutTypes.h note 2); it is
//     "" exactly for the ROOT row, which then simply has no fallback.
//
//  3. UNRESOLVED TOKENS (has_* false, token non-empty) fall out naturally:
//     only has_color / has_font_color are ever consulted, so such rows read
//     as "absent" (D-GA4-7: skip). Deliberately NO BootLog here even though
//     D-GA4-7 permits a one-shot one: WM_CTLCOLOR* fires per control per
//     paint (hot path), a per-call log would flood BootLog.txt, and the
//     generator's own gate already reports unresolved tokens at generation
//     time -- where a data problem belongs.
//
//  4. DC PREPARATION. SetTextColor only when the row carries a font colour.
//     SetBkColor ALWAYS when a brush is returned, so the text-cell
//     background (dialogs default to OPAQUE bk mode) matches the brush the
//     control is erased with -- including the "text without background"
//     case, which pins both to COLOR_BTNFACE instead of inheriting whatever
//     a previous WM_CTLCOLOR left in the shared DC.
//
//  5. BRUSH OWNERSHIP (D-GA4-7). Literal colours -> rt.CachedBrush()
//     (FormRuntime owns, frees in Destroy; the cache-miss CreateSolidBrush
//     is the single allowed allocation on this path). System colours ->
//     GetSysColorBrush() (OS-owned, refreshed by the OS on theme change,
//     must never enter -- or be freed by -- the cache).
// ---------------------------------------------------------------------------
HBRUSH CtlColorRoute(FormRuntime& rt, HDC hdc, HWND hCtl, int nCtlColorType)
{
    const ht9045_layout::DfmControlLayout* row = rt.RowByHwnd(hCtl);

    if (row == NULL)
    {
        if (nCtlColorType == CTLCOLOR_DLG && hCtl == rt.Root() && rt.Map() != NULL)
        {
            //AI(W906-GA4) 20260804: root fallback (banner point 1) -- the
            // root HWND is made by the owning CDialog, not BuildDialogTree,
            // so it may not be in the HWND->row index yet.
            const ht9045_uimap::UiMapForm* pMap = rt.Map();
            for (int i = 0; i < pMap->dialog_count; ++i)
            {
                if (pMap->dialogs[i].is_root)
                {
                    row = rt.RowByPath(pMap->dialogs[i].dfm_path);
                    break;
                }
            }
        }
        if (row == NULL)
            return NULL;            // not table-driven -> default handler
    }

    // ---- background: own colour, else host dialog's, else absent ----------
    bool bHasBk = false;
    int  iBk    = 0;                // signed TColor, meaningful when bHasBk
    if (row->has_color)
    {
        bHasBk = true;
        iBk    = row->color;
    }
    else if (row->host_dialog_path != NULL && row->host_dialog_path[0] != '\0')
    {
        const ht9045_layout::DfmControlLayout* pHost = rt.RowByPath(row->host_dialog_path);
        if (pHost != NULL && pHost->has_color)
        {
            bHasBk = true;
            iBk    = pHost->color;  // ParentColor fallback (banner point 2)
        }
    }

    const bool bHasText = row->has_font_color;

    if (!bHasBk && !bHasText)
        return NULL;                // nothing table-driven -> default handler

    if (bHasText)
        ::SetTextColor(hdc, ResolveTColor(row->font_color));

    if (bHasBk)
    {
        const COLORREF crBk = ResolveTColor(iBk);   // paint-time resolve
        ::SetBkColor(hdc, crBk);
        if (iBk < 0)
            return ::GetSysColorBrush(iBk & 0xFF);  // OS-owned, never cached
        return rt.CachedBrush(crBk);                // owned; miss allocates once
    }

    //AI(W906-GA4) 20260804: text colour with no background anywhere -- keep
    // the stock dialog background but make the DC's text background agree
    // with the brush returned (banner point 4).
    ::SetBkColor(hdc, ::GetSysColor(COLOR_BTNFACE));
    return ::GetSysColorBrush(COLOR_BTNFACE);
}

} // namespace ht9045_ui
