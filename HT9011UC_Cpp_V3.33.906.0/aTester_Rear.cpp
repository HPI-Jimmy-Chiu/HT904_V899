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
//
//  WAVE 2 (Translator: AI(W5-aTester_Rear-Translate) 20260710) EXTENDS the file
//  above (same conventions, Wave-1 content untouched) with:
//    * ResetInxedArm2Flag()                            golden :100-113
//    * ResetArm2VacFlag()                              golden :115-124
//    * CheckBTFullSite()                               golden :137-168
//    * iRearTestSuckICTask + InitRearTestSuckICTask()   golden :888-895
//    * DoRearTestSuckIC()                               golden :898-2209
//  (ResetInxedArm2Flag/ResetArm2VacFlag/CheckBTFullSite precede ResetArm2PutFlag
//  in golden line order but are appended AFTER DoRearTestDestroyIC here to avoid
//  touching Wave-1 reviewed code -- same out-of-golden-order append convention
//  already used by aHotPlateSubstrate.h's "-- W6.x ADD" blocks.)
//  All other golden aTester_Rear.cpp content (DoBTestSuckTestIC,
//  DoRearTestPurgBeforePickShuttle/InitRearTestPurgBeforePickShuttle, DoArm2Suck,
//  TestZ2OutRandge2, BTestCombineSLK/BTestSeparateSLK, DoBRTCAutoModelVerify, the
//  D44/RTC/torque/purge sub-engines beyond what DoRearTestSuckIC itself inlines,
//  etc.) remains OUT OF SCOPE and stays routed through atester_shims.cpp as today.
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
//  WAVE 2 KNOWN GAPS (AI(W5-aTester_Rear-Translate) 20260710 -- same "gate with
//  a compiling offline default + golden line citation" discipline; new stub
//  block sits right before DoRearTestSuckIC, after ResetInxedArm2Flag /
//  ResetArm2VacFlag / CheckBTFullSite):
//    * TMyKitSuck::MoveSuckData(...) -- same absent method as above; reuses the
//      Wave-1 W64bT2_MoveSuckData(dst,src,i,j) stub verbatim (golden :1940/:2127)
//    * TMyKitSuck::SetUnuseToNullIC() (golden MyKitSuck.cpp:354-366) -- absent
//      from aHotPlateSubstrate.h's minimal mirror, BUT its golden body only
//      touches members already exposed (SetItemData/iMaxRow/iMaxCol/iShtRow/
//      iShtCol) -- so W64bT2_SetUnuseToNullIC(kit) is a FAITHFUL free-function
//      equivalent, not an approximated no-op (golden :2142)
//    * TMyProductionRecord::AddErrorRecord(...)/AddErrorRecordNoSave(...) --
//      TMyProductionRecord (Public/MyProductionRecord.h) has NO .cpp anywhere in
//      the tree yet (same blanket gap as Wave-1's AddIndexCycleTimeRecord/
//      AddIndexPlaceShuttleRecord) -- gated individually per call site
//      (golden :1730/:1800/:2083); AddErrorRecordNoSave is not even declared in
//      the current minimal MyProductionRecord.h (a further sub-gap)
//    * TfSCKART::iInfo_MultiLotCnt (golden Automation/SCK_ART.h:323) + LotSummary/
//      TLotSummary (golden cSocket.h, entire class untranslated) -- the whole
//      `if(fSCKART->iInfo_MultiLotCnt>1){...}` block is gated (golden :2145-2158)
//    * APAX_WriteData(bool,WORD,int=0) (golden adam6024.h) -- a DIFFERENT DAQ
//      family from the already-shimmed ADAM_* (atester_shims.h); adam6024.cpp
//      itself is not translated.  W64bT2_APAX_WriteData is a TU-local no-op,
//      mirroring the ADAM_* convention (golden :2163)
//    * fAutoTeach (golden AutoTeach.h TfAutoTeach*) -- unlike chkReadTorque/
//      edTorue1 below, fAutoTeach has NO home ANYWHERE in the tree yet (checked:
//      every other TU's fAutoTeach-> use is either fully `#if 0`-gated or a
//      TU-local `#define ...(false)` macro in csystem.cpp).  The single call
//      here (`fAutoTeach->SetIndexBinError(1)`, golden :2165) is gated rather
//      than stubbed since it is a pure UI auto-align marker with no state the
//      SM reads back
//    * fMain->chkReadTorque1/chkReadTorque2/edTorue1 (golden main.h) -- the
//      FormsFacade TfMain (FormsFacade.h) lacks these torque-read UI widgets.
//      atester.cpp's DoTestHeadMotor (T1) already carries an analogous TU-local
//      seam (W7T1_TfMainTorqueSeam) for the SAME golden gap; this file defines
//      its own independent twin (W64bT2_TfMainTorqueSeam, this file's own
//      prefix) rather than reach across translation units (golden :1269-1274/
//      :1443/:1553-1557/:1644)
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
//  WAVE 2 INTEGRATE-STEP NOTE (AI(W5-aTester_Rear-Translate) 20260710):
//  atester_shims.h/.cpp TODAY still declare+define offline-stub bodies for the
//  3 symbols this file now defines for real (iRearTestSuckICTask,
//  InitRearTestSuckICTask, DoRearTestSuckIC) -- REMOVE from atester_shims.h/.cpp
//  as part of the integrate step (same pattern as the Wave-1 note above).
//  bArm2SuckFinish[][] stays in atester_shims.cpp UNCHANGED (a plain data
//  global both DoRearTestDestroyIC and DoRearTestSuckIC merely read/write --
//  not a function this file now defines, so no duplicate-definition risk).
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
// AI(W5-Automation-Integrate) 20260710: ProcessPiggyBackFunction is now real
// (atester_ProcessCount.cpp, removed from atester_shims.h) -- include its own header.
#include "atester_ProcessCount.h"
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
#include "FormsFacade.h"            // (Wave 2) fMain->ResetRecordforPiggyBack / fSCKART
#include "acarry.h"                 // (Wave 2) CheckCFixTrayFullPlace / CheckShuttle2EncoderPos
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

// =============================================================================
//  WAVE 2 gap stubs -- see "WAVE 2 KNOWN GAPS" in the file-top banner for the
//  full rationale on each.  File-scope (static where applicable) so they
//  cannot collide at link time with a future real implementation.
//  Integrate: once the real members/functions/FormsFacade widgets exist, drop
//  this block and the call-site substitutions in DoRearTestSuckIC/
//  ResetInxedArm2Flag, and restore the golden text verbatim.
// =============================================================================
// -- TMyKitSuck::SetUnuseToNullIC (golden MyKitSuck.cpp:354-366) -- absent from
//    aHotPlateSubstrate.h's minimal mirror.  FAITHFUL free-function equivalent:
//    the golden body only touches members already exposed here (SetItemData/
//    iMaxRow/iMaxCol/iShtRow/iShtCol), so this is not an approximation.
static void W64bT2_SetUnuseToNullIC(TMyKitSuck &kit)
{
    for(int i=0; i<kit.iMaxRow; i++)
    {
        for(int j=0; j<kit.iMaxCol; j++)
        {
            if(i>=kit.iShtRow || j>=kit.iShtCol)
                kit.SetItemData(i, j, NULL_IC);
        }
    }
}

// -- APAX_WriteData (golden adam6024.h `void APAX_WriteData(bool bDir, WORD
//    wdata, int iArm=0);`) -- a DIFFERENT DAQ family from the already-shimmed
//    ADAM_* (atester_shims.h); adam6024.cpp itself is not translated.  Offline:
//    no-op (mirrors the ADAM_* convention).  `int` in place of `WORD` for the
//    2nd param avoids depending on <windows.h> WORD being visible in this TU;
//    the only call site passes the literal 0, so this is behaviour-preserving.
static void W64bT2_APAX_WriteData(bool bDir, int wdata, int iArm=0)
{
    (void)bDir; (void)wdata; (void)iArm;
}

// -- fMain torque-read UI widgets (golden main.h) -- FormsFacade TfMain
//    (FormsFacade.h) lacks chkReadTorque1/chkReadTorque2/edTorue1.
//    atester.cpp's DoTestHeadMotor (T1) already carries an analogous TU-local
//    seam (W7T1_TfMainTorqueSeam) for the SAME golden gap; this is aTester_Rear.
//    cpp's own independent twin (this file's own W64bT2_ prefix) rather than
//    reaching across translation units.  Integrate: add the real widgets
//    (TCheckBox/TEdit) to TfMain + drop the W64bT2_FMAIN_* call-site macros.
struct W64bT2_TCheckSeam { bool Checked; W64bT2_TCheckSeam():Checked(false){} };
struct W64bT2_TEditSeam  { AnsiString Text; };
struct W64bT2_TfMainTorqueSeam
{
    W64bT2_TCheckSeam chkReadTorque1, chkReadTorque2;
    W64bT2_TEditSeam  edTorue1;
};
static W64bT2_TfMainTorqueSeam W64bT2_fMainTorque;
// golden derefs these as POINTERS (fMain->chkReadTorque1->Checked, ->edTorue1->
// Text); macros yield &member so the golden `->` deref pattern is preserved.
#define W64bT2_FMAIN_CHKREADTORQUE1 (&W64bT2_fMainTorque.chkReadTorque1)
#define W64bT2_FMAIN_CHKREADTORQUE2 (&W64bT2_fMainTorque.chkReadTorque2)
#define W64bT2_FMAIN_EDTORUE1       (&W64bT2_fMainTorque.edTorue1)

// =============================================================================
//  WAVE 2 (AI(W5-aTester_Rear-Translate) 20260710) -- adds DoRearTestSuckIC + three small
//  golden-earlier prerequisite leaves it (and ResetInxedArm2Flag) call.  These
//  three precede ResetArm2PutFlag in golden line order (Wave 1 already
//  translated ResetArm2PutFlag) but are appended here, after
//  DoRearTestDestroyIC, to avoid touching Wave-1 reviewed code -- same
//  out-of-golden-order append convention already used by aHotPlateSubstrate.h's
//  "-- W6.x ADD" blocks.
// =============================================================================
//------------------------------------------------------------------------------
// golden :100-113 (Steven 20241017 : 打包成function)
void ResetInxedArm2Flag()
{
    for(int i=0; i<BTestSuck.iMaxRow; i++)
    {
        for(int j=0; j<BTestSuck.iMaxCol; j++)
        {
            bArm2SuckFinish[i][j]=false;                                        //Steven 20110301 : 初始化，都當作還沒做完
            bArm2DuplicateErr[i][j]=false;
            BTestSuck.Suck[i][j].Reset();                                       //Steven 20140213 : Jordan說Index下去不吸直接Alarm
        }
    }

    W64bT2_SetUnuseToNullIC(BLCarryKit);                                        // AI(W5-aTester_Rear-Translate) 20260710: golden :107 BLCarryKit.SetUnuseToNullIC() -- gap stub, see below (Steven 20241017 : 清除沒用到的資料)
}
//------------------------------------------------------------------------------
// golden :115-124 (Steven 20241017 : 打包成function)
void ResetArm2VacFlag()
{
    for(int i=0; i<BTestSuck.iMaxRow; i++)
    {
        for(int j=0; j<BTestSuck.iMaxCol; j++)
        {
            bSkipNeedCheckVac[1][i][j]=false;
        }
    }
}
//------------------------------------------------------------------------------
// golden :137-168 (wei 20151228 No FullSite delay)
void CheckBTFullSite()
{
    int iCount=0, iUseSiteCount=0;

    iArmsitecount[1]=0;
    for(int i=0; i<BTestSuck.iShtRow; i++)
    {
        for(int j=0; j<BTestSuck.iShtCol; j++)
        {
            if(BTestSuck.Item[i][j]==HAS_IC ||
               BTestSuck.Item[i][j]==HAS_HOT_IC)
            {
                iCount++;
            }
            iUseSiteCount++;
        }
    }

    if(iCount!=0)
    {
        iArmsitecount[1]=iUseSiteCount-iCount;
        if(iCount!=iUseSiteCount)
        {
            iInitContactCount=0;                                                //Steven 20160519 : 恆溫時溫度要補Offset
            bDoWhenNoFullSiteUseInitialDelay=true;
        }
        else
        {
            bDoWhenNoFullSiteUseInitialDelay=false;
        }
    }
}

// golden :67/:69 -- two more file-scope prerequisites of DoRearTestSuckIC that
// precede bArm2PutDuplicateErr (:89, Wave 1's first translated line); appended
// here (out of golden order, same convention as the rest of this Wave-2 block)
// rather than inserted at the true top of the file.
static bool bIndexZ2NeedUp=false;                                               //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
extern bool CheckCFixTrayFullPlace();                                           // real body already lives in the translated acarry.cpp:225 (golden acarry.cpp:225); golden itself forward-declares this locally rather than via a header, mirrored verbatim
//------------------------------------------------------------------------------
TQPF_Timer hDoRearTestSuckIC;
TQPF_Timer hDoRearTestSuckICdelay;                                              //JerryYang 20220923 : add
int iRearTestSuckICTask=1;
void InitRearTestSuckICTask()
{
    iRearTestSuckICTask=1;
}
//------------------------------------------------------------------------------
extern int CheckOneCycleAction(int iTask);                                      //Steven 20240326 : 判斷one cycle的時候要不要繼續放料
TQPF_Timer dwStartInitialCount2;
bool DoRearTestSuckIC()
{
    static int iHomeAlarm=0;
    static bool bOverHappen=false, bHasDropAtShuttle=false, bHasICErr=false;
    static AnsiString ErrPart2="";
    int ZPos=0;
    int sp=0;
    int &Task=iRearTestSuckICTask, ret=0;
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

            if(iOneCycle &&
               bUseTwoArm32Site &&                                              //針對NN mode的one cycle動作做額外卡控
               BTestSuck.UseSiteNoIC() &&                                       //Arm 2沒料
               BLCarryKit.UseSiteNoIC())
            {
                if(FTestSuck.HasRealIC()==false &&                              //Arm 1有料
                   FLCarryKit.HasRealIC()==false)
                {
                    FTestSuck.SetAllToNullIC();
                    FLCarryKit.SetAllToNullIC();
                    MOT[MInShuttle2].fCanMoveM=true;
                    return true;
                }
                else
                {
                    if(iOneCycle && IsInArmOneCycleFinish())                    //Sam 20250417 : 修正 AutoSiteOff 後 OneCycle Hangup 問題
                    {
                        return true;
                    }
                    else if(InSHT2InLF())                                       //等待
                    {
                        MOT[MInShuttle2].fCanMoveM=false;
                        return false;
                    }
                    else
                    {
                        return false;
                    }
                }
            }

            if((iOneCycle && IsInArmOneCycleFinish()) ||
               (iCleanOut && IsInArmCleanOutFinish()))
            {
                if(BTestSuck.UseSiteNoIC() &&
                   BLCarryKit.UseSiteNoIC() &&
                   InArmSuck.HasRealIC()==false)                                //JerryYang 20170801 (wei) 修正按one cycle hang up問題
                {
                    MOT[MInShuttle2].fCanMoveM=true;                            //RogerYang 20250820 : 1 --> 2
                    return true;
                }
            }

            if(iCleanOut          &&
               BLCarryKit.UseSiteNoIC()  &&
               InArmSuck.UseSiteNoIC()   &&
               MOT[MMPlate1].HasIC()==false &&
               MOT[MMPlate2].HasIC()==false)
            {
                return true;
            }

            if(CheckCFixTrayFullPlace() &&
               MOT[MTestZ2].Led[iHomeLed]==true)                                //Steven 20220524 : fixed for fix 3 氣缸 Hang up
            {
                MOT[MInShuttle2].fCanMoveM=true;
                return false;
            }

            if(BLCarryKit.UseSiteHasIC())
            {
                if(CanYieldAlarmRemainInSHT()==true)                            //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
                {
                    return true;
                }

                if(InSHT2InRT()==false)
                {
                    if((bUseTwoArm32Site==false && IndexStatus==Z1Down_Z2Up) ||
                       (bUseTwoArm32Site==true  && IndexStatus==Z1_Z2_Normal))
                        MOT[MInShuttle2].fCanMoveM=true;
                    return false;
                }

                if(bReadAndCheckCPUName==true &&
                   CosFunction.bInShuttleDetectByLatch &&                       //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
                   Prod.bF18InshuttleDetect &&
                   MOTION_CARD_TYPE==MotionCard_Contec &&
                   LastSet.iRealDummy==REALLY)                                  //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
                {
                    if(bInSht2LtcDetectTesterCanMove==false)
                    {
                        return false;
                    }
                }

                if((Prod.bF18InshuttleDetect ||                                 //Sam 20250925 : 修正關 Arm 後檢查 F18 功能被卡死 hangeup
                    IniConfig.bF20InShuttleProminentDetect) &&
                    CosFunction.bInShuttleDetectByLatch==false)
                {
                    if(bF18CheckShuttle2MustHasIC)
                        break;
                }

                if(In_Shuttle_Auto_Latch==eInSHAutoLtc &&
                   bInSh2DoLtc==true)                                           //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
                    return false;
                MOT[MInShuttle2].fCanMoveM=false;
                bSuckingFlagZ2=true;
                Task=300;
                bIndexZ2NeedUp=false;                                           //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
            }
            break;
        case 300:
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&                   //ChungHung 20120717 add Index Drop Error Can Retry and Start
               bShuttle2MoveToRight)                                            //ChungHung 20131015 fix hangup
            {
                if(InSHT2InRT()!=true)
                {
                    return false;
                }
            }
            bIndexPickUpErrMoveSht2=false;                                      //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
            MOT[MInShuttle2].fCanMoveM=false;
            bShuttle2MoveToRight=false;
            bShuttle2HasPickErr=false;                                          //Steven 20220712 : 避免In arm 偷放料
            if(CosFunction.bPurgeBeforePickShuttle)                             //JerryYang 20160906 PurgeBeforePickShuttle改成可以By客戶功能開啟
                InitRearTestPurgBeforePickShuttle();                            //ChungHung 20150517 add for ATK Try to Fix TSMC Device Die Crack Issue Start
            flag2=false;
            Task=301;
            if(IniConfig.bIndexPickupWait==true &&
               (LastSet.iTemperature==Tempture_Hot ||
                LastSet.iTemperature==Tempture_AmbientHot) &&
               Temperature.iInitialStart2Time!=0)                               //kevin 20180905 (Steven) : add  iInitialStart2Time
            {
                bInitialStart2Time=true;                                        //wei 20171020 (jou) InitialStart1 秒數倒數
            }

            if(IsNNMode()==NN_2Row)
                break;
        case 301:                                                                                                                                               //確認Z軸位置 位置在下->吸取IC->Z軸到位後->Task=310
            if(IniConfig.bIndexPickupWait==true &&
               (LastSet.iTemperature==Tempture_Hot ||                                                                                                           //jou 2012-06-29 Index Pick up need wait Soak Time
                LastSet.iTemperature==Tempture_AmbientHot) &&                                                                                                   //kevin 20180903 (Steven) : add 恆溫控制
               Temperature.iInitialStart2Time!=0)
            {
                dwEndShuttle2Soak=MyTickCount();                                                                                                                //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常  //kevin 20180905 add  iInitialStart2Time
                if(iInitialStart2Count>0)
                {
                    iInitialStart2Count=(Temperature.iInitialStart2Time)-((dwEndShuttle2Soak-dwStartShuttle2Soak)/1000);                                        //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)                                                                                            //Steven 20161213 : Fix iInitialStart2 關ARM Hangup
                {
                }
                else
                {
                    if(BLCarryKit.TSoakTime.Off()==false)
                        break;
                }
                bInitialStart2Time=false;                                                                                                                       //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
            }

            if(bNeedTesterDuckingFinishLightYellowAndAlarmOn)                                                                                                   //ChungHung 20141015 add for SCK When the handler start running (finished Start count function),Yellow light blinking & alarm on (press "alarm reset" then clear)
            {
                bNeedTesterDuckingFinishLightYellowAndAlarmOn=false;
                bTesterDuckingFinishLightYellowAndAlarmOn=true;
            }

            if(TestIF_File.bIndexPickICWhenOutShtNoIC)                                                                                                          //Steven 20221207 : Index必須在out shuttle沒料才可以吸
            {
                if(BRCarryKit.HasRealIC())
                {
                    return false;
                }
            }

            if(TestIF.iShuttleMode==1 &&                                                                                                                        //jou 980317 SOFT_SIMULTE hang
               TestIF.iShuttle_Sel!=0 &&
               bCheckShuttle2Flag)
                return false;
            MOT[MInShuttle2].ScanMotorStatus();
            if(BLCarryKit.UseSiteHasIC() && bCheckShuttle2Flag==false)
            {
                flag2=false;
                if(CheckZ1IsDown())
                {
                    if(BLCarryKit.HasRealIC()==false)
                    {
                        flag2=true;
                    }
                    else
                    {
                        ZPos=MOT[MTestZ1].Gali_ReadPos();
                        //==> Eastsun 20260511 F007 整合: Ifor 20240430 add:secs gem cmd Index Yiel Fail
                        if(CUSTOMER_CODE==CC_KYEC_LEE && iSECSGEM_ConsecutiveFailureAlarm==2)
                        {
                        }
                        else if(CosFunction.bSortingBy2DList==true &&           //jou Index更換Offset會hang up? 2010-04-13
                           LastSet.iTester==_2D_SORT &&
                           TestIF_File.bSortingBy2DIDList==true)                                                                                                //Frank 20221122 : 2DID sorting for ATK
                        //<== Eastsun 20260511 F007 整合
                        {
                        }
                        else
                        {
                            if(bUseTwoArm32Site==false)
                            {
                                if(ZPos>=(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+500))                                                                        //2008/06/24
                                {
                                    if(ZPos!=iBackUpZ1DownPosition)
                                        return false;
                                }
                            }
                        }

                        if(MOT[MTestZ2].Gali_ReadPos()<(Prod.TestZ2_Pick+1000))                                                                                 //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                        {
                            bZ2PickShuttle=true;
                        }

                        if(flag2==false)
                        {
                            bSuckingFlagZ2=true;                                                                                                                //Steven 20240916 : index下降到shuttle吸放料
                            sp=GetIndexZSpeed(1);                                                                                                               //Steven 20160524 : Index Z軸速度整合為Function
                            if(CosFunction.bPurgeBeforePickShuttle)                                                                                             //JerryYang 20160906 PurgeBeforePickShuttle改成可以By客戶功能開啟
                            {
                                if(DeviceForm_File.bPurgeBeforePickShuttle)                                                                                     //ChungHung 20150517 add for ATK Try to Fix TSMC Device Die Crack Issue
                                {
                                    if(DoRearTestPurgBeforePickShuttle(sp)==false)
                                        break;
                                }
                            }
                            #ifdef DEBUG_INDEX_UPH
                            flag2=MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Pick, iIndexSpeed, iIndexAcc);                                                         //Steven 20170818 : 測試UPH用
                            #else
                            flag2=MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Pick, sp);
                            #endif
                        }
                    }
                }
            }
            else if(CosFunction.bIndexPickErrSkipNeedCheckVac &&                                                                                                //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
                    IniConfig.bD50IndexPickErrSkipNeedCheckVac &&
                    bArm2PressSkipNeedDownCheckVac==true &&
                    BLCarryKit.UseSiteNoIC())
            {
                flag2=false;
                if(CheckZ1IsDown())
                {
                    ZPos=MOT[MTestZ1].Gali_ReadPos();
                    //==> Eastsun 20260511 F007 整合: Ifor 20240430 add:secs gem cmd Index Yiel Fail
                    if(CUSTOMER_CODE==CC_KYEC_LEE && iSECSGEM_ConsecutiveFailureAlarm==2)
                    {
                    }
                    else if(IsNNMode()!=NN_2Row)                                                                                                                     //jou Index更換Offset會hang up? 2010-04-13
                    //<== Eastsun 20260511 F007 整合
                    {
                        if(ZPos>=(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+500))                                                                                //2008/06/24
                        {
                            if(ZPos!=iBackUpZ1DownPosition)
                                return false;
                        }
                    }

                    if(MOT[MTestZ2].Gali_ReadPos()<(Prod.TestZ2_Pick+1000))                                                                                     //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                    {
                        bZ2PickShuttle=true;
                    }

                    if(flag2==false)
                    {
                        bSuckingFlagZ2=true;                                                                                                                    //Steven 20240916 : index下降到shuttle吸放料
                        if(CUSTOMER_CODE==CC_AMKOR_Philippines)                                                                                                 //Frank QQ
                        {
                            flag2=true;
                        }
                        else
                        {
                            #ifdef DEBUG_INDEX_UPH
                            flag2=MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Pick, iIndexSpeed, iIndexAcc);                                                         //Steven 20170818 : 測試UPH用
                            #else
                            flag2=MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Pick, iSpeedSlow);                                                                      //JerryYang 20170610 這裡改慢速
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

            if(LastSet.iRealDummy==REALLY && ArmSpeed[IndexArm].bSuckOnDown && INDEX_SUCKER_TYPE==0)
            {
                for(int i=0; i<BLCarryKit.iShtRow; i++)
                {
                    for(int j=0; j<BLCarryKit.iShtCol; j++)
                    {
                        if(BLCarryKit.Item[i][j]!=NULL_IC &&
                           BLCarryKit.Item[i][j]!=HAS_NULL_IC)
                        {
                            BTestSuck.Suck[i][j].On();
                        }
                    }
                }
            }

            if(flag2)
            {
                RecordIndexPosition(2, 0);                                                                                                                      //Isaac 20200922 : 紀錄indexArmY encoder值和command值，Arm2/Shuttle
                EncoderTeachingMaxMinCount(2);                                                                                                                  //Isaac 20201012 : 每次完成動作，比較紀錄Encoder和Teaching點的差值
                flag2=false;                                                                                                                                    //JerryYang 20170610 (wei) 移到上面
                bZ2PickShuttle=true;                                                                                                                            //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                if(CosFunction.bIndexPickErrSkipNeedCheckVac &&
                   IniConfig.bD50IndexPickErrSkipNeedCheckVac &&
                   bArm2PressSkipNeedDownCheckVac==true &&
                   BLCarryKit.UseSiteNoIC())                                                                                                                    //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(bSkipNeedCheckVac[1][i][j]==true)
                            {
                                bArm2SuckFinish[i][j]=false;                                                                                                    //Steven 20110301 : 初始化，都當作還沒做完
                                bArm2DuplicateErr[i][j]=false;
                                BTestSuck.Suck[i][j].Reset();                                                                                                   //Steven 20140213 : Jordan說Index下去不吸直接Alarm
                            }
                        }
                    }
                    Task=307;                                                                                                                                   //JerryYang 20170610 (wei) 這裡跳去檢查按skip的真空
                    break;
                }
                ResetInxedArm2Flag();                                                                                                                           //Steven 20241017 : 打包成function
                if(CosFunction.bUseShuttlePickShiftDetect==true &&
                   DeviceForm_File.dDropByPassDetect!=0)                                                                                                        //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
                {
                    if(TestIF_File.iShuttleMode==0 ||                                                                                                           //Ifor 20221013 add:關Arm不處理避免資料轉移異常
                       (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1))
                    {
                        bIndexZ2NeedUp=true;
                    }
                    else
                    {
                        bIndexZ2NeedUp=false;
                    }
                }
                else
                {
                    bIndexZ2NeedUp=false;
                }

                if(USE_IO_CHANGE_TOQUE==true &&                                                                                                                 //jou 2012-06-12 吸shuttle時，需檢測Torque，過大需alarm
                   (bNeedCheckIndexToque==true ||
                    bNeedCheckIndexToque2==true))                                                                                                               //jou 2012-06-12 即時更新扭力值不能開，不然會衝突
                {
                    bNeedCheckIndexToque=false;
                    bNeedCheckIndexToque2=false;
                    W64bT2_FMAIN_CHKREADTORQUE1->Checked=false;
                    W64bT2_FMAIN_CHKREADTORQUE2->Checked=true;
                    W64bT2_FMAIN_EDTORUE1->Text="";
                    bOverHappen=false;
                    hDoRearTestSuckIC.SetSecAndOn(1);
                }
                bHasDropAtShuttle=false;
                if(IniConfig.bD81IndexCheckVacuumOnShuttle)
                {
                    for(int i=0; i<MAX_Index_Row; i++)                                                                                                          //JerryYang 20241002 : add
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(BLCarryKit.Item[i][j]==NULL_IC || BLCarryKit.Item[i][j]==HAS_NULL_IC)
                            {
                                if(INDEX_SUCKER_TYPE==1 && bDropAtSht2NeedCheckVac[i][j]==true)                                                                 //Steven 20111202
                                {
                                    fiosetview->bIndexSuck[1][i][j]=true;
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
            bArm2SuckComplete=true;
            ErrPart="";
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bSkipNeedCheckVac[1][i][j] &&
                       BLCarryKit.Item[i][j]==NULL_IC)
                    {
                        if(BLCarryKit.Item[i][j]==NULL_IC &&
                           bArm2SuckFinish[i][j]==true)                         //Steven 20110301
                        {
                            if(BTestSuck.Item[i][j]==HAS_NULL_IC ||
                               BTestSuck.Item[i][j]==NULL_IC)
                            {
                                BTestSuck.Suck[i][j].Normal();                  //Steven 20111201 : 預防負壓壓降
                            }
                        }
                        else
                        {
                            if(BTestSuck.Item[i][j]==HAS_NULL_IC &&
                               BTestSuck.Suck[i][j].Suck())
                            {
                                bHasErr=true;                                   //按skip後卻吸到IC，要跳alarm
                                bArm2DuplicateErr[i][j]=true;
                                bArm2SuckFinish[i][j]=true;
                            }
                            else if(BTestSuck.Suck[i][j].Error)                 //Steven 20110301 : 有錯誤的不做
                            {
                                bArm2SuckFinish[i][j]=true;
                            }
                            else if(BTestSuck.Item[i][j]==HAS_IC &&
                                    BTestSuck.Suck[i][j].Suck())                //Ifor 20171122 (Steven) :add 修正bArmSuckFinish 資料異常造成Hangup
                            {
                                bArm2SuckFinish[i][j]=true;
                            }
                            else
                            {
                                bArm2SuckComplete=false;                        //jou 2011-08-16 只要有未完成的就繼續等
                            }
//                            else
//                                flag1=false;                                  //jou 2011-08-16 只要有未完成的就繼續等
                        }
                    }
                    else
                    {
                        if(BTestSuck.Item[i][j]==HAS_NULL_IC ||
                           BTestSuck.Item[i][j]==NULL_IC)
                            BTestSuck.Suck[i][j].Normal();                      //Steven 20111201 : 預防負壓壓降
                        bArm2SuckFinish[i][j]=true;                             //Steven 20110301 : 沒有東西的地方要跳過
                    }
                }
            }
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bArm2SuckFinish[i][j]==false)                            //只要有未完成的就繼續等
                        bArm2SuckComplete=false;
                }
            }

            if(bArm2SuckComplete==true)                                         //Steven 20110301 : 所有吸嘴都做完
            {
                bArm2SuckComplete=false;
                if(bHasErr)
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(bSkipNeedCheckVac[1][i][j]==true &&
                               bArm2DuplicateErr[i][j]==true)
                                BTestSuck.Suck[i][j].Normal();
                        }
                    }
                    Task=308;
                    return false;
                }

                if(BLCarryKit.HasRealIC())
                    break;
                ZeroMemory(bArm2DuplicateErr, sizeof(bArm2DuplicateErr));
                if(BLCarryKit.UseSiteHasIC())
                    break;
                iD43AutoRetryWhenIndexPickErrCnt[0]=0;
                bResetIndexArm1Pick=false;
                bArm2PressSkipNeedDownCheckVac=false;
                ResetArm2VacFlag();                                             //Steven 20241017 : 打包成function
                Task=500;
            }
            break;
        case 308:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, iSpeedSlow))         //JerryYang 20180628 (wei) 只有pick up error的arm要上升
            {
                bSuckingFlagZ2=false;                                           //Steven 20240916 : index下降到shuttle吸放料
                Task=309;
            }
            break;
        case 309:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                bArm2PressSkipNeedDownCheckVac=false;
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(bSkipNeedCheckVac[1][i][j]==true)
                            ErrPart+=IndexSuckName[i][j];
                        if(CUSTOMER_CODE!=CC_AMKOR_Philippines)                 //Frank QQ
                            bSkipNeedCheckVac[1][i][j]=false;
                    }
                }

                if(CUSTOMER_CODE==CC_AMKOR_Philippines)                         //Frank QQ
                {
                    bArm2PressSkipNeedDownCheckVac=true;
                    str.sprintf("Arm2 detect %s has device",ErrPart);
                    ShowMyMessage(str,"請檢查Arm2上是否有IC");
                    ErrPart="";
                    Task=3091;
                }
                else
                {
                    str.sprintf("Arm2 detect Shuttle %s has device",ErrPart);
                    ShowMyMessage(str,"請檢查shutle2上是否有IC");
                    ErrPart="";
                    Task=500;
                }
            }
            break;
        case 3091:
            ResetInxedArm2Flag();                                               //Steven 20241017 : 打包成function
            Task=307;
            break;
        case 3099:                                                              //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
            bSuckingFlagZ2=true;                                                //Steven 20240916 : index下降到shuttle吸放料
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Pick+DeviceForm_File.dDropByPassDetect*100, iSpeedSlow))
            {
                ResetInxedArm2Flag();                                           //Steven 20241017 : 打包成function
                Task=310;
            }
            break;
        case 310:                                                                                                       //jou 2012-06-12 吸shuttle時，需檢測Torque，過大需alarm
            if(USE_IO_CHANGE_TOQUE==true && bOverHappen==false)                                                         //jou 2012-06-12 即時更新扭力值不能開，不然會衝突
            {
                if(W64bT2_FMAIN_EDTORUE1->Text!="")
                {
                    if(atoi(W64bT2_FMAIN_EDTORUE1->Text.c_str())>=ciIndex5FSetValue)
                    {
                        ShowMyMessage("Index Z2 Torque Over,Z2 Pick from Shuttle need up","Index Z2 Torque 過大,需往上調整");
                    }
                    bOverHappen=true;
                }
                else if(hDoRearTestSuckIC.Off()==false)
                {
                    break;                                                                                              //Wait Read Torque
                }
                else
                {
                    bOverHappen=true;                                                                                   //Over Wait Time
                }
            }
            bArm2NeedSuck=true;                                                                                         //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
            bArm2SuckComplete=true;
            DoArm2Suck();                                                                                               //JerryYang 20190123 把index arm吸真空&交換狀態包成函式
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bArm2SuckFinish[i][j]==false)                                                                    //只要有未完成的就繼續等
                        bArm2SuckComplete=false;
                }
            }

            if(bArm2SuckComplete==true)                                                                                 //Steven 20110301 : 所有吸嘴都做完
            {
                if(bIndexZ2NeedUp)                                                                                      //Ifor 20220906 add:ASEM要求Shuttle 吸料後上升設定高度再判斷是否有無吸到IC
                {
                    bIndexZ2NeedUp=false;
                    Task=3099;
                    break;
                }
                BTestSuck.HotCount=BLCarryKit.HotCount;                                                                 //JerryYang 20230204 : add hot plate放置順序資料
                if(IniConfig.bD81IndexCheckVacuumOnShuttle && bHasDropAtShuttle==true)                                  //JerryYang 20241002 : add
                {
                    if(INDEX_SUCKER_TYPE==1)
                    {
                        if(W64bT2_ProcessIndexSuckDestroy2()==true) // AI(W5-aTester_Rear-Translate) 20260710: golden :1485 fiosetview->ProcessIndexSuckDestroy2() -- reuses Wave-1 gap stub (see file-top)
                        {
                            bHasICErr=false;
                            ErrPart2="";
                            for(int i=0; i<MAX_Index_Row; i++)
                            {
                                for(int j=0; j<NEW_MAX_Index_Col; j++)
                                {
                                    #ifdef SOFT_SIMULTE
                                    if((BTestSuck.Item[i][j]==NULL_IC || BTestSuck.Item[i][j]==HAS_NULL_IC) && i==0 && j==3)
                                    #else
                                    if((BTestSuck.Item[i][j]==NULL_IC || BTestSuck.Item[i][j]==HAS_NULL_IC) && BTestSuck.Suck[i][j].GetStatus()==true)
                                    #endif
                                    {
                                        bHasICErr=true;
                                        bSHT2_DropPosHasIC[i][j]=true;
                                        if(TestIF_File.iTestMode==_32Site4X8M ||
                                           TestIF_File.iTestMode==_32Site4X8N ||
                                           TestIF_File.iTestMode==_16Site4X4)                                           //Sam 20190226 : 16Site4X4 ///kevin 20180504 add  error pos
                                            ErrPart2+=IndexSuckName[i+2][j];
                                        else if(TestIF_File.iTestMode==QualSite2X2N ||
                                                TestIF_File.iTestMode==_6Site2X3N)
                                            ErrPart2+=IndexSuckName[i+1][j];                                            //Steven 20230712 : 修正NN mode alarm顯示
                                        else
                                            ErrPart2+=IndexSuckName[i][j];
                                        BTestSuck.Suck[i][j].Normal();
                                    }
                                }
                            }

                            if(bHasICErr==false)
                            {
                                for(int i=0; i<MAX_Index_Row; i++)
                                {
                                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                                    {
                                        bDropAtSht2NeedCheckVac[i][j]=false;
                                        bSHT2_DropPosHasIC[i][j]=false;
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
                bHasErr=false;
                //──── DEBUG: 強制觸發 JAM0302 Rear pick-up error ────
//                static bool bDebugForceJAM0302Rear = true;   // ← 設 true 重現，false 關閉
//                if(bDebugForceJAM0302Rear)
//                {
//                    bHasErr = true;
//                    BTestSuck.Suck[0][0].Error = true;
//                    bDebugForceJAM0302Rear = false;           // 只觸發一次
//                }
                //──── DEBUG END ────
                bArm2NeedSuck=false;
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BTestSuck.Suck[i][j].Error)
                        {
                            bHasErr=true;
                            if(USE_IO_CHANGE_TOQUE==true)                                                               //jou 2012-06-12 即時更新扭力值不能開，不然會衝突
                            {                                                                                           //jou 2012-06-12 吸shuttle時，需檢測Torque，過大需alarm
                                W64bT2_FMAIN_CHKREADTORQUE1->Checked=false;
                                W64bT2_FMAIN_CHKREADTORQUE2->Checked=true;
                                W64bT2_FMAIN_EDTORUE1->Text="";
                                bOverHappen=false;
                                hDoRearTestSuckIC.SetSecAndOn(1);
                            }

                            if(IniConfig.bD62PickUpErrorNeedPurge)                                                      //Steveb 20161024 : 吸取異常需要吹氣一次
                            {
                                BTestSuck.Suck[i][j].Off();
                            }
                        }
                    }
                }

                if(bHasErr || (IniConfig.bD81IndexCheckVacuumOnShuttle && bHasICErr))
                {
                    bShuttle2HasPickErr=true;                                                                           //Steven 20230118 : 避免In arm 偷放料, 往上移動
                    bIndexPickUpErrMoveSht2=true;                                                                       //Steven 20221107 : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
                    if(CosFunction.bIndexPickErrSkipBlowAirTime)                                                        //Frank QQ
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

                if(BLCarryKit.HasRealIC())
                    break;
                if(TestIF_File.iShuttleMode==0 ||
                   (TestIF_File.iShuttleMode==1 &&
                    TestIF_File.iShuttle_Sel==1))                                                                       //Jou 20230224 : 修正Auto Site Map關arm異常
                {
                    if(bAutoSiteMapWaitTestResult==true &&
                       BTestSuck.HasRealIC()==false &&
                       FTestSuck.HasRealIC()==false &&                                                                  //Steven 20200326 : 修正JCET Auto site map發生inarm掉料會hang up
                       TestSocket.HasRealIC()==false &&
                       FLCarryKit.HasRealIC()==false &&
                       BLCarryKit.HasRealIC()==false)
                    {
                        if(IsNNMode()==NN_2Row)
                        {
                        }
                        else
                        {
                            bAutoSiteMapWaitTestResult=false;
                        }
                    }
                }
                ZeroMemory(bArm2DuplicateErr, sizeof(bArm2DuplicateErr));
                if(BLCarryKit.UseSiteHasIC())
                    break;
                bResetIndexArm2Pick=false;
                if(CUSTOMER_CODE==CC_TSMC_TAINAN &&
                   (Prod.bWhenNoFullSiteUseInitialDelay &&                                                              //wei 20161102 No FullSite delay修改||->&&
                    IniConfig.bL18NofullsiteaddTemperatureoffset))                                                      //wei 20151228 No FullSite delay
                {
                    CheckBTFullSite();
                }
                Task=500;
                if(DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==true &&
                   LastSet.iTemperature==Tempture_Hot &&                                                                //JerryYang 20220805 : 下壓shuttle預熱功能initial第一次吸shuttle時需下壓等待Jam soak time
                   FTestSuck.HasRealIC()==false)
                {
                    hDoRearTestSuckICdelay.SetSecAndOn(Temperature.fJamSoakTime);
                }
                iD43AutoRetryWhenIndexPickErrCnt[1]=0;                                                                  //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
            }
            break;
        case 3051:                                                              //Frank QQ
            bSuckingFlagZ2=true;                                                //Steven 20240916 : index下降到shuttle吸放料
            if(MOT[MTestZ2].Gali_MotMoveNoWait(Prod.TestZ2_Place, MOT[MTestZ2].GailSpeed, 0))
            {
                hDoRearTestSuckIC.SetSecAndOn(IniConfig.iD62IndexBlowAirTime);
                Task=3052;
            }
            break;
        case 3052:                                                              //Frank QQ
            if(hDoRearTestSuckIC.Off())
            {
                Task=305;
            }
            break;
        case 305:
            if(USE_IO_CHANGE_TOQUE==true &&                                     //jou 2012-06-12 即時更新扭力值不能開，不然會衝突
               bOverHappen==false)                                              //jou 2012-06-12 吸shuttle時，需檢測Torque，過大需alarm
            {
                if(W64bT2_FMAIN_EDTORUE1->Text!="")
                {
                    if(atoi(W64bT2_FMAIN_EDTORUE1->Text.c_str())>=ciIndex5FSetValue)
                    {
                        ShowMyMessage("Index Z2 Torque Over,Z2 Pick from Shuttle need up","Index Z2 Torque 過大,需往上調整");
                    }
                    bOverHappen=true;
                }
                else if(hDoRearTestSuckIC.Off()==false)
                {
                    break;                                                      //Wait Read Torque
                }
                else
                {
                    bOverHappen=true;                                           //Over Wait Time
                }
            }
            CheckIndexAllSuckICFallDown(false, true);                           //Steven 20110725 : 修改負壓檢查方式
            if(MOT[MTestZ2].Gali_ReadPos()>(Prod.TestZ2_Pick+1000))             //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            {
                bZ2PickShuttle=false;
            }
            #ifdef DEBUG_INDEX_UPH
            if(MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Safe, iIndexSpeed, iIndexAcc))
            #else
            if(MOT[MTestZ2].Gali_MotMoveNoWait(Prod.TestZ2_Safe, MOT[MTestZ2].GailSpeed, 0))
            #endif
            {
                bSuckingFlagZ2=false;                                           //Steven 20240916 : index下降到shuttle吸放料
                Task=306;                                                       //Steven 20160718 : Index pick up error with [D43]
            }
            break;
        case 306:
            bZ2PickShuttle=false;                                               //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip)
            {
                if(BRCarryKit.UseSiteNoIC())                                    //ChungHung 20120717 add Index Drop Error Can Retry and Start
                {
                    MOT[MInShuttle2].fCanMoveM=true;
                    bShuttle2MoveToLeft=true;
                    bShuttle2HasPickErr=true;                                   //Steven 20220712 : 避免In arm 偷放料
                    bIndexPickErrShtStayRight2=false;                           //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
                    bCheckNullIC2=false;                                        //JerryYang 20170623 (wei) 修正有裝out shuttle 前後對照的機台發生index arm吸取異常無法跳出alram造成hang up
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
               bShuttle2MoveToLeft)                                             //ChungHung 20131015 fix hangup
            {
                if(InSHT2InLF()!=true)
                {
                    return false;
                }
            }
            bShuttle2MoveToLeft=false;
            MOT[MInShuttle2].fCanMoveM=false;
            Task=321;
            if(IsNNMode()==NN_2Row)
                break;
        case 321:                                                                                                       //ChungHung 20130924 add
            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            bSht1OnlyHasICErr=false;
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(bArm2DuplicateErr[i][j])
                        bHasDuplicateErr=true;
                    if(BTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i][j];
                        if(IniConfig.bD62PickUpErrorNeedPurge)                                                          //Steveb 20161024 : 吸取異常需要吹氣一次
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }
#if 0 // TODO(W7) -- golden :1730 BLCarryKit.PordRec[i][j].AddErrorRecordNoSave("JAM0302") (TMyProductionRecord has no .cpp yet -- same gap as Wave-1's AddIndexCycleTimeRecord/AddIndexPlaceShuttleRecord)
                        BLCarryKit.PordRec[i][j].AddErrorRecordNoSave("JAM0302");
#endif
                    }
                    else
                    {
                        BTestSuck.Suck[i][j].Error=false;
                    }
                }
            }

            if(bHasErr)
            {
                if(IndexAlarmInArmAway()==false)                                                                        //Steven 20130613 : Index異常時, In Arm要先讓位功能
                {
                    return false;
                }
                bHasErr=false;
                if(IniConfig.bD81IndexCheckVacuumOnShuttle && bHasICErr==true)
                {
                    bHasICErr=false;
                    str.sprintf("Need to remove device of Shuttle2 %s", ErrPart2);
                    ShowMyMessage(str);
                }

                if(IniConfig.bD43AutoRetryWhenIndexPickErr &&                                                           //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
                   iD43AutoRetryWhenIndexPickErrCnt[1]==0)
                {
                    ret=K_RETRY;
                }
                else if(IniConfig.bNewResetFunction==true &&
                        bResetIndexArm2Pick==true)
                {
                    ret=K_SKIP;
                }
                else
                {
                    if(CosFunction.bJAM0301NeedOpenChamberDoor)                                                         //wei : JAM0301 & JAM0302需要開啟Chamber門10秒
                    {
                        bIsTestSitICFallDown=true;
                    }

                    if(IniConfig.bIndexPickErrOnlySKIP==true ||                                                         //jou 2012-02-13 index pick-up error only skip
                       IniConfig.bD64IndexPickErrOnlySKIP)                                                              //kevin 20171103 (wei) add retry function
                        ret=ShowErrorMessage("JAM0302", K_SKIP, MTestZ2, bHasDuplicateErr, ErrPart);                    //Devicr Pick-Up Error
                    else
                        ret=ShowErrorMessage("JAM0302", K_SKIP|K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);            //Devicr Pick-Up Error
                }

                if(ret==K_SKIP)
                {
                    if(LastSet.iRunStartMode==rsmAutoSiteMap)
                        iAutoSiteMapCount++;                                                                            //Steven 20220811 : 紀錄目前是哪個Shuttle
                    bAutoSiteMapWaitTestResult=false;                                                                   //Ifor 20180115 (Steven) : add Site Mapping SKIP 需清除旗標
                    bIndexPickUpErrMoveSht2=false;                                                                      //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
                    iD43AutoRetryWhenIndexPickErrCnt[1]=0;                                                              //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(BTestSuck.Item[i][j]==NULL_IC)                                                           //jou 2011-12-27 有發生過Skip又重吸一次,改成下面的方式
                            {
                                if(BTestSuck.Suck[i][j].Error)                                                          //JerryYang 20200303 fix 沒IC的地方jam count被++
                                {
                                    if(CosFunction.bUseSCKART)                                                          //Steven 20161214 (wei) : For SCK ART
                                    {
                                        if(TestIF_File.bRENESAS_EnableFTCT==true)                                       //RogeryYang 20251014 : FTCT add to HdRejectBT
                                            fSCKART->iInputJamCnt++;
                                        else
                                            fSCKART->AddOutputJamCnt(i, j, ret);                                        //RogerYang 20250923 : 整合ART OutArm JamCount
                                    }
                                }
#if 0 // TODO(W7) -- golden :1800 BLCarryKit.PordRec[i][j].AddErrorRecord("JAM0302") (TMyProductionRecord has no .cpp yet)  //Steven 20161214 : 加上Index異常Skip的ErrorLog
                                BLCarryKit.PordRec[i][j].AddErrorRecord("JAM0302");                                     //Steven 20161214 : 加上Index異常Skip的ErrorLog
#endif
                                BTestSuck.SetItemData(i, j, HAS_NULL_IC);
                                BTestSuck.Suck[i][j].Normal();
                                if(CosFunction.bIndexPickErrSkipNeedCheckVac==true &&
                                   IniConfig.bD50IndexPickErrSkipNeedCheckVac)                                          //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
                                {
                                    bSkipNeedCheckVac[1][i][j]=true;
                                    bArm2PressSkipNeedDownCheckVac=true;
                                }
                            }
                            BLCarryKit.SetItemData(i, j, NULL_IC);
                            bArm2DuplicateErr[i][j]=false;
                        }
                    }

                    if(IniConfig.bNewResetFunction==true &&
                       bResetIndexArm2Pick==true)
                    {
                    }
                    else
                    {
                        if(IniConfig.bD42IndexPickICShuttlePause)
                        {
                            bInArmNeedToSafePos=true;
                            bShuttle2Pause=true;
                            bIndexArm2PickupErrStop=true;                                                               //jou 2012-02-29 index pick up error,index arm move to center & alarm
                            bShowShuttle2Device=true;                                                                   //kevin 20180504 index pick up error
                        }
                    }
                    bIndexPickUpErrorWaitRetry=false;                                                                   //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                    if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                     //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
                    {
                        bIndexPickErrShtStayRight2=false;
                    }

                    if(IniConfig.bD43IndexPickErrCheckSocket==true)                                                     //Steven 20190115 : SCC要求吸取異常要檢查Socket
                    {
                        bIndexArm2PickUpErrNeedPiggyback=true;
                        if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                                            //Sam 20230108 : 矽格俊堯要求不要 Reset IC
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
                    if(IniConfig.bD43AutoRetryWhenIndexPickErr &&                                                       //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
                       iD43AutoRetryWhenIndexPickErrCnt[1]==0)
                    {
                        iD43AutoRetryWhenIndexPickErrCnt[1]++;
                    }
                    else
                    {
                        iD43AutoRetryWhenIndexPickErrCnt[1]=0;                                                          //Steven 20170105 : Index吸取異常,要退出來用Shuttle Sensor檢查後, 再進去吸一次
                        for(int i=0; i<BTestSuck.iShtRow; i++)
                            for(int j=0; j<BTestSuck.iShtCol; j++)
                                if(BTestSuck.Suck[i][j].Error)
                                    bArm2DuplicateErr[i][j]=true;
                    }
                    bIndexPickUpErrorWaitRetry=true;                                                                    //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                    if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                     //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
                    {
                        bIndexPickErrShtStayRight2=true;
                    }
                    bIndexPickUpErrorWaitRetry=true;                                                                    //Ifor 20171119 : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
                }
                BTestSuck.ResetAll();                                                                                   //Steven 20160323 : 避免未開啟真空
            }
            else if(IniConfig.bD81IndexCheckVacuumOnShuttle && bHasICErr==true)
            {
                if(IndexAlarmInArmAway()==false)                                                                        //Steven 20130613 : Index異常時, In Arm要先讓位功能
                {
                    return false;
                }
                bHasICErr=false;
                bSht2OnlyHasICErr=true;
                str.sprintf("Need to remove device of Shuttle2 %s", ErrPart2);
                ShowMyMessage(str);
                if(IniConfig.bD42IndexPickICShuttlePause)
                {
                    bInArmNeedToSafePos=true;
                    bShuttle2Pause=true;
                    bIndexArm2PickupErrStop=true;                                                                       //jou 2012-02-29 index pick up error,index arm move to center & alarm
                    bShowShuttle2Device=true;                                                                           //kevin 20180504 index pick up error
                }
            }

            if(bShuttle2Pause)
                MOT[MInShuttle2].SetSpeed(10);                                                                          //kevin 20180226 (Steven) add pick up error  shuttle down speed
            if(IniConfig.bNewResetFunction==true && bResetIndexArm2Pick==true)
            {
                bResetIndexArm2Pick=false;
            }
            else
            {
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                         //ChungHung 20120717 add Index Drop Error Can Retry and Start
                {
                    MOT[MInShuttle2].fCanMoveM=true;                                                                    //等待Shuttle 移至右邊
                    bShuttle2MoveToRight=true;
                    bShuttle2HasPickErr=true;                                                                           //Steven 20220712 : 避免In arm 偷放料
                }
            }

            if(IniConfig.bD42IndexPickICShuttlePause &&                                                                 //AI(staterecord-analysis) 20260417 (RogerYang) : D42 SKIP後強制走500, 避免300讓Z2再次下降造成四方死鎖
               bShuttle2Pause)
            {
                Task=500;
            }
            else if(BLCarryKit.HasRealIC())
            {
                Task=300;
            }
            else if(CosFunction.bIndexPickErrSkipNeedCheckVac==true &&
                    IniConfig.bD50IndexPickErrSkipNeedCheckVac &&
                    bArm2PressSkipNeedDownCheckVac==true)                                                               //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
            {
                Task=300;
            }
            else
            {
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        if(BLCarryKit.Item[i][j])
                        {
                            if(BLCarryKit.Item[i][j]==HAS_NULL_IC &&
                               BTestSuck.Item[i][j]!=NULL_IC)
                            {
                                BLCarryKit.SetItemData(i, j, NULL_IC);
                            }

                            if(BLCarryKit.Item[i][j]==HAS_NULL_IC)
                            {
                                BTestSuck.Suck[i][j].Normal();
                                W64bT2_MoveSuckData(BTestSuck, BLCarryKit, i, j);                                       // AI(W5-aTester_Rear-Translate) 20260710: golden :1940 BTestSuck.MoveSuckData(BLCarryKit,i,j) -- gap stub (TMyKitSuck::MoveSuckData absent from aHotPlateSubstrate.h; reuses Wave-1's W64bT2_MoveSuckData(dst,src,i,j))
                                bArm2DuplicateErr[i][j]=false;
                            }
                        }
                    }
                }

                if(BLCarryKit.UseSiteHasIC())
                {
                    Task=300;
                    break;
                }
                Task=500;
            }
            break;
        case 500:
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&                   //ChungHung 20120717 add Index Drop Error Can Retry and Start
               bShuttle2MoveToRight)                                            //ChungHung 20131015 fix hangup
            {
                if(InSHT2InRT()!=true)
                {
                    return false;
                }
            }
            CheckShuttle2EncoderPos();                                          //JerryYang 20230131 : add shuttle shift log
            MOT[MInShuttle2].fCanMoveM=false;
            bShuttle2MoveToRight=false;
            bShuttle2HasPickErr=false;                                          //Steven 20220712 : 避免In arm 偷放料
            bIndexPickUpErrorWaitRetry=false;                                   //Ifor 20171119 (Steven) : add 避免Index Pick Up Err Inarm 偷跑造成資料異常導致Hangup
            if(DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==true &&
               LastSet.iTemperature==Tempture_Hot)                              //Ifor 20180606 (wei) : add Index 再Shuttle 上等待測試結果
            {
                if(fFrontNeedTest==true && bD52IndexArmUp==false)               //JerryYang 20200812 : fix RTC error hang up
                    break;
                if(FTestSuck.HasRealIC()==false)                                //JerryYang 20220805 : 下壓shuttle預熱功能initial第一次吸shuttle時需下壓等待Jam soak time
                {
                    if(hDoRearTestSuckICdelay.Off()==false)
                        break;
                }
            }
            Task=501;
            iHomeAlarm=0;
            if(IsNNMode()==NN_2Row)
                break;
        case 501:                                                                                                       //ChungHung 20130924 add
            if(MOT[MTestZ2].Gali_ReadPos()>(Prod.TestZ2_Pick+1000))                                                     //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            {
                bZ2PickShuttle=false;
            }
            sp=GetIndexZSpeed(1);                                                                                       //Steven 20160524 : Index Z軸速度整合為Function
            #ifdef DEBUG_INDEX_UPH
            flag1=MOT[MTestZ2].Gali_MotMove2(Prod.TestZ2_Safe, iIndexSpeed, iIndexAcc);
            #else
            flag1=MOT[MTestZ2].Gali_MotMoveNoWait(Prod.TestZ2_Safe, sp, 0);
            #endif
            if(flag1)
            {
                bSuckingFlagZ2=true;                                                                                    //Steven 20240916 : index下降到shuttle吸放料
                #ifndef SOFT_SIMULTE
                if(TestIF_File.iShuttleMode==0 ||                                                                       //Ifor 20160303 新增馬達回Home保護機制
                   (TestIF_File.iShuttleMode==1 &&
                    TestIF_File.iShuttle_Sel==1))
                {
                    MOT[MTestZ2].ScanMotorStatus();
                    if(MOT[MTestZ2].Led[iHomeLed]==false)
                    {
                        if(iHomeAlarm>10)
                        {
                            ShowMyMessage("Motor Z2 not at the origin!!", "馬達Z2不在原點上!");
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
                bZ2PickShuttle=false;                                                                                   //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip)                                                         //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
                {
                    bIndexPickErrShtStayRight2=false;
                }
                #ifndef SOFT_SIMULTE
                {
                    if(LastSet.iRealDummy==REALLY)                                                                      //Steven 20110131 Start
                    {
                        ErrPart=" ";
                        bHasErr=false;
                        bHasDuplicateErr=false;
                        for(int i=0; i<BTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<BTestSuck.iShtCol; j++)
                            {
                                if(bArm2DuplicateErr[i][j])
                                    bHasDuplicateErr=true;
                                if(BTestSuck.Item[i][j]!=NULL_IC &&
                                   BTestSuck.Item[i][j]!=HAS_NULL_IC &&
                                   BTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    bHasErr=true;
                                    BTestSuck.Suck[i][j].Normal();                                                      //Steven 20110707 : 預防負壓掉料
                                    BTestSuck.Suck[i][j].Error=true;
                                    ErrPart+=IndexSuckName[i][j];
                                }
                                else
                                {
                                    BTestSuck.Suck[i][j].Error=false;
                                }
                            }
                        }

                        if(bHasErr)
                        {
                            if(IndexAlarmInArmAway()==false)                                                            //Steven 20130613 : Index異常時, In Arm要先讓位功能
                            {
                                return false;
                            }
                            bHasErr=false;
                            if(CosFunction.bJAM0303NeedOpenChamberDoor)                                                 //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
                                bIsTestSitICFallDown=true;                                                              //kevin 20130706
                            if(IniConfig.bIndexDropOnlySKIP==true ||
                               IniConfig.bKoreaFunction)                                                                //jou 2012-02-13 index drop error only skip
                            {
                                ret=ShowErrorMessage("JAM0304", K_SKIP, MTestZ2, bHasDuplicateErr, ErrPart);            //Device Drop Error
                            }
                            else
                            {
                                ret=ShowErrorMessage("JAM0304", K_SKIP|K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);    //Device Drop Error
                            }

                            if(ret==K_SKIP)
                            {
                                bAutoSiteMapWaitTestResult=false;                                                       //Ifor 20180115 (Steven) : add Site Mapping SKIP 需清除旗標
                                for(int i=0; i<BTestSuck.iShtRow; i++)
                                {
                                    for(int j=0; j<BTestSuck.iShtCol; j++)
                                    {
                                        if(BTestSuck.Suck[i][j].Error)
                                        {
#if 0 // TODO(W7) -- golden :2083 BTestSuck.PordRec[i][j].AddErrorRecord("JAM0304") (TMyProductionRecord has no .cpp yet)  //Steven 20161214 : 加上Index異常Skip的ErrorLog
                                            BTestSuck.PordRec[i][j].AddErrorRecord("JAM0304");                          //Steven 20161214 : 加上Index異常Skip的ErrorLog
#endif
                                            if(CosFunction.bUseSCKART)                                                  //Steven 20161214 (wei) : For SCK ART
                                            {
                                                fSCKART->AddOutputJamCnt(i, j, ret,
                                                    Prod.bIsPassBin[BTestSuck.iBinData[i][j]]);                         //RogerYang 20250923 : 整合ART OutArm JamCount
                                            }
                                            BTestSuck.SetItemData(i, j, HAS_NULL_IC);
                                            BTestSuck.Suck[i][j].Error=false;
                                        }
                                        bArm2DuplicateErr[i][j]=false;
                                    }
                                }

                                if(IniConfig.bD42IndexPickICShuttlePause)                                               //ChungHung 20110302 start
                                {
                                    bInArmNeedToSafePos=true;
                                    bShuttle2Pause=true;
                                    bIndexArm2PickupErrStop=true;                                                       //jou 2012-02-29 index pick up error,index arm move to center & alarm
                                    bShowShuttle2Device=true;                                                           //kevin 20180504 index pick up error
                                }

                                if(IniConfig.bD43IndexPickErrCheckSocket==true)                                         //Steven 20190115 : SCC要求吸取異常要檢查Socket
                                {
                                    bIndexArm2PickUpErrNeedPiggyback=true;
                                    if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                                //Sam 20230108 : 矽格俊堯要求不要 Reset IC
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
                                for(int i=0; i<BTestSuck.iShtRow; i++)
                                {
                                    for(int j=0; j<BTestSuck.iShtCol; j++)
                                    {
                                        if(BTestSuck.Suck[i][j].Error)
                                        {
                                            bArm2DuplicateErr[i][j]=true;
                                            W64bT2_MoveSuckData(BLCarryKit, BTestSuck, i, j);                          // AI(W5-aTester_Rear-Translate) 20260710: golden :2127 BLCarryKit.MoveSuckData(BTestSuck,i,j) -- gap stub (see above)
                                            BTestSuck.Suck[i][j].Error=false;
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
                if(bShuttle2Pause)
                    MOT[MInShuttle2].SetSpeed(10);                                                                      //kevin 20180226 (Steven) add pick up error  shuttle down speed
                MOT[MInShuttle2].fCanMoveM=true;
                W64bT2_SetUnuseToNullIC(BTestSuck);                                                                     // AI(W5-aTester_Rear-Translate) 20260710: golden :2142 BTestSuck.SetUnuseToNullIC() -- gap stub, see file-top (Steven 20241017 : 清除沒用到的資料)
                bSuckingFlagZ2=false;
                ResetInxedArm2Flag();                                                                                   //Steven 20241017 : 打包成function
#if 0 // TODO(W5-Automation) -- golden :2145-2158 fSCKART->iInfo_MultiLotCnt / LotSummary.AddByLotLoadCount
                // (AI(W5-aTester_Rear-Translate) 20260710: TfSCKART FormsFacade (FormsFacade.h) lacks iInfo_MultiLotCnt
                //  (golden Automation/SCK_ART.h:323, Automation subsystem not yet home);
                //  LotSummary/TLotSummary (golden cSocket.h) not yet translated at all.
                //  Pure per-2DID lot-load bookkeeping, no state the SM reads back --
                //  gating the whole guarded block is behaviourally identical offline.)
                if(fSCKART->iInfo_MultiLotCnt>1)
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            if(BTestSuck.Item[i][j]!=NULL_IC &&
                               BTestSuck.cDeviceInf[i][j]!="")
                            {
                                LotSummary.AddByLotLoadCount(BTestSuck.cDeviceInf[i][j]);
                            }
                        }
                    }
                }
#endif

                if(DeviceForm_File.ContactMode==DirectContactSoftEP ||
                   DeviceForm_File.ContactMode==DropContactSoftEP)                                                      //kevin 20130608 Soft Contact mode
                {
                     W64bT2_APAX_WriteData(true, 0, 2);                                                               // AI(W5-aTester_Rear-Translate) 20260710: golden :2163 APAX_WriteData (adam6024.h APAX_* family; different from the already-shimmed ADAM_* -- gap stub, see file-top)  //ARM2 shuttle 吸完ic 浮動頭不充氣
                }
#if 0 // TODO(W7) -- golden :2165 fAutoTeach SetIndexBinError(1) (TfAutoTeach VCL form -- unlike chkReadTorque/edTorue1, NO FormsFacade/TU-local stand-in exists anywhere in the tree yet; pure UI auto-align marker, no state the SM reads back)  //JimmyChiu 20211020 : Auto alignment mode
                fAutoTeach->SetIndexBinError(1);                                                                        //JimmyChiu 20211020 : Auto alignment mode
#endif
                if(bHPCleanout)                                                                                         //wei 20160624 Hotplate clean out
                {
                    return true;
                }
                else if(LastSet.iTemperature==Tempture_Hot && bInitialSackTime &&                                       //2013-11-27   Dell    需要做Index soak time
                        DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle==false &&                                       //JerryYang 20220805 : 有開下壓shuttle預熱功能就不要做懸空預熱
                        (Temperature.iInitialStart1Time>0 || Temperature.iIndexSoakTime>0))                             //kevin 20131112 第一次吸取ic等待時間)
                {
                    int iSetTmr=Temperature.iInitialStart1Time;
                    if(Temperature.iIndexSoakTime>Temperature.iInitialStart1Time)
                        iSetTmr=Temperature.iIndexSoakTime;

                    DoTestHeadMotorDelay2.SetSecAndOn(iSetTmr);
                    Task=502;
                    bInitialSackTime=false;
                    bInitialStart1Time=true;                                                                            //wei 20171020 (jou) InitialStart1 秒數倒數
                    dwStartInitialCount2.LatchCycleTime(true);
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
                    iInitialStart1Count=(Temperature.iInitialStart1Time)-(dwStartInitialCount2.LatchCycleTime()/1000);
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
//------------------------------------------------------------------------------
