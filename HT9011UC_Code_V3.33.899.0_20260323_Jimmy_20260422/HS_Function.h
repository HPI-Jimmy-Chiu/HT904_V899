//---------------------------------------------------------------------------

#ifndef HS_FunctionH
#define HS_FunctionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <NMFtp.hpp>
#include <Psock.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
#define HS_ERR_NoError                       0
#define HS_ERR_FunctionDisable              -1
#define HS_ERR_FTPConnectError              -2
#define HS_ERR_FTPUploadError               -3
#define HS_ERR_FTPNoFileForUploadError      -4
#define HS_ERR_DirectoryError               -5

//---------------------------------------------------------------------------
enum eRecordType                   //決定要幾種不一樣的Record設定
{   eReocrd_EP          =0,         //EP
    eReocrd_Temp        =1,         //Temp
    eReocrd_ESD         =2,         //ESD
    eReocrd_ATCEven     =3,         //ATC Even
    eReocrd_Space       =4,         //預留
    eReocrd_TestArm     =5,         //TestArm
    eReocrd_ChangEven   =6,         //ChangEven
    eRecordTypeTotal
};
class TFormHS : public TForm
{
__published:    // IDE-managed Components
    TTimer *TimerAutoBackup;
    TPanel *pnlSlfeTestRun;
    TLabel *Label1;
    TPanel *pl_ATCSelfTestSatus_01;
    TLabel *Label2;
    TPanel *pl_ATCSelfTestSatus_02;
    TLabel *Label3;
    TPanel *pl_ATCSelfTestSatus_03;
    TLabel *Label4;
    TPanel *pl_ATCSelfTestSatus_99;
    TLabel *Label5;
    TServerSocket *ESDServerSocket;
    TServerSocket *RTMServerSocket;
    TTimer *TimerRTMMsg;
    TClientSocket *HandlerClientSocket;
    void __fastcall TimerAutoBackupTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ESDServerSocketClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ESDServerSocketClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ESDServerSocketClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall RTMServerSocketClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall RTMServerSocketClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall RTMServerSocketClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall TimerRTMMsgTimer(TObject *Sender);
    void __fastcall RTMServerSocketClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall HandlerClientSocketConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall HandlerClientSocketError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall HandlerClientSocketRead(TObject *Sender,
          TCustomWinSocket *Socket);
private:    // User declarations
    bool bFTPError;
    AnsiString sFTPErrMsg;
    bool bNeedUpload_N10;
    bool bN10_3_1_Flag;
public:     // User declarations
    __fastcall TFormHS(TComponent* Owner);

    void __fastcall RecordLog_HS(bool bAnyWaySend=false);
    void __fastcall ClearLastFileLogName_HS(int iType);
    AnsiString __fastcall GetLastFileLogName_HS(int iType);                     //iType 0:EP 1:Temperature
    void __fastcall RecordEPLog_HS(AnsiString aFileName);
    bool __fastcall ReadMultiEP();                                              //AI(ht9045-v899) 20260504: port from V896 (Ifor 20250417) Multi EP 16ch APAX read.
    void __fastcall RecordESDLog_HS(AnsiString aFileName);                      //Ifor 20160305 ESD Log 記錄
    void __fastcall RecordTemperatureLog_HS(AnsiString aFileName);
    int  __fastcall UpDataToServer_KYEC(int iLog, bool bForceSend=false);       //JerryYang 20190426 fix server上傳問題                       //iLog 0: temperature 1: EP
    int  __fastcall UpDataToServerByFTP(AnsiString asDirPath,AnsiString sFileName, AnsiString asFileType, bool bDailyReport=false);
    void CalculateUploadToHostIntervalTimeFunction();                           //Ifor 20160302 Add FTP Up load Log To Host
    bool __fastcall CheckSetupNamelist_Hisi();
    AnsiString __fastcall GetTempUseName(int source);                           //Ifor 20160303 依據溫控器開啟狀況變更Log 顯示名稱
    AnsiString __fastcall GetTempUseNamevalue(int source);                      //Ifor 20160303 依據溫控器開啟狀況變更Log 顯示數值
    bool __fastcall CheckCanRunStart_HS();
    bool __fastcall CheckSetupFileData(AnsiString strSetupFile);                //Ifor 20160822 add 確認Setup File Data 資料是否有缺
    TStringList *tHiSiRecord;
    bool    bIndexForHT9046AH;  //20150706 add
    void __fastcall ShowATCSelfTestSatus(bool bFlash,bool bHasAlarm);           //Ifor 20160829 add ATC Self Test Satus 顯示
    void __fastcall LoaderBufPreAlarm();                                        //Ifor 20160829 add Loader Try PreAlarm
    void __fastcall FixTrayPreAlarm();                                          //Ifor 20160829 add Fix Try PreAlarm
    int __fastcall ShowATCAlarmPosition(int iCode);                             //Ifor 20160830 add ATC Alarm 位置顯示判斷
    void __fastcall ShowATCAlarmMessage();                                      //Ifor 20160831 add Show ATC Alarm Message
    void __fastcall RecordATCEvenLog_HS(AnsiString aFileName);                  //Ifor 20161202 add ATC Even Log 資料
    void __fastcall SaveCloseOpenSiteEven(int iZ,int iRow,int iCol,bool bUse);  //Ifor 20161219 add KYEC 開關Site Even Log 格式 開Site:O 關Site:X
    void __fastcall AutoConnectATCNetwork();                                    //Ifor 20170202 (Steven) add KYEC ATC 網路芳鄰自動連線
    bool __fastcall CheckIndependentPassWord();                                 //Ifor 20170925 (Steven) add Confirm the independent password

    void __fastcall NMFTP1Failure(bool &Handled, TCmdType Trans_Type);
    void __fastcall NMFTP1Error(TComponent *Sender, WORD Errno, AnsiString Errmsg);
    void __fastcall NMFTP1UnSupportedFunction(TCmdType Trans_Type);
    bool __fastcall CheckTempOffset(int Addr,double SetTemp, bool bSendAlarm=false); //Ifor 20180809 (Steven) : add Check the temperature Offset limit
    void __fastcall RecordArmTestInfoLog_HS(AnsiString aFileName);              //Ifor 20190912 :add 海思 V02.30 版 Record Torque
    void __fastcall CloseWindowsKeyboard();                                     //Ifor 20190920 : add 關閉 Windows 小鍵盤
    void __fastcall RecordChangeLogByLot(AnsiString S, AnsiString S2);          //Ifor 20191002 : add Change Log By Lot
    void __fastcall RecordContact_SLTLog_HS();                                  //Ifor 20191218 : add KYEC 要求 同SLT輸出表格
    bool bHaltHandler;
    AnsiString RTMStr;
    void __fastcall RecordRunState();                                           //Ifor 20230420 add:ASEM要求新增Run Status Log Function
    bool CheckClockTrigger(int iClock);                                         //Sam 20230504 : 整合 Clock 觸發執行動作
    void __fastcall ShowHisiAddAlarm(AnsiString Code, int KCode, int Pos, bool bDuplicateErr, AnsiString errPart);  //Ifor 20200728 add:ATC溫度異常需測試完畢才可Alarm
    void __fastcall CheckEPRange();                                                //Ifor 20220616 : add KLT要求 EP Alarm By SLK force 精準度
    void __fastcall ATC_FFCTrigger(bool bOnOff);                                   //Ifor 20221003 add: ATC FFC Trigger Even
    void __fastcall RecordAutoMotive(AnsiString sDirPath, AnsiString aFileName, AnsiString sD1, eRecordType iType);//Ifor 20221024 add: KLT Auto Motive Function
    void __fastcall BackupGPIBLog();                                               //Ifor 20221024 add: KLT Auto Motive Function
    void __fastcall GetTitleName(AnsiString &S, eRecordType iType);                 //Ifor 20221024 add: KLT Auto Motive Function
    void __fastcall RecordPowerSaveEvenLog_HS(AnsiString SRecordData, bool bIsATC=false);          //Ifor 20220929 add Record Power Save Even Log
    bool __fastcall CheckATCTempWait(); //Ifor 20230608 add:KYEC 要求新增ATC 溫度等待功能                                         //Ifor 20230608 add:    
    void __fastcall TESTTEMPSETTING(int iTemp);                                 //Ifor 20240117 add:SECSGEM測試中修改ATC溫度
    bool __fastcall CheckKeyPro(bool bStart=false);                             //Ifor 20230721 : add Check Keypro
    void __fastcall RecordParameter_TFAMDLog();                                 //Ifor 20191218 : add KYEC 要求 同SLT輸出表格
    AnsiString GetUploadServerByFTPPath(AnsiString asFileType, bool bDailyReport=false);
    AnsiString GetGPIBLogFilePath();
    std::vector<AnsiString> GetGPIBLogFileName();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormHS *FormHS;
//---------------------------------------------------------------------------
#endif
