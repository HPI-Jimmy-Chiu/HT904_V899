// =============================================================================
//  ui/CTrayCtrl.cpp -- see CTrayCtrl.h for the design rationale.
//  AI(W906-GateA-4-C5) 20260804: NEW FILE.
// =============================================================================
#include "CTrayCtrl.h"
#include "RegisterCustomClasses.h"
#include "vclcompat/render/TrayRender.h"

BEGIN_MESSAGE_MAP(CTrayCtrl, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_MESSAGE(WM_PRINTCLIENT, &CTrayCtrl::OnPrintClient)
END_MESSAGE_MAP()

CTrayCtrl::CTrayCtrl()
    : m_pCore(&m_ownCore)
    , m_pCore256(NULL)
    , m_bClickPending(FALSE)
{
}

void CTrayCtrl::AttachCore(vclcompat::TrayCore* pCore)
{
    m_pCore    = (pCore != NULL) ? pCore : &m_ownCore;
    m_pCore256 = NULL;
    ApplyExtents();
    SyncFromCore();
}

void CTrayCtrl::AttachCore256(vclcompat::Tray256Core* pCore)
{
    m_pCore256 = pCore;     // NULL here means "back to the 100-cell core"
    ApplyExtents();
    SyncFromCore();
}

// The WM_SIZE half of golden's WndProc (HTray.cpp:81-85), expressed through
// the core's documented stand-in for it (TrayCore.h:104-108): SetExtents sets
// Width/Height and re-derives the geometry.
void CTrayCtrl::ApplyExtents()
{
    if (m_hWnd == NULL)
        return;
    CRect rc;
    GetClientRect(&rc);
    if (m_pCore256 != NULL)
        m_pCore256->SetExtents(rc.Width(), rc.Height());
    else
        m_pCore->SetExtents(rc.Width(), rc.Height());
}

void CTrayCtrl::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    ApplyExtents();
    Invalidate(FALSE);
}

// golden CreateWnd (HTray.cpp:62-68): CaculateTrayParameter() then ClearCell().
// SetExtents already performs the first (see ApplyExtents). See the header for
// why the destructive ClearCell is kept.
void CTrayCtrl::PreSubclassWindow()
{
    CWnd::PreSubclassWindow();
    ApplyExtents();
    if (m_pCore256 != NULL)
        m_pCore256->ClearCell();
    else
        m_pCore->ClearCell();
}

void CTrayCtrl::DrawInto(HDC hdc)
{
    if (m_pCore256 != NULL)
        vclcompat::render::RenderTray256(hdc, *m_pCore256);
    else
        vclcompat::render::RenderTray(hdc, *m_pCore);
}

void CTrayCtrl::OnPaint()
{
    CPaintDC dc(this);
    DrawInto(dc.GetSafeHdc());
}

// See CLedCtrl.cpp's note (headless rendering into a memory DC).
LRESULT CTrayCtrl::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
    if (wParam != 0)
        DrawInto((HDC)wParam);
    return 0;
}

BOOL CTrayCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;    // RenderTray's first primitive is a whole-control
                    // (0,0,Width,Height) FTrayColor fill (TrayRender.cpp,
                    // golden HTray.cpp:278-282), so the client is fully
                    // covered and a pre-erase would only flicker.
}

void CTrayCtrl::SyncFromCore()
{
    if (m_hWnd != NULL)
        Invalidate(FALSE);
}

// --- mouse: 32 OnMouseDown + 9 OnMouseUp bindings, 0 OnClick ---------------
// The point is forwarded RAW. Golden's own handlers are the ones that call
// ConvertIndexCells(X, Y) to turn it into a cell index (16 call sites in
// golden form code), so hit-testing here would be relocating form logic into
// the widget -- and ConvertIndexCells has two documented GOLDEN BUGs
// (TrayCore.h:150, plan S6-W7-C1 items 2 and 3, including in-place rewrite of
// X/Y before its -1 early-out) whose observable effects must stay exactly
// where golden's callers see them.
// BN_CLICKED is still emitted for uniformity with the other two shells; no
// tray instance binds OnClick today, so nothing consumes it.
void CTrayCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bClickPending = TRUE;
    HT9045_NotifyParentMouse(m_hWnd, HT9045N_LBUTTONDOWN, point, nFlags);
}

void CTrayCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    HT9045_NotifyParentMouse(m_hWnd, HT9045N_LBUTTONUP, point, nFlags);

    CRect rc;
    GetClientRect(&rc);
    if (m_bClickPending && rc.PtInRect(point))
        HT9045_NotifyParentClicked(m_hWnd);
    m_bClickPending = FALSE;
}
