# HT9045 Web HMI

A browser-based operator screen for the HT9045 handler, built to drive the
UI of **`HT9011UC_Cpp_V3.33.906.0`** over the network.

This first deliverable is the **home screen** (the legacy `fMain` "Main" tab,
redesigned), running against simulated data. No backend exists yet; the wire
contract it will speak is fixed in [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md).

---

## Run it

```
cd D:\HT9045\web
python serve.py
```

Opens <http://127.0.0.1:8045/>. Stdlib Python only — nothing to install.

| URL | Data source |
|---|---|
| `http://127.0.0.1:8045/` | built-in mock (default) |
| `http://127.0.0.1:8045/?src=ws` | live handler at `ws://<same host>/ht9045` |
| `http://127.0.0.1:8045/?src=ws&ws=ws://10.0.0.5:8080/ht9045` | live handler elsewhere |

> A static server is required: ES modules do not load over `file://`.

---

## Stack: none

Plain HTML + CSS + ES modules. **No npm, no bundler, no framework, no build
step.** Edit a file, reload the browser.

That is a deliberate choice, not laziness:

- **The handler will serve these files itself.** Whatever ships has to be the
  files on disk. A build step means a build artifact that can silently drift
  from its source on a machine nobody rebuilds for five years.
- **Nothing to install on the machine PC.** No Node, no toolchain, no
  `node_modules` to go stale or fail an audit.
- **No dependency rot.** A handler is supported for a decade. Every
  third-party package is a decade of CVEs and breaking majors. This has zero.
- **It stays readable to a C++ engineer.** No JSX, no reactive graph, no
  compile-to-something. What you read is what the browser runs.

The cost is that there is no component framework, so the code supplies a very
small one: a 40-line `h()` helper and a tag-binding layer. That is the whole
abstraction budget.

---

## Layout

```
web/
├── index.html              page shell (loads CSS, boots js/main.js)
├── serve.py                dev static server, stdlib only
├── assets/
│   ├── css/tokens.css      ALL colour/spacing/type values live here
│   ├── css/base.css        reset + the 3-column app shell
│   ├── css/components.css  every visual primitive
│   └── img/logo.jpg        real HON PREC mark (from IMG/BMP/Logo_New.jpg)
├── js/
│   ├── main.js             boot: build shell, register bindings, connect
│   ├── ui/dom.js           h() hyperscript helper
│   ├── ui/icons.js         inline SVG icons
│   ├── ui/widgets.js       panel, button, grid, table, pill, LED, ...
│   ├── ui/bind.js          ★ the ONLY seam between state and the DOM
│   ├── model/state.js      canonical tag list + initial snapshot
│   ├── model/tagmap.js     ★ web tag -> real legacy VCL control
│   ├── panels/chrome.js    icon rail + top bar
│   ├── panels/left.js      status / temperature / tester / sites / zones
│   ├── panels/right.js     category / contact / lot / sort / bin / index
│   ├── panels/statusrail.js far-right status pills
│   └── transport/          mock.js | ws.js | index.js (picker)
└── docs/ARCHITECTURE.md    how this connects to the C++ handler
```

---

## How a value gets on screen

Every live value is addressed by a **tag** — a dotted string like `temp.pv`.

1. `model/state.js` declares the tag and its initial value.
2. A panel stamps it onto an element: `data-tag="temp.pv" data-fmt="fixed1"`.
3. `ui/bind.js` pushes values in when a patch arrives.
4. `model/tagmap.js` records which real VCL control that tag corresponds to.

Panels never read or write machine state directly, and never talk to the
transport. That is why swapping `mock.js` for `ws.js` changes nothing else.

**Adding a field** is therefore: add the tag to `state.js`, stamp it in a
panel, and record its `dfm` path in `tagmap.js`.

### Bind modes (`data-bind`)

| mode | effect |
|---|---|
| `text` *(default)* | sets `textContent` through `data-fmt` |
| `value` | sets an input's value — skipped while the user is typing in it |
| `tone` | sets `data-tone`, which CSS turns into a colour |
| `lit` | sets `data-lit="0\|1"` — tower light, LEDs, site cells |
| `on` | toggles `.btn--on` — pressed state |
| `sel` | sets `aria-selected` — tabs |
| `html` | sets `innerHTML` through `data-fmt` (bin maps only) |

`data-tone-tag` is a second channel on the same element, so a table cell can
bind its **text** and its **colour** to two different tags.

### Formats (`data-fmt`)

`raw` `dash` `int` `fixed1` `fixed2` `fixed3` `pct0` `pct2` `hms`
`digitOrRing` `binmap`

`null` renders as `---` (unknown / not installed). `""` renders as blank
(deliberately empty). That distinction is load-bearing — `Number(null)` is
`0`, so a naive check prints `0.00` for every uninstalled heater zone.

---

## Debugging

The console exposes a handle:

```js
__ht9045.applyState({ "machine.state": "RUN", "tower.red": 0 })
__ht9045.boundTags()      // every tag the page actually displays
```

---

## Status

**Done** — home screen, full layout, 234 bound nodes, mock transport,
WebSocket transport written against a fixed contract, tag map for the left
column and status rail.

**Not done** — no backend; the right-hand panels (contact counter, lot info,
bin select, index speed) are laid out but their tags are not yet mapped to
real controls; navigation buttons do not open anything yet.

See `docs/ARCHITECTURE.md` for what has to happen on the C++ side.
