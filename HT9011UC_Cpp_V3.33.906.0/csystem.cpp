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
