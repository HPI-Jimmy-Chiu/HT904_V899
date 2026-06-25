// =============================================================================
//  tests/test_motor_w4.cpp  -- W4 HAL motor layer smoke test
//
//  Exercises the INTERFACE-CUT end-to-end with NO vendor SDK:
//    1. TMySimMotor polymorphic dispatch through HTMotor*.
//    2. TMyMotor routing methods (ReadPos / MotorHome / GetMotorAlarm /
//       ScanMotorStatus / GetErrorIndex / PCIL132_SetPos / JogP/JogN /
//       Lock/UnLock / SetSpeed / SetADCRate / GetRealPos etc.)
//    3. TTrayMotor tray data helpers (SetTraySingleData / HasIC / FullIC /
//       UpHalfIsFull / HowManyDevice / MoveTrayAllItem / ClearTray).
//    4. MOT[] global array: basic MOT[0] wiring.
// =============================================================================
#include "Motor/mySimMotor.h"
#include "Motor/mymotor.h"
#include "MachineType.h"        // motor name constants (MInArmX, MTestY1, etc.)
#include <cassert>
#include <cstdio>

// --------------------------------------------------------------------------
//  Safe-door callback for tests: always returns false (door closed / safe).
//  Assigned to Motor->MotorIdleSafeDoorCheck so JogP / MotorHome / Home
//  do not abort early.  Without this, Enable==true causes CheckIsSafeDoorOpen
//  to return true (door open), blocking motion in the test.
// --------------------------------------------------------------------------
static bool sim_door_closed(void) { return false; }

// --------------------------------------------------------------------------
//  Tiny PASS / FAIL helpers
// --------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg) \
    do { \
        if (cond) { \
            printf("  PASS: %s\n", msg); ++g_pass; \
        } else { \
            printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; \
        } \
    } while(0)

// --------------------------------------------------------------------------
//  Section 1 -- TMySimMotor as HTMotor* (pure virtual interface)
// --------------------------------------------------------------------------
static void test_simmotor_as_port()
{
    printf("\n[1] TMySimMotor as HTMotor* (polymorphic)\n");
    HTMotor *m = new TMySimMotor();

    // Verify defaults set in TMySimMotor ctor
    CHECK(m->Enable == true,    "Enable==true after ctor");
    CHECK(m->GearRatio == 1.0,  "GearRatio==1.0 (HTMotor default)");

    // InitMotor resets and returns 0 (success)
    int r = m->InitMotor(0x340);
    CHECK(r == 0,               "InitMotor returns 0");
    CHECK(m->Enable == true,    "Enable still true after InitMotor");

    // ReadPos after ctor = 0
    CHECK(m->ReadPos() == 0,    "ReadPos()==0 initially");

    // MoveToPos -- instant
    m->MoveToPos(5000);
    CHECK(m->ReadPos() == 5000, "ReadPos()==5000 after MoveToPos(5000)");

    // MotionDone always true
    CHECK(m->MotionDone() == true, "MotionDone()==true");

    // HomeObject -> position resets to 0
    m->HomeObject();
    CHECK(m->ReadPos() == 0,    "ReadPos()==0 after HomeObject()");

    // HomeFlag always true
    CHECK(m->HomeFlag() == true, "HomeFlag()==true");

    // GetAlarm -- no alarms in sim
    CHECK(m->GetAlarm() == false, "GetAlarm()==false");

    // ReadEnCoderRealPos == ReadPos in sim
    m->MoveToPos(1234);
    CHECK(m->ReadEnCoderRealPos() == 1234, "ReadEnCoderRealPos==1234");
    // ReadEncoderPos = ReadEnCoderRealPos * GearRatio (concrete in HTMotor)
    CHECK(m->ReadEncoderPos() == 1234,     "ReadEncoderPos()==1234 (GearRatio=1)");

    // GearRatio != 1
    m->GearRatio = 2.0;
    m->MoveToPos(100);
    CHECK(m->ReadEncoderPos() == 200,      "ReadEncoderPos()==200 (GearRatio=2)");
    m->GearRatio = 1.0;

    // JogP / JogN
    m->MoveToPos(0);
    m->JogP();
    CHECK(m->ReadPos() == MYSIM_JOG_DELTA, "JogP steps by MYSIM_JOG_DELTA");
    m->JogN();
    CHECK(m->ReadPos() == 0,               "JogN restores to 0");

    // ScanMotorStatus
    bool Led[10] = {};
    m->ScanMotorStatus(Led);
    CHECK(Led[iInposLed] == true,   "ScanMotorStatus: iInposLed=true");
    CHECK(Led[iServoOn]  == true,   "ScanMotorStatus: iServoOn=true");
    CHECK(Led[iAlarmLed] == false,  "ScanMotorStatus: iAlarmLed=false");
    CHECK(Led[iCwLed]    == false,  "ScanMotorStatus: iCwLed=false");
    CHECK(Led[iCcwLed]   == false,  "ScanMotorStatus: iCcwLed=false");

    // ResetPos
    m->MoveToPos(9999);
    m->ResetPos(0);
    CHECK(m->ReadPos() == 0,        "ResetPos(0) -> ReadPos()==0");

    // SetCommand / SetPosition
    m->SetCommand(777);
    CHECK(m->ReadPos() == 777,      "SetCommand(777) -> ReadPos()==777");
    m->SetPosition(888);
    CHECK(m->ReadPos() == 888,      "SetPosition(888) -> ReadPos()==888");

    // Stop / DecStop -- no-op, no crash
    m->Stop();
    m->DecStop();
    CHECK(true, "Stop()/DecStop() no-op");

    // CheckIsSafeDoorOpen: MotorIdleSafeDoorCheck==NULL, Enable==true -> returns true
    CHECK(m->CheckIsSafeDoorOpen() == true, "CheckIsSafeDoorOpen returns true when Enable");
    m->Enable = false;
    CHECK(m->CheckIsSafeDoorOpen() == false, "CheckIsSafeDoorOpen returns false when !Enable");
    m->Enable = true;

    delete m;
}

// --------------------------------------------------------------------------
//  Section 2 -- TMyMotor routing through TMySimMotor
// --------------------------------------------------------------------------
static void test_mymotor_routing()
{
    printf("\n[2] TMyMotor routing through TMySimMotor\n");

    TMyMotor mot;
    TMySimMotor sim;

    // Wire the sim motor
    mot.Motor    = &sim;
    mot.Motor->Enable = true;
    mot.Motor->MotorIdleSafeDoorCheck = sim_door_closed;  // door closed -> motion allowed
    mot.SetAlias(MInArmX, "MInArmX");

    CHECK(mot.Alias == "MInArmX", "SetAlias stores Alias");
    CHECK(mot.Mot_Name == MInArmX, "SetAlias stores Mot_Name");

    // ReadPos routes through Motor->ReadPos()
    sim.MoveToPos(3000);
    int p = mot.ReadPos();
    CHECK(p == 3000, "TMyMotor::ReadPos routes to sim (3000)");
    CHECK(mot.Position == 3000, "TMyMotor::Position updated");

    // ReadEncoderPos
    int ep = mot.ReadEncoderPos();
    CHECK(ep == 3000, "TMyMotor::ReadEncoderPos routes to sim");

    // SetSpeed (non-index axis, p < 100)
    mot.Motor->PJogHighSpeed = 1000;
    mot.Motor->PJogLowSpeed  = 0;
    mot.SetSpeed(50.0);
    CHECK(mot.speed == 500, "SetSpeed(50%) -> speed=500 (JogHighSpeed*50/100)");

    // GetSpeed -- SetSpeed stores to protected iSpeed; read via ReadSpeed()
    mot.Motor->SetSpeed(500);
    int gs = mot.GetSpeed();
    // TMySimMotor::SetSpeed stores x to iSpeed; ReadSpeed() returns iSpeed.
    CHECK(gs == 500, "GetSpeed() routes to Motor->ReadSpeed()");

    // EnableMotorMove
    mot.fCanMove  = false;
    mot.fCanMoveR = false;
    mot.EnableMotorMove();
    CHECK(mot.fCanMove && mot.fCanMoveR, "EnableMotorMove restores fCanMove flags");

    // IsCanMove
    CHECK(mot.IsCanMove() == true, "IsCanMove() == true after EnableMotorMove");

    // GetMotorAlarm (non-index motor -> Motor->GetAlarm())
    CHECK(mot.GetMotorAlarm() == false, "GetMotorAlarm()==false for sim");

    // ScanMotorStatus
    mot.ScanMotorStatus();
    CHECK(mot.Led[iInposLed] == true,  "ScanMotorStatus: Led[iInposLed]=true");
    CHECK(mot.Led[iServoOn]  == true,  "ScanMotorStatus: Led[iServoOn]=true");

    // GetErrorIndex: all-good leds -> returns 9 (no match)
    mot.Led[iAlarmLed]      = false;
    mot.Led[iServoalarmLed] = false;
    mot.Led[iCwLed]         = false;
    mot.Led[iCcwLed]        = false;
    mot.Led[iSoftcwLed]     = false;
    mot.Led[iSoftccwLed]    = false;
    mot.Led[iInposLed]      = true;
    CHECK(mot.GetErrorIndex() == 9, "GetErrorIndex() == 9 for clean leds");

    // GetErrorIndex: alarm + CW -> 2
    mot.Led[iAlarmLed] = true;
    mot.Led[iCwLed]    = true;
    CHECK(mot.GetErrorIndex() == 2, "GetErrorIndex() == 2 for alarm+CW");

    // PCIL132_SetPos
    mot.PCIL132_SetPos(1500);
    CHECK(sim.ReadPos() == 1500, "PCIL132_SetPos(1500) -> sim.ReadPos()==1500");

    // PCIL132_StopMotor (non-index motor -> Motor->DecStop())
    mot.PCIL132_StopMotor();
    CHECK(true, "PCIL132_StopMotor no crash");

    // JogP / JogN
    sim.MoveToPos(0);
    mot.JogP(100);
    CHECK(sim.ReadPos() == MYSIM_JOG_DELTA, "TMyMotor::JogP steps sim");
    mot.JogN(100);
    CHECK(sim.ReadPos() == 0, "TMyMotor::JogN restores sim");

    // SetArmMaxSpeed (delegates to Motor->SetArmMaxSpeed, which is no-op in sim)
    mot.SetArmMaxSpeed();
    CHECK(true, "SetArmMaxSpeed no crash");

    // InitMOTParameter
    mot.fCMD = true;
    mot.InitMOTParameter();
    CHECK(mot.fCMD == false, "InitMOTParameter resets fCMD");

    // SetScreenScale
    mot.Position = 5000;
    mot.SetScreenScale(0, 100, 0, 10000);
    // Scale = (0-100)/(0-10000) = 0.01; ScreenPos = 0.01*(5000-0)+0 = 50
    CHECK(mot.ScreenPos == 50, "SetScreenScale -> ScreenPos computed correctly");
}

// --------------------------------------------------------------------------
//  Section 3 -- TMyMotor MotorHome state machine
// --------------------------------------------------------------------------
static void test_motor_home()
{
    printf("\n[3] TMyMotor::MotorHome state machine\n");

    TMyMotor mot;
    TMySimMotor sim;
    mot.Motor = &sim;
    mot.Motor->Enable = true;
    mot.Motor->MotorIdleSafeDoorCheck = sim_door_closed;  // door closed -> motion allowed
    mot.SetAlias(MInArmX, "MInArmX_home");

    // MotorInitial() arms ResetTime (90 s timeout) and resets the home task.
    // Without this, ResetTime.Off() fires immediately (unstarted timer returns
    // true) and MotorHome returns 2 (timeout) on the very first tick.
    mot.MotorInitial();

    // Drive MotorHome to completion.
    // Sim: HomeObject() returns true (instant), HomeFlag() always true.
    // MotorHome starts at Task=1 -> Task=10 (calls Home()->HomeObject) ->
    // Task=20 (polls HomeFlag()) -> returns 1 (success) next tick.
    int result = 0;
    const int kMaxIter = 20;
    int iter = 0;
    do {
        result = mot.MotorHome(false);
        iter++;
    } while (result == 0 && iter < kMaxIter);

    CHECK(result == 1,        "MotorHome() returns 1 (success) with sim");
    CHECK(mot.HomeFlag == 1,  "HomeFlag set to 1 after successful home");
    CHECK(mot.fCanMove == true, "fCanMove restored after home");
}

// --------------------------------------------------------------------------
//  Section 4 -- Lock / UnLock / ClearLock
// --------------------------------------------------------------------------
static void test_lock()
{
    printf("\n[4] TMyMotor Lock / UnLock / ClearLock\n");

    TMyMotor mot;
    TMySimMotor sim;
    mot.Motor = &sim;

    CHECK(mot.GetLockCount() == 0, "GetLockCount()==0 initially");

    mot.Lock("MInArmX", "TestFunc", 1);
    CHECK(mot.GetLockCount() == 1, "GetLockCount()==1 after Lock");

    mot.Lock("MInArmY", "TestFunc2", 2);
    CHECK(mot.GetLockCount() == 2, "GetLockCount()==2 after second Lock");

    AnsiString s = mot.GetLockString(0);
    CHECK(s.Length() > 0, "GetLockString(0) non-empty");

    mot.UnLock("MInArmX", "TestFunc");
    CHECK(mot.GetLockCount() == 1, "GetLockCount()==1 after UnLock one");

    mot.ClearLock();
    CHECK(mot.GetLockCount() == 0, "GetLockCount()==0 after ClearLock");
}

// --------------------------------------------------------------------------
//  Section 5 -- GetRealPos (GearRatio conversion)
// --------------------------------------------------------------------------
static void test_get_real_pos()
{
    printf("\n[5] TMyMotor::GetRealPos (GearRatio conversion)\n");

    TMyMotor mot;
    TMySimMotor sim;
    mot.Motor = &sim;
    mot.Motor->Enable = true;
    mot.Mot_Name = MInArmX;

    // GearRatio=1: GetRealPos should be identity
    sim.MoveToPos(0);
    mot.Motor->GearRatio = 1.0;
    int pos = 1000;
    mot.GetRealPos(&pos);
    CHECK(pos == 1000, "GetRealPos: GearRatio=1 -> pos unchanged");

    // GearRatio=0.6: 1000 logical -> ~1667 pulses
    // ChangeToFloatNonPcnt(1000, 0.6) = 1000/0.6 ~= 1666 or 1667
    mot.Motor->GearRatio = 0.6;
    pos = 1000;
    mot.GetRealPos(&pos);
    // Acceptable: pos*GearRatio must >= 1000
    int check = (int)((double)pos * 0.6);
    CHECK(check >= 1000, "GetRealPos: GearRatio=0.6 -> pos*0.6 >= 1000");
}

// --------------------------------------------------------------------------
//  Section 6 -- TTrayMotor tray data
// --------------------------------------------------------------------------
static void test_tray_motor()
{
    printf("\n[6] TTrayMotor tray data helpers\n");

    TTrayMotor tm;
    TMySimMotor sim;
    tm.Motor = &sim;
    tm.Motor->Enable = true;
    tm.SetAlias(MLoaderZ, "Loader");

    // Initial state: no tray
    CHECK(tm.fHasTray == false, "fHasTray==false initially");
    CHECK(tm.HasIC()   == false, "HasIC()==false with no tray");

    // SetXYItem
    tm.Tray.SetXYItem(4, 2);
    CHECK(tm.Tray.XItem == 4, "XItem==4 after SetXYItem");
    CHECK(tm.Tray.YItem == 2, "YItem==2 after SetXYItem");

    // InitNewTray with HAS_IC
    tm.InitNewTray(HAS_IC, false, "test");
    tm.fHasTray = true;

    CHECK(tm.HasIC() == true,  "HasIC()==true after InitNewTray(HAS_IC)");
    CHECK(tm.FullIC() == true, "FullIC()==true after InitNewTray(HAS_IC)");

    // SetTraySingleData to NULL_IC for one slot
    tm.SetTraySingleData(0, 0, NULL_IC);
    CHECK(tm.FullIC() == false, "FullIC()==false after one slot NULL_IC");
    CHECK(tm.HasIC()  == true,  "HasIC()==true after one slot NULL_IC (rest still HAS_IC)");

    // HowManyDevice(HAS_IC)
    int cnt = tm.HowManyDevice(HAS_IC);
    // 4x2=8 slots, one is NULL_IC -> 7 HAS_IC
    CHECK(cnt == 7, "HowManyDevice(HAS_IC)==7 (7 out of 8 slots)");

    // HowManyDevice() (not NULL_IC or HAS_NULL_IC)
    int total = tm.HowManyDevice();
    CHECK(total == 7, "HowManyDevice()==7");

    // UpHalfIsFull / DownHalfIsFull (YItem=2, half=1 row)
    // Row 0: mixed (slot [0][0]=NULL_IC, [1..3][0]=HAS_IC)
    // Row 1: all HAS_IC
    // UpHalf = rows 0..(YItem/2-1) = row 0 -- not full (slot [0][0]=NULL_IC)
    CHECK(tm.UpHalfIsFull()   == false, "UpHalfIsFull()==false (row 0 has NULL_IC)");
    CHECK(tm.DownHalfIsFull() == true,  "DownHalfIsFull()==true (row 1 all HAS_IC)");

    // SetNullIcToHasNullIc
    tm.SetNullIcToHasNullIc();
    CHECK(tm.Tray.Data[0][0] == HAS_NULL_IC, "SetNullIcToHasNullIc converts NULL_IC->HAS_NULL_IC");
    CHECK(tm.HasIC() == true,  "HasIC()==true after SetNullIcToHasNullIc (HAS_NULL_IC counts)");

    // ClearTray
    tm.ClearTray("test_clear");
    CHECK(tm.fHasTray == false, "fHasTray==false after ClearTray");
    CHECK(tm.HasIC()   == false, "HasIC()==false after ClearTray");

    // MoveTrayAllItem
    TTrayMotor src;
    src.Tray.SetXYItem(2, 2);
    src.InitNewTray(HAS_IC, false, "src_init");
    src.fHasTray = true;
    src.SetAlias(MEmptyZ, "Src");
    tm.Tray.SetXYItem(2, 2);
    tm.MoveTrayAllItem(&src);
    CHECK(tm.fHasTray == true,   "MoveTrayAllItem: dest fHasTray=true");
    CHECK(src.fHasTray == false, "MoveTrayAllItem: src cleared after move");
    CHECK(tm.Tray.Data[0][0] == HAS_IC, "MoveTrayAllItem: data transferred");
}

// --------------------------------------------------------------------------
//  Section 7 -- MOT[] global array
// --------------------------------------------------------------------------
static void test_mot_global()
{
    printf("\n[7] MOT[] global array\n");

    // Wire MOT[MInArmX] with a sim motor
    TMySimMotor *sim = new TMySimMotor();
    MOT[MInArmX].Motor = sim;
    MOT[MInArmX].Motor->Enable = true;
    MOT[MInArmX].SetAlias(MInArmX, "MOT_InArmX");

    sim->MoveToPos(12345);
    int p = MOT[MInArmX].ReadPos();
    CHECK(p == 12345, "MOT[MInArmX].ReadPos()==12345 via sim");

    // Check array boundary: index MAX_TRAY_MOTOR-1 accessible
    MOT[MAX_TRAY_MOTOR - 1].Motor = NULL;
    CHECK(MOT[MAX_TRAY_MOTOR - 1].Motor == NULL,
          "MOT[MAX_TRAY_MOTOR-1] accessible (no crash)");

    delete sim;
    MOT[MInArmX].Motor = NULL;
}

// --------------------------------------------------------------------------
//  main
// --------------------------------------------------------------------------
int main()
{
    printf("=== W4 Motor HAL smoke test ===\n");

    test_simmotor_as_port();
    test_mymotor_routing();
    test_motor_home();
    test_lock();
    test_get_real_pos();
    test_tray_motor();
    test_mot_global();

    printf("\n=== Results: %d passed, %d failed ===\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
