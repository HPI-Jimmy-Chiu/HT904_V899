cmake_minimum_required(VERSION 3.10)
project(HT9045_Cpp C CXX)   # C needed for Public/cJSON.c

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
)

target_include_directories(ht9045_globals PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_globals PUBLIC vclcompat)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # The translated globals carry the original BCB6 idioms (e.g. AnsiString
    # array brace-init with NULL).  Keep them compiling without the strict
    # -Werror-style flags used on freshly-written modules.
    target_compile_options(ht9045_globals PRIVATE
        -Wno-conversion-null -Wno-write-strings)
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
# Tests
# ---------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests)
