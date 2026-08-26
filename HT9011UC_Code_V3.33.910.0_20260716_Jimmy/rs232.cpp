#include "MachineDefine.h"
#pragma hdrstop

#include "rs232.h"

#include "myswitch.h"
#include "cContact.h"
#include "cpublic.h"
#include "cmydef.h"
#include "cprod.h"
#include "common.h"
#include "bthermo.h"
#include "mymessbox.h"
#include "note.h"
#include "MyMotor.h"
#include "cinitial.h"
#include "uLotInfo.h"
#include "cMyDB.h"
#include "database.h"
#include "uTemp_Set.h"
#include "mysensor.h"
#include "DynamicTemp.h"
#include "TrayStepMotor.h"
#include "uPadInterface.h"
#include "myTimer.h"
//----- by dell ccd realtime-------------
extern ScanBtn *ScanBtnThd;
//---------------------------------------
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
extern int g_iHeaterTypeIdx_SendCmd;                                            //AI(ht9045-heater-control) 20260618 (RogerYang) : 移植創發2type溫控模組設定
#pragma link "SPComm"
#pragma resource "*.dfm"
TCOM2 *COM2;

#define WT  100
#define iCom3 0
#define iCom4 1

bool bRWPanasonicParameterFlag;                                                 //True = Read ; False = Write
int  iReadPanasonicIndex;                                                       //0 = Index Arm 1 ; 1 = Index Arm 2

bool bReceive=false;
bool bReceive1=false;                                                           //KenHsieh 20220409 : nn Mode扭力紀錄
bool bHasInstallCOM3=true;

//---------------------------------------------------------------------------
//Mitsubishi Driver Use  Eliot 2010_05_01
//---------------------------------------------------------------------------
int iReadTorqueTask=1;
int iWriteTorqueTask=1;
int iRead_Now_Torque_Task=1;
char _SOH=0x01, _STX=0x02, _ETX=0x03, _EOT=0x04;
char cSendMitsubishi_Data[32]="";
char cReciveMitsubishi_Data[32]="";

char cChnageIntToASCII[2]="";
char cMotorAddress;
char cCommand_1,cCommand_2;                                                     //命令.
char cDataNo_1,cDataNo_2;                                                       //命令指定資料.
char cWriteDataTitle[2];
char cNoData[8]="";                                                             //有的資料需附指定內容.
char cCheckSumTemp[3]="";
int iCheckSum=0;
double fReadMotorTorque;
double fWriteTorque;
double fRead_Now_TorqueP, fRead_Now_TorqueN;
TQPF_Timer hTimeOutLimit;
TDateTime tTimeTemp;

double fSetTorqueValue;
int iSetMitsubishiIndex;
int iReadMitsubishiIndex;
bool bsetTorque_Mitsubishi=false;
bool bReadTorque_Mitsubishi=false;
bool bRWActionFlag=false;

//jou 2012-03-14 ATC start:
TQPF_Timer ATCTimeOut[4];
const int ATC_READ_STATUS_AND_PROCESS_VARIABLE  =0x0000;
const int ATC_READ_FULL_STATUS_COMMAND          =0x0005;
const int ATC_READ_1SP1_VARIABLE                =0x0100;
const int ATC_WRITE_1SP1_COMMAND                =0x0200;
const int ATC_WRITE_2SP1_COMMAND                =0x0201;
const int ATC_WRITE_SP2_COMMAND                 =0x0204;
const int ATC_WRITE_A1LO_COMMAND                =0x0205;
const int ATC_WRITE_A1HI_COMMAND                =0x0206;
const int ATC_WRTIE_INPC_COMMAND                =0x024E;
const int ATC_SET_FOR_REMOTE_MODE               =0x0400;
const int ATC_SET_FOR_LOCAL_MODE                =0x0401;
//jou 2012-03-14 ATC end
const int OpenRTCComPortAgainMaxCount=2;                                        //ChungHung 20140520 add 不要第一次TimeOut就秀錯誤訊息

//---------------------------------------------------------------------------
__fastcall TCOM2::TCOM2(TComponent* Owner)
    : TDataModule(Owner)
{
    bsetTorque_Mitsubishi=false;
    bReadTorque_Mitsubishi=false;

    Com2Buffer="";                                                              //ChungHung 20121030 add

    INDEX_DRIVER_TYPE=CheckAndReadIniDataGeneral("IndexDriver", "INDEX_DRIVER_TYPE", Panasonic_DRIVER);
    TorqueUseHPComCard=CheckAndReadIniDataGeneral("IndexDriver", "USE_HP_COM_CARD", false); //Steven 20210204 : 使用鴻勁自製的通訊卡

    iPanasonicDriverType=Panasonic_DRIVER;                                      // A4
    if(INDEX_DRIVER_TYPE==Panasonic_DRIVER_A5)                                  // 2011.08.11 , Joye , Panasonic A5 //Steven 20120629 add from 7045--------
    {
        INDEX_DRIVER_TYPE=Panasonic_DRIVER;
        iPanasonicDriverType=Panasonic_DRIVER_A5;                               // A5
    }
    fPanasonicParameterRW=false;
    flagCommSD[0]=false;
    flagCommSD[1]=false;
    flagCommSD[2]=false;
    flagCommSD[3]=false;
    InitReadTorueTask();
    bRTCVerSupportAutoTurnning=false;                                           //Sam 20240425 : 啟動時傳送 AutoTeach 功能，若沒有回應代表不支援就不要顯示。
    rwCommandDelay=5;                                                           //ChungHung 20140610 modify all Type use 0.5S

    iOpenRTCComPortAgainCount=0;                                                //ChungHung 20140520 add 不要第一次TimeOut就秀錯誤訊息
    _byte_datas.clear();                                                        //Steven 20210524 : 連續讀取扭力
    bGetValue=false;                                                            //KenHsieh 20220317 : 確認有Get value後Test Arm才能上升
    bGetValue1=false;                                                           //KenHsieh 20220409 : nn Mode扭力紀錄
}
//---------------------------------------------------------------------------
bool RS232zipShowFlag ;
bool Comm2ReceiveFlag;
bool BIBTestOkFlag;
bool Comm2BusyFlag;
bool Rs232Comm2Busy;
//---------------------------------------------------------------------------
#define QUE_LEN 4096
typedef struct tagQue
{
    int Read;
    int Write;
    Byte ptr[QUE_LEN];
}Que;
static Que RxQue;
//---------------------------------------------------------------------------
BOOL InitQue(Que *que)
{
    que->Read=0;
    que->Write=0;
    return TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::RS232Init()
{
    int iErr=0;
    bool flag[11];
    ZeroMemory(flag, sizeof(flag));
    AnsiString Str;                                                             //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    flag[0]=GetCOMPortStatus(HSys.sTorqueComPort);                              //Steven 20120217 : Com Port改成可定義

    if(flag[0]==false)
    {
        iErr=GetLastError();
        ShowMyMessage("Index Torque : "+HSys.sTorqueComPort+" port error , "+GetErrorMessage(iErr), "");
    }

    //if(IsValEqual_HeaterInsOpt(g_iHeaterTypeIdx_SendCmd, NoHeater))             //AI(ht9045-heater-control) 20260618 (RogerYang) : 移植創發2type溫控模組設定  //Steven 20171227 (Wei) : Add for HT-9045L
    if(IsNoHeaterMachine())                                                     //RogerYang 20260630 : Add NoHeater 判斷
    {
        flag[1]=false;
        flag[2]=false;
    }
    else
    {
        flag[1]=GetCOMPortStatus(HSys.sTempComPort);
        if(flag[1]==false)
            ShowMyMessage("Temperature KT4H : "+HSys.sTempComPort+" port error", "");

        if(USE_16_HEATER==eht16HeaterEJ1N ||
           USE_16_HEATER==eht32HeaterEJ1N ||
           USE_16_HEATER==eht32HeaterDTME08)                                    //Steven 20140923 : Index使用EJ1N版32組加熱器
        {
            flag[2]=GetCOMPortStatus(HSys.sTempOmronComPort);
            if(flag[2]==false)
                ShowMyMessage("Temperature EJ1N :"+HSys.sTempOmronComPort+" port error", "");
        }
    }

    if(REAL_TIME_CCD==true)
    {
        flag[3]=GetCOMPortStatus(HSys.sRTCComPort);
        if(flag[3]==false)
            ShowMyMessage("Real Time CCD : "+HSys.sRTCComPort+" port error", "");
    }

    if(IniConfig.bC04EnableTestTempIC==true)
    {
        flag[4]=GetCOMPortStatus(HSys.sTempDynamicComPort);
        if(flag[4]==false)
            ShowMyMessage("Temperature IC : "+HSys.sTempDynamicComPort+" port error", "");
    }

#ifdef SOFT_SIMULTE
    flag[5]=false;
    flag[6]=false;
    flag[7]=false;
    flag[8]=false;
#else
    if(ATC_SYSTEM==eATCSiliconType)                                             //jou 2012-03-08 Enable ATC mode
    {
        flag[5]=GetCOMPortStatus(HSys.asATC1ComPort);
        if(flag[5]==false)
            ShowMyMessage("ATC 1 : "+HSys.asATC1ComPort+" port error", "");
        flag[6]=GetCOMPortStatus(HSys.asATC2ComPort);
        if(flag[6]==false)
            ShowMyMessage("ATC 2 : "+HSys.asATC2ComPort+" port error", "");
        flag[7]=GetCOMPortStatus(HSys.asATC3ComPort);
        if(flag[7]==false)
            ShowMyMessage("ATC 3 : "+HSys.asATC3ComPort+" port error", "");
        flag[8]=GetCOMPortStatus(HSys.asATC4ComPort);
        if(flag[8]==false)
            ShowMyMessage("ATC 4 : "+HSys.asATC4ComPort+" port error", "");
    }
#endif

    if(LoaderUnload_StepMotor==true ||                                          //Steven 20200529 : Loader入Tray改步進
       iControlPanelMode)
    {
        if(iControlPanelMode && LoaderUnload_StepMotor)                         //KenHsieh 20211222 : Pad與步進馬達為同一Comport
            Str="Tray Step Motor & Control Panel : ";
        else if(LoaderUnload_StepMotor)
            Str="Tray Step Motor : ";
        else if(iControlPanelMode)
            Str="Control Panel : ";

        flag[9]=GetCOMPortStatus(HSys.TrayStepMotor_ComPort);                   //KenHsieh 20211222 : Pad與步進馬達為同一Comport
        if(flag[9]==false)
            ShowMyMessage(Str+HSys.TrayStepMotor_ComPort+" port error", "");    //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    }

    if(flag[0])
    {
        COM2->Comm1->CommName="\\\\.\\"+HSys.sTorqueComPort;
        try
        {
            //Eliot 2010_05_01
            if(INDEX_DRIVER_TYPE==Mitsubishi_DRIVER)
                COM2->Comm1->Parity=Even;
            else
                COM2->Comm1->Parity=None;

            if(TorqueUseHPComCard)                                              //Steven 20210204 : 使用鴻勁自製的通訊卡
                COM2->Comm1->BaudRate=115200;
            else
                COM2->Comm1->BaudRate=9600;
            COM2->Comm1->ByteSize=_8;
            COM2->Comm1->ParityCheck=false;
            COM2->Comm1->StopBits=_1;

            COM2->Comm1->StartComm();                                           //僅能啟動一次
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TCOM2::RS232Init");
            ShowMyMessage("Index Torque : "+HSys.sTorqueComPort+" port error", "");
        }
    }

    if(flag[1] && USE_NEW_TEMPCTRL_FUNCTION==false)
    {
        COM2->Comm2->CommName="\\\\.\\"+HSys.sTempComPort;
        COM2->Comm2->Parity=None;
        COM2->Comm2->BaudRate=9600;
        COM2->Comm2->ByteSize=_8;
        COM2->Comm2->ParityCheck=false;
        COM2->Comm2->StopBits=_1;
        try
        {
            COM2->Comm2->StartComm();                                           //僅能啟動一次
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TCOM2::RS232Init");
            ShowMyMessage("Temperature KT4H : "+HSys.sTempComPort+" port error", "");
        }
    }

    if(flag[3] && REAL_TIME_CCD==true)                                          //----- by dell ccd realtime-------------
    {
        COM2->Comm4->CommName="\\\\.\\"+HSys.sRTCComPort;
        try
        {
            COM2->Comm4->StartComm();
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TCOM2::RS232Init");
            ShowMyMessage("Real Time CCD : "+HSys.sRTCComPort+" port error", "");
        }

        CCDReceiveBuffer="";
//        bCCDDummyRum=false;
        for(int i=0; i<CCD_MAX_CMD; i++)
        {
            bRealTimeCom_ReceiveOK[i]=false;
        }

      //Send-------------------------------------------------
        sRealTimeCom_Send[rtHome]               = "@ALIGN+";                    //Handler 通知編碼器座標歸零
        sRealTimeCom_Send[rtInspStart]          = "@START+";                    //Handler 通知啟動檢測
        sRealTimeCom_Send[rtInspStartROI]       = "@START+";                    //Handler 通知啟動ROI檢測
        sRealTimeCom_Send[rtInspStartGolden]    = "@START+";                    //Handler 通知啟動Full View檢測
        sRealTimeCom_Send[rtInspStartUnKnow]    = "@START+";                    //Handler 通知啟動

        sRealTimeCom_Send[rtInspVerifyOK]       = "@VERIFY+";                   //Handler Use Model Verify
        sRealTimeCom_Send[rtInspVerifyNG]       = "@VERIFY+";                   //Handler Use Model Verify
        sRealTimeCom_Send[rtROICount]           = "@ROICOUNT";                  //jou 2013-11-25 Handler ROI COUNT

        sRealTimeCom_Send[rtInspEnd]            = "@END+";                      //Handler 通知關閉檢測
        sRealTimeCom_Send[rtArmIndex1]          = "@ARM1+";                     //Handler Index Arm 1 準備下壓
        sRealTimeCom_Send[rtArmIndex2]          = "@ARM2+";                     //Handler Index Arm 2 準備下壓
        sRealTimeCom_Send[rtFileOK]             = "@FILE+";                     //Handler 通知換料檔名
        sRealTimeCom_Send[rtAlarHasIC]          = "@ALARM+";
        sRealTimeCom_Send[rtAlarGrabTimeOut]    = "@ALARM+";
        sRealTimeCom_Send[rtClear]              = "@CLEAR+";                    //Handler通知結批完成
        sRealTimeCom_Send[rtRelease]            = "@RELEASE+";                  //Handler通知狀況排除 , Vision關閉Error Message
        sRealTimeCom_Send[rtRsOK]               = "@RSOK+";
        sRealTimeCom_Send[rtCHECKNULL]          = "@CHECKNULL+";                //Ifor 20190708 : add RTC 置偏殘料檢查命令
        sRealTimeCom_Send[rtCHECKIC]            = "@CHECKIC+";                  //Ifor 20190708 : add RTC 置偏殘料檢查命令
        sRealTimeCom_Send[rtTESTMAP]            = "@TESTMAP+";                  //Ifor 20190722 : add RTC 開關Site檢測

        //"@FULLL+"---
        //Handler確認建立 Full ViewModel時機 , 通知Vision 新建或重建Model
        sRealTimeCom_Send[rtFullLOK]            = "@FULLL+";
        sRealTimeCom_Send[rtFullLNG]            = "@FULLL+";
        //------------

        //"@FULLT+"---
        //Handler 在Has IC發生後將IndexArm分開並通知Vision進行全景檢測 , 用以確認Has IC真實發生的位置
        sRealTimeCom_Send[rtFullTOK]            = "@FULLT+";
        sRealTimeCom_Send[rtFullTNG]            = "@FULLT+";
        //------------

        sRealTimeCom_Send[rtPassOn]             = "@PASSON+";
        sRealTimeCom_Send[rtReall]              = "@REALL+";
        sRealTimeCom_Send[rtPassOff]            = "@PASSOFF+";
        sRealTimeCom_Send[rtMODEL]              = "@MODEL+";
        sRealTimeCom_Send[rtReallNG]            = "@REALLNG+";                  //有IC掉落要重新Learning

        sRealTimeCom_Send[rtLightReal]          = "@LIGHTREAL+";                //RTC檢查用的燈光
        sRealTimeCom_Send[rtLightBox]           = "@LIGHTBOX+";                 //一般的燈光
        sRealTimeCom_Send[rtLightOn]            = "@LIGHTON+";                  //開燈
        sRealTimeCom_Send[rtLightOff]           = "@LIGHTOFF+";                 //關燈

        //ChungHung 20130122 add for SCK
        sRealTimeCom_Send[rtFullMOK]            = "@FULLM+";                    //單獨詢問 Full Modal 是否存在
        sRealTimeCom_Send[rtFullMNG]            = "@FULLM+";
        sRealTimeCom_Send[rtHalfMOK]            = "@HALFM+";                    //單獨詢問 Half Modal 是否存在
        sRealTimeCom_Send[rtHalfMNG]            = "@HALFM+";

        //Handle 更新 Site 資訊------
        //1 x 2      @SITE1+
        //1 x 4      @SITE2+
        //2 x 2      @SITE3+
        //2 x 4      @SITE4+
        //2 x 8      @SITE5+
        //----------------------------
        sRealTimeCom_Send[rtSite]               = "@SITE+";

        sRealTimeCom_Send[rtArmFinish]          = "@ARMFINISH+";                //JerryYang 20220215 : 修正ArmFinish指令錯誤
        sRealTimeCom_Send[rtVISIONON]           = "@VISIONON+";                 //確認Vision有被開啟
        sRealTimeCom_Send[rtDELETE]             = "@DELETE+";                   //換工作檔時要砍掉Golden sample
        sRealTimeCom_Send[rtAlarEncode]         = "@ALARM-ENCODE+";             //ChungHung 20111110  alarm-encode
        sRealTimeCom_Send[rtReLoad]             = "@RELOAD+";                   //ChungHung 20120106  RTC Reload File
        sRealTimeCom_Send[rtArmWork]            = "@ARMWORK+";                  //查詢Index Arm工作次數 ,Vision System會回 '@ARMWORK__+' __ 代表次數 00 ~ 20

        sRealTimeCom_Send[rtSiteMap]            = "@MAP";                       //jou 2014-05-06 add RTC site map

        sRealTimeCom_Send[rtOPENVERIFYOK]       ="@OPENVERIFY1+";               //jou 2014-06-24 RTC 自動進行Model驗證
        sRealTimeCom_Send[rtOPENVERIFYNG]       ="@OPENVERIFY2+";               //jou 2014-06-24 RTC 自動進行Model驗證
        sRealTimeCom_Send[rtALLPASSOK]          ="@ALLPASS+";                   //jou 2014-06-24 RTC 自動進行Model驗證
        sRealTimeCom_Send[rtALLPASSNG]          ="@ALLPASS+";                   //jou 2014-06-24 RTC 自動進行Model驗證
        sRealTimeCom_Send[rtALLFAILOK]          ="@ALLFAIL+";                   //jou 2014-06-24 RTC 自動進行Model驗證
        sRealTimeCom_Send[rtALLFAILNG]          ="@ALLFAIL+";                   //jou 2014-06-24 RTC 自動進行Model驗證
        sRealTimeCom_Send[rtCLOSEVERIFY]        ="@CLOSEVERIFY+";               //jou 2014-06-24 RTC 自動進行Model驗證

        sRealTimeCom_Send[rtROICheckOK]         ="@ROICHECK+";                  //wei 20170308 (jou) RTC ROI Check
        sRealTimeCom_Send[rtTIMESYNC]           ="@TIMESYNC+";                  //Ifor 20181127 : add RTC TIME SYNC
        sRealTimeCom_Send[rtOpenLive]           ="@OPENLIVE+";                  //jou 20180420 : RTC Open/Close Live
        sRealTimeCom_Send[rtCloseLive]          ="@CLOSELIVE+";                 //jou 20180420 : RTC Open/Close Live

        sRealTimeCom_Send[rtROIsize]            ="@SIZE+";                      //jou 20180724 : 新增RTC ROI size command
        sRealTimeCom_Send[rtArm1AllPassVerify]  ="@ARM1ALLPASS+";              //JerryYang 20220215 : RTC Auto Verify half view check
        sRealTimeCom_Send[rtArm2AllPassVerify]  ="@ARM2ALLPASS+";

        sRealTimeCom_Send[rtArm1AllFailVerify]  ="@ARM1ALLFAIL+";
        sRealTimeCom_Send[rtArm2AllFailVerify]  ="@ARM2ALLFAIL+";

        sRealTimeCom_Send[rtFullGrabOK]             ="@FULLGB+";                //Sam 20230419 : 新增 RTC Auto Tuning 功能
        sRealTimeCom_Send[rtFullGrabDummyBlackOK]   ="@FULLGBB+";
        sRealTimeCom_Send[rtFullGrabDummyWhiteOK]   ="@FULLGBW+";
        sRealTimeCom_Send[rtFullGrabRealICOK]       ="@FULLGBIC+";
        sRealTimeCom_Send[rtLightAutoTuningOK]      ="@LIGHTTUN+";
        sRealTimeCom_Send[rtParamAutoTuningOK]      ="@PARTUN+";
        sRealTimeCom_Send[rtFullAllCheckOK]         ="@FULLALL+";

        sRealTimeCom_Send[rtAUTOTEACHON]        = "@AUTOTEACHMODE+";            //Ifor 20230908 add:RTC 讓位功能 By Setup File
        sRealTimeCom_Send[rtAUTOTEACHOFF]       = "@NONAUTOTEACHMODE+";
        sRealTimeCom_Send[rtSTEPASIDEMODEON]    = "@STEPASIDEMODE+";
        sRealTimeCom_Send[rtSTEPASIDEMODEOFF]   = "@NONSTEPASIDEMODE+";

        sRealTimeCom_Send[rtAUTOSTDICPARATUNE]       ="@AUTOSTDICPARATUNE+";    //JerryYang 20240829 : SPIL訓永 要求手動觸發RTC AUTO STD

        sRealTimeCom_Send[rtErrorType]          ="@ErrorType+";               //wei 20221222 RTC ARM Error
        sRealTimeCom_Send[rtALARMArm1NG]        ="@ALARM-Arm1NG+";            //wei 20221222 RTC ARM Error
        sRealTimeCom_Send[rtALARMArm2NG]        ="@ALARM-Arm2NG+";            //wei 20221222 RTC ARM Error
        sRealTimeCom_Send[rtCheckThirdPos]      ="@CHECKTHIRDPOS+";           //wei 20221222 RTC ARM Error
        //Recevie-------------------------------------------------
        sRealTimeCom_Receive[rtNODEVICE]        = "@NODEVICE+";                 //沒有Full View Golden Sample
        sRealTimeCom_Receive[rtNOMODEL]         = "@NOMODEL+";                  //沒有Full View Golden Sample
        sRealTimeCom_Receive[rtGETMODEL]        = "@GETMODEL+";                 //更新Full View Golden Sample
        sRealTimeCom_Receive[rtMODEL]           = "@MODEL+";                    //取得Full View Golden Sample

        sRealTimeCom_Receive[rtHome]            = "@ALIGN+";                    //Handler 通知編碼器座標歸零完成
        sRealTimeCom_Receive[rtInspStart]       = "@STARTOK+";                  //Vision 回應可正常啟動
        sRealTimeCom_Receive[rtInspStartROI]    = "@STARTROI+";                 //Real Time Start時,沒有ROI資料
        sRealTimeCom_Receive[rtInspStartGolden] = "@STARTGOLDEN+";              //Real Time Start時,沒有Golden Sample
        sRealTimeCom_Receive[rtInspStartUnKnow] = "@STARTUNKNOWN+";             //Real Time Start時,有未知的異常
        sRealTimeCom_Receive[rtStartLightF]     = "@STARTLIGHTF+";              //燈光錯誤
        sRealTimeCom_Receive[rtInspStartROIF]   = "@STARTROIF+";                //Full View Start時,沒有ROI資料
        sRealTimeCom_Receive[rtInspStartGoldenF]= "@STARTGOLDENF+";             //Full View Start時,沒有Golden Sample
        sRealTimeCom_Receive[rtEncodeFail]      = "@STARTENCODEF+";             //Feedback檢查錯誤
        sRealTimeCom_Receive[rtPosFail]         = "@STARTPOSF+";                //Trigger間距檢查錯
        sRealTimeCom_Receive[rtStartRepeat]     = "@STARTREPEAT+";              //Handler前一道 @START+ 命令還沒處理完 ,又收到同一個命令

        sRealTimeCom_Receive[rtInspVerifyOK]    = "@VERIFYOK+";                 //Handler Use Model Verify
        sRealTimeCom_Receive[rtInspVerifyNG]    = "@VERIFYNG+";                 //Handler Use Model Verify
        sRealTimeCom_Receive[rtNeedVerify]      = "@NEEDVERIFY+";               //jou 2014-06-24 RTC 自動進行Model驗證 start
        sRealTimeCom_Receive[rtROICount]        = "@ROICOUNT+";                 //jou 2013-11-25 Handler ROI COUNT
        sRealTimeCom_Receive[rtROICountOK]      = "@ROICOUNTOK+";
        sRealTimeCom_Receive[rtROICountNG]      = "@ROICOUNTNG+";

        sRealTimeCom_Receive[rtArmWork]         = "@ARMWORK";                   //查詢Index Arm工作次數 ,Vision System會回 '@ARMWORK__+' __ 代表次數 00 ~ 20

        sRealTimeCom_Receive[rtInspEnd]         = "@END+";                      //Vision 進入 IDLE 狀態
        sRealTimeCom_Receive[rtArmIndex1]       = "@ARM1+";                     //Vision 啟動TEST Y1 Trigger
        sRealTimeCom_Receive[rtArmIndex2]       = "@ARM2+";                     //Vision 啟動TEST Y2 Trigger
        sRealTimeCom_Receive[rtFileOK]          = "@FILEOK+";                   //Vision 自動建檔並通知換料成功
        sRealTimeCom_Receive[rtCHECKNULL]       = "@CHECKNULL+";                //Ifor 20190708 : add RTC 置偏殘料檢查命令
        sRealTimeCom_Receive[rtCHECKIC]         = "@CHECKIC+";                  //Ifor 20190708 : add RTC 置偏殘料檢查命令
        sRealTimeCom_Receive[rtTESTMAP]         = "@TESTMAP+";                  //Ifor 20190722 : add RTC 開關Site檢測

        //"@ALARM-HASIC"---
        //Vision 偵測殘留IC通知Handler錯誤型態 , Show error message並透過 NET 傳送檢測圖到網
        //路磁碟機(S):\\FrontCCD1.bmp,FrontCCD2.bmpRearCCD1.bmp ,RearCCD1.bmpHandler讀檔完成後刪圖
        //Handler 收到Vision Alarm 訊息進入排除狀態 :
        //  (1)關閉RealTime檢測功能
        //  (2)FULL VIEW RECHECK
        //  (3)發現殘留IC , 進行排除工作
        //  (4)解除Vision Alarm狀態
        //  (5)啟動檢測功能
        //  (6)Handler 恢復動作
        //-----------------
        sRealTimeCom_Receive[rtAlarHasIC]       = "@ALARM-HASIC+";
        sRealTimeCom_Receive[rtAlarGrabTimeOut] = "@ALARM-GRABTIMEOUT+";        //Vision Grab Timeout通知Handler錯誤型態
        sRealTimeCom_Receive[rtAlarEncode]      = "@ALARM-ENCODE+";             //ChungHung 20111110  alarm-encode
        sRealTimeCom_Receive[rtClear]           = "@CLEAR+";                    //Vision 製作檢測報表
        sRealTimeCom_Receive[rtRelease]         = "@RELEASE+";                  //解除Error Message , 進入關閉檢測功能狀態

        sRealTimeCom_Receive[rtRsOK]            = "@RSOK+";
        sRealTimeCom_Receive[rtFullLOK]         = "@FULLLOK+";                  //Vision 進行Full View Auto Training 並自動產生ROI
        sRealTimeCom_Receive[rtFullLNG]         = "@FULLLNG+";

        //"@FULLTNG+"---
        //全景檢測發現IC當全景檢測發現IC時 , Vision程式透過NET傳送檢測圖到網路
        //磁碟機(S):\\CCD1.bmp , CCD2.bmpHandler讀檔完成後刪圖
        //-----------------
        sRealTimeCom_Receive[rtFullTOK]         = "@FULLTOK+";                  //全景檢測未發現異常
        sRealTimeCom_Receive[rtFullTNG]         = "@FULLTNG+";

        //"@REALL+"----
        //Vision 進入 Training 模式,Handler Index Arm 自動切換20次觸發取圖Handler 通知自動學習結束
        sRealTimeCom_Receive[rtReall]           = "@REALL+";
        //-----------------

        sRealTimeCom_Receive[rtPassOn]          = "@PASSON+";                   //Vision 程式設定為ByPass狀態
        sRealTimeCom_Receive[rtPassOff]         = "@PASSOFF+";                  //Vision 程式設定為Non-ByPass狀態

        sRealTimeCom_Receive[rtSite]            = "@SITEOK+";                   //Vison 接收命後更新 Site 資訊
        sRealTimeCom_Receive[rtArmFinish]       = "@ARMFINISH+";                //JerryYang 20220215 : 修正ArmFinish指令錯誤

        sRealTimeCom_Receive[rtVISIONON]        = "@VISIONON+";                 //Vision程式開啟
        sRealTimeCom_Receive[rtVISIONOFF]       = "@VISIONOFF+";                //Vision程式關閉
        sRealTimeCom_Receive[rtDELETE]          = "@DELETE+";                   //換工作檔時要砍掉Golden sample
        sRealTimeCom_Receive[rtReallNG]         = "@REALLNG+";                  //有IC掉落要重新Learning
        sRealTimeCom_Receive[rtReLoad]          = "@RELOAD+";                   //ChungHung 20120106  RTC Reload File
        sRealTimeCom_Receive[rtVisionStop]      = "@VISIONSTOP+";               //RTC被按下暫停

        //jou 2012-03-29 RTC啟動時,自動將燈箱打開
        sRealTimeCom_Receive[rtLightOn]         = "@LIGHTON+";                  //開燈
        sRealTimeCom_Receive[rtLightOff]        = "@LIGHTOFF+";                 //關燈

        //ChungHung 20130122 add for SCK
        sRealTimeCom_Receive[rtFullMOK]         = "@FULLMOK+";
        sRealTimeCom_Receive[rtFullMNG]         = "@FULLMNG+";
        sRealTimeCom_Receive[rtHalfMOK]         = "@HALFMOK+";
        sRealTimeCom_Receive[rtHalfMNG]         = "@HALFMNG+";

//jou 2014-05-06 add RTC site map
//       Vision 回應碼(Handler必須收到回碼才可繼續工作 , timeout時間設定5 sec) :
//       @MAPOK+           轉換成功
//       @MAPCODENG+       傳碼錯誤
//       @MAPCOUNTNG+      ROI 設定數量錯誤
        sRealTimeCom_Receive[rtSiteMap]            = "@MAP";

        sRealTimeCom_Receive[rtOPENVERIFYOK]    ="@OPENVERIFYOK+";              //jou 2014-06-24 RTC 自動進行Model驗證
        sRealTimeCom_Receive[rtOPENVERIFYNG]    ="@OPENVERIFYNG+";              //jou 2014-06-24 RTC 自動進行Model驗證
        sRealTimeCom_Receive[rtALLPASSOK]       ="@ALLPASSOK+";                 //jou 2014-06-24 RTC 自動進行Model驗證
        sRealTimeCom_Receive[rtALLPASSNG]       ="@ALLPASSNG+";                 //jou 2014-06-24 RTC 自動進行Model驗證
        sRealTimeCom_Receive[rtALLFAILOK]       ="@ALLFAILOK+";                 //jou 2014-06-24 RTC 自動進行Model驗證
        sRealTimeCom_Receive[rtALLFAILNG]       ="@ALLFAILNG+";                 //jou 2014-06-24 RTC 自動進行Model驗證
        sRealTimeCom_Receive[rtCLOSEVERIFY]     ="@CLOSEVERIFYOK+";             //jou 2014-06-24 RTC 自動進行Model驗證

        sRealTimeCom_Receive[rtROICheckOK]      ="@ROICHECKOK+";                //wei 20170308 (jou) RTC ROI Check
        sRealTimeCom_Receive[rtROICheckNG]      ="@ROICHECKNG+";                //wei 20170308 (jou) RTC ROI Check
        sRealTimeCom_Receive[rtModelNG]         ="@MODELNG+";                   //wei 20170504 (Steven) RTC Model NG
        sRealTimeCom_Receive[rtTIMESYNC]        ="@TIMESYNCOK+";                //Ifor 20181127 : add RTC TIME SYNC
        sRealTimeCom_Receive[rtOpenLive]        ="@OPENLIVEOK+";                //jou 20180420 : RTC Open/Close Live
        sRealTimeCom_Receive[rtCloseLive]       ="@CLOSELIVEOK+";               //jou 20180420 : RTC Open/Close Live

        sRealTimeCom_Receive[rtROIsize]         ="@SIZE+";                      //jou 20180724 : 新增RTC ROI size command

        sRealTimeCom_Receive[rtArm1AllPassVerify]      ="@ARM1ALLPASS+";        //JerryYang 20220215 : RTC Auto Verify half view check
        sRealTimeCom_Receive[rtArm2AllPassVerify]      ="@ARM2ALLPASS+";

        sRealTimeCom_Receive[rtArm1AllFailVerify]      ="@ARM1ALLFAIL+";
        sRealTimeCom_Receive[rtArm2AllFailVerify]      ="@ARM2ALLFAIL+";

        sRealTimeCom_Receive[rtFullGrabOK]             ="@FULLGBOK+";           //Sam 20230419 : 新增 RTC Auto Tuning 功能
        sRealTimeCom_Receive[rtFullGrabNG]             ="@FULLGBNG+";
        sRealTimeCom_Receive[rtFullGrabDummyBlackOK]   ="@FULLGBBOK+";
        sRealTimeCom_Receive[rtFullGrabDummyBlackNG]   ="@FULLGBBNG+";
        sRealTimeCom_Receive[rtFullGrabDummyWhiteOK]   ="@FULLGBWOK+";
        sRealTimeCom_Receive[rtFullGrabDummyWhiteNG]   ="@FULLGBWNG+";
        sRealTimeCom_Receive[rtFullGrabRealICOK]       ="@FULLGBICOK+";
        sRealTimeCom_Receive[rtFullGrabRealICNG]       ="@FULLGBICNG+";
        sRealTimeCom_Receive[rtLightAutoTuningOK]      ="@LIGHTTUNOK+";
        sRealTimeCom_Receive[rtLightAutoTuningNG]      ="@LIGHTTUNNG+";
        sRealTimeCom_Receive[rtParamAutoTuningOK]      ="@PARTUNOK+";
        sRealTimeCom_Receive[rtParamAutoTuningNG]      ="@PARTUNNG+";
        sRealTimeCom_Receive[rtFullAllCheckOK]         ="@FULLALLOK+";
        sRealTimeCom_Receive[rtFullAllCheckNG]         ="@FULLALLNG+";

        sRealTimeCom_Receive[rtAUTOTEACHON]            ="@AUTOTEACHMODE+";      //Sam 20240425 : 啟動時傳送 AutoTeach 功能，若沒有回應代表不支援就不要顯示。
        sRealTimeCom_Receive[rtAUTOTEACHOFF]           ="@NONAUTOTEACHMODE+";
        sRealTimeCom_Receive[rtSTEPASIDEMODEON]        ="@STEPASIDEMODEOK+";
        sRealTimeCom_Receive[rtSTEPASIDEMODEOFF]       ="@NONSTEPASIDEMODEOK+";
        sRealTimeCom_Receive[rtAUTOSTDICPARATUNE]      ="@AUTOSTDICPARATUNEOK+";//JerryYang 20240829 : SPIL訓永 要求手動觸發RTC AUTO STD
        sRealTimeCom_Receive[rtAUTOSTDICPARATUNENG]    ="@AUTOSTDICPARATUNENG+";
        sRealTimeCom_Receive[rtErrorType]       ="@ErrorType+";               //wei 20221222 RTC ARM Error
        sRealTimeCom_Receive[rtALARMArm1NG]     ="@ALARM-Arm1NG+";            //wei 20221222 RTC ARM Error
        sRealTimeCom_Receive[rtALARMArm2NG]     ="@ALARM-Arm2NG+";            //wei 20221222 RTC ARM Error
        sRealTimeCom_Receive[rtCheckThirdPos]   ="@CHECKTHIRDPOSOK+";         //wei 20221222 RTC ARM Error
    }
    //---------------------------------------

    if(flag[4] && IniConfig.bC04EnableTestTempIC)                               //jou 980420 add temp ic test
    {
        COM2->TempComm6->CommName="\\\\.\\"+HSys.sTempDynamicComPort;
        try
        {
            COM2->TempComm6->StartComm();                                       //僅能啟動一次
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TCOM2::RS232Init");
            ShowMyMessage("Temperature IC : "+HSys.sTempDynamicComPort+" port error", "");
        }
    }

    if(flag[5] && ATC_SYSTEM==eATCSiliconType)                                  //jou 2012-03-08 Enable ATC mode
    {
        COM2->cmATC1->CommName="\\\\.\\"+HSys.asATC1ComPort;
        try
        {
            COM2->cmATC1->StartComm();                                          //僅能啟動一次
            iATCProcessTask[tcAa1]=0;
            iATCInitialTask[tcAa1]=1;
            bATCInitialFinish=false;
            bATCInitialOK[0]=false;
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TCOM2::RS232Init");
            ShowMyMessage("ATC 1 : "+HSys.asATC1ComPort+" port error", "");
        }
    }

    if(flag[6] && ATC_SYSTEM==eATCSiliconType)                                  //jou 2012-03-08 Enable ATC mode
    {
        COM2->cmATC2->CommName="\\\\.\\"+HSys.asATC2ComPort;
        try
        {
            COM2->cmATC2->StartComm();                                          //僅能啟動一次
            iATCProcessTask[tcAb1]=0;
            iATCInitialTask[tcAb1]=1;
            bATCInitialFinish=false;
            bATCInitialOK[1]=false;
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TCOM2::RS232Init");
            ShowMyMessage("ATC 2 : "+HSys.asATC2ComPort+" port error", "");
        }
    }

    if(flag[7] && ATC_SYSTEM==eATCSiliconType)                                  //jou 2012-03-08 Enable ATC mode
    {
        COM2->cmATC3->CommName="\\\\.\\"+HSys.asATC3ComPort;
        try
        {
            COM2->cmATC3->StartComm();                                          //僅能啟動一次
            iATCProcessTask[tcAa2]=0;
            iATCInitialTask[tcAa2]=1;
            bATCInitialFinish=false;
            bATCInitialOK[2]=false;
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TCOM2::RS232Init");
            ShowMyMessage("ATC 3 : "+HSys.asATC3ComPort+" port error", "");
        }
    }

    if(flag[8] && ATC_SYSTEM==eATCSiliconType)                                  //jou 2012-03-08 Enable ATC mode
    {
        COM2->cmATC4->CommName="\\\\.\\"+HSys.asATC4ComPort;
        try
        {
            COM2->cmATC4->StartComm();                                          //僅能啟動一次
            iATCProcessTask[tcAb2]=0;
            iATCInitialTask[tcAb2]=1;
            bATCInitialFinish=false;
            bATCInitialOK[3]=false;
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TCOM2::RS232Init");
            ShowMyMessage("ATC 4 : "+HSys.asATC4ComPort+" port error", "");
        }
    }

    iErr=0;
    bVisionLightCommOK=false;                                                   //AI(appexception-av-log-flood) 20260617 (RogerYang) : 進入前先設未開啟
    bool bVisionLightflag;
    AnsiString asMsgTmp;
    bVisionLightflag=GetCOMPortStatus(HSys.asVisionLightPort);                  //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
    if(USE_Fix_AI_CCD==0)                                                       //Steven 20260423 : add protection
    {
        bVisionLightflag=false;
    }

    if(bVisionLightflag==false)
    {
        iErr=GetLastError();
        asMsgTmp.sprintf("Vision RS232 : %s port error , %s", HSys.asVisionLightPort, GetErrorMessage(iErr));
        ShowMyMessage(asMsgTmp);
    }

    if(bVisionLightflag)
    {
        cmVisionLight->CommName="\\\\.\\"+HSys.asVisionLightPort;
        try
        {
            cmVisionLight->Parity=None;
            cmVisionLight->BaudRate=9600;
            cmVisionLight->ByteSize=_8;
            cmVisionLight->ParityCheck=false;
            cmVisionLight->StopBits=_1;

            cmVisionLight->StartComm();                                         //僅能啟動一次
            bVisionLightCommOK=true;                                            //AI(appexception-av-log-flood) 20260617 (RogerYang) : 開啟成功才允許後續寫入
        }
        catch(...)
        {
            bVisionLightCommOK=false;                                           //AI(appexception-av-log-flood) 20260617 (RogerYang) : 開啟失敗，禁止寫入
            asMsgTmp.sprintf("Vision RS232 : %s port error", HSys.asVisionLightPort);
            ShowMyMessage(asMsgTmp);
        }
    }

    Rs232Comm2Busy=false;
    InitQue(&RxQue);
    if(TorqueUseHPComCard)                                                      //kevin 20210824
        TimerHPCard->Enabled=true;                                              //Steven 20210524 : 連續讀取扭力
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::InitRTCComPortAgain()                                    //ChungHung 20140520 add 不要第一次TimeOut就秀錯誤訊息
{
    iOpenRTCComPortAgainCount=0;
}
//---------------------------------------------------------------------------
bool __fastcall TCOM2::OpenRTCComPortAgain()                                    //ChungHung 20121005 add
{
    bool ret=false;
    iOpenRTCComPortAgainCount++;                                                //ChungHung 20140520 add 不要第一次TimeOut就秀錯誤訊息

    Comm4->StopComm();
    MySleep(10);
    if(GetCOMPortStatus(AnsiString(Comm4->CommName)))
    {
        Comm4->StartComm();
        MySleep(10);
    }
    else
    {
        ShowMyMessage("RTC"+AnsiString(Comm4->CommName)+" port error", "");
    }

    if(iOpenRTCComPortAgainCount>OpenRTCComPortAgainMaxCount)                   //ChungHung 20140520 add 不要第一次TimeOut就秀錯誤訊息
    {
        InitRTCComPortAgain();                                                  //Steven 20161206 (wei) : fixed for RTC time out error
        ret=true;
    }

    return ret;
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::Comm2ReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    if(InitialOK==false)                                                        //Steven 20120202 : 加入Thread保護
        return;

    if(BufferLength<7)
        return;

    byte *data;
    data=(byte *)Buffer;

    if(BufferLength>250)
       BufferLength=250;

    if(IsValEqual_HeaterInsOpt(g_iHeaterTypeIdx_SendCmd, TC401))                //AI(ht9045-heater-control) 20260618 (RogerYang) : 移植創發2type溫控模組設定  //Steven 20141030 : 新增OMRON E5DC溫控器
    {
        for(int i=0; i<8; i++)
            READBUFF[i]=data[i];
    }
    else if(IsValEqual_HeaterInsOpt(g_iHeaterTypeIdx_SendCmd, E5DC))            //AI(ht9045-heater-control) 20260618 (RogerYang) : 移植創發2type溫控模組設定  //Steven 20141030 : 新增OMRON E5DC溫控器
    {
        Com2Buffer.sprintf("%s", (char*)Buffer);                                //Steven 20111028 : 改成AnsiString
    }
    else
    {
        if(data[0]!=':')
        {
            return;
        }
        else
        {
            Com2Buffer.sprintf("%s", (char*)Buffer);                            //Steven 20111028 : 改成AnsiString
        }
    }
    Com2ReceiveOK=true;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//馬達收到的資料
//----------------------------------------------------------------------------
#define MOTOR_MIN_WAIT  10                                                      //2008/07/15 lee
int iPanasonicWT=500;                                                           //Sam 20200507 : 400>500
static int ptreot;
static int ptrenq;
void __fastcall TCOM2::TorqueSend(unsigned char *str, int len)
{
    AnsiString S="Send    ";
    static char str2[256];
    Comm1->WriteCommData(str, len);
    for(int i=0; i<len; i++)
    {
        sprintf(str2," %02X", (unsigned)str[i]);
        S+=AnsiString(str2);
    }
    fMain->AddTorqueLog(S);
    bReceive=false;
}
//----------------------------------------------------------------------------
void TCOM2::ReadIndexTorqueSetting(int Index)                                   //Steven 20210223 : 整合Torque存取
{
    if(TorqueUseHPComCard)                                                      //Steven 20210204 : 使用鴻勁自製的通訊卡
        ReadIndexTorqueSetting_HPCard(Index);
    else if(INDEX_DRIVER_TYPE==Panasonic_DRIVER)                                //Panasonic
        ReadIndexTorqueSetting_Pana(Index);
    else                                                                        //Mitsubishi
        ReadIndexTorqueSetting_Mitu(Index);
}
//----------------------------------------------------------------------------
void TCOM2::WriteIndexTorqueSetting(int Index, AnsiString Torque)               //Steven 20210223 : 整合Torque存取
{
    if(TorqueUseHPComCard)                                                      //Steven 20210204 : 使用鴻勁自製的通訊卡
        WriteIndexTorqueSetting_HPCard(Index, atoi(Torque.c_str()));
    else if(INDEX_DRIVER_TYPE==Panasonic_DRIVER)                                //Panasonic
        WriteIndexTorqueSetting_Pana(Index, atoi(Torque.c_str()));
    else                                                                        //Mitsubishi
        WriteIndexTorqueSetting_Mitu(Index, atof(Torque.c_str()));
}
//----------------------------------------------------------------------------
int iPanasonicNum;
int iPanasonicTask=1;
int iReadTorueIndex;

void TCOM2::InitReadTorueTask()
{
    if(INDEX_DRIVER_TYPE==Panasonic_DRIVER)                                     //Panasonic
        autoTask=1;
}
//----------------------------------------------------------------------------
void __fastcall TCOM2::ReadTorque()
{
    if(TorqueUseHPComCard)                                                      //Steven 20210204 : 使用鴻勁自製的通訊卡
        ReadTorque_HPCard();
    else if(INDEX_DRIVER_TYPE==Panasonic_DRIVER)                                //Panasonic
        ReadTorque_Panasonic();
    else
        ReadTorque_Mitsubishi();
}
//----------------------------------------------------------------------------
TQPF_Timer ReadTorque_PanasonicDelay;
void __fastcall TCOM2::ReadTorque_Panasonic()
{
    #ifdef SOFT_SIMULTE
    {
        return;
    }
    #else
    {
        static int Address;
        static int ct=0, iAlarmCT=0;
        static unsigned char ENQ[1]={0x05};                                     //請求傳送資料
        static unsigned char EOT[1]={0x04};                                     //同意對方可以傳送資料
        static unsigned char ACK[1]={0x06};                                     //確定收到正確資料
        unsigned char datatrq[4]={0x00, 0x00, 0x52, 0xAE};                      //PC對馬達所傳送的資料，要求讀取扭力值

        int &Task=autoTask;
        AnsiString asString;

        if(fPanasonicParameterRW)
            return;

        if(fMain->chkReadTorque1->Checked || fMain->chkReadTorque2->Checked)
        {
            if(fMain->chkReadTorque1->Checked)
            {
                iReadTorueIndex=0;
                SW[SwReadTorue].Off();
            }
            else
            {
                iReadTorueIndex=1;
                SW[SwReadTorue].On();
            }
        }
        else
        {
            Task=1;
            iAlarmCT=0;
            return;
        }

        if(iAlarmCT>10)                                                         //jou 981225 start : rs232 fail need alarm
        {
            if(fMain->chkReadTorque1->Checked)
                asString="Z1";
            else
                asString="Z2";
            ShowMyMessage("Rs232 Read Index "+asString+" Torque error!! maybe Relay or Com Port fail, please check .", "Rs232 讀取 Index "+asString+" 扭力錯誤!! 可能是Relay或是線材脫落,請確認.");

            Task=1;
            iAlarmCT=0;
        }

        switch(Task)
        {
            case 1:
                ReadTorque_PanasonicDelay.SetMSAndOn(100);
                Task=3;
//                break;
            case 3:
                if(ReadTorque_PanasonicDelay.Off())
                {
                    Address=iReadTorueIndex;
                    Task=5;
                }
                break;
            case 5:
                TorqueSend(ENQ, 1);                                             //告知馬達要求傳送資料
                bReceive=false;
                ct=0;
                Task=10;
                break;
            case 10:
                ct++;
                if(ct>iPanasonicWT)
                {
                    Task=1;
                    Comm1->StopComm();
                    Comm1->StartComm();
                    Torque[Address]=-9999;
                    iAlarmCT++;
                    break;
                }

                if(bReceive)                                                    //待馬達回應可以傳送資料
                {
                    iAlarmCT=0;
                    if(ptreot==EOT[0])
                    {
                        datatrq[1]=Address;
                        datatrq[3]=0xAE-Address;
                        TorqueSend(datatrq, 4);                                 //寫入資料，要求讀取扭力值
                        bReceive=false;
                        Task=20;
                        ct=0;
                    }
                    else
                    {
                        Task=1;
                        break;
                    }
                }
                break;
            case 20:
                ct++;
                if(ct>iPanasonicWT)
                {
                    Task=1;
                    Comm1->StopComm();
                    Comm1->StartComm();
                    Torque[Address]=-9999;
                    iAlarmCT++;
                    break;
                }

                if(bReceive)
                {
                    iAlarmCT=0;
                    if(ptreot==ACK[0] && ptrenq==ENQ[0])                        //待馬達告知PC要求將扭力值傳回
                    {
                        TorqueSend(EOT, 1);                                     //回應馬達可以將資料傳回
                        bReceive=false;
                        Task=30;
                        ct=0;
                    }
                    else
                    {
                        Task=1;
                        break;
                    }
                }
                break;
            case 30:
                ct++;
                if(ct>iPanasonicWT)
                {
                    Task=1;
                    Comm1->StopComm();
                    Comm1->StartComm();
                    Torque[Address]=-9999;
                    iAlarmCT++;
                    break;
                }

                if(bReceive)
                {
                    iAlarmCT=0;
                    if(ptreot==0x03)                                            //確定資料已收到
                    {
                        TorqueSend(ACK, 1);                                     //回應馬達已收到正確資料
                        Task=40;
                        ct=0;

                        if(fMain->chkReadTorque1->Checked)                      //jou 981016 start
                        {
                            fMain->edTorue0->Text=asReceiveTorue;
                            fContact->PnlTorue0->Caption=asReceiveTorue;
                        }
                        else if(fMain->chkReadTorque2->Checked)
                        {
                            fMain->edTorue1->Text =asReceiveTorue;
                            fContact->PnlTorue1->Caption=asReceiveTorue;
                        }
                    }
                    else
                    {
                        Task=1;
                        break;
                    }
                }
                break;
            case 40:
                Task=999;
                fMain->chkReadTorque1->Checked=false;
                fMain->chkReadTorque2->Checked=false;
                iAlarmCT=0;
                break;
        }
    }
    #endif
}
//----------------------------------------------------------------------------
void __fastcall TCOM2::ReadTorque_HPCard()                                      //Steven 20210204 : 使用鴻勁自製的通訊卡
{
    #ifdef SOFT_SIMULTE
    {
        return;
    }
    #else
    {
        AnsiString asString;
        static int Address;
        char str[256];
        static int iAlarmCT=0;
        int &Task=autoTask;

        if(fPanasonicParameterRW)
            return;

        if(IndexStatus==Z1_Z2_Down &&
           fMain->chkReadTorque1->Checked &&
           fMain->chkReadTorque2->Checked)                                      //KenHsieh 20220409 : nn Mode扭力紀錄
        {
            iReadTorueIndex=2;
        }
        else if(fMain->chkReadTorque1->Checked ||
                fMain->chkReadTorque2->Checked)
        {
            if(fMain->chkReadTorque1->Checked)
            {
                iReadTorueIndex=0;
            }
            else
            {
                iReadTorueIndex=1;
            }
        }
        else
        {
            Task=1;
            iAlarmCT=0;
            return;
        }

        if(iAlarmCT>10)                                                         //jou 981225 start : rs232 fail need alarm
        {
            if(fMain->chkReadTorque1->Checked)
                asString="Z1";
            else
                asString="Z2";

            ShowMyMessage("Rs232 Read Index "+asString+" Torque error!! maybe Relay or Com Port fail, please check .", "Rs232 讀取 Index "+asString+" 扭力錯誤!! 可能是線材脫落,請確認.");
            Task=1;
            iAlarmCT=0;
        }

        switch(Task)
        {
            case 1:
                ReadTorque_PanasonicDelay.SetMSAndOn(100);
                Task=3;
            case 3:
                if(ReadTorque_PanasonicDelay.Off())
                {
                    Address=iReadTorueIndex;
                    ReadTorque_PanasonicDelay.SetSecAndOn(TestIF_File.dSendGetValueDelayTime);       //KenHsieh 20220317 : 延遲取值時間
                    Task=5;
                }
                break;
            case 5:
                if(ReadTorque_PanasonicDelay.Off())                             //KenHsieh 20220317 : 延遲取值時間
                {
                    bReceive=false;
                    bGetValue=false;                                            //KenHsieh 20220317 : 確認有Get value後Test Arm才能上升
                    if(IndexStatus==Z1_Z2_Down)                                 //KenHsieh 20220409 : nn Mode扭力紀錄
                        bGetValue1=false;
                    ZeroMemory(str, sizeof(str));

                    if(iReadTorueIndex==2)                                      //KenHsieh 20220409 : nn Mode扭力紀錄
                        sprintf(str, ":@00A000203%c@#", 0xC0);
                    else if(iReadTorueIndex==0)
                        sprintf(str, ":@00A000201%c@#", 0xC2);
                    else
                        sprintf(str, ":@00A000202%c@#", 0xC1);

                    fMain->AddTorqueLog(AnsiString("Send ")+AnsiString(str));
                    Comm1->WriteCommData(str, 15);
                    ReadTorque_PanasonicDelay.SetSecAndOn(3);
                    if(iReadTorueIndex==2)                                      //KenHsieh 20220409 : nn Mode扭力紀錄
                    {
                        bReadMCU1=true;
                        bReadMCU2=true;
                    }
                    else if(iReadTorueIndex==0)
                    {
                        bReadMCU1=true;                                         //kevin 20220225 read MCU DATA
                    }
                    else
                    {
                        bReadMCU2=true;                                         //kevin 20220225 read MCU DATA
                    }
                    Task=10;
                }
                break;
            case 10:
                if(ReadTorque_PanasonicDelay.Off())
                {
                    Comm1->StopComm();
                    Task=11;
                    break;
                }

                if(IndexStatus==Z1_Z2_Down)                                     //KenHsieh 20220409 : nn Mode扭力紀錄
                {
                    if(bReceive && bReceive1)
                    {
                        iAlarmCT=0;
                        Task=40;
                    }
                }
                else if(bReceive)
                {
                    iAlarmCT=0;
                    Task=40;
                }
                break;
            case 11:
                Task=1;
                Comm1->StartComm();
                if(IndexStatus==Z1Down_Z2Up)                                    //kevin 20211027  重新送資料
                {
                    fMain->chkReadTorque1->Checked=true;
                    fMain->chkReadTorque2->Checked=false;
                }
                else if(IndexStatus==Z1Up_Z2Down)
                {
                    fMain->chkReadTorque1->Checked=false;
                    fMain->chkReadTorque2->Checked=true;
                }
                else if(IndexStatus==Z1_Z2_Down)                                //KenHsieh 20220409 : nn Mode扭力紀錄
                {
                    fMain->chkReadTorque1->Checked=true;
                    fMain->chkReadTorque2->Checked=true;
                }

                bCom1Error=false;                                               //kevin 20210902 Torque error
                iAlarmCT++;
                break;
            case 40:
                Task=999;
                fPanasonicParameterRW=false;
                fMain->chkReadTorque1->Checked=false;
                fMain->chkReadTorque2->Checked=false;
                iAlarmCT=0;
                break;
        }
    }
    #endif
}
//----------------------------------------------------------------------------
TQPF_Timer HPCardTimer;
int __fastcall TCOM2::StartReadTorque_HPCard()                                  //Steven 20210524 : 連續讀取扭力
{
    int iResult=2;
    #ifdef SOFT_SIMULTE
    {
        iResult=1;
    }
    #else
    {
        static int iCount=0;
        AnsiString str="", sSendCmd;
        unsigned char Btmp1;
        int &Task=iReadTorque_HPCardTask;

        if(TorqueUseHPComCard==false)
        {
            iResult=1;
        }
        else
        {
            switch(Task)                                                        //Steven 20211116 : 改成一送一收
            {
                case 1:
                    iCount=0;
                    Task=100;
                case 100:
                    str.sprintf(":@00A800701START");
                    Btmp1=A_Create_LRC(str.c_str(), str.Length());
                    sSendCmd.sprintf("%s%c@#", str, Btmp1);
                    fMain->AddTorqueLog(AnsiString("Send ")+sSendCmd);
                    bTorueReceve=false;                                         //kevin 20211117 扭力100 筆資料
                    HPCardTimer.SetSecAndOn(2);
                    Comm1->WriteCommData(sSendCmd.c_str(), sSendCmd.Length());
                    Task=110;
                case 110:
                    if(bTorueReceve)                                            //kevin 20220818 change  bTorueReceve
                    {
                        iCount=0;
                        Task=2000;
                    }
                    else if(HPCardTimer.Off())
                    {
                        iCount++;
                        if(iCount<2)
                        {
                            Task=100;                                           //Steven 20211116 : 暫時先直接Retry, 後續根據狀況修改例外
                        }
                        else                                                    //kevin 20211123 add MCU ALARM
                        {
                            iCount=0;
                            ShowErrorMessage("WAR03601", K_SKIP, MTestZ2, false, "");
                        }
                    }
                    break;
                case 2000:
                    str.sprintf(":@00A800702START");
                    Btmp1=A_Create_LRC(str.c_str(), str.Length());
                    sSendCmd.sprintf("%s%c@#", str, Btmp1);
                    fMain->AddTorqueLog(AnsiString("Send ")+sSendCmd);
                    bTorueReceve=false;
                    HPCardTimer.SetSecAndOn(2);
                    Comm1->WriteCommData(sSendCmd.c_str(), sSendCmd.Length());
                    Task=2100;
                case 2100:
                    if(bTorueReceve)                                            //kevin 20220818 (bRecvData)
                    {
                        iCount=0;
                        iResult=1;
                    }
                    else if(HPCardTimer.Off())
                    {
                        iCount++;
                        if(iCount<2)
                        {
                            Task=2000;                                          //Steven 20211116 : 暫時先直接Retry, 後續根據狀況修改例外
                        }
                        else                                                    //kevin 20211123 add MCU ALARM
                        {
                            iCount=0;
                            ShowErrorMessage("WAR03601", K_SKIP, MTestZ2, false,"");
                        }
                    }
                    break;
            }
        }
    }
    #endif
    return iResult;
}
//------------------------------------------------------------------------------
TQPF_Timer HPCardTimer1;
int __fastcall TCOM2::StopReadTorque_HPCard()                                   //kevin 20211207 change Type //Steven 20210524 : 連續讀取扭力
{
    int iResult=0;
    #ifdef SOFT_SIMULTE
    {
        iResult=1;
    }
    #else
    {
        static int iCount=0;
        AnsiString str="", sSendCmd;
        unsigned char Btmp1;
        int &Task=iReadSTopTorque_HPCardTask;                                   //Steven 20211116 : 改成一送一收

        if(TorqueUseHPComCard==false)
        {
            iResult=1;
        }
        else
        {
            switch(Task)                                                        //Steven 20211116 : 改成一送一收
            {
                case 1:
                    iCount=0;
                    Task=100;
               case 100:
                    str.sprintf(":@00A900601STOP");
                    Btmp1=A_Create_LRC(str.c_str(), str.Length());
                    sSendCmd.sprintf("%s%c@#", str, Btmp1);
                    fMain->AddTorqueLog(AnsiString("Send ")+sSendCmd);
                    bTorueReceveSTOP=false;                                     //kevin 20211117 扭力STOP資料
                    HPCardTimer1.SetSecAndOn(5);
                    Comm1->WriteCommData(sSendCmd.c_str(), sSendCmd.Length());
                    Task=110;
                case 110:
                    if(bTorueReceveSTOP)
                    {
                        Task=2000;
                    }
                    else if(HPCardTimer1.Off())
                    {
                        iCount++;
                        if(iCount <2)
                        {
                            Task=100;
                        }
                        else                                                    //kevin 20211123 add MCU ALARM
                        {
                            iCount=0;
                            ShowErrorMessage("WAR03602", K_SKIP, MTestZ1, false,"");
                        }
                    }
                    break;
                case 2000:
                    iCount=0;
                    Task=2100;
                case 2100:
                    str.sprintf(":@00A900602STOP");
                    Btmp1=A_Create_LRC(str.c_str(), str.Length());
                    sSendCmd.sprintf("%s%c@#", str, Btmp1);
                    fMain->AddTorqueLog(AnsiString("Send ")+sSendCmd);
                    bTorueReceveSTOP=false;                                     //kevin 20211117 扭力STOP資料
                    HPCardTimer1.SetSecAndOn(5);
                    Comm1->WriteCommData(sSendCmd.c_str(), sSendCmd.Length());
                    Task=2200;
                case 2200:
                    if(bTorueReceveSTOP)
                    {
                        iResult=1;
                    }
                    else if(HPCardTimer1.Off())
                    {
                        iCount++;
                        if(iCount<2)
                        {
                            Task=2100;
                        }
                        else                                                    //kevin 20211123 add MCU ALARM
                        {
                            iCount=0;
                            ShowErrorMessage("WAR03603", K_SKIP, MTestZ2, false,"");
                        }
                    }
                break;
            }
        }
    }
    #endif
    return iResult;
}
//----------------------------------------------------------------------------
void __fastcall TCOM2::ReadVer_HPCard()                                         //kevin 20211110 : 讀取韌體 版號
{
    #ifdef SOFT_SIMULTE
    {
        return;
    }
    #else
    {
        if(TorqueUseHPComCard==false)
            return;

        AnsiString str="", sSendCmd;
        unsigned char Btmp1;

        str.sprintf(":@0000003VERF@#");
        Btmp1=A_Create_LRC(str.c_str(), str.Length());
        sSendCmd.sprintf("%s%c@#", str, Btmp1);
        fMain->AddTorqueLog(AnsiString("Send ")+sSendCmd);
        Comm1->WriteCommData(sSendCmd.c_str(), sSendCmd.Length());
    }
    #endif
}
//---------------------------------------------------------------------------
unsigned char Panasonicdatatrq[20];
TQPF_Timer hPanasonicParameterTimeOut;
void TCOM2::ReadIndexTorqueSetting_Pana(int Index)
{
    if(Index==0)
        SW[SwReadTorue].Off();
    else
        SW[SwReadTorue].On();

    if(iPanasonicDriverType==Panasonic_DRIVER_A5)                               // 2011.08.11 , Joye , Panasonic A5 //Steven 20120629 add from 7045
    {
        iPanasonicNum=6;
        unsigned char szReadCommandStr[]={0x02, 0x00, 0x07, 0x00, 0x0D, 0x00};
        unsigned char sum=0;

        for(int i=0; i<5; i++)
            sum+=szReadCommandStr[i];

        sum=~sum+1;
        szReadCommandStr[5]=sum;

        for(int i=0; i<6; i++)
            Panasonicdatatrq[i]=szReadCommandStr[i];
    }
    else
    {
        iPanasonicNum=5;
        unsigned char szReadCommandStr[]={0x01, 0x00, 0x08, 0x5E, 0x00};
        unsigned char sum=0;

        szReadCommandStr[3]=0x5e;
        for(int i=0; i<4; i++)
            sum+=szReadCommandStr[i];

        sum=~sum+1;                                                             //checksum 二補數
        szReadCommandStr[4]=sum;
        for(int i=0; i<5; i++)
            Panasonicdatatrq[i]=szReadCommandStr[i];
    }
    fPanasonicParameterRW=true;
    hPanasonicParameterTimeOut.SetSecAndOn(10);
    bRWPanasonicParameterFlag=true;                                             //read
    iReadPanasonicIndex=Index;

    iPanasonicTask=1;                                                           //jou 2011-11-18重新initial Task
}
//----------------------------------------------------------------------------
bool bPanasonicCommErr=false;
void TCOM2::WriteIndexTorqueSetting_Pana(int Index, unsigned Data)
{
    unsigned char sum=0;

    if(Index==0)
        SW[SwReadTorue].Off();
    else
        SW[SwReadTorue].On();

    if(iPanasonicDriverType==Panasonic_DRIVER_A5)                               // 2011.08.11 , Joye , Panasonic A5 //Steven 20120629 add from 7045
    {
        iPanasonicNum=10;
        unsigned char szReadCommandStr[]={0x06, 0x00, 0x17, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00};

        szReadCommandStr[5]=Data%256;
        szReadCommandStr[6]=Data/256;
        szReadCommandStr[7]=0x00;
        szReadCommandStr[8]=0x00;

        for(int i=0; i<9; i++)
            sum+=szReadCommandStr[i];

        sum=~sum+1;
        szReadCommandStr[9]=sum;

        for(int i=0; i<10; i++)
            Panasonicdatatrq[i]=szReadCommandStr[i];
    }
    else
    {
        iPanasonicNum=7;
        unsigned char szReadCommandStr[]={0x03, 0x00, 0x18, 0x5E, 0x00, 0x00, 0x00};

        szReadCommandStr[3]=0x5e;
        szReadCommandStr[4]=Data%256;
        szReadCommandStr[5]=Data/256;
        for(int i=0; i<6; i++)
            sum+=szReadCommandStr[i];

        sum=~sum+1;
        szReadCommandStr[6]=sum;
        for(int i=0; i<7; i++)
            Panasonicdatatrq[i]=szReadCommandStr[i];
    }

    fPanasonicParameterRW=true;
    hPanasonicParameterTimeOut.SetSecAndOn(10);
    bPanasonicCommErr=false;
    bRWPanasonicParameterFlag=false;                                            //write

    iPanasonicTask=1;                                                           //jou 2011-11-18重新initial Task
}
//----------------------------------------------------------------------------
void TCOM2::ReadIndexTorqueSetting_HPCard(int Index)                            //Steven 20210223 : 整合Torque存取
{
    AnsiString str="", sSendCmd;
    unsigned char Btmp1;

    str.sprintf(":@00A4002%02d", Index+1);
    Btmp1=A_Create_LRC(str.c_str(), str.Length());
    sSendCmd.sprintf("%s%c@#", str, Btmp1);
    fMain->AddTorqueLog(AnsiString("Send ")+sSendCmd);
    hPanasonicParameterTimeOut.SetSecAndOn(10);
    bRWPanasonicParameterFlag=true; //read
    iReadPanasonicIndex=Index;
    iPanasonicTask=20;
    fPanasonicParameterRW=true;
    Comm1->WriteCommData(sSendCmd.c_str(), sSendCmd.Length());
}
//----------------------------------------------------------------------------
void TCOM2::WriteIndexTorqueSetting_HPCard(int Index, unsigned Data)            //Steven 20210223 : 整合Torque存取
{
    AnsiString str="", sSendCmd;
    unsigned char Btmp1;

    str.sprintf(":@00A6008%02dW%05d", Index+1, Data);
    Btmp1=A_Create_LRC(str.c_str(), str.Length());
    sSendCmd.sprintf("%s%c@#", str, Btmp1);
    fMain->AddTorqueLog(AnsiString("Send ")+sSendCmd);

    hPanasonicParameterTimeOut.SetSecAndOn(10);
    bPanasonicCommErr=false;
    bRWPanasonicParameterFlag=false;                                            //write
    iReadPanasonicIndex=Index;
    iPanasonicTask=1;
    bReceive=false;
    Comm1->WriteCommData(sSendCmd.c_str(), sSendCmd.Length());
    fPanasonicParameterRW=true;
}
//----------------------------------------------------------------------------
void TCOM2::ResetPanasonicTime()
{
    if(fPanasonicParameterRW)
    {
        hPanasonicParameterTimeOut.SetMSAndOn(5000);
    }
}
//----------------------------------------------------------------------------
TQPF_Timer hPDelay;
void __fastcall TCOM2::ReadWriterParameter()
{
    if(TorqueUseHPComCard)                                                      //Steven 20210204 : 使用鴻勁自製的通訊卡
        ReadWriterParameter_HPCard();
    else if(INDEX_DRIVER_TYPE==Panasonic_DRIVER)                                //Panasonic
        ReadWriterParameter_Panasonic();
    else                                                                        //Mitsubishi
        ReadWriterParameter_Mitsubishi();
}
//----------------------------------------------------------------------------
void __fastcall TCOM2::ReadWriterParameter_Panasonic()
{
    static int Address;
    static int ct=0;
    static unsigned char ENQ[1]={0x05};                                         //請求傳送資料
    static unsigned char EOT[1]={0x04};                                         //同意對方可以傳送資料
    static unsigned char ACK[1]={0x06};                                         //確定收到正確資料

    if(fPanasonicParameterRW==false)
        return;

    int &Task=iPanasonicTask;

    if(hPanasonicParameterTimeOut.Off())                                        //10 sec time out, but 機台暫停後ResetPanasonicTime()重設5 sec
    {
        fPanasonicParameterRW=false;
        bPanasonicCommErr=true;
        return;
    }
    switch(Task)
    {
        case 1:
            hPDelay.Set0_1SecAndOn(rwCommandDelay);                             //0.1 sec  //2013-01-15    Dell 設定通訊的delay時間 (9046LS)設0.1會一直timeout
            Address=0;                                                          // which motor
            Task=5;
//            break;
        case 5:
            if(hPDelay.Off())
            {
                TorqueSend(ENQ, 1);                                             //告知馬達要求傳送資料
                bReceive=false;
                ct=0;
                Task=10;
            }
            break;
        case 10:
            ct++;
            if(ct>iPanasonicWT)
            {
                Task=1;
                Comm1->StopComm();
                Comm1->StartComm();
                break;
            }

            if(bReceive)                                                        //待馬達回應可以傳送資料
            {
                if(ptreot==EOT[0])
                {
                    TorqueSend(Panasonicdatatrq, iPanasonicNum);
                    bReceive=false;
                    Task=20;
                    ct=0;
                }
                else
                {
                    Task=1;
                    break;
                }
            }
            break;
        case 20:
            ct++;
            if(ct>iPanasonicWT)
            {
                Task=1;
                Comm1->StopComm();
                Comm1->StartComm();
                break;
            }

            if(bReceive)
            {
                if(ptreot==ACK[0] && ptrenq==ENQ[0])                            //待馬達告知PC要求將扭力值傳回
                {
                    TorqueSend(EOT, 1);                                         //回應馬達可以將資料傳回
                    bReceive=false;
                    Task=30;
                    ct=0;
                }
                else
                {
                    Task=1;
                    break;
                }
            }
            break;
        case 30:
            ct++;
            if(ct>iPanasonicWT)
            {
                Task=1;
                Comm1->StopComm();
                Comm1->StartComm();
                break;
            }

            if(bReceive)
            {
                if(iPanasonicDriverType==Panasonic_DRIVER_A5)                   // 2011.08.11 , Joye , Panasonic A5 //Steven 20120629 add from 7045
                {
                    if(iPanasonicNum==6 && ptreot==0x05)                        //確定資料已收到
                    {
                        TorqueSend(ACK, 1);                                     //回應馬達已收到正確資料
                        Task=40;
                        ct=0;
                    }
                    else if(iPanasonicNum==10 && ptreot==0x01)                  //確定資料已收到
                    {
                        TorqueSend(ACK, 1);                                     //回應馬達已收到正確資料
                        Task=40;
                        ct=0;
                    }
                    else if(iPanasonicNum==8 && ptreot==0x01)                   //2013-01-15    Dell//確定資料已收到
                    {
                        TorqueSend(ACK, 1);                                     //回應馬達已收到正確資料
                        Task=40;
                        ct=0;
                    }
                    else
                    {
                        Task=1;
                        break;
                    }
                }
                else
                {
                    if(iPanasonicNum==5 && ptreot==0x03)                        //確定資料已收到
                    {
                        TorqueSend(ACK, 1);                                     //回應馬達已收到正確資料
                        Task=40;
                        ct=0;
                    }
                    else if(iPanasonicNum==7 && ptreot==0x01)                   //確定資料已收到
                    {
                        TorqueSend(ACK, 1);                                     //回應馬達已收到正確資料
                        Task=40;
                        ct=0;
                    }
                    else
                    {
                        Task=1;
                        break;
                    }
                }
            }
            break;
        case 40:
            ct++;
            if(ct>MOTOR_MIN_WAIT)
            {
                ct=0;
                Task=1;
                fPanasonicParameterRW=false;
                break;
            }
            break;
    }
}
//----------------------------------------------------------------------------
void __fastcall TCOM2::ReadWriterParameter_HPCard()                             //Steven 20210204 : 使用鴻勁自製的通訊卡
{
    if(fPanasonicParameterRW==false)
        return;

    static int Address;
    static int ct=0;
    int &Task=iPanasonicTask;
    if(hPanasonicParameterTimeOut.Off())                                        //10 sec time out, but 機台暫停後ResetPanasonicTime()重設5 sec
    {
        fPanasonicParameterRW=false;
        bPanasonicCommErr=true;
        return;
    }
    switch(Task)
    {
        case 1:
            Address=0;                                                          // which motor
            ct=0;
            Task=10;
        case 10:
            ct++;
            if(ct>iPanasonicWT)
            {
                Task=1;
                Comm1->StopComm();
                Comm1->StartComm();
                break;
            }

            if(bReceive)                                                        //寫完讀一次設定值
            {
                COM2->ReadIndexTorqueSetting(iReadPanasonicIndex);
                bReceive=false;
                Task=20;
                ct=0;
            }
            break;
        case 20:
            ct++;
            if(ct>iPanasonicWT)
            {
                Task=1;
                Comm1->StopComm();
                Comm1->StartComm();
                break;
            }

            if(bReceive)
            {
                Task=30;
                ct=0;
            }
            break;
        case 30:
            ct++;
            if(ct>MOTOR_MIN_WAIT)
            {
                ct=0;
                Task=1;
                fPanasonicParameterRW=false;
                break;
            }
            break;
    }
}
//----------------------------------------------------------------------------
void __fastcall TCOM2::Comm1ReceiveData(TObject *Sender, Pointer Buffer,
      WORD BufferLength)
{
    if(InitialOK==false)                                                        //Steven 20120202 : 加入Thread保護
        return;

    AnsiString S1="Recv ", Arm, str3="";
    char str2[2000];
    short int k;
    byte *data;
    data=(byte *)Buffer;
    unsigned char str[2000]={'\0'};

    if(TorqueUseHPComCard)                                                      //Steven 20210204 : 使用鴻勁自製的通訊卡
    {
        if(_byte_datas.empty())
        {
            _byte_datas.clear();
        }

        DATA data1;
        for(int i=0; i<BufferLength; i++)
        {
            data1.push_back(data[i]);
        }
        _byte_datas.push_back(data1);
    }
    else
    {
        if(BufferLength>2000)
        {
            S1.sprintf("Receive (%d): %s", BufferLength, Buffer);
            fMain->AddTorqueLog(S1);
            fMain->slTorqueLog->AddTextWithDateTime(S1);
            return;
        }

        for(int i=0; i<BufferLength; i++)
        {
            sprintf(str2, " %02X", data[i]);
            S1+=str2;

            sprintf(str2, "%c", data[i]);
            str3+=str2;

            str[i]=data[i];
        }
        fMain->AddTorqueLog(str3+AnsiString(" : ")+S1);                         //Steven 20160908 : 整合成Function

        if(INDEX_DRIVER_TYPE==Panasonic_DRIVER)                                 //Panasonic
        {
            if(BufferLength==7 ||
               (iPanasonicDriverType==Panasonic_DRIVER_A5 && BufferLength==9))  // 2011.08.11 , Joye , Panasonic A5 //Steven 20120629 add from 7045
            {
                k=str[4];
                k<<=8;
                k+=str[3];
                if(str[1]<6)
                {
                    if(k<0)
                        k=0;
                    Torque[iReadTorueIndex]=(double)(k)/20.0;
                    sprintf(str2, "%5.2f", Torque[str[1]]);
                    asReceiveTorue=str2;
                }

                if(fPanasonicParameterRW && bRWPanasonicParameterFlag)          //read
                {
                    if(iReadPanasonicIndex==0)
                        fMain->edtReadZ1->Text=k;
                    else
                        fMain->edtReadZ2->Text=k;
                }
            }
        }
        else
        {
            ::sprintf(cReciveMitsubishi_Data, data);
        }
        bReceive=true;
    }
    ptreot=data[0];
    ptrenq=data[1];
}
//---------------------------------------------------------------------------
// 10/01
//---------------------------------------------------------------------------
int iWriteAndCheckMotorTorqueTask=1;
void InitWriteAndCheckMotorTorqueTask()
{
    iWriteAndCheckMotorTorqueTask=1;
    fMain->edtReadZ1->Text=0;
    fMain->edtReadZ2->Text=0;
    fMain->chkReadTorque1->Checked=false;
    fMain->chkReadTorque2->Checked=false;
}
//---------------------------------------------------------------------------
int __fastcall TCOM2::iWriteAndCheckMotorTorque(int MotorIndex, int Torque)     // MotorIndex =0 , 1
{
    static int iRetryCount=0;

    #ifdef SOFT_SIMULTE
    {
        if(iRetryCount!=0)
        {
            iRetryCount=0;
        }
        return 1;
    }
    #else
    {
        static int iRetryTimeOutCTW=0, iRetryTimeOutCTR=0;                      //jou 2014-07-06 修正寫入motor driver中途被按pause會秀alarm
        TEdit *Ptr[2]={fMain->edtReadZ1, fMain->edtReadZ2};
        int &Task=iWriteAndCheckMotorTorqueTask;
        double fReadTorque;

        if(INDEX_DRIVER_TYPE==Mitsubishi_DRIVER)                                //Steven 20101111 : 三菱馬達Contact過壓不會Alarm
        {
            if(MOT[MTestZ1].Led[iAlarmLed]==true || MOT[MTestZ1].Led[iServoalarmLed]==true ||
               MOT[MTestZ2].Led[iAlarmLed]==true || MOT[MTestZ2].Led[iServoalarmLed]==true)
            {
                return 2;
            }

            if(Torque>100)
                Torque=100;
        }

        switch(Task)
        {
            case 1:
                iRetryCount=0;
                iRetryTimeOutCTW=0;
                iRetryTimeOutCTR=0;
                Task=2;
                break;
            case 2:
                fMain->edtReadZ1->Text=0;
                fMain->edtReadZ2->Text=0;

                if(MotorIndex==0)
                {
                    fMain->chkReadTorque1->Checked=true;
                    fMain->chkReadTorque2->Checked=false;
                }
                else
                {
                    fMain->chkReadTorque1->Checked=false;
                    fMain->chkReadTorque2->Checked=true;
                }

                iWriteAndCheckMotorTorqueDelay.SetMSAndOn(20);

                Task=5;
                break;
            case 5:
                if(iWriteAndCheckMotorTorqueDelay.Off())
                {
                    Task=10;
                }
                break;
            case 10:
                if(INDEX_DRIVER_TYPE==Mitsubishi_DRIVER)                        //Mitsubishi
                {
                    if(bRWActionFlag)
                        return 0;
                }

                WriteIndexTorqueSetting(MotorIndex, Torque);
                iWriteAndCheckMotorTorqueDelay.SetSecAndOn(iTorqueCommMaxTime);
                Task=100;
                break;
            case 100:
                if(INDEX_DRIVER_TYPE==Mitsubishi_DRIVER)                        //Mitsubishi
                {
                    if(bRWActionFlag)
                        return 0;
                }

                if(fPanasonicParameterRW==false)
                {
                    ReadIndexTorqueSetting(MotorIndex);
                    iWriteAndCheckMotorTorqueDelay.SetSecAndOn(iTorqueCommMaxTime);
                    iRetryTimeOutCTW=0;
                    Task=200;
                }
                else if(iWriteAndCheckMotorTorqueDelay.Off())
                {
                    Task=2;
                    iRetryTimeOutCTW++;
                    if(iRetryTimeOutCTW>1)                                      //jou 2014-07-06 修正寫入motor driver中途被按pause會秀alarm
                    {
                        ShowMyMessage("Torque Comm Timeout. Please check Torque Comm and try home or to restart!", "");
                        iRetryTimeOutCTW=0;
                    }
                }
                break;
            case 200:
                if(fPanasonicParameterRW==false)
                {
                    if(INDEX_DRIVER_TYPE==Panasonic_DRIVER)                     //Panasonic
                    {
                        if(Ptr[MotorIndex]->Text!=AnsiString(Torque))
                        {
                            iRetryCount++;
                            if(iRetryCount<5)                                   //jou 2014-07-06 3->5 修正馬達扭力異常錯誤
                            {
                                bRWActionFlag=false;
                                Task=2;                                         //jou 2014-07-06 1->2 修正馬達扭力異常錯誤
                                break;
                            }
                            return 2;
                        }
                        else
                        {
                            iRetryCount=0;
                            iRetryTimeOutCTR=0;
                            return 1;
                        }
                    }
                    else
                    {
                        fReadTorque=atof(Ptr[MotorIndex]->Text.c_str());

                        if(fReadTorque<(Torque-1) || fReadTorque>(Torque+1))
                        {
                            iRetryCount++;
                            if(iRetryCount<5)                                   //jou 2014-07-06 3->5 修正馬達扭力異常錯誤
                            {
                                bRWActionFlag=false;                            //Eliot 2010_05_03
                                Task=2;                                         //jou 2014-07-06 1->2 修正馬達扭力異常錯誤
                                break;
                            }
                            return 2;
                        }
                        else
                        {
                            iRetryCount=0;
                            iRetryTimeOutCTR=0;
                            return 1;
                        }
                    }
                }

                if(iWriteAndCheckMotorTorqueDelay.Off())
                {
                    Task=2;
                    iRetryTimeOutCTR++;
                    if(iRetryTimeOutCTR>1)                                      //jou 2014-07-06 修正寫入motor driver中途被按pause會秀alarm
                    {
                        ShowMyMessage("Torque Comm Timeout. Please check Torque Comm and try home or to restart!", "");
                        iRetryTimeOutCTR=0;
                    }
                }
                break;
        }
        return 0;
    }
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::ReadHP34970A(int Index)
{
    char str[256]={"MEAS:TEMP? TC,K,(@101,102,103,104,105,106,107,108,109,110,111,112)\n"};
    char *ptr;
    int len;
    ptr=&str[0];
    len=strlen(ptr);
    Comm3->WriteCommData(str,len);
    bReadHP34970A=false;
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::Comm3ReceiveData(TObject *Sender, Pointer Buffer,
      WORD BufferLength)
{
    if(InitialOK==false)                                                        //Steven 20120202 : 加入Thread保護
        return;

    char *str;
    char rData[1024], sData[1024];
    str=(char *)Buffer;
    if(BufferLength>1000)
        str[1000]=0;
    strncpy(rData, str, sizeof(rData));

    for(int i=0; i<12; i++)
    {
        if(SplitStrByDotSpaceOnly(rData,sData,32))
        {
            fDataHP34970A[i]=atof(sData);
        }
    }
    bReadHP34970A=true;
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::SendStartTest(int iComMPtr,char *str)
{
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::SendSKYMEDITimeOut(int iSite)
{
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::SendSMITimeOut()
{
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::TempComm6ReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    if(bTempComm6ReceiveOK)
    {
        return;
    }

    int iPos;
    char *str;
    char rData[1024];
    AnsiString asString, asFind, asCh;
    double fTemp[20];

    str=(char *)Buffer;
    if(BufferLength>1000)
        str[1000]=0;

    strncpy(rData, str, sizeof(rData));
    asString=rData;

    if(asString.Pos("Initial")>0)
    {
        fDynamicTemp->ListBox1->Items->Insert(0, sAlarmTime+AnsiString(" ")+asString);
        return;
    }

    if(asString.Pos("T1:")==0)
    {
        return;
    }

    for(int i=0; i<16; i++)
    {
        asCh.printf("T%d:", (i+1));
        iPos=asString.Pos(asCh);
        asFind=asString.SubString(iPos+asCh.Length(), 6);
        fTemp[i]=atof(asFind.c_str());
        fDynamicTemp->SetEdit[i]->Text=FormatFloat("0.00", fTemp[i]);
    }

    fDynamicTemp->ListBox1->Items->Insert(0, sAlarmTime+AnsiString(" ")+asString);
    fMain->ProcessTimeUpdate(true);

    for(int i=0; i<16; i++)
    {
        fDynamicTemp->Chart1->Series[i]->AddY(fTemp[i], fMain->StatusBar1->Panels->Items[6]->Text, fDynamicTemp->TC[i]);
    }

    fDynamicTemp->Chart1->Series[16]->AddY(atof(fDynamicTemp->edUpper->Text.c_str()), fMain->StatusBar1->Panels->Items[6]->Text, fDynamicTemp->TC[16]);
    fDynamicTemp->Chart1->Series[17]->AddY(atof(fDynamicTemp->edLower->Text.c_str()), fMain->StatusBar1->Panels->Items[6]->Text, fDynamicTemp->TC[17]);

    bTempComm6ReceiveOK=true;
}
//---------------------------------------------------------------------------
//以下都是為了Mitsubishi Motor新增低~~
//---------------------------------------------------------------------------
void __fastcall TCOM2::ReadWriterParameter_Mitsubishi()
{
    if(bReadTorque_Mitsubishi)
    {
        bRWActionFlag=true;
        if(DoRead_Now_Torque_Action(iReadMitsubishiIndex))
        {
            if(iReadMitsubishiIndex==0)
                fMain->edtReadZ1->Text=fRead_Now_TorqueP;
            else
                fMain->edtReadZ2->Text=fRead_Now_TorqueP;

            bRWActionFlag=false;
            bReadTorque_Mitsubishi=false;
            fPanasonicParameterRW=false;
        }
    }

    if(bsetTorque_Mitsubishi)
    {
        bRWActionFlag=true;
        if(DoSet_Torque_Action(iSetMitsubishiIndex, fSetTorqueValue))
        {
            bRWActionFlag=false;
            bsetTorque_Mitsubishi=false;
            fPanasonicParameterRW=false;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::ReadIndexTorqueSetting_Mitu(int iIndex)
{
    if(bRWActionFlag)
        return;

    iReadMitsubishiIndex=iIndex;
    InitReadTorque_Mitsubishi();
    fPanasonicParameterRW=true;
    bReadTorque_Mitsubishi=true;
    bsetTorque_Mitsubishi=false;
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::WriteIndexTorqueSetting_Mitu(int iIndex, double fTorque)
{
    if(bRWActionFlag)
        return;

    if(fTorque>=100)                                                            //Steven 20140528 : Add From 7045 for Mitsubishi Motor
        fTorque=100;

    iSetMitsubishiIndex=iIndex;
    fSetTorqueValue=fTorque;
    InitSetTorque_Mitsubishi();
    fPanasonicParameterRW=true;
    bsetTorque_Mitsubishi=true;
    bReadTorque_Mitsubishi=false;
}
//---------------------------------------------------------------------------
//Mitsubishi Driver communication
//---------------------------------------------------------------------------
double __fastcall TCOM2::DoChangeASCII_TO_INT(char *Input)                      //把ascii碼轉成數字.
{
    if(Input[0]>='0' && Input[0]<='9')
        return StrToInt(Input[0]);                                              //wei 20150316  原int(Input[0])轉換後與StrToInt(Input[0])不一樣會導致hang up
    else if(Input[0]>='a' && Input[0]<='f')
        return double(Input[0])-87;
    else if(Input[0]>='A' && Input[0]<='F')
        return double(Input[0])-55;
    else
        return 0;
}
//---------------------------------------------------------------------------
bool __fastcall TCOM2::ReadTorque_Mitsubishi()                                  //讀現在的扭力值.
{                                                                               //1次只讀1個馬達,return ture後數值存於float fReadMotorTorque.
    int &Task=autoTask;
    AnsiString asSendData;

    if(fPanasonicParameterRW)
        return false;

    if(fMain->chkReadTorque1->Checked || fMain->chkReadTorque2->Checked)
    {
        if(fMain->chkReadTorque1->Checked)
        {
            iReadTorueIndex=0;
            SW[SwReadTorue].Off();
        }
        else
        {
            iReadTorueIndex=1;
            SW[SwReadTorue].On();
        }
    }
    else
    {
        Task=1;
        return false;
    }

    switch(Task)
    {
        case 1:                                                                 //讀出指定參數群組的指令.
            if(iReadTorueIndex<0 || iReadTorueIndex>31)
            {
                ShowMyMessage("can't read mitsubishi index");
                return false;
            }

            if(iReadTorueIndex<10)
                sprintf(cChnageIntToASCII, "%C", '0'+iReadTorueIndex);          //ascii
            else
                sprintf(cChnageIntToASCII, "%C", 'A'+iReadTorueIndex-10);

            cMotorAddress=cChnageIntToASCII[0];
            cCommand_1='0';
            cCommand_2='4';
            cDataNo_1='0';
            cDataNo_2='1';

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c",
                                            _SOH,
                                            cMotorAddress,
                                            cCommand_1,
                                            cCommand_2,
                                            _STX,
                                            cDataNo_1,
                                            cDataNo_2,
                                            _ETX,
                                            cCheckSumTemp[strlen(cCheckSumTemp)-2],
                                            cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            sprintf(cReciveMitsubishi_Data, "");
            Comm1->WriteCommData(cSendMitsubishi_Data, strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            hTimeOutLimit.SetSecAndOn(3);                                       //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            Task=10;
            break;
        case 10:
            if(strlen(cReciveMitsubishi_Data)>1)                                //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            {
                iCheckSum=0;
                for(int i=1; i<=7; i++)
                    iCheckSum+=int(cReciveMitsubishi_Data[i]);

                sprintf(cCheckSumTemp, "%X", iCheckSum);
                if(cReciveMitsubishi_Data[1]==cMotorAddress                 &&
                   cReciveMitsubishi_Data[2]=='A'                           &&  //'A'代表正常
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-2]==cCheckSumTemp[strlen(cCheckSumTemp)-2] &&
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-1]==cCheckSumTemp[strlen(cCheckSumTemp)-1])
                {
                    if(cReciveMitsubishi_Data[3]=='0'    &&                     //確認參數群組為0002,不然要重新指定.
                       cReciveMitsubishi_Data[4]=='0'    &&
                       cReciveMitsubishi_Data[5]=='0'    &&
                       cReciveMitsubishi_Data[6]=='2'    )
                    {
                        Task=30;
                    }
                    else
                    {
                        Task=20;
                    }
                }
                else                                                            //Steven 20140528 : Add From 7045 for Mitsubishi Motor
                {
                    Task=1;
                    return false;
                }
            }

            if(hTimeOutLimit.Off())
            {
                Task=1;
                return false;
            }
            break;
        case 20:                                                                //指定參數群組為0002的指令.
            cCommand_1='8';
            cCommand_2='5';
            cDataNo_1='0';
            cDataNo_2='0';
            cNoData[0]='0';
            cNoData[1]='0';
            cNoData[2]='0';
            cNoData[3]='2';

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                int(cNoData[0])+
                int(cNoData[1])+
                int(cNoData[2])+
                int(cNoData[3])+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
                                            _SOH,
                                            cMotorAddress,
                                            cCommand_1,
                                            cCommand_2,
                                            _STX,
                                            cDataNo_1,
                                            cDataNo_2,
                                            cNoData[0],
                                            cNoData[1],
                                            cNoData[2],
                                            cNoData[3],
                                            _ETX,
                                            cCheckSumTemp[strlen(cCheckSumTemp)-2],
                                            cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            Comm1->WriteCommData(cSendMitsubishi_Data, strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            Task=1;
            break;
        case 30:                                                                //讀取狀態.
            cCommand_1='0';
            cCommand_2='1';

            cDataNo_1='8';
            cDataNo_2='A';

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c",
                                            _SOH,
                                            cMotorAddress,
                                            cCommand_1,
                                            cCommand_2,
                                            _STX,
                                            cDataNo_1,
                                            cDataNo_2,
                                            _ETX,
                                            cCheckSumTemp[strlen(cCheckSumTemp)-2],
                                            cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            for(int i=0; i<32; i++)
                cReciveMitsubishi_Data[i]=0x00;
            Comm1->WriteCommData(cSendMitsubishi_Data, strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            hTimeOutLimit.SetSecAndOn(3);                                       //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            Task=40;
            break;
        case 40:                                                                //回收的資料= STX+馬達編號+異常碥+資料+ETX+檢查合.
            if(strlen(cReciveMitsubishi_Data)>1)                                //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            {
                iCheckSum=0;
                for(int i=1; i<(int)(strlen(cReciveMitsubishi_Data)-2); i++)
                {
                    iCheckSum+=int(cReciveMitsubishi_Data[i]);
                }

                sprintf(cCheckSumTemp,"%X",iCheckSum);
                if(cReciveMitsubishi_Data[1]==cMotorAddress                 &&
                   cReciveMitsubishi_Data[2]=='A'                           &&  //'A'代表正常
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-2]==cCheckSumTemp[strlen(cCheckSumTemp)-2] &&
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-1]==cCheckSumTemp[strlen(cCheckSumTemp)-1])
                {
                    fReadMotorTorque=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[10])*(16<<12);//65536
                    fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[11])*(16<<8);//4096
                    fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[12])*(16<<4);//256
                    fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[13])*16;
                    fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[14]);

                    if(cReciveMitsubishi_Data[7]=='F')
                    {
                        fReadMotorTorque=int(fReadMotorTorque)-1048576;
                    }

                    for(int i=1; i<StrToInt(cReciveMitsubishi_Data[4]); i++)    //數字2=除10,3=除100.   //jou 2014-10-22 StrToInt -> atoi   //wei 20150316 改回StrToInt
                        fReadMotorTorque/=10;

                    tTimeTemp=Now();
                    sprintf(cSendMitsubishi_Data,"%s < %s =%2.1f",
                        tTimeTemp.FormatString("hh:nn:ss").c_str(),
                        cReciveMitsubishi_Data,
                        fReadMotorTorque);
                    asSendData="Data : ";
                    asSendData+=cSendMitsubishi_Data;
                    fMain->AddTorqueLog(asSendData);                            //Steven 20160908 : 整合成Function

                    if(fReadMotorTorque>100)
                        fReadMotorTorque=-1;

                    if(iReadTorueIndex==0)
                    {
                        fMain->edTorue0->Text=fReadMotorTorque;
                        fContact->PnlTorue0->Caption=fReadMotorTorque;
                    }
                    else
                    {
                        fMain->edTorue1->Text=fReadMotorTorque;
                        fContact->PnlTorue1->Caption=fReadMotorTorque;
                    }
                    Task=1;
                    return true;
                }
                else                                                            //Steven 20140528 : Add From 7045 for Mitsubishi Motor
                {
                    Task=1;
                    return false;
                }
            }

            if(hTimeOutLimit.Off())
            {
                Task=1;
                return false;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
int iMtsubishi_Count=0;                                                         //Steven 20140627 : Fixed Mitsubishi Torque Hang Up
void __fastcall TCOM2::InitSetTorque_Mitsubishi()
{
    iWriteTorqueTask=1;
    iMtsubishi_Count=0;
}
//---------------------------------------------------------------------------
bool __fastcall TCOM2::DoSet_Torque_Action(int iIndex, double fTorque)          //寫入需要的扭力值.
{
    int &Task=iWriteTorqueTask;
    AnsiString asSendData;
    switch(Task)
    {
        case 1:                                                                 //讀出指定參數群組的指令.
            if(iIndex<0 || iIndex>31)
            {
                ShowMyMessage("can't read mitsubishi index");
                return false;
            }

            if(iIndex<10)
                sprintf(cChnageIntToASCII, "%C", '0'+iIndex);                   //ascii
            else
                sprintf(cChnageIntToASCII, "%C", 'A'+iIndex-10);

            cMotorAddress=cChnageIntToASCII[0];
            cCommand_1='0';
            cCommand_2='4';
            cDataNo_1='0';
            cDataNo_2='1';

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c",
                                            _SOH,
                                            cMotorAddress,
                                            cCommand_1,
                                            cCommand_2,
                                            _STX,
                                            cDataNo_1,
                                            cDataNo_2,
                                            _ETX,
                                            cCheckSumTemp[strlen(cCheckSumTemp)-2],
                                            cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            sprintf(cReciveMitsubishi_Data, "");
            Comm1->WriteCommData(cSendMitsubishi_Data, strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            hTimeOutLimit.Set0_1SecAndOn(20);                                   // 2010/5/4 lee
            fWriteTorque=int(fTorque*10.0);                                     //because this motor torque 0~100.0,must change new Torque lile 0~300.
            Task=10;
            break;
        case 10:
            if(strlen(cReciveMitsubishi_Data)>1)                                //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            {
                iCheckSum=0;
                for(int i=1; i<=7; i++)
                    iCheckSum+=int(cReciveMitsubishi_Data[i]);

                sprintf(cCheckSumTemp, "%X", iCheckSum);
                if(cReciveMitsubishi_Data[1]==cMotorAddress                 &&
                   cReciveMitsubishi_Data[2]=='A'                           &&  //'A'代表正常
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-2]==cCheckSumTemp[strlen(cCheckSumTemp)-2] &&
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-1]==cCheckSumTemp[strlen(cCheckSumTemp)-1])
                {
                    if(cReciveMitsubishi_Data[3]=='0'    &&                     //確認參數群組為0000,不然要重新指定.
                       cReciveMitsubishi_Data[4]=='0'    &&
                       cReciveMitsubishi_Data[5]=='0'    &&
                       cReciveMitsubishi_Data[6]=='0'    )
                    {
                        Task=30;
                    }
                    else
                    {
                        Task=20;
                    }
                }
                else                                                            //Steven 20140528 : Add From 7045 for Mitsubishi Motor
                {
                    Task=1;
                    return false;
                }
            }

            if(hTimeOutLimit.Off())
            {
                Task=1;
                iMtsubishi_Count++;

                if(iMtsubishi_Count>10)                                         //Steven 20140627 : Fixed Mitsubishi Torque Hang Up
                {
                    Comm1->StopComm();
                    Comm1->StartComm();
                }
                return false;
            }
            break;
        case 20:                                                                //指定參數群組為0000的指令.
            cCommand_1='8';
            cCommand_2='5';
            cDataNo_1='0';
            cDataNo_2='0';
            cNoData[0]='0';
            cNoData[1]='0';
            cNoData[2]='0';
            cNoData[3]='0';

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                int(cNoData[0])+
                int(cNoData[1])+
                int(cNoData[2])+
                int(cNoData[3])+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
                                            _SOH,
                                            cMotorAddress,
                                            cCommand_1,
                                            cCommand_2,
                                            _STX,
                                            cDataNo_1,
                                            cDataNo_2,
                                            cNoData[0],
                                            cNoData[1],
                                            cNoData[2],
                                            cNoData[3],
                                            _ETX,
                                            cCheckSumTemp[strlen(cCheckSumTemp)-2],
                                            cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            Comm1->WriteCommData(cSendMitsubishi_Data, strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            Task=1;
            break;
        case 30:                                                                //read torque P
            cCommand_1='0';
            cCommand_2='5';

            cDataNo_1='0';
            cDataNo_2='B';

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c",
                                            _SOH,
                                            cMotorAddress,
                                            cCommand_1,
                                            cCommand_2,
                                            _STX,
                                            cDataNo_1,
                                            cDataNo_2,
                                            _ETX,
                                            cCheckSumTemp[strlen(cCheckSumTemp)-2],
                                            cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            for(int i=0; i<32; i++)
                cReciveMitsubishi_Data[i]=0x00;
            Comm1->WriteCommData(cSendMitsubishi_Data, strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            hTimeOutLimit.Set0_1SecAndOn(20);
            Task=40;
            break;
        case 40:                                                                //回收的資料= STX+馬達編號+異常碥+資料+ETX+檢查合.
            if(strlen(cReciveMitsubishi_Data)>1)                                //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            {
                iCheckSum=0;
                for(int i=1; i<(int)(strlen(cReciveMitsubishi_Data)-2); i++)
                {
                    iCheckSum+=int(cReciveMitsubishi_Data[i]);
                }

                sprintf(cCheckSumTemp, "%X", iCheckSum);
                if(cReciveMitsubishi_Data[1]==cMotorAddress                 &&
                   cReciveMitsubishi_Data[2]=='A'                           &&  //'A'代表正常
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-2]==cCheckSumTemp[strlen(cCheckSumTemp)-2] &&
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-1]==cCheckSumTemp[strlen(cCheckSumTemp)-1])
                {
                    if(cReciveMitsubishi_Data[3]=='1')
                    {
                        fReadMotorTorque=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[6])*(16<<12);//65536
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[7])*(16<<8);//4096
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[8])*(16<<4);//256
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[9])*16;
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[10]);
                    }

                    tTimeTemp=Now();
                    sprintf(cSendMitsubishi_Data, "%s < %s =%2.1f",
                        tTimeTemp.FormatString("hh:nn:ss").c_str(),
                        cReciveMitsubishi_Data,
                        fReadMotorTorque);
                    asSendData="Data : ";
                    asSendData+=cSendMitsubishi_Data;
                    fMain->AddTorqueLog(asSendData);                            //Steven 20160908 : 整合成Function

                    if(fReadMotorTorque==fWriteTorque)
                    {
                        Task=60;
                    }
                    else
                    {
                        Task=50;
                    }
                }
                else                                                            //Steven 20140528 : Add From 7045 for Mitsubishi Motor
                {
                    Task=1;
                    return false;
                }
            }

            if(hTimeOutLimit.Off())
            {
                Task=1;
            }
            break;
        case 50:                                                                //write torque P
            cCommand_1='8';
            cCommand_2='4';
            cDataNo_1='0';
            cDataNo_2='B';
            cWriteDataTitle[0]='3';
            cWriteDataTitle[1]='2';

            sprintf(cNoData, "%06X", int(fWriteTorque));

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                int(cWriteDataTitle[0])+
                int(cWriteDataTitle[1])+
                int(cNoData[0])+
                int(cNoData[1])+
                int(cNoData[2])+
                int(cNoData[3])+
                int(cNoData[4])+
                int(cNoData[5])+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
                                            _SOH,
                                            cMotorAddress,
                                            cCommand_1,
                                            cCommand_2,
                                            _STX,
                                            cDataNo_1,
                                            cDataNo_2,
                                            cWriteDataTitle[0],
                                            cWriteDataTitle[1],
                                            cNoData[0],
                                            cNoData[1],
                                            cNoData[2],
                                            cNoData[3],
                                            cNoData[4],
                                            cNoData[5],
                                            _ETX,
                                            cCheckSumTemp[strlen(cCheckSumTemp)-2],
                                            cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            Comm1->WriteCommData(cSendMitsubishi_Data, strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            Task=30;
            break;
        case 60:                                                                //read torque N
            cCommand_1='0';
            cCommand_2='5';

            cDataNo_1='0';
            cDataNo_2='C';

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c",
                                            _SOH,
                                            cMotorAddress,
                                            cCommand_1,
                                            cCommand_2,
                                            _STX,
                                            cDataNo_1,
                                            cDataNo_2,
                                            _ETX,
                                            cCheckSumTemp[strlen(cCheckSumTemp)-2],
                                            cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            for(int i=0; i<32; i++)
                cReciveMitsubishi_Data[i]=0x00;
            Comm1->WriteCommData(cSendMitsubishi_Data, strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            hTimeOutLimit.Set0_1SecAndOn(20);
            Task=70;
            break;
        case 70:                                                                //回收的資料= STX+馬達編號+異常碥+資料+ETX+檢查合.
            if(strlen(cReciveMitsubishi_Data)>0)                                //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            {
                iCheckSum=0;
                for(int i=1; i<(int)(strlen(cReciveMitsubishi_Data)-2); i++)
                {
                    iCheckSum+=int(cReciveMitsubishi_Data[i]);
                }

                sprintf(cCheckSumTemp, "%X", iCheckSum);
                if(cReciveMitsubishi_Data[1]==cMotorAddress                 &&
                   cReciveMitsubishi_Data[2]=='A'                           &&  //'A'代表正常
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-2]==cCheckSumTemp[strlen(cCheckSumTemp)-2] &&
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-1]==cCheckSumTemp[strlen(cCheckSumTemp)-1])
                {
                    if(cReciveMitsubishi_Data[3]=='1')
                    {
                        fReadMotorTorque=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[6])*(16<<12);//65536
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[7])*(16<<8);//4096
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[8])*(16<<4);//256
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[9])*16;
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[10]);
                    }

                    tTimeTemp=Now();
                    sprintf(cSendMitsubishi_Data, "%s < %s =%2.1f",
                                                    tTimeTemp.FormatString("hh:nn:ss").c_str(),
                                                    cReciveMitsubishi_Data,
                                                    fReadMotorTorque);
                    asSendData="Data : ";
                    asSendData+=cSendMitsubishi_Data;
                    fMain->AddTorqueLog(asSendData);                            //Steven 20160908 : 整合成Function

                    if(fReadMotorTorque==fWriteTorque)
                    {
                        Task=1;
                        return true;
                    }
                    else
                    {
                        Task=80;
                    }
                }
                else                                                            //Steven 20140528 : Add From 7045 for Mitsubishi Motor
                {
                    Task=1;
                    return false;
                }
            }

            if(hTimeOutLimit.Off())
            {
                Task=1;
            }
            break;
        case 80:                                                                //write torque P
            cCommand_1='8';
            cCommand_2='4';
            cDataNo_1='0';
            cDataNo_2='C';
            cWriteDataTitle[0]='3';
            cWriteDataTitle[1]='2';

            sprintf(cNoData, "%06X", int(fWriteTorque));

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                int(cWriteDataTitle[0])+
                int(cWriteDataTitle[1])+
                int(cNoData[0])+
                int(cNoData[1])+
                int(cNoData[2])+
                int(cNoData[3])+
                int(cNoData[4])+
                int(cNoData[5])+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
                                            _SOH,
                                            cMotorAddress,
                                            cCommand_1,
                                            cCommand_2,
                                            _STX,
                                            cDataNo_1,
                                            cDataNo_2,
                                            cWriteDataTitle[0],
                                            cWriteDataTitle[1],
                                            cNoData[0],
                                            cNoData[1],
                                            cNoData[2],
                                            cNoData[3],
                                            cNoData[4],
                                            cNoData[5],
                                            _ETX,
                                            cCheckSumTemp[strlen(cCheckSumTemp)-2],
                                            cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            Comm1->WriteCommData(cSendMitsubishi_Data, strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            Task=60;
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::InitReadTorque_Mitsubishi()
{
    iRead_Now_Torque_Task=1;
}
//---------------------------------------------------------------------------
bool __fastcall TCOM2::DoRead_Now_Torque_Action(int iIndex)                     //讀取Driver設定的扭力值.
{
    int &Task=iRead_Now_Torque_Task;
    AnsiString asSendData;
    switch(Task)
    {
        case 1:                                                                 //讀出指定參數群組的指令.
            if(iIndex<0 || iIndex>31)
            {
                ShowMyMessage("can't read mitsubishi index");
                return false;
            }

            if(iIndex<10)
                sprintf(cChnageIntToASCII, "%C", '0'+iIndex);                   //ascii
            else
                sprintf(cChnageIntToASCII, "%C", 'A'+iIndex-10);

            cMotorAddress=cChnageIntToASCII[0];
            cCommand_1='0';
            cCommand_2='4';
            cDataNo_1='0';
            cDataNo_2='1';

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c",
                _SOH,
                cMotorAddress,
                cCommand_1,
                cCommand_2,
                _STX,
                cDataNo_1,
                cDataNo_2,
                _ETX,
                cCheckSumTemp[strlen(cCheckSumTemp)-2],
                cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            sprintf(cReciveMitsubishi_Data,"");
            Comm1->WriteCommData(cSendMitsubishi_Data,strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            hTimeOutLimit.SetSecAndOn(3);                                       //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            Task=10;
            break;
        case 10:
            if(strlen(cReciveMitsubishi_Data)>1)                                //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            {
                iCheckSum=0;
                for(int i=1; i<=7; i++)
                    iCheckSum+=int(cReciveMitsubishi_Data[i]);

                sprintf(cCheckSumTemp,"%X",iCheckSum);
                if(cReciveMitsubishi_Data[1]==cMotorAddress                 &&
                   cReciveMitsubishi_Data[2]=='A'                           &&  //'A'代表正常
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-2]==cCheckSumTemp[strlen(cCheckSumTemp)-2] &&
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-1]==cCheckSumTemp[strlen(cCheckSumTemp)-1])
                {
                    //確認參數群組為0000,不然要重新指定.
                    if(cReciveMitsubishi_Data[3]=='0'    &&
                       cReciveMitsubishi_Data[4]=='0'    &&
                       cReciveMitsubishi_Data[5]=='0'    &&
                       cReciveMitsubishi_Data[6]=='0'    )
                    {
                        Task=30;
                    }
                    else
                    {
                        Task=20;
                    }
                }
            }

            if(hTimeOutLimit.Off())
            {
                Task=1;
                return false;
            }
            break;
        case 20:                                                                //指定參數群組為0000的指令.
            cCommand_1='8';
            cCommand_2='5';
            cDataNo_1='0';
            cDataNo_2='0';
            cNoData[0]='0';
            cNoData[1]='0';
            cNoData[2]='0';
            cNoData[3]='0';

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                int(cNoData[0])+
                int(cNoData[1])+
                int(cNoData[2])+
                int(cNoData[3])+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
                                            _SOH,
                                            cMotorAddress,
                                            cCommand_1,
                                            cCommand_2,
                                            _STX,
                                            cDataNo_1,
                                            cDataNo_2,
                                            cNoData[0],
                                            cNoData[1],
                                            cNoData[2],
                                            cNoData[3],
                                            _ETX,
                                            cCheckSumTemp[strlen(cCheckSumTemp)-2],
                                            cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            Comm1->WriteCommData(cSendMitsubishi_Data, strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            Task=1;
            break;
        case 30:                                                                //read torque P
            cCommand_1='0';
            cCommand_2='5';

            cDataNo_1='0';
            cDataNo_2='B';

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c",
                                            _SOH,
                                            cMotorAddress,
                                            cCommand_1,
                                            cCommand_2,
                                            _STX,
                                            cDataNo_1,
                                            cDataNo_2,
                                            _ETX,
                                            cCheckSumTemp[strlen(cCheckSumTemp)-2],
                                            cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            for(int i=0; i<32; i++)
                cReciveMitsubishi_Data[i]=0x00;
            Comm1->WriteCommData(cSendMitsubishi_Data, strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            hTimeOutLimit.SetSecAndOn(3);                                       //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            Task=40;
            break;
        case 40:                                                                //回收的資料= STX+馬達編號+異常碥+資料+ETX+檢查合.
            if(strlen(cReciveMitsubishi_Data)>1)                                //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            {
                iCheckSum=0;
                if(strlen(cReciveMitsubishi_Data)>1)
                {
                    for(int i=1; i<(int)(strlen(cReciveMitsubishi_Data)-2); i++)
                    {
                        if(cReciveMitsubishi_Data[i]!='\0')
                            iCheckSum+=int(cReciveMitsubishi_Data[i]);
                    }
                }

                sprintf(cCheckSumTemp, "%X", iCheckSum);
                if(cReciveMitsubishi_Data[1]==cMotorAddress                 &&
                   cReciveMitsubishi_Data[2]=='A'                           &&  //'A'代表正常
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-2]==cCheckSumTemp[strlen(cCheckSumTemp)-2] &&
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-1]==cCheckSumTemp[strlen(cCheckSumTemp)-1])
                {
                    if(cReciveMitsubishi_Data[3]=='1')
                    {
                        fReadMotorTorque=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[6])*(16<<12);//65536
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[7])*(16<<8);//4096
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[8])*(16<<4);//256
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[9])*16;
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[10]);
                    }

                    for(int i=1; i<StrToInt(cReciveMitsubishi_Data[4]); i++)    //數字2=除10,3=除100. //jou 2014-10-22 StrToInt -> atoi   //wei 20150316 改回StrToInt
                        fReadMotorTorque/=10.0;

                    fRead_Now_TorqueP=fReadMotorTorque;

                    tTimeTemp=Now();
                    sprintf(cSendMitsubishi_Data,"%s < %s =%2.1f",
                        tTimeTemp.FormatString("hh:nn:ss").c_str(),
                        cReciveMitsubishi_Data,
                        fReadMotorTorque);
                    asSendData="Data : ";
                    asSendData+=cSendMitsubishi_Data;
                    fMain->AddTorqueLog(asSendData);                            //Steven 20160908 : 整合成Function

                    Task=50;
                }
            }

            if(hTimeOutLimit.Off())
            {
                Task=1;
            }
            break;
        case 50:                                                                //read torque N
            cCommand_1='0';
            cCommand_2='5';

            cDataNo_1='0';
            cDataNo_2='C';

            iCheckSum=
                int(cMotorAddress)+
                int(cCommand_1)+
                int(cCommand_2)+
                _STX+
                int(cDataNo_1)+
                int(cDataNo_2)+
                _ETX;

            sprintf(cCheckSumTemp, "%X", iCheckSum);

            sprintf(cSendMitsubishi_Data, "%c%c%c%c%c%c%c%c%c%c",
                                            _SOH,
                                            cMotorAddress,
                                            cCommand_1,
                                            cCommand_2,
                                            _STX,
                                            cDataNo_1,
                                            cDataNo_2,
                                            _ETX,
                                            cCheckSumTemp[strlen(cCheckSumTemp)-2],
                                            cCheckSumTemp[strlen(cCheckSumTemp)-1]);

            for(int i=0; i<32; i++)
                cReciveMitsubishi_Data[i]=0x00;
            Comm1->WriteCommData(cSendMitsubishi_Data,strlen(cSendMitsubishi_Data));
            asSendData="Send : ";
            asSendData+=cSendMitsubishi_Data;
            fMain->AddTorqueLog(asSendData);                                    //Steven 20160908 : 整合成Function
            hTimeOutLimit.SetSecAndOn(3);                                       //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            Task=60;
            break;
        case 60:                                                                //回收的資料= STX+馬達編號+異常碥+資料+ETX+檢查合.
            if(strlen(cReciveMitsubishi_Data)>1)                                //Steven 20140528 : Add From 7045 for Mitsubishi Motor
            {
                iCheckSum=0;
                for(int i=1; i<(int)(strlen(cReciveMitsubishi_Data)-2); i++)
                {
                    iCheckSum+=int(cReciveMitsubishi_Data[i]);
                }

                sprintf(cCheckSumTemp,"%X",iCheckSum);
                if(cReciveMitsubishi_Data[1]==cMotorAddress                 &&
                   cReciveMitsubishi_Data[2]=='A'                           &&  //'A'代表正常
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-2]==cCheckSumTemp[strlen(cCheckSumTemp)-2] &&
                   cReciveMitsubishi_Data[strlen(cReciveMitsubishi_Data)-1]==cCheckSumTemp[strlen(cCheckSumTemp)-1])
                {
                    if(cReciveMitsubishi_Data[3]=='1')
                    {
                        fReadMotorTorque=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[6])*(16<<12);//65536
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[7])*(16<<8);//4096
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[8])*(16<<4);//256
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[9])*16;
                        fReadMotorTorque+=DoChangeASCII_TO_INT(&cReciveMitsubishi_Data[10]);
                    }

                    for(int i=1; i<StrToInt(cReciveMitsubishi_Data[4]);i++)     //數字2=除10,3=除100. //jou 2014-10-22 StrToInt -> atoi   //wei 20150316 改回StrToInt
                        fReadMotorTorque/=10.0;

                    fRead_Now_TorqueN=fReadMotorTorque;

                    tTimeTemp=Now();
                    sprintf(cSendMitsubishi_Data, "%s < %s =%2.1f",
                                                    tTimeTemp.FormatString("hh:nn:ss").c_str(),
                                                    cReciveMitsubishi_Data,
                                                    fReadMotorTorque);
                    asSendData="Data : ";
                    asSendData+=cSendMitsubishi_Data;
                    fMain->AddTorqueLog(asSendData);                            //Steven 20160908 : 整合成Function
                    Task=1;
                    return true;
                }
            }

            if(hTimeOutLimit.Off())
            {
                Task=1;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::Comm4ReceiveData(TObject *Sender, Pointer Buffer,
      WORD BufferLength)
{
    if(InitialOK==false)                                                        //Steven 20120202 : 加入Thread保護
        return;

    if(BufferLength==0)
        return;
    char temp[1024]={'\0'};

    if(BufferLength>1024)                                                       //JimmyChiu 20210922 Avoid overflow
    {
        LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);
        fMain->AddTorqueLog(LogMsg);
        fMain->slTorqueLog->AddTextWithDateTime(LogMsg);
        return;
    }

    memcpy(temp, Buffer, BufferLength);
    CCDReceiveBuffer = CCDReceiveBuffer + AnsiString(temp);
    int iStart;
    AnsiString asString="";

    for(int i=0; i<CCD_MAX_CMD; i++)
    {
        iStart=CCDReceiveBuffer.Pos(sRealTimeCom_Receive[i]);
        if(iStart!=0)
        {
            switch(i)
            {
                case rtHome:
                    bRealTimeCom_ReceiveOK[rtHome]=true;
                    break;
                case rtInspStart:
                    bRealTimeCom_ReceiveOK[rtInspStart]=true;
                    break;
                case rtInspStartROI:
                    bRealTimeCom_ReceiveOK[rtInspStartROI]=true;
                    bRealTimeCom_ReceiveOK[rtInspStart]=false;
                    Comm4->WriteCommData(sRealTimeCom_Receive[rtRelease].c_str(), sRealTimeCom_Receive[rtRelease].Length());
                    bSendRealCCDSendStart=true;
                    bSendRealCCDSendVerify=true;
                    ShowErrorMessage("MES0331", 0, MMIndex);                    //Please AutoHight to get ROI Model!
                    bSendRealCCDSendStart=true;
                    break;
                case rtInspStartGolden:
                    bRealTimeCom_ReceiveOK[rtInspStartGolden]=true;
                    bRealTimeCom_ReceiveOK[rtInspStart]=false;
                    Comm4->WriteCommData(sRealTimeCom_Receive[rtRelease].c_str(), sRealTimeCom_Receive[rtRelease].Length());
                    ShowErrorMessage("MES0331", 0, MMIndex);                    //Please AutoHight to get ROI Model!
                    bSendRealCCDSendStart=true;
                    bSendRealCCDSendVerify=true;
                    break;
                case rtInspStartUnKnow:
                    bRealTimeCom_ReceiveOK[rtInspStartUnKnow]=true;
                    bRealTimeCom_ReceiveOK[rtInspStart]=false;
                    Comm4->WriteCommData(sRealTimeCom_Receive[rtRelease].c_str(), sRealTimeCom_Receive[rtRelease].Length());
                    ShowErrorMessage("MES0331", 0, MMIndex);                    //Please AutoHight to get ROI Model!
                    bSendRealCCDSendStart=true;
                    bSendRealCCDSendVerify=true;
                    break;
                case rtStartLightF:
                    bRealTimeCom_ReceiveOK[rtStartLightF]=true;
                    bRealTimeCom_ReceiveOK[rtInspStart]=false;
                    Comm4->WriteCommData(sRealTimeCom_Receive[rtRelease].c_str(), sRealTimeCom_Receive[rtRelease].Length());
                    bSendRealCCDSendStart=true;
                    bSendRealCCDSendVerify=true;
                    ShowErrorMessage("WAR0333", 0, MMIndex);                    //RTC Light Fail!
                    break;
                case rtInspEnd:
                    bRealTimeCom_ReceiveOK[rtInspEnd]=true;
                    break;
                case rtArmIndex1:
                    bRealTimeCom_ReceiveOK[rtArmIndex1]=true;
                    break;
                case rtArmIndex2:
                    bRealTimeCom_ReceiveOK[rtArmIndex2]=true;
                    break;
                case rtFileOK:
                    bRealTimeCom_ReceiveOK[rtFileOK]=true;
                    Comm4->WriteCommData(sRealTimeCom_Send[rtInspEnd].c_str(), sRealTimeCom_Send[rtInspEnd].Length());
                    break;
                case rtAlarHasIC:
                    if(ScanBtnThd->GetRunState()==true)
                        bRealTimeCom_ReceiveOK[rtAlarHasIC]=true;
                    break;
                case rtAlarGrabTimeOut:
                    if(ScanBtnThd->GetRunState()==true)
                    {
                        bRealTimeCom_ReceiveOK[rtAlarGrabTimeOut]=true;
                        if(SystemStart==false)
                        {
                            Comm4->WriteCommData(sRealTimeCom_Receive[rtRelease].c_str(), sRealTimeCom_Receive[rtRelease].Length());
                            bRealTimeCom_ReceiveOK[rtAlarGrabTimeOut]=false;
                        }
                    }
                    break;
                case rtAlarEncode:                                              //ChungHung 20111110  alarm-encode
                    bRealTimeCom_ReceiveOK[rtAlarEncode]=true;
                    Comm4->WriteCommData(sRealTimeCom_Receive[rtRelease].c_str(), sRealTimeCom_Receive[rtRelease].Length());
                    ShowErrorMessage("WAR0340", 0, MMIndex);                    //RTC Please Check Encode!
                    bSendRealCCDSendStart=true;
                    bSendRealCCDSendVerify=true;
                    break;
                case rtClear:
                    bSendRealCCDSendStart=true;
                    bSendRealCCDSendVerify=true;
                    bRealTimeCom_ReceiveOK[rtClear]=true;
                    break;
                case rtRelease:
                    bRealTimeCom_ReceiveOK[rtRelease]=true;
                    break;
                case rtRsOK:
                    bRealTimeCom_ReceiveOK[rtRsOK]=true;
                    break;
                case rtFullLOK:
                    bRealTimeCom_ReceiveOK[rtFullLOK]=true;
                    fLotInfo->LoadRTCFullViewImg(true);                         //Steven 20110825 : Real Time CCD - 顯示Full View Image
                    break;
                case rtFullLNG:
                    bRealTimeCom_ReceiveOK[rtFullLNG]=true;
                    break;
                case rtFullTOK:
                    RecordProcess("REALTIME CCD Full OK");                      //Steven 20181105 : ADD RTC Full View Log
                    bRealTimeCom_ReceiveOK[rtFullTOK]=true;
                    break;
                case rtFullTNG:
                    RecordProcess("REALTIME CCD Full NG");                      //Steven 20181105 : ADD RTC Full View Log
                    bRealTimeCom_ReceiveOK[rtFullTNG]=true;
                    break;
                case rtPassOn:
                    bRealTimeCom_ReceiveOK[rtPassOn]=true;
                    break;
                case rtReall:
                    bRealTimeCom_ReceiveOK[rtReall]=true;
                    break;
                case rtPassOff:
                    bRealTimeCom_ReceiveOK[rtPassOff]=true;
                    break;
                case rtInspStartROIF:
                    bRealTimeCom_ReceiveOK[rtInspStartROIF]=true;
                    Comm4->WriteCommData(sRealTimeCom_Receive[rtRelease].c_str(), sRealTimeCom_Receive[rtRelease].Length());
                    ShowErrorMessage("MES0332", 0, MMIndex);                    //Please AutoHight to get FullView Model!
                    bSendRealCCDSendStart=true;
                    bSendRealCCDSendVerify=true;
                    break;
                case rtInspStartGoldenF:
                    bRealTimeCom_ReceiveOK[rtInspStartGoldenF]=true;
                    Comm4->WriteCommData(sRealTimeCom_Receive[rtRelease].c_str(), sRealTimeCom_Receive[rtRelease].Length());
                    ShowErrorMessage("MES0332", 0, MMIndex);                    //Please AutoHight to get FullView Model!
                    bSendRealCCDSendStart=true;
                    bSendRealCCDSendVerify=true;
                    break;
                case rtSite:
                    bRealTimeCom_ReceiveOK[rtSite]=true;
                    break;
                case rtArmFinish:
                    bRealTimeCom_ReceiveOK[rtArmFinish]=true;
                    break;
                case rtNODEVICE:
                    bRealTimeCom_ReceiveOK[rtNODEVICE]=true;
                    fLotInfo->LoadRTCFullViewImg(false);                        //Steven 20110825 : Real Time CCD - 使用白色圖
                    break;
                case rtNOMODEL:
                    bRealTimeCom_ReceiveOK[rtNOMODEL]=true;
                    fLotInfo->LoadRTCFullViewImg(false);                        //Steven 20110825 : Real Time CCD - 使用白色圖
                    break;
                case rtMODEL:
                    bRealTimeCom_ReceiveOK[rtMODEL]=true;
                    fLotInfo->LoadRTCFullViewImg(true);                         //Steven 20110825 : Real Time CCD - 顯示Full View Image
                    break;
                case rtGETMODEL:
                    bRealTimeCom_ReceiveOK[rtGETMODEL]=true;
                    fLotInfo->LoadRTCFullViewImg(true);                         //Steven 20110825 : Real Time CCD - 顯示Full View Image
                    break;
                case rtVISIONON:
                    bRealTimeCom_ReceiveOK[rtVISIONON]=true;
                    MyDBIProcess("Process", "RTC Vision On.");                  //Steven 20110825 : Real Time CCD - 程式有開
                    if(REAL_TIME_CCD==true && COM2->bCCDDummyRum==false)
                    {
                        if(bRealTimeCom_ReceiveOK[rtVISIONON])
                        {
                            bSendRealCCDSendStart=true;
                            #ifdef DEBUG_AutoHomeLog
                            if(fAllMotorHome==true)
                            {
                                NewRecordProcess("", "fAllMotorHome", "Comm4ReceiveData");
                            }
                            #endif
                            fAllMotorHome=false;
                            MyDBIProcessNew("Motion", "WAR2206", "Auto homing", "RTC Program Open Again Must Home");
                        }
                    }
                    bRealTimeCom_ReceiveOK[rtVISIONON]=true;
                    fMain->Timer4->Enabled=true;                                //Ifor 20180830 :add RTC連線後開啟Time4
                    bCheckThirdPos=false;                                    //wei 20221222 RTC ARM Error
                    COM2->SendCommToVision(COM2->rtCheckThirdPos, false);    //wei 20221222 RTC ARM Error
                    break;
                case rtVISIONOFF:
                    bRealTimeCom_ReceiveOK[rtVISIONOFF]=true;
                    MyDBIProcess("Process", "RTC Vision Off.");                 //Steven 20110825 : Real Time CCD - 程式沒開
                    if(REAL_TIME_CCD==true && COM2->bCCDDummyRum==false)
                    {
                        ShowErrorMessage("WAR0347", 0, MMIndex);                //RTC程式被關閉
                        bSendRealCCDSendStart=true;
                    }
                    break;
                case rtDELETE:
                    bRealTimeCom_ReceiveOK[rtDELETE]=true;
                    fLotInfo->LoadRTCFullViewImg(false);                        //Steven 20110825 : Real Time CCD - 使用白色圖
                    break;
                case rtEncodeFail:
                    bRealTimeCom_ReceiveOK[rtEncodeFail]=true;
                    Comm4->WriteCommData(sRealTimeCom_Receive[rtRelease].c_str(), sRealTimeCom_Receive[rtRelease].Length());
                    ShowErrorMessage("WAR0344", 0, MMIndex);                    //RTC Feedback Check Error
                    bSendRealCCDSendStart=true;
                    bSendRealCCDSendVerify=true;
                    break;
                case rtPosFail:
                    bRealTimeCom_ReceiveOK[rtPosFail]=true;
                    Comm4->WriteCommData(sRealTimeCom_Receive[rtRelease].c_str(), sRealTimeCom_Receive[rtRelease].Length());
                    ShowErrorMessage("WAR0345", 0, MMIndex);                    //RTC Trigger Check Error
                    bSendRealCCDSendStart=true;
                    bSendRealCCDSendVerify=true;
                    break;
                case rtStartRepeat:
                    bRealTimeCom_ReceiveOK[rtStartRepeat]=true;
                    bSendRealCCDSendStart=false;
                    bSendRealCCDSendVerify=false;
                    break;
                case rtArmWork:                                                 //Steven 20120222 : ROI的學習次數
                    fContact->ROILearningTimes=atoi(CCDReceiveBuffer.SubString(9, 2).c_str());
                    bRealTimeCom_ReceiveOK[rtArmWork]=true;
                    break;
                case rtReLoad:
                    bRealTimeCom_ReceiveOK[rtReLoad]=true;
                    fLotInfo->LoadRTCFullViewImg(true);                         //Steven 20110825 : Real Time CCD - 顯示Full View Image
                    break;
                case rtVisionStop:
                    bRealTimeCom_ReceiveOK[rtVisionStop]=true;
                    bSendRealCCDSendStart=true;
                    bSendRealCCDSendVerify=true;
                    fMain->Pause("Handler paused by RTC");
                    break;
                case rtLightOn:                                                 //jou 2012-03-29 RTC啟動時,自動將燈箱打開
                    bRealTimeCom_ReceiveOK[rtLightOn]=true;
                    break;
                case rtLightOff:
                    bRealTimeCom_ReceiveOK[rtLightOff]=true;
                    break;
                case rtFullMOK:
                    bRealTimeCom_ReceiveOK[rtFullMOK]=true;
                    break;
                case rtFullMNG:
                    bRealTimeCom_ReceiveOK[rtFullMNG]=true;
                    break;
                case rtHalfMOK:
                    bRealTimeCom_ReceiveOK[rtHalfMOK]=true;
                    break;
                case rtHalfMNG:
                    bRealTimeCom_ReceiveOK[rtHalfMNG]=true;
                    break;
                case rtInspVerifyOK:                                            //Handler Use Model Verify
                    bRealTimeCom_ReceiveOK[rtInspVerifyOK]=true;
                    bSendRealCCDSendVerifyOK=true;
                    break;
                case rtInspVerifyNG:                                            //Handler Use Model Verify
                    bRealTimeCom_ReceiveOK[rtInspVerifyNG]=true;
                    ShowMyMessage("RTC Verify NG", "");
                    bSendRealCCDSendVerifyOK=false;
                    bSendRealCCDSendVerify=true;
                    break;
                case rtNeedVerify:                                              //jou 2014-06-24 RTC 自動進行Model驗證
                    if(CosFunction.bRTCAutoModelVerify==true &&
                       IniConfig.bD36EnableRTCAutoModelVerify==true &&
                       bNeedWaitRTCAutoVerify==false)
                    {
                        bRTCAutoModelVerifyFirstTime=true;
                    }
                    break;
                case rtROICount:                                                //jou 2013-11-25 Handler ROI COUNT
                    bRealTimeCom_ReceiveOK[rtROICount]=true;
                    RTC_ROICount();                                             //Steven 20241017 : 統一到function回覆ROICount
                    break;
                case rtROICountOK:                                              //jou 20171201 (Steven) : RTC ROI 確認數量是否正確
                    bRealTimeCom_ReceiveOK[rtROICountOK]=true;
                    bRealCCDROICountCheckOK=true;
                    break;
                case rtROICountNG:                                              //jou 20171201 (Steven) : RTC ROI 確認數量是否正確
                    bRealTimeCom_ReceiveOK[rtROICountNG]=true;
                    ShowMyMessage("RTC ROI Count Check NG","");
                    bRealCCDROICountCheck=true;
                    bRealCCDROICountCheckOK=false;
                    break;
                case rtSiteMap:                                                 //jou 2014-05-06 add RTC site map
                    asString=CCDReceiveBuffer.SubString(iStart, iStart+CCDReceiveBuffer.Length());
                    if(asString=="@MAPOK+")                                     //轉換成功
                    {
                        bRealTimeCom_ReceiveOK[rtSiteMap]=true;
                    }
                    else if(asString=="@MAPCODENG+")                            //傳碼錯誤
                    {
                        ShowMyMessage("RTC Site Map code NG","");
                    }
                    else if(asString=="@MAPCOUNTNG+")                           //ROI 設定數量錯誤,RTC滿site數量錯誤
                    {
                        ShowMyMessage("RTC Site Map count NG","");
                    }
                    break;
                case rtOPENVERIFYOK:                                            //jou 2014-06-24 RTC 自動進行Model驗證
                    bRealTimeCom_ReceiveOK[rtOPENVERIFYOK]=true;                //視窗開啟成功
                    break;
                case rtOPENVERIFYNG:                                            //jou 2014-06-24 RTC 自動進行Model驗證
                    bRealTimeCom_ReceiveOK[rtOPENVERIFYNG]=true;                //視窗開啟失敗 , 因為Handler未結束工作狀態
                    break;
                case rtALLPASSOK:                                               //jou 2014-06-24 RTC 自動進行Model驗證
                    bRealTimeCom_ReceiveOK[rtALLPASSOK]=true;
                    break;
                case rtALLPASSNG:                                               //jou 2014-06-24 RTC 自動進行Model驗證
                    bRealTimeCom_ReceiveOK[rtALLPASSNG]=true;
                    break;
                case rtALLFAILOK:                                               //jou 2014-06-24 RTC 自動進行Model驗證
                    bRealTimeCom_ReceiveOK[rtALLFAILOK]=true;
                    break;
                case rtALLFAILNG:                                               //jou 2014-06-24 RTC 自動進行Model驗證
                    bRealTimeCom_ReceiveOK[rtALLFAILNG]=true;
                    break;
                case rtCLOSEVERIFY:                                             //jou 2014-06-24 RTC 自動進行Model驗證
                    bRealTimeCom_ReceiveOK[rtCLOSEVERIFY]=true;
                    break;
                case rtROICheckOK:                                              //wei 20170308 (jou) RTC ROI Check
                    bRealTimeCom_ReceiveOK[rtROICheckOK]=true;
                    bRealCCDROICheckOK=true;
                    break;
                case rtROICheckNG:                                              //wei 20170308 (jou) RTC ROI Check
                    bRealTimeCom_ReceiveOK[rtROICheckNG]=true;
                    ShowMyMessage("RTC ROI Check NG","");
                    bRealCCDROICheck=true;
                    bRealCCDROICheckOK=false;
                    break;
                case rtModelNG:                                                 //wei 20170504 (Steven) RTC Model NG
                    bRealTimeCom_ReceiveOK[rtModelNG]=true;
                    bRTCModelNG=true;
                    MyDBIProcess("Process", "Handler paused by RTC");
                    fMain->BtnOneCycleClick(fMain);
                    break;
                case rtOpenLive:
                    bRealTimeCom_ReceiveOK[rtOpenLive]=true;
                    break;
                case rtCloseLive:
                    bRealTimeCom_ReceiveOK[rtCloseLive]=true;
                    break;
                case rtROIsize:
                    bRealTimeCom_ReceiveOK[rtROIsize]=true;
                    break;
                case rtCHECKNULL:                                               //Ifor 20190708 : add RTC 置偏殘料檢查命令
                    bRealTimeCom_ReceiveOK[rtCHECKNULL]=true;
                    break;
                case rtCHECKIC:                                                 //Ifor 20190708 : add RTC 置偏殘料檢查命令
                    bRealTimeCom_ReceiveOK[rtCHECKIC]=true;
                    break;
                case rtTESTMAP:                                                 //Ifor 20190722 : add RTC 開關Site檢測
                    bRealTimeCom_ReceiveOK[rtTESTMAP]=true;
                    break;
                case rtArm1AllPassVerify:                                       //JerryYang 20220215 : RTC Auto Verify half view check
                    bRealTimeCom_ReceiveOK[rtArm1AllPassVerify]=true;
                    break;
                case rtArm2AllPassVerify:
                    bRealTimeCom_ReceiveOK[rtArm2AllPassVerify]=true;
                    break;
                case rtArm1AllFailVerify:
                    bRealTimeCom_ReceiveOK[rtArm1AllFailVerify]=true;
                    break;
                case rtArm2AllFailVerify:
                    bRealTimeCom_ReceiveOK[rtArm2AllFailVerify]=true;
                    break;
               case rtFullGrabOK:                                               //Sam 20230419 : 新增 RTC Auto Tuning 功能
                    bRealTimeCom_ReceiveOK[rtFullGrabOK]=true;
                    break;
                case rtFullGrabNG:
                    bRealTimeCom_ReceiveOK[rtFullGrabNG]=true;
                    break;
                case rtFullGrabDummyBlackOK:
                    bRealTimeCom_ReceiveOK[rtFullGrabDummyBlackOK]=true;
                    break;
                case rtFullGrabDummyBlackNG:
                    bRealTimeCom_ReceiveOK[rtFullGrabDummyBlackNG]=true;
                    break;
                case rtFullGrabDummyWhiteOK:
                    bRealTimeCom_ReceiveOK[rtFullGrabDummyWhiteOK]=true;
                    break;
                case rtFullGrabDummyWhiteNG:
                    bRealTimeCom_ReceiveOK[rtFullGrabDummyWhiteNG]=true;
                    break;
                case rtFullGrabRealICOK:
                    bRealTimeCom_ReceiveOK[rtFullGrabRealICOK]=true;
                    break;
                case rtFullGrabRealICNG:
                    bRealTimeCom_ReceiveOK[rtFullGrabRealICNG]=true;
                    break;
                case rtLightAutoTuningOK:
                    bRealTimeCom_ReceiveOK[rtLightAutoTuningOK]=true;
                    break;
                case rtLightAutoTuningNG:
                    bRealTimeCom_ReceiveOK[rtLightAutoTuningNG]=true;
                    break;
                case rtParamAutoTuningOK:
                    bRealTimeCom_ReceiveOK[rtParamAutoTuningOK]=true;
                    break;
                case rtParamAutoTuningNG:
                    bRealTimeCom_ReceiveOK[rtParamAutoTuningNG]=true;
                    break;
                case rtFullAllCheckOK:
                    bRealTimeCom_ReceiveOK[rtFullAllCheckOK]=true;
                    break;
                case rtFullAllCheckNG:
                    bRealTimeCom_ReceiveOK[rtFullAllCheckNG]=true;
                    break;
                case rtAUTOTEACHON:
                    bRealTimeCom_ReceiveOK[rtAUTOTEACHON]=true;
                    break;
                case rtAUTOTEACHOFF:
                    bRealTimeCom_ReceiveOK[rtAUTOTEACHOFF]=true;
                    break;
                case rtSTEPASIDEMODEON:                                         //Ifor 20230908 add:RTC 讓位功能 By Setup File
                    bRealTimeCom_ReceiveOK[rtSTEPASIDEMODEON]=true;
                    break;
                case rtSTEPASIDEMODEOFF:
                    bRealTimeCom_ReceiveOK[rtSTEPASIDEMODEOFF]=true;
                    break;
                case rtAUTOSTDICPARATUNE:                                       //JerryYang 20240829 : SPIL訓永 要求手動觸發RTC AUTO STD
                    bRealTimeCom_ReceiveOK[rtAUTOSTDICPARATUNE]=true;
                    break;
                case rtAUTOSTDICPARATUNENG:
                    bRealTimeCom_ReceiveOK[rtAUTOSTDICPARATUNENG]=true;
                case rtALARMArm1NG:                                             //wei 20221222 RTC ARM Error
                    if(ScanBtnThd->GetRunState()==true)
                        bRealTimeCom_ReceiveOK[rtALARMArm1NG]=true;
                    break;
                case rtALARMArm2NG:                                             //wei 20221222 RTC ARM Error
                    if(ScanBtnThd->GetRunState()==true)
                        bRealTimeCom_ReceiveOK[rtALARMArm2NG]=true;
                    break;
                case rtCheckThirdPos:
                    bCheckThirdPos=true;
                    break;
            }
            CCDReceiveBuffer.Delete(iStart, iStart+sRealTimeCom_Receive[i].Length()-1);     //JerryYang 20210324 : 處理RTC指令連在一起
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::InitRealTimeCCDPara(bool bSendRealCCDStart)
{
    bSendRealCCDSendStart=bSendRealCCDStart;
    bSendRealCCDSendVerify=bSendRealCCDStart;
    bRealCCDSendArm=true;
    ScanBtnThd->SetArmState(false);

    ScanBtnThd->Stop();

    for(int j=0; j<CCD_MAX_CMD; j++)
    {
        if(bSendRealCCDStart==false &&
           (j==COM2->rtInspStart ||
            j==COM2->rtInspStartROI ||
            j==COM2->rtInspStartGolden ||
            j==COM2->rtInspStartUnKnow ||
            j==COM2->rtInspStartROIF)  ||                                       //Steven 20120523 : add j== COM2->rtInspStartROIF
            j==COM2->rtVISIONON          )                                      //ChungHung 20121005 add
            continue;
        COM2->bRealTimeCom_ReceiveOK[j]=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::DataModuleDestroy(TObject *Sender)
{
    TimerHPCard->Enabled=false;                                                 //Steven 20210524 : 連續讀取扭力
    Comm1->StopComm();
    Comm2->StopComm();
    Comm3->StopComm();
    Comm4->StopComm();
    TempComm6->StopComm();
    cmATC1->StopComm();
    cmATC2->StopComm();
    cmATC3->StopComm();
    cmATC4->StopComm();

    WriteVisionLight(0, 0);                                                     //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
    HTimer HDelayTime;
    HDelayTime.ResetMSAndOn(500);
    while(HDelayTime.Off()==false)
    {
    }
    cmVisionLight->StopComm();
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::SendCommToVision(RTComIndex rtCommand, bool bSetToFalse)
{
    if(InitialOK==false)
        return;

    if(rtCommand==COM2->rtFullTOK)                                              //Steven 20181105 : ADD RTC Full View Log
        RecordProcess("REALTIME CCD Start Full View Check");

    if(bSetToFalse)
        bRealTimeCom_ReceiveOK[rtCommand]=false;

    Comm4->WriteCommData(sRealTimeCom_Send[rtCommand].c_str(), strlen(sRealTimeCom_Send[rtCommand].c_str()));   //jou 2012-05-04 出現找不到cstring.h的錯誤,改用strlen
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::DoReleaseAndInspEnd()
{
    SendCommToVision(COM2->rtRelease, false);
    MySleep(100);
    SendCommToVision(COM2->rtInspEnd, false);
    if(CUSTOMER_CODE==CC_QUALCOMM)                                                  //RogerYang 20260119 : 100->50 for RF360
    {
        MySleep(50);
    }
    else
    {
        MySleep(100);                                                               //Ifor 20251023 add:避免RTC命令送太快導致RTC 時序異常Handler 報警Time Out
    }
    InitRealTimeCCDPara();
}
//---------------------------------------------------------------------------
int  __fastcall TCOM2::GetReadTorueTask()                                       //jou 2011-11-29防止Read Torue後數值被清掉，還傻傻的在那邊等
{
    return autoTask;
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::OmronComm5ReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    if(InitialOK==false)                                                        //Steven 20120202 : 加入Thread保護
        return;

    if(BufferLength<7)
        return;

//    if(BufferLength>250)
//       BufferLength=250;

    ComOmronBuffer.sprintf("%s", (char*)Buffer);
    ComOmronReceiveOK=true;                                                     //Steven 20120220 : Omron EJ1N溫控器
}
//---------------------------------------------------------------------------
unsigned char * TCOM2::ATCchksum(char *str)                                     //jou 2012-03-14 ATC start:
{
    static unsigned char str2[16];
    int L, A=0;
    L=strlen(str);
    for(int i=0; i<L; i++)
        A+=(int)str[i];
    A=A & 0xFF;
    ::sprintf(str2, "%X", A);

    return (unsigned char *)&str2[0];
}
//---------------------------------------------------------------------------
void TCOM2::ATCSendComm(int iAddr, AnsiString asString)
{
    char ATCSendBuffer[256];
    unsigned char str[256];
    unsigned char sum[16];
    ::sprintf(str, "%02d%s", 1, asString);
    ::strncpy(sum, ATCchksum(str), sizeof(sum));
    ::sprintf(ATCSendBuffer, "%cL%s%s%c", 2, str, sum, 3);

    if(iAddr==tcAa1)
        cmATC1->WriteCommData(ATCSendBuffer, ::strlen(ATCSendBuffer)+1);
    else if(iAddr==tcAb1)
        cmATC2->WriteCommData(ATCSendBuffer, ::strlen(ATCSendBuffer)+1);
    else if(iAddr==tcAa2)
        cmATC3->WriteCommData(ATCSendBuffer, ::strlen(ATCSendBuffer)+1);
    else if(iAddr==tcAb2)
        cmATC4->WriteCommData(ATCSendBuffer, ::strlen(ATCSendBuffer)+1);

    asString.sprintf("Send : %s", ATCSendBuffer);
    fMain->AddTorqueLog(asString);                                              //Steven 20160908 : 整合成Function
    bATCReceiveErr[iAddr]=false;
}
//---------------------------------------------------------------------------
void TCOM2::ATCFuntion_Read(int iAddr)
{
    ATCSendComm(iAddr, "00");
    iATCProcessTask[iAddr]=ATC_READ_STATUS_AND_PROCESS_VARIABLE;
    bATCWriteCommand[iAddr]=false;
//    fATCReadBuffer[iAddr]=999.9;
}
//---------------------------------------------------------------------------
void TCOM2::ATCFuntion_ReadFull(int iAddr)
{
    ATCSendComm(iAddr, "05");
    iATCProcessTask[iAddr]=ATC_READ_FULL_STATUS_COMMAND;
    bATCWriteCommand[iAddr]=false;
    asATCErrorString[iAddr]="No Error";
}
//---------------------------------------------------------------------------
void TCOM2::ATCFuntion_WriteSP1(int iAddr,double dSP1)
{
    AnsiString asString;
    int iTemp=dSP1*10;
    asString.printf("0200%04d00", iTemp);
    ATCSendComm(iAddr,asString);
    iATCProcessTask[iAddr]=ATC_WRITE_1SP1_COMMAND;
    ComATCReceiveOK[iAddr]=false;
    bATCWriteCommand[iAddr]=true;
}
//---------------------------------------------------------------------------
void TCOM2::ATCFuntion_WriteSP2(int iAddr,double dSP2)
{
    AnsiString asString;
    int iTemp=dSP2*10;
    asString.printf("0204%04d00", iTemp);
    ATCSendComm(iAddr,asString);
    iATCProcessTask[iAddr]=ATC_WRITE_SP2_COMMAND;
    ComATCReceiveOK[iAddr]=false;
    bATCWriteCommand[iAddr]=true;
}
//---------------------------------------------------------------------------
void TCOM2::ATCFuntion_WriteInPC(int iAddr,double dInPC)
{
    AnsiString asString;
    int iTemp=dInPC*10;
    iTemp=abs(iTemp);
    if(dInPC<0.0)
        asString.printf("024E%04d10", iTemp);
    else
        asString.printf("024E%04d00", iTemp);
    ATCSendComm(iAddr,asString);
    iATCProcessTask[iAddr]=ATC_WRTIE_INPC_COMMAND;
    ComATCReceiveOK[iAddr]=false;
    bATCWriteCommand[iAddr]=true;
}
//---------------------------------------------------------------------------
void TCOM2::ATCFuntion_ReadProcess(int iAddr)
{
    ATCSendComm(iAddr, "0100");
    iATCProcessTask[iAddr]=ATC_READ_1SP1_VARIABLE;
    bATCWriteCommand[iAddr]=false;
}
//---------------------------------------------------------------------------
void TCOM2::ATCFuntion_SetRemoteMode(int iAddr)
{
    ATCSendComm(iAddr, "0400");
    iATCProcessTask[iAddr]=ATC_SET_FOR_REMOTE_MODE;
    bATCWriteCommand[iAddr]=true;
}
//---------------------------------------------------------------------------
void TCOM2::cmATCReceiveData(int iAddr, Pointer Buffer, WORD BufferLength)
{
    int iATCSite[4]={tcAa1, tcAb1, tcAa2, tcAb2};
    AnsiString S, S1, S2;
    char str2[256];
    byte *data;
    data=(byte *)Buffer;

    if(BufferLength>256)                                                        //JimmyChiu 20210922 Avoid overflow
    {
        LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);
        fMain->AddTorqueLog(LogMsg);
        fMain->slTorqueLog->AddTextWithDateTime(LogMsg);
        return;
    }

    S2.printf("%s", data);
    S.printf("Receive : %s", Buffer);
    fMain->AddTorqueLog(S);                                                     //Steven 20160908 : 整合成Function

    for(int i=0; i<BufferLength; i++)
    {
        sprintf(str2," %02X", data[i]);
        S1+=str2;
    }

    fMain->AddTorqueLog(S1);                                                    //Steven 20160908 : 整合成Function
    bATCReceiveErr[iATCSite[iAddr]]=true;

    if(data[0]!=0x02)
    {
        //STX
        return;
    }

    if(data[1]!=0x4C)
    {
        //L
        return;
    }

    if(data[BufferLength-1]!=0x06)
    {
        //ACK
        return;
    }

    switch(iATCProcessTask[iATCSite[iAddr]])
    {
        case -1:
            //不處理
            break;
        case ATC_READ_STATUS_AND_PROCESS_VARIABLE:
            S2=S2.SubString(9,4);
//            lblTemp->Caption="Temp : "+AnsiString(atoi(S2.c_str())/10.0);
            fATCReadBuffer[iATCSite[iAddr]]=atof(S2.c_str())/10.0;
            bATCReceiveErr[iATCSite[iAddr]]=false;
            break;
        case ATC_READ_FULL_STATUS_COMMAND:
            if((data[4] & 0x1)==1)
                asATCErrorString[iATCSite[iAddr]]="OFL, Overflow Error";
            else if((data[4] & 0x2)==1)
                asATCErrorString[iATCSite[iAddr]]="Check Calibration Error";
            else if((data[4] & 0x8)==1)
                asATCErrorString[iATCSite[iAddr]]="Fail Test Error";
            else if((data[5] & 0x1)==1)
                asATCErrorString[iATCSite[iAddr]]="Area Error";
            else if((data[5] & 0x2)==1)
                asATCErrorString[iATCSite[iAddr]]="Open Input Error";
            else if((data[5] & 0x4)==1)
                asATCErrorString[iATCSite[iAddr]]="Bad Input Error";
            else if((data[5] & 0x8)==1)
                asATCErrorString[iATCSite[iAddr]]="UFL, Underflow Error";
            else if((data[6] & 0x4)==1)
                asATCErrorString[iATCSite[iAddr]]="Sensor Rate of Change Error";
            else if((data[6] & 0x8)==1)
                asATCErrorString[iATCSite[iAddr]]="Loop Break Error";
            else
            {
                asATCErrorString[iATCSite[iAddr]]="No Error";
                bATCReceiveErr[iATCSite[iAddr]]=false;
            }
            break;
        case ATC_READ_1SP1_VARIABLE:
            S2=S2.SubString(7, 4);
            break;
        case ATC_WRITE_1SP1_COMMAND:
        case ATC_WRITE_2SP1_COMMAND:
        case ATC_WRITE_SP2_COMMAND:
        case ATC_WRITE_A1LO_COMMAND:
        case ATC_WRITE_A1HI_COMMAND:
        case ATC_WRTIE_INPC_COMMAND:
            if((data[4]==0x30) && (data[5]==0x30))
            {
                bATCReceiveErr[iATCSite[iAddr]]=false;
            }
            else
            {
                asATCErrorString[iATCSite[iAddr]]="Error, Out of range, bad comand";
            }
            break;
        case ATC_SET_FOR_REMOTE_MODE:
            if((data[4]==0x30) && (data[5]==0x30))
            {
                bATCReceiveErr[iATCSite[iAddr]]=false;
            }
            else
            {
                asATCErrorString[iATCSite[iAddr]]="Error, bad command";
            }
            break;
    }
    iATCProcessTask[iATCSite[iAddr]]=0;
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::cmATC1ReceiveData(TObject *Sender, Pointer Buffer,
      WORD BufferLength)
{
    ComATCReceiveOK[tcAa1]=true;
    cmATCReceiveData(0, Buffer, BufferLength);
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::cmATC2ReceiveData(TObject *Sender, Pointer Buffer,
      WORD BufferLength)
{
    ComATCReceiveOK[tcAb1]=true;
    cmATCReceiveData(1, Buffer, BufferLength);
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::cmATC3ReceiveData(TObject *Sender, Pointer Buffer,
      WORD BufferLength)
{
    ComATCReceiveOK[tcAa2]=true;
    cmATCReceiveData(2, Buffer, BufferLength);
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::cmATC4ReceiveData(TObject *Sender, Pointer Buffer,
      WORD BufferLength)
{
    ComATCReceiveOK[tcAb2]=true;
    cmATCReceiveData(3, Buffer, BufferLength);
}
//---------------------------------------------------------------------------
bool TCOM2::ATCInitital()
{
    static int CommRetry[tcTotalCount];

    int iMAX_RETRY=2;
    int iATCSite[4]={tcAa1, tcAb1, tcAa2, tcAb2};
    bool bInitial=false;

    if(fTemp_Set->fShow==true)
    {
        return false;
    }

    for(int i=0; i<4; i++)
    {
        if(bUT150Install[iATCSite[i]]==false)
            continue;

        int &Task=iATCInitialTask[iATCSite[i]];
        switch(Task)
        {
            case 1:
                ATCFuntion_SetRemoteMode(iATCSite[i]);                          //Remote mode
                CommRetry[iATCSite[i]]=iMAX_RETRY;
                bATCInitialOK[i]=false;
                ATCTimeOut[i].SetSecAndOn(3);
                Task=100;
                break;
            case 100:
                if(ComATCReceiveOK[iATCSite[i]])
                {
                    ATCFuntion_ReadFull(iATCSite[i]);                           //read state & error
                    CommRetry[iATCSite[i]]=iMAX_RETRY;
                    ATCTimeOut[i].SetSecAndOn(3);
                    Task=200;
                    break;
                }

                if(ATCTimeOut[i].Off())
                {
                    if(CommRetry[iATCSite[i]]>0 &&
                       CommRetry[iATCSite[i]]<=iMAX_RETRY)
                    {
                        CommRetry[iATCSite[i]]--;
                        Task=1;
                    }
                    else
                    {
                        UN150CommError[iATCSite[i]]=true;
                        CommRetry[iATCSite[i]]=iMAX_RETRY;
                        UN150Read[iATCSite[i]]=999;
                        Task=1000;
                        break;
                    }
                }
                break;
            case 200:
                if(ComATCReceiveOK[iATCSite[i]])
                {
                    ATCTimeOut[i].SetSecAndOn(1);

                    if(bATCReceiveErr[iATCSite[i]]==true)                       //have error happen
                    {
                        if(CommRetry[iATCSite[i]]>0 &&
                           CommRetry[iATCSite[i]]<=iMAX_RETRY)
                        {
                            CommRetry[iATCSite[i]]--;
                            Task=100;
                        }
                        else
                        {
                            UN150CommError[iATCSite[i]]=true;
                            CommRetry[iATCSite[i]]=iMAX_RETRY;
                            UN150Read[iATCSite[i]]=999;
                            Task=1000;
                        }
                        break;
                    }

                    ATCFuntion_WriteSP2(iATCSite[i], Temperature.dATCSP2);      //寫溫度上限值
                    CommRetry[iATCSite[i]]=iMAX_RETRY;
                    ATCTimeOut[i].SetSecAndOn(1);
                    Task=300;
                    break;
                }

                if(ATCTimeOut[i].Off())
                {
                    if(CommRetry[iATCSite[i]]>0 && CommRetry[iATCSite[i]]<=iMAX_RETRY)
                    {
                        CommRetry[iATCSite[i]]--;
                        Task=100;
                    }
                    else
                    {
                        UN150CommError[iATCSite[i]]=true;
                        CommRetry[iATCSite[i]]=iMAX_RETRY;
                        UN150Read[iATCSite[i]]=999;
                        Task=1000;
                        break;
                    }
                }
                break;
            case 300:
                if(ComATCReceiveOK[iATCSite[i]])
                {
                    if(LastSet.iTemperature==Tempture_Hot ||
                       LastSet.iTemperature==Tempture_AmbientHot)               //kevin 2080811 (Steven) : 恆溫控制
                        ATCFuntion_WriteSP1(iATCSite[i], Temperature.fWorkTemperBase);
                    else
                        ATCFuntion_WriteSP1(iATCSite[i], 25.0);                  //寫工作溫度值
                    CommRetry[iATCSite[i]]=iMAX_RETRY;
                    ATCTimeOut[i].SetSecAndOn(1);
                    Task=400;
                    break;
                }

                if(ATCTimeOut[i].Off())
                {
                    if(CommRetry[iATCSite[i]]>0 && CommRetry[iATCSite[i]]<=iMAX_RETRY)
                    {
                        CommRetry[iATCSite[i]]--;
                        Task=1;
                    }
                    else
                    {
                        UN150CommError[iATCSite[i]]=true;
                        CommRetry[iATCSite[i]]=iMAX_RETRY;
                        UN150Read[iATCSite[i]]=999;
                        Task=1000;
                        break;
                    }
                }
                break;
            case 400:
                if(ComATCReceiveOK[iATCSite[i]])
                {
                    ATCFuntion_WriteInPC(iATCSite[i], Temperature.dATCInPC[i]); //寫溫度Offset值
                    CommRetry[iATCSite[i]]=iMAX_RETRY;
                    ATCTimeOut[i].SetSecAndOn(1);
                    Task=500;
                    break;
                }

                if(ATCTimeOut[i].Off())
                {
                    if(CommRetry[iATCSite[i]]>0 && CommRetry[iATCSite[i]]<=iMAX_RETRY)
                    {
                        CommRetry[iATCSite[i]]--;
                        Task=1;
                    }
                    else
                    {
                        UN150CommError[iATCSite[i]]=true;
                        CommRetry[iATCSite[i]]=iMAX_RETRY;
                        UN150Read[iATCSite[i]]=999;
                        Task=1000;
                        break;
                    }
                }
                break;
            case 500:
                if(ComATCReceiveOK[iATCSite[i]])
                {
                    CommRetry[iATCSite[i]]=iMAX_RETRY;
                    ATCTimeOut[i].SetSecAndOn(1);
                    Task=1000;
                    break;
                }

                if(ATCTimeOut[i].Off())
                {
                    if(CommRetry[iATCSite[i]]>0 && CommRetry[iATCSite[i]]<=iMAX_RETRY)
                    {
                        CommRetry[iATCSite[i]]--;
                        Task=1;
                    }
                    else
                    {
                        UN150CommError[iATCSite[i]]=true;
                        CommRetry[iATCSite[i]]=iMAX_RETRY;
                        UN150Read[iATCSite[i]]=999;
                        Task=1000;
                        break;
                    }
                }
                break;
            case 1000:
                CommRetry[iATCSite[i]]=iMAX_RETRY;
                bATCInitialOK[i]=true;
                break;
        }
    }

    bInitial=true;
    for(int i=0; i<4; i++)
    {
        if(bUT150Install[iATCSite[i]]==true && bATCInitialOK[i]==false)
        {
            bInitial=false;
        }
    }

    if(bUT150Install[iATCSite[0]]==false && bUT150Install[iATCSite[1]]==false &&
       bUT150Install[iATCSite[2]]==false && bUT150Install[iATCSite[3]]==false)
    {
        bInitial=false;
    }

    if(bInitial==true)
        return true;

    return false;
}
//---------------------------------------------------------------------------
void TCOM2::ATCInitialTask()
{
    int iATCSite[4]={tcAa1, tcAb1, tcAa2, tcAb2};

    if(ATC_SYSTEM==eATCSiliconType && Temperature.bATCActiveCooling==true &&
       (TestIF.iTestMode==DualSite || TestIF.iTestMode==SingleSite))
    {
        bATCInitialFinish=false;
        for(int i=0; i<4; i++)
        {
            iATCProcessTask[iATCSite[i]]=0;
            iATCInitialTask[iATCSite[i]]=1;
            bATCInitialOK[i]=false;
        }
    }
}
//---------------------------------------------------------------------------
bool TCOM2::ATCAlarmSenCheck()
{
    bool bRetrun=true;
    int iATCSite[4]={tcAa1, tcAb1, tcAa2, tcAb2};

    for(int i=0; i<4; i++)
    {
        if(bUT150Install[iATCSite[i]]==true && Sen[SnATCAlarm1+i].IsOff()==true)
        {
            bRetrun=false;
        }
    }
    return bRetrun;
}
//jou 2012-03-14 ATC end
//---------------------------------------------------------------------------
void TCOM2::RTC_ROICount()                                                      //jou 20171201 (Steven) : RTC ROI 確認數量是否正確
{
    int iColCT[4]={0, 0, 0, 0};
    AnsiString asString="";

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestIF.iSiteMap[i][j]>0)                                         //Steven 20241017 : 改成by Site Map設定每一Row的ROI數量
                iColCT[i]++;
        }
    }

    asString.printf("%s%02d%02d%02d%02d+", sRealTimeCom_Send[rtROICount], iColCT[0], iColCT[1], iColCT[2], iColCT[3]);
    Comm4->WriteCommData(asString.c_str(), asString.Length());
}
//---------------------------------------------------------------------------
int TCOM2::FunctionDataLength(HPCard_FUNCTION Tag)                              //Steven 20210524 : 連續讀取扭力
{
    if(Tag==HPCard_READ_Torque)
        iFunctionSize=22;
    else if(Tag==HPCard_READ_SETTING)
        iFunctionSize=20;
    else if(Tag==HPCard_WRITE_SETTING)
        iFunctionSize=20;
    else if(Tag==HPCard_START)
        iFunctionSize=19;
    else if(Tag==HPCard_STOP)
        iFunctionSize=25;
    else if(Tag==HPCard_ERR_CODE)
        iFunctionSize=20;
    else
        iFunctionSize=20;

    return iFunctionSize;
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::TimerHPCardTimer(TObject *Sender)                        //Steven 20210524 : 連續讀取扭力
{
    static RunTimer=false;
    AnsiString ProcessString;
    if(InitialOK==false || RunTimer==true)
        return;

    AnsiString S1, S2, S3, Arm;
    AnsiString ErrCode;
    int iErrCode;
    short int iValue=0;
    int iArm, iLength, iTotalCount;
    double dValue;
    RunTimer=true;
    if(_byte_datas.empty()==false)
    {
        if(ProcessData.empty())
            ProcessData.clear();

        int size=_byte_datas[0].size();
        for(int i=0; i<size; i++)
        {
            BYTE x=_byte_datas[0][i];
            ProcessData.push_back(x);
            S2.sprintf(" %02X", x);
            S1+=S2;
            S3+=MyDeCodeASCII(x);
        }
        fMain->AddTorqueLog(S1);
        fMain->AddTorqueLog(S3);
    }

    if(ProcessData.empty()==false)
    {
        //:@00A100A00+000.00%x@#                                                //讀取下壓的扭力
        //:@00A100A01+000.00%x@#
        if((int)ProcessData.size()>=9 &&
           ProcessData[0]==0x3A && ProcessData[1]==0x40 && ProcessData[2]==0x30 && ProcessData[3]==0x30 &&
           ProcessData[4]==0x41 && ProcessData[5]==0x31 && ProcessData[6]==0x30 && ProcessData[7]==0x30 && ProcessData[8]==0x41)
        {
            bTorueReceve=true;                                                  //kevin 20211117 扭力100 筆資料
            iLength=FunctionDataLength(HPCard_READ_Torque);
            if((int)ProcessData.size()>=iLength)
            {
                Arm.sprintf("%c%c", ProcessData[9], ProcessData[10]);
                iArm=HexStrToInt(Arm);
                asReceiveTorue="";
                for(int i=11; i<18; i++)
                {
                    asReceiveTorue+=MyDeCodeASCII(ProcessData[i]);              //kevin 20210824 12
                }

                if(iArm==1)
                {
                    if(bReadMCU1)                                               //kevin 20220225 read MCU DATA
                    {
                       bReadMCU1=false;
                       fMain->edTorue0->Text=asReceiveTorue;
                       fContact->PnlTorue0->Caption=asReceiveTorue;
                    }
                }
                else if(iArm==2)
                {
                    if(bReadMCU2)                                               //kevin 20220225 read MCU DATA
                    {
                        bReadMCU2=false;
                        fMain->edTorue1->Text=asReceiveTorue;
                        fContact->PnlTorue1->Caption=asReceiveTorue;
                    }
                }

                ProcessString="";
                for(int i=0; i<iLength; i++)
                    ProcessString+=MyDeCodeASCII(ProcessData[i]);

                ProcessData.erase(ProcessData.begin(), ProcessData.begin()+iLength);
                S1.sprintf("RECV ReadTrq (%d): %s", iArm, ProcessString);
                fMain->AddTorqueLog(S1);
                if(IndexStatus==Z1_Z2_Down)                                     //KenHsieh 20220409 : nn Mode扭力紀錄
                {
                    if(iArm==2)
                    {
                        bReceive1=true;
                        bGetValue1=true;
                    }

                    if(iArm==1)
                    {
                        bReceive=true;
                        bGetValue=true;
                    }
                }
                else
                {
                    bReceive=true;
                    bGetValue=true;                                             //KenHsieh 20220317 : 確認有Get value後Test Arm才能上升
                }
            }
        }
        //:@00A500801R00000x@#      //讀取目前設定的扭力
        //:@00A500802R00000x@#
        else if((int)ProcessData.size()>=9 &&
                ProcessData[0]==0x3A && ProcessData[1]==0x40 && ProcessData[2]==0x30 && ProcessData[3]==0x30 &&
                ProcessData[4]==0x41 && ProcessData[5]==0x35 && ProcessData[6]==0x30 && ProcessData[7]==0x30 && ProcessData[8]==0x38)
        {
            iLength=FunctionDataLength(HPCard_WRITE_SETTING);
            if((int)ProcessData.size()>=iLength)
            {
                Arm.sprintf("%c%c", ProcessData[9], ProcessData[10]);
                iArm=HexStrToInt(Arm);
                asReceiveTorue="";
                for(int i=12; i<17; i++)
                {
                    asReceiveTorue+=MyDeCodeASCII(ProcessData[i]);
                }
                iValue=atoi(asReceiveTorue.c_str());
                asReceiveTorue=AnsiString(iValue);
                if(iArm==1)
                {
                    fMain->edtReadZ1->Text=asReceiveTorue;
                }
                else if(iArm==2)
                {
                    fMain->edtReadZ2->Text=asReceiveTorue;
                }

                ProcessString="";
                for(int i=0; i<iLength; i++)
                    ProcessString+=MyDeCodeASCII(ProcessData[i]);

                ProcessData.erase(ProcessData.begin(), ProcessData.begin()+iLength);
                S1.sprintf("RECV SetTrq (%d): %s", iArm, ProcessString);
                fMain->AddTorqueLog(S1);
                bReceive=true;
            }
        }
        //:@00A700801R00000x@#      //寫入要設定的扭力
        //:@00A700802R00000x@#
        else if((int)ProcessData.size()>=9 &&
                ProcessData[0]==0x3A && ProcessData[1]==0x40 && ProcessData[2]==0x30 && ProcessData[3]==0x30 &&
                ProcessData[4]==0x41 && ProcessData[5]==0x37 && ProcessData[6]==0x30 && ProcessData[7]==0x30 && ProcessData[8]==0x38)
        {
            iLength=FunctionDataLength(HPCard_WRITE_SETTING);
            if((int)ProcessData.size()>=iLength)
            {
                Arm.sprintf("%c%c", ProcessData[9], ProcessData[10]);
                iArm=HexStrToInt(Arm);
                asReceiveTorue="";
                for(int i=12; i<17; i++)
                {
                    asReceiveTorue+=MyDeCodeASCII(ProcessData[i]);
                }
                iValue=atoi(asReceiveTorue.c_str());
                asReceiveTorue=AnsiString(iValue);
                if(iArm==1)
                {
                    fMain->edtSetZ1->Text=asReceiveTorue;
                }
                else if(iArm==2)
                {
                    fMain->edtSetZ2->Text=asReceiveTorue;
                }

                ProcessString="";
                for(int i=0; i<iLength; i++)
                    ProcessString+=MyDeCodeASCII(ProcessData[i]);

                ProcessData.erase(ProcessData.begin(), ProcessData.begin()+iLength);
                S1.sprintf("RECV ReadSetTrq (%d): %s", iArm, ProcessString);
                fMain->AddTorqueLog(S1);
                bReceive=true;
            }
        }
        //:@00FF00801ERRXXX'@#
        //:@00FF00802ERRXXX'@#  //錯誤代碼
        else if((int)ProcessData.size()>=9 &&
                ProcessData[0]==0x3A && ProcessData[1]==0x40 && ProcessData[2]==0x30 && ProcessData[3]==0x30 &&
                ProcessData[4]==0x46 && ProcessData[5]==0x46 && ProcessData[6]==0x30 && ProcessData[7]==0x30 && ProcessData[8]==0x38)
        {
            iLength=FunctionDataLength(HPCard_ERR_CODE);
            if((int)ProcessData.size()>=iLength)
            {
                AnsiString Err[]={"EOT error", "Time out error", "CMD error", "RS485 error", "LRC error", "NACK error", "String error", "Other error"};
                Arm.sprintf("%c%c", ProcessData[9], ProcessData[10]);
                ErrCode.sprintf("%c%c%c", ProcessData[14], ProcessData[15], ProcessData[16]);
                iErrCode=atoi(ErrCode.c_str());

                ProcessString="";
                for(int i=0; i<iLength; i++)
                    ProcessString+=MyDeCodeASCII(ProcessData[i]);

                ProcessData.erase(ProcessData.begin(), ProcessData.begin()+iLength);
                if(iErrCode<7)
                    S1.sprintf("RECV ErrorCode %s, %s: %s, %s", Arm, ErrCode, ProcessString, Err[iErrCode]);
                else
                    S1.sprintf("RECV ErrorCode %s, %s: %s, %s", Arm, ErrCode, ProcessString, Err[7]);
                fMain->AddTorqueLog(S1);
                fMain->slTorqueLogNew->AddTextWithDateTime(S1);
                if(fAllMotorHome)
                {
                    if(S1.Pos("@00FF00801"))                                    //kevin 20211215 add error
                    {
                        RunTimer=false;                                         //KenHsieh 20220127 : MCU修改
                        ShowErrorMessage("WAR03604", K_RETRY, MMSystem);
                    }
                    else if(S1.Pos("@00FF00802"))
                    {
                        RunTimer=false;                                         //KenHsieh 20220127 : MCU修改
                        ShowErrorMessage("WAR03605", K_RETRY, MMSystem);
                    }
                }
            }
        }
        //:@00A800701START'@#
        //:@00A800702START&@#   //連續讀取扭力
        else if((int)ProcessData.size()>=9 &&
                ProcessData[0]==0x3A && ProcessData[1]==0x40 && ProcessData[2]==0x30 && ProcessData[3]==0x30 &&
                ProcessData[4]==0x41 && ProcessData[5]==0x38 && ProcessData[6]==0x30 && ProcessData[7]==0x30 && ProcessData[8]==0x37)
        {
            bTorueReceve=true;                                                  //kevin 20211117 扭力100 筆資料
            iLength=FunctionDataLength(HPCard_START);
            if((int)ProcessData.size()>=iLength)
            {
                ProcessString="";
                for(int i=0; i<iLength; i++)
                    ProcessString+=MyDeCodeASCII(ProcessData[i]);

                ProcessData.erase(ProcessData.begin(), ProcessData.begin()+iLength);
                S1.sprintf("RECV StartTrq (%d): %s", iArm, ProcessString);
                fMain->AddTorqueLog(S1);
            }
        }
        //:@00A900601STOPx@#:@00,...@#
        //:@00A900602STOPx@#:@00,...@#     //讀取暫存的扭力值
        else if((int)ProcessData.size()>=9 &&
                ProcessData[0]==0x3A && ProcessData[1]==0x40 && ProcessData[2]==0x30 && ProcessData[3]==0x30 &&
                ProcessData[4]==0x41 && ProcessData[5]==0x39 && ProcessData[6]==0x30 && ProcessData[7]==0x30 && ProcessData[8]==0x36)
        {
            bTorueReceveSTOP=true;                                              //kevin 20211117 扭力STOP資料
            iLength=FunctionDataLength(HPCard_STOP);
            if((int)ProcessData.size()>=iLength)
            {
                Arm.sprintf("%c%c", ProcessData[9], ProcessData[10]);
                iArm=HexStrToInt(Arm)-1;
                S1.sprintf("%02X", ProcessData[21]);
                iTotalCount=HexStrToInt(S1);

                iLength=iLength+iTotalCount*3;
                if((int)ProcessData.size()>=iLength)
                {
                    if(iArm>=0 && iArm<2)
                    {
                        iValue=0;
                        dValue=0.0;
                        ProcessString="";
                        iTorqueArrayCnt[iArm]=0;
                        if(iTotalCount!=0)
                        {
                            for(int i=23; i<iLength-4; i+=3)
                            {
                                iValue=ProcessData[i]<<8;
                                iValue+=ProcessData[i+1];
                                dValue=double(iValue)/20.0;
                                S1.sprintf("%0.2f,", dValue);
                                ProcessString+=S1;
                                if((iArm==0 || iArm==1) &&
                                   iTorqueArrayCnt[iArm]<2000)
                                {
                                    dTorqueArray[iArm][iTorqueArrayCnt[iArm]]=dValue;
                                    iTorqueArrayCnt[iArm]++;
                                }
                            }
                        }

                        if(iCheckTorqueCount<1)
                        {
                            if(iTorqueArrayCnt[iArm]>0)
                                fMain->chtTorque->Series[iArm]->AddArray(dTorqueArray[iArm], iTorqueArrayCnt[iArm]-1);
                            iTorqueArrayCnt[iArm]=0;
                        }

                        S1.sprintf("RECV GetTrq %d, %d, %s", iArm+1, iTotalCount, ProcessString);
                        fMain->AddTorqueLog(S1);
                        fMain->slTorqueLogNew->AddTextWithDateTime(S1);
                    }

                    ProcessString="";
                    for(int i=0; i<iLength; i++)
                        ProcessString+=MyDeCodeASCII(ProcessData[i]);

                    ProcessData.erase(ProcessData.begin(), ProcessData.begin()+iLength);
                    S1.sprintf("RECV GetTrq (%d): %s", iArm+1, ProcessString);
                    fMain->AddTorqueLog(S1);
                }
            }
        }
        else if((int)ProcessData.size()>=2 &&                                   //:@    //刪除沒使用的資料
                ProcessData[0]==0x3A && ProcessData[1]==0x40)
        {
            ProcessString="";
            iLength=0;
            for(int i=0; i<(int)ProcessData.size()-1; i++)
            {
                ProcessString+=MyDeCodeASCII(ProcessData[i]);
                if(ProcessData[i]==0x40 && ProcessData[i+1]==0x23)              //@#
                {
                    ProcessString+=MyDeCodeASCII(ProcessData[i]);
                    ProcessString+=MyDeCodeASCII(ProcessData[i+1]);
                    iLength=i+1;
                    break;
                }
            }

            if(iLength!=0)
            {
                ProcessData.erase(ProcessData.begin(), ProcessData.begin()+iLength);
                S1.sprintf("RECV Data : %s", ProcessString);
                fMain->AddTorqueLog(S1);
            }
        }
        else if(ProcessData.empty()==false)                                     //刪除錯誤資料
        {
            ProcessString="";
            iLength=0;
            for(int i=0; i<(int)ProcessData.size()-1; i++)
            {
                ProcessString+=MyDeCodeASCII(ProcessData[i]);
                if(ProcessData[i]==0x3A && ProcessData[1+1]==0x40)              //:@ 新的開頭, 刪除他之前的
                {
                    iLength=i;
                    break;
                }
                else if(ProcessData[i]==0x40 && ProcessData[i+1]==0x23)         //@#   只有結尾
                {
                    ProcessString+=MyDeCodeASCII(ProcessData[i]);
                    ProcessString+=MyDeCodeASCII(ProcessData[i+1]);
                    iLength=i+1;
                    break;
                }
            }

            if(iLength!=0)
            {
                ProcessData.erase(ProcessData.begin(), ProcessData.begin()+iLength);
                S1.sprintf("RECV Data : %s", ProcessString);
                fMain->AddTorqueLog(S1);
            }
        }
    }

    if(_byte_datas.empty()==false)
        _byte_datas.erase(_byte_datas.begin(), _byte_datas.begin()+1);          //做完才刪除

    RunTimer=false;
}
//---------------------------------------------------------------------------
void TCOM2::AddTryData()
{
    DATA data;
    data.push_back(0x3A);
    data.push_back(0x40);
    data.push_back(0x30);
    data.push_back(0x30);
    data.push_back(0x41);
    data.push_back(0x31);
    data.push_back(0x30);
    data.push_back(0x30);
    data.push_back(0x36);
    data.push_back(0x30);
    data.push_back(0x31);
    data.push_back(0x2B);
    data.push_back(0x30);
    data.push_back(0x30);
    data.push_back(0x30);
    data.push_back(0x2E);
    data.push_back(0x30);
    data.push_back(0x30);
    data.push_back(0x25);
    data.push_back(0x78);
    data.push_back(0x40);
    data.push_back(0x23);

    _byte_datas.push_back(data);
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::PadCommReceiveData(TObject *Sender, Pointer Buffer,
      WORD BufferLength)
{                                                                                //Ken 20210805 AddPadInterface
    vector<Byte> ReData;
    AnsiString asModule, asCommand="";
    ReData.clear();

    char *buf=new char [BufferLength+1];
    memset(buf, '\0', sizeof(buf));

    for(int i=0; i<BufferLength; i++)
    {
        ReData.push_back(*((Byte*)Buffer+i));
    }
    buf=(char*)Buffer;
    buf[BufferLength]='\0';
    for(int i=0; i<BufferLength; i++)
    {
        asCommand+=(char)buf[i];
    }

    asModule=asCommand.SubString(2, 2);

    if(asModule!="05")
    {
        return;
    }

    fPadInterface->CommReceiveLength->Add(BufferLength);
    fPadInterface->CommReceiveList->Add(asCommand);

    fPadInterface->RecordCommunication("[Recive]", asCommand);
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::Comm1ReceiveError(TObject *Sender, DWORD EventMask)
{
     bCom1Error=true;                                                           //kevin 20210902 Torque error
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::Comm1RequestHangup(TObject *Sender)
{
     bCom1Error=true;                                                           //kevin 20210902 Torque error
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::WriteVisionLight(int iVisionLight1,int iVisionLight2)    //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
{
    if(USE_Fix_AI_CCD==0)                                                       //Steven 20260423 : add protection
        return;
    if(bVisionLightCommOK==false)                                               //AI(appexception-av-log-flood) 20260617 (RogerYang) : COM8 未開啟成功，不可對無效 handle 寫入 (否則 ntdll Access violation)
        return;

    char cVisionLight[32]="";
//    char cEnd[2]={0x0d,0x0a};

//    sprintf(cVisionLight, "1,%d,2,%d,3,0,4,0 \r\n",iVisionLight1,iVisionLight2);
    sprintf(cVisionLight, "1,%d,2,%d \r\n",iVisionLight1,iVisionLight2);
//    sprintf(cVisionLight, "1,%d \r\n",iVisionLight1);
    cmVisionLight->WriteCommData(cVisionLight,strlen(cVisionLight));
}
//---------------------------------------------------------------------------
void __fastcall TCOM2::cmVisionLightReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)                                        //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
{
    if(InitialOK==false)                                                        //Steven 20120202 : 加入Thread保護
        return;

    bRS232Delay=false;
}
//---------------------------------------------------------------------------
bool TCOM2::RTC_AlarmType()                                                     //wei 20221222 RTC ARM Error
{
    bool bRetrun=false;
    if(COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC] ||
       COM2->bRealTimeCom_ReceiveOK[COM2->rtALARMArm1NG] ||
       COM2->bRealTimeCom_ReceiveOK[COM2->rtALARMArm2NG])
    {
        bRetrun=true;
    }

    return bRetrun;
}
//---------------------------------------------------------------------------
void TCOM2::RTC_ResetAlarm()                                                    //wei 20221222 RTC ARM Error
{
    COM2->bRealTimeCom_ReceiveOK[COM2->rtAlarHasIC]=false;
    COM2->bRealTimeCom_ReceiveOK[COM2->rtALARMArm1NG]=false;
    COM2->bRealTimeCom_ReceiveOK[COM2->rtALARMArm2NG]=false;
}
//---------------------------------------------------------------------------