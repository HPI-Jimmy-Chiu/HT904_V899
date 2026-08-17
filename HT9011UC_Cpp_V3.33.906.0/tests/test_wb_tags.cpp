// =============================================================================
//  test_wb_tags.cpp -- WebBridgeTags: does the browser get the truth?
//
//  AI(W906-WebBridge) 20260806.
//
//  The single property under test is the one the whole design exists for:
//
//      a tag whose source is loaded carries a REAL value;
//      a tag whose source is NOT loaded carries NULL, never 0.
//
//  The browser renders null as "---" and 0 as "0.00", so getting this backwards
//  puts a measurement on an operator's screen that was never taken. On a
//  machine that runs at 130 C that is not a cosmetic bug.
//
//  DO-NOT-MODIFY-REAL-CONFIG: LoadMachineConfig() seeds missing keys, i.e. it
//  WRITES to asGeneralPath. As in tests/test_wb_datalayer.cpp, this repoints
//  asGeneralPath at a scratch copy first and restores it afterwards.
// =============================================================================
#include "WebBridgeTags.h"

#include "database.h"
#include "cprod.h"
#include "Config.h"
#include "LastSet.h"
#include "cmydef.h"
#include "common.h"

#include "WebBridge/TagSnapshot.h"
#include "WebBridge/TagValue.h"

#include <windows.h>
#include <cstdio>

static int g_total = 0;
static int g_fail = 0;

static void check(bool ok, const char* what)
{
    ++g_total;
    std::printf("%s: %s\n", ok ? "PASS" : "FAIL", what);
    if (!ok) ++g_fail;
}

int main()
{
    std::setvbuf(stdout, 0, _IONBF, 0);

    using webbridge::TagSnapshot;
    using webbridge::TagSnapshotView;
    using webbridge::TagValue;

    // --- 1. before loading, EVERYTHING must be null --------------------------
    // This is the control. If a tag carried a value here, the "live" checks
    // below would prove nothing -- they could be reading a leftover.
    {
        TagSnapshot snap;
        ht9045::PublishHandlerTags(snap);
        const TagSnapshotView v = snap.read();

        std::size_t nonNull = 0;
        for (webbridge::TagMap::const_iterator it = v.tags.begin();
             it != v.tags.end(); ++it) {
            if (!it->second.isNull()) ++nonNull;
        }
        std::printf("-- 1. before LoadMachineConfig: %u tags, %u non-null\n",
                    (unsigned)v.tags.size(), (unsigned)nonNull);
        check(v.tags.size() > 0, "PublishHandlerTags stages a non-empty snapshot");
        check(nonNull == 0,
              "every tag is null before the data layer is loaded "
              "(nothing is inventing values)");

        const ht9045::TagCoverage c = ht9045::HandlerTagCoverage();
        std::printf("   coverage: %u live / %u total\n",
                    (unsigned)c.live, (unsigned)c.total);
        check(c.live == 0, "coverage reports 0 live before loading");
    }

    // --- 2. load the data layer, on a scratch copy ---------------------------
    const AnsiString savedGeneralPath = asGeneralPath;

    char tmp[MAX_PATH];
    if (::GetTempPathA(MAX_PATH, tmp) == 0) {
        std::printf("SKIP: no temp path\n");
        std::printf("\ntest_wb_tags: %d checks, %d failure(s)\n", g_total, g_fail);
        return g_fail == 0 ? 0 : 1;
    }
    const AnsiString scratch = AnsiString(tmp) + "wb_tags_general.ini";

    if (!::CopyFileA(savedGeneralPath.c_str(), scratch.c_str(), FALSE)) {
        std::printf("SKIP: no Gerneral.ini on this box (err %lu)\n",
                    (unsigned long)::GetLastError());
        std::printf("\ntest_wb_tags: %d checks, %d failure(s)\n", g_total, g_fail);
        return g_fail == 0 ? 0 : 1;
    }

    asGeneralPath = scratch;
    const bool loaded = LoadMachineConfig();
    check(loaded, "LoadMachineConfig() succeeded");

    // --- 3. after loading ----------------------------------------------------
    {
        TagSnapshot snap;
        const std::size_t staged = ht9045::PublishHandlerTags(snap);
        const TagSnapshotView v = snap.read();

        std::printf("\n-- 3. after LoadMachineConfig: %u tags staged\n",
                    (unsigned)staged);

        const TagValue& type = v.tags.find("machine.id.type")->second;
        const TagValue& cust = v.tags.find("machine.customerCode")->second;
        std::printf("   machine.id.type      = %s\n", type.debugString().c_str());
        std::printf("   machine.customerCode = %s\n", cust.debugString().c_str());

        check(type.isString() && type.asString().size() > 0,
              "machine.id.type carries a REAL value once IniConfig strings load");
        check(cust.isInt() && cust.asInt() != 0,
              "machine.customerCode carries a REAL value");

        // The load-bearing negative. Temperature.* and UN150Read[] are both
        // measurably unwritten in this port, so these MUST be null. If this ever
        // reports 0 instead, the browser will draw "0.00" for a heater zone
        // nobody read.
        const char* mustBeNull[] = {
            "temp.pv", "temp.sv", "temp.soak", "temp.mode",
            "zone.hotplate.1", "zone.shuttle.1", "zone.heatgun.1",
            "tower.red", "tester.name", "status.uph"
        };
        bool allNull = true;
        for (std::size_t i = 0; i < sizeof(mustBeNull) / sizeof(mustBeNull[0]); ++i) {
            webbridge::TagMap::const_iterator it = v.tags.find(mustBeNull[i]);
            if (it == v.tags.end() || !it->second.isNull()) {
                std::printf("   NOT NULL: %s = %s\n", mustBeNull[i],
                            it == v.tags.end() ? "(absent)"
                                               : it->second.debugString().c_str());
                allNull = false;
            }
        }
        check(allNull,
              "every tag with an unloaded source is NULL, not 0 "
              "(null renders \"---\", 0 renders \"0.00\")");

        const ht9045::TagCoverage c = ht9045::HandlerTagCoverage();
        std::printf("   coverage: %u live / %u total\n",
                    (unsigned)c.live, (unsigned)c.total);
        check(c.live > 0 && c.live < c.total,
              "coverage is partial and honest about it");
    }

    // --- 4. FW-1a: the LastSet-blob tags decode and map correctly ------------
    //AI(W906-FW1) 20260817: direct-write oracle. The test seeds exact fields
    // and asserts the published tag, pinning three properties: (a) the decode
    // goes through golden's own StartModeName array (cmydef.cpp:62), (b) the
    // [arm][row][col] -> site.arm{a}.s{n} mapping measured from ReadTestMode
    // ([0]=Arm1 "Dut <name>" / [1]=Arm2 "Dut <name>2", cprod.cpp:3689/:3773;
    // sites 9..16 = row 1), and (c) out-of-range codes publish null. Tests may
    // write machine globals; production code may not -- that asymmetry is the
    // same one the idle-pump change established.
    {
        LastSet.iRunStartMode = rsmContinuStart_ART;             // code 9
        LastSet.bUseTestSocket[1][1][2] = true;                  // arm2 row1 col2 -> s11
        LastSet.bUseTestSocket[0][0][0] = false;                 // arm1 s1 -> real 0

        TagSnapshot snap;
        ht9045::PublishHandlerTags(snap);
        const TagSnapshotView v = snap.read();

        const TagValue& sm  = v.tags.find("startmode.value")->second;
        const TagValue& s11 = v.tags.find("site.arm2.s11")->second;
        const TagValue& s1  = v.tags.find("site.arm1.s1")->second;
        std::printf("\n-- 4. FW-1a oracle\n");
        std::printf("   startmode.value = %s\n", sm.debugString().c_str());
        check(sm.isString() && sm.asString() == "ContinuStart_ART",
              "startmode.value decodes code 9 through StartModeName (cmydef.cpp:66)");
        check(s11.isInt() && s11.asInt() == 1,
              "site.arm2.s11 reads bUseTestSocket[1][1][2] -- arm dim per cprod.cpp:3773");
        check(s1.isInt() && s1.asInt() == 0,
              "site.arm1.s1 is a REAL 0 (site off) while the blob is live, not null");

        LastSet.iRunStartMode = -1;                              // rsmNull
        TagSnapshot snap2;
        ht9045::PublishHandlerTags(snap2);
        check(snap2.read().tags.find("startmode.value")->second.isNull(),
              "an out-of-range start-mode code publishes null, never a guessed word");
    }

    // --- 5. FW-1b: sort counters pin BOTH index spaces ------------------------
    //AI(W906-FW1b) 20260817: the gate reads Prod.iTrayType[e6TrayName]
    // (eFix2 == 7) while the value reads LastSet.BinCT[0][e3TrayName]
    // (e3Fix2 == 4) -- golden maps between them via iTo3Unload (main.cpp:
    // 1954-1966), whose PORT global is uninitialized all-zero, which is why
    // the wiring uses constants. This oracle fails if anyone "simplifies"
    // the two spaces into one.
    {
        Prod.iTrayType[eFix2]  = tTrayFix;                       // gate idx 7 -> configured
        Prod.iTrayType[eAuto2] = tNotUse;                        // gate idx 1 -> not configured
        LastSet.BinCT[0][e3Fix2] = 77;                           // column idx 4

        TagSnapshot snap;
        ht9045::PublishHandlerTags(snap);
        const TagSnapshotView v = snap.read();
        const TagValue& f2 = v.tags.find("sort.fix2.count")->second;
        const TagValue& a2 = v.tags.find("sort.auto2.count")->second;
        std::printf("\n-- 5. FW-1b oracle: sort.fix2.count = %s\n",
                    f2.debugString().c_str());
        check(f2.isInt() && f2.asInt() == 77,
              "sort.fix2.count reads BinCT[0][e3Fix2==4] gated by iTrayType[eFix2==7] (cSortCT.cpp:396/:399)");
        check(a2.isNull(),
              "an unconfigured station (iTrayType==tNotUse) publishes null even with a live blob");
    }

    CloseGeneralIniFile();
    asGeneralPath = savedGeneralPath;
    ::DeleteFileA(scratch.c_str());

    std::printf("\ntest_wb_tags: %d checks, %d failure(s)\n", g_total, g_fail);
    std::printf("RESULT: %s\n", g_fail == 0 ? "PASS" : "FAIL");
    return g_fail == 0 ? 0 : 1;
}
