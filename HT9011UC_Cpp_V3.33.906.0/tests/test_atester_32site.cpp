// =============================================================================
//  test_atester_32site.cpp -- W5-atester32site VERIFY: the 32-Site dual-arm
//                              test-head state machines PUMP over the Sim HAL,
//                              and the two gated vision islands PARK safely
//                              (no fabricated pass, no crash, no cursor escape).
//
//  Translation wave: W5-atester32site (biggest single unit in this batch)
//  Author: AI(W5-atester32site-Translate) 20260710
//  Suite name (add_test): W5_Atester32Site
//
//  PURPOSE
//  -------
//  Proves atester_32Site.cpp/.h (new translation) LINK + their 8 state
//  machines PUMP over the Sim HAL with no hardware, and specifically that:
//    (a) Init*Task() functions reset their cursor to 1.
//    (b) Do32RTCAutoModelVerify(true) resets the cursor + returns false.
//    (c) Do32RTCAutoModelVerify's cases 1/100/200 converge over the Sim HAL
//        (Gali_Two_ZAxis_Move/GalilTwoY_Move report complete when
//        Motor==NULL, mymotor.cpp:944/961) and then PARK at case 200 forever
//        (DoAllPassVerifyRTC() is ITSELF an already-gated atester.cpp stub
//        that always returns false offline -- this file's own gate never
//        even needs to run for the oracle path to hold).
//    (d) Forcing the cursor directly into each of the two GATED vision
//        islands (Do32RTCAutoModelVerify case 12500; DoTestY_TwoArm32Site
//        case 55) parks it there indefinitely -- no fabricated pass/fail, no
//        crash, no runaway cursor value.
//    (e) Every state machine's cursor never escapes its own documented
//        switch(Task) label set over many pump ticks (no crash, no wild
//        jump) -- mirrors tests/test_w6_4b_front_rear_destroy.cpp's own
//        "cursorSane" convergence-knob pattern.
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here == clean
//  g++ compile/link + the SMs pump over the Sim HAL without crash + outputs
//  match values hand-derived from the golden formula/cursor logic (mirrors
//  test_w6_4_tester.cpp / test_w6_4b_front_rear_destroy.cpp's own convention).
// =============================================================================
#include "atester_32Site.h"
#include "atester.h"
#include "atester_shims.h"
#include "aTester_Front.h"
#include "aTester_Rear.h"
#include "Motor/mymotor.h"          // MOT[]
#include "aHotPlateSubstrate.h"     // FTestSuck/BTestSuck/FLCarryKit/BLCarryKit/FRCarryKit/BRCarryKit/TestSocket/InArmSuck
#include "cprod.h"
#include "cpublic.h"
#include "cmydef.h"
#include "csystem.h"
#include "canary_support.h"
#include "FormsFacade.h"
#include <cstdio>

static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) label sets (golden, per this file's own
//      translated source -- see atester_32Site.cpp for the exact case lists) --
static bool interFaceErrCursorSane(int t)
{   // golden atester_32Site.cpp :70-179 switch(Task) { 1,2,100,150,170,200 }
    switch (t) { case 1: case 2: case 100: case 150: case 170: case 200: return true; default: return false; }
}
static bool rtcVerifyCursorSane(int t)
{   // golden atester_32Site.cpp :184-681 switch(Task) { 1,100,200,12010,12020,
    // 12100,12200,12300,12310,12320,12330,12340,12400 } + gated entry {12500}
    switch (t) {
        case 1: case 100: case 200: case 12010: case 12020: case 12100: case 12200:
        case 12300: case 12310: case 12320: case 12330: case 12340: case 12400:
        case 12500:
            return true;
        default: return false;
    }
}
static bool testYCursorSane(int t)
{   // golden atester_32Site.cpp :693-2291 switch(Task) -- gated entry {55} + the
    // full active label set.
    switch (t) {
        case 1: case 50: case 52: case 55: case 81: case 82: case 84: case 85:
        case 90: case 95: case 97: case 100: case 105: case 106: case 108: case 110:
        case 120: case 130: case 200: case 209: case 210: case 220: case 221: case 225:
        case 226: case 230: case 240: case 241: case 250: case 300: case 310: case 311:
        case 312: case 313: case 314: case 315: case 316: case 320: case 325: case 330:
        case 340: case 2000: case 12000: case 12010:
            return true;
        default: return false;
    }
}
static bool testSuckCursorSane(int t)
{   // golden atester_32Site.cpp :2300-3138 switch(Task)
    switch (t) {
        case 1: case 2200: case 2400: case 2500: case 2600: case 2700: case 3000:
        case 3100: case 5000: case 5100: case 5200: case 5300: case 5400: case 5500:
        case 6000: case 6100:
            return true;
        default: return false;
    }
}
static bool destroyICCursorSane(int t)
{   // golden atester_32Site.cpp :3141-3684 switch(Task)
    switch (t) { case 1: case 310: case 320: case 500: case 600: return true; default: return false; }
}
static bool suckICCursorSane(int t)
{   // golden atester_32Site.cpp :3689-4749 switch(Task)
    switch (t) {
        case 1: case 299: case 300: case 301: case 305: case 3010: case 3020:
        case 320: case 321: case 500: case 501: case 502:
            return true;
        default: return false;
    }
}

int main()
{
    printf("==== W5-atester32site \"32-Site dual-arm test-head\" SM verify ====\n");

    // -----------------------------------------------------------------------
    //  KNOWN Sim HAL state (same convention as test_w6_4b_front_rear_destroy.cpp).
    //  Motor==NULL (no sim motor attached) for every axis this suite touches.
    // -----------------------------------------------------------------------
    TestSocket.SetAllToNullIC();
    FTestSuck.SetAllToNullIC();
    BTestSuck.SetAllToNullIC();
    FLCarryKit.SetAllToNullIC();
    BLCarryKit.SetAllToNullIC();
    FRCarryKit.SetAllToNullIC();
    BRCarryKit.SetAllToNullIC();
    InArmSuck.SetAllToNullIC();
    iOneCycle = 0;
    iCleanOut = 0;
    bContactCTOverCHK = false;
    bContactTimeOverStep = false;
    bNowDoInterFaceErrorStep = false;

    // =======================================================================
    //  PART A -- Init*Task() reset the cursor to 1 (oracle a)
    // =======================================================================
    printf("[A] Init*Task cursor reset\n");
    iTestTwoArm32SiteTask = 999;
    InitTestYTwoArm32SiteTask();
    CHECK(iTestTwoArm32SiteTask == 1, "InitTestYTwoArm32SiteTask: cursor -> 1");

    iTestSuckTestIC_TwoArm32Site_Task = 999;
    InitTestSuckTestIC_TwoArm32Site_Task();
    CHECK(iTestSuckTestIC_TwoArm32Site_Task == 1, "InitTestSuckTestIC_TwoArm32Site_Task: cursor -> 1");

    // =======================================================================
    //  PART B -- Do32RTCAutoModelVerify(true) resets cursor + returns false
    //            (oracle b); cases 1/100/200 converge over Sim HAL then PARK
    //            at 200 forever (oracle c: DoAllPassVerifyRTC() is itself an
    //            already-gated atester.cpp stub, always false offline).
    // =======================================================================
    printf("[B] Do32RTCAutoModelVerify: init + cases 1/100/200 converge, park at 200\n");
    i32RTCAutoModelVerifyTask = 999;
    CHECK(Do32RTCAutoModelVerify(true) == false, "Do32RTCAutoModelVerify(true): returns false");
    CHECK(i32RTCAutoModelVerifyTask == 1, "Do32RTCAutoModelVerify(true): cursor -> 1");

    Do32RTCAutoModelVerify(false);   // case 1: Gali_Two_ZAxis_Move(Motor==NULL)->true -> Task=100
    CHECK(i32RTCAutoModelVerifyTask == 100, "Do32RTCAutoModelVerify: case 1 -> Task=100 (Motor==NULL fast path)");
    Do32RTCAutoModelVerify(false);   // case 100: GalilTwoY_Move(Motor==NULL)->true -> DoAllPassVerifyRTC(true); Task=200
    CHECK(i32RTCAutoModelVerifyTask == 200, "Do32RTCAutoModelVerify: case 100 -> Task=200 (Motor==NULL fast path)");
    for (int tick = 0; tick < 16; ++tick)
        Do32RTCAutoModelVerify(false);
    CHECK(i32RTCAutoModelVerifyTask == 200,
          "Do32RTCAutoModelVerify: PARKS at Task=200 (DoAllPassVerifyRTC() is an already-gated atester.cpp stub, always false)");

    // =======================================================================
    //  PART C -- forcing the cursor directly into each GATED vision island
    //            parks it there indefinitely (oracle d) -- the core deliverable
    //            of this translation unit's two named islands.
    // =======================================================================
    printf("[C] Gated vision islands: forced entry parks, never escapes, never crashes\n");
    i32RTCAutoModelVerifyTask = 12500;
    for (int tick = 0; tick < 32; ++tick)
        Do32RTCAutoModelVerify(false);
    CHECK(i32RTCAutoModelVerifyTask == 12500,
          "Do32RTCAutoModelVerify: forced Task=12500 (vision handshake island) PARKS -- no fabricated pass/fail");

    iTestTwoArm32SiteTask = 55;
    for (int tick = 0; tick < 32; ++tick)
        DoTestY_TwoArm32Site();
    CHECK(iTestTwoArm32SiteTask == 55,
          "DoTestY_TwoArm32Site: forced Task=55 (RTC full-view + ROI-learning island) PARKS -- no fabricated pass/fail");

    // =======================================================================
    //  PART D -- pump-sanity: every SM's cursor stays in its documented
    //            switch(Task) label set over many ticks (no crash, no escape).
    // =======================================================================
    printf("[D] Pump-sanity: cursor stays in the documented switch(Task) set\n");
    {
        iDoInterFaceErrorStepTask = 1;
        bool sane = true;
        for (int tick = 0; tick < 64; ++tick) {
            DoInterFaceErrorStep_TwoArm32Site();
            if (!interFaceErrCursorSane(iDoInterFaceErrorStepTask)) { sane = false; break; }
        }
        CHECK(sane, "DoInterFaceErrorStep_TwoArm32Site: cursor stays in the documented switch(Task) set");
    }
    {
        i32RTCAutoModelVerifyTask = 1;
        bool sane = true;
        for (int tick = 0; tick < 64; ++tick) {
            Do32RTCAutoModelVerify(false);
            if (!rtcVerifyCursorSane(i32RTCAutoModelVerifyTask)) { sane = false; break; }
        }
        CHECK(sane, "Do32RTCAutoModelVerify: cursor stays in the documented switch(Task) set");
    }
    {
        iTestTwoArm32SiteTask = 1;
        bool sane = true;
        for (int tick = 0; tick < 128; ++tick) {
            DoTestY_TwoArm32Site();
            if (!testYCursorSane(iTestTwoArm32SiteTask)) { sane = false; break; }
        }
        CHECK(sane, "DoTestY_TwoArm32Site: cursor stays in the documented switch(Task) set");
    }
    {
        iTestSuckTestIC_TwoArm32Site_Task = 1;
        bool sane = true;
        for (int tick = 0; tick < 64; ++tick) {
            DoTestSuckTestIC_TwoArm32Site();
            if (!testSuckCursorSane(iTestSuckTestIC_TwoArm32Site_Task)) { sane = false; break; }
        }
        CHECK(sane, "DoTestSuckTestIC_TwoArm32Site: cursor stays in the documented switch(Task) set");
    }
    {
        iFrontTestDestroyICTask = 1;   // shared cursor with (offline stub) DoFrontTestDestroyIC -- see file banner
        bool sane = true;
        for (int tick = 0; tick < 64; ++tick) {
            Do32SiteTestDestroyIC();
            if (!destroyICCursorSane(iFrontTestDestroyICTask)) { sane = false; break; }
        }
        CHECK(sane, "Do32SiteTestDestroyIC: cursor stays in the documented switch(Task) set");
    }
    {
        iFrontTestSuckICTask = 1;      // shared cursor with (offline stub) DoFrontTestSuckIC -- see file banner
        bool sane = true;
        for (int tick = 0; tick < 64; ++tick) {
            Do32SiteTestSuckIC();
            if (!suckICCursorSane(iFrontTestSuckICTask)) { sane = false; break; }
        }
        CHECK(sane, "Do32SiteTestSuckIC: cursor stays in the documented switch(Task) set");
    }

    // =======================================================================
    printf("\n==== W5-atester32site summary: %d PASS, %d FAIL ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
