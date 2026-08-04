// =============================================================================
//  ui/CLedCtrl.h -- W7-C5 / GA-4-C5
//  MFC CWnd shell for the LED custom-control family: window classes
//  "HT9045.ALed" (530 .dfm instances), "HT9045.MyLed" (96) and
//  "HT9045.MyLedLane" (1,214) -- 1,840 instances, the highest-count custom
//  control in the codebase.
//
//  AI(W906-GateA-4-C5) 20260804: NEW FILE.
//
//  THE SHELL OWNS NO BEHAVIOUR. Every piece of state is vclcompat::LedCore
//  (W7-C1, translated from elec\Component\aled.pas) and every pixel is
//  vclcompat::render::RenderLed (W7-C2, which embeds the aled.res bitmaps).
//  This class only:
//     WM_PAINT / WM_PRINTCLIENT -> RenderLed
//     WM_ERASEBKGND             -> swallowed (RenderLed covers the client)
//     WM_TIMER                  -> LedCore::OnTimerTick + repaint
//     WM_LBUTTONDOWN/UP         -> parent notification
//  If a future change is tempted to compute a colour or a blink phase here,
//  it belongs in LedCore instead -- the core is part of the MinGW oracle and
//  is unit-tested there (tests/test_ledcore.cpp); anything reimplemented in
//  this MSVC-only file is invisible to that oracle.
//
//  WHY THE SHELL OWNS THE TIMER. LedCore.h's own banner (:28-34) states the
//  contract: "aled.pas has NO real OS timer in a framework-free core
//  (fLedTimer is a VCL TTimer -- a real Win32 timer needs a window to post
//  WM_TIMER to, which doesn't exist here) ... exposes OnTimerTick() as the
//  hook a real timer (owned by the eventual W7-C5 MFC shell) would call on
//  each tick". This file is that shell, so SyncTimer() below is the piece
//  W7-C1 explicitly deferred, not a new invention. golden aled.pas creates
//  the TTimer in SetLedTimer (:194-202), destroys it in ResetLedTimer
//  (:204-210) and re-arms Interval live at :285-286; the core models those
//  three as IsTimerActive()/GetInterval(), and SyncTimer() turns them into
//  SetTimer/KillTimer.
//
//  WHY THE FORWARDING SETTERS EXIST. LedCore's setters have side effects on
//  the timer (SetValue -> StartTimer/StopTimer, SetBlink -> ditto), so the
//  shell must re-read IsTimerActive() after any mutation. SetValue/SetBlink/
//  SetInterval below are one-line forwarders that do exactly that; a caller
//  that prefers to drive Core() directly must call SyncFromCore() afterwards.
//  No decision is duplicated -- the forwarders contain no policy at all.
//
//  ONE SHELL, THREE CLASSES. TMyLed and TMyLedLane add only IO-address
//  metadata (Port/Bit/Type/Ring/IP/Alias) on top of TALed and have ZERO
//  drawing code of their own (plan S4-V3), so one shell serves all three
//  registered class names. AttachCore() lets a form point the shell at the
//  MyLedCore / MyLedLaneCore instance the facade owns; the internal LedCore
//  is only the standalone default.
// =============================================================================
#ifndef HT9045_UI_CLEDCTRL_H
#define HT9045_UI_CLEDCTRL_H

#include <afxwin.h>
#include "vclcompat/LedCore.h"

class CLedCtrl : public CWnd
{
public:
    CLedCtrl();

    // Point the shell at a caller-owned core (a MyLedCore / MyLedLaneCore for
    // the .rc classes that carry IO metadata). NULL restores the internal one.
    // The shell never takes ownership.
    void AttachCore(vclcompat::LedCore* pCore);
    vclcompat::LedCore&       Core()       { return *m_pCore; }
    const vclcompat::LedCore& Core() const { return *m_pCore; }

    // Forwarders (see banner): mutate the core, then re-sync timer + repaint.
    void SetValue(bool v);
    void SetBlink(bool v);
    void SetInterval(long ms);

    // Call after mutating Core() directly.
    void SyncFromCore();

    // Timer id is per-window, so a fixed value is safe and makes the timer
    // recognisable in a debugger. Deliberately not 1 (the Gate A smoke timer).
    // Public so a headless test can inject a tick without a message pump.
    enum { kBlinkTimerId = 0x9045 };

protected:
    virtual void PreSubclassWindow();      // runs for BOTH creation paths in MFC:
                                            // CreateEx and DDX_Control/SubclassDlgItem
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnDestroy();
    afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

private:
    void DrawInto(HDC hdc);
    void SyncTimer();

    vclcompat::LedCore  m_ownCore;
    vclcompat::LedCore* m_pCore;
    UINT  m_uTimerInterval;    // 0 == no timer running
    BOOL  m_bClickPending;
};

#endif // HT9045_UI_CLEDCTRL_H
