//---------------------------------------------------------------------------

#ifndef cTrayMappingH
#define cTrayMappingH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "SPComm.hpp"
#include <ScktComp.hpp>
//#include "HTimer.h"
#include <Grids.hpp>
#include <NMFtp.hpp>
#include <Psock.hpp>
#include <FileCtrl.hpp>
#include "MyMotor.h"
#include "HTray.h"
#include "ProductionInfo.h"

//---------------------------------------------------------------------------
class TfAOI;
class TfTrayDeviceCheckFromLoader;
class TfTrayMapFromLoader;
//---------------------------------------------------------------------------
class cDatabaseJson : public uBasicPickPlace
{
private:
    AnsiString sTableName;
    TStringList *slSchemaList;

public:     // User declarations
    cDatabaseJson();
    ~cDatabaseJson();
    cJSON* table_root;
    void AddColumnTitle(AnsiString titleListByDot);
    bool AddRowData(AnsiString dataListByDot);
    bool UpdateRowData(int iPos,AnsiString dataListByDot);
    //To File
    bool LoadFile(AnsiString FileName);
    void SaveFile(AnsiString FileName);
};
//---------------------------------------------------------------------------
class cLineScanRemainICYieldRecord
{
private:
    AnsiString GetDirector(){return "D:\\HT9045_Log";}
    AnsiString GetFolderName(){return "LineScanRemainICYield";}
    AnsiString GetNowYearMonth();
    AnsiString GetFileName();
    AnsiString GetTotalNumFileName(){return "TotalNum.json";}
    cDatabaseJson dbJ_History;
    cDatabaseJson dbJ_TotalNum;
    void RefreshTotalNum(AnsiString sDatetime,bool bYield);
public:     // User declarations
    cLineScanRemainICYieldRecord();
    ~cLineScanRemainICYieldRecord();
    AnsiString GetFileNameNoExt();
    AnsiString GetFileNameWithDir();
    AnsiString GetTotalNumDir();
    AnsiString GetYield();
    void InitialParameter();
    void AddYieldData(bool bYield);                                             //true:無殘料  false:有殘料
};
//---------------------------------------------------------------------------
class TfRFID                                                                    //RogerYang 20250830 : Add RFID
{
private:
    TComm *COMRFID;
    TMemo *MemoStrLog;
    bool bChannelChange;
    int mSearchStage;
    int gStartBlock;
    int gBlockCount;
//    bool bAutoRead;
    int iAutoReadTimeout;
    bool bShowRequestLog;

    void SendNFCClearFlag(int iCh);
public:
    TfRFID(TComm *commRFID, TMemo *memoLdRFID);
    ~TfRFID();
    void ShowCommData(AnsiString sType, AnsiString sData);
//    void ComPortChagne(int ComPort);
//    void ChannelChange(int Channel);
    void SendNFCReadHardwareVersion(int iCh);
    void SendNFCReadFirmwareVersion(int iCh);
    void SendNFCReaderGetID(int iCh);
    void SendNFCReadMemData(int iCh);
    void SendNFCReadBlock(int iCh);
    void SendNFCReadUID(int iCh);
    bool OpenCommPort();
    bool CloseCommPort();
    bool bComPortConnect;
    int  iFunction;
    bool bCommConnect;

    void SendNFCRequestToSlaveNumberState(int iCh);
    void SendNFCReadSearchBlock(int startBlock, int blockCount);
    void COMRFIDReceiveData(TObject *Sender, Pointer Buffer, WORD BufferLength);
    void SetClearFlag(int iCh);
    void ReadUID(int iCh);
    AnsiString sUID1;
    AnsiString sBlockID1;
    AnsiString sUID2;
    AnsiString sBlockID2;
//    void UIDReadTimer();
    bool GetRecvStat();
    bool bRecv;

    bool bClearCmd1;
    bool bReadUID1;
    bool bReadBlock1;

    bool bClearCmd2;
    bool bReadUID2;
    bool bReadBlock2;

    enum eRFIDCmd
    {
        eNFCReadHardwareVersion1=1,
        eNFCReadFirmwareVersion1=2,
        eNFCReaderGetID1        =3,
        eNFCReadUID1            =4,
        eNFCReadMemData1        =5,
        eNFCReadBlock1          =6,
        eNFCReqToSlaveNoState1  =7,
        eNFCReadSearchBlock1    =8,
        eNFCClearFlag1          =9,

        eNFCReadHardwareVersion2=10,
        eNFCReadFirmwareVersion2=11,
        eNFCReaderGetID2        =12,
        eNFCReadUID2            =13,
        eNFCReadMemData2        =14,
        eNFCReadBlock2          =15,
        eNFCReqToSlaveNoState2  =16,
        eNFCReadSearchBlock2    =17,
        eNFCClearFlag2          =18,

        eRFIDCmdTotal
    };
    int iNeedTDoFlag[eRFIDCmdTotal];
};
//---------------------------------------------------------------------------
class TfTrayMapping : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *tsTraySetting;
    TPanel *Panel1;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TClientSocket *ClientSocket_Tray_ID;
    TClientSocket *ClientSocket_Tray_Map;
    TTimer *TimerDownCCDTrayConnect;
    TTimer *TimerCCDTrayInitial;
    TTabSheet *tsTrayCCD;
    TPanel *Panel2;
    TGroupBox *GroupBox2;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *edTray_ID_Address;
    TEdit *edTray_ID_Port;
    TBitBtn *BtTray_ID_Connect;
    TBitBtn *BtTray_ID_Disconnect;
    TButton *btTray_ID_Trigger;
    TEdit *edTray_ID_CMD;
    TGroupBox *GroupBox3;
    TLabel *Label7;
    TLabel *Label8;
    TEdit *edTray_Map_Address;
    TEdit *edTray_Map_Port;
    TBitBtn *BtTray_Map_Connect;
    TBitBtn *BtTray_Map_Disconnect;
    TButton *btTray_Map_Trigger;
    TEdit *edTray_Map_CMD;
    TMemo *memoTrayCCDCommLog;
    TLabel *labSFCGetResultTimeOut;
    TLabel *labSFCStartDelay;
    TLabel *labSFCAutoRetry;
    TLabel *labSFCExposureTimeOut;
    TCheckBox *cbEnableTrayMap;
    TCheckBox *cbEnableTrayID;
    TEdit *edTrayGetResultTimeOut;
    TEdit *edTrayStartDelay;
    TEdit *edTrayAutoRetry;
    TEdit *edTrayExposureTimeOut;
    TTabSheet *tsView;
    TLabel *Label1;
    TLabel *labTrayID;
    TLabel *labTrayIDStatus;
    TLabel *labTrayMapStatus;
    TGroupBox *GroupBox1;
    TEdit *Edit1;
    TEdit *Edit2;
    TButton *Button1;
    TButton *Button2;
    TCheckBox *cbBarCodeSimulate;
    TEdit *Edit3;
    TMemo *MemoTrayMap;
    TPageControl *PageControl2;
    TTabSheet *tsTrayView;
    TStringGrid *sgTraymap;
    TTabSheet *tsSuckView;
    TStringGrid *sgSuckmap;
    TButton *Button4;
    TDirectoryListBox *DirectoryListBox1;
    TFileListBox *FileListBox1;
    TTimer *TimerProcessTrayData;
    TCheckBox *cbEnableDeviceRemain;
    TCheckBox *CheckBox1;
    TGroupBox *GroupBox4;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *edTray_ID2_Address;
    TEdit *edTray_ID2_Port;
    TBitBtn *BtTray_ID2_Connect;
    TBitBtn *BtTray_ID2_Disconnect;
    TButton *btTray_ID2_Trigger;
    TEdit *edTray_ID2_CMD;
    TCheckBox *cbEnableTrayID2;
    TClientSocket *ClientSocket_Tray_ID2;
    TLabel *Label4;
    TLabel *labTrayID2;
    TLabel *labTrayID2Status;
    TEdit *Edit4;
    TTabSheet *tsAutoTrayView1;
    TTabSheet *tsAutoTrayView2;
    TTabSheet *tsAutoTrayView3;
    TStringGrid *sgTrayDeviceCnt1;
    TStringGrid *sgTrayDeviceCnt2;
    TStringGrid *sgTrayDeviceCnt3;
    TCheckBox *cbEnableTrayDeviceCnt;
    TClientSocket *ClientSocket_Tray_DeviceCnt3;
    TClientSocket *ClientSocket_Tray_DeviceCnt2;
    TClientSocket *ClientSocket_Tray_DeviceCnt1;
    TGroupBox *GroupBox6;
    TLabel *Label11;
    TLabel *Label12;
    TEdit *edTrayDeviceCnt1_Address;
    TEdit *edTrayDeviceCnt1_Port;
    TBitBtn *btnTrayDeviceCnt1_Connect;
    TBitBtn *btnTrayDeviceCnt1_Disconnect;
    TButton *btnTrayDeviceCnt1_Trigger;
    TEdit *edTrayDeviceCnt1_CMD;
    TGroupBox *GroupBox7;
    TLabel *Label13;
    TLabel *Label14;
    TEdit *edTrayDeviceCnt2_Address;
    TEdit *edTrayDeviceCnt2_Port;
    TBitBtn *btnTrayDeviceCnt2_Connect;
    TBitBtn *btnTrayDeviceCnt2_Disconnect;
    TButton *btnTrayDeviceCnt2_Trigger;
    TEdit *edTrayDeviceCnt2_CMD;
    TGroupBox *GroupBox5;
    TLabel *Label9;
    TLabel *Label10;
    TEdit *edTrayDeviceCnt3_Address;
    TEdit *edTrayDeviceCnt3_Port;
    TBitBtn *btnTrayDeviceCnt3_Connect;
    TBitBtn *btnTrayDeviceCnt3_Disconnect;
    TButton *btnTrayDeviceCnt3_Trigger;
    TEdit *edTrayDeviceCnt3_CMD;
    TPanel *palTrayIDGroup;
    TLabel *labCodeMinLen;
    TEdit *edtTraycodeMinLength;
    TEdit *edtTraycodeMaxLength;
    TLabel *labCodeMaxLen;
    TLabel *labTrayIDShift;
    TEdit *edtTrayIDShift;
    TLabel *labTrayID2Shift;
    TEdit *edtTrayID2Shift;
    TCheckBox *cbEnableOCRTrayID;
    TCheckBox *cbEnableTrayIDFTP;
    TPanel *palTrayMapFunGroup;
    TCheckBox *cbDisableMapSuck;
    TCheckBox *cbEnableSuckMapCheck;
    TPanel *palTrayMapGroup;
    TEdit *edtTrayMapCatch;
    TLabel *labTrayMapCatch;
    TLabel *labTrayMapStartPos;
    TEdit *edtTrayMapStart;
    TPanel *palDeviceCountGroup;
    TLabel *lblTrayDeciveCntStartPos;
    TEdit *edtTrayDeciveCntStart;
    TTabSheet *TabSheet1;
    TPanel *Panel3;
    TGroupBox *GroupBox8;
    TLabel *Label18;
    TLabel *Label19;
    TEdit *edTray_ID2_AddressKY;
    TEdit *edTray_ID2_PortKY;
    TBitBtn *BtTray_ID2_ConnectKY;
    TBitBtn *BtTray_ID2_DisconnectKY;
    TButton *btTray_ID2_TriggerKY;
    TEdit *edTray_ID2_CMDKY;
    TGroupBox *GroupBox9;
    TLabel *Label20;
    TLabel *Label21;
    TEdit *edTray_ID_AddressKY;
    TEdit *edTray_ID_PortKY;
    TBitBtn *BtTray_ID_ConnectKY;
    TBitBtn *BtTray_ID_DisconnectKY;
    TButton *btTray_ID_TriggerKY;
    TEdit *edTray_ID_CMDKY;
    TGroupBox *GroupBox10;
    TLabel *Label22;
    TLabel *Label23;
    TEdit *edTray_ID4_AddressKY;
    TEdit *edTray_ID4_PortKY;
    TBitBtn *BtTray_ID3_ConnectKY;
    TBitBtn *BtTray_ID3_DisconnectKY;
    TButton *btTray_ID4_TriggerKY;
    TEdit *edTray_ID4_CMDKY;
    TGroupBox *GroupBox11;
    TLabel *Label24;
    TLabel *Label25;
    TEdit *edTray_ID3_AddressKY;
    TEdit *edTray_ID3_PortKY;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TButton *btTray_ID3_TriggerKY;
    TEdit *edTray_ID3_CMDKY;
    TMemo *memoKeyenceCommLog;
    TClientSocket *ClientSocket_Keyence1;
    TClientSocket *ClientSocket_Keyence2;
    TClientSocket *ClientSocket_Keyence3;
    TClientSocket *ClientSocket_Keyence4;
    TCheckBox *cbEnableTrayID3;
    TTabSheet *tsLoaderDeviceCheck;
    TGroupBox *GroupBox12;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label26;
    TLabel *Label27;
    TLabel *Label28;
    TLabel *Label29;
    TLabel *lbTaskLDC;
    TEdit *edLoaderDC_Address;
    TEdit *edLoaderDC_Port;
    TBitBtn *BtLoaderDC_Connect;
    TEdit *edLoopTimes;
    TComboBox *cbLaserScanMode;
    TEdit *edWaitAOITimes;
    TEdit *edLoaderCWSpeed;
    TPanel *Panel4;
    TPanel *Panel5;
    TCheckBox *cbAutoLearning;
    TPanel *Panel6;
    TButton *btnSimulateRemainIC;
    TPanel *Panel8;
    TPanel *Panel7;
    TCheckBox *cbEnableRetryCount;
    TEdit *edRemainIC_Laser_Retry;
    TPanel *Panel9;
    TCheckBox *cbAutoSkip;
    TPanel *Panel10;
    TCheckBox *cbRunAtInitialStart;
    TMemo *mLoaderDCCommLog;
    TPanel *plTrayMap;
    TPageControl *PageControl3;
    TTabSheet *tsCommand;
    TPanel *Panel11;
    TEdit *edLoaderDC_CMD;
    TButton *btLoaderDC_Trigger;
    TButton *btnLoadFile;
    TButton *btnClearBuffer;
    TButton *btnGrab;
    TButton *btnGetValue;
    TButton *btnSetLot;
    TButton *btnCreateFile;
    TButton *btnGoldenImage;
    TCheckBox *cbPassAOI;
    TCheckBox *cbPassActive;
    TCheckBox *cbSimulationAOICommand;
    TGroupBox *GroupBox13;
    TLabel *Label30;
    TButton *btnMoveIn;
    TButton *btnMoveOut;
    TTabSheet *TabSheet4;
    TTabSheet *TabSheet2;
    TLabel *lbTrayMapName;
    TTMyTray *mtTrayMap;
    TTabSheet *TabSheet3;
    TLabel *Label31;
    TLabel *lbResult;
    TTabSheet *TabSheet5;
    TButton *btnAddRandom;
    TButton *btnRefreshYield;
    TStringGrid *sgTrayDeviceYield;
    TCheckBox *cbEnableTrayMapLaser;
    TCheckBox *cbEnableDeviceRemainLaser;
    TTabSheet *TabSheet6;
    TClientSocket *ClientSocket_CoverTray_ID_LoaderCar;
    TClientSocket *ClientSocket_CoverTray_ID_Loader;
    TClientSocket *ClientSocket_CoverTray_ID_AUTO1Car;
    TClientSocket *ClientSocket_CoverTray_ID_AUTO2Car;
    TClientSocket *ClientSocket_CoverTray_ID_AUTO3Car;
    TPanel *Panel12;
    TGroupBox *GroupBox17;
    TLabel *Label38;
    TLabel *Label39;
    TEdit *edCoverTray_ID5_AddressKY;
    TEdit *edCoverTray_ID5_PortKY;
    TBitBtn *BitBtn9;
    TBitBtn *BitBtn10;
    TButton *Button7;
    TEdit *edCoverTray_ID_AUTO3Car_CMDKY;
    TGroupBox *GroupBox16;
    TLabel *Label36;
    TLabel *Label37;
    TEdit *edCoverTray_ID3_AddressKY;
    TEdit *edCoverTray_ID3_PortKY;
    TBitBtn *BitBtn7;
    TBitBtn *BitBtn8;
    TButton *Button6;
    TEdit *edCoverTray_ID_AUTO1Car_CMDKY;
    TGroupBox *GroupBox14;
    TLabel *Label32;
    TLabel *Label33;
    TEdit *edCoverTray_ID_AddressKY;
    TEdit *edCoverTray_ID_PortKY;
    TBitBtn *BitBtn3;
    TBitBtn *BitBtn4;
    TButton *Button3;
    TEdit *edCoverTray_ID_LoaderCar_CMDKY;
    TGroupBox *GroupBox15;
    TLabel *Label34;
    TLabel *Label35;
    TEdit *edCoverTray_ID2_AddressKY;
    TEdit *edCoverTray_ID2_PortKY;
    TBitBtn *BitBtn5;
    TBitBtn *BitBtn6;
    TButton *Button5;
    TEdit *edCoverTray_ID_Loader_CMDKY;
    TMemo *MemoKeyenceLogCover;
    TGroupBox *GroupBox18;
    TLabel *Label40;
    TLabel *Label41;
    TEdit *edCoverTray_ID4_AddressKY;
    TEdit *edCoverTray_ID4_PortKY;
    TBitBtn *BitBtn11;
    TBitBtn *BitBtn12;
    TButton *Button8;
    TEdit *edCoverTray_ID_AUTO2Car_CMDKY;
    TTabSheet *tsLoaderRFID;
    TComm *commRFID;
    TGroupBox *grpRFIDCommSetting;
    TButton *btnLdRFIDConn;
    TButton *btnLdRFIDDisConn;
    TComboBox *cbbLdRFIDComPort;
    TPanel *pnlLdRFIDState;
    TMemo *memoLdRFID;
    TGroupBox *grpNFC_CH1;
    TButton *btnLdRFIDClear1;
    TButton *btnReadMemData1;
    TButton *btnReadUID1;
    TButton *btnReadID1;
    TButton *btnLdRFIDReadBlock1;
    TGroupBox *grpNFC_CH2;
    TButton *btnLdRFIDClear2;
    TButton *btnReadMemData2;
    TButton *btnReadUID2;
    TButton *btnReadID2;
    TButton *btnLdRFIDReadBlock2;
    TTimer *tmrNFC;
    TLabel *lblNFC_CH1_UID;
    TLabel *lblNFC_CH1_CID;
    TLabel *lblNFC_CH2_UID;
    TLabel *lblNFC_CH2_CID;
    void __fastcall btTray_ID_TriggerClick(TObject *Sender);
    void __fastcall TimerDownCCDTrayConnectTimer(TObject *Sender);
    void __fastcall TimerCCDTrayInitialTimer(TObject *Sender);
    void __fastcall BtTray_ID_ConnectClick(TObject *Sender);
    void __fastcall BtTray_ID_DisconnectClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ClientSocket_Tray_IDDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket_Tray_IDError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ClientSocket_Tray_IDRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket_Tray_IDConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall TimerProcessTrayDataTimer(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
//    void __fastcall memoKeyenceTraCommLogChange(TObject *Sender);
    void __fastcall PageControl1Change(TObject *Sender);
    void __fastcall edTrayStartDelayClick(TObject *Sender);
    void __fastcall edTrayAutoRetryClick(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall edtTrayMapCatchClick(TObject *Sender);
    void __fastcall edtTrayMapShiftClick(TObject *Sender);
    void __fastcall edtTrayIDReadShiftClick(TObject *Sender);
    void __fastcall edtTrayDeciveCntStartClick(TObject *Sender);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall ClientSocket_Keyence1Connect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket_Keyence1Error(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ClientSocket_Keyence1Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket_Keyence1Read(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall BtTray_ID_ConnectKYClick(TObject *Sender);
    void __fastcall BtTray_ID_DisconnectKYClick(TObject *Sender);
    void __fastcall btTray_ID_TriggerKYClick(TObject *Sender);
    void __fastcall edTray_ID_PortMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall BtLoaderDC_ConnectClick(TObject *Sender);
    void __fastcall btLoaderDC_TriggerClick(TObject *Sender);
    void __fastcall btnClearBufferClick(TObject *Sender);
    void __fastcall btnLoadFileClick(TObject *Sender);
    void __fastcall btnGrabClick(TObject *Sender);
    void __fastcall btnGetValueClick(TObject *Sender);
    void __fastcall btnSetLotClick(TObject *Sender);
    void __fastcall btnCreateFileClick(TObject *Sender);
    void __fastcall btnGoldenImageClick(TObject *Sender);
    void __fastcall cbLaserScanModeChange(TObject *Sender);
    void __fastcall btnSimulateRemainICClick(TObject *Sender);
    void __fastcall edLoopTimesKeyPress(TObject *Sender, char &Key);
    void __fastcall cbSimulationAOICommandClick(TObject *Sender);
    void __fastcall btnMoveOutClick(TObject *Sender);
    void __fastcall btnMoveInClick(TObject *Sender);
    void __fastcall Label18MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnAddRandomClick(TObject *Sender);
    void __fastcall btnRefreshYieldClick(TObject *Sender);
    void __fastcall ClientSocket_CoverTray_ID_LoaderCarConnect(
          TObject *Sender, TCustomWinSocket *Socket);
    void __fastcall btnLdRFIDConnClick(TObject *Sender);
    void __fastcall commRFIDReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall btnLdRFIDDisConnClick(TObject *Sender);
    void __fastcall btnLdRFIDClear1Click(TObject *Sender);
    void __fastcall btnReadID1Click(TObject *Sender);
    void __fastcall btnReadUID1Click(TObject *Sender);
    void __fastcall btnReadMemData1Click(TObject *Sender);
    void __fastcall btnLdRFIDReadBlock1Click(TObject *Sender);
    void __fastcall btnLdRFIDClear2Click(TObject *Sender);
    void __fastcall btnReadUID2Click(TObject *Sender);
    void __fastcall btnLdRFIDReadBlock2Click(TObject *Sender);
    void __fastcall btnReadMemData2Click(TObject *Sender);
    void __fastcall btnReadID2Click(TObject *Sender);
    void __fastcall tmrNFCTimer(TObject *Sender);
private:    // User declarations
    AnsiString sDownCCDRecevieData[iTotalFunction];                             //Sam 20190405 : Tray Decive Count //Ifor 20151224 add CCD Bar Code
    TClientSocket *clntsckt_Tray[iTotalFunction];
    TEdit *edAddress[iTotalFunction];
    TEdit *edPort[iTotalFunction];
    TClientSocket *clntsckt_Tray_Keyence[9];
    TEdit *edAddress_Keyence[9];
    TEdit *edPort_Keyence[9];
    double dVisionVer;
    //JimmyChiu 20220219 : 殘料檢查 From Loader
    //<==
    /************************************/
    std::map<int,TfAOI*> mapAOI;
    void AddAOIMemeber(TfAOI *aoi);
    int GetAOISize(){return mapAOI.size();}
    void DeleteAOI(){mapAOI.erase(GetAOISize()-1);}
    void ClearAllAOI();
    /************************************/
    void ShowMSDCCommLog(AnsiString asStr);
    void UpdateConnectionState(TBitBtn * btn);
    bool IsNumber(AnsiString& asValue);
    int iLoaderYCWSpeedPerc;
    bool bSimulate;
    int iSettingCount;
    //<==
    //JimmyChiu 20220219 : 殘料檢查 From Loader
public:     // User declarations
    __fastcall TfTrayMapping(TComponent* Owner);
    __fastcall ~TfTrayMapping();                                                //KevinCheng 20250830 : Add RFID
    void __fastcall DoIniDataToForm();
    void SendCCDTrayCommand(int index, AnsiString Msg, AnsiString Msg2="");
    void AddCCDTrayCommunicationLog(int index, AnsiString Str);                 //Steven 20160421 : Add 2D CCD Log
    void ChangeTraySetupFile();
    bool    bEnableCCDTrayChannelConnect[iTotalFunction];                       //Sam 20190405 : Tray Decive Count
    bool    bSFCGetGF;
    bool    bSFCGetLF;
    bool    bTrayGetVR;
    int     iGetSE2[iTotalFunction];                                            //Sam 20190405 : Tray Decive Count
    bool    bGetTraySE9[iTotalFunction];                                        //Sam 20190405 : Tray Decive Count
    bool    bGetST;
    bool    bGetGF;
    bool    bGetLF;
    bool    bGetSF;
    bool    bGetCF;
    AnsiString sCurrentGF;
    bool    bSFCGetSE9[iTotalFunction];                                         //Sam 20190405 : Tray Decive Count
    AnsiString sSFCCurrentGF;
    bool    bShow;
    bool    bDoTrayMapTune;
    TStringList *SocketTrayReceiveList[iTotalFunction];                         //Sam 20190405 : Tray Decive Count
    void __fastcall ReadFile();
    bool    bTrayMapGetResultOK[iTotalFunction];                                //Sam 20190405 : Tray Decive Count
    bool    bTrayMapExposureOK[iTotalFunction];                                 //Sam 20190405 : Tray Decive Count
    AnsiString sTrayMapFileName[iTotalFunction];                                //Sam 20200211 : Vision Tray Map 通訊增加 圖檔名紀錄
    bool    CCDTrayID_Decoding(int Channel);
    bool    CCDTrayMap_Decoding(int Channel);
    int     iTrayMappingDate[35][70];
    int     iTrayDeviceCountDate[3][35][70];                                    //Sam 20190405 : Tray Decive Count
    int     iDeviceRemainCount;                                                 //Sam 20191115 : 殘留 IC 數量

    AnsiString sTrayIDDate;
    AnsiString sTrayIDDate2;
    bool DoTrayIDCCD();
    bool DoTrayID2CCD();
    void InitialTrayIDTask();
    bool DoTrayMapCCD(int iTrayType);                                           //Sam 20200323 : Modify Tray Function

    int iSFCCurrentStep         [iTotalFunction];                               //Sam 20190405 : Tray Decive Count
    bool bSFCStepError          [iTotalFunction];                               //Sam 20190405 : Tray Decive Count

    bool    iSFCCheckResult     [iTotalFunction];                               //Sam 20190405 : Tray Decive Count
    bool    bTrayError          [iTotalFunction];                               //Sam 20190405 : Tray Decive Count
    void    TrayMapSetDefaultView();
    void    TrayMapSetDefault(TStringGrid *tsGrid);                             //JimmyChiu 20220219 : 整理程式碼
    bool    bStepError          [iTotalFunction];                               //Sam 20190405 : Tray Decive Count
    int     iCurrentStep        [iTotalFunction];                               //Sam 20190405 : Tray Decive Count

    TQPF_Timer BarcodePosDelay      [iTotalFunction];                           //Sam 20190405 : Tray Decive Count                //wei 20151126 BarCode Pos Delay
    TQPF_Timer TMAResultDelay       [iTotalFunction];                           //Sam 20190405 : Tray Decive Count
    TQPF_Timer TMAStartDelay        [iTotalFunction];                           //Sam 20190405 : Tray Decive Count
    TQPF_Timer TMAExposureDelay     [iTotalFunction];                           //Sam 20190405 : Tray Decive Count

    TQPF_Timer TMMultileTrayIDDelay;
    void __fastcall DoTrayMapAutoTuneFunction();

    bool fShow;
    bool DoTrayMapAutoTuneCCD(int iiTuneType,bool Reset=false);
   // bool DoTrayDeciveCntAutoTuneCCD(int Count ,int iAuto,bool Reset=false);   //Sam 20190405 : Tray Decive Count
    bool DoReadTrayData(AnsiString FileName);
    int iComparUnloadIC(int iAuto);
    int iVisonAndHandlerComparLessCnt(int iAuto);                               //Sam 20200323 : Modify Tray Function
    int iVisionICCnt(int iAuto);                                                //Sam 20200323 : Modify Tray Function
    void WriteTrayMapLog(int index, AnsiString Message);
    void WritePickMapLog();
    int iFunction;
    void __fastcall AutoConnectNetwork();
    void __fastcall SearchTrayMapFile();
    void WriteUnloadMapLog(int iWhichAuto, bool bTray=false);
    int     iTrayMappingDateCheck[30][70];
    int     iDeviceRemainData[30][70];
    bool bUseTrayMap;
    bool bUseDeviceRemain;
    bool bUseDeviceCount;
    bool bUseTrayID;
    bool bUseTrayMapLaser;                                                      //JimmyChiu 20220219 : 殘料檢查 From Loader
    int ShowTrayFunErrMsg(int iErr,int iTrayType);                              //Sam 20200323 : Modify Tray Function
    void SendKeyenceCommand(int index, AnsiString Msg, AnsiString Msg2="");
    void AddKeyenceCommunicationLog(int index, AnsiString Str);                 //Steven 20160421 : Add 2D CCD Log
    bool bEnableKeyenceChannelConnect[iKeyenceTotalFunction];
    bool DoTrayIDKeyence();
    bool DoTrayIDKeyence2(int iFunction);
    bool DoCoverTrayIDKeyence(int iPos, bool bAlarm=false);
    bool DoCoverTrayID(int iFunction, bool bAlarm=false);

    bool DoTrayIDKeyence4();
    bool bKeyenceResultOK[iKeyenceTotalFunction];
    bool DoMultileTrayIDKeyence3();
    //JimmyChiu 20220219 : 殘料檢查 From Loader
    //<==
    TfAOI* GetAOI(int iIndex);
    TfAOI* GetAOI(AnsiString asName);
    bool bDoTrayDeviceCheckFromLoader;
    bool bSimulationAOICommand;
    TQPF_Timer DCCycleDelay;
    AnsiString asErrorLaserScan;
    void RecordMsgLaser(AnsiString asMsg);
    void RecordErrorMsgLaser(AnsiString asMsg);
    void SetSimulationState(bool bswitch=false);
    bool GetSimulationState(){return bSimulate;}
    bool MoveTrayArmToSafeColor();
    bool DoLaserScanFromLoader(int iAOIType,bool bIsFirst,bool bIsRunMoveOut=false);
    enum eAOI_Type
    {
        eAOIType_TrayDeviceCheckLaser=0,
        eAOIType_TrayMapLaser,
    };
    TfAOI* GetNowAOI(); 
    void CloseAllSocket();
    int iLaserScanTask;
    int iLoopCount;
    int iLoopNow;
    int iAutoRetryCount;
    bool IsRunMotor();
    bool IsCommAOI();
    bool IsRunTrayMapLaser();
    bool IsRunDeviceRemainLaser();
    int  GetLoaderCWSpeed(){return iLoaderYCWSpeedPerc;}
    int iLdYSpdBackup;
    void InitialcbLaserScanMode();
    void ResetcbLaserScanMode();
    void InitialAOIGroup();
    void ShowLaserScanSheet(TTabSheet *tsShow);
    void ReleaseLoaderBreak(int iAOIType,bool bswitch=true);
    bool GetTrayDeviceCheckIsRemainIC();
    bool DoGetValueAfterMotorArrival(int iMode,bool bIsFirst,AnsiString &asError);
    bool GetTrayMapHasNullIC();
    bool DoSettingBeforeMotorMove(int iMode,bool bIsFirst,AnsiString &asError);
    void SetYieldDatas(AnsiString sData);
    cLineScanRemainICYieldRecord yieldRemainIC;
    void InitialTrayDeviceYield();
    void SetTtileTrayDeviceYield(int icol,int irow,AnsiString sValue);
    void SetValueTrayDeviceYield(int icol,int irow,AnsiString sValue);
    bool CheckRunAutoRetry(int iAOIType);
    bool DoMoveOut(int iAOIType,bool bIsFirst);
    bool DoMoveIn(int iAOIType,bool bIsFirst);
    void ReadFile_ScanLine();
    void ReadFile_ScanLine(AnsiString szDir,int iAoi,AnsiString sHead,bool bUpdateUI);
    void SaveFile_ScanLine();
    void SaveFile_ScanLine(AnsiString szDir,int iAoi,AnsiString sHead);
    bool DoAutoSkip(int iAOIType);
    bool DoRunAtInitialStart(int iAOIType);
    void SetRealRunInitialStart(int iAOIType,bool bSwitch);
    bool GetRealRunInitialStart(int iAOIType);
    void SearchPageCtrl(AnsiString sFilter);
    void ShowCheckDevicePage();
    void ShowCheckDeviceResult(AnsiString sValue);
    //<==
    //JimmyChiu 20220219 : 殘料檢查 From Loader

    TfRFID *ldRFID;                                                             //KevinCheng 20250830 : Add RFID
    int  iNFCTask;
};
//---------------------------------------------------------------------------
extern PACKAGE TfTrayMapping *fTrayMapping;
extern bool MoveOutArmToAutoSafe();
extern bool MoveInArm2XYToWait();
extern bool MoveOutArmXY_ToFix_Tray_Full(bool bMoveY);
extern void InitLoadTask();
extern void DoLoad();
extern int LoadTask;
extern int iWhichTrayDeciveCount();                                             //Sam 20190405 : Tray Decive Count
extern bool CheckNeedCCDTrayDeviceCount(int iAuto);                             //Sam 20190405 : Tray Decive Count
extern void SetNeedTrayDeciveCount(int iAuto);                                  //Sam 20190405 : Tray Decive Count
extern int  iCoverTrayIDTask[iKeyenceTotalFunction];
extern int  iReadCIDAction;
extern bool DoCoverTrayID_NFC(int iPos, bool bAlarm=false);
extern void InitialCoverTrayIDTask(int iPos);
//---------------------------------------------------------------------------
#endif
