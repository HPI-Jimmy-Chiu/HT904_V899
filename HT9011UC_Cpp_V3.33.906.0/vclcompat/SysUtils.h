// ===========================================================================
//  vclcompat/SysUtils.h
//  BCB6 SysUtils free-function compatibility shim (used subset).
//
//  Re-creates the SysUtils-style free functions the V906 source calls:
//  IntToStr/StrToInt(/Def)/StrToFloat(/Def)/FloatToStr/FormatFloat/Format,
//  FileExists/DirectoryExists/CreateDir/ForceDirectories/DeleteFile/CopyFile,
//  ExtractFilePath/Name/Ext, ChangeFileExt, IncludeTrailingBackslash,
//  Now/Date/Time/FormatDateTime/Decode*/Encode*/Str<->DateTime, Sleep, Trim,
//  UpperCase/LowerCase (free-fn forms).
//
//  Numeric/date semantics mirror BCB6 (see AnsiString / TDateTime headers).
// ===========================================================================
#ifndef VCLCOMPAT_SYSUTILS_H
#define VCLCOMPAT_SYSUTILS_H

#include "vclcompat/AnsiString.h"
#include "vclcompat/TDateTime.h"

namespace vclcompat {

// ---- numbers <-> strings --------------------------------------------------
AnsiString IntToStr(int v);
int        StrToInt(const AnsiString& s);              // throws on bad input
int        StrToIntDef(const AnsiString& s, int def);
double     StrToFloat(const AnsiString& s);            // throws on bad input
double     StrToFloatDef(const AnsiString& s, double def);
AnsiString FloatToStr(double v);                       // FloatToStr (15 sig digits)
AnsiString FormatFloat(const AnsiString& fmt, double v);

// FloatToStrF format selector (only the values used by the source).
enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
AnsiString FloatToStrF(double v, TFloatFormat fmt, int precision, int digits);

// Format(fmt, args...) : printf-style. The BCB6 source wraps args with the
// ARRAYOFCONST((...)) macro; that macro (below) reduces to the bare arg list,
// so Format behaves like a free-function sprintf.
template <typename... Args>
AnsiString Format(const AnsiString& fmt, Args... args) {
    AnsiString r;
    r.sprintf(fmt.c_str(), args...);
    return r;
}
inline AnsiString Format(const AnsiString& fmt) { return fmt; }

// ---- string helpers (free-fn forms) ---------------------------------------
inline AnsiString Trim(const AnsiString& s)      { return s.Trim(); }
inline AnsiString UpperCase(const AnsiString& s) { return s.UpperCase(); }
inline AnsiString LowerCase(const AnsiString& s) { return s.LowerCase(); }

// ---- filesystem -----------------------------------------------------------
bool FileExists(const AnsiString& path);
bool DirectoryExists(const AnsiString& path);
bool CreateDir(const AnsiString& path);              // single level
bool ForceDirectories(const AnsiString& path);       // recursive mkdir -p
bool DeleteFile(const AnsiString& path);
bool CopyFile(const AnsiString& src, const AnsiString& dst, bool failIfExists = false);

AnsiString ExtractFilePath(const AnsiString& path);  // incl. trailing sep, "" if none
AnsiString ExtractFileName(const AnsiString& path);  // last component
AnsiString ExtractFileExt(const AnsiString& path);   // incl. leading '.', "" if none
AnsiString ChangeFileExt(const AnsiString& path, const AnsiString& newExt);
AnsiString IncludeTrailingBackslash(const AnsiString& path);

// ---- date/time (re-exported; defined in TDateTime) ------------------------
// Now()/Date()/Time()/FormatDateTime()/Decode*/Encode*/Str<->DateTime are
// declared in TDateTime.h and brought into scope by including this header.

// ---- misc -----------------------------------------------------------------
void Sleep(int milliseconds);   // Win32-style Sleep (portable wrapper)

} // namespace vclcompat

// BCB6 ARRAYOFCONST((a, b, ...)) builds a TVarRec open array. In the compat
// layer Format is a variadic template, so reduce the macro to its inner
// argument list:  ARRAYOFCONST((x)) -> (x).
#ifndef ARRAYOFCONST
#define ARRAYOFCONST(x) x
#endif

#endif // VCLCOMPAT_SYSUTILS_H
