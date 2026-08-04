cmake_minimum_required(VERSION 3.10)
project(HT9045_Cpp C CXX)   # C needed for Public/cJSON.c

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =============================================================================
# AI(W906-W7-L1-Wave0) 20260801: pin the Win32 API floor for BOTH oracles, so the
# two of them stop seeing DIFFERENT API surfaces.  This lands decision DA2 (of
# decision set D2) from docs/W7_UI_ARCHITECTURE_PLAN.md SS2, which was already
# taken and recorded there but explicitly left un-landed with "owner = main loop";
# the reasoning below is that section's, not re-derived here.
#
#   * The PROBLEM being fixed is oracle divergence, not a missing feature.  With
#     nothing pinned, MinGW defaults to _WIN32_WINNT=0x0500 (Windows 2000) while
#     MSVC auto-selects the newest (it prints "_WIN32_WINNT not defined.
#     Defaulting to _WIN32_WINNT_MAXVER").  An API declared on only one of those
#     two surfaces therefore slips past BOTH oracles.  Pinning one value is what
#     makes "declared on one side only" a build error somewhere.
#   * WHY 0x0601 (Windows 7) AND NOT 0x0A00.  The primary oracle is MinGW.org
#     MinGW32 gcc 6.3.0, whose C:\MinGW\include\sdkddkver.h tops out at
#     _WIN32_WINNT_WINBLUE (0x0603) and has no _WIN32_WINNT_WIN10 at all.  Worse
#     than failing, the bare number 0x0A00 compiles SILENTLY there while being
#     completely inert (that w32api carries no Win10-only declarations), which
#     would manufacture a false "we are pinned to Win10" impression.  0x0601 is
#     the highest value that means anything on this oracle.
#   * A COMPILE-TIME FLOOR IS NOT A RUNTIME TARGET.  The shipped runtime target
#     stays Windows 10/11; this only fixes which declarations the headers expose.
#   * WINVER is set to the same value deliberately -- the two are independent
#     macros in the Windows headers and leaving WINVER unpinned would reintroduce
#     exactly the inconsistency this exists to remove.
# VERIFIED BY REBUILDING AFTER ADDING IT, and stated precisely rather than as
# "warning-clean" (this build is NOT warning-free and never was): a full MinGW
# rebuild produced 277 warning lines before the pin and 284 after, and the entire
# +7 delta is -Wmissing-field-initializers from the 7 LAST_GENERAL_SET fields this
# same pass added to canary_support.h under its pre-existing `LastSet = {0}`
# aggregate initialiser.  The set of NON-missing-field-initializer warning texts
# is byte-identical before and after, i.e. the pin itself introduced ZERO new
# warnings.  common.cpp:2313's ERROR_SHARING_VIOLATION / ERROR_LOCK_VIOLATION also
# still resolve (they are plain winerror.h codes, not version-gated).
# =============================================================================
add_compile_definitions(_WIN32_WINNT=0x0601 WINVER=0x0601)

# =============================================================================
# AI(W906-W7-A1) 20260728: MSVC second-oracle enablement block. MUST stay at
# the TOP of this file (after project(), before any add_library) -- see
# docs/W7_UI_ARCHITECTURE_PLAN.md SS6 W7-A1 / SS4-V5 / SS4-V8 for the full
# rationale; only a compressed summary is kept here.
#
#   1. LINK_GROUP:RESCAN feature flags -- tests/CMakeLists.txt has exactly 66
#      real "$<LINK_GROUP:RESCAN,...>" generator-expression sites (70 lines
#      match the string; 4 of them are comments).  AI(W906-F0fix2) 20260729:
#      this said "~69 real ... now more as later waves append tests", which was
#      wrong -- the count is unchanged from the plan's SS4-V8 measurement of 66,
#      because W7-F0 only edited existing group lines to insert ht9045_forms and
#      added no new group.  Those exist ONLY to force GNU ld
#      to re-scan static-archive dependency cycles at link time. CMake's
#      LINK_GROUP feature is GNU/Clang-ld-specific and, unless MSVC is told
#      the feature exists, CMake fails at CONFIGURE time with
#      "Feature 'RESCAN' ... is not supported" -- before a single MSVC source
#      file is even compiled. MSVC's link.exe already rescans archives on its
#      own, so declaring the feature "supported" with an empty prefix/suffix
#      makes every one of those sites a genuine no-op under MSVC. None of the
#      66 sites needs editing for MSVC.
#   2. /utf-8 -- translated sources contain UTF-8 non-ASCII (banner comments
#      citing Chinese golden identifiers, etc). Without /utf-8, MSVC decodes
#      source with the active code page (cp950 on this machine), which both
#      warns (C4819) and can re-encode narrow string literals wrongly.
#      /W3 matches the project's existing baseline warning level.
#   3. _HAS_STD_BYTE=0 -- cmydef.h:127 / MyLaneIo.h:76-78,98 /
#      Motor/HTMotor.h:92 use a bare `byte` name while `using namespace std;`
#      is in scope (MachineDefine.h:30,67, database.h:39). Under MSVC's
#      C++17 <cstddef>, std::byte pulled into scope makes `byte` C2872
#      ambiguous; g++ 6.3 (MinGW) predates std::byte entirely, so MinGW never
#      saw this. _CRT_SECURE_NO_WARNINGS -- silence MSVC's "unsafe" CRT
#      function warnings (sprintf/strcpy/etc, already used throughout the
#      ported BCB6-era code) so real signal isn't drowned by noise.
#   4. FP fidelity: on 32-bit MSVC, /arch:IA32 restores x87 (rather than SSE2)
#      floating point codegen. Per SS4-V5, /arch:IA32 ALONE is NOT enough to
#      reproduce BCB6's x87 80-bit intermediate precision -- the runtime FP
#      control word also has to be widened to 64-bit (extended) precision,
#      which is done in tests/test_bootstrap.cpp via _controlfp_s. Both are
#      required together; neither alone reproduces it.
#
# This whole block is a no-op for GNU/Clang (MinGW): every line is gated on
# if(MSVC), so the primary MinGW oracle's build is byte-for-byte unaffected.
# =============================================================================
if(MSVC)
    # --- 1. LINK_GROUP:RESCAN: declare the feature "supported" as a no-op ---
    set(CMAKE_C_LINK_GROUP_USING_RESCAN_SUPPORTED TRUE)
    set(CMAKE_C_LINK_GROUP_USING_RESCAN "" "")
    set(CMAKE_CXX_LINK_GROUP_USING_RESCAN_SUPPORTED TRUE)
    set(CMAKE_CXX_LINK_GROUP_USING_RESCAN "" "")

    # --- 2. Source encoding ---------------------------------------------------
    # AI(W906-W7-A1) 20260728: /wd4828 was added here after a from-scratch
    # MSVC build produced 8,961,951 total C4828 warning lines, traced to
    # leftover raw Big5 bytes in comments (and, in one file, in narrow string
    # literals) across Config.h, CosFunction.h, MachineDefine.h, MachineType.h,
    # cmydef.{h,cpp}, cprod.h, cpublic.h, myTimer.{h,cpp},
    # Public/WinSocketErrorCode.cpp -- never converted when those files were
    # copied out of the (Big5) golden tree.
    # AI(W906-encoding) 20260728: fixed at the source instead of leaving the
    # suppression permanent. All 11 files were transcoded whole-file
    # cp950 -> UTF-8 (mechanical, not manual retranslation: each file's raw
    # bytes decode losslessly under strict cp950 -- verified no exceptions --
    # so decode(cp950).encode('utf-8') recovers the exact original Chinese
    # text with only the on-disk byte encoding changed; confirmed line-by-line
    # that decoded text is character-for-character identical before/after).
    # Public/WinSocketErrorCode.cpp needed extra care: 8 of its bad-byte
    # occurrences (7 string-literal assignments, lines ~60-231) are the Big5
    # em dash (0xA1 0x58, U+2014) embedded in English MSDN-derived error text,
    # not a comment -- under UTF-8 that raw pair is an invalid/orphan
    # continuation byte (this IS the C4828 trigger), so leaving it raw would
    # have been a real behaviour difference, not just noise. The same
    # cp950->UTF-8 transcode turns it into a valid UTF-8 EM DASH, which is
    # what a Big5-locale BCB6 build actually rendered -- so the fix recovers
    # intended behaviour rather than changing it.
    # All 11 files re-verified UTF-8-valid, BOM-free, and free of embedded
    # U+FFFD after the transcode; /wd4828 is removed below because the
    # detector's only known target is gone. g++/MinGW has no equivalent of
    # this per-byte source-charset validation, so this class of defect is
    # invisible to the primary (MinGW) oracle -- exactly why the MSVC second
    # oracle (plan SS6) exists.
    add_compile_options(/utf-8 /W3)

    # --- 3. std::byte / using-namespace-std collision workaround --------------
    add_compile_definitions(_HAS_STD_BYTE=0 _CRT_SECURE_NO_WARNINGS)

    # --- 4. FP fidelity: x87-equivalent codegen on 32-bit MSVC -----------------
    #        (paired with the _controlfp_s call in tests/test_bootstrap.cpp)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_compile_options(/arch:IA32)
    endif()
endif()

# ---------------------------------------------------------------------------
#  GNU/MinGW: static-link the compiler runtimes
# ---------------------------------------------------------------------------
# AI(W906-W7-A1fix) 20260729: link libgcc/libstdc++ statically so every
# executable is self-contained.
#
# WHY, and it is not cosmetic: without this every test .exe imports
# libgcc_s_dw2-1.dll and libstdc++-6.dll (confirmed with objdump -p) and
# resolves them from PATH at LOAD time. Launch one from any environment whose
# PATH lacks C:\MinGW\bin -- a plain PowerShell, a vcvarsall shell that
# rewrote PATH, a scheduler, another tool's subprocess -- and the loader fails
# with STATUS_DLL_INIT_FAILED (0xc0000142), which Windows reports as a MODAL
# "application was unable to start correctly" dialog. That happened to
# test_testertcp_socket.exe on 2026-07-29 and interrupted the user.
#
# This failure mode is INVISIBLE to tests/test_bootstrap.cpp's dialog guards:
# a loader failure occurs before ANY user code -- including static
# initializers -- has run, so SetErrorMode has not been called yet and cannot
# suppress it. The earlier claim that the bootstrap covers batch runs was
# therefore overstated; it covers crashes and asserts DURING execution, not
# failures to start. Removing the external dependency is the only fix that
# addresses the cause rather than the symptom, and it makes the exes runnable
# from any working directory or shell.
#
# Cost: a few hundred KB per executable, and no behaviour change -- the same
# libstdc++/libgcc code, bound at link time instead of load time.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_link_options(-static-libgcc -static-libstdc++)
endif()

# ---------------------------------------------------------------------------
# Library: vclcompat  (W0 keystone)
#   BCB6 / VCL compatibility layer: recreate ONLY the API surface the V906
#   source actually uses (AnsiString / TStringList / TDateTime / SysUtils),
#   with semantics that mirror BCB6 exactly (1-based AnsiString indexing, etc.).
#   Translated files #include "vclcompat/vcl_compat.h" instead of <vcl.h>.
# ---------------------------------------------------------------------------
add_library(vclcompat
    vclcompat/AnsiString.cpp
    vclcompat/TStringList.cpp
    vclcompat/TDateTime.cpp
    vclcompat/SysUtils.cpp
    vclcompat/IniFiles.cpp
    # --- HAL enabler: SPComm TComm serial shim (Spcomm::TComm) --------------
    #   Faithful stand-in for the SPComm VCL serial component
    #   (D:/HT9045/elec/Component/spcomm.hpp:154-277) used by the temp /
    #   barcode / EJ1N / RS232-tester code.  REAL mode drives \\.\COMx via
    #   Win32 CreateFile/SetCommState/ReadFile/WriteFile + a Win32 reader
    #   thread; SIM mode (auto fallback / SetSimMode) needs no hardware so the
    #   compat layer compiles/links/runs offline.  Lives in the vclcompat lib
    #   because it derives from vclcompat::TComponent and is pulled in via the
    #   vcl_compat.h umbrella.  On Win32 the .cpp uses the kernel32 serial API
    #   (already in the default MinGW link set -- no extra target_link needed).
    vclcompat/Comm.cpp
    # --- W5 comms enabler: Scktcomp TClientSocket/TCustomWinSocket shim -------
    #   Faithful stand-in for the VCL Scktcomp::TClientSocket (Winsock2 TCP
    #   client) used by MyPLC/ModbusTCPClient.cpp.  Default is SIM mode (no
    #   real connect attempted); Real mode is an explicit composition-root
    #   opt-in (SetSimMode(false)) -- see the file header for why this does
    #   NOT auto-fall-back to Sim on a failed Real connect (Enable_PLCSafety_IO
    #   is a safety-interlock read; masking a real connect failure would be
    #   unsafe).  Lives here (not in MyPLC/ or a new library) because it is
    #   general vclcompat infra, matching Comm.cpp's precedent.
    vclcompat/ClientSocket.cpp
    # --- W906-ServerSocket enabler: Scktcomp TServerSocket/TServerWinSocket
    #   shim (multi-connection server sibling of ClientSocket.cpp above).
    #   Faithful stand-in for the VCL ScktComp::TServerSocket used by
    #   SECSGEM/uHGemEquipment.cpp's srvGem (passive-HSMS role) and
    #   Automation/automation.cpp's OLPServer -- see ServerSocket.h's own
    #   file-head for both golden call-shape citations.  No production
    #   consumer wired yet (prerequisite infra, same shape as the earlier
    #   SecsWireCodec/SecsSvEcRegistration slices); reuses ClientSocket.h's
    #   TCustomWinSocket for its per-connection objects (see that header's
    #   own EXTENSION note), so it must link/compile alongside ClientSocket.cpp
    #   in the same library, not standalone.  Default is Sim mode; Real mode
    #   is an explicit SetSimMode(false) opt-in (WinSock2 listen/accept +
    #   a background acceptor thread + per-connection reader threads).
    vclcompat/ServerSocket.cpp
    # --- W906-uHGemEquipment enabler: headless TStringGrid shim (Grids.hpp) --
    #   Faithful stand-in for the real VCL TStringGrid used by SECSGEM/
    #   uHGemEquipment.h's CEID/Report/Alarm "StringGrid database" family
    #   (strGrdCEID/stdGridReportID/strGrdAlarm/strGrdAlarmOld -- see that
    #   header's own file-head note). Headless: Cells[ACol][ARow] (0-based,
    #   unlike AnsiString's 1-based indexing) + growable RowCount/ColCount
    #   property proxies; no rendering/FixedRows/Options. Zero dependency
    #   beyond vclcompat/AnsiString.h + <vector>. NOT brought into the global
    #   namespace by vcl_compat.h (consumers add their own `using
    #   vclcompat::TStringGrid;` -- see that header's own note on why).
    vclcompat/StringGrid.cpp
    # --- W906-uHGemEquipment-BucketC enabler: headless TMemoryStream shim
    #   (Classes.hpp). Faithful stand-in for the real VCL TMemoryStream used
    #   by SECSGEM/uHGemEquipment.cpp's RecvMemoryBuffer/ProcBuffer/
    #   TempProcBuffer (clientGemRead/ProcessSocketReceiveData) -- see
    #   vclcompat/MemoryStream.h's own file-head note for the exact API
    #   surface + semantics. Zero dependency beyond <vector>/<cstring>.
    #   vclcompat/SyncObjs.h (TCriticalSection) is header-only (inline) --
    #   no matching .cpp, no separate CMake source entry needed for it.
    vclcompat/MemoryStream.cpp
    # AI(W906-SpoolCluster) 20260721: headless TFileListBox shim (FileCtrl.hpp) -
    #   Faithful stand-in for the real VCL TFileListBox used by SECSGEM/
    #   uHGemEquipment.h's THGem::FileListBox1 (Spool cluster: WriteToSpoolFile/
    #   DoSpool/SetSpoolActive/GetSpoolActive) -- see vclcompat/FileListBox.h's
    #   own file-head note for the exact scan-logic/sort-order/Refresh()==
    #   Update() rationale. Zero dependency beyond vclcompat/AnsiString.h +
    #   vclcompat/TStringList.h + vclcompat/SysUtils.h's FindFirst/FindNext/
    #   FindClose/TSearchRec family (already real). NOT brought into the
    #   global namespace by vcl_compat.h (consumers add their own `using
    #   vclcompat::TFileListBox;` -- same established precedent as
    #   vclcompat::TStringGrid above).
    vclcompat/FileListBox.cpp
)

target_include_directories(vclcompat PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ---------------------------------------------------------------------------
# Library: ht9045_core
#   Collects all ROOT-level translated source files.
#   Mirror the BCB6 source tree: translated .cpp/.h files live at the project
#   root (same relative path as their BCB6 originals).
# ---------------------------------------------------------------------------
add_library(ht9045_core
    ContactForce.cpp
    cUnitConvert.cpp      # W2 partial: iUnitMultiply100/iUnitMultiply1000
    common.cpp            # W3 partial: ini-helper family only; other regions gated #if 0
    # --- W64b (TESTER/INDEX force-calc extract): TfContact::GetMaxIndexForceLimit /
    #   GetMinForce turned into free functions (ComputeMaxIndexForceLimit / ComputeMinForce).
    #   Only #includes MachineType.h (header-only enum/macro header per KNOWLEDGE.md W0); every
    #   other external read (dDutCount, TestIF.iTestMode, IniConfig.*, fContactForce->SLKClass[i]->
    #   dContactOffset*) is passed in as an explicit scalar parameter, so this TU needs NO link to
    #   ht9045_globals (confirmed empirically: the clean build below links fine without it).
    cContact.cpp
    # --- W906-MainCalcCore (TfMain calc-core extract): 6 pure calc-core TfMain member
    #   functions turned into free functions (ComputeShtModeFlag / ComputeCanChangeRealDummy /
    #   ComputeSiteMapIsStander / ComputeSiteMapPriority / ComputeCanChangeSite /
    #   ComputeCanChangeToSocket). Only #includes MachineType.h (header-only); every other
    #   external read (TestIF.*, MOT[].HasIC(), InArmSuck/OutArmSuck.HasIC(), ShuttleHasIC()/
    #   IndexHasIC()/InputShuttleHasIC(), IniConfig.*, bCanAutoCloseSite, bPickFromLoader,
    #   iPickFromLoadStageTask, SystemStart, iSiteTotal) is passed in as an explicit scalar
    #   parameter -- same "no ht9045_globals link needed" shape as cContact.cpp above.
    MainCalcCore.cpp
)

target_include_directories(ht9045_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ht9045_core links the compat layer (translated logic will depend on it).
target_link_libraries(ht9045_core PUBLIC vclcompat)

# Strict warnings help catch porting errors early.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(HT9045_WARNFLAGS -Wall -Wextra -Wshadow -Wconversion -Wpedantic)
    target_compile_options(vclcompat   PRIVATE ${HT9045_WARNFLAGS})
    target_compile_options(ht9045_core PRIVATE ${HT9045_WARNFLAGS})

    # AI(W64b-ForceCalc-Translate) 20260710: cContact.cpp is the first ht9045_core TU to
    # #include MachineType.h under full -Wpedantic.  MachineType.h (already-accepted W0
    # keystone header) carries several BCB6-preserved `};` stray-semicolon idioms after
    # free-function template bodies (e.g. ChangeToFloatNonPcnt) that ISO C++ pedantically
    # flags as "extra ';'" at namespace scope -- harmless, but not this wave's file to fix.
    # Narrowly suppress -Wpedantic on this ONE translation unit only (same precedent as
    # ht9045_public's HTMD5.cpp / ht9045_globals' idiom carve-outs); every other warning
    # class (-Wall -Wextra -Wshadow -Wconversion) still applies in full to cContact.cpp.
    set_source_files_properties(cContact.cpp PROPERTIES COMPILE_FLAGS "-Wno-pedantic")

    # AI(ht9045-v899) 20260722: MainCalcCore.cpp also #includes MachineType.h (via
    # MainCalcCore.h) and hits the SAME stray-';' -Wpedantic idiom noted above for
    # cContact.cpp. Same narrow suppression, same reasoning.
    set_source_files_properties(MainCalcCore.cpp PROPERTIES COMPILE_FLAGS "-Wno-pedantic")
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_public
#   Portable utility modules translated from BCB6 Public/*.
#
#   cJSON.c  -- portable C (Dave Gamble, MIT); compiled as C via set_source_files_properties.
#              No VCL dependency.  #include "Public/cJSON.h" from any C++ TU.
#
#   HTMD5.cpp -- MD5 class + file/folder helpers.  Depends on vclcompat for
#               AnsiString, TStrings/TStringList, FileExists, DirectoryExists.
#               VCL TMask replaced by simple_wildcard_match(); auto_ptr -> unique_ptr.
# ---------------------------------------------------------------------------
# Compile cJSON.c as C (it must not see C++ name mangling on its own symbols).
set_source_files_properties(Public/cJSON.c PROPERTIES LANGUAGE C)

add_library(ht9045_public
    Public/cJSON.c
    Public/HTMD5.cpp
    # --- W1 Public leaf utilities (BCB6 -> standard C++17) ------------------
    Public/ExternFunction.cpp      # Math_AdjustAngle / String_JustNumber /
                                   # String_Erase[LR]Space / RecordTime  (pure logic)
    Public/WinSocketErrorCode.cpp  # GetErrorMsg(Winsock code -> AnsiString); needs ws2_32 headers
    Public/cBootLog.cpp            # Win32 file-based boot tracer (WriteBootLog / Rotate)
    # --- HAL enabler: KeyPro license interface-cut --------------------------
    #   KeyPro_GetLevel(unsigned) -- portable stand-in for the SOLE KeyPro entry
    #   the V906 source calls (KEYPRO_GET_LEVEL(3) at HS_Function.cpp:4933).
    #   Resolves the export from KeyProDLL.dll at RUN TIME (LoadLibrary/
    #   GetProcAddress) and returns 1 (valid dongle) when the DLL is absent
    #   (offline).  Deliberately does NOT static-link KeyProDLL.lib (32-bit OMF
    #   Borland import lib, unusable by MinGW) -- no extra target_link_libraries
    #   entry is added for it.  The std::string-returning KeyPro exports are
    #   NEVER called in 906 and are NOT ported.
    Public/HTKeyProShim.cpp
)

# cJSON.c uses WIN32 FindFirstFile indirectly (via cJSON.h guards); HTMD5.cpp
# uses FindFirstFileA directly.  Both need windows.h available under MinGW --
# it is, but we silence the dllexport chatter by hiding symbols for the static build.
target_compile_definitions(ht9045_public PRIVATE
    CJSON_HIDE_SYMBOLS          # suppress __declspec(dllexport) in cJSON.h
)

target_include_directories(ht9045_public PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_public PUBLIC vclcompat)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_public PRIVATE ${HT9045_WARNFLAGS})
    # HTMD5.cpp contains the original RFC 1321 MD5 algorithm bit-operations and
    # streampos casts that were never ported (intentional faithful translation).
    # Suppress inherited warnings on that TU only; all other TUs keep full strictness.
    set_source_files_properties(Public/HTMD5.cpp PROPERTIES COMPILE_FLAGS
        "-Wno-conversion -Wno-parentheses")
endif()

# ht9045_core also benefits from public utilities.
target_link_libraries(ht9045_core PUBLIC ht9045_public)

# ---------------------------------------------------------------------------
# Library: ht9045_common
#   Translated logic islands under Common/ that are interface-driven and free
#   of Prod/cmydef/MachineType/hardware/VCL-form coupling.
#
#   PickPlanner -- generic enumerate-and-score Arm pick/place planner
#     (cArmPickPlan) plus two production adapters that are interface-driven
#     via IArmPickPlanContext:
#       cInArmLoaderPickPlanner -- Phase 3 scaffold (Search() returns false)
#       cOutArmPlacePlanner     -- Phase 4 scaffold (Search() returns false)
#     The adapters ship as scaffolds: the Phase 3.1 / 4.1 bodies that would
#     pull MOT[]/InArmSuck/OutArmSuck globals are deferred to W6 and remain
#     inline TODOs.  Nothing here reaches an untranslated global, so the whole
#     subtree compiles standalone against vclcompat only.
# ---------------------------------------------------------------------------
add_library(ht9045_common
    Common/PickPlanner/cArmPickPlan.cpp
    Common/PickPlanner/cInArmLoaderPickPlanner.cpp
    Common/PickPlanner/cOutArmPlacePlanner.cpp
)

target_include_directories(ht9045_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Common/PickPlanner
)

# PickPlanner .cpp files swap <vcl.h> for the vclcompat umbrella header, so the
# compat layer must be on the include path even though no VCL type is used.
target_link_libraries(ht9045_common PUBLIC vclcompat)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_common PRIVATE ${HT9045_WARNFLAGS})
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_globals  (W0-TAIL keystone)
#   The de-VCL'd GLOBAL define/type headers + the .cpp regions that define the
#   contract globals. These headers (MachineType / Config / CosFunction / cprod /
#   cpublic / cmydef / myTimer / MachineDefine) gate most downstream logic.
#
#   Each .cpp keeps its active global var DEFINITIONS (Prod / IniConfig /
#   CosFunction / Offset / ARM_OFFSET* arrays / name-tables / TQPF_Timer) while
#   the function bodies that reach untranslated state machines / app headers are
#   gated behind '#if 0 // TODO(Wx)'.  Built as a static library: it must COMPILE
#   standalone (verified), but is NOT linked into an executable here because the
#   active globals construct objects whose ctors live in the gated regions
#   (those resolve in W3/W6/W7).  myTimer.cpp is fully translated this wave.
# ---------------------------------------------------------------------------
add_library(ht9045_globals STATIC
    myTimer.cpp
    cprod.cpp
    cpublic.cpp
    cmydef.cpp
    LastSet.cpp    # AI(W906-GA1-B2-integrate) 20260804: moved here from ht9045_sm -- the
                   # ungated cprod.cpp (same archive) references LastSet directly, and
                   # globals is the lowest common archive every consumer already links.
)

target_include_directories(ht9045_globals PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_globals PUBLIC vclcompat)

# AI(ht9045-v899) 20260626: cpublic.cpp now ungates FindAndKillProcess (psapi) and
# VerInfo::GetAppVersion/m_GetVerInfo (version.lib).  Both ship with MinGW but
# must be listed explicitly -- the linker does not auto-pull them.
if(WIN32)
    target_link_libraries(ht9045_globals PUBLIC psapi version)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # The translated globals carry the original BCB6 idioms (e.g. AnsiString
    # array brace-init with NULL).  Keep them compiling without the strict
    # -Werror-style flags used on freshly-written modules.
    target_compile_options(ht9045_globals PRIVATE
        -Wno-conversion-null -Wno-write-strings)
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_forms  (W7-F0 keystone -- the FormsFacade sink)
#
# AI(W906-W7-F0) 20260728: NEW library, created by the W7-F0 FormsFacade
# refactor (docs/W7_UI_ARCHITECTURE_PLAN.md SS2-D6 / SS6-F0-d).
#
# WHAT IT IS
#   The 9 non-VCL form-pointer stand-ins the state machines dereference
#   (fMain / fLotInfo / fSCKART / fSortCT / fAGV / fNote / fOffSet / fCleaning /
#   fShowMessage), previously all in a single root-level FormsFacade.cpp that
#   was compiled into ht9045_sm.  FormsFacade.cpp is gone; FormsFacade.h is now
#   an umbrella that just #includes forms/*.h, so all 113 existing
#   `#include "FormsFacade.h"` lines are untouched.
#
# WHY IT EXISTS -- IT BREAKS A REAL CMake CYCLE ("Wall 2", plan SS4-V1/SS3-C7)
#   Before this wave, verified directly in THIS file:
#     * FormsFacade.cpp was in ht9045_sm's source list;
#     * `target_link_libraries(ht9045_secsgem PUBLIC vclcompat ht9045_public
#        ht9045_core ht9045_globals ht9045_db)` -- ht9045_secsgem does NOT link
#        ht9045_sm;
#     * `target_link_libraries(ht9045_sm PUBLIC ht9045_io ht9045_motor
#        ht9045_globals vclcompat ht9045_secsgem)` -- ht9045_sm DOES link
#        ht9045_secsgem.
#   So SECSGEM/uHGemHT9045.cpp could never reach fMain: adding ht9045_sm to
#   ht9045_secsgem would have created a genuine CMake target cycle.  That, not
#   the "~19 missing FormsFacade stubs" MIGRATION_ROADMAP.md:133 blames, was the
#   structural blocker.  Sinking the facade to a BOTTOM layer removes it:
#   ht9045_secsgem now links ht9045_forms (see its own link line below), and
#   ht9045_forms links nothing above itself.
#
#   THE EXACT MECHANISM -- AI(W906-W7-F0-fix) 20260728, measured, because the
#   plain phrase "CMake target cycle" is NOT self-evidently an error:
#     * A cycle between two STATIC libraries is LEGAL in CMake.  Measured with a
#       2-target throwaway probe (A STATIC <-> B STATIC): configure exit 0,
#       build exit 0, and the generated linkLibs.rsp reads
#       `libA.a libB.a libA.a libB.a` -- CMake just repeats the archives.  Every
#       library in this file is STATIC (no add_library(... SHARED), and
#       BUILD_SHARED_LIBS is never set), so "they are all static libs, therefore
#       a cycle is fine" would have been a reasonable and WRONG conclusion.
#     * What makes it a hard error HERE is that the cycle runs through a
#       $<LINK_GROUP:RESCAN,...> node, and a group is not a STATIC_LIBRARY
#       target -- CMake's rule is "cyclic dependencies are allowed only among
#       static libraries".  Measured on THIS project without editing it, by
#       re-injecting the pre-F0 edge via
#         cmake -DCMAKE_PROJECT_INCLUDE=<file containing>
#           cmake_language(DEFER ... CALL target_link_libraries
#                          ht9045_secsgem PUBLIC ht9045_sm)
#       -> configure/generate FAILED (exit 1) with
#         "The inter-target dependency graph, for the target
#          test_config_loaders, contains the following strongly connected
#          component (cycle): group RESCAN:{ht9045_secsgem,ht9045_forms,...}
#          depends on ht9045_sm; ht9045_sm depends on group ..."
#       and the same error for test_uHGemEquipment -- and ONLY those two, because
#       they are the only two targets whose RESCAN group contains
#       ht9045_secsgem but NOT ht9045_sm (tests/CMakeLists.txt:291 and :2027).
#       Both groups already had that shape before this wave (verified against
#       git HEAD), so the blocker was real pre-F0 too.
#     * The same injection with `target_link_libraries(ht9045_forms PUBLIC
#       ht9045_sm)` instead ALSO fails (exit 1, same two targets).  So "just
#       declare the ht9045_forms -> ht9045_sm dependency honestly and drop the
#       bottom-layer claim" is not an available option -- it does not configure.
#       That is why the fix below moves the source instead.
#
# DEPENDENCIES -- and why they are acyclic
#   * vclcompat       -- AnsiString / TStringList / TStringGrid / the unified
#                        widget stand-ins in vclcompat/Controls.h.
#   * ht9045_globals  -- forms/fMain.cpp's faithfully-translated
#                        WritePERSITETemperature body reads TestIF.iTestType
#                        (cprod.cpp) and writes asTCPIPTemperature (cmydef.cpp),
#                        and the ctor uses MAX_SOCKET_TOTAL (MachineType.h via
#                        cprod.h).  ht9045_globals itself links no project
#                        library except vclcompat (line ~359 in this file; the
#                        only other entries are the psapi/version Win32 import
#                        libs at ~365), so this edge introduces no cycle.
#   * Automation/AGV_predicates.cpp is a SOURCE of this target (not a link
#     dependency) -- see "NO UNDECLARED BACK-EDGE" below.
#   The plan's wording is "depends only on vclcompat"; ht9045_globals is added
#   because it is a REAL, already-existing dependency of the moved code (it was
#   satisfied implicitly while the facade sat inside ht9045_sm) and declaring it
#   keeps the layering honest without touching the property that matters -- that
#   ht9045_forms depends on nothing that depends on ht9045_secsgem or ht9045_sm.
#
# NO UNDECLARED BACK-EDGE -- AI(W906-W7-F0-fix) 20260728
#   As first landed, this library had a 4th, UNDECLARED dependency: forms/
#   fAGV.cpp's 3 predicates delegate to the free functions in
#   Automation/AGV_predicates.cpp, which was compiled into ht9045_sm.  So
#   libht9045_forms.a carried 3 undefined references it declared no dependency
#   for -- measured on the built archive with
#     nm --undefined-only libht9045_forms.a
#   which attributed all 3 (AGV_IsSPIL_AMR / AGV_IsATK_AMR / AGV_Use_AMR) to
#   fAGV.cpp.obj.  In archive reality ht9045_forms depended on ht9045_sm, i.e.
#   the "bottom layer" property that justifies the ht9045_secsgem edge below did
#   not actually hold; it only LOOKED like it held because GNU ld extracts
#   fAGV.cpp.obj lazily, so targets that never touch fAGV (e.g.
#   test_config_loaders, tests/CMakeLists.txt:291, which links ht9045_forms
#   WITHOUT ht9045_sm) linked anyway.
#   FIX: Automation/AGV_predicates.cpp is compiled HERE (and REMOVED from
#   ht9045_sm's source list, see that target).  This is sound because:
#     * Its only external symbols are IniConfig (cprod.cpp:50),
#       USE_COVER_TRAYID and CUSTOMER_CODE (cmydef.cpp) -- all in
#       ht9045_globals, which is already a declared dependency above.  It pulls
#       in nothing upward, so no cycle.
#     * It matches GOLDEN's own grouping: golden defines all 3 as TfAGV MEMBER
#       functions inside the fAGV form's own unit (Automation/AGV.cpp:1315 /
#       1327 / 1339, declared Automation/AGV.h:204-206) -- they are form code,
#       not state-machine code.  W5 extracted them to free functions so the SMs
#       could call them without the VCL form; the extraction chose the
#       Automation/ directory, not a different layer.
#     * ht9045_sm PUBLIC-links this target (see its link line), so its own
#       consumer Automation/AGV_PortScan.cpp still resolves them.  No test target
#       is stranded either: grep over tests/CMakeLists.txt finds ZERO
#       $<LINK_GROUP:...> containing ht9045_sm without ht9045_forms.  (The
#       converse DOES occur -- :291 and :2027 have ht9045_forms without
#       ht9045_sm -- and that is exactly the direction the layering permits, and
#       is now genuinely safe rather than accidentally safe.)
#   ht9045_forms remains listed INSIDE the existing $<LINK_GROUP:RESCAN,...>
#   groups because its members reference ht9045_globals, which is listed AFTER
#   it in those groups -- GNU ld is single-pass, so the group's re-scan is what
#   settles that.  MSVC's link.exe re-scans archives on its own, which is why
#   the RESCAN group is a declared no-op there (see the if(MSVC) block at the
#   top of this file).
# ---------------------------------------------------------------------------
add_library(ht9045_forms STATIC
    forms/fMain.cpp
    forms/fLotInfo.cpp
    forms/fSCKART.cpp
    forms/fSortCT.cpp
    forms/fAGV.cpp
    forms/fNote.cpp
    forms/fOffSet.cpp
    forms/fCleaning.cpp
    forms/fShowMessage.cpp
    # AI(W906-W7-L1-Wave0) 20260801: 4 new form stand-ins for the asendic_* tray SM
    # family, landed by the W7-L1 Wave-0 serialized facade pass.  fTrayForm and
    # fOCR are genuinely first homes; forms/fProductionInfo.cpp supplies the first
    # DEFINITION of a global two out-arm variants only ever extern-declared; and
    # forms/fFixAICCD.cpp takes over the fFixAICCD global that aoutarm_shims.cpp
    # used to define as a NULL pointer (see FormsFacade.h for the exact status of
    # each).  Same bottom-layer dependency shape as the 9 above (vclcompat +
    # ht9045_globals only).
    forms/fTrayForm.cpp
    forms/fOCR.cpp
    forms/fProductionInfo.cpp
    forms/fFixAICCD.cpp
    # AI(W906-W7-L2) 20260803: 5 new form stand-ins landed by the W7-L2 substrate
    # pass, in front of the golden ckernel.cpp translation.  All five are FIRST
    # homes -- none of these form types existed anywhere in this tree before.
    #   fShuttleMove -- WaitManualRetryKey (golden ckernel.cpp:133-149) only.
    #   fHome / fSetup / fRotate / fAOI -- ScanSystemSensor (golden :359-691),
    #   which derefs 7 distinct form globals across 15 live sites.
    #   AI(W906-W7-L2) 20260803 CORRECTED: this comment first said fAOI's global is
    #   deliberately NULL.  It is not.  forms/fAOI.cpp:35 allocates a live instance
    #   at static init, deliberately and against the wave brief, because golden
    #   HT9045.cpp:233 creates the form at application start -- so golden's guard
    #   at ckernel.cpp:502 (`if(FrmAOI!=NULL)`) is TRUE on a real machine and the
    #   faithful offline state is a live object.  See forms/fAOI.cpp:19-24.
    # Same layer as their siblings above (plain-data facades over ht9045_globals,
    # no upward edge), so no new link dependency is introduced.
    forms/fShuttleMove.cpp
    forms/fHome.cpp
    forms/fSetup.cpp
    forms/fRotate.cpp
    forms/fAOI.cpp
    # AI(W906-W7-F0-fix) 20260728: MOVED here from ht9045_sm -- the 3 AMR-mode
    # predicates forms/fAGV.cpp delegates to.  See the "NO UNDECLARED BACK-EDGE"
    # block above.  File content unchanged (zero behaviour change: same code,
    # same callers, different archive).
    Automation/AGV_predicates.cpp
)

target_include_directories(ht9045_forms PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_forms PUBLIC vclcompat ht9045_globals)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Same warning posture as ht9045_globals / ht9045_sm: the facade sees the
    # BCB6 idioms inherited from the global headers (AnsiString brace-init etc.)
    # via cprod.h / cmydef.h.
    target_compile_options(ht9045_forms PRIVATE
        -Wall -Wextra -Wno-conversion-null -Wno-write-strings -Wno-unused-parameter)
endif()

# ---------------------------------------------------------------------------
# third_party: sqlite3  (W3 external dependency)
#   The golden ref links a prebuilt sqlite3.lib and ships only sqlite3.h (no
#   sqlite3.c).  Per the migration plan, sqlite3 is an EXTERNAL portable
#   dependency, NOT hand-translated -- cMyDB (the WRAPPER) is what we translate
#   (scheduled W3).  This subdir creates an `sqlite3` target resolved as:
#   vendored amalgamation  ->  system find  ->  header-only stub.
#   No executable links it yet (cMyDB is not translated this wave), so the
#   stub path keeps the build green while documenting the dependency.
# ---------------------------------------------------------------------------
add_subdirectory(third_party/sqlite3)
message(STATUS "sqlite3 resolution: ${SQLITE3_RESOLUTION}")

# ---------------------------------------------------------------------------
# Library: ht9045_db  (W3-cont2 config-table loaders)
#   PARTIAL translation of database.cpp: the Mot_Table.csv / IO_Table.csv
#   parsers (SYSTEM_MODULAR::LoadMotData / LoadIoData + the TMOTDATA / TIODATA
#   row ctors + the name-based column resolvers TMOTNO::SetMOTTableNo /
#   TIOTABLENO::SetIOTableNo).  All other database.cpp regions (TDataModule1,
#   SYSTEM_MODULAR ctor/dtor, ReadGeneralIni, SystemModularInitial body,
#   InstallColorBinDisplay) are gated #if 0 // TODO(wave) inside the .cpp.
#
#   Depends on:
#     * vclcompat       -- AnsiString / TStringList / FileExists / HexStrToInt
#     * ht9045_globals  -- INDEX_MOTION_CARD (cmydef.cpp), the cmydef/MachineType
#                          headers, and TQPF_Timer (myTimer.o) for HSys.SysTimer.
#
#   NOT linked into an executable directly here; the test_config_loaders target
#   links it and supplies the two stubbed externals (MyDBIProcess / ShowMyMessage)
#   that the gated UI/DB waves will provide for real.
# ---------------------------------------------------------------------------
add_library(ht9045_db
    database.cpp
)

target_include_directories(ht9045_db PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_db PUBLIC vclcompat ht9045_globals)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # database.cpp carries the original BCB6 idioms inherited from the global
    # headers (AnsiString brace-init etc. pulled via cmydef/cprod).  Match the
    # ht9045_globals warning posture so the translated loaders compile without
    # tripping on idioms that live in the (already-accepted) global headers,
    # while still flagging genuine porting mistakes in the loader bodies.
    target_compile_options(ht9045_db PRIVATE
        -Wall -Wextra -Wno-conversion-null -Wno-write-strings)
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_motor  (W4 HAL motor layer)
#
#   HTMotor.{h,cpp}    -- abstract port (virtual base, no vendor SDK)
#   mySimMotor.{h,cpp} -- TMySimMotor : offline sim adapter (default brand)
#   mymotor.{h,cpp}    -- TMyMotor / TTrayMotor (PARTIAL: motion-routing core)
#
#   Sim path (default): compiles + links with NO vendor SDK; no HAVE_xxx macros
#   needed.  The brand drivers (SMC/Galil/MN200/SYNTEK/EtherCAT/MC88X1) are
#   deferred to W4-part2 -- their .cpp files are NOT translated yet because they
#   pull the same W6 god-stack (main.h / cinitial.h / mycylin.h / etc.) as
#   mymotor.cpp.  They will be translated with #if HAVE_xxx vendor guards once
#   the W6 sensor layer lands.
#
#   mymotor.cpp pulls sIC_Type[] (cmydef.h), iYRegNum (cmydef.h), bTestSiteUse[]
#   (cmydef.h), HAS_IC/NULL_IC/HAS_NULL_IC (cprod/cmydef), INDEX_MOTION_CARD
#   (cmydef), MOTION_CARD_TYPE (cmydef), ChangeToFloatNonPcnt (MachineType.h),
#   Prod.* (cprod) -- all resolved by ht9045_globals.
#   mytray.h (TMyTray): NOT yet in Motor/ -- include path is project root.
# ---------------------------------------------------------------------------
add_library(ht9045_motor STATIC
    Motor/HTMotor.cpp
    Motor/mySimMotor.cpp
    Motor/mymotor.cpp
    mytray.cpp          # TMyTray: required by TTrayMotor in mymotor.cpp (W4 partial)
)

target_include_directories(ht9045_motor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_motor PUBLIC vclcompat ht9045_globals)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_motor PRIVATE
        -Wall -Wextra -Wno-conversion-null -Wno-write-strings -Wno-unused-parameter)
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_io  (W4-IO HAL IO layer)
#
#   IOBackend.{h,cpp}   -- abstract port TIOBackend (WriteBit/WriteByte/ReadBit/
#                          ReadByte, int<0=fail) + TSimIOBackend (offline in-memory
#                          default, always active) + real backends TMN200Backend /
#                          TPci1203Backend / TMnetLegacyBackend / TRawPortBackend
#                          (bodies behind #if HAVE_MN200 / HAVE_PCI1203 / HAVE_MNET /
#                          (HAVE_RAWPORT && x86), all OFF by default).
#   MyLaneIo.{h,cpp}    -- TLaneIO facade: keeps the 6 BCB6 method names + signatures
#                          + OutPortData cache + CheckPortRangeErr + ret<0 / SOFT_SIMULTE
#                          bookkeeping; routes vendor calls through TIOBackend* pIO.
#                          Default: pIO = new TSimIOBackend() (offline).
#   myswitch.{h,cpp}    -- TMySwitch On/Off/Status/OnOff/CopySwitch; eMotionNet||
#                          ePCI1203 -> MyLaneIO (active); pad/raw branches gated
#                          #if 0 // TODO(W6).
#   mysensor.{h,cpp}    -- TMySensor Status/IsOn/IsOff/CopySensor via MyLaneIO;
#                          same pad gate; raw branch stub.
#
#   Sim path (default): compiles + links with NO vendor SDK; no HAVE_xxx macros
#   needed.  The real backends compile as stubs (each method {return 0;}) and
#   never link to vendor symbols.
#
#   Depends on:
#     * vclcompat       -- AnsiString / windows.h (byte)
#     * ht9045_globals  -- InitialOK, IO_CARD_TYPE, MyBitMask, MachineTypeChoice,
#                          uiDevhand, iPLCSafetyVer, eMotionNet/ePCI1203/etc.
#                          (cmydef.h / MachineType.h / cprod.h resolved here)
#
#   DEFER to W4-IO-part2 / W6 (NOT compiled now):
#     * TMyCylinder Push/Pop/On/Off/SetSimulateCompoment (Alarm/SystemStart/
#       bHandlerPause/TControl god-stack)
#     * TMySucker Suck()/Destroy() state machines (TQPF_Timer/alarm)
#     * Entire TMyKitSuck data grid (TALed/pLed/TMyProductionRecord/TTrayMotor/
#       cprod/MyDBIProcess)
#     * myio.cpp raw-port free funcs (outportb/IOSetOutport/TTL_CARD_TYPE +
#       2-arg IdleCheckSafeDoorByCylinder)
#     * Real backend wiring/selection in cinitial by IO_CARD_TYPE
#     * MyPLC_IO_Modbus/ModbusTCPClient PLC input state
# ---------------------------------------------------------------------------
add_library(ht9045_io STATIC
    IOBackend.cpp
    MyLaneIo.cpp
    myswitch.cpp
    mysensor.cpp
    # --- W6.0: TMyCylinder HAL piece (the one real substrate gap the W6.1
    #   Empty-tray canary needs).  The W4-IO note above DEFERRED TMyCylinder
    #   Push/Pop/On/Off to "W4-IO-part2 / W6"; this is that wave.  Routes over
    #   the Sim IO HAL (MyLaneIO) exactly like myswitch/mysensor; the alarm /
    #   SmartDiagnostic / VCL-widget god-stack is stubbed/gated in mycylin.cpp.
    mycylin.cpp
)

target_include_directories(ht9045_io PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_io PUBLIC vclcompat ht9045_globals)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_io PRIVATE
        -Wall -Wextra -Wno-conversion-null -Wno-write-strings -Wno-unused-parameter)
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_secsgem  (W5 SECSGEM base layer + Sim EventReport seam)
#
#   uHGemClass.{h,cpp}   -- HTGem: abstract SECS/GEM S,F-handler base (~90
#                           virtuals; the ~33 golden already left fully inline
#                           carry verbatim, the 57 that need THGem/uHGemEquipment
#                           (out of scope) are #if 0-gated with a conservative
#                           ACTIVE stub -- link-complete today).
#   SecsEventType.{h,cpp}-- SECS_EVENT (ETypeStruct): the CEID enum extracted
#                           standalone from uHGemHT9045.h, verbatim values.
#   SecsEventReport.{h,cpp} -- NEW (integrate-agent addition, not a golden
#                           translation): a minimal Sim-first free function
#                           `EventReport(unsigned Ceid)` that gives csystem.cpp's
#                           W7C1_EVENTREPORT_*/W7C2_EVENTREPORT_* seam macros a
#                           REAL, offline-safe call target (records last-CEID +
#                           a running count) instead of a no-op.  Deliberately
#                           does NOT instantiate HTGem -- see its file header:
#                           HTGem::~HTGem() needs a 3-arg MyDBIProcess overload
#                           that only tests/test_uHGemClass.cpp stubs today, and
#                           forcing every ht9045_sm-linked executable to resolve
#                           that too is not worth it for a Sim counter.
#
#   ht9045_sm links this library so csystem.cpp can call EventReport()/read
#   SECS_EVENT directly; only SecsEventType.o + SecsEventReport.o are ever
#   extracted from this archive for that link (uHGemClass.o stays a dormant,
#   unreferenced archive member there -- same "compiled but not pulled in"
#   shape as other inert files elsewhere in this tree). tests/test_uHGemClass
#   links this library directly to exercise HTGem itself.
#
#   SecsWireCodec.{h,cpp} -- W5-Final: THGem's pure byte-level SECS-II item
#     codec slice (GetLengthOfType/GetLengthByte/GetSMLLenthByte/DataItemOut
#     x2/DataItemInSub/DataItemIn x2/DataItemInNew/GetDataItemLenAndType(Sub)/
#     StringOut), plus the shared `HType`/HTypeStruct global (the canonical
#     home for HTypeStruct/STypeStruct/HSMS_Head_Struct -- see its own file
#     header). Zero dependency beyond vclcompat.
#   SecsSvEcRegistration.{h,cpp} -- W5-Final: THGem's SV/EC registration API
#     (SetSVDataPointer x4 / SetECDataPointer x4 / GetECDataValue). A
#     deliberately SEPARATE class from SecsWireCodec (reuses its `HType`
#     global via #include, does not extend it -- see that file's own
#     "DESIGN: SEPARATE CLASS" note); depends on SecsWireCodec.h + vclcompat/
#     TList.h (vclcompat::TList, used qualified -- never brought into the
#     global namespace, per that shim's own ODR note re aHotPlateSubstrate.h).
#   Both slices are prerequisites -- not yet consumers -- of a FUTURE full
#   THGem/uHGemEquipment.cpp translation wave (out of scope here); see each
#   file's own "INTEGRATE-AGENT WIRING POINT" note.
#
#   SECSGEM/uHGemEquipment.{h,cpp} -- W906-uHGemEquipment INTEGRATE: THGem's
#     FIRST real slice (a deliberately partial class -- see that header's own
#     file-head note for exactly what is/isn't modeled yet). This wave's slice
#     is the CEID/Report/Alarm StringGrid "database" family (SetCEIDContent/
#     GetCEIDContent/DeleteReportID/.../CopyStringGridAsTabFormat/SetAlamData/
#     ReadAlamData/WriteAlamData, 26 methods) + the standalone GemTimer utility.
#     Depends on vclcompat + the new vclcompat/StringGrid.h shim (see above) +
#     SECSGEM/SecsEventType.h (SECS_EVENT.TotalEvent array bound). Deliberately
#     does NOT yet embed SecsWireCodec/SecsSvEcRegistration (InitLocalHead/
#     DataItemOut/GetDataItemLenAndType/SetSVDataPointer/... are still OUT of
#     THGem's scope this wave -- a future wave should add them as by-value
#     members, matching HTGem's own already-proven WireCodec/SvEcReg pattern;
#     see uHGemEquipment.h's own "MEMBERS DELIBERATELY NOT PRESENT YET" note).
#     INTEGRATE CHECK (this wave): confirmed zero ODR overlap with
#     uHGemClass.cpp/SecsWireCodec.cpp's own GetDataItemLenAndTypeAndDelete(Sub)
#     -- uHGemEquipment.cpp does not define or call either; THGem does not yet
#     model that method at all. No production consumer wired yet (HTGem's
#     `THGem *HGemPtr` in uHGemClass.h stays a forward declaration -- see that
#     header's own comment on why an #include swap was evaluated and declined
#     this wave: every currently-gated HTGem method needs at least one THGem
#     member/method still absent from this partial slice, so switching to a
#     full #include would add a large new dependency for zero un-gating
#     benefit today).
#
#   AI(W906-uHGemEquipment-BucketB) 20260717: THIRD wave on this same file --
#     ~15 small widget stand-ins (THGemRadioGroup/THGemEdit/THGemCheckBox/
#     THGemComboBox/THGemPanel/THGemSpeedButton/THGemMemo, uHGemEquipment.h) +
#     InitialHGem/SaveSystemDefault/DoUpdateStatus/ProcessShow/FormClose/
#     ManualCreatergRoleClick/FormShow/StringOut(2-arg). DEPENDENCY WIDENING:
#     this slice needs CUSTOMER_CODE/CC_KYEC_LEE/CC_SIGURD_ChungXing/
#     CC_MAXIM_THAILAND/CosFunction/bSECSGEMbyPass/bSECSGEMConnectionFail
#     (cmydef.h -> ht9045_globals) and ReadWriteIni/ReadIniData/WriteIniData
#     (common.h -> ht9045_core) -- see uHGemEquipment.cpp's own top-of-file
#     note for the full reasoning (common.h confirmed to stay lightweight:
#     ini-helper family only, does not itself drag in the rest of golden's
#     common.h). Same "translated unit needs the global/ini-helper families"
#     shape already established by KYECFTP/FTPClient_Transfer.cpp's own
#     ht9045_core+ht9045_globals link (see that library's own comment above).
#     No circular dependency: ht9045_core/ht9045_globals each only link
#     vclcompat (+psapi/version for ht9045_globals on WIN32), neither links
#     back to ht9045_secsgem or ht9045_public.
# ---------------------------------------------------------------------------
add_library(ht9045_secsgem
    SECSGEM/uHGemClass.cpp
    SECSGEM/SecsEventType.cpp
    SECSGEM/SecsEventReport.cpp
    SECSGEM/SecsWireCodec.cpp
    SECSGEM/SecsSvEcRegistration.cpp
    SECSGEM/uHGemEquipment.cpp
    # AI(W906-SvEcDataItem) 20260720: TasmInfo -- THGem::FormCreate's SV10-19
    # (CPU/disk/memory) source functions. New, standalone (vclcompat only),
    # zero new link edge (same archive as uHGemEquipment.cpp, which calls it).
    SECSGEM/TasmInfo.cpp
    # AI(W906-uHGemHT9045-Bucket0) 20260721: HT9045Gem's permanent home
    # (supersedes the deleted uHGemHT9045_Shim.cpp, W906-SysModWire) -- the
    # ONLY definition of `HT9045Gem`, needed by database.cpp's now-real
    # SystemModularInitial (`new HT9045Gem(...)`). See that header's own
    # file-head note for scope (still zero of golden's 22 overrides).
    SECSGEM/uHGemHT9045.cpp
)

target_include_directories(ht9045_secsgem PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/SECSGEM
)

# AI(W906-uHGemEquipment-ConnLifecycle) 20260717: THGem::GetSocketErrorMsg
# (uHGemEquipment.cpp, golden :6842-6845) is a thin forward to the already-real
# GetErrorMsg(TObject*, int) -- its one implementation is
# Public/WinSocketErrorCode.cpp, which lives in the ht9045_public library (see
# that library's own target above). GetErrorMsg only reads <winsock2.h>
# constants (no WSA*/socket function calls -- see that .cpp's own header
# note), so this adds no further transitive link requirement (no ws2_32 etc.).
# AI(W906-uHGemEquipment-BucketB) 20260717: ht9045_core (common.h's
# ReadWriteIni/ReadIniData/WriteIniData) + ht9045_globals (cmydef.h's
# CUSTOMER_CODE/CC_*/CosFunction/bSECSGEMbyPass/bSECSGEMConnectionFail) added
# -- see this target's own comment above for the full reasoning.
# AI(W906-uHGemEquipment-BucketC) 20260717: ht9045_db added -- D3's HSys.MyGem
# seam (`#include "database.h"`, `extern SYSTEM_MODULAR HSys;`) needs
# database.cpp's one definition of `HSys` linked in. ht9045_db's OWN
# target_link_libraries declaration only lists vclcompat+ht9045_globals (see
# that library's own target above) -- no CMake-target-graph cycle.
#
# AI(W906-SysModWire) 20260720: a real (object-file-level, not CMake-target-
# level) circular SYMBOL dependency now exists as of this wave: database.cpp's
# SystemModularInitial calls `new HT9045Gem(...)`, a symbol this library
# (ht9045_secsgem) defines (uHGemHT9045.cpp.o as of W906-uHGemHT9045-Bucket0,
# 20260721; was uHGemHT9045_Shim.cpp.o before the swap-back) -- so ht9045_db's
# database.cpp.o now has an undefined reference back INTO this archive, even
# though ht9045_db's CMake target never declares ht9045_secsgem as a
# dependency. Any executable that ends up needing BOTH archives (this one,
# for HSys via ht9045_db) must link them through a `$<LINK_GROUP:RESCAN,...>`
# (or equivalent --start-group/--end-group) wrapper, not a plain sequential
# link -- see tests/CMakeLists.txt's test_uHGemEquipment/test_uHGemClass/
# test_config_loaders (the three targets that actually pull database.cpp.o;
# independent review verified via nm that only these executables contain
# _HSys -- the motor/io targets linking ht9045_db never pull database.cpp.o,
# so they need no ht9045_secsgem edge) for the applied fix.
# Deliberately NOT making ht9045_db declare
# `target_link_libraries(ht9045_db PUBLIC ht9045_secsgem)` here -- that would
# invert this library's intended layering (ht9045_db is a lower layer than
# SECS/GEM) for a dependency that is a database.cpp implementation detail, not
# ht9045_db's own public interface.
# AI(W906-W7-F0) 20260728: ht9045_forms ADDED -- this single edit is what
# actually unblocks "Wall 2".  SECSGEM/uHGemHT9045.cpp touches 28 distinct fMain
# members in golden; it could not reach ANY of them before, because the facade
# lived in ht9045_sm and adding ht9045_sm here would have made a CMake cycle
# (ht9045_sm already links this target -- see target_link_libraries(ht9045_sm
# PUBLIC ...) below).  ht9045_forms is a bottom layer (vclcompat +
# ht9045_globals only), so this edge is acyclic.
# AI(W906-W7-F0-fix) 20260728: that acyclicity now has a POSITIVE control, not
# just "the configure succeeded" -- re-injecting the ht9045_secsgem->ht9045_sm
# edge into this exact tree makes configure FAIL with a strongly-connected-
# component error on test_config_loaders and test_uHGemEquipment, while the tree
# without it configures clean.  Both runs are recorded in the ht9045_forms block
# above ("THE EXACT MECHANISM"), including why a cycle among STATIC libraries is
# normally legal and why THIS one is not.
target_link_libraries(ht9045_secsgem PUBLIC vclcompat ht9045_public ht9045_core ht9045_globals ht9045_db ht9045_forms)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_secsgem PRIVATE ${HT9045_WARNFLAGS})

    # AI(W5-SECSGEM-Integrate) 20260711: uHGemClass.h/.cpp carry 2 golden BCB6
    # idioms (an intentionally-unused virtual-override parameter; a trailing
    # ';' after a ctor body) -- same class of carve-out already accepted for
    # cContact.cpp. Narrowly suppress just these two warning classes on this
    # file; every other warning class (-Wall -Wextra -Wshadow -Wconversion)
    # still applies in full.
    set_source_files_properties(SECSGEM/uHGemClass.cpp PROPERTIES COMPILE_FLAGS
        "-Wno-pedantic -Wno-unused-parameter")
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_automation  (W5 Automation wave -- RENESAS host-protocol data shapes)
#
#   Automation/uRENESAS_Other.{h,cpp} -- ~22 plain-old-data record/reply classes
#     (TRENESAS_OnLine/_ProdInfo/_ModeStart/_HandlerStart/_ModeEnd/_LotEnd/_TestEnd/
#     _SendAlarm/_SendCleanOut + their _Reply counterparts + the aggregate
#     TRENESAS_RECV_CMD_DATA/_Send_CMD_DATA containers), the TRENESAS_CMD /
#     TRENESAS_Status_Code / TRENESAS_Err_Index command/status/error-code constant
#     tables, and TRENESAS_EndFileData (the only class with real logic). Zero
#     hardware/VCL/global coupling -- pure data shapes prerequisite for the FUTURE
#     Automation/uRENESAS_Server.cpp wave (not yet scheduled, not yet a consumer),
#     so this gets its own small standalone library (matching the unit's own
#     translate report) rather than being folded into ht9045_sm's larger link
#     surface for no reason.
# ---------------------------------------------------------------------------
add_library(ht9045_automation STATIC
    Automation/uRENESAS_Other.cpp
)

target_include_directories(ht9045_automation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_automation PUBLIC vclcompat)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_automation PRIVATE ${HT9045_WARNFLAGS})
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_comms  (W5 comms wave -- CCLink / CanBus / MyPLC / EtherCAT)
#
#   CCLink/MyCCLink.{h,cpp}       -- TMyCCLinkCtrl (top-level comm state machine)
#   CCLink/MyCCLinkBoard.{h,cpp}  -- TMyCCLinkBoard (Mdfunc vendor board wrapper;
#                                    real vendor calls behind #if HAVE_CCLINK,
#                                    default OFF -- mirrors IOBackend.cpp's
#                                    HAVE_MN200/HAVE_PCI1203/HAVE_MNET).
#   CCLink/MyCCLinkSensor_predicates.{h,cpp} -- UseCanBusOrEtherCAT/UseSensorMod3.
#   CanBus/cMyNUDN1.{h,cpp}       -- TMyNUDN1 (NUDN1 DeviceNet amplifier station
#                                    wrapper); real vendor calls behind
#                                    #if HAVE_DNM100, default OFF.
#   MyPLC/ModbusTCPClient.{h,cpp} -- Modbus-TCP-ish framing client over
#                                    vclcompat/ClientSocket.h (above).
#   MyPLC/MyPLC_IO_Modbus.{h,cpp} -- PLC safety-IO polling (InitPLCIO/
#                                    PLCIOTaskCycle/PLCStatusCheck) + TPLCIOThread
#                                    (offline-inert driver, no real OS thread).
#   EtherCAT/MyNUEC1.{h,cpp}      -- TMyNUEC1 (FS-N12 EtherCAT current-sense
#                                    amplifier slave); real vendor calls behind
#                                    #if HAVE_PCI1203 (REUSES the flag
#                                    IOBackend.cpp/MyLaneIo.cpp already
#                                    established for the same Advantech API
#                                    family), default OFF.
#
#   Interface/InterfaceSYS.cpp is DELIBERATELY NOT part of this library: its
#   own translate report found it both depends on (TestSocket/LastSet/
#   FormsFacade) and unblocks (atester.cpp's GetTesterResult/ProcessTesterTimeOut)
#   symbols that live in ht9045_sm -- making it a separate library here would
#   create a circular ht9045_comms<->ht9045_sm CMake target dependency (CMake
#   rejects cycles in the target link graph, unlike the linker's own archive
#   re-scanning). It is instead compiled directly into ht9045_sm's own source
#   list (see that library below) -- the integrate agent's own resolution of
#   this unit's flagged conflict.
#
#   No cross-unit archive-selection conflicts: the four units here never
#   reference each other's symbols (verified by grep during integrate); each
#   unit's own dedicated test below links this whole library, and only that
#   unit's own translation units are ever extracted from the archive.
# ---------------------------------------------------------------------------
add_library(ht9045_comms STATIC
    CCLink/MyCCLink.cpp
    CCLink/MyCCLinkBoard.cpp
    CCLink/MyCCLinkSensor_predicates.cpp
    CanBus/cMyNUDN1.cpp
    MyPLC/ModbusTCPClient.cpp
    MyPLC/MyPLC_IO_Modbus.cpp
    EtherCAT/MyNUEC1.cpp
)

target_include_directories(ht9045_comms PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_comms PUBLIC vclcompat ht9045_globals)

if(WIN32)
    target_link_libraries(ht9045_comms PUBLIC ws2_32)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # BCB6-idiom-heavy new comms code (narrowing int->short/byte at vendor call
    # boundaries, empty golden event handlers, etc.) -- match the ht9045_io /
    # ht9045_motor relaxed posture rather than ht9045_core's strict one (none of
    # these files is the "genuinely idiom-free" case that earned cContact.cpp
    # its stricter flags).
    target_compile_options(ht9045_comms PRIVATE
        -Wall -Wextra -Wno-conversion-null -Wno-write-strings -Wno-unused-parameter)
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_sm  (W6 state-machine wave -- W6.1 Empty-tray CANARY)
#
#   The first ROOT state machine translated over the W6 substrate, plus the
#   W6 decoupling scaffold that lets it stand alone:
#
#     asendic_Empty.cpp     -- the Empty-tray stack-feed SM (CANARY).  Faithful
#                              switch(Task) steppers; each Do* owns its cursor int.
#     csystem_predicates.cpp-- the frozen csystem.h HasIC predicate interface
#                              (implemented over the Sim HAL; canary uses none,
#                              but they link the contract for later sub-waves).
#     (FormsFacade.cpp)     -- WAS here: the non-VCL fAGV satellite stub (the
#                              canary's only form-pointer deref,
#                              fAGV->IsATK_AMR()).  AI(W906-W7-F0) 20260728
#                              moved it to forms/fAGV.cpp in the new
#                              ht9045_forms library (see its block above);
#                              this target links ht9045_forms, so the canary is
#                              unaffected.
#     asendic.cpp           -- SIM bodies for CylinderUp/Middle/Lower + TrayMove*
#                              tray-move/lifter helpers (manipulate Cylinder[]/
#                              MOT[]; full lifter SM deferred W6.x).
#     canary_support.cpp    -- minimal LastSet shim + sim bodies for the free
#                              functions the canary calls that have no shared
#                              target header (ShowErrorMessage/RecordProcess/
#                              ShowUnloaderTrayMessage/WhichAutoNeedTray/
#                              ShowMyMessage/ReadWriteTrayID).
#
#   Depends on the full substrate: ht9045_io (Cylinder[]/SW[]/Sen[]/MyLaneIO),
#   ht9045_motor (MOT[]), ht9045_globals (Prod/IniConfig/CosFunction/TrayForm/
#   RunInfo/Ld_UldDelayTime/TrayID/enums), ht9045_db (database.cpp -- pulled by
#   the RESCAN group for cross-archive ctor resolution), vclcompat.
# ---------------------------------------------------------------------------
add_library(ht9045_sm STATIC
    asendic_Empty.cpp
    # AI(W906-W7-L1) 20260729: asendic_Auto2.cpp -- the Auto2-tray stack-feed SM,
    # first landing of the W7-L1 asendic_* feed-SM family (see plan SS6 W7-L1).
    # Verified zero outgoing cross-file calls into the other 6 asendic_* siblings
    # (Loader/Loader_RT/Scanner/Auto/Auto_RT/Color), so it is safe to land
    # standalone in the same library asendic_Empty.cpp already occupies (same
    # substrate: Cylinder[]/MOT[]/Sen[]/SW[]/canary_support/FormsFacade fAGV).
    asendic_Auto2.cpp
    # AI(W906-W7-L1-Wave1) 20260802: the W7-L1 Wave-1 landings.  All three join the
    # same library and the same Sim HAL substrate asendic_Empty.cpp / asendic_Auto2.cpp
    # already occupy (Cylinder[]/MOT[]/Sen[]/SW[] + canary_support + FormsFacade +
    # acatchtray_shims); NO new link dependency was introduced by either.
    #
    #   asendic_Color.cpp      -- the Color-tray stack-feed SM.  Verified by two
    #     independent recons and re-checked by its own wave: ZERO outgoing calls into
    #     any asendic_* sibling, so it is the family's only true leaf.  It is also the
    #     PREREQUISITE for Wave 2's asendic_Auto.cpp, which calls this file's
    #     ForTERAPOWERCheckColorSensor (golden asendic_Auto.cpp:363/:2029/:2074).
    #
    #   asendic_Loader.cpp + asendic_Loader_RT.cpp -- the Loader bundle.  These two
    #     MUST land together: they are a genuine bidirectional cycle (golden
    #     asendic_Loader.cpp:1973/:1987 call into _RT, and _RT.cpp:190 calls back into
    #     asendic_Loader.cpp:3167 CheckLoaderICFloating), so neither compiles as a
    #     standalone unit of work.
    #
    # LANDED ATOMICALLY WITH FIVE SHIM-BODY RETIREMENTS (see the SHIM RETIREMENT DEBT
    # REGISTER banners in acatchtray_shims.h and csystem_shims.h, where these are now
    # marked DONE): acatchtray_shims DoAutoColor + InitAutoColorReceiveTask, and
    # csystem_shims InitAutoColorTask + DoLoad + InitLoadTask.  Earlier would have
    # given undefined symbols, later duplicate symbols.  DoAutoColor's DECLARATION was
    # additionally corrected bool -> void, which is what golden says
    # (golden asendic_Color.h:20); all ported call sites discarded the value.
    asendic_Color.cpp
    asendic_Loader.cpp
    asendic_Loader_RT.cpp
    # AI(W906-W7-L1-Wave2) 20260802: the W7-L1 Wave-2 landings -- the LAST two SM
    # files of the asendic_* family.  Same library and Sim HAL substrate as the five
    # units above; no new link dependency.
    #
    #   asendic_Auto.cpp     -- the Auto-tray (automatic sort-output) stack-feed SM
    #     for Auto1..Auto6.  2,561 golden lines, of which ONE function
    #     (DoAutoReceiveBinTray) is 2,189 lines across 56 cases.  ORDER MATTERS: it
    #     must follow asendic_Color.cpp, whose ForTERAPOWERCheckColorSensor it calls
    #     (golden asendic_Auto.cpp:363/:2029/:2074).
    #
    #   asendic_Auto_RT.cpp  -- the Auto1..Auto6 unloader-tray ROUND-TRIP
    #     (auto-retest) variant.  Lands standalone: zero facade members and zero shim
    #     retirements of its own (proven with `nm -g --defined-only` over all ten
    #     archives).  It also SUPPLIES the two definitions that asendic_Auto2.h and
    #     asendic_Auto.h already extern-declare as "owned by asendic_Auto_RT.cpp"
    #     (InitUnLoadNewAutoTrayTask / DoUnLoadNewAutoToStack), so those stop being
    #     orphan declarations.
    #
    # LANDED ATOMICALLY WITH the Auto track's two shim-body retirements
    # (acatchtray_shims DoAutoReceiveBinTray, csystem_shims Initial_Auto_BinTray_Task),
    # its bool -> void declaration correction, and the TfTrayMapping::DoCoverTrayID
    # signature fix -- see the DEBT REGISTER banners.  That last one changes a mangled
    # name, so this landing needs a FULL rebuild, not an incremental one.
    asendic_Auto.cpp
    asendic_Auto_RT.cpp
    csystem_predicates.cpp
    # AI(W906-W7-F0) 20260728: FormsFacade.cpp REMOVED from this library -- the
    # facade now compiles into the bottom-layer ht9045_forms target (see its
    # block above for why: having it here was the actual "Wall 2" CMake cycle).
    # ht9045_sm links ht9045_forms below, so nothing in this library loses access
    # to fMain/fLotInfo/... .
    asendic.cpp
    canary_support.cpp
    cinitial.cpp          # AI(W906-GA2-C1) 20260804: block 1 (golden :77-3060) -- Sucker/Switch/Sensor names+init
    # --- W6.2 (IN-ARM foundation): shared HotPlate pick/place geometry leaves ---
    #   ainarm_SearchPickPlate.cpp  -- HP-pick geometry + DoInArmPickFromHotPlate_9045 SM
    #   ainarm_SearchPlacePlate.cpp -- HP-place geometry + search-planner + data-swap leaves
    #   aHotPlateSubstrate.cpp      -- the W6.2 in-arm substrate the leaves pump
    #                                  (InArmSuck/FLCarryKit/.. TMyKitSuck grid +
    #                                   PickFromHPList uPlateInfo + ainarm2 cursors +
    #                                   in-arm engine helper stubs + tRotate/Laser)
    ainarm_SearchPickPlate.cpp
    ainarm_SearchPlacePlate.cpp
    aHotPlateSubstrate.cpp
    # AI(W906-Reconcile) 20260721: Public/MyProductionRecord.cpp's real pure-logic
    # bodies (ctor + ~28 leaves) supersede the offline no-op stand-ins
    # aHotPlateSubstrate.cpp used to carry for the same 8 methods (removed there,
    # see that file's own comment) -- lives in ht9045_sm because that's what
    # aHotPlateSubstrate.cpp/AutoClean.cpp (both in this target) call into.
    Public/MyProductionRecord.cpp
    # --- W5-comms INTEGRATE: Interface/InterfaceSYS.cpp (WM_COPYDATA IPC bridge
    #     to the GPIB/Tester bridge / ESD / Auto-Update / Event-Log-Analyzer
    #     helper programs).  Lives HERE, not in ht9045_comms, per that unit's own
    #     translate report: it depends on TestSocket/LastSet (aHotPlateSubstrate.
    #     cpp/canary_support.cpp, just above) + fMain/fBarCode (was
    #     FormsFacade.cpp here; AI(W906-W7-F0) 20260728 moved it to
    #     forms/fMain.cpp in ht9045_forms, which this target links)
    #     and in turn unblocks atester.cpp's still-gated GetTesterResult/
    #     ProcessTesterTimeOut (both also in this library) -- a standalone
    #     library here would create a circular ht9045_comms<->ht9045_sm target
    #     dependency.  See CMakeLists.txt's ht9045_comms comment block.
    Interface/InterfaceSYS.cpp
    # --- W5-Final-TesterTCPSocket INTEGRATE: Interface/TesterTCP_Socket.cpp
    #     (bounded socket-management subset of Interface/TesterTCP.cpp --
    #     Connect/Disconnect/Error/Read handlers, the 30-tick reconnect timer,
    #     SendTCPIPCommand/AddTCPIPCommunicationLog). Same rationale as
    #     InterfaceSYS.cpp just above: compiled directly into ht9045_sm (not a
    #     separate library) since it depends on FormsFacade/canary_support/
    #     common already carried here. Depends on vclcompat/ClientSocket.h's
    #     TClientSocket/TCustomWinSocket/TErrorEvent shim (already present on
    #     disk from an earlier front in this integrate sequence) + the
    #     StringReplace/TReplaceFlags addition this unit made to
    #     vclcompat/SysUtils.h/.cpp (additive only, no hard-boundary file touched).
    Interface/TesterTCP_Socket.cpp
    # --- W6.2b (IN-ARM engine): the central pick SM + iInArmType dispatch
    #     ladders + pure pitch/offset geometry + 3 pumpable task SMs.  Variant
    #     dispatch bodies + dense MOT[]/sensor/UI SMs are gated (#if 0) with
    #     active stubs; see the file-head gate documentation.
    ainarm9045.cpp
    # --- W7-A1 INTEGRATE: offline link stubs for the shuttle-motion engine seam
    #     symbols the un-gated in-arm floating/latch PAIR references (golden
    #     ainarm2.h: DoShakeShuttle/DoKnockShuttle/DoKnockShuttleFirst/
    #     DoVibrateShuttle/RecordShtSuperflous + IndexZCanMove[2]/bShuttleKnock).
    #     IsTestZ1/Z2NotSafe... are NOT here (defined in acarry.cpp).
    ainarm9045_w7_shims.cpp
    # --- W6.2b-canary (IN-ARM site variants): 3 per-site-config variant files
    #     translated faithfully from the golden BCB6 tree ROOT (mirror name/path).
    #       ainarm9045_1x1_1.cpp -- single-site 1x1_1 (THE buildable, live arm;
    #                               defines DoInArm_9045_1x1_1 + _SuckerMap, called
    #                               by the un-gated dispatch arm in ainarm9045.cpp).
    #       ainarm9045_2x4_16.cpp + _shims -- 16-site variant; faithful translation
    #                               but DEAD by the golden engine design (no
    #                               e9045_2x4_16 enum, no dispatch arm; references a
    #                               file-local NN-site map -- golden member absent by
    #                               design).  Registered so it compiles/links; never
    #                               dispatched.
    #       ainarm9045_2x8_32.cpp -- 32-site variant; whole golden body GATED (needs
    #                               Prod.fInArmSuckUse_TwoArm32Site, absent from
    #                               golden cprod.h by design) + the golden engine
    #                               routes e9045_2x8_32 -> 2x8_8 (DEAD).  Active
    #                               offline stubs make the TU link; never dispatched.
    ainarm9045_1x1_1.cpp
    ainarm9045_2x4_16.cpp
    ainarm9045_2x4_16_shims.cpp
    ainarm9045_2x8_32.cpp
    # --- W6.2c batch-1 (5 IN-ARM site variants made ACTIVE; dispatch arms pulled
    #     out of the "#if 0 // TODO(W6.2b variants)" gate in ainarm9045.cpp):
    ainarm9045_1x3_4.cpp
    ainarm9045_1x2_2.cpp
    ainarm9045_1x2_2_14.cpp
    ainarm9045_1x2_4_Hot.cpp
    ainarm9045_1x3_2_14.cpp
    # --- W6.2c batch-2 (6 IN-ARM site variants made ACTIVE; dispatch arms pulled
    #     out of the "#if 0 // TODO(W6.2b variants)" gate in ainarm9045.cpp).
    #     2x2_4 / 2x2_4_14 also drove the substrate add of TMyKitSuck::
    #     ArmDownSideAllTypeIC (golden MyKitSuck.h:335 -> aHotPlateSubstrate).
    ainarm9045_1x4_2.cpp
    ainarm9045_1x4_8_Hot.cpp
    ainarm9045_2x2_4_12.cpp
    ainarm9045_2x1_2.cpp
    ainarm9045_2x2_4.cpp
    ainarm9045_2x2_4_14.cpp
    # --- W6.2c batch-3 (6 IN-ARM site variants made ACTIVE; dispatch arms pulled
    #     out of the "#if 0 // TODO(W6.2b variants)" gate in ainarm9045.cpp).
    #     1x4_4 also drove the substrate add of cSiteUseManager (SiteUseMgr) +
    #     RecordErrorLog (see substrate note).  All_1Pick's DoInArm arm is the
    #     FIRST ladder arm (USE_PICKER_COUNT==ep1Picker), and its *_SuckerMap()
    #     is declared-but-NEVER-dispatched by the golden engine (faithful).
    ainarm9045_1x4_4.cpp
    ainarm9045_1x4_4_Back.cpp
    ainarm9045_2x3_6.cpp
    ainarm9045_2x3_6_14.cpp
    ainarm9045_2x4_4.cpp
    ainarm9045_All_1Pick.cpp
    cSiteUseManager.cpp
    # --- W6.2c batch-4 (5 IN-ARM site variants made ACTIVE; dispatch arms pulled
    #     out of the "#if 0 // TODO(W6.2b variants)" gate in ainarm9045.cpp).
    #     2x2_8_Hot drove the substrate add of TMyKitSuck::SetNullIcToHasNullIc
    #     (golden MyKitSuck.h:280/.cpp:340) + the engine predicate RowCanDualSite
    #     (golden ainarm2.cpp:1554, offline false).  2x8_8 drove the offline homes
    #     of IsMoveInArm2XYToShuttle2Wait (golden ainarm2.h:142 -> acatchtray_shims)
    #     + XPHSuckToSht_2x8_8_OutArm (golden aoutarm9045_2x8_8.h:7 -> substrate).
    #     The golden engine routes e9045_2x8_32 -> DoInArm_9045_2x8_8 (the LIVE
    #     family), so the DEAD ainarm9045_2x8_32.cpp (line 419) stays compiled-but-
    #     never-dispatched -- do NOT remove it.  iCloseSiteModeFor2x6/2x8 +
    #     iCloseSiteStep_2x8 placeholder definitions in aHotPlateSubstrate.cpp /
    #     acatchtray_shims.cpp were removed (the variant .cpp own the real defs).
    ainarm9045_2x2_8_Hot.cpp
    ainarm9045_2x4_8.cpp
    ainarm9045_2x5_8.cpp
    ainarm9045_2x6_8.cpp
    ainarm9045_2x8_8.cpp
    # --- W6.2c batch-5 (2 S-family HT-9045S in-arm variants made ACTIVE; dispatch
    #     arms un-gated in BOTH ladders of ainarm9045.cpp).  IRREGULAR func names:
    #     ainarm9045S_1x4_4.cpp defines DoInArm_9045S_1x4_4(+_SuckerMap) (S in func);
    #     ainarm9045S_2x4_4_13.cpp defines DoInArm_9045_2x4_4_13(+_SuckerMap) (NO S).
    ainarm9045S_1x4_4.cpp
    ainarm9045S_2x4_4_13.cpp
    # --- W6.2b-canary (IN-ARM shared helpers): the two ainarm2.cpp leaves the
    #     site-variant SMs call but that had no body yet -- SetInArmUseSuckToHas
    #     NullIC (golden ainarm2.cpp:201) + InitInArmPickFromHotPlateTask (golden
    #     :623), FAITHFUL pure-grid/cursor logic over the Sim HAL.  Also hosts the
    #     single ACTIVE definition of bDestoryOnSht (cmydef.cpp's is gated #if 0).
    ainarm2.cpp
    # --- W6.2c (OUT-ARM engine): the central place-to-Auto SM + iInArmType
    #     dispatch ladders (DoOutArm_9045 / DoPickFromShuttle_9045) + pure
    #     pitch/offset geometry + 4 pumpable task SMs (DoOutArmAdditionalFunction /
    #     DoOutArmPlaceToAuto_9045 / DoOutArmAfterPlaceToAuto / DoOutArmIonFanGiveWay).
    #     Variant dispatch bodies + dense MOT[]/sensor/Magazine/AOI/Rotate SMs are
    #     gated (#if 0) with active stubs; see the file-head gate documentation.
    #     Pumps the same aHotPlateSubstrate (OutArmSuck/FRCarryKit/BRCarryKit/
    #     OutArm2Suck + out-arm cursor ints) as the in-arm engine.
    aoutarm9045.cpp
    # --- W6.2c-OUT batch (6 OUT-ARM site variants made ACTIVE; dispatch arms
    #     pulled out of the "#if 0 // TODO(W6.2b variants)" gate in aoutarm9045.cpp
    #     + their 2 static no-op stubs removed).  All 6 route to a LIVE callee per
    #     the golden ENUM->CALLEE map.  These drove the substrate adds of
    #     TMySucker::GetNeedSuckStatus/SetNeedSuck/bNeedSuck (golden MyKitSuck.h:
    #     119/121/123) + TMyKitSuck::SetUnuseAndHasNullICToNullIC (golden
    #     MyKitSuck.cpp:369) in aHotPlateSubstrate.{h,cpp}.
    aoutarm9045_1x1_1.cpp
    aoutarm9045_1x2_2.cpp
    aoutarm9045_1x2_4.cpp
    aoutarm9045_1x3_2_14.cpp
    aoutarm9045_1x3_4.cpp
    aoutarm9045_1x4_2.cpp
    # --- W6.2c-OUT batch-2 (7 more OUT-ARM site variants made ACTIVE; LIVE mode):
    #     1x4_4S/1x4_4/1x4_8/2x1_2/2x2_4/2x2_8/2x3_6_14.  Their dispatch arms were
    #     already active in aoutarm9045.cpp; here their 2 static no-op stubs are
    #     removed + their 2 extern decls un-gated.  All route to a LIVE callee per
    #     the golden ENUM->CALLEE map.  Drove the substrate add of CheckDuplicateBarCode
    #     (golden aoutarm.h:130, offline true) in aoutarm_shims.cpp -- used by 1x4_4.
    aoutarm9045_1x4_4S.cpp
    aoutarm9045_1x4_4.cpp
    aoutarm9045_1x4_8.cpp
    aoutarm9045_2x1_2.cpp
    aoutarm9045_2x2_4.cpp
    aoutarm9045_2x2_8.cpp
    aoutarm9045_2x3_6_14.cpp
    # --- W6.2c-OUT batch-3 (FINAL 7 OUT-ARM site variants made ACTIVE; LIVE mode):
    #     2x3_6/2x4_4/2x4_8/2x5_8/2x6_8/2x8_8/All_1Picker.  This empties BOTH the
    #     gated-extern and static-stub blocks in aoutarm9045.cpp -- all out-arm
    #     variants now resolve to real translated funcs.  Drove the substrate add of
    #     TMyKitSuck::HasRealIC_Left/Right (golden MyKitSuck.h:294-295) used by
    #     2x5_8/2x6_8/2x8_8, and the REMOVAL of the temp MoveOutArmToShuttleIncludeZ_
    #     9045_2x4_4 stub (now defined for real in aoutarm9045_2x4_4.cpp).
    #     All_1Picker is DoOutArm-ONLY (picker-gated; no DoPickFromShuttle callee).
    aoutarm9045_2x3_6.cpp
    aoutarm9045_2x4_4.cpp
    aoutarm9045_2x4_8.cpp
    aoutarm9045_2x5_8.cpp
    aoutarm9045_2x6_8.cpp
    aoutarm9045_2x8_8.cpp
    aoutarm9045_All_1Picker.cpp
    # --- W6.2c-OUT cross-module offline shims: the out-arm ENGINE free-functions
    #     + SM-cursor globals the 6 variants call but whose golden bodies (aoutarm.cpp
    #     / aoutarm9045.cpp engine helpers / Magazine / aRotateKIT_Out / AutoTeach /
    #     FixAICCD) are not translated yet.  Offline-safe non-static defs so an exe
    #     linking ht9045_sm resolves them; the variants forward-declare each.
    aoutarm_shims.cpp
    # --- W6.2c-OUT batch-4 (DEAD): 6 out-arm site variants that are NEVER
    #     dispatched -- compiled as dead weight only (like in-arm 2x4_16/2x8_32).
    #     1x4_4_Back (enum present but DoOutArm ladder arm is an EMPTY //-body and
    #     DoPickFromShuttle routes it to LIVE 1x4_4); 2x2_4_14 (enum present, BOTH
    #     ladders route to LIVE 2x2_4); 2x2_4_23 / 2x4_16 / 2x8_16 (enums ABSENT
    #     from MachineType.h -> no dispatch arm); 2x8_32 (enum present, BOTH ladders
    #     route to LIVE 2x8_8).  Each carries the full _<deadv> suffix on its
    #     DoOutArm_9045_<deadv> + helpers so all symbols are UNIQUE never-called
    #     globals.  CMake-only: NO static-stub removal, NO extern un-gate, NO
    #     dispatch-ladder edit to aoutarm9045.cpp (the ladders are already 100% live).
    aoutarm9045_1x4_4_Back.cpp
    aoutarm9045_2x2_4_14.cpp
    aoutarm9045_2x2_4_23.cpp
    aoutarm9045_2x4_16.cpp
    aoutarm9045_2x8_16.cpp
    aoutarm9045_2x8_32.cpp
    # --- W6.2c-OUT B5 (S-prefix, dead-but-compiled): the LAST out-arm variant
    #     file.  aoutarm9045S_1x4_4.cpp carries the IRREGULAR _9045S_1x4_4 infix
    #     on its top-level export DoOutArm_9045S_1x4_4() AND all helpers.  VERIFIED
    #     DEAD: zero external callers in the golden tree (out-arm dispatch ladders
    #     route to the SUFFIX-S DoOutArm_9045_1x4_4S, defined in the DIFFERENT
    #     already-committed aoutarm9045_1x4_4S.cpp).  Compiled-only, like in-arm
    #     2x8_32: NO static-stub removal, NO extern un-gate, NO dispatch-ladder
    #     edit to aoutarm9045.cpp (dispatch unchanged -- file never dispatched).
    aoutarm9045S_1x4_4.cpp
    # --- W906-Automation INTEGRATE: Automation/automation.cpp -- TfAutomation,
    #     the OLP host-command engine's "network wiring + small helpers" slice
    #     (~34 methods + 6 free functions: CommandProcess/SendClient/SendServer,
    #     OLPServerClient*/OLPClient* socket event handlers, tmrOLPTimer,
    #     DoCommandBuffer, ShowCharHex/ShowRecord/SaveRecord/ShowMSG,
    #     GetHTMLJSONDatas/GetMachineStatus/GetWorkOrder/GetMainTemp/
    #     GetTemperatures, ...). Lives HERE (not a standalone library) because
    #     it depends on FormsFacade.h (fMain->palMainStatus/cbSetupFileName/
    #     edWorkTemperBase) + canary_support.h (RecordProcess) + auto9045.h
    #     (GetHandlerType/GetDataPath/GetMainStatus/...), all already carried
    #     by this library -- same rationale as Interface/InterfaceSYS.cpp /
    #     Interface/TesterTCP_Socket.cpp above. ProcessBuffer/ProcessBuffer1/
    #     SendReportRequest stay safe no-op stubs (deferred to a dedicated
    #     future wave, see the file's own header). INTEGRATE CHECK: golden's
    #     bare `fAutomation` global is already claimed by atester_shims.h's
    #     TfAutomationShim (consumed by atester.cpp/atester_32Site.cpp) -- this
    #     translation names its own real singleton `fAutomationEngine` instead
    #     (see automation.h's own "GLOBAL-NAME COLLISION AVOIDED" note); no ODR
    #     conflict confirmed by grep across the whole tree.
    Automation/automation.cpp
    # --- W906-D-Interface-TesterTCP INTEGRATE: Interface/TesterTCP.cpp -- a
    #     SIBLING slice of Interface/TesterTCP_Socket.cpp above (same golden
    #     TfTesterTCP form, a DISJOINT function subset: CopyRecipeToTester/
    #     CopyRecipeFromTester OS-recipe zip-transfer pair + the per-machine
    #     btnSaveClick arm-speed-save handler; TesterTCP_Socket.cpp keeps sole
    #     ownership of the ClientSocket_TCPIP connect/timer/read/error family --
    #     zero overlap, confirmed by both units' own translate reports). Lives
    #     HERE for the identical reason as its sibling: depends on common.h
    #     (DataPath/sSaveByMachine/WriteIniData, ht9045_core, transitively
    #     needed at final-link time) + canary_support.h (RecordProcess/
    #     ShowMyMessage) + the REAL offline MySleep(DWORD) already defined in
    #     acarry_shims.cpp below (this file's own extern redeclaration now
    #     resolves for real once compiled into this same library, matching the
    #     precedent already established by Automation/auto9045.cpp/
    #     CCLink/MyCCLink.cpp/KYECFTP/FTPClient_Transfer.cpp's identical
    #     extern-redeclaration idiom for this exact symbol).
    Interface/TesterTCP.cpp
    # --- W5 Automation wave (Automation network unit): TeraPower AMR tray-count
    #     bridge (TTeraPowerAMR / global `AMR`) + the HANA-ART SRQ protocol
    #     helper (uHANA_ART) + the 8-function SCK/ATK "Auto Retest" extract
    #     (Automation/SCK_ART.cpp, wired into csystem.cpp's W7C1/W7C2 SCKART
    #     seams -- see csystem.cpp).  Same footprint already carried by
    #     ht9045_sm: ht9045_globals/ht9045_motor/FormsFacade.h/canary_support.h.
    #     NOTE: Automation/AMR.cpp's global `AMR` retires acatchtray_shims.cpp's
    #     TAMRShim/AMR stopgap (ODR: same name, different type) -- see that
    #     file's comment.
    # AI(W906-W7-F0-fix) 20260728: this wave's 4th member,
    # Automation/AGV_predicates.cpp (the 3 AGV AMR-mode predicates
    # AGV_IsSPIL_AMR/AGV_IsATK_AMR/AGV_Use_AMR), was MOVED OUT of this library
    # into ht9045_forms -- forms/fAGV.cpp delegates to them, and leaving them
    # here gave libht9045_forms.a 3 undefined refs into ht9045_sm that
    # ht9045_forms declared no dependency for, silently voiding its bottom-layer
    # property.  See the "NO UNDECLARED BACK-EDGE" block in the ht9045_forms
    # target above.  This library's own consumer, Automation/AGV_PortScan.cpp
    # (below), still resolves them: ht9045_sm PUBLIC-links ht9045_forms.
    Automation/AMR.cpp
    Automation/HANA_ART.cpp
    Automation/SCK_ART.cpp
    # --- W5-Final-AGV_E84 INTEGRATE: Automation/AGV_E84.cpp -- the E84
    #     loader/unloader handshake core (DoE84Loader/DoE84Unloader, 15/15
    #     case labels each, faithful) + ShowE84Log/E84StatusLog file logging.
    #     Same footprint as the AMR/HANA_ART/SCK_ART cluster just above
    #     (ht9045_globals/ht9045_motor/FormsFacade.h/
    #     canary_support.h) -- that cluster's 4th member, AGV_predicates.cpp,
    #     moved to ht9045_forms, see the AI(W906-W7-F0-fix) note above;
    #     additionally now uses fAGV->mmE84Log (see
    #     FormsFacade.h/.cpp TfAGV::mmE84Log, added this integrate).
    Automation/AGV_E84.cpp
    # --- W5-Automation-cont INTEGRATE: Automation/AGV_PortScan.cpp -- the
    #     second AGV.cpp slice (after AGV_E84.cpp's E84 handshake core above and
    #     AGV_predicates.cpp's 3 AMR predicates, which AI(W906-W7-F0-fix)
    #     20260728 moved to ht9045_forms -- this file calls them and still
    #     resolves them via ht9045_sm's PUBLIC link to that target): E84
    #     sensor-status
    #     telemetry (NowE84Status/E84StatusChange) + the AMR/AGV port-scan
    #     dispatcher family (ScanLoadPort + 8 bScan*/bIsStackBusy free functions),
    #     13 functions total.  Same footprint as the AGV_E84.cpp/AGV_predicates.cpp
    #     cluster (ht9045_globals/ht9045_motor/FormsFacade.h/canary_support.h),
    #     plus 3 new FormsFacade stand-in members this unit added (TfMain::ALed1/
    #     labAutomation, TfLotInfo::ALedLoader/aLedAuto[3]/palRemoveTray -- see
    #     FormsFacade.h/.cpp "W5-Automation ADD" blocks) and #includes AGV_E84.h
    #     (reuses its iLoaderScanTask[]/iUnloaderScanTask[]/bE84Loaderflag[]/
    #     bE84Unloaderflag[] externs, does not redeclare) + AGV_predicates.h.
    Automation/AGV_PortScan.cpp
    # --- W5-Final-SckArtRemainder INTEGRATE: Automation/SCK_ART_Remainder.cpp --
    #     the next contiguous golden slice (SetSetupFilePath/AccessFile/
    #     SetGPIBVersion/UpdateCount/AddAlarmCode/ClearLotInfo/AddOutputJamCnt/
    #     SaveTestSummary-dispatcher) continuing the sibling SCK_ART.cpp 8-
    #     function extract just above.
    #     AI(W906-W7-F2) 20260729: the "flagged for a future TfSCKART/FormsFacade
    #     unification wave" TODO that used to end this entry is REPLACED by what
    #     that wave actually did, because it landed PARTIALLY and the difference
    #     matters to whoever picks this up next:
    #       DONE  -- SckArtRemainderState now `: public SckArtState`, so
    #                dCurrYield / iFTRTCount / iNeedRT / iInputCount /
    #                iManualRejectCnt / iTesterType / sLOTSTATUS / iCurrentStatus
    #                (+ iCurrent93KARTStep) are declared ONCE instead of twice.
    #                Provably behaviour-neutral: both ctors were compared field by
    #                field and already produced identical values.  Locked by
    #                tests/test_w7_f2_sckart_state.cpp.
    #       NOT DONE -- forms/fSCKART.h's TfSCKART still carries its own
    #                iInputJamCnt/iFTRTCount/iInputCount/iCurrent93KARTStep, and
    #                csystem.cpp's W7C1_TfSCKARTSeam / W7C2_TfSCKARTSeam each still
    #                embed a private SckArtState plus shadow fields.  Golden has
    #                ONE fSCKART object; this tree has several, so collapsing them
    #                CHANGES BEHAVIOUR (a write through one instance would become
    #                visible to readers of another) -- that is a semantic decision,
    #                not a refactor, and W7-F2 deliberately did not take it.
    #       FOUND    -- two real golden divergences surfaced by that diff
    #                (iLOTSTATUS_L/_W/_R/_A constructed 0 vs golden 3/1/4/6;
    #                iTesterType constructed 0 vs golden 1 for every CUSTOMER_CODE
    #                except CC_SCK).  Reported and pinned by test, NOT reconciled,
    #                per plan SS6-F2 / SS9-R9.  See the divergence blocks in
    #                csystem.cpp (W7C1_TfSCKARTSeam) and Automation/SCK_ART.cpp
    #                (SckArtState ctor).
    Automation/SCK_ART_Remainder.cpp
    # --- W5-Final-Auto9045 INTEGRATE: Automation/auto9045.cpp -- the whole OLP/
    #     GPIB host-command config-accessor layer (148 golden free functions,
    #     confirmed NOT a VCL TForm). Same footprint as the Automation cluster
    #     above; all VCL-form gaps this file reaches (fBinSel/fShowBinSelect/
    #     fTrayAssignment/fTestCategory/fContactCT/fCounterClear/fMonitor/
    #     fProductionInfo/fTemp_Set + extra fMain/fObserver/fLotInfo/fContact/
    #     fAutomation members) stay as this file's own TU-local `W5FA_`
    #     companion objects -- NOT folded into FormsFacade.h/atester_shims.h by
    #     this integrate (would each need a brand-new shared form class with no
    #     other consumer yet; deferred, see this integrate's own report).
    Automation/auto9045.cpp
    # --- W7 (TRAY-ARM engine): the top dispatcher DoCatchTray (flat switch) + 8
    #     pumpable task SM skeletons (DoCatchFromLoader / CatchNewTrayFromBuffer /
    #     DoPlaceTrayToAuto / DoPlaceToBuffer / DoSlapTray / DoCatchUnderTray /
    #     DoSupportUnderTray / DoPlaceBufferTray) + cover-tray/RFID/cassette SMs +
    #     the pure decision helpers (WhichAutoNeedTray/IsOutArmSafe/...).  No
    #     site-variant dispatch in this file.  Hardware behind the Sim HAL
    #     (MOT[]/Cylinder[]/Sen[]/SW[]/CatchTraySuck); cross-module/UI surface
    #     (fTrayMapping/LoadCCD/MyMessageBox/HSys.BinDisCtrl) behind offline
    #     shims + ACTIVE stubs; AMR is REAL now (Automation/AMR.cpp above); see
    #     the file-head gate documentation.  OWNS the real WhichAutoNeedTray()
    #     (the canary_support.cpp sim stub was removed).
    acatchtray.cpp
    # --- W6.3 TRAY-ARM cross-module offline shims (fTrayMapping / MyMessageBox /
    #     LoadCCD / SECS_EVENT+EventReport / Tray-Mapping-laser / AMR-feed /
    #     NewRecordProcess / BoolToStr / iCoverTrayIDTask / iReadCIDAction).
    acatchtray_shims.cpp
    # --- W6.4 (TESTER/INDEX engine): the test-result-DECODE anchor (GetBinaryData /
    #     GetOneByOneData fed by Test_GetTestResulSub/Test_GetTestResul) + its
    #     consumers (Check_TTL_Status / CheckTestSocketIsError / ProcessTestResult)
    #     + the central index SMs DoTestY / DoTestHeadMotor (entry faithful, deep
    #     MOT[]/EP/torque/Front-Rear/32Site bodies gated) + pure site geometry
    #     (ArrayConvertSite / GetRowCol / GetRTCSiteMap).  Pumps the same Sim HAL +
    #     aHotPlateSubstrate (TestSocket/FTestSuck/BTestSuck/InArmSuck/FRCarryKit/
    #     BRCarryKit + iTestYTask/iTestHeadMotorTask cursors).  See the file-head
    #     gate documentation.
    atester.cpp
    # --- W64b (TESTER/INDEX engine): the Front (Z1/Arm1) and Rear (Z2/Arm2)
    #     "Destroy IC" + "Suck IC" down-press state machines (DoFrontTestDestroyIC /
    #     DoRearTestDestroyIC / DoFrontTestSuckIC / DoRearTestSuckIC +
    #     CheckZ1IsDown/CheckZ2IsDown/TestZ1OutRandge/TestZ2OutRandge/
    #     TestZ1SetPos/TestZ2SetPos leaf deps), faithfully translated -- no
    #     longer offline shims for these symbols (see atester_shims.cpp note
    #     below for what of Front/Rear is still shimmed).
    aTester_Front.cpp
    aTester_Rear.cpp
    # --- W5-atester32site / W5-ProcessCount-Translate: the 32-Site dual-arm
    #     test-head engine (all 8 golden functions, 2 vision islands internally
    #     gated -- see the file's own header) + the ProcessCount/low-yield-alarm
    #     leaf subset (14 golden functions: RecordPiggyBackStartEnd/
    #     ProcessPiggyBackFunction/InitialPiggyBackFunction/ProcessSocketPurgeCount/
    #     DoLowYieldAlarm/DoLowYieldAlarmFirst/CheckContinuoussFail/
    #     ProcessContinuoussFailForATC/GetMultiplierNum/CheckContinuoussPassBySocket/
    #     CheckContinuoussPass/ProcessHeadContactCount/ProcessSocketCount/
    #     ProcessAutoloadcellMeasureCount).  Both retire their corresponding
    #     stub entries from atester_shims.cpp/.h (see that file's remaining
    #     scope note below).
    atester_32Site.cpp
    atester_ProcessCount.cpp
    # --- W6.4 TESTER/INDEX cross-module offline shims (fContact / ADAM_* /
    #     CCDInterfaceForm / fAutomation / fObserver / fiosetview / COM2 / IsNNMode /
    #     CheckHeaterOK; aTester_Front/Rear's Destroy+Suck pairs and the whole of
    #     atester_32Site/atester_ProcessCount are real now -- see above -- every
    #     OTHER Front/Rear export, e.g. DoTestYFront/TestZ1OutRandge2, plus the
    #     ProcessCount exports still out of this wave's scope [ProcessShowTestStatus/
    #     ProcessCount/ProcessStartTestData/RecordHistroy/ProcessQASampling], stay
    #     offline shims here).
    atester_shims.cpp
    # --- W6.5 (SHUTTLE/CARRY engine): the two input-shuttle master SMs
    #     Do_Auto_SHT1 / Do_Auto_SHT2 (39 cases each, ALL ACTIVE; case 200->201
    #     fall-through preserved) + the 9046AU sort-shuttle Do_Auto_SHT3 + the
    #     residual/sensor-broken/null-IC/floating checks (switch TestIF.iTestMode)
    #     + the In/Out-arm-vs-shuttle move-safe interlocks + pure pitch geometry +
    #     the uShuttleThread sensor-poll LOGIC (verbatim; the Win32 wrapper is a
    #     thin offline driver).  Pumps the same Sim HAL + aHotPlateSubstrate
    #     (FLCarryKit/BLCarryKit/FRCarryKit/BRCarryKit + AutoSHT1/2/3Task cursors).
    #     NO #if 0 gates -- every golden case body is ACTIVE.  See the file head.
    acarry.cpp
    # --- W6.5 SHUTTLE/CARRY cross-module offline shims (Tech / fLtcSensor /
    #     OutSht3Kit / ATC_InterfaceForm+ATC_Data / CheckShtFloating /
    #     InShtInLF-RT(int) / SortSHTInLF-RT / DoStepShuttleCheck / shuttle
    #     Empty/MustHas/Prominent checks / Check32site* / MoveOutArmXY_To* /
    #     SendHTTPRequest / MoveSortArmToAutoSafe / Win32 thread tokens).
    acarry_shims.cpp
    # --- W906-AutoCleanFoundation (~30 pure-calc/HAL-only helpers) + W906-
    #     AutoCleanCluster (9 small helpers + the 4 named core pick/place
    #     engines -- DoAutoCleanPickfromCleanKit/DoPlaceToShuttle/
    #     DoPickFromShuttle/DoAutoCleanPlaceToCleanKit -- + the 3 shuttle-clean
    #     state machines + the DoAutoCleanKit master orchestrator), out of
    #     golden AutoClean/AutoClean.cpp (9,137 lines). Still explicitly OUT OF
    #     SCOPE (a separate follow-on wave, the SECOND dependency cluster):
    #     DoIndexAutoClean(+_Arm1PickArm2Test variant) -- see the TEMPORARY
    #     placeholder stub for DoIndexAutoClean near the top of the .cpp.
    #     Mirrors golden's own AutoClean/ subfolder (new in the target tree).
    AutoClean/AutoClean.cpp
    # --- W6.6 (csystem ORCHESTRATION HUB): the integration SPINE that ties the
    #     five already-translated engines together.  csystem.cpp realizes the
    #     frozen csystem.h spine free-functions: MainProc (slim) + DoAllProcess
    #     (THE per-tick engine driver -- bDoProcess 2-tick alternation + master
    #     guards + the 5 engine calls in documented order + DoLoad/DoCatchTray/
    #     DoAutoReceiveBinTray loop + tail loops) + the alive-instrumentation
    #     (GetMainProc*/IsMainProcAlive) + InitAllProcessTask + CheckMotorHome +
    #     SetInitialICCheck + InitCleanOutFunction/InitOneCycle/InitTrayEndFunction.
    #     The ~24K-line god-file bulk (AutoRetest/OCR/AutoClean/mode-ladder/
    #     DoOneCycleFinishCheck/DoCleanOutFinishCheck/...) is GATED #if 0
    #     // TODO(W7).  Predicates (HasIC family) stay in csystem_predicates.cpp
    #     (ODR-skipped here).  See the file-head documentation.
    csystem.cpp
    # --- W6.6 HUB cross-module offline shims (DoInArm->DoInArm_9045 /
    #     DoOutArm->DoOutArm_9045 / DoLoad / DoSortArm / bShuttleShake +
    #     InitLoadTask / InitOutArmTask / InitialDoLockUnloader /
    #     Initial_Auto_BinTray_Task / InitAutoColorTask / Magazine inits /
    #     DoAuto3Magazine / NewDoAutoTrayEdgeCylinderLoop / DoAutoEmpty1 /
    #     DoInArm/OutArmTeachAlignmentProcess / SetFixTrayMiddleDtata /
    #     DoOneCycleFinishCheck / DoCleanOutFinishCheck).
    #     AI(W906-W7-L1) 20260729: DoAuto2 REMOVED from this list -- it is now a
    #     real body in asendic_Auto2.cpp (added just above), not a shim here.
    csystem_shims.cpp
    # --- W5-BarCode (BarCode front, 2026-07-11): real bodies for 18 of the 20
    #     TfBarCode_Shim methods (aHotPlateSubstrate.h:623-664).  7 parallel
    #     translate units extracting SPECIFIC functions from the still-largely-
    #     untranslated ~24,500-line BarCode/{BarCode,BarCode_Sh1,BarCode_Sh2,
    #     BarcodeXML}.cpp VCL-form core (see MIGRATION_ROADMAP.md DEFERRED
    #     table).  DoBarcodeScanInShuttle_2/DoShuttleFloatCheck_2 were NOT
    #     delivered this wave (clean hand-off, BarCode_Shuttle2_Scan's own
    #     report) -- those 2 TfBarCode_Shim methods keep their pre-existing
    #     offline stub.  BarCode_Bottom2DID8CCD.{h,cpp} intentionally live at
    #     project ROOT (not under BarCode/), per that unit's exact task-brief
    #     path -- see its own file banner.
    #     INTEGRATE FIX: resolved 2 real cross-unit ODR conflicts before wiring
    #     (BarCode_Shuttle2_Scan.cpp's own bCCDcheckShuttle[2] vs. the existing
    #     cmydef.cpp global; BarCode_Shuttle2_CCDScan.cpp's independent
    #     redefinition of 8 golden TfBarCode-member globals already owned by
    #     BarCode_Bottom2DID.cpp) -- both units' own translate reports flagged
    #     these; see aHotPlateSubstrate.cpp's include-block comment for the
    #     resolution.
    BarCode/BarCode_Helpers.cpp
    BarCode/BarCode_Bottom2DID.cpp
    BarCode_Bottom2DID8CCD.cpp
    BarCode/BarCode_Shuttle1_Scan.cpp
    BarCode/BarCode_Shuttle2_Scan.cpp
    BarCode/BarCode_Shuttle1_CCDScan.cpp
    BarCode/BarCode_Shuttle2_CCDScan.cpp
    # --- W5-BarCode-Final (BarCode remainder front, 2026-07-11): the last 2/20
    #     TfBarCode_Shim methods (DoBarcodeScanInShuttle_2/DoShuttleFloatCheck_2,
    #     the exact "REMAINING / HANDED OFF" leftover flagged above) + the
    #     8-CCD ClientSocket connection-lifecycle glue (SendCCDCommand's REAL
    #     body + the 13 real TClientSocket globals + the Bottom-8CCD Connect/
    #     Init timer pair).  TfBarCode_Shim is 20/20 real methods as of this
    #     integrate.  See aHotPlateSubstrate.cpp's include-block comment.
    #     INTEGRATE DECISION (BarCode-8CCD-Glue's own flagged question):
    #     the 6 pre-existing per-unit gated Gated_SendCCDCommand-style no-op
    #     stand-ins (BarCode_Bottom2DID.cpp, BarCode_Shuttle1_CCDScan.cpp,
    #     BarCode_Shuttle1_Scan.cpp, BarCode_Shuttle2_CCDScan.cpp,
    #     BarCode_Shuttle2_Scan.cpp, BarCode_Bottom2DID8CCD.cpp) are LEFT
    #     SEPARATE, NOT retargeted onto the new real ::SendCCDCommand -- (1)
    #     those 6 files are already-integrated production code outside this
    #     front's 3-unit scope, and this campaign's whole premise is that
    #     serial integrates avoid touching files another front owns; (2) each
    #     already has a passing oracle test asserting its own no-op/log
    #     behaviour -- retargeting risks a silent behavioural change (real
    #     SendCCDCommand's own log format + Socket->Active/SendText branches)
    #     for zero required benefit, since every gated stand-in is already
    #     faithful to a disconnected-CCD machine; (3) consolidating call sites
    #     is a cleanup, not a correctness fix -- better done as its own
    #     deliberate follow-up wave with full re-verification of all 6 affected
    #     test suites, not a side effect of this hand-off completion.
    BarCode/BarCode_Shuttle2_ScanRemainder1.cpp
    BarCode/BarCode_Shuttle2_ScanRemainder2.cpp
    BarCode/BarCode_8CCD_Glue.cpp
    # --- W7-L2 substrate (2026-08-03): free-function seams that golden's
    #     ScanSystemSensor (ckernel.cpp:359-691) calls and that have NO ported
    #     home of their own -- SetWorkParameter (called golden ckernel.cpp:371,
    #     declared golden cinitial.h:17) and SetSuckRetryCount (called :496).
    #     Correct target: ckernel's substrate (Sen[]/SW[]/MOT[]/Cylinder[]/
    #     canary_support/FormsFacade/csystem.h) is exactly what ht9045_sm
    #     already carries, so no new link dependency is introduced.
    ckernel_shims.cpp
    # --- W7-L2 (2026-08-03): golden ckernel.cpp in full -- the system-sensor /
    #     panel-key / panel-lamp / motor-alarm layer.  15 of golden's 17
    #     functions are real bodies; ShowRunLed (golden :704-932) and
    #     ShowRunLabel (:935-1726) are explicitly-labelled DEFERRED stubs with
    #     call counters, because between them they make 181 fMain-> widget
    #     dereferences and belong to W7-U, not here.  One ported file per golden
    #     file per KNOWLEDGE.md:14, so this is assembled from six translation
    #     fragments in golden line order rather than split by function.
    #     ScanSystemSensor -- CORRECTED 20260803 BY INDEPENDENT AUDIT.  This comment
    #     used to claim it "has NO live caller yet" and that csystem.cpp:16894 was
    #     golden's only call site.  Both were false.  Golden has TWO call sites
    #     (exhaustive scan of 786 files over the golden tree AND the out-of-tree
    #     package D:\HT9045\elec\Component): csystem.cpp:16894, whose ported MainProc
    #     ladder really is still the three-line `#if 0` at ported csystem.cpp:415-418
    #     and so really is dormant here; and golden ckernel.cpp:2511 inside
    #     ProcessAlarm, which THIS wave translates and which calls it live at ported
    #     ckernel.cpp:2852.  Wiring the MainProc one would still be an ordering
    #     deviation, so csystem.cpp is untouched -- but the function is NOT uncalled.
    ckernel.cpp
)

target_include_directories(ht9045_sm PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_sm PUBLIC
    ht9045_io ht9045_motor ht9045_globals vclcompat
    # W5-comms INTEGRATE: EventReport()/SECS_EVENT (csystem.cpp) + link-
    # completeness for Interface/InterfaceSYS.cpp (see source-list comment above).
    ht9045_secsgem
    # AI(W906-W7-F0) 20260728: the ~60 TUs in this library that #include
    # "FormsFacade.h" used to get fMain/fLotInfo/... from FormsFacade.cpp inside
    # this same archive; it now lives in ht9045_forms.  (Strictly redundant with
    # the transitive edge via ht9045_secsgem above, but declared explicitly
    # because ht9045_sm's own sources are the direct consumers -- if the secsgem
    # edge is ever narrowed, this must not silently break.)
    ht9045_forms)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # The SM carries BCB6 idioms inherited from the global headers (AnsiString
    # brace-init etc.).  Match the ht9045_io / ht9045_globals warning posture so
    # the translated stepper compiles without tripping on already-accepted
    # global-header idioms, while still flagging genuine porting mistakes.
    target_compile_options(ht9045_sm PRIVATE
        -Wall -Wextra -Wno-conversion-null -Wno-write-strings -Wno-unused-parameter)
endif()

# W6.6: enable the csystem.cpp tick-sequence ORACLE hook (compiles a tiny
# std::vector trace into DoAllProcess so the W6.6 verify TU can confirm the
# documented per-tick engine ORDER).  The CSYS_TICK macro is a NO-OP unless this
# is defined, so the faithful golden body is untouched in spirit; the trace is
# write-only instrumentation read only by the integration test.
target_compile_definitions(ht9045_sm PRIVATE CSYSTEM_TICK_ORACLE)

# ---------------------------------------------------------------------------
# Library: ht9045_kyecftp  (W5-Final KYECFTP wave -- FTP client engine +
#   TfFTPClient session logic)
#
#   KYECFTP/MiniFtpEngine.{h,cpp}            -- Nmftp::TNMFTP: a from-scratch FTP
#     protocol engine standing in for the closed BCB6 TNMFTP component (no vendor
#     source exists anywhere in this repo -- see the file's own banner for every
#     inferred wire-behavior judgment call: async->sync, PASV-only, Vendor/
#     ParseList inert, CurrentDir's live PWD round-trip, Mode()'s MODE_BYTE->
#     "TYPE L 8" mapping, 502->OnUnSupportedFunction classification). Built on
#     vclcompat/ClientSocket.h (one control-channel connection + a fresh PASV
#     data-channel connection per transfer). Zero dependency beyond vclcompat.
#   KYECFTP/FTPClient_EventHandlers.{h,cpp}  -- TfFTPClient's NMFTP1 event-handler
#     bodies (status/success/failure logging), as free functions matching
#     TNMFTP's std::function callback-slot signatures. Needs CUSTOMER_CODE
#     (cmydef.cpp) + MyDBIProcess (aHotPlateSubstrate.cpp) + ShowMyMessage
#     (canary_support.cpp).
#   KYECFTP/FTPClient_Transfer.{h,cpp}       -- the 4 real FTP-transfer session
#     functions (LoadFileFormServer2/UploadFileToServer2/Download_2DSortingList/
#     Download_2DID_WhiteList), built against both siblings above. Needs
#     CUSTOMER_CODE/IniConfig/CosFunction/Temperature (cprod.cpp/cmydef.cpp) +
#     DataPath/OffsetPath/DefaultPath/LastDataPath/asATCFileTransferPath/
#     sWhite2DIDListLoc (common.cpp) + MyDBIProcess/NewRecordProcess/MySleep/
#     ShowMyMessage/RecordProcess (aHotPlateSubstrate.cpp/acatchtray_shims.cpp/
#     acarry_shims.cpp/canary_support.cpp, all in ht9045_sm).
#
#   PRODUCTION link surface: vclcompat (MiniFtpEngine's transport) + ht9045_sm
#   (CUSTOMER_CODE/IniConfig/CosFunction/Temperature transitively via
#   ht9045_globals, + the 5 real free-function bodies above) + ht9045_core
#   (common.cpp's DataPath family -- ht9045_sm does NOT itself link ht9045_core,
#   see that library's own link line above, so it must be named explicitly here
#   too). NOT exercised end-to-end by this library's own unit tests below --
#   see those targets' own comments in tests/CMakeLists.txt for why: each test
#   supplies TU-local stand-in globals/functions instead, deliberately avoiding
#   the cmydef/cprod/common/canary_support/aHotPlateSubstrate "god-stack" for a
#   fast, focused unit test (same avoidance already established by
#   tests/test_config_loaders.cpp / tests/test_MyCCLink.cpp) -- ht9045_kyecftp
#   itself compiling clean here is what proves these 3 files type-check against
#   the REAL header declarations (cmydef.h/cprod.h/Config.h/CosFunction.h/
#   common.h/canary_support.h), which the units' own narrower manual
#   verification could not check.
#
#   No circular dependency: grepped the whole tree (production + tests) for any
#   existing caller of LoadFileFormServer2/UploadFileToServer2/
#   Download_2DSortingList/Download_2DID_WhiteList/Nmftp:: outside KYECFTP/
#   itself -- zero hits. ht9045_sm/ht9045_core need nothing from ht9045_kyecftp,
#   so this is a clean one-way dependency (unlike Interface/InterfaceSYS.cpp,
#   which had a genuine two-way need and was folded directly into ht9045_sm's
#   own source list instead -- see that library's comment above).
# ---------------------------------------------------------------------------
add_library(ht9045_kyecftp
    KYECFTP/MiniFtpEngine.cpp
    KYECFTP/FTPClient_EventHandlers.cpp
    KYECFTP/FTPClient_Transfer.cpp
)

target_include_directories(ht9045_kyecftp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_kyecftp PUBLIC vclcompat ht9045_sm ht9045_core)

if(WIN32)
    target_link_libraries(ht9045_kyecftp PUBLIC ws2_32)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Same relaxed posture as ht9045_sm/ht9045_comms: FTPClient_EventHandlers.cpp/
    # FTPClient_Transfer.cpp #include the BCB6-idiom-heavy global headers
    # (cmydef.h/cprod.h/Config.h/CosFunction.h/common.h). MiniFtpEngine.cpp itself
    # (zero god-stack dependency) is independently clean under the full strict
    # set per its own unit's report; no per-file carve-out is needed since the
    # relaxed flags here are a strict subset of warnings, not additions.
    target_compile_options(ht9045_kyecftp PRIVATE
        -Wall -Wextra -Wno-conversion-null -Wno-write-strings -Wno-unused-parameter)
endif()

# =============================================================================
# AI(W906-GateA-0) 20260804: W7-U0 -- the MFC UI executable, opt-in, MSVC-only.
# docs/GATE_A_FIRST_LIGHT_PLAN.md GA-0 / docs/DESIGN_GateA.md D-A0-1.
#
#   * option() default OFF is LOAD-BEARING: MFC exists only under MSVC, and this
#     project's primary oracle is MinGW -- a default-ON UI target would kill that
#     oracle (MIGRATION_ROADMAP.md:22 flagged exactly this). scripts/
#     build_msvc.bat:90 has been passing -DHT9045_UI=OFF since W7-A1 against an
#     option that did not exist (CMake only warns on an undefined -D); this
#     block finally defines it.
#   * MSVC-only guard is FATAL_ERROR, not a silent skip: asking for the UI on
#     MinGW is a wrong-toolchain mistake and must say so loudly.
#   * CMAKE_MFC_FLAG=2 (shared MFC DLL) + explicit _AFXDLL: the 2026-07-29 MFC
#     probe validated exactly this shape (Release /MD + /D_AFXDLL, vcvarsall
#     x86 -vcvars_ver=14.44). Debug CRT is prohibited tree-wide (KNOWLEDGE.md
#     Gotcha 9). _AFXDLL is defined explicitly because CMAKE_MFC_FLAG's
#     define-injection is generator-dependent; doubling it is harmless.
#   * OUTPUT_NAME HT9045: golden's exe name (HT9045.bpr PROJECT value), so the
#     faithful exe-path check in ui/HT9045App.cpp compares like golden.
#   * links ht9045_public only (cBootLog); the SM/engine libraries join in
#     GA-3 when the FormRegistry starts constructing real facade objects.
# =============================================================================
option(HT9045_UI "Build the MFC UI executable ht9045_app (MSVC only)" OFF)
if(HT9045_UI)
    if(NOT MSVC)
        message(FATAL_ERROR "HT9045_UI=ON requires MSVC+MFC. Use scripts/build_msvc_ui.bat; the MinGW oracle never builds UI.")
    endif()
    enable_language(RC)
    set(CMAKE_MFC_FLAG 2)

    # AI(W906-GateA-4-C5) 20260804: W7-C5 -- the three custom-control CWnd
    # shells + the single global window-class registration (plan S7-6/7,
    # S3-C1), together with the W7-C1 cores and W7-C2 renderers they drive.
    #
    #   * the vclcompat/*Core.cpp + vclcompat/render/*.cpp files are listed
    #     here rather than linked from a library ON PURPOSE: they are NOT part
    #     of the `vclcompat` target (grep -- only tests/CMakeLists.txt:2332-2412
    #     compiles them today, once per test target), and adding them to that
    #     library would change the MinGW oracle's link graph from inside a
    #     UI-only wave. Compiling them into the UI targets keeps the change
    #     confined to `if(HT9045_UI)`.
    #   * shared between ht9045_app and the headless probe below so the two
    #     never drift apart.
    set(HT9045_UI_CTRL_SOURCES
        ui/RegisterCustomClasses.cpp
        ui/CLedCtrl.cpp
        ui/CBtnPanelCtrl.cpp
        ui/CTrayCtrl.cpp
        vclcompat/LedCore.cpp
        vclcompat/BtnPanelCore.cpp
        vclcompat/TrayCore.cpp
        vclcompat/render/RenderCommon.cpp
        vclcompat/render/LedRender.cpp
        vclcompat/render/BtnPanelRender.cpp
        vclcompat/render/TrayRender.cpp
    )

    add_executable(ht9045_app WIN32
        ui/HT9045App.cpp
        ui/GateAPlaceholderDlg.cpp
        ${HT9045_UI_CTRL_SOURCES}
        ui/app.rc
    )
    target_compile_definitions(ht9045_app PRIVATE _AFXDLL)
    # AI(W906-GateA-vscode) 20260804: emit debug info for the RELEASE exe so
    # VS Code (F5 / cppvsdbg) can do source-level debugging. This deliberately
    # does NOT switch to a Debug configuration: the Debug CRT is prohibited
    # tree-wide (KNOWLEDGE.md Gotcha 9 -- its asserts raise MODAL dialogs that
    # wedged a whole ctest run). /Zi only ADDS a .pdb; optimisation flags and
    # therefore codegen are unchanged, so the binary the smoke test validates
    # stays the binary we ship. /FS is required because Ninja compiles this
    # target's TUs in parallel into one compiler PDB.
    target_compile_options(ht9045_app PRIVATE /Zi /FS)
    target_link_options(ht9045_app PRIVATE /DEBUG)
    target_include_directories(ht9045_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ui)
    set_target_properties(ht9045_app PROPERTIES OUTPUT_NAME "HT9045")
    target_link_libraries(ht9045_app PRIVATE ht9045_public)

    # AI(W906-GateA-4-C5) 20260804: headless verification exe for the shells.
    # Console subsystem (its own main() + AfxWinInit), creates the three
    # controls under an HWND_MESSAGE parent and paints them into a memory DC,
    # so it is structurally incapable of showing a window (plan S5-11 / S7).
    # Run by scripts/build_msvc_ui.bat; exit code 0 == every check passed.
    add_executable(ht9045_ctrl_probe
        ui/tests_headless/headless_ctrl_probe.cpp
        ${HT9045_UI_CTRL_SOURCES}
    )
    target_compile_definitions(ht9045_ctrl_probe PRIVATE _AFXDLL)
    target_include_directories(ht9045_ctrl_probe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ui)
    target_link_libraries(ht9045_ctrl_probe PRIVATE ht9045_public)
endif()

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests)
