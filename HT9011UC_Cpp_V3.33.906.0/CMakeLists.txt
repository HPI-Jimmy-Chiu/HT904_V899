cmake_minimum_required(VERSION 3.10)
project(HT9045_Cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

# Strict warnings help catch porting errors early.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_core PRIVATE
        -Wall -Wextra -Wshadow -Wconversion -Wpedantic
    )
endif()

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests)
