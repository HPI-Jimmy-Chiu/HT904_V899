//---------------------------------------------------------------------------

#ifndef mySYNTEKmotorH
#define mySYNTEKmotorH

#include "HTMotor.h"
//#include "HTimer.h"
#include "myTimer.h"
//---------------------------------------------------------------------------
class TMySYNTEKMotor:public HTMotor
{
    private:
        char  cDeviceName[16];
        short Id ;
        int   iSYNTEKHomeObjectTask;
        int   OldSpeed;
        TQPF_Timer HomeDelay;

        void  SYNTEKSoftLimitEnable(bool bFlag);
        bool  SYNTEKMotHome();
        bool  SYNTEKOpenCard();
    protected:
    public:
        __fastcall TMySYNTEKMotor(int Addr);
        ~TMySYNTEKMotor();
        virtual int     InitMotor(int IoAddress);
        virtual void    SetSpeed(unsigned int x, bool bSetJog=false);           //RogerYang 20250729 Add for 9046AU
        virtual void    SetInitSpeed(unsigned int x);
        virtual void    SetServoAlarmOn(bool Value);
        virtual int     ReadPos();
        virtual void    ScanMotorStatus(bool *Led);
        virtual bool    MoveToPos(int Tar);
        virtual void    Stop();
        virtual void    DecStop();
        virtual bool    JogP();
        virtual bool    JogN();
        virtual bool    HomeObject();
        virtual void    SetRange(unsigned int a);
        virtual bool    GetAlarm(void);
        virtual bool    HomeFlag(void);

        virtual void    MotOutputOn(int iOutPort);
        virtual void    MotOutputOff(int iOutPort);
        virtual void    MotInputStatus(bool *bInputPort);
        virtual bool    LinearAxisMoveTo(int iPortID[8],long lPos[8],bool bFlag);
        virtual void    EnableTrigger(bool bFlag,int iMode,long lValue);
        virtual bool    ResetPos(int p =0);
        virtual bool    MotionDone();
        virtual void    SetArmMaxSpeed();
        virtual int     ReadRealPos();
        virtual int     ReadEnCoderRealPos();
        virtual int     SetCommand(int p);
        virtual int     SetPosition(int p);
        virtual void    SetServoOn(bool IsOn);
        virtual void    SetSoftLimit(int iPLimit,int iNLimit);

        //-----------------------------
//        virtual void    SetEnableLatch(bool a);
        virtual void    ResetLatch();
        virtual int     GetLatchTotalLen();
        virtual int     GetLatchBuffer(int LatchInputNo, unsigned short *AxisCounterNo, long *LatchDataTable);
        virtual int     GetLatchBuffer(int LatchInputNo, short *AxisCounterNo, long *LatchDataTable) {return 0;};
        virtual bool    GetLatchIOStatus(unsigned short LatchInputNo);
        virtual void    SetFIFOLatchSrc(int AxisCounterNo, int LatchInputNo, int bEnable); //ChungHung 20130625 add ¸ò­ì¥»Motor Enable½Ä¬ð   Enable-->bEnable
};
#endif
