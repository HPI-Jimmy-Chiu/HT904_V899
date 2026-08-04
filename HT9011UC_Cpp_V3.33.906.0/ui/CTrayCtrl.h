// =============================================================================
//  ui/CTrayCtrl.h -- W7-C5 / GA-4-C5
//  MFC CWnd shell for the tray family: window class "HT9045.MyTray"
//  (246 .dfm instances, golden TTMyTray : TCustomControl) and
//  "HT9045.MyTray256" (0 .dfm instances but 7 dynamic `new` sites --
//  cBinSel.cpp:232-235, uYieldMonitoring -- see plan S4-V12).
//
//  AI(W906-GateA-4-C5) 20260804: NEW FILE.
//
//  Base is CWnd (not CStatic): golden's base is TCustomControl, a plain
//  self-painting windowed control with no static-control semantics at all.
//
//  ALL state, geometry and hit-testing is vclcompat::TrayCore /
//  vclcompat::Tray256Core (W7-C1); all pixels are
//  vclcompat::render::RenderTray / RenderTray256 (W7-C2). This shell only
//  routes messages.
//
//  TWO CORE TYPES, ONE SHELL. TrayCore and Tray256Core share no base class
//  (Tray256Core has no block/visibility support at all -- golden HTray256.h
//  genuinely lacks those fields, verified in TrayCore.h:168-174), so a single
//  core pointer is impossible. Rather than a fourth shell class for a control
//  with zero .dfm instances, this one carries one pointer of each type and
//  paints whichever is attached. That costs six lines and no duplicated
//  logic, and it gives the 7 dynamic TTMyTray256 sites a home.
//
//  WM_SIZE IS FAITHFUL ROUTING, NOT NEW BEHAVIOUR. golden HTray.cpp:78-85:
//      TCustomControl::WndProc(Message);
//      if (Message.Msg == WM_PAINT || Message.Msg == WM_SIZE)
//          { CaculateTrayParameter(); DrawTray(); }
//  TrayCore::SetExtents() is documented (TrayCore.h:104-108) as this wave's
//  stand-in for exactly that VCL WM_SIZE hook -- it sets Width/Height and
//  re-derives the geometry -- so OnSize() below calls it and invalidates.
//  golden also recomputed on every WM_PAINT; that is unnecessary here because
//  the only input to CaculateTrayParameter that can change without going
//  through a core setter is the window extent, and WM_SIZE already covers it.
//
//  PreSubclassWindow REPRODUCES golden CreateWnd (HTray.cpp:62-68), which
//  calls CaculateTrayParameter() then ClearCell(). ClearCell wipes every cell
//  colour/text back to the ctor defaults, so attaching an already-populated
//  core and THEN subclassing the window will discard that data -- that is
//  golden's own ordering (in VCL the window handle is created with the form,
//  long before any application code writes cells), preserved rather than
//  quietly improved.
//
//  REPAINT POLICY FOR CELL WRITES IS DELIBERATELY NOT DECIDED HERE. golden's
//  SetCellNumber/SetCellColorIndex each called DrawSingleIC immediately; the
//  W7-C1 cores dropped that side effect (documented per method in
//  TrayCore.h:139-146). There are ~1,655 call points tree-wide, and whether
//  they should each force a paint, or be batched by the facade, is a binder
//  (W7-U0/W7-C4) decision with real performance consequences. This shell
//  therefore exposes Core() + SyncFromCore() and takes no position.
// =============================================================================
#ifndef HT9045_UI_CTRAYCTRL_H
#define HT9045_UI_CTRAYCTRL_H

#include <afxwin.h>
#include "vclcompat/TrayCore.h"

class CTrayCtrl : public CWnd
{
public:
    CTrayCtrl();

    // Exactly one of these is in force at a time; attaching either clears the
    // other. NULL restores the internal TrayCore. No ownership is taken.
    void AttachCore(vclcompat::TrayCore* pCore);
    void AttachCore256(vclcompat::Tray256Core* pCore);

    vclcompat::TrayCore&       Core()       { return *m_pCore; }     // valid when !Is256()
    const vclcompat::TrayCore& Core() const { return *m_pCore; }
    vclcompat::Tray256Core&    Core256()    { return *m_pCore256; }  // valid when Is256()
    bool Is256() const { return m_pCore256 != NULL; }

    void SyncFromCore();     // repaint after mutating the core directly

protected:
    virtual void PreSubclassWindow();   // golden CreateWnd, HTray.cpp:62-68
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

private:
    void DrawInto(HDC hdc);
    void ApplyExtents();

    vclcompat::TrayCore     m_ownCore;
    vclcompat::TrayCore*    m_pCore;
    vclcompat::Tray256Core* m_pCore256;   // NULL unless AttachCore256 was used
    BOOL m_bClickPending;
};

#endif // HT9045_UI_CTRAYCTRL_H
