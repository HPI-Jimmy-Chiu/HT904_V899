/* ==========================================================================
   right.js -- the production-data column.
   These panels consolidate what the legacy build spreads across several
   forms (contact counter = cContactCT, lot info = the LotInfo form, bin
   select / index speed = their own dialogs). Bringing them onto one screen
   is the point of the redesign, so each panel records its origin in a title
   tooltip rather than pretending it came from fMain.
   ========================================================================== */

import { h, rangeOf } from "../ui/dom.js";
import { panel, btn, siteGrid, kv, tabs, radioGroup, binRow } from "../ui/widgets.js";
import { CONTACT_KINDS, SPEED_ROWS } from "../model/state.js";

/* --- Test Category --------------------------------------------------------- */

function testCategoryPanel() {
  return panel({ title: "Test Category" },
    siteGrid({ tagPrefix: "cat", arms: [1, 2], muted: true }),
  );
}

/* --- Contact Counter Kinds -------------------------------------------------- */

function contactPanel() {
  return panel({ title: "Contact Counter Kinds" },
    h("div.btnrow", null,
      btn("Count Clear", { cmd: "contact.clear" }),
      btn("Yield Chart", { cmd: "contact.yieldChart" }),
    ),
    h("div.radios", { style: { margin: "10px 0 8px" } },
      radioGroup("ccScope", ["History", "Kind", "By Head Yield"], { active: "History" }),
      radioGroup("ccMode", ["Total", "Kind (%)"], { active: "Total" }),
    ),
    h("table.dtable.dtable--split", null,
      h("thead", null, h("tr", null,
        h("th"), h("th", null, "Arm 2"), h("th"), h("th", null, "Arm 1"),
      )),
      h("tbody", null, ...CONTACT_KINDS.map(([left, right]) => h("tr", null,
        h("td.k", null, left),
        h("td.v", { "data-tag": `contact.${left}`,  "data-fmt": "pct2" }),
        h("td.k", null, right),
        h("td.v", { "data-tag": `contact.${right}`, "data-fmt": "pct2" }),
      ))),
    ),
  );
}

/* --- Lot Info ---------------------------------------------------------------- */

const LOT_ROWS = [
  ["Loader Last Bundle ID:", "lot.loaderLastBundleId", null, null],
  ["Loadercar Bundle ID:",   "lot.loadercarBundleId",  null, null],
  ["Auto 1 Now Tray ID:",    "lot.auto1.nowTrayId",  "Auto 1 Tray Count:", "lot.auto1.trayCount"],
  ["Auto 2 Now Tray ID:",    "lot.auto2.nowTrayId",  "Auto 2 Tray Count:", "lot.auto2.trayCount"],
  ["Auto 3 Now Tray ID:",    "lot.auto3.nowTrayId",  "Auto 3 Tray Count:", "lot.auto3.trayCount"],
  ["Auto 1 Tray ID:",        "lot.auto1.trayId",     "Lot Tray Count:",    "lot.trayCount"],
  ["Auto 2 Tray ID:",        "lot.auto2.trayId",     "Finish Tray Count:", "lot.finishTrayCount"],
  ["Auto 3 Tray ID:",        "lot.auto3.trayId",     null, null],
  ["Loader Tray Enable:",    "lot.loaderTrayEnable", "Color Tray Enable:", "lot.colorTrayEnable"],
  ["Empty Tray Enable:",     "lot.emptyTrayEnable",  null, null],
];

function lotInfoPanel() {
  return panel({ title: "Lot Info" },
    tabs(["Lot", "Bundle"], { group: "lot", active: 0 }),
    h("div.center", { style: { padding: "8px 0 6px", fontWeight: "600" } }, "Lot Info"),
    h("div.kvgrid", null,
      ...LOT_ROWS.flatMap(([lk, lt, rk, rt]) => [
        kv(lk, lt),
        rk ? kv(rk, rt) : h("div"),
      ]),
    ),
  );
}

/* --- Sort Count ---------------------------------------------------------------- */

function sortCountPanel() {
  const line = (label, key) => [
    h("div.kv__k", null, label),
    h("div.num.right", { "data-tag": `sort.${key}.pct`,   "data-fmt": "pct2" }),
    h("div.num.right", { "data-tag": `sort.${key}.count`, "data-fmt": "int" }),
  ];
  return panel({ title: "Sort Count" },
    h("div.sorthead", null,
      h("div.kv__k", null, "Loading:"),
      h("div.num", { "data-tag": "sort.loading" }),
      h("div.kv__k", null, "Total:"),
      h("div.num", { "data-tag": "sort.total" }),
    ),
    h("div.sortgrid", null,
      ...rangeOf(1, 3).flatMap(i => [
        ...line(`Auto ${i}:`, `auto${i}`),
        ...line(`Fix ${i}:`,  `fix${i}`),
      ]),
    ),
    h("div.center", { style: { marginTop: "10px" } },
      btn("Clear Count", { cmd: "sort.clear" })),
  );
}

/* --- Bin Select ------------------------------------------------------------------ */

function binSelectPanel() {
  return panel({ title: "Bin Select" },
    tabs(["Bin Display Status", "Index", "SLT"], { group: "binView", active: 0 }),
    h("div", { style: { height: "4px" } }),
    tabs(["Test Bin", "Category Info", "UPH Information"], { group: "binKind", active: 0 }),
    h("div", { style: { height: "8px" } },),
    binRow("Auto 1 :", "bin.auto1"),
    binRow("Auto 2 :", "bin.auto2"),
    binRow("Auto 3 :", "bin.auto3"),
    binRow("Fix 1 :",  "bin.fix1"),
    binRow("Fix 2 :",  "bin.fix2"),
    binRow("Fix 3 :",  "bin.fix3"),
  );
}

/* --- Index speed / accel ------------------------------------------------------------ */

function indexPanel() {
  return panel({ title: "Index" },
    h("table.dtable", null,
      h("thead", null, h("tr", null,
        h("th"), h("th", null, "XY Speed"), h("th", null, "XY Accel"),
      )),
      h("tbody", null, ...SPEED_ROWS.map(([label, key]) => h("tr", null,
        h("td", null, label),
        h("td.c", { "data-tag": `speed.${key}.xy`,  "data-fmt": "pct0" }),
        h("td.c", { "data-tag": `speed.${key}.acc`, "data-fmt": "pct0" }),
      ))),
    ),
  );
}

/* --- assembled column ------------------------------------------------------------------ */

export function rightColumn() {
  return h("div.col.col--right", null,
    testCategoryPanel(),
    h("div.rightrow.rightrow--a", null,
      contactPanel(),
      h("div.stack", null, lotInfoPanel(), sortCountPanel()),
    ),
    h("div.rightrow.rightrow--b", null,
      binSelectPanel(),
      indexPanel(),
    ),
  );
}
