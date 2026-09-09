//---------------------------------------------------------------------------

#ifndef PMAlarmInterFaceH
#define PMAlarmInterFaceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>

#include "PMAlarmSystem.h"
#include "SHDocVw_OCX.h"
#include <OleCtrls.hpp>          // 2015.03.26 , Joye , PM Alarm

//---------------------------------------------------------------------------
class TfPMAlarmInterFace : public TForm
{
__published:    // IDE-managed Components
    TPanel *pnlPMAlarm;
    TPanel *pnlPMAlarm_Title;
    TPageControl *pc_PMAlarmFuction;
    TTabSheet *ts_PMItemSetup;
    TTabSheet *ts_PMItemCheck;
    TPanel *pnlPMAlarmSet;
    TPanel *pnlPMAlarmSetting_Title;
    TPageControl *pgPMAlarmSetting;
    TTabSheet *tsPMAlarmMonth;
    TSpeedButton *sbPMAlarmMonth_Add;
    TSpeedButton *sbPMAlarmMonth_Delete;
    TStringGrid *sgPMAlarmMonthItem;
    TTabSheet *tsPMAlarmQuarter;
    TSpeedButton *sbPMAlarmQuarter_Add;
    TSpeedButton *sbPMAlarmQuarter_Delete;
    TStringGrid *sgPMAlarmQuarterItem;
    TTabSheet *QuarterYear;
    TSpeedButton *sbPMAlarmYear_Add;
    TSpeedButton *sbPMAlarmYear_Delete;
    TStringGrid *sgPMAlarmYearItem;
    TPanel *pnlPMAlarmSetting_Bottom;
    TPanel *pnlPMAlarmCheck;
    TPanel *pnlPMAlarmCheck_Title;
    TPanel *pnlPMAlarmCheck_Bottom;
    TSpeedButton *sbPMAlarmCheck_Update;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TPanel *Panel6;
    TPanel *Panel7;
    TPanel *Panel8;
    TPanel *Panel9;
    TTabSheet *TabSheet5;
    TSpeedButton *sbPMAlarmTemperature_Add;
    TSpeedButton *sbPMAlarmTemperature_Delete;
    TStringGrid *sgPMAlarmTemperatureItem;
    TPanel *Panel10;
    TPanel *Panel11;
    TSpeedButton *sbPMAlarmESD_Add;
    TSpeedButton *sbPMAlarmESD_Delete;
    TStringGrid *sgPMAlarmESDItem;
    TPanel *Panel12;
    TPanel *Panel13;
    TSpeedButton *sbPMAlarmIonFan_Add;
    TSpeedButton *sbPMAlarmIonFan_Delete;
    TStringGrid *sgPMAlarmIonFanItem;
    TPanel *Panel14;
    TPanel *Panel15;
    TTabSheet *ts_PMSetup;
    TPanel *Panel16;
    TPanel *pnlPMAlarmNextPMDate;
    TDateTimePicker *dtpPMAlarmNextPMDate;
    TButton *bt_ResetPMDate;
    TGroupBox *GroupBox2;
    TPanel *Panel18;
    TEdit *ed_GerneralPMDocVer;
    TPanel *Panel19;
    TEdit *ed_TemperaturePMDocVer;
    TPanel *Panel20;
    TEdit *ed_ESDPMDocVer;
    TPanel *Panel21;
    TEdit *ed_IonFanPMDocVer;
    TPageControl *pc_PMItemCheck;
    TTabSheet *ts_GerneralItemCheck;
    TTabSheet *ts_TemperatureCheck;
    TTabSheet *TabSheet9;
    TStringGrid *sgGerneralPMAlarmCheckItem;
    TStringGrid *sgTemperaturePMAlarmCheckItem;
    TPanel *Panel17;
    TSpeedButton *sbPMAlarmCheck_ACCClick;
    TPanel *Panel22;
    TSpeedButton *sbPMAlarmCheck_REJClick;
    TPanel *Panel23;
    TEdit *ed_ExecutiveOwner;
    TPanel *Panel24;
    TEdit *ed_ConformOwner;
    TEdit *ed_ExecutiveItem;
    TEdit *ed_ConformItem;
    TPanel *Panel25;
    TPanel *Panel26;
    TPanel *pn_GerneralExecutiveOwner;
    TPanel *pn_GerneralConformOwner;
    TEdit *ed_GerneralExecutiveOwner;
    TEdit *ed_GerneralConformOwner;
    TSpeedButton *sb_GerneralExecutiveOwnerCheck;
    TSpeedButton *sb_GerneralConformOwnerCheck;
    TSpeedButton *sbPMAlarmCheck_Check;
    TPanel *Panel29;
    TSpeedButton *sbPMAlarmCheck_IsAlarm;
    TPanel *pnlPMAlarmAfterDays;
    TEdit *edtPMAlarmStopDays;
    TTabSheet *ts_Others;
    TPanel *Panel30;
    TSpeedButton *SpeedButton9;
    TSpeedButton *SpeedButton10;
    TPanel *pn_TemperatureExecutiveOwner;
    TEdit *ed_TemperatureExecutiveOwner;
    TSpeedButton *sb_TemperatureExecutiveOwnerCheck;
    TSpeedButton *sb_TemperatureConformOwnerCheck;
    TEdit *ed_TemperatureConformOwner;
    TPanel *pn_TemperatureConformOwner;
    TTabSheet *TabSheet7;
    TSpeedButton *sb_ESDExecutiveOwnerCheck;
    TSpeedButton *sb_ESDConformOwnerCheck;
    TPanel *pn_ESDExecutiveOwner;
    TEdit *ed_ESDExecutiveOwner;
    TEdit *ed_ESDConformOwner;
    TPanel *pn_ESDConformOwner;
    TSpeedButton *sb_IonFanExecutiveOwnerCheck;
    TSpeedButton *sb_IonFanConformOwnerCheck;
    TPanel *pn_IonFanExecutiveOwner;
    TEdit *ed_IonFanExecutiveOwner;
    TEdit *ed_IonFanConformOwner;
    TPanel *pn_IonFanConformOwner;
    TStringGrid *sgESDPMAlarmCheckItem;
    TStringGrid *sgIonFanPMAlarmCheckItem;
    TPageControl *PageControl1;
    TTabSheet *TabSheet10;
    TTabSheet *TabSheet11;
    TTabSheet *TabSheet12;
    TTabSheet *TabSheet13;
    TStringGrid *sg_GerneralPMAlarmToCSV;
    TStringGrid *sg_TemperaturePMAlarmToCSV;
    TStringGrid *sg_ESDPMAlarmToCSV;
    TStringGrid *sg_IonFanPMAlarmToCSV;
    TPanel *pnlPMAlarmDate;
    TComboBox *cbPMAlarmDate;
    TPanel *pnlPMAlarmBeforeDays;
    TEdit *edtPMAlarmAlarmDays;
    TPanel *Panel27;
    TSpeedButton *sb_SaveSetting;
    TPanel *Panel28;
    TPanel *Panel1;
    TSpeedButton *sbPMAlarm_Save;
    TPanel *Panel31;
    TEdit *ed_MachineNo;
    TPanel *Panel32;
    TEdit *ed_SaveFilePath;
    TSpeedButton *sb_LoadTemp;
    TEdit *ed_PMDate;
    TSpeedButton *sb_SetPmDate;
    TPanel *pn_SetPMDate;
    TMonthCalendar *mc_SetPMDate;
    TPanel *pn_SetPMDateClose;
    TPanel *Panel33;
    TPanel *Panel34;
    TComboBox *cbPMStartMonth;
    TPanel *Panel35;
    TSpeedButton *sbPMAlarmCheck_ShowData;
    TTabSheet *ts_SOPItem;
    TCppWebBrowser *cpwbrwsrPmSOP;
    TGroupBox *gbUpdateByServer;
    TButton *btnDownload;
    TButton *btnUpload;
    void __fastcall sbPMAlarmMonth_AddClick(TObject *Sender);
    void __fastcall sbPMAlarmMonth_DeleteClick(TObject *Sender);
    void __fastcall sbPMAlarm_SaveClick(TObject *Sender);
    void __fastcall sbPMAlarmCheck_UpdateClick(TObject *Sender);
    void __fastcall sgPMAlarmMonthItemSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall bt_ResetPMDateClick(TObject *Sender);
    void __fastcall sgGerneralPMAlarmCheckItemDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall sgGerneralPMAlarmCheckItemSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall sbPMAlarmCheck_ACCClickClick(TObject *Sender);
    void __fastcall sb_GerneralExecutiveOwnerCheckClick(TObject *Sender);
    void __fastcall pc_PMItemCheckChange(TObject *Sender);
    void __fastcall pc_PMAlarmFuctionChange(TObject *Sender);
    void __fastcall sbPMAlarmCheck_CheckClick(TObject *Sender);
    void __fastcall sbPMAlarmCheck_IsAlarmClick(TObject *Sender);
    void __fastcall SpeedButton9Click(TObject *Sender);
    void __fastcall SpeedButton10Click(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall sbPMAlarmTemperature_AddClick(TObject *Sender);
    void __fastcall sbPMAlarmTemperature_DeleteClick(TObject *Sender);
    void __fastcall sgPMAlarmTemperatureItemSelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect);
    void __fastcall sgTemperaturePMAlarmCheckItemSelectCell(
          TObject *Sender, int ACol, int ARow, bool &CanSelect);
    void __fastcall sb_TemperatureExecutiveOwnerCheckClick(
          TObject *Sender);
    void __fastcall sbPMAlarmESD_AddClick(TObject *Sender);
    void __fastcall sbPMAlarmESD_DeleteClick(TObject *Sender);
    void __fastcall sbPMAlarmIonFan_AddClick(TObject *Sender);
    void __fastcall sbPMAlarmIonFan_DeleteClick(TObject *Sender);
    void __fastcall sgPMAlarmESDItemSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall sgPMAlarmIonFanItemSelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect);
    void __fastcall sb_ESDExecutiveOwnerCheckClick(TObject *Sender);
    void __fastcall sb_IonFanExecutiveOwnerCheckClick(TObject *Sender);
    void __fastcall sgESDPMAlarmCheckItemKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall sgIonFanPMAlarmCheckItemKeyUp(TObject *Sender,
          WORD &Key, TShiftState Shift);
    void __fastcall sgTemperaturePMAlarmCheckItemKeyUp(TObject *Sender,
          WORD &Key, TShiftState Shift);
    void __fastcall sgGerneralPMAlarmCheckItemKeyUp(TObject *Sender,
          WORD &Key, TShiftState Shift);
    void __fastcall sb_SaveSettingClick(TObject *Sender);
    void __fastcall Panel28DblClick(TObject *Sender);
    void __fastcall sb_LoadTempClick(TObject *Sender);
    void __fastcall pn_SetPMDateCloseClick(TObject *Sender);
    void __fastcall sb_SetPmDateClick(TObject *Sender);
    void __fastcall mc_SetPMDateClick(TObject *Sender);
    void __fastcall sbPMAlarmCheck_ShowDataClick(TObject *Sender);
    void __fastcall edtPMAlarmAlarmDaysMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnDownloadClick(TObject *Sender);
    void __fastcall btnUploadClick(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfPMAlarmInterFace(TComponent* Owner);
    PMAlarmSystem PMAlarm_SYS;      // 2015.03.26 , Joye , PM Alarm
    void UpdatePMAlarmToScreen(bool bIsCheckOk = false);           // 2015.03.26 , Joye , PM Alarm
    bool bIsGerneralItemCheckOK(bool bIsItemCheckOnly = false);
    int iPMAlarmSelectRow;                  // 2015.03.26 , Joye , PM Alarm
    int iPMAlarmSelectCol;
    int iSelectRow;
    int iSelectCol;
    void SaveGerneralPMAlarmToCSV();
    void UpdateToGerneralPMAlarmToCSV();
    void LoadGerneralPMAlarmFromCSV();
    AnsiString GetCsvData(int iDataNum,AnsiString sCSVStr); //取得CSV格式中的第幾筆資料
    int iPMAlarmTemperatureSelectRow;
    int iPMAlarmTemperatureSelectCol;
    bool bIsTemperatureItemCheckOK(bool bIsItemCheckOnly = false);
    void UpdateToTemperaturePMAlarmToCSV();
    void SaveTemperaturePMAlarmToCSV();
    void LoadTemperaturePMAlarmFromCSV();
    int iPMAlarmESDSelectRow;
    int iPMAlarmESDSelectCol;
    bool bIsESDItemCheckOK(bool bIsItemCheckOnly = false);
    void UpdateToESDPMAlarmToCSV();
    void SaveESDPMAlarmToCSV();
    void LoadESDPMAlarmFromCSV();

    int iPMAlarmIonFanSelectRow;
    int iPMAlarmIonFanSelectCol;
    bool bIsIonFanItemCheckOK(bool bIsItemCheckOnly = false);
    void UpdateToIonFanPMAlarmToCSV();
    void SaveIonFanPMAlarmToCSV();
    void LoadIonFanPMAlarmFromCSV();

    void SaveSetting();
    void SetTemperatureValue(AnsiString sTmpBase,       double fLowBase,    double fHighBase,
                             AnsiString sPLATE1,        double fPLATE1_L,   double fPLATE1_H,
                             AnsiString sPLATE2,        double fPLATE2_L,   double fPLATE2_H,
                             AnsiString sSHUTTLE1,      double fSHUTTLE1_L, double fSHUTTLE1_H,
                             AnsiString sSHUTTLE2,      double fSHUTTLE2_L, double fSHUTTLE2_H,
                             AnsiString sSocket,        double fSocket_L,   double fSocket_H,
                             AnsiString sChamber,       double fChamber_L,  double fChamber_H,
                             AnsiString sAa1,           double fAa1_L,      double fAa1_H,
                             AnsiString sAb1,           double fAb1_L,      double fAb1_H,
                             AnsiString sAc1,           double fAc1_L,      double fAc1_H,
                             AnsiString sAd1,           double fAd1_L,      double fAd1_H,
                             AnsiString sBa1,           double fBa1_L,      double fBa1_H,
                             AnsiString sBb1,           double fBb1_L,      double fBb1_H,
                             AnsiString sBc1,           double fBc1_L,      double fBc1_H,
                             AnsiString sBd1,           double fBd1_L,      double fBd1_H,
                             AnsiString sAa2,           double fAa2_L,      double fAa2_H,
                             AnsiString sAb2,           double fAb2_L,      double fAb2_H,
                             AnsiString sAc2,           double fAc2_L,      double fAc2_H,
                             AnsiString sAd2,           double fAd2_L,      double fAd2_H,
                             AnsiString sBa2,           double fBa2_L,      double fBa2_H,
                             AnsiString sBb2,           double fBb2_L,      double fBb2_H,
                             AnsiString sBc2,           double fBc2_L,      double fBc2_H,
                             AnsiString sBd2,           double fBd2_L,      double fBd2_H
                             );

    void TemperaturePMAlarmLoadFromSetup();
    AnsiString GetWhenAlarmShowPMDate();
    void SetVisibleLevel(bool bVisiblePMItemSetup,bool bVisibleSetup,bool bVisibleExecutive);
    bool fShow;
    bool Upload2Server();      //JimmyChiu 20230315 : PM Alarm Update From Server by FTP
    bool DownloadFromServer(); //JimmyChiu 20230315 : PM Alarm Update From Server by FTP
    bool PMAllAlarmCheckOK();                                                   //JimmyChiu 20231230 : PM All Alarm Check
    void InitialGrid(TStringGrid *sg,int iTotalCount);                          //JimmyChiu 20231230 : Initial PM Alarm Grid
    AnsiString GetFilePath(AnsiString sFilePath="");                            //JimmyChiu 20231230 : PM Alarm Get File Path
    AnsiString GetFileName(AnsiString sNowYear,AnsiString sFileName);           //JimmyChiu 20231230 : PM Alarm Get File Name
    AnsiString GetFullFileName(AnsiString sNowYear,AnsiString sFileName);       //JimmyChiu 20231230 : PM Alarm Get Full File Name
    int GetYear(TDateTime tdt);                                                 //JimmyChiu 20231230 : PM Alarm Get Year
    int GetMonth(TDateTime tdt);                                                //JimmyChiu 20231230 : PM Alarm Get Month
};
//---------------------------------------------------------------------------
extern PACKAGE TfPMAlarmInterFace *fPMAlarmInterFace;
//---------------------------------------------------------------------------
#endif
