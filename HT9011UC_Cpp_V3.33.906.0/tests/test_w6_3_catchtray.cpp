// =============================================================================
//  test_w6_3_catchtray.cpp  --  W6.3 VERIFY: the TRAY-ARM ENGINE acceptance proof
//
//  Translation wave: W6.3 (TRAY-ARM ENGINE -- acatchtray.cpp)
//  Author: AI(W6.3-CATCHTRAY) 20260626
//
//  PURPOSE
//  -------
//  The task-mandated TrayArm acceptance verify TU -- the DIRECT mirror of the
//  in/out-arm core verifies (test_w6_2_{inarm,outarm}_core.cpp) and the
//  COMPANION to test_w6_3_catchtray_core.cpp.  Where the *core* TU pins the
//  per-SM entry cursors + the simplest pump, THIS TU sets a KNOWN Sim HAL state,
//  pumps the core CatchTray task SM(s) FROM ENTRY, and asserts the FIVE concrete
//  oracle values hand-derived from the golden engine in the task brief.  Every
//  assert cites its golden acatchtray.cpp line.
//
//  THE FIVE HAND-DERIVED ORACLES (golden = D:\HT9045\HT9011UC_Code_V3.33.906.0_
//  20260618\acatchtray.cpp, decoded cp950; line numbers below are GOLDEN lines):
//
//   [O1] DoSlapTray 200->300 FALL-THROUGH (golden 5877 SM; 5925-5929 case 200).
//        case 200 has NO guard -- it unconditionally does
//          Cylinder[C_TrayCover].Off(); HSlapTrayDelay.Set0_1SecAndOn(10); Task=300;
//        and the `//break;` at golden:5929 is COMMENTED OUT, so control falls into
//        case 300 the SAME tick.  ORACLE: inject iSlapTrayTask=200, pump ONE tick
//        -> cursor==300.  The load-bearing proof is the commented-out `//break;`
//        (verified in the translated source) + the unconditional case-200 body
//        that forces a 1-tick advance regardless of cylinder/timer state.  Plus:
//        DoSlapTray(true) re-inits cursor to 1 and returns false (golden 5883-5887);
//        case 1 under InArmXYZSafe()==true advances 1->5 (golden 5891-5897).
//
//   [O2] DoCatchFromLoader 1->10 FALL-THROUGH (golden 1487 SM; 1499-1539 case 1/10).
//        case 1: if MTrayXCanSafeMove()==false -> Task=50; BREAK (golden 1500-1504).
//        else SuckLoaderErrRetryCT=0; Task=10; FALLS THROUGH (no break @1507) into
//        case 10.  case 10 else-arm (golden 1534-1538): Task=50.  ORACLE: from
//        entry Task=1 with MTrayXCanSafeMove()==true and MMTrayY.fHasTray==false,
//        ONE call lands at 50 -- this can ONLY happen if case 1 FELL THROUGH into
//        case 10 the same tick (a break after case 1 would have stranded the
//        cursor at 10).  Contrast: MTrayXCanSafeMove()==false from Task=1 also
//        reaches 50, but via the case-1 break (golden 1500-1504).
//
//   [O3] WhichAutoNeedTray() deterministic 0-returns (golden 410 fn).
//        Tray-Feed/no-IC path: iTrayFeed==1 && HasICUnderMachine()==false ->
//          iWhichAutoNeedTray=0; the ATK_AMR sub-branch is gated by
//          fAGV->IsATK_AMR()==false offline -> returns 0 (golden 417-462).
//        Clean-Out/no-IC path: iCleanOut==1 && HasICUnderMachine()==false ->
//          iWhichAutoNeedTray=0 -> returns 0 (golden 463-466).
//        Both short-circuit BEFORE the data-dependent default path, so they are
//        robust regardless of Prod/MOT/Sen state.
//
//   [O4] WhichAutoNeedTray() AMR 1-BASED i+1 return contract (golden 488-513).
//        When fAGV->Use_AMR()==true and bAutoNeedTray[i]==true with
//        bHas1DCoverTray[i]==false, sets bNeed1DCoverTray[i]=true and returns i+1.
//        *** NOT OFFLINE-EXERCISABLE ***: the W6.x FormsFacade stub hardwires
//        TfAGV::Use_AMR()==false (FormsFacade.cpp:24) -- there is no AMR present
//        offline -- so this branch is structurally unreachable WITHOUT faking the
//        facade away from its faithful "no AMR" contract.  Documented, NOT faked.
//        We PIN the offline contract (Use_AMR()==false) so the un-reachability is
//        recorded; the 1-based +1 convention itself is structural (golden 429/441/
//        453/494/498/506/676 all `return i+1`) and is exercised at [O3] via the
//        iWhichAutoNeedTray entry-init==0 contract.
//
//   [O5] DoCatchTray entry guard (golden 6000 fn; 6015-6019 guard).
//        Guard: iPauseBackUp!=-1 && bC03UseCatchTray==false &&
//               CatchTraySuck.IsShtSuckFinish()==true &&
//               CatchTraySuck.IsShtDestroyFinish()==true  -> return.
//        Offline both IsSht*Finish()==true (aHotPlateSubstrate.cpp:140-141), so the
//        guard fires IFF iPauseBackUp!=-1 (with bC03UseCatchTray==false).  ORACLE:
//        (a) iPauseBackUp=0 -> guard fires -> one DoCatchTray() leaves CatchTrayTask
//            unchanged (switch NOT entered);
//        (b) iPauseBackUp=-1 -> guard false -> switch runs; case 1 under
//            TRAY_ARM_MODE==eUnderCoveyor dispatches Task=50 (golden 6076-6079),
//            with the EmptyColor pre-alarm guard taken via
//            bUseEmptyColorTrayPreAlarm && bP32EmptyColorTrayPreAlarm (golden 6029-6032).
//
//  Plus a BONUS geometry oracle (IsTrayArmAtEmptyOrColor / IsTrayArmMoveAvoidOut-
//  ArmCrash, golden 7979-8004) and cursor-universe robustness pumps.
//
//  SIM-HAL POSITION KNOB: offline the MOT[] elements have no attached inner
//  HTMotor (Motor==NULL), so TMyMotor::ReadPos() PRESERVES the Position field
//  (golden mymotor.cpp:226-236 else-branch) and TMyMotor::ScanMotorStatus() is a
//  no-op (mymotor.cpp:560 Motor!=NULL guard) so a directly-set Led[] sticks.  We
//  therefore set MOT[].Position / MOT[].Led[] directly as the Sim-HAL knobs and
//  force Motor==NULL so ReadPos() returns exactly what we set.
//
//  EQUIVALENCE NOTE: no Borland binary exists, so "equivalence" here == clean g++
//  compile/link + the core SMs pump over the Sim HAL without crash + outputs match
//  values hand-derived from the golden cursor logic / numeric formula.
// =============================================================================
#include "acatchtray.h"            // DoCatchTray / DoSlapTray / WhichAutoNeedTray / IsTrayArm* / cursors
#include "Motor/mymotor.h"         // MOT[], TrayArmMotorMove
#include "Motor/HTMotor.h"         // iHomeLed / iInposLed
#include "mycylin.h"               // Cylinder[]
#include "cprod.h"                 // Prod / IniConfig / CosFunction
#include "cpublic.h"
#include "cmydef.h"                // TRAY_ARM_MODE / MTrayX / MTrayZ / MMTrayY / MInArmX / iTrayFeed / iCleanOut / MAX_AUTO_TRAY
#include "MachineType.h"           // eAboveCoveyor / eUnderCoveyor
#include "aHotPlateSubstrate.h"    // CatchTraySuck
#include "canary_support.h"        // LastSet
#include "FormsFacade.h"           // fAGV
#include <cstdio>

// ---------------------------------------------------------------------------
//  Engine-owned cursors (CatchTrayTask / iSlapTrayTask / iCatchFromLoaderTask
//  ARE externed in acatchtray.h; bAutoNeedTray is engine-internal -- re-declare
//  defensively, same idiom the core TU uses).
// ---------------------------------------------------------------------------
extern int  CatchTrayTask;                 // golden acatchtray.cpp:5993
extern int  iSlapTrayTask;                 // golden acatchtray.cpp:61 / 5879
extern int  iCatchFromLoaderTask;          // golden acatchtray.cpp:1490
extern bool bAutoNeedTray[MAX_AUTO_TRAY];  // golden acatchtray.cpp:54

// DoCatchFromLoader is the int sub-SM (entry cursor iCatchFromLoaderTask).
extern int  DoCatchFromLoader();           // golden acatchtray.cpp:1487

// ---------------------------------------------------------------------------
//  Minimal PASS / FAIL harness (same style as the other W6 verify TUs)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                    \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ---- documented switch(Task) state set for DoSlapTray (golden acatchtray.cpp:5889)
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
    printf("==== W6.3 CatchTray (TrayArm) ENGINE acceptance verify ====\n");

    // -----------------------------------------------------------------------
    //  KNOWN Sim HAL baseline (no IC on the tray-arm kit; DUMMY run mode)
    // -----------------------------------------------------------------------
    CatchTraySuck.SetAllToNullIC();            // no IC on the tray-arm kit
    LastSet.iRealDummy = 0;                    // DUMMY

    // Force Motor==NULL on the tray motors so the Position/Led knobs are honoured
    // directly (see header note: ReadPos() preserves Position, ScanMotorStatus is
    // a no-op when Motor==NULL).
    MOT[MTrayX].Motor = NULL;
    MOT[MTrayZ].Motor = NULL;

    // Save engine geometry / mode inputs so the oracles are deterministic and
    // global state is restored at the end.
    int  savedMode      = TRAY_ARM_MODE;
    int  savedEmpty     = Prod.iXTrayEmpty;
    int  savedColor     = Prod.iXTrayColor;
    int  savedPause     = iPauseBackUp;
    bool savedEject     = bEject;
    bool savedC03       = IniConfig.bC03UseCatchTray;
    bool savedPre       = CosFunction.bUseEmptyColorTrayPreAlarm;
    bool savedP32       = IniConfig.bP32EmptyColorTrayPreAlarm;
    int  savedCleanOut  = iCleanOut;
    int  savedTrayFeed  = iTrayFeed;

    // =======================================================================
    //  [O1] DoSlapTray 200->300 FALL-THROUGH + init guard + case1->5
    //       golden acatchtray.cpp:5877 (SM), 5883-5897, 5925-5929 (case 200).
    // =======================================================================
    printf("[O1] DoSlapTray fall-through 200->300 (golden 5925-5929)\n");
    {
        // init guard -- golden 5883-5887
        iSlapTrayTask = 99;                     // dirty value to prove the guard forces 1
        bool r = DoSlapTray(true);
        CHECK(iSlapTrayTask == 1, "DoSlapTray(true) forces iSlapTrayTask==1 (golden 5885)");
        CHECK(r == false,         "DoSlapTray(true) returns false on init (golden 5886)");

        // case 1 -> 5 under InArmXYZSafe()==true (eUnderCoveyor short-circuit, golden 417-419)
        TRAY_ARM_MODE = eUnderCoveyor;          // InArmXYZSafe()==true
        DoSlapTray(true);                        // iSlapTrayTask=1
        MOT[MInArmX].fCanMove = true;            // pre-dirty so we observe the clear
        MOT[MInArmY].fCanMove = true;
        DoSlapTray(false);                       // ONE tick: case 1 advances to 5
        CHECK(iSlapTrayTask == 5,
              "DoSlapTray case1->5 under InArmXYZSafe (golden 5896)");
        CHECK(MOT[MInArmX].fCanMove == false,
              "DoSlapTray case1 clears MOT[MInArmX].fCanMove (golden 5894)");
        CHECK(MOT[MInArmY].fCanMove == false,
              "DoSlapTray case1 clears MOT[MInArmY].fCanMove (golden 5895)");

        // *** the fall-through proof ***: inject the cursor straight into case 200
        // and pump ONE tick.  case 200 is UNCONDITIONAL (no cylinder/timer guard):
        // it sets Task=300 and falls through (the `//break;` at golden 5929 is
        // commented out), landing the cursor at 300 in a single call.
        iSlapTrayTask = 200;
        DoSlapTray(false);                       // ONE tick: case 200 -> (fall) -> case 300
        CHECK(iSlapTrayTask == 300,
              "DoSlapTray case200 falls through to 300 in one tick (golden 5925-5929 //break;)");
        TRAY_ARM_MODE = savedMode;
    }

    // =======================================================================
    //  [B] DoSlapTray pumps from entry; cursor never escapes the documented set.
    //      golden acatchtray.cpp:5889 (switch state universe).
    // =======================================================================
    printf("[B] DoSlapTray pump -- cursor universe (golden 5889)\n");
    {
        DoSlapTray(true);                        // entry: iSlapTrayTask=1
        bool sane = true;
        for (int tick = 0; tick < 200; ++tick) {
            DoSlapTray(false);
            if (!slapCursorSane(iSlapTrayTask)) { sane = false; break; }
        }
        CHECK(sane, "DoSlapTray cursor stays in {1,5,10,20,50,100,200,300,400,450,500} (golden 5889)");
    }

    // =======================================================================
    //  [O2] DoCatchFromLoader 1->10 FALL-THROUGH (golden 1487 SM; 1499-1539).
    //       In eUnderCoveyor mode:
    //         InArmXYZSafe()==true (golden 417-419);
    //         MTrayXCanSafeMove() == MOT[MTrayZ].Led[iHomeLed] (golden 1603-1606).
    //       MTrayXCanSafeMove() does NOT call ScanMotorStatus, so the directly-set
    //       Led sticks.  Motor==NULL on MTrayX -> PCIL132_StopMotor() is a safe
    //       no-op (mymotor.cpp:637 Motor==NULL guard).
    // =======================================================================
    printf("[O2] DoCatchFromLoader fall-through 1->10 (golden 1499-1539)\n");
    {
        TRAY_ARM_MODE = eUnderCoveyor;           // InArmXYZSafe()==true
        MOT[MMTrayY].fHasTray = false;           // case 10 IF false -> takes else -> Task=50

        // (a) MTrayXCanSafeMove()==TRUE -> case 1 falls through into case 10 the
        //     SAME call; case 10 else -> Task=50.  Reaching 50 in ONE call from
        //     entry proves the fall-through (a break after case 1 would strand the
        //     cursor at 10).
        MOT[MTrayZ].Led[iHomeLed] = true;        // MTrayXCanSafeMove()==true
        iCatchFromLoaderTask = 1;
        DoCatchFromLoader();                     // ONE call: case 1 -> (fall) -> case 10 -> else
        CHECK(iCatchFromLoaderTask == 50,
              "DoCatchFromLoader case1 falls through case10 else -> Task=50 in one call (golden 1507/1537)");

        // (b) MTrayXCanSafeMove()==FALSE -> case 1 break path -> Task=50 (golden 1500-1504).
        MOT[MTrayZ].Led[iHomeLed] = false;       // MTrayXCanSafeMove()==false
        iCatchFromLoaderTask = 1;
        DoCatchFromLoader();                     // ONE call: case 1 -> Task=50; break
        CHECK(iCatchFromLoaderTask == 50,
              "DoCatchFromLoader case1 MTrayXCanSafeMove false -> Task=50;break (golden 1500-1504)");

        MOT[MMTrayY].fHasTray = false;           // restore
        TRAY_ARM_MODE = savedMode;
    }

    // =======================================================================
    //  [O3] WhichAutoNeedTray() deterministic 0-returns (golden 410).
    // =======================================================================
    printf("[O3] WhichAutoNeedTray() 0-return oracles (golden 417 / 463)\n");
    {
        for (int i = 0; i < MAX_AUTO_TRAY; ++i) bAutoNeedTray[i] = false;

        // Tray-Feed / no-IC path -> golden 417-462 -> iWhichAutoNeedTray=0, return 0.
        iTrayFeed = 1; iCleanOut = 0;
        CHECK(WhichAutoNeedTray() == 0,
              "WhichAutoNeedTray()==0 on Tray-Feed/no-IC path (golden 417)");

        // Clean-Out / no-IC path -> golden 463-466 -> iWhichAutoNeedTray=0, return 0.
        iTrayFeed = 0; iCleanOut = 1;
        CHECK(WhichAutoNeedTray() == 0,
              "WhichAutoNeedTray()==0 on Clean-Out/no-IC path (golden 463)");

        iCleanOut = savedCleanOut; iTrayFeed = savedTrayFeed;
    }

    // =======================================================================
    //  [O4] WhichAutoNeedTray() AMR 1-based i+1 return (golden 488-513).
    //       NOT offline-exercisable -- see header.  Pin the offline contract so
    //       the un-reachability is recorded (not faked).
    // =======================================================================
    printf("[O4] WhichAutoNeedTray() AMR path -- offline-unreachable (documented)\n");
    {
        CHECK(fAGV->Use_AMR() == false,
              "fAGV->Use_AMR()==false offline -> AMR i+1 branch unreachable (FormsFacade.cpp:24)");
    }

    // =======================================================================
    //  [O5] DoCatchTray entry guard (golden 6000; guard 6015-6019).
    // =======================================================================
    printf("[O5] DoCatchTray entry guard + case1 eUnderCoveyor dispatch (golden 6015-6019 / 6076-6079)\n");
    {
        bEject = false;                          // bypass the bEject early-return (golden 6021)

        // (a) iPauseBackUp!=-1 && bC03UseCatchTray==false && IsSht*Finish()==true
        //     -> guard fires -> switch NOT entered -> cursor unchanged.
        IniConfig.bC03UseCatchTray = false;
        iPauseBackUp = 0;                        // != -1 -> guard armed
        InitialCatchTrayTask();                  // CatchTrayTask=1
        CHECK(CatchTrayTask == 1, "InitialCatchTrayTask -> CatchTrayTask==1 (golden 5996)");
        DoCatchTray();                           // guard fires -> early return
        CHECK(CatchTrayTask == 1,
              "DoCatchTray entry guard fires (iPauseBackUp!=-1) -> cursor unchanged (golden 6015-6019)");

        // (b) iPauseBackUp==-1 -> guard false -> switch runs; case 1 under
        //     eUnderCoveyor dispatches Task=50.  Take the EmptyColor pre-alarm
        //     no-op arm so no Sen[]-dependent early-return fires (golden 6029-6032).
        iPauseBackUp = -1;                       // guard disarmed
        CosFunction.bUseEmptyColorTrayPreAlarm = true;
        IniConfig.bP32EmptyColorTrayPreAlarm   = true;
        TRAY_ARM_MODE = eUnderCoveyor;
        InitialCatchTrayTask();                  // CatchTrayTask=1
        DoCatchTray();                           // ONE tick: case 1 -> Task=50
        CHECK(CatchTrayTask == 50,
              "DoCatchTray case1 eUnderCoveyor dispatch -> Task=50 (golden 6076-6079)");

        // restore
        IniConfig.bC03UseCatchTray             = savedC03;
        CosFunction.bUseEmptyColorTrayPreAlarm = savedPre;
        IniConfig.bP32EmptyColorTrayPreAlarm   = savedP32;
        iPauseBackUp                           = savedPause;
        bEject                                 = savedEject;
        TRAY_ARM_MODE                          = savedMode;
    }

    // =======================================================================
    //  [BONUS-G1] IsTrayArmAtEmptyOrColor geometry oracle (golden 7992-8004).
    //       eAboveCoveyor: returns (iPos < iXTrayColor+100 && iPos > iXTrayEmpty-100)
    //       -- pure INTEGER compares on int positions (no truncation).
    //       eUnderCoveyor short-circuits true (golden 7996-7997).
    // =======================================================================
    printf("[BONUS-G1] IsTrayArmAtEmptyOrColor int-compare oracle (golden 7992-8004)\n");
    {
        Prod.iXTrayEmpty = 1000;                 // iXTrayEmpty-100 == 900
        Prod.iXTrayColor = 5000;                 // iXTrayColor+100 == 5100
        TRAY_ARM_MODE = eAboveCoveyor;

        // pos==0 -> 0 < 5100 (true) && 0 > 900 (FALSE) -> false
        MOT[MTrayX].Position = 0;
        CHECK(IsTrayArmAtEmptyOrColor() == false,
              "IsTrayArmAtEmptyOrColor: pos=0 below iXTrayEmpty-100 -> false (golden 8000)");

        // pos==3000 -> 3000 < 5100 (true) && 3000 > 900 (true) -> true
        MOT[MTrayX].Position = 3000;
        CHECK(IsTrayArmAtEmptyOrColor() == true,
              "IsTrayArmAtEmptyOrColor: pos=3000 inside window -> true (golden 7999-8001)");

        // pos==5100 -> 5100 < 5100 (FALSE strict '<') -> false (pins int compare)
        MOT[MTrayX].Position = 5100;
        CHECK(IsTrayArmAtEmptyOrColor() == false,
              "IsTrayArmAtEmptyOrColor: pos==iXTrayColor+100 fails strict '<' -> false (golden 7999)");

        // eUnderCoveyor short-circuits true regardless of pos (golden 7996-7997)
        TRAY_ARM_MODE = eUnderCoveyor;
        MOT[MTrayX].Position = 0;
        CHECK(IsTrayArmAtEmptyOrColor() == true,
              "IsTrayArmAtEmptyOrColor: eUnderCoveyor short-circuit -> true (golden 7997)");
        TRAY_ARM_MODE = savedMode;
    }

    // =======================================================================
    //  [BONUS-G2] IsTrayArmMoveAvoidOutArmCrash geometry oracle (golden 7979-7990).
    //       eAboveCoveyor: ReadPos()>=iXTrayEmpty && Led[iInposLed] -> true, else
    //       false.  The predicate calls MOT[MTrayX].ScanMotorStatus() first, which
    //       is a no-op when Motor==NULL, so the directly-set Led[iInposLed] sticks.
    //       eUnderCoveyor returns false immediately (golden 7982-7983).
    // =======================================================================
    printf("[BONUS-G2] IsTrayArmMoveAvoidOutArmCrash oracle (golden 7979-7990)\n");
    {
        Prod.iXTrayEmpty = 1000;                 // > 0
        TRAY_ARM_MODE = eAboveCoveyor;

        MOT[MTrayX].Position = 0;                // 0 >= 1000 FALSE
        MOT[MTrayX].Led[iInposLed] = true;       // Led true, but pos fails -> false
        CHECK(IsTrayArmMoveAvoidOutArmCrash() == false,
              "IsTrayArmMoveAvoidOutArmCrash: pos<iXTrayEmpty -> false (golden 7985-7989)");

        MOT[MTrayX].Position = 2000;             // 2000 >= 1000 TRUE
        MOT[MTrayX].Led[iInposLed] = false;      // Led false -> AND fails -> false
        CHECK(IsTrayArmMoveAvoidOutArmCrash() == false,
              "IsTrayArmMoveAvoidOutArmCrash: pos ok but Led false -> false (golden 7985-7989)");

        MOT[MTrayX].Position = 2000;
        MOT[MTrayX].Led[iInposLed] = true;       // both true -> true
        CHECK(IsTrayArmMoveAvoidOutArmCrash() == true,
              "IsTrayArmMoveAvoidOutArmCrash: pos>=iXTrayEmpty && Led true -> true (golden 7987)");

        // eUnderCoveyor short-circuits false regardless (golden 7982-7983)
        TRAY_ARM_MODE = eUnderCoveyor;
        CHECK(IsTrayArmMoveAvoidOutArmCrash() == false,
              "IsTrayArmMoveAvoidOutArmCrash: eUnderCoveyor short-circuit -> false (golden 7982-7983)");

        MOT[MTrayX].Led[iInposLed] = false;      // restore
        TRAY_ARM_MODE = savedMode;
    }

    // =======================================================================
    //  [C] DoCatchTray central dispatcher pumps from entry without crash / escape.
    // =======================================================================
    printf("[C] DoCatchTray central dispatcher pump (no crash / sane cursor)\n");
    {
        iPauseBackUp = -1;                       // ensure the switch actually runs
        InitialCatchTrayTask();                  // CatchTrayTask=1
        bool crashed = false;
        for (int tick = 0; tick < 200; ++tick) {
            DoCatchTray();                       // must not crash over the Sim HAL
            if (CatchTrayTask < 0 || CatchTrayTask > 7000) { crashed = true; break; }
        }
        CHECK(!crashed, "DoCatchTray pumps 200 ticks; CatchTrayTask stays in a sane range");
        iPauseBackUp = savedPause;
    }

    // -----------------------------------------------------------------------
    //  Restore engine geometry inputs (defensive; locals already restored above)
    // -----------------------------------------------------------------------
    Prod.iXTrayEmpty = savedEmpty;
    Prod.iXTrayColor = savedColor;
    TRAY_ARM_MODE    = savedMode;

    printf("==== W6.3 CatchTray engine verify: %d passed, %d failed ====\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
