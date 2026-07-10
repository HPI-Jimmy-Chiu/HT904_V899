//---------------------------------------------------------------------------
//  atester_ProcessCount.cpp  --  TESTER/INDEX count/yield-alarm bookkeeping (leaf subset)
//
//  Translation wave: W5-ProcessCount-Translate (atester_ProcessCount partial leaf extraction)
//  Translator: AI(W5-ProcessCount-Translate) 20260710
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/atester_ProcessCount.cpp (2161 lines, cp950)
//
//  SCOPE (narrow, per translation-plan hand-off -- do NOT expand).  14 functions,
//  faithfully transcribed, golden line ranges (re-verified against golden before
//  writing this file):
//    RecordPiggyBackStartEnd            golden :61-92
//    ProcessPiggyBackFunction            golden :94-140
//    InitialPiggyBackFunction            golden :142-217
//    ProcessSocketPurgeCount              golden :219-223
//    DoLowYieldAlarm                      golden :258-355
//    DoLowYieldAlarmFirst                 golden :357-394
//    CheckContinuoussFail                 golden :396-1018   (largest -- 623 lines)
//    ProcessContinuoussFailForATC         golden :1020-1050
//    GetMultiplierNum                     golden :1052-1055
//    CheckContinuoussPassBySocket          golden :1057-1129
//    CheckContinuoussPass                 golden :1131-1183
//    ProcessHeadContactCount              golden :1185-1232
//    ProcessSocketCount                    golden :1234-1307
//    ProcessAutoloadcellMeasureCount       golden :1730-1747
//
//  Deliberately OUT OF SCOPE this wave (leave for the brand-new cSocket.cpp
//  module + 7 new forms per the translation-plan hand-off; every OTHER golden
//  export -- ProcessShowTestStatus(:225-256)/ProcessCount/RecordHistroy/
//  ProcessStartTestData/DoLowYieldAlarm's sibling ProcessAutoCleanCount/
//  ProcessQASampling/... -- stays served by the existing atester_shims.h/.cpp
//  offline stubs exactly as today; NOT re-declared/re-defined here (would be a
//  duplicate-definition ODR clash at link -- the integrate agent removes the
//  now-superseded stub bodies for the 14 symbols THIS file defines for real,
//  mirroring the aTester_Front.cpp/aTester_Rear.cpp W64b integrate precedent).
//
//  TRANSLATION RULES (mirrors atester.cpp / aTester_Front.cpp conventions)
//  --------------------------------------------------------------------
//    * Off Borland: __fastcall removed; AnsiString/TQPF_Timer via vclcompat;
//      numeric semantics EXACT (no int/float rewrites; the ContinuousPassBinCount
//      256-element array zeroed only over its first 30 slots in Initial/Process
//      PiggyBackFunction is a FAITHFUL golden quirk -- golden :126/:205 literally
//      loop `i<30`, NOT `i<256` -- preserved verbatim, not "fixed").
//    * Big5 Chinese comments decoded via cp950, preserved as UTF-8. ZERO U+FFFD.
//    * bContinuoussPass is GLOBAL (cmydef.h) and used as such in
//      CheckContinuoussPassBySocket (no local shadow, golden :1057-1129 has none);
//      CheckContinuoussPass DOES locally shadow it with `bool bContinuoussPass=
//      false;` (golden :1134) -- BOTH forms preserved exactly as golden, a
//      faithful same-named global/local split across two sibling functions.
//
//  GATED DEPENDENCIES (verified ABSENT from the already-translated substrate;
//  each gated individually with the golden default reproduced, per the
//  established "#if 0 // TODO(...)" convention -- see aTester_Front.cpp for the
//  precedent this follows):
//    1. TastCategory (TEST_CATEGORY, golden cSocket.h:144-174) -- cSocket.cpp is
//       a future ~1300-line module + 7 new forms (explicitly out of THIS wave's
//       scope per the hand-off).  Gates the two golden
//       `if(CosFunction.bBySiteByBinPercentCompare==true){...}` blocks inside
//       CheckContinuoussFail (golden :588-591 and :764-936) -- the by-site/by-bin
//       and by-arm-per-site percent-compare low-yield alarms (WAR07335/36/37).
//       Default: skip (golden CosFunction.bBySiteByBinPercentCompare defaults
//       false anyway, so this is the observed baseline behavior).
//    2. InitialAutoCleanAllTask() (golden AutoClean/AutoClean.h:7, AutoClean.cpp)
//       -- the AutoClean engine module is not translated (separate wave, see
//       ht9045-autoclean-flow skill).  Gates 3 call sites inside
//       CheckContinuoussFail (golden :970/:981/:994).  Default: no-op (the
//       auto-clean trigger itself is skipped; the surrounding low-yield
//       ignore-count-clear / alarm logic that does NOT depend on AutoClean is
//       untouched).
//    3. fCleaning->CheckSmartAutoClean(AnsiString,AnsiString) (golden
//       AutoClean/uCleaning.h:356, TfCleaning : public TForm) -- gated via the
//       PC_CHECKSMARTAUTOCLEAN() macro below (2 call sites in DoLowYieldAlarm,
//       golden :265/:287).  Default: false (both golden call sites have an
//       EMPTY if-body anyway -- see the macro comment for why false is exactly
//       equivalent to "feature not present").
//    4. fMain->slLowYieldAlarm (TStringList*, golden main.h) -- FormsFacade.h's
//       TfMain (already included) does not have this member yet.  Gated as a
//       verbatim block (golden :295-298) inside DoLowYieldAlarm's bOneCycle
//       branch; pure log/bookkeeping side effect, no bearing on the ret code or
//       on the BtnOneCycleClick() call immediately after it (which is NOT gated
//       -- FormsFacade.h's TfMain::BtnOneCycleClick/BtnOneCycle->Down already
//       exist and are used directly).
//    5. fYieldMonitoring->ClearYieldCount() (golden uYieldMonitoring.h) --
//       aHotPlateSubstrate.h's TfYieldMonitoring_2x4_16 (the ALREADY-DECLARED
//       offline mirror of the golden TfYieldMonitoring form) only exposes
//       DoAutoCloseSite() today; ClearYieldCount() has no member yet.  Gated via
//       the PC_YIELDMON_CLEARCOUNT() macro below (6 call sites in
//       CheckContinuoussFail).  Default: no-op.
//    6. fProductionInfo->cDynaThres->GetMultiplierNum()/AddThresholdNum() (golden
//       ProductionInfo.h:547/587-588, cDynamicMultiContinualPassBinBySocket) --
//       gated in GetMultiplierNum() and CheckContinuoussPassBySocket (golden
//       :1054/:1126).  Default: GetMultiplierNum()->1 (golden
//       ProductionInfo.cpp:5955-5961's own default when
//       IniConfig.bN14_24_DynaMultiContinuPassSocket==false, i.e. no dynamic
//       multiplier scaling); AddThresholdNum() no-op (has zero observable effect
//       while GetMultiplierNum() is gated to always return 1).  NOT invented as
//       a new global here -- see this unit's translate report for why (the tree
//       already carries multiple mutually-incompatible per-file
//       `extern <Stub>* fProductionInfo;` declarations with no actual
//       definition anywhere, a pre-existing cross-unit landmine, not introduced
//       by this file).
//  Every gate is individually re-cited at its call site below.
//---------------------------------------------------------------------------
#include "atester_ProcessCount.h"

#include "MachineType.h"            // ePiggyBackTrugger (pbtNone..), CUSTOMER_CODE consts, NN_2Row, MAX_SOCKET_ROW/COL
#include "cmydef.h"                 // global scalar universe (Continuous*Count arrays, bCheckIndex, iOneCycle, ...)
#include "cprod.h"                  // Prod / TestIF / TestIF_File / RunInfo / Temperature
#include "Config.h"                 // IniConfig (HT9045_CONFIG)
#include "CosFunction.h"            // CosFunction (HT9045_COUSTOMER_FUNCTION)
#include "canary_support.h"         // LastSet / ShowErrorMessage / RecordProcess
#include "aHotPlateSubstrate.h"     // TestSocket / FTestSuck / MyDBIProcess / fYieldMonitoring
#include "csystem.h"                // InitOneCycle
#include "atester_shims.h"          // IsNNMode() / fContact (TfContactShim)
#include "FormsFacade.h"            // fMain (BtnOneCycleClick / BtnOneCycle->Down already present)

//---------------------------------------------------------------------------
//  Gate macros (see file-header "GATED DEPENDENCIES" list above for full
//  rationale).  Each mirrors the golden default so the surrounding logic
//  compiles + behaves identically to a handler with that specific feature
//  disabled/absent, matching the W64B_* macro-gate convention established in
//  aTester_Front.cpp.
//---------------------------------------------------------------------------
// AI(W5-ProcessCount-Translate) 20260710: fCleaning->CheckSmartAutoClean(...) --
// see file-header gate #3.  Both golden call sites (:265/:287) have an EMPTY
// if-body ("//DoSmartAutoClean", no code) -- so whether this predicate is true
// or false has NO effect on ret/control-flow beyond which branch of the
// if/else-if ladder is (harmlessly) skipped.  false is the conservative,
// behaviorally-safe default (identical to "no smart-auto-clean gate active").
#define PC_CHECKSMARTAUTOCLEAN(AlarmCode, ErrPart)  (false)

// AI(W5-ProcessCount-Translate) 20260710: fYieldMonitoring->ClearYieldCount() --
// see file-header gate #5.  6 call sites below.
#define PC_YIELDMON_CLEARCOUNT()   ((void)0)

//---------------------------------------------------------------------------
TQPF_Timer PiggyBackTimer;                                                      //golden :60
void RecordPiggyBackStartEnd(bool bStart)                                       //jou 2011-11-14 start : 記錄piggyback時間
{
    bPiggyBackIndexCheck=false;

    if(IniConfig.bRecordPiggyBackStartEnd==false)
        return;

    AnsiString AStr;

    if(bStart==true)
    {
        if(bRecordPiggyBackStart==true)
        {
            PiggyBackTimer.LatchCycleTimeSec(true);
        }
    }
    else
    {
        if(bRecordPiggyBackEnd==true && iOneCycle==false)
        {
            bRecordPiggyBackEnd=false;
            bRecordPiggyBackStart=false;
            iPiggyBackTime[1]=MyTickCount();                                    //jou test delay time

            AStr.sprintf("PiggyBack Funtion record time : %d Sec", PiggyBackTimer.LatchCycleTimeSec());
            MyDBIProcess("Message", AStr);

            AStr.sprintf("Piggy Back Pass Finish");                             //JerryYang 20220614 松諭要求新增
            MyDBIProcess("Message", AStr);
        }
    }
}
//---------------------------------------------------------------------------
void ProcessPiggyBackFunction()                                                 //Steven 20110725 : 將所有的Piggy Back整合到這裡
{
    if(CosFunction.bIndexCheckCanTurnOff &&                                     //Isaac 20211019 : 可選擇做index check的時機，符合條件則不檢查
       ((IniConfig.iD71IndexCheckOnOffMode==0 && bLotStartEndNeedIndexCheck==false) ||
        (IniConfig.iD71IndexCheckOnOffMode==1 && bIndexJamNeedIndexcheck==false) ||
         IniConfig.iD71IndexCheckOnOffMode==2))
    {                                                                           //不檢查
    }
    else
    {
        if(bCheckIndex==false)
        {
            bRecordPiggyBackStart=true;
        }

        bCheckIndex=true;

        if(fMain->BtnOneCycle->Down==false)
        {
            InitOneCycle("Process Piggy Back Function");
            fMain->BtnOneCycle->Down=true;
            LastSet.SendCT[3]=0;
            LastSet.SendCT[2]=0;
            LastSet.iIndexCount=0;                                              //wei 20141201 Low Yield Auto Clean
            if((USE_AUTO_RETEST==eartInstall &&
                (bAutoReTest_ART || IniConfig.bA10_AutoReTest)) ||              //kevin 20150615
                CosFunction.bUseARTSortCount)                                   //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
            {
               LastSet.SendCT_ART[3]=0;
               LastSet.SendCT_ART[2]=0;
            }

            for(int i=0; i<30; i++)
            {
                ContinuousPassBinCount[i]=0;
            }

            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    ContinuousPassBinBySocket[i][j]=0;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void InitialPiggyBackFunction()                                                 //Steven 20110725 : 重置Piggy Back的狀態
{
    AnsiString str;
    if(bCheckIndex==true)
    {
        switch(iWhoTriggerPiggyBack)                                            //Steven 20111207 : 誰觸發了Piggy Back
        {
            case pbtNone:
                str.sprintf("PiggyBack Function Start");
                break;
            case pbtContinualPassBin:
                str.sprintf("PiggyBack Function Start - Continual Pass Bin");
                break;
            case pbtContinualPassBinBySocket:
                str.sprintf("PiggyBack Function Start - Continual Pass Bin By Socket");
                break;
            case pbtContinualLoader:
                str.sprintf("PiggyBack Function Start - Continual Loader");
                break;
            case pbtContinualContactCount:
                str.sprintf("PiggyBack Function Start - Continual Contact Count");
                break;
            case pbtHome:
                str.sprintf("PiggyBack Function Start - Home");
                break;
            case pbtOneCycle:
                str.sprintf("PiggyBack Function Start - One Cycle");
                break;
            case pbtCleanout:
                str.sprintf("PiggyBack Function Start - Clean Out");
                break;
            case pbtReset:
                str.sprintf("PiggyBack Function Start - Reset");
                break;
            case pbtOutShuttleLose:
                str.sprintf("PiggyBack Function Start - Out Shuttle Lose IC");
                break;
            case pbtIndexArmVacOffErr:                                          //JerryYang 20160815 Index arm 發生Vaccum off error要做piggy back
                str.sprintf("PiggyBack Function Start - Index Arm Vacuum Off Error");
                break;
            case pbtRTCAlarm:
                str.sprintf("PiggyBack Function Start - RTC Alarm");
                break;
            case pbtOneCycleRemainSht:                                          //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
                str.sprintf("PiggyBack Function Start - [I09] one cycle remain shuttle device");
                break;
        }
        iWhoTriggerPiggyBack=pbtNone;
        bRecordPiggyBackEnd=true;
        MyDBIProcess("Message", str);
    }

    bCheckIndex=false;
    LastSet.SendCT[3]=0;
    LastSet.SendCT[2]=0;
    LastSet.iIndexCount=0;                                                      //wei 20141201 Low Yield Auto Clean
    if(USE_AUTO_RETEST==eartInstall &&                                          //kevin 20150615
       (bAutoReTest_ART || IniConfig.bA10_AutoReTest))                          //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
    {
        LastSet.SendCT_ART[3]=0;
        LastSet.SendCT_ART[2]=0;
    }

    for(int i=0; i<30; i++)
    {
        ContinuousPassBinCount[i]=0;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            ContinuousPassBinBySocket[i][j]=0;
        }
    }
}
//---------------------------------------------------------------------------
void ProcessSocketPurgeCount()
{
    if(IniConfig.bD47EnableSocketPurgeFunction)
        LastSet.iD47SocketTestedCount++;
}
//------------------------------------------------------------------------------
int DoLowYieldAlarm(AnsiString AlarmCode, AnsiString ErrPart)                   //Steven 20180627 (wei) : 整合Low Yield Alarm
{
    AnsiString Str;
    bool bOneCycle=false;
    int ret=K_ONECYCLE;

    if(CosFunction.bSmartAutoClean &&
       PC_CHECKSMARTAUTOCLEAN(AlarmCode, ErrPart))                              //Sam 20250916 : Alarm後需要清除資料才能Start //Sam 20230111 : Smart Auto Clean
    {
        //DoSmartAutoClean
    }
    else if(CosFunction.bYieldAlmNeedOneCycle)
    {
        bOneCycle=true;
        if(CosFunction.bContinueFailNeedAlarmDirectly)                          //Steven 20230831 : 當使用bYieldAlmNeedOneCycle, 連續Fail要直接Alarm
        {
            if(AlarmCode=="WAR07301" ||                                         //Socket consecutive failureAlarmCode
               AlarmCode=="WAR07321" ||                                         //Arm1: consecutive failure
               AlarmCode=="WAR07322" ||                                         //Consecutive Pass Error!
               AlarmCode=="WAR07329" ||                                         //Arm2: consecutive failure
               AlarmCode=="WAR07331" ||                                         //Special Bin Socket Consecutive Failure
               AlarmCode=="WAR07332" ||                                         //Arm1: Special Bin Consecutive Failure
               AlarmCode=="WAR07333")                                           //Arm2: Special Bin Consecutive Failure
            {
                bOneCycle=false;
            }
        }
    }

    if(PC_CHECKSMARTAUTOCLEAN(AlarmCode, ErrPart))                              //Sam 20230111 : Smart Auto Clean
    {
        //DoSmartAutoClean
    }
    else if(bOneCycle)                                                          //JerryYang 20161006 Yeild Alarm 時要做完Onecycle才能跳出Alarm視窗
    {
        bNeedOneCycleByYieldAlm=true;
        Str.sprintf("%s,%s", AlarmCode, ErrPart);
        // AI(W5-ProcessCount-Translate) 20260710: fMain->slLowYieldAlarm (TStringList*)
        // -- see file-header gate #4.  Golden verbatim, gated whole (pure log/
        // bookkeeping side effect; the BtnOneCycleClick() call right after this
        // block is NOT gated -- it is unconditional in golden too).
#if 0 // TODO(W6.4b-integrate): ADD `TStringList *slLowYieldAlarm;` (golden main.h) to FormsFacade.h's TfMain, initialized in TfMain's ctor.
        if(fMain->slLowYieldAlarm->Text.AnsiPos(AlarmCode)==0)
        {
            fMain->slLowYieldAlarm->Add(Str);
        }
#endif
        fMain->BtnOneCycleClick(fMain);
    }
    else if(CosFunction.bLowYieldDoOneCycle)                                    //Steven 20170308 (wei) : 良率相關的都做One Cycle
    {
        if(CUSTOMER_CODE==CC_JCET)                                              //JerryYang 20190709 JCET黃剛要求low yield不要強制one cycle
        {
            if(IniConfig.bI05LowYieldForcedOneCycle)
            {
                ret=ShowErrorMessage(AlarmCode, K_ONECYCLE, MMInterface, false, ErrPart);
            }
            else
            {
                ret=ShowErrorMessage(AlarmCode, K_RETRY|K_ONECYCLE, MMInterface, false, ErrPart);
            }
        }
        else
        {
            ret=ShowErrorMessage(AlarmCode, K_ONECYCLE, MMInterface, false, ErrPart);
        }
    }
    else if(CUSTOMER_CODE==CC_Greatek ||
            CUSTOMER_CODE==CC_AMKOR_China ||
            CUSTOMER_CODE==CC_QUALCOMM ||
            IniConfig.bKoreaFunction==true)
    {
        if(CosFunction.bYieldAlmNeedOneCycle)                                   //Steven 20240131 : 修正連續fail做 auto clean 跟 alarm互卡的問題
        {
            if(iOneCycle==0)
                ret=ShowErrorMessage(AlarmCode, K_RETRY, MMInterface, false, ErrPart);
        }
        else
        {
            ret=ShowErrorMessage(AlarmCode, K_RETRY, MMInterface, false, ErrPart);
        }
    }
    else if(CosFunction.bSpecailLowYeild && Prod.bFailAlarmLowYieldSpecial &&
            (bLowYeildAlarmSpecial || bLowYeildAlarmSpecial1stPass))            //Sam 20210505 : PTI 要求的兩段 Low Yeild
    {
        ret=ShowErrorMessage(AlarmCode, 0, MMInterface, false, ErrPart);
    }
    else
    {
        ret=ShowErrorMessage(AlarmCode, K_RETRY|K_ONECYCLE, MMInterface, false, ErrPart);
    }

    if(CUSTOMER_CODE==CC_SIGURD_ChungXing)                                      //Sam 20230728 : 發生 LowYield 後重新 Start 清除 Sort Count 和 contact counterKinds
    {
        if(AlarmCode=="WAR0702" || AlarmCode=="WAR0702" || AlarmCode=="WAR0705" || AlarmCode=="WAR07321" || AlarmCode=="WAR07329")
            bInitailQtyByLowYield=true;
    }

    if(ret==K_ONECYCLE)
    {
        fMain->BtnOneCycleClick(fMain);
    }
    return ret;
}
//------------------------------------------------------------------------------
int DoLowYieldAlarmFirst(AnsiString AlarmCode, AnsiString ErrPart)              //Steven 20180627 (wei) : 整合Low Yield Alarm
{
    int ret=K_ONECYCLE;
    if(CosFunction.bLowYieldDoOneCycle)                                         //Steven 20170308 (wei) : 良率相關的都做One Cycle
    {
        if(CUSTOMER_CODE==CC_JCET)                                              //JerryYang 20190709 JCET黃剛要求low yield不要強制one cycle
        {
            if(IniConfig.bI05LowYieldForcedOneCycle)
            {
                ret=ShowErrorMessage(AlarmCode, K_ONECYCLE, MMInterface, false, ErrPart);
            }
            else
            {
                ret=ShowErrorMessage(AlarmCode, K_RETRY|K_ONECYCLE, MMInterface, false, ErrPart);
            }
        }
        else
        {
            ret=ShowErrorMessage(AlarmCode, K_ONECYCLE, MMInterface, false, ErrPart);
        }
    }
    else if(CUSTOMER_CODE==CC_Greatek ||
            CUSTOMER_CODE==CC_AMKOR_China ||
            CUSTOMER_CODE==CC_QUALCOMM)
    {
        ret=ShowErrorMessage(AlarmCode, K_RETRY, MMInterface, false, ErrPart);
    }
    else
    {
        ret=ShowErrorMessage(AlarmCode, K_RETRY|K_ONECYCLE, MMInterface, false, ErrPart);
    }

    if(ret==K_ONECYCLE)
    {
        fMain->BtnOneCycleClick(fMain);
    }
    return ret;
}
//------------------------------------------------------------------------------
void CheckContinuoussFail(int Index)                                            //Steven 20110603 : 整合只叫一次
{
    bool bConseFailureBySocket_AutoClean;
    unsigned int  iConseFailureCountBySocket_AutoClean;
    bool bConseFailureByHead_AutoClean;
    unsigned int  iConseFailureCountByHead_AutoClean;
    if(iRunStartMode==FT)
    {
        bConseFailureBySocket_AutoClean     =TestIF.bAutoClean_ConseFailureBySocket_Normal;
        iConseFailureCountBySocket_AutoClean=TestIF.iAutoClean_ConseFailureCountBySocket_Normal;
        bConseFailureByHead_AutoClean       =TestIF.bAutoClean_ConseFailureByHead_Normal;
        iConseFailureCountByHead_AutoClean  =TestIF.iAutoClean_ConseFailureCountByHead_Normal;
    }
    else
    {
        bConseFailureBySocket_AutoClean     =TestIF.bAutoClean_ConseFailureBySocket_Retest;
        iConseFailureCountBySocket_AutoClean=TestIF.iAutoClean_ConseFailureCountBySocket_Retest;
        bConseFailureByHead_AutoClean       =TestIF.bAutoClean_ConseFailureByHead_Retest;
        iConseFailureCountByHead_AutoClean  =TestIF.iAutoClean_ConseFailureCountByHead_Retest;
    }
    //ChungHung 20131223 add for SCK request end

    bool bSocketConsecutiveFailure=false, bConsecutiveFailure=false, bSocketAllSiteFail=false;                          //kevin 20170825 (Steven) add all site fail
    bool bSocketConsecutiveFailure_AutoClean=false, bConsecutiveFailure_AutoClean=false;                                //ChungHung 20131223 add for SCK request
    bool bDoAutoCleanByBinCount=false;                                          //Steven 20160308 : By Bin count do auto clean
    bool bDoAutoCleanBySiteCount=false;                                         //Steven 20160308 : By Bin pre site count do auto clean
    bool bSpecailBinContinuousFailByArm=false,bSpecialBinContinuousFailBySocket=false;                                  //ChungHung 20140730 add ContinuousFailHaveOneCycle
    AnsiString SocketErrPart="", ErrPart="", AutoCleanErrPart="";
    int iGetBin=0, iBinCount=0, iSite=0, iTray[TEST_MAX_BIN]={0};               //JerryYang 20221215 : 33->TEST_MAX_BIN
    int iAllSiteFailCount[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    int iSiteCount=0;

    // AI(W5-ProcessCount-Translate) 20260710: TastCategory-dependent locals (golden
    // :428-443) -- ONLY consumed inside the gated bBySiteByBinPercentCompare block
    // below (file-header gate #1).  Declared here verbatim (matching golden's
    // top-of-function layout for easy diffing against golden) even though they are
    // presently unused outside that gate.
    double dSiteByBinYield=0.0;                                                 //JerryYang 20170712 (Steven) by site by bin compare percent
    double dSiteByBinYield1[32]={0.0};
    double dArm1ByBinYield=0.0;
    double dArm2ByBinYield=0.0;
    double dSiteByBinYieldMax=0.0;
    double dSiteByBinYieldMin=100.0;
    double dArm1ByBinYieldMax=0.0;
    double dArm1ByBinYieldMin=100.0;
    double dArm2ByBinYieldMax=0.0;
    double dArm2ByBinYieldMin=100.0;
    AnsiString sWhichSiteByBinYieldMax="";
    AnsiString sWhichSiteByBinYieldMin="";
    AnsiString sArm1WhichSiteByBinYieldMax="";
    AnsiString sArm1WhichSiteByBinYieldMin="";
    AnsiString sArm2WhichSiteByBinYieldMax="";
    AnsiString sArm2WhichSiteByBinYieldMin="", sSocketAllSiteErrPart="";

    int Z=0, iTotalSite=0, iSocketFailIc=0;

    if(TestIF.iShuttleMode==1)                                                  //kevin 20170825 (Steven) use arm open/close site
    {
        if(TestIF.iShuttle_Sel==1)
        {
            Z=1;
        }
        else
        {
            Z=0;
        }
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(bTestSiteUse[Z][i][j])
            {
                iSite++;
                if(IniConfig.bA09_ByArmCloseSite &&                             //Steven 20220818 : By Arm Continue fail, auto site off
                   IsNNMode()==0)                                               //Sam 20250401 : 修正 NN 模式計數錯誤
                {
                    iSiteCount+=2;
                }
                else
                {
                    iSiteCount++;
                }
            }

            iAllSiteFailCount[i][j]=0;                                          //kevin 20170825 (Steven) all site fail
            if(TestSocket.Item[i][j]>=TEST_PASS)                                //ChungHung 20140516 fix 記憶體溢位
            {
                iGetBin=TestSocket.Item[i][j]-TEST_PASS;

                if(iGetBin>(iTestBinCount-1))                                   //Steven 20121112 : RS232支援32Bin 14->iTestBinCount-1
                    continue;

                if(IniConfig.bEnableAutoCleanFunction &&
                   TestIF.iAutoClean_Function==true &&
                   bIsAutoOneCycle==false)                                      //確保不要重複計算
                {
                    iAutoCleanByBinCount[iGetBin]++;
                    iAutoCleanBySiteCount[i][j][iGetBin]++;
                    if(Prod.iAutoCleanByBin[iGetBin]>0 &&                       //Steven 20160308 : By Bin count do auto clean
                       iAutoCleanByBinCount[iGetBin]>=Prod.iAutoCleanByBin[iGetBin])
                    {
                        bDoAutoCleanByBinCount=true;
                        AutoCleanErrPart+=IndexSuckName[i][j];                  //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                    }

                    if(Prod.iAutoCleanBySite[iGetBin]>0 &&                      //Steven 20160308 : By Bin pre site count do auto clean
                       iAutoCleanBySiteCount[i][j][iGetBin]>=Prod.iAutoCleanBySite[iGetBin])
                    {
                        bDoAutoCleanBySiteCount=true;
                        AutoCleanErrPart+=IndexSuckName[i][j];                  //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                    }
                }

                if(TestSocket.bPass[i][j])                                      //pass
                {
                    ContinuousFailSKTCount[i][j]=0;
                    ContinuousFailARMCount[Index][i][j]=0;
                    ContinuousFailSKTCount_AutoClean[i][j]=0;                   //ChungHung 20131223 add for SCK request
                    ContinuousFailARMCount_AutoClean[Index][i][j]=0;

                    if(Prod.iContsFailSocketAlarmCT>0)                          //Steven 20230529 : Spc Bin Couont改成連續錯誤
                    {
                        SpecialBinContinuousFailSKTCount[i][j]=0;
                        SpecialBinContinuousFailARMCount[Index][i][j]=0;
                    }
                }
                else if(TestSocket.Item[i][j]!=HAS_IC &&                        //ChungHung 20140730 add ContinuousFailHaveOneCycle start
                        TestSocket.Item[i][j]!=HAS_NULL_IC &&
                        TestSocket.bPass[i][j]==false)                          //Kevin 20161102 : 沒IC不Alarm連續fail
                {
                    if(Prod.bConsFail[iGetBin]==true ||
                       CUSTOMER_CODE==CC_Greatek ||                             //Sam 20200224 : 正賢要求只要是 Fail 就計數不管 Prod.bConsFail 設定
                       CUSTOMER_CODE==CC_SIGURD_HUKOU)                          //Sam 20210312 : 矽格湖口卓邵要求
                    {
                        if(fContact->fShow==false)
                        {
                            if(CUSTOMER_CODE==CC_KYEC_CHEN &&
                               Prod.bContsFailIgnore)                           //wei 20160115 銅鑼前幾顆不計算ContsFail
                            {
                                if(Prod.bContsFailBySocket==false &&
                                   Prod.bContsFailByHead==false)
                                    iContsFailIgnoreCount=0;
                                bContsFailIgnoreStart=true;
                                iContsFailIgnoreCount++;
                                if(iContsFailIgnoreCount>=Prod.iContsFailIgnore)
                                    bContsFailIgnoreStart=false;
                            }
                            else
                            {
                                bContsFailIgnoreStart=false;
                            }

                            if(bContsFailIgnoreStart==false)
                            {
                                ContinuousFailSKTCount[i][j]++;
                                ContinuousFailARMCount[Index][i][j]++;
                                ContinuousFailSKTCount_AutoClean[i][j]++;       //ChungHung 20131223 add for SCK request
                                ContinuousFailARMCount_AutoClean[Index][i][j]++;
                            }
                        }
                    }

                    if(Prod.bSpecialBinBySocket[iGetBin]==1)                    //ChungHung 20140730 add ContinuousFailHaveOneCycle start
                        SpecialBinContinuousFailSKTCount[i][j]++;

                    if(Prod.bSpecialBinByArm[iGetBin]==1)
                        SpecialBinContinuousFailARMCount[Index][i][j]++;

                    if(CosFunction.bAllSiteSameFailBinShowAlarm==true &&        //JerryYang 20170221 (wei) 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm,改成可複選bin
                       IniConfig.bI34AllSiteAreSameFailBinShowAlarm==true)
                    {
                        for(int k=0; k<iTestBinCount; k++)                      //JerryYang 20170221 (wei) 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm,改成可複選bin
                        {
                            iTray[k]=Prod.iT6CatData[k];
                            if(iTray[k]>=0 &&                                   //Steven 20230929 : 加入保護
                               Prod.iIsFailT6[iTray[k]]==1 &&                   //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6
                               bSpecificBin[k]==true)
                            {
                                if(iGetBin==k)
                                {
                                    iBinCount++;
                                    if(bTestSiteUse[Z][i][j])                   //kevin 20170831 開關site
                                    {
                                        sSocketAllSiteErrPart+=IndexSuckName[i][j];                                     //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                                    }
                                }
                            }
                        }
                    }
                    iAllSiteFailCount[i][j]=1;                                  //kevin 20170825 (Steven) all site fail
                }
            }
        }
    }

    // AI(W5-ProcessCount-Translate) 20260710: gate #1 (TastCategory / cSocket.cpp,
    // golden :588-591) -- see file header.
#if 0 // TODO(cSocket-module): TastCategory (TEST_CATEGORY, golden cSocket.h:144-174) has no translated home yet.
    if(CosFunction.bBySiteByBinPercentCompare==true)                            //JerryYang 20170712 (Steven) by site by bin compare percent
    {
        TastCategory.UpdataCount(true);                                         //Steven 20250514 : 統一計算數量
    }
#endif

    if(CosFunction.bAllSiteSameFailBinShowAlarm==true &&                        //kevin 20170825 (Steven) add all site fail
       IniConfig.bI34AllSiteAreSameFailBinShowAlarm==true)                      //JerryYang 20160913 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm
    {
        if(iBinCount>0 && iBinCount>=iSite)
        {
            DoLowYieldAlarmFirst("WAR07334", asErrPart);                        //Steven 20180627 (wei) : 整合Low Yield Alarm
        }
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(Prod.bAllSiteFail && bTestSiteUse[Z][i][j])                      //kevin 20170825 (Steven) all site fail  //Isaac 20180305 (Steven) ATK要求，只有FT要alarm，FT/RT分開
            {
                iTotalSite++;
                if(iAllSiteFailCount[i][j]>0)                                   //kevin 20170825 all site fail[i][j]>0)
                {
                    iSocketFailIc++;
                    sSocketAllSiteErrPart+=IndexSuckName[i][j];                 //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                }
            }

            if(Prod.bContsFailBySocket && Prod.iContsFailSocketAlarmCT!=0)
            {
                if(ContinuousFailSKTCount[i][j]>=Prod.iContsFailSocketAlarmCT)
                {
                    ContinuousFailSKTCount[i][j]=0;

                    if(bCanAutoCloseSite &&
                       TestIF_File.bLowYieldAutoSiteOffByContiFail)             //Steven 20200420 : Continue fail, auto site off
                    {
                        if(IniConfig.bA09_ByArmCloseSite &&                     //Steven 20220818 : By Arm Continue fail, auto site off
                           IsNNMode()==0)                                       //Sam 20250401 : 修正 NN 模式計數錯誤
                        {
                            iSiteCount-=2;
                        }
                        else
                        {
                            iSiteCount--;
                        }

                        if(iSiteCount<TestIF_File.iAlarmWhenSiteOnCountLess)
                        {
                            bSocketConsecutiveFailure=true;
                        }
                        else
                        {
                            bLowYieldCloseSite[0][i][j]=true;
                            bLowYieldCloseSite[1][i][j]=true;
                        }
                    }
                    else
                    {
                        bSocketConsecutiveFailure=true;
                    }
                    SocketErrPart+=IndexSuckName[i][j];                         //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                    asErrTemp+=IndexSuckName[i][j];                             //JerryYang 20160407 存error位置
                }
            }
            else
            {
                ContinuousFailSKTCount[i][j]=0;
            }

            if(Prod.bContsFailByHead && Prod.iContsFailHeadAlarmCT!=0)
            {
                if(ContinuousFailARMCount[Index][i][j]>=Prod.iContsFailHeadAlarmCT)
                {
                    ContinuousFailARMCount[Index][i][j]=0;
                    if(bCanAutoCloseSite &&
                       TestIF_File.bLowYieldAutoSiteOffArmContiFail &&
                       (IniConfig.bA09_ByArmCloseSite ||                        //Steven 20220818 : By Arm Continue fail, auto site off
                        IsNNMode()>0))                                          //Sam 20250401 : 修正 NN 模式計數錯誤
                    {
                        if(IsNNMode()>0)                                        //Sam 20250401 : 修正 NN 模式計數錯誤
                        {
                            if(bLowYieldCloseSite[Index][i][j]==false)
                                iSiteCount--;
                        }
                        else
                        {
                            iSiteCount--;
                        }

                        if(iSiteCount<TestIF_File.iAlarmWhenSiteOnCountLess)
                        {
                            bConsecutiveFailure=true;
                        }
                        else
                        {
                            bLowYieldCloseSite[Index][i][j]=true;
                        }
                    }
                    else
                    {
                        bConsecutiveFailure=true;
                    }
                    ErrPart+=IndexSuckName[i][j];                               //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                }
            }
            else
            {
                ContinuousFailARMCount[Index][i][j]=0;
            }

            if(bConseFailureBySocket_AutoClean &&                               //ChungHung 20131223 add for SCK request start
               iConseFailureCountBySocket_AutoClean!=0)
            {
                if(ContinuousFailSKTCount_AutoClean[i][j]>=iConseFailureCountBySocket_AutoClean)
                {
                    ContinuousFailSKTCount_AutoClean[i][j]=0;
                    bSocketConsecutiveFailure_AutoClean=true;
                    SocketErrPart+=IndexSuckName[i][j];                         //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                }
            }
            else
            {
                ContinuousFailSKTCount_AutoClean[i][j]=0;
            }

            if(bConseFailureByHead_AutoClean && iConseFailureCountByHead_AutoClean!=0)
            {
                if(ContinuousFailARMCount_AutoClean[Index][i][j]>=iConseFailureCountByHead_AutoClean)
                {
                    ContinuousFailARMCount_AutoClean[Index][i][j]=0;
                    bConsecutiveFailure_AutoClean=true;
                    ErrPart+=IndexSuckName[i][j];                               //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                }
            }
            else
            {
                ContinuousFailARMCount_AutoClean[Index][i][j]=0;
            }

            if(TestSocket.Item[i][j]>=TEST_PASS)                                //ChungHung 20140730 add ContinuousFailHaveOneCycle start
            {
                iGetBin=TestSocket.Item[i][j]-TEST_PASS;
                if(iGetBin>(iTestBinCount-1))
                    continue;

                if(Prod.bSpecialBinBySocket[iGetBin] &&
                   SpecialBinContinuousFailSKTCount[i][j]>=Prod.iSpecialBinCountBySocket[iGetBin])
                {
                    SpecialBinContinuousFailSKTCount[i][j]=0;
                    bSpecialBinContinuousFailBySocket=true;
                    ErrPart+=IndexSuckName[i][j];                               //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                }

                if(Prod.bSpecialBinByArm[iGetBin] &&
                   SpecialBinContinuousFailARMCount[Index][i][j]>=Prod.iSpecialBinCountByArm[iGetBin])
                {
                    SpecialBinContinuousFailARMCount[Index][i][j]=0;
                    bSpecailBinContinuousFailByArm=true;
                    ErrPart+=IndexSuckName[i][j];                               //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                    if(CUSTOMER_CODE==CC_SJ_Semiconductor)                      //RogerYang 20260123 : add for Alarm Message
                    {
                        asErrPart.sprintf("%s : BIN %d over limit count %d ", ErrPart, iGetBin, Prod.iSpecialBinCountByArm[iGetBin]);
                    }
                }
            }
        }
    }

    if(Prod.bAllSiteFail && (iSocketFailIc>=iTotalSite))                        //Ifor 20190611 all site fail Alarm By Ft/Rt Setting
    {
        iallSitCount++;
        if(iallSitCount>=Prod.iAllSiteFailCount)                                //kevin 20180720 (wei) add all site fail count      //Steven 20230118 : All site fail RT
            bSocketAllSiteFail=true;                                            //kevin 20170825 add
    }

    // AI(W5-ProcessCount-Translate) 20260710: gate #1 (TastCategory / cSocket.cpp,
    // golden :764-936, the "Site by Bin" + "Site by Arm" percent-compare low-yield
    // alarms WAR07335/07336/07337) -- see file header.  ENTIRE golden for(iCat...)
    // loop below is dormant offline (matches CosFunction.bBySiteByBinPercentCompare's
    // own default==false baseline).
#if 0 // TODO(cSocket-module): TastCategory (TEST_CATEGORY, golden cSocket.h:144-174) has no translated home yet.
    if(CosFunction.bBySiteByBinPercentCompare==true)                            //JerryYang 20170712 (Steven) by site by bin compare percent
    {
        for(int iCat=0; iCat<iTestBinCount; iCat++)                             //Site by Bin
        {
            dSiteByBinYieldMax=0.0;
            dSiteByBinYieldMin=100.0;
            dArm1ByBinYieldMax=0.0;
            dArm1ByBinYieldMin=100.0;
            dArm2ByBinYieldMax=0.0;
            dArm2ByBinYieldMin=100.0;
            if(Prod.bSpecBinBySiteCompareEnable[iCat]==true &&
               Prod.dSpecBinBySiteComparePercent[iCat]>0 &&
               RunInfo.iUnloadCount-iYeildCT[5]>Prod.iSpecBinBySiteCompareIgnore[iCat])
            {
                if(CUSTOMER_CODE==CC_JCET)
                {
                    for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
                    {
                        for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                        {
                            if((LastSet.bUseTestSocket[0][iRow][iCol] ||        //jou 20171023 (wei) : 修正關site也會by bin site gap alarm
                                LastSet.bUseTestSocket[1][iRow][iCol]) &&
                               TastCategory.iCountSocketTotal[iRow][iCol]>0)
                            {
                                dSiteByBinYield=ChangeToFloat((double)(TastCategory.iCountCategory[0][iRow][iCol][iCat]+TastCategory.iCountCategory[1][iRow][iCol][iCat]), (double)TastCategory.iCountSocketTotal[iRow][iCol]);  //Steven 20250820 : 針對除以0加上保護
                            }
                            else
                            {
                                dSiteByBinYield=-1.0;
                                if(LastSet.bUseTestSocket[0][iRow][iCol]==false)                                        //jou 20171225 (Steven) : 修正被關掉的site再開site也會by bin site gap alarm
                                    TastCategory.iCountCategory[0][iRow][iCol][iCat]=0;
                                if(LastSet.bUseTestSocket[1][iRow][iCol]==false)
                                    TastCategory.iCountCategory[1][iRow][iCol][iCat]=0;
                            }

                            if(dSiteByBinYieldMax<dSiteByBinYield)
                            {
                                dSiteByBinYieldMax=dSiteByBinYield;
                                sWhichSiteByBinYieldMax=IndexSuckName[iRow][iCol];                                      //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                            }

                            if(dSiteByBinYieldMin>dSiteByBinYield && dSiteByBinYield!=-1.0)
                            {
                                dSiteByBinYieldMin=dSiteByBinYield;
                                sWhichSiteByBinYieldMin=IndexSuckName[iRow][iCol];                                      //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                            }
                        }
                    }

                    if((dSiteByBinYieldMax-dSiteByBinYieldMin)>Prod.dSpecBinBySiteComparePercent[iCat])
                    {
                        ErrPart=sWhichSiteByBinYieldMax+sWhichSiteByBinYieldMin;
                        DoLowYieldAlarm("WAR07335", ErrPart);                   //Steven 20180627 (wei) : 整合Low Yield Alarm
                        iYeildCT[5]=RunInfo.iUnloadCount;
                        PC_YIELDMON_CLEARCOUNT();
                    }
                }
                else
                {
                    for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
                    {
                        for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                        {
                            if((LastSet.bUseTestSocket[0][iRow][iCol] ||        //jou 20171023 (wei) : 修正關site也會by bin site gap alarm
                                LastSet.bUseTestSocket[1][iRow][iCol]) &&
                                TastCategory.iCountSocketTotal[iRow][iCol]>0)
                            {
                                dSiteByBinYield1[iRow*8+iCol]=ChangeToFloat((double)(TastCategory.iCountCategory[0][iRow][iCol][iCat]+TastCategory.iCountCategory[1][iRow][iCol][iCat]), (double)TastCategory.iCountSocketTotal[iRow][iCol]);
                            }
                            else
                            {
                                dSiteByBinYield1[iRow*8+iCol]=0.0;
                            }

                            if(dSiteByBinYieldMin>dSiteByBinYield1[iRow*8+iCol])
                            {
                                dSiteByBinYieldMin=dSiteByBinYield1[iRow*8+iCol];
                            }
                        }
                    }

                    for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
                    {
                        for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                        {
                            if((dSiteByBinYield1[iRow*8+iCol]-dSiteByBinYieldMin)>Prod.dSpecBinBySiteComparePercent[iCat] && bLowYeildAlarm==false)
                            {
                                bByBinPercentCompareAlarm=true;
                                ErrPart+=IndexSuckName[iRow][iCol];             //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                            }
                        }
                    }

                    if(bByBinPercentCompareAlarm && bLowYeildAlarm==false)
                    {
                        DoLowYieldAlarm("WAR07335", ErrPart);                   //Steven 20180627 (wei) : 整合Low Yield Alarm
                        iYeildCT[5]=RunInfo.iUnloadCount;
                        PC_YIELDMON_CLEARCOUNT();
                        bByBinPercentCompareAlarm=false;
                    }
                }
            }
            //Site by Arm
            if(Prod.bSpecBinByArmPerSiteCompareEnable[iCat]==true &&
               Prod.dSpecBinByArmPerSiteComparePercent[iCat]>0 &&
               RunInfo.iUnloadCount-iYeildCT[6]>Prod.iSpecBinByArmPerSiteCompareIgnore[iCat])
            {
                ErrPart="";
                for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
                {
                    for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                    {
                        if(TastCategory.iCountCategory[0][iRow][iCol][iCat]>0 && TastCategory.iCountHeadTotal[0][iRow][iCol]>0)
                            dArm1ByBinYield=ChangeToFloat((double)TastCategory.iCountCategory[0][iRow][iCol][iCat], (double)TastCategory.iCountHeadTotal[0][iRow][iCol]);
                        else
                            dArm1ByBinYield=0.0;
                        if(dArm1ByBinYieldMax<dArm1ByBinYield)
                        {
                            dArm1ByBinYieldMax=dArm1ByBinYield;
                            sArm1WhichSiteByBinYieldMax=IndexSuckName[iRow][iCol];                                      //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                        }

                        if(dArm1ByBinYieldMin>dArm1ByBinYield &&
                          (LastSet.bUseTestSocket[0][iRow][iCol] ||
                           LastSet.bUseTestSocket[1][iRow][iCol]))              //JerryYang 20250120 : modify
                        {
                            dArm1ByBinYieldMin=dArm1ByBinYield;
                            sArm1WhichSiteByBinYieldMin=IndexSuckName[iRow][iCol];                                      //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                        }

                        if(TastCategory.iCountCategory[0][iRow][iCol][iCat]>0 && TastCategory.iCountHeadTotal[0][iRow][iCol]>0)
                            dArm2ByBinYield=ChangeToFloat((double)TastCategory.iCountCategory[0][iRow][iCol][iCat], (double)TastCategory.iCountHeadTotal[0][iRow][iCol]);
                        else
                            dArm2ByBinYield=0.0;
                        if(dArm2ByBinYieldMax<dArm2ByBinYield)
                        {
                            dArm2ByBinYieldMax=dArm2ByBinYield;
                            sArm2WhichSiteByBinYieldMax=IndexSuckName[iRow][iCol];
                        }

                        if(dArm2ByBinYieldMin>dArm2ByBinYield)
                        {
                            dArm2ByBinYieldMin=dArm2ByBinYield;
                            sArm2WhichSiteByBinYieldMin=IndexSuckName[iRow][iCol];
                        }
                    }
                }

                if(Prod.bSpecBinByArmPerSiteCompareEnable[iCat]==true &&
                   Prod.dSpecBinByArmPerSiteComparePercent[iCat]>0 &&
                   RunInfo.iUnloadCount-iYeildCT[6]>Prod.iSpecBinByArmPerSiteCompareIgnore[iCat] &&
                   (dArm1ByBinYieldMax-dArm1ByBinYieldMin) > Prod.dSpecBinByArmPerSiteComparePercent[iCat])
                {
                    ErrPart=sArm1WhichSiteByBinYieldMax+sArm1WhichSiteByBinYieldMin;
                    DoLowYieldAlarm("WAR07336", ErrPart);                       //Steven 20180627 (wei) : 整合Low Yield Alarm
                    iYeildCT[6]=RunInfo.iUnloadCount;
                    PC_YIELDMON_CLEARCOUNT();
                }

                if(Prod.bSpecBinByArmPerSiteCompareEnable[iCat]==true &&
                    Prod.dSpecBinByArmPerSiteComparePercent[iCat]>0 &&
                    TastCategory.iTotalSocket>Prod.iSpecBinByArmPerSiteCompareIgnore[iCat] &&
                    RunInfo.iUnloadCount-iYeildCT[6]>Prod.iSpecBinBySiteCompareIgnore[iCat] &&
                  (dArm2ByBinYieldMax-dArm2ByBinYieldMin)>Prod.dSpecBinByArmPerSiteComparePercent[iCat])
                {
                    ErrPart=sArm2WhichSiteByBinYieldMax+sArm2WhichSiteByBinYieldMin;
                    DoLowYieldAlarm("WAR07337", ErrPart);                       //Steven 20180627 (wei) : 整合Low Yield Alarm
                    iYeildCT[6]=RunInfo.iUnloadCount;
                    PC_YIELDMON_CLEARCOUNT();
                }
            }
        }
    }
#endif

    if(bSocketAllSiteFail)                                                      //kevin 20170825 (Steven) all site fail
    {
        bSocketAllSiteFail=false;
        iallSitCount=0;                                                         //kevin 20180720 (wei) all site fail count
        DoLowYieldAlarmFirst("WAR07334", asErrPart);                            //Steven 20180627 (wei) : 整合Low Yield Alarm
    }

    if(bSpecialBinContinuousFailBySocket)                                       //ChungHung 20140730 add ContinuousFailHaveOneCycle start
    {
        PC_YIELDMON_CLEARCOUNT();                                               //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
        bSpecialBinContinuousFailBySocket=false;
        DoLowYieldAlarm("WAR07331", ErrPart);                                   //Steven 20180627 (wei) : 整合Low Yield Alarm
    }

    if(bSpecailBinContinuousFailByArm)
    {
        PC_YIELDMON_CLEARCOUNT();                                               //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
        bSpecailBinContinuousFailByArm=false;
        if(Index==0)
            DoLowYieldAlarm("WAR07332", asErrPart);                             //Steven 20180627 (wei) : 整合Low Yield Alarm
        else
            DoLowYieldAlarm("WAR07333", asErrPart);                             //Steven 20180627 (wei) : 整合Low Yield Alarm
    }

    AnsiString aLowYield;
    if(bSocketConsecutiveFailure_AutoClean)                                     //ChungHung 20131223 add for SCK request start
    {
        if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true &&
           (TestIF.iAutoClean_Mode & M_SOCKET_ALARM) && bConseFailureBySocket_AutoClean)
        {
            aLowYield = "AutoClean : Consecutive Failure by Socket : " + SocketErrPart + " Contact Count : " + iAutoClean_IndexContactCount;
            RecordProcess(aLowYield);
            // AI(W5-ProcessCount-Translate) 20260710: gate #2 (InitialAutoCleanAllTask,
            // AutoClean module not translated) -- see file header.
#if 0 // TODO(AutoClean-module): InitialAutoCleanAllTask (golden AutoClean/AutoClean.h:7) has no translated home yet.
            InitialAutoCleanAllTask();                                          //Sam 20230504 : 整理 InitialAutoCleanTask
#endif
        }
    }

    if(bConsecutiveFailure_AutoClean)
    {
        if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true &&
           (TestIF.iAutoClean_Mode & M_SOCKET_ALARM) && bConseFailureByHead_AutoClean)
        {
            aLowYield = "AutoClean : Consecutive Failure by Head : " + SocketErrPart + " Contact Count : " + iAutoClean_IndexContactCount;
            RecordProcess(aLowYield);
#if 0 // TODO(AutoClean-module): InitialAutoCleanAllTask (golden AutoClean/AutoClean.h:7) has no translated home yet.
            InitialAutoCleanAllTask();                                          //Sam 20230504 : 整理 InitialAutoCleanTask
#endif
        }
    }

    if(bDoAutoCleanByBinCount || bDoAutoCleanBySiteCount)                       //Steven 20160308 : By Bin count do auto clean
    {
        if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true)
        {
            if(bDoAutoCleanByBinCount)
                aLowYield = "AutoClean by Bin count : " + AutoCleanErrPart;
            else
                aLowYield = "AutoClean by site count: " + AutoCleanErrPart;
            RecordProcess(aLowYield);
#if 0 // TODO(AutoClean-module): InitialAutoCleanAllTask (golden AutoClean/AutoClean.h:7) has no translated home yet.
            InitialAutoCleanAllTask();                                          //Sam 20230504 : 整理 InitialAutoCleanTask
#endif
        }
    }

    if(bSocketConsecutiveFailure)                                               //ChungHung 20131223 add for SCK request end
    {
        DoLowYieldAlarm("WAR07301", SocketErrPart);                             //Steven 20180627 (wei) : 整合Low Yield Alarm
        PC_YIELDMON_CLEARCOUNT();                                               //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
    }

    if(bUseTwoArm32Site==true)
    {
        if(bSocketConsecutiveFailure)
            bConsecutiveFailure=false;
    }

    if(bConsecutiveFailure)
    {
        PC_YIELDMON_CLEARCOUNT();                                               //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
        if(Index==0)
            DoLowYieldAlarm("WAR07321", ErrPart);                               //Steven 20180627 (wei) : 整合Low Yield Alarm
        else
            DoLowYieldAlarm("WAR07329", ErrPart);                               //Steven 20180627 (wei) : 整合Low Yield Alarm
    }
}
//------------------------------------------------------------------------------
void ProcessContinuoussFailForATC(int Index)                                    //Steven 20151123 : Continue Fail Temp Offset for ATC
{
    int iGetBin=0;

    if(ATC_SYSTEM!=eATCHonPrecType || LastSet.iTemperature!=Tempture_Hot)
        return;

    for(int j=0; j<2; j++)                                                      //Steven 20151209 : Modify for ATC 7.0
    {
        if(TestSocket.Item[0][j]>=TEST_PASS)
        {
            iGetBin=TestSocket.Item[0][j]-TEST_PASS;
            if(iGetBin>(iTestBinCount-1))
            {
                Temperature.iATCCurrentFailCount[j]++;
                Temperature.iATCCurrentFailCount[j+2]++;
            }

            if(TestSocket.bPass[0][j])                                          //pass
            {
                Temperature.iATCCurrentFailCount[j]=0;
                Temperature.iATCCurrentFailCount[j+2]=0;
            }
            else
            {
                Temperature.iATCCurrentFailCount[j]++;
                Temperature.iATCCurrentFailCount[j+2]++;
            }
        }
    }
}
//---------------------------------------------------------------------------
int GetMultiplierNum()                                                          //JimmyChiu 20240411 : Dynamic multiplier for Continual Pass Bin( Socket )
{
    // AI(W5-ProcessCount-Translate) 20260710: gate #6 (fProductionInfo, golden
    // ProductionInfo.h:547/587) -- see file header.  Golden default when
    // IniConfig.bN14_24_DynaMultiContinuPassSocket==false (ProductionInfo.cpp:
    // 5955-5961) is 1 (no multiplier scaling); reproduced verbatim as the gate
    // default (this offline build never sets that INI flag, so 1 is exact).
#if 0 // TODO(W6.4b-integrate): fProductionInfo->cDynaThres->GetMultiplierNum() -- see file-header gate #6 for the exact shim + pre-existing cross-unit fProductionInfo naming conflict to resolve.
    return fProductionInfo->cDynaThres->GetMultiplierNum();
#else
    return 1;
#endif
}
//---------------------------------------------------------------------------
void CheckContinuoussPassBySocket(int Index)                                    //Steven 20110919
{
    unsigned int iGetBin=0;
    AnsiString ErrPart="";

    if(LastSet.iRunStartMode==rsmAutoSiteMap)                                   //Steven 20110506 : Auto Site Mapping時不啟動
    {
        return;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestSocket.Item[i][j]>=TEST_PASS)                                //ChungHung 20140516 fix 記憶體溢位
            {
                iGetBin=TestSocket.Item[i][j]-TEST_PASS;
                if(int(iGetBin)>(iTestBinCount-1))                              //Steven 20121112 : RS232支援32Bin 14->iTestBinCount-1
                    continue;

                if(Prod.bContinuousPassBySocket && Prod.iContinuousPassBinCountBySocket!=0 && TestSocket.bPass[i][j])   //pass
                {
                    ContinuousPassBinBySocket[i][j]++;
                    if(ContinuousPassBinBySocket[i][j]>=(Prod.iContinuousPassBinCountBySocket*GetMultiplierNum()))      //JimmyChiu 20240411 : Dynamic multiplier for Continual Pass Bin( Socket )
                    {
                        if(CosFunction.bPiggyBackShowMainForm && Prod.iCountAlarmAction==0)                             //kevin 20131101 強致 Enable Yield 裡面piggyback 功能    //Steven 20131101 : PiggyBack數量到達時,顯示在Main Form上面
                        {
                            bContinuoussPass=true;                              //秀警告不停機
                        }
                        else if(CosFunction.bPiggyBackShowMainForm && Prod.iCountAlarmAction==1)                        //kevin 20131101 強致 Enable Yield 裡面piggyback 功能   //Steven 20131101 : PiggyBack數量到達時,顯示在Main Form上面
                        {                                                       //index check
                            iWhoTriggerPiggyBack=pbtContinualPassBinBySocket;   //Steven 20111207 : 誰觸發了Piggy Back
                            ProcessPiggyBackFunction();                         //Steven 20110725 : 整合成function
                            bContinuoussPass=false;                             //  kevin 20131009改為全域  //Steven 20140923 : 移到上面
                        }
                        else if(Prod.iCountAlarmAction==0)
                        {
                            ContinuousPassBinBySocket[i][j]=0;
                            bContinuoussPass=true;
                            ErrPart+=IndexSuckName[i][j];                       //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                        }
                        else
                        {
                            bContinuoussPass=false;                             //  kevin 20131009改為全域
                            ContinuousPassBinBySocket[i][j]=0;
                            iWhoTriggerPiggyBack=pbtContinualPassBinBySocket;   //Steven 20111207 : 誰觸發了Piggy Back
                            ProcessPiggyBackFunction();                         //Steven 20110725 : 整合成function
                        }
                    }
                }
                else
                {
                    ContinuousPassBinBySocket[i][j]=0;
                }
            }
        }
    }

    if(bContinuoussPass)
    {
        if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true && (TestIF.iAutoClean_Mode & M_SOCKET_ALARM))
        {
#if 0 // TODO(AutoClean-module): InitialAutoCleanAllTask (golden AutoClean/AutoClean.h:7) has no translated home yet.
            InitialAutoCleanAllTask();                                          //Sam 20230504 : 整理 InitialAutoCleanTask
#endif
        }

        if(CosFunction.bPiggyBackShowMainForm==false)                           //kevin 20131009 高雄ASE不能停機只能畫面秀警告
        {
            DoLowYieldAlarm("WAR07322", ErrPart);                               //Steven 20180627 (wei) : 整合Low Yield Alarm
            bContinuoussPass=false;
            // AI(W5-ProcessCount-Translate) 20260710: gate #6 (fProductionInfo,
            // golden ProductionInfo.h:588) -- see file header + GetMultiplierNum()
            // above.  No-op offline: GetMultiplierNum() is gated to always return
            // 1, so skipping this threshold-bump has zero observable effect.
#if 0 // TODO(W6.4b-integrate): fProductionInfo->cDynaThres->AddThresholdNum() -- see file-header gate #6.
            fProductionInfo->cDynaThres->AddThresholdNum();                     //JimmyChiu 20240411 : Dynamic multiplier for Continual Pass Bin( Socket )
#endif
        }
    }
}
//---------------------------------------------------------------------------
void CheckContinuoussPass(int Index)
{
    unsigned int iGetBin=0;
    bool bContinuoussPass=false;
    AnsiString ErrPart="";

    if(LastSet.iRunStartMode==rsmAutoSiteMap)                                   //Steven 20110506 : Auto Site Mapping時不啟動
    {
        return;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestSocket.Item[i][j]>=TEST_PASS)                                //ChungHung 20140516 fix 記憶體溢位
            {
                iGetBin=TestSocket.Item[i][j]-TEST_PASS;
                if((int)iGetBin>(iTestBinCount-1))                              //Steven 20121112 : RS232支援32Bin 14->iTestBinCount-1
                    continue;

                if(Prod.bContinuousPass && ((Prod.iContinuousPassBin)==iGetBin) &&
                   Prod.iContinuousPassBinCount!=0 && TestSocket.bPass[i][j])   //pass    //Steven 20140922 : 因為沒有Bin 0,所以不用-1
                {
                    ContinuousPassBinCount[iGetBin]++;
                    if(ContinuousPassBinCount[iGetBin]>=Prod.iContinuousPassBinCount)
                    {
                        ContinuousPassBinCount[iGetBin]=0;
                        if(Prod.iCountAlarmAction==0)
                        {
                            bContinuoussPass=true;
                            ErrPart+=IndexSuckName[i][j];                       //JerryYang 20180815 (Steven) : 修正32 site Error part異常
                        }
                        else
                        {
                            iWhoTriggerPiggyBack=pbtContinualPassBin;           //Steven 20111207 : 誰觸發了Piggy Back
                            ProcessPiggyBackFunction();                         //Steven 20110725 : 整合成function
                        }
                    }
                }
                else
                {
                    ContinuousPassBinCount[Prod.iContinuousPassBin]=0;          //Steven 20210812 : iGetBin --> Prod.iContinuousPassBin
                }
            }
        }
    }

    if(bContinuoussPass)
    {
        DoLowYieldAlarm("WAR07322", ErrPart);                                   //Steven 20180627 (wei) : 整合Low Yield Alarm
    }
}
//---------------------------------------------------------------------------
void ProcessHeadContactCount(int Index)                                         //Ifor 20160516 京元要求銦片 Life Time 功能
{
    bool bHaveUseArm=true;
    int iPos;

    if(CosFunction.bUseHeadContactCount)
    {
        if(TestIF_File.iShuttleMode==1)
        {
            if(TestIF_File.iShuttle_Sel==0 && Index==1)
            {
                bHaveUseArm=false;
            }
            else if(TestIF_File.iShuttle_Sel==1 && Index==0)
            {
                bHaveUseArm=false;
            }
        }
        else
        {
            bHaveUseArm=true;
        }

        //Ifor 20160513 修改Arm2 Life Time 異常問題
        for(int iA=0; iA<3 ; iA++)                                              //wei 20160509
        {
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[Index][i][j])
                    {
                        iPos=j*2+i;
                        if(bHaveUseArm==true)
                        {
                            IniConfig.HeadContactCount[iA][Index][iPos]++;
                            IniConfig.HeadContactCountHistory[iA][Index][iPos]++;

                            if(IniConfig.HeadContactCountHistory[iA][Index][iPos]>=1000000000)
                                IniConfig.HeadContactCountHistory[iA][Index][iPos]=0;
                        }
                        iSLT_HeadContactCount[Index][iPos]=IniConfig.HeadContactCount[0][Index][iPos];                  //Ifor 20191218 : add KYEC 要求 同SLT輸出表格
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void ProcessSocketCount(int Index)
{
    //JerryYang 20170208 (wei) socket contact計數
    //==>
    if(CUSTOMER_CODE==CC_ASE_CL ||
       CUSTOMER_CODE==CC_PTI ||                                                 //Sam 20170526 (jou) PTI要求socket contact計數
       CUSTOMER_CODE==CC_TERAPOWER ||                                           //Sam 20190916 : 晶兆成也記錄測頭 Coun
       CUSTOMER_CODE==CC_ASE_JP ||                                              //Sam 20220706 : ASE_JP 也記錄測頭 Count
       CUSTOMER_CODE==CC_AMD_M ||
       CosFunction.bOEEFunction ||
       CosFunction.bUseSocketContactCount)                                      //Sam 20220720 : 新增一組 Socket Count
    {
        for(int i=0; i<FTestSuck.iShtRow; i++)
        {
            for(int j=0; j<FTestSuck.iShtCol; j++)
            {
                if(IsNNMode()==NN_2Row)
                {
                    if(LastSet.bUseTestSocket[0][i][j] &&
                       TestSocket.Item[i][j]!=NULL_IC &&
                       TestSocket.Item[i][j]!=HAS_NULL_IC)                      //Steven 20221216 : Socket count加上有無IC判斷
                    {
                        LastSet.iSocketContactCount[i][j]++;
                        if(LastSet.iSocketContactCount[i][j]>=1000000000 ||
                           LastSet.iSocketContactCount[i][j]<0)
                            LastSet.iSocketContactCount[i][j]=0;

                       //Sam 20220720 : 新增一組 Socket Count
                        IniConfig.SocketContactCount[i][j]++;
                        IniConfig.SocketContactCountHistory[i][j]++;
                        if(IniConfig.SocketContactCountHistory[i][j]>=1000000000 || IniConfig.SocketContactCountHistory[i][j]<0)
                            IniConfig.SocketContactCountHistory[i][j]=0;
                    }

                    if(LastSet.bUseTestSocket[1][i+2][j] &&                     //Sam 20201021 : Fix 32Site contact count
                       TestSocket.Item[i+2][j]!=NULL_IC &&
                       TestSocket.Item[i+2][j]!=HAS_NULL_IC)                    //Steven 20221216 : Socket count加上有無IC判斷
                    {
                        LastSet.iSocketContactCount[i+2][j]++;
                        if(LastSet.iSocketContactCount[i+2][j]>=1000000000 || LastSet.iSocketContactCount[i][j]<0)
                            LastSet.iSocketContactCount[i+2][j]=0;

                        //Sam 20220720 : 新增一組 Socket Count
                        IniConfig.SocketContactCount[i+2][j]++;
                        IniConfig.SocketContactCountHistory[i+2][j]++;
                        if(IniConfig.SocketContactCountHistory[i+2][j]>=1000000000 || IniConfig.SocketContactCountHistory[i][j]<0)
                            IniConfig.SocketContactCountHistory[i+2][j]=0;
                    }

                    if(LastSet.iSocketContactCount[i][j]>=1000000000 || LastSet.iSocketContactCount[i][j]<0)
                        LastSet.iSocketContactCount[i][j]=0;
                }
                else
                {
                    if(LastSet.bUseTestSocket[Index][i][j] &&
                       TestSocket.Item[i][j]!=NULL_IC && TestSocket.Item[i][j]!=HAS_NULL_IC)                            //Steven 20221216 : Socket count加上有無IC判斷
                    {
                        LastSet.iSocketContactCount[i][j]++;
                        if(LastSet.iSocketContactCount[i][j]>=1000000000 || LastSet.iSocketContactCount[i][j]<0)
                            LastSet.iSocketContactCount[i][j]=0;

                        //Sam 20220720 : 新增一組 Socket Count
                        IniConfig.SocketContactCount[i][j]++;
                        IniConfig.SocketContactCountHistory[i][j]++;
                        if(IniConfig.SocketContactCountHistory[i][j]>=1000000000 || IniConfig.SocketContactCountHistory[i][j]<0)
                            IniConfig.SocketContactCountHistory[i][j]=0;
                    }
                }
            }
        }
    }
    //<==
    //JerryYang 20170208 (wei) socket contact計數
}
//---------------------------------------------------------------------------
// kevin 20190907 add load cell Measure                                         //kevin 20190907 Arm 測區次數道量測 功能;
//---------------------------------------------------------------------------
void ProcessAutoloadcellMeasureCount()
{
    if(IniConfig.bD67LoadCellMeasure && iOneCycle==0)
    {
        if(TestIF.bLoadCellMeasure)                                             //kevin 20190907 Arm 測區次數道量測 功能;
            iloadcellCount++;
        else
            iloadcellCount=0;

        if(iloadcellCount>= TestIF.iLoadCellCount)
        {
            bIsAutoOneCycle=true;
            fMain->BtnOneCycleClick(fMain);
            iloadcellCount=0;
            AsebLoadCellTest=true;
        }
    }
}
