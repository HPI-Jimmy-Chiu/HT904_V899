// =============================================================================
//  Motor/mySimMotor.h  -- TMySimMotor : offline simulation adapter
//
//  NEW file (additive sibling to HTMotor.h / mymotor.h).
//  Translation wave: W4 (HAL motor layer / HARDWARE INTERFACE-CUT)
//  Author: AI(W4) 20260626
//
//  Purpose:
//    TMySimMotor is a concrete subclass of HTMotor that requires NO vendor SDK
//    and NO real hardware.  It models a motor as a single integer position
//    that moves instantaneously.  MotionDone() always returns true; alarms are
//    always false.  This lets the motor layer (TMyMotor) compile and run fully
//    offline so that W4 integration tests exercise real polymorphic dispatch
//    end-to-end without a physical machine.
//
//    In cinitial (translated in a later wave), the brand-dispatch default can
//    be `new TMySimMotor()` when no HAVE_xxx vendor macro is defined.
//
//  Design notes:
//    - iSimPos tracks the simulated command position (int, matches ReadPos
//      contract).
//    - ReadEnCoderRealPos returns iSimPos so the concrete base
//      ReadEncoderPos() = iSimPos * GearRatio also works.
//    - SetCommand/SetPosition also update iSimPos so PCIL132_SetPos is stable.
//    - ScanMotorStatus fills the Led[] array consistently with "healthy,
//      in-position, no alarm" using the enum from HTMotor.h.
//    - JogP/JogN step iSimPos by JOG_DELTA pulses; callers can observe the
//      change via ReadPos() without needing MotionDone polling.
//    - All speed/accel/limit setters are no-ops or store the value (the base
//      class already stores dAcc/dDec etc. via the virtual defaults; where the
//      sim adds storage it is documented inline).
// =============================================================================
#ifndef mySimMotorH
#define mySimMotorH

#include "Motor/HTMotor.h"

// Number of pulses per JogP/JogN step in sim mode
#define MYSIM_JOG_DELTA  100

class TMySimMotor : public HTMotor
{
private:
    int iSimPos;        // simulated command + encoder position (pulses)

public:
    TMySimMotor();
    virtual ~TMySimMotor();

    // -----------------------------------------------------------------------
    //  Motion
    // -----------------------------------------------------------------------
    virtual int     ReadPos()                           ;
    virtual bool    MoveToPos(int Tar)                  ;
    // MoveToPosShortDistance / MoveToPosShortDisSlowSP inherit base (call MoveToPos)

    virtual bool    MotionDone()                        ;
    virtual bool    HomeObject()                        ;
    virtual bool    HomeFlag(void)                      ;
    virtual bool    GetAlarm(void)                      ;
    virtual void    Stop()                              ;
    virtual void    DecStop(void)                       ;
    virtual bool    JogP()                              ;
    virtual bool    JogN()                              ;

    // -----------------------------------------------------------------------
    //  Position / encoder
    // -----------------------------------------------------------------------
    virtual int     ReadRealPos()                       ;
    virtual int     ReadEnCoderRealPos()                ;
    virtual int     SetCommand(int p)                   ;
    virtual int     SetPosition(int p)                  ;
    virtual bool    ResetPos(int Pulse = 0)             ;

    // -----------------------------------------------------------------------
    //  Servo / init
    // -----------------------------------------------------------------------
    virtual int     InitMotor(int IoAddress)            ;
    virtual void    ServerOnOff(bool bStatus)           {}
    virtual void    SetServoOn(bool IsOn)               {}
    virtual void    SetServoAlarmOn(bool Value)         {}

    // -----------------------------------------------------------------------
    //  Speed / accel / limit -- no-ops; base stores dAcc/dDec via virtuals
    // -----------------------------------------------------------------------
    virtual void    SetSpeed(unsigned int x, bool bSetJog = false) { iSpeed = x; }
    virtual void    SetInitSpeed(unsigned int x)        { InitSpeed = x; }
    virtual void    SetRange(unsigned int a)            { Range = a; }
    virtual void    SetRate(unsigned int a)             { Rate = a; }
    virtual void    SoftLimitEnable(bool bFlag)         {}
    virtual void    SetSoftLimit(int iPLimit, int iNLimit)
    {
        PSoftLimitP = iPLimit;
        PSoftLimitN = iNLimit;
    }

    // -----------------------------------------------------------------------
    //  Status scan -- fills Led[] for "healthy in-position, no alarm"
    // -----------------------------------------------------------------------
    virtual void    ScanMotorStatus(bool *Led)          ;
};

#endif // mySimMotorH
