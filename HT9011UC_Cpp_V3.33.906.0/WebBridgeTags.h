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
//    DEAD    Temperature.*                      nothing writes it
//            IniConfig numeric/bool flags       loaded by cConfiguration.cpp,
//                                               which is untranslated (7,808
//                                               golden lines).  Only the few
//                                               STRING fields ReadLastSetIni
//                                               sets are live.
//            CosFunction (140 flags)            0 of 488 bytes set --
//                                               CustomerFunctionSelect runs but
//                                               every branch tests an IniConfig
//                                               flag that is not loaded yet
//            UN150Read (all temperature PVs)    0 of 71 zones -- no controller
//                                               polling in the port
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

// ---------------------------------------------------------------------------
//  PUMP MODE  (wb_publish --pump)
//
//  AI(W906-SimPump) 20260813: the translated spine can be DRIVEN offline, and the
//  browser already binds machine.state -- "the big HALT/RUN word",
//  web/js/model/tagmap.js:47-48. This block is what connects the two, and it is
//  the first time anything outside tests/ advances the state machine.
//
//  WHY THE WORD SAYS "SIM", AND WHY THAT IS NOT COSMETIC
//  The machine can never legitimately reach RUN in this tree. DoAllProcess's
//  master guard (csystem.cpp:4235, golden csystem.cpp:10049) needs
//  SystemStart && fAllMotorHome && !SoftStop, and BOTH of the true-writers are
//  unreachable -- measured 20260813, not assumed:
//    * SystemStart=true  -- exactly ONE non-test writer, ckernel.cpp:1015, inside
//      `if(SoftStart==true)`. Its only live caller executes `SoftStart=false;` on
//      the line before the call (ckernel.cpp:4009-4010), so that arm cannot be
//      entered. The other call site, csystem.cpp:778, is inside `#if 0`.
//    * fAllMotorHome=true -- exactly ONE, csystem.cpp:9709 in DoHomeProcess, which
//      this file's own tree records as having no caller and being unreachable at
//      runtime (csystem.cpp:9632 "HAS NO CALLER TODAY", csystem.cpp:8678).
//  So PumpInit() FORCES the guard terms, exactly as the passing W6.6 cycle test
//  does (tests/test_w6_6_csystem_cycle.cpp:135-156). A forced precondition is not
//  a sensed one, and an operator-facing word must not be able to hide that. Hence
//  "SIM RUN"/"SIM HALT" plus the machine.stateSource tag beside it, and hence the
//  raw guard terms published individually as pump.guard.* -- nothing derived is
//  published without the inputs it was derived from.
//
//  WHAT IT IS AND IS NOT. Cursors move, the A/B tick alternates, nothing crashes:
//  that is a LIVENESS harness. It is not a machine cycle -- with no tray or IC
//  seeded, DoLoad sits in case 800 ("Loader has no tray") and the sim
//  ShowErrorMessage returns K_RETRY (canary_support.cpp:57), so there is no
//  product progress to see. Do not report it as one.
// ---------------------------------------------------------------------------

// Put the translated spine into the offline "Run" fixture and reset every engine
// cursor. Call ONCE, before the first PumpTick().
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
