// ===========================================================================
//  tests/test_wb_tcplink.cpp
//  Tests for the sidecar seam: WebBridge/TagJson (tag <-> JSON, both ways) and
//  WebBridge/TcpTagLink (loopback publisher + reconnecting consumer).
//
//  AI(W906-WebBridge-Tcp) 20260812: new file.
//
//  WHAT THIS ASSERTS
//  -----------------
//   A  TagJson round trip preserves every TagValue type, and -- the one that
//      actually bites -- keeps Null and "" distinct. Collapsing them prints
//      0.00 for an uninstalled heater zone instead of "---".
//   B  The documented lossy edge is PINNED, not merely tolerated: an integral
//      Double decodes as Int. If someone "fixes" that silently, this fails and
//      they have to read TagJson.h and decide on purpose.
//   C  Decode is all-or-nothing. A frame with an array value must not
//      half-apply, because on the real link the target is a live snapshot.
//   D  A real socket round trip on an ephemeral loopback port: hello +
//      snapshot on connect, a patch when a tag changes, and a removal actually
//      removing. This is the end-to-end contract wb_publish/wb_gateway rely on.
//   E  The consumer survives the publisher going away and coming back. The
//      gateway is expected to outlive handler restarts, so reconnect is a
//      feature, not an accident.
//
//  Ports: every publisher here binds port 0 (ephemeral) and the test reads
//  BoundPort() back. A test that hard-codes a port fails on whatever machine
//  happens to be using it, which is a bug in the test, not the code.
// ===========================================================================
#include "WebBridge/TagJson.h"
#include "WebBridge/TagSnapshot.h"
#include "WebBridge/TagValue.h"
#include "WebBridge/TcpTagLink.h"

#include <windows.h>

#include <cstdio>
#include <string>

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

// Poll `cond` until true or the budget runs out. Returns whether it came true.
// Sockets and threads make "wait for it" unavoidable; what is avoidable is a
// fixed Sleep long enough to be slow AND short enough to be flaky.
template <typename Fn>
static bool waitFor(Fn cond, int budgetMs)
{
    for (int waited = 0; waited < budgetMs; waited += 25) {
        if (cond()) return true;
        ::Sleep(25);
    }
    return cond();
}

// ===========================================================================
//  A/B/C -- TagJson
// ===========================================================================
static void testJsonRoundTrip()
{
    TagMap in;
    in["a.null"]   = TagValue::makeNull();
    in["a.empty"]  = TagValue::makeString("");
    in["a.text"]   = TagValue::makeString("HT-9046LS");
    in["a.quote"]  = TagValue::makeString("he said \"hi\"\n\tand left");
    in["a.true"]   = TagValue::makeBool(true);
    in["a.false"]  = TagValue::makeBool(false);
    in["a.int"]    = TagValue::makeInt(957);
    in["a.neg"]    = TagValue::makeInt(-42);
    in["a.double"] = TagValue::makeDouble(25.5);

    const std::string json = EncodeTagObject(in);

    TagMap out;
    std::string err;
    CHECK(DecodeTagObject(json, out, err));
    CHECK(err.empty());
    CHECK(out.size() == in.size());

    // A -- the Null / "" distinction, the one that misreports hardware.
    CHECK(out["a.null"].isNull());
    CHECK(out["a.empty"].isString());
    CHECK(out["a.empty"].asString() == "");
    CHECK(!(out["a.null"] == out["a.empty"]));

    CHECK(out["a.text"].asString() == "HT-9046LS");
    CHECK(out["a.quote"].asString() == "he said \"hi\"\n\tand left");
    CHECK(out["a.true"].isBool()  && out["a.true"].asBool());
    CHECK(out["a.false"].isBool() && !out["a.false"].asBool());
    CHECK(out["a.int"].isInt()    && out["a.int"].asInt() == 957);
    CHECK(out["a.neg"].isInt()    && out["a.neg"].asInt() == -42);
    CHECK(out["a.double"].isDouble());
    CHECK(out["a.double"].asDouble() > 25.49 && out["a.double"].asDouble() < 25.51);

    // Encoding is deterministic (std::map order), so re-encoding the decoded
    // map must reproduce the bytes. That is what makes wire logs diffable.
    CHECK(EncodeTagObject(out) == json);
}

static void testDocumentedLossyEdge()
{
    // B -- PINNED, see TagJson.h "ONE LOSSY EDGE". A Double that is exactly
    // integral comes back as Int because JSON has one number type and cJSON
    // does not keep the original token. Harmless here; asserted so that any
    // future change to it is a deliberate act with a failing test attached.
    TagMap in;
    in["d"] = TagValue::makeDouble(1.0);

    TagMap out;
    std::string err;
    CHECK(DecodeTagObject(EncodeTagObject(in), out, err));
    CHECK(out["d"].isInt());
    CHECK(out["d"].asInt() == 1);
    CHECK(!out["d"].isDouble());
}

static void testDecodeIsAllOrNothing()
{
    // C -- a member this seam cannot carry must reject the WHOLE frame and
    // leave the caller's map untouched, because on the live link that map is
    // the state a browser is looking at.
    TagMap out;
    out["pre.existing"] = TagValue::makeInt(7);

    std::string err;
    CHECK(!DecodeTagObject("{\"ok\":1,\"bad\":[1,2]}", out, err));
    CHECK(!err.empty());
    CHECK(out.size() == 1);
    CHECK(out["pre.existing"].asInt() == 7);

    CHECK(!DecodeTagObject("{\"ok\":1,\"bad\":{\"x\":1}}", out, err));
    CHECK(out.size() == 1);

    CHECK(!DecodeTagObject("not json at all", out, err));
    CHECK(!DecodeTagObject("[1,2,3]", out, err));      // array, not an object
    CHECK(out.size() == 1);
}

// ===========================================================================
//  D -- one real socket round trip
// ===========================================================================
static void testLiveLink()
{
    TagSnapshot source;
    TagMap m;
    m["machine.id"]   = TagValue::makeString("HT-9046LS");
    m["temp.pv"]      = TagValue::makeDouble(25.5);
    m["status.spare"] = TagValue::makeInt(1);
    source.publish(m);

    TcpPublisherConfig pcfg;
    pcfg.port        = 0;      // ephemeral -- see the header note
    pcfg.heartbeatMs = 200;

    TcpTagPublisher pub(source, pcfg);
    std::string err;
    CHECK(pub.Start(err));
    if (!err.empty()) { std::printf("publisher start error: %s\n", err.c_str()); return; }
    CHECK(pub.BoundPort() != 0);

    TagSnapshot sink;
    TcpClientConfig ccfg;
    ccfg.host = "127.0.0.1";
    ccfg.port = pub.BoundPort();
    TcpTagClient client(sink, ccfg);
    CHECK(client.Start(err));

    // On connect the publisher sends hello + a FULL snapshot, so the consumer
    // is correct from its first frame without asking for anything.
    CHECK(waitFor([&] { return sink.publishedTagCount() == 3; }, 5000));

    TagSnapshotView got = sink.read();
    CHECK(got.tags["machine.id"].asString() == "HT-9046LS");
    CHECK(got.tags["status.spare"].asInt() == 1);

    // A change must arrive as a patch, not a re-send of everything.
    m["temp.pv"] = TagValue::makeDouble(80.25);
    source.publish(m);
    CHECK(waitFor([&] {
        return sink.read().tags["temp.pv"].asDouble() > 80.0;
    }, 5000));

    // A tag that stops being published must actually disappear on the far
    // side. This is the `removed` array doing its job -- encoding it as null
    // would leave a stale key behind that reads as "present but unknown".
    m.erase("status.spare");
    source.publish(m);
    CHECK(waitFor([&] { return sink.publishedTagCount() == 2; }, 5000));
    // ONE copy, then look in it. read() returns a VALUE by contract
    // (TagSnapshot.h: "never a reference into a buffer that can be swapped"),
    // so find() and end() on two separate read() calls are iterators into two
    // different containers and comparing them is meaningless.
    const TagSnapshotView afterRemoval = sink.read();
    CHECK(afterRemoval.tags.find("status.spare") == afterRemoval.tags.end());

    const TcpTagPublisher::Stats ps = pub.GetStats();
    CHECK(ps.clientsAccepted >= 1);
    CHECK(ps.framesSent >= 1);
    CHECK(ps.clientsDropped == 0);

    const TcpTagClient::Stats cs = client.GetStats();
    CHECK(cs.parseErrors == 0);
    CHECK(cs.framesReceived >= 2);

    client.Stop();
    pub.Stop();
    CHECK(!pub.Running());
    CHECK(!client.Running());
}

// ===========================================================================
//  E -- the consumer outlives the publisher
// ===========================================================================
static void testReconnect()
{
    TagSnapshot source;
    TagMap m;
    m["k"] = TagValue::makeInt(1);
    source.publish(m);

    TcpPublisherConfig pcfg;
    pcfg.port        = 0;
    pcfg.heartbeatMs = 200;

    std::string err;
    TagSnapshot sink;

    unsigned short port = 0;
    {
        TcpTagPublisher pub(source, pcfg);
        CHECK(pub.Start(err));
        port = pub.BoundPort();

        TcpClientConfig ccfg;
        ccfg.host        = "127.0.0.1";
        ccfg.port        = port;
        ccfg.reconnectMs = 200;
        TcpTagClient client(sink, ccfg);
        CHECK(client.Start(err));
        CHECK(waitFor([&] { return client.Connected(); }, 5000));
        CHECK(waitFor([&] { return sink.publishedTagCount() == 1; }, 5000));

        pub.Stop();                                   // publisher goes away
        CHECK(waitFor([&] { return !client.Connected(); }, 5000));

        // Same port again: the gateway is expected to survive a handler
        // restart, so the consumer must find it without being told.
        TcpPublisherConfig pcfg2 = pcfg;
        pcfg2.port = port;
        TagSnapshot source2;
        TagMap m2;
        m2["k"]     = TagValue::makeInt(2);
        m2["after"] = TagValue::makeString("restart");
        source2.publish(m2);

        TcpTagPublisher pub2(source2, pcfg2);
        if (pub2.Start(err)) {
            CHECK(waitFor([&] { return client.Connected(); }, 8000));
            CHECK(waitFor([&] { return sink.publishedTagCount() == 2; }, 8000));
            CHECK(sink.read().tags["k"].asInt() == 2);
            pub2.Stop();
        } else {
            // Rebinding the same ephemeral port can lose a race with TIME_WAIT
            // on a busy machine. That is an environment property, not a defect
            // in the link, so say so rather than failing the suite.
            std::printf("NOTE: could not rebind port %u (%s) -- reconnect leg skipped\n",
                        (unsigned)port, err.c_str());
        }
        client.Stop();
    }
}

int main()
{
    std::printf("== WebBridge TCP sidecar link ==\n");

    testJsonRoundTrip();
    testDocumentedLossyEdge();
    testDecodeIsAllOrNothing();
    testLiveLink();
    testReconnect();

    std::printf("%d checks, %d failed\n", g_total, g_fail);
    return g_fail == 0 ? 0 : 1;
}
