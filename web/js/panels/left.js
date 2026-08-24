/* ==========================================================================
   left.js -- the operator column.
   Legacy origin: fMain.pgMain.tsMain.Panel2 (status + site map) and
   fMain.pgMain.tsMain.palSetting (temperature / tester / start / run mode).
   ========================================================================== */

import { h } from "../ui/dom.js";
import { icon } from "../ui/icons.js";
import {
  panel, block, btn, select, field, ledStrip, towerLight,
  siteGrid, valTable, zoneBox, hr,
} from "../ui/widgets.js";
import { ARM1_ROWS, ARM2_ROWS, START_MODES } from "../model/state.js";

/* --- machine state: tower light + big state word + FT/RT/OFF Line ------- */

function machineCard() {
  return h("div.modecol", null,
    panel({},
      h("div.machine", null,
        towerLight(),
        h("div.halt", null,
          h("div.halt__state", { "data-tag": "machine.state" }),
          h("div.halt__sub",   { "data-tag": "machine.mode" }),
        ),
      ),
    ),
    h("div.btnrow", null,
      btn("FT",       { cmd: "run.setFT",      tag: "run.ft" }),
      btn("RT",       { cmd: "run.setRT",      tag: "run.rt" }),
      btn("OFF Line", { cmd: "run.setOffLine", tag: "run.offline" }),
    ),
  );
}

/* --- temperature + start mode -------------------------------------------- */

function temperatureCard() {
  return h("div.modecol", null,
    panel({ title: "Temperature" },
      h("div.readout", null,
        h("div.readout__v", { "data-tag": "temp.pv", "data-fmt": "int" }),
        h("div.readout__u", null, "Deg"),
      ),
      h("div.readout__mode", { "data-tag": "temp.mode", "data-fmt": "tempMode" }),
      hr(),
      field({ label: "Temperature", tag: "temp.sv",   cmd: "temp.setSV",   unit: "Deg" }),
      field({ label: "Soak Time",   tag: "temp.soak", cmd: "temp.setSoak", unit: "Sec" }),
      btn("Set", { cmd: "temp.apply", cls: "btn--block" }),
      ledStrip("temp.led"),
    ),
    panel({ title: "Start Mode", cls: "panel--center" },
      h("div.glyph.glyph--danger", null, icon("refresh")),
      h("div", { style: { marginTop: "8px" } },
        select({ tag: "startmode.value", cmd: "startmode.change", block: true, options: START_MODES }),
      ),
    ),
    h("div.btnrow", null,
      btn("Normal", { cmd: "runmode.setNormal", tag: "runmode.normal", strong: true }),
      btn("Prime",  { cmd: "runmode.setPrime",  tag: "runmode.prime",  strong: true }),
    ),
  );
}

/* --- tester + run mode ----------------------------------------------------- */

function testerCard() {
  return h("div.modecol", null,
    panel({ title: "Tester", cls: "panel--center" },
      h("div.glyph", null, icon("chip")),
      h("div.glyph__cap", { "data-tag": "tester.name" }),
      ledStrip("tester.led"),
    ),
    panel({ title: "Run Mode", cls: "panel--center" },
      h("div.glyph.glyph--muted", null, icon("cpu")),
      h("div.glyph__cap", { "data-tag": "runmode.value" }),
    ),
    h("div.btnrow", null,
      btn("Light Off", { cmd: "light.toggle", tag: "light.off", iconName: "bulb" }),
      btn("Fan Off",   { cmd: "fan.toggle",   tag: "fan.off",   iconName: "fan" }),
    ),
  );
}

/* --- assembled column ------------------------------------------------------- */

export function leftColumn(topbarNode) {
  return h("div.col.col--left", null,
    topbarNode,

    h("div.moderow", null, machineCard(), temperatureCard(), testerCard()),

    panel({ title: "Test Site" },
      siteGrid({ tagPrefix: "site", arms: [1, 2] }),
    ),

    block({ title: "Arm 1" }, valTable("arm", ARM1_ROWS)),
    block({ title: "Arm 2" }, valTable("arm", ARM2_ROWS)),

    h("div.zonestrip", null,
      zoneBox({
        title: "Hot Plate",
        cells: [
          { label: "Plate 1", tag: "zone.hotplate.1" },
          { label: "Plate 2", tag: "zone.hotplate.2" },
        ],
      }),
      zoneBox({
        title: "Shuttle",
        cells: [
          { label: "SH 1", tag: "zone.shuttle.1" },
          { label: "SH 2", tag: "zone.shuttle.2" },
        ],
      }),
      zoneBox({
        title: "Index",
        cells: [
          { label: "Chamber", tag: "zone.index.1" },
          { label: "---",     tag: "zone.index.2" },
        ],
      }),
      zoneBox({
        title: "Heat Gun",
        cells: [
          { label: "Gun1", tag: "zone.heatgun.1" },
          { label: "Gun1", tag: "zone.heatgun.2" },
        ],
      }),
    ),
  );
}
