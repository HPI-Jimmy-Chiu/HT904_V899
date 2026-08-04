// =============================================================================
//  ui/CBtnPanelCtrl.cpp -- see CBtnPanelCtrl.h for the design rationale
//  (CStatic vs CButton, why SS_NOTIFY is not applied, and the written
//  hand-over of bevel+caption from BtnPanelRender.h:24-29).
//  AI(W906-GateA-4-C5) 20260804: NEW FILE.
// =============================================================================
#include "CBtnPanelCtrl.h"
#include "RegisterCustomClasses.h"
#include "vclcompat/render/BtnPanelRender.h"

BEGIN_MESSAGE_MAP(CBtnPanelCtrl, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_MESSAGE(WM_PRINTCLIENT, &CBtnPanelCtrl::OnPrintClient)
END_MESSAGE_MAP()

CBtnPanelCtrl::CBtnPanelCtrl()
    : m_pCore(&m_ownCore)
    , m_bClickPending(FALSE)
{
}

void CBtnPanelCtrl::AttachCore(vclcompat::BtnPanelCoreBase* pCore)
{
    m_pCore = (pCore != NULL) ? pCore : &m_ownCore;
    SyncFromCore();
}

// --- bevel: golden TPanel chrome, handed to this wave by BtnPanelRender.h --
// VCL's TPanel.Paint draws two nested 3-D frames, the outer one from
// BevelOuter and the inner one from BevelInner. Win32 DrawEdge expresses
// exactly that pair in one call (BDR_*OUTER | BDR_*INNER), so the mapping is
// 1:1 rather than an approximation. bvSpace maps to "no edge" -- in VCL it
// reserves blank space without drawing; neither golden class ever assigns it
// (BtnPanelCore.h:59-61), so this branch is unreachable today.
static UINT BevelOuterFlag(vclcompat::TBevelShim b)
{
    if (b == vclcompat::bvRaised)  return BDR_RAISEDOUTER;
    if (b == vclcompat::bvLowered) return BDR_SUNKENOUTER;
    return 0;
}

static UINT BevelInnerFlag(vclcompat::TBevelShim b)
{
    if (b == vclcompat::bvRaised)  return BDR_RAISEDINNER;
    if (b == vclcompat::bvLowered) return BDR_SUNKENINNER;
    return 0;
}

void CBtnPanelCtrl::DrawInto(HDC hdc)
{
    CRect rc;
    GetClientRect(&rc);

    // 1. the fill -- W7-C2, unmodified (core.Color, resolved via ResolveColor
    //    so a still-unset clBtnFace system reference paints correctly).
    vclcompat::render::RenderBtnPanel(hdc, rc, *m_pCore);

    // 2. the bevel (see the mapping note above).
    UINT uEdge = BevelOuterFlag(m_pCore->BevelOuter) | BevelInnerFlag(m_pCore->BevelInner);
    if (uEdge != 0)
    {
        RECT r = rc;
        ::DrawEdge(hdc, &r, uEdge, BF_RECT);
    }

    // 3. the caption. Uses the window text (the layout pass sets it from the
    //    .dfm Caption) and the control's own font (the layout pass does the
    //    per-control CreateFontIndirect/SetFont, plan S7-2), so nothing about
    //    the typeface is decided here.
    //
    //    FontColor may still be BtnPanelCoreBase::kUninitializedColorSentinel
    //    if no SetTrueFontColor/SetFalseFontColor has run yet. That is
    //    deliberately NOT special-cased: it is golden's own never-initialised
    //    colour member (butPa1.cpp:21-22 / BtnPanelLane.cpp:23-24 shadow the
    //    members with ctor locals -- the GOLDEN BUG documented in
    //    BtnPanelCore.h:101-123), and hiding it here would make the bug
    //    invisible at exactly the layer where it shows. In practice 756 of
    //    757 .dfm instances set FalseFontColor, so the layout pass resolves it
    //    before first paint on all but one.
    CString strText;
    GetWindowText(strText);
    if (!strText.IsEmpty())
    {
        HGDIOBJ hOldFont = NULL;
        CFont* pFont = GetFont();
        if (pFont != NULL)
            hOldFont = ::SelectObject(hdc, pFont->GetSafeHandle());

        int      nOldBk = ::SetBkMode(hdc, TRANSPARENT);
        COLORREF crOld  = ::SetTextColor(hdc, vclcompat::render::ResolveColor(m_pCore->FontColor));
        RECT     r      = rc;
        ::DrawText(hdc, strText, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        ::SetTextColor(hdc, crOld);
        ::SetBkMode(hdc, nOldBk);

        if (hOldFont != NULL)
            ::SelectObject(hdc, hOldFont);
    }
}

void CBtnPanelCtrl::OnPaint()
{
    CPaintDC dc(this);
    DrawInto(dc.GetSafeHdc());
}

// See CLedCtrl.cpp's note: this is what lets a headless test render the real
// control into a memory DC (DefWindowProc turns WM_PRINT|PRF_CLIENT into it).
LRESULT CBtnPanelCtrl::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
    if (wParam != 0)
        DrawInto((HDC)wParam);
    return 0;
}

BOOL CBtnPanelCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;    // RenderBtnPanel FillRect's the entire client rect.
}

void CBtnPanelCtrl::SetDown(bool v)
{
    m_pCore->SetDown(v);    // butPa1.cpp:81-106 -- rewrites Color/FontColor/bevels
    SyncFromCore();
}

void CBtnPanelCtrl::SyncFromCore()
{
    if (m_hWnd != NULL)
        Invalidate(FALSE);
}

// --- mouse: 714 OnClick + 32 OnMouseDown + 1 OnMouseUp bindings ------------
// (see RegisterCustomClasses.h for the measurement and for why a click goes
// out as WM_COMMAND/BN_CLICKED while down/up go out as WM_NOTIFY.)
void CBtnPanelCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bClickPending = TRUE;
    HT9045_NotifyParentMouse(m_hWnd, HT9045N_LBUTTONDOWN, point, nFlags);
}

void CBtnPanelCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    HT9045_NotifyParentMouse(m_hWnd, HT9045N_LBUTTONUP, point, nFlags);

    CRect rc;
    GetClientRect(&rc);
    if (m_bClickPending && rc.PtInRect(point))
        HT9045_NotifyParentClicked(m_hWnd);
    m_bClickPending = FALSE;
}
