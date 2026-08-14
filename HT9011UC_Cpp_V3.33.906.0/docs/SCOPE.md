# SCOPE — what the C++ migration is, and what it is not

> AI(W906-Scope) 20260814. Written after the user said the work had drifted, and set three
> boundaries: **the migration is 906 only**, `HT9011UC_Cpp_V3.33.906.0` is **one of the final
> product's folders**, and **the web UI replaces the DFM forms** because porting DFM to C++ is
> too hard. This file is the anchor; if a task cannot be traced to a line in §2, it is drift.

---

## 1. What the product is

Three parts. The C++ tree is one of them, not all of them.

| Part | Where | What it is |
|---|---|---|
| **C++ core** | `HT9011UC_Cpp_V3.33.906.0/` | Machine logic and control. The translated state machine, HAL, comms, and the tag bridge. Owns everything that touches hardware. |
| **Web UI** | `D:\HT9045\web` (⚠ see §5) | The operator interface. **Replaces the DFM forms**, not renders them. |
| **Runtime config/data** | `system/`, `config/`, `CFG/`, `IniData/` | Shared with the production machine today, which is a problem of its own (§5). |

The seam between core and UI is the **tag bridge** already built and verified: the core publishes a
tag snapshot over loopback TCP; a gateway that links no machine code serves the browser. That seam
is what makes "web UI + C++ core" a real architecture rather than a slogan, and it is the piece of
this session's work that survives the re-scope untouched.

---

## 2. In scope

1. **Translate the non-form C++ logic to completion.** Measured 20260813: 336,509 golden code
   lines, 313,904 done = **93.3%**. This is the least ambiguous work left.
2. **Extract the business logic out of the form files** — see §3, the crux.
3. **The tag bridge and the core's publish path.** Built and verified: `WebBridge/`,
   `WebBridgeTags.cpp`, `wb_publish`/`wb_gateway`, 61 tags, snapshot-only wire.
4. **Drive the machine from the C++ core**: the `MainProc`/`DoAllProcess` spine over the HAL, the
   home/start path, and eventually real hardware instead of the Sim HAL.
5. **The web UI itself** — the 296 tags it already binds are the specification of what the core
   must eventually publish. Today the core publishes 61.
6. **A write path (commands) from the browser to the core.** Not designed yet. Safety-critical:
   every command must re-enter the interlocks, and the browser must stay a view.

## 2b. OUT of scope — explicitly

1. **Every tree other than `HT9011UC_Cpp_V3.33.906.0`.** The V899 tree is the shipping product and
   is **read-only**. `HT9011UC_Code_V3.33.906.0_20260618` stays read-only golden reference.
2. **Route B — the V899 TCP snapshot port. CANCELLED.** It required editing the shipping binary,
   which the 906-only boundary forbids. The design work is preserved
   (`DESIGN_RouteB_V899SnapshotPort.md`, `docs/parked/WebSnapPort.*.txt`) because the recon inside it
   is valuable, but **nothing from it ships**. Consequence to accept: the browser and any native
   monitor show the **C++ core's** state, which is simulated until the core drives real hardware.
   There is no shortcut to real machine data that stays inside the 906 boundary.
3. **Porting DFM layout to C++ dialogs.** This is the user's decision in point 4 and it retires a
   whole line of work — see §4.

---

## 3. The crux: the 261,862 lines in the form files

Census 20260813, and the number that misleads:

| group | golden code lines | translated | % |
|---|---|---|---|
| non-form | 336,509 | 313,904 | 93.3% |
| **form** | **261,862** | **11,864** | **4.5%** |
| ALL | 598,371 | 325,768 | 54.4% |

Read naively, 45.6% of the migration is left and most of it is forms. **That reading is wrong once
the UI is web**, because a `TfXxx` form file holds two different things:

- **rendering and event plumbing** — control creation, layout, captions, colours, `OnClick`
  wiring, `.dfm` coupling. **Web replaces this. It must NOT be translated.**
- **business logic** — the calculations, sequencing and decisions that happen to live in a form
  because BCB6 put them there. `cContact.cpp`'s ~11,347-line contact-height state machine is the
  clearest example: it is machine behaviour wearing a form's clothes. **This must be translated.**

**Nobody has measured that split.** Until it is measured, every schedule for the remaining work is
guesswork, and the 4.5% figure actively misleads planning. So:

> **The first task under this scope is a measurement, not a translation:** per form file, how many
> golden code lines are rendering versus logic. Output: a ranked list of "logic to extract" with
> line counts, and a list of "rendering, deliberately dropped".

That number decides everything downstream — how much work is left, in what order, and whether the
remaining logic is worth extracting file-by-file or as vertical slices per machine function.

---

## 4. What happens to the scaffolding already built

The web-UI decision retires work. Saying so explicitly is cheaper than letting it rot.

| Asset | Status under this scope |
|---|---|
| `tools/dfm2rc/` (91 dialogs, 946 controls, layout/uimap generators) | **No longer on the product path.** Its output described DFM geometry so C++ could reproduce it; web does not need that. Keep as a *reference* for what each form contains and which controls map to which tags — that is genuinely useful for building web screens — but stop investing in it. |
| GA-3 (god-stack into the MFC exe) | **Retired.** It existed to make `HT9045.exe` the real app. |
| GA-4 / `ui/forms/FMainFirstLightDlg` (unwired) | **Retired** as a product path. |
| **C1061** (`EJ1N/TextProcess.cpp:404-424`, 127-deep else-if breaking MSVC) | **Downgraded to near-irrelevant.** It only ever blocked linking the god-stack into an MSVC target, i.e. GA-3. With no MSVC product target, it stops being a blocker. Still worth fixing eventually as a portability wart. |
| `ui/MachineStateDlg` (built this session) | **Keep, reclassified.** It is a thin native monitor: ~350 lines, links `ht9045_webbridge` and no machine code, reads the same wire the browser reads. It is a **developer/verification convenience**, explicitly not the product UI. Cheap to keep, cheap to delete. **User decision.** |
| The tag bridge, `--pump`, `WB_SimPump` tests | **Core product path.** Unaffected. |

---

## 5. Boundaries and known hazards that outlive any plan

1. **V899 is read-only.** No exceptions. (Violated once, 20260814; reverted, with one real
   casualty — see the DEVLOG entry for that date.)
2. **`D:\HT9045\web` is outside `allowedWriteRoots` and has ZERO git tracking.** The product's UI
   is currently unversioned. This must be fixed before serious web work starts, and it is a
   scope-level decision: either the web folder moves inside the repo, or the write-boundary policy
   is extended to cover it.
3. **The 906 tree shares runtime config with the production machine.** `common.cpp:89`'s
   `asGeneralPath` points at the production `system\Gerneral.ini`, and `LoadMachineConfig()` writes
   it. `--dry` only redirects that one path; `lastdata.dat` paths are hard-coded literals. Also
   measured 20260813: **the V906 test suite itself writes** `system\BinCount.txt`. Splitting the
   experimental machine's data from production is a prerequisite for any long-running work.
4. **Never report simulated state as machine state.** The `SIM` prefix on the state word exists
   because nothing in the translated tree can currently set `SystemStart`/`fAllMotorHome`.

---

## 6. Open decisions (user)

1. **The 296-tag web UI**: is it the specification to build towards, or a mockup to be redesigned?
   It currently binds 234 `data-tag` + 62 `data-tone-tag` nodes and its layout mirrors the legacy
   fMain screen. Building the core to fill 296 legacy-shaped tags is a very different project from
   designing screens for how the machine is actually operated.
2. **Keep or delete the native `MachineStateDlg`** (§4).
3. **The web folder's home** (§5.2) — inside the repo, or policy extension?
4. **Order of §3's measurement output**: extract logic file-by-file (mirrors the census, easy to
   track) or as vertical slices per machine function (InArm, Index, Shuttle… — each slice ends with
   something demonstrable on screen)?
