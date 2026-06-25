// =============================================================================
//  Motor/mySimMotor.cpp  -- TMySimMotor implementation
//
//  NEW file (additive sibling).
//  Translation wave: W4 (HAL motor layer / HARDWARE INTERFACE-CUT)
//  Author: AI(W4) 20260626
//
//  See mySimMotor.h for design rationale.
// =============================================================================
#include "MachineDefine.h"
#include "Motor/mySimMotor.h"

// ---------------------------------------------------------------------------
TMySimMotor::TMySimMotor()
    : iSimPos(0)
{
    Enable = true;  // sim motor is always "enabled" after construction
}

// ---------------------------------------------------------------------------
TMySimMotor::~TMySimMotor()
{
}

// ---------------------------------------------------------------------------
//  InitMotor -- sim: mark enabled, reset position, return success (0)
// ---------------------------------------------------------------------------
int TMySimMotor::InitMotor(int /*IoAddress*/)
{
    Enable   = true;
    iSimPos  = 0;
    return 0;
}

// ---------------------------------------------------------------------------
//  ReadPos -- return current simulated command position
// ---------------------------------------------------------------------------
int TMySimMotor::ReadPos()
{
    return iSimPos;
}

// ---------------------------------------------------------------------------
//  ReadRealPos / ReadEnCoderRealPos -- sim: encoder = command position
// ---------------------------------------------------------------------------
int TMySimMotor::ReadRealPos()
{
    return iSimPos;
}

int TMySimMotor::ReadEnCoderRealPos()
{
    return iSimPos;
    // NOTE: concrete base ReadEncoderPos() returns ReadEnCoderRealPos()*GearRatio,
    //       so the GearRatio scaling is preserved for free.
}

// ---------------------------------------------------------------------------
//  MoveToPos -- sim: instantaneous; sets position and returns true
// ---------------------------------------------------------------------------
bool TMySimMotor::MoveToPos(int Tar)
{
    iSimPos = Tar;
    return true;
}

// ---------------------------------------------------------------------------
//  MotionDone -- sim: motion is always complete (instantaneous model)
// ---------------------------------------------------------------------------
bool TMySimMotor::MotionDone()
{
    return true;
}

// ---------------------------------------------------------------------------
//  HomeObject -- sim: move to 0, return true (home found)
// ---------------------------------------------------------------------------
bool TMySimMotor::HomeObject()
{
    iSimPos = 0;
    return true;
}

// ---------------------------------------------------------------------------
//  HomeFlag -- sim: home is always detected
// ---------------------------------------------------------------------------
bool TMySimMotor::HomeFlag(void)
{
    return true;
}

// ---------------------------------------------------------------------------
//  GetAlarm -- sim: no alarms
// ---------------------------------------------------------------------------
bool TMySimMotor::GetAlarm(void)
{
    return false;
}

// ---------------------------------------------------------------------------
//  Stop / DecStop -- sim: motion is instantaneous, nothing to stop
// ---------------------------------------------------------------------------
void TMySimMotor::Stop()
{
    // no-op: sim motor has no inertia
}

void TMySimMotor::DecStop(void)
{
    // no-op
}

// ---------------------------------------------------------------------------
//  JogP / JogN -- step iSimPos by MYSIM_JOG_DELTA in each direction
// ---------------------------------------------------------------------------
bool TMySimMotor::JogP()
{
    iSimPos += MYSIM_JOG_DELTA;
    return true;
}

bool TMySimMotor::JogN()
{
    iSimPos -= MYSIM_JOG_DELTA;
    return true;
}

// ---------------------------------------------------------------------------
//  SetCommand / SetPosition -- sim: directly set the position register
// ---------------------------------------------------------------------------
int TMySimMotor::SetCommand(int p)
{
    iSimPos = p;
    return 0;
}

int TMySimMotor::SetPosition(int p)
{
    iSimPos = p;
    return 0;
}

// ---------------------------------------------------------------------------
//  ResetPos -- sim: reset position to Pulse (default 0)
// ---------------------------------------------------------------------------
bool TMySimMotor::ResetPos(int Pulse)
{
    iSimPos = Pulse;
    return true;
}

// ---------------------------------------------------------------------------
//  ScanMotorStatus -- fill Led[] for healthy / in-position / no-alarm state
//
//  Led array layout (from HTMotor.h enum):
//    iCwLed=0, iHomeLed=1, iCcwLed=2, iEmgLed=3, iAlarmLed=4,
//    iSoftcwLed=5, iSoftccwLed=6, iServoalarmLed=7, iInposLed=8, iServoOn=9
//
//  Sim: servo is on, in-position, no limits, no alarms.
//  Home (iHomeLed) is set to false -- it fires only when physically at home;
//  callers that test Led[iHomeLed] in MotorHome() will see the correct
//  "not-on-home-sensor" state after MoveToPos(non-zero).  After HomeObject()
//  returns (iSimPos=0) the caller's MotorHome state machine uses HomeFlag()
//  not Led[iHomeLed], so this is safe.
// ---------------------------------------------------------------------------
void TMySimMotor::ScanMotorStatus(bool *Led)
{
    if (Led == NULL)
        return;

    Led[iCwLed]         = false;    // CW  limit  -- not triggered
    Led[iHomeLed]       = false;    // Home sensor -- not triggered (see note above)
    Led[iCcwLed]        = false;    // CCW limit  -- not triggered
    Led[iEmgLed]        = false;    // Emergency stop -- inactive
    Led[iAlarmLed]      = false;    // Alarm        -- none
    Led[iSoftcwLed]     = false;    // Soft CW  limit -- not triggered
    Led[iSoftccwLed]    = false;    // Soft CCW limit -- not triggered
    Led[iServoalarmLed] = false;    // Servo alarm  -- none
    Led[iInposLed]      = true;     // In-position  -- always settled in sim
    Led[iServoOn]       = true;     // Servo on     -- always on in sim
}
