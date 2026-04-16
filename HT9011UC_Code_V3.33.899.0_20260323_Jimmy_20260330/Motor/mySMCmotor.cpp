#include "MachineDefine.h"
#pragma hdrstop

#include "mySMCmotor.h"
#include "common.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

#define MAX_SMC_CARD 16
#define MAX_LATCH_COUNT 1024

//-------------
// SmcWSetCtrlTypeIn : Sets the type of control input signal.
//-------------
const int SMC_CtrlIn_ALM            =0x01;
const int SMC_CtrlIn_INP            =0x02;
const int SMC_CtrlIn_SD             =0x04;
const int SMC_CtrlIn_LTC            =0x08;
const int SMC_CtrlIn_CTR            =0x10;
const int SMC_CtrlIn_CLR            =0x20;

//-------------
// SmcWSetCtrlTypeOut : Sets the type of control output signal
//-------------
const int SMC_CtrlOut_General       =0;
const int SMC_CtrlOut_AlarmClear    =1;
const int SMC_CtrlOut_ERC           =2;
const int SMC_CtrlOut_OutPulseSignal=3;
const int SMC_CtrlOut_EncoderSignal =4;
const int SMC_CtrlOut_HoldOffSignal =5;

//-------------
// SmcWSetCtrlInOutLog : Sets the logic of I/O control signal.  for Positive logic
//-------------
const int SMC_CtrlInOutLog_IN1      =0x0001;
const int SMC_CtrlInOutLog_IN2      =0x0002;
const int SMC_CtrlInOutLog_IN3      =0x0004;
const int SMC_CtrlInOutLog_IN4      =0x0008;
const int SMC_CtrlInOutLog_IN5      =0x0010;
const int SMC_CtrlInOutLog_IN6      =0x0020;
const int SMC_CtrlInOutLog_IN7      =0x0040;
const int SMC_CtrlInOutLog_LIM      =0x0080;
const int SMC_CtrlInOutLog_OUT1     =0x0100;
const int SMC_CtrlInOutLog_OUT2     =0x0200;
const int SMC_CtrlInOutLog_OUT3     =0x0400;

//-------------
// SmcWSetOrgLog : Sets the logic of origin input.
//-------------
const int SMC_OrgLog_ORG_Negative   =0x00;
const int SMC_OrgLog_ORG_Positive   =0x01;
const int SMC_OrgLog_Z_RisingEdge   =0x02;

//-------------
// SmcWSetOrgMode : Sets the mode of origin return (origin return motion pattern).
//-------------
    //-------------
    // LimitTurn  : Set whether perform limit turning over or not.
    //-------------
    const int SMC_OrgMode_LimitTurn_Off =0;
    const int SMC_OrgMode_LimitTurn_On  =1;
    //-------------
    // OrgType  : Set whether use Z-phase or not.
    //-------------
    const int SMC_OrgMode_OrgType_UnUseZ=0;
    const int SMC_OrgMode_OrgType_UseZ  =1;
    //-------------
    // EndDir  : Set origin rushing direction (End direction to move to origin) when the origin returned.
    //-------------
    const int SMC_OrgMode_EndDir_UnSpec =0;
    const int SMC_OrgMode_EndDir_PosCW  =1;
    const int SMC_OrgMode_EndDir_NegCCW =2;
    //-------------
    // ZCount  : Set the number of Z-phase when the origin returned. The setting is invalid when "0: Unused" is selected for OrgType.
    //-------------
    const int SMC_OrgMode_ZCount        =0;

//-------------
// SmcWSetPulseType : Sets output mode of pulse.
//-------------
    //-------------
    // DirTimer : If the common pulse method is being set, when the direction changes depending on the change of DIR change,
    //            insert 200[u sec] wait (delay) before the pulse output. (Only used in the case of common pulse method)
    //-------------
    const int SMC_Pulse_DirTimerOn      =1;
    const int SMC_Pulse_DirTimerOff     =0;
    //-------------
    // PulseMode : Set pulse output mode.
    //-------------
    const int SMC_Pulse_OutNeg_DirHigh  =0;
    const int SMC_Pulse_OutPos_DirHigh  =1;
    const int SMC_Pulse_OutNeg_DirLow   =2;
    const int SMC_Pulse_OutPos_DirLow   =3;
    const int SMC_Pulse_2Pulse_Neg      =4;
    const int SMC_Pulse_2Pulse_Pos      =5;
    const int SMC_Pulse_PhaseDiff       =6;
    const int SMC_Pulse_PhaseDiffDelay  =7;

//-------------
// SmcWSetCounterMode : Sets the counter mode.
//-------------
    //-------------
    // ClearCntLtc : Set the type of counter you want to clear when the LTC signal changs from OFF to ON.
    //-------------
    const int SMC_ClearCntLtc_Off       =0;
    const int SMC_ClearCntLtc_OutPulse  =1;
    const int SMC_ClearCntLtc_Encoder   =2;
    const int SMC_ClearCntLtc_Both      =3;
    //-------------
    // LtcMode : Set the type of counter you want to latch when the LTC signal comes in.
    //-------------
    const int SMC_LtcMode_Off           =0;
    const int SMC_LtcMode_OutPulse      =1;
    const int SMC_LtcMode_Encoder       =2;
    const int SMC_LtcMode_Both          =3;
    //-------------
    // ClearCntClr : Set the type of counter you want to clear when the CLR signal changes from OFF to ON.
    //-------------
    const int SMC_ClearCntClr_Off       =0;
    const int SMC_ClearCntClr_OutPulse  =1;
    const int SMC_ClearCntClr_Encoder   =2;
    const int SMC_ClearCntClr_Both      =3;
    //-------------
    // ClrMode : ClrMode (Reserved) 0 fixed.
    //-------------
    const int SMC_ClrMode               =0;

//-------------
// SmcWSetFIFOLatchSrc : Set the source of latch counter and enable the external latch input signals.
//-------------
    //-------------
    // LatchAxisNo  : Specifies the axis number to latch
    // Enable       : Specifies the axis number to enable
    //-------------
    const int SMC_FIFOLtcSrc_Axis1      =0x01;
    const int SMC_FIFOLtcSrc_Axis2      =0x02;
    const int SMC_FIFOLtcSrc_Axis3      =0x04;
    const int SMC_FIFOLtcSrc_Axis4      =0x08;
    const int SMC_FIFOLtcSrc_Axis5      =0x10;
    const int SMC_FIFOLtcSrc_Axis6      =0x20;
    const int SMC_FIFOLtcSrc_Axis7      =0x40;
    const int SMC_FIFOLtcSrc_Axis8      =0x80;

//-------------
// SmcWGetCtrlInOutStatus : Retrieves the status of I/O control signal.
//-------------
    const int SMC_CtrlInOutSts_PCS      =0x01;
    const int SMC_CtrlInOutSts_ERC      =0x02;
    const int SMC_CtrlInOutSts_EZ       =0x04;
    const int SMC_CtrlInOutSts_CLR      =0x08;
    const int SMC_CtrlInOutSts_LTC      =0x10;
    const int SMC_CtrlInOutSts_SD       =0x20;
    const int SMC_CtrlInOutSts_INP      =0x40;
    const int SMC_CtrlInOutSts_DIRCCW   =0x80;

//-------------
// SmcWSetStopPosition : Sets stop position of the motor(total number of output pulses).
//-------------
const int SMC_StopPosition_Absolute     =0;
const int SMC_StopPosition_Relative     =1;

//-------------
// SmcWSetReady : Set beginning preparation and motor operation type/direction where basic operation begins operation.
//-------------
    //-------------
    // MotionType : Set motor motion type.
    //-------------
    const int SMC_MotionType_NoMotion   =0;
    const int SMC_MotionType_PTPMotion  =1;
    const int SMC_MotionType_JogMotion  =2;
    const int SMC_MotionType_OrgMotion  =3;
    const int SMC_MotionType_ZPhaMotion =6;
    const int SMC_MotionType_NoDecMotion=7;     //取代PTP功能
    //-------------
    // StartDir : Set start direction of motor motion. This parameter is invalid when performing PTP motion.
    //-------------
    const int SMC_Ready_StartDir_Pos    =0;
    const int SMC_Ready_StartDir_Neg    =1;

//-------------
// SmcWSetEncType : Specify the address of a variable to store the type of encoder input.
//-------------
    const int SMC_MotionType_AB_1X      =0;
    const int SMC_MotionType_AB_2X      =1;
    const int SMC_MotionType_AB_4X      =2;
    const int SMC_MotionType_UD         =3;
    const int SMC_MotionType_Unused     =4;
//==============================================================================

bool MCSMCCardInstall[MAX_SMC_CARD]={false,false,false,false,false,false,false,false,
                                     false,false,false,false,false,false,false,false};
byte DestructorCount[MAX_SMC_CARD]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

short CardId[MAX_SMC_CARD]={0};

//==============================================================================
__fastcall TMySMCMotor::TMySMCMotor(int Addr)
{
    if(Addr==-1)
    {
        iBoardID=MAX_SMC_CARD-1;
        iPortID =MAX_SMC_CARD-1;
        Id      =MAX_SMC_CARD-1;
    }
    else
    {
        iBoardID=Addr/10;
        iPortID=Addr%10+1;
        sprintf(cDeviceName,"%s%02d","SMC0",iBoardID);
        Open_SMCCard();

        Id=CardId[iBoardID];
    }

    MotorType =Servo_Motor;
    EncoderType=SMC_MotionType_AB_4X;
}
//==============================================================================
TMySMCMotor::~TMySMCMotor()
{
    try
    {
        Close_SMCCard();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~MySMCMotor");
    }
}
//==============================================================================
bool TMySMCMotor::Open_SMCCard()                                                // open card //
{
    #ifndef SOFT_SIMULTE
//    if((iBoardID < 0) || (iBoardID >= MAX_SMC_CARD)) //unsigned int不可能小於0
    if(iBoardID>=MAX_SMC_CARD)   //2012-05-03 Dell  unsigned int    iBoardID
        return false;

    int ret=0;

    if(MCSMCCardInstall[iBoardID]==false)
    {
        ret=SmcWInit(cDeviceName, &CardId[iBoardID]);
        if(ret!=0)
        {
            return false;
        }
        else
        {
            MCSMCCardInstall[iBoardID]=true;
            if(DestructorCount[iBoardID]==0)
                DestructorCount[iBoardID]=1<<1;
            else
                DestructorCount[iBoardID]=DestructorCount[iBoardID]<<1;
            return true;
        }
    }
    #endif
    return false;
}
//==============================================================================
void TMySMCMotor::Close_SMCCard()                                               // close card //
{
    DestructorCount[iBoardID]=DestructorCount[iBoardID]>>1;
    if(DestructorCount[iBoardID]!=0)
        return;
    if(MCSMCCardInstall[iBoardID]==true)
    {
        MCSMCCardInstall[iBoardID]=false;
        SmcWExit(Id);
    }
}
//==============================================================================
//設定軟體極限位置時直接Enable//
void TMySMCMotor::SetSoftLimit(int iPLimit, int iNLimit)                      // set softlimit //
{
    int LP, LN;
    if(Direction)
    {
        LP = -iNLimit;
        LN = -iPLimit;
    }
    else
    {
        LP = iPLimit;
        LN = iNLimit;
    }
    LP=ChangeToFloatNonPcnt((double)(LP), (double)(GearRatio));
    LN=ChangeToFloatNonPcnt((double)(LN), (double)(GearRatio));
    SmcWSetSoftLimit(Id, iPortID, 0x01, 0x01, LP, LN);
}
//==============================================================================
//將軟體極限Disable，如需Enable，直接使用SetSoftLimit//
void TMySMCMotor::SMCSoftLimitEnable(bool bFlag)                                // softlimit enable or disable //
{
    SmcWSetSoftLimit(Id, iPortID, bFlag, bFlag, 0, 0);
}
//==============================================================================
int TMySMCMotor::InitMotor(int IoAddress)
{
    if(!Enable)
        return true;
    SetEncodeMultiple(EncoderType);                                             //將Encode乘4倍，Panasonic伺服//

    if(MotorType==YASKAWA_Liner_Motor)
    {
        SmcWSetCtrlTypeIn(Id, iPortID, SMC_CtrlIn_INP|SMC_CtrlIn_LTC);          // LTC & INP
        SmcWSetCtrlTypeOut(Id, iPortID, SMC_CtrlOut_General, SMC_CtrlOut_EncoderSignal, SMC_CtrlOut_General);
    }
    else
    {
        SmcWSetCtrlTypeIn(Id, iPortID, SMC_CtrlIn_ALM|SMC_CtrlIn_INP|SMC_CtrlIn_LTC);       // ALM & INP & LTC
        SmcWSetCtrlTypeOut(Id, iPortID, SMC_CtrlOut_General, SMC_CtrlOut_General, SMC_CtrlOut_General);
    }

    if(bSensorType)
    {
        SmcWSetOrgLog(Id, iPortID, SMC_OrgLog_ORG_Negative);                    //設定home type//
    }
    else
    {
        SmcWSetOrgLog(Id, iPortID, SMC_OrgLog_ORG_Positive);                    //設定home type//
    }

    SetSMCInType();
    SetCommand(0);
    SetPosition(0);

    if(MotorType==Servo_Motor)                                                  //Pulse mode共有7種模式//
        SmcWSetPulseType(Id, iPortID, SMC_Pulse_2Pulse_Neg,    SMC_Pulse_DirTimerOn);
    else if(MotorType==YASKAWA_Servo_Motor ||
            MotorType==YASKAWA_Liner_Motor ||
            MotorType==Step_Motor_Oriental)                                     //RogerYang 20250904 Add Loader Y
        SmcWSetPulseType(Id, iPortID, SMC_Pulse_2Pulse_Pos,    SMC_Pulse_DirTimerOn);
    else
        SmcWSetPulseType(Id, iPortID, SMC_Pulse_OutPos_DirLow, SMC_Pulse_DirTimerOn);

//    MotOutputOn(3);
    MotOutputOn(1);
    SetServoAlarm();
    SmcWSetInitParam(Id, iPortID);

    return true;
}
//==============================================================================
int TMySMCMotor::ReadRealPos()                                                  // command //
{
    long lGetCommandPos;
    SmcWGetOutPulse(Id, iPortID, &lGetCommandPos);
    if(Direction)
        lGetCommandPos=-lGetCommandPos;
    return(lGetCommandPos);
}
//==============================================================================
int TMySMCMotor::ReadEnCoderRealPos()                                           // EnCoder //
{
    long lGetFeedbackPos;
    if(MotorType==Step_Motor ||
       MotorType==Step_Motor_Oriental)                                          //RogerYang 20250904 Add Loader Y
        SmcWGetOutPulse(Id, iPortID, &lGetFeedbackPos);
    else
        SmcWGetCountPulse(Id, iPortID, &lGetFeedbackPos);

    if(Direction)
        lGetFeedbackPos=-lGetFeedbackPos;
    return(lGetFeedbackPos);
}
//==============================================================================
bool TMySMCMotor::MotionDone()                                                  // motion done //
{
    short MoveSts ;
    bool bStatus=true;

    if(Enable)
    {
        SmcWGetMoveStatus(Id, iPortID, &MoveSts);
        bStatus=(0x00==MoveSts);

        if(bStatus)
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}
//==============================================================================
int TMySMCMotor::SetCommand(int p)                                              // Set Command //
{
    int iRe=0;
    if(Enable)
    {
        iRe=SmcWSetOutPulse(Id, iPortID, p);
    }
    return iRe;
}
//==============================================================================
int TMySMCMotor::SetPosition(int p)                                             // Set encoder //
{
    int iRe=0;
    if(Enable)
    {
        iRe=SmcWSetCountPulse(Id, iPortID, p);
    }
    return iRe;
}
//==============================================================================
void TMySMCMotor::SetSpeed(unsigned int x)                                      // set speed (pps) //
{
    int Ret;

    if(x>ChangeToFloatNonPcnt((double)(6553500), (double)(Range)))                                                         //DL單軸最大可達9.8Mpps//
        x=ChangeToFloatNonPcnt((double)(6553500), (double)(Range));                                                        //DF單軸最大可達6.5Mpps//
    else if(x==0)
        x=100;
    iSpeed=x;

    //-----2012-06-01   Dell Fix 設定速度失敗-----------
    //**Contect motion card 一定要同時設定Resolve/start/target/acc/dec
    //這些參數且順序不可變

    Ret = SmcWSetResolveSpeed(Id, iPortID, Range);
    Ret = SmcWSetStartSpeed  (Id, iPortID, InitSpeed*Range);
    Ret = SmcWSetTargetSpeed (Id, iPortID, iSpeed*Range);
    Ret = SmcWSetAccelTime   (Id, iPortID, dAcc);
    Ret = SmcWSetDecelTime   (Id, iPortID, dDec);
    //--------------------------------------------------
    if(Ret==0)
    {

    }
}
//==============================================================================
void TMySMCMotor::SetSpeedShortDistance(unsigned int x)                         //Steven 20181129 : 短距離的移動把加減速縮短一半
{
    int Ret;

    if(x>ChangeToFloatNonPcnt((double)(6553500), (double)(Range)))                                                         //DL單軸最大可達9.8Mpps//
        x=ChangeToFloatNonPcnt((double)(6553500), (double)(Range));                                                        //DF單軸最大可達6.5Mpps//
    else if(x==0)
        x=100;
    iSpeed=x;

    //-----2012-06-01   Dell Fix 設定速度失敗-----------
    //**Contect motion card 一定要同時設定Resolve/start/target/acc/dec
    //這些參數且順序不可變

    Ret = SmcWSetResolveSpeed(Id, iPortID, Range);
    Ret = SmcWSetStartSpeed  (Id, iPortID, InitSpeed*Range);
    Ret = SmcWSetTargetSpeed (Id, iPortID, iSpeed*Range);
    Ret = SmcWSetAccelTime   (Id, iPortID, dAcc/1.6);
    Ret = SmcWSetDecelTime   (Id, iPortID, dDec/1.6);
    //--------------------------------------------------
    if(Ret==0)
    {

    }
}
//==============================================================================
void TMySMCMotor::SetSpeedShortDisSlowSP(unsigned int x)                        //Steven 20181129 : 短距離的移動把加減速縮短一半
{
    int Ret;

    if(x>ChangeToFloatNonPcnt((double)(6553500), (double)(Range)))                                                         //DL單軸最大可達9.8Mpps//
        x=ChangeToFloatNonPcnt((double)(6553500), (double)(Range));                                                        //DF單軸最大可達6.5Mpps//
    else if(x==0)
        x=100;
    iSpeed=x;

    //-----2012-06-01   Dell Fix 設定速度失敗-----------
    //**Contect motion card 一定要同時設定Resolve/start/target/acc/dec
    //這些參數且順序不可變

    Ret = SmcWSetResolveSpeed(Id, iPortID, Range);
    Ret = SmcWSetStartSpeed  (Id, iPortID, InitSpeed*Range);
    Ret = SmcWSetTargetSpeed (Id, iPortID, iSpeed*Range);
    Ret = SmcWSetAccelTime   (Id, iPortID, dAcc*2.5);
    Ret = SmcWSetDecelTime   (Id, iPortID, dDec*2.5);
    //--------------------------------------------------
    if(Ret==0)
    {

    }
}
//==============================================================================
int TMySMCMotor::ReadPos()                                                      // READ POS //
{
    return ReadRealPos()*GearRatio;
}
//==============================================================================
void TMySMCMotor::ScanMotorStatus(bool *Led)                                    // motor status //
{
    short Status;//, CtrlOut1, CtrlOut2, CtrlOut3 ;

    if(Enable==true)  //Steven 20141016 : Disable時,不要掃LED
    {
        SmcWGetLimitStatus(Id, iPortID, &Status);

        Led[iServoalarmLed] =  0x01 & Status;
        Led[iCwLed]         =  0x02 & Status;
        Led[iCcwLed]        =  0x04 & Status;
        Led[iHomeLed]       =  0x08 & Status;

        SmcWGetDigitalIn(Id, iPortID, &Status);
        Led[iAlarmLed]      =  0x01 & Status;
        Led[iInposLed]      =!(0x02 & Status);
        if(MotorType==Servo_Motor || MotorType==YASKAWA_Servo_Motor || MotorType==YASKAWA_Liner_Motor)
        {
            Led[iServoOn]   =  0x20 & Status;
            Led[iEmgLed]    =!(0x40 & Status);
        }
        else
        {
            Led[iServoOn]   =true;  //步進強制為亮燈
            Led[iEmgLed]    =false; //步進強制為滅燈
        }
    }
    else
    {
        Led[iHomeLed]       =true;
        Led[iInposLed]      =false;
    }
//    SmcWGetCtrlTypeOut(Id, iPortID, &CtrlOut1, &CtrlOut2, &CtrlOut3);
//    Led[iServoOn]       =  0x05 & CtrlOut1;

    if(Led[iCwLed] || Led[iCcwLed])
        Led[iAlarmLed]=true;

    Led[iSoftcwLed] =false;                                                     //無此訊號，只有count
    Led[iSoftccwLed]=false;                                                     //無此訊號，只有count
}
//==============================================================================
bool TMySMCMotor::MoveTo(int Tar)                                            // Move to //
{
    if(MCSMCCardInstall[iBoardID]==false)
        return false;
    if(Enable==false)
        return true;

    if(!MotionDone())
        return false;
    int iPos, iP1, iP2;
    iPos=ReadPos();
    iP1=ChangeToFloatNonPcnt((double)(Tar), (double)(GearRatio));
    iP2=iP1*GearRatio;
    if(iP2<Tar)
    {
        while(1)
        {
            iP1++;
            iP2=iP1*GearRatio;
            if(iP2>=Tar)
                break;
        }
    }
    else if(iP2>Tar)
    {
        while(1)
        {
            iP1--;
            iP2=iP1*GearRatio;
            if(iP2<=Tar)
                break;
        }
    }
    if(Direction)
        iP1=-iP1;
    else
        iP1=iP1;

    SetSpeed(iSpeed);
    SmcWSetStopPosition(Id, iPortID, 0x00, iP1);
    SmcWSetReady(Id, iPortID, 0x01, 0x00);

    SmcWMotionStart(Id, iPortID);
    if(MotionDone() && Tar==iPos)
        return true;
    else
        return false;
}
//==============================================================================
bool TMySMCMotor::JogP()                                                        // JOG P //
{
    if( Enable==false)
        return false;
    if(Direction)
    {
        SmcWSetReadyEx(Id, iPortID, SMC_MotionType_JogMotion, SMC_Ready_StartDir_Neg);
    }
    else
    {
        SmcWSetReadyEx(Id, iPortID, SMC_MotionType_JogMotion, SMC_Ready_StartDir_Pos);
    }
    SmcWMotionStart(Id, iPortID);
    return true;

}
//==============================================================================
bool TMySMCMotor::JogN()                                                        // JOG N //
{
    if(Enable==false)
        return false;

    if(Direction)
    {
        SmcWSetReadyEx(Id, iPortID, SMC_MotionType_JogMotion, SMC_Ready_StartDir_Pos);
    }
    else
    {
        SmcWSetReadyEx(Id, iPortID, SMC_MotionType_JogMotion, SMC_Ready_StartDir_Neg);
    }
    SmcWMotionStart(Id, iPortID);
    return true;
}
//==============================================================================
void TMySMCMotor::Stop()                                                     // STOP //
{
    SmcWMotionStop(Id, iPortID);
}
//==============================================================================
void TMySMCMotor::DecStop()                                                  // STOP //
{
    SmcWMotionDecStop(Id, iPortID);
}
//==============================================================================
bool TMySMCMotor::HomeObject()
{
    return SMCMotHome();
}
//==============================================================================
bool TMySMCMotor::SMCMotHome()
{
    int &Task=iHomeObjectTask;
    switch(Task)
    {
        case 1:
            if(!MotionDone())
            {
                Stop();
                break;
            }
            SetCommand(0);
            SetPosition(0);
            OldSpeed=iSpeed;                                                    //RogerYang 20250508 改在SetSpeed上面
            SetSpeed(PHomeHighSpeed);
            SMCSoftLimitEnable(false);
            Stop();
            if(HomeDirection)
                SmcWSetOrgMode(Id, iPortID, SMC_OrgMode_LimitTurn_On, SMC_OrgMode_OrgType_UnUseZ, SMC_OrgMode_EndDir_NegCCW, SMC_OrgMode_ZCount);
            else
                SmcWSetOrgMode(Id, iPortID, SMC_OrgMode_LimitTurn_On, SMC_OrgMode_OrgType_UnUseZ, SMC_OrgMode_EndDir_PosCW , SMC_OrgMode_ZCount);

            InitMotor(Address);

            SmcWSetReadyEx(Id, iPortID, SMC_MotionType_OrgMotion, HomeDirection);
            Task=10;
            break;
        case 10:
            if(0==SmcWMotionStart(Id, iPortID))
                Task=20;
            break;
        case 20:
            if(MotionDone())
                Task=30;
            break;
        case 30:
            LastHomePos=-ReadPos();

            SetCommand(0);

            MySleep(100);

            SetPosition(0);
            SetSpeed(OldSpeed);
            SetSoftLimit(PSoftLimitP, PSoftLimitN);

            Task=1;
            return true;
        default:
            Task=1;
            return false;
    }
    return false;
}
//==============================================================================
void TMySMCMotor::SetInitSpeed(unsigned int x)                                  //set start speed//
{
    InitSpeed = x;
    //2012-06-01    Dell 參數寫入時機必需與(Basic Motion Setting)同步
//    Ret =SmcWSetStartSpeed(Id, iPortID, InitSpeed * Range);
}
//==============================================================================
void TMySMCMotor::SetServoAlarmOn(bool Value)
{
    PServoAlarmOn = Value;
}
//==============================================================================
void TMySMCMotor::SetServoAlarm()
{
    if(PServoAlarmOn==false)
    {
        SmcWSetCtrlTypeIn(Id, iPortID, SMC_CtrlIn_INP|SMC_CtrlIn_LTC);
        SetSMCInType();
    }
}
//==============================================================================
void TMySMCMotor::SetAcc(double a)
{
    dAcc = a;
    //2012-06-01    Dell 參數寫入時機必需與(Basic Motion Setting)同步
//    SmcWSetAccelTime(Id,iPortID,dAcc);
}
//==============================================================================

void TMySMCMotor::SetDec(double a)
{
    dDec = a;
    //2012-06-01    Dell 參數寫入時機必需與(Basic Motion Setting)同步
//    SmcWSetDecelTime(Id,iPortID,dDec);
}
//==============================================================================
void TMySMCMotor::SetSMCInType()
{
    int iLimitLogic, iIn1ALMLogic, iIn2INPLogic;
    if(bLimitLogic)
        iLimitLogic=SMC_CtrlInOutLog_LIM;
    else
        iLimitLogic=0x00;

    if(bIn1Logic)
        iIn1ALMLogic=SMC_CtrlInOutLog_IN1;
    else
        iIn1ALMLogic=0x00;

    if(MotorType==Servo_Motor || MotorType==YASKAWA_Servo_Motor || MotorType==YASKAWA_Liner_Motor)
        iIn2INPLogic=0x00;
    else
        iIn2INPLogic=SMC_CtrlInOutLog_IN2;

    SmcWSetCtrlInOutLog(Id, iPortID, iLimitLogic|iIn1ALMLogic|iIn2INPLogic);    //設定極限type//
}
//==============================================================================
void TMySMCMotor::SetRange(unsigned int a)
{
    if(a>1000)
        a=1000;
    Range = a;
//        2012-06-01    Dell 參數寫入時機必需與(Basic Motion Setting)同步
//    SmcWSetResolveSpeed(Id,iPortID,Range);
}
//==============================================================================
bool TMySMCMotor::GetAlarm(void)
{
    short Status ;
    SmcWGetLimitStatus(Id, iPortID, &Status);
    if(0x01 & Status)
        return true;
    else
        return false;
}
//==============================================================================
/*
設定Encode倍率
iMultiple設0等1倍
iMultiple設1等2倍
iMultiple設2等4倍
*/
void TMySMCMotor::SetEncodeMultiple(int iMultiple)
{
    SmcWSetEncType(Id, iPortID, iMultiple);
}
//==============================================================================
bool TMySMCMotor::HomeFlag(void)
{
    short Status ;
    SmcWGetLimitStatus(Id, iPortID, &Status);
    if(Status & 0x08)
        return(true);
    else
        return(false);
}
//==============================================================================
/*
bInputPort是7個陣列bool
*/
void TMySMCMotor::MotInputStatus(bool *bInputPort)                           //讀DI訊號//
{
    short InData ;
    SmcWGetDigitalIn(Id,iPortID, &InData);
    bInputPort[0]=InData & 0x01;
    bInputPort[1]=InData & 0x02;
    bInputPort[2]=InData & 0x04;
    bInputPort[3]=InData & 0x08;
    bInputPort[4]=InData & 0x10;
    bInputPort[5]=InData & 0x20;
    bInputPort[6]=InData & 0x40;
}
//==============================================================================
/*
iOutPort請代入1~3，軸卡上的編號為Out1~Out3
*/
void TMySMCMotor::MotOutputOn(int iOutPort)                                     //將DO on//
{
    short OutData=0, OutDataEnable=0;
    OutData = 1<<(iOutPort-1);
    OutDataEnable = (1<<iOutPort-1);
    SmcWSetDigitalOut(Id, iPortID, OutData, OutDataEnable);
}
//==============================================================================
void TMySMCMotor::MotOutputOff(int iOutPort)                                    //將DO off//
{
    short OutData=0, OutDataEnable=0;
    OutData = 0<<(iOutPort-1);
    OutDataEnable = 1<<(iOutPort-1);
    SmcWSetDigitalOut(Id, iPortID, OutData, OutDataEnable);
}
//==============================================================================
bool TMySMCMotor::LinearAxisMoveTo(int iPortID[8],long lPos[8],bool bFlag)
{
    if(bFlag==false)
    {
        short InterAxis;
        short InterType=1; // line interpolation
        short BankNum=1;

        short AxisNo=1;
        short MotionType=4;
        InterAxis = 0x03;

        SmcWSetBankNumber       (Id, AxisNo,    BankNum);
        SmcWSetBankInterpolation(Id, AxisNo,    BankNum, InterType, InterAxis, Range);
        SmcWSetBankDistance     (Id, AxisNo,    1,       lPos[0]);
        SmcWSetBankDistance     (Id, AxisNo+1,  1,       lPos[1]);
        SmcWSetBankStartSpeed   (Id, AxisNo,    1,       iSpeed*Range);
        SmcWSetBankTargetSpeed  (Id, AxisNo,    1,       iSpeed*Range);
        SmcWSetBankAccelTime    (Id, AxisNo,    1,       dAcc);
        SmcWSetBankDecelTime    (Id, AxisNo,    1,       dDec);
        SmcWSetBankSSpeed       (Id, AxisNo,    1,       200);
        SmcWSetBankReady        (Id, AxisNo,    MotionType);
        SmcWMotionStart         (Id, AxisNo);
    }
    else
    {
        if(MotionDone())
            return true;
    }
    return false;
}
//==============================================================================
/*
1、mode=0 pulse command
2、mode=1 encode
3、需設定output defind，設定那一個Out port輸出，預設用Out1
   SmcWSetCtrlTypeOut( Id, AxisNo, CtrlOut1, CtrlOut2, CtrlOut3)
4、需設定Windows handle
*/
void TMySMCMotor::EnableTrigger(bool bFlag,int iMode,long lValue)
{
    HWND hWnd=FindWindow(NULL, "MotorTest");
    if(bFlag)
    {

        long lPitch=ChangeToFloatNonPcnt((double)(lValue), (double)(GearRatio));
        if(Direction)
            lPitch=-lPitch;
        else
            lPitch=lPitch;
        if(iMode)
        {
            SmcWSetCtrlTypeOut  (Id, iPortID, 0x03, 0x00, 0x00);
            SmcWCountEvent      (Id, iPortID, hWnd, 0x01, 0x00, lPitch);
        }
        else
        {
            SmcWSetCtrlTypeOut  (Id, iPortID, 0x04, 0x00, 0x00);
            SmcWCountEvent      (Id, iPortID, hWnd, 0x01, 0x01, lPitch);
        }
        SmcWSetInitParam        (Id, iPortID);
    }
    else
    {
        SmcWSetCtrlTypeOut  (Id, iPortID, 0x03, 0x00, 0x00);
        SmcWStopEvent       (Id, iPortID, hWnd, 1);
        SmcWSetInitParam    (Id, iPortID);
    }
}
//==============================================================================
//2012-05-28    Add
bool TMySMCMotor::ResetPos(int Pulse=0)
{
    long iR1 ,iR2;

    iR1 =SetCommand(Pulse);
    iR2 =SetPosition(Pulse);

//    itest1 =iR1;
//    itest2 =iR2;

    if ((iR1 == 0) && (iR2 ==0))
        return true;
    return false;
//    SetSpeed(iSpeed);
//    SmcWSetStopPosition(Id,iPortID,0x00,Pulse);
//    SmcWSetReady(Id,iPortID,0x01,0x00);
//    SmcWSetInitParam( Id ,iPortID);
}
//==============================================================================
//bool TMySMCMotor::ResetPos()
//{
//    long lGetCommandPos;
//    int iRe=0;
//
//    SmcWGetOutPulse(Id, iPortID, &lGetCommandPos);
//
//    if(Enable)
//    {
//        iRe=SmcWSetCountPulse(Id, iPortID, lGetCommandPos);
//    }
//    return iRe;
//}
//==============================================================================
void TMySMCMotor::ResetLatch()
{
    SmcWResetLatchFIFO(Id);

    for(int i=1; i<=8; i++) //Steven 20140314 : for HT9045WA因為重新排列軸,會導致第8軸無Init
    {
        SmcWSetCtrlTypeIn(Id,  i, SMC_CtrlIn_ALM|SMC_CtrlIn_INP|SMC_CtrlIn_LTC);       // ALM & INP & LTC
        SmcWSetCtrlTypeOut(Id, i, SMC_CtrlOut_General, SMC_CtrlOut_General, SMC_CtrlOut_General);
        SmcWSetInitParam(Id,   i);
    }
}
//==============================================================================
int TMySMCMotor::GetLatchTotalLen()
{
    short Length;
    long dwRet;
    dwRet = SmcWGetLatchFIFOLength(Id, iPortID, &Length);
    if(dwRet)
    {
        SmcWGetErrorString(dwRet, ErrorString);
        return 0;
    }

    return Length;
}
//==============================================================================
int TMySMCMotor::GetLatchBuffer(int LatchInputNo, short *AxisCounterNo, long *LatchDataTable)
{
    long  dwRet;
    short LatchDataCnt ,UpCnt ,DownCnt;

//    dwRet=SmcWGetLatchDataFromBuffer(Id, LatchInputNo, AxisCounterNo, &LatchDataCnt, &LatchDataTable[0]);
    dwRet=SmcWGetLatchDataFromBufferEx(Id, LatchInputNo, AxisCounterNo, &LatchDataCnt, &LatchDataTable[0] ,&UpCnt ,&DownCnt);
    if(dwRet)
    {
        if(dwRet==20262)
        {
            SmcWGetErrorString(dwRet, ErrorString);
            LatchDataCnt=0;
        }
    }
    PulseEdgeCnt[LatchInputNo-1][0] = UpCnt;
    PulseEdgeCnt[LatchInputNo-1][1] = DownCnt;
    return LatchDataCnt;
}
//--------------------------------------------------------------------------
bool TMySMCMotor::GetLatchIOStatus(unsigned short LatchInputNo)
{
    short CtrlSts=0;
    SmcWGetDigitalIn(Id, (LatchInputNo +1), &CtrlSts);

    return (CtrlSts & SMC_CtrlIn_LTC);
}
//--------------------------------------------------------------------------
void TMySMCMotor::SetFIFOLatchSrc(int AxisCounterNo, int LatchInputNo, int bEnable)  //ChungHung 20130625 add 跟原本Motor Enable衝突   Enable-->bEnable
{
    SmcWSetFIFOLatchSrc(Id, AxisCounterNo, LatchInputNo, bEnable);
}
//--------------------------------------------------------------------------
void TMySMCMotor::SetServoOn(bool IsOn)
{
    if(Enable==false)
        return;

    if(IsOn)
    {
        MotOutputOn(1);
    }
    else
    {
        MotOutputOff(1);
    }
}
//--------------------------------------------------------------------------
bool TMySMCMotor::MoveToPos(int Tar)                                            // Move to //
{
    if(MCSMCCardInstall[iBoardID]==false)
        return false;

    if(Enable==false)
        return true;

    if(!MotionDone())
        return false;

    int iPos;
    iPos=ReadPos();

    SetSpeed(iSpeed);
    SmcWSetStopPosition(Id, iPortID, SMC_StopPosition_Absolute, Tar);
    SmcWSetReadyEx     (Id, iPortID, SMC_MotionType_NoDecMotion , SMC_Ready_StartDir_Pos);

    SmcWMotionStart(Id, iPortID);
    if(MotionDone() && Tar==iPos)
        return true;
    else
        return false;
}
//--------------------------------------------------------------------------
//Steven 20181129 : 短距離的移動把加減速縮短一半
//--------------------------------------------------------------------------
bool TMySMCMotor::MoveToPosShortDistance(int Tar)                               // Move to //
{
    if(MCSMCCardInstall[iBoardID]==false)
        return false;

    if(Enable==false)
        return true;

    if(!MotionDone())
        return false;

    int iPos;
    iPos=ReadPos();

    SetSpeedShortDistance(iSpeed);
    SmcWSetStopPosition(Id, iPortID, SMC_StopPosition_Absolute, Tar);
    SmcWSetReadyEx     (Id, iPortID, SMC_MotionType_NoDecMotion , SMC_Ready_StartDir_Pos);

    SmcWMotionStart(Id, iPortID);
    if(MotionDone() && Tar==iPos)
        return true;
    else
        return false;
}
//--------------------------------------------------------------------------
bool TMySMCMotor::MoveToPosShortDisSlowSP(int Tar)                              // Move to //
{
    if(MCSMCCardInstall[iBoardID]==false)
        return false;

    if(Enable==false)
        return true;

    if(!MotionDone())
        return false;

    int iPos;
    iPos=ReadPos();

    SetSpeedShortDisSlowSP(iSpeed);
    SmcWSetStopPosition(Id, iPortID, SMC_StopPosition_Absolute, Tar);
    SmcWSetReadyEx     (Id, iPortID, SMC_MotionType_NoDecMotion , SMC_Ready_StartDir_Pos);

    SmcWMotionStart(Id, iPortID);
    if(MotionDone() && Tar==iPos)
        return true;
    else
        return false;
}
//--------------------------------------------------------------------------
