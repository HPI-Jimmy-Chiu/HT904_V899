# Connecting the web HMI to `HT9011UC_Cpp_V3.33.906.0`

> Status: **design, not yet implemented.** No C++ code has been written for
> this. The purpose of the document is to fix the contract now, so the
> browser side and the handler side can be built independently and meet.

---

## 1. Where this sits in the V906 migration

The V906 migration already has a UI plan: **MFC**, decided in
`HT9011UC_Cpp_V3.33.906.0/docs/W7_UI_ARCHITECTURE_PLAN.md` §2-D1, currently
being built out as Gate A / GA-4 (a data-driven layout engine that replays the
`.dfm` geometry onto real Win32 dialogs).

A web UI is **not** a competitor to that plan at this stage, and this document
does not propose cancelling it. What it proposes is narrower and safer:

> Expose the handler's state and commands as a **tag/command contract**, and
> let a browser be *one* consumer of it.

That framing matters because all three integration options below consume the
*same* contract. Choosing between them later therefore costs nothing today —
which is exactly why the contract is worth fixing first.

It is worth saying plainly, though, that the option space includes a
strategically large one (option C), because the MFC path carries a very large
mechanical burden: 133 forms, 22,586 emitted `CONTROL` statements, 3,637
dialog templates, 16 custom window classes to register and subclass, and a
px→DLU geometry problem that only 17% of controls survive intact. If the web
UI is going to be the operator's real screen anyway, that burden is
avoidable. That is a decision for the user, not for this document.

---

## 2. The seam: tags and commands

Everything crossing the boundary is one of two things.

**A tag** is a named, JSON-serialisable value the handler publishes:

```
"temp.pv"          130
"machine.state"    "HALT"
"tower.red"        1
"arm.A1"           11.0
"arm.A1.tone"      "blue"
"zone.heatgun.1"   null          // not installed
"speed.index.acc"  ""            // deliberately blank
```

**A command** is a named operator action the browser requests:

```
{ cmd: "temp.setSV",   tag: "temp.sv",   value: "135" }
{ cmd: "run.setOffLine" }
{ cmd: "sort.clear" }
```

Tag names are defined in `js/model/state.js` and mapped to real VCL controls
in `js/model/tagmap.js`. **The `.dfm` leaf control name is the join key** —
the same key W7 decision D11 already uses for the MFC path, and for the same
reason: it is unique within a form (measured: zero intra-form duplicates
across all 133 forms), so it aligns the C++ name space (`fMain->cbSetupFileName`)
and the resource name space (`IDC_MAIN_CBSETUPFILENAME`) for free.

The numeric resource id for any leaf is already generated:
`tools/dfm2rc/rc_out/main_uimap.gen.cpp` (91 dialogs / 758 controls for
`fMain`). A bridge does not need to hard-code ids; it can look them up.

---

## 3. Three ways to connect

### Option A — embedded HTTP + WebSocket server inside `HT9045.exe`

```
  browser  ──ws──►  HT9045.exe { http server thread │ state snapshot │ machine }
```

* **For** — one process, no IPC protocol to invent, the server can read
  handler state directly, and V906 is a tree we own and can modify.
* **Against** — puts a socket server inside the application being migrated;
  gets threading wrong and you stall the machine.
* **Cost** — an HTTP/WS library (see §6) plus a publisher thread.

### Option B — sidecar bridge process

```
  browser ──ws──► ht9045_web.exe ──IPC──► HT9045.exe
```

* **For** — zero risk to the handler binary; can be developed and shipped
  independently; **there is direct precedent in this product** —
  `H9046_32GPIB.exe` is exactly this shape (GPIB ⇄ `WM_COPYDATA` ⇄ handler).
* **Against** — you must invent and version an IPC contract, and every value
  crosses two boundaries instead of one. A second process to deploy, launch,
  supervise and diagnose.

### Option C — the web page *is* the UI, hosted in-process (WebView2)

```
  HT9045.exe { WebView2 control showing this page } + the same ws endpoint for remote
```

* **For** — one UI codebase for the local operator panel *and* remote access;
  removes the entire MFC form-porting workstream; modern UI capability the
  VCL forms never had.
* **Against** — the biggest change to the migration plan, a hard dependency on
  the WebView2 runtime being present on the machine PC, and it must be able to
  reach 100% of what 133 forms do before it can replace them (this home screen
  is 1 of those 133).

### Recommendation

**Start with A, keep C open, treat B as the fallback.**

A is recommended over B because the IPC contract B requires is pure overhead
here — the GPIB bridge exists because the GPIB card genuinely lives in another
process's world; the web UI does not. B becomes right only if it turns out the
handler binary must not be touched.

C is the interesting one, but it is a *strategy* decision about the migration,
not an implementation detail, and it should be taken deliberately. Nothing in
A forecloses it: A's `ws` endpoint is exactly what C would use for its remote
case, and the page is identical either way.

---

## 4. Wire protocol

JSON text frames over one WebSocket at `/ht9045`. Fully specified in
`js/transport/ws.js`; summarised here.

**handler → browser**

| frame | meaning |
|---|---|
| `{"type":"snapshot","data":{tag:value,…}}` | full state, sent on connect |
| `{"type":"patch","data":{tag:value,…}}` | deltas, as they change |
| `{"type":"ack","id":n,"ok":true\|false,"error":"…"}` | result of a command |
| `{"type":"alarm","code":"WAR0152","text":"…","at":"…"}` | alarm raised |

**browser → handler**

| frame | meaning |
|---|---|
| `{"type":"cmd","id":n,"cmd":"…","tag":"…","value":…}` | operator action |
| `{"type":"ping","id":n}` | liveness |

**Rules**

1. **The handler is authoritative.** The browser never assumes a command
   succeeded and never optimistically updates machine state — it waits for the
   resulting `patch`. This is what stops two operators on two browsers from
   disagreeing about what the machine is doing.
2. Send `patch`, not `snapshot`, for updates. A full snapshot of this screen is
   ~234 tags; at 10 Hz that is pointless traffic.
3. `null` means *unknown / not installed*. `""` means *deliberately blank*.
   They render differently and are not interchangeable.
4. Unknown tags in a patch are ignored by the browser, so the handler may
   publish more than any given page displays. Add tags freely.

---

## 5. Threading — the constraint that will bite

The handler's UI work runs on the UI thread, and that thread has hard
real-time-ish obligations elsewhere in the system (the SECS/GEM layer has a
30-second reply budget; blocking it is a known failure mode in this product).

So:

* The socket server runs on **its own thread**. It never calls into machine
  logic directly.
* State reaches it through a **snapshot the UI thread publishes**, not by the
  server reaching in and reading live objects. A double-buffered
  `map<tag,value>` behind one mutex is sufficient; the publisher swaps, the
  server reads.
* Commands go the other way through a **queue the UI thread drains** on its
  existing timer tick. The server enqueues and returns immediately; the `ack`
  is sent when the UI thread has actually processed it.
* Nothing on the socket thread may block on machine I/O. Ever.

---

## 6. Open questions — these need a decision

1. **HTTP/WS library.** The migration has been deliberately dependency-averse.
   Options, in order of my preference:
   * **civetweb** — single `.c` file, MIT, HTTP + WebSocket, builds clean on
     MSVC and MinGW. Smallest real dependency that does the job.
   * **hand-rolled Winsock** — zero dependencies; a WebSocket frame codec for
     one endpoint is roughly 200 lines. Defensible, and more code we own.
   * *not* `cpp-httplib` — header-only and pleasant, but no WebSocket support.

2. **Bind address and authentication.** Default must be **loopback only**.
   This UI can command machine motion; exposing it on a fab LAN without at
   minimum a shared token is not acceptable, and that decision should be
   explicit rather than a default. Recommend: loopback by default, LAN
   exposure behind an opt-in flag in `Gerneral.ini` plus a token.

3. **Read-only first?** A read-only dashboard (no `cmd` frames at all) is a
   genuinely zero-risk first milestone and would already be useful for remote
   monitoring. Recommend shipping that before any command path exists.

4. **Does this replace the MFC UI, or sit beside it?** — §1, option C. This is
   the strategic question and it is the user's call.

---

## 7. What the C++ side has to do (option A, read-only first)

1. Add a socket server thread with one WebSocket endpoint, bound to loopback.
2. Publish a `map<string,json>` snapshot from the UI thread's existing timer.
3. Populate the snapshot from the tags in `js/model/tagmap.js` whose `dfm`
   field is resolved — those already name the exact VCL control to read.
4. Serve `D:\HT9045\web\` as static files from the same server.
5. Diff snapshots and emit `patch` frames.

Steps 1–5 are independent of every remaining question in §6.
