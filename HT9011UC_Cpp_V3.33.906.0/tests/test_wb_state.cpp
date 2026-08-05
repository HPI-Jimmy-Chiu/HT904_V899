// ===========================================================================
//  tests/test_wb_state.cpp
//  Tests for the WebBridge state seam: TagValue, TagSnapshot, CommandQueue.
//
//  AI(W906-WebBridge-State) 20260805: new file.
//
//  WHAT THIS ASSERTS
//  -----------------
//   A  TagValue: Null != "" everywhere it matters, plus every other
//      cross-type inequality the diff depends on.
//   B  diffMaps/applyPatch: the patch is EXACTLY the changed set, including
//      appear, disappear, Null -> "" and "" -> Null.
//   C  TagSnapshot single-threaded: generations, read-returns-a-copy, the
//      fast path, beginPublish clearing staging.
//   D  Publisher-thread misuse detection.
//   E  CONCURRENCY: 1 publisher + 4 reader threads, thousands of generations,
//      asserting no reader ever observes a MIXED generation (torn read).
//   F  CommandQueue single-threaded: exact bound, FIFO bulk drain, counters,
//      capacity-0 read-only mode, field round-trip.
//   G  CONCURRENCY: 4 producers + 1 drainer -- nothing lost, nothing
//      duplicated, per-producer FIFO preserved.
//   H  CONCURRENCY: bound is exact under contention, and tryPush on a FULL
//      queue with NO drainer returns immediately (never blocks).
//
//  WHY Win32 THREADS AND NOT std::thread
//  -------------------------------------
//  This tree's MinGW.org GCC 6.3.0 is built `Thread model: win32` and its
//  libstdc++ has no usable <thread>/<mutex> -- probed: `'thread' is not a
//  member of 'std'`. These are real OS threads via _beginthreadex, which is
//  what MSVC's std::thread wraps anyway. A vacuous "skip if unavailable" would
//  defeat the point of the file, so a non-Win32 build is a hard error instead.
//
//  Non-zero exit on any failure.
// ===========================================================================
#include "WebBridge/TagValue.h"
#include "WebBridge/TagSnapshot.h"
#include "WebBridge/CommandQueue.h"

#include <cstdio>
#include <cmath>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <vector>

#if !defined(_WIN32)
#  error "test_wb_state.cpp needs Win32 threads (this tree's MinGW has no std::thread). Do not silently skip the concurrency tests."
#endif
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#  define NOMINMAX
#endif
#include <windows.h>
#include <process.h>
#include <atomic>

using namespace webbridge;

// ===========================================================================
//  Tiny check harness (same shape as the rest of tests/)
// ===========================================================================
static int g_fail  = 0;
static int g_total = 0;

static void checkBool(bool ok, const char* expr, const char* file, int line)
{
    ++g_total;
    if (!ok) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n", file, line, expr);
    }
}
#define CHECK(expr) checkBool((expr) ? true : false, #expr, __FILE__, __LINE__)

static void checkSize(size_t got, size_t expect, const char* expr,
                      const char* file, int line)
{
    ++g_total;
    if (got != expect) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got %lu  expected %lu\n",
                    file, line, expr,
                    (unsigned long)got, (unsigned long)expect);
    }
}
#define CHECK_SIZE(expr, expect) checkSize((size_t)(expr), (size_t)(expect), \
                                           #expr, __FILE__, __LINE__)

static void checkU64(unsigned long long got, unsigned long long expect,
                     const char* expr, const char* file, int line)
{
    ++g_total;
    if (got != expect) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got %llu  expected %llu\n",
                    file, line, expr, got, expect);
    }
}
#define CHECK_U64(expr, expect) checkU64((unsigned long long)(expr), \
                                         (unsigned long long)(expect), \
                                         #expr, __FILE__, __LINE__)

// ===========================================================================
//  Patch inspection helpers
// ===========================================================================
static bool patchChangedHas(const TagPatch& p, const char* tag, const TagValue& v)
{
    TagMap::const_iterator it = p.changed.find(tag);
    if (it == p.changed.end()) return false;
    return it->second == v;
}

static bool patchChangedHasTag(const TagPatch& p, const char* tag)
{
    return p.changed.find(tag) != p.changed.end();
}

static bool patchRemovedHas(const TagPatch& p, const char* tag)
{
    for (size_t i = 0; i < p.removed.size(); ++i) {
        if (p.removed[i] == tag) return true;
    }
    return false;
}

// ===========================================================================
//  Thread helpers (Win32)
// ===========================================================================
static HANDLE startThread(unsigned (__stdcall *fn)(void*), void* arg)
{
    uintptr_t h = _beginthreadex(NULL, 0, fn, arg, 0, NULL);
    if (h == 0) {
        ++g_fail; ++g_total;
        std::printf("FAIL [%s:%d]  _beginthreadex returned 0 -- cannot run the "
                    "concurrency tests, so they prove nothing\n", __FILE__, __LINE__);
    }
    return (HANDLE)h;
}

static void joinThread(HANDLE h)
{
    if (h == NULL) return;
    ::WaitForSingleObject(h, INFINITE);
    ::CloseHandle(h);
}

static double elapsedMs(const LARGE_INTEGER& a, const LARGE_INTEGER& b)
{
    LARGE_INTEGER f;
    ::QueryPerformanceFrequency(&f);
    if (f.QuadPart == 0) return 0.0;
    return (double)(b.QuadPart - a.QuadPart) * 1000.0 / (double)f.QuadPart;
}

// ===========================================================================
//  A. TagValue -- Null vs "" and every other cross-type distinction
// ===========================================================================
static void testTagValue()
{
    std::printf("-- A. TagValue\n");

    const TagValue vNull  = TagValue::makeNull();
    const TagValue vEmpty = TagValue::makeString("");
    const TagValue vBlank = TagValue::makeString(" ");

    // Default construction is Null (a tag nobody set is "unknown", not "").
    CHECK(TagValue().isNull());
    CHECK(TagValue().type() == TagType::Null);

    // ---- THE load-bearing distinction: section 4 rule 3 -------------------
    CHECK(vNull != vEmpty);          // null is NOT ""
    CHECK(vEmpty != vNull);          // ... in both directions
    CHECK(!(vNull == vEmpty));
    CHECK(!(vEmpty == vNull));
    CHECK(vNull.isNull());
    CHECK(!vEmpty.isNull());
    CHECK(vEmpty.isString());
    CHECK(!vNull.isString());
    // Both render as "no text", so the ONLY way to tell them apart is the
    // type. Assert asString() cannot be used for it.
    CHECK(vNull.asString() == vEmpty.asString());   // both ""
    CHECK(vNull.type() != vEmpty.type());           // but different values

    // Same-type equality still works.
    CHECK(vNull == TagValue::makeNull());
    CHECK(vEmpty == TagValue::makeString(""));
    CHECK(vEmpty != vBlank);                        // "" is not " "

    // ---- Cross-type inequalities the diff depends on ----------------------
    CHECK(TagValue::makeBool(true)  != TagValue::makeInt(1));
    CHECK(TagValue::makeBool(false) != TagValue::makeInt(0));
    CHECK(TagValue::makeBool(false) != vNull);      // false is not "unknown"
    CHECK(TagValue::makeBool(false) != vEmpty);
    CHECK(TagValue::makeInt(0)      != vNull);      // 0 is not "unknown"
    CHECK(TagValue::makeInt(5)      != TagValue::makeDouble(5.0));
    CHECK(TagValue::makeString("0") != TagValue::makeInt(0));
    CHECK(TagValue::makeString("1") != TagValue::makeBool(true));

    // Same-type, same-payload.
    CHECK(TagValue::makeBool(true)   == TagValue::makeBool(true));
    CHECK(TagValue::makeBool(true)   != TagValue::makeBool(false));
    CHECK(TagValue::makeInt(-7)      == TagValue::makeInt(-7));
    CHECK(TagValue::makeInt(-7)      != TagValue::makeInt(7));
    CHECK(TagValue::makeDouble(130.5) == TagValue::makeDouble(130.5));
    CHECK(TagValue::makeDouble(130.5) != TagValue::makeDouble(130.6));

    // 64-bit range really is 64-bit.
    const std::int64_t big = 4294967296LL;           // 2^32, breaks a 32-bit store
    CHECK(TagValue::makeInt(big).asInt() == big);
    CHECK(TagValue::makeInt(big) != TagValue::makeInt(0));

    // ---- Double diff-stability -------------------------------------------
    const double nan = std::numeric_limits<double>::quiet_NaN();
    CHECK(nan != nan);                                        // raw IEEE
    CHECK(TagValue::makeDouble(nan) == TagValue::makeDouble(nan));  // but stable here
    CHECK(TagValue::makeDouble(nan) != TagValue::makeDouble(1.0));
    CHECK(TagValue::makeDouble(0.0) == TagValue::makeDouble(-0.0));
    const double inf = std::numeric_limits<double>::infinity();
    CHECK(TagValue::makeDouble(inf) == TagValue::makeDouble(inf));
    CHECK(TagValue::makeDouble(inf) != TagValue::makeDouble(-inf));

    // ---- Strict accessors: no silent conversion --------------------------
    CHECK(TagValue::makeInt(42).asInt() == 42);
    CHECK(TagValue::makeInt(42).asDouble(-1.0) == -1.0);   // Int is not Double
    CHECK(TagValue::makeInt(42).toNumber() == 42.0);       // ... unless asked
    CHECK(TagValue::makeDouble(1.5).toNumber() == 1.5);
    CHECK(TagValue::makeString("x").asInt(-9) == -9);
    CHECK(TagValue::makeBool(true).asBool() == true);
    CHECK(TagValue::makeString("HALT").asString() == "HALT");
    CHECK(vNull.toNumber(-3.0) == -3.0);
    CHECK(vNull.asBool(true) == true);                     // fallback honoured

    // Copy/assign preserve the alternative (the snapshot copies these a lot).
    TagValue copy = vNull;
    CHECK(copy.isNull());
    copy = vEmpty;
    CHECK(copy.isString() && copy != vNull);

    // Diagnostics are distinguishable (a log that renders both as "" would
    // make this whole class of bug invisible).
    CHECK(std::string(vNull.typeName()) == "Null");
    CHECK(std::string(vEmpty.typeName()) == "String");
    CHECK(vNull.debugString() != vEmpty.debugString());
}

// ===========================================================================
//  B. diffMaps / applyPatch -- exactly the changed set
// ===========================================================================
static void testDiff()
{
    std::printf("-- B. diffMaps / applyPatch\n");

    // ---- ordinary change + appear + disappear ----------------------------
    TagMap from;
    from["machine.state"] = TagValue::makeString("HALT");
    from["temp.pv"]       = TagValue::makeInt(130);
    from["tower.red"]     = TagValue::makeInt(1);
    from["zone.gone"]     = TagValue::makeInt(7);

    TagMap to;
    to["machine.state"] = TagValue::makeString("RUN");     // changed
    to["temp.pv"]       = TagValue::makeInt(130);          // unchanged
    to["tower.red"]     = TagValue::makeInt(1);            // unchanged
    to["arm.A1"]        = TagValue::makeDouble(11.0);      // appeared
    // "zone.gone" is absent                              -> disappeared

    TagPatch p = diffMaps(from, to);

    // EXACTLY the changed set: 2 changed, 1 removed, and nothing else.
    CHECK_SIZE(p.changed.size(), 2);
    CHECK_SIZE(p.removed.size(), 1);
    CHECK(patchChangedHas(p, "machine.state", TagValue::makeString("RUN")));
    CHECK(patchChangedHas(p, "arm.A1", TagValue::makeDouble(11.0)));
    CHECK(!patchChangedHasTag(p, "temp.pv"));      // unchanged must NOT be sent
    CHECK(!patchChangedHasTag(p, "tower.red"));
    CHECK(patchRemovedHas(p, "zone.gone"));
    CHECK(!patchChangedHasTag(p, "zone.gone"));    // removal is not a "change"

    // applyPatch reconstructs `to` exactly -- so a connection that only ever
    // receives patches ends up agreeing with the handler.
    TagMap rebuilt = from;
    applyPatch(rebuilt, p);
    CHECK_SIZE(rebuilt.size(), to.size());
    CHECK(rebuilt == to);                          // std::map ==, elementwise

    // ---- identical state produces NOTHING --------------------------------
    TagPatch none = diffMaps(to, to);
    CHECK(none.empty());
    CHECK_SIZE(none.size(), 0);

    // ---- Null -> "" is a change ------------------------------------------
    {
        TagMap a; a["speed.index.acc"] = TagValue::makeNull();
        TagMap b; b["speed.index.acc"] = TagValue::makeString("");
        TagPatch q = diffMaps(a, b);
        CHECK_SIZE(q.changed.size(), 1);
        CHECK_SIZE(q.removed.size(), 0);
        CHECK(patchChangedHas(q, "speed.index.acc", TagValue::makeString("")));
        CHECK(q.changed["speed.index.acc"].isString());   // and it is "", not null
    }

    // ---- "" -> Null is a change too --------------------------------------
    {
        TagMap a; a["speed.index.acc"] = TagValue::makeString("");
        TagMap b; b["speed.index.acc"] = TagValue::makeNull();
        TagPatch q = diffMaps(a, b);
        CHECK_SIZE(q.changed.size(), 1);
        CHECK_SIZE(q.removed.size(), 0);
        CHECK(patchChangedHas(q, "speed.index.acc", TagValue::makeNull()));
        CHECK(q.changed["speed.index.acc"].isNull());
    }

    // ---- absent -> Null is an APPEAR, not a no-op ------------------------
    //  "not in the map" and "in the map with value null" are different facts.
    {
        TagMap a;
        TagMap b; b["zone.heatgun.1"] = TagValue::makeNull();
        TagPatch q = diffMaps(a, b);
        CHECK_SIZE(q.changed.size(), 1);
        CHECK(patchChangedHasTag(q, "zone.heatgun.1"));
        CHECK(q.changed["zone.heatgun.1"].isNull());
        CHECK_SIZE(q.removed.size(), 0);
    }

    // ---- Null -> absent is a REMOVE, not a no-op -------------------------
    {
        TagMap a; a["zone.heatgun.1"] = TagValue::makeNull();
        TagMap b;
        TagPatch q = diffMaps(a, b);
        CHECK_SIZE(q.changed.size(), 0);
        CHECK_SIZE(q.removed.size(), 1);
        CHECK(patchRemovedHas(q, "zone.heatgun.1"));
    }

    // ---- everything disappears ------------------------------------------
    {
        TagPatch q = diffMaps(from, TagMap());
        CHECK_SIZE(q.changed.size(), 0);
        CHECK_SIZE(q.removed.size(), from.size());
    }

    // ---- first frame: empty -> full is all-changed, nothing removed ------
    {
        TagPatch q = diffMaps(TagMap(), from);
        CHECK_SIZE(q.changed.size(), from.size());
        CHECK_SIZE(q.removed.size(), 0);
    }

    // ---- a type flip is a change even when it "looks" the same -----------
    {
        TagMap a; a["temp.pv"] = TagValue::makeInt(130);
        TagMap b; b["temp.pv"] = TagValue::makeDouble(130.0);
        TagPatch q = diffMaps(a, b);
        CHECK_SIZE(q.changed.size(), 1);
    }
}

// ===========================================================================
//  C. TagSnapshot -- single-threaded semantics
// ===========================================================================
static void testSnapshotBasics()
{
    std::printf("-- C. TagSnapshot basics\n");

    TagSnapshot snap;

    // Nothing published yet: generation 0, no tags.
    CHECK_U64(snap.generation(), 0);
    CHECK_SIZE(snap.publishedTagCount(), 0);
    CHECK(snap.read().tags.empty());
    CHECK_U64(snap.read().generation, 0);

    // ---- publish() ------------------------------------------------------
    TagMap t1;
    t1["machine.state"] = TagValue::makeString("HALT");
    t1["temp.pv"]       = TagValue::makeInt(130);
    t1["zone.heatgun.1"] = TagValue::makeNull();

    CHECK_U64(snap.publish(t1), 1);            // first publish is generation 1
    CHECK_U64(snap.generation(), 1);
    CHECK_SIZE(snap.publishedTagCount(), 3);

    TagSnapshotView v1 = snap.read();
    CHECK_U64(v1.generation, 1);
    CHECK_SIZE(v1.tags.size(), 3);
    CHECK(v1.tags["machine.state"] == TagValue::makeString("HALT"));
    CHECK(v1.tags["zone.heatgun.1"].isNull());

    // ---- read() hands back a COPY, not a window into the live buffer ----
    v1.tags["machine.state"] = TagValue::makeString("TAMPERED");
    v1.tags["injected"]      = TagValue::makeInt(1);
    TagSnapshotView v1b = snap.read();
    CHECK(v1b.tags["machine.state"] == TagValue::makeString("HALT"));
    CHECK(v1b.tags.find("injected") == v1b.tags.end());
    CHECK_SIZE(snap.publishedTagCount(), 3);

    // ---- generation advances monotonically ------------------------------
    CHECK_U64(snap.publish(t1), 2);
    CHECK_U64(snap.publish(t1), 3);
    CHECK_U64(snap.generation(), 3);

    // ---- diffFrom fast path: same generation -> empty patch -------------
    TagSnapshotView cur = snap.read();
    TagPatch fast = snap.diffFrom(cur);
    CHECK(fast.empty());
    CHECK_U64(fast.generation, cur.generation);

    // ---- diffFrom after a real change ----------------------------------
    TagMap t2 = t1;
    t2["machine.state"]  = TagValue::makeString("RUN");   // changed
    t2["zone.heatgun.1"] = TagValue::makeString("");      // Null -> "" !
    t2.erase("temp.pv");                                  // disappears
    t2["arm.A1"] = TagValue::makeDouble(11.0);            // appears
    const std::uint64_t gen4 = snap.publish(t2);
    CHECK_U64(gen4, 4);

    TagPatch p = snap.diffFrom(cur);
    CHECK_U64(p.generation, 4);
    CHECK_SIZE(p.changed.size(), 3);       // state, heatgun (Null->""), arm.A1
    CHECK_SIZE(p.removed.size(), 1);       // temp.pv
    CHECK(patchChangedHas(p, "machine.state", TagValue::makeString("RUN")));
    CHECK(patchChangedHas(p, "zone.heatgun.1", TagValue::makeString("")));
    CHECK(patchChangedHas(p, "arm.A1", TagValue::makeDouble(11.0)));
    CHECK(patchRemovedHas(p, "temp.pv"));

    // Applying it to the old view reproduces the new state exactly.
    applyPatch(cur.tags, p);
    cur.generation = p.generation;
    CHECK(cur.tags == snap.read().tags);

    // ---- the TagMap overload has no fast path but the same semantics ----
    TagMap lastSent = snap.read().tags;
    CHECK(snap.diffFrom(lastSent).empty());
    lastSent.erase("arm.A1");
    CHECK_SIZE(snap.diffFrom(lastSent).changed.size(), 1);

    // ---- stage/commit, and beginPublish clearing staging ---------------
    TagSnapshot s2;
    s2.beginPublish();
    s2.stage("a", TagValue::makeInt(1));
    s2.stage("b", TagValue::makeInt(2));
    CHECK_SIZE(s2.stagedTagCount(), 2);
    CHECK_U64(s2.generation(), 0);              // not visible until commit
    CHECK_SIZE(s2.publishedTagCount(), 0);
    CHECK_U64(s2.commitPublish(), 1);
    CHECK_SIZE(s2.publishedTagCount(), 2);

    s2.beginPublish();                          // must CLEAR, not merge
    CHECK_SIZE(s2.stagedTagCount(), 0);
    s2.stage("b", TagValue::makeInt(99));
    CHECK_U64(s2.commitPublish(), 2);
    TagSnapshotView v2 = s2.read();
    CHECK_SIZE(v2.tags.size(), 1);              // "a" is gone, as it must be
    CHECK(v2.tags.find("a") == v2.tags.end());
    CHECK(v2.tags["b"] == TagValue::makeInt(99));

    // Publishing an empty set is legal: everything goes absent.
    CHECK_U64(s2.publish(TagMap()), 3);
    CHECK_SIZE(s2.publishedTagCount(), 0);
    CHECK_SIZE(s2.diffFrom(v2).removed.size(), 1);
}

// ===========================================================================
//  D. Publisher-thread misuse detection
// ===========================================================================
struct PublishOnceArg {
    TagSnapshot* snap;
    unsigned long tid;
};

static unsigned __stdcall publishOnceThread(void* raw)
{
    PublishOnceArg* a = (PublishOnceArg*)raw;
    a->tid = (unsigned long)::GetCurrentThreadId();
    TagMap t;
    t["from.thread"] = TagValue::makeInt(1);
    a->snap->publish(t);
    return 0;
}

static void testPublisherThreadGuard()
{
    std::printf("-- D. publisher-thread guard\n");

    // Case 1: main thread is the publisher; a second thread publishing is a
    // contract violation and must be COUNTED (not ignored, not fatal).
    {
        TagSnapshot snap;
        TagMap t; t["x"] = TagValue::makeInt(1);
        snap.publish(t);
        CHECK_U64(snap.publisherViolations(), 0);
        CHECK_U64(snap.publisherThreadId(), (unsigned long)::GetCurrentThreadId());

        PublishOnceArg arg; arg.snap = &snap; arg.tid = 0;
        joinThread(startThread(publishOnceThread, &arg));

        CHECK(snap.publisherViolations() > 0);
        // The publisher slot still names the FIRST thread.
        CHECK_U64(snap.publisherThreadId(), (unsigned long)::GetCurrentThreadId());
        // ... and it did not lose the data or crash: detection, not protection.
        CHECK_SIZE(snap.publishedTagCount(), 1);
    }

    // Case 2: a non-main thread may be the publisher, as long as it is the
    // ONLY one. (The real bridge publishes from the VCL/MFC UI thread, which
    // is not necessarily where a test's main() runs.)
    {
        TagSnapshot snap;
        PublishOnceArg arg; arg.snap = &snap; arg.tid = 0;
        joinThread(startThread(publishOnceThread, &arg));
        CHECK_U64(snap.publisherViolations(), 0);
        CHECK_U64(snap.publisherThreadId(), arg.tid);
        CHECK(arg.tid != (unsigned long)::GetCurrentThreadId());
        // Reading from another thread is always fine and must NOT count.
        CHECK_SIZE(snap.read().tags.size(), 1);
        CHECK_U64(snap.publisherViolations(), 0);
    }
}

// ===========================================================================
//  E. CONCURRENCY -- torn-read hunt
//
//  One publisher thread stamps EVERY tag with the generation it belongs to.
//  A reader that ever observes two different stamps in one view, or a stamp
//  that disagrees with the generation it was handed, has seen a torn read.
//  That is the failure the mutex + swap exists to prevent, and it is the one
//  that would put wrong numbers on an operator's screen.
// ===========================================================================
static const int kTagCount   = 40;
static const int kGenerations = 2000;

static std::atomic<unsigned long> g_stop(0);

static std::string tagName(int i)
{
    char buf[32];
    std::sprintf(buf, "wb.tag.%02d", i);
    return std::string(buf);
}

struct PublisherArg {
    TagSnapshot* snap;
    unsigned long tid;
};

static unsigned __stdcall publisherThread(void* raw)
{
    PublisherArg* a = (PublisherArg*)raw;
    a->tid = (unsigned long)::GetCurrentThreadId();

    // Pre-build the names so the publish loop is not dominated by sprintf.
    std::vector<std::string> names;
    for (int i = 0; i < kTagCount; ++i) names.push_back(tagName(i));

    for (int gen = 1; gen <= kGenerations; ++gen) {
        a->snap->beginPublish();
        for (int i = 0; i < kTagCount - 1; ++i) {
            // Every value carries the generation it belongs to.
            a->snap->stage(names[i], TagValue::makeInt(gen));
        }
        // One String tag as well, so a torn read cannot hide behind all tags
        // having the same representation.
        a->snap->stage(names[kTagCount - 1],
                       TagValue::makeString("gen" + std::to_string(gen)));
        a->snap->commitPublish();

        // Yield periodically so the readers are guaranteed real scheduling and
        // this test cannot degenerate into "publisher finished before any read".
        if ((gen % 200) == 0) ::Sleep(1);
    }
    g_stop.store(1UL);
    return 0;
}

struct ReaderStats {
    TagSnapshot*  snap;
    unsigned long reads;
    unsigned long tornViews;        // a view mixing two generations
    unsigned long stampMismatch;    // value stamp != reported generation
    unsigned long wrongTagCount;    // a partially-published map
    unsigned long generationWentBackwards;
    unsigned long distinctGens;
    unsigned long patchesApplied;
    unsigned long patchStampMismatch;
    unsigned long patchHadRemovals;
};

static unsigned __stdcall readerThread(void* raw)
{
    ReaderStats* st = (ReaderStats*)raw;
    unsigned long long lastGen = 0;
    std::set<unsigned long long> seen;

    // Per-connection last-sent state, exactly as a real WebSocket connection
    // would keep it.
    TagSnapshotView lastSent;

    while (g_stop.load() == 0UL) {
        // ---------------- read path: full snapshot ----------------------
        TagSnapshotView v = st->snap->read();
        ++st->reads;

        if (v.generation != 0) {
            if (v.tags.size() != (size_t)kTagCount) ++st->wrongTagCount;

            // Every tag must carry v.generation, and nothing else.
            std::set<unsigned long long> stamps;
            TagMap::const_iterator it = v.tags.begin();
            for (; it != v.tags.end(); ++it) {
                if (it->second.isInt()) {
                    stamps.insert((unsigned long long)it->second.asInt());
                } else if (it->second.isString()) {
                    const std::string& s = it->second.asString();
                    if (s.compare(0, 3, "gen") == 0) {
                        stamps.insert(strtoull(s.c_str() + 3, NULL, 10));
                    } else {
                        ++st->stampMismatch;   // not even the right shape
                    }
                } else {
                    ++st->stampMismatch;
                }
            }
            if (stamps.size() > 1) ++st->tornViews;
            if (stamps.size() == 1 && *stamps.begin() != v.generation) {
                ++st->stampMismatch;
            }

            if (v.generation < lastGen) ++st->generationWentBackwards;
            lastGen = v.generation;
            seen.insert(v.generation);
        }

        // ---------------- diff path: what actually goes on the wire -----
        TagPatch p = st->snap->diffFrom(lastSent);
        if (!p.empty()) {
            ++st->patchesApplied;
            // Tags never disappear in this test, so any removal is a bug.
            if (!p.removed.empty()) ++st->patchHadRemovals;
            // Every value in the patch must belong to p.generation.
            TagMap::const_iterator ci = p.changed.begin();
            for (; ci != p.changed.end(); ++ci) {
                if (ci->second.isInt()) {
                    if ((unsigned long long)ci->second.asInt() != p.generation) {
                        ++st->patchStampMismatch;
                    }
                } else if (ci->second.isString()) {
                    if (ci->second.asString() != ("gen" + std::to_string(p.generation))) {
                        ++st->patchStampMismatch;
                    }
                } else {
                    ++st->patchStampMismatch;
                }
            }
            applyPatch(lastSent.tags, p);
            lastSent.generation = p.generation;
        }
    }
    st->distinctGens = (unsigned long)seen.size();
    return 0;
}

static void testSnapshotConcurrency()
{
    std::printf("-- E. TagSnapshot under concurrent load "
                "(1 publisher + 4 readers, %d generations x %d tags)\n",
                kGenerations, kTagCount);

    TagSnapshot snap;
    g_stop.store(0UL);

    const int kReaders = 4;
    ReaderStats stats[kReaders];
    HANDLE readers[kReaders];
    for (int i = 0; i < kReaders; ++i) {
        ReaderStats z; z.snap = &snap;
        z.reads = 0; z.tornViews = 0; z.stampMismatch = 0; z.wrongTagCount = 0;
        z.generationWentBackwards = 0; z.distinctGens = 0;
        z.patchesApplied = 0; z.patchStampMismatch = 0; z.patchHadRemovals = 0;
        stats[i] = z;
        readers[i] = startThread(readerThread, &stats[i]);
    }

    PublisherArg parg; parg.snap = &snap; parg.tid = 0;
    HANDLE pub = startThread(publisherThread, &parg);

    joinThread(pub);
    for (int i = 0; i < kReaders; ++i) joinThread(readers[i]);

    CHECK_U64(snap.generation(), (unsigned long long)kGenerations);
    // Only the publisher thread ever used the publish path.
    CHECK_U64(snap.publisherViolations(), 0);
    CHECK_U64(snap.publisherThreadId(), parg.tid);

    unsigned long totalReads = 0, totalPatches = 0;
    for (int i = 0; i < kReaders; ++i) {
        const ReaderStats& s = stats[i];
        std::printf("   reader %d: %lu reads, %lu distinct generations, "
                    "%lu patches\n", i, s.reads, s.distinctGens, s.patchesApplied);

        // THE assertions this whole section exists for.
        CHECK_U64(s.tornViews, 0);                 // no mixed generation, ever
        CHECK_U64(s.stampMismatch, 0);             // view matches its own gen
        CHECK_U64(s.wrongTagCount, 0);             // no partially-filled map
        CHECK_U64(s.generationWentBackwards, 0);   // generations only advance
        CHECK_U64(s.patchStampMismatch, 0);        // patches are coherent too
        CHECK_U64(s.patchHadRemovals, 0);

        // Guard against a vacuous pass: a reader that never ran, or that only
        // ever saw one generation, proves nothing about concurrency.
        CHECK(s.reads > 50);
        CHECK(s.distinctGens >= 2);
        CHECK(s.patchesApplied >= 2);

        totalReads   += s.reads;
        totalPatches += s.patchesApplied;
    }
    CHECK(totalReads > 1000);
    CHECK(totalPatches > 100);
}

// ===========================================================================
//  F. CommandQueue -- single-threaded semantics
// ===========================================================================
static WebCommand makeCmd(std::int64_t id, const char* name)
{
    WebCommand c;
    c.id  = id;
    c.cmd = name;
    return c;
}

static void testCommandQueueBasics()
{
    std::printf("-- F. CommandQueue basics\n");

    CommandQueue q(4);
    CHECK_SIZE(q.capacity(), 4);
    CHECK(q.empty());
    CHECK_U64(q.acceptedCount(), 0);
    CHECK_U64(q.rejectedCount(), 0);

    // ---- the bound is EXACT ---------------------------------------------
    CHECK(q.tryPush(makeCmd(1, "run.start")));
    CHECK(q.tryPush(makeCmd(2, "run.stop")));
    CHECK(q.tryPush(makeCmd(3, "sort.clear")));
    CHECK(q.tryPush(makeCmd(4, "run.setOffLine")));
    CHECK_SIZE(q.size(), 4);
    CHECK(!q.tryPush(makeCmd(5, "one.too.many")));   // rejected, not queued
    CHECK(!q.tryPush(makeCmd(6, "also.rejected")));
    CHECK_SIZE(q.size(), 4);                         // and the queue is intact
    CHECK_U64(q.acceptedCount(), 4);
    CHECK_U64(q.rejectedCount(), 2);                 // == ok:false acks owed
    CHECK_SIZE(q.peakSize(), 4);

    // ---- drain is bulk, FIFO, and APPENDS -------------------------------
    std::vector<WebCommand> out;
    out.push_back(makeCmd(999, "pre.existing"));     // must be preserved
    CHECK_SIZE(q.drain(out), 4);
    CHECK_SIZE(out.size(), 5);
    CHECK_U64(out[0].id, 999);
    CHECK_U64(out[1].id, 1);
    CHECK_U64(out[2].id, 2);
    CHECK_U64(out[3].id, 3);
    CHECK_U64(out[4].id, 4);                         // oldest first, in order
    CHECK(out[1].cmd == "run.start");
    CHECK(out[4].cmd == "run.setOffLine");
    CHECK(q.empty());
    CHECK_SIZE(q.size(), 0);
    CHECK_U64(q.drainedCount(), 4);

    // Draining an empty queue is a no-op, not an error.
    CHECK_SIZE(q.drain(out), 0);
    CHECK_SIZE(out.size(), 5);

    // ---- capacity frees up again after a drain -------------------------
    CHECK(q.tryPush(makeCmd(7, "again")));
    CHECK_SIZE(q.size(), 1);
    CHECK_U64(q.acceptedCount(), 5);

    // ---- every field survives the crossing -----------------------------
    {
        CommandQueue q2(2);
        WebCommand c;
        c.id       = 42;
        c.cmd      = "temp.setSV";
        c.tag      = "temp.sv";
        c.hasTag   = true;
        c.value    = TagValue::makeString("135");
        c.hasValue = true;
        c.connId   = 7ULL;
        CHECK(q2.tryPush(c));

        // A command with NO tag and NO value (e.g. run.setOffLine) must keep
        // "absent" distinguishable from "present and empty".
        WebCommand bare = makeCmd(43, "run.setOffLine");
        CHECK(!bare.hasTag);
        CHECK(!bare.hasValue);
        CHECK(bare.value.isNull());
        CHECK(q2.tryPush(bare));

        std::vector<WebCommand> got;
        CHECK_SIZE(q2.drain(got), 2);
        CHECK_U64(got[0].id, 42);
        CHECK(got[0].cmd == "temp.setSV");
        CHECK(got[0].tag == "temp.sv");
        CHECK(got[0].hasTag);
        CHECK(got[0].hasValue);
        CHECK(got[0].value == TagValue::makeString("135"));
        CHECK_U64(got[0].connId, 7);
        CHECK(!got[1].hasTag);
        CHECK(!got[1].hasValue);
        CHECK(got[1].tag.empty());

        // A command whose value is explicitly Null is NOT the same as one with
        // no value at all -- same rule as tag values (section 4 rule 3).
        WebCommand explicitNull = makeCmd(44, "zone.clear");
        explicitNull.hasValue = true;
        explicitNull.value    = TagValue::makeNull();
        CHECK(explicitNull.hasValue && explicitNull.value.isNull());
        CHECK(bare.hasValue != explicitNull.hasValue);
    }

    // ---- capacity 0 == read-only bridge (section 6, question 3) ---------
    {
        CommandQueue ro(0);
        CHECK_SIZE(ro.capacity(), 0);
        CHECK(!ro.tryPush(makeCmd(1, "run.start")));
        CHECK(!ro.tryPush(makeCmd(2, "run.start")));
        CHECK_SIZE(ro.size(), 0);
        CHECK_U64(ro.acceptedCount(), 0);
        CHECK_U64(ro.rejectedCount(), 2);      // explicit refusals, not silence
        std::vector<WebCommand> nothing;
        CHECK_SIZE(ro.drain(nothing), 0);
    }

    // ---- default capacity is the documented one ------------------------
    {
        CommandQueue def;
        CHECK_SIZE(def.capacity(), CommandQueue::kDefaultCapacity);
    }
}

// ===========================================================================
//  G. CONCURRENCY -- producers vs drainer: nothing lost, nothing duplicated
// ===========================================================================
static const int kProducers        = 4;
static const int kPushesPerProducer = 2000;
static const std::int64_t kIdStride = 1000000;

struct ProducerArg {
    CommandQueue* q;
    int           index;
    unsigned long accepted;
    unsigned long rejected;
    std::int64_t  lastAcceptedSeq;   // to prove per-producer ordering later
};

static unsigned __stdcall producerThread(void* raw)
{
    ProducerArg* a = (ProducerArg*)raw;
    for (int i = 0; i < kPushesPerProducer; ++i) {
        WebCommand c;
        c.id     = (std::int64_t)a->index * kIdStride + i;
        c.cmd    = "temp.setSV";
        c.connId = (std::uint64_t)a->index;
        if (a->q->tryPush(c)) ++a->accepted;
        else                  ++a->rejected;
    }
    return 0;
}

struct DrainerArg {
    CommandQueue*            q;
    std::vector<WebCommand>* out;
    unsigned long            drains;
};

static std::atomic<unsigned long> g_producersDone(0);

static unsigned __stdcall drainerThread(void* raw)
{
    DrainerArg* a = (DrainerArg*)raw;
    // Keep draining until the producers are finished AND the queue is empty.
    for (;;) {
        const bool done = (g_producersDone.load() == (unsigned long)kProducers);
        const size_t n = a->q->drain(*a->out);
        if (n > 0) ++a->drains;
        if (done && n == 0 && a->q->empty()) break;
        if (n == 0) ::Sleep(0);
    }
    return 0;
}

static void testQueueConcurrency()
{
    std::printf("-- G. CommandQueue under concurrent load "
                "(%d producers x %d pushes + 1 drainer)\n",
                kProducers, kPushesPerProducer);

    // Bound deliberately smaller than the total traffic so rejections happen
    // for real and the accounting has to hold with both outcomes in play.
    CommandQueue q(128);
    std::vector<WebCommand> drained;
    g_producersDone.store(0UL);

    DrainerArg darg; darg.q = &q; darg.out = &drained; darg.drains = 0;
    HANDLE drainer = startThread(drainerThread, &darg);

    ProducerArg pargs[kProducers];
    HANDLE prods[kProducers];
    for (int i = 0; i < kProducers; ++i) {
        pargs[i].q = &q; pargs[i].index = i;
        pargs[i].accepted = 0; pargs[i].rejected = 0; pargs[i].lastAcceptedSeq = -1;
        prods[i] = startThread(producerThread, &pargs[i]);
    }
    for (int i = 0; i < kProducers; ++i) {
        joinThread(prods[i]);
        g_producersDone.fetch_add(1UL);
    }
    joinThread(drainer);

    unsigned long accepted = 0, rejected = 0;
    for (int i = 0; i < kProducers; ++i) {
        accepted += pargs[i].accepted;
        rejected += pargs[i].rejected;
    }
    std::printf("   accepted=%lu rejected=%lu drained=%lu drains=%lu\n",
                accepted, rejected, (unsigned long)drained.size(), darg.drains);

    // Every push had exactly one outcome.
    CHECK_U64(accepted + rejected,
              (unsigned long long)kProducers * kPushesPerProducer);
    // Nothing accepted was lost, and nothing was invented.
    CHECK_U64(q.acceptedCount(), accepted);
    CHECK_U64(q.rejectedCount(), rejected);
    CHECK_U64(drained.size(), accepted);
    CHECK_U64(q.drainedCount(), accepted);
    CHECK(q.empty());
    CHECK(q.peakSize() <= q.capacity());

    // No duplicates, and every id is one that was actually pushed.
    std::set<std::int64_t> ids;
    unsigned long dupes = 0, bogus = 0, outOfOrder = 0;
    std::vector<std::int64_t> lastSeqPerProducer(kProducers, -1);
    for (size_t i = 0; i < drained.size(); ++i) {
        const std::int64_t id = drained[i].id;
        if (!ids.insert(id).second) ++dupes;
        const int p = (int)(id / kIdStride);
        const std::int64_t seq = id % kIdStride;
        if (p < 0 || p >= kProducers || seq < 0 || seq >= kPushesPerProducer) {
            ++bogus;
            continue;
        }
        // FIFO is only meaningful PER producer (there is no global order among
        // concurrent pushers), and it must hold strictly.
        if (seq <= lastSeqPerProducer[p]) ++outOfOrder;
        lastSeqPerProducer[p] = seq;
        // The payload must not have been shuffled between commands.
        if (drained[i].connId != (std::uint64_t)p) ++bogus;
    }
    CHECK_U64(dupes, 0);
    CHECK_U64(bogus, 0);
    CHECK_U64(outOfOrder, 0);        // per-producer FIFO preserved end to end

    // Guard against a vacuous pass.
    CHECK(accepted > 100);
    CHECK(darg.drains > 5);
}

// ===========================================================================
//  H. CONCURRENCY -- a FULL queue with a STALLED drainer must not block
//
//  This is the production scenario the bound exists for: the UI thread is not
//  draining (SECS/GEM reply, alarm dialog, motion sequence) and browsers keep
//  sending. tryPush must return false straight away, the bound must hold
//  exactly, and no socket thread may be parked.
// ===========================================================================
static const int kFloodPerThread = 20000;

struct FloodArg {
    CommandQueue* q;
    unsigned long accepted;
    unsigned long rejected;
    double        worstPushMs;
};

static unsigned __stdcall floodThread(void* raw)
{
    FloodArg* a = (FloodArg*)raw;
    for (int i = 0; i < kFloodPerThread; ++i) {
        LARGE_INTEGER t0, t1;
        ::QueryPerformanceCounter(&t0);
        const bool ok = a->q->tryPush(makeCmd(i, "flood"));
        ::QueryPerformanceCounter(&t1);
        const double ms = elapsedMs(t0, t1);
        if (ms > a->worstPushMs) a->worstPushMs = ms;
        if (ok) ++a->accepted; else ++a->rejected;
    }
    return 0;
}

static void testPushNeverBlocks()
{
    std::printf("-- H. tryPush on a full queue with NO drainer\n");

    const size_t cap = 8;
    CommandQueue q(cap);

    const int kFlooders = 4;
    FloodArg fargs[kFlooders];
    HANDLE hs[kFlooders];

    LARGE_INTEGER t0, t1;
    ::QueryPerformanceCounter(&t0);
    for (int i = 0; i < kFlooders; ++i) {
        fargs[i].q = &q; fargs[i].accepted = 0; fargs[i].rejected = 0;
        fargs[i].worstPushMs = 0.0;
        hs[i] = startThread(floodThread, &fargs[i]);
    }
    for (int i = 0; i < kFlooders; ++i) joinThread(hs[i]);
    ::QueryPerformanceCounter(&t1);

    const double totalMs = elapsedMs(t0, t1);
    unsigned long accepted = 0, rejected = 0;
    double worst = 0.0;
    for (int i = 0; i < kFlooders; ++i) {
        accepted += fargs[i].accepted;
        rejected += fargs[i].rejected;
        if (fargs[i].worstPushMs > worst) worst = fargs[i].worstPushMs;
    }
    std::printf("   %d pushes in %.1f ms; accepted=%lu rejected=%lu "
                "worst single push=%.3f ms\n",
                kFlooders * kFloodPerThread, totalMs, accepted, rejected, worst);

    // The bound is EXACT even with four threads racing at it: exactly `cap`
    // commands are accepted and every one of the rest is refused.
    CHECK_U64(accepted, (unsigned long long)cap);
    CHECK_U64(rejected, (unsigned long long)kFlooders * kFloodPerThread - cap);
    CHECK_SIZE(q.size(), cap);
    CHECK_SIZE(q.peakSize(), cap);
    CHECK_U64(q.acceptedCount(), (unsigned long long)cap);

    // Nothing waited on the (permanently stalled) drainer. Thresholds are
    // deliberately loose -- this catches "somebody added a wait/sleep/retry
    // loop to tryPush", not microsecond regressions.
    CHECK(totalMs < 5000.0);
    CHECK(worst   < 250.0);

    // Sanity: the 8 that made it are the FIRST 8 offered, still intact.
    std::vector<WebCommand> out;
    CHECK_SIZE(q.drain(out), cap);
    CHECK_SIZE(out.size(), cap);
    for (size_t i = 0; i < out.size(); ++i) {
        CHECK(out[i].cmd == "flood");
    }
}

// ===========================================================================
int main()
{
    std::printf("=== test_wb_state: WebBridge state seam ===\n");

    testTagValue();
    testDiff();
    testSnapshotBasics();
    testPublisherThreadGuard();
    testSnapshotConcurrency();
    testCommandQueueBasics();
    testQueueConcurrency();
    testPushNeverBlocks();

    std::printf("=== %d checks, %d failed ===\n", g_total, g_fail);
    if (g_fail != 0) {
        std::printf("RESULT: FAIL\n");
        return 1;
    }
    std::printf("RESULT: PASS\n");
    return 0;
}
