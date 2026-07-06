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
