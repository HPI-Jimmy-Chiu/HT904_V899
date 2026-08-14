# DESIGN — Route B: a TCP snapshot port in the V899 production handler

> AI(ht9045-v899 / W906-RouteB) 20260814. Design round for the change the risk assessment
> (`RD5軟體_V899加TCP快照埠_風險評估_20260812_084103.md`) scoped but deliberately did not design.
> **This document must be reviewed before any V899 source is edited** — the target is the
> shipping binary that runs customer machines.
>
> Every decision below is grounded in a recon measurement, cited. Where the recon contradicted
> the risk assessment, the contradiction is called out rather than smoothed over.

---

## 0. What changed versus the risk assessment

Three of its assumptions moved, and all three moved in a useful direction.

| Risk assessment said | Recon measured | Effect |
|---|---|---|
| R1: "VCL socket events and the SECS reply budget both live on the UI thread" (high) | **Understated, but the conclusion flips.** The UI thread is not "the UI thread with a SECS stack on it" — it **is the machine's single execution thread**. `TRunControl::Execute` (`uruncontrol.cpp:42-70`) `Synchronize()`s `MainProc` onto it every ~1 ms at `tpTimeCritical` (`main.cpp:21553-21554`), and five more threads do the same (`uShuttleThread.cpp:59-63`, `MyPLC/MyPLC_IO_Modbus.cpp:240-247`, `uHeaterThread.cpp:68-76`, `EJ1N/OmronEJ1N.cpp:40/59`). | Less headroom than assumed — **but a 2 Hz emit is a rounding error next to what already runs there.** See §5 for what still must be measured. |
| R1 implied the SECS reply is sent from the socket event | **It is not.** `clientGemRead` (`uHGemEquipment.cpp:8921-8941`) only buffers bytes. Decode + reply happen in `ProcessSocketReceiveData`, whose sole caller is `uHGemEquipment.cpp:5229` inside `THGem::Timer1Timer` at **Interval=300** (`uHGemEquipment.dfm:555-561`). T3/T6 = **30 s** (`uHGemEquipment.cpp:454/456`, and `secs_gem.ini` on this box). | The existing design already self-inflicts **0–300 ms** of reply jitter against a **30 000 ms** deadline. Our emit competes with that, not with a tight budget. **R1 drops from high to medium.** |
| R4: Big5 → UTF-8 transcoding risk; v1 should send "numbers and ASCII only" | **The state text is ALREADY ASCII.** `sMacStatus[30]` (`cmydef.cpp:5205-5207`) is `"LOCK"`, `"EMG 1".."EMG 4"`, `"Power Off"`, `"Homing"`, `"Running"`, `"HALT"`, `"PAUSE"`, `"Alarm"`, `"Auto Retest"`, … and the `ShowNowStatus` call sites pass ASCII literals (`ckernel.cpp:946-1085`, sampled). | **v1 can publish the REAL state word**, not a numeric stand-in. R4 is avoided because the payload is already ASCII, not by omitting it. Non-ASCII bytes are still sanitised (§4) because one call site passes a variable (`EmgStr`, `ckernel.cpp:993`). |

---

## 1. Where the emit goes — `TfMain::Timer10Timer`

**Decision: `main.cpp:33100-33144`, `Interval=100` (explicit, `main.dfm:17065-17070`), emit on every
5th tick → 2 Hz.**

Why this timer and not one of the other thirteen (`main.h` lines 77/78/82/92/95/96/98/99/100/108/109/119/125/817):

1. **Always on and never re-driven.** `Enabled` comes from the DFM default and nothing in the tree
   touches it. Contrast `Timer1` and `Timer7`, which are called **synchronously from two 10 ms
   timers** (`mymessbox.cpp:503-504`, `note.cpp:3352-3353`): a "2 Hz" emit in either becomes
   **~100 Hz during every alarm screen** — precisely when the machine is least idle.
2. **Its period is explicit.** `Timer2/3/5/7/8/ESD` carry no `Interval` line at all, so their 1000 ms
   is only inferred from the VCL default. A snapshot cadence should not rest on an inference.
3. **No customer or feature gate.** `Timer9` needs `AUTO_SENSOR_INSTALL`; `TimerDLL` needs
   `CosFunction.bDLLCommands`; `Timer6` needs `bRCMDStart` or `CC_ChipMos_ZHUBEI`; `Timer4` needs
   `REAL_TIME_CCD`. Any of those would silently kill the feed on most machines.
4. **Never self-disables.** `Timer4` (`main.cpp:27548/27554/27560`), `Timer6` (`:30287`) and
   `Timer9` (`:31640`) are one-shots masquerading as periodic timers.
5. **One early-out only** — `if(InitialOK==false) return;` (`main.cpp:33102-33103`). Contrast
   `TimerScanKey`, which goes dark exactly during trouble: it returns while
   `bEnableEmployeeIDCheck` (`:30970-30971`) or while `bSECSGEMAlarm` is latched (`:30973-30979`).
6. **Its body cannot go modal.** 46 lines of widget writes (`main.cpp:33105-33122`); no
   `ShowErrorMessage` / `ShowMyMessage`, no file I/O, no `#ifdef`, no blocking loop.

### Explicitly rejected: `Timer1`
`main.cpp:2592-3729` (1138 lines) is the obvious-looking host and it is the wrong one.
Besides the ~100 Hz re-entry above, it contains an **unbounded blocking `do/while`** at
`main.cpp:2995-3003` that spins until `ProcessIndexSuckDestroy1/2` both return true, calling
`MySleepEx(1,true)` — which degrades to a bare `SleepEx` because `USE_EC_CHANGE` is commented out
(`MachineType.h:38`, `common.cpp:1758-1760`) — **with no message pumping inside the loop**. Adding a
socket emit to a handler that can already park there is not a risk worth taking. (That loop is a
pre-existing hazard and deserves its own ticket; it is out of scope here.)

---

## 2. The wire — snapshot-only, which is what makes this cheap

**Decision: emit exactly one line per tick:**

```
{"type":"snapshot","data":{"tag":value, ...}}\n
```

**No `hello`, no `patch`, no `ping`, no diffing, no generation counter, no per-peer state.**
Verified against the CONSUMER's parsing code, not its protocol comment:

- `"hello"` is only ever *produced* (`TcpTagLink.cpp:115`) and **never parsed**. Any frame whose
  `type` is neither `snapshot` nor `patch` falls through `TcpTagLink.cpp:547-553`, which just
  counts it. There is no handshake gate anywhere in `TcpTagClient`.
- `type == "snapshot"` → `sink.publish(data)` (`TcpTagLink.cpp:521-523`), a **full replace**.
- The browser-facing delta is **the gateway's own**, computed per browser connection against
  `c.lastSent` (`WebBridgeServer.cpp:1257-1264`) and skipped when empty (`:1272`). The gateway gates
  on its own local generation counter, bumped in `TagSnapshot::commitPublish` (`TagSnapshot.cpp:233`).

**Consequence: an unchanged snapshot re-sent 1000 times produces ZERO WebSocket bytes to the
browser.** A snapshot-only stream and a patch stream are indistinguishable downstream. Removals are
free (drop the tag; the WS hop turns "absent" into `"tag":null` → the `---` rendering), and reconnect
needs no special handling because the next tick re-establishes full state.

**The one obligation: every tick must carry every tag.** A tick that omits a tag makes it vanish and
the browser flickers `---`.

### Hard constraints from the consumer (each one rejects the WHOLE frame)
- **No arrays and no nested objects as values** (`TagJson.cpp:101-107`). Flatten to indexed names.
- **`data` must be present and be an object** (`TagJson.cpp:75-78`). `{"type":"snapshot"}` is a
  parse error, not an empty snapshot.
- **No `NaN` / `Infinity`** — not JSON. Send `null` (what V906's own writer does,
  `JsonWriter.cpp:241-246`).
- **No embedded NUL** — `cJSON_Parse` measures with `strlen` (`cJSON.c:1096`).
- **A line >1 MiB with no `\n`** silently drops the TCP connection. Our line is ~1 KB.

---

## 3. The socket

**Decision: create it in CODE, not as a `.dfm` component.** The project rule is that `.dfm` files
are Delphi-format and not hand-edited without an explicit instruction, and a dynamically created
`TServerSocket` is a standard VCL pattern that needs no template change. This also keeps the diff
to `.cpp/.h` only, which is far easier to review on a shipping binary.

- `ServerType = stNonBlocking` — matching **all 11** existing listeners in the tree
  (`main.dfm:17029/17039`, `uHGemEquipment.dfm:562-573`, `ATC/ATCInterface.dfm`,
  `automation.dfm:459`, `SCK_ART.dfm:2982`, `HS_Function.dfm:147/157`, `LoadCCDMap.dfm:101`, …).
  Non-blocking means socket events arrive as window messages on the UI thread — the same thread the
  emit runs on, so no locking is introduced anywhere.
- **Bind `127.0.0.1` only.** LAN exposure must never be the default (risk assessment R7). Remote
  viewing is the gateway's problem, one hop away, not the handler's.
- **Port 8046**, ini-overridable. **Verified free in V899**: the tree's DFM `Port` literals are
  `0, 21, 23, 1023, 1234, 3000, 5000, 5001, 5002, 5100, 6000, 6666, 7000, 9000, 9001, 10001`
  (`rg -h "Port = " --glob '*.dfm'`, 20260814). Every textual "8046" hit in the tree is hex inside a
  DFM image blob, not a port.
- **Default OFF, ini flag.** With the flag off: **no socket object is created and the timer does zero
  extra work** — the acceptance criterion is behaviour identical to the current release, item by item.

---

## 4. Building the string safely — what NOT to copy

`Public/cJSON.c` **is** compiled into the shipping project (`HT9045.bpr` lists `cJSON.c` and
`cJSON.obj`), and the gateway parses with cJSON too, so round-trip compatibility is native. Even so,
**emission is hand-built with `AnsiString`** concatenation: cJSON's printer allocates per call and we
are on the machine's execution thread twice a second.

Rules, in priority order:

1. **No fixed `char` buffers, no `sprintf` into them, ever.** This is risk R2. The existing
   comparable socket path contains exactly that pattern; a new listener must not reproduce it,
   because anything that can reach the port can reach that code.
2. **`AnsiString` for the payload** — it owns its length; concatenation cannot overflow.
3. **Escape every string value**: `"` → `\"`, `\` → `\\`, control bytes < 0x20 → `\uXXXX`, and
   **strip or escape any byte ≥ 0x80**. The state captions are ASCII today (§0) but one call site
   passes a variable (`EmgStr`, `ckernel.cpp:993`), and a stray Big5 byte would make the whole frame
   a parse error at the consumer.
4. **Never `NaN`/`Infinity`** for a double; emit `null`.
5. **Inbound bytes are discarded and counted.** Read-only port. Drop the connection if a single line
   exceeds 4 KB.
6. **Bounded send queue.** If a client cannot keep up, **drop the client** — never let it slow the UI
   thread. The queue depth and the drop count are exposed for the acceptance run.

---

## 5. v1 tag set

Names must match what the browser and the MFC monitor already bind, so `machine.state` is the state
word. Deliberately small: this is a shipping binary and the set is trivially extended later.

| tag | source | note |
|---|---|---|
| `machine.state` | `palMainStatus->Caption` | **The de-facto published machine state.** Single writer: `TfMain::ShowNowStatus(TColor, AnsiString)`, caption write at `main.cpp:21378`, declared `main.h:1259`, body `main.cpp:21279`. The control is `palMainStatus` (`main.dfm:1854-1869`, `main.h:668`) — a `TPanel` used as a 48 pt text field, nested `Panel2 > Off_lineDisplay > palMainStatus`. This **resolves** the V906 tagmap note that said the label "lives in Panel2.Off_lineDisplay, exact label unresolved": `Off_lineDisplay` is only the frame and has **no caption writer at all** (its 6 code references are BorderWidth/Color only). |
| `machine.stateCode` | `iSECSGEMMachineState` | 1-based index into `sMacStatus[30]` (`cmydef.cpp:5205-5207`). Numeric, unambiguous, immune to any caption-text question. |
| `machine.run.systemStart` | `SystemStart` | The authoritative "machine is running" master flag. |
| `machine.run.allMotorHome` | `fAllMotorHome` | Precondition, **not** a run state — "all motors finished HOME". |
| `machine.run.softStop` | `SoftStop` | One-shot STOP request latch. |
| `machine.id.type` / `.gpib` / `.tester` | `IniConfig` strings | Same names the V906 publisher already uses, so the monitor and browser need no change. |

Everything the V906 publisher sends and V899 does not must simply be **absent** — the consumer turns
absent into `null` → `---`, which is the honest rendering.

---

## 6. Acceptance criteria (from the risk assessment §7, made concrete)

1. **Flag OFF → identical to the current release**, item by item: no socket created, no listen, and
   the `Timer10` body does no extra work. Verify by reading the compiled path, not by inspection of
   the flag alone.
2. **8 h soak with the flag ON**: `dropped = 0`, no memory growth.
3. **SECS turnaround distribution, flag OFF vs flag ON** — the quantified answer to R1. Baseline
   context established by recon: the existing `ProcessSocketReceiveData` poll already adds 0–300 ms
   against a 30 s T3/T6, so the bar is "no measurable shift", and the measurement must be taken with
   the automation thread running, not on an idle handler.
4. **Hostile input**: flood, half-lines, binary garbage, abrupt disconnect, 1 MiB line with no `\n`.
   The handler must neither crash nor stall.
5. **Consumer swap**: kill and restart `wb_gateway` — the handler is unaffected. Then point the
   **unmodified** `wb_gateway` and the **unmodified** V906 MFC monitor at the V899 port and confirm
   both render the real state word. This is the whole point of matching the wire.
6. **Full regression**: SECS online/offline, Lot Start/End, alarm reporting.
7. **BCB6 build check** on `HT9045.bpr` before anything is called done.

---

## 7. V899 house rules this change must obey

- **BCB6 / pre-C++11**: no `auto`, no `nullptr`, no lambdas, no range-for, no new STL or third-party
  dependency. `AnsiString`, not `std::string`.
- **Big5 source**: minimal diff, no whole-file re-encoding, no U+FFFD anywhere in review.
- **`//AI(ht9045-v899) YYYYMMDD: <action + reason>`** comment format; new lines get a standalone
  comment on the preceding line, single-line edits get an end-of-line comment.
- **`.dfm` untouched** (§3 is designed around this).
- **Safety-critical review**: state the risk and blast radius before the edit and the regression plan
  after it.

---

## 8. Still open

- **Where the code lives.** A new `.cpp/.h` pair is architecturally cleaner than inlining ~200 lines
  into a 33 000-line `main.cpp`, but it requires adding the unit to `HT9045.bpr`, and corrupting the
  project file of a shipping build is a worse outcome than a slightly fatter `main.cpp`. Decide by
  inspecting how `.bpr` lists units before editing it either way.
- **The `Timer1` blocking loop** (`main.cpp:2995-3003`) — pre-existing, out of scope, worth a ticket.
- **The V899 test-suite/production-config coupling** noted in the V906 DEVLOG is a separate item and
  does not touch this design.
