//---------------------------------------------------------------------------
//  SECSGEM/SecsWireCodec.h -- SECS-II wire-level codec primitives
//
//  Translation wave: W5-Final (SECSGEM wire codec)
//  Translator: AI(W5-Final-SecsWireCodec) 20260711
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.h
//  :43-85 (HTypeStruct/STypeStruct/HSMS_Head_Struct) + uHGemEquipment.cpp
//  (exact per-function golden citations are on each declaration below; see
//  SecsWireCodec.cpp file header for the "SCOPE DISCREPANCY" note on 2
//  extra helper functions pulled in beyond the task's original line range).
//
//  WHAT THIS FILE IS
//  ------------------
//  `THGem` (uHGemEquipment.h/.cpp) is the ~6200-line VCL TForm that is the
//  actual SECS/GEM engine: connection state, the S,F message handlers,
//  StringGrid/TMemo/TClientSocket/TServerSocket widgets, AND the pure
//  byte<->item wire-level codec this file translates. THGem itself is
//  explicitly OUT OF SCOPE for this unit -- deferred to a dedicated future
//  wave (MIGRATION_ROADMAP.md W5 "SECSGEM 協定引擎" entry). This file only
//  carries THGem's byte-level SECS-II item encode/decode primitives: turning
//  a typed value (int/AnsiString/float/...) into the SECS-II
//  Format-byte + Length-bytes + Data wire encoding used by DataItemOut, and
//  the inverse decode (DataItemIn family) that a future S,F handler wave
//  will call.
//
//  DESIGN: METHODS ON A PLAIN CLASS, NOT FREE FUNCTIONS
//  ------------------------------------------------------
//  Golden's 8 functions are THGem *methods* that mutate persistent
//  per-connection state across calls (LocalLength_4 advances with every
//  DataItemOut call in a message-build burst; SReceiveData shrinks by
//  Delete(0) with every DataItemIn* call in a message-decode burst). Per
//  this project's "interface-cut stateful engine" convention (KNOWLEDGE.md:
//  e.g. TMySimMotor holds its own persistent motion state as instance
//  members, not free-function statics), that state is modeled here as
//  instance members of a small standalone class, `SecsWireCodec`, rather
//  than free functions threading a dozen buffer/cursor parameters, and
//  rather than reusing file-scope `static` locals (this genuinely IS
//  per-instance state -- a future wave may need >1 live SECS connection).
//  This class is NOT, and is not trying to be, a THGem shell: it has no
//  Connect/OnLine/S1Fxx/... surface at all.
//
//  SHARED GLOBAL: HType
//  ---------------------
//  Golden declares `struct HTypeStruct HType;` as a FILE-SCOPE GLOBAL (NOT a
//  THGem member!) at uHGemEquipment.cpp:22, extern-declared in the header at
//  uHGemEquipment.h:729, and populated once by THGem::InitHType() (golden
//  :353-368) with the literal SECS-II format-byte codes (0x00 List,
//  0x40 ASCII, ... 0x80 FT_8). Every one of GetLengthOfType / DataItemOut /
//  DataItemInSub / DataItemIn compares against this ONE global table, so it
//  MUST be a real global here too (not a SecsWireCodec instance member) to
//  stay faithful -- two SecsWireCodec instances (e.g. two future Tester
//  connections) share the identical format-code table, exactly as any two
//  THGem instances would in golden.
//
//  This header is therefore now the CANONICAL HOME of `HTypeStruct` / the
//  `HType` global instance. Along for the ride -- because they are declared
//  on the very same golden lines (43-85) and have zero dependency of their
//  own -- the sibling PODs `STypeStruct` / `HSMS_Head_Struct` are declared
//  here too as plain *types* (no live instance): golden's `SType` (a THGem
//  *member* of type STypeStruct, populated by the out-of-scope
//  InitSTypeStruct()) and the Local/Remote HSMS message heads are genuine
//  THGem-instance state that belongs to the future full-THGem translation
//  wave, NOT to this codec slice -- neither is instantiated here.
//
//  INTEGRATE-AGENT WIRING POINT (flag for a human / the THGem wave):
//    A FUTURE wave that translates uHGemEquipment.h/.cpp for the real THGem
//    MUST `#include "SECSGEM/SecsWireCodec.h"` for `HTypeStruct` /
//    `STypeStruct` / `HSMS_Head_Struct` / the `HType` global instead of
//    re-declaring them (ODR violation risk otherwise). THGem would then hold
//    a `SecsWireCodec` member (or subclass it) rather than re-implementing
//    GetLengthOfType/DataItemOut/DataItemInSub/DataItemIn/GetSMLLenthByte/
//    GetLengthByte/StringOut(1-arg)/DataItemInNew -- those 8 (+2, see .cpp)
//    already live here.
//---------------------------------------------------------------------------
#ifndef SECSGEM_SECSWIRECODEC_H
#define SECSGEM_SECSWIRECODEC_H

#include "vclcompat/vcl_compat.h"
#include <vector>

//---------------------------------------------------------------------------
//  Golden uHGemEquipment.h:43-59 -- SECS-II format-byte lookup table shape.
//  Field order/types are POD and preserved verbatim (bit-layout is not
//  load-bearing here -- every consumer accesses fields by name, never by
//  offset/sizeof -- but preserving golden's declared order costs nothing and
//  keeps a byte-for-byte diff-able transcription).
//---------------------------------------------------------------------------
struct HTypeStruct
{
    unsigned char LIST_TYPE;
    unsigned char ASCII_TYPE;
    unsigned char JIS_TYPE;
    unsigned char BINARY_TYPE;
    unsigned char BOOLEAN_TYPE;
    unsigned char UINT_1_TYPE;
    unsigned char UINT_2_TYPE;
    unsigned char UINT_4_TYPE;
    unsigned char UINT_8_TYPE;
    unsigned char INT_1_TYPE;
    unsigned char INT_2_TYPE;
    unsigned char INT_4_TYPE;
    unsigned char INT_8_TYPE;
    unsigned char FT_4_TYPE;
    unsigned char FT_8_TYPE;
    unsigned char VCL_TYPE;
};

// Golden uHGemEquipment.h:61-70 -- HSMS message-kind byte values (SType is a
// THGem *instance member* in golden, populated by InitSTypeStruct(), out of
// this unit's scope). Type declared here only, per the file-head note above.
struct STypeStruct
{
    unsigned char Data_Message;
    unsigned char Select_req;
    unsigned char Select_rsp;
    unsigned char Deselect_req;
    unsigned char Deselect_rsp;
    unsigned char Linktest_req;
    unsigned char Linktest_rsp;
    unsigned char Reject_req;
    unsigned char Separate_req;
};

// Golden uHGemEquipment.h:72-80 -- HSMS message head shape (Local/Remote
// heads are THGem *instance state*, out of this unit's scope). Type declared
// here only, per the file-head note above.
struct HSMS_Head_Struct
{
    unsigned short int DeviceID;
    unsigned char MessageID_S;
    unsigned char MessageID_F;
    unsigned W_Bit;
    unsigned char PType;
    unsigned char SType;
    unsigned int SystemByte;
};

// Golden uHGemEquipment.h:729 `extern struct HTypeStruct HType;` /
// uHGemEquipment.cpp:22 `struct HTypeStruct HType;` -- see file-head note:
// this IS the same process-wide global, defined + seeded in SecsWireCodec.cpp.
extern struct HTypeStruct HType;

//---------------------------------------------------------------------------
//  SecsWireCodec -- the pure byte-level SECS-II item codec slice of THGem.
//---------------------------------------------------------------------------
class SecsWireCodec
{
public:
    SecsWireCodec();
    ~SecsWireCodec();

    // Owns a large buffer + 3 heap TStringList*s -- forbid copy (matches the
    // vclcompat TStringList's own no-copy stance; golden THGem is likewise
    // never copied, always used through a single pointer/instance).
    SecsWireCodec(const SecsWireCodec&) = delete;
    SecsWireCodec& operator=(const SecsWireCodec&) = delete;

    // ---- outbound encode state (golden THGem members) ---------------------
    // Golden: `unsigned char LocalBuffer[64*1024*1024];` -- a FIXED in-object
    // array member (uHGemEquipment.h:205).
    // AI(W5-Final-SecsWireCodec) 20260711 JUDGMENT CALL (flag for review):
    // heap-backed std::vector<unsigned char> of the SAME 64MB size, instead
    // of an embedded fixed array, so SecsWireCodec stays safely stack-
    // instantiable (a 64MB embedded array would make every by-value/local
    // SecsWireCodec instance -- e.g. in a test harness -- a 64MB stack frame
    // guest). Index semantics, capacity, and byte layout are identical to
    // golden; only the storage strategy differs. Alternative considered:
    // keep the fixed array (rejected -- no test/consumer benefits from an
    // embedded array over a heap one, and the stack-safety risk is real).
    std::vector<unsigned char> LocalBuffer;
    unsigned LocalLength;     // golden uHGemEquipment.h:203 (final encoded length, header-exclusive)
    unsigned LocalLength_4;   // golden uHGemEquipment.h:204 (running write cursor, header-inclusive)

    // ---- inbound decode state (golden THGem member) ------------------------
    // Golden `TStringList *SReceiveData;` (uHGemEquipment.h:672) holds the
    // decoded message as a flat sequence of decimal-string tokens
    // ("Type","len","value","value",...); DataItemInSub/GetDataItemLenAndType*
    // consume it strictly left-to-right via Delete(0). Populating this list
    // from a raw received byte stream is ProcessSML's job (out of scope --
    // golden uHGemEquipment.h:256/257); tests push tokens directly.
    TStringList *SReceiveData;

    // ---- display/log sinks (golden THGem members) --------------------------
    // Golden `TStringList *WaitShowString;` (uHGemEquipment.h:597) /
    // `TStringList *LogDataString;` (uHGemEquipment.h:686) -- StringOut's
    // 1-arg overload appends the same line to BOTH (golden :392-396).
    TStringList *WaitShowString;
    TStringList *LogDataString;

    // Golden `int iReturnCode;` (uHGemEquipment.h public section) -- a
    // "sticky" decode-error code: once a DataItemIn* call in a burst returns
    // non-1, iReturnCode latches to that error and later successful calls in
    // the same burst do NOT clear it (see golden comments on every one of
    // these functions: "初始值=1,...避免錯誤碼被後來的正確碼所取代"). Reset it
    // to 1 yourself before starting a fresh decode burst (golden resets it
    // inside ProcessReceiceData, out of scope here).
    int iReturnCode;

    // ADDITIVE (NOT a golden method): resets the outbound-encode cursor.
    // Golden's CreateLocalHead() (uHGemEquipment.h:198, out of scope here --
    // it writes the 10-byte HSMS head into LocalBuffer[4..13], which needs
    // the Local/Remote HSMS_Head_Struct + live connection state) is what
    // actually primes LocalLength_4=4 before a DataItemOut() burst in golden.
    // This hook reproduces just that one side effect so DataItemOut is
    // independently exercisable without the rest of THGem. See .cpp.
    void ResetLocalBuffer();

    // ==== golden THGem methods (faithful; __fastcall dropped per project
    //      convention -- neutralized anyway by vcl_compat.h) ================

    // golden uHGemEquipment.cpp:392-396 (1-arg overload only; the 2-arg
    // StringOut(AnsiString,TColor) touches the `DB` TMemo widget -- VCL,
    // out of scope).
    void StringOut(AnsiString S);

    int GetLengthOfType(unsigned char Type);                       // golden :917-932
    unsigned char GetLengthByte(unsigned len, unsigned char *Ptr);  // golden :941-974
    int GetSMLLenthByte(unsigned char TypeChar, unsigned char *Ptr, int RunLength); // golden :1235-1244

    void DataItemOut(unsigned char Type, AnsiString S);   // golden :979-984
    void DataItemOut(int len, unsigned char Type, void *P); // golden :986-1183

    int DataItemInSub(int len, unsigned char Type, void *P); // golden :2225-2432

    // Peek-only length/type reader -- a DIRECT prerequisite of DataItemInNew
    // (below) that the task's original scope line-range (~7114-7134) did not
    // enumerate. See .cpp file-head "SCOPE DISCREPANCY" note.
    int GetDataItemLenAndTypeSub(int &len, unsigned char &Type);  // golden :2440-2447
    int GetDataItemLenAndType(int &len, unsigned char &Type);     // golden :7290-7296 (sticky iReturnCode wrapper)

    int DataItemInNew(AnsiString &P);                     // golden :7114-7122
    int DataItemIn(int len, unsigned char Type, void *P); // golden :7125-7130
    int DataItemIn(int len, unsigned char t, AnsiString &Str); // golden :7134-7281
};

#endif // SECSGEM_SECSWIRECODEC_H
