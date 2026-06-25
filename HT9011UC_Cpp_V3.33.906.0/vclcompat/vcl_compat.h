// ===========================================================================
//  vclcompat/vcl_compat.h  -- UMBRELLA HEADER
//
//  Translated BCB6 source files #include THIS in place of <vcl.h>:
//      #include "vclcompat/vcl_compat.h"   // was: #include <vcl.h>
//
//  It pulls in the AnsiString / TStringList / TDateTime / SysUtils shims and
//  neutralizes the Borland-specific keywords/macros so the bulk of the code
//  compiles under standard C++17 (MinGW g++ / MSVC) with minimal edits.
//
//  The compat types live in namespace vclcompat; this header brings them into
//  the global namespace (via `using`) so translated code keeps the original
//  unqualified spellings (AnsiString, TStringList, Now(), IntToStr(), ...).
//  Define VCLCOMPAT_NO_GLOBAL_USING before including to keep them namespaced.
// ===========================================================================
#ifndef VCLCOMPAT_VCL_COMPAT_H
#define VCLCOMPAT_VCL_COMPAT_H

// ---------------------------------------------------------------------------
//  Borland keyword / macro neutralization
//  (Define BEFORE including anything that might use them.)
// ---------------------------------------------------------------------------

// Calling-convention + linkage keywords -> nothing (standard convention).
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
// keep __cdecl meaningful on MSVC; only neutralize where unknown (MinGW knows it)
#endif

// VCL closure (method pointer) modifier -> neutralize. Where a __closure typedef
// is needed, translated code should use a std::function-style typedef instead.
#ifndef __closure
#define __closure
#endif

// VCL property syntax. In pure-logic (.cpp) translation units there are no
// __property declarations (they live in form .h paired with .dfm, which is UI
// and out of W0 scope). We map the keyword to a no-op token-eater note: any
// surviving `__property ...;` line should be hand-translated to a getter/setter.
// Mapping to `public:` would mis-parse the declaration, so we DO NOT define
// __property here; form headers are handled in the W7 UI wave. (Documented.)

// Access specifier used in VCL form classes.
#ifndef __published
#define __published public
#endif

// VCL package/export markers -> empty.
#ifndef PACKAGE
#define PACKAGE
#endif
#ifndef DELPHICLASS
#define DELPHICLASS
#endif
#ifndef DELPHIRETURN
#define DELPHIRETURN
#endif

// __declspec(dllimport/dllexport) on driver headers -> empty for the
// non-exported portable build. (MSVC understands __declspec natively; this
// only fires where the compiler doesn't, i.e. it's a safety net.)
#if !defined(_MSC_VER) && !defined(__declspec)
#define __declspec(x)
#endif

// ---------------------------------------------------------------------------
//  Compat types & functions
// ---------------------------------------------------------------------------
#include "vclcompat/AnsiString.h"
#include "vclcompat/TStringList.h"
#include "vclcompat/TDateTime.h"
#include "vclcompat/SysUtils.h"
#include "vclcompat/IniFiles.h"

// ---------------------------------------------------------------------------
//  Win32 A/W macro guard
//
//  windows.h #defines several file/string function names to their *A variants
//  (e.g. DeleteFile -> DeleteFileA, CopyFile -> CopyFileA, MoveFile ->
//  MoveFileA).  These macros would silently shadow the vclcompat AnsiString
//  overloads that the `using` declarations below bring into the global
//  namespace, causing "undefined reference to vclcompat::DeleteFile" or
//  silent name corruption at the call site.
//
//  Strategy: include <windows.h> centrally here (it was already being pulled
//  in by individual TUs such as SysUtils.cpp anyway) and immediately undefine
//  the colliding macros using guarded #ifdef / #undef so the header is safe
//  whether or not windows.h happened to be included earlier.  The real Win32
//  entry points remain reachable through their explicit *A spellings
//  (CopyFileA / DeleteFileA / MoveFileA).
//
//  NOT undefined: FindClose (real Win32 function, not a macro; vclcompat
//  FindClose(TSearchRec&) has a different signature and overload resolution
//  disambiguates); Sleep (Win32 real function, different signature).
// ---------------------------------------------------------------------------
#if defined(_WIN32)
// Do NOT define WIN32_LEAN_AND_MEAN here: some consumers (cmydef.h) rely on
// rpcndr.h's `byte` typedef that the lean build omits.  Include the full
// windows.h and let each consumer opt into LEAN themselves if desired.
#  include <windows.h>
// DeleteFile  -> DeleteFileA  -- shadows vclcompat::DeleteFile(AnsiString)
#  ifdef DeleteFile
#    undef DeleteFile
#  endif
// CopyFile    -> CopyFileA    -- shadows vclcompat::CopyFile(AnsiString,AnsiString,bool)
#  ifdef CopyFile
#    undef CopyFile
#  endif
// MoveFile    -> MoveFileA    -- preventive: vclcompat has no MoveFile yet but
//                                 a future Rename/MoveFile shim would collide.
#  ifdef MoveFile
#    undef MoveFile
#  endif
#endif // _WIN32

// BCB6 spelling alias: a lot of code uses `String` as a synonym for AnsiString.
#ifndef VCLCOMPAT_NO_GLOBAL_USING
using vclcompat::AnsiString;
using vclcompat::TStrings;       // abstract base (BCB6 TStrings*)
using vclcompat::TStringList;
using vclcompat::TDateTime;
using vclcompat::TObject;
using vclcompat::Word;

// IniFiles shims (BCB6 TIniFile / TMemIniFile)
using vclcompat::TIniFile;
using vclcompat::TMemIniFile;

// SysUtils free functions
using vclcompat::IntToStr;
using vclcompat::IntToHex;
using vclcompat::HexStrToInt;
using vclcompat::StrToInt;
using vclcompat::StrToIntDef;
using vclcompat::StrToFloat;
using vclcompat::StrToFloatDef;
using vclcompat::TryStrToFloat;
using vclcompat::FloatToStr;
using vclcompat::FloatToStrF;
using vclcompat::FormatFloat;
using vclcompat::Format;
using vclcompat::Trim;
using vclcompat::UpperCase;
using vclcompat::LowerCase;
using vclcompat::FileExists;
using vclcompat::DirectoryExists;
using vclcompat::CreateDir;
using vclcompat::ForceDirectories;
using vclcompat::DeleteFile;
using vclcompat::CopyFile;
using vclcompat::ExtractFilePath;
using vclcompat::ExtractFileName;
using vclcompat::ExtractFileExt;
using vclcompat::ChangeFileExt;
using vclcompat::IncludeTrailingBackslash;
using vclcompat::RemoveDir;
using vclcompat::FileSetAttr;
using vclcompat::FileGetAttr;
// file-system search (BCB6 FindFirst/FindNext/FindClose + TSearchRec)
using vclcompat::TSearchRec;
using vclcompat::FindFirst;
using vclcompat::FindNext;
using vclcompat::FindClose;
using vclcompat::faReadOnly;
using vclcompat::faHidden;
using vclcompat::faSysFile;
using vclcompat::faVolumeID;
using vclcompat::faDirectory;
using vclcompat::faArchive;
using vclcompat::faAnyFile;
using vclcompat::Sleep;
using vclcompat::TFloatFormat;
using vclcompat::ffGeneral;
using vclcompat::ffExponent;
using vclcompat::ffFixed;
using vclcompat::ffNumber;
using vclcompat::ffCurrency;

// TDateTime free functions
using vclcompat::Now;
using vclcompat::Date;
using vclcompat::Time;
using vclcompat::DecodeDate;
using vclcompat::DecodeTime;
using vclcompat::EncodeDate;
using vclcompat::EncodeTime;
using vclcompat::FormatDateTime;
using vclcompat::DateTimeToStr;
using vclcompat::TimeToStr;
using vclcompat::StrToDateTime;

// BCB6 `String` alias for AnsiString (common in VCL code).
typedef vclcompat::AnsiString String;
#endif // VCLCOMPAT_NO_GLOBAL_USING

// ---------------------------------------------------------------------------
//  BCB pragmas: g++ ignores unknown #pragma, but #pragma hdrstop / package /
//  resource carry no portable meaning. Translated files should simply drop
//  `#pragma hdrstop`, `#pragma package(...)` and `#pragma resource "*.dfm"`.
//  No macro can strip a #pragma; this is a translation-time edit (documented).
// ---------------------------------------------------------------------------

#endif // VCLCOMPAT_VCL_COMPAT_H
