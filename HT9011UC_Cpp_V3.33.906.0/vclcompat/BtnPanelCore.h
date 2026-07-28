// ===========================================================================
//  vclcompat/BtnPanelCore.h
//  Framework-free state core for the "Button panel" custom-control family
//  (D5: own file, not vclcompat/Controls.h).
//
//  Golden sources (D:\HT9045\elec\myvcl\, cp950/big5-decoded, verified
//  D:\HT9045 §4-V3):
//    - butPa1.h/.cpp       (TBtnPanel : TPanel,     217+67 lines,  56 .dfm instances)
//    - BtnPanelLane.h/.cpp (TBtnPanelLane : TPanel, 178+78 lines, 701 .dfm instances)
//  ZERO drawing code in either (confirmed by grep for
//  Canvas|Paint|DrawText|Rectangle|Ellipse|StretchDraw -- both score 0):
//  these controls are entirely "toggle Down and the base TPanel's own
//  Color/Font->Color/BevelInner/BevelOuter properties follow", which is why
//  a framework-free core is a complete, faithful translation with no
//  renderer half needed at all (unlike Tray/Led, there is no W7-C2 piece
//  for this family -- the "rendering" IS just those 4 TPanel property
//  writes, exposed here as plain public state for a real TPanel/CStatic
//  shell to copy across).
//
//  AI(W906-W7-C1) 20260728: added the GOLDEN BUG banners on BtnPanelCore/
//  BtnPanelLaneCore below (write-only Port/Bit/Type/Lane/IP properties
//  whose golden getters always read "") and corrected the ctor comment in
//  BtnPanelCore.cpp, which had misattributed why those fields go unread.
// ===========================================================================
#ifndef VCLCOMPAT_BTNPANELCORE_H
#define VCLCOMPAT_BTNPANELCORE_H

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
const TColor clBtnFace = TColor(0x8000000F);  // system-colour reference (COLOR_BTNFACE), stored
                                               // verbatim/unresolved -- see TrayCore.h's own note.
#endif // HT9045_W7C1_TCOLOR_SHIM

// golden ComCtrls.hpp TTabStyle (real VCL declared order: tsTabs, tsButtons,
// tsFlatButtons). Both golden classes' `TTabStyle FStyle` member (added
// 2023-08-26, "Steven 20230826 : for new GUI, add Flat Btn option") only
// ever distinguishes tsFlatButtons from "anything else", so the exact
// non-tsFlatButtons value is never semantically load-bearing here -- kept
// as a 3-way enum anyway for source fidelity with the golden field's
// declared type.
enum TTabStyleShim { tsTabs, tsButtons, tsFlatButtons };

// golden Graphics.hpp TPanelBevel (real VCL declared order: bvNone,
// bvLowered, bvRaised, bvSpace). Only bvNone/bvLowered/bvRaised are ever
// assigned by either golden class.
enum TBevelShim { bvNone, bvLowered, bvRaised, bvSpace };

// ===========================================================================
//  BtnPanelCoreBase -- the state machine shared byte-for-byte between
//  TBtnPanel and TBtnPanelLane (SetTrueColor/SetFalseColor/
//  SetTrueFontColor/SetFalseFontColor/SetPanelStatus/WriteStyle are
//  IDENTICAL between butPa1.cpp and BtnPanelLane.cpp -- verified by direct
//  comparison, not assumed). The two subclasses below only differ in their
//  IO-address (Port/Bit/Type/Alias vs Ring/IP/Port/Bit/Type/Alias/ISA)
//  parsing surface.
// ===========================================================================
class BtnPanelCoreBase {
public:
    BtnPanelCoreBase();                                   // butPa1.cpp:18-51 /
                                                           // BtnPanelLane.cpp:20-59 (identical body)
    virtual ~BtnPanelCoreBase() {}

    // -- outputs a real TPanel-backed shell applies verbatim --
    TColor Color;
    TColor FontColor;          // stand-in for golden's `Font->Color`
    TBevelShim BevelInner, BevelOuter;

    void   SetDown(bool v);                                // SetPanelStatus, butPa1.cpp:81-106 /
                                                            // BtnPanelLane.cpp:98-123 (identical)
    bool   GetDown() const { return bDown; }

    void   SetTrueColor(TColor v);                         // butPa1.cpp:53-58 / BtnPanelLane.cpp:70-75
    void   SetFalseColor(TColor v);                        // butPa1.cpp:60-65 / BtnPanelLane.cpp:77-82
    void   SetTrueFontColor(TColor v);                     // butPa1.cpp:67-72 / BtnPanelLane.cpp:84-89
    void   SetFalseFontColor(TColor v);                    // butPa1.cpp:74-79 / BtnPanelLane.cpp:91-96
    TColor GetTrueColor() const { return tcTrueColor; }
    TColor GetFalseColor() const { return tcFalseColor; }
    TColor GetTrueFontColor() const { return tcTrueFontColor; }
    TColor GetFalseFontColor() const { return tcFalseFontColor; }

    void   SetStyle(TTabStyleShim v);                      // WriteStyle, butPa1.cpp:202-206 /
                                                            // BtnPanelLane.cpp:173-177 (identical)
    TTabStyleShim GetStyle() const { return FStyle; }

protected:
    // GOLDEN BUG (butPa1.cpp:21-22 AND BtnPanelLane.cpp:23-24): the ctor
    // declares `TColor tcTrueColor=clBtnFace; TColor tcFalseColor=clBtnFace;`
    // as LOCAL variables inside the constructor body -- these shadow the
    // (identically-named) protected members below for the rest of the ctor.
    // The protected members tcTrueColor/tcFalseColor are therefore NEVER
    // assigned by the constructor in EITHER golden class (only the locals
    // are, and only the locals feed the one `Color=tcFalseColor;` ctor-time
    // read). tcTrueFontColor/tcFalseFontColor have no shadowing local at all
    // and are simply never touched anywhere in the ctor. All 4 members stay
    // whatever raw memory held before construction until the FIRST real
    // SetTrueColor/SetFalseColor/SetTrueFontColor/SetFalseFontColor call.
    //
    // Reading genuinely-uninitialised memory is undefined behaviour in both
    // golden and a faithful from-scratch port; per this wave's own
    // instructions to preserve OBSERVABLE BEHAVIOUR rather than real UB
    // (same treatment as MyLedLaneCore's Ring/IP/Port/Bit parsing, see
    // LedCore.cpp), these 4 members are seeded here to a recognisable,
    // deterministic sentinel (kUninitializedColorSentinel) instead of being
    // left as true indeterminate memory -- so a regression test can assert
    // "not yet meaningfully set" without invoking real UB. This IS a
    // documented divergence: it makes the bug observable/testable rather
    // than reproducing literal memory garbage.
    static const TColor kUninitializedColorSentinel = TColor(0xDEADBEEF);

    TColor tcTrueColor, tcFalseColor, tcTrueFontColor, tcFalseFontColor;
    bool bDown;
    TTabStyleShim FStyle;
};

// ===========================================================================
//  BtnPanelCore -- golden TBtnPanel (butPa1.h/.cpp). 56 .dfm instances.
//
//  GOLDEN BUG (butPa1.h:37-39 declarations + :60-63 properties, with
//  butPa1.cpp:18-51/156-190 -- AI(W906-W7-C-followup) 20260728: the
//  declaration citation was previously written as butPa1.h:32-34, which is
//  actually SetFalseFontColor/blank/SetPanelStatus; re-read and corrected):
//  golden declares `__property AnsiString Port={read=sOutPort,
//  write=WritePort}` (same shape for Bit/sOutBit, Type/sOutType), but
//  WritePort/WriteBit/WriteType NEVER assign sOutPort/sOutBit/sOutType --
//  those three AnsiString fields are set to "" exactly once, in the ctor
//  (butPa1.cpp:45-47), and never touched again. So in golden, `->Port`/
//  `->Bit`/`->Type` reads always yield "" no matter what was ever written
//  through the setter -- the same class of write-only-property bug as
//  LedCore.h's MyLedCore (see that banner). (Contrast Alias: WriteAlias/
//  ReadAlias DO use the same backing field, sAlias, correctly.) This core
//  deliberately does NOT provide a GetPort()/GetBit()/GetType() string
//  getter, for the identical reason given there: adding a working one
//  would be a false upgrade over golden, and OutPort/OutBit/OutType (the
//  actually-parsed, actually-consumed ints) are the complete observable
//  surface -- nothing outside WritePort/WriteBit/WriteType itself ever
//  reads sOutPort/sOutBit/sOutType in golden, since nothing COULD get
//  anything useful out of them.
// ===========================================================================
class BtnPanelCore : public BtnPanelCoreBase {
public:
    BtnPanelCore();                                        // butPa1.cpp:18-51 (ctor tail: sOutPort=""
                                                           // etc, OutPort/OutBit/OutType=0)

    int OutPort, OutBit, OutType;                          // butPa1.h public block

    void SetPort(const AnsiString& s);                    // butPa1.cpp WritePort :156-180 (hex,
                                                           // tail-anchored -- same shape as
                                                           // MyLedCore::SetPort, see LedCore.cpp)
    void SetBit(const AnsiString& s);                     // butPa1.cpp WriteBit :182-185
    void SetType(const AnsiString& s);                    // butPa1.cpp WriteType :187-190
    void SetAlias(const AnsiString& s);                   // butPa1.cpp WriteAlias :192-195
    AnsiString GetAlias() const;                          // butPa1.cpp ReadAlias :197-200

private:
    AnsiString sAlias;
};

// ===========================================================================
//  BtnPanelLaneCore -- golden TBtnPanelLane (BtnPanelLane.h/.cpp).
//  701 .dfm instances -- 2nd-highest-count custom control in the codebase.
//
//  GOLDEN BUG (BtnPanelLane.h:15-19,58-68 + BtnPanelLane.cpp:20-59/132-155,
//  verified directly): same shape as BtnPanelCore's bug above --
//  `Lane`/`IP`/`Port`/`Bit`/`Type` are declared as `__property AnsiString
//  ...={read=sOutLane/sOutIP/sOutPort/sOutBit/sOutType, write=SetRing/
//  SetIP/WritePort/WriteBit/WriteType}`, but none of those 5 setters ever
//  assign their matching backing field -- all 5 are set to "" once in the
//  ctor (BtnPanelLane.cpp:45-50) and never again, so golden's `->Lane`/
//  `->IP`/`->Port`/`->Bit`/`->Type` reads always yield "". IMPORTANT
//  ASYMMETRY, verified directly rather than assumed from the pattern
//  above: `Alias` (sAlias, via WriteAlias/ReadAlias) and `IsISA` (sISA,
//  via WriteISA) are BOTH genuinely, correctly assigned in golden --
//  WriteISA is `sISA=S; ISABase=atoi(S.c_str());` (BtnPanelLane.cpp:162-
//  166), so IsISA is NOT part of this bug, unlike the near-identical-
//  looking Lane/IP/Port/Bit/Type quintet. Same design choice as
//  BtnPanelCore: no GetRing()/GetIP()/GetPort()/GetBit()/GetType() string
//  getter is provided here, for the identical reason given there.
// ===========================================================================
class BtnPanelLaneCore : public BtnPanelCoreBase {
public:
    BtnPanelLaneCore();                                    // BtnPanelLane.cpp:20-59

    int OutRing, OutIP, OutPort, OutBit, OutType, ISABase; // BtnPanelLane.h public block

    void SetRing(const AnsiString& value);                // BtnPanelLane.cpp SetRing :132-135
                                                           // (plain atoi -- NOT the fixed-buffer
                                                           // MyLedLaneCore::SetRing shape; verified
                                                           // directly, not assumed by symmetry)
    void SetIP(const AnsiString& value);                  // BtnPanelLane.cpp SetIP :137-140 (plain atoi)
    void SetPort(const AnsiString& s);                    // BtnPanelLane.cpp WritePort :142-145
                                                           // (plain atoi -- NOT butPa1's hex-tail-scan)
    void SetBit(const AnsiString& s);                     // BtnPanelLane.cpp WriteBit :147-150 (plain atoi)
    void SetType(const AnsiString& s);                    // BtnPanelLane.cpp WriteType :152-155 (plain atoi)
    void SetAlias(const AnsiString& s);                   // BtnPanelLane.cpp WriteAlias :157-160
    AnsiString GetAlias() const;                          // BtnPanelLane.cpp ReadAlias :168-171
    void SetISA(const AnsiString& s);                     // BtnPanelLane.cpp WriteISA :162-166 (plain atoi)

private:
    AnsiString sAlias;
};

} // namespace vclcompat
#endif // VCLCOMPAT_BTNPANELCORE_H
