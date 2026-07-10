// tests/test_MyNUEC1.cpp
// Verification harness for the EtherCAT/MyNUEC1 translation (TMyNUEC1).
//
// Exercises the translated public API against behavior hand-derived/transcribed
// from the BCB6 golden reference
//   HT9011UC_Code_V3.33.906.0_20260618/EtherCAT/MyNUEC1.cpp / .h
//
// LIMITATION (same as test_ContactForce.cpp / test_cContact.cpp / test_cMyNUDN1.cpp):
// we CANNOT run the original BCB6 binary (no Borland compiler in this
// environment), and this build has NO vendor Advantech EtherCAT SDK
// (HAVE_PCI1203 is OFF by default -- see MyNUEC1.cpp).  Verification here is:
//   (1) the translation compiles clean under MinGW g++, AND
//   (2) the pure-logic paths (bitfield union decode, getters, array
//       bookkeeping, argument-range guards, dead-code/quirk findings) match
//       golden exactly, AND
//   (3) the #else (no-vendor-SDK) stub paths behave as golden's own already-
//       modeled "Result != SUCCESS" communication-failure branches.
//
// No external test framework: a tiny check harness prints PASS/FAIL per case
// and a final summary, and returns non-zero on ANY failure.

#include "EtherCAT/MyNUEC1.h"
#include "cmydef.h"       // NUEC_TYPE (extern int) -- flipped between eNUEC1/eNUEC1A below
#include "MachineType.h"  // eNUEC1, eNUEC1A
#include <cstdio>
#include <cstring>

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

static void check_s(const char* name, const AnsiString& got, const char* expected)
{
    if (got.c_str() != nullptr && std::strcmp(got.c_str(), expected) == 0)
    {
        printf("PASS  %-64s got=\"%s\"\n", name, got.c_str());
        ++g_pass;
    }
    else
    {
        printf("FAIL  %-64s got=\"%s\" exp=\"%s\"\n", name, got.c_str(), expected);
        ++g_fail;
    }
}

int main()
{
    printf("=== EtherCAT/MyNUEC1 (TMyNUEC1) translation verification ===\n");
    printf("(no vendor Advantech EtherCAT SDK in this build -- HAVE_PCI1203 OFF;\n");
    printf(" exercising the stub-fallback paths + pure bitfield-union decode logic)\n\n");

    // =========================================================================
    // Layout oracle: bitfield unions must pack to the same sizes as the golden
    // BCB6 layout.  A silent GCC-vs-Borland bitfield-packing divergence would
    // corrupt every amplifier's decoded IO bit -- pin it down at compile time,
    // same as the CanBus/cMyNUDN1 sibling test.
    // =========================================================================
    printf("-- layout oracle (golden MyNUEC1.h union sizes) --\n");
    {
        static_assert(sizeof(TMyNUEC1::_IOWord) == 2, "_IOWord must be 2 bytes (16 x 1-bit fields == unsigned short iRaw)");
        static_assert(sizeof(TMyNUEC1::_IOChar) == 1, "_IOChar must be 1 byte (8 x 1-bit fields == unsigned char iRaw)");
        static_assert(sizeof(TMyNUEC1::_IOArea) == 4, "_IOArea must be 4 bytes (Area = IOWord(2) + IOCharL(1) + IOCharH(1))");
        printf("PASS  static_assert layout sizes (_IOWord=2, _IOChar=1, _IOArea=4 bytes)\n");
        ++g_pass;
    }

    // =========================================================================
    // Ctor + read-only getters (golden __published __property -> getter methods)
    // golden EtherCAT/MyNUEC1.cpp:14-64
    // =========================================================================
    printf("\n-- ctor + getters (golden MyNUEC1.cpp:14-64, .h __published block) --\n");
    {
        // Ring=2, MACID=12, Amplifier=8
        TMyNUEC1 nu(2, 12, 8);
        check_i("iGetRing() == 2",       nu.iGetRing(),       2);
        check_i("iGetDesMACID() == 12",  nu.iGetDesMACID(),   12);
        check_i("iGetAmplifier() == 8",  nu.iGetAmplifier(),  8);
    }

    // =========================================================================
    // Ctor zero-init bookkeeping.  golden's ctor explicitly ZeroMemory()s
    // iCurrectValue/iSettingValue/iNeedSettingValue/RetWriteSettingValue but
    // NOT bNeedDosetValue (see MyNUEC1.cpp ctor comment: golden relies on
    // VCL NewInstance's zero-fill-on-allocate for that one array; we add an
    // explicit ZeroMemory to reproduce the SAME real runtime behavior).  This
    // checks OUR reproduction actually holds (all-false at construction, not
    // whatever `new` happened to leave on the heap).
    // =========================================================================
    printf("\n-- ctor zero-init (RetWriteSettingValue[] / bNeedDosetValue[] all-clear) --\n");
    {
        TMyNUEC1 nu(1, 5, 16);
        bool all_ok = true;
        for (int i = 0; i < 16; ++i)
        {
            if (nu.RetWriteSettingValue[i] != 0) all_ok = false;
            if (nu.bNeedDosetValue[i] != false)  all_ok = false;
            if (nu.iNeedSettingValue[i] != 0)    all_ok = false;
        }
        check_b("after ctor: RetWriteSettingValue[]==0 && bNeedDosetValue[]==false && iNeedSettingValue[]==0", all_ok, true);
    }

    // =========================================================================
    // iGetValue / iGetIO_Value -- bound-check ASYMMETRY (all 3 read-back
    // getters use a DIFFERENT off-by-one policy in golden -- preserved
    // verbatim, see MyNUEC1.cpp comments at each site):
    //   iGetValue     :  0<=i && i< MaxFSN12(16)   -- correct bound
    //   iGetIO_Value  :  0<=i && i<=MaxFSN12(16)   -- REAL array-OOB-read bug
    //                    at i==16 (iSettingValue[16] is one past the array).
    //                    NOT exercised at i==16 here on purpose (would be
    //                    genuine UB even though it faithfully matches golden's
    //                    own UB) -- only the documented-safe boundary (15) and
    //                    the clearly-out-of-range low side (-1) are checked.
    // =========================================================================
    printf("\n-- iGetValue/iGetIO_Value bound checks (golden asymmetric off-by-one, preserved) --\n");
    {
        TMyNUEC1 nu(1, 5, 16);
        check_i("iGetValue(15) in-range (untouched -> 0)",  nu.iGetValue(15), 0);
        check_i("iGetValue(16) out-of-range -> 0",           nu.iGetValue(16), 0);
        check_i("iGetValue(-1) out-of-range -> 0",           nu.iGetValue(-1), 0);
        check_i("iGetIO_Value(15) in-range (untouched -> 0)", nu.iGetIO_Value(15), 0);
        check_i("iGetIO_Value(-1) out-of-range -> 0",         nu.iGetIO_Value(-1), 0);
        // iGetIO_Value(16) intentionally NOT called -- see comment block above.
    }

    // =========================================================================
    // _IOArea::CurrectOutput / CurrectOutput2 -- direct bitfield-union decode
    // oracle (golden MyNUEC1.h:96-128).  Poke IOArea directly (bypassing
    // ReadCurrentIO/vendor SDK entirely) to pin down the raw bitfield decode
    // logic itself.  Safe to call out-of-range here (no array indexing inside
    // these helpers -- see MyNUEC1.h comment -- just an if/else-if/else
    // ladder over named bitfields, so index 16 aliases to the No15/No07
    // branch with NO out-of-bounds memory access).
    // =========================================================================
    printf("\n-- IOArea.CurrectOutput/CurrectOutput2 bitfield decode oracle (direct poke) --\n");
    {
        TMyNUEC1 nu(1, 5, 16);

        nu.IOArea.Area.IOWord.IO.No04 = 1;
        check_b("IOWord.No04=1 -> CurrectOutput(4)==true",  nu.IOArea.CurrectOutput(4), true);
        check_b("IOWord.No04=1 -> CurrectOutput(3) unaffected ==false", nu.IOArea.CurrectOutput(3), false);
        nu.IOArea.Area.IOWord.IO.No15 = 1;
        check_b("IOWord.No15=1 -> CurrectOutput(15)==true (else-branch)", nu.IOArea.CurrectOutput(15), true);
    }
    {
        TMyNUEC1 nu(1, 5, 16);

        // IOCharL covers amplifiers 0-7, IOCharH covers 8-15 (CurrectOutput2).
        nu.IOArea.Area.IOCharL.IO.No03 = 1;
        check_b("IOCharL.No03=1 -> CurrectOutput2(3)==true",  nu.IOArea.CurrectOutput2(3), true);
        nu.IOArea.Area.IOCharH.IO.No02 = 1;
        check_b("IOCharH.No02=1 -> CurrectOutput2(10)==true (8+2)", nu.IOArea.CurrectOutput2(10), true);
        check_b("IOCharH.No02=1 -> CurrectOutput2(9) unaffected ==false", nu.IOArea.CurrectOutput2(9), false);
    }

    // =========================================================================
    // bGetIO -- NUEC_TYPE-based dispatch (CurrectOutput2 for eNUEC1/eNUEC1A,
    // CurrectOutput otherwise) + the golden off-by-one bound (<=MaxFSN12,
    // harmless here because CurrectOutput/2 have no array indexing -- see
    // MyNUEC1.cpp bGetIO comment).
    // =========================================================================
    printf("\n-- bGetIO NUEC_TYPE dispatch + off-by-one bound (golden MyNUEC1.cpp:171-184) --\n");
    {
        int saved = NUEC_TYPE;

        NUEC_TYPE = eNUEC1;  // dispatches to CurrectOutput2 (IOCharL/IOCharH)
        {
            TMyNUEC1 nu(1, 5, 16);
            nu.IOArea.Area.IOCharL.IO.No05 = 1;
            check_b("NUEC_TYPE=eNUEC1: bGetIO(5)==true (IOCharL path)", nu.bGetIO(5), true);
            check_b("NUEC_TYPE=eNUEC1: bGetIO(-1) out-of-range -> false", nu.bGetIO(-1), false);
            // bGetIO(16): off-by-one lets it through to IOArea.CurrectOutput2(16),
            // which has no array indexing (if/else-if/else over named
            // bitfields) so this is a SAFE call -- aliases to the No07 (last)
            // branch of IOCharH, matching golden's own documented quirk.
            check_b("NUEC_TYPE=eNUEC1: bGetIO(16) off-by-one lets it through (safe alias, not memory-unsafe)", nu.bGetIO(16), false);
        }

        NUEC_TYPE = 99;  // neither eNUEC1 nor eNUEC1A -> dispatches to CurrectOutput (IOWord)
        {
            TMyNUEC1 nu(1, 5, 16);
            nu.IOArea.Area.IOWord.IO.No09 = 1;
            check_b("NUEC_TYPE=other: bGetIO(9)==true (IOWord path)", nu.bGetIO(9), true);
        }

        NUEC_TYPE = saved;  // restore shared global for subsequent tests
    }

    // =========================================================================
    // Stub (no vendor SDK) fallback values -- mirrors golden's own
    // "Result != SUCCESS" branches (see MyNUEC1.cpp comments at each call site).
    // =========================================================================
    printf("\n-- stub fallback paths (HAVE_PCI1203 off -- golden's own failure branches) --\n");
    {
        TMyNUEC1 nu(1, 5, 4);

        check_b("ReadCurrentValue(0) -> false (stub)", nu.ReadCurrentValue(0), false);
        check_i("iGetValue(0) == -99999 after failed read", nu.iGetValue(0), -99999);
        check_b("ReadCurrentValue() [[all]] -> false (stub)", nu.ReadCurrentValue(), false);

        check_b("ReadCurrentIO() -> false (stub)", nu.ReadCurrentIO(), false);

        check_b("ReadSettingValue(0) -> false (stub)", nu.ReadSettingValue(0), false);
        check_i("iGetIO_Value(0) == -99999 after failed read", nu.iGetIO_Value(0), -99999);
        check_b("ReadSettingValue() [[all]] -> false (stub)", nu.ReadSettingValue(), false);

        check_b("WriteSettingValue(0, 1234) -> false (stub)", nu.WriteSettingValue(0, 1234), false);
        check_i("RetWriteSettingValue[0] == 1 (first vendor write call \"failed\")", nu.RetWriteSettingValue[0], 1);

        check_b("CheckAmplifierExist(0) -> false (stub)", nu.CheckAmplifierExist(0), false);
    }

    // =========================================================================
    // SetSetIOValue / WriteSettingValue() (all-pending) bookkeeping.
    // golden MyNUEC1.cpp:234-252 -- only VALID indices exercised (no bounds
    // check exists in golden SetSetIOValue; not probed out-of-range here to
    // avoid an out-of-bounds WRITE, see MyNUEC1.cpp comment).
    // =========================================================================
    printf("\n-- SetSetIOValue + WriteSettingValue() [[all-pending]] bookkeeping --\n");
    {
        TMyNUEC1 nu(1, 5, 4);
        nu.SetSetIOValue(1, 500);
        nu.SetSetIOValue(3, 700);
        check_b("SetSetIOValue(1,500): bNeedDosetValue[1]==true", nu.bNeedDosetValue[1], true);
        check_i("SetSetIOValue(1,500): iNeedSettingValue[1]==500", nu.iNeedSettingValue[1], 500);
        check_b("bNeedDosetValue[0] untouched ==false", nu.bNeedDosetValue[0], false);

        bool ret = nu.WriteSettingValue();  // stub: both pending writes "fail"
        check_b("WriteSettingValue() [[all]] -> false (stub, 2 pending writes both fail)", ret, false);
        check_i("RetWriteSettingValue[1] == 1 after failed flush", nu.RetWriteSettingValue[1], 1);
        check_i("RetWriteSettingValue[3] == 1 after failed flush", nu.RetWriteSettingValue[3], 1);
        check_i("RetWriteSettingValue[0] untouched (was never pending) == 0", nu.RetWriteSettingValue[0], 0);
        // Stub never succeeds -> bNeedDosetValue is only cleared on SUCCESS
        // (golden .cpp:277/:335); still true after a failed flush.
        check_b("bNeedDosetValue[1] still true (only cleared on success)", nu.bNeedDosetValue[1], true);
    }

    // =========================================================================
    // ClearError
    // golden EtherCAT/MyNUEC1.cpp:338-343
    // =========================================================================
    printf("\n-- ClearError --\n");
    {
        TMyNUEC1 nu(1, 5, 4);
        nu.SetSetIOValue(0, 1);
        nu.WriteSettingValue(0, 1);  // stub -> bNeedDosetValue[0] stays true, RetWriteSettingValue[0]=1
        check_b("pre-ClearError: bNeedDosetValue[0]==true",        nu.bNeedDosetValue[0], true);
        check_i("pre-ClearError: RetWriteSettingValue[0]==1",      nu.RetWriteSettingValue[0], 1);

        nu.ClearError();
        bool all_clear = true;
        for (int i = 0; i < 16; ++i)
        {
            if (nu.bNeedDosetValue[i] != false)     all_clear = false;
            if (nu.RetWriteSettingValue[i] != 0)    all_clear = false;
        }
        check_b("post-ClearError: bNeedDosetValue[]==false && RetWriteSettingValue[]==0 (all 16)", all_clear, true);
    }

    // =========================================================================
    // CheckAmplifierExist() [[all, summary text]]
    // golden EtherCAT/MyNUEC1.cpp:357-378.  Stub -> every single-amp check
    // returns false -> AmpCnt stays 0 -> mismatch branch formats the error text.
    // =========================================================================
    printf("\n-- CheckAmplifierExist() [[all]] summary text (golden .cpp:357-378) --\n");
    {
        TMyNUEC1 nu(1, 7, 8);  // MACID=7, Amplifier=8
        AnsiString s = nu.CheckAmplifierExist();
        check_s("CheckAmplifierExist(): stub -> AmpCnt=0 mismatch text", s, "NU-EC1[7] Amp is 0 less than 8");
    }
    {
        TMyNUEC1 nu(1, 7, 0);  // Amplifier=0 -> golden's `if(iAmplifier>0)` guard is false -> "OK"
        AnsiString s = nu.CheckAmplifierExist();
        check_s("CheckAmplifierExist(): iAmplifier==0 -> \"OK\" (golden .cpp:363/375)", s, "OK");
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
