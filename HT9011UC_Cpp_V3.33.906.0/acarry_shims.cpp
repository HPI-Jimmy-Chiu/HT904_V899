// =============================================================================
//  acarry_shims.cpp  --  definitions for the W6.5 SHUTTLE/CARRY engine offline shims
//
//  Translation wave: W6.5 (SHUTTLE/CARRY ENGINE -- acarry.cpp)
//  Translator: AI(W6.5-CARRY) 20260626
//
//  All bodies are offline-safe (false / 0 / no-op / empty grid) so the shuttle
//  state machines pump over the Sim HAL and take the deterministic canary
//  branches (no floating, no HTTP fail, no 9046AU sort-arm, no ATC flood-gate,
//  no latch events).  See acarry_shims.h for the golden-home documentation.
//
//  Big5: Chinese comments decoded via cp950, preserved UTF-8.  ZERO U+FFFD.
// =============================================================================
#include "acarry_shims.h"
#include "csystem.h"      // InSHT1InLF / InSHT2InLF / InSHT1InRT / InSHT2InRT (real predicates)
#include "cmydef.h"       // K_RESET (not used here, but keeps constant set consistent)
#include <cstring>

// ---------------------------------------------------------------------------
//  TECH Tech -- AI(W906-GA1-B1) 20260804: shim ctor+definition RETIRED; the
//  real definition lives in LastSet.cpp (golden LastSet.cpp).
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  fLtcSensor (TfLtcSensor)  -- golden LtcSensor.h.  All latch counts = 0 offline.
// ---------------------------------------------------------------------------
TfLtcSensor::TfLtcSensor()
{
    LatchDataCnt0 = LatchDataCnt1 = LatchDataCnt2 = LatchDataCnt3 = LatchDataCnt4 = 0;
    LatchDataCnt5 = LatchDataCnt6 = LatchDataCnt7 = LatchDataCnt8 = LatchDataCnt9 = 0;
    std::memset(LatchDataTable0, 0, sizeof(LatchDataTable0));
    std::memset(LatchDataTable1, 0, sizeof(LatchDataTable1));
    std::memset(LatchDataTable2, 0, sizeof(LatchDataTable2));
    std::memset(LatchDataTable3, 0, sizeof(LatchDataTable3));
    std::memset(LatchDataTable4, 0, sizeof(LatchDataTable4));
    std::memset(LatchDataTable5, 0, sizeof(LatchDataTable5));
    std::memset(LatchDataTable6, 0, sizeof(LatchDataTable6));
    std::memset(LatchDataTable7, 0, sizeof(LatchDataTable7));
    std::memset(LatchDataTable8, 0, sizeof(LatchDataTable8));
    std::memset(LatchDataTable9, 0, sizeof(LatchDataTable9));
}
int  TfLtcSensor::GetLtcSensor(int /*iWhich*/)   { return 0; }   // offline: no latch read
void TfLtcSensor::SetLtcSensor(int /*iWhich*/)   {}              // offline: no-op
void TfLtcSensor::ClearLtcSensor(int iWhich)
{
    // offline: clear the matching latch count (defensive; counts are already 0)
    switch(iWhich)
    {
        case 0: LatchDataCnt0=0; break;
        case 1: LatchDataCnt1=0; break;
        case 2: LatchDataCnt2=0; break;
        case 3: LatchDataCnt3=0; break;
        case 4: LatchDataCnt4=0; break;
        case 5: LatchDataCnt5=0; break;
        case 6: LatchDataCnt6=0; break;
        case 7: LatchDataCnt7=0; break;
        case 8: LatchDataCnt8=0; break;
        case 9: LatchDataCnt9=0; break;
        default: break;
    }
}
TfLtcSensor *fLtcSensor = new TfLtcSensor();            // golden LtcSensor.h

// ---------------------------------------------------------------------------
//  OutSht3Kit (TMyKitSuck)  -- golden MyKitSuck.h (9046AU sort kit).  Sole def.
// ---------------------------------------------------------------------------
TMyKitSuck OutSht3Kit;                                  // golden MyKitSuck.h

// ---------------------------------------------------------------------------
//  ATC_InterfaceForm / ATC_Data  -- golden ATC_Handler_Side.h.  No ATC offline.
// ---------------------------------------------------------------------------
TATC_InterfaceFormShim::TATC_InterfaceFormShim() : iATC_MODE_TYPE(0) {}
TATC_InterfaceFormShim *ATC_InterfaceForm = new TATC_InterfaceFormShim();
TATC_DataShim::TATC_DataShim() : dChillerSetTemp(0.0) {}
TATC_DataShim ATC_Data;

// ---------------------------------------------------------------------------
//  Shuttle-floating detectors (golden LaserSensorShuttle.h / csystem.cpp).
// ---------------------------------------------------------------------------
bool CheckShtFloating(int /*iSht*/, bool /*bAlarm*/)            { return false; }   // no floating offline
bool UseInArmCheckShtFloating(int /*iSht*/, bool /*bAlarm*/)    { return false; }
bool UseOutArmCheckShtFloating(int /*iSht*/, bool /*bAlarm*/)   { return false; }

// ---------------------------------------------------------------------------
//  In-shuttle LF/RT (int) overloads (golden csystem.cpp) -- route to the real
//  (void) predicates already implemented in csystem_predicates.cpp.
// ---------------------------------------------------------------------------
bool InShtInLF(int iSht) { return (iSht==0) ? InSHT1InLF() : InSHT2InLF(); }     // golden csystem.cpp
bool InShtInRT(int iSht) { return (iSht==0) ? InSHT1InRT() : InSHT2InRT(); }     // golden csystem.cpp

// ---------------------------------------------------------------------------
//  9046AU sort-shuttle position predicates (golden csystem.cpp) -- offline false.
// ---------------------------------------------------------------------------
bool SortSHTInLF() { return false; }                   // golden csystem.cpp (9046AU)
bool SortSHTInRT() { return false; }                   // golden csystem.cpp (9046AU)

// ---------------------------------------------------------------------------
//  In-shuttle step / residual / prominent checks (golden csystem.cpp).
//   - DoStepShuttleCheck: offline no step pending -> true (proceed).
//   - EmptyIC / MustHasIC / Prominent: offline find no anomaly -> true (OK).
// ---------------------------------------------------------------------------
bool DoStepShuttleCheck(int /*iShuttle*/) { return true; }     // golden csystem.cpp
bool DoCheckShuttle1EmptyIC()             { return true; }     // golden csystem.cpp
bool DoCheckShuttle2EmptyIC()             { return true; }     // golden csystem.cpp
bool DoCheckShuttle1MustHasIC()           { return true; }     // golden csystem.cpp
bool DoCheckShuttle2MustHasIC()           { return true; }     // golden csystem.cpp
bool DoCheckShuttle1Prominent()           { return true; }     // golden csystem.cpp
bool DoCheckShuttle2Prominent()           { return true; }     // golden csystem.cpp
void InitialCheckShuttle1MustHasICTask()           {}          // golden csystem.cpp
void InitialCheckShuttle2MustHasICTask()           {}          // golden csystem.cpp
void InitialCheckShuttle1ProminentNoHasICTask()    {}          // golden csystem.cpp
void InitialCheckShuttle2ProminentNoHasICTask()    {}          // golden csystem.cpp
void InitialCheckShuttle1ProminentTask()           {}          // golden csystem.cpp
void InitialCheckShuttle2ProminentTask()           {}          // golden csystem.cpp

// ---------------------------------------------------------------------------
//  32-site enable predicates (golden csystem.cpp) -- offline false.
// ---------------------------------------------------------------------------
bool Check32siteOnlyEnabled24Site() { return false; }  // golden csystem.cpp
bool Check32siteOnlyEnabled4x4Site(){ return false; }  // golden csystem.cpp

// ---------------------------------------------------------------------------
//  out-arm let-shuttle-pass / bottom-2DID HTTP / sort-arm safe-move.
//   - MoveOutArmXY_ToShuttleAlarmArea: offline out-arm already away -> true.
//   - SendHTTPRequest: offline server OK -> 0 (NOT -1, so no retry loop).
//   - MoveSortArmToAutoSafe: 9046AU sort-arm -> no-op offline.
// ---------------------------------------------------------------------------
bool MoveOutArmXY_ToShuttleAlarmArea() { return true; }    // golden aoutarm
int  SendHTTPRequest(int /*iSht*/)     { return 0; }       // golden main -- 0 == OK
void MoveSortArmToAutoSafe()           {}                  // golden 9046AU

// ---------------------------------------------------------------------------
//  Do_Auto_SHT1AsOutArmIsRotater -- golden acarry.h decl-only (no .cpp body).
//  Inert active stub so the declaration resolves.
// ---------------------------------------------------------------------------
void Do_Auto_SHT1AsOutArmIsRotater() {}                    // golden acarry.h decl-only

// ---------------------------------------------------------------------------
//  out-arm cross-module safe-move + offline timing + system-NG / 2x8 step.
//   - MoveOutArmToAutoSafe: offline the out-arm Z reaches the safe position
//     immediately -> true (same posture as aoutarm9045.cpp's file-local variant).
//   - MySleepEx / MySleep: offline no real OS wait; MySleepEx returns 0 (the
//     WAIT_OBJECT_0 "completed" code the kept-verbatim thread/step loops ignore).
//   - SystemNG: false offline (system not in NG -> the SM guards proceed).
//   - iCloseSiteStep_2x8: 0 offline (2x8 close-site geometry baseline).
// ---------------------------------------------------------------------------
bool  MoveOutArmToAutoSafe()                       { return true; }   // golden aoutarm.h:52
// AI(W906-CommonCompletion) 20260721: MySleepEx/MySleep stand-in DEFINITIONS
// REMOVED -- common.cpp now provides the real bodies (golden common.h:260-261
// un-gated this wave), and this file's own header (acarry_shims.h:230-232)
// already declares these with the exact same signature, citing "golden
// common.h:260/261" -- so removing just the definitions here needs zero
// caller changes; the real common.cpp definitions now satisfy every caller
// that previously linked against these offline stand-ins.
bool  SystemNG = false;                            // golden main-side global
//AI(W6.2c-INARM-batch4) 20260626: the placeholder DEFINITION of iCloseSiteStep_2x8
// (was =0) was REMOVED -- ainarm9045_2x8_8.cpp now owns the REAL def (=0, golden
// ainarm9045_2x8_8.cpp:110).  The extern decl in acarry_shims.h:234 still satisfies
// acarry.cpp.  Value identical (0) so numeric behavior is unchanged.

// ---------------------------------------------------------------------------
//  Cross-module GLOBALS + LOG functions owned by aoutarm.cpp / cpublic.cpp /
//  main.cpp (no translated home this wave).  Offline-safe defaults matching the
//  golden initializers (aoutarm.cpp:67-70 all false) + no-op logging.
//    * bCarryControlOutarm1/2 -- golden aoutarm.cpp:67-68 (carry-control hand-off
//      flags; the carry engine WRITES them, the out-arm READS them).  false.
//    * bPickShuttleError       -- golden aoutarm.cpp:70 (shuttle-pick error latch).
//    * OutShuttleLog           -- golden cpublic.cpp:679 (fMain-UI shuttle-sensor
//      log; gated #if 0 in cpublic.cpp -> own an offline no-op here).
//    * LogSoftwareOffTime      -- golden main.cpp (software-off-time logger).
// ---------------------------------------------------------------------------
bool bCarryControlOutarm1 = false;                 // golden aoutarm.cpp:67
bool bCarryControlOutarm2 = false;                 // golden aoutarm.cpp:68
bool bPickShuttleError    = false;                 // golden aoutarm.cpp:70
void OutShuttleLog(bool /*bFlag*/)            {}    // golden cpublic.cpp:679 -- offline no-op
void LogSoftwareOffTime(AnsiString /*Flag*/)  {}    // golden main.cpp -- offline no-op
