// ===========================================================================
//  Public/ExternFunction.h  -- W1 portable translation
//  BCB6 golden source: HT9011UC_Code_V3.33.906.0_20260618/Public/ExternFunction.h
//
//  Only the four pure-logic leaf functions are exposed here (W1 scope).
//  VCL-form-coupled functions (StringGrid_Insert_Row, StringGrid_Delete_Row,
//  StatusBar_ItemText, ShowRecordTime) are deferred to the UI wave (W7).
//  DeleteDirectory is deferred: it depends on BCB6 SysUtils FindFirst/FindNext
//  which require a fuller SysUtils shim (out of W0/W1 scope).
// ===========================================================================
#ifndef PUBLIC_EXTERN_FUNCTION_H
#define PUBLIC_EXTERN_FUNCTION_H

#include "vclcompat/vcl_compat.h"

// ---------------------------------------------------------------------------
//  Math_AdjustAngle
//  BCB6 source: ExternFunction.cpp:126-151
//
//  Normalises fAngle into the range:
//    bSign == false  ->  [0, 360)   (add 360 until >= 0, subtract 360 until < 360)
//    bSign == true   ->  (-360, 0]  (add 360 only while angle <= -360; negative
//                                    angles in (-360,0] are left untouched; positive
//                                    angles are left untouched too)
//
//  Known I/O:
//    Math_AdjustAngle(-450, true)  == -90
//    Math_AdjustAngle(-450, false) == 270
//    Math_AdjustAngle( 720, false) ==   0
//    Math_AdjustAngle( 370, false) ==  10
// ---------------------------------------------------------------------------
double Math_AdjustAngle(double fAngle, bool bSign = true);

// ---------------------------------------------------------------------------
//  String_JustNumber
//  BCB6 source: ExternFunction.cpp:153-185
//
//  Strips any leading spaces then keeps the leading numeric prefix:
//    - Allows one leading '+' or '-' (position 1 only).
//    - Allows one '.'; inserts '0' before a leading dot or a dot immediately
//      after a sign character.
//    - Stops at the first character that is not a digit, dot, or the one sign.
//
//  Known I/O:
//    String_JustNumber("  -12.3abc") == "-12.3"
//    String_JustNumber(".5")         == "0.5"
//    String_JustNumber("+.5")        == "+0.5"
//    String_JustNumber("1.2.3")      == "1.2"
//    String_JustNumber("abc")        == ""
// ---------------------------------------------------------------------------
AnsiString String_JustNumber(AnsiString sStr);

// ---------------------------------------------------------------------------
//  String_EraseLSpace / String_EraseRSpace
//  BCB6 source: ExternFunction.cpp:187-203
//
//  In-place strip of leading / trailing space characters.
// ---------------------------------------------------------------------------
void String_EraseLSpace(AnsiString& sStr);
void String_EraseRSpace(AnsiString& sStr);

// ---------------------------------------------------------------------------
//  RecordTime
//  BCB6 source: ExternFunction.cpp:205-220
//
//  Simple clock()-based stopwatch with internal static state.
//    bIsStart == true  : resets the internal start time; returns 0.
//    bIsStart == false : returns (clock_t)(now - start) in clock ticks.
//
//  Note: return unit is clock ticks (CLOCKS_PER_SEC ticks per second), same
//  as BCB6. Callers that need milliseconds must divide by CLOCKS_PER_SEC/1000.
// ---------------------------------------------------------------------------
int RecordTime(bool bIsStart);

#endif // PUBLIC_EXTERN_FUNCTION_H
