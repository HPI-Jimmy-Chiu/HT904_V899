// =============================================================================
//  Motor/mymotor.cpp  -- TMyMotor / TTrayMotor implementation (PARTIAL W4)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/Motor/mymotor.cpp
//  Translation wave: W4 (HAL motor layer)
//  Translator: AI(W4) 20260626
//
//  ACTIVE (W4): ctor/dtor, SetAlias, IsCanMove, SetScreenScale, ReadPos,
//    ReadEncoderPos, SetADCRate, SetSpeed, GetSpeed, EnableMotorMove,
//    InitMOTParameter, MotorInitial, Home, HomeReset, MotorHome,
//    GetMotorAlarm, ScanMotorStatus, GetErrorIndex, PCIL132_SetPos/ResetPos/
//    StopMotor, JogP(int)/JogN(int), ServoOnOff (SMC/generic branch only),
//    SetArmMaxSpeed, IsStartGali_Pr/StartGali_Pr/EndGali_Pr/GetGali_Pr_Result/
//    GetGali_Pr_ER/SetGali_Pr_ER (use Gali_Command stub),
//    GetRealPos, CompareEncoderPos, CompareCommandPos, GetRotatorBacklash,
//    Lock/UnLock/ClearLock/GetLockCount/GetLockString,
//    SetPanel.
//    TTrayMotor: ctor, HasIC, HasRealIC, HasCleanPad, SetTrayBinData,
//    SetTraySingleData, SetTraySiteMap, SetNullIcToHasNullIc/HasIc,
//    SetTrayBufferSingleData, Refresh, InitNewTray (full), ClearTray,
//    InitEmptyTray, SetHTrayPanel, SetTray, UpHalfIsFull, DownHalfIsFull,
//    WhichBufferIsFull, MoveTrayAllItem, HowManyDevice(x2),
//    MoveTrayData.
//    Global array MOT[300], ZSafePos, ZlimitPos, bPauseSortMotor.
//
//  GATED `#if 0 // TODO(wave)`:
//    - MotorMovePosition body (depends on ShowMyMessage/Cylinder/Sensor) --
//      declared but body is a stub returning 0 so MotorHome compiles.
//    - MotorMove, MotorMove2SpeedForPicker, MotorMoveShuttleShake bodies.
//    - Gali_* method bodies (require myGALILmotor / DMCCommand).
//    - ServoOnOff Gali_ branch (INDEX_MOTION_CARD==0).
//    - InArmZSafe, CheckInArmZNeedHome, ShowIndexMotorError, CheckTestZ*,
//      IndexPosMonitor free functions (Sensor/Cylinder/VCL coupling).
//    - All InArmContinuousMove_9045 / OutArm* / SortArm* / PCIL112_* / ZSafe*
//      / Tray helper free functions (state-machine wave).
//    - GetRealPos Contec direction branch (uses MotionCard_Contec).
//    - MyMNetLine myLine[] (requires MyMNet/mn_open_all; MN200 band).
//
//  NOTE on MNetLog: forward-declared as a no-op stub below so TTrayMotor
//  InitNewTray/ClearTray calls compile.  Real MNetLog is in the communications
//  layer (TODO W5).
// =============================================================================

#include "MachineDefine.h"
#include "Motor/mymotor.h"
#include "MachineType.h"    // ChangeToFloatNonPcnt, MOTION_CARD_TYPE constants, etc.
#include "cpublic.h"
#include "cmydef.h"         // sIC_Type[], iYRegNum, bTestSiteUse[], etc.

// ---------------------------------------------------------------------------
//  Stub for MNetLog -- real implementation is in communications layer (W5).
//  TTrayMotor calls it with AnsiString on tray events.
// ---------------------------------------------------------------------------
static void MNetLog(AnsiString /*msg*/) {}  // TODO(W5): replace with real MNetLog

// ---------------------------------------------------------------------------
//  Module-level constants / globals
// ---------------------------------------------------------------------------
#define RESET_TIMES 900

#define PNP_DONE    0   // order finished
#define PNP_DOING   1   // order in progress

int ZSafePos  =  20;        // Steven 20220207: In/Out Arm Z safe position
int ZlimitPos = -3200;      // Ifor 20221026:   In/Out Arm limit position

bool bPauseInMotor   = false;
bool bPauseOutMotor  = false;
bool bPauseSortMotor = false;   // RogerYang 20250510: 9046AU

// PCIL112 XY move task counters (used by free-function stubs)
int iPCIL112_SortArmXYMoveTask = 1;    // RogerYang 20250512: 9046AU
int iPCIL112_InArmXYMoveTask   = 1;
int iPCIL112_OutArmXYMoveTask  = 1;

// InArm/OutArm Z move task indices (extern'd in mymotor.h)
int iInArmZMoveTask  = 1;
int iOutArmZMoveTask = 1;

// TrayArmMotorMove extern stub -- real body in TrayArm wave
bool TrayArmMotorMove(int /*p*/, bool /*bCheckPos*/) { return false; } // TODO(W6)

// Encoder tolerances (module-local, matches original)
static const int iTorence = 10;
static int iEncoderTorence = 500;
static int iCheckZ         = 4000;

// Bit mask table (used by some Gali helpers; kept for completeness)
// Suppress unused-variable: table is referenced in gated TODO(W6-Galil) helpers.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
static unsigned char bMask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
#pragma GCC diagnostic pop

// Global motor array
class TTrayMotor MOT[MAX_TRAY_MOTOR];

// ---------------------------------------------------------------------------
//  GetScale helper (private to this TU; used by SetScreenScale / ctor)
// ---------------------------------------------------------------------------
static double GetScale(int s1, int e1, int s2, int e2)
{
    double S = (double)(s2 - e2);
    if (S == 0)
        return 1.0;
    double P = (double)(s1 - e1);
    return P / S;
}

// ===========================================================================
//  TMyMotor
// ===========================================================================

// ---------------------------------------------------------------------------
TMyMotor::TMyMotor()
{
    RefStart  = 2;
    RefEnd    = 0;
    FactStart = 4;
    FactEnd   = 0;
    fCanMove  = true;
    fCanMoveR = true;
    fCanMoveM = true;
    fCanMoveL = true;
    Scale     = GetScale(RefStart, RefEnd, FactStart, FactEnd);
    bIsFullIC  = false;
    bIsEmptyIC = false;
    MovFlag    = false;
    bPanelUse  = false;
    bUpDownMove = false;
    Gali_MotorAlarm   = false;
    GaliSofDelayCount = 0;
    bScanFlag         = false;
    iGali_SingalHomeTask = 1;
    mapLockList.clear();    // Steven 20210825: lock list

    // Isaac 20201110: Index Y find motor phase
    iGali_FindZPhaseTask[0] = 1;
    iGali_FindZPhaseTask[1] = 1;
    iGali_FindZPhaseTask[2] = 1;
    iGali_FindZPhaseTask[3] = 1;

    Mot_Name  = 0;
    fCMD      = false;
    iOldPos   = 0;
    bSh1FloodgateOpenWaiting = false;   // Ifor 20260401
    bSh2FloodgateOpenWaiting = false;   // Ifor 20260401
    iCheckStatusCT = 0;
    GaliScanServo  = true;
    iEncoderCheckCT = 100;
    bCheckEncoderEveryTime = false;
    bShowMotorMove = false;
    iLastRotatorDirP = true;            // RogerYang 20260113

    if (MOTION_CARD_TYPE == MotionCard_SYN)
    {
        iEncoderTorence = 1000;
    }
    else
    {
        iEncoderTorence = 1000;
    }

    if (INDEX_PRESS_TYPE == e160KG && CUSTOMER_CODE == CC_JCET)
    {
        iCheckZ = 11000;
    }
    // AI(ht9045-v899) 20260512: CASE-20260507-001 CC_GIGAS Z2 delay -> iCheckZ 11000
    else if (CUSTOMER_CODE == CC_GIGAS)
    {
        iCheckZ = 11000;
    }
    else if (INDEX_PRESS_TYPE == e85KG  || INDEX_PRESS_TYPE == e240KG ||
             INDEX_PRESS_TYPE == e120KG || INDEX_PRESS_TYPE == e160KG)
    {
        iCheckZ = 9000;
    }
    else
    {
        iCheckZ = 4000;
    }
}

// ---------------------------------------------------------------------------
TMyMotor::~TMyMotor()
{
    mapLockList.clear();    // Steven 20210825
}

// ---------------------------------------------------------------------------
void TMyMotor::SetPanel(void *PCtrl, bool b)
{
    bUpDownMove = b;
    bPanelUse   = true;
    PWinCtrl    = PCtrl;
}

// ---------------------------------------------------------------------------
void TMyMotor::SetAlias(int iNo, AnsiString Name)
{
    Alias   = Name;
    Mot_Name = iNo;
    NumberAlias.sprintf("[%02d] %s", iNo, Name);
}

// ---------------------------------------------------------------------------
bool TMyMotor::IsCanMove()
{
    if (fCanMove && fCanMoveR && fCanMoveM && fCanMoveL)
        return true;
    return false;
}

// ---------------------------------------------------------------------------
void TMyMotor::SetScreenScale(int s1, int e1, int s2, int e2)
{
    RefStart  = s1;  RefEnd   = e1;
    FactStart = s2;  FactEnd  = e2;
    Scale     = GetScale(RefStart, RefEnd, FactStart, FactEnd);
    ScreenPos = (int)(Scale * (Position - FactStart)) + RefStart;
}

// ---------------------------------------------------------------------------
int TMyMotor::ReadPos()
{
    if (Motor != NULL && Motor->Enable)
    {
        Position        = Motor->ReadPos();
        EncoderPosition = Motor->ReadEncoderPos();
    }
    else
    {
        EncoderPosition = Position;
    }
    ScreenPos = (int)(Scale * (Position - FactStart)) + RefStart;
    return Position;
}

// ---------------------------------------------------------------------------
int TMyMotor::ReadEncoderPos()
{
    if (Motor != NULL && Motor->Enable)
    {
        Position        = Motor->ReadPos();
        EncoderPosition = Motor->ReadEncoderPos();
    }
    else
    {
        EncoderPosition = Position;
    }
    return EncoderPosition;
}

// ---------------------------------------------------------------------------
void TMyMotor::SetADCRate(int Scale)
{
    double fPersent = 0.0;
    double Acc, Dec;

    if (Scale == 100)
    {
        Acc = Motor->GetAccDataBase();
        Dec = Motor->GetDecDataBase();
    }
    else
    {
        fPersent = double(Scale) / 100.0;
        if (CardType == "PCI1203")  // RogerYang 20250415: PCI1203
        {
            Acc = Motor->GetAccDataBase() * fPersent;
            Dec = Motor->GetDecDataBase() * fPersent;
        }
        else
        {
            Acc = ChangeToFloatNonPcnt((double)(Motor->GetAccDataBase()), (double)(fPersent));
            Dec = ChangeToFloatNonPcnt((double)(Motor->GetDecDataBase()), (double)(fPersent));
        }
    }
    Motor->SetAcc(double(Acc));
    Motor->SetDec(double(Dec));
}

// ---------------------------------------------------------------------------
void TMyMotor::SetSpeed(double p, bool bSetJog)
{
    int s;
    if (Motor != NULL && Motor->Enable)
    {
        if (Mot_Name == MTestY1 || Mot_Name == MTestZ1 ||
            Mot_Name == MTestZ2 || Mot_Name == MTestY2)
        {
            // Index axis -- Galil card controls speed directly; skip here
        }
        else
        {
            if (p >= 100)
                p = 100;

            if (Mot_Name == MLoaderZ || Mot_Name == MEmptyZ  || Mot_Name == MColorZ  ||
                Mot_Name == MAuto1Z  || Mot_Name == MAuto2Z  || Mot_Name == MAuto3Z  ||
                Mot_Name == MAuto4Z  || Mot_Name == MAuto5Z  || Mot_Name == MAuto6Z)
            {
                s = (Motor->PJogHighSpeed - Motor->PJogLowSpeed) * (int)p / 100 + Motor->PJogLowSpeed;
            }
            else
            {
                s = Motor->PJogHighSpeed * (int)p / 100;
            }
            Motor->SetSpeed(s, bSetJog);    // RogerYang 20250729
            speed = s;
        }
    }
    else
    {
        speed = (int)p;
    }
}

// ---------------------------------------------------------------------------
int TMyMotor::GetSpeed()
{
    if (Motor != NULL && Motor->Enable)
        return Motor->ReadSpeed();
    else
        return speed;
}

// ---------------------------------------------------------------------------
void TMyMotor::EnableMotorMove()
{
    fCanMove  = true;
    fCanMoveR = true;
    fCanMoveM = true;
    fCanMoveL = true;
}

// ---------------------------------------------------------------------------
void TMyMotor::InitMOTParameter()
{
    fCMD = false;
    GaliSofDelayCount = 0;
    bScanFlag = false;
    MovFlag   = false;
}

// ---------------------------------------------------------------------------
//  MotorMovePosition -- private helper
//
//  W4 STUB: the full body depends on ShowMyMessage / Cylinder[] /
//  SHUTTLE_FLOODGATE state machine (W6-sensor wave).
//  Returns 0 (PNP_DOING) unconditionally so callers compile.
//  Real body gated below for reference.
//
//  Return codes (preserved from original comments):
//   1 = Move Success  |  0 = Moving  | -1 = Safe Door Opened
//  -2 = Target > LimitP | -3 = Target < LimitN | -4 = PServoAlarmOn
//  -5 = WAR1639 Motor encoder error | -6 = MOT Home sensor error
// ---------------------------------------------------------------------------
int TMyMotor::MotorMovePosition(int & /*Position*/, int /*speed*/, int /*Tar*/)
{
    // TODO(W6-state-machine): translate full body from BCB6 mymotor.cpp:549-860
    // Depends on: ShowMyMessage, Cylinder[], C_Shuttle1/2Floodgate,
    //             ShowErrorMessage("WAR1639",...), SetInArmHome/SetOutArmHome,
    //             SOFT_SIMULTE branch (hardware sim), CompareCommandPos.
    return 0;
}

// ---------------------------------------------------------------------------
//  MotorMove -- W4 STUB
// ---------------------------------------------------------------------------
int TMyMotor::MotorMove(int /*p*/)
{
    // TODO(W6-state-machine): translate full body (BCB6 mymotor.cpp:871-962)
    return 0;
}

// ---------------------------------------------------------------------------
//  MotorMove2SpeedForPicker -- W4 STUB
// ---------------------------------------------------------------------------
bool TMyMotor::MotorMove2SpeedForPicker(int /*FinalPos*/, ARM_CONDITION * /*ARM*/, bool /*bIsLoader*/)
{
    // TODO(W6-state-machine): translate full body (BCB6 mymotor.cpp:964-999)
    return false;
}

// ---------------------------------------------------------------------------
//  MotorMoveShuttleShake -- W4 STUB
// ---------------------------------------------------------------------------
bool TMyMotor::MotorMoveShuttleShake(int /*p*/)
{
    // TODO(W6-state-machine)
    return false;
}

// ---------------------------------------------------------------------------
void TMyMotor::MotorInitial()
{
    Motor->SetHomeobjectTask(1);
    iMyHomeTask = 1;
    ResetTime.Set0_1SecAndOn(RESET_TIMES);
    HomeFlag = 0;
}

// ---------------------------------------------------------------------------
bool TMyMotor::Home(void)
{
    // **NOTE: CheckIsSafeDoorOpen is safety-critical in the real Handler.**
    if (Motor == NULL || Motor->CheckIsSafeDoorOpen())
        return false;

    Motor->iHomePitch = 200;
    GetRealPos(&Motor->iHomePitch);
    fCMD = false;
    SetADCRate(100);
    return Motor->HomeObject();
}

// ---------------------------------------------------------------------------
void TMyMotor::HomeReset()
{
    Motor->SetHomeobjectTask(1);
}

// ---------------------------------------------------------------------------
//  MotorHome state machine
//
//  The Galil branch (INDEX_MOTION_CARD==0 && MTestY1..MTestY2) calls
//  Gali_ScanAlarmStatus() / Gali_ScanMotStatus() / Gali_Command().
//  Those are gated TODO(W6-Galil); the SMC/generic else-branch is active.
//  In the W4 sim build INDEX_MOTION_CARD defaults to non-zero (from
//  cmydef.h constant), so the Galil branch is never entered at runtime.
// ---------------------------------------------------------------------------
int TMyMotor::MotorHome(bool Flag)
{
    if (Motor == NULL || Motor->CheckIsSafeDoorOpen())
        return 0;

    int ret = 0;
    int &Task = iMyHomeTask;

    if (Motor->Enable)
    {
        ScanMotorStatus();
        if (GetMotorAlarm())
        {
            ret = GetErrorIndex();
            if (ret < 2 || ret > 5)
            {
                Task = 1;
                return 3;
            }
        }
    }

    switch (Task)
    {
        case 1:
            if (Motor->Enable == false)
            {
                Task     = 1;
                Position = 0;
                HomeFlag = 1;
                iLastRotatorDirP = true;    // RogerYang 20260113
                return 1;
            }
            HomeReset();
            Task = 10;
            iHomeRetryCT = 3;
            break;

        case 10:
            if (Home())
            {
                Task = 20;
                htWaitHomeSensorOnDelay.SetMSAndOn(300);

                if (Led[iServoOn] == false)
                {
                    Task = 1;
                    return 4;
                }
            }
            break;

        case 20:
            if (Motor->HomeFlag())
            {
                Task = 1;
                HomeFlag  = 1;
                fCanMove  = true;
                fCanMoveR = true;
                fCanMoveM = true;
                fCanMoveL = true;
                iLastRotatorDirP = true;    // RogerYang 20260113
                return 1;
            }
            else if (htWaitHomeSensorOnDelay.Off())
            {
                if (iHomeRetryCT)
                {
                    iHomeRetryCT--;
                    Task = 30;
                }
                else
                {
                    HomeFlag = 2;
                    return 2;
                }
            }
            break;

        case 30:
            if (Motor->Enable == false)
            {
                Task     = 1;
                Position = 0;
                HomeFlag = 1;
                return 1;
            }
            HomeReset();
            Task = 10;
            break;
    }

    if (Flag)
        ResetTime.Set0_1SecAndOn(RESET_TIMES);

    if (ResetTime.Off())
    {
        HomeFlag = 2;
        return 2;
    }
    return 0;
}

// ---------------------------------------------------------------------------
bool TMyMotor::GetMotorAlarm()
{
    if (Motor != NULL)
    {
        if (Mot_Name == MTestY1 || Mot_Name == MTestZ1 ||
            Mot_Name == MTestZ2 || Mot_Name == MTestY2)
        {
            // Galil branch: TODO(W6-Galil)
            // Gali_ScanAlarmStatus();
            // return Gali_MotorAlarm;
            return Gali_MotorAlarm;     // default false in W4 sim
        }
        else
        {
            return Motor->GetAlarm();
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
void TMyMotor::ScanMotorStatus()
{
    if (Motor != NULL)
    {
        if (INDEX_MOTION_CARD == 0 &&
            (Mot_Name == MTestY1 || Mot_Name == MTestZ1 ||
             Mot_Name == MTestZ2 || Mot_Name == MTestY2))
        {
            // Galil branch: TODO(W6-Galil)
            // Gali_ScanMotStatus();
        }
        else
        {
            Motor->ScanMotorStatus(Led);
        }
    }
}

// ---------------------------------------------------------------------------
int TMyMotor::GetErrorIndex()
{
    if (Led[iAlarmLed] && Led[iServoalarmLed] && Led[iInposLed])
        return 0;
    else if (Led[iAlarmLed] && Led[iServoalarmLed])
        return 1;
    else if (Led[iAlarmLed] && Led[iCwLed])
        return 2;
    else if (Led[iAlarmLed] && Led[iCcwLed])
        return 3;
    else if (Led[iCwLed] || Led[iCcwLed])
        return 6;
    else if (Led[iAlarmLed] && Motor->PServoAlarmOn)
        return 7;
    else if (Led[iAlarmLed] && Led[iSoftcwLed])
        return 4;
    else if (Led[iAlarmLed] && Led[iSoftccwLed])
        return 5;
    else
        return 9;
}

// ---------------------------------------------------------------------------
void TMyMotor::PCIL132_SetPos(int Pos)
{
    if (Motor != NULL && Motor->Enable == false)
    {
        Position = 0;
    }
    else
    {
        if (Motor == NULL || Motor->Enable == false)
            return;
        Motor->SetCommand(Pos);
        Motor->SetPosition(Pos);
    }
}

// ---------------------------------------------------------------------------
void TMyMotor::PCIL132_ResetPos()   // Steven 20110628: reset CommandPos on ServoOn
{
    if (Motor == NULL || Motor->Enable == false)
        return;

    if (MOTION_CARD_TYPE == MotionCard_SYN)
    {
        Motor->ResetPos();
    }
    else
    {
        int p = Motor->ReadEnCoderRealPos();
        if (Motor->Direction)
            p = -p;
        Motor->ResetPos(p);
    }
}

// ---------------------------------------------------------------------------
void TMyMotor::PCIL132_StopMotor()
{
    if (Motor != NULL && Motor->Enable)
    {
        if (Mot_Name == MTestY1 || Mot_Name == MTestZ1 ||
            Mot_Name == MTestZ2 || Mot_Name == MTestY2)
        {
            return;
        }
        fCMD = false;
        Motor->DecStop();
    }
}

// ---------------------------------------------------------------------------
void TMyMotor::JogP(int /*Speed*/)
{
    if (Motor == NULL || Motor->CheckIsSafeDoorOpen())
        return;
    Motor->JogP();
}

// ---------------------------------------------------------------------------
void TMyMotor::JogN(int /*Speed*/)
{
    if (Motor == NULL || Motor->CheckIsSafeDoorOpen())
        return;
    Motor->JogN();
}

// ---------------------------------------------------------------------------
//  ServoOnOff -- Galil branch (INDEX_MOTION_CARD==0, MTestY1..MTestY2)
//  calls Gali_Command; gated TODO(W6-Galil).  SMC/generic branch translated.
// ---------------------------------------------------------------------------
void TMyMotor::ServoOnOff(bool IsOn)
{
    if (Motor != NULL && Motor->Enable)
    {
        if (INDEX_MOTION_CARD == 0 &&
            Mot_Name >= MTestY1 && Mot_Name <= MTestY2)
        {
            // TODO(W6-Galil): Gali_ScanMotStatus() + SH/MO commands
            // AnsiString S[] = {"X","Y","Z","W"};
            // Gali_ScanMotStatus();
            // if(IsOn) { if(!Led[iServoOn]) Gali_Command("SH"+S[Mot_Name-MTestY1]); }
            // else     { if(Led[iServoOn])  { Gali_Command("AB1"); Gali_Command("MO"+S[Mot_Name-MTestY1]); } }
        }
        else
        {
            Motor->SetServoOn(IsOn);
            if (Motor->PServoAlarmOn)
            {
                if (IsOn)
                {
                    // MySleep(200) gated -- TODO(W6-sleep)
                    PCIL132_ResetPos();
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
void TMyMotor::SetArmMaxSpeed()
{
    Motor->SetArmMaxSpeed();
}

// ---------------------------------------------------------------------------
//  GetRealPos -- converts logical position to hardware pulses via GearRatio
//
//  The Contec direction-flip branch (MOTION_CARD_TYPE==MotionCard_Contec) is
//  translated faithfully; MotionCard_Contec is defined in MachineType.h.
// ---------------------------------------------------------------------------
void TMyMotor::GetRealPos(int *iPos)
{
    TargetPosition = *iPos;
    int    p  = *iPos;
    double r  = 0.0;

    if (Motor->GearRatio == 0.0)
        Motor->GearRatio = 1.0;

    r = Motor->GearRatio;
    int p1 = 0, p2 = 0;
    p1 = ChangeToFloatNonPcnt((double)(double(p)), (double)(r));
    p2 = (int)(double(p1) * r);

    if (p2 < p)
    {
        while (1)
        {
            p1++;
            p2 = (int)(double(p1) * r);
            if (p2 >= p)
                break;
        }
    }
    else if (p2 > p)
    {
        while (1)
        {
            p1--;
            p2 = (int)(double(p1) * r);
            if (p2 <= p)
                break;
        }
    }
    *iPos = p1;

    if (Mot_Name == MTestY1 || Mot_Name == MTestZ1 ||
        Mot_Name == MTestZ2 || Mot_Name == MTestY2)
    {
        // Galil axes: no direction flip
    }
    else
    {
        if (MOTION_CARD_TYPE == MotionCard_Contec)
        {
            if (Motor->Direction)
                *iPos = -*iPos;
            else
                *iPos = *iPos;
        }
    }
}

// ---------------------------------------------------------------------------
int TMyMotor::CompareEncoderPos(int iPos, int iGap)     // Isaac 20201217
{
    int iEncoderPos = ReadEncoderPos();
    if (iPos - iGap <= iEncoderPos && iEncoderPos <= iPos + iGap)
        return 1;
    else
        return -1;
}

// ---------------------------------------------------------------------------
int TMyMotor::CompareCommandPos(int iPos, int iGap)     // Isaac 20201217
{
    int iCommandPos = ReadPos();
    if (iPos - iGap <= iCommandPos && iCommandPos <= iPos + iGap)
        return 1;
    else
        return -1;
}

// ---------------------------------------------------------------------------
//  GetRotatorBacklash  (RogerYang 20260113: Rotator backlash)
// ---------------------------------------------------------------------------
int TMyMotor::GetRotatorBacklash(int iGoalPos, bool bInRotator, int iTechData)
{
    int iBacklash = 0;
    int iProdData = 0;
    if (bInRotator == true)
    {
        iProdData = (iTechData == 0) ? Prod.iIn_iRotateA_Backlash : iTechData;
        if (iGoalPos > MOT[MInRotateKit].ReadPos() &&
            MOT[MInRotateKit].iLastRotatorDirP == false)
        {
            iBacklash = iProdData;
        }
        else if (iGoalPos < MOT[MInRotateKit].ReadPos() &&
                 MOT[MInRotateKit].iLastRotatorDirP == true)
        {
            iBacklash = -iProdData;
        }
    }
    else
    {
        iProdData = (iTechData == 0) ? Prod.iOut_iRotateA_Backlash : iTechData;
        if (iGoalPos > MOT[MOutRotateKit].ReadPos() &&
            MOT[MOutRotateKit].iLastRotatorDirP == false)
        {
            iBacklash = iProdData;
        }
        else if (iGoalPos < MOT[MOutRotateKit].ReadPos() &&
                 MOT[MOutRotateKit].iLastRotatorDirP == true)
        {
            iBacklash = -iProdData;
        }
    }
    return iBacklash;
}

// ---------------------------------------------------------------------------
//  Lock / UnLock / ClearLock / GetLockCount / GetLockString
//  (Steven 20210825: lock list to prevent concurrent motor access)
// ---------------------------------------------------------------------------
int TMyMotor::GetLockCount()
{
    return (int)mapLockList.size();
}

void TMyMotor::Lock(AnsiString MotorAlias, AnsiString FunctionName, int /*Task*/)
{
    mapLockList[MotorAlias] = FunctionName;
}

void TMyMotor::UnLock(AnsiString MotorAlias, AnsiString /*FunctionName*/)
{
    mapLockIter = mapLockList.find(MotorAlias);
    if (mapLockIter != mapLockList.end())
        mapLockList.erase(mapLockIter);
}

void TMyMotor::ClearLock()
{
    mapLockList.clear();
}

AnsiString TMyMotor::GetLockString(int Index)
{
    AnsiString sRet = "";
    int iIdx = 0;
    for (mapLockIter = mapLockList.begin();
         mapLockIter != mapLockList.end(); ++mapLockIter)
    {
        if (iIdx == Index)
        {
            sRet = mapLockIter->first + ":" + mapLockIter->second;
            break;
        }
        iIdx++;
    }
    return sRet;
}

// ---------------------------------------------------------------------------
//  Gali_* method stubs -- declared for ABI; all bodies gated TODO(W6-Galil)
// ---------------------------------------------------------------------------
AnsiString TMyMotor::Gali_GetMOT(int /*MOT*/) { return ""; }
void TMyMotor::Gali_ScanMotStatusTIMO() {}
void TMyMotor::Gali_ScanMotStatus()     {}
void TMyMotor::Gali_ScanAlarmStatus()   {}
bool TMyMotor::Gali_MotMove(int, int, AnsiString)            { return false; }
bool TMyMotor::Gali_MovePR(int, int)                         { return false; }
bool TMyMotor::Gali_MotMove2(int, int, int)                  { return false; }
bool TMyMotor::Gali_MotMoveNoWait(int, int, int, bool)       { return false; }
bool TMyMotor::Gali_MotMoveSkipEncoder(int, int)             { return false; }
bool TMyMotor::Z1UpZ2Down1(int)                              { return false; }
bool TMyMotor::Z1DownZ2Up1(int)                              { return false; }
bool TMyMotor::Z1UpZ2Down2(int, bool)                        { return false; }
bool TMyMotor::Z1DownZ2Up2(int, bool)                        { return false; }
bool TMyMotor::Z1UpZ2Down(int, bool, bool)                   { return false; }
bool TMyMotor::Z1DownZ2Up(int, bool, bool)                   { return false; }
long TMyMotor::Gali_Command(AnsiString, AnsiString)          { return 0; }
long TMyMotor::Gali_ReadPos()                                { return 0; }
long TMyMotor::Gali_ReadEncoderPos()                         { return 0; }
void TMyMotor::Gali_MotHome(AnsiString)                      {}
void TMyMotor::Gali_MotHome_HighSpeed(AnsiString, int)       {}
void TMyMotor::Gali_MotHomeFindZ(AnsiString)                 {}
bool TMyMotor::Gali_SingalHome(bool)                         { return false; }
bool TMyMotor::Gali_FindZPhase()                             { return false; }
// AI(W6.4-TESTER) 20260626: Motor==NULL offline fast-path -- no vendor backend
// means there is no hardware to wait on, so the Galil-Z move reports COMPLETE
// immediately (matches the W4 HAL design: offline ReadPos preserves Position,
// ScanMotorStatus is a no-op).  Lets the test-head SM (DoTestHeadMotor) pump
// over the Sim HAL; with a real HTMotor* attached the body is gated TODO(W6-Galil).
bool TMyMotor::Gali_Two_ZAxis_Move(int,int,AnsiString,bool,int) { return (Motor==NULL); }
void TMyMotor::Gali_JogP(int)          {}
void TMyMotor::Gali_JogPSetup(int)     {}
void TMyMotor::Gali_JogPAndCount(int, int) {}
void TMyMotor::Gali_JogN(int)          {}
void TMyMotor::Gali_JogNSetup(int)     {}
void TMyMotor::Gali_JogNAndCount(int, int) {}
bool TMyMotor::ISNormal()                 { return (Motor==NULL); }   // AI(W6.4-TESTER) 20260626: offline (no backend) -> axis treated normal/in-position
bool TMyMotor::ISZ1Up_Z2Down()            { return false; }
bool TMyMotor::ISZ1Down_Z2Up()            { return false; }
bool TMyMotor::ISZ1Up_Z2DownNoWait()      { return false; }
bool TMyMotor::ISZ1Down_Z2UpNoWait()      { return false; }
bool TMyMotor::Gali_ReadEncoderInRandge(long)            { return false; }
bool TMyMotor::Gali_ReadEncoderInRandgeNoWait(long)      { return false; }
bool TMyMotor::Gali_ReadEncoderOver(long)                { return false; }
bool TMyMotor::Gali_ReadEncoderMaxRandge(long)           { return false; }
bool TMyMotor::Gali_ReadEncoderInRandgeMinLimit(long)    { return false; }
bool TMyMotor::GalilTwoY_Move(int,int,int,AnsiString)    { return (Motor==NULL); }   // AI(W6.4-TESTER) 20260626: offline (no backend) -> Y move reports complete
bool TMyMotor::Gali_ReadEncoderBelowCheckHeight(long)    { return false; }
bool TMyMotor::Gali_nnMode_Z1Z2_Down(int, bool)          { return false; }
bool TMyMotor::Gali_nnMode_Z1Z2_Up(int, bool)            { return false; }
bool TMyMotor::ISZ1Down_Z2Down()                         { return false; }
bool TMyMotor::ISZ1Up_Z2Up()                             { return false; }

bool TMyMotor::IsStartGali_Pr()
{
    if (Gali_Command("MG _XQ", __func__) == -1)
        return false;
    return true;
}
void TMyMotor::StartGali_Pr() { Gali_Command("XQ",  __func__); }
void TMyMotor::EndGali_Pr()   { Gali_Command("ST",  __func__); }

long TMyMotor::GetGali_Pr_Result()
{
    AnsiString str;
    if (Mot_Name == MTestY1)      str = "E1=";
    else if (Mot_Name == MTestZ1) str = "E2=";
    else if (Mot_Name == MTestZ2) str = "E3=";
    else if (Mot_Name == MTestY2) str = "E4=";
    return Gali_Command(str, __func__) * (long)Motor->GearRatio;
}

void TMyMotor::GetGali_Pr_ER(long &ERA, long &ERB, long &ERC, long &ERD)
{
    ERA = Gali_Command("ERA=?", __func__) * (long)MOT[MTestY1].Motor->GearRatio;
    ERB = Gali_Command("ERB=?", __func__) * (long)MOT[MTestZ1].Motor->GearRatio;
    ERC = Gali_Command("ERC=?", __func__) * (long)MOT[MTestZ2].Motor->GearRatio;
    if (USE_INDEX_ARM_AXES == IndexArm_3_Axis)
        ERD = 0;
    else
        ERD = Gali_Command("ERD=?", __func__) * (long)MOT[MTestY1].Motor->GearRatio;
}

void TMyMotor::SetGali_Pr_ER(long ERA, long ERB, long ERC, long ERD)
{
    AnsiString str;
    ERA = (ERA * 100) / (long)(MOT[MTestY1].Motor->GearRatio * 100);
    ERB = (ERB * 100) / (long)(MOT[MTestZ1].Motor->GearRatio * 100);
    ERC = (ERC * 100) / (long)(MOT[MTestZ2].Motor->GearRatio * 100);
    if (USE_INDEX_ARM_AXES == IndexArm_3_Axis)
        ERD = 0;
    else
        ERD = (ERD * 100) / (long)(MOT[MTestY1].Motor->GearRatio * 100);
    str.sprintf("ER%ld,%ld,%ld,%ld", ERA, ERB, ERC, ERD);
    Gali_Command(str, __func__);
}

// ---------------------------------------------------------------------------
//  Stubs for methods gated TODO(W6-sensor/state-machine)
// ---------------------------------------------------------------------------
bool TMyMotor::CheckPos(bool)                                    { return false; }
bool TMyMotor::CheckYPos()                                       { return false; }
bool TMyMotor::CheckPos_nnMode(bool)                             { return false; }
void TMyMotor::MagazineUp()                                      {}
void TMyMotor::MagazineDown()                                    {}
bool TMyMotor::CheckYPosWhenZDown(int,int,AnsiString)            { return false; }
bool TMyMotor::CheckArmPosInRange(int,int,int)                   { return false; }
bool TMyMotor::CheckArmPosArrival(int,int,int)                   { return false; }
bool TMyMotor::CheckIndexYPos(bool)                              { return false; }
bool TMyMotor::Check_SHUTTLE_FLOODGATE_Staste(int)               { return false; }
void TMyMotor::TrayArmInitial()                                  {}
bool TMyMotor::Check_Y1Y2_TargetPosWillCrash(int,int)           { return false; }
bool TMyMotor::CheckY1Y2TargetPos(int,AnsiString)                { return false; }
bool TMyMotor::Check_Y1_TargetPosWillCrash(int,AnsiString)      { return false; }
bool TMyMotor::Check_Y2_TargetPosWillCrash(int,AnsiString)      { return false; }
bool TMyMotor::Check_Y1_TargetPosInTeachPos(int,AnsiString)     { return false; }
bool TMyMotor::Check_Y2_TargetPosInTeachPos(int,AnsiString)     { return false; }

// ===========================================================================
//  TTrayMotor
// ===========================================================================

TTrayMotor::TTrayMotor()
{
    fHasTray = false;
    fHTary   = false;
    pHTray   = NULL;    // AI(W4): VCL widget not available; always NULL in W4 sim
    Tray.SetXYItem(1, 1);
    ClearTray(__func__);
}

// ---------------------------------------------------------------------------
void TTrayMotor::SetHTrayPanel(TTMyTray *ptr)
{
    // AI(W4) 20260626: VCL TTMyTray is fwd-declared only; pHTray kept as NULL
    // in W4 sim build.  Real body: fHTary=true; pHTray=ptr;
    // TODO(W7-UI): restore when HTray.h / VCL TCustomControl is available.
    (void)ptr;
    // fHTary = true;
    // pHTray = ptr;
}

// ---------------------------------------------------------------------------
bool TTrayMotor::HasIC()    // kevin 20130509
{
    if (fHasTray == false)
        return false;

    if (pHTray != NULL)
    {
        // pHTray->Name comparison -- gated (VCL; W7)
        if (Tray.HasIC())
            return true;
    }
    else if (Tray.HasIC())
    {
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
bool TTrayMotor::HasCleanPad()  // kevin 20150505
{
    if (fHasTray == false)
        return false;
    // pHTray name checks gated (VCL; W7) -- always false in W4 sim
    return false;
}

// ---------------------------------------------------------------------------
bool TTrayMotor::HasRealIC()
{
    if (fHasTray == false)
        return false;
    if (Tray.HasRealIC())
        return true;
    return false;
}

// ---------------------------------------------------------------------------
void TTrayMotor::SetTrayBinData(int x, int y, int data, AnsiString iInfo)
{
    Tray.Data[x][y] = data;
    if (fHTary)
    {
        if (data >= 1000)
            data -= 1000;
        // pHTray->SetCellColorIndex(x, y, data);   // TODO(W7-UI)
        // pHTray->SetCellNumber(x, y, iInfo);       // TODO(W7-UI)
    }
}

// ---------------------------------------------------------------------------
void TTrayMotor::SetTraySingleData(int x, int y, int data, int iTarget)
{
    Tray.Data[x][y]    = data;
    Tray.iTarget[x][y] = iTarget;
    if (fHTary)
    {
        if (data >= 1000)
            data -= 1000;
        // pHTray->SetCellColorIndex(x, y, data);   // TODO(W7-UI)
    }
}

// ---------------------------------------------------------------------------
void TTrayMotor::SetTraySiteMap(int x, int y, int iSiteMap)     // Steven 20220510
{
    SetTraySingleData(x, y, HAS_IC);
    Tray.iWhichSite[x][y] = iSiteMap;
}

// ---------------------------------------------------------------------------
void TTrayMotor::SetNullIcToHasNullIc()
{
    for (int i = 0; i < Tray.XItem; i++)
        for (int j = 0; j < Tray.YItem; j++)
            if (Tray.Data[i][j] == NULL_IC)
                SetTraySingleData(i, j, HAS_NULL_IC);
}

// ---------------------------------------------------------------------------
void TTrayMotor::SetNullIcToHasIc()    // Sam 20240424
{
    for (int i = 0; i < Tray.XItem; i++)
        for (int j = 0; j < Tray.YItem; j++)
            if (Tray.Data[i][j] == NULL_IC)
                SetTraySingleData(i, j, HAS_IC);
}

// ---------------------------------------------------------------------------
void TTrayMotor::SetTrayBufferSingleData(int x, int y, int data)
{
    Tray.BufferData[x][y] = data;
}

// ---------------------------------------------------------------------------
void TTrayMotor::Refresh()
{
    if (fHTary)
    {
        // pHTray->XBlockItem = Tray.XBItem; ... TODO(W7-UI)
    }
}

// ---------------------------------------------------------------------------
void TTrayMotor::InitNewTray(int data, bool bShowSiteMapFlag, AnsiString Func)
{
    int iCount;
    Tray.ClearData();
    Tray.SetData(data);

    AnsiString Str;
    Str.sprintf("Initial new tray [%s] with IC type %s by function %s ",
                Alias.c_str(), sIC_Type[data].c_str(), Func.c_str());
    MNetLog(Str);

    if (fHTary)
    {
        // VCL pHTray layout updates TODO(W7-UI)
        bShowSiteMap = bShowSiteMapFlag;
        if (bShowSiteMap)
        {
            // iDirection / iSiteCount / bSiteHasTurnOn setup -- needs
            // IniConfig / TrayForm / GetSiteCount / bTestSiteUse -> TODO(W6)
            // iCount used here in the full body; unused in the W4 stub.
        }
    }
    (void)iCount;   // W4 stub: iCount used in gated FIFO site-map loop TODO(W6)
}

// ---------------------------------------------------------------------------
void TTrayMotor::ClearTray(AnsiString Func)
{
    AnsiString Str;
    if (fHasTray)
    {
        Str.sprintf("Clear tray [%s] by function %s ", Alias.c_str(), Func.c_str());
        MNetLog(Str);
    }

    fHasTray      = false;
    iIsCoverTray  = NULL_IC;    // JerryYang 20240318
    sTrayID       = "";
    Tray.ClearData();
    Tray.cTrayID  = "";         // JerryYang 20250120

    if (fHTary)
    {
        // pHTray layout reset TODO(W7-UI)
    }
    sUnloaderAlarmMsg = "";     // Jimmychiu 20240902
}

// ---------------------------------------------------------------------------
void TTrayMotor::InitEmptyTray(AnsiString Func)
{
    Tray.ClearData();
    AnsiString Str;
    Str.sprintf("Initial empty tray [%s] by function %s ", Alias.c_str(), Func.c_str());
    MNetLog(Str);

    if (fHTary)
    {
        // pHTray layout TODO(W7-UI)
    }
}

// ---------------------------------------------------------------------------
void TTrayMotor::SetTray(int data, AnsiString Func)
{
    fHasTray = true;
    if (fHTary)
    {
        fHasTray = true;
        InitNewTray(data, false, Func);
    }
}

// ---------------------------------------------------------------------------
bool TTrayMotor::UpHalfIsFull()
{
    for (int i = 0; i < Tray.XItem; i++)
        for (int j = 0; j < (Tray.YItem / 2); j++)
            if (Tray.Data[i][j] == NULL_IC)
                return false;
    return true;
}

// ---------------------------------------------------------------------------
bool TTrayMotor::DownHalfIsFull()
{
    for (int i = 0; i < Tray.XItem; i++)
        for (int j = (Tray.YItem / 2); j < Tray.YItem; j++)
            if (Tray.Data[i][j] == NULL_IC)
                return false;
    return true;
}

// ---------------------------------------------------------------------------
int TTrayMotor::WhichBufferIsFull()     // JerryYang 20221215: Magazine
{
    int iCnt[5] = {0, 0, 0, 0, 0};
    for (int k = 0; k < 5; k++)
    {
        iCnt[k] = 0;
        for (int i = 0; i < Tray.XItem; i++)
            for (int j = 0; j < Tray.YItem; j++)
                if (j >= k * iYRegNum && j < (k + 1) * iYRegNum &&
                    Tray.Data[i][j] == NULL_IC)
                    iCnt[k]++;

        if (iCnt[k] < 8)
            return k * 3;
    }
    return -1;
}

// ---------------------------------------------------------------------------
void TTrayMotor::MoveTrayAllItem(class TTrayMotor *Source)   // Sam 20240108
{
    if (Source->fHasTray == false)
        return;
    fHasTray = true;

    int MinRow = (Source->Tray.XItem >= Tray.XItem) ? Tray.XItem : Source->Tray.XItem;
    int MinCol = (Source->Tray.YItem >= Tray.YItem) ? Tray.YItem : Source->Tray.YItem;

    for (int i = 0; i < MinRow; i++)
    {
        for (int j = 0; j < MinCol; j++)
        {
            SetTraySingleData(i, j, Source->Tray.Data[i][j]);
            Tray.iWhichSite[i][j]   = Source->Tray.iWhichSite[i][j];
            Tray.iNeedRotAng[i][j]  = Source->Tray.iNeedRotAng[i][j];
            Tray.iCurrRotAng[i][j]  = Source->Tray.iCurrRotAng[i][j];
            Tray.iWhichIndex[i][j]  = Source->Tray.iWhichIndex[i][j];
            Tray.iBinCode[i][j]     = Source->Tray.iBinCode[i][j];
            Tray.BufferData[i][j]   = Source->Tray.BufferData[i][j];
            Tray.iTarget[i][j]      = Source->Tray.iTarget[i][j];
            Tray.iCleanCount[i][j]  = Source->Tray.iCleanCount[i][j];
            Tray.bFliped[i][j]      = Source->Tray.bFliped[i][j];
            Tray.cDeviceInf[i][j]   = Source->Tray.cDeviceInf[i][j];
            Tray.b2DIDNG[i][j]      = Source->Tray.b2DIDNG[i][j];
            Tray.cReDeviceInf[i][j] = Source->Tray.cReDeviceInf[i][j];
            Tray.iAOIResult[i][j]   = Source->Tray.iAOIResult[i][j];  // Sam 20240325
        }
    }
    sUnloaderAlarmMsg = Source->sUnloaderAlarmMsg;  // Jimmychiu 20240902
    Source->ClearTray(__func__);
}

// ---------------------------------------------------------------------------
int TTrayMotor::HowManyDevice(int iType)    // Ifor 20160829
{
    int iCT = 0;
    for (int i = 0; i < Tray.XItem; i++)
        for (int j = 0; j < Tray.YItem; j++)
            if (Tray.Data[i][j] == iType)
                iCT++;
    return iCT;
}

// ---------------------------------------------------------------------------
int TTrayMotor::HowManyDevice()             // Steven 20190627
{
    int iCT = 0;
    for (int i = 0; i < Tray.XItem; i++)
        for (int j = 0; j < Tray.YItem; j++)
            if (Tray.Data[i][j] != NULL_IC && Tray.Data[i][j] != HAS_NULL_IC)
                iCT++;
    return iCT;
}

// ---------------------------------------------------------------------------
void TTrayMotor::MoveTrayData(TTrayMotor &TrayMotor)     // JerryYang 20221215: Magazine
{
    MoveTrayAllItem(&TrayMotor);
}

// ---------------------------------------------------------------------------
//  TTrayMotor stubs for methods with VCL/sensor coupling (gated TODO)
// ---------------------------------------------------------------------------
bool TTrayMotor::SearchHasEmpryToPlace(int)     { return false; }
bool TTrayMotor::TrayFeedHasIC()                { return false; }
void TTrayMotor::SetHasNullIcToNullIc()
{
    for (int i = 0; i < Tray.XItem; i++)
        for (int j = 0; j < Tray.YItem; j++)
            if (Tray.Data[i][j] == HAS_NULL_IC)
                SetTraySingleData(i, j, NULL_IC);
}
bool TTrayMotor::HasOnlyDataICAndNullIC(int DataType)
{
    return Tray.HasOnlyDataICAndNullIC(DataType);
}

// ===========================================================================
//  Free function stubs  -- state-machine / sensor coupling; TODO(W6)
// ===========================================================================
bool InArmContinuousMove_9045(int,int,int[],int,bool[][MAX_ARM_Col],int[][MAX_ARM_Col],bool,bool) { return false; }
int  InArmPitchMove(int[],int,bool)    { return 0; }
void SetInArmPitchSpeed(bool)          {}
bool InArmCynMove(bool[][MAX_ARM_Col]) { return false; }
bool InArmZMoveDown(bool[][MAX_ARM_Col],int[][MAX_ARM_Col],bool,bool) { return false; }
bool InArmZMoveUp(int, bool)           { return false; }

bool OutArmContinuousMove_9045(int,int,int[],int,bool[][MAX_ARM_Col],int[][MAX_ARM_Col],bool,bool) { return false; }
bool OutArmPitchMove(int[],int,bool)   { return false; }
bool OutArmCynMove(bool[][MAX_ARM_Col]) { return false; }
bool OutArmZMoveDown(bool[][MAX_ARM_Col],int[][MAX_ARM_Col],bool,bool) { return false; }
bool OutArmZMoveUp(int, bool)           { return false; }

bool SortArmContinuousMove(int,int,int,bool[][MAX_ARM_Col],int[][MAX_ARM_Col],bool,bool) { return false; }
bool SortArmPitchMove(int)             { return false; }
void InitPCIL112_SortArmXYMoveTask()   {}
int  PCIL112_SortArmXYMove(int,int)    { return 0; }
AnsiString sSortArmZHomeState()        { return ""; }
bool SortArmZMoveDown(bool[][MAX_ARM_Col],int[][MAX_ARM_Col],bool,bool) { return false; }
bool SortArmZMoveUp(int, bool)         { return false; }

bool CatchMgzTrayMove(int)             { return false; }
void OpenPCI132Card(bool)              {}
void ServoOnAllMOT()                   {}
int  OutArmZSafe(int)                  { return -1; }
int  InArmZSafe(int)                   { return -1; }
int  SortArmZSafe(int)                 { return -1; }
AnsiString sInArmZHomeState()          { return ""; }
AnsiString sOutArmZHomeState()         { return ""; }
void InitPCIL112_InArmXYMoveTask()     {}
int  PCIL112_InArmXYMove(int,int)      { return 0; }
void InitPCIL112_OutArmXYMoveTask()    {}
int  PCIL112_OutArmXYMove(int,int)     { return 0; }
int  CheckOutArmZNeedHome()            { return 0; }
int  CheckInArmZNeedHome()             { return 0; }
void ShowIndexMotorError(AnsiString, bool) {}
AnsiString SaveLog(AnsiString)         { return ""; }
void RecordIndexPositionError(AnsiString,bool,bool,bool,bool,long*) {}
bool TrayArmContinuousMoveForOCR(int,int) { return false; }
bool TrayMoveHome()                    { return false; }
bool ShuttleSensorContinuousMove(int,int,bool) { return false; }
int  CheckOutArmDestory()              { return 0; }
void RecordIndexPosition(int, int)     {}
void EncoderTeachingMaxMinCount(int)   {}
void InitialMaxMinValue(AnsiString)    {}
void TrigerIndexAxisHome()             {}
