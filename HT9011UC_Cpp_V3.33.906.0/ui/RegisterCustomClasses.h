// =============================================================================
//  ui/RegisterCustomClasses.h -- W7-C5 / GA-4-C5
//  ONE global registration entry point for every self-made VCL control class
//  named by the generated dialog templates, plus the tiny parent-notification
//  protocol the three CWnd shells share.
//
//  AI(W906-GateA-4-C5) 20260804: NEW FILE.
//
//  WHY A SINGLE FUNCTION (plan W7_UI_ARCHITECTURE_PLAN.md S3-C1 ruling,
//  :110): "registered window class must be registered before any
//  CreateDialog. Implementation = one HT9045_RegisterAllCustomClasses(),
//  called once from App InitInstance (NOT 39 forms each doing their own)".
//  This file is that function. The consequence is load-bearing, not
//  cosmetic: when a dialog template names a window class that is not
//  registered, the dialog manager's CreateWindowEx for that control fails
//  and CreateDialogIndirectParam returns NULL -- i.e. the WHOLE form fails
//  to come up, not just one control. 15 of the 261 generated .rc files'
//  class strings are ours, so without this call almost nothing opens.
//
//  *** CALLER STILL MISSING (handoff, GA-4 owner) ***
//  Nothing in this tree calls HT9045_RegisterAllCustomClasses() yet.
//  ui/HT9045App.cpp::InitInstance is outside this task's write
//  authorization, so the one line it needs --
//      if (!HT9045_RegisterAllCustomClasses()) { /* log + fail */ }
//  placed BEFORE the first dialog is created -- must be added by whoever
//  owns HT9045App.cpp. Until then only ui/tests_headless/
//  headless_ctrl_probe.cpp exercises it.
//
//  CLASS-NAME PROVENANCE (measured, not invented). The names below are
//  exactly what the .rc generator already emits, per
//      grep -rhoE '"HT9045\.[A-Za-z0-9_]+"' tools/dfm2rc/rc_out --include=*.rc
//  run 2026-08-04 over all 261 generated .rc files:
//      1214 "HT9045.MyLedLane"    701 "HT9045.BtnPanelLane"
//       530 "HT9045.ALed"         246 "HT9045.MyTray"
//       120 "HT9045.StringGrid"    96 "HT9045.MyLed"
//        62 "HT9045.Shape"         56 "HT9045.BtnPanel"
//        10 "HT9045.APHeadLabel"    5 "HT9045.Chart"
//         2 "HT9045.EBW8ROI"        2 "HT9045.CppWebBrowser"
//         1 "HT9045.MediaPlayer"    1 "HT9045.EasyMain"
//         1 "HT9045.ECameraPicolo"
//  ...plus "HT9045.MyTray256", which tools/dfm2rc/classmap.py:145 declares
//  but which no .rc emits (0 .dfm instances -- it is created dynamically at
//  golden cBinSel.cpp:232-235 and in uYieldMonitoring, see plan S4-V12).
//  The generator's own table is tools/dfm2rc/classmap.py:130-160
//  (`_custom(name)` -> 'HT9045.%s'). Per the brief, generator output is the
//  authority; nothing here was named by hand.
//
//  WINDOW-CLASS SHAPE (each decision has a reason; see .cpp for the table):
//   * lpfnWndProc = ::DefWindowProc, NOT AfxWndProc. These HWNDs are born
//     from a dialog template with no CWnd attached yet; AfxWndProc requires
//     a CWnd already in the permanent handle map and asserts otherwise. The
//     MFC shells attach LATER (DDX_Control / SubclassDlgItem), and MFC's
//     subclassing swaps the proc at that point -- which is the standard,
//     documented way an MFC custom control works.
//   * hbrBackground = NULL. All three shells' WM_PAINT covers the entire
//     client rect (RenderTray fills (0,0,Width,Height) with FTrayColor,
//     RenderBtnPanel FillRect's the whole rect with core.Color, RenderLed
//     stretch-blits the art over the whole destRect), so a class background
//     brush would only add a flicker frame. Documented failure mode: a
//     control of one of these classes that nobody subclasses paints nothing
//     at all and shows the parent through. That is deliberate -- a blank
//     hole is a visible "you forgot the DDX" signal, whereas a themed grey
//     brush would look like a working control.
//   * CS_HREDRAW|CS_VREDRAW: golden TTMyTray recomputes geometry and repaints
//     on every WM_SIZE (HTray.cpp:81-85), and TCustomControl/TGraphicControl
//     repaint wholesale on resize; without these flags a grown control would
//     show a stale partial image.
//   * No CS_DBLCLKS: zero of the 2,853 .dfm instances bind OnDblClick
//     (measured over all 133 tools/dfm2rc/ir_out/*.ir.json), so asking for
//     double-click messages would be inventing an event golden never has.
//
//  REGISTRATION API -- AfxRegisterClass, and why NOT AfxRegisterWndClass:
//  plan S7-7 (:545) says "the actual AfxRegisterWndClass calls". That API
//  name is wrong for this job and is corrected here: AfxRegisterWndClass
//  SYNTHESIZES the class name ("Afx:%p:%x:...") and RETURNS it -- the caller
//  cannot choose it. D12 requires the exact strings above because they are
//  already baked into 261 generated .rc files. AfxRegisterClass(WNDCLASS*)
//  is the MFC API that honours a caller-supplied lpszClassName, and it also
//  (a) is idempotent by construction (it GetClassInfo's first and returns
//  TRUE if the class already exists) and (b) records the class in the MFC
//  module state so it is unregistered when the module unloads -- which a
//  bare RegisterClassEx would not do. The pure-Win32 RegisterClassExA path
//  is kept for a non-MFC consumer and is selected automatically when neither
//  _AFXDLL nor _AFX is defined; both paths build the identical WNDCLASS.
// =============================================================================
#ifndef HT9045_UI_REGISTERCUSTOMCLASSES_H
#define HT9045_UI_REGISTERCUSTOMCLASSES_H

// MFC must be the first thing in the translation unit when it is present:
// afxwin.h:90 is `#error MFC requires use of Winsock2.h` and it fires the
// moment a bare <windows.h> got there first (measured 2026-08-04 -- this
// exact error killed the first build of this file). Pulling afxwin.h from
// here, rather than <windows.h>, makes the header safe to include in any
// order inside the MFC build while still standing alone in the pure-Win32
// build. Same family of problem as W7-A0's vcl_compat.h include-order fix.
#if defined(_AFXDLL) || defined(_AFX)
#include <afxwin.h>
#else
#include <windows.h>
#endif

// ---------------------------------------------------------------------------
//  Class-name strings. Use these macros, never a hand-typed literal: they are
//  the join key between the generated .rc (D12) and the shells, and a typo in
//  either place is a silent whole-dialog creation failure.
// ---------------------------------------------------------------------------
#define HT9045_CLASS_ALED           "HT9045.ALed"
#define HT9045_CLASS_MYLED          "HT9045.MyLed"
#define HT9045_CLASS_MYLEDLANE      "HT9045.MyLedLane"
#define HT9045_CLASS_BTNPANEL       "HT9045.BtnPanel"
#define HT9045_CLASS_BTNPANELLANE   "HT9045.BtnPanelLane"
#define HT9045_CLASS_MYTRAY         "HT9045.MyTray"
#define HT9045_CLASS_MYTRAY256      "HT9045.MyTray256"

// ---------------------------------------------------------------------------
//  Parent-notification protocol shared by the three shells.
//
//  Two DISTINCT golden event families, measured over all 133
//  tools/dfm2rc/ir_out/*.ir.json (2026-08-04), not one duplicated in two
//  shapes:
//      OnClick     : BtnPanel family 714, LED family 25, Tray 0
//      OnMouseDown : BtnPanel 32, Tray 32, LED 0
//      OnMouseUp   : BtnPanel  1, Tray  9, LED 0
//      (OnMouseMove: Tray 14 -- NOT forwarded, see the gap note below)
//
//  So a click and a mouse-down are separately observable in golden and both
//  need a route:
//   * click  -> WM_COMMAND, MAKEWPARAM(ctrl-id, BN_CLICKED), lParam = HWND.
//     Deliberately the STOCK button notification rather than a private code:
//     it makes an ordinary ON_BN_CLICKED(IDC_..., handler) entry in the
//     parent's message map work unchanged, which is what the eventual
//     message-map wave (plan S7-5, 4,519 OnClick bindings tree-wide) wants.
//   * mouse down/up -> WM_NOTIFY with HT9045_NMMOUSE, carrying the point and
//     the MK_* flags, because WM_COMMAND has nowhere to put coordinates and
//     golden's OnMouseDown(Sender, Button, Shift, X, Y) needs them (all 32
//     Tray OnMouseDown handlers exist to turn X/Y into a cell via
//     ConvertIndexCells -- 16 call sites in golden form code).
//
//  KNOWN GAP (recorded, not silently dropped): WM_MOUSEMOVE is NOT
//  forwarded, so the 14 Tray OnMouseMove bindings have no route yet. That is
//  not an oversight of scope but a deferred design call -- move messages
//  arrive in floods and whether they should cross the control/form boundary
//  one-for-one is a binder-wave (W7-U0) decision, not something this shell
//  wave should settle by accident.
// ---------------------------------------------------------------------------
// NMHDR::code values. 0x9045xx is chosen to sit far away from both the
// common-control ranges (which are (UINT)0-0 .. (UINT)0-1199, i.e. values
// near UINT_MAX) and from any small user code, so a stray notification is
// unambiguous when it turns up in a debugger.
#define HT9045N_LBUTTONDOWN     0x90450001u
#define HT9045N_LBUTTONUP       0x90450002u

typedef struct tagHT9045_NMMOUSE
{
    NMHDR hdr;      // hdr.code = HT9045N_LBUTTONDOWN / HT9045N_LBUTTONUP
    POINT pt;       // client coordinates of the control, as WM_LBUTTON* gave them
    UINT  nFlags;   // the MK_* wParam of the original message
} HT9045_NMMOUSE;

// Sends the WM_NOTIFY above to hCtrl's parent. No-op if hCtrl has no parent.
void HT9045_NotifyParentMouse(HWND hCtrl, UINT code, POINT pt, UINT nFlags);

// Sends WM_COMMAND / BN_CLICKED to hCtrl's parent (golden OnClick).
void HT9045_NotifyParentClicked(HWND hCtrl);

// ---------------------------------------------------------------------------
//  Registers every custom class listed in the .cpp table. Idempotent: calling
//  it twice succeeds (an already-registered class is not an error).
//  Returns TRUE only if every class is registered afterwards.
// ---------------------------------------------------------------------------
BOOL HT9045_RegisterAllCustomClasses();

#endif // HT9045_UI_REGISTERCUSTOMCLASSES_H
