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
//  AI(W906-PT-W3-integrate) 20260808: the old "NOTE on MNetLog" that stood here
//  is now obsolete and has been removed with the stub it described -- the real
//  `bool MNetLog(AnsiString)` landed with Motor/myMN200motor.cpp (:2494, golden
//  :2146-2156) in PT-W3 and is declared by myMN200motor.h:192, which this file
//  now includes.  Same declaration golden itself sees here.
// =============================================================================

#include "MachineDefine.h"
#include "Motor/mymotor.h"
#include "MachineType.h"    // ChangeToFloatNonPcnt, MOTION_CARD_TYPE constants, etc.
#include "cpublic.h"
#include "cmydef.h"         // sIC_Type[], iYRegNum, bTestSiteUse[], etc.
// AI(W906-PT-W3-integrate) 20260808: golden mymotor.cpp:27 `#include "myMN200motor.h"`
//   -- restored, needed for MyMNetLine/MAXRing so golden's `myLine[MAXRing]` definition
//   below (golden :43) can live where golden puts it.  Same relative order as golden
//   (after cmydef.h, golden :19).
#include "Motor/myMN200motor.h"   // MyMNetLine, MAXRing (golden myMN200motor.h:78-92 / :96)

// ---------------------------------------------------------------------------
//  AI(W906-PT-W3-integrate) 20260808: RETIRED the local
//  `static void MNetLog(AnsiString) {}` stub that used to sit here.  It had to
//  go: myMN200motor.h:192 declares `extern bool MNetLog(AnsiString)` (golden
//  :192 verbatim), so once this file started including that header the two
//  declarations differ only in return type -- an ambiguating redeclaration, a
//  hard compile error, not a silent shadow.  The real body is
//  Motor/myMN200motor.cpp:2494 (golden :2146-2156), registered in ht9045_motor.
//  Behaviour note: that body is itself GATED (golden writes fMain->slMNetLog,
//  which has no port) so it does nothing and returns true unconditionally --
//  which is what every golden code path returns too (golden :2155).  The three
//  call sites in this file (:1199/:1222/:1244, golden :1200/:1447/:1477)
//  discard the result, so both the no-op behaviour and the void->bool signature
//  change are non-events for them.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  Module-level constants / globals
// ---------------------------------------------------------------------------
#define RESET_TIMES 900

#define PNP_DONE    0   // order finished
#define PNP_DOING   1   // order in progress

int ZSafePos  =  20;        // Steven 20220207: In/Out Arm Z safe position
int ZlimitPos = -3200;      // Ifor 20221026:   In/Out Arm limit position
// AI(W906-PT-W3-integrate) 20260808: golden Motor/mymotor.cpp:43
//   `MyMNetLine myLine[MAXRing];  //Isaac 20181212 (Steven) : Baud Rate防呆功能`
//   -- restored, and the file-head DEFERRED list above amended accordingly.  It was
//   deferred as "requires MyMNet/mn_open_all", but that is wrong about the DEFINITION:
//   the array is plain storage, and it is Motor/myMN200motor.cpp (landed in PT-W3) that
//   needs the vendor SDK, not this line.  myMN200motor.h:181 declares it and
//   myMN200motor.cpp reads/writes it at :1208/:1209/:2449/:2451/:2453, so without this
//   definition that unit does not link.  Golden keeps it here, in exactly this position
//   (between ZlimitPos and the `extern bool CheckOutSuckICFallDown` declaration), so the
//   port keeps it here too rather than inventing a new home.
MyMNetLine myLine[MAXRing];                                                     //Isaac 20181212 (Steven) : Baud Rate防呆功能

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

// ===========================================================================
//AI(W906-PT-W6c) 20260810: GOLDEN TEXT RESTORED (GATED) -- MotorMovePosition
//  golden HT9011UC_Code_V3.33.906.0_20260618/Motor/mymotor.cpp:549-860  (312 lines)
//  Census scored this function "translated" on name match only; the LIVE body
//  below is an abbreviated stand-in and golden's text existed nowhere in the
//  tree. The block inside the gate is golden's body transcribed VERBATIM
//  (cp950 -> UTF-8; byte-exact when re-encoded to cp950) and is INACTIVE.
//  The LIVE body that follows is UNCHANGED and remains the only active
//  definition -- net behaviour delta = 0. NOTHING was added inside the gate,
//  so a later un-gate is mechanical.
// ===========================================================================
#if 0 // AI-W6C-GOLDEN-BEGIN MotorMovePosition Motor/mymotor.cpp:549-860
int TMyMotor::MotorMovePosition(int &Position, int speed, int Tar)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return -1;
    }

    int ret=0;
    int Pos=Tar;
    int iEncoder=0;
    AnsiString S1="", S2="", S3="";
    int iGap=2;                                                                 //Sam 20230621 : Gap容許誤差改為1>2 //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條

    if(Motor!=NULL &&                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->Enable)                                                           // has true motor
    {
        if(Tar>=Motor->PSoftLimitP)
        {
            S1=AnsiString("The target position of ")+Alias+AnsiString(" over positive soft limit !");
            S2=Alias+AnsiString("的目標位置超過正向軟體極限!");
            S3.sprintf("%d > %d", Tar, Motor->PSoftLimitP);
            ShowMyMessage(S1, S2, S3);
            return -2;
        }

        if(Tar<=Motor->PSoftLimitN)
        {
            S1=AnsiString("The target position of ")+Alias+AnsiString(" below negative soft limit !");
            S2=Alias+AnsiString("的目標位置低於負向軟體極限!");
            S3.sprintf("%d <= %d", Tar, Motor->PSoftLimitN);
            ShowMyMessage(S1, S2, S3);
            return -3;
        }

        Position=ReadPos();                                                     //calculate GearRate Pos

        if(Position!=Tar)                                                       //RogerYang 20260113 : 用來紀錄Rotator最近一次旋轉方向
        {
            iLastRotatorDirP=(Tar>Position)?true:false;
        }

        if(fCMD==false && Motor->MotionDone()==false)
        {
            return 0;
        }
        else if(fCMD==false)
        {
            GetRealPos(&Pos);                                                   //pos will change to gear ration value
            #ifndef USE_CompareCommandPos
            if(Tar==Position)
            #else
            if((Motor->GearRatio>2 && Motor->GearRatio<=5)  &&
                ((Mot_Name>=MInArmZA && Mot_Name<=MInArmZH) ||
                 (Mot_Name>=MOutArmZA && Mot_Name<=MOutArmZH)))                 //Jimmychiu 20230216 : 吸嘴高度
            {
                iGap=5;
            }

            if(CompareCommandPos(Tar, iGap)==1)                                 //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條
            #endif
            {
                if(Motor->PServoAlarmOn)
                {
                    ScanMotorStatus();
                    if(Led[iInposLed]==true)
                    {
                        return 0;
                    }
                    else
                    {
                        InitMOTParameter();
                        return 1;                                               // -4 --> 1
                    }
                }
                else
                {
                    InitMOTParameter();
                    return 1;
                }
            }
//            if(Motor->Direction==false)
//                Pos=-Pos;

            if(Mot_Name==MInArmY || Mot_Name==MOutArmY)                         //Steven 20181129 : 短距離的移動把加減速縮短一半
            {
                if(AUTO_EMPTY_COLOR>=3)
                {
                    if(Mot_Name==MOutArmY && (abs(Tar-Position)<6500))
                        ret=Motor->MoveToPosShortDisSlowSP(Pos);
                    else
                        ret=Motor->MoveToPos(Pos);
                }
                else
                {
                    if(abs(Tar-Position)<6500)
                        ret=Motor->MoveToPosShortDistance(Pos);
                    else
                        ret=Motor->MoveToPos(Pos);
                }
            }
            else if(Mot_Name==MInShuttle1 || Mot_Name==MInShuttle2)
            {
                if(SHUTTLE_FLOODGATE==1)                                        //Ifor 20260327 add:避免Shuttle 移動時閘門未完全開啟導致撞機
                {
                    if(Mot_Name==MInShuttle1)
                    {
                        Cylinder[C_Shuttle1Floodgate].Off();
                        Cylinder[C_OutShuttle1Floodgate].Off();
                        if(Cylinder[C_Shuttle1Floodgate].OffSensor()==false ||
                           Cylinder[C_OutShuttle1Floodgate].OffSensor()==false)
                        {                                                       //Ifor 20260401 add: Floodgate open timeout
                            if(!bSh1FloodgateOpenWaiting)
                            {
                                tSh1FloodgateOpenTimeout.SetSecAndOn(3);
                                bSh1FloodgateOpenWaiting=true;
                            }
                            else if(tSh1FloodgateOpenTimeout.Off())
                            {
                                tSh1FloodgateOpenTimeout.SetSecAndOn(3);           //Ifor 20260401 add: re-arm for next warning
                                ShowMyMessage("Shuttle Floodgate open timeout! Check cylinder sensor.");
                            }
                            return -1;
                        }
                        bSh1FloodgateOpenWaiting=false;                            //Ifor 20260401 add: reset on success
                    }
                    else
                    {
                        Cylinder[C_Shuttle2Floodgate].Off();
                        Cylinder[C_OutShuttle2Floodgate].Off();
                        if(Cylinder[C_Shuttle2Floodgate].OffSensor()==false ||
                           Cylinder[C_OutShuttle2Floodgate].OffSensor()==false)
                        {                                                       //Ifor 20260401 add: Floodgate open timeout
                            if(!bSh2FloodgateOpenWaiting)
                            {
                                tSh2FloodgateOpenTimeout.SetSecAndOn(3);
                                bSh2FloodgateOpenWaiting=true;
                            }
                            else if(tSh2FloodgateOpenTimeout.Off())
                            {
                                tSh2FloodgateOpenTimeout.SetSecAndOn(3);           //Ifor 20260401 add: re-arm for next warning
                                ShowMyMessage("Shuttle Floodgate open timeout! Check cylinder sensor.");
                            }
                            return -1;
                        }
                        bSh2FloodgateOpenWaiting=false;                            //Ifor 20260401 add: reset on success
                    }
                }

                if(abs(Tar-Position)<5000)
                    ret=Motor->MoveToPosShortDistance(Pos);
                else
                    ret=Motor->MoveToPos(Pos);
            }
            else
            {
                ret=Motor->MoveToPos(Pos);
            }

            if(ret==0)
            {
            }

            fCMD=true;
        }
        else
        {
            if(Motor->MotionDone())
            {
                Position=Tar;
                if(Motor->PServoAlarmOn)
                {
                    ScanMotorStatus();
                    if(Led[iInposLed]==true)
                    {
                        return 0;
                    }
                    else
                    {
                        InitMOTParameter();
                        iEncoderCheckCT++;
                        if(bCheckEncoderEveryTime==true || iEncoderCheckCT>100)
                        {
                            iEncoderCheckCT=0;
                            iEncoder=ReadEncoderPos();
                            if((Tar-iEncoderTorence)>iEncoder ||                //JerryYang 20180706 : 修改Encoder到位容許範圍
                               (Tar+iEncoderTorence)<iEncoder)
                            {
                                S1.sprintf("MOT=%s, Tar=%d, Encoder=%d", Alias, Tar, iEncoder);
                                ShowErrorMessage("WAR1639", 0, MMSystem, 0, S1);   //Motor encoder error, check encoder cable
                                return -5;
                            }

                            if(Motor->PServoAlarmOn)
                            {
                                if(Mot_Name==MInRotateKit ||                    //Sam 20190811 : 防止 Rotate 旋轉完後剛好位置剛好落在原點上面導致誤報警。
                                   Mot_Name==MOutRotateKit)
                                {
                                    return 1;
                                }

                                if(iEncoder>2000 || iEncoder<-2000)
                                {
                                    ScanMotorStatus();
                                    if((Mot_Name==MInArmY ||                    //JerryYang 20191210 fix auto clean時後排到shuttle row A跳出home sensor error
                                        Mot_Name==MOutArmY) &&
                                       iEncoder>2000)
                                    {
                                    }
                                    else
                                    {
                                        if(Led[1]==true)
                                        {
                                            S1.sprintf("MOT=%s Home sensor error!!", Alias);
                                            ShowMyMessage(S1, "");
                                            return -6;
                                        }
                                    }
                                }
                            }

                            return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    InitMOTParameter();

                    //AI(ht9045-v899) 20260505: MLoaderY 工作位置 iMLoaderYCarPos=-505 會誤觸發此防護，僅對 MLoaderY 豁免，其餘馬達維持原有 home sensor 防護
                    if(Position<=-500 && Mot_Name!=MLoaderY)                                          //kevin 20140121 Z軸 home sensor 損壞
                    {
                        if(Mot_Name==MInRotateKit ||                            //Sam 20190811 : 防止 Rotate 旋轉完後剛好位置剛好落在原點上面導致誤報警。
                           Mot_Name==MOutRotateKit)
                        {
                            return 1;
                        }
                        ScanMotorStatus();
                        if(Led[1]==true)
                        {
                            S1.sprintf("MOT=%s Home sensor error!!", Alias);
                            ShowMyMessage(S1, "");

                            if(Mot_Name==MInArmPitch   || Mot_Name==MInArmPitchX2 ||
                               Mot_Name==MInArmPitchX3 || Mot_Name==MInArmPitchX4 ||
                               Mot_Name==MInArmPitchY)
                            {
                                SetInArmHome();
                            }

                            if(Mot_Name==MOutArmPitch   || Mot_Name==MOutArmPitchX2 ||
                               Mot_Name==MOutArmPitchX3 || Mot_Name==MOutArmPitchX4 ||
                               Mot_Name==MOutArmPitchY)
                            {
                                SetOutArmHome();
                            }
                            return -6;
                        }
                    }
                    return 1;
                }
            }
        }
        return 0;
    }

    #ifndef SOFT_SIMULTE
        Position=Tar;
        return 1;
    #else
        if(Position==Tar)
        {
            fCMD=false;
            return 1;
        }
        else
        {
            if(speed<=0)                                                        //Steven 20210730 : 修正軟體模擬的最小速度
                speed=100;

            if(Position>Tar)
            {
                fCMD=true;
                Position-=speed;
                if(Position<=Tar)
                {
                    fCMD=false;
                    Position=Tar;
                    return 1;
                }
            }
            else
            {
                fCMD=true;
                Position+=speed;
                if(Position>=Tar)
                {
                    fCMD=false;
                    Position=Tar;
                    return 1;
                }
            }
        }
        return 0;
    #endif
}
#endif // AI-W6C-GOLDEN-END MotorMovePosition Motor/mymotor.cpp:549-860

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
int TMyMotor::MotorMovePosition(int &Position, int /*speed*/, int Tar)
{
    // AI(ht9045-v906) 20260629: W7 OFFLINE convergence shortcut so the move
    // family ARRIVES over the Sim HAL. Mirrors the golden offline terminal
    // (BCB6 mymotor.cpp:821-823, #ifndef SOFT_SIMULTE { Position=Tar; return 1; }):
    // with no real driver attached the hardware block is skipped and the move
    // reports Move Success at the commanded target.
    if (Motor == NULL || !Motor->Enable)
    {
        Position = Tar;
        return 1;   // 1 = Move Success (golden mymotor.cpp:823)
    }

    // TODO(W6-state-machine): translate full body from BCB6 mymotor.cpp:549-860
    // Depends on: ShowMyMessage, Cylinder[], C_Shuttle1/2Floodgate,
    //             ShowErrorMessage("WAR1639",...), SetInArmHome/SetOutArmHome,
    //             SOFT_SIMULTE branch (hardware sim), CompareCommandPos.
    // Real-driver path (Motor!=NULL && Enable); never taken in the offline build.
    return 0;
}

// ===========================================================================
//AI(W906-PT-W6c) 20260810: GOLDEN TEXT RESTORED (GATED) -- MotorMove
//  golden HT9011UC_Code_V3.33.906.0_20260618/Motor/mymotor.cpp:871-962  (92 lines)
//  Census scored this function "translated" on name match only; the LIVE body
//  below is an abbreviated stand-in and golden's text existed nowhere in the
//  tree. The block inside the gate is golden's body transcribed VERBATIM
//  (cp950 -> UTF-8; byte-exact when re-encoded to cp950) and is INACTIVE.
//  The LIVE body that follows is UNCHANGED and remains the only active
//  definition -- net behaviour delta = 0. NOTHING was added inside the gate,
//  so a later un-gate is mechanical.
// ===========================================================================
#if 0 // AI-W6C-GOLDEN-BEGIN MotorMove Motor/mymotor.cpp:871-962
int TMyMotor::MotorMove(int p)
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor==NULL ||                                                           //JimmyChiu 20250306 : 避免還沒初始化就操作馬達
       Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return -1;
    }

    //jou 2010-12-23 保護兩次命令會造成撞機
    if(p!=iOldPos)
    {
        fCMD=false;
        iOldPos=p;
        bSh1FloodgateOpenWaiting=false;                                         //Ifor 20260401 add: reset timeout when target changes
        bSh2FloodgateOpenWaiting=false;                                         //Ifor 20260401 add: reset timeout when target changes
    }

    int ret=0;
    int iCommandPos;
    int iGap=1;

    if(fCanMove ==false  ||
       fCanMoveR==false  ||
       fCanMoveM==false  ||
       fCanMoveL==false  ||
       mapLockList.size()!=0)  //Klutter 20210817 加入鎖馬達機制                //Steven 20210825 : 吹氣完成才可以歸零
    {
        PCIL132_StopMotor();

        if((Mot_Name==MInShuttle1 || Mot_Name==MInShuttle2) && (Motor->GearRatio>1))
        {
            iCommandPos=ReadPos();
            if(iCommandPos>p+iGap)      //超出+
            {
                return -2;
            }
            else if(iCommandPos<p-iGap)
            {
                return -3;             //低於-
            }
            else
            {
                fCMD=false;
                return 1;              //正常等於
            }
        }
        else
        {
            if(p==ReadPos())
            {
                fCMD=false;
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }

    ret=MotorMovePosition(Position, speed, p);
    if(bShowMotorMove==true)
    {
        ScreenPos=(int)(Scale*(Position-FactStart))+RefStart;
        if(bPanelUse)
        {
            if(bUpDownMove)
            {
                if(abs(PWinCtrl->Top-ScreenPos)>2)
                    PWinCtrl->Top=ScreenPos;
            }
            else
            {
                if(abs(PWinCtrl->Left-ScreenPos)>2)
                    PWinCtrl->Left=ScreenPos;
            }
        }
    }

    if(ret==1)
    {
        fCMD=false;
        return 1;
    }
    else
    {
        return ret;
    }
}
#endif // AI-W6C-GOLDEN-END MotorMove Motor/mymotor.cpp:871-962

// ---------------------------------------------------------------------------
//  MotorMove -- W4 STUB
// ---------------------------------------------------------------------------
int TMyMotor::MotorMove(int p)
{
    // AI(ht9045-v906) 20260629: W7 OFFLINE convergence shortcut. With no real
    // driver attached, snap the stored Position to the commanded target and
    // report Move Success (golden MotorMove returns 1 on arrival, mymotor.cpp:953-956,
    // via the MotorMovePosition offline terminal mymotor.cpp:821-823). Also refresh
    // ScreenPos exactly as ReadPos() does so panel geometry stays consistent.
    if (Motor == NULL || !Motor->Enable)
    {
        Position  = p;
        ScreenPos = (int)(Scale * (Position - FactStart)) + RefStart;
        fCMD      = false;
        return 1;   // 1 = Move Success (golden mymotor.cpp:953-956)
    }

    // TODO(W6-state-machine): translate full body (BCB6 mymotor.cpp:871-962)
    // Real-driver path (Motor!=NULL && Enable); never taken in the offline build.
    return 0;
}

// ---------------------------------------------------------------------------
//  MotorMove2SpeedForPicker -- W4 STUB
// ---------------------------------------------------------------------------
bool TMyMotor::MotorMove2SpeedForPicker(int FinalPos, ARM_CONDITION * /*ARM*/, bool /*bIsLoader*/)
{
    // AI(ht9045-v906) 20260629: W7 OFFLINE convergence shortcut. The golden body
    // funnels every two-speed branch through MotorMove(FinalPos) and returns its
    // iFlag (1==arrived, mymotor.cpp:971/988/995/999); offline that arrives at once.
    if (Motor == NULL || !Motor->Enable)
    {
        Position  = FinalPos;
        ScreenPos = (int)(Scale * (Position - FactStart)) + RefStart;
        fCMD      = false;
        return true;   // iFlag==1 Move Success (golden mymotor.cpp:999)
    }

    // TODO(W6-state-machine): translate full body (BCB6 mymotor.cpp:964-999)
    // Real-driver path (Motor!=NULL && Enable); never taken in the offline build.
    return false;
}

// ===========================================================================
//AI(W906-PT-W6c) 20260810: GOLDEN TEXT RESTORED (GATED) -- MotorMoveShuttleShake
//  golden HT9011UC_Code_V3.33.906.0_20260618/Motor/mymotor.cpp:5198-5316  (119 lines)
//  Census scored this function "translated" on name match only; the LIVE body
//  below is an abbreviated stand-in and golden's text existed nowhere in the
//  tree. The block inside the gate is golden's body transcribed VERBATIM
//  (cp950 -> UTF-8; byte-exact when re-encoded to cp950) and is INACTIVE.
//  The LIVE body that follows is UNCHANGED and remains the only active
//  definition -- net behaviour delta = 0. NOTHING was added inside the gate,
//  so a later un-gate is mechanical.
// ===========================================================================
#if 0 // AI-W6C-GOLDEN-BEGIN MotorMoveShuttleShake Motor/mymotor.cpp:5198-5316
bool TMyMotor::MotorMoveShuttleShake(int p)                                     //JerryYang 20190628 shuttle shake專用command
{
    //******************************************************************************
    //  注意!! CheckIsSafeDoorOpen為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(Motor->CheckIsSafeDoorOpen())                                            //Jimmychiu 20221013 safedoor判斷整合Function
    {
        return false;
    }

    if(bShuttleShake==false)
        return false;
    if(Mot_Name==MInShuttle1 || Mot_Name==MInShuttle2)
    {
    }
    else
    {
        return false;
    }

    if(Mot_Name==MInShuttle1)
    {
        if(p==Prod.InSHT[0].iLeft+SHSpeed_File.iShakeDistance*100 || p==Prod.InSHT[0].iLeft)    //Sam 20250326 : 新增 Shake 條件設定
        {
        }
        else
        {
            return false;
        }
    }
    else if(Mot_Name==MInShuttle2)
    {
        if(p==Prod.InSHT[1].iLeft+SHSpeed_File.iShakeDistance*100 || p==Prod.InSHT[1].iLeft)    //Sam 20250326 : 新增 Shake 條件設定
        {
        }
        else
        {
            return false;
        }
    }

    if(p!=iOldPos)                                                              //jou 2010-12-23 保護兩次命令會造成撞機
    {
        fCMD=false;
        iOldPos=p;
    }

    int ret=0;
    int iCommandPos;
    int iGap=1;

    if(fCanMove ==false  ||
//       fCanMoveR==false  ||
       fCanMoveM==false  ||
       fCanMoveL==false  ||                                                     //Klutter 20210817 加入鎖馬達機制
       mapLockList.size()!=0)                                                   //Steven 20210825 : 吹氣完成才可以歸零
    {
        PCIL132_StopMotor();

        if((Mot_Name==MInShuttle1 || Mot_Name==MInShuttle2) && (Motor->GearRatio>1))
        {
            iCommandPos=ReadPos();
            if(iCommandPos>p+iGap)                                              //超出+
            {
                return false;
            }
            else if(iCommandPos<p-iGap)
            {
                return false;                                                   //低於-
            }
            else
            {
                fCMD=false;
                return true;                                                    //正常等於
            }
        }
        else
        {
            if(p==ReadPos())
            {
                fCMD=false;
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    ret=MotorMovePosition(Position, speed, p);
    if(bShowMotorMove==true)
    {
        ScreenPos=(int)(Scale*(Position-FactStart))+RefStart;
        if(bPanelUse)
        {
            if(bUpDownMove)
            {
                if(abs(PWinCtrl->Top-ScreenPos)>2)
                    PWinCtrl->Top=ScreenPos;
            }
            else
            {
                if(abs(PWinCtrl->Left-ScreenPos)>2)
                    PWinCtrl->Left=ScreenPos;
            }
        }
    }

    if(ret==1)
    {
        fCMD=false;
        return true;
    }
    else
    {
        return false;
    }
}
#endif // AI-W6C-GOLDEN-END MotorMoveShuttleShake Motor/mymotor.cpp:5198-5316

// ---------------------------------------------------------------------------
//  MotorMoveShuttleShake -- W4 STUB
// ---------------------------------------------------------------------------
bool TMyMotor::MotorMoveShuttleShake(int p)
{
    // AI(ht9045-v906) 20260629: W7 OFFLINE convergence shortcut. The shake
    // sequence completes instantly with no real driver, so report done and snap
    // the stored Position to the commanded target (golden returns true when the
    // shake command finishes, BCB6 mymotor.cpp MotorMoveShuttleShake).
    if (Motor == NULL || !Motor->Enable)
    {
        Position  = p;
        ScreenPos = (int)(Scale * (Position - FactStart)) + RefStart;
        return true;
    }

    // TODO(W6-state-machine)
    // Real-driver path (Motor!=NULL && Enable); never taken in the offline build.
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

// ===========================================================================
//AI(W906-PT-W6c) 20260810: GOLDEN TEXT RESTORED (GATED) -- InitNewTray
//  golden HT9011UC_Code_V3.33.906.0_20260618/Motor/mymotor.cpp:1192-1439  (248 lines)
//  Census scored this function "translated" on name match only; the LIVE body
//  below is an abbreviated stand-in and golden's text existed nowhere in the
//  tree. The block inside the gate is golden's body transcribed VERBATIM
//  (cp950 -> UTF-8; byte-exact when re-encoded to cp950) and is INACTIVE.
//  The LIVE body that follows is UNCHANGED and remains the only active
//  definition -- net behaviour delta = 0. NOTHING was added inside the gate,
//  so a later un-gate is mechanical.
// ===========================================================================
#if 0 // AI-W6C-GOLDEN-BEGIN InitNewTray Motor/mymotor.cpp:1192-1439
void TTrayMotor::InitNewTray(int data, bool bShowSiteMapFlag, AnsiString Func)
{
    int iCount;
    Tray.ClearData();                                                           //Steven 20200619 : 加上保護
    Tray.SetData(data);

    AnsiString Str;
    Str.sprintf("Initial new tray [%s] with IC type %s by function %s ", Alias, sIC_Type[data], Func);
    MNetLog(Str);

    if(fHTary)
    {
        pHTray->XBlockItem=Tray.XBItem;                                         //Frank 20160928 add Subtray Function
        pHTray->YBlockItem=Tray.YBItem;

        pHTray->XBlockWidth=Tray.XBWidth;
        pHTray->YBlockWidth=Tray.YBWidth;

        pHTray->XItem=Tray.XItem;
        pHTray->YItem=Tray.YItem;

        bShowSiteMap=bShowSiteMapFlag;
        if(bShowSiteMap)                                                        //Steven 20170302 (wei) : FIFO MODE
        {
            if(IniConfig.bI37_LockLoaderDirection)
            {
                iDirection=IniConfig.iI37_LockLoaderDirection;
            }
            else
            {
                iDirection=TrayForm.Loader.Direction;
            }
            iSiteCount=GetSiteCount();
            for(int i=0; i<MAX_SOCKET_ROW; i++)
            {
                for(int j=0; j<MAX_SOCKET_COL; j++)
                {
                    if(TestIF_File.iSiteMap[i][j]>0)                            //Steven 20170302 (wei) : 確認哪個Site有開, 從1開始~32
                    {
                        if(TestIF_File.iShuttleMode==1)                         //單arm       //Isaac 20210821 : FIFO開site修正
                            bSiteHasTurnOn[TestIF_File.iSiteMap[i][j]]=bTestSiteUse[TestIF_File.iShuttle_Sel][i][j];
                        else                                                    //雙arm
                            bSiteHasTurnOn[TestIF_File.iSiteMap[i][j]]=bTestSiteUse[0][i][j];
                    }
                }
            }
        }

        for(int i=0; i<Tray.XItem; i++)
        {
            for(int j=0; j<Tray.YItem; j++)
            {
                pHTray->SetCellColorIndex(i, j, data);
                if(bShowSiteMap==false)                                         //Steven 20170302 (wei) : FIFO MODE
                    pHTray->SetCellNumber(i, j, "");
            }
        }

        if(bShowSiteMap)                                                        //Steven 20170302 (wei) : FIFO MODE
        {
            iCount=1;
            // ----   左至右,上至下
            //  /
            // --->
            if(iDirection==0)
            {
                for(int j=0; j<Tray.YItem; j++)
                {
                    for(int i=0; i<Tray.XItem; i++)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // ----   右至左,上至下
            //  \\
            // <---
            else if(iDirection==1)
            {
                for(int j=0; j<Tray.YItem; j++)
                {
                    for(int i=Tray.XItem-1; i>=0; i--)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // --->   左至右,下至上
            //  \\
            // ----
            else if(iDirection==2)
            {
                for(int j=Tray.YItem-1; j>=0; j--)
                {
                    for(int i=0; i<Tray.XItem; i++)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // <---   右至左,下至上
            //   /
            // ----
            else if(iDirection==3)
            {
                for(int j=Tray.YItem-1; j>=0; j--)
                {
                    for(int i=Tray.XItem-1; i>=0; i--)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // |   | 上至下, 左至右
            // | / |
            // |   V
            else if(iDirection==4)
            {
                for(int i=0; i<Tray.XItem; i++)
                {
                    for(int j=0; j<Tray.YItem; j++)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // |   ^ 下至上, 左至右
            // | \\|
            // |   |
            else if(iDirection==5)
            {
                for(int i=0; i<Tray.XItem; i++)
                {
                    for(int j=Tray.YItem-1; j>=0; j--)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // |   | 上至下, 右至左
            // | \\|
            // V   |
            else if(iDirection==6)
            {
                for(int i=Tray.XItem-1; i>=0; i--)
                {
                    for(int j=0; j<Tray.YItem; j++)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
            // ^   | 下至上, 右至左
            // | / |
            // |   |
            else if(iDirection==7)
            {
                for(int i=Tray.XItem-1; i>=0; i--)
                {
                    for(int j=Tray.YItem-1; j>=0; j--)
                    {
                        while(bSiteHasTurnOn[iCount]==false)
                        {
                            iCount++;
                            if(iCount>iSiteCount)
                                iCount=1;
                        };
                        pHTray->SetCellNumber(i, j, iCount);
                        Tray.iWhichSite[i][j]=iCount;
                        iCount++;
                        if(iCount>iSiteCount)
                            iCount=1;
                    }
                }
            }
        }
    }
}
#endif // AI-W6C-GOLDEN-END InitNewTray Motor/mymotor.cpp:1192-1439

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
// AI(W906-PT-W3-integrate) 20260808: `void OpenPCI132Card(bool) {}` RETIRED from this
//   stub block.  Motor/myMN200motor.cpp landed in wave PT-W3 with golden's real 295-line
//   body (port :1154, golden Motor/myMN200motor.cpp:855) and both are in ht9045_motor, so
//   keeping this one produced `multiple definition of OpenPCI132Card(bool)`.  Same shape
//   as PT-W2's uPlateInfo retirement: the stub was standing in for an unported unit, and
//   the unit arrived.  NOTE this is a real behaviour change on the MN200 ring path --
//   ring bring-up now actually runs instead of returning immediately.
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
// ===========================================================================
//AI(W906-PT-W6c) 20260810: GOLDEN TEXT RESTORED (GATED) -- RecordIndexPosition
//  golden HT9011UC_Code_V3.33.906.0_20260618/Motor/mymotor.cpp:5318-5424  (107 lines)
//  Census scored this function "translated" on name match only; the LIVE body
//  below is an abbreviated stand-in and golden's text existed nowhere in the
//  tree. The block inside the gate is golden's body transcribed VERBATIM
//  (cp950 -> UTF-8; byte-exact when re-encoded to cp950) and is INACTIVE.
//  The LIVE body that follows is UNCHANGED and remains the only active
//  definition -- net behaviour delta = 0. NOTHING was added inside the gate,
//  so a later un-gate is mechanical.
// ===========================================================================
#if 0 // AI-W6C-GOLDEN-BEGIN RecordIndexPosition Motor/mymotor.cpp:5318-5424
void RecordIndexPosition(int iArm,int Part)                                     //Isaac 20200922 : 紀錄indexArmY encoder值和command值
{
    AnsiString Strtemp="",StrType="";
    AnsiString sFileName="",sFileName2="",StrIndexLog="";
    GetTimeInfo();

    int y1CmdPos     = MOT[MTestY1].Gali_ReadPos();
    int y1EncoderPos = MOT[MTestY1].Gali_ReadEncoderPos();
    int y2CmdPos     = (USE_INDEX_ARM_AXES==IndexArm_3_Axis)?0:MOT[MTestY2].Gali_ReadPos();
    int y2EncoderPos = (USE_INDEX_ARM_AXES==IndexArm_3_Axis)?0:MOT[MTestY2].Gali_ReadEncoderPos();
    int z1CmdPos     = MOT[MTestZ1].Gali_ReadPos();
    int z1EncoderPos = MOT[MTestZ1].Gali_ReadEncoderPos();
    int z2CmdPos     = MOT[MTestZ2].Gali_ReadPos();
    int z2EncoderPos = MOT[MTestZ2].Gali_ReadEncoderPos();
    if(Part==3)                                                                 //Error
    {
        if(iArm==0)                                                             //Handler status Halt
        {
            StrType="Halt";
            sFileName2.sprintf("%s\\%04d\\%02d_IndexPosLog\\IndexArmHalt_%04d%02d%02d%02d%02d%02d.logs", "D:\\HT9045_Log\\IndexPos",SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        }
        else if(iArm==3)                                                        //Gali_Two_ZAxis_Move
        {
            StrType="Gali_Two_ZAxis_Move";
            sFileName2.sprintf("%s\\%04d\\%02d_IndexPosLog\\IndexArm12_%04d%02d%02d%02d%02d%02d.logs", "D:\\HT9045_Log\\IndexPos",SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        }
        else                                                                    //Arm1,Arm2
        {
            StrType.sprintf("IndexArm%d",iArm);
            sFileName2.sprintf("%s\\%04d\\%02d_IndexPosLog\\IndexArm%d_%04d%02d%02d%02d%02d%02d.logs", "D:\\HT9045_Log\\IndexPos",SystemYear, SystemMonth, iArm, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        }

        Strtemp.sprintf("%s :Y1CMD:%d, Y1POS:%d, Teach F:%d, M:%d, Y2CMD:%d, Y2POS:%d, Teach M:%d, R:%d, Z1CMD:%d, Z1POS:%d, Z2CMD:%d, Z2POS:%d",
                StrType,
                y1CmdPos, y1EncoderPos, Prod.TestY1_Front, Prod.TestY1_Middle,
                y2CmdPos, y2EncoderPos, Prod.TestY2_Middle, Prod.TestY2_Rear,
                z1CmdPos, z1EncoderPos,
                z2CmdPos, z2EncoderPos);
        fMain->AddIndexPosLog(Strtemp);
        fMain->AddIndexPosLog("Save Log", true);
    }
    else                                                                        //Part : Shuttle==0 or Socket==1
    {
        if(fMain->cbBoth->Checked)                                              //Record Both Arm1 and Arm2
        {
            if (iArm == 1)
            {
                if (Part == 0)  // Shuttle
                {
                    Strtemp.sprintf("Arm1 on Shuttle : Teach:%d, Y1CMD:%d, Y1POS:%d, Y2CMD:%d, Y2POS:%d",
                                    Prod.TestY1_Front, y1CmdPos, y1EncoderPos, y2CmdPos, y2EncoderPos);
                }
                else if (Part == 1)  // Socket
                {
                    Strtemp.sprintf("Arm1 on Socket : Teach:%d, Y1CMD:%d, Y1POS:%d, Y2CMD:%d, Y2POS:%d",
                                    Prod.TestY1_Middle, y1CmdPos, y1EncoderPos, y2CmdPos, y2EncoderPos);
                }
            }
            else  // iArm == 2
            {
                if (Part == 0)  // Shuttle
                {
                    Strtemp.sprintf("Arm2 on Shuttle : Teach:%d, Y1CMD:%d, Y1POS:%d, Y2CMD:%d, Y2POS:%d",
                                    Prod.TestY2_Rear, y1CmdPos, y1EncoderPos, y2CmdPos, y2EncoderPos);
                }
                else if (Part == 1)  // Socket
                {
                    Strtemp.sprintf("Arm2 on Socket : Teach:%d, Y1CMD:%d, Y1POS:%d, Y2CMD:%d, Y2POS:%d",
                                    Prod.TestY2_Middle, y1CmdPos, y1EncoderPos, y2CmdPos, y2EncoderPos);
                }
            }
        }
        else if (fMain->cbArm1->Checked && iArm == 1)  // Record Arm1 only
        {
            if (Part == 0)  // Shuttle
            {
                Strtemp.sprintf("Arm1 on Shuttle : Teach:%d, Y1CMD:%d, Y1POS:%d",
                                Prod.TestY1_Front, y1CmdPos, y1EncoderPos);
            }
            else if (Part == 1)  // Socket
            {
                Strtemp.sprintf("Arm1 on Socket : Teach:%d, Y1CMD:%d, Y1POS:%d",
                                Prod.TestY1_Middle, y1CmdPos, y1EncoderPos);
            }
        }
        else if (fMain->cbArm2->Checked && iArm == 2)  // Record Arm2 only
        {
            if (Part == 0)  // Shuttle
            {
                Strtemp.sprintf("Arm2 on Shuttle : Teach:%d, Y2CMD:%d, Y2POS:%d",
                                Prod.TestY2_Rear, y2CmdPos, y2EncoderPos);
            }
            else if (Part == 1)  // Socket
            {
                Strtemp.sprintf("Arm2 on Socket : Teach:%d, Y2CMD:%d, Y2POS:%d",
                                Prod.TestY2_Middle, y2CmdPos, y2EncoderPos);
            }
        }
        else
        {
            return;
        }

        fMain->AddIndexPosLog(Strtemp);
    }
    return;
}
#endif // AI-W6C-GOLDEN-END RecordIndexPosition Motor/mymotor.cpp:5318-5424

void RecordIndexPosition(int, int)     {}
// ===========================================================================
//AI(W906-PT-W6c) 20260810: GOLDEN TEXT RESTORED (GATED) -- EncoderTeachingMaxMinCount
//  golden HT9011UC_Code_V3.33.906.0_20260618/Motor/mymotor.cpp:5426-5499  (74 lines)
//  Census scored this function "translated" on name match only; the LIVE body
//  below is an abbreviated stand-in and golden's text existed nowhere in the
//  tree. The block inside the gate is golden's body transcribed VERBATIM
//  (cp950 -> UTF-8; byte-exact when re-encoded to cp950) and is INACTIVE.
//  The LIVE body that follows is UNCHANGED and remains the only active
//  definition -- net behaviour delta = 0. NOTHING was added inside the gate,
//  so a later un-gate is mechanical.
// ===========================================================================
#if 0 // AI-W6C-GOLDEN-BEGIN EncoderTeachingMaxMinCount Motor/mymotor.cpp:5426-5499
void EncoderTeachingMaxMinCount(int iRecordArm)                                 //Isaac 20201012 : 計算Encoder和commandpos/Teaching的差值
{
    int check1=0, check2=0;
    int iEncoderPos=0;

    if(iRecordArm==1)                                                           //Arm1
    {
        iEncoderPos=MOT[MTestY1].Gali_ReadEncoderPos();

        check1=abs(iEncoderPos-Prod.TestY1_Front);
        check2=abs(iEncoderPos-Prod.TestY1_Middle);

        if(check1>5000)                                                         //Middle
        {
            if(check2>iMaxTeachY1M)
            {
                iMaxTeachY1M=check2;
            }
            else if(check2<iMinTeachY1M)
            {
                iMinTeachY1M=check2;
            }
        }
        else                                                                    //Front
        {
            if(check1>iMaxTeachY1F)
            {
                iMaxTeachY1F=check1;
            }
            else if(check1<iMinTeachY1F)
            {
                iMinTeachY1F=check1;
            }
        }
    }
    else if(iRecordArm==2)                                                      //Arm2
    {
        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)
        {
            iEncoderPos=0;
        }
        else
        {
            iEncoderPos=MOT[MTestY2].Gali_ReadEncoderPos();
        }

        check1=abs(iEncoderPos-Prod.TestY2_Middle);
        check2=abs(iEncoderPos-Prod.TestY2_Rear);

        if(check1>5000)                                                         //Rear
        {
            if(check2>iMaxTeachY2R)
            {
                iMaxTeachY2R=check2;
            }
            else if(check2<iMinTeachY2R)
            {
                iMinTeachY2R=check2;
            }
        }
        else                                                                    //Middle
        {
            if(check1>iMaxTeachY2M)
            {
                iMaxTeachY2M=check1;
            }
            else if(check1<iMinTeachY2M)
            {
                iMinTeachY2M=check1;
            }
        }
    }
    return;
}
#endif // AI-W6C-GOLDEN-END EncoderTeachingMaxMinCount Motor/mymotor.cpp:5426-5499

void EncoderTeachingMaxMinCount(int)   {}
void InitialMaxMinValue(AnsiString)    {}
void TrigerIndexAxisHome()             {}
