// =============================================================================
//  ainarm2.cpp  --  in-arm shared helpers (offline Sim-HAL translation)
//
//  Translation wave: W6.2b-canary (in-arm per-site-config variant dispatch)
//  Translator: AI(W6.2b-canary) 20260626
//
//  Mirror of the golden BCB6 ainarm2.cpp (D:/HT9045/
//  HT9011UC_Code_V3.33.906.0_20260618/ainarm2.cpp).  W6.2b only needs the two
//  in-arm helpers that the freshly-translated site-variant SMs
//  (ainarm9045_1x1_1 / _2x4_16 / _2x8_32) call into but that had no body yet:
//
//    * SetInArmUseSuckToHasNullIC(int, int)  -- golden ainarm2.cpp:201
//        Re-classifies the in-arm pick grid's NULL_IC nozzles to HAS_NULL_IC
//        (the "I am committed to picking here" marker) honouring the picker
//        kind (AxEx / AxxG step) and the 2x8 close-site sub-modes.  Pure grid
//        logic over the Sim HAL InArmSuck object -- NO hardware touched.
//    * InitInArmPickFromHotPlateTask()       -- golden ainarm2.cpp:623
//        Resets the HP-pick task cursor to 1.  (Its 340/50 siblings already
//        live in aHotPlateSubstrate.cpp.)
//
//  Both are FAITHFUL line-for-line translations -- names, formulas, branches and
//  comments preserved verbatim.  The declarations these realize already exist in
//  aHotPlateSubstrate.h:610/611 (so no header edit is needed).
//
//  Also hosts the single active definition of bDestoryOnSht (golden moved it
//  ainarm2 -> cmydef by RogerYang 20251021; cmydef.h:5868 declares it extern,
//  but the cmydef.cpp definition sits inside a #if 0 // TODO(W6) block, so the
//  one live definition is provided here in this linked TU -- exactly one
//  definition, no ODR dupes).
//
//  Big5: Chinese comments preserved as UTF-8 (cp950).  NO U+FFFD.
// =============================================================================
#include "cmydef.h"                 // iInArmType / MInArmX / NULL_IC / HAS_NULL_IC consts
#include "MachineType.h"            // eTestMode (SingleSite) / e9045_* picker enums
#include "cprod.h"                  // TestIF (iTestMode)
#include "ainarm9045.h"             // bUseAxExPicker / bUseAxxGPicker / GetJStep
#include "aHotPlateSubstrate.h"     // InArmSuck grid + iInArmPickFromHotPlateTask / iCloseSiteModeFor2x8 cursors

// AI(pt-wave) 20260811 PT-W7e: includes the appended bodies need. Hoisted from part
// 03050, which injected them MID-FILE (a part file may not touch the file head).
#include "FormsFacade.h"
#include "canary_support.h"
#include "csystem.h"
#include "mysensor.h"
#include "myswitch.h"                   // SW[]                     -- myswitch.h:43
#include "acatchtray_shims.h"           // NewRecordProcess
#include "aoutarm9045.h"                // GetOutArmPitchX/Y_9045
#include "SECSGEM/SecsEventType.h"      // SECS_EVENT
#include "SECSGEM/SecsEventReport.h"    // EventReport

// ---------------------------------------------------------------------------
//  PT-W7e: the THIRTY-SEVEN file-scope globals golden declares BETWEEN functions.
//  Copied VERBATIM from golden by line number, not retyped from a type+name summary --
//  PT-W7d got three of seven wrong that way: two array types declared as scalars (one of
//  which compiled fine) and two initial values written 0 where golden has 1. SEVEN of the
//  declarations below carry array dimensions (golden :88, :102, :2952, :2956, :2959, :2960,
//  :3527), which is exactly what a type+name summary drops.
//  Two are typed HTimer in golden (:2951, :3792) and are declared TQPF_Timer here, because
//  the port's only HTimer is atester_shims.h:463 whose Off() is hard-coded true and would
//  silently zero every dwell; each carries that reason inline.
//  Added by the integrator, never by an agent: two agents emitting one global is a
//  multiple-definition link error.
//  AI(pt-wave) 20260811
// ---------------------------------------------------------------------------
const int cMaxInArmOrder=2;   // golden :66
int cMaxKit=1;   // golden :67
int iNextPlateRow;   // golden :81
TQPF_Timer VibrationShuttleDelay;                                               //JerryYang 20171205 (Steven) Shuttle 震動馬達 delay   // golden :87
TQPF_Timer VibrationOutShuttleDelay[2];                                         //JerryYang 20171205 (Steven) Shuttle 震動馬達 delay   // golden :88
int iAutoCleanCnt=0;                                                            //JerryYang 20171127 (Steven) add auto clean清潔次數   // golden :93
bool bInArmZNeedCheck[4]={false, false, false, false};   // golden :102
bool bRecordVacuumTim = false;                                                  //kevin 20210224 記錄計算真空產生器壽命   // golden :103
bool bAutoCleanUse_13_24=false;                                                 //ChungHung 20131118 add for SCK AutoClean use HotPlate   // golden :109
int iInArmZCheckPosTask=1;                                                      //Richard 2017.11.1 After pause check arm Z in position   // golden :110
int iInArmDevicePosPrecise=1;                                                   //Frank 20180410 (Steven) : InArm Preciser Station   // golden :111
TQPF_Timer ShakeShuttleDelay;   // golden :2313
TQPF_Timer ShakeDelay;   // golden :2314
int iKnockShuttleTask;   // golden :2449
int iKnockShtFirstTask;   // golden :2535
int iProcessTrayMapDataErrorTask;   // golden :2839
int iOneKitPreciser=0;   // golden :2944
bool bOneKitPreciser=false;   // golden :2945
TQPF_Timer DoInArmDevicePosPreciseDelay;   // golden :2951 -- golden type HTimer; TQPF_Timer because atester_shims.h:463 HTimer::Off() is always true
int  iPrecise_XP[X_PITCH_COUNT];   // golden :2952
int  iPrecise_YP=0;   // golden :2953
int  iPrecise_MoveXP=12000;   // golden :2954
int  iPrecise_MoveYP=6000;   // golden :2955
int  iPrecise_Z[MAX_ARM_Row][MAX_ARM_Col];   // golden :2956
bool bPrecise_XYZ=false;   // golden :2957
bool bPreciseFlag=false;   // golden :2958
bool bPrecise_Z[MAX_ARM_Row][MAX_ARM_Col];   // golden :2959
bool bPrecise_SuckFlag[MAX_ARM_Row][MAX_ARM_Col];   // golden :2960
int iInitialFlag    =0;   // golden :2962
int iMoveDownPlace  =1;   // golden :2963
int iMoveDownAdd200 =2;   // golden :2964
int iMoveDownPick   =3;   // golden :2965
int iVibrateShuttleTask;   // golden :3459
int iVibrateOutShuttleTask[2];   // golden :3527
int  InDieCleanTask=1;   // golden :3791
TQPF_Timer DoInDieCleanDelay;   // golden :3792 -- golden type HTimer; TQPF_Timer because atester_shims.h:463 HTimer::Off() is always true
int  iOCRWaitTask=1;   // golden :3948

// =============================================================================
//  bDestoryOnSht  (golden cmydef.cpp:5955 -- RogerYang 20251021: 從inarm2移過來)
//  在Shuttle吹氣與資料交換的Flag (Steven 20170905 wei).  cmydef.cpp's definition
//  is gated #if 0 // TODO(W6); this is the single ACTIVE definition.
// =============================================================================
#ifndef HT9045_bDestoryOnSht_DEFINED
#define HT9045_bDestoryOnSht_DEFINED
bool bDestoryOnSht=false;                                                       //RogerYang 20251021 : 從inarm2移過來 //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
#endif

// =============================================================================
void SetInArmUseSuckToHasNullIC(int iSht, int iKit)
{
//    int iKit32=(iKit==1)?4:0;

    if(iInArmType==e9045_1x4_8_Hot ||                                           //Steven 20240119 : fixed for 2x4_16
       iInArmType==e9045_1x2_4_Hot ||
       iInArmType==e9045_2x2_8_Hot)
    {
//        iSht=0;
//        iKit32=0;
    }

    if(TestIF.iTestMode==SingleSite ||                                          //Steven 20140614 : for Auto Clean Single Site
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
    }
    else if(bUseAxExPicker() ||                                                 //Steven 20241102 : Fixed for HP
            bUseAxxGPicker())
    {
        for(int i=0; i<InArmSuck.iPickRow; i++)
        {
            for(int j=0; j<InArmSuck.iPickCol; j++)
            {
                int j2=j*GetJStep();
                if(j2<InArmSuck.iMaxCol &&
                   InArmSuck.Item[i][j2]==NULL_IC)
                {
                    InArmSuck.SetItemData(i, j2, HAS_NULL_IC);
                }
            }
        }
    }
    else
    {
        for(int i=0; i<InArmSuck.iPickRow; i++)
        {
            for(int j=0; j<InArmSuck.iPickCol; j++)
            {
                if(InArmSuck.Item[i][j]==NULL_IC)
                    InArmSuck.SetItemData(i, j, HAS_NULL_IC);
            }
        }
    }

    if(iCloseSiteModeFor2x8==e2x8Run2x2_13)
    {
        InArmSuck.SetItemData(0, 1, NULL_IC);
        InArmSuck.SetItemData(0, 3, NULL_IC);
        InArmSuck.SetItemData(1, 1, NULL_IC);
        InArmSuck.SetItemData(1, 3, NULL_IC);
    }
    else if(iCloseSiteModeFor2x8==e2x8Run2x2_14)
    {
        InArmSuck.SetItemData(0, 1, NULL_IC);
        InArmSuck.SetItemData(0, 2, NULL_IC);
        InArmSuck.SetItemData(1, 1, NULL_IC);
        InArmSuck.SetItemData(1, 2, NULL_IC);
    }
}
//==============================================================================
void InitInArmPickFromHotPlateTask()
{
    iInArmPickFromHotPlateTask=1;
}
//==============================================================================
// =============================================================================
//  Check_QA_ModeCount()   golden ainarm2.cpp:160-199   (Steven 20111019 : QA Mode)
//  Translation wave: PT-W7e (ainarm2.cpp leaf batch)  Translator: AI(k7-ainarm2) 20260811
//
//  ROLE -- QA-Mode lot-length governor, polled from the in-arm loader-pick path.
//    Counts loader cycles (iQAModeLoaderCT) against Prod.iQAModeCount and drives the
//    three-stage QA-Mode exit: (1) at/over the count -> force a quick Clean Out via
//    InitOneCycle + iOneCycle=1 and return true; (2) past the count -> flip the tester
//    OFF_LINE, restore the backed-up in-arm speed mode and fire the real Clean Out;
//    (3) approaching the count -> switch the in-arm to FIX speed mode so the last
//    devices are produced deterministically.
//
//  WAVE SCOPE -- Check_QA_ModeCount (golden :160-199) ACTIVE, no gates.
//  GATE REGISTER -- (empty: this function needed no #if 0.)
//
//  NET-NEW SYMBOL.  Unlike 9 of this wave's 11 leaves, Check_QA_ModeCount has NO
//  stub and NO declaration anywhere in the port tree (measured 2026-08-11 15:05:
//  python walk of 34,016 .cpp/.h files under D:/HT9045/HT9011UC_Cpp_V3.33.906.0,
//  _w7e_parts excluded -- 0 hits for 'Check_QA_ModeCount').  Its golden declaration
//  is ainarm2.h:81 and this tree has no ainarm2.h, so INTEGRATOR: the golden callers
//  (ainarm9045*.cpp loader-pick arms) still need a declaration -- until then this is
//  TRAP-1 shape (a), nobody references it.  Deliberately NOT `static` (shape (d)).
//
//  ------------------------------------------------------------------------------
//  STITCH HAND-OFF: INCLUDES THE STITCHED ainarm2.cpp NEEDS FOR THIS WAVE'S PARTS.
//  The port ainarm2.cpp today includes only cmydef.h / MachineType.h / cprod.h /
//  ainarm9045.h / aHotPlateSubstrate.h.  Verified by g++ -fsyntax-only on a scratch
//  TU carrying all 11 bodies of this wave (2026-08-11) -- the additions are exactly
//  the header set the sibling engines asortarm.cpp:505-527 / aoutarm9045.cpp:53-79
//  already use:
//     #include "MachineDefine.h"        // <windows.h> -> ZeroMemory (CheckInArmSuckInitial)
//     #include "csystem.h"              // CheckSuckInitialStatus:390 / IndexHasIC / ShuttleHasIC /
//                                        // InitOneCycle:88 / InitCleanOutFunction:74
//     #include "Motor/mymotor.h"        // MOT[] / InArmZSafe:440 / DETECT_ALL_FLAG / DETECT_SENSOR_FLAG
//     #include "mycylin.h"              // Cylinder[] (DoKnockShuttle)
//     #include "cpublic.h"              // CosFunction
//     #include "ainarm9045_w7_shims.h"  // IndexZCanMove[2]:58 / bShuttleKnock:62 (+ the
//                                        // DoKnockShuttle decl:48 that carries the default arg)
//     #include "csystem_shims.h"        // bShuttleShake:73 (def csystem_shims.cpp:38)
//     #include "atester_shims.h"        // IsNNMode:277 / NN_2Row
//     #include "FormsFacade.h"          // fMain (forms/fMain.h:217 ModifyTester, :168 CleanOut)
//     #include "acatchtray_shims.h"     // NewRecordProcess:388
//     #include "canary_support.h"       // ShowErrorMessage:66 / ShowMyMessage:80 / RecordProcess /
//                                        // LastSet / IniConfig / K_RETRY|K_SKIP / __FUNC__:45
//  (TrayForm comes from cprod.h:1359 and ArmSpeed[] from cprod.h:2875 -- already included.)
//  ------------------------------------------------------------------------------
//  Big5 golden -> UTF-8 here; Chinese comments transcribed character-for-character.
// =============================================================================
bool Check_QA_ModeCount()                                                       //Steven 20111019 : QA Mode //Dell 20111024
{
    if(IniConfig.bQAMode==true && LastSet.iRunStartMode==rsmQAMode)             //Steven 20111005 : QA Mode
    {
        if(iQAModeLoaderCT>=Prod.iQAModeCount && bQAModeQuickCleanOut==false && bQAModeFinishCleanOut==false)  //數量到的話要先快速CleanOut
        {                                                                       //jou 2012-05-03 增加QA mode使用獨立的Loader Count
            if(bQAModeQuickCleanOut==false && bQAModeFinishCleanOut==false)
            {
                if(CUSTOMER_CODE!=CC_KYEC_LEE)  //Eastsun 20260526 #026-1.1 Ifor 20201102 add:KYEC QA Mode 不顯示 Tray Feed
                    TrayForm.bAutoFeed=false;
                bQAModeQuickCleanOut=true;
                InitOneCycle("QA Mode");
                iOneCycle=1;
                return true;
            }
        }
        else if(iQAModeLoaderCT>Prod.iQAModeCount)                              //數量比設定值多，表示已經做完，所以就直接送Bin 1
        {
            if(bQAModeQuickCleanOut==true && bQAModeFinishCleanOut==false)
            {
                fMain->ModifyTester(OFF_LINE);                                  //Steven 20191218 : 整合修改LastSet.iTester
                NewRecordProcess("MES2155", "Change to Off_Line", "by Check_QA_ModeCount QAMode1");                     //ChungHung 20140722 add add record

                ArmSpeed[InArm].bVariModeFIX=IniConfig.bBackUpInArmMode;
                bQAModeFinishCleanOut=true;
                bQAModeQuickCleanOut=false;

                if(iCleanOut==0 &&
                   CUSTOMER_CODE!=CC_KYEC_LEE)                                  //Eastsun 20260526 #026-1.2 Ifor 20201102 add:KYEC QA Mode 不自動Clean Out
                    InitCleanOutFunction();
            }
        }
        else if(iQAModeLoaderCT>Prod.iQAModeCount-((InArmSuck.iMaxRow*InArmSuck.iMaxCol*2>20)?(InArmSuck.iMaxRow*InArmSuck.iMaxCol*2):20) &&
                iQAModeLoaderCT<Prod.iQAModeCount)                              //數量接近時，要改用Fix方式生產
        {
            ArmSpeed[InArm].bVariModeFIX=true;
        }
    }
    return false;
}
//==============================================================================

//==============================================================================
//== ROLE: AdjustShuttlePlaceOrderForASM -- choose which shuttle Auto Site Mapping
//==       places the next HotPlate device into.  In fixed single-shuttle mode
//==       (iShuttleMode==1) it mirrors the operator shuttle selection; otherwise
//==       it either resets the cursor to shuttle 0 or advances it round-robin over
//==       shuttle 0 and shuttle 1.
//==
//== WAVE SCOPE (k2-ainarm2):
//==   AdjustShuttlePlaceOrderForASM  golden ainarm2.cpp:748-770  -- ACTIVE
//==     (golden lines emitted VERBATIM; ZERO gates)
//==
//== GATE REGISTER: (none)
//==   * TestIF_File.iShuttleMode (cprod.h:1654) and .iShuttle_Sel (cprod.h:1655);
//==     the object is extern cprod.h:2577 -- resolved by the include block already
//==     at the top of port ainarm2.cpp.
//==   * iAutoSiteMapHPToSht -- declared cmydef.h:4386, really DEFINED at
//==     cmydef.cpp:4537 (int, initial 0).  A real definition, not a stub.
//==
//== INTEGRATION NOTE -- TRAP 1 shape (a), NO LIVE CALLER YET:
//==   grep -rn --include=*.cpp "AdjustShuttlePlaceOrderForASM" D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//==   -- ran 2026-08-11 06:24 +0800: exactly three hits, and ALL THREE sit inside
//==   block comments that golden itself opened (ainarm_SearchPlacePlate.cpp:4839
//==   inside the comment opened at its :4835; csystem.cpp:7226 and :7262 inside
//==   the comment opened at its :7219).  So after this part lands the archive
//==   member is still not extracted by anyone -- a green build proves nothing.
//==   There is also NO declaration anywhere in the port.  Golden ainarm2.h:129 is
//==     extern void AdjustShuttlePlaceOrderForASM(bool bResetSht=false);
//==   and the integrator must add it.  The default argument belongs on the
//==   DECLARATION only: golden's definition at :748 carries none and this part
//==   matches that, so a later header add cannot become a re-default error.
//==   Not static -- once that declaration lands it is non-static (shape (d)
//==   avoided).
//==
//== Big5 -> UTF-8: the trailing Chinese comment on the signature line is
//== transcribed character for character from cp950.  Zero U+FFFD.
//==============================================================================
void AdjustShuttlePlaceOrderForASM(bool bResetSht)                              //Steven 20250206 : fixed ASM之中作one cycle會造成關arm 1時異常
{
    if(TestIF_File.iShuttleMode==1)
    {
        if(TestIF_File.iShuttle_Sel==0)
            iAutoSiteMapHPToSht=0;
        else if(TestIF_File.iShuttle_Sel==1)
            iAutoSiteMapHPToSht=1;
    }
    else
    {
        if(bResetSht)
        {
            iAutoSiteMapHPToSht=0;
        }
        else
        {
            iAutoSiteMapHPToSht++;
            if(iAutoSiteMapHPToSht>=2)
                iAutoSiteMapHPToSht=0;
        }
    }
}

//==============================================================================
//  ResetInArmParam()  --  golden ainarm2.cpp:923-926
//
//  ROLE          One-line wrapper: reset the whole in-arm parameter set by
//                re-running InitInArmTask().
//  WAVE SCOPE    ResetInArmParam()          ACTIVE   golden :923-926
//  GATE REGISTER (none -- InitInArmTask() has a linkable body)
//
//  HAND-OFF, NOT A GATE (TRAP 1 shape (b), stated so it is not mistaken for
//  "working"): InitInArmTask() resolves to aHotPlateSubstrate.cpp:1062
//  `void InitInArmTask() {}` -- a no-op STUB (decl aHotPlateSubstrate.h:896, and
//  a second decl at acarry_shims.h:217).  So this call is translated FAITHFULLY
//  and links, but offline it resets nothing.  The degradation is entirely in the
//  callee and is NOT this wave's to fix; do not read a green build as evidence
//  that in-arm params are being reset.  1 live call site today.
//==============================================================================
void ResetInArmParam()
{
    InitInArmTask();
}

//==============================================================================
//  DoInArm_SuckerMapForCloseArm()                         golden ainarm2.cpp:928-963
//  Translation wave: PT-W7e   Translator: AI(k4-ainarm2) 20260811
//
//  ROLE       : when only one of the two in-arms is selected (iShuttleMode==1),
//               blanks the OTHER arm`s whole 2x8 in-use nozzle map, then snapshots
//               both arms` maps into fNeedToCheckASM (the "these nozzles still owe an
//               Auto-Site-Map result" grid) and clears bInitialAutoSiteMap.
//  WAVE SCOPE : DoInArm_SuckerMapForCloseArm()  golden :928-963  -- ACTIVE
//  GATE REGISTER: none.  Prod.fInArmSuck4x8 (cprod.h:820), Prod.fNeedToCheckASM
//               (cprod.h:818), Prod.bInitialAutoSiteMap (cprod.h:816) and
//               TestIF_File (cprod.h) are all reachable from this TU`s existing
//               include head.
//  TRAP 1     : shape (a) applies TODAY -- the port has NO declaration for this
//               function in any header (golden home ainarm2.h:149) and its only
//               golden caller is golden ainarm9045.cpp:4814, which the port has not
//               reached.  So this body links in but has no caller until the
//               integrator adds the declaration.  It is deliberately NON-static:
//               making it static to silence a "defined but not used" style warning
//               would create shape (d) the moment that declaration lands.  There is
//               no pre-existing stub to retire (checked: 0 definitions tree-wide).
//==============================================================================
void DoInArm_SuckerMapForCloseArm()
{
    if(TestIF_File.iShuttleMode==1)
    {
        if(TestIF_File.iShuttle_Sel==0)                                         //只用Arm 1
        {
            for(int i=0; i<2; i++)
            {
                for(int j=0; j<8; j++)
                {
                    Prod.fInArmSuck4x8[1][i][j]=false;
                }
            }
        }
        else                                                                    //只用Arm 2
        {
            for(int i=0; i<2; i++)
            {
                for(int j=0; j<8; j++)
                {
                    Prod.fInArmSuck4x8[0][i][j]=false;
                }
            }
        }
    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            Prod.fNeedToCheckASM[0][i][j]=Prod.fInArmSuck4x8[0][i][j];
            Prod.fNeedToCheckASM[1][i][j]=Prod.fInArmSuck4x8[1][i][j];
        }
    }
    Prod.bInitialAutoSiteMap=false;
}

//==============================================================================
//  ChangeHotPlateData(bool bSwapSht)  --  golden ainarm2.cpp:976-991
//  Wave PT-W7e, part k1-ainarm2.  Translator: AI(W7E-k1-ainarm2) 20260811
//
//  ROLE  After a one-cycle / order change, re-point the in-arm at the Shuttle+Kit
//        of the LAST recorded hot-plate suck team that still holds data (so UPH
//        does not collapse by re-planning from scratch); if there is no such team,
//        fall back to re-ordering the plain shuttle place order.
//
//  WAVE SCOPE  ChangeHotPlateData  PARTLY GATED  golden :976-991
//              (:978-987 gated, :988-991 ACTIVE -- see GATE k1-G1).
//
//  GOLDEN BUG KEPT: the `bSwapSht' parameter is never read by golden's body.
//  Kept verbatim (name included) -- callers pass it and golden ignores it.
//
//  GATE REGISTER
//   GATE k1-G1 -- golden :978-987: `uHPSuckTeam *HTTeam;' +
//     `HTTeam=PickFromHPList->ExtractLastTeamHasData();' + the whole
//     `if(HTTeam!=NULL){...}' arm (InArmSuck.iWhichSht/iWhichKit <- HTTeam->iSht/
//     iKit + AdjustShuttleWhichKitOrder()).  The `else' keyword is gated with it;
//     its BLOCK stays active, so control still runs AdjustShuttlePlaceOrder().
//
//     WHY GATED -- THIS IS A TRAP 3 / TRAP 5 CASE, NOT A PLAIN ABSENCE.
//     A real port body for ExtractLastTeamHasData DOES exist:
//     Public/HTEditList.cpp:2234 (declared Public/HTEditList.h:365), and that
//     header also carries a COMPLETE uHPSuckTeam with the very iSht/iKit fields
//     this code reads (Public/HTEditList.h:258-282).  Taking it is still WRONG:
//       * There are TWO different `class uPlateInfo' in the port -- the minimal
//         mirror at aHotPlateSubstrate.h:672-716 (NO data members at all, so
//         sizeof==1, and no ExtractLastTeamHasData) and the full golden-shaped one
//         at Public/HTEditList.h.  They can never coexist in one TU (redefinition),
//         so including HTEditList.h here would SWAP the class identity of
//         PickFromHPList for this TU only.
//       * The live object is allocated by the MINIMAL class:
//         aHotPlateSubstrate.cpp:855-862 `if(PickFromHPList==NULL)
//         PickFromHPList = new uPlateInfo();' -- a data-member-less object.  The
//         single definition of the pointer is Public/HTEditList.cpp:204.
//         Calling the FULL class's ExtractLastTeamHasData on that object reads its
//         TList members past the end of a 1-byte allocation.  It would compile,
//         it would link, and it would corrupt memory -- exactly TRAP 5.
//       * uHPSuckTeam as seen from THIS TU (aHotPlateSubstrate.h:652) is an
//         incomplete type, so `HTTeam->iSht' cannot compile at all without that
//         swap.
//     So the honest answer to "why SHOULD this be gated" is not "the symbol is
//     missing" -- it is "the symbol belongs to a second, incompatible definition of
//     uPlateInfo, and this TU's PickFromHPList does not point at that layout".
//     Retire this gate only when the two uPlateInfo classes are unified (the
//     cross-cutting job HTEditList.h:40-63 already describes and defers).
//
//     WHY THE OFFLINE DEFAULT IS FAITHFUL: with the minimal uPlateInfo the team
//     list can never be non-empty -- every mutator (AddHPSuckGroup /
//     UpdateHPSuckGroup / ClearGroupList / ExtractFirstTeam) is an unconditional
//     no-op and the class holds no list (documented at aHotPlateSubstrate.h:653-671).
//     So ExtractLastTeamHasData() would return NULL, golden would take its OWN
//     `else' arm, and AdjustShuttlePlaceOrder() is exactly what runs here.  The
//     gated form reproduces golden's offline behaviour statement for statement.
//     REAL-MACHINE DIFFERENCE: on hardware with the hot-plate list populated,
//     golden resumes at the last team's Shuttle/Kit and calls
//     AdjustShuttleWhichKitOrder(); gated, the arm instead re-orders the plain
//     shuttle place order, i.e. it may target a different Shuttle/Kit than the
//     half-finished team -- a UPH / place-order difference, not a motion-safety one
//     (both arms only rewrite selectors, they move nothing).
//     ABSENCE COMMAND (exact -- proves the ABSENCE IS ONLY FROM THIS TU'S HEADER):
//       rg -n -w ExtractLastTeamHasData --glob '*.cpp' --glob '*.h' D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//         -> 3 hits, all in Public/HTEditList.{h:365,cpp:75,cpp:2234}; ZERO in
//            aHotPlateSubstrate.h (the uPlateInfo this TU sees).
//       rg -n "class uPlateInfo" --glob '*.h' D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//         -> TWO classes (aHotPlateSubstrate.h + Public/HTEditList.h).
//     MEASURED AT: 2026-08-11T06:36:51+0800 (first) / 2026-08-11T06:44 (second).
//     TRAP 2 RE-RUN, 2026-08-11T06:48:10+0800 -- the ABSENCE premise is unchanged (no
//     sibling part adds ExtractLastTeamHasData to aHotPlateSubstrate.h's uPlateInfo,
//     and unifying the two classes is explicitly out of this wave), BUT TWO OTHER
//     THINGS IN THIS FUNCTION MOVED and they must be stated:
//       * _w7e_parts/00849_AdjustShuttleWhichKitOrder.txt landed, so the call INSIDE
//         this gate (:985) now has a REAL body waiting -- the gate is withholding a
//         real call, not a no-op.  It stays gated only because reaching it requires
//         the wrong uPlateInfo; the moment the classes are unified, retiring this gate
//         becomes a behaviour change, not a cosmetic one.
//       * _w7e_parts/00707_AdjustShuttlePlaceOrder.txt landed, so the ACTIVE else-arm
//         (:989) also stops being a no-op (it is aHotPlateSubstrate.cpp:1066
//         `void AdjustShuttlePlaceOrder(int) {}' today).  Its declaration
//         aHotPlateSubstrate.h:905 carries golden's default `int iReset=-1', which is
//         what makes golden's zero-argument call at :989 compile -- the sibling's
//         DEFINITION carries no default, correctly.
//     Both make the offline default MORE faithful, not less; neither changes the gate.
//
//  TRAP 5 STATEMENT: this part relies on aHotPlateSubstrate.h for uPlateInfo /
//  uHPSuckTeam / InArmSuck (TMyKitSuck) -- the mirror 177 TUs already use.  It does
//  NOT include Public/HTEditList.h or mykitsuck.h.  The object PickFromHPList
//  points at is created in aHotPlateSubstrate.cpp:857; the pointer itself is
//  defined in Public/HTEditList.cpp:204.
//
//  Big5: the TWO Chinese trailing comments in this range -- golden :976 and :985
//  (measured: exactly the non-ASCII lines of :976-991) -- are transcribed character
//  for character from cp950.  ZERO U+FFFD.
//==============================================================================
void ChangeHotPlateData(bool bSwapSht)                                          //Steven 20100816 : 修正OneCycle時，會導致UPH降低的問題
{
#if 0 // GATE k1-G1 -- golden :978-987 (uHPSuckTeam/ExtractLastTeamHasData belong to the OTHER uPlateInfo; see GATE REGISTER above).  The `else` keyword at :987 is gated with the arm; its BLOCK (:988-990) stays ACTIVE so AdjustShuttlePlaceOrder() still runs -- which is exactly what golden does when the team list is empty.
    uHPSuckTeam *HTTeam;

    HTTeam=PickFromHPList->ExtractLastTeamHasData();
    if(HTTeam!=NULL)
    {
        InArmSuck.iWhichSht=HTTeam->iSht;
        InArmSuck.iWhichKit=HTTeam->iKit;
        AdjustShuttleWhichKitOrder();                                           //Steven 20160201 : 修正Shuttle放料順序
    }
    else
#endif // GATE k1-G1
    {
        AdjustShuttlePlaceOrder();
    }
}

// =============================================================================
//  ChangeHotPlateDataToOneShuttle()   golden ainarm2.cpp:993-1014
//  (JerryYang 20171218 (Steven) -- hot-mode dual-arm -> single-arm hang-up fix)
//  Translation wave: PT-W7e   Translator: AI(k7-ainarm2) 20260811
//
//  ROLE -- hot-mode dual-arm -> single-arm recovery: when the machine is idle enough
//    (index OR shuttle OR in-arm carries no IC) and single-shuttle mode is selected,
//    rewrite the WHOLE iHotWhichShuttle[2][50][50] hot-plate reservation grid to the one
//    selected shuttle, so devices already reserved for the now-closed shuttle cannot
//    hang the flow.
//
//  WAVE SCOPE -- ChangeHotPlateDataToOneShuttle (golden :993-1014) ACTIVE, no gates.
//  GATE REGISTER -- (empty.)
//
//  NET-NEW SYMBOL (this wave's second, with Check_QA_ModeCount): 0 hits tree-wide in
//  the 34,016-file walk measured 2026-08-11 15:05.  Golden's declaration is ainarm2.h;
//  this tree has no ainarm2.h, so no caller can reach it yet (TRAP-1 shape (a)).  NOT
//  `static` (shape (d)).  Its golden caller is the ainarm2.cpp:640-671 hot-mode
//  dispatcher, which is not in this wave.
//  BOUNDS NOTE -- golden walks j,k<50 over iHotWhichShuttle[2][50][50] (golden :73):
//  the literal 50s are golden's own array extents, NOT the runtime plate size.  Kept.
//  Big5 golden -> UTF-8 here; Chinese comments transcribed character-for-character.
// =============================================================================
void ChangeHotPlateDataToOneShuttle()                                           //JerryYang 20171218 (Steven) fix 加熱模式雙arm切單arm hang up問題
{
    if(LastSet.iTemperature==Tempture_Hot)
    {
        if(IndexHasIC()==false || ShuttleHasIC()==false || InArmSuck.HasIC()==false)
        {
            if(TestIF.iShuttleMode==1)
            {
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<50; j++)
                    {
                        for(int k=0; k<50; k++)
                        {
                            iHotWhichShuttle[i][j][k]=TestIF_File.iShuttle_Sel;
                        }
                    }
                }
            }
        }
    }
}
//==============================================================================

//==============================================================================
// ---- k3-ainarm2 / golden ainarm2.cpp:1140-1184 -----------------------------
//  ROLE          -- MoveInArmZToPlateSafe() plus Ifor's 20221227 guard: after the
//                   in-arm has picked off the Loader tray and its Z is back at the
//                   plate-safe height, verify the machine did not lift the LOADER
//                   TRAY ITSELF along with the ICs.  Suspicion = the sure-tray or
//                   pre-detect sensor went OFF, or the Y fixer cylinder is pushed
//                   out while its push sensor reads ON.  Three consecutive
//                   suspicious passes raise JAM0929 (RETRY|SKIP); SKIP clears the
//                   tray data + fHasTray and lets the flow continue, RETRY keeps
//                   the caller blocked.
//  WAVE SCOPE    -- MoveInArmZToPlateSafeAndCheckLoaderTray(int) ACTIVE golden :1140-1184
//  GATE REGISTER -- (empty: this range contains no #if 0 -- every symbol it needs
//                   has a real port home; see the include block below)
//
//  FIDELITY NOTES (golden quirks kept, NOT fixed)
//   * `int ret=0;` is dead on every path that does not reach the JAM -- kept.
//   * `bfirstIn=true;` is written twice on the SKIP path (:1162 before the alarm
//     and again :1168 inside the SKIP arm) -- redundant, kept verbatim.
//   * Because :1162 sets bfirstIn=true BEFORE the alarm, a RETRY answer restarts
//     the 3-strike counter from zero rather than resuming it.  That is golden's
//     behaviour and callers depend on the resulting re-count; NOT changed.
//   * `LastSet.iRealDummy>=HAS_TRAY` is an ordered compare on the dummy-mode enum,
//     not an equality -- reproduced exactly.
//   * The 2 function-local `static`s are golden's own and are PODs with constant
//     initialisers -- no ctor runs, so TRAP 4 does not apply.  They are function-
//     local, NOT the file-scope `static` function definition TRAP 1 (d) forbids.
//
//  TRAP 1 (a) HAND-OFF -- NOTHING IN THE PORT CALLS THIS YET.  Golden's 7+ call
//  sites are all in ainarm9045.cpp (:7667 :8008 :8028 :8156 :8167 :8173 :8254 ...)
//  and none of them survived into the port: `grep -rIn "Ifor 20221227"` over the
//  port tree returns ZERO hits (run 2026-08-11 06:31 +0800), while the port's
//  ainarm9045.cpp instead has bare `MoveInArmZToPlateSafe(Task)` at :1226 :1309
//  :1376 :1473.  So this body will link only once a caller is (re)wired, and until
//  then the loader-tray-lifted JAM0929 protection is ABSENT AT RUNTIME even though
//  the function exists -- exactly the "build green proves nothing" shape.  Also,
//  golden declares it at ainarm2.h:140; the port has no ainarm2.h and
//  aHotPlateSubstrate.h (the mirror) does not declare it, so the integrator must
//  add `extern bool MoveInArmZToPlateSafeAndCheckLoaderTray(int iTask);` there.
//
//  DEPENDENCY: the wrapped MoveInArmZToPlateSafe(int) is declared
//  aHotPlateSubstrate.h:901 and its CURRENT port body :1065 is the offline stub
//  `return true;` (Z reaches safe immediately).  Sibling part
//  _w7e_parts/01025_MoveInArmZToPlateSafe.txt lands the real golden body (:1025) in
//  this same stitch -- re-checked 2026-08-11 06:46:04 +0800 (`ls _w7e_parts | grep
//  MoveInArmZToPlateSafe` -> 01025, 01140 (mine)).  Until that lands, this guard
//  runs on EVERY tick instead of only after the Z is genuinely back at safe height,
//  which makes the 3-strike counter reach 3 far sooner.  Both parts must land
//  together for the timing to be golden-faithful.
// ---------------------------------------------------------------------------
//AI(k3-ainarm2) 20260811: HOIST ME to ainarm2.cpp's include head -- mid-file
//  include block, the convention already used at ainarm9045.cpp:5389/:10133/:10607.
#include "mysensor.h"               // Sen[] (mysensor.h:48) : SnLoaderSureTray / SnLoaderPreDete / SnLoaderFixCyPush -- golden :1155-1156
#include "mycylin.h"                // Cylinder[] (mycylin.h:176) / TMyCylinder::GetOutBit (:121) -- golden :1156
#include "canary_support.h"         // ShowErrorMessage (:66, signature verbatim), __FUNC__ (:45), LastSet (via LastSet.h:587) -- golden :1153/:1163/:1166
// ---------------------------------------------------------------------------
bool MoveInArmZToPlateSafeAndCheckLoaderTray(int iTask)                         //Ifor 20221227 add:新增Loader 取料後判斷是否吸到Loader Tray
{
    static bool bfirstIn=true;
    static int iCheckCount=0;
    int ret=0;
    if(MoveInArmZToPlateSafe(iTask))
    {
        if(bfirstIn==true)
        {
            bfirstIn=false;
            iCheckCount=0;
        }

        if(LastSet.iRealDummy>=HAS_TRAY && MOT[MMTrayY].fHasTray==true)
        {
            if(Sen[SnLoaderSureTray].IsOff()==true || Sen[SnLoaderPreDete].IsOff()==true ||
              (Cylinder[C_TrayY_Fixer].GetOutBit()==true && Sen[SnLoaderFixCyPush].IsOn()==true))
            {
                iCheckCount++;

                if(iCheckCount>=3)
                {
                    bfirstIn=true;
                    ret=ShowErrorMessage("JAM0929", K_RETRY|K_SKIP, MMTrayY, false, "CheckLoaderTray");                 //Steven 20120925 : JAM0909 -> JAM0929
                    if(ret==K_SKIP)
                    {
                        MOT[MMTrayY].ClearTray(__FUNC__);
                        MOT[MMTrayY].fHasTray=false;
                        bfirstIn=true;
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                return false;
            }
        }

        bfirstIn=true;
        return true;
    }
    return false;
}

// =============================================================================
//  AI(W906-PT-W7e-k8) 20260811 -- golden ainarm2.cpp translation part
//  golden file : D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/ainarm2.cpp (cp950, CRLF)
//  this part   : golden lines 1188..1204
// -----------------------------------------------------------------------------
//  ROLE        : true -- and forces a re-home -- if ANY in-arm Z motor is off its taught safe position; golden's own section header above it reads 'Z axis safe-position check, kevin 20120217'.
//  WAVE SCOPE  : CheckInArmZ -- ACTIVE, golden ainarm2.cpp:1188..1204.
//  INCLUDES     : this body needs, beyond what port ainarm2.cpp already includes
//              : (cmydef.h / MachineType.h / cprod.h / ainarm9045.h /
//              : aHotPlateSubstrate.h):
//              : Motor/mymotor.h (MOT[] and ReadPos), aHotPlateSubstrate.h (SetInArmHome :906) -- already included
//  GATE REGISTER: none.  The whole selector line is already proven compilable in this
//              : tree: acatchtray.cpp:407 (inside InArmZInSafe) carries the
//              : byte-identical `(USE_PICKER_COUNT==ep16Picker &&
//              : InOutArmPickerUseMotor==eptUseMotCyn)?MInArmZA:
//              : InArmSuck.Suck[i][j].iMotNo` expression.  Prod.ZInArmSafe cprod.h:403
//              : inside PROD_INFO_ST (which closes at cprod.h:1137; `extern
//              : PROD_INFO_ST Prod;` cprod.h:1138).
//  TRAP 5       : InArmSuck is the aHotPlateSubstrate.h:365 TMyKitSuck (declared
//              : aHotPlateSubstrate.h:624, object DEFINED aHotPlateSubstrate.cpp:80),
//              : whose TMySucker carries iMotNo at aHotPlateSubstrate.h:132.  NOT the
//              : mykitsuck.h:274 class -- that one has a different layout, and picking
//              : it would link cleanly and then read iMotNo at the wrong offset.
//  EXPIRED GATE : landing this body makes RotateKit/aRotateKIT_In.cpp's GATE (9) stale.
//              : That file routes golden :53 / :878 through
//              : `#define W906RKIN_CHECKINARMZ() (CheckInArmZ())` at
//              : aRotateKIT_In.cpp:377 with prose at :142, :375, :438 and :1276 claiming
//              : CheckInArmZ has no compiled body.  That premise dies here.  Re-ask
//              : TRAP 3 before retiring it: this function has a real side effect
//              : (SetInArmHome), so ungating CHANGES machine behaviour and belongs in
//              : its own measured commit, not in this stitch.
// =============================================================================
bool CheckInArmZ()
{
    int iMot;
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            iMot=(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)?MInArmZA:InArmSuck.Suck[i][j].iMotNo;
            if(MOT[iMot].ReadPos()!=Prod.ZInArmSafe[i][j])
            {
                SetInArmHome();
                return true;
            }
        }
    }
    return false;
}

//==============================================================================
//  MoveInArm2XYToLoaderWait()                             golden ainarm2.cpp:1374-1426
//  Translation wave: PT-W7e   Translator: AI(k4-ainarm2) 20260811
//
//  ROLE       : parks the in-arm at the NOT-hot wait position above the Loader tray,
//               used for one-cycle / clean-out / tray-feed.  Refuses while an
//               alarm-triggered ServoOff on the in-arm is still pending; refuses
//               (after HARD-STOPPING both in-arm axes and telling the operator) if
//               the Tray arm is not clear of the empty-tray X position; otherwise
//               samples the current pitch-axis positions so the pitch mechanism is
//               carried rather than re-driven, marks iInArmWaitPosition=1, and issues
//               the continuous XY move to (Tray pick X +6000, Tray pick Y -10000).
//  WAVE SCOPE : MoveInArm2XYToLoaderWait()  golden :1374-1426  -- ACTIVE (no gate)
//  GATE REGISTER: none.  Every symbol resolves: IniConfig.bAlarmNeedServoOff
//               (Config.h:125, reachable via cmydef.h -> cprod.h:6),
//               fNote->bMyServoOffInArm (forms/fNote.h:99), MOT[] +
//               TTrayMotor::ReadPos/PCIL132_StopMotor (Motor/mymotor.h:385/:264,
//               reachable via aHotPlateSubstrate.h:68), Prod.iXTrayEmpty
//               (cprod.h:470), ShowMyMessage (canary_support.h:80),
//               USE_IN_OUT_ARM_Y_PITCH + the four iXYPitch* consts + the MInArm*
//               motor indices (cmydef.h:2103/:2133/:2134/:2162/:2163/:2877-:2888),
//               InArmContinuousMove_9045 (Motor/mymotor.h:396 -- 7 args, bLoader
//               defaults false, signature matches golden exactly),
//               Prod.XInArm_Tray_Pick / YInArm_Tray_Pick (cprod.h:481-482),
//               iInArmXBase / iInArmYBase (cmydef.h:3638-3639).  ZeroMemory comes
//               from <windows.h>, pulled by vclcompat/vcl_compat.h.
//  TRAP 1     : shape (b) IS ALREADY PRESENT in the tree and this body does NOT fix
//               it -- csystem.cpp:4880-4881 carries
//                 static bool W7C1_MoveInArm2XYToLoaderWait(){ return true; }
//                 #define MoveInArm2XYToLoaderWait  W7C1_MoveInArm2XYToLoaderWait
//               so csystem.cpp`s call sites (:5578, :7333, :14702, plus the :11952
//               note) keep calling that TU-local always-true stub even after this
//               real body lands.  The macro must be deleted (main-loop task #15) or
//               "build green" will again prove nothing.  Note the stub returns TRUE
//               where this body returns FALSE on both refusal paths -- deleting the
//               macro is a real behaviour change, and that is the point.  Different
//               NAME, so this is not shape (d).
//  GOLDEN QUIRK PRESERVED (not fixed here): at golden :1396-1398 RogerYang 20250820
//               commented out the `if(...) return false; else` around the
//               ShowMyMessage, leaving :1399 indented as if it were still the else
//               arm.  It now runs unconditionally on that path.  Indentation and the
//               dead comment lines are transcribed verbatim.
//
//  HOIST ME to ainarm2.cpp`s include head at integration:
#include "canary_support.h"      // ShowMyMessage(AnsiString,AnsiString,...) :80 -- golden :1399
#include "forms/fNote.h"         // fNote (TfNote*) / bMyServoOffInArm :99 -- golden :1386
//  iInArmWaitPosition (golden ainarm2.h:219 -- THIS file is its golden home) has a
//  temporary port definition at acatchtray.cpp:135 and NO declaration in any header,
//  so it is declared TU-locally here exactly as asendic_Loader.cpp:233 already does.
//  TODO(integrate): when the golden-home definition moves into ainarm2.cpp, delete
//  this extern and the acatchtray.cpp:135 definition together.
extern int iInArmWaitPosition;                                                  // golden ainarm2.h:219
//==============================================================================
bool MoveInArm2XYToLoaderWait()                                                 //Steven 20120314 : 要移動到不熱的等待位置
{
    int iXVariable[X_PITCH_COUNT];
    int iYVariable=0;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20111226 : In Arm 咖啡杯解除
    {
        if(fNote->bMyServoOffInArm)
        {
            return false;
        }
    }

    if(MOT[MTrayX].ReadPos()<Prod.iXTrayEmpty)                                  //JerryYang 20200206 one cycle, clean out, tray feed時in arm移動到loader
    {
        MOT[MInArmX].PCIL132_StopMotor();
        MOT[MInArmY].PCIL132_StopMotor();
//        if(MOT[MInArmY].ReadPos()<Prod.iInArmSafeY)                             //RogerYang 20250820 : InArmY到loader區才判斷
//            return false;
//        else
            ShowMyMessage("Tray arm not Safe pos", "MoveInArm2XYToLoaderWait");

        return false;
    }

    iXVariable[0]=MOT[MInArmPitch].ReadPos();
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
       USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                            //Ztex 2024.02.24 Add HT-1132
    {
        iXVariable[1]=MOT[MInArmPitchX2].ReadPos();
        iYVariable   =MOT[MInArmPitchY].ReadPos();
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                         //Steven for HT1032
            USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
        iXVariable[1]=MOT[MInArmPitchX2].ReadPos();
        iXVariable[2]=MOT[MInArmPitchX3].ReadPos();
        iXVariable[3]=MOT[MInArmPitchX4].ReadPos();
        iYVariable   =MOT[MInArmPitchY].ReadPos();
    }

    iInArmWaitPosition=1;                                                       //Ifor 20191105 : add InArm 安全位置判斷 0:Wait 1:LoaderWait 2:DecayWait 3:Shuttle2
    if(InArmContinuousMove_9045(Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+6000, Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]-10000, iXVariable, iYVariable, bZFlag, iZPos, false))
    {
        return true;
    }
    return false;
}

// =============================================================================
//  AI(W906-PT-W7e-k8) 20260811 -- golden ainarm2.cpp translation part
//  golden file : D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/ainarm2.cpp (cp950, CRLF)
//  this part   : golden lines 1482..1535
// -----------------------------------------------------------------------------
//  ROLE        : move the in-arm X/Y onto the Decay (ion-fan) teach point, carrying the current pitch-motor positions through so the pitch axes are not disturbed; refuses and demands a re-home if an alarm left the in-arm servo off.
//  WAVE SCOPE  : MoveInArm2XYToDecayTeach -- ACTIVE, golden ainarm2.cpp:1482..1535.
//  COLLISION    : none -- no port body exists today (see EXPIRED GATES below).  This
//              : is a NET-NEW body and it is deliberately NON-static (TRAP 1(d)):
//              : golden declares `extern bool MoveInArm2XYToDecayTeach();` at
//              : ainarm2.h:145 and three port TUs already reference the name.
//  INCLUDES     : this body needs, beyond what port ainarm2.cpp already includes
//              : (cmydef.h / MachineType.h / cprod.h / ainarm9045.h /
//              : aHotPlateSubstrate.h):
//              : Motor/mymotor.h (MOT[] + InArmContinuousMove_9045 :396), Config.h (IniConfig.bAlarmNeedServoOff :125), forms/fNote.h (fNote->bMyServoOffInArm :99), canary_support.h (ShowMyMessage :78-79), cMyDB.h or acatchtray_shims.h (NewRecordProcess -- only inside the DEBUG_AutoHomeLog block)
//  GATE REGISTER: none.  Every symbol resolves: fAllMotorHome cmydef.h:222,
//              : CUSTOMER_CODE cmydef.h:3181, CC_SIGURD_PeiXing MachineType.h:319 (=946),
//              : MInArmPitch / MInArmPitchX2 / X3 / X4 / MInArmPitchY,
//              : USE_IN_OUT_ARM_Y_PITCH plus iXYPitchVariable / iXYPitchIn_Bb_Out_Bc /
//              : iXYPitch16Picker / iXYPitch16Bd_Be, Prod.iInArmDecay_X cprod.h:871 and
//              : Prod.iInArmDecay_Y cprod.h:873, bMoveInArm2XYToWaitOk cmydef.h:3509,
//              : X_PITCH_COUNT MachineType.h:408 (=4), InArmContinuousMove_9045
//              : Motor/mymotor.h:396 (8 parameters, last one defaulted -- golden passes 7).
//  ONE DECL HOIST: iInArmWaitPosition -- see the block just below this banner.
//  #ifdef KEPT  : the `#ifdef DEBUG_AutoHomeLog` block (golden :1496-:1501) is
//              : transcribed verbatim.  It is INACTIVE in this tree -- MachineType.h:32
//              : carries the define commented out, `//#define DEBUG_AutoHomeLog` --
//              : which is exactly golden's shipped configuration.  Not removed and not
//              : rewritten: a translation preserves every #ifdef block.
//  EXPIRED GATES: this body retires the PREMISE of THREE existing port gates, each of
//              : which states in prose that MoveInArm2XYToDecayTeach has no body
//              : anywhere in the tree:
//              :   (1) ainarm9045.cpp:1317  `#if 0 // TODO(W7) -- golden :9205 ...`
//              :       -- its #else substitutes `bDecayReached=true;`, i.e. it FAKES the
//              :       move as reached.  That is the dangerous direction (a stub turning
//              :       not-done into done) and it should be the first one retired.
//              :   (2) csystem.cpp:20296-20320  GATE G01 -- its own DELTA text explains
//              :       it took the fail-loud direction (bflag2 stays false, WAR2026 keeps
//              :       firing) precisely because faking the move would blow the ion fan
//              :       at an untaught X/Y.  THAT PREMISE IS THIS AGENT'S PREMISE TOO --
//              :       see gate W7E-K8-C in part 03799, which reuses G01's reasoning
//              :       instead of re-deriving it.
//              :   (3) csystem.cpp:31215-31240  the third call site.
//              : DO NOT retire any of the three inside this stitch: all three drive real
//              : motor moves, so each is a measured behaviour commit of its own.
// =============================================================================
// -----------------------------------------------------------------------------
//  DECLARATION HOIST (a declaration -- NOT a definition, NOT a global, NOT a gate)
//  iInArmWaitPosition has a real port DEFINITION with external linkage at
//  acatchtray.cpp:135 (`int  iInArmWaitPosition = 0;`, golden home cmydef.cpp,
//  Ifor 20191105) but NO `extern` declaration in ANY port header.  Command, run from
//  D:/HT9045/HT9011UC_Cpp_V3.33.906.0 at 2026-08-11 06:33:21 local:
//      rg -n --no-messages -g '*.h' 'iInArmWaitPosition' .
//  -> 1 hit, and that hit is PROSE INSIDE A COMMENT (acatchtray_shims.h:435, the note
//  recording that acatchtray.cpp owns this global this wave).  So the symbol LINKS but
//  does not COMPILE from here.  TRAP 3 applied: the answer to "no declaration" is to
//  DECLARE, not to gate -- gating would throw away real behaviour over a prototype.
//  INTEGRATOR: please hoist this one line into the stitched file's declaration block
//  beside the ainarm2 globals, and delete it from here.
#ifndef HT9045_W7E_K8_iInArmWaitPosition_DECLARED
#define HT9045_W7E_K8_iInArmWaitPosition_DECLARED
extern int iInArmWaitPosition;
#endif
// -----------------------------------------------------------------------------
bool MoveInArm2XYToDecayTeach()                                                 //Ifor 20151210 :新增InArm Move to Decay Teach 點位
{
    int iXVariable[X_PITCH_COUNT];
    int iYVariable=0;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20111226 : In Arm 咖啡杯解除
    {
        if(fNote->bMyServoOffInArm)
        {
            #ifdef DEBUG_AutoHomeLog
            if(fAllMotorHome==true)
            {
                NewRecordProcess("", "fAllMotorHome", "MoveInArm2XYToDecayTeach");
            }
            #endif

            fAllMotorHome=false;                                                //Ifor 20151211 須重新回home
            if(CUSTOMER_CODE==CC_SIGURD_PeiXing)
            {
                ShowMyMessage("Input Arm Servo Off, need home");                //JerryYang 20160328 for 矽格北興,觸發回home的地方補上log
            }
            return false;
        }
    }

    iXVariable[0]=MOT[MInArmPitch].ReadPos();
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
       USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                            //Ztex 2024.02.24 Add HT-1132
    {
        iXVariable[1]=MOT[MInArmPitchX2].ReadPos();
        iYVariable   =MOT[MInArmPitchY].ReadPos();
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                         //Steven for HT1032
            USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
        iXVariable[1]=MOT[MInArmPitchX2].ReadPos();
        iXVariable[2]=MOT[MInArmPitchX3].ReadPos();
        iXVariable[3]=MOT[MInArmPitchX4].ReadPos();
        iYVariable   =MOT[MInArmPitchY].ReadPos();
    }

    iInArmWaitPosition=2;                                                       //Ifor 20191105 : add InArm 安全位置判斷 0:Wait 1:LoaderWait 2:DecayWait 3:Shuttle2
    if(InArmContinuousMove_9045(Prod.iInArmDecay_X, Prod.iInArmDecay_Y, iXVariable, iYVariable, bZFlag, iZPos, false))
    {
        bMoveInArm2XYToWaitOk=true;                                             //Steven 20131025 : 要先動到安全位置才可以再到別的地方
        return true;
    }
    return false;
}

//==============================================================================
//== ROLE: TransferLoaderRatio -- the Loader software gear ratio.  Re-scales an
//==       in-arm XY Loader-tray target about the teach base cell
//==       Prod.[XY]InArm_Tray_Pick[iInArmYBase][iInArmXBase] by whichever loader
//==       tray scale applies: tri-temp Hot, tri-temp Cold, the per-setup-file one,
//==       or the plain one.  No expansion coefficient is used on this path.
//==
//== WAVE SCOPE (k2-ainarm2):
//==   TransferLoaderRatio  golden ainarm2.cpp:2112-2183  -- ACTIVE with 1 GATE
//==     (golden lines emitted VERBATIM apart from the #if 0 / #endif wrapping
//==      golden :2117)
//==
//== GATE REGISTER
//== -- GATE k2ai2-G2 ------------------------------------------------------------
//==   GOLDEN LINE         : ainarm2.cpp:2117
//==                         CheckInArmXYScaleByAutoTeach(*iXPos, *iYPos, AOA_Loader)
//==   WHY THE OFFLINE DEFAULT IS FAITHFUL:
//==     Identical premise to GATE k2ai2-G1 in part 01767 -- and per the task
//==     instruction I state the carry-over rather than re-deriving it.  The gated
//==     statement is the whole body of the
//==     if(MACHINE_HAS_AUTO_ALIGNMENT_CCD && TestIF.bEnableAutoAlignment==true)
//==     arm; MACHINE_HAS_AUTO_ALIGNMENT_CCD is DEFINED false at cmydef.cpp:5316,
//==     so offline the arm is never entered and an empty arm is bit-identical.
//==     The JUSTIFICATION, though, is the absence premise below, not that
//==     equivalence (TRAP 2).
//==   REAL-MACHINE BEHAVIOUR DIFFERENCE:
//==     On an AOA machine golden replaces the loader gear ratio with the CCD
//==     auto-teach correction.  With this gate on such a machine gets NEITHER --
//==     the raw loader XY passes through with no compensation at all.
//==   EXACT COMMAND PROVING THE SYMBOL IS ABSENT (TRAP 2):
//==     grep -rnE "^[[:space:]]*(extern[[:space:]]+)?void[[:space:]]+CheckInArmXYScaleByAutoTeach" --include=*.cpp --include=*.h D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//==     -> no output, exit 1.
//==   TIME I RAN IT: 2026-08-11 06:37:25 +0800.
//==   TRAP 3 -- WHY THIS SHOULD STAY GATED EVEN IF THE PREMISE DIES:
//==     Same as k2ai2-G1: the AutoClean.cpp:232 static stand-in must be deleted in
//==     the SAME commit that retires this gate, or that file gains a static shadow
//==     of a now-real function while still calling it at its :2261 (TRAP 1 shape
//==     (d)).  Also re-check the new signature takes int& / int&.
//==
//== GOLDEN QUIRKS PRESERVED (not fixed):
//==   * golden :2161 re-declares double fi; inside the
//==     else if(TestIF_File.bInArmUseDifferentScaleBySetupFile) block, SHADOWING
//==     the outer fi from :2121.  Reproduced verbatim -- expect a -Wshadow warning
//==     from the project warn flags; that warning is golden's, not a defect of
//==     this translation.
//==   * golden :2124 repeats &&LastSet.iTemperature==Tempture_Hot (with no space
//==     before the &&) inside an if that already tested exactly that.  Reproduced
//==     verbatim including the spacing.
//==   * Because the Hot arm at :2122 swallows the whole tri-temp block, the
//==     setup-file scale (:2159) and the plain scale (:2172) are UNREACHABLE while
//==     iTemperature==Tempture_Hot.  Reproduced verbatim.
//==   * The outer double fi at :2121 is left uninitialised, exactly as golden has
//==     it; every path that reads it assigns it first.
//==   * The double-to-int narrowing on *iXPos= / *iYPos= is golden's truncation
//==     and is load-bearing.  There is NO int/int division in this function.
//==
//== GATE-FREE DEPENDENCIES (all real, all resolved in this TU):
//==   Prod.{X,Y}InArm_Tray_Pick cprod.h:481/482 (object cprod.h:1138);
//==   LastSet.fLoaderTray{X,Y}Scale_Hot LastSet.h:546/547, _Cold :557/558, plain
//==   :381/382; TestIF_File.fLoaderTray{X,Y}ScaleBySetupFile cprod.h:2423/2424;
//==   AOA_Loader cprod.h:3175; the rest as listed in part 01767.
//==
//== INTEGRATION NOTE -- TRAP 1 shapes (a) AND (c), TWO STEPS:
//==   Unlike its HotPlate sibling this function has NO stub and NO declaration
//==   anywhere in the port.  Verified with
//==     grep -rn --include=*.cpp --include=*.h "TransferLoaderRatio" D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//==   ran 2026-08-11 06:26 +0800: every hit is either a comment or a GATED call.
//==   (1) ADD the declaration -- golden ainarm2.h:160 is
//==       extern void TransferLoaderRatio(int *iXPos, int *iYPos);
//==       Without it no caller can even name this function, so nothing extracts
//==       the archive member and the build stays green while doing nothing.
//==   (2) TWO LIVE GATES ELSEWHERE GO STALE THE MOMENT THIS LANDS, which is
//==       exactly TRAP 2 seen from the other side:
//==         * ainarm9045.cpp:7967  GATE k4-G4  -- its own text says "LIVE,
//==           re-verified absent 2026-08-11 01:50:31 +0800"; that claim expires
//==           against this part.
//==         * cInArmPlacement.cpp:724  GATE (1)
//==       Both must be retired in the same commit.  ainarm9045_2x8_32.cpp:1217 is
//==       a third call site but it is DEAD -- that file is #if 0 from its line 91
//==       to EOF -- so it needs no action beyond the eventual whole-file ungate.
//==   Not static -- once the declaration lands it is non-static (shape (d)
//==   avoided).
//==
//== TRAP 4 ANSWER: no file-scope object is added by this part.
//==
//== Big5 -> UTF-8: the Chinese comments on golden :2112, :2115, :2121 and :2158
//== are transcribed character for character from cp950.  Zero U+FFFD.
//==============================================================================
void TransferLoaderRatio(int *iXPos, int *iYPos)                                //Steven 20141029 : Loader的軟體齒輪比
{
    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&                                        //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
       TestIF.bEnableAutoAlignment==true)                                       //KenHsieh 20210824 : add AOA功能開啟不補償
    {
#if 0 // GATE k2ai2-G2 -- golden ainarm2.cpp:2117 CheckInArmXYScaleByAutoTeach: same dead symbol as GATE k2ai2-G1, the only body in the tree is the static no-op at AutoClean/AutoClean.cpp:232.  Retire BOTH gates together.  See this part's GATE REGISTER for the absence command, its timestamp and the real-machine delta.
        CheckInArmXYScaleByAutoTeach(*iXPos, *iYPos, AOA_Loader);
#endif
    }
    else
    {
        double fi;                                                              //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature -->
        if(LastSet.iTemperature==Tempture_Hot)
        {
            if(CosFunction.bHotModeUseDiffScale==true && IniConfig.bE30_1InArmUseDifferentScale_Hot &&LastSet.iTemperature==Tempture_Hot)
            {
                fi=*iXPos-Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];
                fi*=LastSet.fLoaderTrayXScale_Hot;
                *iXPos=fi+Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];

                fi=*iYPos-Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
                fi*=LastSet.fLoaderTrayYScale_Hot;
                *iYPos=fi+Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
            }
            else
            {
                if(IniConfig.bE30_1InArmUseDifferentScale_Hot==true && Temperature.fWorkTemperBase>=26)
                {
                    fi=*iXPos-Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];
                    fi*=LastSet.fLoaderTrayXScale_Hot;
                    *iXPos=fi+Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];

                    fi=*iYPos-Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
                    fi*=LastSet.fLoaderTrayYScale_Hot;
                    *iYPos=fi+Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
                }

                if(IniConfig.bE30_2InArmUseDifferentScale_Cold==true && Temperature.fWorkTemperBase<26)
                {
                    fi=*iXPos-Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];
                    fi*=LastSet.fLoaderTrayXScale_Cold;
                    *iXPos=fi+Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];

                    fi=*iYPos-Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
                    fi*=LastSet.fLoaderTrayYScale_Cold;
                    *iYPos=fi+Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
                }
            }
        }                                                                       //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature <--
        else if(TestIF_File.bInArmUseDifferentScaleBySetupFile)
        {
            double fi;

            fi=*iXPos-Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];
            fi*=TestIF_File.fLoaderTrayXScaleBySetupFile;
            *iXPos=fi+Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];

            fi=*iYPos-Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
            fi*=TestIF_File.fLoaderTrayYScaleBySetupFile;
            *iYPos=fi+Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
        }
        //pig 20220101 AddInputScaleBySetup
        else if(IniConfig.bE30InArmUseDifferentScale)
        {
            fi=*iXPos-Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];
            fi*=LastSet.fLoaderTrayXScale;
            *iXPos=fi+Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];

            fi=*iYPos-Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
            fi*=LastSet.fLoaderTrayYScale;
            *iYPos=fi+Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
        }
    }
}

// =============================================================================
//  AI(W906-PT-W7e-k8) 20260811 -- golden ainarm2.cpp translation part
//  golden file : D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/ainarm2.cpp (cp950, CRLF)
//  this part   : golden lines 2620..2722
// -----------------------------------------------------------------------------
//  ROLE        : AutoTeach helper: move the IN arm (iInArm==0) or the OUT arm to the hard-coded per-nozzle teach X/Y for Z-number ZNumber, after the HotPlate-ratio transform; a Reset call just answers false.
//  WAVE SCOPE  : bMoveInArmXYToTeach -- ACTIVE, golden ainarm2.cpp:2620..2722.
//  COLLISION    : none in the port TODAY -- no port body, no port caller (measured
//              : 2026-08-11 06:20 local, `rg -w bMoveInArmXYToTeach` over
//              : D:/HT9045/HT9011UC_Cpp_V3.33.906.0 -> 0 hits).  BUT SEE THE GOLDEN BUG.
//  *** GOLDEN BUG -- NOT FIXED HERE, RECORDED AS REQUIRED ***
//              : golden defines `bool bMoveInArmXYToTeach(bool Reset, int ZNumber,
//              : int iInArm)` TWICE, both NON-static and both unconditionally compiled:
//              :     ainarm2.cpp:2620             (this part)
//              :     AutoTeach/AutoTeach.cpp:1972 (a MATERIALLY DIFFERENT body)
//              : Both translation units are in HT9045.bpr (`ainarm2.cpp` / `ainarm2.obj`
//              : and `AutoTeach\AutoTeach.cpp` / `AutoTeach.obj` all appear in it), and
//              : neither definition sits inside any #if or #ifdef -- I walked the
//              : preprocessor nesting of both files and the nesting depth at both
//              : definition lines is 0.  golden ainarm2.h does NOT declare the symbol,
//              : so each TU binds its own copy at the source level while the linker sees
//              : one public name: which body actually serves AutoTeach.cpp's 8 call
//              : sites (:2222, :2243, :2257, :2268, :2282, :2292) is link-order
//              : dependent.  The two bodies are NOT equivalent -- THIS one drives
//              : InArmContinuousMove_9045 / OutArmContinuousMove_9045 with the
//              : X_PITCH_COUNT pitch array and HAS the `USE_IN_Y_IS_AUTO_PITCH &&
//              : MACHINE_HAS_AUTO_ALIGNMENT_CCD -> iYPitchData=4000` branch, while the
//              : AutoTeach one drives individual MOT[].MotorMove() latched through
//              : static bMove[10] / bOutMove[10] and has NO 4000 branch.
//              : CONSEQUENCE FOR THE PORT: landing this body is collision-free now,
//              : because AutoTeach/AutoTeach.cpp is untranslated, and it WILL become a
//              : `multiple definition` link error the day AutoTeach lands.  Whoever
//              : translates AutoTeach must decide which body golden's AutoTeach form
//              : really gets -- that is a behaviour decision, not a build fix.
//  INCLUDES     : this body needs, beyond what port ainarm2.cpp already includes
//              : (cmydef.h / MachineType.h / cprod.h / ainarm9045.h /
//              : aHotPlateSubstrate.h):
//              : Motor/mymotor.h (MOT[], InArmContinuousMove_9045 :396, OutArmContinuousMove_9045 :409), aoutarm9045.h (GetOutArmPitchY_9045 :56, GetOutArmPitchX_9045 :57)
//  GATE REGISTER: none.  GetInArmPitchX_9045 ainarm9045.h:48 and GetInArmPitchY_9045
//              : ainarm9045.h:50, USE_IN_Y_IS_AUTO_PITCH cmydef.h:2890,
//              : MACHINE_HAS_AUTO_ALIGNMENT_CCD cmydef.h:5237, MachineTypeChoice
//              : cmydef.h:3286, Type_HT9046_LS MachineType.h:428 (=300),
//              : iInArmAutoYTeachOffset cmydef.h:4203, iOutArmAutoYTeachOffset
//              : cmydef.h:4204, TransferHotPlateRatio aHotPlateSubstrate.h:898.
//  INTEGER DIV  : `3000*(ZNumber/2)` at golden :2658, :2667, :2698 and :2706 is INTEGER
//              : division and it is LOAD-BEARING -- it is what makes nozzle pairs
//              : (A,B), (C,D) and so on share one X column while ZNumber%2 picks the Y
//              : row.  Left as int/int; converting it to floating point would smear
//              : the columns.  Likewise `ZNumber%2` stays integer modulo.
//  #ifdef KEPT  : golden's trailing `#ifdef SOFT_SIMULTE / return true / #else /
//              : return false / #endif` (:2717-:2721) is transcribed verbatim.
//  TRAP 1(a)    : NOTHING in the port calls this yet -- golden's only callers are the 8
//              : sites in the untranslated AutoTeach.cpp.  So a green build proves
//              : NOTHING about it: it is an unreferenced archive member and will not
//              : even be extracted from the archive.  Do not report it as 'linked'.
// =============================================================================
bool bMoveInArmXYToTeach(bool Reset, int ZNumber, int iInArm)
{                                                                               //目前使用吸嘴
    int iXPos=0, iYPos=0, iYPitchData=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable  =0;
    int iMovePitchX =9000;
    int iMovePitchY =6000;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(Reset)
    {
        return false;
    }

    if(iInArm==0)                                                               //INARM
    {
        iYVariable=GetInArmPitchY_9045(iMovePitchY, iOffsetPos);
        for(int i=0; i<X_PITCH_COUNT; i++)
            iXVariable[i]=GetInArmPitchX_9045(iMovePitchX, i, iOffsetPos);

        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 (Steven) : Y變距
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            iYPitchData=3300;
        }

        if(USE_IN_Y_IS_AUTO_PITCH==true && MACHINE_HAS_AUTO_ALIGNMENT_CCD==true)                                        //Ztex 2024.02.24 Add HT-1132 HT-1032AT  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {
            iYPitchData=4000;
        }

        if(ZNumber%2==0)                                                        //吸嘴 A C E G
        {
            iXPos=29659-3000*(ZNumber/2);                                       //Tech.iInArmTeachX-3000*(ZNumber/2);

            if(MachineTypeChoice==Type_HT9046_LS)
                iYPos=-69249+iInArmAutoYTeachOffset+iYPitchData;                //kevin 20181203//kevin 20171115 add    //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
            else
                iYPos=-52130+iInArmAutoYTeachOffset;                            //kevin 20171115 add//Tech.iInArmTeachY;
        }
        else
        {
            iXPos=29659-3000*(ZNumber/2);

            if(MachineTypeChoice==Type_HT9046_LS)
                iYPos=-69249+6000+iInArmAutoYTeachOffset+iYPitchData;           //kevin 20181203//kevin 20171115 add   //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
            else
                iYPos=-52130+6000+iInArmAutoYTeachOffset;                       //kevin 20171115 add
        }
        TransferHotPlateRatio(false, &iXPos, &iYPos);                           //Steven 20110324

        if(InArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
            return true;
    }
    else
    {
        iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);
        for(int i=0; i<X_PITCH_COUNT; i++)
            iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);

        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 (Steven) : Y變距
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            iYPitchData=3300;
        }

        if(USE_IN_Y_IS_AUTO_PITCH==true && MACHINE_HAS_AUTO_ALIGNMENT_CCD==true)                                        //Ztex 2024.02.24 Add HT-1132 HT-1032AT  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {
            iYPitchData=4000;
        }

        if(ZNumber%2==0)                                                        //吸嘴 A C E G
        {
            iXPos=-20193-3000*(ZNumber/2);
            if(MachineTypeChoice==Type_HT9046_LS)
                iYPos=-69249+iOutArmAutoYTeachOffset+iYPitchData;               //kevin 20181203 kevin 20171115 add     //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
            else
                iYPos=-52526+iOutArmAutoYTeachOffset;                           //kevin 20171115 add
        }
        else
        {
            iXPos=-20193-3000*(ZNumber/2);
            if(MachineTypeChoice==Type_HT9046_LS)
                iYPos=-69249+6000+iOutArmAutoYTeachOffset+iYPitchData;          //kevin 20181203//kevin 20171115 add    //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
            else
                iYPos=-52526+6000+iOutArmAutoYTeachOffset;                      //kevin 20171115 add
        }
        TransferHotPlateRatio(false, &iXPos, &iYPos);                           //Steven 20110324

        if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
            return true;
    }
    #ifdef SOFT_SIMULTE
        return true;
    #else
        return false;
    #endif
}

//==============================================================================
//  bOffsetClean()                                         golden ainarm2.cpp:2726-2758
//  Translation wave: PT-W7e   Translator: AI(k4-ainarm2) 20260811
//
//  ROLE       : the operator`s "clear ALL taught position offsets" one-shot.  For
//               each of the 14 offset parts it picks the ambient or the Hot offset
//               data file, force-creates the directory, and writes 0.0 over the
//               part`s PickUp / Place keys plus every per-nozzle "PickUp <letter>" /
//               "Place <letter>" key of the in/out-arm suction grid.
//  WAVE SCOPE : bOffsetClean()  golden :2726-2758  -- GATED (whole body, golden
//               :2728-2757).  The function shell is ACTIVE and non-static so the
//               eventual ainarm2.h declaration binds to it.
//  RETIRES    : nothing -- there is no port stub and no port declaration for this
//               function (0 definitions tree-wide).  TRAP 1 shape (a): it will have
//               no caller until the integrator lands golden ainarm2.h`s declaration
//               and the fOffSet UI that calls it.
//
//  GATE REGISTER
//  ------------------------------------------------------------------------------
//  [k4e-G02]  golden :2728-2757   the entire body
//    SYMBOL (a): TfOffSet::GetOffsetPath()  -- golden cOffSet.h, called golden
//               :2731.  The port`s TfOffSet (forms/fOffSet.h:18-24) has exactly two
//               methods, UseAutoOffsetFunction(AnsiString) and UseInArmSetupTeach(int).
//               There is no GetOffsetPath anywhere.
//      ABSENCE COMMAND (cwd D:/HT9045/HT9011UC_Cpp_V3.33.906.0):
//               rg -nw --glob "*.h" --glob "!build/**" GetOffsetPath .
//               RESULT: 0 lines.  RUN AT: 2026-08-11 06:31:32 +0800.
//               (SECSGEM/uHGemHT9045.cpp does write fOffSet->GetOffsetPath(...) at
//               :1132-:1145, but that file`s own note at :827-828 records the same
//               absence, so those uses are not evidence of a home.)
//    SYMBOL (b): CapStr[]  -- golden cOffSet.h:503 `extern AnsiString CapStr[]`,
//               indexed golden :2746/:2747/:2752/:2753.  The port has NO cOffSet.h at
//               all, so the array does not exist.
//      ABSENCE COMMAND (same cwd):
//               rg -n --glob "*.cpp" --glob "*.h" --glob "!build/**" "CapStr\s*\[" .
//               RESULT: 0 lines.  RUN AT: 2026-08-11 06:31:32 +0800.
//               (language.h:34 / language.cpp:409 `AnsiString *CapStr` is a PARAMETER
//               of TfLan::GotLanString, not this array -- hand-checked 2026-08-11
//               06:20 +0800.)
//    EVERYTHING ELSE IN THE BODY DOES EXIST: MyForceDirectories (common.h:341),
//               WriteIniData(...,double) (common.h:249), LastSet.iTemperature,
//               Tempture_Hot, the Ofs* enumerators (MachineType.h:741/:765/:770/:782/
//               :787) and InArmSuck.iMotRow/iMotCol (aHotPlateSubstrate.h:419-420).
//               (a) and (b) are the only blockers.
//    WHY THE OFFLINE DEFAULT (empty body) IS FAITHFUL: bOffsetClean is a pure
//               operator-initiated side effect with no return value and no state any
//               SM reads back, and it has ZERO call sites in the port tree, so an
//               empty body is observationally identical to golden on every path
//               offline can reach.
//    REAL-MACHINE DIFFERENCE: pressing "clear offsets" silently does nothing --
//               every previously taught PickUp/Place value for all 14 parts, and
//               every per-nozzle letter key, survives instead of being zeroed.
//    TRAP 3 -- WHY IT SHOULD STAY GATED EVEN AFTER (a) AND (b) LAND: the body`s ONLY
//               effect is WriteIniData(..., 0.0) into
//               "<GetOffsetPath()>\Position Offset.Data" and
//               "...\Position Offset Hot.Data".  GetOffsetPath resolves under the
//               machine`s real setup tree, so an ungated bOffsetClean reachable from
//               ctest would ZERO the real machine`s taught position offsets on disk --
//               the same hazard class as WriteLastDataFile hard-coding
//               D:\HT9045\system\lastdata.dat.  Un-gate only together with a test
//               path redirection, never merely because the two symbols appeared.
//  ------------------------------------------------------------------------------
//  GOLDEN BUG PRESERVED (not fixed here): the loop bound is iSelPartData<=13, but the
//               Hot-file selector at golden :2740-2741 tests
//               iSelPartData>=OfsInSh1LB (27) and >=OfsOutSh1LB (44).  Those two
//               sub-conditions can NEVER be true inside a 0..13 loop, so the "For 32
//               Site Loader and Shuttle Offset" and "For 32 Site Out Shuttle Offset"
//               additions are dead: the 32-site LB/RB parts are never cleared at all,
//               and if they ever were reached they would be written to the ambient
//               file.  Kept verbatim.
//==============================================================================
void bOffsetClean()
{
#if 0 // GATE k4e-G02 -- golden :2728-2757, whole body.  fOffSet->GetOffsetPath() and
      // CapStr[] are both absent from the port; see the GATE REGISTER above for the
      // two absence commands + timestamps, the real-machine difference, and the
      // TRAP 3 reason this must stay gated even after both symbols land.
    AnsiString str;
    AnsiString szDir="", szFilePath="";

    szDir=fOffSet->GetOffsetPath();                                             //Steven 20190109 : 整合Offset路徑
    szFilePath=szDir;
    MyForceDirectories(szDir);

    for(int iSelPartData=0; iSelPartData<=13; iSelPartData++)
    {
        szDir=szFilePath;                                                       //kevin 20150105
        if(LastSet.iTemperature==Tempture_Hot && ((iSelPartData==OfsInSh1  || iSelPartData==OfsInSh2 ||
                                                   iSelPartData==OfsOutSh1 || iSelPartData==OfsOutSh2) ||
                                                  (iSelPartData>=OfsInSh1LB && iSelPartData<=OfsInSh2RB)||              //Steven 20140811 : For 32 Site Loader and Shuttle Offset
                                                  (iSelPartData>=OfsOutSh1LB && iSelPartData<=OfsOutSh2RB)))            //Steven 20190802 : For 32 Site Out Shuttle Offset
            szDir+="\\Position Offset Hot.Data";
        else
            szDir+="\\Position Offset.Data";

        WriteIniData(szDir, CapStr[iSelPartData], "PickUp",   0.0);
        WriteIniData(szDir, CapStr[iSelPartData], "Place",    0.0);
        for(int i=0; i<InArmSuck.iMotRow; i++)                                  //8=In/out arm suction
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)                              //8=In/out arm suction
            {
                WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("PickUp %c", 'A'+i+j*2), 0.0);
                WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("Place %c", 'A'+i+j*2),  0.0);
            }
        }
        szDir="";
    }
#endif // GATE k4e-G02
}

//==============================================================================
//== ROLE: RecordAutoSiteMapStart -- when Auto Site Mapping is enabled, emit the
//==       SECS/GEM SiteMappingStart event (CEID 51) if SECS/GEM is on, and write
//==       the "Auto Site Map Start." line to the process log.  When ASM is
//==       disabled it does nothing at all.
//==
//== WAVE SCOPE (k2-ainarm2):
//==   RecordAutoSiteMapStart  golden ainarm2.cpp:2760-2768  -- ACTIVE
//==     (golden lines emitted VERBATIM; ZERO gates)
//==
//== GATE REGISTER: (none -- this function needed no #if 0)
//==   * IniConfig.bI21EnableASM Config.h:769, .bEnable_SECS_GEM Config.h:92
//==     (object Config.h:1499).
//==   * EventReport(unsigned) -- declared SECSGEM/SecsEventReport.h:55.  This is
//==     the Sim-first entry point, NOT golden UsecegemMainFrom.cpp:191; it has an
//==     observable counter side effect instead of talking to THGem.  That
//==     substitution predates this wave and is documented in that header, so it is
//==     not a gate of mine.
//==   * SECS_EVENT.SiteMappingStart -- SECSGEM/SecsEventType.h:93.  The trailing
//==     "51     Site Mapping Start" comment is golden's own CEID annotation.
//==   * RecordProcess(AnsiString,AnsiString) -- declared cMyDB.h:122.
//==
//== INTEGRATION NOTE -- TRAP 1 shape (a), NO CALLER AND NO DECLARATION:
//==   grep -rn --include=*.cpp --include=*.h "RecordAutoSiteMapStart" D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//==   -- ran 2026-08-11 06:16 +0800: ZERO hits.  Nothing in the port names this
//==   function today, so landing it cannot change any behaviour and a green build
//==   is no evidence of anything.  Golden ainarm2.h:228 is
//==     extern void RecordAutoSiteMapStart();
//==   which the integrator must add before any caller can reach it.  The natural
//==   first callers are golden's ASM start paths in main.cpp / csystem.cpp, which
//==   are not part of this wave.
//==   Not static -- once that declaration lands it is non-static (shape (d)
//==   avoided).
//==
//== Big5 -> UTF-8: the Chinese comment on golden :2760 is transcribed character
//== for character from cp950.  Zero U+FFFD.
//==============================================================================
void RecordAutoSiteMapStart()                                                   //Steven 20230117 : 修正Auto Site map的訊息
{
    if(IniConfig.bI21EnableASM)
    {
        if(IniConfig.bEnable_SECS_GEM==true)                                    //Steven 20140528 : Secs Gem
            EventReport(SECS_EVENT.SiteMappingStart);                           //51     Site Mapping Start
        RecordProcess("Auto Site Map Start.");                                  //kevin 20160121  add
    }
}

//==============================================================================
//== ROLE: RecordAutoSiteMapFinish -- the closing half of RecordAutoSiteMapStart.
//==       With ASM enabled it emits SECS/GEM SiteMappingEnd (CEID 52) when
//==       SECS/GEM is on and logs "Auto Site Map Finish."; with ASM disabled it
//==       logs "Auto Site Map Disabled." instead.  EITHER WAY it clears
//==       bAutoSiteMapHasPickHP -- the flag is cleared on BOTH arms, which is the
//==       whole point of golden's Steven 20230210 fix.
//==
//== WAVE SCOPE (k2-ainarm2):
//==   RecordAutoSiteMapFinish  golden ainarm2.cpp:2770-2784  -- ACTIVE
//==     (golden lines emitted VERBATIM; ZERO gates)
//==
//== GATE REGISTER: (none -- this function needed no #if 0)
//==   Same four dependencies as part 02760, plus:
//==   * SECS_EVENT.SiteMappingEnd -- SECSGEM/SecsEventType.h:94.
//==   * bAutoSiteMapHasPickHP -- declared cmydef.h:4398 (bool).
//==
//== INTEGRATION NOTE -- TRAP 1 shape (a), NO CALLER AND NO DECLARATION:
//==   grep -rn --include=*.cpp --include=*.h "RecordAutoSiteMapFinish" D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//==   -- ran 2026-08-11 06:16 +0800: ZERO hits.  Golden ainarm2.h:229 is
//==     extern void RecordAutoSiteMapFinish();
//==   and must be added before anything can call it.  Because this function is the
//==   only writer that clears bAutoSiteMapHasPickHP on the ASM-disabled path,
//==   wiring the caller is a BEHAVIOUR change and should be measured, not folded
//==   into a translation-only commit.
//==   Not static -- shape (d) avoided.
//==
//== Big5 -> UTF-8: the Chinese comment on golden :2770 is transcribed character
//== for character from cp950.  Zero U+FFFD.
//==============================================================================
void RecordAutoSiteMapFinish()                                                  //Steven 20230117 : 修正Auto Site map的訊息
{
    if(IniConfig.bI21EnableASM)
    {
        if(IniConfig.bEnable_SECS_GEM==true)                                    //Steven 20140528 : Secs Gem
            EventReport(SECS_EVENT.SiteMappingEnd);                             //52     Site Mapping End
        RecordProcess("Auto Site Map Finish.");                                 //kevin 20160121  add
        bAutoSiteMapHasPickHP=false;                                            //Steven 20230210 : Fixed for auto site map
    }
    else
    {
        RecordProcess("Auto Site Map Disabled.");
        bAutoSiteMapHasPickHP=false;                                            //Steven 20230210 : Fixed for auto site map
    }
}

// =============================================================================
//  AI(W906-PT-W7e-k8) 20260811 -- golden ainarm2.cpp translation part
//  golden file : D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/ainarm2.cpp (cp950, CRLF)
//  this part   : golden lines 2929..2932
// -----------------------------------------------------------------------------
//  ROLE        : rewind the after-pause in-arm-Z position-check cursor to 1.
//  WAVE SCOPE  : InitInArmZCheckPos -- ACTIVE, golden ainarm2.cpp:2929..2932.
//  COLLISION    : none -- no port body, no port declaration, no port caller.
//  INCLUDES     : none beyond the current set.
//  GATE REGISTER: none.  iInArmZCheckPosTask is golden ainarm2.cpp:110
//              : `int iInArmZCheckPosTask=1;` (Richard 2017.11.1) -- one of the ainarm2
//              : file-scope globals THE INTEGRATOR ADDS, so this part must not and does
//              : not emit it.  It is absent from the port until then: command run from
//              : D:/HT9045/HT9011UC_Cpp_V3.33.906.0 at 2026-08-11 06:20 local,
//              :     rg -w -g '*.cpp' -g '*.h' 'iInArmZCheckPosTask' .
//              : -> 0 hits.  If the integrator's global block does not carry golden
//              : :110, this part does not compile -- that is the correct loud failure
//              : mode, not a licence for this agent to define the global.
//  TRAP 1(a)    : golden itself has NO caller for this function.  An exhaustive scan of
//              : the golden tree finds only its own definition (ainarm2.cpp:2929), its
//              : declaration (ainarm2.h:202) and main.cpp:9945, which registers the
//              : CURSOR, not the function -- `QueueTaskList[206].SetAliasAndTask(
//              : "InArmZCheckPosTask", &iInArmZCheckPosTask)` takes the variable's
//              : address.  So this is a faithful translation of a golden function that
//              : nobody calls, and a green build says nothing whatsoever about it.
// =============================================================================
void InitInArmZCheckPos()
{
    iInArmZCheckPosTask=1;
}

//==============================================================================
//  InitInArmDevicePosPrecise()                            golden ainarm2.cpp:2946-2949
//  Translation wave: PT-W7e   Translator: AI(k4-ainarm2) 20260811
//
//  ROLE       : homes the in-arm Preciser-station (device-position-precise) task
//               cursor to step 1.
//  WAVE SCOPE : InitInArmDevicePosPrecise()  golden :2946-2949  -- ACTIVE
//  GATE REGISTER: none.
//  FILE-SCOPE GLOBALS DELIBERATELY NOT EMITTED: iInArmDevicePosPrecise is golden
//               ainarm2.cpp:111 and belongs to the integrator`s one-place block, so
//               it is USED here and assumed to exist.  Golden :2944 iOneKitPreciser
//               and :2945 bOneKitPreciser sit immediately above this function and are
//               ALSO integrator-owned -- they are outside my assigned range
//               (2946..2949) and are not emitted.
//  TRAP 1     : shape (a) today.  No port declaration exists (golden home
//               ainarm2.h:222 region) and the only textual caller,
//               ainarm9045.cpp:1174, sits inside that file`s
//               "#if 0 // TODO(W7) -- golden :3221-3239" block, so it is not a live
//               caller.  Non-static on purpose.  No stub to retire (0 definitions
//               tree-wide).  IN-WAVE UPDATE (checked 2026-08-11 06:45 +0800, 61
//               sibling part files present): the SM this cursor drives IS landing --
//               _w7e_parts/03050_DoInArmDevicePosPrecise.txt, whose golden :3052 does
//               `int &Task=iInArmDevicePosPrecise;` -- but NO sibling part calls
//               InitInArmDevicePosPrecise itself, so the no-caller status is
//               unchanged until ainarm9045.cpp:1174 is ungated.
//==============================================================================
void InitInArmDevicePosPrecise()                                                //Frank 20180410 (Steven) : InArm Preciser Station
{
    iInArmDevicePosPrecise=1;
}

//==============================================================================
//  AI(k6-ainarm2) 20260811 -- PT-W7e part 02966 : SetPrecisorZPos
//  Golden: ainarm2.cpp:2966-3048 (cp950, CRLF).
//
//  ROLE          : precomputes, per in-arm nozzle, the Precisor-station Z target
//                  (iPrecise_Z[][]) and the "this nozzle goes down" flag (bPrecise_Z[][])
//                  for the four actions DoInArmDevicePosPrecise drives: iMoveDownPlace /
//                  iMoveDownPick take Prod.iInArmPreciserPlaceZ / iInArmPreciserPickUpZ,
//                  iMoveDownAdd200 takes the same +200, and anything else (iInitialFlag)
//                  clears every flag and parks every Z at ZSafePos.  Each of the first
//                  three actions has a one-kit variant (bOneKitPreciser: only row
//                  iOneKitPreciser) and a whole-grid variant.
//  WAVE SCOPE    : SetPrecisorZPos(int)  golden ainarm2.cpp:2966-3048  ACTIVE
//  GATE REGISTER : (none)  InArmSuck aHotPlateSubstrate.cpp:80 of class
//                  aHotPlateSubstrate.h:365 (see TRAP 5 in the report) ;
//                  Prod.iInArmPreciserPlaceZ/PickUpZ cprod.h:941/:942 ; ZSafePos
//                  Motor/mymotor.h:392 ; NULL_IC/HAS_NULL_IC cmydef.h.  iOneKitPreciser /
//                  bOneKitPreciser / iPrecise_Z / bPrecise_Z / iMoveDownPlace /
//                  iMoveDownAdd200 / iMoveDownPick are the integrator's file-scope globals
//                  (golden :2944/:2945/:2952-2965) -- USED, not defined here.
//  GOLDEN BUGS (kept verbatim -- do NOT "fix" at integration):
//                  (1) :2995 and :3028 test `InArmSuck.Item[j][j]!=HAS_NULL_IC` where every
//                      neighbouring line uses [i][j].  With MachineType.h:387-388
//                      MAX_ARM_Row=2 / MAX_ARM_Col=4 the loop runs j=0..3, so j=2 and j=3
//                      index PAST Item's row bound -- an in-object out-of-range read of
//                      TMyKitSuck (garbage, not a segfault) -- and the HAS_NULL_IC exclusion
//                      silently misfires for the whole non-one-kit path.  Preserved.
//                  (2) the iMoveDownAdd200 one-kit branch (:3017) adds 200 to
//                      iInArmPreciserPickUpZ while its whole-grid twin (:3031) adds 200 to
//                      iInArmPreciserPlaceZ.  Asymmetric in golden; preserved.
//                  (3) the else-branch comment at :3037 says `if(iAction==iInitialFlag)` but
//                      the branch is a bare else, so ANY unknown action parks at ZSafePos.
//  INTEGRATOR    : no stub to retire -- SetPrecisorZPos has no port definition and no
//                  header declaration (golden declares it nowhere either: its only callers
//                  are golden ainarm2.cpp:3130/:3161/:3259/:3302 inside
//                  DoInArmDevicePosPrecise, which is NOT in this wave).  Defined
//                  NON-static, exactly as golden, so no TRAP-1(d) static shadow.  Until
//                  DoInArmDevicePosPrecise lands this body has NO caller (TRAP-1(a));
//                  that is expected and is stated in the report.
//  VERIFY        : port ainarm2.cpp + the integrator-owned file-scope globals + all 10
//                  k6-ainarm2 parts pass  g++ -std=c++1z -fsyntax-only -Wall -Wextra
//                  CLEAN (exit 0) in a SCRATCH stitched copy, measured 20260811
//                  06:45:05 local.  A fragment cannot be syntax-checked on its own --
//                  the REAL stitched ainarm2.cpp is the main loop's check, not this.
//  SELF-CHECK    : brace delta 0 ; U+FFFD 0 (Chinese comment at golden :2966 transcribed
//                  character-for-character)
//==============================================================================
//==============================================================================
void SetPrecisorZPos(int iAction)                                               //Steven 20241009 : 重新整理Precisor Function
{
//    SetPrecisorZPos(iInitialFlag);

    if(iAction==iMoveDownPlace ||
       iAction==iMoveDownPick)
    {
        if(bOneKitPreciser==true)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(InArmSuck.Item[iOneKitPreciser][j]!=NULL_IC &&
                   InArmSuck.Item[iOneKitPreciser][j]!=HAS_NULL_IC)
                {
                    bPrecise_Z[iOneKitPreciser][j]=true;
                    if(iAction==iMoveDownPick)
                        iPrecise_Z[iOneKitPreciser][j]=Prod.iInArmPreciserPickUpZ[iOneKitPreciser][j];
                    else
                        iPrecise_Z[iOneKitPreciser][j]=Prod.iInArmPreciserPlaceZ[iOneKitPreciser][j];
                }
            }
        }
        else
        {
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(InArmSuck.Item[i][j]!=NULL_IC &&
                       InArmSuck.Item[j][j]!=HAS_NULL_IC)
                    {
                        bPrecise_Z[i][j]=true;
                        if(iAction==iMoveDownPick)
                            iPrecise_Z[i][j]=Prod.iInArmPreciserPickUpZ[i][j];
                        else
                            iPrecise_Z[i][j]=Prod.iInArmPreciserPlaceZ[i][j];
                    }
                }
            }
        }
    }
    else if(iAction==iMoveDownAdd200)
    {
        if(bOneKitPreciser==true)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(InArmSuck.Item[iOneKitPreciser][j]!=NULL_IC &&
                   InArmSuck.Item[iOneKitPreciser][j]!=HAS_NULL_IC)
                {
                    bPrecise_Z[iOneKitPreciser][j]=true;
                    iPrecise_Z[iOneKitPreciser][j]=Prod.iInArmPreciserPickUpZ[iOneKitPreciser][j]+200;
                }
            }
        }
        else
        {
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(InArmSuck.Item[i][j]!=NULL_IC &&
                       InArmSuck.Item[j][j]!=HAS_NULL_IC)
                    {
                        bPrecise_Z[i][j]=true;
                        iPrecise_Z[i][j]=Prod.iInArmPreciserPlaceZ[i][j]+200;
                    }
                }
            }
        }
    }
    else                                                                        //if(iAction==iInitialFlag)
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                bPrecise_Z[i][j]=false;
                iPrecise_Z[i][j]=ZSafePos;
            }
        }
    }
}

//------------------------------------------------------------------------------
//  DoInArmDevicePosPrecise()  --  golden ainarm2.cpp:3050-3421 (372 lines)
//  Wave PT-W7e, part k1-ainarm2.  Translator: AI(W7E-k1-ainarm2) 20260811
//
//  ROLE  The InArm PRECISER STATION state machine (Frank 20180410).  One pass per
//        kernel tick over `iInArmDevicePosPrecise'; returns true exactly once, at
//        case 2000, when the whole re-centring cycle is done.  Sequence:
//          1     decide one-kit vs two-kit precising (no Y pitch + 2 pick rows ->
//                one kit at a time), arm the open speed          -> 100
//          80    (one-kit second pass) re-open the precisor jaws  -> 100
//          100   Z back to plate-safe, then branch on USE_PRECISER / the two
//                Preciser-detect sensors                         -> 150 / 200
//          150   refuse to continue while an IC is still sitting in the Preciser
//          200   XY to the Preciser position (+ optional one-kit Y pitch offset,
//                + the auto-Y-pitch correction), compute the X/Y pitch vectors,
//                open the jaws, and sanity-check that the home sensor is OFF
//          300   Z down to the PLACE height                      -> 400
//          400   close the jaws onto the device (open->close span scaled by the
//                0.2mm/2.44mm tray tolerance and the operator offset), optionally
//                stopping for a manual offset entry              -> 420 / 1000
//          420   wait for the offset dialog, restart at 1 or retry 400
//          1000  mark every nozzle that did NOT go down as "already blown"
//                (FALLS THROUGH to 1050 -- golden has no break here)
//          1050  blow the devices off the nozzles onto the jaws   -> 1100
//          1100  Z down another 200 and return the nozzles to normal
//          1150  close the jaws the rest of the way (this is the actual centring)
//          1200  dwell
//          1300  Z down to the PICK height, then re-suck               -> 1310
//          1310  suck every nozzle back up; if any nozzle errored go 1400 to
//                re-open the jaws, else straight to 1500
//          1400  re-open the jaws after an error
//          1500  Z back to plate-safe, re-classify any dropped IC, and either
//                run the second kit (80) or finish (2000)
//          2000  clear bWaitPreciserFinish and return true
//        bWaitPreciserFinish is raised on EVERY entry (golden :3059) and only
//        cleared at 2000, which is what makes the other mechanisms wait.
//
//  WAVE SCOPE  DoInArmDevicePosPrecise  ACTIVE except ONE line  golden :3050-3421
//              (the only gate is k1-G3 at :3307; the gate k1-G2 that :3151-3152 was
//              going to get is WITHDRAWN -- see the GATE REGISTER, its premise died
//              inside the wave and that is recorded there rather than silently fixed).
//
//  GOLDEN BUGS FOUND -- ALL KEPT VERBATIM (do not "fix" on review):
//   (a) :3320 `flag=true;' in case 1310 is never set false anywhere in that case
//       (its sibling case 1050 does set it false at :3238).  So the
//       `if(flag==false) break;' at :3351-3352 is DEAD: case 1310 cannot wait for
//       a slow suck, it always falls through to the error scan in the same tick.
//   (b) :3337-3340 in case 1310 treats `Suck[i][j].Error==true' as "this nozzle is
//       done" (bDestroyflag=true), i.e. a vacuum error is swallowed as completion;
//       only the later Error scan at :3361 diverts to case 1400.
//   (c) :3216 case 1000 has NO `break' -- deliberate fall-through into case 1050
//       in the same tick.  Preserved exactly (do not add a break).
//   (d) :3212/:3312 `bDestroyflag[i][j]=!bPrecise_Z[i][j];' walks iMaxRow/iMaxCol
//       while bDestroyflag's initialiser at :3054 only names MAX_ARM_Row x
//       MAX_ARM_Col entries -- fine today, but the two bounds are independent.
//   (e) :3368 `if(Task!=1400)` reads Task right after a loop that may have set it
//       to 1400 -- the KevinCheng 20260521 comment says this is intentional
//       ("skip to case1500"), it just makes the loop's last write the decision.
//   (f) :3184 gates the manual-offset stop on AccessLevel>=1 only, so the
//       Sen[SnRKManualTStart] read happens on every 400 tick.
//
//  INTEGER / FLOAT NOTE (this is the trap that has cost this port before):
//   :3177-3179 and :3284-3285 are golden's own DOUBLE arithmetic --
//   dTrayTorence=((open-close)*0.2)/2.44 and dOffset=(open-close)*
//   (Offset.iPreciserOpen/244), where Offset.iPreciserOpen is a double
//   (cprod.h:252).  So `/244' here is NOT integer division and must stay double;
//   the result is then truncated by MotorMove(int) exactly as golden truncates it.
//   :3114 `iYPos+=(TestIF_File.dPreciserYPitch*100);' likewise truncates a double
//   into an int on assignment -- kept.  No float helper substituted anywhere.
//
//  HARD DEPENDENCIES -- DECLARED AND CALLED, NOT GATED (read this at integration)
//   Two functions this body calls live in THIS SAME GOLDEN FILE, a few dozen lines
//   outside my assigned range, and are on this wave's own target list
//   (tools/census/wave_targets.py ainarm2.cpp):
//     HD1  void SetPrecisorZPos(int iAction)      golden ainarm2.cpp:2966-3048 (83)
//     HD2  void PreciserPitchCalculate(int &iXPitch) golden ainarm2.cpp:3423-3457
//   Neither exists anywhere in the port yet.  I did NOT gate them, and that is a
//   deliberate, reported decision rather than an oversight:
//     * They are not absent cross-module dependencies -- they are in-file peers of
//       my range, and SetPrecisorZPos is the ONLY writer of bPrecise_Z[][] /
//       iPrecise_Z[][], i.e. of every Z target this SM hands to InArmZMoveDown /
//       InArmContinuousMove_9045.  Gating its four call sites (:3130, :3161, :3259,
//       :3302) would leave the file-scope bPrecise_Z all-false and iPrecise_Z all
//       zero, so the SM would sail through all four Z-down steps WITHOUT MOVING Z
//       and still report success -- a silent, safety-relevant behaviour loss, which
//       is worse than a loud failure.  Gating PreciserPitchCalculate would silently
//       pin the X pitch at the file-scope initial 12000 instead of the recipe value.
//     * So instead this part emits a GUARDED FORWARD DECLARATION of each (golden's
//       exact signature, no body, no `static' -- so it cannot become TRAP 1 shape
//       (d), and a sibling part's real definition redeclares compatibly).  If the
//       sibling parts for golden :2966 and :3423 are NOT part of this wave, the
//       stitch fails LOUDLY with `undefined reference to SetPrecisorZPos(int)' /
//       `...PreciserPitchCalculate(int&)' and the integrator must add them.  Both
//       are pure Item/Prod-grid computation with no HAL and total ~118 golden lines.
//     ABSENCE COMMANDS (exact):
//       rg -c -w SetPrecisorZPos D:/HT9045/HT9011UC_Cpp_V3.33.906.0        -> 0 files
//       rg -c -w PreciserPitchCalculate D:/HT9045/HT9011UC_Cpp_V3.33.906.0 -> 0 files
//     MEASURED AT: 2026-08-11T06:36:51+0800  (port .cpp/.h only; parts dir empty then).
//     *** TRAP 2 RE-RUN, 2026-08-11T06:48:10+0800 -- THE PREMISE IS ALREADY DEAD ***
//     Both bodies landed as SIBLING PARTS while this part was being written:
//       _w7e_parts/02966_SetPrecisorZPos.txt        `void SetPrecisorZPos(int iAction)'
//       _w7e_parts/03423_PreciserPitchCalculate.txt `void PreciserPitchCalculate(int &iXPitch)'
//     Signatures checked against these declarations: identical, non-static, external
//     linkage.  So the decision NOT to gate them was right, and the LOUD-failure risk
//     described above no longer exists.
//     KEEP BOTH DECLARATIONS ANYWAY -- one of them is now load-bearing: parts are
//     stitched in golden-line order, so 02966 lands BEFORE this part (its declaration
//     is redundant) but 03423 lands AFTER it, so without the forward declaration
//     `PreciserPitchCalculate(iPrecise_MoveXP)' at :3121 would not compile.  Removing
//     either one is only safe if the integrator also moves the corresponding body
//     above this range.
//   NOTE ON SetPrecisorZPos ITSELF (for whoever translates golden :2966): golden
//   :2995 and :3028 read `InArmSuck.Item[j][j]' where every neighbour reads
//   `[i][j]' -- a real golden bug that must be KEPT.  Flagging it here because my
//   range is its only caller.
//
//  GATE REGISTER
//   GATE k1-G2 -- *** WITHDRAWN.  NOT PRESENT IN THIS PART.  This entry is kept as
//   the record of a TRAP 2 expiry that was caught before delivery, not after. ***
//     It would have gated golden :3151-3152, inside case 200's home-sensor check:
//       bool bPreciNeedHome=true;
//       SetInArmHome(bPreciNeedHome);
//     WHY IT WAS DROPPED: re-listing the parts directory at 2026-08-11T06:48:10+0800
//     found _w7e_parts/01266_SetInArmHome.txt, a sibling part whose definition line is
//     `void SetInArmHome(bool bPrecisorNeedHome)' -- GOLDEN'S ARITY, non-static,
//     landing in THIS SAME TU and (golden-line order) BEFORE my range.  With it in the
//     TU the call site compiles: the header's zero-arg `SetInArmHome()' and the
//     sibling's `SetInArmHome(bool)' form an overload set and `SetInArmHome(bool)' is
//     the exact match.  Gating a call whose real body arrives in the same file in the
//     same wave would have been a defect, so the two lines are ACTIVE and faithful.
//     THE FORWARD DECLARATION below makes this independent of part ordering, and it
//     deliberately carries NO default argument (golden's .cpp definition has none
//     either): adding `=false' while aHotPlateSubstrate.h:906's zero-arg declaration
//     is still visible would make every existing `SetInArmHome()' call AMBIGUOUS.
//     INTEGRATION HAZARD THE INTEGRATOR STILL OWNS (this is the part that did NOT go
//     away): the tree then holds TWO functions -- the empty no-op
//     `void SetInArmHome() {}' at aHotPlateSubstrate.cpp:1067, which all 15 existing
//     zero-arg call sites (ainarm9045_2x4_16.cpp:1410/:1576/:2029, ainarm9045_2x8_32
//     .cpp:1909/:4553, ainarm_SearchPickPlate.cpp:1165, AutoClean/AutoClean.cpp:4277/
//     :4893, csystem.cpp:1491/:2786/:19938/:24159, Motor/mymotor.cpp:639,
//     RotateKit/aRotateKIT_In.cpp:3692) keep resolving to, and the real body.  A green
//     build proves nothing here -- this is exactly TRAP 1 shape (b).  Fixing it means
//     changing aHotPlateSubstrate.h:906 to golden arity WITH the default and deleting
//     the no-op, in one commit, which is what 01266_SetInArmHome.txt's own banner also
//     asks for.  MY range is the ONLY caller that passes the flag.
//     ABSENCE COMMAND still worth re-running (it is what made the gate look needed):
//       rg -n -e "SetInArmHome\s*\(\s*bool" --glob '*.cpp' --glob '*.h' D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//       -> 0 hits at 2026-08-11T06:36:51+0800 and STILL 0 at 06:46:03 (part files are
//          .txt, so they are invisible to that glob -- the parts DIRECTORY listing is
//          the check that matters during a wave, and that is the lesson here).
//
//   [historical text of the withdrawn gate, kept for the audit trail]
//     SIGNATURE MISMATCH, not a missing name.  Golden declares
//     `void SetInArmHome(bool bPrecisorNeedHome=false)' (golden ainarm2.h:136, body
//     golden ainarm2.cpp:1266 -- also on this wave's target list).  The port has
//     ONLY a ZERO-ARG `void SetInArmHome()' (declared aHotPlateSubstrate.h:906,
//     defined aHotPlateSubstrate.cpp:1067 as `{}'), and all 15 existing port call
//     sites pass no argument.  Passing the flag therefore does not compile, and
//     dropping the argument silently would be worse than gating: it would claim the
//     "precisor also needs homing" request was honoured when nothing carries it.
//     The DECLARATION of bPreciNeedHome is gated together with the call because the
//     variable exists solely to feed it.
//     WHY THE OFFLINE DEFAULT IS FAITHFUL: the port's only SetInArmHome IS an empty
//     no-op body, so even the zero-arg call would change nothing offline; the
//     ShowMyMessage, `Task=100;' and `break;' around it stay ACTIVE, so the SM still
//     refuses to advance and still re-runs case 100 exactly as golden does.
//     REAL-MACHINE DIFFERENCE: on hardware, golden re-homes the in-arm Z axes AND
//     asks for a precisor home before retrying; gated, the operator gets the same
//     "Precisor open but home sensor is on!" message and the SM loops back to case
//     100 without a home request -- so a genuinely mis-homed precisor would keep
//     failing the same check instead of being re-homed.  It fails SAFE (it will not
//     proceed with a bad precisor) but it will not self-recover.
//     ABSENCE COMMAND (exact):
//       rg -n -e "SetInArmHome\s*\(\s*bool" --glob '*.cpp' --glob '*.h' D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//       -> 0 hits (the only declaration is the zero-arg aHotPlateSubstrate.h:906).
//     MEASURED AT: 2026-08-11T06:36:51+0800.
//     TRAP 2/TRAP 3: golden :1266 WAS on this wave's target list and DID land (see the
//     WITHDRAWN header above), which is precisely why this gate no longer exists.
//     [end of historical text]
//
//   GATE k1-G3 -- golden :3307 `InArmSuck.ClearAllError();' inside case 1300.
//     TRAP 5 SHAPE, and the answer is still "stay gated".  A real body exists --
//     mykitsuck.cpp:2689 `void TMyKitSuck::ClearAllError()' (declared
//     mykitsuck.h:392) -- but it belongs to the OTHER TMyKitSuck.  This TU's
//     InArmSuck is the aHotPlateSubstrate.h:365 mirror (aHotPlateSubstrate.cpp:80),
//     the one 177 TUs use, and that class has NO ClearAllError.  mykitsuck.cpp is
//     DELIBERATELY not registered in the build (CMakeLists.txt:2099 "*** DELIBERATELY
//     NOT REGISTERED IN THIS WAVE: mykitsuck.cpp ***", :2105 notes both headers
//     declare the same extern globals with different layouts), so switching this TU
//     to mykitsuck.h would read every InArmSuck field at the wrong offset while
//     linking perfectly.  Reaching for it "just for one error-clear" is exactly the
//     dependency the MyMessageBox precedent says not to take.
//     WHY THE OFFLINE DEFAULT IS FAITHFUL: golden's ClearAllError only clears the
//     per-nozzle TMySucker::Error flags before the re-suck at :3333.  Offline
//     TMySucker::Suck()/Destroy() never SET Error (aHotPlateSubstrate.h:109 is only
//     ever read in this file's paths), so there is nothing to clear and the
//     following case-1310 scan behaves identically.
//     REAL-MACHINE DIFFERENCE: on hardware a vacuum error latched during the blow-off
//     phase (cases 1050/1100) would survive into case 1310; combined with golden bug
//     (b) above that nozzle is then marked done AND diverts the SM to case 1400
//     (jaw re-open) even though the re-suck never happened.  Net effect: an extra
//     jaw-open/close and a device that may not be held.  This is the one gate here
//     with a real hardware consequence -- it must be retired together with the
//     TMyKitSuck unification (task #-shaped work, same as GATE k1-G1's).
//     ABSENCE COMMAND (exact):
//       rg -n -w ClearAllError --glob '*.cpp' --glob '*.h' D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//       -> 4 hits, ALL in mykitsuck.{h:392,cpp:112,cpp:2689,cpp:2749}; ZERO in
//          aHotPlateSubstrate.h.
//       rg -n "mykitsuck.cpp" CMakeLists.txt  -> only the "NOT REGISTERED" comment.
//     MEASURED AT: 2026-08-11T06:36:51+0800.
//
//  TRAP 4 STATEMENT: this part adds NO file-scope object of any kind -- the only
//  static it introduces is golden's own FUNCTION-LOCAL `static bool bDestroyflag
//  [MAX_ARM_Row][MAX_ARM_Col]' (:3054), a POD with a constant initialiser, so no
//  constructor runs before main() and no NULL global can be touched.  fMain is
//  dereferenced only inside case 400 (:3187), at run time, exactly as golden does,
//  and the existing tree already relies on that (acarry.cpp:3994, acatchtray.cpp:7007,
//  ainarm9045.cpp:6127).
//
//  TRAP 5 STATEMENT: InArmSuck / TMySucker come from aHotPlateSubstrate.h (the
//  :365 TMyKitSuck, object defined aHotPlateSubstrate.cpp:80).  NOT mykitsuck.h.
//  No TInLaserCheck is touched.
//
//  TRAP 1 STATEMENT: nothing here is `static' at file scope; the definition is a
//  plain external one, so it can only be reached by resolving the real symbol.
//  There is no pre-existing stub of DoInArmDevicePosPrecise in the tree -- its one
//  call site (ainarm9045.cpp:1203) sits inside `#if 0 // TODO(W7)' (ainarm9045.cpp
//  :1192), so shape (b) cannot hide this body: retiring that gate is what makes it
//  reachable, and until then this body has NO live caller (shape (a)).  Say so out
//  loud rather than let a green build imply otherwise.
//    rg -n -w DoInArmDevicePosPrecise --glob '*.cpp' --glob '*.h' D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//    -> 3 hits, all in ainarm9045.cpp (:1119 comment, :1192 gate text, :1203 the
//       gated call).  MEASURED AT 2026-08-11T06:33:56+0800.
//
//  Big5: TWENTY non-ASCII golden lines in this range -- :3051, :3059, :3062, :3081,
//  :3086, :3099, :3108, :3116, :3132, :3142, :3160, :3169, :3170, :3176, :3277,
//  :3278, :3368, :3373, :3376, :3415 (measured, not eyeballed) -- transcribed
//  character for character from cp950, including the Chinese STRING LITERAL passed
//  to ShowMyMessage at :3099.  ZERO U+FFFD.
//
//  INCLUDES this part needs (each one MEASURED with a scratchpad probe TU -- g++
//  6.3 -std=c++1z -fsyntax-only over port ainarm2.cpp + this part, first run
//  2026-08-11T06:47:12+0800 gave exactly these five "was not declared in this
//  scope" errors and nothing else; adding them cleared the TU).  HOIST ME to the
//  ainarm2.cpp include head at integration -- they are mid-file only because a part
//  file may not touch the head, the same convention ainarm9045.cpp:3057/:5389/
//  :6091/:10607 already uses.
//    "mysensor.h"      Sen[] / TMySensor::Enable / IsOn()  (mysensor.h:42/44/48)
//                      -- guarded header, pulls only vclcompat + MachineType.h, so
//                      it cannot swap a same-named class into this TU (TRAP 5 check:
//                      it does NOT include mykitsuck.h).
//    "canary_support.h" ShowMyMessage(AnsiString,...)      (canary_support.h:80)
//    "FormsFacade.h"    fMain->Pause(AnsiString)           (forms/fMain.h:142)
//    "csystem.h"        CheckInArmSuckICFallDownToHasNullIC(bool bAlarm=true)
//                                                          (csystem.h:99)
//------------------------------------------------------------------------------
#include "mysensor.h"               // AI(W7E-k1-ainarm2) 20260811: Sen[]/SnPreciserDetect1|2/SnRKManualTStart -- golden mysensor.h:48.  HOIST ME to the include head.
#include "canary_support.h"         // AI(W7E-k1-ainarm2) 20260811: ShowMyMessage (golden mymessbox.h:58) for golden :3099/:3150.  HOIST ME.
#include "FormsFacade.h"            // AI(W7E-k1-ainarm2) 20260811: fMain->Pause (golden main.h:1249) for golden :3187.  HOIST ME.
#include "csystem.h"                // AI(W7E-k1-ainarm2) 20260811: CheckInArmSuckICFallDownToHasNullIC (golden csystem.h:99) for golden :3392.  HOIST ME.
//------------------------------------------------------------------------------
// HD1/HD2 forward declarations -- golden's own signatures (golden ainarm2.cpp:2966
// and :3423; golden needs no declaration there only because both bodies precede
// this one in golden's file order, which the port's append-only stitch does not
// guarantee).  Guarded so a sibling part's identical declaration is harmless; a
// declaration is NOT a definition, so no ODR/duplicate-symbol risk either way.
#ifndef HT9045_SetPrecisorZPos_DECLARED
#define HT9045_SetPrecisorZPos_DECLARED
void SetPrecisorZPos(int iAction);                                              // golden ainarm2.cpp:2966
#endif
#ifndef HT9045_PreciserPitchCalculate_DECLARED
#define HT9045_PreciserPitchCalculate_DECLARED
void PreciserPitchCalculate(int &iXPitch);                                      // golden ainarm2.cpp:3423
#endif
// SetInArmHome at GOLDEN'S arity (golden ainarm2.h:136 declares it with a default
// argument; golden's DEFINITION at ainarm2.cpp:1266 carries none, and neither does
// this -- see the WITHDRAWN GATE k1-G2 note above for why adding `=false' here would
// make every existing zero-arg call ambiguous against aHotPlateSubstrate.h:906).
// Body: sibling part _w7e_parts/01266_SetInArmHome.txt, same TU.
#ifndef HT9045_SetInArmHome_BOOL_DECLARED
#define HT9045_SetInArmHome_BOOL_DECLARED
void SetInArmHome(bool bPrecisorNeedHome);                                      // golden ainarm2.cpp:1266
#endif
//------------------------------------------------------------------------------
bool DoInArmDevicePosPrecise()                                                  //Frank 20180410 (Steven) : InArm Preciser Station
{                                                                               //Steven 20241009 : 重新整理Precisor Function
    int &Task=iInArmDevicePosPrecise;

    static bool bDestroyflag[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
    double dOffset=0, dTrayTorence=0;
    int iXPos=0, iYPos=0;
    bool flag;

    bWaitPreciserFinish=true;                                                   //Ifor 20180703 (Steven) : : 等待 Preciser 完成
    switch(Task)
    {
        case 1:                                                                 //決定哪幾隻要下去做Precise
            if(TestIF_File.dPreciserYPitch==0 &&
               InArmSuck.iPickRow==2)
                bOneKitPreciser=true;
            else
                bOneKitPreciser=false;

            iOneKitPreciser=0;
            MOT[MPreciser].SetSpeed(ArmSpeed_File[0].PrecisorOpenSp);           //JerryYang 20250120 : add
            Task=100;
            break;
        case 80:
            if(DoInArmDevicePosPreciseDelay.Off())
            {
                bPreciseFlag=MOT[MPreciser].MotorMove(Prod.iPreciserOpenPitch);
                if(bPreciseFlag)
                    Task=100;
            }
            break;
        case 100:                                                               //Z軸回Safe位置
            if(MoveInArmZToPlateSafe(Task))
            {
                bPreciseFlag=false;
                bPrecise_XYZ=false;
                if(USE_PRECISER==2)                                             //Steven 20191113 : 使用HP當Preciser
                    Task=200;
                else if(Sen[SnPreciserDetect1].Enable==true ||
                        Sen[SnPreciserDetect2].Enable==true)
                    Task=150;
                else
                    Task=200;
            }
            break;
        case 150:
            if((Sen[SnPreciserDetect1].Enable && Sen[SnPreciserDetect1].IsOn()==false) ||
               (Sen[SnPreciserDetect2].Enable && Sen[SnPreciserDetect2].IsOn()==false))
            {
                ShowMyMessage("請將Preciser內IC移除");
            }
            else
            {
                bPreciseFlag=false;
                bPrecise_XYZ=false;
                Task=200;
            }
            break;
        case 200:                                                               //移動至Precise 位置  Precise打開 X Pitch Y Pitch 開合完成
            iXPos=Prod.iInArmPreciserX+InArmOffSet[InOfsPreciser]->GetX();
            iYPos=Prod.iInArmPreciserY+InArmOffSet[InOfsPreciser]->GetY();

            iPrecise_MoveYP=TestIF_File.iARM_Y_PITCH;
            if(bOneKitPreciser && iOneKitPreciser==1)
                iYPos+=(TestIF_File.dPreciserYPitch*100);

            if(USE_IN_Y_IS_AUTO_PITCH==true)                                    //Steven 20131002 : XY變距  //JerryYang 20251218 : IN/OUT ARM支援不同模組
            {
                iYPos=(iYPos-TestIF_File.iARM_Y_PITCH);
            }

            PreciserPitchCalculate(iPrecise_MoveXP);
            if(iPrecise_MoveXP>12000)                                           //JerryYang 20250120 : add
            {
                iPrecise_MoveXP=12000;
            }
            for(int i=0; i<X_PITCH_COUNT; i++)
                iPrecise_XP[i]=GetInArmPitchX_9045(iPrecise_MoveXP, i, InOfsPreciser);
            iPrecise_YP=GetInArmPitchY_9045(iPrecise_MoveYP, InOfsPreciser);

            SetPrecisorZPos(iInitialFlag);

            if(USE_PRECISER==2)                                                 //Steven 20191113 : 使用HP當Preciser
                bPreciseFlag=true;
            else if(bPreciseFlag==false)
                bPreciseFlag=MOT[MPreciser].MotorMove(Prod.iPreciserOpenPitch);

            if(bPrecise_XYZ==false)
                bPrecise_XYZ=InArmContinuousMove_9045(iXPos, iYPos, iPrecise_XP, iPrecise_YP, bPrecise_Z, iPrecise_Z, false);

            if(bPrecise_XYZ && bPreciseFlag)
            {
                if(USE_PRECISER!=2)                                             //KevinCheng 20260521 : HP當Preciser時不檢查Sensor
                {
                    MOT[MPreciser].ScanMotorStatus();

                    MOT[MPreciser].SetSpeed(ArmSpeed_File[0].PrecisorCloseSp);      //JerryYang 20250120 : add

                    if(MOT[MPreciser].Led[iHomeLed]==true)
                    {
                        ShowMyMessage("Precisor open but home sensor is on!");
                        bool bPreciNeedHome=true;
                        SetInArmHome(bPreciNeedHome);
                        Task=100;
                        break;
                    }
                }
                Task=300;
            }
            break;
        case 300:                                                               //使用的Z軸下降
            SetPrecisorZPos(iMoveDownPlace);

            bPrecise_XYZ=InArmZMoveDown(bPrecise_Z, iPrecise_Z, false);
            if(bPrecise_XYZ==true)
            {
                Task=400;
            }
            break;
        case 400:                                                               //Precise 夾持
            if(USE_PRECISER==2)                                                 //Steven 20191113 : 使用HP當Preciser
            {
                bPreciseFlag=true;
            }
            else
            {
//              0.2mm/2.44mm=x/(Prod.iPreciserOpenPitch-Prod.iPreciserClosePitch)   //0.2mm為20條 = Tray盤Torence
                dTrayTorence=((Prod.iPreciserOpenPitch-Prod.iPreciserClosePitch)*0.2)/2.44;
                dOffset     =(Prod.iPreciserOpenPitch-Prod.iPreciserClosePitch)*(Offset.iPreciserOpen/244);
                bPreciseFlag=MOT[MPreciser].MotorMove(Prod.iPreciserClosePitch+dTrayTorence+dOffset);
            }

            if(bPreciseFlag)
            {
                if(AccessLevel>=1 && Sen[SnRKManualTStart].IsOn())
                {
                    bEnterOffset=false;
                    fMain->Pause("DoInArmDevicePosPrecise");
                    Task=420;
                    break;
                }

                DoInArmDevicePosPreciseDelay.SetMSAndOn(100);
                Task=1000;
            }
            break;
        case 420:
            if(bEnterOffset==true)
            {
                bEnterOffset=false;
                Task=1;
            }
            else
            {
                Task=400;
            }
            break;
        case 1000:
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    bDestroyflag[i][j]=!bPrecise_Z[i][j];
                }
            }
            Task=1050;
        case 1050:
            if(DoInArmDevicePosPreciseDelay.Off())
            {
                flag=true;
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if((bOneKitPreciser==false) ||
                           (bOneKitPreciser==true && i==iOneKitPreciser))
                        {
                            if(bDestroyflag[i][j]==false)
                            {
                                if(InArmSuck.Item[i][j]!=NULL_IC &&
                                   InArmSuck.Item[i][j]!=HAS_NULL_IC)
                                {
                                    if(InArmSuck.Suck[i][j].Destroy())
                                    {
                                        bDestroyflag[i][j]=true;
                                    }
                                    else if(InArmSuck.Suck[i][j].Error==false)
                                    {
                                        flag=false;
                                    }
                                }
                                else
                                {
                                    bDestroyflag[i][j]=true;
                                }
                            }
                        }
                    }
                }

                if(flag==false)
                    break;
                DoInArmDevicePosPreciseDelay.SetMSAndOn(100);
                Task=1100;
            }
            break;
        case 1100:
            if(DoInArmDevicePosPreciseDelay.Off())
            {
                SetPrecisorZPos(iMoveDownAdd200);
                bPrecise_XYZ=InArmZMoveDown(bPrecise_Z, iPrecise_Z, false);

                if(bPrecise_XYZ==true)
                {
                    for(int i=0; i<InArmSuck.iMaxRow; i++)
                    {
                        for(int j=0; j<InArmSuck.iMaxCol; j++)
                        {
                            if(bPrecise_Z[i][j]==true)
                                InArmSuck.Suck[i][j].Normal();
                        }
                    }

                    Task=1150;
                }
            }
            break;
        case 1150:                                                              //Precise 夾持
            if(USE_PRECISER==2)                                                 //Steven 20191113 : 使用HP當Preciser
            {
                bPreciseFlag=true;
            }
            else
            {
                dOffset     =(Prod.iPreciserOpenPitch-Prod.iPreciserClosePitch)*(Offset.iPreciserClose/244);
                bPreciseFlag=MOT[MPreciser].MotorMove(Prod.iPreciserClosePitch+dOffset);
            }

            if(bPreciseFlag)
            {
                DoInArmDevicePosPreciseDelay.SetMSAndOn(100);
                bPreciseFlag=false;
                Task=1200;
            }
            break;
        case 1200:
            if(DoInArmDevicePosPreciseDelay.Off())
            {
                Task=1300;
            }
            break;
        case 1300:
            SetPrecisorZPos(iMoveDownPick);
            bPrecise_XYZ=InArmZMoveDown(bPrecise_Z, iPrecise_Z, false);

            if(bPrecise_XYZ==true)
            {
#if 0 // GATE k1-G3 -- golden :3307 InArmSuck.ClearAllError(): exists only on the OTHER TMyKitSuck (mykitsuck.h:392 / .cpp:2689, and mykitsuck.cpp is deliberately not in the build -- CMakeLists.txt:2099); this TU's InArmSuck is the aHotPlateSubstrate.h:365 mirror, which has no such member.  Offline the per-nozzle Error flags are never set, so there is nothing to clear.  See GATE REGISTER above.
                InArmSuck.ClearAllError();                                      //Steven 20141107 : Add for Loader pickup Error
#endif // GATE k1-G3
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        bDestroyflag[i][j]=!bPrecise_Z[i][j];
                    }
                }

                Task=1310;
            }
            break;
        case 1310:
            flag=true;
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if((bOneKitPreciser==false) ||
                       (bOneKitPreciser==true && i==iOneKitPreciser))
                    {
                        if(bDestroyflag[i][j]==false)
                        {
                            if(InArmSuck.Item[i][j]!=NULL_IC &&
                               InArmSuck.Item[i][j]!=HAS_NULL_IC)
                            {
                                if(InArmSuck.Suck[i][j].Suck())
                                {
                                    bDestroyflag[i][j]=true;
                                }
                                else if(InArmSuck.Suck[i][j].Error==true)
                                {
                                    bDestroyflag[i][j]=true;
                                }
                            }
                            else
                            {
                                bDestroyflag[i][j]=true;
                            }
                        }
                    }
                }
            }

            if(flag==false)
                break;

            DoInArmDevicePosPreciseDelay.SetMSAndOn(100);
            bPreciseFlag=false;

            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(InArmSuck.Suck[i][j].Error)
                    {
                        Task=1400;
                    }
                }
            }

            if(Task!=1400)                                                      //KevinCheng 20260521 : 補到case1500的路徑
            {
                Task=1500;
            }
            break;
        case 1400:                                                              //Precise 打開
            if(DoInArmDevicePosPreciseDelay.Off())
            {
                if(USE_PRECISER==2)                                             //Steven 20191113 : 使用HP當Preciser
                    bPreciseFlag=true;
                else
                    bPreciseFlag=MOT[MPreciser].MotorMove(Prod.iPreciserOpenPitch);

                if(bPreciseFlag)
                {
                    bPreciseFlag=false;
                    Task=1500;
                }
            }
            break;
        case 1500:
            if(MoveInArmZToPlateSafe(Task))
            {
                DoInArmDevicePosPreciseDelay.SetMSAndOn(100);
                CheckInArmSuckICFallDownToHasNullIC();
                bPreciseFlag=false;
                if(bOneKitPreciser==true)
                {
                    if(iOneKitPreciser==0)
                    {
                        iOneKitPreciser=1;
                        Task=80;
                    }
                    else
                    {
                        Task=2000;
                    }
                }
                else
                {
                    Task=2000;
                }
            }
            break;
        case 2000:
            if(DoInArmDevicePosPreciseDelay.Off())
            {
                bWaitPreciserFinish=false;                                      //Ifor 20180703 : 等待 Preciser 完成
                return true;
            }
            break;
    }
    return false;
}

// =============================================================================
//  AI(W906-PT-W7e-k8) 20260811 -- golden ainarm2.cpp translation part
//  golden file : D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/ainarm2.cpp (cp950, CRLF)
//  this part   : golden lines 3423..3457
// -----------------------------------------------------------------------------
//  ROLE        : compute the InArm Preciser-station X pitch, in 0.01 mm motor units, for the current test mode and write it through the int& out-parameter.
//  WAVE SCOPE  : PreciserPitchCalculate -- ACTIVE, golden ainarm2.cpp:3423..3457.
//  COLLISION    : none -- no port body, no port declaration, no port caller.
//  INCLUDES     : none beyond the current set.  TestIF / TestIF_File are cprod.h:2577
//              : and dPreciserXPitch is cprod.h:1663, a double.
//  GATE REGISTER: none -- every symbol resolves.
//  ARITHMETIC IS LOAD-BEARING, in three separate places, all transcribed verbatim:
//              : (1) golden :3425 `int iPreciserXPitch=TestIF_File.dPreciserXPitch*100;`
//              :     -- double*100 TRUNCATED to int, so 40.99 mm becomes 4099, not 4100.
//              : (2) golden :3439 `iPreciserXPitch+iPreciserXPitch/2` -- INTEGER
//              :     division; an odd value loses its half count right here.  NOT
//              :     converted to any float helper: a previous float-helper
//              :     substitution in this campaign broke rounding and produced stepped
//              :     position drift.
//              : (3) golden :3455 `iXPitch=iPreciserXPitch*1.5;` -- int = int*double,
//              :     truncated.  Left exactly as written; NOT rewritten as *3/2.
//  GOLDEN QUIRK : the `else if(TestIF.iTestMode==DualSite2x1)` arm at :3441-:3444 has an
//              : EMPTY body whose only content is the commented-out `// return false;`
//              : -- inside a void function.  It exists purely to stop DualSite2x1 from
//              : falling into the DualSite arm below, so iXPitch keeps the plain
//              : iPreciserXPitch assigned at :3427.  Transcribed including the dead
//              : comment; the commented-out `return false;` is NOT reinstated.
//  TRAP 1 / TRAP 2 -- MY OWN ABSENCE CLAIM HERE EXPIRED INSIDE THE WAVE, AND THIS IS
//              : THE CORRECTED TEXT.  At 2026-08-11 06:20 local `rg -w
//              : PreciserPitchCalculate D:/HT9045/HT9011UC_Cpp_V3.33.906.0` returned 0
//              : hits and I was about to report this as TRAP 1 shape (a), nobody
//              : references it.  Re-checked at 2026-08-11 06:43 local against the part
//              : directory and that was already FALSE: sibling part
//              : _w7e_parts/03050_DoInArmDevicePosPrecise.txt had landed golden's only
//              : call site (golden ainarm2.cpp:3121) AND a guarded forward declaration
//              : `void PreciserPitchCalculate(int &iXPitch);` of exactly this signature,
//              : deliberately un-gated so a missing body fails loudly.  So after the
//              : stitch this body IS referenced from inside the same TU, the archive
//              : member IS extracted, and its int& out-parameter really does drive that
//              : SM's iPrecise_MoveXP.  The lesson recorded, not just the fact: an
//              : "unreferenced" claim about an in-file peer is the shortest-lived kind
//              : of absence claim there is -- re-run it at integration anyway.
// =============================================================================
void PreciserPitchCalculate(int &iXPitch)
{
    int iPreciserXPitch=TestIF_File.dPreciserXPitch*100;

    iXPitch=iPreciserXPitch;
    if(TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==_8Site2X4 ||
       TestIF.iTestMode==_16Site2X8  || TestIF.iTestMode==_12Site2X6 ||
       TestIF.iTestMode==TriSite1X3  ||                                         //Frank 20160329 add for 1x3_4
       TestIF.iTestMode==_6Site2X3   ||                                         //ChungHung 20140115 add for 2x3
       TestIF.iTestMode==_8Site1X4   ||                                         //ChungHung 20150528 add for 海思 _8Site1x4
       TestIF.iTestMode==_16Site4X4  ||                                         //Sam 20190226 : 16Site4X4
       TestIF.iTestMode==_32Site4X8N)                                           //2013-01-15    Dell    Add nn Mode
    {
        if(TestIF_File.dPreciserXPitch<41.0)                                    //Steven 20210412 : 修正Preciser的Pitch錯誤問題
            iXPitch=iPreciserXPitch*3;
        else
            iXPitch=iPreciserXPitch+iPreciserXPitch/2;
    }
    else if(TestIF.iTestMode==DualSite2x1)
    {
//        return false;
    }
    else if(TestIF.iTestMode==QualSite2X2)
    {
        if(TestIF_File.dPreciserXPitch<41.0)                                    //Steven 20210412 : 修正Preciser的Pitch錯誤問題
            iXPitch=iPreciserXPitch*3;
    }
    else if(TestIF.iTestMode==DualSite)                                         //Steven 20150505 : 1x2加大支援X-Pitch 120mm
    {
        if(iPreciserXPitch>8000)
            iXPitch=12000;
        else
            iXPitch=iPreciserXPitch*1.5;
    }
}

//==============================================================================
//== ROLE: DoVibrateOutShuttle -- the out-shuttle vibration-motor state machine,
//==       one task cursor per shuttle.  Step 1 switches the vibrator on, arms the
//==       dwell for IniConfig.iF25VibrateTime*100 ms and accumulates the motor
//==       run-time counter; step 100 switches it off and arms a fixed 1000 ms
//==       settle; step 200 ends the cycle by returning true.  Returns true only
//==       once, at the end of a completed vibrate-and-settle cycle.
//==
//== WAVE SCOPE (k2-ainarm2):
//==   DoVibrateOutShuttle  golden ainarm2.cpp:3528-3583  -- ACTIVE
//==     (golden lines emitted VERBATIM; ZERO gates)
//==
//== GATE REGISTER: (none -- this function needed no #if 0)
//==   * SW[] -- extern class TMySwitch SW[MAX_SWITCH_ITEM], myswitch.h:43; On()
//==     myswitch.h:32, Off() myswitch.h:33.  Reached through the extra include
//==     block emitted by part 00320.
//==   * SwShuttleVibration1 / SwShuttleVibration2 -- extern const int,
//==     cmydef.h:1872/1873.
//==   * IniConfig.iF25VibrateTime Config.h:694, .iVibratorSht1 Config.h:249,
//==     .iVibratorSht2 Config.h:250 (object Config.h:1499).
//==   * iVibrateOutShuttleTask[2] -- golden ainarm2.cpp:3527, i.e. ON THE MAIN
//==     LOOP'S file-scope-globals list.  This part does NOT emit it (two agents
//==     emitting one global is a multiple-definition link error); it just uses it.
//==   * VibrationOutShuttleDelay[2] -- golden ainarm2.cpp:88, TQPF_Timer, also on
//==     the main loop's globals list and likewise NOT emitted here.  It is already
//==     TQPF_Timer in golden, so it is NOT one of the two HTimer cases the
//==     integrator has to retype: .SetMSAndOn(DWORD) is myTimer.h:38 and .Off() is
//==     myTimer.h:30, a REAL timer whose Off() actually measures elapsed time.  If
//==     it were mis-declared as the atester_shims.h:463 HTimer instead, that Off()
//==     is hard-coded true and BOTH dwells here (the iF25VibrateTime one and the
//==     fixed 1000 ms settle) would silently collapse to zero.
//==
//== GOLDEN QUIRKS PRESERVED (not fixed):
//==   * golden :3551 and :3555 compute int(IniConfig.iF25VibrateTime/10.0) --
//==     FLOATING division by 10.0 on purpose, then truncated by the int cast.
//==     Transcribed EXACTLY.  It is deliberately NOT rewritten as /10, and no
//==     integer division exists anywhere in this function, so the known
//==     float-helper-substitution regression cannot apply here.
//==   * case 150 (golden :3568-:3573) is UNREACHABLE -- nothing in golden ever
//==     assigns 150 to iVibrateOutShuttleTask.  Kept verbatim, break intact.
//==   * iShuttleVibraCount (golden :3539) is incremented at :3558 and zeroed at
//==     :3577 but never read anywhere.  Kept, including its function-local static
//==     storage.
//==   * AnsiString str; (golden :3537) is never used.  Kept verbatim.  MEASURED:
//==     it does NOT produce an unused-variable warning under -Wall -Wextra,
//==     because AnsiString has a non-trivial constructor, so GCC assumes the
//==     declaration has a side effect.  Likewise iShuttleVibraCount produces no
//==     -Wunused-but-set-variable because it has static storage.
//==   * The two statics bInitial and iShuttleVibraCount are FUNCTION-LOCAL, which
//==     is golden's design: bInitial is NOT per-iShuttle, so the first call for
//==     shuttle 1 after shuttle 0 has initialised does NOT re-initialise -- callers
//==     pass bNeedInitial=true to force it.  Being function-local they create no
//==     TRAP 1 shape (d) file-scope static shadow.
//==
//== INTEGRATION NOTE -- TRAP 1 shapes (a) AND (e), THREE STEPS:
//==   grep -rn --include=*.cpp --include=*.h "DoVibrateOutShuttle" D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//==   -- ran 2026-08-11 06:16 +0800: no real body anywhere; every hit is a
//==   comment, a gate, or the renamed stand-in below.
//==   (1) ADD the declaration -- golden ainarm2.h:215 is
//==       extern bool DoVibrateOutShuttle(int iShuttle, bool bNeedInitial=false);
//==       The default argument matters: golden calls the one-argument form.  It
//==       belongs on the DECLARATION only -- golden's definition at :3528 carries
//==       no default and this part matches that.
//==   (2) aTester_Rear.cpp:392 holds static bool W64bT2_DoVibrateOutShuttle(int,
//==       bool=false) -- a RENAMED TU-local stand-in, TRAP 1 shape (e) inside the
//==       engine rather than inside tests.  It does not collide with this
//==       definition, which is precisely the danger: the build stays green and
//==       aTester_Rear.cpp:704 and :774 keep calling the stand-in forever.  Delete
//==       it and repoint both call sites.
//==   (3) aTester_Front.cpp:602 and :671 hold the same two calls under
//==       #if 0 // TODO(W7) with the reason "ainarm2.h, no translated home yet".
//==       That premise dies with this part; retire both.
//==   Not static -- once the declaration lands it is non-static (shape (d)
//==   avoided).
//==
//== TRAP 4 ANSWER: no file-scope object is added by this part.
//==
//== Big5 -> UTF-8: the Chinese comments on golden :3528, :3549, :3561, :3569 and
//== :3575 are transcribed character for character from cp950.  Zero U+FFFD.
//==============================================================================
bool DoVibrateOutShuttle(int iShuttle, bool bNeedInitial)                       //JerryYang 20190123 shuttle震動馬達
{
    static bool bInitial=true;
    if(bInitial || bNeedInitial)
    {
        iVibrateOutShuttleTask[iShuttle]=1;
        bInitial=false;
        return false;
    }
    AnsiString str;
    int iShuttleVibration=0;
    static int iShuttleVibraCount=0;
    if(iShuttle==0)
        iShuttleVibration=SwShuttleVibration1;
    else
        iShuttleVibration=SwShuttleVibration2;
    switch(iVibrateOutShuttleTask[iShuttle])
    {
        case 1:
            SW[iShuttleVibration].On();
            VibrationOutShuttleDelay[iShuttle].SetMSAndOn(IniConfig.iF25VibrateTime*100);
            if(iShuttle==0)                                                     //JerryYang 20200612 振動馬達作動時間累計
            {
                IniConfig.iVibratorSht1=IniConfig.iVibratorSht1+int(IniConfig.iF25VibrateTime/10.0);
            }
            else
            {
                IniConfig.iVibratorSht2=IniConfig.iVibratorSht2+int(IniConfig.iF25VibrateTime/10.0);
            }
            iVibrateOutShuttleTask[iShuttle]=100;
            iShuttleVibraCount++;
            break;
        case 100:
            if(VibrationOutShuttleDelay[iShuttle].Off())                        //JerryYang 20171205 (Steven) shuttle震動馬達功能
            {
                SW[iShuttleVibration].Off();
                iVibrateOutShuttleTask[iShuttle]=200;
                VibrationOutShuttleDelay[iShuttle].SetMSAndOn(1000);
            }
            break;
        case 150:
            if(VibrationOutShuttleDelay[iShuttle].Off())                        //JerryYang 20171205 (Steven) shuttle震動馬達功能
            {
                iVibrateOutShuttleTask[iShuttle]=1;
            }
            break;
        case 200:
            if(VibrationOutShuttleDelay[iShuttle].Off())                        //JerryYang 20171212 (Steven) 關閉震動後先delay 1秒,避免shuttle馬上移動IC會飛走
            {
                iShuttleVibraCount=0;
                return true;
            }
            break;
    }
    return false;
}

//------------------------------------------------------------------------------
//  EnableTraymapCheckFunction(int iCheck)  --  golden ainarm2.cpp:3585-3610
//  Wave PT-W7e, part k1-ainarm2.  Translator: AI(W7E-k1-ainarm2) 20260811
//
//  ROLE  One predicate, two questions selected by iCheck, both meaning "is the
//        tray-map DATA check feature live right now?":
//          iCheck==0 -> ... && TestIF_File.bEnableSuckMapCheck   (do the check)
//          iCheck==1 -> ... && bTrayMapDataCheckErrorSkip        (auto-SKIP on a
//                                                                 data mismatch)
//        Both share the same three preconditions: tray-ID running (bRTNoRunTrayID
//        false), tray mapping installed (USE_TRAY_MAPPING != etmUninstall), and a
//        recipe that enables either the tray map or tray-ID-with-FTP-download.
//        Any other iCheck returns the initial false.
//        Golden's header gives it a default argument (`int iCheck=0',
//        golden ainarm2.h:216) -- the DEFINITION carries none, exactly as here.
//
//  WAVE SCOPE  EnableTraymapCheckFunction  ACTIVE  golden :3585-3610.
//              CheckTrayMapData (golden :3612) is NOT in this part.
//
//  GATE REGISTER  none -- every symbol is real and verified by header+line:
//    bRTNoRunTrayID cmydef.h:4869, USE_TRAY_MAPPING cmydef.h:3006, etmUninstall
//    MachineType.h:910 (enum eTrayMap), TestIF_File.bEnableTrayMap cprod.h:2253,
//    .bEnableTrayID cprod.h:2254, .bEnableTrayIDDownFTP cprod.h:2265,
//    .bEnableSuckMapCheck cprod.h:2264, bTrayMapDataCheckErrorSkip cmydef.h:4263.
//
//  *** THIS PART EXPIRES FIVE EXISTING GATES (TRAP 3 -- and here the premise
//  GENUINELY dies, so they should be RETIRED, not rewritten) ***
//  Five #if 0 blocks in the tree are gated on exactly "EnableTraymapCheckFunction
//  has no port body and no port declaration".  Landing this body kills that
//  premise.  They must be re-examined together (all five call it with a literal
//  0 and/or 1, nothing else):
//    ainarm9045.cpp:5110  GATE W7D-K2-G05   golden ainarm9045.cpp:2637-2648
//    ainarm9045.cpp:5546  GATE W7D-G01      golden ainarm9045.cpp:2900
//    ainarm9045.cpp:10423 GATE k6-G4        golden ainarm9045.cpp:7132-7137
//    aTester_Rear.cpp:9793   TODO(G18)
//    aTester_Front.cpp:9863  GATE W7F3-G03
//  (ainarm9045.cpp:10255 already predicts this: "retired together when ainarm2.cpp's
//  EnableTraymapCheckFunction lands".)  Note W7D-K2-G05 also gates MyDeCodeASCII,
//  so THAT one may have to stay gated for its second, independent reason -- check
//  before deleting.  Retiring them changes behaviour only when the tray-map recipe
//  flags are on, and each of those gates documents its own direction.
//  A DECLARATION is still missing tree-wide: golden's home is ainarm2.h:216 and the
//  port has no ainarm2.h, so the integrator must publish
//    extern bool EnableTraymapCheckFunction(int iCheck=0);
//  (aHotPlateSubstrate.h's "-- functions (golden ainarm2.cpp)" block, next to
//  ResetInToShtFlag at :1066, is where its siblings live) before any of the five
//  call sites can be ungated.  I did not edit that header -- out of my boundary.
//    rg -c --glob '*.cpp' --glob '*.h' -w EnableTraymapCheckFunction D:/HT9045/HT9011UC_Cpp_V3.33.906.0
//    -> before this part: 3 files (ainarm9045.cpp / aTester_Rear.cpp /
//       aTester_Front.cpp), every hit inside gate text or a gated call; no
//       declaration, no body.  MEASURED AT 2026-08-11T06:33:56+0800.
//------------------------------------------------------------------------------
bool EnableTraymapCheckFunction(int iCheck)
{
    bool Result=false;

    if(iCheck==0)
    {
        if(bRTNoRunTrayID==false &&
           USE_TRAY_MAPPING!=etmUninstall &&
           (TestIF_File.bEnableTrayMap || (TestIF_File.bEnableTrayID && TestIF_File.bEnableTrayIDDownFTP)) &&
           TestIF_File.bEnableSuckMapCheck)                                     //wei 20170302 (Steven) Tray map data error
        {
            Result=true;
        }
    }
    else if(iCheck==1)
    {
        if(bRTNoRunTrayID==false &&
           USE_TRAY_MAPPING!=etmUninstall &&
           (TestIF_File.bEnableTrayMap || (TestIF_File.bEnableTrayID && TestIF_File.bEnableTrayIDDownFTP)) &&
           bTrayMapDataCheckErrorSkip)                                          //wei 20170302 Tray map data error
        {
            Result=true;
        }
    }
    return Result;
}

//==============================================================================
//  RestoreLoadeIC(int)  --  golden ainarm2.cpp:3660-3725
//
//  ROLE          End of the "try the last row once more" rescue (E62): when the
//                in-arm has reached the far row of the loader tray, clear
//                bTryPickLastRowIng, FLIP the tray scan Direction back (0<->3,
//                1<->2), and restore every cell the buffer still says had an IC
//                but the live map had degraded to NULL_IC -- excluding the row
//                just attempted.
//  WAVE SCOPE    RestoreLoadeIC()           ACTIVE   golden :3660-3725
//  GATE REGISTER (none -- every symbol has a real linkable home)
//                  IniConfig.bE62TryPickLastRow Config.h; bTryPickLastRowIng
//                    cmydef.h/cmydef.cpp (real global, not one of the 37).
//                  TrayForm.Loader.Direction cprod.h:1265; MOT[] Motor/mymotor.h
//                    :385 with Tray.XItem/.YItem mytray.h:33/:34, Tray.Data :40,
//                    Tray.BufferData :46, SetTraySingleData(int,int,int,int=0)
//                    Motor/mymotor.h:357; MMTrayY cmydef.h; HAS_IC/NULL_IC cmydef.h.
//
//  GOLDEN ODDITIES PRESERVED (deliberate, verified against golden, not tidied):
//    * :3692-3694 an EMPTY `else { }`.  Kept byte-for-byte.
//    * the two arms scan DIFFERENT row windows on purpose -- Direction 0/1 uses
//      `j < YItem-1` and Direction 2/3 uses `j = 1 ... YItem`.  Each excludes the
//      end row it has just finished retrying.  This asymmetry is the feature; do
//      not "symmetrise" it.
//    * golden's header declares a default argument (ainarm2.h:220
//      `RestoreLoadeIC(int iLoaderY=0)`); the DEFINITION carries none.  Kept as
//      golden has it -- the default belongs on the declaration only.
//  OFFLINE REACHABILITY: net-new symbol (no prior stub anywhere).  Its 1 live call
//    site is inert offline because IniConfig.bE62TryPickLastRow is false, so the
//    whole body is skipped -- landing it is behaviour-neutral offline.
//==============================================================================
void RestoreLoadeIC(int iLoaderY)                                               //JerryYang 20200422 Auto skip次數到達後, 自動再去最後一排吸吸看
{
    if(IniConfig.bE62TryPickLastRow && bTryPickLastRowIng==true)
    {
        if(TrayForm.Loader.Direction==0 || TrayForm.Loader.Direction==1)
        {
            if(iLoaderY==MOT[MMTrayY].Tray.YItem-1)
            {
                bTryPickLastRowIng=false;
                if(TrayForm.Loader.Direction==0)
                {
                    TrayForm.Loader.Direction=3;
                }
                else if(TrayForm.Loader.Direction==1)
                {
                    TrayForm.Loader.Direction=2;
                }

                for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
                {
                    for(int j=0; j<MOT[MMTrayY].Tray.YItem-1; j++)
                    {
                        if(MOT[MMTrayY].Tray.BufferData[i][j]==HAS_IC)
                        {
                            if(MOT[MMTrayY].Tray.Data[i][j]==NULL_IC)
                            {
                                MOT[MMTrayY].SetTraySingleData(i, j, HAS_IC);
                            }
                        }
                    }
                }
            }
            else
            {
            }
        }
        else
        {
            if(iLoaderY==0)
            {
                bTryPickLastRowIng=false;
                if(TrayForm.Loader.Direction==2)
                {
                    TrayForm.Loader.Direction=1;
                }
                else if(TrayForm.Loader.Direction==3)
                {
                    TrayForm.Loader.Direction=0;
                }
                for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
                {
                    for(int j=1; j<MOT[MMTrayY].Tray.YItem; j++)
                    {
                        if(MOT[MMTrayY].Tray.BufferData[i][j]==HAS_IC)
                        {
                            if(MOT[MMTrayY].Tray.Data[i][j]==NULL_IC)
                            {
                                MOT[MMTrayY].SetTraySingleData(i, j, HAS_IC);
                            }
                        }
                    }
                }
            }
        }
    }
}

//==============================================================================
//  VacuumOnOffLog(AnsiString sLog)                        golden ainarm2.cpp:3727-3744
//  Translation wave: PT-W7e   Translator: AI(k4-ainarm2) 20260811
//
//  ROLE       : appends one CSV row to the vacuum-generator on/off life log, bucketed
//               into a per-day directory and a per-hour file, writing the column
//               header first if that hour`s file does not exist yet.
//  WAVE SCOPE : VacuumOnOffLog(AnsiString)  golden :3727-3744  -- ACTIVE (no gate)
//  GATE REGISTER: none.  asVacuumOnOffLogPath (common.h:155, defined common.cpp:194 =
//               "D:\HT9045_log\VacuumOnOffLog"), MyForceDirectories (common.h:341),
//               WriteDataToFile (common.h:338), FileExists (vclcompat/SysUtils.h:90)
//               and SystemYear/SystemMonth/SystemDate/SystemHour (cmydef.h:227/:226,
//               type Word) all have real bodies.
//  WHY NOT GATED DESPITE WRITING UNDER D:\ (contrast GATE k4e-G02 in this same wave):
//               this is an APPEND-ONLY log under D:\HT9045_log, not a rewrite of
//               machine state.  It cannot destroy a taught value or a saved lot, and
//               the port already writes several sibling logs to the same tree
//               (asEPLogPath, asTorqLogPath, asBarCodeLogPath).  It also only runs
//               when a caller asks; there is no timer behind it.
//  TRAP 1     : shape (a) today -- no port declaration and no other definition (0
//               tree-wide), so no caller until golden ainarm2.h`s declaration lands.
//               Non-static on purpose.  Nothing to retire.
//  FIDELITY NOTE: golden passes raw AnsiString objects to the printf-family
//               (s.sprintf("%s", sLog) and Path.sprintf with asVacuumOnOffLogPath).
//               That is transcribed VERBATIM, with no .c_str() added:
//               vclcompat/AnsiString.h:142 is a variadic template whose conv()
//               overload (:177) converts an AnsiString argument to c_str() for
//               exactly this BCB6 idiom.  Adding .c_str() would be a gratuitous edit.
//  GOLDEN QUIRKS PRESERVED (not fixed here): s.sprintf("%s", sLog) is a pointless
//               copy of sLog; Path is formatted twice and the first value is used
//               only for MyForceDirectories; the header string
//               "Time,Use Site,Test Time, Vacuum On,Vacuum Off" has an inconsistent
//               space before "Vacuum On"; and the two-statement if-body at golden
//               :3739-3740 is indented with 3 spaces, not 4.
//
//  HOIST ME to ainarm2.cpp`s include head at integration:
#include "common.h"              // asVacuumOnOffLogPath :155, MyForceDirectories :341, WriteDataToFile :338
//==============================================================================
void VacuumOnOffLog(AnsiString sLog)
{
    AnsiString s;
    AnsiString Path, buffer;

    s.sprintf("%s", sLog);
    Path.sprintf("%s\\%04d_%02d_%02d", asVacuumOnOffLogPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path);

    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asVacuumOnOffLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
    if(FileExists(Path)==false)
    {
       buffer.sprintf("Time,Use Site,Test Time, Vacuum On,Vacuum Off");
       WriteDataToFile(Path, buffer);
    }

    WriteDataToFile(Path, s);
}

//==============================================================================
//  MoveInArm2XYToClean()  --  golden ainarm2.cpp:3746-3787
//
//  ROLE          Move the in-arm to the in-rotate (die-clean) station with every
//                nozzle commanded DOWN to the die-clean height: derive the X pitch
//                per axis from a fixed 12000 and Y from 6000 through the
//                InOfsRotate_In offset family, set bZFlag/iZPos for the whole
//                nozzle grid, and hand the move to InArmContinuousMove_9045 with
//                ZAxisDown.
//  WAVE SCOPE    MoveInArm2XYToClean()      ACTIVE   golden :3746-3787
//  GATE REGISTER (none -- every symbol has a real linkable home)
//                  GetInArmPitchX_9045(int,int,int=-1) ainarm9045.h:48;
//                    GetInArmPitchY_9045(int,int=-1) ainarm9045.h:50.
//                  InArmSuck -- TMyKitSuck.  ** TRAP 5 ** resolved via
//                    aHotPlateSubstrate.h:365, the copy 177 TUs use and the copy
//                    the port ainarm2.cpp ALREADY includes and reads
//                    (.iPickRow/.iPickCol/.Item/.SetItemData at port ainarm2.cpp
//                    :69-99).  NOT mykitsuck.h:274, whose layout differs; the
//                    object itself is defined in aHotPlateSubstrate.cpp.  Only
//                    .iMaxRow/.iMaxCol are read here.
//                  ArmSpeed_File[] cprod.h:2876, .dDieCleanHeight cprod.h:2860.
//                  Prod.iInArm_RotateX/Y cprod.h:802/:803 (int).
//                  InArmOffSet[] cprod.h:233, GetX()/GetY() cprod.h:197/:198.
//                  ZAxisDown aHotPlateSubstrate.h:738; InOfsRotate_In MachineType.h;
//                    CUSTOMER_CODE / CC_TSMC_TAINAN MachineType.h.
//                  InArmContinuousMove_9045 Motor/mymotor.h:396.
//
//  ** NULL-DATA HAZARD, DELIBERATELY NOT GUARDED ** InArmOffSet[] is defined at
//    cprod.cpp:67 as a bare array of ARM_OFFSET* and nothing in this tree ever
//    allocates its elements (golden allocates them at startup; offline they are
//    NULL -- open task #10).  So `InArmOffSet[InOfsRotate_In]->GetX()` here, and
//    the same deref inside GetInArmPitchX_9045/GetInArmPitchY_9045 via
//    ainarm9045.cpp:190-191, WILL segfault if this function is ever called before
//    task #10 lands.  No `if(ptr)` guard is added, for the reason the tree already
//    wrote down at AutoClean/AutoClean.cpp:4505-4507 and repeated for GATE
//    (W7a-I4)/(W7d-I1): this is a POSITION calculation, and silently dropping an
//    offset is a worse failure mode than crashing.  TRAP 4 is NOT triggered --
//    this file adds no file-scope object, so nothing runs before main().
//  SAFE TO LAND TODAY: this symbol has ZERO callers in the port (verified by grep,
//    20260811 06:35 local) and no prior stub, so nothing can reach the deref.  It
//    must NOT be wired to a caller before task #10.
//
//  INTEGER SEMANTICS: `iZPos[i][j]=-(ArmSpeed_File[InArm].dDieCleanHeight*100)`
//    keeps golden's double multiply then narrows on store, and the TSMC_TAINAN arm
//    keeps golden's sign flip plus its `0+(...)` on the other arm.  Copied exactly,
//    including the redundant `0+`.
//==============================================================================
bool MoveInArm2XYToClean()
{
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    int iMovePitchX=12000;
    int iMovePitchY=6000;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20111226 : In Arm 咖啡杯解除
    {
        if(fNote->bMyServoOffInArm)
        {
            return false;
        }
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetInArmPitchX_9045(iMovePitchX, i, InOfsRotate_In);
    iYVariable=GetInArmPitchY_9045(iMovePitchY, InOfsRotate_In);

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            bZFlag[i][j]=true;
            if(CUSTOMER_CODE==CC_TSMC_TAINAN)
                iZPos[i][j]=-(ArmSpeed_File[InArm].dDieCleanHeight*100);
            else
                iZPos[i][j]=0+(ArmSpeed_File[InArm].dDieCleanHeight*100);
        }
    }

    if(InArmContinuousMove_9045(Prod.iInArm_RotateX+InArmOffSet[InOfsRotate_In]->GetX(), Prod.iInArm_RotateY+InArmOffSet[InOfsRotate_In]->GetY(), iXVariable, iYVariable, bZFlag, iZPos, ZAxisDown))
    {
        return true;
    }
    return false;
}

//==============================================================================
//  InitInDieCleanTask()  --  golden ainarm2.cpp:3794-3797
//
//  ROLE          Reset the in-arm die-clean cursor to 1.
//  WAVE SCOPE    InitInDieCleanTask()       ACTIVE   golden :3794-3797
//  GATE REGISTER (none)
//                  InDieCleanTask -- golden ainarm2.cpp:3791 `int InDieCleanTask=1;`
//                    is one of the 37 file-scope globals the INTEGRATOR adds; it is
//                    NOT emitted here, only used.  Verified absent today: the only
//                    tree hit for the string is ainarm9045.cpp:1169, which is the
//                    substring inside `InitInDieCleanTask();` -- checked, not
//                    assumed (grep -rn "InDieCleanTask", 20260811 06:36 local).
//                  DoInDieCleanDelay (golden :3792) is NOT touched by this range.
//                    Note for the integrator: golden types it HTimer, and per the
//                    wave brief it must be declared TQPF_Timer instead, because the
//                    port's only HTimer (atester_shims.h:463) has Off() hard-coded
//                    true and would zero every die-clean dwell.
//
//  NET-NEW, AND CURRENTLY UNREACHABLE: no prior stub anywhere.  Its single call
//    site, ainarm9045.cpp:1169, sits INSIDE `#if 0 // TODO(W7)` (that gate opens at
//    ainarm9045.cpp:1166), so this body has zero live callers and cannot change
//    offline behaviour.  It is groundwork for the DoInDieClean SM, not a live fix;
//    do not report it as an activated feature.
//==============================================================================
void InitInDieCleanTask()                                                       //wei 20170418
{
    InDieCleanTask=1;
}

// =============================================================================
//  AI(W906-PT-W7e-k8) 20260811 -- golden ainarm2.cpp translation part
//  golden file : D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/ainarm2.cpp (cp950, CRLF)
//  this part   : golden lines 3799..3869
// -----------------------------------------------------------------------------
//  ROLE        : the InArm Die-Clean state machine: Z to plate-safe, X/Y to the clean position (with an optional AccessLevel>=1 manual-key pause/offset detour), settle, open the clean blow plus suck for the configured delay, Z back to safe, close them and report done.
//  WAVE SCOPE  : DoInDieClean -- ACTIVE with ONE gated condition, golden ainarm2.cpp:3799..3869.
//  COLLISION    : none -- no port body today.  Its Init sibling InitInDieCleanTask
//              : (golden :3794) is NOT part of this part file.
//  GLOBALS      : InDieCleanTask (golden :3791) and DoInDieCleanDelay (golden :3792)
//              : are integrator-owned file-scope globals and are NOT emitted here.
//              : DoInDieCleanDelay is golden-typed HTimer; the integrator declares it
//              : TQPF_Timer instead, and that is the right call -- the port's only
//              : HTimer (atester_shims.h:463) has `Off()` hard-coded to `return true`,
//              : which would silently zero BOTH dwells in this SM: the 500 ms settle at
//              : case 200 and the whole dDieCleanDelay blow at case 400.  TQPF_Timer
//              : (myTimer.h:10) really does provide `void SetMSAndOn(DWORD)` at :39 and
//              : `bool Off()` at :30, which is exactly what this body calls.
//  INCLUDES     : this body needs, beyond what port ainarm2.cpp already includes
//              : (cmydef.h / MachineType.h / cprod.h / ainarm9045.h /
//              : aHotPlateSubstrate.h):
//              : mysensor.h (Sen[] :48, IsOn :42), myswitch.h (SW[] :43, On :32, Off :33), forms/fMain.h (fMain->Pause :142), myTimer.h (TQPF_Timer) -- MoveInArmZToPlateSafe is aHotPlateSubstrate.h:901, already included
//  GATE REGISTER: ONE gate, W7E-K8-C, on golden :3813 -- full WHAT/WHY/DELTA plus the
//              : exact absence command and the time it was run are inline at the gate.
//  ACTIVE DEPS  : AccessLevel cmydef.h:3503, SnRKManualTStart cmydef.h:702,
//              : bEnterOffset cmydef.h:2734, SwDieClean cmydef.h:1875 (=187,
//              : cmydef.cpp:2089), SwDieCleanSuck cmydef.h:1876 (=297,
//              : cmydef.cpp:2219), ArmSpeed_File cprod.h:2876 whose dDieCleanDelay is
//              : cprod.h:2859 (double), InArm MachineType.h:682 (eSpeedPart, =0).
//  SIGNATURE    : golden's `fMain->Pause("DoInDieClean")` PASSES an argument and the
//              : port's TfMain::Pause takes `AnsiString Func` (forms/fMain.h:142), so
//              : the call is transcribed VERBATIM.  Do NOT swap the literal for
//              : __FUNC__ the way aoutarm9045S_1x4_4.cpp had to -- that substitution
//              : was for golden call sites that take NO argument.  Offline Pause()
//              : returns false and never pauses, but it does record
//              : W906_PauseCallCount / W906_PauseLastFunc, so the literal
//              : "DoInDieClean" is observable from tests.
//  DOUBLE->DWORD: golden :3846 `SetMSAndOn(ArmSpeed_File[InArm].dDieCleanDelay*1000)`
//              : converts a double to DWORD, truncating; negative or huge values wrap.
//              : Verbatim -- not clamped, not rounded.
// =============================================================================
bool DoInDieClean()                                                             //wei 20170418
{
    int &Task=InDieCleanTask;
    bool Result=false;

    switch(Task)
    {
        case 1:                                                                 // z safe
            if(MoveInArmZToPlateSafe(199))
            {
                Task=100;
            }
            break;
        case 100:                                                               //xy move clean pos
//------------------------------------------------------------------------------
//  GATE W7E-K8-C -- golden ainarm2.cpp:3813
//  WHAT  : case 100 waits for MoveInArm2XYToClean() -- move the in-arm X/Y onto the
//  WHAT  : taught Die-Clean position -- before it starts the blow/suck sequence.
//  WHY   : MoveInArm2XYToClean has NO port body and NO port declaration.  Command,
//  WHY   : run from D:/HT9045/HT9011UC_Cpp_V3.33.906.0 at 2026-08-11 06:33:21 local:
//  WHY   :     rg -n --no-messages -g '*.cpp' -g '*.h' 'MoveInArm2XYToClean' .
//  WHY   : -> zero hits, exit status 1.
//  WHY   : TRAP 2 -- THIS ABSENCE CAN EXPIRE INSIDE THIS SAME WAVE: golden's body is
//  WHY   : ainarm2.cpp:3746, i.e. THIS file, 53 lines above this call and just outside
//  WHY   : this agent's assigned ranges.  Re-run the command above at integration.
//  WHY   : TRAP 3 -- if it HAS landed, retiring is correct here and needs no prototype:
//  WHY   : golden has none either (ainarm2.h does not declare it -- golden relies on
//  WHY   : :3746 preceding :3813 inside one file), and a part file named 03746_* stitches
//  WHY   : ahead of this one.  If it has NOT landed, the gate stays.
//  WHY   : RE-CHECKED, AND IT HAS NOT LANDED.  At 2026-08-11T06:43:57+0800 I re-ran
//  WHY   : the command above (still zero hits, exit 1) AND listed the whole part
//  WHY   : directory: 50 part files, none named 03746_*, and
//  WHY   :     rg -n 'MoveInArm2XYToClean' D:/HT9045/HT9011UC_Cpp_V3.33.906.0/_w7e_parts
//  WHY   : matched only this part's own banner and gated line.  Two of my three
//  WHY   : original absence claims DID expire in that window (ChangeHotPlateData in
//  WHY   : part 00654 and the caller of PreciserPitchCalculate in part 03423) and
//  WHY   : both were fixed before shipping -- this one genuinely did not.
//  WHY   : Re-run it once more at integration anyway.
//  DELTA : `false` means the XY move NEVER reports complete, so case 100 never advances
//  DELTA : and DoInDieClean never returns true.  THIS IS THE DELIBERATE FAIL-LOUD
//  DELTA : CHOICE, and the reasoning is not re-derived here: it is exactly the argument
//  DELTA : csystem.cpp:20296-20320 GATE G01 already wrote for the ion-fan twin --
//  DELTA : defaulting the move to 'reached' would run the actuator sequence with the arm
//  DELTA : parked at WHATEVER X/Y it happened to be at, i.e. open SwDieClean and
//  DELTA : SwDieCleanSuck over an untaught position and then report a clean that cleaned
//  DELTA : nothing.  A visible stall beats a silent wrong-position air blast.
//  DELTA : Not reachable today either way: the only caller, ainarm9045.cpp:1194, is
//  DELTA : itself inside the `#if 0 // TODO(W7)` block opened at ainarm9045.cpp:1192.
//  DELTA : If that caller is ever ungated while this gate is still in place, DoInArm's
//  DELTA : case 10000 will hang -- ungate the two together, or not at all.
#if 0 // GATE W7E-K8-C -- golden ainarm2.cpp:3813 (text below is golden VERBATIM)
            if(MoveInArm2XYToClean())
#else
            if(false)                                                           // GATE W7E-K8-C: Die-Clean XY move never reports complete
#endif
            {
                if(AccessLevel>=1 && Sen[SnRKManualTStart].IsOn())
                {
                    bEnterOffset=false;
                    fMain->Pause("DoInDieClean");
                    Task=120;
                    break;
                }
                DoInDieCleanDelay.SetMSAndOn(500);
                Task=200;
            }
            break;
        case 120:
            if(bEnterOffset==true)
            {
                bEnterOffset=false;
                Task=1;
            }
            else
            {
                Task=100;
            }
            break;
        case 200:                                                               //到位延遲
            if(DoInDieCleanDelay.Off())
            {
                Task=300;
            }
            break;
        case 300:                                                               //開吹氣
            SW[SwDieClean].On();                                                //wei 20170525
            SW[SwDieCleanSuck].On();                                            //wei 20210326
            DoInDieCleanDelay.SetMSAndOn(ArmSpeed_File[InArm].dDieCleanDelay*1000);
            Task=400;
            break;
        case 400:                                                               //吹氣延遲
            if(DoInDieCleanDelay.Off())
            {
                Task=500;
            }
            break;
        case 500:
            if(MoveInArmZToPlateSafe(599))
            {
                Task=600;
            }
            break;
        case 600:                                                               //關吹氣
            SW[SwDieClean].Off();                                               //wei 20170525
            SW[SwDieCleanSuck].Off();                                           //wei 20210326
            Result=true;
            break;
    }

    return Result;
}
