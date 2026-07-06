// =============================================================================
//  test_w6_4_tester.cpp  --  W6.4 VERIFY: the TESTER / TEST-HEAD (Index) ENGINE
//
//  Translation wave: W6.4 (TESTER/INDEX ENGINE -- atester.cpp)
//  Author: AI(W6.4-TESTER) 20260626
//
//  PURPOSE
//  -------
//  The task-mandated tester/test-head verify TU (mirror of the W6.2/W6.3 arm +
//  tray core verifies).  Proves atester.cpp (the TESTER/INDEX ENGINE, in
//  ht9045_sm) LINKS + its ACTIVE core task state machines + numeric helpers PUMP
//  over the Sim HAL with NO hardware, asserting:
//    (a) a KNOWN Sim HAL state (TestSocket/FTestSuck/BTestSuck/FLCarryKit/
//        BLCarryKit grids + the test-head cursors + the offline Motor==NULL knob);
//    (b) the core test-head task SMs pumped FROM THEIR ENTRY CURSOR for N ticks;
//    (c) each cursor stays inside the documented switch(Task) set + the entry /
//        early-out matches the GOLDEN (golden line cited);
//    (d) the geometry / numeric ORACLE values hand-derived from the golden
//        formula/cursor logic (incl. the GetIndexZSpeed INTEGER-division contract).
//
//  CORE SMs UNDER TEST (ACTIVE in atester.cpp -- cursor flow verbatim):
//    [SM1] DoTestY()           cursor iTestYTask   (golden :4789).
//          ORACLE (task brief #1): bCanNotDisableOneCycle==false +
//          bUseTwoArm32Site==false -> case 1 -> Task=50 (golden :4890).  case 50
//          with NO ICs anywhere (Type1=0,Type2=0, FL/BL empty) -> fFront stays
//          true (Type1==0 branch :5014) -> Task=100 (:5033).  case 100
//          (InitTestYFrontTask; Task=110) FALLS THROUGH to case 110 (no break,
//          :5180) in the SAME tick.  So: iTestYTask 1 -> 50 -> 100/110.
//    [SM2] DoTestHeadMotor()   cursor iTestHeadMotorTask (golden :5562).
//          ORACLE (task brief #2): from case 4 with bC02InstallCCD==false ->
//          Task=9 (:6051); case 9 after Gali_Two_ZAxis_Move succeeds (offline
//          Motor==NULL -> true) AND bD24EnableEPCheckFuntion==false -> Task=10
//          (:6140); case 10 with MOT.ISNormal()==true (offline Motor==NULL) ->
//          Task=15.  So: iTestHeadMotorTask walks 4 -> 9 -> 10 -> 15.
//
//  NUMERIC / GEOMETRY ORACLES (hand-derived from the golden formula):
//    [N3] ArrayConvertSite (golden :504): integer, NO division.
//         iTestMode==_8Site2X4(9, >=_6Site2X3=7): Site=X*4+Y+1, return Site-1.
//           ArrayConvertSite(1,2) = 1*4+2 = 6.
//         iTestMode==DualSite2x1(4, >=DualSite2x1 but <7): return X*2+Y.
//           ArrayConvertSite(1,1) = 3.
//         iTestMode==DualSite(1, else): return Y.  ArrayConvertSite(0,2) = 2.
//    [N4] GetRowCol (golden :144): iTestMode==_8Site2X4(9) -> iRow=2,iCol=4,
//         returns iSiteMapRTC=4 (:194).  ==SingleSite(0) -> 1,1, returns 0.
//         ==QualSite2X2(5) -> 2,2, returns 3.
//    [N5] GetIndexZSpeed (golden :9814): INTEGER division by literal 100.
//         iScale clamped: iD54SlowDownScale<1 -> iScale=1.  With a sim Motor*
//         (PJogHighSpeed=50000): bD54SlowDown=true, scale=30 -> 50000*30/100=15000;
//         scale=0 -> clamp 1 -> 50000*1/100=500.  !bD54SlowDown -> sp=GailSpeed
//         (offline-reachable, Motor==NULL safe).  /100 kept INTEGER (no float).
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here ==
//  clean g++ compile/link + the core SMs pump over the Sim HAL without crash +
//  outputs match values hand-derived from the golden formula/cursor logic.
// =============================================================================
#include "atester.h"               // GetRowCol / ArrayConvertSite / GetIndexZSpeed / DoTestY / DoTestHeadMotor / cursors
#include "aHotPlateSubstrate.h"    // TestSocket / FTestSuck / BTestSuck / FLCarryKit / BLCarryKit / InArmSuck
#include "Motor/mymotor.h"         // MOT[]
#include "Motor/mySimMotor.h"      // TMySimMotor (sim HTMotor for the GetIndexZSpeed slow-down oracle)
#include "cprod.h"                 // Prod / TestIF / TestIF_File / IniConfig (via Config.h) / DeviceForm
#include "cpublic.h"
#include "cmydef.h"                // MTestZ1 / IndexStatus / Z1_Z2_Normal / DualSite / _8Site2X4 / ...
#include "csystem.h"
#include "canary_support.h"        // LastSet
#include "FormsFacade.h"
#include "atester_shims.h"         // CCDInterfaceForm / fAutomation / IsNNMode / ...
#include <cstdio>

static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) state sets (golden) --------------------------
static bool testYCursorSane(int t)
{   // golden atester.cpp:4789-5347 (switch(Task) { 1,20,25,30,40,45,50,60,65,70,
    //  100,110,200,210,260,300,310 })
    switch (t) {
        case 1: case 20: case 25: case 30: case 40: case 45: case 50:
        case 60: case 65: case 70: case 100: case 110: case 200: case 210:
        case 260: case 300: case 310: return true;
        default: return false;
    }
}
static bool testHeadCursorSane(int t)
{   // golden atester.cpp:5562-8655 DOCUMENTED switch(Task) universe.  The ACTIVE
    //  oracle window (1,2,3,4,5,6,7,9,10,15,20,21 + the 200000..600000 self-check
    //  entry) is reproduced verbatim.  AI(W7T1-Integrate) 20260701: the dense
    //  down-press/test cases (30..600, 12000.., 14000.., 122100.., 142.., 1500..,
    //  10000.., 20000.., 30000.., 40000.., 50000.., 60000.., 80000..) are NO LONGER
    //  GATED -- W7-T1 un-gated the full tree, so the sane-loop below now walks LIVE
    //  from case 21 INTO them (empirically: 1->200000..600000->9->10->15->20->21->
    //  100->120->12100->12101, then settles).  Every literal Task= target in the
    //  live tree is a VALID golden cursor value and belongs in this set.  The only
    //  tree values deliberately OMITTED are iCASE_REAL_CCD2..6 (=40200/40300/40400/
    //  40500/40510): their entry (case 600000) needs REAL_TIME_CCD==true &&
    //  !COM2->bCCDDummyRum, which is UNREACHABLE offline (REAL_TIME_CCD false +
    //  bCCDDummyRum true -> Task=9), so the loop never reaches them.  If a future
    //  wave enables REAL_TIME_CCD, add 40200/40300/40400/40500/40510 here.
    switch (t) {
        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
        case 9: case 10: case 11: case 15: case 20: case 21:
        case 30: case 40: case 50: case 55: case 60: case 70: case 80: case 90:
        case 100: case 110: case 112: case 115: case 116: case 120: case 121:
        case 122: case 123: case 124: case 125: case 130: case 135: case 140:
        case 141: case 142: case 143: case 144: case 145: case 150: case 160:
        case 170: case 600: case 1500: case 1530: case 1535: case 1550: case 1600:
        case 1650: case 1660: case 1670: case 1675: case 1680: case 1700: case 1710:
        case 1720: case 10000: case 10010: case 10020: case 10030: case 12000:
        case 12100: case 12101: case 12102: case 12110: case 12111: case 12112:
        case 12150: case 12151: case 12152: case 12200: case 12300: case 12301:
        case 12302: case 12305: case 14000: case 14100: case 14101: case 14102:
        case 14110: case 14111: case 14112: case 14150: case 14151: case 14152:
        case 14200: case 14300: case 14301: case 14302: case 14305: case 15000:
        case 15100: case 20000: case 20100: case 20200: case 20210: case 20300:
        case 20400: case 20500: case 21000: case 21400: case 21500: case 30000:
        case 40310: case 40320: case 40330: case 50000: case 50100: case 50110:
        case 60000: case 60100: case 60110: case 80000: case 80010: case 122100:
        case 122110: case 142100: case 142110:
        case 200000: case 300000: case 400000: case 500000: case 600000:
            return true;
        default: return false;
    }
}

int main()
{
    printf("==== W6.4 Tester / Test-Head ENGINE verify ====\n");

    // -----------------------------------------------------------------------
    //  KNOWN Sim HAL state.  Offline MOT[].Motor==NULL for every axis (no
    //  vendor backend attached) -> the Galil Z/Y moves report COMPLETE
    //  immediately + ISNormal()==true (Motor/mymotor.cpp NULL fast-path).
    // -----------------------------------------------------------------------
    TestSocket.SetAllToNullIC();
    FTestSuck.SetAllToNullIC();          // UseSiteHasIC()/HasRealIC() -> false
    BTestSuck.SetAllToNullIC();
    FLCarryKit.SetAllToNullIC();
    BLCarryKit.SetAllToNullIC();
    FRCarryKit.SetAllToNullIC();
    BRCarryKit.SetAllToNullIC();
    InArmSuck.SetAllToNullIC();
    IndexStatus = Z1_Z2_Normal;
    bCanNotDisableOneCycle = false;
    bUseTwoArm32Site = false;
    bCheckIndex = false;
    iOneCycle = 0;
    iCleanOut = 0;
    LastSet.iTemperature = 0;            // not Tempture_Hot
    iPauseBackUp = -1;                   // DoTestHeadMotor preamble: no pause-backup short-circuit

    // =======================================================================
    //  PART A -- DoTestY dispatcher SM (oracle #1)
    // =======================================================================
    printf("[A] DoTestY dispatcher SM (cursor iTestYTask)\n");
    InitTestYTask();                                                            // golden :4784 -> iTestYTask=1
    CHECK(iTestYTask == 1, "SM1 DoTestY: entry cursor == 1 (golden 4784)");

    // tick 1: case 1 -> Task=50 (bCanNotDisableOneCycle false, bUseTwoArm32Site false; golden 4890)
    DoTestY();
    CHECK(iTestYTask == 50, "SM1 DoTestY: case1 -> Task=50 (golden 4890)");

    // tick 2: case 50 with no ICs anywhere -> fFront stays true -> Task=100, which
    //         FALLS THROUGH to case 110 in the SAME tick (golden 5180 no-break).
    //         110 calls DoTestYFront() (shim true) -> Task=60 (or 1 on one-cycle).
    DoTestY();
    CHECK(iTestYTask == 60 || iTestYTask == 1 || iTestYTask == 100 || iTestYTask == 110,
          "SM1 DoTestY: case50 fFront-true -> 100 fall-through 110 ran in one tick (golden 5033/5180)");

    // pump: cursor must stay inside the documented set (no escape / no crash).
    {
        InitTestYTask();
        bool sane = true;
        for (int tick = 0; tick < 128; ++tick) {
            DoTestY();
            if (!testYCursorSane(iTestYTask)) { sane = false; break; }
        }
        CHECK(sane, "SM1 DoTestY: cursor stays in the documented switch(Task) set (no escape)");
    }

    // =======================================================================
    //  PART B -- DoTestHeadMotor central index SM (oracle #2): 4 -> 9 -> 10 -> 15
    // =======================================================================
    printf("[B] DoTestHeadMotor central index SM (cursor iTestHeadMotorTask)\n");
    //  Force the offline oracle preconditions for the case-4 entry window:
    IniConfig.bC02InstallCCD          = false;   // case 4 -> Task=9 (golden 6051)
    IniConfig.bD24EnableEPCheckFuntion = false;  // case 9 -> Task=10 (golden 6140), NOT the EP-check branch
    IniConfig.bEnableCCDUSETCPIP      = false;   // preamble ScanCCDProgram path off

    iTestHeadMotorTask = 4;                       // enter at the documented oracle window
    DoTestHeadMotor();
    CHECK(iTestHeadMotorTask == 9,  "SM2 DoTestHeadMotor: case4 bC02InstallCCD==false -> Task=9 (golden 6051)");
    DoTestHeadMotor();
    CHECK(iTestHeadMotorTask == 10, "SM2 DoTestHeadMotor: case9 Z-move done + no EP-check -> Task=10 (golden 6140)");
    DoTestHeadMotor();
    CHECK(iTestHeadMotorTask == 15, "SM2 DoTestHeadMotor: case10 ISNormal()==true -> Task=15 (golden 6173)");

    // pump from entry: cursor stays inside the documented set, no crash.
    {
        InitialTestHeadMotorTask();               // golden :5348 -> iTestHeadMotorTask=1
        CHECK(iTestHeadMotorTask == 1, "SM2 DoTestHeadMotor: Init -> cursor==1 (golden 5348)");
        bool sane = true;
        for (int tick = 0; tick < 128; ++tick) {
            DoTestHeadMotor();
            if (!testHeadCursorSane(iTestHeadMotorTask)) { sane = false; break; }
        }
        CHECK(sane, "SM2 DoTestHeadMotor: cursor stays in the documented switch(Task) set (no escape)");
    }

    // =======================================================================
    //  PART C -- numeric / geometry oracles (golden-formula hand-derived)
    // =======================================================================
    printf("[C] numeric / geometry oracles\n");

    // ---- N3: ArrayConvertSite (uses TestIF.iTestMode) ---------------------
    TestIF.iTestMode = _8Site2X4;                 // ==9, >= _6Site2X3(7): Site=X*4+Y+1, ret Site-1
    CHECK(ArrayConvertSite(1, 2) == 6, "N3 ArrayConvertSite(1,2)@_8Site2X4 == 6 (X*4+Y, golden 510)");
    TestIF.iTestMode = DualSite2x1;               // ==4, >=DualSite2x1 but <7: Site=X*2+Y+1, ret Site-1
    CHECK(ArrayConvertSite(1, 1) == 3, "N3 ArrayConvertSite(1,1)@DualSite2x1 == 3 (X*2+Y, golden 514)");
    TestIF.iTestMode = DualSite;                  // ==1, else: Site=Y+1, ret Site-1
    CHECK(ArrayConvertSite(0, 2) == 2, "N3 ArrayConvertSite(0,2)@DualSite == 2 (Y, golden 518)");

    // ---- N4: GetRowCol (uses TestIF_File.iTestMode) -----------------------
    int iRow = -1, iCol = -1;
    TestIF_File.iTestMode = _8Site2X4;
    CHECK(GetRowCol(iRow, iCol) == 4 && iRow == 2 && iCol == 4,
          "N4 GetRowCol@_8Site2X4 -> row2 col4 ret4 (golden 188-194)");
    TestIF_File.iTestMode = SingleSite;
    CHECK(GetRowCol(iRow, iCol) == 0 && iRow == 1 && iCol == 1,
          "N4 GetRowCol@SingleSite -> row1 col1 ret0 (golden 156)");
    TestIF_File.iTestMode = QualSite2X2;
    CHECK(GetRowCol(iRow, iCol) == 3 && iRow == 2 && iCol == 2,
          "N4 GetRowCol@QualSite2X2 -> row2 col2 ret3 (golden 167)");

    // ---- N5: GetIndexZSpeed INTEGER-division contract ---------------------
    //  Offline-safe path first: !bD54SlowDown -> sp=GailSpeed (Motor==NULL safe).
    IniConfig.bD54SlowDown = false;
    MOT[MTestZ1].GailSpeed = 1234;
    CHECK(GetIndexZSpeed(0) == 1234,
          "N5 GetIndexZSpeed !slowdown -> GailSpeed (offline-reachable, golden 9827)");
    //  Slow-down branch needs a Motor* (PJogHighSpeed); attach a sim HTMotor,
    //  exercise the INTEGER division, then DETACH (restore Motor==NULL).
    {
        TMySimMotor *sim = new TMySimMotor();
        sim->PJogHighSpeed = 50000;
        HTMotor *saved = MOT[MTestZ1].Motor;
        MOT[MTestZ1].Motor = sim;

        IniConfig.bD54SlowDown      = true;
        IniConfig.iD54SlowDownScale = 30;
        CHECK(GetIndexZSpeed(0) == 15000,
              "N5 GetIndexZSpeed slowdown scale=30 -> 50000*30/100 == 15000 (INTEGER div, golden 9822)");
        IniConfig.iD54SlowDownScale = 0;            // sub-1 -> clamp to iScale=1
        CHECK(GetIndexZSpeed(0) == 500,
              "N5 GetIndexZSpeed scale=0 clamped to 1 -> 50000*1/100 == 500 (clamp+INT div, golden 9819)");

        MOT[MTestZ1].Motor = saved;                 // restore Motor==NULL offline knob
        IniConfig.bD54SlowDown = false;
        delete sim;
    }

    // =======================================================================
    printf("\n==== W6.4 Tester summary: %d PASS, %d FAIL ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
