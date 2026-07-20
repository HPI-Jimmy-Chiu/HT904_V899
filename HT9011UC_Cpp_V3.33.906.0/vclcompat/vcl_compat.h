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

// AI(W906-FastcallFix) 20260720: REMOVED a dead `#ifndef __fastcall /
// #define __fastcall / #endif` neutralization block that used to sit here.
// It was never effective: `__fastcall` is a compiler-BUILT-IN keyword/macro
// on i686 MinGW GCC (a real calling-convention attribute, not a Borland-only
// token this toolchain is ignorant of), so `#ifndef __fastcall` was always
// false and the `#define` never fired -- ALL `__fastcall`-marked code in this
// tree has always compiled under the REAL fastcall ABI, not a neutralized
// no-op, contrary to what this block's old comment implied.
//
// Practical consequence (verified by the 2026-07-20 audit,
// AUDIT_fastcall_tree.md): every function marked `__fastcall` in one
// translation unit's DECLARATION must be marked `__fastcall` in whatever
// translation unit provides its DEFINITION too (and vice versa) -- a mismatch
// silently produces two DIFFERENTLY MANGLED symbols (fastcall decorates the
// mangled name, e.g. `@_Z...@72`) instead of a compile error, which then
// surfaces only as an unresolved-external at link time, or worse, as an ld
// "fixup" heuristic silently binding mismatched ABI calling conventions. The
// audit found exactly one such live inconsistency (`MyDBIProcess`'s 3-arg
// overload, fixed this same wave -- see SECSGEM/uHGemEquipment.cpp) and
// confirmed 91 other __fastcall-marked pairs across the tree were already
// consistent.
//
// The guardrail going forward is procedural, not a macro: keep every
// declaration/definition pair's `__fastcall` presence in lockstep, and treat
// a nonzero `grep -ic resolving` hit in a fresh build's linker log as a
// signal to go check for exactly this class of mismatch. Do NOT "fix" this
// by defining `-D__fastcall=` (or an empty `#define __fastcall` here) as a
// global neutralization -- that would flip the ACTUAL calling convention of
// every one of those 91-plus already-consistent pairs tree-wide, a
// deliberate large-scale ABI change this fix explicitly does not make; it
// would need its own dedicated, carefully-verified wave.
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
#include "vclcompat/TList.h"       // VCL.Classes TList (generic void* list)
#include "vclcompat/TDateTime.h"
#include "vclcompat/SysUtils.h"
#include "vclcompat/IniFiles.h"
#include "vclcompat/Comm.h"        // Spcomm::TComm serial shim (SPComm.hpp)
// AI(W906-ServerSocket) 20260713: TServerSocket/TServerWinSocket (multi-
// connection ScktComp server shim). Transitively pulls in ClientSocket.h too
// (TClientSocket/TCustomWinSocket -- not otherwise listed in this umbrella;
// see ClientSocket.h's own header note on why it wasn't wired here directly).
// Naming-collision check done first (per TList.h's own precedent below):
// grepped the whole Cpp tree for TServerSocket/TServerWinSocket/
// TCustomWinSocket/TClientSocket/TErrorEvent/TSocketNotifyEvent/
// TSocketErrorEvent -- no existing class definitions anywhere outside
// vclcompat/ (only comments referencing the eventual golden translation),
// so no TList-style exclusion is needed for this include.
#include "vclcompat/ServerSocket.h"

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
// NOTE: vclcompat::TList is deliberately NOT brought into the global
// namespace here (unlike the other compat types on this list). A DIFFERENT,
// unrelated global-namespace `class TList` shim already exists in
// aHotPlateSubstrate.h (a hard-boundary shared file -- see W6.2 substrate,
// golden Public/HTEditList.h pointer-list usage) for a narrower touched
// surface (Add/Clear/Count/Items[] read-only). A `using` here would collide
// with that class's later `class TList { ... };` definition in the same
// (global) scope wherever both headers are included in one TU --
// confirmed by compiling aHotPlateSubstrate.h against this change before
// finishing (redefinition error). Consumers of vclcompat::TList (e.g. the
// planned SECSGEM SV/EC registration API) should use the qualified name
// `vclcompat::TList`, or add their own local `using vclcompat::TList;` in a
// TU that does NOT also include aHotPlateSubstrate.h.
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
// AI(W5-Final-TesterTCPSocket) 20260711: StringReplace + its TReplaceFlags set
// (golden SysUtils; see vclcompat/SysUtils.h for the exact call-shape citation).
using vclcompat::TReplaceFlag;
using vclcompat::rfReplaceAll;
using vclcompat::rfIgnoreCase;
using vclcompat::TReplaceFlags;
using vclcompat::StringReplace;
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
