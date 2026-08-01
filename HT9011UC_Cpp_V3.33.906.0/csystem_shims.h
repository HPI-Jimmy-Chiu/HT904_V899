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
// =============================================================================
//  AI(W906-W7-L1-Wave0) 20260801 -- SHIM RETIREMENT DEBT owed by THIS file (W7-L1)
//
//  The full cross-file register lives in the matching banner at the top of
//  acatchtray_shims.h; this is the slice that belongs here, repeated so nobody
//  editing csystem_shims.{h,cpp} has to know to look there first.  Wave 0
//  deliberately performs NONE of these: retiring a shim before its owning
//  translated file exists turns a working link into undefined symbols.  Each
//  belongs to the INTEGRATE step of the wave named against it.  Line numbers
//  re-derived by reading csystem_shims.cpp in this pass.
//
//  (Declaration positions are given by NAME, not by line number: this banner
//  itself shifts every line below it, and a stale line citation is exactly the
//  kind of claim this project keeps having to correct.)
//
//    * DoLoad()       -- declared below, body csystem_shims.cpp:104.
//      InitLoadTask() -- declared below, body csystem_shims.cpp:117.
//      OWNER: the wave that lands asendic_Loader.cpp (W7-L1 Wave 1, the
//      Loader + Loader_RT bundle).  Retire the BODIES only and KEEP both
//      declarations here -- csystem.cpp calls them through this header.
//      CONSEQUENCE that agent must plan for: these two are the hub's only
//      loader-feed entry points and are no-ops today, so retiring them turns the
//      per-tick main loop into a REAL Loader SM invocation and forces a
//      re-baseline of tests/test_w6_6_csystem_cycle.cpp and
//      tests/test_w6_6_hub.cpp (both need sane Loader global state or DoLoad will
//      alarm-loop inside them).
//    * Initial_Auto_BinTray_Task(int) -- declared below, body
//      csystem_shims.cpp:120.  OWNER: the wave that lands asendic_Auto.cpp.
//    * InitAutoColorTask()            -- declared below, body
//      csystem_shims.cpp:121.  OWNER: the wave that lands asendic_Color.cpp.
//
//  Precedent for how a completed retirement is recorded: the DoAuto2 note further
//  down this file plus its matching comment in the root CMakeLists.txt.
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
// NOTE: DoAutoEmpty1() is OWNED by asendic_Empty.cpp (declared asendic_Empty.h:33);
// csystem.cpp calls it via that header.  NOT shimmed here (avoid ODR).
// AI(W906-W7-L1) 20260729: DoAuto2() is now OWNED by asendic_Auto2.cpp (declared
// asendic_Auto2.h) -- csystem.cpp calls it via that header (#include added there).
// The no-op stand-in that used to live here was REMOVED to keep a single ODR
// definition (same retirement idiom as DoAutoEmpty1 just above).
bool DoInArmTeachAlignmentProcess(unsigned long &lAction);    // golden AutoAlignment.h:225 -- offline: complete (true)
bool DoOutArmTeachAlignmentProcess(unsigned long &lAction);   // golden AutoAlignment.h:252 -- offline: complete (true)
void SetFixTrayMiddleDtata();                                 // golden aoutarm.cpp -- Fix-tray middle data (offline no-op)

// ---------------------------------------------------------------------------
//  End-of-lot bookkeeping FSMs the HUB ticks after DoAllProcess (golden
//  csystem.cpp:12813 / :14713).  Their REAL faithful bodies now live in
//  csystem.cpp (W7-C1 DoCleanOutFinishCheck / W7-C2 DoOneCycleFinishCheck) and
//  their prototypes MOVED to csystem.h; the no-op stubs here were removed to
//  keep a single ODR definition each.
// ---------------------------------------------------------------------------
// DoOneCycleFinishCheck() : decl MOVED to csystem.h (W7-C2); REAL body in csystem.cpp.
// DoCleanOutFinishCheck() : decl MOVED to csystem.h (W7-C1); REAL body in csystem.cpp.

#endif
