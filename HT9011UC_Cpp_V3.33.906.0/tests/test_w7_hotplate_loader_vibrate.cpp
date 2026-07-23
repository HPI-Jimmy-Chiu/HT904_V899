// =============================================================================
//  test_w7_hotplate_loader_vibrate.cpp -- W7 VERIFY: HotPlate edge-cylinder
//  knock loop + Loader vibration-motor loop (un-gated tail loops, csystem.cpp)
//
//  Translation wave: W7 (un-gate DoHotplateEdgeCylinderLoop golden csystem.cpp
//                    :20323-20388 + DoLoaderVibrateLoop golden csystem.cpp
//                    :20391-20432, plus their file-scope TQPF_Timer globals at
//                    golden :20321-20322 / :20390).  Both were ZERO-BLOCKED --
//                    every symbol they touch (Cylinder[]/SW[]/MOT[]/IniConfig/
//                    LastSet/Ld_UldDelayTime) already had a real translated home.
//  Author: AI(ht9045-v906) 20260722
//  Suite name (add_test): W7_HotplateLoaderVibrate
//
//  PURPOSE
//  -------
//  Drives both un-gated tail loops over the Sim HAL through a couple of real
//  ticks each, toggling the relevant globals and asserting the Cylinder[]/SW[]
//  end state + the TQPF_Timer on/off transitions against values HAND-DERIVED
//  from the golden.  Both functions gate all real work behind a function-local
//  static `iCT` that must reach 10 ticks before doing anything (golden
//  :20343-20347 / :20405-20409) -- every guard in this TU that must NOT
//  perform real work is proven by then completing the normal 10-tick cadence
//  starting from that same point (if the guard had leaked a partial increment,
//  the cadence would need fewer/more than 10 ticks to fire).
//
//  TEST-HARNESS NOTE on the Sim IO HAL: TMyCylinder/TMySwitch route their
//  On()/Off()/Status()/GetOutBit() through the SAME MyLaneIO OutPortData[Ring]
//  [IP][Port] byte cache, keyed by (Ring,IP,Port,Bit).  Both TMyCylinder and
//  TMySwitch default-construct to Ring=IP=Port=Bit=0 -- so this TU explicitly
//  assigns the two globals under test DISTINCT OutBit/Bit indices before use,
//  so the two loops' simulated IO never stomps on each other's bit within this
//  one test process.  TMySwitch::Status() also inverts when Type==0 (golden
//  myswitch.cpp: `if(Type) return OutValue; else return !OutValue;`) -- this TU
//  sets SW[SwLoaderVibration].Type=1 so Status() mirrors OutValue directly.
//  Both choices are pure test-harness wiring (same category as Ring/IP/Port/
//  Bit/Enable, all populated by the config loader in production) -- neither
//  touches the translated golden LOGIC under test.
//
//  ORACLES (hand-derived from the golden csystem.cpp):
//    O1 HOTPLATE guards (golden :20329-20341): SystemStart==false /
//       Cylinder[C_HotplateVibration].Enable==false /
//       IniConfig.bP16EnableHotplateEdgePushCylinderLoop==false / the ambient-
//       temperature guard all early-return with ZERO side effect and, load-
//       bearing, do NOT advance the function-local iCT counter -- proven by
//       then running the normal 10-tick cadence from this same point.
//    O2 HOTPLATE 10-tick cadence + bFirst clamp (golden :20343-20366): the
//       first 9 ticks are no-ops; the 10th tick clamps a <2 LoopDelay up to 2
//       (golden :20361-20362) and arms HotplateEdgeOffDelay -- Cylinder stays
//       OFF (timer just armed, not yet elapsed).
//    O3 HOTPLATE off-delay elapses -> On() (golden :20368-20377): after a real
//       sleep past the loop-delay, MOT[MMPlate1].HasRealIC()==true drives
//       Cylinder[C_HotplateVibration].On(), arms HotplateEdgeOnDelay, and
//       increments IniConfig.iVibratorHP1 by int(OnDelay/10.0) -- preserved
//       VERBATIM including the truncation quirk (a decisecond OnDelay of 3
//       (=0.3) truncates to 0; not "fixed", per task instructions).
//    O4 HOTPLATE on-delay elapses -> Off() (golden :20379-20387): after a real
//       sleep past the on-delay, Cylinder[C_HotplateVibration].Off() again.
//    O5 HOTPLATE bPickFromHotplate+VibrationMotor guard (golden :20349-20356):
//       with the cylinder physically ON (GetOutBit()==true), setting
//       bPickFromHotplate=true and TRAY_VIBRATION==VibrationMotor forces
//       Cylinder.Off() on the next real tick.  NOTE (preserved verbatim, not
//       "fixed"): the golden `return` on this path is BEFORE the `iCT=0;`
//       reset (golden :20358), so iCT is left >=10 and the very next tick
//       reaches the body immediately with no further 10-tick wait -- see the
//       matching comment left in csystem.cpp at the un-gated site.
//    O6 LOADER guards (golden :20396-20403): SystemStart==false /
//       SW[SwLoaderVibration].Enable==false / Ld_UldDelayTime.LD_EnableVibrate
//       ==false all early-return, zero side effect, no iCT advance.
//    O7 LOADER 10-tick cadence + On() (golden :20405-20421): first 9 ticks
//       no-op; 10th tick (bLoaderNeedVibrate==true) consumes the flag, turns
//       SW[SwLoaderVibration] ON and arms LoaderVibrateOnDelay -- switch STAYS
//       on (timer just armed, not yet elapsed).
//    O8 LOADER on-delay elapses -> Off() (golden :20424-20430): after a real
//       sleep past LD_VibrateOnDelay, the switch turns back OFF.
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" == clean g++
//  compile/link + both loops reproduce the golden's tick-cadence / timer-arm /
//  HAL end-state exactly as hand-derived above, including the OnDelay/10.0
//  truncation quirk.
// =============================================================================
#include "csystem.h"                // DoHotplateEdgeCylinderLoop / DoLoaderVibrateLoop
#include "cmydef.h"                 // SystemStart / Tempture_Hot|Ambient|AmbientHot /
                                     // C_HotplateVibration / SwLoaderVibration / TRAY_VIBRATION /
                                     // VibrationMotor / NonVibration / bLoaderNeedVibrate / HAS_IC /
                                     // MMPlate1 / MMPlate2 / bAmbientHotPlate
#include "canary_support.h"         // LastSet (LAST_GENERAL_SET)
#include "aHotPlateSubstrate.h"     // bPickFromHotplate
#include "Config.h"                 // IniConfig
#include "cprod.h"                  // Ld_UldDelayTime
#include "mycylin.h"                // Cylinder[]
#include "myswitch.h"               // SW[]
#include "Motor/mymotor.h"          // MOT[]
#include <windows.h>                // Sleep
#include <cstdio>

static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

static void PumpHotplate(int n)
{
    for (int i = 0; i < n; ++i)
        DoHotplateEdgeCylinderLoop();
}

static void PumpLoader(int n)
{
    for (int i = 0; i < n; ++i)
        DoLoaderVibrateLoop();
}

int main()
{
    printf("==== W7 HotplateLoaderVibrate verify (tail cylinder/vibration loops) ====\n");

    // =========================================================================
    //  PART A -- DoHotplateEdgeCylinderLoop
    // =========================================================================
    printf("[A] DoHotplateEdgeCylinderLoop\n");

    // Distinct HAL bit so this loop's simulated IO never collides with the
    // loader switch tested in Part B (see TEST-HARNESS NOTE above).
    Cylinder[C_HotplateVibration].OutBit = 3;
    Cylinder[C_HotplateVibration].Off();
    CHECK(Cylinder[C_HotplateVibration].Status == false,
          "setup: Cylinder[C_HotplateVibration] baseline OFF");

    // -- O1: the four top guards -- each must be a clean no-op AND must not
    //    advance the function-local iCT (golden :20329-20341).
    printf("[A-O1] four top guards: zero side effect, no iCT advance (golden :20329-20341)\n");
    Cylinder[C_HotplateVibration].Enable = true;
    IniConfig.bP16EnableHotplateEdgePushCylinderLoop = true;
    LastSet.iTemperature = Tempture_Hot;      // != Ambient/AmbientHot -> ambient guard passes
    bAmbientHotPlate = true;

    SystemStart = false;
    PumpHotplate(20);
    CHECK(Cylinder[C_HotplateVibration].Status == false,
          "O1a SystemStart==false -> no-op (golden :20329-20330)");
    SystemStart = true;

    Cylinder[C_HotplateVibration].Enable = false;
    PumpHotplate(20);
    CHECK(Cylinder[C_HotplateVibration].Status == false,
          "O1b Cylinder Enable==false -> no-op (golden :20332-20333)");
    Cylinder[C_HotplateVibration].Enable = true;

    IniConfig.bP16EnableHotplateEdgePushCylinderLoop = false;
    PumpHotplate(20);
    CHECK(Cylinder[C_HotplateVibration].Status == false,
          "O1c bP16EnableHotplateEdgePushCylinderLoop==false -> no-op (golden :20335-20336)");
    IniConfig.bP16EnableHotplateEdgePushCylinderLoop = true;

    LastSet.iTemperature = Tempture_Ambient;
    bAmbientHotPlate = false;
    PumpHotplate(20);
    CHECK(Cylinder[C_HotplateVibration].Status == false,
          "O1d ambient-temp guard (Ambient/AmbientHot && !bAmbientHotPlate) -> no-op (golden :20338-20341)");
    LastSet.iTemperature = Tempture_Hot;
    bAmbientHotPlate = true;

    // -- O2: 10-tick cadence + bFirst LoopDelay<2 clamp (golden :20343-20366).
    //    80 guarded calls above never advanced iCT (all 4 guards return before
    //    `iCT++`) -- so this is genuinely the FIRST 10-tick cadence.
    printf("[A-O2] 10-tick cadence + LoopDelay<2 clamp (golden :20343-20366)\n");
    bPickFromHotplate = false;
    TRAY_VIBRATION = NonVibration;
    IniConfig.iP16HotplateEdgePushCylinderLoopDelay = 1;   // < 2 -> must clamp to 2 (golden :20361-20362)
    IniConfig.iP16HotplateEdgePushCylinderOnDelay   = 3;   // 0.3s -> int(3/10.0)==0 truncation (O3)
    MOT[MMPlate1].fHasTray = true;
    MOT[MMPlate1].Tray.Data[0][0] = HAS_IC;                // XItem/YItem default 1x1 -> in scan range
    MOT[MMPlate2].fHasTray = false;                        // isolate: only MMPlate1 carries an IC
    int vibratorHP1Before = IniConfig.iVibratorHP1;

    PumpHotplate(9);
    CHECK(Cylinder[C_HotplateVibration].Status == false,
          "O2a first 9 ticks are no-ops (iCT<10, golden :20344-20347)");

    PumpHotplate(1);   // 10th tick: bFirst branch fires, arms HotplateEdgeOffDelay
    CHECK(Cylinder[C_HotplateVibration].Status == false,
          "O2b 10th tick arms HotplateEdgeOffDelay -- Cylinder still OFF (not yet elapsed)");
    CHECK(IniConfig.iP16HotplateEdgePushCylinderLoopDelay == 2,
          "O2c LoopDelay clamped 1->2 (golden :20361-20362)");

    Sleep(300);   // > 200ms (LoopDelay=2 deciseconds) so HotplateEdgeOffDelay elapses

    // -- O3: off-delay elapsed -> On(), truncation quirk on iVibratorHP1.
    printf("[A-O3] off-delay elapses -> Cylinder On() + iVibratorHP1 truncation quirk (golden :20368-20377)\n");
    PumpHotplate(9);
    CHECK(Cylinder[C_HotplateVibration].Status == false,
          "O3a still no-op through tick 9 of this cadence");
    PumpHotplate(1);   // 10th tick: off-delay elapsed, MOT[MMPlate1].HasRealIC() true -> On()
    CHECK(Cylinder[C_HotplateVibration].Status == true,
          "O3b off-delay elapsed + HasRealIC -> Cylinder On() (golden :20370-20376)");
    CHECK(IniConfig.iVibratorHP1 == vibratorHP1Before + 0,
          "O3c iVibratorHP1 += int(OnDelay/10.0) == int(0.3) == 0 -- truncation preserved VERBATIM, not fixed (golden :20374)");

    Sleep(400);   // > 300ms (OnDelay=3 deciseconds) so HotplateEdgeOnDelay elapses

    // -- O4: on-delay elapsed -> Off() again.
    printf("[A-O4] on-delay elapses -> Cylinder Off() again (golden :20379-20387)\n");
    PumpHotplate(9);
    CHECK(Cylinder[C_HotplateVibration].Status == true,
          "O4a still ON through tick 9 of this cadence");
    PumpHotplate(1);   // 10th tick: on-delay elapsed -> Off(), re-arm off-delay
    CHECK(Cylinder[C_HotplateVibration].Status == false,
          "O4b on-delay elapsed + HasRealIC -> Cylinder Off() again (golden :20381-20386)");

    // -- O5: bPickFromHotplate + TRAY_VIBRATION==VibrationMotor forces Off()
    //    when the physical bit reads ON (golden :20349-20356).
    printf("[A-O5] bPickFromHotplate+VibrationMotor guard forces Off() when GetOutBit()==true (golden :20349-20356)\n");
    PumpHotplate(9);   // reach the next cadence boundary with the guard still disarmed
    Cylinder[C_HotplateVibration].On();     // force the cylinder physically ON for this probe
    CHECK(Cylinder[C_HotplateVibration].Status == true && Cylinder[C_HotplateVibration].GetOutBit() == true,
          "setup: Cylinder forced ON directly, GetOutBit() reflects it");
    bPickFromHotplate = true;
    TRAY_VIBRATION = VibrationMotor;
    PumpHotplate(1);   // 10th tick: pick-guard fires -> Off() + return (skips iCT=0 reset, verbatim quirk)
    CHECK(Cylinder[C_HotplateVibration].Status == false,
          "O5 pick-guard forces Cylinder Off() when bPickFromHotplate && TRAY_VIBRATION==VibrationMotor (golden :20349-20356)");
    bPickFromHotplate = false;
    TRAY_VIBRATION = NonVibration;

    // =========================================================================
    //  PART B -- DoLoaderVibrateLoop
    // =========================================================================
    printf("[B] DoLoaderVibrateLoop\n");

    // Distinct HAL bit (see TEST-HARNESS NOTE); Type=1 so Status() mirrors
    // OutValue directly (golden myswitch.cpp inverts on Type==0).
    SW[SwLoaderVibration].Bit  = 4;
    SW[SwLoaderVibration].Type = 1;
    SW[SwLoaderVibration].Enable = true;
    SW[SwLoaderVibration].Off();
    CHECK(SW[SwLoaderVibration].Status() == false,
          "setup: SW[SwLoaderVibration] baseline OFF");

    // -- O6: the three top guards -- clean no-op, no iCT advance (golden :20396-20403).
    printf("[B-O6] three top guards: zero side effect, no iCT advance (golden :20396-20403)\n");
    Ld_UldDelayTime.LD_EnableVibrate = true;

    SystemStart = false;
    PumpLoader(20);
    CHECK(SW[SwLoaderVibration].Status() == false,
          "O6a SystemStart==false -> no-op (golden :20396-20397)");
    SystemStart = true;

    SW[SwLoaderVibration].Enable = false;
    PumpLoader(20);
    CHECK(SW[SwLoaderVibration].Status() == false,
          "O6b SW[SwLoaderVibration].Enable==false -> no-op (golden :20399-20400)");
    SW[SwLoaderVibration].Enable = true;

    Ld_UldDelayTime.LD_EnableVibrate = false;
    PumpLoader(20);
    CHECK(SW[SwLoaderVibration].Status() == false,
          "O6c Ld_UldDelayTime.LD_EnableVibrate==false -> no-op (golden :20402-20403)");
    Ld_UldDelayTime.LD_EnableVibrate = true;

    // -- O7: 10-tick cadence -> On() (golden :20405-20421).  The 60 guarded
    //    calls above never advanced iCT, so this is genuinely the first cadence.
    printf("[B-O7] 10-tick cadence -> SW On() + bLoaderNeedVibrate consumed (golden :20405-20421)\n");
    Ld_UldDelayTime.LD_VibrateOnDelay = 5;   // 0.5s
    bLoaderNeedVibrate = true;

    PumpLoader(9);
    CHECK(SW[SwLoaderVibration].Status() == false,
          "O7a first 9 ticks are no-ops (iCT<10, golden :20406-20409)");

    PumpLoader(1);   // 10th tick: bLoaderNeedVibrate consumed, SW On(), arm LoaderVibrateOnDelay
    CHECK(SW[SwLoaderVibration].Status() == true,
          "O7b 10th tick -> SW[SwLoaderVibration] On() (golden :20417-20420)");
    CHECK(bLoaderNeedVibrate == false,
          "O7c bLoaderNeedVibrate consumed (golden :20415)");

    Sleep(600);   // > 500ms (LD_VibrateOnDelay) so LoaderVibrateOnDelay elapses

    // -- O8: on-delay elapsed -> Off() again (golden :20424-20430).
    printf("[B-O8] on-delay elapses -> SW Off() again (golden :20424-20430)\n");
    PumpLoader(9);
    CHECK(SW[SwLoaderVibration].Status() == true,
          "O8a still ON through tick 9 of this cadence");
    PumpLoader(1);   // 10th tick: LoaderVibrateOnDelay elapsed -> Off()
    CHECK(SW[SwLoaderVibration].Status() == false,
          "O8b on-delay elapsed -> SW[SwLoaderVibration] Off() again (golden :20426-20429)");

    printf("\n==== W7 HotplateLoaderVibrate verify summary: %d PASS, %d FAIL ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
