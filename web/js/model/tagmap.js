/* ==========================================================================
   tagmap.js -- web tag  <->  real legacy VCL control.
   ==========================================================================

   WHY THIS FILE EXISTS
   This dashboard is not a mockup. Every value it shows already exists in the
   HT9045 handler as a named VCL control on a .dfm form. Recording that
   correspondence here -- once, in data -- is what turns "a nice picture" into
   "a UI that can be wired to the machine mechanically instead of by
   archaeology". When the C++ bridge is written, it walks this table.

   THE JOIN KEY
   W7_UI_ARCHITECTURE_PLAN.md decision D11: the join key between logic and
   widget is the .dfm LEAF control name, which is unique within a form
   (measured: 0 intra-form duplicates across all 133 forms). So `dfm` below is
   the authoritative identifier; the numeric IDC id is derivable from it via
   tools/dfm2rc/rc_out/<form>_uimap.gen.cpp.

   FIELD MEANINGS
     dfm    full IR path, copied verbatim from
            tools/dfm2rc/ir_out/<form>.dfm.ir.json  (null = not yet resolved)
     cls    the VCL class at that path
     dir    "r"  handler -> browser (display only)
            "w"  browser -> handler (operator action)
            "rw" both
     note   anything a reader would otherwise have to rediscover

   HONESTY RULE
   An entry with dfm: null means NOBODY HAS FOUND IT YET. Do not fill one in
   from a plausible-looking guess -- verify against the IR JSON first. A wrong
   mapping here is worse than a missing one, because the bridge will wire it
   and the wrong control will move.
   ========================================================================== */

const M = "fMain.pgMain.tsMain";

export const TAGMAP = {
  /* ---- top bar ------------------------------------------------------- */
  "clock.text":      { dfm: null, cls: null, dir: "r",
                       note: "legacy shows date/time on StatusBar1; exact panel index unresolved" },
  "recipe.current":  { dfm: `${M}.cbSetupFileName`, cls: "TComboBox", dir: "rw",
                       note: "the selector; Panel2.edSetupFileName mirrors it as read-only text" },
  "user.level":      { dfm: `${M}.Panel2.cbUserSelect`, cls: "TComboBox", dir: "rw",
                       note: "paired with Panel2.spbUserName / btLogin / edtLogin" },

  /* ---- machine status -------------------------------------------------- */
  "machine.state":   { dfm: null, cls: null, dir: "r",
                       note: "the big HALT/RUN word; lives in Panel2.Off_lineDisplay, exact label unresolved" },
  "machine.mode":    { dfm: `${M}.Panel2.Off_lineDisplay.palMainStatus.labARTmode`, cls: "TLabel", dir: "r",
                       note: "one of 14 overlay labels on palMainStatus; the others (labQAMode, " +
                             "lblMaintMode, lbCCDStatus, lbl2DSort, ...) are not yet on the web screen" },
  "tower.red":       { dfm: `${M}.palSetting.ledRed`,    cls: "TALed", dir: "r" },
  "tower.amber":     { dfm: `${M}.palSetting.ledYellow`, cls: "TALed", dir: "r" },
  "tower.green":     { dfm: `${M}.palSetting.ledGreen`,  cls: "TALed", dir: "r",
                       note: "the three sit inside palSetting.palLedTower" },
  "run.ft":          { dfm: `${M}.Panel2.palFT`,      cls: "TPanel", dir: "rw" },
  "run.rt":          { dfm: `${M}.Panel2.palRT`,      cls: "TPanel", dir: "rw",
                       note: "has OnClick palRTClick" },
  "run.offline":     { dfm: `${M}.Panel2.palOffLine`, cls: "TPanel", dir: "rw",
                       note: "Panel2.palEQC is a fourth mode the web screen does not show yet" },

  /* ---- temperature ------------------------------------------------------- */
  "temp.pv":         { dfm: null, cls: null, dir: "r",
                       note: "big present-value readout; not a pnlTempSetting child -- unresolved" },
  "temp.mode":       { dfm: `${M}.palSetting.pnlTempSetting.lblTemperatureMode`, cls: "TLabel", dir: "r",
                       note: "wire carries the raw recipe code (0=Hot 1=Ambient 2=ATC 3=AmbientHot); " +
                             "bind.js tempMode formatter decodes it (user ruling 20260824)" },
  "temp.sv":         { dfm: `${M}.palSetting.pnlTempSetting.edWorkTemperBase`, cls: "TEdit", dir: "rw",
                       note: "edATCAmbientTemper is the separate ambient setpoint" },
  "temp.soak":       { dfm: `${M}.palSetting.pnlTempSetting.edSoakTime`, cls: "TEdit", dir: "rw",
                       note: 'its label is labSoakTime = "Soak Time(Sec)" -- unit is in the caption' },
  "temp.apply":      { dfm: `${M}.palSetting.pnlTempSetting.spbSet`, cls: "TSpeedButton", dir: "w" },

  /* ---- tester ---------------------------------------------------------------- */
  "tester.name":     { dfm: `${M}.palSetting.palTester`, cls: "TPanel", dir: "r",
                       note: "palTester.imgTester is the icon; palTesterMode holds On-Line/Off-Line" },

  /* ---- start / run mode --------------------------------------------------------- */
  "startmode.value": { dfm: `${M}.palSetting.cbRunStartMode`, cls: "TComboBox", dir: "rw",
                       note: "sits on palSetting.palStartMode; cbbRunModeSel is a DIFFERENT combo " +
                             '("Continuous Start") -- do not confuse them' },
  "runmode.value":   { dfm: `${M}.palSetting.palRunMode_1`, cls: "TPanel", dir: "r",
                       note: 'design-time caption "Real"; container is palRunMode' },
  "runmode.normal":  { dfm: `${M}.palSetting.palNormal`, cls: "TPanel", dir: "rw" },
  "runmode.prime":   { dfm: `${M}.palSetting.palPrime`,  cls: "TPanel", dir: "rw" },
  "light.off":       { dfm: `${M}.palSetting.spbLight`, cls: "TSpeedButton", dir: "rw",
                       note: 'caption toggles "Light OFF"/"Light ON"; OnClick spbLightClick' },
  "fan.off":         { dfm: `${M}.palSetting.spbFan`,   cls: "TSpeedButton", dir: "rw",
                       note: "palSetting.spbChamberFan is a separate chamber fan, not shown yet" },

  /* ---- site map ------------------------------------------------------------------- */
  // One TTMyTray holds the whole 2-arm x 16-site map, so the 32 web tags
  // site.arm{1,2}.s{1..16} address cells of a single control rather than 32
  // controls. XItem=8 YItem=4 -> col = (site-1)%8, row = (arm-1)*2 + (site>8).
  "site.*":          { dfm: `${M}.Panel2.SitePanel.mtDutOnOff`, cls: "TTMyTray", dir: "r",
                       note: "XItem=8 YItem=4. Headers are sibling trays: mtDutCol (8x1, a..h) " +
                             "and mtDutRow (1x4, arm/row). Cell addressing, not per-cell controls" },

  /* ---- right status rail ------------------------------------------------------------- */
  "status.powerSaving": { dfm: `${M}.pnlPowerSaving`,          cls: "TPanel", dir: "r",
                          note: 'design-time caption "PowerSaving"; runtime text adds the countdown' },
  "status.cleanCount":  { dfm: `${M}.pnlCleanCount`,           cls: "TPanel", dir: "r",
                          note: 'design-time caption is "AOA" -- retitled at runtime. Not a typo here' },
  "status.ep":          { dfm: `${M}.lbEPenconder`,            cls: "TPanel", dir: "r" },
  "status.safeDoor":    { dfm: `${M}.lbCheckSafeDoorDisable`,  cls: "TPanel", dir: "r" },
  "status.rtc":         { dfm: `${M}.labRTCCD`,                cls: "TPanel", dir: "r" },
  "status.rms":         { dfm: null, cls: null, dir: "r", note: "no design-time caption match; unresolved" },
  "status.ftp":         { dfm: `${M}.lbFTPOnoffStatus`,        cls: "TPanel", dir: "r" },
  "status.triTemp":     { dfm: `${M}.lbl_TriTempState`,        cls: "TPanel", dir: "r" },
  "status.indexTime":   { dfm: null, cls: null, dir: "r", note: "no design-time caption match; unresolved" },
  "status.uph":         { dfm: null, cls: null, dir: "r", note: "no design-time caption match; unresolved" },
  "status.saveSummary": { dfm: `${M}.pnlSaveSummary`,          cls: "TPanel", dir: "r" },

  /* ---- navigation (rail) ---------------------------------------------------------------- */
  "nav.msg":    { dfm: `${M}.sbMessage`,    cls: "TSpeedButton", dir: "w" },
  "nav.speed":  { dfm: `${M}.sbSpeed`,      cls: "TSpeedButton", dir: "w" },
  "nav.calc":   { dfm: `${M}.sbCalculator`, cls: "TSpeedButton", dir: "w" },
  "nav.io":     { dfm: `${M}.sbIO`,         cls: "TSpeedButton", dir: "w" },
  "nav.offset": { dfm: `${M}.sbOffset`,     cls: "TSpeedButton", dir: "w" },
  "nav.tools":  { dfm: `${M}.sbSetting`,    cls: "TSpeedButton", dir: "w" },
  "nav.config": { dfm: `${M}.sbConfig`,     cls: "TSpeedButton", dir: "w" },
  "ui.language":{ dfm: `${M}.sbLaguage`,    cls: "TSpeedButton", dir: "w",
                  note: "spelling is the legacy one (sbLaguage), not a typo here" },
  "app.exit":   { dfm: `${M}.sbCloseProgram`, cls: "TSpeedButton", dir: "w" },
};

/**
 * Tags the page displays that are NOT yet mapped. These come from forms other
 * than fMain (contact counter = cContactCT, lot info, bin select, index
 * speed), so resolving them means reading those forms' IR, not fMain's.
 * Listed explicitly so "unmapped" is a visible number, not a silent gap.
 */
export const UNMAPPED_GROUPS = [
  { prefix: "cat.",     owner: "fMain (Test Category grid) -- control unresolved" },
  { prefix: "arm.",     owner: "heater zone channels A1..P2 -- source form unresolved" },
  { prefix: "zone.",    owner: "Hot Plate / Shuttle / Index / Heat Gun temperatures" },
  { prefix: "contact.", owner: "cContactCT.dfm" },
  { prefix: "lot.",     owner: "the LotInfo form" },
  { prefix: "sort.",    owner: "sort counters -- source form unresolved" },
  { prefix: "bin.",     owner: "bin select -- source form unresolved" },
  { prefix: "speed.",   owner: "the Speed form reached by sbSpeed" },
];

/** Coverage, for the console and for docs/TAGMAP.md. */
export function coverage() {
  const all = Object.entries(TAGMAP);
  const resolved = all.filter(([, v]) => v.dfm);
  return { total: all.length, resolved: resolved.length, unresolved: all.length - resolved.length };
}
