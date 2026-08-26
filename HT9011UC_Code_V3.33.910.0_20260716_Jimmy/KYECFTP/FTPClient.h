//---------------------------------------------------------------------------

#ifndef FTPClientH
#define FTPClientH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <NMFtp.hpp>
#include <Psock.hpp>
#include <StdCtrls.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TfFTPClient : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TGroupBox *GroupBox1;
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TPanel *Panel6;
    TPanel *Panel7;
    TEdit *edtMachineName;
    TEdit *edtHostName;
    TEdit *edtMachineIPAddress0;
    TEdit *edtSubNetMask0;
    TEdit *edtHostIPAddress0;
    TEdit *edtGatewayAddress0;
    TEdit *edtEthernetAddress;
    TLabel *Label1;
    TGroupBox *GroupBox2;
    TPanel *Panel8;
    TPanel *Panel9;
    TPanel *Panel10;
    TPanel *Panel11;
    TPanel *Panel12;
    TEdit *edtUserName;
    TEdit *edtPassword;
    TEdit *edtDownload;
    TEdit *edtUpload;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TGroupBox *GroupBox3;
    TPanel *Panel13;
    TEdit *edtServerWaferName;
    TListBox *lstServerFile;
    TGroupBox *GroupBox4;
    TGroupBox *GroupBox5;
    TPanel *Panel14;
    TEdit *edtHDWaferName;
    TListBox *lstHDFile;
    TGroupBox *GroupBox6;
    TListBox *ListBox1;
    TPanel *plLoad;
    TPanel *plUnload;
    TPanel *plSLoad;
    TProgressBar *ProgressBar1;
    TStatusBar *sbFTPStatus;
    TTabSheet *TabSheet4;
    TGroupBox *GroupBox7;
    TEdit *edHandlerType;
    TEdit *edHandlerID;
    TLabel *Label2;
    TLabel *Label3;
    TGroupBox *GroupBox8;
    TButton *btSafeTasterName;
    TButton *Button3;
    TMemo *memoFTP;
    TRadioGroup *rgInputMethod;
    TGroupBox *grpTesterMap;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label7;
    TComboBox *cbTesterType;
    TComboBox *cbTesterID;
    TComboBox *cbTasterIp;
    TGroupBox *grpTesterName;
    TEdit *edTesterName;
    TNMFTP *NMFTP1;
    TFileListBox *FileListBox1;
    TLabel *labN06_DownloadPath;
    TEdit *FTP_DownPath;
    TEdit *FTP_UpLdPath;
    TLabel *labN06_UploadPath;
    TLabel *labN06_DownloadPath1;
    TEdit *FTP_DownPath1;
    TLabel *labN06_UploadPath1;
    TEdit *FTP_UpLdPath1;
    TPanel *Panel15;
    TPanel *Panel16;
    TPanel *plUnloadALL;

    void __fastcall plSLoadClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall plUnloadClick(TObject *Sender);
    void __fastcall plLoadClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall cbTesterTypeChange(TObject *Sender);
    void __fastcall btSafeTasterNameClick(TObject *Sender);
    void __fastcall cbTesterIDChange(TObject *Sender);
    void __fastcall NMFTP1ListItem(AnsiString Listing);
    void __fastcall NMFTP1Success(TCmdType Trans_Type);
    void __fastcall NMFTP1TransactionStop(TObject *Sender);
    void __fastcall NMFTP1TransactionStart(TObject *Sender);
    void __fastcall NMFTP1ConnectionFailed(TObject *Sender);
    void __fastcall NMFTP1UnSupportedFunction(TCmdType Trans_Type);
    void __fastcall NMFTP1Failure(bool &Handled, TCmdType Trans_Type);
    void __fastcall NMFTP1AuthenticationFailed(bool &Handled);
    void __fastcall NMFTP1Error(TComponent *Sender, WORD Errno, AnsiString Errmsg);
    void __fastcall NMFTP1Status(TComponent *Sender, AnsiString Status);

    void __fastcall memoFTPDblClick(TObject *Sender);
    void __fastcall rgInputMethodClick(TObject *Sender);
    void __fastcall FilterList(TObject *Sender); //Landam
    void __fastcall edtHDWaferNameChange(TObject *Sender);
    void __fastcall edtServerWaferNameChange(TObject *Sender);
    void __fastcall lstServerFileDblClick(TObject *Sender);
    void __fastcall lstHDFileDblClick(TObject *Sender);
    void __fastcall edtServerWaferNameKeyPress(TObject *Sender, char &Key);
    void __fastcall edtHDWaferNameKeyPress(TObject *Sender, char &Key);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall Panel15Click(TObject *Sender);
    void __fastcall Panel16Click(TObject *Sender);
    void __fastcall plUnloadALLClick(TObject *Sender);
    void __fastcall edtHDWaferNameKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edtHDWaferNameMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edtServerWaferNameKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edtServerWaferNameMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations
    long BytesToTransfer ;
    TDateTime STime;
    double AverageSpeed;
    AnsiString sRootPath;
    bool enter;
    bool bListOk;
    int iHD;
    void __fastcall GetTesterType();                                            //Steven 20121018 : 取消不用MDB, 改用直接讀取文字檔
    bool __fastcall DoPassword();                                               //JerryYang 20151028 add for Amkor
public:     // User declarations
    bool bShow;
    bool bControlBySECSGEM;                                                     //ChungHung 20150515 add Control by SECSGEM
    int  iErrorBySECSGEM;                                                       //ChungHung 20150515 add Control by SECSGEM
    bool bError;                                                                //KaiChen 20190530 ：改為public
    AnsiString aSetUpNameBySECSGEM;                                             //ChungHung 20150515 add Control by SECSGEM
    bool bControlByGPIB;                                                        //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
    int  iErrorByGPIB;                                                          //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
    AnsiString asSetUpNameByGPIB;                                               //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
    bool bPIDTransferErr;                                                       //Eastsun 20260522 整合: PID Transfer Error flag

    void __fastcall NMFTP2Success(TCmdType Trans_Type);                         //Steven 20220510 : 針對FTP動作做紀錄
    void __fastcall NMFTP2TransactionStop(TObject *Sender);
    void __fastcall NMFTP2TransactionStart(TObject *Sender);
    void __fastcall NMFTP2ConnectionFailed(TObject *Sender);
    void __fastcall NMFTP2UnSupportedFunction(TCmdType Trans_Type);
    void __fastcall NMFTP2Failure(bool &Handled, TCmdType Trans_Type);
    void __fastcall NMFTP2AuthenticationFailed(bool &Handled);
    void __fastcall NMFTP2Error(TComponent *Sender, WORD Errno, AnsiString Errmsg);
    void __fastcall NMFTP2Status(TComponent *Sender, AnsiString Status);
    void __fastcall NMFTP2Connect(TObject *Sender);
    void __fastcall NMFTP2Disconnect(TObject *Sender);

    bool bIsFtpRunning;                                                         //Sam 20210120 : Auto Upload FTP Temperature Info 功能
    __fastcall TfFTPClient(TComponent* Owner);
    bool TransferrignData;
    void __fastcall LoadFileFormServer2(AnsiString FtpPath, AnsiString Source);
    void __fastcall UploadFileToServer2(AnsiString FtpPath, AnsiString Source, bool bZip=false); //ChungHung 20140108 add FTP unload jam code
    void __fastcall UpSocketIdPoductDataToServerByFTP(AnsiString asDirPath, AnsiString sFileName); //Sam 20170525 (wei) 力成 add Socket ID Product Data Upload To FTP
    void __fastcall ShowFTPModal(int HD);
    AnsiString __fastcall SaveJamCodeFile(AnsiString aHandlerID, TDateTime tDate, AnsiString aJamCode, AnsiString aMessage);   //ChungHung 20140108 add FTP unload jam code

    bool bDownLoadFile;
    void DoUpdateSetupFileData(AnsiString FileName);                            //Steven 20181224 : For ASE-CL
    void DownloadWorkFileSettingFormServer();                                   //Steven 20181224 : For ASE-CL
    void __fastcall CheckFTPConnection();                                       //Isaac 20200710 : 全智科技，從FTP下載後要切換到initial start
    bool __fastcall CheckSetupFileNameFromServer(AnsiString asCheckSetupFileName);      //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
    void UploadFileFTP(AnsiString asSourcePath,AnsiString asSourceFileName, AnsiString asTargetPath, AnsiString asTargetFileName, AnsiString asUserID, AnsiString asPassword, AnsiString asHost, AnsiString asFun); //Sam 20210120 : Auto Upload FTP Temperature Info 功能

    int N25_ReadAutoStartFileFromFTP();                                         //Steven 20210413 : 南茂的自動Start功能, -1:Alarm, 0:不能跑, 1:可以跑
    void N25_UploadTempDataToFTP(int iType);                                    //Steven 20210413 : 南茂的自動Start功能
    bool DownloadPasswordFormServer();                                          //Steven 20210618 : For 晶兆成
    void N22_Upload_JHT_LOG();                                                  //JerryYang 20250120 : modify
    void N35_Upload_JHT_LOG();
    AnsiString N31_AtuoTempureOfsByFTP();                                       //Sam 20220406 : 溫度自動補償功能 By FTP
    AnsiString N31_AtuoTempureOfsByLocal();                                     //Sam 20220427 : 溫度自動補償功能 By Loacl
    void DownloadUpdateAutomatically(bool bhint=false);                         //Sam 20220824 : FTP 自動下載安裝更新包
    AnsiString N32_DownloadUpdateAutomatically(AnsiString sPatch="",bool bDeleteFile=true); //Sam 20230328 : 自動更新增加版本檢查 //Sam 20220824 : FTP 自動下載安裝更新包
    TMyStringList *slDailyTempLog;
    TMyStringList *sl10MinTempLog;
    void __fastcall Download_2DSortingList(AnsiString FtpPath, AnsiString Source, int iTimeOut=20000);  //RogerYang 20251217 Add for timeout    //Frank 20221122 : 2DID sorting for ATK
    void __fastcall Download_2DID_WhiteList(AnsiString FtpPath);                //JerryYang 20250120 : modify
    bool __fastcall DoCheckPassword();                                          //Ifor 20240109 add:FTP 上傳需要帳號密碼
    void BackUpDataToServerByFTP(AnsiString asDirPath,AnsiString sFileName);    //Eastsun 20260520 : 整合
};
//---------------------------------------------------------------------------
extern PACKAGE TfFTPClient *fFTPClient;
//---------------------------------------------------------------------------
#endif
