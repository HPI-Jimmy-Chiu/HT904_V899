// ===========================================================================
//  Public/ExternFunction.h  -- W1 portable translation
//  BCB6 golden source: HT9011UC_Code_V3.33.906.0_20260618/Public/ExternFunction.h
//
//  Only the four pure-logic leaf functions are exposed here (W1 scope).
//  VCL-form-coupled functions (StringGrid_Insert_Row, StringGrid_Delete_Row,
//  StatusBar_ItemText, ShowRecordTime) are deferred to the UI wave (W7).
//  DeleteDirectory -- AI(W906-uHGemClass-Micro6) 20260721: the W1-era
//  "deferred, needs FindFirst/FindNext SysUtils shim" note below was stale by
//  this wave: vclcompat/SysUtils.h now carries FindFirst/FindNext/FindClose/
//  TSearchRec/RemoveDir/DeleteFile/DirectoryExists (added by a later wave than
//  when that note was written), so it is translated for real here (needed by
//  SECSGEM/uHGemClass.cpp's S7F18_DeleteProcessProgramAcknowledge).
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

// ---------------------------------------------------------------------------
//  DeleteDirectory
//  BCB6 source: ExternFunction.cpp:249-280 ("2014.06.12, Joye, ASE-SG")
//
//  Recursively deletes sDir and everything under it.
//    - If sDir does not exist, returns true immediately (deleting a
//      nonexistent directory is treated as success, not an error).
//    - Otherwise walks sDir via FindFirst/FindNext (faAnyFile mask):
//      plain files are DeleteFile()'d; subdirectories (Attr & faDirectory,
//      excluding "." and "..") are recursed into via DeleteDirectory() itself.
//    - Return values of the individual DeleteFile() calls are NOT checked
//      (golden ignores them).
//    - If a recursive call on a subdirectory returns false, the enumeration
//      loop `break`s early (remaining siblings at that level are left
//      untouched) but FindClose() + the final RemoveDir(sDir) still run --
//      RemoveDir on a non-empty directory then typically fails, so that
//      failure propagates up as this call's own return value. Preserved
//      verbatim, not hardened into an early `return false`.
//    - Final return is RemoveDir(sDir) -- true only if the directory (now
//      believed empty) was actually removed.
// ---------------------------------------------------------------------------
bool DeleteDirectory(AnsiString sDir);

#endif // PUBLIC_EXTERN_FUNCTION_H
