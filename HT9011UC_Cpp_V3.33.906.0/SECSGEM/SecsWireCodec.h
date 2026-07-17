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
//  WAVE 2 ADDENDUM (20260713): 15 more THGem methods added -- the frame
//  header builder/parser (ConvertLocalData/CreateLocalHead/InitLocalHead/
//  HostConvertToData/ProcessRemoteHead/LocalAcknowledge/StoreToReceiveString)
//  and the byte-stream decoder/pretty-printer (ProcessSML/ProcessSMLBinary/
//  ShowSML/ShowSMLBinary/MakeSMLSpaceString/ShowSMLSpaceString/
//  ShowSMLSpaceBinaryString/ShowSFDescription), plus 2 direct-callee
//  prerequisites (StringBinaryOut, SendLocalData -- GATED, see .cpp). This
//  is why `Local`/`Remote` (HSMS_Head_Struct) are now live instance members
//  below instead of type-only declarations -- see the updated note further
//  down. Per-function golden citations are on each Wave-2 declaration below;
//  see SecsWireCodec.cpp's own "WAVE 2" file-head block for the full
//  quirks/gated-widget rundown.
//
//  WAVE 3 ADDENDUM (AI(W906-uHGemClass-Unlock2) 20260716): 3 more THGem
//  methods added, all confirmed by direct golden read to be pure in-memory
//  buffer bookkeeping / wire composition on state this class already owns --
//  zero new dependency, zero VCL: GetDataItemLenAndTypeAndDeleteSub/
//  GetDataItemLenAndTypeAndDelete (the destructive "AndDelete" siblings of
//  the peek-only GetDataItemLenAndType(Sub) pair already above -- by far the
//  single most common blocker recorded in uHGemClass.cpp's "INTEGRATE WAVE"
//  note, ~20 of its 47 still-gated methods) and SendInvalidDataMessageToHost
//  (a 3-line InitLocalHead+DataItemOut+SendLocalData composer, golden
//  uHGemEquipment.cpp:7353-7358). See each declaration below for its own
//  citation, and uHGemClass.cpp's own "INTEGRATE WAVE 2" note for exactly
//  which previously-gated methods this un-blocks.
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
//  own -- the sibling POD `STypeStruct` is declared here too as a plain
//  *type* (no live instance): golden's `SType` (a THGem *member* of type
//  STypeStruct, populated by the out-of-scope InitSTypeStruct()) remains
//  genuine THGem-instance state that belongs to the future full-THGem
//  translation wave, NOT to this codec slice -- it is not instantiated here.
//
//  UPDATE (Wave 2, frame-header builder/parser + SML decoder, this file's
//  second translation pass): `HSMS_Head_Struct` is NO LONGER type-only --
//  golden's `Local`/`Remote` instance members (uHGemEquipment.h:396) ARE now
//  live `SecsWireCodec` members too, because `CreateLocalHead`/
//  `InitLocalHead`/`ConvertLocalData` (outbound) and `ProcessRemoteHead`/
//  `HostConvertToData` (inbound) -- all now translated below -- read/write
//  them directly. See each member's own declaration comment further down for
//  its golden citation.
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

// Golden uHGemEquipment.h:72-80 -- HSMS message head shape. Local/Remote
// heads WERE THGem *instance state* out of this unit's original scope; as of
// Wave 2 (see file-head "UPDATE" note above) they are live `Local`/`Remote`
// members of `SecsWireCodec` (declared further down, next to the other
// Wave-2 state) because the frame-header builder/parser methods need them.
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

    // ==== Wave 2 state (golden THGem members) -- frame header + SML decode
    //      pretty-printer. Added alongside the methods that need them below;
    //      see each method's own golden citation for how it is used. ========

    // golden uHGemEquipment.h:396 `struct HSMS_Head_Struct Local,Remote;` --
    // now live instance state (see the .h file-head "UPDATE" note and the
    // HSMS_Head_Struct declaration comment above for why this is no longer
    // type-only). Value-initialized to all-zero in the constructor, matching
    // golden's THGem ctor never explicitly touching `Remote` and only ever
    // setting `Local.DeviceID`/`Local.SystemByte` from a device-ID/counter
    // that golden sources from InitialHGem() (out of scope) -- zero is the
    // faithful "never connected yet" starting point either way.
    struct HSMS_Head_Struct Local;
    struct HSMS_Head_Struct Remote;

    // golden uHGemEquipment.h:211 -- outbound system-byte counter, bumped by
    // InitLocalHead() for odd (primary/request) function codes.
    unsigned int EquipmentSystemByte;

    // golden uHGemEquipment.h:206 -- length of the last message ProcessRemoteHead
    // decoded out of the wire (its own 4-byte length header, exclusive).
    unsigned RemoteLength;

    // golden uHGemEquipment.h:208 -- HostConvertToData's own remaining-bytes
    // countdown for the message currently being walked by ProcessRemoteHead.
    unsigned Host_HSMS_RunLength;

    // golden uHGemEquipment.h (declared alongside TCP_Address/TCP_Port, near
    // iSMLSpace/SpaceStr) -- StoreToReceiveString's "am I mid-decode of an
    // inbound message" gate: ProcessSML calls StoreToReceiveString for EVERY
    // token it walks regardless of direction, but golden only wants tokens
    // actually captured into SReceiveData while decoding real Host data (not
    // e.g. while echoing/tracing a local outbound buffer); golden's
    // ProcessReceiceData (out of scope) sets this true for the duration of an
    // inbound decode. Defaults false (golden THGem ctor: `bReceiveData=false;`)
    // -- tests that want ProcessSML to actually populate SReceiveData must set
    // this true first (see tests/test_SecsWireCodec.cpp).
    bool bReceiveData;

    // golden uHGemEquipment.h:412 -- ProcessSML's file-upload/download fast
    // path selector (BINARY_TYPE items become a raw pointer capture into
    // DownLoadFilePtr instead of being logged/stored token-by-token). Golden
    // flips this true/false only inside S7Fxx/S101Fxx handlers (out of this
    // unit's scope); defaults false (golden THGem ctor).
    bool bDisableBinaryShow;

    // golden uHGemEquipment.h:458 -- non-owning pointer into the CALLER's
    // decode buffer (set by ProcessSML's BINARY_TYPE branch when
    // bDisableBinaryShow is true); nothing in this unit's scope reads it back
    // (the out-of-scope file-download consumer does) -- kept only so
    // ProcessSML's assignment has somewhere faithful to write. Never
    // allocated/freed by SecsWireCodec (not owned).
    char *DownLoadFilePtr;

    // golden uHGemEquipment.h:240 -- ShowSML's own busy flag (set true/false
    // around its own body only; golden's THGem ctor never explicitly
    // initializes it -- defaulted to false here for determinism, which is a
    // harmless normalization since nothing in this unit's scope reads it
    // before ShowSML's own first assignment).
    bool bOutputBusy;

    // golden uHGemEquipment.h:213/214 -- MakeSMLSpaceString's indent-depth
    // counter and its cached "N*2 spaces" string, shared by every SML
    // pretty-printer method below.
    int iSMLSpace;
    AnsiString SpaceStr;

    // ADDITIVE (NOT a golden method): resets the outbound-encode cursor
    // WITHOUT touching Local/Remote or re-zeroing the full 64MB LocalBuffer
    // (unlike the now-real CreateLocalHead() below, which does both plus
    // writes the 10-byte HSMS head from Local). Kept as a lighter-weight
    // alternative for exercising DataItemOut() in isolation (e.g. from a
    // test) without needing a populated Local struct first.
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

    // Wave 3 (AI(W906-uHGemClass-Unlock2) 20260716): destructive (peek+consume)
    // siblings of the peek-only pair immediately above -- SAME family (golden
    // declares all 4 in uHGemEquipment.h: 435-436 are adjacent, but 448 and 452
    // are separated from that pair and from each other by unrelated intervening
    // declarations -- "same family", not literally back-to-back), just the
    // "AndDelete" flavor that also pops the 2 tokens off SReceiveData instead
    // of leaving them for a later re-peek. This is THE most common blocker
    // across uHGemClass.cpp's still-gated methods (see that file's own
    // "INTEGRATE WAVE" note, ~20 of 44) -- a future S,F-handler wave calls
    // this instead of GetDataItemLenAndType once it has decided it's really
    // going to consume the item (GetDataItemLenAndType alone would leave the
    // Type/len tokens sitting in front of the next DataItemIn* call, double-
    // reading them).
    int GetDataItemLenAndTypeAndDeleteSub(int &len, unsigned char &Type);  // golden :2453-2462
    int GetDataItemLenAndTypeAndDelete(int &len, unsigned char &Type);     // golden :7099-7106 (sticky iReturnCode wrapper)

    // Wave 3 (AI(W906-uHGemClass-Unlock2) 20260716): golden uHGemEquipment.cpp
    // :7353-7358 -- THGem's generic "malformed S,F body -> reply S9F7 Illegal
    // Data" sender. Pure WireCodec composition (InitLocalHead+DataItemOut+
    // SendLocalData, the identical 3-call shape LocalAcknowledge/S9F7_
    // IllegalData already use) -- zero new dependency, confirmed by direct
    // golden read (see uHGemClass.cpp's "INTEGRATE WAVE 2" note for which
    // gated callers this un-blocks).
    void SendInvalidDataMessageToHost(AnsiString S);                       // golden uHGemEquipment.cpp:7353-7358

    int DataItemInNew(AnsiString &P);                     // golden :7114-7122
    int DataItemIn(int len, unsigned char Type, void *P); // golden :7125-7130
    int DataItemIn(int len, unsigned char t, AnsiString &Str); // golden :7134-7281

    // ==== Wave 2: frame header builder/parser (golden THGem methods) =======

    void ConvertLocalData(int len, void *Value);                  // golden :847-854
    void CreateLocalHead();                                       // golden :859-889
    void InitLocalHead(int SCode, int FCode, int WBit);           // golden :894-912
    unsigned char *HostConvertToData(unsigned char *ProcessBuffer, int len, void *Value); // golden :2150-2159
    void ProcessRemoteHead(unsigned char *ProcessBuffer);         // golden :2164-2201
    void LocalAcknowledge(unsigned char SCode, unsigned char FCode, unsigned char Command); // golden :2207-2214
    void StoreToReceiveString(AnsiString S);                      // golden :1442-1447

    // PREREQUISITE (not one of the 15 task functions, but a direct callee of
    // LocalAcknowledge -- see .cpp "PREREQUISITE ADDITIONS" note): golden
    // uHGemEquipment.cpp:1985-2107, THGem::SendLocalData() -- dispatches
    // LocalBuffer over a live HSMS TClientSocket/TServerSocket connection.
    // GENUINELY out of scope for this wave (needs the comms layer the W5
    // roadmap entry names as a future prerequisite); GATED here to a minimal
    // stub that preserves the one piece of in-scope state golden mutates
    // (bReceiveData=false) so LocalAcknowledge's call shape stays faithful.
    void SendLocalData();                                         // golden :1985-2107 (GATED, see .cpp)

    // ==== Wave 2: byte-stream decoder / pretty-printer (golden THGem methods)

    // PREREQUISITE (not one of the 15 task functions, but a direct callee of
    // ProcessSMLBinary/ShowSMLBinary/ShowSMLSpaceBinaryString below): golden
    // uHGemEquipment.cpp:401-404 -- body is `//LogDataString->Add(S);`,
    // i.e. commented OUT in golden itself. A genuinely-empty golden function,
    // not a gated widget stub -- translated verbatim as a real no-op.
    void StringBinaryOut(AnsiString S);                           // golden :401-404

    void MakeSMLSpaceString();                                    // golden :1194-1207
    void ShowSMLSpaceString(AnsiString S);                        // golden :1212-1216
    void ShowSMLSpaceBinaryString(AnsiString S);                  // golden :1218-1224 (1 gated checkbox read, see .cpp)

    int ProcessSMLBinary(unsigned char *Ptr, int Len, int &RunLength); // golden :1250-1329
    int ShowSMLBinary(unsigned char *Ptr, int len);               // golden :1334-1345 (1 gated checkbox read, see .cpp)

    // golden uHGemEquipment.cpp:1411-1436. SFCodeAndMean[] (the S,F
    // description lookup table) is transcribed verbatim into SecsWireCodec.cpp
    // as file-static data (cp950-aware read of golden -- see that file's own
    // note); NOT a class member (golden doesn't make it one either -- it's a
    // plain file-scope global in uHGemEquipment.cpp).
    AnsiString ShowSFDescription(unsigned char SCode, unsigned char FCode);

    int ProcessSML(unsigned char *Ptr, int Len, int &RunLength);  // golden :1452-1863
    void ShowSML(unsigned char *Ptr, int len);                    // golden :1869-1882 (1 gated TMemo->Clear() call, see .cpp)
};

#endif // SECSGEM_SECSWIRECODEC_H
