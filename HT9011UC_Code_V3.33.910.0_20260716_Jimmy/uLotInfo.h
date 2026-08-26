//---------------------------------------------------------------------------

#ifndef uLotInfoH
#define uLotInfoH
//---------------------------------------------------------------------------
#include "ALed.hpp"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <FileCtrl.hpp>
#include <Grids.hpp>
#include <Forms.hpp>
#include "HTray.h"
#include "SPComm.hpp"
#include <Dialogs.hpp>
#include "BtnPanelLane.h"
#include "butPa1.h"
//---------------------------------------------------------------------------
class TfLotInfo : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pgLotinfo;
    TTabSheet *tsDeviceInfo;
    TTabSheet *tsLotID;
    TTabSheet *tsFTP;
    TTabSheet *tsRTCFullViewImg;
    TTimer *Timer1;
    TTabSheet *tsATC;
    TPanel *pan_ATCInterface;
    TPanel *palATC;
    TTabSheet *ts_OCRInterface;
    TTimer *Timer2;
    TTabSheet *ts_SocketInterface;
    TTabSheet *tsBarCode;
    TPanel *Panel1;
    TPanel *palATCWorkingTemp;
    TTabSheet *tsSelection;
    TTabSheet *ts_AutoCleanMonitor;
    TPanel *palAutoCleanMonitor;
    TGroupBox *GroupBox1;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TEdit *edtAutoCleanLowYield;
    TEdit *edtAutoCleanSiteYieldDiff;
    TGroupBox *GroupBox2;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label21;
    TEdit *edtAutoCleanContactCount;
    TTabSheet *ts_AutoRetestMonitor;
    TPanel *palAutoRetestMonitor;
        TPanel *ART_Panel;
        TLabel *LART_Limit;
        TEdit *eART_Limit;
        TEdit *eART_Count;
        TLabel *LART_Count;
        TLabel *LART_Passlimit;
        TEdit *eART_Passlimit;
        TEdit *eART_PassYield;
        TLabel *LART_PassYield;
        TLabel *LART_Openlimit;
        TLabel *LART_OpenYield;
        TEdit *eART_Openlimit;
        TEdit *eART_OpenYield;
        TLabel *LART_RecoverPasslimit;
        TEdit *eART_RecoverPasslimit;
        TLabel *LART_RecoverPassYield;
        TEdit *eART_RecoverPassYield;
        TLabel *Label33;
    TTabSheet *tsOCRBarCode;
    TMemo *Memo2;
    TTimer *Timer3;
    TTimer *Timer4;                                                        //Eastsun 20260515 F020
    TTabSheet *tsESDMonitor;
    TPanel *pnlESDMonitor;
    TTimer *NetATCTime;
    TPanel *pnl1;
    TPanel *pnl2;
    TPanel *pnl3;
    TPanel *pnl4;
    TPanel *pl_ESDProx1_1;
    TPanel *pl_ESDProx2_1;
    TPanel *pl_ESDProx1_2;
    TPanel *pl_ESDProx2_2;
    TPanel *pl_ESDProx1_3;
    TPanel *pl_ESDProx2_3;
    TPanel *pl_ESDProx3_1;
    TPanel *pl_ESDProx3_2;
    TPanel *pl_ESDProx3_3;
    TPanel *pnl14;
    TPanel *pnl15;
    TPanel *pnl16;
    TPanel *pl_PositiveDecay3_3;
    TPanel *pl_PositiveDecay3_2;
    TPanel *pl_PositiveDecay3_1;
    TPanel *pl4;
    TPanel *pl5;
    TPanel *pl6;
    TPanel *pl7;
    TPanel *pl_PositiveDecay1_1;
    TPanel *pl_PositiveDecay2_1;
    TPanel *pl_PositiveDecay2_2;
    TPanel *pl_PositiveDecay1_2;
    TPanel *pl12;
    TPanel *pl13;
    TPanel *pl_PositiveDecay1_3;
    TPanel *pl_PositiveDecay2_3;
    TPanel *pl_NegativeDecay3_3;
    TPanel *pl_NegativeDecay3_2;
    TPanel *pl_NegativeDecay3_1;
    TPanel *pl19;
    TPanel *pl20;
    TPanel *pl21;
    TPanel *pl22;
    TPanel *pl_NegativeDecay1_1;
    TPanel *pl_NegativeDecay2_1;
    TPanel *pl_NegativeDecay2_2;
    TPanel *pl_NegativeDecay1_2;
    TPanel *pl27;
    TPanel *pl28;
    TPanel *pl_NegativeDecay1_3;
    TPanel *pl_NegativeDecay2_3;
    TPanel *pl31;
    TPanel *pl32;
    TPanel *pl34;
    TPanel *pnl5;
    TPanel *pl_ESDProx4_1;
    TPanel *pl_ESDProx4_2;
    TPanel *pl_ESDProx4_3;
    TPanel *pnl9;
    TPanel *pl_ESDProx5_1;
    TPanel *pl_ESDProx5_2;
    TPanel *pl_ESDProx5_3;
    TPanel *pnl13;
    TPanel *pl_ESDProx6_1;
    TPanel *pl_ESDProx6_2;
    TPanel *pl_ESDProx6_3;
    TPanel *pnl20;
    TPanel *pl_PositiveDecay4_1;
    TPanel *pl_PositiveDecay4_2;
    TPanel *pl_PositiveDecay4_3;
    TPanel *pnl24;
    TPanel *pl_PositiveDecay5_1;
    TPanel *pl_PositiveDecay5_2;
    TPanel *pl_PositiveDecay5_3;
    TPanel *pnl28;
    TPanel *pl_PositiveDecay6_1;
    TPanel *pl_PositiveDecay6_2;
    TPanel *pl_PositiveDecay6_3;
    TPanel *pnl32;
    TPanel *pl_NegativeDecay4_1;
    TPanel *pl_NegativeDecay4_2;
    TPanel *pl_NegativeDecay4_3;
    TPanel *pnl36;
    TPanel *pl_NegativeDecay5_1;
    TPanel *pl_NegativeDecay5_2;
    TPanel *pl_NegativeDecay5_3;
    TPanel *pnl40;
    TPanel *pl_NegativeDecay6_1;
    TPanel *pl_NegativeDecay6_2;
    TPanel *pl_NegativeDecay6_3;
    TLabel *lbESDReportData;
    TLabel *lbESDDecayReportData;
    TMemo *Memo3;
    TStringGrid *sgATRCount;
    TPanel *Panel6;
    TTimer *TimerERMS;
    TPanel *Panel9;
    TSpeedButton *spOCRSaveLog;
    TLabel *Label40;
    TSpeedButton *spOCRChangeFile;
    TLabel *Label41;
    TLabel *Label42;
    TSpeedButton *spOCRLogIn;
    TLabel *Label36;
    TLabel *Label37;
    TLabel *Label38;
    TLabel *Label39;
    TLabel *Label43;
    TLabel *lbOCRNowFile;
    TLabel *lbOCRUseFile;
    TEdit *edOCRLotID;
    TStringGrid *sgOCR;
    TGroupBox *GroupBox3;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label22;
    TLabel *Label27;
    TLabel *Label28;
    TLabel *Label30;
    TLabel *Label46;
    TEdit *edInputAmount;
    TEdit *edLoadingCnt;
    TEdit *edAuto1Cnt;
    TEdit *edAuto2Cnt;
    TEdit *edAuto3Cnt;
    TEdit *edARTMaxLimit;
    TEdit *edCurrARTCnt;
    TEdit *edARTPassLimit;
    TEdit *Edit16;
    TPanel *palCurrFailRate;
    TLabel *Label47;
    TEdit *edCurrARTRate;
    TPanel *palFTRateLmt;
    TLabel *Label29;
    TEdit *Edit10;
    TEdit *Edit11;
    TEdit *Edit12;
    TPanel *palRTRateLmt;
    TLabel *Label45;
    TEdit *Edit13;
    TEdit *Edit14;
    TEdit *Edit15;
    TTabSheet *tsASEMARMS;
    TPanel *pnARMSTitle;
    TLabel *Label48;
    TLabel *eART_PasslimitL;
    TLabel *eART_OpenlimitL;
    TLabel *Label49;
    TLabel *Label50;
    TLabel *eART_RecoverPasslimitL;
    TLabel *Label34;
    TLabel *Label35;
    TLabel *Label51;
    TEdit *edAuto1Cnt_KS;
    TEdit *edAuto2Cnt_KS;
    TEdit *edAuto3Cnt_KS;
    TTabSheet *tsASECLEventLog;
    TPanel *Panel10;
    TScrollBox *ScrollBox1;
    TPanel *pnlLotStart_ASECL;
    TSpeedButton *btnASECL_LotStart;
    TSpeedButton *btnASECL_LotEnd;
    TPanel *Panel12;
    TPanel *Panel13;
    TPanel *Panel14;
    TLabel *Label44;
    TGroupBox *groupbDownloadItem;
    TCheckBox *chkTempOffset;
    TCheckBox *chkContactHigh;
    TCheckBox *chkContactForce;
    TCheckBox *chkContactMode;
    TCheckBox *chkHotPlate;
    TCheckBox *chkLoadUnload;
    TCheckBox *chkSpeedSetting;
    TCheckBox *chkShuttleMode;
    TButton *btnSave;
    TCheckBox *chkTestMode;
    TCheckBox *chkBinasgn;
    TCheckBox *chkBinasgnOff;
    TCheckBox *checkbAutoClean;
    TCheckBox *chkIndexHeatingMode;
    TPanel *Panel16;
    TPanel *Panel17;
    TStringGrid *sgBarcode;
    TButton *btClearBarcodeCount;
    TButton *btClearBarcodeList;
    TButton *btChangeFile;
    TPanel *Panel18;
    TPanel *Panel19;
    TPanel *Panel20;
    TSpeedButton *sbARMSShow;
    TPanel *Panel21;
    TPanel *Panel22;
    TLabel *Label12;
    TEdit *edtHandlerID;
    TMemo *Memo1;
    TSpeedButton *OnLine;
    TPanel *Panel23;
    TLabel *lbl1;
    TLabel *lbl2;
    TLabel *lblTester_LotID;
    TLabel *Label11;
    TLabel *lblOCR_LotID;
    TImage *Image1;
    TMemo *mmoHandlerWithTester;
    TPanel *palHandlerwithTester;
    TPanel *Panel24;
    TPanel *Panel25;
    TButton *btnFtpServer;
    TButton *btnFtpHD;
    TButton *btnFtpTester;
    TButton *btSaveSetupFile;
    TButton *btnDataFTPSaveToData;
    TPanel *Panel26;
    TPanel *Panel27;
    TPanel *Panel8;
    TPanel *palSecsGem;
    TLabel *labLotID;
    TSpeedButton *sbSECSLotStart;
    TSpeedButton *sbSECSLotEnd;
    TSpeedButton *spSECSLotCheck;
    TEdit *edtSysLotID;
    TPanel *Panel28;
    TLabel *lblOPID;
    TLabel *lbLotRunMode;
    TEdit *edtSysOperatorID;
    TComboBox *cbRunMode;
    TPanel *Panel29;
    TPanel *Panel30;
    TLabel *Label153;
    TLabel *Label5;
    TLabel *labLevelMode;
    TEdit *edDeviceName;
    TButton *btDownload;
    TButton *btUpload;
    TEdit *edTemp;
    TComboBox *cbbDeviceName;
    TFileListBox *FileListBox1;
    TFileListBox *FileListBox2;
    TComboBox *coLevelMode;
    TPanel *Panel31;
    TPanel *Panel32;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TALed *ALed1;
    TALed *ALed2;
    TALed *ALed3;
    TButton *btStartCount;
    TImage *imgRTCFullView2;
    TImage *imgRTCFullView1;
    TImage *imgRTCFullView3;
    TImage *imgRTCFullView4;
    TLabel *lbSocketIDRowA;
    TEdit *edtSocketAa;
    TEdit *edtSocketAb;
    TEdit *edtSocketAc;
    TEdit *edtSocketAd;
    TLabel *lbSocketIDRowB;
    TEdit *edtSocketAe;
    TEdit *edtSocketAf;
    TEdit *edtSocketAg;
    TEdit *edtSocketAh;
    TLabel *lbSocketIDRowC;
    TEdit *edtSocketBa;
    TEdit *edtSocketBb;
    TEdit *edtSocketBc;
    TEdit *edtSocketBd;
    TLabel *lbSocketIDRowD;
    TEdit *edtSocketBe;
    TEdit *edtSocketBf;
    TEdit *edtSocketBg;
    TEdit *edtSocketBh;
    TEdit *edtSocketCa;
    TEdit *edtSocketCb;
    TEdit *edtSocketCc;
    TEdit *edtSocketCd;
    TEdit *edtSocketCe;
    TEdit *edtSocketCf;
    TEdit *edtSocketCg;
    TEdit *edtSocketCh;
    TEdit *edtSocketDa;
    TEdit *edtSocketDb;
    TEdit *edtSocketDc;
    TEdit *edtSocketDd;
    TEdit *edtSocketDe;
    TEdit *edtSocketDf;
    TEdit *edtSocketDg;
    TEdit *edtSocketDh;
    TLabel *lbSocketIDColA;
    TLabel *lbSocketIDColB;
    TLabel *lbSocketIDColC;
    TLabel *lbSocketIDColD;
    TLabel *lbSocketIDColE;
    TLabel *lbSocketIDColF;
    TLabel *lbSocketIDColG;
    TLabel *lbSocketIDColH;
    TPanel *palAa;
    TPanel *palAb;
    TPanel *palAc;
    TPanel *palAd;
    TPanel *palAg;
    TPanel *palAh;
    TPanel *palAf;
    TPanel *palAe;
    TPanel *palBa;
    TPanel *palBb;
    TPanel *palBc;
    TPanel *palBd;
    TPanel *palBg;
    TPanel *palBh;
    TPanel *palBf;
    TPanel *palBe;
    TPanel *palCa;
    TPanel *palCb;
    TPanel *palCc;
    TPanel *palCd;
    TPanel *palCg;
    TPanel *palCh;
    TPanel *palCf;
    TPanel *palCe;
    TPanel *palDa;
    TPanel *palDb;
    TPanel *palDc;
    TPanel *palDd;
    TPanel *palDg;
    TPanel *palDh;
    TPanel *palDf;
    TPanel *palDe;
    TPanel *palQAMode;
    TButton *btnQAmodeSave;
    TEdit *edQAMode;
    TLabel *labQACount;
    TGroupBox *grpBarcodeDisplayLotInfo;
    TTMyTray *myInShuttleLotInfo;
    TTMyTray *mtBarcodeInShLotInfo;
    TTimer *LotKeyInTime;
    TCheckBox *chkART;
    TLabel *lblPage;
    TEdit *edPage;
    TCheckBox *chkART_RTCount;
    TTabSheet *tsChamberBoost;
    TPanel *Panel33;
    TSpeedButton *btnStopChamberBoost;
    TSpeedButton *SpeedButton1;
    TSpeedButton *btnStartChamberBoost;
    TPanel *pnlChamberBoost;
    TTimer *tmrChamberBoost;
    TButton *btnFTPTryConnect;
    TLabel *lbFTPStatus;
    TSpeedButton *btnSaveData;
    TCheckBox *cbBottom2DOffset;
    TTabSheet *tsRFMD;
    TButton *btnESCFunction;
    TButton *btnCancelTestPause;
    TPanel *lbCheckCodeByLot;
    TTabSheet *ts_FTPAutomation;
    TGroupBox *gbFTPAutomation_Download;
    TLabel *lblN06_Host01;
    TLabel *lblN06_UserName01;
    TLabel *lblN06_Password01;
    TLabel *labN06_DownloadPath;
    TEdit *edFTPAutomation_Down_ServerPath;
    TEdit *edFTPAutomation_Down_Password;
    TEdit *edFTPAutomation_Down_UserID;
    TEdit *edFTPAutomation_Down_ServerIP;
    TGroupBox *gbFTPAutomation_Upload;
    TLabel *lblN06_Host02;
    TLabel *lblN06_UserName02;
    TLabel *lblN06_Password02;
    TLabel *lblN06_UploadPath;
    TLabel *lblN06_UploadErrPath;
    TEdit *edFTPAutomation_Up_ServerPath;
    TEdit *edFTPAutomation_Up_Password;
    TEdit *edFTPAutomation_Up_UserID;
    TEdit *edFTPAutomation_Up_ServerIP;
    TEdit *edFTPAutomation_Up_ErrorCodePath;
    TSpeedButton *sbTest;
    TSpeedButton *sbRecipeUpload;
    TSpeedButton *sbRecipeDownload;
    TSpeedButton *sbFTPAutomationSave;
    TTabSheet *tsYieldMonitior;
    TTabSheet *tsTesterLog;
    TPanel *Panel35;
    TLabel *labTCPIPStatus;
    TLabel *labTCPIPSimulate;
    TButton *btTesterTCPShow;
    TMemo *mmTesterLog;
    TLabel *lbProcess;
    TComboBox *cbProcess;
    TGroupBox *grpRFID;
    TPanel *pnlLoader;
    TButton *btnGetLoader;
    TMemo *mmRFID;
    TComm *RFID_Reader;
    TPageControl *pgcYieldMonitor;
    TTabSheet *tsSigurd;
    TPanel *pnlYieldMonitor;
    TGroupBox *gbRMS;
    TLabel *lblMonitor_Contact;
    TCheckBox *cbMonitor_FTPRMS;
    TComboBox *cbContactMode;
    TGroupBox *gbCmpBySite;
    TLabel *lblSiteYieldCmp_Cur;
    TLabel *lblSiteYieldCmpIg_Cur;
    TCheckBox *cbSiteYieldCmp_FT;
    TEdit *edSiteYieldCmp_FT;
    TEdit *edSiteYieldCmpIg_FT;
    TEdit *edSiteYieldCmpIg_Cur;
    TEdit *edSiteYieldCmp_Cur;
    TEdit *edSiteYieldCmpOnOff_Cur;
    TGroupBox *gbLowYieldByTotal;
    TLabel *lblLowYieldByTotal_Cur;
    TLabel *lblLowYieldByTotalIg_Cur;
    TCheckBox *cbLowYieldByTotal_FT;
    TEdit *edLowYieldByTotalIg_FT;
    TEdit *edLowYieldByTotal_FT;
    TEdit *edLowYieldByTotalIg_Cur;
    TEdit *edLowYieldByTotal_Cur;
    TEdit *edLowYieldByTotalOnOff_Cur;
    TGroupBox *gbContsFailBySocket;
    TLabel *labContsFailBySocket_FT;
    TLabel *lblContsFailSocketAlarmCT_Cur;
    TRadioButton *rbContsFailBySocket_FTOn;
    TRadioButton *rbContsFailBySocket_FTOff;
    TEdit *edContsFailSocketAlarmCT_FT;
    TEdit *edContsFailSocketAlarmCT_Cur;
    TEdit *edtContsFailBySocket_Cur;
    TGroupBox *gbContsFailByHead;
    TLabel *labContsFailByHead_FT;
    TLabel *lblContsFailHeadAlarmCT_Cur;
    TRadioButton *rbContsFailByHead_FTOn;
    TRadioButton *rbContsFailByHead_FTOff;
    TEdit *edContsFailHeadAlarmCT_FT;
    TEdit *edContsFailHeadAlarmCT_Cur;
    TEdit *edtContsFailByHead_Cur;
    TGroupBox *grpOSBin;
    TLabel *lblOSBin;
    TLabel *lblOSBin_Cur;
    TEdit *edOSBin;
    TEdit *edOSBinPreset;
    TEdit *edOSBinCnt;
    TEdit *edOSBinCnt_Cur;
    TEdit *edOSBinPreset_Cur;
    TGroupBox *gbManualCheckList;
    TButton *btnManualCheckList;
    TButton *btnManualStandard;
    TTabSheet *tsTPW;
    TGroupBox *gbLowYield;
    TCheckBox *cbLowYield;
    TEdit *edLowYieldIg;
    TEdit *edLowYield;
    TGroupBox *GroupBox4;
    TLabel *lblContsFailByHead;
    TRadioButton *rbContsFailByHead_On;
    TRadioButton *rbContsFailByHead_Off;
    TEdit *edContsFailHeadAlarmCT;
    TGroupBox *GroupBox5;
    TLabel *lblContsFailBySocket;
    TRadioButton *rbContsFailBySocket_On;
    TRadioButton *rbContsFailBySocket_Off;
    TEdit *edContsFailSocketAlarmCT;
    TGroupBox *gb_HeadToHeadYieldEnable;
    TCheckBox *cb_HeadToHeadYieldEnable;
    TEdit *ed_HeadToHeadYield;
    TEdit *ed_HeadToHeadYieldCount;
    TGroupBox *gbAllSiteFail;
    TLabel *lblAllSiteFail;
    TCheckBox *cbAllSiteFail;
    TEdit *edAllSiteFailCount;
    TGroupBox *gb_SiteToSiteYieldEnable;
    TCheckBox *cb_SiteToSiteYieldEnable;
    TEdit *ed_SiteToSiteYield;
    TEdit *ed_SiteToSiteYieldCount;
        TTabSheet *ATC_WinWay;
        TPanel *pnlWinwayCH1;
        TPanel *pnlWinwayCH2;
        TPanel *pnlWinwayCH3;
        TPanel *pnlWinwayCH4;
        TPanel *pnlWinwaySTCH1;
        TPanel *pnlWinwaySTCH2;
        TPanel *pnlWinwaySTCH3;
        TPanel *pnlWinwaySTCH4;
        TPanel *pnlWinwayPVCH1;
        TPanel *pnlWinwayPVCH2;
        TPanel *pnlWinwayPVCH3;
        TPanel *pnlWinwayPVCH4;
    TOpenDialog *OpenDialog1;
    TPanel *pnlLotInfo_ASECL;
    TLabel *lb_Main_LotName;
    TLabel *lb_Main_EventLogOPID;
    TLabel *lb_Main_EventLogTesterID;
    TLabel *lb_Main_EventLogLoginMode;
    TSpeedButton *sb_Main_EvenLevelLogin;
    TLabel *lb_Main_TesterLoadBoard;
    TEdit *edtASECL_LotID;
    TEdit *edtASECL_OPID;
    TEdit *edtASECL_TesterID;
    TComboBox *cbbASECL_LoginMode;
    TEdit *edtASECL_LoadBoard;
    TPageControl *pgcLotInfo;
    TTabSheet *tsMurata;
    TTabSheet *tsSigurd_CX;
    TLabeledEdit *edtLine;
    TLabeledEdit *edtProcessName;
    TLabeledEdit *edtProduct;
    TPanel *pnlXMLOnLine;
    TLabel *labCustomerLotId;
    TEdit *edCustomerLotId;
    TComboBox *coStation;
    TLabel *labStation;
    TEdit *edStationNum;
    TLabel *labStationNum;
    TTabSheet *tsSPIL_SZ;
    TLabel *labJobSeq;
    TEdit *edtJobSeq;
    TTabSheet *tsOEE;
    TPanel *pal_PIOEELotStatus;
    TLabel *lbl_PIOEELotStatus;
    TLabel *lb_PIOEELotStatus;
    TEdit *ed_PIOEEMO;
    TLabel *lbl_PIOEEMO;
    TSpeedButton *sb_RunExecutFile;
    TTabSheet *ts2DSort;
    TTabSheet *tsChipAdv;
    TLabeledEdit *lbledtStarTime;
    TLabeledEdit *lbledtEndTime;
    TLabeledEdit *lbledtTesterOsVer;
    TLabeledEdit *lbledtCustomer;
    TLabeledEdit *lbledtTestProg;
    TLabeledEdit *lbledtDeviceName;
    TLabeledEdit *lbledtTesterID;
    TLabeledEdit *lbledtSubLotNo;
    TLabeledEdit *lbledtTestCode;
    TLabeledEdit *lbledtMachineID;
    TLabeledEdit *lbledtTestBinNo;
    TLabeledEdit *lbledtModeCode;
    TLabel *labLowYieldICCount;
    TBtnPanelLane *btnVerifyMode;
    TLabel *labCusLotID;
    TLabel *labCusDevGrp;
    TLabel *labDeviceName;
    TEdit *edtCusLotID;
    TEdit *edtCusDevGrp;
    TEdit *edtDevice;
    TGroupBox *grp_SmartAutoClean;
    TLabel *lbl_SmartAutoCleanCount;
    TLabel *lblAdaptiveIntervalCount;
    TLabel *lblAdaptiveLowYield;
    TLabel *lbl_SmartAutoCleanCount_CTF;
    TCheckBox *chk_SmartAutoClean;
    TEdit *edt_SmartAutoClean;
    TEdit *ed_SmartAutoCleanCTF;
    TCheckBox *cbRTCASTD;
    TCheckBox *chkCleanCount;
    TSpeedButton *spOCRCleanList;
    TTabSheet *ts_ATC6_1;
    TPanel *pnl_ATC61;
    TLabel *lblDryAirSwSts;
    TLabel *lblTesterDryAirSwSts;
    TPanel *Panel34;
    TGroupBox *grp_DewPoint_1032;
    TLabel *Label52;
    TLabel *Label57;
    TLabel *Label58;
    TPanel *hlpal1;
    TPanel *pnl_DewPoint_InArm;
    TPanel *hlpal2;
    TPanel *pnl_DewPoint_OutArm;
    TPanel *hlpal3;
    TPanel *pnl_DewPoint_Index;
    TPanel *palATC61Status;
    TPageControl *pgcATC61;
    TTabSheet *ts1;
    TGroupBox *grp_AirStreamStatus;
    TLabel *lblAirStreamStatus;
    TLabel *Label60;
    TLabel *lbl5;
    TLabel *lbl6;
    TLabel *lbl7;
    TLabel *lbl12;
    TLabel *lbl13;
    TLabel *lbl14;
    TLabel *lbl15;
    TLabel *lbl16;
    TLabel *Label61;
    TLabel *Label62;
    TLabel *Label65;
    TPanel *palAirDefrostSec;
    TPanel *palAirMachineAlarmStatus;
    TPanel *palAirMachineSetTemperature;
    TPanel *palAirMachineStatus;
    TPanel *pal_Air_Machine_Temp_Ch1;
    TPanel *pal_Air_Machine_Temp_Ch2;
    TPanel *pal_AirStream_AirVolume;
    TPanel *palAirMachineStatus_Index;
    TPanel *palAirMachineSetTemperature_Index;
    TPanel *palAirMachineAlarmStatus_Index;
    TPanel *palAirDefrostSec_Index;
    TPanel *pnl_AirMachineTemp_Index_Ch_1;
    TPanel *pnl_AirMachineTemp_Index_Ch_2;
    TPanel *pal_AirStream_AirVolume_Index;
    TButton *btnAirStreamOnOff;
    TCheckBox *chk_ManualControl_AirStream;
    TTabSheet *ts_RefrigerantStatus;
    TPageControl *PageControl2;
    TTabSheet *ts_RefrigerantStatus_Page_1;
    TLabel *labRefrigerantMachineLowLimit;
    TLabel *labRefrigerantMachineHighLimit;
    TPanel *pnlRefrigerantMachine2;
    TGroupBox *gbRefrigerantMachine2;
    TLabel *LabRefrigerantValue2;
    TLabel *LabRefrigerantAdjustValue2;
    TPanel *pnlRefCopm1Status_2;
    TPanel *pnlRefCopm2Status_2;
    TGroupBox *GroupBox10;
    TLabel *labRefCopm1HpValue_2;
    TLabel *labRefCopm1LpValue_2;
    TGroupBox *GroupBox11;
    TLabel *labRefCopm2HpValue_2;
    TLabel *labRefCopm2LpValue_2;
    TPanel *pnlRefrigerantMachine3;
    TGroupBox *gbRefrigerantMachine3;
    TLabel *LabRefrigerantValue3;
    TLabel *LabRefrigerantAdjustValue3;
    TPanel *pnlRefCopm1Status_3;
    TPanel *pnlRefCopm2Status_3;
    TGroupBox *GroupBox12;
    TLabel *labRefCopm1HpValue_3;
    TLabel *labRefCopm1LpValue_3;
    TGroupBox *GroupBox13;
    TLabel *labRefCopm2HpValue_3;
    TLabel *labRefCopm2LpValue_3;
    TPanel *pnlRefrigerantMachine1;
    TGroupBox *gbRefrigerantMachine1;
    TLabel *LabRefrigerantValue1;
    TLabel *LabRefrigerantAdjustValue1;
    TPanel *pnlRefCopm1Status_1;
    TPanel *pnlRefCopm2Status_1;
    TGroupBox *GroupBox8;
    TLabel *labRefCopm1HpValue_1;
    TLabel *labRefCopm1LpValue_1;
    TGroupBox *GroupBox9;
    TLabel *labRefCopm2HpValue_1;
    TLabel *labRefCopm2LpValue_1;
    TPanel *pnlRefrigerantMachine4;
    TGroupBox *gbRefrigerantMachine4;
    TLabel *LabRefrigerantValue4;
    TLabel *LabRefrigerantAdjustValue4;
    TPanel *pnlRefCopm1Status_4;
    TPanel *pnlRefCopm2Status_4;
    TGroupBox *GroupBox15;
    TLabel *labRefCopm1HpValue_4;
    TLabel *labRefCopm1LpValue_4;
    TGroupBox *GroupBox17;
    TLabel *labRefCopm2HpValue_4;
    TLabel *labRefCopm2LpValue_4;
    TTabSheet *ts_RefrigerantStatus_Page_2;
    TPanel *pnlRefrigerantMachine5;
    TGroupBox *gbRefrigerantMachine5;
    TLabel *LabRefrigerantValue5;
    TLabel *LabRefrigerantAdjustValue5;
    TPanel *pnlRefCopm1Status_5;
    TPanel *pnlRefCopm2Status_5;
    TGroupBox *GroupBox14;
    TLabel *labRefCopm1HpValue_5;
    TLabel *labRefCopm1LpValue_5;
    TGroupBox *GroupBox16;
    TLabel *labRefCopm2HpValue_5;
    TLabel *labRefCopm2LpValue_5;
    TPanel *pnlRefrigerantMachine6;
    TGroupBox *gbRefrigerantMachine6;
    TLabel *LabRefrigerantValue6;
    TLabel *LabRefrigerantAdjustValue6;
    TPanel *pnlRefCopm1Status_6;
    TPanel *pnlRefCopm2Status_6;
    TGroupBox *GroupBox18;
    TLabel *labRefCopm1HpValue_6;
    TLabel *labRefCopm1LpValue_6;
    TGroupBox *GroupBox19;
    TLabel *labRefCopm2HpValue_6;
    TLabel *labRefCopm2LpValue_6;
    TPanel *pnlRefrigerantMachine7;
    TGroupBox *gbRefrigerantMachine7;
    TLabel *LabRefrigerantValue7;
    TLabel *LabRefrigerantAdjustValue7;
    TPanel *pnlRefCopm1Status_7;
    TPanel *pnlRefCopm2Status_7;
    TGroupBox *GroupBox20;
    TLabel *labRefCopm1HpValue_7;
    TLabel *labRefCopm1LpValue_7;
    TGroupBox *GroupBox21;
    TLabel *labRefCopm2HpValue_7;
    TLabel *labRefCopm2LpValue_7;
    TPanel *pnlRefrigerantMachine8;
    TGroupBox *gbRefrigerantMachine8;
    TLabel *LabRefrigerantValue8;
    TLabel *LabRefrigerantAdjustValue8;
    TPanel *pnlRefCopm1Status_8;
    TPanel *pnlRefCopm2Status_8;
    TGroupBox *GroupBox22;
    TLabel *labRefCopm1HpValue_8;
    TLabel *labRefCopm1LpValue_8;
    TGroupBox *GroupBox23;
    TLabel *labRefCopm2HpValue_8;
    TLabel *labRefCopm2LpValue_8;
    TLabel *labConfigL04;
    TGroupBox *grpMesCheck;
    TCheckBox *chkStopYield;
    TCheckBox *chkConsecutiveFailure;
    TRadioGroup *rgHomeStopPos;
    TTabSheet *tsVTest;
    TGroupBox *grpOEEState;
    TLabel *labOEEState;
    TStringGrid *sgOEEState;
    TRadioGroup *rgOEEState;
    TTabSheet *tsBundle;
    TPanel *Panel36;
    TLabel *labNowAuto1TrayID;
    TLabel *Label67;
    TLabel *Label55;
    TLabel *labNowAuto2TrayID;
    TLabel *Label70;
    TLabel *labNowAuto3TrayID;
    TLabel *Label72;
    TLabel *labAuto1TrayID;
    TLabel *Label74;
    TLabel *labAuto2TrayID;
    TLabel *Label76;
    TLabel *labAuto3TrayID;
    TLabel *lbl3;
    TLabel *labAuto1TrayCount;
    TLabel *Label56;
    TLabel *labAuto2TrayCount;
    TLabel *Label59;
    TLabel *labAuto3TrayCount;
    TLabel *lbl10;
    TLabel *lbl11;
    TLabel *labLoaderEnable;
    TLabel *labEmptyEnable;
    TLabel *lablbl9;
    TLabel *lab46;
    TLabel *labLotTrayCount;
    TLabel *labLotFinishTrayCount;
    TLabel *lab47;
    TPanel *Panel37;
    TPanel *palClearLotInfo;
    TPanel *palRemoveTray;
        TLabel *labLoader;
        TLabel *labLoaderBundleID;
        TLabel *lbl4;
        TLabel *lblLoaderCarBundleID;
    TLabel *lblAutoCount;
    TLabel *lblAutoCount2;
    TLabel *lblAutoCount3;
    TLabeledEdit *edtLotVerify;
    TBtnPanel *BtnPause;
    TGroupBox *grpManualSet2D;
    TLabel *Label53;
    TLabel *Label54;
    TRadioGroup *rgSort2DID;
    TEdit *edSort2DIDLotID;
    TEdit *edSort2DIDBinFile;
    TButton *btnLoadFile;
    TGroupBox *grp2DLotInfo;
    TMemo *mmo2DLotInfo;
    TPanel *pnlATC_Flag;
    TALed *aldATCPower;
    TALed *aldATCChillerStatus;
    TLabel *Label9;
    TLabel *lblChiller;
    TPanel *pl_ATC_Online;
    TButton *btnClearTemperature;
    TLabel *lblATC_Now_RecipeFile;
    TPanel *pan_ATCChillerSV;
    TPanel *pl_ATCChillerSV;
    TPanel *pan_DewPoint;
    TPanel *pl_DewPoint;
    TPanel *Pan_ATC_Use_8Head;
    TPanel *pan_ATCTempHead05;
    TPanel *pan_ATCTempHead06;
    TPanel *pan_ATCTempHead07;
    TPanel *pan_ATCTempHead08;
    TPanel *pl_ATCTempHead05;
    TPanel *pl_ATCTempHead06;
    TPanel *pl_ATCTempHead07;
    TPanel *pl_ATCTempHead08;
    TPanel *pl_ATCRefHead05;
    TPanel *pl_ATCRefHead07;
    TPanel *pl_ATCRefHead08;
    TPanel *pl_ATCRefHead06;
    TPanel *Pan_ATC_Use_4Head;
    TPanel *pan_ATCTempHead01;
    TPanel *pl_ATCTempHead01;
    TPanel *pan_ATCTempHead02;
    TPanel *pl_ATCTempHead02;
    TPanel *pan_ATCTempHead03;
    TPanel *pl_ATCTempHead03;
    TPanel *pan_ATCTempHead04;
    TPanel *pl_ATCTempHead04;
    TPanel *pl_ATCRefHead01;
    TPanel *pl_ATCRefHead02;
    TPanel *pl_ATCRefHead03;
    TPanel *pl_ATCRefHead04;
    TPanel *Pan_ATC_Use_32Head;
    TPanel *pan_ATCTempHead09;
    TPanel *pl_ATCRefHead09;
    TPanel *pan_ATCTempHead10;
    TPanel *pl_ATCRefHead10;
    TPanel *pan_ATCTempHead11;
    TPanel *pl_ATCRefHead11;
    TPanel *pan_ATCTempHead12;
    TPanel *pl_ATCRefHead12;
    TPanel *pl_ATCTempHead09;
    TPanel *pl_ATCTempHead10;
    TPanel *pl_ATCTempHead11;
    TPanel *pl_ATCTempHead12;
    TPanel *pan_ATCTempHead13;
    TPanel *pan_ATCTempHead14;
    TPanel *pan_ATCTempHead15;
    TPanel *pan_ATCTempHead16;
    TPanel *pl_ATCRefHead13;
    TPanel *pl_ATCRefHead14;
    TPanel *pl_ATCRefHead15;
    TPanel *pl_ATCRefHead16;
    TPanel *pl_ATCTempHead13;
    TPanel *pl_ATCTempHead15;
    TPanel *pl_ATCTempHead16;
    TPanel *pl_ATCTempHead14;
    TPanel *pan_ATCTempHead17;
    TPanel *pl_ATCRefHead17;
    TPanel *pan_ATCTempHead18;
    TPanel *pl_ATCRefHead18;
    TPanel *pan_ATCTempHead19;
    TPanel *pl_ATCRefHead19;
    TPanel *pan_ATCTempHead20;
    TPanel *pl_ATCRefHead20;
    TPanel *pl_ATCTempHead17;
    TPanel *pl_ATCTempHead18;
    TPanel *pl_ATCTempHead19;
    TPanel *pl_ATCTempHead20;
    TPanel *pan_ATCTempHead21;
    TPanel *pan_ATCTempHead22;
    TPanel *pan_ATCTempHead23;
    TPanel *pan_ATCTempHead24;
    TPanel *pl_ATCRefHead21;
    TPanel *pl_ATCRefHead22;
    TPanel *pl_ATCRefHead23;
    TPanel *pl_ATCRefHead24;
    TPanel *pl_ATCTempHead21;
    TPanel *pl_ATCTempHead23;
    TPanel *pl_ATCTempHead24;
    TPanel *pl_ATCTempHead22;
    TPanel *pan_ATCTempHead25;
    TPanel *pl_ATCRefHead25;
    TPanel *pan_ATCTempHead26;
    TPanel *pl_ATCRefHead26;
    TPanel *pan_ATCTempHead27;
    TPanel *pl_ATCRefHead27;
    TPanel *pan_ATCTempHead28;
    TPanel *pl_ATCRefHead28;
    TPanel *pl_ATCTempHead25;
    TPanel *pl_ATCTempHead26;
    TPanel *pl_ATCTempHead27;
    TPanel *pl_ATCTempHead28;
    TPanel *pan_ATCTempHead29;
    TPanel *pan_ATCTempHead30;
    TPanel *pan_ATCTempHead31;
    TPanel *pan_ATCTempHead32;
    TPanel *pl_ATCRefHead29;
    TPanel *pl_ATCRefHead30;
    TPanel *pl_ATCRefHead31;
    TPanel *pl_ATCRefHead32;
    TPanel *pl_ATCTempHead29;
    TPanel *pl_ATCTempHead31;
    TPanel *pl_ATCTempHead32;
    TPanel *pl_ATCTempHead30;
    TALed *aldATC7Status;
    TLabel *lblATC70;
    TTabSheet *tsPATSetUp;
    TLabeledEdit *leRunCardNumber;
    TComboBox *cbPATMode;
    TButton *btnPATInstall;
    TLabel *lbShowDevName;
        TALed *aLedAuto1;
        TALed *ALed5;
        TALed *ALed6;
        TALed *ALed7;
        TALed *aLedAuto2;
        TALed *aLedAuto3;
        TALed *ALedLoader;
    TSpeedButton *sbLotReset;
    TLabeledEdit *lnPartNumber;
    TLabeledEdit *lnTestProgram;
    TLabeledEdit *lnOpName;
    TLabeledEdit *lnEntityNo;
    TLabeledEdit *lnLoadBoard;
    TLabeledEdit *lnStartTime;
    TLabeledEdit *lnEndTime;
    TButton *btnRealTime;
    TButton *btnpatHourly;
    TButton *btnpatEndLot;
    TComboBox *cbPATSubMode;
    TLabeledEdit *lnLotNumber;
    TSpeedButton *sbUploadPAT;
    TTabSheet *tsSetupFileCheck;
    TEdit *edDeviceID;
    TLabel *Label1;
    TLabeledEdit *edtLotEventLogName;
    TComboBox *cbTestTimes;
    TLabel *lblTestTimes;
    TLabeledEdit *edtStage;
    TLabeledEdit *edtStep;
    TCheckBox *chkPositionOffset;
    TButton *btnManualI49;
    TTabSheet *tsAMR;
    TCheckBox *cbA60_1;
    TLabel *lblAMRLoaderTotal;
    TPanel *pnlWaitTXTotalLoader;
    TLabel *lblAMRCnt;
    TLabel *lblAMRSet;
    TPanel *pnlWaitTXSetLoader;
    TPanel *pnlWaitTXCntLoader;
    TPanel *pnlWaitRXCntAuto1;
    TPanel *pnlWaitRXSetAuto1;
    TPanel *pnlWaitRXSetAuto2;
    TPanel *pnlWaitRXCntAuto2;
    TPanel *pnlWaitRXCntAuto3;
    TPanel *pnlWaitRXSetAuto3;
    TPanel *pnlWaitTXLoader;
    TPanel *pnlWaitRXAuto1;
    TPanel *pnlWaitRXAuto2;
    TPanel *pnlWaitRXAuto3;
    TPanel *pnlWaitTrayFeed;
    TPanel *pnlLoaderLast;
    TALed *aldLoaderLast;
    TALed *aldWaitTrayFeed;
    TALed *aldWaitRXAuto3;
    TALed *aldWaitRXAuto2;
    TALed *aldWaitRXAuto1;
    TALed *aldWaitTXLoader;
    TPanel *Panel2;
    TStringGrid *StrGrdCategory;
    TTabSheet *tsOtherTool;
    TGroupBox *GroupBox6;
    TCheckBox *cbFirstTrayCheckOnUnloader;
    TCheckBox *cb1stCheck_Auto1;
    TCheckBox *cb1stCheck_Auto2;
    TCheckBox *cb1stCheck_Auto3;
    TGroupBox *GroupBox7;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *edLoaderCountNow;
    TEdit *edLoaderCountAlarm;
    TSpeedButton *btnFTPDownLoadbyDeviceID;
    TRadioGroup *rgUnloader;
    TLabel *labCusStep;
    TEdit *edtCusStep;
    TLabel *Label4;
    TLabel *Label10;
    TLabel *Label31;
    TLabel *Label32;
    TComboBox *cbRunModeASECL;
    TEdit *edInsertion;
    TEdit *edCustomerDevice;
    TEdit *edFlowID;
    //==> Eastsun 20260515 F009 KYEC AMR tsKYEC_AMR published decls (components)
    TTabSheet *tsKYEC_AMR;
    TALed *ledStartAGV;
    TALed *ledSTART;
    TALed *ledLoaderTotalTray;
    TALed *ledLOT_START;
    TLabel *labAMRHdrLoader;
    TLabel *labAMRHdrEmpty;
    TLabel *labAMRHdrColor;
    TLabel *labAMRHdrAuto1;
    TLabel *labAMRHdrAuto2;
    TLabel *labAMRHdrAuto3;
    TPanel *paStartAGV;
    TPanel *paSTART;
    TPanel *paLoaderTotalTray;
    TPanel *paLOT_START;
    TPanel *Panel36_KYEC;
    TLabel *lblAMRSpacer_KYEC;
    TLabel *labNowAuto1TrayID_KYEC;
    TLabel *Label67_KYEC;
    TLabel *Label65_KYEC;
    TLabel *labNowAuto2TrayID_KYEC;
    TLabel *Label70_KYEC;
    TLabel *labNowAuto3TrayID_KYEC;
    TLabel *lbl3_KYEC;
    TLabel *labAuto1TrayCount_KYEC;
    TLabel *lbl5_KYEC;
    TLabel *labAuto2TrayCount_KYEC;
    TLabel *lbl7_KYEC;
    TLabel *labAuto3TrayCount_KYEC;
    TLabel *Label52_KYEC;
    TLabel *labNowLoaderTrayID;
    TLabel *Label56_KYEC;
    TLabel *labLoaderTrayCount;
    TLabel *Label57_KYEC;
    TLabel *labLotTrayCount_KYEC;
    TLabel *Label58_KYEC;
    TLabel *labNowTrayCount;
    TLabel *Label59_KYEC;
    TLabel *LabDiffTrayCount;
    TPanel *Panel37_KYEC;
    TPanel *pnl6;
    TALed *ledLoader;
    TALed *ledEmpty;
    TALed *ledColor;
    TALed *ledAuto1;
    TALed *ledAuto2;
    TALed *ledAuto3;
    TALed *ledLoaderLock;
    TALed *ledEmptyLock;
    TALed *ledColorLock;
    TButton *btnAMRSupplement;
    TButton *btnAMRLDUnLDCheck;
    TButton *btnAMRLDUnLDFinish;
    TButton *btnAMRClearCount;
    TButton *btnAMRSetSECS;
    TEdit *edAMREventParam;
    TEdit *edAMRLoaderTrayID;
    TEdit *edAMRAuto1TrayID;
    TEdit *edAMRAuto2TrayID;
    TEdit *edAMRAuto3TrayID;
    TEdit *edAMRTrayCount;
    TEdit *edAMRDeviceCount;
    TEdit *edAMRBinSetting;
    TPanel *palAQLMode;
    TLabel *lbLotAQLCount;
    TLabel *lbLotAQLBin;
    TLabel *lbLotAQLSetCount;
    TLabel *lbLotAQLSetBin;
    TTimer *ATCTransferFileTime;
    TLabel *labBarcodeRecipe;
    TEdit *edtBarcodeRecipe;
    //<== Eastsun 20260515 F009 KYEC AMR tsKYEC_AMR published decls (components)
    TLabel *lblDownloadAccessWarning;
    TCheckBox *chkAutoCleanContactHeight;
    void __fastcall ATCTransferFileTimeTimer(TObject *Sender);  //Eastsun 20260522 add: ATC Recipe FileTransfer
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btUploadClick(TObject *Sender);
    void __fastcall btDownloadClick(TObject *Sender);
    void __fastcall edTempKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall btnFtpServerClick(TObject *Sender);
    void __fastcall btSaveSetupFileClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall Timer2Timer(TObject *Sender);
    void __fastcall edDeviceNameMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbbDeviceNameChange(TObject *Sender);
    void __fastcall pgLotinfoChange(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall sbSECSLotStartClick(TObject *Sender);
    void __fastcall btStartCountClick(TObject *Sender);
    void __fastcall sbSECSLotEndClick(TObject *Sender);
    void __fastcall labLotIDMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall spOCRSaveLogClick(TObject *Sender);
    void __fastcall spOCRChangeFileClick(TObject *Sender);
    void __fastcall Timer3Timer(TObject *Sender);
    void __fastcall Timer4Timer(TObject *Sender);                         //Eastsun 20260515 F020
    void __fastcall spOCRLogInClick(TObject *Sender);
    void __fastcall NetATCTimeTimer(TObject *Sender);
    void __fastcall coLevelModeChange(TObject *Sender);
    void __fastcall btClearBarcodeCountClick(TObject *Sender);
    void __fastcall btClearBarcodeListClick(TObject *Sender);
    void __fastcall spSECSLotCheckClick(TObject *Sender);
    void __fastcall TimerERMSTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btChangeFileClick(TObject *Sender);
    void __fastcall pl_ATC_OnlineClick(TObject *Sender);
    void __fastcall edtSysLotIDKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall sbARMSShowClick(TObject *Sender);
    void __fastcall palSecsGemMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall sb_RunExecutFileClick(TObject *Sender);
    void __fastcall edDeviceNameKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall sb_Main_EvenLevelLoginClick(TObject *Sender);
    void __fastcall edtASECL_LotIDClick(TObject *Sender);
    void __fastcall btnASECL_LotStartClick(TObject *Sender);
    void __fastcall btnASECL_LotEndClick(TObject *Sender);
    void __fastcall btnQAmodeSaveClick(TObject *Sender);
    void __fastcall edQAModeMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edPageMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall LotKeyInTimeTimer(TObject *Sender);
    void __fastcall edtSysOperatorIDKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall tmrChamberBoostTimer(TObject *Sender);
    void __fastcall btnStartChamberBoostClick(TObject *Sender);
    void __fastcall btnStopChamberBoostClick(TObject *Sender);
    void __fastcall edtSysOperatorIDMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edtSysLotIDMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnFTPTryConnectClick(TObject *Sender);
    void __fastcall edtSysLotIDKeyPress(TObject *Sender, char &Key);
    void __fastcall edPageKeyPress(TObject *Sender, char &Key);
    void __fastcall edtSysOperatorIDKeyPress(TObject *Sender, char &Key);
    void __fastcall cbRunModeKeyPress(TObject *Sender, char &Key);
    void __fastcall pnlXMLOnLineClick(TObject *Sender);
    void __fastcall btnSaveDataClick(TObject *Sender);
    void __fastcall btnCancelTestPauseClick(TObject *Sender);
    void __fastcall btnESCFunctionClick(TObject *Sender);
    void __fastcall sbTestClick(TObject *Sender);
    void __fastcall sbRecipeUploadClick(TObject *Sender);
    void __fastcall sbRecipeDownloadClick(TObject *Sender);
    void __fastcall sbFTPAutomationSaveClick(TObject *Sender);
    void __fastcall btnManualCheckListClick(TObject *Sender);
    void __fastcall edDeviceNameKeyPress(TObject *Sender, char &Key);
    void __fastcall edTempKeyPress(TObject *Sender, char &Key);
    void __fastcall edDeviceNameKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edTempKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edtSysLotIDKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edtSysOperatorIDKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall btTesterTCPShowClick(TObject *Sender);
    void __fastcall btnClearTemperatureClick(TObject *Sender);
    void __fastcall edStationNumMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnManualStandardClick(TObject *Sender);
    void __fastcall btnGetLoaderClick(TObject *Sender);
    void __fastcall RFID_ReaderReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall btnLoadFileClick(TObject *Sender);
    void __fastcall spOCRCleanListClick(TObject *Sender);
    void __fastcall btnAirStreamOnOffClick(TObject *Sender);
    void __fastcall rgOEEStateClick(TObject *Sender);
    void __fastcall edtLotVerifyMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edtLotVerifyKeyPress(TObject *Sender, char &Key);
    void __fastcall BtnPauseClick(TObject *Sender);
    void __fastcall BtnPauseMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall BtnPauseMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnPATInstallClick(TObject *Sender);
    void __fastcall cbRTCASTDClick(TObject *Sender);
    void __fastcall cbPATModeChange(TObject *Sender);
    void __fastcall sbUploadPATClick(TObject *Sender);
    void __fastcall sbLotResetClick(TObject *Sender);
    void __fastcall leRunCardNumberMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnRealTimeClick(TObject *Sender);
    void __fastcall btnpatHourlyClick(TObject *Sender);
    void __fastcall btnpatEndLotClick(TObject *Sender);
    void __fastcall btnManualI49Click(TObject *Sender);
    void __fastcall cbFirstTrayCheckOnUnloaderClick(TObject *Sender);
    void __fastcall cbFirstTrayCheckOnUnloaderMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbRunModeDropDown(TObject *Sender);
    void __fastcall cbRunModeKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall cbRunModeKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall btnFTPDownLoadbyDeviceIDClick(TObject *Sender);
    void __fastcall rgUnloaderClick(TObject *Sender);
    //==> Eastsun 20260515 F009 KYEC AMR tsKYEC_AMR published decls (events)
    void __fastcall labNowTrayCountClick(TObject *Sender);
    void __fastcall btnAMRSupplementClick(TObject *Sender);
    void __fastcall btnAMRLDUnLDCheckClick(TObject *Sender);
    void __fastcall btnAMRLDUnLDFinishClick(TObject *Sender);
    void __fastcall btnAMRClearCountClick(TObject *Sender);
    void __fastcall btnAMRSetSECSClick(TObject *Sender);
    void __fastcall edDeviceIDMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    //<== Eastsun 20260515 F009 KYEC AMR tsKYEC_AMR published decls (events)
private:    // User declarations
    void CutTempToEdit(AnsiString asString);
    bool bNeedToDeleteFile;                                                     //Steven 20110907 : RTC
    TPanel *ATCPtrWinWay[4];
    int iProduceTimeCT;                                                         //jou 20221125 : 機台添加三小時送檢報警，從lot start時間開始計算
    bool bP60UserClicked;                                                       //Jimmychiu 20251205 : First Tray Check On Unloader
public:                                                                         // User declarations
    int iUnloaderTask[3];                                                //Eastsun 20260515 F011 整合:
    void __fastcall InitialUnLoaderTask(int iPos);                       //Eastsun 20260515 F011 整合:
    void CheckActionFlag();                                                     //Eastsun 20260515 F011 moved from private (F009 wiring fix)
    //==> Eastsun 20260515 F020 (AMR function declarations)
    void InitialLoaderTask(int iPos);
    void InitialLDLevelTask(int iPos);
    void LoaderAction(int iAuto);
    void UnLoaderAction(int iAuto);
    void CheckLDLevel(int iAuto);
    void CheckAMRAction();
    void ReflashInfo();
    void bClearAMRPar();                                                        //Eastsun 20260710整合
    //<== Eastsun 20260515 F020
    unsigned int GET_CRC_16(unsigned char *string, unsigned char length)  ;
    __fastcall TfLotInfo(TComponent* Owner);
    bool __fastcall DownloadFromServer(AnsiString sDLFileName, bool bFromFTP=true);     //Ifor 20181107 :add 資料來源
    bool __fastcall DownloadFromERMS(AnsiString sDLFileName);                           //Steven 20160711 : 使用進階版RMS
    bool __fastcall DownloadFromServer_TSMC(AnsiString sOrgFileName, AnsiString sDLFileName); //ChungHung 20150413 add for TSMC
    void __fastcall UploadToServer();
    void __fastcall UploadToServer(AnsiString sULFileName, bool bSuccessAlarm);
    int iWaitRtcDeleteTask;                                                     //Steven 20110907 : RTC
    int iRTC_AutoSTDTask;

    bool bShow;
    TComboBox *Ptr[4];
    void __fastcall LoadRTCFullViewImg(bool bShowImage);
    bool bRTCChangeFileFinish;                                                          //ChungHung 20140514 fix in homeing and contact make time out
    void __fastcall RTCChangeFile(bool bNeedDelete=true);
    void __fastcall ShowATCThermo();                                                    //Steven 20120410 : Hontech ATC
    void __fastcall _DelTree(String dir, String sExcpet="");                            //ChungHung 20150413 add for TSMC
    void ConnectNetDeviceS();                                                           //Steven 20140306 : 使用網路硬碟
    bool bStartCount_SCK;                                                               //ChungHung 20140714 only for SCK add bStartCount_SCK
    void Save_BarCodeLog();                                                             //wei 20150720 BarCode log
    bool __fastcall DoPassword();                                                       //JerryYang 20151028 add for Amkor
    void SaveLotOperatorID(AnsiString Lot, AnsiString Operator, bool bRead=false);      //wei 20151230 art 紀錄資料
    void SetATCFormVisible();                                                   //Steven 20160217 : For ATC7.0
    TPanel *ESD_DataPtr[18];
    TPanel *ESD_DECAY_DATA_Ptr[36];
    void OCRConnectTest();                                                      //wei 20161028 ocr連線測試
//    void SetATCOffset(bool bFirstSetOffset);
    void SetATCOffset(bool bFirstSetOffset, bool bOFSClose=false);

    void SetATCFFCOffset();                                                     //Ifor 20190730 : add FFC Temperature Offset
    int iLotRead ;
    int iLotStart;
    int iLotEnd  ;
    void __fastcall ReadWriteLotInfo(bool bRead);                               //Steven 20250515 : 整合Lot測試報表
    void __fastcall SetLotID(AnsiString ID, bool bReadFromFile=false);
    void __fastcall SetLotStart(AnsiString sFunc, bool bReadFromFile=false);    //Steven 20250515 : 整合Lot測試報表
    void __fastcall SetLotEnd(AnsiString sFunc);                                //Steven 20250515 : 整合Lot測試報表
    void __fastcall SetLotComponents(bool bLotEnd);                             //Steven 20250515 : 整合Lot測試報表

    bool __fastcall CheckEventLogParameter();                                   //Steven 20181224 : For ASE-CL
    void __fastcall SaveBackEventLogInfo(AnsiString aBackAlarm);
    void __fastcall SaveBackEventTracker(AnsiString aBackAlarm);
    void __fastcall UploadEventLogFile(AnsiString aFileName);
    void __fastcall SaveASECLTestLogInfo();
    void __fastcall SaveASECLNewTestLogInfo(int iR, int iC, int iBinData);
    void __fastcall SaveOEELog(bool bforce=false, bool bClear=false);
    void __fastcall SaveASECLTesterHardwareLog();
    AnsiString aBackTestSummaryFile;
    AnsiString aBackTestLogFile;
    bool bEventLogAlarm;
    TEdit *edSocket[4][8];
    TPanel *SocketSiteCH_Display[MAX_SOCKET_ROW][MAX_SOCKET_COL];               //JerryYang 20180626 (wei) : MicroChip要求initial要給OP確認site mapping
    TPanel *ATCChPal[ATC_HEAD_COUNT];
    TPanel *ATCReferPtr[ATC_HEAD_COUNT];
    TPanel *ATCPtr[ATC_HEAD_COUNT];
    TStringList *slASECLTestInfor;
    void __fastcall ShowSocketID();

    double fTempUserOffset[tcTotalCount];                                       //ChungHung 20140804 add 修正 RMS 原本 只還原到前十個 uer Offset   //Ifor 20191003 : add Die Force 可以自定義Kit直徑
    double fContactHeight[25];
    double fTempATCOffset[tcTotalCount];                                        //Steven 20221209 : ATC offset要不要覆蓋
    int iShuttleMode[2];                                                        //JerryYang 20170411 (Steven) 15->24
    AnsiString iAutoClean[69];                                                  //Steven 20240911 : 24 --> 63, 改用AnsiString
    bool bART[3];
    int iART;
    int iIndexHeatingMode;
    AnsiString strCleanCnt[20][10];                                             //JerryYang 20190703 auto clean清潔次數備份
    AnsiString sJSCCOSFileName;                                                 //RogerYang 20260127 : Add For JSCC_OS download by Device list

    int DoBackupSetupFile(AnsiString DataPath, AnsiString sDLFileName1);        //Steven 20191101 : 整合下載工作檔的覆蓋方式
    int DoOverWriteSetupFile(AnsiString DataPath, AnsiString sDLFileName);
    bool bStartChamberBoost;                                                    //Steven 20191128 : Chamber Boost Function
    void __fastcall VTestSummaryReport();
    void SettsChipAdvVisible();
    void SetSelectionVisible();                                                 //Steven 20250519 : 統一Selection的顯示設定
    void ClearAllSetupFile(AnsiString sSetupFile, AnsiString sSetupFile_Net="");    //Steven 20200512 : 刪除全部工作檔, 只留下當下的
    int  iXMLOnLineStatus;
    void ShowXMLOnLine();
    void __fastcall WhenTestRecordTemperatureLog_3Sigma(AnsiString aFileName, int iUseIndex); //kevin 20200521 add 3 SIGMA //Hmy 20200515 Copy//Hmy 20200510 Add Enable 3 Sigma Temp Monitor//Hmy 20200510 Add Enable 3 Sigma Temp Monitor
    void SetCheckCodeByLot(bool _enable);
    void ReadWriteFTPAutomationData(bool bRead);                                //KaiChen 20190530 ：Sigurd FTP Automation
    AnsiString GenerateCheckList(bool bCheckFlag=false, AnsiString asCheckListName="", bool bBack=false, bool bInitialStart=false);   //KaiChen 20191003 ：修改回傳 AnsiString   //KaiChen 20190530 ：Sigurd FTP Automation
    void AlarmCodeUpload(AnsiString JamCode="");                                //KaiChen 20190530 ：Sigurd FTP Automation
    AnsiString CheckingCheckList(bool bInitialStart=false);                     //KaiChen 20190530 ：Sigurd FTP Automation
    void ShowInformation(bool bShow=false);                                     //KaiChen 20191111 ：權限卡關顯示
    void RefreshYieldMonitor();                                                 //Sam 20210331 : 北興新增 Yield Monitor
    void RefreshYieldMonitor_SIGURD();
    void RefreshYieldMonitor_TERAPOWER();                                       //Sam 20210916 : 新增 Yiled Monitor 到畫面上
    void WriteFTPSetupFileChangeLog(AnsiString msg);                            //Sam 20210803 : FTP SetFile Change Log
    void AdjtsYieldMonitiorSize();
    void ASECL_LotEnd(bool bManual=false);
    void SaveGroundESDByLot(AnsiString sLot,AnsiString sCoustomerLot, AnsiString sStation, AnsiString sStationNum); //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
    void AutoTempOfsByFTP();                                                    //Sam 20220406 : 溫度自動補償功能 By FTP
    void DoReadRFID(int Pos);
    void InitRFIDRS232();                                                       //Steven 20220713 : RFID Reader for SJSEMI
    //void VerifyMode(bool bOnOff);                                             //Sam 20231117 : 整合到 QA 模式 Mark  //Sam 20221012 : 新增 VerifyMode 功能
    bool bCheckOnlyOneFile(AnsiString asPath, AnsiString &asFileName);          //KenHsieh 20230727 : 更改工作檔與資料 By NetFile
    bool bCheckOnlyOneFileAndData();                                            //KenHsieh 20230727 : 更改工作檔與資料 By NetFile
    void ProductTesterReport();                                                 //Sam 20231115 : PTI 新增 Tester report
    void SetTesterStartTimeByB03();                                             //Sam 20240809 : PTI ART 模式
    void ResetLotInfo();                                                        //Steven 20240925 : 重開軟體時, 要讀回lot info
    void ShowATC70Thermo(double SetATCTemp, double iTempRange);
    void ShowATC20Thermo(double SetATCTemp, double iTempRange);
    void ShowNewATCThermo(double SetATCTemp, double iTempRange, double &OldSetATCTemp);
    void ShowATCTempPanel();                                                    //Steven 20241112 : 調整ATC溫度顯示
    bool CheckNoRetestBinFlag();                                                //RogerYang 20250604 偉測不可複測bin功能
    AnsiString sVTestInternalLot;                                               //RogerYang 20250809 偉測Summary文件修改
    void __fastcall ReadAirMachineStatus();                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function ==>
    void __fastcall CheckAirMachineStatus();
    bool bManualAirCoolingOnOff;   //add Manual Air Cooling On/Off
    void SetCloseSiteTemp(double dTemp);
    TPanel *TripnlRefrigerantMachine[8];
    TLabel *TriLabRefrigerantValue[8];
    TPanel *TripnlRefCopm1Status[8];
    TPanel *TripnlRefCopm2Status[8];
    TLabel *TriLabRefCopm1HpValue[8];
    TLabel *TriLabRefCopm2HpValue[8];
    TLabel *TriLabRefCopm1LpValue[8];
    TLabel *TriLabRefCopm2LpValue[8];
    TLabel *TriLabRefrigerantAdjustValue[8];
    bool bInitFormcomponent;
    bool OldRefrigerantCommand;
    void InitialRefrigerantSystem();
    void ScanRefrigerantSystem();
    void ATC_OFFLINE_FormComInit();
    void RefreshRefrigerantAllStatus();                                         //Ztex 2023.04.19 Add HT-1032 TriTemp Function <==

    double TransformTemperature_AirStream(double Offset, int iIndex);           //Ztex 2023.12.19 Add Air Stream Offset
    void SaveGroundESDData_Upolad();                                            //Sam 20250609 : Record Ground and ESD at intervals and upload
    bool RMSDownloadByNetwork(AnsiString sSourcesFilePath,AnsiString sTargetFilePath,AnsiString sSetupFileName);                       //Jimmychiu 20250707 : add RMS connect method
    bool RMSDownloadByFTP(AnsiString sSourcesFilePath,AnsiString sTargetFilePath,AnsiString sSetupFileName);                           //Jimmychiu 20250707 : add RMS connect method
    bool RMSUploadByNetwork(AnsiString sSourcesFilePath,AnsiString sTargetFilePath,AnsiString sSetupFileName);                         //Jimmychiu 20250707 : add RMS connect method
    bool RMSUploadByFTP(AnsiString sSourcesFilePath,AnsiString sTargetFilePath,AnsiString sSetupFileName);                             //Jimmychiu 20250707 : add RMS connect method
    void GetFTP_SettingN06(AnsiString &asUserID, AnsiString &asPassword, AnsiString &asHost);
    void DoUpload(AnsiString sSetupFileName,AnsiString sCustomName);
    void ClearLotInfoPAT();                                                     //Jimmychiu 20241110 : add PAT Class
    void UpdateLotInfoPAT();                                                    //Jimmychiu 20241110 : add PAT Class
    void VisibleUploadBtnPAT(bool bVisible);
    void UpdatePATSubMode(const AnsiString& sModeName);
    bool N23UseLotInfoFile();
    void RefreshAMR();                                                          //Sam 20240304 : 新增 AMR 功能
    void ShowAMRCategoryBin(bool bInitial=false);                               //Sam 20240304 : 新增 AMR 功能
    void JCETWhite2DIDShow(bool bUse);                                          //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能
    void SetFirstTrayCheckOnUnloader();                                         //Jimmychiu 20251205 : First Tray Check On Unloader
    void RefreshOtherTool();                                                    //Jimmychiu 20251219 : Refresh PTI funciton
    bool __fastcall SetAQLMode(bool bSet=false);                                //Eastsun 20260520 整合
};
//---------------------------------------------------------------------------
extern PACKAGE TfLotInfo *fLotInfo;
//---------------------------------------------------------------------------
#endif
