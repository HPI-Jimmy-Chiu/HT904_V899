//---------------------------------------------------------------------------
#ifndef myGALILmotorH
#define myGALILmotorH

#include <System.hpp>
#include "HTMotor.h"

class TMyGALILMotor:public HTMotor
{
    private:
    protected:
    public:
        __fastcall TMyGALILMotor(int Addr);
        virtual ~TMyGALILMotor();

        virtual int     InitMotor(int IoAddress);
        virtual void    SetSpeed(unsigned int x);
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
        virtual void    SetRate(unsigned int a);
        virtual bool    GetAlarm(void);
        virtual bool    HomeFlag(void);

        virtual bool    ResetPos(int Pulse=0);
        virtual bool    MotionDone();
        virtual int     ReadRealPos();
        virtual int     ReadEnCoderRealPos();
        virtual int     SetCommand(int p);
        virtual int     SetPosition(int p);
        virtual void    SetServoOn(bool IsOn);
        virtual void    SetSoftLimit(int iPLimit, int iNLimit);
};

bool Open_GaliCard();
bool Close_GaliCard();
void StopAllMotor(bool bIndexCanStop=true);

extern bool bGali_CardInstall;
extern bool bPauseInMotor, bPauseOutMotor;
extern bool bGalilTwoYMoveFlag;
extern const int iPreMovePos;

bool ScanIndexMotorCanMove();
bool ScanIndexVectorCanMove(const char* vecCmd);
bool ScanIndexOtherMotorCanMove(int Index);
void InitialGaliDelayCount();
void InitialSetUpGaliDelayCount(int iDelayCT);

AnsiString GetGalilErrString(long RC, AnsiString FunctionName="");

#endif
