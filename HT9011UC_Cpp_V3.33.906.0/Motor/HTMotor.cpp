// =============================================================================
//  Motor/HTMotor.cpp  -- HTMotor concrete (non-virtual) implementations
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/Motor/HTMotor.cpp
//  Translation wave: W4 (HAL motor layer)
//  Translator: AI(W4) 20260626
//
//  Changes vs. BCB6 original:
//    - Dropped `#pragma hdrstop` / `#pragma package(smart_init)` (BCB-specific).
//    - `#include "MachineDefine.h"` -> same path (already de-VCL'd in target).
//    - All ctor defaults, dtor, concrete accessors translated faithfully.
//    - CheckIsSafeDoorOpen: translated verbatim (NOTE comment preserved).
// =============================================================================
#include "MachineDefine.h"
#include "Motor/HTMotor.h"

// ---------------------------------------------------------------------------
//  MotorType constants  (extern declarations in HTMotor.h)
// ---------------------------------------------------------------------------
const int Step_Motor           = 0;
const int Servo_Motor          = 1;
const int Rotate_Motor         = 2;     // Steven 20170425: Add rotate motor
const int YASKAWA_Servo_Motor  = 3;     // Steven 20181111: Yaskawa servo
const int YASKAWA_Liner_Motor  = 4;
const int Step_Motor_Oriental  = 5;     // RogerYang 20250904: Add Loader Y

// ---------------------------------------------------------------------------
HTMotor::HTMotor()
{
    Address         = 0x340;
    GearRatio       = 1;
    PHomeHighSpeed  = 100;
    PHomeLowSpeed   = 100;
    PJogHighSpeed   = 100;
    PJogLowSpeed    = 100;
    Rate            = 100;
    PSoftLimitN     = -999999;
    PSoftLimitP     =  999999;
    Range           = 20;
    InitSpeed       = 10;
    Enable          = false;
    PServoAlarmOn   = false;
    Direction       = true;
    HomeDirection   = true;
    MotorType       = 1;
    bSensorType     = true;
    bLimitLogic     = true;
    bIn1Logic       = true;
    bAxisID         = 0x0;          // Jimmychiu 20220926: ADD MC88X1

    MotorIdleSafeDoorCheck = NULL;  // 2015.01.15, Joye: Safe door check
}

// ---------------------------------------------------------------------------
HTMotor::~HTMotor()
{
}

// ---------------------------------------------------------------------------
unsigned int HTMotor::ReadSpeed()
{
    return iSpeed;
}

// ---------------------------------------------------------------------------
unsigned int HTMotor::ReadRange()
{
    return Range;
}

// ---------------------------------------------------------------------------
unsigned int HTMotor::ReadRate()
{
    return Rate;
}

// ---------------------------------------------------------------------------
int HTMotor::ReadEncoderPos()
{
    return (int)(ReadEnCoderRealPos() * GearRatio);
}

// ---------------------------------------------------------------------------
unsigned int HTMotor::ReadInitSpeed()
{
    return InitSpeed;
}

// ---------------------------------------------------------------------------
double HTMotor::ReadAcc()
{
    return dAccDataBase;
}

// ---------------------------------------------------------------------------
double HTMotor::ReadDec()
{
    return dDecDataBase;
}

// ---------------------------------------------------------------------------
void HTMotor::SetHomeobjectTask(int Task)
{
    iHomeObjectTask = Task;
}

// ****************************************************************************
//  NOTE: CheckIsSafeDoorOpen is Handler safety-critical.
//        Read carefully before modifying.
// ****************************************************************************
bool HTMotor::CheckIsSafeDoorOpen()     // Jimmychiu 20221013: safedoor check
{
    if (MotorIdleSafeDoorCheck != NULL) // 2015.01.15, Joye: Safe door check
    {
        if (MotorIdleSafeDoorCheck() == true)
            return true;
        else
            return false;
    }
    else
    {
        if (Enable == true)
            return true;
        else
            return false;
    }
}
