// =============================================================================
//  test_w6_3_catchtray_core.cpp  --  W6.3 VERIFY: the TRAY-ARM ENGINE core SMs
//
//  Translation wave: W6.3 (TRAY-ARM ENGINE -- acatchtray.cpp)
//  Author: AI(W6.3-CATCHTRAY) 20260626
//
//  PURPOSE
//  -------
//  The task-mandated *core engine* verify TU for the TrayArm side -- the mirror
//  of test_w6_2_outarm_core.cpp.  It proves the TrayArm ENGINE's task state
//  machines + the real WhichAutoNeedTray() decision helper link into ht9045_sm
//  and PUMP over the Sim HAL with NO hardware, asserting:
//
//    (a) the per-SM entry cursors initialise to 1 (golden Initial*Task());
//    (b) the SIMPLEST fully-pumpable sub-SM (DoSlapTray) pumps FROM its entry
//        cursor and the cursor stays inside the documented switch(Task) state set
//        {1,5,10,20,50,100,200,300,400,450,500} (golden acatchtray.cpp:5889);
//    (c) DoSlapTray(true) re-inits the cursor to 1 (golden :5883-5887);
//    (d) the central DoCatchTray dispatcher pumps from entry without crashing and
//        keeps CatchTrayTask inside its documented universe;
//    (e) the real WhichAutoNeedTray() ORACLE: on the Clean-Out / no-IC path it
//        returns 0 (golden acatchtray.cpp:463), and on the Tray-Feed / no-IC path
//        it also returns 0 (golden :417) -- the two deterministic 0-return guards.
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here == clean g++
//  compile/link + the core SMs pump over the Sim HAL without crash + outputs match
//  values hand-derived from the golden cursor logic.
// =============================================================================
#include "acatchtray.h"            // DoCatchTray / DoSlapTray / WhichAutoNeedTray / cursors
#include "Motor/mymotor.h"         // MOT[]
#include "mycylin.h"               // Cylinder[]
#include "cprod.h"                 // Prod / IniConfig
#include "cpublic.h"
#include "cmydef.h"                // TRAY_ARM_MODE / iCleanOut / iTrayFeed / MAX_AUTO_TRAY
#include "aHotPlateSubstrate.h"    // CatchTraySuck
#include "canary_support.h"        // LastSet
#include "FormsFacade.h"
#include <cstdio>

// engine-owned cursors not all on the acatchtray.h surface:
extern int CatchTrayTask;                 // golden acatchtray.cpp:5993 (also csystem.h)
extern int iSlapTrayTask;                 // golden acatchtray.cpp:61 (also acatchtray.h)
extern bool bAutoNeedTray[MAX_AUTO_TRAY]; // golden acatchtray.cpp:54

static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; }\
    } while (0)

// documented switch(Task) state set for DoSlapTray (golden acatchtray.cpp:5889)
static bool slapCursorSane(int t)
{
    switch (t) {
        case 1: case 5: case 10: case 20: case 50: case 100:
        case 200: case 300: case 400: case 450: case 500: return true;
        default: return false;
    }
}

int main()
{
    printf("==== W6.3 CatchTray (TrayArm) CORE engine verify ====\n");

    // -----------------------------------------------------------------------
    //  KNOWN Sim HAL baseline
    // -----------------------------------------------------------------------
    CatchTraySuck.SetAllToNullIC();           // no IC on the tray-arm kit
    LastSet.iRealDummy = 0;                    // DUMMY

    // =======================================================================
    //  [A] entry-cursor inits
    // =======================================================================
    printf("[A] entry-cursor inits\n");
    InitialCatchTrayTask();
    CHECK(CatchTrayTask == 1, "InitialCatchTrayTask -> CatchTrayTask==1 (golden 5996)");
    DoSlapTray(true);
    CHECK(iSlapTrayTask == 1, "DoSlapTray(true) re-inits iSlapTrayTask==1 (golden 5885)");
    {
        bool r = DoSlapTray(true);
        CHECK(r == false, "DoSlapTray(true) returns false on init (golden 5886)");
    }

    // =======================================================================
    //  [B] DoSlapTray pumps; cursor stays in the documented set
    // =======================================================================
    printf("[B] DoSlapTray pump (simplest sub-SM)\n");
    DoSlapTray(true);                          // entry: iSlapTrayTask=1
    {
        bool sane = true;
        for (int tick = 0; tick < 200; ++tick) {
            DoSlapTray(false);
            if (!slapCursorSane(iSlapTrayTask)) { sane = false; break; }
        }
        CHECK(sane, "DoSlapTray cursor stays in {1,5,10,20,50,100,200,300,400,450,500} (golden 5889)");
    }

    // =======================================================================
    //  [C] DoCatchTray central dispatcher pumps without crash / escape
    // =======================================================================
    printf("[C] DoCatchTray central dispatcher pump\n");
    InitialCatchTrayTask();                    // CatchTrayTask=1
    {
        bool crashed = false;
        for (int tick = 0; tick < 200; ++tick) {
            DoCatchTray();                     // must not crash over the Sim HAL
            if (CatchTrayTask < 0 || CatchTrayTask > 7000) { crashed = true; break; }
        }
        CHECK(!crashed, "DoCatchTray pumps 200 ticks; CatchTrayTask stays in a sane range");
    }

    // =======================================================================
    //  [D] real WhichAutoNeedTray() ORACLE -- the two deterministic 0-returns
    // =======================================================================
    printf("[D] WhichAutoNeedTray() oracle (real engine body)\n");
    for (int i = 0; i < MAX_AUTO_TRAY; ++i) bAutoNeedTray[i] = false;

    int savedCleanOut = iCleanOut, savedTrayFeed = iTrayFeed;
    // Clean-Out / no-IC path -> golden acatchtray.cpp:463 -> iWhichAutoNeedTray=0
    iTrayFeed = 0; iCleanOut = 1;
    CHECK(WhichAutoNeedTray() == 0, "WhichAutoNeedTray()==0 on Clean-Out/no-IC path (golden 463)");
    // Tray-Feed / no-IC path -> golden acatchtray.cpp:417 -> iWhichAutoNeedTray=0
    iTrayFeed = 1; iCleanOut = 0;
    CHECK(WhichAutoNeedTray() == 0, "WhichAutoNeedTray()==0 on Tray-Feed/no-IC path (golden 417)");
    iCleanOut = savedCleanOut; iTrayFeed = savedTrayFeed;

    printf("==== W6.3 CatchTray core verify: %d passed, %d failed ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
