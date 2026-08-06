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

#include "WebBridge/TagValue.h"

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

}  // namespace

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

    // --- LastSet-derived scalars --------------------------------------------
    // These read ONLY LastSet, so the blob's liveness settles them. Their
    // meaning is deliberately not interpreted here (no "Real"/"Dummy" label):
    // that mapping lives in golden's UI code and has not been verified, and a
    // confidently wrong label is worse than a raw code.
    stageInt(snap, "lastset.tester",     lastS, LastSet.iTester);
    stageInt(snap, "lastset.realDummy",  lastS, LastSet.iRealDummy);
    stageInt(snap, "lastset.runStartMode", lastS, LastSet.iRunStartMode);
    stageInt(snap, "lastset.temperature",  lastS, LastSet.iTemperature);

    // --- everything whose source is measurably dead --------------------------
    for (std::size_t i = 0; i < kUnloadedCount; ++i) {
        stageNull(snap, kUnloadedTags[i]);
    }

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

    // 3 identity strings + 1 customer code + 4 LastSet scalars + the dead set.
    c.total = 3 + 1 + 4 + kUnloadedCount;
    c.live  = (strs ? 3u : 0u) + (cust ? 1u : 0u) + (lastS ? 4u : 0u);

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
