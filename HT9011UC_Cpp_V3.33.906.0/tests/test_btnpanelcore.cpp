// ===========================================================================
//  tests/test_btnpanelcore.cpp
//  Assertion harness for vclcompat::BtnPanelCore / BtnPanelLaneCore (W7-C1).
//  Verified line-by-line against golden butPa1.cpp / BtnPanelLane.cpp
//  (D:\HT9045\elec\myvcl\, cp950/big5-decoded). Non-zero exit on any
//  failure. No external test framework.
//
//  AI(W906-W7-C1) 20260728: added this file's convention tag (LOW-3 audit
//  -- no functional change).
// ===========================================================================
#include "vclcompat/BtnPanelCore.h"

#include <cstdio>
#include <string>

using vclcompat::BtnPanelCore;
using vclcompat::BtnPanelLaneCore;
using vclcompat::TColor;
using vclcompat::TTabStyleShim;
using vclcompat::tsTabs;
using vclcompat::tsButtons;
using vclcompat::tsFlatButtons;
using vclcompat::TBevelShim;
using vclcompat::bvNone;
using vclcompat::bvLowered;
using vclcompat::bvRaised;
using vclcompat::clBtnFace;
using vclcompat::AnsiString;

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
    //  BtnPanelCore ctor (butPa1.cpp:18-51)
    // =======================================================================
    {
        BtnPanelCore p;
        CHECK(p.GetDown() == false);
        CHECK(p.GetStyle() == tsButtons);
        // bDown starts false in the ctor -> the `else` branch runs -> Color
        // reads the LOCAL shadow variable (== clBtnFace), a real, deterministic
        // ctor-time value (NOT the uninitialised-member bug below):
        CHECK(p.Color == clBtnFace);
        // FStyle==tsButtons (!=tsFlatButtons) -> bvRaised/bvRaised:
        CHECK(p.BevelInner == bvRaised);
        CHECK(p.BevelOuter == bvRaised);
        CHECK(p.OutPort == 0 && p.OutBit == 0 && p.OutType == 0);
    }

    // =======================================================================
    //  GOLDEN BUG 6 (butPa1.cpp:21-22 / BtnPanelLane.cpp:23-24): the ctor's
    //  `TColor tcTrueColor=clBtnFace; TColor tcFalseColor=clBtnFace;` are
    //  LOCAL variables shadowing the protected members of the same name --
    //  the real members (tcTrueColor/tcFalseColor/tcTrueFontColor/
    //  tcFalseFontColor) are NEVER assigned by either golden ctor.
    //  Regression: FAILS if a future "fix" removes the shadowing (making the
    //  assignment actually reach the member) or adds a sensible default.
    // =======================================================================
    {
        BtnPanelCore p;
        CHECK(p.GetTrueColor() != clBtnFace);        // NOT the seemingly-obvious default
        CHECK(p.GetFalseColor() != clBtnFace);        // ditto
        // Practical consequence: toggling Down to true BEFORE ever calling
        // SetTrueColor/SetTrueFontColor makes the panel adopt the
        // never-initialised sentinel as its real Color/FontColor -- this is
        // the actual observable bug, not just an academic member-shadowing
        // curiosity:
        p.SetDown(true);
        CHECK(p.Color == p.GetTrueColor());           // == the never-set sentinel value
        CHECK(p.Color != clBtnFace);

        BtnPanelLaneCore lane;
        CHECK(lane.GetTrueColor() != clBtnFace);
        CHECK(lane.GetFalseColor() != clBtnFace);
        lane.SetDown(true);
        CHECK(lane.Color == lane.GetTrueColor());
    }

    // =======================================================================
    //  SetDown / SetPanelStatus (butPa1.cpp:81-106, BtnPanelLane.cpp:98-123
    //  -- identical bodies, verified by direct comparison)
    // =======================================================================
    {
        BtnPanelCore p;
        p.SetTrueColor(0x00112233);
        p.SetFalseColor(0x00445566);
        p.SetTrueFontColor(0x00AABBCC);
        p.SetFalseFontColor(0x00DDEEFF);

        p.SetDown(true);
        CHECK(p.GetDown() == true);
        CHECK(p.Color == 0x00112233);
        CHECK(p.FontColor == 0x00AABBCC);
        CHECK(p.BevelInner == bvLowered);
        CHECK(p.BevelOuter == bvLowered);

        p.SetDown(false);
        CHECK(p.GetDown() == false);
        CHECK(p.Color == 0x00445566);
        CHECK(p.FontColor == 0x00DDEEFF);
        CHECK(p.BevelInner == bvRaised);    // tsButtons -> raised, not flat
        CHECK(p.BevelOuter == bvRaised);
    }

    // =======================================================================
    //  SetTrueColor/SetFalseColor/SetTrueFontColor/SetFalseFontColor only
    //  touch the LIVE side's Color/FontColor (butPa1.cpp:53-79)
    // =======================================================================
    {
        BtnPanelCore p;
        p.SetDown(false);
        p.SetTrueColor(0x00010101);     // Down==false -> Color must NOT change
        CHECK(p.Color != 0x00010101);
        p.SetFalseColor(0x00020202);    // Down==false -> Color DOES change
        CHECK(p.Color == 0x00020202);

        p.SetDown(true);
        p.SetFalseColor(0x00030303);    // Down==true -> Color must NOT change
        CHECK(p.Color != 0x00030303);
        p.SetTrueColor(0x00040404);     // Down==true -> Color DOES change
        CHECK(p.Color == 0x00040404);
    }

    // =======================================================================
    //  SetStyle / WriteStyle (butPa1.cpp:202-206) -- re-applies bevel under
    //  the CURRENT Down state
    // =======================================================================
    {
        BtnPanelCore p;
        CHECK(p.GetStyle() == tsButtons);
        p.SetDown(false);
        p.SetStyle(tsFlatButtons);
        CHECK(p.BevelInner == bvNone);
        CHECK(p.BevelOuter == bvNone);
        p.SetStyle(tsButtons);
        CHECK(p.BevelInner == bvRaised);
        CHECK(p.BevelOuter == bvRaised);

        p.SetDown(true);
        p.SetStyle(tsFlatButtons);
        CHECK(p.BevelInner == bvLowered);   // Down==true wins over the style's bvNone
        CHECK(p.BevelOuter == bvLowered);
    }

    // =======================================================================
    //  BtnPanelCore::SetPort -- hex, tail-anchored (butPa1.cpp WritePort
    //  :156-180 -- identical algorithm to MyLedCore::SetPort, see LedCore
    //  harness)
    // =======================================================================
    {
        BtnPanelCore p;
        p.SetPort(AnsiString("1F"));
        CHECK(p.OutPort == 0x1F);
        p.SetPort(AnsiString(" 1f "));
        CHECK(p.OutPort == 0x1F);
        p.SetPort(AnsiString("XY12"));
        CHECK(p.OutPort == 0x12);

        p.SetBit(AnsiString("7"));
        CHECK(p.OutBit == 7);
        p.SetType(AnsiString("2"));
        CHECK(p.OutType == 2);
        p.SetAlias(AnsiString("OutSuck"));
        CHECK(std::string(p.GetAlias().c_str()) == "OutSuck");
    }

    // =======================================================================
    //  BtnPanelLaneCore -- Ring/IP/Port/Bit/Type/ISA are ALL plain atoi, NO
    //  fixed buffer (BtnPanelLane.cpp:132-166) -- the DIRECT CONTRAST with
    //  MyLedLaneCore's truncating buffer-overflow bug. Regression: FAILS if
    //  a future "fix" makes these match the (buggy) LED-lane truncation
    //  instead of staying full-value-safe.
    // =======================================================================
    {
        BtnPanelLaneCore lane;
        CHECK(lane.GetDown() == false);
        CHECK(lane.GetStyle() == tsButtons);
        CHECK(lane.OutRing == 0 && lane.OutIP == 0 && lane.OutPort == 0);
        CHECK(lane.OutBit == 0 && lane.OutType == 0 && lane.ISABase == 0);

        lane.SetRing(AnsiString("12"));
        CHECK(lane.OutRing == 12);      // full value -- NOT truncated to "1"
        lane.SetIP(AnsiString("192"));
        CHECK(lane.OutIP == 192);       // full value -- NOT truncated to "19"
        lane.SetPort(AnsiString("99"));
        CHECK(lane.OutPort == 99);      // full value -- NOT truncated to "9"
        lane.SetBit(AnsiString("42"));
        CHECK(lane.OutBit == 42);       // full value -- NOT truncated to "4"
        lane.SetType(AnsiString("123"));
        CHECK(lane.OutType == 123);
        lane.SetISA(AnsiString("55"));
        CHECK(lane.ISABase == 55);
        lane.SetAlias(AnsiString("LaneBtn"));
        CHECK(std::string(lane.GetAlias().c_str()) == "LaneBtn");

        // Same Down/Style/colour state machine as BtnPanelCore (shared base):
        lane.SetTrueColor(0x00778899);
        lane.SetDown(true);
        CHECK(lane.Color == 0x00778899);
        CHECK(lane.BevelInner == bvLowered);
    }

    // ===================================================================
    //  summary
    // ===================================================================
    std::printf("\nbtnpanelcore harness: %d/%d checks passed\n", g_total - g_fail, g_total);
    if (g_fail != 0) {
        std::printf("RESULT: FAILED (%d failures)\n", g_fail);
        return 1;
    }
    std::printf("RESULT: OK\n");
    return 0;
}
