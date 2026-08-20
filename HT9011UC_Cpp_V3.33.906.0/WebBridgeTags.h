// =============================================================================
//  WebBridgeTags.h -- the ONE place where the machine and the browser meet.
//
//  AI(W906-WebBridge) 20260806.  NOT in golden.
//
//  WHY IT LIVES HERE AND NOT IN WebBridge/
//  WebBridge/ is deliberately free of vclcompat and of every machine header, so
//  the socket layer can be reasoned about on its own (see WebBridge/Sync.h and
//  the ht9045_webbridge target's comment in CMakeLists.txt). This file is the
//  deliberate exception: it includes BOTH worlds, reads machine globals, and
//  stages them into a webbridge::TagSnapshot. Keeping that mixing confined to
//  one translation unit is the entire point -- if this were inside WebBridge/,
//  that layer's independence would be gone.
//
//  THE RULE THIS FILE EXISTS TO ENFORCE
//  A tag is published with a REAL value only when its source is actually
//  loaded. Everything else is published as NULL, never as 0.
//
//  That is not fussiness. The browser renders null as "---" and 0 as "0.00"
//  (web/README.md, "Formats"), so publishing 0 for a heater zone nobody has
//  read is a screen that states a measurement which was never taken. On a
//  machine that runs at 130 C, "0.00" and "---" mean very different things to
//  the operator standing in front of it.
//
//  PRECISION ON THAT, measured 20260813: "null renders as ---" holds only for the
//  formatters that say so -- dash/int/fixed*/pct*/hms (web/js/ui/bind.js:49-72).
//  The DEFAULT format is "raw" (bind.js:86), and raw renders null as the EMPTY
//  STRING (bind.js:48). Both are honest -- neither invents a number -- but a tag
//  bound without an explicit data-fmt goes BLANK rather than dashed, so do not
//  expect "---" as proof a tag arrived. machine.state is exactly such a tag.
//
//  WHAT IS ACTUALLY LOADED TODAY -- measured 20260806, not assumed
//  (tests/test_wb_datalayer.cpp and the survey behind it):
//
//    LIVE    CUSTOMER_CODE                      957
//            IniConfig.sMachineType             "HT-9046LS"
//            IniConfig.sGPIBMachineID           "GLY320"
//            IniConfig.RMSTesterID              "HT9046"
//            LastSet.*                          from system\lastdata.dat
//                                               (12,358 non-zero bytes)
//            HSys.*                             420 keys from Gerneral.ini
//
//    DEAD    Temperature.*                      HAS a translated loader now
//                                               (ReadTempFile, uTemp_Set.cpp
//                                               :2172-3143) but it is UNSAFE
//                                               to call from host boot -- see
//                                               AI(W906-FW-TEMP1) 20260820
//                                               below. Not "nothing writes
//                                               it" any more; "no safe writer
//                                               reaches it from a boot chain".
//            IniConfig numeric/bool flags       loaded by cConfiguration.cpp,
//                                               which is untranslated (7,808
//                                               golden lines).  Only the few
//                                               STRING fields ReadLastSetIni
//                                               sets are live.
//            CosFunction (140 flags)            0 of 488 bytes set --
//                                               CustomerFunctionSelect runs but
//                                               every branch tests an IniConfig
//                                               flag that is not loaded yet
//            UN150Read (all temperature PVs)    genuinely dead, re-measured
//                                               20260820 (AI(W906-FW-TEMP1)):
//                                               its one writer thread never
//                                               starts offline, and its other
//                                               two write sites are compiled
//                                               out. See the block below.
//
//  AI(W906-FW-TEMP1) 20260820: temp.* / zone.* recon (12 tags: temp.pv/sv/
//  soak/mode, zone.{hotplate,shuttle,index,heatgun}.{1,2}). Both halves of
//  "DEAD Temperature.*" above turned out to need a MORE careful answer than
//  the previous wave's blanket claim -- measured this wave, not assumed:
//
//   (a) temp.sv/soak/mode  (Temperature.fWorkTemperBase/fSoakTime/
//       iMachineTempMode, cprod.h:1377/1378/1392) -- FW3-TempSet (commit
//       c60e9f4) landed uTemp_Set.cpp's ReadTempFile as a FAITHFUL, ACTIVE
//       translation (golden :1976-3143 -> port uTemp_Set.cpp:2172-3143), so
//       these fields ARE assignable today. But ReadTempFile is not a safe
//       thing to call from a host boot chain:
//         * uTemp_Set.cpp:2191 `MyForceDirectories(szDir)` unconditionally
//           creates a directory on disk, before even the FileExists check
//           three lines later -- not gated on any missing-key condition, it
//           just always runs.
//         * dozens of fields (fSoakTime :2331, bUseAbitCHK :2275, fAbitTemp
//           :2278/:2280, ...) are read via `CheckAndReadIniData`
//           (common.cpp:432-464), the SAME missing-key-seeds-a-WriteXxx
//           pattern already on record for Gerneral.ini and for BinSelect's
//           Binasgn*.Data (docs/RECON_binstar_datasource.md section 2g) --
//           this repo's "counter.clear precedent" for real file writes.
//         * The target file is `DataPath+recipe+"Temperature.Data"`, a THIRD
//           hardcoded shared production path (DataPath="D:\\HT9045\\IniData\\
//           Data\\", common.cpp:104) that --dry does not redirect for this
//           tool the way it redirects asGeneralPath.
//       Per this wave's explicit brief, any chain with counter.clear-style
//       write-on-missing-key behaviour does NOT get wired into host boot,
//       full stop -- no scratch-redirect workaround this time (contrast
//       bin.*, which got one). Verified NO OTHER reachable path sets these
//       three fields without an external trigger: the only other writers are
//       Automation/auto9045.cpp:902/923 and Command.cpp:9526-9551 and
//       SECSGEM/uHGemHT9045.cpp:3443-3447 (all parse an incoming command/SECS
//       message -- a live round-trip, not a boot-time load) and
//       ProductionInfo/uPAT_Function.cpp:1829-1830 (reads a `temp` struct
//       from a PAT-function source, also not a boot load). So temp.sv/soak/
//       mode stay null -- but the honest reason is "no safe loader exists
//       yet", not "nothing writes it".
//
//   (b) temp.pv + the 8 zone.* tags (UN150Read[tc*], cmydef.cpp:2768) --
//       genuinely dead, confirmed three independent ways:
//         * DoThermo() (bthermo.cpp:1262, writes UN150Read across ~80 sites)
//           is called only from THeaterThread::HeaterThreadProcess
//           (uHeaterThread.cpp:342/375), which is called only from
//           THeaterThread::Execute (:366) -- and Execute() is NEVER invoked
//           offline: Resume() is a documented no-op (uHeaterThread.cpp:
//           389-392, "offline: no real OS thread to resume").
//         * The other two UN150Read writers -- uTemp_Set.cpp:7228-7229 and
//           forms/fLotInfo.cpp:1291-1297 -- both sit inside `#if 0` blocks
//           (dep-ATC-IsConnect / the fLotInfo AirMachine display gate), so
//           they do not even compile in.
//         * ShowThermo's own SOFT_SIMULTE debug-fill branch
//           (cTemperFrom.cpp:338-348, `UN150Read[Addr]=Addr`) is call-site
//           unreachable outside tests/test_temperfrom_core.cpp (grepped, 0
//           production callers) AND compiled out anyway --
//           `//#define SOFT_SIMULTE` is commented at MachineType.h:48.
//       zone.* channel mapping (for whenever a writer lands): tcHotPlate1/2=0/1,
//       tcShuttle1/2=2/3, tcHead1/2=4/5 ("index" = the test-head channels),
//       tcHeatGun1/2=27/28 (MachineType.h:637-641) -- one UN150Read[] cell
//       each, same predicate as temp.pv.
//
//  Verification commands (20260820): `Grep "DoThermo\(\)\|HeaterThreadProcess\|
//  THeaterThread::Execute" *.cpp`; `Grep "ShowThermo\s*\(" *.cpp` (4 hits, all
//  tests/test_temperfrom_core.cpp); `Grep "SOFT_SIMULTE" MachineType.h`
//  (commented at :48); `Read uTemp_Set.cpp:2172-2340` (ReadTempFile head);
//  `Read uHeaterThread.cpp:330-415` (THeaterThread::Execute/Resume).
//
//  So the live tag set is small on purpose. It grows when the sources do, and
//  every addition has to answer "is this source loaded?" with a measurement.
//
//  THREADING
//  PublishHandlerTags() reads machine globals, so it must be called from the UI
//  thread, on the same tick that owns them -- never from the socket thread.
//  That is the contract in web/docs/ARCHITECTURE.md section 5, and it is the
//  reason the snapshot exists at all.
// =============================================================================
#ifndef WEBBRIDGETAGS_H
#define WEBBRIDGETAGS_H

#include "WebBridge/TagSnapshot.h"

#include <string>

namespace ht9045 {

// Fill `snap` with one full generation of tag values and commit it.
// UI THREAD ONLY. Returns the number of tags staged.
std::size_t PublishHandlerTags(webbridge::TagSnapshot& snap);

// AI(W906-FW-W3) 20260819: the web server's control-token holder, injected by
// the serving loop before each PublishHandlerTags() call so the snapshot can
// carry it as the `control.owner` tag (design doc section 3). 0 = nobody.
// Published as "conn-<id>" / "" -- every browser sees who operates.
void SetWebControlOwner(unsigned long long connId);

// AI(W906-FW-BIN1) 20260820: host-process marker that the BinSelect->
// SetTechDataToProd_Yield->ShowBinSel chain actually ran (wb_serve does it at
// boot under the DataPath dry-redirect). The six bin.* tags publish null
// until this says true -- "the recipe's bin table was loaded" is a source
// question, same philosophy as every other liveness key here.
void SetWebBinSelLoaded(bool loaded);

// ---------------------------------------------------------------------------
//  PUMP MODE  (wb_publish --pump)
//
//  AI(W906-SimPump) 20260813: the translated spine can be DRIVEN offline, and the
//  browser already binds machine.state -- "the big HALT/RUN word",
//  web/js/model/tagmap.js:47-48. This block is what connects the two, and it is
//  the first time anything outside tests/ advances the state machine.
//
//  AI(W906-IdlePump) 20260817: THE PUMP NO LONGER STARTS THE MACHINE.
//  PumpInit used to force SystemStart=true and fAllMotorHome=true so the engines
//  would run. The user rejected that -- "軟體開啟正常是不會 Start" -- because a
//  freshly opened BCB6 HT9045 sits IDLE until the operator presses HOME then
//  START. Both forced writes are gone. The spine is still driven every tick, the
//  master guard now correctly refuses, and the word reads "SIM IDLE".
//
//  ONE CORRECTION TO THIS BLOCK'S OWN EVIDENCE, found while making that change.
//  It claimed the second ScanSystemSensor call site, csystem.cpp:778, "is inside
//  `#if 0`". It is NOT. It sits inside `#ifdef DEBUG_TRY_CATCH / try { / #endif`
//  -- a conditional TRY WRAPPER, not conditional compilation of the call. The
//  macro is commented out (MachineType.h:23), so there is no try, but the call
//  compiles unconditionally and MainProc (csystem.cpp:595) reaches it every tick.
//  The rest of the measurement stands, and it is what makes the write path cheap:
//    * SystemStart=true  -- exactly ONE non-test writer, ckernel.cpp:1015, inside
//      `if(SoftStart==true)`. The OTHER caller, ckernel.cpp:4009-4010, does
//      `SoftStart=false;` on the line before the call, so it cannot take that arm.
//      But csystem.cpp:778 CAN: raise SoftStart and golden's real admission
//      sequence runs (lamps, shuttle floodgates, DoInArm_SuckerMap, safe-door
//      check) and sets SystemStart itself. That is how the browser's future START
//      button should work -- SCOPE.md section 2.6 -- and it is one flag away.
//    * fAllMotorHome=true -- exactly ONE, csystem.cpp:9709 in DoHomeProcess, which
//      this file's own tree records as having no caller and being unreachable at
//      runtime (csystem.cpp:9632 "HAS NO CALLER TODAY", csystem.cpp:8678). So the
//      browser will eventually need HOME as well as START, in that order, which is
//      also the real machine's operating sequence.
//
//  WHY THE WORD STILL SAYS "SIM". Nothing here is sensed: there is no hardware,
//  no tray, no IC, and the machine SHAPE below is still a test fixture rather than
//  a config load. "SIM IDLE"/"SIM HALT"/"SIM RUN" plus the machine.stateSource tag
//  beside it, and the raw guard terms published individually as pump.guard.* --
//  nothing derived is published without the inputs it was derived from.
//
//  WHAT IT IS AND IS NOT. Ticks advance, mainProcCalls tracks them, exceptions
//  stay 0: that is a LIVENESS harness, and it still is one. What it is NOT, and
//  now visibly is not, is a machine cycle -- the engine cursors do not move,
//  because the guard stops DoAllProcess before any engine runs. Do not report an
//  idle pump as production.
//
//  HISTORICAL NOTE, kept because it explains a symptom someone will hit again if
//  the guard is ever forced back on: with the engines running but no tray or IC
//  seeded, DoLoad sits in case 800 "Loader has no tray" (asendic_Loader.cpp:3091)
//  and the sim ShowErrorMessage answers K_RETRY every time
//  (canary_support.cpp:97), so the console fills at roughly 2 lines/sec with
//  `[ShowErrorMessage] Code=MES0920 KCode=5 Pos=168` (Pos 168 == MMTrayY_Car,
//  cmydef.cpp:2537; KCode 5 == K_RETRY|K_CLEAN_OUT, cmydef.cpp:337/339). That is
//  a machine asking a question with nobody to answer it -- never a hang.
// ---------------------------------------------------------------------------

// Put the translated spine into the offline OPENED-BUT-NOT-STARTED fixture and
// reset every engine cursor. Call ONCE, before the first PumpTick().
//AI(W906-IdlePump) 20260817: this used to say "Run" fixture. It no longer starts
// the machine -- see the block above.
//
// ORDERING IS LOAD-BEARING: call this AFTER any config load, never before.
// ReadGeneralIni OVERWRITES five of the very globals set here -- AUTO_EMPTY_COLOR
// (database.cpp:454), TRAY_VIBRATION (:593), USE_OUT_SORT_ARM (:703),
// MOTION_CARD_TYPE (:1079), AUTO3_IS_MAGAZINE (:1430) -- so the wrong order
// silently changes the tick shape with everything still green.
//
// Returns false, having changed NOTHING, if the sim canary does not hold; `whyNot`
// then carries the reason. A false return means DO NOT PUMP.
bool PumpInit(std::string& whyNot);

// One tick of the spine. Bounds the tick-oracle trace, then MainProc() inside a
// try/catch -- the compiled MainProc has none of its own, because DEBUG_TRY_CATCH
// is defined nowhere in the build (csystem.cpp:3015-3023). Never throws.
// No-op unless PumpInit() has succeeded.
void PumpTick();

// True once PumpInit() has succeeded. PublishHandlerTags() consults this itself,
// so the publish call needs no extra argument.
bool PumpActive();

// Publisher-side telemetry, for the tool's log line. All zero/false when not
// pumping. This exists so the TOOL never has to include csystem.h: keeping the
// god-stack confined to this translation unit is the same rule the file header
// states, and a printf is not a good enough reason to break it.
//
// `exceptions` is the one to watch. A publisher can serve frames perfectly while
// every single tick throws, and from the browser those two look identical.
struct PumpStats {
    unsigned long long ticks;
    unsigned int       mainProcCalls;   // golden's own counter, csystem.h:52
    unsigned long long exceptions;      // ticks that threw out of MainProc()
    bool               alive;           // IsMainProcAlive(60), csystem.h:56
};
PumpStats PumpTelemetry();

// How many of the tags this file knows about currently have a loaded source.
// Reported so "coverage" is a measured number rather than an impression.
//
//AI(W906-SimPump) 20260813: this counts MACHINE DATA SOURCES ONLY, and therefore
// excludes the 18 clock/state/pump tags added with pump mode -- those are the
// publisher's own clock and telemetry, not readings taken from the machine, so
// counting them would raise `live` without a single new machine source having been
// read. `total` here is consequently NOT the number of tags on the wire; for that,
// use PublishHandlerTags()'s return value (43 machine + 18 process = 61).
struct TagCoverage {
    std::size_t total;   // machine-sourced tags this file publishes
    std::size_t live;    // ...of which carry a real value right now
};
TagCoverage HandlerTagCoverage();

}  // namespace ht9045

#endif  // WEBBRIDGETAGS_H
