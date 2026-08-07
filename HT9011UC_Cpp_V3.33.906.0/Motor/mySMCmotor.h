// =============================================================================
//  Motor/mySMCmotor.h  -- TMySMCMotor : Contec/SMC(WDM) servo/step motor driver
//
//  Faithful translation of golden Motor/mySMCmotor.h (75 lines, BCB6,
//  Big5/cp950 -- verified: 10 non-ASCII bytes in the whole file, all inside
//  one trailing Chinese comment; decoded via cp950, preserved as UTF-8).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (motor group).
//
//  ROLE: concrete HTMotor subclass wrapping the vendor CSmc.h "SmcW*" API
//  (API-SMC(WDM), a Contec-style servo/step/YASKAWA motion card family).
//  Every virtual method overridden here mirrors an HTMotor.h virtual 1:1 by
//  name/signature (see that header); the 6 private helpers
//  (Open_SMCCard/Close_SMCCard/SMCSoftLimitEnable/SMCMotHome/
//  SetEncodeMultiple/SetServoAlarm/SetSMCInType) are internal to this driver
//  only, called from within its own .cpp, never from outside.
//
//  WAVE SCOPE -- this header carries NO function bodies (declarations only);
//  all bodies live in mySMCmotor.cpp (see that file own banner for the
//  golden-line inventory of all 34 member functions -- 2 ctor/dtor + 6
//  private + 26 public virtual overrides).
//
//  GATE REGISTER -- NONE.  This header only declares the class; it makes no
//  calls of its own.
//
//  VCL/Borland conversions:
//    - `__fastcall` KEPT on the constructor declaration (golden :26).  Per
//      this tree's own fastcall audit (vclcompat/vcl_compat.h, AI(W906-
//      FastcallFix) 20260720), __fastcall is a REAL i686 MinGW GCC calling-
//      convention attribute, not neutralized to a no-op -- every
//      declaration/definition pair must keep __fastcall in lockstep or the
//      two ends mangle to different symbols (silent link failure, or worse,
//      an ABI mismatch that only an audit catches).  This header keeps it
//      exactly where golden put it so the ctor definition in mySMCmotor.cpp
//      (which also keeps golden's `__fastcall` on the out-of-line
//      definition) matches.
//    - `char cDeviceName[16]` / `short Id` / `int iSMCHomeObjectTask` /
//      `int OldSpeed` kept EXACTLY as golden -- private data members, no
//      VCL types among them.
//    - Base class HTMotor (Motor/HTMotor.h, already ported W4) supplies the
//      byte/BYTE/bool typedefs transitively via its own windows.h include; no
//      extra include needed here beyond golden CSmc.h.
//    - The commented-out golden method (golden :63,
//      `//        virtual void    SetEnableLatch(bool a);`) is preserved
//      verbatim as a dead comment -- NOT activated, matching golden exactly
//      (HTMotor.h DOES declare SetEnableLatch as a virtual with a default
//      empty body; TMySMCMotor simply never overrides it, same as golden).
//    - The inline stub overload `GetLatchBuffer(int, unsigned short*, long*)
//      {return 0;}` (golden :70) is kept inline exactly as golden wrote it --
//      NOT split into a .cpp definition, since golden itself defines it
//      in-header.
//
//  Big5: the one non-ASCII comment (golden :72, ChungHung 20130625 note on
//  SetFIFOLatchSrc) decoded via cp950 and reproduced as UTF-8 below. Final
//  gate: ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef mySMCmotorH
#define mySMCmotorH

#include "HTMotor.h"
#include "CSmc.h"

//---------------------------------------------------------------------------
class TMySMCMotor:public HTMotor
{
    private:
        char  cDeviceName[16];
        short Id ;
        int   iSMCHomeObjectTask;
        int   OldSpeed;
        bool  Open_SMCCard();
        void  Close_SMCCard();
        void  SMCSoftLimitEnable(bool bFlag);
        bool  SMCMotHome();
        void  SetEncodeMultiple(int iMultiple);
        void  SetServoAlarm();
        void  SetSMCInType();
    protected:
    public:
        __fastcall TMySMCMotor(int Addr);
        ~TMySMCMotor();
        virtual int     InitMotor(int IoAddress);
        virtual void    SetSpeed(unsigned int x, bool bSetJog=false);           //RogerYang 20250729 Add for 9046AU
        virtual void    SetSpeedShortDistance(unsigned int x);
        virtual void    SetSpeedShortDisSlowSP(unsigned int x);
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
        virtual void    MotInputStatus(bool *bInputPort);
        virtual bool    LinearAxisMoveTo(int iPortID[8],long lPos[8],bool bFlag);
        virtual void    EnableTrigger(bool bFlag,int iMode,long lValue);
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
        virtual bool    MoveToPosShortDistance(int Tar);
        virtual bool    MoveToPosShortDisSlowSP(int Tar);
        virtual void    ResetLatch();
        virtual int     GetLatchTotalLen();
        virtual int     GetLatchBuffer(int LatchInputNo, short *AxisCounterNo, long *LatchDataTable);
        virtual int     GetLatchBuffer(int LatchInputNo, unsigned short *AxisCounterNo, long *LatchDataTable) {return 0;};
        virtual bool    GetLatchIOStatus(unsigned short LatchInputNo);
        virtual void    SetFIFOLatchSrc(int AxisCounterNo, int LatchInputNo, int bEnable); //ChungHung 20130625 add 跟原本Motor Enable衝突   Enable-->bEnable
};
#endif
