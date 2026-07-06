//---------------------------------------------------------------------------
//  aTester_Rear.cpp  --  TESTER/INDEX ENGINE, Rear (Z2/Arm2) narrow-scope slice
//
//  Translation wave: W64b-Translate (paired with a sibling aTester_Front.cpp
//  translated concurrently by another agent -- this file is self-contained
//  except for the one CheckZ1IsDown() cross-call declared in aTester_Front.h)
//  Translator: AI(W64b-Translate) 20260706
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/aTester_Rear.cpp (9787
//  lines, cp950), translating exactly:
//    * bArm2PutDuplicateErr[][]      golden :89-92
//    * ResetArm2PutFlag()            golden :126-135
//    * CheckZ2IsDown()                golden :170-222
//    * TestZ2OutRandge()              golden :224-255
//    * TestZ2SetPos()                 golden :291-298
//    * iRearTestDestroyICTask + InitRearTestDestroyICTask()  golden :300-305
//    * DoRearTestDestroyIC(bool)      golden :309-886
//  All other golden aTester_Rear.cpp content (DoRearTestSuckIC, CheckBTFullSite,
//  ResetInxedArm2Flag, ResetArm2VacFlag, DoBTestSuckTestIC, TestZ2OutRandge2,
//  BTestCombineSLK/BTestSeparateSLK, DoBRTCAutoModelVerify, the D44/RTC/torque/
//  purge sub-engines, etc.) is OUT OF SCOPE and stays routed through
//  atester_shims.cpp as today.
//
//  KNOWN GAPS discovered while translating (none silently dropped -- each is
//  gated below with a compiling offline default and a golden line citation;
//  see the W64bT2 stub block right before DoRearTestDestroyIC, plus the two
//  #if 0 // TODO(W7) blocks inside case 310):
//    * TMyKitSuck::bNeedCheck[][]              (aHotPlateSubstrate.h) -- absent
//    * TMyKitSuck::MoveSuckData(...)           (aHotPlateSubstrate.h) -- absent
//    * TMyKitSuck::SetHasNullIcToNullIc()      (aHotPlateSubstrate.h) -- absent,
//      ALREADY a documented gap (docs/DEVLOG.md, MIGRATION_ROADMAP.md) and
//      already gated the same way in csystem.cpp (W7C2_BLCARRYKIT_SETHASNULLIC)
//    * TMyProductionRecord::AddIndexPlaceShuttleRecord(...) -- absent
//    * TMySucker::sRecordStartOffDelayTime/sRecordEndOffDelayTime/
//      iRecordOffDelayTime -- absent (gated inline, #if 0)
//    * LAST_GENERAL_SET::iIndexInputOutPut[] (canary_support.h shim) -- absent
//      (gated inline, #if 0; canary_support.h is a shared file this task may
//      not edit)
//    * DoVibrateOutShuttle(int,bool) (golden ainarm2.h:215) -- ainarm2.cpp only
//      has SetInArmUseSuckToHasNullIC/InitInArmPickFromHotPlateTask so far
//    * RecordIndexAirOnTime2() (golden cObserver.h:565) -- cObserver.cpp not
//      yet translated
//    * TfiosetviewShim::ProcessIndexSuckDestroy2(int) (golden iosetview.h:3019)
//      -- the shim (atester_shims.h) exposes bIndexSuck[][][] but not this
//      method (atester.cpp has its own zero-arg W7T1_ precedent for this exact
//      gap; mine takes the arg the Rear call sites actually pass)
//    * ATC_TYPE_33 / ATC_TYPE_35 (golden ATC/ATC_Handler_Side.h:24-25) -- that
//      header pulls in the untranslated ATC subsystem, so the two fixed
//      literal values are supplied locally instead (not a behavioural
//      approximation -- exact real values)
//    * TQPF_Timer BTempRiseHasICDelay (golden :308, file-scope peer of
//      DoRearTestDestroyIC, analogous to bArm2PutDuplicateErr) -- added here
//    * TMyProductionRecord::AddIndexCycleTimeRecord(double) (golden
//      MyProductionRecord.h:38) -- same declaration-only forward-stub class as
//      AddIndexPlaceShuttleRecord above (no .cpp this wave); gated the same
//      way (golden :514).  AI(W64b-Integrate) 20260706.
//
//  INTEGRATE-STEP FIXES (AI(W64b-Integrate) 20260706 -- discovered as
//  undefined-reference LINK errors while wiring this file into the build, not
//  caught by the translation-time call-graph review since both are golden
//  symbols DECLARED in an already-translated header with NO compiled body
//  anywhere in the tree):
//    * OutSHT2InLF() / CheckIndexAllSuckICFallDown() (golden csystem.cpp:700 /
//      :2417) -- csystem.h declared them but csystem_predicates.cpp never
//      defined them.  Translated FAITHFULLY (not gated) into
//      csystem_predicates.cpp.
//    * GetSiteCount(bool) (golden cprod.cpp:305, declared cprod.h:3299) --
//      real body exists but sits inside cprod.cpp's whole-region
//      `#if 0 // TODO(W6)` gate (cprod.cpp:184-4036); a faithful duplicate was
//      added to atester_shims.cpp (same pattern already used there for
//      IsInArmCleanOutFinish/IsInArmOneCycleFinish).
//  Integrate-step note: atester_shims.h/.cpp TODAY still declare+define
//  offline-stub bodies for all 6 symbols this file now defines for real
//  (iRearTestDestroyICTask, InitRearTestDestroyICTask, DoRearTestDestroyIC,
//  CheckZ2IsDown, TestZ2OutRandge, TestZ2SetPos) -- REMOVED from
//  atester_shims.h/.cpp as part of this same integrate step.
//
//  TRANSLATION RULES (mirrors atester.cpp / aTester_Front.cpp):
//    * AnsiString/TQPF_Timer via the substrate headers; numeric semantics EXACT.
//    * SOFT_SIMULTE / DEBUG_INDEX_UPH NOT defined in this tree -> the #ifdef
//      bodies for both are reproduced VERBATIM (inert, matches golden text
//      byte-for-byte including golden's own missing-paren typo in the dead
//      DEBUG_INDEX_UPH branch of case 100 -- never parsed since the branch is
//      preprocessed away, exactly as it never was in the golden BCB6 build).
//    * Big5 Chinese comments decoded via cp950, preserved as UTF-8.  ZERO U+FFFD.
//---------------------------------------------------------------------------
#include "MachineDefine.h"

#include "aTester_Rear.h"

#include "atester.h"                // GetIndexZSpeed / DoTemperatureRise / IndexAlarmInArmAway / IndexCheck4Site
#include "atester_shims.h"          // fiosetview (TfiosetviewShim) / IsNNMode
#include "aTester_Front.h"          // CheckZ1IsDown() -- sibling translation unit (parallel wave)
#include "aArmHeader.h"             // guard-only shim (matches atester.cpp convention)

#include "MachineType.h"           // eSpeedPart::IndexArm / eNewATCSystem / MAX_Index_Row / MAX_SOCKET_ROW/COL / CC_KYEC_LEE
#include "Motor/mymotor.h"         // MOT[] / TTrayMotor / ShowIndexMotorError / RecordIndexPositionError
#include "vclcompat/vcl_compat.h"  // AnsiString
#include "csystem.h"                // InShtInLF / OutSHT2InLF / CheckIndexAllSuckICFallDown / IsInArmOneCycleFinish / IsInArmCleanOutFinish / InSHT2InRT
#include "cprod.h"                  // Prod / TestIF / TestIF_File / ArmSpeed_File / GetSiteCount / Temperature
#include "cmydef.h"                  // global scalar universe + IC consts + K_SKIP/K_RETRY/REALLY + bArm2D44SuckCheck/...
#include "aHotPlateSubstrate.h"     // BLCarryKit / BRCarryKit / BTestSuck / TestSocket (TMyKitSuck) + TMySucker
#include "CosFunction.h"            // CosFunction
#include "Config.h"                  // IniConfig
#include "acarry_shims.h"           // ATC_InterfaceForm (TATC_InterfaceFormShim*)
#include "canary_support.h"         // LastSet / ShowMyMessage / ShowErrorMessage / __FUNC__
//---------------------------------------------------------------------------

// golden :89-92 -- plain global,排除重複的Alarm Code (Steven 20100105 Start)
bool bArm2PutDuplicateErr[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                           {false, false, false, false, false, false, false, false},
                                                           {false, false, false, false, false, false, false, false},
                                                           {false, false, false, false, false, false, false, false}};
//------------------------------------------------------------------------------
// golden :126-135 (Steven 20241017 : 打包成function).  NOTE: golden Rear's
// ResetArm2PutFlag loops over BTestSuck (its OWN kit instance), NOT FTestSuck --
// verified against golden text directly (deviates from the up-front task
// paraphrase, which is expected/flagged per the task instructions).
void ResetArm2PutFlag()
{
    for(int i=0; i<BTestSuck.iMaxRow; i++)
    {
        for(int j=0; j<BTestSuck.iMaxCol; j++)
        {
            bArm2PutDuplicateErr[i][j]=false;
        }
    }
}
//------------------------------------------------------------------------------
bool CheckZ2IsDown()
{
    #ifdef SOFT_SIMULTE
        return true;
    #else
        long lPos[4]={0, 0, 0, 0};                                              //kevin 20150915

        if(IniConfig.bD52InterFaceErrHeadNeedUp && bNowDoInterFaceErrorStep)
            return false;

        if(bContactTimeOverStep)
            return false;

        if(CosFunction.bSortingBy2DList==true &&
           LastSet.iTester==_2D_SORT &&
           TestIF_File.bSortingBy2DIDList==true)                                //Frank 20221122 : 2DID sorting for ATK
        {
            return true;
        }

        if(MOT[MTestZ2].Motor->Enable==false)
            return true;

        if(TestIF_File.iShuttleMode==1 &&                                       //20111114  Dell for Disable Index Arm    Start
           TestIF_File.iShuttle_Sel==0)                                         //jou 2014-11-13 remake code , close arm index postion error
        {
            return true;
        }

        if(bUseTwoArm32Site==true)
        {                                                                       //nn Mode 是同時有吸取IC動作
            return true;
        }
        else
        {
            //==> Eastsun 20260511 F007 整合: Ifor 20240430 add:secs gem cmd Index Yiel Fail 0:Normal 1:收到命令 2:Index 上升
            if(CUSTOMER_CODE==CC_KYEC_LEE && iSECSGEM_ConsecutiveFailureAlarm==2)
            {
                return true;
            }
            else if(MOT[MTestZ2].Gali_ReadEncoderPos()>Prod.iTestDown_Z2)
            //<== Eastsun 20260511 F007 整合
            {
                lPos[2]=MOT[MTestZ2].Gali_ReadEncoderPos();                     //kevin 20150915
                lPos[3]=Prod.iTestDown_Z2;                                      //kevin 20150915
                RecordIndexPositionError("CheckZ2IsDown()", false, false, true, true, &lPos[0]);                        //kevin 20150915 record
                ShowIndexMotorError(AnsiString("CheckZ2IsDown"));
                return false;
            }
        }
        return true;
    #endif
}
//==============================================================================
bool TestZ2OutRandge()
{
    #ifdef SOFT_SIMULTE
        return false;
    #else
        int Pos1=MOT[MTestZ2].Gali_ReadPos();
        int Pos2=MOT[MTestZ2].Gali_ReadEncoderPos();
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
void TestZ2SetPos()
{
    int Pos3=ChangeToFloatNonPcnt((double)(MOT[MTestZ2].Gali_ReadEncoderPos()), (double)(MOT[MTestZ2].Motor->GearRatio));                                       //Steven 20100728 Start: 要加上齒輪比
    int Pos4=ChangeToFloatNonPcnt((double)(MOT[MTestY2].Gali_ReadEncoderPos()), (double)(MOT[MTestY2].Motor->GearRatio));
    char str[256];
    sprintf(str, "DP,,%d,%d", 0-Pos3, 0-Pos4);
    MOT[MTestY2].Gali_Command(str, __FUNC__);
}
//---------------------------------------------------------------------------
int iRearTestDestroyICTask=1;
void InitRearTestDestroyICTask()
{
    iRearTestDestroyICTask=1;
    bZ2Isdownflag=false;
}
//------------------------------------------------------------------------------
// golden :308 -- file-scope peer of DoRearTestDestroyIC (same status as
// bArm2PutDuplicateErr above); golden :307 CheckSocketHasICDelay2 is NOT added
// -- every golden reference to it inside DoRearTestDestroyIC (case 700/750) is
// itself commented-out dead code, reproduced verbatim as comments below.
TQPF_Timer BTempRiseHasICDelay;

// =============================================================================
//  W64bT2 offline gap stubs -- see file-top "KNOWN GAPS" list for the full
//  rationale on each.  All are file-scope (static where applicable) so they
//  cannot collide at link time with a future real implementation landing in
//  aHotPlateSubstrate.h / ainarm2.cpp / cObserver.cpp / atester_shims.h.
//  Integrate: once the real members/functions exist, drop this block and the
//  call-site substitutions below and restore the golden text verbatim.
// =============================================================================
// -- TMyKitSuck::bNeedCheck[][] (golden MyKitSuck.h:174) -- per-nozzle "destroy
//    just fired, needs a vacuum re-check" latch (D44 recheck sub-flow, case
//    310/500/600).  Offline default: no site ever needs a recheck (false),
//    which routes the SM through the "no recheck needed" branch -- mirrors how
//    other offline gaps in this tree default (e.g. atester.cpp's
//    W7T1_CHECKVACINIT leaves its out-param false).
static bool W64bT2_BNeedCheckGet(int /*i*/, int /*j*/) { return false; }
static void W64bT2_BNeedCheckSet(int /*i*/, int /*j*/, bool /*val*/) {}

// -- TMyKitSuck::MoveSuckData (golden MyKitSuck.h:268) -- copies one site's
//    decoded per-site data from Source[SourceR][SourceC] into this kit's same
//    cell.  Offline default: no-op (BRCarryKit's grid keeps its current data;
//    the test-result decode path reads off TestSocket directly, not
//    BRCarryKit, so no result data is lost by this gap).
static void W64bT2_MoveSuckData(TMyKitSuck &dst, TMyKitSuck &src, int r, int c)
{
    (void)dst; (void)src; (void)r; (void)c;
}

// -- TMyProductionRecord::AddIndexPlaceShuttleRecord (golden MyProductionRecord.h)
//    per-site Index->OutShuttle place log record.  Offline default: no-op
//    (pure logging, no state read back by the SM).
static void W64bT2_AddIndexPlaceShuttleRecord(TMyProductionRecord &rec, int siteMap, int r, int c)
{
    (void)rec; (void)siteMap; (void)r; (void)c;
}

// -- TMyKitSuck::SetHasNullIcToNullIc (golden MyKitSuck.h) -- KNOWN, already
//    documented gap (docs/DEVLOG.md, MIGRATION_ROADMAP.md); TMyKitSuck only has
//    the inverse SetNullIcToHasNullIc.  Gated the same way csystem.cpp already
//    gates it (W7C2_BLCARRYKIT_SETHASNULLIC): offline no-op.
static void W64bT2_SetHasNullIcToNullIc(TMyKitSuck &kit) { (void)kit; }

// -- DoVibrateOutShuttle (golden ainarm2.h:215 `bool DoVibrateOutShuttle(int
//    iShuttle, bool bNeedInitial=false);`) -- ainarm2.cpp only carries
//    SetInArmUseSuckToHasNullIC/InitInArmPickFromHotPlateTask so far.  Offline
//    default: report "done" (true) immediately so the SM does not stall on it
//    (mirrors the "offline: destroy done immediately" default used elsewhere,
//    e.g. ainarm9045.cpp ProcessSCKARTLoadingCount case 1).
static bool W64bT2_DoVibrateOutShuttle(int iShuttle, bool bNeedInitial=false)
{
    (void)iShuttle; (void)bNeedInitial;
    return true;
}

// -- RecordIndexAirOnTime2 (golden cObserver.h:565 `void RecordIndexAirOnTime2();`)
//    -- cObserver.cpp not yet translated.  Offline no-op (pure logging).
static void W64bT2_RecordIndexAirOnTime2() {}

// -- TfiosetviewShim::ProcessIndexSuckDestroy2(int) (golden iosetview.h:3019
//    `bool ProcessIndexSuckDestroy2(int iType=0);`) -- atester_shims.h's
//    TfiosetviewShim exposes bIndexSuck[][][] but not this method.  atester.cpp
//    already has its own zero-arg W7T1_ precedent for the SAME gap; this one
//    takes the int arg the Rear call sites actually pass.  Offline default:
//    report suck self-check "done" (true), same offline value as the W7T1
//    precedent.
static bool W64bT2_ProcessIndexSuckDestroy2(int iType=0) { (void)iType; return true; }

// -- ATC_TYPE_33 / ATC_TYPE_35 (golden ATC/ATC_Handler_Side.h:24-25, fixed
//    literal #defines 33/35) -- that header pulls in the full untranslated ATC
//    subsystem, so not included here; the two values are hard-coded locally
//    (exact real values, not an approximated default).
#ifndef ATC_TYPE_33
#define ATC_TYPE_33   33
#endif
#ifndef ATC_TYPE_35
#define ATC_TYPE_35   35
#endif
//------------------------------------------------------------------------------
TQPF_Timer CheckSocketHasICDelay2;                                              //kevin 20190819 add
bool DoRearTestDestroyIC(bool bCheckZ1)
{
    static int iSiteCount=0;
    static int iRetry=0;                                                        //kevin 20190709  index check one by one

    int &Task=iRearTestDestroyICTask;
    int sp=0, iPos=0;
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
            iIndexTakeDeviceChk2=0;                                             //Steven 20250224 回吸檢測狀態
            if(BRCarryKit.UseSiteHasIC())
            {
                return false;
            }

            if(OutSHT2InLF()==false)
            {
                Task=100;                                                       //JerryYang 20170106 (Steven) fix out shuttle 偵測殘料後hang up
                return false;
            }

            MOT[MInShuttle2].fCanMoveM=false;
            Task=200;
            if(CheckIndexAllSuckICFallDown(false, true))                        //jou 20210713 : 增加index destroy ic時掉料偵測
            {
                ShowMyMessage("Index Arm2 掉料，請檢查IC是否有在Out Shuttle中!!");
            }

            if(IsNNMode()==NN_2Row)
                break;
            break;                                                              //JerryYang 20170106 (Steven) fix out shuttle 偵測殘料後hang up
        case 100:
            #ifdef DEBUG_INDEX_UPH
            if(MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Safe, iIndexSpeed, iIndexAcc);                                    //Steven 20170818 : 測試UPH用
            #else
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, MOT[MTestZ2].GailSpeed))
            #endif
            {
                if(bHasDropErr)                                                                                         //JerryYang 20241122 : Index arm放料到shuttle偵測drop error
                {
                    bHasDropErr=false;
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(BTestSuck.Suck[i][j].Enable       &&
                               BTestSuck.Suck[i][j].SenUsing!="" &&
                               BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                               BTestSuck.Item[i][j]!=NULL_IC)
                            {
                                if(BTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    ErrPart+=IndexSuckName[i][j];
                                    bHasErr=true;
                                }
                            }
                        }
                    }

                    if(LastSet.iRealDummy==REALLY && bHasErr)
                    {
                        if(CosFunction.bJAM0303NeedOpenChamberDoor)                                                     //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
                            bIsTestSitICFallDown=true;
                        ShowErrorMessage("JAM0304", K_SKIP, MTestZ2, false, ErrPart);                                   //Steven 20100129 : Device Drop Error          //Jimmychiu 20260223 : 修正Index Arm 異常顯示位置錯誤
                        for(int i=0; i<MAX_Index_Row; i++)
                        {
                            for(int j=0; j<NEW_MAX_Index_Col; j++)
                            {
                                if(BTestSuck.Suck[i][j].Error ||
                                   (BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                    BTestSuck.Item[i][j]!=NULL_IC &&
                                    BTestSuck.Suck[i][j].GetStatus()==false))                                           //有用到且有吸到IC的卻掉了
                                {
                                    BTestSuck.SetItemData(i, j, HAS_NULL_IC);                                           //Steven 20110829 : 把有IC掉料的位置改成Has Null IC
                                    BTestSuck.Suck[i][j].Normal();                                                      //Steven 20110829 : 把真空關掉
                                }
                            }
                        }
                    }
                }

                if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                                                     //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                    TestIF_File.bArm1PickPlaceArm2Test==true) &&
                    TestIF_File.bArm1OnlyPlaceArm2TestAndSuck==true)                                                    //JerryYang 20250723 : avoid shuttle hang up
                {
                    if(BLCarryKit.HasRealIC()==false &&                                                                 //Arm 2 no料
                       BRCarryKit.HasRealIC()==false)
                    {
                        W64bT2_SetHasNullIcToNullIc(BLCarryKit);                                                        // AI(W64b-Translate) 20260706: golden :407 BLCarryKit.SetHasNullIcToNullIc() -- gap stub (see file-top)
                        W64bT2_SetHasNullIcToNullIc(BRCarryKit);                                                        // AI(W64b-Translate) 20260706: golden :408 BRCarryKit.SetHasNullIcToNullIc() -- gap stub (see file-top)
                        MOT[MInShuttle2].fCanMoveM=true;
                    }
                }
                bSuckingFlagZ2=false;                                                                                   //Steven 20240916 : index下降到shuttle吸放料
                Task=1;
            }
            break;
        case 200:
            bHasDropErr|=CheckIndexAllSuckICFallDown(false, true);              //JerryYang 20241122 : Index arm放料到shuttle偵測drop error
            if(bHasDropErr)
            {
                Task=100;
                break;
            }

            if(InShtInLF(1)==false)                                             //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條
            {
                bZ2Isdownflag=false;                                            //Isaac 20180307 (Steven) index arm在shuttle放置位子時，shuttle抖抖須等到arm起來才能抖
                ShowMyMessage("Shuttle 2 position error", "Shuttle 2 位置錯誤");
                MOT[MInShuttle2].fCanMoveM=true;
                Task=1;
            }
            else if(bCheckZ1==false || CheckZ1IsDown())                         //ChungHung 20111115
            {
                bSuckingFlagZ2=true;                                            //Steven 20240916 : index下降到shuttle吸放料
                flag2=false;                                                    //ChungHung 20110816 add Index Place slow down
                bZ2Isdownflag=true;                                             //Isaac 20180307 (Steven) index arm在shuttle放置位子時，shuttle抖抖須等到arm起來才能抖
                if(BTestSuck.HasRealIC()==false)
                {
                    flag2=true;
                }
                else
                {
                    if(IniConfig.bD54SlowDown)
                    {
                        sp=GetIndexZSpeed(1);                                   //Steven 20160524 : Index Z軸速度整合為Function
                        #ifdef DEBUG_INDEX_UPH
                        if(MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Place, iIndexSpeed, iIndexAcc))
                        #else
                        if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Place, sp))
                        #endif
                        {
                            flag2=true;
                        }
                    }
                    else
                    {
                        #ifdef DEBUG_INDEX_UPH
                        if(MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Place, iIndexSpeed, iIndexAcc))
                        #else
                        if(MOT[MTestZ2].Gali_MotMoveNoWait(Prod.TestZ2_Place, MOT[MTestZ2].GailSpeed, IniConfig.iD46WaitIndexDestroyTime))
                        #endif
                        {
                            flag2=true;
                        }
                    }
                }

                if(flag2)
                {
                    BTestSuck.ResetAll();
                    if(ATC_SYSTEM==eNewATCSystem && Temperature.bEnableTempRise==true && (ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33 || ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35))
                    {
                        DoTemperatureRise(1, true);
                        BTempRiseHasICDelay.SetSecAndOn(Temperature.dTempRiseDelay);
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
            if(BTempRiseHasICDelay.Off())
            {
                DoTemperatureRise(1, false);
                Task=310;
            }
            break;
        case 310:
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Item[i][j])
                    {
                        if(BTestSuck.Item[i][j]==HAS_NULL_IC ||
                           BTestSuck.Suck[i][j].Destroy())
                        {
                            if(IniConfig.bD44CheckIndexICDestroy)
                            {
                                if(BTestSuck.Item[i][j]!=HAS_NULL_IC)
                                {
                                    BTestSuck.Suck[i][j].Off();                                                         //Suck Off and Destroy On
                                    W64bT2_BNeedCheckSet(i, j, true);                                                   // AI(W64b-Translate) 20260706: golden :505 BTestSuck.bNeedCheck[i][j]=true -- gap stub (see file-top)
                                    iIndexTakeDeviceChk2=2;                                                             //kevin 20190103 回吸檢測狀態
                                }
                            }

                            if(BTestSuck.Item[i][j]!=HAS_NULL_IC)
                            {
                                #if 0 // TODO(W7) -- golden :512 LastSet.iIndexInputOutPut[1]++ (LAST_GENERAL_SET shim in canary_support.h -- shared file, not editable from this task)
                                LastSet.iIndexInputOutPut[1]++;                                                         //kevin 20130125
                                #endif
// AI(W64b-Integrate) 20260706: TMyProductionRecord::AddIndexCycleTimeRecord is
// declared (Public/MyProductionRecord.h:38) but that whole class is a
// declaration-only forward-stub this wave (no .cpp; see its file-top banner) --
// same documented gap as W64bT2_AddIndexPlaceShuttleRecord above; gated the
// same way (pure logging, no state the SM reads back).
#if 0 // TODO(W7) -- golden aTester_Rear.cpp:514 BTestSuck.PordRec[i][j].AddIndexCycleTimeRecord (TMyProductionRecord has no .cpp yet)
                                BTestSuck.PordRec[i][j].AddIndexCycleTimeRecord(fRecindexCycleTim);                     //Frank 20160505 add
#endif
                                if(CosFunction.RecordIndexAirOnTime)                                                    //Sam 20220329 : Record Index Air On Time
                                {
                                    #if 0 // TODO(W7) -- golden :515 TMySucker::sRecordStartOffDelayTime/sRecordEndOffDelayTime/iRecordOffDelayTime (not yet members of TMySucker, aHotPlateSubstrate.h)
                                    QueueAirOnTime2.Add(BTestSuck.Suck[i][j].sRecordStartOffDelayTime, BTestSuck.Suck[i][j].sRecordEndOffDelayTime, BTestSuck.Suck[i][j].iRecordOffDelayTime);
                                    #endif
                                }
                            }
                            W64bT2_MoveSuckData(BRCarryKit, BTestSuck, i, j);                                           // AI(W64b-Translate) 20260706: golden :517 BRCarryKit.MoveSuckData(BTestSuck,i,j) -- gap stub (see file-top)
                            W64bT2_AddIndexPlaceShuttleRecord(BRCarryKit.PordRec[i][j], TestIF.iSiteMap[i][j], i, j);    // AI(W64b-Translate) 20260706: golden :518 BRCarryKit.PordRec[i][j].AddIndexPlaceShuttleRecord(...) -- gap stub (see file-top)  //Sam 20201216 : Add record //Sam 20200716 : Add record
                            bArm2PutDuplicateErr[i][j]=false;
                        }
                    }
                }
            }
            flag=false;
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Item[i][j] &&
                       BTestSuck.Suck[i][j].Error==false)
                    {
                        flag=true;
                        break;
                    }
                }
            }

            if(flag==false)
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Suck[i][j].Error)
                        {
                            Task=320;
                            return false;
                        }
                    }
                }
            }

            if(BTestSuck.UseSiteHasIC())
            {
                iRetry=0;
                break;
            }

            if(TestZ2OutRandge())
            {
                iRetry++;
                if(iRetry>20)
                {
                    iRetry=0;
                    TestZ2SetPos();
                }
                else
                {
                    break;
                }
            }
            W64bT2_RecordIndexAirOnTime2();                                                                             // AI(W64b-Translate) 20260706: golden :572 RecordIndexAirOnTime2() -- gap stub (see file-top)  //Sam 20220329 : Record Index Air On Time
            iRetry=0;
            ResetArm2PutFlag();                                                                                         //Steven 20241017 : 打包成function
            if(IniConfig.bF25VibrateForOutShuttle)
            {
                W64bT2_DoVibrateOutShuttle(1, true);                                                                    // AI(W64b-Translate) 20260706: golden :577 DoVibrateOutShuttle(1,true) -- gap stub (see file-top)
            }

            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                                                         //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                TestIF_File.bArm1PickPlaceArm2Test==true) &&
                TestIF_File.bArm1OnlyPlaceArm2TestAndSuck==true)                                                        //Steven 20150129 : 需要確認Arm2有沒有粘料
            {
                BLCarryKit.SetNullIcToHasNullIc();
                BRCarryKit.SetNullIcToHasNullIc();
            }
            Task=500;
            break;
        case 320:                                                                                                       //破壞異常處理
            W64bT2_RecordIndexAirOnTime2();                                                                             // AI(W64b-Translate) 20260706: golden :590 RecordIndexAirOnTime2() -- gap stub (see file-top)  //Sam 20220329 : Record Index Air On Time
            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bArm2PutDuplicateErr[i][j])
                        bHasDuplicateErr=true;
                    if(BTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i][j];
                        BTestSuck.Suck[i][j].Error=false;                                                               //Steven 20101229 : 換位置
                        bArm2PutDuplicateErr[i][j]=true;
                    }
                    else
                    {
                        bArm2PutDuplicateErr[i][j]=false;
                    }
                }
            }

            if(bHasErr)
                ShowErrorMessage("JAM0327", K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);                               //Vacuum sensor OFF error
            bHasErr=false;
            if(ATC_SYSTEM==eNewATCSystem && Temperature.bEnableTempRise==true && (ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33 || ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35))
            {
                DoTemperatureRise(1, true);
                BTempRiseHasICDelay.SetSecAndOn(Temperature.dTempRiseDelay);
                Task=309;
            }
            else
            {
                Task=310;
            }
            BTestSuck.ResetAll();                                                                                       //Sam 20220330 : 補 ResetAll
            break;
        case 500:
            if(IniConfig.bL42_UseOutShuttleDesoakTime==true)                    //Ztex 2023.04.19 Add HT-1032 TriTemp Function
            {
                BTestSuck.TSoakTime.SetSecAndOn(IniConfig.iL42_UseOutShuttleDesoakTime);
            }
            iPos=Prod.TestZ1_Safe;
            sp=GetIndexZSpeed(1);                                               //Steven 20160524 : Index Z軸速度整合為Function
            #ifdef DEBUG_INDEX_UPH
            if(MOT[MTestZ2].Gali_MotMove2(iPos, iIndexSpeed, iIndexAcc))
            #else
            if(MOT[MTestZ2].Gali_MotMoveNoWait(iPos, sp, 0))
            #endif
            {
                bSuckingFlagZ2=true;                                            //Steven 20240916 : index下降到shuttle吸放料
                bZ2Isdownflag=false;                                            //Isaac 20180307 (Steven) index arm在shuttle放置位子時，shuttle抖抖須等到arm起來才能抖 //Steven 20180522 : 換位置, 解除Hang up
                flag=false;
                if(IniConfig.bF25VibrateForOutShuttle)
                {
                    if(W64bT2_DoVibrateOutShuttle(1)==false)                                                            // AI(W64b-Translate) 20260706: golden :647 DoVibrateOutShuttle(1) -- gap stub (see file-top)
                    {
                        break;
                    }
                }
                MOT[MInShuttle2].fCanMoveM=true;                                //JerryYang 20181219 fix D44異常時 shuttle剛好要搖搖會發生hang up
                if(ArmSpeed_File[IndexArm].bDevicConfirm &&
                   INDEX_SUCKER_TYPE==1)                                        //kevin 20190819 add index confim 一次4個SITE
                {
                    TotalErrPart="";
                    Task=650;
                    W64bT2_SetHasNullIcToNullIc(BRCarryKit);                                                            // AI(W64b-Translate) 20260706: golden :658 BRCarryKit.SetHasNullIcToNullIc() -- gap stub (see file-top)
                    return false;
                }

                if(IniConfig.bD44CheckIndexICDestroy)
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(W64bT2_BNeedCheckGet(i, j))                                                              // AI(W64b-Translate) 20260706: golden :668 BTestSuck.bNeedCheck[i][j] -- gap stub (see file-top)
                            {
                                bIndexPlaceIcCheck=true;                        //kevin 20110505
                                flag=true;
                                if(INDEX_SUCKER_TYPE==1)                        //Steven 20111202
                                {
                                    fiosetview->bIndexSuck[1][i][j]=true;
                                }
                                else
                                {
                                    BTestSuck.Suck[i][j].On();
                                }
                            }
                        }
                    }
                }

                if(flag)
                {
                    bD44Arm2CheckVacOn=true;                                    //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                    W64bT2_ProcessIndexSuckDestroy2(1);                                                                 // AI(W64b-Translate) 20260706: golden :688 fiosetview->ProcessIndexSuckDestroy2(1) -- gap stub (see file-top)  //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆 //kevin 20181102 add 提前動作
                    DoRearTestDestroyICDelay.SetMSAndOn(IniConfig.iD44TestHeadCheckVacuumTime);
                    Task=600;
                    break;
                }
                else
                {
                    bD44Arm2CheckVacOn=false;                                   //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                }
                W64bT2_SetHasNullIcToNullIc(BRCarryKit);                                                                // AI(W64b-Translate) 20260706: golden :697 BRCarryKit.SetHasNullIcToNullIc() -- gap stub (see file-top)  //jou 2011-06-29 out shuttle has_null_ic hang up fix    //Steven 20160524 : ADD SetHasNullIcToNullIc
                if(IniConfig.bD44CheckIndexICDestroy)
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            bArm2PutDuplicateErr[i][j]=false;
                            if(W64bT2_BNeedCheckGet(i, j))                                                              // AI(W64b-Translate) 20260706: golden :705 BTestSuck.bNeedCheck[i][j] -- gap stub (see file-top)
                            {
                                BTestSuck.Suck[i][j].OffDestroy();              //kevin 20130719 關吹氣
                            }
                        }
                    }
                }
                iIndexTakeDeviceChk2=0;                                         //kevin 20190103 回吸檢測狀態
                if(((iOneCycle && IsInArmOneCycleFinish()) ||
                    (iCleanOut && IsInArmCleanOutFinish())) &&
                    BLCarryKit.UseSiteNoIC())                                   //Sam 20230809 : OneCycle 最後一次 IndexArm 動作，需要等待 OutShuttle 動作做完才能做 IndexCheck，避免黏料壓壞 IC
                    Task=550;
                else
                    return true;
            }
            break;
        case 550:
            if(BRCarryKit.UseSiteNoIC() ||
               InSHT2InRT())
            {
                return true;
            }
            break;
        case 600:
            if(INDEX_SUCKER_TYPE==1)                                                                                    //jou 2012-01-04 需確認Index suck已經完整做完
            {
                bArm2D44SuckCheck=false;
                bArm2D44SuckCheck=W64bT2_ProcessIndexSuckDestroy2(1);                                                   // AI(W64b-Translate) 20260706: golden :732 fiosetview->ProcessIndexSuckDestroy2(1) -- gap stub (see file-top)  //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆
            }
            else
            {
                bArm2D44SuckCheck=true;
            }

            if(DoRearTestDestroyICDelay.Off() && bArm2D44SuckCheck==true)
            {
                flag=false;
                ErrPart=" ";
                bHasErr=false;
                bHasDuplicateErr=false;
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(bArm2PutDuplicateErr[i][j])
                            bHasDuplicateErr=true;
                    }
                }
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(W64bT2_BNeedCheckGet(i, j))                                                                  // AI(W64b-Translate) 20260706: golden :757 BTestSuck.bNeedCheck[i][j] -- gap stub (see file-top)
                        {
                            if(BTestSuck.Suck[i][j].GetStatus())
                            {
                                bHasErr=true;
                                ErrPart+=IndexSuckName[i][j];
                                flag=true;
                                bArm2PutDuplicateErr[i][j]=true;
                                iIndexTakeDeviceChk2=3;                                                                 //kevin 20190103 回吸檢測狀態
                            }
                            else
                            {
                                W64bT2_BNeedCheckSet(i, j, false);                                                      // AI(W64b-Translate) 20260706: golden :769 BTestSuck.bNeedCheck[i][j]=false -- gap stub (see file-top)
                                BTestSuck.Suck[i][j].Normal();
                                bArm2PutDuplicateErr[i][j]=false;
                            }
                        }

                        if(INDEX_SUCKER_TYPE==1)                                                                        //jou 2011-11-01負壓不能一直開著真空，必須關掉
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                bIndexPlaceIcCheck=false;                                                                               //kevin 20110505
                bD44Arm2CheckVacOn=false;                                                                               //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
                if(bHasErr)
                {
                    if(MOT[MInArmY].ReadPos()<Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase])                     //kevin 20181107 shuttle 1
                    {
                       ShowErrorMessage("JAM0327", K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);                        //Vacuum Sensor Off Error jou 2010-11-23
                    }
                    else if(IndexAlarmInArmAway()==true)                                                                //Steven 20130613 : Index異常時, In Arm要先讓位功能
                    {
                        ShowErrorMessage("JAM0327", K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);                       //Vacuum Sensor Off Error jou 2010-11-23
                    }
                    Task=500;                                                                                           //kevin 20181102
                    return false;
                }
                else
                {
                    iIndexTakeDeviceChk2=0;                                                                             //Steven 20250224 回吸檢測狀態
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
                        for(int i=0; i<BTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<BTestSuck.iShtCol; j++)
                            {
                                if(W64bT2_BNeedCheckGet(i, j))                                                          // AI(W64b-Translate) 20260706: golden :814 BTestSuck.bNeedCheck[i][j] -- gap stub (see file-top)
                                {
                                    bIndexPlaceIcCheck=true;                                                            //kevin 20110505
                                    if(INDEX_SUCKER_TYPE==1)                                                            //Steven 20111202
                                    {
                                        fiosetview->bIndexSuck[1][i][j]=true;
                                    }
                                    else
                                    {
                                        BTestSuck.Suck[i][j].On();
                                    }
                                }
                            }
                        }
                    }
                    DoRearTestDestroyICDelay.SetMSAndOn(IniConfig.iD44TestHeadCheckVacuumTime);
                }
            }
            break;
        case 650:                                                               //kevin 2019819 add onecycle index check 4 Site
            if((ArmSpeed_File[IndexArm].bDevicConfirm ||
               (IniConfig.bD44CheckIndexICDestroy &&
                (CosFunction.bD44Use4ea && GetSiteCount()>4))) &&               //JerryYang 20250120 : modify
                INDEX_SUCKER_TYPE==1)                                           //kevin 20190709 add 20190629 回吸檢測一次 4 個 SITE   //kevin 20190530 add index check
            {
                IndexCheck4Site(true, 1, iSiteCount);
                Task=700;
            }
            break;
        case 700:                                                               //kevin 20190709 20190531 index check 4 Site
            if(IndexCheck4Site(false, 1, iSiteCount))
            {
                iSiteCount++;
                if(iSiteCount<TestSocket.iShtCol/2)                             //JerryYang 20250120 : modify
                    Task=650;
                else
                {
                    Task=750;
                    iSiteCount=0;
//                    CheckSocketHasICDelay2.SetSecAndOn(0.5);     //Steven 20110908 : 上來後也要Delay一下
                }
            }
            break;
        case 750:
//            if(CheckSocketHasICDelay2.Off())
            {
                flag=false;
                if((ArmSpeed_File[IndexArm].bDevicConfirm ||
                   (IniConfig.bD44CheckIndexICDestroy &&
                    (CosFunction.bD44Use4ea && GetSiteCount()>4))) &&           //JerryYang 20250120 : modify
                    TotalErrPart!="")                                           //kevin 20190629 add error show
                {
                   flag=true;
                   ShowErrorMessage("JAM0327", K_RETRY, MTestY2, false, TotalErrPart);
                }
                TotalErrPart="";
                if(flag)                                                        //Steven 20120726 : 有跑IC才檢查Socket
                {
                    Task=650;                                                   //kevin 20181102
                    return false;
                }
                else
                {
                    Task=800;                                                   //pass
                }
            }
            break;
        case 800:
            iIndexTakeDeviceChk2=0;                                             //kevin 20190103 回吸檢測狀態
            return true;
    }
    return false;
}
