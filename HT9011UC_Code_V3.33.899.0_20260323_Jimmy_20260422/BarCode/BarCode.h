//---------------------------------------------------------------------------

#ifndef BarCodeH
#define BarCodeH
//---------------------------------------------------------------------------
#include <math.h>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SPComm.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//#include "HTimer.h"
#include <ComCtrls.hpp>
#include "HTray.h"
#include "myTimer.h"
#include "uSocketServerClient.h"
//------------------------------------------------------------------------------
#include "mytray.h"
#include "MyKitSuck.h"
#include <Grids.hpp>
#include <ScktComp.hpp>


#include <map>
//using namespace std;
//---------------------------------------------------------------------------
typedef void (__closure *TPointVoidShowMsg)(AnsiString);
//---------------------------------------------------------------------------
#define BAR_CODE_COUNT 8        //KaiChen 20200513 : 4-->8
class TfHIKROBOTcmd                                                             //Jimmychiu 20250818 : 海康智能讀碼
{
private:    // User declarations
public:     // User declarations
    TfHIKROBOTcmd(){}
    ~TfHIKROBOTcmd(){}
    AnsiString GetStartCmd();
    AnsiString GetAnalysisResult(AnsiString sreceive);
};
//------------------------------------------------------------------------------
class uCCDUnloaderClip
{
private:
    uSocketClient *socketClient;
    bool bflag;
    void SendCommandSocket(AnsiString asCommand);
    void ReceiveData(char* cGet,int iLen);
    bool bSimulationCommand;
    int iCommandTask;
public: // User declarations
    uCCDUnloaderClip();
    ~uCCDUnloaderClip();
    void InitialDatas()
    {
        asReceiveMsg="";
        bflag=false;
        #ifdef SOFT_SIMULTE
        bSimulationCommand=true;
        #else
        bSimulationCommand=false;
        #endif
        sIP="";
        sPort="";
        iCommandTask=1;
        iRetryConn=0;
        bGetResult=false;
    }
    int iRetryConn;
    bool bGetResult;
    //socket
    AnsiString asReceiveMsg;
    void SendCommandPhoto();
    bool SetCommParameter(AnsiString asAddress,AnsiString asPort);
    bool DoCommuncation();
    void DoSocketClose();
    void SendCommandStr(AnsiString sMsg);
    AnsiString sIP;
    AnsiString sPort;
    TPointVoidShowMsg RecMsgCCDUnloaderClip;
    AnsiString DoGetPhotoCmd(bool bReset,AnsiString &sErr);
};
//------------------------------------------------------------------------------
enum eMulti2DType
{
    e1x2In1CCD=0,
    e2x1In1CCD=1,
    e2x1In2CCD=2,
    e2x2In1CCD=3,
    e2x2In2CCD=4,
    eMulti2DTypeTotal
};
enum eCCD_ReadUnloaderClip
{
    eCCDClipAuto1=0,
    eCCDClipAuto2,
    eCCDClipAuto3,
    eCCDClipFix1,
    eCCDClipFix2,
    eCCDClipFix3,
    eCCDClipTotal
};

typedef struct
{
    AnsiString sMulti2D[4];
    AnsiString sSeparator;
    void       Set2D(int i, int j, AnsiString Str);
    void       SetToErrCode();
    AnsiString Get2DText();
    AnsiString Get2DCommaText();
    AnsiString Get2DTextByMap(int i);
    void Clear2D();
}_MyMulti2DStruct;

class TfBarCode : public TForm
{
__published:    // IDE-managed Components
    TComm *Barcode_1;
    TComm *Barcode_2;
    TComm *Barcode_3;
    TComm *Barcode_4;
    TPanel *Panel55;
    TPanel *Panel1;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TTimer *tmr1;
    TClientSocket *ClientSocket_Shuttle1_A;
    TClientSocket *ClientSocket_Shuttle1_B;
    TClientSocket *ClientSocket_Shuttle2_A;
    TClientSocket *ClientSocket_Shuttle2_B;
    TTimer *TimerDownCCDConnect;
    TTimer *TimerCCDInitial;
    TTimer *TimerProcess2DData;
    TClientSocket *ClientSocket_BarcodeChangeFile;
    TTimer *TimerBarcodeChangeFile;
    TPageControl *pgc2DID;
    TTabSheet *ts2DID;
    TPageControl *pc2DID;
    TTabSheet *tsResult;
    TPanel *palOutSht;
    TTMyTray *myOutShuttleLabel;
    TTMyTray *mtBarcodeOutSh;
    TPanel *palInSht;
    TTMyTray *myInShuttleLabel;
    TTMyTray *mtBarcodeInSh;
    TTabSheet *tsLog;
    TListBox *lb_2DBarcodeData;
    TPanel *btnClear;
    TTabSheet *tsBarCodeSetting;
    TCheckBox *cbEnableBarCode;
    TTabSheet *tsCCD;
    TTabSheet *tsCognexSetting;
    TTabSheet *tsFloating;
    TSpeedButton *spbResetCom;
    TSpeedButton *spbStartCom;
    TMemo *Memo1;
    TMemo *Memo2;
    TGroupBox *gb1;
    TCheckBox *cbCommReader1;
    TCheckBox *cbCommReader2;
    TComboBox *coCommReader1;
    TComboBox *coCommReader2;
    TButton *btReader2On;
    TButton *btReader1On;
    TGroupBox *gb2;
    TCheckBox *cbCommReader3;
    TCheckBox *cbCommReader4;
    TComboBox *coCommReader3;
    TComboBox *coCommReader4;
    TButton *btReader3On;
    TButton *btReader4On;
    TButton *btnGPIBReader4;
    TMemo *moComparison;
    TGroupBox *gbChangFile;
    TLabel *Label14;
    TLabel *Label15;
    TButton *btBarcodeChangeFileDisConnect;
    TButton *btBarcodeChangeFileSend;
    TEdit *edtCognexSendCmd;
    TButton *btBarcodeChangeFileConnect;
    TEdit *IPAddress1;
    TEdit *IPPort1;
    TMemo *moBarcodeChangeFile;
    TMemo *memoCCDCommLog;
    TPageControl *pcShtFloat;
    TTabSheet *tsFloatResult;
    TTabSheet *tsFloatSetting;
    TCheckBox *cbEnableShtFloatChk;
    TLabel *labSFCGetResultTimeOut;
    TLabel *labSFCStartDelay;
    TLabel *labSFCAutoRetry;
    TLabel *labSFCExposureTimeOut;
    TEdit *edSFCGetResultTimeOut;
    TEdit *edSFCStartDelay;
    TEdit *edSFCAutoRetry;
    TEdit *edSFCExposureTimeOut;
    TCheckBox *cbSFCUse2Photo;
    TEdit *edSFCUse2PhotoOffset;
    TTMyTray *mtSFC_Label1;
    TTMyTray *mtSFC_Shuttle;
    TPanel *palCheck2DID;
    TSpeedButton *btStart2DIDCheckSh1;
    TSpeedButton *bt2DIDOffset;
    TSpeedButton *btStart2DIDCheckSh2;
    TCheckBox *chkBottom2DID;
    TTabSheet *tsBarcodeMapping;
    TLabel *Label13;
    TPageControl *pgc2DMap;
    TTabSheet *tsBarcodeAuto;
    TLabel *labBinMapping_Auto1;
    TLabel *labBinMapping_Auto2;
    TLabel *labBinMapping_Auto3;
    TComboBox *cobMain_BinMapping_Auto1;
    TTMyTray *trShowBinMappingAuto1;
    TTMyTray *trShowBinMappingAuto2;
    TTMyTray *trShowBinMappingAuto3;
    TComboBox *cobMain_BinMapping_Auto2;
    TComboBox *cobMain_BinMapping_Auto3;
    TTabSheet *tsBarcodeFix;
    TLabel *labBinMapping_Fix1;
    TLabel *labBinMapping_Fix2;
    TLabel *labBinMapping_Fix3;
    TComboBox *cobMain_BinMapping_Fix1;
    TTMyTray *trShowBinMappingFix1;
    TTMyTray *trShowBinMappingFix2;
    TTMyTray *trShowBinMappingFix3;
    TComboBox *cobMain_BinMapping_Fix2;
    TComboBox *cobMain_BinMapping_Fix3;
    TTabSheet *tsXML;
    TMemo *mmoXML;
    TPanel *Panel3;
    TStringGrid *strngrdXMLResult2;
    TStringGrid *strngrdXMLResult1;
    TLabel *lblURL;
    TCheckBox *chkMulti2DID;
    TTabSheet *tsMulti2DID;
    TLabeledEdit *edtXPitch;
    TRadioGroup *rgMulti2DType;
    TGroupBox *grpMulti2DMap;
    TComboBox *cbAa;
    TComboBox *cbBa;
    TComboBox *cbBb;
    TComboBox *cbAb;
    TTabSheet *tsOCR;
    TLabel *Label645;
    TComboBox *cbReadMultiLine_2DBarcode;
    TLabel *lab1stLineLength_2DBarCode;
    TEdit *edt1stLineLength_2DBarCode;
    TLabel *lab2ndLineLength_2DBarCode;
    TEdit *edt2ndLineLength_2DBarCode;
    TLabel *labInsertString1stAnd2nd_2DBarCode;
    TEdit *edtInsertString1stAnd2nd_2DBarCode;
    TPageControl *pgcFunction;
    TTabSheet *TabSheet2;
    TLabel *Label395;
    TLabel *Label1;
    TLabel *labCodeMinLen;
    TLabel *labRetryCount;
    TLabel *labTriggerTime;
    TLabel *Label9;
    TLabel *labCodeMaxLen;
    TLabel *lblCheckSum;
    TEdit *edBarcodeScanDelayTime;
    TEdit *edBarcodePosDelayTime;
    TEdit *edtBarcodeMinLength;
    TEdit *edBarcodeRetryCount;
    TEdit *edTriggerTime;
    TEdit *edFirstDelay;
    TEdit *edtBarcodeMaxLength;
    TRadioGroup *rgStringFormat;
    TEdit *edtCheckSumLength;
    TCheckBox *cb2DUseAnyChar;
    TCheckBox *cbNoCodeDeviceAutoSkip;
    TCheckBox *cbTriggerMode;
    TCheckBox *cbExposureTimeOut;
    TCheckBox *cbCheckCodeByShuttle;
    TCheckBox *cbCheckCodeByLot;
    TCheckBox *cbCheckLotHaveCode;
    TEdit *edShtDuplicateRetryCnt;
    TEdit *edConsecutiveFailure;
    TCheckBox *cbConsecutiveFailure;
    TCheckBox *cbRetryOffsetMove;
    TEdit *edRetryOffsetMove;
    TCheckBox *cbRetryShiftOffsetMove;
    TEdit *edRetryShiftOffsetMove;
    TCheckBox *cbSearch2DIDByLot;
    TCheckBox *cbSaveFailImage;
    TCheckBox *chk2DIDYield;
    TEdit *edt2DIDYield;
    TCheckBox *chkCloseSite2DIDIsEmpty;
    TComboBox *cbb2DIDListErrorBin;
    TRadioGroup *rgNoCodeDeviceToErr;
    TTabSheet *TabSheet1;
    TCheckBox *cbUseHandShakeCommunication;
    TEdit *edHandShakeTimeOut;
    TCheckBox *cbCheckSum;
    TLabel *Label2D;
    TCheckBox *cb2DUseSubJob;
    TEdit *edt2DFileName;
    TClientSocket *ClientSocket_Bottom_1;
    TClientSocket *ClientSocket_Bottom_2;
    TClientSocket *ClientSocket_Bottom_3;
    TClientSocket *ClientSocket_Bottom_4;
    TClientSocket *ClientSocket_Bottom_8;
    TClientSocket *ClientSocket_Bottom_7;
    TClientSocket *ClientSocket_Bottom_6;
    TClientSocket *ClientSocket_Bottom_5;
    TTimer *TimerBotton8CCDConnect;
    TTimer *TimerBottom8CCDInitial;
    TGroupBox *gbCheckCodeByServer2DID;
    TLabel *lbfinalpath;
    TLabel *lbfinalpathShow;
    TCheckBox *cbCheckCodeByServer2DID;
    TLabeledEdit *le2DID_ServerURL;
    TCheckBox *chkSearch2DIDToError;
    TTabSheet *tsBarcodeAuto4;
    TTabSheet *tsBarcodeFix4;
    TLabel *labBinMapping_Auto4;
    TLabel *labBinMapping_Auto5;
    TLabel *labBinMapping_Auto6;
    TComboBox *cobMain_BinMapping_Auto4;
    TTMyTray *trShowBinMappingAuto4;
    TTMyTray *trShowBinMappingAuto5;
    TTMyTray *trShowBinMappingAuto6;
    TComboBox *cobMain_BinMapping_Auto5;
    TComboBox *cobMain_BinMapping_Auto6;
    TLabel *labBinMapping_Fix4;
    TLabel *labBinMapping_Fix5;
    TLabel *labBinMapping_Fix6;
    TComboBox *cobMain_BinMapping_Fix4;
    TTMyTray *trShowBinMappingFix4;
    TTMyTray *trShowBinMappingFix5;
    TTMyTray *trShowBinMappingFix6;
    TComboBox *cobMain_BinMapping_Fix5;
    TComboBox *cobMain_BinMapping_Fix6;
    TTabSheet *tsBarcodeBinBox;
    TLabel *labBinMapping_BinBox;
    TComboBox *cobMain_BinMapping_BinBox;
    TTMyTray *trShowBinMappingBinBox;
    TTabSheet *tsMagazine1;
    TTabSheet *tsMagazine2;
    TLabel *labBinMapping_Mag01;
    TLabel *labBinMapping_Mag02;
    TLabel *labBinMapping_Mag03;
    TComboBox *cobMain_BinMapping_Mag01;
    TTMyTray *trShowBinMappingMag01;
    TTMyTray *trShowBinMappingMag02;
    TTMyTray *trShowBinMappingMag03;
    TComboBox *cobMain_BinMapping_Mag02;
    TComboBox *cobMain_BinMapping_Mag03;
    TLabel *labBinMapping_Mag04;
    TTMyTray *trShowBinMappingMag04;
    TComboBox *cobMain_BinMapping_Mag04;
    TLabel *labBinMapping_Mag05;
    TLabel *labBinMapping_Mag06;
    TLabel *labBinMapping_Mag07;
    TComboBox *cobMain_BinMapping_Mag05;
    TTMyTray *trShowBinMappingMag05;
    TTMyTray *trShowBinMappingMag06;
    TTMyTray *trShowBinMappingMag07;
    TComboBox *cobMain_BinMapping_Mag06;
    TComboBox *cobMain_BinMapping_Mag07;
    TLabel *labBinMapping_Mag08;
    TTMyTray *trShowBinMappingMag08;
    TComboBox *cobMain_BinMapping_Mag08;
    TLabel *labBinMapping_Mag09;
    TLabel *labBinMapping_Mag10;
    TLabel *labBinMapping_Mag11;
    TComboBox *cobMain_BinMapping_Mag09;
    TTMyTray *trShowBinMappingMag09;
    TTMyTray *trShowBinMappingMag10;
    TTMyTray *trShowBinMappingMag11;
    TComboBox *cobMain_BinMapping_Mag10;
    TComboBox *cobMain_BinMapping_Mag11;
    TLabel *labBinMapping_Mag12;
    TTMyTray *trShowBinMappingMag12;
    TComboBox *cobMain_BinMapping_Mag12;
    TLabel *labBinMapping_Mag13;
    TLabel *labBinMapping_Mag14;
    TComboBox *cobMain_BinMapping_Mag13;
    TTMyTray *trShowBinMappingMag13;
    TTMyTray *trShowBinMappingMag14;
    TComboBox *cobMain_BinMapping_Mag14;
    TGroupBox *grpLotVerify;
    TCheckBox *chkEnableLotVerify;
    TPanel *Panel7;
    TGroupBox *grpSubString;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label21;
    TEdit *edtLotIDStart;
    TEdit *edtLotIDEnd;
    TEdit *edt2DStart;
    TEdit *edt2DEnd;
    TEdit *edtLotIDVerify;
    TEdit *edtLotIDSubstr;
    TEdit *edtSampleString;
    TEdit *edtSampleSubstr;
    TButton *btnGetLotID;
    TButton *btnTestLotID;
    TEdit *ed_2D_YieldIgnoreCnt;
    TCheckBox *cb2DIDAllowList;
    TPageControl *pgcCCDSetting;
    TTabSheet *ts4CCD;
    TGroupBox *gbATC;
    TLabel *labAddress;
    TLabel *labPort;
    TEdit *edShuttle_1A_Address;
    TEdit *edShuttle_1A_Port;
    TBitBtn *BtShuttle_1A_Connect;
    TBitBtn *BtShuttle_1A_Disconnect;
    TButton *btShuttle_1A_Trigger;
    TEdit *edShuttle_1A_CMD;
    TGroupBox *GroupBox1;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *edShuttle_1B_Address;
    TEdit *edShuttle_1B_Port;
    TBitBtn *BtShuttle_1B_Connect;
    TBitBtn *BtShuttle_1B_Disconnect;
    TButton *btShuttle_1B_Trigger;
    TEdit *edShuttle_1B_CMD;
    TGroupBox *GroupBox2;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *edShuttle_2A_Address;
    TEdit *edShuttle_2A_Port;
    TBitBtn *BtShuttle_2A_Connect;
    TBitBtn *BtShuttle_2A_Disconnect;
    TButton *btShuttle_2A_Trigger;
    TEdit *edShuttle_2A_CMD;
    TGroupBox *GroupBox3;
    TLabel *Label7;
    TLabel *Label8;
    TEdit *edShuttle_2B_Address;
    TEdit *edShuttle_2B_Port;
    TBitBtn *BtShuttle_2B_Connect;
    TBitBtn *BtShuttle_2B_Disconnect;
    TButton *btShuttle_2B_Trigger;
    TEdit *edShuttle_2B_CMD;
    TTabSheet *ts8CCD;
    TPanel *Panel8;
    TGroupBox *GroupBox8;
    TLabel *Label24;
    TLabel *Label25;
    TEdit *edBottom_8_Address;
    TEdit *Edit13;
    TBitBtn *BtBottom_8_Connect;
    TBitBtn *BtBottom_8_Disconnect;
    TButton *btBottom8_Trigger;
    TEdit *edBottom_8_CMD;
    TGroupBox *GroupBox9;
    TLabel *Label26;
    TLabel *Label27;
    TEdit *edBottom_7_Address;
    TEdit *Edit14;
    TBitBtn *BtBottom_7_Connect;
    TBitBtn *BtBottom_7_Disconnect;
    TButton *btBottom7_Trigger;
    TEdit *edBottom_7_CMD;
    TGroupBox *GroupBox10;
    TLabel *Label28;
    TLabel *Label29;
    TEdit *edBottom_6_Address;
    TEdit *Edit15;
    TBitBtn *BtBottom_6_Connect;
    TBitBtn *BtBottom_6_Disconnect;
    TButton *btBottom6_Trigger;
    TEdit *edBottom_6_CMD;
    TGroupBox *GroupBox11;
    TLabel *Label30;
    TLabel *Label31;
    TEdit *edBottom_5_Address;
    TEdit *Edit16;
    TBitBtn *BtBottom_5_Connect;
    TBitBtn *BtBottom_5_Disconnect;
    TButton *btBottom5_Trigger;
    TEdit *edBottom_5_CMD;
    TGroupBox *GroupBox12;
    TLabel *Label32;
    TLabel *Label33;
    TEdit *edBottom_4_Address;
    TEdit *Edit17;
    TBitBtn *BtBottom_4_Connect;
    TBitBtn *BtBottom_4_Disconnect;
    TButton *btBottom4_Trigger;
    TEdit *edBottom_4_CMD;
    TGroupBox *GroupBox13;
    TLabel *Label34;
    TLabel *Label35;
    TEdit *edBottom_3_Address;
    TEdit *Edit18;
    TBitBtn *BtBottom_3_Connect;
    TBitBtn *BtBottom_3_Disconnect;
    TButton *btBottom3_Trigger;
    TEdit *edBottom_3_CMD;
    TGroupBox *GroupBox14;
    TLabel *Label36;
    TLabel *Label37;
    TEdit *edBottom_2_Address;
    TEdit *Edit21;
    TBitBtn *BtBottom_2_Connect;
    TBitBtn *BtBottom_2_Disconnect;
    TButton *btBottom2_Trigger;
    TEdit *edBottom_2_CMD;
    TGroupBox *GroupBox15;
    TLabel *Label38;
    TLabel *Label39;
    TEdit *edBottom_1_Address;
    TEdit *Edit24;
    TBitBtn *BtBottom_1_Connect;
    TBitBtn *BtBottom_1_Disconnect;
    TEdit *edBottom_1_CMD;
    TButton *btBottom1_Trigger;
    TPanel *pnlFunc2Left;
    TGroupBox *grpBottom2D;
    TLabel *Label11;
    TLabel *Label12;
    TEdit *edtOffsetX;
    TEdit *edtOffsetY;
    TGroupBox *gb2DTriggerCmd;
    TLabel *Label2;
    TLabel *Label10;
    TEdit *edtBarcodeONCMD;
    TEdit *edtBarcodeOFFCMD;
    TCheckBox *cb2DUseUndefinedCMD;
    TGroupBox *gbManualTest;
    TCheckBox *cb_2DBarcodeTestMode;
    TBitBtn *btn_2DBarcodeDataSend;
    TRadioGroup *rg_2DBarcodeWhichNum;
    TEdit *ed_2DBarcode;
    TGroupBox *gbSimu;
    TLabel *Label255;
    TEdit *ed_BarCodeNo;
    TCheckBox *cb_ShowNoteAlarm;
    TCheckBox *cbBarCodeSimulate;
    TButton *btResetBarCodeCount;
    TCheckBox *cbDelay;
    TBitBtn *btSaveImage;
    TBitBtn *BitBtn1;
    TCheckBox *cb2DIDAllfailSimulate;
    TGroupBox *gbSelect2DIDUse;
    TPanel *Panel6;
    TRadioGroup *rgSelectSh1;
    TRadioGroup *rgSelectSh2;
    TGroupBox *grp2DIDSorting;
    TCheckBox *cbSortingBy2DIDList;
    TRadioGroup *rgActionOf2DSort;
    TGroupBox *grpAllSiteFail;
    TRadioGroup *rgAllSiteFail;
    TTabSheet *tsCCD_Unloader;
    TGroupBox *GroupBox17;
    TLabel *Label48;
    TLabel *Label49;
    TEdit *edClip_Address_Fix3;
    TEdit *edClip_Port_Fix3;
    TBitBtn *btnClip_Connect_Fix3;
    TBitBtn *btnClip_Disconnect_Fix3;
    TButton *btnClip_SendCmd_Fix3;
    TEdit *edClip_CMD_Fix3;
    TGroupBox *GroupBox16;
    TLabel *Label46;
    TLabel *Label47;
    TEdit *edClip_Address_Fix2;
    TEdit *edClip_Port_Fix2;
    TBitBtn *btnClip_Connect_Fix2;
    TBitBtn *btnClip_Disconnect_Fix2;
    TButton *btnClip_SendCmd_Fix2;
    TEdit *edClip_CMD_Fix2;
    TGroupBox *GroupBox7;
    TLabel *Label44;
    TLabel *Label45;
    TEdit *edClip_Address_Fix1;
    TEdit *edClip_Port_Fix1;
    TBitBtn *btnClip_Connect_Fix1;
    TBitBtn *btnClip_Disconnect_Fix1;
    TButton *btnClip_SendCmd_Fix1;
    TEdit *edClip_CMD_Fix1;
    TGroupBox *GroupBox6;
    TLabel *Label42;
    TLabel *Label43;
    TEdit *edClip_Address_Auto3;
    TEdit *edClip_Port_Auto3;
    TBitBtn *btnClip_Connect_Auto3;
    TBitBtn *btnClip_Disconnect_Auto3;
    TButton *btnClip_SendCmd_Auto3;
    TEdit *edClip_CMD_Auto3;
    TGroupBox *GroupBox5;
    TLabel *Label40;
    TLabel *Label41;
    TEdit *edClip_Address_Auto2;
    TEdit *edClip_Port_Auto2;
    TBitBtn *btnClip_Connect_Auto2;
    TBitBtn *btnClip_Disconnect_Auto2;
    TButton *btnClip_SendCmd_Auto2;
    TEdit *edClip_CMD_Auto2;
    TGroupBox *GroupBox4;
    TLabel *Label22;
    TLabel *Label23;
    TEdit *edClip_Address_Auto1;
    TEdit *edClip_Port_Auto1;
    TBitBtn *btnClip_Connect_Auto1;
    TBitBtn *btnClip_Disconnect_Auto1;
    TEdit *edClip_CMD_Auto1;
    TButton *btnClip_SendCmd_Auto1;
    TButton *btnInitialGetBarcode;
    TButton *btnGetBarcode;
    TButton *Button1;
    TButton *Button2;
    TButton *Button3;
    TButton *Button4;
    TButton *Button5;
    TButton *Button6;
    TButton *Button7;
    TButton *Button8;
    TButton *Button9;
    TButton *Button10;
    TGroupBox *grpMakeWhite2DIDList;
    TCheckBox *chkMakeWhite2DIDList;
    void __fastcall Barcode_1ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall btnClearClick(TObject *Sender);
    void __fastcall btnClearMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnClearMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Barcode_2ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall Barcode_3ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall Barcode_4ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall btn_2DBarcodeDataSendClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall edBarcodeScanDelayTimeClick(TObject *Sender);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall btResetBarCodeCountClick(TObject *Sender);
    void __fastcall spbResetComClick(TObject *Sender);
    void __fastcall btReader1OnClick(TObject *Sender);
    void __fastcall btnGPIBReader4Click(TObject *Sender);
    void __fastcall tmr1Timer(TObject *Sender);
    void __fastcall edBarcodePosDelayTimeClick(TObject *Sender);
    void __fastcall spbStartComClick(TObject *Sender);
    void __fastcall pgc2DIDChange(TObject *Sender);
    void __fastcall edBarcodeRetryCountClick(TObject *Sender);
    void __fastcall BtShuttle_1A_ConnectClick(TObject *Sender);
    void __fastcall BtShuttle_1A_DisconnectClick(TObject *Sender);
    void __fastcall btShuttle_1A_TriggerClick(TObject *Sender);
    void __fastcall TimerDownCCDConnectTimer(TObject *Sender);
    void __fastcall ClientSocket_Shuttle1_AConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket_Shuttle1_ADisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket_Shuttle1_AError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ClientSocket_Shuttle1_ARead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall TimerCCDInitialTimer(TObject *Sender);
    void __fastcall TimerProcess2DDataTimer(TObject *Sender);
    void __fastcall btBarcodeChangeFileConnectClick(TObject *Sender);
    void __fastcall btBarcodeChangeFileDisConnectClick(TObject *Sender);
    void __fastcall btBarcodeChangeFileSendClick(TObject *Sender);
    void __fastcall ClientSocket_BarcodeChangeFileConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket_BarcodeChangeFileDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket_BarcodeChangeFileRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall TimerBarcodeChangeFileTimer(TObject *Sender);
    void __fastcall edRetryOffsetMoveClick(TObject *Sender);
    void __fastcall btSaveImageClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall bt2DIDOffsetClick(TObject *Sender);
    void __fastcall btStart2DIDCheckSh1Click(TObject *Sender);
    void __fastcall btStart2DIDCheckSh2Click(TObject *Sender);
    void __fastcall edt2DIDYieldClick(TObject *Sender);
    void __fastcall edtOffsetXClick(TObject *Sender);
    void __fastcall cobMain_BinMapping_Auto1Change(TObject *Sender);
    void __fastcall chkMulti2DIDClick(TObject *Sender);
    void __fastcall rgMulti2DTypeClick(TObject *Sender);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall edtXPitchMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall IPPort1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edtBarcodeMinLengthClick(TObject *Sender);
    void __fastcall edtBarcodeMaxLengthClick(TObject *Sender);
    void __fastcall edt1stLineLength_2DBarCodeMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall TimerBotton8CCDConnectTimer(TObject *Sender);
    void __fastcall TimerBottom8CCDInitialTimer(TObject *Sender);
    void __fastcall btnGetLotIDClick(TObject *Sender);
    void __fastcall btnTestLotIDClick(TObject *Sender);
    void __fastcall ed_2D_YieldIgnoreCntClick(TObject *Sender);
    void __fastcall btnClip_Connect_Auto1Click(TObject *Sender);
    void __fastcall btnClip_Disconnect_Auto1Click(TObject *Sender);
    void __fastcall btnClip_SendCmd_Auto1Click(TObject *Sender);
    void __fastcall btnGetBarcodeClick(TObject *Sender);
    void __fastcall btnInitialGetBarcodeClick(TObject *Sender);
private:    // User declarations
    bool bStartTest[4];
    void TurnOffInspection(int BarCodeIndex);
    //CCD相關------------------------------
    AnsiString sDownCCDRecevieData[BAR_CODE_COUNT]; //KaiChen 20200513 : Bottom 2D 8CCD     //Ifor 20151224 add CCD Bar Code
    double dVisionVer;

    int ChangeCharacter(AnsiString aNum);                   //KaiChen 20191121 ：中壢日月光 2D Check Sum
    bool ReChangeCharacter(int iNum, AnsiString aCom);      //KaiChen 20191121 ：中壢日月光 2D Check Sum
    bool CheckDigit(AnsiString sSource);                    //KaiChen 20191121 ：中壢日月光 2D Check Sum
    //JimmyChiu 20210922 Avoid overflow
    AnsiString LogMsg;

public:     // User declarations
    __fastcall TfBarCode(TComponent* Owner);
    int i2DIDCheckTask;
    void Do2DIDCheck();
    bool Do2DIDCheckSh1();
    bool Do2DIDCheckSh2();

    TComm  *BarcodeCOM              [BAR_CODE_COUNT];
    int     iBarCodeNo              [4][8];
    bool    bEnableCCDChannelConnect[BAR_CODE_COUNT];
    bool    bBarcodeStartDelay      [BAR_CODE_COUNT];
    bool    bBarcodeError           [BAR_CODE_COUNT];
    bool    bBarcodeDataSaveReady   [BAR_CODE_COUNT];
    bool    bCCDBarcodeExposureOK   [BAR_CODE_COUNT];   //Steven 20160106 : 改用CCD拍完就跑的方式
    bool    bCCDBarcodeExposureError[BAR_CODE_COUNT];   //KaiHuang 20200824 : 避免 Exposure Time Out 時又收到 2D 回應,造成 Retry 送錯 cmd 卡死
    bool    bCCDBarcodeGetResultOK  [BAR_CODE_COUNT];   //Steven 20160106 : 改用CCD拍完就跑的方式
    int     iGetSE2                 [BAR_CODE_COUNT];   //Steven 20160120 : Auto Teach 2D Matrix
    int     iGetSE3                 [BAR_CODE_COUNT];   //Steven 20120120 : Auto Learn 2D ROI
    int     iGetSE4                 [BAR_CODE_COUNT];   //Steven 20120126 : 只拍一張, 只確認有無讀取
    bool    bGetSE9                 [BAR_CODE_COUNT];
    bool    bGetGS                  [BAR_CODE_COUNT];
    bool    bGetST;
    bool    bGetGF;
    bool    bGetLF;
    AnsiString sCurrentGF;
    int     iBarCode1_1;
    int     iBarCode1_2;
    int     iBarCode2_1;
    int     iBarCode2_2;

    //KaiChen 20200513 : Bottom 2D 8CCD
    //==>
    int     iBottom_1;
    int     iBottom_2;
    int     iBottom_3;
    int     iBottom_4;
    int     iBottom_5;
    int     iBottom_6;
    int     iBottom_7;
    int     iBottom_8;
    TEdit  *edBottomCCDAddress[8];
    TEdit  *edBottomCCDPort[8];
    int iBottom2DID_8CCDTask;
    bool DoBottom2DID_8CCD_Scan();
    //<==
    //KaiChen 20200513 : Bottom 2D 8CCD

    bool    bShow;
    bool    bImageSaveflag          [BAR_CODE_COUNT];

    TEdit  *edCCDAddress[BAR_CODE_COUNT];
    TEdit  *edCCDPort[BAR_CODE_COUNT];

    bool bGPIBTestBarCodeError;
    AnsiString iBinReturnMess[32];
    bool bGPIBTestBarCodeFormatError;

    int iInitialBarcodeInShuttle1Task;
    int iInitialBarcodeInShuttle2Task;
    int iInitialBarcodeOutShuttle1Task;
    int iInitialBarcodeOutShuttle2Task;

    void __fastcall InitBarCodeRS232(int iStep=2);   //0:Sht1, 1:Sht2, 2:both
    void InitBarCodeParameter();
    void mtBarcodeSetDefaultView();

    bool Barcode_1StartScan(int BarCodeIndex, int Sht);
    bool Barcode_StartScan_In(int BarCodeIndex, int iXpos, AnsiString &cDeviceInf, AnsiString cLastDeviceInf);
    bool Barcode_4StartScan_In(int iXpos, int iSize, AnsiString &cDeviceInf, AnsiString cLastDeviceInf);

    void InitialBarcodeScanInShuttle1(bool bClear2DID=true);
    void InitialBarcodeScanInShuttle2(bool bClear2DID=true);
    void InitialBarcodeScanOutShuttle1();
    void InitialBarcodeScanOutShuttle2();
    bool DoBarcodeScanInShuttle_1();
    bool DoBarcodeScanInShuttle_2();
    bool DoBarcodeTriggerInShuttle_1();                                         //Steven 20151225 : 改用拍完就跑的方式
    bool DoBarcodeTriggerInShuttle_2();                                         //Steven 20151225 : 改用拍完就跑的方式
    bool DoBarcodeCCDInShuttle_1(bool bVerify=false);                           //Steven 20160106 : 改用CCD拍完就跑的方式
    bool DoBarcodeCCDInShuttle_2(bool bVerify=false);                           //Steven 20160106 : 改用CCD拍完就跑的方式
    bool DoBarcodeCCDAutoTeach(bool bReset=false);                              //Steven 20160118 : 自動學習2D code Matrix
    bool DoBarcodeScanOutShuttle_1();
    bool DoBarcodeScanOutShuttle_2();
    int iShtDupilcateFailCnt[2];

    int iBottom2DIDTask;
    void InitBottom2DIDScan(bool bClear2DID=true);                              //Steven 20190308 : Bottom 2D
    bool DoBottom2DIDScan();

    AnsiString GetSimuBarCodeName(int iRow, int iCol);

    void __fastcall ReadFile();
    void __fastcall DoIniDataToForm();
    void InitSht2DCodeComPort(int iStep);                                       //Steven 20151221 : 重置Com Port, 避免異常
    void WriteBarCodeLog(int index, AnsiString Message);
    void ClearLogName();                                                        //Steven 20160728 : Log by running mode
    AnsiString asBarcodeLogName;                                                //Steven 20160728 : Log by running mode
    int  iRunningMode;                                                          //Steven 20160728 : Log by running mode

    void CCDBarcode_Decoding(int Channel);                                      //Ifor 20151228 CCD BarcodeDecoding
    void CCDBarcode_DecodingForMulti2D(int Channel);                            //Steven 20200810 : 一個IC使用多個2DID
    void CCDBarcode_SortingForMulti2D(int iSht);                                //Steven 20200810 : 一個IC使用多個2DID
    void SetVisible();
    void SendCCDCommand(int index, AnsiString Msg, AnsiString Msg2="");
    void Change2DSetupFile();

    TStringList *Socket2DReceiveList[BAR_CODE_COUNT];
    TStringList *list2DByLot;           //用來存檔目前讀到的資料到硬碟          //Steven 20160429 : 開程式要把2D List讀回來
    TStringList *list2DSorting;
    TStringList *s2DSorting;
    TStringList *listError2DID;                                                 //JerryYang 20231218 : 2DID黑名單功能
    TStringList *list2DWhitle;
    TStringList *list2DWhitleResult;
    TStringList *list2DWhitleTemp;

    std::map<AnsiString, int>map2DSortingList;                                  //Steven 20240515 : modified for 2D sort
    std::map<AnsiString, int>mapMulti2DList;
    std::map<AnsiString, int>::iterator map2DSortIter;
    void TransformListToMap();
    int  GotBinFrom2DSortList(AnsiString s2DID);

    std::map<AnsiString, AnsiString>map2DShuttleList[BAR_CODE_COUNT];
    std::map<AnsiString, AnsiString>::iterator mapShtIter[BAR_CODE_COUNT];
    std::map<AnsiString, AnsiString>map2DList;
    std::map<AnsiString, AnsiString>::iterator map2DIter;
    std::map<AnsiString, AnsiString>check2DList;                                //wei 20160728 Barcode File切換
    int iCurrentStep[BAR_CODE_COUNT];                                           //Steven 20160503 : 確認移動位置與拍照位置相同
    bool bStepError[BAR_CODE_COUNT];                                            //Steven 20160503 : 確認移動位置與拍照位置相同

    bool DoBarcodeCount();                                                      //wei 20160325 Barcode Rate計算             //Steven 20171222 (Wei) : 改成判斷要不要Alarm
    void AddCCDCommunicationLog(int index, AnsiString Str);                     //Steven 20160421 : Add 2D CCD Log
    AnsiString sDuplicateErrPart[BAR_CODE_COUNT];                               //Steven 20160425 : 確認重複碼
    bool bHasDuplicateCode[BAR_CODE_COUNT];                                     //Steven 20160425 : 確認重複碼
    bool DoBarcodeCheck(int iBarCode, AnsiString &Str,int iXItem);              //wei 20160505 Barcode 比對Lot
//    TStringList *list4;                                                         //wei 20160505 Barcode 比對Lot
    int iInitialChangeFileTask;                                                 //wei 20160729
    void InitialBarcodeScanChangeFile()  {iInitialChangeFileTask =1;};
    bool DoBarcodeChangeFile();
    void CleanBarcodeError(int iStep);

    TQPF_Timer BarcodeDelay[BAR_CODE_COUNT];
    TQPF_Timer BarcodeExposureDelay[2];
    TQPF_Timer BarcodePosDelay[2];                                              //wei 20151126 BarCode Pos Delay
    TQPF_Timer Bottom2DPosDelay;
    TQPF_Timer Bottom2DExposureDelay;
    TQPF_Timer Bottom2DDelay;

    TQPF_Timer BarcodeWaitReplyTimeOut[BAR_CODE_COUNT];                         //Ifor 20190225 :add Bar Code Use HandShake Communication

    //Steven 20160920 : IC置偏檢查
    int iShuttleFloatCheck1Task;
    int iShuttleFloatCheck2Task;
    int iSFCAutoTune1Task;
    int iSFCAutoTune2Task;
    int iSFCTotalMoveStep;
    void SetSFCCheckStepCount();
    void InitialShuttleFloatCheck1();
    void InitialShuttleFloatCheck2();
    bool DoShuttleFloatCheck_1();
    bool DoShuttleFloatCheck_2();
    void InitialSFCAutoTune1(bool bHasIC);
    void InitialSFCAutoTune2(bool bHasIC);
    bool DoSFCAutoTune_1(bool bHasIC);
    bool DoSFCAutoTune_2(bool bHasIC);
    TQPF_Timer SFCStartDelay[BAR_CODE_COUNT];
    TQPF_Timer SFCExposureDelay[2];
    TQPF_Timer SFCResultDelay[2];
    AnsiString sSFCCurrentGF;
    bool    bSFCGetGF;
    bool    bSFCGetLF;
    bool    bSFCGetVR;
    int     iSFCCurrentStep[BAR_CODE_COUNT];                                    //Steven 20160503 : 確認移動位置與拍照位置相同
    bool    bSFCStepError[BAR_CODE_COUNT];                                      //Steven 20160503 : 確認移動位置與拍照位置相同
    bool    bSFCEnableCCDChannelConnect[BAR_CODE_COUNT];
    bool    bSFCStartDelay      [BAR_CODE_COUNT];
    bool    bSFCError           [BAR_CODE_COUNT];
    bool    bSFCDataSaveReady   [BAR_CODE_COUNT];
    bool    bSFCExposureOK      [BAR_CODE_COUNT];
    bool    bSFCGetResultOK     [BAR_CODE_COUNT];
    bool    bSFCGetSE9          [BAR_CODE_COUNT];
    bool    iSFCCheckResult     [BAR_CODE_COUNT][16];
    bool    CCDSFC_Decoding(int Channel);
    AnsiString DoBarcodeAnalysis(AnsiString str);                               //wei 20161223 (Steven) Barcode 品質顯示
    void __fastcall SaveFailImage(int Pos);
    TMemoryStream   *BarCode_SaveImage[BAR_CODE_COUNT];                         //Frank 20170408 (Steven) add Save Fail Image
    AnsiString s2DIDYield;
    bool bNeedScanBottom2D[2][4];
    bool DoCheckSHT2DIDStatus(int SHT);                                         //Isacc 20180329 (Steven) 2DID All site fail recheck twice and alarm
    bool MoveInArmXY_ToBottom2DID();
    void CheckWhichKitBottom2DID();
    int iBottomKit;
    bool bBottom2DNeedMoveInArm;
    bool bCCDBarcodeWaitReply   [BAR_CODE_COUNT];                               //Ifor 20190225 : Add 等待Reader 回覆
    void Read2DIDList();                                                        //Steven 20190604 : 2DID不在List內的另外分bin
    int  b2DIDIsInsideList(int i, int j);                                       //Steven 20190604 : 2DID不在List內的另外分bin
    int  b2DIDIsInsideToErrorBin(int i, int j);                                 //JerryYang 20231218 : 2DID黑名單功能
    int  bSHT_2DIDIsInsideList(int iSHT, int i, int j, int *iIndex);            //JerryYang 20241104 : 支援2DID白名單功能

    TTMyTray *ShowBinMappingUnloadPtr[eTrayCount];                              //jou 20190930 : Barcode Tray record file
    TComboBox *ShowBinMappingAuto_Index_Ptr[eTrayCount];                        //jou 20190930 : Barcode Tray record file
    void __fastcall DoShow_BinMapping_Info(int iAuto);                          //jou 20190930 : Barcode Tray record file
    void __fastcall DoShow_BinMapping_Info_Old_Data(int iAuto, int iTrayNumber);//jou 20190930 : Barcode Tray record file
    void __fastcall WriteTrayInfo( AnsiString asData, int iAuto, int iTrayNo ); //jou 20190930 : Barcode Tray record file
    void __fastcall BarWriteTrayMap( AnsiString asData, int iAuto, int iTrayNo);//kevin 20220326 : Auto 1 2 3 tray map
    void __fastcall Write_Device_Info_By_Tray(int iAuto, int iTray_Number);     //jou 20190930 : Barcode Tray record file
    void __fastcall Read_Device_Info_By_Tray(int iAuto, int iTray_Number);      //jou 20190930 : Barcode Tray record file
    void __fastcall InitBarcodeRecFile();                                       //jou 20190930 : Barcode Tray record file
    bool IsSHT2DIDScanFinish(int SHT);                                          //JerryYang 20200916 : 判斷是否掃完2D
    void __fastcall SetMulti2DMap(int iType);
    bool __fastcall CheckMulti2DMap();
    void ClearBuffer(int iShuttle, AnsiString Log);                             //Steven 20200908 : 整合2DID Clear Buffer
    void SendSE0(int iShuttle, int iHasIC, int iNowMoveStep, int iiNowMoveStepMulti2D, AnsiString Log); //Steven 20200908 : 整合2DID SE0
    AnsiString cMilti2DBuffer[BAR_CODE_COUNT][_MAX_SUCK_COL_ITEM*4];
    _MyMulti2DStruct cMilti2DList[2][BAR_CODE_COUNT][_MAX_SUCK_COL_ITEM];
    int GetMovePos(int iSht, int iNowMoveStep, int iNowMoveStepMulti2DX, bool bAlarm=true);
    char mtShowBarcodeTray[6][_MAX_COL_ITEM][_MAX_ROW_ITEM][24];                //jou 20190930 : Barcode Tray record file
    //Ifor 20210407 add: 自製OCR
    //==>
    AnsiString asMultiLine_Ocr[5];
    bool bHasMultiLineBarcode[BAR_CODE_COUNT];
    //<==
    //Ifor 20210407 add: 自製OCR
    bool __fastcall DoPassword();                                               //KaiHuang 20201203 add for ASE-CL L8
    bool RunCheckBarcodeByServerData();                                         //Jimmychiu 20230925 : read 2did in json file
    AnsiString GetBarcodeByServerData();                                        //Jimmychiu 20230925 : read 2did in json file
    bool GetWhite2DIDList(AnsiString LotID, AnsiString sRunMode="FT1");         //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    bool JCETUseMakeWhite2DIDList();                                            //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    bool UpdateWhite2DIDList(AnsiString LotID, AnsiString sRunMode);            //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    int iJCETWhitelistSN;                                                       //RogerYang 20251214 : JCET 2D FT1白名單/FT2比對功能
    bool bWhitelistAlarm;                                                       //JerryYang 20241104 : 支援2DID白名單功能
    bool bNeedCheckWhitleList;
    enum eMulti2DType
    {
        eUnloaderClipAuto1=0,
        eUnloaderClipAuto2,
        eUnloaderClipAuto3,
        eUnloaderClipFix1,
        eUnloaderClipFix2,
        eUnloaderClipFix3,
        eUnloaderClipTotal
    };
    uCCDUnloaderClip ccdUnloader[eUnloaderClipTotal];
    void ReadUnloaderClipIni();
    void WriteUnloaderClipIni();
    AnsiString GetUnloaderClipFileName(){return AnsiString("UnloaderClipSetting.ini");}
    void InitUnloaderClipEdtList();
    HTEditList *elUnloaderClip;
    void RecMsgCCDClip(AnsiString asMsg);
    void InitialAllCCDUnloaderClip();
};
//---------------------------------------------------------------------------
extern PACKAGE TfBarCode *fBarCode;
extern int iConntectionOkTask;
extern int i2DIDCheckSH1Task;
extern int i2DIDCheckSH2Task;
//---------------------------------------------------------------------------
#endif
