// =============================================================================
//  tests/test_w7_s0_motor_convergence.cpp
//  Suite: W7_S0_MotorConvergence
//
//  W7 ENABLER S0 -- prove the OFFLINE motor "move family" CONVERGES so that
//  motion-bearing state machines (DoInArmPickFromLoadStage, the atester press
//  tree, shuttle/index arrivals, ...) can reach their ARRIVED/Success terminals
//  over the Sim HAL with NO vendor SDK and NO hardware.
//
//  The S0 change (Motor/mymotor.cpp) added a Motor==NULL/!Enable CONVERGENCE
//  SHORTCUT to the four move-family methods: with no real driver attached they
//  snap the stored Position to the commanded target and report success, instead
//  of the old W4 no-op stubs (MotorMove returned 0=Moving forever).
//
//  Oracles (each constructs a TMyMotor with NO attached driver, i.e. the offline
//  Motor==NULL case the offline build always takes):
//    (a) CONVERGE   -- fresh ReadPos()==0; MotorMove(N) returns 1 (success) AND
//                      ReadPos()==N; a second MotorMove(M) -> ReadPos()==M.
//    (b) PICKER     -- MotorMove2SpeedForPicker(FinalPos,...) returns true AND
//                      ReadPos()==FinalPos.
//    (c) HOME       -- MotorHome offline still converges Position->0 (regression
//                      guard; MotorHome offline path was NOT changed by S0).
//    (d) FAST-PATH  -- Gali_Two_ZAxis_Move / ISNormal / GalilTwoY_Move all return
//      NO-REGRESS     true offline (Motor==NULL) -- the atester one-liners are
//                     NOT part of the move family and must be UNCHANGED by S0.
//
//  Golden success semantics (BCB6 mymotor.cpp, cp950-aware):
//    MotorMove return legend :862-869  -> 1 = Move Success
//    MotorMovePosition offline terminal :821-823 -> { Position=Tar; return 1; }
//    MotorMove2SpeedForPicker funnels through MotorMove, returns iFlag(==1) :999
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

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                   \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// --------------------------------------------------------------------------
//  Oracle (a) -- CONVERGE
//
//  With NO driver attached (Motor==NULL) the move family must ARRIVE: a fresh
//  motor reads pulse 0, MotorMove(N) reports Move Success (1) and snaps the
//  stored Position to N, and a second different target tracks faithfully.
// --------------------------------------------------------------------------
static void test_converge()
{
    printf("\n[a] CONVERGE -- MotorMove snaps Position to target offline\n");

    TMyMotor mot;
    mot.Motor = NULL;                   // offline: no driver attached
    mot.SetAlias(MInArmX, "S0_InArmX");

    CHECK(mot.Motor == NULL,            "Motor==NULL (offline, no driver attached)");

    // Establish the documented initial baseline (the TMyMotor ctor does not
    // zero Position; the real Handler homes to 0 before any move).  Offline
    // ReadPos returns the stored Position unchanged.
    mot.Position = 0;
    CHECK(mot.ReadPos() == 0,           "fresh ReadPos()==0 (initial)");

    // MotorMove(N) -> 1 (Move Success) AND the stored position becomes N.
    int rc = mot.MotorMove(50000);
    CHECK(rc == 1,                      "MotorMove(50000) returns 1 (Move Success)");
    CHECK(mot.ReadPos() == 50000,       "ReadPos()==50000 after MotorMove(50000)");

    // A second, different target proves the position register tracks faithfully
    // (the old W4 stub returned 0 forever and never wrote Position).
    int rc2 = mot.MotorMove(-12345);
    CHECK(rc2 == 1,                     "MotorMove(-12345) returns 1 (Move Success)");
    CHECK(mot.ReadPos() == -12345,      "ReadPos()==-12345 after second MotorMove");

    // ScreenPos is refreshed (mirrors ReadPos geometry) -- with default scale
    // (Scale=0 until SetScreenScale) ScreenPos stays at RefStart(0); just prove
    // the call does not crash and a subsequent ReadPos agrees with the target.
    CHECK(mot.ReadPos() == -12345,      "ReadPos() stable after convergence");
}

// --------------------------------------------------------------------------
//  Oracle (b) -- PICKER
//
//  MotorMove2SpeedForPicker is the two-speed pick/place entry every InArm/OutArm
//  Z-index pick funnels through.  Offline it must report true AND arrive at the
//  commanded FinalPos.  ARM_CONDITION* is ignored by the offline shortcut, so a
//  NULL arm pointer is the cleanest offline canary.
// --------------------------------------------------------------------------
static void test_picker()
{
    printf("\n[b] PICKER -- MotorMove2SpeedForPicker arrives offline\n");

    TMyMotor mot;
    mot.Motor = NULL;                   // offline: no driver attached
    mot.SetAlias(MInArmY, "S0_InArmY");

    bool ok = mot.MotorMove2SpeedForPicker(33000, NULL, false);
    CHECK(ok == true,                   "MotorMove2SpeedForPicker(33000,NULL) returns true");
    CHECK(mot.ReadPos() == 33000,       "ReadPos()==33000 after picker move");

    // bIsLoader==true branch must behave identically offline (shortcut is
    // taken before any branch-specific body).
    bool ok2 = mot.MotorMove2SpeedForPicker(7777, NULL, true);
    CHECK(ok2 == true,                  "MotorMove2SpeedForPicker(7777,NULL,true) returns true");
    CHECK(mot.ReadPos() == 7777,        "ReadPos()==7777 after loader-branch picker move");
}

// --------------------------------------------------------------------------
//  Oracle (c) -- HOME (regression guard, unchanged by S0)
//
//  MotorHome offline (Motor->Enable==false branch, case 1) converges Position
//  -> 0 / HomeFlag=1 / return 1.  S0 did NOT touch this path; this oracle locks
//  it so the move-family change cannot silently perturb home convergence.
//
//  Note: with a sim motor attached but Enable==false, MotorHome case 1 takes the
//  `Motor->Enable == false` early-success branch (Position=0, HomeFlag=1, ret 1).
//  We must attach a driver here because MotorHome's FIRST guard returns 0 when
//  Motor==NULL -- the home convergence terminal under test is the Enable==false
//  branch, not the no-driver guard.
// --------------------------------------------------------------------------
static void test_home_unchanged()
{
    printf("\n[c] HOME -- offline MotorHome still converges Position->0\n");

    TMyMotor    mot;
    TMySimMotor sim;
    mot.Motor = &sim;
    mot.Motor->Enable = false;          // disabled => MotorHome case 1 early success

    // MotorInitial arms the home task (iMyHomeTask=1) and the ResetTime watchdog;
    // the TMyMotor ctor leaves iMyHomeTask indeterminate, so this is required
    // setup before pumping MotorHome (same as the W4 / sim_motor home tests).
    mot.MotorInitial();

    // Pre-load a non-zero position so we can see it driven to 0.
    mot.Position = 99999;

    // With Enable==false, MotorHome case 1 takes the early-success branch
    // (Position=0, HomeFlag=1, return 1) on the first tick.
    int rc = mot.MotorHome(false);
    CHECK(rc == 1,                      "MotorHome() returns 1 (success) when !Enable");
    CHECK(mot.Position == 0,            "Position driven to 0 after home convergence");
    CHECK(mot.HomeFlag == 1,            "HomeFlag latched to 1");

    mot.Motor = NULL;                   // detach (sim is stack-local)
}

// --------------------------------------------------------------------------
//  Oracle (d) -- FAST-PATH NO-REGRESSION
//
//  The atester one-liners (Gali_Two_ZAxis_Move / ISNormal / GalilTwoY_Move) are
//  INDEPENDENT of the move family: each is `{ return (Motor==NULL); }`.  They
//  MUST keep returning true offline.  S0 did not touch them -- this oracle is the
//  tripwire that proves it.
// --------------------------------------------------------------------------
static void test_fastpath_no_regression()
{
    printf("\n[d] FAST-PATH NO-REGRESSION -- atester one-liners unchanged\n");

    TMyMotor mot;
    mot.Motor = NULL;                   // offline: no driver attached
    mot.SetAlias(MTestZ1, "S0_TestZ1");

    CHECK(mot.Motor == NULL,            "Motor==NULL (offline) precondition");

    bool z = mot.Gali_Two_ZAxis_Move(1000, 100, "S0", false, 0);
    CHECK(z == true,                    "Gali_Two_ZAxis_Move(...)==true offline (Motor==NULL)");

    bool n = mot.ISNormal();
    CHECK(n == true,                    "ISNormal()==true offline (Motor==NULL)");

    bool y = mot.GalilTwoY_Move(500, 600, 100, "S0");
    CHECK(y == true,                    "GalilTwoY_Move(...)==true offline (Motor==NULL)");
}

// --------------------------------------------------------------------------
//  main
// --------------------------------------------------------------------------
int main()
{
    printf("=== W7 S0 motor-convergence enabler test ===\n");
    printf("(offline Motor==NULL: move family must ARRIVE; fast-paths unchanged)\n");

    test_converge();
    test_picker();
    test_home_unchanged();
    test_fastpath_no_regression();

    printf("\n=== Results: %d passed, %d failed ===\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
