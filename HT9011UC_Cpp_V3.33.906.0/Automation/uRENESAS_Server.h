// =============================================================================
//  Automation/uRENESAS_Server.h  --  RENESAS FT-CT TCP/IP host-protocol server
//
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/Automation/uRENESAS_Server.h
//                 (221 lines, cp950) + uRENESAS_Server.cpp (3187 lines, cp950)
//  Translation wave: PT-W3 "renesas"
//  Translator: AI(W906-PT-W3) 20260807
//
//  ROLE: TRENESAS_Server is a TCP/IP listen-server (BCB6 TServerSocket) that
//  speaks the RENESAS FT-CT handler protocol (commands 00/10/20/50/30/40/70/
//  90/B0 in, 01/11/21/51/31/41/71/91/B1 out).  DoProcess() is a big polling
//  state machine (golden .cpp :325-1880, one static Task int driving a big
//  switch) that reacts to RecvData.*.iRecv flags set by DoProcRecvData() (the
//  socket OnRead handler) and drives fSCKART/fMain/fLotInfo/fSortCT through
//  the ART lot-start/lot-end/test-end sequence.
//
//  WAVE SCOPE
//  ----------
//  ACTIVE (every one of the 62 golden top-level functions gets a real body
//  in the .cpp -- see that file's own function-by-function banner index for
//  golden line numbers):
//    TRENESAS_Server:: ctor/dtor, Listen, OnProcSocket{Listen,Read,Write,
//    Error,Connect,Disconnect}, GetClientConnect, AppendToMemo,
//    DoProcRecvData, GetFieldData, DoProcess, Get{OnLine,ProductInfo,Start,
//    HandlerStart,End,LotEnd,TestEnd,SendAlarm,SendCleanOut}Messge/Msg,
//    Send{OnLine,ProductInfo,Start,HandlerStart,End,LotEnd,TestEnd,DoSend
//    TestEnd,Alarm,CleanOut}, SendSocketData, ShowServerForm, DoProcOnShow,
//    OnProcTimer, SaveModeEndFile, Set/GetErrData, GetErrCode, InitErrData,
//    GetNowErrInt, OnCallHandler{OnLine,Start,ProdInfo}Event, InitParamter,
//    bCanNotUse_Cmd_With_TestEnd, SendReplyToFTCT, SaveTaskLog,
//    DoCheckAfter71CmdTimeOut, DoCheckAfter41CmdTimeOut,
//    DoCheckAfter71_50_40_90CmdTimeOut, iGetAfter71CmdTimeOut,
//    iGetAfter71_50_40_90CmdTimeOut, DoCheckWait20or40TimeOut,
//    DoGet20or40After71, DoNeedSupplyOrNot, FTCTManStartUnlock/Lock,
//    ResetFTCTSeq, CheckInputCommand, Check71CommandDuring50,
//    ShowErrorMessageByReplyError, ShowContinueMessageByReply71,
//    CloseContinueMessageForm;
//    free functions GetDelayStart(), CheckTimeOut();
//    TErrorForm::{ctor,BtnCloseClick,SetMessage}; TContinueForm::{ctor,
//    BtnCloseClick}.
//  golden RENESAS_LoadWorkSetup (declared in the .h) has NO body anywhere in
//  golden uRENESAS_Server.cpp (it is fully commented out, golden :2540-2572)
//  and is never called live -- a FAITHFUL GOLDEN GAP, reproduced by leaving
//  it undeclared here too (same "declared-but-dead, don't invent a body"
//  precedent as HANA_ART.h's handleError(int)).
//
//  GATE REGISTER
//  -------------
//  [G1] `class TErrorForm : public TForm` / `class TContinueForm : public
//       TForm` (golden this-file :15-37).  No compiled `TForm` exists
//       anywhere in the port tree (grepped tree-wide 20260807: zero hits for
//       "class TForm" outside golden/tools; consistent with
//       docs/PT_CAMPAIGN_PLAN.md's own measurement that VCL-form units are
//       4.0% translated project-wide).  GATED: the `: public TForm` base is
//       dropped; both classes are free-standing.  Their golden ctor bodies
//       (TErrorForm::TErrorForm golden :3055-3072, TContinueForm::
//       TContinueForm golden :3113-3144) are ALL cosmetic VCL layout/paint
//       property assignments (Width/Height/BorderStyle/Position/Color/
//       FormStyle/BorderIcons, and on the child TLabel/TPanel/TButton:
//       Font->Size/Color/Name/Style, Alignment, Layout, AutoSize, WordWrap,
//       Transparent, SetBounds) -- vclcompat/Controls.h's TLabel/TPanel/
//       TButton deliberately do not carry any of those (that file's own
//       "MINIMAL SURFACE" rule: only members with a real ported consumer),
//       and Controls.h is a pre-existing file this wave must not edit.  Each
//       such line is gated individually in the .cpp with the golden line
//       quoted verbatim.  FAITHFUL BECAUSE: this is a headless console-mode
//       build with no window system pumping paint messages at all (same
//       "no window ever appears" reality vclcompat/Controls.h's own
//       DEFAULT-VALUE RULE documents for every stock widget); the two
//       classes' only OBSERVABLE (non-paint) behaviour -- Visible tracking
//       (Show()/Hide() flip a bool), the message text (SetMessage / the
//       contfrm->lbl->Caption assignment), and the three global alarm/buzzer
//       booleans (bReplyFTCTAlarm/bAlarmBuzzer/bContinueMessageByReply71)
//       these methods set -- is preserved exactly, unGated.  BEHAVIOUR
//       DELTA ON A REAL MACHINE: no popup window is ever drawn; an operator
//       watching the screen will not see the red "Return Error TO FTCT"
//       banner or the blue "waiting after 71" banner, but every OTHER
//       consumer of bReplyFTCTAlarm/bAlarmBuzzer/bContinueMessageByReply71
//       (e.g. fMain's alarm-buzzer polling loop, not in this file) still
//       fires exactly as golden drives it.
//  [G2] `TfServerFrm` / `TframeProdInfo` (golden types, from the SEPARATE,
//       NOT-in-this-wave's-scope golden units Automation/fRENESAS_ServerFrm.h
//       /.cpp and Automation/ufrmProdInfo.h/.cpp -- both real VCL FORMS;
//       docs/PT_CAMPAIGN_PLAN.md buckets "VCL form units" as a distinct,
//       still ~4%-translated category, and this wave's charter names only
//       uRENESAS_Server.cpp).  `TRENESAS_Server::ServerFrm` (golden this-file
//       :76) is a `TfServerFrm*` read/written by 9 LIVE (non-comment,
//       grepped exhaustively over all 3187 golden .cpp lines) member
//       accesses -- asFTCTLastSeqStatus, asHandlerLastSeqStatus, TestMode,
//       iServerListenStatus, iOnLineStatus, iNowCounter, frameProdInfo1,
//       BringToFront(), Show() -- plus frameProdInfo1's own 8 live members
//       (ShowData + the 7 lbledtXxx->Text golden TLabeledEdit* fields).  A
//       forward-declared incomplete type cannot support that.  GATED: this
//       header carries a MINIMAL, DATA-ONLY stand-in below carrying EXACTLY
//       those touched members (each with its golden fRENESAS_ServerFrm.h /
//       ufrmProdInfo.h line cited in the class comment) -- NOT a translation
//       of either golden file, and explicitly NOT extended with anything
//       those two files declare that this TU does not touch (e.g. the 13
//       other TLabeledEdit* HdPass/HdFail/... counters visible on the real
//       form, ProcTimer, the 4 event-handler methods).  FUTURE-WAVE ACTION:
//       when Automation/fRENESAS_ServerFrm.cpp/ufrmProdInfo.cpp get their own
//       translation wave, DELETE this stand-in and #include their real
//       headers instead -- do not let both definitions of TfServerFrm exist
//       in the same binary.
//  [G3] `TTimer` (golden this-file uses `TTimer *ProcTimer;`, golden ctor
//       :53-56).  No compiled TTimer exists anywhere in the port tree
//       (grepped 20260807).  The tree's established idiom for a periodically-
//       fired body is a TThread-driven loop (see uHeaterThread.cpp), which is
//       main-loop integration work, not a one-file translation.  GATED: a
//       minimal data-only TTimer stand-in (Interval/Enabled/OnTimer) is
//       declared below -- it stores the golden Interval=10/Enabled=true
//       values and the OnTimer closure faithfully, but nothing in this
//       translation unit pumps it.  BEHAVIOUR DELTA: OnProcTimer() (which
//       calls DoProcOnShow/DoProcess/the timeout checks -- the actual FT-CT
//       protocol engine) will never fire on its own; whichever future wave
//       wires TRENESAS_Server into the real main loop must call
//       `RENESAS_Server->OnProcTimer(NULL)` (or invoke DoProcess() etc.
//       directly) on a ~10ms cadence, exactly as golden's VCL TTimer did.
//  See uRENESAS_Server.cpp's own banner for [G4]..[G6] (ServerSocket
//  OnListen/OnClientWrite, ShowLotEndMessage, and the two same-result
//  substitutions that are NOT gates) and for the FACADE ADDITIONS NEEDED
//  list (fMain/fSCKART/fLotInfo/fSortCT/LotSummary members this file calls
//  exactly as golden does, per the tree's own Automation/HANA_ART.cpp
//  precedent for a consumer written ahead of its facade).
//
//  TRANSLATION RULES APPLIED
//  --------------------------
//    * __fastcall / __published / __property all removed.  The two
//      golden read-only __property members (bHaveClientConnect, LogMemo)
//      become plain getter/setter methods with the SAME NAME as the golden
//      property identifier (established convention -- see
//      Automation/uRENESAS_Other.h's identical __property collapse, whose
//      own banner explicitly flagged this file as the future consumer that
//      would need `()` added at every RENESAS_CMD.<Prop> / RENESAS_Err_Index.
//      <Prop> call site; done throughout the .cpp).
//    * AnsiString/TStringList/TDateTime/TServerSocket/TCustomWinSocket/
//      TErrorEvent all come from this tree's own vclcompat headers, NOT
//      std::string / real VCL.
//    * `System::TObject` -> bare `TObject` (matches Automation/automation.h's
//      identical socket-handler-signature translation).
//    * Big5 Chinese comments decoded via cp950 (python
//      open(...,encoding='cp950')), reproduced as real UTF-8.  ZERO U+FFFD.
// =============================================================================
#ifndef uRENESAS_ServerH
#define uRENESAS_ServerH

#include "vclcompat/vcl_compat.h"      // AnsiString, DWORD, Word, ...
#include "vclcompat/TStringList.h"     // TObject, TStringList
#include "vclcompat/TDateTime.h"       // TDateTime, Now()
#include "vclcompat/ServerSocket.h"    // TServerSocket, TCustomWinSocket, TErrorEvent
#include "vclcompat/Controls.h"        // TLabel, TPanel, TButton, TLabeledEdit (real, reused)

#include "uRENESAS_Other.h"            // TRENESAS_RECV_CMD_DATA / TRENESAS_Send_CMD_DATA /
                                       // TRENESAS_EndFileData / RENESAS_CMD / RENESAS_Status /
                                       // RENESAS_Err_Index (already ported, W5-RENESAS_Other-Translate)

#include <functional>                  // std::function -- event-closure idiom (matches
                                       // vclcompat/ClientSocket.h's TSocketNotifyEvent)

//---------------------------------------------------------------------------
// [G3] TTimer -- minimal data-only stand-in.  See file banner GATE [G3].
// golden VCL TTimer surface used by this file: Interval (int), Enabled
// (bool), OnTimer (TNotifyEvent).  NOT a general-purpose TTimer -- if a
// future unit needs more of real TTimer's surface, extend there, not here
// (mirrors vclcompat/ServerSocket.h's own "API SURFACE BUILT -- nothing
// beyond what the cited consumer needs" discipline).
//---------------------------------------------------------------------------
class TTimer
{
public:
    int  Interval;
    bool Enabled;
    std::function<void(TObject* Sender)> OnTimer;
    explicit TTimer(TObject* /*Owner*/) : Interval(0), Enabled(false) {}
};

//---------------------------------------------------------------------------
// [G2] TframeProdInfo -- minimal data-only stand-in for golden
// Automation/ufrmProdInfo.h's TframeProdInfo (32 lines).  ONLY the members
// TRENESAS_Server::DoProcess/DoProcOnShow touch (golden ufrmProdInfo.h line
// cited per member below).  See file banner GATE [G2].
//---------------------------------------------------------------------------
class TframeProdInfo
{
public:
    TRENESAS_RECV_CMD_DATA ShowData;      // golden ufrmProdInfo.h:27 (public data member, not a __property)
    TLabeledEdit *lbledtLotNo;            // golden ufrmProdInfo.h:16
    TLabeledEdit *lbledtLotID;            // golden ufrmProdInfo.h:17
    TLabeledEdit *lbledtHdParameter;      // golden ufrmProdInfo.h:18
    TLabeledEdit *lbledtProduct;          // golden ufrmProdInfo.h:19
    TLabeledEdit *lbledtStepCode;         // golden ufrmProdInfo.h:20
    TLabeledEdit *lbledtQty;              // golden ufrmProdInfo.h:21
    TLabeledEdit *lbledtDate;             // golden ufrmProdInfo.h:22

    TframeProdInfo()
        : lbledtLotNo(new TLabeledEdit()), lbledtLotID(new TLabeledEdit()),
          lbledtHdParameter(new TLabeledEdit()), lbledtProduct(new TLabeledEdit()),
          lbledtStepCode(new TLabeledEdit()), lbledtQty(new TLabeledEdit()),
          lbledtDate(new TLabeledEdit())
    {}
    ~TframeProdInfo()
    {
        delete lbledtLotNo; delete lbledtLotID; delete lbledtHdParameter;
        delete lbledtProduct; delete lbledtStepCode; delete lbledtQty; delete lbledtDate;
    }
};

//---------------------------------------------------------------------------
// [G2] TfServerFrm -- minimal data-only stand-in for golden
// Automation/fRENESAS_ServerFrm.h's TfServerFrm (85 lines).  ONLY the members
// TRENESAS_Server touches (golden fRENESAS_ServerFrm.h line cited per
// member below).  See file banner GATE [G2].
//---------------------------------------------------------------------------
class TfServerFrm
{
public:
    int iServerListenStatus;              // golden fRENESAS_ServerFrm.h:55
    int iOnLineStatus;                    // golden fRENESAS_ServerFrm.h:56
    int iNowCounter;                      // golden fRENESAS_ServerFrm.h:74
    AnsiString TestMode;                  // golden fRENESAS_ServerFrm.h:76
    AnsiString asFTCTLastSeqStatus;       // golden fRENESAS_ServerFrm.h:77
    AnsiString asHandlerLastSeqStatus;    // golden fRENESAS_ServerFrm.h:78
    TframeProdInfo *frameProdInfo1;       // golden fRENESAS_ServerFrm.h:18
    bool Visible;                         // golden TForm-inherited property (no own fRENESAS_ServerFrm.h line;
                                          //   BringToFront()/Show() below are the only writers, matching the
                                          //   [G1]-style "Visible tracked, painting gated" split)

    TfServerFrm(TObject* /*Owner*/)
        : iServerListenStatus(0), iOnLineStatus(0), iNowCounter(0),
          frameProdInfo1(new TframeProdInfo()), Visible(false)
    {}
    ~TfServerFrm() { delete frameProdInfo1; }

    // golden TForm::Show()/BringToFront() -- see file banner GATE [G1]/[G2]:
    // no window is ever painted in this headless build; Visible is tracked
    // faithfully (DoProcOnShow reads it via bHaveClientConnect, unrelated;
    // ShowServerForm()'s own call sites only care about the side effect of
    // "the form now considers itself open"). BringToFront() has no
    // observable state at all in golden beyond raising window Z-order, so it
    // is a pure no-op here, exactly as vclcompat::TControl::Click() is a
    // no-op for the same "no window" reason.
    void Show()         { Visible = true; }
    void BringToFront() {}
};

//---------------------------------------------------------------------------
class TErrorForm                                                                // golden: public TForm -- see GATE [G1]
{
public:
    AnsiString lblCaption;   // stand-in for golden `TLabel* lbl` -- only ->Caption is touched
                             // (SetMessage writes it); the rest of TLabel's layout surface
                             // (Font/Alignment/Layout/AutoSize/WordWrap/Transparent/SetBounds)
                             // is gated in the .cpp ctor -- see GATE [G1].
    bool Visible;

    explicit TErrorForm(TObject* Owner);
    void BtnCloseClick(TObject* Sender);
    void SetMessage(AnsiString str1, AnsiString str2);
    void Show()         { Visible = true; }
    void Hide()         { Visible = false; }
    void BringToFront() {}
};
//---------------------------------------------------------------------------
class TContinueForm                                                             // golden: public TForm -- see GATE [G1]
                                                                                  //RogerYang 20251107 : Add Continue Form
{
public:
    AnsiString lblCaption;   // stand-in for golden `TLabel* lbl` -- see TErrorForm::lblCaption note
    int        lblFontColor; // stand-in for golden `lbl->Font->Color` (clRed/clBlue) -- the ONLY
                             // other lbl-> member DoProcess/Check71CommandDuring50 touch besides Caption
    bool Visible;

    explicit TContinueForm(TObject* Owner);
    void BtnCloseClick(TObject* Sender);
    void Show()         { Visible = true; }
    void Hide()         { Visible = false; }
    void BringToFront() {}
};
//-----------------------------------------------------------------------------
class TRENESAS_Server
{
    private:
    char cLineEndData[2];
    AnsiString FLogSavePath;
    TStringList* FLogMemo;                                                     // golden `TMemo* FLogMemo;` -- only ever
                                                                                 // assigned NULL / read via the LogMemo
                                                                                 // property, never ->Lines-manipulated by
                                                                                 // this file live (the one Lines touch is
                                                                                 // commented out, ctor golden :48); a
                                                                                 // TStringList* is enough surface for that.
    int iProcTask;
    TTimer *ProcTimer;
    bool GetClientConnect();
    void OnProcTimer(TObject *Sender);
    bool bRenFTCTManualStartTmp;
    //Kirin 20191218 不用了
    //==>
//    TNotifyEvent    FOnCallHandlerOnLineEvent;
//    TNotifyEvent    FOnCallHandlerStartEvent;
//    TNotifyEvent    FOnCallHandlerProdInfoEvent;
//    TNotifyEvent    FOnCallHandlerModeEndEvent;
    //<==
    //Kirin 20191218 不用了
    AnsiString sModeStartTmp;
    bool bCanReturn71;
    static TErrorForm* frm;                                                     // 靜態指標，確保只有一個 Form
    static TContinueForm* contfrm;                                              //RogerYang 20251107 : Add Continue Form

    public:
    int iOnLineStatus;
    int iErrInt;
    //Kirin 20191218 FTCT上報用
    //==>
    int iLoadWorkFileStatus;
    AnsiString asErrGroup[255];
    AnsiString asErrCommGroup[255];
    AnsiString asSendErrCode;
    AnsiString asSendErrComm;
    //<==
    //Kirin 20191218 FTCT上報用
    TRENESAS_EndFileData    EndFileData;
    TfServerFrm *ServerFrm;
    TServerSocket *ServerSocket;
    //Kirin 20191218 FTCT上報用
    //==>
    AnsiString asHandleName;
    AnsiString asFTCTModeEndFilePath;
    //<==
    //Kirin 20191218 FTCT上報用
    AnsiString asMsg;                                                           //Kirin 20200107 Add TempLog use
    int iListenPort;
    //Kirin 20191219 FTCT上報用
    //==>
    int iModeEndCounterClear;
    int iHaveRecvModeStart;
    int iHaveRecvModeEnd;
    int iHaveProdInfoStart;
    int iProdStartProcess;
    int iHaveRecvTestEnd;
    //<==
    //Kirin 20191219 FTCT上報用
    //int iCleanOutFinishLockSTART;                                             //Kirin 20200115 CleanOut Finish Lock start
    //int iManualStart;                                                           //Kirin 20200217 Add Manual Start
    int iFTCTMagazineAlarmShow;                                                 //Kirin 20200220 Show Magazine Alarm
    int iFTCTSendCleanOut;                                                      //Kirin 20221121 FTCT Send Clean Out

    bool bSend71WaitReply30;                                                    //Kirin 20220214 bSend71WaitReply30
    TDateTime TimeOut_Send71WaitReply30;                                        //Kirin 20220214 TimeOut_Send71WaitReply30
    int iSend71WaitReply30_TimeOutSet;                                          //Kirin 20220214 iSend71WaitReply30_TimeOutSet
    bool bSend71WaitReply30Alarm;                                               //Kirin 20220224 iSend71WaitReply30Alarm

    bool bSend71WiteReply50_40_90;                                              //Kirin 20240312 50_40_90RecvTimeOut
    TDateTime TimeOut_Send71WaitReply50_40_90;                                  //Kirin 20240312 50_40_90RecvTimeOut
    int iSend71WaitReply50_40_90_TimeOutSet;                                    //Kirin 20240312 50_40_90RecvTimeOut
    bool bSend71WaitReply50_40_90Alarm;                                         //Kirin 20240312 50_40_90RecvTimeOut

    bool b70_Disconnect;                                                        //Kirin 20220225 b70_Disconnect
    bool b70_DisconnectAlarm;                                                   //Kirin 20220225 b70_Disconnect
    bool b70_Do70MsgAlarm;                                                      //RogerYang 20251111 Add
    bool bCanRecvCleanOut_B0;                                                   //Kirin 20221123 bCanRecvCleanOut_B0
    bool bAfterSend41CanCheckAuto2Remove;                                       //Kirin 20230113 bAfterSend41CanCheckAuto2Remove
    bool bCleanCountFail;                                                       //Kirin 20230814 add 清量失敗旗標

    bool bSend41WaitTimeout;
    TDateTime TimeOut_Send41WaitTimeout;
    bool bSend41WaitTimeoutAlarm;

    TRENESAS_RECV_CMD_DATA RecvData;
    TRENESAS_Send_CMD_DATA SendData;
    TRENESAS_Server();
    ~TRENESAS_Server();
    void OnProcSocketListen(TObject* Sender, TCustomWinSocket* Socket);
    void OnProcSocketRead(TObject* Sender, TCustomWinSocket* Socket);
    void OnProcSocketWrite(TObject* Sender, TCustomWinSocket* Socket);
    void OnProcSocketError(TObject* Sender, TCustomWinSocket* Socket, TErrorEvent ErrorEvent, int &ErrorCode);
    void OnProcSocketConnect(TObject* Sender, TCustomWinSocket* Socket);
//    void __fastcall OnProcSocketConnecting(TObject* Sender, TCustomWinSocket* Socket);
    void OnProcSocketDisconnect(TObject* Sender, TCustomWinSocket* Socket);
    void Listen();
    void AppendToMemo(AnsiString asMsg);
    void DoProcRecvData(int iLen,char* cbuf);
    void DoProcess();
    void DoProcOnShow();
    void SetErrData(int iAlarmIndex,AnsiString asErrCode,AnsiString asErrComm);
    void GetErrCode(int iAlarmIndex,AnsiString &asErrCode,AnsiString &asErrComm);
    void InitErrData();
    void InitParamter();                                                        //Kirin 20220118 初始化變數
    bool bCanNotUse_Cmd_With_TestEnd(AnsiString asCmd);                         //Kirin 20220119 檢查是否有收到70
    int GetNowErrInt();
    AnsiString GetFieldData(AnsiString asSource,AnsiString asFieldName);
    AnsiString GetOnLineMessge();
    AnsiString GetProductInfoMessge();
    AnsiString GetStartMessge();
    AnsiString GetHandlerStartMessge();
    AnsiString GetEndMessge();
    AnsiString GetLotEndMessge();
    AnsiString GetTestEnd();                                                    //Kirin 20220119 GetSendTestEnd
    AnsiString GetSendAlarm();                                                  //Kirin 20211020
    AnsiString GetSendCleanOut();                                               //Kirin 20221121 GetSendCleanOut
    //AnsiString GetFTCTResult(int iIndex);                                     //Kirin 20230926 FTCT自定義CODE

    void SendOnLine(int iErrorCode=-1); //defaule -1=NG
    void SendProductInfo(int iErrorCode=-1);
    void SendStart(int iErrorCode=-1);
    void SendHandlerStart(int iErrorCode=-1);
    void SendEnd(int iErrorCode=-1);
    void DoSendTestEnd();
    void SendLotEnd(int iErrorCode=-1);
    void SendTestEnd(int iErrorCode=-1);                                                         //Kirin 20220119 SendTestEnd
    void SendAlarm(int iErrorCode=0);                                           //Kirin 20211020 SendAlarm
    void SendCleanOut(int iErrorCode=0);                                        //Kirin 20221124 SendCleanOut
    void SendReplyToFTCT(AnsiString asCmd, int iErrorCode=-1);
    bool SendSocketData(AnsiString asMsg);
    void ShowServerForm();
    void SaveModeEndFile();
    void OnCallHandlerOnLineEvent();
    void OnCallHandlerProdInfoEvent();
    void OnCallHandlerStartEvent();
    void SaveTaskLog(const char *Task_Name,int iTask);                          //Kirin 20220127 SaveTaskLog
    // golden `RENESAS_LoadWorkSetup` is declared here but has NO body anywhere
    // in golden uRENESAS_Server.cpp (fully commented out, golden :2540-2572)
    // and is never called live -- FAITHFUL GOLDEN GAP, left undeclared (see
    // file banner "WAVE SCOPE" note; same precedent as HANA_ART.h's
    // handleError(int)).
    void DoCheckAfter71CmdTimeOut();                                            //Kirin 20220224 DoCheckAfter71CmdTimeOut
    void DoCheckAfter71_50_40_90CmdTimeOut();                                   //Kirin 20240312
    void DoCheckAfter41CmdTimeOut();
    int iGetAfter71CmdTimeOut();                                                //Kirin 20220224
    int iGetAfter71CmdTimeOutSet();                                             //Kirin 20220224
    int iGetAfter71_50_40_90CmdTimeOut();                                       //Kirin 20240312
    int iGetAfter71_50_40_90CmdTimeOutSet();                                    //Kirin 20240312
    bool   bHaveClientConnect() { return GetClientConnect(); }                  // golden: __property bool bHaveClientConnect={read=GetClientConnect}
    TStringList* LogMemo() const   { return FLogMemo; }                         // golden: __property TMemo* LogMemo={read=FLogMemo,write=FLogMemo}
    void         LogMemo(TStringList* v) { FLogMemo = v; }
    //Kirin 20191218 不用了
    //==>
    //__property TNotifyEvent  OnCallHandlerOnLineEvent          ={read=FOnCallHandlerOnLineEvent  ,write=FOnCallHandlerOnLineEvent };
    //__property TNotifyEvent  OnCallHandlerStartEvent           ={read=FOnCallHandlerStartEvent   ,write=FOnCallHandlerStartEvent  };
    //__property TNotifyEvent  OnCallHandlerProdInfoEvent        ={read=FOnCallHandlerProdInfoEvent,write=FOnCallHandlerProdInfoEvent};
    //__property TNotifyEvent  OnCallHandlerModeEndEvent         ={read=FOnCallHandlerModeEndEvent ,write=FOnCallHandlerModeEndEvent };
    //<==
    //Kirin 20191218 不用了
    //int iNeedToRT;

    bool bWait20or40;
    bool bWait20or40Alarm;
    TDateTime TimeOut_Wait20or40;
    void DoCheckWait20or40TimeOut();
    bool DoGet20or40After71();
    bool bLoadingCountFullFlag;
    bool bNeedSupply;
    bool DoNeedSupplyOrNot(bool bNotEnough);
    void FTCTManStartUnlock(AnsiString str="");
    void FTCTManStartLock();
    void ResetFTCTSeq();
    bool Check71CommandDuring50();
    bool CheckInputCommand(AnsiString str);
    void ShowErrorMessageByReplyError(AnsiString asSendErrCode, AnsiString asSendErrComm);
    void ShowContinueMessageByReply71(bool bAlarm);                             //RogerYang 20251107 : Add Continue Form
    void CloseContinueMessageForm();                                            //RogerYang 20251107 : Add Continue Form

    bool bContFrmShow;                                                          //RogerYang 20251107 : Add Continue Form

    bool bReturn41Flag;
    AnsiString sLastLegalState; //用來判斷接收下一次命令是否合法的依據(見規格表)
    bool bTrayFeedDone;
    int iFTRTCntTmpFor70;
};
//---------------------------------------------------------------------------
extern TDateTime GetDelayStart();                                               //Kirin 20220224
extern bool CheckTimeOut(TDateTime DelayStamp,DWORD OutTime);                   //Kirin 20220224
#endif
