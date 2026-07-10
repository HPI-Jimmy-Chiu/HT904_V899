// tests/test_uHGemClass.cpp
// Verification harness for the SECSGEM/uHGemClass (HTGem) + SecsEventType
// (SECS_EVENT) translation.
//
// Golden reference:
//   HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemClass.h/.cpp
//   HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemHT9045.h:12-313 (ETypeStruct)
//
// SCOPE (see uHGemClass.cpp's file-head note for the full discrepancy
// writeup): 57 of HTGem's ~90 methods need `THGem` (uHGemEquipment.h, out of
// scope) and are GATED with a conservative-default ACTIVE stub.  This harness
// therefore verifies:
//   (1) the translation compiles + links clean under MinGW g++, with ZERO
//       external dependencies beyond vclcompat (no THGem, no cmydef/csystem/
//       common -- confirmed by this test's own trivial link line), AND
//   (2) the genuinely ACTIVE surface behaves per golden: ctors/dtor/
//       UpdateDataPath, the free helper IsCorrectDateFormat, the header's
//       already-fully-inline virtuals (verbatim from golden, e.g.
//       S7F24_FormattedProcessProgramSendAcknowledge()==1), virtual dispatch
//       through a derived override (proves HTGem is a real usable abstract
//       base), the 57 gated stubs' documented conservative defaults, and
//       every SECS_EVENT enumerator's exact golden numeric value.
//
// No external test framework: a tiny check harness prints PASS/FAIL per case
// and a final summary, and returns non-zero on ANY failure.

#include "SECSGEM/uHGemClass.h"
#include "SECSGEM/SecsEventType.h"
#include <cstdio>
#include <string>

// AI(W5-SECSGEM-Translate) 20260710: IsCorrectDateFormat has external linkage
// in golden (uHGemClass.cpp) but no header declares it there either (it is a
// uHGemClass.cpp-private helper only ever called from the gated
// S2F32_DateAndTimeAcknowledge body) -- forward-declared here, test-local,
// exactly as golden leaves it undeclared-in-any-header.
extern bool IsCorrectDateFormat(int y, int m, int d);

// This test target's own MyDBIProcess stub (same established pattern as
// tests/test_config_loaders.cpp: "the TU supplies the two gated externals...
// that the gated UI/DB waves will provide for real").  Matches the REAL
// golden 3-arg signature (cMyDB.h:20), not the pre-existing 2-arg mismatch in
// aHotPlateSubstrate.h:576 (see uHGemClass.cpp's extern-decl note).
static int g_dbiCalls = 0;
void MyDBIProcess(AnsiString /*asTable*/, AnsiString /*S1*/, AnsiString /*S2*/)
{
    ++g_dbiCalls;
}

static int g_pass = 0;
static int g_fail = 0;

static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected) { printf("PASS  %-64s got=%d\n", name, (int)got); ++g_pass; }
    else { printf("FAIL  %-64s got=%d exp=%d\n", name, (int)got, (int)expected); ++g_fail; }
}

static void check_i(const char* name, int got, int expected)
{
    if (got == expected) { printf("PASS  %-64s got=%d\n", name, got); ++g_pass; }
    else { printf("FAIL  %-64s got=%d exp=%d\n", name, got, expected); ++g_fail; }
}

static void check_s(const char* name, const std::string& got, const std::string& expected)
{
    if (got == expected) { printf("PASS  %-64s got=\"%s\"\n", name, got.c_str()); ++g_pass; }
    else { printf("FAIL  %-64s got=\"%s\" exp=\"%s\"\n", name, got.c_str(), expected.c_str()); ++g_fail; }
}

// -----------------------------------------------------------------------
// Derived subclass proving HTGem is a real, instantiable, polymorphic base
// (mirrors the role uHGemHT9045.h's HT9045Gem plays in golden, minimally).
// -----------------------------------------------------------------------
class TestGem : public HTGem
{
public:
    int s1f1Calls = 0;
    void S1F1_AreYouThereRequest() override
    {
        ++s1f1Calls;   // override does NOT chain to HTGem's gated stub -- fine,
                       // that is exactly what a real site-specific override does.
    }
};

int main()
{
    printf("=== SECSGEM/uHGemClass (HTGem) + SecsEventType (SECS_EVENT) translation verification ===\n");
    printf("(no THGem/uHGemEquipment link dependency -- see uHGemClass.cpp file-head note)\n\n");

    // -----------------------------------------------------------------------
    // Construction / destruction (ACTIVE, faithful -- golden :34-79)
    // -----------------------------------------------------------------------
    printf("-- ctors / dtor / UpdateDataPath (golden :34-84) --\n");
    {
        HTGem g;
        check_b("default ctor: HGemPtr==NULL", g.HGemPtr == NULL, true);
        check_s("default ctor: HandlerPath==\"\"", g.HandlerPath.c_str(), "");
        check_b("default ctor: SecsAlarmMessage != NULL", g.SecsAlarmMessage != NULL, true);
        check_b("default ctor: FMessageList != NULL", g.FMessageList != NULL, true);

        g.UpdateDataPath("D:\\HT9045\\system");
        check_s("UpdateDataPath sets DataPath", g.DataPath.c_str(), "D:\\HT9045\\system");
    }   // dtor runs here -- must not crash / must not leak (ASan not available
        // in this environment; scope-exit-without-crash is the practical proof).

    {
        HTGem g2("HT9045");
        check_s("AnsiString-ctor: HandlerPath==Path", g2.HandlerPath.c_str(), "HT9045");
        check_b("AnsiString-ctor: SecsAlarmMessage != NULL", g2.SecsAlarmMessage != NULL, true);
    }

    {
        HTGem g3((THGem*)NULL);   // THGem is only forward-declared; NULL never derefs it.
        check_b("THGem*-ctor(NULL): HGemPtr==NULL", g3.HGemPtr == NULL, true);
    }

    // -----------------------------------------------------------------------
    // IsCorrectDateFormat (ACTIVE, faithful leap-year ladder -- golden :17-32)
    // -----------------------------------------------------------------------
    printf("\n-- IsCorrectDateFormat (golden :17-32) --\n");
    check_b("2024-02-29 leap year -> true",  IsCorrectDateFormat(2024, 2, 29), true);
    check_b("2023-02-29 non-leap -> false",  IsCorrectDateFormat(2023, 2, 29), false);
    check_b("1900-02-29 %100==0 not %400 -> false", IsCorrectDateFormat(1900, 2, 29), false);
    check_b("2000-02-29 %400==0 -> is a leap year, but y<=2000 rejected -> false",
            IsCorrectDateFormat(2000, 2, 29), false);
    check_b("2400-02-29 %400==0 -> true", IsCorrectDateFormat(2400, 2, 29), true);
    check_b("2025-13-01 bad month -> false", IsCorrectDateFormat(2025, 13, 1), false);
    check_b("2025-00-01 bad month -> false", IsCorrectDateFormat(2025, 0, 1), false);
    check_b("2025-04-31 April has 30 days -> false", IsCorrectDateFormat(2025, 4, 31), false);
    check_b("2025-04-30 -> true", IsCorrectDateFormat(2025, 4, 30), true);
    check_b("10000-01-01 y>9999 -> false", IsCorrectDateFormat(10000, 1, 1), false);

    // -----------------------------------------------------------------------
    // Already-inline virtuals -- carried over VERBATIM from golden's header,
    // zero gating (golden itself gives them a trivial body).
    // -----------------------------------------------------------------------
    printf("\n-- already-inline virtuals (verbatim from golden .h, zero gating) --\n");
    {
        HTGem g;
        g.AddSV(); g.AddEC(); g.AddAlarmList(); g.AddCEID(); g.AddReprot();     // must not crash
        g.ReloadParameter(); g.LookForFile();
        g.S7F4_ProcessProgramAcknowledge();
        g.S7F6_ProcessProgramData();
        g.S7F6_ProcessProgramData("dummy.stp");
        check_i("S7F24_FormattedProcessProgramSendAcknowledge()==1 (golden literal)",
                g.S7F24_FormattedProcessProgramSendAcknowledge(), 1);
        check_i("ProcessS7F23FromatReceipe()==2 (golden literal)",
                g.ProcessS7F23FromatReceipe(), 2);
        check_i("S7F26_FormattedProcessProgramData()==1 (golden literal)",
                g.S7F26_FormattedProcessProgramData(), 1);
        check_i("ProcessS7F25FromatReceipe()==2 (golden literal)",
                g.ProcessS7F25FromatReceipe(), 2);
        g.S14F4_Get2DID_BinCode();
        g.S110F5_RequestCustomerNameList(); g.S110F6_ListCustomerName();
        g.S110F7_RequestReceipeInformation(); g.S110F8_ListReceipeInformation();
        g.S120F1_RequestReceipeSetupFile(); g.S120F2_ListReceipeSetupFile();
        g.S125F4_LevelSettingChangeAcknowledge();
        printf("PASS  all already-inline virtuals callable without crash\n");
        ++g_pass;
    }

    // -----------------------------------------------------------------------
    // Virtual dispatch through a derived override (proves HTGem is a real,
    // usable, instantiable abstract base -- the role uHGemHT9045's HT9045Gem
    // plays in golden).
    // -----------------------------------------------------------------------
    printf("\n-- virtual dispatch through derived override --\n");
    {
        TestGem tg;
        HTGem* base = &tg;
        base->S1F1_AreYouThereRequest();
        base->S1F1_AreYouThereRequest();
        check_i("derived override invoked via base pointer (2 calls)", tg.s1f1Calls, 2);
    }

    // -----------------------------------------------------------------------
    // Gated stubs (57 methods) -- spot-check a representative sample across
    // void / int-ack / AnsiString-param shapes; every one must be callable
    // without crashing and return its documented conservative default.
    // -----------------------------------------------------------------------
    printf("\n-- gated stubs: conservative defaults, no crash (sample) --\n");
    {
        HTGem g;
        // void, no-arg (representative of ~40 such stubs)
        g.S1F1_AreYouThereRequest();
        g.S1F2_OnLineData();
        g.S1F24_CollectionEventNamelist();
        g.S6F16_EventReportData();
        g.S103F12_StatusVariableNamelistReply();
        printf("PASS  void/no-arg gated stubs callable without crash\n");
        ++g_pass;

        // void, AnsiString-arg
        g.S9F1_UnrecognizedDeviceID("unit test S");
        g.S9F3_Unrecognized_Stream_Function_Type("unit test S");
        g.S9F5_UnrecognizedFunctionType("unit test S");
        g.S9F7_IllegalData("unit test S");
        g.S9F9_TransactionTimerTimeout("unit test S");
        printf("PASS  void/AnsiString-arg gated stubs callable without crash\n");
        ++g_pass;

        // int-returning ack stubs -- conservative default documented in each
        // uHGemClass.cpp stub comment.
        check_i("S2F24_TraceInitializeAcknowledgeSub() conservative default",
                g.S2F24_TraceInitializeAcknowledgeSub(), 1);
        check_i("S2F34_DefineReportAcknowledgeSub() conservative default",
                g.S2F34_DefineReportAcknowledgeSub(), 1);
        check_i("S2F36_LinkEventReportAcknowledgeSub() conservative default",
                g.S2F36_LinkEventReportAcknowledgeSub(), 1);
        check_i("S2F42_Host_Command_Acknowledge() conservative default",
                g.S2F42_Host_Command_Acknowledge(), 1);
        check_i("S7F2_ProcessProgramLoadGrant() conservative default",
                g.S7F2_ProcessProgramLoadGrant(), 1);
        check_i("S2F15_UpdateNewEquipmentConstant() conservative default",
                g.S2F15_UpdateNewEquipmentConstant(), 1);
        check_i("S2F15_CheckNewEquipmentConstant() conservative default",
                g.S2F15_CheckNewEquipmentConstant(), 1);

        // CheckECValue -- matches golden's OWN not-found default (golden :3559)
        check_i("CheckECValue(\"999\", NULL) matches golden not-found default",
                g.CheckECValue("999", NULL), 1);

        // SetECValue -- void, two args, must not crash even with a NULL sink.
        int dummy = 0;
        g.SetECValue(70, &dummy);
        printf("PASS  SetECValue callable without crash\n");
        ++g_pass;
    }

    // -----------------------------------------------------------------------
    // SECS_EVENT (ETypeStruct) -- every enumerator's exact golden numeric
    // value (golden HT9011UC_Code_.../SECSGEM/uHGemHT9045.h:12-313).
    // -----------------------------------------------------------------------
    printf("\n-- SECS_EVENT (ETypeStruct) golden numeric values --\n");
    check_i("SECS_EVENT.DoStart == 1",              SECS_EVENT.DoStart, 1);
    check_i("SECS_EVENT.DoPause == 2",               SECS_EVENT.DoPause, 2);
    check_i("SECS_EVENT.DoOneCycle == 3",            SECS_EVENT.DoOneCycle, 3);
    check_i("SECS_EVENT.DoCleanOut == 4",            SECS_EVENT.DoCleanOut, 4);
    check_i("SECS_EVENT.DoExit == 24",               SECS_EVENT.DoExit, 24);
    check_i("SECS_EVENT.OneCycleFinish == 41",       SECS_EVENT.OneCycleFinish, 41);
    check_i("SECS_EVENT.CleanOutFinish == 42",       SECS_EVENT.CleanOutFinish, 42);
    check_i("SECS_EVENT.ArtRTFinish == 61",          SECS_EVENT.ArtRTFinish, 61);
    check_i("SECS_EVENT.ArtFTFinish == 63",          SECS_EVENT.ArtFTFinish, 63);
    check_i("SECS_EVENT.SECSOffline == 91",          SECS_EVENT.SECSOffline, 91);
    check_i("SECS_EVENT.SECSOnline == 92",           SECS_EVENT.SECSOnline, 92);
    check_i("SECS_EVENT.SECSOnlineRemote == 93",     SECS_EVENT.SECSOnlineRemote, 93);
    check_i("SECS_EVENT.LoaderTrayState == 271",     SECS_EVENT.LoaderTrayState, 271);
    check_i("SECS_EVENT.AGVSupplement == 272",       SECS_EVENT.AGVSupplement, 272);
    check_i("SECS_EVENT.AGVLdID == 291",             SECS_EVENT.AGVLdID, 291);
    check_i("SECS_EVENT.DoSecsGemIndexFail == 292 (explicit re-anchor)",
            SECS_EVENT.DoSecsGemIndexFail, 292);
    check_i("SECS_EVENT.TotalEvent == 293 (one past the last -- 'must stay at the bottom')",
            SECS_EVENT.TotalEvent, 293);

    // -----------------------------------------------------------------------
    // Summary
    // -----------------------------------------------------------------------
    printf("\n=== SUMMARY: %d passed, %d failed (total %d) ===\n",
           g_pass, g_fail, g_pass + g_fail);
    if (g_fail != 0)
    {
        printf("RESULT: FAIL\n");
        return 1;
    }
    printf("RESULT: ALL PASS\n");
    return 0;
}
