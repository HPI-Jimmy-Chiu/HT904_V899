// =============================================================================
//  ui/CLedCtrl.cpp -- see CLedCtrl.h for the design rationale.
//  AI(W906-GateA-4-C5) 20260804: NEW FILE.
// =============================================================================
#include "CLedCtrl.h"
#include "RegisterCustomClasses.h"
#include "vclcompat/render/LedRender.h"

BEGIN_MESSAGE_MAP(CLedCtrl, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_TIMER()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_PRINTCLIENT, &CLedCtrl::OnPrintClient)
END_MESSAGE_MAP()

CLedCtrl::CLedCtrl()
    : m_pCore(&m_ownCore)
    , m_uTimerInterval(0)
    , m_bClickPending(FALSE)
{
}

void CLedCtrl::AttachCore(vclcompat::LedCore* pCore)
{
    m_pCore = (pCore != NULL) ? pCore : &m_ownCore;
    SyncFromCore();
}

// --- painting --------------------------------------------------------------
// RenderLed is given the CLIENT rect, not core.Width/Height. In VCL those are
// the same thing (a TALed's Width/Height IS its window extent); in Win32 the
// HWND extent is the authority because it came from the dialog template /
// layout table. They agree in practice: plan S4-V12 / KNOWLEDGE gotcha 11
// measured 0 of 1,840 .dfm instances overriding the geometry away from what
// ApplyStyleExtents produces. Passing the client rect also means RenderLed's
// stretch keeps the art covering the whole client if the layout pass ever
// does resize one -- matching golden Paint, which StretchDraws to whatever
// rect it is handed (LedRender.h:58-65).
void CLedCtrl::DrawInto(HDC hdc)
{
    CRect rc;
    GetClientRect(&rc);
    vclcompat::render::RenderLed(hdc, rc, *m_pCore);
}

void CLedCtrl::OnPaint()
{
    CPaintDC dc(this);
    DrawInto(dc.GetSafeHdc());
}

// WM_PRINTCLIENT is what makes this control paintable with no screen at all:
// DefWindowProc turns WM_PRINT|PRF_CLIENT into WM_PRINTCLIENT, so a headless
// test (ui/tests_headless/headless_ctrl_probe.cpp) can render a real control
// into a memory DC without ever showing a window. Handling it is also plain
// good custom-control manners (layered windows / print preview use it).
LRESULT CLedCtrl::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
    if (wParam != 0)
        DrawInto((HDC)wParam);
    return 0;
}

BOOL CLedCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;    // RenderLed covers the whole client rect; erasing first
                    // would only add a flicker frame.
}

// --- timer (the piece LedCore.h:28-34 explicitly deferred to this shell) ----
void CLedCtrl::SyncTimer()
{
    if (m_hWnd == NULL)
        return;

    const bool bWant = m_pCore->IsTimerActive();
    const UINT uWant = bWant ? (UINT)m_pCore->GetInterval() : 0;

    if (uWant == m_uTimerInterval)
        return;                      // includes "still running, same interval":
                                     // re-arming would restart the countdown,
                                     // which golden's live `fLedTimer.Interval
                                     // := V` (aled.pas:285-286) does not do.
    if (m_uTimerInterval != 0)
        KillTimer(kBlinkTimerId);
    if (uWant != 0)
        SetTimer(kBlinkTimerId, uWant, NULL);
    m_uTimerInterval = uWant;
}

void CLedCtrl::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == (UINT_PTR)kBlinkTimerId)
    {
        m_pCore->OnTimerTick();      // aled.pas OnLedTimer :175-180 (flips ColorTemp)
        Invalidate(FALSE);
        return;
    }
    CWnd::OnTimer(nIDEvent);
}

void CLedCtrl::OnDestroy()
{
    if (m_uTimerInterval != 0)
    {
        KillTimer(kBlinkTimerId);
        m_uTimerInterval = 0;
    }
    CWnd::OnDestroy();
}

void CLedCtrl::PreSubclassWindow()
{
    CWnd::PreSubclassWindow();
    SyncTimer();    // a core configured (Blink+Value) before the window existed
                    // must start blinking as soon as it does.
}

// --- forwarders ------------------------------------------------------------
void CLedCtrl::SetValue(bool v)     { m_pCore->SetValue(v);    SyncFromCore(); }
void CLedCtrl::SetBlink(bool v)     { m_pCore->SetBlink(v);    SyncFromCore(); }
void CLedCtrl::SetInterval(long ms) { m_pCore->SetInterval(ms); SyncFromCore(); }

void CLedCtrl::SyncFromCore()
{
    SyncTimer();
    if (m_hWnd != NULL)
        Invalidate(FALSE);
}

// --- mouse -----------------------------------------------------------------
// aled.pas:72 publishes OnClick; 25 .dfm LED instances bind it. No LED
// instance binds OnMouseDown/OnMouseUp, but the notification is sent anyway
// so all three shells behave identically (see RegisterCustomClasses.h).
// No SetCapture: a press that ends outside the control simply produces no
// click, because the WM_LBUTTONUP goes elsewhere. Whether golden's VCL fires
// OnClick in that drag-out case was NOT verifiable here (no running BCB6
// oracle) and is recorded as an open fidelity question rather than guessed.
void CLedCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bClickPending = TRUE;
    HT9045_NotifyParentMouse(m_hWnd, HT9045N_LBUTTONDOWN, point, nFlags);
}

void CLedCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    HT9045_NotifyParentMouse(m_hWnd, HT9045N_LBUTTONUP, point, nFlags);

    CRect rc;
    GetClientRect(&rc);
    if (m_bClickPending && rc.PtInRect(point))
        HT9045_NotifyParentClicked(m_hWnd);
    m_bClickPending = FALSE;
}
