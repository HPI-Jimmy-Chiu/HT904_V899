//---------------------------------------------------------------------------
//  Interface/TesterTCP_Socket.h -- bounded socket-management subset of the
//    golden TfTesterTCP form (Interface/TesterTCP.cpp/.h), a Tester
//    "OS/Open-Short" TCP/IP channel used only by CUSTOMER_CODE==CC_JSCC_OS
//    (Steven 20230116 "OS測試機傳送工作檔名"; wei 20211027 "open short TCP/IP").
//
//  AI(W5-Final-TesterTCPSocket) 20260711: new file. Deliberately a DIFFERENT
//  class/file from Interface/InterfaceSYS.{h,cpp} (the WM_COPYDATA GPIB/ESD/
//  AutoUpdate/EventLog IPC bridge, a W5 Wave-1 unit) -- do not confuse the
//  two; they share no state and are unrelated golden modules that both
//  happen to live under Interface/.
//
//  SCOPE (verified directly against golden by grep + full read, this unit's
//  task brief's "~166-345" line estimate re-checked against the actual file):
//    ClientSocket_TCPIPConnect     golden TesterTCP.cpp:166-174
//    ClientSocket_TCPIPDisconnect  golden TesterTCP.cpp:176-183
//    TimerTCPIPConnectTimer        golden TesterTCP.cpp:185-239
//    SendTCPIPCommand              golden TesterTCP.cpp:241-274
//    AddTCPIPCommunicationLog      golden TesterTCP.cpp:276-300 (SendTCPIPCommand's
//                                  own dependency -- not named in the task brief's
//                                  6-function list, but required to make those 6
//                                  functions link; included as part of this bounded
//                                  subset, not scope creep)
//    btTCPIP_TriggerClick          golden TesterTCP.cpp:302-307
//    btTCPIP_ConnectClick          golden TesterTCP.cpp:309-313
//    btTCPIP_DisconnectClick       golden TesterTCP.cpp:315-320
//    ClientSocket_TCPIPRead        golden TesterTCP.cpp:322-345
//    ClientSocket_TCPIPError       golden TesterTCP.cpp:615-633 (named in the task
//                                  brief's function list, but NOT inside the
//                                  166-345 line estimate -- it lives later in the
//                                  file, right after SimulateBin(); confirmed the
//                                  only other ClientSocket_TCPIP* handler in the
//                                  whole file, included per the brief's explicit
//                                  naming)
//    TimerProcessTCPDataTimer      golden TesterTCP.cpp:349-552 (the protocol DECODE
//                                  dispatcher: WORKFILE_OK/FAIL, BARCODE?, ECHOCODE:,
//                                  Test Arm?, TempArm?, BINON:, ECHOOK, GETOSSETUP)
//                                  -- AI(W906-TesterTCPTimer) 20260720: translated
//                                  this wave; see DESIGN_TesterTCP_TimerProcessTCPDataTimer.md.
//    SimulateBin                   golden TesterTCP.cpp:554-613 -- AI(W906-TesterTCPTimer)
//                                  20260720: translated this wave (same design doc).
//
//  NOT attempted (per task brief, confirmed genuinely VCL-widget-bound / a
//  different concern on a full read):
//    - The ctor / FormShow / FormDestroy / rgUnloaderClick / btnSaveClick
//      (per-tray TMemo/TGroupBox[] construction, OS-only page-index wiring).
//    - CopyOSTestResult / PlaceOSTestResultToTray / ProcessOSPrint /
//      ProcessOSTrayData / CopyRecipeToTester / CopyRecipeFromTester
//      (RichEdit/TMemo OS report-generation family).
//    - cbSiteOn[32] (golden TCheckBox* h:250; ctor sets it but neither
//      TimerProcessTCPDataTimer nor SimulateBin ever reads/writes it --
//      left for a real UI wave).
//
//  [RESOLVED -- AI(W906-TesterTCPTimer) 20260720] TimerProcessTCPDataTimer +
//  SimulateBin (the two items this file's original banner flagged above as
//  NOT attempted, pending "a future wave") are now translated -- see this
//  file's SCOPE list above and TesterTCP_Socket.cpp. The original banner text
//  here had two small inaccuracies, corrected by that same design doc's own
//  recon: (1) the golden line range is :349-552 (timer) + :554-613
//  (SimulateBin), not one contiguous ":349-613"; (2) "LOTNUMBER" was an
//  erroneous mention -- LOTNUMBER is an OUTBOUND command built elsewhere
//  (uLotInfo.cpp), it is not one of this timer's inbound dispatch branches.
//  The 5 fMain-side members this DISCREPANCY note (below) said were used
//  "ONLY inside TimerProcessTCPDataTimer, which this unit does NOT attempt"
//  are consumed for real now that the timer itself is translated (definition
//  B of the design doc: tTestResult/tBarCodeList/SVID1190_OSSetup/
//  WritePERSITETemperature added to FormsFacade.h/.cpp this wave;
//  cbSetupFileName was already added by the 2026-07-16 W906-Automation wave).
//
//  DISCREPANCY vs. this unit's task brief (golden/actual-code wins, per the
//  ground rules): the brief listed "~7 new FormsFacade entries" needed --
//  fLotInfo->labTCPIPStatus (Caption/Color) + fMain->cbSetupFileName/
//  tTestResult/tBarCodeList/SVID1190_OSSetup/WritePERSITETemperature. A full
//  read + grep of the ACTUAL golden file shows the 5 fMain-side members are
//  used ONLY inside TimerProcessTCPDataTimer (golden :385/392-394/454/466/
//  491/514/585/597-599/797), which this unit does NOT attempt (see above --
//  confirmed genuinely out of scope, widget-bound). They are therefore NOT
//  needed by anything in this file. In their place, this file's ACTUAL
//  dependency is fLotInfo->mmTesterLog (golden uLotInfo.h TMemo*, referenced
//  at TesterTCP.cpp:290/294 inside AddTCPIPCommunicationLog, which IS in
//  scope) -- a real FormsFacade gap the brief did not flag. See the exact
//  wiring asks in this unit's translate-report "shared-file wiring" section.
//
//  [RESOLVED -- AI(W5-Final-Integrate) 20260711] INTEGRATE added exactly the
//  2 real members this file needs (TfLotInfo::labTCPIPStatus + ::mmTesterLog,
//  FormsFacade.h/.cpp) -- NOT the 5 fMain members above (confirmed unneeded,
//  per this DISCREPANCY note) -- and lifted all 5 `#if 0 // TODO(FormsFacade-
//  wiring)` gates in TesterTCP_Socket.cpp to real calls.
//---------------------------------------------------------------------------
#ifndef TesterTCP_SocketH
#define TesterTCP_SocketH

#include "vclcompat/vcl_compat.h"     // AnsiString / TStringList / StringReplace+TReplaceFlags
#include "vclcompat/ClientSocket.h"   // Scktcomp::TClientSocket / TCustomWinSocket / TErrorEvent / TObject

#include "cmydef.h"                   // InitialOK, OFF_LINE, SystemYear/Month/Date/Hour/Min/Sec/MSec
#include "cprod.h"                    // TestIF_File.asTester_Address / .iTester_Port
#include "cpublic.h"                  // GetTimeInfo()
#include "common.h"                   // asTestTCPIPLogPath
#include "canary_support.h"           // LastSet.iTester, ShowMyMessage, RecordProcess (hard-boundary file: READ-ONLY, not edited)
#include "Public/WinSocketErrorCode.h"// GetErrorMsg
#include "FormsFacade.h"              // fLotInfo (TfLotInfo) -- see gated members below

//---------------------------------------------------------------------------
//  AI(W5-Final-TesterTCPSocket) 20260711: JUDGMENT CALL -- local widget
//  stand-ins for the two golden TfTesterTCP widgets this bounded subset
//  needs, that this unit's own not-yet-built form does not otherwise have a
//  home for.  Both are Count-only mirrors of FormsFacade's own established
//  TfMainMemo/TfMainMemoLines shape (golden main.h shuttle-log memos), which
//  is itself already Count-only (no per-line text storage) -- so this is not
//  a narrower stand-in than an already-accepted project precedent, just a
//  LOCAL (non-FormsFacade) copy of the same shape, since this unit's write
//  scope is Interface/TesterTCP_Socket.{h,cpp} only and it must not add to
//  FormsFacade.h itself (hard-boundary shared file; the INTEGRATE agent adds
//  real facade members instead -- see the translate-report's wiring section).
//  When TfTesterTCP is eventually translated for real (W7 UI), its actual
//  mmTCPIPCommLog should become a real TMemo/FormsFacade type, superseding
//  this local stand-in (call sites are trivial to repoint: ->Lines->Add /
//  ->Lines->Count / ->Clear()).
//---------------------------------------------------------------------------
struct TesterTCPSocket_MemoLines
{
    int Count;
    void Add(const AnsiString& /*s*/) { ++Count; }
    TesterTCPSocket_MemoLines() : Count(0) {}
};
struct TesterTCPSocket_Memo
{
    TesterTCPSocket_MemoLines Lines;
    void Clear() { Lines.Count = 0; }
};

//---------------------------------------------------------------------------
//  AI(W906-TesterTCPTimer) 20260720: local widget stand-ins for the 3 golden
//  TfTesterTCP per-site widget arrays TimerProcessTCPDataTimer/SimulateBin
//  deref (golden TesterTCP.h:248-251, ctor TesterTCP.cpp:49-59). Same
//  "LOCAL (non-FormsFacade) copy" judgment call as TesterTCPSocket_Memo above
//  -- this unit's own not-yet-built form has no other home for them, and this
//  unit's write scope is Interface/TesterTCP_Socket.{h,cpp} only. golden
//  `cbSiteOn[32]` (TCheckBox*) is NOT stood in here -- ctor sets it but
//  neither TimerProcessTCPDataTimer nor SimulateBin ever reads/writes it
//  (verified full read); left for a real UI wave.
//---------------------------------------------------------------------------
//  AI(W906-W7-F2) 20260729: TesterTCPSocket_Combo / _Panel / _Label RETIRED --
//  vclcompat/Controls.h (reached here via this file's existing FormsFacade.h include)
//  now owns the unified stock-widget stand-ins (plan D4), so the three arrays below
//  name TComboBox / TPanel / TLabel directly.  Golden classes re-read for this change:
//  golden Interface/TesterTCP.h:248 `TComboBox *cbSimulateBin[32];`, :249 `TPanel
//  *plSite[32];`, :251 `TLabel *labOcr[32];`.  Zero behaviour change: identical
//  defaults (ItemIndex 0, Caption ""), and the arrays are plain member arrays of the
//  single TesterTCPSocket object -- never copied by value (TesterTCPSocketState's copy
//  ctor and operator= are private and undefined) and never aggregate-initialised, so
//  the unified types' vtable is inert here.  The ctor's explicit plSite[i].Caption="--"
//  / labOcr[i].Caption="" loop (golden TesterTCP.cpp:49-59) is unchanged.

//---------------------------------------------------------------------------
//  TesterTCPSocketState -- the (single) golden TfTesterTCP instance's data
//  members this bounded subset touches. Golden has exactly ONE TfTesterTCP
//  instance ever (extern PACKAGE TfTesterTCP *fTesterTCP;), so a single
//  extern struct instance below (TesterTCPSocket) is the faithful shape --
//  matches this project's existing singleton-engine-state precedent (e.g.
//  Interface/InterfaceSYS.cpp's `GPIBInterface_Data *GPIBSend=new ...`).
//---------------------------------------------------------------------------
struct TesterTCPSocketState
{
    bool       bEnableTCPIPChannelConnect; // golden TesterTCP.h:236
    bool       bConnectOK;                 // golden TesterTCP.h:237
    bool       bConnect;                   // golden TesterTCP.h:238
    bool       bTCPError;                  // golden TesterTCP.h:253
    AnsiString ErrorMessage;                // golden TesterTCP.h:252
    AnsiString sTCPIPRecevieData;           // golden TesterTCP.h:231 (private in golden;
                                             // this subset only ever writes it, matching
                                             // golden -- no in-scope reader exists here)
    // AI(W5-Final-TesterTCPSocket) 20260711: golden `Tag` (TesterTCP.cpp:343
    // `AddTCPIPCommunicationLog(Tag, Log);`) is TfTesterTCP's OWN inherited
    // VCL TComponent::Tag (via TForm), NOT the event's Socket->Tag (a
    // DIFFERENT Tag added to Scktcomp::TClientSocket by the ClientSocketExt
    // unit for an unrelated purpose -- BarCode's per-socket discriminator).
    // Golden's ctor never assigns fTesterTCP->Tag, so it stays at the VCL
    // TComponent default (0) for the lifetime of the (singleton) form.
    int        Tag;                         // golden: inherited TComponent::Tag, default 0

    TStringList* SocketTCPIPReceiveList;    // golden TesterTCP.h:235 (TStringList*, ctor-allocated)

    // -- JUDGMENT CALL: local widget stand-ins, see the struct comment above --
    TesterTCPSocket_Memo mmTCPIPCommLog;     // golden TesterTCP.h (TMemo* __published)
    // Stands in for golden's TTimer* TimerTCPIPConnect->Enabled (golden
    // TesterTCP.h __published; set by btTCPIP_ConnectClick/DisconnectClick,
    // read by nothing IN this bounded subset -- TimerTCPIPConnectTimer itself
    // is the timer's OWN fired handler, it never reads its own Enabled flag).
    // A future real-timer-driven main loop (W7 UI) should read this flag to
    // decide whether to fire TesterTCPSocket_TimerTCPIPConnectTimer() at all,
    // exactly as a real VCL TTimer would gate its own OnTimer firing.
    bool       bTimerTCPIPConnectEnabled;

    // -- W906-TesterTCPTimer ADD (20260720) ------------------------------------
    bool SimulateStart;                       // golden TesterTCP.h:243 (ctor :64 false)
                                              // GOLDEN QUIRK: full golden tree has NO true-setter
                                              // (dead trigger) -- preserved faithfully; tests
                                              // drive it directly (public bool, same golden shape).
    bool bTimerProcessTCPDataEnabled;         // golden .dfm:2947-2953 TTimer Enabled=False, Interval=1
                                              // state mirror (same idiom as bTimerTCPIPConnectEnabled
                                              // above); real gate lives in untranslated
                                              // cTesterIF.cpp:580-617 / main.cpp:10879/28726.
    // -- widget stand-ins (golden TfTesterTCP own arrays, h:248-251, ctor :49-59) --
    TComboBox cbSimulateBin[32];              // golden TesterTCP.h:248 TComboBox* ->ItemIndex (ctor 0)
    TPanel    plSite[32];                     // golden TesterTCP.h:249 TPanel*    ->Caption  (ctor "--")
    TLabel    labOcr[32];                     // golden TesterTCP.h:251 TLabel*    ->Caption  (ctor "")

    TesterTCPSocketState();
    ~TesterTCPSocketState();

private:
    TesterTCPSocketState(const TesterTCPSocketState&);
    TesterTCPSocketState& operator=(const TesterTCPSocketState&);
};

// The one-and-only instance (mirrors golden's singleton `fTesterTCP`).
extern TesterTCPSocketState TesterTCPSocket;

// The one-and-only ClientSocket_TCPIP component this subset owns (golden
// TesterTCP.h:20-ish `TClientSocket *ClientSocket_TCPIP;`, .dfm-constructed
// in the real form; owned here directly since this unit does not translate
// the whole ctor). Defaults to vclcompat ClientSocket's own Sim mode (see
// vclcompat/ClientSocket.h) -- safe/offline/deterministic until a
// composition root explicitly calls ->SetSimMode(false).
extern Scktcomp::TClientSocket* TesterTCPSocket_ClientSocket;

// AI(W5-Final-TesterTCPSocket) 20260711: JUDGMENT CALL -- reproduces the
// ONE piece of the golden .dfm's declarative event-handler wiring this
// subset depends on (OnConnect=ClientSocket_TCPIPConnect / OnDisconnect=.../
// OnError=.../OnRead=...). A .dfm binding has no C++ source line to
// mechanically translate; this explicit init function is the faithful
// substitute. Idempotent (safe to call more than once). A future real
// TfTesterTCP ctor translation should call this once (or inline the same 4
// assignments) instead of re-wiring independently.
void TesterTCPSocket_Init();

//---------------------------------------------------------------------------
//  Event handlers -- assignable directly to TClientSocket::OnConnect/
//  OnDisconnect/OnError/OnRead (matching Scktcomp::TSocketNotifyEvent /
//  TSocketErrorEvent signatures exactly; see TesterTCPSocket_Init()).
//---------------------------------------------------------------------------
void TesterTCPSocket_OnConnect(TObject* Sender, TCustomWinSocket* Socket);
    // golden TfTesterTCP::ClientSocket_TCPIPConnect, TesterTCP.cpp:166-174
void TesterTCPSocket_OnDisconnect(TObject* Sender, TCustomWinSocket* Socket);
    // golden TfTesterTCP::ClientSocket_TCPIPDisconnect, TesterTCP.cpp:176-183
void TesterTCPSocket_OnError(TObject* Sender, TCustomWinSocket* Socket,
                             TErrorEvent ErrorEvent, int& ErrorCode);
    // golden TfTesterTCP::ClientSocket_TCPIPError, TesterTCP.cpp:615-633
void TesterTCPSocket_OnRead(TObject* Sender, TCustomWinSocket* Socket);
    // golden TfTesterTCP::ClientSocket_TCPIPRead, TesterTCP.cpp:322-345

//---------------------------------------------------------------------------
//  Timer / command / log methods (golden public TfTesterTCP methods --
//  exposed here as free functions, extract-calc-core style, matching the
//  project's established pattern for VCL-form subsets with no full form
//  translated yet).
//---------------------------------------------------------------------------
// golden TfTesterTCP::TimerTCPIPConnectTimer, TesterTCP.cpp:185-239.
// AI(W5-Final-TesterTCPSocket) 20260711: golden signature is
// `void __fastcall TimerTCPIPConnectTimer(TObject *Sender)`; Sender is never
// read in the body (verified on the full read) -- dropped here (extract-
// calc-core), matching this project's established simplification for
// genuinely-unused event-handler parameters (e.g. InterfaceSYS.cpp's
// `_SendInformationToInterfaceProgram`'s explicit `(void)_iMessage;` idiom
// for params it keeps; here the param carries no information at all, so it
// is simply omitted rather than kept-and-ignored).
void TesterTCPSocket_TimerTCPIPConnectTimer();

// golden TfTesterTCP::TimerProcessTCPDataTimer, TesterTCP.cpp:349-552.
// golden signature void __fastcall (TObject *Sender); Sender unread -> dropped
// here per the TimerTCPIPConnectTimer convention just above (extract-calc-core).
void TesterTCPSocket_TimerProcessTCPDataTimer();

// golden TfTesterTCP::SimulateBin, TesterTCP.cpp:554-613 (golden itself has NO
// __fastcall on this one -- a plain method).
void TesterTCPSocket_SimulateBin();

// golden TfTesterTCP::SendTCPIPCommand(int,AnsiString,AnsiString=""), TesterTCP.cpp:241-274.
void TesterTCPSocket_SendTCPIPCommand(int index, AnsiString Msg, AnsiString Msg2 = "");

// golden TfTesterTCP::AddTCPIPCommunicationLog(int,AnsiString), TesterTCP.cpp:276-300.
// `index` is accepted but UNUSED in the golden body itself (verified on the
// full read) -- kept in the signature for call-shape fidelity (matches every
// call site, e.g. SendTCPIPCommand(index,...)->AddTCPIPCommunicationLog(index,Log)),
// not read internally, exactly as golden.
void TesterTCPSocket_AddTCPIPCommunicationLog(int index, AnsiString Str);

//---------------------------------------------------------------------------
//  Click handlers, golden TesterTCP.cpp:302-320 (small, in-range, calc-core
//  extractable -- widget reads become caller-supplied parameters).
//---------------------------------------------------------------------------
// golden TfTesterTCP::btTCPIP_TriggerClick, TesterTCP.cpp:302-307. Golden
// reads `edTCPIP_CMD->Text` into a local before calling SendTCPIPCommand;
// the widget read is the caller's job here (extract-calc-core).
void TesterTCPSocket_btTCPIP_TriggerClick(AnsiString strSendCMD);

// golden TfTesterTCP::btTCPIP_ConnectClick, TesterTCP.cpp:309-313.
void TesterTCPSocket_btTCPIP_ConnectClick();

// golden TfTesterTCP::btTCPIP_DisconnectClick, TesterTCP.cpp:315-320.
void TesterTCPSocket_btTCPIP_DisconnectClick();

#endif // TesterTCP_SocketH
