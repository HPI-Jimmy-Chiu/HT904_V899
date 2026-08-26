#include "MachineDefine.h"
#pragma hdrstop

#include "HTMotor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-------------
//MotorType
//-------------
    const int Step_Motor  =0;
    const int Servo_Motor =1;
    const int Rotate_Motor=2;           //Steven 20170425 (wei) : Add rotate motor
    const int YASKAWA_Servo_Motor=3;    //Steven 20181111 : Yaskawa馬達
    const int YASKAWA_Liner_Motor=4;
    const int Step_Motor_Oriental=5;    //RogerYang 20250904 Add Loader Y

//---------------------------------------------------------------------------
HTMotor::HTMotor()
{
    Address         = 0x340   ;
    GearRatio       = 1       ;
    PHomeHighSpeed  = 100     ;
    PHomeLowSpeed   = 100     ;
    PJogHighSpeed   = 100     ;
    PJogLowSpeed    = 100     ;
    Rate            = 100     ;
    PSoftLimitN     = -999999 ;
    PSoftLimitP     = 999999  ;
    Range           = 20      ;
    InitSpeed       = 10      ;
    Enable          = false   ;
    PServoAlarmOn   = false   ;
    Direction       = true    ;
    HomeDirection   = true    ;
    MotorType       = 1       ;
    bSensorType     = true    ;
    bLimitLogic     = true    ;
    bIn1Logic       = true    ;
    bAxisID         = 0x0     ;//Jimmychiu 20220926 : ADD MC88X1
//    HomeDirection   = true    ;

    MotorIdleSafeDoorCheck = NULL;      // 2015.01.15 , Joye , Safe door check
}
//---------------------------------------------------------------------------
HTMotor::~HTMotor()
{
}
//---------------------------------------------------------------------------
unsigned int HTMotor::ReadSpeed()
{
    return iSpeed;
}
//---------------------------------------------------------------------------
unsigned int HTMotor::ReadRange()
{
    return Range;
}
//---------------------------------------------------------------------------
unsigned int HTMotor::ReadRate()
{
    return Rate;
}
//---------------------------------------------------------------------------
int HTMotor::ReadEncoderPos()
{
    return ReadEnCoderRealPos()*GearRatio;
}
//---------------------------------------------------------------------------
unsigned int HTMotor::ReadInitSpeed()
{
    return InitSpeed;
}
//---------------------------------------------------------------------------
double HTMotor::ReadAcc()
{
    return dAccDataBase;
}
//---------------------------------------------------------------------------
double HTMotor::ReadDec()
{
    return dDecDataBase;
}
//---------------------------------------------------------------------------
void HTMotor::SetHomeobjectTask(int Task)
{
    iHomeObjectTask=Task;
}
//******************************************************************************
//  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
//******************************************************************************
bool HTMotor::CheckIsSafeDoorOpen()         //Jimmychiu 20221013 safedoor判斷整合Function
{
    if(MotorIdleSafeDoorCheck!=NULL)        // 2015.01.15 , Joye , Safe door check
    {
        if(MotorIdleSafeDoorCheck()==true)
            return true;
        else
            return false;
    }
    else
    {
        if(Enable==true)
            return true;
        else
            return false;
    }
}
//---------------------------------------------------------------------------

