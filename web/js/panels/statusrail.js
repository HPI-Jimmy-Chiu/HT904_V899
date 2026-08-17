/* ==========================================================================
   statusrail.js -- the far-right column of status pills.
   ==========================================================================
   Legacy origin: the vertical stack of TPanel rows at left=746 in
   fMain.pgMain.tsMain. Each row is a whole caption string (that is how the
   legacy control works too -- the code assigns Panel->Caption wholesale),
   so a pill binds one tag to its full text plus an optional tone.

   `legacy` names the tsMain control each pill stands in for. Rows whose
   legacy control is still unconfirmed carry legacy: null and are listed in
   docs/TAGMAP.md as open items rather than guessed at.
   ========================================================================== */

import { h } from "../ui/dom.js";
import { pill } from "../ui/widgets.js";
import { STATUS_SPARE_SLOTS } from "../model/state.js";

export const STATUS_ROWS = [
  { tag: "status.powerSaving", legacy: "pnlPowerSaving" },
  { tag: "status.cleanCount",  legacy: "pnlCleanCount" },
  { tag: "status.ep",          legacy: "lbEPenconder" },
  { tag: "status.safeDoor",    legacy: "lbCheckSafeDoorDisable" },
  { tag: "status.rtc",         legacy: "labRTCCD" },
  { tag: "status.rms",         legacy: null },
  { tag: "status.ftp",         legacy: "lbFTPOnoffStatus" },
  { tag: "status.triTemp",     legacy: "lbl_TriTempState" },
  { tag: "status.indexTime",   legacy: null },
  { tag: "status.uph",         legacy: null },
  { tag: "status.saveSummary", legacy: "pnlSaveSummary" },
];

export function statusRail() {
  const spare = [];
  for (let i = 1; i <= STATUS_SPARE_SLOTS; i++) {
    spare.push(pill(`status.slot${i}`, { empty: true }));
  }
  return h("aside.statusrail", { "aria-label": "Machine status" },
    ...STATUS_ROWS.map(r => {
      const node = pill(r.tag);
      if (r.legacy) node.title = `fMain.pgMain.tsMain.${r.legacy}`;
      return node;
    }),
    ...spare,
  );
}
