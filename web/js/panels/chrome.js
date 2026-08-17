/* ==========================================================================
   chrome.js -- the app frame: left icon rail and the top bar.
   The rail replaces the horizontal TSpeedButton toolbar that sits at the top
   of the legacy tsMain tab; `legacy` records which button each icon stands in
   for so the wiring is unambiguous later.
   ========================================================================== */

import { h } from "../ui/dom.js";
import { icon } from "../ui/icons.js";
import { select } from "../ui/widgets.js";
import { RECIPES, USER_LEVELS } from "../model/state.js";

export const RAIL_ITEMS = [
  { key: "main",   iconName: "home",   label: "Main",        legacy: "(tsMain)" },
  { key: "msg",    iconName: "chart",  label: "Message",     legacy: "sbMessage" },
  { key: "speed",  iconName: "gauge",  label: "Speed",       legacy: "sbSpeed" },
  { key: "calc",   iconName: "calc",   label: "Calculater",  legacy: "sbCalculator" },
  { key: "io",     iconName: "oval",   label: "IO",          legacy: "sbIO" },
  { key: "offset", iconName: "axes",   label: "Offset",      legacy: "sbOffset" },
  { key: "tools",  iconName: "wrench", label: "Tools",       legacy: "sbSetting" },
  { key: "config", iconName: "gear",   label: "Config",      legacy: "sbConfig" },
];

export function rail({ active = "main" } = {}) {
  return h("nav.rail", { "aria-label": "Main navigation" },
    h("div.rail__logo", null,
      h("img", { src: "assets/img/logo.jpg", alt: "HON PREC" })),

    ...RAIL_ITEMS.map(it => h(
      `button.railbtn${it.key === active ? ".railbtn--active" : ""}`,
      {
        type: "button",
        title: `${it.label}  (${it.legacy})`,
        "aria-label": it.label,
        "data-cmd": "nav.open",
        "data-value": it.key,
      },
      icon(it.iconName),
    )),

    h("div.rail__spacer"),

    h("button.railbtn.railbtn--round.railbtn--lang", {
      type: "button", title: "Language  (sbLaguage)", "data-cmd": "ui.language",
    }, "中"),

    h("button.railbtn.railbtn--round.railbtn--exit", {
      type: "button", title: "Exit  (sbCloseProgram)", "data-cmd": "app.exit",
    }, icon("exit")),
  );
}

export function topbar() {
  return h("header.topbar", null,
    h("button.iconbtn", { type: "button", title: "Screen capture", "data-cmd": "ui.capture" },
      icon("camera")),

    h("div.topbar__time", { "data-tag": "clock.text" }),

    h("div.topbar__recipe", null,
      select({
        tag: "recipe.current",
        cmd: "recipe.change",
        block: true,
        title: "Setup file  (fMain.pgMain.tsMain.cbSetupFileName)",
        options: RECIPES,
      }),
    ),

    select({
      tag: "user.level",
      cmd: "user.change",
      title: "Operator level",
      options: USER_LEVELS,
    }),
  );
}
