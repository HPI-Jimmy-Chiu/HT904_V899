// =============================================================================
//  ui/GateAPlaceholderDlg.h -- Gate A placeholder main dialog (W7-U0)
//
//  AI(W906-GateA-0) 20260804: NEW FILE, deliberately disposable. A minimal
//  CDialog over IDD_GATEA_PLACEHOLDER (ui/app.rc) that proves the MFC message
//  loop opens, pumps and closes. GA-4 replaces this with the real generated
//  fMain dialog; nothing here is load-bearing beyond the smoke timer.
// =============================================================================
#ifndef GateAPlaceholderDlg_H
#define GateAPlaceholderDlg_H

#include <afxwin.h>
#include "resource.h"

class CGateAPlaceholderDlg : public CDialog
{
public:
    // iSmokeCloseMs: 0 = interactive; >0 = EndDialog(IDOK) after that many ms.
    explicit CGateAPlaceholderDlg(int iSmokeCloseMs, CWnd* pParent = NULL);

    enum { IDD = IDD_GATEA_PLACEHOLDER };

protected:
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    int m_iSmokeCloseMs;
    static const UINT_PTR kSmokeTimerId = 1;
};

#endif // GateAPlaceholderDlg_H
