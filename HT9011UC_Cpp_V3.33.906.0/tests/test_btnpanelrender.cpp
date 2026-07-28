// ===========================================================================
//  tests/test_btnpanelrender.cpp
//  Headless HDC pixel-test harness for vclcompat::render::RenderBtnPanel
//  (W7-C2). Same technique as test_trayrender.cpp/test_ledrender.cpp (memory
//  DC + CreateDIBSection, no window ever created -- PROHIBITION 3).
//
//  Deliberately small: RenderBtnPanel paints ONLY a flat Color fill (see
//  BtnPanelRender.h's banner for why -- TBtnPanel/TBtnPanelLane have zero
//  custom Paint code in golden, and the generic TPanel bevel they inherit
//  is out of scope for a golden-fidelity translation). There is exactly one
//  behaviour to verify: the rendered fill matches core.Color, correctly
//  resolved through the system-colour-reference path when applicable.
//
//  AI(W906-W7-C2) 20260728: added this file's convention tag (LOW-3 audit
//  -- no functional change).
// ===========================================================================
#include <windows.h>
#include <cstdio>
#include <memory>

#include "vclcompat/BtnPanelCore.h"
#include "vclcompat/render/BtnPanelRender.h"

using namespace vclcompat;
using namespace vclcompat::render;

#define NEW_HEAP(Type, Name) std::unique_ptr<Type> Name##_owner(new Type()); Type& Name = *Name##_owner

static int g_fail = 0;
static int g_total = 0;

static void check(bool cond, const char* expr, const char* file, int line) {
    ++g_total;
    if (!cond) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n", file, line, expr);
    }
}
#define CHECK(cond) check((cond), #cond, __FILE__, __LINE__)

namespace {

struct Canvas {
    HDC screen;
    HDC mem;
    HBITMAP dib;
    HGDIOBJ oldBmp;
    unsigned* px;
    int w, h;

    Canvas(int width, int height) : w(width), h(height) {
        screen = ::GetDC(NULL);   // reference DC only -- no window created/shown
        mem = ::CreateCompatibleDC(screen);
        BITMAPINFO bi;
        ZeroMemory(&bi, sizeof(bi));
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = w;
        bi.bmiHeader.biHeight = -h;
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;
        void* bits = 0;
        dib = ::CreateDIBSection(mem, &bi, DIB_RGB_COLORS, &bits, NULL, 0);
        oldBmp = ::SelectObject(mem, dib);
        px = static_cast<unsigned*>(bits);
    }
    ~Canvas() {
        ::SelectObject(mem, oldBmp);
        ::DeleteObject(dib);
        ::DeleteDC(mem);
        ::ReleaseDC(NULL, screen);
    }
    unsigned At(int x, int y) const { return px[y * w + x] & 0x00FFFFFFu; }
};

const unsigned kGreen = 0x00FF00u;   // clLime, as read back from a rendered pixel (see
const unsigned kRed   = 0xFF0000u;   // test_trayrender.cpp's identical banner on byte order)

} // namespace

int main() {
    ::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

    // =======================================================================
    //  Freshly-constructed BtnPanelCore: Color==clBtnFace (a SYSTEM colour
    //  reference, butPa1.cpp:33 via the locally-shadowed tcFalseColor --
    //  BtnPanelCore.h/.cpp's own GOLDEN BUG note). Must resolve through
    //  GetSysColor(COLOR_BTNFACE), not be painted as a literal (and wrong)
    //  0x0F0000-ish colour from a naive direct cast. Compared against
    //  GetSysColor() queried AT TEST TIME (not a hardcoded hex) so this
    //  stays correct across whatever theme the test happens to run under.
    // =======================================================================
    {
        Canvas c(20, 20);
        NEW_HEAP(BtnPanelCore, p);
        CHECK((unsigned)p.Color == 0x8000000Fu);   // clBtnFace, unresolved raw state

        RECT rc = {0, 0, 20, 20};
        RenderBtnPanel(c.mem, rc, p);

        unsigned expected = ::GetSysColor(COLOR_BTNFACE) & 0x00FFFFFFu;
        CHECK(c.At(5, 5) == expected);
        CHECK(c.At(19, 19) == expected);   // fills the WHOLE rect, corner included (plain FillRect)
    }

    // =======================================================================
    //  SetTrueColor/SetFalseColor/SetDown -- the Color field this renderer
    //  actually paints; toggling Down flips it, as BtnPanelCoreBase::SetDown
    //  (BtnPanelCore.cpp, translating butPa1.cpp SetPanelStatus) already
    //  established and this file re-verifies through to rendered pixels.
    // =======================================================================
    {
        Canvas c(20, 20);
        NEW_HEAP(BtnPanelCore, p);
        p.SetTrueColor(clLime);
        p.SetFalseColor(clRed);
        RECT rc = {0, 0, 20, 20};

        p.SetDown(false);
        RenderBtnPanel(c.mem, rc, p);
        CHECK(c.At(10, 10) == kRed);

        p.SetDown(true);
        RenderBtnPanel(c.mem, rc, p);
        CHECK(c.At(10, 10) == kGreen);

        p.SetDown(false);
        RenderBtnPanel(c.mem, rc, p);
        CHECK(c.At(10, 10) == kRed);
    }

    // =======================================================================
    //  BtnPanelLaneCore shares BtnPanelCoreBase byte-for-byte (BtnPanelCore.h
    //  banner) -- one confirmation that the renderer, which only ever sees
    //  the base class, doesn't care which subclass it came from.
    // =======================================================================
    {
        Canvas c(20, 20);
        NEW_HEAP(BtnPanelLaneCore, p);
        p.SetTrueColor(clBlue);
        p.SetDown(true);
        RECT rc = {0, 0, 20, 20};
        RenderBtnPanel(c.mem, rc, p);
        CHECK(c.At(10, 10) == 0x0000FFu);   // clBlue
    }

    // =======================================================================
    //  Renders into an offset sub-rectangle of a larger canvas -- confirms
    //  RenderBtnPanel respects `rc` rather than always painting from (0,0).
    // =======================================================================
    {
        Canvas c(40, 40);
        NEW_HEAP(BtnPanelCore, p);
        p.SetTrueColor(clYellow);
        p.SetDown(true);

        RECT rc = {10, 10, 30, 30};
        // pre-fill with a sentinel so "untouched outside rc" is checkable
        for (int y = 0; y < 40; ++y)
            for (int x = 0; x < 40; ++x)
                c.px[y * 40 + x] = 0x55555555u;
        RenderBtnPanel(c.mem, rc, p);

        CHECK(c.At(15, 15) == 0xFFFF00u);          // inside rc -- clYellow
        CHECK((c.px[0] & 0xFFFFFFFFu) == 0x55555555u);   // (0,0), outside rc -- untouched
        CHECK((c.px[5 * 40 + 5] & 0xFFFFFFFFu) == 0x55555555u); // still outside rc
    }

    std::printf("%d/%d checks passed\n", g_total - g_fail, g_total);
    return g_fail ? 1 : 0;
}
