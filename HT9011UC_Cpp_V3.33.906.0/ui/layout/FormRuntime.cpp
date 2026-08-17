// ===========================================================================
//  ui/layout/FormRuntime.cpp -- per-form runtime state shared by the GA-4 UI
//  engines. Contract: FormRuntime.h (hand-authored by the main loop); design:
//  docs/DESIGN_GA4_UI_ENGINES.md.
//
//  AI(W906-GA4) 20260804: NEW FILE (agent B). Implementation notes beyond the
//  header's own banner:
//
//  * INDEX KEYS. The four table indexes (m_rowByPath/m_dlgByPath/m_ctlByPath/
//    m_tabByPath) key on `const char*` pointers INTO the generated tables'
//    static storage (kmainLayout[] string literals / *_uimap.gen.cpp
//    literals) with CStrLess comparing content -- zero string copies for 850
//    layout rows + 758 controls. The two live-window maps use std::string
//    keys instead, because SetHwnd may legitimately be called with a
//    transient buffer (FormRuntime.h:115-117 spells this split out).
//
//  * OWNERSHIP. Destroy() is the single release point for everything this
//    object owns: attached shells (UnsubclassWindow + delete, D-GA4-8),
//    cached HFONTs (D-GA4-6) and cached solid HBRUSHes (D-GA4-7). It does
//    NOT destroy any window -- the root dialog owns its children the normal
//    Win32 way (FormRuntime.h:61-63) -- and it is idempotent because the
//    dtor calls it unconditionally after the owner may already have called
//    it explicitly.
//
//  * SYSTEM COLOURS never enter the brush cache. That rule is structural,
//    not enforced here: a COLORREF cannot carry "I am a system colour", so
//    the CALLERS (CtlColorEngine per D-GA4-7) must resolve negative TColors
//    through GetSysColorBrush (OS-owned, never deleted) and only hand this
//    cache genuine literal RGB values. This file documents the contract; it
//    cannot check it.
// ===========================================================================
#include "FormRuntime.h"

#include <string.h>            // strcmp for the CStrLess-keyed maps (afx.h
                               // pulls it transitively, but this TU uses it
                               // directly, so it is named directly)

#include "Public/cBootLog.h"   // WriteBootLog -- same diagnostics channel as
                               // ui/HT9045App.cpp (task contract)

namespace ht9045_ui {

FormRuntime::FormRuntime()
    : m_pMap(NULL)
    , m_pRows(NULL)
    , m_nRows(0)
    , m_hInst(NULL)
    , m_hRoot(NULL)
    , m_bZOrderFlip(false)
{
}

FormRuntime::~FormRuntime()
{
    Destroy();      // FormRuntime.h:51 -- dtor delegates; Destroy is idempotent
}

// ---------------------------------------------------------------------------
// Init -- bind the generated tables and build the four static indexes.
// No windows are touched here (FormRuntime.h:53-54): BuildDialogTree fills
// the live maps later. Re-Init on a live object is not part of the GA-4
// lifecycle, but the index maps are cleared first anyway so a misuse degrades
// to "stale live maps" rather than "duplicated index entries".
// ---------------------------------------------------------------------------
void FormRuntime::Init(const ht9045_uimap::UiMapForm* pMap,
                       const ht9045_layout::DfmControlLayout* pRows,
                       int nRowCount,
                       HINSTANCE hInst)
{
    m_pMap  = pMap;
    m_pRows = pRows;
    m_nRows = (pRows != NULL && nRowCount > 0) ? nRowCount : 0;
    m_hInst = hInst;

    m_rowByPath.clear();
    m_dlgByPath.clear();
    m_ctlByPath.clear();
    m_tabByPath.clear();

    // Layout rows: dfm_path is globally unique per form (DfmLayoutTypes.h:128),
    // so plain operator[] assignment cannot silently mask a duplicate that
    // matters -- but the generator guarantees uniqueness, we just consume it.
    for (int i = 0; i < m_nRows; ++i)
        m_rowByPath[m_pRows[i].dfm_path] = &m_pRows[i];

    if (m_pMap != NULL)
    {
        for (int i = 0; i < m_pMap->dialog_count; ++i)
            m_dlgByPath[m_pMap->dialogs[i].dfm_path] = &m_pMap->dialogs[i];

        // Controls key on source_path (UiFormMap.h:46): for synthesized
        // controls this is a generator-invented path that by construction
        // never collides with a real .dfm path (UiFormMap.h banner, JOIN
        // KEYS paragraph) -- so one flat map serves both populations.
        for (int i = 0; i < m_pMap->control_count; ++i)
            m_ctlByPath[m_pMap->controls[i].source_path] = &m_pMap->controls[i];

        for (int i = 0; i < m_pMap->tabhost_count; ++i)
            m_tabByPath[m_pMap->tabhosts[i].dfm_path] = &m_pMap->tabhosts[i];
    }
    else
    {
        // A NULL map is a wiring bug in the owner, not something this class
        // can recover from -- record it so a blank first-light window is
        // diagnosable from BootLog alone (GA-4 has no golden oracle).
        WriteBootLog("FormRuntime::Init called with NULL UiMapForm -- no dialogs/controls indexable");
    }
}

// ---------------------------------------------------------------------------
// Destroy -- release order matters:
//   1. shells FIRST, while their HWNDs (children of a possibly still-alive
//      root) can still be safely unsubclassed. UnsubclassWindow restores the
//      class wndproc (DefWindowProc, per RegisterCustomClasses) and detaches,
//      so the subsequent `delete` runs on a CWnd with m_hWnd == NULL and
//      cannot double-destroy the window. If the window is ALREADY gone
//      (root destroyed before us), MFC's WM_NCDESTROY handling detached the
//      shell back then, and the IsWindow guard skips the unsubclass.
//   2. GDI objects AFTER shells: a shell's dtor path must never touch a
//      deleted HFONT/HBRUSH.
//   3. maps last. Idempotent: after the first pass every container is empty
//      and every loop below is a no-op.
// m_bZOrderFlip is deliberately NOT reset: it is launch configuration
// (--zorder, D-GA4-5), not a per-window artefact.
// ---------------------------------------------------------------------------
void FormRuntime::Destroy()
{
    for (size_t i = 0; i < m_shells.size(); ++i)
    {
        CWnd* pShell = m_shells[i];
        if (pShell == NULL)
            continue;
        HWND h = pShell->GetSafeHwnd();
        if (h != NULL && ::IsWindow(h))
            pShell->UnsubclassWindow();
        delete pShell;
    }
    m_shells.clear();

    for (std::map<std::string, HFONT>::iterator it = m_fonts.begin();
         it != m_fonts.end(); ++it)
    {
        if (it->second != NULL)
            ::DeleteObject(it->second);
    }
    m_fonts.clear();

    for (std::map<COLORREF, HBRUSH>::iterator it = m_brushes.begin();
         it != m_brushes.end(); ++it)
    {
        if (it->second != NULL)
            ::DeleteObject(it->second);
    }
    m_brushes.clear();

    m_rowByPath.clear();
    m_dlgByPath.clear();
    m_ctlByPath.clear();
    m_tabByPath.clear();
    m_hwndByPath.clear();
    m_rowByHwnd.clear();

    m_pMap  = NULL;
    m_pRows = NULL;
    m_nRows = 0;
    m_hInst = NULL;
    m_hRoot = NULL;
}

// ---------------------------------------------------------------------------
// Static-table lookups. All return NULL on miss -- a miss is MEANINGFUL for
// RowByPath (synthesized controls have no layout row; that miss selects the
// D-GA4-4 rescale set, UiFormMap.h banner) and merely absent for the others.
// ---------------------------------------------------------------------------
const ht9045_layout::DfmControlLayout* FormRuntime::RowByPath(const char* dfmPath) const
{
    if (dfmPath == NULL)
        return NULL;
    std::map<const char*, const ht9045_layout::DfmControlLayout*, CStrLess>::const_iterator
        it = m_rowByPath.find(dfmPath);
    return (it != m_rowByPath.end()) ? it->second : NULL;
}

const ht9045_uimap::UiMapDialog* FormRuntime::DialogByPath(const char* dfmPath) const
{
    if (dfmPath == NULL)
        return NULL;
    std::map<const char*, const ht9045_uimap::UiMapDialog*, CStrLess>::const_iterator
        it = m_dlgByPath.find(dfmPath);
    return (it != m_dlgByPath.end()) ? it->second : NULL;
}

const ht9045_uimap::UiMapControl* FormRuntime::ControlByPath(const char* dfmPath) const
{
    if (dfmPath == NULL)
        return NULL;
    std::map<const char*, const ht9045_uimap::UiMapControl*, CStrLess>::const_iterator
        it = m_ctlByPath.find(dfmPath);
    return (it != m_ctlByPath.end()) ? it->second : NULL;
}

const ht9045_uimap::UiMapTabHost* FormRuntime::TabHostByPath(const char* dfmPath) const
{
    if (dfmPath == NULL)
        return NULL;
    std::map<const char*, const ht9045_uimap::UiMapTabHost*, CStrLess>::const_iterator
        it = m_tabByPath.find(dfmPath);
    return (it != m_tabByPath.end()) ? it->second : NULL;
}

// ---------------------------------------------------------------------------
// Live-window index. SetHwnd feeds BOTH maps: path->HWND always, and
// HWND->layout-row whenever a layout row exists for that path (the row is
// what CtlColorRoute needs at WM_CTLCOLOR* time, D-GA4-7 -- an HWND with no
// row simply never gets table-driven colouring, which is exactly right for
// synthesized controls).
// ---------------------------------------------------------------------------
void FormRuntime::SetHwnd(const char* dfmPath, HWND h)
{
    if (dfmPath == NULL || h == NULL)
        return;
    m_hwndByPath[std::string(dfmPath)] = h;

    const ht9045_layout::DfmControlLayout* pRow = RowByPath(dfmPath);
    if (pRow != NULL)
        m_rowByHwnd[h] = pRow;
}

HWND FormRuntime::HwndByPath(const char* dfmPath) const
{
    if (dfmPath == NULL)
        return NULL;
    std::map<std::string, HWND>::const_iterator it = m_hwndByPath.find(std::string(dfmPath));
    return (it != m_hwndByPath.end()) ? it->second : NULL;
}

const ht9045_layout::DfmControlLayout* FormRuntime::RowByHwnd(HWND h) const
{
    if (h == NULL)
        return NULL;
    std::map<HWND, const ht9045_layout::DfmControlLayout*>::const_iterator
        it = m_rowByHwnd.find(h);
    return (it != m_rowByHwnd.end()) ? it->second : NULL;
}

// ---------------------------------------------------------------------------
// Font cache. Key format "face|height|style" is ApplyLayoutEngine's business
// (FormRuntime.h:87); this side only stores and owns. The expected caller
// pattern is check-then-insert:
//     f = CachedFont(key); if (!f) { f = CreateFontIndirectW(...); CacheFont(key, f); }
// so a duplicate key with a DIFFERENT handle can only mean a caller bug.
// Deleting either handle at that point would be worse than the bug itself:
// the old one may already be selected into windows (WM_SETFONT), the new one
// is what the confused caller is about to WM_SETFONT with. So BOTH are kept
// alive: the key now maps to the new handle, and the old one is re-filed
// under a synthetic retire key ('\n' cannot occur in a real "face|height|
// style" key, so retire keys can never shadow a lookup) and freed with
// everything else in Destroy(). Zero dangling GDI handles either way.
// ---------------------------------------------------------------------------
HFONT FormRuntime::CachedFont(const std::string& key) const
{
    std::map<std::string, HFONT>::const_iterator it = m_fonts.find(key);
    return (it != m_fonts.end()) ? it->second : NULL;
}

void FormRuntime::CacheFont(const std::string& key, HFONT f)
{
    if (f == NULL)
        return;                                  // nothing to own

    std::map<std::string, HFONT>::iterator it = m_fonts.find(key);
    if (it == m_fonts.end())
    {
        m_fonts[key] = f;                        // the normal (only expected) path
        return;
    }
    if (it->second == f)
        return;                                  // same handle re-cached: no-op

    WriteBootLog("FormRuntime::CacheFont duplicate key with new handle (caller skipped CachedFont?)",
                 AnsiString(key.c_str()));
    HFONT hOld = it->second;
    it->second = f;
    for (int n = 0; ; ++n)                       // find a free retire slot
    {
        char szSuffix[32];
        ::wsprintfA(szSuffix, "\n#retired#%d", n);
        std::string retireKey = key + szSuffix;
        if (m_fonts.find(retireKey) == m_fonts.end())
        {
            m_fonts[retireKey] = hOld;           // stays alive until Destroy()
            break;
        }
    }
}

// ---------------------------------------------------------------------------
// Brush cache: resolved literal COLORREF -> owned solid brush, created on
// miss (FormRuntime.h:92). System colours must never reach here -- see the
// file banner and D-GA4-7 (GetSysColorBrush brushes are OS-owned and must
// not be DeleteObject'd, which Destroy() would do to anything cached here).
// A CreateSolidBrush failure (GDI handle exhaustion) is NOT cached, so a
// later call may still succeed once handles free up.
// ---------------------------------------------------------------------------
HBRUSH FormRuntime::CachedBrush(COLORREF cr)
{
    std::map<COLORREF, HBRUSH>::const_iterator it = m_brushes.find(cr);
    if (it != m_brushes.end())
        return it->second;

    HBRUSH hbr = ::CreateSolidBrush(cr);
    if (hbr == NULL)
    {
        WriteBootLog("FormRuntime::CachedBrush CreateSolidBrush FAILED",
                     AnsiString().sprintf("colorref=0x%08lX", (unsigned long)cr));
        return NULL;
    }
    m_brushes[cr] = hbr;
    return hbr;
}

// ---------------------------------------------------------------------------
// Shell ownership (D-GA4-8): CustomCtrlAttach hands every attached MFC shell
// here; Destroy() unsubclasses + deletes them.
// ---------------------------------------------------------------------------
void FormRuntime::AddShell(CWnd* pShell)
{
    if (pShell != NULL)
        m_shells.push_back(pShell);
}

} // namespace ht9045_ui
