//---------------------------------------------------------------------------

#ifndef fAOIH
#define fAOIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include "HTray.h"
#include "SPComm.hpp"
#include <Grids.hpp>
#include <vector>
#include <SysUtils.hpp>
#include "TfAOILaserScan.h"
#include "HTEditList.h"
#include "aoutarm9045.h"
using namespace std;
//---------------------------------------------------------------------------
#define iAOIFailCheckMax          20
class TTopBottomInspect;
class TFrmAOI : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TCheckBox *cbEnabledTopView;
    TEdit *edt_FailTopView;
    TEdit *edt_TimeoutTopView;
    TEdit *edt_SDelayTopView;
    TGroupBox *GroupBox2;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label18;
    TCheckBox *cbEnabledPADView;
    TEdit *edt_FailPADView;
    TEdit *edt_TimeoutPADView;
    TEdit *edt_SDelayPADView;
    TEdit *edt_FailFailPADView;
    TGroupBox *GroupBox3;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label17;
    TGroupBox *GroupBox4;
    TLabel *Label16;
    TEdit *edt_AlarmCount;
    TCheckBox *cbBGAFailRun;
    TCheckBox *cbAutoSkipBGA;
    TCheckBox *cbAutoSkipPadPkg;
    TCheckBox *cbTesterFailBin;
    TTabSheet *TabSheet2;
    TPanel *Panel1;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TCheckBox *cb_VitContiuneAlarm;
    TGroupBox *GroupBox7;
    TGroupBox *GroupBox5;
    TCheckBox *cb_AlarmBySite;
    TCheckBox *cb_AlarmByArm;
    TGroupBox *GroupBox8;
    TGroupBox *GroupBox9;
    TCheckBox *cb_AlarmBySite1;
    TCheckBox *cb_AlarmByArm1;
    TGroupBox *GroupBox11;
    TLabel *Label19;
    TEdit *edt_ConAlarmConut;
    TLabel *Label20;
    TEdit *edt_ConAlarmConut1;
    TCheckBox *cbEnabledBGAView;
    TEdit *edt_FailBGAView;
    TEdit *edt_SDelayBGAView;
    TEdit *edt_TimeoutBGAView;
    TEdit *edt_FailFailBGAView;
    TComboBox *ComboBox1;
    TPanel *Panel3;
    TEdit *edt_PassFailBGAPADView;
    TEdit *edt_FailFailBGAPADView;
    TLabel *Label21;
    TLabel *Label22;
    TGroupBox *GroupBox6;
    TPanel *SitePanel_Vit;
    TTMyTray *mtDutRow_Vitrox;
    TTMyTray *mtDutCol_Vitrox;
    TTMyTray *mtDutOnOff_Vitrox;
    TGroupBox *GroupBox10;
    TPanel *Panel2;
    TTMyTray *mtDutRow_Vitrox1;
    TTMyTray *mtDutCol_Vitrox1;
    TTMyTray *mtDutOnOff_Vitrox1;
    TCheckBox *cbAOINoSort;
    TTabSheet *TabSheet3;
    TRadioGroup *rgScannerMode;
    TPanel *pnlAOISet;
    TLabel *Label470;
    TLabel *Label471;
    TEdit *edtAOIIntervalCounter;
    TEdit *edtAOIRetryCounter;
    TRadioGroup *rgBallDamageType;
    TComboBox *cbAOIFial;
    TPanel *pnlAOIManual;
    TSpeedButton *sbMoveToScanner;
    TRadioGroup *rgAOISelectZ;
    TLabel *Label468;
    TEdit *edtScannerReadTimeout;
    TLabel *Label469;
    TPanel *pnlAOIBDAlarm;
    TSpeedButton *sbBDTotalReset;
    TSpeedButton *SpeedButton58;
    TSpeedButton *SpeedButton59;
    TSpeedButton *SpeedButton60;
    TCheckBox *cbBDByTotal;
    TCheckBox *cbBDBySiteConti;
    TCheckBox *cbBDBySite;
    TCheckBox *cbBDByTotalConti;
    TEdit *edtAOIBDCounterTotal;
    TEdit *edtAOIBDCounterTotalConti;
    TEdit *edtAOIBDCounterSite;
    TEdit *edtAOIBDCounterSiteConti;
    TPanel *pnlAOIBDTotalLast;
    TPanel *pnlAOIBDTotalContiLast;
    TPanel *pnlAOIBDSite1Last;
    TPanel *pnlAOIBDSite1ContiLast;
    TPanel *Panel74;
    TPanel *pnlAOIBDSite2ContiLast;
    TPanel *pnlAOIBDSite2Last;
    TPanel *pnlBottom;
    TCheckBox *cbDBAlarmAutoReset;
    TTabSheet *TabSheet4;
    TGroupBox *GroupBox12;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label27;
    TComboBox *cbBaudRate;
    TComboBox *cbByteSize;
    TComboBox *cbStopBit;
    TComboBox *cbParity;
    TComboBox *cbDevice;
    TPanel *Panel4;
    TPanel *pnlManual;
    TSpeedButton *sbSend;
    TSpeedButton *sbClear;
    TEdit *edtManual;
    TPageControl *pcLog;
    TTabSheet *tsString;
    TMemo *MemoLogString;
    TTabSheet *tsHex;
    TMemo *MemoLogHex;
    TComm *AOIComm;
    TSpeedButton *spbStartCom;
    TSpeedButton *spbStopCom;
    TLabel *Label28;
    TEdit *edt_SDelayScanAOI;
    TLabel *Label29;
    TLabel *Label30;
    TEdit *edt_TimeoutScanAOI;
    TLabel *Label31;
    TGroupBox *GroupBox13;
    TCheckBox *cb_ScanAOIAlarmBySite;
    TCheckBox *cb_ScanAOIAlarmByArm;
    TLabel *Label32;
    TEdit *edt_ScanAOIAlarmCountBySite;
    TLabel *Label33;
    TEdit *edt_ScanAOIAlarmCountByArm;
    TPanel *Panel76;
    TSpeedButton *sbAOISend;
    TRadioGroup *rgAOISiteMode;
    TLabel *Label34;
    TEdit *edtScannerICGain;
    TCheckBox *cb_ScanAOIUseLGAMode;
    TLabel *Label35;
    TEdit *edtScannerLGAWaitTime;
    TTMyTray *mtAOIBuffer;
    TLabel *Label36;
    TTabSheet *TabSheet5;
    TGroupBox *GroupBox14;
    TLabel *Label38;
    TLabel *Label39;
    TCheckBox *cb_TopScanAOIAlarmBySite;
    TCheckBox *cb_TopScanAOIAlarmByArm;
    TEdit *edt_TopScanAOIAlarmCountBySite;
    TEdit *edt_TopScanAOIAlarmCountByArm;
    TPanel *Panel5;
    TLabel *Label37;
    TLabel *Label40;
    TLabel *Label41;
    TEdit *edtTopAOIIntervalCounter;
    TEdit *edtTopAOIRetryCounter;
    TComboBox *cbTopAOIFial;
    TRadioGroup *rgTopScannerMode;
    TLabel *Label42;
    TLabel *Label43;
    TLabel *Label44;
    TCheckBox *cb_TopScanAOIUnUseFailBin;
    TEdit *edtTopScannerReadTimeout;
    TEdit *edt_TimeoutTopScanAOI;
    TEdit *edt_SDelayTopScanAOI;
    TLabel *Label45;
    TLabel *Label46;
    TLabel *Label47;
    TTabSheet *TabSheet6;
    TGroupBox *GroupBox15;
    TLabel *Label48;
    TLabel *Label49;
    TLabel *Label50;
    TLabel *Label51;
    TLabel *Label52;
    TSpeedButton *spbTopStartCom;
    TSpeedButton *SpeedButton2;
    TComboBox *cbTopBaudRate;
    TComboBox *cbTopByteSize;
    TComboBox *cbTopStopBit;
    TComboBox *cbTopParity;
    TComboBox *cbTopDevice;
    TPanel *Panel6;
    TSpeedButton *sbTopAOISend;
    TRadioGroup *rgTopAOISiteMode;
    TPanel *Panel7;
    TPanel *Panel8;
    TSpeedButton *sbTopSend;
    TSpeedButton *sbTopClear;
    TEdit *edtTopManual;
    TPageControl *PageControl2;
    TTabSheet *TabSheet7;
    TMemo *TopMemoLogString;
    TTabSheet *TabSheet8;
    TMemo *TopMemoLogHex;
    TCheckBox *cb_ScanAOIUnUseFailBin;
    TRadioButton *rbSetFocus;
    TComm *TopAOIComm;
    TCheckBox *cb_LGAModeDirection;
    TCheckBox *cb_EnabledPositionByAOI;
    TRadioGroup *rgAOIFailBinType;
    TLabel *lblAOIBinSel1;
    TLabel *lblAOIBinSel2;
    TComboBox *cbAOIFialAndTestPass;
    TTabSheet *tsTopBtnModSet;
    TTabSheet *tsTopBtnCommSet;
    TPanel *Panel17;
    TPanel *Panel15;
    TPanel *Panel13;
    TLabel *Label55;
    TLabel *Label57;
    TEdit *edStartDelayTimeTBAOI;
    TPanel *Panel10;
    TLabel *Label56;
    TLabel *Label58;
    TEdit *edTBTimeout;
    TRadioGroup *rgActionMode;
    TPanel *Panel29;
    TLabel *Label68;
    TLabel *Label69;
    TEdit *edGetResultDelay;
    TButton *btSaveReport;
    TRadioGroup *rgTopBtmScannEnable;
    TPageControl *PageControl4;
    TTabSheet *TabSheet11;
    TPanel *Panel19;
    TPanel *Panel20;
    TLabel *Label59;
    TEdit *edCamaName;
    TPanel *Panel26;
    TLabel *Label60;
    TLabel *Label61;
    TEdit *edRotate0;
    TEdit *edRotate180;
    TPanel *Panel27;
    TLabel *Label62;
    TLabel *Label63;
    TEdit *edTopBtnCenterX;
    TEdit *edTopBtnCenterY;
    TGroupBox *GroupBox20;
    TLabel *Label64;
    TLabel *Label65;
    TEdit *edLightZTop;
    TEdit *edLightZBtm;
    TGroupBox *GroupBox21;
    TEdit *edZoffset;
    TPanel *Panel30;
    TGroupBox *GroupBox19;
    TLabel *Label66;
    TLabel *Label67;
    TEdit *edCCDZTop;
    TEdit *edCCDZBtm;
    TGroupBox *GroupBox31;
    TLabel *Label79;
    TLabel *Label80;
    TEdit *edRotateKitAngOffset_In;
    TEdit *edRotateKitAngOffset_Out;
    TPanel *Panel21;
    TGroupBox *GroupBox17;
    TStringGrid *sgICSmall;
    TGroupBox *GroupBox18;
    TStringGrid *sgICLarge;
    TTabSheet *TabSheet12;
    TLabel *Label70;
    TLabel *Label71;
    TLabel *Label72;
    TLabel *Label73;
    TLabel *Label74;
    TLabel *Label75;
    TLabel *Label76;
    TLabel *Label77;
    TLabel *Label78;
    TCheckBox *cbConsecutiveFail;
    TEdit *edConsecutiveFailCount;
    TCheckBox *cbIntervalCheck;
    TEdit *edIntervalCount;
    TEdit *edAOIFailSetBin;
    TCheckBox *cbConsecutiveFailPicture;
    TEdit *edConsecutiveFailPictureCount;
    TCheckBox *cbAccumulatedFail;
    TEdit *edAccumulatedFailCount;
    TCheckBox *cbAOIBin;
    TGroupBox *GroupBox16;
    TLabel *lbTaskLDC;
    TPanel *Panel9;
    TPanel *pConnect;
    TLabel *Label54;
    TLabel *Label53;
    TBitBtn *btnTopBtm_Connect;
    TEdit *edTopBtm_Address;
    TEdit *edTopBtm_Port;
    TPanel *Panel11;
    TButton *btnSimulateTopBtm;
    TPanel *Panel28;
    TCheckBox *cbCycleRun;
    TPanel *pPageRight;
    TMemo *mTopBtmCommLog;
    TPageControl *PageControl3;
    TTabSheet *TabSheet13;
    TPanel *Panel14;
    TPanel *Panel16;
    TCheckBox *cbSkipAOI;
    TPanel *Panel22;
    TCheckBox *cbSkipMotor;
    TPanel *Panel24;
    TButton *btnCmmStartLot;
    TButton *btnCmmEndLot;
    TButton *btnStart;
    TButton *btnPause;
    TPanel *Panel25;
    TCheckBox *cbICBig;
    TPanel *Panel23;
    TPanel *Panel12;
    TLabel *blTopBtmTask;
        TScrollBox *scroAOIFailCountLinkLotRunMode;
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall edt_FailTopViewClick(TObject *Sender);
    void __fastcall edt_SDelayTopViewClick(TObject *Sender);
    void __fastcall DrawSitePanelVitrox1();
    void __fastcall DrawSitePanelVitrox2();
    void __fastcall mtDutOnOff_Vitrox1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall mtDutOnOff_VitroxMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbTesterFailBinMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall spbStopComClick(TObject *Sender);
    void __fastcall spbStartComClick(TObject *Sender);
    void __fastcall sbSendClick(TObject *Sender);
    void __fastcall sbClearClick(TObject *Sender);
    void __fastcall AOICommReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall sbBDTotalResetClick(TObject *Sender);
    void __fastcall sbAOISendClick(TObject *Sender);
    void __fastcall edtScannerICGainMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edtScannerLGAWaitTimeMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall mtAOIBufferMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edt_AlarmCountClick(TObject *Sender);
    void __fastcall edtAOIRetryCounterClick(TObject *Sender);
    void __fastcall edtScannerReadTimeoutClick(TObject *Sender);
    void __fastcall rgAOIFailBinTypeClick(TObject *Sender);
    void __fastcall btnTopBtm_ConnectClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btnSimulateTopBtmClick(TObject *Sender);
    void __fastcall sgICSmallDblClick(TObject *Sender);
    void __fastcall btnCmmStartLotClick(TObject *Sender);
    void __fastcall btnCmmEndLotClick(TObject *Sender);
    void __fastcall btnStartClick(TObject *Sender);
    void __fastcall btnPauseClick(TObject *Sender);
    void __fastcall btSaveReportClick(TObject *Sender);

private:    // User declarations
    int iQuotient;                                                              //KenHsieh 20220830 : 修改add:AOI Tray Edit
    int iRemainder;                                                             //KenHsieh 20220830 : 修改add:AOI Tray Edit
    void __fastcall UpdateAOIFailBinTypetoForm();                               //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    HTEditList* elParameter;                                                    //AI(general) 20260411 (Rogeryang) : init NULL, 避免非TopBottom模式野指標
    void IntialParameter();
    void CloseAllPage();
    void __fastcall AOIConsecutiveFailPictureLabel_Click(TObject *Sender);
public:        // User declarations
    __fastcall TFrmAOI(TComponent* Owner);

    void fAOI_ReadFile();
    void fAOI_DoIniDataToForm();
    void InitAOIFunction();
    bool CheckFailBin();
    void __fastcall RS232Init();
    void ShowCommData(AnsiString sType , AnsiString sData, int iNum=0);         //Ifor 20200908 add:可使用多台Scan AOI 裝置
    void SendCommand(AnsiString sData, int iNum=0);                             //Ifor 20200908 add:可使用多台Scan AOI 裝置
    void GetScannerResult( AnsiString sData);
    void __fastcall TriggerAOISystem(bool bSite1, bool bSite2, int iNum=0);     //Ifor 20200908 add:可使用多台Scan AOI 裝置
    void __fastcall CheckBallDamageCounter();
    void __fastcall DoContinuousFailByArmScanAOI(bool Res, int ArmSide, int iNum=0);
    void __fastcall DoContinuousFailBySiteScanAOI(bool Res, int iRow, int iCol, int iNum=0);
    void __fastcall UpDataTrayData();

    //Jimmychiu 20240322 : Top & Bottom Inspect
    //<==
    bool fShow;
    TTopBottomInspect *ttbInsp;
    void UpdateConnectionState(TBitBtn * btn);
    bool bSimulate;
    bool bSimuSingleComm;
    bool RunTopBottomInspect();
    void ShowMsgTopBtmCommLog(AnsiString asStr);
    void RecordMsgTopBtmComm(AnsiString asMsg);
    void ShowTopBtnTask(AnsiString sMsg);
    void DoTopBottomInspectSpin();
    TQPF_Timer DoSpinDelay;
    TQPF_Timer DoManualTimeOut;
    void SetSimulationState(bool bswitch=false);
    bool GetSimulationState(){return bSimulate;}
    AnsiString sTopBtmInspErrMsg;
    void InitialTopBtmSG(TStringGrid* sg);
    void InitialTopBtmSGTitle(TStringGrid* sg);
    AnsiString GetNewLineStr(){return "\r\n";}
    bool DoSingleComm(bool bFirst,int iType);
    int iDoSingleComm;
    enum eSingleComm
    {
        eSC_StartLot=0,
        eSC_EndLot,
        eSCTotal
    };
    TEdit* edlsSamll[2][6];
    TEdit* edlsLarge[4][6];
    bool bManualSend;
    void InitialEdList(TEdit* (*edl)[6],int iRow,int iCow);
    AnsiString GetStringGridTitle(int iCol);
    bool GetRotateIs0();
    void SetICSmallGrid(int iRow,int iCol,int iValue);
    void SetICLargeGrid(int iRow,int iCol,int iValue);
    bool SendCommandManual(int iCmd,bool bfirst);
    bool CheckAirPressureIsEnough();
    bool bCheckAOIFailBinUse();                                                 //Eastsun 20260318 : 新增AOI即時良率
    int iConFailCount;                                                            //Eastsun 20260515 F023: E4 AOI fail counter
    int iConFailCountPicture[4];                                                  //Eastsun 20260515 F023: E4 AOI fail counter
    int iAccumulatedCount;                                                        //Eastsun 20260515 F023: E4 AOI fail counter
    bool bSimulateTopBtm;                                                         //Eastsun 20260515 F023: E2 lock simulate during run
    struct TAOIFailCountComboItem                                               //Eastsun 20260401 : 客戶要求RunMode 分開設定
    {
        TCheckBox* cbConsecutiveFail;
        TEdit* edConsecutiveFailCount;
        TCheckBox* cbIntervalCheck;
        TEdit* edIntervalCount;
        TCheckBox* cbConsecutiveFailPicture;
        TEdit* edConsecutiveFailPictureCount;
        TCheckBox* cbAccumulatedFail;
        TEdit* edAccumulatedFailCount;
        AnsiString asRunMode;
    };
    std::vector<TAOIFailCountComboItem> AOIFailList;                            //Eastsun 20260401 :
    void AOIFailCountAddRow(AnsiString runMode,int RowCount);                   //Eastsun 20260401 :
    void AOIFailCountRefresh();                                                 //Eastsun 20260401 :
    //<==
    //Jimmychiu 20240322 : Top & Bottom Inspect
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAOI *FrmAOI;

extern void InitAOIFunction();
extern bool DoAOIFunction();
//extern bool bPickSH1Flag;                                                     //Alick 20160714  //Ifor 20191226 移至全域
//extern bool bPickSH2Flag;                                                     //Alick 20160714  //Ifor 20191226 移至全域
extern bool bBGABySiteFailCont;                                                 //Alick 20160714
extern bool bBGAByArmFailCont;                                                  //Alick 20160714
extern bool bPADBySiteFailCont;                                                 //Alick 20160714
extern bool bPADByArmFailCont;                                                  //Alick 20160714
//int iScannerTask;
extern int iAOIResult[2];
extern bool bTriggerAOI;
extern int iScanAOIIntervalCounter;
extern bool bScanAOIBySiteFailCont;
extern bool bScanAOIByArmFailCont;
extern bool bScanAOIByArmDouChk[2];
extern int  iScanAOIByArmTotal[2];

extern int iTopViewTask;
extern int iTopViewInspectionTask;
extern int iPADViewTask;
extern int iPADViewInspectionTask;
extern int iBGAViewTask;
extern int iBGAViewInspectionTask;

extern int iScanAOITask;
extern int iScannerAOIInspectionTask;
extern int iAOITask;

extern int iTopScanAOIIntervalCounter;
extern bool bTopScanAOIBySiteFailCont;
extern bool bTopScanAOIByArmFailCont;
extern bool bTopScanAOIByArmDouChk[2];
extern int  iTopScanAOIByArmTotal[2];
extern void InitTopScannerAOIInspection();
extern bool DoTopScanAOIFunction_Inspection(int &iRow, int &iCol);
extern int  GetAOIFailBin(int iRow, int iCol);                                  //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
//---------------------------------------------------------------------------
class TMotorXC100                                                               //Jimmychiu 20240322 : Top & Bottom Inspect
{
public:     // User declarations
    TMotorXC100(){}
    ~TMotorXC100(){}
};
//---------------------------------------------------------------------------
class TTopBottomInspect                                                         //Jimmychiu 20240322 : Top & Bottom Inspect
{
private:    // User declarations
    //---------------------------------------------------------------------------
    typedef struct
    {
        int iPosName;
        AnsiString CamName;
        AnsiString Timestamp;
        AnsiString LotID;
        AnsiString ArmSite;
        AnsiString BinResult;
        AnsiString TouchDown;
        AnsiString DeviceInfo;
        int iGrabIndex;
    } t_TopBtnInspect;
    typedef struct
    {
        uPoint2D ppPosition;
        bool bClampLT;
        bool bClampLB;
        bool bClampRT;
        bool bClampRB;
        void Clear()
        {
            ppPosition.Clear();
            bClampLT=false;
            bClampLB=false;
            bClampRT=false;
            bClampRB=false;
        }
    } t_TopBtnPhotoInfo;
    typedef struct
    {
        AnsiString Timestamp;
        AnsiString TrayID;
        AnsiString Bin_out_location;
    } t_BinOutInfo;
    enum ePosCama
    {
        ePC_Top=0,
        ePC_Bottom,
        ePCTotal
    };
    enum eClampName
    {
        eCN_TopLeft=0,
        eCN_TopRight,
        eCN_BottomLeft,
        eCN_BottomRight,
        eCN_Total
    };
    enum eICSize
    {
        eICS_LessThanOrEqual65mm=0,
        eICS_MoreThan65mm,
        eICS_Total
    };
    enum eGrabIndex
    {
        eGI_1_2=0,  //1/2
        eGI_2_2=0,  //2/2
        eGI_1_4,    //1/4
        eGI_2_4,    //2/4
        eGI_3_4,    //3/4
        eGI_4_4,    //4/4
        eGI_Total
    };
    enum eSuckStatus   //0:running 1:finish 2:skip 3:retry
    {
        ess_Running=0,
        ess_Finish,
        ess_Skip,
        ess_Retry,
        eSS_Total
    };

    enum eAOIActionMode                  //Frank 20230419
    {   eTopBottom   =0,
        eBottom      =1,
    };
    //---------------------------------------------------------------------------
    TAOISocket  *socketAOI;
    TStringList *tsAnalysis;
    AnsiString asSendMsg;
    AnsiString asRecMsg;
    AnsiString sErrorMsg;
    AnsiString sTemp;
    TPointVoidShowMsg RecMsg;
    AnsiString GetAOIReturnError(){return "TopBottomInspect return Error";}
    AnsiString GetReceiveWrongCommand(){return "Receive wrong command";}
    int iTakePhotoTask;
    int iTopBtmInspFuncTask;
    int iTopBtmInspFuncInspTask;
    int iPhotosCenterposTask;
    int iPhotos4posTask;
    int iMoveFixedSeatAndCCD2ReadyTask;
    int iSimulationTopBtmInspTask;
    int iIC_Size;
    void SetSimuICInfo();
    TQPF_Timer tTimeOut;
    int GetTimeOutSec(){return iTimeout;};
    int iRetryCount;
public:     // User declarations
    TTopBottomInspect();
    ~TTopBottomInspect();
    //enum
    enum ePhotoPos
    {
        ePP_TopLeft=0,
        ePP_TopRight,
        ePP_BottomLeft,
        ePP_BottomRight,
        ePP_Center,
        ePP_Total
    };
    //parameter
    int iEnable;
    int iAction;

    int iStartDelayTime;
    int iGetResultDelay;
    int iTimeout;
    AnsiString sSocketAddress;
    AnsiString sSocketPort;
    bool bSimuAOICommand;
    bool bSimuMot;
    bool bCycleRun;
    AnsiString asErrorMsg;
    t_TopBtnInspect t_ti;
    t_BinOutInfo t_bi;
    AnsiString sCamaName;
    t_TopBtnPhotoInfo tbpiLessThanOrEqual65mm[2];
    t_TopBtnPhotoInfo tbpiMoreThan65mm[4];
    t_TopBtnPhotoInfo tbCenter;
    int iRotate0Pos;
    int iRotate180Pos;
    int iCCD_Up_Z;
    int iCCD_Up_Z_Top;
    int iCCD_Up_Z_Btm;
    int iLight_Up_Z_Top;
    int iLight_Up_Z_Btm;

    int iRotateKitAngOffset_In;                                                 //Eastsun 20260305 : RoataeKit Offset
    int iRotateKitAngOffset_Out;                                                //Eastsun 20260305 : RoataeKit Offset

    int iZPickOffset;

    bool bIntervalCheck;
    bool bConsecutiveFailCheck;
    bool bAOIFailBin;                                                           //Eastsun 20260319 : AOI Bin
    int iIntervalCount;
    int iConsecutiveFailCount;

    bool bConsecutiveFailPictureCheck;
    int iConsecutiveFailPictureCount;

    bool bAccumulatedFailCheck;
    int iAccumulatedFailCount;

    int iAOIFailSetBin;
    int SuckActiveCount;
    //command
    bool DoStartInspect(bool bIsFirst);
    bool DoImagingCompleted(bool bIsFirst);
    bool DoInquireResult(bool bIsFirst, int iStep=0, int iRetry=0);
    bool DoRecordResult();
    bool DoBinOutInfo(bool bIsFirst);
    bool DoStartLot(bool bIsFirst,AnsiString sLotID);
    bool DoEndLot(bool bIsFirst);
    bool DoTestLighting(bool bIsFirst,int iRetryCount);
    bool DoCamaChange(bool bIsFirst);
    bool DoAutoRun(bool bIsFirst);
    bool DoPause(bool bIsFirst);
    bool DoRecipeChange(bool bIsFirst);
    bool DoTimeSynchronization(bool bIsFirst);
    bool DoResetFlow(bool bIsFirst,int iRetry=0);
    //Tool
    bool SetCommParameter(AnsiString asAddress,AnsiString asPort){return socketAOI->SetCommParameter(asAddress,asPort);}
    AnsiString GetTimestamp();
    AnsiString GetInspectCommand(t_TopBtnInspect t);
    AnsiString GetBinOutInfoCommand(t_BinOutInfo t);
    void SetRecordMsg(TPointVoidShowMsg rm){RecMsg=rm;}
    void RecordMsg(AnsiString str);
    void RecordErrorMsg(AnsiString str);
    AnsiString GetErrorMsg(AnsiString asFunName,AnsiString asErrorType,AnsiString asContent);
    int GetIC_Size();
    void SetIC_Size(int is);//0:<=65mm 1:>65mm
    bool SetSuckActive(int iStation);
    bool GetNeedActSuck(int &iSuckRow,int &iSuckCol);
    AnsiString DoSendCommand(bool bIsFirst,AnsiString asCommand);
    void ReceiveData(char* cGet,int iLen);
    void SendCommandSocket(AnsiString asCommand);
    bool DoCommuncation();
    bool IsCommActive();
    void CommClose(){socketAOI->Close();}
    bool GetRotateIs0();
    bool GetRotateIs180();
    AnsiString GetGrabIndex(int iGrpIdx);
    AnsiString GetTaskInfo();
    bool IsOutArmSuckArrival(int iSuckRow,int iSuckCol,int iPlace);
    bool CheckAirPressureIsEnough();
    bool DoCylinder(int iCylin,bool bOn);
    //Action
    bool DoTopBtmInspFunc(bool bIsFirst);
    bool DoTopBtmInspFunc_Inspection(bool bIsFirst);
    bool DoClamp(int iPos,bool bOn);
    bool DoRotate(int iDeg, bool bReset=false);
    bool DoCamaDown(bool bDowm);
    bool DoCCDLightDown(bool bDowm, bool bReset=false);
    bool DoMoveFixedSeatXY(int iX,int iY);
    bool DoMoveOutArmSeatXY(int iX,int iY);
    bool DoMoveFixedSeatXY2RotatePos();
    bool DoTakePhoto(bool bIsFirst);
    bool DoPhotosCenterpos(bool bIsFirst);
    bool DoPhotos4pos(bool bIsFirst);
    bool DoMoveXYZ2AOI(int iSuckRow,int iSuckCol,bool bPlace);
    bool DoMoveXYZ2Sht2(int iSuckRow,int iSuckCol,bool bPlace);
    bool DoMoveXY2SafePos();
    int  DoSuckDestroyVacuum(int iSuckRow,int iSuckCol);        //0:running 1:finish 2:skip 3:retry
    int  DoSuckCreateVacuum(int iSuckRow,int iSuckCol);         //0:running 1:finish 2:skip 3:retry
    bool DoMoveFixedSeatAndCCD2Ready(bool bIsFirst,bool &bError);
    bool DoSimulationTopBtmInspFunc(bool bIsFirst,AnsiString &sErr);
    bool DoMoveFixedSeatXYandClamp(int iIC_Size,int iStep);
    bool DoLockRotate(bool bOn);
    bool DoSht2ToRight();
    int DoOutarm2Sht2PickPlaceIC(int iSuckRow,int iSuckCol,bool bPlace);        //0:running 1:finish 2:skip 3:retry
    int DoOutarm2AOI_PickPlaceIC(int iSuckRow,int iSuckCol,bool bPlace);        //0:running 1:finish 2:skip 3:retry

    void AddAOIRecord(AnsiString asPass, AnsiString asArea, int iwhichAuto, AnsiString asErrorCode, int iNo);
    int iAOIYieldTotalCount;                                                  //Eastsun 20260318 : 新增AOI即時良率
    int iAOIYieldPassCount;
    bool bConsecutiveFailCheck_List[iAOIFailCheckMax];                                        //Eastsun 20260402 :
    int iConsecutiveFailCount_List[iAOIFailCheckMax];                                         //Eastsun 20260402 :
    bool bIntervalCheck_List[iAOIFailCheckMax];                                               //Eastsun 20260402 :
    int iIntervalCount_List[iAOIFailCheckMax];                                                //Eastsun 20260402 :

    bool bConsecutiveFailPictureCheck_List[iAOIFailCheckMax];                                 //Eastsun 20260402 :
    int iConsecutiveFailPictureCount_List[iAOIFailCheckMax];                                  //Eastsun 20260402 :

    bool bAccumulatedFailCheck_List[iAOIFailCheckMax];                                        //Eastsun 20260402 :
    int iAccumulatedFailCount_List[iAOIFailCheckMax];                                         //Eastsun 20260402 :
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
