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
| **C1061** (`EJ1N/TextProcess.cpp`, 128-deep else-if breaking MSVC) | **Downgraded to near-irrelevant** for the product: it is one compiler's nesting limit on one function, and MinGW compiles it (`wb_publish.exe` carries the whole god-stack). **One consumer still needs it fixed: the MSVC second oracle** (`scripts/build_msvc.bat` builds all targets with no `--target`, so it dies on this file). **Citation correction 20260817:** every reference in this tree says `404-424`; those 21 lines are only the *tail*. The construct is `MyASCIIToDec` declared at `:293`, chain from `:297` to `:424` = 1 `if` + 127 `else if` = 128 levels. Anyone fixing it edits `297-424`. (The sibling `MyDeCodeASCII`, `:155-291`, is the same table in reverse but written as a `switch` — not a problem.) |
| `ui/MachineStateDlg` (built this session) | **KEEP — user decision 20260817.** A thin native monitor: 321 lines (`.cpp`) + 95 (`.h`), links `ht9045_webbridge` and no machine code, reads the same wire the browser reads. A **developer/verification convenience**, explicitly not the product UI. Consequence now settled rather than open: because it is MFC it needs MSVC, and because it consumes `tcp://127.0.0.1:8046` **something must keep publishing on 8046** — so the product exe keeps its `TcpTagPublisher` and F5 stays at 2 processes, not 1. |
| The tag bridge, `--pump`, `WB_SimPump` tests | **Core product path.** Unaffected. |

---

## 5. Boundaries and known hazards that outlive any plan

1. **V899 is read-only.** No exceptions. (Violated once, 20260814; reverted, with one real
   casualty — see the DEVLOG entry for that date.)
2. **`D:\HT9045\web` has ZERO git tracking.** Verified 20260817: `git ls-files web/` → 0 files.
   The product's UI is unversioned — that is the real hazard and one `git add web/` fixes it.
   **Correction to this entry's earlier wording**, which conflated two independent mechanisms and
   overstated the second:
   - "Move it into the repo" was wrong. `D:\HT9045` **is** the repo root, `web/` is already inside
     it and is **not** `.gitignore`d (`git check-ignore` → exit 1). Nothing needs moving; it has
     simply never been `git add`ed.
   - "Outside `allowedWriteRoots`" is literally true but **blocks nothing**. Three independent
     checks, all measured 20260817: (a) `.claude/settings.json` does not exist, so
     `scripts/ops/check-write-boundary.ps1` is **never invoked by Claude Code** — the only live
     `PreToolUse` hook is the user-level `guard-big5.ps1`, which decides on *encoding*, not path;
     (b) even if it ran, the policy sets `"confirmOutsideAllowed": false`, which suppresses the
     ask for paths outside the allowed roots — only `readonlyRoots` deny and only
     `.ini/.csv/.dat`-class extensions ask; (c) the user profile runs `defaultMode:
     bypassPermissions`. Adding `web/` to `allowedWriteRoots` would align the document with
     reality but changes no behaviour.
3. **The 906 tree shares runtime config with the production machine.** `common.cpp:89`'s
   `asGeneralPath` points at the production `system\Gerneral.ini`, and `LoadMachineConfig()` writes
   it. `--dry` only redirects that one path; `lastdata.dat` paths are hard-coded literals. Also
   measured 20260813: **the V906 test suite itself writes** `system\BinCount.txt`. Splitting the
   experimental machine's data from production is a prerequisite for any long-running work.
4. **Never report simulated state as machine state.** The `SIM` prefix on the state word exists
   because nothing in the translated tree can currently set `SystemStart`/`fAllMotorHome`.

---

## 6. Decisions — settled 20260817

The four questions this section used to ask are answered. Recorded as decisions, not options.

| # | Question | Decision |
|---|---|---|
| 1 | Is the 296-tag page the spec, or a mockup to redesign? | **It is the target, as-is.** `D:\HT9045\web\index.html` stays put; the goal is that it *works* and updates live once the core starts. No redesign, no new screens. |
| 2 | Keep or delete `MachineStateDlg`? | **Keep** (§4). |
| 3 | Web folder — move, or extend the policy? | Neither is required; **`git add web/`** is the whole fix (§5.2 records why). |
| 4 | Extraction order — file-by-file or vertical slices? | **File-by-file.** |
| 5 | (new) Do the DFM forms get touched? | **No.** Not edited, not converted, not migrated. The web page replaces them; the `.dfm` files are left alone. |
| 6 | (new) One process, or the publisher/gateway sidecar split? | **One process** — see §7. |

## 7. The product process: one exe, decided 20260817

The user asked whether the sidecar split could be dropped in favour of what HT160S does — a BCB6
app that opens its own HTTP server so a browser can just connect. Recon (9 read-only agents,
2 adversarial verifiers) says yes, with one correction worth carrying:

- **The merged link shape already exists**: `wb_serve` links the full god-stack **and**
  `ht9045_webbridge`, and `nm -C build/wb_serve.exe` shows 321 `webbridge::WebBridgeServer::`
  symbols beside `MainProc` and `LoadMachineConfig`. Zero `CMakeLists` edits are needed.
- **The merged RUNTIME shape has never existed** — this is the verifier's refutation, and it is
  correct. No binary on disk holds all three halves: `wb_serve` has the HTTP server and
  `PumpInit`/`PumpTick` compiled in but **never calls them** (`grep -c Pump tools/wb_serve.cpp` →
  0) and has no `TcpTagPublisher`; `wb_publish` pumps and publishes but has 0 `WebBridgeServer`
  symbols. `MainProc()` ticking inside a process that is simultaneously running a live accept loop
  has no precedent here. Treat it as new work, not as a rebuild.
- **No structural obstacle.** Verified rather than assumed: no blocking accept (non-blocking
  listener, one thread, `select` 50 ms, accept only under `FD_ISSET`); no duplicate symbols
  (intersecting `libht9045_webbridge.a`'s 1,516 defined symbols against the god-stack's 14,568
  gives 136, of which 133 are STL instantiations and the residue is COMDAT `operator new/delete`
  and `sprintf`); no `atexit`/`signal`/console-ctrl/unhandled-exception handler anywhere in the
  tree; no test asserts link shape; port collision fails loudly because `SO_REUSEADDR` is
  deliberately unset on both listeners.
- **Four behavioural rules must be hand-carried**, and `tools/wb_serve.cpp` — the nearest template
  — violates three of them: it has **no `SetErrorMode`** (root-CMakeLists targets do not get
  `tests/`'s modal-dialog suppression, so an unattended fault opens an invisible WER box and
  blocks forever with nothing logged); it calls **`LoadMachineConfig()` unconditionally**
  (`tools/wb_serve.cpp:90`), which **seeds keys into the shared production `system\Gerneral.ini`**
  — `wb_publish`'s `loadConfig = (!pump || withCfg)` rule must be adopted instead; and
  `PumpInit()` must be called **after** any config load, because `ReadGeneralIni` overwrites five
  of the globals it pins.
- **What merging costs.** The "gateway links no machine code" property is enforced *nowhere* — no
  test, no assertion, only comments. It is delivered by static-archive extraction, not by the link
  list: `wb_gateway`'s generated link line does carry `libht9045_public.a` and `libvclcompat.a`,
  and 80 cJSON symbols **are** extracted into `wb_gateway.exe`. Keep `wb_gateway` as a target so
  that property survives *somewhere*; merging does not delete it.
- Because §6.2 keeps `MachineStateDlg`, the product exe also runs the `TcpTagPublisher` on 8046.
  One `TagSnapshot`, one publisher thread (the pump), two independent reader threads.
  **F5 goes from three processes to two.**

## 8. The number that decides the schedule: 0 of 296

Measured 20260817 by booting the page's own module graph headless and calling `bind.boundTags()`.

| set | count | denominator |
|---|---|---|
| bound by the page | 296 | distinct tag names = 234 `[data-tag]` + 62 `[data-tone-tag]` |
| published by the core | 61 | `stage()` calls = 43 machine + 18 process |
| bound **and** published | 37 | of 296 bound (12.5%) |
| …of which staged as unconditional null | 35 | of 37 (94.6%) |
| bound, **not** published | **259** | of 296 bound (87.5%) |
| published, not bound | 24 | of 61 published (39.3%) |

**The "235-tag gap" quoted in earlier entries was wrong** — it assumed the 61 published tags were
a subset of the 296 bound. Only 37 are. The gap is **259**.

Worse, and this is the headline: **live machine values reaching the screen today = 0 of 296.**
All eight published tags that can carry a real machine value (`machine.id.*`,
`machine.customerCode`, `lastset.*`) are bound by **nothing**. Only `clock.text` and
`machine.state` ever carry a value at all, and both are gated on `--pump`.

The 259, classified by why each is missing:

| bucket | count | % of 259 | what it means |
|---|---|---|---|
| **NO-SOURCE** | 93 | 35.9% | golden produces nothing either — the page invented it. 32 `arm.<zone>.tone`, 19 `status.slot<N>` (+19 tones), 11 `status.<name>.tone`, 6 `temp.led.<N>`, 6 `tester.led.<N>`. The slots self-declare as placeholders rendering `"----"`. |
| **SOURCE-EXISTS** | 84 | 32.4% | the variable is translated and reachable **today**; publishing is a one-line registration. 32 `site.arm*.s*` (← `LastSet.bUseTestSocket`, inside the live blob), 32 `arm.<zone>` (← `UN150Read[]`, already read by `WebBridgeTags.cpp`), 8 `speed.*`, 6 `bin.*`, 3 `lot.auto*.nowTrayId`, `recipe.current`, `startmode.value`, `user.level`. |
| **SOURCE-UNTRANSLATED** | 64 | 24.7% | blocked behind four modules absent from the tree (`main.cpp`, `cSortCT.cpp`, `uLotInfo.cpp`, `cContactCT.cpp`): 32 `cat.arm*.s*`, 14 `sort.*`, 13 `lot.*`, 4 `speed.shuttle*`, `machine.mode`. |
| **SOURCE-IS-FORM** | 18 | 6.9% | all of `contact.*` — raw per-socket counters exist, the KIND grouping is the form's aggregation. |

Two caveats the measurement states about itself: the 32 `cat.*` could equally be NO-SOURCE (the
page's own `tagmap.js` says the control is unresolved), which would move 12.4% of the gap; and the
exact `bUseTestSocket` subscript for a given `site.*` tag is unverified.

**Transport is NOT the blocker.** `WebBridgeServer.cpp` already emits all four frame types
(`snapshot`/`patch`/`ack`/`alarm`) at the shapes `js/transport/ws.js` expects, and already binds
`/ht9045`. The entire gap is which tags `WebBridgeTags.cpp` stages. Two sharp edges to respect:
the page handles `snapshot` and `patch` in the **same** switch arm and never clears state, so a
snapshot cannot retract a tag — send explicit null to blank one; and the page defaults to the
**mock** transport, so the URL must carry `?src=ws` (F5's `serverReadyAction` already does).
