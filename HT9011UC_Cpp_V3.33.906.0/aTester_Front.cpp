//---------------------------------------------------------------------------
//  aTester_Front.cpp  --  TESTER/INDEX front (Z1) down-press test-head SM
//
//  Translation wave: W64b-Translate (front test-head destroy/down-press SM)
//  Translator: AI(W64b-Translate) 20260706
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/aTester_Front.cpp (8580 lines, cp950)
//
//  SCOPE (narrow, per translation-plan hand-off -- do NOT expand):
//    * bArm1PutDuplicateErr[MAX_SOCKET_ROW][MAX_SOCKET_COL]   golden :89-92
//    * ResetArm1PutFlag()                                     golden :127-136 (file-local; not in .h)
//    * CheckZ1IsDown()                                        golden :170-222
//    * TestZ1OutRandge()                                      golden :224-255
//    * TestZ1SetPos()                                         golden :291-298
//    * iFrontTestDestroyICTask + InitFrontTestDestroyICTask() golden :300-305
//    * FTempRiseHasICDelay (TQPF_Timer)                       golden :308 (file-scope global DoFrontTestDestroyIC needs; declared here, just outside the stated 309-869 range)
//    * DoFrontTestDestroyIC(bool bCheckZ2)                    golden :309-865 (full switch(Task) SM, verbatim)
//  Every OTHER golden aTester_Front.cpp function (DoFrontTestSuckIC, DoTestYFront,
//  TestZ1OutRandge2, DoArm1Suck, DoArm1D44VacCheck, FTestSeparateSLK/CombineSLK,
//  DoFRTCAutoModelVerify, DoFrontTestPurgBeforePickShuttle, ...) is OUT OF SCOPE
//  this wave and stays served by the existing atester_shims.h/.cpp offline shims.
//
//  TRANSLATION RULES (mirrors atester.cpp/ainarm9045.cpp conventions)
//  --------------------------------------------------------------------
//    * Off Borland: __fastcall removed; AnsiString/TQPF_Timer via vclcompat;
//      numeric semantics EXACT (no int/float rewrites).
//    * SOFT_SIMULTE is NOT defined in this target build (MachineType.h:30 keeps
//      the golden DEBUG_INDEX_UPH toggle commented out the same way) -- the
//      #ifdef SOFT_SIMULTE / #else and #ifdef DEBUG_INDEX_UPH / #else blocks are
//      reproduced VERBATIM (both branches) exactly as golden; the preprocessor
//      selects the #else branch today, same as every other translated engine file.
//    * Big5 Chinese comments decoded via cp950, preserved as UTF-8. ZERO U+FFFD.
//
//  GATED DEPENDENCIES (verified ABSENT from the already-translated substrate;
//  NOT part of the pre-verified call-graph list handed to this wave -- discovered
//  while translating DoFrontTestDestroyIC's body and gated per the "#if 0 //
//  TODO(W7)" convention, each with a minimal compiling default that mirrors the
//  golden park/default behaviour for that branch):
//    1. FTestSuck.bNeedCheck[][]              -- TMyKitSuck (aHotPlateSubstrate.h)
//       has no per-cell "D44 recheck" flag member yet (golden MyKitSuck.h).
//       Reads default false / writes no-op via W64B_NEEDCHECK_GET/SET (mirrors
//       IniConfig.bD44CheckIndexICDestroy's own zero-initialized default).
//    2. TMySucker::sRecordStartOffDelayTime / sRecordEndOffDelayTime /
//       iRecordOffDelayTime + the QueueAirOnTime1.Add() telemetry call that
//       reads them -- absent from the minimal TMySucker shim; telemetry-only,
//       parked whole (golden :502-503).
//    3. FRCarryKit.MoveSuckData(TMyKitSuck&,int,int) -- no such TMyKitSuck method
//       yet; parked (golden :505).
//    4. TMyProductionRecord::AddIndexPlaceShuttleRecord -- not in the translated
//       Public/MyProductionRecord.h yet; parked (golden :506).
//    5. RecordIndexAirOnTime1() -- golden cObserver.h, no translated home yet;
//       parked no-op (golden :559, :569).
//    6. LastSet.iIndexInputOutPut[] -- not in the LAST_GENERAL_SET minimal shim
//       (canary_support.h) yet; parked no-op (golden :500).
//    7. DoVibrateOutShuttle(int,bool) -- golden ainarm2.h, no translated home yet;
//       void call parked no-op (golden :564); bool-checked call defaults to
//       true ("vibrate done") so the SM is not stuck park-waiting, mirroring the
//       fiosetview ProcessIndexSuckDestroy1/2 "suck self-check done" precedent
//       already established in atester.cpp (golden :626).
//    8. fiosetview->ProcessIndexSuckDestroy1(int) -- TfiosetviewShim
//       (atester_shims.h) exposes bIndexSuck[][][] but not this method (SAME
//       documented gap atester.cpp already gates via W7T1_FIOSET_PISD1/2); this
//       file's own W64B_FIOSET_PISD1 mirrors that exact precedent (golden :666,
//       :709), default true ("done").
//    9. FRCarryKit.SetHasNullIcToNullIc() -- no such TMyKitSuck method yet (SAME
//       documented gap as csystem.cpp's W7C2_BLCARRYKIT_SETHASNULLIC); parked
//       no-op via W64B_FRCARRYKIT_SETHASNULLIC (golden :675, :860).
//   10. ATC_TYPE_33 / ATC_TYPE_35 -- golden ATC_Handler_Side.h manifest constants
//       (plain #define 33 / 35); that header has no translated home yet, but the
//       values are unambiguous data (not a behavior gate) -- defined locally.
//   11. TMyProductionRecord::AddIndexCycleTimeRecord -- Public/MyProductionRecord.h
//       is a declaration-only forward-stub this wave (no .cpp); parked, same
//       treatment as gate #4's AddIndexPlaceShuttleRecord (golden :501).
//       AI(W64b-Integrate) 20260706.
//  Every gate is individually re-cited at its call site below.
//
//  INTEGRATE-STEP FIXES (AI(W64b-Integrate) 20260706 -- discovered as
//  undefined-reference LINK errors, not caught by either translator's report
//  since neither is a "call graph" gap in the usual sense -- both are golden
//  symbols DECLARED in an already-translated header with NO compiled body
//  anywhere in the tree):
//    * OutSHT1InLF() / CheckIndexAllSuckICFallDown() (golden csystem.cpp:698 /
//      :2417) -- csystem.h declared them but csystem_predicates.cpp never
//      defined them.  Golden bodies translated FAITHFULLY (not gated) into
//      csystem_predicates.cpp -- OutSHT1InLF is a trivial one-line delegate,
//      CheckIndexAllSuckICFallDown is a self-contained FTestSuck/BTestSuck
//      grid scan with no further substrate gaps.
//    * GetSiteCount(bool) (golden cprod.cpp:305, declared cprod.h:3299) --
//      the real body exists in cprod.cpp but sits inside that file's whole-
//      region `#if 0 // TODO(W6)` gate (cprod.cpp:184-4036), so it never
//      compiled.  A faithful duplicate was added to atester_shims.cpp (same
//      "shim a real-but-gated-elsewhere body" pattern already used there for
//      IsInArmCleanOutFinish/IsInArmOneCycleFinish); TODO(W6): delete the
//      duplicate once cprod.cpp's gate lifts.
//  Integrate-step note: atester_shims.h/.cpp TODAY still declare+define
//  offline-stub bodies for all 6 symbols this file now defines for real
//  (iFrontTestDestroyICTask, InitFrontTestDestroyICTask, DoFrontTestDestroyIC,
//  CheckZ1IsDown, TestZ1OutRandge, TestZ1SetPos) -- REMOVED from
//  atester_shims.h/.cpp as part of this same integrate step to avoid a
//  duplicate-definition link error.
//---------------------------------------------------------------------------
#include "aTester_Front.h"
#include "aTester_Rear.h"           // CheckZ2IsDown() -- golden aTester_Rear.h; sibling W64b-Translate wave (translated in parallel; this header did not exist yet at the time this file was written -- see final report)
#include "atester_shims.h"          // IsNNMode() (golden cinitial.h:60, offline shim) -- ALSO still stubs the 6 symbols above; see integrate-step note
// AI(W5-Automation-Integrate) 20260710: ProcessPiggyBackFunction is now real
// (atester_ProcessCount.cpp, removed from atester_shims.h) -- include its own header.
#include "atester_ProcessCount.h"

#include "aArmHeader.h"             // __FUNC__ shim
#include "MachineType.h"            // MAX_SOCKET_ROW/COL, NN_2Row, eSpeedPart::IndexArm, ChangeToFloatNonPcnt
#include "Motor/mymotor.h"          // MOT[], TTrayMotor, RecordIndexPositionError, ShowIndexMotorError
#include "csystem.h"                // OutSHT1InLF/InShtInLF/InSHT1InRT/CheckIndexAllSuckICFallDown/IsInArmOneCycleFinish/IsInArmCleanOutFinish
#include "atester.h"                // GetIndexZSpeed/IndexAlarmInArmAway/IndexCheck4Site/DoTemperatureRise
#include "cmydef.h"                 // global scalar universe + IC consts + enums
#include "cprod.h"                  // Prod / TestIF / TestIF_File / CosFunction / ArmSpeed_File / GetSiteCount
#include "aHotPlateSubstrate.h"     // FTestSuck / FRCarryKit / FLCarryKit / TestSocket
#include "acarry_shims.h"           // ATC_InterfaceForm (golden ATC_Handler_Side.h, offline shim)
#include "canary_support.h"         // LastSet / ShowMyMessage / ShowErrorMessage
#include "FormsFacade.h"            // Wave 2: fMain->ResetRecordforPiggyBack / fSCKART->iInputJamCnt+AddOutputJamCnt
//---------------------------------------------------------------------------

// AI(W64b-Translate) 20260706: golden ATC_Handler_Side.h:24-25 manifest constants
// (plain int literals; that header has no translated home yet, but the values are
// unambiguous data, not a behavior gate) -- needed by the ATC-type compare in
// DoFrontTestDestroyIC case 200/500.
#define ATC_TYPE_33 33
#define ATC_TYPE_35 35

// AI(W64b-Translate) 20260706: golden :308 -- file-scope TQPF_Timer DoFrontTestDestroyIC
// needs (Index-arm temperature-rise soak delay).  Declared here, just outside the
// stated 309-869 translation range, because it is otherwise undeclared anywhere in
// the target tree.  (golden :307 TQPF_Timer CheckSocketHasICDelay1 is NOT declared --
// every golden reference to it inside DoFrontTestDestroyIC is itself commented out,
// so it is dead text, preserved verbatim as a comment below, not a live symbol.)
TQPF_Timer FTempRiseHasICDelay;                                                 //Ifor 20230418 add

// AI(W64b-Translate) 20260706: golden :89-92 -- per-cell "already alarmed this site"
// dedupe flag DoFrontTestDestroyIC reads/writes; file-local (golden aTester_Front.h
// does not declare it either).
bool bArm1PutDuplicateErr[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},    //Steven 20100105 Start: 排除重複的Alarm Code
                                                           {false, false, false, false, false, false, false, false},
                                                           {false, false, false, false, false, false, false, false},
                                                           {false, false, false, false, false, false, false, false}};

// ---------------------------------------------------------------------------
//  Gate helpers (TODO(W7) -- see file-header "GATED DEPENDENCIES" list above for
//  the full rationale of each).  Each mirrors the golden default/park behaviour
//  for its branch so DoFrontTestDestroyIC's cursor flow compiles + pumps
//  unchanged when the gated sub-feature is (as today, by default) inert.
// ---------------------------------------------------------------------------
#if 0 // TODO(W7) -- golden aTester_Front.cpp (case 500/600) FTestSuck.bNeedCheck[][] real member (aHotPlateSubstrate.h has no home for it yet)
#define W64B_NEEDCHECK_GET(i,j)      (FTestSuck.bNeedCheck[i][j])
#define W64B_NEEDCHECK_SET(i,j,v)    (FTestSuck.bNeedCheck[i][j]=(v))
#else
#define W64B_NEEDCHECK_GET(i,j)      (false)
#define W64B_NEEDCHECK_SET(i,j,v)    do{}while(0)
#endif

// AI(W64b-Translate) 20260706: mirrors atester.cpp's own W7T1_FIOSET_PISD1/2
// precedent for the SAME documented gap (TfiosetviewShim has bIndexSuck[][][]
// but not the ProcessIndexSuckDestroy1/2 pump methods, golden iosetview.h).
// Offline (no DAQ): report the suck self-check "complete" (true) so the D44
// recheck SM advances instead of parking forever.
static bool W64B_ProcessIndexSuckDestroy1(int /*iType*/){ return true; }        // golden iosetview.h:3018 -- offline: suck self-check done (true)
#define W64B_FIOSET_PISD1(iType)     W64B_ProcessIndexSuckDestroy1(iType)

// AI(W64b-Translate) 20260706: same documented gap as csystem.cpp's
// W7C2_BLCARRYKIT_SETHASNULLIC() (TMyKitSuck has no SetHasNullIcToNullIc()
// method yet, golden MyKitSuck.h; docs/MIGRATION_ROADMAP.md).
#define W64B_FRCARRYKIT_SETHASNULLIC()     do { } while(0)   // golden FRCarryKit.SetHasNullIcToNullIc()

//------------------------------------------------------------------------------
void ResetArm1PutFlag()                                                         //Steven 20241017 : 打包成function
{
    for(int i=0; i<FTestSuck.iMaxRow; i++)
    {
        for(int j=0; j<FTestSuck.iMaxCol; j++)
        {
            bArm1PutDuplicateErr[i][j]=false;
        }
    }
}
//------------------------------------------------------------------------------
bool CheckZ1IsDown()
{
    #ifdef SOFT_SIMULTE
        return true;
    #else
        long lPos[4]={0, 0, 0, 0};                                              //kevin 20150915

        if(IniConfig.bD52InterFaceErrHeadNeedUp && bNowDoInterFaceErrorStep)
            return false;

        if(bContactTimeOverStep)
            return false;

        if(MOT[MTestZ1].Motor->Enable==false)
            return true;

        if(CosFunction.bSortingBy2DList==true &&
           LastSet.iTester==_2D_SORT &&
           TestIF_File.bSortingBy2DIDList==true)                                //Frank 20221122 : 2DID sorting for ATK
        {
            return true;
        }

        if(TestIF_File.iShuttleMode==1 &&                                       //20111114  Dell for Disable Index Arm    Start
           TestIF_File.iShuttle_Sel==1)                                         //jou 2014-11-13 remake code , close arm index postion error
        {
            return true;
        }

        if(bUseTwoArm32Site==true)
        {                                                                       //nn Mode 是同時有吸取IC動作
            return true;
        }
        else
        {
            //==> Eastsun 20260511 F006 整合: Ifor 20240430 add:secs gem cmd Index Yiel Fail
            if(CUSTOMER_CODE==CC_KYEC_LEE && iSECSGEM_ConsecutiveFailureAlarm==2)//Ifor 20240430 add:secs gem cmd Index Yiel Fail 0:Normal 1:收到命令 2:Index 上升
            {
                return true;
            }
            //<== Eastsun 20260511 F006 整合
            else if(MOT[MTestZ1].Gali_ReadEncoderPos()>Prod.iTestDown_Z1)
            {
                lPos[0]=MOT[MTestZ1].Gali_ReadEncoderPos();                     //kevin 20150915
                lPos[1]=Prod.iTestDown_Z1;                                      //kevin 20150915
                RecordIndexPositionError("CheckZ1IsDown()", true, true, false, false, &lPos[0]);                        //kevin 20150915 record
                ShowIndexMotorError(AnsiString("CheckZ1IsDown"));
                return false;
            }
        }
        return true;
    #endif
}
//==============================================================================
bool TestZ1OutRandge()
{
    #ifdef SOFT_SIMULTE
        return false;
    #else
        int Pos1=MOT[MTestZ1].Gali_ReadPos();
        int Pos2=MOT[MTestZ1].Gali_ReadEncoderPos();
        Pos1-=Pos2;
        if(Pos1<0)
            Pos1=0-Pos1;

        if(Pos1>10)                                                             //Steven 20100728 :縮小範圍
            return true;
        Pos1=MOT[MTestY1].Gali_ReadPos();
        Pos2=MOT[MTestY1].Gali_ReadEncoderPos();
        Pos1-=Pos2;
        if(Pos1<0)
            Pos1=0-Pos1;

        if(Pos1>10)                                                             //Steven 20100728 :縮小範圍
            return true;
        Pos1=MOT[MTestY2].Gali_ReadPos();
        Pos2=MOT[MTestY2].Gali_ReadEncoderPos();
        Pos1-=Pos2;
        if(Pos1<0)
            Pos1=0-Pos1;

        if(Pos1>10)                                                             //Steven 20100728 :縮小範圍
            return true;
        return false;
    #endif
}
//==============================================================================
void TestZ1SetPos()
{
    int Pos1=ChangeToFloatNonPcnt((double)(MOT[MTestY1].Gali_ReadEncoderPos()), (double)(MOT[MTestY1].Motor->GearRatio));                                       //Steven 20100728 Start: 要加上齒輪比
    int Pos2=ChangeToFloatNonPcnt((double)(MOT[MTestZ1].Gali_ReadEncoderPos()), (double)(MOT[MTestZ1].Motor->GearRatio));
    char str[256];
    sprintf(str, "DP%d,%d", Pos1, 0-Pos2);
    MOT[MTestY1].Gali_Command(str, __FUNC__);
}
//---------------------------------------------------------------------------
int iFrontTestDestroyICTask=1;
void InitFrontTestDestroyICTask()
{
    iFrontTestDestroyICTask=1;
    bZ1Isdownflag=false;
}
//------------------------------------------------------------------------------
bool DoFrontTestDestroyIC(bool bCheckZ2)
{
    static int iRetry=0;
    static int iSiteCount=0;                                                    //kevin 20190709  index check one by one

    int &Task=iFrontTestDestroyICTask;
    int sp=0, iPos=0;
    int iNN=IsNNMode();
    bool flag=false, flag2=false;
    bool bHasDuplicateErr=false;
    bool bHasErr=false;
    AnsiString sTempMessge="";
    AnsiString ErrPart="";
    static bool bHasDropErr=false;

    switch(Task)
    {
        case 1:
            iSiteCount=0;                                                       //kevin 20190819  index check one by one
            bHasDropErr=false;
            iIndexTakeDeviceChk1=0;                                             //Steven 20250224 回吸檢測狀態
            if(FRCarryKit.UseSiteHasIC())
            {
                return false;
            }

            if(OutSHT1InLF()==false)
            {
                Task=100;                                                       //JerryYang 20170106 (Steven) fix out shuttle 偵測殘料後hang up
                return false;
            }

            MOT[MInShuttle1].fCanMoveM=false;
            Task=200;

            if(CheckIndexAllSuckICFallDown(true, false))                        //jou 20210713 : 增加index destroy ic時掉料偵測
            {
                ShowMyMessage("Index Arm1 掉料，請檢查IC是否有在Out Shuttle中!!");
            }

            if(iNN==NN_2Row)
                break;
            break;                                                              //JerryYang 20170106 (Steven) fix out shuttle 偵測殘料後hang up
        case 100:
            #ifdef DEBUG_INDEX_UPH
            if(MOT[MTestZ1].Gali_MotMove2(Prod.TestZ1_Safe, iIndexSpeed, iIndexAcc);                                    //Steven 20170818 : 測試UPH用
            #else
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Safe, MOT[MTestZ1].GailSpeed))
            #endif
            {
                if(bHasDropErr)                                                                                         //JerryYang 20241122 : Index arm放料到shuttle偵測drop error
                {
                    bHasDropErr=false;
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(FTestSuck.Suck[i][j].Enable       &&
                               FTestSuck.Suck[i][j].SenUsing!="" &&
                               FTestSuck.Item[i][j]!=HAS_NULL_IC &&
                               FTestSuck.Item[i][j]!=NULL_IC)
                            {
                                if(FTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    ErrPart+=IndexSuckName[i+iNN][j];
                                    bHasErr=true;
                                }
                            }
                        }
                    }

                    if(LastSet.iRealDummy==REALLY && bHasErr)
                    {
                        if(CosFunction.bJAM0303NeedOpenChamberDoor)                                                     //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
                            bIsTestSitICFallDown=true;
                        ShowErrorMessage("JAM0303", K_SKIP, MTestZ1, false, ErrPart);                                   //Steven 20100129 : Device Drop Error
                        for(int i=0; i<MAX_Index_Row; i++)
                        {
                            for(int j=0; j<NEW_MAX_Index_Col; j++)
                            {
                                if(FTestSuck.Suck[i][j].Error ||
                                   (FTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                    FTestSuck.Item[i][j]!=NULL_IC &&
                                    FTestSuck.Suck[i][j].GetStatus()==false))                                           //有用到且有吸到IC的卻掉了
                                {
                                    FTestSuck.SetItemData(i, j, HAS_NULL_IC);                                           //Steven 20110829 : 把有IC掉料的位置改成Has Null IC
                                    FTestSuck.Suck[i][j].Normal();                                                      //Steven 20110829 : 把真空關掉
                                }
                            }
                        }
                    }
                }
                Task=1;
            }
            break;
        case 200:
            bHasDropErr|=CheckIndexAllSuckICFallDown(true, false);              //JerryYang 20241122 : Index arm放料到shuttle偵測drop error
            if(bHasDropErr)
            {
                Task=100;
                break;
            }

            if(InShtInLF(0)==false)                                             //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條
            {
                ShowMyMessage("Shuttle 1 position error ", "Shuttle 1 位置錯誤");
                MOT[MInShuttle1].fCanMoveM=true;
                bZ1Isdownflag=false;                                            //Isaac 20180307 (Steven) index arm在shuttle放置位子時，shuttle抖抖須等到arm起來才能抖
                Task=1;
            }
            else if(bCheckZ2==false || CheckZ2IsDown())                         //ChungHung 20111115
            {
                flag2=false;
                bZ1Isdownflag=true;                                             //Isaac 20180307 (Steven) index arm在shuttle放置位子時，shuttle抖抖須等到arm起來才能抖
                if(FTestSuck.HasRealIC()==false)
                {
                    flag2=true;
                }
                else
                {
                    bSuckingFlagZ1=true;                                        //Steven 20240916 : index下降到shuttle吸放料
                    if(IniConfig.bD54SlowDown)                                  //ChungHung 20110816 add Index Place slow down
                    {
                        sp=GetIndexZSpeed(0);                                   //Steven 20160524 : Index Z軸速度整合為Function
                        #ifdef DEBUG_INDEX_UPH
                        if(MOT[MTestZ1].Gali_MotMove2(Prod.TestZ1_Place, iIndexSpeed, iIndexAcc))
                        #else
                        if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Place, sp))
                        #endif
                        {
                            flag2=true;
                        }
                    }
                    else
                    {
                        #ifdef DEBUG_INDEX_UPH
                        if(MOT[MTestZ1].Gali_MotMove2(Prod.TestZ1_Place, iIndexSpeed, iIndexAcc))
                        #else
                        if(MOT[MTestZ1].Gali_MotMoveNoWait(Prod.TestZ1_Place, MOT[MTestZ1].GailSpeed, IniConfig.iD46WaitIndexDestroyTime))
                        #endif
                        {
                            flag2=true;
                        }
                    }
                }

                if(flag2)
                {
                    FTestSuck.ResetAll();
                    if(ATC_SYSTEM==eNewATCSystem && Temperature.bEnableTempRise==true && (ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33 || ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35))
                    {
                        DoTemperatureRise(0, true);
                        FTempRiseHasICDelay.SetSecAndOn(Temperature.dTempRiseDelay);
                        Task=309;
                    }
                    else
                    {
                        Task=310;
                    }
                }
            }
            break;
        case 309:
            if(FTempRiseHasICDelay.Off())
            {
                DoTemperatureRise(0, false);
                Task=310;
            }
            break;
        case 310:
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FTestSuck.Item[i][j])
                    {
                        if(FTestSuck.Item[i][j]==HAS_NULL_IC ||
                           FTestSuck.Suck[i][j].Destroy())                                                              //放下IC
                        {
                            if(IniConfig.bD44CheckIndexICDestroy)
                            {
                                if(FTestSuck.Item[i][j]!=HAS_NULL_IC)
                                {
                                    FTestSuck.Suck[i][j].Off();                                                         //Suck Off and Destroy On
                                    W64B_NEEDCHECK_SET(i, j, true);                                                     //Destroy finish -- TODO(W7) golden :493 FTestSuck.bNeedCheck[i][j]=true (see file-header gate #1)
                                    iIndexTakeDeviceChk1=2;                                                             //kevin 20190103 回吸檢測狀態
                                }
                            }

                            if(FTestSuck.Item[i][j]!=HAS_NULL_IC)                                                       //kevin 20130125
                            {
#if 0 // TODO(W7) -- golden aTester_Front.cpp:500 LastSet.iIndexInputOutPut[][] (LAST_GENERAL_SET minimal shim, canary_support.h, has no such field yet)
                                LastSet.iIndexInputOutPut[1]++;                                                         //kevin 20130125
#endif
// AI(W64b-Integrate) 20260706: TMyProductionRecord::AddIndexCycleTimeRecord is
// declared (Public/MyProductionRecord.h:38) but that whole class is a
// declaration-only forward-stub this wave (no .cpp; see its file-top banner) --
// same documented gap as the sibling AddIndexPlaceShuttleRecord below; gated the
// same way (pure logging, no state the SM reads back).
#if 0 // TODO(W7) -- golden aTester_Front.cpp:501 FTestSuck.PordRec[i][j].AddIndexCycleTimeRecord (TMyProductionRecord has no .cpp yet)
                                FTestSuck.PordRec[i][j].AddIndexCycleTimeRecord(fRecindexCycleTim);                     //Frank 20160505 add
#endif
#if 0 // TODO(W7) -- golden aTester_Front.cpp:502-503 CosFunction.RecordIndexAirOnTime telemetry (TMySucker has no sRecordStartOffDelayTime/sRecordEndOffDelayTime/iRecordOffDelayTime fields yet)
                                if(CosFunction.RecordIndexAirOnTime)                                                    //Sam 20220329 : Record Index Air On Time
                                    QueueAirOnTime1.Add(FTestSuck.Suck[i][j].sRecordStartOffDelayTime, FTestSuck.Suck[i][j].sRecordEndOffDelayTime, FTestSuck.Suck[i][j].iRecordOffDelayTime);
#endif
                            }
#if 0 // TODO(W7) -- golden aTester_Front.cpp:505 FRCarryKit.MoveSuckData (TMyKitSuck substrate, no such method yet)
                            FRCarryKit.MoveSuckData(FTestSuck, i, j);
#endif
#if 0 // TODO(W7) -- golden aTester_Front.cpp:506 FRCarryKit.PordRec[][].AddIndexPlaceShuttleRecord (TMyProductionRecord, no such method yet)
                            FRCarryKit.PordRec[i][j].AddIndexPlaceShuttleRecord(TestIF.iSiteMap[i+iNN][j], i+iNN, j);   //Sam 20201216 : Add record
#endif
                            bArm1PutDuplicateErr[i][j]=false;
                        }
                    }
                }
            }
            flag=false;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FTestSuck.Item[i][j] && FTestSuck.Suck[i][j].Error==false)
                    {
                        flag=true;
                        break;
                    }
                }
            }

            if(flag==false)
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Suck[i][j].Error)
                        {
                            Task=320;
                            return false;
                        }
                    }
                }
            }

            if(FTestSuck.UseSiteHasIC())
            {
                iRetry=0;
                break;
            }

            if(TestZ1OutRandge())
            {
                iRetry++;
                if(iRetry>20)
                {
                    iRetry=0;
                    TestZ1SetPos();
                }
                else
                {
                    break;
                }
            }
#if 0 // TODO(W7) -- golden aTester_Front.cpp:559 RecordIndexAirOnTime1 (cObserver.h, no translated home yet)
            RecordIndexAirOnTime1();                                                                                    //Sam 20220329 : Record Index Air On Time
#endif
            iRetry=0;
            ResetArm1PutFlag();                                                                                         //Steven 20241017 : 打包成function
            if(IniConfig.bF25VibrateForOutShuttle)
            {
#if 0 // TODO(W7) -- golden aTester_Front.cpp:564 DoVibrateOutShuttle (ainarm2.h, no translated home yet)
                DoVibrateOutShuttle(0, true);
#endif
            }
            Task=500;
            break;
        case 320:                                                                                                       //破壞異常處理
#if 0 // TODO(W7) -- golden aTester_Front.cpp:569 RecordIndexAirOnTime1 (cObserver.h, no translated home yet)
            RecordIndexAirOnTime1();                                                                                    //Sam 20220329 : Record Index Air On Time
#endif
            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(bArm1PutDuplicateErr[i][j])
                        bHasDuplicateErr=true;
                    if(FTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i+iNN][j];
                        FTestSuck.Suck[i][j].Error=false;                                                               //Steven 20101229 : 換位置
                        bArm1PutDuplicateErr[i][j]=true;                                                                //Steven 20101229 : 換位置
                    }
                    else
                    {
                        bArm1PutDuplicateErr[i][j]=false;
                    }
                }
            }

            if(bHasErr)
                ShowErrorMessage("JAM0327", K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);                               //Vacuum sensor OFF error
            bHasErr=false;
            if(ATC_SYSTEM==eNewATCSystem && Temperature.bEnableTempRise==true && (ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33 || ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35))
            {
                DoTemperatureRise(0, true);
                FTempRiseHasICDelay.SetSecAndOn(Temperature.dTempRiseDelay);
                Task=309;
            }
            else
            {
                Task=310;
            }
            FTestSuck.ResetAll();                                                                                       //Sam 20220330 : 補 ResetAll
            break;
        case 500:
            if(IniConfig.bL42_UseOutShuttleDesoakTime==true)                    //Ztex 2023.04.19 Add HT-1032 TriTemp Function
            {
                FTestSuck.TSoakTime.SetSecAndOn(IniConfig.iL42_UseOutShuttleDesoakTime);
            }
            iPos=Prod.TestZ1_Safe;
            sp=GetIndexZSpeed(0);                                               //Steven 20160524 : Index Z軸速度整合為Function
            #ifdef DEBUG_INDEX_UPH
            if(MOT[MTestZ1].Gali_MotMove2(iPos, iIndexSpeed, iIndexAcc))
            #else
            if(MOT[MTestZ1].Gali_MotMoveNoWait(iPos, sp, 0))
            #endif
            {
                bSuckingFlagZ1=false;                                           //Steven 20240916 : index下降到shuttle吸放料
                bZ1Isdownflag=false;                                            //Isaac 20180307 (Steven) index arm在shuttle放置位子時，shuttle抖抖須等到arm起來才能抖  //Steven 20180522 : 換位置, 解除Hang up
                flag=false;
                if(IniConfig.bF25VibrateForOutShuttle)
                {
                    // golden :626 -- if(DoVibrateOutShuttle(0)==false) { break; } -- ainarm2.h, no translated
                    // home yet.  Default: vibrate-mechanism reports done (true) so the SM advances instead of
                    // parking here forever (mirrors the fiosetview ProcessIndexSuckDestroy1/2 "done" default below).
#if 0 // TODO(W7) -- golden aTester_Front.cpp:626 DoVibrateOutShuttle (ainarm2.h, no translated home yet)
                    if(DoVibrateOutShuttle(0)==false)
                    {
                        break;
                    }
#endif
                }
                MOT[MInShuttle1].fCanMoveM=true;                                //JerryYang 20181219 fix D44異常時 shuttle剛好要搖搖會發生hang up
                if(ArmSpeed_File[IndexArm].bDevicConfirm &&
                   INDEX_SUCKER_TYPE==1)                                        //kevin 20190819 add index confim 一次4個SITE
                {
                    TotalErrPart="";
                    Task=650;
                    return false;
                }

                if(IniConfig.bD44CheckIndexICDestroy)
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(W64B_NEEDCHECK_GET(i, j))                        //Suck On Destry Off -- TODO(W7) golden :646 FTestSuck.bNeedCheck[i][j] (see file-header gate #1)
                            {
                                bIndexPlaceIcCheck=true;                        //kevin 20110505
                                flag=true;
                                if(INDEX_SUCKER_TYPE==1)                        //Steven 20111202
                                {
                                    fiosetview->bIndexSuck[0][i][j]=true;
                                }
                                else
                                {
                                    FTestSuck.Suck[i][j].On();                  //kevin 20110504 check 掉料
                                }
                            }
                        }
                    }
                }

                if(flag)
                {
                    bD44Arm1CheckVacOn=true;                                    //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                    W64B_FIOSET_PISD1(1);                                       //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆 //kevin 20181108 add 提前動作 -- TODO(W7) golden :666 fiosetview->ProcessIndexSuckDestroy1 (see file-header gate #8)
                    DoFrontTestDestroyICDelay.SetMSAndOn(IniConfig.iD44TestHeadCheckVacuumTime);
                    Task=600;
                    break;
                }
                else
                {
                    bD44Arm1CheckVacOn=false;                                   //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                }
                W64B_FRCARRYKIT_SETHASNULLIC();                                 //jou 2011-06-29 : out shuttle has_null_ic hang up fix  //Steven 20160524 : ADD SetHasNullIcToNullIc -- TODO(W7) golden :675 FRCarryKit.SetHasNullIcToNullIc (see file-header gate #9)
                if(IniConfig.bD44CheckIndexICDestroy)
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            bArm1PutDuplicateErr[i][j]=false;
                            if(W64B_NEEDCHECK_GET(i, j))                        // Destroy Off -- TODO(W7) golden :683 FTestSuck.bNeedCheck[i][j] (see file-header gate #1)
                            {
                                FTestSuck.Suck[i][j].OffDestroy();              //kevin 20130719 關吹氣
                            }
                        }
                    }
                }
                iIndexTakeDeviceChk1=0;                                         //kevin 20190103 回吸檢測狀態
                if(((iOneCycle && IsInArmOneCycleFinish()) ||
                    (iCleanOut && IsInArmCleanOutFinish())) &&
                    FLCarryKit.UseSiteNoIC())                                   //Sam 20230809 : OneCycle 最後一次 IndexArm 動作，需要等待 OutShuttle 動作做完才能做 IndexCheck，避免黏料壓壞 IC
                    Task=550;
                else
                    return true;
            }
            break;
        case 550:
            if(FRCarryKit.UseSiteNoIC() || InSHT1InRT())
            {
                return true;
            }
            break;
        case 600:
            if(INDEX_SUCKER_TYPE==1)                                                                                    //jou 2012-01-04 需確認Index suck已經完整做完
            {
                bArm1D44SuckCheck=false;                                                                                //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                bArm1D44SuckCheck=W64B_FIOSET_PISD1(1);                                                                 //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆 -- TODO(W7) golden :709 fiosetview->ProcessIndexSuckDestroy1 (see file-header gate #8)
            }
            else
            {
                bArm1D44SuckCheck=true;
            }

            if(DoFrontTestDestroyICDelay.Off() && bArm1D44SuckCheck==true)
            {
                flag=false;
                ErrPart=" ";
                bHasErr=false;
                bHasDuplicateErr=false;                                                                                 //Steven 20110127 : Start
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(bArm1PutDuplicateErr[i][j])
                            bHasDuplicateErr=true;
                    }
                }
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(W64B_NEEDCHECK_GET(i, j))                                                                    //確認真空狀態 -- TODO(W7) golden :734 FTestSuck.bNeedCheck[i][j] (see file-header gate #1)
                        {
                            if(FTestSuck.Suck[i][j].GetStatus())
                            {
                                bHasErr=true;
                                ErrPart+=IndexSuckName[i+iNN][j];
                                flag=true;
                                bArm1PutDuplicateErr[i][j]=true;
                                iIndexTakeDeviceChk1=3;                                                                 //kevin 20190103 回吸檢測狀態
                            }
                            else
                            {
                                W64B_NEEDCHECK_SET(i, j, false);                                                        // TODO(W7) golden :746 FTestSuck.bNeedCheck[i][j]=false (see file-header gate #1)
                                FTestSuck.Suck[i][j].Normal();
                                bArm1PutDuplicateErr[i][j]=false;
                            }
                        }

                        if(INDEX_SUCKER_TYPE==1)                                                                        //jou 2011-11-01負壓不能一直開著真空，必須關掉
                        {
                            FTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                bIndexPlaceIcCheck=false;                                                                               //kevin 20110505
                bD44Arm1CheckVacOn=false;                                                                               //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                if(bHasErr)
                {
                    if(MOT[MInArmY].ReadPos()<Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase])                     //kevin 20181107 shuttle 1 在load 不需移走
                    {
                        ShowErrorMessage("JAM0327", K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);                       //Vacuum Sensor Off Error jou 2010-11-23
                    }
                    else if(IndexAlarmInArmAway()==true)                                                                //Steven 20130613 : Index異常時, In Arm要先讓位功能
                    {
                        ShowErrorMessage("JAM0327", K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);                       //Vacuum Sensor Off Error jou 2010-11-23
                    }
                    Task=500;                                                                                           //kevin 20181102
                    return false;
                }
                else
                {
                    iIndexTakeDeviceChk1=0;                                                                             //Steven 20250224 回吸檢測狀態
                }
                bIndexAlarmInArmAway=false;                                                                             //kevin 20181102 (Steven) :  index掉料 inarm 讓位 inarm先不要動
                bHasErr=false;
                if(flag==false)
                {
                    Task=500;
                }
                else
                {
                    if(IniConfig.bD44CheckIndexICDestroy)                                                               //jou 2011-11-01負壓不能一直開著真空，必須關掉
                    {
                        for(int i=0; i<FTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<FTestSuck.iShtCol; j++)
                            {
                                if(W64B_NEEDCHECK_GET(i, j))                                                            // TODO(W7) golden :791 FTestSuck.bNeedCheck[i][j] (see file-header gate #1)
                                {
                                    bIndexPlaceIcCheck=true;                                                            //kevin 20110505
                                    if(INDEX_SUCKER_TYPE==1)                                                            //Steven 20111202
                                    {
                                        fiosetview->bIndexSuck[0][i][j]=true;
                                    }
                                    else
                                    {
                                        FTestSuck.Suck[i][j].On();                                                      //kevin 20110504 check 掉料
                                    }
                                }
                            }
                        }
                    }
                    DoFrontTestDestroyICDelay.SetMSAndOn(IniConfig.iD44TestHeadCheckVacuumTime);
                }
            }
            break;
         case 650:                                                                                                      //kevin 2019819 add onecycle index check 4 Site
            if((ArmSpeed_File[IndexArm].bDevicConfirm ||
               (IniConfig.bD44CheckIndexICDestroy &&
                (CosFunction.bD44Use4ea && GetSiteCount()>4))) &&                                                       //JerryYang 20250120 : modify
               INDEX_SUCKER_TYPE==1)                                                                                    //kevin 20190709 add 20190629 回吸檢測一次 4 個 SITE   //kevin 20190530 add index check
            {
                IndexCheck4Site(true, 0, iSiteCount);
                Task=700;
            }
            break;
        case 700:                                                               //kevin 20190819 add index check 4 Site
            if(IndexCheck4Site(false, 0, iSiteCount))
            {
                iSiteCount++;
                if(iSiteCount<TestSocket.iShtCol/2)                             //JerryYang 20250120 : modify
                {
                    Task=650;
                }
                else
                {
                    Task=750;
                    iSiteCount=0;
//                    CheckSocketHasICDelay1.SetSecAndOn(0.5);     //Steven 20110908 : 上來後也要Delay一下
                }
            }
            break;
        case 750:
//            if(CheckSocketHasICDelay1.Off())
            {
                flag=false;
                if((ArmSpeed_File[IndexArm].bDevicConfirm ||
                   (IniConfig.bD44CheckIndexICDestroy &&
                   (CosFunction.bD44Use4ea && GetSiteCount()>4))) &&            //JerryYang 20250120 : modify
                    TotalErrPart!="")                                           //kevin 20190629 add error show
                {
                   flag=true;
                   ShowErrorMessage("JAM0327", K_RETRY, MTestY1, false, TotalErrPart);
                }
                TotalErrPart="";
                if(flag )
                {
                    Task=650;
                }
                else
                {
                    Task=800;
                }
            }
            break;
       case 800:
            W64B_FRCARRYKIT_SETHASNULLIC();                                    //jou 2011-06-29 : out shuttle has_null_ic hang up fix  //Steven 20160524 : ADD SetHasNullIcToNullIc -- TODO(W7) golden :860 FRCarryKit.SetHasNullIcToNullIc (see file-header gate #9)
            iIndexTakeDeviceChk1=0;                                             //kevin 20190103 回吸檢測狀態
            return true;                                                        //kevin 20190819
    }
    return false;
}
//---------------------------------------------------------------------------
//  WAVE 2 ADDITION (same file, additive) -- DoFrontTestSuckIC + InitFrontTestSuckICTask
//
//  Translation wave: W64b-Translate-Wave2 (front test-head SUCK/pick-from-shuttle SM)
//  Translator: AI(W64b-Translate) 20260710
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/aTester_Front.cpp (cp950)
//    * InitFrontTestSuckICTask()                              golden :870-875
//    * TQPF_Timer dwStartInitialCount1                         golden :876 (file-scope; DoFrontTestSuckIC's initial-soak countdown needs it, declared just above it)
//    * DoFrontTestSuckIC()                                     golden :877-2224 (full switch(Task) SM, verbatim)
//
//  This wave also faithfully translates 5 small same-file leaf dependencies
//  that Wave 1 left untranslated (out of its narrower stated scope) because
//  DoFrontTestSuckIC calls them directly and each is trivial/self-contained
//  (no substrate gap beyond what is already gated below):
//    * ResetInxedArm1Flag()                                    golden :101-113 (file-local; not in .h)
//    * ResetArm1VacFlag()                                      golden :116-124 (file-local; not in .h)
//    * CheckFTFullSite()                                       golden :138-163 (file-local; not in .h)
//    * TQPF_Timer hDoFrontTestSuckIC / hDoFrontTestSuckICdelay  golden :867-868 (file-scope; just above InitFrontTestSuckICTask)
//    * static bool bIndexZ1NeedUp                               golden :66 (file-local static; re-verified against
//      golden line numbers -- EVERY golden read/write of this flag (:958/:1231/
//      :1235/:1240/:1467/:1469) is inside DoFrontTestSuckIC's own body :877-2224)
//  Every OTHER golden aTester_Front.cpp function (DoTestYFront, TestZ1OutRandge2,
//  DoArm1Suck, DoArm1D44VacCheck, FTestSeparateSLK/CombineSLK,
//  DoFRTCAutoModelVerify, DoFrontTestPurgBeforePickShuttle, ...) remains OUT OF
//  SCOPE this wave and stays served by atester_shims.h/.cpp.
//
//  NEWLY-DISCOVERED GATED DEPENDENCIES (continuing the numbered list in the
//  Wave 1 file-header banner above; same convention -- minimal compiling
//  default mirroring golden's park/default behaviour, re-cited at each call
//  site):
//   12. TMyKitSuck::MoveSuckData(TMyKitSuck&,int,int) -- SAME gap as Wave 1's
//       gate #3 (FRCarryKit.MoveSuckData), now also hit on FTestSuck/FLCarryKit
//       (golden :1948/:2139).  Reuses the identical no-op treatment the
//       parallel aTester_Rear.cpp Wave 1 translation already established
//       (W64bT2_MoveSuckData) -- locally duplicated here as W64B_MoveSuckData
//       per this batch's "duplicate seam, integrate agent dedupes" convention.
//   13. TMyKitSuck::SetUnuseToNullIC() -- golden MyKitSuck.cpp; DISTINCT from the
//       already-mirrored SetUnuseAndHasNullICToNullIC/SetAllToNullIC (different
//       method, no substrate member yet).  Offline no-op (golden :113/:2155,
//       i.e. inside both ResetInxedArm1Flag and DoFrontTestSuckIC case 500).
//   14. TMyProductionRecord::AddErrorRecord / AddErrorRecordNoSave on
//       FTestSuck/FLCarryKit's PordRec[][] -- SAME declaration-only-forward-stub
//       gap as Wave 1's gate #11 (Public/MyProductionRecord.h has no .cpp this
//       wave); AddErrorRecordNoSave is not even declared in that header.
//       Offline no-op (pure logging; golden :1729/:1802/:2095).
//   15. fSCKART->iInfo_MultiLotCnt (golden Automation/SCK_ART.h) -- the
//       FormsFacade TfSCKART mirror (FormsFacade.h) has iInputJamCnt/
//       iFTRTCount/iInputCount/CheckLoadingCount/AddOutputJamCnt but not this
//       counter.  Parked whole block with LotSummary (next item) since both are
//       needed together (golden :2165-2178).
//   16. LotSummary (golden cLotSummary.h) -- NO translated home at all yet
//       (same documented gap as csystem.cpp's W7C2_LOTSUMMARY_CLEARRTDATA /
//       cpublic.cpp's gated GetBundleInfo).  Offline: the by-lot load-count
//       tally is simply not kept (golden :2170).
//   17. fAutoTeach (golden AutoTeach.h TfAutoTeach) -- NO FormsFacade stand-in
//       (SAME documented gap as aoutarm9045*.cpp's many fAutoTeach-> gates, see
//       MIGRATION_ROADMAP.md).  SetIndexBinError(0) parked (golden :2180).
//   18. APAX_WriteData(bool,int,int) -- a DIFFERENT DAQ brand than the already-
//       gated ADAM_* EP API (atester_shims.h); no translated home.  Parked
//       (golden :2162); the sibling EPSwitchOnOff(eEPSwArm2) call on the same
//       line stays ACTIVE (already available, atester_shims.h).
//   19. fMain->tInitSoakTimer (golden main.h, a TQPF_Timer-shaped member) -- the
//       FormsFacade TfMain mirror has no such member.  Parked (golden :989);
//       the sibling globals it would arm (iInitialSoakTimer/bInitialSackTime)
//       are still set for real just below/above it (cmydef.h-backed globals).
//   20. fiosetview->ProcessIndexSuckDestroy1() 0-ARG overload (golden
//       iosetview.h) -- DISTINCT call shape from Wave-1's gate #8
//       (ProcessIndexSuckDestroy1(int), used by DoFrontTestDestroyIC).  Same
//       offline contract (report "done"/true); added as its own local shim
//       (W64B_FIOSET_PISD1_V0) rather than overloading the 1-arg macro.
//
//  INTEGRATE-STEP FIX NEEDED (same shape as the Wave-1 note above): atester_shims.h/
//  .cpp TODAY still declare+define offline-stub bodies for the 3 symbols this
//  wave now defines for real -- iFrontTestSuckICTask (atester_shims.h:81,
//  atester_shims.cpp:144 `int iFrontTestSuckICTask=1;`), InitFrontTestSuckICTask
//  (atester_shims.h:94, atester_shims.cpp:157 `{}`), DoFrontTestSuckIC
//  (atester_shims.h:102, atester_shims.cpp:165 `{ return true; }`).  These 3
//  declarations + 3 definitions must be REMOVED by the integrate agent (same
//  multiple-definition-avoidance reason as Wave 1's 6-symbol note) once this
//  file lands.
//---------------------------------------------------------------------------

// -- forward decls golden aTester_Front.cpp top-of-file (outside the Wave 1
//    scope; DoFrontTestSuckIC needs both) --
extern bool CheckCFixTrayFullPlace();                                           //golden :68 (real body acarry.cpp:225)
extern void CheckShuttle1EncoderPos();                                          //golden :69 (real body acarry.cpp:7985) //JerryYang 20230131 : add shuttle shift log

// golden :66 -- file-local static; see the Wave-2 banner note above for the
// re-verified golden line numbers of every read/write.
static bool bIndexZ1NeedUp=false;                                               //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC

TQPF_Timer hDoFrontTestSuckIC;                                                  //golden :867
TQPF_Timer hDoFrontTestSuckICdelay;                                             //golden :868 //JerryYang 20220805 : 下壓shuttle預熱功能initial第一次吸shuttle時需下壓等待Jam soak time

//------------------------------------------------------------------------------
void ResetInxedArm1Flag()                                                       //Steven 20241017 : 打包成function
{
    for(int i=0; i<FTestSuck.iMaxRow; i++)
    {
        for(int j=0; j<FTestSuck.iMaxCol; j++)
        {
            bArm1SuckFinish[i][j]=false;                                        //Steven 20110301 : 初始化，都當作還沒做完
            bArm1DuplicateErr[i][j]=false;
            FTestSuck.Suck[i][j].Reset();                                       //Steven 20140213 : Jordan說Index下去不吸直接Alarm
        }
    }

#if 0 // TODO(W7) -- golden :113 FLCarryKit.SetUnuseToNullIC (TMyKitSuck substrate, no such method yet -- see file-header gate #13)
    FLCarryKit.SetUnuseToNullIC();                                              //Steven 20241017 : 清除沒用到的資料
#endif
}
//------------------------------------------------------------------------------
void ResetArm1VacFlag()                                                         //Steven 20241017 : 打包成function
{
    for(int i=0; i<FTestSuck.iMaxRow; i++)
    {
        for(int j=0; j<FTestSuck.iMaxCol; j++)
        {
            bSkipNeedCheckVac[0][i][j]=false;
        }
    }
}
//------------------------------------------------------------------------------
void CheckFTFullSite()                                                          //wei 20151228 No FullSite delay
{
    int iCount=0, iUseSiteCount=0;
    iArmsitecount[0]=0;
    for(int i=0; i<FTestSuck.iShtRow; i++)
    {
        for(int j=0; j<FTestSuck.iShtCol; j++)
        {
            if(FTestSuck.Item[i][j]==HAS_IC ||
               FTestSuck.Item[i][j]==HAS_HOT_IC)
            {
                iCount++;
            }
            iUseSiteCount++;
        }
    }

    if(iCount!=0)
    {
        iArmsitecount[0]=iUseSiteCount-iCount;
        if(iCount!=iUseSiteCount)
        {
            iInitContactCount=0;                                                //Steven 20160519 : 起測時溫度要補Offset
            bDoWhenNoFullSiteUseInitialDelay=true;
        }
        else
        {
            bDoWhenNoFullSiteUseInitialDelay=false;
        }
    }
}

// ---------------------------------------------------------------------------
//  Wave-2 gate helpers (TODO(W7)) -- see the numbered "NEWLY-DISCOVERED GATED
//  DEPENDENCIES" list in the banner above for the full rationale of each.
// ---------------------------------------------------------------------------
// gate #12 -- golden MyKitSuck.h:268 `void MoveSuckData(TMyKitSuck &Source, int SourceR, int SourceC);`
// this=dst, Source=src (copies src[r][c] into dst[r][c]).  Offline no-op --
// mirrors aTester_Rear.cpp's identical W64bT2_MoveSuckData (locally duplicated
// per this batch's convention; integrate agent may promote to one shared home).
static void W64B_MoveSuckData(TMyKitSuck &dst, TMyKitSuck &src, int /*r*/, int /*c*/)
{
    (void)dst; (void)src;
}
// gate #20 -- golden iosetview.h fiosetview->ProcessIndexSuckDestroy1() 0-arg overload.
static bool W64B_ProcessIndexSuckDestroy1_V0(){ return true; }                  // offline: suck self-check done (true)
#define W64B_FIOSET_PISD1_V0()      W64B_ProcessIndexSuckDestroy1_V0()

// -- fMain torque UI members (golden main.h) -- SAME documented gap as
//    atester.cpp's own W7T1_TfMainTorqueSeam (atester.cpp, "fMain torque /
//    open-bin UI members" block): the FormsFacade TfMain mirror (FormsFacade.h)
//    has no chkReadTorque1/chkReadTorque2/edTorue0 widgets.  Locally duplicated
//    here (TU-local, internal linkage) per this batch's "duplicate seam,
//    integrate agent dedupes" convention -- do NOT invent a different shape.
//    AI(W906-W7-F2) 20260729: W64B_TCheckSeam / W64B_TEditSeam RETIRED -- the members
//    below name vclcompat/Controls.h's unified stand-ins instead (plan D4).  Golden
//    classes re-read from golden main.h for this change: chkReadTorque1/2 :464-465
//    TCheckBox, edTorue0 :466 TEdit.  Zero behaviour change (same single member, same
//    default; the only instances are the members of the one file-scope object below,
//    so no by-value copy or aggregate initialisation can be affected by the vtable).
struct W64B_TfMainTorqueSeam { TCheckBox chkReadTorque1, chkReadTorque2; TEdit edTorue0; };
static W64B_TfMainTorqueSeam W64B_fMainTorque;
#define W64B_FMAIN_CHKREADTORQUE1 (&W64B_fMainTorque.chkReadTorque1)
#define W64B_FMAIN_CHKREADTORQUE2 (&W64B_fMainTorque.chkReadTorque2)
#define W64B_FMAIN_EDTORUE0       (&W64B_fMainTorque.edTorue0)

//------------------------------------------------------------------------------
// AI(integrate) 20260710: the real DEFINITION of iFrontTestSuckICTask was
// missing from this file (only the `extern` decl in aTester_Front.h + the
// in-body assignment below existed) -- the translate unit's own report
// documented removing atester_shims.cpp's `int iFrontTestSuckICTask=1;` stub
// but never added the replacement definition here.  Added now, mirroring
// aTester_Rear.cpp's identical `int iRearTestSuckICTask=1;` convention
// (aTester_Rear.cpp:1110) -- without this the symbol is undefined at link.
int iFrontTestSuckICTask=1;
void InitFrontTestSuckICTask()
{
    iFrontTestSuckICTask=1;
    bZ1Isdownflag=false;
}
//------------------------------------------------------------------------------
TQPF_Timer dwStartInitialCount1;                                                //golden :876
bool DoFrontTestSuckIC()
{
    static int iHomeAlarm=0;
    static bool bOverHappen=false, bHasDropAtShuttle=false, bHasICErr=false;
    static AnsiString ErrPart2="";
    int ZPos=0;
    int sp=0;
    int iNN=IsNNMode();
    int &Task=iFrontTestSuckICTask, ret=0;
    bool bHasDuplicateErr=false;
    bool bHasErr=false;
    bool flag1=false, flag2=false;
    AnsiString ErrPart="", str="";
    switch(Task)
    {
        case 1:                                                                 //判斷Index與Shuttle上IC的狀態
            if(CheckHeaterOK()==false)                                          //Steven 20250116 : 確認HeaterOK
            {
                bHangTimePause=true;
                return false;                                                   //kevin 20161102 break;
            }

            if((iOneCycle && IsInArmOneCycleFinish()) ||
               (iCleanOut && IsInArmCleanOutFinish()))
            {
                if(FTestSuck.UseSiteNoIC() &&
                   FLCarryKit.UseSiteNoIC() &&
                   InArmSuck.HasRealIC()==false)                                //JerryYang 20170801 (wei) 修正按one cycle hang up問題
                {
                    MOT[MInShuttle1].fCanMoveM=true;
                    return true;
                }
            }

            if(CheckCFixTrayFullPlace() &&
               MOT[MTestZ1].Led[iHomeLed]==true)                                //Steven 20220524 : fixed for fix 3 氣缸 Hang up
            {
                MOT[MInShuttle1].fCanMoveM=true;
                return false;
            }

            if(FLCarryKit.UseSiteHasIC())
            {
                if(CanYieldAlarmRemainInSHT()==true)                            //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
                {
                    return true;
                }

                if(InSHT1InRT()==false)
                {
                    if(IndexStatus==Z1Up_Z2Down)
                        MOT[MInShuttle1].fCanMoveM=true;
                    return false;
                }

                if(bReadAndCheckCPUName==true &&                                //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
                   CosFunction.bInShuttleDetectByLatch &&
                   Prod.bF18InshuttleDetect &&
                   MOTION_CARD_TYPE==MotionCard_Contec &&
                   LastSet.iRealDummy==REALLY)                                  //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
                {
                    if(bInSht1LtcDetectTesterCanMove==false)
                    {
                        return false;
                    }
                }

                if((Prod.bF18InshuttleDetect ||                                 //Sam 20250925 : 修正關 Arm 後檢查 F18 功能被卡死 hangeup
                    IniConfig.bF20InShuttleProminentDetect) &&
                    CosFunction.bInShuttleDetectByLatch==false)
                {
                    if(bF18CheckShuttle1MustHasIC)
                        break;
                }

                if(In_Shuttle_Auto_Latch==eInSHAutoLtc &&
                   bInSh1DoLtc==true)                                           //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
                    return false;
                MOT[MInShuttle1].fCanMoveM=false;
                bSuckingFlagZ1=true;
                Task=300;
                bIndexZ1NeedUp=false;                                           //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
            }
            break;
        case 299:                                                               //Steven 20140827 : 第一次吸取ic等待時間
            if(DoTestHeadMotorDelay2.Off())
            {
                flag2=false;
                Task=301;
            }
            break;
        case 300:
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&                   //ChungHung 20120717 add Index Drop Error Can Retry and Start
               bShuttle1MoveToRight)                                            //ChungHung 20131015 fix hangup
            {
                if(InSHT1InRT()!=true)
                {
                    return false;
                }
            }
            bIndexPickUpErrMoveSht1=false;                                      //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
            MOT[MInShuttle1].fCanMoveM=false;
            bShuttle1MoveToRight=false;
            bShuttle1HasPickErr=false;                                          //Steven 20220712 : 避免In arm 偷放料
            if(DeviceForm.bSuckShuttleDeviceAfterTested &&                      //Steven 20140827 : 第一次吸取ic等待時間
               (LastSet.iTemperature==Tempture_Hot ||                           //2013-11-27   Dell    需要做Index soak time
                LastSet.iTemperature==Tempture_AmbientHot) &&                   //JerryYang 20181214 : non hotplate mode也要支援溫度優先功能
               bInitialSackTime &&
               Temperature.iInitialStart1Time>0)                                //kevin 20131112 第一次吸取ic等待時間)
            {
                DoTestHeadMotorDelay2.SetSecAndOn(Temperature.iInitialStart1Time);
                iInitialSoakTimer = Temperature.iInitialStart1Time;
#if 0 // TODO(W7) -- golden :989 fMain->tInitSoakTimer (FormsFacade TfMain has no such member -- see file-header gate #19)
                fMain->tInitSoakTimer.LatchCycleTimeSec(true);
#endif
                bInitialSackTime=false;
                Task=299;
                break;
            }

            if(CosFunction.bPurgeBeforePickShuttle)                             //JerryYang 20160906 PurgeBeforePickShuttle改成可以By客戶功能開啟
                InitFrontTestPurgBeforePickShuttle();                           //ChungHung 20150517 add for ATK Try to Fix TSMC Device Die Crack Issue Start

            flag2=false;
            Task=301;
            if(IniConfig.bIndexPickupWait==true &&
               (LastSet.iTemperature==Tempture_Hot ||
                LastSet.iTemperature==Tempture_AmbientHot) &&
               Temperature.iInitialStart2Time!=0)                               //kevin 20180905 (Steven) : add  iInitialStart2Time
            {
                bInitialStart2Time=true;                                        //wei 20171020 (jou) InitialStart1 秒數倒數
            }

            if(iNN==NN_2Row)
                break;
        case 301:                                                                                                                                               //確認Z軸位置 位置在下->吸取IC->Z軸到位後->Task=310
            if(IniConfig.bIndexPickupWait==true &&                                                                                                              //jou 2012-06-29 Index Pick up need wait Soak Time
               (LastSet.iTemperature==Tempture_Hot ||
                LastSet.iTemperature==Tempture_AmbientHot) &&
               Temperature.iInitialStart2Time!=0)                                                                                                               //kevin 20180905 (Steven) : add  iInitialStart2Time
            {
                dwEndShuttle1Soak=MyTickCount();                                                                                                                //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常  //kevin 20180905 add  iInitialStart2Time
                if(iInitialStart2Count>0)
                {
                    iInitialStart2Count=(Temperature.iInitialStart2Time)-((dwEndShuttle1Soak-dwStartShuttle1Soak)/1000);                                        //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)                                                                                            //Steven 20161213 : Fix iInitialStart2 關ARM Hangup
                {
                }
                else
                {
                    if(FLCarryKit.TSoakTime.Off()==false)
                        break;
                }
                bInitialStart2Time=false;                                                                                                                       //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
            }

            if(((IniConfig.bA05UseAutoDocking==true &&                                                                                                          //jou 2014-04-25 Tester Docking Wait delay time
                 Temperature.bUseTesterDocking==true) &&                                                                                                        //ChungHung 20141111 modify for SCK request
               LastSet.iTemperature==Tempture_Hot) && iTesterDucking>0)
            {
                bHangTimePause=true;                                                                                                                            //ChungHung 20140714 add fix Tester Ducking function if wait long time will Auto Home then Show Hangup Message
                break;
            }

            if(bNeedTesterDuckingFinishLightYellowAndAlarmOn)                                                                                                   //ChungHung 20141015 add for SCK When the handler start running (finished Start count function),Yellow light blinking & alarm on (press "alarm reset" then clear)
            {
                bNeedTesterDuckingFinishLightYellowAndAlarmOn=false;
                bTesterDuckingFinishLightYellowAndAlarmOn=true;
            }

            if(TestIF_File.bIndexPickICWhenOutShtNoIC)                                                                                                          //Steven 20221207 : Index必須在out shuttle沒料才可以吸
            {
                if(FRCarryKit.HasRealIC())
                {
                    return false;
                }
            }

            if(TestIF.iShuttleMode==1 &&                                                                                                                        //jou 980317 SOFT_SIMULTE hang
               TestIF.iShuttle_Sel!=1 &&
               bCheckShuttle1Flag)
                return false;

            MOT[MInShuttle1].ScanMotorStatus();
            if(FLCarryKit.UseSiteHasIC() && bCheckShuttle1Flag==false)
            {
                flag2=false;

                if(CheckZ2IsDown())
                {
                    if(FLCarryKit.HasRealIC()==false)
                    {
                        flag2=true;
                    }
                    else
                    {
                        ZPos=MOT[MTestZ2].Gali_ReadPos();
                        //==> Eastsun 20260511 F006 整合: Ifor 20240430 add:secs gem cmd Index Yiel Fail
                        if(CUSTOMER_CODE==CC_KYEC_LEE && iSECSGEM_ConsecutiveFailureAlarm==2)   //Ifor 20240430 add:secs gem cmd Index Yiel Fail 0:Normal 1:收到命令 2:Index 上升
                        {
                        }
                        //<== Eastsun 20260511 F006 整合
                        else if(CosFunction.bSortingBy2DList==true &&
                           LastSet.iTester==_2D_SORT &&
                           TestIF_File.bSortingBy2DIDList==true)                                                                                                //Frank 20221122 : 2DID sorting for ATK
                        {
                        }
                        else
                        {
                            if(bUseTwoArm32Site==false)
                            {
                                if(ZPos>=(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+500))                                                                        //2008/06/24
                                {
                                    if(ZPos!=iBackUpZ2DownPosition)
                                        return false;
                                }
                            }
                        }

                        if(MOT[MTestZ1].Gali_ReadPos()<(Prod.TestZ1_Pick+1000))                                                                                 //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                        {
                            bZ1PickShuttle=true;
                        }

                        if(flag2==false)
                        {
                            bSuckingFlagZ1=true;
                            sp=GetIndexZSpeed(0);                                                                                                               //Steven 20160524 : Index Z軸速度整合為Function

                            if(CosFunction.bPurgeBeforePickShuttle)                                                                                             //JerryYang 20160906 PurgeBeforePickShuttle改成可以By客戶功能開啟
                            {
                                if(DeviceForm_File.bPurgeBeforePickShuttle)                                                                                     //ChungHung 20150517 add for ATK Try to Fix TSMC Device Die Crack Issue
                                {
                                    if(DoFrontTestPurgBeforePickShuttle(sp)==false)
                                        break;
                                }
                            }
                            #ifdef DEBUG_INDEX_UPH
                            flag2=MOT[MTestZ1].Gali_MotMove2(Prod.TestZ1_Pick, iIndexSpeed, iIndexAcc);                                                         //Steven 20170818 : 測試UPH用
                            #else
                            flag2=MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Pick, sp);                                                                              //in shuttle 吸取位置 20130615
                            #endif
                        }
                    }
                }
            }
            else if(CosFunction.bIndexPickErrSkipNeedCheckVac &&                                                                                                //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
                    IniConfig.bD50IndexPickErrSkipNeedCheckVac &&
                    bArm1PressSkipNeedDownCheckVac==true &&
                    FLCarryKit.UseSiteNoIC())
            {
                flag2=false;

                if(CheckZ2IsDown())
                {
                    ZPos=MOT[MTestZ2].Gali_ReadPos();
                    //==> Eastsun 20260511 F006 整合: Ifor 20240430 add:secs gem cmd Index Yiel Fail (KYEC SECS=2 跳過下降位置檢查)
                    if(CUSTOMER_CODE==CC_KYEC_LEE && iSECSGEM_ConsecutiveFailureAlarm==2)   //Ifor 20240430 add:secs gem cmd Index Yiel Fail 0:Normal 1:收到命令 2:Index 上升
                    {
                    }
                    else
                    //<== Eastsun 20260511 F006 整合
                    if(iNN!=NN_2Row)                                                                                                                            //jou Index更換Offset會hang up? 2010-04-13
                    {
                        if(ZPos>=(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+500))                                                                                //2008/06/24
                        {
                            if(ZPos!=iBackUpZ2DownPosition)
                                return false;
                        }
                    }

                    if(MOT[MTestZ1].Gali_ReadPos()<(Prod.TestZ1_Pick+1000))                                                                                     //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                    {
                        bZ1PickShuttle=true;
                    }

                    if(flag2==false)
                    {
                        if(CUSTOMER_CODE==CC_AMKOR_Philippines)                                                                                                 //Frank QQ
                        {
                            flag2=true;
                        }
                        else
                        {
                            bSuckingFlagZ1=true;
                            #ifdef DEBUG_INDEX_UPH
                            flag2=MOT[MTestZ1].Gali_MotMove2(Prod.TestZ1_Pick, iIndexSpeed, iIndexAcc);                                                         //Steven 20170818 : 測試UPH用
                            #else
                            flag2=MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Pick, iSpeedSlow);                                                                      //JerryYang 20170610 (wei) 這裡改慢速
                            #endif
                        }
                    }
                }
            }
            else
            {
                Task=1;
                break;
            }

            if(LastSet.iRealDummy==REALLY &&
               ArmSpeed[IndexArm].bSuckOnDown &&
               INDEX_SUCKER_TYPE==0)
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FLCarryKit.Item[i][j]!=NULL_IC &&
                           FLCarryKit.Item[i][j]!=HAS_NULL_IC)
                        {
                            FTestSuck.Suck[i][j].On();
                        }
                    }
                }
            }

            if(flag2)
            {
                RecordIndexPosition(1, 0);                                                                                                                      //Isaac 20200922 : 紀錄indexArmY encoder值和command值，Arm1/Shuttle
                EncoderTeachingMaxMinCount(1);                                                                                                                  //Isaac 20201012 : 每次完成動作，比較紀錄Encoder和Teaching點的差值
                flag2=false;                                                                                                                                    //JerryYang 20170610 (wei) 移到上面
                bZ1PickShuttle=true;                                                                                                                            //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing

                if(CosFunction.bIndexPickErrSkipNeedCheckVac &&
                   IniConfig.bD50IndexPickErrSkipNeedCheckVac &&
                   bArm1PressSkipNeedDownCheckVac==true &&
                   FLCarryKit.UseSiteNoIC())                                                                                                                    //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(bSkipNeedCheckVac[0][i][j]==true)
                            {
                                bArm1SuckFinish[i][j]=false;                                                                                                    //Steven 20110301 : 初始化，都當作還沒做完
                                bArm1DuplicateErr[i][j]=false;
                                FTestSuck.Suck[i][j].Reset();                                                                                                   //Steven 20140213 : Jordan說Index下去不吸直接Alarm
                            }
                        }
                    }
                    Task=307;                                                                                                                                   //JerryYang 20170610 (wei) 這裡跳去檢查按skip的真空
                    break;
                }

                ResetInxedArm1Flag();                                                                                                                           //Steven 20241017 : 打包成function

                if(CosFunction.bUseShuttlePickShiftDetect==true &&
                   DeviceForm_File.dDropByPassDetect!=0)                                                                                                        //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
                {
                    if(TestIF_File.iShuttleMode==0 ||                                                                                                           //Ifor 20221013 add:關Arm不處理避免資料轉移異常
                       (TestIF_File.iShuttleMode==1 &&
                        TestIF_File.iShuttle_Sel==0))
                    {
                        bIndexZ1NeedUp=true;
                    }
                    else
                    {
                        bIndexZ1NeedUp=false;
                    }
                }
                else
                {
                    bIndexZ1NeedUp=false;
                }

                if(USE_IO_CHANGE_TOQUE==true &&                                                                                                                 //jou 2012-06-12 吸shuttle時，需檢測Torque，過大需alarm
                   (bNeedCheckIndexToque==true ||
                   bNeedCheckIndexToque1==true))                                                                                                                //jou 2012-06-12 即時更新扭力值不能開，不然會衝突
                {
                    bNeedCheckIndexToque=false;
                    bNeedCheckIndexToque1=false;
                    W64B_FMAIN_CHKREADTORQUE1->Checked=true;                                                                                                    //golden fMain->chkReadTorque1->Checked (locally-duplicated W7T1-style torque seam, see gate helpers above)
                    W64B_FMAIN_CHKREADTORQUE2->Checked=false;
                    W64B_FMAIN_EDTORUE0->Text="";
                    bOverHappen=false;
                    hDoFrontTestSuckIC.SetSecAndOn(1);
                }

                bHasDropAtShuttle=false;
                if(IniConfig.bD81IndexCheckVacuumOnShuttle)
                {
                    for(int i=0; i<MAX_Index_Row; i++)                                                                                                          //JerryYang 20241002 : add
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(FLCarryKit.Item[i][j]==NULL_IC || FLCarryKit.Item[i][j]==HAS_NULL_IC)
                            {
                                if(INDEX_SUCKER_TYPE==1 && bDropAtSht1NeedCheckVac[i][j]==true)                                                                 //Steven 20111202
                                {
                                    fiosetview->bIndexSuck[0][i][j]=true;
                                    bHasDropAtShuttle=true;
                                }
                            }
                        }
                    }
                }
                Task=310;
            }
            break;
        case 307:                                                               //JerryYang 20170610 (wei) JSCC要求index arm pick up error按SKIP要慢速下降再吸一次
            bArm1SuckComplete=true;                                             //Steven 20210818 : Index吸料中判斷
            flag1=true;
            ErrPart="";
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(bSkipNeedCheckVac[0][i][j] &&
                       FLCarryKit.Item[i][j]==NULL_IC)
                    {
                        if(FLCarryKit.Item[i][j]==NULL_IC &&
                           bArm1SuckFinish[i][j]==true)                         //Steven 20110301
                        {
                            if(FTestSuck.Item[i][j]==HAS_NULL_IC ||
                               FTestSuck.Item[i][j]==NULL_IC)
                                FTestSuck.Suck[i][j].Normal();                  //Steven 20111201 : 預防負壓壓降
                        }
                        else
                        {
                            if(FTestSuck.Item[i][j]==HAS_NULL_IC &&
                               FTestSuck.Suck[i][j].Suck())
                            {
                                bHasErr=true;                                   //按skip後卻吸到IC，要跳alarm
                                bArm1DuplicateErr[i][j]=true;
                                bArm1SuckFinish[i][j]=true;
                            }
                            else if(FTestSuck.Suck[i][j].Error)                 //Steven 20110301 : 有錯誤的不做
                            {
                                bArm1SuckFinish[i][j]=true;
                            }
                            else if(FTestSuck.Item[i][j]==HAS_IC &&
                                    FTestSuck.Suck[i][j].Suck())                //Ifor 20171122 (Steven) :add 修正bArmSuckFinish 資料異常造成Hangup
                            {
                                bArm1SuckFinish[i][j]=true;
                            }
                            else
                            {
                                bArm1SuckComplete=false;                        //jou 2011-08-16 只要有未完成的就繼續等
                            }
                        }
                    }
                    else
                    {
                        if(FTestSuck.Item[i][j]==HAS_NULL_IC ||
                           FTestSuck.Item[i][j]==NULL_IC)
                        {
                            FTestSuck.Suck[i][j].Normal();                      //Steven 20111201 : 預防負壓壓降
                        }
                        bArm1SuckFinish[i][j]=true;                             //Steven 20110301 : 沒有東西的地方要跳過
                    }
                }
            }

            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(bArm1SuckFinish[i][j]==false)                            //只要有未完成的就繼續等
                        bArm1SuckComplete=false;
                }
            }

            if(bArm1SuckComplete==true)                                         //Steven 20110301 : 所有吸嘴都做完
            {
                bArm1SuckComplete=false;                                        //Steven 20210818 : Index吸料中判斷
                if(bHasErr)
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(bSkipNeedCheckVac[0][i][j]==true &&
                               bArm1DuplicateErr[i][j]==true)
                            {
                                FTestSuck.Suck[i][j].Normal();
                            }
                        }
                    }
                    Task=308;
                    return false;
                }

                if(FLCarryKit.HasRealIC())
                    break;

                ZeroMemory(bArm1DuplicateErr, sizeof(bArm1DuplicateErr));

                if(FLCarryKit.UseSiteHasIC())
                    break;

                iD43AutoRetryWhenIndexPickErrCnt[0]=0;
                bResetIndexArm1Pick=false;

                bArm1PressSkipNeedDownCheckVac=false;
                ResetArm1VacFlag();                                             //Steven 20241017 : 打包成function
                Task=500;
            }
            break;
        case 308:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Safe, iSpeedSlow))         //JerryYang 20180628 (wei) 只有pick up error的arm要上升
            {
                bSuckingFlagZ1=false;
                Task=309;
            }
            break;
        case 309:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                bArm1PressSkipNeedDownCheckVac=false;
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(bSkipNeedCheckVac[0][i][j]==true)
                        {
                            ErrPart+=IndexSuckName[i+iNN][j];
                        }

                        if(CUSTOMER_CODE!=CC_AMKOR_Philippines)
                            bSkipNeedCheckVac[0][i][j]=false;
                    }
                }

                if(CUSTOMER_CODE==CC_AMKOR_Philippines)                         //Frank QQ
                {
                    bArm1PressSkipNeedDownCheckVac=true;
                    str.sprintf("Arm1 detect %s has device", ErrPart);
                    ShowMyMessage(str,"請檢查Arm上是否有IC");
                    ErrPart="";
                    Task=3091;
                }
                else
                {
                    str.sprintf("Arm1 detect Shuttle %s has device", ErrPart);
                    ShowMyMessage(str, "請檢查shutle1上是否有IC");
                    ErrPart="";
                    Task=500;
                }
            }
            break;
        case 3091:
            ResetInxedArm1Flag();                                               //Steven 20241017 : 打包成function
            Task=307;
            break;
        case 3099:                                                              //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
            bSuckingFlagZ1=true;
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Pick+DeviceForm_File.dDropByPassDetect*100, iSpeedSlow))
            {
                ResetInxedArm1Flag();                                           //Steven 20241017 : 打包成function
                Task=310;
            }
            break;
        case 310:
            if(USE_IO_CHANGE_TOQUE==true &&                                                                             //jou 2012-06-12 吸shuttle時，需檢測Torque，過大需alarm
               bOverHappen==false)                                                                                      //jou 2012-06-12 即時更新扭力值不能開，不然會衝突
            {
                if(W64B_FMAIN_EDTORUE0->Text!="")
                {
                    if(atoi(W64B_FMAIN_EDTORUE0->Text.c_str())>=ciIndex5FSetValue)
                    {
                        ShowMyMessage("Index Z1 Torque Over,Z1 Pick from Shuttle need up","Index Z1 Torque 過大,需往上調整");
                    }
                    bOverHappen=true;
                }
                else if(hDoFrontTestSuckIC.Off()==false)
                {
                    break;                                                                                              //Wait Read Torque
                }
                else
                {
                    bOverHappen=true;                                                                                   //Over Wait Time
                }
            }
            bArm1NeedSuck=true;                                                                                         //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
            bArm1SuckComplete=true;

            DoArm1Suck();                                                                                               //JerryYang 20190123 把index arm吸真空&交換狀態包成函式

            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(bArm1SuckFinish[i][j]==false)                                                                    //只要有未完成的就繼續等
                        bArm1SuckComplete=false;
                }
            }

            if(bArm1SuckComplete==true)                                                                                 //Steven 20110301 : 所有吸嘴都做完
            {
                if(bIndexZ1NeedUp)                                                                                      //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
                {
                    bIndexZ1NeedUp=false;
                    Task=3099;
                    break;
                }

                FTestSuck.HotCount=FLCarryKit.HotCount;                                                                 //JerryYang 20230204 : add hot plate放置順序資料
                bArm1NeedSuck=false;

                if(IniConfig.bD81IndexCheckVacuumOnShuttle && bHasDropAtShuttle==true)                                  //JerryYang 20241002 : add
                {
                    if(INDEX_SUCKER_TYPE==1)
                    {
                        if(W64B_FIOSET_PISD1_V0()==true)                                                                //golden fiosetview->ProcessIndexSuckDestroy1() 0-arg overload -- TODO(W7) see file-header gate #20
                        {
                            bHasICErr=false;
                            ErrPart2="";
                            for(int i=0; i<MAX_Index_Row; i++)
                            {
                                for(int j=0; j<NEW_MAX_Index_Col; j++)
                                {
                                    #ifdef SOFT_SIMULTE
                                    if((FTestSuck.Item[i][j]==NULL_IC || FTestSuck.Item[i][j]==HAS_NULL_IC) && i==0 && j==3)
                                    #else
                                    if((FTestSuck.Item[i][j]==NULL_IC || FTestSuck.Item[i][j]==HAS_NULL_IC) && FTestSuck.Suck[i][j].GetStatus()==true)
                                    #endif
                                    {
                                        bHasICErr=true;

                                        bSHT1_DropPosHasIC[i][j]=true;

                                        if(TestIF_File.iTestMode==_32Site4X8M ||
                                           TestIF_File.iTestMode==_32Site4X8N ||
                                           TestIF_File.iTestMode==_16Site4X4)                                           //Sam 20190226 : 16Site4X4 ///kevin 20180504 add  error pos
                                            ErrPart2+=IndexSuckName[i+2][j];
                                        else if(TestIF_File.iTestMode==QualSite2X2N ||
                                                TestIF_File.iTestMode==_6Site2X3N)
                                            ErrPart2+=IndexSuckName[i+1][j];                                            //Steven 20230712 : 修正NN mode alarm顯示
                                        else
                                            ErrPart2+=IndexSuckName[i][j];

                                        FTestSuck.Suck[i][j].Normal();
                                    }
                                }
                            }

                            if(bHasICErr==false)
                            {
                                for(int i=0; i<MAX_Index_Row; i++)
                                {
                                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                                    {
                                        bDropAtSht1NeedCheckVac[i][j]=false;
                                        bSHT1_DropPosHasIC[i][j]=false;
                                    }
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                FTestSuck.HotCount=FLCarryKit.HotCount;                                                                 //JerryYang 20230204 : add hot plate放置順序資料
                bHasErr=false;
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Suck[i][j].Error)
                        {
                            bHasErr=true;
                            if(IniConfig.bD62PickUpErrorNeedPurge)                                                      //Steveb 20161024 : 吸取異常需要吹氣一次
                            {
                                FTestSuck.Suck[i][j].Off();
                            }
                        }
                    }
                }

                if(bHasErr || (IniConfig.bD81IndexCheckVacuumOnShuttle && bHasICErr))
                {
                    if(USE_IO_CHANGE_TOQUE==true)                                                                       //jou 2012-06-12 即時更新扭力值不能開，不然會衝突      //Steven 20230307 : 往下移動
                    {
                        W64B_FMAIN_CHKREADTORQUE1->Checked=true;
                        W64B_FMAIN_CHKREADTORQUE2->Checked=false;
                        W64B_FMAIN_EDTORUE0->Text="";
                        bOverHappen=false;
                        hDoFrontTestSuckIC.SetSecAndOn(1);
                    }

                    bShuttle1HasPickErr=true;                                                                           //Steven 20230118 : 避免In arm 偷放料, 往上移動
                    bIndexPickUpErrMoveSht1=true;                                                                       //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
                    if(CosFunction.bIndexPickErrSkipBlowAirTime)                                                        //Frank 20171213 (Steven) : Index Pick Err In Shuttle Skip and Blow Air
                    {
                        Task=3051;
                        return false;
                    }
                    else
                    {
                        Task=305;
                        return false;
                    }
                }

                if(FLCarryKit.HasRealIC())
                    break;

                if(TestIF_File.iShuttleMode==0 ||
                   (TestIF_File.iShuttleMode==1 &&
                    TestIF_File.iShuttle_Sel==0))                                                                       //Jou 20230224 : 修正Auto Site Map關arm異常
                {
                    if(bAutoSiteMapWaitTestResult==true &&
                       BTestSuck.HasRealIC()==false &&
                       FTestSuck.HasRealIC()==false &&                                                                  //Steven 20200326 : 修正JCET Auto site map發生inarm掉料會hang up
                       TestSocket.HasRealIC()==false &&
                       FLCarryKit.HasRealIC()==false &&
                       BLCarryKit.HasRealIC()==false)
                    {
                        if(iNN==NN_2Row)                                                                                //Steven 20220531 : for NN mode Auto Site Map
                        {
                        }
                        else
                        {
                            bAutoSiteMapWaitTestResult=false;
                        }
                    }
                }

                ZeroMemory(bArm1DuplicateErr, sizeof(bArm1DuplicateErr));
                if(FLCarryKit.UseSiteHasIC())
                    break;

                bResetIndexArm1Pick=false;
                if(CUSTOMER_CODE==CC_TSMC_TAINAN &&
                   (Prod.bWhenNoFullSiteUseInitialDelay &&                                                              //wei 20161102 No FullSite delay修改||->&&
                    IniConfig.bL18NofullsiteaddTemperatureoffset))                                                      //wei 20151228 No FullSite delay
                    CheckFTFullSite();
                Task=500;
                if(DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==true &&
                   LastSet.iTemperature==Tempture_Hot &&                                                                //JerryYang 20220805 : 下壓shuttle預熱功能initial第一次吸shuttle時需下壓等待Jam soak time
                   BTestSuck.HasRealIC()==false)
                {
                    hDoFrontTestSuckICdelay.SetSecAndOn(Temperature.fJamSoakTime);
                }
                iD43AutoRetryWhenIndexPickErrCnt[0]=0;                                                                  //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
            }
            break;
        case 3051:                                                              //Frank 20171213 (Steven) : Index Pick Err In Shuttle Skip and Blow Air
            bSuckingFlagZ1=true;
            if(MOT[MTestZ1].Gali_MotMoveNoWait(Prod.TestZ1_Place, MOT[MTestZ1].GailSpeed, 0))
            {
                hDoFrontTestSuckIC.SetSecAndOn(IniConfig.iD62IndexBlowAirTime);
                Task=3052;
            }
            break;
        case 3052:                                                              //Frank 20171213 (Steven) : Index Pick Err In Shuttle Skip and Blow Air
            if(hDoFrontTestSuckIC.Off())
            {
                Task=305;
            }
            break;
        case 305:
            if(USE_IO_CHANGE_TOQUE==true &&                                     //jou 2012-06-12 吸shuttle時，需檢測Torque，過大需alarm
               bOverHappen==false)                                              //jou 2012-06-12 即時更新扭力值不能開，不然會衝突
            {
                if(W64B_FMAIN_EDTORUE0->Text!="")
                {
                    if(atoi(W64B_FMAIN_EDTORUE0->Text.c_str())>=ciIndex5FSetValue)
                    {
                        ShowMyMessage("Index Z1 Torque Over, Z1 Pick from Shuttle need up", "Index Z1 Torque 過大,需往上調整");
                    }
                    bOverHappen=true;
                }
                else if(hDoFrontTestSuckIC.Off()==false)
                {
                    break;                                                      //Wait Read Torque
                }
                else
                {
                    bOverHappen=true;                                           //Over Wait Time
                }
            }

            CheckIndexAllSuckICFallDown(true, false);                           //Steven 20110725 : 修改負壓檢查方式

            if(MOT[MTestZ1].Gali_ReadPos()>(Prod.TestZ1_Pick+1000))             //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            {
                bZ1PickShuttle=false;
            }

            bSuckingFlagZ1=true;
            #ifdef DEBUG_INDEX_UPH
            if(MOT[MTestZ1].Gali_MotMove2(Prod.TestZ1_Safe, iIndexSpeed, iIndexAcc))
            #else
            if(MOT[MTestZ1].Gali_MotMoveNoWait(Prod.TestZ1_Safe, MOT[MTestZ1].GailSpeed, 0))
            #endif
            {
                bSuckingFlagZ1=false;
                Task=306;                                                       //Steven 20160718 : Index pick up error with [D43]
            }
            break;
        case 306:
            bZ1PickShuttle=false;                                               //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip)
            {
                if(FRCarryKit.UseSiteNoIC())                                    //ChungHung 20120717 add Index Drop Error Can Retry and Start
                {
                    MOT[MInShuttle1].fCanMoveM=true;
                    bShuttle1MoveToLeft=true;
                    bShuttle1HasPickErr=true;                                   //Steven 20220712 : 避免In arm 偷放料
                    bIndexPickErrShtStayRight1=false;                           //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
                    bCheckNullIC1=false;                                        //JerryYang 20170623 (wei) 修正有裝out shuttle 前後對照的機台發生index arm吸取異常無法跳出alram造成hang up
                    Task=320;
                }
            }
            else
            {
                if(bPlaceToShuttle2Step)                                        //Steven 20160718 : 避免放蝦頭放到一半讓位會死雞
                    return false;
                Task=320;
            }
            break;
        case 320:                                                               //等Shuttle移出來
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&                   //ChungHung 20120717 add Index Drop Error Can Retry and Start
               bShuttle1MoveToLeft)                                             //ChungHung 20131015 fix hangup
            {
                if(InSHT1InLF()!=true)
                {
                    return false;
                }
            }
            bShuttle1MoveToLeft=false;
            MOT[MInShuttle1].fCanMoveM=false;
            Task=321;

            if(iNN==NN_2Row)
                break;
        case 321:                                                                                                                                               //ChungHung 20130924 add
            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            bSht1OnlyHasICErr=false;
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(bArm1DuplicateErr[i][j])
                        bHasDuplicateErr=true;

                    if(FTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;                                                                                                                           //Steven 20240927 : fixed for index arm 1 pickup error
                        ErrPart+=IndexSuckName[i+iNN][j];

                        if(IniConfig.bD62PickUpErrorNeedPurge)                                                                                                  //Steveb 20161024 : 吸取異常需要吹氣一次
                        {
                            FTestSuck.Suck[i][j].Normal();
                        }
#if 0 // TODO(W7) -- golden :1729 FLCarryKit.PordRec[i][j].AddErrorRecordNoSave (TMyProductionRecord declaration-only forward-stub, no .cpp yet -- see file-header gate #14)
                        FLCarryKit.PordRec[i][j].AddErrorRecordNoSave("JAM0301");
#endif
                    }
                    else
                    {
                        FTestSuck.Suck[i][j].Error=false;
                    }
                }
            }

            if(bHasErr)
            {
                if(IndexAlarmInArmAway()==false)                                                                                                                //Steven 20130613 : Index異常時, In Arm要先讓位功能
                {
                    return false;
                }

                bHasErr=false;

                if(IniConfig.bD81IndexCheckVacuumOnShuttle && bHasICErr)
                {
                    bHasICErr=false;
                    str.sprintf("Need to remove device of Shuttle1 %s", ErrPart2);
                    ShowMyMessage(str);
                }

                if(IniConfig.bD43AutoRetryWhenIndexPickErr &&                                                                                                   //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
                   iD43AutoRetryWhenIndexPickErrCnt[0]==0)
                {
                    ret=K_RETRY;
                }
                else if(IniConfig.bNewResetFunction==true &&
                        bResetIndexArm1Pick==true)
                {
                    ret=K_SKIP;
                }
                else
                {
                    if(CosFunction.bJAM0301NeedOpenChamberDoor)                                                                                                 //wei : JAM0301 & JAM0302需要開啟Chamber門10秒
                    {
                        bIsTestSitICFallDown=true;
                    }

                    if(IniConfig.bIndexPickErrOnlySKIP==true ||                                                                                                 //jou 2012-02-13 index pick-up error only skip
                       IniConfig.bD64IndexPickErrOnlySKIP)                                                                                                      //kevin 20171103 (wei) add retry function
                        ret=ShowErrorMessage("JAM0301", K_SKIP, MTestZ1, bHasDuplicateErr, ErrPart);                                                            //Devicr Pick-Up Error
                    else
                        ret=ShowErrorMessage("JAM0301", K_SKIP|K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);                                                    //Devicr Pick-Up Error
                }

                if(ret==K_SKIP)
                {
                    if(LastSet.iRunStartMode==rsmAutoSiteMap)
                        iAutoSiteMapCount++;                                                                                                                    //Steven 20220811 : 紀錄目前是哪個Shuttle

                    bAutoSiteMapWaitTestResult=false;                                                                                                           //Ifor 20180115 (Steven) : add Site Mapping SKIP 需清除旗標
                    bIndexPickUpErrMoveSht1=false;                                                                                                              //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
                    iD43AutoRetryWhenIndexPickErrCnt[0]=0;                                                                                                      //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(FTestSuck.Item[i][j]==NULL_IC)                                                                                                   //jou 2011-12-27 有發生過Skip又重吸一次,改成下面的方式
                            {
                                if(FTestSuck.Suck[i][j].Error)                                                                                                  //JerryYang 20200303 fix 沒IC的地方jam count被++
                                {
                                    if(CosFunction.bUseSCKART)                                                                                                  //Steven 20161214 (wei) : For SCK ART
                                    {
                                        if(TestIF_File.bRENESAS_EnableFTCT==true)                                                                               //RogeryYang 20251014 : FTCT add to HdRejectBT
                                            fSCKART->iInputJamCnt++;
                                        else
                                            fSCKART->AddOutputJamCnt(i, j, ret);                                                                                //RogerYang 20250923 : 整合ART OutArm JamCount
                                    }
                                }
#if 0 // TODO(W7) -- golden :1802 FLCarryKit.PordRec[i][j].AddErrorRecord (TMyProductionRecord declaration-only forward-stub, no .cpp yet -- see file-header gate #14)
                                FLCarryKit.PordRec[i][j].AddErrorRecord("JAM0301");                                                                             //Steven 20161214 : 加上Index異常Skip的ErrorLog
#endif
                                FTestSuck.SetItemData(i, j, HAS_NULL_IC);
                                FTestSuck.Suck[i][j].Normal();
                                if(CosFunction.bIndexPickErrSkipNeedCheckVac==true && IniConfig.bD50IndexPickErrSkipNeedCheckVac)                               //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
                                {
                                    bSkipNeedCheckVac[0][i][j]=true;
                                    bArm1PressSkipNeedDownCheckVac=true;
                                }
                            }
                            FLCarryKit.SetItemData(i, j, NULL_IC);

                            bArm1DuplicateErr[i][j]=false;
                        }
                    }

                    if(IniConfig.bNewResetFunction==true && bResetIndexArm1Pick==true)
                    {
                    }
                    else
                    {
                        if(IniConfig.bD42IndexPickICShuttlePause)
                        {
                            bInArmNeedToSafePos=true;
                            bShuttle1Pause=true;
                            bIndexArm1PickupErrStop=true;                                                                                                       //jou 2012-02-29 index pick up error,index arm move to center & alarm
                            bShowShuttle1Device=true;                                                                                                           //kevin 20180504 index pick up error
                        }
                    }
                    bIndexPickUpErrorWaitRetry=false;                                                                                                           //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                    if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                                                             //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
                    {
                        bIndexPickErrShtStayRight1=false;
                    }

                    if(IniConfig.bD43IndexPickErrCheckSocket==true)                                                                                             //Steven 20190115 : SCC要求吸取異常要檢查Socket
                    {
                        bIndexArm1PickUpErrNeedPiggyback=true;
                        if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                                                                                    //Sam 20230108 : 矽格俊堯要求不要 Reset IC
                        {
                        }
                        else
                        {
                            fMain->ResetRecordforPiggyBack("RESET_ForIndexPickUpErr");
                        }
                        iWhoTriggerPiggyBack=pbtIndexArmPickUpErr;
                        ProcessPiggyBackFunction();
                    }
                }
                else
                {
                    if(IniConfig.bD43AutoRetryWhenIndexPickErr &&                                                                                               //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
                       iD43AutoRetryWhenIndexPickErrCnt[0]==0)
                    {
                        iD43AutoRetryWhenIndexPickErrCnt[0]++;
                    }
                    else
                    {
                        iD43AutoRetryWhenIndexPickErrCnt[0]=0;                                                                                                  //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
                        for(int i=0; i<FTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<FTestSuck.iShtCol; j++)
                                if(FTestSuck.Suck[i][j].Error)
                                    bArm1DuplicateErr[i][j]=true;
                        }
                    }
                    bIndexPickUpErrorWaitRetry=true;                                                                                                            //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                    if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                                                             //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
                    {
                        bIndexPickErrShtStayRight1=true;
                        bFTestSuckHasError=false;   //Eastsun 20260526 #026-4.9 Ifor 20240413 :客戶需求 Retry 後重新確認
                    }
                    bIndexPickUpErrorWaitRetry=true;                                                                                                            //Ifor 20171119 : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                }

                FTestSuck.ResetAll();                                                                                                                           //Steven 20160323 : 避免未開啟真空
            }
            else if(IniConfig.bD81IndexCheckVacuumOnShuttle && bHasICErr)
            {
                if(IndexAlarmInArmAway()==false)                                                                                                                //Steven 20130613 : Index異常時, In Arm要先讓位功能
                {
                    return false;
                }
                bHasICErr=false;
                bSht1OnlyHasICErr=true;

                str.sprintf("Need to remove device of Shuttle1 %s", ErrPart2);
                ShowMyMessage(str);

                if(IniConfig.bD42IndexPickICShuttlePause)
                {
                    bInArmNeedToSafePos=true;
                    bShuttle1Pause=true;
                    bIndexArm1PickupErrStop=true;                                                                                                               //jou 2012-02-29 index pick up error,index arm move to center & alarm
                    bShowShuttle1Device=true;                                                                                                                   //kevin 20180504 index pick up error
                }
            }

            if(IniConfig.bNewResetFunction==true && bResetIndexArm1Pick==true)
            {
                bResetIndexArm1Pick=false;
            }
            else
            {
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                                                                 //ChungHung 20120717 add Index Drop Error Can Retry and Start
                {
                    MOT[MInShuttle1].fCanMoveM=true;                                                                                                            //等待Shuttle 移至右邊
                    bShuttle1MoveToRight=true;
                    bShuttle1HasPickErr=true;                                                                                                                   //Steven 20220712 : 避免In arm 偷放料
                }
            }

            if(bShuttle1Pause)
                MOT[MInShuttle1].SetSpeed(10);                                                                                                                  //kevin 20180226 (Steven) add pick up error  shuttle down speed

            if(IniConfig.bD42IndexPickICShuttlePause &&                                                                                                         //AI(staterecord-analysis) 20260417 (RogerYang) : D42 SKIP後強制走500, 避免300讓Z1再次下降造成四方死鎖
               bShuttle1Pause)
            {
                Task=500;
            }
            else if(FLCarryKit.HasRealIC())
            {
                Task=300;
            }
            else if(CosFunction.bIndexPickErrSkipNeedCheckVac==true &&
                    IniConfig.bD50IndexPickErrSkipNeedCheckVac &&
                    bArm1PressSkipNeedDownCheckVac==true)                                                                                                       //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
            {
                Task=300;
            }
            else
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FLCarryKit.Item[i][j])
                        {
                            if(FLCarryKit.Item[i][j]==HAS_NULL_IC &&                                                                                            //ChungHung 20120911 add
                               FTestSuck.Item[i][j]!=NULL_IC)
                            {
                                FLCarryKit.SetItemData(i, j, NULL_IC);
                            }

                            if(FLCarryKit.Item[i][j]==HAS_NULL_IC)
                            {
                                FTestSuck.Suck[i][j].Normal();
                                W64B_MoveSuckData(FTestSuck, FLCarryKit, i, j);                                                                                 //golden FTestSuck.MoveSuckData(FLCarryKit,i,j) -- TODO(W7) see file-header gate #12
                                bArm1DuplicateErr[i][j]=false;
                            }
                        }
                    }
                }

                if(FLCarryKit.UseSiteHasIC())
                {
                    Task=300;
                    break;
                }
                Task=500;
            }
            break;
        case 500:                                                               //ChungHung 20120717 add Index Drop Error Can Retry and Start
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
               bShuttle1MoveToRight)                                            //ChungHung 20131015 fix hangup
            {
                if(InSHT1InRT()!=true)
                {
                    return false;
                }
            }

            CheckShuttle1EncoderPos();                                          //JerryYang 20230131 : add shuttle shift log

            MOT[MInShuttle1].fCanMoveM=false;
            bShuttle1MoveToRight=false;
            bShuttle1HasPickErr=false;                                          //Steven 20220712 : 避免In arm 偷放料
            bIndexPickUpErrorWaitRetry=false;                                   //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup

            if(DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==true &&
               LastSet.iTemperature==Tempture_Hot)                              //Ifor 20180606 (wei) : add Index 再Shuttle 上等待測試結果
            {
                if(fRearNeedTest==true && bD52IndexArmUp==false)                //JerryYang 20200804 : fix D52 & Index arm在shuttle高度預熱功能同時啟用時，發生tester timeout時會誤發handler hang up
                    break;
                if(BTestSuck.HasRealIC()==false)                                //JerryYang 20220805 : 下壓shuttle預熱功能initial第一次吸shuttle時需下壓等待Jam soak time
                {
                    if(hDoFrontTestSuckICdelay.Off()==false)
                        break;
                }
            }
            Task=501;
            iHomeAlarm=0;
            if(iNN==NN_2Row)
                break;
        case 501:                                                                                                                                               //ChungHung 20130924 add
            if(MOT[MTestZ1].Gali_ReadPos()>(Prod.TestZ1_Pick+1000))                                                                                             //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            {
                bZ1PickShuttle=false;
            }
            sp=GetIndexZSpeed(0);                                                                                                                               //Steven 20160524 : Index Z軸速度整合為Function
            #ifdef DEBUG_INDEX_UPH
            flag1=MOT[MTestZ1].Gali_MotMove2(Prod.TestZ1_Safe, iIndexSpeed, iIndexAcc);
            #else
            flag1=MOT[MTestZ1].Gali_MotMoveNoWait(Prod.TestZ1_Safe, sp, 0);
            #endif
            if(flag1)
            {
                bSuckingFlagZ1=false;
                #ifndef SOFT_SIMULTE
                if(TestIF_File.iShuttleMode==0 ||                                                                                                               //Ifor 20160303 新增馬達回Home保護機制
                   (TestIF_File.iShuttleMode==1 &&
                    TestIF_File.iShuttle_Sel==0))
                {
                    MOT[MTestZ1].ScanMotorStatus();
                    if(MOT[MTestZ1].Led[iHomeLed]==false)
                    {
                        if(iHomeAlarm>10)
                        {
                            ShowMyMessage("Motor Z1 not at the origin!!", "馬達Z1不在原點上!");
                            fAllMotorHome=false;
                            iHomeAlarm=0;
                        }
                        else
                        {
                            iHomeAlarm++;
                        }
                        return false;
                    }
                }
                #endif
                iHomeAlarm=0;
                bZ1PickShuttle=false;                                                                                                                           //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                                                                 //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
                {
                    bIndexPickErrShtStayRight1=false;
                }
                #ifndef SOFT_SIMULTE
                {
                    if(LastSet.iRealDummy==REALLY)
                    {
                        ErrPart=" ";
                        bHasErr=false;
                        bHasDuplicateErr=false;
                        for(int i=0; i<FTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<FTestSuck.iShtCol; j++)
                            {
                                if(bArm1DuplicateErr[i][j])
                                    bHasDuplicateErr=true;

                                if(FTestSuck.Item[i][j] &&
                                   FTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                   FTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    bHasErr=true;
                                    FTestSuck.Suck[i][j].Normal();                                                                                              //Steven 20110707 : 預防負壓掉料
                                    FTestSuck.Suck[i][j].Error=true;
                                    ErrPart+=IndexSuckName[i+iNN][j];
                                }
                                else
                                {
                                    FTestSuck.Suck[i][j].Error=false;
                                }
                            }
                        }

                        if(bHasErr)
                        {
                            if(IndexAlarmInArmAway()==false)                                                                                                    //Steven 20130613 : Index異常時, In Arm要先讓位功能
                            {
                                return false;
                            }

                            bHasErr=false;
                            if(CosFunction.bJAM0303NeedOpenChamberDoor)                                                                                         //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
                                bIsTestSitICFallDown=true;                                                                                                      //kevin 20130706
                            if(IniConfig.bIndexDropOnlySKIP==true || IniConfig.bKoreaFunction)                                                                  //jou 2012-02-13 index drop error only skip
                            {
                                ret=ShowErrorMessage("JAM0303", K_SKIP, MTestZ1, bHasDuplicateErr, ErrPart);                                                    //Device Drop Error
                            }
                            else
                            {
                                ret=ShowErrorMessage("JAM0303", K_SKIP|K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);                                            //Device Drop Error
                            }

                            if(ret==K_SKIP)
                            {
                                bAutoSiteMapWaitTestResult=false;                                                                                               //Ifor 20180115 (Steven) : add Site Mapping SKIP 需清除旗標
                                for(int i=0; i<FTestSuck.iShtRow; i++)
                                {
                                    for(int j=0; j<FTestSuck.iShtCol; j++)
                                    {
                                        if(FTestSuck.Suck[i][j].Error)
                                        {
#if 0 // TODO(W7) -- golden :2095 FTestSuck.PordRec[i][j].AddErrorRecord (TMyProductionRecord declaration-only forward-stub, no .cpp yet -- see file-header gate #14)
                                            FTestSuck.PordRec[i][j].AddErrorRecord("JAM0303");                                                                  //Steven 20161214 : 加上Index異常Skip的ErrorLog
#endif
                                            if(CosFunction.bUseSCKART)                                                                                          //Steven 20161214 (wei) : For SCK ART
                                            {
                                                fSCKART->AddOutputJamCnt(i, j, ret,
                                                    Prod.bIsPassBin[FTestSuck.iBinData[i][j]]);                                                                 //RogerYang 20250923 : 整合ART OutArm JamCount
                                            }
                                            FTestSuck.SetItemData(i, j, HAS_NULL_IC);
                                            FTestSuck.Suck[i][j].Error=false;
                                        }
                                        bArm1DuplicateErr[i][j]=false;
                                    }
                                }

                                if(IniConfig.bD42IndexPickICShuttlePause)                                                                                       //ChungHung 20110302 start
                                {
                                    bInArmNeedToSafePos=true;
                                    bShuttle1Pause=true;
                                    bIndexArm1PickupErrStop=true;                                                                                               //jou 2012-02-29 index pick up error,index arm move to center & alarm
                                    bShowShuttle1Device=true;                                                                                                   //kevin 20180504 index pick up error
                                }

                                if(IniConfig.bD43IndexPickErrCheckSocket==true)                                                                                 //Steven 20190115 : SCC要求吸取異常要檢查Socket
                                {
                                    bIndexArm1PickUpErrNeedPiggyback=true;
                                    if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                                                                        //Sam 20230108 : 矽格俊堯要求不要 Reset IC
                                    {
                                    }
                                    else
                                    {
                                        fMain->ResetRecordforPiggyBack("RESET_ForIndexDropUpErr");
                                    }
                                    iWhoTriggerPiggyBack=pbtIndexArmPickUpErr;
                                    ProcessPiggyBackFunction();
                                }
                            }
                            else
                            {
                                for(int i=0; i<FTestSuck.iShtRow; i++)
                                {
                                    for(int j=0; j<FTestSuck.iShtCol; j++)
                                    {
                                        if(FTestSuck.Suck[i][j].Error)
                                        {
                                            bArm1DuplicateErr[i][j]=true;
                                            W64B_MoveSuckData(FLCarryKit, FTestSuck, i, j);                                                                     //golden FLCarryKit.MoveSuckData(FTestSuck,i,j) -- TODO(W7) see file-header gate #12
                                            FTestSuck.Suck[i][j].Error=false;
                                        }
                                    }
                                }
                                Task=300;
                                return false;
                            }
                        }
                    }
                }
                #endif
                if(bShuttle1Pause)
                    MOT[MInShuttle1].SetSpeed(10);                                                                                                              //kevin 20180226 (Steven) add pick up error  shuttle down speed

                MOT[MInShuttle1].fCanMoveM=true;
#if 0 // TODO(W7) -- golden :2155 FTestSuck.SetUnuseToNullIC (TMyKitSuck substrate, no such method yet -- see file-header gate #13)
                FTestSuck.SetUnuseToNullIC();                                                                                                                   //Steven 20241017 : 清除沒用到的資料
#endif

                if(DeviceForm_File.ContactMode==DirectContactSoftEP ||                                                                                          //kevin 20130608 Soft Contact mode
                   DeviceForm_File.ContactMode==DropContactSoftEP)
                {
//                    bContSoftEpSwitch(0, false);                                //ARM1 shuttle 吸完ic 浮動頭不充氣  //JerryYang 20151202 true->flase
                    EPSwitchOnOff(eEPSwArm2);
#if 0 // TODO(W7) -- golden :2162 APAX_WriteData (different DAQ brand than ADAM_*, no translated home -- see file-header gate #18)
                    APAX_WriteData(true, 0, 1);
#endif
                }

#if 0 // TODO(W7) -- golden :2165-2178 fSCKART->iInfo_MultiLotCnt + LotSummary.AddByLotLoadCount (FormsFacade TfSCKART has no iInfo_MultiLotCnt member; LotSummary has no translated home at all -- see file-header gates #15/#16)
                if(fSCKART->iInfo_MultiLotCnt>1)
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(FTestSuck.Item[i][j]!=NULL_IC &&
                               FTestSuck.cDeviceInf[i][j]!="")
                            {
                                LotSummary.AddByLotLoadCount(FTestSuck.cDeviceInf[i][j]);
                            }
                        }
                    }
                }
#endif

#if 0 // TODO(W7) -- golden :2180 fAutoTeach->SetIndexBinError (no FormsFacade TfAutoTeach stand-in -- see file-header gate #17)
                fAutoTeach->SetIndexBinError(0);                                                                                                                //JimmyChiu 20211020 : Auto alignment mode
#endif
                if(bHPCleanout)                                                                                                                                 //wei 20160624 Hotplate clean out
                {
                    return true;
                }
                else if(DeviceForm.bSuckShuttleDeviceAfterTested==false &&                                                                                      //Steven 20140827 : 第一次吸取ic等待時間
                        DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==false &&                                                                               //JerryYang 20220805 : 有開下壓shuttle預熱功能就不要做懸空預熱
                        (LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot) && bInitialSackTime &&                                //JerryYang 20181214 : non hotplate mode也要支援溫度優先功能              //2013-11-27   Dell    需要做Index soak time
                        (Temperature.iInitialStart1Time>0 || Temperature.iIndexSoakTime>0))                                                                     //kevin 20131112 第一次吸取ic等待時間)
                {
                    int iSetTmr=Temperature.iInitialStart1Time;
                    if(Temperature.iIndexSoakTime>Temperature.iInitialStart1Time)
                        iSetTmr=Temperature.iIndexSoakTime;

                    DoTestHeadMotorDelay2.SetSecAndOn(iSetTmr);
                    Task=502;
                    bInitialSackTime=false;
                    bInitialStart1Time=true;                                                                                                                    //wei 20171020 (jou) InitialStart1 秒數倒數
                    dwStartInitialCount1.LatchCycleTime(true);
                    iInitialStart1Count=Temperature.iInitialStart1Time;
                }
                else
                {
                    return true;
                }
            }
            break;
        case 502:                                                               //kevin 20131112  等待initial sock time
            if(bInitialStart1Time)                                              //wei 20171020 (jou) InitialStart1 秒數倒數
            {
                if(iInitialStart1Count>0)
                {
                    iInitialStart1Count=(Temperature.iInitialStart1Time)-(dwStartInitialCount1.LatchCycleTime()/1000);
                    break;
                }
            }

            if(DoTestHeadMotorDelay2.Off())
            {
                return true;
            }
            break;
    }
    return false;
}
