//---------------------------------------------------------------------------

#ifndef SCK_ARTH
#define SCK_ARTH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "ALed.hpp"
#include <ComCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdIPWatch.hpp>
#include <ScktComp.hpp>
#include <NMFtp.hpp>
#include <Psock.hpp>

//---------------------------------------------------------------------------
class TfSCKART : public TForm
{
__published:    // IDE-managed Components
    TPanel *palARTLeft;
    TPanel *palARTSetting;
    TGroupBox *gbARTMode;
    TLabel *lblSortMethod;
    TComboBox *cbSortMode;
    TLabel *lblRTStartMode;
    TComboBox *cbRTStartMode;
    TGroupBox *gbSocketOff;
    TLabel *lblSocketOffMode;
    TComboBox *cbRTOffMode;
    TLabel *lblBestYieldRate;
    TEdit *edBestYield;
    TCheckBox *cbAutoSocketOff;
    TGroupBox *gdAlarmList;
    TMemo *memoAlarmList;
    TButton *btnDeleteLotInfo;
    TTimer *TimerSCKARTFlow;
    TPanel *palARTCount;
    TGroupBox *gbCountInfo;
    TLabel *lblInputPickCount;
    TLabel *lblInShtCount;
    TLabel *lblOutCnt;
    TLabel *lblMergeCount;
    TLabel *lblGap;
    TLabel *lblInputQty;
    TLabel *lblInputJamCount;
    TLabel *lblOutputJamCount;
    TPanel *palRejectCnt;
    TPanel *palOutputCnt;
    TPanel *palInShtCnt;
    TPanel *palInputCnt;
    TPanel *palOutputGap;
    TPanel *palInShtCntGap;
    TPanel *palInputCount;
    TPanel *palInputJamCount;
    TPanel *palOutputJamCount;
    TGroupBox *gbLotStatus;
    TLabel *lblTestCount;
    TLabel *lblLotNumber;
    TLabel *lblMesInputQty;
    TLabel *lblHandlerStatus;
    TLabel *lblTesterType;
    TLabel *lblRTTryCount;
    TLabel *lblPassRate;
    TLabel *lblCurrentRate;
    TPanel *palTestCnt;
    TPanel *palLotNumber;
    TPanel *palInputQty;
    TPanel *palLotStatus;
    TPanel *palTesterType;
    TPanel *palRTTryCnt;
    TComboBox *cbLotStatus;
    TPanel *palARTPassRate;
    TEdit *edlRTTryCnt;
    TEdit *edARTPassRate;
    TPanel *palCurrentRate;
    TEdit *edLotCount;
    TGroupBox *gbUserFix;
    TLabel *lblInputJamCountFix;
    TLabel *lblInputRemoveCountFix;
    TLabel *lblOutputJamCountFix;
    TLabel *lblOutputRemoveCountFix;
    TSpeedButton *btnApplyCount;
    TPanel *palInRemoveCnt;
    TPanel *palInputJamCnt;
    TPanel *palOutRemoveCnt;
    TPanel *palOutputJamCnt;
    TEdit *edTemp;
    TEdit *edtInputCount;
    TPageControl *pgARTFlow;
    TTabSheet *ts93KFlow;
    TTabSheet *tsFlexFlow;
    TGroupBox *grpFlexARTFlow;
    TALed *ledFlexInitART;
    TALed *ledFlexFTQTY;
    TALed *ledFlexLotClear;
    TPanel *palFlexInitART;
    TPanel *palFlexFTQTY;
    TPanel *palFlexLotClear;
    TPanel *palFlexLotQty;
    TALed *ledFlexLotQty;
    TPanel *palFlexFTTesting;
    TALed *ledFlexFTTesting;
    TPanel *palFlexFTStsL;
    TALed *ledFlexFTStsL;
    TPanel *palFlexFTAlarm;
    TALed *ledFlexFTAlarm;
    TPanel *palFlexFTStsR;
    TALed *ledFlexFTStsR;
    TPanel *palFlexLotRTClear;
    TALed *ledFlexLotRTClear;
    TGroupBox *grp93KARTFlow;
    TALed *aledInitART;
    TALed *aledFTLotStart;
    TALed *aledWaitingTestercommand1;
    TALed *aledFTTesting;
    TALed *aledFTLotEnd;
    TALed *aledWaitingTestercommand2;
    TALed *aledRTLotStart;
    TALed *aledWaitingTestercommand3;
    TALed *aledRTTesting;
    TALed *aledRTLotEnd;
    TALed *aledWaitingTestercommand4;
    TALed *aledARTFinish;
    TALed *aledMoveTrayToLoader;
    TPanel *paInitART;
    TPanel *paFTLotStart;
    TPanel *paWaitingTestercommand1;
    TPanel *paFTTesting;
    TPanel *paFTLotEnd;
    TPanel *paWaitingTestercommand2;
    TPanel *paRTLotStart;
    TPanel *paWaitingTestercommand3;
    TPanel *paRTTesting;
    TPanel *paRTLotEnd;
    TPanel *paWaitingTestercommand4;
    TPanel *paARTFinish;
    TPanel *paARTMoveTray;
    TPanel *palFlexRTTesting;
    TALed *ledFlexRTTesting;
    TPanel *palFlexRTStsL;
    TPanel *palFlexRTAlarm;
    TPanel *palFlexRTStsR;
    TALed *ledFlexRTStsL;
    TALed *ledFlexRTAlarm;
    TALed *ledFlexRTStsR;
    TALed *ledFlexStsF;
    TPanel *palFlexStsF;
    TPanel *palFlexRTQTY;
    TALed *ledFlexRTQTY;
    TSpeedButton *btnApplyQty;
    TSpeedButton *btnApplySetting;
    TSpeedButton *btnExit;
    TSpeedButton *btnApplyLotInfo;
    TSpeedButton *btnExit1;
    TGroupBox *grpARTSetting;
    TCheckBox *chkEnableART;
    TEdit *edtVersion;
    TCheckBox *chkVersion;
    TGroupBox *gbAutoSkip;
    TLabel *lblAutoSkipCount;
    TEdit *edARTAutoSkip;
    TCheckBox *cbAutoSkip;
    TGroupBox *gbOutputLoader;
    TLabel *lblAuto1;
    TLabel *lblAuto2;
    TLabel *lblAuto3;
    TPanel *palAutoTrayCnt1;
    TPanel *palAutoTrayCnt2;
    TPanel *palAutoTrayCnt3;
    TCheckBox *cbSPBinAlarm;
    TComboBox *cbSPBinSelect;
    TEdit *edSPBinYield;
    TLabel *lblYield;
    TCheckBox *chkNoTesterCmd;
    TLabel *lblProcessCode;
    TPanel *pnlProcessCode;
    TServerSocket *srvrscktTSV;
    TTimer *TimerTSV;
    TCheckBox *chkLowYieldForFT;
    TEdit *edtLowYieldForFT;
    TEdit *edtAlmAutoCloseSite;
    TLabel *lblAlmSiteLess;
    TEdit *edtMRejectCnt;
    TLabel *lblManul;
    TPanel *palManualRejectCnt;
    TGroupBox *gbDevice;
    TCheckBox *cbAutoCleanOut;
    TCheckBox *cbAutoLotEnd;
    TCheckBox *cbFinalLotEndMsg;
    TCheckBox *cbLotDeviceCheck;
    TCheckBox *chkAlarmKIND2;
    TCheckBox *chkEnableFTCT;
    TALed *ledShowFTCTData;
    TButton *btnFTCTReset;
    TLabel *lblLdCntLimP;
    TEdit *edtLdCntLimP;
    TEdit *edtLdCntLimN;
    TLabel *lblLdCntLimN;
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnApplyCountClick(TObject *Sender);
    void __fastcall palInputJamCntClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall edlRTTryCntClick(TObject *Sender);
    void __fastcall edARTPassRateClick(TObject *Sender);
    void __fastcall edLotCountClick(TObject *Sender);
    void __fastcall edARTAutoSkipClick(TObject *Sender);
    void __fastcall btnDeleteLotInfoClick(TObject *Sender);
    void __fastcall TimerSCKARTFlowTimer(TObject *Sender);
    void __fastcall btnApplyQtyClick(TObject *Sender);
    void __fastcall btnApplySettingClick(TObject *Sender);
    void __fastcall btnApplyLotInfoClick(TObject *Sender);
    void __fastcall edtVersionMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edSPBinYieldClick(TObject *Sender);
    void __fastcall srvrscktTSVClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall TimerTSVTimer(TObject *Sender);
    void __fastcall edtMRejectCntClick(TObject *Sender);
    void __fastcall edtAlmAutoCloseSiteClick(TObject *Sender);
    void __fastcall ledShowFTCTDataClick(TObject *Sender);
    void __fastcall palInputCountMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnFTCTResetClick(TObject *Sender);
    void __fastcall edtLdCntLimNClick(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfSCKART(TComponent* Owner);

    AnsiString sLOTSTATUS;                                                      //Steven 20161025 (wei) : SCK ART function
    AnsiString sLotID;
    AnsiString sProcessCode;                                                    //Steven 20190521 : ATK lot count
    AnsiString sAlarmPath;
    AnsiString sSetupFilePath;
    AnsiString sLotStartTime;
    AnsiString sLotEndTime;
    TStringList *lAlarmList;
    TStringList *slExe;
    int iInputCount;
    int iLotCount;
//    int iRTUnitCount;
    int iFTRTCount;
    int iTesterType;
    int iManualRejectCnt;
    int iCurrentStatus;
    int iLOTSTATUS_NONE;
    int iLOTSTATUS_W;
    int iLOTSTATUS_T;
    int iLOTSTATUS_L;
    int iLOTSTATUS_R;
    int iLOTSTATUS_F;
    int iLOTSTATUS_A;
    int iWaitGPIBLotR;
    int iAutoSkipCount;
    double dCurrYield;

    int iCurrent93KARTStep;
    int iCurrentFlexARTStep;

    void SetSetupFilePath();
    void AddAlarmCode(AnsiString Code, AnsiString Message, int iDuplicate);
    void ClearAlarmCode();
    void SetLotStatus(int iStatus);
    AnsiString GetLotStatus();
    void AccessFile(bool bRead, int iAccess=-1);
    void SafeTestIFFile();
    void ReadTestIFFile();
    void ClearLotInfo();
    void UpdateCount();
    bool bShow;
    bool bShowSetting;
    bool bNeedRT;
    bool bBackUpInArmMode;
    bool bBackUpOutArmMode;                                                     //Sam 20211108 : TPW OutArm 也要一顆一顆放
    int  iLoadPickX;
    int  iLoadPickY;
    int  iInArmX;
    int  iInArmY;
    int  iNeedRT;
    bool bShowTSVMsg;
    AnsiString sTSVMsg;
    int iInputJamCnt;
    int iOutputJamCnt;
    void SettingPanelOnOff(bool bOn);
    void CheckNeedRT();
    int CheckLoadingCount();
    void CheckInArmNeedVariModeFIX();                                           //Sam 20211118 : 整合 ART InOut Arm 一顆一顆放
    void CheckOutArmNeedVariModeFIX(int iWhichAuto);                            //Sam 20211118 : 整合 ART InOut Arm 一顆一顆放
    void DoAutoSocketOff(bool bAllSiteOn=false);
    void SaveTestSummary(int iSaveData=0);
    void SaveTestSummaryTSV(int iSaveData=0);                                   //Steven 20240830 : N09獨立出來
    void Save2DSortingSummary(int iSaveData=0);                                 //Steven 20240830 : 2D sort獨立出來
    void SaveTestSummarySECS(int iSaveData=0);
    void SaveSummaryTrayFeed();
    void SaveMultiLotTestSummary(bool bSaveData);
    void AddOutputJamCnt(int row, int col, int ret, int iBinOnCarryKit=0);      //RogerYang 20260226 : 修正計數 //RogerYang 20250923 : 整合ART OutArm JamCount
    TQPF_Timer TSVdelay;
    bool __fastcall FTP_Upload(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString sULFileName);
    //int iTCPModeLotState;                                                     //Sam 20200311 : Fix TCP ART MODE Mark //Sam 20191113 : TCP ART

    AnsiString sInfo_Customer;
    AnsiString sInfo_InnerLotID;
    AnsiString sInfo_CustLotID;
    AnsiString sInfo_CustDevGup;
    AnsiString sInfo_DeviceName;
    AnsiString sInfo_Stage;
    AnsiString sInfo_Step;
    AnsiString sInfo_ReportCnt;
    AnsiString sInfo_ProgramName;
    AnsiString sInfo_TestBinNo;
    AnsiString sInfo_TesterID;
    AnsiString sInfo_HandlerID;
    AnsiString sInfo_Temperauture;
    AnsiString sInfo_CurrQty;
    AnsiString sInfo_OperatorID;
    AnsiString sInfo_BinSet;                                                    //Jerryyang 20250529 : add
    int iInfo_MultiLotCnt;

    AnsiString sInfoArr_Customer[5];
    AnsiString sInfoArr_InnerLotID[5];
    AnsiString sInfoArr_CustLotID[5];
    AnsiString sInfoArr_CustDevGup[5];
    AnsiString sInfoArr_DeviceName[5];
    AnsiString sInfoArr_Stage[5];
    AnsiString sInfoArr_Step[5];
    AnsiString sInfoArr_ReportCnt[5];
    AnsiString sInfoArr_ProgramName[5];
    AnsiString sInfoArr_TestBinNo[5];
    AnsiString sInfoArr_TesterID[5];
    AnsiString sInfoArr_HandlerID[5];
    AnsiString sInfoArr_Temperauture[5];
    AnsiString sInfoArr_CurrQty[5];
    AnsiString sInfoArr_OperatorID[5];
    AnsiString sInfoArr_BinSet[5];                                              //Jerryyang 20250529 : add

    AnsiString sBundleList;
    int iBundleInCnt;
    int iBundleOutCnt;

    void SetGPIBVersion(AnsiString Str);                                        //Steven 20230811 : 針對GPIB發送Version命令, 統一處理
    void DoARTLotStart(AnsiString _sLotID, AnsiString _sProcessCode, int _iLotCount);

    int iManualStart;                                                           //RogerYang 20250918 : 瑞薩FT-CT
    bool bLdCntExdInputCnt;                                                     //RogerYang 20250918 : 瑞薩FT-CT LoadingCount>fSCKART->iInputCount
    bool bFirstFullSkip;                                                        //RogerYang 20251112 : ART數量到，如果inarm上有IC直接丟到shuttle

    bool DoChkInputCntAlarm(bool bExcess);                                      //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定
};
//---------------------------------------------------------------------------
extern PACKAGE TfSCKART *fSCKART;
//---------------------------------------------------------------------------
class TFormInputQty : public TForm                                              //RogerYang 20251002 : RogerYang 瑞薩FT-CT
{
__published:
    void __fastcall BtnInputQTYApplyClick(TObject *Sender);
    void __fastcall BtnInputQTYEdtClick(TObject *Sender);
private:
    TButton *btn;
    TEdit   *edt;
public:
    __fastcall TFormInputQty(TComponent* Owner, int idx);
    int index;
};
#endif
