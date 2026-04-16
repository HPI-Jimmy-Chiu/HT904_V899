//---------------------------------------------------------------------------
#ifndef uHGemEquipmentH
#define uHGemEquipmentH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <fcntl.h>
#include <stdio.h>
#include <dos.h>
#include <dir.h>
#include <io.h>
#include <fcntl.h>
#include <Grids.hpp>
#include <Dialogs.hpp>

//---------------------------------------------------------------------------
class TFixedCriticalSection : public TCriticalSection
{
    private:
    byte FDummy[95];
};
//---------------------------------------------------------------------------
class GemTimer
{
    private:
        DWORD   ulStartTicks;           // 開始時的TICK COUNT
        int     iTimeLen;               // 計時長度
    public:
        void TimerSet(int iTime);
        void TimerSetSecAndOn(double iTime);
        void TimerSetMSAndOn(int iTime);
        void TimerOn();
        bool TimerOff();
};
struct HTypeStruct
{
    unsigned char LIST_TYPE;
    unsigned char ASCII_TYPE;
    unsigned char JIS_TYPE;
    unsigned char BINARY_TYPE;
    unsigned char BOOLEAN_TYPE;
    unsigned char UINT_1_TYPE;
    unsigned char UINT_2_TYPE;
    unsigned char UINT_4_TYPE;
    unsigned char UINT_8_TYPE;
    unsigned char INT_1_TYPE;
    unsigned char INT_2_TYPE;
    unsigned char INT_4_TYPE;
    unsigned char INT_8_TYPE;
    unsigned char FT_4_TYPE;
    unsigned char FT_8_TYPE;
    unsigned char VCL_TYPE;

};

struct STypeStruct
{
    unsigned char Data_Message;
    unsigned char Select_req;
    unsigned char Select_rsp;
    unsigned char Deselect_req;
    unsigned char Deselect_rsp;
    unsigned char Linktest_req;
    unsigned char Linktest_rsp;
    unsigned char Reject_req;
    unsigned char Separate_req;
};

struct HSMS_Head_Struct
{
    unsigned short int DeviceID;
    unsigned char MessageID_S;
    unsigned char MessageID_F;
    unsigned W_Bit;
    unsigned char PType;
    unsigned char SType;
    unsigned int SystemByte;
};

//==============================================================================
//=                                                                            =
//==============================================================================
class THGem : public TForm
{
__published:    // IDE-managed Components
    TClientSocket *clientGem;
    TServerSocket *srvGem;

    TTimer *Timer1;

    TFileListBox *FileListBox1;
    TFileListBox *FileListBox2;

    TCheckListBox *CheckListBox1;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TTabSheet *TabSheet5;
    TTabSheet *TabSheet6;
    TPanel *palSheet0;
    TLabel *Label8;
    TComboBox *ComboBox1;
    TRadioGroup *OnLineOrOffLine;
    TRadioGroup *RemoteOrLocal;
    TPanel *palSheet1;
    TLabel *lblAddress;
    TLabel *lblPort;
    TLabel *lblT3TimeOut;
    TLabel *lblT5TimeOut;
    TLabel *lblT6TimeOut;
    TLabel *lblT7TimeOut;
    TLabel *lblT8TimeOut;
    TRadioGroup *rgRole;
    TEdit *edtIP;
    TEdit *edtPort;
    TEdit *edtT3TimeOut;
    TEdit *edtT5TimeOut;
    TEdit *edtT6TimeOut;
    TEdit *edtT7TimeOut;
    TEdit *edtT8TimeOut;
    TPanel *palSheet2;
    TCheckBox *GemCheckBoxAcceptHostOnlineRequest;
    TCheckBox *GemCheckBoxShowBinary;
    TCheckBox *GemCheckBoxShowHeadInformation;
    TCheckBox *GemCheckBoxUseExtendedAlarm;
    TCheckBox *chkAnnotatedEventReport;
    TCheckBox *chkMoreMessageAbortProcess;
    TPanel *palSheet3;
    TListBox *SFCodeResponseList;
    TStringGrid *strGrdAlarmOld;
    TPanel *palSheet4;
    TStringGrid *strGrdCEID;
    TStringGrid *stdGridReportID;
    TPanel *palSheet5;
    TStringGrid *strGrdAlarm;
    TTabSheet *tsSetup;
    TPanel *pSetup;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TEdit *edtRunTime;
    TEdit *edtAverageTestTime;
    TEdit *edtOEE_Yield;
    TCheckBox *ckAddDefaultReport;
    TTabSheet *tsSecsECData;
    TStringGrid *sgSECSECData;
    TDirectoryListBox *DirectoryListBox1;
    TTabSheet *tsPPBody;
    TMemo *memoPPBody;
    TFileListBox *FileListBox3;
    TLabel *lblDeviceID;
    TEdit *edDeviceID;
    TCheckBox *cbECChaneEventReport;
    TSaveDialog *SaveDialog1;
    TButton *btnExport;

    void __fastcall clientGemConnect(TObject *Sender,TCustomWinSocket *Socket);
    void __fastcall clientGemDisconnect(TObject *Sender,TCustomWinSocket *Socket);
    void __fastcall clientGemRead(TObject *Sender,TCustomWinSocket *Socket);
    void __fastcall clientGemError(TObject *Sender,TCustomWinSocket *Socket, TErrorEvent ErrorEvent,int &ErrorCode);
    void __fastcall clientGemConnecting(TObject *Sender,TCustomWinSocket *Socket);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall srvGemClientConnect(TObject *Sender,TCustomWinSocket *Socket);
    void __fastcall srvGemClientError(TObject *Sender,TCustomWinSocket *Socket, TErrorEvent ErrorEvent,int &ErrorCode);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall srvGemClientDisconnect(TObject *Sender,TCustomWinSocket *Socket);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall edtPortClick(TObject *Sender);
    void __fastcall edtT3TimeOutClick(TObject *Sender);
    void __fastcall btnExportClick(TObject *Sender);
private:    // User declarations
//==============================================================================

        bool bSpooling;
        bool bAutoConnect;
        bool bTCPIP_Error;

        //   GEM SV data
        unsigned char  GemControlState;     // 4
        unsigned char  GemLinkState;        // 5
        char SECSCommunicationMode;         // 6

        unsigned char  GemControlPreState;  // 9

        int  GemSpoolCountActual;           // 53
        int  GemSpoolCountTotal;            // 54
        char GemSpoolStartTime[256];        // 57

        bool bBackupOnLineLocal;
        unsigned int EvenCount;

        unsigned LocalLength;
        unsigned LocalLength_4;
        unsigned char LocalBuffer[64*1024*1024];
        unsigned RemoteLength;

        unsigned Host_HSMS_RunLength;
        struct STypeStruct SType;

        unsigned int EquipmentSystemByte;
        unsigned int RemoteSystemByte;
        int iSMLSpace;
        AnsiString SpaceStr;
        bool bReceiveData;
        AnsiString TCP_Address;
        AnsiString TCP_Port;

        //16.04.01.01 Roy Change    bool bConnect;  //14.06.04.01 從私有成員改成公有成員歐.  marvin//
        int iStartConnectTask;
        bool bStartConnect;
        int  iEstablishCommunicationsTryCount;

        bool bOnLineLocal;
        int  iStartOnLineTask;
        bool bStartOnLine;

        TStringList *UploadFileName;
        TStringList *TimeLeft;
        Word SystemYear, SystemMonth, SystemDate;
        Word SystemHour, SystemMin,   SystemSec,SystemMSec;
        int iTimeFormatECID;
        int iTimeFormat;
        int EC69_UNT1_MaxTranslateLen;
        int iTimeFormatDefault;
        int EC69_UNT1_MaxTranslateLenDefault;
        TColor StringOutColor;

        TStringList *RequestRemoteDownLoad;
        bool bOutputBusy;
        bool bShow;
        TStringList *ALID_ALED;

//==============================================================================
        void __fastcall SaveSystemDefault();
        void __fastcall DoDownLoadRemoteFile();
        void __fastcall InitialHGem();
        void DoSelect();
        void DoSeparate();
        void ProcessRemoteHead(unsigned char *ProcessBuffer);
        void CreateLocalHead();
        void OnlineLocalOrRemote();
        int GetLengthOfType(unsigned char Type);
        void __fastcall ShowSML(unsigned char *Ptr,int len);
        int __fastcall ShowSMLBinary(unsigned char *Ptr,int len);
        int  __fastcall ProcessSML(unsigned char *Ptr,int Len,int &RunLength);
        int  __fastcall ProcessSMLBinary(unsigned char *Ptr,int Len,int &RunLength);
        void MakeSMLSpaceString();
        void ShowSMLSpaceString(AnsiString S);
        void ShowSMLSpaceBinaryString(AnsiString S);
        AnsiString ShowSFDescription(unsigned char SCode,unsigned char FCode);  //Steven 20180815 : 針對S6F12連收兩次的修正 回傳SFCode
        void InitHType();
        void ShowLocalHeadInfo();
        void ShowLocalBufferBinaryData();
        void InitSTypeStruct();
        unsigned char * HostConvertToData(unsigned char *ProcessBuffer,int len, void *Value);
        void ConvertLocalData(int len,void *Value);
        void __fastcall StoreToReceiveString(AnsiString S);
        unsigned char  GetLengthByte(unsigned  len,unsigned char *Ptr);
        int __fastcall GetSMLLenthByte(unsigned char TypeChar,unsigned char *Ptr,int RunLength );

        void ReplyECDataChange();
        void DoReportECDataChangeCheck();
//        int  CheckECValue(AnsiString ECID,void *PtrSour);
        void DoTraceDataResponse(int );
        void WriteToSpoolFile();
        int DoSpoolSendLocalData(unsigned char *Ptr);
        void DoSpool();
        void DoUploadFileToHost_ForSingleFile();
        void DoUploadFileToHost_ForMultiFile();
        void DoUploadFileToHost_ForDirectoryFile();

        void DoUploadFileToHost();

        void __fastcall DoProcessSFNoResponse();
        void __fastcall ManualCreatergRoleClick(TObject *Sender);

//##############################################################################
//      主要的 S,F 的處理
//##############################################################################

        void LinktestRsp();
        void SelectRsp();
        void DeselectRsp();
        void ProcessReceiceData();
public:     // User declarations
        __fastcall THGem(TComponent* Owner);
        __fastcall ~THGem();                                                    //16.09.14.00 Roy Add

        TMemo *DB;
        //   GEM SV data
        AnsiString GemClock;                 // 3
        int iMinTimeFormat;
        int iMaxTimeFormat;

        AnsiString GemMDLN;                  // 24
        AnsiString GemSOFTREV;               // 25

        bool bConnect;                      //16.04.01.01 Roy Change

        int  DoReportECChange(int iIndex);
        void SetDisplayPtr(TMemo *DispBox);
        void MyTestType();
        void InitLocalHead(int SCode, int FCode,int WBit);
        void DataItemOut(int len, unsigned char Type, void *P);
        void DataItemOut(unsigned char Type, AnsiString S);
        int DataItemInSub(int len, unsigned char Type, void *P);
        void SendLocalData();
        int (*MoveCheckCallBack)();
        void Connect();
        void DisConnect();
        bool IsConnect();
        void SetEstablishCommunicationsTryCount(int);
        void OnLine(bool Mode);
        void OffLine();
        bool IsOnLine();
        bool GetOnLineMode();
        void OnLineLocal();
        void OnLineRemote();
        AnsiString GetECDataValue(AnsiString ECID);

//##############################################################################
//      主要的 S,F 的處理用到的輔助函數
//##############################################################################
        bool CheckSFFormatOnlyHead(AnsiString ErrStr);

        void SetSVDataPointer(AnsiString SVID,unsigned char Type, AnsiString SVName, AnsiString SVUnit, void *Ptr,      AnsiString Remark);
        void SetSVDataPointer(AnsiString SVID,unsigned char Type, AnsiString SVName, AnsiString SVUnit, AnsiString *P,  AnsiString Remark);
        void SetSVDataPointer(AnsiString SVID,unsigned char Type, AnsiString SVName, AnsiString SVUnit, void *Ptr,int,  AnsiString Remark);
        void SetSVDataPointer(AnsiString SVID,unsigned char Type, AnsiString SVName, AnsiString SVUnit, TObject *P,     AnsiString Remark);
        void SetCanAcceptHostOnLineRequest(bool);
        void SetECDataPointer(AnsiString ECID,unsigned char Type, AnsiString ECName, AnsiString ECUnit, void *P,        int Min,            int Max,            int Default,            AnsiString Remark);
        void SetECDataPointer(AnsiString ECID,unsigned char Type, AnsiString ECName, AnsiString ECUnit, void *Ptr,      AnsiString Min,     AnsiString Max,     AnsiString Default,     AnsiString Remark);
        void SetECDataPointer(AnsiString ECID,unsigned char Type, AnsiString ECName, AnsiString ECUnit, AnsiString *P,  AnsiString MinValue, AnsiString MaxValue, AnsiString DefaultValue, AnsiString Remark);
        void SetECDataPointer(AnsiString ECID,unsigned char Type, AnsiString ECName, AnsiString ECUnit, TObject *P,     char *MinValue,      char *MaxValue,      char *DefaultValue,      AnsiString Remark);
        void SetTimeFormat(int Format); // 0:12-bytes, 1:16-bytes, 2:14-bytes, 3:ISO8601 format
        void SetTerminalWindows(TObject *Ptr);
        void SetTerminalWindows2(TObject *Ptr);
        void TerminalRequest(AnsiString S);
        void SetSpoolActive(bool);
        bool GetSpoolActive();
        unsigned GetGemSpoolCountActual(){return GemSpoolCountActual;};
        unsigned GetGemSpoolCountTotal(){return GemSpoolCountTotal;};
        void __fastcall SetAlamData(int iRowCount, AnsiString ALID, AnsiString Class, AnsiString ALTX, AnsiString Position);
        void __fastcall ReportAlarm(AnsiString AlarmCode, bool bIsJam, int iDuplicateError, AnsiString SubMessage, bool bReleaseAlm=false);     //JerryYang (Steven) 20170504 bReleaseAlm為true表示解除alarm需上報0x00
        bool __fastcall CheckNeedReportAlarm(AnsiString S);
        void __fastcall ReportAlarmWithMessage(AnsiString ALID,AnsiString ALTX,AnsiString sHappenedTime,AnsiString sProcessKey,bool SameCase);
        int THGem::SetReceipeDirectoryAndGlobalName(AnsiString Path,AnsiString FileMask,int Type);
        void LocalAcknowledge(unsigned char S,unsigned char F ,unsigned char Command);
        void DoLocalAllProcessLoop();
        bool DoOpenCommuncation();
        int  DoConnect();
        void DoUpdateStatus();
        bool DoOnLine();
        void __fastcall GemBtnSendTerminalMessageClick(TObject *Sender);
        void __fastcall GemTerminalSendEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
//        void OpenCommuncation(AnsiString Address, int Port);
//        void OpenCommuncation(int Port);
        void CloseCommuncation();
        void __fastcall BtnEnableCommClick(TObject *Sender);
        void __fastcall BtnDisableCommClick(TObject *Sender);
        void __fastcall GemBtnOnlineRequestClick(TObject *Sender);
        void __fastcall GemBtnOfflineRequestClick(TObject *Sender);
        void __fastcall GemBtnOnlineRemoteClick(TObject *Sender);
        void __fastcall GemBtnOnlineLocalClick(TObject *Sender);
        void __fastcall GemListRemoteFileNameClick(TObject *Sender);
        void __fastcall GemDownLoadRemoteFileNameClick(TObject *Sender);
        void __fastcall GemDeleteRemoteFileNameClick(TObject *Sender);
        void __fastcall GemUpLoadLocalFileNameClick(TObject *Sender);

        void __fastcall GemRefreshLocalFileClick(TObject *Sender);

        void __fastcall GemSelectAllRemoteFileClick(TObject *Sender);
        void __fastcall GemDisSelectAllRemoteFileClick(TObject *Sender);
        void __fastcall GemSelectAllLocalFileClick(TObject *Sender);
        void __fastcall GemDisSelectAllLocalFileClick(TObject *Sender);
        //void __fastcall GemDeleteRemoteFileNameClick(TObject *Sender);
        void __fastcall SetMachineTypeAndSoftwarseVer(AnsiString Mdln,AnsiString SoftVer);
        //----------------------------------------------------------------------
        bool bWaitSelectRsp;
        bool bWaitDeSelectRsp;
        bool bAreYouThereRequest;
        bool bS1F2_OnLineData;
        bool bSeprate;
        unsigned int DeviceID;
        struct HSMS_Head_Struct Local,Remote;
        bool bOpenCommuncation;
        bool bCloseCommuncation;
        bool bReceiveEstablishCommunicationsRequest;
        TPanel *SECSConnectionState;
        TPanel *GEMCommunicatingState;
        TPanel *GemPanelControlState;
        TEdit  *GemTerminalSendEdit;
        TSpeedButton *GemBtnSendTerminalMessage;
        TSpeedButton *BtnEnableComm;
        TSpeedButton *BtnDisableComm;
        TSpeedButton *GemBtnOnlineRequest;
        TSpeedButton *GemBtnOfflineRequest;
        TSpeedButton *GemBtnOnlineRemote;
        TSpeedButton *GemBtnOnlineLocal;
        TSpeedButton *GemListRemoteFileName;
        bool bDisableBinaryShow;
        TRadioGroup *EnableOrDisablePtr;
        TCheckListBox *GemRemoteReceipeList;
        TSpeedButton *GemDownLoadRemoteFileName;
        TSpeedButton *GemDeleteRemoteFileName;
        TSpeedButton *GemSelectAllRemoteFile;
        TSpeedButton *GemDisSelectAllRemoteFile;
        TCheckListBox *GemLocalFileLixtBox;
        TSpeedButton *GemRefreshLocalFile;
        TSpeedButton *GemUpLoadLocalFileName;
        TSpeedButton *GemSelectAllLocalFile;
        TSpeedButton *GemDisSelectAllLocalFile;

        bool CheckSocketActiveFalse();
        void SetCurrentDirectory(AnsiString Path);
        AnsiString CurrentDirectory;
        AnsiString GemSystemPath;
        AnsiString GemSystemIniPath;
        AnsiString GemSpoolPath;

        void __fastcall GemSBSetupClick(TObject *Sender);
        AnsiString __fastcall ReadALED(AnsiString ALID);
        void __fastcall WriteALED(AnsiString ALID,AnsiString S);
        int GetDataItemLenAndTypeSub(int &len,unsigned char &Type);
        int GetDataItemLenAndTypeAndDeleteSub(int &len,unsigned char &Type);
        void __fastcall StringOut(AnsiString S);
        void __fastcall StringBinaryOut(AnsiString S);
        void __fastcall StringOut(AnsiString S,TColor C);
        bool bCommandLengthError;
        int iPackageLenErrCnt;

        void __fastcall SaveTCPIPRecieveData(unsigned char *DataBuf, int iLen); //20130409 Daver add
        void __fastcall SaveSECSGEMTextToLog();                                 //20130409 Daver add
        AnsiString __fastcall GetSocketErrorMsg(TObject *Sender,int iErrCode);  //20130409 Daver add
        void __fastcall SaveSECSGEMErrToLog(AnsiString asSaveStr);              //20130409 Daver add
        TStringList *LogDataString;
        int GetDataItemLenAndTypeAndDelete(int &len, unsigned char &Type);
        int DataItemIn(int len, unsigned char Type, void *P);
        int DataItemIn(int len, unsigned char Type, AnsiString &P);
        int DataItemInNew(AnsiString &P);
        int GetDataItemLenAndType(int &len, unsigned char &Type);
        int iReturnCode;
        bool bFirstEntry;
        bool bReportSECS_GEM_Message;

        bool bServoSocketConnect;
        char *DownLoadFilePtr;
        bool bReceiveS7F6;
        bool bReceiveS101F5;
        bool bReceiveS101F6;
        bool bReceiveS101F7;
        bool bReceiveS101F8;
        bool bUseClientSocket;
        bool fALID_ALED_Exist;
        unsigned char *MulitBuffer;
        int iTotalBlockLength;
        int iCurrentBlockLength;
        bool bALEDflag;
        int ctSpoolFile;

        bool bFirstBlock;
        unsigned char *EthernetBuffer;
        int iEthernetBufferLen;
        int iOpenCommuncationTask;
        GemTimer DelayOpenCommuncation;
        int countConnect;
        int iConnectTryCount;
        GemTimer ConnectDelay;
        Word iReportECDataChangeCheckOldSecond;
        int iSpoolTask;
        unsigned char *SpoolPtr,*SpoolRunPtr;
        WORD OldSpoolSystemMin;
        GemTimer SpoolDelay;
        GemTimer TraceDataResponseDelay[10];
        int iUploadFileToHost_ForSingleFile;
        long UploadFileToHost_ForSingleFileTotalFileSize;
        FILE *UploadFileToHost_ForSingleFileFilePtr;
        int iUploadFileToHost_ForSingleFileStoreCT,iUploadFileToHost_ForSingleFileTotalCount;
        int iUploadFileToHost_ForSingleFileMaxSend;
        GemTimer UploadFileToHost_ForMultiFileDelay;
        int iUploadFileToHost_ForMultiFileTask;
        int iUploadFileToHost_ForMultiFileCT;
        long UploadFileToHost_ForMultiFileTotalFileSize;
        FILE *PFileUploadFileToHost_ForMultiFile;
        AnsiString UploadFileToHost_ForMultiFileMultiFile;
        int iStoreCTUploadFileToHost_ForMultiFile;
        int iTotalCountUploadFileToHost_ForMultiFile;
        int iUploadFileToHost_ForDirectoryFileTask;
        int iCTUploadFileToHost_ForDirectoryFile;
        long TotalFileSizeUploadFileToHost_ForDirectoryFile;
        FILE *FilePUploadFileToHost_ForDirectoryFile;
        AnsiString PathNameUploadFileToHost_ForDirectoryFile;
        AnsiString MultiFileUploadFileToHost_ForDirectoryFile;
        AnsiString FileNameUploadFileToHost_ForDirectoryFile;
        int iStoreCTUploadFileToHost_ForDirectoryFile;
        int iTotalCountUploadFileToHost_ForDirectoryFile;
        WORD iOldSecProcessSFNoResponse;
        int iAutoConnectSec,iAutoConnectDelay;
        int ctUpdateStatus;
        bool bClientSocketActive;
        bool bOldConnect;
        bool flag1UpdateStatus;
        bool flag2UpdateStatus;
        bool flag3UpdateStatus;
        AnsiString OldSUpdateStatus;
        int iServoConnectCT;
        bool bConnectUpdateStatus;
        int Timer1Task,Timer1ct;
        int iRetryCTDownLoadRemoteFile;
        GemTimer DelayDownLoadRemoteFile;
        int iDownLoadRemoteFileTask;
        char PtrUploadFileToHost_ForSingleFile[256*256*256];
        AnsiString DefaultAddress;
        AnsiString DefaultPort;
        AnsiString DefaultDeviceID;
        unsigned char MHEAD[10];
        bool bDataFormatOK;
        AnsiString Alias;
        TStringList *slTempReportID;
        TList       *lTempReportIDContent;
        TStringList *slTempCeID;
        TList       *lTempCeIDContent;

        void SendInvalidDataMessageToHost(AnsiString S);

        bool IsValidSVID(AnsiString SVID);
        void SetDefaultAddressAndPort(AnsiString Address, AnsiString Port, AnsiString DeviceID);
        void __fastcall CheckSFCodeResponse();
        bool CheckSFFormatDataRequest(AnsiString ErrString);
        void __fastcall SetCEIDContent(unsigned iCeid, unsigned iReportCount, unsigned *iReportIDData, int Mode);
        void __fastcall SetCEIDContent(unsigned iCeid, AnsiString CeidAlias, unsigned iReportCount, unsigned *iReportIDData, int Mode);   //2014/01/01  lee
        unsigned __fastcall GetCEIDContent(unsigned iCeid, unsigned *iReportIDData, int Mode);
        bool __fastcall SetReportIDContent(unsigned iCeid, unsigned iReportCount, unsigned *iReportIDData, int Mode);
        bool __fastcall DeleteReportID(unsigned iCeid, int Type);                       //2013/11/20 lee
        bool __fastcall DeleteReportIDOfCeid(unsigned iCeid);
        unsigned __fastcall GetReportIDContent(unsigned iCeid,unsigned *iReportIDData,int Mode);
        void __fastcall SendRepoerID(unsigned iReportID);
        void __fastcall SendAnnotatedRepoerID(unsigned iReportID);
        void __fastcall SendCeid(unsigned iCeid);
        void __fastcall SendAnnotatedCeid(unsigned iCeid);
        void __fastcall EventReport(unsigned iDataID,unsigned iCeid);
        void __fastcall ProcessHostSendReportID();
        void __fastcall ReportAcknowledge(unsigned char Code);
        void __fastcall DefineReportAcknowledgeAccept();
        void __fastcall DefineReportAcknowledgeInsufficientSpace();
        void __fastcall DefineReportAcknowledgeFormatError();
        void __fastcall DefineReportAcknowledgeAlreadyDefined();
        void __fastcall DefineReportAcknowledgeInvalidSVID();
        bool __fastcall DeleteAllHostDefineReportID();
        bool __fastcall CheckReportAlreadyDefine(AnsiString ReportID);
        void __fastcall ProcessHostSendReportLinkID();
        void __fastcall ReportLinkAcknowledgeError(unsigned char ErrCode);
        void __fastcall LinkReportAcknowledgeFormatError();
        void __fastcall LinkReportAcknowledgeAlreadyDefined();
        void __fastcall LinkReportAcknowledgeInvalidCeID();
        void __fastcall LinkReportAcknowledgeInvalidReportID();
        bool __fastcall CheckCeidExist( AnsiString Ceid);
        bool __fastcall CheckCeidAlreadyDefine(AnsiString Ceid);
        int __fastcall  GetReportIDType(AnsiString  ReportID);
        bool __fastcall CheckReportIDExist( AnsiString ReportID );
        void __fastcall AddCeidReportID(AnsiString Ceid,AnsiString ReportID);
        void __fastcall DeleteAllHostDefineCeid();
        void __fastcall DeleteAllHostReportIDOfCeid( unsigned ReportID );
        void __fastcall DeleteHostReportIDOfAssignCeid( AnsiString Ceid);
        void __fastcall EnableDisableEventReportAcknowledgeError(unsigned char ErrCode);
        void __fastcall EnableDisableEventReportAcknowledgeCeidNotExist();
        void __fastcall EnableDisableEventReportAcknowledgeFormatError();
        bool __fastcall CheckCEIDExist(AnsiString sID);
        void __fastcall EnableDisableEventReport(bool CEED,int slen,unsigned *CEID);
        bool __fastcall IsEnableEvent( unsigned iDataID,unsigned iCeid);
        void __fastcall CopyStringGridAsTabFormat(TStringGrid *strGrd,TStringList *memoPtr);
        void __fastcall PasteStringGridAsTabFormat(TStringGrid *strGrd,TStringList *memoPtr);
        void __fastcall ReadEventReportData();
        void __fastcall SaveEventReportData();
        void __fastcall GetAllSVInformation(TMemo *Ptr);
        //void __fastcall GetAllECInformation(TMemo *Ptr);
        void __fastcall GetECInformation(TObject * Ptr,int Index);              //Steven 20130914 : TMemo --> TObject * Ptr
        void __fastcall ReadAlamData();
        void __fastcall WriteAlamData();
        bool bReceiveMultiConnect;
        GemTimer DelayForServoError;

        TList       *SocketReceiveList;
        TStringList *SocketReceiveLength;
        void __fastcall ProcessSocketReceiveData();
        TStringList *WaitShowString;
        void __fastcall ProcessShow();
        void __fastcall GetALLECInformation(TObject * Ptr);

        //2013/09/16 lee start
        long lCPUFreq;
        char szManID[256];
        char szGetCPUType[256];
        //2013/09/16 lee end

        //2013/09/30  lee start
        int Disk_C_TotalSpaceMB;
        int Disk_D_TotalSpaceMB;
        int Disk_E_TotalSpaceMB;
        int Disk_C_TotalFreeSpaceMB;
        int Disk_D_TotalFreeSpaceMB;
        int Disk_E_TotalFreeSpaceMB;
        int LastGetDiskInfoMin;
        unsigned long ulMemoryLoad;
        unsigned long ulTotalPhys;
        unsigned long ulAvailPhys;
        int LastGetMemoryStatus;
        //2013/09/30  lee end
        void __fastcall GemRemoteReceipeListClick(TObject *Sender);             //2013/12/01 lee
        void __fastcall ClearDefaultEvenReport();                               //2013/11/20  lee
        AnsiString ForMultiFileFileName;                                        //2014/02/01 lee   // ASEM

        //pig 2014.04.01 ASEM SECS start
        bool bReceiveS110F2;
        bool bFinishDownloadFile;
        TStringList *SECSReportIDChange;                                        //pig 2014.04.23 KYEC SECS
        TStringList *SECSOriginalValue;                                         //pig 2014.04.23 KYEC SECS
        TStringList *SECSNewECValue;                                            //pig 2014.04.23 KYEC SECS

        //pig 2014.07.28 KYEC_SECS start
        int T3TimeOut;
        int T5TimeOut;
        int T6TimeOut;
        int T7TimeOut;
        int T8TimeOut;
        //pig 2014.07.28 KYEC_SECS end
        //pig 2014.08.27 start
        bool __fastcall ReadECEnableData();
        void __fastcall WriteECEnableData();
        bool EnableDisableECData(AnsiString ID,unsigned char T);
        void EnableDisableECDataAll(unsigned char T);
        bool __fastcall GetECEnableData(AnsiString ID);
        void __fastcall SetECEnableData(AnsiString ECID,AnsiString Function);
        //pig 2014.08.27 end

        TStringList *SV_ID;
        TStringList *SV_TYPE;
        TStringList *SV_NAME;
        TStringList *SV_UNIT;
        TList       *SV_Ptr;
        TStringList *VCL_NAME;
        TStringList *SV_LEN;
        TStringList *SV_Remark;

        TStringList *EC_ID;
        TStringList *EC_TYPE;
        TStringList *EC_NAME;
        TStringList *EC_UNIT;
        TList       *EC_Ptr;
        TList       *EC_Ptr_Min;
        TList       *EC_Ptr_Max;
        TList       *EC_Ptr_Default;
        TStringList *EC_Remark;

        TStringList *EC_Ptr_Min_Value;
        TStringList *EC_Ptr_Max_Value;
        TStringList *EC_Ptr_Default_Value;
        TStringList *EC_VCL_NAME;
        TStringList *EC_OldValue;

        TStringList *SReceiveData;
        TStringList *SReceiveDataBackup;
        TStringList *TraceData[10];
        TStringList *UploadFileString;

        TMemo    *TerminalMemoPtr;
        TListBox  *TerminalListboxPtr;
        TCustomEdit    *TerminalEditPtr;
        TPanel   *TerminalPanelPtr;
        int      TerminalDisplayIndex;
        TMemo    *TerminalMemoPtr2;
        TListBox  *TerminalListboxPtr2;
        TCustomEdit    *TerminalEditPtr2;
        TPanel   *TerminalPanelPtr2;
        int      TerminalDisplayIndex2;

        bool DataItemOutSV(AnsiString SVID);
        bool DataItemOutSVNameList(AnsiString SVID);
        bool DataItemOutSVNameListWithValue(AnsiString SVID);
        bool bWaitEstablishCommunicationsResponse;
        bool bWaitEstablishCommunicationsResponseError;
        bool bOnLine;
        bool bSpoolActive;
        bool bBeginTransferSpool;
        bool bTraceData[10];

        AnsiString UpLoadPath;
        AnsiString iTRID[10];
        AnsiString TimeString;
        AnsiString SV_71_ASCII_FilenameExtened;
        unsigned DSPER[10];
        unsigned iTOTSMP[10];
        unsigned iREPGSZ[10];
        unsigned iTOTSMP_Count[10];
        int TraceDataResponseTask[10];
        int SV_70_UNT1_ReceipeStruct;

        void DataItemOutEC(AnsiString ECID);
        void DataItemOutECNameList(AnsiString ECID);

        void GetTimeInfo();
        bool EnableDisableAlarm(AnsiString S,unsigned char T);
        void EnableDisableAlarmAll(unsigned char T);
        int GetAlarmIndex(AnsiString S);

        TFixedCriticalSection *pLockOnSocketRecvice;                            //16.10.05.00 Roy Add (Debug from kirin)
        TMemoryStream *RecvMemoryBuffer;
        TMemoryStream *ProcBuffer;
        TMemoryStream *TempProcBuffer;
        int iFileCount;

        TCriticalSection *csSFCodeResponse;                                     // 20221111 Joseph (Jason) 新增 TCriticalSection //
};
//---------------------------------------------------------------------------
extern PACKAGE THGem *HGem;
//extern PACKAGE THGem *HGem2;
//---------------------------------------------------------------------------
extern struct HTypeStruct HType;
#endif
