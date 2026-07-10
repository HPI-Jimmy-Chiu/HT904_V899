// tests/test_cMyNUDN1.cpp
// Verification harness for the CanBus/cMyNUDN1 translation (TMyNUDN1).
//
// Exercises the translated public API against behavior hand-derived/transcribed
// from the BCB6 golden reference
//   HT9011UC_Code_V3.33.906.0_20260618/CanBus/cMyNUDN1.cpp / .h
//
// LIMITATION (same as test_ContactForce.cpp / test_cContact.cpp): we CANNOT run
// the original BCB6 binary (no Borland compiler in this environment), and this
// build has NO vendor DNM100UD SDK (HAVE_DNM100 is OFF by default -- see
// cMyNUDN1.cpp).  Verification here is therefore:
//   (1) the translation compiles clean under MinGW g++, AND
//   (2) the pure-logic paths (bitfield union decode, getters, array
//       bookkeeping, argument-range guards) match golden exactly, AND
//   (3) the #else (no-vendor-SDK) stub paths behave as the documented
//       "benign all-zero / success" simulated device.
//
// No external test framework: a tiny check harness prints PASS/FAIL per case
// and a final summary, and returns non-zero on ANY failure.

#include "CanBus/cMyNUDN1.h"
#include <cstdio>

static int g_pass = 0;
static int g_fail = 0;

static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected)
    {
        printf("PASS  %-64s got=%-5s exp=%-5s\n", name, got ? "true" : "false", expected ? "true" : "false");
        ++g_pass;
    }
    else
    {
        printf("FAIL  %-64s got=%-5s exp=%-5s\n", name, got ? "true" : "false", expected ? "true" : "false");
        ++g_fail;
    }
}

static void check_i(const char* name, long got, long expected)
{
    if (got == expected)
    {
        printf("PASS  %-64s got=%-6ld exp=%-6ld\n", name, got, expected);
        ++g_pass;
    }
    else
    {
        printf("FAIL  %-64s got=%-6ld exp=%-6ld\n", name, got, expected);
        ++g_fail;
    }
}

int main()
{
    printf("=== CanBus/cMyNUDN1 (TMyNUDN1) translation verification ===\n");
    printf("(no vendor DNM100UD SDK in this build -- HAVE_DNM100 OFF; exercising\n");
    printf(" the Sim-default paths + pure bitfield-union decode logic)\n\n");

    // =========================================================================
    // Layout oracle: bitfield unions must pack to the same sizes as the golden
    // BCB6 layout (InArea_Output/InArea_Status = 2 bytes each matching their
    // sibling `unsigned short iRaw`; InArea_Monitor_Mode = 40 bytes matching
    // `unsigned char iRawItem[40]`).  A silent GCC-vs-Borland bitfield-packing
    // divergence would corrupt every amplifier's decoded value/IO bit --
    // this is the highest-risk part of a faithful translation, so pin it down
    // at compile time.
    // =========================================================================
    printf("-- layout oracle (golden cMyNUDN1.h union sizes) --\n");
    {
        static_assert(sizeof(TMyNUDN1::InArea_Output) == 2, "InArea_Output must be 2 bytes (matches sibling unsigned short iRaw)");
        static_assert(sizeof(TMyNUDN1::InArea_Status) == 2, "InArea_Status must be 2 bytes (matches sibling unsigned short iRaw)");
        static_assert(sizeof(TMyNUDN1::InArea_Monitor_Mode) == 40, "InArea_Monitor_Mode must be 40 bytes (matches unsigned char iRawItem[40])");
        printf("PASS  static_assert layout sizes (2/2/40 bytes)\n");
        ++g_pass;
    }

    // =========================================================================
    // ctor + read-only getters (golden __published __property -> getter methods)
    // =========================================================================
    printf("\n-- ctor + getters (golden cMyNUDN1.cpp:10-34, .h __published block) --\n");
    {
        // BoardNo=1, MACID=5, Type=ConType_Poll(1), InputLen=16, OutputLen=0, EPR=200, Amplifier=8
        TMyNUDN1 nu(1, 5, 1, 16, 0, 200, 8);
        check_i("iGetActivedBoardNo() == 1", nu.iGetActivedBoardNo(), 1);
        check_i("iGetDesMACID() == 5",       nu.iGetDesMACID(),       5);
        check_i("iGetAmplifier() == 8",      nu.iGetAmplifier(),      8);
    }

    // =========================================================================
    // Initail() -- golden's REAL (non-commented) ctor/Initail split: only
    // Initail() actually zeroes iGetIO_Value[]/iSetIO_Value[] (golden
    // cMyNUDN1.cpp:39-56); the ctor itself does NOT (its zero-init block is
    // dead/commented-out code in golden -- see cMyNUDN1.cpp header comment).
    // =========================================================================
    printf("\n-- Initail() array bookkeeping (golden cMyNUDN1.cpp:39-56) --\n");
    {
        TMyNUDN1 nu(1, 5, 1, 16, 0, 200, 8);
        nu.Initail();
        bool all_ok = true;
        for (int i = 0; i < 16; ++i)
        {
            if (nu.iGetIO_Value[i] != 0) all_ok = false;
            if (nu.iSetIO_Value[i] != -1) all_ok = false;
        }
        check_b("after Initail(): iGetIO_Value[0..15]==0 && iSetIO_Value[0..15]==-1", all_ok, true);
    }

    // =========================================================================
    // iReadInputData -- mode dispatch (golden cMyNUDN1.cpp:57-109)
    // =========================================================================
    printf("\n-- iReadInputData mode dispatch --\n");
    {
        // InputLen MUST equal (Amplifier*2)+8 to cover the whole 40-byte
        // InArea_Monitor_Mode struct on a mode-2 poll (8-byte Output/Status/
        // reserved/Property header + 2 bytes per amplifier Value0N) -- this
        // is exactly the formula the real caller uses (golden
        // CanBus/cMyDNM100UD.cpp:52-58: `(iAmplifierList[i]*2)+8`).  A
        // mismatched (too-small) InputLen would only refresh the first few
        // Value0N slots on each poll, leaving the rest stale/garbage from
        // Initail()'s own preserved uninitialized-read bug (see cMyNUDN1.cpp)
        // -- an accurate-but-surprising golden behavior, not exercised here.
        TMyNUDN1 nu(1, 5, 1, /*InputLen=*/16 * 2 + 8, 0, 200, /*Amplifier=*/16);
        nu.Initail();

        // Modes 0/1/3/4 are empty branches in golden -> fall through to `return 0;`.
        check_i("mode 0 (Standard)  -> 0", nu.iReadInputData(0), 0);
        check_i("mode 1 (External)  -> 0", nu.iReadInputData(1), 0);
        check_i("mode 3 (Full)      -> 0", nu.iReadInputData(3), 0);
        check_i("mode 4 (Minimum)   -> 0", nu.iReadInputData(4), 0);

        // Unknown mode -> -1 (golden cMyNUDN1.cpp:104-108).
        check_i("mode 99 (unknown)  -> -1", nu.iReadInputData(99), -1);

        // Mode 2 (Monitor): with HAVE_DNM100 off, the Sim stub simulates a
        // successful poll of an all-zero device (golden real-vendor path
        // would call DNM100_WriteOutputData/DNM100_ReadInputData).
        check_i("mode 2 (Monitor, no vendor SDK) -> 0 (simulated success)", nu.iReadInputData(2), 0);

        // After an all-zero simulated poll, every decoded amplifier value/IO
        // bit must read back as 0/false (golden CurrectValue/CurrectOutput
        // over an all-zero InAreaMonitor).
        bool values_zero = true, io_false = true;
        for (int i = 0; i < 16; ++i)
        {
            if (nu.iGetValue(i) != 0) values_zero = false;
            if (nu.bGetIO(i) != false) io_false = false;
        }
        check_b("post-poll: iGetValue(0..15)==0 for all channels", values_zero, true);
        check_b("post-poll: bGetIO(0..15)==false for all channels", io_false, true);
    }

    // =========================================================================
    // iGetValue / bGetIO -- direct bitfield-union decode oracle (golden
    // cMyNUDN1.cpp:111-133 + the union's CurrectValue/CurrectOutput helpers).
    // Poke InAreaMonitor directly (bypassing iReadInputData/vendor SDK
    // entirely) to pin down the raw bitfield decode logic itself.
    // =========================================================================
    printf("\n-- iGetValue/bGetIO bitfield decode oracle (direct InAreaMonitor poke) --\n");
    {
        TMyNUDN1 nu(1, 5, 1, 16, 0, 200, 16);

        // Value05 corresponds to amplifier index 4 (0-based) per
        // CurrectValue's if/else-if ladder (golden .h CurrectValue body).
        nu.InAreaMonitor.InArea.Value05 = 1234;
        check_i("Value05=1234 -> iGetValue(4)", nu.iGetValue(4), 1234);
        check_i("Value05=1234 -> iGetValue(0) unaffected", nu.iGetValue(0), 0);

        // Value16 corresponds to amplifier index 15 (the `else` fallthrough
        // branch in CurrectValue -- also exercises index-15/highest-branch).
        nu.InAreaMonitor.InArea.Value16 = 4321;
        check_i("Value16=4321 -> iGetValue(15)", nu.iGetValue(15), 4321);

        // Out-of-range amplifier index -> 0 / false (golden cMyNUDN1.cpp
        // iGetValue/bGetIO range guard `iAmplifierNo>=0 && iAmplifierNo<=15`).
        check_i("iGetValue(16) out-of-range -> 0", nu.iGetValue(16), 0);
        check_i("iGetValue(-1) out-of-range -> 0", nu.iGetValue(-1), 0);
        check_b("bGetIO(16) out-of-range -> false", nu.bGetIO(16), false);
    }
    {
        TMyNUDN1 nu(1, 5, 1, 16, 0, 200, 16);

        // Output.IO.No07 corresponds to amplifier index 7 in CurrectOutput.
        nu.InAreaMonitor.InArea.Output.IO.No07 = 1;
        check_b("Output.No07=1 -> bGetIO(7)==true", nu.bGetIO(7), true);
        check_b("Output.No07=1 -> bGetIO(6) unaffected ==false", nu.bGetIO(6), false);
        check_b("Output.No07=1 -> bGetIO(8) unaffected ==false", nu.bGetIO(8), false);

        // Output.IO.No15 exercises the `else` fallthrough branch in CurrectOutput.
        nu.InAreaMonitor.InArea.Output.IO.No15 = 1;
        check_b("Output.No15=1 -> bGetIO(15)==true", nu.bGetIO(15), true);
    }

    // =========================================================================
    // iSetIO_Value_Send -- argument-range guard + (no vendor SDK) simulated
    // send success/failure (golden cMyNUDN1.cpp:135-160).
    // =========================================================================
    printf("\n-- iSetIO_Value_Send range guard + simulated send --\n");
    {
        TMyNUDN1 nu(1, 5, 1, 16, 0, 200, 16);

        // In range (0..15): with HAVE_DNM100 off, iRet is simulated as 0
        // (success) -> function returns -iRet == 0.
        check_i("iSetIO_Value_Send(0, 100)  in-range -> 0 (simulated success)", nu.iSetIO_Value_Send(0, 100), 0);
        check_i("iSetIO_Value_Send(15, 500) in-range -> 0 (simulated success)", nu.iSetIO_Value_Send(15, 500), 0);

        // Out of range: golden sets iRet=-1 then `return -iRet;` -> +1.
        check_i("iSetIO_Value_Send(16, 100) out-of-range -> 1", nu.iSetIO_Value_Send(16, 100), 1);
        check_i("iSetIO_Value_Send(-1, 100) out-of-range -> 1", nu.iSetIO_Value_Send(-1, 100), 1);
    }

    // -----------------------------------------------------------------------
    // Summary
    // -----------------------------------------------------------------------
    printf("\n=== SUMMARY: %d passed, %d failed (total %d) ===\n", g_pass, g_fail, g_pass + g_fail);
    if (g_fail != 0)
    {
        printf("RESULT: FAIL\n");
        return 1;
    }
    printf("RESULT: ALL PASS\n");
    return 0;
}
