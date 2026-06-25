# ===========================================================================
#  third_party/sqlite3 -- EXTERNAL portable dependency target  (W3)
#
#  The BCB6 golden ref (HT9011UC_Code_V3.33.906.0_20260618) links a PREBUILT
#  `sqlite3.lib` and ships only `sqlite3.h` -- there is NO `sqlite3.c` in the
#  source tree.  Per the migration plan, sqlite3 is therefore treated as an
#  EXTERNAL portable dependency, NOT hand-translated:  cMyDB (the wrapper) is
#  what we translate (W3); the sqlite engine itself stays the upstream
#  public-domain amalgamation.
#
#  This module resolves sqlite3 in priority order and exposes a single
#  INTERFACE/STATIC target `sqlite3` that the (future, W3) cMyDB library links:
#
#    1. VENDORED AMALGAMATION (preferred, fully reproducible / offline):
#         drop the upstream public-domain files here:
#             third_party/sqlite3/sqlite3.c
#             third_party/sqlite3/sqlite3.h
#         (download once from https://sqlite.org/download.html -- the
#          "sqlite-amalgamation-*.zip").  They compile as C into a static lib.
#         This is ADDITIVE: the .c is upstream, not a translation of anything.
#
#    2. SYSTEM/FIND fallback:  find_package(SQLite3) (CMake >= 3.14 builtin
#         FindSQLite3) or a manually located libsqlite3 + sqlite3.h.
#
#  If neither is available the target is created as a STUB INTERFACE library
#  that carries ONLY the golden `sqlite3.h` include path (so cMyDB can compile
#  against the header) and prints a configure-time NOTE that a real engine must
#  be provided before linking an executable that uses the DB.  This keeps the
#  W3 build green today (no cMyDB executable is linked yet) while documenting
#  the dependency honestly.
# ===========================================================================

set(_SQLITE_VENDORED_C "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.c")
set(_SQLITE_VENDORED_H "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.h")
# The golden header (used for the include path when vendoring the .c is not done).
set(_SQLITE_GOLDEN_H   "${CMAKE_SOURCE_DIR}/../HT9011UC_Code_V3.33.906.0_20260618/sqlite3.h")

if(EXISTS "${_SQLITE_VENDORED_C}" AND EXISTS "${_SQLITE_VENDORED_H}")
    # ---- (1) vendored amalgamation -> compile as C ------------------------
    message(STATUS "sqlite3: using VENDORED amalgamation (third_party/sqlite3/sqlite3.c)")
    add_library(sqlite3 STATIC "${_SQLITE_VENDORED_C}")
    set_source_files_properties("${_SQLITE_VENDORED_C}" PROPERTIES LANGUAGE C)
    target_include_directories(sqlite3 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
    # The amalgamation is upstream code: do not apply our strict warning set.
    target_compile_definitions(sqlite3 PRIVATE
        SQLITE_THREADSAFE=1            # match BCB6 default serialized mode
        SQLITE_OMIT_LOAD_EXTENSION=1)  # the wrapper never loads extensions
    if(WIN32)
        # the amalgamation links no extra libs on win32; pthread only on POSIX.
    else()
        find_package(Threads REQUIRED)
        target_link_libraries(sqlite3 PUBLIC Threads::Threads ${CMAKE_DL_LIBS})
    endif()
    set(SQLITE3_RESOLUTION "vendored-amalgamation" PARENT_SCOPE)
else()
    # ---- (2) system find --------------------------------------------------
    find_package(SQLite3 QUIET)
    if(SQLite3_FOUND)
        message(STATUS "sqlite3: using SYSTEM SQLite3 (${SQLite3_LIBRARIES})")
        add_library(sqlite3 INTERFACE)
        target_include_directories(sqlite3 INTERFACE "${SQLite3_INCLUDE_DIRS}")
        target_link_libraries(sqlite3 INTERFACE "${SQLite3_LIBRARIES}")
        set(SQLITE3_RESOLUTION "system-find" PARENT_SCOPE)
    else()
        # ---- (3) header-only STUB (documented, keeps W3 build green) ------
        message(STATUS
            "sqlite3: NO engine found -- header-only STUB target created.\n"
            "         Drop the upstream public-domain amalgamation at\n"
            "         third_party/sqlite3/sqlite3.{c,h} (https://sqlite.org/download.html)\n"
            "         OR install a system SQLite3 before linking the (future) cMyDB\n"
            "         executable.  The golden sqlite3.h include path is wired so cMyDB\n"
            "         can COMPILE against the header now.")
        add_library(sqlite3 INTERFACE)
        if(EXISTS "${_SQLITE_GOLDEN_H}")
            get_filename_component(_GOLDEN_INC "${_SQLITE_GOLDEN_H}" DIRECTORY)
            target_include_directories(sqlite3 INTERFACE "${_GOLDEN_INC}")
        endif()
        # Mark it so a downstream link of a real executable fails loudly rather
        # than silently producing a binary with no DB engine.
        target_compile_definitions(sqlite3 INTERFACE HT9045_SQLITE3_STUB=1)
        set(SQLITE3_RESOLUTION "header-only-stub" PARENT_SCOPE)
    endif()
endif()
