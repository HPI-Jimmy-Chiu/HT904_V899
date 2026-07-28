// ===========================================================================
//  vclcompat/LedCore.cpp
//  See LedCore.h for scope/provenance. Every method banner cites the exact
//  golden line range it translates (aled.pas, MyLed.cpp, MyLedLane.cpp,
//  all under D:\HT9045\elec\).
// ===========================================================================
#include "vclcompat/LedCore.h"
#include <cctype>
#include <cstdlib>

namespace vclcompat {

// ===========================================================================
//  LedCore
// ===========================================================================

LedCore::LedCore()                                                // aled.pas Create :81-97
{
    // aled.pas:86-91. (Golden also sets Height:=16;Width:=16 at :84-85
    // BEFORE this, then immediately overwrites both via CreateLedBitmap at
    // the end of the ctor for every style including the default LEDSmall --
    // that transient 16x16 is never observable after construction returns,
    // so it is elided here rather than modelled and discarded.)
    fTrueColor = clLime;
    fFalseColor = clSilver;
    fBlink = false;
    fValue = false;
    fLEDStyle = LEDSmall;
    fInterval = 1000;
    bTimerActive = false;      // aled.pas fLedTimer := nil
    ColorTemp = true;          // aled.pas:95 -- set BEFORE CreateLedBitmap, deliberately not inside it
    ApplyStyleExtents();       // aled.pas:96 CreateLedBitmap -- Width/Height half only
}

void LedCore::ApplyStyleExtents()                                  // aled.pas CreateLedBitmap
                                                                   // :106-160, Width/Height half
{
    switch (fLEDStyle) {
        case LEDSmall:      Width = 22; Height = 22; break;   // aled.pas:112-118
        case LEDLarge:      Width = 32; Height = 32; break;   // aled.pas:119-125
        case LEDSqSmall:    Width = 22; Height = 22; break;   // aled.pas:126-132
        case LEDSqLarge:    Width = 22; Height = 22; break;   // aled.pas:133-139
        case LEDHorizontal: Width = 22; Height = 14; break;   // aled.pas:140-146
        case LEDVertical:   Width = 14; Height = 22; break;   // aled.pas:147-153
    }
    // The bitmap Create/LoadImage/FloodFill half (aled.pas:106-111,156-159)
    // is a real-GDI concern -- see vclcompat/render/LedRender.cpp (W7-C2),
    // which owns the embedded aled.res bytes for these 6 styles.
}

void LedCore::SetToTrueColorState()                                // aled.pas SetToTrueColor :182-186
{
    // ColorTemp:=false selects fLedBitmap[0], the TRUE-colour art -- the
    // naming looks backwards (ColorTemp==true shows the FALSE colour) but
    // this is golden's own polarity, preserved verbatim; Paint's
    // fLedBitmap[Integer(ColorTemp)] is what makes it consistent.
    ColorTemp = false;
    // Canvas->StretchDraw(...) dropped -- renderer concern (W7-C2).
}

void LedCore::SetToFalseColorState()                               // aled.pas SetToFalseColor :188-192
{
    ColorTemp = true;
    // Canvas->StretchDraw(...) dropped -- renderer concern (W7-C2).
}

void LedCore::StartTimer()                                         // aled.pas SetLedTimer :194-202
{
    if (bTimerActive) return;                  // aled.pas:196 "if Assigned(fLedTimer) then Exit"
    // aled.pas:197 "if (csDesigning in ComponentState) then Exit" -- design-time-only guard, N/A
    // to a framework-free/headless build (there is no IDE design surface here to guard against).
    ColorTemp = false;                          // aled.pas:198 -- yes, SetLedTimer forces ColorTemp
                                                 // false directly, independent of/in addition to
                                                 // whatever SetToTrueColorState already did before it
                                                 // was called from ChangeValue/ChangeBlink.
    bTimerActive = true;                        // aled.pas:199 fLedTimer := TTimer.Create(Self)
    // fLedTimer.Interval := fInterval (aled.pas:200) -- fInterval is already the single source of
    // truth here (no separate copy inside a real timer object to keep in sync).
}

void LedCore::StopTimer()                                          // aled.pas ResetLedTimer :204-210
{
    bTimerActive = false;                       // aled.pas:206-209 (Destroy + nil out fLedTimer)
}

void LedCore::SetValue(bool v)                                     // aled.pas ChangeValue :223-237
{
    if (fValue != v) {
        fValue = v;
        if (fValue) {
            SetToTrueColorState();
            if (fBlink) StartTimer();
        } else {
            StopTimer();
            SetToFalseColorState();
        }
    }
}

void LedCore::SetBlink(bool v)                                     // aled.pas ChangeBlink :239-252
{
    if (fBlink != v) {
        if (fValue) SetToTrueColorState(); else SetToFalseColorState();
        fBlink = v;
        if (v && fValue) StartTimer(); else StopTimer();
    }
}

void LedCore::SetTrueColor(TColor v)                                // aled.pas SetTrueColor :254-266
{
    if (fTrueColor != v) {
        bool temp = fBlink;
        if (fBlink) SetBlink(false);   // aled.pas:260 "if fBlink then Blink := false" -- goes
                                        // through the full property setter (ChangeBlink), same as
                                        // golden (`Blink := false;` is a property write, not a
                                        // direct field assignment).
        fTrueColor = v;
        // AI(W906-W7-C1) 20260728: golden calls CreateLedBitmap() here (aled.pas:262), and
        // CreateLedBitmap unconditionally reassigns Width/Height from fLEDStyle in every one
        // of its 6 branches (aled.pas:112-153) -- so changing TrueColor snaps the control's
        // size back to its style extents even if a caller had set Width/Height to something
        // else in between. That IS a real, observable core-state side effect (confirmed by
        // reading aled.pas:106-160), not just a bitmap-art regen -- the previous comment here
        // claiming "no further CORE state changes result" was wrong. Reproduced via
        // ApplyStyleExtents(), which is exactly the Width/Height half of CreateLedBitmap this
        // core already models (see LedCore::ApplyStyleExtents). The LoadImage/FloodFill half
        // remains a renderer concern (W7-C2, vclcompat/render/LedRender.cpp).
        ApplyStyleExtents();
        SetBlink(temp);                 // aled.pas:263 "Blink := Temp" -- same property-setter
                                        // re-entry note as above; if temp is true this re-enables
                                        // blink/restarts the timer, exactly as golden does.
        // Repaint() dropped (aled.pas:264) -- renderer concern.
    }
}

void LedCore::SetFalseColor(TColor v)                               // aled.pas SetFalseColor :268-280
{
    if (fFalseColor != v) {
        bool temp = fBlink;
        if (fBlink) SetBlink(false);
        fFalseColor = v;
        ApplyStyleExtents();   // aled.pas:276 CreateLedBitmap() -- same Width/Height snap-back
                                // side effect as SetTrueColor above; see the comment there.
        SetBlink(temp);
    }
}

void LedCore::SetInterval(long v)                                   // aled.pas SetInterval :282-287
{
    fInterval = v;
    // aled.pas:285-286 "if Assigned(fLedTimer) then fLedTimer.Interval := V" -- no-op here since
    // fInterval IS the timer's interval in this stand-in (see StartTimer's own note above).
}

void LedCore::SetLEDStyle(TLEDStyle v)                              // aled.pas ChangeStyle :213-220
{
    if (fLEDStyle != v) {
        fLEDStyle = v;
        ApplyStyleExtents();
        // Repaint() dropped -- renderer concern.
    }
}

void LedCore::OnTimerTick()                                        // aled.pas OnLedTimer :175-180
{
    ColorTemp = !ColorTemp;
    // Canvas->StretchDraw(...) dropped -- renderer concern.
    // "if (Assigned(OnTimer)) then fOnTimer(Self)" (aled.pas:179) -- the OnTimer VCL event is a
    // shell/binder concern (nothing in golden's MyLed/MyLedLane .dfm instances wires OnTimer to
    // application code; grep confirms zero handlers), not modelled here.
}

// ===========================================================================
//  MyLedCore -- golden TMyLed
//
//  GOLDEN BUG: see LedCore.h's banner on this class -- WritePort/WriteBit/
//  WriteType below never assign sInPort/sInBit/sInType, so golden's own
//  Port/Bit/Type property getters always read back "". No string getter is
//  modelled here for that reason (see header).
// ===========================================================================

MyLedCore::MyLedCore()                                              // MyLed.cpp:19-29
    : LedCore(), InPort(0), InBit(0), InType(0)
{
    SetLEDStyle(LEDSqLarge);   // MyLed.cpp:22 "LEDStyle=LEDSqLarge;" -- a __published property
                               // write, so it runs the full ChangeStyle body (base ctor left
                               // fLEDStyle==LEDSmall, so this DOES fire CreateLedBitmap again --
                               // see KNOWLEDGE gotcha #11 / plan §6-B1c: the resulting Width/Height
                               // (22x22) happens to be identical to LEDSmall's, so this is not
                               // observable as a size change, just documented for fidelity).
}

// Golden's CutSpaceAtHead/CutSpaceAtTail (MyLed.cpp:31-70, byte-identical
// copies also in butPa1.cpp) strip ONLY literal ' ' (0x20) from each end --
// narrower than AnsiString::Trim()'s BCB6-faithful "strip anything <= ' ',
// including control chars" (vclcompat/AnsiString.cpp:111-117). For every
// realistic Port string (hex digits + spaces) the two are indistinguishable,
// but we reproduce the narrower, golden-exact rule rather than lean on the
// broader Trim() and risk silently changing behaviour for a stray control
// byte. Shared by MyLedCore::SetPort (below) and BtnPanelCore::SetPort
// (BtnPanelCore.cpp), which run the identical algorithm (MyLed.cpp:72-96 /
// butPa1.cpp:156-180).
static AnsiString TrimAsciiSpaceOnly(const AnsiString& s)
{
    int len = s.Length();
    int b = 1, e = len;
    while (b <= e && s[b] == ' ') ++b;
    while (e >= b && s[e] == ' ') --e;
    if (b > e) return AnsiString("");
    return s.SubString(b, e - b + 1);
}

void MyLedCore::SetPort(const AnsiString& s)                        // MyLed.cpp WritePort :72-96
{
    // Golden trims leading/trailing ' ' (CutSpaceAtHead/CutSpaceAtTail,
    // MyLed.cpp:31-70), uppercases (strupr), then walks the result from the
    // TAIL backward accumulating a base-16 value, stopping at the first
    // non-hex-digit character (so it parses the trailing contiguous hex
    // run, not the whole string). Golden does this via a fixed char[256]
    // buffer + strcpy/strupr; reimplemented here with AnsiString/std::string
    // ops (no fixed buffer at all) -- for every input that golden's 256-byte
    // buffer wasn't already going to overflow (i.e. every realistic .dfm
    // Port string), this produces the byte-identical InPort. Not treated as
    // a "behaviour-preserved-not-UB" case (unlike MyLedLane's 2/3-byte
    // buffers below): a 256-byte stack buffer is never actually exercised
    // past its limit by any real Port string in this codebase.
    AnsiString trimmed = TrimAsciiSpaceOnly(s);
    AnsiString upper = trimmed.UpperCase();
    int len = upper.Length();
    int inPort = 0, scale = 1;
    for (int i = len; i >= 1; --i) {     // AnsiString is 1-based; walk tail to head
        char c = upper[i];
        int j;
        if (c >= '0' && c <= '9') j = c - '0';
        else if (c >= 'A' && c <= 'F') j = 10 + (c - 'A');
        else break;
        inPort += j * scale;
        scale *= 16;
    }
    InPort = inPort;
}

void MyLedCore::SetBit(const AnsiString& s)                         // MyLed.cpp WriteBit :98-101
{
    InBit = std::atoi(s.c_str());
}

void MyLedCore::SetType(const AnsiString& s)                        // MyLed.cpp WriteType :103-106
{
    InType = std::atoi(s.c_str());
}

void MyLedCore::SetAlias(const AnsiString& s)                       // MyLed.cpp WriteAlias :108-111
{
    sAlias = s;
}

AnsiString MyLedCore::GetAlias() const                              // MyLed.cpp ReadAlias :113-116
{
    return sAlias;
}

// ===========================================================================
//  MyLedLaneCore -- golden TMyLedLane
//
//  GOLDEN BUG: see LedCore.h's banner on this class -- WriteRing/WriteIP/
//  WritePort/WriteBit/WriteType below never assign sRing/sIP/sInPort/
//  sInBit/sInType, so golden's own Ring/IP/Port/Bit/Type property getters
//  always read back "" (Alias and IsISA are NOT affected -- both are
//  genuinely assigned, see header). No string getter is modelled here for
//  the affected five, for the same reason as MyLedCore above.
// ===========================================================================

// GOLDEN BUG (MyLedLane.cpp WriteRing/WriteIP/WritePort/WriteBit, :74-104):
// each does `char buf[N]; strcpy(buf, Value.c_str()); buf[N-1]='\0'; atoi(buf);`
// with N=2 (Ring/Port/Bit) or N=3 (IP). For any Value longer than N-1
// characters this is a genuine stack-buffer overflow (strcpy writes past the
// end of buf) -- real undefined behaviour in golden, not just a quirky
// result. Per this wave's own instructions we preserve OBSERVABLE BEHAVIOUR,
// not the UB itself: the post-write `buf[N-1]='\0'` always wins regardless
// of what the overflowing strcpy scribbled into/past buf[N-1], so the value
// golden's atoi() actually sees is consistently "first N-1 characters of
// Value, forcibly NUL-terminated" -- i.e. a truncation, not garbage. We
// reproduce that truncation directly via AnsiString::SubString, with no real
// buffer and therefore no real overflow. Marked
// GOLDEN BUG / behaviour-preserved-not-UB-preserved, matching this wave's
// brief verbatim. (BtnPanelLane.cpp's equivalent setters use plain atoi with
// no fixed buffer at all -- confirmed by reading that file -- so they need
// no such treatment; only the LED lane family has this bug.)
static int ParseFixedPrefixInt(const AnsiString& value, int maxChars)
{
    AnsiString truncated = value.SubString(1, maxChars);
    return std::atoi(truncated.c_str());
}

MyLedLaneCore::MyLedLaneCore()                                      // MyLedLane.cpp:19-35
    : LedCore(), InRing(0), InIP(0), InPort(0), InBit(0), InType(0), ISABase(0)
{
    SetLEDStyle(LEDSqLarge);   // MyLedLane.cpp:22, same note as MyLedCore::MyLedCore above
}

void MyLedLaneCore::SetRing(const AnsiString& value)                // MyLedLane.cpp WriteRing :74-80
{
    InRing = ParseFixedPrefixInt(value, 1);   // golden: char str[2] -- 1 real char + NUL
}

void MyLedLaneCore::SetIP(const AnsiString& value)                  // MyLedLane.cpp WriteIP :82-88
{
    InIP = ParseFixedPrefixInt(value, 2);     // golden: char str[3] -- 2 real chars + NUL
}

void MyLedLaneCore::SetPort(const AnsiString& s)                    // MyLedLane.cpp WritePort :90-96
{
    InPort = ParseFixedPrefixInt(s, 1);       // golden: char str[2] -- 1 real char + NUL. NOTE:
                                              // unlike MyLedCore::SetPort above, TMyLedLane's own
                                              // WritePort is NOT the hex-tail-scan algorithm -- it
                                              // is this fixed-buffer-then-atoi shape instead
                                              // (verified directly against MyLedLane.cpp, not
                                              // assumed from MyLed's shape).
}

void MyLedLaneCore::SetBit(const AnsiString& s)                     // MyLedLane.cpp WriteBit :98-104
{
    InBit = ParseFixedPrefixInt(s, 1);        // golden: char str[2]
}

void MyLedLaneCore::SetType(const AnsiString& s)                    // MyLedLane.cpp WriteType :106-111
{
    InType = std::atoi(s.c_str());            // plain atoi, no fixed buffer -- safe in golden too
}

void MyLedLaneCore::SetAlias(const AnsiString& s)                   // MyLedLane.cpp WriteAlias :113-116
{
    sAlias = s;
}

AnsiString MyLedLaneCore::GetAlias() const                          // MyLedLane.cpp ReadAlias :118-121
{
    return sAlias;
}

void MyLedLaneCore::SetISA(const AnsiString& s)                     // MyLedLane.cpp WriteISA :123-127
{
    ISABase = std::atoi(s.c_str());           // plain atoi, no fixed buffer -- safe
}

} // namespace vclcompat
