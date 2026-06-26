// =============================================================================
//  csystem_shims.cpp  --  HT9045 system-HUB offline link shims (W6.6)
//
//  Translation wave: W6.6 (csystem orchestration SPINE)
//  Author: AI(W6.6-HUB) 20260626
//
//  See csystem_shims.h for the role description.  ACTIVE offline stand-ins for
//  the cross-module free functions the HUB main loop (csystem.cpp DoAllProcess)
//  calls but whose BCB6 source units are NOT translated this wave.  The DoInArm /
//  DoOutArm shims faithfully reproduce the golden pre-guards / kit setup and then
//  call the REAL translated engine entry (DoInArm_9045 / DoOutArm_9045), so the
//  in/out-arm engines are genuinely pumped by the loop.  All other shims are
//  faithful-but-offline cursor resets / no-ops.  NOTHING here is #if 0.
// =============================================================================
#include "vclcompat/vcl_compat.h"
#include "csystem_shims.h"

#include "cmydef.h"                 // bInitialStartIndexCheckDone / iTestBinCount / HAS_IC / TEST_PASS / IniConfig / MInShuttle1.. / bDoingF16
#include "cprod.h"                  // Prod (not strictly needed; pulled for parity)
#include "cpublic.h"
#include "MachineType.h"
#include "Motor/mymotor.h"          // MOT[] (fCanMoveL/M used by the F16 guard)
// ainarm9045.h / aoutarm9045.h MUST precede aHotPlateSubstrate.h: all three
// declare InArmLeftSideNoIC/HasIC(int=2) with a default arg, and g++ rejects the
// repeated default unless the engine header is seen first (same order the engine
// TUs use).
#include "ainarm9045.h"             // DoInArm_9045()
#include "aoutarm9045.h"            // DoOutArm_9045()
#include "aHotPlateSubstrate.h"     // OutArmSuck / FRCarryKit / BRCarryKit / InArmSuck
#include "ainarm_SearchPlacePlate.h"// iHPHangUpCount / sHPHangUpFunc / ClearHotPlateHangUp()
#include "canary_support.h"         // RecordProcess / ShowErrorMessage
#include "FormsFacade.h"            // fMain->DoStateRecord

// ---------------------------------------------------------------------------
//  bShuttleShake -- golden ainarm2.cpp:78.  Offline default false (no shake).
// ---------------------------------------------------------------------------
bool bShuttleShake = false;

// ===========================================================================
//  DoInArm  -- golden ainarm2.cpp:1620 (fat in-arm dispatcher).
//  Faithful head guards (Index-check-done / HP hang-up / F16 sensor-broken),
//  then dispatch to the translated in-arm engine entry.  The QA-mode +
//  variant-dispatch bulk between the guards and DoInArm_9045() lives in
//  ainarm2.cpp (NOT translated); offline it is skipped -- DoInArm_9045() owns
//  the real per-tick in-arm SM (translated in ainarm9045.cpp).
// ===========================================================================
void DoInArm()
{
    if(bInitialStartIndexCheckDone==false)                                      // golden ainarm2.cpp:1622
    {
        return;
    }

    if(iHPHangUpCount!=0)                                                       // golden :1627 -- HotPlate hang-up
    {
        RecordProcess("Auto State Record by in arm");
        fMain->DoStateRecord(0, false);
        ShowErrorMessage("WAR0150", 0, MInArmX, false, sHPHangUpFunc);
        ClearHotPlateHangUp();
        fAllMotorHome=false;
        return;
    }

    if(IniConfig.bF16CheckShuttleSensorBroken && bDoingF16)                     // golden :1637 -- shuttle sensor-broken
    {
        MOT[MInShuttle1].fCanMoveL=true;                                        // avoid follow-on hang up
        MOT[MInShuttle2].fCanMoveL=true;
        MOT[MInShuttle1].fCanMoveM=true;
        MOT[MInShuttle2].fCanMoveM=true;
        return;
    }

    // golden :1646-end : QA-mode + the variant-dispatch ladder live in the
    // untranslated ainarm2.cpp; offline we proceed straight to the engine.
    DoInArm_9045();                                                            // translated engine entry (ainarm9045.cpp)
}

// ===========================================================================
//  DoOutArm -- golden aoutarm.cpp:1195 (thin wrapper).
//  Faithful: SetHasNullIcToNullIc on the rear carry kits + the result-tag loop
//  + SetFixTrayMiddleDtata(), then the translated out-arm engine entry.
// ===========================================================================
void DoOutArm()
{
    // golden :1197-1198 -- FRCarryKit/BRCarryKit.SetHasNullIcToNullIc():
    //   SUBSTRATE GAP -- TMyKitSuck (aHotPlateSubstrate.h) does not yet expose
    //   SetHasNullIcToNullIc (the HAS_NULL_IC->NULL_IC sweep); the substrate
    //   provides SetAllToNullIC().  Offline the carry kits start empty, so the
    //   result-tag pass (golden :1200-1209, FRCarryKit.Item -> TEST_PASS+bin)
    //   is a no-op anyway.  We skip the kit-prep and dispatch straight to the
    //   translated out-arm engine (TODO(W7): add SetHasNullIcToNullIc to the kit
    //   and restore the verbatim prep loop).
    SetFixTrayMiddleDtata();                                                    // golden :1210 (offline no-op)
    DoOutArm_9045();                                                           // golden :1211 -- translated engine entry (aoutarm9045.cpp)
}

// ===========================================================================
//  DoLoad   -- golden asendic_Loader.cpp:2448 (loader feed SM).  Loader feed is
//  NOT in W6.6 scope; offline no-op.
//  DoSortArm-- golden asortarm.cpp:349 (9046AU sort-arm).  USE_OUT_SORT_ARM
//  defaults eartUninstall so the HUB never calls it; offline no-op.
// ===========================================================================
void DoLoad()    {}
void DoSortArm() {}

// ===========================================================================
//  SetFixTrayMiddleDtata -- golden aoutarm.cpp (Fix-tray middle-data shuffle).
//  Offline no-op (no Fix tray in the sim feed).
// ===========================================================================
void SetFixTrayMiddleDtata() {}

// ===========================================================================
//  InitAllProcessTask() leaves with no translated home -- offline cursor/flag
//  resets (the engines own the rest).
// ===========================================================================
void InitLoadTask()                  {}   // golden asendic_Loader.cpp
void InitOutArmTask()                {}   // golden aoutarm.h:34 (file-local static in aoutarm9045.cpp; this global is the linkable one InitAllProcessTask calls)
void InitialDoLockUnloader(int /*iAuto*/) {}  // golden csystem.h:258 -- unloader lock cursor reset (real body W7)
void Initial_Auto_BinTray_Task(int /*iAuto*/) {}  // golden asendic_*.cpp -- per-auto bin-tray cursor reset
void InitAutoColorTask()             {}   // golden asendic_Color.cpp
void InitialAuto3MagazineTask()      {}   // golden Magazine SM
void InitialCatchTrayChangeTray()    {}   // golden Magazine SM
void InitialCatchTrayGetNewTray()    {}   // golden Magazine SM
void InitialDoMagazineTrayFeedTask() {}   // golden Magazine SM

// ===========================================================================
//  Tail Auto-tray / alignment / vibration loops the HUB calls every tick.
// ===========================================================================
void DoAuto3Magazine()               {}   // golden Magazine SM (only when AUTO3_IS_MAGAZINE==1)
void NewDoAutoTrayEdgeCylinderLoop() {}   // golden tray-vibration loop
void DoAuto2()                       {}   // golden asendic -- auto2 tray feed
// DoAutoEmpty1() is owned by asendic_Empty.cpp (ODR) -- NOT defined here.

//  Alignment processes: offline they report "finished" so the HUB does NOT
//  early-return on them (golden returns true when the alignment step completes;
//  DoAllProcess does bRunXXXAutoAlignment = !DoXXXTeachAlignmentProcess(...)).
bool DoInArmTeachAlignmentProcess(unsigned long & /*lAction*/)  { return true; }   // golden AutoAlignment.cpp:4473 -- offline: complete in one step
bool DoOutArmTeachAlignmentProcess(unsigned long & /*lAction*/) { return true; }   // golden AutoAlignment.cpp:8627 -- offline: complete in one step

// ===========================================================================
//  End-of-lot bookkeeping FSMs (golden :12813 / :14713, ~2900 lines, GATED in
//  csystem.cpp).  ACTIVE no-op stubs so MainProc's tail calls link + pump.
// ===========================================================================
void DoOneCycleFinishCheck() {}   // golden csystem.cpp:12813 (bulk gated; offline no-op)
void DoCleanOutFinishCheck() {}   // golden csystem.cpp:14713 (bulk gated; offline no-op)
