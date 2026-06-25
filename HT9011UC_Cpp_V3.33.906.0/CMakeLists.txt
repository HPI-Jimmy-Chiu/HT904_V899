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
# Tests
# ---------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests)
