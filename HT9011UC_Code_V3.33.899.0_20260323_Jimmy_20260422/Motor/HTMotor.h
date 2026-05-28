//---------------------------------------------------------------------------
#ifndef HTMotorH
#define HTMotorH
//---------------------------------------------------------------------------
#include "halarm.h"
//---------------------------------------------------------------------------
#define ALM_MOTOR_MOVE  55555
//---------------------------------------------------------------------------
typedef bool (*PF_CHECK)(void);     // 2015.01.15 , Joye , Safe door check
//---------------------------------------------------------------------------
enum {  iCwLed          =0,
        iHomeLed        =1,
        iCcwLed         =2,
        iEmgLed         =3,
        iAlarmLed       =4,
        iSoftcwLed      =5,
        iSoftccwLed     =6,
        iServoalarmLed  =7,
        iInposLed       =8,
        iServoOn        =9
     };

extern const int Step_Motor  ;
extern const int Servo_Motor ;
extern const int Rotate_Motor;
extern const int YASKAWA_Servo_Motor;
extern const int YASKAWA_Liner_Motor;
extern const int Step_Motor_Oriental;

//---------------------------------------------------------------------------
class HTMotor
{
    private:

    protected:
        unsigned int    iSpeed                                                  ;
        unsigned int    Rate                                                    ;
        unsigned int    Range                                                   ;
        int             iHomeObjectTask                                         ;
        double          dAcc                                                    ;
        double          dDec                                                    ;
        double          dAccDataBase                                            ;
        double          dDecDataBase                                            ;
        byte bAxisID                                                            ;//Jimmychiu 20220926 : ADD MC88X1
    public:
        HTMotor();
        virtual ~HTMotor()                                                      ;
        unsigned int    Address                                                 ;
        unsigned int    iBoardID                                                ;
        unsigned int    iPortID                                                 ;
        unsigned int    PHomeHighSpeed                                          ;
        unsigned int    PHomeLowSpeed                                           ;
        unsigned int    PJogHighSpeed                                           ;
        unsigned int    PJogLowSpeed                                            ;
        unsigned int    InitSpeed                                               ;
        bool            PServoAlarmOn                                           ;
        bool            Enable                                                  ;
        bool            Direction                                               ;
        bool            HomeDirection                                           ;
        int             MotorType                                               ;   //Steven 20170505 (wei) : MotorType從 bool 改成 int
        bool            bSensorType                                             ;
        bool            bLimitLogic                                             ;
        bool            bIn1Logic                                               ;
        double          GearRatio                                               ;
        int             PSoftLimitP                                             ;
        int             PSoftLimitN                                             ;
        int             LastHomePos                                             ;
        int             EncoderType                                             ;
        int             iHomePitch                                              ;

        unsigned int    ReadSpeed()                                             ;
        unsigned int    ReadInitSpeed()                                         ;
        unsigned int    ReadRate()                                              ;
        unsigned int    ReadRange()                                             ;
        double          ReadAcc()                                               ;
        double          ReadDec()                                               ;
        int             ReadEncoderPos()                                        ;
        void            SetHomeobjectTask(int Task)                             ;
        char            ErrorString[256]                                        ;
        short PulseEdgeCnt[8][2];

        virtual void    Stop()                          {}                      ;
        virtual bool    JogP()                          {return false;}         ;
        virtual bool    JogN()                          {return false;}         ;
        virtual int     ReadPos()                       {return 0;}             ;
        virtual bool    MoveToPos(int Tar)              {return false;}         ;
        virtual bool    MoveToPosShortDistance(int Tar) {return MoveToPos(Tar);};
        virtual bool    MoveToPosShortDisSlowSP(int Tar){return MoveToPos(Tar);};
        virtual bool    HomeObject()                    {return false;}         ;
        virtual bool    HomeFlag(void)                  {return false;}         ;
        virtual bool    GetAlarm(void)                  {return false;}         ;
        virtual void    SetSpeed(unsigned int x)        {}                      ;
        virtual void    SetSpeedShortDistance(unsigned int x){SetSpeed(x);     };
        virtual void    SetSpeedShortDisSlowSP(unsigned int x){SetSpeed(x);     };
        virtual void    SetInitSpeed(unsigned int x)    {}                      ;
        virtual void    SetServoAlarmOn(bool Value)     {}                      ;
        virtual int     InitMotor(int IoAddress)        {return 0;}             ;
        virtual void    SetRange(unsigned int a)        {}                      ;
        virtual void    SetRate(unsigned int a)         {}                      ;

        //新增函數//
        virtual bool    ResetPos(int Pulse =0)          {return false;}         ;
        virtual void    SoftLimitEnable(bool bFlag)     {}                      ;
        virtual void    ServerOnOff(bool bStatus)       {}                      ;
        virtual void    MotOutputOn(int iOutPort)       {}                      ;
        virtual void    MotOutputOff(int iOutPort)      {}                      ;
        virtual void    MotInputStatus(bool *bInputPort){}                      ;
        virtual void    EnableTrigger(bool bFalg,int iMode,long lValue){}       ;
        virtual bool    LinearAxisMoveTo(int iPortID[8],long lPos[8],bool bFlag){return false;} ;
        virtual void    ScanMotorStatus(bool *Led)      {};
        virtual void    DecStop(void)                   {};
        virtual bool    MotionDone()                    {return false;};
        virtual void    SetArmMaxSpeed()                {};
        virtual int     ReadRealPos()                   {return 0;};
        virtual int     ReadEnCoderRealPos()            {return 0;};
        virtual int     SetCommand(int p)               {return 0;};
        virtual int     SetPosition(int p)              {return 0;};
        virtual void    SetServoOn(bool IsOn)           {};
        virtual void    SetSoftLimit(int iPLimit, int iNLimit) {};
        virtual void    SetAccDataBase(double a)        {dAccDataBase=a;}       ;
        virtual void    SetDecDataBase(double a)        {dDecDataBase=a;}       ;
        virtual void    SetAcc(double a)                {dAcc=a;}               ;
        virtual void    SetDec(double a)                {dDec=a;}               ;
        virtual double  GetAccDataBase()                {return dAccDataBase;}  ;
        virtual double  GetDecDataBase()                {return dDecDataBase;}  ;

        //Latch
        virtual void    SetEnableLatch(bool a)          {};
        virtual void    ResetLatch()                    {};
        virtual int     GetLatchTotalLen()              {return 0;};
        virtual int     GetLatchBuffer(int LatchInputNo, unsigned short *AxisCounterNo, long *LatchDataTable)   {return 0;};
        virtual int     GetLatchBuffer(int LatchInputNo, short *AxisCounterNo, long *LatchDataTable)  {return 0;};
        virtual bool    GetLatchIOStatus(unsigned short LatchInputNo)  {return false;};
        virtual void    SetFIFOLatchSrc(int AxisCounterNo, int LatchInputNo, int bEnable){};  //ChungHung 20130625 add 跟原本Motor Enable衝突   Enable-->bEnable

        //MN200補間運動
        virtual int  SetGroup(BYTE bGrpNo, BYTE bNumDev, BYTE bDevNo[])         {return 0;};
        virtual int  LineNMove(BYTE bDevNo[], long DevPos[], BYTE bNumDev)      {return 0;};

        bool            CheckIsSafeDoorOpen();  //Jimmychiu 20221013 safedoor判斷整合Function
        PF_CHECK MotorIdleSafeDoorCheck;        // 2015.01.15 , Joye , Safe door check
};
//---------------------------------------------------------------------------
#endif
