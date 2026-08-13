// =============================================================================
//  test_wb_simpump.cpp -- WebBridgeTags PUMP MODE: does the state word tell the
//  truth, and does the spine actually advance?
//
//  AI(W906-SimPump) 20260813.
//
//  WHAT THIS GUARDS
//  Pump mode is the first thing outside tests/ that DRIVES the translated state
//  machine, and it puts a word on an operator-facing screen. Two ways that can go
//  wrong, and this TU exists for both:
//
//    (1) THE WORD LIES. machine.state must be null when we are not pumping (we are
//        not observing a machine, so we have nothing to report) and must track the
//        LIVE master guard when we are -- not a value latched at startup. O6 below
//        flips a guard term mid-run and requires the word to follow.
//
//    (2) THE PUMP DOES NOT ACTUALLY PUMP. A publisher can serve frames perfectly
//        while MainProc is never called, or called and throwing every time, and
//        from the browser those look identical to a healthy one. O3/O4 pin the
//        golden call-count and the exception count separately for that reason.
//
//  NO CONFIG IS LOADED, AND THAT IS PART OF THE TEST. PumpInit() is specified to
//  work with zero config (WebBridgeTags.h), the sim canary holds naturally at the
//  static-init defaults (MOTION_CARD_TYPE==0 != MotionCard_Contec==1, cmydef.cpp:3643
//  and cmydef.h:104; LastSet.iRealDummy==0==DUMMY != REALLY==2, cmydef.cpp:265-267),
//  and not loading it means this TU cannot write the shared production config the
//  way tests/test_wb_tags.cpp has to guard against.
// =============================================================================
#include "WebBridgeTags.h"

#include "cmydef.h"
#include "csystem.h"
#include "LastSet.h"   // LastSet.iRealDummy -- the sim canary's second term (O7)

#include "WebBridge/TagSnapshot.h"
#include "WebBridge/TagValue.h"

#include <cstdio>
#include <string>
#include <vector>

// The tick oracle, to prove the trace stays BOUNDED across ticks -- csystemTraceClear()
// inside PumpTick() is the only thing that bounds it (csystem.cpp:194-196).
extern std::vector<int> g_csystemTrace;

static int g_total = 0;
static int g_fail  = 0;

static void check(bool ok, const char* what)
{
    ++g_total;
    std::printf("%s: %s\n", ok ? "PASS" : "FAIL", what);
    if (!ok) ++g_fail;
}

// Fetch one tag out of a freshly published generation.
static webbridge::TagValue publishAndGet(webbridge::TagSnapshot& snap, const char* tag)
{
    ht9045::PublishHandlerTags(snap);
    const webbridge::TagSnapshotView v = snap.read();
    webbridge::TagMap::const_iterator it = v.tags.find(tag);
    if (it == v.tags.end()) return webbridge::TagValue::makeNull();
    return it->second;
}

int main()
{
    std::setvbuf(stdout, 0, _IONBF, 0);
    std::printf("==== WebBridgeTags pump mode (SIM RUN/HALT word + spine advance) ====\n");

    webbridge::TagSnapshot snap;

    // -------------------------------------------------------------------------
    //  O1 -- BEFORE PumpInit: the pump publishes nothing, and says so with null.
    // -------------------------------------------------------------------------
    std::printf("\n-- O1 pre-init: no pump, therefore no state to report\n");
    check(ht9045::PumpActive() == false, "O1 PumpActive() is false before PumpInit()");
    check(publishAndGet(snap, "machine.state").isNull(),
          "O1 machine.state is NULL before pumping (not \"HALT\" -- we are not "
          "observing a machine, so \"HALT\" would be an unsupported claim)");
    check(publishAndGet(snap, "clock.text").isNull(),
          "O1 clock.text is NULL before pumping");
    check(publishAndGet(snap, "pump.task.load").isNull(),
          "O1 pump.task.load is NULL before pumping");
    {
        const ht9045::PumpStats st = ht9045::PumpTelemetry();
        check(st.ticks == 0 && st.mainProcCalls == 0 && st.exceptions == 0 &&
              st.alive == false,
              "O1 telemetry is all-zero before pumping");
    }

    // The invariant tests/test_wb_tags.cpp:68-75 relies on, re-asserted here so a
    // future pump tag that forgets to gate itself fails in THIS file too, next to
    // the code that would have caused it.
    {
        webbridge::TagSnapshot s2;
        ht9045::PublishHandlerTags(s2);
        const webbridge::TagSnapshotView v = s2.read();
        std::size_t nonNull = 0;
        for (webbridge::TagMap::const_iterator it = v.tags.begin();
             it != v.tags.end(); ++it) {
            if (!it->second.isNull()) ++nonNull;
        }
        std::printf("   pre-init snapshot: %u tags, %u non-null\n",
                    (unsigned)v.tags.size(), (unsigned)nonNull);
        check(nonNull == 0,
              "O1 EVERY tag is null pre-init (no process tag leaks a value into "
              "the pre-load control state)");
        const ht9045::TagCoverage c = ht9045::HandlerTagCoverage();
        check(c.live == 0,
              "O1 machine-source coverage is still 0 live (process tags are "
              "excluded from the coverage denominator on purpose)");
    }

    // -------------------------------------------------------------------------
    //  O2 -- PumpInit arms the fixture. The canary holds at static-init defaults.
    // -------------------------------------------------------------------------
    std::printf("\n-- O2 PumpInit\n");
    std::string whyNot("(untouched)");
    const bool armed = ht9045::PumpInit(whyNot);
    if (!armed) std::printf("   PumpInit refused: %s\n", whyNot.c_str());
    check(armed, "O2 PumpInit() succeeds with no config loaded (sim canary holds "
                 "at the static-init defaults)");
    check(whyNot.empty(), "O2 PumpInit() clears whyNot on success");
    check(ht9045::PumpActive(), "O2 PumpActive() is true after PumpInit()");

    // The three master-guard terms it must have set -- csystem.cpp:4235.
    check(SoftStop == false,      "O2 fixture set SoftStop=false");
    check(SystemStart == true,    "O2 fixture set SystemStart=true");
    check(fAllMotorHome == true,  "O2 fixture set fAllMotorHome=true");
    check(InitialOK == true,      "O2 fixture set InitialOK=true (MainProc head "
                                  "guard, csystem.cpp:3001)");

    // -------------------------------------------------------------------------
    //  O3 -- the spine ADVANCES: golden's own counter, +1 per tick.
    // -------------------------------------------------------------------------
    std::printf("\n-- O3 spine advance (golden's MainProc counter, csystem.h:52)\n");
    const unsigned int before = ht9045::PumpTelemetry().mainProcCalls;
    const int kTicks = 8;
    for (int i = 0; i < kTicks; ++i) ht9045::PumpTick();
    const ht9045::PumpStats after = ht9045::PumpTelemetry();

    std::printf("   mainProcCalls %u -> %u over %d ticks; exceptions=%lu\n",
                before, after.mainProcCalls, kTicks,
                (unsigned long)after.exceptions);
    check(after.mainProcCalls == before + (unsigned)kTicks,
          "O3 GetMainProcCallCount() advanced exactly once per PumpTick()");
    check(after.ticks == (unsigned long long)kTicks,
          "O3 telemetry tick count matches the PumpTick() calls");
    check(after.exceptions == 0,
          "O3 ZERO ticks threw out of MainProc() (a pump that throws every tick "
          "looks healthy from the browser)");
    check(after.alive, "O3 IsMainProcAlive(60) is true after pumping");

    // -------------------------------------------------------------------------
    //  O4 -- the tick oracle trace stays BOUNDED. Without csystemTraceClear() in
    //  PumpTick() this grows for the life of the process (csystem.cpp:196), which
    //  is a leak in a publisher meant to run for hours.
    // -------------------------------------------------------------------------
    std::printf("\n-- O4 trace is bounded\n");
    const std::size_t sizeAfter8 = g_csystemTrace.size();
    for (int i = 0; i < 40; ++i) ht9045::PumpTick();
    const std::size_t sizeAfter48 = g_csystemTrace.size();
    std::printf("   g_csystemTrace: %u tokens after 8 ticks, %u after 48\n",
                (unsigned)sizeAfter8, (unsigned)sizeAfter48);
    check(sizeAfter48 <= sizeAfter8 + 4,
          "O4 trace does NOT grow with tick count (PumpTick clears it each tick)");
    check(sizeAfter48 > 0,
          "O4 trace is non-empty, i.e. engines really are being called");

    // -------------------------------------------------------------------------
    //  O5 -- the published word and the cursors.
    // -------------------------------------------------------------------------
    std::printf("\n-- O5 published state\n");
    {
        ht9045::PublishHandlerTags(snap);
        const webbridge::TagSnapshotView v = snap.read();

        webbridge::TagMap::const_iterator st = v.tags.find("machine.state");
        const bool haveState = (st != v.tags.end()) && !st->second.isNull();
        check(haveState, "O5 machine.state carries a value while pumping");
        if (haveState) {
            std::printf("   machine.state = \"%s\"\n", st->second.asString().c_str());
            check(st->second.asString() == std::string("SIM RUN"),
                  "O5 machine.state reads \"SIM RUN\" with all guard terms held "
                  "(SIM because this process FORCED them -- nothing in the "
                  "translated tree can set SystemStart/fAllMotorHome)");
        }

        webbridge::TagMap::const_iterator src = v.tags.find("machine.stateSource");
        check(src != v.tags.end() && !src->second.isNull() &&
              src->second.asString().size() > 0,
              "O5 machine.stateSource states the provenance beside the word");

        check(v.tags.find("clock.text") != v.tags.end() &&
              !v.tags.find("clock.text")->second.isNull(),
              "O5 clock.text is live while pumping");

        // Raw guard terms, so the derived word can be checked against its inputs.
        check(v.tags.find("pump.guard.systemStart") != v.tags.end() &&
              v.tags.find("pump.guard.systemStart")->second.asBool() == true,
              "O5 pump.guard.systemStart publishes the raw term");
        check(v.tags.find("pump.guard.softStop") != v.tags.end() &&
              v.tags.find("pump.guard.softStop")->second.asBool() == false,
              "O5 pump.guard.softStop publishes the raw term");

        // Cursors: the evidence the engines are cycling rather than merely linked.
        const char* cursors[] = {
            "pump.task.load", "pump.task.inArm", "pump.task.outArm",
            "pump.task.sht1", "pump.task.sht2", "pump.task.testHead",
            "pump.task.catchTray"
        };
        std::size_t present = 0;
        for (std::size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
            webbridge::TagMap::const_iterator c = v.tags.find(cursors[i]);
            if (c != v.tags.end() && !c->second.isNull()) ++present;
        }
        std::printf("   engine cursors carrying a value: %u of 7\n", (unsigned)present);
        check(present == 7, "O5 all seven engine cursors publish a value while pumping");
    }

    // -------------------------------------------------------------------------
    //  O6 -- THE ANTI-LATCH CHECK. The word must be re-derived from the guard on
    //  every publish, not captured once at PumpInit. Flip one term and require the
    //  word to follow; then flip it back and require it to follow again.
    // -------------------------------------------------------------------------
    std::printf("\n-- O6 the word tracks the LIVE guard, it is not latched\n");
    SoftStop = true;                       // one of the three terms, csystem.cpp:4235
    {
        const webbridge::TagValue s = publishAndGet(snap, "machine.state");
        std::printf("   with SoftStop=true  -> \"%s\"\n",
                    s.isNull() ? "<null>" : s.asString().c_str());
        check(!s.isNull() && s.asString() == std::string("SIM HALT"),
              "O6 machine.state becomes \"SIM HALT\" the moment a guard term "
              "blocks the engines");
    }
    SoftStop = false;
    {
        const webbridge::TagValue s = publishAndGet(snap, "machine.state");
        check(!s.isNull() && s.asString() == std::string("SIM RUN"),
              "O6 machine.state returns to \"SIM RUN\" when the term clears");
    }

    // -------------------------------------------------------------------------
    //  O7 -- THE SAFETY GATE. PumpInit must REFUSE when the sim canary says the
    //  hardware interlock is live.
    //
    //  Why this is worth a test rather than a code read: the refusal condition is
    //  the logical NEGATION of the interlock's own self-disable guard
    //  (ainarm9045.cpp:1891-1893 returns early when
    //  `MOTION_CARD_TYPE!=Contec || iRealDummy!=REALLY`, so the interlock is LIVE
    //  exactly when `==Contec && ==REALLY`). An && flipped to || here would still
    //  compile, still pass every other check in this file, and would let a
    //  config-loaded pump be the first thing ever to execute CCLink/Ltc/MOT bodies
    //  offline. That is the one failure in this feature with a physical-machine
    //  consequence, so it gets its own oracle.
    // -------------------------------------------------------------------------
    std::printf("\n-- O7 PumpInit REFUSES when the interlock is live\n");
    {
        const int savedCard  = MOTION_CARD_TYPE;
        const int savedDummy = LastSet.iRealDummy;

        MOTION_CARD_TYPE     = MotionCard_Contec;   // cmydef.h:104 == 1
        LastSet.iRealDummy   = REALLY;              // cmydef.cpp:265 == 2

        std::string why("(untouched)");
        const bool ok = ht9045::PumpInit(why);
        std::printf("   PumpInit -> %s; whyNot=\"%s\"\n",
                    ok ? "true" : "false", why.c_str());
        check(!ok, "O7 PumpInit() REFUSES with MOTION_CARD_TYPE==Contec AND "
                   "iRealDummy==REALLY (the interlock at ainarm9045.cpp:1891 is live)");
        check(!why.empty(), "O7 the refusal explains itself in whyNot");

        // Only ONE of the two terms live must NOT refuse -- the interlock
        // self-disables on either, so refusing there would be a false positive that
        // makes the pump unusable on a box with a Contec card.
        LastSet.iRealDummy = savedDummy;            // back to DUMMY, card still Contec
        std::string why2("(untouched)");
        const bool ok2 = ht9045::PumpInit(why2);
        check(ok2, "O7 PumpInit() ALLOWS a Contec card when iRealDummy!=REALLY "
                   "(one term is enough to self-disable the interlock)");

        MOTION_CARD_TYPE   = savedCard;
        LastSet.iRealDummy = savedDummy;
    }

    std::printf("\ntest_wb_simpump: %d checks, %d failure(s)\n", g_total, g_fail);
    return g_fail == 0 ? 0 : 1;
}
