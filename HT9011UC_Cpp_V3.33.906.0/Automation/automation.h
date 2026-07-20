// =============================================================================
//  Automation/automation.h  --  TfAutomation: OLP host-command engine
//                                (network wiring + small helpers slice)
//
//  Translation wave: W906-Automation
//  Translator: AI(W906-Automation) 20260716
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/Automation/automation.h
//                 (153 lines, cp950) + automation.cpp (2501 lines, cp950) +
//                 automation.dfm (component/event wiring, decoded for the
//                 exact .dfm defaults / OnXxx assignments cited below).
//
//  SCOPE (per synthesis-reviewed task brief -- "network wiring + small
//  helpers" bucket, ~34 methods + 6 free functions, ~1300-1400 golden lines):
//    ctor / FormDestroy / GetTimeInfo
//    GetEventNum / SetEventResult / GetEventResult / CompareEvent / ClearEvent /
//      GetEventStrResult
//    6 zero-dependency free functions: SplitDataBySoh / GetCRC / AddBlock /
//      IsStartWord / crc16 / charcmp
//    CommandProcess / SendClient / SendServer
//    OLPServerClientConnect / Disconnect / Error / Read
//    OLPClientRead / Connect / Disconnect / Error / Connecting
//    tmrOLPTimer
//    DoCommandBuffer
//    ShowCharHex / ShowRecord / SaveRecord / ShowMSG
//    SendOLPClient / SckSendAlarmCode
//    WriteTCPDataToTextFile
//    btnConnectClick / btnDisconnectClick / btEventReportClick /
//      btAlarmReportClick / btTestResultClick / MemoDblClick
//    GetHTMLJSONDatas / GetMachineStatus / GetWorkOrder / GetMainTemp /
//      GetTemperatures
//
//  TRANSLATED (W906-AutoPB 20260720): the 3 methods below were GATED (safe
//  no-op stubs) through the W906-Automation wave; this wave (W906-AutoPB, per
//  DESIGN_automation_ProcessBuffer.md) replaced all three with real, golden-
//  faithful bodies. Kept the original scope note below (now historical) for
//  context on WHY these three were split into their own wave in the first
//  place -- see the .cpp's own per-function AI(W906-AutoPB) comments for the
//  full per-branch translation, the golden bug/quirk preservation list (P1-
//  P18), and the 3 PORT-ONLY UB guards (D1-D3), all cross-referenced against
//  the design doc.
//    ProcessBuffer   (golden :951-1978,  ~1030 lines) -- the OLP command-
//      dispatch ladder (~130 branch string literals: PP_LOAD_INQUIRE,
//      HTMLJSON_INQUIRE, ACT_TEMP_INQUIRE, TRAY_INFO_INQUIRE, ... dozens more).
//    ProcessBuffer1  (golden :1980-2044, ~66 lines)  -- PP_DL_REQUEST binary
//      file-receive sibling of the same ladder (writes a .zip via TCPstr).
//    SendReportRequest (golden :2046-2130, ~86 lines) -- host EVENT/REPORT
//      sender (ReportID 0001-0007 branches into GetProductivity/GetSortingCount/
//      GetSocketCount/GetHeadCount etc).
//  NOTE (main-loop adjudication, W906-AutoPB): the once-planned 2-line
//  btnConnectClick NULL/0-fidelity fix (design doc's D5 / §3.1-e) was pulled
//  OUT of this wave for blame-isolation (to land as its own independent
//  commit later) -- btnConnectClick's `!=0` comparisons are UNCHANGED by this
//  wave. The NEW code these three methods introduce (ProcessBuffer's own
//  entry guard + the two INITIATE_REQUEST edinputIP/edinputport comparisons)
//  DOES use the AnsiString(0) fidelity form -- only the pre-existing shipped
//  btnConnectClick lines were left alone.
//
//  NOT DECLARED AT ALL this wave (genuinely dead/unreachable from every
//  in-scope method -- omitted rather than faked, matching this project's
//  established AGV_E84.h/AGV_PortScan.h precedent of leaving out handlers
//  nothing here calls):
//    ShowCharHexATK / AmkorSendMessage / NMUDP1DataReceived / btUDPSendClick /
//      btUDPTempClick / btUDPMapClick   -- the "NMUDP1/Amkor-UDP block" (6
//      methods): needs a brand-new vclcompat UDP shim design (no TNMUDP
//      precedent anywhere in this tree), low customer relevance (ATK/Amkor +
//      Qualcomm-gated in golden: `if(CosFunction.bAmkorFunction ||
//      CUSTOMER_CODE==CC_QUALCOMM)`), indefinitely deferred per task brief.
//      The NMUDP1 *widget itself* is likewise omitted (grepped: zero
//      references anywhere in golden automation.cpp outside this dead block).
//    edinputportMouseDown -- needs myQwertyKeyBoard.h/.cpp (virtual on-screen
//      keyboard popup), indefinitely deferred per task brief.
//  Also omitted (zero references anywhere in golden automation.cpp, grep-
//  verified against the WHOLE 2501-line file, not just the in-scope subset --
//  pure design-time/layout widgets no C++ body ever dereferences):
//    pgcOLP (TPageControl), TabSheet20/TabSheet6/TabSheet22 (TTabSheet),
//    Panel1/lbIP/lbPORT (TPanel/TLabel container for edinputIP/edinputport),
//    btnDisconnect/btEventReport/btAlarmReport/btTestResult widget OBJECTS
//      (their click-handler METHODS are very much in scope and declared below;
//      only the backing TButton* field itself is unreferenced anywhere).
//
//  GLOBAL-NAME COLLISION AVOIDED (read before touching `fAutomation` anywhere):
//  golden declares `extern PACKAGE TfAutomation *fAutomation;` (automation.h:
//  151). That EXACT global name is ALREADY CLAIMED by atester_shims.h's
//  pre-existing minimal offline stand-in `TfAutomationShim *fAutomation`
//  (atester_shims.cpp), consumed TODAY by atester.cpp/atester_32Site.cpp
//  (DoCommandBuffer/GetEventNum/GetEventResult/GetEventStrResult/ClearEvent/
//  TestMode/sATKSendMessage[Count]). Declaring a second global `TfAutomation
//  *fAutomation` here would be a duplicate-symbol LINK ERROR the moment both
//  translation units land in the same binary. This translation therefore
//  names its own (REAL, fully-faithful) singleton `fAutomationEngine` instead
//  of golden's bare `fAutomation` -- see automation.cpp's own extern decl +
//  definition. TfAutomationShim's entire method surface (DoCommandBuffer/
//  GetEventNum/GetEventResult/GetEventStrResult/ClearEvent/TestMode/
//  sATKSendMessage/sATKSendMessageCount) is now a STRICT SUBSET of this real
//  class's surface -- a future integrate wave can retarget atester_shims.h's
//  `fAutomation` extern onto `fAutomationEngine` (or delete TfAutomationShim
//  outright) once CommandBuffer construction / OLP socket wiring are judged
//  safe to share with the tester-engine call sites. NOT done in this wave
//  (atester_shims.* is explicitly off-limits to this front's write scope).
//
//  WIDGET STAND-INS: TfAutomation's own __published components have no VCL
//  home, so each gets a minimal non-VCL stand-in struct (same idiom as
//  FormsFacade.h's TfMainPanel/TfMainCheckBox/TfMainMemo family, but declared
//  locally here since these are TfAutomation's OWN components, not another
//  form's). ONE exception is genuinely a REAL translation, not a stub:
//  Memo/MemoChar/MemoCode's ->Lines is modeled as a real vclcompat TStringList*
//  (not a fake counter) -- golden TMemo->Lines IS just a TStrings collection
//  with Add/Count/Clear/SaveToFile, and vclcompat::TStringList already
//  implements all four for real, so ShowMSG/ShowCharHex/SaveRecord/
//  MemoDblClick translate as genuine, faithful logic here, not gated no-ops.
//
//  EVENT WIRING: golden automation.dfm wires OLPServer's 4 events and
//  OLPClient's 5 events to the exact TfAutomation methods below (grepped from
//  the decoded .dfm, cited per-event in automation.cpp's ctor). This shim has
//  no VCL streaming, so the ctor wires each explicitly via a `[this]` lambda
//  forwarding to the real method -- same idiom already established by
//  MyPLC/ModbusTCPClient.cpp's ctor. tmrOLP's OnTimer is NOT wired (no
//  background ticking thread exists in this shim; tests/future callers invoke
//  tmrOLPTimer(...) directly) -- and Button OnClick handlers are likewise not
//  wired to anything (no simulated-click dispatch exists); each click handler
//  is simply a normal callable public method.
//
//  Big5: automation.cpp's Chinese comments were decoded via cp950 (python
//  `open(path, encoding='cp950')`) before translation; reproduced here/there
//  as correct UTF-8. Final gate: ZERO U+FFFD bytes (verified before hand-off).
// =============================================================================
#ifndef automationH
#define automationH

#include "vclcompat/vcl_compat.h"     // AnsiString, __fastcall no-op (unused, dropped per project convention)
#include "vclcompat/ServerSocket.h"   // TServerSocket/TServerWinSocket (OLPServer) -- also pulls in ClientSocket.h
#include "vclcompat/ClientSocket.h"   // TClientSocket/TCustomWinSocket (OLPClient) -- explicit include for clarity
#include "vclcompat/TStringList.h"    // TStringList (CommandClientReplyBuffer, Memo/MemoChar/MemoCode->Lines)
#include "vclcompat/TList.h"          // vclcompat::TList (CommandBuffer) -- fully-qualified use only, see KNOWLEDGE.md
                                      // TList naming gotcha (aHotPlateSubstrate.h has an unrelated global class TList)

// -----------------------------------------------------------------------
//  MyProcessBuffer / ProcessBuffer  (golden automation.h:16-25)
//
//  NOTE (preserved golden quirk): the typedef alias name is literally
//  `ProcessBuffer` -- the SAME bare identifier as TfAutomation's own member
//  method `void ProcessBuffer(AnsiString, int)` declared further down. These
//  are two DISTINCT entities in different scopes (a file-scope type alias vs.
//  a class member function) so this compiles without conflict in standard
//  C++ (member-name lookup inside TfAutomation's own bodies would shadow the
//  global alias, but nothing in this file ever uses `ProcessBuffer` as a type
//  name -- golden itself never does either; `P1`/`CommandBuffer` items are
//  always typed `MyProcessBuffer*`, never via this alias). Kept verbatim for
//  structural fidelity; flagged here so nobody "cleans up" the apparent
//  redundancy without realizing it is a harmless, faithfully-preserved quirk.
// -----------------------------------------------------------------------
typedef class MyProcessBuffer
{
public:
    AnsiString Command;
    AnsiString SubCommand;
    AnsiString AlarmID;
    AnsiString AlarmMessage;
    AnsiString TimeString;
    int        ErrType;     // golden: no ctor, so this is left uninitialized on
                             // `new MyProcessBuffer()` exactly as golden does --
                             // harmless because DoCommandBuffer (the only
                             // constructor site in the in-scope method set)
                             // always assigns ErrType explicitly right after.
} ProcessBuffer;

// ---------------------------------------------------------------------------
//  Widget stand-ins for TfAutomation's own __published components (golden
//  automation.h:30-58 / automation.dfm). Minimal: only the property each is
//  actually dereferenced for anywhere in golden automation.cpp gets a field.
// ---------------------------------------------------------------------------

// TGroupBox/TButton ->Visible (gbAutomation; btUDPSend/btUDPMap/btUDPTemp --
// ctor-only touch, their OnClick handlers are the deferred NMUDP1/Amkor block
// and are NOT declared, see file-head note).
struct TfAutomationVisibleFlag
{
    bool Visible;
    TfAutomationVisibleFlag() : Visible(true) {}
};

// TSpeedButton/TButton ->Enabled (OnLine, OnLine2; btnConnect).
// golden .dfm: OnLine/OnLine2/btnConnect all design-time Enabled=False.
struct TfAutomationEnableFlag
{
    bool Enabled;
    TfAutomationEnableFlag() : Enabled(false) {}
};

// TComboBox/TEdit ->Text (cbbOLPCommand; edinputIP; edinputport). Default text
// is set explicitly per-instance in the ctor (golden .dfm per-widget Text=)
// rather than baked into this shared struct's own ctor.
struct TfAutomationTextField
{
    AnsiString Text;
};

// TCheckBox ->Checked (chkViewComm). golden .dfm: no explicit Checked= line,
// i.e. the ordinary VCL design-time default (unchecked).
struct TfAutomationCheckFlag
{
    bool Checked;
    TfAutomationCheckFlag() : Checked(false) {}
};

// TTimer ->Enabled (tmrOLP). golden .dfm: Enabled=False, Interval=1 (Interval
// is never read by any translated body -- omitted, matching ClientSocket.h/
// ServerSocket.h's own "NOT implemented: design-time-only property" posture).
struct TfAutomationTimer
{
    bool Enabled;
    TfAutomationTimer() : Enabled(false) {}
};

// TMemo ->Lines (Memo, MemoChar, MemoCode). REAL, not a stub: golden TMemo's
// Lines property is just a TStrings collection, and vclcompat::TStringList
// already implements Add/Count/Clear/SaveToFile faithfully -- so this is a
// genuine translation, not a gate. ->Clear() forwards to Lines->Clear() (the
// golden TMemo::Clear() convenience method MemoDblClick calls on the memo
// itself, distinct from ->Lines->Clear()).
struct TfAutomationMemo
{
    TStringList *Lines;
    TfAutomationMemo() { Lines = new TStringList(); }
    void Clear() { Lines->Clear(); }
};

// ---------------------------------------------------------------------------
//  TfAutomation
// ---------------------------------------------------------------------------
class TfAutomation
{
public:
    // ---- __published component stand-ins (golden automation.h:30-58) ------
    TfAutomationMemo        *Memo;
    TfAutomationMemo        *MemoChar;
    TfAutomationMemo        *MemoCode;
    TServerSocket           *OLPServer;
    TfAutomationTimer       *tmrOLP;
    TClientSocket           *OLPClient;
    TfAutomationVisibleFlag *btUDPSend;
    TfAutomationVisibleFlag *gbAutomation;
    TfAutomationEnableFlag  *OnLine;
    TfAutomationEnableFlag  *OnLine2;
    TfAutomationTextField   *cbbOLPCommand;
    TfAutomationCheckFlag   *chkViewComm;
    TfAutomationEnableFlag  *btnConnect;
    TfAutomationVisibleFlag *btUDPMap;
    TfAutomationVisibleFlag *btUDPTemp;
    TfAutomationTextField   *edinputIP;
    TfAutomationTextField   *edinputport;

    // ---- __published event handlers (golden automation.h:59-93) -----------
    void OLPServerClientConnect(TObject *Sender, TCustomWinSocket *Socket);
    void OLPServerClientDisconnect(TObject *Sender, TCustomWinSocket *Socket);
    void OLPServerClientError(TObject *Sender, TCustomWinSocket *Socket,
                               TErrorEvent ErrorEvent, int &ErrorCode);
    void OLPServerClientRead(TObject *Sender, TCustomWinSocket *Socket);
    void tmrOLPTimer(TObject *Sender);
    void btnConnectClick(TObject *Sender);
    void btnDisconnectClick(TObject *Sender);
    void btEventReportClick(TObject *Sender);
    void btAlarmReportClick(TObject *Sender);
    void MemoDblClick(TObject *Sender);
    void btTestResultClick(TObject *Sender);
    void OLPClientRead(TObject *Sender, TCustomWinSocket *Socket);
    void OLPClientConnect(TObject *Sender, TCustomWinSocket *Socket);
    void OLPClientDisconnect(TObject *Sender, TCustomWinSocket *Socket);
    void OLPClientError(TObject *Sender, TCustomWinSocket *Socket,
                         TErrorEvent ErrorEvent, int &ErrorCode);
    void OLPClientConnecting(TObject *Sender, TCustomWinSocket *Socket);
    void FormDestroy(TObject *Sender);
    // NMUDP1DataReceived / btUDPSendClick / btUDPTempClick / btUDPMapClick /
    // edinputportMouseDown: NOT declared -- see file-head "NOT DECLARED"
    // note (indefinitely deferred, zero in-scope callers).

private:
    // golden automation.h:94-101 (private:)
    Word aSystemHour, aSystemMin, aSystemSec, aSystemMSec;
    Word aSystemYear, aSystemMonth, aSystemDate;
    bool SendInvalid;
    // ShowCharHexATK: NOT declared (part of the omitted NMUDP1/Amkor block).
    bool OLPClientConnected;
    void ShowMSG(AnsiString asStr);
    bool brecordmsgLock;

public:
    // golden automation.h:102-149 (public:, continued)
    TfAutomation(TComponent *Owner);
    void ShowCharHex(AnsiString S);
    void ShowRecord(bool bRead, AnsiString S, int iHandle);
    // ---- REAL as of W906-AutoPB (see file-head note) -----------------------
    void ProcessBuffer1();
    void ProcessBuffer(AnsiString Buffer, int iHandle);
    // -------------------------------------------------------------------
    void WriteTCPDataToTextFile();
    void SendClient(AnsiString S, AnsiString S2, int iHandle);
    void SendServer(AnsiString S, AnsiString S2);
    // ---- REAL as of W906-AutoPB (see file-head note) -----------------------
    void SendReportRequest(AnsiString ReportID, bool Standard = true);
    // -------------------------------------------------------------------
    void CommandProcess(AnsiString CMD, AnsiString V_TOTAL, AnsiString *Data,
                         bool Standard = true, int iHandle = 0);              // 被動回傳 (passive reply)
    void DoCommandBuffer(AnsiString Command, AnsiString SubCommand = "",
                         AnsiString AlarmMessage = "", int ErrType = 0,
                         AnsiString AlarmID = "");                            // 主動回傳 (active/unsolicited report)
    AnsiString GetTimeInfo();
    bool bReceive;
    bool bReceive2;
    int  iSocketHandle;              // Sam 20200727 : 修正連線問題 (connection-identity fix)
    bool bReceiveHasData;            // 2011.09.05 , Joye , OLP
    vclcompat::TList *CommandBuffer;
    TStringList      *CommandClientReplyBuffer;   // Sam 20200813 : Client 收到資料回傳若遇到斷線將資料丟到 Buffer
    int TestMode;

    bool GetEventNum(int &num, AnsiString cmdstr, AnsiString numstr);
    bool SetEventResult(int num, bool def, AnsiString cmdstr, AnsiString numstr);
    bool GetEventResult(int num);
    int  GetEventStrResult(int num);           // Brian,20181025
    void CompareEvent(AnsiString str);
    void ClearEvent(int num);
    bool bOneCycle;
    bool bCleanOut;
    int  iClientReConnect;            // Sam 20240226 : Client 重連次數 (client reconnect count)
    // AmkorSendMessage / sATKSend* buffers: NOT declared -- part of the
    // omitted NMUDP1/Amkor block (see file-head "NOT DECLARED" note). golden
    // `MyProcessBuffer *P1;` (below) and the ATK buffers are interleaved in
    // the golden header; P1 IS in scope (DoCommandBuffer uses it), the ATK
    // buffers are not.
    MyProcessBuffer *P1;              // 2012-10-26 Dell
    void SckSendAlarmCode(AnsiString Text);    // ChungHung 20150518 add for SCK Send JamCode By TcpIp
    void SaveRecord();                          // Sam 20200826 : 改為 Public //Steven 20141229 : OLP的Log要存檔
    void SendOLPClient(AnsiString Text);
    // HTML JSON
    AnsiString GetHTMLJSONDatas();
    AnsiString GetMachineStatus();
    AnsiString GetWorkOrder();
    AnsiString GetMainTemp();
    void GetTemperatures(double temps[10]);
};

// AI(W906-Automation) 20260716: see file-head "GLOBAL-NAME COLLISION AVOIDED"
// note -- golden's bare `fAutomation` name is already claimed by
// atester_shims.h's TfAutomationShim. This is the REAL engine's singleton,
// under a distinct name.
extern TfAutomation *fAutomationEngine;

// ---------------------------------------------------------------------------
//  File-scope free functions (golden Automation/automation.cpp, already
//  plain functions in golden -- not TfAutomation members).
// ---------------------------------------------------------------------------
AnsiString SplitDataBySoh(AnsiString &Buffer);        // golden :212-223
AnsiString GetCRC(AnsiString &Buffer);                // golden :225-231
AnsiString AddBlock(AnsiString S);                    // golden :233-238
bool       IsStartWord(AnsiString &Buffer, AnsiString w);  // golden :240-248
AnsiString crc16(char *ip_str, int n);                // golden :250-275
bool       charcmp(char *buf1, char *buf2, int size); // golden :2279-2288 (ChungHung 20120413 add)

// ---------------------------------------------------------------------------
//  File-scope globals (golden Automation/automation.cpp:20-42, non-static /
//  external linkage in golden -- preserved as such here; grep-verified against
//  the whole currently-translated tree that none of these bare names are
//  already claimed as GLOBAL variables elsewhere -- see automation.cpp's own
//  file-head note for the exact collision check performed).
// ---------------------------------------------------------------------------
extern AnsiString HandlerType;    // golden :36 -- set once in ctor via auto9045.h's GetHandlerType()
extern AnsiString aDataPath;      // golden :37 -- set once in ctor via auto9045.h's GetDataPath()

#endif // automationH
