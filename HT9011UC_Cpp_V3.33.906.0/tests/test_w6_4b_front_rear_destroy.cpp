// =============================================================================
//  test_w6_4b_front_rear_destroy.cpp -- W64b VERIFY: the Front (Z1/Arm1) and
//                                        Rear (Z2/Arm2) "Destroy IC" down-press
//                                        state machines PUMP over the Sim HAL.
//
//  Translation wave: W64b-Translate (aTester_Front.cpp / aTester_Rear.cpp)
//  Author: AI(W64b-Integrate) 20260706
//  Suite name (add_test): W6_4b_FrontRearDestroy
//
//  PURPOSE
//  -------
//  Proves aTester_Front.cpp / aTester_Rear.cpp (now real translations, no
//  longer atester_shims.cpp offline stubs) LINK + their state machines PUMP
//  over the Sim HAL with NO hardware, asserting:
//    (a) Init*TestDestroyICTask() resets its cursor to 1 and the
//        bZ1Isdownflag/bZ2Isdownflag "index-is-down" latch to false
//        (golden aTester_Front.cpp:300-305 / aTester_Rear.cpp:300-305).
//    (b) CheckZ1IsDown()/CheckZ2IsDown()/TestZ1OutRandge()/TestZ2OutRandge()
//        return the golden-documented default under the SAME offline-safe
//        convergence knob the rest of this tree uses for MOT[] axes:
//        MOT[axis].Motor->Enable==false (see test_w6_4_tester.cpp Part C /
//        N5, which attaches a TMySimMotor with the HTMotor-ctor default
//        Enable=false).  CheckZ1IsDown/CheckZ2IsDown's OWN golden text checks
//        exactly this flag (golden aTester_Front.cpp:170-171 / aTester_Rear.cpp
//        :125-126) and early-returns true ("treat the axis as already down") --
//        this is the SAME "no vendor backend -> report done" contract as
//        TMyMotor::ISNormal()/Gali_Two_ZAxis_Move() (mymotor.cpp:944/951).
//        TestZ1OutRandge/TestZ2OutRandge read ONLY the unconditional-0 Galil
//        stubs Gali_ReadPos()/Gali_ReadEncoderPos() (mymotor.cpp:932-933, no
//        Motor dependency at all) -- Pos1-Pos2 is always 0 offline, so the
//        golden ">10" out-of-range gate never trips -> always false.
//    (c) DoFrontTestDestroyIC()/DoRearTestDestroyIC() from their Init cursor:
//        cursor legitimately PARKS at Task=100 and CANNOT converge further
//        offline -- NOT a defect of this translation wave, but the SAME
//        pre-existing TMyMotor::Gali_MotMove() stub limitation documented at
//        mymotor.cpp:920 ("Gali_* method stubs ... TODO(W6-Galil)"), which
//        (unlike Gali_Two_ZAxis_Move/ISNormal) has NO Motor==NULL offline
//        fast path and unconditionally returns false:
//          case 1:   OutSHT1InLF()/OutSHT2InLF() delegate to InSHT1InLF()/
//                    InSHT2InLF() (csystem_predicates.cpp:227-228, offline
//                    stub always false) -> ==false is true -> Task=100,
//                    return false (golden aTester_Front.cpp:286-289 /
//                    aTester_Rear.cpp:317-320).
//          case 100: MOT[MTestZ1/2].Gali_MotMove(...) is the unconditional
//                    `return false;` stub -> the `if` body (the only place
//                    Task changes) never runs -> Task stays 100 forever
//                    (golden aTester_Front.cpp:307-354 / aTester_Rear.cpp
//                    :337-398, `#else` branch since DEBUG_INDEX_UPH is not
//                    defined in this tree).
//        A pump-sanity loop additionally proves the cursor never escapes the
//        documented switch(Task) label set (no crash, no wild jump).
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here ==
//  clean g++ compile/link + the SMs pump over the Sim HAL without crash +
//  outputs match values hand-derived from the golden formula/cursor logic
//  (mirrors test_w6_4_tester.cpp's own equivalence note).
// =============================================================================
#include "aTester_Front.h"          // iFrontTestDestroyICTask / InitFrontTestDestroyICTask /
                                    // DoFrontTestDestroyIC / CheckZ1IsDown / TestZ1OutRandge
#include "aTester_Rear.h"           // iRearTestDestroyICTask / InitRearTestDestroyICTask /
                                    // DoRearTestDestroyIC / CheckZ2IsDown / TestZ2OutRandge
#include "aHotPlateSubstrate.h"    // FTestSuck/BTestSuck/FLCarryKit/BLCarryKit/FRCarryKit/BRCarryKit/TestSocket/InArmSuck
#include "Motor/mymotor.h"          // MOT[]
#include "Motor/mySimMotor.h"       // TMySimMotor (sim HTMotor; Enable defaults false, HTMotor.cpp:41)
#include "cprod.h"                  // Prod / TestIF / TestIF_File / IniConfig (via Config.h) / CosFunction
#include "cpublic.h"
#include "cmydef.h"                 // bZ1Isdownflag/bZ2Isdownflag / MTestZ1/MTestZ2/MTestY1/MTestY2 /
                                    // bContactTimeOverStep / bNowDoInterFaceErrorStep / bUseTwoArm32Site
#include "csystem.h"
#include "canary_support.h"         // LastSet
#include "FormsFacade.h"
#include "atester_shims.h"          // IsNNMode / fiosetview / ...
#include <cstdio>

static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) label sets (golden, both files share the shape) --
static bool frontCursorSane(int t)
{   // golden aTester_Front.cpp:309-865 switch(Task) { 1,100,200,309,310,320,500,550,600,650,700,750,800 }
    switch (t) {
        case 1: case 100: case 200: case 309: case 310: case 320: case 500:
        case 550: case 600: case 650: case 700: case 750: case 800:
            return true;
        default: return false;
    }
}
static bool rearCursorSane(int t)
{   // golden aTester_Rear.cpp:309-886 switch(Task) { 1,100,200,309,310,320,500,550,600,650,700,750,800 }
    switch (t) {
        case 1: case 100: case 200: case 309: case 310: case 320: case 500:
        case 550: case 600: case 650: case 700: case 750: case 800:
            return true;
        default: return false;
    }
}

int main()
{
    printf("==== W64b Front/Rear \"Destroy IC\" SM verify ====\n");

    // -----------------------------------------------------------------------
    //  KNOWN Sim HAL state (same convention as test_w6_4_tester.cpp Part A/B).
    // -----------------------------------------------------------------------
    TestSocket.SetAllToNullIC();
    FTestSuck.SetAllToNullIC();
    BTestSuck.SetAllToNullIC();
    FLCarryKit.SetAllToNullIC();
    BLCarryKit.SetAllToNullIC();
    FRCarryKit.SetAllToNullIC();
    BRCarryKit.SetAllToNullIC();
    InArmSuck.SetAllToNullIC();
    bUseTwoArm32Site = false;
    IniConfig.bD52InterFaceErrHeadNeedUp = false;
    bNowDoInterFaceErrorStep = false;
    bContactTimeOverStep     = false;
    CosFunction.bSortingBy2DList = false;
    iOneCycle  = 0;
    iCleanOut  = 0;

    // =======================================================================
    //  PART A -- Init*TestDestroyICTask() reset the cursor + down-flag (oracle a)
    // =======================================================================
    printf("[A] Init*TestDestroyICTask cursor/flag reset\n");
    iFrontTestDestroyICTask = 999;   bZ1Isdownflag = true;
    InitFrontTestDestroyICTask();    // golden aTester_Front.cpp:300-305 -> Task=1, bZ1Isdownflag=false
    CHECK(iFrontTestDestroyICTask == 1, "InitFrontTestDestroyICTask: cursor -> 1 (golden 304)");
    CHECK(bZ1Isdownflag == false,       "InitFrontTestDestroyICTask: bZ1Isdownflag -> false (golden 305)");

    iRearTestDestroyICTask = 999;    bZ2Isdownflag = true;
    InitRearTestDestroyICTask();     // golden aTester_Rear.cpp:300-305 -> Task=1, bZ2Isdownflag=false
    CHECK(iRearTestDestroyICTask == 1, "InitRearTestDestroyICTask: cursor -> 1 (golden 304)");
    CHECK(bZ2Isdownflag == false,       "InitRearTestDestroyICTask: bZ2Isdownflag -> false (golden 305)");

    // =======================================================================
    //  PART B -- CheckZ1IsDown/CheckZ2IsDown/TestZ1OutRandge/TestZ2OutRandge
    //            golden-documented offline defaults (oracle b)
    // =======================================================================
    printf("[B] CheckZ*IsDown / TestZ*OutRandge offline defaults\n");
    {
        // Attach sim HTMotors to the 4 axes both functions touch, mirroring
        // test_w6_4_tester.cpp's own "attach sim Motor, exercise, detach back
        // to NULL" convergence knob. AI(W64b-Integrate-fix) 20260706: TMySimMotor
        // sets Enable=true in its own ctor (mySimMotor.cpp:17,30, "sim motor is
        // always enabled") -- it does NOT inherit HTMotor's base default of false
        // (HTMotor.cpp:41). To actually exercise CheckZ1IsDown/CheckZ2IsDown's
        // documented "Motor->Enable==false -> return true" early-out branch
        // (golden aTester_Front.cpp:170-171 / aTester_Rear.cpp:125-126) rather
        // than falling through to the coincidental Gali_ReadEncoderPos()==0<=
        // Prod.iTestDown_Z1==0 tail branch, explicitly force Enable=false here.
        TMySimMotor simZ1, simZ2, simY1, simY2;
        simZ1.Enable = simZ2.Enable = simY1.Enable = simY2.Enable = false;
        HTMotor *savedZ1 = MOT[MTestZ1].Motor;
        HTMotor *savedZ2 = MOT[MTestZ2].Motor;
        HTMotor *savedY1 = MOT[MTestY1].Motor;
        HTMotor *savedY2 = MOT[MTestY2].Motor;
        MOT[MTestZ1].Motor = &simZ1;
        MOT[MTestZ2].Motor = &simZ2;
        MOT[MTestY1].Motor = &simY1;
        MOT[MTestY2].Motor = &simY2;

        CHECK(MOT[MTestZ1].Motor->Enable == false, "precondition: sim Motor Enable forced false");

        CHECK(CheckZ1IsDown() == true,
              "CheckZ1IsDown: Motor->Enable==false -> true (golden aTester_Front.cpp:170-171)");
        CHECK(CheckZ2IsDown() == true,
              "CheckZ2IsDown: Motor->Enable==false -> true (golden aTester_Rear.cpp:125-126)");

        CHECK(TestZ1OutRandge() == false,
              "TestZ1OutRandge: Gali_ReadPos()==Gali_ReadEncoderPos()==0 stubs -> diff 0, not >10 -> false (golden aTester_Front.cpp:224-241)");
        CHECK(TestZ2OutRandge() == false,
              "TestZ2OutRandge: Gali_ReadPos()==Gali_ReadEncoderPos()==0 stubs -> diff 0, not >10 -> false (golden aTester_Rear.cpp:224-241)");

        // restore the Motor==NULL offline knob for sibling suites
        MOT[MTestZ1].Motor = savedZ1;
        MOT[MTestZ2].Motor = savedZ2;
        MOT[MTestY1].Motor = savedY1;
        MOT[MTestY2].Motor = savedY2;
    }

    // =======================================================================
    //  PART C -- DoFrontTestDestroyIC / DoRearTestDestroyIC pump (oracle c)
    // =======================================================================
    printf("[C] DoFrontTestDestroyIC / DoRearTestDestroyIC pump (parks at Task=100)\n");

    // ---- Front: case1 OutSHT1InLF()==false -> Task=100; case100 Gali_MotMove
    //      stub always false -> parks at 100 forever (see file-top rationale).
    InitFrontTestDestroyICTask();
    CHECK(iFrontTestDestroyICTask == 1, "DoFrontTestDestroyIC: entry cursor == 1");
    DoFrontTestDestroyIC(false);
    CHECK(iFrontTestDestroyICTask == 100,
          "DoFrontTestDestroyIC: case1 OutSHT1InLF()==false -> Task=100 (golden aTester_Front.cpp:286-289)");
    for (int tick = 0; tick < 16; ++tick)
        DoFrontTestDestroyIC(false);
    CHECK(iFrontTestDestroyICTask == 100,
          "DoFrontTestDestroyIC: PARKS at Task=100 (Gali_MotMove stub always false, mymotor.cpp:920 TODO(W6-Galil) -- not a W64b defect)");

    // ---- Rear: mirror-image park at Task=100.
    InitRearTestDestroyICTask();
    CHECK(iRearTestDestroyICTask == 1, "DoRearTestDestroyIC: entry cursor == 1");
    DoRearTestDestroyIC(false);
    CHECK(iRearTestDestroyICTask == 100,
          "DoRearTestDestroyIC: case1 OutSHT2InLF()==false -> Task=100 (golden aTester_Rear.cpp:317-320)");
    for (int tick = 0; tick < 16; ++tick)
        DoRearTestDestroyIC(false);
    CHECK(iRearTestDestroyICTask == 100,
          "DoRearTestDestroyIC: PARKS at Task=100 (Gali_MotMove stub always false, mymotor.cpp:920 TODO(W6-Galil) -- not a W64b defect)");

    // ---- pump-sanity: cursor never escapes the documented switch(Task) set.
    {
        InitFrontTestDestroyICTask();
        bool sane = true;
        for (int tick = 0; tick < 128; ++tick) {
            DoFrontTestDestroyIC(false);
            if (!frontCursorSane(iFrontTestDestroyICTask)) { sane = false; break; }
        }
        CHECK(sane, "DoFrontTestDestroyIC: cursor stays in the documented switch(Task) set (no escape)");
    }
    {
        InitRearTestDestroyICTask();
        bool sane = true;
        for (int tick = 0; tick < 128; ++tick) {
            DoRearTestDestroyIC(false);
            if (!rearCursorSane(iRearTestDestroyICTask)) { sane = false; break; }
        }
        CHECK(sane, "DoRearTestDestroyIC: cursor stays in the documented switch(Task) set (no escape)");
    }

    // =======================================================================
    printf("\n==== W64b Front/Rear Destroy summary: %d PASS, %d FAIL ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
