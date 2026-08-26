#include "MachineDefine.h"
#pragma hdrstop
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "HTMC88X1Motor.h"
#include "common.h"
#include "cmydef.h"
#include "Mc88x1p_DLL.h"
#include "mymessbox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

#define MC88X1_RESET_TIMES 90

//---------------------------------------------------------------------------
bool MC88X1CardInstall[MAX_MC88X1_CARD]={false,false,false,false,false,false,false,false,
                                         false,false,false,false,false,false,false,false};
const AnsiString asFileLog=AnsiString("D:\\HT9045_Log\\MC88X1");
AnsiString asFolderLog="";
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
HTMC88X1Motor::HTMC88X1Motor(int Addr)
{
    // 2011.11.02 , Joye , HTMotor ---------
    Address = Addr;
    iBoardID = Address/0x10;
    iPortID = Address%0x10;
    bAxisID = 1<<iPortID;
    Enable = false;
}
//---------------------------------------------------------------------------
HTMC88X1Motor::~HTMC88X1Motor()
{
    Close_Card();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/*
1、開卡時會將軸卡參數戴入
2、參數檔放在固定位置，無法改變
3、沒有Initial到的參數，可用和樁 tool 設定
4、也可以不用Initial以下參數，可全部用和樁 tool設定
*/
int HTMC88X1Motor::InitMotor(int IoAddress)
{
    if (!Enable)
        return true;
    if(Open_Card()==false)
        ShowMyMessage("MC88X1 Card open failed!");
    iHomeType   = 7;
    if (dAcc==0)                     //Nickliu 20230315 add dAcc is 0 set 0.1
        dAcc = 0.1;                                                        //20121203 terry //
    // -------------------------------------
    MC88X1SoftLimitEnable( false );       // 2011.11.04 , Joye , HTMotor
    MC88X1PSetNLimitLogic(iBoardID, bAxisID, bSensorType);
    MC88X1PSetPLimitLogic(iBoardID, bAxisID, bSensorType);
    MC88X1PSetHomeLogic(iBoardID, bAxisID, bSensorType);

    if(PServoAlarmOn==true)
    {
        MC88X1PSetServoAlarm(iBoardID, bAxisID, 1, 1);                          //ServoAlarm預設低準位觸發//
    }
    else
    {
        MC88X1PSetServoAlarm(iBoardID, bAxisID, 0, 0);
    }

    if(MotorType==Step_Motor)
    {
        MC88X1PSetInposition(iBoardID, bAxisID, 0, 0);                          // 2012.03.30 , Joye , Add Terry
    }
    else
    {
        MC88X1PSetInposition(iBoardID, bAxisID, 1, 0);                          // 2012.03.30 , Joye , Add Terry
    }                                                                           //20121203 terry S//
    MC88X1PMotWrReg(iBoardID, bAxisID, HomeType, iHomeType);                    //歸零模式有7種 //
    //HomeType 0x30B 原點搜尋模式

    MC88X1PMotWrReg(iBoardID,bAxisID,HomeP0_Dir,HomeDirection);
    //HomeP0_Dir 0x30C 原點搜尋過程第0 階段方向

    MC88X1PMotWrReg(iBoardID,bAxisID,HomeP0_Speed,PHomeHighSpeed*Range);
    //Home_P0_Speed 0x30D 第0 階段速度

    MC88X1PMotWrReg(iBoardID,bAxisID,HomeP1_Dir,!HomeDirection);
    //HomeP1_Dir 0x30E 原點搜尋過程第1 階段方向

    MC88X1PMotWrReg(iBoardID,bAxisID,HomeP1_Speed,PHomeLowSpeed*Range);
    //HomeP1_Speed 0x30F 第1 階段速度

    MC88X1PMotWrReg(iBoardID,bAxisID,HomeOffset,0);
    //HomeOffset 0x309 機械原點與程式原點的偏移值

    MC88X1PMotWrReg(iBoardID,bAxisID,HomeP2_Dir,HomeDirection);
    //HomeP2_Dir 0x310 原點搜尋過程第2 階段方向

    MC88X1PMotWrReg(iBoardID,bAxisID,HomeOffset_Speed,PHomeLowSpeed*Range);
    //HomeOffset_Speed 0x311 移至程式原點的速度

    MC88X1PMotWrReg(iBoardID,bAxisID,HomeLimitAlarm,0);
    //設定HomeLimitAlarm設定碰到極限是否自動改變方向，若設0，表示自動改變方向，若設1則否。

    if(MotorType)                                                              //總共有9種模式//
        MC88X1PSetPulseMode( iBoardID,bAxisID,4);       // 2011.11.17 , Joye , 6->4
    else
        MC88X1PSetPulseMode( iBoardID,bAxisID,1);
    MC88X1PMotWrReg(iBoardID,bAxisID,HomeClearPos,0x00);
    SetSpeed( PJogLowSpeed );
    // ---------------------------------------------------------
    //加速度=(JogLowSpeed-InitSpeed)/Second      //
    //加加速度 1~JogHighSpeed-JogLowSpeed/2      //
    // 2012.03.30 , Joye , Add Terry -------------------------------------------
    MC88X1PSetEncoderDir(iBoardID,bAxisID,1);
    MC88X1PSetEncoderMultiple(iBoardID,bAxisID,3);
    // -------------------------------------------------------------------------
    // 2011.11.03 , Joye , HTMotor ----
    SetPos(0);
    // --------------------------------
    return true;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void HTMC88X1Motor::Stop(void)
{
    MC88X1PMotStop(iBoardID,bAxisID,0);     // 2012.11.09 , Joye , MC88X1 Stop
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::DecStop(void)
{
    MC88X1PMotStop(iBoardID,bAxisID,0);     // 2012.11.09 , Joye , MC88X1 Stop
}
//---------------------------------------------------------------------------
bool HTMC88X1Motor::JogP(void)
{
    if( Enable==false)
        return false;
    if(CheckIsSafeDoorOpen())        //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }
    else if( !MotionDone() )    // 2012.03.30 , Joye , Add Terry
        return false;
    //Nickli
//    if(Direction)
//    {
//        MC88X1PMotCmove(iBoardID,bAxisID,0x00);
//    }
//    else
//    {
//        MC88X1PMotCmove(iBoardID,bAxisID,bAxisID);
//    }
    //Nickliu 20230314 add MC88X
    if(Direction)
    {
        MC88X1PMotCmove(iBoardID,bAxisID,bAxisID);
    }
    else
    {
        MC88X1PMotCmove(iBoardID,bAxisID,0x00);
    }
    //Nickliu 20230314 add MC88X
    return true;
}
//---------------------------------------------------------------------------
bool HTMC88X1Motor::JogN(void)
{
    if( Enable==false)
        return false;
    if(CheckIsSafeDoorOpen())    //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }
    else if( !MotionDone() )    // 2012.03.30 , Joye , Add Terry
        return false;

//    if(Direction)
//    {
//        MC88X1PMotCmove(iBoardID,bAxisID,bAxisID);
//    }
//    else
//    {
//        MC88X1PMotCmove(iBoardID,bAxisID,0x00);
//    }
    //Nickliu 20230314 add MC88X
    if(Direction)
    {
        MC88X1PMotCmove(iBoardID,bAxisID,0x00);
    }
    else
    {
        MC88X1PMotCmove(iBoardID,bAxisID,bAxisID);
    }
    //Nickliu 20230314 add MC88X
    return true;
}
//---------------------------------------------------------------------------
//20111124 terry modify 相對移動決對移動//
bool HTMC88X1Motor::MoveTo(int Tar)    // MoveTo()   //HT70XX is G00
{
    if(CheckIsSafeDoorOpen())        //Jimmychiu 20221013 add
    {
        return false;
    }

    if(MC88X1CardInstall[iBoardID]==false)
        return false;
    if(Enable==false )
        return true;
    if( !MotionDone() )
        return false;
    int iP1=0,iP2=0;
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
    RealG00(iP1);
    if(MotionDone() && iP1==ReadRealPos())
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void HTMC88X1Motor::SetRange(unsigned int a)
{
    if(a>1000) a=1000;   // 2011.11.18 , Joye
    Range=a;
    SetMC88X1MotPara();
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::SetRate(unsigned int a)
{
    if(a<1) return;     // 2011.11.18 , Joye
    Rate = a;
    dAcc = (PJogHighSpeed-InitSpeed)*Rate/8000000.;  // 8 x 10^6      // 2011.11.28 , Joye
    SetMC88X1MotPara();
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::SetAccDataBase(double a)
{
    SetRate(a);
    dAccDataBase=a;
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::SetDecDataBase(double a)
{
    SetRate(a);
    dDecDataBase=a;
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::SetAcc(double a)
{
    dAcc=a;
    SetMC88X1MotPara();
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::SetDec(double a)
{
    dAcc=a;
    SetMC88X1MotPara();
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::SetSpeed(unsigned int x, bool bSetJog)
{
    if( x > 8191 )          // 2011.11.18 , Joye
        x = 8191;
    else if( x < 1 )
        x = 1;
    if( x < InitSpeed )
        x = InitSpeed*2;
    iSpeed=x;
    SetMC88X1MotPara();
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::SetInitSpeed(unsigned int x)
{
    InitSpeed=x;
    SetMC88X1MotPara();
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::SetPos(int p)
{
    if(Enable)
    {
        MC88X1PSetTheorecticalRegister(iBoardID, bAxisID, p);
    }
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::SetEnCoderPos(int p)
{
    if(Enable)
    {
        MC88X1PSetPracticalRegister(iBoardID, bAxisID, p);
    }
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::SetSoftLimit(int iPLimit,int iNLimit)
{
    int LP=0,LN=0;
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
    MC88X1SoftLimitEnable( true );       // 2011.11.04 , Joye , HTMotor
    LP=ChangeToFloatNonPcnt((double)(LP), (double)(GearRatio));
    LN=ChangeToFloatNonPcnt((double)(LN), (double)(GearRatio));
    MC88X1PSetCompNLimit(iBoardID,bAxisID,LN);
    MC88X1PSetCompPLimit(iBoardID,bAxisID,LP);
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::SetServoAlarmOn(bool Value)
{
    PServoAlarmOn = Value;
}
//---------------------------------------------------------------------------
int HTMC88X1Motor::ReadPos()
{
    return ReadRealPos()*GearRatio;
}
//---------------------------------------------------------------------------
bool HTMC88X1Motor::GetAlarm()
{
    if(!PServoAlarmOn)
        return false;
    byte bMotorStatus=0;
    MC88X1PMotDI(iBoardID, bAxisID, &bMotorStatus);
    bool bAlarm = bMotorStatus & 0x80;
    return bAlarm;
}
//---------------------------------------------------------------------------
void HTMC88X1Motor::ScanMotorStatus(bool *bLed)
{
    bool Flag=false;
    if(Enable)
    {
        byte bMotorStatus;
        MC88X1PMotDI(iBoardID, bAxisID, &bMotorStatus);
        if(MotorType==Step_Motor)
            bLed[iInposLed]=false;
        else
            bLed[iInposLed]=bMotorStatus&0x40;                                  //InPos
        // 2013.09.02 , Joye , KYEC MC88X1 -------------------------------------
        Flag=bMotorStatus&0x08;
        if(bSensorType)
            bLed[iHomeLed]=Flag;                                                // Home
        else
            bLed[iHomeLed]=!Flag;                                               // Home
        // ---------------------------------------------------------------------
        if(!PServoAlarmOn)
            bLed[iServoalarmLed]=false;                                         //ALARM
        else
            bLed[iServoalarmLed]=bMotorStatus&0x80;                             //ALARM
        /*
        1、和樁卡片EMG只要輸入0v，就會將馬達緊停。
        2、和樁軸卡沒有偵測EMG狀態，如果要偵測，可用IN1或IN2的訊號，當按緊停時，輸入0V就可使用EMG訊號
        3、目前公司做法是按緊停按鈕時，將Servo On訊號斷掉，ChangeToFloatNonPcnt((double)(再由I), (double)(O偵測EMG訊號))
        Led[iEmgLed]=!(bMotorStatus&0x02);                                      //EMG.S
        */
        bLed[iEmgLed        ]=!(bMotorStatus&0x02);                             //EMG.S
        bLed[iServoOn       ]=(bMotorStatus&0x02);
        unsigned short sMotorStatus=0;
        MC88X1PGetMotionInput(iBoardID, bAxisID, &sMotorStatus);
        bLed[iCwLed         ]=sMotorStatus&0x04;                                //CW
        bLed[iCcwLed        ]=sMotorStatus&0x08;                                //CCW
        bLed[iSoftcwLed     ]=sMotorStatus&0x01;                                //Soft CW
        bLed[iSoftccwLed    ]=sMotorStatus&0x02;                                //Soft CCW
        bLed[iServoalarmLed ]=sMotorStatus&0x10;                                //Servo ALARM
        if(bLed[iCwLed] || bLed[iCcwLed])
            bLed[iAlarmLed]=true;
    }
}
//---------------------------------------------------------------------------
// 2013.04.03 , Joye , MC88X1 (Jason)
bool HTMC88X1Motor::HomeFlag(void)
{
    // 2013.09.02 , Joye , KYEC MC88X1 -------------------------------------
    WORD Data2=0;
    ReadStatus( 0x08, &Data2 );//Read PG signal read port
    Data2=Data2 & 0x0080;//Home signal
    if( bSensorType )
    {
        if(Data2 == 0)
            return true;
        else
            return false;
    }
    else
    {
        if(Data2 == 1)
            return true;
        else
            return false;
    }
}
//---------------------------------------------------------------------------
// MC88X1 Motor Only --------------------------------------------------------
// open card //
bool HTMC88X1Motor::Open_Card()
{
    if( iBoardID<0 || iBoardID>=MAX_MC88X1_CARD)
        return false;
    if( MC88X1CardInstall[iBoardID] == false )
    {
        if ( ERROR_SUCCESS != MC88X1PMotDevOpen(iBoardID))
            return false;
        else
        {
            MC88X1CardInstall[iBoardID]=true;
            if(DestructorCount[iBoardID]==0)
                DestructorCount[iBoardID]=1<<1;
            else
                DestructorCount[iBoardID]=DestructorCount[iBoardID]<<1;
            return true;
        }
    }
    return true;
}
//---------------------------------------------------------------------------
// close card //
void HTMC88X1Motor::Close_Card()
{
    DestructorCount[iBoardID]=DestructorCount[iBoardID]>>1;
    if(DestructorCount[iBoardID]!=0)
        return;
    if(MC88X1CardInstall[iBoardID]==true)
    {
        MC88X1CardInstall[iBoardID]=false;
        MC88X1PMotDevClose(iBoardID);
    }
}
//---------------------------------------------------------------------------
// softlimit enable or disable //
void HTMC88X1Motor::MC88X1SoftLimitEnable(bool bFlag)
{
    if(bFlag)
        MC88X1PEnableCompLimit(iBoardID,bAxisID,0x01);
    else
        MC88X1PEnableCompLimit(iBoardID,bAxisID,0x00);
}
//---------------------------------------------------------------------------
// motion done //
bool HTMC88X1Motor::MotionDone()
{
    if(Enable)
    {
        long lReturnValue=MC88X1PMotAxisBusy(iBoardID, bAxisID);
        if(ERROR_SUCCESS==lReturnValue)
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
// command //
int HTMC88X1Motor::ReadRealPos()
{
    long lGetCommandPos=0;
    MC88X1PGetTheorecticalRegister(iBoardID, bAxisID, &lGetCommandPos);

    if(Direction)                                                               //Nickliu 20230314 add MC88X
        lGetCommandPos=-lGetCommandPos;

    return(lGetCommandPos);
}
//---------------------------------------------------------------------------
// EnCoder //
int HTMC88X1Motor::ReadEnCoderRealPos()
{
    long lGetFeedbackPos=0;
    if(MotorType==Step_Motor)
        MC88X1PGetTheorecticalRegister(iBoardID, bAxisID, &lGetFeedbackPos);
    else
        MC88X1PGetPracticalRegister(iBoardID, bAxisID, &lGetFeedbackPos);

    if(Direction)                                                               //Nickliu 20230314 add MC88X
        lGetFeedbackPos=-lGetFeedbackPos;

    return(lGetFeedbackPos);
}
//---------------------------------------------------------------------------
// 2013.04.03 , Joye , MC88X1 (Jason)
bool HTMC88X1Motor::MC88X1MotHome()
{
    int iMaxTryCT=0;
    int &Task=iHomeObjectTask;
    unsigned int iAccPersent=0,iAccMax=0;
    switch(Task)
    {
        case 1:
            if(!MotionDone())
            {
                ClearAxisAlarm();
                break;
            }
            SetPos(0);
            SetEnCoderPos(0);
            OldSpeed=iSpeed;
            Stop();
            InitMotor(Address);
            MC88X1SoftLimitEnable(false);
            LastHomePos=0;
            Task=5;
            break;
        case 5:                                                                //20111230//
            iAccPersent=(PHomeHighSpeed*Range)/65535;
            if(iAccPersent==0)
                iAccPersent=1;
            iAccMax=iAccPersent*8192000;
            MC88X1PMotAxisParaSet(iBoardID,bAxisID,0,(PHomeLowSpeed*Range)/2,                    //第三組0是T型，1是S型//
                                  PHomeLowSpeed*Range,PHomeHighSpeed*Range,iAccMax,100 );          //第八組S型的加加速度//
            Task=10;
            break;
        case 10:
            WriteStatus( 0x02, 0xffff );
            WriteStatus( 0x04, 0x04 );//Write high speed data
            WriteStatus( 0x02, PHomeHighSpeed*Range );
            WriteStatus( 0x04, 0x02 );//Write low speed data
            Task=11;
            break;
        case 11:
            if(HomeFlag())
                Task=13;
            else
            {
                IfTouchHomeStop();
                Task=12;
            }
            break;
        case 12:
            if(MotionDone()==false)
                break;
            Task=13;
//            break;
        case 13:
            if(MotionDone()==false)
                break;
            if(HomeFlag())
            {
                WriteStatus( 0x02, 0xffff );
                WriteStatus( 0x04, 0x04 );//Write high speed data
                WriteStatus( 0x02, PHomeLowSpeed*Range );
                WriteStatus( 0x04, 0x02 );//Write low speed data
                IfLeaveHomeStop();
                Task=14;
            }
            else
                Task=1;
            break;
        case 14:
            if(MotionDone()==false)
                break;
            LevalHomeSensorConstDistance();
            Task=15;
        case 15:
            if(MotionDone()==false)
                break;
            IfTouchHomeStop();
            Task=16;
        case 16:
            if(MotionDone()==false)
                break;
            if(HomeFlag())
            {
                TouchHomeSensorConstDistance(10);
                iMaxTryCT=ChangeToFloatNonPcnt((double)(200), (double)(GearRatio)); // if still no find home sensor
                Task=17;
            }
            else
                Task=1;
            break;
        case 17:
            if(HomeFlag()==false)   // can not detect home sensor
            {
                if(iMaxTryCT)
                {
                    TouchHomeSensorConstDistance(1);
                    iMaxTryCT--;
                    return false;
                }
            }
            Task=20;
            break;
        case 20:
            if(HomeFlag())
                Task=30;
            break;
        case 30:
            if(MotionDone())
                Task=31;
            break;
        case 31:
            LastHomePos=-ReadPos();
            SetPos(0);          // 2013.04.03 , Joye , MC88X1
            MySleep(100);
            SetEnCoderPos(0);
            SetSoftLimit(PSoftLimitP, PSoftLimitN);
            MC88X1SoftLimitEnable(true);
            SetSpeed(OldSpeed);
            Task=1;
            return true;
        default:
            Task=1;
            return false;
    }
    return(false);
}
//---------------------------------------------------------------------------
//將Home Error重置//
void HTMC88X1Motor::MC88X1HomeReset()
{
    MC88X1PMotHomeReset(iBoardID,bAxisID);
}
//---------------------------------------------------------------------------
//將直線補間Error重置//
void HTMC88X1Motor::MotIpReset()
{
    MC88X1PMotIpReset(iBoardID);
}
//---------------------------------------------------------------------------
// Set Motor Parame
void HTMC88X1Motor::SetMC88X1MotPara()
{
    if(Enable)
    {
        // 2011.11.30 , Joye , 加入 Terry 的保護 ----------
        //dMC88X1Acc = ( iSpeed*Range - InitSpeed*Range ) / dAcc;
        dAcc=1;
        dMC88X1Acc = ( iSpeed*Range - InitSpeed*Range ) / dAcc;
        unsigned int iAccPersent,iAccMax,iAccMin;
        iAccPersent = (iSpeed*Range)/65535;
        if( iAccPersent == 0 )
            iAccPersent = 1;
        iAccMin = iAccPersent*2001;
        iAccMax = iAccPersent*8192000;
        if( dMC88X1Acc < iAccMin )
            dMC88X1Acc = iAccMin;
        if( dMC88X1Acc > iAccMax )
            dMC88X1Acc = iAccMax;
//        LRESULT lRes = MC88X1PMotAxisParaSet(iBoardID , bAxisID ,
//                                     0 ,                   //第三組0是T型，1是S型
//                                     InitSpeed*Range ,     //初速度
//                                     iSpeed*Range ,        //驅動速度
//                                     2000,//PJogHighSpeed*Range , //最大速度
//                                     dMC88X1Acc ,          //加速度
//                                     100 );                //第八組S型的加加速度
         LRESULT lRes = MC88X1PMotAxisParaSet(iBoardID , bAxisID ,
                                   0 ,                   //第三組0是T型，1是S型
                                   InitSpeed*Range ,     //初速度
                                   iSpeed*Range ,        //驅動速度
                                   PJogHighSpeed*Range,  //最大速度      //Nickliu 20230314 add MC88X
                                   dMC88X1Acc ,          //加速度
                                   100 );                //第八組S型的加加速度
        if( lRes != ERROR_SUCCESS )
        {
            /*
            int x=0;
            if( lRes == ParaValueErr )
                x = 1;
            else if( lRes == ParaValueUnderRange )
                x = 1;
            else if( lRes == ParaValueOverRange )
                x = 1;
            */
        }
    }
}
//---------------------------------------------------------------------------
//20111124 terry modify 相對移動決對移動//
void HTMC88X1Motor::RealG00(int iPos)
{
    long lDistance[8] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
    if(Direction)
        lDistance[iPortID]=-iPos;
    else
        lDistance[iPortID]=iPos;
    MC88X1PMotPtp(  iBoardID,  //board ID
                    bAxisID,   //Axis
                    bAxisID,   //RA 0:Relatively 1:Absolute
                    lDistance[0] , lDistance[1] , lDistance[2] , lDistance[3] ,
                    lDistance[4] , lDistance[5] , lDistance[6] , lDistance[7] );
    if (GetAlarm())                                                 //pig 2016.03.07 MototAlarmCode
    {
        //Alarm->Set(ALM_MOTOR_MOVE);             // 無法移動到定位
        MotorLog(__FUNC__,"ALM_MOTOR_MOVE");
    }
}
//---------------------------------------------------------------------------
//20111124 terrry add//
void HTMC88X1Motor::ClearAxisAlarm()
{
    if(AxisIpBusy==MC88X1PMotAxisBusy(iBoardID,bAxisID))
        MotIpReset();
    else if(AxisHomeBusy==MC88X1PMotAxisBusy(iBoardID,bAxisID))
        MC88X1HomeReset();
    else if(AxisHomeErr==MC88X1PMotAxisBusy(iBoardID,bAxisID))
        MC88X1HomeReset();
}
//==============================================================================

// 2013.04.03 , Joye , MC88X1 (Jason) ---------------------------------------->>
//==============================================================================
bool HTMC88X1Motor::LimitPFlag(void)
{
    WORD Data2=0;
    ReadStatus( 0x08, &Data2 );//Read PG signal read port
    Data2=Data2 & 0x0100;//Home signal
    if( bSensorType )
    {
        if(Data2 > 0)
            return true;
        else
            return false;
    }
    else
    {
        if(Data2 == 0)
            return true;
        else
            return false;
    }
}
//==============================================================================
bool HTMC88X1Motor::LimitNFlag(void)
{
    WORD Data2;
    ReadStatus( 0x08, &Data2 );//Read PG signal read port
    Data2=Data2 & 0x0200;//Home signal
    //if(bSensorType)
    if( bSensorType )
    {
        if(Data2 > 0)
            return true;
        else
            return false;
    }
    else
    {
        if(Data2 == 0)
            return true;
        else
            return false;
    }
}
//==============================================================================
bool HTMC88X1Motor::ReadStatus(DWORD offset,WORD *ReadData)
{
    bool ret;
    WORD Data;
    DWORD Port;
    Port=(0x80)*(iPortID/4)+(0x10)*(iPortID%4);
    if(Enable)
    {
        ret = MC88X1PReadWord(iBoardID,offset+Port, &Data);
        *ReadData=Data;
        return ret;
    }
    return true;    // 2013.04.03 , Joye , MC88X1
}
//==============================================================================
bool HTMC88X1Motor::WriteStatus(DWORD offset,WORD WriteData)
{
    DWORD Port;
    Port=(0x80)*(iPortID/4)+(0x10)*(iPortID%4);
    if(Enable)
    {
        return MC88X1PWriteWord(iBoardID, offset+Port, WriteData );
    }
    return true;    // 2013.04.03 , Joye , MC88X1
}
//==============================================================================
int HTMC88X1Motor::IfTouchHomeStop()
{
    WORD Data2;
    int ret=0;
    ret=WriteStatus( 0x04, 0x15 );//Read signal selector
    ret=ReadStatus( 0x02, &Data2 );//Read signal selector data
    Data2=Data2<<4;
    Data2=Data2>>4;
    if( bSensorType )
        Data2=Data2 | 0x7000;  //home signal high to low
    else
        Data2=Data2 | 0xf000;  //home signal low to high

    ret=WriteStatus( 0x02, Data2 );//Write signal selector data
    ret=WriteStatus( 0x04, 0x14 );//Write signal selector
    if(HomeDirection)
        ret=WriteStatus( 0x04, 0x27 );//- signal scan-b drive
    else
        ret=WriteStatus( 0x04, 0x26 );//+ signal scan-b drive
    return ret;
}
//==============================================================================
void HTMC88X1Motor::IfLeaveHomeStop()
{
    WORD Data2=0;
    WriteStatus( 0x04, 0x15 );//Read signal selector
    ReadStatus( 0x02, &Data2 );//Read signal selector data
    Data2=Data2<<4;
    Data2=Data2>>4;
    //if(bSensorType)
    if( bSensorType )
        Data2=Data2 | 0xf000;  //home signal low to high
    else
        Data2=Data2 | 0x7000;  //home signal high to low
    WriteStatus( 0x02, Data2 );//Write signal selector data
    WriteStatus( 0x04, 0x14 );//Write signal selector
    if(HomeDirection)
        WriteStatus( 0x04, 0x26 );//+ signal scan-b drive
    else
        WriteStatus( 0x04, 0x27 );//- signal scan-b drive
}
//==============================================================================
// 2013.09.02 , Joye , KYEC MC88X1 Home Type 90
void HTMC88X1Motor::LevalHomeSensorConstDistance(int Pulse)
{
    WORD Data1,Data2;
    DWORD Data;
    // 2013.09.02 , Joye , KYEC MC88X1 Home Type 90 -------
    Pulse=ChangeToFloatNonPcnt((double)(Pulse), (double)(GearRatio));
    if( abs( Pulse ) < 1 )
        Pulse = 1;
    // ----------------------------------------------------
    Data1=Pulse >> 16;
    Data=Pulse << 16;
    Data2=Data >> 16;
    WriteStatus( 0x00, Data1 );//Read signal selector data
    WriteStatus( 0x02, Data2 );//Read signal selector data
    WriteStatus( 0x04, 0x15 );//Read signal selector
    if(HomeDirection)
        WriteStatus(0x04,0x20);//+ index pulse drive
    else
        WriteStatus(0x04,0x21);//- index pulse drive
}
//==============================================================================
void HTMC88X1Motor::TouchHomeSensorConstDistance(int Pulse)
{
    WORD Data1,Data2;
    DWORD Data;
    // 2013.09.02 , Joye , KYEC MC88X1 Home Type 90 -------
    Pulse=ChangeToFloatNonPcnt((double)(Pulse), (double)(GearRatio));
    if( abs( Pulse ) < 1 )
        Pulse = 1;
    // ----------------------------------------------------
    Data1=Pulse >> 16;
    Data=Pulse << 16;
    Data2=Data >> 16;
    WriteStatus( 0x00, Data1 );//Read signal selector data
    WriteStatus( 0x02, Data2 );//Read signal selector data
    WriteStatus( 0x04, 0x15 );//Read signal selector
    if(HomeDirection)
        WriteStatus(0x04,0x21);//- index pulse drive
    else
        WriteStatus(0x04,0x20);//+ index pulse drive
}
//==============================================================================
// 2013.04.03 , Joye , MC88X1 (Jason) ----------------------------------------<<
//---------------------------------------------------------------------------
//Mylin 20200603 Add Set Servo On Off for MC88X1Motor ==>
void HTMC88X1Motor::SetServoOn(bool IsOn)
{
    if(!PServoAlarmOn)
        return;
    byte bMotorGetOutput=0x0;
    byte bMotorSetOutput=0x0;
    byte bSetValue=0x0;
    MC88X1PGetOutput(iBoardID,bAxisID,&bMotorGetOutput);
    if(IsOn==true)
    {
        bSetValue=0x01;//0000 0001
        bMotorSetOutput = bMotorGetOutput | bSetValue;
    }
    else
    {
        bSetValue=0xfe;//1111 1110
        bMotorSetOutput = bMotorGetOutput & bSetValue;
    }
    MC88X1PMotDO(iBoardID,bAxisID,bMotorSetOutput);
}
//Mylin 20200603 Add Set Servo On Off for MC88X1Motor <==
//---------------------------------------------------------------------------
void HTMC88X1Motor::MotorLog(AnsiString asFuncName,AnsiString Message)
{
    if(!(DirectoryExists(asFileLog)))
        ForceDirectories(asFileLog);
    asFolderLog.sprintf("%s\\%04d%02d", asFileLog, SystemYear, SystemMonth);
    if(!(DirectoryExists(asFolderLog)))
        ForceDirectories(asFolderLog);
    asFolderLog.sprintf("%s\\%02d.txt", asFolderLog, SystemDate);
    Message.sprintf("%02d%02d%02d Func:%s Msg:%s", SystemHour, SystemMin, SystemSec, asFuncName, Message);
    WriteDataToFile(asFolderLog,Message);
}
//--------------------------------------------------------------------------
bool HTMC88X1Motor::HomeObject()
{
    if(CheckIsSafeDoorOpen())        //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }
    //2012.12.03 Q_Q Hontech HOME
    return MC88X1MotHome();
}
//--------------------------------------------------------------------------
bool HTMC88X1Motor::MoveToPos(int Tar)
{
    if(Enable==false)
        return true;
    if(!MotionDone())
        return false;
    int iPos=ReadPos();
    SetSpeed(iSpeed);
    RealG00(Tar);
    if(MotionDone() && Tar==iPos)
        return true;
    else
        return false;
}
//--------------------------------------------------------------------------
