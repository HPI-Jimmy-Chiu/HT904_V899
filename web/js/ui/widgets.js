/* ==========================================================================
   widgets.js -- the reusable pieces every panel is assembled from.
   Each returns a detached DOM node. None of them read or write machine
   state directly; they only stamp the [data-tag] / [data-cmd] attributes
   that bind.js understands.
   ========================================================================== */

import { h, rangeOf } from "./dom.js";
import { icon } from "./icons.js";

/* --- containers --------------------------------------------------------- */

export function panel({ title, tools, cls = "", flush = false } = {}, ...body) {
  return h(`section.panel${flush ? ".panel--flush" : ""}${cls ? "." + cls.split(" ").join(".") : ""}`, null,
    (title || tools) && h("header.panel__head", null,
      title ? h("div.panel__title", null, title) : h("div"),
      tools ? h("div.panel__tools", null, ...[].concat(tools)) : null,
    ),
    ...body,
  );
}

/** A titled section with no border -- "Arm 1", "Hot Plate", ... */
export function block({ title } = {}, ...body) {
  return h("section.block", null,
    title && h("div.block__title", null, title),
    ...body,
  );
}

export function hr() { return h("div.hr"); }

/* --- controls ------------------------------------------------------------ */

export function btn(label, { cmd, value, tag, cls = "", iconName, strong } = {}) {
  return h(`button.btn${strong ? ".btn--strong" : ""}${iconName ? ".btn--icon" : ""}${cls ? "." + cls.split(" ").join(".") : ""}`, {
    type: "button",
    "data-cmd": cmd || null,
    "data-value": value ?? null,
    "data-tag": tag || null,
    "data-bind": tag ? "on" : null,
  }, iconName && icon(iconName), label);
}

export function select({ tag, cmd, options = [], block: isBlock = false, title } = {}) {
  return h(`div.select${isBlock ? ".select--block" : ""}`, null,
    h("select", {
      "data-tag": tag || null,
      "data-bind": "value",
      "data-cmd": cmd || null,
      title: title || null,
    }, ...options.map(o => {
      const value = typeof o === "string" ? o : o.value;
      const label = typeof o === "string" ? o : (o.label ?? o.value);
      return h("option", { value }, label);
    })),
  );
}

/** label | numeric input | unit */
export function field({ label, tag, cmd, unit = "" }) {
  return h("div.field", null,
    h("div.field__k", null, label),
    h("input", {
      type: "text", inputmode: "decimal",
      "data-tag": tag, "data-bind": "value", "data-cmd": cmd || null,
    }),
    h("div.field__u", null, unit),
  );
}

/* --- indicators ---------------------------------------------------------- */

/** The "1 1 o 1 1 o" strip under the Temperature and Tester cards. */
export function ledStrip(tagPrefix, count = 6) {
  return h("div.ledrow", null,
    ...rangeOf(1, count).map(i =>
      h("div.ledrow__cell", { "data-tag": `${tagPrefix}.${i}`, "data-fmt": "digitOrRing" })),
  );
}

/** Vertical 3-segment tower light. */
export function towerLight(tagPrefix = "tower") {
  return h("div.tower", null,
    h("div.tower__seg", { "data-tone": "red",   "data-tag": `${tagPrefix}.red`,   "data-bind": "lit" }),
    h("div.tower__seg", { "data-tone": "amber", "data-tag": `${tagPrefix}.amber`, "data-bind": "lit" }),
    h("div.tower__seg", { "data-tone": "green", "data-tag": `${tagPrefix}.green`, "data-bind": "lit" }),
    h("div.tower__mast"),
  );
}

/* --- grids and tables ----------------------------------------------------- */

const COLS = ["a", "b", "c", "d", "e", "f", "g", "h"];

/**
 * The 16-site-per-arm grid used by both "Test Site" and "Test Category".
 *   row A -> sites 1..8, row B -> sites 9..16
 * Tags: `${tagPrefix}.arm1.s7`
 */
export function siteGrid({ tagPrefix, arms = [1, 2], muted = false }) {
  const rows = [];
  rows.push(h("div.sitegrid__arm"), h("div"),
    ...COLS.map(c => h("div.sitegrid__colhead", null, c)));
  for (const arm of arms) {
    for (const [ri, rowKey] of ["A", "B"].entries()) {
      rows.push(
        h("div.sitegrid__arm", null, ri === 0 ? `Arm ${arm}` : ""),
        h("div.sitegrid__rowhead", null, rowKey),
        ...rangeOf(1, 8).map(i => {
          const site = ri * 8 + i;
          return h("div.sitegrid__cell", {
            "data-tag": `${tagPrefix}.arm${arm}.s${site}`,
            "data-bind": "lit",
          }, String(site));
        }),
      );
    }
  }
  return h(`div.sitegrid${muted ? ".sitegrid--muted" : ""}`, null, ...rows);
}

/**
 * The Arm 1 / Arm 2 value tables.
 *   heads: ["A1","C1","E1","G1","I1","K1","M1","O1"]
 * Each cell binds text to `${tagPrefix}.${head}` and colour to
 * `${tagPrefix}.${head}.tone`.
 */
export function valRow(tagPrefix, heads) {
  return [
    ...heads.map(k => h("div.valtable__h", null, k)),
    ...heads.map(k => h("div.valtable__c", {
      "data-tag": `${tagPrefix}.${k}`,
      "data-fmt": "fixed2",
      "data-tone-tag": `${tagPrefix}.${k}.tone`,
    })),
  ];
}

export function valTable(tagPrefix, rows) {
  return h("div.valtable", null, ...rows.flatMap(heads => valRow(tagPrefix, heads)));
}

/** Hot Plate / Shuttle / Index / Heat Gun mini table: 2 headers + 2 values. */
export function zoneBox({ title, cells }) {
  return h("div.zone", null,
    h("div.zone__title", null, title),
    h("div.zone__grid", null,
      ...cells.map(c => h("div.valtable__h", null, c.label)),
      ...cells.map(c => h("div.valtable__c", {
        "data-tag": c.tag, "data-fmt": c.fmt || "fixed2",
      })),
    ),
  );
}

/** label / value pair used by Lot Info. */
export function kv(label, tag, fmt = "raw") {
  return h("div.kv", null,
    h("div.kv__k", null, label),
    h("div.kv__v", { "data-tag": tag, "data-fmt": fmt }),
  );
}

export function tabs(items, { group, active = 0 } = {}) {
  return h("div.tabs", { role: "tablist" },
    ...items.map((label, i) => h("div.tab", {
      role: "tab",
      "aria-selected": i === active ? "true" : "false",
      "data-cmd": group ? `ui.tab.${group}` : null,
      "data-value": label,
    }, label)),
  );
}

/** Returns an ARRAY of <label> nodes so several groups can share one grid. */
export function radioGroup(name, items, { active } = {}) {
  return items.map(label => h(`label.radio${label === active ? ".radio--on" : ""}`, null,
    h("input", {
      type: "radio", name,
      checked: label === active || null,
      "data-cmd": `ui.radio.${name}`, "data-value": label,
    }),
    label,
  ));
}

/** One "Auto 1 : ....4 5 6 7...." line in the Bin Select panel. */
export function binRow(label, tag) {
  return h("div.binrow", null,
    h("div.binrow__k", null, label),
    h("div.binrow__v", { "data-tag": tag, "data-bind": "html", "data-fmt": "binmap" }),
  );
}

/** A right-hand status pill. */
export function pill(tag, { empty = false } = {}) {
  return h(`div.pill${empty ? ".pill--empty" : ""}`, {
    "data-tag": tag, "data-fmt": "raw",
    "data-tone-tag": `${tag}.tone`,
  });
}
