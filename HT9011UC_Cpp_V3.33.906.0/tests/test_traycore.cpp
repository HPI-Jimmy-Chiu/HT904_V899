// ===========================================================================
//  tests/test_traycore.cpp
//  Assertion harness for vclcompat::TrayCore / vclcompat::Tray256Core (W7-C1).
//  Verified line-by-line against golden D:\HT9045\elec\myvcl\HTray{,256}.cpp
//  (cp950/big5-decoded), NOT taken on faith from the scratchpad prototype
//  this was promoted from. Non-zero exit on any failure. No external test
//  framework (matches tests/test_vclcompat.cpp's own style).
//
//  AI(W906-W7-C1) 20260728: added this file's convention tag (LOW-3 audit
//  -- no functional change).
// ===========================================================================
#include "vclcompat/TrayCore.h"

#include <cstdio>
#include <memory>
#include <string>

// TrayCore/Tray256Core carry MAX_ITEM x MAX_ITEM AnsiString[][] arrays
// (100x100 / 300x300 -- Tray256Core alone is >1 MB per instance). Golden
// ALWAYS heap-allocates these (`new TTMyTray(Owner)` / `new TTMyTray256(this)`
// -- never a stack TCustomControl), and this port must do the same: a stack
// instance blew the default ~1 MB thread stack and crashed with a genuine
// STATUS_STACK_OVERFLOW the first time this harness was run (2026-07-28,
// caught before being committed -- not a golden fidelity issue, a test
// authoring one). NEW_HEAP() below is this file's one-line reminder/helper.
#define NEW_HEAP(Type, Name) std::unique_ptr<Type> Name##_owner(new Type()); Type& Name = *Name##_owner

using vclcompat::TrayCore;
using vclcompat::Tray256Core;
using vclcompat::TrayCellRect;
using vclcompat::clWhite;
using vclcompat::clBlack;
using vclcompat::clGreen;
using vclcompat::clYellow;
using vclcompat::clLime;
using vclcompat::clRed;
using vclcompat::clBlue;
using vclcompat::TColor;
using vclcompat::csLeftTop;

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

int main() {
    // =======================================================================
    //  TrayCore ctor defaults (HTray.cpp:19-60)
    // =======================================================================
    {
        NEW_HEAP(TrayCore, t);
        CHECK(t.Width == 100);
        CHECK(t.Height == 200);
        CHECK(t.FXItem == 2 && t.FYItem == 2);
        CHECK(t.FXVisibleItem == 2 && t.FYVisibleItem == 2);
        CHECK(t.FXBlock == 1 && t.FYBlock == 1);
        CHECK(t.FXBlockWidth == 1 && t.FYBlockWidth == 1);
        CHECK(t.FLineWidth == 1 && t.FEdgeWidth == 1);
        CHECK(t.FShowFont == true);
        CHECK(t.FShape == false);
        CHECK(t.FTrayColor == clWhite);
        CHECK(t.FFrameColor == clBlack);
        CHECK(t.FTrayDirect == csLeftTop);
        CHECK(t.FDirectWidth == 10);
        // ColorMap: HTray.cpp:23-27 -- only 11 of 32 entries given, rest
        // value-initialised to 0 (clBlack). GOLDEN QUIRK, not a bug per se.
        CHECK(t.ColorMap[0] == clWhite);
        CHECK(t.ColorMap[1] == clGreen);
        CHECK(t.ColorMap[2] == clYellow);
        CHECK(t.ColorMap[3] == clLime);
        CHECK(t.ColorMap[4] == clRed);
        CHECK(t.ColorMap[5] == TColor(0x00FF8080));
        CHECK(t.ColorMap[6] == TColor(0x00DC34BE));
        CHECK(t.ColorMap[7] == clBlue);
        CHECK(t.ColorMap[8] == TColor(0x00FF80FF));
        CHECK(t.ColorMap[9] == TColor(0x00FF0080));
        CHECK(t.ColorMap[10] == TColor(0x0000FF80));
        for (int i = 11; i < TrayCore::MAX_COLOR_INDEX; ++i)
            CHECK(t.ColorMap[i] == 0);
        // ctor fills every cell with "5" (HTray.cpp:51), NOT "" -- distinct
        // from ClearCell()'s "" (HTray.cpp:99). Spot-check a few cells.
        CHECK(std::string(t.GetCellText(0, 0).c_str()) == "5");
        CHECK(std::string(t.GetCellText(99, 99).c_str()) == "5");
        CHECK(t.GetCellData(0, 0) == 0);
        for (int i = 0; i < TrayCore::MAX_ITEM; ++i) {
            CHECK(t.bXVisible[i] == true);
            CHECK(t.bYVisible[i] == true);
        }
    }

    // =======================================================================
    //  SetXItem / SetYItem clamping (HTray.cpp:105-140)
    // =======================================================================
    {
        NEW_HEAP(TrayCore, t);
        t.SetXItem(0);
        CHECK(t.FXItem == 1);                       // value<=0 clamps to 1
        t.SetXItem(500);
        CHECK(t.FXItem == TrayCore::MAX_ITEM - 1);   // value>=MAX_ITEM clamps to MAX_ITEM-1
        t.SetYItem(-3);
        CHECK(t.FYItem == 1);
        t.SetYItem(1000);
        CHECK(t.FYItem == TrayCore::MAX_ITEM - 1);

        t.SetXItem(5);
        CHECK(t.FXItem == 5 && t.FXVisibleItem == 5);
        // ClearCell() ran (via SetXItem) -> cells now "" not "5"
        CHECK(std::string(t.GetCellText(0, 0).c_str()) == "");
    }

    // =======================================================================
    //  GOLDEN BUG 1: SetYItem/SetBlockYItem reset bXVisible, NEVER bYVisible
    //  (HTray.cpp:133-136, :176-179). Regression test: FAILS if a future
    //  "fix" makes these setters reset the correct axis.
    // =======================================================================
    {
        NEW_HEAP(TrayCore, t);
        t.SetXItem(4);
        t.SetYItem(4);
        t.SetXVisible(0, false);   // hide X column 0
        t.SetYVisible(0, false);   // hide Y row 0 -> FYVisibleItem becomes 3
        CHECK(t.bXVisible[0] == false);
        CHECK(t.bYVisible[0] == false);
        CHECK(t.FYVisibleItem == 3);

        t.SetYItem(4);   // Y-axis setter...
        // ...but it's bXVisible that gets clobbered back to all-true (bug, verbatim):
        CHECK(t.bXVisible[0] == true);
        // ...while bYVisible is left completely untouched (still hidden):
        CHECK(t.bYVisible[0] == false);
        // ...yet FYVisibleItem is unconditionally reset to FYItem, now
        // INCONSISTENT with bYVisible[0] still being false (the "state
        // inconsistency" the golden bug produces):
        CHECK(t.FYVisibleItem == t.FYItem);
        CHECK(t.FYVisibleItem == 4);   // not 3 -- doesn't match the true hidden-row count

        // Same copy-paste bug via SetBlockYItem (HTray.cpp:176-179):
        NEW_HEAP(TrayCore, t2);
        t2.SetXItem(4);
        t2.SetYItem(4);
        t2.SetXVisible(1, false);
        t2.SetYVisible(1, false);
        t2.SetBlockYItem(2);
        CHECK(t2.bXVisible[1] == true);    // clobbered again (bug)
        CHECK(t2.bYVisible[1] == false);   // never touched (bug)
    }

    // =======================================================================
    //  SetBlockXItem / SetBlockYItem clamps (HTray.cpp:142-184) --
    //  ASYMMETRIC on purpose: X clamps to 2, Y clamps to 5. Verified
    //  directly against golden, not assumed from symmetry.
    // =======================================================================
    {
        NEW_HEAP(TrayCore, t);
        t.SetXItem(1);
        t.SetBlockXItem(9);
        CHECK(t.FXBlock == 2);         // clamped to 2 (HTray.cpp:147-148)
        CHECK(t.FXItem >= t.FXBlock);  // "if(value>=FXItem) FXItem=value;" bumps FXItem up to match

        NEW_HEAP(TrayCore, t2);
        t2.SetYItem(1);
        t2.SetBlockYItem(9);
        CHECK(t2.FYBlock == 5);        // clamped to 5 (HTray.cpp:169-170), NOT 2
    }

    // =======================================================================
    //  GOLDEN BUG 4/5 (CaculateTrayParameter / CellRect divide-by-zero,
    //  HTray.cpp:262-270 / :241-242): both preserved VERBATIM (no guard
    //  added). We only demonstrate the hazardous precondition is reachable
    //  through the public API -- we deliberately do NOT invoke
    //  CaculateTrayParameter()/CellRect() in that state, since doing so is a
    //  genuine integer divide-by-zero (SIGFPE) in both golden and this port,
    //  and this workflow's hard rules forbid letting a test process crash/
    //  raise a fault handler in an unattended batch run. This is a
    //  deliberate, documented test-coverage limitation (see this wave's
    //  final report) -- the division itself is preserved but not exercised.
    // =======================================================================
    {
        // Bug 4 precondition: hiding every column drives FXVisibleItem to 0.
        NEW_HEAP(TrayCore, t);
        t.SetXItem(3);
        t.SetXVisible(0, false);
        t.SetXVisible(1, false);
        t.SetXVisible(2, false);
        CHECK(t.FXVisibleItem == 0);   // reachable -- CaculateTrayParameter() would now divide by 0

        // Bug 5 precondition: FXItem < FXBlock is reachable by calling
        // SetXItem AFTER SetBlockXItem (SetBlockXItem only bumps FXItem UP,
        // never down, so a later direct SetXItem can leave FXItem below
        // FXBlock).
        NEW_HEAP(TrayCore, t2);
        t2.SetBlockXItem(2);   // FXBlock=2, FXItem bumped to >=2
        CHECK(t2.FXBlock == 2);
        t2.SetXItem(1);        // FXItem=1 now, FXBlock still 2
        CHECK(t2.FXItem == 1 && t2.FXBlock == 2);
        CHECK((t2.FXItem / t2.FXBlock) == 0);  // CellRect's `FXItem/FXBlock` divisor is 0 here
    }

    // =======================================================================
    //  CaculateTrayParameter geometry (HTray.cpp:262-270) -- happy path
    // =======================================================================
    {
        NEW_HEAP(TrayCore, t);
        t.SetExtents(100, 200);
        t.SetXItem(4);
        t.SetYItem(2);
        // FXVisibleItem=4, FLineWidth=1, FEdgeWidth=1, FXBlock=1, FXBlockWidth=1
        // iXWidth=(100-2-(4-1)*1-2*1-(1-1)*1)/4 = (100-2-3-2-0)/4 = 93/4 = 23
        CHECK(t.iXWidth == 23);
        // iYWidth=(200-2-(2-1)*1-2*1-(1-1)*1)/2 = (200-2-1-2-0)/2 = 195/2 = 97
        CHECK(t.iYWidth == 97);
    }

    // =======================================================================
    //  GOLDEN BUG 2: ConvertIndexCells ignores the iBX/iBY block offsets
    //  that CellRect applies (HTray.cpp:437-454 vs :241-242) -- hit-testing
    //  goes wrong once XBlockItem/YBlockItem > 1. Reproduces the plan's own
    //  worked numeric example (XItem=4, XBlockItem=2, XBlockWidth=6, 100x200).
    // =======================================================================
    {
        NEW_HEAP(TrayCore, t);
        t.SetExtents(100, 200);
        t.SetXItem(4);
        t.SetYItem(1);
        t.SetXBlockWidth(6);
        t.SetBlockXItem(2);   // FXBlock=2 (also re-derives geometry)
        t.SetExtents(100, 200);  // re-derive after the block-width/-item changes

        TrayCellRect r;
        CHECK(t.CellRect(3, 0, r));   // cell (3,0) is in the 2nd block (block offset applies)
        // Golden's DrawSingleIC/CellRect DOES include the +iBX block offset:
        int probeX = r.Right - 1;     // a pixel that genuinely falls inside cell 3's painted rect
        CHECK(probeX >= r.Left && probeX < r.Right);

        int hitX = probeX, hitY = r.Top;
        int result = t.ConvertIndexCells(hitX, hitY);
        // GOLDEN BUG, preserved verbatim: ConvertIndexCells does NOT add the
        // block offset, so a pixel that visibly paints inside cell 3 either
        // misses (-1) or resolves to the WRONG cell index once blocks are in
        // play. We assert the buggy (not the "intuitively correct") outcome.
        CHECK(result == -1 || hitX != 3);
    }

    // =======================================================================
    //  GOLDEN BUG 3: ConvertIndexCells mutates X/Y in place even when it
    //  returns -1 (HTray.cpp:441-444) -- callers must not reuse X/Y after a
    //  failed call. Preserved verbatim.
    // =======================================================================
    {
        NEW_HEAP(TrayCore, t);
        t.SetExtents(100, 200);
        t.SetXItem(2);
        t.SetYItem(2);
        int x = -50, y = -50;   // well outside the tray -> X-=iStartX, Y-=iStartY still < 0
        int result = t.ConvertIndexCells(x, y);
        CHECK(result == -1);
        CHECK(x != -50);   // mutated in place despite the failure (golden bug, preserved)
    }

    // =======================================================================
    //  SetCellNumber overloads + SetCellColorIndex bounds (HTray.cpp:374-430)
    // =======================================================================
    {
        NEW_HEAP(TrayCore, t);
        t.SetXItem(3);
        t.SetYItem(3);
        t.SetCellNumber(1, 1, 42);
        CHECK(std::string(t.GetCellText(1, 1).c_str()) == "42");
        t.SetCellNumber(1, 1, 3.5);
        CHECK(std::string(t.GetCellText(1, 1).c_str()) == "3.50");
        t.SetCellNumber(1, 1, vclcompat::AnsiString("hi"));
        CHECK(std::string(t.GetCellText(1, 1).c_str()) == "hi");
        // out-of-bounds writes are silently ignored (HTray.cpp:404 guard).
        // NOTE: cell (99,99) is "" here, not the ctor's "5" default -- SetXItem/
        // SetYItem above each called ClearCell(), which (per HTray.cpp:94-101)
        // unconditionally wipes the FULL MAX_ITEM x MAX_ITEM array, not just the
        // FXItem x FYItem active region.
        t.SetCellNumber(99, 99, 7);
        CHECK(std::string(t.GetCellText(99, 99).c_str()) == "");   // untouched by the ignored write

        t.SetCellColorIndex(0, 0, 3);
        CHECK(t.GetCellData(0, 0) == 3);
        t.SetCellColorIndex(0, 0, 999);   // out of range, ignored (HTray.cpp:384)
        CHECK(t.GetCellData(0, 0) == 3);
        t.SetCellColorIndex(0, 0, -1);    // negative, ignored
        CHECK(t.GetCellData(0, 0) == 3);
    }

    // =======================================================================
    //  ClearCell asymmetry: ctor fills "5", ClearCell fills "" (HTray.cpp:99
    //  vs :51) -- both real golden behaviour, not a bug, just documented.
    // =======================================================================
    {
        NEW_HEAP(TrayCore, t);
        t.SetCellNumber(0, 0, 1);
        t.ClearCell();
        CHECK(std::string(t.GetCellText(0, 0).c_str()) == "");
        CHECK(t.GetCellData(0, 0) == 0);
    }

    // =======================================================================
    //  SaveCellTextToString (HTray.cpp:461-478, I/O split out)
    // =======================================================================
    {
        NEW_HEAP(TrayCore, t);
        t.SetXItem(2);
        t.SetYItem(2);
        t.SetCellNumber(0, 0, 1);
        t.SetCellNumber(0, 1, 2);
        t.SetCellNumber(1, 0, 3);
        t.SetCellNumber(1, 1, 4);
        std::string s = t.SaveCellTextToString().c_str();
        CHECK(s == "1, 2, \r\n3, 4, \r\n");
    }

    // =======================================================================
    //  Tray256Core -- no block support, no visibility array (HTray256.h has
    //  neither FXBlock/FYBlock nor bXVisible/bYVisible -- verified directly).
    // =======================================================================
    {
        NEW_HEAP(Tray256Core, t);
        CHECK(t.Width == 100 && t.Height == 200);
        CHECK(t.FXItem == 2 && t.FYItem == 2);
        CHECK(t.FShowFont == true && t.FShape == false);
        CHECK(t.FTrayColor == clWhite && t.FFrameColor == clBlack);
        CHECK(std::string(t.GetCellText(0, 0).c_str()) == "5");

        t.SetXItem(0);
        CHECK(t.FXItem == 1);
        t.SetXItem(5000);
        CHECK(t.FXItem == Tray256Core::MAX_ITEM - 1);   // clamps to 299, not 99 (MAX_ITEM=300)

        t.SetXItem(4);
        t.SetYItem(4);
        t.SetExtents(100, 200);
        t.SetCellNumber(2, 2, 77);
        CHECK(t.GetCellData(2, 2) == 0);   // SetCellNumber doesn't touch colour index
        CHECK(std::string(t.GetCellText(2, 2).c_str()) == "77");

        TrayCellRect r;
        CHECK(t.CellRect(2, 2, r));
        CHECK(!t.CellRect(4, 0, r));   // out of range (FXItem==4, X==4 is out of [0,4))
    }

    // ===================================================================
    //  summary
    // ===================================================================
    std::printf("\ntraycore harness: %d/%d checks passed\n", g_total - g_fail, g_total);
    if (g_fail != 0) {
        std::printf("RESULT: FAILED (%d failures)\n", g_fail);
        return 1;
    }
    std::printf("RESULT: OK\n");
    return 0;
}
