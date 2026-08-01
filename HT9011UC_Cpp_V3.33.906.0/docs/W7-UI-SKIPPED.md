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

---

## W7-F0-fix (2026-07-28) -- review-track fixes for F0's false citations /
self-contradictions / doc gaps (2 MEDIUM + 6 LOW from the independent
review of W7-A1/W7-F0)

Per plan S12.6, the two deliberate deferrals below must be recorded here,
not only in source comments. Both were already disclosed inline by F0; this
entry is the required doc-side pointer, not a new decision.

1. **TfMainMemo / TfMainMemoLines NOT repointed onto `vclcompat::TMemo`**
   (forms/FormWidgets.h:301-332). `vclcompat::TMemo` now exists and is the
   correct eventual target for `fMain->meShuttle1`/`meShuttle2`,
   `fAGV->mmE84Log`, `fLotInfo->mmTesterLog` -- but switching them is NOT a
   pure refactor: the current stand-ins' `->Lines->Add()` is a no-op and
   `->Lines->Count` is a permanently-0 int, whereas `vclcompat::TMemo::Lines`
   is a REAL `TStringList`. Repointing would make reachable two
   size-threshold flush branches that are dead today (`Automation/
   AGV_E84.cpp` `mmE84Log->Lines->Count>=500`; `Interface/
   TesterTCP_Socket.cpp` `mmTesterLog->Lines->Add`), which is a real
   behaviour change with no test observing it today -- explicitly deferred
   out of a wave whose contract is zero behaviour change. Fix = a one-line-
   per-member follow-up (`TfMainMemo` -> `vclcompat::TMemo`, drop
   `TfMainMemoLines`) landed as its OWN change with its own assertions on
   the flush paths. Full analysis: forms/FormWidgets.h:301-332.

2. **`vclcompat::TStringGrid` TObject-base gap -- CLOSED this pass, recorded
   for the historical trail.** MEDIUM-2 of this review round found
   `vclcompat::TStringGrid` (vclcompat/StringGrid.h) declared with NO base
   class, while `fMain->AutoCleanStringGrid` is a golden `TStringGrid*`
   (golden main.h:457) that in real VCL derives from TObject like every
   other control -- so `SetSVDataPointer(..., AutoCleanStringGrid, ...)`
   would have silently bound the `void*` overload instead of the `TObject*`
   one (SecsSvEcRegistration.h:133 vs :149; the same R8/SS9-R8 hazard class
   `vclcompat/Controls.h`'s file-head note documents for the other stock
   widgets), with ~1,740 real SV/EC registrations still to land on top of
   it. AI(W906-F0fix) 20260728: closed by adding `: public
   vclcompat::TObject` to `TStringGrid` (verified safe first: every
   construction site tree-wide is `new TStringGrid(...)` through a pointer,
   no by-value/aggregate-init use exists) and extending
   `tests/test_w7_f0_controls_guard.cpp`'s static_assert coverage to include
   it, proven capable of failing by temporarily reverting the base and
   confirming the guard's static_assert fires (then restoring it -- see
   that test file's own note). No longer a blocking prerequisite for the
   upcoming SV/EC registration wave.

### Disclosure item (LOW-7): a preserved GOLDEN BUG whose UB shape shifted
(not its behaviour) under this wave

`Automation/SCK_ART_Remainder.cpp` (~line 1651, near golden
`SCK_ART.cpp:3636`) carries a verbatim-preserved GOLDEN BUG:
`Str.sprintf("LOT_ID:%s", fLotInfo->edtSysLotID);` passes a **widget
pointer** where the format string wants a `char*` -- undefined behaviour in
golden already (it reads whatever bytes happen to sit at the start of the
`TEdit` object as if they were a C string). Verified this pass: before
W7-F0, `TfLotInfoEdit` was a non-polymorphic `struct { AnsiString Text; };`,
so this UB read the leading bytes of an `AnsiString` (itself just a
`char*`-like handle in this codebase's AnsiString shim). After W7-F0,
`TfLotInfoEdit` is a typedef of `vclcompat::TEdit -> TCustomEdit ->
TControl -> TObject`, and `TObject` is polymorphic (has a virtual
destructor, vclcompat/TStringList.h:32-35) -- so the object now has a
vptr as its first member, and this same UB now reads a **vptr** instead.
Both are equally undefined and equally golden's own pre-existing bug (not
introduced or fixed by this wave); what changed is *which* garbage bytes
get misread, which is an observable difference on a reachable branch inside
a wave contracted to zero behaviour change. Documented at the call site
(`Automation/SCK_ART_Remainder.cpp:1651`, comment only, code left
untouched) and here, per plan S12.6, so the next reader finds both: that
golden's own behaviour here was already garbage, and that the ported
tree's flavor of garbage shifted.

---

## W7-F2-fix (2026-07-29)

Review-track disclosures for W7-F2 (TU-local seam retirement + SckArt state
consolidation), recorded here per plan S12.6. **Four** code/test sites cite this
heading; until 2026-07-31 it did not exist, because the W7-F2-fix pass was
interrupted before it wrote the doc side. **AI(W906-W7-DOCfix) 20260731**: the
heading keeps the wave's own date; every figure below was re-derived from the
tree and from GOLDEN on 2026-07-31, not transcribed from the wave's report.

**AI(W906-W7-DOCfix2) 20260801**: the opening word was "Five"; it is four.
Re-counted this run with a whole-tree scan of every `.c`/`.cpp`/`.h`/`.hpp` for
the literal `W7-F2-fix`: **4 lines in 3 files** -- `csystem.cpp` x1,
`Automation/SCK_ART_Remainder.h` x2, `tests/test_w7_f2_sckart_state.cpp` x1.
Same count at `f61e25e` (`git grep`, 4 lines / 3 files), so this was never five.
The sibling W7-L1 section's "One code site cites this heading" was re-counted the
same way and is exact (`asendic_Auto2.cpp`), so it is left alone.

Per this file's convention, GOLDEN is cited by `file:line` (stable, read-only);
the ported tree is cited **by symbol name only** (KNOWLEDGE gotcha 10).

### 1. Six seam initialisers in `csystem.cpp` diverge from golden, and only a source-text tripwire guards them

`csystem.cpp` holds two file-`static` SckArt seam structs, `W7C1_TfSCKARTSeam`
and `W7C2_TfSCKARTSeam`, reached only through the file-local `W7C1_SCKART` /
`W7C2_SCKART` macros. Six of their ctor mem-initialisers construct a value
golden does not. Both init-lists and all four golden sites were re-read this
pass:

| seam ctor | field | ported | golden | golden site |
|---|---|---|---|---|
| `W7C1_TfSCKARTSeam` | `iLOTSTATUS_L` | 0 | 3 | `Automation/SCK_ART.cpp:46` |
| `W7C1_TfSCKARTSeam` | `iTesterType` | 0 | 1 for every `CUSTOMER_CODE` except `CC_SCK` | `Automation/SCK_ART.cpp:42` then `:111-114` |
| `W7C2_TfSCKARTSeam` | `iLOTSTATUS_W` | 0 | 1 | `Automation/SCK_ART.cpp:44` |
| `W7C2_TfSCKARTSeam` | `iLOTSTATUS_R` | 0 | 4 | `Automation/SCK_ART.cpp:47` |
| `W7C2_TfSCKARTSeam` | `iLOTSTATUS_A` | 0 | 6 | `Automation/SCK_ART.cpp:49` |
| `W7C2_TfSCKARTSeam` | `iTesterType` | 0 | 1 (as above) | `Automation/SCK_ART.cpp:42` then `:111-114` |

Golden's `TfSCKART` ctor assigns the whole status family explicitly at
`Automation/SCK_ART.cpp:42-49` (`iTesterType=0`, then `NONE=0 W=1 T=2 L=3 R=4
F=5 A=6`), then at `:102` takes `if(CUSTOMER_CODE==CC_SCK)` for SCK only and
`else { iTesterType=1; }` at `:111-114` for every other customer.

**Why the guard is text-level.** Both structs and both instances
(`W7C1_fSCKART_ext` / `W7C2_fSCKART_ext`) are `static` at file scope inside
`csystem.cpp` and appear in no header, so no test translation unit can name
them and no runtime observation is available. `tests/test_w7_f2_sckart_state.cpp`
PART C therefore reads `csystem.cpp`'s own source text and asserts each of the
six by symbol name.

**RESIDUAL EXPOSURE -- the part that is NOT closed:**

- It is a source-text regression pin, not a behavioural observation. It cannot
  show the seams behave like golden; they demonstrably do not.
- ~~The parsed region ends at the first `{` after the ctor-name anchor, so the
  ctor **body** is never inspected. An `iTesterType=1;` statement added inside
  the body would change the constructed value with the pin still green~~ -- and
  body assignment is *golden's own shape*, so it is precisely the edit a future
  reconciliation wave would write.
  **AI(W906-W7-DOCfix2) 20260801, measured not assumed**: as of today the
  working-tree copy of `tests/test_w7_f2_sckart_state.cpp` DOES inspect the ctor
  body and DOES reject a body assignment. Evidence, run this pass entirely in a
  scratch directory (no managed file and nothing under `build/` was written):
  a snapshot of that file taken today, compiled off-tree against a scratch copy
  of `csystem.cpp`, gives `PASS 46/46` unmutated and `FAIL 1/46` on check C5
  ("... ctor BODY assigns `iTesterType` after the init-list ...") once
  `{ core.sLOTSTATUS = "/x"; iTesterType = 1; }` is added to
  `W7C1_TfSCKARTSeam`'s ctor. **The hole this bullet described is closed; the
  bullet is struck through rather than deleted so the trail survives.** That file
  is owned by another track this round and its edit is uncommitted, so this is an
  observation of a snapshot, not a claim about anyone's finished work -- the
  authority remains that test file's own header block, exactly as the caveat at
  the end of this list already says. A different, subtler hole in the same
  mechanism (the comment stripper must be string-literal-aware) is written up as
  KNOWLEDGE gotcha 17.
- No offline path exercises the ART branches these constants gate
  (`CosFunction.bUseSCKART` and friends are false offline), so no behavioural
  test is possible today. That is the reason this entry exists rather than a
  test.
- A sibling track is hardening this guard in the same round as this entry.
  **Treat that test file's own header block as the authority on the guard's
  final shape, not this paragraph** -- this paragraph is only guaranteed to
  describe the state as of 2026-07-31.

Do not "tidy" any of the six initialisers without changing the corresponding
expected value in the guard in the SAME edit.

### 2. Control-flow divergence: a non-`CC_SCK` machine ENTERS a block golden SKIPS

Divergence D2 above is behavioural, not cosmetic. `DoCleanOutFinishCheck` in
`csystem.cpp` gates a whole Flex-ART block on
`... && CosFunction.bUseSCKART && W7C1_SCKART->iTesterType==0`. Golden reaches
that test with `iTesterType==1` on every customer except `CC_SCK`
(`Automation/SCK_ART.cpp:111-114`), so golden **skips** the block; the port
constructs 0, so the port **enters** it. The same inversion applies to the five
`W7C2_SCKART->iTesterType==0/==1` reads in `DoART_AfterCleanOut`.

Inert offline only because `CosFunction.bUseSCKART` is false by default -- i.e.
the divergence is masked by configuration, not by code. Reported, not
reconciled, per plan S6-F2 / S9-R9.

### 3. A TU-local stand-in still intercepts reads of a field that has become real

`Automation/SCK_ART.cpp` carries a TU-local stand-in array for
`LastSet.bUseTestSocketEE` (a gate-#3 stand-in from an earlier wave, cited to
golden `LastSet.h:392`) plus the macro that routes reads to it. That field now
**exists for real** on the shared `LastSet` global -- the ported
`canary_support.h` declares `bool bUseTestSocketEE[2][4][8];` on the shared
`LastSet` struct, cited to the same golden `LastSet.h:392`, and golden
`MachineType.h:386-387` gives `MAX_SOCKET_ROW 4` / `MAX_SOCKET_COL 8`, so the
two have identical extents.
*(AI(W906-W7-DOCfix2) 20260801: this sentence previously carried two PORTED-tree
line numbers, `canary_support.h:147` and `MachineType.h:391-392` -- forbidden by
KNOWLEDGE gotcha 10 and by this section's own stated convention two paragraphs
up. Both happened to still be right today, and both are now anchored by symbol
instead. The two `MAX_SOCKET_*` defines are cited to GOLDEN, where they sit at
`MachineType.h:386-387`; re-read from the cp950-decoded golden this run, as were
`LastSet.h:392` and the `Automation/SCK_ART.cpp` sites below.)*

W7-F2 made `SckArtRem_ClearLotInfo` call the sibling's `SckArt_DoAutoSocketOff`
directly (closing a golden `:877` fidelity gap), which made this file **reach**
the read golden performs at `Automation/SCK_ART.cpp:1340-1343`
(`if(LastSet.bUseTestSocketEE[k][i][j]==true) LastSet.bUseTestSocket[k][i][j]=true;`).
So the stand-in is now stale and shadows real storage: a write to the real
`LastSet` field would not be seen by this read.

Left as-is deliberately: retiring the stand-in changes behaviour on golden's
`IniConfig.bI35UseThirdSiteControlByEngineer` arm. Inert offline regardless --
golden's whole `DoAutoSocketOff` body sits inside
`if(TestIF_File.bSCKART_AutoSocketOff)` (`Automation/SCK_ART.cpp:1238`), false
by default.

### 4. `vclcompat::TStringList::GetText()` uses a bare `\n`; real VCL uses CRLF after EVERY line

Verified this pass by reading the implementation: `GetText()` joins items with a
single `'\n'` and appends **no** trailing break. Real VCL
`TStrings::GetTextStr` emits `sLineBreak` (CRLF on Win32) after every line,
including the last.

Consequence today: `TListTrayIDShim` in `acatchtray_shims.h` was on W7-F2's
list of TU-local value-holders to retire onto `vclcompat`, and was
**deliberately not retired**, because its `Add()` appends `"\r\n"` and its
`Text` is written straight to disk by `acatchtray.cpp`'s two
`WriteDataToFile(asTrayIDByLot, ...->Text)` call sites. Aliasing it onto
`vclcompat::TStringList` would silently change the on-disk tray-ID-by-lot
record bytes. Of the two, the shim is the closer match to real VCL; the
divergence is in `vclcompat::TStringList`, which was outside W7-F2's write
scope.

**Why this is recorded here and not only at that call site.** As of 2026-07-31
this divergence is described in exactly ONE place in the tree: the comment
block above `TListTrayIDShim` in `acatchtray_shims.h`. Every future wave that
translates another `->Text`/`GetText()` consumer inherits the same trap, and
each one silently changes on-disk bytes if it aliases onto
`vclcompat::TStringList` without noticing. Fixing `GetText()` itself is a
`vclcompat`-wide behaviour change that needs its own wave with its own
assertions on every existing consumer -- not a drive-by.

### AI(W906-W7-DOCfix2) 20260801 -- honesty sweep of this section

Everything above was written by the 2026-07-31 pass. Re-derived by hand this run,
not taken on trust; each item states what was actually checked.

- **The six seam initialisers** still read `0` today: `W7C1_TfSCKARTSeam` gives
  `iTesterType(0)`, `iLOTSTATUS_L(0)`; `W7C2_TfSCKARTSeam` gives `iTesterType(0)`,
  `iLOTSTATUS_W(0)`, `iLOTSTATUS_R(0)`, `iLOTSTATUS_A(0)`. All six golden
  counterparts re-read from the cp950-decoded golden: `Automation/SCK_ART.cpp:42`
  `iTesterType=0`, `:44` `iLOTSTATUS_W=1`, `:46` `iLOTSTATUS_L=3`, `:47`
  `iLOTSTATUS_R=4`, `:49` `iLOTSTATUS_A=6`, and `:102` `if(CUSTOMER_CODE==CC_SCK)`
  with `else { iTesterType=1; }` at `:111-114`. Table stands.
- **"appear in no header"** confirmed: `W7C1_fSCKART_ext` / `W7C2_fSCKART_ext`
  occur only in `csystem.cpp` (as `static` definitions plus a comment) and in
  comments inside the pin test; no header declares them.
- **Section 2's "five `W7C2_SCKART->iTesterType==0/==1` reads"** confirmed: six
  textual occurrences in `csystem.cpp`, of which one is a comment, leaving **5**
  live reads, all after the real `DoART_AfterCleanOut` definition.
- **Section 3's golden citations** confirmed: `Automation/SCK_ART.cpp:877`
  `DoAutoSocketOff(true);`, `:1238` `if(TestIF_File.bSCKART_AutoSocketOff)`,
  `:1340-1343` the `bUseTestSocketEE`->`bUseTestSocket` copy, `LastSet.h:392`.
- **Section 4's `GetText()` claim** confirmed by reading the implementation
  again: `vclcompat::TStringList::GetText` joins with a single `'\n'` and appends
  no trailing break.
- **Not re-verified, flagged rather than restated**: the claim that
  `TListTrayIDShim`'s divergence "is described in exactly ONE place in the tree"
  is a whole-tree negative that this pass did not re-run; treat it as of
  2026-07-31 only.

---

## W7-L1 (2026-07-29)

Deferral record for W7-L1 (`asendic_Auto2.cpp`, the first landing of the
asendic feed-SM family), per plan S12.6. One code site cites this heading.
Created 2026-07-31 by **AI(W906-W7-DOCfix)**; the heading keeps the wave's date.

### `AutoCylinderUp` / `AutoCylinderMiddle` / `AutoCylinderLower` are no-op stubs -- deferred

`acatchtray_shims.cpp` defines all three as
`bool AutoCylinderXxx(int, int, int, bool) { return true; }`. Golden defines the
real state machines in `asendic.cpp`: `AutoCylinderUp` at `:562-765`,
`AutoCylinderMiddle` at `:767`, `AutoCylinderLower` at `:937`. Every
`if(AutoCylinderXxx(...))` guard in the translated `asendic_Auto2.cpp`
therefore succeeds instantly and unconditionally. Retiring the stubs is
deferred to a later wave.

**Retiring them is NOT merely transcription.** Two independent hazards, both
established by recon on 2026-07-31 and written down here because they are
written down nowhere else:

**(a) With the default offline all-disabled config, the real state machine
would never converge -- "always succeeds instantly" would become "never
succeeds".** Golden `AutoCylinderUp` mixes two opposite disabled-cylinder
conventions:

- The mid-cylinder **predicate** `AutoCylinderMidIsOn` (golden
  `asendic.cpp:529-560`) opens with
  `if(Cylinder[CylinderNameMid].Enable==false) bRet=true;` -- it returns TRUE
  when the cylinder is disabled. `case 100`'s non-ART arm
  (`asendic.cpp:661-662`) uses this predicate, so it advances to `Task=200`.
- `case 201`'s non-ART arm (`asendic.cpp:720-724`) instead demands the **raw**
  status: `if(Cylinder[CylinderNameMid].OnStatus()==false) { Task=1; return
  false; }`. `TMyCylinder::OnStatus` (golden `mycylin.cpp:123-149`) returns
  FALSE when `OnSenEnable==false`.

So on the default offline config the SM would walk `1 -> 50 -> 100 -> 200 ->
201 -> 1` forever and never return true, stalling every landed test that walks
an Auto lifter path. Whoever retires the stubs must supply an enabled-cylinder
fixture (or an explicit Sim convergence seam) in the SAME change -- swapping the
stub for the real body alone will turn the suite red in a way that looks like a
translation bug and is not one.

Note the underlying trap, which is a general one: `OnSensor()`/`OffSensor()`
(golden `mycylin.cpp:151-188`) return TRUE when disabled and key off `Enable`,
while `OnStatus()`/`OffStatus()` (`:123`, `:190`) return FALSE when disabled and
key off the *different* flags `OnSenEnable`/`OffSenEnable`. See KNOWLEDGE
gotcha 12.

**(b) Parameters 2 and 3 (`CylinderName`, `CylinderNameMid`) are NOT symmetric
inside the real bodies, and golden's own call sites disagree about their
order.** The asymmetry is plain in golden `asendic.cpp`: `case 1` does
`Cylinder[CylinderName].On();` unconditionally but gates the mid one on
`if(Cylinder[CylinderNameMid].Enable)` (`:584-586`); `case 100` reads
`CylinderName` through `OnStatus()` and `CylinderNameMid` through the predicate
(`:661-662`); `case 201` reads only `CylinderNameMid` (`:720`).

Measured across every golden call site of the three functions (88 sites in 7
`.cpp` files, classified by whether the `_Up` or the `_Selector` cylinder is
passed second):

| golden file | `(_Up, _Selector)` | `(_Selector, _Up)` |
|---|---|---|
| `asendic_Auto2.cpp` | 0 | **12** |
| `asendic_Auto.cpp` | 19 | 5 |
| `asendic_Auto_RT.cpp` | 21 | 4 |
| `csystem.cpp` | 7 | 2 |
| `acatchtray.cpp` | 8 | 0 |
| `AutoRetest.cpp` | 5 | 0 |
| `uhome.cpp` | 5 | 0 |

`asendic_Auto2.cpp` is the only golden file that passes them reversed at
**every** one of its sites, and it does so unconditionally. The other 11
reversed sites, spread over 3 files, are all inside
`if(bARTUnloaderUseTwoCylin(Pos)) ... else ...` pairs -- i.e. those callers
swap the two arguments **at runtime on a flag** (e.g. golden
`asendic_Auto.cpp:809-824`, `:784` vs `:788`).

**The ported `asendic_Auto2.cpp` reproduces the reversal FAITHFULLY at all 12
sites and must NOT be "fixed".** Re-counted this pass: 12 of 12 ported call
sites pass `(1, C_Auto2_Selector, C_Auto2_Up)`, matching golden verbatim.

While the stubs return `true` unconditionally this asymmetry is completely
invisible and no test can observe it. Recorded so that no future agent
"normalises" a call site to match its neighbours.

**AI(W906-W7-DOCfix2) 20260801 -- both counts in this subsection re-derived, and
one thing the assertions above do NOT cover.**

- The **88-site table** reproduces exactly. Scanning every golden `.cpp` (`.svn`
  excluded, `//` comments stripped) for calls to `AutoCylinderUp`/`Middle`/`Lower`
  and classifying by whether the `_Up` or the `_Selector` cylinder is argument 3
  gives 91 textual matches, of which 3 are the definitions in `asendic.cpp`,
  leaving **88 call sites in 7 files** split exactly as tabulated:
  `asendic_Auto2.cpp` 0/12, `asendic_Auto.cpp` 19/5, `asendic_Auto_RT.cpp` 21/4,
  `csystem.cpp` 7/2, `acatchtray.cpp` 8/0, `AutoRetest.cpp` 5/0, `uhome.cpp` 5/0.
  The runtime-swap example is real too: golden `asendic_Auto.cpp:782-789` and
  `:809-824` are `if(bARTUnloaderUseTwoCylin(Pos)) ... else ...` pairs whose two
  arms pass the pair in opposite orders.
- The **"12 of 12"** claim reproduces: the ported `asendic_Auto2.cpp` has 12
  non-comment call sites and all 12 pass `(1, C_Auto2_Selector, C_Auto2_Up)`.
- **What is NOT covered, proven this run rather than argued**: the landed
  assertions for this file observe *trajectory*, not *commanded output*. Against
  the `f61e25e` test (46 assertions, `46 passed, 0 failed` baseline), three
  independent mutations of `asendic_Auto2.cpp` -- inverting every conveyor
  `SW[SwACAuto2]`/`SW[SwACAuto2CW]` `On()`/`Off()`, swapping every cylinder
  `.Push()`/`.Pop()`, and deleting all **19** `MOT[...].ClearTray(__FUNC__)` call
  sites -- each left the suite at `46 passed, 0 failed`. (Compiled off-tree and
  linked against a scratch copy of `libht9045_sm.a`; nothing under `build/` and
  no managed file was written.) Note 19, not the "four" the hand-off brief said.
  Written up as KNOWLEDGE gotcha 18. Another track is closing this in the same
  round; the numbers here describe the committed baseline, not that track's
  result.

---

## W7-DOCfix (2026-07-31) -- W7-L family scope: `asendic_Scanner.cpp` is EXCLUDED from translation

Main-loop decision (plan S2 D15). `asendic_Scanner.cpp` is **not** translated
and is **not** a member of the W7-L1 asendic family. The family is therefore
**6 files**, not the 7 the plan's S6 W7-L table originally listed.

Evidence, each item re-derived on 2026-07-31 by **AI(W906-W7-DOCfix)**:

1. **It is not in golden's build at all.** The string `Scanner` occurs **0**
   times in `HT9045.bpr` (**87,463 bytes**) and **0** times in `HT9045.mak`
   (**19,405 bytes**) -- so it appears in neither the `.bpr` FILELIST nor the
   `.mak` OBJFILES. For contrast, `asendic_Auto2` occurs 3x in the `.bpr` and 1x
   in the `.mak`, `asendic_Empty` likewise 3x / 1x.
   - **AI(W906-W7-DOCfix2) 20260801 correction to the two sizes.** This item
     originally gave 86,271 and 19,030 "bytes". Those are not byte counts: they
     are newline-*normalised character* counts, i.e. what Python reports when the
     file is opened in text mode and every CRLF collapses to one `\n`. Re-measured
     in binary mode this run: `HT9045.bpr` = **87,463** bytes with 1,192 CRLF
     pairs (87,463 - 1,192 = 86,271, the old figure); `HT9045.mak` = **19,405**
     bytes with 375 CRLF pairs (19,405 - 375 = 19,030, likewise). The four
     occurrence counts above (`Scanner` 0/0, `asendic_Auto2` 3/1,
     `asendic_Empty` 3/1) were re-counted over the raw bytes this run and are
     correct as written.
2. **golden `asendic.h` omits it.** `asendic.h:6-12` includes exactly seven
   sibling headers -- `asendic_Loader.h`, `asendic_Loader_RT.h`, `asendic_Auto.h`,
   `asendic_Auto_RT.h`, `asendic_Auto2.h`, `asendic_Empty.h`, `asendic_Color.h`
   -- and not `asendic_Scanner.h`.
3. **Its top-level function has zero callers tree-wide.** `DoLoad_Scanner`
   (golden `asendic_Scanner.cpp:223`) appears in exactly two places in the whole
   golden corpus: its own definition and its declaration at
   `asendic_Scanner.h:7`. The file's other three functions
   (`DoSupplyNewICTray_Scanner:27`, `DoTrayZLoadTrayToWait_Scanner:135`,
   `DoLoadNewICTray_Scanner:176`) are called only from inside the file itself.
4. **It cannot compile even in golden, because it reads two `PROD_INFO_ST`
   members that do not exist.** `asendic_Scanner.cpp:141` and `:152` call
   `MOT[MMAuto1Z].MotorMove(Prod.Auto1Z_Up)` and `...(Prod.Auto1Z_Down)`.
   Neither `Auto1Z_Up` nor `Auto1Z_Down` occurs anywhere else in the golden
   corpus -- `cprod.h`'s only near-match is `int Auto1ZOffset;` at `cprod.h:313`,
   a different member. Translating the file would mean inventing both the struct
   fields **and** the motor target positions they carry, for a real Z axis with
   no value anywhere in the corpus. **Inventing machine-motion values is not
   acceptable** (plan S5-4: never invent functionality).

Consequence for planning: after `asendic_Auto2.cpp` landed, the remaining
untranslated members are the five files `asendic_Loader.cpp`,
`asendic_Loader_RT.cpp`, `asendic_Auto.cpp`, `asendic_Auto_RT.cpp`,
`asendic_Color.cpp`. Line totals are corrected in plan S6's W7-L table.

**One figure from the brief that this pass could NOT reproduce**: the brief
said translating the file would require fabricating *four* undefined
identifiers. Only **two** were found -- `Prod.Auto1Z_Up` and `Prod.Auto1Z_Down`.
The other two candidates that occur nowhere else in golden *code*
(`"JAM1130"`, `"JAM1168"`) are string literals, not identifiers, and both DO
exist as real alarm codes in the shared `Error/AlarmCodeList.txt`, so they are
not fabrications. The exclusion decision does not depend on the count: two
invented motion values are already disqualifying.

**AI(W906-W7-DOCfix2) 20260801 -- items 2-4 re-derived from the cp950-decoded
golden this run** (item 1's two sizes were wrong and are corrected in place
above):

- Item 2: golden `asendic.h:6-12` re-read; it includes exactly the seven sibling
  headers listed and not `asendic_Scanner.h`.
- Item 3: a whole-corpus scan of every golden `.c`/`.cpp`/`.h` finds
  `DoLoad_Scanner` in exactly **2** places -- `asendic_Scanner.cpp:223` (its
  definition) and `asendic_Scanner.h:7` (its declaration). Zero callers stands.
- Item 4: `Auto1Z_Up` and `Auto1Z_Down` each occur exactly **once** in the whole
  golden corpus, at `asendic_Scanner.cpp:141` and `:152` respectively; the only
  near-match in `cprod.h` is `int Auto1ZOffset;` at `:313`. The two alarm strings
  are likewise as described: `JAM1130` and `JAM1168` appear only inside
  `asendic_Scanner.cpp` in code, and both exist in the shared
  `Error/AlarmCodeList.txt`. `asendic_Scanner.cpp` is 372 lines.
