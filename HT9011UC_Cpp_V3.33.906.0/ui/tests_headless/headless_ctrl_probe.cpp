// =============================================================================
//  ui/tests_headless/headless_ctrl_probe.cpp -- W7-C5 / GA-4-C5 verification
//
//  AI(W906-GateA-4-C5) 20260804: NEW FILE.
//
//  Proves, with no screen and no possibility of a window appearing, that:
//    1. HT9045_RegisterAllCustomClasses() registers every custom class named
//       by the generated .rc files, and is idempotent.
//    2. All three CWnd shells can be created against those classes.
//    3. Painting works end to end -- a real WM_PRINT/WM_PRINTCLIENT round
//       trip through the shell into the W7-C2 renderer, verified by reading
//       back a pixel, not merely by "it did not crash".
//    4. Core state survives the shell: set/get round-trips through the shell
//       forwarders and through Core() directly.
//
//  WHY IT CANNOT POP A WINDOW (plan S5-11 / S7's own argument, and the reason
//  this is structural rather than disciplinary): the parent is created with
//  HWND_MESSAGE, i.e. a message-only window, which "is not visible, has no
//  z-order, cannot be enumerated and does not receive broadcast messages".
//  Its children inherit that -- there is no path by which they reach the
//  desktop. SetErrorMode() is called first regardless, per S5-11, so a fault
//  cannot raise a WER dialog either.
//
//  This is a console exe: MFC needs AfxWinInit(), and a CWinApp object must
//  exist so AfxGetInstanceHandle()/module state resolve. Its InitInstance is
//  never called (there is no AfxWinMain here) -- main() below is the entry
//  point and it never pumps a message loop.
//
//  Exit code 0 = every check passed. Non-zero = number of failed checks (or
//  a hard setup failure); every check prints PASS/FAIL with its own line.
// =============================================================================
#include <afxwin.h>
#include <stdio.h>

#include "RegisterCustomClasses.h"
#include "CLedCtrl.h"
#include "CBtnPanelCtrl.h"
#include "CTrayCtrl.h"

CWinApp theProbeApp;    // module-state anchor only; InitInstance never runs

static int g_nFail = 0;

static void Check(bool bOk, const char* pszWhat)
{
    printf("  %s  %s\n", bOk ? "PASS" : "FAIL", pszWhat);
    if (!bOk)
        ++g_nFail;
}

static bool ClassRegistered(const char* pszName)
{
    WNDCLASSEXA wc;
    ::ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    if (::GetClassInfoExA(AfxGetInstanceHandle(), pszName, &wc))
        return true;
    // A class registered with CS_GLOBALCLASS, or by another module, answers
    // to a NULL instance handle -- accept that too rather than report a
    // false negative.
    return ::GetClassInfoExA(NULL, pszName, &wc) != FALSE;
}

int main()
{
    // S5-11: a program run in a background/batch context must have no path to
    // a modal box. This is the very first statement for that reason.
    ::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
    {
        printf("headless_ctrl_probe: AfxWinInit failed\n");
        return 90;
    }

    printf("== HT9045 headless custom-control probe (W7-C5 / GA-4-C5) ==\n");

    // ---- 1. registration --------------------------------------------------
    Check(HT9045_RegisterAllCustomClasses() != FALSE, "RegisterAllCustomClasses (1st call)");
    Check(HT9045_RegisterAllCustomClasses() != FALSE, "RegisterAllCustomClasses (2nd call, idempotent)");

    Check(ClassRegistered(HT9045_CLASS_ALED),          "class HT9045.ALed registered");
    Check(ClassRegistered(HT9045_CLASS_MYLED),         "class HT9045.MyLed registered");
    Check(ClassRegistered(HT9045_CLASS_MYLEDLANE),     "class HT9045.MyLedLane registered");
    Check(ClassRegistered(HT9045_CLASS_BTNPANEL),      "class HT9045.BtnPanel registered");
    Check(ClassRegistered(HT9045_CLASS_BTNPANELLANE),  "class HT9045.BtnPanelLane registered");
    Check(ClassRegistered(HT9045_CLASS_MYTRAY),        "class HT9045.MyTray registered");
    Check(ClassRegistered(HT9045_CLASS_MYTRAY256),     "class HT9045.MyTray256 registered");
    Check(ClassRegistered("HT9045.StringGrid"),        "placeholder class HT9045.StringGrid registered");
    Check(ClassRegistered("HT9045.APHeadLabel"),       "placeholder class HT9045.APHeadLabel registered");

    // ---- 2. message-only parent ------------------------------------------
    HWND hHost = ::CreateWindowExA(0, "STATIC", "HT9045 headless host", 0,
                                   0, 0, 0, 0, HWND_MESSAGE, NULL,
                                   AfxGetInstanceHandle(), NULL);
    if (hHost == NULL)
    {
        printf("headless_ctrl_probe: message-only host creation failed, err=%lu\n",
               (unsigned long)::GetLastError());
        return 91;
    }
    CWnd host;
    host.Attach(hHost);

    // ---- 3. create one of each shell --------------------------------------
    CLedCtrl      led;
    CBtnPanelCtrl btn;
    CTrayCtrl     tray;

    Check(led.CreateEx(0, HT9045_CLASS_MYLEDLANE, NULL, WS_CHILD,
                       CRect(0, 0, 32, 16), &host, 101) != FALSE,
          "CLedCtrl::CreateEx on HT9045.MyLedLane");
    Check(btn.CreateEx(0, HT9045_CLASS_BTNPANELLANE, NULL, WS_CHILD,
                       CRect(0, 0, 40, 20), &host, 102) != FALSE,
          "CBtnPanelCtrl::CreateEx on HT9045.BtnPanelLane");
    Check(tray.CreateEx(0, HT9045_CLASS_MYTRAY, NULL, WS_CHILD,
                        CRect(0, 0, 100, 200), &host, 103) != FALSE,
          "CTrayCtrl::CreateEx on HT9045.MyTray");

    if (led.GetSafeHwnd() == NULL || btn.GetSafeHwnd() == NULL || tray.GetSafeHwnd() == NULL)
    {
        printf("headless_ctrl_probe: control creation failed, err=%lu -- aborting\n",
               (unsigned long)::GetLastError());
        host.Detach();
        ::DestroyWindow(hHost);
        return 92;
    }
    Check(!led.IsWindowVisible() && !btn.IsWindowVisible() && !tray.IsWindowVisible(),
          "no control is visible (message-only lineage)");

    // ---- 4. core state round-trips ----------------------------------------
    // LED: the shell forwarders must move core state AND the timer with it
    // (LedCore StartTimer/StopTimer fire from inside SetValue/SetBlink).
    led.SetInterval(250);
    led.SetValue(true);
    led.SetBlink(true);
    Check(led.Core().GetValue() == true,        "LED Value round-trip (true)");
    Check(led.Core().GetBlink() == true,        "LED Blink round-trip (true)");
    Check(led.Core().GetInterval() == 250,      "LED Interval round-trip (250)");
    Check(led.Core().IsTimerActive() == true,   "LED core timer active while blinking");

    bool bColorTempBefore = led.Core().ColorTemp;
    led.SendMessage(WM_TIMER, (WPARAM)CLedCtrl::kBlinkTimerId, 0);
    Check(led.Core().ColorTemp != bColorTempBefore,
          "LED WM_TIMER reaches LedCore::OnTimerTick (ColorTemp flipped)");

    led.SetValue(false);
    Check(led.Core().IsTimerActive() == false,  "LED core timer stops when Value goes false");

    // Button: SetDown must pull Color/FontColor/bevels across from the core.
    btn.Core().SetTrueColor(vclcompat::clRed);
    btn.Core().SetFalseColor(vclcompat::clSilver);
    btn.SetDown(true);
    Check(btn.Core().GetDown() == true,                          "Btn Down round-trip (true)");
    Check(btn.Core().Color == vclcompat::clRed,                  "Btn Color follows TrueColor when down");
    Check(btn.Core().BevelInner == vclcompat::bvLowered,         "Btn BevelInner lowered when down");
    btn.SetDown(false);
    Check(btn.Core().Color == vclcompat::clSilver,               "Btn Color follows FalseColor when up");

    // Tray: WM_SIZE during creation must have reached TrayCore::SetExtents.
    Check(tray.Core().Width == 100 && tray.Core().Height == 200,
          "Tray extents taken from the HWND client rect (100x200)");
    tray.Core().SetCellNumber(0, 0, vclcompat::AnsiString("A7"));
    Check(tray.Core().GetCellText(0, 0) == "A7",                 "Tray cell text round-trip");

    // ---- 5. real painting into a memory DC --------------------------------
    HDC hMem = ::CreateCompatibleDC(NULL);
    BITMAPINFO bi;
    ::ZeroMemory(&bi, sizeof(bi));
    bi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth       = 128;
    bi.bmiHeader.biHeight      = -256;      // top-down
    bi.bmiHeader.biPlanes      = 1;
    bi.bmiHeader.biBitCount    = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    void*   pBits = NULL;
    HBITMAP hbm   = ::CreateDIBSection(hMem, &bi, DIB_RGB_COLORS, &pBits, NULL, 0);
    HGDIOBJ hOld  = ::SelectObject(hMem, hbm);

    // Button first: it has the cleanest single-colour oracle (a flat fill of
    // core.Color), so a matching pixel proves the whole chain -- WM_PRINT ->
    // DefWindowProc -> WM_PRINTCLIENT -> shell -> RenderBtnPanel -> GDI.
    ::PatBlt(hMem, 0, 0, 128, 256, BLACKNESS);
    btn.SetDown(true);      // Color = clRed
    btn.SendMessage(WM_PRINT, (WPARAM)hMem, PRF_CLIENT);
    COLORREF crBtn = ::GetPixel(hMem, 8, 8);
    Check(crBtn == RGB(255, 0, 0), "Btn WM_PRINT paints core.Color (clRed) into a memory DC");
    if (crBtn != RGB(255, 0, 0))
        printf("        (measured 0x%08lX, expected 0x%08lX)\n",
               (unsigned long)crBtn, (unsigned long)RGB(255, 0, 0));

    // Tray: whole-control FTrayColor (clWhite) fill, then the grid
    // (HTray.cpp:278-282 + the cell loop). The sample point matters and was
    // chosen against the default 2x2 grid, not at random: (50,100) -- the
    // geometric centre of a 100x200 tray -- is exactly ON the black grid line
    // between the four cells (measured: it read back RGB(0,0,0) and failed
    // this check on the first run), and the top-left 10x10 corner carries the
    // csLeftTop direction ticks (FDirectWidth=10). (15,30) is interior to
    // cell (0,0), clear of the ticks and clear of the centred "5" glyph.
    ::PatBlt(hMem, 0, 0, 128, 256, BLACKNESS);
    tray.SendMessage(WM_PRINT, (WPARAM)hMem, PRF_CLIENT);
    COLORREF crTray = ::GetPixel(hMem, 15, 30);
    Check(crTray == RGB(255, 255, 255),
          "Tray WM_PRINT paints the cell fill (clWhite) into a memory DC");
    if (crTray != RGB(255, 255, 255))
        printf("        (measured 0x%08lX at (15,30), expected 0x%08lX)\n",
               (unsigned long)crTray, (unsigned long)RGB(255, 255, 255));

    // LED: the aled.res art is stretch-blitted, and its exact pixels are
    // already covered by the W7-C2 renderer's own tests, so this asserts the
    // weaker but still failable property that SOMETHING landed inside the
    // control's 32x16 rect over the blacked-out DC. (A `GetPixel(...) !=
    // CLR_INVALID` check was written here first and then removed: on a valid
    // DC and an in-range point it can never fail, so it would have passed
    // even with the paint route completely broken.)
    ::PatBlt(hMem, 0, 0, 128, 256, BLACKNESS);
    led.SendMessage(WM_PRINT, (WPARAM)hMem, PRF_CLIENT);
    bool bLedPainted = false;
    for (int y = 0; y < 16 && !bLedPainted; ++y)
        for (int x = 0; x < 32 && !bLedPainted; ++x)
            if (::GetPixel(hMem, x, y) != RGB(0, 0, 0))
                bLedPainted = true;
    Check(bLedPainted, "LED WM_PRINT paints the aled.res art into a memory DC");

    ::SelectObject(hMem, hOld);
    ::DeleteObject(hbm);
    ::DeleteDC(hMem);

    // ---- 6. teardown ------------------------------------------------------
    led.DestroyWindow();
    btn.DestroyWindow();
    tray.DestroyWindow();
    host.Detach();
    ::DestroyWindow(hHost);

    printf("== %s (%d failure%s) ==\n", (g_nFail == 0) ? "ALL PASS" : "FAILURES",
           g_nFail, (g_nFail == 1) ? "" : "s");
    return g_nFail;
}
