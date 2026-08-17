/* ==========================================================================
   bind.js -- the one and only seam between machine state and the DOM.
   ==========================================================================

   Every live value on screen is addressed by a TAG (a dotted string such as
   "temp.pv" or "site.arm1.a.3"). Markup declares its interest like this:

       <span data-tag="temp.pv" data-fmt="fixed1"></span>
       <div  data-tag="tower.red" data-bind="lit"></div>
       <input data-tag="temp.sv" data-bind="value" data-cmd="temp.setSV">

   Nothing else in the app touches the DOM to show a value, and nothing else
   reads the DOM to send one. That means the day the real backend arrives, the
   only file that has to know about it is transport/*.js -- every panel keeps
   working untouched.

   Tag -> real VCL control mapping lives in js/model/tagmap.js.
   ========================================================================== */

/** tag -> array of {el, mode, fmt} */
const registry = new Map();
/** listeners for operator-initiated commands */
const commandHandlers = [];
/** last known value per tag, so late-registered nodes can catch up */
const lastValue = new Map();

const EM_DASH = "---";

/**
 * Numeric formatter factory.
 *   null / undefined -> "---"  (unknown, or hardware not installed)
 *   ""               -> ""     (deliberately blank: a coloured cell with no value)
 * The null-vs-"" distinction matters: `Number(null)` is 0, so a naive
 * isFinite check would print "0.00" for every uninstalled zone.
 */
function num(decimals, suffix = "") {
  return v => {
    if (v === "") return "";
    if (v === null || v === undefined) return EM_DASH;
    const n = Number(v);
    return Number.isFinite(n) ? n.toFixed(decimals) + suffix : EM_DASH;
  };
}

const escapeHtml = s => String(s).replace(/[&<>]/g, c => ({ "&": "&amp;", "<": "&lt;", ">": "&gt;" }[c]));

const formatters = {
  raw:     v => v == null ? "" : String(v),
  dash:    v => (v === null || v === undefined || v === "") ? EM_DASH : String(v),
  fixed1:  num(1),
  fixed2:  num(2),
  fixed3:  num(3),
  pct0:    num(0, "%"),
  pct2:    num(2, "%"),
  int: v => {
    if (v === "") return "";
    if (v === null || v === undefined) return EM_DASH;
    const n = Number(v);
    return Number.isFinite(n) ? String(Math.round(n)) : EM_DASH;
  },
  /** the "1 1 o 1 1 o" indicator strip: a value, or a hollow ring when unset */
  digitOrRing: v => (v === null || v === undefined || v === "") ? "○" : String(v),
  /** bin map string: dots stay faint, everything else is emphasised */
  binmap: v => v == null ? "" :
    escapeHtml(v).replace(/[^.]+/g, m => `<b>${m}</b>`),
  /** seconds -> HH:MM:SS */
  hms: v => {
    if (v === null || v === undefined || !Number.isFinite(Number(v))) return EM_DASH;
    const s = Math.max(0, Math.floor(Number(v)));
    const p = n => String(n).padStart(2, "0");
    return `${p(Math.floor(s / 3600))}:${p(Math.floor(s / 60) % 60)}:${p(s % 60)}`;
  },
};

/**
 * Scan a subtree for [data-tag] nodes and register them.
 * Safe to call repeatedly; panels call it once after mounting.
 */
export function register(root) {
  const nodes = root.querySelectorAll("[data-tag]");
  for (const el of nodes) {
    const tag = el.getAttribute("data-tag");
    const entry = {
      el,
      mode: el.getAttribute("data-bind") || defaultModeFor(el),
      fmt: el.getAttribute("data-fmt") || "raw",
    };
    if (!registry.has(tag)) registry.set(tag, []);
    registry.get(tag).push(entry);
    if (lastValue.has(tag)) applyTo(entry, lastValue.get(tag));
  }
  // A cell often needs two independent bindings: its text AND its colour.
  // data-tone-tag is the second channel, always in "tone" mode.
  for (const el of root.querySelectorAll("[data-tone-tag]")) {
    const tag = el.getAttribute("data-tone-tag");
    const entry = { el, mode: "tone", fmt: "raw" };
    if (!registry.has(tag)) registry.set(tag, []);
    registry.get(tag).push(entry);
    if (lastValue.has(tag)) applyTo(entry, lastValue.get(tag));
  }
  wireCommands(root);
  return nodes.length;
}

function defaultModeFor(el) {
  const t = el.tagName;
  if (t === "INPUT" || t === "SELECT" || t === "TEXTAREA") return "value";
  return "text";
}

/**
 * Push a state patch: a flat object of {tag: value}.
 * Unknown tags are remembered (so panels mounted later still get them) but
 * are otherwise a no-op -- a backend may legitimately publish more than the
 * current page displays.
 */
export function applyState(patch) {
  for (const tag in patch) {
    const v = patch[tag];
    lastValue.set(tag, v);
    const entries = registry.get(tag);
    if (!entries) continue;
    for (const e of entries) applyTo(e, v);
  }
}

function applyTo(entry, v) {
  const { el, mode, fmt } = entry;
  switch (mode) {
    case "text":
      el.textContent = (formatters[fmt] || formatters.raw)(v);
      break;
    case "value":
      // Never yank the field out from under someone who is typing in it.
      if (document.activeElement !== el) el.value = v == null ? "" : String(v);
      break;
    case "tone":                       // data-tone drives a CSS colour
      if (v) el.setAttribute("data-tone", String(v));
      else el.removeAttribute("data-tone");
      break;
    case "lit":                        // tower-light / LED on-off
      el.setAttribute("data-lit", v ? "1" : "0");
      break;
    case "on":                         // toggle-button pressed state
      el.classList.toggle("btn--on", !!v);
      break;
    case "sel":                        // tab / radio selected state
      el.setAttribute("aria-selected", v ? "true" : "false");
      break;
    case "show":
      el.hidden = !v;
      break;
    case "class": {                    // {add:"x", remove:"y"} or "x"
      if (typeof v === "string") el.className = v;
      break;
    }
    case "html":
      el.innerHTML = (formatters[fmt] || formatters.raw)(v);
      break;
    default:
      el.textContent = (formatters[fmt] || formatters.raw)(v);
  }
}

/** Read the current value the UI holds for a tag (post-format value not included). */
export function getValue(tag) {
  return lastValue.get(tag);
}

/* --- operator commands -------------------------------------------------- */

/**
 * Any element carrying data-cmd fires a command on click (or on change, for
 * inputs and selects). The payload is:
 *   { cmd, tag, value, source }
 */
function wireCommands(root) {
  for (const el of root.querySelectorAll("[data-cmd]")) {
    if (el.__ht9045Wired) continue;
    el.__ht9045Wired = true;
    const cmd = el.getAttribute("data-cmd");
    const tag = el.getAttribute("data-tag") || "";
    const isInput = el.tagName === "INPUT" || el.tagName === "SELECT";
    el.addEventListener(isInput ? "change" : "click", () => {
      emitCommand({
        cmd,
        tag,
        value: isInput ? el.value : (el.getAttribute("data-value") ?? null),
        source: el,
      });
    });
  }
}

export function onCommand(fn) { commandHandlers.push(fn); }

export function emitCommand(payload) {
  for (const fn of commandHandlers) {
    try { fn(payload); }
    catch (err) { console.error("[bind] command handler threw", payload, err); }
  }
}

/** Diagnostics: which tags does the page actually display? */
export function boundTags() { return [...registry.keys()].sort(); }
