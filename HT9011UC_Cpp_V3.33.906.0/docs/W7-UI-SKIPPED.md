# W7 UI -- Skipped Items / Real Blockers Log

Per the user's standing "hit a real blocker -> SKIP and record, don't stall
the whole line" instruction (plan `W7_UI_ARCHITECTURE_PLAN.md` S12 gate 6).
Each wave appends its own dated section; do not edit another wave's section.

---

## W7-B1b (2026-07-28) -- `.rc` + `_ids.h` generator, gates G5/G6

Full-corpus run: 133 forms, 3,637 generated `DIALOGEX` templates, 22,586
`CONTROL` entries. **G5 (rc.exe + windres) = 133/133 clean. G6 (`.res`
binary read-back diff) = 133/133 clean, 0 problems.** Details/exact
per-form numbers: `tools/dfm2rc/reports/b1b_gate_report.json`.

### SKIP-AND-RECORD items (anticipated by the plan, now precisely located)

These are emitted as ordinary `CONTROL "", id, "HT9045.<Name>", ...`
entries (D12) so the census/geometry gates never see a dropped node --
they are "skip" only in the sense of "not a faithful translation target",
per plan S6 SKIP-AND-RECORD / S9-R5:

| Class | Instances | Forms | Why skipped |
|---|---|---|---|
| `TEasyMain` | 1 | `cheksocket.dfm` | Euresys frame-grabber SDK singleton (vendor SDK surface, not translatable UI) |
| `TECameraPicolo` | 1 | `cheksocket.dfm` | same (Euresys SDK) |
| `TAPHeadLabel` | 10 | `Automation/mainAT.dfm` | Source doesn't exist (only a compiled `.dcu` + machine-generated `.hpp`, S4-V12); a real port is a later, approximate-only W7-U task |

### Discrepancies found against the plan's own cited numbers (flagged per
the project's "verify, don't accept self-claims" rule -- not silently
reconciled)

1. **§6-B1d "5,594 個 DIALOGEX template"**: measured (both from a direct
   walk of the real IR *and* from this generator's actual output)
   **3,637** DIALOGEX templates = 133 form roots + 3,504 SUBDLG-kind
   containers (`kind_histogram.SUBDLG` in `b1a_gate_report.json` summary:
   TPanel-with-kids 1,177 + TGroupBox 1,248 + TScrollBox 51 + TTabSheet
   691 + TRadioGroup 336 + TframeProdInfo 1 = 3,504). The number 5,594
   numerically coincides with the *unrelated* "HWNDs saved by not
   placeholder-Static'ing containers" figure the plan itself cites in
   S9-R3 -- almost certainly a copy/reuse slip when S6-B1d's summary line
   was written, not a sign of a different intended design. Generator and
   both gates were built and validated against the measured 3,637, not
   the cited 5,594.
2. **§9-R6 "802 個合成的 TRadioGroup radio button"**: measured (both
   independently straight from the IR's raw `Items.Strings` lists, and
   from this generator's own synthesis pass) **2,012** radio buttons
   across the corpus's 336 `TRadioGroup` instances (0 groups have an
   empty item list). `SYNTH_RADIOGROUP_ITEM` count in every generated
   `.rcmeta.json` sums to 2,012, not 802.

Neither discrepancy blocks anything -- both are recorded here per the
"flag, don't silently pick one" instruction, and the generator/gates were
built against the measured (not cited) values in both cases.

### Design choices made where the plan under-specifies (documented, not
silently decided)

- **Output staging location**: generated `.rc`/`_ids.h`/`.rcmeta.json`
  live under `tools/dfm2rc/rc_out/<mirrored golden relpath>/`, not
  scattered directly into the PORTED tree's module directories (most of
  which don't exist yet for un-translated UI forms). This follows the
  precedent W7-B1a already set with `tools/dfm2rc/ir_out/`. Final
  placement into the PORTED tree proper is a B1d/W7-U integration
  decision, not a B1b one.
- **Geometry**: per D7, the `.rc`'s DLU-labelled x/y/cx/cy fields are a
  literal, unconverted copy of the `.dfm`'s pixel integers (no px->DLU
  formula is applied) -- deliberately, so as not to manufacture a
  conversion that could be mistaken for meaningful; the real geometry
  authority is `W7-B1c`'s `_layout.gen.cpp`.
- **MinGW `windres` header gap**: `SS_REALSIZECONTROL` (used for
  `TImage`/`TEBW8Image`, 107 instances) is absent from MinGW's own
  `winuser.h` (verified: not present anywhere in
  `C:\MinGW\include\winuser.h`) though present in the Windows SDK
  `winuser.h` that `rc.exe` uses. Every generated `.rc` carries a local
  `#ifndef SS_REALSIZECONTROL / #define ... 0x40L / #endif` guard so both
  compilers accept it (this is exactly the class of environment gap G5's
  "both must pass" requirement exists to catch).

---

## W7-B1d (2026-07-28) -- full-corpus run + gate triage + CMake wiring

**New files**: `tools/dfm2rc/run_b1d.py` (the aggregation/ratchet driver,
4 modes), `tests/test_dfm2rc_pipeline.cpp` (compiled ctest wrapper),
`tools/dfm2rc/reports/b1d_fidelity_report.json`,
`b1d_idempotent_report.json`, `b1d_per_form_table.md` (generated, refreshed
by re-running the ctest targets). **Edited**: `tests/CMakeLists.txt`
(append-only, one new block; landed immediately before a concurrently-
appended sibling W7-C2 block and immediately after B1c's `DfmLayoutGen`
block -- re-read and grep-verified after every edit per plan S8's "don't
trust the tool's report" rule, since this file was actively being edited
by a parallel wave while this one was in progress).

### Independent re-verification (not trusted from B1b/B1c's self-reports)

Before wiring anything into CMake, re-ran `run_b1a.py`/`run_b1b.py`/
`run_b1c.py` fresh, then built this wave's own `run_b1d.py` and exercised
all 4 of its modes end to end, first standalone, then through the real
compiled ctest target. All confirmed independently, this session:

| Check | Result |
|---|---|
| B1a re-run (parse, G0-G4) | 133 files, 22,627 control nodes, 0 problems (2.7s) |
| B1b re-run (`.rc` emit + `rc.exe`/`windres` compile + G6) | 133 files, 3,637 dialogs, 22,586 controls, 0 problems (2m02s) |
| B1c re-run (layout emit + G9 crosscheck + compile smoke) | 133 files, 22,760 layout rows, 7,280 events, 0 problems (56s) |
| **B1d `fidelity`** (fresh re-derive of the ENTIRE pipeline from GOLDEN, all of G0-G9 + mojibake/BOM scan, in one pass) | 133/133 forms PASS, 0 problems, 0 mojibake/BOM hits (2m11s) |
| **B1d `idempotent`** (fresh regen into a second location, byte-diff vs checked-in `ir_out`/`rc_out`/`layout_out`) | 0 diffs across all 133 forms' generated `.dfm.ir.json`/`.rc`/`_ids.h`/`.rcmeta.json`/`_layout.gen.{h,cpp}`/`_events.gen.json` (6s) |
| **B1d `rc_compiles`** (fast path: compiles the checked-in `rc_out` corpus as-is) | 133/133 clean on both `rc.exe` and `windres` (1m21s) |
| **B1d `layout_full`** (fast path: `g++ -std=c++14 -Wall -Wextra -fsyntax-only` over the checked-in `layout_out` corpus as-is) | 133/133 clean (42s) |

No discrepancy against B1b's/B1c's own already-recorded numbers (above,
and this file's B1b section) turned up on re-run -- all match exactly.

### Full-corpus coverage statement (of 22,627 total control nodes / 331
NONVISUAL / 22,296 visual)

- **G0-G1 (parse + census bijection)**: 22,627/22,627 nodes parsed, 0
  unparsed lines, 0 object/end imbalance, 133/133 forms have 0 duplicate
  leaf names.
- **G2-G4 (geometry/order/taborder)**: 22,296/22,296 visual nodes exact
  pixel match + declared sibling order; 43 named TabOrder-non-permutation
  exception groups (not a failure -- an enumerated, expected exception
  class per plan; see B1a's own gate report for the list).
- **G5 (`.rc` compiles)**: 22,586/22,586 emitted `CONTROL` entries across
  3,637 `DIALOGEX` templates compile clean on both `rc.exe` (authority)
  and `windres` (secondary) -- 0 fallback/uncurated classes across the
  full 72-class corpus.
- **G6 (`.res` binary read-back)**: 133/133 forms, 0 diffs against the IR
  (template count, per-ID uniqueness, class string, caption).
- **G7 (idempotency, this wave's own addition)**: 0 byte diffs between a
  from-scratch regeneration and the checked-in artifacts, across every
  generated text file in the corpus.
- **G9 (layout/events + geometry-taborder independent crosscheck +
  compile)**: 22,760/22,760 layout rows (133 ROOT + 3,504 SUBDLG + 143
  TABHOST + 18,649 LEAF + 331 NONVISUAL), 7,280 events, 0 unresolved
  colour tokens, 0 geometry/taborder mismatches against the independent
  scanner, 133/133 `_layout.gen.cpp` files compile clean.
- **Mojibake/BOM (S12 ratchet #4)**: 0 U+FFFD, 0 BOM across every file
  this wave's fresh regeneration produced (`.json`/`.rc`/`.h`/`.cpp`).

**Net: all 133 forms / all 22,627 control nodes pass every gate this
pipeline defines. Zero forms skipped, zero silently capped.** The only
items NOT put through a faithful-translation gate are the 12 controls
already recorded above under B1b's SKIP-AND-RECORD table (`TEasyMain`,
`TECameraPicolo`, `TAPHeadLabel` x10) -- re-confirmed present and
unchanged on this wave's fresh re-run (`skip_vendor_hits`/
`skip_no_source_hits` in the fresh `b1b` report match exactly).

Full per-form detail (all 133 rows, individually verifiable): see
`tools/dfm2rc/reports/b1d_per_form_table.md` and the machine-readable
`b1d_fidelity_report.json` / `b1d_idempotent_report.json`.

### Two real bugs found and fixed while wiring this wave's OWN new driver
(neither in B1b/B1c's generators -- both are new code introduced by this
wave, caught and fixed before landing)

1. **False-positive G7 diff on a hand-authored file.**
   `layout_out/DfmLayoutTypes.h` is deliberately hand-authored schema (not
   per-form generated -- see its own banner), but the first version of
   `run_b1d.py`'s idempotency diff treated every `.h`/`.cpp`/`.json` under
   `layout_out/` as generator output and flagged it `G7_MISSING_IN_REGEN`.
   Fixed by excluding that one named file from the diff's file-set (see
   `_STATIC_NONGENERATED_FILES` in `run_b1d.py`) -- and, separately, by
   copying it into the fresh regen root before the `fidelity` mode's
   compile-smoke step, since every generated `_layout.gen.h` `#include`s
   it by relative path and a fresh regen root does not otherwise have it.
2. **`std::system()` + multiple quoted paths silently breaks under
   `cmd.exe`.** The first version of `test_dfm2rc_pipeline.cpp` built a
   `"python.exe" "script.py" mode` command string and ran it via
   `std::system()`. Caught via `ctest --output-on-failure`, not
   anticipated: every one of the 4 new ctest targets failed in <1s with a
   Big5-locale `cmd.exe` error ("the filename, directory name, or volume
   label syntax is incorrect"). Root cause: Windows' `system()` shells out
   through `cmd.exe /c <string>`, and `cmd.exe`'s own argument re-parsing
   only preserves quotes intact when the WHOLE command line contains
   *exactly* 2 quote characters; with 4 (two quoted paths), it falls back
   to legacy behaviour that strips the first character and the LAST quote
   in the entire line -- severing the closing quote off the *first*
   argument and gluing both paths into one bogus filename with an
   embedded `" "`. Fixed by switching to `_spawnvp()` (`<process.h>`),
   which bypasses `cmd.exe` entirely and passes each argument as its own
   array slot, so this class of bug cannot occur regardless of how many
   arguments need quoting. Re-verified clean afterward via the real
   compiled ctest target (see table above), not just standalone.

### CMake wiring (append-only, landed as instructed in plan S6-B1d)

One new compiled ctest wrapper, `test_dfm2rc_pipeline` (built from the new
`tests/test_dfm2rc_pipeline.cpp`, inherits `ht9045_test_bootstrap`
automatically via the existing directory-scoped `link_libraries()`),
invoked by 4 separate `add_test()` entries with different arguments --
ctest natively supports this "one binary, several tests" shape, so no
need for 4 near-identical `.cpp` files:

| ctest name | mode | reads GOLDEN at test time? | measured runtime | explicit TIMEOUT |
|---|---|---|---|---|
| `dfm2rc_rc_compiles` | fast path: compiles checked-in `rc_out` corpus | no | ~85s | 300s |
| `dfm2rc_layout_full` | fast path: `g++ -fsyntax-only` over checked-in `layout_out` corpus | no | ~45s | 300s |
| `dfm2rc_fidelity` | full re-derivation from GOLDEN into `${CMAKE_BINARY_DIR}/dfm2rc_regen`, all gates G0-G9 + mojibake scan | yes | ~130s | 300s |
| `dfm2rc_idempotent` | full re-derivation from GOLDEN into `${CMAKE_BINARY_DIR}/dfm2rc_regen`, byte-diff vs checked-in artifacts (G7) | yes | ~7s | 300s |

Deliberate naming choice, flagged rather than silently substituted: the
plan's own S6-B1d prose names a 4th target "`test_DfmLayout`" alongside
the 3 `dfm2rc_*` names. This wave uses `dfm2rc_layout_full` instead, for
two reasons: (1) naming consistency with the other 3 `dfm2rc_*` targets
this same wave adds, and (2) to avoid any naming collision/confusion with
the sibling W7-B1c wave's own already-landed `DfmLayoutGen` ctest (a
different thing -- a curated 4-form *linked* test with real hand-written
assertions, `tests/test_DfmLayoutGen.cpp`). `dfm2rc_layout_full` is what
that sibling wave's own CMakeLists comment explicitly deferred to
"W7-B1d's job": bringing the full-133-form compile proof (previously only
run ad hoc via `python run_b1c.py`, outside `ctest`) into `ctest`.

All 4 new targets guarded by `find_package(Python3 COMPONENTS
Interpreter)` -- on a machine without Python3 they are cleanly not
registered (with a `message(WARNING ...)`), rather than failing the whole
CMake configure; this machine has Python 3.13.3 and all 4 register and
pass.

### Full-suite regression check (S12 ratchet #1)

Fresh incremental build (`cmake --build build_resume_verify_20260727 --
-j4`) after landing all of the above: clean, 0 errors, 0 new warnings from
this wave's own files. Full `ctest --timeout 300 --output-on-failure`
(no `-R` filter, whole suite, run once all sibling waves' concurrent
additions were also present): **103/107 passed**, and the 4 failures are
byte-for-byte the same pre-existing environment-drift set named in the
task brief (`config_db`, `IniFiles`, `ini_helpers`, `config_loaders`) --
zero regressions caused by this wave. (107, not 95, because this wave ran
concurrently with several sibling waves that also each added tests --
W7-C1's `TrayCore`/`LedCore`/`BtnPanelCore`, W7-C2's `TrayRender`/
`LedRender`/`BtnPanelRender`, W7-B1c's `DfmLayoutGen`, W7-A2's
`W7_A2_SearchTrayMagazineReturn` -- all of which also pass.)

### Design choices made where the plan under-specifies (documented, not
silently decided)

- **Regen root location**: `${CMAKE_BINARY_DIR}/dfm2rc_regen`, exactly as
  the plan's own S6-B1d prose names it -- confirmed `build_*/` is already
  in `.gitignore`, so this never lands in the repo tree.
- **`run_b1d.py` reuses B1a/B1b/B1c's library modules directly**
  (`dfm_parse`, `emit_rc`, `emit_layout`, `gates`, `check_g6`, `classmap`)
  rather than shelling out to `run_b1a.py`/`run_b1b.py`/`run_b1c.py`,
  because those 3 scripts hardcode their own output directories as
  module-level constants -- calling their library functions directly
  (which all take explicit output paths as arguments) lets every B1d mode
  target an arbitrary root without touching those 3 files at all.
