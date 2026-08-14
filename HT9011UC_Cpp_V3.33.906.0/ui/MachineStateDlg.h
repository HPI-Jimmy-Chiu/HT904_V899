// =============================================================================
//  ui/MachineStateDlg.h -- the machine-state monitor window (HT9045.exe default)
//
//  AI(W906-StateWin) 20260814: NEW FILE.
//
//  WHY THIS EXISTS
//  The user's goal is one keypress that gives a compiled core, a native window
//  showing machine state, and a browser view. The browser half already worked;
//  this is the native half. Until now HT9045.exe only ever opened
//  CGateAPlaceholderDlg, which by design says nothing about a machine.
//
//  HOW IT GETS ITS DATA, AND WHY THAT SHAPE
//  It is a CONSUMER of the same loopback tag feed the browser consumes --
//  webbridge::TcpTagClient against tcp://127.0.0.1:8046 -- and it links NO
//  machine code at all. Three things fall out of that, all of them the point:
//
//   1. IT SIDESTEPS C1061 ENTIRELY. ht9045_app links only ht9045_public today
//      (CMakeLists.txt:2351). The MSVC compiler cannot build the god-stack --
//      EJ1N/TextProcess.cpp:404-424 is a 127-deep else-if chain that trips
//      MSVC's nesting limit (C1061) -- so a window that read machine globals
//      directly could not be built by the only toolchain that can build MFC.
//      Reading the wire instead needs ht9045_webbridge and nothing else.
//   2. IT IS SOURCE-AGNOSTIC. The wire format is identical whether the publisher
//      is V906's wb_publish or (Route B) the V899 production handler. When the
//      real machine starts publishing, this window shows the real machine with
//      no code change here -- the "SIM" prefix simply stops appearing.
//   3. IT CANNOT HURT THE MACHINE. There is no write path. The window is a view.
//
//  THREADING CONTRACT -- the one rule that matters
//  TcpTagClient's worker thread IS the sink's publisher (TcpTagLink.h:203-207).
//  This dialog must therefore only ever READ the snapshot (TagSnapshot::read())
//  from the UI thread and must NEVER publish into it. Violating that is counted
//  by TagSnapshot::publisherViolations(), but by then the staging buffer has
//  already been raced on.
// =============================================================================
#ifndef MachineStateDlg_H
#define MachineStateDlg_H

#include <afxwin.h>
#include "resource.h"

#include "WebBridge/TagSnapshot.h"
#include "WebBridge/TcpTagLink.h"

#include <vector>

class CMachineStateDlg : public CDialog
{
public:
    // iSmokeCloseMs: 0 = interactive; >0 = EndDialog(IDOK) after that many ms.
    // Carried over from CGateAPlaceholderDlg so `--smoke N` keeps working
    // unchanged now that this is the default window -- the headless smoke gate in
    // build.bat depends on the exe opening, pumping and closing on its own.
    CMachineStateDlg(int iSmokeCloseMs, const char* szHost, unsigned short usPort,
                     CWnd* pParent = NULL);
    virtual ~CMachineStateDlg();

    enum { IDD = IDD_MACHINE_STATE };

protected:
    virtual BOOL OnInitDialog();
    virtual void OnDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    void BuildControls();      // creates every label/value from the tag table
    void Refresh();            // one read of the snapshot -> the value labels

    int            m_iSmokeCloseMs;
    std::string    m_sHost;
    unsigned short m_usPort;

    // The sink and its feeder. Declared in this order on purpose: the client's
    // worker publishes into the snapshot, so the snapshot must outlive it, and
    // members are destroyed in reverse declaration order.
    webbridge::TagSnapshot     m_snap;
    webbridge::TcpTagClient*   m_pClient;

    CFont   m_fontBig;         // the state word
    CFont   m_fontSub;         // provenance / link line
    CFont   m_fontRow;         // tag rows

    CStatic  m_wndState;       // machine.state, large
    CStatic  m_wndSource;      // machine.stateSource
    CStatic  m_wndLink;        // connection + frame counters

    std::vector<CStatic*> m_apRowValue;   // one per tag-table row, parallel index

    static const UINT_PTR kSmokeTimerId   = 1;
    static const UINT_PTR kRefreshTimerId = 2;
    static const UINT     kRefreshMs      = 250;
};

#endif // MachineStateDlg_H
