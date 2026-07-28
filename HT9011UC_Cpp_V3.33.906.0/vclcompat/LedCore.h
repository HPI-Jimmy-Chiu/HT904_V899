// ===========================================================================
//  vclcompat/LedCore.h
//  Framework-free state core for the "LED" custom-control family (D5: own
//  file, not vclcompat/Controls.h).
//
//  Golden sources (cp950/big5-decoded, verified D:\HT9045 §4-V3):
//    - D:\HT9045\elec\Component\aled.pas (TALed : TGraphicControl, 306 lines,
//      the common base -- Delphi, not C++, but this IS the real base class
//      every instance ultimately runs; ZERO drawing code lives in the two
//      C++ subclasses below, confirmed by grep, so aled.pas's state machine
//      -- Value/Blink/Interval/LEDStyle/TrueColor/FalseColor/ColorTemp -- is
//      the actual behaviour this core must reproduce).
//    - D:\HT9045\elec\myvcl\MyLed.h/.cpp (TMyLed : TALed, 126+39 lines,
//      96 .dfm instances) -- adds Port/Bit/Type/Alias IO-address metadata.
//    - D:\HT9045\elec\myvcl\MyLedLane.h/.cpp (TMyLedLane : TALed, 136+50
//      lines, 1214 .dfm instances -- the single highest-count custom
//      control in the whole codebase) -- adds Ring/IP/ISA on top of
//      Port/Bit/Type/Alias.
//
//  SCOPE: LedCore models TALed's full Value/Blink/Interval/LEDStyle/
//  TrueColor/FalseColor state machine (aled.pas:81-287) MINUS every Canvas
//  call (Paint/CreateLedBitmap's bitmap loading/StretchDraw/FloodFill) --
//  those move to the W7-C2 HDC renderer (vclcompat/render/LedRender.*),
//  which embeds the aled.res bitmap bytes and consumes this core's ColorTemp/
//  Width/Height/TrueColor/FalseColor state directly. MyLedCore/
//  MyLedLaneCore add the two subclasses' IO-address parsing on top.
//
//  aled.pas has NO real OS timer in a framework-free core (fLedTimer is a
//  VCL TTimer -- a real Win32 timer needs a window to post WM_TIMER to,
//  which doesn't exist here). This core models "is a timer conceptually
//  running" as a bool (IsTimerActive()) and exposes OnTimerTick() as the
//  hook a real timer (owned by the eventual W7-C5 MFC shell) would call on
//  each tick -- same shape as aled.pas's own OnLedTimer, just externally
//  driven instead of VCL-timer-driven.
//
//  AI(W906-W7-C1) 20260728: added the GOLDEN BUG banners on MyLedCore/
//  MyLedLaneCore below (write-only Port/Bit/Type/Ring/IP properties whose
//  golden getters always read "") and, in LedCore.cpp, made SetTrueColor/
//  SetFalseColor call ApplyStyleExtents() -- golden's CreateLedBitmap
//  (aled.pas:262/276) resets Width/Height on every colour change, a real
//  side effect a prior pass here had dropped.
// ===========================================================================
#ifndef VCLCOMPAT_LEDCORE_H
#define VCLCOMPAT_LEDCORE_H

#include "vclcompat/AnsiString.h"

namespace vclcompat {

// See TrayCore.h for why this block is guarded and shared verbatim across
// TrayCore.h / BtnPanelCore.h / LedCore.h.
#ifndef HT9045_W7C1_TCOLOR_SHIM
#define HT9045_W7C1_TCOLOR_SHIM
typedef int TColor;                           // golden Graphics.hpp TColor (0x00BBGGRR int)
const TColor clBlack   = 0x00000000;
const TColor clGreen   = 0x00008000;
const TColor clRed     = 0x000000FF;
const TColor clBlue    = 0x00FF0000;
const TColor clLime    = 0x0000FF00;
const TColor clYellow  = 0x0000FFFF;
const TColor clWhite   = 0x00FFFFFF;
const TColor clSilver  = 0x00C0C0C0;
const TColor clBtnFace = TColor(0x8000000F);
#endif // HT9045_W7C1_TCOLOR_SHIM

// golden aled.pas:23 -- order matters (it's the Pascal enum's declared
// order); preserved verbatim even though it looks unsorted (Small before
// Large before SqSmall before SqLarge before Vertical before Horizontal).
enum TLEDStyle { LEDSmall, LEDLarge, LEDSqSmall, LEDSqLarge, LEDVertical, LEDHorizontal };

// ===========================================================================
//  LedCore -- golden TALed (aled.pas:24-74 decl, :81-287 impl).
// ===========================================================================
class LedCore {
public:
    LedCore();                                            // aled.pas:81-97 (Create), minus
                                                           // CreateLedBitmap's actual bitmap I/O
    virtual ~LedCore() {}

    // -- geometry output, driven by LEDStyle (aled.pas:106-160 CreateLedBitmap,
    //    Width/Height half only -- the LoadImage/TBitmap/FloodFill half is a
    //    W7-C2 renderer concern; see LedRender.cpp for the aled.res bytes) --
    int Width, Height;

    // -- aled.pas Paint (:170-173)/OnLedTimer(:175-180) select
    //    fLedBitmap[Integer(ColorTemp)]; a framework-free core has no bitmap
    //    to pick, so ColorTemp is exposed directly for the renderer to index
    //    with (false==showing the TrueColor art, true==showing the
    //    FalseColor art -- yes, this polarity looks backwards; it is
    //    golden's own naming, preserved verbatim, see SetToTrueColorState
    //    below). --
    bool ColorTemp;

    // -- published properties (aled.pas:63-68) --
    void       SetValue(bool v);                          // aled.pas ChangeValue :223-237
    bool       GetValue() const { return fValue; }
    void       SetBlink(bool v);                           // aled.pas ChangeBlink :239-252
    bool       GetBlink() const { return fBlink; }
    void       SetTrueColor(TColor v);                     // aled.pas SetTrueColor :254-266
    TColor     GetTrueColor() const { return fTrueColor; }
    void       SetFalseColor(TColor v);                    // aled.pas SetFalseColor :268-280
    TColor     GetFalseColor() const { return fFalseColor; }
    void       SetInterval(long v);                        // aled.pas SetInterval :282-287
    long       GetInterval() const { return fInterval; }
    void       SetLEDStyle(TLEDStyle v);                   // aled.pas ChangeStyle :213-220
    TLEDStyle  GetLEDStyle() const { return fLEDStyle; }

    // -- timer stand-in (see file banner) --
    bool IsTimerActive() const { return bTimerActive; }
    void OnTimerTick();                                    // aled.pas OnLedTimer :175-180 (state
                                                            // half only -- Canvas->StretchDraw and
                                                            // the OnTimer user-event callback are
                                                            // renderer/shell concerns)

protected:
    void SetToTrueColorState();                            // aled.pas SetToTrueColor :182-186
    void SetToFalseColorState();                           // aled.pas SetToFalseColor :188-192
    void StartTimer();                                      // aled.pas SetLedTimer :194-202
    void StopTimer();                                       // aled.pas ResetLedTimer :204-210
    void ApplyStyleExtents();                              // Width/Height half of CreateLedBitmap
                                                            // (aled.pas:106-160)

private:
    TColor fTrueColor, fFalseColor;
    bool fBlink;
    TLEDStyle fLEDStyle;
    long fInterval;
    bool fValue;
    bool bTimerActive;   // stand-in for "Assigned(fLedTimer)" (aled.pas has no real TTimer here)
};

// ===========================================================================
//  MyLedCore -- golden TMyLed (MyLed.h/.cpp). Adds Port/Bit/Type/Alias IO
//  address metadata on top of LedCore. ZERO drawing code (verified §4-V3).
//
//  GOLDEN BUG (MyLed.h:32-34 + MyLed.cpp:19-29/72-106, verified directly --
//  not assumed by symmetry with MyLedLaneCore below): golden declares
//  `__property AnsiString Port={read=sInPort, write=WritePort}` (same shape
//  for Bit/sInBit, Type/sInType), but WritePort/WriteBit/WriteType NEVER
//  assign sInPort/sInBit/sInType -- those three AnsiString fields are set to
//  "" exactly once, in the ctor (MyLed.cpp:23-25), and never touched again.
//  So in golden, `->Port`/`->Bit`/`->Type` reads always yield "" no matter
//  what was ever written through the setter -- a genuine write-only-property
//  bug, not a modelling gap. (Contrast Alias: WriteAlias/ReadAlias DO use
//  the same backing field, sAlias, correctly -- only Port/Bit/Type are
//  affected.) This core deliberately does NOT provide a GetPort()/GetBit()/
//  GetType() string getter: adding one that returns a real value would be
//  MORE capable than golden (a false upgrade), and adding one hardcoded to
//  always return "" would be a strange thing to test/rely on. InPort/InBit/
//  InType (the actually-parsed, actually-consumed ints) are the complete
//  observable surface, matching what every real caller in golden actually
//  reads (verified: nothing outside WritePort/WriteBit/WriteType itself
//  ever reads sInPort/sInBit/sInType, since nothing COULD get anything
//  useful out of them).
// ===========================================================================
class MyLedCore : public LedCore {
public:
    MyLedCore();                                           // MyLed.cpp:19-29

    int InPort, InBit, InType;                             // MyLed.h public block

    void SetPort(const AnsiString& s);                     // MyLed.cpp WritePort :72-96 (hex,
                                                            // tail-anchored -- see .cpp)
    void SetBit(const AnsiString& s);                      // MyLed.cpp WriteBit :98-101
    void SetType(const AnsiString& s);                     // MyLed.cpp WriteType :103-106
    void SetAlias(const AnsiString& s);                    // MyLed.cpp WriteAlias :108-111
    AnsiString GetAlias() const;                           // MyLed.cpp ReadAlias :113-116

private:
    AnsiString sAlias;
};

// ===========================================================================
//  MyLedLaneCore -- golden TMyLedLane (MyLedLane.h/.cpp). Adds
//  Ring/IP/Port/Bit/Type/Alias/ISA. ZERO drawing code (verified §4-V3).
//  1214 .dfm instances -- the single highest-count custom control in the
//  codebase.
//
//  GOLDEN BUG (MyLedLane.h:41-47 + MyLedLane.cpp:19-35/74-111, verified
//  directly): same shape as MyLedCore's bug above -- `Ring`/`IP`/`Port`/
//  `Bit`/`Type` are declared as `__property AnsiString ... ={read=sRing/
//  sIP/sInPort/sInBit/sInType, write=WriteRing/WriteIP/WritePort/WriteBit/
//  WriteType}`, but none of those 5 Write* functions ever assign their
//  matching backing field -- all 5 are set to "" once in the ctor
//  (MyLedLane.cpp:19-35) and never again, so golden's `->Ring`/`->IP`/
//  `->Port`/`->Bit`/`->Type` reads always yield "". IMPORTANT ASYMMETRY,
//  verified directly rather than assumed from the pattern above: `Alias`
//  (sAlias, via WriteAlias/ReadAlias) and `IsISA` (sISA, via WriteISA) are
//  BOTH genuinely, correctly assigned in golden -- WriteISA is
//  `sISA=S; ISABase=atoi(S.c_str());` (MyLedLane.cpp:123-127), so IsISA is
//  NOT part of this bug, unlike the near-identical-looking Ring/IP/Port/Bit/
//  Type quintet. Same design choice as MyLedCore: no GetRing()/GetIP()/
//  GetPort()/GetBit()/GetType() string getter is provided here, precisely
//  because golden's own would always return "" and adding a working one
//  would be a false upgrade over golden.
// ===========================================================================
class MyLedLaneCore : public LedCore {
public:
    MyLedLaneCore();                                       // MyLedLane.cpp:19-35

    int InRing, InIP, InPort, InBit, InType, ISABase;      // MyLedLane.h public block

    void SetRing(const AnsiString& value);                 // MyLedLane.cpp WriteRing :74-80 --
                                                            // GOLDEN BUG, behaviour-preserved-not-
                                                            // UB-preserved, see .cpp
    void SetIP(const AnsiString& value);                   // MyLedLane.cpp WriteIP :82-88 -- ditto
    void SetPort(const AnsiString& s);                     // MyLedLane.cpp WritePort :90-96 -- ditto
    void SetBit(const AnsiString& s);                      // MyLedLane.cpp WriteBit :98-104 -- ditto
    void SetType(const AnsiString& s);                     // MyLedLane.cpp WriteType :106-111
                                                            // (plain atoi, no fixed buffer -- safe
                                                            // in golden too, unlike Ring/IP/Port/Bit
                                                            // above)
    void SetAlias(const AnsiString& s);                    // MyLedLane.cpp WriteAlias :113-116
    AnsiString GetAlias() const;                           // MyLedLane.cpp ReadAlias :118-121
    void SetISA(const AnsiString& s);                      // MyLedLane.cpp WriteISA :123-127
                                                            // (plain atoi, safe)

private:
    AnsiString sAlias;
};

} // namespace vclcompat
#endif // VCLCOMPAT_LEDCORE_H
