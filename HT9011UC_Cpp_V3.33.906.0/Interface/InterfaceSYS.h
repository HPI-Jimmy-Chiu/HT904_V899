//---------------------------------------------------------------------------
//  Interface/InterfaceSYS.h -- translated whole-file from golden reference
//    HT9011UC_Code_V3.33.906.0_20260618/Interface/InterfaceSYS.h (315 lines)
//
//  AI(W5-InterfaceSYS-Translate) 20260710: W5 (comms) wave, whole-file unit.
//  This module is the WM_COPYDATA IPC bridge between the Handler process and
//  four external helper programs: the GPIB/Tester bridge ("TSerialPoll"
//  window, see d:\GPIB9045 / gpib-settemp-respond-format memory), the ESD
//  program, the Auto Update program, and the Event Log Analyzer
//  ("TfrmELA"/"Event Log Analyzer" window).  It is ~0% VCL-WIDGET (no .dfm
//  control reads) but IS Win32-message-loop coupled: HWND / FindWindow /
//  SendMessage / WM_COPYDATA / COPYDATASTRUCT are all plain Win32 API, not
//  VCL, and come for free via vclcompat/vcl_compat.h's central
//  `#include <windows.h>` (see docs/KNOWLEDGE.md gotcha entry on the Win32
//  macro guard).  No new Win32 primitive was added to vclcompat for this
//  unit.
//---------------------------------------------------------------------------
#ifndef InterfaceSYSH
#define InterfaceSYSH

#include "vclcompat/vcl_compat.h"   // AnsiString + central <windows.h> (HWND/WPARAM/LPARAM/
                                    // COPYDATASTRUCT/WM_COPYDATA/FindWindow/SendMessage/WM_USER)
#include <vector>
#include <cstring>                  // memset (M_V ctor) -- golden pulled this in transitively
                                    // via <vcl.h>; explicit here since we no longer include it.

// AI(W5-InterfaceSYS-Translate) 20260710: `Byte` is a core Borland VCL/RTL typedef
// (System.hpp: `typedef unsigned char Byte;`) that golden pulls in transitively via
// <vcl.h>.  No prior translated unit needed it, so it is not yet centralized in
// vclcompat/vcl_compat.h.  Defined locally here, guarded, so a later central addition
// to vcl_compat.h does not collide (recommended to the integrate agent as a one-line
// promotion -- see this unit's translate report -- at which point this guard becomes
// an inert no-op; zero behavior change either way).
#ifndef HT9045_VCLCOMPAT_TYPE_BYTE_DEFINED
#define HT9045_VCLCOMPAT_TYPE_BYTE_DEFINED
typedef unsigned char Byte;
#endif

//---------------------------------------------------------------------------
#define WM_GPIBInterface WM_USER+100
#define WM_GPIB_Program     0
#define WM_ASEKH_Program    1
#define WM_CCD_Program      2
#define WM_OCR_Program      3
#define WM_ESD_Program      4
#define WM_AutoUpdate       5                                                   //Steven 20150728 : Auto Update
#define WM_EventAnalysis    6
#define WM_HeartBeat        8
//==========================================================================
#define TYPE_HANDLER_GPIB                    0x01
#define TYPE_HANDLER_ESD                      0x02
#define TYPE_HANDLER_AutoUpdate               0x03
#define TYPE_HANDLER_EventLog                 0x04
//==========================================================================
#define CommandType_NULL                     0x00
#define Command_NULL                         0x00
//==========================================================================
//_TEST 為組合方法使用
#define CommandType_TEST                     0x01

#define CommandType_TEST_REAL                0x01
#define CommandType_TEST_DOUBLE              0x02
#define CommandType_TEST_TIMEOUT             0x04
//==========================================================================
#define CommandType_TESTECHO                 0x02

#define CommandType_TESTECHOOK               0x01
//==========================================================================
#define CommandType_CLOSE                    0x03
//==========================================================================
#define CommandType_INFSEND                  0x04
#define CommandType_INFSEND_SENDGPIBADDR     0x01
#define CommandType_INFSEND_SENDTESTMODE     0x02
#define CommandType_INFSEND_SENDBINPASSDEF   0x03
//==========================================================================
#define CommandType_ECHOBACK                 0x05
#define CommandType_ECHOBACK_BACKGPIBADDR    0x01

//==========================================================================
#define CommandType_BARCODE                  0x06                               //2012.06.01 Q_Q

//==========================================================================
#define CommandType_ESD                      0x07                               //Steven 20140521 : ESD
#define CommandType_AutoUpdate               0x08                               //Steven 20150728 : Auto Update
#define CommandType_EventLog                 0x09                               //Steven 20231127 : 改用分析器

//******************************************************************************
//
//  注意!! ESD_COMMAND為Handler與ESD通訊相關, 修改時要小心!!
//
//******************************************************************************
enum ESD_COMMAND
{
    ESD_NORMAL=0,
    ESD_STATION1_ALARM=1,
    ESD_STATION2_ALARM=2,
    ESD_STATION3_ALARM=3,
    ESD_PROX1_1_ALARM=4,
    ESD_PROX1_2_ALARM=5,
    ESD_PROX1_3_ALARM=6,
    ESD_PROX2_1_ALARM=7,
    ESD_PROX2_2_ALARM=8,
    ESD_PROX2_3_ALARM=9,
    ESD_PROX3_1_ALARM=10,
    ESD_PROX3_2_ALARM=11,
    ESD_PROX3_3_ALARM=12,
    ESD_STATION1_COMERR=13,
    ESD_STATION2_COMERR=14,
    ESD_STATION3_COMERR=15,
    ESD_STATION1_SET_ERR=16,
    ESD_STATION2_SET_ERR=17,
    ESD_STATION3_SET_ERR=18,
    ESD_STATION1_SET_ZERO_ERR=19,
    ESD_STATION2_SET_ZERO_ERR=20,
    ESD_STATION3_SET_ZERO_ERR=21,
    ESD_STATION1_SET_GAIN_ERR=22,
    ESD_STATION2_SET_GAIN_ERR=23,
    ESD_STATION3_SET_GAIN_ERR=24,
    ESD_SYSTEM_START=25,
    ESD_SYSTEM_STOP=26,
    ESD_DECAY_TEST=27,
    ESD_DECAY_TIME_LIMIT=28,
    ESD_SYSTEM_CLOSE=29,                                                        //Frank 20150309 : 程式關閉後將ESD系統停止
    ESD_3M_STATION1_ALARM=30,                                                   //Ifor 20150724 :新增3M ESD Alarm上報與通訊異常上報
    ESD_3M_STATION2_ALARM=31,
    ESD_3M_STATION3_ALARM=32,
    ESD_3M_STATION1_COMERR=33,
    ESD_3M_STATION2_COMERR=34,
    ESD_3M_STATION3_COMERR=35,
    ESD_KASUGA_STATION1_ALARM=36,                                               //Ifor 20150731 :新增Kasuga Alarm上報與異常上報
    ESD_KASUGA_Connect_Error=37,
    ESD_STATION1_SET_DECAYTIME_ERR=38,                                          //Ifor 20150922 新增SIMCO Set Decay Time 錯誤訊息
    ESD_STATION2_SET_DECAYTIME_ERR=39,
    ESD_STATION3_SET_DECAYTIME_ERR=40,
    ESD_STATION1_READ_DECAYTIME_ERR=41,                                         //Ifor 20150922 新增SIMCO Read Decay Time 錯誤訊息
    ESD_STATION2_READ_DECAYTIME_ERR=42,
    ESD_STATION3_READ_DECAYTIME_ERR=43,
    ESD_STATION1_START_DECAYTEST_ERR=44,                                        //Ifor 20150922 新增SIMCO Start Decay Test 錯誤訊息
    ESD_STATION2_START_DECAYTEST_ERR=45,
    ESD_STATION3_START_DECAYTEST_ERR=46,
    ESD_STATION1_READ_DECAYTEST_ERR=47,                                         //Ifor 20150922 新增SIMCO Read Decay Test 錯誤訊息
    ESD_STATION2_READ_DECAYTEST_ERR=48,
    ESD_STATION3_READ_DECAYTEST_ERR=49,
    ESD_SIMCO_DECAYTEST_START=50,                                               //Ifor 20150922 新增SIMCO Decay Test Command
    ESD_SIMCO_DECAYTEST_Complete=51,
    ESD_STATION1_DECAY_ALARM=52,                                                //Ifor 20151021 新增 Decay Test Alarm
    ESD_STATION2_DECAY_ALARM=53,
    ESD_STATION3_DECAY_ALARM=54,
    ESD_ESD_Data=55,                                                            //kevin 20160106  傳送ESD 資料
    ESD_EP_Data=56,                                                             //kevin 20160106  傳送EP  資料
    ESD_Temper=57,                                                              //kevin 20160106   傳送溫度資料
    ESD_RECEIVE_ESD_DATA=58,                                                    //Ifor 20160305 Add ESD Program Send Data to Handler
    ESD_RECEIVE_DECAY_DATA=59,                                                  //Ifor 20160305 Add ESD Program Send Data to Handler
    ESD_REQUEST_ESD_DATA=60,                                                    //Ifor 20160316 Add ESD Program Send Data to Handler
    ESD_Read_ESD_SettDATA=61,                                                   //kevin 20160630 讀取ESD 設定資料檔
    ESD_Read_ESD_Power_ON=62,                                                   //kevin 20170310 (wei) 打開 Ion Bar 電源
    ESD_Read_ESD_Power_OFF=63,                                                  //kevin 20170310 (wei) 關閉 Ion Bar 電源
    ESD_Version=64,                                                             //Ifor 20170320 add Send ESD Version to Handler
    ESD_ANTENNA1_RELAY_ON=65,                                                   //Jeffrey 20180502 add Send ESD Antenna1 2 Change Switch
    ESD_ANTENNA1_RELAY_OFF=66,                                                  //Jeffrey 20180502 add Send ESD Antenna1 2 Change Switch
    ESD_ANTENNA2_RELAY_ON=67,                                                   //Jeffrey 20180502 add Send ESD Antenna1 2 Change Switch
    ESD_ANTENNA2_RELAY_OFF=68,                                                  //Jeffrey 20180502 add Send ESD Antenna1 2 Change Switch
    ESD_ANTENNA3_RELAY_ON=69,                                                   //Jeffrey 20180502 add Send ESD Antenna1 2 Change Switch
    ESD_ANTENNA3_RELAY_OFF=70,                                                  //Jeffrey 20180502 add Send ESD Antenna1 2 Change Switch
    ESD_ANTENNA4_RELAY_ON=71,                                                   //Jeffrey 20180502 add Send ESD Antenna1 2 Change Switch
    ESD_ANTENNA4_RELAY_OFF=72,                                                  //Jeffrey 20180502 add Send ESD Antenna1 2 Change Switch
    ESD_IONBAR_ALARM_1=73,
    ESD_IONBAR_ALARM_2=74,
    ESD_IONBAR_ALARM_3=75,
    ESD_IONBAR_ALARM_4=76,
    ESD_IONBAR_ALARM_5=77,
    ESD_IONBAR_ALARM_6=78,
    ESD_IONBAR_ALARM_7=79,
    ESD_IONBAR_ALARM_8=80,
    ESD_StartAutoCalibration=81,
    ESD_TemperatureAmbient=82,
    ESD_TemperatureHot=83,
    ESD_TemperatureCold=84,
    ESD_IONBAR_AirOn=85,
    ESD_IONBAR_AirOff=86,
    ESD_IONBAR_AirCheck=87,
    ESD_TemperatureSuperHot=88,
    ESD_TestOnLine=89,                                                          //kevin 20180816  測試機是否連線生產
    ESD_TestOFFLine=90,                                                         //kevin 20180816  測試機是否連線生產
    ESD_Check_IonBar_Balance_Req=91,                                            //Hmy 20231024 Add//Hmy 20231016 Add 機台OneCycle後需要等待超過設定時間才可以運行
    ESD_Check_IonBar_Balance_Rec_OK=92,                                         //Hmy 20231024 Add//Hmy 20231016 Add 機台OneCycle後需要等待超過設定時間才可以運行
    ESD_Check_IonBar_Balance_Rec_NG=93,                                         //Hmy 20231024 Add//Hmy 20231016 Add 機台OneCycle後需要等待超過設定時間才可以運行
    ESD_Check_IonBar_Balance_SetRange1=94,                                      //kevin 20231025 add banance over 40 -40
    ESD_Check_IonBar_Balance_SetRange2=95,                                      //kevin 20231025 add banance over 40 -40
    ESD_Check_IonBar_Balance_SetRange3=96,                                      //kevin 20231025 add banance over 40 -40
    ESD_Check_IonBar_Balance_SetRange4=97,                                      //kevin 20231025 add banance over 40 -40
    ESD_Check_IonBar_Balance_SetRange5=98,                                      //kevin 20231025 add banance over 40 -40
    ESD_Check_IonBar_Balance_SetRange6=99,                                      //kevin 20231025 add banance over 40 -40
    ESD_Check_IonBar_Balance_SetRange7=100,                                     //kevin 20231025 add banance over 40 -40
    ESD_Check_IonBar_Balance_SetRange8=101,                                     //kevin 20231025 add banance over 40 -40
    ESD_HT_IONBAR_ALARM_1=102,
    ESD_HT_IONBAR_ALARM_2=103,
    ESD_HT_IONBAR_ALARM_3=104,
    ESD_HT_IONBAR_ALARM_4=105,
    ESD_HT_IONBAR_STATION1_ALARM=106,
    ESD_HT_IONBAR_STATION2_ALARM=107,
    ESD_HT_IONBAR_STATION3_ALARM=108,
    ESD_HT_IONBAR_STATION4_ALARM=109,
    ESD_HT_IONBAR_Controller1_PowerOn=110,                                      //Hmy 20240630 Add HT Ion Bar Controller Power
    ESD_HT_IONBAR_Controller2_PowerOn=111,                                      //Hmy 20240630 Add HT Ion Bar Controller Power
    ESD_HT_IONBAR_Controller3_PowerOn=112,                                      //Hmy 20240630 Add HT Ion Bar Controller Power
    ESD_HT_IONBAR_Controller4_PowerOn=113,                                      //Hmy 20240630 Add HT Ion Bar Controller Power
    ESD_HT_IONBAR_Controller1_PowerOff=114,                                     //Hmy 20240630 Add HT Ion Bar Controller Power
    ESD_HT_IONBAR_Controller2_PowerOff=115,                                     //Hmy 20240630 Add HT Ion Bar Controller Power
    ESD_HT_IONBAR_Controller3_PowerOff=116,                                     //Hmy 20240630 Add HT Ion Bar Controller Power
    ESD_HT_IONBAR_Controller4_PowerOff=117,                                     //Hmy 20240630 Add HT Ion Bar Controller Power
    ESD_KASUGA_Fan_DECAYTEST_START=118,                                         //KaiChen 20191225 ：KASUGA Fan 通訊
    ESD_KASUGA_Fan_DECAYTEST_Complete=119,                                      //KaiChen 20191225 ：KASUGA Fan 通訊
    ESD_KASUGA_Fan_STATION1_DECAY_ALARM=120,                                    //KaiChen 20191225 ：KASUGA Fan 通訊
    ESD_KASUGA_Fan_STATION2_DECAY_ALARM=121,                                    //KaiChen 20191225 ：KASUGA Fan 通訊

    ESD_COMMAND_TOTAL
};
//******************************************************************************
//
//  注意!! ESD_COMMAND為Handler與ESD通訊相關, 修改時要小心!!
//
//******************************************************************************
enum AutoUpdate_COMMAND                                                         //Steven 20150728 : Auto Update
{
    AU_HANDLER_RUNNING          =0,
    AU_HAS_IC_UNDER_MACHINE     =1,
//    AU_CLEAN_OUT_HANDLER        =2,
//    AU_CLEAN_OUT_FINISH         =3,
    AU_CLOSE_HANDLER            =4,
//    AU_HANDLER_RUN              =5,

    AU_COMMAND_TOTAL
};
//******************************************************************************
enum EventLog_COMMAND
{
    EL_UPDATE_PARAMETER      =0,
    EL_UPLOAD_JAMWEEK        =1,                                                //Jimmychiu 20240925 : Add the jam log and upload it to FTP once a week
    EL_UPLOAD_CHIPADV_LOTEND =2,
    EL_UPLOAD_SUMMARY        =3,                                                //Jimmychiu 20240925 : Add the jam log and upload it to FTP once a week
    EL_UPLOAD_EVENTLOG       =4,                                                //Jimmychiu 20240925 : Add the jam log and upload it to FTP once a week
    EL_UPLOAD_BYFILE_N10     =5,
    EL_VTEST_MTBF_SUM        =6,                                                //RogerYang 20251104 : 偉測MTBF文件生成
    EL_COMMAND_TOTAL
};

using namespace std;
//******************************************************************************
//
//  注意!! struct M_V為Handler與ESD通訊相關, 修改時要小心!!
//
//******************************************************************************
struct M_V
{
    Byte bModeType;
    Byte bCommandType;
    Byte bCommand;
    unsigned int  iDataSize;
    Byte bData[256];
    unsigned int  iMessageSize;
    //Byte bMessage[256];
    Byte bMessage[2048];
    M_V();
    char cSendData[3000];                                                       //kevin 20160106
};
//===========================================================================
// AI(W5-InterfaceSYS-Translate) 20260710: golden defines this ctor body directly
// in the header (InterfaceSYS.h), and the header is #included by 18 different
// golden .cpp translation units.  Without `inline`, that is a hard multiple-
// definition link error under standard C++ ODR rules; BCB6's ILink32 tolerates
// duplicate identical header-defined function bodies across TUs (a known
// Borland-linker leniency this codebase relies on elsewhere), but a portable
// g++/MSVC toolchain does not.  Added `inline` here -- pure porting adaptation
// (same category as neutralizing __fastcall/__property elsewhere in vclcompat),
// zero behavior change.
inline M_V::M_V()
{
    bModeType     = 0x00 ;
    bCommandType  = 0x00 ;
    bCommand      = 0x00 ;
    iDataSize     = 0;
    iMessageSize  = 0;
    memset(bData,    0x00, sizeof(bData));
    memset(bMessage, 0x00, sizeof(bMessage));
};
//===========================================================================
//void M_V::InitM_V()
//{
//    bModeType     = 0x00 ;
//    bCommandType  = 0x00 ;
//    bCommand      = 0x00 ;
//    iDataSize     = 0;
//    iMessageSize  = 0;
//    memset(bData,    0x00, sizeof(bData));
//    memset(bMessage, 0x00, sizeof(bMessage));
//};
//============================================================================
class GPIBInterface_Data
{
    private:
    protected:
        M_V            *pMV ;
        COPYDATASTRUCT *pcp ;
        AnsiString FormName ;
        AnsiString CaptionName;
        HWND    _HVisionWnd;
        WPARAM  _WPARAM_TO;
    public:
        Byte bModeType;
        Byte bCommandType;
        Byte bCommand;
        vector <Byte> vData;
        vector <Byte> vMessage;
        //==============================================
        GPIBInterface_Data(AnsiString aFormName, AnsiString aCaptionName, WPARAM WPARAM_TO);
        ~GPIBInterface_Data();
        void Clear();
        bool bFindProgram();
        void PushData(int  iDatain[], int iSize);
        void PushData(bool bDatain[], int iSize);
        void PushMessage(int  iMessagein[]          , int iSize);
        void PushMessage(bool bMessagein[]          , int iSize);
        void PushMessage(Byte ByteMessagein[]       , int iSize);
        void PushMessage(vector <Byte> vMessagein   , int iSize);
        void SendStructMessage_SendMessage();
        //==============================================
};
//kevin 20180606 (Steven) : Set ESD DATA
struct ESD_SetData
{
    Byte bModeType;
    Byte bCommandType;
    Byte bCommand;
    bool Alarm_Enable;                                                          //超出設定值機台停機ALARM
    int ScanInterval;                                                           //讀取時間
    int Decay_Permissible_Time;                                                 //離子消散時間 ms
    int iStationActive[3];                                                      //模組單體使用偵測
    int iProx_Alarm[3][3];                                                      //ESD 偵測ALARM 值
    int iProx_On_Off[3][3];
    char cPwd[20];
};
// AI(W5-InterfaceSYS-Translate) 20260710: golden signature is
// `void _OnMyCopyMsg_Interface(TMessage &msg)`.  TMessage is a VCL message-map
// type (Borland BEGIN_MESSAGE_MAP/MESSAGE_HANDLER machinery); vclcompat has no
// TMessage shim (not needed anywhere else yet).  The golden body only ever
// reads `msg.LParam`, so -- extract-calc-core style -- the signature here takes
// that LPARAM directly.  Whoever wires up the real WM_COPYDATA handler on the
// translated TfMain (W7) should call `_OnMyCopyMsg_Interface(msg.LParam)`
// instead of passing the whole TMessage.  See translate report for the exact
// ask.
void _OnMyCopyMsg_Interface(LPARAM lParam);
bool _RunTestProgram(int iDataSize, bool *bSiteOpen);
bool _RunTestProgram_BarMess(int iDataSize, bool *bSiteOpen, int iMessageSize, Byte *bMessage);
void _SendADDRToInterfaceProgram(int iADDR);
void _SendInformationToInterfaceProgram(Byte _BModeType, Byte _bCommandType, Byte _bCommand, int _iData[], int _iDataSize, int _iMessage[]=0, int _iMessageSize=0);
void _SendStructMessage_Send(Byte bType,Byte bSendCommandType,Byte bSendCommand,vector <Byte> vData,vector <Byte> vMessage);
void SendCommand_ESD(ESD_COMMAND CMD);
void SendCommand_EventLog(EventLog_COMMAND CMD, AnsiString Data);               //Steven 20231127 : 改用分析器
void SendCommand_AutoUpdate(AutoUpdate_COMMAND CMD, AnsiString Data);           //Steven 20150728 : Auto Update
extern GPIBInterface_Data *GPIBSend;
extern M_V *MV_Event;
extern M_V *MV_ESD;
extern M_V *MV_AutoUpdate;                                                      //Steven 20150728 : Auto Update
#endif
