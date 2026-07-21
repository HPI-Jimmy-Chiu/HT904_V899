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
//   (1) the translation compiles + links clean under MinGW g++.  AI(W906-
//       SysModWire) 20260720: this test's link line grew a real THGem/
//       uHGemEquipment.cpp dependency this wave (8 more methods un-gated --
//       see uHGemClass.cpp's "INTEGRATE WAVE 3" note -- call real out-of-line
//       THGem methods) -- see tests/CMakeLists.txt's own updated comment on
//       this target and the new tests/test_uHGemClass_link_stubs.cpp
//       (supplies the two extra gated externals that dependency pulls in;
//       kept in a SEPARATE TU so this file itself stays untouched by that
//       plumbing), AND
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
// AI(W906-VCW1) 20260721: SetECValue's UN-GATED IsVCL==1 dynamic_cast
// cascade (below) needs the 6 VCL-widget stand-ins this header supplies --
// see its own file-head scope-boundary note.
#include "vclcompat/Controls.h"
#include <cstdio>
#include <cstring>   // strcpy (test-side buffer prep for SetECValue's void* sink)
#include <string>

// AI(W5-SECSGEM-Translate) 20260710: IsCorrectDateFormat has external linkage
// in golden (uHGemClass.cpp) but no header declares it there either (it is a
// uHGemClass.cpp-private helper only ever called from the gated
// S2F32_DateAndTimeAcknowledge body) -- forward-declared here, test-local,
// exactly as golden leaves it undeclared-in-any-header.
extern bool IsCorrectDateFormat(int y, int m, int d);

// AI(W906-FastcallFix) 20260720: this test's own local MyDBIProcess (3-arg,
// __fastcall) stub -- previously here, matching the REAL golden 3-arg
// signature (cMyDB.h:20) -- is REMOVED. SECSGEM/uHGemEquipment.cpp now
// supplies the real, externally-linkable, __fastcall-decorated definition of
// this overload (moved out of an anonymous namespace and given __fastcall
// this same wave), and this target's RESCAN link group (tests/CMakeLists.txt)
// already links ht9045_secsgem, the library that carries it, to satisfy
// this file's own uHGemClass.cpp:310 extern+__fastcall declaration. Keeping
// the local stub would now be a duplicate-definition link error, not a
// missing-symbol fix (2026-07-20 audit, AUDIT_fastcall_tree.md finding 1).
// The removed stub's call counter (g_dbiCalls) was incremented but never
// read/asserted anywhere in this file -- confirmed before deletion that no
// PASS/FAIL check depended on it, so no coverage is lost by this removal.
//
// NOTE: this edit knowingly breaks this file's own established "stays
// byte-for-byte unchanged" convention (see tests/test_uHGemClass_link_stubs.cpp's
// file-head note, citing design brief Sec.9 item 5) -- that convention was
// adopted to avoid touching this file for an unrelated (SysModWire) reason;
// it does not anticipate a defect fix that requires deleting code FROM this
// file itself, which cannot be done any other way (a stub left in place here
// collides at link time with the production definition this fix adds
// elsewhere). Flagged explicitly rather than silently overridden.

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
    printf("(as of W906-SysModWire: real THGem/uHGemEquipment link dependency -- see this file's own updated file-head note)\n\n");

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
    // Gated stubs (47 of the original 57 remain gated after the 20260713
    // integrate wave -- see uHGemClass.cpp's file-head "INTEGRATE WAVE" note)
    // -- spot-check a representative sample across void / int-ack /
    // AnsiString-param shapes; every one must be callable without crashing
    // and return its documented conservative default.
    // -----------------------------------------------------------------------
    printf("\n-- gated stubs: conservative defaults, no crash (sample) --\n");
    {
        HTGem g;
        // void, no-arg (representative of the still-gated void/no-arg stubs)
        // AI(W906-SysModWire) 20260720: S1F1_AreYouThereRequest/S1F2_OnLineData
        // MOVED OUT of this sample (same "moved out" precedent as the
        // S2F42/CheckECValue/S7F2/S2F15_Update/S2F15_Check comment above) --
        // this wave un-gated both (see uHGemClass.cpp's "INTEGRATE WAVE 3"
        // note). They are NO LONGER blanket-safe on a default-constructed `g`:
        // both now dereference `HGemPtr` for real THGem state
        // (bS1F2_OnLineData / CheckSFFormatOnlyHead+GemMDLN+GemSOFTREV), and
        // `g`'s HGemPtr is NULL here (golden pre-AddSV UB window, deliberately
        // un-guarded -- see design brief risk R8). This TU only
        // forward-declares THGem (cannot construct one to wire HGemPtr for
        // real), so their real behavior is exercised over in
        // tests/test_uHGemEquipment.cpp instead, where a real THGem instance
        // exists (design brief's W2/W3/W4/W5 tests). Replaced here with two
        // more still-gated void/no-arg stubs to keep the sample's width.
        g.S1F4_SelectedStatusReply();
        g.S1F12_StatusVariableNamelistReply();
        g.S1F24_CollectionEventNamelist();
        g.S6F16_EventReportData();
        g.S103F12_StatusVariableNamelistReply();
        printf("PASS  void/no-arg gated stubs callable without crash\n");
        ++g_pass;

        // int-returning ack stubs -- conservative default documented in each
        // uHGemClass.cpp stub comment. (S2F42_Host_Command_Acknowledge,
        // CheckECValue, S7F2_ProcessProgramLoadGrant,
        // S2F15_UpdateNewEquipmentConstant, S2F15_CheckNewEquipmentConstant
        // moved out of this sample -- see the UN-GATED section below, they
        // are no longer blanket stubs.)
        check_i("S2F24_TraceInitializeAcknowledgeSub() conservative default",
                g.S2F24_TraceInitializeAcknowledgeSub(), 1);
        // AI(W906-AlarmReportAck) 20260721: S2F34_DefineReportAcknowledgeSub/
        // S2F36_LinkEventReportAcknowledgeSub MOVED OUT of this sample -- same
        // "moved out" precedent as S1F1/S1F2 above (this wave un-gated both,
        // see uHGemClass.cpp's "INTEGRATE WAVE 5" note). They are NO LONGER
        // blanket-safe on a default-constructed `g`: both now dereference
        // `HGemPtr->slTempReportID`/`HGemPtr->slTempCeID` for real THGem
        // state, and `g`'s HGemPtr is NULL here (golden pre-AddSV UB window,
        // deliberately un-guarded -- see design brief risk R8). This TU only
        // forward-declares THGem (cannot construct one to wire HGemPtr for
        // real), so their real behavior is exercised over in
        // tests/test_uHGemEquipment.cpp instead, where a real THGem instance
        // exists. NOT replaced with 2 more int-returning stubs here (unlike
        // the void/no-arg sample's own replacement above) -- every one of the
        // 15 still-gated methods left in uHGemClass.cpp is void-returning
        // (grepped), so no like-for-like int-ack replacement exists; the
        // int-ack shape stays represented by S2F24_TraceInitializeAcknowledgeSub
        // above.

        // SetECValue -- void, two args, must not crash even with a NULL sink.
        // UN-GATED as of AI(W906-VCW1) 20260721 (see the dedicated section
        // below for real dispatch coverage); still exercised once here with
        // an unregistered ECID (70) to confirm the not-found path (i<0) is
        // a harmless no-op, matching golden's own falls-off-the-end-of-the-
        // if body for that case.
        int dummy = 0;
        g.SetECValue(70, &dummy);
        printf("PASS  SetECValue(unregistered ECID) callable without crash\n");
        ++g_pass;
    }

    // -----------------------------------------------------------------------
    // AI(W906-VCW1) 20260721: SetECValue UN-GATED -- vclcompat/Controls.h
    // dynamic_cast dispatch coverage (golden uHGemClass.cpp:2682-2882).
    // SCOPE REMINDER: this proves the CAST-DISPATCH CODE compiles and
    // dispatches correctly against real instances of the 6 new stand-in
    // types constructed BY THIS TEST -- it does NOT mean any real widget-
    // backed EC from uHGemHT9045_EC.cpp is functional (none exist yet; see
    // vclcompat/Controls.h's own file-head note).
    // -----------------------------------------------------------------------
    printf("\n-- SetECValue: UN-GATED VCL-widget dynamic_cast dispatch --\n");
    {
        // (c) each of the 6 new stand-in types' dynamic_cast against an
        // UNRELATED sibling type correctly resolves to nullptr -- the "safe
        // no-op for unbacked types" property this whole wave rests on,
        // confirmed explicitly here rather than just relying on it
        // compiling. Also confirms TStringList (pre-existing, gained its
        // TObject base via this wave's own R1 step) is a correct match/
        // non-match partner too.
        TPanel panelInstance;
        TObject *asPanelObj = &panelInstance;
        check_b("dynamic_cast<TPanel*> matches a real TPanel instance",
                dynamic_cast<TPanel*>(asPanelObj) != NULL, true);
        check_b("dynamic_cast<TCustomEdit*> vs TPanel instance -> nullptr",
                dynamic_cast<TCustomEdit*>(asPanelObj) == NULL, true);
        check_b("dynamic_cast<TComboBox*> vs TPanel instance -> nullptr",
                dynamic_cast<TComboBox*>(asPanelObj) == NULL, true);
        check_b("dynamic_cast<TLabel*> vs TPanel instance -> nullptr",
                dynamic_cast<TLabel*>(asPanelObj) == NULL, true);
        check_b("dynamic_cast<TCheckBox*> vs TPanel instance -> nullptr",
                dynamic_cast<TCheckBox*>(asPanelObj) == NULL, true);
        check_b("dynamic_cast<TRadioGroup*> vs TPanel instance -> nullptr",
                dynamic_cast<TRadioGroup*>(asPanelObj) == NULL, true);
        check_b("dynamic_cast<TStringList*> vs TPanel instance -> nullptr",
                dynamic_cast<TStringList*>(asPanelObj) == NULL, true);

        TStringList realList;
        TObject *asListObj = &realList;
        check_b("dynamic_cast<TStringList*> matches a real TStringList instance (R1)",
                dynamic_cast<TStringList*>(asListObj) != NULL, true);
        check_b("dynamic_cast<TPanel*> vs TStringList instance -> nullptr",
                dynamic_cast<TPanel*>(asListObj) == NULL, true);

        // (a) SetECValue writing to a REAL TStringList*-backed EC (VCL_NAME
        // =="1") -- the one widget type with a real instance possible today
        // via SetECDataPointer's TObject* overload (R1 gave TStringList the
        // `: public TObject` base this dynamic_cast needs). Registered by
        // directly populating HTGem's own SvEcReg parallel lists (matching
        // exactly what SetECValue itself reads) rather than through
        // SetECDataPointer, to sidestep that overload's own documented
        // registration-time ASCII-hazard (see SecsSvEcRegistration.cpp's
        // file-head note) -- irrelevant to what THIS test is verifying.
        HTGem g;
        TStringList *slEc = new TStringList();
        g.SvEcReg.EC_ID->Add(AnsiString(200));
        g.SvEcReg.EC_TYPE->Add(AnsiString((int)HType.ASCII_TYPE));
        g.SvEcReg.EC_NAME->Add("TestStrEC");
        g.SvEcReg.EC_UNIT->Add("unit");
        g.SvEcReg.EC_Ptr->Add((void*)slEc);
        g.SvEcReg.EC_VCL_NAME->Add("1");

        char strBuf[64];
        strcpy(strBuf, "hello,world");
        g.SetECValue(200, strBuf);
        check_s("SetECValue: TStringList-backed EC (StringListPtr branch) -> CommaText written",
                slEc->GetCommaText().str(), "hello,world");

        // Second widget type, end to end through the SAME real SetECValue
        // dispatch -- proves the dynamic_cast cascade selects the CORRECT
        // branch out of all 7 candidates (not just "compiles"), and that
        // TCheckBox's ->Checked write-through (S!="0") is byte-for-byte
        // golden's own logic.
        TCheckBox cbEc;
        g.SvEcReg.EC_ID->Add(AnsiString(202));
        g.SvEcReg.EC_TYPE->Add(AnsiString((int)HType.BOOLEAN_TYPE));
        g.SvEcReg.EC_NAME->Add("TestCheckEC");
        g.SvEcReg.EC_UNIT->Add("unit");
        g.SvEcReg.EC_Ptr->Add((void*)&cbEc);
        g.SvEcReg.EC_VCL_NAME->Add("1");
        unsigned char boolByte = 1;   // non-"0" byte -> S!="0" -> Checked=true
        g.SetECValue(202, &boolByte);
        check_b("SetECValue: TCheckBox-backed EC (CheckBoxPtr branch) -> Checked==true",
                cbEc.Checked, true);

        // (b) SetECValue's IsVCL==2 bare-return asymmetry (golden :2718-2721,
        // preserved verbatim): an EC registered via the AnsiString* overload
        // is host-READABLE (GetECDataValue) but silently NON-WRITABLE via
        // SetECValue. Assert the stored value is genuinely UNCHANGED after a
        // SetECValue call, proving the preserved golden bug (this
        // translation did not silently "fix" it into an actual write-through).
        AnsiString ecBacking = "original";
        g.SvEcReg.EC_ID->Add(AnsiString(201));
        g.SvEcReg.EC_TYPE->Add(AnsiString((int)HType.ASCII_TYPE));
        g.SvEcReg.EC_NAME->Add("TestAnsiEC");
        g.SvEcReg.EC_UNIT->Add("unit");
        g.SvEcReg.EC_Ptr->Add((void*)&ecBacking);
        g.SvEcReg.EC_VCL_NAME->Add("2");
        char strBuf2[64];
        strcpy(strBuf2, "attempted-overwrite");
        g.SetECValue(201, strBuf2);
        check_s("SetECValue: IsVCL==2 (AnsiString*-backed) EC -> bare return, value UNCHANGED (golden asymmetry)",
                ecBacking.str(), "original");

        // Non-VCL (raw-pointer, VCL_NAME=="0") scalar write-through still
        // works exactly as before this wave (unaffected by the IsVCL==1/2
        // additions above) -- spot-check one scalar type end to end.
        int rawEcValue = 0;
        g.SvEcReg.EC_ID->Add(AnsiString(203));
        g.SvEcReg.EC_TYPE->Add(AnsiString((int)HType.INT_4_TYPE));
        g.SvEcReg.EC_NAME->Add("TestRawEC");
        g.SvEcReg.EC_UNIT->Add("unit");
        g.SvEcReg.EC_Ptr->Add((void*)&rawEcValue);
        g.SvEcReg.EC_VCL_NAME->Add("0");
        int newRawValue = 777;
        g.SetECValue(203, &newRawValue);
        check_i("SetECValue: raw-pointer (VCL_NAME==\"0\") EC -> *Ptr=*Ptr2 scalar write-through",
                rawEcValue, 777);
    }

    // -----------------------------------------------------------------------
    // UN-GATED (10 methods, integrate wave 20260713): S9F1/S9F3/S9F5/S9F7/
    // S9F9_*, S2F24_TraceInitializeAcknowledge, S2F26_DiagnosticLoopbackData,
    // S2F42_Host_Command_Acknowledge, S2F44_ResetSpoolingAcknowledge,
    // CheckECValue. These now call HTGem's own `WireCodec`/`SvEcReg` engine
    // members for real instead of returning a blanket conservative default --
    // verified here via WireCodec/SvEcReg OBSERVABLE STATE, not just
    // "does not crash". See uHGemClass.cpp's file-head "INTEGRATE WAVE" note
    // for exactly why these 10 (of 57) and not more.
    //
    // UN-GATED WAVE 2 (3 MORE, integrate wave 20260716, 13/57 total):
    // S7F2_ProcessProgramLoadGrant, S2F15_UpdateNewEquipmentConstant,
    // S2F15_CheckNewEquipmentConstant -- unblocked once SecsWireCodec gained
    // GetDataItemLenAndTypeAndDelete + SendInvalidDataMessageToHost (see
    // uHGemClass.cpp's "INTEGRATE WAVE 2" note). IMPORTANT: unlike the 47
    // still-gated stubs, these 3 (like the first 10) no longer return a
    // blanket conservative default on ANY input -- their return value now
    // genuinely depends on what is seeded into WireCodec.SReceiveData, so
    // each gets its own format-error-path AND success-path case below
    // (see uHGemClass.cpp's "INTEGRATE WAVE 2" note for exactly why these 3
    // and not more).
    // -----------------------------------------------------------------------
    printf("\n-- UN-GATED methods: real WireCodec/SvEcReg behavior --\n");
    {
        // S9F1/S9F3/S9F5/S9F7/S9F9 -- identical 4-call shape (StringOut +
        // InitLocalHead(9,<F>,0) + DataItemOut(ASCII,S) + SendLocalData).
        // Verify the real StringOut side effect (LogDataString gains the
        // exact message) and the real InitLocalHead side effect (Local.
        // MessageID_S/F set from the S,F code) -- a blanket stub could do
        // neither.
        HTGem g;
        g.S9F7_IllegalData("format error test");
        check_i("S9F7_IllegalData: WireCodec.LogDataString gained a line",
                g.WireCodec.LogDataString->Count, 1);
        check_s("S9F7_IllegalData: LogDataString[0] == the message",
                g.WireCodec.LogDataString->GetString(0).c_str(), "format error test");
        check_i("S9F7_IllegalData: Local.MessageID_S == 9 (InitLocalHead(9,7,0))",
                g.WireCodec.Local.MessageID_S, 9);
        check_i("S9F7_IllegalData: Local.MessageID_F == 7",
                g.WireCodec.Local.MessageID_F, 7);

        g.S9F1_UnrecognizedDeviceID("dev id test");
        check_i("S9F1_UnrecognizedDeviceID: Local.MessageID_F == 1", g.WireCodec.Local.MessageID_F, 1);
        g.S9F3_Unrecognized_Stream_Function_Type("s9f3 test");
        check_i("S9F3_Unrecognized_Stream_Function_Type: Local.MessageID_F == 3", g.WireCodec.Local.MessageID_F, 3);
        g.S9F5_UnrecognizedFunctionType("s9f5 test");
        check_i("S9F5_UnrecognizedFunctionType: Local.MessageID_F == 5", g.WireCodec.Local.MessageID_F, 5);
        g.S9F9_TransactionTimerTimeout("s9f9 test");
        check_i("S9F9_TransactionTimerTimeout: Local.MessageID_F == 9", g.WireCodec.Local.MessageID_F, 9);

        // S2F44_ResetSpoolingAcknowledge -- pure InitLocalHead+DataItemOut
        // burst; verify the wire cursor actually advanced past the bare
        // 14-byte header (proves DataItemOut really ran, not just InitLocalHead).
        g.S2F44_ResetSpoolingAcknowledge();
        check_i("S2F44_ResetSpoolingAcknowledge: Local.MessageID_S == 2", g.WireCodec.Local.MessageID_S, 2);
        check_i("S2F44_ResetSpoolingAcknowledge: Local.MessageID_F == 44", g.WireCodec.Local.MessageID_F, 44);
        check_b("S2F44_ResetSpoolingAcknowledge: LocalLength_4 advanced past bare header",
                g.WireCodec.LocalLength_4 > 14, true);

        // S2F26_DiagnosticLoopbackData -- format-error path: seed a
        // NON-BINARY item (ASCII) so GetDataItemLenAndType's peek is fully
        // deterministic (Type gets a real value, no uninitialized-read
        // risk) and the `Type==HType.BINARY_TYPE` check is reliably false ->
        // falls to S9F7_IllegalData.
        {
            HTGem g2;
            g2.WireCodec.SReceiveData->Add(AnsiString((int)HType.ASCII_TYPE));
            g2.WireCodec.SReceiveData->Add(AnsiString(3));
            g2.S2F26_DiagnosticLoopbackData();
            check_i("S2F26 with non-BINARY peeked item -> falls to S9F7 (MessageID_F==7)",
                    g2.WireCodec.Local.MessageID_F, 7);
        }
        // S2F26_DiagnosticLoopbackData -- success path: seed a real
        // BINARY_TYPE item (Type token, len token, len data-value tokens) so
        // GetDataItemLenAndType + DataItemIn both succeed -> real
        // InitLocalHead(2,26,0)+DataItemOut(BINARY)+SendLocalData echo.
        {
            HTGem g3;
            g3.WireCodec.SReceiveData->Add(AnsiString((int)HType.BINARY_TYPE));
            g3.WireCodec.SReceiveData->Add(AnsiString(3));
            g3.WireCodec.SReceiveData->Add(AnsiString(0xAA));
            g3.WireCodec.SReceiveData->Add(AnsiString(0xBB));
            g3.WireCodec.SReceiveData->Add(AnsiString(0xCC));
            g3.S2F26_DiagnosticLoopbackData();
            check_i("S2F26 with seeded BINARY item -> real echo (MessageID_S==2)",
                    g3.WireCodec.Local.MessageID_S, 2);
            check_i("S2F26 with seeded BINARY item -> real echo (MessageID_F==26)",
                    g3.WireCodec.Local.MessageID_F, 26);
        }

        // S2F42_Host_Command_Acknowledge -- empty SReceiveData -> DataItemIn
        // returns -1 immediately (SReceiveData->Count==0 is the very FIRST
        // check, before any local variable is touched -- deterministic, no
        // uninitialized-read risk) -> HCACK=3 (golden's own quirk: this path
        // never calls InitLocalHead first, preserved verbatim).
        {
            HTGem g4;
            check_i("S2F42 with empty SReceiveData -> HCACK=3 (format error, golden quirk)",
                    g4.S2F42_Host_Command_Acknowledge(), 3);
        }
        // S2F42_Host_Command_Acknowledge -- seeded success path: <L,2 <A,3
        // "ABC">> so DataItemIn(2,LIST,NULL) then the ASCII decode both
        // succeed -> HCACK=1 (golden's own dead if(HCACK==0)/else split,
        // where HCACK is unconditionally 1 either way, preserved verbatim).
        {
            HTGem g5;
            g5.WireCodec.SReceiveData->Add(AnsiString((int)HType.LIST_TYPE));
            g5.WireCodec.SReceiveData->Add(AnsiString(2));
            g5.WireCodec.SReceiveData->Add(AnsiString((int)HType.ASCII_TYPE));
            g5.WireCodec.SReceiveData->Add(AnsiString(3));
            g5.WireCodec.SReceiveData->Add(AnsiString("ABC"));
            check_i("S2F42 with seeded LIST+ASCII command -> HCACK=1 (accept)",
                    g5.S2F42_Host_Command_Acknowledge(), 1);
        }

        // S2F24_TraceInitializeAcknowledge -- thin wrapper: its own Sub()
        // sibling stays gated (returns its conservative default, 1), so this
        // always takes the `else` branch -> a real LocalAcknowledge(2,24,1)
        // burst (InitLocalHead+DataItemOut+SendLocalData, all via WireCodec).
        {
            HTGem g6;
            g6.S2F24_TraceInitializeAcknowledge();
            check_i("S2F24_TraceInitializeAcknowledge: Local.MessageID_S == 2", g6.WireCodec.Local.MessageID_S, 2);
            check_i("S2F24_TraceInitializeAcknowledge: Local.MessageID_F == 24", g6.WireCodec.Local.MessageID_F, 24);
        }

        // CheckECValue -- not-found path matches golden's own default (1),
        // now reached via a REAL SvEcReg.EC_ID->IndexOf lookup instead of a
        // blanket stub -- proved by ALSO exercising the found/in-range/
        // out-of-range paths, which a blanket stub could never do.
        check_i("CheckECValue(\"999\", NULL): not registered -> golden not-found default",
                g.CheckECValue("999", NULL), 1);

        int ecRaw = 50;
        g.SvEcReg.SetECDataPointer(AnsiString("100"), HType.INT_4_TYPE, "TestEC", "unit",
                                    (void*)&ecRaw, 0, 100, 50, "remark");
        int inRange = 30, outHigh = 999, outLow = -5;
        check_i("CheckECValue: 30 in [0,100] -> 0 (ok)", g.CheckECValue("100", &inRange), 0);
        check_i("CheckECValue: 999 > max 100 -> 3 (out of range)", g.CheckECValue("100", &outHigh), 3);
        check_i("CheckECValue: -5 < min 0 -> 3 (out of range)", g.CheckECValue("100", &outLow), 3);

        // -------------------------------------------------------------------
        // S7F2_ProcessProgramLoadGrant (golden uHGemClass.cpp:2081-2113,
        // UN-GATED 20260716) -- format-error path (empty SReceiveData):
        // WireCodec.DataItemIn(2,LIST_TYPE,NULL) hits SReceiveData->Count==0
        // -> DataItemInSub's very first check -> -1 -> falls straight to
        // S9F7_IllegalData("S7,F1 Data Format error !!!") -> return 0.
        // -------------------------------------------------------------------
        {
            HTGem g7;
            int ret = g7.S7F2_ProcessProgramLoadGrant();
            check_i("S7F2 with empty SReceiveData -> 0 (format error)", ret, 0);
            check_i("S7F2 format error: Local.MessageID_S == 9 (S9F7_IllegalData)",
                    g7.WireCodec.Local.MessageID_S, 9);
            check_i("S7F2 format error: Local.MessageID_F == 7",
                    g7.WireCodec.Local.MessageID_F, 7);
        }
        // Success path: <L,2 <A,3 "ABC"> <U1,1 42>> -- a well-formed PPGNT
        // body (PPID="ABC", a 1-byte numeric LENGTH item of any of the 8
        // accepted numeric types).
        {
            HTGem g8;
            g8.WireCodec.SReceiveData->Add(AnsiString((int)HType.LIST_TYPE));
            g8.WireCodec.SReceiveData->Add(AnsiString(2));
            g8.WireCodec.SReceiveData->Add(AnsiString((int)HType.ASCII_TYPE));
            g8.WireCodec.SReceiveData->Add(AnsiString(3));
            g8.WireCodec.SReceiveData->Add(AnsiString("ABC"));
            g8.WireCodec.SReceiveData->Add(AnsiString((int)HType.UINT_1_TYPE));
            g8.WireCodec.SReceiveData->Add(AnsiString(1));
            int ret = g8.S7F2_ProcessProgramLoadGrant();
            check_i("S7F2 with well-formed PPGNT body -> 1 (grant)", ret, 1);
            check_i("S7F2 grant: Local.MessageID_S == 7 (LocalAcknowledge(7,2,0))",
                    g8.WireCodec.Local.MessageID_S, 7);
            check_i("S7F2 grant: Local.MessageID_F == 2",
                    g8.WireCodec.Local.MessageID_F, 2);
            check_i("S7F2 grant: fully consumes its seeded burst",
                    g8.WireCodec.SReceiveData->Count, 0);
        }

        // -------------------------------------------------------------------
        // S2F15_UpdateNewEquipmentConstant (golden uHGemClass.cpp:2884-3024,
        // UN-GATED 20260716).
        // -------------------------------------------------------------------
        {
            // Format-error path: empty SReceiveData -> GetDataItemLenAndTypeAndDelete
            // returns -2 (Count<2) -> outer if fails -> return -1.
            HTGem g9;
            check_i("S2F15_Update with empty SReceiveData -> -1", g9.S2F15_UpdateNewEquipmentConstant(), -1);
        }
        {
            // EClen==0 (an empty <L,0>) -- golden's for-loop simply never
            // runs; falls straight through to ReloadParameter()+return 0.
            HTGem g10;
            g10.WireCodec.SReceiveData->Add(AnsiString((int)HType.LIST_TYPE));
            g10.WireCodec.SReceiveData->Add(AnsiString(0));
            check_i("S2F15_Update with <L,0> (no ECs) -> 0 (success, no-op)",
                    g10.S2F15_UpdateNewEquipmentConstant(), 0);
        }
        {
            // Full single-EC round trip: <L,1 <L,2 <A,3 "100"> <U1,1 42>>> --
            // one EC (ECID="100") set to a UINT_1 value of 42. SetECValue
            // itself is STILL gated (no observable side effect from the set
            // itself), so this test's oracle is that WireCodec correctly
            // walks/consumes the ENTIRE nested burst without misparsing --
            // proved by SReceiveData->Count==0 afterward -- and the function
            // still returns golden's success code (0).
            HTGem g11;
            g11.WireCodec.SReceiveData->Add(AnsiString((int)HType.LIST_TYPE));   // outer <L,1
            g11.WireCodec.SReceiveData->Add(AnsiString(1));
            g11.WireCodec.SReceiveData->Add(AnsiString((int)HType.LIST_TYPE));   // per-EC <L,2
            g11.WireCodec.SReceiveData->Add(AnsiString(2));
            g11.WireCodec.SReceiveData->Add(AnsiString((int)HType.ASCII_TYPE));  // ECID
            g11.WireCodec.SReceiveData->Add(AnsiString(3));
            g11.WireCodec.SReceiveData->Add(AnsiString("100"));
            g11.WireCodec.SReceiveData->Add(AnsiString((int)HType.UINT_1_TYPE)); // EC value
            g11.WireCodec.SReceiveData->Add(AnsiString(1));
            g11.WireCodec.SReceiveData->Add(AnsiString(42));
            int ret = g11.S2F15_UpdateNewEquipmentConstant();
            check_i("S2F15_Update with 1 well-formed UINT_1 EC -> 0 (success)", ret, 0);
            check_i("S2F15_Update fully consumes its seeded burst",
                    g11.WireCodec.SReceiveData->Count, 0);
        }
        {
            // Trailing "error format" else-branch: EC value Type byte
            // matches none of golden's known HType constants -> falls to
            // `WireCodec.SendInvalidDataMessageToHost("error format"); return -1;`
            // (0x01 is not any HType.*_TYPE literal -- see SecsWireCodec.cpp's
            // g_HTypeInit literal table).
            HTGem g12;
            g12.WireCodec.SReceiveData->Add(AnsiString((int)HType.LIST_TYPE));
            g12.WireCodec.SReceiveData->Add(AnsiString(1));
            g12.WireCodec.SReceiveData->Add(AnsiString((int)HType.LIST_TYPE));
            g12.WireCodec.SReceiveData->Add(AnsiString(2));
            g12.WireCodec.SReceiveData->Add(AnsiString((int)HType.ASCII_TYPE));
            g12.WireCodec.SReceiveData->Add(AnsiString(3));
            g12.WireCodec.SReceiveData->Add(AnsiString("100"));
            g12.WireCodec.SReceiveData->Add(AnsiString(1));    // unrecognized Type byte
            g12.WireCodec.SReceiveData->Add(AnsiString(1));
            int ret = g12.S2F15_UpdateNewEquipmentConstant();
            check_i("S2F15_Update with unrecognized EC value Type -> -1 (error format)", ret, -1);
            check_i("S2F15_Update error format: Local.MessageID_S == 9 (SendInvalidDataMessageToHost)",
                    g12.WireCodec.Local.MessageID_S, 9);
            check_i("S2F15_Update error format: Local.MessageID_F == 7",
                    g12.WireCodec.Local.MessageID_F, 7);
        }

        // -------------------------------------------------------------------
        // S2F15_CheckNewEquipmentConstant (golden uHGemClass.cpp:3026-3190,
        // UN-GATED 20260716).
        // -------------------------------------------------------------------
        {
            // Format-error path: empty SReceiveData -> -1 (same guard as Update).
            HTGem g13;
            check_i("S2F15_Check with empty SReceiveData -> -1", g13.S2F15_CheckNewEquipmentConstant(), -1);
        }
        {
            // EClen==0: UNLIKE Update, Check has its own explicit
            // `if(EClen<1) return -1;` guard (golden's own asymmetry between
            // the two siblings, preserved verbatim).
            HTGem g14;
            g14.WireCodec.SReceiveData->Add(AnsiString((int)HType.LIST_TYPE));
            g14.WireCodec.SReceiveData->Add(AnsiString(0));
            check_i("S2F15_Check with <L,0> (EClen<1) -> -1 (golden's own guard)",
                    g14.S2F15_CheckNewEquipmentConstant(), -1);
        }
        {
            // Full single-EC round trip, in-range value -> CheckECValue's
            // real registered-bounds check (via SvEcReg), not a stub --
            // proves this un-gated method actually reaches CheckECValue.
            HTGem g15;
            int ecRaw2 = 50;
            g15.SvEcReg.SetECDataPointer(AnsiString("200"), HType.INT_4_TYPE, "TestEC2", "unit",
                                          (void*)&ecRaw2, 0, 100, 50, "remark2");
            g15.WireCodec.SReceiveData->Add(AnsiString((int)HType.LIST_TYPE));
            g15.WireCodec.SReceiveData->Add(AnsiString(1));
            g15.WireCodec.SReceiveData->Add(AnsiString((int)HType.LIST_TYPE));
            g15.WireCodec.SReceiveData->Add(AnsiString(2));
            g15.WireCodec.SReceiveData->Add(AnsiString((int)HType.ASCII_TYPE));
            g15.WireCodec.SReceiveData->Add(AnsiString(3));
            g15.WireCodec.SReceiveData->Add(AnsiString("200"));
            g15.WireCodec.SReceiveData->Add(AnsiString((int)HType.INT_4_TYPE));
            g15.WireCodec.SReceiveData->Add(AnsiString(1));
            g15.WireCodec.SReceiveData->Add(AnsiString(30));   // in [0,100]
            int ret = g15.S2F15_CheckNewEquipmentConstant();
            check_i("S2F15_Check with in-range EC value -> 0 (via real CheckECValue)", ret, 0);
        }
        {
            // Same EC, out-of-range value -> CheckECValue's real bounds check
            // returns 3, and S2F15_CheckNewEquipmentConstant propagates it
            // immediately (`if(ret!=0) return ret;`).
            HTGem g16;
            int ecRaw3 = 50;
            g16.SvEcReg.SetECDataPointer(AnsiString("200"), HType.INT_4_TYPE, "TestEC2", "unit",
                                          (void*)&ecRaw3, 0, 100, 50, "remark2");
            g16.WireCodec.SReceiveData->Add(AnsiString((int)HType.LIST_TYPE));
            g16.WireCodec.SReceiveData->Add(AnsiString(1));
            g16.WireCodec.SReceiveData->Add(AnsiString((int)HType.LIST_TYPE));
            g16.WireCodec.SReceiveData->Add(AnsiString(2));
            g16.WireCodec.SReceiveData->Add(AnsiString((int)HType.ASCII_TYPE));
            g16.WireCodec.SReceiveData->Add(AnsiString(3));
            g16.WireCodec.SReceiveData->Add(AnsiString("200"));
            g16.WireCodec.SReceiveData->Add(AnsiString((int)HType.INT_4_TYPE));
            g16.WireCodec.SReceiveData->Add(AnsiString(1));
            g16.WireCodec.SReceiveData->Add(AnsiString(999));  // > max 100
            int ret = g16.S2F15_CheckNewEquipmentConstant();
            check_i("S2F15_Check with out-of-range EC value -> 3 (via real CheckECValue)", ret, 3);
        }
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
