//---------------------------------------------------------------------------

#ifndef ATCInterfaceH
#define ATCInterfaceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include "ALed.hpp"
#include "MyLed.h"

//---------------------------------------------------------------------------
#include "ATCSystem.h"
#include "MachineType.h"    //Steven 20130809 : OK
//
//#ifdef HT9045
//    #include "MachineDefine.h"
//#endif
//---------------------------------------------------------------------------
class PACKAGE TMyHonPrecATCPanel : public TComponent
{
    private:    // User declarations
        void __fastcall ATCSocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);
    public:
        __fastcall TMyHonPrecATCPanel(TComponent* Owner, int index);
        __fastcall ~TMyHonPrecATCPanel();

        TGroupBox       *gbATC;
        TGroupBox       *gbOffset;
        TClientSocket   *ATCSocket;
        TMyLed          *LedATCConnect;
        TMyLed          *LedATCActive;
        TLabel          *labState;
        TLabel          *labAddress;
        TLabel          *labPort;
        TEdit           *edtAddress;
        TEdit           *edtOffset;
        TEdit           *edtPort;
        TEdit           *edtSendData;
        TBitBtn         *BitBtnSet;
        TBitBtn         *BitBtnConnect;
        TBitBtn         *BitBtnDisconnect;
        TBitBtn         *BitBtnSend;
        TButton         *btSetOffset;
        TMemo           *MemoATC;
        TEdit           *pal_SV;
        TPanel          *pal_PV;
        TCheckBox       *cbEnableSite;
        AnsiString sAddress;
        int iPort;
        int iMyTag;
        //void SetDisplay(int iLevel);

        bool bATCRunSetting;      //Steven 20120608 : ATC啟動的狀態
        bool bATCRunStatus;
        bool bSiteOnOffStatus;    //Steven 20120528 : 確認開關Site狀態
        bool bSiteOnOffSetting;   //Steven 20120528 : 開關Site設定
        bool bATCTempReady;       //Steven 20120530 : 確認溫度是否到達
        int iRecordCount;         // 2011.05.24 , Joye , ATC
//        int iSetTempCheckCount;
        int iErrorCount;         //pig 2011.12.21 ATC改
        double fLastTemp;     // 2011.05.24 , Joye , ATC
        double fATCSiteNowTemp;
        double fATCRefSensorTemp;       //Steven 20150108 : [L11-5] For海思使用兩組感溫
        double fATCSiteSVSetting;       //Steven 20120528 : 預期的設定溫度
        double fATCSiteSVValue;         //Steven 20120528 : 目前的ATC設定溫度
        double fATCSetChillerSV;        //Steven 20120528 : 冰水機預期的設定溫度
        double fATCSetChillerValue;     //Steven 20120528 : 冰水機目前的工作溫度
        double fATCOffsetSetting;       //Steven 20120626 : Offset設定溫度
        double fATCOffsetValue;         //Steven 20120706 : Offset目前的工作溫度
        void SetATCPIDParameter( double fMinP , double fMinI ,double fMinD ,double fMaxP ,double fMaxI , double fMaxD );    //Ifor 20150910 :ATC PID Control
        int fATCNotReceivedCount;       //Ifor 20160223 未收到溫度回傳次數
        int iATCSelfTestResult;         //Ifor 20160720 Add ATC SELF TEST RESULT 結果
        bool bRunChillerStatus;         //Ifor 20160801 Add ATC2.0 Run Chiller Status
        bool bRunATCStatus;             //Ifor 20160801 Add ATC2.0 Run ATC Status
        bool bUseRefSensorStatus;       //Ifor 20160801 Add ATC2.0 Use RefSensor Status
        int iATCSelfTestStatus;         //Ifor 20160824 Add ATC SELF TEST Status
        bool bATCLotStart;              //Ifor 20161118 add ATC2.0 Lot Start flag
        bool bATCLotEnd;                //Ifor 20161118 add ATC2.0 Lot End flag
};
//---------------------------------------------------------------------------
class TATCInterfaceForm : public TForm
{
__published:    // IDE-managed Components
    TTimer *ATCWatchTimer;
    TPanel *Panel2;
    TBitBtn *btnClearAll;
    TBitBtn *btnExit;
    TTimer *TimerChillerStop;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TPanel *pnlTitle;
    TScrollBox *ScrollBoxATC;
    TGroupBox *gbATC;
    TMyLed *LedATCConnect;
    TLabel *labState;
    TLabel *labAddress;
    TLabel *labPort;
    TMyLed *LedATCActive;
    TEdit *edtAddress;
    TEdit *edtPort;
    TBitBtn *BitBtnConnect;
    TBitBtn *BitBtnDisconnect;
    TEdit *edtSendData;
    TBitBtn *BitBtnSend;
    TMemo *MemoATC;
    TBitBtn *BitBtnSet;
    TPanel *pal_PV;
    TGroupBox *gbOffset;
    TEdit *edtOffset;
    TButton *btSetOffset;
    TCheckBox *cbEnableSite;
    TEdit *pal_SV;
    TGroupBox *gbChiller;
    TMyLed *ledChiller;
    TLabel *Label1;
    TPanel *palChillerPos;
    TPanel *palChillerCurr;
    TPanel *palChillerSet;
    TPanel *pan_ATCTempChiller;
    TPanel *pl_ATCTempChiller;
    TEdit *pl_ATCSetTempChiller;
    TButton *btnATCChillerSwitchRun;
    TButton *btnATCChillerSwitchStop;
    TButton *btnATCPower;
    TButton *btOnLine;
    TButton *btOffLine;
    TButton *btSetChillerTemp;
    TButton *btSave;
    TButton *Button1;
    TButton *btSetWorkTemp;
    TGroupBox *gbL11;
    TLabel *labChillerProtectedFunction;
    TEdit *edATCChillerCheckTime;
    TCheckBox *cbChillerProtectedFunction;
    TButton *Button2;
    TPanel *Panel1;
    TScrollBox *ScrollBox1;
    TMyLed *LedATC7Connect;
    TLabel *Label2;
    TMyLed *LedATC7Active;
    TLabel *Label3;
    TMemo *HandlerMemo;
    TGroupBox *GroupBox3;
    TPanel *Panel_CH1_Temp;
    TPanel *Panel_CH1_TSDTemp;
    TGroupBox *GroupBox2;
    TPanel *Panel_CH2_Temp;
    TPanel *Panel_CH2_TSDTemp;
    TGroupBox *GroupBox4;
    TCheckBox *cBoxEnabledCH1;
    TCheckBox *cBoxEnabledCH2;
    TCheckBox *cBoxEnabledTSD;
    TEdit *edATC7_Temp;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TBitBtn *BitBtn3;
    TBitBtn *BitBtn4;
    TBitBtn *BitBtn5;
    TServerSocket *ATC7_ServerSocket;
    TCheckBox *cBoxEnabledCH3;
    TCheckBox *cBoxEnabledCH4;
    TGroupBox *GroupBox1;
    TPanel *Panel_CH3_Temp;
    TPanel *Panel_CH3_TSDTemp;
    TGroupBox *GroupBox5;
    TPanel *Panel_CH4_Temp;
    TPanel *Panel_CH4_TSDTemp;
    TTimer *TimerATC;
    TCheckBox *chkSaveLog;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ATCWatchTimerTimer(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall btnClearAllClick(TObject *Sender);

    void __fastcall FormDestroy(TObject *Sender);
        void __fastcall btnATCPowerClick(TObject *Sender);
        void __fastcall btnATCChillerSwitchRunClick(TObject *Sender);
        void __fastcall btnATCChillerSwitchStopClick(TObject *Sender);
    void __fastcall btOnLineClick(TObject *Sender);
    void __fastcall cbEnableSiteClick(TObject *Sender);
    void __fastcall btOffLineClick(TObject *Sender);
    void __fastcall btSetChillerTempClick(TObject *Sender);
    void __fastcall btSetOffsetClick(TObject *Sender);
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall btSetWorkTempClick(TObject *Sender);
    void __fastcall TimerChillerStopTimer(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall ATC7_ServerSocketClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ATC7_ServerSocketClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ATC7_ServerSocketClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn3Click(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall TimerATCTimer(TObject *Sender);
    void __fastcall pl_ATCSetTempChillerClick(TObject *Sender);
    void __fastcall edtOffsetClick(TObject *Sender);
    void __fastcall edATCChillerCheckTimeClick(TObject *Sender);
    void __fastcall edATC7_TempClick(TObject *Sender);
    void __fastcall edtPortClick(TObject *Sender);
private:    // User declarations
    int _Show_Type;  // Show Type
    bool _Is_Show;
    int iATCUseCount;
    int iGroupBoxHeight;
    bool bSystemStart;
public:     // User declarations
    void __fastcall ATCSocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);
    void __fastcall BitBtnConnectClick(TObject *Sender);
    void __fastcall BitBtnSetClick(TObject *Sender);
    void __fastcall BitBtnDisconnectClick(TObject *Sender);
    void __fastcall BitBtnSendClick(TObject *Sender);

    __fastcall TATCInterfaceForm(TComponent* Owner);
    __fastcall ~TATCInterfaceForm();

    ATCSystem ATC_SYS;
    ATC60System ATC_60_SYS;     //2014-05-30    Dell    for ATC6.0
    vector< TMyHonPrecATCPanel* > ATC_SYS_PAL;

    int iTempCheckAlarmCount;
    int iTempCheckDelayTime;

    int iCheckSameTempTime;

    void LoadATCSystem();
    void WriteATCSystem( int iChannel );

    void ShowInterface( int iType = 0);
    void OnLine();
    void OffLine();
    bool IsOnLine();

    void OpenChannel( int iChannel );
    void CloseChannel( int iChannel );
    void SetWorkTemperature( int iChannel , double fTemp );
    void TATCInterfaceForm::SetATCSelfTest();
    void TATCInterfaceForm::SetATCManualSelfTest();             //Ifor 20160823 add Manual Send ATC Self Test Command
    bool CheckWorkTemperature(int iChannel, double fTemp);      //Steven 20130701 : 確認ATC設定正確
//    bool TemperatureReady( int iChannel );
//    bool GetNowTemperature( int iChannel );

    void SendTestStart( int iIndex );
    void SendTestEnd( int iIndex );

    void RefreshChannelState();
    void SetOffsetTemperature( int iChannel , double fTemp );   //pig 2011.12.21 ATC改
    void SetRunATC(bool bRun);                                  //pig 2011.12.21 ATC改
    void SetChillerTemperature(int fTemp);                      //pig 2011.12.21 ATC改
    double GetATCSiteNowTemperature(int iChannel);              //pig 2011.12.21 ATC改
    double GetATCSiteNowTemperature_Ref(int iChannel);          // 2014.10.04 , Joye , KYEC Reffer Temperatrue   //Steven 20150108 : [L11-5] For海思使用兩組感溫
    bool ATCRun(bool bRun) ;                                    //pig 2011.12.21 ATC改
    void ATCChillerSwitch(bool bopen);                          //pig 2011.12.21 ATC改
    void CheckHandlerConnectATC();                              //pig 2011.12.21 ATC改

    void SetSystemStatus(bool bStart)   {bSystemStart=bStart;}; //機台運作狀態
    void __fastcall InitialATC(eATCType AtcType=eATCHonPrecType, int ATCUseCount=2, AnsiString sATCIniPath="Config\\ATC.ini");
    bool bOnLineSetting;
    int  iOnLineStep;

    int  ATC_SYSTEM;                        // 2011.04.15 , Joye , ATC
    bool bATCConnectError;                  //pig 2011.12.21 ATC改
    int  iStopATCChillerType;               // 2012.05.07 , Joye , Chiller
    int  iATCChillerCheckTime;
    bool bL11_2ATCChillerProtectedFunction;

    void __fastcall SendCommToATC7(int iCommIndex, String sData1, String sData2); //Eliot 2015_0105
    char pucInBuff[8192];                   //Eliot 2015_0105
    double fATC7NowTemp[4];                 //Eliot 2015_0105
    double fATC7NowTSDTemp[4];              //Eliot 2015_0105
    bool bShowAlarm;                        //Eliot 2015_0105
    AnsiString asAlarmMessage;              //Eliot 2015_0105
    AnsiString ATCIniPath;
    int __fastcall GetRunSelfTestResult();
    int __fastcall GetRunSelfTestStatus();  //Ifor 20160824 add Get ATC Self Test Sataus
    bool bATC20AutoConnect;
    void __fastcall LotStart(String sLotID);//Ifor 20161118 add ATC2.0 Lot Start Command for KYEC(ATCInterfaceForm::)
    void __fastcall LotEnd(String sLotID);  //Ifor 20161118 add ATC2.0 Lot End Command for KYEC(ATCInterfaceForm::)
    bool __fastcall GetATCLotStartStatus(); //Ifor 20160824 Get ATC2.0 Lot Start Status
    bool __fastcall GetATCLotEndStatus();   //Ifor 20160824 Get ATC2.0 Lot End Status
    bool bIsWaterLeakage;                   //Steven 20210325 : 漏水檢知
};
//---------------------------------------------------------------------------
extern const int ATC_RUN;     //Eliot 2014_0918
extern const int ATC_STOP;
extern const int ATC_SET_TEMP;
extern const int ATC_USE_TSD;
extern const int ATC_CH_ENABLED;
extern const int ATC_TEMP_VALUE;
extern const int ATC_MESSAGE;
extern const int ATC_EMG_UP;    //Eliot 2015_0122
extern const int ATC_EMG_DOWN;  //Eliot 2015_0122
extern const int ATC_SOT;       //Steven 20151112 : for ATC 7.0
extern const int ATC_EOT;       //Steven 20151112 : for ATC 7.0
extern const int ATC_OFFSET;    //Steven 20151112 : 修改ATC7.0 Offset
extern const int ATC_SEND_TEMP_READY;   //Steven 20160604 : by site TSD
extern bool bShowAlarm;
extern AnsiString asAlarmMessage;

//---------------------------------------------------------------------------
extern PACKAGE TATCInterfaceForm *ATCInterfaceForm;
//---------------------------------------------------------------------------
#endif
