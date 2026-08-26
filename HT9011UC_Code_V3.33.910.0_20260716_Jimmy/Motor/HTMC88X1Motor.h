//---------------------------------------------------------------------------
#ifndef HTMC88X1MotorH
#define HTMC88X1MotorH
//---------------------------------------------------------------------------
#include "HTMotor.h"
//---------------------------------------------------------------------------
#define MAX_MC88X1_CARD 16
//---------------------------------------------------------------------------
class HTMC88X1Motor : public HTMotor
{
    private:
        void MotorLog(AnsiString asFuncName,AnsiString Message);
        byte DestructorCount[MAX_MC88X1_CARD];
    protected:
        int  iBoardID;
        int  iPortID;
        double dAcc;
        double dMC88X1Acc;
        int OldSpeed;
        int OldInitSpeed;       // 2011.11.18 , Joye
        int OldRate;            // 2011.11.18 , Joye
        bool Open_Card();
        void Close_Card();
        void MC88X1SoftLimitEnable(bool bFlag);
        bool MotionDone();
//        int ReadMC88X1RealPos();
//        int ReadMC88X1EnCoderRealPos();
        bool MC88X1MotHome();
        void MC88X1HomeReset();
        void MotIpReset();
        void SetMC88X1MotPara();
        void RealG00( int iPos );
        void ClearAxisAlarm();    //20111124 terry
        // 2013.04.03 , Joye , MC88X1 (Jason) ----------------------------------
        int IfTouchHomeStop()                                                  ;
        void IfLeaveHomeStop()                                                  ;
        void TouchHomeSensorConstDistance(int Pulse)                            ;
        // ---------------------------------------------------------------------
        void LevalHomeSensorConstDistance(int iPulse=200);      // 2013.04.30 , Joye , Home Type 90
        int iHomeType;
    public:
        HTMC88X1Motor(int Addr);
        ~HTMC88X1Motor();
        virtual int     InitMotor(int IoAddress);       // 起始設定函數
        virtual void    Stop(void);
        virtual void    DecStop();
        virtual bool    JogP(void);
        virtual bool    JogN(void);
        virtual bool    MoveTo(int iPos);                  // 移動點位
        virtual void    SetRange(unsigned int a);
        virtual void    SetRate(unsigned int a);
        virtual void    SetAccDataBase(double a);
        virtual void    SetDecDataBase(double a);
        virtual void    SetAcc(double a);
        virtual void    SetDec(double a);
        virtual void    SetSpeed(unsigned int x, bool bSetJog=false);
        virtual void    SetInitSpeed(unsigned int x);
        virtual void    SetPos(int p);
        virtual void    SetEnCoderPos(int p);
        virtual void    SetSoftLimit(int lp,int ln);
        virtual void    SetServoAlarmOn(bool );
        virtual int     ReadPos();                      // 讀取馬達位置(1/100mm)
        virtual bool    GetAlarm();
        virtual void    ScanMotorStatus(bool *bLed);
        virtual bool    HomeFlag(void);
        // 2013.04.03 , Joye , MC88X1 (Jason) ----------------------------------
        virtual bool    LimitPFlag(void)                                        ;
        virtual bool    LimitNFlag(void)                                        ;
        virtual bool    ReadStatus(DWORD offset, WORD *ReadData)                ;
        virtual bool    WriteStatus(DWORD offset, WORD WriteData)               ;
        // ---------------------------------------------------------------------
        void SetServoOn(bool IsOn); //Mylin 20200603 Add Set Servo On Off for MC88X1Motor
        virtual bool    HomeObject();
        virtual bool    MoveToPos(int Tar);
        virtual int     ReadRealPos();
        virtual int     ReadEnCoderRealPos();
};
//---------------------------------------------------------------------------

#endif
