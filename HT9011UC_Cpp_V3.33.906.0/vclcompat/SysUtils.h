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
// HexStrToInt : project-local hex parser (golden ref EJ1N/TextProcess.cpp:347
//   int HexStrToInt(AnsiString)).  Net BCB6 semantics, faithfully reproduced:
//   if the string has no "0x" prefix, one is prepended, then StrToIntDef(s,-1)
//   parses it as hex; an unparseable value yields -1.  Used by the CSV config
//   readers (database.cpp SetIOTableNo / cinitial.cpp) for the IO_Table
//   Port / OnPort / ISABase hex columns.  A bare "$"-prefixed BCB6 hex literal
//   is also accepted (BCB6 StrToInt understands '$').
int        HexStrToInt(const AnsiString& s);
// BCB6 IntToHex(Value, Digits): uppercase hex, zero-padded to >= Digits width.
// Overloaded for the integer widths the V906 source passes (int / __int64).
AnsiString IntToHex(long long value, int digits);
AnsiString IntToHex(int value, int digits);
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

// ---- directory removal / file attributes (BCB6 SysUtils) ------------------
// Companions of the FindFirst/FindNext family below: ExternFunction::
// DeleteDirectory (golden ref Public/ExternFunction.cpp:279) ends with
// RemoveDir(sDir), and csystem.cpp Del_Tree (csystem.cpp:23101) calls
// FileSetAttr(...,faArchive) before DeleteFile.  Added here so the recursive
// directory-removal callers translate without further shim work.
bool RemoveDir(const AnsiString& path);                 // rmdir; true on success
// FileSetAttr/FileGetAttr operate on the Win32 file-attribute bitmask (the same
// fa* bits below).  FileSetAttr returns 0 on success (BCB6 semantics);
// FileGetAttr returns the attribute mask, or -1 on failure.
int  FileSetAttr(const AnsiString& path, int attr);
int  FileGetAttr(const AnsiString& path);

// ===========================================================================
//  FILE-SYSTEM SEARCH  (BCB6 SysUtils FindFirst/FindNext/FindClose)
//
//  Faithful re-creation of the BCB6 directory-enumeration API used by the
//  golden ref (Public/ExternFunction.cpp DeleteDirectory, csystem.cpp Del_Tree,
//  cDataHandling.cpp, cBuilder.cpp, HS_Function.cpp, main.cpp, ...).
//
//  BCB6 semantics replicated EXACTLY:
//    * FindFirst returns 0 on success, non-zero (a Win32 error code) on no
//      match / error.  Callers test `FindFirst(...)==0`.
//    * FindNext returns 0 while more entries exist, non-zero when exhausted.
//      Callers loop `do {...} while (FindNext(sr)==0);`.
//    * FindClose releases the search handle (callers always pair it).
//    * The `attr` argument is a FILTER: entries whose attributes are a subset
//      of (faAnyFile-masked) `attr` are returned.  faAnyFile (0x3F) matches
//      everything (incl. directories); the BCB6 filter rule is
//          (Attr & not_in_filter) == 0  ->  entry kept,
//      where the "always-returned" volatile bits (Archive/ReadOnly) are not
//      filtered against.  We reproduce BCB6's actual VCL FindMatchingFile rule.
//    * sr.Attr is the entry's attribute mask AND-ed to the fa* bits.
//    * sr.Name is the bare file name (no path), as AnsiString.
//    * sr.Size is the file size (__int64; BCB6 uses Integer historically but
//      modern BCB6 RTL exposes __int64 -- callers here only use Name/Attr).
//    * sr.Time is the DOS-packed last-write time (Integer), via FileTime->DOS.
// ===========================================================================

// BCB6 file-attribute constants (SysUtils.hpp values).
const int faReadOnly  = 0x00000001;
const int faHidden    = 0x00000002;
const int faSysFile   = 0x00000004;
const int faVolumeID  = 0x00000008;
const int faDirectory = 0x00000010;
const int faArchive   = 0x00000020;
const int faAnyFile   = 0x0000003F;   // ReadOnly|Hidden|SysFile|VolumeID|Directory|Archive

// BCB6 TSearchRec.  Field names/types match the BCB6 record so callers compile
// unchanged (sr.Attr, sr.Name, sr.Size, sr.Time).  `Handle`/`FindData` are the
// opaque Win32 search state, kept as void* so this header need not pull
// windows.h (the .cpp owns the WIN32_FIND_DATA).
struct TSearchRec {
    int           Time;       // DOS-packed last-write time
    long long     Size;       // file size in bytes (BCB6 __int64)
    int           Attr;       // attribute mask (fa* bits)
    AnsiString    Name;       // bare file name
    // ---- opaque Win32 search state (do not touch from translated code) ----
    int           ExcludeAttr;// the FindFirst filter (BCB6 keeps it in the rec)
    void*         FindHandle; // HANDLE from FindFirstFile (INVALID == none)
    void*         FindData;   // heap WIN32_FIND_DATAA carrying the current entry

    TSearchRec()
        : Time(0), Size(0), Attr(0), ExcludeAttr(0),
          FindHandle(0), FindData(0) {}
    ~TSearchRec();                 // frees FindData / closes a dangling handle
private:
    TSearchRec(const TSearchRec&);            // non-copyable (owns Win32 state)
    TSearchRec& operator=(const TSearchRec&);
};

int  FindFirst(const AnsiString& path, int attr, TSearchRec& sr); // 0 == match
int  FindNext(TSearchRec& sr);                                    // 0 == match
void FindClose(TSearchRec& sr);

// ---- date/time (re-exported; defined in TDateTime) ------------------------
// Now()/Date()/Time()/FormatDateTime()/Decode*/Encode*/Str<->DateTime are
// declared in TDateTime.h and brought into scope by including this header.

// ---- misc -----------------------------------------------------------------
void Sleep(int milliseconds);   // Win32-style Sleep (portable wrapper)

// TryStrToFloat (BCB6 SysUtils): attempt locale-independent '.' decimal parse.
// Sets `value` and returns true on success; leaves `value` unchanged and
// returns false on failure (empty, non-numeric, etc.).
// Used by common.cpp WriteIniData(AnsiString) and WriteIniData1 at lines
// 1053-1054, 1200-1201 to decide whether to compare strings as floats.
bool TryStrToFloat(const char* s, double& value);

} // namespace vclcompat

// BCB6 ARRAYOFCONST((a, b, ...)) builds a TVarRec open array. In the compat
// layer Format is a variadic template, so reduce the macro to its inner
// argument list:  ARRAYOFCONST((x)) -> (x).
#ifndef ARRAYOFCONST
#define ARRAYOFCONST(x) x
#endif

#endif // VCLCOMPAT_SYSUTILS_H
