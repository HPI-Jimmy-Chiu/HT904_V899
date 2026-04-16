//---------------------------------------------------------------------------

#ifndef uRENESAS_ServerH
#define uRENESAS_ServerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ScktComp.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#include "uRENESAS_Other.h"
#include "ufrmProdInfo.h"
#include "fRENESAS_ServerFrm.h"
//#include "DateUtils.hpp"
//---------------------------------------------------------------------------
class TErrorForm : public TForm
{
__published:
    TLabel* lbl;
    TButton* btnClose;
    void __fastcall BtnCloseClick(TObject* Sender);

public:
    __fastcall TErrorForm(TComponent* Owner);
    void SetMessage(AnsiString str1, AnsiString str2);
    void CloseForm();
};
//---------------------------------------------------------------------------
class TContinueForm : public TForm                                              //RogerYang 20251107 : Add Continue Form
{
__published:
    TLabel* lbl;
    TPanel* pnl;
    void __fastcall BtnCloseClick(TObject* Sender);

public:
    __fastcall TContinueForm(TComponent* Owner);
};
//-----------------------------------------------------------------------------
class TRENESAS_Server
{
    private:
    char cLineEndData[2];
    AnsiString FLogSavePath;
    TMemo* FLogMemo;
    int iProcTask;
    TTimer *ProcTimer;
    bool GetClientConnect();
    void __fastcall OnProcTimer(TObject *Sender);
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
    void __fastcall OnProcSocketListen(System::TObject* Sender, TCustomWinSocket* Socket);
    void __fastcall OnProcSocketRead(System::TObject* Sender, TCustomWinSocket* Socket);
    void __fastcall OnProcSocketWrite(System::TObject* Sender, TCustomWinSocket* Socket);
    void __fastcall OnProcSocketError(System::TObject* Sender, TCustomWinSocket* Socket, TErrorEvent ErrorEvent, int &ErrorCode);
    void __fastcall OnProcSocketConnect(System::TObject* Sender, TCustomWinSocket* Socket);
//    void __fastcall OnProcSocketConnecting(System::TObject* Sender, TCustomWinSocket* Socket);
    void __fastcall OnProcSocketDisconnect(System::TObject* Sender, TCustomWinSocket* Socket);
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
    void SaveTaskLog(char *Task_Name,int iTask);                                //Kirin 20220127 SaveTaskLog
    bool RENESAS_LoadWorkSetup(AnsiString asWorkFileName);                      //Kirin 20191218 RENESAS FTCT載入工作檔
    void DoCheckAfter71CmdTimeOut();                                            //Kirin 20220224 DoCheckAfter71CmdTimeOut
    void DoCheckAfter71_50_40_90CmdTimeOut();                                   //Kirin 20240312
    void DoCheckAfter41CmdTimeOut();
    int iGetAfter71CmdTimeOut();                                                //Kirin 20220224
    int iGetAfter71CmdTimeOutSet();                                             //Kirin 20220224
    int iGetAfter71_50_40_90CmdTimeOut();                                       //Kirin 20240312
    int iGetAfter71_50_40_90CmdTimeOutSet();                                    //Kirin 20240312
    __property bool   bHaveClientConnect={read=GetClientConnect};
    __property TMemo* LogMemo={read=FLogMemo,write=FLogMemo};
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
    void __fastcall ShowErrorMessageByReplyError(AnsiString asSendErrCode, AnsiString asSendErrComm);
    void __fastcall ShowContinueMessageByReply71(bool bAlarm);                  //RogerYang 20251107 : Add Continue Form
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
