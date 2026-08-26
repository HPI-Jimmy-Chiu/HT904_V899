//---------------------------------------------------------------------------
#ifndef rs232H
#define rs232H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "SPComm.hpp"
#include "myTimer.h"
#include <ExtCtrls.hpp>
#include "TCPData.h"
//---------------------------------------------------------------------------
class TCOM2 : public TDataModule
{
__published:    // IDE-managed Components
    TComm *Comm2;
    TComm *Comm1;
    TComm *Comm3;
    TComm *TempComm6;
    TComm *Comm4;
    TComm *cmATC1;
    TComm *cmATC2;
    TComm *cmATC3;
    TComm *cmATC4;
    TTimer *TimerHPCard;
    TComm *PadComm;
    TComm *cmVisionLight;
    void __fastcall Comm2ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall Comm1ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall Comm3ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall TempComm6ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall Comm4ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall DataModuleDestroy(TObject *Sender);
    void __fastcall OmronComm5ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall cmATC1ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall cmATC2ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall cmATC3ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall cmATC4ReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall TimerHPCardTimer(TObject *Sender);
    void __fastcall PadCommReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall Comm1ReceiveError(TObject *Sender, DWORD EventMask);
    void __fastcall Comm1RequestHangup(TObject *Sender);
    void __fastcall cmVisionLightReceiveData(TObject *Sender,
          Pointer Buffer, WORD BufferLength);
private:    // User declarations
    int autoTask;
    AnsiString LogMsg;                                                          //JimmyChiu 20210922 Avoid overflow

    bool bATCInitialOK[4];                                                      //jou 2012-03-14 ATC start:
    unsigned char * ATCchksum(char *str);
    void ATCSendComm(int iAddr,AnsiString asString);
    void ATCFuntion_WriteSP1(int iAddr, double dSP1);                           //ATC Write溫度值
    void ATCFuntion_WriteSP2(int iAddr, double dSP2);                           //ATC Write Alarm溫度值
    void ATCFuntion_WriteInPC(int iAddr, double dInPC);                         //ATC Write 溫度 offset
    void ATCFuntion_ReadFull(int iAddr);
    void ATCFuntion_ReadProcess(int iAddr);
    void ATCFuntion_SetRemoteMode(int iAddr);
    void cmATCReceiveData(int iAddr, Pointer Buffer, WORD BufferLength);
public:     // User declarations
    __fastcall TCOM2(TComponent* Owner);
    void __fastcall TCOM2::RS232Init();
    void __fastcall TorqueSend(unsigned char *str, int len);

    //讀取扭力回饋值------------------------
    void __fastcall ReadTorque();
    void __fastcall ReadTorque_Panasonic();
    bool __fastcall ReadTorque_Mitsubishi();
    void __fastcall ReadTorque_HPCard();
    int __fastcall StartReadTorque_HPCard();
    int __fastcall StopReadTorque_HPCard();                                     //kevin 20211207 : 改成一送一收
    void __fastcall ReadVer_HPCard();                                           //kevin 20211110 : 讀取韌體 版號
    int iReadTorque_HPCardTask;                                                 //Steven 20211116 : 改成一送一收
    bool bRecvData;                                                             //Steven 20211116 : 改成一送一收
    int iReadSTopTorque_HPCardTask;                                             //Steven 20211116 : 改成一送一收

    //讀寫扭力設定值------------------------
    void __fastcall ReadWriterParameter();
    void __fastcall ReadWriterParameter_Panasonic();
    void __fastcall ReadWriterParameter_Mitsubishi();
    void __fastcall ReadWriterParameter_HPCard();

    //讀寫扭力設定值------------------------
    void WriteIndexTorqueSetting(int Index, AnsiString Torque);
    void ReadIndexTorqueSetting(int Index);

    void WriteIndexTorqueSetting_Pana(int Index, unsigned Data);
    void ReadIndexTorqueSetting_Pana(int index);

    void __fastcall WriteIndexTorqueSetting_Mitu(int iIndex, double fTorque);
    void __fastcall ReadIndexTorqueSetting_Mitu(int iIndex);

    void WriteIndexTorqueSetting_HPCard(int Index, unsigned Data);
    void ReadIndexTorqueSetting_HPCard(int index);

    double __fastcall DoChangeASCII_TO_INT(char *Input);                        //把ascii碼轉成數字.
    bool __fastcall DoSet_Torque_Action(int iIndex, double fTorque);            //寫入需要的扭力值.
    bool __fastcall DoRead_Now_Torque_Action(int iIndex);                       //讀取Driver設定的扭力值.
    void __fastcall InitSetTorque_Mitsubishi();
    void __fastcall InitReadTorque_Mitsubishi();
    int  __fastcall iWriteAndCheckMotorTorque(int MotorIndex, int Torque);
    void ResetPanasonicTime();

    void __fastcall ReadHP34970A(int Index);
    void __fastcall SendStartTest(int iComMPtr, char *str);
    void __fastcall SendSKYMEDITimeOut(int iSite);
    void __fastcall SendSMITimeOut();

    bool bReadHP34970A;
    double fDataHP34970A[20];
    TComm *CommSD[4];
    bool  flagCommSD[4];
    AnsiString asReceiveTorue;
    void InitReadTorueTask();

    //----- by dell ccd realtime-------------
    enum RTComIndex{rtHome=0,               rtInspStart=1,      rtInspStartROI=2,       rtInspStartGolden=3,    rtInspStartUnKnow=4,
                    rtInspEnd=5,            rtArmIndex1=6,      rtArmIndex2=7,          rtFileOK=8,             rtAlarHasIC=9,
                    rtAlarGrabTimeOut=10,   rtClear=11,         rtRelease=12,           rtRsOK=13,              rtFullLOK=14,
                    rtFullLNG=15,           rtFullTOK=16,       rtFullTNG=17,           rtPassOn=18,            rtReall=19,
                    rtPassOff=20,           rtInspStartROIF=21, rtInspStartGoldenF=22,  rtSite=23,              rtArmFinish=24,
                    rtNODEVICE=25,          rtNOMODEL=26,       rtMODEL=27,             rtGETMODEL=28,          rtVISIONON=29,
                    rtVISIONOFF=30,         rtDELETE=31,        rtReallNG=32,           rtLightReal,            rtLightBox,
                    rtStartLightF,          rtAlarEncode,       rtReLoad,               rtEncodeFail,           rtPosFail,
                    rtStartRepeat,          rtArmWork,          rtVisionStop,           rtLightOn,              rtLightOff,
                    rtFullMOK,              rtFullMNG,          rtHalfMOK,              rtHalfMNG,              rtInspVerifyOK,
                    rtInspVerifyNG,         rtROICount,                                                                                         //ChungHung 20130122 add for SCK
                    rtSiteMap,              rtOPENVERIFYOK,     rtOPENVERIFYNG,         rtALLPASSOK,            rtALLPASSNG,
                    rtALLFAILOK,            rtALLFAILNG,        rtCLOSEVERIFY,          rtROICheckOK,           rtROICheckNG,                   //wei 20170308 (jou) RTC ROI Check
                    rtModelNG,              rtNeedVerify,       rtROICountOK,           rtROICountNG,           rtOpenLive,     rtCloseLive,    //wei 20170504 RTC Model NG  //jou 20180420 : RTC Open/Close Live
                    rtROIsize,              rtTIMESYNC,                                                                                         //jou 20180724 : 新增RTC ROI size command
                    rtCHECKNULL,            rtCHECKIC,          rtTESTMAP,              rtArm1AllPassVerify,    rtArm2AllPassVerify,            //Ifor 20190708 : add RTC 置偏殘料檢查命令
                    rtArm1AllFailVerify,    rtArm2AllFailVerify,                                                                                //JerryYang 20220215 : RTC Auto Verify half view check
                    rtFullGrabOK,           rtFullGrabNG,       rtFullGrabDummyBlackOK, rtFullGrabDummyBlackNG,                                 //Sam 20230419 : 新增 RTC Auto Tuning 功能
                    rtFullGrabDummyWhiteOK, rtFullGrabDummyWhiteNG,  rtFullGrabRealICOK,rtFullGrabRealICNG,
                    rtLightAutoTuningOK,    rtLightAutoTuningNG,rtParamAutoTuningOK,    rtParamAutoTuningNG,
                    rtFullAllCheckOK,       rtFullAllCheckNG,   rtAUTOTEACHON,          rtAUTOTEACHOFF,
                    rtSTEPASIDEMODEON,      rtSTEPASIDEMODEOFF, rtAUTOSTDICPARATUNE,    rtAUTOSTDICPARATUNENG,
                    rtErrorType,            rtALARMArm1NG,      rtALARMArm2NG,          rtCheckThirdPos,
                    CCD_MAX_CMD //這個一定得在最下面,不可以動
                   };

    AnsiString sRealTimeCom_Send[CCD_MAX_CMD];
    AnsiString sRealTimeCom_Receive[CCD_MAX_CMD];
    bool bRealTimeCom_ReceiveOK[CCD_MAX_CMD];

    bool bRTCVerSupportAutoTurnning;                                            //Sam 20240425 : 啟動時傳送 AutoTeach 功能，若沒有回應代表不支援就不要顯示。
    AnsiString CCDReceiveBuffer;
    bool bCCDDummyRum;
    void __fastcall InitRealTimeCCDPara(bool bSendRealCCDStart=true);
    void __fastcall SendCommToVision(RTComIndex rtCommand, bool bSetToFalse=false);
    void __fastcall DoReleaseAndInspEnd();
    //---------------------------------------
    int  __fastcall GetReadTorueTask();                                         //jou 2011-11-29防止Read Torue後數值被清掉，還傻傻的在那邊等

    void __fastcall InitRTCComPortAgain();                                      //ChungHung 20140520 add 不要第一次TimeOut就秀錯誤訊息
    int iOpenRTCComPortAgainCount;                                              //ChungHung 20140520 add 不要第一次TimeOut就秀錯誤訊息
    bool __fastcall OpenRTCComPortAgain();                                      //ChungHung 20121005 add

    void ATCInitialTask();                                                      //jou 2012-03-14 ATC start:
    bool ATCInitital();                                                         //ATC Initial
    void ATCFuntion_Read(int iAddr);                                            //ATC Read度值
    bool ATCAlarmSenCheck();

    int  rwCommandDelay;                                                        //2013-01-15    Dell    //設定通訊的delay時間
    TQPF_Timer iWriteAndCheckMotorTorqueDelay;                                  //Steven 20130701 : 避免暫停時Torque會Time Out

    void RTC_ROICount();                                                        //jou 20171201 (Steven) : RTC ROI 確認數量是否正確
    bool RTC_AlarmType();   //wei 20221222 RTC ARM Error
    void RTC_ResetAlarm();  //wei 20221222 RTC ARM Error
    bool fPanasonicParameterRW;                                                 //Read & Write Driver Parameter Flag

    DATAs       _byte_datas;
    DATA       ProcessData;

    enum HPCard_FUNCTION
    {
        HPCard_SEND              =-1,
        HPCard_READ_Torque       =0,
        HPCard_READ_SETTING      =1,
        HPCard_WRITE_SETTING     =3,
        HPCard_START             =4,
        HPCard_STOP              =5,
        HPCard_ERR_CODE          =6,
    };

    int FunctionDataLength(HPCard_FUNCTION Tag);
    int iFunctionSize;
    void AddTryData();
    void __fastcall WriteVisionLight(int iVisionLight1, int iVisionLight2);     //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
    bool bVisionLightCommOK;                                                    //AI(appexception-av-log-flood) 20260617 (RogerYang) : COM8(Vision Light)是否真的開啟成功，false 時禁止寫入避免 ntdll AV 灌爆 log
    bool bGetValue;                                                             //KenHsieh 20220317 : 確認有Get value後Test Arm才能上升
    bool bGetValue1;                                                            //KenHsieh 20220409 : nn Mode扭力紀錄
};
//---------------------------------------------------------------------------
extern PACKAGE TCOM2 *COM2;
//---------------------------------------------------------------------------
void InitWriteAndCheckMotorTorqueTask();

#endif