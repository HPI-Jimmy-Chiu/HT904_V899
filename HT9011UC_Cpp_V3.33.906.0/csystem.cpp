// =============================================================================
//  csystem.cpp  --  HT9045 system-coordination HUB (the orchestration spine)
//
//  Mirrored from : HT9011UC_Code_V3.33.906.0_20260618/csystem.cpp  (READ-ONLY golden)
//  Translation wave : W6.6 (csystem orchestration SPINE)
//  Translator : AI(W6.6-HUB) 20260626
//
//  WHAT THIS FILE IS
//  -----------------
//  csystem.cpp is NOT a class -- it is a flat free-function translation unit, the
//  system-coordination HUB.  This wave delivers the runnable INTEGRATION SPINE
//  (the ~1000-line load-bearing core of the 25483-line golden), NOT all 25K lines:
//
//    * MainProc()            golden :16711 -- OUTER per-frame entry.  ACTIVE:
//                            the alive-instrumentation head (golden :16724-16735)
//                            + a SLIM dispatch that ticks the integration spine
//                            (DoAllProcess + DoOneCycleFinishCheck +
//                            DoCleanOutFinishCheck).  The 2000+ line mode / SECS /
//                            AGV / temp dispatch ladder (DoSystem / DoTriTempState /
//                            DoHomeProcess / DoTrayFeedProcess) is GATED #if 0
//                            // TODO(W7) -- those units are not translated.
//    * DoAllProcess()        golden :9115 -- THE SPINE.  The per-tick engine
//                            driver: the static bDoProcess 2-tick alternation +
//                            the master SoftStop/SystemStart/fAllMotorHome guards
//                            + the FIVE engine calls in the documented order +
//                            DoLoad / DoCatchTray / DoAutoReceiveBinTray loop +
//                            the tail cylinder/alignment loops.  Translated
//                            VERBATIM (tick order / guards / fall-throughs).  The
//                            AutoRetest / OCR / Laser / Decay / AutoClean early-
//                            return ladder (golden :9132-10047) is GATED #if 0
//                            // TODO(W7) -- those reach untranslated UI / OCR /
//                            DB / SECS surface -- BUT the AutoClean engine CALLS
//                            (DoAutoCleanKit / DoShuttle1/2AutoClean /
//                            DoIndexAutoClean) would stay active when ungated
//                            (those engines exist); see the gate note in-body.
//    * GetMainProcCallCount / GetMainProcLastEnterTime[String] /
//      GetMainProcSilentSeconds / IsMainProcAlive  golden :16677-16709 --
//                            VERBATIM (the alive-instrumentation the frozen
//                            csystem.h declares; the oracle reads them).
//    * InitAllProcessTask()  golden :5698 -- the deterministic per-engine
//                            cursor-reset aggregator the test harness calls
//                            before pumping.  ACTIVE (calls each engine's own
//                            init; the no-home leaves route to csystem_shims).
//    * CheckMotorHome()      golden :15833 -- small predicate, ACTIVE.
//    * SetInitialICCheck()   golden :1049 -- small state setter, ACTIVE.
//    * InitCleanOutFunction / InitOneCycle / InitTrayEndFunction  golden :15749
//                            / :15797 / :15824 -- small cursor/flag resets,
//                            ACTIVE (the AutoSiteMap branch of InitCleanOut is
//                            GATED -- it derefs HotPlateForm / fMain mode setters).
//
//  ODR -- SKIPPED here (already defined elsewhere; do NOT redefine):
//    * The HasIC predicate family + InSHT*/OutSHT*/CheckSafeDoorIsClosed /
//      DoAutoSiteMappingDropError / HasICUnderMachine / HasAutoICInMachine /
//      HasAnyICInMachine / HasICUnderHotPlate  -> csystem_predicates.cpp.
//    * DoTestHeadMotor / DoTestY (atester.cpp); DoOutArm_9045 /
//      DoPickFromShuttle_9045 (aoutarm9045.cpp); DoInArm_9045 (ainarm9045.cpp);
//      Do_Auto_SHT1/2/3 + Initial_Auto_SHT1/2_Task (acarry.cpp); DoCatchTray /
//      InitialCatchTrayTask (acatchtray.cpp); InitInArmTask /
//      InitialTestHeadMotorTask / InitAutoEmptyTask / InitFrontTestSuckICTask /
//      InitBTestSuckTestICTask -> the engines / engine shims.  csystem.cpp CALLS
//      these; it never redefines them.
//    * DoInArm / DoOutArm / DoLoad / DoSortArm / DoAuto3Magazine /
//      NewDoAutoTrayEdgeCylinderLoop / DoAuto2 / DoAutoEmpty1 /
//      DoInArmTeachAlignmentProcess / DoOutArmTeachAlignmentProcess /
//      DoOneCycleFinishCheck / DoCleanOutFinishCheck / InitLoadTask /
//      InitOutArmTask / InitialDoLockUnloader / Initial_Auto_BinTray_Task /
//      InitAutoColorTask / Initial*Magazine* / bShuttleShake / SetFixTrayMiddleDtata
//      -> csystem_shims.cpp (ACTIVE offline stand-ins for untranslated units).
//
//  Big5 note: the golden is cp950.  Chinese comments decode cleanly and are
//  preserved as UTF-8.  ZERO U+FFFD is emitted.
//
//  VCL / Borland conversion: spine free functions are plain (no __fastcall).
//  FormatDateTime / Now() / TDateTime double-cast via vclcompat.  The
//  #ifdef DEBUG_TRY_CATCH wrappers compile OUT (macro not defined), leaving the
//  bare spine calls -- kept verbatim.  The offline "pump" is the TEST harness
//  calling MainProc()/DoAllProcess() in a for-loop (no Win32 thread translated).
// =============================================================================
#include "vclcompat/vcl_compat.h"   // AnsiString / TDateTime / Now / FormatDateTime
#include "csystem.h"                 // the frozen HUB interface this TU realizes
#include "csystem_shims.h"           // DoInArm/DoOutArm/DoLoad/DoSortArm/bShuttleShake/... offline shims

#include "cmydef.h"                  // SoftStop/SystemStart/fAllMotorHome/iCleanOut/iHome/iReset/iTrayFeed/... globals
#include "cprod.h"                   // Prod (parity)
#include "cpublic.h"
#include "MachineType.h"             // eAuto1 / eartUninstall / TRAY_VIBRATION enums
#include "Config.h"                  // IniConfig (bF06InitialICCheck / ...)
#include "CosFunction.h"             // CosFunction (parity)
#include "Motor/mymotor.h"           // MOT[] (CheckMotorHome: HomeFlag / TOTAL_MOTOR)
#include "myswitch.h"                // SW[]  (SwSocketClean.Off in the gated ladder)
#include "mycylin.h"                 // Cylinder[] (tail vibration loop enables)
#include "aHotPlateSubstrate.h"      // InArmSuck / carry kits (parity with engines)

// NOTE: csystem.cpp does NOT include ainarm9045.h / aoutarm9045.h -- DoInArm_9045 /
// DoOutArm_9045 are called only by csystem_shims.cpp (the DoInArm/DoOutArm
// dispatchers).  ainarm9045.h + aHotPlateSubstrate.h BOTH declare
// InArmLeftSideNoIC/HasIC with a default arg, which g++ rejects when both land
// in one TU; the engine TUs include ainarm9045.h FIRST.  csystem.cpp needs
// neither, so we simply omit them (mirrors acarry.cpp's same avoidance).
#include "acarry.h"                  // Do_Auto_SHT1/2/3 + Initial_Auto_SHT1/2_Task
#include "atester.h"                 // DoTestHeadMotor + InitialTestHeadMotorTask
#include "acatchtray.h"              // DoCatchTray + InitialCatchTrayTask
#include "asendic_Empty.h"           // DoAutoEmpty / DoAutoColor / InitAutoEmptyTask / DoAutoEmpty1(shim)
#include "atester_shims.h"           // InitFrontTestSuckICTask / InitBTestSuckTestICTask
#include "acatchtray_shims.h"        // DoAutoReceiveBinTray / DoAutoColor
#include "canary_support.h"          // LastSet / RecordProcess / REALLY (SetInitialICCheck / InitOneCycle)
#include "FormsFacade.h"             // fMain (DoAllProcess :9130 ProcessSensorScan; gated branches)

#include <cstdio>

// DoAllProcess() is the file-internal per-tick engine driver; the golden keeps
// it extern-visible but only MainProc() calls it.  Forward-declare it so the
// (earlier) MainProc definition can call it; its body is defined below.
void DoAllProcess();

// ---------------------------------------------------------------------------
//  iAllArmZHomeTask -- golden cmydef (set by InitAllProcessTask :5718).  No
//  translated home yet; the HUB owns it (the Arm-Z-home FSM that consumes it is
//  DoArmZHome, gated/untranslated this wave).
// ---------------------------------------------------------------------------
int iAllArmZHomeTask = 1;

// =============================================================================
//  TICK-SEQUENCE ORACLE hook (test-only; compiles OUT of production).
//  When CSYSTEM_TICK_ORACLE is defined (the W6.6 verify TU), DoAllProcess
//  records the engine it is about to call into g_csystemTrace, so the harness
//  can confirm the documented per-tick engine ORDER.  In production the macro
//  expands to nothing, leaving the faithful golden body untouched.
// =============================================================================
#ifdef CSYSTEM_TICK_ORACLE
  #include <vector>
  std::vector<int> g_csystemTrace;
  void csystemTraceClear() { g_csystemTrace.clear(); }
  #define CSYS_TICK(id) do { g_csystemTrace.push_back(id); } while(0)
#else
  #define CSYS_TICK(id) do { } while(0)
#endif
// engine-id constants the oracle keys on (documented tick order)
enum {
    CT_SENSORSCAN = 1, CT_DOLOAD = 2, CT_DOINARM = 3, CT_SHT1 = 4, CT_SHT2 = 5,
    CT_SHT3 = 6, CT_TESTHEAD = 7, CT_CATCHTRAY = 8, CT_OUTARM = 9, CT_SORTARM = 10
};

// ===========================================================================
//  SetInitialICCheck  -- golden csystem.cpp:1049.  Small state setter (ACTIVE).
// ===========================================================================
void SetInitialICCheck()
{
    if(IniConfig.bF06InitialICCheck && LastSet.iRealDummy!=DUMMY &&
       ShuttleHasIC()==false && IndexHasIC()==false &&
       IniConfig.bF20InShuttleProminentDetect==false)
    {
        bInitialICCheck=true;
        iInitialICCheckTask=1;
    }
    else
    {
        bInitialICCheck=false;
    }

    if(IniConfig.bF09CheckICWhichFirstTimeLoad &&
       LastSet.iRealDummy==REALLY)                                              //Jimmychiu 20230808 : F09 function !=DUMMY --> ==REALLY
    {
        bCheckShuttle1MustHasIC=true;
        bCheckShuttle2MustHasIC=true;
    }
    else
    {
        bCheckShuttle1MustHasIC=false;
        bCheckShuttle2MustHasIC=false;
    }
}

// ===========================================================================
//  InitAllProcessTask  -- golden csystem.cpp:5698.  The deterministic per-engine
//  cursor-reset aggregator (ACTIVE).  Each leaf is owned by the engine that
//  declares it; the no-translated-home leaves route to csystem_shims.cpp.
// ===========================================================================
void InitAllProcessTask()
{
    InitLoadTask();                                                            // shim (golden asendic_Loader.cpp)
    InitInArmTask();                                                           // aHotPlateSubstrate.cpp
//#ifdef Carry4
//    Initial_Auto_InSHT_Task();
//    Initial_Auto_OutSHT_Task();
//#else
    Initial_Auto_SHT1_Task();                                                  // acarry.cpp
    Initial_Auto_SHT2_Task();                                                  // acarry.cpp
//#endif
    InitialTestHeadMotorTask();                                                // atester.cpp
    InitOutArmTask();                                                          // shim (file-local static in aoutarm9045.cpp)
    InitialCatchTrayTask();                                                    // acatchtray.cpp
    for(int i=eAuto1; i<=iAutoRight; i++)
    {
        int iAuto=iAutoIndex[i];
        InitialDoLockUnloader(iAuto);                                          // shim //JerryYang 20191210 只對Auto定位方式,先放->夾住->放料+夾住
        Initial_Auto_BinTray_Task(iAuto);                                     // shim
    }
    iAllArmZHomeTask=1;
    InitAutoEmptyTask();                                                       // asendic_Empty.cpp

    InitAutoColorTask();                                                       // shim //ChungHung 20121011 add 解決機器?? Initial Start ?都會跳出Tray Setting Error
    //JerryYang 20220909 : add magazine
    //==>
    if(AUTO3_IS_MAGAZINE==1)
    {
        iMagazineStatus=0;
        InitialAuto3MagazineTask();                                            // shim
        InitialCatchTrayChangeTray();                                          // shim
        InitialCatchTrayGetNewTray();                                          // shim
        InitialDoMagazineTrayFeedTask();                                       // shim
    }
    //<==
    //JerryYang 20220909 : add magazine
    InitFrontTestSuckICTask();                                                 //JerryYang 20250110 : add  (atester_shims.cpp)
    InitBTestSuckTestICTask();                                                 // atester_shims.cpp
}

// ===========================================================================
//  CheckMotorHome  -- golden csystem.cpp:15833.  Small predicate (ACTIVE).
// ===========================================================================
bool CheckMotorHome()
{
    for(int i=0; i<TOTAL_MOTOR; i++)
        if(MOT[i].HomeFlag!=1)
            return false;
    return true;
}

// ===========================================================================
//  InitCleanOutFunction  -- golden csystem.cpp:15749.  ACTIVE tail (the simple
//  iHome/iReset/iCleanOut/iTrayFeed/bCleanoutStart resets).  The AutoSiteMap
//  branch (golden :15751-15785) derefs HotPlateForm / fMain mode setters /
//  MMPlate site grids -- GATED #if 0 // TODO(W7); the cursor resets below are
//  the load-bearing part the HUB needs.
// ===========================================================================
void InitCleanOutFunction()
{
#if 0 // TODO(W7): AutoSiteMap-mode clean-out branch derefs HotPlateForm/fMain mode setters/MMPlate site grids (golden csystem.cpp:15751-15785)
    if(LastSet.iRunStartMode==rsmAutoSiteMap                &&
       LastSet.iTemperature==Tempture_Hot                   &&
       CosFunction.bUSEJCETSiteMapMode)
    {
        if(bAutoSiteMapHasPickHP)
        {
            iResetSiteMappingStep=2;
        }
        else
        {
            bSiteMappingCHKOK=true;
            if(iAutoSiteMapRunStartMode==0)
                fMain->SetMainRunStartMode(rsmContinuStart);
            else
                fMain->SetMainRunStartMode(rsmContinuRetest);

            for(int i=0; i<2; i++)
                for(int j=0; j<HotPlateForm.XDivision; j++)
                    for(int k=0; k<HotPlateForm.YDivision; k++)
                        MOT[MMPlate1+i].Tray.SiteMapData[j][k]=0;
            bAutoSiteMapHotplateSave=false;
            InitInArmTask();
            fMain->ShowTestHeadComp(false);
        }
    }
#endif

    iHome=0;
    iReset=0;
    iCleanOut=1;
    iTrayFeed=0;
    bCleanoutStart=true;                                                        //kevin 20130506
}

// ===========================================================================
//  InitOneCycle  -- golden csystem.cpp:15797.  ACTIVE (cursor/flag resets +
//  RecordProcess log).  RecordProcess is the canary_support sim sink.
// ===========================================================================
void InitOneCycle(AnsiString sFunc, bool bQAModeTrayEnd)                        //Steven 20220319 : 整合觸發One Cycle的動作
{
    if(iCleanOut==1 &&                                                          //jou 2010-12-07 : 防止index check oncyle與clean out funtion衝突
       iTrayFeedTask==1 &&                                                      //jou 2011-07-06 : 改成不論狀況下,都要等到clean out模式,不然會hang up
       iTrayFeed==0 &&
       iHome==0 &&
       iReset==0)
    {
        bBackupCleanOut=true;
    }

    AnsiString Str;
    if(sFunc=="One Cycle Finish")
        Str.sprintf("%s", sFunc.c_str());
    else
        Str.sprintf("Start ONE CYCLE by %s", sFunc.c_str());

    RecordProcess(Str);
    iOneCycle=0;
    iCleanOut=0;
    iHome=0;
    iReset=0;
    if(bQAModeTrayEnd==false)
        iTrayFeed=0;
    bLampOneCycle=true;
}

// ===========================================================================
//  InitTrayEndFunction  -- golden csystem.cpp:15824.  Small cursor reset (ACTIVE).
// ===========================================================================
void InitTrayEndFunction()
{
    iHome=0;
    iReset=0;
    iTrayFeed=0;
}

// ===========================================================================
//  MainProc alive-instrumentation  -- golden csystem.cpp:16673-16709.  VERBATIM.
//  The frozen csystem.h declares Get*/IsMainProcAlive; the oracle reads them.
// ===========================================================================
static unsigned int guMainProcCallCount=0;
static bool gbMainProcEntered=false;
static TDateTime gtMainProcLastEnter=0;
//---------------------------------------
unsigned int GetMainProcCallCount()                                             //JerryYang 20260414 : Add 紀錄機器最後進入時間log (golden Big5 comment glossed)
{
    return guMainProcCallCount;
}
//---------------------------------------
TDateTime GetMainProcLastEnterTime()
{
    return gtMainProcLastEnter;
}
//---------------------------------------
AnsiString GetMainProcLastEnterTimeString()
{
    if(gbMainProcEntered==false)
        return "N/A";

    return FormatDateTime("yyyy/mm/dd hh:nn:ss.zzz", gtMainProcLastEnter);
}
//---------------------------------------
double GetMainProcSilentSeconds()
{
    if(gbMainProcEntered==false)
        return -1.0;

    return (double(Now())-double(gtMainProcLastEnter))*86400.0;
}
//---------------------------------------
bool IsMainProcAlive(int iTimeoutSec)
{
    if(gbMainProcEntered==false)
        return false;

    return GetMainProcSilentSeconds()<=iTimeoutSec;
}

// ===========================================================================
//  MainProc  -- golden csystem.cpp:16711.  OUTER per-frame entry.
//
//  ACTIVE: the InitialOK guard + the alive-instrumentation head (golden
//  :16724-16729) + the SLIM integration-spine dispatch.  The full golden body
//  (the 2000+ line mode / SECS / AGV / temp dispatch ladder: ScanSystemSensor /
//  DoSystem / DoTriTempState / DoHomeProcess / DoTrayFeedProcess and their
//  thousands of lines of UI / comms / DB) is GATED #if 0 // TODO(W7) -- those
//  TUs are not translated.  The integration spine (DoAllProcess +
//  DoOneCycleFinishCheck + DoCleanOutFinishCheck) is dispatched directly, which
//  is exactly the inner-most call the golden makes at :18728-18740 once it has
//  walked the (gated) mode ladder into the normal Run path.
// ===========================================================================
void MainProc()
{
    if(InitialOK==false)                                                        //Steven 20141023 : for 解構防呆
        return;

    gtMainProcLastEnter=Now();                                                  //JerryYang 20260414 : Add 紀錄機器最後進入時間log
    guMainProcCallCount++;
    gbMainProcEntered=true;

#if 0 // TODO(W7): the full MainProc mode/SECS/AGV/temp dispatch ladder (golden csystem.cpp:16730-19101) -- ScanSystemSensor/DoSystem/DoTriTempState/DoHomeProcess/DoTrayFeedProcess + their UI/comms/DB bodies are not translated this wave.
    // ... golden :16730-18727 walks the mode ladder, eventually reaching the
    // CheckContinusStartIsReady() Run path that contains the spine calls below ...
#endif

    // ---- SLIM integration-spine dispatch (golden :18728-18740, the inner-most
    //      Run-path calls -- ACTIVE) -------------------------------------------
#ifdef DEBUG_TRY_CATCH
    try { DoAllProcess(); } catch(...) { MyDBIProcess("Exception", "DoAllProcess()"); }
    try { DoOneCycleFinishCheck(); DoCleanOutFinishCheck(); }
    catch(...) { MyDBIProcess("Exception", "DoOneCycleAndCleanOutFinishCheck()"); }
#else
    DoAllProcess();                                                            // golden :18728
    DoOneCycleFinishCheck();                                                   // golden :18739
    DoCleanOutFinishCheck();                                                   // golden :18740
#endif
}

// ===========================================================================
//  DoAllProcess  -- golden csystem.cpp:9115.  THE SPINE: the per-tick engine
//  driver.  The static bDoProcess 2-tick alternation (:9120 / toggled :10254)
//  drives a mutually-exclusive InArm/SHT1 vs OutArm/SHT2 interleave.
//
//  NET ENGINE ORDER over a tick PAIR (bDoProcess starts true):
//    TICK A (bDoProcess==true) : DoLoad, DoInArm, Do_Auto_SHT1, DoTestHeadMotor, DoCatchTray
//    TICK B (bDoProcess==false): DoLoad, Do_Auto_SHT2, DoTestHeadMotor, DoCatchTray, DoOutArm(+DoSortArm if 9046AU)
//  (Do_Auto_SHT3 only when USE_OUT_SORT_ARM!=eartUninstall -- default uninstall -> skipped.)
//  This 2-phase interleave is THE oracle the W6.6 verify TU confirms.
//
//  GATED (#if 0 // TODO(W7)): the AutoRetest yield-math (golden :9165-9471), the
//  Laser/InitialICCheck/Magazine/Cylinder/LoadCell/ATCTemp/Decay/AutoClean/
//  EmptySocketCheck/OCR/GPIB-wait/UTAC-wait/ClearSocket early-return ladder
//  (golden :9477-10047).  Those reach untranslated UI / OCR / DB / SECS surface.
//  NOTE: when ungated, the AutoClean block keeps DoAutoCleanKit /
//  DoShuttle1/2AutoClean / DoIndexAutoClean active (those engines exist); the
//  block is gated WHOLESALE this wave because its surrounding guards
//  (fContact->IsRun2DCheck / ShowMyMessage / fOCR / fLotInfo) are not.
// ===========================================================================
void DoAllProcess()
{
    int cmpt=MAX_AUTO_TRAY;                                                     //Frank 20170109 add
    bool flag, flag2=true;                                                      //ChungHung  modify flag2--->flag2=true

    static bool bDoProcess=true;                                                //Steven 20110608 : 降低CPU負載

    fMain->ProcessSensorScan();                                                //ChungHung 20140815 測試用
    CSYS_TICK(CT_SENSORSCAN);

#if 0 // TODO(W7): AutoRetest/Laser/InitialICCheck/Magazine/Cylinder/LoadCell/ATCTemp/Decay/AutoClean/EmptySocketCheck/OCR/GPIB-wait/UTAC-wait early-return ladder (golden csystem.cpp:9165-10047) -- reaches untranslated UI/OCR/DB/SECS surface
    if(LastSet.iRunStartMode==rsmAutoRetest || bART_needRT2) { flag=DoAutoRetest(); /* ... ~300 lines ... */ return; }
    flag=true;
    if(bInitialLaserCheck)         { /* ShtLaserInit ladder */ return; }
    if(bInitialICCheck)            { DoInitialICCheck(); return; }
    if(bRunAutoClean && ...)       { DoAutoCleanKit(); DoShuttle1AutoClean(); DoShuttle2AutoClean(); DoIndexAutoClean(); return; }  // engines exist; gated wholesale with their UI guards
    if(bDoEmptySocketCheck)        { DoIndexSocketCheck(); return; }
    if(... bRunOcrInsp ...)        { DoOCRFlow1()/DoOCRFlow(); return; }        // OCR / fLotInfo UI
    if(CosFunction.bAutoRetestGPIBmode==true) { /* GPIB lot-start wait */ }
    if(CUSTOMER_CODE==CC_UTAC)     { /* UTAC online wait */ }
    if(iClearSocketFunction==2)    { DoCleanSocket(); return; } else SW[SwSocketClean].Off();
#endif
    flag=true;                                                                  // golden :9473 (normal Run path -- gates DoLoad/DoCatchTray/Auto-receive)

    if(SoftStop==true || SystemStart==false || fAllMotorHome==false)            //Jou 20180102 (Steven) : 後面加上 SystemStart==false
    {
        return;
    }

    if(flag)
    {
        if(TrayForm.bEnableAMR)                                                 //Eastsun 20260514 F010 AMR loader lock
        {
            if(bLoaderLockActionFlag[0]==false)
                DoLoad();
        }
        else
        {
            DoLoad();
        }
        CSYS_TICK(CT_DOLOAD);
    }

    if(SoftStop==true || SystemStart==false || fAllMotorHome==false)            //Jou 20180102 (Steven) : 後面加上 SystemStart==false
    {
        return;
    }

    if(bDoProcess)                                                              //Steven 20110608
    {
        DoInArm();
        CSYS_TICK(CT_DOINARM);
    }

    if(bShuttleShake)                                                           //Sam 20210701 : Tray Arm 放好放 Tray 時，放好的好好動作，Tray Arm 將 InArm 抬起來導致好好動作無法完成。
    {
    }
    else
    {
        if(SoftStop==true || SystemStart==false || fAllMotorHome==false)        //Jou 20180102 (Steven) : 後面加上 SystemStart==false
        {
            return;
        }

        if(bDoProcess)                                                          //Steven 20110608
        {
            Do_Auto_SHT1();
            CSYS_TICK(CT_SHT1);
        }
        else
        {
            Do_Auto_SHT2();
            CSYS_TICK(CT_SHT2);
        }

        if(USE_OUT_SORT_ARM!=eartUninstall)
        {
            Do_Auto_SHT3();                                                     //RogerYang 20250514 Add for 9046AU
            CSYS_TICK(CT_SHT3);
        }

        if(SoftStop==true || SystemStart==false || fAllMotorHome==false)        //Jou 20180102 (Steven) : 後面加上 SystemStart==false
            return;

        DoTestHeadMotor();
        CSYS_TICK(CT_TESTHEAD);
    }

    if(SoftStop==true || SystemStart==false || fAllMotorHome==false)            //Jou 20180102 (Steven) : 後面加上 SystemStart==false
        return;

    if(flag)
    {
        DoCatchTray();
        CSYS_TICK(CT_CATCHTRAY);
    }

    if(SoftStop==true || SystemStart==false || fAllMotorHome==false)            //Jou 20180102 (Steven) : 後面加上 SystemStart==false
        return;

    if(bDoProcess==false)                                                       //Steven 20110608
    {
        DoOutArm();
        CSYS_TICK(CT_OUTARM);
        if(USE_OUT_SORT_ARM!=eartUninstall)                                     //RogerYang 20250515 add for 9046AU
        {
            DoSortArm();
            CSYS_TICK(CT_SORTARM);
        }
    }

    if(SoftStop==true || SystemStart==false || fAllMotorHome==false)            //Jou 20180102 (Steven) : 後面加上 SystemStart==false
        return;

    if(flag)
    {
        flag2=false;

        if(AUTO3_IS_MAGAZINE==1)                                                //JerryYang 20220909 : add magazine
        {
            DoAuto3Magazine();
            cmpt=2;
        }

        if(AUTO_EMPTY_COLOR<3)
        {
            if(AUTO3_IS_MAGAZINE==1)                                            //JerryYang 20220909 : add magazine
                cmpt=2;
            else
                cmpt=3;
        }
        else if(AUTO_EMPTY_COLOR==3)
        {
            if(AUTO3_IS_MAGAZINE==1)
                cmpt=4;
            else
                cmpt=5;
        }
        else if(AUTO_EMPTY_COLOR==4)
        {
            if(AUTO3_IS_MAGAZINE==1)
                cmpt=5;
            else
                cmpt=6;
        }

        for(int i=0; i<cmpt; i++)
        {
            flag2|=Cylinder[C_AutoEdgePush[i]].Enable;                          // 2011.10.19 , Q_Q V207C new Tray Vibration
            if(TrayForm.AutoFromEmptyColor[iRunStartMode][i]==2 && i==1)        //kevin 20120726   AUTO 2
            {
            }
            else
            {
                if(TrayForm.bEnableAMR)                                         //Eastsun 20260514 F010 AMR unloader lock
                {
                    if(bUnLoaderLockActionFlag[i]==false)
                        DoAutoReceiveBinTray(i);
                }
                else
                {
                    DoAutoReceiveBinTray(i);
                }
            }

            if(SoftStop==true || SystemStart==false || fAllMotorHome==false)    //Jou 20180102 (Steven) : 後面加上 SystemStart==false
                return;
        }
    }

    flag2|=Cylinder[C_TrayVibration].Enable;
    // 2011.10.19 , Q_Q V207C new Tray Vibration{
    if(TRAY_VIBRATION!=NonVibration && flag2)                                   //JerryYang 20170531 (wei) 向tray方式新增起動馬達
    {
        NewDoAutoTrayEdgeCylinderLoop();
    }
    //}

    if(AUTO_EMPTY_COLOR!=0)                                                     //Eliot 2007_12_10
    {
        if(flag)
        {
            if(TrayForm.bEnableAMR)                                             //Eastsun 20260514 F010 AMR empty/color lock
            {
                if(bLoaderLockActionFlag[1]==false)
                    DoAutoEmpty();
                if(bLoaderLockActionFlag[2]==false)
                    DoAutoColor();
            }
            else
            {
                DoAutoEmpty();
                DoAutoColor();
            }
        }
    }

    if(bUseAuto2Empty && TrayForm.AutoFromEmptyColor[iRunStartMode][eAuto2]==2)                                         //使用auto2 送tray
         DoAuto2();                                                             //kevin 20120718 auto2送空tray

    if(SUPPORT_2_EMPTY_EMPTY)                                                   //Eliot 2007_12_10
    {
        DoAutoEmpty1();
    }

    //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
    //==>
    //ChungHung 20210113 add for Alignment CCD start
    if(bLoaderNeedTrayMustFinish==false && bAutoNeedTrayMustFinish==false)
    {
        if(bRunInArmAutoAlignment)
        {
            bRunInArmAutoAlignment=!DoInArmTeachAlignmentProcess(lInArmAutoAlignmentFlag);
        }
        else
        {
            bInArmAutoAlignmentClearFlag=false;                                 //Kenhsieh 20211007 : 解決沒有把飛Kit問題
        }

        if(bRunOutArmAutoAlignment)
        {
            bRunOutArmAutoAlignment=!DoOutArmTeachAlignmentProcess(lOutArmAutoAlignmentFlag);
        }
        else
        {
            bOutArmAutoAlignmentClearFlag=false;                                //Kenhsieh 20211007 : 解決沒有把飛Kit問題
        }

        if(bRunInArmAutoAlignment==true || bRunOutArmAutoAlignment==true)
            return;
    }
    //ChungHung 20210113 add for Alignment CCD end
    //<==
    //KenHsieh 20210813 : add CCD AUTO ALIGNMENT

    DoHotplateEdgeCylinderLoop();                                               //jou 2011-08-09 start : Hotplate也要敲敲敲
    DoLoaderVibrateLoop();                                                      //JerryYang 20191001 loader起動馬達
    bDoProcess=!bDoProcess;                                                     //Steven 20110608
}

// ===========================================================================
//  DoHotplateEdgeCylinderLoop / DoLoaderVibrateLoop -- golden csystem.cpp:
//  (tail cylinder loops; declared in the frozen csystem.h).  The full bodies
//  walk the Cylinder[] edge-push arrays + loader vibration motor; offline they
//  are no-ops so the spine tail links + pumps.  GATED #if 0 with active stubs.
// ===========================================================================
void DoHotplateEdgeCylinderLoop()
{
#if 0 // TODO(W7): Hotplate edge-cylinder knock loop (golden csystem.cpp) -- Cylinder[] edge-push arrays + timing
#endif
}
void DoLoaderVibrateLoop()
{
#if 0 // TODO(W7): loader vibration-motor loop (golden csystem.cpp) -- MOT[MLoader*] vibrate + timing
#endif
}

// ===========================================================================
//  CheckInArmSuckICFallDownToHasNullIC -- golden csystem.cpp:1677 (declared
//  csystem.h:86).  W6.2b-canary: the in-arm site-variant SMs (ainarm9045_1x1_1
//  / _2x4_16 / _2x8_32) call this drop-detect/recover predicate, so it must be
//  defined to link.  FAITHFUL line-for-line translation -- names, formulas,
//  branches and Chinese comments preserved verbatim.
//
//  Offline-safe over the Sim HAL: it walks the InArmSuck grid reading each
//  nozzle's vacuum sensor (Suck[][].GetStatus(), sim=false=no IC), the Z-axis
//  home LED (MOT[].Led[iHomeLed]) and IniConfig flags; only on a real drop
//  (bHasErr && bAlarm) does it call ShowErrorMessage (canary_support: offline
//  K_SKIP) and re-classify the grid -- all data over the substrate, no hardware.
//  The #ifdef SOFT_SIMULTE injection block (BCB6 sim flag, NOT defined in this
//  build) compiles out, exactly as in the BCB6 non-sim release.
//  Steven 20110516 : 修改成整合式Alarm  //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
// ===========================================================================
bool CheckInArmSuckICFallDownToHasNullIC(bool bAlarm)                           //Steven 20110516 : 修改成整合式Alarm  //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
{
    AnsiString ErrPart="", sRetryLog="";;
    int ret=0;
    bool bHasErr=false;
    bool bIsSuckICFallDown[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false},
                                                      {false, false, false, false}};

    static int iSuckICFallDownCnt[MAX_ARM_Row][MAX_ARM_Col]={{0, 0, 0, 0},      //Sam 20221006 : In/Out Arm IC 掉落狀態多檢查幾次再報警
                                                             {0, 0, 0, 0}};

    if(LastSet.iRealDummy==REALLY)
    {
        if(IniConfig.bInOutArmPlaceSkipSuckDetect==true)                        //Steven 20171219 (Wei) : 修正[E35], 改成判斷Z軸是不是在Home點
        {
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(MOT[InArmZIndex[i][j]].Motor->Enable==true   &&
                       MOT[InArmZIndex[i][j]].Led[iHomeLed]==true   &&
                       InArmSuck.Suck[i][j].Enable                  &&
                       InArmSuck.Suck[i][j].SenUsing!=""            &&
                       InArmSuck.Item[i][j]!=HAS_NULL_IC            &&
                       InArmSuck.Item[i][j]!=HAS_NULL_CLEAN_IC      &&          //JerryYang 20160217 AutoClean hang up問題
                       InArmSuck.Item[i][j]!=NULL_IC)
                    {
                        if(InArmSuck.Suck[i][j].GetStatus()==false)
                        {
                            iSuckICFallDownCnt[i][j]++;
                            if(IniConfig.bE68RecheckInOutArmICFallDown &&       //Sam 20221006 : In/Out Arm IC 掉落狀態多檢查幾次再報警
                               iSuckICFallDownCnt[i][j]>IniConfig.iE68RecheckInOutArmICFallDown)
                            {
                                bHasErr=true;
                            }
                            else
                            {
                                bIsSuckICFallDown[i][j]=true;
                                ErrPart+=InArmSuck.Suck[i][j].sName;
                                bHasErr=true;
                            }
                        }
                        else
                        {
                            if(IniConfig.bE68RecheckInOutArmICFallDown &&
                               iSuckICFallDownCnt[i][j]>0)                      //Sam 20221006 : In/Out Arm IC 掉落狀態多檢查幾次再報警
                            {
                                sRetryLog.sprintf("iSuckICFallDownCnt[%d][%d]=%d", i, j, iSuckICFallDownCnt[i][j]);
                                NewRecordProcess("", "InArm E68 retry record", sRetryLog);
                            }
                            iSuckICFallDownCnt[i][j]=0;
                        }
                    }
                }
            }
        }
        else
        {
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(InArmSuck.Suck[i][j].Enable                  &&
                       InArmSuck.Suck[i][j].SenUsing!=""            &&
                       InArmSuck.Item[i][j]!=HAS_NULL_IC            &&
                       InArmSuck.Item[i][j]!=HAS_NULL_CLEAN_IC      &&          //JerryYang 20160217 AutoClean hang up問題
                       InArmSuck.Item[i][j]!=NULL_IC)
                    {
                        #ifdef SOFT_SIMULTE
                        if(fMain->chkInToShtDrop->Checked &&
                           i==atoi(fMain->edHPY->Text.c_str()) &&
                           j==atoi(fMain->edHPX->Text.c_str()))
                        {
                            bIsSuckICFallDown[i][j]=true;
                            ErrPart+=InArmSuck.Suck[i][j].sName;
                            bHasErr=true;
                        }
                        #endif
                        if(InArmSuck.Suck[i][j].GetStatus()==false)
                        {
                            iSuckICFallDownCnt[i][j]++;
                            if(IniConfig.bE68RecheckInOutArmICFallDown &&       //Sam 20221006 : In/Out Arm IC 掉落狀態多檢查幾次再報警
                               iSuckICFallDownCnt[i][j]>IniConfig.iE68RecheckInOutArmICFallDown)
                            {
                                bHasErr=true;
                            }
                            else
                            {
                                bIsSuckICFallDown[i][j]=true;
                                ErrPart+=InArmSuck.Suck[i][j].sName;
                                bHasErr=true;
                            }
                        }
                        else
                        {
                            if(IniConfig.bE68RecheckInOutArmICFallDown &&
                               iSuckICFallDownCnt[i][j]>0)                      //Sam 20221006 : In/Out Arm IC 掉落狀態多檢查幾次再報警
                            {
                                sRetryLog.sprintf("iSuckICFallDownCnt[%d][%d]=%d", i, j, iSuckICFallDownCnt[i][j]);
                                NewRecordProcess("", "InArm E68 retry record", sRetryLog);
                            }
                            iSuckICFallDownCnt[i][j]=0;
                        }
                    }
                }
            }
        }
    }

    if(bHasErr && bAlarm)
    {
        if(IniConfig.bE68RecheckInOutArmICFallDown)                             //Sam 20221006 : In/Out Arm IC 掉落狀態多檢查幾次再報警
        {
            ErrPart="";
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(iSuckICFallDownCnt[i][j]>0)
                    {
                        iSuckICFallDownCnt[i][j]=0;
                        bIsSuckICFallDown[i][j]=true;
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                    }
                }
            }
        }

        if(IniConfig.bNewResetFunction==true && bResetInArm==true)
        {
            bResetInArm=false;
            ret=K_SKIP;
        }
        else
        {
            if(fContact->IsRun2DCheck())                                        //JerryYang 20250220 : 2DID硬體順序檢查功能
            {
                ret=ShowErrorMessage("JAM0126", K_SKIP, MInArmX, false, ErrPart);                                       //kevin 20120418 //Steven 20091123 : Device Drop Error
            }
            else if(bRunAutoClean)                                              //kevin 20121020 add //Steven 20121015 : Auto Clean有Alarm要開後門
            {
                bAutoCleanCheckOpenDoor=true;
                ret=ShowErrorMessage("JAM0128", K_SKIP, MInArmX, false, ErrPart);                                       //JerryYang 20160511 Auto Clean時要show出 clean pad 掉料異常
            }
            else
            {
                ret=ShowErrorMessage("JAM0126", K_SKIP, MInArmX, false, ErrPart);                                       //kevin 20120418 //Steven 20091123 : Device Drop Error
            }
        }

        if(SoftStop)
            fMain->Pause("CheckInArmSuckICFallDownToHasNullIC");

        if(ret==K_SKIP)
        {
            bAutoSiteMapWaitTestResult=false;                                   //Ifor 20180115 (Steven) : add Site Mapping SKIP 需清除旗標
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(bIsSuckICFallDown[i][j])
                    {
                        if(bAutoCleaning==true)                                 //JerryYang 20160217 AutoClean中掉Clean pad, skip後要設為HAS_NULL_CLEAN_IC
                        {
                            InArmSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);
                        }
                        else
                        {
                            if(CosFunction.bUseSCKART)                          //Steven 20161214 (wei) : For SCK ART
                                fSCKART->iInputJamCnt++;
                            InArmSuck.PordRec[i][j].AddErrorRecord("JAM0126");  //Steven 20161214 : Add Jam Record
                            InArmSuck.SetItemData(i, j, HAS_NULL_IC);           //Isaac 20170706 (Steven) AutoClean HangUp fix
                        }

//                        InArmSuck.SetItemData(i, j, HAS_NULL_IC);             //Isaac 20170706 (Steven) AutoClean HangUp fix，往上移
                        InArmSuck.Suck[i][j].Normal();                          //Steven 20101112    //Steven 20120612 : index掉料跟in Arm掉料同時發生會Hang Up
                    }
                }
            }

            if(bRunAutoSiteMapping==true)
            {
                DoAutoSiteMappingDropError();                                   //Ifor 20210524 add 避免補料時掉料導致Site Mapping Hang up
                bPlaceToHotplate=false;
            }
        }
    }
    return bHasErr;
}

// ===========================================================================
//  W7-C1 SEAM BLOCK  -- TU-local forward declarations / offline stand-ins for
//  the substrate symbols DoCleanOutFinishCheck (below) references that are NOT
//  yet present in the translated tree.  Each is config-gated (false offline) or
//  a small reset target; the stand-ins make this TU SYNTAX-COMPILE so the
//  faithful golden body can land verbatim NOW.  The serial Integrate must:
//    (a) replace every W7C1_SEAM_* stand-in with the REAL translated symbol /
//        the real facade member, and
//    (b) remove the csystem_shims.cpp:146 / csystem_shims.h:87
//        `void DoCleanOutFinishCheck(){}` no-op stub (ODR: two definitions).
//  Every symbol below is itemised in seamNeeds with its golden home + the
//  offline (inert) behaviour that keeps the no-regression key intact: the whole
//  body is INERT unless (iCleanOut==1 && !iOneCycle), and the config guards
//  around these derefs (USE_AUTO_RETEST/bUseSCKART/bEnable_SECS_GEM/IniConfig.*/
//  CUSTOMER_CODE/TrayForm.bEnableAMR/REAL_TIME_CCD/INSTALL_OCR) are all false in
//  the offline sim, so none of these stand-ins is reached at runtime offline.
// ===========================================================================
#ifndef W7C1_SEAM
#define W7C1_SEAM

// --- absent FREE globals / arrays (golden cmydef / aHotPlateSubstrate homes) ---
static bool                 W7C1_bFix3_CleanOutTimeSet = false;   // golden cmydef -- Fix3 clean-out alarm timer set flag
#define bFix3_CleanOutTimeSet  W7C1_bFix3_CleanOutTimeSet
static bool                 W7C1_bNeedQuickCleanOut    = false;   // golden cmydef -- quick clean-out request (Steven 20110524)
#define bNeedQuickCleanOut     W7C1_bNeedQuickCleanOut
static bool                 W7C1_bBackMMTrayY          = false;   // golden cmydef -- restore-MMTrayY hasTray flag
#define bBackMMTrayY           W7C1_bBackMMTrayY
static bool                 W7C1_bBackMMTrayY_Car      = false;   // golden cmydef -- restore-MMTrayY_Car hasTray flag
#define bBackMMTrayY_Car       W7C1_bBackMMTrayY_Car
static int                  W7C1_iHotInArmOrder[2][50][50];       // golden aHotPlateSubstrate -- HP in-arm pick order grid (reset to -1)
#define iHotInArmOrder         W7C1_iHotInArmOrder
static AnsiString           W7C1_sCleanPlaceRec;                  // golden aHotPlateSubstrate -- clean-place rec filename
#define sCleanPlaceRec         W7C1_sCleanPlaceRec

// --- absent FREE objects with the methods the body calls ----------------------
//  FixTrayAlarmCheck : golden timer object (SetSecAndOn(int)/Off()).
struct W7C1_TTimerSeam { void SetSecAndOn(int /*s*/){} bool Off(){ return false; } };
static W7C1_TTimerSeam      W7C1_FixTrayAlarmCheck;
#define FixTrayAlarmCheck      W7C1_FixTrayAlarmCheck
//  AOIKit : golden TMySucker (UseSiteHasIC inside if(USE_AOI_Inspection), default off).
struct W7C1_TKitSeam   { bool HasIC(){ return false; } };
static W7C1_TKitSeam        W7C1_AOIKit;
#define AOIKit                 W7C1_AOIKit
//  PickFromHPList / PlaceToCleanList : golden record-list objects.  The golden
//  calls ->ResetFile(AnsiString); the translated uPlateInfo exposes SaveFile but
//  NOT ResetFile yet, so BOTH ->ResetFile calls are seamed to a stand-in list.
struct W7C1_TRecListSeam { void ResetFile(AnsiString /*fn*/){} };
static W7C1_TRecListSeam    W7C1_PlaceToCleanList;
static W7C1_TRecListSeam    W7C1_PickFromHPList;
#define PlaceToCleanList       (&W7C1_PlaceToCleanList)
#define PickFromHPList         (&W7C1_PickFromHPList)
//  sHPPickRec : golden aHotPlateSubstrate filename -- exists, but ResetFile is the
//  seam; keep the real sHPPickRec arg flowing into the stand-in ResetFile.
//  fContactCT : golden TfContactCT* (rgYieldType->ItemIndex / ClearData(i,j)).
struct W7C1_TRadioSeam   { int ItemIndex; W7C1_TRadioSeam():ItemIndex(0){} };
struct W7C1_TfContactCTSeam { W7C1_TRadioSeam *rgYieldType; void ClearData(int,int){}
                              W7C1_TfContactCTSeam(){ rgYieldType=new W7C1_TRadioSeam(); } };
static W7C1_TfContactCTSeam *W7C1_fContactCT = new W7C1_TfContactCTSeam();
#define fContactCT             W7C1_fContactCT
//  fCounterClear : golden TfCounterClear* (LowYieldSpecialInitail()).
struct W7C1_TfCounterClearSeam { void LowYieldSpecialInitail(){} };
static W7C1_TfCounterClearSeam *W7C1_fCounterClear = new W7C1_TfCounterClearSeam();
#define fCounterClear          W7C1_fCounterClear
//  fShowBinSelect : golden TfShowBinSelect* (UPH_StringGrid->Cells[c][r]).
struct W7C1_TGridSeam { AnsiString &Cells(int,int){ static AnsiString s; return s; } };
//  golden indexes Cells[col][row]; expose Cells as a 2-step proxy.
struct W7C1_TGridProxy { AnsiString s; AnsiString &operator[](int){ return s; } };
struct W7C1_TGridCells { W7C1_TGridProxy p; W7C1_TGridProxy &operator[](int){ return p; } };
struct W7C1_TfShowBinSelectSeam { W7C1_TGridCells UPH_StringGrid; };
static W7C1_TfShowBinSelectSeam *W7C1_fShowBinSelect = new W7C1_TfShowBinSelectSeam();
#define fShowBinSelect         W7C1_fShowBinSelect
//  ASESendMessage->SendToASEData : golden ASE comms object (1-arg send).
struct W7C1_TAseSeam { void SendToASEData(AnsiString /*s*/){} };
static W7C1_TAseSeam        W7C1_ASESendMessage_obj;
#ifndef ASESendMessage
#define W7C1_ASESendMessage_GUARD
static W7C1_TAseSeam       *ASESendMessage = &W7C1_ASESendMessage_obj;
#endif

// --- absent FREE functions ----------------------------------------------------
static bool W7C1_Fix3MoveToLeft(bool /*b*/){ return false; }            // golden -- Fix3 stepper to right; offline never reaches left
#define Fix3MoveToLeft         W7C1_Fix3MoveToLeft
static void W7C1_InitialAutoCleanTask(){}                               // golden AutoClean init (Steven)
#define InitialAutoCleanTask   W7C1_InitialAutoCleanTask
static void W7C1_InitialShuttleAutoCleanTask(){}                        // golden AutoClean init
#define InitialShuttleAutoCleanTask  W7C1_InitialShuttleAutoCleanTask
static void W7C1_InitialIndexAutoCleanTask(){}                          // golden AutoClean init
#define InitialIndexAutoCleanTask    W7C1_InitialIndexAutoCleanTask
static void W7C1_ResetInArmParam(){}                                    // golden -- in-arm param reset
#define ResetInArmParam        W7C1_ResetInArmParam
static bool W7C1_MoveInArm2XYToLoaderWait(){ return true; }             // golden -- E61 in-arm->loader; offline: done (true)
#define MoveInArm2XYToLoaderWait     W7C1_MoveInArm2XYToLoaderWait
static bool W7C1_DoART_AfterCleanOut(int &/*ret*/){ return true; }      // golden -- ART post-clean-out; gated USE_AUTO_RETEST(off)
#define DoART_AfterCleanOut    W7C1_DoART_AfterCleanOut
static void W7C1_SetInArmSpeed(bool /*a*/, bool /*b*/){}                 // golden -- in-arm speed reset
#define SetInArmSpeed          W7C1_SetInArmSpeed
static void W7C1_SetOutArmSpeed(bool /*a*/){}                           // golden -- out-arm speed reset
#define SetOutArmSpeed         W7C1_SetOutArmSpeed
static void W7C1_SetSortArmSpeed(bool /*a*/){}                          // golden -- sort-arm speed reset (9046AU; USE_OUT_SORT_ARM off)
#define SetSortArmSpeed        W7C1_SetSortArmSpeed
static void W7C1_VerifyNeedDoAlignment(int /*a*/, int /*b*/){}          // golden -- CCD auto-alignment (KenHsieh 20210813)
#define VerifyNeedDoAlignment  W7C1_VerifyNeedDoAlignment
static void W7C1_ResetHotPlateSearchParameter(){}                      // golden -- HP search param reset
#define ResetHotPlateSearchParameter  W7C1_ResetHotPlateSearchParameter
static void W7C1_RecordAutoCleanOutStartEnd(bool /*b*/){}              // golden -- SCK auto-clean-out record
#define RecordAutoCleanOutStartEnd    W7C1_RecordAutoCleanOutStartEnd
//  ReadTestMode (golden cprod.cpp:3479): recipe-file TestMode reader.  The real
//  cprod.cpp body is gated #if 0 // TODO(W6); it is called ONLY inside the
//  if(bASECleanOutCloseSite) branch (false offline -> no ASE close-site).  TU-
//  local no-op stand-in (offline: no recipe re-read).  Integrate: un-gate the
//  cprod.cpp definition.  AI(W7C1-Integrate) 20260629.
static void W7C1_ReadTestMode(){}                                      // golden cprod.cpp:3479 (gated; offline no-op)
#define ReadTestMode           W7C1_ReadTestMode
//  RespondASECom : REAL (cpublic.h, bool) -- NO seam.
//  DoLowYieldAlarm : REAL (atester_shims.h, int) -- NO seam.
//  InitialPiggyBackFunction : REAL (atester_shims.h) -- NO seam.

// --- absent record/util symbols (not visible in this TU's include set) --------
//  HPPlaceLog : golden HP-place position log (InitialPosition()).
struct W7C1_THPPlaceLogSeam { void InitialPosition(){} };
static W7C1_THPPlaceLogSeam W7C1_HPPlaceLog;
#define HPPlaceLog             W7C1_HPPlaceLog
//  fLtcSensor : REAL (acarry_shims.h) but acarry_shims.h is NOT included here.
//  Gate the two ClearLtcSensor calls (offline: latch clear no-op).
#define W7C1_FLTCSENSOR_CLEAR(w)  do { } while(0)   // golden fLtcSensor->ClearLtcSensor(w)
//  GetLastOpenFN / DataPath / WriteIniData (common.h; GetLastOpenFN is #if 0 there,
//  WriteIniData is __fastcall).  Used only inside if(bRestModeBackupParm) (false
//  offline).  Provide TU-local stand-ins (offline: no recipe file written).
static AnsiString W7C1_GetLastOpenFN(){ return AnsiString(""); }
#define GetLastOpenFN          W7C1_GetLastOpenFN
static AnsiString W7C1_DataPath_seam;
#define DataPath               W7C1_DataPath_seam
static void W7C1_WriteIniData(AnsiString,AnsiString,AnsiString,int){}   // golden common.h WriteIniData(int overload)
static void W7C1_WriteIniData(AnsiString,AnsiString,AnsiString,double){}// golden common.h WriteIniData(double overload)
#define WriteIniData           W7C1_WriteIniData

// --- facade members ABSENT (gate the deref statements) ------------------------
//  fBarCode->Change2DSetupFile / fTrayMapping->ChangeTraySetupFile -- absent on
//  the offline stubs; gate (offline: 2D/Tray-mapping setup-file reset no-op).
#define W7C1_FBARCODE_CHANGE2D()      do { } while(0)   // golden fBarCode->Change2DSetupFile()
#define W7C1_FTRAYMAP_CHANGETRAY()    do { } while(0)   // golden fTrayMapping->ChangeTraySetupFile()
//  SECS_EVENT (ETypeStruct) lacks CleanOutFinish / AGVSupplement members; the
//  EventReport calls are inside if(IniConfig.bEnable_SECS_GEM)/if(TrayForm.bEnableAMR)
//  (both false offline).  Gate the two reports (offline: no SECS event emitted).
#define W7C1_EVENTREPORT_CLEANOUTFINISH()  do { } while(0)  // golden EventReport(SECS_EVENT.CleanOutFinish)
#define W7C1_EVENTREPORT_AGVSUPPLEMENT()   do { } while(0)  // golden EventReport(SECS_EVENT.AGVSupplement)

// --- facade members ABSENT from FormsFacade.h (gate the deref statements) -----
//  These reside in CONFIG-GATED branches (false offline) so gating the access
//  changes NO offline behaviour; Integrate adds the real members + drops gates.
//  W7-C1 INTEGRATED: fMain->Start / ChangeLevelAttr / ModifyTester /
//  CleanYieldCount / BtnOneCycle->Down are now REAL members on the FormsFacade
//  TfMain (FormsFacade.h, W7-C1 ADD); the macros dispatch to the real fMain.
#define W7C1_FMAIN_START(s)        fMain->Start(s)
#define W7C1_FMAIN_CHANGELEVEL()   fMain->ChangeLevelAttr()
#define W7C1_FMAIN_MODIFYTESTER(t) fMain->ModifyTester(t)
#define W7C1_FMAIN_CLEANYIELD()    fMain->CleanYieldCount()
#define W7C1_FMAIN_BTNONECYCLE_DOWN  (fMain->BtnOneCycle->Down)

//  fSCKART extended members (golden Automation/SCK_ART.h) absent from the W6 stub.
struct W7C1_TfSCKARTSeam {
    int  iTesterType;       int iLOTSTATUS_L;        int iWaitGPIBLotR;
    int  iCurrentFlexARTStep; int iInputJamCnt;      int iOutputJamCnt;
    void SetLotStatus(int){}
    bool DoChkInputCntAlarm(bool){ return false; }
    void SaveTestSummary(int){}
    W7C1_TfSCKARTSeam():iTesterType(0),iLOTSTATUS_L(0),iWaitGPIBLotR(0),
                        iCurrentFlexARTStep(0),iInputJamCnt(0),iOutputJamCnt(0){}
};
static W7C1_TfSCKARTSeam    W7C1_fSCKART_ext;
#define W7C1_SCKART          (&W7C1_fSCKART_ext)

//  fAGV->IsSPIL_AMR (golden Automation/AGV.h) absent (only IsATK_AMR present).
struct W7C1_TfAGVSeam { bool IsSPIL_AMR(){ return false; } };
static W7C1_TfAGVSeam       W7C1_fAGV_ext;
#define W7C1_FAGV_ISSPIL()   W7C1_fAGV_ext.IsSPIL_AMR()

//  fLotInfo tray-count labels (golden uLotInfo.h) absent from the W6 stub.
struct W7C1_TfLotInfoLabelSeam { AnsiString Caption; };
static W7C1_TfLotInfoLabelSeam W7C1_labLotTrayCount, W7C1_LabDiffTrayCount, W7C1_labNowTrayCount;
#define W7C1_LOTINFO_LABLOTTRAY   W7C1_labLotTrayCount
#define W7C1_LOTINFO_LABDIFFTRAY  W7C1_LabDiffTrayCount
#define W7C1_LOTINFO_LABNOWTRAY   W7C1_labNowTrayCount

//  fContact->ReadFile (golden cContact.h) -- fContact exists; ReadFile may be
//  absent on the offline stub.  Gate the single call.
#define W7C1_FCONTACT_READFILE()  do { } while(0)   // golden fContact->ReadFile(); offline no-op

//  LastSet extended members (golden LastSet.h) absent from canary_support shim.
static bool W7C1_LastSet_bWaitStartLotAutoRetestGPIB = false; // golden LastSet.h
static int  W7C1_LastSet_iLoaderTotalTray = 0;                // golden LastSet.h
static int  W7C1_LastSet_iLoaderTrayCount_ART = 0;            // golden LastSet.h
static int  W7C1_LastSet_iBarcodeTrayRecFileRetestCT = 0;     // golden LastSet.h
#define W7C1_LS_bWaitStartLotAutoRetestGPIB  W7C1_LastSet_bWaitStartLotAutoRetestGPIB
#define W7C1_LS_iLoaderTotalTray             W7C1_LastSet_iLoaderTotalTray
#define W7C1_LS_iLoaderTrayCount_ART         W7C1_LastSet_iLoaderTrayCount_ART
#define W7C1_LS_iBarcodeTrayRecFileRetestCT  W7C1_LastSet_iBarcodeTrayRecFileRetestCT

//  fOCR->DoOCRReleaseAndInspEnd (golden uOCR.h) -- fOCR absent.  Gate.
#define W7C1_FOCR_RELEASEINSPEND()  do { } while(0)  // golden fOCR->DoOCRReleaseAndInspEnd(); offline no-op (INSTALL_OCR==eocrUninstal)

//  fLtcSensor->ClearLtcSensor exists in acarry_shims; no seam needed.

#endif // W7C1_SEAM

// ===========================================================================
//  DoCleanOutFinishCheck  -- golden csystem.cpp:14713-15748 (~1035 lines).
//  THE end-of-lot CLEAN OUT drain finish-check.  Translated FAITHFULLY this
//  wave (W7-C1): the iOneCycle/iCleanOut==1 entry guard, the iCleanOutCycleTask
//  cursor transitions, every grid-backed predicate read, the SOFT_SIMULTE
//  Fix3-cylinder block, and the config-gated customer/mode branches (SCKART
//  AutoRetest / OCR / SECS EventReport / AGV / Fix3 / Low-Yield / QA-Mode) are
//  reproduced VERBATIM.  Those branches are INERT offline (their guards
//  USE_AUTO_RETEST/bUseSCKART/bEnable_SECS_GEM/IniConfig.*/CUSTOMER_CODE are
//  false) but must compile+link -- the W7C1_SEAM block above supplies stand-ins
//  for symbols not yet in the tree; the serial Integrate swaps them for the real
//  symbols and removes the csystem_shims no-op stub (ODR).
//
//  NO-REGRESSION KEY: the body does REAL work only when (iCleanOut==1 &&
//  !iOneCycle).  The W6.6 hub never sets iCleanOut, so it sees NO behaviour
//  change; InitCleanOutFunction() sets iCleanOut=1 for the oracle to exercise
//  the residual-IC-detection / drain-to-finish path.
// ===========================================================================
void DoCleanOutFinishCheck()
{
    int iTrayXPos=0;
    int ret=0;
//    bool bRet=false;                                                            //kevin 20150717
    AnsiString S1, S2;

//    bool bNeedRetest=false;                                                     //ChungHung 20141002 add for KYEC AutoRetest
    if(iOneCycle)
    {
    }
    else if(iCleanOut==1)                                                       //Marc 2007/10/11 Start
    {
        if(IndexStatus==Z1_Z2_Normal)                                           //Steven 20240110 : 修正one cycle之後, socket上面遺留HAS_NULL_IC造成Hang up
        {
            if(InArmSuck.HasIC()==false &&
               IndexHasRealIC()==false &&                                       //Steven 20240217 : Fixed for one cycle / clean out hang up
               OutArmSuck.HasIC()==false &&
               FRCarryKit.UseSiteHasIC()==false &&
               BRCarryKit.UseSiteHasIC()==false &&
               AllArmZIsSafe())
            {
                if(TestSocket.HasRealIC()==false)
                {
                    TestSocket.ClearAll();
                }
            }
        }

        if(IniConfig.bP27AutoSortingBinTrayByOutArmwhenCleanOut==true &&        //JerryYang 20150917 整盤中，避免跳出OneCycleFinish
           bSortingAllBinTrayFinish==false &&
           CosFunction.bSortingBinTraywhenCleanOut)                             //JerryYang 20170914 (Steven) 避免clean out時hang up
        {
            iCleanOutCycleTask=1;
            return;
        }

        if(MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC() ||                    //KaiChen 20190423 ：增加 IC 檢查，避免 OutArm 有 IC 需要放 Fix3 時互卡
           InArmSuck.HasIC()     || OutArmSuck.HasIC()    ||
           ShuttleHasIC()        || IndexHasIC())
        {
            iCleanOutCycleTask=1001;
            return;
        }

        if(FIX3_FULL_PLACE==Fix3K_UseCylinder)
        {
            if(bShuttleMoveToLeftforFix3)                                       //Steven 20220526 : 修正Fix3氣缸要在One Cycle之前打回去
            {
                iCleanOutCycleTask=1002;
                return;
            }
            else if(Cylinder[C_FixTray_FullPlace].OffSensor()==false)           //KaiChen 20201119 ：修改CleanOut 異常
            {
                bShuttleMoveToLeftforFix3=false;
                Cylinder[C_FixTray_FullPlace].Off();
                iCleanOutCycleTask=1002;
                return;
            }
        }
        else if(FIX3_FULL_PLACE==Fix3K_UseStepperMotor &&
                Fix3MoveToLeft(false)==false)                                   //Jimmychiu 20240401 : FIX3回到右側
        {
            iCleanOutCycleTask=1003;
            return;
        }

        if(MOT[MMTrayZ].fHasTray      ||                                        //不能檢查MOT[MMTrayY].fHasTray，因為會跟快速CleanOut衝突
           MOT[MMPlate1].HasIC()      || MOT[MMPlate2].HasIC() ||               //kevin 20120217 MOT[MMPlate1].Tray.HasIC() || MOT[MMPlate2].Tray.HasIC() ||
           InArmSuck.HasIC()          || OutArmSuck.HasIC()         ||
           ShuttleHasIC()             || IndexHasIC()               ||
           Cylinder[C_FixTray_FullPlace].Status==true                           //ChungHung 20150520 modify fix Clean Out Fix3 full cylinder 未退回    //Steven 20180919 : Mark for fix full no goes back hang up
           )
        {
            iCleanOutCycleTask=2;
            return;
        }

        if(MOT[MInRotateKit].HasIC() || MOT[MOutRotateKit].HasIC())             //2013-04-17    Dell
        {
            iCleanOutCycleTask=3;
            return;
        }

        if(USE_AOI_Inspection)
        {
            if(AOIKit.HasIC())
            {
                iCleanOutCycleTask=4;
                return;
            }
        }

        iWhoTriggerPiggyBack=pbtCleanout;                                       //Steven 20111207 : 誰觸發了Piggy Back
        InitialPiggyBackFunction();                                             //Steven 20110725 : 重置Piggy Back的狀態
        bOneTimeWait=false;
        if(CheckIndexIsNormal()==false)
        {
            iCleanOutCycleTask=5;
            return;
        }

        if(AllArmZIsSafe()==false)                                              //Steven 20120314 : 要確保Z軸都在上面
        {
            iCleanOutCycleTask=6;
            return;
        }

        if(IniConfig.bD51UseOnecycleCleanOutFinishTestArmAtRear)
        {
            if(IndexStatus!=IndexIsBack)
            {
                iCleanOutCycleTask=7;
                return;
            }
        }
        else
        {
            if(IndexStatus!=Z1_Z2_Normal)
            {
                iCleanOutCycleTask=8;
                return;
            }
        }

        if(TrayForm.bAutoFeed || bMustCleanAllTray)
        {
            if(MOT[MMTrayY].fHasTray || MOT[MMTrayY_Car].fHasTray)
            {
                iCleanOutCycleTask=9;
                return ;
            }
        }

        if(TrayForm.bAutoFeed==false && bQAModeFinishCleanOut &&
           (MOT[MMTrayY].fHasTray || MOT[MMTrayY_Car].fHasTray) &&              //Steven 20111005 : QA Mode
           CosFunction.bQAModeUseUnloadCnt==false)                             //JerryYang 20221004 : Maxim版本QA mode
        {
            iCleanOutCycleTask=10;
            return;
        }

        if(MOT[MMAuto1_Car].fHasTray ||
           MOT[MMAuto2_Car].fHasTray ||
           MOT[MMAuto3_Car].fHasTray ||
           MOT[MMAuto4_Car].fHasTray ||
           MOT[MMAuto5_Car].fHasTray ||
           MOT[MMAuto6_Car].fHasTray)
        {
            iCleanOutCycleTask=11;
            return;
        }

        if((MOT[MMAuto1].fHasTray && MOT[MMAuto1].FullIC()) ||
           (MOT[MMAuto2].fHasTray && MOT[MMAuto2].FullIC()) ||
           (MOT[MMAuto3].fHasTray && MOT[MMAuto3].FullIC()) ||
           (MOT[MMAuto4].fHasTray && MOT[MMAuto4].FullIC()) ||
           (MOT[MMAuto5].fHasTray && MOT[MMAuto5].FullIC()) ||
           (MOT[MMAuto6].fHasTray && MOT[MMAuto6].FullIC()))
        {
            iCleanOutCycleTask=12;
            return;
        }

        if(MOT[MTrayX].fHasTray ||
           MTrayXCanSafeMove()==false)
        {
            iCleanOutCycleTask=13;
            return;
        }

        if(PitchCylinderState[0]!=0 ||
           PitchCylinderState[1]!=0 ||
           PitchCylinderState[2]!=0)
        {
            iCleanOutCycleTask=14;
            return;
        }
        MOT[MOutArmX].ScanMotorStatus();
        if(MOT[MOutArmX].Led[iInposLed])
        {
            iCleanOutCycleTask=15;
            return;
        }
        MOT[MOutArmY].ScanMotorStatus();
        if(MOT[MOutArmY].Led[iInposLed])
        {
            iCleanOutCycleTask=16;
            return;
        }

        if(IsOutArmSafe()==false)
        {
            iCleanOutCycleTask=17;
            return;
        }

        if(IniConfig.bEnableAutoCleanFunction &&
           TestIF.iAutoClean_Function==true &&                                  //pig 2011.09.01 AutoClean start
           TestIF_File.iAutoClean_Tray!=eCKPos_HP2 &&                           //kevin 20130509 沒有使用hotplate autoclean
           (TestIF.iAutoClean_Mode&M_FINISH) &&
           iCheckFinish_ByAutoClean==0)
        {
            InitialAutoCleanTask();
            InitialShuttleAutoCleanTask();
            InitialIndexAutoCleanTask();
            bRunAutoClean=true;
            hAutoCleanHangUp.SetSecAndOn(Prod.iHangupMaxTime);                  //Steven 20220702 : 針對Auto Clean的Hang Up偵測
            iCheckFinish_ByAutoClean=1;
        }

        if(iCheckFinish_ByAutoClean==1)
        {
            iCleanOutCycleTask=18;
            return;
        }

        if(InSHT1InLF()==false)
        {
            iCleanOutCycleTask=19;
            return;
        }

        if(IniConfig.bIndexArm2SupplyLight==false &&                            //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
           TestIF_File.bForEgisTecTest==false     &&                            //Steven 20140922 : Arm2當作指紋測試
           TestIF_File.bArm1PickPlaceArm2Test==false)                           //kevin 20150127 Arm1 下壓 arm2 測試
        {
            if(InSHT2InLF()==false)
            {
                iCleanOutCycleTask=20;
                return;
            }
        }

        if(iCatchTrayControlManual==2)                                          //Sam 20250926 : 修正 Hangup //kevin 20130723 add 避免CLEANOUT沒有放完AUTOTRAY HANG UP 尚有auto tray需要補tray
        {
           iCleanOutCycleTask=21;
           return;
        }

        #ifndef SOFT_SIMULTE
        if(FIX3_FULL_PLACE==Fix3K_UseCylinder &&                                //Ifor 20190516 :add KYEC 避免Fix3 Full功能導致無預警Hang up 新增Alarm
           Cylinder[C_FixTray_FullPlace].Pop()==false)                          //JerryYang 20180129 (Steven) 用Pop函式判斷，避免汽缸異常時無法跳出one cycle finish
        {
            iCleanOutCycleTask=22;
            return;
        }

        if(FIX3_FULL_PLACE==Fix3K_UseCylinder &&                                //Steven 20150914 : Fixed for FIX3_FULL_PLACE==2
           (Cylinder[C_FixTray_FullPlace].OffSensor()==false ||                 //Ifor 20210226 add:UseFix3Cylinder()未完成 導致 Shuttle鎖死Hang up
            bUseFix3CylinderActive==true))
        {
            if(bFix3_CleanOutTimeSet==false)                                    //避免汽缸移動時兩顆 Sensor Off時誤報Alarm
            {
                bFix3_CleanOutTimeSet=true;
                FixTrayAlarmCheck.SetSecAndOn(5);
            }

            if(FixTrayAlarmCheck.Off())
            {
                ShowMyMessage("Please Check the Cylinder 'C_FixTray_FullPlace'.", "請確認汽缸'C_FixTray_FullPlace'。");
                bFix3_CleanOutTimeSet=false;
            }
            iCleanOutCycleTask=23;
            return;
        }
        #endif
        bFix3_CleanOutTimeSet=false;

        if(FIX3_FULL_PLACE==Fix3K_UseCylinder &&                                //JerryYang 20220718 : 修正汽缸流程還沒把shuttle canMove的flag解開就跳one cycle finish, 造成後續hang up
           (iFix3CanFullTask==50 ||
            iFix3CanFullTask==51 ||
            iFix3CanFullTask==65))
        {
            iCleanOutCycleTask=28;
            return;
        }

        if(AUTO3_IS_MAGAZINE==1 && TestIF_File.iMagFixTrayType==1 &&
           (MOT[MManualTray1].Tray.HasRealIC() || MOT[MManualTray2].Tray.HasRealIC() || MOT[MManualTray3].Tray.HasRealIC() ||
            MOT[MManualTray4].Tray.HasRealIC() || MOT[MManualTray5].Tray.HasRealIC() || MOT[MManualTray6].Tray.HasRealIC()))                                    //JerryYang 20220909 : add magazine
        {
            iCleanOutCycleTask=29;
            return;
        }

        if(bDoLoaderCleanOut || bLoadBFBackTray)                                //KenHsieh 20230510 : 確認退Tray完成才CleanOut Finish
        {
            iCleanOutCycleTask=30;
            return;
        }
        bCleanoutStart=false;                                                   //kevin 20130506 cleanout已使用 但onecycle 在做完了繼續cleanout
        InitialTestHeadMotorTask();
        ResetHotPlateSearchParameter();
        for(int i=0; i<4; i++)
            Temperature.iATCCurrentFailCount[i]=0;                              //Steven 20151123 : Continue Fail Temp Offset for ATC

        ZeroMemory(bPickLoaderDuplicateErr, sizeof(bPickLoaderDuplicateErr));
        ZeroMemory(bPickHPDuplicateErr, sizeof(bPickHPDuplicateErr));
        ZeroMemory(bTryPickHPDuplicateErr, sizeof(bTryPickHPDuplicateErr));

        if(bResetModeAndCleanOut==true)                                         //JerryYang 20151007 START:ResetMode可選Onecycle或CleanOut
        {
            MyDBIProcess("Process", "Do reset finish.");                        //jou 2010-11-23
            InitialPiggyBackFunction();                                         //Steven 20110725 : 重置Piggy Back的狀態
            bResetModeAndCleanOut=false;
            bLampReset=false;
            if(bRestModeBackupParm)                                             //Sam 20250820 : [I49] 清料時 Contact Heigh 要拉高
            {
                bRestModeBackupParm=false;
                AnsiString S="";
                AnsiString szDir="";
                S=GetLastOpenFN();
                szDir=DataPath+S;
                szDir+="\\Contact.Data";
                WriteIniData(szDir, "Mode", "Contact",    iRestModeBackContactMode);
                WriteIniData(szDir, "Test Arm1", "Contact", dRestModeBackContactHeigh1);
                WriteIniData(szDir, "Test Arm2", "Contact", dRestModeBackContactHeigh2);
                W7C1_FCONTACT_READFILE();                                       // golden fContact->ReadFile(); (W7C1 seam: ReadFile absent on offline stub)
            }
        }

        ResetShuttleWhichKit();                                                 //Steven 20140710

        ResetInArmParam();
        MOT[MTestY1].MovFlag=false;
        MOT[MTestY2].MovFlag=false;
        MOT[MTestZ1].MovFlag=false;
        MOT[MTestZ2].MovFlag=false;
        InitInArmTask();
        InitOutArmTask();
        bBackupCleanOut=false;                                                  //jou 2010-12-07 : 防止index check oncyle與clean out funtion衝突
        iHotCount=0;

        PickFromHPList->ResetFile(sHPPickRec);                                  //Jimmychiu 20250220 : 避免重新開機後Hotplate rec有殘留資料
        PlaceToCleanList->ResetFile(sCleanPlaceRec);                            //Jimmychiu 20250220 : 避免重新開機後Hotplate rec有殘留資料

        for(int i=0; i<2; i++)
        {
            for(int j=0; j<50; j++)
            {
                for(int k=0; k<50; k++)
                {
                    iHotPlateCount[i][j][k]=-1;
                    iHotInArmOrder[i][j][k]=-1;
                    iHotWhichKit[i][j][k]=-1;
                    iHotWhichShuttle[i][j][k]=-1;
                }
            }
        }

        if(USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest &&         //Steven 20170322 (wei) : Fixed no ART handler alarm.
           CosFunction.bUseSCKART && W7C1_SCKART->iTesterType==0)               //Steven 20161201 (wei) : For SCK 93K ART   (W7C1 seam: fSCKART->iTesterType)
        {
            if(LastSet.iRunStartMode==rsmContinuRetest ||                       //Steven 20170513 (jou) : For SCK ART can do manual RT
               LastSet.iRunStartMode==rsmCInitialRetest)
            {
                W7C1_SCKART->SetLotStatus(W7C1_SCKART->iLOTSTATUS_L);           // golden fSCKART->SetLotStatus(fSCKART->iLOTSTATUS_L)
            }
            else if(W7C1_SCKART->iWaitGPIBLotR==0)                              // golden fSCKART->iWaitGPIBLotR
            {
                iSCKARTLoadingStatus=fSCKART->CheckLoadingCount();
                if(TestIF_File.bSCKART_LotDeviceCheck)
                {
                    if(iSCKARTLoadingStatus==2)
                    {
                        if(W7C1_SCKART->DoChkInputCntAlarm(true)==true)         //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定   (W7C1 seam: fSCKART->DoChkInputCntAlarm)
                        {
                            iCleanOutCycleTask=24;
                            ShowErrorMessage("WAR0120", K_RETRY, MInArmX, false, "DoCleanOutFinishCheck 1");
                            return;
                        }
                    }
                    else if(iSCKARTLoadingStatus==0)
                    {
                        if(W7C1_SCKART->DoChkInputCntAlarm(false)==true)        //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定
                        {
                            ret=ShowErrorMessage("WAR0119", K_RETRY|K_SKIP, MInArmX, false, "DoCleanOutFinishCheck 1");
                            if(ret==K_RETRY)
                            {
                                iCleanOut=0;
                                bLoaderNoTrayAutoCleanOut=false;                //Steven 20170320 (wei) : fixed for check loader tray again
                            }
                            else
                            {
                            }
                            iCleanOutCycleTask=25;
                            return;
                        }
                    }
                }

                if(W7C1_SCKART->iInputJamCnt!=0 || W7C1_SCKART->iOutputJamCnt!=0) //Steven 20170320 (wei) : Must after check load count  (W7C1 seam: fSCKART->iInputJamCnt/iOutputJamCnt)
                {
                    ret=ShowErrorMessage("WAR0121", K_RETRY|K_SKIP, MMSystem, false, "DoCleanOutFinishCheck 1");
                    if(ret==K_RETRY)
                    {
                        iCleanOut=0;
                        bLoaderNoTrayAutoCleanOut=false;                        //Steven 20170320 : fixed for check loader tray again
                        iCleanOutCycleTask=26;
                        return;
                    }
                    else
                    {
                        iCleanOutCycleTask=27;
                        return;                                                 //Steven 20170331 (wei) : Add for re-check count
                    }
                }
                W7C1_SCKART->SetLotStatus(W7C1_SCKART->iLOTSTATUS_L);           //Steven 20161025 (wei) : SCK ART function
                W7C1_SCKART->iCurrentFlexARTStep=6;                             // golden fSCKART->iCurrentFlexARTStep
                W7C1_SCKART->iWaitGPIBLotR=1;                                   // golden fSCKART->iWaitGPIBLotR
            }
        }

        if(IniConfig.bE61InArmStandbyPosOnLoader)                               //JerryYang 20200206 one cycle, clean out, tray feed時in arm移動到loader
        {
            bOneCycleInArmToLoader=true;

            if(MoveInArm2XYToLoaderWait())
            {
                bOneCycleInArmToLoader=false;
            }
            else
            {
                iCleanOutCycleTask=31;
                return;
            }
        }

        iCleanOutCycleTask=0;
        HPPlaceLog.InitialPosition();                                           //Steven 20211110 : 記錄放料到加熱盤的位置, initial start的時候要清空
        if((CosFunction.bAutoRetestGPIBmode==true) &&
          (LastSet.iRunStartMode==rsmInitial_ART ||
           LastSet.iRunStartMode==rsmContinuStart_ART ||
           LastSet.iRunStartMode==rsmContinuRetest_ART ||
           IniConfig.bA37LotStartLotEnd) &&
          (W7C1_LS_bWaitStartLotAutoRetestGPIB==false &&                        // golden LastSet.bWaitStartLotAutoRetestGPIB (W7C1 seam)
           TestIF_File.bSCKART_RunARTWithoutCmd==false))
        {
        }
        else
        {
            if(IniConfig.bEnable_SECS_GEM==true)                                //Steven 20140528 : Secs Gem
                W7C1_EVENTREPORT_CLEANOUTFINISH();                              //42     Clean Out Finish  (W7C1 seam: SECS_EVENT.CleanOutFinish absent)
        }

      #ifdef DEBUG_DUTONOFF
        if(fAutomation->bCleanOut)                                              //Steven 20110902 : 自動找蟲蟲模式
        {
            AnsiString Data[2]={0, 0};
            fAutomation->CommandProcess("CLEANOUT_REPLY", 1, Data);
            fAutomation->bCleanOut=false;
            fMain->Pause("CLEANOUT_REPLY");
        }
        else
      #endif
        {
            if((TrayForm.bAutoFeed && LastSet.bCleanOut_ART==false &&           //ChungHung 20141002 add for KYEC AutoRetest
               (CosFunction.bUseSCKART==false || TestIF_File.bSCKART_EnableART==false ||
               (USE_COVER_TRAYID!=tCIDNotUse &&
                W7C1_FAGV_ISSPIL()==true))) ||                                  // golden fAGV->IsSPIL_AMR() (W7C1 seam)
                LastSet.iRunStartMode==rsmAutoSiteMap)                          //Sam 20200210 : ART Function Off 時 Auto Tray Feed Issue Fix
            {
                if(bTrySuckHotPlateCleanOut==true &&                            //ChungHung 20120206 Hotplate check
                   LastSet.iTemperature==Tempture_Hot)
                {
                    tUPH_PauseTime=0;
                    iTrayFeed=1;
                    iTrayFeedTask=1;
                    bTrySuckHotPlateCleanOut=false;
                    bHotPlateCheckNeedTrayFeed=false;
                    if(IniConfig.bOneCycleDoQuickCleanOut==true)                //Steven 20110524
                    {
                        TrayForm.bAutoFeed=bNeedQuickCleanOut;
                        bNeedQuickCleanOut=true;
                    }
                    W7C1_FMAIN_START("DoCleanOutFinishCheck 1");                // golden fMain->Start("...") (W7C1 seam)
                }
                else
                {
                    if(IniConfig.bE39CheckHotPlateAfterCleanOutAndBeforeTrayFeed &&
                       TrayForm.bAutoFeed &&
                       LastSet.iTemperature==Tempture_Hot &&
                       bCanUseHotPlateCheck)                                    //ChungHung 20120206 Hotplate check
                    {                                                           //ChungHung 20120206 Hotplate check  是否開啟Tray Feed後 HotPlate Check
                        bNeedTrySuckHotPlate=true;                              //ChungHung 20120206 Hotplate check
                        bOneTimeHotPlateCheckAll=true;                          //ChungHung 20120206 Hotplate check
                        bTryPickFromHotPlateShowError=false;                    //ChungHung 20120206 Hotplate check
                        bHotPlateCheckNeedTrayFeed=true;

                        W7C1_FMAIN_START("DoCleanOutFinishCheck 2");            // golden fMain->Start("...")
                    }
                    else
                    {
                        tUPH_PauseTime=0;
                        SetInitialICCheck();
                        iTrayFeed=1;
                        iTrayFeedTask=1;
                        if(IniConfig.bOneCycleDoQuickCleanOut==true)            //Steven 20110524
                        {
                            TrayForm.bAutoFeed=bNeedQuickCleanOut;
                            bNeedQuickCleanOut=true;
                        }
                    }
                }
                W7C1_FBARCODE_CHANGE2D();                                       //Steven 20160425 : 重置2DSys時間  (W7C1 seam: fBarCode->Change2DSetupFile)
                W7C1_FTRAYMAP_CHANGETRAY();                                     //wei 20161219 Tray Mapping  (W7C1 seam: fTrayMapping->ChangeTraySetupFile)
            }
            else
            {
                if(bQAModeQuickCleanOut==true && bQAModeFinishCleanOut==false)  //Steven 20111005 : QA Mode
                {
                    if(Prod.iQAModeRunType==1)                                  //Steven 20121016 : QA Mode
                    {
                        ret=ShowErrorMessage("MES1648", K_RETRY|K_SKIP|K_CLEAN_OUT|K_TRAY_FEED|K_TRAY_END, MMSystem, false);                                    //QA Mode finish
                        SetRunStartMode(rsmContinuStart);

                        IniConfig.bQAModeFirstIn    =true;
                        LastSet.iTester             =IniConfig.iBackUpTesterMode;

                        if(LastSet.iTester==ON_LINE)                            //Steven 20150713 : 整理LastSet.iTester
                            NewRecordProcess("MES2157", "Change to On_Line", "by CleanOut QAModeQuickCleanOut and QAModeFinishCleanOut  QAMode");               //ChungHung 20140722 add add record
                        else
                            NewRecordProcess("MES2155", "Change to Off_Line", "by CleanOut QAModeQuickCleanOut and QAModeFinishCleanOut QAMode");               //ChungHung 20140722 add add record

                        ArmSpeed[InArm].bVariModeFIX=IniConfig.bBackUpInArmMode;
                        TrayForm.bAutoFeed          =IniConfig.bBackUpAutoFeed;
                        bQAModeFinishCleanOut       =false;
                        bQAModeQuickCleanOut=false;

                        if(ret==K_TRAY_FEED)
                        {
                            tUPH_PauseTime=0;
                            iTrayFeed=1;
                            iTrayFeedTask=1;
                            bCleanOutTrayEnd=false;
                        }
                        else if(ret==K_TRAY_END)
                        {
                            tUPH_PauseTime=0;
                            iTrayFeed=1;
                            iTrayFeedTask=1;
                            bCleanOutTrayEnd=true;
                        }
                        else if(ret==K_CLEAN_OUT)
                        {
                            fMain->CleanOut("DoCleanOutFinishCheck");
                        }
                    }
                    else
                    {
                        bQAModeFinishCleanOut=true;                             //Sam 20200804 : QA 模式完成後切回 OffLine 時，強制顯示 OnLine 圖片。換位置
//                        LastSet.iTester=OFF_LINE;                             //Steven 20150713 : 整理LastSet.iTester
                        W7C1_FMAIN_MODIFYTESTER(OFF_LINE);                      //Steven 20191218 : 整合修改LastSet.iTester   (W7C1 seam: fMain->ModifyTester)
                        NewRecordProcess("MES2155", "Change to Off_Line", "by CleanOut QAModeQuickCleanOut and QAModeFinishCleanOut  QAMode1");                 //ChungHung 20140722 add add record

                        ArmSpeed[InArm].bVariModeFIX=IniConfig.bBackUpInArmMode;

                        bNeedTrySuckHotPlate=false;                             //ChungHung 20120206 Hotplate check
                        bQAModeQuickCleanOut=false;

                        if(LastSet.iTemperature==Tempture_Hot)
                            bMustCleanAllTray=true;

                        if(iCleanOut==0)
                            InitCleanOutFunction();
                    }
                }
                else                                                            //Steven 20120130 : QA完成後的CleanOut
                {
                    if(bTrySuckHotPlateCleanOut==true &&                        //ChungHung 20120206 Hotplate check
                       LastSet.iTemperature==Tempture_Hot)
                    {
                        if(bHotPlateCheckNeedTrayFeed==true)
                        {
                            iTrayFeed=1;
                            iTrayFeedTask=1;
                        }
                        bOneTimeHotPlateCheckAll=false;
                        bHotPlateCheckNeedTrayFeed=false;
                        tUPH_PauseTime=0;
                        bTrySuckHotPlateCleanOut=false;
                        bNeedTrySuckHotPlate=false;                             //ChungHung 20120206 Hotplate check
                        W7C1_FMAIN_START("DoCleanOutFinishCheck 3");            // golden fMain->Start("...")
                    }
                    else
                    {
                                                                                //RogerYang 20251217 : 移到下面，避免ART開啟時一直被重置
//                        if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum)          //Steven 20120222 : Real Time CCD 新增One cycle完成時送出@END+動作來停止檢測動作, 要在Alarm之前,給OA喘息的機會
//                        {
//                            COM2->DoReleaseAndInspEnd();
//                        }
//
//                        if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction)    //Steven 20120716 : OCR
//                        {
//                            fOCR->DoOCRReleaseAndInspEnd();
//                        }

                        if(USE_AUTO_RETEST==eartInstall &&
                           (IniConfig.bA10_AutoReTest || bAutoReTest_ART) &&
                           (LastSet.iRunStartMode==rsmInitial_ART       ||
                            LastSet.iRunStartMode==rsmContinuStart_ART  ||
                            LastSet.iRunStartMode==rsmContinuRetest_ART))       //kevin 20150616
                        {
                            if(DoART_AfterCleanOut(ret)==false)
                            {
                                return;
                            }
//                            else
//                            {
//                                ret=K_TRAY_FEED;
//                            }
                        }
                        else if(IniConfig.bCleanOutCanTrayEnd)
                        {
                            if(bQAModeFinishCleanOut)
                            {
                                if(CosFunction.bQAModeUseUnloadCnt)             //JerryYang 20221004 : Maxim版本QA mode
                                {
                                    ret=ShowErrorMessage("MES1642", K_TRAY_END, MMSystem, false);
                                }
                                else
                                {
                                    ret=ShowErrorMessage("MES1642", K_TRAY_FEED|K_TRAY_END, MMSystem, false);
                                }

                                if(ret==K_TRAY_END)                             //JerryYang 20160622 Tray End一定秒數後停止錄影
                                    bCleanOutClosedMonitorVideo=true;
                            }
                            else
                            {
                                if(IniConfig.bA08LastLoaderAutoCleanOutAndCheckAgain &&
                                   bLoaderNoTrayAutoCleanOut==true &&           //ChungHung 20140512 合code漏掉 補上 bLoaderNoTrayAutoCleanOut==true
                                   bSecondTimeLoaderCheckNoTray==false)         //ChungHung 20130528 SCK要求AutoClean後要自動檢測是否Loader有補Tray
                                {
                                    ret=K_RETRY;
                                    W7C1_FMAIN_START("DoCleanOutFinishCheck 4"); // golden fMain->Start("...")
                                    bSecondTimeLoaderCheckNoTray=true;
                                }

                                if(IniConfig.bE53LowYieldAutoClean &&           //kevin 20160802
                                    bLowYieldCleanOut)                          //wei 20141201 Low Yield Auto Clean(%) start
                                {
                                    ret=K_RETRY;

                                    for(int i=0; i<TestSocket.iShtRow; i++)
                                    {
                                        for(int j=0; j<TestSocket.iShtCol; j++)
                                        {
                                            if(bLowYieldCloseSite[0][i][j]==true)
                                            {
                                                LastSet.bUseTestSocket[0][i][j]=false;
                                                bTestSiteUse[0][i][j]=false;
                                                LastSet.bUseTestSocket[1][i][j]=false;
                                                bTestSiteUse[1][i][j]=false;
                                                iLowYieldCloseCount++;
                                                iLowYieldSiteCount[i][j]=0;
                                                fContactCT->rgYieldType->ItemIndex=3;
                                                fContactCT->ClearData(i, j);    //Steven 20220301 : 修正Clear Yield
                                            }
                                        }
                                    }
                                    fMain->ShowTestHeadComp(false);

                                    if(iLowYieldCloseCount>=(TestSocket.iShtCnt)/4)
                                    {
                                        ShowMyMessage("Low Yield close sites over set Site","低良率關Site數超過設定數量");
                                        iLowYieldCloseCount=0;
                                        fMain->Pause("Low Yield close sites 1");
                                    }
                                    else
                                    {
                                        W7C1_FMAIN_START("DoCleanOutFinishCheck 5"); // golden fMain->Start("...")
                                    }

                                    bLowYieldCleanOut=false;
                                    bLowYieldAutoCleanEnd=false;                //wei 20141216  LowYieldautoclean  避免重複進去
                                }
                                else
                                {
                                    ret=ShowErrorMessage("MES1642", K_TRAY_FEED|K_RETRY|K_TRAY_END, MMSystem, false);

                                    if(ret==K_RETRY && bSecondTimeLoaderCheckNoTray==true)
                                    {
                                        bSecondTimeLoaderCheckNoTray=true;
                                    }
                                    else
                                    {
                                        bSecondTimeLoaderCheckNoTray=false;
                                    }

                                    if(ret==K_TRAY_END)                         //JerryYang 20160622 Tray End一定秒數後停止錄影
                                    {
                                        bCleanOutClosedMonitorVideo=true;
                                    }
                                    else if(ret==K_TRAY_FEED)                   //kevin 20210104 add
                                    {
                                        NewRecordProcess("MES2118", "TRAY FEED pressed", "DoCleanOutFinishCheck 2");
                                    }
                                }
                            }
                        }
                        else if(bASECleanOutCloseSite)                          //kevin 20160715 claen out 自動關site系統
                        {
                            bASECleanOutCloseSite=false;
                            ReadTestMode();
                            fMain->ShowTestHeadComp(false);
                            ASESendMessage->SendToASEData("@e02005Done");       //kevin 20160715 回應 ase close Site
                        }
                        else
                        {
                            if(IniConfig.bE53LowYieldAutoClean &&               //kevin 20160802
                               bLowYieldCleanOut)                               //wei 20141201 Low Yield Auto Clean(%) start
                            {
                                ret=K_RETRY;

                                for(int i=0; i<TestSocket.iShtRow; i++)
                                {
                                    for(int j=0; j<TestSocket.iShtCol; j++)     //close Site
                                    {
                                        if(bLowYieldCloseSite[0][i][j]==true)
                                        {
                                            LastSet.bUseTestSocket[0][i][j]=false;
                                            bTestSiteUse[0][i][j]=false;
                                            iLowYieldCloseCount++;
                                            iLowYieldSiteCount[i][j]=0;

                                            fContactCT->rgYieldType->ItemIndex=3;
                                            fContactCT->ClearData(i, j);        //Steven 20220301 : 修正Clear Yield
                                        }
                                    }
                                }
                                fMain->ShowTestHeadComp(false);

                                if(iLowYieldCloseCount>=(TestSocket.iShtCnt)/4)
                                {
                                    ShowMyMessage("Low Yield close sites over set Site","低良率關Site數超過設定數量");
                                    iLowYieldCloseCount=0;
                                    fMain->Pause("Low Yield close sites 2");
                                }
                                else
                                {
                                    W7C1_FMAIN_START("DoCleanOutFinishCheck 6"); // golden fMain->Start("...")
                                }

                                bLowYieldCleanOut=false;
                                bLowYieldAutoCleanEnd=false;                    //wei 20141216  LowYieldautoclean  避免重複進去
                            }
                            else
                            {
                                if(bQAModeFinishCleanOut)
                                {
                                    ret=ShowErrorMessage("MES1642", K_TRAY_FEED, MMSystem, false);
                                }
                                else if(CosFunction.bSpecailLowYeild &&
                                        Prod.bFailAlarmLowYieldSpecial &&
                                        bLowYeildAlarmSpecial)                  //Sam 20210505 : PTI 要求的兩段 Low Yeild
                                {
                                    DoLowYieldAlarm("WAR0724", "");             //第二段 Low Yield 直接報警
                                    fCounterClear->LowYieldSpecialInitail();
                                }
                                else
                                {
                                    if(IniConfig.bSPILFunction==true)
                                    {
                                    }
                                    else
                                    {
                                        W7C1_SCKART->SaveTestSummary(1);        //Steven 20190521 : ATK lot count   (W7C1 seam: fSCKART->SaveTestSummary)
                                    }
                                    ret=ShowErrorMessage("MES1642", K_TRAY_FEED|K_RETRY, MMSystem, false);
                                }

                                if(TrayForm.bEnableAMR)                         //Eastsun 20260519 整合
                                {
                                    AnsiString asstr[3];
                                    if(TrayForm.bEnableAMRLoader==false)
                                    {
                                        asstr[0]="P1:0,P2:0,P3:0,P4:1,P5:0,P6:0";
                                        asstr[1]="P1:0,P2:0,P3:0,P4:0,P5:1,P6:0";
                                        asstr[2]="P1:0,P2:0,P3:0,P4:0,P5:0,P6:1";

                                        for(int i=0; i<3; i++)
                                        {
                                            if(iUnloaderTrayCountCal[i]>0)
                                            {
                                                    //TestIF_File.iAMRTrayCount[i+3]++;                 //Eastsun20260416 : 註解調
                                                asSupplementBin=asstr[i];
                                                bUnLoaderActionFlag[i]=true;
                                                iUnloaderTrayCountCal[i]=0;

                                                //TestIF_File.asAMRBinSetting[i]=AMRUnloadBin(i);
                                                //==>
                                                TestIF_File.asAMRBinSetting[i]=AMRUnloadBin(i+1);   //Eastsun20260123 : 算錯
                                                TestIF_File.iAMRTrayCount[i+3]-=iAMRCoverTray;

                                                W7C1_EVENTREPORT_AGVSUPPLEMENT(); // golden EventReport(SECS_EVENT.AGVSupplement) (W7C1 seam)
                                                asSupplementBin="";
                                                TestIF_File.iAMRTrayCount[i+3]=0;
                                                TestIF_File.iAMRDeviceCount[i+3]=0;
                                                    TestIF_File.asAMRBinSetting[i]="";
                                            }
                                        }

                                        for(int i=0; i<14; i++)
                                        {
                                            TestIF_File.iAMRMagzineDeviceCount[i]=0;
                                        }
                                    }

                                    bNeedCoverTray[0]=false;
                                    bNeedCoverTray[1]=false;
                                    bNeedCoverTray[2]=false;
                                    RunInfo.bLotStart=false;
                                    iLoaderTrayCountCal=0;
                                    iLoaderTrayCountAMRCal=0;
                                    W7C1_LS_iLoaderTotalTray=0;                 // golden LastSet.iLoaderTotalTray (W7C1 seam)
                                    W7C1_LS_iLoaderTrayCount_ART=0;             // golden LastSet.iLoaderTrayCount_ART (W7C1 seam)
                                    TestIF_File.iAMRLDNowTrayCount=0;
                                    W7C1_LOTINFO_LABLOTTRAY.Caption="0";        // golden fLotInfo->labLotTrayCount->Caption (W7C1 seam)
                                    W7C1_LOTINFO_LABDIFFTRAY.Caption="0";       // golden fLotInfo->LabDiffTrayCount->Caption (W7C1 seam)
                                    W7C1_LOTINFO_LABNOWTRAY.Caption="0";        // golden fLotInfo->labNowTrayCount->Caption (W7C1 seam)
                                    bAMRReceiveLoaderTotalTray=false;
                                    bAMRReceiveAGVStart=false;
                                    bAMRReceiveStart=false;
                                }
                            }
                        }
                                                                                //RogerYang 20251217 : 移到下面，避免ART開啟時一直被重置
                        if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum)          //Steven 20120222 : Real Time CCD 新增One cycle完成時送出@END+動作來停止檢測動作, 要在Alarm之前,給OA喘息的機會
                        {
                            COM2->DoReleaseAndInspEnd();
                        }

                        if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction)    //Steven 20120716 : OCR
                        {
                            W7C1_FOCR_RELEASEINSPEND();                         // golden fOCR->DoOCRReleaseAndInspEnd(); (W7C1 seam: fOCR absent)
                        }

                        if(LastSet.iRealDummy==REALLY           &&
//                           LastSet.iTesterMode==ON_LINE         &&
                           TestIF_File.bEnableBarCode==true     &&
                           CosFunction.bBarcodeTrayRecFile==true )              //jou 20190930 : Barcode Tray record file
                        {
                            if(iRunStartMode==FT)
                                W7C1_LS_iBarcodeTrayRecFileRetestCT=0;          // golden LastSet.iBarcodeTrayRecFileRetestCT (W7C1 seam)
                            else if(iRunStartMode==RT)
                                W7C1_LS_iBarcodeTrayRecFileRetestCT++;
                        }

                        if(iAseCleanOut!=0)                                     //kevin 20150925
                        {
                            iAseCleanOut=0;
                            RespondASECom("@e02107Done");                       //kevin 20150415 回應 ase CleanOut finish
                        }

                        bReOpenGpib=true;                                       //Steven 20101013
                        bInitialMaxTime=true;                                   //jou 2011-11-09 增加initial max time set

                        if(CosFunction.bPiggyBackShowMainForm)                  //Steven 20131101 : PiggyBack數量到達時,顯示在Main Form上面
                        {
                            bContinuoussPass=true;
                            bContactCounOven=true;
                            W7C1_FMAIN_CLEANYIELD();                            //kevin 20131009 清除yield 計數   (W7C1 seam: fMain->CleanYieldCount)
                        }

                        if(ret==K_TRAY_FEED)
                        {
                            NewRecordProcess("MES2118", "TRAY FEED pressed", "DoCleanOutFinishCheck 3");

                            if(bQAModeFinishCleanOut)                           //Steven 20120407
                            {
                                bNeedTrySuckHotPlate=false;                     //ChungHung 20120206 Hotplate check
                                tUPH_PauseTime=0;
                                iTrayFeed=1;
                                iTrayFeedTask=1;
                                bMustCleanAllTray=false;
                            }
                            else if(LastSet.iTemperature==Tempture_Hot &&
                                    IniConfig.bE39CheckHotPlateAfterCleanOutAndBeforeTrayFeed &&
                                    bCanUseHotPlateCheck)                       //ChungHung 20120206 Hotplate check  是否開啟Tray Feed後 HotPlate Check
                            {
                                bNeedTrySuckHotPlate=true;                      //ChungHung 20120206 Hotplate check
                                bOneTimeHotPlateCheckAll=true;                  //ChungHung 20120206 Hotplate check
                                bTryPickFromHotPlateShowError=false;            //ChungHung 20120206 Hotplate check
                                bHotPlateCheckNeedTrayFeed=true;
                            }
                            else
                            {
                                bNeedTrySuckHotPlate=false;                     //ChungHung 20120206 Hotplate check
                                tUPH_PauseTime=0;
                                iTrayFeed=1;
                                iTrayFeedTask=1;
                            }
                        }
                        else
                        {
                            if((IniConfig.bCleanOutCanTrayEnd ||
                                (CUSTOMER_CODE==CC_ASE_KaohSiung &&
                                 USE_AUTO_RETEST==eartInstall &&
                                 bAutoReTest_ART)) &&
                                ret==K_TRAY_END)                                //kevin 20150716
                            {
                                tUPH_PauseTime=0;
                                iTrayFeed=1;
                                iTrayFeedTask=1;
                                bCleanOutTrayEnd=true;
                                NewRecordProcess("MES2119", "TRAY FEED pressed", "DoCleanOutFinishCheck 3");
                            }
                            else
                            {
                                bCleanOutTrayEnd=false;                         //kevin 20150717
                            }
                        }
                    }
                }
                W7C1_FMAIN_CHANGELEVEL();                                       // golden fMain->ChangeLevelAttr() (W7C1 seam)
                SetInitialICCheck();

                bIsShowPMAlarmMessage=true;                                     //wei 20160225 PMAlarmFunction

                if(bBackMMTrayY==true && MOT[MMTrayY].Tray.HasIC())
                {
                    MOT[MMTrayY].fHasTray=true;
                }

                if(bBackMMTrayY_Car==true && MOT[MMTrayY_Car].Tray.HasIC())
                {
                    MOT[MMTrayY_Car].fHasTray=true;
                }

                bBackMMTrayY_Car=false;
                bBackMMTrayY=false;

                bLoaderNoTrayAutoCleanOut=false;                                //jou 2010-09-01 Loader沒有tray時,Loader不再進Tray,Tran End時自動clean out

                if(IniConfig.bA08LastLoaderAutoCleanOutAndCheckAgain &&         //ChungHung 20130910 add for SCK
                   bLoaderNoTrayAutoCleanOut==false &&
                   bSecondTimeLoaderCheckNoTray==true)
                {
                    RecordAutoCleanOutStartEnd(false);                          //ChungHung 20130910 add for SCK
                }
            }
            iCleanOut=0;                                                        //wei 20160421 ART一定要在最下面
        }
        W7C1_FBARCODE_CHANGE2D();                                               //Steven 20160425 : 重置2DSys時間  (W7C1 seam: fBarCode->Change2DSetupFile)
        W7C1_FTRAYMAP_CHANGETRAY();                                             //wei 20161219 Tray Mapping  (W7C1 seam: fTrayMapping->ChangeTraySetupFile)

        if(CosFunction.bAutoCleanShuttleDisable==true)
        {
            if(CUSTOMER_CODE==CC_KYEC_LEE)                                      //wei 20151221 KYEC 清除UPH紀錄
            {
                for(int i=1; i<=12; i++)                                        //將row[i+1]的值放到row[i]
                {
                    fShowBinSelect->UPH_StringGrid[0][i]="";
                    fShowBinSelect->UPH_StringGrid[1][i]="";
                    fShowBinSelect->UPH_StringGrid[2][i]="";
                    fShowBinSelect->UPH_StringGrid[3][i]="";
                }
                bOneTimes=true;
            }
        }

        if(IniConfig.bSIGURDFunction ||                                         //KaiChen 20190910 ：加入 SIGURD_PeiXing、SIGURD_ChungXing
           CUSTOMER_CODE==CC_FMSH)                                              //Steven 20220722 : 復旦微要求Tray Feed之後切成initial start
        {
            bTrayFeedAfterCleanOut=true;
        }
        else
        {
            bTrayFeedAfterCleanOut=false;
        }

        SetInArmSpeed(false, true);                                             //Steven 20140909 : Clean Out後要重置Speed
        SetOutArmSpeed(false);
        if(USE_OUT_SORT_ARM!=eartUninstall)                                     //RogerYang 20250515 add for 9046AU
            SetSortArmSpeed(false);

//        if(CUSTOMER_CODE==CC_PTI || CUSTOMER_CODE==CC_MTI)                      //Sam 20200226 : OLP 增加 LotStar & LotEnd 控制
//        {
//            if(USE_AUTO_RETEST==eartInstall && (IniConfig.bA10_AutoReTest || bAutoReTest_ART) &&    //Sam 20240809 : PTI ART 模式
//                (LastSet.iRunStartMode==rsmInitial_ART       ||
//                 LastSet.iRunStartMode==rsmContinuStart_ART  ||
//                 LastSet.iRunStartMode==rsmContinuRetest_ART ||
//                 LastSet.iRunStartMode==rsmAutoRetest))
//            {
//                                                                                //跑 ART 的時候不要 LotEnd
//            }
//            else
//            {
//                if(iOlpLotStatus==1)
//                {
//                    fLotInfo->sbSECSLotEnd->Tag=1;
//                    fLotInfo->sbSECSLotEndClick(fLotInfo->sbSECSLotEnd);
//                    fLotInfo->sbSECSLotEnd->Tag=0;
//                    RecordProcess("OLP End Lot");
//                    if(RunInfo.bLotStart==false)
//                    {
//                        fLotInfo->sbSECSLotStart->Down=false;
//                        fLotInfo->sbSECSLotEnd->Down=true;
//                        iOlpLotStatus=0;
//                        fMain->SetLotState(10);                                 //PTI ART Lot Start
//                    }
//                }
//            }
//        }

        VerifyNeedDoAlignment(AutoAlignmentTray_AfterOneCycle, AutoAlignmentCK_AfterOneCycle);                          //KenHsieh 20210813 : add CCD AUTO ALIGNMENT

        W7C1_FLTCSENSOR_CLEAR(1);                                               //Sam 20221101 : Latch 清除都要確認是否清清乾淨  (W7C1 seam: fLtcSensor not included in this TU)
        W7C1_FLTCSENSOR_CLEAR(0);                                               //Sam 20221101 : Latch 清除都要確認是否清清乾淨

        iCheckFinish_ByAutoClean=0;                                             //kevin 20130226   //Steven 20140725 : 換位置
        bTT_SetSpeed_Check=false;                                               //Ztex 2024.08.11 Add Test Time Set Speed
        bUseSocketHeating_Wait=false;                                           //Ztex 2024.09.07 Add Use Socket Heating
        iTrayXPos=MOT[MTrayX].ReadPos();
        if(iTrayXPos<Prod.iXTrayEmpty-50 && iTrayXPos>Prod.iXTrayColor+50)
        {
            iCleanOutCycleTask=35;
            return;
        }
    }
    else
    {
        if(W7C1_FMAIN_BTNONECYCLE_DOWN &&                                       //ChungHung 20150514 add for 12 site full shuttle //ChungHung 20150213 add fix 2x6 if open munt full issue hangup  (W7C1 seam: fMain->BtnOneCycle->Down)
           iOneCycle==0 &&
           bOneCycle_BackUp==false)                                            //JerryYang 20161129 iOneCycle_BackUp改成bool
        {
            fMain->DebugOneCycleHotPlate("DoCleanOutFinishCheck");              //Sam 20210915 : 增加 OneCycle Hotpalte Debug Log
            iOneCycle=1;
        }
    }
}
