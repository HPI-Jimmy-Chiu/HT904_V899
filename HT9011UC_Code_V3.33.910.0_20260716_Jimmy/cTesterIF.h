//---------------------------------------------------------------------------

#ifndef cTesterIFH
#define cTesterIFH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>

//---------------------------------------------------------------------------
class TFTestIF : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *grpIFType;
    TRadioGroup *rgInterfaceType;
    TPageControl *PageControl2;
    TTabSheet *tsDio;
    TGroupBox *grpTTL;
    TGroupBox *grpTTLSetting;
    TComboBox *cbDIOType;
    TListBox *lstTTL;
    TCheckBox *AntiSignalCBox;
    TTabSheet *tsGpib;
    TGroupBox *grpGPIB;
    TGroupBox *grpGPIBType;
    TLabel *lblGPIBAddr;
    TComboBox *cbGPIBType;
    TEdit *edGPIBAddress;
    TTabSheet *tsRs232;
    TPanel *pnlBtm;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TPanel *pnlTimeSetting;
    TGroupBox *grpTestingStopTime;
    TLabel *lblInitialWaitTIme;
    TLabel *lblInitialWaitTImeSec;
    TLabel *lblTestWaitTime;
    TLabel *lblTestWaitTimeSec;
    TEdit *edtTestingWaitTime;
    TEdit *edtInitWaitTime;
    TGroupBox *gbRs232BinCount;
    TLabel *labRs232BinCount;
    TEdit *edMaxBinCount;
    TGroupBox *gbInitialDelay;
    TCheckBox *cbSpiroxTesterLotEnd;
    TPageControl *PageControl1;
    TTabSheet *rgTime1;
    TTabSheet *rgTime2;
    TLabel *Label34;
    TLabel *Label35;
    TLabel *Label3;
    TLabel *Label5;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label28;
    TLabel *Label29;
    TLabel *Label36;
    TLabel *Label37;
    TLabel *Label38;
    TLabel *Label39;
    TLabel *Label40;
    TLabel *Label41;
    TEdit *edtInitialDec1;
    TEdit *edtInitialDecCount1;
    TComboBox *iStartDelayCount;
    TEdit *edtInitialDec2;
    TEdit *edtInitialDecCount2;
    TEdit *edtInitialDec3;
    TEdit *edtInitialDecCount3;
    TEdit *edtInitialDec4;
    TEdit *edtInitialDecCount4;
    TPanel *palTime;
    TLabel *Label50;
    TLabel *Label51;
    TLabel *Label53;
    TLabel *Label58;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label49;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label52;
    TLabel *Label8;
    TLabel *Label12;
    TEdit *edMaxTestTime;
    TEdit *edInitialMaxTest;
    TEdit *edDummyTestTime;
    TEdit *edtInitStartDelay;
    TEdit *edtInitStartDelayCT;
    TEdit *edtAfterTestedDelay;
    TPanel *palStartDelay;
    TLabel *Label54;
    TLabel *Label45;
    TEdit *edStartDelayTime;
    TPanel *pnl1stDevice;
    TCheckBox *cbEveryFirstDeviceUseInitialDelay;
    TEdit *edtInitialDelay_1;
    TPanel *pnlTitle;
    TLabel *lblDelayTime;
    TLabel *lblTriggerTime;
    TPanel *pnlPause;
    TCheckBox *cbWhenPressStopOverUseInitialDelay;
    TEdit *edtInitialDelay_6;
    TEdit *edOverSec;
    TPanel *pnlLowTestTime;
    TCheckBox *cbWhenHappenTestedTimeBelowUseInitialDelay;
    TEdit *edtInitialDelay_4;
    TEdit *edBelowSec;
    TPanel *pnlHeatDoor;
    TCheckBox *cbAfterOpenHeatDoorUseInitialDelay;
    TEdit *edtInitialDelay_5;
    TPanel *pnlAutoClean;
    TEdit *edtInitialDelay_3;
    TCheckBox *cbAfterAutoCleanFunctionUseInitialDelay;
    TPanel *pnlAlarmMsg;
    TCheckBox *cbAfterShowAlarmMessageUseInitialDelay;
    TEdit *edtInitialDelay_2;
    TPanel *pnlOTD;
    TCheckBox *cbOTDUnlockDelay;
    TEdit *edtInitialDelay_9;
    TPanel *pnlEOTTime;
    TCheckBox *cbTestFinishToNextTestOver;
    TEdit *iTestFinishToNextTestOver;
    TEdit *edtInitialDelay_8;
    TPanel *pnlNoFullsite;
    TCheckBox *cbWhenNoFullSiteUseInitialDelay;
    TEdit *edtInitialDelay_7;
    TPanel *Panel39;
    TCheckBox *cbUseOtherArmToTestAfterInitialDelay;
    TTabSheet *rgPurgeAir;
    TCheckBox *cbPurgeAir;
    TEdit *edPurgeAir;
    TLabel *Label7;
    TPanel *pnlSOTTime;
    TCheckBox *cbTestStartToNextTestStart;
    TEdit *iTeststartToNextTestStart;
    TEdit *edtInitialDelay_10;
    TLabel *lblFT;
    TLabel *lblRT;
    TEdit *edtInitialDelay_1_RT;
    TEdit *edtInitialDelay_2_RT;
    TEdit *edtInitialDelay_3_RT;
    TEdit *edtInitialDelay_5_RT;
    TEdit *edtInitialDelay_4_RT;
    TEdit *edtInitialDelay_6_RT;
    TEdit *edtInitialDelay_7_RT;
    TEdit *edtInitialDelay_9_RT;
    TEdit *edtInitialDelay_8_RT;
    TEdit *edtInitialDelay_10_RT;
    TCheckBox *cbAutoOnecycleHomStart;
    TEdit *edtAutoOnecycleHomStartTime;
    TRadioGroup *rg2DID_Format;
    TLabel *labcAutoOnecycleHomStart;  //JerryYang 20200422 2DID format選項改用下拉選單
    TTabSheet *tsTCPIP;
    TButton *btTesterTCPShow;
    TCheckBox *cbASEJPMode;
    TTabSheet *tsRT;
    TTabSheet *tsEQC;
    TLabel *labEQC1;
    TLabel *labEQC3;
    TLabel *labEQC7;
    TLabel *labEQC9;
    TLabel *labEQC2;
    TLabel *labEQC8;
    TLabel *labEQC5;
    TLabel *labEQC11;
    TLabel *labEQC6;
    TLabel *labEQC12;
    TEdit *edMaxTestTime_EQC;
    TEdit *edInitialMaxTest_EQC;
    TEdit *edDummyTestTime_EQC;
    TEdit *edtInitStartDelay_EQC;
    TEdit *edtInitStartDelayCT_EQC;
    TPanel *Panel2;
    TLabel *labEQC10;
    TLabel *labEQC4;
    TEdit *edStartDelayTime_EQC;
    TLabel *labRT1;
    TLabel *labRT3;
    TLabel *labRT7;
    TLabel *labRT9;
    TLabel *labRT2;
    TLabel *labRT8;
    TLabel *labRT5;
    TLabel *labRT11;
    TLabel *labRT6;
    TLabel *labRT12;
    TEdit *edMaxTestTime_RT;
    TEdit *edInitialMaxTest_RT;
    TEdit *edDummyTestTime_RT;
    TEdit *edtInitStartDelay_RT;
    TEdit *edtInitStartDelayCT_RT;
    TPanel *Panel1;
    TLabel *labRT10;
    TLabel *labRT4;
    TEdit *edStartDelayTime_RT;
    TPageControl *pgcRS232;
    TTabSheet *tsRS232Setting;
    TGroupBox *grpRS232;
    TGroupBox *grpRS232Type;
    TComboBox *cbRs232Type;
    TCheckBox *cb_NeedSendVSOT;
    TCheckBox *cbForEgistec;
    TPanel *pnlRS232;
    TRadioGroup *rgParity;
    TRadioGroup *rgStopBit;
    TRadioGroup *rgBitLength;
    TRadioGroup *rgBaudRate;
    TGroupBox *gbBaudRate;
    TComboBox *cbbBaudRate;
    TTabSheet *tsSLTSetting;
    TLabel *labProgramerrorcode;
    TCheckBox *cbRunCheckProgramUse;
    TEdit *edtRunCheckProgramName;
    TEdit *edtRunCheckProgramNameErrCode;
    TGroupBox *GroupBox5;
    TLabel *Label43;
    TCheckBox *cbMaxBIOSWaitTimeAlm;
    TCheckBox *cbMinTestTimeAlm;
    TCheckBox *cbMaxTestTimeAlm;
    TGroupBox *GroupBox4;
    TEdit *edMinTestTimeErrCode;
    TEdit *edBIOSErrCode;
    TEdit *edMaxTestTimeErrCode;
    TGroupBox *GroupBox6;
    TLabel *Label31;
    TLabel *Label32;
    TLabel *Label33;
    TLabel *Label4;
    TLabel *Label6;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label17;
    TLabel *Label30;
    TLabel *Label18;
    TLabel *Label21;
    TLabel *Label22;
    TEdit *edTestOKWaitTime;                                                    //Richard 20230322 : limit修改  沒用到的edit
    TEdit *edSendNEXTDelay;                                                     //Richard 20230322 : limit修改  沒用到的edit
    TEdit *edMaxBIOSWaitTime;                                                   //Richard 20230322 : limit修改  沒用到的edit
    TEdit *edMinTestTime;                                                       //Richard 20230322 : limit修改  沒用到的edit
    TEdit *edPowerSwitchDelay;                                                  //Richard 20230322 : limit修改  沒用到的edit
    TEdit *edSLTMaxTestTime;
    TRadioButton *rbTemp;
    TLabel *Label23;
    TLabel *Label24;
    TEdit *edTCPIP_Address;
    TEdit *edTCPIP_Port;
    TCheckBox *cbUseSocketHeating;
    TEdit *edt_UseSocketHeating;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall edMaxTestTimeKeyPress(TObject *Sender, char &Key);
    void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
    void __fastcall rgInterfaceTypeClick(TObject *Sender);
    void __fastcall edMaxTestTimeMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Edit1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall cbDIOTypeChange(TObject *Sender);
    void __fastcall AntiSignalCBoxClick(TObject *Sender);
    void __fastcall cbRs232TypeChange(TObject *Sender);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall edMaxBinCountMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edTempOffsetCountMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edInitialMaxTestClick(TObject *Sender);
    void __fastcall edtInitialDelay_1Click(TObject *Sender);
    void __fastcall edtInitStartDelayClick(TObject *Sender);
    void __fastcall btTesterTCPShowClick(TObject *Sender);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall edTCPIP_PortClick(TObject *Sender);
    void __fastcall edPowerSwitchDelayClick(TObject *Sender);
    void __fastcall edMaxBIOSWaitTimeClick(TObject *Sender);
    void __fastcall edMinTestTimeClick(TObject *Sender);
    void __fastcall edtAutoOnecycleHomStartTimeClick(TObject *Sender);
    void __fastcall edGPIBAddressClick(TObject *Sender);
    void __fastcall edtInitialDecCount1Click(TObject *Sender);
    void __fastcall edPurgeAirClick(TObject *Sender);
private:    // User declarations
    //TStringList *MyList;
    void __fastcall ShowTTLState();
    void __fastcall ShowPageControl2(int Index);
    void __fastcall InitcbDIOType(bool bAlarm);

//jou 980716 end
public:     // User declarations
    __fastcall TFTestIF(TComponent* Owner);
    void __fastcall SaveFile();
    void __fastcall ReadTestIFFile();
    void __fastcall DoIniDataToForm();
    void __fastcall SaveSetupFile(AnsiString szDir,AnsiString S);//kevin 20150105
    void __fastcall CheckRs232StandardIni();    //Sam 20181219 : Handler 與 Rs232Standard 設定同步
    bool fShow;
    bool bIsResetRs232Standard;                 //Sam 20181219 : Handler 與 Rs232Standard 設定同步
    int ioldTestType;    //0:TTL_MODE ; 1: GPIB_MODE ; 2: RS232_MODE     //Isaac 20200903 :TTL RS232通訊
    int ioldDIOType;
    void __fastcall SendSLTState();      //Isaac 20210331
    //Sam 20201209 : Default Recipe ChangeLog
    //==>
    void __fastcall DoSetRPDefault(TWinControl *PCtrl,int iSection);
    void __fastcall DoReplyDefaultToForm(TWinControl *PCtrl,int iSection);
    void __fastcall SearchRecipeParameter(TWinControl *PCtrl,int iSection);
    //<==
    //Sam 20201209 : Default Recipe ChangeLog
};
//---------------------------------------------------------------------------
extern PACKAGE TFTestIF *FTestIF;
//---------------------------------------------------------------------------
#endif
