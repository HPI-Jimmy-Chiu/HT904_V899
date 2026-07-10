// tests/test_barcode_shuttle1_ccdscan.cpp
// Verification harness for BarCode_DoBarcodeCCDInShuttle_1 (golden
// BarCode_Sh1.cpp:83-2453, TfBarCode::DoBarcodeCCDInShuttle_1).
//
// LIMITATION (stated explicitly, same class of limitation as test_ContactForce):
// we CANNOT run the original BCB6 binary, and this SM's "happy path" needs a
// real CCD ClientSocket ack that has no vclcompat shim yet (see the .cpp file
// banner gate #1) -- driving the SM all the way to a barcode-decoded state is
// therefore not reachable offline.  This harness instead verifies the two
// deterministic, timing-independent entry behaviours that ARE fully faithful
// offline:
//   (1) the case-100 fast path: an empty (no real-IC) shuttle with
//       bVerify=false returns true immediately (golden BarCode_Sh1.cpp:135-137).
//   (2) the "shuttle has a real IC" busy path: the SM does NOT take the fast
//       path, instead arms its exposure-timer ladder and returns false (still
//       busy) on the very first call -- exercising the gated no-op CCD-comm
//       stand-ins (CCD1_ClearBuffer/CCD1_AddCCDCommunicationLog) end-to-end
//       without crashing, and confirming it does not falsely report done.
//
// No external test framework: PASS/FAIL harness, non-zero exit on any FAIL.

#include "BarCode/BarCode_Shuttle1_CCDScan.h"
#include "aHotPlateSubstrate.h"   // FLCarryKit / InArmSuck (TMyKitSuck) / HAS_IC
#include <cstdio>

static int g_pass = 0;
static int g_fail = 0;

static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected) { std::printf("PASS  %-46s got=%d\n", name, (int)got); ++g_pass; }
    else { std::printf("FAIL  %-46s got=%d exp=%d\n", name, (int)got, (int)expected); ++g_fail; }
}

int main()
{
    std::printf("=== BarCode_DoBarcodeCCDInShuttle_1 translation verification ===\n");
    std::printf("(no real CCD ClientSocket ack available offline; original BCB6\n");
    std::printf(" binary NOT run -- see .cpp file banner gate #1 for scope)\n\n");

    // -------------------------------------------------------------------
    // Case 1: empty shuttle (default global state -- TMyKitSuck ctor never
    // sets Item[][], which is zero-initialised == NULL_IC(0) at global scope,
    // so HasRealIC() is false out of the box).  bVerify=false -> the
    // case-100 fast path (golden :135-137) must return true on the very
    // first call, without needing any real HAL/CCD comms.
    // -------------------------------------------------------------------
    std::printf("-- fast path: empty shuttle, bVerify=false --\n");
    {
        bool r = BarCode_DoBarcodeCCDInShuttle_1(false);
        check_b("empty shuttle -> immediate true", r, true);
    }

    // -------------------------------------------------------------------
    // Case 2: force a real IC into row 0 / col 0 of FLCarryKit (the site the
    // default InArmSuck.iShtCol=1 / iShtRow=2 topology scans first).  Task
    // is now sitting at 100 (from case 1's fallthrough, never advanced past
    // the fast-return check), so this next call re-enters case 100, this
    // time finds a real IC, and must NOT return true: it should run the
    // full case-100 body (grid-clear/log/ClearBuffer no-ops, arm the
    // 5-second BarcodePosDelay retry timer) and fall through into case 500,
    // where the just-armed timer has not elapsed yet -> returns false
    // (still busy).  This exercises the gated CCD-comm stand-ins for real
    // without any wall-clock wait.
    // -------------------------------------------------------------------
    std::printf("\n-- busy path: shuttle has a real IC, bVerify=false --\n");
    {
        FLCarryKit.Item[0][0] = HAS_IC;   // golden cmydef.cpp: HAS_IC=2 (real device)
        bool r = BarCode_DoBarcodeCCDInShuttle_1(false);
        check_b("real IC present -> not yet finished (false)", r, false);
    }

    // -------------------------------------------------------------------
    // Case 3: calling again immediately (same process, timer still running)
    // must be idempotent/safe -- still false, no crash, no state corruption.
    // -------------------------------------------------------------------
    std::printf("\n-- busy path: immediate re-poll (timer still running) --\n");
    {
        bool r = BarCode_DoBarcodeCCDInShuttle_1(false);
        check_b("re-poll before timer elapses -> still false", r, false);
    }

    // -------------------------------------------------------------------
    // Summary
    // -------------------------------------------------------------------
    std::printf("\n=== SUMMARY: %d passed, %d failed (total %d) ===\n",
                g_pass, g_fail, g_pass + g_fail);
    if (g_fail != 0)
    {
        std::printf("RESULT: FAIL\n");
        return 1;
    }
    std::printf("RESULT: ALL PASS\n");
    return 0;
}
