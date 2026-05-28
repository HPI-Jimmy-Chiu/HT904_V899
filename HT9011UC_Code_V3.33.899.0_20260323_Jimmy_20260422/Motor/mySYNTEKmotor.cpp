#include "MachineDefine.h"
#pragma hdrstop

#include "mySYNTEKmotor.h"

//#include "PCI_L112.h"
//#include "PCI_L122.h"
//#include "Hontech_M4.h"
//#include "CMNet.h"
//#include "MNETError.h"

#include "cMyDef.h"
#include "note.h"

#define MAX_LATCH_COUNT 1024
unsigned int _MyLongMask[32]=
    {0x00000001,0x00000002,0x00000004,0x00000008,0x00000010,0x00000020,0x00000040,0x00000080,
     0x00000100,0x00000200,0x00000400,0x00000800,0x00001000,0x00002000,0x00004000,0x00008000,
     0x00010000,0x00020000,0x00040000,0x00080000,0x00100000,0x00200000,0x00400000,0x00800000,
     0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000};

//------------------------------------------------------------------------------
//RingNo    ==> Address
//SlaveIP   ==> iBoardID
//AxisNo    ==> iPortID
//------------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall TMySYNTEKMotor::TMySYNTEKMotor(int Addr)
{
    if(Addr==-1)
    {
        Address  =-1;
        iBoardID =-1;
        iPortID  =-1;
    }
    else
    {
        Address  =Addr/1000;
        iBoardID =(Addr%1000)/10;
        iPortID  =(Addr%10);

        SYNTEKOpenCard();
    }
}
//==============================================================================
TMySYNTEKMotor::~TMySYNTEKMotor()
{

}
//==============================================================================
//設定軟體極限位置時直接Enable//
void TMySYNTEKMotor::SetSoftLimit(int iPLimit, int iNLimit)                      // set softlimit //
{
    int LP,LN;
    if(Enable==false)
        return;
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
    _Hon_m4_set_soft_limit(Address, iBoardID, iPortID, LP, LN);
}
//==============================================================================
//將軟體極限Disable，如需Enable，直接使用SetSYNTEKSoftLimit//
void TMySYNTEKMotor::SYNTEKSoftLimitEnable(bool bFlag)                                // softlimit enable or disable //
{
    if(Enable==false)
        return;
    _Hon_m4_enable_soft_limit(Address, iBoardID, iPortID, (int)bFlag);
}
//==============================================================================
//2013-03-27    Dell  ＊重要_
//起因：motion card送出去的pulse count與command count對不起來 ,造成機台誤動作
//原因：Hon_m4_set_feedback_src不可以跟_Hon_m4_load_motion_file放在一起
//，因為先達在load file(HT9045_M204_2.cfg)的時候會去設定feedback_src，所以放在一起
//就會因為load file而導致重設 feedback_src，造成設定跑掉．
int TMySYNTEKMotor::InitMotor(int IoAddress)
{
    U8 u8MNetType;

    _mnet_get_slave_type(Address, iBoardID, &u8MNetType);

    if(PServoAlarmOn==0)//step motor
    {
        _Hon_m4_set_feedback_src(Address, iBoardID, iPortID, 1);//使用command
    }
    else
    {
        if(u8MNetType==G9004_M204)
        {
            _Hon_m4_set_feedback_src(Address, iBoardID, iPortID, 3);//使用encoder
        }
        else
        {
            _Hon_m4_set_feedback_src(Address, iBoardID, iPortID, 2);//使用encoder
        }
    }

    return true;
}
//==============================================================================
bool TMySYNTEKMotor::SYNTEKOpenCard()
{
    U8 u8MNetType;
    AnsiString str;
    AnsiString asStr="", temp="";

    _mnet_get_slave_type(Address, iBoardID, &u8MNetType);
    if(u8MNetType==G9004_M204)
    {
        if(Address==0 && iBoardID==3)                                           //jou 2014-10-09 修正 Device Superfluous at Output shuttle error
        {
            SYN_TEK_MOTION_MODULE=G9004_M204;                                   //Steven 20110304
        }

        if(_Hon_m4_initial(Address, iBoardID)!=ERR_NoError)
        {
//            str.sprintf("Lan NO:%d IP:%d", Address, iBoardID);
//            ShowErrorMessage("WAR1692", 0, MMSystem, 0, asStr);  //Open SYN-TEK DIO Module Fail!!!
            return false;
        }

        //HT9045_M204_2.cfg CMNet.dll 2011/04/02 use
        //HT9045_M204.cfg   CMNet.dll 2011/03/xx use
        temp="D:\\HT9045\\CFG\\HT9045_M204_2.cfg";
        if(_Hon_m4_load_motion_file(Address, iBoardID, temp.c_str())!=ERR_NoError)
        {
//            str.sprintf("Lan NO:%d IP:%d", Lan, IP);
//            ShowErrorMessage("WAR1692", 0, MMSystem, 0, asStr);  //Load SYN-TEK Motion Module Config Fail!!!
            return false;
        }
    }
    else if(u8MNetType==G9004_M104)
    {
        if(Address==0 && iBoardID==3)                                           //jou 2014-10-09 修正 Device Superfluous at Output shuttle error
        {
            SYN_TEK_MOTION_MODULE=G9004_M104;                                   //Steven 20110304
        }

        if(_Hon_m4_initial(Address, iBoardID)!=ERR_NoError)
        {
//            str.sprintf("Lan NO:%d IP:%d", Address, iBoardID);
//            ShowErrorMessage("WAR1692", 0, MMSystem, 0, asStr);  //Open SYN-TEK DIO Module Fail!!!
            return false;
        }

        if(MOTOR_DRIVER_TYPE==Panasonic_DRIVER) //Panasonic     //Steven 20110321
        {
            temp="D:\\HT9045\\CFG\\HT9045.cfg";
        }
        else
        {
            temp="D:\\HT9045\\CFG\\HT9045_Mitsubishi.cfg";
        }

        if(_Hon_m4_load_motion_file(Address, iBoardID, temp.c_str())!=ERR_NoError)
        {
//            str.sprintf("Lan NO:%d IP:%d", Address, iBoardID);
//            ShowErrorMessage("WAR1692", 0, MMSystem, 0, asStr);  //Load SYN-TEK Motion Module Config Fail!!!
            return false;
        }
    }
    else
    {
        if(Address==0 && iBoardID==3)                                           //jou 2014-10-09 修正 Device Superfluous at Output shuttle error
        {
            SYN_TEK_MOTION_MODULE=G9004_M104;                                   //Steven 20110304
        }

        if((Address==0 && iBoardID<4) || (Address==1 && iBoardID<3))
        {
//            str.sprintf("Disappear NO:%d IP:%d Error", Lan, IP);
//            ShowErrorMessage("WAR1691", 0, MMSystem, 0, str);  //'No SYN-TEK DIO Module!!!
            return false;
        }
    }
    return true;
}
//==============================================================================
int TMySYNTEKMotor::ReadRealPos()                                               // command //
{
    long lPos;
    if(Enable)
    {
        _Hon_m4_get_command(Address, iBoardID, iPortID, &lPos);
        if(Direction)
            return lPos;
        else
            return -lPos;
    }
    else
    {
        return 0;
    }
}
//==============================================================================
int TMySYNTEKMotor::ReadEnCoderRealPos()                                        // EnCoder //
{
    long lPos;
    if(Enable)
    {
        _Hon_m4_get_position(Address, iBoardID, iPortID, &lPos);
        if(Direction)
            return lPos;
        else
            return -lPos;
    }
    else
    {
        return 0;
    }
}
//==============================================================================
bool TMySYNTEKMotor::MotionDone()                                               // motion done //
{
    unsigned short iStatus=0;
    if(Enable==false)
    {
        return true;
    }
    else
    {
        _Hon_m4_motion_done(Address, iBoardID, iPortID, &iStatus);
        if(iStatus!=0)
            return false;
    }
    return true;
}
//==============================================================================
int TMySYNTEKMotor::SetCommand(int p)                                           // Set Command //
{
    int iRe =0;
    if(Enable)
    {
        iRe =_Hon_m4_set_command(Address, iBoardID, iPortID, p);
    }
    return iRe;
}
//==============================================================================
int TMySYNTEKMotor::SetPosition(int p)                                          // Set encoder //
{
    int iRe =0;
    if(Enable)
    {
        iRe =_Hon_m4_set_position(Address, iBoardID, iPortID, p);
    }
    return iRe;
}
//==============================================================================
void TMySYNTEKMotor::SetSpeed(unsigned int x)                                   // set speed (pps) //
{
    unsigned iInitS=0;

    if(Address==-1 || iBoardID==-1 || iPortID==-1)
    {
        return;
    }

//    if(x>100)
//        x=100;
//    if(x<0.01)
//        x=0.01;
    double dScaleSpeed = x;
    dScaleSpeed = ChangeToFloatNonPcnt((double)(dScaleSpeed), (double)(PJogHighSpeed));

    unsigned int uiSpeed = x*65;
    iSpeed=uiSpeed;

    iInitS=InitSpeed * dScaleSpeed *65;
    if(uiSpeed>1000000)
        uiSpeed=1000000;
    else if(uiSpeed<=0)
        uiSpeed=1;
    if(iInitS>uiSpeed)
        iInitS=uiSpeed;
    else if(iInitS<=0)
        iInitS=1;

    _Hon_m4_set_tmove_speed(Address, iBoardID, iPortID, iInitS, uiSpeed, dAcc, dDec);
}
//==============================================================================
int TMySYNTEKMotor::ReadPos()                                                   // READ POS //
{
    return ReadRealPos()*GearRatio;
}
//==============================================================================
void TMySYNTEKMotor::ScanMotorStatus(bool *Led)                                 // motor status //
{
    unsigned long mode;
    if(Enable==false)
        return;

    _Hon_m4_get_io_status(Address, iBoardID, iPortID, &mode);
    Led[iCwLed]         =mode&_MyLongMask[2];               //CW
    Led[iHomeLed]       =mode&_MyLongMask[4];               //HOME
    Led[iCcwLed]        =mode&_MyLongMask[3];               //CCW
    Led[iEmgLed]        =mode&_MyLongMask[6];               //EMG.S
    //Led[iSoftcwLed]=SFLimit&MyBitMask[0];             //Soft Cw
    //Led[iSoftccwLed]=SFLimit&MyBitMask[1];            //SoftCCW
    Led[iSoftcwLed]     =false;
    Led[iSoftccwLed]    =false;
    Led[iServoalarmLed] =mode&_MyLongMask[1];           //Servo alarm
    if(Enable==false)
        Led[iInposLed]  =false;
    else
        Led[iInposLed]  =mode&_MyLongMask[13];          //InPos
    Led[iServoOn]=mode&_MyLongMask[14];                 //Servo On

//jou 20180103 (Steven) : 這樣馬達不會跳掉不會alarm
//    Led[iAlarmLed]=GetAlarm();                          //Alarm
    if(Led[iCwLed]      || Led[iCcwLed]     ||
       Led[iSoftcwLed]  || Led[iSoftccwLed] ||
       Led[iServoalarmLed])
        Led[iAlarmLed]=true;
    else
        Led[iAlarmLed]=false;
}
//==============================================================================
bool TMySYNTEKMotor::MoveToPos(int Tar)                                         // Move to //
{
    AnsiString S1;

    if(Enable==false)
        return true;

    if(!MotionDone())
        return false;

    if(Direction == false)
        Tar=-Tar;

    int ret;
//    int iPos;
//    iPos=ReadPos();
    ReadPos();

    ret=_Hon_m4_start_a_move(Address, iBoardID, iPortID, Tar);

    if(ret!=ERR_NoError)
    {
        if(ret==ERR_G94_RECEIVE_TimeOut)
        {
            return false;
        }
        else
        {
            S1.sprintf("ret=%d", ret);
            ShowErrorMessage("WAR1638", 0, MMSystem, 0, S1);                    //Motor control error, check control box
        }
    }

    if(MotionDone())
        return true;
    else
        return false;
}
//==============================================================================
bool TMySYNTEKMotor::JogP()                                                     // JOG P //
{
    if(Enable==false)
        return false;

    _Hon_m4_v_move(Address, iBoardID, iPortID, (Direction)?1:0);                //一直跑
    return true;
}
//==============================================================================
bool TMySYNTEKMotor::JogN()                                                     // JOG N //
{
    if(Enable==false)
        return false;

    _Hon_m4_v_move(Address, iBoardID, iPortID, (Direction)?0:1);                //一直跑
    return true;
}
//==============================================================================
void TMySYNTEKMotor::Stop()                                                     // STOP //
{
    if(Enable==false)
        return;
    _Hon_m4_emg_stop(Address, iBoardID, iPortID);                               //Steven 20131125 : 用 _Hon_m4 才對
//    _mnet_m204_emg_stop(Address, iBoardID, iPortID);
}
//==============================================================================
void TMySYNTEKMotor::DecStop()                                                  // STOP //
{
    if(Enable==false)
        return;
    _Hon_m4_sd_stop(Address, iBoardID, iPortID);
}
//==============================================================================
bool TMySYNTEKMotor::HomeObject()
{
    bool ret=SYNTEKMotHome();

    if(ret==true && Address==0 && iBoardID==3)
    {
        if(iPortID==0 || iPortID==1)
            _Hon_m4_set_axis_counter(Address, iBoardID, iPortID, 0, 1-Direction, 0);
    }

    return ret;
}
//------------------------------------------------------------------------------
//PCI 132 function start
//HomeDir==0 motor CCW direct to home
//HomeDir==1 motor CW direct to home
//Motor->Direction==1 motor action is correct(ok)
//Motor->Direction==0 motor action direction need reverse
//==============================================================================
bool TMySYNTEKMotor::SYNTEKMotHome()
{
    if(Enable==false)
        return true;
    int &Task=iHomeObjectTask;
    int HomeInitSpeed=10;
    static int iHomeMode=0;
//    static int Cmdpos=0;
    bool    Led[10];
//    int     iMovePitch=200;
    int     iStatus=ERR_NoError;

    switch(Task)
    {
        case 1:
//            Cmdpos=0;
            LastHomePos=0;
            SetPosition(0);
            SetCommand(0);
            HomeDelay.SetSecAndOn(0.1);
            Task=25;
            break;
        case 25:
            if(HomeDelay.Off()==false)
                return false;
            SetServoOn(true);
            Task=30;
            break;
        case 30:
            ScanMotorStatus(Led);
            if(Led[iServoOn]==true)//servo on
            {
                Task=50;
            }
            else
            {
                Task=1;
                Stop();
                return true;
            }
            break;
        case 50:
            iHomeMode=0;
            if(_Hon_m4_set_home_config(Address, iBoardID, iPortID, iHomeMode, bSensorType, 1, 0, 0)==ERR_NoError)
            {
                ScanMotorStatus(Led);
                if(Led[iHomeLed])//in home pos
                {
                    Stop();
                    SetSpeed(1);
                    Task=100;
                }
                else
                {
                    Task=75;
                }
            }
            break;
        case 75:
            _Hon_m4_set_tmove_speed(Address, iBoardID, iPortID, HomeInitSpeed, PHomeHighSpeed, 0.1, 0.1);
            if(HomeDirection)
            {
                if(Direction)
                    JogP();
                else
                    JogN();
            }
            else
            {
                if(Direction)
                    JogN();
                else
                    JogP();
            }
            Task=80;
            break;
        case 80:
            ScanMotorStatus(Led);
            if(Led[iHomeLed])
            {
                Stop();
                HomeDelay.Set0_1SecAndOn(1);
                Task=90;
            }
            break;
        case 90:
            if(HomeDelay.Off())
                Task=50;
            break;
        case 100:
            _Hon_m4_set_tmove_speed(Address, iBoardID, iPortID, HomeInitSpeed, PHomeHighSpeed, 0.1, 0.1);
            if(HomeDirection)
            {
                _Hon_m4_start_r_move(Address, iBoardID, iPortID, -iHomePitch);
//                Cmdpos-=iMovePitch;
            }
            else
            {
                _Hon_m4_start_r_move(Address, iBoardID, iPortID, iHomePitch);
//                Cmdpos+=iMovePitch;
            }
            Task=200;
            break;
        case 200:
            ScanMotorStatus(Led);
            if(MotionDone()==false)
                return false;
            if(PServoAlarmOn)//servo motor
            {
                if(Led[iInposLed])
                    return false;
            }
            if(Led[iHomeLed]==false)
            {
                Stop();
                HomeDelay.Set0_1SecAndOn(10);
                Task=300;
            }
            else
            {
                Task=100;
            }
            break;
        case 300:
            if(HomeDelay.Off())
            {
                _Hon_m4_set_tmove_speed(Address, iBoardID, iPortID, HomeInitSpeed, PHomeLowSpeed, 0.01, 0.01);
                iStatus=_Hon_m4_start_home_move(Address, iBoardID, iPortID, (HomeDirection)?1:0);

                if(iStatus==ERR_NoError)
                {
                    Task=400;
                }
                else
                {
                    Task=1;
                }
            }
            break;
        case 400:
            ScanMotorStatus(Led);
            if((HomeDirection==0 && Led[iCcwLed]) ||
               (HomeDirection==1 && Led[iCwLed]))//in ccw
            {
                Stop();
                Task=500;
            }
            else if(MotionDone()==true)
            {
                HomeDelay.SetSecAndOn(0.3);
                Task=450;
            }
            break;
        case 450:
            if(HomeDelay.Off())
            {
                LastHomePos=-ReadPos();
                SetPosition(0);
                SetCommand(0);
                Task=1;
                ScanMotorStatus(Led);
                if(Led[iHomeLed]==false)
                {
                    return false;
                }
                return true;
            }
            break;
        case 500:
            _Hon_m4_set_tmove_speed(Address, iBoardID, iPortID, 100, 1000, 0.1, 0.1);
            if(HomeDirection)
                _Hon_m4_start_r_move(Address, iBoardID, iPortID, -4000);
            else
                _Hon_m4_start_r_move(Address, iBoardID, iPortID, 4000);
            Task=600;
            break;
        case 600:
            if(MotionDone())
            {
                Task=50;
            }
            break;
    }
    return false;
}
//==============================================================================
void TMySYNTEKMotor::SetInitSpeed(unsigned int x)                               //set start speed//
{
    InitSpeed=x;
}
//==============================================================================
void TMySYNTEKMotor::SetServoAlarmOn(bool Value)
{
    PServoAlarmOn=Value;
}
//==============================================================================
void TMySYNTEKMotor::SetRange(unsigned int a)
{
    if(a>1000)
        a=1000;
    Range=a;
}
//==============================================================================
bool TMySYNTEKMotor::GetAlarm(void)
{
//jou 20180103 (Steven) : 這樣馬達不會跳掉不會alarm
//    bool Led[10]={false, false, false, false, false, false, false, false, false, false};
//    if(Led[iCwLed]      || Led[iCcwLed]     ||
//       Led[iSoftcwLed]  || Led[iSoftccwLed] ||
//       Led[iServoalarmLed])
//        return true;
    return false;
}
//==============================================================================
bool TMySYNTEKMotor::HomeFlag(void)
{
    unsigned long mode;

    if(Enable==false)
        return true;

    _Hon_m4_get_io_status(Address, iBoardID, iPortID, &mode);

    if(mode&_MyLongMask[4])
        return true;
    else
        return false;
}
//==============================================================================
void TMySYNTEKMotor::MotInputStatus(bool *bInputPort)                           //讀DI訊號//
{
    ;
}
//==============================================================================
void TMySYNTEKMotor::MotOutputOn(int iOutPort)                                  //將DO on//
{
    ;
}
//==============================================================================
void TMySYNTEKMotor::MotOutputOff(int iOutPort)                                 //將DO off//
{
    ;
}
//==============================================================================
bool TMySYNTEKMotor::LinearAxisMoveTo(int iPortID[8],long lPos[8],bool bFlag)
{
    return true;
}
//==============================================================================
void TMySYNTEKMotor::EnableTrigger(bool bFlag, int iMode, long lValue)
{
    ;
}
//==============================================================================
bool TMySYNTEKMotor::ResetPos(int p)
{
    if(Enable)
    {
        long lPos;
        _Hon_m4_get_position(Address, iBoardID, iPortID, &lPos);
        _Hon_m4_set_command(Address, iBoardID, iPortID, lPos);
    }
    return true;
}
//==============================================================================
void TMySYNTEKMotor::ResetLatch()
{
    if(Enable==false)
        return;
    _Hon_m4_reset_latch_FIFO(Address, iBoardID);
}
//==============================================================================
int TMySYNTEKMotor::GetLatchTotalLen()
{
    if(Enable==false)
        return 0;

    U16 length;
//    I16 dwRet;
    I16 status;

    status=_Hon_m4_get_latch_FIFO_length(Address, iBoardID, &length);
    if(status!=ERR_NoError)
    {
        return 0;
    }

    return length;
}
//==============================================================================
int TMySYNTEKMotor::GetLatchBuffer(int LatchInputNo, unsigned short *AxisCounterNo, long *LatchDataTable)
{
    if(Enable==false)
        return 0;
    I16 status;
    U16 LatchDataCnt;

    status=_Hon_m4_get_latch_data_from_buffer(Address, iBoardID, LatchInputNo, AxisCounterNo, &LatchDataCnt, &LatchDataTable[0]);
    if(status!=ERR_NoError || LatchInputNo<0)
    {
        return 0;
    }
    return LatchDataCnt;
}
//--------------------------------------------------------------------------
bool TMySYNTEKMotor::GetLatchIOStatus(unsigned short LatchInputNo)
{
    if(Enable==false)
        return false;

    U32 IO_status;
    bool bOnOff;

    _Hon_m4_get_io_status(Address, iBoardID, LatchInputNo, &IO_status);
    bOnOff=IO_status & 0x800;
    return bOnOff;
}
//--------------------------------------------------------------------------
void TMySYNTEKMotor::SetArmMaxSpeed()
{
    if(Enable==false)
        return;
    _Hon_m4_set_velocity_limit(Address, iBoardID, iPortID, PJogHighSpeed*65);
}
//--------------------------------------------------------------------------
void TMySYNTEKMotor::SetServoOn(bool IsOn)
{
    if(Enable==false)
        return;
    _Hon_m4_set_svon(Address, iBoardID, iPortID, IsOn);
}
//--------------------------------------------------------------------------
void TMySYNTEKMotor::SetFIFOLatchSrc(int AxisCounterNo, int LatchInputNo, int bEnable) //ChungHung 20130625 add 跟原本Motor Enable衝突   Enable-->bEnable
{
    if(Enable==false)
        return;
    _Hon_m4_set_FIFO_latch_src(Address, iBoardID, AxisCounterNo, LatchInputNo, bEnable);
}
