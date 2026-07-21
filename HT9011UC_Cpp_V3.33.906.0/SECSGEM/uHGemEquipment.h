//---------------------------------------------------------------------------
//  SECSGEM/uHGemEquipment.h -- THGem, first real slice
//
//  Translation wave: W906-uHGemEquipment (Front B of the "triple-front"
//  THGem wave; see this wave's hand-off prompt / synthesis note for the
//  full multi-front split)
//  Translator: AI(W906-uHGemEquipment) 20260716
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.h
//                 (730 lines) + uHGemEquipment.cpp (9353 lines)
//
//  WHAT THIS FILE IS (READ BEFORE EXTENDING)
//  ------------------------------------------
//  `THGem` (golden uHGemEquipment.h:90-724) is a ~6200-line VCL TForm and the
//  real SECS/GEM protocol engine: TClientSocket/TServerSocket connection
//  lifecycle, the SML wire codec (InitLocalHead/DataItemOut/SendLocalData/...
//  -- ALREADY translated as the standalone SecsWireCodec class,
//  SECSGEM/SecsWireCodec.h), SV/EC registration (ALREADY translated as
//  SecsSvEcRegistration, SECSGEM/SecsSvEcRegistration.h), and ~200 more
//  methods spanning spool files, terminal windows, upload/download, and the
//  ~90 S,F protocol handlers (those live on HTGem, SECSGEM/uHGemClass.h/.cpp,
//  a SEPARATE class that holds a `THGem *HGemPtr` -- see that file's own
//  notes).
//
//  This file creates `class THGem` FOR THE FIRST TIME. It is DELIBERATELY A
//  PARTIAL SLICE, not the full class: only the members/methods this wave's
//  scope actually needs are declared. THIS IS NOT THE FULL THGem --
//  a future wave (or several) will grow this SAME class definition
//  additively (add members/methods to the class body below), exactly the
//  way SECSGEM/uHGemClass.cpp has been unlocked incrementally, method group
//  by method group, across several past waves. Do NOT create a second
//  `class THGem` anywhere else -- this is the one and only definition.
//
//  THIS WAVE'S SLICE = the CEID/Report/Alarm "StringGrid database" family:
//    * Event/Report registration bookkeeping on strGrdCEID/stdGridReportID
//      (SetCEIDContent x2, GetCEIDContent, SetReportIDContent, DeleteReportID,
//      DeleteReportIDOfCeid, DeleteAllHostDefineReportID, GetReportIDContent,
//      IsEnableEvent, CheckCeidExist, GetReportIDType, CheckCeidAlreadyDefine,
//      CheckReportIDExist, AddCeidReportID, DeleteAllHostReportIDOfCeid,
//      DeleteHostReportIDOfAssignCeid, DeleteAllHostDefineCeid,
//      CheckReportAlreadyDefine, CheckCEIDExist(upper-case sibling),
//      EnableDisableEventReport, ReadEventReportData, SaveEventReportData).
//    * Generic StringGrid<->TStringList tab-format (de)serialization
//      (CopyStringGridAsTabFormat, PasteStringGridAsTabFormat, and the file-
//      scope helper SplitStrByTabOnly).
//    * The 3 Alarm-grid siblings (SetAlamData, ReadAlamData, WriteAlamData).
//  Plus, as a small standalone utility used throughout the real THGem (but
//  needed by none of the above -- included because it was named explicitly
//  in this wave's scope): `GemTimer` (golden uHGemEquipment.h:31-42).
//
//  NEW SUPPORTING SHIM: vclcompat/StringGrid.h (headless TStringGrid) was
//  created THIS wave alongside this file -- see that header for the full
//  design/scope note. strGrdCEID/stdGridReportID/strGrdAlarm/strGrdAlarmOld
//  below are allocated at the exact RowCount/ColCount their golden .dfm
//  entries specify (uHGemEquipment.dfm:375-436) -- see the constructor.
//
//  AI(W906-uHGemEquipment-ConnLifecycle) 20260717: SECOND WAVE landed on this
//  same class -- the TCP/IP connection-lifecycle slice: clientGem/srvGem
//  (TClientSocket*/TServerSocket*, allocated+wired in the ctor), their
//  connection-state bools/ints/GemTimers, Connect/DisConnect/IsConnect/OnLine
//  family/CloseCommuncation/CheckSocketActiveFalse, DoOpenCommuncation/
//  OnlineLocalOrRemote/DoOnLine, clientGemConnect/Disconnect/Error/Connecting,
//  srvGemClientConnect/GetSocketErrorMsg/Error/Disconnect, StringOut(1-arg)/
//  StringBinaryOut, SaveSECSGEMErrToLog, ClearDefaultEvenReport, and
//  GetTimeInfo's TimeString/GemClock half. See each method's own .cpp comment
//  for golden line citations.
//
//  AI(W906-uHGemEquipment-BucketB) 20260717: THIRD WAVE ("Bucket B" of the
//  original triple-front recon) landed on this same class -- the ~15 small
//  widget stand-ins (TRadioGroup/TEdit/TCheckBox/TComboBox/TPanel/
//  TSpeedButton/TMemo shapes, see the THGemXxx structs below) plus their
//  first real consumers: InitialHGem/SaveSystemDefault (ini-persisted widget
//  state), DoUpdateStatus (status-panel/button refresh + EventReport on
//  control-state transitions, INCLUDING the KYEC 30-second forced-disconnect
//  dance), ProcessShow, FormClose, ManualCreatergRoleClick, FormShow (partial
//  -- see its own .cpp comment), and the StringOut(AnsiString,TColor)
//  2-arg overload. A GATED NO-OP STUB for DoSeparate() is added too (golden's
//  real body is wire-codec/SML, out of scope -- DoUpdateStatus's KYEC branch
//  unconditionally calls it, same "stub so the caller's control flow stays
//  translatable" precedent as EnableDisableEventReportAcknowledgeError below).
//  DEPENDENCY WIDENING (flagged explicitly -- a real deviation from Bucket
//  A's own "does NOT include cmydef.h/common.h" claim above, which this wave
//  narrows rather than removes): DoUpdateStatus/InitialHGem/SaveSystemDefault
//  need CUSTOMER_CODE/CC_KYEC_LEE/CC_SIGURD_ChungXing/CC_MAXIM_THAILAND/
//  CosFunction/bSECSGEMbyPass/bSECSGEMConnectionFail (cmydef.h, which itself
//  pulls MachineType.h/cprod.h/cpublic.h) and ReadWriteIni/ReadIniData/
//  WriteIniData (common.h, confirmed lightweight -- only vclcompat +
//  MachineType.h, NOT the rest of golden's common.h). Both #includes are
//  confined to the .cpp (this header stays free of them); see root
//  CMakeLists.txt's ht9045_secsgem target for the matching new
//  ht9045_core/ht9045_globals link deps (same shape already established by
//  the KYECFTP/FTPClient_Transfer.cpp precedent for an identical need).
//  Still explicitly OUT OF SCOPE (see below): clientGemRead/
//  ProcessSocketReceiveData, DoConnect/DoSelect, Timer1Timer,
//  DoProcessSFNoResponse, DoLocalAllProcessLoop, FormCreate, and the
//  Left/Top/Width/Height/PageControl1 cosmetic half of FormShow (THGem is not
//  modeled as a real window in this port).
//
//  MEMBERS DELIBERATELY NOT PRESENT YET (out of scope; a future wave adds
//  them to this SAME class body, does not redefine it):
//    - The wire-codec-dependent connection functions: DoConnect/DoSelect
//      (need SecsWireCodec embedded as a member first), clientGemRead/
//      ProcessSocketReceiveData (need TMemoryStream/TFixedCriticalSection
//      shims, not yet designed), DoProcessSFNoResponse (needs a
//      TCriticalSection shim), Timer1Timer (its body calls into all of the
//      above) -- see each's golden citation at uHGemEquipment.cpp's
//      clientGemRead/DoConnect/Timer1Timer. (DoSeparate itself IS now
//      declared/defined -- as a GATED STUB, see above -- so it no longer
//      belongs on this specific "not present" list, but its REAL wire-codec
//      body is still deferred exactly like its DoConnect/DoSelect siblings.)
//    - DataItemInSub/DataItemIn/DataItemInNew/GetDataItemLenAndType(Sub)/
//      StringOut and SetSVDataPointer/SetECDataPointer/GetECDataValue -- per
//      the project's established design (see SecsWireCodec.h/
//      SecsSvEcRegistration.h's own "INTEGRATE-AGENT WIRING POINT" notes), a
//      future full-THGem wave should forward these to WireCodec/SvEcReg
//      (thin wrappers, matching HTGem's already-proven pattern in
//      SECSGEM/uHGemClass.h/.cpp) rather than re-implementing the logic
//      here. NOT done this wave. AI(W906-AlarmReportAck) 20260721:
//      InitLocalHead and ONE DataItemOut overload (the `(int len, unsigned
//      char Type, void *P)` pointer form) are the exception -- ADDED this
//      wave as one-line forwarders to THGem's own embedded WireCodec (see
//      the class body below), because this wave's new Report/Link
//      Acknowledge composer family (ReportAcknowledge/
//      ReportLinkAcknowledgeError/EnableDisableEventReportAcknowledgeError/
//      ProcessHostSendReportLinkID, all below) calls them bare (implicit
//      `this->`, per golden). The 2-arg `DataItemOut(unsigned char, AnsiString)`
//      overload golden's THGem.h also declares is still NOT added -- no
//      in-scope THGem-level caller uses it (verified by reading every new
//      method's body before writing this note); DataItemInSub/DataItemIn/.../
//      SetSVDataPointer/etc. remain fully out of scope as before.
//    - EnableDisableEventReportAcknowledgeError: AI(W906-AlarmReportAck)
//      20260721: NO LONGER a stub -- this wave gave it its real body (see the
//      .cpp), now that InitLocalHead/DataItemOut(pointer overload) exist on
//      THGem (immediately above). The paragraph below is preserved as
//      historical record of why it WAS stubbed:
//      it was THGem::EnableDisableEventReport's one dependency outside that
//      wave's StringGrid-only slice (its golden body is
//      InitLocalHead+DataItemOut+SendLocalData, i.e. the wire-codec family
//      above). Kept as a stub purely so EnableDisableEventReport's own
//      control flow (which unconditionally calls it) is translatable without
//      silently dropping the call.
//
//  AI(W906-uHGemEquipment-BucketC) 20260717: FOURTH wave ("Bucket C", the
//  final wave of the original triple-front recon) landed on this same class
//  -- the socket receive pump / T3 timeout machinery / HSMS control-message
//  handshake / Timer1Timer master state machine. THGem now embeds
//  `SecsWireCodec WireCodec;` BY VALUE (D1) -- codec state (LocalBuffer/
//  Local/Remote/bReceiveData/...) lives there, NOT duplicated on THGem; see
//  SendLocalDataFrom's own .cpp comment for the codec-state routing table.
//  Real methods added: clientGemRead, ProcessSocketReceiveData, Timer1Timer
//  (the ~350-line master SM), DoConnect, DoSelect, DoSeparate (REPLACES the
//  Bucket-B gated stub), DoProcessSFNoResponse, SendLocalData/
//  SendLocalDataFrom, DoLocalAllProcessLoop (shell + 4 gated callees),
//  CheckSFCodeResponse, SaveSECSGEMTextToLog, ShowLocalBufferBinaryData/
//  ShowLocalHeadInfo (each now 2 overloads), InitSTypeStruct, SelectRsp/
//  DeselectRsp/LinktestRsp, and ProcessReceiceData -- but ProcessReceiceData
//  is a SPLIT: the HSMS control-message head (Select/Deselect/Linktest/
//  Separate) is real; the ~180-line S,F DATA-MESSAGE dispatch tail stays ONE
//  gated `#if 0` block (needs live HSys.MyGem + ~15 flag members +
//  MoveCheckCallBack -- see that method's own .cpp comment for the full
//  rationale and the two-codec-instance boundary note it flags).
//  Still explicitly OUT OF SCOPE: FormCreate's SV/EC registration (a future
//  dedicated wave, brings SecsSvEcRegistration along), DoSpool/
//  DoTraceDataResponse/DoUploadFileToHost/DoDownLoadRemoteFile (gated no-op
//  stubs, same idiom as EnableDisableEventReportAcknowledgeError), and the
//  ProcessReceiceData data-message tail above.
//
//  AI(W906-SysModWire) 20260720: FIFTH wave -- SystemModularInitial wiring.
//  The ProcessReceiceData tail's blocker list from the Bucket C note above
//  (live HSys.MyGem + ~15 flag members + MoveCheckCallBack) is now satisfied:
//  MoveCheckCallBack + the 6 bReceive* flags (bReceiveS7F6/S101F5/S101F6/
//  S101F7/S101F8/S110F2) are added members (see the class body below);
//  HSys.MyGem is wired by database.cpp's SystemModularInitial (new
//  SECSGEM/uHGemHT9045_Shim.h "thin shim" HT9045Gem class). The tail is now a
//  REAL block (single `if(HSys.MyGem != NULL)` guard around the golden body,
//  see ProcessReceiceData's own .cpp comment) -- no longer gated. Also added
//  this wave: GemMDLN/GemSOFTREV members + SetMachineTypeAndSoftwarseVer/
//  CheckSFFormatOnlyHead methods (both prerequisites for 8 of uHGemClass.cpp's
//  gated S,F handlers -- see that file's own un-gating note).
//
//  AI(W906-SvEcDataItem) 20260720: SIXTH wave -- SECSGEM closing-waves Wave 1
//  (design doc DESIGN_SECSGEM_closing_waves.md). THGem now embeds
//  `SecsSvEcRegistration SvEcReg;` BY VALUE (mirrors HTGem's own
//  already-proven precedent, uHGemClass.h:137) -- SV/EC registration
//  bookkeeping (SV_ID/SV_TYPE/.../EC_OldValue, SetSVDataPointer x4/
//  SetECDataPointer x4/GetECDataValue) now lives there, reused (not
//  reimplemented) via `#include "SECSGEM/SecsSvEcRegistration.h"`. Real
//  methods added: DataItemOutSV/DataItemOutSVNameList/
//  DataItemOutSVNameListWithValue/DataItemOutEC/DataItemOutECNameList (golden
//  VCL-widget-cast sub-branch GATED, `#if 0` -- see .cpp; the non-VCL raw-
//  ptr/AnsiString* path, which is ALL FormCreate ever registers, is real),
//  IsValidSVID, SendRepoerID/SendAnnotatedRepoerID/SendCeid/
//  SendAnnotatedCeid (Report/CEID data composers -- pure WireCodec+SV
//  composition, no VCL), and FormCreate itself (the "system SV" registration
//  block: GemClock/GemControlState/GemLinkState/SECSCommunicationMode/
//  GemControlPreState/CPU-freq-manufacturer-type (via new SECSGEM/
//  TasmInfo.{h,cpp})/disk-space/memory-status/GemMDLN/GemSOFTREV/
//  GemSpoolCountActual/GemSpoolStartTime/Time-Format EC/Receipe-Struct/
//  Receipe-Extend -- golden uHGemEquipment.cpp:6165-6207). The large bulk of
//  golden's SV/EC registration (uHGemHT9045_SV.cpp/_EC.cpp, ~877+~1740 calls)
//  stays OUT OF SCOPE (see SecsSvEcRegistration.h's own "INTEGRATE-AGENT
//  WIRING POINT" note -- unaffected by this wave).
//
//  AI(W906-AlarmReportAck) 20260721: SEVENTH wave -- SECSGEM closing-waves
//  Wave 2 (Alarm/Report-Ack family). Closes the exact InitLocalHead/
//  DataItemOut gap the SIXTH wave's own note (above) and the
//  EnableDisableEventReportAcknowledgeError stub both named as their
//  blocker. Added:
//    * 2 THGem-level wire-codec forwarders -- InitLocalHead(int,int,int) and
//      DataItemOut(int len, unsigned char Type, void *P), both one-line
//      forwards to THGem's own embedded WireCodec (same idiom as the
//      already-real SendLocalData() below). Only the ONE DataItemOut
//      overload this wave's new callers actually use is added (see that
//      declaration's own comment).
//    * 4 new members (golden uHGemEquipment.h:530-533): slTempReportID/
//      lTempReportIDContent/slTempCeID/lTempCeIDContent -- temp CEID/
//      ReportID staging lists populated by HTGem::
//      S2F34_DefineReportAcknowledgeSub/S2F36_LinkEventReportAcknowledgeSub
//      (uHGemClass.cpp, this same wave) while parsing an incoming S2F33/
//      S2F35 message, drained by ProcessHostSendReportID (STILL not present
//      -- see its own note below)/ProcessHostSendReportLinkID (below).
//    * 3 new Alarm methods (golden uHGemEquipment.cpp:3337-3372):
//      EnableDisableAlarm/EnableDisableAlarmAll/GetAlarmIndex -- pure
//      strGrdAlarm/WriteAlamData composition, zero new dependency.
//    * The Report/Link Acknowledge composer family (golden
//      uHGemEquipment.cpp:7838-8049): ProcessHostSendReportLinkID/
//      ReportAcknowledge(+5 named wrappers)/ReportLinkAcknowledgeError(+4
//      named wrappers) -- pure InitLocalHead/DataItemOut/SendLocalData
//      composition (now real, see above) plus already-real
//      CheckCeidExist/CheckCeidAlreadyDefine/CheckReportIDExist/
//      AddCeidReportID/DeleteHostReportIDOfAssignCeid/SaveEventReportData.
//    * EnableDisableEventReportAcknowledgeError given its REAL body
//      (replacing the no-op stub) + its 2 named wrappers
//      (EnableDisableEventReportAcknowledgeCeidNotExist/FormatError, golden
//      :8041-8049).
//  NOT added: golden's OWN `THGem::ProcessHostSendReportID()` (declared
//  golden uHGemEquipment.h:553) -- confirmed by grep to have ZERO definition
//  and ZERO call sites anywhere in golden's SECSGEM directory (a dead/
//  vestigial declaration golden itself never defines or calls; NOT the same
//  method as `HTGem::S2F34_ProcessHostSendReportID`, uHGemClass.cpp, which
//  IS translated this wave and needs no THGem-level sibling). Not declared
//  here either, matching golden's own effectively-abstract state for it.
//  See uHGemClass.cpp's own "INTEGRATE WAVE 5" note for the matching
//  8-method un-gating on the HTGem side.
//
//  AI(W906-uHGemClass-Micro5) 20260721: EIGHTH wave -- uHGemClass.cpp
//  "remaining 15 gated methods" cluster, micro-slice #5 (the last 4 of that
//  cluster resolvable without a whole new subsystem: recipe-upload-listing +
//  remote-recipe-checklist + EC-enable "StringGrid database"). Added:
//    * TStringList *UploadFileString -- see that member's own comment above
//      (mirrors HTGem's SecsAlarmMessage/FMessageList new/delete lifecycle,
//      NOT the default-NULL idiom below).
//    * THGemListBox *GemRemoteReceipeList -- see that member's own comment
//      above (mirrors TerminalMemoPtr's default-NULL/externally-assigned
//      idiom, NOT the new/delete idiom above -- the two members added this
//      wave deliberately use OPPOSITE lifecycles, each matching its own
//      golden precedent).
//    * TStringGrid *sgSECSECData + WriteECEnableData/EnableDisableECData/
//      EnableDisableECDataAll (golden uHGemEquipment.cpp:9257-9302) -- a
//      mechanical structural clone of the SEVENTH wave's own strGrdAlarm/
//      WriteAlamData/EnableDisableAlarm/EnableDisableAlarmAll family, just
//      for EC instead of Alarm. GOLDEN COMMENT preserved verbatim in the
//      .cpp (see EnableDisableECData/EnableDisableECDataAll's own citation):
//      "Steven 20150603 : T&0x10 --> T&0x80".
//  Consumed by HTGem::S101F2_CurrentEPPDData/S101F4_CurrentEPPDData/
//  Process_S7F20_CurrentEPPIDData/S125F2_EnableDisableECDataAcknowledge
//  (uHGemClass.cpp, this same wave) -- see that file's own "INTEGRATE WAVE 6"
//  note for the matching 4-method un-gating on the HTGem side.
//---------------------------------------------------------------------------
#ifndef uHGemEquipmentH
#define uHGemEquipmentH

#include "vclcompat/vcl_compat.h"
#include "vclcompat/StringGrid.h"
#include "SECSGEM/SecsEventType.h"   // SECS_EVENT.TotalEvent (array bound in SetCEIDContent)

// AI(W906-uHGemEquipment-BucketC) 20260717: D1 -- embed `SecsWireCodec
// WireCodec;` by value (see the class body below), following the proven
// HTGem precedent (uHGemClass.h:127). This header supplies HTypeStruct/
// STypeStruct/HSMS_Head_Struct + `extern HType` per SecsWireCodec.h:99-107's
// own "INTEGRATE-AGENT WIRING POINT" note -- they are NOT redeclared here.
#include "SECSGEM/SecsWireCodec.h"
// AI(W906-SvEcDataItem) 20260720: SvEcReg embed (see file-head note) --
// reuses SecsSvEcRegistration's own SetSVDataPointer/SetECDataPointer/
// GetECDataValue + SV_*/EC_* bookkeeping lists rather than reimplementing
// them on THGem. #include, not redeclare (ODR-safe, same reuse pattern
// SecsSvEcRegistration.h itself already established for HType).
#include "SECSGEM/SecsSvEcRegistration.h"
// THGem::FormCreate's SV10-19 (CPU/disk/memory) source functions.
#include "SECSGEM/TasmInfo.h"
// TCriticalSection (TFixedCriticalSection's base below; csSFCodeResponse).
#include "vclcompat/SyncObjs.h"
// TMemoryStream (RecvMemoryBuffer/ProcBuffer/TempProcBuffer below); already
// brings TMemoryStream/soFromBeginning/soFromCurrent/soFromEnd into the
// global namespace itself (see that header's own tail note).
#include "vclcompat/MemoryStream.h"

// This TU does not include aHotPlateSubstrate.h (the other place a global
// `class TList` lives -- see vclcompat/vcl_compat.h's own TList.h precedent
// note) and none of this wave's methods need vclcompat::TList, so bringing
// TStringGrid into the global namespace here is collision-free (grepped: no
// other `class TStringGrid` exists anywhere in this tree).
using vclcompat::TStringGrid;
// TCriticalSection is NOT auto-brought into the global namespace by
// vclcompat/SyncObjs.h (matches TStringGrid.h's own posture) -- brought in
// explicitly here, same idiom as the TStringGrid line above (golden spells
// both types unqualified: `TCriticalSection *csSFCodeResponse;`,
// `class TFixedCriticalSection : public TCriticalSection`).
using vclcompat::TCriticalSection;

//---------------------------------------------------------------------------
//  GemTimer -- golden uHGemEquipment.h:31-42.
//  A simple elapsed-tick-count timer (BCB6 idiom: TimerOn()/TimerSet(...)
//  arm it, TimerOff() polls "has the interval elapsed yet").
//
//  golden ulStartTicks is filled from `MyTickCount()` (common.cpp:1726-1729,
//  a bare `return GetTickCount();` wrapper). common.h's own translation
//  currently has MyTickCount gated out (`#if 0 // TODO(wave-timing)`,
//  D:/HT9045/HT9011UC_Cpp_V3.33.906.0/common.h:304-305) and common.h/.cpp are
//  NOT in this front's allowed file list, so GemTimer calls
//  `::GetTickCount()` (windows.h) DIRECTLY here rather than depending on an
//  extern that doesn't exist as a linkable symbol yet -- semantically
//  IDENTICAL to what MyTickCount() would return (it is nothing but that one
//  call). When a future wave un-gates common.h's MyTickCount, this can be
//  simplified to call it instead; behavior does not change either way.
//---------------------------------------------------------------------------
class GemTimer
{
    private:
        DWORD   ulStartTicks;           // tick count when the timer was (re)started
        int     iTimeLen;               // timer duration, in milliseconds
    public:
        // AI(W906-fire-verify) 20260716: DEVIATION, flagged by fire-wave
        // fidelity review -- golden's GemTimer (uHGemEquipment.h:31-42) has NO
        // constructor, so a real BCB6 instance's fields are whatever memory
        // happened to hold until TimerSet*/TimerOn is first called. Zero-init
        // added here defensively (needed by this wave's own "never-armed
        // timer" test case, and safe/behavior-preserving for every real golden
        // call path since none reads ulStartTicks/iTimeLen before TimerSet*).
        GemTimer() : ulStartTicks(0), iTimeLen(0) {}

        void TimerSet(int iTime);              // golden: iTime is in units of 0.1s
        void TimerSetSecAndOn(double iTime);    // golden: iTime is in seconds
        void TimerSetMSAndOn(int iTime);        // golden: iTime is in milliseconds
        void TimerOn();
        bool TimerOff();
};

//---------------------------------------------------------------------------
//  AI(W906-uHGemEquipment-BucketB) 20260717: TColor -- golden Graphics.hpp
//  TColor (a plain BGR-packed int RGB code, 0x00BBGGRR). vclcompat/
//  vcl_compat.h has no such alias (confirmed by grep, matching this wave's
//  own brief's own note). A separate `typedef int TColor;` ALREADY exists
//  TU-locally in cmydef.h:16 and acatchtray_shims.h:41 (same underlying
//  type, so no ODR conflict if both ever appear in one TU) -- declared again
//  here, header-local, so uHGemEquipment.h stays independently compilable
//  without pulling in either of those files just for a type alias (the .cpp
//  separately #includes cmydef.h for the CUSTOMER_CODE/CosFunction family --
//  see this header's own file-head "DEPENDENCY WIDENING" note -- so TColor
//  would in practice resolve to the SAME identical typedef there too).
//  Only the 3 clXxx constants golden's DoUpdateStatus actually reads are
//  declared (verified by reading golden uHGemEquipment.cpp:4747-4985 -- NOT
//  clBlack; StringOutColor's own clBlack use, golden :502/1992, is outside
//  this wave's member set and not translated).
//---------------------------------------------------------------------------
typedef int TColor;
const TColor clRed    = 0x000000FF;
const TColor clLime   = 0x0000FF00;
const TColor clYellow = 0x0000FFFF;
// AI(W906-uHGemEquipment-BucketC) 20260717: clBlack -- golden StringOutColor's
// ctor default (golden :502, `StringOutColor=clBlack;`) and SendLocalDataFrom's
// own reset (golden :1992). Real VCL clBlack is 0x00000000 (BGR-packed).
const TColor clBlack  = 0x00000000;

//---------------------------------------------------------------------------
//  AI(W906-uHGemEquipment-BucketB) 20260717: THGemXxx widget stand-ins --
//  THGem was a TForm (golden uHGemEquipment.h:90); these tiny structs stand
//  in for its real VCL components/externally-assigned pointers, matching the
//  established per-owner-class idiom already used by vclcompat/StringGrid.h
//  (TStringGrid, above) and FormsFacade.h's own TfMainTrayPanel{int
//  Color;bool Visible;}-shaped stand-ins -- each struct here models ONLY the
//  fields this wave's in-scope methods (InitialHGem/SaveSystemDefault/
//  DoUpdateStatus/ProcessShow/ManualCreatergRoleClick/FormShow/StringOut)
//  actually read or write; see each member declaration below for its exact
//  golden field citation.
//---------------------------------------------------------------------------

// golden TRadioGroup* (EnableOrDisablePtr/OnLineOrOffLine/RemoteOrLocal/
// rgRole) -- ->ItemIndex (int, r/w), ->Enabled (bool, w -- only
// OnLineOrOffLine/RemoteOrLocal are ever ->Enabled-written, by FormShow's
// KYEC branch), ->Items->Count (int, r -- InitialHGem/SaveSystemDefault pass
// it as ReadWriteIni's Minimum bound; golden's own .dfm Items.Strings count
// per radio group, e.g. 2 for OnLineOrOffLine/RemoteOrLocal/rgRole -- a
// caller/test must set this explicitly, same "caller-set" idiom already
// established by THGem::GemSystemPath above).
struct THGemRadioGroup
{
    int ItemIndex;
    bool Enabled;
    struct { int Count; } Items;
    THGemRadioGroup() : ItemIndex(0), Enabled(true) { Items.Count = 0; }
};

// golden TEdit* (edtIP/edtPort/edDeviceID/edtT3TimeOut/edtT5TimeOut/
// edtT6TimeOut/edtT7TimeOut/edtT8TimeOut -- NOTE: golden has NO edtT4TimeOut,
// confirmed by reading golden uHGemEquipment.h:117-129/123-129; T3/T5/T6/T7/T8
// only) -- ->Text (AnsiString, r/w), ->Enabled (bool, w -- only edtIP is ever
// ->Enabled-written).
struct THGemEdit
{
    AnsiString Text;
    bool Enabled;
    THGemEdit() : Enabled(true) {}
};

// golden TCheckBox* (GemCheckBoxAcceptHostOnlineRequest/GemCheckBoxShowBinary/
// GemCheckBoxShowHeadInformation/GemCheckBoxUseExtendedAlarm/
// chkAnnotatedEventReport/chkMoreMessageAbortProcess/ckAddDefaultReport/
// cbECChaneEventReport) -- ->Checked (bool, r/w); ->Visible (bool, w -- only
// cbECChaneEventReport is ever ->Visible-written, by FormShow).
struct THGemCheckBox
{
    bool Checked;
    bool Visible;
    THGemCheckBox() : Checked(false), Visible(true) {}
};

// golden TComboBox* (ComboBox1) -- ->ItemIndex (int, read only in this
// wave's scope: DoUpdateStatus's `SECSCommunicationMode=ComboBox1->ItemIndex;`).
struct THGemComboBox
{
    int ItemIndex;
    THGemComboBox() : ItemIndex(0) {}
};

// golden TPanel* (SECSConnectionState/GEMCommunicatingState/
// GemPanelControlState) -- externally-assigned (public, not __published;
// confirmed by golden header: declared alongside GemTerminalSendEdit/
// BtnEnableComm etc in THGem's plain `public:` section, NOT inside the
// `__published:` IDE-component block) -- ->Caption (AnsiString, r/w,
// compared against literals like "1:OffLine"/"SECS GEM Connection"),
// ->Color (TColor, w).
struct THGemPanel
{
    AnsiString Caption;
    TColor Color;
    THGemPanel() : Color(0) {}
};

// golden TSpeedButton* (BtnEnableComm/GemBtnOfflineRequest/
// GemBtnOnlineRequest/GemBtnOnlineRemote/GemBtnOnlineLocal) -- externally
// assigned (same public-not-__published category as THGemPanel above);
// ->Enabled (bool, write only in this wave's scope -- DoUpdateStatus never
// reads it back). UNLIKE the 3 TPanel*s above, golden's own DoUpdateStatus
// body does NOT NULL-guard these 5 -- a genuine golden invariant ("must be
// wired externally before DoUpdateStatus ever runs"), not a translation gap;
// preserved as-is (a caller/test must assign a real instance to each before
// calling DoUpdateStatus, exactly matching golden's own real-VCL risk).
struct THGemSpeedButton
{
    bool Enabled;
    THGemSpeedButton() : Enabled(true) {}
};

// golden TMemo* (DB/TerminalMemoPtr) -- ->Lines (a TStrings*; modeled here
// directly as a real vclcompat::TStringList*, which already supports the
// exact ->Add/->Assign(TStringList*)/->Count/->Clear() calls golden's own
// ProcessShow/StringOut(2-arg) bodies make -- no extra "Lines" wrapper layer
// needed, unlike FormsFacade.h's own no-op TfMainMemoLines shape, because
// THIS wave's tests need to inspect REAL flushed content, not just a call
// count), ->SelStart (int, w). Both DB and TerminalMemoPtr are externally
// assigned in golden (DB via the out-of-scope SetDisplayPtr(TMemo*);
// TerminalMemoPtr likewise, always NULL-guarded at its own call site per
// this file's Bucket-A commentary) -- THGem does NOT allocate either; a
// caller/test must `new` one and assign it (see THGem's own ctor note).
struct THGemMemo
{
    TStringList *Lines;
    int SelStart;
    THGemMemo() : SelStart(0) { Lines = new TStringList(); }
    ~THGemMemo() { delete Lines; }
    THGemMemo(const THGemMemo&) = delete;
    THGemMemo& operator=(const THGemMemo&) = delete;
    void Clear() { Lines->Clear(); }
};

//---------------------------------------------------------------------------
//  AI(W906-uHGemEquipment-BucketC) 20260717: THGemListBox -- stand-in for
//  golden TListBox *SFCodeResponseList (uHGemEquipment.h:138, __published).
//  Fields actually used in this wave's scope: ->Items (Add :2062 /
//  Strings[i] :4644 / Delete :4654 / Count :4617 / IndexOf :7040 / Clear
//  :7037) and ->Clear() (:4624, real TListBox::Clear == Items.Clear).
//  Same minimal-stand-in idiom as the Bucket-B THGemXxx widgets above.
//---------------------------------------------------------------------------
struct THGemListBox
{
    TStringList *Items;
    THGemListBox() { Items = new TStringList(); }
    ~THGemListBox() { delete Items; }
    THGemListBox(const THGemListBox&) = delete;
    THGemListBox& operator=(const THGemListBox&) = delete;
    void Clear() { Items->Clear(); }
};

// AI(W906-uHGemEquipment-BucketC) 20260717: TFixedCriticalSection --
// transcribed verbatim from golden uHGemEquipment.h:25-29 (golden `byte
// FDummy[95];` -> `unsigned char FDummy[95];`, noted deviation -- `byte` is
// a BCB6 Windows.pas alias for `unsigned char`, identical width/semantics).
// Needs vclcompat/SyncObjs.h's TCriticalSection (included above) as its base.
//---------------------------------------------------------------------------
class TFixedCriticalSection : public TCriticalSection
{
    private:
    unsigned char FDummy[95];
};

//---------------------------------------------------------------------------
//  THGem -- see this header's own file-head note above for scope.
//---------------------------------------------------------------------------
class THGem
{
public:
    THGem();
    ~THGem();

    THGem(const THGem&) = delete;
    THGem& operator=(const THGem&) = delete;

    // ==== StringGrid "databases" (golden uHGemEquipment.h:139-144) =========
    // Allocated (in the ctor) at the exact RowCount/ColCount their golden
    // .dfm entries specify -- see vclcompat/StringGrid.h's file-head note
    // for the full citation. Public so a test harness / future wave can
    // inspect them directly (matches golden __published visibility, which
    // in real VCL made these form-designer-visible/public-ish already).
    TStringGrid *strGrdAlarmOld;    // uHGemEquipment.dfm:375-383 (ColCount=<default 5>, RowCount=1)
                                     // NOTE: ReadAlamData (see .cpp) only ever
                                     // assigns RowCount, never ColCount -- a
                                     // real THGem needs ColCount widened
                                     // elsewhere (out of this wave's scope,
                                     // presumably to match strGrdAlarm's 12)
                                     // before ReadAlamData can hold a full
                                     // AlarmData.def row (11 used columns).
                                     // Confirmed empirically by this wave's
                                     // own test (tests/test_uHGemEquipment.cpp).
    TStringGrid *strGrdCEID;        // uHGemEquipment.dfm:396-405 (ColCount=258,  RowCount=1025)
    TStringGrid *stdGridReportID;   // uHGemEquipment.dfm:406-415 (ColCount=1026, RowCount=257)
    TStringGrid *strGrdAlarm;       // uHGemEquipment.dfm:428-437 (ColCount=12,   RowCount=<default 5>)

    // AI(W906-uHGemClass-Micro5) 20260721: sgSECSECData -- golden
    // uHGemEquipment.h:155 (__published), .dfm:452-462 (ColCount=4, RowCount=1,
    // FixedRows=0) -- the EC-side sibling of strGrdAlarm immediately above
    // (same "StringGrid database" shape: col0=No./col1=ECID/col2=Enable(0/1)/
    // col3=Function -- see golden uHGemEquipment.cpp:640-648, ReadECEnableData).
    // Allocated (ctor) at its exact .dfm dimensions, same idiom as
    // strGrdAlarm/strGrdCEID/stdGridReportID above.
    TStringGrid *sgSECSECData;      // uHGemEquipment.dfm:452-462 (ColCount=4, RowCount=1)

    // AI(W906-AlarmReportAck) 20260721: temp CEID/ReportID staging lists
    // (golden uHGemEquipment.h:530-533) -- populated by HTGem::
    // S2F34_DefineReportAcknowledgeSub/S2F36_LinkEventReportAcknowledgeSub
    // (uHGemClass.cpp, HGemPtr->...) while parsing an incoming S2F33/S2F35
    // message; drained by ProcessHostSendReportLinkID (below) /
    // HTGem::S2F34_ProcessHostSendReportID (uHGemClass.cpp). `vclcompat::TList`
    // spelled out fully-qualified here (NOT brought into the global
    // namespace by this header, matching SecsSvEcRegistration.h's own
    // established precedent -- see vcl_compat.h's own "TList deliberately
    // NOT brought into global namespace" note for why: a different,
    // unrelated global `class TList` shim lives in aHotPlateSubstrate.h).
    TStringList *slTempReportID;
    vclcompat::TList *lTempReportIDContent;
    TStringList *slTempCeID;
    vclcompat::TList *lTempCeIDContent;

    // ==== supporting state this wave's methods need ========================
    // golden AnsiString member (uHGemEquipment.h:428/429/430 area); populated
    // for real by THGem::SaveSystemDefault/InitialHGem (AI(W906-uHGemEquipment-
    // BucketB) 20260717: now in scope, see file-head note -- this comment's
    // original "OUT OF SCOPE" is stale). Defaults to "" here; a caller/test
    // must set it explicitly before calling ReadAlamData/WriteAlamData.
    AnsiString GemSystemPath;

    // ==== TCP/IP connection lifecycle (W906-uHGemEquipment-ConnLifecycle) ===
    // golden uHGemEquipment.h:93-94 (clientGem/srvGem), :185-227/309/451-479/
    // 591/693/700-701 (state bools/ints/AnsiStrings), .dfm:542-573 (clientGem/
    // srvGem design-time defaults, applied in the ctor -- see the .cpp).
    // TClientSocket/TServerSocket/TCustomWinSocket/TErrorEvent all come from
    // vclcompat/ClientSocket.h + vclcompat/ServerSocket.h, already pulled in
    // transitively by vclcompat/vcl_compat.h above (both headers' own
    // `using namespace Scktcomp;` already brings these into global scope).
    TClientSocket *clientGem;   // golden .dfm:542-554 (active/client role)
    TServerSocket *srvGem;      // golden .dfm:562-573 (passive/server role)

    bool bConnect;                // golden uHGemEquipment.h:309
    bool bOnLine;                 // golden :693
    bool bOnLineLocal;            // golden :224
    bool bAutoConnect;            // golden :186
    bool bStartConnect;           // golden :221
    bool bStartOnLine;            // golden :226
    bool bTCPIP_Error;            // golden :187
    bool bServoSocketConnect;     // golden :457
    bool bReceiveMultiConnect;    // golden :591
    bool bUseClientSocket;        // golden :464
    // AI(W906-uHGemEquipment-ConnLifecycle) 20260717: bOpenCommuncation/
    // bCloseCommuncation/bS1F2_OnLineData are golden members (:397,398,393)
    // not named in this wave's own header-additions list, but DoOpenCommuncation/
    // CloseCommuncation/OnlineLocalOrRemote/DoOnLine (all explicitly in this
    // wave's scope) directly read/write them -- added here as the minimal
    // extra surface those in-scope functions require to compile at all.
    bool bOpenCommuncation;       // golden :397
    bool bCloseCommuncation;      // golden :398
    bool bS1F2_OnLineData;        // golden :393

    int countConnect;                       // golden :477
    int iConnectTryCount;                   // golden :478
    int iEstablishCommunicationsTryCount;    // golden :222
    int iOpenCommuncationTask;               // golden :475
    int iStartConnectTask;                  // golden :220
    int iStartOnLineTask;                   // golden :225
    // AI(W906-uHGemEquipment-ConnLifecycle) 20260717: iTimeFormat (golden
    // :233) is likewise not in this wave's enumerated member list, but
    // GetTimeInfo (in scope) branches on it directly.
    int iTimeFormat;                        // golden :233

    GemTimer DelayOpenCommuncation;   // golden :476
    GemTimer ConnectDelay;            // golden :479

    // AI(W906-uHGemEquipment-BucketC) 20260717: D4 -- these two are ALIASED
    // (not separately allocated) as of this wave. WireCodec allocates its own
    // WaitShowString/LogDataString (SecsWireCodec.cpp ctor); THGem's ctor
    // assigns these two pointers to point at WireCodec's instances instead of
    // `new`ing its own pair, so codec-side StringOut() calls (SendLocalDataFrom,
    // ShowSML, ...) land in the SAME sink THGem's own ProcessShow/
    // SaveSECSGEMTextToLog drain -- golden had exactly ONE such pair; before
    // this wave the port had two (THGem's own + WireCodec's), so codec-side
    // trace lines never reached ProcessShow/the log file. ~THGem does NOT
    // delete these two (ownership = WireCodec; its dtor, which runs AFTER
    // ~THGem's body per member-destruction order, deletes them for real).
    TStringList *WaitShowString;   // golden :597 (ALIASED to WireCodec.WaitShowString, see ctor)
    TStringList *LogDataString;    // golden :447 (ALIASED to WireCodec.LogDataString, see ctor)

    AnsiString TimeString;   // golden :700
    AnsiString GemClock;     // golden :302
    // AI(W906-SysModWire) 20260720: GemMDLN/GemSOFTREV -- golden uHGemEquipment.h
    // :306-307. Set by SetMachineTypeAndSoftwarseVer(Mdln,SoftVer) (golden
    // :6510-6514, declared below near DoConnect); golden's own ctor never
    // touches either (both stay AnsiString-default "" until that setter runs) --
    // deliberately NOT added to this ctor's init-list, matching that golden
    // "ctor does not touch" semantics exactly (C++ default-constructs AnsiString
    // to "" either way, so omitting from the init-list is behavior-neutral).
    AnsiString GemMDLN;      // golden :306
    AnsiString GemSOFTREV;   // golden :307
    // NOTE: golden's real 3rd field name is "SystemDate" (uHGemEquipment.h:230),
    // not "SystemDay" -- kept as golden spells it (GetTimeInfo/DecodeDate both
    // reference it by this exact name).
    Word SystemYear, SystemMonth, SystemDate;           // golden :230
    Word SystemHour, SystemMin, SystemSec, SystemMSec;  // golden :231

    // ==== Bucket C: wire-codec-embedded engine + socket-receive-pump / ======
    // ==== T3-timeout / HSMS-handshake / Timer1Timer master-SM state    ======
    // (W906-uHGemEquipment-BucketC 20260717; golden line cites per-member.)
    // D1: embedded BY VALUE, following HTGem's own already-proven precedent
    // (uHGemClass.h:127). The WaitShowString/LogDataString raw pointers
    // declared earlier in this class are ALIASED to WireCodec's own lists in
    // the ctor BODY (which runs only after every member -- including this one
    // -- has finished constructing, so relative declaration order between
    // them is immaterial; the dtor likewise skips deleting the aliased pair,
    // see ~THGem). Does NOT embed SecsSvEcRegistration this wave -- no
    // in-scope method touches SV/EC registration state (FormCreate itself is
    // out of scope, see file-head note) -- deferred to a future "SV/EC
    // registration wave".
    SecsWireCodec WireCodec;

    // AI(W906-SvEcDataItem) 20260720: SvEcReg -- SV/EC registration
    // bookkeeping (see file-head note above). By-value embed, default-
    // constructed (SecsSvEcRegistration's own ctor allocates its 18 heap
    // TStringList*/TList*s; nothing extra needed in THGem's own ctor).
    SecsSvEcRegistration SvEcReg;

    STypeStruct SType;                              // golden :209 (InitSTypeStruct() populates it, ctor call below)
    TColor StringOutColor;                           // golden :237 (ctor = clBlack)

    bool bWaitSelectRsp;                             // golden :390 (ctor false)
    bool bWaitDeSelectRsp;                           // golden :391 (ctor false)
    bool bWaitEstablishCommunicationsResponse;       // golden :691 (ctor false)
    bool bWaitEstablishCommunicationsResponseError;  // golden :692 (ctor false)
    bool bReceiveEstablishCommunicationsRequest;     // golden :399 (ctor false)
    bool bSeprate;                                   // golden :394 (ctor false)
    // golden ctor-untouched (real BCB6 zero-inits for free; explicit here per
    // this file's own established "flagged deviation" precedent, see
    // DoUpdateStatus's own supporting-state note below).
    bool bDataFormatOK;                              // golden :528
    bool bFirstEntry;                                 // golden :454 (ctor :450 true)
    bool bFirstBlock;                                 // golden :472 (ctor :472 true)

    // AI(W906-SysModWire) 20260720: ProcessReceiceData's S,F data-message
    // dispatch tail (golden :8812-8988) needs these -- MoveCheckCallBack (a
    // GemCallBack installer, golden :319, ctor :503 NULL; the only real
    // installer in golden is UsecegemMainFrom.cpp:625, TFSECS -- untranslated,
    // so this stays NULL offline) and 6 flag members set (never guarded) by
    // the tail's own dispatch branches.
    int (*MoveCheckCallBack)();          // golden :319 (ctor :503 NULL)
    bool bReceiveS7F6;                   // golden :459 (ctor :462 false)
    bool bReceiveS101F5;                 // golden :460 (ctor :463 false)
    bool bReceiveS101F6;                 // golden :461 (ctor :464 false)
    bool bReceiveS101F7;                 // golden :462 (ctor :465 false)
    bool bReceiveS101F8;                 // golden :463 (ctor :466 false)
    // AI(W906-SysModWire) 20260720: bReceiveS110F2 (golden :625) -- GOLDEN BUG
    // preserved: golden's own ctor NEVER initializes this member (grep confirms
    // no ctor assignment anywhere in golden uHGemEquipment.cpp), and no golden
    // code ever READS it either (write-only at golden :8942). This port's ctor
    // explicitly zero-inits it below (matching this file's own established
    // "flagged deviation from golden's raw uninitialized state" precedent, see
    // bDataFormatOK/iOldSecProcessSFNoResponse/RemoteSystemByte above) -- the
    // ONLY behavioral non-bit-identity this wave introduces vs. golden.
    bool bReceiveS110F2;                 // golden :625 -- see note above

    int  iFileCount;                                  // golden :721 (ctor :672 = 0)
    int  Timer1Task, Timer1ct;                        // golden :519 (ctor :494-495 = 1, 0)
    Word iOldSecProcessSFNoResponse;                  // golden :508 (ctor-untouched; zero-init, flag)
    int  iAutoConnectSec, iAutoConnectDelay;          // golden :509 (ctor :485-486 = 0, 2)
    unsigned int RemoteSystemByte;                    // golden :212 (ctor-untouched; zero-init, flag)

    AnsiString Alias;                                 // golden :529
    // golden TForm's own `Caption` property (Timer1Timer :5251 `Caption=Alias;`)
    // -- THGem is not modeled as a real window in this port (same "not a real
    // window" posture as FormShow's own dropped Left/Top/Width/Height) -- a
    // plain AnsiString stand-in suffices since nothing reads it back except
    // that same assignment's own right-hand side.
    AnsiString Caption;

    GemTimer DelayForServoError;                      // golden :592

    // golden :473-474 (ctor :474-475 = 10240 + `new[]`; dtor `delete[]`).
    // NOTE: ProcessSocketReceiveData's own LOCAL `unsigned char *EthernetBuffer`
    // (golden :9035) SHADOWS this member inside that one function's body --
    // a genuine golden quirk (the member is never actually read/written by
    // that function; only the local is) -- preserved, see that method's own
    // .cpp comment.
    unsigned char *EthernetBuffer;
    int iEthernetBufferLen;

    THGemListBox *SFCodeResponseList;                 // golden :138 (__published; ctor-allocated)
    TStringList  *TimeLeft;                           // golden :229 (ctor :584 new)
    TFixedCriticalSection *pLockOnSocketRecvice;      // golden :717 (ctor :668 new)
    TCriticalSection      *csSFCodeResponse;          // golden :723 (ctor :673 new)
    TMemoryStream *RecvMemoryBuffer;                  // golden :718 (ctor :669 new)
    TMemoryStream *ProcBuffer;                        // golden :719 (ctor :670 new)
    TMemoryStream *TempProcBuffer;                    // golden :720 (ctor :671 new)

    // ==== Widget stand-ins (W906-uHGemEquipment-BucketB) ====================
    // golden uHGemEquipment.h's __published block (:92-165) for the first 20
    // (all real .dfm components -- allocated in THGem's own ctor, see the
    // .cpp; never NULL in a normally-constructed THGem, matching real VCL's
    // Owner/.dfm-streaming guarantee), and its plain `public:` section
    // (:400-423) for the trailing 11 (externally-assigned pointers, default
    // NULL -- a caller/test must wire one up before exercising the method
    // that needs it; see each THGemXxx struct's own comment above for the
    // exact golden field-category citation).
    THGemRadioGroup *EnableOrDisablePtr;     // golden :413 (externally assigned, default NULL)
    THGemRadioGroup *OnLineOrOffLine;        // golden :112 (__published)
    THGemRadioGroup *RemoteOrLocal;          // golden :113 (__published)
    THGemRadioGroup *rgRole;                 // golden :122 (__published)

    THGemEdit *edtIP;                        // golden :123 (__published)
    THGemEdit *edtPort;                      // golden :124 (__published)
    THGemEdit *edDeviceID;                   // golden :161 (__published)
    THGemEdit *edtT3TimeOut;                 // golden :125 (__published)
    THGemEdit *edtT5TimeOut;                 // golden :126 (__published)
    THGemEdit *edtT6TimeOut;                 // golden :127 (__published)
    THGemEdit *edtT7TimeOut;                 // golden :128 (__published)
    THGemEdit *edtT8TimeOut;                 // golden :129 (__published)

    THGemCheckBox *GemCheckBoxAcceptHostOnlineRequest;   // golden :131 (__published)
    THGemCheckBox *GemCheckBoxShowBinary;                // golden :132 (__published)
    THGemCheckBox *GemCheckBoxShowHeadInformation;       // golden :133 (__published)
    THGemCheckBox *GemCheckBoxUseExtendedAlarm;          // golden :134 (__published)
    THGemCheckBox *chkAnnotatedEventReport;              // golden :135 (__published)
    THGemCheckBox *chkMoreMessageAbortProcess;           // golden :136 (__published)
    THGemCheckBox *ckAddDefaultReport;                   // golden :153 (__published)
    THGemCheckBox *cbECChaneEventReport;                 // golden :162 (__published)

    THGemComboBox *ComboBox1;                // golden :111 (__published)

    THGemPanel *SECSConnectionState;         // golden :400 (externally assigned, default NULL)
    THGemPanel *GEMCommunicatingState;       // golden :401 (externally assigned, default NULL)
    THGemPanel *GemPanelControlState;        // golden :402 (externally assigned, default NULL)

    THGemSpeedButton *BtnEnableComm;         // golden :405 (externally assigned, default NULL)
    THGemSpeedButton *GemBtnOfflineRequest;  // golden :408 (externally assigned, default NULL)
    THGemSpeedButton *GemBtnOnlineRequest;   // golden :407 (externally assigned, default NULL)
    THGemSpeedButton *GemBtnOnlineRemote;    // golden :409 (externally assigned, default NULL)
    THGemSpeedButton *GemBtnOnlineLocal;     // golden :410 (externally assigned, default NULL)

    THGemMemo *DB;                // golden :300 (externally assigned via out-of-scope SetDisplayPtr, default NULL)
    THGemMemo *TerminalMemoPtr;   // golden :677 (externally assigned, default NULL, always null-guarded at its call site)

    // AI(W906-uHGemClass-Micro5) 20260721: GemRemoteReceipeList -- golden
    // uHGemEquipment.h:414 (`TCheckListBox *GemRemoteReceipeList;`, plain
    // `public:`, NOT __published) -- externally assigned, matching golden's
    // own ctor :504 `GemRemoteReceipeList=NULL;` (right alongside
    // `MoveCheckCallBack=NULL;` immediately above it there). Reuses the
    // EXISTING THGemListBox stand-in (above, already used for
    // SFCodeResponseList) -- its ->Items/->Clear() cover every field
    // HTGem::Process_S7F20_CurrentEPPIDData (uHGemClass.cpp, this wave)
    // touches. UNLIKE SFCodeResponseList (__published, ctor-allocated --
    // golden .dfm-streamed before any real VCL form's ctor body runs), this
    // one is NOT allocated here -- same default-NULL/externally-assigned
    // idiom as TerminalMemoPtr immediately above (a caller/test must `new`
    // one and assign it; Process_S7F20's own golden NULL-guard, preserved
    // verbatim in uHGemClass.cpp, is what makes that safe).
    THGemListBox *GemRemoteReceipeList;

    // golden AnsiString members (:524-526); read by InitialHGem as the
    // ReadIniData "DefaultValue" fallback for Address/Port/DeviceID when no
    // ini entry exists yet. Ctor-inited to "" (golden ctor :451-453).
    AnsiString DefaultAddress;
    AnsiString DefaultPort;
    AnsiString DefaultDeviceID;

    // golden int members (:632-636, "pig 2014.07.28 KYEC_SECS"); ctor-inited
    // to 30 (golden ctor :454-458).
    int T3TimeOut;
    int T5TimeOut;
    int T6TimeOut;
    int T7TimeOut;
    int T8TimeOut;

    // golden AnsiString member (:429); the ini file InitialHGem/SaveSystemDefault
    // actually read/write (DISTINCT from GemSystemPath above, which
    // ReadAlamData/WriteAlamData use for AlarmData.def). Defaults to ""; a
    // caller/test must set it explicitly, same idiom as GemSystemPath.
    AnsiString GemSystemIniPath;

    // golden bool member (:... FormShow/ManualCreatergRoleClick's shared
    // "has the form actually been shown yet" latch -- gates
    // ManualCreatergRoleClick's whole body). Ctor-inited false (golden ctor
    // :617, "bShow=false;").
    bool bShow;

    // ==== DoUpdateStatus's own supporting state (golden uHGemEquipment.h
    // "GEM SV data" block :190-192, plus :... UpdateStatus locals) ==========
    // AI(W906-uHGemEquipment-BucketB) 20260717: golden's own ctor NEVER
    // explicitly assigns bClientSocketActive/bOldConnect/GemControlState/
    // GemControlPreState/OldSUpdateStatus/ctUpdateStatus (grepped golden's
    // whole ctor body, uHGemEquipment.cpp:444-674 -- confirmed absent) --
    // real BCB6/VCL zero-initializes EVERY instance field before a ctor body
    // even runs (TObject.NewInstance zeroes the block), so golden gets
    // false/0/"" for these "for free" without an explicit assignment. C++
    // gives no such guarantee, so (matching this file's own established
    // "flagged deviation: zero-init defensively" precedent, see GemTimer's
    // ctor note) all 6 are explicitly zero-initialized in THGem's own ctor
    // below.
    int ctUpdateStatus;                 // golden :... (the /10 update throttle counter)
    bool bClientSocketActive;
    int iServoConnectCT;                // golden ctor :492 (explicitly -1, NOT zero -- see ctor)
    bool bOldConnect;
    bool flag1UpdateStatus;             // golden ctor :489 (explicitly true, NOT zero -- see ctor)
    bool flag2UpdateStatus;             // golden ctor :490 (explicitly true)
    bool flag3UpdateStatus;             // golden ctor :491 (explicitly true)
    bool bConnectUpdateStatus;          // golden ctor :493 (explicitly true)
    AnsiString OldSUpdateStatus;
    unsigned char GemControlState;
    unsigned char GemControlPreState;
    char SECSCommunicationMode;         // golden ctor :500 (explicitly 0)

    // ==== FormCreate's "system SV" targets (W906-SvEcDataItem 20260720) =====
    // golden uHGemEquipment.h:191/196/198/235/303-304/602-604/608-617/701/707
    // (member declarations) + uHGemEquipment.cpp:6165-6207 (FormCreate, the
    // sole registration site -- see .cpp). None of these is ever explicitly
    // assigned by golden's own ctor (uHGemEquipment.cpp:444-674, grepped) --
    // real BCB6/VCL zero-inits every instance field for free; all are
    // explicitly zero/empty-initialized in THIS port's ctor below, matching
    // this file's own established "flagged deviation: zero-init defensively"
    // precedent (see GemTimer/DoUpdateStatus's own ctor notes above).
    unsigned char GemLinkState;                    // golden :191 (SV5)
    long lCPUFreq;                                  // golden :602 (SV10, via TasmInfo::GetCPUFreq)
    char szManID[256];                              // golden :603 (SV11, via TasmInfo::GetManID)
    char szGetCPUType[256];                         // golden :604 (SV12, via TasmInfo::GetCPUType)
    int Disk_C_TotalSpaceMB;                        // golden :608 (SV13)
    int Disk_D_TotalSpaceMB;                        // golden :609 (SV14)
    int Disk_C_TotalFreeSpaceMB;                    // golden :611 (SV15)
    int Disk_D_TotalFreeSpaceMB;                    // golden :612 (SV16)
    unsigned long ulMemoryLoad;                     // golden :615 (SV17)
    unsigned long ulTotalPhys;                      // golden :616 (SV18)
    unsigned long ulAvailPhys;                      // golden :617 (SV19)
    // golden :303-304/235 (EC68 "Time Format" min/max/default bounds --
    // iTimeFormat itself already exists above, golden :233).
    int iMinTimeFormat;                             // golden ctor :650 (explicitly 0)
    int iMaxTimeFormat;                             // golden ctor :651 (explicitly 3)
    int iTimeFormatDefault;                         // golden NEVER inits this either (see note above) -- flagged deviation
    int SV_70_UNT1_ReceipeStruct;                   // golden :707 (SV70)
    AnsiString SV_71_ASCII_FilenameExtened;         // golden :701 (SV71)

    // AI(W906-uHGemClass-Micro5) 20260721: UploadFileString -- golden
    // uHGemEquipment.h:675 (part of golden's SV/EC-cluster private members,
    // ctor-allocated alongside SV_ID/EC_ID -- golden uHGemEquipment.cpp:567
    // `UploadFileString=new TStringList;`, ~THGem :713/744 Clear()+delete).
    // Added standalone here rather than folded into SvEcReg -- that embed
    // (above) models the DISTINCT SV_ID/SV_TYPE/EC_ID/... registration-table
    // cluster only (confirmed by reading SecsSvEcRegistration.h: no
    // UploadFileString member there); this is a simple recipe/upload-filename
    // staging list, populated by SetReceipeDirectoryAndGlobalName (still out
    // of scope) and read by S101F2/S101F4/Process_S7F20's sibling S7F20 (all
    // uHGemClass.cpp -- S101F2/S101F4 un-gated this wave). Lifecycle idiom
    // deliberately mirrors HTGem's OWN SecsAlarmMessage/FMessageList pair
    // (uHGemClass.h:104/242; uHGemClass.cpp ctor/dtor) -- `new`'d in THGem's
    // ctor, `delete`d (NULL-guarded, ->Clear()'d first) in THGem's dtor -- see
    // both .cpp definitions for the exact shape this mirrors.
    TStringList *UploadFileString;

    int GemSpoolCountActual;                        // golden :196 (SV54)
    char GemSpoolStartTime[256];                    // golden :198 (SV57)

    // ==== S6F24/S7F18 supporting state (W906-uHGemClass-Micro6) =============
    // golden uHGemEquipment.h:430/694-695/698. bSpoolActive/bBeginTransferSpool
    // are S6F24_RequestSpooledDataAcknowledgementSend's own gate + latch
    // (ctor false, golden ctor :604/607 -- see THGem's ctor below); with the
    // safe false default, S6F24's only side effect (the `system("del ...")`
    // spool-wipe) stays a no-op until some future "DoSpool subsystem" wave
    // actually drives bSpoolActive true -- NOT built this wave, per plan.
    // GemSpoolPath is the directory that wipe targets (golden ctor :814 builds
    // it from CurrentDirectory+"SPOOL", out of scope; defaults to "" here,
    // same "caller/test must set it explicitly" idiom as GemSystemPath above).
    // AI(W906-uHGemClass-Micro6) 20260721: added 3 THGem members to un-gate
    // S6F24_RequestSpooledDataAcknowledgementSend (golden uHGemClass.cpp:2053-2079).
    bool bSpoolActive;                              // golden :694 (ctor false)
    bool bBeginTransferSpool;                       // golden :695 (ctor false)
    AnsiString GemSpoolPath;                        // golden :430

    // UpLoadPath -- golden :698. The recipe-upload base directory S7F18
    // (delete process program) resolves each PPID against (`UpLoadPath+"\\"+
    // PPID`) and SetReceipeDirectoryAndGlobalName (still out of scope, see
    // S7F20's own gate note above UploadFileString) is its only real writer
    // in golden. Plain scalar, no ctor default in golden's own ctor either
    // (grepped uHGemEquipment.cpp:444-674 -- absent), so it defaults to ""
    // here too, same as UpLoadPath's own AnsiString() default ctor would give
    // BCB6 for free.
    // AI(W906-uHGemClass-Micro6) 20260721: added to un-gate
    // S7F18_DeleteProcessProgramAcknowledge (golden uHGemClass.cpp:2115-2166).
    AnsiString UpLoadPath;                          // golden :698

    // ==== CEID / Report StringGrid-backed "database" family =================
    void SetCEIDContent(unsigned iCeid, AnsiString CeidAlias, unsigned iReportCount, unsigned *iReportIDData, int Mode);
    void SetCEIDContent(unsigned iCeid, unsigned iReportCount, unsigned *iReportIDData, int Mode);
    unsigned GetCEIDContent(unsigned iCeid, unsigned *iReportIDData, int Mode);
    bool SetReportIDContent(unsigned iCeid, unsigned iReportCount, unsigned *iReportIDData, int Mode);
    bool DeleteReportID(unsigned iCeid, int Type);
    bool DeleteReportIDOfCeid(unsigned iCeid);
    bool DeleteAllHostDefineReportID();
    unsigned GetReportIDContent(unsigned iCeid, unsigned *iReportIDData, int Mode);
    bool IsEnableEvent(unsigned iDataID, unsigned iCeid);
    bool CheckCeidExist(AnsiString Ceid);
    int GetReportIDType(AnsiString ReportID);
    bool CheckCeidAlreadyDefine(AnsiString Ceid);
    bool CheckReportIDExist(AnsiString ReportID);
    void AddCeidReportID(AnsiString Ceid, AnsiString ReportID);
    void DeleteAllHostReportIDOfCeid(unsigned ReportID);
    void DeleteHostReportIDOfAssignCeid(AnsiString Ceid);
    void DeleteAllHostDefineCeid();
    bool CheckReportAlreadyDefine(AnsiString ReportID);
    bool CheckCEIDExist(AnsiString sID);   // upper-case-CEID sibling of CheckCeidExist (golden :8051)

    // ==== Report/Link Acknowledge composers (AI(W906-AlarmReportAck) 20260721)
    // golden uHGemEquipment.cpp:7838-8026 -- pure InitLocalHead/DataItemOut/
    // SendLocalData composition (see those forwarders below) + already-real
    // CheckCeidExist/CheckCeidAlreadyDefine/CheckReportIDExist/
    // AddCeidReportID/DeleteHostReportIDOfAssignCeid/SaveEventReportData
    // (above); no VCL dependency. ProcessHostSendReportLinkID drains
    // slTempCeID/lTempCeIDContent (above); consumed by
    // HTGem::S2F36_LinkEventReportAcknowledge (uHGemClass.cpp). The
    // ReportAcknowledge/ReportLinkAcknowledgeError families are consumed by
    // HTGem::S2F34_ProcessHostSendReportID/S2F34_DefineReportAcknowledge/
    // S2F36_LinkEventReportAcknowledgeSub (ditto).
    void ProcessHostSendReportLinkID();                                    // golden :7838-7898
    void ReportAcknowledge(unsigned char Code);                            // golden :7969-7974 (S2F34 DRACK composer)
    void DefineReportAcknowledgeAccept();                                  // golden :7976-7979
    void DefineReportAcknowledgeInsufficientSpace();                       // golden :7981-7984
    void DefineReportAcknowledgeFormatError();                             // golden :7986-7989
    void DefineReportAcknowledgeAlreadyDefined();                          // golden :7991-7994
    void DefineReportAcknowledgeInvalidSVID();                             // golden :7996-7999
    void ReportLinkAcknowledgeError(unsigned char ErrCode);                // golden :8001-8006 (S2F36 LRACK composer)
    void LinkReportAcknowledgeFormatError();                               // golden :8008-8011
    void LinkReportAcknowledgeAlreadyDefined();                            // golden :8013-8016
    void LinkReportAcknowledgeInvalidCeID();                               // golden :8018-8021
    void LinkReportAcknowledgeInvalidReportID();                           // golden :8023-8026

    // golden's tail call inside EnableDisableEventReport (golden
    // uHGemEquipment.cpp:8034-8039). AI(W906-AlarmReportAck) 20260721: no
    // longer a stub -- now a REAL forwarder (InitLocalHead/DataItemOut/
    // SendLocalData, see .cpp), now that THGem has its own InitLocalHead/
    // DataItemOut (above). Replaces the earlier no-op stub this wave closed
    // the gap for.
    void EnableDisableEventReportAcknowledgeError(unsigned char ErrCode);
    void EnableDisableEventReportAcknowledgeCeidNotExist();                 // golden :8041-8044
    void EnableDisableEventReportAcknowledgeFormatError();                  // golden :8046-8049
    void EnableDisableEventReport(bool CEED, int slen, unsigned *CEID);

    void ReadEventReportData();
    void SaveEventReportData();

    // ==== generic StringGrid <-> TStringList tab-format (de)serialization ===
    void CopyStringGridAsTabFormat(TStringGrid *strGrd, TStringList *memoPtr);
    void PasteStringGridAsTabFormat(TStringGrid *strGrd, TStringList *memoPtr);

    // ==== Alarm-grid siblings ===============================================
    void SetAlamData(int iRowCount, AnsiString ALID, AnsiString Class, AnsiString ALTX, AnsiString Position);
    void ReadAlamData();
    void WriteAlamData();

    // AI(W906-AlarmReportAck) 20260721: golden uHGemEquipment.cpp:3337-3372 --
    // pure strGrdAlarm StringGrid composition (+ WriteAlamData above); zero
    // new dependency. Consumed by HTGem::S5F4_EnableDisableAlarmAcknowledge/
    // S5F6_ListAlarmData (uHGemClass.cpp, this same wave).
    bool EnableDisableAlarm(AnsiString S, unsigned char T);       // golden :3337-3352
    void EnableDisableAlarmAll(unsigned char T);                  // golden :3354-3364
    int GetAlarmIndex(AnsiString S);                              // golden :3366-3372

    // AI(W906-uHGemClass-Micro5) 20260721: EC-side siblings of the Alarm
    // family above -- golden uHGemEquipment.cpp:9257-9302, a mechanical
    // structural clone of WriteAlamData/EnableDisableAlarm/
    // EnableDisableAlarmAll, just against sgSECSECData instead of
    // strGrdAlarm (see that member's own comment above). golden's remaining
    // 3 EC-file siblings -- ReadECEnableData/GetECEnableData/SetECEnableData,
    // uHGemEquipment.cpp:9226-9253/9306-9334 -- are NOT added: no in-scope
    // caller needs them (only HTGem::S125F2_EnableDisableECDataAcknowledge,
    // this same wave, calls EnableDisableECData/EnableDisableECDataAll).
    void WriteECEnableData();                                      // golden :9257-9269
    bool EnableDisableECData(AnsiString ID, unsigned char T);      // golden :9273-9288
    void EnableDisableECDataAll(unsigned char T);                  // golden :9292-9302

    // ==== SV/EC DataItem family (W906-SvEcDataItem 20260720) ================
    // golden uHGemEquipment.cpp:2472-3336 (SvEcReg-backed) + :7623-7688
    // (Report/CEID composers, pure WireCodec+SV composition). See .cpp for
    // the golden-VCL-branch gate note (design doc D2).
    bool DataItemOutSV(AnsiString SVID);                          // golden :2472-2761
    bool DataItemOutSVNameList(AnsiString SVID);                  // golden :2765-2801
    bool DataItemOutSVNameListWithValue(AnsiString SVID);         // golden :2803-2840 (S103F11/F12)
    void DataItemOutEC(AnsiString ECID);                          // golden :2845-3112
    bool IsValidSVID(AnsiString SVID);                            // golden :3114-3122
    void DataItemOutECNameList(AnsiString ECID);                  // golden :3127-3332
    void FormCreate(TObject *Sender);                             // golden :6165-6207 (__fastcall dropped, project convention)

    void SendRepoerID(unsigned iReportID);                        // golden :7623-7632 (__fastcall dropped)
    void SendAnnotatedRepoerID(unsigned iReportID);                // golden :7648-7662 (__fastcall dropped)
    void SendCeid(unsigned iCeid);                                 // golden :7664-7675 (__fastcall dropped)
    void SendAnnotatedCeid(unsigned iCeid);                        // golden :7677-7688 (__fastcall dropped)

    // ==== TCP/IP connection lifecycle methods (W906-uHGemEquipment-ConnLifecycle) ====
    void __fastcall clientGemConnect(TObject *Sender, TCustomWinSocket *Socket);        // golden :2100-2104
    void __fastcall clientGemDisconnect(TObject *Sender, TCustomWinSocket *Socket);      // golden :2108-2116
    void __fastcall clientGemError(TObject *Sender, TCustomWinSocket *Socket,
                                    TErrorEvent ErrorEvent, int &ErrorCode);              // golden :2120-2134
    void __fastcall clientGemConnecting(TObject *Sender, TCustomWinSocket *Socket);      // golden :2138-2142

    bool DoOpenCommuncation();       // golden :3382-3490
    void OnlineLocalOrRemote();      // golden :3604-3621 (mostly inert -- see .cpp)
    bool DoOnLine();                 // golden :3626-3645

    bool CheckSocketActiveFalse();   // golden :5146-5159

    void CloseCommuncation();                              // golden :5548-5552
    void Connect();                                        // golden :5557-5563
    void DisConnect();                                      // golden :5567-5572
    bool IsConnect();                                       // golden :5576-5579
    void SetEstablishCommunicationsTryCount(int ct);        // golden :5583-5586
    void OnLine(bool Mode);                                 // golden :5592-5598
    void OnLineLocal();                                     // golden :5602-5606
    void OnLineRemote();                                    // golden :5610-5614
    void OffLine();                                         // golden :5619-5624
    bool IsOnLine();                                        // golden :5628-5631
    bool GetOnLineMode();                                   // golden :5635-5638
    void SetCanAcceptHostOnLineRequest(bool flag);          // golden :5642-5644 (truly empty body)

    void __fastcall srvGemClientConnect(TObject *Sender, TCustomWinSocket *Socket);      // golden :6812-6837
    AnsiString __fastcall GetSocketErrorMsg(TObject *Sender, int iErrCode);               // golden :6842-6845
    void __fastcall srvGemClientError(TObject *Sender, TCustomWinSocket *Socket,
                                       TErrorEvent ErrorEvent, int &ErrorCode);           // golden :6850-6874
    void __fastcall srvGemClientDisconnect(TObject *Sender, TCustomWinSocket *Socket);    // golden :6897-6910

    void __fastcall StringOut(AnsiString S);          // golden :392-396
    void __fastcall StringBinaryOut(AnsiString S);    // golden :401-404 (truly commented-out body)

    void __fastcall SaveSECSGEMErrToLog(AnsiString asSaveStr);   // golden :422-439

    void __fastcall ClearDefaultEvenReport();   // golden :4988-5008

    void GetTimeInfo();   // golden :315-348 (TimeString/GemClock half only -- see .cpp)

    // ==== Widget-persisted state + status refresh (W906-uHGemEquipment-BucketB) ====
    void __fastcall InitialHGem();        // golden :5011-5099
    void __fastcall SaveSystemDefault();  // golden :5101-5140
    void DoUpdateStatus();                // golden :4747-4985
    void __fastcall ProcessShow();        // golden :5165-5173
    void FormClose();                     // golden :6887-6892 (Sender/TCloseAction& dropped -- see .cpp)
    void __fastcall ManualCreatergRoleClick(TObject *Sender);   // golden :6915-6931
    void __fastcall FormShow(TObject *Sender);                  // golden :6936-6950 (partial -- see .cpp)
    void __fastcall StringOut(AnsiString S, TColor C);          // golden :409-417

    // AI(W906-uHGemEquipment-BucketC) 20260717: golden uHGemEquipment.h:249
    // `void DoSeparate();` -- REAL as of this wave (REPLACES the Bucket-B
    // gated no-op stub that used to sit here -- golden's real body is
    // wire-codec/SML, which needed WireCodec embedded first; see
    // uHGemEquipment.cpp's own DoSeparate definition, golden :3518-3531).
    // DoSelect is golden's sibling (golden :248, :3496-3509) -- was never
    // declared at all before this wave (no stub existed for it).
    void DoSelect();      // golden :3496-3509
    void DoSeparate();    // golden :3518-3531 (now real -- see above)

    // ==== Bucket C: socket receive pump / T3 timeout / HSMS control-message
    // ==== handshake / Timer1Timer master state machine (W906-uHGemEquipment-
    // ==== BucketC 20260717) ==================================================
    void __fastcall clientGemRead(TObject *Sender, TCustomWinSocket *Socket);   // golden :9008-9028
    void __fastcall ProcessSocketReceiveData();                                // golden :9030-9207
    void __fastcall Timer1Timer(TObject *Sender);                              // golden :5176-5527

    int  DoConnect();                          // golden :3536-3599
    void __fastcall DoProcessSFNoResponse();   // golden :4604-4694
    void DoLocalAllProcessLoop();              // golden :4699-4741 (shell; 4 callees gated, see .cpp)

    // AI(W906-SysModWire) 20260720: SetMachineTypeAndSoftwarseVer/
    // CheckSFFormatOnlyHead -- both prerequisites for un-gating uHGemClass.cpp's
    // S1F1/S1F2/S1F13/S1F14/Process_S1F14/S1F16/S1F18/S2F18 (see that file's own
    // un-gating notes); also consumed directly by ProcessReceiceData's tail.
    void __fastcall SetMachineTypeAndSoftwarseVer(AnsiString Mdln, AnsiString SoftVer); // golden :6510-6514 ("Softwarse" misspelling preserved verbatim)
    bool CheckSFFormatOnlyHead(AnsiString ErrStr);   // golden :8679-8693 (no __fastcall, matches golden)

    // golden's private "S,F 主要處理" block (uHGemEquipment.h:292-295) --
    // SelectRsp/DeselectRsp/LinktestRsp are pure WireCodec composition (REAL);
    // ProcessReceiceData is a SPLIT -- the HSMS control-message head is real,
    // the S,F data-message dispatch tail stays gated -- see the .cpp's own
    // comment on ProcessReceiceData for the full rationale.
    void LinktestRsp();          // golden :8751-8765
    void SelectRsp();            // golden :8707-8723
    void DeselectRsp();          // golden :8729-8745
    void ProcessReceiceData();   // golden :8772-8989 (control head REAL; S,F tail GATED, see .cpp)

    // AI(W906-AlarmReportAck) 20260721: THGem-level InitLocalHead/DataItemOut
    // forwarders (golden .h:314-315) -- same one-line-forwarder idiom as
    // SendLocalData() below, targeting THGem's own embedded WireCodec
    // directly (`this->WireCodec` / bare `WireCodec`, NEVER `ActiveWire` --
    // that pointer belongs to HTGem, a DIFFERENT class, see this file's own
    // D1/D2 notes above). This wave's new wire composers below
    // (ReportAcknowledge/ReportLinkAcknowledgeError/
    // EnableDisableEventReportAcknowledgeError/ProcessHostSendReportLinkID)
    // call these bare (implicit `this->`, matching golden). Only the ONE
    // DataItemOut overload those composers actually call (the pointer form)
    // is added -- golden's sibling `DataItemOut(unsigned char, AnsiString)`
    // overload has no in-scope THGem-level caller today (verified by reading
    // every new method's body), so it is not added speculatively.
    void InitLocalHead(int SCode, int FCode, int WBit);        // golden .h:314
    void DataItemOut(int len, unsigned char Type, void *P);    // golden .h:315

    void SendLocalData();   // golden .h:318, uHGemEquipment.cpp:1985-2107 (now real -- forwards to SendLocalDataFrom(WireCodec))
    // AI(W906-uHGemEquipment-BucketC) 20260717: ADDITIVE, not a golden method
    // (D2) -- the REAL golden SendLocalData(1985-2107) body, parameterized
    // over WHICH SecsWireCodec instance to send (THGem's own WireCodec, or --
    // once a future wiring wave installs the identical hook on
    // HSys.MyGem->WireCodec -- HTGem's). See the .cpp definition for the full
    // golden-to-parameterized substitution table.
    void SendLocalDataFrom(SecsWireCodec &wc);

    void __fastcall CheckSFCodeResponse();       // golden :7020-7058
    void __fastcall SaveSECSGEMTextToLog();      // golden :7068-7091
    void InitSTypeStruct();                      // golden :376-387

    // golden's zero-arg signatures (uHGemEquipment.h:263-264) forward to
    // `(WireCodec)` below; the `(SecsWireCodec&)` overloads are ADDITIVE (not
    // golden) so SendLocalDataFrom can trace the INVOKING codec's own buffer
    // (needed once HTGem-originated sends are wired through the same hook).
    void ShowLocalHeadInfo();                    // golden :1378-1405 (forwards to ShowLocalHeadInfo(WireCodec))
    void ShowLocalHeadInfo(SecsWireCodec &wc);        // ADDITIVE (real body)
    void ShowLocalBufferBinaryData();            // golden :1350-1373 (forwards to ShowLocalBufferBinaryData(WireCodec))
    void ShowLocalBufferBinaryData(SecsWireCodec &wc); // ADDITIVE (real body)

    // ==== Bucket C: gated no-op stubs (golden citation each; no in-scope
    // ==== consumer needs a real body -- same idiom as
    // ==== EnableDisableEventReportAcknowledgeError above) ====================
    void DoSpool();                    // golden :4079 -- #if 0 TODO(W906-SECSGEM-spool), needs spool-file surface
    void DoTraceDataResponse(int TR);  // golden :4190 -- #if 0 TODO(W906-SECSGEM-trace), needs TraceData[]/TraceDataResponseTask[]
    void DoUploadFileToHost();         // golden :4591 -- #if 0 TODO(W906-SECSGEM-upload), needs FTP/file-transfer surface
    void DoDownLoadRemoteFile();       // golden :6746 -- #if 0 TODO(W906-SECSGEM-download), needs FTP/file-transfer surface
};

//---------------------------------------------------------------------------
//  SplitStrByTabOnly -- file-scope helper (golden is a bare global function,
//  NOT a THGem method, and is NOT declared in golden's own uHGemEquipment.h
//  either -- golden's .cpp defines it (line 8086) strictly before its one
//  call site inside PasteStringGridAsTabFormat (line 8182), so no forward
//  declaration is needed there. Declared here (a header-level addition
//  beyond golden's own header) purely so this wave's test harness can
//  exercise it directly -- no behavior change, source-compatible.
//---------------------------------------------------------------------------
bool SplitStrByTabOnly(char *str, char *dest, int Max);

//---------------------------------------------------------------------------
//  AI(W906-uHGemEquipment-BucketB) 20260717: HGem -- golden's own global
//  singleton pointer (`extern PACKAGE THGem *HGem;`, golden uHGemEquipment.h's
//  own tail declaration, right before `extern struct HTypeStruct HType;`).
//  ProcessShow's own golden body (uHGemEquipment.cpp:5165-5173) dereferences
//  `HGem->WaitShowString`/`HGem->DB` rather than an implicit `this->` --
//  preserved verbatim (see ProcessShow's own .cpp comment) rather than
//  "simplified" to `this->`, so a caller/test MUST set `HGem = &instance;`
//  before calling ProcessShow (and DoUpdateStatus's own KYEC branch, which
//  ALSO dereferences `HGem->srvGem`/`HGem->clientGem`) for either to behave
//  sensibly -- exactly the same "must be wired externally" category already
//  established by the THGemPanel*/THGemSpeedButton* members above. Defaults
//  NULL here (golden itself never explicitly initializes this file-scope
//  global; C++ static-storage globals zero-init by default regardless, so
//  no deviation-flag needed, unlike THGem's own instance-field members).
//---------------------------------------------------------------------------
extern THGem *HGem;

#endif // uHGemEquipmentH
