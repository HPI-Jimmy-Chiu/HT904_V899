//---------------------------------------------------------------------------
#ifndef ProductionInfoH
#define ProductionInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <FileCtrl.hpp>
#include <Buttons.hpp>
#include "myTimer.h"
#include "uSetUpConfiguration.h"
#include "HTEditList.h"
#include "uTimeTool.h"
#define TenUM_TO_MM_FACTOR 0.01 // 10微米到毫米的轉換係數
#define MM_TO_TenUM_FACTOR 100
//---------------------------------------------------------------------------
class cDynamicMultiContinualPassBinBySocket;
//---------------------------------------------------------------------------
class TfProductionInfo : public TForm
{
__published:    // IDE-managed Components
    TTimer *tm_PI;
    TTimer *tm_IPSCControl;
    TPageControl *PageControl1;
    TTabSheet *tsInformation;
    TPageControl *PageControl2;
    TTabSheet *TabSheet1;
    TStringGrid *sg_OEEList;
    TTabSheet *TabSheet2;
    TStringGrid *sg_SummaryReport;
    TTabSheet *TabSheet3;
    TStringGrid *sg_SetupCondition;
    TTabSheet *TabSheet4;
    TStringGrid *sg_History;
    TTabSheet *TabSheet5;
    TStringGrid *sg_ESDControlMsg;
    TPanel *Panel1;
    TPanel *pnlPMAlarmBeforeDays;
    TComboBox *cb_PIChoiseMO;
    TButton *bt_PIRefreshMOList;
    TComboBox *coCheckFTPFileList;
    TPanel *Panel2;
    TButton *bt_PICloseForm;
    TTabSheet *tsFilePath;
    TDirectoryListBox *dlb_PIChoiseMO;
    TTabSheet *tsHaltStatusForm;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *lbStatus_HALT;
    TLabel *Label6;
    TSpeedButton *sbScreenkeyboard_HALT;
    TSpeedButton *SpeedButton_Halt00;
    TSpeedButton *SpeedButton_Halt01;
    TSpeedButton *SpeedButton_Halt02;
    TSpeedButton *SpeedButton_Halt03;
    TSpeedButton *SpeedButton_Halt04;
    TSpeedButton *SpeedButton_Halt05;
    TSpeedButton *SpeedButton_Halt06;
    TSpeedButton *SpeedButton_Halt07;
    TSpeedButton *SpeedButton_Halt08;
    TSpeedButton *SpeedButton_Halt09;
    TSpeedButton *SpeedButton_Halt10;
    TSpeedButton *SpeedButton_Halt11;
    TSpeedButton *SpeedButton_Halt12;
    TSpeedButton *SpeedButton_Halt13;
    TSpeedButton *SpeedButton_Halt14;
    TSpeedButton *SpeedButton_Halt15;
    TPanel *Panel3;
    TPanel *Panel4;
    TEdit *edInsertOPID_HALT;
    TButton *Button2;
    TPanel *Panel9;
    TPanel *pn_ErrorMsg_HALTStatus;
    TTabSheet *tsPauseStatusForm;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TSpeedButton *sbScreenkeyboard_Pause;
    TLabel *lbStatus_Pause;
    TSpeedButton *SpeedButton_Pause00;
    TSpeedButton *SpeedButton_Pause01;
    TSpeedButton *SpeedButton_Pause02;
    TSpeedButton *SpeedButton_Pause03;
    TSpeedButton *SpeedButton_Pause04;
    TSpeedButton *SpeedButton_Pause05;
    TSpeedButton *SpeedButton_Pause06;
    TSpeedButton *SpeedButton_Pause07;
    TSpeedButton *SpeedButton_Pause08;
    TSpeedButton *SpeedButton_Pause09;
    TSpeedButton *SpeedButton_Pause10;
    TSpeedButton *SpeedButton_Pause11;
    TSpeedButton *SpeedButton_Pause12;
    TSpeedButton *SpeedButton_Pause13;
    TSpeedButton *SpeedButton_Pause14;
    TSpeedButton *SpeedButton_Pause15;
    TPanel *Panel5;
    TPanel *Panel6;
    TEdit *edInsertOPID_Pause;
    TButton *Button1;
    TPanel *Panel7;
    TPanel *pn_ErrorMsg_PauseStatus;
    TTabSheet *tsCompareData;
    TPanel *Panel8;
    TPanel *pnl1;
    TStringGrid *sgDatas;
    TPanel *pnl2;
    TSpeedButton *btnCancel;
    TSpeedButton *btnSummit;
    TTabSheet *tsCalibrateSuckZHeight;
    TPageControl *pcInOutArmSuckZ;
    TTabSheet *tsInArmSuckZ;
    TPanel *Panel12;
    TCheckBox *cbInarmSuckZAutoEnable;
    TPanel *Panel13;
    TGroupBox *GroupBox12;
    TLabel *Label90;
    TLabel *Label91;
    TLabel *Label92;
    TLabel *Label93;
    TLabel *Label94;
    TLabel *Label95;
    TLabel *Label96;
    TLabel *Label97;
    TEdit *setEditZ1A;
    TEdit *setEditZ1B;
    TEdit *setEditZ1C;
    TEdit *setEditZ1D;
    TEdit *setEditZ1E;
    TEdit *setEditZ1F;
    TEdit *setEditZ1G;
    TEdit *setEditZ1H;
    TGroupBox *gbTestAreaInarm;
    TLabel *Label64;
    TLabel *Label65;
    TEdit *edInarmTestAreaX;
    TEdit *edInarmTestAreaY;
    TGroupBox *gbInarmSearchStartZ;
    TLabel *Label5;
    TEdit *edInarmSearchStartZ;
    TTabSheet *tsOutArmSuckZ;
    TPanel *Panel10;
    TGroupBox *GroupBox13;
    TLabel *Label19;
    TLabel *Label23;
    TLabel *Label27;
    TLabel *Label28;
    TLabel *Label30;
    TLabel *Label31;
    TLabel *Label42;
    TLabel *Label43;
    TEdit *setEditZ2G;
    TEdit *setEditZ2E;
    TEdit *setEditZ2C;
    TEdit *setEditZ2A;
    TEdit *setEditZ2H;
    TEdit *setEditZ2F;
    TEdit *setEditZ2D;
    TEdit *setEditZ2B;
    TGroupBox *gbTestAreaOutarm;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *edOutarmTestAreaX;
    TEdit *edOutarmTestAreaY;
    TGroupBox *gbOutarmSearchStartZ;
    TLabel *Label10;
    TEdit *edOutarmSearchStartZ;
    TPanel *Panel11;
    TCheckBox *cbOutarmSuckZAutoEnable;
    TPanel *plBottomArmAutoSuckZ;
    TSpeedButton *SpeedButton1;
    TSpeedButton *btnAutoCalSuckZSave;
    void __fastcall tm_PITimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall bt_PIRefreshMOListClick(TObject *Sender);
    void __fastcall cb_PIChoiseMOKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall bt_PICloseFormClick(TObject *Sender);
    void __fastcall edInsertOPID_HALTClick(TObject *Sender);
    void __fastcall sbScreenkeyboard_HALTClick(TObject *Sender);
    void __fastcall SpeedButton_Halt00Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall SpeedButton_Pause00Click(TObject *Sender);
    void __fastcall PageControl1Change(TObject *Sender);
    void __fastcall tm_IPSCControlTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btnSummitClick(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnAutoCalSuckZSaveClick(TObject *Sender);
private:    // User declarations
    TDateTime dtNowDateTime;
    double dOffsetContactForce;     //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
    WORD h, n, s, z;                //JimmyChiu 20220125 add for time
    AnsiString ACM_WriteMsgAndCallExe_sMessage;//Jimmychiu 20220901 降低ACM交握影響主程序效能
    void SetRTColorSensorEnable(AnsiString snot_RT_BIN);//JimmyChiu 20231127 : Set color sensor parameter by server
    AnsiString sPI_STime;                                                       //Jimmychiu 20240119 : ScheduleName = Mo+Flow
public:     // User declarations
    __fastcall TfProductionInfo(TComponent* Owner);
    void __fastcall DoIniDataToForm();
    void OEE_EndLot();
    void OEE_SetMO(AnsiString sMO);
    void OEE_SetHandlerID(AnsiString sHDID);
    void TimeCount();
    void LoadSetting();
    void SaveSetting();
    void CalculateOEEReport(AnsiString &sResultOEEReport,bool bSaveNow);        //Jimmychiu 20231023 : 非整點製作報表，時間準確到秒
    void CalculateOEEPlanOut();
    void InitialOEECount();
    void ClearOEECount();
    void SetOEEReportMessage();
    void SaveAndUpdateOEEFiles(bool bSaveNow);                                  //Jimmychiu 20231023 : 非整點製作報表，時間準確到秒
    void EachCycleSecondDo_SaveAndUpdateOEEFiles(bool bSaveNow = false);
    void SetNextRecordDateTime();
    void SaveMessageHistroy(AnsiString sAlarmCode,AnsiString sAlarmMessage,int iType,int iSkipLevel);
    void SaveSetupCondition(AnsiString sSaveFileName="");
    void SaveProductionSummaryReport();
    void SaveProductionDataToPath();
    void LoadCSVFileToStringGrid(AnsiString sFilePathName,TStringGrid *sgList,int iColCount);
    void RefreshPIChoiseMO();
    void ShowInformationForm();                                                 //JimmyChiu 20230503 : 新增Infomation顯示function
    void ShowHaltStatusForm();
    void ShowPauseStatusForm();
    int  ShowCompareDataForm();
    void SetStartStatus();
    void ScreenkeyboardShow();                                                  //Sam 20170603 移植超豐 OEE 功能 form HT-7045
    bool LoadHaltAndPauseSelectStatusName();
    void __fastcall OEEFTPList(AnsiString Listing);                             //Sam 20170811 移植超豐 OEE 功能 form HT-7045

    bool OEE_StartLot();
    bool IsOEEStartLotSuccess();
    bool LoadMOInformation(bool bCheckMoInforOnly = false);
    bool SetMOInformation();
    bool CheckMOInformation();
    bool DeleteAllFileInDirectory(AnsiString sDirectoryName);                   //Delete All File In Directory
    bool LoadYiedlInformation();
    bool SetYieldInformation();
    AnsiString OEE_GetMO();
    AnsiString GetCSVLineData(int iDataNum , AnsiString sCSVLineStr);           // 取得CSV格式中的第幾筆資料
    AnsiString GetStringBySeparatedValues(AnsiString sFullFileName,AnsiString sItemValue,AnsiString sSeparatedValues);
    AnsiString MyBoolToString(bool b);                                          //Sam 20170818 移植超豐 OEE 功能 form HT-7045

    int _iOEE_PowerTime;        //Power On時間
    int _iOEE_RunTime;          //Run時間
    int _iOEE_SoakTime;         //Soak時間
    int _iOEE_StopTime;         //Stop時間
    int _iOEE_JamCount;         //Jam次數
    int _iOEE_ServiceCount;     //Error次數
    int _iOEE_sINQty;           //IN Q'ty
    int _iOEE_sBIN[8];          //BIN1~8 = Auto1~3 and Fix1~Fix3        //Sam 20171019 : Bin0~15 改為 Auto1~3 and Fix1~Fix3
    int _iOEE_ActuralOut;       //Actural Out
    int _iOEE_planout;          //plan out
    int _iOEE_PauseTime;

    int iLastINQty;             //IN Q'ty
    int iLastBIN[8];            //Auto1~3 and Fix1~Fix3 //Sam 20171018 : OEE Bug 修正
    int iLastActuralOut;        //Actural Out
    int iplanout;               //plan out
    int iNextDateTime;

    bool bCycleTimeUp;
    bool _bOEEStartLotSuccess;  //Start Lot是否成功
    bool bNeedLotEndAfterCleanOut;
    bool bFTPError;
    bool bIsPauseTime;
    bool bPauseTimeNeedSaveAndUpdateOEEFiles;
    bool bStartNeedSaveAndUpdateOEEFiles;
    bool __fastcall FTP_OEEFile_Upload(AnsiString sSourcesFilePath="", AnsiString sTargetFilePath="", AnsiString sULFileName="");//Sam 20170811 移植超豐 OEE 功能 form HT-7045
    bool __fastcall FTP_MOFile_Download(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFilter="TXT");//Sam 20170811 移植超豐 OEE 功能 form HT-7045

    AnsiString sLocalMOFullFileName;    //Sam 20190227 : Try Catch Mo Download SetupFile
    bool AutoDownloadSetupFileByMO();   //Sam 20190227 : Try Catch Mo Download SetupFile
    bool bAutoDownloadSetupFile;        //Sam 20190227 : Try Catch Mo Download SetupFile
    AnsiString sLoadMO_MO;
    AnsiString sLoadMO_TestTime;
    AnsiString sLoadMO_Temperature;
    AnsiString sLoadMO_SoakTime;
    AnsiString sLoadMO_TestFlow;
    AnsiString sLoadMO_PASSBIN;
    AnsiString sLoadMO_InterfaceType;
    AnsiString sLoadMO_Handler;
    AnsiString sLoadMO_AUTOMOTIVE;          //KaiChen 20171116 ：超豐 人員車規認證 (AUTOMOTIVE)
    AnsiString sLoadMO_YieldLimit;          //Sam 20171212 : 超豐良率監控
    AnsiString sLoadMO_AlarmYield;          //Sam 20171212 : 超豐良率監控
    AnsiString sLoadMO_OSBin;               //Sam 20171212 : 超豐良率監控
    AnsiString sLoadMO_CONTINUE_PASS;       //Sam 20171227 : Add CONTINUE Pass/Fail
    AnsiString sLoadMO_CONTINUE_FAIL;       //Sam 20171227 : Add CONTINUE Pass/Fail
    AnsiString sLoadMO_SetupFileName;       //Sam 20190227 : Try Catch Mo Download SetupFile
    AnsiString sLoadMO_Tester;
    AnsiString sLoadMO_Device;
    //Sam 20180423 (wei) : MOFile of Yeild Download
    //==>
    AnsiString sLoadMOYiled_Qty;
    AnsiString sLoadMOYiled_AVG_Alarm_Yield;
    AnsiString sLoadMOYiled_AVG_Reject_Yield;
    AnsiString sLoadMOYiled_OS_rate;
    AnsiString sLoadMOYiled_OS_rate_Rej;
    AnsiString sLoadMOYiled_Different_Alarm_Yield;
    AnsiString sLoadMOYiled_Different_Alarm_Yield_Rej;
    AnsiString sLoadMOYiled_Yield_Difference;
    AnsiString sLoadMOYiled_Yield_Difference_Rej;
    //Sam 20200525 : Contolr Bin
    //==>
    AnsiString sLoadMOYiled_InterValTime;   //Sam 20201209 : 修改 Control Bin 讀取方式
    AnsiString sLoadMOYiled_InterValCount;  //Sam 20201216 : Add 數量監控
    AnsiString sLoadMOYiled_HW_BinNumber;   //Sam 20201209 : 修改 Control Bin 讀取方式
    AnsiString sLoadMO_TemperatureMode; //Sam 20210423 : MO file 新增 Temperature mode
    AnsiString sLoadMO_RELEASE_SITE;
    AnsiString sLoadMO_LOT;             //JimmyChiu 20211109 : MO file 新增 Lot
    AnsiString sDevice_Pin_Count;       //JimmyChiu 20220114 : MO file 新增 Device Pin Count
    AnsiString sDevice_Pin_Force;       //JimmyChiu 20220114 : MO file 新增 Device Pin Force
    AnsiString sTrayForm;               //JimmyChiu 20220303 : Add class SetUpConfiguration
    AnsiString sSiteMap;                //JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
    AnsiString sBinCategory;            //JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
    AnsiString sNot_RT_BIN;             //JimmyChiu 20231127 : Set color sensor parameter by server

    double dOriginal_Device_Pin_Count;  //JimmyChiu 20220119 : Save Original_Device_Pin_Count
    double dOriginal_Device_Pin_Force;  //JimmyChiu 20220119 : Save Original_Device_Pin_Force

    int iControlBinCheckPoint;      //區間多久計算 Yield
    int iControlBinCheckCount;      //測試多少 IC 後計算 Yield //Sam 20201216 : Add 數量監控
    int iControlBinNum;             //監控 Bin 總數
    int iControlBin[TEST_MAX_BIN];  //監控 Bin
    double dControlPercent[TEST_MAX_BIN];

    int iControlCount[TEST_MAX_BIN];            //Sam 20201216 : Add 數量監控
    double dControlSiteToSite[TEST_MAX_BIN];    //Sam 20201209 : Add by site by bin site to site 比較

    int iControlBinContinueFail[TEST_MAX_BIN];
    double dControlBinYield[MAX_SOCKET_ROW][MAX_SOCKET_COL][TEST_MAX_BIN];//By Site By Bin
    double dControlBinCount[MAX_SOCKET_ROW][MAX_SOCKET_COL][TEST_MAX_BIN];//By Site By Bin
    void UpdateControlBinCount(bool bClear);
    bool bIsControlBinCounting;
    //<==
    //Sam 20200525 : Contolr Bin

    double dTempTajOfs;     //Sam 20200806 : 溫度 By Servo

    //<==
    //Sam 20180423 (wei) : MOFile of Yeild Download
    AnsiString _sOEE_DirectoryName;         //資料夾名稱 YYYYMMDD_HHMMSS_MO
    AnsiString _sOEE_FLOW;                  //FT or RT
    AnsiString _sOEE_MO;                    //此Lot的MO
    AnsiString _sOEE_HandlerID;             //Handler ID
    AnsiString _sOEE_Status;
    AnsiString _sOEE_JamCode;
    AnsiString _sOEE_ActivityID;
    AnsiString _sOEE_TestSite;
    AnsiString sPauseButtonCaption;
    AnsiString sStartDateTime;

    double fLastIndexTime;
    double fLastTestTime;
    double fLastIndexCycleTime;             //Sam 20180802 (wei) : OEE 32Site 修正

    TDateTime _dtOEE_StartLotTime; //Start Lot時間
    TDateTime _dtOEE_EndLotTime; //End Lot時間
    TDateTime _dtOEE_StartDateTime; //Start Lot時間

    bool bOEEFTPAction;
    TStringList *sSourceList;
    TStringList *sTargetSList;
    TStringList *sULFileNameList;
    bool bShow;                 //Sam 20200225 : OEE 功能防呆保護提示顯示在畫面，不要用 MessageBox。
    //Steven 20190621 : OEE Function
    //==>
    void SaveBinQTYReport(AnsiString sUploadFileName);
    void SetLastBinQty();
    TQPF_Timer BinQtyTimer;
    void UploadBinQtyReport(bool bSaveNow=false);
    void UploadSetupCondition();
    void ClearTrayCnt();
    void CalICCountInHandler();
    void CalTrayICCount(int iWitchTray);
    void UpdateTrayCntReport();
    void DoRunExecutFile(AnsiString sExecutFullFileName, AnsiString sParameters="");
    void SocketLifeTimeUpload();
    AnsiString ACM_ReadFlag(AnsiString sName);
    void ACM_WriteMsgAndCallExe(AnsiString sMessage);
    void UploadTrayMappingLog();    //Sam 20201209 : 增加資料上傳
    bool CheckContactForceExist();  //JimmyChiu 20220117 確認Server是否設定，如無設定依照原設定

    TQPF_Timer tIPSCTimer;
    TQPF_Timer tIPSCCycleTimer;
    //Sam 20200525 : Contolr Bin
    //==>
    TQPF_Timer tControlBinTimer;
    int iControlBinTimerCnt;
    void RecordControlBinCount(AnsiString msg);
    void bCheckControlBinYield();
    //<==
    //Sam 20200525 : Contolr Bin

    bool bIsNeedCheckControlBin();  //Sam 20201216 : Add 數量監控

    bool bIPSCFlag_NeedOnecycle;
    bool bIPSCFlag_IsYellowLightON;
    bool bIPSCFlag_IsRedLightON;
    bool bIPSCFlag_AfterOnecycleNeedShowMessage;
    bool bIPSCFlag_IntegerTime;                      //整點旗標
    int iIPSCFlag_CountdownQty;
    int iIPSCFlag_ClearQty;
    int iIPSCFlag_NeedOnecycleStartCount;
    int iIPSCFlag_TriggerMode;                      //0:CycleTime  1:CycleQty

    AnsiString sIPSCFlag_ShowMessage;
    AnsiString sIPSCFlag_AfterOnecycleNeedShowMessage;
    void DoIPSCProcess();

    void SaveAndUpdateIPSCControlFiles();
    void ReadAndDelIPSCFlagIniFile();
    bool bIntegerTimeCheck();         //整點檢查
    bool bTimerCheck(int iTimeMode);         //整點檢查
    //Sam 20200305 : 增加 FTP 檔案檢查，檢查到 VIE_STOP.txt 就執行 OneCycle
    //==>
    TQPF_Timer tCheckFTPFileTimer;
    bool DoCheckFTPFile();
    void __fastcall NMFTPListItem(AnsiString Listing);
    //<==
    //Sam 20200305 : 增加 FTP 檔案檢查，檢查到 VIE_STOP.txt 就執行 OneCycle
    AnsiString sTester;             //測試機
    AnsiString sHandler;            //分類機
    AnsiString sMO;                 //製單
    AnsiString sDevice;             //型號
    AnsiString sNowTime;            //當下時間
    AnsiString sTotalLoadQty;       //總進量
    int iNowUnloaderTrayQty[8];     //Bin1 ~ Bin8
    int iLastUnloaderTrayQty[8];    //Bin1 ~ Bin8

    AnsiString sMaxSiteYield;       //各SITE良率最高者
    AnsiString sMinSiteYield;       //各SITE良率最低者
    AnsiString sSiteOpenOrClose;    //更改為 1101   ( 1為開啟SITE 0 為關閉SITE)
    int iNowArmSiteBinQty   [2][MAX_SOCKET_ROW][MAX_SOCKET_COL][TEST_MAX_BIN];
    int iLastArmSiteBinQty  [2][MAX_SOCKET_ROW][MAX_SOCKET_COL][TEST_MAX_BIN];
    int iNowSiteBinTotalQty [TEST_MAX_BIN];

    void InitialIPSCControlCount();
    void CalculateNowArmSiteBinQty(bool bIsClear=false);
    void CalculateNowUnloaderTrayQty(bool bIsClear=false);
    int CalculateNowTotalICQty();
    int CalculateUnloadTotalICQty();
    void ClearArmSiteBinQty();
    void ClearUnloaderTrayQty();
    int GetArmBySiteFor32Site(int iSite);
    int iOneCycleFinishShowMsg;               //Sam 20250115 : 矽格湖口 GPIB OneCycle 要強制切 ASM //Sam 20221103 : OneCycle 完後顯示訊息
    AnsiString sOneCycleFinishShowMsg;
    void DoOneCycleFinishShowMsg();
    //<==
    //Steven 20190621 : OEE Function
    bool bEnableIPSC();
    bool bEnableRPLog();
    void SaveInfoFileWhenStart();    //Jimmychiu 20211109 : (Upate)Handler 實際運轉Site資料討論
    void CopyIPSCback2IPSC();        //JimmyChiu 20211227 : 讀取IPSC ini file
    void CheckFTPFilePath(AnsiString &aspath); //JimmyChiu 20220111 check FTP Sources File Path
    void CheckNewDayAndSubtract(int &iTimeSec);     //JimmyChiu 20220115 跨日後重頭計算時間
    void SetOffsetContactForce(AnsiString asValue);                             //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
    void SetOffsetContactForce(double dValue){dOffsetContactForce=dValue;};     //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
    double GetOffsetContactForce(){return dOffsetContactForce;};                //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
    void SetControlBin(AnsiString asFullFileName,AnsiString asHW_BinNumber,AnsiString asInterValTime,AnsiString asInterValCount);
    bool CheckOEE_WhenStart(AnsiString &asErrorMsg);                            //JimmyChiu 20220125 add
    void ClickPause();                                                          //JimmyChiu 20220125 add
    int GetNowTimeSec();                                                        //JimmyChiu 20220125 add
    //JimmyChiu 20220303 : Add class SetUpConfiguration
    //<==
    AnsiString asTempstring;
    SetUpConfiguration sucSetUp;
    SetTrayForm_Greatek sucTrayForm;
    //
    bool LoadHotPlateFormFromServer();
    void SettingHotPlateFormFromServer();
    void OverWritePlateFormData(AnsiString FilePath, AnsiString TrayPN, AnsiString XStartPos,
                                 AnsiString YStartPos, AnsiString XPitch, AnsiString YPitch,
                                 AnsiString ColumnsX, AnsiString RowsY, AnsiString XWidth,
                                 AnsiString YHeight, AnsiString ZThickness, AnsiString Group,
                                 AnsiString Memo, AnsiString BlockNumberX, AnsiString BlockNumberY,
                                 AnsiString BlockPitchX, AnsiString BlockPitchY);
    //
    bool LoadTrayFormFromServer(AnsiString asFileName);
    void SettingTrayFormFromServer();
    void SettingTrayFormFromServerOnlyOne();
    //
    bool DoSetUpFileFromServer();
    AnsiString GetProInfoFilePath(){return "D:\\HT9045\\system\\ProductionInfo";}
    AnsiString GetTargetFilePath_SetSiteMap(){return GetProInfoFilePath()+"\\SetSiteMap\\";}
    AnsiString GetTargetFilePath_SetBinCategory(){return GetProInfoFilePath()+"\\SetBinCategory\\";}
    AnsiString GetTargetFilePath_HotPlate(){return "D:\\HT9045\\system\\";}
    AnsiString GetTargetFilePath_TrayForm(){return GetProInfoFilePath()+"\\TrayFormBin\\";}
    AnsiString GetBinTraySetting();
    bool SetBinTraySetting(TStringList* tlBinTray,AnsiString asSource,AnsiString asBin);  //if get "E" return true
    AnsiString GetSetUpName();
    bool CopyFolder(AnsiString asSource,AnsiString asTarget);
    //<==
    //JimmyChiu 20220303 : Add class SetUpConfiguration
    void SetACM_WriteMsgAndCallExe_sMessage(AnsiString sMessage);
    bool bRecFromErrorNote;
    //JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
    //<==
    bool LoadSiteMapFromServer(AnsiString asFileName);
    void SettingSiteMapFromServer();
    bool LoadBinCategoryFromServer(AnsiString asFileName);
    void SettingBinCategoryFromServer();
    bool LoadSetUpFileFromServer(AnsiString sSourcesFilePath,AnsiString asFileName,AnsiString asTargetPath);
    bool DoSetRecipe(AnsiString sSiteMap,AnsiString sBinCategory,AnsiString sTrayForm);
    //<==
    //JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
    //JimmyChiu 20230410 : Config update from server
    //<==
    bool LoadConfigFromServer();
    bool UploadConfigToServer();
    bool UploadSingleFileToServer(AnsiString sSourcesFilePath,AnsiString asFileName,AnsiString asTargetPath);
    bool DownloadFilterFileToServer(AnsiString sSourcesFilePath,AnsiString asFileName,AnsiString asTargetPath);
    //<==
    //JimmyChiu 20230410 : Config update from server
    //JimmyChiu 20230504 : 新增資料比對顯示function
    //<==
    void CloseAllPage();
    int ShowSheet(TTabSheet *tsShow);
    void InitialsgDatas();
    void AddDBColHead(AnsiString asName,int iColWidth=80);
    TList *CompareDataList;
    void ClearCompareDataList();
    bool DoCompareConfigLocalAndCloud();
    void AddData2Grid(AnsiString asContent,AnsiString asLocal,AnsiString asCloud);
    bool bAgreeExportConfig;
    //<==
    //JimmyChiu 20230504 : 新增資料比對顯示function

    //JimmyChiu 20230616 : add IPSC Control
    //<==
    AnsiString sIPSCFlag_ShowMessageAndStop;
    bool bIPSCFlag_ShowMessageAndStop;
    bool bIPSCFlag_NeedPause;
    bool bIPSCFlag_NeedStart;
    //<==
    //JimmyChiu 20230616 : add IPSC Control
    bool IsContinueFailAlarm();                                                 //Jimmychiu 20230919 : Close Continue Fail In RT mode
    void GetFTP_Setting(AnsiString &asUserID,AnsiString &asPassword,AnsiString &asHost);//Jimmychiu 20231223 : add FTP Setting function
    AnsiString GetScheduleName();                                               //Jimmychiu 20240119 : ScheduleName = Mo+Flow
    uTimeTool utimetool;                                                        //Jimmychiu 20231218 : Greatek add error of each position
    AnsiString GetNowTime();                                                    //Jimmychiu 20231218 : Greatek add error of each position
    void SetPISTime();                                                          //Jimmychiu 20231218 : Greatek add error of each position
    AnsiString GetPISTime();                                                    //Jimmychiu 20231218 : Greatek add error of each position
    bool bIPSCPause;                                                            //Jimmychiu 20240220 : Show IPSC Pause
    cDynamicMultiContinualPassBinBySocket* cDynaThres;                          //JimmyChiu 20240411 : Dynamic multiplier for Continual Pass Bin( Socket )
    //Jimmychiu 20240712 : Auto Calibrate Suck Z height
    //<==
    void ShowAutoCalSuckZForm();
    HTEditList *elData;
    void InitAutoCalSuckZ();
    void SaveAutoCalSuckZ();
    void LoadAutoCalSuckZ();
    bool EnableAutoCalSuckZ(){return EnableInArmAutoCalSuckZ() || EnableOutArmAutoCalSuckZ();}
    bool EnableInArmAutoCalSuckZ();
    bool EnableOutArmAutoCalSuckZ();
    //<==
    //Jimmychiu 20240712 : Auto Calibrate Suck Z height
    void InitialStringGrid(TStringGrid *sg);
    bool bN14_21_UsedGDK04;
    int iAutoClean_IntervalContact_Server;
    void SetInsertOPIDStr(const AnsiString& s);
    AnsiString GetInsertOPIDStr();
    AnsiString FilterAlphanumeric(const AnsiString& s);
    bool bN14_21_SetUpConfig_Success;
    bool CheckCloseInfo();
    bool bHadCloseInfo;
    std::vector<int> GetBin0_8List();
};
//---------------------------------------------------------------------------
extern PACKAGE TfProductionInfo *fProductionInfo;
//---------------------------------------------------------------------------
class cDynamicMultiContinualPassBinBySocket                                     //JimmyChiu 20240411 : Dynamic multiplier for Continual Pass Bin( Socket )
{
private:
    int iDynamicThresholdNum;
public:     // User declarations
    cDynamicMultiContinualPassBinBySocket()
    {
        ResetThresholdNum();
    }
    ~cDynamicMultiContinualPassBinBySocket()
    {
        ResetThresholdNum();
    }
    int GetMultiplierNum();
    void AddThresholdNum();
    void ResetThresholdNum()
    {
        iDynamicThresholdNum=0;
    }
};
//---------------------------------------------------------------------------
#endif
