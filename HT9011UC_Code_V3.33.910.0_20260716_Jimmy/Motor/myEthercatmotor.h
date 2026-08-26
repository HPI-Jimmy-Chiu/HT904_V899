//---------------------------------------------------------------------------
#ifndef myEthercatmotorH
#define myEthercatmotorH
//---------------------------------------------------------------------------
//#include "MyMotor.h"
#include "HTMotor.h"
#include "HTimer.h"
#include "AdvMotDrv.h"
#include "AdvMotApi.h"
//#include "General.h"

//---------------------------------------------------------------------------
extern unsigned int gDevhand;
extern void EtherCatWriteAO(F32 Data);
//---------------------------------------------------------------------------
class TMyEtherCatMotor:public HTMotor
{
    //friend class HTAdvantechGroup;

    private:
        //char  cDeviceName[24];
        AnsiString  cDeviceName;
        short   MotorID ;
        int   iSMCHomeObjectTask;
        int   OldSpeed;
        static unsigned int gDevhand;
        bool   DoHome(unsigned int mode, unsigned int dir);
        void   Open_Card();
        void   Close_Card();
        void   Open_Axis();
        HTimer HomeDelay;
        bool EtherCatMotHome();
        int iMotorCount;
        void  SetEtherCatInType();
        void SetHomeSpeed();
        bool bAxisOpen;                                                         //RogerYang 20250402 9046AU
        HAND m_Axishand[999];                                                   //RogerYang 20250402 9046AU
    protected:
        short  iBoardID;
        short  iPortID;
        ULONG MotorCount;
        int  bAxisID;
        double dAcc;

        unsigned int axisHand;

        bool GetHomeIO(void);
        bool RealG00(int p);
        //int ReadRealPos(void);

        bool Error(void);
        bool Busy(void);

        bool PosDirectHome();
        bool NegDirectHome();
        bool PosDirectHomeOneTouch();
        bool NegDirectHomeOneTouch();
//        bool AddAxis(int iAxis);
//        bool HomeType90();

    public:
        __fastcall TMyEtherCatMotor(int addr);
        ~TMyEtherCatMotor();

        virtual int     InitMotor(int IoAddress);

        virtual bool    G00(int iPos);           //改用Move to

        virtual void    SetRate(unsigned int a); //其他沒用
        virtual void    SetSpeed(unsigned int x, bool bSetJog=false);
        virtual void    SetInitSpeed(unsigned int x);
        virtual void    SetServoAlarmOn(bool Value);
        virtual void    SetAcc(double a);
        virtual void    SetDec(double a);

        virtual int     ReadPos();
        virtual void    ScanMotorStatus(bool *Led);
        virtual bool    MoveTo(int Tar);
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
        virtual void    MotInputStatus(bool *bInputPort);//not sure
        virtual bool    LinearAxisMoveTo(int iPortID[8],long lPos[8],bool bFlag);   //not sure
        virtual void    EnableTrigger(bool bFlag,int iMode,long lValue);            //not sure

        virtual bool    AddAxis(int axis);  //mark //Jeffrey 20161005 Add 4 AdvantechMotor
        virtual bool    AddPath(int axis);  //mark //Jeffrey 20161005 Add 4 AdvantechMotor
        virtual bool    RunPath(void);      //mark //Jeffrey 20161005 Add 4 AdvantechMotor

        //virtual void    SetPos(int p);//改用SetPosition

        virtual bool    ResetPos(int Pulse);
        virtual bool    MotionDone();
        virtual int     ReadRealPos();
        virtual int     ReadEnCoderRealPos();
        virtual int     SetCommand(int p);
        virtual int     SetPosition(int p);
        virtual void    SetServoOn(bool IsOn);
        virtual void    SetSoftLimit(int iPLimit,int iNLimit);

        //-----------------------------
//        virtual void    SetEnableLatch(bool a);
        virtual bool    MoveToPos(int Tar);
        virtual void    ResetLatch();
        virtual int     GetLatchTotalLen();
        virtual int     GetLatchBuffer(int LatchInputNo, short *AxisCounterNo, long *LatchDataTable);
        virtual int     GetLatchBuffer(int LatchInputNo, unsigned short *AxisCounterNo, long *LatchDataTable) {return 0;};
        virtual bool    GetLatchIOStatus(unsigned short LatchInputNo);
        virtual void    SetFIFOLatchSrc(int AxisCounterNo, int LatchInputNo, int bEnable); //ChungHung 20130625 add 跟原本Motor Enable衝突   Enable-->bEnable

        virtual void    ResetState();
        virtual void    ResetAxisOpen();
        //
        int iDelayCount;
        int iWaitCount;
        int iStepRange;

        ULONG AxisCount;
        DWORD m_dwDevNum;
        //

        // 2018.12.13 Advantech Added Ether DO
        //int ethercat_set_output_bit(int Ring, int IP, int Port, int Bit, int Data);
        int ethercat_set_output_bit(int Ring, int IP, int Port, int Bit, int Data);
        int ethercat_set_output_byte(int Ring, int IP, int Port, int Byte);
        bool IOBitOff(int port, int bit);
};
//---------------------------------------------------------------------------
#endif