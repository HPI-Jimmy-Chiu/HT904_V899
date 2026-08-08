// =============================================================================
//  ATC/ATCSystem.h  --  ATC 2.0/6.0 temperature-controller channel model
//                       (ATC_COMMAND / ATCChannel / ATCData / HT_ATC / ATCSystem
//                       / ATC60System).
//
//  Faithful translation of golden ATC/ATCSystem.h (387 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W3) 20260807
//  Translation wave: W906-PT-W3, group "atcsystem".
//
//  ROLE: the non-UI half of the Handler<->ATC link.  ATC_COMMAND is a plain
//  (Command-string, iDataPosition, iDataLength, iCommandLength, iType) record
//  the ATC-2.0 protocol table is built from.  ATCChannel is the abstract
//  transport interface; HT_ATC is its ATC-2.0-protocol concrete
//  implementation over a TCPIP* (TCPData.h's TCPClient), owning the fixed
//  ATC_MAX_CMD-sized command table, the two mutex-guarded queues
//  (_history / _read_data) and the small set of "H->A" one-line senders.
//  ATCSystem is the multi-channel façade ATCInterface.cpp drives (index by
//  channel number, cast Channels[i] back to HT_ATC*).  ATC60System is a
//  SEPARATE, older ATC-6.0/3.0-protocol driver over a raw TClientSocket,
//  bracket-framed ("[STX]CMD<n>,args[ETX]") rather than the 2.0 table
//  protocol -- unrelated wire format, unrelated state, kept side by side
//  exactly as golden does.  NO golden state machine / switch(Task) cursor in
//  this header -- it is data + small imperative helpers.
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim, whole header): ATC_COMMAND (golden :18-27),
//     ATCChannel (golden :29-43), ATCData (golden :45-81), HT_ATC (golden
//     :83-201) including its full ATCCommandIndex enum and every inline H->A
//     one-liner, HT_ATC_TCPIP_WATCH decl (golden :202), ATCSystem (golden
//     :205-272), ATC60System (golden :275-384) including its own
//     ATCCommandIndex enum and every field.
//   SATISFIED-BY-SUBSTRATE (already real in this tree, nothing shimmed here):
//     TCPIP/TCPClient/DATA/DATAs/STRINGs/TD_* (ATC/TCPData.h, landed PT-W2),
//     HThreadCtrl (HThreadCtrl/HThreadCtrl.h, landed PT-W2), AnsiString/
//     TStringList (vclcompat), Scktcomp::TClientSocket/TCustomWinSocket/
//     TErrorEvent (vclcompat/ClientSocket.h, via the vcl_compat.h umbrella --
//     same substrate TCPData.h's own banner already cites), MySleep/MySleepEx
//     (common.h), tcTotalCount (MachineType.h enum).
//   GATE (#if 0): NONE.  Every symbol this header names resolves to a real
//     compiled body somewhere in this tree (verified by grep, see
//     ATCSystem.cpp's own banner for the citations) -- this header itself
//     only declares.
//
//  VCL/Borland conversions: golden's `#include <ScktComp.hpp>` is replaced by
//  pulling Scktcomp::TClientSocket/TCustomWinSocket/TErrorEvent in through
//  `vclcompat/vcl_compat.h` (the same substitution ATC/TCPData.h already made
//  for the identical golden include -- see that header's own banner).  golden
//  `String` parameters (ATC60System::LoadRecipe is the one AnsiString-typed
//  exception; HT_ATC::LotStart/LotEnd/ATCSystem::LotStart/LotEnd/
//  ATC60System ctor are the `String`-typed ones) become `AnsiString` --
//  BCB6's `String` IS `AnsiString` in this project's non-Unicode ANSI build
//  (confirmed: every other translated unit in this tree spells golden
//  `String` parameters as `AnsiString`, e.g. aHotPlateSubstrate.h; this is a
//  spelling normalisation, not a type change).  `__fastcall` is DROPPED from
//  every method (golden marks none in THIS header at all -- ATCSystem/HT_ATC
//  carry none; only ATC60System's ctor/dtor/GetRunSelfTestResult-style
//  `__fastcall` markers exist, and those are actually on the .cpp
//  definitions -- kept dropped there too, matching TCPData.h's own
//  established `__fastcall`-drop precedent).  No __property / __published /
//  #pragma in this header (golden has none either).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
#ifndef _ATC_SYSTEM_H
#define _ATC_SYSTEM_H

#include "vclcompat/vcl_compat.h"   // AnsiString/TStringList/DWORD; Scktcomp::TClientSocket/
                                    // TCustomWinSocket/TErrorEvent/TServerSocket (golden <ScktComp.hpp>)
#include "ATC/TCPData.h"            // TCPIP/TCPClient, DATA/DATAs/STRINGs, TD_*
#include "HThreadCtrl/HThreadCtrl.h" // HThreadCtrl
#include "MachineType.h"            // tcTotalCount (enum), ATC_HEAD_COUNT

#define ARD_NONE        0
#define ARD_VALUE       1
#define ARD_ALARM       2   //Site Alarm
#define ARD_SYS_ALARM   3   //系統Alarm

extern DWORD MySleepEx(DWORD dwMilliseconds, bool bAlertable);

class ATC_COMMAND
{
    public:
        ATC_COMMAND(int iType, AnsiString Command, int iDataPosition, int iDataLength, int iCommandLength);
        AnsiString Command;
        int  iDataPosition;
        int  iDataLength;
        int  iCommandLength;
        int  iType;
};
// ATC Channel------------------------------------------------------------------
class ATCChannel
{
    private:
    protected:
        vector< string >    _history;
    public:
        virtual void Connect()      = 0;
        virtual void Disconnect()   = 0;
        virtual bool IsConnect()    = 0;
        virtual void ClearHistory()                     = 0;
        virtual void PushHistoryString( string log)    = 0;
        virtual bool PopHistoryString( string& log)    = 0;
        virtual int SendCommand( DATA SendData )        = 0;
        virtual int SendCommand( string SendData )      = 0;
};
//---------------------------------------------------------------------------
class ATCData
{
    private:
    protected:
    public:
        ATCData()
        {
            iType               = ARD_NONE;
            iData               = -9999;
            fData               = -9999;
            fReftemp            = -9999;       //Ifor 20151008 Add ATC 第二組溫度值
            bData               = false;
            bATCLotStart        = false;       //Ifor 20161118 add ATC2.0 Lot Start flag
            bATCLotEnd          = false;       //Ifor 20161118 add ATC2.0 Lot End flag
            sData               = "";
            sCommand            = "";
            iATCSelfTestResult  =0;
            iATCSelfTestStatus  =0;
            Index               =0;            //Jimmychiu 20230307 initialized in the constructor
            sATCSwVer           = "";          //Jimmychiu 20230307 initialized in the constructor
            strATCStatus        = "";          //Jimmychiu 20230307 initialized in the constructor
        };
        int     iType;              //資料型別種類, 決定作法
        int     Index;              //對應到的指令序號
        int     iData;
        double  fData;
        bool    bData;
        double  fReftemp;           //Ifor 20151008 Add ATC 第二組溫度值
        string  sData;              //準備要顯示的文字
        string  sCommand;           //對應到的指令碼
        string  sATCSwVer;          //Ifor 20151026 目前ATC軟體版本
        int iATCSelfTestResult;     //Ifor ATC Self Test Result
        AnsiString strATCStatus;    //Ifor 20160801 add ATC 2.0 狀態
        int iATCSelfTestStatus;     //Ifor 20160824 add ATC Self Test Status
        bool bATCLotStart;          //Ifor 20161118 add ATC2.0 Lot Start flag
        bool bATCLotEnd;            //Ifor 20161118 add ATC2.0 Lot End flag
};
//---------------------------------------------------------------------------
class HT_ATC: public ATCChannel
{
    private:
    protected:
        unsigned int        _number;
        CRITICAL_SECTION    csHistory;
        CRITICAL_SECTION    csReadData;
        vector< ATCData >   _read_data;
    public:
        TCPIP*      _socket;
        HThreadCtrl         _socket_thread;
        double              _Temperature_P_Limit;   // 2012.02.02 , Joye , ATC改
        HT_ATC(TCPIP* pSocket, int iNumber);
        ~HT_ATC();

        //注意排序：有相同字串的，較長的要放在較前面的位置！
        enum ATCCommandIndex{ATC_SITE_XXXX=0,
                             ATC_NOW_SETUP_TEMP,                //ATC目前設定溫度為何。
                             ATC_HANDLER_TESTER_POWEROFF,       //HANDLER測試電源OFF(目前並無使用)。
                             ATC_DISABLE,                       //ATC Site關閉。
                             ATC_POWER_SUPPLY_OFF,              //ATC電源供應器不正常。
                             ATC_LEAK_WATER,                    //ATC漏水。
                             ATC_TEMP_READY_XX,                 //ATC XX Site溫度已到達。
                             ATC_TEMP_NOT_READY,                //ATC Site溫度尚未到達。
                             //ATC_TEMP_READY,                    //ATC全部Site溫度已到達。
                             ATC_TEMP_BELOW,                    //ATC溫度過低。
                             ATC_WATER_FLOW_ERROR,              //ATC流量不正常。
                             ATC_CONNECT_X_OK,                  //HANDLER與ATC連線是否成功。
                             ATC_SETUP_TEMP_OK_XX,
                             ATC_CHILLER_CONNECT_ERROR,
                             ATC_OFFSET_OK_XX,
                             ATC_SETCHILLERTEMP_OK,
                             ATC_RUN_ATC_FUNCTION_OK,
                             ATC_STOP_ATC_FUNCTION_OK,
                             ATC_RUN_CHILLER_OK,
                             ATC_STOP_CHILLER_OK,
                             ATC_READ_TEN_SAME_TEMP,
                             ATC_CHILLER_HAS_NOT_STARTED_ERROR,
                             ATC_SITE_STATE_X,
                             ATC_WORK_TEMP_XXXX,
                             ATC_CHILLER_TEMP_XXXX,
                             ATC_WATER_LEVEL_OVERLOW,
                             ATC_WATER_TEMPERATURE_OVERHIGH,
                             ATC_WATER_TEMPERATURE_OVERLOW,     //Ifor 20160718 add ATC2.0 溫度過低
                             ATC_WATER_FLOW_TEMP_OVERHIGH,
                             ATC_SETUP_ARM_OK,
                             //ATC_REFER_SITE_XXXX,             //Steven 20150108 : [L11-5] For海思使用第二支溫感   //Steven 20150210 : Mark for海思已拆架
                             ATC_READ_SW_VER,                   //Ifor 20151023 Add ATC 2.0 讀取ATC軟體版本
                             ATC_SET_TIME_SYNC_OK,              //Ifor 20151023 Add ATC 2.0 設定ATC電腦時間同步
                             ATC_SET_DUAL_TEMP_OK,              //Ifor 20160427 Add ATC 2.0 設定第二點Sensor Function On/Off
                             ATC_WATER_PRESSURE_OVERHIGH,       //Ifor 20160718 add ATC2.0 壓力過高
                             ATC_WATER_PRESSURE_OVERLOW,        //Ifor 20160718 add ATC2.0 壓力過低
                             ATC_WRITE_FIVE_SAME_TEMPERATURE,   //Ifor 20160718 add ATC2.0 溫度寫入檔案五次相同
                             ATC_Refer_Temp_Alarm,              //Ifor 20160718 add ATC2.0 第二點溫度異常
                             ATC_SELFTEST_RESULT_OK,            //Ifor 20160721 add ATC self-test OK
                             ATC_SELFTEST_RESULT_NG,            //Ifor 20160721 add ATC self-test +5 NG
                             ATC_SELFTEST_STATUS,               //Ifor 20160824 add ATC2.0 Get ATC Self Test Status
                             ATC_TEMP_OVER_HIGH,                //Ifor 20160721 add ATC2.0 溫度超過最大值
                             ATC_ALARM_TEMP,                    //Ifor 20160721 add ATC2.0 溫度超過最大值
                             ATC_READ_STATUS,                   //Ifor 20160801 add ATC2.0 ATC 狀態讀取
                             ATC_LotStart_STATUS,               //Ifor 20161118 add ATC2.0 接收Lot Start STATUS Command Index
                             ATC_LotEnd_STATUS,                 //Ifor 20161118 add ATC2.0 接收Lot End STATUS Command Index
                             ATC_MAX_CMD                        //這個一定得在最下面,不可以更改
                            };

        ATC_COMMAND *ATC_ReceiveCMD[ATC_MAX_CMD];
        AnsiString ATC_ProcessReceiveCMD;

        // virtual function ----------------------------------------------
        virtual void Connect();
        virtual void Disconnect();
        virtual bool IsConnect();
        virtual void ClearHistory();
        virtual void PushHistoryString( string log);
        virtual void PushHistoryString( string sType , DATA Data );
        virtual bool PopHistoryString( string& log);
        virtual int SendCommand( DATA SendData );
        virtual int SendCommand( string SendData );
        // ---------------------------------------------------------------
        void ClearReadData();
        void PushReadData( ATCData aData );
        bool PopReadData( ATCData& aData );
        void InitialSocket( string sAddress , int iPort );
        int AdapterData( DATA Data );
        bool IsCommandString( string sData , string sCommand , int iPos = 0);
        // Handler -> ATC ------------------------------------------------

        void GetNowSiteOnOff()                                      {SendCommand("ATC_GET_SITE_ON_OFF");};      //H->A 02 Steven 20120528 : 確認開關Site
        void GetNowWorkTemperature()                                {SendCommand("ATC_TEMP_GET");};             //H->A 05 Steven 20120528 : 確認工作溫度
        void GetATCState()                                          {SendCommand("ATC_CHECK");};                //H->A 09 12.06.27.01
        void CheckTargetTemperatureReady()                          {SendCommand("ATC_TEMP_OK");};              //H->A 10
        void GetAllNowTemperature()                                 {SendCommand("HANDLER_READ_ATC_TEMP_ALL");};//H->A 11 12.06.27.01
        void GetNowTemperature()                                    {SendCommand("HANDLER_READ_ATC_TEMP");};    //H->A 12
        void GetNowTemperature_Ref()                                {SendCommand("HANDLER_REFER_READ_ATC_TEMP");};    //H->A      //Steven 20150108 : [L11-5] For海思使用兩組感溫
        void SendTestStart()                                        {SendCommand("HANDLER_TEST_START");};       //H->A 13
        void SendTestEnd()                                          {SendCommand("HANDLER_TEST_END");};         //H->A 14
        void GetNowChillerTemperature()                             {SendCommand("ATC_GetChillerTemp");};       //H->A 17 Steven 20120528 : 確認冰水機溫度
        void RunATC()                                               {SendCommand("RUN_ATC_Function");};         //H->A 18 pig 2011.12.21 ATC改
        void StopATC()                                              {SendCommand("STOP_ATC_Function");};        //H->A 19 pig 2011.12.21 ATC改
        void ATCChillerSwitchOpen()                                 {SendCommand("RUN_Chiller");};              //H->A 20 pig 2011.12.21 ATC改
        void ATCChillerSwitchClose()                                {SendCommand("STOP_Chiller");};             //H->A 21 pig 2011.12.21 ATC改
        void ATCMessageClose()                                      {SendCommand("CANCEL_MESSAGE");};             //H->A 21 pig 2011.12.21 ATC改
        void SetTargetChillerTemperature(int fTarget);              //pig 2011.12.21 ATC改
        void SetTargetOffsetTemperature(double fTarget);            //pig 2011.12.21 ATC改
        void SetTargetTemperature(double fTarget);
        void SetUse(bool bUse);
        void CheckConnectState(int iChannel);                                                                   //12.06.27.01
        void SetHandlerNowArm(int iArm);
        void SetPID( double fMinP , double fMinI , double fMinD , double fMaxP , double fMaxI , double fMaxD ); //Ifor 20150910 ATC PID
        void GetNowATCSwVer()                                       {SendCommand("@READ_ATC_SW_VER+");};        //Ifor 20151026 Add ATC 2.0 讀取ATC軟體版本
        void SetATCTimeSync();                                                                                  //Ifor 20151026 Add ATC 2.0 設定ATC電腦時間同步
        void Set2ndTempFunction(bool bEnable);                                                                  //Ifor 20160427 Add ATC 2.0 設定第二點Sensor Function On/Off
        void SetATCSelfTest()                                       {SendCommand("ATC_RUN_SELFTEST");};         //Ifor 20160720 Add ATC 2.0 執行自我測試模式
        void GetNowATCStatus()                                      {SendCommand("<<GETATCSTATUS>>");};         //Ifor 20160801 Add ATC 2.0 讀取ATC狀態
        void SetATCManualSelfTest()                                 {SendCommand("ATC_MANUAL_SELFTEST");};      //Ifor 20160823 add Manual Send ATC Self Test Command
        void GetNowSelfTestStatus()                                 {SendCommand("ATC_SELFTEST_STATUS");};      //Ifor 20160824 Add ATC 2.0 讀取ATC Self Test 目前狀態
        void LotStart(AnsiString sLotID);                                                                       //Ifor 20161118 add ATC2.0 Lot Start Command for KYEC (HT_ATC)
        void LotEnd(AnsiString sLotID);                                                                         //Ifor 20161118 add ATC2.0 Lot Start Command for KYEC (HT_ATC)
};
void HT_ATC_TCPIP_WATCH( void* pParameter);
// -----------------------------------------------------------------------------
// ATC System ------------------------------------------------------------------
class ATCSystem
{
    private:
    protected:
        vector< bool >      _channel_use;
        bool                _online;
        bool                _ATCRun;                                            //pig 2011.12.21 ATC改
        bool                _Chiller_Run;                                       // 2012.05.07 , Joye , Chiller
        bool                _Chiller_Status;                                    //Steven 20120530 : 冰水機開啟狀態

    public:
        vector< ATCChannel* > Channels;
        ~ATCSystem();
        void Initial();
        unsigned int Size();
        void AddChannel( ATCChannel* pChannel );
        bool DeleteChannel( unsigned int iChannel );
        bool SetChannelSocket( unsigned int iChannel , string sAddress , int iPort );
        bool IsConnect( unsigned int iChannel );
        bool Connect( unsigned int iChannel );
        bool Disconnect( unsigned int iChannel );
        int  SendCommand( unsigned int iChannel , DATA SendData );
        int  SendCommand( unsigned int iChannel , string SendData );
        bool ClearHistory( unsigned int iChannel );
        bool PopHistoryString( unsigned int iChannel , string& log);
        bool ClearData( unsigned int iChannel );
        bool PopData( unsigned int iChannel , ATCData& aData );
        void SetOnLine( bool bOnLine);
        bool SetChannelUse( unsigned int iChannel , bool bUse);
        bool SetWorkTemperature( unsigned int iChannel , double fTemp );
        bool CheckTemperatureReady( unsigned int iChannel );
        bool GetNowTemperature( unsigned int iChannel );
        bool GetNowTemperature_Ref( unsigned int iChannel );
        bool GetAllNowTemperature( unsigned int iChannel );                     //12.06.27.01
        bool SendTestStart( unsigned int iChannel );
        bool SendTestEnd( unsigned int iChannel );
        bool SetOffsetTemperature( unsigned int iChannel , double fTemp );      //pig 2011.12.21 ATC改
        void SetRunATC();                                                       //pig 2011.12.21 ATC改
        bool SetStopATC();                                                      //pig 2011.12.21 ATC改
        bool SetChillerTemperature( unsigned int fTemp );                       //pig 2011.12.21 ATC改
        bool SetATCChillerSwitchOpen();                                         //pig 2011.12.21 ATC改
        bool SetATCChillerSwitchClose();                                        //pig 2011.12.21 ATC改
        bool SetATCCloseMessage();                                              //Ifor 20160223
        void SetTemperaturePLimit( double fTemp );  // 2012.02.02 , Joye , ATC改
        void GetNowWorkTemperature(unsigned int iChannel);                      //Steven 20120528 : 確認工作溫度
        void GetNowSiteOnOff(unsigned int iChannel=-1);                         //Steven 20120528 : 確認開關Site
        void GetNowChillerTemperature(unsigned int iChannel);                   //Steven 20120528 : 確認冰水機溫度
        void GetConnectState(unsigned int iChannel);
        bool SetNowArm(int iArm);                                               //2013-12-26    Dell    設定目前哪一手臂用溫控

        bool IsOnLine()                                 {return _online;};
        bool IsATCRun()                                 {return _ATCRun;};          //pig 2011.12.21 ATC改
        bool IsChillerRun()                             {return _Chiller_Run;};     // 2012.05.07 , Joye , Chiller
        bool ChillerStatus()                            {return _Chiller_Status;};  //Steven 20120530
        void SetChillerStatus(bool bStatus)             {_Chiller_Status=bStatus;}; //Steven 20120530
        void SetATCRun(bool bRunning)                   {_ATCRun=bRunning;};        //pig 2011.12.21 ATC改
        void SetPIDParameter( double fMinP , double fMinI , double fMinD , double fMaxP , double fMaxI , double fMaxD );    //Ifor 20150910 ATC PID

        bool GetNowATCSwVer();                          //Ifor 20151023 Add ATC 2.0 讀取ATC軟體版本
        bool SetATCTimeSync();                          //Ifor 20151023 Add ATC 2.0 設定ATC電腦時間同步
        bool Set2ndTempFunction(bool bEnable);          //Ifor 20160427 Add ATC 2.0 設定第二點Sensor Function On/Off
        bool SetATCSelfTest();                          //Ifor 20160720 Add ATC 2.0 執行自我測試模式
        bool GetNowATCStatus();                         //Ifor 20160801 Add ATC 2.0 讀取ATC狀態
        bool SetATCManualSelfTest();                    //Ifor 20160823 add Manual Send ATC Self Test Command
        bool GetNowSelfTestStatus();                    //Ifor 20160801 Add ATC 2.0 讀取ATC狀態
        void LotStart(AnsiString sLotID);               //Ifor 20161118 add ATC2.0 Lot Start Command for KYEC (ATCSystem::)
        void LotEnd(AnsiString sLotID);                 //Ifor 20161118 add ATC2.0 Lot End Command for KYEC (ATCSystem::)
};
// -----------------------------------------------------------------------------
//2014-05-30    Dell    for ATC6.0
class ATC60System
{
    private:
        AnsiString ErrorMessage;
        bool bStartCount;
        int iSocket;

    protected:
        bool _online;
    public:
    ATC60System();
    ~ATC60System();
    bool Connected;
    bool Connecting;

    enum ATCCommandIndex{ATCENABLED = 0,
                         SETTEMP,
                         USESITE,
                         SETOFFSET,
                         HANDLERARM,
                         TESTSTART,
                         GETTEMP,
                         LOADRECIPE,        //Steven 20150810 : add for ATC

                         Max_Command};      //ChungHung 20141105 add for ATC

    vector <AnsiString> vReciveBuf;
    vector <AnsiString> vSendBuf;
    HThreadCtrl _socket_thread;
    TClientSocket *clientsocket;
    AnsiString sCmd[Max_Command];
    AnsiString rCmd[Max_Command];
    AnsiString sCmdBackUp[Max_Command];
    TStringList *sList;
    TStringList *sTjList;
    TStringList *sTjLogList;
    vector<double > vTjTemp[16];
    double HandlerOffset[16];
    double TjOffset[16];
    double ReadTemp[tcTotalCount];  //ChungHung 20141105 add for ATC6.0 & 3.0
    double SendTemp[tcTotalCount];  //ChungHung 20141105 add for ATC6.0 & 3.0
    double SetTemp;                 //ChungHung 20141105 add for ATC6.0 & 3.0
    AnsiString UseSite;             //ChungHung 20141105 add for ATC6.0 & 3.0
    bool EnableATC;                 //ChungHung 20141105 add for ATC6.0 & 3.0
    int HandlerArm;                 //ChungHung 20141105 add for ATC6.0 & 3.0
    int TestStart;                  //ChungHung 20141105 add for ATC6.0 & 3.0
    AnsiString  FileName;

    bool bSend[Max_Command];   //送出去的命令ATC6.0
    bool bReciveOK[Max_Command];  //接收到的命令ATC6.0

    void ClientSocket1Connect(TObject *Sender,
          TCustomWinSocket *Socket);
    void ClientSocket1Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void ClientSocket1Error(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void clientsocketRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void NewSocket(); //ChungHung 20141211 add

    void SetCommand();
    void SetATCRun(bool bRunning);                                              //open/close ATC control
    void SetTargetTemperature(double fTarget);                                  //Send TEMP to ATC
    void LoadRecipe(AnsiString sFileName);                                      //Steven 20150810 : add for ATC
    void SetChannelUse(AnsiString bUse, int iSize);                             //Send Use Site state to ATC
    void SetTargetOffsetTemperature(double *fOffset, int iSize);                //Send Temp Offset to ATC    //ChungHung 20141105 add for ATC6.0 & 3.0 //void DoATC60Temperature()
    void SetHandlerNowArm(int iArm);                                            //Send now use Arm to ATC
    void SendTestStart(int iStart);                                             //Send Start Test to ATC
    void GetATCTemperature(AnsiString aTemp, int iSize);                        //Get Temp form ATC          //ChungHung 20141105 add for ATC6.0 & 3.0 //void DoATC60Temperature()
    void GetRealTemp();
    void SendCommand();

    void ReSendCommand(ATCCommandIndex iCommand);

    //ChungHung 20141105 add for ATC6.0 & 3.0
    bool GetErrorMessage(AnsiString &Error)
    {
        if(ErrorMessage=="")
            return false;
        Error=ErrorMessage;
        ErrorMessage="";
        return true;
    };

    void DoCommmand();
    bool IsConnected();  //ChungHung 20141021 add
    bool IsConnecting();

    static void _socket_execute( LPVOID Param )
    {
        ATC60System* lpTest = (ATC60System*) Param;

        MySleepEx(100,true);
        lpTest->SendCommand();
        lpTest->DoCommmand();
    };
    void OpenSocket();
    void CloseSocket();
    void ATCResetParm();

    void CheckCommand(int iCommand); //ChungHung 20141212 add
    void SetTjOffset(AnsiString buffer);
    void TransformTjData(AnsiString str);  //ChungHung 20150122 add for TSMC Use Tj
    void SaveTjLog(AnsiString Str);

    int iTimeLog;
    int iTimeLogEnd;
};
//---------------------------------------------------------------------------
#endif
