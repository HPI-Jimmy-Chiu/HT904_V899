// =============================================================================
//  test_w906_doindexautoclean.cpp -- W906-AutoCleanCluster (20260728) VERIFY:
//  DoIndexAutoClean + DoIndexAutoClean_Arm1PickArm2Test (the SECOND, final
//  AutoClean dependency cluster) are REAL, faithful translations -- no longer
//  the TEMPORARY empty-body placeholder that used to sit in AutoClean.cpp.
//
//  Translation wave: W906-AutoCleanCluster (this wave, 20260728)
//  Author: AI(W906-AutoCleanCluster) 20260728
//  Suite name (add_test): W906_DoIndexAutoClean
//
//  PURPOSE
//  -------
//  Golden: AutoClean.cpp:6465-7358 (DoIndexAutoClean_Arm1PickArm2Test), then
//  :7360-9105 (DoIndexAutoClean, which delegates to the former at :7421-7428
//  whenever RunAutoCleanByArmPickArm2Test()==true). Both share ONE Task cursor
//  (iDoIndexAutoCleanTask). This TU proves, over the Sim HAL (no hardware):
//
//    O1 guard-false #1 (golden :7368-7369): IniConfig.bF16CheckShuttleSensor-
//       Broken && bDoingF16 -> DoIndexAutoClean() returns BEFORE even reading
//       MOT[MTestY1].Motor->PJogHighSpeed -- proven by leaving MOT[MTestY1]
//       .Motor==NULL (would crash if dereferenced) and still getting a clean
//       early return with Task untouched.
//    O2 guard-false #2 (golden :7408-7409): bFullViewCheckFinish==false ->
//       early return, Task untouched (Motor attached from here on so the
//       unconditional PJogHighSpeed read earlier in the function body does
//       not crash).
//    O3 guard-false #3 (golden :7430-7431): Task in [400,1900] AND
//       MOT[MInShuttle1].IsCanMove()==true (the TMyMotor ctor default, golden
//       :7430) -> early return, Task untouched.
//    O4 a REAL run through DoIndexAutoClean's OWN switch (RunAutoCleanByArm-
//       PickArm2Test()==false): case 1 (golden :7438-7447) -> Task=100 +
//       bInedxCleanFinish[0]/[1] cleared from a true sentinel; case 100
//       (golden :7448-7503, bUseTwoArm32Site=false -> CheckAutoCleanCloseSite
//       returns false -> the FTestSuck branch, all-empty -> else -> Task=200);
//       case 200 (golden :7504-7509) -> Task=300.
//    O5 the DELEGATION branch itself (golden :7421-7428): RunAutoCleanByArm-
//       PickArm2Test()==true -> DoIndexAutoClean_Arm1PickArm2Test's OWN case 1
//       body runs (golden :6482-6487, IDENTICAL bInedxCleanFinish-clear +
//       Task=100 shape) and DoIndexAutoClean returns right after -- proven by
//       reading Task/bInedxCleanFinish[] after ONE call.
//    O6 DoIndexAutoClean_Arm1PickArm2Test driven DIRECTLY (now exposed in
//       AutoClean.h for testability), its OWN case 100 (golden :6488-6512,
//       DIFFERENT branch shape than DoIndexAutoClean's case 100 -- checks
//       TestSocket first, then FTestSuck, no CheckAutoCleanCloseSite/
//       CosFunction override tail): all-empty TestSocket+FTestSuck -> Task=200.
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" == clean g++
//  compile/link + the cursor/global mutations match golden's hand-derived
//  guard/case logic exactly (mirrors test_w906_autositemap_cleanout.cpp's own
//  equivalence note).
// =============================================================================
#include "AutoClean/AutoClean.h"    // DoIndexAutoClean / DoIndexAutoClean_Arm1PickArm2Test / RunAutoCleanByArmPickArm2Test
#include "aHotPlateSubstrate.h"     // TestSocket/FTestSuck/BTestSuck/FLCarryKit/BLCarryKit
#include "Motor/mymotor.h"          // MOT[] / MTestY1 etc
#include "Motor/mySimMotor.h"       // TMySimMotor (sim HTMotor)
#include "cprod.h"                  // Prod / TestIF / TestIF_File / IniConfig (via Config.h)
#include "cpublic.h"                // CosFunction
#include "cmydef.h"                 // iDoIndexAutoCleanTask / bInedxCleanFinish / bDoingF16 / bUseTwoArm32Site
#include <cstdio>

static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

int main()
{
    printf("==== W906-AutoCleanCluster DoIndexAutoClean verify ====\n");

    // -----------------------------------------------------------------------
    //  KNOWN Sim HAL baseline (same convention as test_w6_4_tester.cpp /
    //  test_w6_4b_front_rear_destroy.cpp).
    // -----------------------------------------------------------------------
    TestSocket.SetAllToNullIC();
    FTestSuck.SetAllToNullIC();
    BTestSuck.SetAllToNullIC();
    FLCarryKit.SetAllToNullIC();
    BLCarryKit.SetAllToNullIC();

    bUseTwoArm32Site = false;                          // CheckAutoCleanCloseSite(0) -> false (golden :4406-4410's bResult default)
    CosFunction.bAutoCleanAutoSelIndexArm = false;
    TestIF.iAutoClean_SelectArm = 0;                    // != 1, keeps case 100's override block from re-firing (golden :7484-7500)
    TestIF.bCleanIndexOtherArm  = false;
    TestIF_File.iShuttleMode = 0;
    TestIF_File.iShuttle_Sel = 0;
    TestIF_File.iAutoClean_Tray = eCKPos_CleanKit;      // avoid the eCKPos_CleanAir -> InitialSet() side effect in case 1
    TestIF_File.iAutoClean_MotorSpeed[2] = 50;          // <80 branch (golden :7371-7374), arbitrary but deterministic
    IniConfig.bF16CheckShuttleSensorBroken = false;
    bDoingF16 = false;
    IniConfig.bD58UseArm1PickPlaceArm2Test = false;     // RunAutoCleanByArmPickArm2Test() == false by default
    TestIF_File.bArm1PickPlaceArm2Test = false;
    TestIF_File.bArm1PickPlaceArm2Test_RunAutoClean = false;
    CUSTOMER_CODE = 0;                                  // != CC_GIGAS, takes the iSpeedSY=50000 else-branch (golden :7416-7419)
    bFullViewCheckFinish = true;

    // =========================================================================
    //  O1 -- guard-false #1: bF16CheckShuttleSensorBroken && bDoingF16, BEFORE
    //  the unconditional MOT[MTestY1].Motor->PJogHighSpeed read (golden
    //  :7368-7369, :7376). Left MOT[MTestY1].Motor==NULL here on purpose --
    //  if this guard were mistranslated (e.g. moved after the speed read),
    //  this call would SEGFAULT instead of returning cleanly.
    // =========================================================================
    printf("[O1] guard-false #1 (F16 shuttle-sensor-broken check), Motor==NULL untouched\n");
    CHECK(MOT[MTestY1].Motor == NULL, "precondition: MOT[MTestY1].Motor==NULL (no sim attached yet)");
    iDoIndexAutoCleanTask = 777;
    IniConfig.bF16CheckShuttleSensorBroken = true;
    bDoingF16 = true;
    DoIndexAutoClean();
    CHECK(iDoIndexAutoCleanTask == 777,
          "O1 F16 guard true -> DoIndexAutoClean returns before the Motor-> dereference, Task untouched (golden :7368-7369)");
    IniConfig.bF16CheckShuttleSensorBroken = false;
    bDoingF16 = false;

    // From here on every call reaches the unconditional
    // `MOT[MTestY1].Motor->PJogHighSpeed` read (golden :7376) -- attach a sim
    // HTMotor to MTestY1 only (same "attach/detach a TMySimMotor" idiom as
    // test_w6_4_tester.cpp Part C / N5 and test_w6_4b_front_rear_destroy.cpp
    // Part B). Every OTHER axis (MTestZ1/MTestZ2/MInShuttle1/MInShuttle2)
    // stays Motor==NULL for the rest of this file, so Gali_Two_ZAxis_Move /
    // Gali_MotMove on THOSE axes keep their offline-complete-immediately fast
    // path (mymotor.cpp:944) -- only GalilTwoY_Move on MTestY1 itself becomes
    // permanently "not complete" (Motor!=NULL -> returns false), which none of
    // this file's asserted transitions depend on.
    TMySimMotor simY1;
    simY1.PJogHighSpeed = 50000;
    HTMotor *savedY1 = MOT[MTestY1].Motor;
    MOT[MTestY1].Motor = &simY1;

    // =========================================================================
    //  O2 -- guard-false #2: bFullViewCheckFinish==false, AFTER the
    //  PJogHighSpeed read (golden :7408-7409) -- proves the read itself does
    //  not crash now that Motor is attached, and that this guard still blocks
    //  the switch.
    // =========================================================================
    printf("[O2] guard-false #2 (bFullViewCheckFinish==false), after the Motor-> read\n");
    iDoIndexAutoCleanTask = 778;
    bFullViewCheckFinish = false;
    DoIndexAutoClean();
    CHECK(iDoIndexAutoCleanTask == 778,
          "O2 bFullViewCheckFinish==false -> early return, Task untouched (golden :7408-7409)");
    bFullViewCheckFinish = true;

    // =========================================================================
    //  O3 -- guard-false #3: Task in [400,1900] + MOT[MInShuttle1].IsCanMove()
    //  (the ctor-default true -- fCanMove/fCanMoveR/fCanMoveM/fCanMoveL all
    //  start true, mymotor.cpp:121-124) -> early return (golden :7430-7431).
    // =========================================================================
    printf("[O3] guard-false #3 (Task in [400,1900] + MInShuttle1.IsCanMove() default-true)\n");
    CHECK(MOT[MInShuttle1].IsCanMove() == true, "precondition: MInShuttle1.IsCanMove() ctor-default true");
    iDoIndexAutoCleanTask = 400;
    DoIndexAutoClean();
    CHECK(iDoIndexAutoCleanTask == 400,
          "O3 Task=400 in-range + IsCanMove()==true -> early return, Task untouched (golden :7430-7431)");

    // =========================================================================
    //  O4 -- a REAL run through DoIndexAutoClean's OWN switch (delegation
    //  guard false): case 1 -> 100 -> 200 -> 300.
    // =========================================================================
    printf("[O4] DoIndexAutoClean own switch: case 1 -> 100 -> 200 -> 300\n");
    iDoIndexAutoCleanTask = 1;
    bInedxCleanFinish[0] = true;   // sentinel: case 1 must clear both to false
    bInedxCleanFinish[1] = true;

    DoIndexAutoClean();   // tick 1: case 1 -> Task=100 (golden :7438-7447)
    CHECK(iDoIndexAutoCleanTask == 100, "O4a case1 -> Task=100 (golden :7446)");
    CHECK(bInedxCleanFinish[0] == false && bInedxCleanFinish[1] == false,
          "O4b case1 clears bInedxCleanFinish[0]/[1] (golden :7444-7445)");

    DoIndexAutoClean();   // tick 2: case 100 -> Task=200 (CheckAutoCleanCloseSite(0)==false -> FTestSuck all-empty -> else -> 200)
    CHECK(iDoIndexAutoCleanTask == 200,
          "O4c case100: bUseTwoArm32Site==false -> CheckAutoCleanCloseSite(0)==false -> FTestSuck empty -> Task=200 (golden :7478-7481)");

    DoIndexAutoClean();   // tick 3: case 200 -> Task=300 (golden :7504-7509)
    CHECK(iDoIndexAutoCleanTask == 300, "O4d case200 -> Task=300 (golden :7508)");

    // =========================================================================
    //  O5 -- the DELEGATION branch itself: RunAutoCleanByArmPickArm2Test()==true
    //  routes to DoIndexAutoClean_Arm1PickArm2Test's OWN case 1 (golden
    //  :6482-6487), then DoIndexAutoClean returns immediately (golden :7427).
    // =========================================================================
    printf("[O5] RunAutoCleanByArmPickArm2Test()==true delegation -> Arm1PickArm2Test's own case1\n");
    IniConfig.bD58UseArm1PickPlaceArm2Test = true;
    TestIF_File.bArm1PickPlaceArm2Test = true;
    TestIF_File.bArm1PickPlaceArm2Test_RunAutoClean = true;
    CHECK(RunAutoCleanByArmPickArm2Test() == true, "precondition: delegation guard true");

    iDoIndexAutoCleanTask = 1;
    bInedxCleanFinish[0] = true;   // sentinel again
    bInedxCleanFinish[1] = true;

    DoIndexAutoClean();   // ONE call: delegates, Arm1PickArm2Test's case1 runs, then returns
    CHECK(iDoIndexAutoCleanTask == 100,
          "O5a delegated case1 -> Task=100 (golden :6486, same shape as DoIndexAutoClean's own case1)");
    CHECK(bInedxCleanFinish[0] == false && bInedxCleanFinish[1] == false,
          "O5b delegated case1 clears bInedxCleanFinish[0]/[1] (golden :6484-6485)");

    IniConfig.bD58UseArm1PickPlaceArm2Test = false;     // restore: back to DoIndexAutoClean's own switch for any sibling suite
    TestIF_File.bArm1PickPlaceArm2Test = false;
    TestIF_File.bArm1PickPlaceArm2Test_RunAutoClean = false;

    // =========================================================================
    //  O6 -- DoIndexAutoClean_Arm1PickArm2Test driven DIRECTLY with its own
    //  local arrays (now exposed in AutoClean.h for exactly this purpose):
    //  its OWN case 100 (DIFFERENT branch shape than DoIndexAutoClean's case
    //  100 -- checks TestSocket first, then FTestSuck, no CheckAutoCleanClose-
    //  Site/CosFunction override tail).
    // =========================================================================
    printf("[O6] DoIndexAutoClean_Arm1PickArm2Test driven directly: case100 (all-empty) -> Task=200\n");
    bool localDup[MAX_SOCKET_ROW][MAX_SOCKET_COL]        = {{false}};
    bool localSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{false}};
    bool localFallDown[MAX_SOCKET_ROW][MAX_SOCKET_COL]   = {{false}};
    bool localTestSuckUse[MAX_SOCKET_ROW][MAX_SOCKET_COL]= {{false}};
    int  localContactCount = -1;

    iDoIndexAutoCleanTask = 100;   // enter directly at case100 (case1's own transition already proven by O4/O5)
    DoIndexAutoClean_Arm1PickArm2Test(1000, 1000, 2000, localContactCount,
                                      localDup, localSuckFinish, localFallDown,
                                      localTestSuckUse, "");
    CHECK(iDoIndexAutoCleanTask == 200,
          "O6 Arm1PickArm2Test case100: TestSocket+FTestSuck both empty -> Task=200 (golden :6507-6510)");

    // --- restore the Motor==NULL offline knob for sibling suites ------------
    MOT[MTestY1].Motor = savedY1;

    printf("\n==== W906-AutoCleanCluster DoIndexAutoClean summary: %d passed, %d failed ====\n",
           g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
