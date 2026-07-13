//---------------------------------------------------------------------------
//  SECSGEM/SecsSvEcRegistration.h -- THGem's SV/EC registration API
//
//  Translation wave: W5-Final (SECSGEM SV/EC registration API)
//  Translator: AI(W5-Final-SecsSvEcRegistration) 20260711
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.h
//  :647-670 (SV_*/EC_* member declarations) + uHGemEquipment.cpp:5648-5730
//  (SetSVDataPointer x4) + :5732-6025 (SetECDataPointer x4, including the
//  registration-safe path through GetECDataValue at :3650-3925).
//
//  SCOPE-BOUNDS CORRECTION (flag per task instructions, item 8): the task's
//  original estimate for the SetECDataPointer region was "5732-6165". Reading
//  golden directly (grep-verified) shows the 4th/last SetECDataPointer
//  overload's body actually ends at line 6025 (the next function,
//  THGem::SetTimeFormat, starts at :6033) -- there is no function content
//  between 6025 and 6165 relevant to this unit's 8 overloads. Golden/actual
//  code wins per the ground rules; the whole 8-overload span is materially
//  SHORTER than estimated (5648-6025, ~378 lines total across both families,
//  not counting GetECDataValue).
//
//  WHAT THIS FILE IS
//  ------------------
//  `THGem` (uHGemEquipment.h/.cpp) is the ~6200-line VCL TForm that is the
//  real SECS/GEM engine (see SecsWireCodec.h's file-head note -- THGem
//  itself remains OUT OF SCOPE, deferred to a dedicated future wave). This
//  file carries THGem's SV (Status Variable) / EC (Equipment Constant)
//  REGISTRATION API: the bookkeeping lists an AP-side caller (uHGemHT9045_SV
//  .cpp / uHGemHT9045_EC.cpp, both explicitly OUT OF SCOPE here -- see the
//  hand-off prompt's own framing) populates once at startup via ~877
//  SetSVDataPointer(...) and ~1740 SetECDataPointer(...) calls, plus the one
//  read-back primitive (GetECDataValue) those calls need internally to seed
//  each EC's "old value" snapshot.
//
//  Translating this unit does NOT make uHGemHT9045_SV.cpp/_EC.cpp themselves
//  translatable -- each call's target parameter (a global/struct member, or
//  in ~dozens of cases a live VCL widget pointer like `fLotInfo->edtSysLotID`)
//  still needs its own home in a future wave; per a prior assessment only 7
//  of ~28 referenced forms have ANY translated presence today. This unit only
//  makes the ~2600 calls' CALL SHAPE mechanically compilable once their
//  arguments resolve.
//
//  DESIGN: SEPARATE CLASS, NOT A SecsWireCodec EXTENSION
//  -------------------------------------------------------
//  The hand-off prompt asked me to check whether the Phase-1 sibling unit's
//  `SecsWireCodec` shell (SECSGEM/SecsWireCodec.h, read in full before
//  writing this file) naturally fits this unit's 8 methods + SV/EC list
//  members. It does not: SecsWireCodec is explicitly scoped as "the pure
//  byte-level SECS-II item codec slice of THGem" (its own file-head docs) --
//  LocalBuffer/DataItemOut/DataItemIn/GetLengthOfType/etc. None of those are
//  touched by SetSVDataPointer/SetECDataPointer/GetECDataValue, and vice
//  versa (this unit never builds/parses a wire message). Bolting SV/EC
//  registration state onto the wire-codec class would conflate two genuinely
//  separate THGem concerns that happen to both be "THGem slices" -- the task
//  prompt itself names this file `SecsSvEcRegistration.h/.cpp` (a distinct
//  name from SecsWireCodec), which further confirms a separate class is
//  intended. A FUTURE full-THGem translation wave would hold both as
//  independent members (or bases) of THGem, exactly as golden's THGem holds
//  both concerns as independent member groups today.
//
//  SHARED GLOBAL REUSED: HType
//  -----------------------------
//  GetECDataValue's Type-dispatch (see .cpp) compares against the SAME
//  process-wide `HType` global that SecsWireCodec.cpp already defines and
//  seeds (SecsWireCodec.h's "SHARED GLOBAL: HType" note) -- reused via
//  `#include "SECSGEM/SecsWireCodec.h"` here, NOT re-declared (ODR-safe,
//  exactly the reuse pattern that unit's own header flagged for a future
//  consumer to follow).
//
//  INTEGRATE-AGENT WIRING POINT (flag for a human / the THGem wave):
//    A FUTURE wave translating the real THGem for uHGemEquipment.h/.cpp
//    MUST `#include "SECSGEM/SecsSvEcRegistration.h"` and hold a
//    `SecsSvEcRegistration` member (or subclass it) rather than
//    re-implementing SetSVDataPointer/SetECDataPointer/GetECDataValue --
//    those already live here. Likewise, the eventual _SV.cpp/_EC.cpp
//    translation wave should call through a `SecsSvEcRegistration` instance
//    (or a THGem that embeds one) rather than a bespoke registration API.
//---------------------------------------------------------------------------
#ifndef SECSGEM_SECSSVECREGISTRATION_H
#define SECSGEM_SECSSVECREGISTRATION_H

#include "vclcompat/vcl_compat.h"
#include "vclcompat/TList.h"
#include "SECSGEM/SecsWireCodec.h"   // reuse the shared HType global (see note above)

//---------------------------------------------------------------------------
//  SecsSvEcRegistration -- the SV/EC registration-bookkeeping slice of THGem.
//---------------------------------------------------------------------------
class SecsSvEcRegistration
{
public:
    SecsSvEcRegistration();
    ~SecsSvEcRegistration();

    // Owns 18 heap TStringList*/TList*s -- forbid copy, matching the
    // SecsWireCodec/TStringList/TList precedent (golden THGem is likewise
    // never copied).
    SecsSvEcRegistration(const SecsSvEcRegistration&) = delete;
    SecsSvEcRegistration& operator=(const SecsSvEcRegistration&) = delete;

    // ==== SV bookkeeping (golden THGem members, uHGemEquipment.h:647-654) ===
    // Parallel arrays, one slot per registered SV, indexed identically
    // (SV_ID->IndexOf(id) gives the row for every other SV_* list).
    TStringList      *SV_ID;       // SVID (decimal-string form of the numeric ID)
    TStringList      *SV_TYPE;     // SECS-II format byte (decimal-string form)
    TStringList      *SV_NAME;
    TStringList      *SV_UNIT;
    vclcompat::TList *SV_Ptr;      // void* -- raw ptr / AnsiString* / TObject*, per VCL_NAME
    TStringList      *VCL_NAME;    // "0"=raw ptr, "1"=TObject* (VCL widget), "2"=AnsiString*
    TStringList      *SV_LEN;      // array length (element count), default "1"
    TStringList      *SV_Remark;

    // ==== EC bookkeeping (golden THGem members, uHGemEquipment.h:656-670) ===
    TStringList      *EC_ID;
    TStringList      *EC_TYPE;
    TStringList      *EC_NAME;
    TStringList      *EC_UNIT;
    vclcompat::TList *EC_Ptr;               // void* -- raw ptr / AnsiString* / TObject*
    vclcompat::TList *EC_Ptr_Min;           // golden: always NULL-filled by every Set* overload (see .cpp)
    vclcompat::TList *EC_Ptr_Max;           // golden: always NULL-filled (ditto)
    vclcompat::TList *EC_Ptr_Default;       // golden: always NULL-filled (ditto)
    TStringList      *EC_Remark;
    TStringList      *EC_Ptr_Min_Value;     // decimal-string bound (post min/max auto-swap)
    TStringList      *EC_Ptr_Max_Value;
    TStringList      *EC_Ptr_Default_Value;
    TStringList      *EC_VCL_NAME;          // "0"=raw ptr, "1"=TObject*, "2"=AnsiString*
    TStringList      *EC_OldValue;          // snapshot taken at registration time via GetECDataValue

    // ==== golden THGem methods (faithful; __fastcall dropped per project
    //      convention -- neutralized anyway by vcl_compat.h) ================

    // golden uHGemEquipment.cpp:5648-5668 -- P is a raw pointer to one
    // scalar value of `Type` (the dominant, "safe" registration shape).
    void SetSVDataPointer(AnsiString SVID, unsigned char Type, AnsiString SVName,
                          AnsiString SVUnit, void *P, AnsiString Remark);

    // golden uHGemEquipment.cpp:5669-5689 -- P points at a live AnsiString
    // variable (VCL_NAME tagged "2"); see GetECDataValue's file-head note in
    // the .cpp for the registration-time read-back hazard this creates.
    void SetSVDataPointer(AnsiString SVID, unsigned char Type, AnsiString SVName,
                          AnsiString SVUnit, AnsiString *P, AnsiString Remark);

    // golden uHGemEquipment.cpp:5690-5711 -- P is a raw pointer to an ARRAY
    // of `Len` values of `Type` (SV_LEN records Len instead of the default 1).
    void SetSVDataPointer(AnsiString SVID, unsigned char Type, AnsiString SVName,
                          AnsiString SVUnit, void *P, int Len, AnsiString Remark);

    // golden uHGemEquipment.cpp:5712-5730 -- P is a live VCL widget pointer
    // (VCL_NAME tagged "1"); see GetECDataValue's file-head note.
    void SetSVDataPointer(AnsiString SVID, unsigned char Type, AnsiString SVName,
                          AnsiString SVUnit, TObject *P, AnsiString Remark);

    // golden uHGemEquipment.cpp:5732-5769 -- Min/Max/Default are supplied as
    // AnsiString (auto-swapped if Min>Max, numerically, via atof).
    void SetECDataPointer(AnsiString ECID, unsigned char Type, AnsiString ECName,
                          AnsiString ECUnit, AnsiString *P,
                          AnsiString MinValue, AnsiString MaxValue,
                          AnsiString DefaultValue, AnsiString Remark);

    // golden uHGemEquipment.cpp:5771-5809 -- Min/Max/Default supplied as int
    // (auto-swapped if MinValue>MaxValue). Also calls SetECEnableData (gated,
    // see .cpp) -- golden's own "pig 2014.08.27 KYEC" marker.
    void SetECDataPointer(AnsiString ECID, unsigned char Type, AnsiString ECName,
                          AnsiString ECUnit, void *P,
                          int MinValue, int MaxValue, int DefaultValue,
                          AnsiString Remark);

    // golden uHGemEquipment.cpp:5811-5987 -- P is a live VCL widget pointer;
    // Min/Max/Default are C strings.
    // AI(W5-Final-SecsSvEcRegistration) 20260711 JUDGMENT CALL (flag for
    // review): golden declares these 3 params as non-const `char *` (BCB6
    // accepted passing string-literal `""` args, e.g. uHGemHT9045_EC.cpp
    // call sites, into a non-const char* parameter -- a const-discarding
    // conversion standard C++ rejects). Declared `const char*` here instead,
    // matching the EXACT same accommodation SecsWireCodec.cpp already made
    // for THGem::DataItemOut's `Ptr` (see that file's own inline comment) --
    // zero behavior change (the golden body only ever READS through these
    // pointers via atof(), never writes), and it lets a future _EC.cpp
    // translation wave pass string literals at ~dozens of call sites without
    // needing a cast at every one.
    void SetECDataPointer(AnsiString ECID, unsigned char Type, AnsiString ECName,
                          AnsiString ECUnit, TObject *P,
                          const char *MinValue, const char *MaxValue,
                          const char *DefaultValue, AnsiString Remark);

    // golden uHGemEquipment.cpp:5989-6025 -- P is a raw pointer; Min/Max/
    // Default supplied as AnsiString (auto-swapped via atof, like overload 1).
    void SetECDataPointer(AnsiString ECID, unsigned char Type, AnsiString ECName,
                          AnsiString ECUnit, void *P,
                          AnsiString Min, AnsiString Max, AnsiString Default,
                          AnsiString Remark);

    // golden uHGemEquipment.cpp:3650-3925 (THGem::GetECDataValue) -- reads
    // back a registered EC's CURRENT value as text. Called internally by
    // every SetECDataPointer overload above (to seed EC_OldValue at
    // registration time) -- see the "REGISTRATION-TIME REACHABILITY PROOF"
    // note on this method's definition in the .cpp for why that internal use
    // is always safe/reachable even though this method also has a dead
    // (gated) VCL-widget branch that only a FUTURE, out-of-scope caller
    // (S2F13/S2F15 handlers, golden :3968/:4000) could ever reach.
    //
    // CONFIRMED GOLDEN BUG (see "SECOND DISCOVERED BUG" note in the .cpp,
    // proven by a failing test before being documented): for an
    // AnsiString*-backed EC (VCL_NAME=="2"), this method ALWAYS returns ""
    // when queried after its own registration moment -- golden's `Type`
    // local is never assigned on that code path (only the IsVCL==1
    // sub-branch assigns it), so every Type-keyed dispatch after it silently
    // falls through. Preserved verbatim, not fixed.
    AnsiString GetECDataValue(AnsiString ECID);
};

#endif // SECSGEM_SECSSVECREGISTRATION_H
