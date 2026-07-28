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
//  Win32 A/W macro guard
//
//  windows.h #defines several file/string function names to their *A variants
//  (e.g. DeleteFile -> DeleteFileA, CopyFile -> CopyFileA, MoveFile ->
//  MoveFileA).  If any of those macros are already active when SysUtils.h
//  (below) DECLARES vclcompat::DeleteFile/CopyFile, the declaration itself
//  gets macro-substituted (e.g. into `vclcompat::DeleteFileA`), so the name
//  the `using` block further down expects (`vclcompat::DeleteFile`) was never
//  declared at all -- not a shadow, an outright missing declaration.
//
// AI(W906-W7-A0) 20260728: hoist winsock2+windows.h+A/W undefs above
// SysUtils.h so the umbrella is include-order-robust (was: broke whenever
// any TU saw windows.h first -- blocks Public/WinSocketErrorCode.cpp under
// MSVC and every future MFC TU). See docs/W7_UI_ARCHITECTURE_PLAN.md section
// 4, verification result V6.
//
//  Strategy: include <windows.h> HERE, BEFORE any vclcompat header that
//  declares one of the colliding names, and immediately undefine the
//  colliding macros. AI(W906-W7-A0-followup) 20260728: the declaring headers
//  are exactly SysUtils.h (DeleteFile, CopyFile) and TStringList.h
//  (GetObject) -- this list previously also named AnsiString.h, which
//  declares none of them, and implied a vclcompat MoveFile declaration,
//  which does not exist (its undef below is purely preventive, as that
//  undef's own comment says). On
//  MSVC only, <winsock2.h> is included first -- see the MSVC-vs-MinGW split
//  below; under MinGW no extra header is added ahead of <windows.h> here.
//  Doing this before those declarations -- rather than after, as this block
//  used to sit -- means the undefs are in effect no matter whether some
//  earlier-included file (this TU's own #include <windows.h>, or an upstream
//  MFC header such as <afxwin.h>) already pulled windows.h in first: our own
//  <windows.h> include is then just a no-op re-inclusion (header guards), but
//  our #undef lines still run unconditionally and land before SysUtils.h's
//  declarations are parsed either way. The previous position (after all the
//  vclcompat headers) only ever protected call sites made AFTER this header
//  from macro shadowing -- it did nothing for the declarations inside
//  SysUtils.h itself, which is exactly the case that broke (plan section 4,
//  V6 reproduced this verbatim: with windows.h included before vcl_compat.h,
//  the old ordering failed with "vclcompat::DeleteFile has not been
//  declared"). The real Win32 entry points remain reachable through their
//  explicit *A spellings (CopyFileA / DeleteFileA / MoveFileA / GetObjectA).
//
//  On MSVC only, <winsock2.h> is included first (defining _WINSOCKAPI_) so
//  that the windows.h include below never defaults to dragging in the legacy
//  winsock.h (v1) -- avoiding the classic MSVC "WinSock.h already included"
//  hard error for any TU (e.g. Public/WinSocketErrorCode.cpp) that also does
//  its own explicit #include <winsock2.h> afterward.
//
//  This pre-include is deliberately MSVC-only (`#if defined(_MSC_VER)`), NOT
//  applied under MinGW: MinGW's own <windows.h> already prefers WinSock v2
//  internally by default for WinNT4+ targets (via its private _winsock.h
//  chooser) without any help from us, and reproduction proved that adding an
//  unconditional standalone <winsock2.h> include HERE, ahead of <windows.h>,
//  actively BREAKS MinGW. The mechanism, verified by reading the installed
//  MinGW headers directly (NOT winerror.h's own repeat-inclusion guard --
//  that guard is exactly what gets defeated):
//    1. <winsock2.h> (winsock2.h:62) includes "winsock.h".
//    2. winsock.h:50 sets `__WINSOCK_H_SOURCED__ 1`, THEN winsock.h:52-53
//       does its own `#include <windows.h>` followed by `#include
//       <winerror.h>` -- both while that macro is still defined. windows.h
//       transitively reaches winerror.h a second way too, via winnt.h:46.
//    3. winerror.h:33 guards the whole file with `#ifndef _WINERROR_H`, but
//       the statement that actually DEFINES `_WINERROR_H` (winerror.h:41) --
//       along with every general Win32 error code between it and :1604,
//       including ERROR_SHARING_VIOLATION and ERROR_LOCK_VIOLATION -- lives
//       inside a NESTED `#ifndef __WINSOCK_H_SOURCED__` (winerror.h:36).
//       Because that macro is defined for both of winerror.h's entries in
//       this pass (the nested one via windows.h/winnt.h, and winsock.h's own
//       direct one right after), the nested block -- and therefore the
//       `_WINERROR_H` define itself -- is skipped BOTH times.
//    4. `__WINSOCK_H_SOURCED__` is only undefined at winsock.h:682, by which
//       point windows.h's own top-level include guard already marks the
//       whole header "done" for this TU. Any later, legitimate
//       `#include <windows.h>` (e.g. this file's own include just below) is
//       then a silent no-op that never revisits winnt.h/winerror.h to retry
//       -- so `_WINERROR_H` stays permanently unset and the general error
//       codes are never defined for the rest of the translation unit.
//  This was NOT hypothetical: it broke common.cpp's
//  `error==ERROR_SHARING_VIOLATION` (common.cpp:2313, a TU whose line 45
//  includes this umbrella first) in a full tree rebuild the first time this
//  fix was written with the pre-include unconditional, and was found and
//  fixed in the same wave (W7-A0) by gating it to MSVC only, as coded below
//  (docs/W7_UI_ARCHITECTURE_PLAN.md section 10 records the correction to
//  section 6's original unconditional instruction).
//
//  NOT undefined: FindClose (real Win32 function, not a macro; vclcompat
//  FindClose(TSearchRec&) has a different signature and overload resolution
//  disambiguates); Sleep (Win32 real function, different signature).
// ---------------------------------------------------------------------------
#if defined(_WIN32)
// Do NOT define WIN32_LEAN_AND_MEAN here: some consumers (cmydef.h) rely on
// rpcndr.h's `byte` typedef that the lean build omits.  Include the full
// windows.h and let each consumer opt into LEAN themselves if desired.
#  if defined(_MSC_VER)
#    include <winsock2.h>
#  endif
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
// GetObject   -> GetObjectA   -- shadows vclcompat::TStringList::GetObject(int) const
// AI(W906-W7-A0-Followup) 20260728: added after an exhaustive `g++ -dM -E
// windows.h` macro sweep cross-checked against every identifier in
// vclcompat/*.h found this was the one A/W collision the original A0 pass
// missed. Without this undef, TStringList.h:176's `GetObject(int) const`
// declaration is macro-substituted to `GetObjectA` in every TU that includes
// this umbrella, while TStringList.cpp (which only includes TStringList.h,
// never windows.h) still DEFINES the unsubstituted `GetObject` -- a silent
// cross-TU ODR mismatch that passes -fsyntax-only but fails at link time with
// "undefined reference to vclcompat::TStringList::GetObjectA(int) const".
#  ifdef GetObject
#    undef GetObject
#  endif
#endif // _WIN32

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
using vclcompat::random;         // AI(W906-TesterTCPTimer) 20260720: BCB6 RTL random(int) -- SimulateBin's Dell-Fix bin-count picker
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
