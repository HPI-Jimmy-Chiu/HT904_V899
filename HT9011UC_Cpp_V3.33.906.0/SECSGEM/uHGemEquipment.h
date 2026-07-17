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
//    - InitLocalHead/DataItemOut/DataItemInSub/DataItemIn/DataItemInNew/
//      GetDataItemLenAndType(Sub)/StringOut/SendLocalData and
//      SetSVDataPointer/SetECDataPointer/GetECDataValue -- per the project's
//      established design (see SecsWireCodec.h/SecsSvEcRegistration.h's own
//      "INTEGRATE-AGENT WIRING POINT" notes), a future full-THGem wave should
//      hold `SecsWireCodec WireCodec; SecsSvEcRegistration SvEcReg;` as
//      by-value members and forward these methods to them (thin wrappers,
//      matching HTGem's already-proven pattern in SECSGEM/uHGemClass.h/.cpp)
//      rather than re-implementing the logic here. NOT done this wave --
//      this wave's methods (see list above) have ZERO dependency on the wire
//      codec or SV/EC registration, by design (see synthesis note).
//    - EnableDisableEventReportAcknowledgeError: DECLARED and DEFINED below,
//      but as a GATED NO-OP STUB (see its own comment at the definition) --
//      it is THGem::EnableDisableEventReport's one dependency outside this
//      wave's StringGrid-only slice (its golden body is
//      InitLocalHead+DataItemOut+SendLocalData, i.e. the wire-codec family
//      above). Kept as a stub purely so EnableDisableEventReport's own
//      control flow (which unconditionally calls it) is translatable without
//      silently dropping the call.
//---------------------------------------------------------------------------
#ifndef uHGemEquipmentH
#define uHGemEquipmentH

#include "vclcompat/vcl_compat.h"
#include "vclcompat/StringGrid.h"
#include "SECSGEM/SecsEventType.h"   // SECS_EVENT.TotalEvent (array bound in SetCEIDContent)

// This TU does not include aHotPlateSubstrate.h (the other place a global
// `class TList` lives -- see vclcompat/vcl_compat.h's own TList.h precedent
// note) and none of this wave's methods need vclcompat::TList, so bringing
// TStringGrid into the global namespace here is collision-free (grepped: no
// other `class TStringGrid` exists anywhere in this tree).
using vclcompat::TStringGrid;

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

    TStringList *WaitShowString;   // golden :597
    TStringList *LogDataString;    // golden :447

    AnsiString TimeString;   // golden :700
    AnsiString GemClock;     // golden :302
    // NOTE: golden's real 3rd field name is "SystemDate" (uHGemEquipment.h:230),
    // not "SystemDay" -- kept as golden spells it (GetTimeInfo/DecodeDate both
    // reference it by this exact name).
    Word SystemYear, SystemMonth, SystemDate;           // golden :230
    Word SystemHour, SystemMin, SystemSec, SystemMSec;  // golden :231

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

    // golden's tail call inside EnableDisableEventReport; GATED STUB, see
    // definition in the .cpp for why.
    void EnableDisableEventReportAcknowledgeError(unsigned char ErrCode);
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

    // golden uHGemEquipment.h:248 `void DoSeparate();` -- GATED NO-OP STUB
    // (real body is wire-codec/SML, out of THIS wave's scope -- same
    // treatment as EnableDisableEventReportAcknowledgeError above). Exists
    // purely so DoUpdateStatus's own KYEC branch (which unconditionally
    // calls this) is translatable without silently dropping the call.
    void DoSeparate();
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
