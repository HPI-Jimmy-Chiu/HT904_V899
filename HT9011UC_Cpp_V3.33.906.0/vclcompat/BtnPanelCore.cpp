// ===========================================================================
//  vclcompat/BtnPanelCore.cpp
//  See BtnPanelCore.h for scope/provenance. Every method banner cites the
//  exact golden D:\HT9045\elec\myvcl\{butPa1,BtnPanelLane}.cpp line range.
// ===========================================================================
#include "vclcompat/BtnPanelCore.h"
#include <cstdlib>

namespace vclcompat {

// ===========================================================================
//  BtnPanelCoreBase
// ===========================================================================

BtnPanelCoreBase::BtnPanelCoreBase()                    // butPa1.cpp:18-51 / BtnPanelLane.cpp:20-59
{
    // GOLDEN BUG (see header): the real ctor declares LOCAL shadows of the
    // same name (`TColor tcTrueColor=clBtnFace; TColor tcFalseColor=clBtnFace;`)
    // so the protected members below are genuinely never assigned here.
    // Reproduced with a deterministic sentinel instead of real uninitialised
    // memory -- see the kUninitializedColorSentinel note in the header.
    tcTrueColor  = kUninitializedColorSentinel;
    tcFalseColor = kUninitializedColorSentinel;
    tcTrueFontColor  = kUninitializedColorSentinel;   // golden: never touched at all in the ctor,
    tcFalseFontColor = kUninitializedColorSentinel;   // not even via a shadowing local

    bDown = false;                                    // butPa1.cpp:23 / BtnPanelLane.cpp:25
    FStyle = tsButtons;                               // butPa1.cpp:24 / BtnPanelLane.cpp:26

    // butPa1.cpp:25-44 / BtnPanelLane.cpp:27-46 -- `if(bDown) {...} else {...}`. bDown is
    // unconditionally false at this point in both golden ctors (just set above, nothing in
    // between can change it), so the `if(bDown)` branch is dead code here; only the `else`
    // branch below is reachable, reproduced faithfully:
    TColor localTrueColorShadow = clBtnFace;          // the actual local shadow golden reads from
    TColor localFalseColorShadow = clBtnFace;         // (kept local here too, matching golden's
                                                       // own scoping -- these are NOT stored into
                                                       // tcTrueColor/tcFalseColor, per the bug above)
    (void)localTrueColorShadow;                       // golden's shadow is read only in the
                                                       // (dead, bDown==false) `if` branch
    Color = localFalseColorShadow;                    // butPa1.cpp:33 / BtnPanelLane.cpp:35
    if (FStyle == tsFlatButtons) {
        BevelInner = bvNone;
        BevelOuter = bvNone;
    } else {
        BevelInner = bvRaised;                        // butPa1.cpp:41-42 / BtnPanelLane.cpp:43-44
        BevelOuter = bvRaised;
    }
    // FontColor: never assigned anywhere in either golden ctor (no code path touches
    // Font->Color here) -- left at whatever a real inherited TPanel.Font.Color default would be,
    // which is outside this class's own state; modelled here as "not yet set by this class",
    // using the same sentinel for consistency (a real TPanel/CStatic shell applies its own
    // inherited default before Down is ever toggled the first time).
    FontColor = kUninitializedColorSentinel;
}

void BtnPanelCoreBase::SetTrueColor(TColor v)           // butPa1.cpp:53-58 / BtnPanelLane.cpp:70-75
{
    tcTrueColor = v;
    if (bDown) Color = v;
}

void BtnPanelCoreBase::SetFalseColor(TColor v)          // butPa1.cpp:60-65 / BtnPanelLane.cpp:77-82
{
    tcFalseColor = v;
    if (!bDown) Color = v;
}

void BtnPanelCoreBase::SetTrueFontColor(TColor v)       // butPa1.cpp:67-72 / BtnPanelLane.cpp:84-89
{
    tcTrueFontColor = v;
    if (bDown) FontColor = v;
}

void BtnPanelCoreBase::SetFalseFontColor(TColor v)      // butPa1.cpp:74-79 / BtnPanelLane.cpp:91-96
{
    tcFalseFontColor = v;
    if (!bDown) FontColor = v;
}

void BtnPanelCoreBase::SetDown(bool v)                  // SetPanelStatus, butPa1.cpp:81-106 /
                                                         // BtnPanelLane.cpp:98-123 (identical)
{
    bDown = v;
    if (bDown) {
        Color = tcTrueColor;
        FontColor = tcTrueFontColor;
        BevelInner = bvLowered;
        BevelOuter = bvLowered;
    } else {
        Color = tcFalseColor;
        FontColor = tcFalseFontColor;
        if (FStyle == tsFlatButtons) {
            BevelInner = bvNone;
            BevelOuter = bvNone;
        } else {
            BevelInner = bvRaised;
            BevelOuter = bvRaised;
        }
    }
}

void BtnPanelCoreBase::SetStyle(TTabStyleShim v)        // WriteStyle, butPa1.cpp:202-206 /
                                                         // BtnPanelLane.cpp:173-177
{
    FStyle = v;
    SetDown(bDown);   // golden: "SetPanelStatus(bDown);" -- re-applies Color/BevelInner/BevelOuter
                      // under the new style with the CURRENT Down state unchanged.
}

// ===========================================================================
//  BtnPanelCore -- golden TBtnPanel
// ===========================================================================

BtnPanelCore::BtnPanelCore()                            // butPa1.cpp:18-51 (ctor tail)
    : BtnPanelCoreBase(), OutPort(0), OutBit(0), OutType(0)
{
    // AI(W906-W7-C1) 20260728: sOutPort/sOutBit/sOutType (golden's Port/Bit/Type property
    // backing fields, butPa1.cpp:45-47) are not modelled here. The PREVIOUS version of this
    // comment said this was safe because "nothing outside WritePort/WriteBit/WriteType ever
    // reads them" -- true, but the wrong reason: the actual reason nothing reads them
    // usefully is that golden's own WritePort/WriteBit/WriteType (butPa1.cpp:156-190) never
    // ASSIGN sOutPort/sOutBit/sOutType in the first place (only the ctor's one-time ""
    // does) -- see the GOLDEN BUG banner on this class in BtnPanelCore.h. So it's not that
    // golden merely chooses not to read them; golden's own ->Port/->Bit/->Type getters would
    // be unable to return anything useful even if something DID call them. OutPort/OutBit/
    // OutType (the actually-parsed ints) remain the complete observable surface either way.
}

// Golden's CutSpaceAtHead/CutSpaceAtTail (butPa1.cpp:115-154) strip ONLY literal ' ' (0x20) --
// narrower than AnsiString::Trim()'s BCB6-faithful "strip anything <= ' '" rule. See
// LedCore.cpp's identical helper/banner (MyLedCore::SetPort runs the same algorithm) for why
// this narrower, golden-exact rule is used instead of the broader Trim().
static AnsiString TrimAsciiSpaceOnly(const AnsiString& s)
{
    int len = s.Length();
    int b = 1, e = len;
    while (b <= e && s[b] == ' ') ++b;
    while (e >= b && s[e] == ' ') --e;
    if (b > e) return AnsiString("");
    return s.SubString(b, e - b + 1);
}

void BtnPanelCore::SetPort(const AnsiString& s)         // butPa1.cpp WritePort :156-180
{
    // Identical hex-tail-scan algorithm to MyLedCore::SetPort (LedCore.cpp) -- verified
    // line-by-line against butPa1.cpp, not assumed from that similarity. Reimplemented via
    // AnsiString ops rather than golden's char[256] buffer; see MyLedCore::SetPort's banner
    // for why this is not a UB-preservation case (256 bytes is never exercised by any real
    // .dfm Port string).
    AnsiString trimmed = TrimAsciiSpaceOnly(s);
    AnsiString upper = trimmed.UpperCase();
    int len = upper.Length();
    int outPort = 0, scale = 1;
    for (int i = len; i >= 1; --i) {
        char c = upper[i];
        int j;
        if (c >= '0' && c <= '9') j = c - '0';
        else if (c >= 'A' && c <= 'F') j = 10 + (c - 'A');
        else break;
        outPort += j * scale;
        scale *= 16;
    }
    OutPort = outPort;
}

void BtnPanelCore::SetBit(const AnsiString& s)          // butPa1.cpp WriteBit :182-185
{
    OutBit = std::atoi(s.c_str());
}

void BtnPanelCore::SetType(const AnsiString& s)         // butPa1.cpp WriteType :187-190
{
    OutType = std::atoi(s.c_str());
}

void BtnPanelCore::SetAlias(const AnsiString& s)        // butPa1.cpp WriteAlias :192-195
{
    sAlias = s;
}

AnsiString BtnPanelCore::GetAlias() const               // butPa1.cpp ReadAlias :197-200
{
    return sAlias;
}

// ===========================================================================
//  BtnPanelLaneCore -- golden TBtnPanelLane
// ===========================================================================

BtnPanelLaneCore::BtnPanelLaneCore()                    // BtnPanelLane.cpp:20-59
    : BtnPanelCoreBase(), OutRing(0), OutIP(0), OutPort(0), OutBit(0), OutType(0), ISABase(0)
{
    // sOutLane/sOutIP/sOutPort/sOutBit/sOutType (golden's Lane/IP/Port/Bit/Type property
    // backing fields, cleared to "" at BtnPanelLane.cpp:47-51, with sISA at :52 --
    // AI(W906-W7-C-followup) 20260728: previously cited as :45-49, which lands on the
    // BevelOuter block above; re-read and corrected) are not modelled here, for the same reason as
    // BtnPanelCore above: golden's own SetRing/SetIP/WritePort/WriteBit/WriteType (:132-155)
    // never assign them (see the GOLDEN BUG banner on this class in BtnPanelCore.h), so their
    // getters would always read "" in golden regardless. OutRing/OutIP/OutPort/OutBit/OutType
    // remain the complete observable surface. (sISA, by contrast, IS genuinely modelled --
    // see SetISA below -- because golden's own WriteISA genuinely assigns it too.)
}

void BtnPanelLaneCore::SetRing(const AnsiString& value) // BtnPanelLane.cpp SetRing :132-135
{
    OutRing = std::atoi(value.c_str());   // plain atoi -- NOT MyLedLaneCore::SetRing's fixed-buffer
                                          // shape; verified directly against BtnPanelLane.cpp
}

void BtnPanelLaneCore::SetIP(const AnsiString& value)   // BtnPanelLane.cpp SetIP :137-140
{
    OutIP = std::atoi(value.c_str());
}

void BtnPanelLaneCore::SetPort(const AnsiString& s)     // BtnPanelLane.cpp WritePort :142-145
{
    OutPort = std::atoi(s.c_str());       // plain atoi -- NOT BtnPanelCore::SetPort's hex-tail-scan
}

void BtnPanelLaneCore::SetBit(const AnsiString& s)      // BtnPanelLane.cpp WriteBit :147-150
{
    OutBit = std::atoi(s.c_str());
}

void BtnPanelLaneCore::SetType(const AnsiString& s)     // BtnPanelLane.cpp WriteType :152-155
{
    OutType = std::atoi(s.c_str());
}

void BtnPanelLaneCore::SetAlias(const AnsiString& s)    // BtnPanelLane.cpp WriteAlias :157-160
{
    sAlias = s;
}

AnsiString BtnPanelLaneCore::GetAlias() const           // BtnPanelLane.cpp ReadAlias :168-171
{
    return sAlias;
}

void BtnPanelLaneCore::SetISA(const AnsiString& s)      // BtnPanelLane.cpp WriteISA :162-166
{
    ISABase = std::atoi(s.c_str());
}

} // namespace vclcompat
