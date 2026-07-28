//---------------------------------------------------------------------------
//  SECSGEM/uHGemClass.h  --  HTGem abstract base (SECS/GEM S,F-handler layer)
//
//  Translation wave: W5 (SECSGEM base layer)
//  Translator: AI(W5-SECSGEM-Translate) 20260710
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemClass.h (115 lines, cp950)
//
//  WHAT THIS FILE IS
//  ------------------
//  `HTGem` is an abstract base class of ~90 virtual S,F (Stream,Function) SECS/
//  GEM message handlers (S1F1_AreYouThereRequest, S2F14_EquipmentConstanData,
//  ...).  The site-specific subclass (uHGemHT9045.h/.cpp, a ~6200-line override
//  layer, explicitly OUT OF SCOPE for this wave) overrides the ones it needs;
//  the rest keep HTGem's inline no-op/default body (already `{}`/`{return N;}`
//  in golden -- carried over verbatim below, zero gating needed for those).
//
//  INTERFACE-CUT: THGem forward-declared only
//  --------------------------------------------------------------------------
//  Golden includes "uHGemEquipment.h" for the `THGem` type (a ~6200-line VCL
//  TForm -- the actual byte-level SECS engine: InitLocalHead/DataItemIn/
//  DataItemOut/SendLocalData/StringOut, plus VCL widgets like strGrdCEID/
//  chkMoreMessageAbortProcess).  uHGemEquipment.h/.cpp are NOT part of this
//  unit's scope (recon confirmed HTGem itself is 0% VCL, but ~57 of its ~90
//  methods deref `HGemPtr` -- a `THGem*` -- for nearly every statement; see
//  uHGemClass.cpp's file-head note for the discrepancy this created).  `THGem`
//  is therefore only FORWARD-declared here (a bare `class THGem;`), matching
//  the project's established form-pointer interface-cut convention (see
//  KNOWLEDGE.md: "form 指標 extern PACKAGE TfXxx*->前置宣告 class TfXxx;(W7)").
//  A pointer to an incomplete type is a valid class member; only method BODIES
//  that dereference it need the full definition (gated in the .cpp -- see
//  there).
//
//  TRANSLATION RULES
//  ------------------
//    * __fastcall removed (S7F18_DeleteProcessProgramAcknowledge) -- neutralized
//      by vcl_compat.h anyway, but this project's convention is to drop it from
//      new translations and note it here, not rely on the macro silently.
//    * AnsiString / TStringList via vclcompat.
//    * Method list, order, names, signatures, and existing inline `{}` bodies
//      preserved VERBATIM from golden (this header carries zero behavior change
//      -- it is a pure interface transcription).
//---------------------------------------------------------------------------

#ifndef uHGemClassH
#define uHGemClassH
#include "vclcompat/vcl_compat.h"
#include "SECSGEM/SecsWireCodec.h"
#include "SECSGEM/SecsSvEcRegistration.h"
// AI(W906-uHGemClass-TraceUnlock) 20260728: std::function, for
// SetSystemDateTimeHook below (already transitively available via
// SecsWireCodec.h's own <functional> include -- added directly here too,
// this header uses the type by name and should not rely on a transitive
// include from a sibling header to keep compiling).
#include <functional>
//---------------------------------------------------------------------------

// AI(W5-SECSGEM-Translate) 20260710: THGem (uHGemEquipment.h) is the SECS
// engine VCL form -- out of scope for this unit (see file-head note above).
// Forward declaration only; HTGem never dereferences it in ACTIVE code (all
// remaining HGemPtr-dereferencing method bodies are gated in uHGemClass.cpp
// pending a dedicated future wave that translates uHGemEquipment.h/.cpp).
//
// AI(W906-uHGemClass-Unlock) 20260713: as of the uHGemClass integrate wave, a SUBSET of
// methods (see uHGemClass.cpp file-head "INTEGRATE WAVE" note) no longer go
// through HGemPtr at all -- they call the two THGem-slice engines below
// instead (WireCodec / SvEcReg), which are real, already-translated stand-ins
// for the pieces of THGem those methods actually needed. HGemPtr itself is
// untouched and still exists for the remaining gated methods.
//
// AI(W906-fire-integrate) 20260716: EVALUATED AND DECLINED (at that time) --
// switching this forward declaration to `#include "SECSGEM/uHGemEquipment.h"`
// now that a real (but deliberately partial) `class THGem` exists there
// (W906-uHGemEquipment front). Checked every one of uHGemClass.cpp's 44
// still-gated methods against that partial THGem's actual member/method
// list: NONE had its full dependency chain satisfied yet -- each needed at
// least one more THGem member/method still absent from that wave's slice
// (e.g. slTempReportID/lTempReportIDContent, EnableDisableAlarmAll/
// EnableDisableAlarm, EnableDisableECDataAll/EnableDisableECData,
// GemMDLN/GemSOFTREV/bOnLine/GemClock, or a VCL widget type not in
// vclcompat).
//
// AI(W906-SysModWire) 20260720: RE-EVALUATED, NOW ADOPTED (this is the "next
// time uHGemEquipment.h's THGem slice grows" the note above anticipated).
// GemMDLN/GemSOFTREV/bOnLine/GemClock (data members) and a new
// CheckSFFormatOnlyHead method (all added to THGem this wave, see
// uHGemEquipment.h/.cpp) together satisfy the full dependency chain of 8
// methods (S1F1/S1F2/S1F13/S1F14/Process_S1F14/S1F16/S1F18/S2F18 -- see
// uHGemClass.cpp's own "INTEGRATE WAVE 3" note for the per-method
// accounting), taking the gated count from 44 to 36. The include is added in
// uHGemClass.cpp ONLY (not here) -- this header stays at a bare forward
// declaration; only method BODIES that dereference THGem's complete type
// need uHGemEquipment.h, matching this project's established "form pointer
// interface-cut" convention referenced above.
class THGem;

extern AnsiString SYS_ECChangeID             ;    //pig 2014.04.23 KYEC SECS
extern AnsiString SYS_ECChangeIDOriginaValue ;    //pig 2014.04.23 KYEC SECS
extern AnsiString SYS_ECChangeIDNewValue     ;    //pig 2014.04.23 KYEC SECS

// AI(W906-uHGemClass-TraceUnlock) 20260728: pure parse-fields struct + free
// function un-gating S2,F32 (Date And Time Acknowledge) -- golden SECSGEM/
// uHGemClass.cpp:1095-1216. Extracted as a standalone, zero-side-effect
// helper (same "extract pure calc-core, gate the side effect" philosophy
// already established by this project's MainCalcCore.h/.cpp) but kept LOCAL
// to this file rather than folded into MainCalcCore -- golden's origin here
// is SECSGEM/uHGemClass.cpp, not main.cpp, and MainCalcCore.h's own file-head
// note scopes it explicitly to main.cpp extracts only; IsCorrectDateFormat
// (this same file, ACTIVE since the very first translation wave) is the
// direct precedent for keeping a pure SECSGEM-only helper right here instead.
// Decodes one of golden's 6 fixed SECS ASCII date/time lengths (12/14/16/19/
// 21/22 bytes) into year/month/day/hour/min/sec/hundredths -- pure string
// parsing, zero OS calls, fully unit-testable (see tests/test_uHGemClass.cpp).
struct SECSDateTimeFields
{
    int year, month, day, hour, min, sec, hundredths;
};

// Returns false ONLY when `len` matches none of golden's 6 supported lengths
// (golden's own `Error=true` in that trailing `else`, uHGemClass.cpp:
// 1177-1180) -- `out` is left untouched in that case. When it returns true,
// `out` holds the decoded (but not yet validated) fields; `bRangeError` is
// golden's own `Error` flag as set INSIDE the `IsCorrectDateFormat()==true`
// branch (hour>23 || min>59 || sec>59 || hundredths>99, uHGemClass.cpp:
// 1186-1193); `bApplyClock` is true ONLY when golden would actually reach
// `settime()`/`setdate()` (uHGemClass.cpp:1196-1197) -- i.e.
// IsCorrectDateFormat() true AND every range check passes.
//
// GOLDEN QUIRK, PRESERVED VERBATIM (flagged, not "fixed"): if the length
// branch succeeds but IsCorrectDateFormat() returns false (e.g. month=13, or
// a calendar-invalid day), golden's own `if(IsCorrectDateFormat(...)) {...}`
// has NO `else Error=true` -- Error is simply left at its prior value
// (false), so the host still receives DTACK=0 ("accepted") even though NO
// clock write ever happens. This is DISTINCT from the range-check failure
// case (hour>23 etc, which DOES set Error=true -> DTACK=1) -- an asymmetry
// in golden itself, not a translation defect. Callers must check
// `bApplyClock` (NOT just the return value / `bRangeError`) to know whether
// a write would really occur; the return value and `bRangeError` alone
// reproduce golden's own (inconsistent) DTACK decision, nothing more.
bool ParseSECSDateTimeString(const AnsiString &S, int len, SECSDateTimeFields &out,
                              bool &bRangeError, bool &bApplyClock);

// AI(W906-uHGemClass-TraceUnlock) 20260728: injectable OS-clock-write seam --
// golden's ONLY non-portable operations in S2F32 are Borland dos.h's
// settime()/setdate() (struct time/struct date, BCB6-only types/API, not in
// vclcompat and not portable to standard/MinGW C++). Modeled as an
// std::function hook, the SAME established "injectable side-effect" idiom
// this project already uses for SecsWireCodec::SendLocalDataHook
// (SecsWireCodec.h:424, std::function<void(SecsWireCodec&)>) -- defaults to
// an EMPTY/unassigned std::function (calling an empty std::function is
// explicitly guarded at the one call site, S2F32's own body -- see .cpp), so
// this is a safe no-op until a caller explicitly wires it. NO production
// wiring site exists yet anywhere in this ported tree (grepped) -- a future
// "real OS clock" wave would assign this to a Win32 SetLocalTime-equivalent
// (the natural portable substitute for settime()+setdate() together, since
// Win32 SetLocalTime takes one combined SYSTEMTIME); NOT implemented this
// wave (deliberately out of scope -- this wave's mandate is the pure-parse
// extraction + a SAFE seam, not standing up a real system-clock-mutating
// call with no caller and no way to verify it safely under ctest). A test
// may assign its own recording lambda to observe what WOULD have been
// written, then must reset this back to an empty std::function afterward
// (same save/restore-global discipline this project's own test files already
// use for CUSTOMER_CODE).
extern std::function<void(int year, int month, int day, int hour, int min, int sec, int hundredths)> SetSystemDateTimeHook;

class HTGem
{
    private:
    protected:
    public:
        AnsiString HandlerPath;
        AnsiString DataPath;
        HTGem();
        HTGem(THGem *HGemTmp);
        HTGem(AnsiString Path);
        THGem *HGemPtr;
        TStringList *SecsAlarmMessage;   //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞

        // AI(W906-uHGemClass-Unlock) 20260713: DESIGN -- uHGemClass integrate wave.
        // golden's HTGem methods reach into `HGemPtr`/the global `HGem`
        // (both `THGem*`, out of scope -- see file-head note) for almost
        // every statement: InitLocalHead/DataItemOut/DataItemIn/SendLocalData/
        // GetDataItemLenAndType (byte-level wire codec) and EC_ID/EC_TYPE/
        // EC_Ptr_Max/EC_Ptr_Min/... (SV/EC registration bookkeeping). Two
        // prior waves already translated exactly those two THGem slices as
        // standalone, real, non-copyable engine classes: `SecsWireCodec` and
        // `SecsSvEcRegistration` (see each header's own file-head note; their
        // own docs already flagged them as future-THGem-member prerequisites
        // -- this is that future arriving early, one class at a time, for
        // just the methods that need nothing else). HTGem embeds one
        // instance of EACH BY VALUE (RAII, no manual new/delete pair to
        // leak/double-free -- same "own it outright" style already used by
        // this project's other interface-cut engines, e.g. TMySimMotor
        // holding its own persistent motion state as instance members rather
        // than free-function statics or a pointer the caller must manage).
        // Methods whose FULL golden dependency chain resolves entirely
        // within these two engines now call `WireCodec.`/`SvEcReg.` instead
        // of `HGemPtr->`/`HGem->` -- see uHGemClass.cpp's file-head
        // "INTEGRATE WAVE" note for exactly which ones and why the other 47
        // still cannot. Both are non-copyable (deleted copy ctor/assign, see
        // their own headers); embedding them by value makes HTGem itself
        // implicitly non-copyable too (the compiler deletes HTGem's
        // implicit copy ctor/assignment) -- a harmless, arguably-corrective
        // side effect, since HTGem's OTHER raw-pointer-owned members
        // (SecsAlarmMessage/FMessageList above) would already double-free
        // under a naive shallow copy today; not attempting to fix that
        // pre-existing latent issue further here, just noting the
        // incidental improvement.
        SecsWireCodec WireCodec;
        SecsSvEcRegistration SvEcReg;

        // AI(W906-SysModWire) 20260720: receive-side merge (design D). Golden has
        // exactly ONE codec (THGem's); this port grew two (HTGem's by-value
        // WireCodec for standalone tests + THGem's live engine). ActiveWire is
        // the dispatch indirection: defaults to &WireCodec (standalone behavior
        // bit-identical -- test_uHGemClass's 86 assertions poke hgem.WireCodec
        // directly and keep passing), and is re-pointed at THGem::WireCodec by
        // (a) HT9045Gem shim ctor when a live THGem is supplied and (b)
        // ProcessReceiceData's dispatch entry (self-heal for golden's
        // NULL-at-static-init construction order). SEND was already unified via
        // SecsWireCodec::SendLocalDataHook (Bucket C D2); with ActiveWire bound
        // to THGem's codec, sends composed by HTGem handlers go out through the
        // hook THGem's own ctor installed -- no second hook install needed.
        SecsWireCodec *ActiveWire;

        void UpdateDataPath(AnsiString Path);
        virtual ~HTGem()                        ;
        virtual void AddSV()                    {};
        virtual void AddEC()                    {};
        virtual void AddAlarmList()             {};
        virtual void AddCEID()                  {};
        virtual void AddReprot()                {};
        virtual void ReloadParameter()          {};
        virtual void LookForFile()              {};

        virtual void S1F1_AreYouThereRequest();
        virtual void S1F2_OnLineData();
        virtual void S1F4_SelectedStatusReply();
        virtual void S1F12_StatusVariableNamelistReply();
        virtual void S1F13_EstablishCommunicationsRequest();
        virtual void S1F14_ConnectRequestAcknowledge();
        virtual void Process_S1F14_ConnectRequestAcknowledge();
        virtual void S1F16_OFFLINEAcknowledge();
        virtual void S1F18_ONLINEAcknowledge();
        virtual void S1F24_CollectionEventNamelist();         //2014/01/01  lee

        virtual void S2F14_EquipmentConstanData();
        virtual int  S2F15_UpdateNewEquipmentConstant();
        virtual int  S2F15_CheckNewEquipmentConstant();     //wei 20170417 (Steven) add S2F15
        virtual void S2F16_NewEquipmentConstantSendAcknowledge();
        virtual void SetECValue(unsigned ECID, void *PtrSour);
        virtual int  CheckECValue(AnsiString ECID, void *PtrSour);
        virtual void S2F18_DateandTimeData();
        virtual int  S2F24_TraceInitializeAcknowledgeSub();
        virtual void S2F24_TraceInitializeAcknowledge();
        virtual void S2F26_DiagnosticLoopbackData();
        virtual void S2F32_DateAndTimeAcknowledge();
        virtual void S2F30_EquipmentConstantNamelistReply();
        virtual int  S2F34_DefineReportAcknowledgeSub();
        virtual void S2F34_ProcessHostSendReportID();
        virtual void S2F34_DefineReportAcknowledge();
        virtual int  S2F36_LinkEventReportAcknowledgeSub();
        virtual void S2F36_LinkEventReportAcknowledge();
        virtual void S2F38_EnableDisableEventReportAcknowledge();
        virtual int  S2F42_Host_Command_Acknowledge();
        virtual void S2F44_ResetSpoolingAcknowledge();

        virtual void S5F4_EnableDisableAlarmAcknowledge();
        virtual void S5F6_ListAlarmData();
        virtual void S5F8_ListEnableAlarmAcknowledge();

        virtual void S6F16_EventReportData();
        virtual void S6F18_AnnotatedEventReportData();
        virtual void S6F20_IndividualReportData();
        virtual void S6F24_RequestSpooledDataAcknowledgementSend();

        virtual int  S7F2_ProcessProgramLoadGrant();
        virtual void S7F4_ProcessProgramAcknowledge()                           {};
        virtual void S7F6_ProcessProgramData()                                  {};
        virtual void S7F6_ProcessProgramData(AnsiString FileName)               {};
        virtual void S7F18_DeleteProcessProgramAcknowledge();                          // AI(W5-SECSGEM-Translate) 20260710: __fastcall dropped (neutralized anyway; project convention)
        virtual void Process_S7F20_CurrentEPPIDData();
        virtual void S7F20_CurrentEPPDData();
        virtual int S7F24_FormattedProcessProgramSendAcknowledge()              {return 1;};
        virtual int ProcessS7F23FromatReceipe()                                 {return 2;};
        virtual int S7F26_FormattedProcessProgramData()                         {return 1;};
        virtual int ProcessS7F25FromatReceipe()                                 {return 2;};

        virtual void S9F1_UnrecognizedDeviceID(AnsiString S);                         //Ifor 20260402: S9F1 Unrecognized Device ID
        virtual void S9F5_UnrecognizedFunctionType(AnsiString S);                     //Ifor 20260402: S9F5 Unrecognized Function Type
        virtual void S9F7_IllegalData(AnsiString S);
        virtual void S9F9_TransactionTimerTimeout(AnsiString S);                      //Ifor 20260402: S9F9 Transaction Timer Timeout
        virtual void S10F4_TerminalDisplaySingleAcknowledge();
        virtual void S10F6_TerminalDisplayMultiBlockAcknowledge();
        virtual void S100F4_ReportAllAlarm();
        virtual void S101F2_CurrentEPPDData();
        virtual void S101F4_CurrentEPPDData();
        virtual void S101F6_StoreHostUploadFile();
        virtual void S101F6();
        virtual void S101F8_StoreHostUploadFile();
        virtual void S101F8();
        virtual void S103F12_StatusVariableNamelistReply();
        virtual void S14F4_Get2DID_BinCode()                                    {};     //Frank 20221122 : 2DID sorting for ATK
        virtual void S110F5_RequestCustomerNameList()                           {};
        //pig 2014.07.04 ASEM SECS GEM start
        virtual void S110F6_ListCustomerName()                                  {};
        virtual void S110F7_RequestReceipeInformation()                         {};
        virtual void S110F8_ListReceipeInformation()                            {};
        virtual void S120F1_RequestReceipeSetupFile()                           {};
        virtual void S120F2_ListReceipeSetupFile()                              {};
        //pig 2014.07.04 ASEM SECS GEM end
        virtual void S125F2_EnableDisableECDataAcknowledge();
        virtual void S125F4_LevelSettingChangeAcknowledge()                     {};    //Steven 20150605 : S125F3 LevelSettingChangeRequest
        virtual void S9F3_Unrecognized_Stream_Function_Type(AnsiString S);             //KenHsieh 20221006 : ASE_K1要求下錯誤S,F Code時回傳S9F3
        TStringList *FMessageList;                                                     //Ifor 20251018 add:Secs Alarm List
};

#endif
