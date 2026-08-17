// ===========================================================================
//  ui/layout/FormRuntime.h -- per-form runtime state shared by the GA-4 UI
//  engines (docs/DESIGN_GA4_UI_ENGINES.md).
//
//  AI(W906-GA4) 20260804: NEW FILE, hand-authored contract (main loop).
//
//  ONE FormRuntime = ONE live form instance. It owns every runtime artefact
//  the engines create for that form -- HWND indexes, the font/brush caches,
//  the attached custom-control shells -- and releases them in Destroy().
//  The engines themselves are free functions and hold NO global state, so
//  a second form (or a second instance of the same form) is just a second
//  FormRuntime. That is the "write once, all 133 forms benefit" lever the
//  Gate A plan demands of this wave.
//
//  LIFECYCLE (orchestrated by the owning dialog, see DialogTreeEngine.h):
//    Init(map, rows, rowCount, hInst)      bind the two generated tables
//    <caller creates the root window>      (MFC CDialog on IDD_<root>)
//    BuildDialogTree(rt, hRoot)            create the 90 children + tabs
//    ApplyLayoutPass(rt)                   pixel geometry / fonts / vis
//    AttachCustomControls(rt)              subclass HT9045.* shells + props
//    ... WM_CTLCOLOR* -> CtlColorRoute(rt, ...)
//    Destroy()                             frees shells, fonts, brushes;
//                                          child HWNDs die with the root.
//
//  THREADING: UI thread only, like everything it wraps.
// ===========================================================================
#ifndef HT9045_UI_LAYOUT_FORMRUNTIME_H
#define HT9045_UI_LAYOUT_FORMRUNTIME_H

#include <afxwin.h>
#include <map>
#include <string>
#include <vector>

#include "UiFormMap.h"
#include "DfmLayoutTypes.h"   // tools/dfm2rc/layout_out (on the include path)

class CWnd;   // MFC shells are stored type-erased as CWnd* (CLedCtrl etc.)

namespace ht9045_ui {

// Comparator so const char* keys compare by content, not pointer identity.
struct CStrLess {
    bool operator()(const char* a, const char* b) const
    { return ::strcmp(a, b) < 0; }
};

class FormRuntime {
public:
    FormRuntime();
    ~FormRuntime();                       // calls Destroy()

    // Bind the generated tables. Also builds the static indexes (path ->
    // layout row, path -> uimap control/dialog). No windows yet.
    void Init(const ht9045_uimap::UiMapForm* pMap,
              const ht9045_layout::DfmControlLayout* pRows,
              int nRowCount,
              HINSTANCE hInst);

    // Frees shells (Unsubclass+delete), HFONTs, owned HBRUSHes, clears maps.
    // Safe to call twice. Does NOT destroy windows: the root dialog owns its
    // children the normal Win32 way.
    void Destroy();

    // ---- static tables (valid after Init) --------------------------------
    const ht9045_uimap::UiMapForm*            Map() const   { return m_pMap; }
    const ht9045_layout::DfmControlLayout*    Rows() const  { return m_pRows; }
    int                                       RowCount() const { return m_nRows; }
    HINSTANCE                                 Inst() const  { return m_hInst; }

    // Path -> layout row (nullptr when absent -- synthesized controls).
    const ht9045_layout::DfmControlLayout*    RowByPath(const char* dfmPath) const;
    // Path -> uimap entries (nullptr when absent).
    const ht9045_uimap::UiMapDialog*          DialogByPath(const char* dfmPath) const;
    const ht9045_uimap::UiMapControl*         ControlByPath(const char* dfmPath) const;
    const ht9045_uimap::UiMapTabHost*         TabHostByPath(const char* dfmPath) const;

    // ---- live window indexes (filled by DialogTreeEngine) ----------------
    void  SetHwnd(const char* dfmPath, HWND h);            // both dialogs & controls
    HWND  HwndByPath(const char* dfmPath) const;           // NULL when absent
    const ht9045_layout::DfmControlLayout* RowByHwnd(HWND h) const;

    HWND  Root() const          { return m_hRoot; }
    void  SetRoot(HWND h)       { m_hRoot = h; }

    // ---- caches (owned) ---------------------------------------------------
    // Font cache key is built by ApplyLayoutEngine as "face|height|style".
    HFONT  CachedFont(const std::string& key) const;
    void   CacheFont(const std::string& key, HFONT f);     // takes ownership
    // Brush cache keyed by resolved COLORREF. System colours never enter it
    // (GetSysColorBrush is OS-owned, D-GA4-7).
    HBRUSH CachedBrush(COLORREF cr);                        // creates on miss

    // ---- attached custom-control shells (owned) ---------------------------
    void   AddShell(CWnd* pShell);                          // takes ownership
    int    ShellCount() const   { return (int)m_shells.size(); }

    // ---- flags -------------------------------------------------------------
    bool   ZOrderFlip() const   { return m_bZOrderFlip; }
    void   SetZOrderFlip(bool b){ m_bZOrderFlip = b; }

private:
    const ht9045_uimap::UiMapForm*         m_pMap;
    const ht9045_layout::DfmControlLayout* m_pRows;
    int                                    m_nRows;
    HINSTANCE                              m_hInst;
    HWND                                   m_hRoot;
    bool                                   m_bZOrderFlip;

    std::map<const char*, const ht9045_layout::DfmControlLayout*, CStrLess> m_rowByPath;
    std::map<const char*, const ht9045_uimap::UiMapDialog*,  CStrLess>      m_dlgByPath;
    std::map<const char*, const ht9045_uimap::UiMapControl*, CStrLess>      m_ctlByPath;
    std::map<const char*, const ht9045_uimap::UiMapTabHost*, CStrLess>      m_tabByPath;

    // Live-window maps use std::string keys: SetHwnd may be called with
    // transient strings, unlike the table indexes above whose const char*
    // keys point into the generated tables' static storage.
    std::map<std::string, HWND>                                    m_hwndByPath;
    std::map<HWND, const ht9045_layout::DfmControlLayout*>         m_rowByHwnd;

    std::map<std::string, HFONT>   m_fonts;
    std::map<COLORREF, HBRUSH>     m_brushes;
    std::vector<CWnd*>             m_shells;

    FormRuntime(const FormRuntime&);            // non-copyable
    FormRuntime& operator=(const FormRuntime&);
};

} // namespace ht9045_ui

#endif // HT9045_UI_LAYOUT_FORMRUNTIME_H
