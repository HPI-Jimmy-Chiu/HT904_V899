// =============================================================================
//  tests/test_sim_motor.cpp  -- W4 HARDWARE INTERFACE-CUT acceptance test
//
//  Purpose
//  -------
//  Prove that the offline motor HAL works with ZERO hardware and ZERO vendor
//  SDK.  The translated abstract port (HTMotor) plus the additive offline
//  adapter (TMySimMotor) must give a fully exercisable motor through the SAME
//  base-class pointer the production code uses (HTMotor*), and through the
//  routing wrapper (TMyMotor) whose `Motor` field is an HTMotor*.
//
//  This is the explicit W4 "interface-cut" proof:
//    * The brand drivers (SMC / Galil / MN200 / SYNTEK / EtherCAT / MC88X1) call
//      vendor functions (SmcW* / DMCCommand / mn_* / Acm_* ...) that cannot link
//      without the vendor SDK.  Those .cpp files are NOT compiled into
//      ht9045_motor (deferred to W4-part2; vendor calls additionally gated by
//      HAVE_xxx, default OFF).
//    * TMySimMotor substitutes for any brand driver behind the SAME HTMotor*
//      interface, so the motor layer compiles, links and RUNS offline.
//
//  Acceptance assertions (mirror the task wording exactly):
//    InitMotor ok
//    HomeObject() -> HomeFlag() true & ReadPos()==0
//    MoveToPos(N) -> MotionDone() true & ReadPos()==N
//    GetAlarm()==false
//    Stop() ok
//
//  Faithfulness note: ReadPos()/MoveToPos()/HomeObject() carry the BCB6 numeric
//  pulse semantics (int pulses; HomeObject parks at pulse 0).  No re-scaling.
// =============================================================================
#include "Motor/HTMotor.h"
#include "Motor/mySimMotor.h"
#include "Motor/mymotor.h"
#include "MachineType.h"        // motor name constants (MInArmX ...)
#include <cstdio>

// --------------------------------------------------------------------------
//  Tiny PASS / FAIL harness (same style as the other W-series tests)
// --------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                            \
    do {                                                            \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }        \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// Door-closed callback so TMyMotor motion helpers are not gated by the
// safe-door interlock (Enable==true + NULL callback => door treated as open).
static bool sim_door_closed(void) { return false; }

// --------------------------------------------------------------------------
//  Part A -- TMySimMotor exercised purely through the abstract port HTMotor*
//
//  This is the heart of the interface-cut: production code only ever holds an
//  HTMotor*.  Everything below dispatches virtually to the sim adapter.
// --------------------------------------------------------------------------
static void test_via_abstract_port()
{
    printf("\n[A] TMySimMotor through HTMotor* (the interface-cut)\n");

    HTMotor *m = new TMySimMotor();   // upcast to the abstract port

    // --- InitMotor ok -----------------------------------------------------
    int rc = m->InitMotor(0x300);
    CHECK(rc == 0,                 "InitMotor() returns 0 (ok)");
    CHECK(m->Enable == true,       "motor Enable==true after InitMotor");
    CHECK(m->ReadPos() == 0,       "ReadPos()==0 after InitMotor (fresh)");

    // --- GetAlarm()==false (offline sim never faults) ---------------------
    CHECK(m->GetAlarm() == false,  "GetAlarm()==false");

    // --- MoveToPos(N) -> MotionDone() true & ReadPos()==N -----------------
    bool moved = m->MoveToPos(42000);
    CHECK(moved == true,           "MoveToPos(42000) returns true");
    CHECK(m->MotionDone() == true, "MotionDone()==true after MoveToPos");
    CHECK(m->ReadPos() == 42000,   "ReadPos()==42000 after MoveToPos(42000)");

    // A second, negative target proves the position register tracks faithfully
    m->MoveToPos(-1750);
    CHECK(m->ReadPos() == -1750,   "ReadPos()==-1750 after MoveToPos(-1750)");
    CHECK(m->MotionDone() == true, "MotionDone()==true after second move");

    // --- HomeObject() -> HomeFlag() true & ReadPos()==0 -------------------
    bool homed = m->HomeObject();
    CHECK(homed == true,           "HomeObject() returns true (home found)");
    CHECK(m->HomeFlag() == true,   "HomeFlag()==true after HomeObject");
    CHECK(m->ReadPos() == 0,       "ReadPos()==0 after HomeObject (parked at 0)");

    // --- Stop() ok (no inertia in sim; must not crash, position unchanged) -
    m->MoveToPos(900);
    m->Stop();
    CHECK(m->ReadPos() == 900,     "Stop() leaves position intact (no crash)");
    m->DecStop();
    CHECK(true,                    "DecStop() ok (no crash)");

    // --- GetAlarm() still false after motion ------------------------------
    CHECK(m->GetAlarm() == false,  "GetAlarm()==false after motion");

    delete m;                      // virtual dtor through the port
    CHECK(true,                    "delete via HTMotor* (virtual dtor) ok");
}

// --------------------------------------------------------------------------
//  Part B -- the same sim driven through the production wrapper TMyMotor
//
//  TMyMotor::Motor is an HTMotor*.  cinitial wires a brand driver there in the
//  real build; here we wire a TMySimMotor.  The wrapper's routing methods must
//  observe the identical offline behavior, proving the cut holds one layer up.
// --------------------------------------------------------------------------
static void test_via_tmymotor()
{
    printf("\n[B] TMySimMotor through TMyMotor wrapper\n");

    TMyMotor    mot;
    TMySimMotor sim;

    mot.Motor = &sim;                                  // inject the sim adapter
    mot.Motor->Enable = true;
    mot.Motor->MotorIdleSafeDoorCheck = sim_door_closed;
    mot.SetAlias(MInArmX, "SimInArmX");

    // InitMotor through the port the wrapper holds
    CHECK(mot.Motor->InitMotor(0x300) == 0, "TMyMotor.Motor->InitMotor() ok");

    // MoveToPos(N) on the sim -> TMyMotor::ReadPos routes through and caches it
    mot.Motor->MoveToPos(8800);
    CHECK(mot.Motor->MotionDone() == true, "MotionDone()==true via wrapper port");
    CHECK(mot.ReadPos() == 8800,           "TMyMotor::ReadPos()==8800 (routed)");
    CHECK(mot.Position == 8800,            "TMyMotor::Position cached after ReadPos");

    // GetMotorAlarm routes to Motor->GetAlarm() for a non-Galil/sim motor
    CHECK(mot.GetMotorAlarm() == false,    "TMyMotor::GetMotorAlarm()==false");

    // HomeObject through the port, then verify wrapper sees home/zero
    CHECK(mot.Motor->HomeObject() == true, "Motor->HomeObject()==true");
    CHECK(mot.Motor->HomeFlag() == true,   "Motor->HomeFlag()==true");
    CHECK(mot.ReadPos() == 0,              "TMyMotor::ReadPos()==0 after HomeObject");

    // Stop through the wrapper's PCIL132 path (non-index -> Motor->DecStop())
    mot.Motor->MoveToPos(300);
    mot.PCIL132_StopMotor();
    CHECK(mot.ReadPos() == 300,            "PCIL132_StopMotor: position intact");

    // ScanMotorStatus must report healthy / in-position / no-alarm leds
    mot.ScanMotorStatus();
    CHECK(mot.Led[iInposLed]      == true,  "ScanMotorStatus Led[iInposLed]=true");
    CHECK(mot.Led[iServoOn]       == true,  "ScanMotorStatus Led[iServoOn]=true");
    CHECK(mot.Led[iAlarmLed]      == false, "ScanMotorStatus Led[iAlarmLed]=false");
    CHECK(mot.Led[iServoalarmLed] == false, "ScanMotorStatus Led[iServoalarmLed]=false");
}

// --------------------------------------------------------------------------
//  Part C -- full MotorHome state machine over the sim
//
//  This is the strongest offline proof: the translated MotorHome() multi-tick
//  state machine drives HomeObject()/HomeFlag() on the sim to a clean success
//  (return 1) with NO hardware.  Mirrors test_motor_w4 but kept here so this
//  dedicated interface-cut test is self-contained.
// --------------------------------------------------------------------------
static void test_motor_home_via_sim()
{
    printf("\n[C] TMyMotor::MotorHome() state machine over sim\n");

    TMyMotor    mot;
    TMySimMotor sim;
    mot.Motor = &sim;
    mot.Motor->Enable = true;
    mot.Motor->MotorIdleSafeDoorCheck = sim_door_closed;
    mot.SetAlias(MInArmX, "SimHome");

    // MotorInitial arms the ResetTime watchdog so the first tick does not
    // mis-fire the 90s timeout (an unstarted TQPF_Timer::Off() reads true).
    mot.MotorInitial();

    int result = 0, iter = 0;
    const int kMaxIter = 32;
    do {
        result = mot.MotorHome(false);
    } while (result == 0 && ++iter < kMaxIter);

    CHECK(result == 1,           "MotorHome() converges to 1 (success) offline");
    CHECK(mot.HomeFlag == 1,     "TMyMotor::HomeFlag latched to 1");
    CHECK(mot.fCanMove == true,  "fCanMove restored after home");
    CHECK(sim.ReadPos() == 0,    "sim parked at pulse 0 after home");
}

// --------------------------------------------------------------------------
//  main
// --------------------------------------------------------------------------
int main()
{
    printf("=== W4 sim-motor interface-cut acceptance test ===\n");
    printf("(NO vendor SDK, NO hardware -- pure HTMotor* + TMySimMotor)\n");

    test_via_abstract_port();
    test_via_tmymotor();
    test_motor_home_via_sim();

    printf("\n=== Results: %d passed, %d failed ===\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
