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
//  for golden line citations. Still explicitly OUT OF SCOPE (see below):
//  clientGemRead/ProcessSocketReceiveData, DoConnect/DoSelect/DoSeparate,
//  Timer1Timer, DoProcessSFNoResponse, DoLocalAllProcessLoop, FormCreate/
//  InitialHGem/SaveSystemDefault/DoUpdateStatus/ProcessShow/FormClose/
//  ManualCreatergRoleClick/FormShow (need the ~15 widget stand-ins + the wire
//  codec/SV-EC embedding below) -- a future wave's job.
//
//  MEMBERS DELIBERATELY NOT PRESENT YET (out of scope; a future wave adds
//  them to this SAME class body, does not redefine it):
//    - The wire-codec-dependent connection functions: DoConnect/DoSelect/
//      DoSeparate (need SecsWireCodec embedded as a member first),
//      clientGemRead/ProcessSocketReceiveData (need TMemoryStream/
//      TFixedCriticalSection shims, not yet designed), DoProcessSFNoResponse
//      (needs a TCriticalSection shim), Timer1Timer (its body calls into all
//      of the above) -- see each's golden citation at uHGemEquipment.cpp's
//      clientGemRead/DoConnect/Timer1Timer.
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
    // for real by THGem::SaveSystemDefault/InitialHGem (both OUT OF SCOPE
    // this wave -- see file-head note). Defaults to "" here; a caller/test
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

#endif // uHGemEquipmentH
