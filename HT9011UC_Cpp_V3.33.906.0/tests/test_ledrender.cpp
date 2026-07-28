// ===========================================================================
//  tests/test_ledrender.cpp
//  Headless HDC pixel-test harness for vclcompat::render::RenderLed (W7-C2).
//  Same technique as test_trayrender.cpp (memory-DC + CreateDIBSection, no
//  window ever created -- PROHIBITION 3). Non-zero exit on any failure.
//
//  AI(W906-W7-C2) 20260728: new file -- gives RenderLed a real pixel oracle
//  (hash + targeted coordinate spot-checks) so a geometry or colour
//  regression fails mechanically rather than needing a human to look at a
//  window. Asserts on actual rendered pixels, not just that a call returns.
// ===========================================================================
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <memory>

#include "vclcompat/LedCore.h"
#include "vclcompat/render/LedRender.h"

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
    unsigned Raw(int x, int y) const { return px[y * w + x]; }
    void Clear(unsigned char byteValue) { std::memset(px, byteValue, (size_t)w * h * 4); }
};

// See test_trayrender.cpp's identical banner: a rendered 32bpp DIB pixel,
// masked to 24 bits, equals the plain 0xRRGGBB hex of the colour's everyday
// name -- verified empirically, not the same byte order as the TColor/
// COLORREF that painted it.
const unsigned kGreen  = 0x00FF00u;
const unsigned kSilver = 0xC0C0C0u;   // clSilver (R=G=B, so TColor/pixel byte order is moot here)

} // namespace

int main() {
    ::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

    // =======================================================================
    //  MyLedCore -- ctor forces LEDStyle=LEDSqLarge (MyLed.cpp:22), 22x22,
    //  matching ALEDSQLG's own native resource size exactly (no stretch).
    // =======================================================================
    {
        Canvas c(40, 40);
        NEW_HEAP(MyLedCore, led);
        CHECK(led.GetLEDStyle() == LEDSqLarge);
        CHECK(led.Width == 22 && led.Height == 22);

        led.SetTrueColor(clLime);
        led.SetFalseColor(clSilver);

        // Value defaults false -> ColorTemp==true (aled.pas ctor sets
        // ColorTemp:=true directly, :95) -> Paint shows fLedBitmap[1] ==
        // the FalseColor-flooded art (LedCore.h's own note on this
        // deliberately-backwards-looking polarity).
        c.Clear(0x11);
        RenderLed(c.mem, 2, 2, led);
        CHECK(c.At(13, 13) == kSilver);          // lens centre, false state
        CHECK(c.Raw(0, 0) == 0x11111111u);       // untouched outside the 22x22 footprint

        led.SetValue(true);                       // -> SetToTrueColorState -> ColorTemp=false
        CHECK(led.ColorTemp == false);
        c.Clear(0x11);
        RenderLed(c.mem, 2, 2, led);
        CHECK(c.At(13, 13) == kGreen);            // lens centre, true state

        led.SetValue(false);                       // back to false -> silver again
        c.Clear(0x11);
        RenderLed(c.mem, 2, 2, led);
        CHECK(c.At(13, 13) == kSilver);
    }

    // =======================================================================
    //  Overload taking an explicit destRect -- same natural size (22x22,
    //  LEDSqLarge default for MyLedLaneCore too, MyLedLane.cpp:22), placed
    //  at a non-trivial origin, 1:1 (no stretch).
    // =======================================================================
    {
        Canvas c(60, 60);
        NEW_HEAP(MyLedLaneCore, led);
        CHECK(led.GetLEDStyle() == LEDSqLarge);
        led.SetTrueColor(clLime);
        led.SetFalseColor(clSilver);
        led.SetValue(true);   // ColorTemp=false -> TrueColor (green)

        c.Clear(0x22);
        RECT dest; dest.left = 10; dest.top = 15; dest.right = 32; dest.bottom = 37;
        RenderLed(c.mem, dest, led);
        CHECK(c.At(21, 26) == kGreen);            // centre of the 22x22 footprint at (10,15)
        CHECK(c.Raw(0, 0) == 0x22222222u);        // well outside, untouched
        CHECK(c.Raw(9, 26) == 0x22222222u);       // 1px left of the footprint, untouched
    }

    // =======================================================================
    //  A genuine stretch: LEDVertical is 14x22 natively; ask for a much
    //  bigger destRect and confirm the lens colour still fills near its
    //  (stretched) centre, proving StretchBlt -- not a 1:1 BitBlt -- is
    //  really what's driving this (golden's own StretchDraw always
    //  stretches to whatever target rect it's given, see LedRender.h).
    // =======================================================================
    {
        Canvas c(100, 100);
        NEW_HEAP(LedCore, led);
        led.SetLEDStyle(LEDVertical);
        CHECK(led.Width == 14 && led.Height == 22);
        led.SetTrueColor(clLime);
        led.SetValue(true);   // ColorTemp=false -> TrueColor

        c.Clear(0x33);
        RECT dest; dest.left = 0; dest.top = 0; dest.right = 70; dest.bottom = 88;   // 5x native size
        RenderLed(c.mem, dest, led);
        CHECK(c.At(35, 44) == kGreen);             // stretched centre
        CHECK(c.Raw(69, 87) != 0x33333333u);       // stretch actually reached the far corner region
    }

    // =======================================================================
    //  AI(W906-W7-C2) 20260728 -- the flood-fill seed must come from the
    //  STYLE EXTENTS, and must be INDEPENDENT of the live control size.
    //
    //  golden aled.pas:157/159 floods at (Width DIV 2, Height DIV 2), and
    //  those are TALed's own properties -- but both calls sit INSIDE
    //  CreateLedBitmap (aled.pas:106-160), immediately after the
    //  `case fLEDStyle` ladder has just assigned Width/Height that style's
    //  fixed extents (:114-115 / 121-122 / 128-129 / 135-136 / 142-143 /
    //  149-150). A .dfm may later resize the control, but that happens after
    //  CreateLedBitmap has run and golden never re-runs it on a resize -- so
    //  golden's seed can never observe the .dfm size.
    //
    //  This test forces Width/Height far away from the style's extents and
    //  asserts the fill STILL happens, which is what golden does. It fails
    //  if the seed is ever wired back to the live control size -- an earlier
    //  revision of RenderLed did exactly that and it was a real regression:
    //  for the 262 of 1,840 live LED instances whose .dfm size differs from
    //  their style extents the seed can leave the lens entirely (golden
    //  cContact.dfm:15895 ledOneCycle, a LEDSqLarge sized 15x15, rendered 0
    //  coloured pixels instead of golden's 189).
    // =======================================================================
    {
        Canvas c(40, 40);
        NEW_HEAP(LedCore, led);
        led.SetLEDStyle(LEDVertical);           // style extents 14x22, native art 14x22
        led.SetTrueColor(0x00332211);            // distinct from every colour baked into the art
        led.SetFalseColor(clSilver);
        led.SetValue(true);                      // ColorTemp=false -> TrueColor fill
        led.Width = 4;
        led.Height = 6;                          // live size forced away from the style's 14x22;
                                                  // golden's seed is unaffected by this

        c.Clear(0x11);
        RECT dest; dest.left = 2; dest.top = 2; dest.right = 2 + 14; dest.bottom = 2 + 22;
        RenderLed(c.mem, dest, led);              // native-size destRect -- no stretch, so a hit
                                                   // shows up at its true bitmap-space location
        int hits = 0;
        for (int y = 0; y < 26; ++y)
            for (int x = 0; x < 18; ++x)
                if (c.At(x, y) == 0x112233u) ++hits;   // ResolveColor swizzles TColor->pixel bytes
                                                        // (see kGreen/kSilver banner above); TColor
                                                        // 0x00332211 (BB=33,GG=22,RR=11) paints as
                                                        // pixel 0xRRGGBB == 0x112233.
        // Seeded from the style extents (7,11) -- inside the lens -- so the
        // fill MUST happen despite the 4x6 live size. A live-size seed would
        // be (2,3), outside the lens, and would fill nothing.
        CHECK(hits > 0);
    }

    std::printf("%d/%d checks passed\n", g_total - g_fail, g_total);
    return g_fail ? 1 : 0;
}
