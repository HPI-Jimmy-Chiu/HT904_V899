// =============================================================================
//  Motor/mySYNTEKmotor.h  -- TMySYNTEKMotor : Hontech MotionNet SYNTEK
//                            (M204/M104) servo/step motor driver
//
//  Faithful translation of golden Motor/mySYNTEKmotor.h (66 lines, BCB6,
//  Big5/cp950 -- verified: 10 non-ASCII bytes in the whole file, all inside
//  one trailing Chinese comment; decoded via cp950, preserved as UTF-8).
//  Translator: AI(W906-PT-W3) 20260807
//  Translation wave: W906-PT-W3 (motor group, motor_syntek_mn200).
//
//  ROLE: concrete HTMotor subclass wrapping the Hontech_M4.h "_Hon_m4_*" API
//  (already ported W906-PT-W2, this tree Motor/Hontech_M4.h/.cpp), which
//  itself fans out to the vendor MotionNet DLL for either a SYNTEK-M204 or a
//  plain M104 slave.  Every virtual method overridden here mirrors an
//  HTMotor.h virtual 1:1 by name/signature (see that header); the 3 private
//  helpers (SYNTEKSoftLimitEnable/SYNTEKMotHome/SYNTEKOpenCard) are internal
//  to this driver only, called from within its own .cpp, never from outside.
//
//  WAVE SCOPE -- this header carries NO function bodies (declarations only,
//  matching golden byte-for-byte structurally, except the one inline stub
//  golden itself wrote in-header, see below); all other bodies live in
//  mySYNTEKmotor.cpp (see that file's own banner for the golden-line
//  inventory of all 39 member functions -- 2 ctor/dtor + 3 private +
//  34 public virtual overrides).
//
//  GATE REGISTER -- NONE.  This header only declares the class; it makes no
//  calls of its own.
//
//  VCL/Borland conversions:
//    - `__fastcall` KEPT on the constructor declaration (golden :23), same
//      precedent as Motor/mySMCmotor.h -- __fastcall is a REAL i686 MinGW
//      GCC calling-convention attribute in this tree (vclcompat/vcl_compat.h,
//      AI(W906-FastcallFix) 20260720), not a no-op, so declaration and
//      out-of-line definition (mySYNTEKmotor.cpp) must agree.
//    - `char cDeviceName[16]` / `short Id` / `int iSYNTEKHomeObjectTask` /
//      `int OldSpeed` / `TQPF_Timer HomeDelay` kept EXACTLY as golden --
//      private data members, TQPF_Timer already ported (root myTimer.h/.cpp).
//    - Golden's commented-out `//#include "HTimer.h"` (golden :7) is
//      preserved verbatim as a dead comment for provenance -- it was already
//      commented out in golden (superseded by "myTimer.h" on the next line),
//      this translation changes nothing about it.
//    - The commented-out golden method (golden :60,
//      `//        virtual void    SetEnableLatch(bool a);`) is preserved
//      verbatim as a dead comment -- NOT activated, matching golden exactly
//      (HTMotor.h DOES declare SetEnableLatch as a virtual with a default
//      empty body; TMySYNTEKMotor simply never overrides it, same as golden).
//    - The inline stub overload `GetLatchBuffer(int, short*, long*) {return
//      0;}` (golden :63) is kept inline exactly as golden wrote it -- NOT
//      split into a .cpp definition, since golden itself defines it
//      in-header (mirrors the mySMCmotor.h precedent for its own analogous
//      short*/unsigned-short* pair, just with the two overloads' real-vs-stub
//      roles swapped -- SYNTEK's real body takes `unsigned short*`, SMC's
//      real body takes `short*`; each golden driver stubs the OTHER overload,
//      preserved verbatim per-driver, not normalized to match).
//    - Base class HTMotor (Motor/HTMotor.h, already ported W4) supplies the
//      byte/BYTE/bool typedefs transitively via its own windows.h include; no
//      extra include needed here beyond golden HTMotor.h and myTimer.h.
//
//  Big5: the one non-ASCII comment (golden :63, ChungHung 20130625 note on
//  SetFIFOLatchSrc -- identical wording to the mySMCmotor.h precedent)
//  decoded via cp950 and reproduced as UTF-8 below. Final gate: ZERO U+FFFD.
// =============================================================================
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
        virtual void    SetFIFOLatchSrc(int AxisCounterNo, int LatchInputNo, int bEnable); //ChungHung 20130625 add 跟原本Motor Enable衝突   Enable-->bEnable
};
#endif
