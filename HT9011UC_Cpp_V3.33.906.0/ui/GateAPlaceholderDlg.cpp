// =============================================================================
//  ui/GateAPlaceholderDlg.cpp -- see GateAPlaceholderDlg.h banner.
//  AI(W906-GateA-0) 20260804: NEW FILE, disposable (replaced by fMain in GA-4).
// =============================================================================
#include "GateAPlaceholderDlg.h"

BEGIN_MESSAGE_MAP(CGateAPlaceholderDlg, CDialog)
    ON_WM_TIMER()
END_MESSAGE_MAP()

CGateAPlaceholderDlg::CGateAPlaceholderDlg(int iSmokeCloseMs, CWnd* pParent)
    : CDialog(IDD, pParent)
    , m_iSmokeCloseMs(iSmokeCloseMs)
{
}

BOOL CGateAPlaceholderDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    if (m_iSmokeCloseMs > 0)
        SetTimer(kSmokeTimerId, (UINT)m_iSmokeCloseMs, NULL);
    return TRUE;
}

void CGateAPlaceholderDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == kSmokeTimerId)
    {
        KillTimer(kSmokeTimerId);
        EndDialog(IDOK);    // smoke run: prove open->pump->close without a human
        return;
    }
    CDialog::OnTimer(nIDEvent);
}
