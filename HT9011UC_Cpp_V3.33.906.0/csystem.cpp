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
//                            FULLY ACTIVE (the AutoSiteMap branch of InitCleanOut,
//                            golden :15751-15785, was un-gated W906-AutoSiteMapCleanOut
//                            20260727 -- fMain->SetMainRunStartMode is a documented
//                            GAP no-op stub, see FormsFacade.h/.cpp).
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
#include "atester_shims.h"           // InitBTestSuckTestICTask
// AI(W5-Automation-Integrate) 20260710: InitFrontTestSuckICTask is now declared
// in aTester_Front.h for real (removed from atester_shims.h -- see that file).
#include "aTester_Front.h"
// AI(W5-Automation-Integrate) 20260710: InitialPiggyBackFunction/
// ProcessPiggyBackFunction/DoLowYieldAlarm (atester_ProcessCount.h) and
// iTestTwoArm32SiteTask (atester_32Site.h) are now declared in their own real
// headers (removed from atester_shims.h -- see that file).
#include "atester_ProcessCount.h"
#include "atester_32Site.h"
#include "acatchtray_shims.h"        // DoAutoReceiveBinTray / DoAutoColor
#include "canary_support.h"          // LastSet / RecordProcess / REALLY (SetInitialICCheck / InitOneCycle)
#include "FormsFacade.h"             // fMain (DoAllProcess :9130 ProcessSensorScan; gated branches)
#include "SECSGEM/SecsEventType.h"   // SECS_EVENT (ETypeStruct) -- CleanOutFinish/AGVSupplement/OneCycleFinish/ArtFTFinish/ArtRTFinish
#include "SECSGEM/SecsEventReport.h" // EventReport(unsigned) -- Sim-first entry point (W5-comms INTEGRATE)
#include "Automation/SCK_ART.h"      // SckArtState / SckArt_SetLotStatus / SckArt_DoChkInputCntAlarm / SckArt_CheckNeedRT / SckArt_DoAutoSocketOff (W7C1/W7C2 SCKART seams)
#include "Automation/HANA_ART.h"     // uHANA_ART (W7C2 Hana seam)

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
//  InitCleanOutFunction  -- golden csystem.cpp:15749.  FULLY ACTIVE (both the
//  AutoSiteMap branch, golden :15751-15785, and the tail iHome/iReset/
//  iCleanOut/iTrayFeed/bCleanoutStart resets).
//  AI(W906-AutoSiteMapCleanOut) 20260727: un-gated the AutoSiteMap branch --
//  it was the last remaining piece of Wave16 (siblings DoHotplateEdgeCylinderLoop
//  + DoLoaderVibrateLoop landed 20260722, commit 6f60737). Every symbol it
//  touches already had a real translated home in this tree EXCEPT
//  fMain->SetMainRunStartMode(int), which did not exist -- added as a
//  documented GAP no-op stub (FormsFacade.h/.cpp; golden's real body cascades
//  into UpdateMainOperateMode() + a new fBinSel VCL form, both out of scope
//  this wave -- see the stub's banner comment for the full citation). The
//  branch body itself is translated VERBATIM from golden (braces around the
//  single-statement if/else at :15762-15769 dropped; no logic changed).
// ===========================================================================
void InitCleanOutFunction()
{
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
//  20323-20388 / 20391-20432 (declared in the frozen csystem.h).  FAITHFUL
//  line-for-line translation -- names, formulas, branches and Chinese
//  comments preserved verbatim, including the two file-scope TQPF_Timer
//  globals at golden :20321-20322 / :20390.
// AI(ht9045-v906) 20260722: un-gated both tail cylinder/vibration loops --
//  ZERO-BLOCKED per recon (Cylinder[]/SW[]/MOT[]/IniConfig/LastSet/
//  Ld_UldDelayTime all already have real translated homes); no FormsFacade
//  changes required.
// ===========================================================================
TQPF_Timer HotplateEdgeOffDelay;
TQPF_Timer HotplateEdgeOnDelay;
void DoHotplateEdgeCylinderLoop()                                               //jou 2011-08-09 start : Hotplate也要敲敲敲
{
    static bool EdgePushLoop=false;
    static bool bFirst=true;
    static int iCT=0;

    if(SystemStart==false)
        return;

    if(Cylinder[C_HotplateVibration].Enable==false)                             //Steven 20120510 : 全部改用汽缸的Enable判斷
        return;

    if(IniConfig.bP16EnableHotplateEdgePushCylinderLoop==false)
        return;

    if((LastSet.iTemperature==Tempture_Ambient ||
        LastSet.iTemperature==Tempture_AmbientHot) &&
        bAmbientHotPlate==false)                                                //Kevincheng 20260525 : 渠梁常溫模式使用hotplate
        return;                                                                 //kevin 20140918 恆溫控制

    iCT++;
    if(iCT<10)
    {
        return;
    }

    // AI(ht9045-v906) 20260722: golden quirk, preserved verbatim (not "fixed")
    //  -- this `return` is BEFORE the `iCT=0;` reset below, so iCT is left
    //  >=10 when this branch fires; the very next tick re-enters this same
    //  check immediately with no further 10-tick wait.
    if(bPickFromHotplate==true && TRAY_VIBRATION==VibrationMotor)               //JerryYang 20171211 (Steven) In arm在hot plate吸取時,關閉震動馬達,邊吸邊震會造成drop error
    {
        if(Cylinder[C_HotplateVibration].GetOutBit())
        {
            Cylinder[C_HotplateVibration].Off();
            return;
        }
    }

    iCT=0;
    if(bFirst==true)                                                            //JerryYang 20170531 (wei) 敲擊方式改成可以分別設定on off時間
    {
        if(IniConfig.iP16HotplateEdgePushCylinderLoopDelay<2)
            IniConfig.iP16HotplateEdgePushCylinderLoopDelay=2;
        HotplateEdgeOffDelay.Set0_1SecAndOn(IniConfig.iP16HotplateEdgePushCylinderLoopDelay);
        EdgePushLoop=false;
        bFirst=false;
    }

    if(HotplateEdgeOffDelay.Off() && EdgePushLoop==false)
    {
        if(MOT[MMPlate1].HasRealIC() || MOT[MMPlate2].HasRealIC())
        {
            HotplateEdgeOnDelay.Set0_1SecAndOn(IniConfig.iP16HotplateEdgePushCylinderOnDelay);
            Cylinder[C_HotplateVibration].On();
            IniConfig.iVibratorHP1=IniConfig.iVibratorHP1+int(IniConfig.iP16HotplateEdgePushCylinderOnDelay/10.0);      //JerryYang 20200612 振動馬達作動時間累計
            EdgePushLoop=true;
        }
    }

    if(HotplateEdgeOnDelay.Off() && EdgePushLoop==true)
    {
        if(MOT[MMPlate1].HasRealIC() || MOT[MMPlate2].HasRealIC())
        {
            HotplateEdgeOffDelay.Set0_1SecAndOn(IniConfig.iP16HotplateEdgePushCylinderLoopDelay);
            Cylinder[C_HotplateVibration].Off();
            EdgePushLoop=false;
        }
    }
}
//------------------------------------------------------------------------------
TQPF_Timer LoaderVibrateOnDelay;
void DoLoaderVibrateLoop()                                                      //JerryYang 20191001 loader震動馬達
{
    static int iCT=0;
    static bool bFinishVibrate=true;

    if(SystemStart==false)
        return;

    if(SW[SwLoaderVibration].Enable==false)
        return;

    if(Ld_UldDelayTime.LD_EnableVibrate==false)
        return;

    iCT++;
    if(iCT<10)
    {
        return;
    }

    iCT=0;

    if(bLoaderNeedVibrate)
    {
        bLoaderNeedVibrate=false;
        bFinishVibrate=false;
        if(SW[SwLoaderVibration].Status()==false)
        {
            SW[SwLoaderVibration].On();
            LoaderVibrateOnDelay.Set0_1SecAndOn(Ld_UldDelayTime.LD_VibrateOnDelay);
        }
    }

    if(bFinishVibrate==false)
    {
        if(LoaderVibrateOnDelay.Off())
        {
            bFinishVibrate=true;
            SW[SwLoaderVibration].Off();
        }
    }
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
// W7-C2 INTEGRATED: the W7C1 W7C1_DoART_AfterCleanOut stub + `#define
// DoART_AfterCleanOut W7C1_DoART_AfterCleanOut` were REMOVED here.  The REAL
// DoART_AfterCleanOut body now lives at csystem.cpp:3626 (golden csystem.cpp:
// 14049) and is declared in csystem.h, so DoCleanOutFinishCheck's call
// (csystem.cpp:1707) binds to the single real definition.  The former `#undef
// DoART_AfterCleanOut` that preceded the real body is likewise removed (nothing
// left to un-define).  AI(W7C2-Integrate) 20260701.
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
//  W5-comms INTEGRATE (20260710): SECS_EVENT (SECSGEM/SecsEventType.h) now
//  carries CleanOutFinish/AGVSupplement, and EventReport() (SecsEventReport.h)
//  is a real, offline-safe Sim entry point -- wire the two reports directly.
//  Both calls are still reached only inside if(IniConfig.bEnable_SECS_GEM)/
//  if(TrayForm.bEnableAMR) (both false offline by default), so this is a
//  behavior-neutral no-op for every existing offline test.
#define W7C1_EVENTREPORT_CLEANOUTFINISH()  EventReport(SECS_EVENT.CleanOutFinish)
#define W7C1_EVENTREPORT_AGVSUPPLEMENT()   EventReport(SECS_EVENT.AGVSupplement)

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
//  AI(W5-Automation-Integrate) 20260710: SetLotStatus/DoChkInputCntAlarm are now
//  wired to the REAL Automation/SCK_ART.cpp free functions (SckArt_SetLotStatus/
//  SckArt_DoChkInputCntAlarm) over an embedded SckArtState -- see that unit's
//  translate report ("WHY FREE FUNCTIONS, NOT A CLASS": FormsFacade.h already
//  owns a different, offline-stub `class TfSCKART`, so this TU-local seam gets
//  its own private SckArtState core instead). SaveTestSummary is NOT one of the
//  8 translated functions -- stays a no-op. Both wired calls are reached only
//  inside `if(CosFunction.bUseSCKART...)` (default false offline), so this is
//  behavior-neutral for every currently-passing suite.
struct W7C1_TfSCKARTSeam {
    int  iTesterType;       int iLOTSTATUS_L;        int iWaitGPIBLotR;
    int  iCurrentFlexARTStep; int iInputJamCnt;      int iOutputJamCnt;
    SckArtState core;
    void SetLotStatus(int iStatus){ SckArt_SetLotStatus(core, iStatus); }
    bool DoChkInputCntAlarm(bool bExcess){ return SckArt_DoChkInputCntAlarm(core, bExcess); }
    void SaveTestSummary(int){}
    W7C1_TfSCKARTSeam():iTesterType(0),iLOTSTATUS_L(0),iWaitGPIBLotR(0),
                        iCurrentFlexARTStep(0),iInputJamCnt(0),iOutputJamCnt(0){}
};
static W7C1_TfSCKARTSeam    W7C1_fSCKART_ext;
#define W7C1_SCKART          (&W7C1_fSCKART_ext)

//  fAGV->IsSPIL_AMR (golden Automation/AGV.h) -- AI(W5-Automation-Integrate)
//  20260710: now a REAL method on FormsFacade's TfAGV (wired to
//  Automation/AGV_predicates.cpp) -- dispatch straight to fAGV instead of the
//  former always-false seam.
#define W7C1_FAGV_ISSPIL()   fAGV->IsSPIL_AMR()

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

// ===========================================================================
//  W7-C2 SEAM BLOCK  -- TU-local forward declarations / offline stand-ins for
//  the substrate symbols DoOneCycleFinishCheck + DoART_AfterCleanOut (below)
//  reference that are NOT yet present in the translated tree (mirrors the
//  W7-C1 SEAM block convention above).  Each stand-in is config-gated (false
//  offline) or a small reset target; they make this TU SYNTAX-COMPILE so the
//  faithful golden bodies can land NOW.  The serial Integrate must:
//    (a) replace every W7C2_* stand-in with the REAL translated symbol / real
//        facade member (extend LAST_GENERAL_SET / TfSCKART / TfMain / uHANA_ART /
//        TRENESAS_Server / fLotInfo / fSortCT / LotSummary / fSocketCommunication),
//    (b) remove the csystem_shims.cpp:145 `void DoOneCycleFinishCheck() {}`
//        no-op stub (ODR), and
//    (c) remove the W7C1_DoART_AfterCleanOut seam (csystem.cpp:960-961 region)
//        so DoCleanOutFinishCheck's DoART_AfterCleanOut(ret) call binds to the
//        REAL body translated here.
//  NO-REGRESSION KEY: DoOneCycleFinishCheck's real work is gated on the global
//  iOneCycle (never set to 1 by the W6.6 hub or any existing suite; the only
//  flipping seam, fMain->BtnOneCycle->Down, is offline-false), so the body is
//  INERT for every currently-passing suite.  DoART_AfterCleanOut is reached only
//  via DoCleanOutFinishCheck's iCleanOut==1 ART branch, whose guards
//  (bAutoReTest_ART / CosFunction.bUseSCKART / CosFunction.bAutoRetestGPIBmode)
//  are all false offline -- so every W7C2 stand-in below is unreachable at
//  runtime offline; gating them changes NO offline behaviour.
// ===========================================================================
#ifndef W7C2_SEAM
#define W7C2_SEAM

// --- fSCKART extended members (golden Automation/SCK_ART.h) absent from the
//     W6 TfSCKART stub.  A SEPARATE inert seam object (the golden fSCKART->X
//     references for absent members were routed to W7C2_SCKART; the members
//     present on the real TfSCKART -- iInputJamCnt/iFTRTCount/iInputCount/
//     CheckLoadingCount -- stay on fSCKART).  All methods inert / all fields 0. -
struct W7C2_TPanelSeam { AnsiString Caption; };                 // golden TPanel* (palOutputCnt/palRejectCnt)
// AI(W5-Automation-Integrate) 20260710: SetLotStatus/CheckNeedRT/DoAutoSocketOff/
// DoChkInputCntAlarm are now wired to the REAL Automation/SCK_ART.cpp free
// functions over an embedded SckArtState `core` (self-contained per seam
// instance -- see W7C1_TfSCKARTSeam above for the identical pattern/rationale).
// iNeedRT/iTesterType/dCurrYield stay top-level fields (read directly at other
// W7C2_SCKART-> call sites in this file) and are synced with `core` around each
// wired call so those other reads observe the real function's effect.
// AccessFile/UpdateCount/SaveTestSummary are NOT among the 8 translated
// functions -- stay no-ops. Every call site is reached only inside
// CosFunction.bAutoRetestGPIBmode==true (default false offline), so this is
// behavior-neutral for every currently-passing suite.
struct W7C2_TfSCKARTSeam {
    int    iNeedRT;        int iLotCount;       int iTesterType;
    int    iCurrentStatus; int iLOTSTATUS_W;    int iLOTSTATUS_R;   int iLOTSTATUS_A;
    int    iWaitGPIBLotR;  int iOutputJamCnt;   double dCurrYield;
    W7C2_TPanelSeam *palOutputCnt; W7C2_TPanelSeam *palRejectCnt;
    SckArtState core;
    void   SetLotStatus(int iStatus){ SckArt_SetLotStatus(core, iStatus); iCurrentStatus=core.iCurrentStatus; }
    void   AccessFile(bool /*bRead*/, int /*iAccess*/=-1){}
    void   UpdateCount(){}
    void   CheckNeedRT(){
        core.iTesterType=iTesterType; core.dCurrYield=dCurrYield;
        SckArt_CheckNeedRT(core);
        iNeedRT=core.iNeedRT;
    }
    void   DoAutoSocketOff(bool bAllSiteOn=false){ SckArt_DoAutoSocketOff(core, bAllSiteOn); }
    void   SaveTestSummary(int /*iSaveData*/=0){}
    bool   DoChkInputCntAlarm(bool bExcess){ return SckArt_DoChkInputCntAlarm(core, bExcess); }
    W7C2_TfSCKARTSeam():iNeedRT(0),iLotCount(0),iTesterType(0),iCurrentStatus(0),
        iLOTSTATUS_W(0),iLOTSTATUS_R(0),iLOTSTATUS_A(0),iWaitGPIBLotR(0),iOutputJamCnt(0),
        dCurrYield(0.0){ palOutputCnt=new W7C2_TPanelSeam(); palRejectCnt=new W7C2_TPanelSeam(); }
};
static W7C2_TfSCKARTSeam    W7C2_fSCKART_ext;
#define W7C2_SCKART          (&W7C2_fSCKART_ext)

// --- fMain->hanaART extended members (golden uHANA_ART) absent from TfMainHanaART
//     (which has only IsHanaArtAvailable / AddNewTrayHead -> both stay on
//     fMain->hanaART).
//     AI(W5-Automation-Integrate) 20260710: wired to a REAL embedded uHANA_ART
//     instance (Automation/HANA_ART.cpp) -- 1:1 method-signature match
//     (IsContactAvailable/IsPrimeTest/EndPrimeTest/EndReTest/NeedToRT), so this
//     seam simply delegates. Only reached from DoART_AfterCleanOut, which is
//     itself dead offline (see that function's own header comment,
//     csystem.cpp:3654-3659) -- behavior-neutral for every currently-passing
//     suite. Retiring this seam entirely (retyping fMain->hanaART to
//     uHANA_ART*) is a FormsFacade.h change, flagged but not done here (see
//     HANA_ART.cpp's own cross-unit wiring note).
struct W7C2_TfHanaSeam {
    uHANA_ART real;
    bool IsContactAvailable(){ return real.IsContactAvailable(); }
    bool IsPrimeTest(){ return real.IsPrimeTest(); }
    void EndPrimeTest(){ real.EndPrimeTest(); }
    void EndReTest(){ real.EndReTest(); }
    int  NeedToRT(){ return real.NeedToRT(); }              // 0:Waiting command,1:Need RT,2:Lot End
};
static W7C2_TfHanaSeam      W7C2_fHana_ext;
#define W7C2_HANAART         (&W7C2_fHana_ext)

// --- fMain->RENESAS_Server (golden uRENESAS_Server.h TRENESAS_Server) absent. --
struct W7C2_TfRenesasSeam {
    int  iHaveRecvTestEnd;
    bool Check71CommandDuring50(){ return false; }
    void SendTestEnd(){}
    W7C2_TfRenesasSeam():iHaveRecvTestEnd(0){}
};
static W7C2_TfRenesasSeam   W7C2_fRenesas_ext;
#define W7C2_RENESAS         (&W7C2_fRenesas_ext)

// --- LastSet extended members (golden LastSet.h) absent from LAST_GENERAL_SET
//     shim.  golden field TYPES verbatim.  All inert (touched only in the ART
//     branches, false offline). --------------------------------------------------
static unsigned int W7C2_LastSet_BinCT[4][256];      // golden LastSet.h:106
static unsigned int W7C2_LastSet_BinCT_ART[4][256];  // golden LastSet.h:107
static int          W7C2_LastSet_iContactCT[2];      // golden LastSet.h:354
static int          W7C2_LastSet_iAutoRetestCount_ART = 0;  // golden LastSet.h:372
static int          W7C2_LastSet_iRetestFlagART = 0;        // golden LastSet.h:404
static int          W7C2_LastSet_iSCKART_RTUnitCount = 0;   // golden LastSet.h:410
static long         W7C2_LastSet_lSCKARTBinCT[256];         // golden LastSet.h:507
static bool         W7C2_LastSet_bWaitEndLotAutoRetestGPIB = false; // golden LastSet.h:401
static bool         W7C2_LastSet_bEndLotAutoRetestGPIB = false;     // golden LastSet.h:402
static bool         W7C2_LastSet_bFinEndLotAutoRetestGPIB = false;  // golden LastSet.h:403
static bool         W7C2_LastSet_bFirstTestAutoRetestGPIB = false;  // golden LastSet.h:405
static bool         W7C2_LastSet_bBreakSCKART = false;             // golden LastSet.h:420
#define W7C2_LS_BinCT                     W7C2_LastSet_BinCT
#define W7C2_LS_BinCT_ART                 W7C2_LastSet_BinCT_ART
#define W7C2_LS_iContactCT                W7C2_LastSet_iContactCT
#define W7C2_LS_iAutoRetestCount_ART      W7C2_LastSet_iAutoRetestCount_ART
#define W7C2_LS_iRetestFlagART            W7C2_LastSet_iRetestFlagART
#define W7C2_LS_iSCKART_RTUnitCount       W7C2_LastSet_iSCKART_RTUnitCount
#define W7C2_LS_lSCKARTBinCT              W7C2_LastSet_lSCKARTBinCT
#define W7C2_LS_bWaitEndLotAutoRetestGPIB W7C2_LastSet_bWaitEndLotAutoRetestGPIB
#define W7C2_LS_bEndLotAutoRetestGPIB     W7C2_LastSet_bEndLotAutoRetestGPIB
#define W7C2_LS_bFinEndLotAutoRetestGPIB  W7C2_LastSet_bFinEndLotAutoRetestGPIB
#define W7C2_LS_bFirstTestAutoRetestGPIB  W7C2_LastSet_bFirstTestAutoRetestGPIB
#define W7C2_LS_bBreakSCKART              W7C2_LastSet_bBreakSCKART

// --- fMain absent members (golden main.h) not on the FormsFacade TfMain --------
//     slLowYieldAlarm : golden TMyStringList* (low-yield alarm list).  Inert
//     stand-in exposing Count / Strings[] / Clear() (offline: empty list). ------
struct W7C2_TStrListSeam {
    int Count;
    AnsiString *Strings;      // indexable; empty offline
    W7C2_TStrListSeam():Count(0),Strings(new AnsiString[1]){}
    void Clear(){ Count=0; }
};
static W7C2_TStrListSeam    W7C2_fMain_slLowYieldAlarm;
#define W7C2_FMAIN_SLLOWYIELD   (&W7C2_fMain_slLowYieldAlarm)
//     cbRunStartMode : golden TComboBox* (run-start-mode combo; ->Text read). ---
struct W7C2_TComboSeam { AnsiString Text; };
static W7C2_TComboSeam      W7C2_fMain_cbRunStartMode;
#define W7C2_FMAIN_CBRUNSTARTMODE (&W7C2_fMain_cbRunStartMode)
//     Label12 : golden TLabel* Caption (ART retest-count display).  lvalue. -----
static AnsiString           W7C2_fMain_Label12_Caption;
#define W7C2_FMAIN_LABEL12      W7C2_fMain_Label12_Caption
//     method stand-ins (offline no-op). ----------------------------------------
#define W7C2_FMAIN_INITIALTRAYFEED(s)      do { (void)(s); } while(0)   // golden fMain->InitialTrayFeedTask(AnsiString)
#define W7C2_FMAIN_DISABLESITEMAP(b,f)     do { (void)(b); (void)(f); } while(0) // golden fMain->DisableSiteMappingCheck(bool,AnsiString)
#define W7C2_FMAIN_CHANGETESTERCONNECT(n)  do { (void)(n); } while(0)   // golden fMain->ChangeTesterConnect(int)
#define W7C2_FMAIN_SETLOTSTATE(n)          do { (void)(n); } while(0)   // golden fMain->SetLotState(int)
#define W7C2_FMAIN_CLARNDATA(n,s)          do { (void)(n); (void)(s); } while(0) // golden fMain->Clarn_Data(int,AnsiString)
#define W7C2_FMAIN_BTNPAUSECLICK()         do { } while(0)   // golden fMain->BtnPauseClick(fMain) (inside if(fAutoTeach->IsRun()), false offline)

// --- BLCarryKit.SetHasNullIcToNullIc (golden MyKitSuck.h) -- absent on the
//     translated TMyKitSuck (it has SetNullIcToHasNullIc, the INVERSE).  Gate
//     (inside if(bD58UseArm1PickPlaceArm2Test && bArm1PickPlaceArm2Test), false
//     offline).  Integrate: add the real SetHasNullIcToNullIc to TMyKitSuck. -----
#define W7C2_BLCARRYKIT_SETHASNULLIC()     do { } while(0)   // golden BLCarryKit.SetHasNullIcToNullIc()

// --- fContact->rgHandlerMode->Enabled (golden cContact.h TRadioGroup*).  fContact
//     exists (atester_shims TfContactShim) but rgHandlerMode is absent.  Gate. ---
#define W7C2_FCONTACT_RGHANDLER_ENABLE()   do { } while(0)   // golden fContact->rgHandlerMode->Enabled=true

// --- fHome->GaliMotorServoOff (golden uHome.h) -- fHome absent.  Gate. ----------
#define W7C2_FHOME_SERVOOFF(f)             do { (void)(f); } while(0) // golden fHome->GaliMotorServoOff(AnsiString)

// --- fAutoTeach->IsRun (golden AutoTeach.h) -- fAutoTeach absent.  offline: false
#define W7C2_FAUTOTEACH_ISRUN()            (false)          // golden fAutoTeach->IsRun()

// --- fSocketCommunication (golden cSocketCommunication.h) absent.  bOneCycleFormServer
//     offline false so the OneCycle-from-server branch is never taken; ErrorMessage
//     empty.  Inert seam object. -------------------------------------------------
struct W7C2_TfSocketCommSeam { bool bOneCycleFormServer; AnsiString ErrorMessage;
    W7C2_TfSocketCommSeam():bOneCycleFormServer(false){} };
static W7C2_TfSocketCommSeam W7C2_fSocketComm;
#define W7C2_FSOCKETCOMM        (&W7C2_fSocketComm)

// --- fYieldMonitoring->ClearYieldCount / fCounterClear->WriteCTInfo /
//     fContactCT->ClearData_AutoClean : the objects exist (aHotPlateSubstrate /
//     W7C1 seam) but these specific members are absent.  Gate each (offline
//     no-op). --------------------------------------------------------------------
#define W7C2_FYIELD_CLEARCOUNT()           do { } while(0)   // golden fYieldMonitoring->ClearYieldCount()
#define W7C2_FCOUNTER_WRITECTINFO()        do { } while(0)   // golden fCounterClear->WriteCTInfo()
#define W7C2_FCONTACTCT_CLEARAUTOCLEAN()   do { } while(0)   // golden fContactCT->ClearData_AutoClean()

// --- fLotInfo absent members (golden uLotInfo.h) -------------------------------
#define W7C2_FLOTINFO_PRODTESTERREPORT()      do { } while(0)  // golden fLotInfo->ProductTesterReport()
#define W7C2_FLOTINFO_CBRUNMODE_ITEMINDEX(x)  do { (void)(x); } while(0) // golden fLotInfo->cbRunMode->ItemIndex=x
#define W7C2_FLOTINFO_CLEARBARCODE()          do { } while(0)  // golden fLotInfo->btClearBarcodeList->Click()

// --- fSortCT absent members (golden cSortCT.h) ---------------------------------
#define W7C2_FSORTCT_SHOWLOADING()         do { } while(0)   // golden fSortCT->ShowLoadingIC()
#define W7C2_FSORTCT_SHOWSORT()            do { } while(0)   // golden fSortCT->ShowSortIC()

// --- fTrayMapping->ClearTrayIDByLot (golden TrayMapping.h) -- member absent.  Gate.
#define W7C2_FTRAYMAP_CLEARTRAYIDBYLOT()   do { } while(0)   // golden fTrayMapping->ClearTrayIDByLot()

// --- LotSummary (golden cLotSummary.h) absent.  Gate ClearRTData(). -------------
#define W7C2_LOTSUMMARY_CLEARRTDATA()      do { } while(0)   // golden LotSummary.ClearRTData()

// --- EventReport(SECS_EVENT.X) : W5-comms INTEGRATE (20260710) -- mirrors the
//     W7C1 CleanOutFinish wiring above; SECS_EVENT now carries OneCycleFinish/
//     ArtFTFinish/ArtRTFinish and EventReport() is a real Sim entry point.
//     All three calls are still reached only inside if(IniConfig.bEnable_SECS_GEM)
//     (false offline by default) -- behavior-neutral for existing offline tests.
#define W7C2_EVENTREPORT_ONECYCLEFINISH()  EventReport(SECS_EVENT.OneCycleFinish) // 41 One Cycle Finish
#define W7C2_EVENTREPORT_ARTFTFINISH()     EventReport(SECS_EVENT.ArtFTFinish)    // 63 ART FT finish
#define W7C2_EVENTREPORT_ARTRTFINISH()     EventReport(SECS_EVENT.ArtRTFinish)    // 61 ART RT finish

// --- absent FREE functions -----------------------------------------------------
static bool W7C2_InArmSuckState(){ return false; }        // golden -- any in-arm picker vacuum on? offline none
#define InArmSuckState         W7C2_InArmSuckState
static bool W7C2_OutArmSuckState(){ return false; }       // golden -- any out-arm picker vacuum on? offline none
#define OutArmSuckState        W7C2_OutArmSuckState
static bool W7C2_DoLoaderTrayFeed(){ return true; }       // golden -- loader clean-out tray feed; offline: done (true)
#define DoLoaderTrayFeed       W7C2_DoLoaderTrayFeed
static void W7C2_DoInArm_SuckerMap(){}                    // golden -- rebuild in-arm sucker map after site mapping
#define DoInArm_SuckerMap      W7C2_DoInArm_SuckerMap
static bool W7C2_CheckNeedToRT(){ return false; }         // golden -- need auto-retest? offline: no
#define CheckNeedToRT          W7C2_CheckNeedToRT
static int  W7C2_ShowMyMessageBox_YES_NO(AnsiString /*s1*/, AnsiString /*s2*/=""){ return 0; } // golden mymessbox.h -- offline: NO(0)
#define ShowMyMessageBox_YES_NO W7C2_ShowMyMessageBox_YES_NO
static void W7C2_DoAutoRetest(bool /*b*/){}               // golden -- kick auto-retest sequence
#define DoAutoRetest           W7C2_DoAutoRetest
static void W7C2_MySleep(unsigned long /*ms*/){}          // golden common.h:261 MySleep -- offline no-op (acarry_shims/common not included here)
#define MySleep                W7C2_MySleep

// --- absent FREE globals -------------------------------------------------------
static bool W7C2_bOnecycleTrayFeed = false;   // golden cmydef -- one-cycle tray-feed request (Steven 20110525)
#define bOnecycleTrayFeed      W7C2_bOnecycleTrayFeed
static bool W7C2_bFix3_OneCycleTimeSet = false; // golden cmydef -- Fix3 one-cycle alarm timer set flag
#define bFix3_OneCycleTimeSet  W7C2_bFix3_OneCycleTimeSet

// --- header-declared, GATED-body leaves the finish path calls (link-only) ------
//  W7-C2 INTEGRATE: these are declared in csystem.h / cprod.h but their REAL
//  bodies are #if 0-gated in the untranslated app code (csystem.cpp finish-path
//  ladder / cprod.cpp:184 TODO(W6) gate), so they are UNDEFINED at link.  Only
//  csystem.cpp references them (verified: no other linked TU does), so a TU-local
//  `#define realname W7C2_realname` + static offline stand-in is safe and matches
//  the seam convention.  Every one sits on the ONE CYCLE FINISH terminal path
//  (reached only after all wait cursors 1..21/1003 pass) or the offline-false
//  AutoSiteMap sub-path, so it is inert offline; Integrate binds the real bodies
//  when those units are translated.  AI(W7C2-Integrate) 20260701.
static void W7C2_DoSiteMappingResult(){}                        // golden csystem.cpp -- Auto Site Mapping result write
#define DoSiteMappingResult    W7C2_DoSiteMappingResult
static void W7C2_ReadWriteBinCountMode(bool /*bRead*/){}        // golden csystem.cpp -- Bin1/Bin2... count record
#define ReadWriteBinCountMode  W7C2_ReadWriteBinCountMode
static void W7C2_AutoTrayCylinderFree(){}                       // golden csystem.cpp -- release Auto-Tray cylinders
#define AutoTrayCylinderFree   W7C2_AutoTrayCylinderFree
static bool W7C2_EmptySocketCheckModeBeUse(){ return false; }   // golden csystem.cpp -- offline: no empty-socket-check mode
#define EmptySocketCheckModeBeUse W7C2_EmptySocketCheckModeBeUse
static bool W7C2_WriteLastDataFile(bool /*BackUp2*/=false, bool /*bNotContact*/=false){ return true; } // golden cprod.cpp:1944 (gated); offline: write ok
#define WriteLastDataFile      W7C2_WriteLastDataFile
static int  W7C2_iClearSocketFunctionTask = 0;                  // golden csystem.h:61 (extern int) -- clear-socket sub-task cursor
#define iClearSocketFunctionTask W7C2_iClearSocketFunctionTask

#endif // W7C2_SEAM

// ===========================================================================
//  DoOneCycleFinishCheck  -- golden csystem.cpp:12813-14047 (~1235 lines).
//  THE one-cycle-finish check FSM.  Translated FAITHFULLY this wave (W7-C2):
//  the if(iOneCycle) entry guard, every iOneCycleTask cursor transition (0..21
//  plus 1003), every grid-backed predicate read, the customer/mode branches
//  (KYEC Fix3 / bUseTwoArm32Site / Rotate / AOI / AutoSiteMap / SCK AutoClean /
//  QA-Mode / SECS EventReport / Low-Yield / Contact / PickerLife / ART), and
//  the ONE SOFT_SIMULTE region (the commented //#ifndef SOFT_SIMULTE MES1650
//  block) are reproduced VERBATIM.  The #ifdef DEBUG_DUTONOFF /
//  #ifdef DEBUG_OneCycleContinous blocks are reproduced verbatim (both macros
//  undefined -> those blocks compile OUT, matching the golden default build).
//  Those branches are INERT offline (their guards CUSTOMER_CODE / IniConfig.* /
//  CosFunction.* / iOneCycle-gating are false) but must compile+link -- the
//  W7C2_SEAM block above supplies stand-ins for symbols not yet in the tree.
//
//  NO-REGRESSION KEY: the whole body is gated on the global iOneCycle, which
//  the W6.6 hub and every existing suite leave 0 (the only flipping seam,
//  fMain->BtnOneCycle->Down, is offline-false), so the body is INERT for every
//  currently-passing suite (byte-identical).  The final else-branch (golden
//  :14025) reproduces the BtnOneCycle->Down latch that would set iOneCycle=1.
// ===========================================================================
void DoOneCycleFinishCheck()
{
    bool bCleanOut=false;
    bool bNeedTrayFeed=false;
    bool bI09_NeedOneCycleAgain=false;                                          //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
    int ret=0;
    bool bQAModeTrayEnd=false;                                                  //Steven 20120615
    AnsiString AlarmCode, ErrPart;

    if(iOneCycle)
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE)                                          //Ifor 20190516 :add KYEC 避免Fix3 Full功能導致無預警Hang up 新增Alarm
        {
            if(FIX3_FULL_PLACE==Fix3K_UseCylinder && Cylinder[C_FixTray_FullPlace].OffSensor()==false)                  //Steven 20150914 : Fixed for FIX3_FULL_PLACE==2  //JerryYang 20180129 (Steven) Mark掉,改用Pop判斷
            {
                iOneCycleTask=1;
                ShowMyMessage("Please Check the Cylinder 'C_FixTray_FullPlace'.", "請確認汽缸'C_FixTray_FullPlace'。");
                return;
            }
        }

        if(IniConfig.bD51UseOnecycleCleanOutFinishTestArmAtRear)
        {
            if(IndexStatus!=IndexIsBack)
            {
                iOneCycleTask=2;
                return;
            }
        }
        else
        {
            if(IndexStatus!=Z1_Z2_Normal)
            {
                iOneCycleTask=3;
                return;
            }
        }

        if(bNeedOneCycleByYieldAlm==true &&                                     //ChungHung 矽格湖口 20110602 IC未交換完成照程掉料
           CanYieldAlarmRemainInSHT())                                          //JerryYang 20230131 : fix one cycle hang up
        {
            if(OutArmSuckState())
            {                                                                   //新增偵測所有吸嘴狀態
                iOneCycleTask=4;
                return;
            }
        }
        else
        {
            if(InArmSuckState() || OutArmSuckState())
            {
                iOneCycleTask=4;
                return;
            }
        }

        if(bUseTwoArm32Site==true)                                              //kevin 20190710 32 Site use add onecycle 後 arm 在下
        {
            if(bRunAutoClean)                                                   //Steven 20220920 : Fix for auto clean close site hang up
            {
                iOneCycleTask=16;
                return;
            }

            if(InArmSuck.HasIC()==false &&
               IndexHasIC()==false &&
               OutArmSuck.HasIC()==false &&                                     //Steven 20220712 : 修正one cycle之後, shuttle上面遺留HAS_NULL_IC造成Hang up
               FRCarryKit.UseSiteHasIC()==false &&
               BRCarryKit.UseSiteHasIC()==false &&
               AllArmZIsSafe())
            {
                if(FLCarryKit.HasRealIC()==false &&
                   BLCarryKit.HasRealIC()==false)                               //Steven 20230309 : fixed for NN mode hang up
                {
                    FLCarryKit.ClearAll();
                    BLCarryKit.ClearAll();
                }

                iOneCycleTask=16;
            }

            if(iTestTwoArm32SiteTask==230)
            {
                iOneCycleTask=10;
                return;
            }
        }

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

        if(bNeedOneCycleByYieldAlm==true && CanYieldAlarmRemainInSHT())         //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
        {
            if(OutArmSuck.HasIC() || OutputShuttleHasIC() ||
               IndexHasIC() || AllArmZIsSafe()==false)
            {
                iOneCycleTask=5;
                return;
            }
        }
        else
        {
            if(InArmSuck.HasIC() ||                                             //ChungHung 20130426 add
               OutArmSuck.HasIC() ||
               ShuttleHasIC() ||
               IndexHasIC() || AllArmZIsSafe()==false)
            {
                iOneCycleTask=5;
                return;
            }
        }

        if(USE_ROTATE_KIT==1  && tRotate.ActiveRotate)                          //kevin 20130812 有ROTATE裝置沒有使用ROTATE功能  //jou 2013-03-01 Rotate kit
        {
            if(iRotate_Type==eInOutArm1Motor)
            {
            }
            else if(MOT[MInRotateKit].HasIC() || MOT[MOutRotateKit].HasIC())
            {
                iOneCycleTask=6;
                return;
            }
        }

        if(USE_AOI_Inspection)
        {
            if(AOIKit.HasIC())
            {
                iOneCycleTask=7;
                return;
            }
        }

        if(PitchCylinderState[0]!=0 ||
           PitchCylinderState[1]!=0 ||
           PitchCylinderState[2]!=0)
        {
            iOneCycleTask=8;
            return;
        }

        if(CheckIndexIsNormal()==false)
        {
            iOneCycleTask=9;
            return;
        }

        if(IsNNMode()==NN_1Row)                                                 //kevin 20190710 32 Site use add onecycle 後 arm 在下
        {
            if(iTestTwoArm32SiteTask==230)
            {
                iOneCycleTask=10;
                return;
            }
        }

        if(IsTrayArmMoveAvoidOutArmCrash() ||
           iCatchTrayControlManual>=2)                                          //jou 2011-02-17
        {
            iOneCycleTask=11;
            return;
        }

        if(AllArmZIsSafe()==false)
        {
            iOneCycleTask=12;
            return;
        }

        if(InSHT1InLF()==false)
        {
            iOneCycleTask=13;
            return;
        }

        if(IniConfig.bIndexArm2SupplyLight==false &&                            //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
           TestIF_File.bForEgisTecTest==false     &&                            //Steven 20140922 : Arm2當作指紋測試
           TestIF_File.bArm1PickPlaceArm2Test==false)                           //kevin 20150127 Arm1 下壓 arm2 測試
        {
            if(InSHT2InLF()==false)
            {
                iOneCycleTask=14;
                return;
            }
        }

        if(bRunAutoClean==true && fContact->IsRun2DCheck()==false)              //JerryYang 20250328 : fix auto clean 與2D Gating同時觸發one cycle hang up
        {
            iOneCycleTask=15;
            return;
        }

        if(FIX3_FULL_PLACE==Fix3K_UseCylinder)                                  //Ifor 20190516 :add KYEC 避免Fix3 Full功能導致無預警Hang up 新增Alarm
        {
            if(bShuttleMoveToLeftforFix3)                                       //Steven 20220526 : 修正Fix3氣缸要在One Cycle之前打回去
            {
                iOneCycleTask=17;
                return;
            }
            else if(Cylinder[C_FixTray_FullPlace].OffSensor()==false)
            {
                Cylinder[C_FixTray_FullPlace].Off();                            //Steven 20200730 : 修正One Cycle會死雞的問題
                if(bFix3_OneCycleTimeSet==false)                                //避免汽缸移動時兩顆 Sensor Off時誤報Alarm
                {
                    bFix3_OneCycleTimeSet=true;
                    FixTrayAlarmCheck.SetSecAndOn(5);
                }

                if(FixTrayAlarmCheck.Off())
                {
                    ShowMyMessage("Please Check the Cylinder 'C_FixTray_FullPlace'.", "請確認汽缸'C_FixTray_FullPlace'。");
                    bFix3_OneCycleTimeSet=false;
                }
                iOneCycleTask=17;
                return;
            }
        }
        else if(FIX3_FULL_PLACE==Fix3K_UseStepperMotor &&
                Fix3MoveToLeft(false)==false)                                   //Jimmychiu 20240401 : FIX3回到右側
        {
            iOneCycleTask=1003;
            return;
        }

        bFix3_OneCycleTimeSet=false;

        if(MOT[MMAuto1_Car].fHasTray ||                                         //JerryYang 20191113 Auto退完tray再跳出one cycle finish
           MOT[MMAuto2_Car].fHasTray ||
           MOT[MMAuto3_Car].fHasTray ||
           MOT[MMAuto4_Car].fHasTray ||
           MOT[MMAuto5_Car].fHasTray ||
           MOT[MMAuto6_Car].fHasTray)
        {
            iOneCycleTask=18;
            return;
        }

        if(bPlaceToHotplate==true)
        {
            iOneCycleTask=19;
            return;
        }

        if(FIX3_FULL_PLACE==Fix3K_UseCylinder &&                                //JerryYang 20220718 : 修正汽缸流程還沒把shuttle canMove的flag解開就跳one cycle finish, 造成後續hang up
           (iFix3CanFullTask==50 || iFix3CanFullTask==51 || iFix3CanFullTask==65))
        {
            iOneCycleTask=20;
            return;
        }

        if(AUTO3_IS_MAGAZINE==1 && (bChaneMagTrayflag || bMagGetNewTrayflag))   //Ifor 20240905 add:MAGAZINE 動作完成後才可OneCycle Finish
        {
            iOneCycleTask=21;
            return;
        }

        if(bDoLoaderCleanOut)                                                   //kevin 20211106 確認吸取數量完收load tray  //pig 2011.11.08
        {
            if(DoLoaderTrayFeed()==false)
                return;
            else
                bDoLoaderCleanOut=false;
        }
        InitInArmTask();
        InitOutArmTask();
        bIndexArmNoTestting=false;                                              //ChungHung 20140730 add ContinuousFailHaveOneCycle
        for(int i=0; i<4; i++)
            Temperature.iATCCurrentFailCount[i]=0;                              //Steven 20151123 : Continue Fail Temp Offset for ATC

        iOneCycleTask=0;
        if(CUSTOMER_CODE==CC_ASE_CL)                                            //JerryYang 20250728 : Thomas要求只有跳出ONE CYCLE FINISH視窗才發報Event
        {
        }
        else
        {
            if(IniConfig.bEnable_SECS_GEM==true)                                //Steven 20140528 : Secs Gem
                W7C2_EVENTREPORT_ONECYCLEFINISH();                         //41     One Cycle Finish
        }

        W7C1_FLTCSENSOR_CLEAR(1);                                          //Steven 20140805 : 將Latch清空，確保沒有問題
        W7C1_FLTCSENSOR_CLEAR(0);                                          //Sam 20221101 : Latch 清除都要確認是否清清乾淨

        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20220927 : Jovie要求要跳ONE CYCLE FINISH並開門按Z1
        {
        }
        else
        {
            bOutShtLoseICNeedSetErrBin=false;                                   //JerryYang 20170609 (wei) for JSCC 清除旗標
            bIndexDropICNeedSetErrBin=false;
            ZeroMemory(bTestSiteNeedSetErrBin, sizeof(bTestSiteNeedSetErrBin));
        }

        ResetShuttleWhichKit();                                                 //Steven 20220319 : 換到下面

        ZeroMemory(bPickLoaderDuplicateErr, sizeof(bPickLoaderDuplicateErr));
        ZeroMemory(bPickHPDuplicateErr, sizeof(bPickHPDuplicateErr));
        ZeroMemory(bTryPickHPDuplicateErr, sizeof(bTryPickHPDuplicateErr));
        if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
           TestIF_File.bArm1PickPlaceArm2Test==true     )
        {
            if(Prod.TestY2_Middle==Prod.TestY2_Rear ||
               Prod.TestZ2_Test==Prod.TestZ2_Safe ||
               iBackUpZ2DownPosition==Prod.TestZ2_Safe)
            {
                Prod.TestY2_Middle      =iBackupTestY2_Middle;
                Prod.TestZ2_Test        =iBackupTestZ2_Test;
                Prod.TestZ2_Drop_Offset =iBackupTestZ2_Drop;
                iBackUpZ2DownPosition   =Prod.TestZ2_Test;
            }
        }
        bCheckGiveWay=false;                                                    //Ifor 20200521 Fix:清除RTC旗標
        bNeedCheckRTCReport=false;                                              //Ifor 20200521 Fix:清除RTC旗標

        if(IniConfig.bEnableSocketCommunication &&
           W7C2_FSOCKETCOMM->bOneCycleFormServer)                           //ChungHung 20130112 add for ASE_KR Socket Tester
        {
            W7C2_FSOCKETCOMM->bOneCycleFormServer=false;

            ShowMyMessage("OneCycle Form Server", W7C2_FSOCKETCOMM->ErrorMessage, W7C2_FSOCKETCOMM->ErrorMessage);
            ret=K_RETRY;                                                        //Steven 20260612 : Fix == to = (was comparison, not assignment)
        }
        else if(_bHomeNeedOnecycle==false && iClearSocketFunction==0)
        {
            if(bSECSOneCycleComm==true)                                         //RogerYang 20170705 (wei) 避免 SECS 下 OneCycle 後，AutoClean 結束時不跳 OneCycle Finish
            {
                if(IniConfig.bSPILFunction==true)
                {
                    bIsAutoOneCycle=false;
                }
                bSECSOneCycleComm=false;
            }

            if(CosFunction.bUSEJCETSiteMapMode==true &&                         //jou 2016-11-02 JCET要求Auto Site mapping mode做完接續測
               IniConfig.bUseAutoSiteMapping &&
               LastSet.iRunStartMode==rsmAutoSiteMap &&
               bASMFinishOneCycle==true &&
               bAutoSiteMapWaitTestPass==true)
            {
                if(bSiteMappingCHKOK==false)
                {
                    DoSiteMappingResult();
                    DoInArm_SuckerMap();
                    bASMFinishOneCycle=false;
                }
            }
            else if((CUSTOMER_CODE==CC_ASE_M || CUSTOMER_CODE==CC_ASE_CL) &&    //JerryYang 20250120 : add   //Ifor 20180417 :add ASE_M Auto Site mapping mode做完接續測試
                    IniConfig.bUseAutoSiteMapping &&
                    LastSet.iRunStartMode==rsmAutoSiteMap &&
                    bASMFinishOneCycle==true && bAutoSiteMapWaitTestPass==true)
            {
                if(bSiteMappingCHKOK==false)
                {
                    DoSiteMappingResult();
                    DoInArm_SuckerMap();
                    bASMFinishOneCycle=false;
                }
            }
            else if(LastSet.iRunStartMode==rsmAutoSiteMap &&                    //Steven 20120830 : AutoSiteMapping, 手動移除Loader Tray
                    bASMFinishOneCycle &&
                    CUSTOMER_CODE!=CC_ASE_CL)                                   //JerryYang 20250120 : add
            {
                ret=K_TRAY_FEED;
            }
            else if(bIsAutoOneCycle ||
                    bResetMode ||                                               //kevin 20151112 add Reset 起動 Autoclean 起動 造成RESET 無法關閉
                    bDoEmptySocketOneCycle ||
                    bIsASMAutoOneCycle)                                         //JerryYang 20161121 拿掉bNeedOneCycleByYieldAlm==false, one cycle finish時要判斷是否show yield alarm  //JerryYang 20250120 : add
            {
                if(bDoEmptySocketOneCycle)                                      //Steven 20201022 : For RFMD Empty Socket Check Funstion.
                {
                    bDoEmptySocketOneCycle=false;

                    if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum)              //Steven 20120222 : Real Time CCD 新增One cycle完成時送出@END+動作來停止檢測動作, 要在Alarm之前,給OA喘息的機會
                    {
                        COM2->DoReleaseAndInspEnd();
                    }
                    //---------------------------------------

                    if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction)        //Steven 20120716 : OCR    //ChungHung 20120830 add OCR Function
                    {
                        W7C1_FOCR_RELEASEINSPEND();
                    }
                    bResetMode=false;
                    InitialIndexSocketCheckTask();
                    bDoEmptySocketCheck=true;

                    RecordProcess("EmptySocketOneCycle Finsih");
                    RecordProcess("Start EmptySocketCheck");
                }

                if(bResetMode)                                                  //kevin 20151112 add
                {
                    if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum)              //Steven 20120222 : Real Time CCD 新增One cycle完成時送出@END+動作來停止檢測動作, 要在Alarm之前,給OA喘息的機會
                    {
                        COM2->DoReleaseAndInspEnd();
                    }
                    //---------------------------------------

                    if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction)        //Steven 20120716 : OCR    //ChungHung 20120830 add OCR Function
                    {
                        W7C1_FOCR_RELEASEINSPEND();
                    }

                    if(IniConfig.bOneCycleNeedPowerOff==true)                   //Steven 20110214 : OneCycle時要Poewr Off
                    {
                        ret=ShowErrorMessage("MES1641", K_RETRY|K_HOME|K_CLEAN_OUT, MMSystem, false);                   //reset finish  //Steven 20240328 : K_SKIP --> K_HOME
                    }
                    else
                    {
                         ret=ShowErrorMessage("MES1641", K_RETRY|K_CLEAN_OUT, MMSystem, false);                         //kevin 20140412 cancel skip  //reset finish
                    }

                    if(ret==K_CLEAN_OUT &&
                       IniConfig.bA17_1RESETCleanOutWithoutTest)                //JimmuyChiu 20211012   Add Reset->Clean out with no testing
                    {
                        bResetModeAndCleanOutAndNoTest=true;
                    }
                    else
                    {
                        bResetModeAndCleanOutAndNoTest=false;
                    }
                    bReOpenGpib=true;                                           //Steven 20101013
                    bInitialMaxTime=true;                                       //jou 2011-11-09 增加initial max time set
                    bResetMode=false;
                    bLampReset=false;
                    bOutShuttleMissIC=false;                                    //wei 20160509 Out Shuttle Miss IC
                    bTestEPaddKg=false;                                         //jou 20171024
                }

                if(bIsAutoOneCycle && LastSet.iRunStartMode!=rsmAutoSiteMap)
                {
                    bFirstDeviceInitialTestDelayWhichOutAfterAutoClean=true;    //ChungHung 20140105 add for SCK

                    if(LastSet.iTemperature==Tempture_Hot && MOT[MMPlate1].HasIC()==false && MOT[MMPlate2].HasIC()==false)                                      //JerryYang 20251124 : 修正non hot plate模式觸發auto clean後，沒有優先使用auto clean預熱秒數進行預熱
                        bFirstDeviceInitialTestDelayWhichOutAfterAutoClean=false;

                    RecordProcess("ONE CYCLE Finished by Auto Clean.");         //Steven 20130618 : 紀錄Auto Clean
                    if(CosFunction.bYieldAlmNeedOneCycle &&
                       bNeedOneCycleByYieldAlm)                                 //JerryYang 20161121 one cycle finish時要判斷是否show yield alarm
                    {
                        W7C2_FYIELD_CLEARCOUNT();                    //JerryYang 20160401 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                        for(int i=0; i<W7C2_FMAIN_SLLOWYIELD->Count; i++)
                        {
                            AlarmCode=W7C2_FMAIN_SLLOWYIELD->Strings[i].SubString(1, W7C2_FMAIN_SLLOWYIELD->Strings[i].AnsiPos(",")-1);
                            ErrPart=W7C2_FMAIN_SLLOWYIELD->Strings[i].SubString(W7C2_FMAIN_SLLOWYIELD->Strings[i].AnsiPos(",")+1, W7C2_FMAIN_SLLOWYIELD->Strings[i].Length());
                            ShowErrorMessage(AlarmCode, 0, MMInterface, 0, ErrPart);
                        }
                        W7C2_FMAIN_SLLOWYIELD->Clear();
                    }

                    if(TestIF.iAutoClean_Function)                              //Eliot 2008_07_29
                    {
                        //if(CosFunction.bSmartAutoClean && bRunACAdaptive)      //Sam 20240726 : AI Clean Mark //Sam 20230914 : 自適應性良率監控
                        //{
                                                                                //不要清除
                        //}
                        //else
                        //{
                            //Steven 20260427 : ATK P260427-ATK-H9-01 disable redundant reset
                            //                  AutoClean.cpp:5153 already resets after Auto Clean Finish.
                            //                  This second reset on Clean Out path was the perceived double reset.
                            //iAutoClean_IndexContactCount=0;
                        //}
                        lAutoClean_TimeCount=0;                                 //jou 20250102 : auto clean triger time count
                        W7C2_FCONTACTCT_CLEARAUTOCLEAN();                      //ChungHung 20131225 add for SCK
                        bRunAutoClean=true;
                        hAutoCleanHangUp.SetSecAndOn(Prod.iHangupMaxTime);      //Steven 20220702 : 針對Auto Clean的Hang Up偵測
                    }

                    if(IniConfig.bE53LowYieldAutoClean)                         //wei 20141201 Low Yield Auto Clean(%) start
                    {
                        if(bLowYieldCleanOut)
                        {
                            ret=K_CLEAN_OUT;
                        }
                        else
                        {
                            bLowYieldAutoCleanEnd=false;                        //wei 20141216  LowYieldautoclean  避免重複進去
                        }
                    }
                    else
                    {
                        if(CosFunction.bYieldAlmNeedOneCycle &&
                           bNeedOneCycleByYieldAlm)                             //JerryYang 20161118 避免show yield alarm後沒按start就開始做auto clean
                            bNeedOneCycleByYieldAlm=false;
                        else
                            W7C1_FMAIN_START("DoOneCycleFinishCheck 1");
                    }

                    if(CosFunction.bYieldAlmNeedOneCycle &&
                       bNeedOneCycleByYieldAlm)                                 //JerryYang 20161118 show完yield Alarm 要把旗標清成false
                        bNeedOneCycleByYieldAlm=false;
                    bIsAutoOneCycle=false;                                      //Eliot 2007_0926
                    bIsAutoOneCycleAutoclean=false;                             //kevin 20120710 autoclean
                }
                else if(bIsASMAutoOneCycle)                                     //JerryYang 20250120 : add
                {
                    if(IniConfig.bUseAutoSiteMapping && IniConfig.bI21EnableASM && bIsAutoOneCycle==false)
                    {
                        if(MOT[MMTrayY].fHasTray==true || MOT[MMTrayY_Car].fHasTray==true)
                        {
                            if(W7C2_FMAIN_CBRUNSTARTMODE->Text==StartModeName[rsmContinuStart] || W7C2_FMAIN_CBRUNSTARTMODE->Text==StartModeName[rsmContinuRetest])
                            {
                                if(CosFunction.bUseOpenCloseSiteMapAtAnyTime==true && bSiteMappingNeedCheck==false)     //Ifor 20190308 : add 隨時開關 Site Mapping
                                {
                                }
                                else
                                {
                                    SetRunStartMode(rsmAutoSiteMap);
                                }
                            }
                        }
                    }
                    bIsASMAutoOneCycle=false;
                    RecordProcess("ONE CYCLE Finished by Auto Site Mapping.");  //Steven 20130618 : 紀錄Auto Clean
                }
            }
            else
            {
                SetInArmSpeed(false, true);                                     //jou 2014-10-08 One Cycle後要重置 Auto Speed down Speed
                if(bTrySuckHotPlateOneCycle==true &&                            //ChungHung 20120206 Hotplate check
                   LastSet.iTemperature==Tempture_Hot)
                {
                    bHotPlateCheckNeedTrayFeed=false;
                    bOneTimeHotPlateCheckAll=false;
                    bTrySuckHotPlateOneCycle=false;
                    bNeedTrySuckHotPlate=false;                                 //ChungHung 20120206 Hotplate check
                    ret=K_RETRY;
                    ShowMyMessage("Plase Check Hotplate First", "請確認Hotplate");
                }
                else if(bQAModeQuickCleanOut)
                {
                    if(Prod.iQAModeRunType==1)                                  //Steven 20120612 : QA做完後的動作
                    {
                        if(LastSet.iTemperature==Tempture_Hot ||                //Steven 20121016 : QA Mode
                           LastSet.iTemperature==Tempture_AmbientHot)           //kevin 20180811 (Steven) : add 恆溫控制
                        {
                            ret=K_CLEAN_OUT;
                        }
                        else
                        {
                            if(CUSTOMER_CODE==CC_SCS)                           //wei 20121227  4b)After QA Mode finished, Stats required the Tray Feed option only.
                            {
                                ret=ShowErrorMessage("MES1648", K_TRAY_FEED, MMSystem, false);                          //QA Mode finish

                                MOT[MMTrayY].ClearTray(__FUNC__);               //jou 2013-07-19 SCS要求QA mode做完要把Loader汽缸放開
                                Cylinder[C_TrayY_Fixer].Off();
                                Cylinder[C_LoaderEdgePush].Off();
                                Cylinder[C_LoaderUpPress].Off();                //JerryYang 20181120 (Steven) : (Steven) : 獨立控制loader壓tray
                            }
                            else
                            {
                                ret=ShowErrorMessage("MES1648", K_RETRY|K_SKIP|K_CLEAN_OUT|K_TRAY_FEED|K_TRAY_END, MMSystem, false);                            //QA Mode finish
                            }
                            SetRunStartMode(rsmContinuStart);

                            IniConfig.bQAModeFirstIn    =true;
                            LastSet.iTester             =IniConfig.iBackUpTesterMode;

                            if(LastSet.iTester==ON_LINE)                        //Steven 20150713 : 整理LastSet.iTester
                                NewRecordProcess("MES2157", "Change to On_Line", "by OneCycle QAModeQuickCleanOut QAMode");                                     //ChungHung 20140722 add add record
                            else
                                NewRecordProcess("MES2155", "Change to Off_Line", "by OneCycle QAModeQuickCleanOut QAMode");                                    //ChungHung 20140722 add add record

                            ArmSpeed[InArm].bVariModeFIX=IniConfig.bBackUpInArmMode;
                            TrayForm.bAutoFeed          =IniConfig.bBackUpAutoFeed;
                            bQAModeFinishCleanOut       =false;
                            bQAModeQuickCleanOut=false;
                        }
                    }
                    else
                    {
                        ret=K_CLEAN_OUT;
                    }
                }
                else if(bCheckIndex)                                            //Steven 20101116
                {
                    InitialPiggyBackFunction();                                 //Steven 20110725 : 重置Piggy Back的狀態
                    bPiggyBackIndexCheck=true;                                  //jou 2014-09-03 Function State Stop Firt Initital Delay Time
                    W7C1_FMAIN_START("DoOneCycleFinishCheck 2");
                }
              #ifdef DEBUG_DUTONOFF
                else if(fAutomation->bOneCycle)                                 //Steven 20110902 : 自動找蟲蟲模式
                {
                    AnsiString Data[2]={0, 0};
                    fAutomation->CommandProcess("ONECYCLE_REPLY", 1, Data);
                    fAutomation->bOneCycle=false;
                    W7C1_FBARCODE_CHANGE2D();                              //Steven 20160425 : 重置2DSys時間
                    W7C1_FTRAYMAP_CHANGETRAY();                        //wei 20161219 Tray Mapping
                    fMain->Pause("fAutomation");
                }
              #endif
                else
                {
                    if(IniConfig.bUseAutoSiteMapping==true &&
                       IniConfig.bI21EnableASM==true &&
                       bASMFinishOneCycle==false && bCheckIndex==false &&
                       bIsAutoOneCycle==false && bQAModeQuickCleanOut==false)
                    {
                        if(CUSTOMER_CODE==CC_ASE_CL)                            //JerryYang 20250120 : add
                        {
                        }
                        else
                        {
                        if(IniConfig.bI50_EnableAutoSiteMappingTrigger==true &&                                         //Jimmychiu 20230707 : Auto Site Mapping Trigger Function
                           IniConfig.bI50_OnyCycle==true)                       //Steven 20241205 : 修正one cycle後觸發auto site map
                        {
                            if(LastSet.iRunStartMode==rsmAutoSiteMap)           //Steven 20220602 : for Auto Site map
                            {
                                if(LastSet.iTemperature==Tempture_Hot ||
                                   LastSet.iTemperature==Tempture_AmbientHot)
                                {
                                    if(bAutoSiteMapHasPickHP)
                                        iResetSiteMappingStep=1;
//                                    else
//                                        iDoSiteMappingStep=0;
                                }
                                /*
                                else
                                {
                                    iDoSiteMappingStep=0;
                                }
                                iAutoSiteMapCount=-1;                           //Steven 20220811 : 紀錄目前是哪個Shuttle
                                iAutoSiteCurrStep=-1;
                                AdjustShuttlePlaceOrderForASM(true);            //Steven 20250206 : fixed ASM之中作one cycle會造成關arm 1時異常
                                RecordProcess("Reset step of auto site map", "DoOneCycleFinishCheck 1");
                                */
                                fMain->ReStartAutoSiteMapping(true);            //Steven 20250910 : fixed for Auto site map
                                QueueTaskList[266].CheckTaskChange();           //Steven 20220708 : 針對Auto Site Map作紀錄
                            }
                            else
                            {
                                if((W7C2_FMAIN_CBRUNSTARTMODE->Text==StartModeName[rsmContinuStart]) ||
                                   (W7C2_FMAIN_CBRUNSTARTMODE->Text==StartModeName[rsmContinuRetest] &&
                                    IniConfig.bI50_RT==true))
                                {
                                    RecordProcess("Trigger Auto Site Map after OneCycle [I50]");
                                    SetRunStartMode(rsmAutoSiteMap);
                                }
                            }
                        }
                        else if(CosFunction.bUSEJCETSiteMapMode==true ||        //jou 2016-11-02 JCET要求One Cycle後自動切換至Auto Site mapping mode
                                CUSTOMER_CODE==CC_ASE_M)                        //Ifor 20180417 : add ASE_M
                        {
                            if(LastSet.iRunStartMode==rsmAutoSiteMap)           //Steven 20220602 : for Auto Site map
                            {
                                if(LastSet.iTemperature==Tempture_Hot ||
                                   LastSet.iTemperature==Tempture_AmbientHot)
                                {
                                    if(bAutoSiteMapHasPickHP)
                                        iResetSiteMappingStep=1;
//                                    else
//                                        iDoSiteMappingStep=0;
                                }
                                /* else
                                {
                                    iDoSiteMappingStep=0;
                                }
                                iAutoSiteMapCount=-1;                           //Steven 20220811 : 紀錄目前是哪個Shuttle
                                iAutoSiteCurrStep=-1;
                                AdjustShuttlePlaceOrderForASM(true);            //Steven 20250206 : fixed ASM之中作one cycle會造成關arm 1時異常
                                RecordProcess("Reset step of auto site map", "DoOneCycleFinishCheck 2");
                                */
                                fMain->ReStartAutoSiteMapping(true);            //Steven 20250910 : fixed for Auto site map
                                QueueTaskList[266].CheckTaskChange();           //Steven 20220708 : 針對Auto Site Map作紀錄
                            }
                            else
                            {
                                if(bSiteMappingCHKOK==true ||
                                   (CosFunction.bUseOpenCloseSiteMapAtAnyTime==true &&
                                    bSiteMappingNeedCheck==true))               //Ifor 20190308 : add 隨時開關 Site Mapping
                                {
                                    if(MOT[MMTrayY].fHasTray==true ||
                                       MOT[MMTrayY_Car].fHasTray==true)
                                    {
                                        if(W7C2_FMAIN_CBRUNSTARTMODE->Text==StartModeName[rsmContinuStart] ||
                                           W7C2_FMAIN_CBRUNSTARTMODE->Text==StartModeName[rsmContinuRetest])                //Sam 20230314 : RT都需要做 auto site mapping
                                        {
                                            if(CosFunction.bUseOpenCloseSiteMapAtAnyTime==true &&
                                               bSiteMappingNeedCheck==false)    //Ifor 20190308 : add 隨時開關 Site Mapping
                                            {
                                            }
                                            else if(IniConfig.bI50_EnableAutoSiteMappingTrigger==false ||
                                                   (IniConfig.bI50_EnableAutoSiteMappingTrigger==true && IniConfig.bI50_OnyCycle) ||                            //Sam 20250115 : 修正 I50 功能
                                                   (CUSTOMER_CODE==CC_SIGURD_HUKOU && iOneCycleFinishShowMsg>0))        //Sam 20250115 : 矽格湖口 GPIB OneCycle 要強制切 ASM
                                            {
                                                RecordProcess("Trigger Auto Site Map after One Cycle 1");
                                                SetRunStartMode(rsmAutoSiteMap);
                                            }
                                        }

                                        if(CUSTOMER_CODE==CC_ASE_M &&
                                           W7C2_FMAIN_CBRUNSTARTMODE->Text==StartModeName[rsmContinuRetest])
                                        {
                                            if(CosFunction.bUseOpenCloseSiteMapAtAnyTime==true &&
                                               bSiteMappingNeedCheck==false)    //Ifor 20190308 : add 隨時開關 Site Mapping
                                            {
                                            }
                                            else if(IniConfig.bI50_EnableAutoSiteMappingTrigger==false)
                                            {
                                                RecordProcess("Trigger Auto Site Map after OneCycle 2");
                                                SetRunStartMode(rsmAutoSiteMap);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        }
                    }

                    bReOpenGpib=true;                                           //Steven 20101013

                    if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum)              //Steven 20120222 : Real Time CCD 新增One cycle完成時送出@END+動作來停止檢測動作, 要在Alarm之前,給OA喘息的機會
                    {
                        COM2->DoReleaseAndInspEnd();
                    }
                    //---------------------------------------

                    if(iAseOneCycle!=0)                                         //kevin 20150925
                    {
                        iAseOneCycle=0;
                        RespondASECom("@e02110Done");                           //kevin 20150415 回應 ase Onecycle finish
                    }
                    iAddInitStartDelayCT=0;                                     //kevin 20180308 add 動作完成
                    bFinishInitStartDelay =false;                               //kevin 20180308 add 動作完成

                    if(IniConfig.bE61InArmStandbyPosOnLoader)                   //JerryYang 20200206 one cycle, clean out, tray feed時in arm移動到loader
                    {
                        bOneCycleInArmToLoader=true;

                        if(MoveInArm2XYToLoaderWait())
                        {
                            bOneCycleInArmToLoader=false;
                        }
                        else
                        {
                            return;
                        }
                    }

                    if(CosFunction.bTesterLowYieldOneCycle==true &&
                       bTesterLowYieldOneCycle==true)                           //jou 2014-09-23 Tester Low Yield Handler need One Cycle & Alarm
                    {
                        ret=ShowErrorMessage("MES1649", K_RETRY, MMSystem, false);
                    }
                    else if(IniConfig.bOneCycleCanTrayFeed)                     //Steven 20110518
                    {
                        if(IniConfig.bOneCycleNeedPowerOff==true)               //Steven 20110214 : OneCycle時要Poewr Off
                            ret=ShowErrorMessage("MES1640", K_RETRY|K_HOME|K_CLEAN_OUT|K_TRAY_FEED, MMSystem, false);   //one cycle finish   //Steven 20240328 : K_SKIP --> K_HOME
                        else
                            ret=ShowErrorMessage("MES1640", K_RETRY|K_CLEAN_OUT|K_TRAY_FEED, MMSystem, false);          //kevin 20140412 cancel skip one cycle finish
                    }
                    else
                    {
                        if(IniConfig.bOneCycleNeedPowerOff==true)               //Steven 20110214 : OneCycle時要Poewr Off
                        {
                            ret=ShowErrorMessage("MES1640", K_RETRY|K_HOME|K_CLEAN_OUT, MMSystem, false);               //one cycle finish  //Steven 20240328 : K_SKIP --> K_HOME
                        }
                        else
                        {
                            if(IniConfig.bEnable_SECS_GEM==true &&              //JerryYang 20190709 只判斷客戶功能就好      //wei 20150817 SECSGEM 斷線 顯示MES1650
                               CosFunction.bSECS_GEM_OneCycle && IniConfig.bSECS_GEM_OneCycle &&
                               bSECSGEMConnectionFailOneCycle)
                            {
//                                #ifndef SOFT_SIMULTE
                                if(bSECSGEMConnectionFail)
                                    ret=ShowErrorMessage("MES1650", K_RETRY, MMSystem, false);                          //kevin 20140412 cancel skip one cycle finish
                                else
                                    ret=K_RETRY;
                                bSECSGEMConnectionFailOneCycle=false;           //wei 20150825 SECSGEM Connection Fail One Cycle
//                                #endif
                            }
                            else if(CosFunction.bYieldAlmNeedOneCycle && bNeedOneCycleByYieldAlm)                       //JerryYang 20161121 簡化Yied alarm的判斷
                            {
                                MyDBIProcess("Process", "One Cycle Finished, after yield alarm");
                                bNeedOneCycleByYieldAlm=false;
                                W7C2_FYIELD_CLEARCOUNT();            //JerryYang 20160401 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                                for(int i=0; i<W7C2_FMAIN_SLLOWYIELD->Count; i++)
                                {
                                    AlarmCode=W7C2_FMAIN_SLLOWYIELD->Strings[i].SubString(0, W7C2_FMAIN_SLLOWYIELD->Strings[i].AnsiPos(",")-1);
                                    ErrPart=W7C2_FMAIN_SLLOWYIELD->Strings[i].SubString(W7C2_FMAIN_SLLOWYIELD->Strings[i].AnsiPos(",")+1, W7C2_FMAIN_SLLOWYIELD->Strings[i].Length()-1);
                                    ShowErrorMessage(AlarmCode, 0, MMInterface, 0, ErrPart);
                                }
                                W7C2_FMAIN_SLLOWYIELD->Clear();
                                if(IniConfig.bI09LowYieldOneCycleDontCleanShuttle &&                                    //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
                                  (FLCarryKit.UseSiteHasIC() ||
                                   BLCarryKit.UseSiteHasIC() ||
                                   InArmSuck.HasIC()))
                                {
                                    bI09_NeedOneCycleAgain=true;
                                }
                            }
                            else if(CosFunction.bContactAlmNeedOneCycle &&      //Sam 20241226 : Contact Alarm 需要先做 OneCycle
                                    (bNeedOneCycleByContactWar1 ||
                                     bNeedOneCycleByContactWar2 ||
                                     bNeedOneCycleByContactAlm1 ||
                                     bNeedOneCycleByContactAlm2))
                            {
                                AnsiString sMsg="";
                                MyDBIProcess("Process", "One Cycle Finished, after contact alarm");
                                if(bNeedOneCycleByContactWar1)
                                {
                                    sMsg.sprintf("Head1 Contact Count Over %d Warnning", W7C2_LS_iContactCT[0]);
                                    ShowMyMessage(sMsg,"Please Check indium");
                                }

                                if(bNeedOneCycleByContactWar2)
                                {
                                    sMsg.sprintf("Head2 Contact Count Over %d Warnning", W7C2_LS_iContactCT[1]);
                                    ShowMyMessage(sMsg,"Please Check indium");
                                }

                                if(bNeedOneCycleByContactAlm1)
                                {
                                    sMsg.sprintf("Head1 Contact Count Over %d Alarm", W7C2_LS_iContactCT[0]);
                                    ShowMyMessage(sMsg,"Please Check indium");
                                }

                                if(bNeedOneCycleByContactAlm2)
                                {
                                    sMsg.sprintf("Head2 Contact Count Over %d Alarm", W7C2_LS_iContactCT[1]);
                                    ShowMyMessage(sMsg,"Please Check indium");
                                }
                                bNeedOneCycleByContactWar1=false;
                                bNeedOneCycleByContactWar2=false;
                                bNeedOneCycleByContactAlm1=false;
                                bNeedOneCycleByContactAlm2=false;
                            }
                            else if(CosFunction.bPickerLifeAlmNeedOneCycle &&   //AI(ht9045-config) 20260521 (RogerYang) : SCC吸嘴壽命報警OneCycle優化
                                    bNeedOneCycleByPickerLifeAlm)
                            {
                                AnsiString sMsg="";
                                MyDBIProcess("Process", "One Cycle Finished, after picker life alarm");
                                sMsg="Picker suck count over limit, please clear in [In/out arm picker] tab";
                                ShowMyMessage(sMsg, "Picker Life Alarm");
                                MyDBIProcess("Process", sMsg.c_str());
                                bNeedOneCycleByPickerLifeAlm=false;
                            }
                            else if(bRTCModelNG)                                //wei 20170504 (Steven) RTC Model NG
                            {
                                ret=ShowErrorMessage("MES1654", K_RETRY, MMSystem, false);
                            }
                            else if((CUSTOMER_CODE==CC_KYEC_LEE &&
                                     USE_AUTO_RETEST==eartInstall &&
                                     IniConfig.bA10_AutoReTest &&
                                     (LastSet.iRunStartMode==rsmInitial_ART ||
                                      LastSet.iRunStartMode==rsmContinuStart_ART ||
                                      LastSet.iRunStartMode==rsmContinuRetest_ART)))                                    //wei 20150821 KYEC在ART模式退Pass Bin //Ifor 20191127 :移至最後面避免Yield相關的Alarm無法顯示
                            {
                                ret=ShowErrorMessage("MES1640", K_RETRY|K_CLEAN_OUT|K_TRAY_END, MMSystem, false);
                                bPassTrayFeed=true;
                            }
                            else if(W7C2_FAUTOTEACH_ISRUN())                        //JimmyChiu 20211020 : Auto alignment mode
                            {
                                W7C2_FMAIN_BTNPAUSECLICK();
                                ret=K_RETRY;
                            }
                            else if(bOneCycleAfterSHLossIC)                     //KaiChen 20200304 ：矽格-湖口，要求OutShuttle Loss IC 時機台上的IC放到R道
                            {
                                bOneCycleAfterSHLossIC=false;
                                ret=ShowErrorMessage("MES1640", K_RETRY, MMSystem, false);
                                ShowMyMessage("Please Check IC On Machine");
                            }
                            else
                            {
                                if(TestIF_File.bAutoOnecycleHomStart==true &&
                                   bIsAvoidIndexShiftOneCycle==true &&
                                    CUSTOMER_CODE==CC_GIGAS)                    //Isaac 20210821 : 全智要求定時onecycle回home,start
                                {
                                    bIsAvoidIndexShiftOneCycle=false;
                                    bHomeByStart=true;
                                    fAllMotorHome=false;
                                }
                                else if(CUSTOMER_CODE==CC_ASE_KaohSiung)        //kevin 20201116
                                {
                                    ret=ShowErrorMessage("MES1640", K_RETRY|K_CLEAN_OUT, MMSystem, false);              //kevin 20140412 cancel skip one cycle finish
                                    fMain->Pause("DoOneCycleFinishCheck MES1640");
                                }
                                else
                                {
                                    #ifdef DEBUG_OneCycleContinous              //Sam 20221124 : 新增連續 OneCycle 功能 Debug
                                    if(fMain->chkOneCycleContinous->Checked==true)
                                    {
                                        ret=K_RETRY;
                                    }
                                    else
                                    {
                                        ret=ShowErrorMessage("MES1640", K_RETRY|K_CLEAN_OUT, MMSystem, false);          //kevin 20140412 cancel skip one cycle finish
                                    }
                                    #else
                                    if(LastSet.iRunStartMode==rsmQAMode && bQAModeFinishCleanOut)                       //JerryYang 20221004 : Maxim版本QA mode
                                    {
                                        ret=ShowErrorMessage("MES1640", K_CLEAN_OUT, MMSystem, false);                  //kevin 20140412 cancel skip one cycle finish
                                    }
                                    else
                                    {
                                        ret=ShowErrorMessage("MES1640", K_RETRY|K_CLEAN_OUT, MMSystem, false);          //kevin 20140412 cancel skip one cycle finish
                                    }
                                    #endif
                                }
                            }

                            if(TestIF_File.bEnableReadAndCheckTorque)           //kevin 20210804
                            {
                                bResetArm1Value=true;
                                bResetArm2Value=true;
                            }
                            bLowYeildAlarm=false;                               //wei 20151116 Low Yield Onecycle中不alarm
                            iallSitCount=0;                                     //kevin 20180721 add clean
                        }
                        ReadWriteBinCountMode(false);                           //kevin 20210825 寫 Bin 1  Bin 2...記錄
                    }

                    bASMFinishOneCycle=false;                                   //Steven 20230117 : 修正One Cycle之後不執行Auto Site Map的問題

                    if(CosFunction.bPiggyBackShowMainForm)                      //Steven 20131101 : PiggyBack數量到達時,顯示在Main Form上面
                    {
                        bContinuoussPass=true;
                        bContactCounOven=true;
                        fMain->CleanYieldCount();                               //kevin 20131009 清除yield 計數
                    }

                    if(CosFunction.bRTCAutoModelVerify==true &&
                       IniConfig.bD36EnableRTCAutoModelVerify==true &&
                       IniConfig.bD36_2AfterOneCycleNeedAutoVerify==true &&
                       bNeedWaitRTCAutoVerify==false)
                    {
                        bRTCAutoModelVerifyFirstTime=true;                      //JerryYang 20201210 one cycle時要觸發 RTC auto verification
                    }

                    bPiggyBackIndexCheck=true;                                  //jou 2014-09-03 Function State Stop Firt Initital Delay Time
                    bInitialMaxTime=true;                                       //jou 2011-11-09 增加initial max time set
                    iWhoTriggerPiggyBack=pbtOneCycle;                           //Steven 20111207 : 誰觸發了Piggy Back

                    W7C2_FCOUNTER_WRITECTINFO();                               //jou 2012-10-15 one cycle 寫入一次histroy contact,避免中途被不正常關閉程式
                    W7C1_FBARCODE_CHANGE2D();                              //Steven 20160425 : 重置2DSys時間
                    W7C1_FTRAYMAP_CHANGETRAY();                        //wei 20161219 Tray Mapping
                    if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&          //JerryYang 20180518 : add
                       TestIF_File.bArm1PickPlaceArm2Test==true)                //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                        W7C2_BLCARRYKIT_SETHASNULLIC();
                }

                InitialPiggyBackFunction();                                     //Steven 20110725 : 重置Piggy Back的狀態
                bCheckGiveWay=false;                                            //Ifor 20200521 Fix:清除RTC旗標
                bNeedCheckRTCReport=false;                                      //Ifor 20200521 Fix:清除RTC旗標
                bBinDispAlarm=false;                                            //Ifor 20220714 add:Bin Disp 異常報警 每次Onecycle 檢查一次

                if(bBackupCleanOut==true)                                       //jou 2010-12-07 : 防止index check oncyle與clean out funtion衝突
                {                                                               //jou 2011-07-06 : 改成任何狀況下,都要紀錄clean out模式,不然會hang up
                    bBackupCleanOut=false;                                      //Steven 20260612 : Fix == to = (was comparison, not assignment)
                    ret=K_CLEAN_OUT;
                }

                bOneTimeWait=true;
                W7C2_FCONTACT_RGHANDLER_ENABLE();

                bResetMode=false;                                               //jou 981219 start : reset mode
                bLampReset=false;
                bSecondTimeLoaderCheckNoTray=false;                             //ChungHung 20130528 SCK要求AutoClean後要自動檢測是否Loader有補Tray
                bLoaderNoTrayAutoCleanOut=false;                                //jou 2010-09-01 Loader沒有tray時,Loader不再進Tray,Tran End時自動clean out
            }

            if(CosFunction.bUseOpenCloseSiteMapAtAnyTime==true)                 //Steven 20230117 : One Cycle後要取消打勾
            {
                W7C2_FMAIN_DISABLESITEMAP(false, __FUNC__);                //Steven 20230130 : 加上紀錄
            }

            ReadWriteBinCountMode(false);                                       //kevin 20210906 寫 Bin 1  Bin 2...記錄
            SW[SwPurgeAir].Off();                                               //kevin 20180928 (Steven) : add blower load board
            iContractCount=0;                                                   //kevin 20180928 (Steven) : add blower load board

//            fProductionInfo->DoOneCycleFinishShowMsg();                         //JimmyChiu 20211020 : Auto alignment mode

            if(ret==K_RETRY)                                                    //jou 2010-06-25 start : 解除mark，OneCycle後不能直接按CleanOut
            {                                                                   //jou 2010-07-27 one cycle後,會將unload的tray盤釋放,方便OP取tray盤.
//                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                           //kevin 20131101 add    //Steven 20220623 : Mark
                {
                    if(IniConfig.bI40_bStartProductOnLine &&
                       LastSet.iTester==OFF_LINE && AccessLevel<1)              //kevin 20180517 change config setup
                    {                                                           //kevin20140621 卡 OFF_LINE模式 operatpr 變成 ON_LINE
                        iOff_LINE_Mode=1;
                        bOneCycleOperateChangeON_line=true;                     //kevin 20140411
                        W7C2_FMAIN_CHANGETESTERCONNECT(10);                         //切換 on-line
                    }
                }

                if(IniConfig.bEnableUnloadTrayFree)                             //kevin 20180716 add ASE_KH no use  onecycle 釋放所有軌道夾TRAY汽缸
                    AutoTrayCylinderFree();                                     //jou 2010-01-25 start : 釋放Auto Tray上的汽缸
            }
            else if(ret==K_CLEAN_OUT)
            {
                if(bQAModeQuickCleanOut==false)                                 //Steven 20111019
                {
                    if(IniConfig.bOneCycleDoQuickCleanOut==true)                //Steven 20110524
                    {
                        bOnecycleTrayFeed=true;                                 //Steven 20110525
                        bNeedQuickCleanOut=TrayForm.bAutoFeed;
                        TrayForm.bAutoFeed=false;
                    }
                }

                if(IniConfig.bI40_bStartProductOnLine &&                        //kevin 20180517 change config setup 生產前OP OFF_LINE 強制 On line
                   LastSet.iTester==OFF_LINE)
                {
                    TrayForm.bAutoFeed=true;                                    //kevin 20140412 off_line 需將所有IC清出
                }
                bCleanOut=true;
            }
            else if(ret==K_SKIP ||
                    ret==K_HOME)                                                //Steven 20240328 : K_SKIP --> K_HOME
            {
                if(IniConfig.bOneCycleNeedPowerOff==true)                       //Steven 20110214 : OneCycle時要Poewr Off
                {
                    W7C2_FHOME_SERVOOFF("DoOneCycleFinishCheck");          //Steven 20230712 : 修正SwServoOn.Off時, 要抓住Z煞車
                    RecordProcess("K_SKIP");                                    //kevin 20180319 add log
                }
            }
            else if(ret==K_TRAY_FEED)
            {
                bOnecycleTrayFeed=true;                                         //Steven 20110525
                bNeedTrayFeed=true;
                tUPH_PauseTime=0;
            }
            else if(ret==K_TRAY_END)                                            //jou 2010-12-15
            {
                if(IniConfig.bCleanOutCanTrayEnd)
                {
                    bOnecycleTrayFeed=true;                                     //Steven 20110525
                    bNeedTrayFeed=true;
                    tUPH_PauseTime=0;
                    iTrayFeed=1;
                    iTrayFeedTask=1;
                    bCleanOutTrayEnd=true;
                    bQAModeTrayEnd=true;
                }
            }
        }
        else
        {
            if(bDoEmptySocketOneCycle)                                          //Steven 20201022 : For RFMD Empty Socket Check Funstion.
            {
                bDoEmptySocketOneCycle=false;
                bResetMode=false;
                InitialIndexSocketCheckTask();
                bDoEmptySocketCheck=true;

                RecordProcess("EmptySocketOneCycle Finsih");
                RecordProcess("Start EmptySocketCheck");
            }
            else if(bResetMode==true)
            {
                MyDBIProcess("Process", "Do reset finish.");                    //jou 2010-11-23
                InitialPiggyBackFunction();                                     //Steven 20110725 : 重置Piggy Back的狀態
                bResetMode=false;
                bLampReset=false;
            }
            bBackupCleanOut=false;                                              //jou 2010-12-07 : 防止index check oncyle與clean out funtion衝突
        }

        if(iClearSocketFunction==1)
        {
            iClearSocketFunction=2;
            iClearSocketFunctionTask=1;
        }
        _bHomeNeedOnecycle=false;
        fMain->DebugOneCycleHotPlate("OneCycleFinish");                         //Sam 20210915 : 增加 OneCycle Hotpalte Debug Log
        iOneCycle=0;
        InitOneCycle("One Cycle Finish", bQAModeTrayEnd);

        if(EmptySocketCheckModeBeUse() && IniConfig.bI41_3_AfterContactorTeminated)
        {
            InitialIndexSocketCheckTask();
            bDoEmptySocketCheck=true;
            bDoEmptySocketOneCycle=false;
            bIsAutoOneCycle=false;                                              //ChungHung 20151012 modify again RFMD Empty Socket Check
            RecordProcess("Start EmptySocketCheck for AfterContactorTeminated");                                        //ChungHung 20160803 add for RFMD add EmptyScoket log
        }
        else if(IniConfig.bI41EnableEmptySocketCheck && bDoEmptySocketOneCycle==true)
        {
            InitialIndexSocketCheckTask();
            bDoEmptySocketCheck=true;
            bDoEmptySocketOneCycle=false;
            bIsAutoOneCycle=false;
            RecordProcess("Start EmptySocketCheck for EmptySocketOneCycle Finish");                                     //ChungHung 20160803 add for RFMD add EmptyScoket log
        }

        if(bBackupCleanOut==true)                                               //jou 2012-05-03 start : Clean Out到最後一顆,準備放到shuttle時,這時候按home,home完會繼續放IC,未把clean out做完,Hang up
        {                                                                       //此段必須放在 InitOneCycle()之後, if(bCleanOut ) 之前
            bBackupCleanOut=false;
            bCleanOut=true;
        }

        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20220927 : Jovie要求要跳ONE CYCLE FINISH並開門按Z1
        {
            if(bOutShtLoseICNeedSetErrBin || bIndexDropICNeedSetErrBin)         //JerryYang 20220923 : index arm drop error設ERROR BIN
            {
                ShowErrorMessage("MES1640", K_RETRY, MMSystem, false);
                bOutShtLoseICNeedSetErrBin=false;
                bIndexDropICNeedSetErrBin=false;
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<MAX_Index_Col; j++)
                    {
                        bTestSiteNeedSetErrBin[i][j]=false;
                    }
                }
            }
        }

        fMain->BtnOneCycle->Down=false;
        InitialTestHeadMotorTask();
        if(AsebLoadCellTest)
        {
            AsebLoadCellTest = false;
            bLoadCellTest = true;                                               //kevin 20190305 add one cycle run arm 1 arm 2 load cell test
        }
        SW[SwFKOneCycle].Off();
        SW[SwRKOneCycle].Off();
        MOT[MTestY1].MovFlag=false;
        MOT[MTestY2].MovFlag=false;
        MOT[MTestZ1].MovFlag=false;
        MOT[MTestZ2].MovFlag=false;
        SetInitialICCheck();
        bLampOneCycle=false;
        bIndexPickUpErrorWaitRetry=false;                                       //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
        iOneCycleFinishShowMsg=0;                                               //Sam 20250115 : 矽格湖口 GPIB OneCycle 要強制切 ASM

        if(TestIF_File.iTestType==TTL_MODE && (TTL_CARD_TYPE==2 || TTL_CARD_TYPE==3))
            SendTTLRS232CSOTsignal();

        if(bCleanOut || bCleanoutStart)                                         //kevin 20130506
        {
            fMain->CleanOut("DoOneCycleFinishCheck");
        }

        if(bCleanHotplate_ART==2 &&LastSet.iTemperature==Tempture_Hot)
        {
            bCleanHotplate_ART=3;                                               //kevin 20150722
            bMustCleanAllTray=false;
        }
        else
        {
            bCleanHotplate_ART=0;                                               //kevin 20150722
        }

        if(IniConfig.bQAMode==true && bQAModeQuickCleanOut==true)               //Steven 20111019
        {
            W7C1_FMAIN_START("DoOneCycleFinishCheck 3");
        }

        InitialPiggyBackFunction();                                             //Steven 20110725 : 重置Piggy Back的狀態

        if(bNeedTrayFeed)                                                       //Steven 20110518
        {
            if(bCleanOutTrayEnd)
                W7C2_FMAIN_INITIALTRAYFEED("DoOneCycleFinish_TrayEnd");
            else
                W7C2_FMAIN_INITIALTRAYFEED("DoOneCycleFinish_TrayFeed");
        }

//        bChangeSiteMAP=true;                                                  //kevin 201300506 可以改變sitemap
        if(IniConfig.bI09LowYieldOneCycleDontCleanShuttle &&
           bI09_NeedOneCycleAgain)                                              //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
        {
            iWhoTriggerPiggyBack=pbtOneCycleRemainSht;
            ProcessPiggyBackFunction();
        }

        if(bNeedOneCycleByAutoAlignment==false)                                 //KenHsieh 20211202 : 修改重新打開AOA後，先做Onecycle再回Home重做AOA
        {
            VerifyNeedDoAlignment(AutoAlignmentTray_AfterOneCycle, AutoAlignmentCK_AfterOneCycle);                      //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
        }
        else
        {
            bNeedOneCycleByAutoAlignment=false;
            fAllMotorHome=false;
            W7C1_FMAIN_START("DoOneCycleFinishCheck 4");
        }
        #ifdef DEBUG_OneCycleContinous                                          //Sam 20221124 : 新增連續 OneCycle 功能 Debug
        if(fMain->chkOneCycleContinous->Checked==true)
             W7C1_FMAIN_START("DoOneCycleFinishCheck 5");
        #endif
        bTT_SetSpeed_Check=false;                                               //Ztex 2024.08.11 Add Test Time Set Speed
        if(IniConfig.bL46_AStreamErrorCompressOnecycle &&
           iAStreamErrorCompressOnecycle==1)                                    //Ztex 2024.10.01 Add AStream Error Compress Onecycle
        {
            iAStreamErrorCompressOnecycle=2;
        }
    }
    else if(iCleanOut==1)
    {
        if(bDoLoaderCleanOut    ||                                              //kevin 20211106 確認吸取數量完收load tray  //pig 2011.11.08
           bLoadBFBackTray)                                                     //KenHsieh 20230325 : CleanOut時Loader BF 退Tray
        {
            if(DoLoaderTrayFeed()==false)
            {
                return;
            }
            else
            {
                bDoLoaderCleanOut=false;
                bLoadBFBackTray=false;                                          //KenHsieh 20230325 : CleanOut時Loader BF 退Tray
            }
        }
    }
    else
    {
        if(fMain->BtnOneCycle->Down && iOneCycle==0 && bOneCycle_BackUp==false)                                         //ChungHung 20150514 add for 12 site full shuttle //ChungHung 20150213 add fix 2x6 if open munt full issue hangup
        {
            if(bRunAutoSiteMapping==true)                                       //Ifor 20180518 : add 簡化Site Mapping 旗標
            {
                iOneCycle=1;
            }
            else
            {
                fMain->DebugOneCycleHotPlate("DoOneCycleFinishCheck");          //Sam 20210915 : 增加 OneCycle Hotpalte Debug Log
                iOneCycle=1;
            }
        }
    }
}
//------------------------------------------------------------------------------

// W7-C2 INTEGRATED: the former `#undef DoART_AfterCleanOut` was removed together
// with the W7C1 seam alias (csystem.cpp:972 region) -- there is no macro left to
// un-define.  The DoCleanOutFinishCheck call (csystem.cpp:1707) now binds to the
// REAL body below via the csystem.h declaration.  golden csystem.cpp:14049.

// ===========================================================================
//  DoART_AfterCleanOut  -- golden csystem.cpp:14049-14711 (~663 lines).
//  Auto-ReTest (ART) post-clean-out lot-flow decision (returns whether to
//  continue).  Translated FAITHFULLY this wave (W7-C2), REPLACING the W7C1
//  W7C1_DoART_AfterCleanOut seam stub: every bAutoReTest_ART / CosFunction.
//  bAutoRetestGPIBmode / CosFunction.bUseSCKART / HANA-ART / RENESAS FT-CT /
//  SPIL / TSMC branch, the SPBin yield-alarm block, the FT/RT fail-yield model
//  ladder, and the SECOND SOFT_SIMULTE region (the /* #ifdef SOFT_SIMULTE
//  hanaART */ block-comment) are reproduced VERBATIM.
//
//  NO-REGRESSION KEY: DoART_AfterCleanOut is CALLED ONLY by DoCleanOutFinishCheck
//  inside its iCleanOut==1 ART branch (DoART_AfterCleanOut(ret)), whose guards
//  (bAutoReTest_ART / CosFunction.bUseSCKART / CosFunction.bAutoRetestGPIBmode)
//  are all false offline, so this function is UNREACHABLE at runtime offline.
//  Every W7C2 stand-in it touches is therefore inert; the immediate entry
//  else-branch (golden :14506) returns bRet after resetting LastSet ART flags.
// ===========================================================================
bool DoART_AfterCleanOut(int &ret)                                              //return result
{
    static int iFirstIn=0;
    bool bRet=false, bNeedRetest=false;
    int iHanaNeedToRt=0;
    AnsiString S1="";
    LastSet.bCleanOut_ART=false;
    bART_needRT2=false;                                                         //kevin 20150717  RT 完成還需要rt2
    bART_RT2RunNoChangeMode=true;                                               //kevin 20150717  只退fail RT2 不能更改測試模式

    if(bAutoReTest_ART)                                                         //kevin 20150611  自動化要使用 ase-kaosh
    {
        if(W7C2_LS_iAutoRetestCount_ART>=iAutoRetestLimit ||
           LastSet.iRunStartMode==rsmContinuRetest_ART)                         //kevin 20150613  已作各分 tray動作
        {
            if(bCleanHotplate_ART==3 &&
               LastSet.iTemperature==Tempture_Hot)                              //kevin 20150722
            {
                bMustCleanAllTray=true;
                bCleanHotplate_ART=0;
                ret=2;
                ShowMyMessage("Clean hotplate finish !!");
                ret=K_RETRY;                                                    //kevin 20150716
            }
            ret=1;
            bRet=true;                                                          //kevin 20170831
        }
        else
        {
            if(bCleanHotplate_ART==3 &&
               LastSet.iTemperature==Tempture_Hot)                              //kevin 20150722
            {
                bMustCleanAllTray=true;
                bCleanHotplate_ART=0;                                           //kevin 20150722
                ret=2;
                ShowMyMessage("Clean hotplate finish !!");
            }
            else
            {
                RespondASECom("@e02008Done");                                   //kevin 20170830 (Steven) 回應 ase ART Clean out finish
                ret=ShowMyMessageBox_YES_NO("Load No Tray? Yes,Please Print Summary", "Load 沒有tray確定要做 AutoRetest 嗎？,YES ,請結批報表");
            }

            if(ret==1)
                bRet=true;                                                      //kevin 20150717
            else
                ret=K_RETRY;                                                    //kevin 20150716
        }
    }
    else
    {
        if(CosFunction.bAutoRetestGPIBmode==true)                               //jou 2015-10-02 Auto Retest GPIB mode
        {
            if(LastSet.iTemperature==Tempture_Hot)
            {
                if(MOT[MMPlate1].HasIC()==false && MOT[MMPlate2].HasIC()==false &&
                   (MOT[MMTrayZ].fHasTray==true || MOT[MMTrayY].fHasTray==true || MOT[MMTrayY_Car].fHasTray==true))
                {
                    if(W7C2_LS_bBreakSCKART)                                    //Sam 20200311 : Fix TCP ART MODE
                    {
                    }
                    else
                    {
                        iCleanOut=0;
                        ShowMyMessage("Clean hotplate finish !!");
                        return false;
                    }
                }
            }

            if(W7C2_LS_bEndLotAutoRetestGPIB==false)
            {
                iSCKARTLoadingStatus=fSCKART->CheckLoadingCount();
                if(CUSTOMER_CODE==CC_PTI &&                                     //Sam 20240809 : PTI ART 模式
                   IniConfig.bB03_TesterReport &&
                   (fSCKART->iFTRTCount==0 || TestIF_File.iSCKART_TryCnt<fSCKART->iFTRTCount))
                {
                                                                                //第一次 FT 不用檢查數量 or 測試完再拿來回重測
                }
/*                #ifdef SOFT_SIMULTE
                else if(fMain->hanaART->IsHanaArtAvailable()==true)             //Steven 20250415 : mark
                {
                    //
                }
                #endif*/
                else if(TestIF_File.bSCKART_LotDeviceCheck &&                   //RogerYang 20250918 : 瑞薩FT-CT 不要進入，這時候如果收到RT會死(已經更新新的數量)，數量卡控交給FT-CT判斷
                        TestIF_File.bRENESAS_EnableFTCT==false)
                {
                    if(iSCKARTLoadingStatus==2)
                    {
                        if(W7C2_SCKART->DoChkInputCntAlarm(true)==true)             //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定
                        {
                            ShowErrorMessage("WAR0120", K_RETRY, MInArmX, false, "DoCleanOutFinishCheck 2");
                            return false;
                        }
                    }
                    else if(iSCKARTLoadingStatus==0)
                    {
                        if(W7C2_SCKART->DoChkInputCntAlarm(false)==true)            //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定
                        {
                            ret=ShowErrorMessage("WAR0119", K_RETRY|K_SKIP, MInArmX, false, "DoCleanOutFinishCheck 2");
                            if(ret==K_RETRY)
                            {
                                iCleanOut=0;
                                bLoaderNoTrayAutoCleanOut=false;                //Steven 20170320 (wei) : fixed for check loader tray again
                            }
                            return false;
                        }
                    }
                }

                if((fSCKART->iInputJamCnt!=0 ||
                   W7C2_SCKART->iOutputJamCnt!=0) &&                                //Steven 20170320 (wei) : Must after check load count
                    TestIF_File.bRENESAS_EnableFTCT==false)                     //RogerYang 20251014 : 瑞薩FT-CT 不要進入
                {
                    ret=ShowErrorMessage("WAR0121", K_RETRY|K_SKIP, MMSystem, false, "DoCleanOutFinishCheck 2");
                    if(ret==K_RETRY)
                    {
                        iCleanOut=0;
                        bLoaderNoTrayAutoCleanOut=false;                        //Steven 20170320 : fixed for check loader tray again
                        return false;
                    }
                    else
                    {
                        return false;                                           //Steven 20170331 (wei) : Add for re-check count
                    }
                }

                if(CosFunction.bUseSCKART)                                      //Steven 20161201 (wei) : For SCK 93K ART
                {
                    if(fMain->hanaART->IsHanaArtAvailable()==true)              //Steven 20250415 : HANA ART Function
                    {
                        if(W7C2_HANAART->IsContactAvailable())
                        {
                            W7C2_LS_bFirstTestAutoRetestGPIB=false;             //Steven 20260612 : Fix == to = (was comparison, not assignment)
                            if(W7C2_HANAART->IsPrimeTest())
                                W7C2_HANAART->EndPrimeTest();
                            else
                                W7C2_HANAART->EndReTest();
                            return false;
                        }
                        else
                        {
                            iHanaNeedToRt=W7C2_HANAART->NeedToRT();           //0:Waiting command, 1:Need to RT, 2:Lot End
                            if(iHanaNeedToRt==0)
                                return false;
                        }
                        W7C2_SCKART->CheckNeedRT();
                    }
                    else if(TestIF_File.bRENESAS_EnableFTCT==true)              //RogerYang 20250915 : 瑞薩 FT-CT
                    {
                        if(W7C2_RENESAS->Check71CommandDuring50()==false)                                      //RogerYang 20251019 : FT-CT "send 71"
                            return false;
                    }
                    else
                    {
                        if(iFirstIn==0)
                        {
                            W7C2_SCKART->CheckNeedRT();
                            iFirstIn++;
                        }
                    }

                    if(W7C2_SCKART->iNeedRT>0)
                    {
                        W7C2_LS_bFirstTestAutoRetestGPIB=true;
                        S1.sprintf("iNeedRT=%d, iFTRTCount=%d, iInputCount=%d, dCurrYield=%f", W7C2_SCKART->iNeedRT, fSCKART->iFTRTCount, fSCKART->iInputCount, W7C2_SCKART->dCurrYield);
                        RecordProcess("Need RT.", S1);

                        if(CosFunction.bART_SECSGEM_93K==true)
                        {
                            W7C2_SCKART->SaveTestSummary(1);                        //JerryYang 20220923 : SECS GEM版本ART
                        }
                        else
                        {
                            W7C2_SCKART->SaveTestSummary(0);                        //Steven 20190521 : ATK lot count
                        }
                        W7C2_SCKART->DoAutoSocketOff(false);

                        if(IniConfig.bB03_TesterReport)                         //Sam 20240809 : PTI ART 模式
                        {
                            if(fSCKART->iFTRTCount<=1)
                            {
                                W7C2_SCKART->UpdateCount();
                                W7C2_SCKART->iLotCount  =atoi(W7C2_SCKART->palOutputCnt->Caption.c_str());
                                fSCKART->iInputCount=atoi(W7C2_SCKART->palRejectCnt->Caption.c_str());
                                W7C2_SCKART->AccessFile(false, 1);
                            }
                            W7C2_FLOTINFO_PRODTESTERREPORT();
                            W7C2_FLOTINFO_CBRUNMODE_ITEMINDEX(fSCKART->iFTRTCount);
                        }
                    }
                    else
                    {
                        W7C2_LS_bFirstTestAutoRetestGPIB=false;
                        W7C2_SCKART->SaveTestSummary(1);                            //Steven 20190521 : ATK lot count
                        W7C2_SCKART->DoAutoSocketOff(true);
                        S1.sprintf("iNeedRT=%d, iFTRTCount=%d, iInputCount=%d, dCurrYield=%f", W7C2_SCKART->iNeedRT, fSCKART->iFTRTCount, fSCKART->iInputCount, W7C2_SCKART->dCurrYield);
                        RecordProcess("Final lot end.", S1);                    //Steven 20190722 : add TSV log
                    }
                }

                if(CosFunction.bUseSCKART &&
                   TestIF_File.bSCKART_EnableSPBinAlarm==true &&
                   bSPBinYieldAlarm==true &&                                    //Isaac 20171113 (Steven) : add ATK Special Bin Yield alarm
                   W7C2_LS_bFirstTestAutoRetestGPIB==true &&
                   fSCKART->iFTRTCount<=1)                                      //Only FT need to check
                {
                    S1.sprintf("Category %d count over limit %2.1f%% ", Prod.iSCKART_SPBinSelect, Prod.dSCKART_SPBinAlarmYield);
                    ret=ShowErrorMessage("WAR07360", K_TRAY_FEED|K_RETRY, MMInterface, 0, S1);

                    if(ret==K_RETRY)                                            //Retry 繼續流程
                    {
                        W7C2_LS_bFirstTestAutoRetestGPIB=false;
                        W7C2_FMAIN_SETLOTSTATE(8);                                  //ART Low Yield Lot End
                    }
                    else                                                        //TRAY_FEED 不做RT,直接tray feed
                    {
                        W7C2_LS_iSCKART_RTUnitCount=0;
                        W7C2_SCKART->CheckNeedRT();
                        W7C2_FMAIN_SETLOTSTATE(10);                                 //ART Low Yield Final Lot End
                    }
                }
                else if(CosFunction.bUseSCKART &&
                        TestIF_File.iSCKART_RTStartMode==1 &&
                        W7C2_LS_bFirstTestAutoRetestGPIB==true)
                {
                    if(CosFunction.bART_SECSGEM_93K &&                          //JerryYang 20220923 : SECS GEM版本ART
                       (W7C2_SCKART->iTesterType==1 ||
                        W7C2_SCKART->iCurrentStatus!=W7C2_SCKART->iLOTSTATUS_A))
                    {
                        W7C2_FMAIN_CLARNDATA(2, "ART_LOTRETESTCLEARED");

                        if(LastSet.iTester==OFF_LINE)
                        {
                            if(BinSelect[OffT].bAutoRetest[eAuto1])
                                W7C2_LS_BinCT[0][e3Auto1]=0;
                            if(BinSelect[OffT].bAutoRetest[eAuto2])
                                W7C2_LS_BinCT[0][e3Auto2]=0;
                            if(BinSelect[OffT].bAutoRetest[eAuto3])
                                W7C2_LS_BinCT[0][e3Auto3]=0;
                        }
                        else
                        {
                            if(BinSelect[FT].bAutoRetest[eAuto1])
                                W7C2_LS_BinCT[0][e3Auto1]=0;
                            if(BinSelect[FT].bAutoRetest[eAuto2])
                                W7C2_LS_BinCT[0][e3Auto2]=0;
                            if(BinSelect[FT].bAutoRetest[eAuto3])
                                W7C2_LS_BinCT[0][e3Auto3]=0;
                        }

                        for(int i=0; i<10; i++)
                        {
                            W7C2_LS_lSCKARTBinCT[i]=0;
                        }
                        LastSet.iSCKARTInputCT=0;
                        LastSet.lShuttleCount=0;
                        W7C2_LOTSUMMARY_CLEARRTDATA();

                        WriteLastDataFile(false);                               //kevin 20141030
                        W7C2_FSORTCT_SHOWLOADING();
                        W7C2_FSORTCT_SHOWSORT();

                        if(W7C2_SCKART->iTesterType==0)
                        {
                            fSCKART->iInputCount=W7C2_LS_iSCKART_RTUnitCount;
                            W7C2_LS_iSCKART_RTUnitCount=0;
                        }
                        fSCKART->iInputJamCnt    =0;
                        W7C2_SCKART->iOutputJamCnt   =0;

                        if(W7C2_SCKART->iCurrentStatus==W7C2_SCKART->iLOTSTATUS_R)
                        {
                            W7C2_SCKART->SetLotStatus(W7C2_SCKART->iLOTSTATUS_W);
                            W7C2_SCKART->iWaitGPIBLotR=3;
                        }
                        W7C2_SCKART->AccessFile(false, -1);
                        RecordProcess("ART LOTRETESTCLEARED.");
                        W7C2_FLOTINFO_CLEARBARCODE();                  //Steven 20190214 : 統一清除2DID方式
                    }

                    if(IniConfig.bSPILFunction==true && bStopART==true)         //JerryYang 20220923 : SECS GEM版本ART
                    {
                        ret=K_TRAY_FEED;
                        bStopART=false;
                    }
                    else if((CUSTOMER_CODE==CC_PTI &&
                            IniConfig.bB03_TesterReport) ||                     //Sam 20240809 : PTI ART 模式
                            TestIF_File.bRENESAS_EnableFTCT==true)              //RogerYang 20250922 : 瑞薩FT-CT
                    {
                        ret=K_RETRY;
                    }
                    else
                    {
                        ret=ShowErrorMessage("MES1655", K_TRAY_FEED|K_RETRY, MMSystem, 0);      //ART Loading devices finish
                    }

                    if(ret==K_RETRY)                                            //Retry 繼續流程
                    {
                        W7C2_LS_bFirstTestAutoRetestGPIB=false;
                        W7C2_FMAIN_SETLOTSTATE(8);                                  //ART FT Lot End when loading devices finish
                    }
                    else                                                        //TRAY_FEED 不做RT,直接tray feed
                    {
                        W7C2_LS_iSCKART_RTUnitCount=0;
                        W7C2_SCKART->CheckNeedRT();
                        W7C2_FMAIN_SETLOTSTATE(10);                                 //ART Final Lot End when loading devices finish
                    }
                }
                else
                {
                    if(CosFunction.bART_SECSGEM_93K &&                          //JerryYang 20220923 : SECS GEM版本ART
                       (W7C2_SCKART->iTesterType==1 ||
                        W7C2_SCKART->iCurrentStatus!=W7C2_SCKART->iLOTSTATUS_A))
                    {
                        W7C2_FMAIN_CLARNDATA(2, "ART_LOTRETESTCLEARED");

                        if(LastSet.iTester==OFF_LINE)
                        {
                            if(BinSelect[OffT].bAutoRetest[eAuto1])
                                W7C2_LS_BinCT[0][e3Auto1]=0;
                            if(BinSelect[OffT].bAutoRetest[eAuto2])
                                W7C2_LS_BinCT[0][e3Auto2]=0;
                            if(BinSelect[OffT].bAutoRetest[eAuto3])
                                W7C2_LS_BinCT[0][e3Auto3]=0;
                        }
                        else
                        {
                            if(BinSelect[FT].bAutoRetest[eAuto1])
                                W7C2_LS_BinCT[0][e3Auto1]=0;
                            if(BinSelect[FT].bAutoRetest[eAuto2])
                                W7C2_LS_BinCT[0][e3Auto2]=0;
                            if(BinSelect[FT].bAutoRetest[eAuto3])
                                W7C2_LS_BinCT[0][e3Auto3]=0;
                        }

                        for(int i=0; i<10; i++)
                        {
                            W7C2_LS_lSCKARTBinCT[i]=0;
                        }
                        LastSet.iSCKARTInputCT=0;
                        LastSet.lShuttleCount=0;
                        W7C2_LOTSUMMARY_CLEARRTDATA();

                        WriteLastDataFile(false);                               //kevin 20141030
                        W7C2_FSORTCT_SHOWLOADING();
                        W7C2_FSORTCT_SHOWSORT();

                        if(W7C2_SCKART->iTesterType==0)
                        {
                            fSCKART->iInputCount=W7C2_LS_iSCKART_RTUnitCount;
                            W7C2_LS_iSCKART_RTUnitCount=0;
                        }
                        fSCKART->iInputJamCnt    =0;
                        W7C2_SCKART->iOutputJamCnt   =0;

                        if(W7C2_SCKART->iCurrentStatus==W7C2_SCKART->iLOTSTATUS_R)
                        {
                            W7C2_SCKART->SetLotStatus(W7C2_SCKART->iLOTSTATUS_W);
                            W7C2_SCKART->iWaitGPIBLotR=3;
                        }
                        W7C2_SCKART->AccessFile(false, -1);
                        RecordProcess("ART LOTRETESTCLEARED.");
                        W7C2_FLOTINFO_CLEARBARCODE();                  //Steven 20190214 : 統一清除2DID方式
                    }

                    if(W7C2_LS_bBreakSCKART)                                    //Sam 20202015 : TCP ART 增加 OneCycle 可手動強制中斷流程
                    {
                        W7C2_LS_bBreakSCKART=false;
                        W7C2_LS_iSCKART_RTUnitCount=0;
                        W7C2_SCKART->CheckNeedRT();
                        W7C2_FMAIN_SETLOTSTATE(10);                                 //TCP ART Final Lot End
                    }
                    else
                    {
                        if(W7C2_LS_bFirstTestAutoRetestGPIB==true)
                        {
                            if(IniConfig.bSPILFunction==true && bStopART==true)                                         //JerryYang 20220923 : SECS GEM版本ART
                            {
                                bStopART=false;
                                W7C2_LS_iSCKART_RTUnitCount=0;
                                W7C2_SCKART->CheckNeedRT();
                                W7C2_FMAIN_SETLOTSTATE(10);                         //SPIL ART Final Lot End
                            }
                            else
                            {
                                W7C2_LS_bFirstTestAutoRetestGPIB=false;
                                W7C2_FMAIN_SETLOTSTATE(8);                          //ART Lot End
                            }
                        }
                        else
                        {
                            bStopART=false;                                     //JerryYang 20220923 : SECS GEM版本ART
                            W7C2_FMAIN_SETLOTSTATE(10);                             //ART Final Lot End
                        }
                    }
                }

                W7C2_LS_bEndLotAutoRetestGPIB=true;
                if(TestIF_File.bSCKART_RunARTWithoutCmd==false)                 //JerryYang 20200318 fix選擇RunARTWithoutCmd發生hang up
                {
                    W7C2_LS_bWaitEndLotAutoRetestGPIB=false;
                }
                iFirstIn=0;
                return false;
            }
            else
            {
                if(fMain->hanaART->IsHanaArtAvailable()==true)                  //JimmyChiu 20241023 HANA ART Function
                {
//                    if(W7C2_HANAART->IsContactAvailable()==false)           //Steven 20250415 : mark
//                    {
//                        return false;
//                    }
                }
                else if(TestIF_File.bRENESAS_EnableFTCT==true)                  //RogerYang 20250913 : FT-CT
                {
//                    if(W7C2_RENESAS->iHaveRecvTestEnd==1)              //RogerYang 20250913 : FT-CT "send 71"
//                    {
//                        W7C2_RENESAS->SendTestEnd();
//                        W7C2_FMAIN_SETLOTSTATE(8);
//                        W7C2_RENESAS->iHaveRecvTestEnd=0;  //這裡才能清0
//                    }
//                    else
//                    {
//                        ShowMyMessage("(FT-CT)未先收到70 TESTEND 命令");
//                        return false;
//                    }
                }
                else if(TestIF_File.bSCKART_RunARTWithoutCmd==false &&
                        W7C2_LS_bWaitEndLotAutoRetestGPIB==false)               //JerryYang 20200318 fix選擇RunARTWithoutCmd發生hang up
                {
                    return false;
                }
                ret=1;
                bRet=true;
            }
        }
        else if(CosFunction.bUseSCKART &&
                W7C2_SCKART->iTesterType==0)                                        //Steven 20161201 (wei) : For SCK 93K ART
        {
            if(W7C2_SCKART->iWaitGPIBLotR==3)
            {
                W7C2_LS_iRetestFlagART=1;
                bRet=true;                                                      //kevin 20150717
            }
            else if(W7C2_SCKART->iWaitGPIBLotR==4)
            {
                W7C2_LS_iRetestFlagART=-1;
                ret=K_TRAY_FEED;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if(IniConfig.bEnable_SECS_GEM)
            {
                if(LastSet.iRunStartMode!=rsmInitial_ART &&
                   LastSet.iRunStartMode!=rsmContinuRetest_ART)
                    bSecsGemCanStart=true;                                      //wei 20150630  SecsGem Can Start
            }
            else
            {
                ShowMyMessage("Please Print Summary ", "請結批報表");
            }
            ret=1;
            bRet=true;                                                          //kevin 20150717
        }
    }

    if(bRet)                                                                    //kevin 20150717
    {
        if(CosFunction.bUseSCKART)                                              //Steven 20161201 (wei) : For SCK 93K ART
        {
            if(W7C2_SCKART->iNeedRT>0)
            {
                bNeedRetest=true;
            }
            else
            {
                bNeedRetest=false;
                ret=K_TRAY_FEED;
            }
        }
        else
        {
            bNeedRetest=CheckNeedToRT();                                        //Steven 20161127 : change to function
            if(CosFunction.bAutoRetestGPIBmode==true)                           //jou 2015-10-02 Auto Retest GPIB mode
            {
                if(W7C2_LS_iRetestFlagART==3)                                   //RETESTFLAG==00 not need retest
                {
                    bNeedRetest=false;
                    W7C2_LS_bFinEndLotAutoRetestGPIB=false;
                    W7C1_LS_bWaitStartLotAutoRetestGPIB=true;                   //Frank 20161212 (Jou) 若Stop Testing後須做HP Check時
                }
                else if(W7C2_LS_iRetestFlagART==1)                              //RETESTFLAG==11 need Auto retest
                {
                    W7C2_LS_bFinEndLotAutoRetestGPIB=false;
                    W7C2_FLOTINFO_CLEARBARCODE();                      //Frank 20170505 (Steven) : add for Xilinx 2DID
                }
                else if(W7C2_LS_iRetestFlagART==2)                              //RETESTFLAG==12 Auto retest Finish
                {
                    W7C2_LS_bFinEndLotAutoRetestGPIB=true;
                    W7C2_FLOTINFO_CLEARBARCODE();                      //Frank 20170505 (Steven) : add for Xilinx 2DID
                }
                W7C2_LS_iRetestFlagART=-1;
            }
        }

        W7C2_FTRAYMAP_CLEARTRAYIDBYLOT();                                       //JerryYang 20250120 : add

        if(bNeedRetest==false)                                                  //ChungHung 20141002 add for KYEC AutoRetest
        {
            if(W7C2_LS_iAutoRetestCount_ART>=1)                                 //wei 20150923 add ART計數
                W7C2_EVENTREPORT_ARTRTFINISH();                            //61     每輪RT結束發報Event給Host
            if(TrayForm.bEnableAMR==false)                                      //Eastsun 20260514 F010 AMR ART guard
            W7C1_LS_iLoaderTrayCount_ART=0;
            ret=K_TRAY_FEED;                                                    //Auto Tray Feed
        }
        else if(LastSet.iRunStartMode==rsmContinuStart_ART)                     //第一次FT (Pass && Faill )測試
        {
            W7C1_LS_iLoaderTrayCount_ART=0;
            W7C2_LS_iAutoRetestCount_ART++;

            if(IniConfig.bEnable_SECS_GEM==true)
            {
                if(W7C2_LS_iAutoRetestCount_ART==1)                             //ChungHung 20150511 modify
                {
                    W7C2_EVENTREPORT_ARTFTFINISH();                        //63     正測流程結束發報Event給Host
                }
                else
                {
                    W7C2_EVENTREPORT_ARTRTFINISH();                        //61     每輪RT結束發報Event給Host
                }
            }
            W7C2_FMAIN_LABEL12=W7C2_LS_iAutoRetestCount_ART;

            if(bUseFailNoDistinction)
            {
                int iFailYield=0, Sum_ART=0, iFail_ART=0;                       //ChungHung 20141002 add for KYEC AutoRetest

                double iPass=0.0,iPassYield=0.0;
                for(int i=0; i<eTrayCount; i++)
                {
                    Sum_ART+=W7C2_LS_BinCT_ART[0][iTo3Unload[i]];
                    if(Prod.iIsFailT6[i]==1)                                    //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6
                    {
                        iFail_ART+=W7C2_LS_BinCT_ART[0][iTo3Unload[i]];         //wei 20150923 add ART計數
                    }
                    else
                    {
                        iPass+=W7C2_LS_BinCT[0][iTo3Unload[i]];                 //kevin 20150613
                    }
                }

                iFailYield=ChangeToFloatNonPcnt((double)(iFail_ART*100), (double)(Sum_ART));                            //wei 20150923 add ART計數
                iPassYield=100-iFailYield;                                      //kevin 20150706 add yield

                if(bAutoLeastRetestFile && W7C2_LS_iAutoRetestCount_ART<=iAutoLeastRetestLimitFile)
                {
                    DoAutoRetest(true);
                    SetRunStartMode(rsmAutoRetest);
                    W7C1_FMAIN_START("DoART_AfterCleanOut 1");
                    if(bAutoReTest_ART)                                         //kevin 20150605
                        bLoaderNoTrayAutoCleanOut=false;                        //kevin 20150605 load 可以再new tray
                }
                else if(W7C2_LS_iAutoRetestCount_ART==1 &&
                        ((iUseFTFailYield==0 &&
                         ((iUseFTFailYieldModel==0 && iPassYield>=dFailYieldRate_ARTFTFile[1]) ||
                         (iUseFTFailYieldModel==1 && iPassYield>dFailYieldRate_ARTFTFile[1]) ||
                         (iUseFTFailYieldModel==2 && iPassYield<dFailYieldRate_ARTFTFile[1]) ||
                         (iUseFTFailYieldModel==3 && iPassYield<=dFailYieldRate_ARTFTFile[1]))) ||
                         (iUseFTFailYield==1 && (iPassYield>=dFailYieldRate_ARTFTFile[0] && iPassYield<=dFailYieldRate_ARTFTFile[2]))))
                {
                    if(IniConfig.bEnable_SECS_GEM==true)
                    {
                        W7C2_EVENTREPORT_ARTRTFINISH();                    //61     每輪RT結束發報Event給Host
                        MySleep(100);
                    }

                    if(TrayForm.bEnableAMR==false)                              //Eastsun 20260514 F010 AMR ART guard
                        W7C1_LS_iLoaderTrayCount_ART=0;
                    ret=K_TRAY_FEED;
                    W7C1_FMAIN_START("DoART_AfterCleanOut 2");
                }
                else if(W7C2_LS_iAutoRetestCount_ART>=1 &&
                        ((iUseRTFailYield==0 &&
                        ((iUseRTFailYieldModel==0 && iPassYield>=dFailYieldRate_ARTRTFile[1]) ||
                         (iUseRTFailYieldModel==1 && iPassYield>dFailYieldRate_ARTRTFile[1]) ||
                         (iUseRTFailYieldModel==2 && iPassYield<dFailYieldRate_ARTRTFile[1]) ||
                         (iUseRTFailYieldModel==3 && iPassYield<=dFailYieldRate_ARTRTFile[1]))) ||
                         (iUseRTFailYield==1 && (iPassYield>=dFailYieldRate_ARTRTFile[0] && iPassYield<=dFailYieldRate_ARTRTFile[2]))))
                {
                    if(IniConfig.bEnable_SECS_GEM==true)
                    {
                        W7C2_EVENTREPORT_ARTRTFINISH();                    //61     每輪RT結束發報Event給Host
                        MySleep(100);
                    }

                    if(TrayForm.bEnableAMR==false)                              //Eastsun 20260514 F010 AMR ART guard
                        W7C1_LS_iLoaderTrayCount_ART=0;
                    ret=K_TRAY_FEED;
                    W7C1_FMAIN_START("DoART_AfterCleanOut 3");
                }
                else
                {
                    DoAutoRetest(true);
                    SetRunStartMode(rsmAutoRetest);
                    W7C1_FMAIN_START("DoART_AfterCleanOut 4");
                    if(bAutoReTest_ART)                                         //kevin 20150605
                        bLoaderNoTrayAutoCleanOut=false;                        //kevin 20150605 load 可以再new tray
                }
            }
            else
            {
                if(CUSTOMER_CODE==CC_TSMC_TAINAN)                               //wei 20170119 (jou) ATR等待Secs Gem指令
                {
                    if(IniConfig.bEnable_SECS_GEM==true)
                    {
                        fMain->Pause("DoART_AfterCleanOut TSMC");
                    }
                    else
                    {
                        DoAutoRetest(true);
                        SetRunStartMode(rsmAutoRetest);
                        W7C1_FMAIN_START("DoART_AfterCleanOut 5");
                        if(bAutoReTest_ART)                                     //kevin 20150605
                            bLoaderNoTrayAutoCleanOut=false;                    //kevin 20150605 load 可以再new tray
                    }
                }
                else
                {
                    DoAutoRetest(true);
                    SetRunStartMode(rsmAutoRetest);
                    if(IniConfig.bSPILFunction==false)                          //JerryYang 20220923 : SPIL半套ART
                    {
                        W7C1_FMAIN_START("DoART_AfterCleanOut 6");
                    }

                    if(bAutoReTest_ART)                                         //kevin 20150605
                        bLoaderNoTrayAutoCleanOut=false;                        //kevin 20150605 load 可以再new tray
                }
            }
        }
        else if(LastSet.iRunStartMode==rsmContinuRetest_ART)                    //最後一次 Finial Testting
        {
            if(IniConfig.bEnable_SECS_GEM==true)
            {
                W7C2_EVENTREPORT_ARTRTFINISH();                            //61     每輪RT結束發報Event給Host
                MySleep(100);
            }
            bART_RT2RunNoChangeMode=false;
            if(TrayForm.bEnableAMR==false)                                      //Eastsun 20260514 F010 AMR ART guard
                W7C1_LS_iLoaderTrayCount_ART=0;
            ret=K_TRAY_FEED;                                                    //Auto Tray Feed
        }
    }
    return true;
}
//------------------------------------------------------------------------------
