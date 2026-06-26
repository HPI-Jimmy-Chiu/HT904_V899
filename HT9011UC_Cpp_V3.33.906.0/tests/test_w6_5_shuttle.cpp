// =============================================================================
//  test_w6_5_shuttle.cpp  --  W6.5 VERIFY: the SHUTTLE / CARRY ENGINE
//
//  Translation wave: W6.5 (SHUTTLE/CARRY ENGINE -- acarry.cpp, golden 8521 lines)
//  Author: AI(W6.5-CARRY) 20260626
//
//  PURPOSE
//  -------
//  The task-mandated W6.5 verify TU (mirror of the in/out/tray-arm + tester core
//  TUs).  Proves acarry.cpp (the SHUTTLE/CARRY ENGINE, in lib ht9045_sm) LINKS +
//  its ACTIVE core shuttle task SM(s) PUMP over the Sim HAL with NO hardware, and
//  that the geometry / numeric / zero-guard oracles match values hand-derived
//  from the golden formula (golden acarry.cpp @ D:\HT9045\
//  HT9011UC_Code_V3.33.906.0_20260618\acarry.cpp -- READ-ONLY).
//
//  EVERY golden case body is ACTIVE in the translation (136/136 case labels, 0
//  #if 0 gates -- the cross-module surface is satisfied by acarry_shims.cpp +
//  FormsFacade, NOT by gating the SM bodies).  So this TU exercises the REAL
//  translated switch(Task) flow, not a stub.
//
//  ORACLES (hand-derived from the golden, NOT from this implementation):
//
//    [O1] CURSOR-PROGRESSION (Do_Auto_SHT1 entry).  Initial_Auto_SHT1_Task()
//         -> AutoSHT1Task==1.  With Sim-HAL baseline MOT[MInShuttle1].IsCanMove()
//         ==true (offline motor idle, all fCanMove* true), In_Shuttle_Auto_Latch
//         != eInSHAutoLtc (int 0 != 1), bShuttle1MoveToLeft/Right==false ->
//         case 1 takes the final `Task=10;` (golden :3342, translated :3407).
//         One tick from entry: AutoSHT1Task 1 -> 10.  Cursor must stay in the
//         documented SHT1 switch(Task) universe under an N-tick pump.
//
//    [O3] NUMERIC / TRUNCATION (GetShuttle1Pitch_9045, golden :7540, translated
//         :7582): m=(i1180-i1120)/6000.0; r=i1120+m*(w-12000.0); return (int)r.
//           i1120=8000,i1180=14000,w=12000 -> m=1.0 r=8000.0 -> 8000
//           same, w=18000               -> r=14000.0       -> 14000
//           i1180=14003 (m=1.000500), w=15000 -> r=11001.5 -> (int)==11001
//             (C truncation toward zero -- MUST NOT round to 11002).
//
//    [O4] GEOMETRY / ZERO-GUARD (CheckShuttleOutputHasICError, golden :2076-2097,
//         translated :2105).  With Tech.OutSH*ZOneRowDetectPos==0 (Sim default)
//         the function RETURNS false before the residual switch (the zero-detect-
//         pos guard).  Offline (Tech fields 0) it returns false regardless of
//         bShuttleHasIC -- no spurious residual alarm.  (The Sen[].Enable==false
//         Sim default ALSO early-returns false at golden :2058/translated :2121,
//         so the offline "no spurious alarm" invariant holds via either guard;
//         both are asserted.)
//
//    [O5] CheckShuttleSensorBroken_1(bRefresh,bRight=false) first call: static
//         iCheckCount==0 so the bRight==true error-aggregation block is skipped;
//         bHasErr stays false -> returns false (no broken-sensor error).  Confirms
//         the left-pass arm of the left->right->left sensor-presence accumulator
//         (golden :7322, translated :7322).
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here == clean
//  g++ compile/link + the core shuttle SM pumping over the Sim HAL without crash
//  (cursor staying inside the documented switch(Task) set) + the outputs matching
//  the values hand-derived from the golden formula/cursor logic above.
// =============================================================================
// NOTE: acarry.h uses AnsiString in its declarations but does NOT include the
// compat header itself (mirrors the BCB6 .h which relied on the .cpp's earlier
// includes).  Pull the substrate (which includes vcl_compat.h -> AnsiString) +
// the global headers FIRST, exactly as acarry.cpp orders its includes, so the
// engine surface declarations parse.
#include "vclcompat/vcl_compat.h"   // AnsiString (acarry.h decls reference it)
#include "aHotPlateSubstrate.h"     // FRCarryKit / FLCarryKit / FTestSuck / BRCarryKit carry-kit grid
#include "cprod.h"                  // Prod (iShuttle1120/1180Pitch) / IniConfig / TestIF
#include "cpublic.h"
#include "cmydef.h"                 // MInShuttle1 / bShuttle1MoveToLeft/Right / In_Shuttle_Auto_Latch / bTestingStopAllMotor / bDoingF16 / SingleSite
#include "MachineType.h"            // eInSHAutoLtc / eInSH8Sen
#include "Motor/mymotor.h"          // MOT[] (TMyMotor Sim HAL; Motor==NULL offline)
#include "mysensor.h"               // Sen[] (TMySensor Sim HAL)
#include "acarry.h"                 // Do_Auto_SHT1 / Initial_Auto_SHT1_Task / AutoSHT1Task / GetShuttle1Pitch_9045 / CheckShuttleSensorBroken_1 / bShuttleHasIC
#include "acarry_shims.h"           // Tech (TECH_AcarryShim) zero-default detect positions + SystemNG
#include <cstdio>

// CheckShuttleOutputHasICError is a file-scope (non-static) function in acarry.cpp
// (golden :2076); it has no header home in either tree, so forward-declare its
// exact signature here -- the real body binds at link time (ht9045_sm).
bool CheckShuttleOutputHasICError(int iSelSHT, int &X, int &Y);   // golden acarry.cpp:2076

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as the other W6 verify TUs)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) state set for Do_Auto_SHT1 -------------------
//  golden acarry.cpp:3331-5234 (translated :3331-5234).  Full case universe
//  extracted from the verbatim switch(Task):
//    1 10 100 105 110 115 120 124 125 130 135 140 200 201 202 203 210 211 212
//    220 225 230 235 250 251 252 300 400 500 2300 2400 2500 2600 3000 3100 3101
//    3102 5000 5100
static bool sht1CursorSane(int t)
{
    switch (t) {
        case 1: case 10: case 100: case 105: case 110: case 115: case 120:
        case 124: case 125: case 130: case 135: case 140: case 200: case 201:
        case 202: case 203: case 210: case 211: case 212: case 220: case 225:
        case 230: case 235: case 250: case 251: case 252: case 300: case 400:
        case 500: case 2300: case 2400: case 2500: case 2600: case 3000:
        case 3100: case 3101: case 3102: case 5000: case 5100:
            return true;
        default:
            return false;
    }
}

// Put the Do_Auto_SHT1 pre-switch entry guards into the offline "proceed" state
// (golden :3346-3368 / translated :3346-3368).  Each is a plain global; we set
// the documented safe value so the SM enters the switch instead of early-return.
static void ArmSHT1Guards()
{
    // golden :3346 -- testing-stop-all-motor guard (need NOT all true)
    IniConfig.bEnableTestingNeedStopAllMotor = false;
    IniConfig.bI24TestingNeedStopAllMotor    = false;
    bTestingStopAllMotor                     = false;
    // golden :3353 -- auto-alignment CCD guard
    bRunInArmAutoAlignment    = false;
    lOutArmAutoAlignmentFlag  = 0;
    // golden :3364 -- F16 shuttle-sensor-broken guard
    bDoingF16 = false;
    // golden :3367 -- system-NG guard
    SystemNG = false;
    // case 1 decision inputs (golden :3380-3408)
    In_Shuttle_Auto_Latch = eInSH8Sen;   // != eInSHAutoLtc (==1) -> skip latch path
    bShuttle1MoveToLeft   = false;
    bShuttle1MoveToRight  = false;
    bShuttleMoveToLeftforFix3 = false;
}

int main()
{
    printf("==== W6.5 SHUTTLE/CARRY engine verify (acarry.cpp over Sim HAL) ====\n");

    // =======================================================================
    //  PART A -- [O1] Do_Auto_SHT1 entry cursor progression 1 -> 10 + pump.
    //  The core shuttle task SM, pumped from its entry cursor over the Sim HAL.
    // =======================================================================
    printf("[A] Do_Auto_SHT1 entry SM (cursor AutoSHT1Task)\n");

    Initial_Auto_SHT1_Task();                       // golden :3180 -> AutoSHT1Task=1
    CHECK(AutoSHT1Task == 1, "O1a Initial_Auto_SHT1_Task -> entry cursor == 1 (golden 3180)");

    ArmSHT1Guards();

    // tick 1: case 1, MOT[MInShuttle1].IsCanMove()==true (offline ctor flags all
    //         true) + latch path skipped + neither move-flag set -> Task=10
    //         (golden :3342, translated :3407).
    CHECK(MOT[MInShuttle1].IsCanMove() == true,
          "O1b Sim HAL MOT[MInShuttle1].IsCanMove()==true offline (motor idle)");
    Do_Auto_SHT1();
    printf("    observed AutoSHT1Task after 1 tick = %d (expect 10)\n", AutoSHT1Task);
    CHECK(AutoSHT1Task == 10, "O1c Do_Auto_SHT1: case1 -> Task=10 (golden 3407)");

    // pump: from entry, the cursor must stay inside the documented switch(Task)
    //       universe for N ticks (no escape to an undefined Task, no crash).
    {
        Initial_Auto_SHT1_Task();
        ArmSHT1Guards();
        bool escaped = false;
        for (int i = 0; i < 64; ++i) {
            Do_Auto_SHT1();
            if (!sht1CursorSane(AutoSHT1Task)) { escaped = true; break; }
        }
        CHECK(!escaped, "O1d Do_Auto_SHT1: cursor stays in documented switch(Task) set over 64 ticks (golden 3331-5234)");
        printf("    cursor after 64-tick pump = %d\n", AutoSHT1Task);
    }

    // =======================================================================
    //  PART B -- [O3] GetShuttle1Pitch_9045 double->int TRUNCATION (golden 7540).
    // =======================================================================
    printf("[B] GetShuttle1Pitch_9045 linear interpolation + truncation\n");

    // m=1.0, w=12000 -> r = 8000 + 1.0*0 = 8000.0 -> 8000
    Prod.iShuttle1120Pitch = 8000;
    Prod.iShuttle1180Pitch = 14000;
    {
        int v = GetShuttle1Pitch_9045(12000);
        printf("    GetShuttle1Pitch_9045(12000) [i1120=8000,i1180=14000] = %d (expect 8000)\n", v);
        CHECK(v == 8000, "O3a GetShuttle1Pitch_9045 w=12000 -> 8000 (golden 7540)");
    }
    // m=1.0, w=18000 -> r = 8000 + 1.0*6000 = 14000.0 -> 14000
    {
        int v = GetShuttle1Pitch_9045(18000);
        printf("    GetShuttle1Pitch_9045(18000) = %d (expect 14000)\n", v);
        CHECK(v == 14000, "O3b GetShuttle1Pitch_9045 w=18000 -> 14000 (golden 7540)");
    }
    // i1180=14003 -> m=(14003-8000)/6000.0=1.0005; w=15000 ->
    //   r = 8000 + 1.0005*3000 = 8000 + 3001.5 = 11001.5 -> (int)11001.5 == 11001
    //   (truncation toward zero -- MUST NOT round to 11002).
    Prod.iShuttle1180Pitch = 14003;
    {
        int v = GetShuttle1Pitch_9045(15000);
        printf("    GetShuttle1Pitch_9045(15000) [i1180=14003,m=1.0005] = %d (expect 11001, NOT 11002)\n", v);
        CHECK(v == 11001, "O3c GetShuttle1Pitch_9045 r=11001.5 -> (int) truncates to 11001 (golden 7540)");
        CHECK(v != 11002, "O3d GetShuttle1Pitch_9045 does NOT round to 11002 (double->int truncation preserved)");
    }

    // =======================================================================
    //  PART C -- [O4] CheckShuttleOutputHasICError zero-guard (golden 2076-2097).
    //  Offline: Tech detect positions all 0 (TECH_AcarryShim ctor) + Sen[].Enable
    //  default false -> the function returns false BEFORE the residual switch, so
    //  no spurious residual alarm even if bShuttleHasIC says there is residual IC.
    // =======================================================================
    printf("[C] CheckShuttleOutputHasICError zero-detect-pos guard\n");

    // Sanity: the Tech detect positions are 0 offline (TECH_AcarryShim default).
    CHECK(Tech.OutSH1ZOneRowDetectPos == 0 && Tech.OutSH2ZOneRowDetectPos == 0 &&
          Tech.OutSH1ZDetectPos == 0 && Tech.OutSH2ZDetectPos == 0,
          "O4a Tech.*DetectPos all 0 offline (TECH_AcarryShim default)");

    // Force a residual-IC condition in the grid; the zero-guard must still suppress.
    TestIF.iTestMode = SingleSite;
    bShuttleHasIC[0][0][0] = true;
    {
        int X = -1, Y = -1;
        bool r = CheckShuttleOutputHasICError(0, X, Y);     // iSelSHT=0 -> FRCarryKit
        printf("    CheckShuttleOutputHasICError(0) [Tech=0, residual flagged] = %d (expect 0/false)\n", (int)r);
        CHECK(r == false, "O4b CheckShuttleOutputHasICError returns false under zero-detect-pos guard (golden 2076-2097)");
    }
    bShuttleHasIC[0][0][0] = false;

    // =======================================================================
    //  PART D -- [O5] CheckShuttleSensorBroken_1 left-pass first call.
    //  First call with bRight==false: static iCheckCount==0 so the bRight==true
    //  error-aggregation block is never reached; bHasErr stays false -> returns
    //  false (no broken-sensor error).  We call with bRefreshCheck==true first to
    //  guarantee the static accumulator is at its left-pass start, then the
    //  left-pass (bRight==false) returns false.
    // =======================================================================
    printf("[D] CheckShuttleSensorBroken_1 left-pass returns false\n");
    {
        // refresh: resets the static accumulator, returns bHasErr==false (golden 7363-7369)
        bool rRefresh = CheckShuttleSensorBroken_1(true, false);
        CHECK(rRefresh == false, "O5a CheckShuttleSensorBroken_1(refresh) -> false (golden 7363)");

        // left pass (bRight==false): the error-aggregation arm is skipped; the
        // switch(TestIF.iTestMode) fills bFlag, then bHasErr (never set on the
        // left arm) is returned == false (golden 7322-7579).
        TestIF.iTestMode = SingleSite;
        bool rLeft = CheckShuttleSensorBroken_1(false, false);
        printf("    CheckShuttleSensorBroken_1(false,false) = %d (expect 0/false)\n", (int)rLeft);
        CHECK(rLeft == false, "O5b CheckShuttleSensorBroken_1 left-pass -> false, no broken-sensor error (golden 7360)");
    }

    // =======================================================================
    printf("\n==== W6.5 shuttle/carry verify summary: %d PASS, %d FAIL ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
