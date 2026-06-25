// ===========================================================================
//  Public/ExternFunction.cpp  -- W1 portable translation
//  BCB6 golden source: HT9011UC_Code_V3.33.906.0_20260618/Public/ExternFunction.cpp
//
//  Translated: W1 batch, pure-logic leaf functions only.
//  Deferred (VCL-form-coupled or requires fuller SysUtils shim):
//    StringGrid_Insert_Row   -> W7 (UI wave)
//    StringGrid_Delete_Row   -> W7 (UI wave)
//    StatusBar_ItemText      -> W7 (UI wave)
//    ShowRecordTime          -> W7 (UI wave, depends on TEdit)
//    DeleteDirectory         -> deferred (needs FindFirst/FindNext SysUtils shim)
// ===========================================================================

// BCB6 had:  #include "MachineDefine.h"  / #pragma hdrstop
// Replaced with the vclcompat umbrella header.
#include "vclcompat/vcl_compat.h"

#include "Public/ExternFunction.h"

#include <ctime>    // clock(), clock_t

// ---------------------------------------------------------------------------
//  Math_AdjustAngle
//  BCB6 source: ExternFunction.cpp:126-151
//
//  Two-branch normalisation:
//    bSign == true  : angle is already negative (or zero); only bring it into
//                     (-360, 0] by adding 360 while <= -360.  Positive angles
//                     (and those already in (-360,0]) pass through unchanged.
//    bSign == false : wrap into [0, 360) by adding 360 while < 0, then
//                     subtracting 360 while >= 360.
// ---------------------------------------------------------------------------
double Math_AdjustAngle(double fAngle, bool bSign)
{
    double adj_angle = fAngle;

    // BCB6 ExternFunction.cpp:130-135
    if (bSign == true && adj_angle < 0)
    {
        while (adj_angle <= -360)
        {
            adj_angle += 360;
        }
    }
    else  // BCB6 ExternFunction.cpp:138-147
    {
        while (adj_angle < 0)
        {
            adj_angle += 360;
        }

        while (adj_angle >= 360)
        {
            adj_angle -= 360;
        }
    }

    return adj_angle;  // BCB6 ExternFunction.cpp:150
}

// ---------------------------------------------------------------------------
//  String_JustNumber
//  BCB6 source: ExternFunction.cpp:153-185
//
//  1-based loop over sStr (after stripping leading spaces).
//  Accumulates AdjustStr while characters are numeric / dot / sign.
//  Inserts '0' before a leading dot, or before a dot that immediately follows
//  a sign character, to canonicalise ".5" -> "0.5" and "+.5" -> "+0.5".
// ---------------------------------------------------------------------------
AnsiString String_JustNumber(AnsiString sStr)
{
    AnsiString AdjustStr("");                  // BCB6 ExternFunction.cpp:155
    String_EraseLSpace(sStr);                  // BCB6 ExternFunction.cpp:156
    int length = sStr.Length();                // BCB6 ExternFunction.cpp:157
    bool first_dot = false;                    // BCB6 ExternFunction.cpp:158

    for (int i = 1; i <= length; i++)          // BCB6 ExternFunction.cpp:159  (1-based)
    {
        bool is_num  = (sStr[i] >= '0' && sStr[i] <= '9');                    // :161
        bool is_dot  = (first_dot == false && sStr[i] == '.');                 // :162
        bool is_sign = (i == 1 && (sStr[i] == '-' || sStr[i] == '+'));         // :163

        if (is_num == false && is_dot == false && is_sign == false)            // :165-168
        {
            break;
        }

        if (is_dot == true)                                                    // :170
        {
            if (i == 1)                                                        // :172-174
            {
                AdjustStr = "0";
            }
            else if (AdjustStr[AdjustStr.Length()] == '-' ||                   // :175-178
                     AdjustStr[AdjustStr.Length()] == '+')
            {
                AdjustStr += '0';
            }
            first_dot = true;                                                  // :180
        }
        AdjustStr += sStr[i];                                                  // :182
    }
    return AdjustStr;                                                          // :184
}

// ---------------------------------------------------------------------------
//  String_EraseLSpace
//  BCB6 source: ExternFunction.cpp:187-193
//
//  Removes all leading space characters from sStr in place (1-based Delete).
// ---------------------------------------------------------------------------
void String_EraseLSpace(AnsiString& sStr)
{
    while (sStr.Length() >= 1 && sStr[1] == ' ')    // BCB6 :189
    {
        sStr.Delete(1, 1);                           // BCB6 :191 -- 1-based Delete
    }
}

// ---------------------------------------------------------------------------
//  String_EraseRSpace
//  BCB6 source: ExternFunction.cpp:195-203
//
//  Removes all trailing space characters from sStr in place.
//  BCB6 re-reads Length() after each Delete; faithfully preserved.
// ---------------------------------------------------------------------------
void String_EraseRSpace(AnsiString& sStr)
{
    int length = sStr.Length();                              // BCB6 :197
    while (length >= 1 && sStr[length] == ' ')              // BCB6 :198
    {
        sStr.Delete(length, 1);                              // BCB6 :200 -- 1-based Delete
        length = sStr.Length();                              // BCB6 :201
    }
}

// ---------------------------------------------------------------------------
//  RecordTime
//  BCB6 source: ExternFunction.cpp:205-220
//
//  Static-local stopwatch.  Returns 0 on start; returns elapsed clock ticks
//  on read (bIsStart == false).  The return type is int, matching BCB6.
//  Overflow at ~2^31 clock ticks is the same risk as in BCB6.
// ---------------------------------------------------------------------------
int RecordTime(bool bIsStart)
{
    static clock_t tStartTime = clock();    // BCB6 :207 -- static local, same semantics

    if (bIsStart == true)                   // BCB6 :209-213
    {
        tStartTime = clock();
        return 0;
    }
    else                                    // BCB6 :214-218
    {
        clock_t tEndTime = clock();
        return static_cast<int>(tEndTime - tStartTime);
    }
}
