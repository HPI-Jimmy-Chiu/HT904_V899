// =============================================================================
//  ui/MachineStateDlg.cpp -- see MachineStateDlg.h banner for the design and the
//  threading contract. AI(W906-StateWin) 20260814.
// =============================================================================
#include "MachineStateDlg.h"

#include "WebBridge/TagValue.h"

#include <cstdio>

// ---------------------------------------------------------------------------
//  The tag table IS the window's content. Adding a row here is the only edit
//  needed to show a new tag -- no resource change, no new control ID -- which is
//  the reason the dialog template is an empty shell.
//
//  Tag names are the wire's names, so they must match the publisher exactly
//  (WebBridgeTags.cpp). A name that does not exist on the wire is not an error:
//  it renders "<absent>", which is a true statement about this publisher and is
//  how a V899 feed carrying a different tag set will read.
// ---------------------------------------------------------------------------
namespace {

struct TagRow {
    const char* tag;
    const char* label;
};

const TagRow kRows[] = {
    { "clock.text",              "Clock"                  },
    { "machine.id.type",         "Machine type"           },
    { "machine.id.gpib",         "GPIB id"                },
    { "machine.customerCode",    "Customer code"          },
    { "",                        ""                       },   // spacer
    { "pump.guard.systemStart",  "guard SystemStart"      },
    { "pump.guard.allMotorHome", "guard fAllMotorHome"    },
    { "pump.guard.softStop",     "guard SoftStop"         },
    { "",                        ""                       },
    { "pump.mainProcCalls",      "MainProc calls"         },
    { "pump.ticks",              "Pump ticks"             },
    { "pump.exceptions",         "Tick exceptions"        },
    { "pump.alive",              "MainProc alive"         },
    { "pump.tickKind",           "Tick A/B"               },
    { "",                        ""                       },
    { "pump.task.load",          "Loader task"            },
    { "pump.task.inArm",         "InArm task"             },
    { "pump.task.outArm",        "OutArm task"            },
    { "pump.task.sht1",          "Shuttle 1 task"         },
    { "pump.task.sht2",          "Shuttle 2 task"         },
    { "pump.task.testHead",      "Index/TestHead task"    },
    { "pump.task.catchTray",     "TrayArm task"           },
};

const int kRowCount = (int)(sizeof(kRows) / sizeof(kRows[0]));

// Render one TagValue for a human.
//
// NULL RENDERS AS "---", NEVER AS 0. That is the same rule the publisher is built
// around (WebBridgeTags.h): on a machine that runs at 130 C, "0.00" and "---"
// mean very different things to whoever is standing in front of it. A window that
// showed 0 for a source nobody has read would be stating a measurement that was
// never taken.
CString FormatValue(const webbridge::TagValue& v)
{
    if (v.isNull())  return CString("---");
    if (v.isBool())  return CString(v.asBool() ? "true" : "false");
    if (v.isInt())
    {
        char buf[32];
        std::sprintf(buf, "%ld", (long)v.asInt());
        return CString(buf);
    }
    if (v.isDouble())
    {
        char buf[64];
        std::sprintf(buf, "%.3f", v.asDouble());
        return CString(buf);
    }
    if (v.isString()) return CString(v.asString().c_str());
    return CString("?");
}

} // namespace

BEGIN_MESSAGE_MAP(CMachineStateDlg, CDialog)
    ON_WM_TIMER()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

CMachineStateDlg::CMachineStateDlg(int iSmokeCloseMs, const char* szHost,
                                   unsigned short usPort, CWnd* pParent)
    : CDialog(IDD, pParent)
    , m_iSmokeCloseMs(iSmokeCloseMs)
    , m_sHost(szHost ? szHost : "127.0.0.1")
    , m_usPort(usPort)
    , m_pClient(NULL)
{
}

CMachineStateDlg::~CMachineStateDlg()
{
    // Stop the feeder BEFORE the snapshot it publishes into goes away. OnDestroy
    // normally already did this; doing it again is harmless and covers the path
    // where the dialog is destroyed without a WM_DESTROY (construction failure).
    if (m_pClient != NULL)
    {
        m_pClient->Stop();
        delete m_pClient;
        m_pClient = NULL;
    }
    for (std::size_t i = 0; i < m_apRowValue.size(); ++i) delete m_apRowValue[i];
    m_apRowValue.clear();
}

BOOL CMachineStateDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    BuildControls();

    // --- start the feed ----------------------------------------------------
    // "Cannot connect yet" is a NORMAL state, not a failure: this window is
    // expected to come up before the publisher and to survive it restarting
    // (TcpTagLink.h:211-214). So a failed Start is reported in the link line and
    // the window stays open rather than refusing to run.
    webbridge::TcpClientConfig cfg;
    cfg.host = m_sHost;
    cfg.port = m_usPort;

    m_pClient = new webbridge::TcpTagClient(m_snap, cfg);
    std::string err;
    if (!m_pClient->Start(err))
    {
        CString s;
        s.Format("link: FAILED to start (%s)", err.c_str());
        m_wndLink.SetWindowText(s);
    }

    Refresh();
    SetTimer(kRefreshTimerId, kRefreshMs, NULL);

    if (m_iSmokeCloseMs > 0)
        SetTimer(kSmokeTimerId, (UINT)m_iSmokeCloseMs, NULL);

    return TRUE;
}

void CMachineStateDlg::BuildControls()
{
    // Geometry is computed in PIXELS from the live client rect, not from the .rc
    // numbers. The template's units are dialog units, which the dialog manager
    // scales by the font, so any pixel constant written into the .rc would be
    // wrong by that factor -- the same DLU trap recorded for the generated forms
    // in docs/DESIGN_GA4_UI_ENGINES.md (D-GA4-2). Rows are laid out between the
    // header and whatever the Close button's real position turns out to be.
    CRect rcClient;
    GetClientRect(&rcClient);

    int iBottomLimit = rcClient.bottom - 8;
    CWnd* pClose = GetDlgItem(IDOK);
    if (pClose != NULL)
    {
        CRect rcClose;
        pClose->GetWindowRect(&rcClose);
        ScreenToClient(&rcClose);
        iBottomLimit = rcClose.top - 8;
    }

    m_fontBig.CreatePointFont(220, "Segoe UI");
    m_fontSub.CreatePointFont(90,  "Segoe UI");
    m_fontRow.CreatePointFont(90,  "Consolas");

    const int kLeft = 12;
    const int kRight = rcClient.right - 12;

    int y = 10;

    CRect rc(kLeft, y, kRight, y + 34);
    m_wndState.Create("---", WS_CHILD | WS_VISIBLE | SS_LEFT, rc, this,
                      IDC_STATEWIN_FIRST);
    m_wndState.SetFont(&m_fontBig);
    y += 36;

    rc.SetRect(kLeft, y, kRight, y + 16);
    m_wndSource.Create("", WS_CHILD | WS_VISIBLE | SS_LEFT, rc, this,
                       IDC_STATEWIN_FIRST + 1);
    m_wndSource.SetFont(&m_fontSub);
    y += 18;

    rc.SetRect(kLeft, y, kRight, y + 16);
    m_wndLink.Create("link: starting...", WS_CHILD | WS_VISIBLE | SS_LEFT, rc, this,
                     IDC_STATEWIN_FIRST + 2);
    m_wndLink.SetFont(&m_fontSub);
    y += 24;

    // --- tag rows ----------------------------------------------------------
    const int kRowH      = 15;
    const int kLabelW    = 150;
    const int kValueLeft = kLeft + kLabelW + 8;

    for (int i = 0; i < kRowCount; ++i)
    {
        if (y + kRowH > iBottomLimit)
        {
            // Stop rather than draw over the button. Silently dropping rows would
            // read as "that tag is not published", so say what happened instead.
            CRect rcMore(kLeft, y, kRight, y + kRowH);
            CStatic* pMore = new CStatic();
            pMore->Create("(window too small -- remaining rows not shown)",
                          WS_CHILD | WS_VISIBLE | SS_LEFT, rcMore, this,
                          IDC_STATEWIN_FIRST + 3 + (UINT)(kRowCount * 2));
            pMore->SetFont(&m_fontSub);
            m_apRowValue.push_back(pMore);   // owned for cleanup; never updated
            break;
        }

        if (kRows[i].tag[0] == '\0')     // spacer row
        {
            m_apRowValue.push_back(NULL);
            y += kRowH / 2 + 2;
            continue;
        }

        CRect rcLabel(kLeft, y, kLeft + kLabelW, y + kRowH);
        CStatic* pLabel = new CStatic();
        pLabel->Create(kRows[i].label, WS_CHILD | WS_VISIBLE | SS_LEFT, rcLabel,
                       this, IDC_STATEWIN_FIRST + 3 + (UINT)(i * 2));
        pLabel->SetFont(&m_fontRow);
        // Labels never change, but they are heap CStatics like the values, so they
        // are pushed onto the same vector purely so the destructor frees them.
        m_apRowValue.push_back(pLabel);

        CRect rcValue(kValueLeft, y, kRight, y + kRowH);
        CStatic* pValue = new CStatic();
        pValue->Create("---", WS_CHILD | WS_VISIBLE | SS_LEFT, rcValue, this,
                       IDC_STATEWIN_FIRST + 4 + (UINT)(i * 2));
        pValue->SetFont(&m_fontRow);
        m_apRowValue.push_back(pValue);

        y += kRowH;
    }
}

void CMachineStateDlg::Refresh()
{
    // READ ONLY. TcpTagClient's worker owns the publish path (MachineStateDlg.h
    // threading contract); this must never stage or commit into m_snap.
    const webbridge::TagSnapshotView view = m_snap.read();

    webbridge::TagMap::const_iterator it = view.tags.find("machine.state");
    if (it != view.tags.end() && !it->second.isNull())
        m_wndState.SetWindowText(FormatValue(it->second));
    else if (view.generation == 0)
        m_wndState.SetWindowText("(no data)");
    else
        m_wndState.SetWindowText("---");

    it = view.tags.find("machine.stateSource");
    m_wndSource.SetWindowText(
        (it != view.tags.end() && !it->second.isNull())
            ? FormatValue(it->second)
            : CString("state provenance: not reported by this publisher"));

    if (m_pClient != NULL)
    {
        const webbridge::TcpTagClient::Stats st = m_pClient->GetStats();
        CString s;
        s.Format("link %s  %s:%u   frames=%lu  gen=%lu  reconnects=%lu  parseErr=%lu",
                 st.connected ? "CONNECTED" : "offline",
                 m_sHost.c_str(), (unsigned)m_usPort,
                 (unsigned long)st.framesReceived,
                 (unsigned long)st.lastGeneration,
                 (unsigned long)st.reconnects,
                 (unsigned long)st.parseErrors);
        m_wndLink.SetWindowText(s);
    }

    // Row values live at the odd offsets of the parallel vector; see BuildControls.
    std::size_t iSlot = 0;
    for (int i = 0; i < kRowCount && iSlot < m_apRowValue.size(); ++i)
    {
        if (kRows[i].tag[0] == '\0') { ++iSlot; continue; }   // spacer
        if (iSlot + 1 >= m_apRowValue.size()) break;          // truncated window

        CStatic* pValue = m_apRowValue[iSlot + 1];
        iSlot += 2;
        if (pValue == NULL) continue;

        webbridge::TagMap::const_iterator f = view.tags.find(kRows[i].tag);
        pValue->SetWindowText(f == view.tags.end() ? CString("<absent>")
                                                  : FormatValue(f->second));
    }
}

void CMachineStateDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == kRefreshTimerId)
    {
        Refresh();
        return;
    }
    if (nIDEvent == kSmokeTimerId)
    {
        KillTimer(kSmokeTimerId);
        EndDialog(IDOK);    // smoke run: prove open->pump->close without a human
        return;
    }
    CDialog::OnTimer(nIDEvent);
}

void CMachineStateDlg::OnDestroy()
{
    KillTimer(kRefreshTimerId);
    KillTimer(kSmokeTimerId);

    // Stop the worker while the window (and the snapshot) are still alive, for the
    // same ordering reason FormRuntime documents for its shells: tearing down the
    // producer after its sink is gone is the bug this avoids.
    if (m_pClient != NULL) m_pClient->Stop();

    CDialog::OnDestroy();
}
