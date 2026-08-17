/* ==========================================================================
   dom.js -- a ~40 line hyperscript helper.
   This is deliberately not a framework. Panels are plain functions that
   return DOM nodes; live values are pushed in later by bind.js via
   [data-tag] attributes, so no diffing or reactivity engine is needed.
   ========================================================================== */

const SVG_NS = "http://www.w3.org/2000/svg";
const SVG_TAGS = new Set([
  "svg", "path", "circle", "rect", "line", "polyline", "polygon", "g", "text",
]);

/**
 * h("div.panel", {title: "x"}, child, child, ...)
 * - tag may carry .class and #id shorthand: "div.panel.panel--flush#main"
 * - props: attributes; `class` merges with the shorthand; on* become listeners;
 *   `style` accepts an object; `html` sets innerHTML (trusted callers only).
 * - children: nodes, strings, numbers, arrays, or null/undefined/false (skipped)
 */
export function h(tag, props, ...children) {
  const [name, cls, id] = parseTag(tag);
  const el = SVG_TAGS.has(name)
    ? document.createElementNS(SVG_NS, name)
    : document.createElement(name);

  if (id) el.id = id;
  const classes = cls.slice();

  for (const key in props || {}) {
    const v = props[key];
    if (v === null || v === undefined || v === false) continue;
    if (key === "class" || key === "className") { classes.push(v); }
    else if (key === "style" && typeof v === "object") { Object.assign(el.style, v); }
    else if (key === "html") { el.innerHTML = v; }
    else if (key.startsWith("on") && typeof v === "function") {
      el.addEventListener(key.slice(2).toLowerCase(), v);
    }
    else { el.setAttribute(key, v === true ? "" : String(v)); }
  }
  if (classes.length) el.setAttribute("class", classes.join(" "));

  append(el, children);
  return el;
}

function parseTag(tag) {
  let name = "div", cls = [], id = "";
  const m = String(tag).match(/^([a-zA-Z][\w-]*)?((?:[.#][\w-]+)*)$/);
  if (m) {
    if (m[1]) name = m[1];
    for (const tok of (m[2] || "").match(/[.#][\w-]+/g) || []) {
      if (tok[0] === ".") cls.push(tok.slice(1));
      else id = tok.slice(1);
    }
  }
  return [name, cls, id];
}

function append(el, children) {
  for (const c of children) {
    if (c === null || c === undefined || c === false || c === "") continue;
    if (Array.isArray(c)) { append(el, c); continue; }
    el.appendChild(c instanceof Node ? c : document.createTextNode(String(c)));
  }
}

/** Replace an element's children in one shot. */
export function fill(el, ...children) {
  el.textContent = "";
  append(el, children);
  return el;
}

/** Range helper: rangeOf(1, 8) -> [1..8] */
export function rangeOf(from, to) {
  const out = [];
  for (let i = from; i <= to; i++) out.push(i);
  return out;
}
