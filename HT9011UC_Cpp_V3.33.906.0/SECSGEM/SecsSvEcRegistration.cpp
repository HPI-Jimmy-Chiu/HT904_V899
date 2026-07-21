//---------------------------------------------------------------------------
//  SECSGEM/SecsSvEcRegistration.cpp -- THGem's SV/EC registration API
//
//  Translation wave: W5-Final (SECSGEM SV/EC registration API)
//  Translator: AI(W5-Final-SecsSvEcRegistration) 20260711
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp
//  (per-function golden line citations are given at each function below; see
//  the .h file header for the overall scope/design note and the
//  "SCOPE-BOUNDS CORRECTION" on the task's original 6165 estimate).
//
//  vclcompat NOTE (same substitution SecsWireCodec.cpp already established --
//  read that file's own file-head comment first): golden repeats
//  `xxxList->Strings[i].c_str()` everywhere. vclcompat's TStringList
//  `Strings[i]` returns a `StringsProxy` (an operator-AnsiString() proxy, not
//  an AnsiString), which has no `.c_str()` member. Translated throughout this
//  file as `xxxList->GetString(i).c_str()` (or, when no `.c_str()` is needed,
//  plain `xxxList->GetString(i)` for a consistent idiom) -- zero behavior
//  change, pure vclcompat-proxy accommodation.
//---------------------------------------------------------------------------
#include "SECSGEM/SecsSvEcRegistration.h"
// AI(W906-VCW1) 20260721: GetECDataValue's IsVCL==1 dynamic_cast cascade
// (below) needs the 6 VCL-widget stand-ins this header supplies (TPanel/
// TCustomEdit/TComboBox/TLabel/TCheckBox/TRadioGroup) -- see that header's
// own file-head scope-boundary note (no real widget-backed EC is made
// functional by this include; see also this method's own "REGISTRATION-TIME
// REACHABILITY PROOF" comment below -- still true, this branch remains
// unreachable from any call site in THIS unit's own scope even though it
// now compiles and dispatches correctly).
#include "vclcompat/Controls.h"
#include <cstdlib>   // atoi/atof/_atoi64
#include <cstring>   // strcpy

namespace {

// golden Dialogs.hpp `void ShowMessage(const AnsiString&)` -- the REAL VCL
// modal dialog (distinct from this project's own `ShowMyMessage` wrapper in
// canary_support.h/mymessbox.h -- no stand-in exists for the real ShowMessage
// global anywhere in the tree today). GATED per project convention (matches
// e.g. BarCode's `Gated_ShowMessageBox_YES_SKIP` precedent): only reachable
// on the rare, misuse-only duplicate-SVID/ECID registration path -- a no-op
// here just means a duplicate registration is silently dropped (matching
// golden's own early `return;` right after the call) instead of also
// popping a dialog.
void Gated_ShowMessage(const AnsiString & /*S*/)
{
    // TODO(W7-UI): wire to a real modal ShowMessage dialog.
}

// golden uHGemEquipment.cpp:9323-9335 (THGem::SetECEnableData) -- mutates
// `sgSECSECData`, a live TStringGrid widget (row-append + 4 cell writes),
// out of scope for this unit (MIGRATION_ROADMAP.md's own W5 SECSGEM-engine
// entry flags a "headless StringGrid helper" as a future prerequisite for
// this exact widget). No-op stand-in: every EC registered through the 3
// overloads that call this (see below -- one of the 4 does NOT, a genuine
// golden asymmetry, preserved) is simply never grid-enabled offline.
// GetECEnableData's golden body (also out of scope, uHGemEquipment.cpp:9306)
// would consequently always report "not enabled" for any EC queried before a
// real StringGrid exists -- harmless here since neither this stub's callers
// nor GetECDataValue (below) ever calls GetECEnableData.
void Gated_SetECEnableData(const AnsiString & /*ECID*/, const AnsiString & /*Function*/)
{
    // TODO(W5/W7 SECSGEM engine wave): real sgSECSECData row-append.
}

} // anonymous namespace

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:549-564 (SV_* group) + :567-581 (EC_* group,
// interleaved in golden with other THGem members not owned by this unit).
//
// AI(W5-Final-SecsSvEcRegistration) 20260711: golden Clear()s every SV_*
// list immediately after allocating it (a no-op on a freshly-constructed,
// already-empty list -- reproduced verbatim below for byte-for-byte
// diffability) but does NOT do the same for the EC_* group -- a genuine
// golden asymmetry, confirmed by direct read (uHGemEquipment.cpp:567-581 has
// no EC_*->Clear() calls at all), preserved as-is rather than "fixed".
//---------------------------------------------------------------------------
SecsSvEcRegistration::SecsSvEcRegistration()
    : SV_ID(new TStringList()), SV_TYPE(new TStringList()), SV_NAME(new TStringList()),
      SV_UNIT(new TStringList()), SV_Ptr(new vclcompat::TList()), VCL_NAME(new TStringList()),
      SV_LEN(new TStringList()), SV_Remark(new TStringList()),
      EC_ID(new TStringList()), EC_TYPE(new TStringList()), EC_NAME(new TStringList()),
      EC_UNIT(new TStringList()), EC_Ptr(new vclcompat::TList()), EC_Ptr_Min(new vclcompat::TList()),
      EC_Ptr_Max(new vclcompat::TList()), EC_Ptr_Default(new vclcompat::TList()),
      EC_Remark(new TStringList()), EC_Ptr_Min_Value(new TStringList()),
      EC_Ptr_Max_Value(new TStringList()), EC_Ptr_Default_Value(new TStringList()),
      EC_VCL_NAME(new TStringList()), EC_OldValue(new TStringList())
{
    SV_ID->Clear();
    SV_TYPE->Clear();
    SV_NAME->Clear();
    SV_UNIT->Clear();
    SV_Ptr->Clear();
    VCL_NAME->Clear();
    SV_LEN->Clear();
    SV_Remark->Clear();
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:704-754 (TStringList group: Clear-all then
// delete-all) + :776-789 (TList group, a separate later block in golden).
// Only this unit's owned members are shown; out-of-scope THGem siblings
// interleaved in golden (SReceiveData/UploadFileString/RequestRemoteDownLoad/
// etc.) are omitted -- relative order among the members shown here matches
// golden's own relative order exactly.
//---------------------------------------------------------------------------
SecsSvEcRegistration::~SecsSvEcRegistration()
{
    SV_ID->Clear();
    SV_TYPE->Clear();
    SV_NAME->Clear();
    SV_UNIT->Clear();
    VCL_NAME->Clear();
    SV_LEN->Clear();
    SV_Remark->Clear();
    EC_ID->Clear();
    EC_TYPE->Clear();
    EC_NAME->Clear();
    EC_UNIT->Clear();
    EC_VCL_NAME->Clear();
    EC_Remark->Clear();
    EC_Ptr_Min_Value->Clear();
    EC_Ptr_Max_Value->Clear();
    EC_Ptr_Default_Value->Clear();
    EC_OldValue->Clear();

    delete SV_ID;
    delete SV_TYPE;
    delete SV_NAME;
    delete SV_UNIT;
    delete VCL_NAME;
    delete SV_LEN;
    delete SV_Remark;
    delete EC_ID;
    delete EC_TYPE;
    delete EC_NAME;
    delete EC_UNIT;
    delete EC_VCL_NAME;
    delete EC_Remark;
    delete EC_Ptr_Min_Value;
    delete EC_Ptr_Max_Value;
    delete EC_Ptr_Default_Value;
    delete EC_OldValue;

    // golden uHGemEquipment.cpp:776-789 -- TList group, Cleared then deleted
    // in its own later block (Ifor 20170603 (wei) comment: "TStringList 刪除
    // 前先清空" -- a pre-existing golden comment/label mismatch, since this
    // second block is actually the TList members, not TStringList; preserved
    // verbatim, not corrected).
    SV_Ptr->Clear();
    EC_Ptr->Clear();
    EC_Ptr_Min->Clear();
    EC_Ptr_Max->Clear();
    EC_Ptr_Default->Clear();
    delete SV_Ptr;
    delete EC_Ptr;
    delete EC_Ptr_Min;
    delete EC_Ptr_Max;
    delete EC_Ptr_Default;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:5648-5668 -- P is a raw pointer to one scalar
// value of `Type` (the dominant, "safe" registration shape: VCL_NAME "0").
//---------------------------------------------------------------------------
void SecsSvEcRegistration::SetSVDataPointer(AnsiString SVID, unsigned char Type, AnsiString SVName,
                                             AnsiString SVUnit, void *P, AnsiString Remark)
{
    AnsiString S;
    if (SV_ID->IndexOf(SVID) >= 0)
    {
        S = "SVID:" + SVID + " duplicate error !";
        Gated_ShowMessage(S);
        return;
    }
    SV_ID->Add(SVID);
    SV_TYPE->Add(Type);
    SV_NAME->Add(SVName);
    SV_UNIT->Add(SVUnit);
    SV_Ptr->Add(P);
    VCL_NAME->Add(0);
    SV_LEN->Add(1);
    SV_Remark->Add(Remark);
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:5669-5689 -- P points at a live AnsiString
// variable (VCL_NAME "2"); see GetECDataValue's reachability-proof comment
// below for the registration-time read-back hazard this (and the TObject*
// overload) creates when Type==ASCII_TYPE.
//---------------------------------------------------------------------------
void SecsSvEcRegistration::SetSVDataPointer(AnsiString SVID, unsigned char Type, AnsiString SVName,
                                             AnsiString SVUnit, AnsiString *P, AnsiString Remark)
{
    AnsiString S;
    if (SV_ID->IndexOf(SVID) >= 0)
    {
        S = "SVID:" + SVID + " duplicate error !";
        Gated_ShowMessage(S);
        return;
    }
    SV_ID->Add(SVID);
    SV_TYPE->Add(Type);
    SV_NAME->Add(SVName);
    SV_UNIT->Add(SVUnit);
    SV_Ptr->Add(P);
    VCL_NAME->Add(2);
    SV_LEN->Add(1);
    SV_Remark->Add(Remark);
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:5690-5711 -- P is a raw pointer to an ARRAY of
// `Len` values of `Type` (SV_LEN records Len instead of the default 1).
//---------------------------------------------------------------------------
void SecsSvEcRegistration::SetSVDataPointer(AnsiString SVID, unsigned char Type, AnsiString SVName,
                                             AnsiString SVUnit, void *P, int Len, AnsiString Remark)
{
    AnsiString S;
    if (SV_ID->IndexOf(SVID) >= 0)
    {
        S = "SVID:" + SVID + " duplicate error !";
        Gated_ShowMessage(S);
        return;
    }

    SV_ID->Add(SVID);
    SV_TYPE->Add(Type);
    SV_NAME->Add(SVName);
    SV_UNIT->Add(SVUnit);
    SV_Ptr->Add(P);
    VCL_NAME->Add(0);
    SV_LEN->Add(Len);
    SV_Remark->Add(Remark);
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:5712-5730 -- P is a live VCL widget pointer
// (VCL_NAME "1"); see GetECDataValue's reachability-proof comment below.
//---------------------------------------------------------------------------
void SecsSvEcRegistration::SetSVDataPointer(AnsiString SVID, unsigned char Type, AnsiString SVName,
                                             AnsiString SVUnit, TObject *P, AnsiString Remark)
{
    AnsiString S;
    if (SV_ID->IndexOf(SVID) >= 0)
    {
        S = "SVID:" + SVID + " duplicate error !";
        Gated_ShowMessage(S);
        return;
    }

    SV_ID->Add(SVID);
    SV_TYPE->Add(Type);
    SV_NAME->Add(SVName);
    SV_UNIT->Add(SVUnit);
    SV_Ptr->Add(P);
    VCL_NAME->Add("1");
    SV_LEN->Add(1);
    SV_Remark->Add(Remark);
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:5732-5769 -- Min/Max/Default supplied as
// AnsiString (numerically auto-swapped via atof if Min>Max).
//---------------------------------------------------------------------------
void SecsSvEcRegistration::SetECDataPointer(AnsiString ECID, unsigned char Type, AnsiString ECName,
                                             AnsiString ECUnit, AnsiString *P,
                                             AnsiString MinValue, AnsiString MaxValue,
                                             AnsiString DefaultValue, AnsiString Remark)
{
    AnsiString S;
    if (EC_ID->IndexOf(ECID) >= 0)
    {
        S = "ECID:" + ECID + " duplicate error !";
        Gated_ShowMessage(S);
        return;
    }

    EC_ID->Add(ECID);
    EC_TYPE->Add(Type);
    EC_NAME->Add(ECName);
    EC_UNIT->Add(ECUnit);
    EC_Ptr->Add(P);

    EC_Ptr_Min->Add(NULL);
    EC_Ptr_Max->Add(NULL);
    EC_Ptr_Default->Add(NULL);

    if (atof(MinValue.c_str()) > atof(MaxValue.c_str()))
    {
        EC_Ptr_Min_Value->Add(MaxValue);
        EC_Ptr_Max_Value->Add(MinValue);
    }
    else
    {
        EC_Ptr_Min_Value->Add(MinValue);
        EC_Ptr_Max_Value->Add(MaxValue);
    }
    EC_Ptr_Default_Value->Add(DefaultValue);

    S = GetECDataValue(ECID);
    EC_OldValue->Add(S);
    EC_VCL_NAME->Add("2");
    EC_Remark->Add(Remark);
    SetSVDataPointer(ECID, Type, ECName, ECUnit, P, Remark);
    // NOTE golden asymmetry (verbatim, not fixed): unlike the other 3
    // SetECDataPointer overloads below, THIS one does NOT call
    // SetECEnableData(ECID, ECName) -- ECs registered via the AnsiString*
    // overload are never grid-enabled by golden itself. Confirmed by direct
    // read (uHGemEquipment.cpp:5732-5769 has no such call); preserved as-is.
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:5771-5809 -- Min/Max/Default supplied as int
// (auto-swapped if MinValue>MaxValue). Also calls SetECEnableData (gated) --
// golden's own "pig 2014.08.27 KYEC" marker.
//---------------------------------------------------------------------------
void SecsSvEcRegistration::SetECDataPointer(AnsiString ECID, unsigned char Type, AnsiString ECName,
                                             AnsiString ECUnit, void *P,
                                             int MinValue, int MaxValue, int DefaultValue,
                                             AnsiString Remark)
{
    AnsiString S;
    if (EC_ID->IndexOf(ECID) >= 0)
    {
        S = "ECID:" + ECID + " duplicate error !";
        Gated_ShowMessage(S);
        return;
    }

    EC_ID->Add(ECID);
    EC_TYPE->Add(Type);
    EC_NAME->Add(ECName);
    EC_UNIT->Add(ECUnit);
    EC_Ptr->Add(P);

    EC_Ptr_Min->Add(NULL);
    EC_Ptr_Max->Add(NULL);
    EC_Ptr_Default->Add(NULL);

    if (MinValue > MaxValue)
    {
        EC_Ptr_Min_Value->Add(MaxValue);
        EC_Ptr_Max_Value->Add(MinValue);
    }
    else
    {
        EC_Ptr_Min_Value->Add(MinValue);
        EC_Ptr_Max_Value->Add(MaxValue);
    }
    EC_Ptr_Default_Value->Add(DefaultValue);

    S = GetECDataValue(ECID);
    EC_OldValue->Add(S);
    EC_VCL_NAME->Add("0");
    EC_Remark->Add(Remark);
    SetSVDataPointer(ECID, Type, ECName, ECUnit, P, Remark);
    Gated_SetECEnableData(ECID, ECName);   // golden: SetECEnableData(ECID, ECName); //pig 2014.08.27 KYEC
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:5811-5987 -- P is a live VCL widget pointer;
// Min/Max/Default are C strings (declared `const char*` here, not golden's
// literal non-const `char*` -- see the .h declaration's JUDGMENT CALL
// comment, same accommodation SecsWireCodec.cpp already made for
// DataItemOut's `Ptr`).
//
// The Type-recognition if/else-if chain below is ITSELF already fully dead
// code in golden (every recognized-Type branch body is commented-out in the
// golden source -- verified by direct read, not an artifact of translation).
// Preserved verbatim as empty branches (golden's own commented-out lines are
// reproduced as comments for byte-for-byte diffability); the chain's only
// live effect is the final `else { return; }` for an unrecognized Type.
//---------------------------------------------------------------------------
void SecsSvEcRegistration::SetECDataPointer(AnsiString ECID, unsigned char Type, AnsiString ECName,
                                             AnsiString ECUnit, TObject *P,
                                             const char *MinValue, const char *MaxValue,
                                             const char *DefaultValue, AnsiString Remark)
{
    AnsiString S;
    if (EC_ID->IndexOf(ECID) >= 0)
    {
        S = "ECID:" + ECID + " duplicate error !";
        Gated_ShowMessage(S);
        return;
    }

    if (Type == HType.UINT_1_TYPE)
    {
        //  unsigned char *Min,*Max,*Default; Min=new unsigned char; Max=new unsigned char;
        //  Default=new unsigned char; *Min=atoi(MinValue); *Max=atoi(MaxValue); *Default=atoi(DefaultValue);
    }
    else if (Type == HType.UINT_2_TYPE)
    {
        //  unsigned short *Min,*Max,*Default; ... (golden: commented out, dead)
    }
    else if (Type == HType.UINT_4_TYPE)
    {
        //  unsigned *Min,*Max,*Default; ... (golden: commented out, dead)
    }
    else if (Type == HType.UINT_8_TYPE)
    {
        //  unsigned __int64 *Min,*Max,*Default; ... (golden: commented out, dead)
    }
    else if (Type == HType.INT_1_TYPE || Type == HType.BINARY_TYPE)
    {
        //  char *Min,*Max,*Default; ... (golden: commented out, dead)
    }
    else if (Type == HType.INT_2_TYPE)
    {
        //  short *Min,*Max,*Default; ... (golden: commented out, dead)
    }
    else if (Type == HType.INT_4_TYPE)
    {
        //  int *Min,*Max,*Default; ... (golden: commented out, dead)
    }
    else if (Type == HType.INT_8_TYPE)
    {
        //  __int64 *Min,*Max,*Default; ... (golden: commented out, dead)
    }
    else if (Type == HType.ASCII_TYPE)
    {
        //  int *Min,*Max,*Default; ... (golden: commented out, dead)
    }
    else if (Type == HType.BOOLEAN_TYPE)
    {
        //  bool *Min,*Max,*Default; ... (golden: commented out, dead)
    }
    else if (Type == HType.FT_4_TYPE)                                    // Steven 20130730 ADD
    {
        //  float *Min,*Max,*Default; ... (golden: commented out, dead)
    }
    else if (Type == HType.FT_8_TYPE)                                    // Steven 20130730 ADD
    {
        //  double *Min,*Max,*Default; ... (golden: commented out, dead)
    }
    else
    {
        return;
    }

    EC_ID->Add(ECID);
    EC_TYPE->Add(Type);
    EC_NAME->Add(ECName);
    EC_UNIT->Add(ECUnit);
    EC_Ptr->Add(P);

    EC_Ptr_Min->Add(NULL);
    EC_Ptr_Max->Add(NULL);
    EC_Ptr_Default->Add(NULL);

    double *Min, *Max;
    Min = new double;
    Max = new double;
    *Min = atof(MinValue);
    *Max = atof(MaxValue);

    if (*Min > *Max)
    {
        EC_Ptr_Min_Value->Add(MaxValue);
        EC_Ptr_Max_Value->Add(MinValue);
    }
    else
    {
        EC_Ptr_Min_Value->Add(MinValue);
        EC_Ptr_Max_Value->Add(MaxValue);
    }
    EC_Ptr_Default_Value->Add(DefaultValue);

    S = GetECDataValue(ECID);
    EC_OldValue->Add(S);
    EC_VCL_NAME->Add("1");
    EC_Remark->Add(Remark);
    SetSVDataPointer(ECID, Type, ECName, ECUnit, P, Remark);
    Gated_SetECEnableData(ECID, ECName);   // golden: SetECEnableData(ECID, ECName); //pig 2014.08.27 KYEC

    delete Min;
    delete Max;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:5989-6025 -- P is a raw pointer; Min/Max/Default
// supplied as AnsiString (auto-swapped via atof, like overload 1 above).
//---------------------------------------------------------------------------
void SecsSvEcRegistration::SetECDataPointer(AnsiString ECID, unsigned char Type, AnsiString ECName,
                                             AnsiString ECUnit, void *P,
                                             AnsiString Min, AnsiString Max, AnsiString Default,
                                             AnsiString Remark)
{
    AnsiString S;
    if (EC_ID->IndexOf(ECID) >= 0)
    {
        S = "ECID:" + ECID + " duplicate error !";
        Gated_ShowMessage(S);
        return;
    }
    EC_ID->Add(ECID);
    EC_TYPE->Add(Type);
    EC_NAME->Add(ECName);
    EC_UNIT->Add(ECUnit);
    EC_Ptr->Add(P);

    EC_Ptr_Min->Add(NULL);
    EC_Ptr_Max->Add(NULL);
    EC_Ptr_Default->Add(NULL);

    if (atof(Min.c_str()) > atof(Max.c_str()))
    {
        EC_Ptr_Max_Value->Add(Min);
        EC_Ptr_Min_Value->Add(Max);
    }
    else
    {
        EC_Ptr_Min_Value->Add(Min);
        EC_Ptr_Max_Value->Add(Max);
    }
    EC_Ptr_Default_Value->Add(Default);

    S = GetECDataValue(ECID);
    EC_OldValue->Add(S);
    EC_VCL_NAME->Add("0");
    EC_Remark->Add(Remark);
    SetSVDataPointer(ECID, Type, ECName, ECUnit, P, Remark);
    Gated_SetECEnableData(ECID, ECName);   // golden: SetECEnableData(ECID, ECName); //pig 2014.08.27 KYEC
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:3650-3925 (THGem::GetECDataValue).
//
// REGISTRATION-TIME REACHABILITY PROOF (verified individually against all 4
// SetECDataPointer overloads above -- the hand-off prompt's own instruction
// was to re-check this for every overload, not trust a single spot check):
// every one of the 4 overloads calls `EC_ID->Add(ECID)` (and the sibling
// EC_TYPE/EC_NAME/EC_UNIT/EC_Ptr/EC_Ptr_Min/EC_Ptr_Max/EC_Ptr_Default/
// EC_Ptr_Min_Value/EC_Ptr_Max_Value/EC_Ptr_Default_Value Adds) BEFORE calling
// `S=GetECDataValue(ECID)`, and only calls `EC_VCL_NAME->Add(...)` AFTER
// that GetECDataValue call returns (confirmed for all 4: overload 1 adds
// "2", overload 2 adds "0", overload 3 adds "1", overload 4 adds "0" --
// always strictly after the GetECDataValue call in every one). Since EC_ID
// rejects duplicates up front (every overload's `if(EC_ID->IndexOf(ECID)>=0)
// return;` guard), `ECID` here is always brand-new, so `i=EC_ID->IndexOf
// (ECID)` below always resolves to the newest index (EC_ID's post-Add
// Count-1). At that exact moment, EC_VCL_NAME->Count still equals the
// PRE-registration count -- exactly `i` (one less than EC_ID's new Count) --
// so the `EC_VCL_NAME->Count>i` test below is ALWAYS false for this call
// path, forcing `IsVCL` to "0" (the safe/raw-pointer branch) REGARDLESS of
// which VCL_NAME tag the calling overload is about to add moments later.
// The `IsVCL==1`/`IsVCL==2` branch below is therefore PROVABLY unreachable
// from every call site in this unit's own scope (all 8 Set*DataPointer
// overloads). It only becomes reachable for an OLDER, already-registered
// ECID queried by a FUTURE, out-of-scope, non-registration caller (S2F13/
// S2F15 handlers, golden uHGemEquipment.cpp:3968/:4000) -- for which
// IsVCL==1 needed live VCL widget types this unit didn't have; AI(W906-VCW1)
// 20260721 supplied them (vclcompat/Controls.h) and un-gated this branch
// below -- while IsVCL==2 needs nothing new (kept ACTIVE, costs nothing, and
// exercises the SAME genuine golden bug documented immediately below).
//
// SECOND DISCOVERED BUG, PRESERVED VERBATIM (flag for review, CONFIRMED by
// a failing test before this comment was written -- tests/
// test_SecsSvEcRegistration.cpp's "GetECDataValue IsVCL==2" case): golden's
// local `unsigned char Type;` (uHGemEquipment.cpp:3654) is declared WITHOUT
// an initializer and is ONLY EVER ASSIGNED inside the `if(IsVCL==1)`
// sub-branch (:3688, gated dead code below) -- NEVER inside the `else`
// (IsVCL==2) sub-branch (:3763-3767) that is this project's one kept-ACTIVE
// path. This means every subsequent Type-keyed dispatch (both the VCLStr->P
// priming block and the final P->AnsiString decode) runs with `Type` holding
// whatever was left on the stack -- genuine undefined behavior in real BCB6.
// This translation value-initializes `Type=0` at declaration (a deliberate
// judgment call to avoid reproducing true C++ UB) rather than leaving it
// truly uninitialized; since 0 happens to equal HType.LIST_TYPE, which NO
// branch in either dispatch chain handles, the observable, DETERMINISTIC
// result of this translation is: GetECDataValue ALWAYS returns "" for any
// AnsiString*-backed (VCL_NAME=="2") EC once queried after its own
// registration moment (see the test for the exact repro). This is a real,
// confirmed defect in THGem::GetECDataValue itself, not introduced by this
// translation -- preserved verbatim (NOT fixed, e.g. by re-deriving `Type`
// from `EC_TYPE->GetString(i)` in the IsVCL==2 branch, which golden itself
// never does) per this project's faithful-translation mandate. Flagged
// prominently: any FUTURE S2F13/S2F15 (out of scope) read of such an EC
// would observe "" instead of the true live value -- a real product-level
// bug inherited from golden, not a translation artifact.
//
// DISCOVERED HAZARD, PRESERVED VERBATIM (flag for review, not fixed): for an
// EC registered via the AnsiString* or TObject* overloads above with
// Type==ASCII_TYPE, THIS call (at registration time, always taking the safe/
// IsVCL==0 branch per the proof above) reinterprets the STORED POINTER
// ITSELF (an AnsiString* or a widget TObject*, cast to void* then to char*)
// as if it were a raw null-terminated char buffer, then strlen/copies
// through it. This is a genuine golden quirk (the exact same category of
// bug exists in real BCB6 too, just against a structurally different
// AnsiString/TObject layout there) -- not something to "fix" per this
// project's translation mandate. It only feeds a one-time "old value"
// snapshot into EC_OldValue (used solely for a later string-compare change
// check, never dereferenced as a pointer again), so it is functionally inert
// even though the memory read itself is technically unsafe. No test in
// tests/test_SecsSvEcRegistration.cpp deliberately exercises this hazard
// (doing so would require actually misusing the API in an unsafe way) --
// exercised only via the genuinely safe `void *P` overloads, whose stored
// pointers really do point at raw scalars.
//---------------------------------------------------------------------------
AnsiString SecsSvEcRegistration::GetECDataValue(AnsiString ECID)
{
    int i;
    unsigned char Type = 0;
    AnsiString ECName;
    AnsiString ECUnit, IsVCL, VCLStr;
    void *P = NULL;

    char                VCL_ASCII[10000];
    char                VCL_BINARY = 0;
    char                VCL_INT1 = 0;
    short               VCL_INT2 = 0;
    int                 VCL_INT4 = 0;
    long long           VCL_INT8 = 0;             // golden __int64
    unsigned char       VCL_UINT1 = 0, VCL_BOOLEAN = 0;
    unsigned short      VCL_UINT2 = 0;
    unsigned int        VCL_UINT4 = 0;
    unsigned long long  VCL_UINT8 = 0;            // golden unsigned __int64
    float               VCL_FT4 = 0.0f;
    double              VCL_FT8 = 0.0;

    i = EC_ID->IndexOf(ECID);
    if (i >= 0)
    {
        if (EC_VCL_NAME->Count > i)
            IsVCL = EC_VCL_NAME->GetString(i);
        else
            IsVCL = 0;

        if (IsVCL == 1 || IsVCL == 2)
        {
            if (IsVCL == 1)
            {
                // AI(W906-VCW1) 20260721: UN-GATED -- golden
                // uHGemEquipment.cpp:3684-3762 (dynamic_casts VclP, a
                // TObject*, against TPanel/TCustomEdit/TComboBox/TLabel/
                // TCheckBox/TRadioGroup/TStringList in turn). vclcompat/
                // Controls.h (this wave) now supplies all 6 previously-
                // missing widget stand-ins. Still UNREACHABLE from any call
                // path in THIS unit's own scope, per the "REGISTRATION-TIME
                // REACHABILITY PROOF" header comment above (un-gating does
                // not change that proof -- it only concerns THIS unit's own
                // 4 SetECDataPointer callers, not a future S2F13/S2F15
                // caller) -- but now compiles and dispatches correctly
                // instead of returning a conservative "" default, matching
                // golden's real logic for whenever that future caller
                // arrives.
                Type    = (unsigned char)atoi(EC_TYPE->GetString(i).c_str());
                ECName  = EC_NAME->GetString(i);
                ECUnit  = EC_UNIT->GetString(i);
                TObject *VclP = (TObject *)EC_Ptr->GetItem(i);

                TPanel      *PanelPtr;
                TCustomEdit *EditPtr;
                TComboBox   *ComboBoxPtr;
                TLabel      *LabelPtr;
                TCheckBox   *CheckBoxPtr;
                TRadioGroup *RadioGroupPtr;
                TStringList *StringListPtr;

                PanelPtr      = dynamic_cast<TPanel       *>(VclP);
                EditPtr       = dynamic_cast<TCustomEdit  *>(VclP);
                ComboBoxPtr   = dynamic_cast<TComboBox    *>(VclP);
                LabelPtr      = dynamic_cast<TLabel       *>(VclP);
                CheckBoxPtr   = dynamic_cast<TCheckBox    *>(VclP);
                RadioGroupPtr = dynamic_cast<TRadioGroup  *>(VclP);
                StringListPtr = dynamic_cast<TStringList  *>(VclP);

                if (StringListPtr != NULL)
                    VCLStr = StringListPtr->CommaText;
                else if (PanelPtr != NULL)
                    VCLStr = PanelPtr->Caption;
                else if (EditPtr != NULL)
                    VCLStr = EditPtr->Text;
                else if (ComboBoxPtr != NULL)
                {
                    if (Type == HType.ASCII_TYPE)
                        VCLStr = ComboBoxPtr->Text;
                    else
                        VCLStr = ComboBoxPtr->ItemIndex;
                }
                else if (RadioGroupPtr != NULL)
                {
                    if (RadioGroupPtr->ItemIndex >= 0)
                    {
                        if (Type == HType.ASCII_TYPE)
                            VCLStr = RadioGroupPtr->Items->Strings[RadioGroupPtr->ItemIndex];
                        else
                            VCLStr = RadioGroupPtr->ItemIndex;
                    }
                    else
                    {
                        if (Type == HType.ASCII_TYPE)
                            VCLStr = "";
                        else
                            VCLStr = "-1";
                    }
                }
                else if (LabelPtr != NULL)
                    VCLStr = LabelPtr->Caption;
                else if (CheckBoxPtr != NULL)
                {
                    Type = HType.BOOLEAN_TYPE;
                    VCLStr = CheckBoxPtr->Checked ? "1" : "0";
                }
                else
                    VCLStr = "";
            }
            else   // IsVCL==2 -- golden uHGemEquipment.cpp:3763-3767, fully
                   // translatable (no missing types needed) -- kept ACTIVE.
            {
                AnsiString *SS;
                SS = (AnsiString *)EC_Ptr->GetItem(i);
                VCLStr = *SS;
            }

            // golden uHGemEquipment.cpp:3769-3818 -- VCLStr -> scratch `P`,
            // keyed by Type. Shared golden code (not duplicated per
            // sub-branch above) -- applies uniformly regardless of which
            // sub-branch set VCLStr.
            if (Type == HType.ASCII_TYPE)
            {
                // AI(W5-Final-SecsSvEcRegistration) 20260711: preserved
                // hazard (not fixed) -- golden itself never bounds-checks
                // VCLStr against the 10000-byte VCL_ASCII buffer here.
                strcpy(VCL_ASCII, VCLStr.c_str());
                P = VCL_ASCII;
            }
            else if (Type == HType.BINARY_TYPE)
            {
                VCL_BINARY = (char)atoi(VCLStr.c_str());
                P = &VCL_BINARY;
            }
            else if (Type == HType.BOOLEAN_TYPE)
            {
                VCL_BOOLEAN = (unsigned char)atoi(VCLStr.c_str());
                P = &VCL_BOOLEAN;
            }
            else if (Type == HType.INT_1_TYPE)
            {
                VCL_INT1 = (char)atoi(VCLStr.c_str());
                P = &VCL_INT1;
            }
            else if (Type == HType.INT_2_TYPE)
            {
                VCL_INT2 = (short)atoi(VCLStr.c_str());
                P = &VCL_INT2;
            }
            else if (Type == HType.INT_4_TYPE)
            {
                VCL_INT4 = atoi(VCLStr.c_str());
                P = &VCL_INT4;
            }
            else if (Type == HType.INT_8_TYPE)                          // Steven 20140911
            {
                // AI(W5-Final-SecsSvEcRegistration) 20260711: golden calls
                // Borland/MSVC `_atoi64` here -- not declared by MinGW's
                // <cstdlib>. Substituted with the standard `strtoll` (same
                // decimal-text-to-int64 semantics) -- a portability
                // accommodation, zero behavior change for any well-formed
                // decimal input (the only kind VCLStr ever holds).
                VCL_INT8 = strtoll(VCLStr.c_str(), NULL, 10);
                P = &VCL_INT8;
            }
            else if (Type == HType.UINT_1_TYPE)
            {
                VCL_UINT1 = (unsigned char)atoi(VCLStr.c_str());
                P = &VCL_UINT1;
            }
            else if (Type == HType.UINT_2_TYPE)
            {
                VCL_UINT2 = (unsigned short)atoi(VCLStr.c_str());
                P = &VCL_UINT2;
            }
            else if (Type == HType.UINT_4_TYPE)
            {
                VCL_UINT4 = (unsigned)atoi(VCLStr.c_str());
                P = &VCL_UINT4;
            }
            else if (Type == HType.UINT_8_TYPE)                         // Steven 20140911
            {
                // AI(W5-Final-SecsSvEcRegistration) 20260711: same
                // `_atoi64` portability substitution as INT_8_TYPE above
                // (`strtoull`, the unsigned counterpart).
                VCL_UINT8 = strtoull(VCLStr.c_str(), NULL, 10);
                P = &VCL_UINT8;
            }
            else if (Type == HType.FT_4_TYPE)
            {
                VCL_FT4 = (float)atof(VCLStr.c_str());
                P = &VCL_FT4;
            }
            else if (Type == HType.FT_8_TYPE)
            {
                VCL_FT8 = atof(VCLStr.c_str());
                P = &VCL_FT8;
            }
        }
        else
        {
            Type   = (unsigned char)atoi(EC_TYPE->GetString(i).c_str());
            ECName = EC_NAME->GetString(i);
            ECUnit = EC_UNIT->GetString(i);
            P      = EC_Ptr->GetItem(i);
        }

        if (Type == HType.ASCII_TYPE)
        {
            char *Ptr;
            Ptr = (char *)P;
            return AnsiString(Ptr);
        }
        else if (Type == HType.BINARY_TYPE)
        {
            char *Ptr;
            Ptr = (char *)P;
            return AnsiString(*Ptr);        // raw 1-char string (AnsiString(char) exact match) --
                                             // NOT decimal-formatted; see file-head discovered-quirk note below.
        }
        else if (Type == HType.BOOLEAN_TYPE)
        {
            unsigned char *Ptr;
            Ptr = (unsigned char *)P;
            return AnsiString(*Ptr);        // promotes to int -> decimal "0"/"1"
        }
        else if (Type == HType.INT_1_TYPE)
        {
            char *Ptr;
            Ptr = (char *)P;
            // AI(W5-Final-SecsSvEcRegistration) 20260711 DISCOVERED QUIRK
            // (preserved, not fixed): unlike SecsWireCodec::DataItemIn's
            // INT_1_TYPE handling (which explicitly does `Str=int(P);` --
            // a decimal-formatted string), THIS golden function does the
            // literal `return AnsiString(*Ptr);` with no int() cast, so
            // vclcompat's overload resolution picks the EXACT `AnsiString
            // (char)` ctor (a raw 1-character string), not the `AnsiString
            // (int)` decimal-formatting ctor. A genuine asymmetry between
            // these two independently-written golden functions for the
            // SAME SECS-II type, reproduced faithfully in each translation.
            return AnsiString(*Ptr);
        }
        else if (Type == HType.INT_2_TYPE)
        {
            short *Ptr;
            Ptr = (short *)P;
            return AnsiString(*Ptr);
        }
        else if (Type == HType.INT_4_TYPE)
        {
            int *Ptr;
            Ptr = (int *)P;
            return AnsiString(*Ptr);
        }
        else if (Type == HType.INT_8_TYPE)                              // Steven 20140911
        {
            long long *Ptr;
            Ptr = (long long *)P;
            return AnsiString(*Ptr);
        }
        else if (Type == HType.UINT_1_TYPE)
        {
            unsigned char *Ptr;
            Ptr = (unsigned char *)P;
            return AnsiString(*Ptr);
        }
        else if (Type == HType.UINT_2_TYPE)
        {
            unsigned short *Ptr;
            Ptr = (unsigned short *)P;
            return AnsiString(*Ptr);
        }
        else if (Type == HType.UINT_4_TYPE)
        {
            unsigned int *Ptr;
            Ptr = (unsigned int *)P;
            return AnsiString(*Ptr);
        }
        else if (Type == HType.UINT_8_TYPE)                             // Steven 20140911
        {
            unsigned long long *Ptr;
            Ptr = (unsigned long long *)P;
            // AI(W5-Final-SecsSvEcRegistration) 20260711: vclcompat
            // AnsiString has no operator matching `unsigned long long`
            // uniquely (ambiguous between the int/long/long-long ctors
            // otherwise) -- narrowed via (long long), the exact same
            // accommodation SecsWireCodec.cpp already established for its
            // own UINT_8_TYPE handling (see that file's own inline comment).
            return AnsiString((long long)(*Ptr));
        }
        else if (Type == HType.FT_4_TYPE)
        {
            float *Ptr;
            Ptr = (float *)P;
            return AnsiString(*Ptr);
        }
        else if (Type == HType.FT_8_TYPE)
        {
            double *Ptr;
            Ptr = (double *)P;
            return AnsiString(*Ptr);
        }
        return "";
    }
    return "";
}
