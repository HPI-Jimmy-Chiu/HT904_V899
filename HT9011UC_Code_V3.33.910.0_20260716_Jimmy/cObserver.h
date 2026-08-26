//---------------------------------------------------------------------------

#ifndef cObserverH
#define cObserverH
//---------------------------------------------------------------------------

#include "HTray.h"
#include <Buttons.hpp>
#include <Chart.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ExtDlgs.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Series.hpp>
#include <StdCtrls.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>

#include "MachineType.h"
#include <Graphics.hpp>
#include <FileCtrl.hpp>
#include <CheckLst.hpp>
#include <Menus.hpp>
#include "MyMemo.h"
//---------------------------------------------------------------------------
class TfObserver : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pgcObserv;
    TTabSheet *tsCounter;
    TTabSheet *tsTestCate;
    TTabSheet *tsScanner;
    TTabSheet *tsMDBQuery;
    TGroupBox *GroupBox7;
    TLabel *Label23;
    TLabel *Label24;
    TRadioButton *RadioButton9;
    TRadioButton *RadioButton10;
    TRadioButton *RadioButton11;
    TRadioButton *RadioButton12;
    TGroupBox *GroupBox9;
    TRadioButton *RadioButton17;
    TRadioButton *RadioButton18;
    TRadioButton *RadioButton19;
    TRadioButton *RadioButton20;
    TTabSheet *tsYield;
    TChart *ChartYield;
    TTabSheet *tsTestInfo;
    TPageControl *pgcTestInfo;
    TTabSheet *tsTestTime;
    TStringGrid *TimeInfoGrid;
    TTimer *Timer1;
    TTabSheet *tsTemperature;
    TSaveDialog *SaveDialog1;
    TSavePictureDialog *SavePictureDialog1;
    TEdit *edYieldMax;
    TEdit *edYieldMin;
    TLineSeries *SeriesAa;
    TLineSeries *SeriesAb;
    TLineSeries *SeriesAc;
    TLineSeries *SeriesAd;
    TLineSeries *SeriesAe;
    TLineSeries *SeriesAf;
    TLineSeries *SeriesAg;
    TLineSeries *SeriesAh;
    TLineSeries *SeriesBa;
    TLineSeries *SeriesBb;
    TLineSeries *SeriesBc;
    TLineSeries *SeriesBd;
    TLineSeries *SeriesBe;
    TLineSeries *SeriesBf;
    TLineSeries *SeriesBg;
    TLineSeries *SeriesBh;
    TLineSeries *SeriesCa;
    TLineSeries *SeriesCb;
    TLineSeries *SeriesCc;
    TLineSeries *SeriesCd;
    TLineSeries *SeriesCe;
    TLineSeries *SeriesCf;
    TLineSeries *SeriesCg;
    TLineSeries *SeriesCh;
    TLineSeries *SeriesDa;
    TLineSeries *SeriesDb;
    TLineSeries *SeriesDc;
    TLineSeries *SeriesDd;
    TLineSeries *SeriesDe;
    TLineSeries *SeriesDf;
    TLineSeries *SeriesDg;
    TLineSeries *SeriesDh;
    TPanel *btExit;
    TSpeedButton *SpeedButton1;
    TPanel *pnlYield;
    TTMyTray *mtRowA;
    TTMyTray *mtRowC;
    TTMyTray *mtRowB;
    TTMyTray *mtRowD;
    TPanel *labDeviceName;
    TGroupBox *GroupBox4;
    TLabel *Label16;
    TStringGrid *StringGrid2;
    TGroupBox *GroupBox5;
    TLabel *Label18;
    TImage *Image1;
    TStringGrid *StringGrid3;
    TLabel *Label1;
    TScrollBox *ScrollBox1;
    TTMyTray *mtRowName;
    TTMyTray *mtCategorySum;
    TTMyTray *myCategoryName;
    TTMyTray *mtDutName;
    TTMyTray *mtArmName;
    TTMyTray *mtHeadTotal;
    TTMyTray *mtSockTotal;
    TTMyTray *mtPassHead;
    TTMyTray *mtPassSocket;
    TTMyTray *mtIfError;
    TTMyTray *mtCategoryNo;
    TGroupBox *GroupBox8;
    TLabel *Label28;
    TLabel *Label29;
    TRadioButton *rbHeadNumber;
    TRadioButton *rbSocketNumber;
    TRadioButton *rbHeadPercent;
    TRadioButton *rbSocketPercent;
    TRadioGroup *rgRowNo;
    TTMyTray *mtNo;
    TTMyTray *mtChName;
    TTMyTray *mtTotalName;
    TTMyTray *mtCategoryTotal;
    TTMyTray *mtTotal;
    TStringGrid *StringGrid5;
    TRadioGroup *rgContactCountKinds;
    TRadioGroup *rgContactCountKindsForm;
    TRadioGroup *rgContactCountHistory;
    TRadioGroup *rgContactCountHistoryForm;
    TStringGrid *TimeInfoGrid_InArm;
    TTabSheet *tsTimeData;
    TStringGrid *sgTimeData;
    TTabSheet *tsLoadInfo;
    TMemo *Memo2;
    TButton *btOpenLoadLog;
    TOpenDialog *OpenDialog1;
    TMemo *Memo3;
    TMemo *Memo4;
    TPanel *APHeadLabel13;
    TPanel *APHeadLabel14;
    TPanel *APHeadLabel18;
    TPageControl *pgcMessage;
    TTabSheet *tsMDB;
    TTabSheet *tsEventLogTxt;
    TLabel *Label25;
    TLabel *Label33;
    TLabel *Label57;
    TLabel *Label58;
    TLabel *Label59;
    TLabel *Label26;
    TLabel *Label5;
    TPanel *lbltTotalLoader;
    TComboBox *cbDisplayData;
    TDateTimePicker *DateTimePicker1;
    TDateTimePicker *DateTimePicker2;
    TDateTimePicker *DateTimePicker3;
    TDateTimePicker *DateTimePicker4;
    TButton *btnQueryEventLog;
    TButton *Button7;
    TButton *btAutoSave;
    TCheckBox *CheckBox1;
    TPanel *pnlTotalCount;
    TStringGrid *strngrdMDBQuery;
    TChart *Chart2;
    TBarSeries *BarSeries2;
    TStringGrid *strngrdEventLog;
    TPanel *pnlEventLogTxt;
    TComboBox *cbbEventLogYear;
    TLabel *lblYear;
    TLabel *Label15;
    TComboBox *cbbMonth;
    TListBox *lstEventLog;
    TButton *btnQueryEventLogTxt;
    TLabel *Label19;
    TComboBox *cbbFilter;
    TTabSheet *tsOEE_ProductionInfor;
    TTabSheet *ts2;
    TStringGrid *sg_ListTimeReceiveInfoGrid;
    TTabSheet *tsDataRecord;
    TPageControl *pgcPrecautions;
    TTabSheet *tsPrecautionsRecord;
    TPanel *pnPrecautionsRecord;
    TPanel *pnMaintenanceRecord;
    TSpeedButton *sbHandlerPrecautionRecordSet;
    TSpeedButton *sbHandlerPrecautionRecordClear;
    TSpeedButton *sbHandlerPrecautionFormShow;
    TSpeedButton *sbPRFinishDate;
    TSpeedButton *sbPRStartDate;
    TSpeedButton *sbPrecautionSave;
    TSpeedButton *cobNoteContentsSet;
    TSpeedButton *sbScreenkeyboard;
    TComboBox *cobHandlerPrecautionRecord;
    TMemo *MemoHandlerPrecautionRecord;
    TPanel *pnPrecautionRecordDocumentNoTitle;
    TEdit *edPrecautionRecordDocumentNo;
    TPanel *Panel91;
    TPanel *Panel92;
    TEdit *edNoteContents;
    TPanel *Panel93;
    TEdit *edApprovedManager;
    TPanel *Panel94;
    TEdit *edWatchmakers;
    TPanel *pnPrecautionStartTime;
    TPanel *Panel96;
    TPanel *Panel5;
    TEdit *edFinishName;
    TComboBox *cobPRFinishType;
    TPanel *pnPrecautionEndTime;
    TComboBox *cobNoteContents;
    TPanel *Panel7;
    TEdit *edPromptDay;
    TDateTimePicker *DateTimePickerEnd;
    TDateTimePicker *DateTimePickerStart;
    TPanel *pnPRSpecificationNO;
    TTabSheet *tsHanderMajorMaintenance;
    TPanel *pnHanderMajrMaintenance;
    TSpeedButton *sbMajorMaintenanceDate;
    TSpeedButton *sbMajorMaintenanceStartTime;
    TSpeedButton *sbMajorMaintenanceEndTime;
    TSpeedButton *sbUndesirablePhenomenonClear;
    TSpeedButton *sbCountermeasureClear;
    TSpeedButton *sbMajorMaintenanceSave;
    TSpeedButton *sbUndesirablePhenomenon;
    TSpeedButton *sbCountermeasure;
    TSpeedButton *sbMajorMaintenanceSearch;
    TSpeedButton *sbScreenkeyboard2;
    TPanel *pnMajorMaintenanceDate;
    TPanel *pnMajorMaintenanceClassType;
    TComboBox *cobMajorMaintenanceClassType;
    TPanel *pnMajorMaintenanceStartTime;
    TPanel *pnMajorMaintenanceEndTime;
    TPanel *pnMajorMaintenanceCheckNo;
    TPanel *pnMajorMaintenancePersonnel;
    TPanel *pnMajorMaintenanceCheckPersonnel;
    TEdit *edMajorMaintenanceCheckNo;
    TEdit *edMajorMaintenancePersonnel;
    TEdit *edMajorMaintenanceCheckPersonnel;
    TPanel *pnUndesirablePhenomenon;
    TPanel *pnCountermeasure;
    TMemo *MemoUndesirablePhenomenon;
    TMemo *MemoCountermeasure;
    TPanel *pnMMSpecificationNO;
    TComboBox *cobUndesirablePhenomenon;
    TComboBox *cobCountermeasure;
    TPanel *Panel10;
    TComboBox *cobMajorMaintenanceSearch;
    TFileListBox *FileListBoxMajorMaintenance;
    TTabSheet *tsPrecautionLog;
    TPanel *pnPrecautionLog;
    TSpeedButton *sbSearchPrecautionLog;
    TBevel *Bevel1;
    TMemo *MemoNoteLog;
    TPanel *pnDOCUMENTNO;
    TPanel *pnNoteLog;
    TPanel *pnApprovedManager;
    TPanel *pnWatchmakers;
    TPanel *pnPrecautionLogPromptDay;
    TPanel *pnFinishType;
    TPanel *pnFinishName;
    TPanel *pnPrecautionLogFinishName;
    TPanel *pnPrecautionLogDocumentNo;
    TPanel *pnPrecautionLogNoteContents;
    TPanel *pnStartTime;
    TPanel *pnEndTime;
    TPanel *pnPrecautionLogStartTime;
    TPanel *pnPrecautionLogApprovedManager;
    TPanel *pnPrecautionLogWatchmakers;
    TPanel *pnPrecautionLogFinishType;
    TPanel *pnPromptDay;
    TPanel *pnPrecautionLogEndTime;
    TPanel *Panel8;
    TComboBox *cobSearchPrecautionLog;
    TFileListBox *FileListBoxPrecautionLog;
    TPanel *pnNoteContents;
    TPanel *Panel9;
    TTabSheet *tsMsgTimeData;
    TPanel *pnlTimeDataTop;
    TListBox *lstTimeData;
    TButton *btnTimeData;
    TStringGrid *strngrdTimeData;
    TTabSheet *tsTimeInfo;
    TStringGrid *strngrdTestTime;
    TStringGrid *strngrdTemp;
    TTabSheet *tsSGJamCount;
    TPanel *Panel2;
    TLabel *labLoaderCount;
    TLabel *Label21;
    TButton *btnSG_QueryNow;
    TButton *btnSG_QueryYesterday;
    TStringGrid *strngrdJamLog;
    TButton *btnBackupLogYear;
    TTabSheet *tsIndexAirOn1;
    TTabSheet *tsIndexAirOn2;
    TStringGrid *strngrdIndeAirOn2;
    TStringGrid *strngrdIndeAirOn1;
    TTabSheet *tsLotInfo;
    TGroupBox *gbLotinfo;
    TLabel *labLotInfo1;
    TLabel *labLotInfo2;
    TLabel *labLotInfo3;
    TLabel *labLotInfo4;
    TLabel *labLotInfo5;
    TLabel *labLotInfo6;
    TLabel *labLotInfo7;
    TLabel *labLotInfo8;
    TLabel *labLotInfo9;
    TLabel *labLotInfo10;
    TLabel *labLotInfo11;
    TLabel *labLotInfo12;
    TLabel *labLotInfo13;
    TLabel *labLotInfo14;
    TLabel *labLotInfo15;
    TPanel *palCustomer;
    TPanel *palInnLotID;
    TPanel *palCustLotID;
    TPanel *palCustDevGup;
    TPanel *palDevName;
    TPanel *palStage;
    TPanel *palStep;
    TPanel *palReportCnt;
    TPanel *palProgramName;
    TPanel *palTestBin;
    TPanel *palTestID;
    TPanel *palHandlerID;
    TPanel *palTemperature;
    TPanel *palCurrQty;
    TPanel *palOPID;
    TGroupBox *grpLotInfo;
    TMyMemo *memoLotSummary;
    TLabel *labMultiLotCnt;
    TPanel *palMultiLotCnt;
    TButton *btnLot2;
    TButton *btnLot3;
    TButton *btnLot4;
    TButton *btnLot5;
    TPanel *pnlCounterTop;
    TGroupBox *GroupBox1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *labDayJamRate;
    TLabel *lblMTBF;
    TPanel *labPowerOnTime;
    TPanel *labRunningTime;
    TPanel *labProductTime;
    TPanel *labLoadingCount;
    TPanel *labMUBA;
    TPanel *labMTBA;
    TPanel *labMTBF;
    TPanel *pnlDayJamRate;
    TGroupBox *GroupBox2;
    TLabel *Label35;
    TLabel *Label38;
    TLabel *Label53;
    TLabel *Label40;
    TLabel *Label6;
    TLabel *Label11;
    TLabel *Label10;
    TLabel *Label12;
    TLabel *Label13;
    TPanel *labModel;
    TPanel *labSerialNo;
    TPanel *labReleaseDate;
    TPanel *labFactory;
    TPanel *labMachineID;
    TPanel *labVersion;
    TMemo *Memo1;
    TGroupBox *grpATCSerialNumber;
    TLabel *lblATCInPC1;
    TLabel *lblATCInPC2;
    TLabel *lblATCInPC4;
    TLabel *lblATCInPC3;
    TLabel *lbSerialNumber04;
    TLabel *lbSerialNumber03;
    TLabel *lbSerialNumber02;
    TLabel *lbSerialNumber01;
    TLabel *Label14;
    TLabel *lbFirmwareNumber01;
    TLabel *Label17;
    TLabel *lbFirmwareNumber02;
    TLabel *Label20;
    TLabel *lbFirmwareNumber03;
    TLabel *Label22;
    TLabel *lbFirmwareNumber04;
    TLabel *Label30;
    TLabel *Label31;
    TPanel *pnlGPIBVersion;
    TPanel *pnlESDVersion;
    TPanel *pnlATCVersion;
    TPanel *pnlTTLRS232Version;
    TButton *btnClearTime;
    TLabel *Label27;
    TLabel *Label32;
    TLabel *Label34;
    TLabel *labBundleID;
    TLabel *labBundlIn;
    TLabel *labBundOut;
    TButton *btnLot1;
    TChart *TempChart;
    TComboBox *cbbTempChart;
        TLabel *labBinSet;
        TPanel *palBinSetting;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall StringGrid2DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall StringGrid3DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall StringGrid5DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall rgRowNoClick(TObject *Sender);
    void __fastcall StringGrid2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall StringGrid3MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall pgcObservChange(TObject *Sender);
    void __fastcall DateTimePicker1CloseUp(TObject *Sender);
    void __fastcall BtnSaveClick(TObject *Sender);
    void __fastcall BtnQueryClick(TObject *Sender);
    void __fastcall cbbTempChartChange(TObject *Sender);
    void __fastcall Image1DblClick(TObject *Sender);
    void __fastcall btAutoSaveClick(TObject *Sender);
    void __fastcall lbltTotalLoaderMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btReportClick(TObject *Sender);
    void __fastcall edYieldMaxClick(TObject *Sender);
    void __fastcall edYieldMinClick(TObject *Sender);
    void __fastcall BtnExitClick(TObject *Sender);
    void __fastcall rgContactCountKindsClick(TObject *Sender);
    void __fastcall rgContactCountHistoryClick(TObject *Sender);
    void __fastcall rgContactCountKindsFormClick(TObject *Sender);
    void __fastcall rgContactCountHistoryFormClick(TObject *Sender);
    void __fastcall mtRowAMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btOpenLoadLogClick(TObject *Sender);
    void __fastcall cbbMonthChange(TObject *Sender);
    void __fastcall lstEventLogClick(TObject *Sender);
    void __fastcall btnQueryEventLogTxtClick(TObject *Sender);
    void __fastcall sbScreenkeyboardClick(TObject *Sender);
    void __fastcall cobNoteContentsSetClick(TObject *Sender);
    void __fastcall sbHandlerPrecautionRecordSetClick(TObject *Sender);
    void __fastcall sbHandlerPrecautionRecordClearClick(TObject *Sender);
    void __fastcall sbHandlerPrecautionFormShowClick(TObject *Sender);
    void __fastcall sbPrecautionSaveClick(TObject *Sender);
    void __fastcall sbPRFinishDateClick(TObject *Sender);
    void __fastcall sbPRStartDateClick(TObject *Sender);
    void __fastcall sbMajorMaintenanceDateClick(TObject *Sender);
    void __fastcall sbMajorMaintenanceStartTimeClick(TObject *Sender);
    void __fastcall sbUndesirablePhenomenonClick(TObject *Sender);
    void __fastcall sbCountermeasureClick(TObject *Sender);
    void __fastcall sbUndesirablePhenomenonClearClick(TObject *Sender);
    void __fastcall sbCountermeasureClearClick(TObject *Sender);
    void __fastcall sbMajorMaintenanceEndTimeClick(TObject *Sender);
    void __fastcall sbMajorMaintenanceSaveClick(TObject *Sender);
    void __fastcall sbMajorMaintenanceSearchClick(TObject *Sender);
    void __fastcall sbSearchPrecautionLogClick(TObject *Sender);
    void __fastcall pgcMessageChange(TObject *Sender);
    void __fastcall lstTimeDataClick(TObject *Sender);
    void __fastcall btnSG_QueryNowClick(TObject *Sender);
    void __fastcall btnSG_QueryYesterdayClick(TObject *Sender);
    void __fastcall btnBackupLogYearClick(TObject *Sender);
    void __fastcall btnClearTimeClick(TObject *Sender);
    void __fastcall btnLot1Click(TObject *Sender);
private:     // User declarations
        HDC DcSG2,DcSG3,DcSG5;
        int RowNo;
        bool bChangeRow[2];
        AnsiString EventName;
        //int i_WriteNo;
        AnsiString __fastcall CalculateStopTime(int Sec);
        int __fastcall UnCalculateStopTime(AnsiString Time);
        TCanvas *pCanvas;
public:     // User declarations
        bool bShow;
        bool bIsLoaded[13];
        int iTestTimeCT;
        int iTestTime;
        __fastcall TfObserver(TComponent* Owner);
        void __fastcall DrawCenterLine(int Mode, int iLeft, int iCellWidth);
        void __fastcall DrawCellCounter(int iCol,int iRow,TRect &Rect, int iLeft, int iCellWidth);
        void __fastcall DrawCellCategory(int iCol,int iRow,TRect &Rect, HDC SGDC, int Mode);
        void __fastcall WriteCategoryData();
        void __fastcall WriteContactKind();
        void __fastcall UpdateYieldChart();
        void __fastcall UpdateBin();
        void __fastcall GetMachineData();
        void __fastcall ProcessRunInfo();
        int iTotoalTestTime;
        double dTotoalIndexCycleTime;                                           //Isaac 20180301 (Steven) Index Cycle Time Monitoring function
        void __fastcall SetSiteYieldDiagram();
        void __fastcall UpdateTempChart();                                      //Steven 20090827
        void __fastcall ShowVer();
        int iShowYieldChart;                                                    //Steven 20100818 : 只顯示Yield Chart
        double fRecordIndexTime[20];
        double fRecordInArmTime[20];
        double fRecordInArmTime1[20];
        double fRecordInArmTime2[20];
        void RecordIndexTime(double fData);
        void RecordInArmTime();
        void AddTimeData(int iRow, double Time);
        void CountMTBF();                                                       //Steven 20170309 (wei) add MTBF(PauseTime/Jam)
        int iPauseTime;                                                         //Steven 20170309 (wei) add MTBF(PauseTime/Jam)
        int iProductTime;                                                       //Isaac 20180417 修正MTBF公式(pause+production+jam)/jamcount
        int iJamTime;                                                           //Isaac 20180417 修正MTBF公式(pause+production+jam)/jamcount
        void __fastcall bAutoSaveEventLog(bool flag);                           //Steven 20110221 : EventLogAutoSave
        double dTempHistroy[tcTotalCount][60];
        bool bSavePrecautionRecordFinish;                                       //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
        bool bStartPrecautionRecord;                                            //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
        bool bChangeReciepeSaveMajorMaintenanceRecord;                          //Sam 20171120 (Steven) AddHanderMajorMaintenanceRecordFunction (form HT7045)
        bool bShowMajorMaintenanceRecord;                                       //Sam 20171120 (Steven) AddHanderMajorMaintenanceRecordFunction (form HT7045)
        AnsiString asStartPrecautionRecordMOId;                                 //Sam 20171122 (Steven) AddPrecautionRecordFunction (form HT7045)
        void __fastcall LoadPrecautionMenu();                                   //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
        void __fastcall LoadPrecautionLogMenu();                                //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
        void __fastcall SavePrecautionMemoInformation();                        //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
        void __fastcall SavePrecautionParameter();                              //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
        void __fastcall LoadPrecautionParameter();                              //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
        bool __fastcall CheckKeyInPrecautionMemoInformation(int iType);         //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
        void __fastcall LoadMajorMaintenanceMenu();                             //Sam 20171120 (Steven) AddHanderMajorMaintenanceRecordFunction (form HT7045)
        void __fastcall LoadMajorMaintenanceLogMenu();                          //Sam 20171120 (Steven) AddHanderMajorMaintenanceRecordFunction (form HT7045)
        void __fastcall SaveMajorMaintenanceInformation();                      //Sam 20171120 (Steven) AddHanderMajorMaintenanceRecordFunction (form HT7045)
        bool __fastcall CheckKeyInMajorMaintenanceInformation(int iType);       //Sam 20171120 (Steven) AddHanderMajorMaintenanceRecordFunction (form HT7045)
        void GetEventLogText();
        void GetTimeDataText();                                                 //Steven 20190903 : Time Data顯示
        int iIndexCycleTimeCount;                                               //Isaac 20180301 (Steven) Index Cycle Time Monitoring function
        int iTestReceiveTimeCount;                                              //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        double dRecordTestTime[10];                                             //Sam 20171205 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        AnsiString sTestReceiveTime;                                            //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        bool bTestIndexZ;                                                       //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        int iTestIndexZCount;                                                   //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        double dRecordIndexZTime[10];                                           //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        AnsiString sTestIndexZTime;                                             //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        double dOEEIndexCycleTime;                                              //Sam 20180802 (wei) : OEE 32Site 修正
        TQPF_Timer tRecordInArmTimer;
        void RecordIndexCycle(bool bReset=false);                               //Sam 20200916 : Add Index Cycle Time Record
        AnsiString sRecordIndexCycleTime[200];                                  //Sam 20200916 : Add Index Cycle Time Record
        void DoProduction_Summary_Report(AnsiString asStartData,AnsiString asStartTime,AnsiString asEndData,AnsiString asEndTime);  //Sam 20210107 : Summary Report fuction
        void StatisticalJamCount(bool bIsNextDay=false);                        //Sam 20210224 : Auto Upload FTP JAMRawData 功能 //KaiChen 20200618 ：矽格，增加Jam統計頁面
        void StatisticalLoaderCount();                                          //KaiChen 20200618 ：矽格，增加Jam統計頁面
        void ReadLoaderCount();                                                 //KaiChen 20200618 ：矽格，增加Jam統計頁面
        bool StatisticalJamCountEnable(AnsiString asJamCode);                   //KaiChen 20200618 ：矽格，增加Jam統計頁面
};
//---------------------------------------------------------------------------
extern PACKAGE TfObserver *fObserver;
void RecordStartTestTime();
int RecordEndTestTime(int iArm);                                                //Sam 20201231 : 修正關 Arm 後，Index Cycle time 異常。0:arm1 1:arm2 2:雙Arm
void RecordReceiveTestTime();                                                   //KaiChen 20171127 ：超豐 OEE 新增 Test Time、Index Time
void IniRecordMonitoringIndexCycleTime();                                       //Isaac (Steven) 20180301 Index Cycle Time Monitoring function
void RecordIndexAirOnTime1();                                                   //Sam 20220329 : Record Index Air On Time
void RecordIndexAirOnTime2();                                                   //Sam 20220329 : Record Index Air On Time
//---------------------------------------------------------------------------
#endif
