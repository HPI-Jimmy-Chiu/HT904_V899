// ===========================================================================
//  tests/test_trayrender.cpp
//  Headless HDC pixel-test harness for vclcompat::render::RenderTray/
//  RenderTray256 (W7-C2). Renders into a memory DC backed by a
//  CreateDIBSection (never a visible window -- GetDC(NULL) only queries the
//  desktop DC's pixel format as a CreateCompatibleDC() reference, it does
//  NOT create or show anything; see PROHIBITION 3 in this wave's brief).
//  Asserts a full-buffer FNV-1a hash (catches ANY regression) PLUS targeted
//  per-pixel spot checks derived from hand-worked geometry (so a maintainer
//  can see WHY a value is expected, not just that some baked hash matches).
//  Non-zero exit on any failure. No external test framework (matches
//  test_traycore.cpp's own style, W7-C1).
//
//  AI(W906-W7-C2) 20260728: added this file's convention tag (LOW-3 audit
//  -- no functional change).
// ===========================================================================
#include <windows.h>
#include <cstdio>
#include <memory>

#include "vclcompat/TrayCore.h"
#include "vclcompat/render/TrayRender.h"

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

// Minimal headless DIB-section canvas -- CreateCompatibleDC(NULL-derived
// screen DC) + CreateDIBSection, exactly the technique demonstrated by the
// design run's gdiprobe*.cpp sketches (session scratchpad), rebuilt here as
// production test infrastructure rather than trusted as-is.
struct Canvas {
    HDC screen;
    HDC mem;
    HBITMAP dib;
    HGDIOBJ oldBmp;
    unsigned* px;
    int w, h;

    Canvas(int width, int height) : w(width), h(height) {
        screen = ::GetDC(NULL);              // reference DC only -- no window created/shown
        mem = ::CreateCompatibleDC(screen);
        BITMAPINFO bi;
        ZeroMemory(&bi, sizeof(bi));
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = w;
        bi.bmiHeader.biHeight = -h;          // top-down, so px[row*w+col] matches natural order
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
    void Clear(unsigned char byteValue) { memset(px, byteValue, (size_t)w * h * 4); }
};

unsigned Fnv1a(const unsigned* px, int n) {
    unsigned h = 2166136261u;
    for (int i = 0; i < n; ++i) { h ^= px[i]; h *= 16777619u; }
    return h;
}

// A 32bpp BI_RGB DIB pixel, read back as a little-endian DWORD and masked to
// 24 bits, equals the plain 0xRRGGBB hex you'd expect from the colour's
// everyday name (verified empirically against known TColor constants before
// writing this file -- clLime => 0x00FF00, clRed => 0xFF0000, clBlue =>
// 0x0000FF, clYellow => 0xFFFF00, clWhite => 0xFFFFFF, clBlack => 0x000000;
// this is NOT the same byte order as the TColor/COLORREF value used to
// paint it, since TColor/COLORREF is 0x00BBGGRR -- R and B swap when you
// read a rendered pixel back out of a 32bpp DIB vs. the TColor that painted
// it, for any colour where R != B).
const unsigned kGreen  = 0x00FF00u;
const unsigned kRed    = 0xFF0000u;
const unsigned kBlue   = 0x0000FFu;
const unsigned kYellow = 0xFFFF00u;
const unsigned kWhite  = 0xFFFFFFu;
const unsigned kBlack  = 0x000000u;

} // namespace

int main() {
    // PROHIBITION 3: no probe here may raise a window/dialog/message box.
    ::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

    // =======================================================================
    //  TrayCore, 2x2, rectangle shape, no font -- full hash + per-cell colour
    //  =======================================================================
    //  Layout worked by hand from TrayCore::CaculateTrayParameter (verified
    //  against the running renderer before being hardcoded here, not the
    //  other way around): Width=Height=40, FXItem=FYItem=2, FLineWidth=
    //  FEdgeWidth=1 (ctor defaults) => iXWidth=iYWidth=17, iStartX=iStartY=2,
    //  iPitchX=iPitchY=18. Cell rects: (0,0)=(2,2,19,19), (1,0)=(20,2,37,19),
    //  (0,1)=(2,20,19,37), (1,1)=(20,20,37,37).
    {
        Canvas c(40, 40);
        NEW_HEAP(TrayCore, t);
        t.SetExtents(40, 40);
        t.SetXItem(2);
        t.SetYItem(2);
        t.SetShowFont(false);          // keep this case font-free/deterministic (see FShowFont test below)
        t.SetCellColorIndex(0, 0, 3);  // ColorMap[3] == clLime
        t.SetCellColorIndex(1, 0, 4);  // ColorMap[4] == clRed
        t.SetCellColorIndex(0, 1, 7);  // ColorMap[7] == clBlue
        t.SetCellColorIndex(1, 1, 2);  // ColorMap[2] == clYellow

        RenderTray(c.mem, t);

        // Cell interiors -- ColorMap fill.
        CHECK(c.At(10, 10) == kGreen);   // cell(0,0)
        CHECK(c.At(30, 10) == kRed);     // cell(1,0)
        CHECK(c.At(10, 30) == kBlue);    // cell(0,1)
        CHECK(c.At(30, 30) == kYellow);  // cell(1,1)

        // Cell borders (FFrameColor, per DrawSingleIC/GdiRectangle) and the
        // outer whole-tray border/gap -- all clBlack/clWhite respectively.
        CHECK(c.At(0, 0) == kBlack);      // outer border, top-left corner
        CHECK(c.At(20, 20) == kBlack);    // cell(1,1)'s own top-left border pixel
        CHECK(c.At(19, 19) == kWhite);    // 1px inter-cell gap (background), NOT a cell

        // Background well outside any cell (still inside the outer frame).
        CHECK(c.At(1, 20) == kWhite);
        CHECK(c.At(20, 1) == kWhite);

        // Full-buffer hash -- catches any pixel-level regression anywhere,
        // not just at the hand-picked spots above. Captured from THIS exact
        // renderer/scenario (see this wave's report for the capture method:
        // a throwaway probe executable, run once, hash read off stdout).
        CHECK(Fnv1a(c.px, 40 * 40) == 0x7528a6b3u);
    }

    // =======================================================================
    //  Tray256Core, 1x1, ELLIPSE shape -- exercises FShape==true's GdiEllipse
    //  path (RenderTrayImpl's `if (core.FShape) GdiEllipse(...) else
    //  GdiRectangle(...)`, HTray256.cpp:153-156).
    // =======================================================================
    {
        Canvas c(50, 50);
        NEW_HEAP(Tray256Core, t);
        t.SetExtents(50, 50);
        t.SetXItem(1);
        t.SetYItem(1);
        t.SetShape(true);
        t.SetShowFont(false);
        t.SetCellColorIndex(0, 0, 3);  // clLime

        c.Clear(0xAA);   // distinguishable "untouched" sentinel
        RenderTray256(c.mem, t);

        TrayCellRect r;
        CHECK(t.CellRect(0, 0, r));
        CHECK(r.Left == 2 && r.Top == 2 && r.Right == 48 && r.Bottom == 48);

        CHECK(c.At(25, 25) == kGreen);    // ellipse centre -- clLime fill
        // Bounding-box corner: a true ellipse never reaches its bounding
        // box's corners (only the 4 edge midpoints) -- but (2,2) is NOT
        // "untouched background" either, because the default FTrayDirect==
        // csLeftTop corner-tick loop (FDirectWidth==10 default) draws a
        // diagonal line `x+y==i` for each i in 0..9, and 2+2==4 lies exactly
        // on that family of lines -- so this pixel is FFrameColor (black),
        // not the 0xAA sentinel and not the ellipse fill.
        CHECK(c.At(2, 2) == kBlack);
    }

    // =======================================================================
    //  Corner "direction" tick geometry -- csRightBottom, DirectWidth=5.
    //  Golden HTray.cpp:327-340: sx=Width,sy=Height,x=sx,y=sy; for i in
    //  0..DirectWidth-1: line (sx,y)-(x,sy), then x--,y--. That traces
    //  points satisfying (Width-x)+(Height-y)==i for i in 0..4 near the
    //  bottom-right corner -- i.e. pixels strictly further from the corner
    //  than DirectWidth-1 are NEVER touched by a tick, only by the outer
    //  border/background.
    // =======================================================================
    {
        Canvas c(50, 50);
        NEW_HEAP(TrayCore, t);
        t.SetExtents(50, 50);
        t.SetXItem(1);
        t.SetYItem(1);
        t.SetShowFont(false);
        t.SetTrayDirect(csRightBottom);
        t.SetDirectWidth(5);
        t.SetCellColorIndex(0, 0, 0);   // ColorMap[0] == clWhite, matches background (isolates ticks)

        c.Clear(0x55);
        RenderTray(c.mem, t);

        CHECK(c.At(49, 49) == kBlack);   // outer border (perimeter pixel)
        CHECK(c.At(45, 49) == kBlack);   // outer border row
        CHECK(c.At(49, 45) == kBlack);   // outer border column
        // (44,44): (50-44)+(50-44)=12, outside the reachable i in [0,4]
        // band for DirectWidth=5 -- must be plain background, not a tick.
        CHECK(c.At(44, 44) == kWhite);

        CHECK(Fnv1a(c.px, 50 * 50) == 0x72674a4eu);
    }

    // =======================================================================
    //  FShowFont==true -- intentionally coarse assertion only. Golden's
    //  DrawText uses whatever font/text-colour is ambient on the Canvas
    //  (HTray.cpp:279 `Canvas->Font=Font` reads the control's own VCL Font
    //  property, which TrayCore carries no field for at all -- out of
    //  scope, see TrayRender.h). Font/glyph rasterisation can legitimately
    //  differ across Windows versions/ClearType settings in ways a hash or
    //  a specific-pixel assert would flakily depend on, so this test only
    //  proves DrawText painted SOMETHING inside the cell (a pixel differing
    //  from the pure fill colour), not its exact shape.
    // =======================================================================
    {
        Canvas c(40, 40);
        NEW_HEAP(TrayCore, t);
        t.SetExtents(40, 40);
        t.SetXItem(1);
        t.SetYItem(1);
        t.SetShowFont(true);
        t.SetCellColorIndex(0, 0, 3);   // clLime
        t.SetCellNumber(0, 0, "8");

        // Caller-supplied ambient font/colour, per TrayRender.h's contract.
        ::SelectObject(c.mem, ::GetStockObject(SYSTEM_FONT));
        ::SetTextColor(c.mem, RGB(0, 0, 0));

        RenderTray(c.mem, t);

        bool sawNonFill = false;
        TrayCellRect r;
        CHECK(t.CellRect(0, 0, r));
        for (int y = r.Top; y < r.Bottom && !sawNonFill; ++y)
            for (int x = r.Left; x < r.Right; ++x)
                if (c.At(x, y) != kGreen) { sawNonFill = true; break; }
        CHECK(sawNonFill);
    }

    std::printf("%d/%d checks passed\n", g_total - g_fail, g_total);
    return g_fail ? 1 : 0;
}
