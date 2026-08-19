// =============================================================================
//  WebBridgeTags.cpp -- machine globals -> webbridge::TagSnapshot.
//  Contract and the measured live/dead source inventory are in WebBridgeTags.h.
//  Read that first; the "publish null, never 0" rule is the whole design.
// =============================================================================
#include "WebBridgeTags.h"

#include "database.h"
#include "cprod.h"
#include "Config.h"
#include "LastSet.h"
#include "cmydef.h"
//AI(W906-SimPump) 20260813: pump mode needs the spine entry + instrumentation
// (MainProc/InitAllProcessTask/GetMainProcCallCount/IsMainProcAlive), the
// machine-shape enums the guard fixture pins (eartUninstall/NonVibration), and
// bShuttleShake, whose home is the shims TU rather than a golden header.
#include "csystem.h"
#include "csystem_shims.h"
#include "MachineType.h"

#include "WebBridge/TagValue.h"

#include <cstdio>
#include <ctime>
#include <vector>

//AI(W906-SimPump) 20260813: engine cursors, declared here rather than by including
// their owning headers. Every one is a plain int, so an extern declaration is
// exact -- and pulling the real headers in would be actively risky: iArmTask's
// home, aHotPlateSubstrate.h, also carries a SECOND TMyKitSuck with a DIFFERENT
// layout from mykitsuck.h's, and 14 globals collide across the two. Picking the
// wrong one links cleanly and reads every field at the wrong offset. A plain
// `extern int` cannot be wrong that way.
extern int iArmTask;              // in-arm      -- aHotPlateSubstrate.h:945
extern int OutArmTask;            // out-arm     -- aoutarm.h:77
extern int AutoSHT1Task;          // shuttle 1   -- acarry.h:39
extern int AutoSHT2Task;          // shuttle 2   -- acarry.h:40
extern int iTestHeadMotorTask;    // index/tester-- atester.h:169
extern int LoadTask;              // loader      -- asendic_Loader.h:53
// CatchTrayTask is already declared by csystem.h:63 (also acatchtray.h:48).

//AI(W906-SimPump) 20260813: the tick oracle. CSYSTEM_TICK_ORACLE is compiled
// UNCONDITIONALLY into ht9045_sm (CMakeLists.txt:2196), so these two symbols are
// present in every binary that links it -- no test-only lib variant is involved.
// That also means g_csystemTrace GROWS ON EVERY ENGINE CALL for the life of the
// process (csystem.cpp:196) and csystemTraceClear() is the ONLY thing that bounds
// it. Calling it each tick is not optional in a process that runs for hours.
extern std::vector<int> g_csystemTrace;   // csystem.cpp:194
extern void csystemTraceClear();          // csystem.cpp:195

namespace ht9045 {

using webbridge::TagValue;

namespace {

// ---------------------------------------------------------------------------
//  Source liveness predicates.
//
//  Each answers "has anything actually loaded this?" for one source. They are
//  deliberately separate from the value getters: a getter that also decided
//  liveness would make it far too easy to fall back to 0 and call it a reading.
//
//  These are conservative on purpose. A source that MIGHT be loaded is treated
//  as dead, because a wrong number on an operator screen costs more than a
//  missing one.
// ---------------------------------------------------------------------------

// IniConfig's STRING fields are set by ReadLastSetIni (cprod.cpp). Its hundreds
// of numeric/bool feature flags are NOT -- those come from cConfiguration.cpp,
// which is untranslated. So liveness has to be asked per field group, not for
// "IniConfig" as a whole.
bool IniConfigStringsLoaded()
{
    return IniConfig.sMachineType.Length() > 0;
}

// CUSTOMER_CODE is assigned inside ReadGeneralIni. 0 is also a legal customer
// code in principle, but in this tree it is the not-yet-read value -- and
// treating a real 0 as "unknown" is the safe direction of the two.
bool CustomerCodeLoaded()
{
    return CUSTOMER_CODE != 0;
}

// LastSet arrives as one raw blob from system\lastdata.dat. An all-zero struct
// means no file was read; a partially-zero one is normal (that file is only
// ~7% non-zero on a typical box), so liveness is a property of the BLOB, not of
// any single field.
bool LastSetLoaded()
{
    const unsigned char* raw = reinterpret_cast<const unsigned char*>(&LastSet);
    for (std::size_t i = 0; i < sizeof(LAST_GENERAL_SET); ++i) {
        if (raw[i] != 0) return true;
    }
    return false;
}

// Nothing in the ported tree writes SYSTEM_TEMPERATURE. Measured 20260806: every
// field reads 0 after a full LoadMachineConfig(). Kept as a named predicate
// rather than a comment so the day it changes, one line moves.
bool TemperatureLoaded()
{
    return false;
}

// UN150Read[] holds every temperature present-value. Nothing writes it either --
// there is no controller polling in the port. Checked at runtime rather than
// hard-coded false, because this one will light up as soon as TempCtrl lands.
bool TemperaturePvLoaded()
{
    for (int i = 0; i < tcTotalCount; ++i) {
        if (UN150Read[i] != 0.0) return true;
    }
    return false;
}

// The 140 customer-profile flags. CustomerFunctionSelect() runs, but every
// branch in it tests an IniConfig flag that is not loaded, so it sets nothing:
// 0 of 488 bytes, measured.
bool CosFunctionLoaded()
{
    const unsigned char* raw = reinterpret_cast<const unsigned char*>(&CosFunction);
    for (std::size_t i = 0; i < sizeof(CosFunction); ++i) {
        if (raw[i] != 0) return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
//  Staging helpers. Every one takes a liveness flag, so that the null-vs-value
//  decision is visible at each call site instead of buried in a getter.
// ---------------------------------------------------------------------------
void stageStr(webbridge::TagSnapshot& s, const char* tag, bool live, const AnsiString& v)
{
    s.stage(tag, live ? TagValue::makeString(std::string(v.c_str()))
                      : TagValue::makeNull());
}

void stageInt(webbridge::TagSnapshot& s, const char* tag, bool live, long long v)
{
    s.stage(tag, live ? TagValue::makeInt(static_cast<std::int64_t>(v))
                      : TagValue::makeNull());
}

void stageNull(webbridge::TagSnapshot& s, const char* tag)
{
    s.stage(tag, TagValue::makeNull());
}

//AI(W906-SimPump) 20260813: bool needs its own stager -- routing a bool through
// stageInt would publish 1/0, and the UI's boolean modes test truthiness
// (data-lit / btn--on, web/js/ui/bind.js:141-149) where a JSON number and a JSON
// bool are not interchangeable for a reader trying to tell a flag from a count.
void stageBool(webbridge::TagSnapshot& s, const char* tag, bool live, bool v)
{
    s.stage(tag, live ? TagValue::makeBool(v) : TagValue::makeNull());
}

// The home screen's tags whose source is measurably not loaded. Publishing them
// as null is not a placeholder -- it is the correct value, and it makes the
// extent of the gap visible on the screen instead of hiding it behind zeros.
const char* const kUnloadedTags[] = {
    // temperature: Temperature.* and UN150Read[] are both dead
    "temp.pv", "temp.sv", "temp.soak", "temp.mode",
    "zone.hotplate.1", "zone.hotplate.2",
    "zone.shuttle.1",  "zone.shuttle.2",
    "zone.index.1",    "zone.index.2",
    "zone.heatgun.1",  "zone.heatgun.2",
    // tester identity comes from TestIF_File, which is not loaded
    "tester.name",
    // signal tower is driven by IO output state; the IO layer is offline
    "tower.red", "tower.amber", "tower.green",
    // run/mode panels need IniConfig feature flags (cConfiguration.cpp untranslated)
    "run.ft", "run.rt", "run.offline",
    "runmode.value", "runmode.normal", "runmode.prime",
    "light.off", "fan.off",
    // throughput counters are computed by main.cpp, which is untranslated
    "status.indexTime", "status.uph",
    "status.powerSaving", "status.cleanCount", "status.ep",
    "status.safeDoor", "status.rtc", "status.rms", "status.ftp",
    "status.triTemp", "status.saveSummary"
};

const std::size_t kUnloadedCount = sizeof(kUnloadedTags) / sizeof(kUnloadedTags[0]);

// ---------------------------------------------------------------------------
//  PUMP MODE internals.  Contract, and the reachability measurement that forces
//  the "SIM" prefix, are in WebBridgeTags.h -- read that before changing any of
//  this.  AI(W906-SimPump) 20260813.
// ---------------------------------------------------------------------------

// Engine ids the tick oracle records; must match the enum at csystem.cpp:198-201.
enum {
    CT_SENSORSCAN = 1, CT_DOLOAD = 2, CT_DOINARM = 3, CT_SHT1 = 4, CT_SHT2 = 5,
    CT_SHT3 = 6, CT_TESTHEAD = 7, CT_CATCHTRAY = 8, CT_OUTARM = 9, CT_SORTARM = 10
};

bool               g_pumpActive     = false;
unsigned long long g_pumpTicks      = 0;
unsigned long long g_pumpExceptions = 0;
int                g_lastTickKind   = 0;   // 1 = A tick, 2 = B tick, 0 = neither

bool traceHas(const std::vector<int>& v, int id)
{
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (v[i] == id) return true;
    }
    return false;
}

// Classify one tick, same rule as tests/test_w6_6_csystem_cycle.cpp:161-170.
// bDoProcess (csystem.cpp:4216, toggled :4448) makes the spine alternate:
//   A = DoInArm + SHT1, no OutArm/SHT2      B = DoOutArm + SHT2, no DoInArm/SHT1
// "neither" is not an error on a guard-truncated tick -- it is the honest answer.
int classifyTick(const std::vector<int>& v)
{
    const bool inarm  = traceHas(v, CT_DOINARM);
    const bool sht1   = traceHas(v, CT_SHT1);
    const bool outarm = traceHas(v, CT_OUTARM);
    const bool sht2   = traceHas(v, CT_SHT2);
    if ( inarm &&  sht1 && !outarm && !sht2) return 1;
    if (!inarm && !sht1 &&  outarm &&  sht2) return 2;
    return 0;
}

// The three master-guard terms, re-read live. This is the ONLY thing behind the
// state word -- csystem.cpp:4235 (golden csystem.cpp:10049), re-evaluated by the
// spine 8 times per tick, so a mid-tick flip truncates the tick.
bool guardAllowsEngines()
{
    return SoftStop == false && SystemStart == true && fAllMotorHome == true;
}

}  // namespace

// ---------------------------------------------------------------------------
//AI(W906-SimPump) 20260813: see the PUMP MODE block in WebBridgeTags.h.
bool PumpInit(std::string& whyNot)
{
    // ---- sim canary -------------------------------------------------------
    // LastSet.iRealDummy==DUMMY(0) != REALLY, together with a non-Contec motion
    // card, is what makes CheckInShuttleSensor_Latch return 1 (Finish) at
    // ainarm9045.cpp:1891-1893 BEFORE any CCLink/Ltc/MOT body runs. Those bodies
    // have never executed offline. If a config load has flipped both terms, the
    // interlock is live and pumping would be the first thing to ever run it --
    // refuse rather than find out.
    if (MOTION_CARD_TYPE == MotionCard_Contec && LastSet.iRealDummy == REALLY) {
        whyNot = "sim canary violated (MOTION_CARD_TYPE==Contec AND "
                 "LastSet.iRealDummy==REALLY): the hardware interlock at "
                 "ainarm9045.cpp:1891 is live -- refusing to pump";
        return false;
    }

    // ---- the offline fixture: OPENED, NOT STARTED --------------------------
    //AI(W906-IdlePump) 20260817: this used to be the "Run" fixture and it FORCED
    // SystemStart=true and fAllMotorHome=true. The user rejected that, and was
    // right to: "軟體開啟正常是不會 Start" -- a freshly opened BCB6 HT9045 sits
    // IDLE waiting for the operator to press HOME then START. Forcing the guard
    // made the port show a machine mid-production-attempt, which is a state the
    // real machine is never in on startup, and the visible symptom was a console
    // filling with `[ShowErrorMessage] Code=MES0920 KCode=5 Pos=168` -- DoLoad
    // case 800 "Loader has no tray" (asendic_Loader.cpp:3091) retrying forever
    // because the sim ShowErrorMessage always answers K_RETRY.
    //
    // The two lines are simply GONE, not replaced by false: these are globals the
    // data layer already zero-initialises, and writing false here would imply this
    // function had a say in the machine's run state. It does not any more.
    //
    // WHAT STILL HAPPENS EVERY TICK, so this is not a downgrade to nothing:
    // MainProc (csystem.cpp:595) is entered, runs its sensor scan, and calls
    // ScanSystemSensor (csystem.cpp:778) -- then DoAllProcess returns at the master
    // guard (csystem.cpp:4235) before DoLoad. So ticks, mainProcCalls, exceptions=0
    // and alive still prove the spine is live; the engine cursors correctly do not
    // move, because no engine ran.
    //
    // HOW START WILL ARRIVE (SCOPE.md section 2.6, the browser->core write path):
    // set SoftStart=true. ScanSystemSensor already tests `if(SoftStart==true)`
    // (ckernel.cpp:816) and, on that arm, runs golden's REAL admission sequence --
    // lamp reset, shuttle floodgates, DoInArm_SuckerMap, the safe-door check -- and
    // sets SystemStart itself at ckernel.cpp:1015. That path is LIVE in this tree
    // and one flag away; do NOT reintroduce a direct SystemStart write.
    //
    // The rest of this block is unchanged and is NOT run state -- it is machine
    // SHAPE (which machine this is), still verbatim from
    // tests/test_w6_6_csystem_cycle.cpp:135-156 and still load-bearing on tick
    // shape once something does start the machine.
    InitialOK                 = true;            // MainProc head guard, csystem.cpp:3001
                                                 //   -- kept: a real opened program
                                                 //   DOES finish initialisation.
    SoftStop                  = false;           // master guard, csystem.cpp:4235
                                                 //   -- kept: "not soft-stopped" is
                                                 //   the state of a fresh boot.
    bShuttleShake             = false;           // else shuttle+index block skipped, :4265

    TrayForm.bEnableAMR       = false;           // plain DoLoad path, :4242
    USE_OUT_SORT_ARM          = eartUninstall;   // no SHT3/SortArm, :4286/:4315

    AUTO_EMPTY_COLOR          = 0;               // :4336 -> cmpt=3; see NOTE below
    AUTO3_IS_MAGAZINE         = 0;               // no DoAuto3Magazine, :4331
    TRAY_VIBRATION            = NonVibration;    // no tray-edge cylinder loop, :4383
    SUPPORT_2_EMPTY_EMPTY     = false;           // no DoAutoEmpty1, :4411
    bUseAuto2Empty            = false;           // no DoAuto2, :4408

    bLoaderNeedTrayMustFinish = false;
    bAutoNeedTrayMustFinish   = false;
    bRunInArmAutoAlignment    = false;           // else DoAllProcess returns at :4440
    bRunOutArmAutoAlignment   = false;           //   BEFORE the bDoProcess toggle :4448,
                                                 //   freezing the A/B parity.
    // NOTE, verified 20260813: the test's own comment at
    // tests/test_w6_6_csystem_cycle.cpp:146 says AUTO_EMPTY_COLOR=0 "skip[s] the
    // DoAutoReceiveBinTray loop". That comment is WRONG -- 0 takes the `<3` branch
    // at csystem.cpp:4336 and sets cmpt=3, so DoAutoReceiveBinTray(0..2) runs every
    // tick. Harmless here (it is why BinTrayTask moves at all), but recorded so the
    // next reader does not inherit the mistake.

    InitAllProcessTask();                        // per-engine cursor reset, csystem.cpp:241
    g_pumpActive = true;
    whyNot.clear();
    return true;
}

void PumpTick()
{
    if (!g_pumpActive) return;

    // Bounds g_csystemTrace. Not cosmetic: nothing else trims it, so skipping this
    // is an unbounded leak for the life of the process (csystem.cpp:196).
    csystemTraceClear();

    // The compiled MainProc path (csystem.cpp:3020-3022) has NO try/catch of its
    // own -- its #ifdef DEBUG_TRY_CATCH pair is dead because DEBUG_TRY_CATCH is
    // defined nowhere in the build. One escaping exception would otherwise take the
    // whole publisher down and, without SetErrorMode, do it behind a modal box.
    try {
        MainProc();
    } catch (...) {
        ++g_pumpExceptions;
    }

    ++g_pumpTicks;
    g_lastTickKind = classifyTick(g_csystemTrace);
}

bool PumpActive()
{
    return g_pumpActive;
}

PumpStats PumpTelemetry()
{
    PumpStats st;
    st.ticks         = g_pumpTicks;
    st.exceptions    = g_pumpExceptions;
    st.mainProcCalls = g_pumpActive ? GetMainProcCallCount() : 0u;
    st.alive         = g_pumpActive ? IsMainProcAlive(60) : false;
    return st;
}

// ---------------------------------------------------------------------------
std::size_t PublishHandlerTags(webbridge::TagSnapshot& snap)
{
    const bool strs  = IniConfigStringsLoaded();
    const bool cust  = CustomerCodeLoaded();
    const bool lastS = LastSetLoaded();

    snap.beginPublish();

    // --- machine identity: the part that IS loaded today --------------------
    stageStr(snap, "machine.id.type",   strs, IniConfig.sMachineType);
    stageStr(snap, "machine.id.gpib",   strs, IniConfig.sGPIBMachineID);
    stageStr(snap, "machine.id.tester", strs, IniConfig.RMSTesterID);
    stageInt(snap, "machine.customerCode", cust, CUSTOMER_CODE);

    // AI(W906-FW-W2) 20260819: the browser's login state mirror (design doc
    // section 2 -- replaces golden's ChangeLevelAttr widget-enable pass).
    // AccessLevel is a plain int global (cmydef.h:3503). Liveness keys on
    // the config-loaded signal (cust) to PRESERVE test_wb_tags' load-bearing
    // invariant "every tag is null before the data layer is loaded": a
    // pre-config snapshot publishes auth.level as null; post-config it is
    // live, and 0 there is the real Operator state.
    stageInt(snap, "auth.level", cust, AccessLevel);

    // --- LastSet-derived scalars --------------------------------------------
    // These read ONLY LastSet, so the blob's liveness settles them. Their
    // meaning is deliberately not interpreted here (no "Real"/"Dummy" label):
    // that mapping lives in golden's UI code and has not been verified, and a
    // confidently wrong label is worse than a raw code.
    stageInt(snap, "lastset.tester",     lastS, LastSet.iTester);
    stageInt(snap, "lastset.realDummy",  lastS, LastSet.iRealDummy);
    stageInt(snap, "lastset.runStartMode", lastS, LastSet.iRunStartMode);
    stageInt(snap, "lastset.temperature",  lastS, LastSet.iTemperature);

    // --- FW-1a: the start-mode word + the 2-arm x 16-site map ----------------
    //AI(W906-FW1) 20260817: first tag-wiring batch of the FW campaign
    // (docs/DFM2WEB_CAMPAIGN_PLAN.md 4-FW-1). Both families read ONLY the
    // LastSet blob, so the blob's liveness settles them like the scalars above.
    //
    // startmode.value decodes through golden's OWN display array,
    // StartModeName[rsmRunModeTotal] (cmydef.cpp:62, translated verbatim) --
    // the same lookup fMain does at golden main.cpp:560 -- rather than a
    // hand-written mapping that could drift. The raw code stays on the wire as
    // lastset.runStartMode so a reader can always cross-check the decode.
    // Out-of-range codes (rsmNull = -1, or a corrupt blob) publish null: a
    // missing word costs less than a confidently wrong one.
    {
        const int m = LastSet.iRunStartMode;
        const bool decodable = lastS && m >= 0 && m < rsmRunModeTotal;
        stageStr(snap, "startmode.value", decodable,
                 decodable ? StartModeName[m] : AnsiString());
    }

    // site.arm{a}.s{n} <- LastSet.bUseTestSocket[a-1][(n-1)/8][(n-1)%8]
    // ([2][4][8], LastSet.h:431).
    //
    // Source choice, verified against cprod.cpp:3630-3807 (ReadTestMode) and
    // deliberately NOT the audit's first suggestion TestMode.iDutOnOff: when
    // CosFunction.bLastSetInSetUpFile==false that function returns at :3642
    // BEFORE filling iDutOnOff, so the TestMode array can sit all-zero while
    // the real site map exists -- publishing it would show "every site off" as
    // a confident value. bUseTestSocket IS the persisted truth in both
    // branches: ReadTestMode seeds its ini reads from it AND writes its result
    // back into it (:3712-3713, :3797-3798).
    //
    // First-dimension semantics measured, not assumed: [0] is Arm1 (ini key
    // "Dut <name>", :3689->:3693), [1] is Arm2 ("Dut <name>2", :3773->:3777);
    // FT/RT selects which ini SECTION is read (DutOnOff vs DutOnOff_RT), never
    // the dimension. Site numbering follows tagmap.js "site.*" (XItem=8):
    // sites 1..8 = row 0, 9..16 = row 1, col = (n-1)%8. Rows 2..3 of the
    // 4-row array belong to shapes this screen does not show; not published.
    for (int arm = 0; arm < 2; ++arm) {
        for (int nSite = 1; nSite <= 16; ++nSite) {
            char tag[24];
            std::sprintf(tag, "site.arm%d.s%d", arm + 1, nSite);
            stageInt(snap, tag, lastS,
                     LastSet.bUseTestSocket[arm][(nSite - 1) / 8][(nSite - 1) % 8] ? 1 : 0);
        }
    }

    // --- FW-1b: sort counters (Auto1..3 / Fix1..3 place-to-tray counts) ------
    //AI(W906-FW1b) 20260817: source LastSet.BinCT[4][256] (LastSet.h:179 =
    // golden LastSet.h:106) -- the exact cell golden's fSortCT displays:
    // cSortCT.cpp:399  pnlCount->Caption = LastSet.BinCT[0][iTo3Unload[i]].
    //
    // TWO index spaces, verified against golden main.cpp:1954-1966 and
    // MachineType.h; conflating them is the exact trap this block dodges:
    //   display gate uses e6TrayName:  eAuto1..3 = 0..2,  eFix1..3 = 6..8
    //   BinCT column uses e3TrayName:  e3Auto1..3 = 0..2, e3Fix1..3 = 3..5
    // The port's own iTo3Unload[] global is DECLARED but never initialized
    // (golden fills it in the untranslated TfMain ctor) -- it reads all-zero
    // today, so publishing through it would silently point every Fix at
    // Auto1's cell. Constants are used directly instead. (That all-zero
    // global is ALSO consumed by translated runtime code, e.g.
    // aoutarm.cpp:3332 -- recorded as a latent port defect, separate issue.)
    //
    // Per-tag liveness mirrors golden's own display gate
    // (Prod.iTrayType[i]!=tNotUse, cSortCT.cpp:396): an unconfigured station
    // publishes null, exactly like golden draws nothing for it. With Prod
    // unloaded (all zero == tNotUse) every station is null -- honest.
    {
        static const struct { const char* tag; int typeIdx; int binIdx; } kSort[6] = {
            {"sort.auto1.count", eAuto1, e3Auto1},
            {"sort.auto2.count", eAuto2, e3Auto2},
            {"sort.auto3.count", eAuto3, e3Auto3},
            {"sort.fix1.count",  eFix1,  e3Fix1 },
            {"sort.fix2.count",  eFix2,  e3Fix2 },
            {"sort.fix3.count",  eFix3,  e3Fix3 },
        };
        for (int i = 0; i < 6; ++i) {
            const bool binLive = lastS && Prod.iTrayType[kSort[i].typeIdx] != tNotUse;
            stageInt(snap, kSort[i].tag, binLive, LastSet.BinCT[0][kSort[i].binIdx]);
        }
    }

    // --- everything whose source is measurably dead --------------------------
    for (std::size_t i = 0; i < kUnloadedCount; ++i) {
        stageNull(snap, kUnloadedTags[i]);
    }

    // --- machine state + pump telemetry -------------------------------------
    //AI(W906-SimPump) 20260813: WITHOUT a pump these are NULL, not "HALT". We are
    // not driving the spine, so we have no state to report, and "HALT" would be a
    // claim about a machine we are not observing. With a pump the word reports the
    // LIVE master guard, re-read here every tick, prefixed "SIM " because this
    // process forced the guard terms rather than sensing them (WebBridgeTags.h).
    const bool pumping = g_pumpActive;

    // --- the wall clock ------------------------------------------------------
    //AI(W906-SimPump) 20260813: gated on `pumping` like every other process tag,
    // and that gate is deliberate rather than incidental.
    //
    // My first version published it unconditionally, on the reasoning that the
    // publisher always knows its own clock. That broke a REAL invariant asserted at
    // tests/test_wb_tags.cpp:68 -- "every tag is null before the data layer is
    // loaded (nothing is inventing values)" -- whose stated purpose (:53-55) is to
    // be the control for the liveness checks after it. Relaxing that assertion to
    // accommodate one convenience tag would have traded a strong, simple invariant
    // for a weaker one coupled to a tag-naming convention. Gating instead keeps the
    // invariant untouched and costs nothing that matters: the F5 compound runs
    // --pump, and a publisher that is not driving the spine has a static screen by
    // definition, so a ticking clock on it would be the misleading part.
    //
    // Format matches what the UI was authored against (web/js/model/state.js:63).
    if (pumping) {
        const std::time_t now = std::time(0);
        const std::tm*    lt  = std::localtime(&now);
        if (lt != 0) {
            char buf[64];
            std::sprintf(buf, "%04d / %02d / %02d   %02d:%02d",
                         lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                         lt->tm_hour, lt->tm_min);
            snap.stage("clock.text", TagValue::makeString(buf));
        } else {
            stageNull(snap, "clock.text");
        }
    } else {
        stageNull(snap, "clock.text");
    }

    if (pumping) {
        //AI(W906-IdlePump) 20260817: three words, not two, and the third is the
        // normal one now. HALT and IDLE are NOT the same machine state and an
        // operator-facing word must not conflate them: HALT means something stopped
        // the machine (SoftStop), IDLE means it was opened and nobody has started it
        // yet. Since PumpInit stopped forcing the guard, IDLE is what a freshly
        // launched publisher reports -- which is the point of the change.
        //
        // Safe to add a third string: web/js/panels/left.js:23 binds machine.state
        // as plain text into div.halt__state with no tone tag and no switch on the
        // value, so the page renders whatever it is told (checked 20260817).
        const bool run  = guardAllowsEngines();
        const char* word = run      ? "SIM RUN"
                         : SoftStop ? "SIM HALT"
                                    : "SIM IDLE";
        snap.stage("machine.state", TagValue::makeString(word));
        snap.stage("machine.stateSource",
                   TagValue::makeString(run
                       ? "sim: guard terms are true; no machine is attached"
                       : "sim: spine is pumping, machine not started "
                         "(SoftStart never raised); no machine is attached"));
    } else {
        stageNull(snap, "machine.state");
        stageNull(snap, "machine.stateSource");
    }

    // Raw guard terms, published individually so the derived word above can always
    // be checked against its inputs rather than trusted.
    stageBool(snap, "pump.guard.softStop",     pumping, SoftStop);
    stageBool(snap, "pump.guard.systemStart",  pumping, SystemStart);
    stageBool(snap, "pump.guard.allMotorHome", pumping, fAllMotorHome);

    // Liveness. guMainProcCallCount is file-static in csystem.cpp, so it is read
    // through golden's own accessor (csystem.h:52) rather than an extern.
    stageInt (snap, "pump.ticks",         pumping, static_cast<long long>(g_pumpTicks));
    stageInt (snap, "pump.mainProcCalls", pumping, GetMainProcCallCount());
    stageInt (snap, "pump.exceptions",    pumping, static_cast<long long>(g_pumpExceptions));
    stageBool(snap, "pump.alive",         pumping, IsMainProcAlive(60));
    if (pumping) {
        snap.stage("pump.tickKind",
                   TagValue::makeString(g_lastTickKind == 1 ? "A" :
                                        g_lastTickKind == 2 ? "B" : "?"));
    } else {
        stageNull(snap, "pump.tickKind");
    }

    // Engine cursors. These are the things that actually MOVE, and therefore the
    // evidence that the translated engines are cycling rather than merely linked.
    stageInt(snap, "pump.task.load",      pumping, LoadTask);
    stageInt(snap, "pump.task.inArm",     pumping, iArmTask);
    stageInt(snap, "pump.task.outArm",    pumping, OutArmTask);
    stageInt(snap, "pump.task.sht1",      pumping, AutoSHT1Task);
    stageInt(snap, "pump.task.sht2",      pumping, AutoSHT2Task);
    stageInt(snap, "pump.task.testHead",  pumping, iTestHeadMotorTask);
    stageInt(snap, "pump.task.catchTray", pumping, CatchTrayTask);

    const std::size_t staged = snap.stagedTagCount();
    snap.commitPublish();
    return staged;
}

// ---------------------------------------------------------------------------
TagCoverage HandlerTagCoverage()
{
    TagCoverage c;

    const bool strs  = IniConfigStringsLoaded();
    const bool cust  = CustomerCodeLoaded();
    const bool lastS = LastSetLoaded();

    // 3 identity strings + 1 customer code + 4 LastSet scalars
    // + FW-1a's 33 LastSet-blob tags (startmode.value + 32 site cells)
    // + the dead set.
    //AI(W906-FW1) 20260817: the 33 count under lastS because coverage asks
    // about SOURCES -- the blob either loaded or it did not; startmode's
    // per-value range guard is a publish-time concern, not a liveness one.
    //AI(W906-FW-W2) 20260819: +1 = auth.level (always-live login mirror).
    c.total = 3 + 1 + 1 + 4 + 33 + kUnloadedCount;
    c.live  = (strs ? 3u : 0u) + (cust ? (1u + 1u /*auth.level*/) : 0u) + (lastS ? (4u + 33u) : 0u);

    //AI(W906-FW1b) 20260817: the 6 sort counters are gated per station
    // (LastSet blob AND Prod.iTrayType configured), so their live count is
    // genuinely dynamic -- counted station by station, same rule as the
    // publish path, never flattered to "6 if the blob loaded".
    {
        static const int kSortTypeIdx[6] = {eAuto1, eAuto2, eAuto3, eFix1, eFix2, eFix3};
        c.total += 6;
        for (int i = 0; i < 6; ++i) {
            if (lastS && Prod.iTrayType[kSortTypeIdx[i]] != tNotUse) c.live += 1;
        }
    }

    //AI(W906-SimPump) 20260813: the 18 clock/state/pump tags are DELIBERATELY NOT
    // counted here, and the reason is the same one this file exists for.
    //
    // TagCoverage answers exactly one question: "how many MACHINE DATA SOURCES are
    // actually loaded?". clock.text is the publisher's own wall clock and pump.* is
    // the publisher's own telemetry -- neither is a reading taken from the machine.
    // machine.state is excluded too, because under --pump it reports a fixture this
    // process FORCED rather than a condition it sensed (see WebBridgeTags.h).
    //
    // Counting any of them would push `live` up without a single new machine source
    // having been read -- precisely the flattering denominator this file's own rule
    // is meant to prevent, and it would also have broken the invariant asserted at
    // tests/test_wb_tags.cpp:75 ("0 live before loading"), which is a genuine
    // property worth keeping rather than an assertion to update.
    //
    // The true count of tags on the wire is PublishHandlerTags()'s return value
    // (snap.stagedTagCount()): 100 = 82 machine + 18 process since FW-1b
    // (61 before 20260817; +33 FW-1a LastSet tags; +6 FW-1b sort counters).
    // Coverage and wire-count are different questions; this struct answers the first.

    // Referenced so the currently-always-false predicates cannot rot into
    // unused code and silently stop being checked when their sources land.
    if (TemperatureLoaded() || TemperaturePvLoaded() || CosFunctionLoaded()) {
        // Deliberately empty: reaching here means a source listed as dead in
        // WebBridgeTags.h has come alive, and that header's inventory -- plus
        // the kUnloadedTags table -- needs updating.
    }

    return c;
}

}  // namespace ht9045
