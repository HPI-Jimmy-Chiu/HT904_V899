// =============================================================================
//  ui/RegisterCustomClasses.cpp -- see RegisterCustomClasses.h for the full
//  rationale (S3-C1 single-entry-point ruling, class-name provenance, why
//  AfxRegisterClass instead of AfxRegisterWndClass, why DefWindowProc and a
//  NULL background brush).
//
//  AI(W906-GateA-4-C5) 20260804: NEW FILE.
// =============================================================================
// The header decides between <afxwin.h> and <windows.h> (see its own note on
// afxwin.h:90's "MFC requires use of Winsock2.h"), so nothing may be included
// before it here.
#include "RegisterCustomClasses.h"

#if defined(_AFXDLL) || defined(_AFX)
#define HT9045_MFC_REGISTRATION 1    // AfxRegisterClass / AfxGetInstanceHandle
#endif

// ---------------------------------------------------------------------------
//  The table. Split in two ON PURPOSE.
//
//  kShellBackedClasses: a real CWnd shell in this directory drives these
//  (ui/CLedCtrl.*, ui/CBtnPanelCtrl.*, ui/CTrayCtrl.*), which in turn drive
//  the W7-C1 cores and W7-C2 renderers. These are finished controls.
//
//  kPlaceholderClasses: named by the generated .rc, but no shell exists yet.
//  They are registered anyway, and that IS the right call rather than a
//  shortcut: an unregistered class in a dialog template fails the ENTIRE
//  CreateDialog (see header), so leaving these out would keep e.g. every form
//  containing a TStringGrid (120 instances) or a TShape (62) from opening at
//  all -- turning "one control is not translated yet" into "this form does
//  not exist". Registered-but-unsubclassed, they are inert transparent holes
//  (hbrBackground = NULL, DefWindowProc), which is visible during bring-up
//  instead of silently plausible. Every one of them is already tracked
//  elsewhere: TStringGrid/TDBGrid target the existing vclcompat::TStringGrid
//  shim, TEasyMain/TECameraPicolo are vendor SDK singletons on the SKIP list
//  (classmap.py SKIP_VENDOR_CLASSES / plan S9-R5), TAPHeadLabel has no source
//  at all and is an explicit SKIP-AND-RECORD approximation (plan S6/S4-V12).
// ---------------------------------------------------------------------------
static const char* const kShellBackedClasses[] =
{
    HT9045_CLASS_ALED,          //  530 .dfm instances -- CLedCtrl (LedCore)
    HT9045_CLASS_MYLED,         //   96                -- CLedCtrl (MyLedCore)
    HT9045_CLASS_MYLEDLANE,     // 1214                -- CLedCtrl (MyLedLaneCore)
    HT9045_CLASS_BTNPANEL,      //   56                -- CBtnPanelCtrl (BtnPanelCore)
    HT9045_CLASS_BTNPANELLANE,  //  701                -- CBtnPanelCtrl (BtnPanelLaneCore)
    HT9045_CLASS_MYTRAY,        //  246                -- CTrayCtrl (TrayCore)
    HT9045_CLASS_MYTRAY256      //    0 .dfm, 7 dynamic `new` sites -- CTrayCtrl (Tray256Core)
};

static const char* const kPlaceholderClasses[] =
{
    "HT9045.StringGrid",        //  120 (TStringGrid 119 + TDBGrid 1, same target shim)
    "HT9045.Shape",             //   62
    "HT9045.APHeadLabel",       //   10 -- source does not exist (plan S4-V12)
    "HT9045.Chart",             //    5
    "HT9045.EBW8ROI",           //    2
    "HT9045.CppWebBrowser",     //    2
    "HT9045.MediaPlayer",       //    1
    "HT9045.EasyMain",          //    1 -- vendor SDK singleton (SKIP, plan S9-R5)
    "HT9045.ECameraPicolo"      //    1 -- vendor SDK singleton (SKIP, plan S9-R5)
};

// ---------------------------------------------------------------------------
//  One class. Returns TRUE if the class is registered when we leave (already
//  being registered counts as success -- the function as a whole is required
//  to be idempotent, and a second InitInstance in the same process, or the
//  headless probe calling it twice, must not be a failure).
// ---------------------------------------------------------------------------
static BOOL RegisterOne(const char* pszClassName, HINSTANCE hInst)
{
#ifdef HT9045_MFC_REGISTRATION
    WNDCLASSA wc;
#else
    WNDCLASSEXA wc;
#endif
    ::ZeroMemory(&wc, sizeof(wc));
    wc.style         = CS_HREDRAW | CS_VREDRAW;   // see header (WM_SIZE repaint)
    wc.lpfnWndProc   = ::DefWindowProc;           // see header (MFC subclasses later)
    wc.hInstance     = hInst;
    wc.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;                      // see header (shells paint the whole client)
    wc.lpszClassName = pszClassName;

#ifdef HT9045_MFC_REGISTRATION
    return AfxRegisterClass(&wc);
#else
    wc.cbSize = sizeof(wc);
    if (::RegisterClassExA(&wc))
        return TRUE;
    // Already registered by an earlier call: not an error (idempotence).
    return (::GetLastError() == ERROR_CLASS_ALREADY_EXISTS) ? TRUE : FALSE;
#endif
}

BOOL HT9045_RegisterAllCustomClasses()
{
#ifdef HT9045_MFC_REGISTRATION
    HINSTANCE hInst = AfxGetInstanceHandle();
#else
    HINSTANCE hInst = ::GetModuleHandle(NULL);
#endif

    BOOL bAll = TRUE;
    int i;
    const int nShell = (int)(sizeof(kShellBackedClasses) / sizeof(kShellBackedClasses[0]));
    const int nPlace = (int)(sizeof(kPlaceholderClasses) / sizeof(kPlaceholderClasses[0]));

    // Every class is attempted even after one fails: a partial report ("which
    // ones are missing") is far more useful during bring-up than stopping at
    // the first, and the aggregate FALSE still tells the caller to abort.
    for (i = 0; i < nShell; ++i)
        if (!RegisterOne(kShellBackedClasses[i], hInst))
            bAll = FALSE;
    for (i = 0; i < nPlace; ++i)
        if (!RegisterOne(kPlaceholderClasses[i], hInst))
            bAll = FALSE;

    return bAll;
}

// ---------------------------------------------------------------------------
//  Notification helpers (see header for the two-family measurement that
//  justifies having both a WM_COMMAND route and a WM_NOTIFY route).
// ---------------------------------------------------------------------------
void HT9045_NotifyParentMouse(HWND hCtrl, UINT code, POINT pt, UINT nFlags)
{
    HWND hParent = ::GetParent(hCtrl);
    if (hParent == NULL)
        return;

    HT9045_NMMOUSE nm;
    nm.hdr.hwndFrom = hCtrl;
    nm.hdr.idFrom   = (UINT_PTR)::GetWindowLongPtr(hCtrl, GWLP_ID);
    nm.hdr.code     = code;
    nm.pt           = pt;
    nm.nFlags       = nFlags;
    ::SendMessage(hParent, WM_NOTIFY, (WPARAM)nm.hdr.idFrom, (LPARAM)&nm);
}

void HT9045_NotifyParentClicked(HWND hCtrl)
{
    HWND hParent = ::GetParent(hCtrl);
    if (hParent == NULL)
        return;

    int nId = (int)::GetWindowLongPtr(hCtrl, GWLP_ID);
    ::SendMessage(hParent, WM_COMMAND, MAKEWPARAM(nId, BN_CLICKED), (LPARAM)hCtrl);
}
