# third_party/sqlite3 — external SQLite dependency (W3)

The BCB6 golden reference (`HT9011UC_Code_V3.33.906.0_20260618`) links a
**prebuilt `sqlite3.lib`** and ships only **`sqlite3.h`** — there is **no
`sqlite3.c`** in the source tree. SQLite is therefore an **external portable
dependency**, *not* something we hand-translate. We translate only the
**`cMyDB` wrapper** (scheduled for W3); the engine stays the upstream
public-domain amalgamation.

## How `CMakeLists.txt` resolves it (priority order)

1. **Vendored amalgamation (preferred, offline-reproducible).**
   Download once from <https://sqlite.org/download.html>
   (`sqlite-amalgamation-*.zip`) and drop the two files here:
   ```
   third_party/sqlite3/sqlite3.c
   third_party/sqlite3/sqlite3.h
   ```
   They compile as C into a static `sqlite3` target. This is **additive
   upstream code**, not a translation.

2. **System find.** `find_package(SQLite3)` (CMake builtin `FindSQLite3`).

3. **Header-only stub.** If neither is present, a stub `sqlite3` INTERFACE
   target is created that exposes the golden `sqlite3.h` include path (so the
   future `cMyDB` can *compile*) and defines `HT9045_SQLITE3_STUB=1`. The W3
   build stays green because **no executable links `cMyDB` yet** (the wrapper
   is not translated in this wave).

## API surface the wrapper uses (all standard amalgamation symbols)

`sqlite3_open_v2`, `sqlite3_exec`, `sqlite3_get_table`, `sqlite3_free_table`,
`sqlite3_prepare*`/`sqlite3_step`/`sqlite3_finalize`, `sqlite3_next_stmt`,
`sqlite3_last_insert_rowid`, `sqlite3_free`, `sqlite3_close` — no extensions,
no internals. (Counted 78 `sqlite3_*` call sites in the golden `cMyDB.cpp`.)

## No-Borland-binary limitation

We cannot live-diff against the original Borland-built binary. Fidelity of the
(future) `cMyDB` translation will be checked by **round-trip DB tests**
(create temp DB → insert → query) once the wrapper is translated, plus the
real-config table-shape oracle already in `tests/test_config_db.cpp`.
