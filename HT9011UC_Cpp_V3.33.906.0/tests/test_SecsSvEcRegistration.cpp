// tests/test_SecsSvEcRegistration.cpp
// Verification harness for the SECSGEM/SecsSvEcRegistration translation
// (THGem's SV/EC registration API: SetSVDataPointer x4 / SetECDataPointer x4
// / GetECDataValue).
//
// Golden reference:
//   HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.h:647-670
//   HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp
//     (SetSVDataPointer x4 :5648-5730, SetECDataPointer x4 :5732-6025,
//      GetECDataValue :3650-3925)
//
// SCOPE: no THGem/VCL-form/socket/grid dependency -- this test links
// SecsSvEcRegistration.cpp (+ SecsWireCodec.cpp, for the shared HType global)
// against vclcompat ONLY.
//
// SAFETY NOTE (see SecsSvEcRegistration.cpp's own GetECDataValue file-head
// comment): the AnsiString*/TObject* overloads have a documented, preserved-
// verbatim golden hazard when Type==ASCII_TYPE (registration-time read-back
// reinterprets the stored pointer's own bytes as raw char* text). This test
// deliberately never registers via those two overloads with Type==ASCII_TYPE
// -- every AnsiString*/TObject* registration below uses a numeric Type
// (INT_4_TYPE), which only ever dereferences a few in-bounds bytes of the
// pointee as a scalar (safe, if semantically meaningless/unasserted) rather
// than scanning for a NUL terminator through arbitrary memory.
//
// No external test framework: a tiny check harness prints PASS/FAIL per case
// and a final summary, and returns non-zero on ANY failure.

#include "SECSGEM/SecsSvEcRegistration.h"
#include <cstdio>
#include <string>

static int g_pass = 0;
static int g_fail = 0;

static void check_i(const char* name, int got, int expected)
{
    if (got == expected) { printf("PASS  %-58s got=%d\n", name, got); ++g_pass; }
    else { printf("FAIL  %-58s got=%d exp=%d\n", name, got, expected); ++g_fail; }
}

static void check_b(const char* name, bool got, bool expected)
{
    if (got == expected) { printf("PASS  %-58s got=%d\n", name, (int)got); ++g_pass; }
    else { printf("FAIL  %-58s got=%d exp=%d\n", name, (int)got, (int)expected); ++g_fail; }
}

static void check_s(const char* name, const std::string& got, const std::string& expected)
{
    if (got == expected) { printf("PASS  %-58s got=\"%s\"\n", name, got.c_str()); ++g_pass; }
    else { printf("FAIL  %-58s got=\"%s\" exp=\"%s\"\n", name, got.c_str(), expected.c_str()); ++g_fail; }
}

int main()
{
    printf("=== SecsSvEcRegistration translation verification ===\n");
    printf("(golden HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp; original binary NOT run)\n\n");

    SecsSvEcRegistration reg;

    // ---------------------------------------------------------------------
    printf("-- SV registration (4 overloads) --\n");
    // ---------------------------------------------------------------------
    int    svRawInt = 12345;
    AnsiString svAnsiVar = "sv-ansi-value";

    reg.SetSVDataPointer(AnsiString("1"), HType.INT_4_TYPE, "SvRaw", "unit1", &svRawInt, "remark1");
    check_i("SV_ID.Count after 1st SV",  reg.SV_ID->Count, 1);
    check_s("SV_ID[0]",                  reg.SV_ID->GetString(0).str(),   "1");
    check_s("SV_NAME[0]",                reg.SV_NAME->GetString(0).str(), "SvRaw");
    check_s("SV_UNIT[0]",                reg.SV_UNIT->GetString(0).str(), "unit1");
    check_s("VCL_NAME[0] (raw ptr tag)",  reg.VCL_NAME->GetString(0).str(), "0");
    check_s("SV_LEN[0] (default 1)",     reg.SV_LEN->GetString(0).str(),  "1");
    check_i("SV_Ptr[0] == &svRawInt",     reg.SV_Ptr->GetItem(0) == &svRawInt, 1);

    reg.SetSVDataPointer(AnsiString("2"), HType.ASCII_TYPE, "SvAnsi", "unit2", &svAnsiVar, "remark2");
    check_i("SV_ID.Count after 2nd SV",   reg.SV_ID->Count, 2);
    check_s("VCL_NAME[1] (AnsiString* tag)", reg.VCL_NAME->GetString(1).str(), "2");

    int svArr[4] = {1, 2, 3, 4};
    reg.SetSVDataPointer(AnsiString("3"), HType.INT_4_TYPE, "SvArr", "unit3", svArr, 4, "remark3");
    check_i("SV_ID.Count after 3rd SV",   reg.SV_ID->Count, 3);
    check_s("SV_LEN[2] (array Len=4)",    reg.SV_LEN->GetString(2).str(), "4");
    check_s("VCL_NAME[2] (raw ptr tag)",  reg.VCL_NAME->GetString(2).str(), "0");

    vclcompat::TObject dummyWidget;   // stand-in for a live VCL widget pointer
    reg.SetSVDataPointer(AnsiString("4"), HType.INT_4_TYPE, "SvWidget", "unit4", &dummyWidget, "remark4");
    check_i("SV_ID.Count after 4th SV",   reg.SV_ID->Count, 4);
    check_s("VCL_NAME[3] (TObject* tag)", reg.VCL_NAME->GetString(3).str(), "1");

    // Duplicate SVID -> silently rejected (golden: ShowMessage + early return).
    reg.SetSVDataPointer(AnsiString("1"), HType.INT_4_TYPE, "SvRawDup", "unitX", &svRawInt, "remarkX");
    check_i("SV_ID.Count unchanged after duplicate SVID", reg.SV_ID->Count, 4);

    // ---------------------------------------------------------------------
    printf("\n-- EC registration overload 2: void*, int Min/Max/Default --\n");
    // ---------------------------------------------------------------------
    int ecRawInt = 42;
    reg.SetECDataPointer(AnsiString("100"), HType.INT_4_TYPE, "EcRaw", "unitE1",
                          &ecRawInt, 100, 0, 50, "ecRemark1");   // MinValue(100) > MaxValue(0) -> auto-swap
    check_i("EC_ID.Count after 1st EC",   reg.EC_ID->Count, 1);
    check_s("EC_Ptr_Min_Value[0] (post-swap, =0)",  reg.EC_Ptr_Min_Value->GetString(0).str(), "0");
    check_s("EC_Ptr_Max_Value[0] (post-swap, =100)", reg.EC_Ptr_Max_Value->GetString(0).str(), "100");
    check_s("EC_Ptr_Default_Value[0]",    reg.EC_Ptr_Default_Value->GetString(0).str(), "50");
    check_s("EC_VCL_NAME[0] (raw ptr tag)", reg.EC_VCL_NAME->GetString(0).str(), "0");
    // Registration-time GetECDataValue(ECID) call is fully safe+correct for
    // this overload (P really is a raw int*) -- verifies the whole
    // Set*->GetECDataValue->decode pipeline end to end.
    check_s("EC_OldValue[0] == decoded live *ecRawInt (42)", reg.EC_OldValue->GetString(0).str(), "42");
    // golden: every SetECDataPointer overload also calls SetSVDataPointer
    // under the same ID -- verify the EC is ALSO now a registered SV.
    check_b("EC #100 is also registered as SV", reg.SV_ID->IndexOf(AnsiString("100")) >= 0, true);

    // ---------------------------------------------------------------------
    printf("\n-- EC registration overload 4: void*, AnsiString Min/Max/Default --\n");
    // ---------------------------------------------------------------------
    double ecRawDouble = 3.5;
    reg.SetECDataPointer(AnsiString("101"), HType.FT_8_TYPE, "EcRawF", "unitE2",
                          &ecRawDouble, AnsiString("10"), AnsiString("-10"), AnsiString("0"), "ecRemark2");
    check_i("EC_ID.Count after 2nd EC",   reg.EC_ID->Count, 2);
    check_s("EC_Ptr_Min_Value[1] (post-swap, =-10)", reg.EC_Ptr_Min_Value->GetString(1).str(), "-10");
    check_s("EC_Ptr_Max_Value[1] (post-swap, =10)",  reg.EC_Ptr_Max_Value->GetString(1).str(), "10");
    check_s("EC_VCL_NAME[1] (raw ptr tag)", reg.EC_VCL_NAME->GetString(1).str(), "0");
    check_s("EC_OldValue[1] == decoded live *ecRawDouble (3.5)", reg.EC_OldValue->GetString(1).str(), "3.5");

    // ---------------------------------------------------------------------
    printf("\n-- EC registration overload 3: TObject*, const char* Min/Max/Default --\n");
    // ---------------------------------------------------------------------
    vclcompat::TObject ecWidget;
    reg.SetECDataPointer(AnsiString("102"), HType.INT_4_TYPE, "EcWidget", "unitE3",
                          &ecWidget, "100", "0", "50", "ecRemark3");   // Min(100) > Max(0) -> auto-swap
    check_i("EC_ID.Count after 3rd EC (recognized Type)", reg.EC_ID->Count, 3);
    check_s("EC_Ptr_Min_Value[2] (post-swap, =0)",  reg.EC_Ptr_Min_Value->GetString(2).str(), "0");
    check_s("EC_Ptr_Max_Value[2] (post-swap, =100)", reg.EC_Ptr_Max_Value->GetString(2).str(), "100");
    check_s("EC_VCL_NAME[2] (TObject* tag)", reg.EC_VCL_NAME->GetString(2).str(), "1");

    // Unrecognized Type (not one of the 12 HType.* codes) -> golden's dead
    // Type-recognition gate's only LIVE effect: silent early return, no
    // registration at all (verified via IndexOf, not Count, since a rejected
    // call never even reaches EC_ID->Add).
    reg.SetECDataPointer(AnsiString("103"), (unsigned char)0xFF, "EcBadType", "unitE4",
                          &ecWidget, "0", "0", "0", "ecRemark4");
    check_b("Unrecognized-Type EC #103 was NOT registered",
            reg.EC_ID->IndexOf(AnsiString("103")) >= 0, false);
    check_i("EC_ID.Count unchanged after unrecognized-Type rejection", reg.EC_ID->Count, 3);

    // ---------------------------------------------------------------------
    printf("\n-- EC registration overload 1: AnsiString*, AnsiString Min/Max/Default --\n");
    // ---------------------------------------------------------------------
    AnsiString ecAnsiVar = "999";   // a live AnsiString-backed EC value
    reg.SetECDataPointer(AnsiString("104"), HType.INT_4_TYPE, "EcAnsi", "unitE5",
                          &ecAnsiVar, AnsiString("5"), AnsiString("1"), AnsiString("1"), "ecRemark5");
    check_i("EC_ID.Count after 4th EC",   reg.EC_ID->Count, 4);
    check_s("EC_Ptr_Min_Value[3] (post-swap, =1)", reg.EC_Ptr_Min_Value->GetString(3).str(), "1");
    check_s("EC_Ptr_Max_Value[3] (post-swap, =5)", reg.EC_Ptr_Max_Value->GetString(3).str(), "5");
    check_s("EC_VCL_NAME[3] (AnsiString* tag)", reg.EC_VCL_NAME->GetString(3).str(), "2");
    // golden asymmetry (preserved, verbatim): overload 1 alone never calls
    // SetECEnableData -- nothing externally observable to assert here since
    // that call is gated to a no-op anyway; documented in the .cpp instead.

    // Duplicate ECID -> silently rejected for every overload family alike.
    reg.SetECDataPointer(AnsiString("100"), HType.INT_4_TYPE, "EcRawDup", "unitX",
                          &ecRawInt, 0, 1, 0, "remarkDup");
    check_i("EC_ID.Count unchanged after duplicate ECID", reg.EC_ID->Count, 4);

    // ---------------------------------------------------------------------
    printf("\n-- GetECDataValue: IsVCL==2, reachable only AFTER a later registration --\n");
    // ---------------------------------------------------------------------
    // At EC #104's OWN registration (above), GetECDataValue took the safe/
    // IsVCL==0 branch (see the reachability proof in the .cpp) -- EC_OldValue
    // for #104 was seeded from that safe path, not asserted here (it reads
    // ecAnsiVar's address as a raw int, not its string content -- documented
    // hazard-adjacent quirk, deliberately not checked).
    //
    // What THIS section proves is a SEPARATE, genuinely-discovered golden
    // BUG (not a translation defect -- confirmed by first writing the
    // "obviously correct" expectation here, watching it FAIL, then re-reading
    // golden byte-for-byte to find out why): once a LATER registration
    // exists (EC_VCL_NAME->Count now exceeds #104's own index), a fresh call
    // to GetECDataValue("104") DOES take the IsVCL==2 branch and DOES
    // correctly set `VCLStr` from the live AnsiString* (proving THAT much of
    // the wiring is correct) -- but golden's `Type` local
    // (uHGemEquipment.cpp:3654 `unsigned char Type;`) is declared WITHOUT an
    // initializer and is ONLY EVER ASSIGNED inside the sibling `if(IsVCL==1)`
    // sub-branch (:3688) -- never inside the `else` (IsVCL==2) sub-branch
    // (:3763-3767) that actually runs here. So every subsequent Type-keyed
    // dispatch (the VCLStr->P priming AND the final P->AnsiString decode)
    // runs with `Type` holding whatever was left over -- true stack garbage
    // in real BCB6 (a genuine golden bug), deterministically 0 in this
    // translation (Type is value-initialized here specifically to avoid
    // reproducing actual C++ undefined behavior -- a judgment call, flagged
    // for review). Since 0 coincidentally equals HType.LIST_TYPE, which NO
    // branch in either dispatch chain handles, the function falls through
    // both chains and hits the final `return "";` -- i.e. GetECDataValue
    // ALWAYS returns an EMPTY STRING for an AnsiString*-backed (VCL_NAME=="2")
    // EC once queried after its own registration moment, regardless of the
    // live value. This is a real, confirmed golden defect in
    // THGem::GetECDataValue itself (not introduced by this translation),
    // preserved verbatim per project convention rather than "fixed" (e.g. by
    // re-deriving Type from EC_TYPE->GetString(i) in the IsVCL==2 branch,
    // which golden itself never does) -- flagged prominently for human
    // review given its real-world impact (any S2F13/S2F15 read of such an EC
    // would observe "" instead of the true value).
    ecAnsiVar = "12345";   // mutate the live AnsiString AFTER registration
    reg.SetECDataPointer(AnsiString("105"), HType.INT_4_TYPE, "EcLater", "unitE6",
                          &ecRawInt, 0, 100, 0, "ecRemark6");   // pushes EC_VCL_NAME->Count past #104's index
    AnsiString liveReRead = reg.GetECDataValue(AnsiString("104"));
    check_s("GetECDataValue(\"104\") (IsVCL==2) returns \"\" -- confirmed golden Type-uninitialized bug",
            liveReRead.str(), "");

    // ---------------------------------------------------------------------
    printf("\n=== SUMMARY: %d passed, %d failed ===\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
