//---------------------------------------------------------------------------
#ifndef noteH
#define noteH
//---------------------------------------------------------------------------
//#include "MyKitSuck.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <MPlayer.hpp>
#include <StdCtrls.hpp>
#include "butPa1.h"
#include "HTray.h"
#include "myTimer.h"
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfNote : public TForm
{
__published:    // IDE-managed Components
    TTimer *Timer1;
    TTimer *TimerFTP;
    TPanel *pnlBottom;
    TPanel *pnlPicker;
    TPanel *pnlMovie;
    TTMyTray *TMyTray1;
    TButton *btPrintSummary;
    TMediaPlayer *MediaPlayer1;
    TPanel *pnlMsg;
    TRichEdit *reDescription;
    TRichEdit *RichEdit1;
    TEdit *ShowMessageEdit1;
    TPanel *pnlNote;
    TPanel *pnlButton;
    TLabel *Label3;
    TLabel *Label5;
    TLabel *lbl1;
    TLabel *labSecsGemLock;
    TBtnPanel *BtnOneCycle;
    TBtnPanel *BtnSkip;
    TBtnPanel *BtnTrayEnd;
    TBtnPanel *BtnCleanOut;
    TBtnPanel *BtnStart;
    TBtnPanel *BtnPause;
    TEdit *edErrorCode;
    TEdit *Edit3;
    TEdit *edUnitName;
    TBtnPanel *BtnReset;
    TBtnPanel *BtnHome;
    TBtnPanel *BtnTrain;
    TBtnPanel *BtnRetry;
    TBtnPanel *BtnTrayFeed;
    TBtnPanel *BtnFix;
    TBtnPanel *BtnAlarmReset;
    TLabel *Label2;
    TPageControl *pgcNote;
    TTabSheet *tsHandler;
    TTabSheet *tsRedAlarm;
    TTabSheet *tsTrayQty;
    TTabSheet *tsRingErr;
    TPanel *PanelMain6;
    TPanel *palCheckSht;
    TLabel *lblCheckShtEn;
    TLabel *lblCheckShtCh;
    TPanel *pnlIndex1Error;
    TLabel *lblIndex1ErrorEN;
    TLabel *lblIndex1ErrorCH;
    TPanel *PanSpecialNote;
    TLabel *lblSpecialNoteEn;
    TLabel *lblSpecialNoteCh;
    TPanel *pnlContact;
    TLabel *lblContactEN;
    TLabel *lblContactCH;
    TPanel *palOutShuttleLossIC;
    TLabel *labOutShuttleLossIEN;
    TLabel *labOutShuttleLossICH;
    TPanel *palWrongPW;
    TLabel *lblWrongPwEN;
    TLabel *lblWrongPwCH;
    TPanel *palSGCheckList;
    TLabel *labSG_CheckList_Check;
    TLabel *labSG_CheckList_Current;
    TLabel *labSG_OneCycleMsg;
    TPanel *pnlTrayQty;
    TLabel *labQtyLoader;
    TLabel *labQtyFix1;
    TLabel *labQtyFix2;
    TLabel *labQtyFix3;
    TLabel *labQtyAuto1;
    TLabel *labQtyAuto2;
    TLabel *labQtyAuto3;
    TLabel *labTrayQty;
    TEdit *edQtyLoader;
    TEdit *edQtyAuto1;
    TEdit *edQtyAuto2;
    TEdit *edQtyAuto3;
    TEdit *edQtyFix3;
    TEdit *edQtyFix2;
    TEdit *edQtyFix1;
    TTreeView *tvMNet;
    TTabSheet *tsMDBQuery;
    TStringGrid *strngrdMDBQuery;
    TTabSheet *tsCorrectionCount;
    TPanel *pnlCorrectionCount;
    TLabel *lblInputJamCount;
    TLabel *lblOutputJamCount;
    TLabel *lblMissMatch;
    TButton *btContinuousTest;
    TPanel *palInputJamCount;
    TPanel *palOutputJamCount;
    TButton *btCorrectCounting;
    TButton *btCorrectCounting_Auto;
    TPanel *pnlMissMatchQty;
    TButton *btCorrectCounting_StopART;
    TPanel *pnlAutoCorrCnt;
    TTabSheet *tsATC;
    TPanel *pnl_ATCAlarmChart_1;
    TImage *img3;
    TImage *img4;
    TImage *img5;
    TImage *img6;
    TImage *img7;
    TImage *img8;
    TImage *img9;
    TImage *img10;
    TImage *img11;
    TPanel *pnlATC_Handler;
    TPanel *pnlATC_TCPIP;
    TPanel *pnlATC_ATC;
    TPanel *pnlATC_RS232;
    TPanel *pnlATC_PowerSupply;
    TPanel *pnlATC_NI;
    TPanel *pnlATC_Chiller;
    TPanel *pnlATC_WaterValve;
    TPanel *pnlATC_Head;
    TTabSheet *tsLotInfo;
    TPanel *pnlLotInfo;
    TLabel *Label16;
    TLabel *lblLotID;
    TLabel *lblLotQty;
    TLabel *lblQCQty;
    TEdit *edtLotCount;
    TEdit *edtLotID;
    TEdit *edEQCQty;
    TTabSheet *ts2DID;
    TTMyTray *t2DCode;
    TButton *btCloseMDBQuery;
    TTabSheet *tsBigDescription;
    TRichEdit *reBigDescription;
    TTabSheet *tsRTC;
    TPanel *palRTCFullView;
    TImage *Image1;
    TImage *Image2;
    TLabel *Label8;
    TImage *Image3;
    TImage *Image4;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *palOCR;
    TPanel *pnlFix4;
    TLabel *labQtyFix4;
    TLabel *labQtyFix5;
    TLabel *labQtyFix6;
    TEdit *edQtyFix4;
    TEdit *edQtyFix5;
    TEdit *edQtyFix6;
    TPanel *pnlFix789;
    TLabel *labQtyFix7;
    TLabel *labQtyFix8;
    TLabel *labQtyFix9;
    TEdit *edQtyFix9;
    TEdit *edQtyFix8;
    TEdit *edQtyFix7;
    TPanel *pnlFix10;
    TLabel *labQtyFix10;
    TLabel *labQtyFix11;
    TLabel *labQtyFix12;
    TEdit *edQtyFix10;
    TEdit *edQtyFix11;
    TEdit *edQtyFix12;
    TPanel *pnlSafeDoorRear;
    TPanel *palSafeDoor3;
    TPanel *palSafeDoor4;
    TPanel *palSafeDoor10;
    TPanel *palSafeDoor5;
    TPanel *palSafeDoor6;
    TPanel *Panel5;
    TPanel *palOutSh;
    TPanel *palTemp;
    TPanel *palScan;
    TPanel *palIF;
    TPanel *palSys;
    TPanel *palInArm;
    TPanel *palOutArm;
    TPanel *palTrayArm;
    TPanel *palLoad;
    TTMyTray *mtLoader;
    TPanel *palOutSh1;
    TPanel *palOutSh2;
    TPanel *palEmpty;
    TTMyTray *mtEmpty;
    TPanel *palColor;
    TTMyTray *mtColor;
    TPanel *palAuto1;
    TTMyTray *mtAuto1;
    TPanel *palAuto2;
    TTMyTray *mtAuto2;
    TPanel *palAuto3;
    TTMyTray *mtAuto3;
    TPanel *palInSh;
    TPanel *palInSh2;
    TPanel *palInSh1;
    TPanel *palHead;
    TPanel *palHead1;
    TPanel *palHead0;
    TPanel *palPlate1;
    TTMyTray *mtPlate2;
    TPanel *palPlate2;
    TTMyTray *mtPlate1;
    TPanel *palCCD;
    TPanel *palManualAll;
    TPanel *palFix1;
    TTMyTray *mtFix1;
    TPanel *palFix2;
    TTMyTray *mtFix2;
    TPanel *palFix3;
    TTMyTray *mtFix3;
    TPanel *palFix4;
    TTMyTray *mtFix4;
    TPanel *palFix5;
    TTMyTray *mtFix5;
    TPanel *palFix6;
    TTMyTray *mtFix6;
    TButton *btnMoveToFront;
    TButton *btnMoveToRear;
    TPanel *palIonFan07;
    TPanel *palIonFan05;
    TPanel *palIonFan03;
    TPanel *palIonFan02;
    TPanel *palIonFan08;
    TPanel *palIonFan01;
    TPanel *palIonFan04;
    TPanel *palIonFan11;
    TPanel *palAuto4;
    TTMyTray *mtAuto4;
    TPanel *palAuto5;
    TTMyTray *mtAuto5;
    TPanel *palAuto6;
    TTMyTray *mtAuto6;
    TPanel *palIonFan06;
    TPanel *pnlTrayCar;
    TBtnPanel *TrayEdit;
    TPanel *palAuto1_Car;
    TTMyTray *mtAuto1Car;
    TPanel *palLoad_Car;
    TTMyTray *mtLoaderBuffer;
    TPanel *palColor_Car;
    TTMyTray *mtColor_Car;
    TPanel *palEmpty_Car;
    TTMyTray *mtEmpty_Car;
    TPanel *palAuto3_Car;
    TTMyTray *mtAuto3Car;
    TPanel *palAuto2_Car;
    TTMyTray *mtAuto2Car;
    TPanel *palIonFan12;
    TPanel *palAuto4_Car;
    TTMyTray *mtAuto4Car;
    TPanel *palAuto5_Car;
    TTMyTray *mtAuto5Car;
    TPanel *palAuto6_Car;
    TTMyTray *mtAuto6Car;
    TPanel *palIonFan09;
    TPanel *palIonFan10;
    TPanel *pnlSafeDoorLeft;
    TPanel *palSafeDoor2;
    TPanel *palSafeDoor1;
    TPanel *pnlSafeDoorRight;
    TPanel *palSafeDoor7;
    TPanel *palSafeDoor8;
    TPanel *Panel8;
    TPanel *Panel9;
    TPanel *palSafeDoor9;
    TPanel *Panel6;
    TPanel *palTrayArm2;
    TPanel *palShtSensorSOP;
    TLabel *labShtSensorSOP;
    TLabel *labShtSensorSOP2;
    TPanel *pnlStopTime;
    TPanel *pnlContactOver;
    TLabel *lblCleanSocketEN;
    TLabel *lblCleanSocketCH;
    TPanel *pnlCleanSocket;
    TLabel *lblSmartCleanEN;
    TLabel *lblSmartCleanCH;
    TPanel *palCheckLoader;
    TLabel *lblCheckLoadEN;
    TLabel *lblCheckLoadCH;
    TPanel *pnlIndex2Error;
    TLabel *lblIndex2ErrorEN;
    TLabel *lblIndex2ErrorCH;
    TPanel *pnlOutArmDrop;
    TLabel *lblOutArmDropEN;
    TLabel *lblOutArmDropCN;
    TTMyTray *tBulkboxKit;
    TPanel *pnlPan_TriMachineSpecialNote;
    TLabel *lbl_TriMachineSpecialNote1;
    TScrollBox *sbBinEdit;
    TLabel *labBin;
    TLabel *labCheckBin;
    TTimer *tmrKeyIn;
    TTabSheet *tsFixBundleID;
    TPanel *Panel1;
    TLabel *lblBundleID;
    TEdit *edBundleID;
    TLabel *lblBundleText;
    TLabel *lblBundleIDErr;
    TLabel *lblBunIDNotInList;
    TLabel *lblBundleUnloadIDuplicted;
    TTimer *timerKeyence;
    TPanel *palOutArm2;
    TPanel *palOutSh3;
    TPanel *palLoad2;
    TTMyTray *mtLoader2;
    TPanel *palLoad2_Car;
    TTMyTray *mtLoaderBuffer2;
    TPanel *pnl_OpenDoor;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BtnSkipClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall BtnStartClick(TObject *Sender);
    void __fastcall BtnPauseClick(TObject *Sender);
    void __fastcall TrayEditClick(TObject *Sender);
    void __fastcall MediaPlayer1Notify(TObject *Sender);
    void __fastcall palShtSensorSOPClick(TObject *Sender);
    void __fastcall BtnResetClick(TObject *Sender);
    void __fastcall TMyTray1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall pnlMovieMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall PanSpecialNoteClick(TObject *Sender);
    void __fastcall btnMoveToFrontClick(TObject *Sender);
    void __fastcall btnMoveToRearClick(TObject *Sender);
    void __fastcall TimerFTPTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall BtnAlarmResetClick(TObject *Sender);
    void __fastcall btPrintSummaryClick(TObject *Sender);
    void __fastcall btCorrectCountingClick(TObject *Sender);
    void __fastcall btCorrectCounting_AutoClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall edtLotCountClick(TObject *Sender);
    void __fastcall edtLotIDClick(TObject *Sender);
    void __fastcall tvMNetCustomDrawItem(TCustomTreeView *Sender,
          TTreeNode *Node, TCustomDrawState State, bool &DefaultDraw);
    void __fastcall palInArmClick(TObject *Sender);
    void __fastcall PanelMain6MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btCloseMDBQueryClick(TObject *Sender);
    void __fastcall strngrdMDBQueryDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall reDescriptionMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btCorrectCounting_StopARTClick(TObject *Sender);
    void __fastcall btContinuousTestClick(TObject *Sender);
    void __fastcall tmrKeyInTimer(TObject *Sender);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall timerKeyenceTimer(TObject *Sender);
    void __fastcall edBundleIDChange(TObject *Sender);
    void __fastcall edBundleIDKeyPress(TObject *Sender, char &Key);
private:    // User declarations
    void __fastcall PlayMovie(AnsiString asPathFile);
    void __fastcall StopMovie();
    TCanvas *pCanvas;
    void __fastcall MovieToBack();
    bool bOpenChamberDoor;
    bool bOpenLeftDoor;                                                         //Steven 20130819 : 加入開門檢查的功能
    bool bOpenSixDoor;                                                          //kevin 20160725 bin error 需開右後方門確認
    bool bIndexMoveToFrontRear;
    int  iSameAlarmCT;                                                          //jou 2014-09-04 Continuous Same Alarm N time Need KeyIn Password
    AnsiString asSameAlarm;                                                     //jou 2014-09-04 Continuous Same Alarm N time Need KeyIn Password
    void __fastcall IndexMoveToFrontRear();
    int iOldKey;
    bool bNeedHighLevelPassword;                                                //jou 20171201 (Steven) : 新增統計jam code alarm次數,達到設定數量後提高一階權限才能解開alarm
    void __fastcall edBinCodeEditKeyPress(TObject *Sender, char &Key);
    bool bPLCFlag;                                                              //KenHsieh 20250307 : fix PLC safedoor 通訊延遲問題
public:     // User declarations
    __fastcall TfNote(TComponent* Owner);
    int Code;
    int ReturnCode;
    bool fShow;
    bool fShowMessage;                                                          //Chunghung 20120110 add
    int KeyCode;
    bool Select[10];                                                            //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home add BtnHome
    void __fastcall FlushLabel();
    void __fastcall ScanKey();
    int iBackOldMemo2Y;
    int iBackMemo2Height;
    bool fMemoPos;
    int  iIndexMoveToFrontRearTask;
    void __fastcall Start();
    void __fastcall UpdateButtonStatus(TObject *Sender);
    void __fastcall Reset();
    void __fastcall ErrShowToForm(AnsiString Code,AnsiString UName, AnsiString Mes, int iMotorErr);
    void __fastcall ShowErrSite(AnsiString Code, int UnitNo, int AxleNo, AnsiString errPart);
    AnsiString __fastcall GetUnitNoData(int Code);
    bool bNeedPassWord;
    bool bCloseShowMsg;                                                         //Sam 20230426 : 通知系統 Handler 已經密碼鎖定
    bool __fastcall DoPassword();
    void __fastcall RTCalarmImageUnload();                                      //jou 20170210 (Steven) : RTC alarm image unload
    void __fastcall AlarmLogXmlToFTP(AnsiString Code,AnsiString errPart);
    TPanel *palRedNotice[12];

    bool bMyServoOffInArm;                                                      //Steven 20110709
    bool bMyServoOffOutArm;                                                     //Steven 20110709
    int iMyServoOffInArmPosX;
    int iMyServoOffInArmPosY;
    int iMyServoOffOutArmPosX;
    int iMyServoOffOutArmPosY;

    //ChungHung 20140522 add OutShuttle lose devices can servo off start
    bool bMyServoOffOutShuttle1;
    bool bMyServoOffOutShuttle2;
    int iMyServoOffOutShuttle1Pos;
    int iMyServoOffOutShuttle2Pos;
    //ChungHung 20140522 add OutShuttle lose devices can servo off end

    bool __fastcall IsTestSitICFallDown();
    AnsiString sJamArea, sJamCode;
    int AlarmType;
    AnsiString TempCode;                                                        //ChungHung 20111230 add CleanOut no music
    AnsiString aJamCodeFilePath;                                                //Steven 20140526 : bSendJamCodeToFTP 改為Timer處理
    bool bSendJamCodeToFTP;                                                     //Steven 20140526 : bSendJamCodeToFTP 改為Timer處理
    bool __fastcall DoUnlockPassword();                                         //Ifor 20170214 (wei) add 解除Alarm 需要獨立密碼
    AnsiString sESDControlMachineGeneralUserID;                                 //Sam 20170822 (Steven) ESDControl 改為 HT9045 方式
    TQPF_Timer tNoteTimer;
    int iAlarmLine;                                                             //Steven 20191016 : 紀錄目前Alarm在檔案裡面的行數
    TList *listO17;
    void __fastcall ShowTrayQty();                                              //Sam 20221101 : OneCycle 完成後顯示所有 Tray 各別的IC數量
    bool bCheckContAlarmNotUpload(AnsiString sJamCode);                         //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    void ClearContAlarmList();                                                  //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    TStringList *listContAlarmList;                                             //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    TList *listContAlarmTimer;                                                  //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    int iContAlarmTime;                                                         //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    bool __fastcall CheckBinCode();
    bool bNeedTCPAlarm;                                                         //Sam 20230426 : 通知系統 Handler 已經密碼鎖定
    int iICFloattingCheckStep;                                                  //Sam 20250415 : Unloader 偵測到置偏 IC 退出後再報警
    int iICFloattingCheckPos;
    void ShowUnloaderChkMsg(iStep);
    AnsiString asMesMessage;                                                    //jou 20221104 : VTest CreateManualEOCAP function;
    bool bScanBinLabel;                                                         //JerryYang 20240821 : 自動讀取BIN LABEL
    bool bAutoIsScanBinLabel[3];                                                //JerryYang 20240821 : 自動讀取BIN LABEL
    int  iScanTask;                                                             //JerryYang 20240821 : 自動讀取BIN LABEL
    TEdit *myBinCodeEdit[TEST_MAX_BIN];                                         //JerryYang 20240111 : add P53 function
    AnsiString PosCodeToStr(int Pos);                                           //Jimmychiu 20240925 : Add the jam log and upload it to FTP once a week
    void SaveErrEventLog(double dt, AnsiString sJamArea, AnsiString sJamCode, AnsiString sRecovery, AnsiString sPassTime, AnsiString sDuplicateError,
                         AnsiString sMessage, AnsiString sErrPart, AnsiString sRecipeName);
};
//---------------------------------------------------------------------------
extern PACKAGE TfNote *fNote;
//---------------------------------------------------------------------------
//int         ShowErrorMessage        (AnsiString Code, int KCode, int Pos);
//int         ShowErrorMessage        (AnsiString Code, int KCode, int Pos, bool bDuplicateErr);
int         ShowErrorMessage        (AnsiString Code, int KCode, int Pos, bool bDuplicateErr=false, AnsiString errPart=" ");   //Steven 20091231 顯示整合的錯誤訊息
void        ShowMotorErrorMessage   (AnsiString Code, int MotorAlarmNo, AnsiString errPart="");
AnsiString  MotorIndexToJamCode     (int MotNo);
void        ShowErrorUnit(int Pos);
AnsiString  CylinderIndexToJamCode(int Code, int *Pos);
bool NowNoteIsShow();                                                           //ChungHung 20130110 add 防止訊息重複進入
extern AnsiString   RecordLastHappenTimeString, RecordLastHappenErrorCode;
extern DWORD        RecordHappenTime;
extern bool NoteFormShow();                                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
extern void ShowNoteMessage_1032(AnsiString sMsg);                              //Ztex 2023.04.19 Add HT-1032 TriTemp Function
#endif
