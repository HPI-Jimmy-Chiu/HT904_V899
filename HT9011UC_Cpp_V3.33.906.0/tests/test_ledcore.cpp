// ===========================================================================
//  tests/test_ledcore.cpp
//  Assertion harness for vclcompat::LedCore / MyLedCore / MyLedLaneCore
//  (W7-C1). Verified line-by-line against golden aled.pas (elec/Component/)
//  and MyLed{,Lane}.cpp (elec/myvcl/), both under D:\HT9045, cp950/big5-decoded.
//  Non-zero exit on any failure. No external test framework.
//
//  AI(W906-W7-C1) 20260728: added the MEDIUM-1 regression block below
//  (SetTrueColor/SetFalseColor must snap Width/Height back to style
//  extents, matching golden's CreateLedBitmap side effect) and this file's
//  convention tag.
// ===========================================================================
#include "vclcompat/LedCore.h"

#include <cstdio>
#include <string>

using vclcompat::LedCore;
using vclcompat::MyLedCore;
using vclcompat::MyLedLaneCore;
using vclcompat::TLEDStyle;
using vclcompat::LEDSmall;
using vclcompat::LEDLarge;
using vclcompat::LEDSqSmall;
using vclcompat::LEDSqLarge;
using vclcompat::LEDVertical;
using vclcompat::LEDHorizontal;
using vclcompat::clLime;
using vclcompat::clSilver;
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
    //  LedCore ctor defaults (aled.pas Create :81-97)
    // =======================================================================
    {
        LedCore led;
        CHECK(led.GetTrueColor() == clLime);
        CHECK(led.GetFalseColor() == clSilver);
        CHECK(led.GetBlink() == false);
        CHECK(led.GetValue() == false);
        CHECK(led.GetLEDStyle() == LEDSmall);
        CHECK(led.GetInterval() == 1000);
        CHECK(led.IsTimerActive() == false);
        // ColorTemp:=true set BEFORE CreateLedBitmap (aled.pas:95) -- LED
        // starts showing the FALSE-colour art (off state), matching Value==false.
        CHECK(led.ColorTemp == true);
        // CreateLedBitmap's Width/Height half for the default style LEDSmall
        // (aled.pas:112-118): 22x22, NOT the transient 16x16 set earlier in
        // the ctor (aled.pas:84-85) and immediately overwritten.
        CHECK(led.Width == 22 && led.Height == 22);
    }

    // =======================================================================
    //  ApplyStyleExtents -- Width/Height per style (aled.pas CreateLedBitmap
    //  :106-153, Width/Height half only)
    // =======================================================================
    {
        LedCore led;
        led.SetLEDStyle(LEDLarge);
        CHECK(led.Width == 32 && led.Height == 32);
        led.SetLEDStyle(LEDSqSmall);
        CHECK(led.Width == 22 && led.Height == 22);
        led.SetLEDStyle(LEDSqLarge);
        CHECK(led.Width == 22 && led.Height == 22);
        led.SetLEDStyle(LEDHorizontal);
        CHECK(led.Width == 22 && led.Height == 14);
        led.SetLEDStyle(LEDVertical);
        CHECK(led.Width == 14 && led.Height == 22);
        led.SetLEDStyle(LEDSmall);
        CHECK(led.Width == 22 && led.Height == 22);
    }

    // =======================================================================
    //  SetValue -- ChangeValue (aled.pas:223-237)
    // =======================================================================
    {
        LedCore led;
        led.SetValue(true);
        CHECK(led.GetValue() == true);
        CHECK(led.ColorTemp == false);       // SetToTrueColorState: shows TRUE-colour art
        CHECK(led.IsTimerActive() == false); // Blink is false -> no timer started

        led.SetValue(false);
        CHECK(led.GetValue() == false);
        CHECK(led.ColorTemp == true);        // SetToFalseColorState
        CHECK(led.IsTimerActive() == false); // StopTimer (was already inactive, no-op)

        // Setting the SAME value again is a documented no-op (aled.pas's own
        // `if (fValue <> V)` guard) -- verify by forcing ColorTemp away from
        // the "just-set-false" state and confirming SetValue(false) again
        // does not touch it.
        led.OnTimerTick();          // flips ColorTemp to false, simulating a stray tick
        CHECK(led.ColorTemp == false);
        led.SetValue(false);        // fValue is already false -> guard should skip everything
        CHECK(led.ColorTemp == false);  // unchanged (would be true if the guard were removed)
    }

    // =======================================================================
    //  SetBlink + timer state machine (aled.pas ChangeBlink :239-252,
    //  SetLedTimer :194-202, ResetLedTimer :204-210)
    // =======================================================================
    {
        LedCore led;
        led.SetValue(true);
        led.SetBlink(true);
        CHECK(led.GetBlink() == true);
        CHECK(led.IsTimerActive() == true);   // Value==true && Blink==true -> StartTimer
        CHECK(led.ColorTemp == false);        // SetLedTimer forces ColorTemp:=false (aled.pas:198)

        led.OnTimerTick();
        CHECK(led.ColorTemp == true);         // OnLedTimer toggles (aled.pas:177)
        led.OnTimerTick();
        CHECK(led.ColorTemp == false);

        led.SetValue(false);                  // turning off stops the timer even mid-blink
        CHECK(led.IsTimerActive() == false);
        CHECK(led.GetBlink() == true);         // SetValue never touches fBlink itself
        CHECK(led.ColorTemp == true);          // SetToFalseColorState

        led.SetValue(true);                    // turning back on with Blink still true restarts it
        CHECK(led.IsTimerActive() == true);
        CHECK(led.ColorTemp == false);
    }

    // =======================================================================
    //  SetBlink(false) while Value==true resets ColorTemp to the solid state
    //  and stops the timer (aled.pas:241-245 "if fValue then SetToTrueColor
    //  else SetToFalseColor" runs BEFORE fBlink is actually cleared)
    // =======================================================================
    {
        LedCore led;
        led.SetValue(true);
        led.SetBlink(true);
        led.OnTimerTick();               // ColorTemp now true (mid-blink)
        CHECK(led.ColorTemp == true);
        led.SetBlink(false);
        CHECK(led.GetBlink() == false);
        CHECK(led.IsTimerActive() == false);
        CHECK(led.ColorTemp == false);    // reset to solid TRUE-colour (Value is still true)
    }

    // =======================================================================
    //  SetTrueColor / SetFalseColor implicitly cycle Blink off-and-back-on
    //  (aled.pas SetTrueColor :254-266 / SetFalseColor :268-280 -- both read
    //  Temp:=fBlink; if fBlink then Blink:=false; ...; Blink:=Temp)
    // =======================================================================
    {
        LedCore led;
        led.SetValue(true);
        led.SetBlink(true);
        CHECK(led.IsTimerActive() == true);

        led.SetTrueColor(0x00112233);
        CHECK(led.GetTrueColor() == 0x00112233);
        // Blink must have been restored to true (Temp was true) -- and since
        // Value is still true, the timer must be running again too:
        CHECK(led.GetBlink() == true);
        CHECK(led.IsTimerActive() == true);

        led.SetBlink(false);
        led.SetFalseColor(0x00445566);
        CHECK(led.GetFalseColor() == 0x00445566);
        CHECK(led.GetBlink() == false);      // Temp was false this time -- stays false
        CHECK(led.IsTimerActive() == false);
    }

    // =======================================================================
    //  MEDIUM-1 regression: SetTrueColor/SetFalseColor snap Width/Height back
    //  to the current style's extents, because golden's SetTrueColor/
    //  SetFalseColor (aled.pas:254-266/:268-280) both call CreateLedBitmap
    //  (:262/:276), and CreateLedBitmap unconditionally reassigns Width/
    //  Height from fLEDStyle in every one of its 6 branches (:112-153).
    //  FAILS if a future change drops the ApplyStyleExtents() call this side
    //  effect requires (LedCore::SetTrueColor/SetFalseColor).
    // =======================================================================
    {
        LedCore led;                       // default style LEDSmall -> 22x22
        led.Width = 999;
        led.Height = 999;                  // force a non-style value
        led.SetTrueColor(0x00778899);       // any color change re-triggers CreateLedBitmap
        CHECK(led.Width == 22 && led.Height == 22);

        led.Width = 12345;
        led.Height = 6789;
        led.SetFalseColor(0x00998877);
        CHECK(led.Width == 22 && led.Height == 22);

        // Same check under a non-default style, to rule out a coincidental
        // match against the default 22x22.
        LedCore led2;
        led2.SetLEDStyle(LEDLarge);        // 32x32
        led2.Width = 1;
        led2.Height = 1;
        led2.SetTrueColor(0x00010203);
        CHECK(led2.Width == 32 && led2.Height == 32);
    }

    // =======================================================================
    //  SetInterval (aled.pas SetInterval :282-287)
    // =======================================================================
    {
        LedCore led;
        led.SetInterval(250);
        CHECK(led.GetInterval() == 250);
    }

    // =======================================================================
    //  MyLedCore -- ctor forces LEDSqLarge (MyLed.cpp:19-29)
    // =======================================================================
    {
        MyLedCore led;
        CHECK(led.GetLEDStyle() == LEDSqLarge);
        CHECK(led.Width == 22 && led.Height == 22);
        CHECK(led.InPort == 0 && led.InBit == 0 && led.InType == 0);

        led.SetPort(AnsiString("1F"));
        CHECK(led.InPort == 0x1F);
        led.SetPort(AnsiString(" 1f "));         // leading/trailing spaces + lowercase
        CHECK(led.InPort == 0x1F);
        // Hex-tail-scan: parses the trailing contiguous hex run only, stops
        // at the first non-hex char scanning BACKWARD (MyLed.cpp:84-95).
        led.SetPort(AnsiString("XY12"));
        CHECK(led.InPort == 0x12);               // "12" parsed, "XY" prefix ignored
        led.SetPort(AnsiString(""));
        CHECK(led.InPort == 0);

        led.SetBit(AnsiString("7"));
        CHECK(led.InBit == 7);
        led.SetType(AnsiString("2"));
        CHECK(led.InType == 2);

        led.SetAlias(AnsiString("TestSuck"));
        CHECK(std::string(led.GetAlias().c_str()) == "TestSuck");
    }

    // =======================================================================
    //  MyLedLaneCore -- ctor forces LEDSqLarge (MyLedLane.cpp:19-35)
    // =======================================================================
    {
        MyLedLaneCore led;
        CHECK(led.GetLEDStyle() == LEDSqLarge);
        CHECK(led.InRing == 0 && led.InIP == 0 && led.InPort == 0);
        CHECK(led.InBit == 0 && led.InType == 0 && led.ISABase == 0);

        // Plain, well-formed, in-buffer-size inputs: byte-identical to golden
        // regardless of the buffer-overflow bug below (never triggered here).
        led.SetRing(AnsiString("5"));
        CHECK(led.InRing == 5);
        led.SetIP(AnsiString("42"));
        CHECK(led.InIP == 42);
        led.SetPort(AnsiString("9"));
        CHECK(led.InPort == 9);
        led.SetBit(AnsiString("3"));
        CHECK(led.InBit == 3);

        // GOLDEN BUG (MyLedLane.cpp WriteRing/WriteIP/WritePort/WriteBit,
        // :74-104): fixed 2/3-byte stack buffers, truncate-not-crash
        // reproduction (see LedCore.cpp ParseFixedPrefixInt). Regression:
        // FAILS if a future "fix" widens the buffer and the full value
        // survives instead of being truncated.
        led.SetRing(AnsiString("12"));
        CHECK(led.InRing == 1);          // truncated to the first 1 char ("1"), NOT 12
        led.SetIP(AnsiString("192"));
        CHECK(led.InIP == 19);           // truncated to the first 2 chars ("19"), NOT 192
        led.SetPort(AnsiString("99"));
        CHECK(led.InPort == 9);          // truncated to "9", NOT 99
        led.SetBit(AnsiString("42"));
        CHECK(led.InBit == 4);           // truncated to "4", NOT 42

        // WriteType uses plain atoi with NO fixed buffer (MyLedLane.cpp:106-111)
        // -- verified NOT subject to the same truncation bug:
        led.SetType(AnsiString("123"));
        CHECK(led.InType == 123);        // full value survives

        led.SetISA(AnsiString("55"));
        CHECK(led.ISABase == 55);        // plain atoi too -- full value survives

        led.SetAlias(AnsiString("Lane7"));
        CHECK(std::string(led.GetAlias().c_str()) == "Lane7");
    }

    // ===================================================================
    //  summary
    // ===================================================================
    std::printf("\nledcore harness: %d/%d checks passed\n", g_total - g_fail, g_total);
    if (g_fail != 0) {
        std::printf("RESULT: FAILED (%d failures)\n", g_fail);
        return 1;
    }
    std::printf("RESULT: OK\n");
    return 0;
}
