/* ==========================================================================
   state.js -- the canonical shape of everything the home screen can show.
   ==========================================================================

   This is a FLAT tag -> value map, deliberately. It is the exact payload
   shape the C++ side will publish (see docs/ARCHITECTURE.md), so there is no
   translation layer between "what the backend sends" and "what the page
   binds". Values here are the initial snapshot; they match the approved
   design mock so the page looks right before any backend exists.

   Conventions
     null  -> unknown / not installed  -> renders as "---"
     ""    -> deliberately blank       -> renders as nothing
   ========================================================================== */

/** Column headers of the two arm value tables (they are heater zone names). */
export const ARM1_ROWS = [
  ["A1", "C1", "E1", "G1", "I1", "K1", "M1", "O1"],
  ["B1", "D1", "F1", "H1", "J1", "L1", "N1", "P1"],
];
export const ARM2_ROWS = [
  ["A2", "C2", "E2", "G2", "I2", "K2", "M2", "O2"],
  ["B2", "D2", "F2", "H2", "J2", "L2", "N2", "P2"],
];

/** Contact counter kind codes, left column (Arm 2) and right column (Arm 1). */
export const CONTACT_KINDS = [
  ["Aa", "Ca"], ["Ab", "Cb"], ["Ac", "Cc"], ["Ad", "Cd"],
  ["Af", "Cf"], ["Ag", "Cg"], ["Ah", "Ch"], ["Ba", "Da"], ["Bb", "Db"],
];

/** Rows of the Index speed/accel table: [label, hasSpeed, hasAccel]. */
export const SPEED_ROWS = [
  ["Index Arm",  "index",   true,  false],
  ["Input Arm",  "input",   true,  true],
  ["Output Arm", "output",  true,  true],
  ["Tray Arm",   "tray",    true,  false],
  ["Shuttle 1",  "shuttle1", true, true],
  ["Shuttle 2",  "shuttle2", true, true],
];

/** How many placeholder rows the status rail keeps below the live ones. */
export const STATUS_SPARE_SLOTS = 19;

/**
 * Dropdown contents. The real setup-file list is enumerated by the handler
 * from its setup directory, so the backend will replace this wholesale; the
 * single entry here is the one shown in the approved design.
 */
export const RECIPES = ["QTI_PAKALA_14X15.95_X10_105C"];
export const USER_LEVELS = ["Operator", "Technician", "Engineer", "Supervisor"];

/** Start Mode dropdown options (legacy: the Start Mode combo on palSetting). */
export const START_MODES = ["ContinueReset_ART"];

/* -------------------------------------------------------------------------
   Initial snapshot
   ------------------------------------------------------------------------- */

export function initialState() {
  const s = {
    /* --- chrome ------------------------------------------------------- */
    "clock.text":      "2025 / 05 / 12   10:12",
    "recipe.current":  "QTI_PAKALA_14X15.95_X10_105C",
    "user.level":      "Supervisor",

    /* --- machine status ----------------------------------------------- */
    "machine.state":   "HALT",
    "machine.mode":    "Auto Retest Mode",
    "tower.red":       1,
    "tower.amber":     1,
    "tower.green":     1,
    "run.ft":          false,
    "run.rt":          false,
    "run.offline":     false,

    /* --- temperature ---------------------------------------------------- */
    "temp.pv":         130,
    /* raw recipe mode code, as the real wire sends it (0=Hot; bind.js
       tempMode formatter renders "Hot Mode" -- same visual as before) */
    "temp.mode":       0,
    "temp.sv":         130,
    "temp.soak":       50,

    /* --- tester ---------------------------------------------------------- */
    "tester.name":     "Advan Type 1",

    /* --- start / run mode -------------------------------------------------- */
    "startmode.value": "ContinueReset_ART",
    "runmode.value":   "Normal",
    "runmode.normal":  false,
    "runmode.prime":   false,
    "light.off":       false,
    "fan.off":         false,

    /* --- lot info ---------------------------------------------------------- */
    "lot.loaderLastBundleId": "",
    "lot.loadercarBundleId":  "",
    "lot.auto1.nowTrayId":    "",
    "lot.auto2.nowTrayId":    "",
    "lot.auto3.nowTrayId":    "",
    "lot.auto1.trayId":       "",
    "lot.auto2.trayId":       "",
    "lot.auto3.trayId":       "",
    "lot.auto1.trayCount":    0,
    "lot.auto2.trayCount":    0,
    "lot.auto3.trayCount":    0,
    "lot.trayCount":          "",
    "lot.finishTrayCount":    "",
    "lot.loaderTrayEnable":   "",
    "lot.colorTrayEnable":    "",
    "lot.emptyTrayEnable":    "",

    /* --- sort count ---------------------------------------------------------- */
    "sort.loading": "",
    "sort.total":   "",

    /* --- bin select ------------------------------------------------------------ */
    "bin.auto1": "1.................................................",
    "bin.auto2": ".......4 5 6 7...............................",
    "bin.auto3": ".....3................................",
    "bin.fix1":  "...2..............",
    "bin.fix2":  ".............8...........................",
    "bin.fix3":  "..................9 E....",

    /* --- right status rail -------------------------------------------------------- */
    "status.powerSaving":  "Power Saving: 00:59:11",
    "status.cleanCount":   "Cleaned Count 61 / 300",
    "status.ep":           "EP: 175 / -213",
    "status.safeDoor":     "Safe Door Disable",
    "status.rtc":          "RTC Off Line",
    "status.rms":          "RMS Disabled",
    "status.ftp":          "FTP On / Off",
    "status.triTemp":      "Tri Temp State",
    "status.indexTime":    "Index Time: 1.052",
    "status.uph":          "UPH: 155",
    "status.saveSummary":  "Save Summary",
  };

  /* --- indicator strips: "1 1 o 1 1 o" ------------------------------------ */
  const strip = [1, 1, null, 1, 1, null];
  strip.forEach((v, i) => {
    s[`temp.led.${i + 1}`] = v;
    s[`tester.led.${i + 1}`] = v;
  });

  /* --- site grids: all sites idle in the mock ------------------------------ */
  for (const arm of [1, 2]) {
    for (let i = 1; i <= 16; i++) {
      s[`site.arm${arm}.s${i}`] = 0;
      s[`cat.arm${arm}.s${i}`] = 0;
    }
  }

  /* --- arm heater zone values ---------------------------------------------- */
  const zones = {
    // Arm 1: two live pairs, two colour-only cells, the rest not installed
    "arm.A1": [11.0, "blue"], "arm.C1": [12.0, "yellow"],
    "arm.E1": ["", "green"],  "arm.G1": ["", "red"],
    "arm.B1": [15.0, null],   "arm.D1": [16.0, null],
    // Arm 2: two mirrored pairs
    "arm.A2": [19.0, null], "arm.C2": [20.0, null],
    "arm.I2": [19.0, null], "arm.K2": [20.0, null],
    "arm.B2": [23.0, null], "arm.D2": [24.0, null],
    "arm.J2": [23.0, null], "arm.L2": [24.0, null],
  };
  for (const row of [...ARM1_ROWS, ...ARM2_ROWS]) {
    for (const k of row) {
      const hit = zones[`arm.${k}`];
      s[`arm.${k}`] = hit ? hit[0] : null;
      s[`arm.${k}.tone`] = hit ? hit[1] : null;
    }
  }

  /* --- ancillary temperature zones ------------------------------------------ */
  s["zone.hotplate.1"] = 0.0;  s["zone.hotplate.2"] = 3.0;
  s["zone.shuttle.1"]  = 2.0;  s["zone.shuttle.2"]  = 4.0;
  s["zone.index.1"]    = 9.0;  s["zone.index.2"]    = null;
  s["zone.heatgun.1"]  = null; s["zone.heatgun.2"]  = null;

  /* --- contact counters ------------------------------------------------------- */
  for (const [left, right] of CONTACT_KINDS) {
    s[`contact.${left}`] = left === "Aa" ? 99.59 : 0;
    s[`contact.${right}`] = 0;
  }

  /* --- sort counters ------------------------------------------------------------ */
  for (const g of ["auto", "fix"]) {
    for (let i = 1; i <= 3; i++) {
      s[`sort.${g}${i}.pct`] = 0;
      s[`sort.${g}${i}.count`] = 0;
    }
  }

  /* --- axis speed / accel --------------------------------------------------------- */
  // Axes without a configured accel column show blank, not "---".
  for (const [, key, hasSpeed, hasAccel] of SPEED_ROWS) {
    s[`speed.${key}.xy`]  = hasSpeed ? 90 : "";
    s[`speed.${key}.acc`] = hasAccel ? 90 : "";
  }

  /* --- spare status slots ----------------------------------------------------------- */
  for (let i = 1; i <= STATUS_SPARE_SLOTS; i++) s[`status.slot${i}`] = "----";

  return s;
}
