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
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
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
    TRadioGroup *rgAOIFailBinType;
    TLabel *lblAOIBinSel1;
    TLabel *lblAOIBinSel2;
    TComboBox *cbAOIFialAndTestPass;
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

private:	// User declarations
    int iQuotient;                                                              //KenHsieh 20220830 : 修改add:AOI Tray Edit
    int iRemainder;                                                             //KenHsieh 20220830 : 修改add:AOI Tray Edit
    void __fastcall UpdateAOIFailBinTypetoForm();                               //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
public:		// User declarations
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
#endif
