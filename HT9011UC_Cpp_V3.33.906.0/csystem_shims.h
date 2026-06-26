// =============================================================================
//  csystem_shims.h  --  HT9045 system-HUB offline link shims (W6.6)
//
//  Translation wave: W6.6 (csystem orchestration SPINE)
//  Author: AI(W6.6-HUB) 20260626
//
//  ROLE
//  ----
//  csystem.cpp (the orchestration HUB) drives the five already-translated
//  engines through the per-tick DoAllProcess() loop.  A handful of cross-module
//  free functions the HUB calls live in BCB6 translation units that are NOT
//  translated this wave (ainarm2.cpp / aoutarm.cpp / asendic_Loader.cpp /
//  asortarm.cpp + several Auto-tray/Magazine init aggregators).  This file is
//  the ACTIVE, transparently-named offline stand-in for exactly those symbols,
//  mirroring the role the engine *_shims.cpp files already play.  Every body is
//  a faithful-but-offline shim (no-op / cursor reset / thin dispatcher to the
//  real translated engine entry), so the HUB LINKS and the cycle actually
//  pumps -- nothing here is #if 0.
//
//  The DoInArm / DoOutArm shims are thin dispatchers that, after the golden
//  pre-guards, call the REAL translated engine entries (DoInArm_9045 /
//  DoOutArm_9045) so the in/out-arm engines are genuinely exercised by the
//  main loop, not stubbed away.
// =============================================================================
#ifndef csystem_shimsH
#define csystem_shimsH

#include "vclcompat/vcl_compat.h"   // AnsiString

// ---------------------------------------------------------------------------
//  Home of bShuttleShake (golden ainarm2.cpp:78) -- read by DoAllProcess to
//  skip the shuttle/index engines while the tray arm shakes a tray loose.
//  Offline default false (no shake), so the engines pump normally.
// ---------------------------------------------------------------------------
extern bool bShuttleShake;

// ---------------------------------------------------------------------------
//  Per-tick engine dispatchers the HUB calls (golden homes in comments).
//  DoInArm  : ainarm2.cpp:1620  -> guards + DoInArm_9045()
//  DoOutArm : aoutarm.cpp:1195  -> kit SetHasNullIcToNullIc + DoOutArm_9045()
//  DoLoad   : asendic_Loader.cpp:2448 -> offline no-op (loader feed not in scope)
//  DoSortArm: asortarm.cpp:349  -> offline no-op (9046AU only; default uninstall)
// ---------------------------------------------------------------------------
void DoInArm();
void DoOutArm();
void DoLoad();
void DoSortArm();

// ---------------------------------------------------------------------------
//  InitAllProcessTask() leaf aggregators with no translated home yet.
//  (The translated engines already own InitInArmTask / Initial_Auto_SHT1/2_Task /
//   InitialTestHeadMotorTask / InitialCatchTrayTask / InitAutoEmptyTask /
//   InitFrontTestSuckICTask / InitBTestSuckTestICTask.)  These remaining leaves
//   are offline cursor/flag resets.
// ---------------------------------------------------------------------------
void InitLoadTask();                       // golden asendic_Loader.cpp
void InitOutArmTask();                      // golden aoutarm.h:34 (file-local static there; this is the global linkable one)
void InitialDoLockUnloader(int iAuto);      // golden csystem.h:258 (real body deferred W7)
void Initial_Auto_BinTray_Task(int iAuto);  // golden asendic_*.cpp
void InitAutoColorTask();                   // golden asendic_Color.cpp
void InitialAuto3MagazineTask();            // golden Magazine SM
void InitialCatchTrayChangeTray();          // golden Magazine SM
void InitialCatchTrayGetNewTray();          // golden Magazine SM
void InitialDoMagazineTrayFeedTask();       // golden Magazine SM

// ---------------------------------------------------------------------------
//  Tail Auto-tray / alignment / vibration loops the HUB calls every tick.
//  (DoAutoEmpty / DoAutoColor / DoAutoReceiveBinTray already have homes in
//   asendic_Empty.cpp / acatchtray_shims.cpp; DoHotplateEdgeCylinderLoop /
//   DoLoaderVibrateLoop are declared in csystem.h and realized in csystem.cpp.)
// ---------------------------------------------------------------------------
void DoAuto3Magazine();                                       // golden Magazine SM
void NewDoAutoTrayEdgeCylinderLoop();                          // golden tray-vibration loop
void DoAuto2();                                               // golden asendic -- auto2 tray feed
// NOTE: DoAutoEmpty1() is OWNED by asendic_Empty.cpp (declared asendic_Empty.h:33);
// csystem.cpp calls it via that header.  NOT shimmed here (avoid ODR).
bool DoInArmTeachAlignmentProcess(unsigned long &lAction);    // golden AutoAlignment.h:225 -- offline: complete (true)
bool DoOutArmTeachAlignmentProcess(unsigned long &lAction);   // golden AutoAlignment.h:252 -- offline: complete (true)
void SetFixTrayMiddleDtata();                                 // golden aoutarm.cpp -- Fix-tray middle data (offline no-op)

// ---------------------------------------------------------------------------
//  End-of-lot bookkeeping FSMs the HUB ticks after DoAllProcess (golden
//  csystem.cpp:12813 / :14713 -- ~2900 lines).  Bodies are GATED #if 0 in
//  csystem.cpp; these ACTIVE no-op stubs keep the loop pumping (NOT spine).
// ---------------------------------------------------------------------------
void DoOneCycleFinishCheck();                                 // golden csystem.cpp:12813 (gated; offline no-op)
void DoCleanOutFinishCheck();                                 // golden csystem.cpp:14713 (gated; offline no-op)

#endif
