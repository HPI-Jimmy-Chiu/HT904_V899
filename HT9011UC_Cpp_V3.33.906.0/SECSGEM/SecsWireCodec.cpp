//---------------------------------------------------------------------------
//  SECSGEM/SecsWireCodec.cpp -- SECS-II wire-level codec primitives
//
//  Translation wave: W5-Final (SECSGEM wire codec)
//  Translator: AI(W5-Final-SecsWireCodec) 20260711
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp
//  (per-function golden line citations are given at each function below).
//
//  SCOPE DISCREPANCY (flag per task instructions, item 8): the task's
//  original line range for the DataItemInNew/DataItemIn pair was
//  "~7114-7134". Reading golden confirms DataItemInNew (:7114-7122) directly
//  calls `GetDataItemLenAndType(len, Type)` (golden :7290-7296), a public
//  wrapper around the private `GetDataItemLenAndTypeSub` (golden :2440-2447)
//  -- NEITHER of which was in the task's enumerated function list. Both are
//  trivial (~7 lines each), touch ONLY `SReceiveData` + `iReturnCode` (both
//  already in this unit's scope per the task's own dependency list), and are
//  a genuine, unavoidable prerequisite for DataItemInNew to do anything at
//  all (without them DataItemInNew would have to be gated). Per the "verify
//  everything yourself" instruction, both are included here as faithful
//  translations rather than gating DataItemInNew -- flagged for review.
//
//  vclcompat NOTE: golden repeats `SReceiveData->Strings[i].c_str()`
//  everywhere. vclcompat's TStringList `Strings[i]` returns a `StringsProxy`
//  (an operator-AnsiString() proxy, not an AnsiString), which has no
//  `.c_str()` member -- calling `.c_str()` directly on it does not compile.
//  Translated throughout this file as `SReceiveData->GetString(i).c_str()`,
//  the same substitution already established at mycylin.cpp:826/844 (see
//  that file's own inline comment) -- purely a vclcompat-proxy accommodation,
//  zero behavior change.
//---------------------------------------------------------------------------
#include "SECSGEM/SecsWireCodec.h"
#include "MachineType.h"  // ChangeToFloatNonPcnt (header-only template; ProcessSML division-count loops)
#include <cstdlib>   // atoi/atof
#include <cstdio>    // sprintf (raw char[] formatting, ProcessSML/ProcessSMLBinary hex dumps)
#include <cstring>   // strlen/strncpy/memset

//---------------------------------------------------------------------------
//  HType -- the shared global SECS-II format-byte table (see .h file-head
//  note "SHARED GLOBAL: HType"). Golden defines the (uninitialized-by-
//  declaration) global at uHGemEquipment.cpp:22 and seeds it once via
//  THGem::InitHType() (golden :353-368), called from the THGem constructor.
//
//  AI(W5-Final-SecsWireCodec) 20260711 JUDGMENT CALL (flag for review):
//  THGem does not exist in this unit's scope, so there is no constructor to
//  hang InitHType()'s call on. Seeded instead via a file-static initializer
//  object (g_HTypeInit below), which C++ guarantees runs once, before any
//  use of HType from a static-init context, and -- because HType and
//  g_HTypeInit are both defined in THIS translation unit -- runs strictly
//  after HType's own (zero-fill) initialization, in declaration order.
//  Alternative considered and rejected: reseed HType from every
//  SecsWireCodec constructor call (redundant -- HType is process-global
//  truth shared by every instance, exactly as in golden where every THGem
//  instance would stomp the same global with the same values; reseeding
//  on each construction also could not deterministically order itself
//  against pre-main() consumers the way a static initializer object does).
//---------------------------------------------------------------------------
struct HTypeStruct HType;

namespace {
struct HTypeInitializer
{
    HTypeInitializer()
    {
        // golden uHGemEquipment.cpp:355-367, literal values verbatim.
        HType.LIST_TYPE    = 0x00;
        HType.ASCII_TYPE   = 0x40;
        HType.JIS_TYPE     = 0x44;
        HType.BINARY_TYPE  = 0x20;
        HType.BOOLEAN_TYPE = 0x24;
        HType.INT_1_TYPE   = 0x64;
        HType.INT_2_TYPE   = 0x68;
        HType.INT_4_TYPE   = 0x70;
        HType.INT_8_TYPE   = 0x60;
        HType.UINT_1_TYPE  = 0xa4;
        HType.UINT_2_TYPE  = 0xa8;
        HType.UINT_4_TYPE  = 0xb0;
        HType.UINT_8_TYPE  = 0xa0;
        HType.FT_4_TYPE    = 0x90;
        HType.FT_8_TYPE    = 0x80;
        // HType.VCL_TYPE: declared in golden HTypeStruct (uHGemEquipment.h:58)
        // but NEVER assigned by golden InitHType() (uHGemEquipment.cpp:353-368)
        // and never compared against by any of this file's functions --
        // faithfully left unset here too (value-initialized to 0 by HType's
        // own zero-fill; the gap itself is the faithful behavior).
    }
};
HTypeInitializer g_HTypeInit;
} // anonymous namespace

//---------------------------------------------------------------------------
//  Wave 2 (frame header builder/parser + SML decoder) gated widget stand-ins.
//
//  These are the 4 gated widget reads the hand-off prompt asked to be
//  neutralized (offline, no UI -- return a safe default instead of touching
//  a live VCL control that doesn't exist in this unit). Style matches the
//  established `Gated_ShowMessage`/`Gated_SetECEnableData` precedent in
//  SecsSvEcRegistration.cpp (own file, same project convention): a small
//  free function in an anonymous namespace, named after what it stands in
//  for, with a TODO citing the real widget + golden call site.
//---------------------------------------------------------------------------
namespace {

// golden uHGemEquipment.h:133 `TCheckBox *GemCheckBoxShowHeadInformation;` --
// read at uHGemEquipment.cpp:2180 (THGem::ProcessRemoteHead) to decide
// whether to trace the decoded Remote HSMS head. GATED: no UI offline, so
// "not checked" -- ProcessRemoteHead still fully decodes the head into
// `Remote`; only the StringOut() trace lines are skipped.
bool Gated_CheckBoxChecked_ShowHeadInformation()
{
    // TODO(W7-UI): wire to the real GemCheckBoxShowHeadInformation TCheckBox.
    return false;
}

// golden uHGemEquipment.h:132 `TCheckBox *GemCheckBoxShowBinary;` -- read at
// TWO golden call sites in this unit's scope: uHGemEquipment.cpp:1220
// (THGem::ShowSMLSpaceBinaryString, guards whether to emit a binary trace
// line at all) and :1342 (THGem::ShowSMLBinary, guards one trailing blank
// StringBinaryOut("") call). Same stand-in reused for both -- it is the same
// conceptual widget in golden, just read at two different call sites. GATED:
// no UI offline, so "not checked" (== binary trace suppressed either way;
// StringBinaryOut's own golden body is already a no-op regardless -- see
// that method's own comment below).
bool Gated_CheckBoxChecked_ShowBinary()
{
    // TODO(W7-UI): wire to the real GemCheckBoxShowBinary TCheckBox.
    return false;
}

// golden uHGemEquipment.h:300 `TMemo *DB;` -- `DB->Clear()` at
// uHGemEquipment.cpp:1874 (THGem::ShowSML), clearing the trace TMemo widget
// right before re-populating it with the freshly-decoded message's trace
// lines ("pig 20170802 (Steven): show the new data before clearing the old,
// keep the previous complete content" -- golden's own comment, gloss only,
// see golden file:line for the original Big5 text). GATED: no TMemo widget
// exists offline; ShowSML's own decode/StringOut work proceeds unaffected.
void Gated_MemoClear()
{
    // TODO(W7-UI): wire to the real DB TMemo widget's Clear().
}

} // anonymous namespace

//---------------------------------------------------------------------------
//  Wave 2 ShowSFDescription lookup table -- golden uHGemEquipment.cpp:60-244
//  `AnsiString SFCodeAndMean[]={...}` (a plain file-scope global in golden,
//  given internal linkage here via the anonymous namespace since no other
//  translation unit needs it -- harmless, ShowSFDescription is its only
//  golden consumer too).
//
//  TRANSCRIPTION NOTE (KNOWLEDGE.md "翻譯中文註解亂碼" rule applied to DATA,
//  not just comments): golden mixes ASCII entries with ~20 Big5 Traditional
//  Chinese entries (the S101F*/S102F* block). Because this table is live
//  lookup DATA consumed at runtime (not just a comment), a naive Read-tool
//  transcription would have corrupted it with U+FFFD replacement characters
//  instead of just risking a cosmetic comment mangle. Extracted instead via
//  a one-off Python `open(path, encoding='cp950')` pass over golden and
//  transcribed as correct UTF-8 below -- verified byte-for-byte against a
//  cp950-decoded reference dump before being typed in.
//---------------------------------------------------------------------------
namespace {
const AnsiString SFCodeAndMean[] =
{
    "[S1F1] Are you There",
    "[S1F2] On Line Data",
    "[S1F3] Selected Status Request",
    "[S1F4] Selected Status Request",
    "[S1F5] Formatted Status Request",
    "[S1F6] Formatted Status Data",
    "[S1F9] Material Transfer Status Data",
    "[S1F10] Material Transfer Status Data",
    "[S1F11] Status Variable Namelist Request",
    "[S1F12] Status Variable Namelist Reply",
    "[S1F13] Connect Request",
    "[S1F14] Connect Request Acknowledge",
    "[S1F15] Request OFF-LINE",
    "[S1F16] OFF-LINE Acknowledge",
    "[S1F17] Request ON-LINE",
    "[S1F18] ON-LINE Acknowledge",
    "[S1F23] Collection Event Namelist Request",
    "[S1F24] Collection Event Namelist",

    "[S2F13] Equipment Constant Request",
    "[S2F14] Equipment Constant Data",
    "[S2F15] New Equipment Constant Send",
    "[S2F16] New Equipment Constant Send Acknowledge",
    "[S2F17] Date and Time Request",
    "[S2F18] Date and Time Data",
    "[S2F21] Remote Command Send",
    "[S2F22] Remote Command Acknowledge",
    "[S2F23] Trace Initial Send",
    "[S2F24] Trace Initial Acknowledge",
    "[S2F25] Diagnostic Loopback Request",
    "[S2F26] Diagnostic Loopback Data",
    "[S2F27] Initiate Processing Request",
    "[S2F28] Initiate Processing Acknowledge",
    "[S2F29] Equipment Constant Namelist Request",
    "[S2F30] Equipment Constant Namelist Reply",
    "[S2F31] Date and Time Send",
    "[S2F32] Date and Time Acknowledge",
    "[S2F33] Define Report",
    "[S2F34] Define Report Acknowledge",
    "[S2F35] Link Event Report",
    "[S2F36] Link Event Report Acknowledge",
    "[S2F37] Enable/Disable Event Report",
    "[S2F38] Enable/Disable Event Report Acknowledge",
    "[S2F39] Multi-Block Inquire",
    "[S2F40] Multi-Block Grant",
    "[S2F41] Remote Command with Parameters",
    "[S2F42] Remote Command Acknowledge",
    "[S2F43] Reset Spooling Streams and Functions",
    "[S2F44] Reset Spooling Acknowledge",
    "[S2F47] Variable Limit Attribute Request",
    "[S2F48] Variable Limit Attribute Send",
    "[S2F49] Enhanced Remote Command",
    "[S2F50] Enhanced Remote Command Acknowledge",
    "[S2F65] Reset Spooling Streams and Functions",
    "[S2F66] Reset Spooling Streams and Functions Ack.",
    "[S2F85] Variable Limit Attribute Request",
    "[S2F86] Variable Limit Attribute Send",
    "[S2F87] Define Variable Limit Attributes",
    "[S2F88] Define Variable Limit Attributes Ack",
    "[S3F1] Material Status Request",
    "[S3F2] Material Status Data",
    "[S3F11] Material ID Request",
    "[S3F12] Material ID Request Acknowledge",
    "[S3F13] Material ID Send",
    "[S3F14] Material ID Acknowledge",
    "[S4F1] Ready to Send Material",
    "[S4F2] Ready to Send Acknowledge",
    "[S4F3] Handshake Complete",
    "[S4F5] Not Ready to Send",
    "[S4F17] Request to Receive Material",
    "[S4F18] Request to Receive Material Acknowledge",
    "[S5F1] Alarm Report Send",
    "[S5F2] Alarm Report Acknowledge",
    "[S5F3] Enable/Disable Alarm Send",
    "[S5F4] Enable/Disable Alarm Acknowledge",
    "[S5F5] List Alarm Request",
    "[S5F6] List Alarm Data",
    "[S5F7] List Enable Alarm Request",
    "[S5F8] List Enable Alarm Acknowledge",
    "[S5F73] Alarm Notification Send",
    "[S5F73] Alarm Notification Acknowledge",
    "[S6F1] Trace Data Send",
    "[S6F2] Trace Data Acknowledge",
    "[S6F3] Discrete Variable Data Send",
    "[S6F4] Discrete Variable Data Acknowledge",
    "[S6F5] Multi-block Data Send Inquire",
    "[S6F6] Multi-block Grant",
    "[S6F9] Formatted Variable Send",
    "[S6F10] Formatted Variable Acknowledge",
    "[S6F11] Event Report Send",
    "[S6F12] Event Report Acknowledge",
    "[S6F13] Annotated Event Report Send",
    "[S6F14] Annotated Event Report Acknowledge",
    "[S6F15] Event Report Request",
    "[S6F16] Event Report Data",
    "[S6F17] Annotated Event Report Request",
    "[S6F18] Annotated Event Report Data",
    "[S6F19] Individual Report Request",
    "[S6F20] Individual Report Data",
    "[S6F21] Annotated Individual Report Request",
    "[S6F22] Annotated Individual Report Data",
    "[S6F23] Request Spooled Data",
    "[S6F24] Request Spooled Data Acknowledgement Send",
    "[S6F65] Request Spooled Data",
    "[S6F66] Request Spooled Data Acknowledge",
    "[S7F1] Process Program Load Inquire",
    "[S7F2] Process Program Load Grant",
    "[S7F3] Process Program Send",
    "[S7F4] Process Program Acknowledge",
    "[S7F5] Process Program Request",
    "[S7F6] Process Program Data",
    "[S7F7] Process Program ID Request",
    "[S7F8] Process Program ID Data",
    "[S7F17] Delete Process Program Send",
    "[S7F18] Delete Process Program Acknowledge",
    "[S7F19] Current EPPID Request",
    "[S7F20] Current EPPID Data",
    "[S7F23] Formatted Process Program Send",
    "[S7F24] Formatted Process Program Acknowledge",
    "[S9F1] Unrecognized Device ID",
    "[S9F3] Unrecognized Stream Type",
    "[S9F5] Unrecognized Function Type",
    "[S9F7] Illegal Data",
    "[S9F9] Transaction Timer Timeout",
    "[S9F11] Data Too Long",
    "[S9F13] Conversation Timeout",
    "[S9F67] Incorrect System Bytes Received",
    "[S10F1] Terminal Request",
    "[S10F2] Terminal Request Acknowledge",
    "[S10F3] Terminal Display Single",
    "[S10F4] Terminal Display Single Acknowledge",
    "[S10F5] Terminal Display Multi-block",
    "[S10F6] Terminal Display Multi-block Acknowledge",
    "[S10F9] Broadcast",
    "[S10F10] Broadcast Acknowledge",
    "[S14F3] Device ID Bin Map Send",
    "[S14F4] Device ID Bin Map Acknowledge",

    "[S101F1] \xe5\x88\x97\xe5\x87\xba\xe5\xa4\x9a\xe6\xaa\x94\xe5\xbc\x8f\xe6\xaa\x94\xe5\x90\x8d",
    "[S101F2] \xe5\x9b\x9e\xe5\x82\xb3\xe5\x88\x97\xe5\x87\xba\xe5\xa4\x9a\xe6\xaa\x94\xe5\xbc\x8f\xe6\xaa\x94\xe5\x90\x8d",
    "[S101F3] \xe5\x88\x97\xe5\x87\xba\xe7\x9b\xae\xe9\x8c\x84\xe5\xbc\x8f\xe6\xaa\x94\xe5\x90\x8d",
    "[S101F4] \xe5\x9b\x9e\xe5\x82\xb3\xe5\x88\x97\xe5\x87\xba\xe7\x9b\xae\xe9\x8c\x84\xe5\xbc\x8f\xe6\xaa\x94\xe5\x90\x8d",
    "[S101F5] Program file upload/download",
    "[S101F6] Program file upload/download Acknowledge",
    "[S101F7] Program file upload/download use Directory",
    "[S101F11] \xe5\x82\xb3\xe9\x80\x81\xe4\xb8\x80\xe5\x80\x8b\xe6\xaa\x94\xe6\xa1\x88",
    "[S101F12] \xe5\x82\xb3\xe9\x80\x81\xe4\xb8\x80\xe5\x80\x8b\xe6\xaa\x94\xe6\xa1\x88\xe5\x9b\x9e\xe8\xa6\x86",
    "[S101F8] Program file upload/download use Directory Acknowledge",
    "[S102F1] \xe5\x8f\x96\xe5\xbe\x97\xe7\xb6\xad\xe4\xbf\xae\xe5\x96\xae\xe5\x96\xae\xe8\x99\x9f",
    "[S102F2] \xe5\x82\xb3\xe5\x9b\x9e\xe7\xb6\xad\xe4\xbf\xae\xe5\x96\xae\xe5\x96\xae\xe8\x99\x9f",
    "[S102F3] \xe5\x82\xb3\xe9\x80\x81\xe7\xb6\xad\xe4\xbf\xae\xe8\xab\x8b\xe6\xb1\x82",
    "[S102F4] \xe5\x82\xb3\xe9\x80\x81\xe7\xb6\xad\xe4\xbf\xae\xe8\xab\x8b\xe6\xb1\x82\xe5\x9b\x9e\xe8\xa6\x86",
    "[S102F5] \xe7\xb6\xad\xe4\xbf\xae\xe5\xae\x8c\xe6\x88\x90\xe5\x85\xa5\xe6\xaa\x94",
    "[S102F6] \xe7\xb6\xad\xe4\xbf\xae\xe5\xae\x8c\xe6\x88\x90\xe5\x85\xa5\xe6\xaa\x94\xe5\x9b\x9e\xe8\xa6\x86",
    "[S102F7] \xe9\x96\x8b\xe5\x95\x9f\xe6\x9f\xa5\xe8\xa9\xa2\xe5\x96\xae",
    "[S102F8] \xe5\x82\xb3\xe5\x9b\x9e\xe7\x9b\xae\xe5\x89\x8d\xe6\x9f\xa5\xe8\xa9\xa2\xe5\x96\xae\xe8\x99\x9f\xe5\x85\xa7\xe5\xae\xb9",
    "[S102F9] \xe8\xa6\x81\xe6\xb1\x82Host \xe5\x82\xb3\xe5\x9b\x9e\xe7\x9b\xae\xe5\x89\x8d\xe6\x89\x80\xe6\x9c\x89\xe8\xa2\xab\xe5\xae\x9a\xe7\xbe\xa9\xe7\x9a\x84\xe7\xb6\xad\xe4\xbf\xae\xe6\xb8\x85\xe5\x96\xae",
    "[S102F10] Host \xe5\x82\xb3\xe5\x9b\x9e\xe7\x9b\xae\xe5\x89\x8d\xe6\x89\x80\xe6\x9c\x89\xe8\xa2\xab\xe5\xae\x9a\xe7\xbe\xa9\xe7\x9a\x84\xe7\xb6\xad\xe4\xbf\xae\xe6\xb8\x85\xe5\x96\xae\xe5\x88\xb0 Equ",
    "[S102F21] \xe5\x82\xb3\xe5\x9b\x9e Socket or Load Board or Kit  \xe7\x9b\xae\xe5\x89\x8d\xe4\xbd\xbf\xe7\x94\xa8\xe6\xac\xa1\xe6\x95\xb8",
    "[S102F22] \xe5\x9b\x9e\xe5\x82\xb3 Socket or Load Board or Kit \xe7\x9b\xae\xe5\x89\x8d\xe4\xbd\xbf\xe7\x94\xa8\xe6\xac\xa1\xe6\x95\xb8",
    "[S102F23] \xe8\xa8\xad\xe5\xae\x9a Socket or Load Board or Kit \xe7\x9b\xae\xe5\x89\x8d\xe4\xbd\xbf\xe7\x94\xa8\xe6\xac\xa1\xe6\x95\xb8",
    "[S102F24] \xe5\x82\xb3\xe5\x9b\x9e\xe8\xa8\xad\xe5\xae\x9a Socket or Load Board or Kit \xe7\x9b\xae\xe5\x89\x8d\xe4\xbd\xbf\xe7\x94\xa8\xe6\xac\xa1\xe6\x95\xb8reply",
    "[S102F25] \xe5\xbb\xba\xe7\xab\x8b\xe6\x96\xb0Socket",
    "[S102F26] \xe5\xbb\xba\xe7\xab\x8b\xe6\x96\xb0 Socket or Load Board or Kit reply",
    "[S102F27] \xe5\xb0\x87 Site \xe7\x94\xa8\xe5\x88\xb0\xe7\x9a\x84 Socket or Load Board or Kit \xe5\x8d\xb8\xe4\xb8\x8b ( Host Database \xe4\xb8\x8d\xe6\xb8\x85\xe8\xb3\x87\xe6\x96\x99 )",
    "[S102F28] \xe5\x9b\x9e\xe6\x87\x89 \xe5\xb0\x87 Site \xe7\x94\xa8\xe5\x88\xb0\xe7\x9a\x84 Socket or Load Board or Kit \xe5\x8d\xb8\xe4\xb8\x8b ( Host Database \xe4\xb8\x8d\xe6\xb8\x85\xe8\xb3\x87\xe6\x96\x99 )\xe5\x91\xbd\xe4\xbb\xa4",
    "[S102F41] \xe6\x93\x9a Run Card \xe4\xb8\x8b\xe8\xbc\x89  work parameter",
    "[S102F42] \xe6\x93\x9a Run Card \xe4\xb8\x8b\xe8\xbc\x89  work parameter \xe5\x88\xb0 Equipment",
    "[S103F11] Status Variable Namelist Request with Value",
    "[S103F12] Status Variable Namelist Reply with Value",
    "[S110F1] Process Program Data Check Request",
    "[S110F2] Process Program Data Check Acknowledge",
    "[S110F3] Process Program Download For ID Request",
    "[S110F4] Process Program Data Acknowledge",
    "[S110F5] Customer Name List Acknowledge",
    "[S110F6] Customer Name List Send",
    "[S110F7] Receipe Information Acknowledge",
    "[S110F8] Receipe Information Send",
    "[S120F1] Setup File Information Acknowledge",
    "[S120F2] Setup File Information Send",
    "[S125F1] Enable/Disable EC Data Send",
    "[S125F2] Enable/Disable EC Data Acknowledge",
};
} // anonymous namespace

//---------------------------------------------------------------------------
SecsWireCodec::SecsWireCodec()
    : LocalBuffer(64u * 1024u * 1024u, 0),
      LocalLength(0),
      LocalLength_4(4),   // see ResetLocalBuffer()'s comment: golden's
                           // CreateLocalHead() (out of scope) reserves the
                           // first 4 bytes for the message-length header
                           // before any DataItemOut() call runs; primed here
                           // so a freshly-constructed codec is immediately
                           // usable for an encode burst.
      SReceiveData(new TStringList()),
      WaitShowString(new TStringList()),
      LogDataString(new TStringList()),
      iReturnCode(1),     // golden convention: "初始值=1" (see every
                           // DataItemIn*/GetDataItemLenAndType* golden
                           // comment) -- sticky-error-code carrier starts
                           // clean.
      // ---- Wave 2 members (see .h declaration comments for golden
      // citations): Local/Remote value-initialized to all-zero (golden's
      // THGem ctor never explicitly touches Remote and only ever seeds
      // Local.DeviceID/Local.SystemByte from out-of-scope state -- zero is
      // the faithful "never connected" starting point either way).
      Local(),
      Remote(),
      EquipmentSystemByte(0),  // golden THGem ctor: `EquipmentSystemByte=0;`
      RemoteLength(0),
      Host_HSMS_RunLength(0),
      bReceiveData(false),     // golden THGem ctor: `bReceiveData=false;`
      bDisableBinaryShow(false), // golden THGem ctor: `bDisableBinaryShow=false;`
      DownLoadFilePtr(NULL),
      bOutputBusy(false),
      iSMLSpace(0),            // golden THGem ctor: `iSMLSpace=0;`
      SpaceStr("")             // golden THGem ctor: `SpaceStr="";`
{
}

SecsWireCodec::~SecsWireCodec()
{
    delete SReceiveData;
    delete WaitShowString;
    delete LogDataString;
}

//---------------------------------------------------------------------------
// ADDITIVE (not golden) -- see .h declaration comment.
//---------------------------------------------------------------------------
void SecsWireCodec::ResetLocalBuffer()
{
    LocalLength = 0;
    LocalLength_4 = 4;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:392-396
//   void __fastcall THGem::StringOut(AnsiString S)
//   {
//       WaitShowString->Add(S);
//       LogDataString->Add(S);
//   }
//---------------------------------------------------------------------------
void SecsWireCodec::StringOut(AnsiString S)
{
    WaitShowString->Add(S);
    LogDataString->Add(S);
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:917-932
// Returns how many bytes one item of `Type` occupies (1/2/4/8), or 0 (+ a
// StringOut diagnostic) for an unrecognized format byte.
//---------------------------------------------------------------------------
int SecsWireCodec::GetLengthOfType(unsigned char Type)
{
    if (Type == HType.LIST_TYPE  || Type == HType.ASCII_TYPE  || Type == HType.BINARY_TYPE ||
        Type == HType.BOOLEAN_TYPE || Type == HType.UINT_1_TYPE || Type == HType.INT_1_TYPE)
        return 1;
    else if (Type == HType.UINT_2_TYPE || Type == HType.INT_2_TYPE)
        return 2;
    else if (Type == HType.UINT_4_TYPE || Type == HType.INT_4_TYPE || Type == HType.FT_4_TYPE)
        return 4;
    else if (Type == HType.UINT_8_TYPE || Type == HType.INT_8_TYPE || Type == HType.FT_8_TYPE)
        return 8;
    else
    {
        StringOut("SECSII format byte not define error?");
        return 0;
    }
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:941-974
// Encodes `len` as 1-3 big-endian bytes into Ptr[0..], returns the byte
// count (the SML "Length of Byte" bit-field value, 1/2/3). >3-byte lengths
// are a hard error (golden: StringOut + return 0 -- never happens for any
// real SECS message, since len<=0xFFFFFF).
//---------------------------------------------------------------------------
unsigned char SecsWireCodec::GetLengthByte(unsigned len, unsigned char *Ptr)
{
    unsigned char ct = 0;
    unsigned char Temp[3];
    do
    {
        if (ct >= 3)
        {
            StringOut("data length byte over 3 error");
            return 0;
        }
        Ptr[ct] = (unsigned char)(len & 0xff);
        len >>= 8;
        ct++;
    } while (len > 0);

    if (ct == 2)                    // put MSB first
    {
        Temp[0] = Ptr[1];
        Temp[1] = Ptr[0];
        Ptr[0] = Temp[0];
        Ptr[1] = Temp[1];
    }
    else if (ct == 3)                // put MSB first
    {
        Temp[0] = Ptr[2];
        Temp[1] = Ptr[1];
        Temp[2] = Ptr[0];
        Ptr[0] = Temp[0];
        Ptr[1] = Temp[1];
        Ptr[2] = Temp[2];
    }
    return ct;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:986-1183
// Encodes one SECS-II data item (Format byte | Length-of-length bits,
// followed by the length bytes, followed by `len` values of `Type`) into
// LocalBuffer starting at LocalLength_4, then rewrites the 4-byte message
// length header at LocalBuffer[0..3] from the new total.
//---------------------------------------------------------------------------
void SecsWireCodec::DataItemOut(int len, unsigned char Type, void *P)
{
    int i, k;
    unsigned char SMLLength;
    unsigned char SMLLengthData[3];
    unsigned char DataSize;

    DataSize = (unsigned char)GetLengthOfType(Type);
    SMLLength = GetLengthByte((unsigned)len * DataSize, SMLLengthData);
    LocalBuffer[LocalLength_4] = (unsigned char)(Type | SMLLength);
    LocalLength_4++;
    for (i = 0; i < SMLLength; i++)
        LocalBuffer[LocalLength_4 + (unsigned)i] = SMLLengthData[i];
    LocalLength_4 += SMLLength;

    if (Type == HType.ASCII_TYPE || Type == HType.BINARY_TYPE)
    {
        unsigned char *ptr;
        ptr = (unsigned char *)P;
        for (i = 0; i < len; i++)
            LocalBuffer[LocalLength_4 + (unsigned)i] = ptr[i];
        LocalLength_4 += (unsigned)len;
    }
    else if (Type == HType.BOOLEAN_TYPE)
    {
        bool *ptr;
        ptr = (bool *)P;
        for (i = 0; i < len; i++)
        {
            if (ptr[i] == true)
                LocalBuffer[LocalLength_4] = 1;
            else
                LocalBuffer[LocalLength_4] = 0;
            LocalLength_4++;
        }
    }
    else if (Type == HType.UINT_4_TYPE)
    {
        unsigned int *ptr, j;
        ptr = (unsigned int *)P;
        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                j >>= 8;
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.UINT_2_TYPE)
    {
        unsigned short *ptr, j;
        ptr = (unsigned short *)P;
        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                j = (unsigned short)(j >> 8);
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.UINT_1_TYPE)
    {
        unsigned char *ptr, j;
        ptr = (unsigned char *)P;
        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                // Steven 20150814 (golden): UINT_1/INT_1 are always 1 byte
                // long, so no right-shift is needed here (faithfully omitted).
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.UINT_8_TYPE)
    {
        unsigned __int64 *ptr, j;
        ptr = (unsigned __int64 *)P;
        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                j >>= 8;
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.INT_1_TYPE)
    {
        char *ptr, j;
        ptr = (char *)P;
        for (i = 0; i < len; i++)               // len entries in the array
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                // Steven 20150814 (golden): UINT_1/INT_1 are always 1 byte
                // long, so no right-shift is needed here (faithfully omitted).
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.INT_2_TYPE)
    {
        short *ptr, j;
        ptr = (short *)P;
        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                j = (short)(j >> 8);
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.INT_4_TYPE)
    {
        int *ptr, j;
        ptr = (int *)P;
        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                j >>= 8;
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.INT_8_TYPE)
    {
        __int64 *ptr, j;
        ptr = (__int64 *)P;

        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                j >>= 8;
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.FT_4_TYPE)
    {
        float *floatPtr;
        floatPtr = (float *)P;
        unsigned char *ptr;
        for (i = 0; i < len; i++)
        {
            ptr = (unsigned char *)(&floatPtr[i]);
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = ptr[0];
                ptr++;
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.FT_8_TYPE)
    {
        double *doublePtr;
        unsigned char *ptr;
        doublePtr = (double *)P;
        for (i = 0; i < len; i++)
        {
            ptr = (unsigned char *)(&doublePtr[i]);
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = *ptr;
                ptr++;
            }
            LocalLength_4 += DataSize;
        }
    }

    LocalLength = LocalLength_4 - 4;
    k = (int)LocalLength;
    for (i = 0; i < 4; i++)
    {
        LocalBuffer[(unsigned)(3 - i)] = (unsigned char)(k & 0xff);
        k >>= 8;
    }
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:979-984
//   void THGem::DataItemOut(unsigned char Type, AnsiString S)
//   {
//       char *Ptr;
//       Ptr=S.c_str();
//       DataItemOut(strlen(Ptr), Type, Ptr);
//   }
// AI(W5-Final-SecsWireCodec) 20260711: golden assigns AnsiString::c_str()'s
// `const char*` result into a raw (non-const) `char *Ptr` -- BCB6 accepted
// this const-discarding assignment; standard C++ does not. Declared `Ptr` as
// `const char*` here and const_cast only at the void* call boundary (the
// ASCII_TYPE/BINARY_TYPE branch of the 3-arg overload only ever READS
// through the pointer, never writes) -- zero behavior change, pure
// const-correctness accommodation.
//---------------------------------------------------------------------------
void SecsWireCodec::DataItemOut(unsigned char Type, AnsiString S)
{
    const char *Ptr;
    Ptr = S.c_str();
    DataItemOut((int)strlen(Ptr), Type, const_cast<char *>(Ptr));
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:1235-1244
// Reads back `TypeChar`'s length-of-length bits (bit0,1) worth of big-endian
// bytes starting at Ptr[RunLength] -- the inverse arithmetic of
// GetLengthByte, used when walking a received SML byte stream. Does NOT
// mutate RunLength (a plain by-value parameter, not a reference, faithfully
// preserved from golden's own `int RunLength` signature).
//---------------------------------------------------------------------------
int SecsWireCodec::GetSMLLenthByte(unsigned char TypeChar, unsigned char *Ptr, int RunLength)
{
    int len = TypeChar & 0x03;
    int ct = 0;
    for (int i = 0; i < len; i++)
    {
        ct <<= 8;
        ct += Ptr[RunLength + i];
    }
    return ct;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:2225-2432
// Consumes ("Type","len",value,value,...) decimal-string tokens from the
// FRONT of SReceiveData and writes `len` values of `Type` into *P.
//   return -1 : wrong Type token, or SReceiveData ran dry
//   return -2 : wrong length token (ASCII: declared len > caller's buffer;
//               everything else: declared len != caller's len)
//   return  1 : ok
//---------------------------------------------------------------------------
int SecsWireCodec::DataItemInSub(int len, unsigned char Type, void *P)
{
    unsigned char t;
    int l, i;

    if (SReceiveData->Count == 0)
        return -1;
    t = (unsigned char)atoi(SReceiveData->GetString(0).c_str());   // Type

    if (t != Type)
        return -1;
    SReceiveData->Delete(0);
    if (SReceiveData->Count == 0)
        return -1;
    l = atoi(SReceiveData->GetString(0).c_str());                  // length of data

    if (t == HType.ASCII_TYPE)               // guard against caller's buffer being too small
    {
        if (l > len)
            return -2;
    }
    else
    {
        if (len != l)
            return -2;
    }
    SReceiveData->Delete(0);                 // length checked out from here on

    if (t == HType.LIST_TYPE)
    {
        return 1;
    }
    else if (t == HType.ASCII_TYPE)
    {
        char *temp;
        temp = (char *)P;
        if (SReceiveData->Count == 0)
            return -1;

        // JerryYang 20230112 (golden): len -> len+1, so the copied ASCII
        // payload keeps its NUL terminator (the earlier len-only strncpy
        // dropped the string's closing byte).
        strncpy(temp, SReceiveData->GetString(0).c_str(), (size_t)(len + 1));
        SReceiveData->Delete(0);
    }
    else if (t == HType.BINARY_TYPE)
    {
        unsigned char *temp;
        temp = (unsigned char *)P;

        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (unsigned char)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.BOOLEAN_TYPE)
    {
        bool *temp;
        temp = (bool *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                if (atoi(SReceiveData->GetString(0).c_str()) == 1)
                    temp[i] = true;
                else
                    temp[i] = false;
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.INT_1_TYPE)
    {
        char *temp;
        temp = (char *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (char)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.INT_2_TYPE)
    {
        short *temp;
        temp = (short *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (short)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.INT_4_TYPE)
    {
        int *temp;
        temp = (int *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.INT_8_TYPE)
    {
        __int64 *temp;
        temp = (__int64 *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.UINT_1_TYPE)
    {
        unsigned char *temp;
        temp = (unsigned char *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (unsigned char)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.UINT_2_TYPE)
    {
        unsigned short *temp;
        temp = (unsigned short *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (unsigned short)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.UINT_4_TYPE)
    {
        unsigned *temp;
        temp = (unsigned *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (unsigned)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.UINT_8_TYPE)
    {
        unsigned __int64 *temp;
        temp = (unsigned __int64 *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (unsigned __int64)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.FT_4_TYPE)
    {
        float *temp;
        temp = (float *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (float)atof(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.FT_8_TYPE)
    {
        double *temp;
        temp = (double *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = atof(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else
    {
        return -1;
    }
    return 1;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:2440-2447 (private in golden -- peek-only: does
// NOT Delete from SReceiveData, unlike GetDataItemLenAndTypeAndDeleteSub,
// which is out of this unit's scope).
//---------------------------------------------------------------------------
int SecsWireCodec::GetDataItemLenAndTypeSub(int &len, unsigned char &Type)
{
    if (SReceiveData->Count < 2)
        return -2;
    Type = (unsigned char)atoi(SReceiveData->GetString(0).c_str());  // Type
    len  = atoi(SReceiveData->GetString(1).c_str());                 // length of data
    return 1;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7290-7296
//---------------------------------------------------------------------------
int SecsWireCodec::GetDataItemLenAndType(int &len, unsigned char &Type)
{
    int ret;
    ret = GetDataItemLenAndTypeSub(len, Type);
    if (iReturnCode == 1)
        iReturnCode = ret;
    return ret;
}

//===========================================================================
//  WAVE 3 -- destructive (peek+consume) GetDataItemLenAndType siblings +
//  SendInvalidDataMessageToHost
//
//  Translator: AI(W906-uHGemClass-Unlock2) 20260716
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp
//  (per-function citations below).
//
//  These 3 functions are declared (and defined) on golden's `THGem`, exactly
//  like every other function already in this file -- they were simply never
//  picked up by Wave 1/2's line-range scoping. Confirmed by direct read of
//  golden: all 3 touch ONLY members this class already owns (SReceiveData /
//  iReturnCode / LocalBuffer+LocalLength_4 via InitLocalHead+DataItemOut),
//  zero VCL, zero new THGem-only state -- a faithful, natural extension of
//  the existing GetDataItemLenAndType(Sub)/LocalAcknowledge family, per this
//  project's established "engine slice" convention (see this file's own
//  Wave-1/Wave-2 header notes).
//===========================================================================

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:2453-2462 (private in golden -- destructive
// sibling of GetDataItemLenAndTypeSub above: reads the SAME 2 leading tokens
// (Type, then length-of-data) but also Delete(0)s both off the front of
// SReceiveData, unlike the peek-only Sub above which leaves them in place).
//---------------------------------------------------------------------------
int SecsWireCodec::GetDataItemLenAndTypeAndDeleteSub(int &len, unsigned char &Type)
{
    if (SReceiveData->Count < 2)
        return -2;
    Type = (unsigned char)atoi(SReceiveData->GetString(0).c_str());  // Type
    len  = atoi(SReceiveData->GetString(1).c_str());                 // length of data
    SReceiveData->Delete(0);
    SReceiveData->Delete(0);
    return 1;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7099-7106 -- sticky-iReturnCode public wrapper
// around GetDataItemLenAndTypeAndDeleteSub, mirroring GetDataItemLenAndType's
// own wrapper shape immediately above (identical "初始值=1,..." convention --
// see that method's own comment for the gloss, not repeated here).
//---------------------------------------------------------------------------
int SecsWireCodec::GetDataItemLenAndTypeAndDelete(int &len, unsigned char &Type)
{
    int ret;
    ret = GetDataItemLenAndTypeAndDeleteSub(len, Type);
    if (iReturnCode == 1)
        iReturnCode = ret;
    return ret;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7353-7358
//   void THGem::SendInvalidDataMessageToHost(AnsiString S)
//   {
//       InitLocalHead(9, 7, 0);
//       DataItemOut(HType.ASCII_TYPE, S);
//       SendLocalData();
//   }
// THGem's generic "malformed S,F body" reply sender -- an S9F7 Illegal Data
// message built without the leading StringOut(S) trace call S9F7_IllegalData
// (uHGemClass.cpp, already un-gated) has; golden itself keeps these as two
// separate, textually near-identical functions (not one calling the other),
// preserved as such here.
//---------------------------------------------------------------------------
void SecsWireCodec::SendInvalidDataMessageToHost(AnsiString S)
{
    InitLocalHead(9, 7, 0);
    DataItemOut(HType.ASCII_TYPE, S);
    SendLocalData();
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7114-7122
// Peeks the next token's (len,Type) then decodes+consumes it into an
// AnsiString via the AnsiString-typed DataItemIn overload below.
//---------------------------------------------------------------------------
int SecsWireCodec::DataItemInNew(AnsiString &P)
{
    int len;
    unsigned char Type;
    GetDataItemLenAndType(len, Type);
    int ret = DataItemIn(len, Type, P);
    if (iReturnCode == 1)
        iReturnCode = ret;
    return ret;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7125-7130
//---------------------------------------------------------------------------
int SecsWireCodec::DataItemIn(int len, unsigned char Type, void *P)
{
    int ret;
    ret = DataItemInSub(len, Type, P);
    if (iReturnCode == 1)
        iReturnCode = ret;
    return ret;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7134-7281
// Decodes exactly one scalar item of `t`/`len` into an AnsiString: ASCII as
// text, every numeric format via its native C type then AnsiString's
// number-formatting operator=.
//---------------------------------------------------------------------------
int SecsWireCodec::DataItemIn(int len, unsigned char t, AnsiString &Str)
{
    int ret;

    if (len <= 0)                    // JerryYang 20250725 (golden): guard added
    {
        iReturnCode = 0;
        Str = "";
        return 0;
    }

    if (t == HType.ASCII_TYPE)
    {
        // Steven 20250726 (golden): dynamically-sized buffer (was a fixed
        // char[10240] before JerryYang 20240328's earlier fix); +100 slack
        // preserved verbatim from golden.
        char *P;
        P = new char[(size_t)len + 100];
        ret = DataItemInSub(len, t, P);
        if (ret == 1)
        {
            Str.sprintf("%s", P);
            iReturnCode = ret;
        }
        delete[] P;
        P = NULL;
        return ret;
    }

    if (len != 1)
        return -1;

    if (t == HType.UINT_1_TYPE)
    {
        unsigned char P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = (unsigned)P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.UINT_2_TYPE)
    {
        unsigned short P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = (unsigned)P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.UINT_4_TYPE)
    {
        unsigned P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.UINT_8_TYPE)
    {
        unsigned __int64 P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            // AI(W5-Final-SecsWireCodec) 20260711: vclcompat AnsiString has
            // operator=(long long) but no operator=(unsigned long long)
            // overload (ambiguous otherwise) -- narrowed via (long long),
            // matching the realistic SECS-II UINT_8 payload range.
            Str = (long long)P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.INT_1_TYPE)
    {
        char P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = int(P);
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.INT_2_TYPE)
    {
        short P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = (int)P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.INT_4_TYPE)
    {
        int P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.INT_8_TYPE)                                          // Steven 20130730 (golden)
    {
        __int64 P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = (long long)P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.FT_4_TYPE)                                           // Steven 20130730 (golden)
    {
        float P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = (double)P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.FT_8_TYPE)                                           // Steven 20130730 (golden)
    {
        double P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = P;
            iReturnCode = ret;
        }
        return ret;
    }
    else
    {
        return -1;
    }
}

//===========================================================================
//  WAVE 2 -- frame header builder/parser + byte-stream decoder/pretty-printer
//
//  Translator: AI(W5-Final-SecsWireCodec-Wave2) 20260713
//  15 task functions + 2 direct-callee prerequisites (see each one's own
//  comment): ConvertLocalData/CreateLocalHead/InitLocalHead/
//  HostConvertToData/ProcessRemoteHead/LocalAcknowledge/StoreToReceiveString/
//  ProcessSML/ProcessSMLBinary/ShowSML/ShowSMLBinary/MakeSMLSpaceString/
//  ShowSMLSpaceString/ShowSMLSpaceBinaryString/ShowSFDescription, plus
//  prerequisite StringBinaryOut (genuinely-empty golden method) and
//  SendLocalData (GATED -- see its own comment).
//
//  PREREQUISITE ADDITIONS (flag, same "verify everything yourself" spirit as
//  Wave 1's own SCOPE DISCREPANCY note): LocalAcknowledge's golden body calls
//  SendLocalData() (golden uHGemEquipment.cpp:1985-2107), a real THGem method
//  that dispatches LocalBuffer over a live HSMS TClientSocket/TServerSocket
//  connection -- NOT one of the 15 task functions, and genuinely out of this
//  wave's scope (needs the comms layer a future SECSGEM-engine wave brings,
//  per KNOWLEDGE.md's vclcompat/ClientSocket.h reuse note). Declared+defined
//  here as a GATED stub (see its own comment) purely so LocalAcknowledge's
//  call shape stays faithful rather than needing to gate LocalAcknowledge
//  itself. Likewise ProcessSMLBinary/ShowSMLBinary/ShowSMLSpaceBinaryString
//  all call StringBinaryOut (golden uHGemEquipment.cpp:401-404), whose golden
//  body is `//LogDataString->Add(S);` -- commented OUT in golden itself, so
//  it is translated verbatim as a real (not gated) no-op.
//===========================================================================

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:847-854
//   V 1.0  “outbound data goes into the buffer MSB-first” (gloss; see golden
//   file:line for the original Big5 comment, not reproduced here per
//   KNOWLEDGE.md's cp950-aware-read rule).
// Copies `len` bytes out of *Value into LocalBuffer starting at
// LocalLength_4, MSB-first (byte len-1-i of Value's native in-memory layout
// goes to LocalBuffer[LocalLength_4+i]) -- the same manual big-endian byte
// swap idiom DataItemOut already uses above. Advances LocalLength_4 by len.
//---------------------------------------------------------------------------
void SecsWireCodec::ConvertLocalData(int len, void *Value)
{
    unsigned char *p;
    p = (unsigned char *)Value;
    for (int i = 0; i < len; i++)
        LocalBuffer[LocalLength_4 + (unsigned)i] = p[len - 1 - i];
    LocalLength_4 += (unsigned)len;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:859-889
//   V 1.0  "outbound Head fields, MSB-first, into the buffer" (gloss).
// Zeroes the whole LocalBuffer, then writes the 10-byte HSMS message head
// (DeviceID, MessageID_S|W_Bit-flag, MessageID_F, PType, SType, SystemByte)
// from `Local` starting at offset 4 via ConvertLocalData, then backfills the
// 4-byte message-length header at LocalBuffer[0..3] from the final
// LocalLength_4. Leaves LocalLength_4==14, ready for a DataItemOut() burst.
//---------------------------------------------------------------------------
void SecsWireCodec::CreateLocalHead()
{
    unsigned int i, j;
    unsigned char c;
    // golden: `memset(LocalBuffer, 0, sizeof(LocalBuffer));` -- LocalBuffer
    // is a heap std::vector here (see .h's own AI note on that judgment
    // call), not an embedded fixed array; .data()/.size() reproduce the
    // identical "zero the whole 64MB buffer" effect.
    memset(LocalBuffer.data(), 0, LocalBuffer.size());
    LocalLength = 0;
    LocalLength_4 = 4;
    ConvertLocalData(2, &Local.DeviceID);
    c = Local.MessageID_S;
    if (Local.W_Bit == 1)
        c |= 0x80;
    ConvertLocalData(1, &c);
    ConvertLocalData(1, &Local.MessageID_F);
    ConvertLocalData(1, &Local.PType);
    ConvertLocalData(1, &Local.SType);
    j = Local.SystemByte;
    for (i = 0; i < 4; i++)
    {
        LocalBuffer[LocalLength_4 + 3 - i] = (unsigned char)(j & 0xff);
        j >>= 8;
    }
    LocalLength_4 += 4;
    LocalLength = LocalLength_4 - 4;
    j = LocalLength;
    for (i = 0; i < 4; i++)
    {
        LocalBuffer[3 - i] = (unsigned char)(j & 0xff);
        j >>= 8;
    }
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:894-912
//   V 1.0  "set up the correct Message Head fields to send next" (gloss).
// Seeds `Local` (S/F/W-bit, PType=SType=0, and the outbound SystemByte: an
// echo of Remote's for even/reply function codes, or a freshly-bumped
// EquipmentSystemByte counter for odd/primary ones), then calls
// CreateLocalHead() to actually build the wire header from it.
//---------------------------------------------------------------------------
void SecsWireCodec::InitLocalHead(int SCode, int FCode, int WBit)
{
    Local.MessageID_S = (unsigned char)SCode;
    Local.MessageID_F = (unsigned char)FCode;
    Local.W_Bit = (unsigned)WBit;
    Local.PType = 0;
    Local.SType = 0;
    if ((FCode % 2) == 0)
    {
        Local.SystemByte = Remote.SystemByte;
    }
    else
    {
        EquipmentSystemByte++;
        Local.SystemByte = EquipmentSystemByte;
    }
    LocalLength = 0;
    CreateLocalHead();
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:2150-2159
//   ProcessBuffer: inbound binary byte stream; len: byte count to pull;
//   Value: destination for the decoded (native-endian) value.
// Inverse of ConvertLocalData: reads `len` wire bytes (MSB-first) out of
// ProcessBuffer into *Value's native in-memory layout, advances
// ProcessBuffer by len, and decrements the running Host_HSMS_RunLength
// countdown by the same amount.
//---------------------------------------------------------------------------
unsigned char *SecsWireCodec::HostConvertToData(unsigned char *ProcessBuffer, int len, void *Value)
{
    unsigned char *p;
    p = (unsigned char *)Value;
    for (int i = 0; i < len; i++)
        p[len - 1 - i] = ProcessBuffer[i];
    ProcessBuffer += len;
    Host_HSMS_RunLength -= (unsigned)len;
    return ProcessBuffer;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:2164-2201
//   V 1.0  "decode the Remote (Host) Head fields and show them" (gloss).
// Walks HostConvertToData across the wire's own 4-byte length header plus
// the 10-byte HSMS head into `RemoteLength`/`Remote`, unpacking the W-bit out
// of MessageID_S's top bit (then masking it back off). The trailing trace
// block (1 of the 4 gated widget reads for this wave) is skipped offline --
// the actual decode into `Remote`/`RemoteLength` above happens unconditionally
// either way.
//---------------------------------------------------------------------------
void SecsWireCodec::ProcessRemoteHead(unsigned char *ProcessBuffer)
{
    AnsiString S;
    ProcessBuffer = HostConvertToData(ProcessBuffer, 4, &RemoteLength);
    Host_HSMS_RunLength = RemoteLength;
    ProcessBuffer = HostConvertToData(ProcessBuffer, 2, &Remote.DeviceID);
    ProcessBuffer = HostConvertToData(ProcessBuffer, 1, &Remote.MessageID_S);
    if (Remote.MessageID_S & 0x80)
        Remote.W_Bit = 1;
    else
        Remote.W_Bit = 0;
    Remote.MessageID_S = (unsigned char)(Remote.MessageID_S & 0x7f);
    ProcessBuffer = HostConvertToData(ProcessBuffer, 1, &Remote.MessageID_F);
    ProcessBuffer = HostConvertToData(ProcessBuffer, 1, &Remote.PType);
    ProcessBuffer = HostConvertToData(ProcessBuffer, 1, &Remote.SType);
    ProcessBuffer = HostConvertToData(ProcessBuffer, 4, &Remote.SystemByte);
    // GATED widget read #1/4 (golden uHGemEquipment.cpp:2180
    // `GemCheckBoxShowHeadInformation->Checked`) -- see Gated_
    // CheckBoxChecked_ShowHeadInformation()'s own comment above.
    if (Gated_CheckBoxChecked_ShowHeadInformation())
    {
        StringOut("");
        S.sprintf("HSMS_Head.Length=%d", RemoteLength);
        StringOut(S);
        S.sprintf("HSMS_Head.DeviceID=%d", Remote.DeviceID);
        StringOut(S);

        S.sprintf("HSMS_MessageID=S%d F%d", Remote.MessageID_S & 0x7f, Remote.MessageID_F);
        StringOut(S);
        S.sprintf("WBit=%d", Remote.W_Bit);
        StringOut(S);

        S.sprintf("PType=%d", Remote.PType);
        StringOut(S);
        S.sprintf("SType=%d", Remote.SType);
        StringOut(S);
        S.sprintf("Remote.SystemByte=%d", Remote.SystemByte);
        StringOut(S);
        StringOut("");
    }
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:2207-2214 (task's original estimate was
// "~2207-2224" -- golden's actual function body ends at line 2214; lines
// 2215-2224 are the NEXT function's leading comment block, not part of
// LocalAcknowledge).
//   V 1.0  command=0: Acknowledge, command=1: Denied (gloss).
// Builds a 1-byte BINARY acknowledge/deny reply message and dispatches it.
//---------------------------------------------------------------------------
void SecsWireCodec::LocalAcknowledge(unsigned char SCode, unsigned char FCode, unsigned char Command)
{
    unsigned char C;
    C = Command;
    InitLocalHead(SCode, FCode, 0);
    DataItemOut(1, HType.BINARY_TYPE, &C);
    SendLocalData();
}

//---------------------------------------------------------------------------
// PREREQUISITE, GATED (see this file's Wave-2 header note + the .h
// declaration comment): golden uHGemEquipment.cpp:1985-2107,
// THGem::SendLocalData() -- dispatches LocalBuffer[0..LocalLength_4) over a
// live HSMS TClientSocket/TServerSocket connection, with a try/catch
// reconnect path, ShowSFDescription/ShowLocalBufferBinaryData/
// ShowLocalHeadInfo/ShowSML tracing, and SFCodeResponseList/TimeLeft
// retry-wait bookkeeping guarded by a TCriticalSection -- none of which
// exists in this unit's scope (the comms layer is a future SECSGEM-engine
// wave; KNOWLEDGE.md already flags vclcompat/ClientSocket.h as the shim to
// reuse for it). Only the one piece of in-scope state golden mutates
// unconditionally at entry is preserved.
//---------------------------------------------------------------------------
void SecsWireCodec::SendLocalData()
{
    bReceiveData = false;   // golden uHGemEquipment.cpp:1988
    // TODO(W5 SECSGEM engine wave): real HSMS TClientSocket/TServerSocket
    // dispatch of LocalBuffer[0..LocalLength_4) + trace + retry-wait
    // bookkeeping.
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:1442-1447
//   V 1.0  "store the decoded stream as Type,Len,Data-array tokens into the
//   TStringList, for easy consumption by the ItemIn family" (gloss).
// Appends one decimal-string token to SReceiveData -- but ONLY while
// bReceiveData is true (golden: "if it's an outbound/non-receive burst,
// there's no need to record it since nothing will read it back", gloss).
//---------------------------------------------------------------------------
void SecsWireCodec::StoreToReceiveString(AnsiString S)
{
    if (bReceiveData == false)
        return;
    SReceiveData->Add(S);
}

//---------------------------------------------------------------------------
// PREREQUISITE (see this file's Wave-2 header note): golden
// uHGemEquipment.cpp:401-404 -- `void __fastcall THGem::StringBinaryOut
// (AnsiString S) { //LogDataString->Add(S); }` -- the body is commented OUT
// in golden ITSELF (not this translation gating anything). A genuinely-empty
// golden function, translated verbatim as a real no-op.
//---------------------------------------------------------------------------
void SecsWireCodec::StringBinaryOut(AnsiString /*S*/)
{
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:1194-1207
//   V 1.0  "Make SML Tab space for easy to read" (golden's own English
//   comment, verbatim).
// Rebuilds SpaceStr as iSMLSpace*2 literal space characters (or "" once
// iSMLSpace<=0), for every pretty-printer method below to prefix its trace
// line with.
//---------------------------------------------------------------------------
void SecsWireCodec::MakeSMLSpaceString()
{
    if (iSMLSpace <= 0)
    {
        SpaceStr = "";
        return;
    }
    // golden: `SpaceStr.sprintf("% *s", iSMLSpace*2, AnsiString(" "));` --
    // standard printf width-from-argument ('*') right-justifies the 1-char
    // string " " into a field of width iSMLSpace*2, i.e. produces exactly
    // iSMLSpace*2 space characters. Verbatim (no BCB6-specific behavior
    // needed here -- vsnprintf under vclcompat's sprintf() handles '*' width
    // exactly like any standard C library).
    SpaceStr.sprintf("% *s", iSMLSpace * 2, AnsiString(" "));
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:1212-1216
//   V 1.0  "prefix the string to show with the Tab space, then show it"
//   (gloss).
//---------------------------------------------------------------------------
void SecsWireCodec::ShowSMLSpaceString(AnsiString S)
{
    S = SpaceStr + S;
    StringOut(S);
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:1218-1224
// GATED widget read #2/4 (golden `GemCheckBoxShowBinary->Checked==false`,
// this codebase's 2nd of 2 call sites reading that same conceptual widget --
// see ShowSMLBinary below for the other). Offline stand-in always answers
// "not checked", so this always takes golden's own early-return path.
//---------------------------------------------------------------------------
void SecsWireCodec::ShowSMLSpaceBinaryString(AnsiString S)
{
    if (Gated_CheckBoxChecked_ShowBinary() == false)
        return;
    S = SpaceStr + S;
    StringBinaryOut(S);
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:1250-1329
//   V 1.0  "recursively walk the buffer in SML format and print out hex
//   data" (gloss).
// Recursive hex-dump pretty-printer: for a LIST_TYPE item, recurses once per
// child; for a scalar item, formats its Format|LenOfLen byte plus its
// length-of-length bytes plus every data byte as space-separated hex pairs
// via ShowSMLSpaceBinaryString. Returns 0 on success, -1/-2 on a
// length/bounds problem (mirrors ProcessSML's own return-code convention).
//---------------------------------------------------------------------------
int SecsWireCodec::ProcessSMLBinary(unsigned char *Ptr, int Len, int &RunLength)
{
    int ItemSize, lct, ret;
    AnsiString str;
    unsigned char ct, TypeChar, c;
    AnsiString S;

    MakeSMLSpaceString();
    if (RunLength >= Len)
        return -1;

    TypeChar = Ptr[RunLength];
    c = (unsigned char)(TypeChar & 0xfc);
    if (c != HType.LIST_TYPE)
    {
        lct = 1;
    }
    else
    {
        RunLength++;
        lct = GetSMLLenthByte(TypeChar, Ptr, RunLength);
        ct = (unsigned char)(TypeChar & 0x03);
        if (ct == 1)
            str.sprintf("%02X %02X", TypeChar, Ptr[RunLength]);
        else if (ct == 2)
            str.sprintf("%02X %02X %02X", TypeChar, Ptr[RunLength], Ptr[RunLength + 1]);
        else if (ct == 3)
            str.sprintf("%02X %02X %02X %02X", TypeChar, Ptr[RunLength], Ptr[RunLength + 1], Ptr[RunLength + 2]);
        RunLength += ct;
        ShowSMLSpaceBinaryString(str.c_str());
        iSMLSpace++;
        MakeSMLSpaceString();
    }

    for (int i = 0; i < lct; i++)
    {
        TypeChar = Ptr[RunLength];
        c = (unsigned char)(Ptr[RunLength] & 0xfc);
        RunLength++;
        if (c == HType.LIST_TYPE)
        {
            RunLength--;
            if (RunLength >= Len)
                return -2;
            ret = ProcessSMLBinary(Ptr, Len, RunLength);
            if (ret < 0)
                return ret;
            else
                continue;
        }

        ct = (unsigned char)(TypeChar & 0x03);

        if (ct == 1)
            str.sprintf("%02X %02X", TypeChar, Ptr[RunLength]);
        else if (ct == 2)
            str.sprintf("%02X %02X %02X", TypeChar, Ptr[RunLength], Ptr[RunLength + 1]);
        else if (ct == 3)
            str.sprintf("%02X %02X %02X %02X", TypeChar, Ptr[RunLength], Ptr[RunLength + 1], Ptr[RunLength + 2]);

        S = str;
        ItemSize = GetSMLLenthByte(TypeChar, Ptr, RunLength);
        RunLength += ct;
        for (int j = 0; j < ItemSize; j++)
        {
            if (RunLength >= Len)
            {
                ShowSMLSpaceBinaryString(S);
                return -2;
            }
            str.sprintf(" %02X", Ptr[RunLength]);
            S += AnsiString(str);
            RunLength++;
        }
        ShowSMLSpaceBinaryString(S);
    }
    iSMLSpace--;
    MakeSMLSpaceString();
    return 0;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:1334-1345
// GATED widget read #3/4 (golden `GemCheckBoxShowBinary->Checked==true`,
// gating one trailing blank StringBinaryOut("") call -- itself a no-op
// either way per StringBinaryOut's own comment above). golden's own "Ifor
// 20260402" comment (verbatim, gloss of the Big5 portion): always validate
// SML format for S9F7 detection now (used to only run ProcessSMLBinary when
// the checkbox was checked) -- ShowSMLSpaceBinaryString() internally guards
// the actual binary trace output based on checkbox state, so ProcessSMLBinary
// itself always runs unconditionally here.
//---------------------------------------------------------------------------
int SecsWireCodec::ShowSMLBinary(unsigned char *Ptr, int len)
{
    int RunLength = 14;
    int ret = 0;
    iSMLSpace = 0;
    ret = ProcessSMLBinary(Ptr, len, RunLength);
    if (Gated_CheckBoxChecked_ShowBinary() == true)
        StringBinaryOut("");
    return ret;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:1411-1436
//   V 1.0  "show the message's brief description according to its S,F code;
//   find the Stream/Function description and print it" (gloss). Golden's own
//   trailing comment (Steven 20180815, English, verbatim): "fixed S6F12 being
//   sent twice, return SFCode".
// Linear-scans the SFCodeAndMean[] lookup table (declared above, this file's
// own file-static transcription of golden's file-scope global) for an entry
// whose text contains "S<code>F<code>" starting at (1-based) position 2 --
// i.e. right after the table entry's leading "[" -- and StringOut()s it.
//
// FAITHFUL QUIRK (confirmed against golden, not a translation bug): Pos()
// returning exactly 2 is a SUBSTRING match, not a whole-token match -- e.g.
// searching "S1F1" would also match inside a hypothetical "[S1F10]" entry at
// position 2. This table happens to list ascending function numbers before
// their concatenation-prone prefixes (S1F1 appears before S1F10), so the
// linear scan finds the correct shorter entry first in practice; preserved
// verbatim, not "fixed" into an exact-match comparison.
//
// The `else if(SFCodeAndMean[i]=="")` sentinel branch is DEAD CODE against
// today's literal table data (there is no "" entry in SFCodeAndMean[] --
// confirmed by direct read of golden uHGemEquipment.cpp:60-244) -- preserved
// verbatim as a harmless extra safety net, not removed.
//---------------------------------------------------------------------------
AnsiString SecsWireCodec::ShowSFDescription(unsigned char SCode, unsigned char FCode)
{
    int i = 0;
    int arrayCount = (int)(sizeof(SFCodeAndMean) / sizeof(AnsiString));
    AnsiString SFCode;
    SFCode.sprintf("S%dF%d", SCode, FCode);
    while (1)
    {
        if (SFCodeAndMean[i].Pos(SFCode) == 2)
        {
            StringOut(SFCodeAndMean[i]);
            return SFCode;
        }
        else if (SFCodeAndMean[i] == "")
        {
            StringOut(SFCodeAndMean[i]);
            return SFCode;
        }
        i++;
        if (i >= arrayCount)
        {
            StringOut("");
            return SFCode;
        }
    }
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:1452-1863
//   V 1.0  "decode the data stream in SML format and show it, and store it
//   into the TStringList so the ItemIn family can read it back if this is
//   Host data" (gloss).
//
// The core inbound byte -> token decoder: walks one SECS-II item (a LIST and
// everything nested inside it, or a single scalar item) starting at
// Ptr[RunLength], both (a) push a decimal-string token trail into
// SReceiveData via StoreToReceiveString (gated no-op unless bReceiveData is
// true -- see that method's own comment) for a later DataItemIn* burst to
// consume, and (b) pretty-print a human-readable "<TYPE[count] v1 v2 ...>"
// trace line per item via ShowSMLSpaceString. Recurses into itself for each
// nested LIST child. Returns 0 on success, -1 (RunLength already past Len /
// wrong-looking format byte) or -2 (ran out of bytes partway through an
// item) otherwise -- mirrors ProcessSMLBinary's own return-code convention.
//
// PRESERVED GOLDEN QUIRKS (confirmed against golden, NOT translation bugs):
//   1. Per-type-branch loop-bound INCONSISTENCY: the combined UINT_1/UINT_2/
//      UINT_4 branch uses `ChangeToFloatNonPcnt(ItemSize,TypeSize)` (a
//      division-by-zero-safe float divide, see MachineType.h) for its OWN
//      for-loop bound; every other numeric branch (UINT_8, INT_1/2/4/8,
//      FT_4/8) uses a raw integer `(ItemSize/TypeSize)` for its own loop
//      bound instead. ALL branches use ChangeToFloatNonPcnt consistently
//      only for the "is this the last element, do I need a separator space"
//      check. This asymmetry is golden's own (confirmed by direct read of
//      uHGemEquipment.cpp:1601-1854), not homogenized here.
//   2. FT_8_TYPE READ-BEFORE-CHECK BUG (golden uHGemEquipment.cpp:1839-1844):
//      every other fixed-width branch checks `(RunLength+TypeSize-1-k)>=Len`
//      BEFORE reading `Ptr[RunLength+TypeSize-1-k]`; the FT_8_TYPE branch
//      alone reads first, THEN checks (`p[k]=Ptr[...]; if(...>=Len)
//      return -2;`) -- meaning a message that runs out of bytes exactly
//      inside an FT_8 payload triggers a 1-byte out-of-bounds read of the
//      caller's Ptr buffer before the bounds check fires and aborts decode.
//      Preserved verbatim (not "fixed" to check-before-read like its
//      siblings) per this project's faithfulness rule -- flagged here for
//      visibility since it is a genuine golden defect, not merely a style
//      inconsistency.
//   3. The combined UINT_1/2/4 branch's `TypeSize` is set by three
//      INDEPENDENT `if` statements (not an if/else-if chain) that are only
//      known-exhaustive because of the ENCLOSING `else if(c==UINT_1_TYPE ||
//      c==UINT_2_TYPE || c==UINT_4_TYPE)` guard -- a control-flow invariant
//      GCC's `-Wmaybe-uninitialized` cannot see across. `TypeSize=0;` is
//      added as the first statement of this branch ONLY (see inline comment
//      there) purely to satisfy that static analysis; golden itself has no
//      such line, and the enclosing guard already makes the real assignment
//      unconditional in practice -- zero behavior change.
//---------------------------------------------------------------------------
int SecsWireCodec::ProcessSML(unsigned char *Ptr, int Len, int &RunLength)
{
    int j, k, ItemSize, TypeSize, ret;

    AnsiString str;
    unsigned char TypeChar;
    unsigned int ct, i;
    AnsiString S;
    unsigned char c;
    bool isListType = false;
    MakeSMLSpaceString();
    if (RunLength >= Len)
        return -1;

    TypeChar = Ptr[RunLength];
    c = (unsigned char)(TypeChar & 0xfc);
    if (c != HType.LIST_TYPE)
    {
        ct = 1;
    }
    else
    {
        StoreToReceiveString(HType.LIST_TYPE);
        RunLength++;

        ct = (unsigned int)GetSMLLenthByte(TypeChar, Ptr, RunLength);
        RunLength += TypeChar & 0x03;

        str.sprintf("<L[%d]", ct);
        ShowSMLSpaceString(str);
        StoreToReceiveString(ct);

        iSMLSpace++;
        MakeSMLSpaceString();
        isListType = true;
    }

    for (i = 0; i < ct; i++)
    {
        if (RunLength >= Len)
        {
            if (isListType == true)
            {
                iSMLSpace--;
                MakeSMLSpaceString();
                ShowSMLSpaceString(">");
            }
            return -2;
        }
        TypeChar = Ptr[RunLength];
        c = (unsigned char)(Ptr[RunLength] & 0xfc);
        RunLength++;
        if (c == HType.LIST_TYPE)
        {
            RunLength--;
            ret = ProcessSML(Ptr, Len, RunLength);
            if (ret < 0)
                return ret;
            else
                continue;
        }
        ItemSize = GetSMLLenthByte(TypeChar, Ptr, RunLength);
        RunLength += TypeChar & 0x03;
        if (c == HType.ASCII_TYPE)
        {
            StoreToReceiveString(c);
            StoreToReceiveString(ItemSize);
            S.sprintf("<A[%d] %c", ItemSize, '"');
            char *Temp;
            Temp = new char[(size_t)ItemSize + 100];
            for (j = 0; j < ItemSize; j++)
                Temp[j] = (char)Ptr[RunLength + j];
            RunLength += ItemSize;
            Temp[ItemSize] = 0;
            StoreToReceiveString(Temp);
            Temp[ItemSize] = '"';
            Temp[ItemSize + 1] = 0;
            S += AnsiString(Temp);
            S += AnsiString(">");
            ShowSMLSpaceString(S);
            delete[] Temp;
            Temp = NULL;
        }
        else if (c == HType.BINARY_TYPE)
        {
            StoreToReceiveString(c);
            StoreToReceiveString(ItemSize);
            S.sprintf("<B[%d]", ItemSize);
            char Temp[16];

            if (bDisableBinaryShow)
            {
                // golden: `DownLoadFilePtr=&Ptr[RunLength];` -- Ptr is
                // `unsigned char*`, DownLoadFilePtr is golden's own
                // `char*` (uHGemEquipment.h:458); BCB6 accepted this
                // signedness-changing pointer assignment, standard C++ does
                // not. Explicit cast, same accommodation already established
                // for DataItemOut's `Ptr`/SecsSvEcRegistration's
                // SetECDataPointer overload -- zero behavior change.
                DownLoadFilePtr = (char *)&Ptr[RunLength];
                RunLength += ItemSize;
            }
            else
            {
                if (ItemSize > 10)
                {
                    ShowSMLSpaceString(S);
                    S = "";
                }
                for (j = 0; j < ItemSize; j++)
                {
                    if (RunLength >= Len)
                        return -2;

                    sprintf(Temp, " 0x%02X", (unsigned char)Ptr[RunLength]);
                    StoreToReceiveString((unsigned)(Ptr[RunLength]));
                    RunLength++;
                    S += AnsiString(Temp);
                    if ((j % 10) == 9)
                    {
                        ShowSMLSpaceString(S);
                        S = "";
                    }
                }
            }
            S += ">";
            ShowSMLSpaceString(S);
        }
        else if (c == HType.BOOLEAN_TYPE)
        {
            StoreToReceiveString(c);
            StoreToReceiveString(ItemSize);
            S.sprintf("<Boolean[%d]", ItemSize);
            char Temp[16];

            for (j = 0; j < ItemSize; j++)
            {
                if (RunLength >= Len)
                    return -2;

                if (Ptr[RunLength] == 1 || Ptr[RunLength] == 0xFF)
                {
                    sprintf(Temp, " 0x%02X", 1);
                    StoreToReceiveString(1);
                }
                else
                {
                    sprintf(Temp, " 0x%02X", 0);
                    StoreToReceiveString(0);
                }
                RunLength++;
                S += AnsiString(Temp);
            }
            S += ">";
            ShowSMLSpaceString(S);
        }
        else if (c == HType.UINT_1_TYPE ||
                 c == HType.UINT_2_TYPE ||
                 c == HType.UINT_4_TYPE)
        {
            StoreToReceiveString(c);
            TypeSize = 0;   // AI translator accommodation for
                            // -Wmaybe-uninitialized -- see this function's
                            // own "PRESERVED GOLDEN QUIRKS" item 3 above.
                            // Golden has no such line.
            if (c == HType.UINT_1_TYPE)   TypeSize = 1;
            if (c == HType.UINT_2_TYPE)   TypeSize = 2;
            if (c == HType.UINT_4_TYPE)   TypeSize = 4;

            StoreToReceiveString((ItemSize / TypeSize));

            S.sprintf("<U%d[%d] ", TypeSize, (ItemSize / TypeSize));
            unsigned Temp;
            // AI translator accommodation for -Wconversion (int-to-float
            // comparison): golden compares `j < ChangeToFloatNonPcnt(...)`
            // directly (int against ChangeToFloatNonPcnt's `float` return);
            // explicit (double) casts on both sides of every such comparison
            // in this function silence that warning with zero behavior
            // change (float->double is a value-preserving widening).
            for (j = 0; (double)j < (double)(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))); j++)
            {
                Temp = 0;
                for (k = 0; k < TypeSize; k++)
                {
                    if (RunLength >= Len)
                        return -2;

                    Temp <<= 8;
                    Temp += Ptr[RunLength];
                    RunLength++;
                }
                StoreToReceiveString(Temp);
                S += AnsiString(Temp);
                if ((double)(j + 1) < (double)(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S += AnsiString(" ");
            }
            S += AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if (c == HType.UINT_8_TYPE)
        {
            StoreToReceiveString(c);
            TypeSize = 8;

            S.sprintf("<U%d[%d] ", TypeSize, (ItemSize / TypeSize));
            StoreToReceiveString(ItemSize / TypeSize);
            unsigned __int64 Temp;
            for (j = 0; j < (ItemSize / TypeSize); j++)
            {
                Temp = 0;
                for (k = 0; k < TypeSize; k++)
                {
                    if (RunLength >= Len)
                        return -2;

                    Temp <<= 8;
                    Temp += Ptr[RunLength];
                    RunLength++;
                }
                // AI(W5-Final-SecsWireCodec) 20260711 idiom (already
                // established above in DataItemIn's own UINT_8_TYPE branch):
                // vclcompat AnsiString has no operator matching `unsigned
                // __int64` exactly (ambiguous across int/unsigned
                // int/long long/double) -- narrowed via (long long),
                // matching the realistic SECS-II UINT_8 payload range.
                StoreToReceiveString((long long)Temp);
                S += AnsiString((long long)Temp);
                if ((double)(j + 1) < (double)(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S += AnsiString(" ");
            }
            S += AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if (c == HType.INT_1_TYPE)                                            //ok
        {
            char temp;
            unsigned char *p;
            StoreToReceiveString(c);
            TypeSize = 1;

            StoreToReceiveString((ItemSize / TypeSize));

            S.sprintf("<I%d[%d] ", TypeSize, (ItemSize / TypeSize));
            for (j = 0; j < (ItemSize / TypeSize); j++)
            {
                if (RunLength >= Len)
                    return -2;

                temp = 0;
                p = (unsigned char *)&temp;
                for (k = 0; k < TypeSize; k++)
                {
                    if ((RunLength + TypeSize - 1 - k) >= Len)
                        return -2;

                    p[k] = Ptr[RunLength + TypeSize - 1 - k];
                }
                RunLength += TypeSize;
                S += AnsiString(short (temp));
                StoreToReceiveString(AnsiString(short (temp)));
                if ((double)(j + 1) < (double)(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S += AnsiString(" ");
            }
            S += AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if (c == HType.INT_2_TYPE)                                            // ok
        {
            short temp;
            unsigned char *p;
            StoreToReceiveString(c);
            TypeSize = 2;

            StoreToReceiveString((ItemSize / TypeSize));

            S.sprintf("<I%d[%d] ", TypeSize, (ItemSize / TypeSize));
            for (j = 0; j < (ItemSize / TypeSize); j++)
            {
                temp = 0;
                if (RunLength >= Len)
                    return -2;

                p = (unsigned char *)&temp;
                for (k = 0; k < TypeSize; k++)
                {
                    if ((RunLength + TypeSize - 1 - k) >= Len)
                        return -2;
                    p[k] = Ptr[RunLength + TypeSize - 1 - k];
                }
                RunLength += TypeSize;
                S += AnsiString(temp);
                StoreToReceiveString(temp);
                if ((double)(j + 1) < (double)(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S += AnsiString(" ");
            }
            S += AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if (c == HType.INT_4_TYPE)                                            // ok
        {
            int temp;
            unsigned char *p;
            StoreToReceiveString(c);
            TypeSize = 4;

            StoreToReceiveString((ItemSize / TypeSize));

            S.sprintf("<I%d[%d] ", TypeSize, (ItemSize / TypeSize));
            for (j = 0; j < (ItemSize / TypeSize); j++)
            {
                temp = 0;
                if (RunLength >= Len)
                    return -2;

                p = (unsigned char *)&temp;
                for (k = 0; k < TypeSize; k++)
                {
                    if ((RunLength + TypeSize - 1 - k) >= Len)
                        return -2;
                    p[k] = Ptr[RunLength + TypeSize - 1 - k];
                }
                RunLength += TypeSize;
                S += AnsiString(temp);
                StoreToReceiveString(temp);
                if ((double)(j + 1) < (double)(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S += AnsiString(" ");
            }
            S += AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if (c == HType.INT_8_TYPE)                                            // ?
        {
            __int64 temp;
            unsigned char *p;
            StoreToReceiveString(c);
            TypeSize = 8;

            StoreToReceiveString((ItemSize / TypeSize));

            S.sprintf("<I%d[%d] ", TypeSize, (ItemSize / TypeSize));
            for (j = 0; j < (ItemSize / TypeSize); j++)
            {
                temp = 0;
                if (RunLength >= Len)
                    return -2;

                p = (unsigned char *)&temp;
                for (k = 0; k < TypeSize; k++)
                {
                    if ((RunLength + TypeSize - 1 - k) >= Len)
                        return -2;
                    p[k] = Ptr[RunLength + TypeSize - 1 - k];
                }
                RunLength += TypeSize;
                S += AnsiString(temp);
                StoreToReceiveString(temp);
                if ((double)(j + 1) < (double)(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S += AnsiString(" ");
            }
            S += AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if (c == HType.FT_4_TYPE)                                             // how to avoid 0.7==> 0.699999999
        {
            float temp;
            unsigned char *p;
            StoreToReceiveString(c);
            TypeSize = 4;

            StoreToReceiveString((ItemSize / TypeSize));

            S.sprintf("<F%d[%d] ", TypeSize, (ItemSize / TypeSize));
            for (j = 0; j < (ItemSize / TypeSize); j++)
            {
                temp = 0;
                if (RunLength >= Len)
                    return -2;

                p = (unsigned char *)&temp;
                for (k = 0; k < TypeSize; k++)
                {
                    if ((RunLength + TypeSize - 1 - k) >= Len)
                        return -2;
                    p[k] = Ptr[RunLength + TypeSize - 1 - k];
                }
                RunLength += TypeSize;
                S += AnsiString(temp);
                StoreToReceiveString(temp);
                if ((double)(j + 1) < (double)(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S += AnsiString(" ");
            }
            S += AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if (c == HType.FT_8_TYPE)                                             //ok
        {
            double temp;
            unsigned char *p;
            StoreToReceiveString(c);
            TypeSize = 8;

            StoreToReceiveString((ItemSize / TypeSize));

            S.sprintf("<F%d[%d] ", TypeSize, (ItemSize / TypeSize));
            for (j = 0; j < (ItemSize / TypeSize); j++)
            {
                temp = 0;
                if (RunLength >= Len)
                    return -2;

                p = (unsigned char *)&temp;
                for (k = 0; k < TypeSize; k++)
                {
                    // GOLDEN BUG, preserved verbatim -- see this function's
                    // own "PRESERVED GOLDEN QUIRKS" item 2 above: this is the
                    // ONLY branch that reads Ptr[] BEFORE checking bounds
                    // (every sibling numeric branch checks first). A message
                    // that runs out of bytes exactly inside an FT_8 payload
                    // triggers a 1-byte out-of-bounds read here before the
                    // very next line's check fires and aborts decode.
                    p[k] = Ptr[RunLength + TypeSize - 1 - k];
                    if ((RunLength + TypeSize - 1 - k) >= Len)
                        return -2;
                }
                RunLength += TypeSize;
                S += AnsiString(temp);
                StoreToReceiveString(temp);
                if ((double)(j + 1) < (double)(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S += AnsiString(" ");
            }
            S += AnsiString(">");
            ShowSMLSpaceString(S);
        }
    }

    if (isListType == true)
    {
        iSMLSpace--;
        MakeSMLSpaceString();
        ShowSMLSpaceString(">");
    }
    return 0;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:1869-1882
//   V1.0  "print out the received data in SML format" (gloss).
// Top-level decode entry point: decodes the message body starting right
// after the 14-byte wire head (4-byte length + 10-byte HSMS head), via
// ProcessSML. If the FIRST ProcessSML call didn't consume the entire
// message (RunLength != len), golden calls ProcessSML a SECOND time
// continuing from wherever RunLength ended up -- i.e. a message may carry
// two top-level items back to back; preserved verbatim, not "fixed" into a
// loop. GATED widget read #4/4: `DB->Clear()` (golden uHGemEquipment.cpp:1874,
// clearing the trace TMemo widget right before re-populating it) is a no-op
// here -- ProcessSML's own decode/StringOut work proceeds unaffected.
//---------------------------------------------------------------------------
void SecsWireCodec::ShowSML(unsigned char *Ptr, int len)
{
    bOutputBusy = true;
    int RunLength = 14;
    iSMLSpace = 0;
    Gated_MemoClear();
    ProcessSML(Ptr, len, RunLength);
    if (len != RunLength)
        ProcessSML(Ptr, len, RunLength);

    ShowSMLSpaceString(".");
    StringOut("");
    bOutputBusy = false;
}
