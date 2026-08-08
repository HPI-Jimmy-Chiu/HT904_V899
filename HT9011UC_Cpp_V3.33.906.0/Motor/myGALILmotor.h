// =============================================================================
//  Motor/myGALILmotor.h  -- TMyGALILMotor (Galil DMC32 axis driver, HTMotor
//                           subclass) + the Galil card open/close + index
//                           four-axis vector-move gatekeepers.
//
//  Faithful translation of golden Motor/myGALILmotor.h (59 lines, BCB6).
//  Translator: AI(W906-PT-W4-galil) 20260808
//  Translation wave: W906-PT-W4 (genuinely-missing non-form units).
//
//  Key changes vs. the BCB6 original, and why:
//    1. `#include <System.hpp>` DROPPED (BCB/Delphi RTL).  Replaced by
//       "vclcompat/vcl_compat.h" (neutralises __fastcall / supplies AnsiString)
//       exactly as Motor/HTMotor.h does.  Nothing else in this header needed
//       System.hpp: the only RTL type it names is AnsiString (GetGalilErrString).
//    2. `__fastcall` on the ctor is dropped (vcl_compat.h neutralises the
//       keyword anyway; the golden spelling is kept in a comment).
//
//  STOPALLMOTOR SIGNATURE COLLISION -- READ BEFORE INCLUDING THIS HEADER
//  ---------------------------------------------------------------------------
//  Golden declares `void StopAllMotor(bool bIndexCanStop=true);` HERE, and its
//  ONE real body is golden Motor/myGALILmotor.cpp:4712 (translated in this
//  unit's .cpp).  This port tree meanwhile carries an UNRELATED no-arg
//  declaration `extern void StopAllMotor();` at aHotPlateSubstrate.h:923 with an
//  empty body at aHotPlateSubstrate.cpp:1063, and 8 existing call sites
//  (ainarm9045.cpp:871, ckernel.cpp:1041/1048/1250, AutoClean/AutoClean.cpp:3251,
//  TempCtrl/TriTemp.cpp:1568/1685/3828, SortingBinTray/SortingBinTray.cpp:
//  1351/1375) reach that no-op through it.
//  These two declarations are OVERLOADS, not redeclarations, so they link
//  side-by-side -- but a TU that includes BOTH headers turns every `StopAllMotor()`
//  into an AMBIGUOUS call (hard error).  No such TU exists today (this header is
//  new and only this unit's .cpp includes it), but the main loop MUST NOT add
//  this include to Motor/mymotor.h or any widely-included hub without first
//  unifying the two.  See the .cpp banner's INTEGRATION REQUESTS.
// =============================================================================
#ifndef myGALILmotorH
#define myGALILmotorH

#include "vclcompat/vcl_compat.h"   // AnsiString + __fastcall/__published neutralisation (golden: <System.hpp>)
#include "Motor/HTMotor.h"          // golden: "HTMotor.h"

class TMyGALILMotor : public HTMotor
{
    private:
    protected:
    public:
        TMyGALILMotor(int Addr);                                                // golden: __fastcall TMyGALILMotor(int Addr);
        virtual ~TMyGALILMotor();

        virtual int     InitMotor(int IoAddress);
        virtual void    SetSpeed(unsigned int x, bool bSetJog=false);
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
void StopAllMotor(bool bIndexCanStop=true);     // SEE "STOPALLMOTOR SIGNATURE COLLISION" IN THIS HEADER'S BANNER

extern bool bGali_CardInstall;
extern bool bPauseInMotor, bPauseOutMotor;      // definitions: Motor/mymotor.cpp:95-96 (golden defines them outside myGALILmotor.cpp too)
extern bool bGalilTwoYMoveFlag;

bool ScanIndexMotorCanMove();
bool DoIndexVectorMove(const char* vecCmd);
bool ScanIndexOtherMotorCanMove(int Index);
void InitialGaliDelayCount();
void InitialSetUpGaliDelayCount(int iDelayCT);
bool CheckIndexArmPosInRange(int iNowPos, int iMin, int iMax);
AnsiString GetGalilErrString(long RC, AnsiString FunctionName="");

#endif
